
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "DFITCSECApiDataType.h"
#include "DFITCSECApiStruct.h"
#include "DFITCSECTraderApi.h"

#include "DFITCSECTraderHandler.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "trader_data.h"
#include "trader_trader_api.h"
#include "trader_trader_api_dfitc_sop.h"

static void trader_trader_api_dfitc_sop_start(trader_trader_api* self);
static void trader_trader_api_dfitc_sop_stop(trader_trader_api* self);
static void trader_trader_api_dfitc_sop_login(trader_trader_api* self);
static void trader_trader_api_dfitc_sop_logout(trader_trader_api* self);
  
static int trader_trader_api_dfitc_sop_order_insert(trader_trader_api* self, char* inst, char* local_id, char buy_sell, char open_close, double price, int vol);
static int trader_trader_api_dfitc_sop_order_action(trader_trader_api* self, char* inst, char* local_id, char* org_local_id, char* order_sys_id);
 
static int trader_trader_api_dfitc_sop_qry_instrument(trader_trader_api* self);
static int trader_trader_api_dfitc_sop_qry_user_investor(trader_trader_api* self);
static int trader_trader_api_dfitc_sop_qry_investor_position(trader_trader_api* self);
static int trader_trader_api_dfitc_sop_qry_trading_account(trader_trader_api* self);

static void dfitc_sop_on_front_connected(void* arg);
static void dfitc_sop_on_front_disconnected(void* arg, int nReason);
static void dfitc_sop_on_rtn_notice(void* arg, DFITCSECRspNoticeField* pNotice);
static void dfitc_sop_on_rsp_error(void* arg, DFITCSECRspInfoField* pRspInfo);
static void dfitc_sop_on_rsp_stock_user_login(void* arg, DFITCSECRspUserLoginField* pData, DFITCSECRspInfoField* pRspInfo);
static void dfitc_sop_on_rsp_stock_user_logout(void* arg, DFITCSECRspUserLogoutField* pData, DFITCSECRspInfoField* pRspInfo);
static void dfitc_sop_on_rsp_stock_user_password_update(void* arg, DFITCSECRspPasswordUpdateField* pData, DFITCSECRspInfoField* pRspInfo);
static void dfitc_sop_on_rsp_stock_entrust_order(void* arg, DFITCStockRspEntrustOrderField* pData, DFITCSECRspInfoField* pRspInfo);
static void dfitc_sop_on_rsp_stock_withdraw_order(void* arg, DFITCSECRspWithdrawOrderField* pData, DFITCSECRspInfoField* pRspInfo);
static void dfitc_sop_on_rsp_stock_qry_entrust_order(void* arg, DFITCStockRspQryEntrustOrderField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast);
static void dfitc_sop_on_rsp_stock_qry_real_time_trade(void* arg, DFITCStockRspQryRealTimeTradeField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast);
static void dfitc_sop_on_rsp_stock_qry_serial_trade(void* arg, DFITCStockRspQrySerialTradeField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast);
static void dfitc_sop_on_rsp_stock_qry_position(void* arg, DFITCStockRspQryPositionField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast);
static void dfitc_sop_on_rsp_stock_qry_capital_account_info(void* arg, DFITCStockRspQryCapitalAccountField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast);
static void dfitc_sop_on_rsp_stock_qry_account_info(void* arg, DFITCStockRspQryAccountField* pData, DFITCSECRspInfoField* pRspInfo);
static void dfitc_sop_on_rsp_stock_qry_shareholder_info(void* arg, DFITCStockRspQryShareholderField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast);
static void dfitc_sop_on_rsp_stock_transfer_funds(void* arg, DFITCStockRspTransferFundsField* pData, DFITCSECRspInfoField* pRspInfo);
static void dfitc_sop_on_rsp_stock_entrust_batch_order(void* arg, DFITCStockRspEntrustBatchOrderField* pData, DFITCSECRspInfoField* pRspInfo);
static void dfitc_sop_on_rsp_stock_withdraw_batch_order(void* arg, DFITCStockRspWithdrawBatchOrderField* pData, DFITCSECRspInfoField* pRspInfo);
static void dfitc_sop_on_rsp_stock_calc_able_entrust_qty(void* arg, DFITCStockRspCalcAbleEntrustQtyField* pData, DFITCSECRspInfoField* pRspInfo);
static void dfitc_sop_on_rsp_stock_calc_able_purchase_etf_qty(void* arg, DFITCStockRspCalcAblePurchaseETFQtyField* pData, DFITCSECRspInfoField* pRspInfo);
static void dfitc_sop_on_rsp_stock_qry_freeze_funds_detail(void* arg, DFITCStockRspQryFreezeFundsDetailField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast);
static void dfitc_sop_on_rsp_stock_qry_freeze_stock_detail(void* arg, DFITCStockRspQryFreezeStockDetailField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast);
static void dfitc_sop_on_rsp_stock_qry_transfer_stock_detail(void* arg, DFITCStockRspQryTransferStockDetailField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast);
static void dfitc_sop_on_rsp_stock_qry_transfer_funds_detail(void* arg, DFITCStockRspQryTransferFundsDetailField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast);
static void dfitc_sop_on_rsp_stock_qry_stock_info(void* arg, DFITCStockRspQryStockField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast);
static void dfitc_sop_on_rsp_stock_qry_stock_static_info(void* arg, DFITCStockRspQryStockStaticField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast);
static void dfitc_sop_on_rsp_stock_qry_tradetime(void* arg, DFITCStockRspQryTradeTimeField* pData, DFITCSECRspInfoField* pRspInfo);
static void dfitc_sop_on_stock_entrust_order_rtn(void* arg, DFITCStockEntrustOrderRtnField* pData);
static void dfitc_sop_on_stock_trade_rtn(void* arg, DFITCStockTradeRtnField* pData);
static void dfitc_sop_on_stock_withdraw_order_rtn(void* arg, DFITCStockWithdrawOrderRtnField* pData);
static void dfitc_sop_on_rsp_sop_user_login(void* arg, DFITCSECRspUserLoginField* pData, DFITCSECRspInfoField* pRspInfo);
static void dfitc_sop_on_rsp_sop_user_logout(void* arg, DFITCSECRspUserLogoutField* pData, DFITCSECRspInfoField* pRspInfo);
static void dfitc_sop_on_rsp_sop_user_password_update(void* arg, DFITCSECRspPasswordUpdateField* pData, DFITCSECRspInfoField* pRspInfo);
static void dfitc_sop_on_rsp_sop_entrust_order(void* arg, DFITCSOPRspEntrustOrderField* pData, DFITCSECRspInfoField* pRspInfo);
static void dfitc_sop_on_rsp_sop_group_split(void* arg, DFITCSOPRspEntrustOrderField* pData, DFITCSECRspInfoField* pRspInfo);
static void dfitc_sop_on_rsp_sop_qry_group_position(void* arg, DFITCSOPRspQryGroupPositionField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast);
static void dfitc_sop_on_rsp_sop_lock_o_unlock_stock(void* arg, DFITCSOPRspLockOUnLockStockField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast);
static void dfitc_sop_on_rsp_sop_withdraw_order(void* arg, DFITCSECRspWithdrawOrderField* pData, DFITCSECRspInfoField* pRspInfo);
static void dfitc_sop_on_rsp_sop_qry_entrust_order(void* arg, DFITCSOPRspQryEntrustOrderField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast);
static void dfitc_sop_on_rsp_sop_qry_serial_trade(void* arg, DFITCSOPRspQrySerialTradeField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast);
static void dfitc_sop_on_rsp_sop_qry_position(void* arg, DFITCSOPRspQryPositionField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast);
static void dfitc_sop_on_rsp_sop_qry_collateral_position(void* arg, DFITCSOPRspQryCollateralPositionField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast);
static void dfitc_sop_on_rsp_sop_qry_capital_account_info(void* arg, DFITCSOPRspQryCapitalAccountField* pData, DFITCSECRspInfoField* pRspInfo);
static void dfitc_sop_on_rsp_sop_qry_account_info(void* arg, DFITCSOPRspQryAccountField* pData, DFITCSECRspInfoField* pRspInfo);
static void dfitc_sop_on_rsp_sop_qry_shareholder_info(void* arg, DFITCSOPRspQryShareholderField* pData, DFITCSECRspInfoField* pRspInfo);
static void dfitc_sop_on_rsp_sop_calc_able_entrust_qty(void* arg, DFITCSOPRspCalcAbleEntrustQtyField* pData, DFITCSECRspInfoField* pRspInfo);
static void dfitc_sop_on_rsp_sop_qry_able_lock_stock(void* arg, DFITCSOPRspQryAbleLockStockField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast);
static void dfitc_sop_on_rsp_sop_qry_contact_info(void* arg, DFITCSOPRspQryContactField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast);
static void dfitc_sop_on_rsp_sop_exectue_order(void* arg, DFITCSOPRspExectueOrderField* pData, DFITCSECRspInfoField* pRspInfo);
static void dfitc_sop_on_rsp_sop_qry_exec_assi_info(void* arg, DFITCSOPRspQryExecAssiInfoField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast);
static void dfitc_sop_on_rsp_sop_qry_trade_time(void* arg, DFITCSOPRspQryTradeTimeField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast);
static void dfitc_sop_on_rsp_sop_qry_exchange_info(void* arg, DFITCSOPRspQryExchangeInfoField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast);
static void dfitc_sop_on_rsp_sop_qry_commission(void* arg, DFITCSOPRspQryCommissionField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast);
static void dfitc_sop_on_rsp_sop_qry_deposit(void* arg, DFITCSOPRspQryDepositField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast);
static void dfitc_sop_on_rsp_sop_qry_contract_object_info(void* arg, DFITCSOPRspQryContractObjectField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast);
static void dfitc_sop_on_sop_entrust_order_rtn(void* arg, DFITCSOPEntrustOrderRtnField* pData);
static void dfitc_sop_on_sop_trade_rtn(void* arg, DFITCSOPTradeRtnField* pData);
static void dfitc_sop_on_sop_withdraw_order_rtn(void* arg, DFITCSOPWithdrawOrderRtnField* pData);
static void dfitc_sop_on_rsp_fasl_user_login(void* arg, DFITCSECRspUserLoginField* pData, DFITCSECRspInfoField* pRspInfo);
static void dfitc_sop_on_rsp_fasl_user_logout(void* arg, DFITCSECRspUserLogoutField* pData, DFITCSECRspInfoField* pRspInfo);
static void dfitc_sop_on_rsp_fasl_qry_able_fin_info(void* arg, DFITCFASLRspAbleFinInfoField* pData, DFITCSECRspInfoField* pRspInfo);
static void dfitc_sop_on_rsp_fasl_qry_able_slo_info(void* arg, DFITCFASLRspAbleSloInfoField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast);
static void dfitc_sop_on_rsp_fasl_transfer_collateral(void* arg, DFITCFASLRspTransferCollateralField* pData, DFITCSECRspInfoField* pRspInfo);
static void dfitc_sop_on_rsp_fasl_direct_repayment(void* arg, DFITCFASLRspDirectRepaymentField* pData, DFITCSECRspInfoField* pRspInfo);
static void dfitc_sop_on_rsp_fasl_repay_stock_transfer(void* arg, DFITCFASLRspRepayStockTransferField* pData, DFITCSECRspInfoField* pRspInfo);
static void dfitc_sop_on_rsp_fasl_entrust_crdt_order(void* arg, DFITCFASLRspEntrustCrdtOrderField* pData, DFITCSECRspInfoField* pRspInfo);
static void dfitc_sop_on_rsp_fasl_entrust_order(void* arg, DFITCFASLRspEntrustOrderField* pData, DFITCSECRspInfoField* pRspInfo);
static void dfitc_sop_on_rsp_fasl_calc_able_entrust_crdt_qty(void* arg, DFITCFASLRspCalcAbleEntrustCrdtQtyField* pData, DFITCSECRspInfoField* pRspInfo);
static void dfitc_sop_on_rsp_fasl_qrycrdt_funds(void* arg, DFITCFASLRspQryCrdtFundsField* pData, DFITCSECRspInfoField* pRspInfo);
static void dfitc_sop_on_rsp_fasl_qrycrdt_contract(void* arg, DFITCFASLRspQryCrdtContractField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast);
static void dfitc_sop_on_rsp_fasl_qrycrdt_con_change_info(void* arg, DFITCFASLRspQryCrdtConChangeInfoField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast);
static void dfitc_sop_on_rsp_fasl_transfer_funds(void* arg, DFITCStockRspTransferFundsField* pData, DFITCSECRspInfoField* pRspInfo);
static void dfitc_sop_on_rsp_fasl_qry_account_info(void* arg, DFITCStockRspQryAccountField* pData, DFITCSECRspInfoField* pRspInfo);
static void dfitc_sop_on_rsp_fasl_qry_capital_account_info(void* arg, DFITCStockRspQryCapitalAccountField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast);
static void dfitc_sop_on_rsp_fasl_qry_shareholder_info(void* arg, DFITCStockRspQryShareholderField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast);
static void dfitc_sop_on_rsp_fasl_qry_position(void* arg, DFITCStockRspQryPositionField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast);
static void dfitc_sop_on_rsp_fasl_qry_entrust_order(void* arg, DFITCStockRspQryEntrustOrderField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast);
static void dfitc_sop_on_rsp_fasl_qry_serial_trade(void* arg, DFITCStockRspQrySerialTradeField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast);
static void dfitc_sop_on_rsp_fasl_qry_real_time_trade(void* arg, DFITCStockRspQryRealTimeTradeField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast);
static void dfitc_sop_on_rsp_fasl_qry_freeze_funds_detail(void* arg, DFITCStockRspQryFreezeFundsDetailField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast);
static void dfitc_sop_on_rsp_fasl_qry_freeze_stock_detail(void* arg, DFITCStockRspQryFreezeStockDetailField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast);
static void dfitc_sop_on_rsp_fasl_qry_transfer_funds_detail(void* arg, DFITCStockRspQryTransferFundsDetailField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast);
static void dfitc_sop_on_rsp_fasl_withdraw_order(void* arg, DFITCFASLRspWithdrawOrderField* pData, DFITCSECRspInfoField* pRspInfo);
static void dfitc_sop_on_rsp_fasl_qry_system_time(void* arg, DFITCFASLRspQryTradeTimeField* pData, DFITCSECRspInfoField* pRspInfo);
static void dfitc_sop_on_rsp_fasl_qry_transferred_contract(void* arg, DFITCFASLRspQryTransferredContractField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast);
static void dfitc_sop_on_rsp_fasl_desirable_funds_out(void* arg, DFITCFASLRspDesirableFundsOutField* pData, DFITCSECRspInfoField* pRspInfo);
static void dfitc_sop_on_rsp_fasl_qry_guaranteed_contract(void* arg, DFITCFASLRspQryGuaranteedContractField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast);
static void dfitc_sop_on_rsp_fasl_qry_underlying_contract(void* arg, DFITCFASLRspQryUnderlyingContractField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast);
static void dfitc_sop_on_fasl_entrust_order_rtn(void* arg, DFITCStockEntrustOrderRtnField* pData);
static void dfitc_sop_on_fasl_trade_rtn(void* arg, DFITCStockTradeRtnField* pData);
static void dfitc_sop_on_fasl_withdraw_order_rtn(void* arg, DFITCStockWithdrawOrderRtnField* pData);

