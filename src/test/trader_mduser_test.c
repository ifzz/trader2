
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <signal.h>

#include <event2/util.h>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>

#include "trader_data.h"
#include "trader_mduser_api.h"

#define FEMAS // FEMAS LTS CTP

typedef struct trader_mduser_test_def trader_mduser_test;
struct trader_mduser_test_def {

  struct event_base *pBase;
  struct event* pSigIntEvt;
  struct event* pSigTermEvt;

  // 对行情线程
  struct bufferevent* pBufMduser;
  struct bufferevent* pBufStdin;

  trader_mduser_api* pApi;

};


static void test_signal_cb(evutil_socket_t fd, short event, void *arg);
static void test_trader_read_cb(struct bufferevent *bev, void *arg);
static void test_stdin_read_cb(struct bufferevent *bev, void *arg);

static void test_trader_evt_cb(struct bufferevent *bev, short event, void *arg);
static void test_stdin_evt_cb(struct bufferevent *bev, short event, void *arg);

static void test_trader_proc(trader_mduser_test* test, trader_mduser_evt* evt);

static trader_mduser_test* trader_mduser_test_get();

trader_mduser_test* trader_mduser_test_get()
{
  static trader_mduser_test trader_mduser_test_st;
  return &trader_mduser_test_st;
}

void test_signal_cb(evutil_socket_t fd, short event, void *arg)
{
  trader_mduser_test* test = (trader_mduser_test*)arg;
  struct timeval tv = {
    2, 0
  };
  event_base_loopexit(test->pBase, &tv);

}

void test_trader_read_cb(struct bufferevent *bev, void *arg)
{
  trader_mduser_test* test = (trader_mduser_test*)arg;

  int nRet = 0;
  int nSize = 0;
  int loop = 1;
  trader_mduser_evt oMsg;
  do {
    nRet = cmn_util_bufferevent_recv(test->pBufMduser, (unsigned char*)&oMsg, sizeof(oMsg));
    if(nRet <= 0){
      loop = 0;
      break;
    }

    test_trader_proc(test, &oMsg);
  }while(loop);

}

void test_stdin_read_cb(struct bufferevent *bev, void *arg)
{
  trader_mduser_test* test = (trader_mduser_test*)arg;
  struct evbuffer* pEvBuf = bufferevent_get_input(bev);
  int ret = evbuffer_get_length(pEvBuf);
  char* buff = (char*)malloc(ret);
  char ch;
  
  
  ret = evbuffer_remove(pEvBuf, buff, ret);
  
  struct timeval tv = {
    2, 0
  };

  int i;
  for(i = 0; i < ret; i++) {
    ch = buff[i];

    switch(ch){
    case '0':
      test->pApi->pMethod->xLogin(test->pApi);
      break;
    case '1':
      test->pApi->pMethod->xLogout(test->pApi);
      break;
    case '2':
      test->pApi->pMethod->xSubscribe(test->pApi, "11000942");
      break;
    case '8':
      event_base_loopexit(test->pBase, &tv);
      break;    
    }

    printf("\ninput:\n");

  }

  
  return;
}

void test_trader_evt_cb(struct bufferevent *bev, short event, void *arg)
{
  trader_mduser_test* test = (trader_mduser_test*)arg;
}

void test_stdin_evt_cb(struct bufferevent *bev, short event, void *arg)
{
  trader_mduser_test* test = (trader_mduser_test*)arg;
}

