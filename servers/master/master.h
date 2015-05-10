#ifndef XD_MASTER_H
#define XD_MASTER_H

#include "app.h"
#include "singleton.h"

class Master : public XDSingleton<Master>
{
public:
  Master();
  ~Master();

//  bool init(int32 argc, char **argv);
//  void run();

private:
  XDApp app_;

};

#endif // end xd_master_h