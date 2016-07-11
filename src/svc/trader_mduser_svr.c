
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <signal.h>

#include <event2/bufferevent.h>
#include <event2/util.h>
#include <event2/event.h>
#include <event2/buffer.h>

#include "trader_mduser_svr.h"

#include "log/cmn_log.h"

#include "api/trader_mduser_api.h"
#include "api/trader_mduser_api_lts.h"

static int trader_mduser_svr_set_user(trader_mduser_svr* self, char* broker_id, char* user, char* pwd);
static int trader_mduser_svr_set_front_addr(trader_mduser_svr* self, char* addr);
static int trader_mduser_svr_set_workspace(trader_mduser_svr* self, char* ws);
static int trader_mduser_svr_add_contract(trader_mduser_svr* self, char* contract);
static int trader_mduser_svr_run(trader_mduser_svr* self);
static int trader_mduser_svr_init(trader_mduser_svr* self);
static int trader_mduser_svr_proc(trader_mduser_svr* self, trader_mduser_evt* evt);

static trader_mduser_svr_method* trader_mduser_svr_method_get();

//CALLBACK
static void trader_mduser_svr_signal_cb(evutil_socket_t fd, short event, void *arg);
static void trader_mduser_svr_mduser_read_cb(struct bufferevent *bev, void *arg);
static void trader_mduser_svr_mduser_evt_cb(struct bufferevent *bev, short event, void *arg);


trader_mduser_svr_method* trader_mduser_svr_method_get()
{
  static trader_mduser_svr_method st_trader_mduser_svr_method = {
    trader_mduser_svr_set_user,
    trader_mduser_svr_set_front_addr,
    trader_mduser_svr_set_workspace,
    trader_mduser_svr_add_contract,
    trader_mduser_svr_run,
    trader_mduser_svr_init,
    trader_mduser_svr_proc
  };

  return &st_trader_mduser_svr_method;
}

int trader_mduser_svr_set_user(trader_mduser_svr* self, char* broker_id, char* user, char* pwd)
{
  trader_mduser_api* pApi = self->pMduserApi;
  pApi->pMethod->xSetUser(pApi, broker_id, user, pwd);

  return 0;
}

int trader_mduser_svr_set_front_addr(trader_mduser_svr* self, char* addr)
{
  trader_mduser_api* pApi = self->pMduserApi;
  pApi->pMethod->xSetFrontAddr(pApi, addr);

  return 0;
}

int trader_mduser_svr_set_workspace(trader_mduser_svr* self, char* ws)
{
  trader_mduser_api* pApi = self->pMduserApi;
  pApi->pMethod->xSetWorkspace(pApi, ws);

  return 0;
}

int trader_mduser_svr_add_contract(trader_mduser_svr* self, char* contract)
{
  
  trader_mduser_api* pApi = self->pMduserApi;
  pApi->pMethod->xSubscribe(pApi, contract);

  return 0;
}

int trader_mduser_svr_run(trader_mduser_svr* self)
{
  int nRet;
  
  nRet = event_add(self->pSigIntEvt, NULL);
  CMN_ASSERT(nRet >= 0);
  
  nRet = event_add(self->pSigTermEvt, NULL);
  CMN_ASSERT(nRet >= 0);

  bufferevent_enable(self->pBufMduser, EV_READ|EV_PERSIST);

  trader_mduser_api* pMduserApi = self->pMduserApi;
  pMduserApi->pMethod->xStart(pMduserApi);
  
  nRet = event_base_dispatch(self->pBase);

  pMduserApi->pMethod->xStop(pMduserApi);
  
  return 0;
}

int trader_mduser_svr_init(trader_mduser_svr* self)
{
  int nRet;
  evutil_socket_t pair[2];

  // 各种初始化
  CMN_DEBUG("self->pBase\n");
  self->pBase = event_base_new();
  CMN_ASSERT(self->pBase);
  
  //signal event init
  CMN_DEBUG("self->pSigIntEvt\n");
  self->pSigIntEvt = evsignal_new(self->pBase, SIGINT, trader_mduser_svr_signal_cb, (void *)self);
  CMN_ASSERT(self->pSigIntEvt);
  
  CMN_DEBUG("self->pSigTermEvt\n");
  self->pSigTermEvt = evsignal_new(self->pBase, SIGTERM, trader_mduser_svr_signal_cb, (void *)self);
  CMN_ASSERT(self->pSigTermEvt);
  
  //self->BufMduser
  CMN_DEBUG("self->pBufMduser\n");
  nRet = evutil_socketpair(AF_UNIX, SOCK_STREAM, 0, pair);
  CMN_ASSERT(nRet >= 0);
  
  self->pBufMduser = bufferevent_socket_new(self->pBase, pair[0], BEV_OPT_CLOSE_ON_FREE);
  bufferevent_setcb(self->pBufMduser, trader_mduser_svr_mduser_read_cb, 
    NULL, trader_mduser_svr_mduser_evt_cb, (void*)self);
 
  CMN_DEBUG("self->pMduserApi\n");
  self->pMduserApi = trader_mduser_api_new(pair[1], trader_mduser_api_lts_method_get());


  return 0;
}

int  trader_mduser_svr_proc(trader_mduser_svr* self, trader_mduser_evt* evt)
{

}

void trader_mduser_svr_signal_cb(evutil_socket_t fd, short event, void *arg)
{
  trader_mduser_svr* self = (trader_mduser_svr*)arg;
  struct event_base *base = self->pBase;
  struct timeval delay = { 1, 0 };

  event_del(self->pSigIntEvt);
  event_del(self->pSigTermEvt);

  event_base_loopexit(base, &delay);
  
}

void trader_mduser_svr_mduser_read_cb(struct bufferevent *bev, void *arg)
{
  CMN_DEBUG("Enter!\n");
  trader_mduser_svr* self = (trader_mduser_svr*)arg;

  int nRet = 0;
  int nSize = 0;
  int loop = 1;
  trader_mduser_evt oMsg;
  do {
    nRet = cmn_util_bufferevent_recv(self->pBufMduser, (unsigned char*)&oMsg, sizeof(oMsg));
    if(nRet <= 0){
      loop = 0;
      break;
    }

    nRet = self->pMethod->xProc(self, &oMsg);
    
  }while(loop);

  return ;
}

void trader_mduser_svr_mduser_evt_cb(struct bufferevent *bev, short event, void *arg)
{
  CMN_ERROR("Cannot reach here!\n");
}


trader_mduser_svr* trader_mduser_svr_new()
{
  trader_mduser_svr* self = (trader_mduser_svr*)malloc(sizeof(trader_mduser_svr));
  self->pMethod = trader_mduser_svr_method_get();
  return self;
}

void trader_mduser_svr_free(trader_mduser_svr* self)
{
  if(self) {
    if(self->pMduserApi) {
      trader_mduser_api_free(self->pMduserApi);
    }

    if(self->pBufMduser) {
      bufferevent_free(self->pBufMduser);
    }

    if(self->pSigTermEvt) {
      event_free(self->pSigTermEvt);
    }

    if(self->pSigIntEvt) {
      event_free(self->pSigIntEvt);
    }

    if(self->pBase) {
      event_base_free(self->pBase);
    }    
    
    free(self);
  }

}

int main(int argc, char* argv[])
{
  return 0;
}