static dfitc_sec_trader_api_cb* dfitc_sop_trader_api_cb_get();

#ifdef __cplusplus
}
#endif

trader_trader_api_method* trader_trader_api_dfitc_sop_method_get()
{
  static trader_trader_api_method trader_trader_api_method_st = {
    trader_trader_api_set_user,
    trader_trader_api_set_front_addr,
    trader_trader_api_set_workspace,
    trader_trader_api_set_param,
    trader_trader_api_get_trading_day,
    trader_trader_api_get_max_order_local_id,
    trader_trader_api_dfitc_sop_start,
    trader_trader_api_dfitc_sop_stop,
    trader_trader_api_dfitc_sop_login,
    trader_trader_api_dfitc_sop_logout,
    trader_trader_api_dfitc_sop_order_insert,
    trader_trader_api_dfitc_sop_order_action,
    trader_trader_api_dfitc_sop_qry_instrument,
    trader_trader_api_dfitc_sop_qry_user_investor,
    trader_trader_api_dfitc_sop_qry_investor_position,
    trader_trader_api_dfitc_sop_qry_trading_account
  };

  return &trader_trader_api_method_st;
}


void trader_trader_api_dfitc_sop_start(trader_trader_api* self)
{
  DFITCSECTraderApi* pTraderApi = DFITCSECTraderApi::CreateDFITCSECTraderApi(self->pWorkspace, self->pWorkspace);
  CDfitcSecTraderHandler* pTraderHandler = new CDfitcSecTraderHandler(dfitc_sop_trader_api_cb_get(), (void*)self);
  
  trader_trader_api_dfitc_sop* pImp = (trader_trader_api_dfitc_sop*)malloc(sizeof(trader_trader_api_dfitc_sop));
  pImp->pTraderApi = (void*)pTraderApi;
  pImp->pTraderHandler = (void*)pTraderHandler;

  pImp->nTraderRequestID = 0;
  self->pUserApi = (void*)pImp;
  
  pTraderApi->SubscribePrivateTopic(TERT_RESUME);
  
  // 连接交易服务器
  pTraderApi->Init(self->pAddress, pTraderHandler);

  return;
}

