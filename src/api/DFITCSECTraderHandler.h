#ifndef _DFITC_SEC_TRADER_HANDLER_H_
#define _DFITC_SEC_TRADER_HANDLER_H_
#include "DFITCSECApiDataType.h"
#include "DFITCSECApiStruct.h"
#include "DFITCSECTraderApi.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct dfitc_sec_trader_api_cb_def dfitc_sec_trader_api_cb;

struct dfitc_sec_trader_api_cb_def{
  /**
   * SEC-网络连接正常响应
   */
  void (*xOnFrontConnected)(void* arg);
  /**
   * SEC-网络连接不正常响应
   */
  void (*xOnFrontDisconnected)(void* arg, int nReason);
  /**
   * SEC-消息通知
   */
  void (*xOnRtnNotice)(void* arg, DFITCSECRspNoticeField *pNotice);
  /**
  * ERR-错误应答
  * @param pRspInfo:指针若非空，返回错误信息结构地址
  */
  void (*xOnRspError)(void* arg, DFITCSECRspInfoField *pRspInfo);
  /**
  * STOCK-登录响应
  * @param pData:指针若非空,返回用户登录响应信息结构体的地址,表明登录请求成功
  * @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明登录请求失败
  */
  void (*xOnRspStockUserLogin)(void* arg, DFITCSECRspUserLoginField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * STOCK-登出响应
  * @param pData:指针若非空,返回用户登出响应信息结构体的地址,表明登出请求成功
  * @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明登出请求失败
  */
  void (*xOnRspStockUserLogout)(void* arg, DFITCSECRspUserLogoutField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * STOCK-密码更新响应
  * @param pData:指针若非空,返回用户密码更新响应信息结构体的地址,表明密码更新请求成功
  * @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明密码更新请求失败
  */
  void (*xOnRspStockUserPasswordUpdate)(void* arg, DFITCSECRspPasswordUpdateField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * STOCK-委托报单响应
  * @param pData:指针若非空,返回用户委托报单响应信息结构体的地址,表明报单请求成功
  * @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明委托报单请求失败
  */
  void (*xOnRspStockEntrustOrder)(void* arg, DFITCStockRspEntrustOrderField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * STOCK-委托撤单响应
  * @param pData:指针若非空,返回用户委托撤单响应信息结构体的地址,表明撤单请求成功
  * @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明委托撤单请求失败
  */
  void (*xOnRspStockWithdrawOrder)(void* arg, DFITCSECRspWithdrawOrderField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * STOCK-委托查询响应
  * @param pData:指针若非空,返回用户委托查询响应信息结构体的地址,表明查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明委托查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息)(void* arg, 0-否,1-是)
  */
  void (*xOnRspStockQryEntrustOrder)(void* arg, DFITCStockRspQryEntrustOrderField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * STOCK-实时成交查询响应
  * @param pData:指针若非空,返回用户实时成交查询响应信息结构体的地址,表明实时成交查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明实时成交查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息)(void* arg, 0-否,1-是)
  */
  void (*xOnRspStockQryRealTimeTrade)(void* arg, DFITCStockRspQryRealTimeTradeField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * STOCK-分笔成交查询响应
  * @param pData:指针若非空,返回用户分笔成交查询响应信息结构体的地址,表明分笔成交查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明分笔成交查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息)(void* arg, 0-否,1-是)
  */
  void (*xOnRspStockQrySerialTrade)(void* arg, DFITCStockRspQrySerialTradeField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * STOCK-持仓查询响应
  * @param pData:指针若非空,返回用户持仓查询响应信息结构体的地址,表明持仓查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明持仓查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息)(void* arg, 0-否,1-是)
  */
  void (*xOnRspStockQryPosition)(void* arg, DFITCStockRspQryPositionField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * STOCK-资金账号查询响应
  * @param pData:指针若非空,返回用户资金账号查询响应信息结构体的地址,表明资金账号查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明资金账号查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息)(void* arg, 0-否,1-是)
  */
  void (*xOnRspStockQryCapitalAccountInfo)(void* arg, DFITCStockRspQryCapitalAccountField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * STOCK-交易账号查询响应
  * @param pData:指针若非空,返回用户交易账号查询响应信息结构体的地址,表明交易账号查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明交易账号查询请求失败
  */
  void (*xOnRspStockQryAccountInfo)(void* arg, DFITCStockRspQryAccountField *pData, DFITCSECRspInfoField *pRspInfo); 
  /**
  * STOCK-股东号查询响应
  * @param pData:指针若非空,返回用户股东号查询响应信息结构体的地址,表明股东号查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明股东号查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息)(void* arg, 0-否,1-是)
  */
  void (*xOnRspStockQryShareholderInfo)(void* arg, DFITCStockRspQryShareholderField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast); 
  /**
  * STOCK-资金调拨响应
  * @param pData:指针若非空,返回用户资金调拨响应信息结构体的地址,表明股资金调拨请求成功
  * @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明资金调拨请求失败
  */
  void (*xOnRspStockTransferFunds)(void* arg, DFITCStockRspTransferFundsField *pData,DFITCSECRspInfoField *pRspInfo);
  /**
  * STOCK-批量委托响应
  * @param pData:指针若非空,返回用户批量委托响应信息结构体的地址,表明批量委托请求成功
  * @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明批量委托请求失败
  */
  void (*xOnRspStockEntrustBatchOrder)(void* arg, DFITCStockRspEntrustBatchOrderField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * STOCK-批量撤单响应
  * @param pData:指针若非空,返回用户批量撤单响应信息结构体的地址,表明批量撤单请求成功
  * @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明批量撤单请求失败
  */
  void (*xOnRspStockWithdrawBatchOrder)(void* arg, DFITCStockRspWithdrawBatchOrderField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * STOCK-计算可委托数量响应
  * @param pData:指针若非空,返回用户计算可委托数量响应信息结构体的地址,表明计算可委托数量请求成功
  * @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明计算可委托数量请求失败
  */
  void (*xOnRspStockCalcAbleEntrustQty)(void* arg, DFITCStockRspCalcAbleEntrustQtyField *pData, DFITCSECRspInfoField *pRspInfo); 
  /**
  * STOCK-计算申购ETF数量响应
  * @param pData:指针若非空,返回用户计算申购ETF数量响应信息结构体的地址,表明计算申购ETF数量请求成功
  * @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明计算申购ETF数量请求失败
  */
  void (*xOnRspStockCalcAblePurchaseETFQty)(void* arg, DFITCStockRspCalcAblePurchaseETFQtyField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * STOCK-冻结资金明细查询响应
  * @param pData:指针若非空,返回用户冻结资金明细查询响应信息结构体的地址,表明冻结资金明细查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明冻结资金明细查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息)(void* arg, 0-否,1-是)
  */
  void (*xOnRspStockQryFreezeFundsDetail)(void* arg, DFITCStockRspQryFreezeFundsDetailField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast); 
  /**
  * STOCK-冻结证券明细查询响应
  * @param pData:指针若非空,返回用户冻结证券明细查询响应信息结构体的地址,表明冻结证券明细查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明冻结证券明细查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息)(void* arg, 0-否,1-是)
  */
  void (*xOnRspStockQryFreezeStockDetail)(void* arg, DFITCStockRspQryFreezeStockDetailField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * STOCK-调拨证券明细查询响应
  * @param pData:指针若非空,返回用户调拨证券明细查询响应信息结构体的地址,表明调拨证券明细查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明调拨证券明细查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息)(void* arg, 0-否,1-是)
  */
  void (*xOnRspStockQryTransferStockDetail)(void* arg, DFITCStockRspQryTransferStockDetailField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * STOCK-调拨资金明细查询响应
  * @param pData:指针若非空,返回用户调拨资金明细查询响应信息结构体的地址,表明调拨资金明细查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明调拨资金明细查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息)(void* arg, 0-否,1-是)
  */
  void (*xOnRspStockQryTransferFundsDetail)(void* arg, DFITCStockRspQryTransferFundsDetailField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * STOCK-证券信息查询响应
  * @param pData:指针若非空,返回用户证券信息查询响应信息结构体的地址,表明证券信息查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明证券信息查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息)(void* arg, 0-否,1-是)
  */
  void (*xOnRspStockQryStockInfo)(void* arg, DFITCStockRspQryStockField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * STOCK-证券静态信息查询响应
  * @param pData:指针若非空,返回用户证券静态信息查询响应信息结构体的地址,表明证券静态信息查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明证券静态信息查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息)(void* arg, 0-否,1-是)
  */
  void (*xOnRspStockQryStockStaticInfo)(void* arg, DFITCStockRspQryStockStaticField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * STOCK-交易时间查询响应
  * @param pData:指针若非空,返回用户交易时间查询响应信息结构体的地址,表明交易时间查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明交易时间查询请求失败
  */
  void (*xOnRspStockQryTradeTime)(void* arg, DFITCStockRspQryTradeTimeField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * STOCK-委托回报响应
  * @param pData:返回委托回报结构体的地址
  */
  void (*xOnStockEntrustOrderRtn)(void* arg, DFITCStockEntrustOrderRtnField * pData);
  /**
  * STOCK-成交回报响应
  * @param pData:返回成交回报结构体的地址
  */
  void (*xOnStockTradeRtn)(void* arg, DFITCStockTradeRtnField * pData);
  /**
  * STOCK-撤单回报响应
  * @param pData:返回撤单回报结构体的地址
  */
  void (*xOnStockWithdrawOrderRtn)(void* arg, DFITCStockWithdrawOrderRtnField * pData);
  
  /**
  * SOP-登录响应
  * @param pRspUserLogin:指针若非空,返回用户登录响应信息结构地址,表明登录请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明登录请求失败
  */
  void (*xOnRspSOPUserLogin)(void* arg, DFITCSECRspUserLoginField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
   * SOP-登出响应
   * @param pData:指针若非空,返回用户登出响应信息结构地址,表明登出请求成功
   * @param pRspInfo:指针若非空，返回错误信息地址，表明登出请求失败
   */
  void (*xOnRspSOPUserLogout)(void* arg, DFITCSECRspUserLogoutField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * SOP-用户口令更新响应
  * @param pData:指针若非空,返回用户口令更新响应信息结构地址,表明用户口令更新请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明用户口令更新请求失败
  */
  void (*xOnRspSOPUserPasswordUpdate)(void* arg, DFITCSECRspPasswordUpdateField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * SOP-报单响应
  * @param pData:指针若非空,返回用户报单响应信息结构地址,表明报单请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明报单请求失败
  */
  void (*xOnRspSOPEntrustOrder)(void* arg, DFITCSOPRspEntrustOrderField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * SOP-组合拆分委托响应
  * @param pData:指针若非空,返回用户响应信息结构地址,表明组合拆分委托请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明组合拆分委托请求失败
  */
  void (*xOnRspSOPGroupSplit)(void* arg, DFITCSOPRspEntrustOrderField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * SOP-查询客户组合持仓明细响应
  * @param pData:指针若非空,返回用户查询客户组合持仓明细响应结构地址,表明查询客户组合持仓明细请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明查询客户组合持仓明细请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息)(void* arg, 0-否,1-是)
  */
  void (*xOnRspSOPQryGroupPosition)(void* arg, DFITCSOPRspQryGroupPositionField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * SOP-证券锁定解锁响应
  * @param pData:指针若非空,返回用户证券锁定解锁响应信息结构地址,表明证券锁定解锁请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明证券锁定解锁请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息)(void* arg, 0-否,1-是)
  */
  void (*xOnRspSOPLockOUnLockStock)(void* arg, DFITCSOPRspLockOUnLockStockField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast); 
  /**
  * SOP-撤单响应
  * @param pData:指针若非空,返回用户撤单响应信息结构地址,表明撤单请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明撤单请求失败
  */
  void (*xOnRspSOPWithdrawOrder)(void* arg, DFITCSECRspWithdrawOrderField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * SOP-委托查询响应
  * @param pData:指针若非空,返回用户委托查询响应信息结构地址,表明委托查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明委托查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息)(void* arg, 0-否,1-是)
  */
  void (*xOnRspSOPQryEntrustOrder)(void* arg, DFITCSOPRspQryEntrustOrderField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * SOP-分笔成交查询响应
  * @param pData:指针若非空,返回用户分笔成交查询响应信息结构地址,表明分笔成交查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明分笔成交查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息)(void* arg, 0-否,1-是)
  */
  void (*xOnRspSOPQrySerialTrade)(void* arg, DFITCSOPRspQrySerialTradeField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * SOP-持仓查询响应
  * @param pData:指针若非空,返回用户持仓查询响应信息结构地址,表明持仓查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明持仓查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息)(void* arg, 0-否,1-是)
  */
  void (*xOnRspSOPQryPosition)(void* arg, DFITCSOPRspQryPositionField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * SOP-客户担保持仓查询响应
  * @param pData:指针若非空,返回用户客户担保持仓查询响应信息结构地址,表明客户担保持仓查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户担保持仓查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息)(void* arg, 0-否,1-是)
  */
  void (*xOnRspSOPQryCollateralPosition)(void* arg, DFITCSOPRspQryCollateralPositionField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * SOP-客户资金查询响应
  * @param pData:指针若非空,返回用户客户资金查询响应信息结构地址,表明客户客户资金查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户客户资金查询请求失败
  */
  void (*xOnRspSOPQryCapitalAccountInfo)(void* arg, DFITCSOPRspQryCapitalAccountField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * SOP-客户信息查询响应
  * @param pData:指针若非空,返回用户客户信息查询响应信息结构地址,表明客户客户信息查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户客户信息查询请求失败
  */
  void (*xOnRspSOPQryAccountInfo)(void* arg, DFITCSOPRspQryAccountField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * SOP-股东信息查询响应
  * @param pData:指针若非空,返回用户股东信息查询响应信息结构地址,表明客户股东信息查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户股东信息查询请求失败
  */
  void (*xOnRspSOPQryShareholderInfo)(void* arg, DFITCSOPRspQryShareholderField *pData, DFITCSECRspInfoField *pRspInfo); 
  /**
  * SOP-可委托数量查询响应
  * @param pData:指针若非空,返回用户可委托数量查询响应信息结构地址,表明客户可委托数量查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户可委托数量查询请求失败
  */
  void (*xOnRspSOPCalcAbleEntrustQty)(void* arg, DFITCSOPRspCalcAbleEntrustQtyField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * SOP-客户可锁定证券查询响应
  * @param pData:指针若非空,返回用户客户可锁定证券查询响应信息结构地址,表明客户客户可锁定证券查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户客户可锁定证券查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息)(void* arg, 0-否,1-是)
  */
  void (*xOnRspSOPQryAbleLockStock)(void* arg, DFITCSOPRspQryAbleLockStockField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast); 
  /**
  * SOP-期权合约代码查询响应
  * @param pData:指针若非空,返回用户期权合约代码查询响应信息结构地址,表明客户期权合约代码查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户期权合约代码查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息)(void* arg, 0-否,1-是)
  */
  void (*xOnRspSOPQryContactInfo)(void* arg, DFITCSOPRspQryContactField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast); 
  /**
  * SOP-执行委托响应
  * @param pData:指针若非空,返回用户执行委托响应信息结构地址,表明客户执行委托请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户执行委托请求失败
  */
  void (*xOnRspSOPExectueOrder)(void* arg, DFITCSOPRspExectueOrderField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * SOP-客户行权指派信息查询响应
  * @param pData:指针若非空,返回用户客户行权指派信息查询响应信息结构地址,表明客户客户行权指派信息查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户客户行权指派信息查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息)(void* arg, 0-否,1-是)
  */
  void (*xOnRspSOPQryExecAssiInfo)(void* arg, DFITCSOPRspQryExecAssiInfoField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * SOP-查询交易时间响应
  * @param pData:指针若非空,返回用户查询交易时间响应信息结构地址,表明客户查询交易时间请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户查询交易时间请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息)(void* arg, 0-否,1-是)
  */
  void (*xOnRspSOPQryTradeTime)(void* arg, DFITCSOPRspQryTradeTimeField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * SOP-获取所有交易所参数响应
  * @param pData:指针若非空,返回用户获取所有交易所参数响应信息结构地址,表明客户获取所有交易所参数请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户获取所有交易所参数请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息)(void* arg, 0-否,1-是)
  */
  void (*xOnRspSOPQryExchangeInfo)(void* arg, DFITCSOPRspQryExchangeInfoField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * SOP-查询手续费参数响应
  * @param pData:指针若非空,返回用户查询手续费参数响应信息结构地址,表明客户查询手续费参数请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户查询手续费参数请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息)(void* arg, 0-否,1-是)
  */
  void (*xOnRspSOPQryCommission)(void* arg, DFITCSOPRspQryCommissionField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * SOP-查询保证金率参数响应
  * @param pData:指针若非空,返回用户查询保证金率参数响应信息结构地址,表明客户查询保证金率参数请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户查询保证金率参数请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息)(void* arg, 0-否,1-是)
  */
  void (*xOnRspSOPQryDeposit)(void* arg, DFITCSOPRspQryDepositField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * SOP-期权标的信息查询响应
  * @param pData:指针若非空,返回用户期权标的信息查询响应信息结构地址,表明客户期权标的信息查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户期权标的信息查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息)(void* arg, 0-否,1-是)
  */
  void (*xOnRspSOPQryContractObjectInfo)(void* arg, DFITCSOPRspQryContractObjectField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * SOP-委托回报响应
  * @param pData:返回委托回报结构体的地址
  */
  void (*xOnSOPEntrustOrderRtn)(void* arg, DFITCSOPEntrustOrderRtnField * pData);
  /**
  * SOP-成交回报响应
  * @param pData:返回成交回报结构体的地址
  */
  void (*xOnSOPTradeRtn)(void* arg, DFITCSOPTradeRtnField * pData);
  /**
  * SOP-撤单回报响应
  * @param pData:返回撤单回报结构体的地址
  */
  void (*xOnSOPWithdrawOrderRtn)(void* arg, DFITCSOPWithdrawOrderRtnField * pData);

  /**
  * FASL-登录响应
  * @param pData:指针若非空,返回用户融资融券登录响应信息结构地址,表明客户融资融券登录请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户融资融券登录请求失败
  */
  void (*xOnRspFASLUserLogin)(void* arg, DFITCSECRspUserLoginField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-登出响应
  * @param pData:指针若非空,返回用户融资融券登出响应信息结构地址,表明客户融资融券登出请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户融资融券登出请求失败
  */
  void (*xOnRspFASLUserLogout)(void* arg, DFITCSECRspUserLogoutField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-客户可融资信息响应
  * @param pData:指针若非空,返回用户客户可融资信息响应信息结构地址,表明客户客户可融资信息请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户客户可融资信息请求失败
  */
  void (*xOnRspFASLQryAbleFinInfo)(void* arg, DFITCFASLRspAbleFinInfoField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-客户可融券信息响应
  * @param pData:指针若非空,返回用户客户可融券信息响应信息结构地址,表明客户客户可融券信息请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户客户可融券信息请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息)(void* arg, 0-否,1-是)
  */
  void (*xOnRspFASLQryAbleSloInfo)(void* arg, DFITCFASLRspAbleSloInfoField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-担保物划转响应
  * @param pData:指针若非空,返回用户担保物划转响应信息结构地址,表明客户担保物划转请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户担保物划转请求失败
  */
  void (*xOnRspFASLTransferCollateral)(void* arg, DFITCFASLRspTransferCollateralField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-直接还款响应
  * @param pData:指针若非空,返回用户直接还款响应信息结构地址,表明客户直接还款请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户直接还款请求失败
  */
  void (*xOnRspFASLDirectRepayment)(void* arg, DFITCFASLRspDirectRepaymentField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-还券划转响应
  * @param pData:指针若非空,返回用户还券划转响应信息结构地址,表明客户还券划转请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户还券划转请求失败
  */
  void (*xOnRspFASLRepayStockTransfer)(void* arg, DFITCFASLRspRepayStockTransferField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-信用交易响应
  * @param pData:指针若非空,返回用户信用交易响应信息结构地址,表明客户信用交易请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户信用交易请求失败
  */
  void (*xOnRspFASLEntrustCrdtOrder)(void* arg, DFITCFASLRspEntrustCrdtOrderField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-融资融券交易响应
  * @param pData:指针若非空,返回用户融资融券交易响应信息结构地址,表明客户融资融券交易请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户融资融券交易请求失败
  */
  void (*xOnRspFASLEntrustOrder)(void* arg, DFITCFASLRspEntrustOrderField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-信用可委托数量查询响应
  * @param pData:指针若非空,返回用户信用可委托数量查询响应信息结构地址,表明客户信用可委托数量查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户信用可委托数量查询请求失败
  */
  void (*xOnRspFASLCalcAbleEntrustCrdtQty)(void* arg, DFITCFASLRspCalcAbleEntrustCrdtQtyField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-查询信用资金响应
  * @param pData:指针若非空,返回用户查询信用资金响应信息结构地址,表明客户查询信用资金请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户查询信用资金请求失败
  */
  void (*xOnRspFASLQryCrdtFunds)(void* arg, DFITCFASLRspQryCrdtFundsField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-信用合约信息响应
  * @param pData:指针若非空,返回用户信用合约信息响应信息结构地址,表明客户信用合约信息请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户信用合约信息请求失败
  */
  void (*xOnRspFASLQryCrdtContract)(void* arg, DFITCFASLRspQryCrdtContractField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLat);
  /**
  * FASL-信用合约变动信息查询响应
  * @param pData:指针若非空,返回用户信用合约变动信息查询响应信息结构地址,表明客户信用合约变动信息查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户信用合约变动信息查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息)(void* arg, 0-否,1-是)
  */
  void (*xOnRspFASLQryCrdtConChangeInfo)(void* arg, DFITCFASLRspQryCrdtConChangeInfoField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-资金调转响应
  * @param pData:指针若非空,返回用户资金调转响应信息结构地址,表明客户资金调转请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户资金调转请求失败
  */
  void (*xOnRspFASLTransferFunds)(void* arg, DFITCStockRspTransferFundsField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-客户信息查询响应
  * @param pData:指针若非空,返回用户客户信息查询响应信息结构地址,表明客户客户信息查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户客户信息查询请求失败
  */
  void (*xOnRspFASLQryAccountInfo)(void* arg, DFITCStockRspQryAccountField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-客户资金查询响应
  * @param pData:指针若非空,返回用户客户资金查询响应信息结构地址,表明客户客户资金查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户客户资金查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息)(void* arg, 0-否,1-是)
  */
  void (*xOnRspFASLQryCapitalAccountInfo)(void* arg, DFITCStockRspQryCapitalAccountField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-股东信息查询响应
  * @param pData:指针若非空,返回用户股东信息查询响应信息结构地址,表明客户股东信息查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户股东信息查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息)(void* arg, 0-否,1-是)
  */
  void (*xOnRspFASLQryShareholderInfo)(void* arg, DFITCStockRspQryShareholderField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-持仓查询响应
  * @param pData:指针若非空,返回用户持仓查询响应信息结构地址,表明客户持仓查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户持仓查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息)(void* arg, 0-否,1-是)
  */
  void (*xOnRspFASLQryPosition)(void* arg, DFITCStockRspQryPositionField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-委托查询响应
  * @param pData:指针若非空,返回用户委托查询响应信息结构地址,表明客户委托查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户委托查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息)(void* arg, 0-否,1-是)
  */
  void (*xOnRspFASLQryEntrustOrder)(void* arg, DFITCStockRspQryEntrustOrderField * pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-分笔成交查询响应
  * @param pData:指针若非空,返回用户分笔成交查询响应信息结构地址,表明客户分笔成交查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户分笔成交查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息)(void* arg, 0-否,1-是)
  */
  void (*xOnRspFASLQrySerialTrade)(void* arg, DFITCStockRspQrySerialTradeField * pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-实时成交查询响应
  * @param pData:指针若非空,返回用户实时成交查询响应信息结构地址,表明客户实时成交查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户实时成交查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息)(void* arg, 0-否,1-是)
  */
  void (*xOnRspFASLQryRealTimeTrade)(void* arg, DFITCStockRspQryRealTimeTradeField * pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-资金冻结明细查询响应
  * @param pData:指针若非空,返回用户资金冻结明细查询响应信息结构地址,表明客户资金冻结明细查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户资金冻结明细查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息)(void* arg, 0-否,1-是)
  */
  void (*xOnRspFASLQryFreezeFundsDetail)(void* arg, DFITCStockRspQryFreezeFundsDetailField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-证券冻结明细查询响应
  * @param pData:指针若非空,返回用户证券冻结明细查询响应信息结构地址,表明客户证券冻结明细查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户证券冻结明细查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息)(void* arg, 0-否,1-是)
  */
  void (*xOnRspFASLQryFreezeStockDetail)(void* arg, DFITCStockRspQryFreezeStockDetailField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-资金调拨明细查询响应
  * @param pData:指针若非空,返回用户资金调拨明细查询响应信息结构地址,表明客户资金调拨明细查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户资金调拨明细查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息)(void* arg, 0-否,1-是)
  */
  void (*xOnRspFASLQryTransferFundsDetail)(void* arg, DFITCStockRspQryTransferFundsDetailField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-撤单响应
  * @param pData:指针若非空,返回用户撤单响应信息结构地址,表明撤单请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明撤单请求失败
  */
  void (*xOnRspFASLWithdrawOrder)(void* arg, DFITCFASLRspWithdrawOrderField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-当前系统时间查询请求响应
  * @param pData:指针若非空,返回用户系统时间查询响应信息结构地址,表明系统时间查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明系统时间查询请求失败
  */
  void (*xOnRspFASLQrySystemTime)(void* arg, DFITCFASLRspQryTradeTimeField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-可转入担保证券查询请求响应
  * @param pData:指针若非空,返回可转入担保证券查询响应信息结构地址,表明可转入担保证券查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明可转入担保证券查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息)(void* arg, 0-否,1-是)
  */
  void (*xOnRspFASLQryTransferredContract)(void* arg, DFITCFASLRspQryTransferredContractField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-客户可取资金调出请求响应
  * @param pData:指针若非空,返回客户可取资金调出响应信息结构地址,表明客户可取资金调出请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户可取资金调出请求失败
  */
  void (*xOnRspFASLDesirableFundsOut)(void* arg, DFITCFASLRspDesirableFundsOutField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-担保证券查询请求响应
  * @param pData:指针若非空,返回担保证券查询响应信息结构地址,表明担保证券查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明担保证券查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息)(void* arg, 0-否,1-是)
  */
  void (*xOnRspFASLQryGuaranteedContract)(void* arg, DFITCFASLRspQryGuaranteedContractField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-标的证券查询请求响应
  * @param pData:指针若非空,返回标的证券查询响应信息结构地址,表明标的证券查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明标的证券查询失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息)(void* arg, 0-否,1-是)
  */
  void (*xOnRspFASLQryUnderlyingContract)(void* arg, DFITCFASLRspQryUnderlyingContractField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-委托回报响应
  * @param pData:返回委托回报结构体的地址
  */
  void (*xOnFASLEntrustOrderRtn)(void* arg, DFITCStockEntrustOrderRtnField *pData);
  /**
  * FASL-成交回报响应
  * @param pData:返回成交回报结构体的地址
  */
  void (*xOnFASLTradeRtn)(void* arg, DFITCStockTradeRtnField *pData);
  /**
  * FASL-撤单回报响应
  * @param pData:返回撤单回报结构体的地址
  */
  void (*xOnFASLWithdrawOrderRtn)(void* arg, DFITCStockWithdrawOrderRtnField *pData);
};
#ifdef __cplusplus
}
#endif


class CDfitcSecTraderHandler : public DFITCSECTraderSpi
{
public:
  /**
   * SEC-网络连接正常响应
   */
  void OnFrontConnected();
  /**
   * SEC-网络连接不正常响应
   */
  void OnFrontDisconnected(int nReason);
  /**
   * SEC-消息通知
   */
  void OnRtnNotice(DFITCSECRspNoticeField *pNotice);
  /**
  * ERR-错误应答
  * @param pRspInfo:指针若非空，返回错误信息结构地址
  */
  void OnRspError(DFITCSECRspInfoField *pRspInfo);
  /**
  * STOCK-登录响应
  * @param pData:指针若非空,返回用户登录响应信息结构体的地址,表明登录请求成功
  * @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明登录请求失败
  */
  void OnRspStockUserLogin(DFITCSECRspUserLoginField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * STOCK-登出响应
  * @param pData:指针若非空,返回用户登出响应信息结构体的地址,表明登出请求成功
  * @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明登出请求失败
  */
  void OnRspStockUserLogout(DFITCSECRspUserLogoutField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * STOCK-密码更新响应
  * @param pData:指针若非空,返回用户密码更新响应信息结构体的地址,表明密码更新请求成功
  * @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明密码更新请求失败
  */
  void OnRspStockUserPasswordUpdate(DFITCSECRspPasswordUpdateField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * STOCK-委托报单响应
  * @param pData:指针若非空,返回用户委托报单响应信息结构体的地址,表明报单请求成功
  * @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明委托报单请求失败
  */
  void OnRspStockEntrustOrder(DFITCStockRspEntrustOrderField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * STOCK-委托撤单响应
  * @param pData:指针若非空,返回用户委托撤单响应信息结构体的地址,表明撤单请求成功
  * @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明委托撤单请求失败
  */
  void OnRspStockWithdrawOrder(DFITCSECRspWithdrawOrderField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * STOCK-委托查询响应
  * @param pData:指针若非空,返回用户委托查询响应信息结构体的地址,表明查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明委托查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
  */
  void OnRspStockQryEntrustOrder(DFITCStockRspQryEntrustOrderField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * STOCK-实时成交查询响应
  * @param pData:指针若非空,返回用户实时成交查询响应信息结构体的地址,表明实时成交查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明实时成交查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
  */
  void OnRspStockQryRealTimeTrade(DFITCStockRspQryRealTimeTradeField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * STOCK-分笔成交查询响应
  * @param pData:指针若非空,返回用户分笔成交查询响应信息结构体的地址,表明分笔成交查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明分笔成交查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
  */
  void OnRspStockQrySerialTrade(DFITCStockRspQrySerialTradeField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * STOCK-持仓查询响应
  * @param pData:指针若非空,返回用户持仓查询响应信息结构体的地址,表明持仓查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明持仓查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
  */
  void OnRspStockQryPosition(DFITCStockRspQryPositionField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * STOCK-资金账号查询响应
  * @param pData:指针若非空,返回用户资金账号查询响应信息结构体的地址,表明资金账号查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明资金账号查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
  */
  void OnRspStockQryCapitalAccountInfo(DFITCStockRspQryCapitalAccountField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * STOCK-交易账号查询响应
  * @param pData:指针若非空,返回用户交易账号查询响应信息结构体的地址,表明交易账号查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明交易账号查询请求失败
  */
  void OnRspStockQryAccountInfo(DFITCStockRspQryAccountField *pData, DFITCSECRspInfoField *pRspInfo); 
  /**
  * STOCK-股东号查询响应
  * @param pData:指针若非空,返回用户股东号查询响应信息结构体的地址,表明股东号查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明股东号查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
  */
  void OnRspStockQryShareholderInfo(DFITCStockRspQryShareholderField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast); 
  /**
  * STOCK-资金调拨响应
  * @param pData:指针若非空,返回用户资金调拨响应信息结构体的地址,表明股资金调拨请求成功
  * @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明资金调拨请求失败
  */
  void OnRspStockTransferFunds(DFITCStockRspTransferFundsField *pData,DFITCSECRspInfoField *pRspInfo);
  /**
  * STOCK-批量委托响应
  * @param pData:指针若非空,返回用户批量委托响应信息结构体的地址,表明批量委托请求成功
  * @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明批量委托请求失败
  */
  void OnRspStockEntrustBatchOrder(DFITCStockRspEntrustBatchOrderField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * STOCK-批量撤单响应
  * @param pData:指针若非空,返回用户批量撤单响应信息结构体的地址,表明批量撤单请求成功
  * @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明批量撤单请求失败
  */
  void OnRspStockWithdrawBatchOrder(DFITCStockRspWithdrawBatchOrderField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * STOCK-计算可委托数量响应
  * @param pData:指针若非空,返回用户计算可委托数量响应信息结构体的地址,表明计算可委托数量请求成功
  * @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明计算可委托数量请求失败
  */
  void OnRspStockCalcAbleEntrustQty(DFITCStockRspCalcAbleEntrustQtyField *pData, DFITCSECRspInfoField *pRspInfo); 
  /**
  * STOCK-计算申购ETF数量响应
  * @param pData:指针若非空,返回用户计算申购ETF数量响应信息结构体的地址,表明计算申购ETF数量请求成功
  * @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明计算申购ETF数量请求失败
  */
  void OnRspStockCalcAblePurchaseETFQty(DFITCStockRspCalcAblePurchaseETFQtyField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * STOCK-冻结资金明细查询响应
  * @param pData:指针若非空,返回用户冻结资金明细查询响应信息结构体的地址,表明冻结资金明细查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明冻结资金明细查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
  */
  void OnRspStockQryFreezeFundsDetail(DFITCStockRspQryFreezeFundsDetailField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast); 
  /**
  * STOCK-冻结证券明细查询响应
  * @param pData:指针若非空,返回用户冻结证券明细查询响应信息结构体的地址,表明冻结证券明细查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明冻结证券明细查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
  */
  void OnRspStockQryFreezeStockDetail(DFITCStockRspQryFreezeStockDetailField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * STOCK-调拨证券明细查询响应
  * @param pData:指针若非空,返回用户调拨证券明细查询响应信息结构体的地址,表明调拨证券明细查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明调拨证券明细查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
  */
  void OnRspStockQryTransferStockDetail(DFITCStockRspQryTransferStockDetailField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * STOCK-调拨资金明细查询响应
  * @param pData:指针若非空,返回用户调拨资金明细查询响应信息结构体的地址,表明调拨资金明细查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明调拨资金明细查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
  */
  void OnRspStockQryTransferFundsDetail(DFITCStockRspQryTransferFundsDetailField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * STOCK-证券信息查询响应
  * @param pData:指针若非空,返回用户证券信息查询响应信息结构体的地址,表明证券信息查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明证券信息查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
  */
  void OnRspStockQryStockInfo(DFITCStockRspQryStockField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * STOCK-证券静态信息查询响应
  * @param pData:指针若非空,返回用户证券静态信息查询响应信息结构体的地址,表明证券静态信息查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明证券静态信息查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
  */
  void OnRspStockQryStockStaticInfo(DFITCStockRspQryStockStaticField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * STOCK-交易时间查询响应
  * @param pData:指针若非空,返回用户交易时间查询响应信息结构体的地址,表明交易时间查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息结构体的地址，表明交易时间查询请求失败
  */
  void OnRspStockQryTradeTime(DFITCStockRspQryTradeTimeField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * STOCK-委托回报响应
  * @param pData:返回委托回报结构体的地址
  */
  void OnStockEntrustOrderRtn(DFITCStockEntrustOrderRtnField * pData);
  /**
  * STOCK-成交回报响应
  * @param pData:返回成交回报结构体的地址
  */
  void OnStockTradeRtn(DFITCStockTradeRtnField * pData);
  /**
  * STOCK-撤单回报响应
  * @param pData:返回撤单回报结构体的地址
  */
  void OnStockWithdrawOrderRtn(DFITCStockWithdrawOrderRtnField * pData);
  
  /**
  * SOP-登录响应
  * @param pRspUserLogin:指针若非空,返回用户登录响应信息结构地址,表明登录请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明登录请求失败
  */
  void OnRspSOPUserLogin(DFITCSECRspUserLoginField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
   * SOP-登出响应
   * @param pData:指针若非空,返回用户登出响应信息结构地址,表明登出请求成功
   * @param pRspInfo:指针若非空，返回错误信息地址，表明登出请求失败
   */
  void OnRspSOPUserLogout(DFITCSECRspUserLogoutField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * SOP-用户口令更新响应
  * @param pData:指针若非空,返回用户口令更新响应信息结构地址,表明用户口令更新请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明用户口令更新请求失败
  */
  void OnRspSOPUserPasswordUpdate(DFITCSECRspPasswordUpdateField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * SOP-报单响应
  * @param pData:指针若非空,返回用户报单响应信息结构地址,表明报单请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明报单请求失败
  */
  void OnRspSOPEntrustOrder(DFITCSOPRspEntrustOrderField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * SOP-组合拆分委托响应
  * @param pData:指针若非空,返回用户响应信息结构地址,表明组合拆分委托请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明组合拆分委托请求失败
  */
  void OnRspSOPGroupSplit(DFITCSOPRspEntrustOrderField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * SOP-查询客户组合持仓明细响应
  * @param pData:指针若非空,返回用户查询客户组合持仓明细响应结构地址,表明查询客户组合持仓明细请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明查询客户组合持仓明细请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
  */
  void OnRspSOPQryGroupPosition(DFITCSOPRspQryGroupPositionField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * SOP-证券锁定解锁响应
  * @param pData:指针若非空,返回用户证券锁定解锁响应信息结构地址,表明证券锁定解锁请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明证券锁定解锁请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
  */
  void OnRspSOPLockOUnLockStock(DFITCSOPRspLockOUnLockStockField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast); 
  /**
  * SOP-撤单响应
  * @param pData:指针若非空,返回用户撤单响应信息结构地址,表明撤单请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明撤单请求失败
  */
  void OnRspSOPWithdrawOrder(DFITCSECRspWithdrawOrderField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * SOP-委托查询响应
  * @param pData:指针若非空,返回用户委托查询响应信息结构地址,表明委托查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明委托查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
  */
  void OnRspSOPQryEntrustOrder(DFITCSOPRspQryEntrustOrderField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * SOP-分笔成交查询响应
  * @param pData:指针若非空,返回用户分笔成交查询响应信息结构地址,表明分笔成交查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明分笔成交查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
  */
  void OnRspSOPQrySerialTrade(DFITCSOPRspQrySerialTradeField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * SOP-持仓查询响应
  * @param pData:指针若非空,返回用户持仓查询响应信息结构地址,表明持仓查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明持仓查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
  */
  void OnRspSOPQryPosition(DFITCSOPRspQryPositionField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * SOP-客户担保持仓查询响应
  * @param pData:指针若非空,返回用户客户担保持仓查询响应信息结构地址,表明客户担保持仓查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户担保持仓查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
  */
  void OnRspSOPQryCollateralPosition(DFITCSOPRspQryCollateralPositionField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * SOP-客户资金查询响应
  * @param pData:指针若非空,返回用户客户资金查询响应信息结构地址,表明客户客户资金查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户客户资金查询请求失败
  */
  void OnRspSOPQryCapitalAccountInfo(DFITCSOPRspQryCapitalAccountField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * SOP-客户信息查询响应
  * @param pData:指针若非空,返回用户客户信息查询响应信息结构地址,表明客户客户信息查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户客户信息查询请求失败
  */
  void OnRspSOPQryAccountInfo(DFITCSOPRspQryAccountField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * SOP-股东信息查询响应
  * @param pData:指针若非空,返回用户股东信息查询响应信息结构地址,表明客户股东信息查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户股东信息查询请求失败
  */
  void OnRspSOPQryShareholderInfo(DFITCSOPRspQryShareholderField *pData, DFITCSECRspInfoField *pRspInfo); 
  /**
  * SOP-可委托数量查询响应
  * @param pData:指针若非空,返回用户可委托数量查询响应信息结构地址,表明客户可委托数量查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户可委托数量查询请求失败
  */
  void OnRspSOPCalcAbleEntrustQty(DFITCSOPRspCalcAbleEntrustQtyField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * SOP-客户可锁定证券查询响应
  * @param pData:指针若非空,返回用户客户可锁定证券查询响应信息结构地址,表明客户客户可锁定证券查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户客户可锁定证券查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
  */
  void OnRspSOPQryAbleLockStock(DFITCSOPRspQryAbleLockStockField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast); 
  /**
  * SOP-期权合约代码查询响应
  * @param pData:指针若非空,返回用户期权合约代码查询响应信息结构地址,表明客户期权合约代码查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户期权合约代码查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
  */
  void OnRspSOPQryContactInfo(DFITCSOPRspQryContactField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast); 
  /**
  * SOP-执行委托响应
  * @param pData:指针若非空,返回用户执行委托响应信息结构地址,表明客户执行委托请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户执行委托请求失败
  */
  void OnRspSOPExectueOrder(DFITCSOPRspExectueOrderField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * SOP-客户行权指派信息查询响应
  * @param pData:指针若非空,返回用户客户行权指派信息查询响应信息结构地址,表明客户客户行权指派信息查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户客户行权指派信息查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
  */
  void OnRspSOPQryExecAssiInfo(DFITCSOPRspQryExecAssiInfoField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * SOP-查询交易时间响应
  * @param pData:指针若非空,返回用户查询交易时间响应信息结构地址,表明客户查询交易时间请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户查询交易时间请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
  */
  void OnRspSOPQryTradeTime(DFITCSOPRspQryTradeTimeField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * SOP-获取所有交易所参数响应
  * @param pData:指针若非空,返回用户获取所有交易所参数响应信息结构地址,表明客户获取所有交易所参数请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户获取所有交易所参数请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
  */
  void OnRspSOPQryExchangeInfo(DFITCSOPRspQryExchangeInfoField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * SOP-查询手续费参数响应
  * @param pData:指针若非空,返回用户查询手续费参数响应信息结构地址,表明客户查询手续费参数请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户查询手续费参数请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
  */
  void OnRspSOPQryCommission(DFITCSOPRspQryCommissionField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * SOP-查询保证金率参数响应
  * @param pData:指针若非空,返回用户查询保证金率参数响应信息结构地址,表明客户查询保证金率参数请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户查询保证金率参数请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
  */
  void OnRspSOPQryDeposit(DFITCSOPRspQryDepositField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * SOP-期权标的信息查询响应
  * @param pData:指针若非空,返回用户期权标的信息查询响应信息结构地址,表明客户期权标的信息查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户期权标的信息查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
  */
  void OnRspSOPQryContractObjectInfo(DFITCSOPRspQryContractObjectField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * SOP-委托回报响应
  * @param pData:返回委托回报结构体的地址
  */
  void OnSOPEntrustOrderRtn(DFITCSOPEntrustOrderRtnField * pData);
  /**
  * SOP-成交回报响应
  * @param pData:返回成交回报结构体的地址
  */
  void OnSOPTradeRtn(DFITCSOPTradeRtnField * pData);
  /**
  * SOP-撤单回报响应
  * @param pData:返回撤单回报结构体的地址
  */
  void OnSOPWithdrawOrderRtn(DFITCSOPWithdrawOrderRtnField * pData);

  /**
  * FASL-登录响应
  * @param pData:指针若非空,返回用户融资融券登录响应信息结构地址,表明客户融资融券登录请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户融资融券登录请求失败
  */
  void OnRspFASLUserLogin(DFITCSECRspUserLoginField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-登出响应
  * @param pData:指针若非空,返回用户融资融券登出响应信息结构地址,表明客户融资融券登出请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户融资融券登出请求失败
  */
  void OnRspFASLUserLogout(DFITCSECRspUserLogoutField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-客户可融资信息响应
  * @param pData:指针若非空,返回用户客户可融资信息响应信息结构地址,表明客户客户可融资信息请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户客户可融资信息请求失败
  */
  void OnRspFASLQryAbleFinInfo(DFITCFASLRspAbleFinInfoField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-客户可融券信息响应
  * @param pData:指针若非空,返回用户客户可融券信息响应信息结构地址,表明客户客户可融券信息请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户客户可融券信息请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
  */
  void OnRspFASLQryAbleSloInfo(DFITCFASLRspAbleSloInfoField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-担保物划转响应
  * @param pData:指针若非空,返回用户担保物划转响应信息结构地址,表明客户担保物划转请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户担保物划转请求失败
  */
  void OnRspFASLTransferCollateral(DFITCFASLRspTransferCollateralField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-直接还款响应
  * @param pData:指针若非空,返回用户直接还款响应信息结构地址,表明客户直接还款请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户直接还款请求失败
  */
  void OnRspFASLDirectRepayment(DFITCFASLRspDirectRepaymentField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-还券划转响应
  * @param pData:指针若非空,返回用户还券划转响应信息结构地址,表明客户还券划转请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户还券划转请求失败
  */
  void OnRspFASLRepayStockTransfer(DFITCFASLRspRepayStockTransferField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-信用交易响应
  * @param pData:指针若非空,返回用户信用交易响应信息结构地址,表明客户信用交易请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户信用交易请求失败
  */
  void OnRspFASLEntrustCrdtOrder(DFITCFASLRspEntrustCrdtOrderField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-融资融券交易响应
  * @param pData:指针若非空,返回用户融资融券交易响应信息结构地址,表明客户融资融券交易请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户融资融券交易请求失败
  */
  void OnRspFASLEntrustOrder(DFITCFASLRspEntrustOrderField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-信用可委托数量查询响应
  * @param pData:指针若非空,返回用户信用可委托数量查询响应信息结构地址,表明客户信用可委托数量查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户信用可委托数量查询请求失败
  */
  void OnRspFASLCalcAbleEntrustCrdtQty(DFITCFASLRspCalcAbleEntrustCrdtQtyField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-查询信用资金响应
  * @param pData:指针若非空,返回用户查询信用资金响应信息结构地址,表明客户查询信用资金请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户查询信用资金请求失败
  */
  void OnRspFASLQryCrdtFunds(DFITCFASLRspQryCrdtFundsField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-信用合约信息响应
  * @param pData:指针若非空,返回用户信用合约信息响应信息结构地址,表明客户信用合约信息请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户信用合约信息请求失败
  */
  void OnRspFASLQryCrdtContract(DFITCFASLRspQryCrdtContractField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLat);
  /**
  * FASL-信用合约变动信息查询响应
  * @param pData:指针若非空,返回用户信用合约变动信息查询响应信息结构地址,表明客户信用合约变动信息查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户信用合约变动信息查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
  */
  void OnRspFASLQryCrdtConChangeInfo(DFITCFASLRspQryCrdtConChangeInfoField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-资金调转响应
  * @param pData:指针若非空,返回用户资金调转响应信息结构地址,表明客户资金调转请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户资金调转请求失败
  */
  void OnRspFASLTransferFunds(DFITCStockRspTransferFundsField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-客户信息查询响应
  * @param pData:指针若非空,返回用户客户信息查询响应信息结构地址,表明客户客户信息查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户客户信息查询请求失败
  */
  void OnRspFASLQryAccountInfo(DFITCStockRspQryAccountField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-客户资金查询响应
  * @param pData:指针若非空,返回用户客户资金查询响应信息结构地址,表明客户客户资金查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户客户资金查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
  */
  void OnRspFASLQryCapitalAccountInfo(DFITCStockRspQryCapitalAccountField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-股东信息查询响应
  * @param pData:指针若非空,返回用户股东信息查询响应信息结构地址,表明客户股东信息查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户股东信息查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
  */
  void OnRspFASLQryShareholderInfo(DFITCStockRspQryShareholderField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-持仓查询响应
  * @param pData:指针若非空,返回用户持仓查询响应信息结构地址,表明客户持仓查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户持仓查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
  */
  void OnRspFASLQryPosition(DFITCStockRspQryPositionField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-委托查询响应
  * @param pData:指针若非空,返回用户委托查询响应信息结构地址,表明客户委托查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户委托查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
  */
  void OnRspFASLQryEntrustOrder(DFITCStockRspQryEntrustOrderField * pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-分笔成交查询响应
  * @param pData:指针若非空,返回用户分笔成交查询响应信息结构地址,表明客户分笔成交查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户分笔成交查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
  */
  void OnRspFASLQrySerialTrade(DFITCStockRspQrySerialTradeField * pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-实时成交查询响应
  * @param pData:指针若非空,返回用户实时成交查询响应信息结构地址,表明客户实时成交查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户实时成交查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
  */
  void OnRspFASLQryRealTimeTrade(DFITCStockRspQryRealTimeTradeField * pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-资金冻结明细查询响应
  * @param pData:指针若非空,返回用户资金冻结明细查询响应信息结构地址,表明客户资金冻结明细查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户资金冻结明细查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
  */
  void OnRspFASLQryFreezeFundsDetail(DFITCStockRspQryFreezeFundsDetailField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-证券冻结明细查询响应
  * @param pData:指针若非空,返回用户证券冻结明细查询响应信息结构地址,表明客户证券冻结明细查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户证券冻结明细查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
  */
  void OnRspFASLQryFreezeStockDetail(DFITCStockRspQryFreezeStockDetailField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-资金调拨明细查询响应
  * @param pData:指针若非空,返回用户资金调拨明细查询响应信息结构地址,表明客户资金调拨明细查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户资金调拨明细查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
  */
  void OnRspFASLQryTransferFundsDetail(DFITCStockRspQryTransferFundsDetailField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-撤单响应
  * @param pData:指针若非空,返回用户撤单响应信息结构地址,表明撤单请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明撤单请求失败
  */
  void OnRspFASLWithdrawOrder(DFITCFASLRspWithdrawOrderField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-当前系统时间查询请求响应
  * @param pData:指针若非空,返回用户系统时间查询响应信息结构地址,表明系统时间查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明系统时间查询请求失败
  */
  void OnRspFASLQrySystemTime(DFITCFASLRspQryTradeTimeField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-可转入担保证券查询请求响应
  * @param pData:指针若非空,返回可转入担保证券查询响应信息结构地址,表明可转入担保证券查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明可转入担保证券查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
  */
  void OnRspFASLQryTransferredContract(DFITCFASLRspQryTransferredContractField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-客户可取资金调出请求响应
  * @param pData:指针若非空,返回客户可取资金调出响应信息结构地址,表明客户可取资金调出请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明客户可取资金调出请求失败
  */
  void OnRspFASLDesirableFundsOut(DFITCFASLRspDesirableFundsOutField *pData, DFITCSECRspInfoField *pRspInfo);
  /**
  * FASL-担保证券查询请求响应
  * @param pData:指针若非空,返回担保证券查询响应信息结构地址,表明担保证券查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明担保证券查询请求失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
  */
  void OnRspFASLQryGuaranteedContract(DFITCFASLRspQryGuaranteedContractField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-标的证券查询请求响应
  * @param pData:指针若非空,返回标的证券查询响应信息结构地址,表明标的证券查询请求成功
  * @param pRspInfo:指针若非空，返回错误信息地址，表明标的证券查询失败
  * @param bIsLast:返回值表明是否是最后一笔响应信息(0-否,1-是)
  */
  void OnRspFASLQryUnderlyingContract(DFITCFASLRspQryUnderlyingContractField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast);
  /**
  * FASL-委托回报响应
  * @param pData:返回委托回报结构体的地址
  */
  void OnFASLEntrustOrderRtn(DFITCStockEntrustOrderRtnField *pData);
  /**
  * FASL-成交回报响应
  * @param pData:返回成交回报结构体的地址
  */
  void OnFASLTradeRtn(DFITCStockTradeRtnField *pData);
  /**
  * FASL-撤单回报响应
  * @param pData:返回撤单回报结构体的地址
  */
  void OnFASLWithdrawOrderRtn(DFITCStockWithdrawOrderRtnField *pData);
  
public:
  CDfitcSecTraderHandler(dfitc_sec_trader_api_cb *pUserCb, void * parent);
  ~CDfitcSecTraderHandler();

private:
  dfitc_sec_trader_api_cb* m_pUserCb;
  void* m_parent;
};

#endif //_DFITC_SEC_TRADER_HANDLER_H_

