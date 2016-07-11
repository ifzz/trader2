
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SecurityFtdcUserApiDataType.h"
#include "SecurityFtdcTraderApi.h"
#include "SecurityFtdcQueryApi.h"

#include "LtsTraderHandler.h"


#ifdef __cplusplus
extern "C" {
#endif

#include "trader_data.h"
#include "trader_trader_api.h"
#include "trader_trader_api_lts.h"

static void trader_trader_api_lts_start(trader_trader_api* self);
static void trader_trader_api_lts_stop(trader_trader_api* self);
static void trader_trader_api_lts_login(trader_trader_api* self);
static void trader_trader_api_lts_logout(trader_trader_api* self);
  
static int trader_trader_api_lts_order_insert(trader_trader_api* self, char* inst, char* local_id, char buy_sell, char open_close, double price, int vol);
static int trader_trader_api_lts_order_action(trader_trader_api* self, char* inst, char* local_id, char* org_local_id, char* order_sys_id);
 
static int trader_trader_api_lts_qry_instrument(trader_trader_api* self);
static int trader_trader_api_lts_qry_user_investor(trader_trader_api* self);
static int trader_trader_api_lts_qry_investor_position(trader_trader_api* self);
static int trader_trader_api_lts_qry_trading_account(trader_trader_api* self);


static void lts_trader_on_front_connected(void* arg);
static void lts_trader_on_front_disconnected(void* arg, int nReason);
static void lts_trader_on_rsp_user_login(void* arg, CSecurityFtdcRspUserLoginField *pRspUserLogin, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
static void lts_trader_on_rsp_user_logout(void* arg, CSecurityFtdcUserLogoutField *pRspUserLogout, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
static void lts_trader_on_rsp_fetch_auth_randcode(void* arg, CSecurityFtdcAuthRandCodeField *pAuthRandCode, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
static void lts_trader_on_rsp_error(void* arg, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

static void lts_trader_on_rsp_order_insert(void* arg, CSecurityFtdcInputOrderField *pInputOrder, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
static void lts_trader_on_rsp_order_action(void* arg, CSecurityFtdcInputOrderActionField *pInputOrderAction, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
static void lts_trader_on_rtn_order(void* arg, CSecurityFtdcOrderField *pOrder);
static void lts_trader_on_rtn_trade(void* arg, CSecurityFtdcTradeField *pTrade);
static void lts_trader_on_err_rtn_order_insert(void* arg, CSecurityFtdcInputOrderField *pInputOrder, CSecurityFtdcRspInfoField *pRspInfo);
static void lts_trader_on_err_rtn_order_action(void* arg, CSecurityFtdcOrderActionField *pOrderAction, CSecurityFtdcRspInfoField *pRspInfo);

static lts_trader_api_cb* lts_trader_api_cb_get();

static void lts_query_on_front_connected(void* arg);
static void lts_query_on_front_disconnected(void* arg, int nReason);
static void lts_query_on_rsp_user_login(void* arg, CSecurityFtdcRspUserLoginField *pRspUserLogin, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
static void lts_query_on_rsp_user_logout(void* arg, CSecurityFtdcUserLogoutField *pRspUserLogout, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
static void lts_query_on_rsp_fetch_auth_randcode(void* arg, CSecurityFtdcAuthRandCodeField *pAuthRandCode, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
static void lts_query_on_rsp_error(void* arg, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

static void lts_query_on_rsp_qry_investor(void* arg, CSecurityFtdcInvestorField *pInvestor, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
static void lts_query_on_rsp_qry_instrument(void* arg, CSecurityFtdcInstrumentField *pInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
static void lts_query_on_rsp_qry_trading_account(void* arg, CSecurityFtdcTradingAccountField *pTradingAccount, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
static void lts_query_on_rsp_qry_investor_position(void* arg, CSecurityFtdcInvestorPositionField *pInvestorPosition, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

static lts_query_api_cb* lts_query_api_cb_get();

#ifdef __cplusplus
}
#endif

trader_trader_api_method* trader_trader_api_lts_method_get()
{
  static trader_trader_api_method trader_trader_api_method_st = {
    trader_trader_api_set_user,
    trader_trader_api_set_front_addr,
    trader_trader_api_set_workspace,
    trader_trader_api_set_param,
    trader_trader_api_get_trading_day,
    trader_trader_api_get_max_order_local_id,
    trader_trader_api_lts_start,
    trader_trader_api_lts_stop,
    trader_trader_api_lts_login,
    trader_trader_api_lts_logout,
    trader_trader_api_lts_order_insert,
    trader_trader_api_lts_order_action,
    trader_trader_api_lts_qry_instrument,
    trader_trader_api_lts_qry_user_investor,
    trader_trader_api_lts_qry_investor_position,
    trader_trader_api_lts_qry_trading_account
  };

  return &trader_trader_api_method_st;
}


void trader_trader_api_lts_start(trader_trader_api* self)
{
  CSecurityFtdcTraderApi* pTraderApi = CSecurityFtdcTraderApi::CreateFtdcTraderApi(self->pWorkspace);
  CSecurityFtdcQueryApi* pQueryApi = CSecurityFtdcQueryApi::CreateFtdcQueryApi(self->pWorkspace);
  CLtsTraderHandler* pTraderHandler = new CLtsTraderHandler(lts_trader_api_cb_get(), (void*)self);
  CLtsQueryHandler* pQueryHandler = new CLtsQueryHandler(lts_query_api_cb_get(), (void*)self);
  
  trader_trader_api_lts* pImp = (trader_trader_api_lts*)malloc(sizeof(trader_trader_api_lts));
  pImp->pTraderApi = (void*)pTraderApi;
  pImp->pQueryApi = (void*)pQueryApi;
  pImp->pTraderHandler = (void*)pTraderHandler;
  pImp->pQueryHandler = (void*)pQueryHandler;

  pImp->nTraderRequestID = 0;
  pImp->nQueryRequestID = 0;

  self->pUserApi = (void*)pImp;

  //拆分服务器地址
  char sTraderAddress[21];
  char sQueryAddress[21];
  char* p = strchr(self->pAddress, (int)'|');

  memset(sTraderAddress, 0, sizeof(sTraderAddress));
  memset(sQueryAddress, 0, sizeof(sQueryAddress));

  memcpy(sTraderAddress, self->pAddress, p - self->pAddress);
  strcpy(sQueryAddress, p+1);

  // 交易
  pTraderApi->RegisterSpi(pTraderHandler);
  pTraderApi->RegisterFront(sTraderAddress);
  pTraderApi->SubscribePrivateTopic(SECURITY_TERT_QUICK);
  pTraderApi->SubscribePublicTopic(SECURITY_TERT_QUICK);

  // 查询
  pQueryApi->RegisterSpi(pQueryHandler);
  pQueryApi->RegisterFront(sQueryAddress);
  
  // 连接查询服务器
  pQueryApi->Init();

  // 连接交易服务器
  pTraderApi->Init();

  return;

}

void trader_trader_api_lts_stop(trader_trader_api* self)
{
  trader_trader_api_lts* pImp = (trader_trader_api_lts*)self->pUserApi;
  CSecurityFtdcTraderApi* pTraderApi = (CSecurityFtdcTraderApi*)pImp->pTraderApi;
  CSecurityFtdcQueryApi* pQueryApi = (CSecurityFtdcQueryApi*)pImp->pQueryApi;
  CLtsTraderHandler* pTraderHandler = (CLtsTraderHandler*)pImp->pTraderHandler;
  CLtsQueryHandler* pQueryHandler = (CLtsQueryHandler*)pImp->pQueryHandler;
  
  pTraderApi->RegisterSpi(NULL);
  pTraderApi->Release();
  delete pTraderHandler;
  
  pQueryApi->RegisterSpi(NULL);
  pQueryApi->Release();
  delete pQueryHandler;
  
  free(pImp);
  self->pUserApi = (void*)NULL;
  
  return;
}

void trader_trader_api_lts_login(trader_trader_api* self)
{
  trader_trader_api_lts* pImp = (trader_trader_api_lts*)self->pUserApi;
  CSecurityFtdcTraderApi* pTraderApi = (CSecurityFtdcTraderApi*)pImp->pTraderApi;
  CSecurityFtdcQueryApi* pQueryApi = (CSecurityFtdcQueryApi*)pImp->pQueryApi;
  
  CSecurityFtdcAuthRandCodeField authRandCodeField;
  memset(&authRandCodeField, 0, sizeof(authRandCodeField));
  
  pTraderApi->ReqFetchAuthRandCode(&authRandCodeField, pImp->nTraderRequestID++);
  
  memset(&authRandCodeField, 0, sizeof(authRandCodeField));
  
  pQueryApi->ReqFetchAuthRandCode(&authRandCodeField, pImp->nQueryRequestID++);
}

void trader_trader_api_lts_logout(trader_trader_api* self)
{
  trader_trader_api_lts* pImp = (trader_trader_api_lts*)self->pUserApi;
  CSecurityFtdcTraderApi* pTraderApi = (CSecurityFtdcTraderApi*)pImp->pTraderApi;
  CSecurityFtdcQueryApi* pQueryApi = (CSecurityFtdcQueryApi*)pImp->pQueryApi;

  CSecurityFtdcUserLogoutField userLogoutField;
  
  memset(&userLogoutField, 0, sizeof(userLogoutField));
  strcpy(userLogoutField.BrokerID, self->pBrokerID);
  strcpy(userLogoutField.UserID, self->pUser);
  pTraderApi->ReqUserLogout(&userLogoutField, pImp->nTraderRequestID++);

  memset(&userLogoutField, 0, sizeof(userLogoutField));
  strcpy(userLogoutField.BrokerID, self->pBrokerID);
  strcpy(userLogoutField.UserID, self->pUser);
  pQueryApi->ReqUserLogout(&userLogoutField, pImp->nQueryRequestID++);

  return;
}

  
int trader_trader_api_lts_order_insert(trader_trader_api* self, char* inst, char* local_id, char buy_sell, char open_close, double price, int vol)
{
  trader_trader_api_lts* pImp = (trader_trader_api_lts*)self->pUserApi;
  CSecurityFtdcTraderApi* pTraderApi = (CSecurityFtdcTraderApi*)pImp->pTraderApi;

  CSecurityFtdcInputOrderField inputOrderField;

  memset(&inputOrderField, 0, sizeof(inputOrderField));
	///经纪公司代码
	strcpy(inputOrderField.BrokerID, self->pBrokerID);
	///投资者代码
	strcpy(inputOrderField.InvestorID, self->pUser);
	///合约代码
	strcpy(inputOrderField.InstrumentID, inst);
	///报单引用
	strcpy(inputOrderField.OrderRef, local_id);
	///用户代码
	strcpy(inputOrderField.UserID, self->pUser);
	///交易所代码
	strcpy(inputOrderField.ExchangeID, "SSE");
	///报单价格条件
	inputOrderField.OrderPriceType = SECURITY_FTDC_OPT_LimitPrice;
	//inputOrderField.OrderPriceType = SECURITY_FTDC_OPT_BestPrice;
	///买卖方向
	inputOrderField.Direction = buy_sell;
	///组合开平标志
	inputOrderField.CombOffsetFlag[0] = open_close;
	///组合投机套保标志
	inputOrderField.CombHedgeFlag[0] = SECURITY_FTDC_HF_Speculation;
	///价格
	sprintf(inputOrderField.LimitPrice, "%lf", price);
	///数量
	inputOrderField.VolumeTotalOriginal = vol;
	///有效期类型
	inputOrderField.TimeCondition = SECURITY_FTDC_TC_GFD;
	///成交量类型
	inputOrderField.VolumeCondition = SECURITY_FTDC_VC_AV;
	///最小成交量
	inputOrderField.MinVolume = 1;
	///触发条件
	inputOrderField.ContingentCondition = SECURITY_FTDC_CC_Immediately;
	///强平原因
	inputOrderField.ForceCloseReason = SECURITY_FTDC_FCC_NotForceClose;
	///自动挂起标志
	inputOrderField.IsAutoSuspend = 0;
	///用户强评标志
	inputOrderField.UserForceClose = 0;

  pTraderApi->ReqOrderInsert(&inputOrderField, pImp->nTraderRequestID++);

  
}

int trader_trader_api_lts_order_action(trader_trader_api* self, char* inst, char* local_id, char* org_local_id, char* order_sys_id)
{
  trader_trader_api_lts* pImp = (trader_trader_api_lts*)self->pUserApi;
  CSecurityFtdcTraderApi* pTraderApi = (CSecurityFtdcTraderApi*)pImp->pTraderApi;

  CSecurityFtdcInputOrderActionField inputOrderActionField;
  
  memset(&inputOrderActionField, 0, sizeof(inputOrderActionField));

	///经纪公司代码
	strcpy(inputOrderActionField.BrokerID, self->pBrokerID);
	///投资者代码
	strcpy(inputOrderActionField.InvestorID, self->pUser);
	///报单操作引用
	inputOrderActionField.OrderActionRef = atoi(local_id);
	///报单引用
	strcpy(inputOrderActionField.OrderRef, org_local_id);
	///前置编号
	inputOrderActionField.FrontID = pImp->nFrontID;
	///会话编号
  inputOrderActionField.SessionID = pImp->nSessionID;
	///交易所代码
	strcpy(inputOrderActionField.ExchangeID, "SSE");
	///操作标志
	inputOrderActionField.ActionFlag = SECURITY_FTDC_AF_Delete;
	///用户代码
	strcpy(inputOrderActionField.UserID, self->pUser);
	///合约代码
	strcpy(inputOrderActionField.InstrumentID, inst);
  
  pTraderApi->ReqOrderAction(&inputOrderActionField, pImp->nTraderRequestID++);

}

 
int trader_trader_api_lts_qry_instrument(trader_trader_api* self)
{
  trader_trader_api_lts* pImp = (trader_trader_api_lts*)self->pUserApi;
  CSecurityFtdcQueryApi* pQueryApi = (CSecurityFtdcQueryApi*)pImp->pQueryApi;

  CSecurityFtdcQryInstrumentField qryInstrumentField;
  
  memset(&qryInstrumentField, 0, sizeof(qryInstrumentField));
	///合约代码
	//TSecurityFtdcInstrumentIDType	InstrumentID;
	///交易所代码
	//TSecurityFtdcExchangeIDType	ExchangeID;
	strcpy(qryInstrumentField.ExchangeID, "SSE");

  pQueryApi->ReqQryInstrument(&qryInstrumentField, pImp->nQueryRequestID++);

}

int trader_trader_api_lts_qry_user_investor(trader_trader_api* self)
{
  trader_trader_api_lts* pImp = (trader_trader_api_lts*)self->pUserApi;
  CSecurityFtdcQueryApi* pQueryApi = (CSecurityFtdcQueryApi*)pImp->pQueryApi;

  CSecurityFtdcQryInvestorField qryInvestorField;

  memset(&qryInvestorField, 0, sizeof(qryInvestorField));
	///经纪公司代码
	strcpy(qryInvestorField.BrokerID, self->pBrokerID);
	///投资者代码
	strcpy(qryInvestorField.InvestorID, self->pUser);  
  
  pQueryApi->ReqQryInvestor(&qryInvestorField, pImp->nQueryRequestID++);

}

int trader_trader_api_lts_qry_investor_position(trader_trader_api* self)
{
  trader_trader_api_lts* pImp = (trader_trader_api_lts*)self->pUserApi;
  CSecurityFtdcQueryApi* pQueryApi = (CSecurityFtdcQueryApi*)pImp->pQueryApi;

  CSecurityFtdcQryInvestorPositionField qryInvestorPositionField;
  memset(&qryInvestorPositionField, 0, sizeof(qryInvestorPositionField));

	///经纪公司代码
	strcpy(qryInvestorPositionField.BrokerID, self->pBrokerID);
	///投资者代码
	strcpy(qryInvestorPositionField.InvestorID, self->pUser);  
	///合约代码
	//TSecurityFtdcInstrumentIDType	InstrumentID;

  pQueryApi->ReqQryInvestorPosition(&qryInvestorPositionField, pImp->nQueryRequestID++);

}

int trader_trader_api_lts_qry_trading_account(trader_trader_api* self)
{
  trader_trader_api_lts* pImp = (trader_trader_api_lts*)self->pUserApi;
  CSecurityFtdcQueryApi* pQueryApi = (CSecurityFtdcQueryApi*)pImp->pQueryApi;
  
  CSecurityFtdcQryTradingAccountField qryTradingAccountField;
  memset(&qryTradingAccountField, 0, sizeof(qryTradingAccountField));

	///经纪公司代码
	strcpy(qryTradingAccountField.BrokerID, self->pBrokerID);
	///投资者代码
	strcpy(qryTradingAccountField.InvestorID, self->pUser);  

  pQueryApi->ReqQryTradingAccount(&qryTradingAccountField, pImp->nQueryRequestID++);

}

void lts_trader_on_front_connected(void* arg)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  trader_trader_api_on_front_connected(self);

}

void lts_trader_on_front_disconnected(void* arg, int nReason)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  trader_trader_api_on_front_disconnected(self, nReason);
}

void lts_trader_on_rsp_user_login(void* arg, CSecurityFtdcRspUserLoginField *pRspUserLogin, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  trader_trader_api_lts* pImp = (trader_trader_api_lts*)self->pUserApi;
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }
  if(pRspUserLogin) {
    pImp->nSessionID = pRspUserLogin->SessionID;
    pImp->nFrontID= pRspUserLogin->FrontID;
  }
  
  trader_trader_api_on_rsp_user_login(self, errNo, errMsg);
}

void lts_trader_on_rsp_user_logout(void* arg, CSecurityFtdcUserLogoutField *pRspUserLogout, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }
  
  trader_trader_api_on_rsp_user_logout(self, errNo, errMsg);
}

void lts_trader_on_rsp_fetch_auth_randcode(void* arg, CSecurityFtdcAuthRandCodeField *pAuthRandCode, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  trader_trader_api_lts* pImp = (trader_trader_api_lts*)self->pUserApi;
  CSecurityFtdcTraderApi* pTraderApi = (CSecurityFtdcTraderApi*)pImp->pTraderApi;
  CSecurityFtdcQueryApi* pQueryApi = (CSecurityFtdcQueryApi*)pImp->pQueryApi;

  CSecurityFtdcReqUserLoginField reqUserLoginField;
  
  memset(&reqUserLoginField, 0, sizeof(reqUserLoginField));
  strcpy(reqUserLoginField.BrokerID, self->pBrokerID);
  strcpy(reqUserLoginField.UserID, self->pUser);
  strcpy(reqUserLoginField.Password, self->pPwd);
  if(pAuthRandCode) {
    strcpy(reqUserLoginField.RandCode, pAuthRandCode->RandCode);
  }
  strcpy(reqUserLoginField.UserProductInfo, "LTS-Test");
  strcpy(reqUserLoginField.AuthCode, "N3EHKP4CYHZGM9VJ");
  
  pTraderApi->ReqUserLogin(&reqUserLoginField, pImp->nTraderRequestID++);
  

}


void lts_trader_on_rsp_error(void* arg, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }
  
  trader_trader_api_on_rsp_error(self, errNo, errMsg);
}


void lts_trader_on_rsp_order_insert(void* arg, CSecurityFtdcInputOrderField *pInputOrder, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }
  
  trader_trader_api_on_rsp_order_insert(self, errNo, errMsg);
  
}

void lts_trader_on_rsp_order_action(void* arg, CSecurityFtdcInputOrderActionField *pInputOrderAction, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }
  
  trader_trader_api_on_rsp_order_action(self, errNo, errMsg);
}

void lts_trader_on_rtn_order(void* arg, CSecurityFtdcOrderField *pOrder)
{
  trader_trader_api* self = (trader_trader_api*)arg;

  trader_order traderOrder;
  memset(&traderOrder, 0, sizeof(traderOrder));
  
  // 合约代码
  char InstrumentID [31];
  strcpy(traderOrder.InstrumentID, pOrder->InstrumentID);
  // 本地报单编号
  strcpy(traderOrder.UserOrderLocalID, pOrder->OrderRef);
  // 买卖
  traderOrder.Direction = pOrder->Direction;
  // 开平
  traderOrder.OffsetFlag = pOrder->CombOffsetFlag[0];
  ///投机套保标志
  traderOrder.HedgeFlag = pOrder->CombHedgeFlag[0];
  // 报单价格
  traderOrder.LimitPrice = atof(pOrder->LimitPrice);
  // 报单手数
  traderOrder.VolumeOriginal = pOrder->VolumeTotalOriginal;
  // 成交手数
  traderOrder.VolumeTraded = pOrder->VolumeTraded;
  // 订单状态
  traderOrder.OrderStatus = pOrder->OrderStatus;
  ///插入时间
  strcpy(traderOrder.InsertTime, pOrder->InsertTime);

  trader_trader_api_on_rtn_order(self, &traderOrder);
  
}

void lts_trader_on_rtn_trade(void* arg, CSecurityFtdcTradeField *pTrade)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  trader_trade traderTrade;
  memset(&traderTrade, 0, sizeof(traderTrade));
  
  ///合约代码
  strcpy(traderTrade.InstrumentID, pTrade->InstrumentID);
  ///本地报单编号
  strcpy(traderTrade.UserOrderLocalID, pTrade->OrderRef);
  ///交易日
  strcpy(traderTrade.TradingDay, pTrade->TradingDay);
  ///成交时间
  strcpy(traderTrade.TradeTime, pTrade->TradeTime);
  ///成交价格
  traderTrade.TradePrice = atof(pTrade->Price);
  ///成交数量
  traderTrade.TradeVolume = pTrade->Volume;
  //成交编号
  strcpy(traderTrade.TradeID, pTrade->TradeID);

  trader_trader_api_on_rtn_trade(self, &traderTrade);
}

void lts_trader_on_err_rtn_order_insert(void* arg, CSecurityFtdcInputOrderField *pInputOrder, CSecurityFtdcRspInfoField *pRspInfo)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }
  
  trader_trader_api_on_err_rtn_order_insert(self, errNo, errMsg);
}

void lts_trader_on_err_rtn_order_action(void* arg, CSecurityFtdcOrderActionField *pOrderAction, CSecurityFtdcRspInfoField *pRspInfo)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }
  
  trader_trader_api_on_err_rtn_order_action(self, errNo, errMsg);
}


lts_trader_api_cb* lts_trader_api_cb_get()
{
  static lts_trader_api_cb lts_trader_api_cb_st = {
    lts_trader_on_front_connected,
    lts_trader_on_front_disconnected,
    lts_trader_on_rsp_user_login,
    lts_trader_on_rsp_user_logout,
    lts_trader_on_rsp_fetch_auth_randcode,
    lts_trader_on_rsp_error,
    lts_trader_on_rsp_order_insert,
    lts_trader_on_rsp_order_action,
    lts_trader_on_rtn_order,
    lts_trader_on_rtn_trade,
    lts_trader_on_err_rtn_order_insert,
    lts_trader_on_err_rtn_order_action
  };

  return &lts_trader_api_cb_st;
}


void lts_query_on_front_connected(void* arg)
{
  trader_trader_api* self = (trader_trader_api*)arg;
}

void lts_query_on_front_disconnected(void* arg, int nReason)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  int errNo = nReason;
}

void lts_query_on_rsp_user_login(void* arg, CSecurityFtdcRspUserLoginField *pRspUserLogin, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }
}