void trader_trader_api_dfitc_sop_stop(trader_trader_api* self)
{
  trader_trader_api_dfitc_sop* pImp = (trader_trader_api_dfitc_sop*)self->pUserApi;
  DFITCSECTraderApi* pTraderApi = (DFITCSECTraderApi*)pImp->pTraderApi;
  CDfitcSecTraderHandler* pTraderHandler = (CDfitcSecTraderHandler*)pImp->pTraderHandler;
  
  pTraderApi->Release();
  delete pTraderHandler;
  
  free(pImp);
  self->pUserApi = (void*)NULL;
  
  return;
}

void trader_trader_api_dfitc_sop_login(trader_trader_api* self)
{
  trader_trader_api_dfitc_sop* pImp = (trader_trader_api_dfitc_sop*)self->pUserApi;
  DFITCSECTraderApi* pTraderApi = (DFITCSECTraderApi*)pImp->pTraderApi;
  
  DFITCSECReqUserLoginField reqUserLoginField;
  memset(&reqUserLoginField, 0, sizeof(reqUserLoginField));
  
  reqUserLoginField.requestID = pImp->nTraderRequestID++;
  strcpy(reqUserLoginField.accountID, self->pUser);
  strcpy(reqUserLoginField.password, self->pPwd);
  reqUserLoginField.compressflag = DFITCSEC_COMPRESS_FALSE;
  
  pTraderApi->ReqSOPUserLogin(&reqUserLoginField);
  return ;
}

void trader_trader_api_dfitc_sop_logout(trader_trader_api* self)
{
  trader_trader_api_dfitc_sop* pImp = (trader_trader_api_dfitc_sop*)self->pUserApi;
  DFITCSECTraderApi* pTraderApi = (DFITCSECTraderApi*)pImp->pTraderApi;

  DFITCSECReqUserLogoutField userLogoutField;
  memset(&userLogoutField, 0, sizeof(userLogoutField));
  
  userLogoutField.requestID = pImp->nTraderRequestID++;
  strcpy(userLogoutField.accountID, self->pUser);
  
  pTraderApi->ReqSOPUserLogout(&userLogoutField);
  return ;
}

  
int trader_trader_api_dfitc_sop_order_insert(trader_trader_api* self, char* inst, char* local_id, char buy_sell, char open_close, double price, int vol)
{
  trader_trader_api_dfitc_sop* pImp = (trader_trader_api_dfitc_sop*)self->pUserApi;
  DFITCSECTraderApi* pTraderApi = (DFITCSECTraderApi*)pImp->pTraderApi;

  DFITCSOPReqEntrustOrderField inputOrderField;
  memset(&inputOrderField, 0, sizeof(inputOrderField));
  
  //请求ID(Y)
  inputOrderField.requestID = pImp->nTraderRequestID++;
  //本地委托号(Y)
	inputOrderField.localOrderID = atol(local_id);
  //客户号(Y)
	strcpy(inputOrderField.accountID, self->pUser);
  //交易所代码(Y)
  //TODO
	strcpy(inputOrderField.exchangeID, "SH");
  //证劵代码(Y)
	strcpy(inputOrderField.securityID, inst);
  //子账户编码(N)(预留字段)
	strcpy(inputOrderField.subAccountID, "");
  //委托数量(Y)
	inputOrderField.entrustQty = vol;
  //委托价格(N)
	inputOrderField.entrustPrice = price;
  //委托类别(Y)
  if('0' == buy_sell){
	  inputOrderField.entrustDirection = DFITCSEC_ED_Buy;
  }else{
	  inputOrderField.entrustDirection = DFITCSEC_ED_Sell;
  }
  //开平标志(Y)
  if('0' == open_close){
    inputOrderField.openCloseFlag = DFITCSEC_OCF_Open;
  }else{
    inputOrderField.openCloseFlag = DFITCSEC_OCF_Close;
  }
  //备兑类型(Y)
	inputOrderField.coveredFlag = DFITCSEC_CF_UnCovered;
  //订单类型(Y)
	inputOrderField.orderType = DFITCSEC_SOP_LimitPrice;
  //订单时效限制(N)
  inputOrderField.orderExpiryDate = DFITCSEC_OE_NONE;
  //委托单类别(Y)
  inputOrderField.orderCategory = DFITCSEC_OC_GeneralOrder;
  //扩展流水号(N)(预留字段)
 // inputOrderField.serialID = 0;
  //厂商ID(N)
	//strcpy(inputOrderField.devID, "");
  //用户自定义字段(N)
	//strcpy(inputOrderField.devDecInfo, "");

  pTraderApi->ReqSOPEntrustOrder(&inputOrderField);
  return 0;
}

int trader_trader_api_dfitc_sop_order_action(trader_trader_api* self, char* inst, char* local_id, char* org_local_id, char* order_sys_id)
{
  trader_trader_api_dfitc_sop* pImp = (trader_trader_api_dfitc_sop*)self->pUserApi;
  DFITCSECTraderApi* pTraderApi = (DFITCSECTraderApi*)pImp->pTraderApi;

  DFITCSECReqWithdrawOrderField inputOrderActionField;
  memset(&inputOrderActionField, 0, sizeof(inputOrderActionField));

  //请求ID(Y)
  inputOrderActionField.requestID = pImp->nTraderRequestID++;
  //客户号(Y)
	strcpy(inputOrderActionField.accountID, self->pUser);
  //柜台委托号(N)
	inputOrderActionField.spdOrderID = 0;
  //本地委托号(N)(柜台委托号和本地委托号二选一)
	inputOrderActionField.localOrderID = atol(org_local_id);
  
  pTraderApi->ReqSOPWithdrawOrder(&inputOrderActionField);
  return 0;
}

 
int trader_trader_api_dfitc_sop_qry_instrument(trader_trader_api* self)
{
  trader_trader_api_dfitc_sop* pImp = (trader_trader_api_dfitc_sop*)self->pUserApi;
  DFITCSECTraderApi* pTraderApi = (DFITCSECTraderApi*)pImp->pTraderApi;

  DFITCSOPReqQryContactField qryInstrumentField;
  memset(&qryInstrumentField, 0, sizeof(qryInstrumentField));

  //请求ID(Y)
  qryInstrumentField.requestID = pImp->nTraderRequestID++;
  //客户号(Y)
	strcpy(qryInstrumentField.accountID, self->pUser);
  /*
  DFITCSECExchangeIDType               exchangeID;               //交易所代码(N)
  DFITCSECSecurityIDType               securityOptionID;         //期权代码(N)
  DFITCSECSecurityIDType               securityID;               //标的代码(N)
  DFITCSECContractObjectTypeType       contractObjectType;       //标的类型(N)
  DFITCSECOptionTypeType               optType;                  //期权类型(N)
  DFITCSECSecurityIDType               rowIndex;                 //分页索引值(预留字段)(N)
  DFITCSECQuantityType                 rowCount;                 //每页查询笔数(预留字段)(N)
  */

  pTraderApi->ReqSOPQryContactInfo(&qryInstrumentField);
  return 0;
}

int trader_trader_api_dfitc_sop_qry_user_investor(trader_trader_api* self)
{
  trader_trader_api_dfitc_sop* pImp = (trader_trader_api_dfitc_sop*)self->pUserApi;
  DFITCSECTraderApi* pTraderApi = (DFITCSECTraderApi*)pImp->pTraderApi;

  DFITCSOPReqQryAccountField qryInvestorField;
  memset(&qryInvestorField, 0, sizeof(qryInvestorField));
  
  //请求ID(Y)
  qryInvestorField.requestID = pImp->nTraderRequestID++;
  //客户号(Y)
	strcpy(qryInvestorField.accountID, self->pUser);
  
  pTraderApi->ReqSOPQryAccountInfo(&qryInvestorField);
  return 0;
}

int trader_trader_api_dfitc_sop_qry_investor_position(trader_trader_api* self)
{
  trader_trader_api_dfitc_sop* pImp = (trader_trader_api_dfitc_sop*)self->pUserApi;
  DFITCSECTraderApi* pTraderApi = (DFITCSECTraderApi*)pImp->pTraderApi;

  DFITCSOPReqQryPositionField qryInvestorPositionField;
  memset(&qryInvestorPositionField, 0, sizeof(qryInvestorPositionField));

  //请求ID(Y)
  qryInvestorPositionField.requestID = pImp->nTraderRequestID++;
  //客户号(Y)
	strcpy(qryInvestorPositionField.accountID, self->pUser);
  /*
  DFITCSECExchangeIDType               exchangeID;               //交易所代码(N)
  DFITCSECSecurityIDType               securityOptionID;         //期权代码(N)
  DFITCSECQuantityType                 rowIndex;                 //分页索引值(N)
  DFITCSECQuantityType                 rowCount;                 //每页查询笔数(N)
  */

  pTraderApi->ReqSOPQryPosition(&qryInvestorPositionField);
  return 0;
}