void test_trader_proc(trader_mduser_test* test, trader_mduser_evt* evt)
{
  if(ONRTNDEPTHMARKETDATA == evt->Type) {
    trader_tick* pTick = &evt->Tick;
    FILE* fp = fopen("mduser.txt", "a+");
    if(fp) {
      fprintf(fp,
        /*"InstrumentID[%s]\n"
        "TradingDay[%s]\n"
        "UpdateTime[%s]\n"
        "UpdateMillisec[%d]\n"
        "BidPrice1[%lf]\n"
        "BidVolume1[%d]\n"
        "AskPrice1[%lf]\n"
        "AskVolume1[%d]\n",*/
        "%s,%s,%s,%d,%lf,%d,%lf,%d\n",
        pTick->InstrumentID,
        pTick->TradingDay,
        pTick->UpdateTime,
        pTick->UpdateMillisec,
        pTick->BidPrice1,
        pTick->BidVolume1,
        pTick->AskPrice1,
        pTick->AskVolume1
        );
      fclose(fp);
    }
    
    return;
  }
  
  printf("\tType=[%d]\n"
    "\tErrorCd=[%d]\n"
    "\tErrorMsg=[%s]\n",
    evt->Type,
    evt->ErrorCd,
    evt->ErrorMsg);
}


int main(int argc, char* argv[])
{
  trader_mduser_test* test = trader_mduser_test_get();
  
  int nRet;
  evutil_socket_t pair[2];


  test->pBase = event_base_new();

  test->pSigIntEvt = evsignal_new(test->pBase, SIGINT, test_signal_cb, (void *)test);
  test->pSigTermEvt = evsignal_new(test->pBase, SIGTERM, test_signal_cb, (void *)test);
  
  nRet = evutil_socketpair(AF_UNIX, SOCK_STREAM, 0, pair);

  test->pBufMduser = bufferevent_socket_new(test->pBase, pair[0], BEV_OPT_CLOSE_ON_FREE);
  bufferevent_setcb(test->pBufMduser, test_trader_read_cb, 
    NULL, test_trader_evt_cb, (void*)test);
  bufferevent_enable(test->pBufMduser, EV_READ|EV_PERSIST);
  
  test->pBufStdin = bufferevent_socket_new(test->pBase, STDIN_FILENO, 0);
  bufferevent_setcb(test->pBufStdin, test_stdin_read_cb, 
    NULL, test_stdin_evt_cb, (void*)test);
  bufferevent_enable(test->pBufStdin, EV_READ|EV_PERSIST);
  
  //设置参数
#ifdef LTS
  //LTS
#include "trader_mduser_api_lts.h"
  test->pApi = trader_mduser_api_new(pair[1], trader_mduser_api_lts_method_get());
  test->pApi->pMethod->xSetUser(test->pApi, "2011", "130000005572", "7024211");
  test->pApi->pMethod->xSetFrontAddr(test->pApi, "tcp://116.228.234.66:34513");
#endif

#ifdef CTP
  //CTP
#include "trader_mduser_api_ctp.h"
  test->pApi = trader_mduser_api_new(pair[1], trader_mduser_api_ctp_method_get());
  test->pApi->pMethod->xSetUser(test->pApi, "9999", "033098", "282038");
  test->pApi->pMethod->xSetFrontAddr(test->pApi, "tcp://180.168.146.187:10010");
#endif

#ifdef FEMAS
  //FEMAS
#include "trader_mduser_api_femas.h"
  test->pApi = trader_mduser_api_new(pair[1], trader_mduser_api_femas_method_get());
  //test->pApi->pMethod->xSetUser(test->pApi, "0162", "9901540501", "282038");
  //test->pApi->pMethod->xSetFrontAddr(test->pApi, "tcp://118.126.16.229:17101");
  test->pApi->pMethod->xSetUser(test->pApi, "0177", "00007870", "123456");
  test->pApi->pMethod->xSetFrontAddr(test->pApi, "tcp://117.185.125.9:6888");
#endif
  
  test->pApi->pMethod->xSetWorkspace(test->pApi, ".");
  
  test->pApi->pMethod->xStart(test->pApi);

  nRet = event_base_dispatch(test->pBase);

  test->pApi->pMethod->xStop(test->pApi);
  
  trader_mduser_api_free(test->pApi);

  bufferevent_free(test->pBufMduser);
  
  bufferevent_free(test->pBufStdin);

  close(pair[1]);
  
  event_base_free(test->pBase);

  return 0;
  
}


