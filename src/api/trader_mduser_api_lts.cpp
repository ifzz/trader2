

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SecurityFtdcUserApiDataType.h"
#include "SecurityFtdcMdApi.h"

#include "LtsMduserHandler.h"


#ifdef __cplusplus
extern "C" {
#endif

#include "trader_data.h"
#include "trader_mduser_api.h"
#include "trader_mduser_api_lts.h"


static void trader_mduser_api_lts_start(trader_mduser_api* self);
static void trader_mduser_api_lts_stop(trader_mduser_api* self);
static void trader_mduser_api_lts_login(trader_mduser_api* self);
static void trader_mduser_api_lts_logout(trader_mduser_api* self);
static void trader_mduser_api_lts_subscribe(trader_mduser_api* self, char* instrument);

static lts_mduser_api_cb* lts_mduser_api_cb_get();

static void lts_mduser_on_front_connected(void* arg);
static void lts_mduser_on_front_disconnected(void* arg, int nReason);
static void lts_mduser_on_rsp_user_login(void* arg, CSecurityFtdcRspUserLoginField *pRspUserLogin, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
static void lts_mduser_on_rsp_user_logout(void* arg, CSecurityFtdcUserLogoutField *pRspUserLogout, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
static void lts_mduser_on_rtn_depth_market_data(void* arg, CSecurityFtdcDepthMarketDataField *pMarketData);
static void lts_mduser_on_rsp_error(void* arg, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
static void lts_mduser_on_rsp_sub_market_data(void* arg, CSecurityFtdcSpecificInstrumentField *pSpecificInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
static void lts_mduser_on_rsp_un_sub_market_data(void* arg, CSecurityFtdcSpecificInstrumentField *pSpecificInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);



#ifdef __cplusplus
}
#endif

lts_mduser_api_cb* lts_mduser_api_cb_get()
{
  static lts_mduser_api_cb lts_mduser_api_cb_st = {
    lts_mduser_on_front_connected,
    lts_mduser_on_front_disconnected,
    lts_mduser_on_rsp_user_login,
    lts_mduser_on_rsp_user_logout,
    lts_mduser_on_rtn_depth_market_data,
    lts_mduser_on_rsp_error,
    lts_mduser_on_rsp_sub_market_data,
    lts_mduser_on_rsp_un_sub_market_data
  };

  return &lts_mduser_api_cb_st;

}


trader_mduser_api_method* trader_mduser_api_lts_method_get()
{
  static trader_mduser_api_method trader_mduser_api_method_st = {
    trader_mduser_api_set_user,
    trader_mduser_api_set_front_addr,
    trader_mduser_api_set_workspace,
    trader_mduser_api_lts_start,
    trader_mduser_api_lts_stop,
    trader_mduser_api_lts_login,
    trader_mduser_api_lts_logout,
    trader_mduser_api_lts_subscribe
  };

  return &trader_mduser_api_method_st;
}

void trader_mduser_api_lts_start(trader_mduser_api* self)
{
  CSecurityFtdcMdApi* pUserApi = CSecurityFtdcMdApi::CreateFtdcMdApi();
  CLtsMduserHandler* pHandler = new CLtsMduserHandler(lts_mduser_api_cb_get(), (void*)self);

  trader_mduser_api_lts* pImp = (trader_mduser_api_lts*)malloc(sizeof(trader_mduser_api_lts));
  pImp->pMdApi = (void*)pUserApi;
  pImp->pHandler = (void*)pHandler;
  pImp->nRequestID = 0;
  
  self->pUserApi = (void*)pImp;

  // connect
  pUserApi->RegisterSpi(pHandler);
  //pUserApi->SubscribeMarketDataTopic(self->TopicId, USTP_TERT_QUICK);
  pUserApi->RegisterFront(self->pAddress);
  pUserApi->Init();

  return ;

}

void trader_mduser_api_lts_stop(trader_mduser_api* self)
{
  trader_mduser_api_lts* pImp = (trader_mduser_api_lts*)self->pUserApi;
  CSecurityFtdcMdApi* pUserApi = (CSecurityFtdcMdApi*)pImp->pMdApi;
  CLtsMduserHandler* pHandler = (CLtsMduserHandler*)pImp->pHandler;

  pUserApi->RegisterSpi(NULL);
  pUserApi->Release();
  delete pHandler;
  free(pImp);
  self->pUserApi = (void*)NULL;
  
  return ;
}

void trader_mduser_api_lts_login(trader_mduser_api* self)
{
  trader_mduser_api_lts* pImp = (trader_mduser_api_lts*)self->pUserApi;
  CSecurityFtdcMdApi* pUserApi = (CSecurityFtdcMdApi*)pImp->pMdApi;

  // login
  CSecurityFtdcReqUserLoginField reqUserLogin;
  memset(&reqUserLogin, 0, sizeof(reqUserLogin));
  //strcpy(reqUserLogin.TradingDay, pUserApi->GetTradingDay());
  strcpy(reqUserLogin.BrokerID, self->pBrokerID);
  strcpy(reqUserLogin.UserID, self->pUser);
  strcpy(reqUserLogin.Password,self->pPwd);
  strcpy(reqUserLogin.UserProductInfo, "Login");
  pUserApi->ReqUserLogin(&reqUserLogin, pImp->nRequestID++);
}

void trader_mduser_api_lts_logout(trader_mduser_api* self)
{
  trader_mduser_api_lts* pImp = (trader_mduser_api_lts*)self->pUserApi;
  CSecurityFtdcMdApi* pUserApi = (CSecurityFtdcMdApi*)pImp->pMdApi;

  CSecurityFtdcUserLogoutField userLogout;
  memset(&userLogout, 0, sizeof(userLogout));
  
  //strcpy(userLogout.BrokerID, self->pBrokerID);
  //strcpy(userLogout.UserID, self->pUser);
  
  pUserApi->ReqUserLogout(&userLogout, pImp->nRequestID++);

}

void trader_mduser_api_lts_subscribe(trader_mduser_api* self, char* instrument)
{
  trader_mduser_api_lts* pImp = (trader_mduser_api_lts*)self->pUserApi;
  CSecurityFtdcMdApi* pUserApi = (CSecurityFtdcMdApi*)pImp->pMdApi;

  char* contracts[1];
  contracts[0] = instrument;
  // 第三个参数
  // 上证 SSE
  // 深证 SCE
  pUserApi->SubscribeMarketData(contracts, 1, "SSE");
  return ;

}

void lts_mduser_on_front_connected(void* arg)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  trader_mduser_api_on_front_connected(self);
  
}

void lts_mduser_on_front_disconnected(void* arg, int nReason)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  int errNo = nReason;
  char errMsg[81];

  switch(nReason) {
  case 0x1001:
    strcpy(errMsg, "0x1001 网络读失败");
    break;
  case 0x1002:
    strcpy(errMsg, "0x1002 网络写失败");
    break;
  case 0x2001:
    strcpy(errMsg, "0x2001 接收心跳超时");
    break;
  case 0x2002:
    strcpy(errMsg, "0x2002 发送心跳失败");
    break;
  case 0x2003:
    strcpy(errMsg, "0x2003 收到错误报文");
    break;
  }
  
  trader_mduser_api_on_front_disconnected(self, errNo, errMsg);

  return ;
}

void lts_mduser_on_rsp_user_login(void* arg, CSecurityFtdcRspUserLoginField *pRspUserLogin, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }
  
  trader_mduser_api_on_rsp_user_login(self, errNo, errMsg);
  
}

void lts_mduser_on_rsp_user_logout(void* arg, CSecurityFtdcUserLogoutField *pRspUserLogout, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }
  
  trader_mduser_api_on_rsp_user_logout(self, errNo, errMsg);
  
}