int trader_trader_api_dfitc_sop_qry_trading_account(trader_trader_api* self)
{
  trader_trader_api_dfitc_sop* pImp = (trader_trader_api_dfitc_sop*)self->pUserApi;
  DFITCSECTraderApi* pTraderApi = (DFITCSECTraderApi*)pImp->pTraderApi;
  
  DFITCSOPReqQryCapitalAccountField qryTradingAccountField;
  memset(&qryTradingAccountField, 0, sizeof(qryTradingAccountField));

  //请求ID(Y)
  qryTradingAccountField.requestID = pImp->nTraderRequestID++;
  //客户号(Y)
	strcpy(qryTradingAccountField.accountID, self->pUser);
  /*
  DFITCSECCurrencyType                 currency;                 //币种(N)(当前只支持人民币)
  DFITCSECFundsQryFlagType             FundsQryFlag;             //查询标志(预留字段)
  */
  pTraderApi->ReqSOPQryCapitalAccountInfo(&qryTradingAccountField);
  return 0;
}

dfitc_sec_trader_api_cb* dfitc_sop_trader_api_cb_get()
{
  static dfitc_sec_trader_api_cb dfitc_sop_trader_api_cb_st = {
    dfitc_sop_on_front_connected,
    dfitc_sop_on_front_disconnected,
    dfitc_sop_on_rtn_notice,
    dfitc_sop_on_rsp_error,
    dfitc_sop_on_rsp_stock_user_login,
    dfitc_sop_on_rsp_stock_user_logout,
    dfitc_sop_on_rsp_stock_user_password_update,
    dfitc_sop_on_rsp_stock_entrust_order,
    dfitc_sop_on_rsp_stock_withdraw_order,
    dfitc_sop_on_rsp_stock_qry_entrust_order,
    dfitc_sop_on_rsp_stock_qry_real_time_trade,
    dfitc_sop_on_rsp_stock_qry_serial_trade,
    dfitc_sop_on_rsp_stock_qry_position,
    dfitc_sop_on_rsp_stock_qry_capital_account_info,
    dfitc_sop_on_rsp_stock_qry_account_info,
    dfitc_sop_on_rsp_stock_qry_shareholder_info,
    dfitc_sop_on_rsp_stock_transfer_funds,
    dfitc_sop_on_rsp_stock_entrust_batch_order,
    dfitc_sop_on_rsp_stock_withdraw_batch_order,
    dfitc_sop_on_rsp_stock_calc_able_entrust_qty,
    dfitc_sop_on_rsp_stock_calc_able_purchase_etf_qty,
    dfitc_sop_on_rsp_stock_qry_freeze_funds_detail,
    dfitc_sop_on_rsp_stock_qry_freeze_stock_detail,
    dfitc_sop_on_rsp_stock_qry_transfer_stock_detail,
    dfitc_sop_on_rsp_stock_qry_transfer_funds_detail,
    dfitc_sop_on_rsp_stock_qry_stock_info,
    dfitc_sop_on_rsp_stock_qry_stock_static_info,
    dfitc_sop_on_rsp_stock_qry_tradetime,
    dfitc_sop_on_stock_entrust_order_rtn,
    dfitc_sop_on_stock_trade_rtn,
    dfitc_sop_on_stock_withdraw_order_rtn,
    dfitc_sop_on_rsp_sop_user_login,
    dfitc_sop_on_rsp_sop_user_logout,
    dfitc_sop_on_rsp_sop_user_password_update,
    dfitc_sop_on_rsp_sop_entrust_order,
    dfitc_sop_on_rsp_sop_group_split,
    dfitc_sop_on_rsp_sop_qry_group_position,
    dfitc_sop_on_rsp_sop_lock_o_unlock_stock,
    dfitc_sop_on_rsp_sop_withdraw_order,
    dfitc_sop_on_rsp_sop_qry_entrust_order,
    dfitc_sop_on_rsp_sop_qry_serial_trade,
    dfitc_sop_on_rsp_sop_qry_position,
    dfitc_sop_on_rsp_sop_qry_collateral_position,
    dfitc_sop_on_rsp_sop_qry_capital_account_info,
    dfitc_sop_on_rsp_sop_qry_account_info,
    dfitc_sop_on_rsp_sop_qry_shareholder_info,
    dfitc_sop_on_rsp_sop_calc_able_entrust_qty,
    dfitc_sop_on_rsp_sop_qry_able_lock_stock,
    dfitc_sop_on_rsp_sop_qry_contact_info,
    dfitc_sop_on_rsp_sop_exectue_order,
    dfitc_sop_on_rsp_sop_qry_exec_assi_info,
    dfitc_sop_on_rsp_sop_qry_trade_time,
    dfitc_sop_on_rsp_sop_qry_exchange_info,
    dfitc_sop_on_rsp_sop_qry_commission,
    dfitc_sop_on_rsp_sop_qry_deposit,
    dfitc_sop_on_rsp_sop_qry_contract_object_info,
    dfitc_sop_on_sop_entrust_order_rtn,
    dfitc_sop_on_sop_trade_rtn,
    dfitc_sop_on_sop_withdraw_order_rtn,
    dfitc_sop_on_rsp_fasl_user_login,
    dfitc_sop_on_rsp_fasl_user_logout,
    dfitc_sop_on_rsp_fasl_qry_able_fin_info,
    dfitc_sop_on_rsp_fasl_qry_able_slo_info,
    dfitc_sop_on_rsp_fasl_transfer_collateral,
    dfitc_sop_on_rsp_fasl_direct_repayment,
    dfitc_sop_on_rsp_fasl_repay_stock_transfer,
    dfitc_sop_on_rsp_fasl_entrust_crdt_order,
    dfitc_sop_on_rsp_fasl_entrust_order,
    dfitc_sop_on_rsp_fasl_calc_able_entrust_crdt_qty,
    dfitc_sop_on_rsp_fasl_qrycrdt_funds,
    dfitc_sop_on_rsp_fasl_qrycrdt_contract,
    dfitc_sop_on_rsp_fasl_qrycrdt_con_change_info,
    dfitc_sop_on_rsp_fasl_transfer_funds,
    dfitc_sop_on_rsp_fasl_qry_account_info,
    dfitc_sop_on_rsp_fasl_qry_capital_account_info,
    dfitc_sop_on_rsp_fasl_qry_shareholder_info,
    dfitc_sop_on_rsp_fasl_qry_position,
    dfitc_sop_on_rsp_fasl_qry_entrust_order,
    dfitc_sop_on_rsp_fasl_qry_serial_trade,
    dfitc_sop_on_rsp_fasl_qry_real_time_trade,
    dfitc_sop_on_rsp_fasl_qry_freeze_funds_detail,
    dfitc_sop_on_rsp_fasl_qry_freeze_stock_detail,
    dfitc_sop_on_rsp_fasl_qry_transfer_funds_detail,
    dfitc_sop_on_rsp_fasl_withdraw_order,
    dfitc_sop_on_rsp_fasl_qry_system_time,
    dfitc_sop_on_rsp_fasl_qry_transferred_contract,
    dfitc_sop_on_rsp_fasl_desirable_funds_out,
    dfitc_sop_on_rsp_fasl_qry_guaranteed_contract,
    dfitc_sop_on_rsp_fasl_qry_underlying_contract,
    dfitc_sop_on_fasl_entrust_order_rtn,
    dfitc_sop_on_fasl_trade_rtn,
    dfitc_sop_on_fasl_withdraw_order_rtn
  };

  return &dfitc_sop_trader_api_cb_st;
}

void dfitc_sop_on_front_connected(void* arg)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  trader_trader_api_on_front_connected(self);
  return ;
}

void dfitc_sop_on_front_disconnected(void* arg, int nReason)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  trader_trader_api_on_front_disconnected(self, nReason);
  return ;
}

void dfitc_sop_on_rtn_notice(void* arg, DFITCSECRspNoticeField* pNotice)
{
}

void dfitc_sop_on_rsp_error(void* arg, DFITCSECRspInfoField* pRspInfo)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->errorID;
    errMsg = pRspInfo->errorMsg;
  }
  
  trader_trader_api_on_rsp_error(self, errNo, errMsg);

}

void dfitc_sop_on_rsp_stock_user_login(void* arg, DFITCSECRspUserLoginField* pData, DFITCSECRspInfoField* pRspInfo)
{
}

void dfitc_sop_on_rsp_stock_user_logout(void* arg, DFITCSECRspUserLogoutField* pData, DFITCSECRspInfoField* pRspInfo)
{
}

