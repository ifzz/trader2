
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <signal.h>

#include <event2/bufferevent.h>
#include <event2/util.h>
#include <event2/event.h>
#include <event2/buffer.h>

#include "hiredis.h"

#include "cmn_log.h"
#include "trader_data.h"

#include "trader_mduser_api.h"
#include "trader_mduser_cnn.h"
#include "trader_mduser_boardcast.h"

#include "trader_mduser_svr.h"

#define MDUSER_BOARDCAST_IP "MDUSER.BOARDCAST.IP"
#define MDUSER_BOARDCAST_PORT "MDUSER.BOARDCAST.PORT"

#define MDUSER_CNN_MAIN_BROKER_ID "MDUSER.CNN.MAIN.BROKER.ID"
#define MDUSER_CNN_MAIN_USER "MDUSER.CNN.MAIN.USER"
#define MDUSER_CNN_MAIN_PASSWD "MDUSER.CNN.MAIN.PASSWD"
#define MDUSER_CNN_MAIN_ADDR "MDUSER.CNN.MAIN.ADDR"

#define MDUSER_CNN_BACKUP_BROKER_ID "MDUSER.CNN.BACKUP.BROKER.ID"
#define MDUSER_CNN_BACKUP_USER "MDUSER.CNN.BACKUP.USER"
#define MDUSER_CNN_BACKUP_PASSWD "MDUSER.CNN.BACKUP.PASSWD"
#define MDUSER_CNN_BACKUP_ADDR "MDUSER.CNN.BACKUP.ADDR"

static int trader_mduser_svr_init(trader_mduser_svr* self);
static int trader_mduser_svr_run(trader_mduser_svr* self);
static int trader_mduser_svr_proc(trader_mduser_svr* self, trader_mduser_evt* evt);

static trader_mduser_svr_method* trader_mduser_svr_method_get();

//CALLBACK
static void trader_mduser_svr_signal_cb(evutil_socket_t fd, short event, void *arg);
static void trader_mduser_svr_tick_cb(void* arg, trader_mduser_evt* evt);

static int trader_mduser_svr_init_cnn(trader_mduser_svr* self);

static int trader_mduser_svr_init_boardcast(trader_mduser_svr* self);

static int trader_mduser_svr_redis_get(trader_mduser_svr* self const char* key, char* val, int size);

trader_mduser_svr_method* trader_mduser_svr_method_get()
{
  static trader_mduser_svr_method st_trader_mduser_svr_method = {
    trader_mduser_svr_init,
    trader_mduser_svr_run,
    trader_mduser_svr_proc
  };

  return &st_trader_mduser_svr_method;
}

int trader_mduser_svr_init_cnn(trader_mduser_svr* self)
{

  return 0;
}

int trader_mduser_svr_init_boardcast(trader_mduser_svr* self)
{
  char ip[32];
  char port[6];
  int nRet = 0;
  nRet = trader_mduser_svr_redis_get(self, MDUSER_BOARDCAST_IP, ip, sizeof(ip));
  nRet = trader_mduser_svr_redis_get(self, MDUSER_BOARDCAST_PORT, port, sizeof(port));

  self->pBoardcast->method->xInit(self->pBoardcast, self->pBase,
    ip, atoi(port));
  
  return 0;
}

int trader_mduser_svr_redis_get(trader_mduser_svr* self, const char* key, char* val, int size)
{
  int nRet = -1;
  redisReply* reply = (redisReply*)redisCommand(self->pRedisCtx, "GET %s", key);
  do {
    if(REDIS_REPLY_STRING == reply->type){
      nRet = 0;
      strncpy(val, reply->str, size);
      break;
    }
  }while(0);
  freeReplyObject(reply);

  return nRet;
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
   
  CMN_DEBUG("self->pCnnMain\n");
  self->pCnnMain = trader_mduser_cnn_new();
  CMN_ASSERT(self->pCnnMain);
  
  CMN_DEBUG("self->pCnnBackup\n");
  self->pCnnBackup= trader_mduser_cnn_new();
  CMN_ASSERT(self->pCnnBackup);

  CMN_DEBUG("self->pBoardcast\n");
  self->pBoardcast = trader_mduser_boardcast_new();
  CMN_ASSERT(self->pBoardcast);


  CMN_DEBUG("self->pRedisCtx\n");
  struct timeval tv = {
    5, 0;
  };
  self->pRedisCtx = redisConnectWithTimeout("localhost", 6379, &tv);
  CMN_ASSERT(self->pRedisCtx);

  CMN_ASSERT(0 == self->pRedisCtx->err);

  nRet = trader_mduser_svr_init_cnn(self);
  CMN_ASSERT(0 == nRet);
  
  nRet = trader_mduser_svr_init_boardcast(self);
  CMN_ASSERT(0 == nRet);
  
  return 0;
}

int trader_mduser_svr_run(trader_mduser_svr* self)
{
  int nRet;
  
  nRet = event_add(self->pSigIntEvt, NULL);
  CMN_ASSERT(nRet >= 0);
  
  nRet = event_add(self->pSigTermEvt, NULL);
  CMN_ASSERT(nRet >= 0);

  trader_mduser_api* pMduserApi = self->pMduserApi;
  pMduserApi->pMethod->xStart(pMduserApi);
  
  nRet = event_base_dispatch(self->pBase);

  pMduserApi->pMethod->xStop(pMduserApi);
  
  return 0;
}

int  trader_mduser_svr_proc(trader_mduser_svr* self, trader_mduser_evt* evt)
{
  //TODO

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

void trader_mduser_svr_tick_cb(void* arg, trader_mduser_evt* evt)
{
  trader_mduser_svr* self = (trader_mduser_svr*)arg;
  int nRet = self->pMethod->xProc(self, evt);
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


