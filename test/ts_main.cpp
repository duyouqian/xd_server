#include "base_object.h"
#include "singleton.h"
#include "log.h"
#include "atomic_counter.h"
#include "socket_util.h"
#include <deque>
#include <functional>

class Obj1 : public XDBaseObject
{
public:
    int32 getID() const { return ID_; }
};

class Obj2 : public XDBaseObject
{
public:
    int32 getID() const { return ID_; }
};

class Obj3 : public XDBaseObject, public XDSingleton<Obj3>
{
public:
    int32 getID() const { return ID_; }
};

class Obj4
{
public:
    explicit Obj4(int32 id) : ID_(id) { }
    int32 getID() { return ID_; }
private:
    int32 ID_;
};

class Obj5
{
public:
    explicit Obj5(int32 id) : ID_(id) { }
    ~Obj5()
    {
        //printf("[Obj5] delete\n");
        XD_LOG_mdebug("[Obj5] delete");
    }
    int32 getID() { return ID_; }
private:
    int32 ID_;
};

void fun1(Obj5 &obj)
{
    XD_LOG_mdebug("[FUN1] Obj4::ID=%d", obj.getID());
}

int32 main(int32 argc, char **argv)
{
    XD_LOG_OPEN("log", 0, 3);
    XD_LOG_minfo("Obj3ID:%d", Obj3::getInstance().getID());
    Obj1 t1;
    Obj2 t2;
    XD_LOG_minfo("Obj1ID:%d", t1.getID());
    XD_LOG_minfo("Obj2ID:%d", t2.getID());
    XD_LOG_minfo("Obj3ID:%d", Obj3::getInstance().getID());
    
    std::deque<Obj4> t;
    {
        Obj4 t3(10);
        t.push_back(t3);
    }
    
    XDAtomicCounter counter;
    XD_LOG_mdebug("counter=%d, ++counter = %d", counter.value(), ++counter);
    XD_LOG_mdebug("counter=%d, --counter = %d", counter.value(), --counter);
    XD_LOG_mdebug("counter=%d, counter++ = %d", counter.value(), counter++);
    XD_LOG_mdebug("counter=%d, counter-- = %d", counter.value(), counter--);
    
    uint64 t4 = 1234;
    XD_LOG_mdebug("t4=%u toNetWork=%u, toHost=%u", t4, XDSocketUtil::hostToNetwork32(t4), XDSocketUtil::networkToHost32(3523477504));
    
    std::function<void()> fun;

    {
        Obj5 t5(10);
        fun = std::bind(fun1, t5);
    }

    fun();

    XD_LOG_CLOSE();
    return 0;
}