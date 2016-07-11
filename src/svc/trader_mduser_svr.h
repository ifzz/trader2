
#ifndef _TRADER_MDUSER_SVR_H_
#define _TRADER_MDUSER_SVR_H_

#include "api/trader_data.h"
#include "api/trader_mduser_api.h"

typedef struct trader_mduser_svr_def trader_mduser_svr;
typedef struct trader_mduser_svr_method_def trader_mduser_svr_method;
struct trader_mduser_svr_def {
  struct event_base *pBase;
  struct event* pSigIntEvt;
  struct event* pSigTermEvt;

  // 对行情线程
  struct bufferevent* pBufMduser;

  trader_mduser_api* pMduserApi;
  
  trader_mduser_svr_method* pMethod;

};

struct trader_mduser_svr_method_def {
  int (*xSetUser)(trader_mduser_svr* self, char* broker_id, char* user, char* pwd);
  int (*xSetFrontAddr)(trader_mduser_svr* self, char* addr);
  int (*xSetWorkspace)(trader_mduser_svr* self, char* ws);
  int (*xAddContract)(trader_mduser_svr* self, char* contract);

  int (*xRun)(trader_mduser_svr* self);
  int (*xInit)(trader_mduser_svr* self);
  int (*xProc)(trader_mduser_svr* self, trader_mduser_evt* evt);
};


extern trader_mduser_svr* trader_mduser_svr_new();
extern void trader_mduser_svr_free(trader_mduser_svr* self);

#endif // _TRADER_MDUSER_SVR_H_

