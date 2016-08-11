
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
#include "trader_trader_api.h"
#include "trader_trader_api_lts.h"

#define FEMAS // FEMAS LTS CTP

typedef struct trader_trader_test_def trader_trader_test;
struct trader_trader_test_def {

  struct event_base *pBase;
  struct event* pSigIntEvt;
  struct event* pSigTermEvt;

  // 对行情线程
  struct bufferevent* pBufTrader;
  struct bufferevent* pBufStdin;

  trader_trader_api* pApi;

};


static void test_signal_cb(evutil_socket_t fd, short event, void *arg);
static void test_trader_read_cb(struct bufferevent *bev, void *arg);
static void test_stdin_read_cb(struct bufferevent *bev, void *arg);

static void test_trader_evt_cb(struct bufferevent *bev, short event, void *arg);
static void test_stdin_evt_cb(struct bufferevent *bev, short event, void *arg);

static void test_trader_proc(trader_trader_test* test, trader_trader_evt* evt);

static trader_trader_test* trader_trader_test_get();

trader_trader_test* trader_trader_test_get()
{
  static trader_trader_test trader_trader_test_st;
  return &trader_trader_test_st;
}

void test_signal_cb(evutil_socket_t fd, short event, void *arg)
{
  trader_trader_test* test = (trader_trader_test*)arg;
  struct timeval tv = {
    2, 0
  };
  event_base_loopexit(test->pBase, &tv);

}

void test_trader_read_cb(struct bufferevent *bev, void *arg)
{
  trader_trader_test* test = (trader_trader_test*)arg;

  int nRet = 0;
  int nSize = 0;
  int loop = 1;
  trader_trader_evt oMsg;
  do {
    nRet = cmn_util_bufferevent_recv(test->pBufTrader, (unsigned char*)&oMsg, sizeof(oMsg));
    if(nRet <= 0){
      loop = 0;
      break;
    }

    test_trader_proc(test, &oMsg);
  }while(loop);

}

void test_stdin_read_cb(struct bufferevent *bev, void *arg)
{
  trader_trader_test* test = (trader_trader_test*)arg;
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
#ifdef CTP
      test->pApi->pMethod->xOrderInsert(test->pApi, "rb1610", "1", 
        '0',/*SECURITY_FTDC_D_Buy*/
        '0',/*SECURITY_FTDC_OF_Open*/
        0.2824f,
        1
      );
#endif
#ifdef LTS
      test->pApi->pMethod->xOrderInsert(test->pApi, "11000942", "1", 
        '0',/*SECURITY_FTDC_D_Buy*/
        '0',/*SECURITY_FTDC_OF_Open*/
        0.2824f,
        1
      );
#endif
#ifdef FEMAS
      test->pApi->pMethod->xOrderInsert(test->pApi, "IF1608", "1", 
        '0',/*SECURITY_FTDC_D_Buy*/
        '0',/*SECURITY_FTDC_OF_Open*/
        3230,
        1
      );
#endif
      break;
    case '3':
#ifdef CTP
      test->pApi->pMethod->xOrderAction(test->pApi, "rb1610", "2", "1", "1");
#endif
#ifdef LTS
      test->pApi->pMethod->xOrderAction(test->pApi, "11000942", "2", "1", "1");
#endif
#ifdef FEMAS
      test->pApi->pMethod->xOrderAction(test->pApi, "IF1608", "2", "1", "1");
#endif
      break;
    case '4':
      test->pApi->pMethod->xQryInstrument(test->pApi);
      break;
    case '5':
      test->pApi->pMethod->xQryUserInvestor(test->pApi);
      break;
    case '6':
      test->pApi->pMethod->xQryInvestorPosition(test->pApi);
      break;
    case '7':
      test->pApi->pMethod->xQryTradingAccount(test->pApi);
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
  trader_trader_test* test = (trader_trader_test*)arg;
}

void test_stdin_evt_cb(struct bufferevent *bev, short event, void *arg)
{
  trader_trader_test* test = (trader_trader_test*)arg;
}