void lts_query_on_rsp_user_logout(void* arg, CSecurityFtdcUserLogoutField *pRspUserLogout, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }
}

void lts_query_on_rsp_fetch_auth_randcode(void* arg, CSecurityFtdcAuthRandCodeField *pAuthRandCode, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  trader_trader_api_lts* pImp = (trader_trader_api_lts*)self->pUserApi;
  CSecurityFtdcQueryApi* pQueryApi = (CSecurityFtdcQueryApi*)pImp->pQueryApi;
  
  CSecurityFtdcReqUserLoginField reqUserLoginField;
  memset(&reqUserLoginField, 0, sizeof(reqUserLoginField));
  strcpy(reqUserLoginField.BrokerID, self->pBrokerID);
  strcpy(reqUserLoginField.UserID, self->pUser);
  strcpy(reqUserLoginField.Password, self->pPwd);
  if(pAuthRandCode) {
    strcpy(reqUserLoginField.RandCode, pAuthRandCode->RandCode);
  }
  strcpy(reqUserLoginField.UserProductInfo, "LTS-Test");
  strcpy(reqUserLoginField.AuthCode, "N3EHKP4CYHZGM9VJ");
  
  pQueryApi->ReqUserLogin(&reqUserLoginField, pImp->nQueryRequestID++);
  
}

