
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
  
  // ���ӽ��׷�����
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
  
  //����ID(Y)
  inputOrderField.requestID = pImp->nTraderRequestID++;
  //����ί�к�(Y)
	inputOrderField.localOrderID = atol(local_id);
  //�ͻ���(Y)
	strcpy(inputOrderField.accountID, self->pUser);
  //����������(Y)
  //TODO
	strcpy(inputOrderField.exchangeID, "SH");
  //֤������(Y)
	strcpy(inputOrderField.securityID, inst);
  //���˻�����(N)(Ԥ���ֶ�)
	strcpy(inputOrderField.subAccountID, "");
  //ί������(Y)
	inputOrderField.entrustQty = vol;
  //ί�м۸�(N)
	inputOrderField.entrustPrice = price;
  //ί�����(Y)
  if('1' == buy_sell){
	  inputOrderField.entrustDirection = DFITCSEC_ED_Buy;
  }else{
	  inputOrderField.entrustDirection = DFITCSEC_ED_Sell;
  }
  //��ƽ��־(Y)
  if('1' == open_close){
    inputOrderField.openCloseFlag = DFITCSEC_OCF_Open;
  }else{
    inputOrderField.openCloseFlag = DFITCSEC_OCF_Close;
  }
  //��������(Y)
	inputOrderField.coveredFlag = DFITCSEC_CF_UnCovered;
  //��������(Y)
	inputOrderField.orderType = DFITCSEC_OT_LimitPrice;
  //����ʱЧ����(N)
  inputOrderField.orderExpiryDate = DFITCSEC_OE_NONE;
  //ί�е����(Y)
  inputOrderField.orderCategory = DFITCSEC_OC_GeneralOrder;
  //��չ��ˮ��(N)(Ԥ���ֶ�)
 // inputOrderField.serialID = 0;
  //����ID(N)
	//strcpy(inputOrderField.devID, "");
  //�û��Զ����ֶ�(N)
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

  //����ID(Y)
  inputOrderActionField.requestID = pImp->nTraderRequestID++;
  //�ͻ���(Y)
	strcpy(inputOrderActionField.accountID, self->pUser);
  //��̨ί�к�(N)
	inputOrderActionField.spdOrderID = 0;
  //����ί�к�(N)(��̨ί�кźͱ���ί�кŶ�ѡһ)
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

  //����ID(Y)
  qryInstrumentField.requestID = pImp->nTraderRequestID++;
  //�ͻ���(Y)
	strcpy(qryInstrumentField.accountID, self->pUser);
  /*
  DFITCSECExchangeIDType               exchangeID;               //����������(N)
  DFITCSECSecurityIDType               securityOptionID;         //��Ȩ����(N)
  DFITCSECSecurityIDType               securityID;               //��Ĵ���(N)
  DFITCSECContractObjectTypeType       contractObjectType;       //�������(N)
  DFITCSECOptionTypeType               optType;                  //��Ȩ����(N)
  DFITCSECSecurityIDType               rowIndex;                 //��ҳ����ֵ(Ԥ���ֶ�)(N)
  DFITCSECQuantityType                 rowCount;                 //ÿҳ��ѯ����(Ԥ���ֶ�)(N)
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
  
  //����ID(Y)
  qryInvestorField.requestID = pImp->nTraderRequestID++;
  //�ͻ���(Y)
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

  //����ID(Y)
  qryInvestorPositionField.requestID = pImp->nTraderRequestID++;
  //�ͻ���(Y)
	strcpy(qryInvestorPositionField.accountID, self->pUser);
  /*
  DFITCSECExchangeIDType               exchangeID;               //����������(N)
  DFITCSECSecurityIDType               securityOptionID;         //��Ȩ����(N)
  DFITCSECQuantityType                 rowIndex;                 //��ҳ����ֵ(N)
  DFITCSECQuantityType                 rowCount;                 //ÿҳ��ѯ����(N)
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

  //����ID(Y)
  qryTradingAccountField.requestID = pImp->nTraderRequestID++;
  //�ͻ���(Y)
	strcpy(qryTradingAccountField.accountID, self->pUser);
  /*
  DFITCSECCurrencyType                 currency;                 //����(N)(��ǰֻ֧�������)
  DFITCSECFundsQryFlagType             FundsQryFlag;             //��ѯ��־(Ԥ���ֶ�)
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
    DFITCSECRequestIDType                requestID;                //����ID
    DFITCSECAccountIDType                accountID;                //�ͻ���
    DFITCSECExchangeIDType               exchangeID;               //����������
    DFITCSECSubAccountIDType             subAccountID;             //���˻�����
    DFITCSECSecurityIDType               securityOptionID;         //��Ȩ����
    DFITCSECContractIDType               contractID;               //��Ȩ����
    DFITCSECContractNameType             contractName;             //��Ȩ����
    DFITCSECEntrustDirectionType         entrustDirection;         //ί�����
    DFITCSECCoveredFlagType              coveredFlag;              //���ұ�־
    DFITCSECDateType                     executeDate;              //��Ȩ����
    DFITCSECQuantityType                 totalQty;                 //��ǰ����
    DFITCSECQuantityType                 availableQty;             //��������
    DFITCSECFundsType                    latestPrice;              //���¼�
    DFITCSECFundsType                    optionMarket;             //��Ȩ��ֵ
    DFITCSECQuantityType                 freezeQty;                //��������
    DFITCSECQuantityType                 executeQty;               //ִ������
    DFITCSECQuantityType                 openEntrustQty;           //����ί������
    DFITCSECQuantityType                 openTradeQty;             //���ֳɽ�����
    DFITCSECQuantityType                 prePosition;              //��ֲ�
    DFITCSECQuantityType                 closeEntrustQty;          //ƽ��ί������
    DFITCSECQuantityType                 closeTradeQty;            //ƽ�ֳɽ�����
    DFITCSECFundsType                    deposit;                  //��֤��
    DFITCSECFundsType                    openDeposit;              //���տ��ֱ�֤��
    DFITCSECFundsType                    closeDeposit;             //����ƽ�ֱ�֤��
    DFITCSECFundsType                    exchangeDeposit;          //��������֤��
    DFITCSECFundsType                    exchangeOpenDeposit;      //���������ֱ�֤��
    DFITCSECFundsType                    exchangeCloseDeposit;     //������ƽ�ֱ�֤��
    DFITCSECFundsType                    openAvgPrice;             //���־���
    DFITCSECOptionTypeType               optType;                  //��Ȩ����
    DFITCSECContractObjectTypeType       contractObjectType;       //�������
    DFITCSECContractUnitType             contractUnit;             //��Լ��λ
    DFITCSECCurrencyType                 currency;                 //����
    DFITCSECFundsType                    totalTradeCost;           //�ۼƽ��׳ɱ�(������λ���ۼ�)
    DFITCSECFundsType                    tradeCost;                //���ս��׳ɱ�(ʵʱ����)
    DFITCSECFundsType                    realizeProfitLoss;        //ʵ��ӯ��(Ԥ���ֶ�)
    DFITCSECFundsType                    profitLoss;               //ӯ�����(Ԥ���ֶ�)
    DFITCSECContractAdjustRemindType     adjustRemind;             //��Լ�������ѱ�־
    DFITCSECContraceExpireRemindType     expireRemind;             //��Լ�����������ѱ�־
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
    DFITCSECRequestIDType                requestID;                //����ID
    DFITCSECAccountIDType                accountID;                //�ͻ���
    DFITCSECBranchIDType                 branchID;                 //��������
    DFITCSECCurrencyType                 currency;                 //����
    DFITCSECFundsType                    accountBanlance;          //�˻����
    DFITCSECFundsType                    availableFunds;           //�����ʽ�
    DFITCSECFundsType                    freezeFunds;              //�����ʽ�
    DFITCSECFundsType                    anticipatedInterest;      //Ԥ����Ϣ
    DFITCSECFundsType                    usedDeposit;              //ռ�ñ�֤��
    DFITCSECAccountStatusType            accountStatus;            //�ͻ�״̬
    DFITCSECFundsType                    totalFunds;               //���ʽ�
    DFITCSECFundsType                    totalMarket;              //����ֵ
    DFITCSECFundsType                    cashAssets;               //�ֽ��ʲ�
    DFITCSECFundsType                    execGuaranteeRatio;       //��Լ��������
    DFITCSECFundsType                    buyLimits;                //������

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
    DFITCSECRequestIDType                requestID;                //����
    DFITCSECAccountIDType                accountID;                //�ͻ���
    DFITCSECAccountNameType              accountName;              //�ͻ�����
    DFITCSECAccountNameType              accountFullName;          //�ͻ�ȫ��
    DFITCSECBranchIDType                 branchID;                 //��������
    DFITCSECAccountIdentityTypeType      identityType;             //֤������
    DFITCSECAccountIdentityIDType        accountIdentityID;        //֤�����
    DFITCSECPhoneIDType                  tel;                      //�ͻ��绰
    DFITCSECPhoneIDType                  mobile;                   //�ƶ��绰
    DFITCSECAccountTypeType              accountType;              //Ͷ���߷���
    DFITCSECAccountPropType              accountProp;              //�ͻ�����
    DFITCSECTradePermissionsType         tradePermissions;         //����Ȩ��(Ԥ���ֶ�)
    DFITCSECEntrustTypeType              entrustType;              //ί�з�ʽ
    DFITCSECAccountStatusType            accountStatus;            //�ͻ�״̬
    DFITCSECPasswdSyncFlagType           pwdSynFlag;               //����ͬ����־
    DFITCSECAccountNodeIDType            accountNodeID;            //�ͻ������ڵ���
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
    DFITCSECRequestIDType               requestID;                 //����ID
    DFITCSECAccountIDType               accountID;                 //�ͻ���
    DFITCSECExchangeIDType              exchangeID;                //������
    DFITCSECSecurityIDType              securityOptionID;          //��Ȩ���״���(10000031)
    DFITCSECContractIDType              contractID;                //��Ȩ��ʶ����(600104C1406M01200)
    DFITCSECContractNameType            contractName;              //��Ȩ����(�������Ź�6��1200)
    DFITCSECSecurityIDType              securityID;                //��Ĵ���(600104)
    DFITCSECContractObjectTypeType      contractObjectType;        //�������
    DFITCSECExecuteTypeType             execType;                  //��Ȩ��ʽ
    DFITCSECDeliveryTypeType            deliType;                  //���ʽ(Ԥ���ֶ�)
    DFITCSECOptionTypeType              optType;                   //��Ȩ����
    DFITCSECContractUnitType            contactUnit;               //��Լ��λ
    DFITCSECFundsType                   latestPrice;               //���¼�
    DFITCSECDateType                    beginTradingDay;           //��ʼ������
    DFITCSECDateType                    endTradingDay;             //�������
    DFITCSECDateType                    execDate;                  //��Ȩ����
    DFITCSECDateType                    endDate;                   //������
    DFITCSECQuantityType                positionUpLimit;           //�ֲ�����
    DFITCSECFundsType                   priceUpLimit;              //��ͣ��
    DFITCSECFundsType                   priceDownLimit;            //��ͣ��
    DFITCSECFundsType                   priceHigher;               //��߼�
    DFITCSECFundsType                   priceLower;                //��ͼ�
    DFITCSECQuantityType                entrustUpLimit;            //ί������
    DFITCSECQuantityType                entrustDownLimit;          //ί������
    DFITCSECQuantityType                entrustUpLimitMarketPri;   //�м�ί������(MP=Market Price)
    DFITCSECQuantityType                entrustDownLimitMarketPri; //�м�ί������
    DFITCSECOpenLimitsType              openLimit;                 //��������
    DFITCSECStockTradeFlagType          stockTradeFlag;            //ͣ�Ʊ�־
    DFITCSECFundsType                   depositUnit;               //��λ��֤��
    DFITCSECFundsType                   depositRatioC;             //��֤�����1
    DFITCSECFundsType                   depositRatioE;             //��֤�����2
    DFITCSECPriceType                   preClosePrice;             //�����̼�
    DFITCSECPriceType                   closePrice;                //�����̼�
    DFITCSECPriceType                   preSettlePrice;            //������
    DFITCSECPriceType                   openPrice;                 //���̼�
    DFITCSECLargeQuantityType           tradeQty;                  //�ɽ�����
    DFITCSECFundsType                   turnover;                  //�ɽ����
    DFITCSECPriceType                   settlePrice;               //�����(Ԥ���ֶ�)
    DFITCSECPriceType                   endCashSettlePrice;        //�����ֽ�����
    DFITCSECQuantityType                handQty;                   //������
    DFITCSECQuantityType                unClosePositionQty;        //δƽ�ֺ�Լ
    DFITCSECApproachExpireFlagType      approachExpireFlag;        //�ٽ����ڱ�־
    DFITCSECTempAdjuestFlagType         tempAdjustFlag;            //��ʱ������־
    DFITCSECStockListFlagType           stockListFlag;             //��Ʊ���Ʊ�־
    DFITCSECFundsType                   execPrice;                 //��Ȩ�۸�
    DFITCSECFundsType                   bidPrice1;                 //�����1
    DFITCSECQuantityType                bidQty1;                   //������1
    DFITCSECFundsType                   bidPrice2;                 //�����2
    DFITCSECQuantityType                bidQty2;                   //������2
    DFITCSECFundsType                   bidPrice3;                 //�����3
    DFITCSECQuantityType                bidQty3;                   //������3
    DFITCSECFundsType                   bidPrice4;                 //�����4
    DFITCSECQuantityType                bidQty4;                   //������4
    DFITCSECFundsType                   bidPrice5;                 //�����5
    DFITCSECQuantityType                bidQty5;                   //������5
    DFITCSECFundsType                   askPrice1;                 //������1
    DFITCSECQuantityType                askQty1;                   //������1
    DFITCSECFundsType                   askPrice2;                 //������2
    DFITCSECQuantityType                askQty2;                   //������2
    DFITCSECFundsType                   askPrice3;                 //������3
    DFITCSECQuantityType                askQty3;                   //������3
    DFITCSECFundsType                   askPrice4;                 //������4
    DFITCSECQuantityType                askQty4;                   //������4
    DFITCSECFundsType                   askPrice5;                 //������5
    DFITCSECQuantityType                askQty5;                   //������5
    DFITCSESecurityOptionIndexType      optionIndex;               //��Ʊ��Ȩ����ֵ
    DFITCSECMiniPriceChangeType         miniPriceChange;           //��С�䶯��λ
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
  DFITCSECLocalOrderIDType             localOrderID;             //����ί�к�
  DFITCSECAccountIDType                accountID;                //�ͻ���
  DFITCSECSessionIDType                sessionID;                //�Ự���
  DFITCSECBranchIDType                 branchID;                 //��������
  DFITCSECShareholderIDType            shareholderID;            //�ɶ���
  DFITCSECExchangeIDType               exchangeID;               //������
  DFITCSECSecurityIDType               securityID;               //֤������
  DFITCSECWithdrawFlagType             withdrawFlag;             //������־
  DFITCSECCurrencyType                 currency;                 //����
  DFITCSECSpdOrderIDType               spdOrderID;               //��̨ί�к�
  DFITCSECEntrustDirectionType         entrustDirection;         //ί�����
  DFITCSECOpenCloseFlagType            openCloseFlag;            //��ƽ��־
  DFITCSECPriceType                    entrustPrice;             //ί�м۸�
  DFITCSECQuantityType                 entrustQty;               //ί������
  DFITCSECTimeType                     entrustTime;              //ί��ʱ��
  DFITCSECCoveredFlagType              coveredFlag;              //��������
  DFITCSECOrderTypeType                orderType;                //��������
  DFITCSECOrderExpiryDateType          orderExpiryDate;          //����ʱЧ����
  DFITCSECOrderCategoryType            orderCategory;            //ί�е����
  DFITCSECDeclareResultType            declareResult;            //�걨���
  DFITCSECMessageType                  noteMsg;                  //���˵��
  DFITCSECTDevIDType                   devID;                    //����ID(N),�µ�ʱ������ֶΣ��Ż᷵��
  DFITCSECTDevDecInfoType              devDecInfo;               //�û��Զ����ֶ�(N)���µ�ʱ������ֶΣ��Ż᷵��
  */
  DFITCSOPEntrustOrderRtnField* pOrder = pData;
  // ��Լ����
  char InstrumentID [31];
  strcpy(traderOrder.InstrumentID, pData->securityID);
  // ���ر������
  snprintf(traderOrder.UserOrderLocalID, sizeof(traderOrder.UserOrderLocalID), "%012ld", pOrder->localOrderID);
  // ����
  traderOrder.Direction = pOrder->entrustDirection;
  // ��ƽ
  traderOrder.OffsetFlag = pOrder->openCloseFlag;
  ///Ͷ���ױ���־
  //traderOrder.HedgeFlag = pOrder->CombHedgeFlag[0];
  // �����۸�
  traderOrder.LimitPrice = pOrder->entrustPrice;
  // ��������
  traderOrder.VolumeOriginal = pOrder->entrustQty;
  // �ɽ�����
  traderOrder.VolumeTraded = 0;
  // ����״̬
  traderOrder.OrderStatus = pOrder->withdrawFlag[0];
  ///����ʱ��
  strcpy(traderOrder.InsertTime, pOrder->entrustTime);

  trader_trader_api_on_rtn_order(self, &traderOrder);

}