void dfitc_sop_on_rsp_stock_user_password_update(void* arg, DFITCSECRspPasswordUpdateField* pData, DFITCSECRspInfoField* pRspInfo)
{
}

void dfitc_sop_on_rsp_stock_entrust_order(void* arg, DFITCStockRspEntrustOrderField* pData, DFITCSECRspInfoField* pRspInfo)
{
}

void dfitc_sop_on_rsp_stock_withdraw_order(void* arg, DFITCSECRspWithdrawOrderField* pData, DFITCSECRspInfoField* pRspInfo)
{
}

void dfitc_sop_on_rsp_stock_qry_entrust_order(void* arg, DFITCStockRspQryEntrustOrderField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast)
{
}

void dfitc_sop_on_rsp_stock_qry_real_time_trade(void* arg, DFITCStockRspQryRealTimeTradeField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast)
{
}

void dfitc_sop_on_rsp_stock_qry_serial_trade(void* arg, DFITCStockRspQrySerialTradeField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast)
{
}

void dfitc_sop_on_rsp_stock_qry_position(void* arg, DFITCStockRspQryPositionField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast)
{
}

void dfitc_sop_on_rsp_stock_qry_capital_account_info(void* arg, DFITCStockRspQryCapitalAccountField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast)
{
}

void dfitc_sop_on_rsp_stock_qry_account_info(void* arg, DFITCStockRspQryAccountField* pData, DFITCSECRspInfoField* pRspInfo)
{
}

void dfitc_sop_on_rsp_stock_qry_shareholder_info(void* arg, DFITCStockRspQryShareholderField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast)
{
}

void dfitc_sop_on_rsp_stock_transfer_funds(void* arg, DFITCStockRspTransferFundsField* pData, DFITCSECRspInfoField* pRspInfo)
{
}

void dfitc_sop_on_rsp_stock_entrust_batch_order(void* arg, DFITCStockRspEntrustBatchOrderField* pData, DFITCSECRspInfoField* pRspInfo)
{
}

void dfitc_sop_on_rsp_stock_withdraw_batch_order(void* arg, DFITCStockRspWithdrawBatchOrderField* pData, DFITCSECRspInfoField* pRspInfo)
{
}

void dfitc_sop_on_rsp_stock_calc_able_entrust_qty(void* arg, DFITCStockRspCalcAbleEntrustQtyField* pData, DFITCSECRspInfoField* pRspInfo)
{
}

void dfitc_sop_on_rsp_stock_calc_able_purchase_etf_qty(void* arg, DFITCStockRspCalcAblePurchaseETFQtyField* pData, DFITCSECRspInfoField* pRspInfo)
{
}

void dfitc_sop_on_rsp_stock_qry_freeze_funds_detail(void* arg, DFITCStockRspQryFreezeFundsDetailField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast)
{
}

void dfitc_sop_on_rsp_stock_qry_freeze_stock_detail(void* arg, DFITCStockRspQryFreezeStockDetailField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast)
{
}

void dfitc_sop_on_rsp_stock_qry_transfer_stock_detail(void* arg, DFITCStockRspQryTransferStockDetailField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast)
{
}

void dfitc_sop_on_rsp_stock_qry_transfer_funds_detail(void* arg, DFITCStockRspQryTransferFundsDetailField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast)
{
}

void dfitc_sop_on_rsp_stock_qry_stock_info(void* arg, DFITCStockRspQryStockField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast)
{
}

void dfitc_sop_on_rsp_stock_qry_stock_static_info(void* arg, DFITCStockRspQryStockStaticField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast)
{
}

void dfitc_sop_on_rsp_stock_qry_tradetime(void* arg, DFITCStockRspQryTradeTimeField* pData, DFITCSECRspInfoField* pRspInfo)
{
}

void dfitc_sop_on_stock_entrust_order_rtn(void* arg, DFITCStockEntrustOrderRtnField* pData)
{
}

void dfitc_sop_on_stock_trade_rtn(void* arg, DFITCStockTradeRtnField* pData)
{
}

void dfitc_sop_on_stock_withdraw_order_rtn(void* arg, DFITCStockWithdrawOrderRtnField* pData)
{
}

void dfitc_sop_on_rsp_sop_user_login(void* arg, DFITCSECRspUserLoginField* pData, DFITCSECRspInfoField* pRspInfo)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  trader_trader_api_dfitc_sop* pImp = (trader_trader_api_dfitc_sop*)self->pUserApi;
  
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->errorID;
    errMsg = pRspInfo->errorMsg;
  }
  
  trader_trader_api_on_rsp_user_login(self, errNo, errMsg);
  
  return ;
}

void dfitc_sop_on_rsp_sop_user_logout(void* arg, DFITCSECRspUserLogoutField* pData, DFITCSECRspInfoField* pRspInfo)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->errorID;
    errMsg = pRspInfo->errorMsg;
  }
  
  trader_trader_api_on_rsp_user_logout(self, errNo, errMsg);
  return ;
}

void dfitc_sop_on_rsp_sop_user_password_update(void* arg, DFITCSECRspPasswordUpdateField* pData, DFITCSECRspInfoField* pRspInfo)
{
}

void dfitc_sop_on_rsp_sop_entrust_order(void* arg, DFITCSOPRspEntrustOrderField* pData, DFITCSECRspInfoField* pRspInfo)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->errorID;
    errMsg = pRspInfo->errorMsg;
  }
  
  trader_trader_api_on_rsp_order_insert(self, errNo, errMsg);

}

void dfitc_sop_on_rsp_sop_group_split(void* arg, DFITCSOPRspEntrustOrderField* pData, DFITCSECRspInfoField* pRspInfo)
{
}

void dfitc_sop_on_rsp_sop_qry_group_position(void* arg, DFITCSOPRspQryGroupPositionField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast)
{
}

void dfitc_sop_on_rsp_sop_lock_o_unlock_stock(void* arg, DFITCSOPRspLockOUnLockStockField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast)
{
}

void dfitc_sop_on_rsp_sop_withdraw_order(void* arg, DFITCSECRspWithdrawOrderField* pData, DFITCSECRspInfoField* pRspInfo)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->errorID;
    errMsg = pRspInfo->errorMsg;
  }
  
  trader_trader_api_on_rsp_order_action(self, errNo, errMsg);

}

void dfitc_sop_on_rsp_sop_qry_entrust_order(void* arg, DFITCSOPRspQryEntrustOrderField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast)
{
}

void dfitc_sop_on_rsp_sop_qry_serial_trade(void* arg, DFITCSOPRspQrySerialTradeField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast)
{
}

void dfitc_sop_on_rsp_sop_qry_position(void* arg, DFITCSOPRspQryPositionField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  trader_position traderPosition;
  memset(&traderPosition, 0, sizeof(traderPosition));
  
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->errorID;
    errMsg = pRspInfo->errorMsg;
  }

  if(pData) {
    //TODO
    /*
    DFITCSECRequestIDType                requestID;                //请求ID
    DFITCSECAccountIDType                accountID;                //客户号
    DFITCSECExchangeIDType               exchangeID;               //交易所代码
    DFITCSECSubAccountIDType             subAccountID;             //子账户编码
    DFITCSECSecurityIDType               securityOptionID;         //期权代码
    DFITCSECContractIDType               contractID;               //期权编码
    DFITCSECContractNameType             contractName;             //期权名称
    DFITCSECEntrustDirectionType         entrustDirection;         //委托类别
    DFITCSECCoveredFlagType              coveredFlag;              //备兑标志
    DFITCSECDateType                     executeDate;              //行权日期
    DFITCSECQuantityType                 totalQty;                 //当前数量
    DFITCSECQuantityType                 availableQty;             //可用数量
    DFITCSECFundsType                    latestPrice;              //最新价
    DFITCSECFundsType                    optionMarket;             //期权市值
    DFITCSECQuantityType                 freezeQty;                //冻结数量
    DFITCSECQuantityType                 executeQty;               //执行数量
    DFITCSECQuantityType                 openEntrustQty;           //开仓委托数量
    DFITCSECQuantityType                 openTradeQty;             //开仓成交数量
    DFITCSECQuantityType                 prePosition;              //昨持仓
    DFITCSECQuantityType                 closeEntrustQty;          //平仓委托数量
    DFITCSECQuantityType                 closeTradeQty;            //平仓成交数量
    DFITCSECFundsType                    deposit;                  //保证金
    DFITCSECFundsType                    openDeposit;              //本日开仓保证金
    DFITCSECFundsType                    closeDeposit;             //本日平仓保证金
    DFITCSECFundsType                    exchangeDeposit;          //交易所保证金
    DFITCSECFundsType                    exchangeOpenDeposit;      //交易所开仓保证金
    DFITCSECFundsType                    exchangeCloseDeposit;     //交易所平仓保证金
    DFITCSECFundsType                    openAvgPrice;             //开仓均价
    DFITCSECOptionTypeType               optType;                  //期权类型
    DFITCSECContractObjectTypeType       contractObjectType;       //标的类型
    DFITCSECContractUnitType             contractUnit;             //合约单位
    DFITCSECCurrencyType                 currency;                 //币种
    DFITCSECFundsType                    totalTradeCost;           //累计交易成本(到昨日位置累计)
    DFITCSECFundsType                    tradeCost;                //本日交易成本(实时更新)
    DFITCSECFundsType                    realizeProfitLoss;        //实现盈亏(预留字段)
    DFITCSECFundsType                    profitLoss;               //盈亏金额(预留字段)
    DFITCSECContractAdjustRemindType     adjustRemind;             //合约调整提醒标志
    DFITCSECContraceExpireRemindType     expireRemind;             //合约即将到期提醒标志
    */
  }

  trader_trader_api_on_rsp_qry_investor_position(self, &traderPosition, errNo, errMsg, bIsLast);

}