void lts_query_on_rsp_error(void* arg, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }
  
  trader_trader_api_on_rsp_error(self, errNo, errMsg);
}


void lts_query_on_rsp_qry_investor(void* arg, CSecurityFtdcInvestorField *pInvestor, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  int errNo = 0;
  char* errMsg = NULL;
  char* investor = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }

  if(pInvestor) {
    investor = pInvestor->InvestorID;
  }
  
  trader_trader_api_on_rsp_qry_user_investor(self, investor, errNo, errMsg);
}

void lts_query_on_rsp_qry_instrument(void* arg, CSecurityFtdcInstrumentField *pInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  trader_instrument traderInstrument;
  memset(&traderInstrument, 0, sizeof(traderInstrument));
  
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }

  if(pInstrument) {
    strcpy(traderInstrument.InstrumentID, pInstrument->InstrumentID);
    ///合约数量乘数
    traderInstrument.VolumeMultiple = pInstrument->VolumeMultiple;
    ///最小变动价位
    traderInstrument.PriceTick = pInstrument->PriceTick;
    ///期权单手保证金
    traderInstrument.UnitMargin = pInstrument->UnitMargin;
  }

  trader_trader_api_on_rsp_qry_instrument(self, &traderInstrument, errNo, errMsg, bIsLast);

  
}