void dfitc_sop_on_sop_trade_rtn(void* arg, DFITCSOPTradeRtnField* pData)
{
  trader_trader_api* self = (trader_trader_api*)arg;
  trader_trade traderTrade;
  memset(&traderTrade, 0, sizeof(traderTrade));
  /*
  DFITCSECLocalOrderIDType             localOrderID;             //����ί�к�
  DFITCSECAccountIDType                accountID;                //�ͻ���
  DFITCSECSessionIDType                sessionID;                //�Ự���
  DFITCSECShareholderIDType            shareholderID;            //�ɶ���
  DFITCSECExchangeIDType               exchangeID;               //������
  DFITCSECSecurityIDType               securityID;               //֤������
  DFITCSECWithdrawFlagType             withdrawFlag;             //������־
  DFITCSECCurrencyType                 currency;                 //����
  DFITCSECSpdOrderIDType               spdOrderID;               //��̨ί�к�
  DFITCSECEntrustDirectionType         entrustDirection;         //ί�����
  DFITCSECOpenCloseFlagType            openCloseFlag;            //��ƽ��־
  DFITCSECCoveredFlagType              coveredFlag;              //���ұ�־
  DFITCSECOrderCategoryType            orderCategory;            //ί�е����
  DFITCSECFundsType                    tradePrice;               //�ɽ��۸�
  DFITCSECQuantityType                 tradeQty;                 //�ɽ�����
  DFITCSECTradeIDType                  tradeID;                  //�ɽ����
  DFITCSECSerialIDType                 rtnSerialID;              //�ر����
  DFITCSECDeclareOrderIDType           declareOrderID;           //�걨ί�к�
  DFITCSECDeclareResultType            declareResult;            //�걨���
  DFITCSECMessageType                  noteMsg;                  //���˵��
  DFITCSECTDevIDType                   devID;                    //����ID(N),�µ�ʱ������ֶΣ��Ż᷵��
  DFITCSECTDevDecInfoType              devDecInfo;               //�û��Զ����ֶ�(N),�µ�ʱ������ֶΣ��Ż᷵��
  DFITCSECTimeType                     tradeTime;                //�ɽ�ʱ��
  */

  DFITCSOPTradeRtnField* pTrade = pData;
  ///��Լ����
  strcpy(traderTrade.InstrumentID, pTrade->securityID);
  ///���ر������
  snprintf(traderTrade.UserOrderLocalID, sizeof(traderTrade.UserOrderLocalID), "%012ld", pTrade->localOrderID);
  ///������
  //TODO
  //strcpy(traderTrade.TradingDay, pTrade->TradingDay);
  ///�ɽ�ʱ��
  strcpy(traderTrade.TradeTime, pTrade->tradeTime);
  ///�ɽ��۸�
  traderTrade.TradePrice = pTrade->tradePrice;
  ///�ɽ�����
  traderTrade.TradeVolume = pTrade->tradeQty;
  //�ɽ����
  strcpy(traderTrade.TradeID, pTrade->tradeID);

  trader_trader_api_on_rtn_trade(self, &traderTrade);

}