void dfitc_sop_on_rsp_sop_qry_collateral_position(void* arg, DFITCSOPRspQryCollateralPositionField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast)
{
}

void dfitc_sop_on_rsp_sop_qry_capital_account_info(void* arg, DFITCSOPRspQryCapitalAccountField* pData, DFITCSECRspInfoField* pRspInfo)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  trader_account traderAccount;
  memset(&traderAccount, 0, sizeof(traderAccount));
  
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->errorID;
    errMsg = pRspInfo->errorMsg;
  }

  if(pData) {
    //TODO
    /*
    DFITCSECRequestIDType                requestID;                //请求ID
    DFITCSECAccountIDType                accountID;                //客户号
    DFITCSECBranchIDType                 branchID;                 //机构编码
    DFITCSECCurrencyType                 currency;                 //币种
    DFITCSECFundsType                    accountBanlance;          //账户余额
    DFITCSECFundsType                    availableFunds;           //可用资金
    DFITCSECFundsType                    freezeFunds;              //冻结资金
    DFITCSECFundsType                    anticipatedInterest;      //预计利息
    DFITCSECFundsType                    usedDeposit;              //占用保证金
    DFITCSECAccountStatusType            accountStatus;            //客户状态
    DFITCSECFundsType                    totalFunds;               //总资金
    DFITCSECFundsType                    totalMarket;              //总市值
    DFITCSECFundsType                    cashAssets;               //现金资产
    DFITCSECFundsType                    execGuaranteeRatio;       //履约担保比例
    DFITCSECFundsType                    buyLimits;                //买入额度

    */
  }

  trader_trader_api_on_rsp_qry_trading_account(self, &traderAccount, errNo, errMsg, 1);

}

void dfitc_sop_on_rsp_sop_qry_account_info(void* arg, DFITCSOPRspQryAccountField* pData, DFITCSECRspInfoField* pRspInfo)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  int errNo = 0;
  char* errMsg = NULL;
  char* investor = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->errorID;
    errMsg = pRspInfo->errorMsg;
  }

  if(pData) {
    /*
    DFITCSECRequestIDType                requestID;                //请求
    DFITCSECAccountIDType                accountID;                //客户号
    DFITCSECAccountNameType              accountName;              //客户姓名
    DFITCSECAccountNameType              accountFullName;          //客户全名
    DFITCSECBranchIDType                 branchID;                 //机构编码
    DFITCSECAccountIdentityTypeType      identityType;             //证件类型
    DFITCSECAccountIdentityIDType        accountIdentityID;        //证件编号
    DFITCSECPhoneIDType                  tel;                      //客户电话
    DFITCSECPhoneIDType                  mobile;                   //移动电话
    DFITCSECAccountTypeType              accountType;              //投资者分类
    DFITCSECAccountPropType              accountProp;              //客户属性
    DFITCSECTradePermissionsType         tradePermissions;         //交易权限(预留字段)
    DFITCSECEntrustTypeType              entrustType;              //委托方式
    DFITCSECAccountStatusType            accountStatus;            //客户状态
    DFITCSECPasswdSyncFlagType           pwdSynFlag;               //密码同步标志
    DFITCSECAccountNodeIDType            accountNodeID;            //客户所属节点编号
    */
    investor = pData->branchID;
  }
  
  trader_trader_api_on_rsp_qry_user_investor(self, investor, errNo, errMsg);

}

void dfitc_sop_on_rsp_sop_qry_shareholder_info(void* arg, DFITCSOPRspQryShareholderField* pData, DFITCSECRspInfoField* pRspInfo)
{
}

void dfitc_sop_on_rsp_sop_calc_able_entrust_qty(void* arg, DFITCSOPRspCalcAbleEntrustQtyField* pData, DFITCSECRspInfoField* pRspInfo)
{
}

void dfitc_sop_on_rsp_sop_qry_able_lock_stock(void* arg, DFITCSOPRspQryAbleLockStockField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast)
{
}

void dfitc_sop_on_rsp_sop_qry_contact_info(void* arg, DFITCSOPRspQryContactField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  trader_instrument traderInstrument;
  memset(&traderInstrument, 0, sizeof(traderInstrument));
  
  int errNo = 0;
  char* errMsg = NULL;
  if(pRspInfo) {
    errNo = pRspInfo->errorID;
    errMsg = pRspInfo->errorMsg;
  }

  if(pData) {
    /*
    DFITCSECRequestIDType               requestID;                 //请求ID
    DFITCSECAccountIDType               accountID;                 //客户号
    DFITCSECExchangeIDType              exchangeID;                //交易所
    DFITCSECSecurityIDType              securityOptionID;          //期权交易代码(10000031)
    DFITCSECContractIDType              contractID;                //期权标识代码(600104C1406M01200)
    DFITCSECContractNameType            contractName;              //期权名称(上汽集团购6月1200)
    DFITCSECSecurityIDType              securityID;                //标的代码(600104)
    DFITCSECContractObjectTypeType      contractObjectType;        //标的类型
    DFITCSECExecuteTypeType             execType;                  //行权方式
    DFITCSECDeliveryTypeType            deliType;                  //交割方式(预留字段)
    DFITCSECOptionTypeType              optType;                   //期权类型
    DFITCSECContractUnitType            contactUnit;               //合约单位
    DFITCSECFundsType                   latestPrice;               //最新价
    DFITCSECDateType                    beginTradingDay;           //开始交易日
    DFITCSECDateType                    endTradingDay;             //最后交易日
    DFITCSECDateType                    execDate;                  //行权日期
    DFITCSECDateType                    endDate;                   //到期日
    DFITCSECQuantityType                positionUpLimit;           //持仓上限
    DFITCSECFundsType                   priceUpLimit;              //涨停价
    DFITCSECFundsType                   priceDownLimit;            //跌停价
    DFITCSECFundsType                   priceHigher;               //最高价
    DFITCSECFundsType                   priceLower;                //最低价
    DFITCSECQuantityType                entrustUpLimit;            //委托上限
    DFITCSECQuantityType                entrustDownLimit;          //委托下限
    DFITCSECQuantityType                entrustUpLimitMarketPri;   //市价委托上限(MP=Market Price)
    DFITCSECQuantityType                entrustDownLimitMarketPri; //市价委托下限
    DFITCSECOpenLimitsType              openLimit;                 //开仓限制
    DFITCSECStockTradeFlagType          stockTradeFlag;            //停牌标志
    DFITCSECFundsType                   depositUnit;               //单位保证金
    DFITCSECFundsType                   depositRatioC;             //保证金比例1
    DFITCSECFundsType                   depositRatioE;             //保证金比例2
    DFITCSECPriceType                   preClosePrice;             //昨收盘价
    DFITCSECPriceType                   closePrice;                //今收盘价
    DFITCSECPriceType                   preSettlePrice;            //昨结算价
    DFITCSECPriceType                   openPrice;                 //开盘价
    DFITCSECLargeQuantityType           tradeQty;                  //成交数量
    DFITCSECFundsType                   turnover;                  //成交金额
    DFITCSECPriceType                   settlePrice;               //结算价(预留字段)
    DFITCSECPriceType                   endCashSettlePrice;        //到期现金结算价
    DFITCSECQuantityType                handQty;                   //整手数
    DFITCSECQuantityType                unClosePositionQty;        //未平仓合约
    DFITCSECApproachExpireFlagType      approachExpireFlag;        //临近到期标志
    DFITCSECTempAdjuestFlagType         tempAdjustFlag;            //临时调整标志
    DFITCSECStockListFlagType           stockListFlag;             //股票挂牌标志
    DFITCSECFundsType                   execPrice;                 //行权价格
    DFITCSECFundsType                   bidPrice1;                 //申买价1
    DFITCSECQuantityType                bidQty1;                   //申买量1
    DFITCSECFundsType                   bidPrice2;                 //申买价2
    DFITCSECQuantityType                bidQty2;                   //申买量2
    DFITCSECFundsType                   bidPrice3;                 //申买价3
    DFITCSECQuantityType                bidQty3;                   //申买量3
    DFITCSECFundsType                   bidPrice4;                 //申买价4
    DFITCSECQuantityType                bidQty4;                   //申买量4
    DFITCSECFundsType                   bidPrice5;                 //申买价5
    DFITCSECQuantityType                bidQty5;                   //申买量5
    DFITCSECFundsType                   askPrice1;                 //申卖价1
    DFITCSECQuantityType                askQty1;                   //申卖量1
    DFITCSECFundsType                   askPrice2;                 //申卖价2
    DFITCSECQuantityType                askQty2;                   //申卖量2
    DFITCSECFundsType                   askPrice3;                 //申卖价3
    DFITCSECQuantityType                askQty3;                   //申卖量3
    DFITCSECFundsType                   askPrice4;                 //申卖价4
    DFITCSECQuantityType                askQty4;                   //申卖量4
    DFITCSECFundsType                   askPrice5;                 //申卖价5
    DFITCSECQuantityType                askQty5;                   //申卖量5
    DFITCSESecurityOptionIndexType      optionIndex;               //股票期权索引值
    DFITCSECMiniPriceChangeType         miniPriceChange;           //最小变动价位
    */
    strcpy(traderInstrument.InstrumentID, pData->securityOptionID);
    strcpy(traderInstrument.ExchangeID, pData->exchangeID);
    traderInstrument.VolumeMultiple = pData->handQty;
    traderInstrument.PriceTick = pData->miniPriceChange;
    traderInstrument.UnitMargin = pData->depositUnit;
  }

  trader_trader_api_on_rsp_qry_instrument(self, &traderInstrument, errNo, errMsg, bIsLast);

}

