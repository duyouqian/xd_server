#include "app.h"
#include "../base/log.h"
#include "../net/socket_handler.h"
#include "tinyxml2.h"

#include <vector>
#include <string.h>

enum XDHostType
{
    HT_MASTER = 1,
    HT_FRONTED,
    HT_RPC
};

struct XDHostInfo
{
    XDHostType type;
    const char *host;
    int32 port;
};

class XDServerNodeConfig
{
public:
    XDServerNodeConfig() { }
    ~XDServerNodeConfig() { }

    bool parse(tinyxml2::XMLElement *node)
    {
        node_ = node;
        if (node_->QueryBoolAttribute("isMaster", &isMaster_)) {
            return false;
        }
        if (node_->QueryBoolAttribute("isFronted", &isFronted_)) {
            return false;
        }
        tinyxml2::XMLElement *subItem = node_->FirstChildElement("node");
        while (subItem) {
            XDHostInfo temp;
            if (subItem->QueryIntAttribute("type", (int*)&temp.type)) {
                return false;
            }
            if (!(temp.host = subItem->Attribute("host"))) {
                return false;
            }
            if (subItem->QueryIntAttribute("port", (int*)&temp.port)) {
                return false;
            }
            subNodeInfo_.push_back(temp);
            subItem = subItem->NextSiblingElement();
        }
        return true;
    }

    bool isMaster() const { return isMaster_; }
    bool isFronted() const { return isFronted_; }
    std::vector<XDHostInfo>& hostInfo() { return subNodeInfo_; }

private:
    tinyxml2::XMLElement *node_;
    bool isMaster_;
    bool isFronted_;
    std::vector<XDHostInfo> subNodeInfo_;
};

XDApp::XDApp()
     : isMaster_(false)
     , isFronted_(false)
     , isStopped_(true)
     , netService_(*this)
     , sessionService_(*this)
{

}

XDApp::~XDApp()
{
    stop();
}

bool XDApp::init(int32 argc, char **argv)
{
    XD_LOG_OPEN("log", 0, 3);
    // path
    tinyxml2::XMLDocument doc;
    doc.LoadFile("config.xml");
    tinyxml2::XMLElement *root = doc.RootElement();
    tinyxml2::XMLElement *item = root->FirstChildElement("server");
    XDServerNodeConfig node;
    bool isOk = false;
    while (item) {
        // type
        const tinyxml2::XMLAttribute *attrib = item->FirstAttribute();
        if (attrib) {
            if (0 == strcmp(attrib->Value(), type_.c_str())) {
                if (node.parse(item)) {
                    isOk = true;
                    break;
                }
            }
        }
        item = item->NextSiblingElement();
    }

    if (!isOk) {
        return false;
    }

    isMaster_ = node.isMaster();
    isFronted_ = node.isFronted();
    startServices();
    return true;
}

void XDApp::run()
{
    isStopped_ = false;
    afterServices();
    while (!isStopped_) {
        ioService_.run();
    }
    XD_LOG_CLOSE();
}

void XDApp::stop()
{
    if (!isStopped_) {
        stopServices();
        isStopped_ = true;
        ioService_.wakeup();
    }
}

// net service
XDSocketConnecterPtr XDApp::connect(const char *host, int32 port, XDTcpClientSocketEventHandler *handler, bool isReconnect, int32 maxReconnectAttempts)
{
    netService_.connect(host, port, handler, isReconnect, maxReconnectAttempts);
}

bool XDApp::registerService(XDBaseService *service)
{
    if (!service) {
        return false;
    }
    if (hasService(service->name())) {
        return false;
    }
    services_.push_back(service);
    std::pair<std::map<std::string, int32>::iterator, bool> res = serviceMap_.insert(std::make_pair(service->name(), services_.size() - 1));
    return res.second;
}

bool XDApp::hasService(const std::string &name) const
{
//    if (isMaster() && strcmp(name.c_str(), XD_MASTER_SERVICE_NAME) == 0) {
//        return false;
//    }
    return serviceMap_.find(name) != serviceMap_.end();
}

void XDApp::startServices()
{
    for (std::vector<XDBaseService*>::iterator it = services_.begin(); it != services_.end(); ++it) {
        (*it)->start();
    }
}

void XDApp::afterServices()
{
    for (std::vector<XDBaseService*>::iterator it = services_.begin(); it != services_.end(); ++it) {
        (*it)->afterStart();
    }
}

void XDApp::stopServices()
{
    for (std::vector<XDBaseService*>::iterator it = services_.begin(); it != services_.end(); ++it) {
        (*it)->stop();
    }
}