void test_trader_proc(trader_trader_test* test, trader_trader_evt* evt)
{

  if(ONRSPQRYINSTRUMENT == evt->Type) {
    
    trader_instrument* pInstrument = &(evt->Body.InstrumentRsp);
    FILE* fp = fopen("inst.txt", "a+");
    if(fp) {
      fprintf(fp,
        "%s,%d,%lf,%lf\n",
        pInstrument->InstrumentID,
        pInstrument->VolumeMultiple,
        pInstrument->PriceTick,
        pInstrument->UnitMargin
      );
      fclose(fp);
    }
    return;
  }else if (ONRTNORDER == evt->Type) {
    trader_order* pOrder = &(evt->Body.OrderRsp);
    printf("\n"
      "InstrumentID[%s]\n"
      "UserOrderLocalID[%s]\n"
      "Direction[%c]\n"
      "OffsetFlag[%c]\n"
      "HedgeFlag[%c]\n"
      "LimitPrice[%lf]\n"
      "VolumeOriginal[%d]\n"
      "VolumeTraded[%d]\n"
      "OrderStatus[%c]\n"
      "InsertTime[%s]\n",
      pOrder->InstrumentID,
      pOrder->UserOrderLocalID,
      pOrder->Direction,
      pOrder->OffsetFlag,
      pOrder->HedgeFlag,
      pOrder->LimitPrice,
      pOrder->VolumeOriginal,
      pOrder->VolumeTraded,
      pOrder->OrderStatus,
      pOrder->InsertTime    
    );
  }

  printf("\tType=[%d]\n"
    "\tIsLast=[%d]\n"
    "\tErrorCd=[%d]\n"
    "\tErrorMsg=[%s]\n",
    evt->Type,
    evt->IsLast,
    evt->ErrorCd,
    evt->ErrorMsg);
}


int main(int argc, char* argv[])
{
  trader_trader_test* test = trader_trader_test_get();
  
  int nRet;
  evutil_socket_t pair[2];


  test->pBase = event_base_new();

  test->pSigIntEvt = evsignal_new(test->pBase, SIGINT, test_signal_cb, (void *)test);
  test->pSigTermEvt = evsignal_new(test->pBase, SIGTERM, test_signal_cb, (void *)test);
  
  nRet = evutil_socketpair(AF_UNIX, SOCK_STREAM, 0, pair);

  test->pBufTrader = bufferevent_socket_new(test->pBase, pair[0], BEV_OPT_CLOSE_ON_FREE);
  bufferevent_setcb(test->pBufTrader, test_trader_read_cb, 
    NULL, test_trader_evt_cb, (void*)test);
  bufferevent_enable(test->pBufTrader, EV_READ|EV_PERSIST);
  
  test->pBufStdin = bufferevent_socket_new(test->pBase, STDIN_FILENO, 0);
  bufferevent_setcb(test->pBufStdin, test_stdin_read_cb, 
    NULL, test_stdin_evt_cb, (void*)test);
  bufferevent_enable(test->pBufStdin, EV_READ|EV_PERSIST);

  //设置参数
#ifdef LTS
    //LTS
#include "trader_trader_api_lts.h"
    test->pApi = trader_trader_api_new(pair[1], trader_trader_api_lts_method_get());
    test->pApi->pMethod->xSetUser(test->pApi, "2011", "130000005572", "7024211");
    test->pApi->pMethod->xSetFrontAddr(test->pApi, "tcp://116.228.234.66:34505|tcp://116.228.234.66:34506");
#endif
  
#ifdef CTP
    //CTP
#include "trader_trader_api_ctp.h"
    test->pApi = trader_trader_api_new(pair[1], trader_trader_api_ctp_method_get());
    test->pApi->pMethod->xSetUser(test->pApi, "9999", "033098", "282038");
    test->pApi->pMethod->xSetFrontAddr(test->pApi, "tcp://180.168.146.187:10000");
#endif
  
#ifdef FEMAS
    //FEMAS
#include "trader_trader_api_femas.h"
    test->pApi = trader_trader_api_new(pair[1], trader_trader_api_femas_method_get());
    test->pApi->pMethod->xSetUser(test->pApi, "0162", "9901540501", "282038");
    test->pApi->pMethod->xSetFrontAddr(test->pApi, "tcp://118.126.16.227:17041");
#endif
  
  test->pApi->pMethod->xSetWorkspace(test->pApi, ".");
  
  test->pApi->pMethod->xStart(test->pApi);

  nRet = event_base_dispatch(test->pBase);

  test->pApi->pMethod->xStop(test->pApi);
  
  trader_trader_api_free(test->pApi);

  bufferevent_free(test->pBufTrader);
  
  bufferevent_free(test->pBufStdin);

  close(pair[1]);
  
  event_base_free(test->pBase);


  return 0;
}