void dfitc_sop_on_rsp_sop_exectue_order(void* arg, DFITCSOPRspExectueOrderField* pData, DFITCSECRspInfoField* pRspInfo)
{
}

void dfitc_sop_on_rsp_sop_qry_exec_assi_info(void* arg, DFITCSOPRspQryExecAssiInfoField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast)
{
}

void dfitc_sop_on_rsp_sop_qry_trade_time(void* arg, DFITCSOPRspQryTradeTimeField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast)
{
}

void dfitc_sop_on_rsp_sop_qry_exchange_info(void* arg, DFITCSOPRspQryExchangeInfoField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast)
{
}

void dfitc_sop_on_rsp_sop_qry_commission(void* arg, DFITCSOPRspQryCommissionField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast)
{
}

void dfitc_sop_on_rsp_sop_qry_deposit(void* arg, DFITCSOPRspQryDepositField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast)
{
}

void dfitc_sop_on_rsp_sop_qry_contract_object_info(void* arg, DFITCSOPRspQryContractObjectField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast)
{
}

void dfitc_sop_on_sop_entrust_order_rtn(void* arg, DFITCSOPEntrustOrderRtnField* pData)
{
  trader_trader_api* self = (trader_trader_api*)arg;

  trader_order traderOrder;
  memset(&traderOrder, 0, sizeof(traderOrder));

  /*
  DFITCSECLocalOrderIDType             localOrderID;             //本地委托号
  DFITCSECAccountIDType                accountID;                //客户号
  DFITCSECSessionIDType                sessionID;                //会话编号
  DFITCSECBranchIDType                 branchID;                 //机构代码
  DFITCSECShareholderIDType            shareholderID;            //股东号
  DFITCSECExchangeIDType               exchangeID;               //交易所
  DFITCSECSecurityIDType               securityID;               //证劵代码
  DFITCSECWithdrawFlagType             withdrawFlag;             //撤销标志
  DFITCSECCurrencyType                 currency;                 //币种
  DFITCSECSpdOrderIDType               spdOrderID;               //柜台委托号
  DFITCSECEntrustDirectionType         entrustDirection;         //委托类别
  DFITCSECOpenCloseFlagType            openCloseFlag;            //开平标志
  DFITCSECPriceType                    entrustPrice;             //委托价格
  DFITCSECQuantityType                 entrustQty;               //委托数量
  DFITCSECTimeType                     entrustTime;              //委托时间
  DFITCSECCoveredFlagType              coveredFlag;              //备兑类型
  DFITCSECOrderTypeType                orderType;                //订单类型
  DFITCSECOrderExpiryDateType          orderExpiryDate;          //订单时效限制
  DFITCSECOrderCategoryType            orderCategory;            //委托单类别
  DFITCSECDeclareResultType            declareResult;            //申报结果
  DFITCSECMessageType                  noteMsg;                  //结果说明
  DFITCSECTDevIDType                   devID;                    //厂商ID(N),下单时填入该字段，才会返回
  DFITCSECTDevDecInfoType              devDecInfo;               //用户自定义字段(N)，下单时填入该字段，才会返回
  */
  DFITCSOPEntrustOrderRtnField* pOrder = pData;
  // 合约代码
  char InstrumentID [31];
  strcpy(traderOrder.InstrumentID, pData->securityID);
  // 本地报单编号
  snprintf(traderOrder.UserOrderLocalID, sizeof(traderOrder.UserOrderLocalID), "%012ld", pOrder->localOrderID);
  // 买卖
  traderOrder.Direction = pOrder->entrustDirection;
  // 开平
  traderOrder.OffsetFlag = pOrder->openCloseFlag;
  ///投机套保标志
  //traderOrder.HedgeFlag = pOrder->CombHedgeFlag[0];
  // 报单价格
  traderOrder.LimitPrice = pOrder->entrustPrice;
  // 报单手数
  traderOrder.VolumeOriginal = pOrder->entrustQty;
  // 成交手数
  traderOrder.VolumeTraded = 0;
  // 订单状态
  traderOrder.OrderStatus = pOrder->withdrawFlag[0];
  ///插入时间
  strcpy(traderOrder.InsertTime, pOrder->entrustTime);

  trader_trader_api_on_rtn_order(self, &traderOrder);

}

void dfitc_sop_on_sop_trade_rtn(void* arg, DFITCSOPTradeRtnField* pData)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  trader_trade traderTrade;
  memset(&traderTrade, 0, sizeof(traderTrade));
  /*
  DFITCSECLocalOrderIDType             localOrderID;             //本地委托号
  DFITCSECAccountIDType                accountID;                //客户号
  DFITCSECSessionIDType                sessionID;                //会话编号
  DFITCSECShareholderIDType            shareholderID;            //股东号
  DFITCSECExchangeIDType               exchangeID;               //交易所
  DFITCSECSecurityIDType               securityID;               //证劵代码
  DFITCSECWithdrawFlagType             withdrawFlag;             //撤销标志
  DFITCSECCurrencyType                 currency;                 //币种
  DFITCSECSpdOrderIDType               spdOrderID;               //柜台委托号
  DFITCSECEntrustDirectionType         entrustDirection;         //委托类别
  DFITCSECOpenCloseFlagType            openCloseFlag;            //开平标志
  DFITCSECCoveredFlagType              coveredFlag;              //备兑标志
  DFITCSECOrderCategoryType            orderCategory;            //委托单类别
  DFITCSECFundsType                    tradePrice;               //成交价格
  DFITCSECQuantityType                 tradeQty;                 //成交数量
  DFITCSECTradeIDType                  tradeID;                  //成交编号
  DFITCSECSerialIDType                 rtnSerialID;              //回报序号
  DFITCSECDeclareOrderIDType           declareOrderID;           //申报委托号
  DFITCSECDeclareResultType            declareResult;            //申报结果
  DFITCSECMessageType                  noteMsg;                  //结果说明
  DFITCSECTDevIDType                   devID;                    //厂商ID(N),下单时填入该字段，才会返回
  DFITCSECTDevDecInfoType              devDecInfo;               //用户自定义字段(N),下单时填入该字段，才会返回
  DFITCSECTimeType                     tradeTime;                //成交时间
  */

  DFITCSOPTradeRtnField* pTrade = pData;
  ///合约代码
  strcpy(traderTrade.InstrumentID, pTrade->securityID);
  ///本地报单编号
  snprintf(traderTrade.UserOrderLocalID, sizeof(traderTrade.UserOrderLocalID), "%012ld", pTrade->localOrderID);
  ///交易日
  //TODO
  //strcpy(traderTrade.TradingDay, pTrade->TradingDay);
  ///成交时间
  strcpy(traderTrade.TradeTime, pTrade->tradeTime);
  ///成交价格
  traderTrade.TradePrice = pTrade->tradePrice;
  ///成交数量
  traderTrade.TradeVolume = pTrade->tradeQty;
  //成交编号
  strcpy(traderTrade.TradeID, pTrade->tradeID);

  trader_trader_api_on_rtn_trade(self, &traderTrade);

}