void dfitc_sop_on_sop_withdraw_order_rtn(void* arg, DFITCSOPWithdrawOrderRtnField* pData)
{
  trader_trader_api* self = (trader_trader_api*)arg;

  trader_order traderOrder;
  memset(&traderOrder, 0, sizeof(traderOrder));

  /*
  DFITCSECLocalOrderIDType             localOrderID;             //����ί�к�
  DFITCSECAccountIDType                accountID;                //�ͻ���
  DFITCSECSessionIDType                sessionID;                //�Ự���
  DFITCSECShareholderIDType            shareholderID;            //�ɶ���
  DFITCSECExchangeIDType               exchangeID;               //����������
  DFITCSECSecurityIDType               securityID;               //֤������
  DFITCSECWithdrawFlagType             withdrawFlag;             //������־
  DFITCSECCurrencyType                 currency;                 //����
  DFITCSECSpdOrderIDType               spdOrderID;               //��̨ί�к�
  DFITCSECEntrustDirectionType         entrustDirection;         //ί�����
  DFITCSECOpenCloseFlagType            openCloseFlag;            //��ƽ��־
  DFITCSECQuantityType                 withdrawQty;              //��������
  DFITCSECQuantityType                 tradeQty;                 //�ɽ�����
  DFITCSECDeclareResultType            declareResult;            //�걨���
  DFITCSECMessageType                  noteMsg;                  //���˵��
  DFITCSECFundsType                    wdUnFreezeFunds;          //�����ⶳ�ʽ�
  DFITCSECTDevIDType                   devID;                    //����ID(N),�µ�ʱ������ֶΣ��Ż᷵��
  DFITCSECTDevDecInfoType              devDecInfo;               //�û��Զ����ֶ�(N),�µ�ʱ������ֶΣ��Ż᷵��
  */
  DFITCSOPWithdrawOrderRtnField* pOrder = pData;
  // ��Լ����
  char InstrumentID [31];
  strcpy(traderOrder.InstrumentID, pOrder->securityID);
  // ���ر������
  snprintf(traderOrder.UserOrderLocalID, sizeof(traderOrder.UserOrderLocalID), "%012ld", pOrder->localOrderID);
  // ����
  traderOrder.Direction = pOrder->entrustDirection;
  // ��ƽ
  traderOrder.OffsetFlag = pOrder->openCloseFlag;
  ///Ͷ���ױ���־
  traderOrder.HedgeFlag = '0';
  // �����۸�
  traderOrder.LimitPrice = 0;
  // ��������
  traderOrder.VolumeOriginal = pOrder->withdrawQty + pOrder->tradeQty;
  // �ɽ�����
  traderOrder.VolumeTraded = pOrder->tradeQty;
  // ����״̬
  traderOrder.OrderStatus = pOrder->withdrawFlag[0];
  ///����ʱ��
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