void lts_mduser_on_rtn_depth_market_data(void* arg, CSecurityFtdcDepthMarketDataField *pMarketData)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  trader_tick oTick;
  memset(&oTick, 0, sizeof(trader_tick));

  strcpy(oTick.InstrumentID, pMarketData->InstrumentID);
  strcpy(oTick.TradingDay, pMarketData->TradingDay);
  strcpy(oTick.UpdateTime, pMarketData->UpdateTime);
  oTick.UpdateMillisec = pMarketData->UpdateMillisec;
  oTick.BidPrice1 = pMarketData->BidPrice1;
  oTick.BidVolume1= pMarketData->BidVolume1;
  oTick.AskPrice1 = pMarketData->AskPrice1;
  oTick.AskVolume1= pMarketData->AskVolume1;

  trader_mduser_api_on_rtn_depth_market_data(self, &oTick);

  return;
}

void lts_mduser_on_rsp_error(void* arg, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }
  
  trader_mduser_api_on_rsp_error(self, errNo, errMsg);

}

void lts_mduser_on_rsp_sub_market_data(void* arg, CSecurityFtdcSpecificInstrumentField *pSpecificInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }
  
  trader_mduser_api_on_rsp_sub_market_data(self, errNo, errMsg);

}

void lts_mduser_on_rsp_un_sub_market_data(void* arg, CSecurityFtdcSpecificInstrumentField *pSpecificInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast)
{
  trader_mduser_api* self = (trader_mduser_api*)arg;
  
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }
  
  trader_mduser_api_on_rsp_un_sub_market_data(self, errNo, errMsg);

}