void dfitc_sop_on_sop_withdraw_order_rtn(void* arg, DFITCSOPWithdrawOrderRtnField* pData)
{
  trader_trader_api* self = (trader_trader_api*)arg;

  trader_order traderOrder;
  memset(&traderOrder, 0, sizeof(traderOrder));

  /*
  DFITCSECLocalOrderIDType             localOrderID;             //本地委托号
  DFITCSECAccountIDType                accountID;                //客户号
  DFITCSECSessionIDType                sessionID;                //会话编号
  DFITCSECShareholderIDType            shareholderID;            //股东号
  DFITCSECExchangeIDType               exchangeID;               //交易所代码
  DFITCSECSecurityIDType               securityID;               //证劵代码
  DFITCSECWithdrawFlagType             withdrawFlag;             //撤销标志
  DFITCSECCurrencyType                 currency;                 //币种
  DFITCSECSpdOrderIDType               spdOrderID;               //柜台委托号
  DFITCSECEntrustDirectionType         entrustDirection;         //委托类别
  DFITCSECOpenCloseFlagType            openCloseFlag;            //开平标志
  DFITCSECQuantityType                 withdrawQty;              //撤单数量
  DFITCSECQuantityType                 tradeQty;                 //成交数量
  DFITCSECDeclareResultType            declareResult;            //申报结果
  DFITCSECMessageType                  noteMsg;                  //结果说明
  DFITCSECFundsType                    wdUnFreezeFunds;          //撤单解冻资金
  DFITCSECTDevIDType                   devID;                    //厂商ID(N),下单时填入该字段，才会返回
  DFITCSECTDevDecInfoType              devDecInfo;               //用户自定义字段(N),下单时填入该字段，才会返回
  */
  DFITCSOPWithdrawOrderRtnField* pOrder = pData;
  // 合约代码
  char InstrumentID [31];
  strcpy(traderOrder.InstrumentID, pOrder->securityID);
  // 本地报单编号
  snprintf(traderOrder.UserOrderLocalID, sizeof(traderOrder.UserOrderLocalID), "%012ld", pOrder->localOrderID);
  // 买卖
  traderOrder.Direction = pOrder->entrustDirection;
  // 开平
  traderOrder.OffsetFlag = pOrder->openCloseFlag;
  ///投机套保标志
  traderOrder.HedgeFlag = '0';
  // 报单价格
  traderOrder.LimitPrice = 0;
  // 报单手数
  traderOrder.VolumeOriginal = pOrder->withdrawQty + pOrder->tradeQty;
  // 成交手数
  traderOrder.VolumeTraded = pOrder->tradeQty;
  // 订单状态
  traderOrder.OrderStatus = pOrder->withdrawFlag[0];
  ///插入时间
  strcpy(traderOrder.InsertTime, "");

  trader_trader_api_on_rtn_order(self, &traderOrder);

}

void dfitc_sop_on_rsp_fasl_user_login(void* arg, DFITCSECRspUserLoginField* pData, DFITCSECRspInfoField* pRspInfo)
{
}

void dfitc_sop_on_rsp_fasl_user_logout(void* arg, DFITCSECRspUserLogoutField* pData, DFITCSECRspInfoField* pRspInfo)
{
}

void dfitc_sop_on_rsp_fasl_qry_able_fin_info(void* arg, DFITCFASLRspAbleFinInfoField* pData, DFITCSECRspInfoField* pRspInfo)
{
}

void dfitc_sop_on_rsp_fasl_qry_able_slo_info(void* arg, DFITCFASLRspAbleSloInfoField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast)
{
}

void dfitc_sop_on_rsp_fasl_transfer_collateral(void* arg, DFITCFASLRspTransferCollateralField* pData, DFITCSECRspInfoField* pRspInfo)
{
}

void dfitc_sop_on_rsp_fasl_direct_repayment(void* arg, DFITCFASLRspDirectRepaymentField* pData, DFITCSECRspInfoField* pRspInfo)
{
}

void dfitc_sop_on_rsp_fasl_repay_stock_transfer(void* arg, DFITCFASLRspRepayStockTransferField* pData, DFITCSECRspInfoField* pRspInfo)
{
}

void dfitc_sop_on_rsp_fasl_entrust_crdt_order(void* arg, DFITCFASLRspEntrustCrdtOrderField* pData, DFITCSECRspInfoField* pRspInfo)
{
}

void dfitc_sop_on_rsp_fasl_entrust_order(void* arg, DFITCFASLRspEntrustOrderField* pData, DFITCSECRspInfoField* pRspInfo)
{
}

void dfitc_sop_on_rsp_fasl_calc_able_entrust_crdt_qty(void* arg, DFITCFASLRspCalcAbleEntrustCrdtQtyField* pData, DFITCSECRspInfoField* pRspInfo)
{
}

void dfitc_sop_on_rsp_fasl_qrycrdt_funds(void* arg, DFITCFASLRspQryCrdtFundsField* pData, DFITCSECRspInfoField* pRspInfo)
{
}

void dfitc_sop_on_rsp_fasl_qrycrdt_contract(void* arg, DFITCFASLRspQryCrdtContractField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast)
{
}

void dfitc_sop_on_rsp_fasl_qrycrdt_con_change_info(void* arg, DFITCFASLRspQryCrdtConChangeInfoField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast)
{
}

void dfitc_sop_on_rsp_fasl_transfer_funds(void* arg, DFITCStockRspTransferFundsField* pData, DFITCSECRspInfoField* pRspInfo)
{
}

void dfitc_sop_on_rsp_fasl_qry_account_info(void* arg, DFITCStockRspQryAccountField* pData, DFITCSECRspInfoField* pRspInfo)
{
}

void dfitc_sop_on_rsp_fasl_qry_capital_account_info(void* arg, DFITCStockRspQryCapitalAccountField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast)
{
}

void dfitc_sop_on_rsp_fasl_qry_shareholder_info(void* arg, DFITCStockRspQryShareholderField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast)
{
}

void dfitc_sop_on_rsp_fasl_qry_position(void* arg, DFITCStockRspQryPositionField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast)
{
}

void dfitc_sop_on_rsp_fasl_qry_entrust_order(void* arg, DFITCStockRspQryEntrustOrderField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast)
{
}

void dfitc_sop_on_rsp_fasl_qry_serial_trade(void* arg, DFITCStockRspQrySerialTradeField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast)
{
}

void dfitc_sop_on_rsp_fasl_qry_real_time_trade(void* arg, DFITCStockRspQryRealTimeTradeField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast)
{
}

void dfitc_sop_on_rsp_fasl_qry_freeze_funds_detail(void* arg, DFITCStockRspQryFreezeFundsDetailField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast)
{
}

void dfitc_sop_on_rsp_fasl_qry_freeze_stock_detail(void* arg, DFITCStockRspQryFreezeStockDetailField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast)
{
}

void dfitc_sop_on_rsp_fasl_qry_transfer_funds_detail(void* arg, DFITCStockRspQryTransferFundsDetailField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast)
{
}

void dfitc_sop_on_rsp_fasl_withdraw_order(void* arg, DFITCFASLRspWithdrawOrderField* pData, DFITCSECRspInfoField* pRspInfo)
{
}

void dfitc_sop_on_rsp_fasl_qry_system_time(void* arg, DFITCFASLRspQryTradeTimeField* pData, DFITCSECRspInfoField* pRspInfo)
{
}

void dfitc_sop_on_rsp_fasl_qry_transferred_contract(void* arg, DFITCFASLRspQryTransferredContractField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast)
{
}

void dfitc_sop_on_rsp_fasl_desirable_funds_out(void* arg, DFITCFASLRspDesirableFundsOutField* pData, DFITCSECRspInfoField* pRspInfo)
{
}

void dfitc_sop_on_rsp_fasl_qry_guaranteed_contract(void* arg, DFITCFASLRspQryGuaranteedContractField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast)
{
}

void dfitc_sop_on_rsp_fasl_qry_underlying_contract(void* arg, DFITCFASLRspQryUnderlyingContractField* pData, DFITCSECRspInfoField* pRspInfo, bool bIsLast)
{
}

void dfitc_sop_on_fasl_entrust_order_rtn(void* arg, DFITCStockEntrustOrderRtnField* pData)
{
}

void dfitc_sop_on_fasl_trade_rtn(void* arg, DFITCStockTradeRtnField* pData)
{
}

void dfitc_sop_on_fasl_withdraw_order_rtn(void* arg, DFITCStockWithdrawOrderRtnField* pData)
{
}