void lts_query_on_rsp_qry_trading_account(void* arg, CSecurityFtdcTradingAccountField *pTradingAccount, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  trader_account traderAccount;
  memset(&traderAccount, 0, sizeof(traderAccount));
  
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }

  if(pTradingAccount) {
    strcpy(traderAccount.AccountID, pTradingAccount->AccountID);
    //TODO
  }

  trader_trader_api_on_rsp_qry_trading_account(self, &traderAccount, errNo, errMsg, bIsLast);
}

void lts_query_on_rsp_qry_investor_position(void* arg, CSecurityFtdcInvestorPositionField *pInvestorPosition, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  trader_position traderPosition;
  memset(&traderPosition, 0, sizeof(traderPosition));
  
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->ErrorID;
    errMsg = pRspInfo->ErrorMsg;
  }

  if(pInvestorPosition) {
    strcpy(traderPosition.InstrumentID, pInvestorPosition->InstrumentID);
    //TODO
  }

  trader_trader_api_on_rsp_qry_investor_position(self, &traderPosition, errNo, errMsg, bIsLast);
}


lts_query_api_cb* lts_query_api_cb_get()
{
  static lts_query_api_cb lts_query_api_cb_st = {
    lts_query_on_front_connected,
    lts_query_on_front_disconnected,
    lts_query_on_rsp_user_login,
    lts_query_on_rsp_user_logout,
    lts_query_on_rsp_fetch_auth_randcode,
    lts_query_on_rsp_error,
    lts_query_on_rsp_qry_investor,
    lts_query_on_rsp_qry_instrument,
    lts_query_on_rsp_qry_trading_account,
    lts_query_on_rsp_qry_investor_position    
  };

  return &lts_query_api_cb_st;

}


