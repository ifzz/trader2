#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "DFITCSECApiDataType.h"
#include "DFITCSECApiStruct.h"
#include "DFITCSECTraderApi.h"

#include "DFITCSECTraderHandler.h"

CDfitcSecTraderHandler::CDfitcSecTraderHandler(dfitc_sec_trader_api_cb *pUserCb, void * parent)
	: m_pUserCb(pUserCb) , m_parent(parent)
{
}

CDfitcSecTraderHandler::~CDfitcSecTraderHandler() 
{
}

/**
 * SEC-��������������Ӧ
 */
void CDfitcSecTraderHandler::OnFrontConnected() {
m_pUserCb->xOnFrontConnected(m_parent);
}
/**
 * SEC-�������Ӳ�������Ӧ
 */
void CDfitcSecTraderHandler::OnFrontDisconnected(int nReason) {
m_pUserCb->xOnFrontDisconnected(m_parent, nReason);
}
/**
 * SEC-��Ϣ֪ͨ
 */
void CDfitcSecTraderHandler::OnRtnNotice(DFITCSECRspNoticeField *pNotice) {
m_pUserCb->xOnRtnNotice(m_parent, pNotice);
}
/**
* ERR-����Ӧ��
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ַ
*/
void CDfitcSecTraderHandler::OnRspError(DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspError(m_parent, pRspInfo);
}
/**
* STOCK-��¼��Ӧ
* @param pData:ָ�����ǿ�,�����û���¼��Ӧ��Ϣ�ṹ��ĵ�ַ,������¼����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ��������¼����ʧ��
*/
void CDfitcSecTraderHandler::OnRspStockUserLogin(DFITCSECRspUserLoginField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspStockUserLogin(m_parent, pData, pRspInfo);
}
/**
* STOCK-�ǳ���Ӧ
* @param pData:ָ�����ǿ�,�����û��ǳ���Ӧ��Ϣ�ṹ��ĵ�ַ,�����ǳ�����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ�������ǳ�����ʧ��
*/
void CDfitcSecTraderHandler::OnRspStockUserLogout(DFITCSECRspUserLogoutField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspStockUserLogout(m_parent, pData, pRspInfo);
}
/**
* STOCK-���������Ӧ
* @param pData:ָ�����ǿ�,�����û����������Ӧ��Ϣ�ṹ��ĵ�ַ,���������������ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ�����������������ʧ��
*/
void CDfitcSecTraderHandler::OnRspStockUserPasswordUpdate(DFITCSECRspPasswordUpdateField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspStockUserPasswordUpdate(m_parent, pData, pRspInfo);
}
/**
* STOCK-ί�б�����Ӧ
* @param pData:ָ�����ǿ�,�����û�ί�б�����Ӧ��Ϣ�ṹ��ĵ�ַ,������������ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ������ί�б�������ʧ��
*/
void CDfitcSecTraderHandler::OnRspStockEntrustOrder(DFITCStockRspEntrustOrderField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspStockEntrustOrder(m_parent, pData, pRspInfo);
}
/**
* STOCK-ί�г�����Ӧ
* @param pData:ָ�����ǿ�,�����û�ί�г�����Ӧ��Ϣ�ṹ��ĵ�ַ,������������ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ������ί�г�������ʧ��
*/
void CDfitcSecTraderHandler::OnRspStockWithdrawOrder(DFITCSECRspWithdrawOrderField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspStockWithdrawOrder(m_parent, pData, pRspInfo);
}
/**
* STOCK-ί�в�ѯ��Ӧ
* @param pData:ָ�����ǿ�,�����û�ί�в�ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,������ѯ����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ������ί�в�ѯ����ʧ��
* @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
*/
void CDfitcSecTraderHandler::OnRspStockQryEntrustOrder(DFITCStockRspQryEntrustOrderField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspStockQryEntrustOrder(m_parent, pData, pRspInfo, bIsLast);
}
/**
* STOCK-ʵʱ�ɽ���ѯ��Ӧ
* @param pData:ָ�����ǿ�,�����û�ʵʱ�ɽ���ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,����ʵʱ�ɽ���ѯ����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ������ʵʱ�ɽ���ѯ����ʧ��
* @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
*/
void CDfitcSecTraderHandler::OnRspStockQryRealTimeTrade(DFITCStockRspQryRealTimeTradeField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspStockQryRealTimeTrade(m_parent, pData, pRspInfo, bIsLast);
}
/**
* STOCK-�ֱʳɽ���ѯ��Ӧ
* @param pData:ָ�����ǿ�,�����û��ֱʳɽ���ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,�����ֱʳɽ���ѯ����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ�������ֱʳɽ���ѯ����ʧ��
* @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
*/
void CDfitcSecTraderHandler::OnRspStockQrySerialTrade(DFITCStockRspQrySerialTradeField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspStockQrySerialTrade(m_parent, pData, pRspInfo, bIsLast);
}
/**
* STOCK-�ֲֲ�ѯ��Ӧ
* @param pData:ָ�����ǿ�,�����û��ֲֲ�ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,�����ֲֲ�ѯ����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ�������ֲֲ�ѯ����ʧ��
* @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
*/
void CDfitcSecTraderHandler::OnRspStockQryPosition(DFITCStockRspQryPositionField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspStockQryPosition(m_parent, pData, pRspInfo, bIsLast);
}
/**
* STOCK-�ʽ��˺Ų�ѯ��Ӧ
* @param pData:ָ�����ǿ�,�����û��ʽ��˺Ų�ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,�����ʽ��˺Ų�ѯ����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ�������ʽ��˺Ų�ѯ����ʧ��
* @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
*/
void CDfitcSecTraderHandler::OnRspStockQryCapitalAccountInfo(DFITCStockRspQryCapitalAccountField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspStockQryCapitalAccountInfo(m_parent, pData, pRspInfo, bIsLast);
}
/**
* STOCK-�����˺Ų�ѯ��Ӧ
* @param pData:ָ�����ǿ�,�����û������˺Ų�ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,���������˺Ų�ѯ����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ�����������˺Ų�ѯ����ʧ��
*/
void CDfitcSecTraderHandler::OnRspStockQryAccountInfo(DFITCStockRspQryAccountField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspStockQryAccountInfo(m_parent, pData, pRspInfo);
}
/**
* STOCK-�ɶ��Ų�ѯ��Ӧ
* @param pData:ָ�����ǿ�,�����û��ɶ��Ų�ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,�����ɶ��Ų�ѯ����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ�������ɶ��Ų�ѯ����ʧ��
* @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
*/
void CDfitcSecTraderHandler::OnRspStockQryShareholderInfo(DFITCStockRspQryShareholderField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspStockQryShareholderInfo(m_parent, pData, pRspInfo, bIsLast);
}
/**
* STOCK-�ʽ������Ӧ
* @param pData:ָ�����ǿ�,�����û��ʽ������Ӧ��Ϣ�ṹ��ĵ�ַ,�������ʽ��������ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ�������ʽ��������ʧ��
*/
void CDfitcSecTraderHandler::OnRspStockTransferFunds(DFITCStockRspTransferFundsField *pData,DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspStockTransferFunds(m_parent, pData, pRspInfo);
}
/**
* STOCK-����ί����Ӧ
* @param pData:ָ�����ǿ�,�����û�����ί����Ӧ��Ϣ�ṹ��ĵ�ַ,��������ί������ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ����������ί������ʧ��
*/
void CDfitcSecTraderHandler::OnRspStockEntrustBatchOrder(DFITCStockRspEntrustBatchOrderField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspStockEntrustBatchOrder(m_parent, pData, pRspInfo);
}
/**
* STOCK-����������Ӧ
* @param pData:ָ�����ǿ�,�����û�����������Ӧ��Ϣ�ṹ��ĵ�ַ,����������������ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ������������������ʧ��
*/
void CDfitcSecTraderHandler::OnRspStockWithdrawBatchOrder(DFITCStockRspWithdrawBatchOrderField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspStockWithdrawBatchOrder(m_parent, pData, pRspInfo);
}
/**
* STOCK-�����ί��������Ӧ
* @param pData:ָ�����ǿ�,�����û������ί��������Ӧ��Ϣ�ṹ��ĵ�ַ,���������ί����������ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ�����������ί����������ʧ��
*/
void CDfitcSecTraderHandler::OnRspStockCalcAbleEntrustQty(DFITCStockRspCalcAbleEntrustQtyField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspStockCalcAbleEntrustQty(m_parent, pData, pRspInfo);
}
/**
* STOCK-�����깺ETF������Ӧ
* @param pData:ָ�����ǿ�,�����û������깺ETF������Ӧ��Ϣ�ṹ��ĵ�ַ,���������깺ETF��������ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ�����������깺ETF��������ʧ��
*/
void CDfitcSecTraderHandler::OnRspStockCalcAblePurchaseETFQty(DFITCStockRspCalcAblePurchaseETFQtyField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspStockCalcAblePurchaseETFQty(m_parent, pData, pRspInfo);
}
/**
* STOCK-�����ʽ���ϸ��ѯ��Ӧ
* @param pData:ָ�����ǿ�,�����û������ʽ���ϸ��ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,���������ʽ���ϸ��ѯ����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ�����������ʽ���ϸ��ѯ����ʧ��
* @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
*/
void CDfitcSecTraderHandler::OnRspStockQryFreezeFundsDetail(DFITCStockRspQryFreezeFundsDetailField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspStockQryFreezeFundsDetail(m_parent, pData, pRspInfo, bIsLast);
}
/**
* STOCK-����֤ȯ��ϸ��ѯ��Ӧ
* @param pData:ָ�����ǿ�,�����û�����֤ȯ��ϸ��ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,��������֤ȯ��ϸ��ѯ����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ����������֤ȯ��ϸ��ѯ����ʧ��
* @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
*/
void CDfitcSecTraderHandler::OnRspStockQryFreezeStockDetail(DFITCStockRspQryFreezeStockDetailField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspStockQryFreezeStockDetail(m_parent, pData, pRspInfo, bIsLast);
}
/**
* STOCK-����֤ȯ��ϸ��ѯ��Ӧ
* @param pData:ָ�����ǿ�,�����û�����֤ȯ��ϸ��ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,��������֤ȯ��ϸ��ѯ����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ����������֤ȯ��ϸ��ѯ����ʧ��
* @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
*/
void CDfitcSecTraderHandler::OnRspStockQryTransferStockDetail(DFITCStockRspQryTransferStockDetailField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspStockQryTransferStockDetail(m_parent, pData, pRspInfo, bIsLast);
}
/**
* STOCK-�����ʽ���ϸ��ѯ��Ӧ
* @param pData:ָ�����ǿ�,�����û������ʽ���ϸ��ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,���������ʽ���ϸ��ѯ����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ�����������ʽ���ϸ��ѯ����ʧ��
* @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
*/
void CDfitcSecTraderHandler::OnRspStockQryTransferFundsDetail(DFITCStockRspQryTransferFundsDetailField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspStockQryTransferFundsDetail(m_parent, pData, pRspInfo, bIsLast);
}
/**
* STOCK-֤ȯ��Ϣ��ѯ��Ӧ
* @param pData:ָ�����ǿ�,�����û�֤ȯ��Ϣ��ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,����֤ȯ��Ϣ��ѯ����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ������֤ȯ��Ϣ��ѯ����ʧ��
* @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
*/
void CDfitcSecTraderHandler::OnRspStockQryStockInfo(DFITCStockRspQryStockField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspStockQryStockInfo(m_parent, pData, pRspInfo, bIsLast);
}
/**
* STOCK-֤ȯ��̬��Ϣ��ѯ��Ӧ
* @param pData:ָ�����ǿ�,�����û�֤ȯ��̬��Ϣ��ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,����֤ȯ��̬��Ϣ��ѯ����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ������֤ȯ��̬��Ϣ��ѯ����ʧ��
* @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
*/
void CDfitcSecTraderHandler::OnRspStockQryStockStaticInfo(DFITCStockRspQryStockStaticField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspStockQryStockStaticInfo(m_parent, pData, pRspInfo, bIsLast);
}
/**
* STOCK-����ʱ���ѯ��Ӧ
* @param pData:ָ�����ǿ�,�����û�����ʱ���ѯ��Ӧ��Ϣ�ṹ��ĵ�ַ,��������ʱ���ѯ����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ�ṹ��ĵ�ַ����������ʱ���ѯ����ʧ��
*/
void CDfitcSecTraderHandler::OnRspStockQryTradeTime(DFITCStockRspQryTradeTimeField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspStockQryTradeTime(m_parent, pData, pRspInfo);
}
/**
* STOCK-ί�лر���Ӧ
* @param pData:����ί�лر��ṹ��ĵ�ַ
*/
void CDfitcSecTraderHandler::OnStockEntrustOrderRtn(DFITCStockEntrustOrderRtnField * pData){
m_pUserCb->xOnStockEntrustOrderRtn(m_parent, pData);
}
/**
* STOCK-�ɽ��ر���Ӧ
* @param pData:���سɽ��ر��ṹ��ĵ�ַ
*/
void CDfitcSecTraderHandler::OnStockTradeRtn(DFITCStockTradeRtnField * pData){
m_pUserCb->xOnStockTradeRtn(m_parent, pData);
}
/**
* STOCK-�����ر���Ӧ
* @param pData:���س����ر��ṹ��ĵ�ַ
*/
void CDfitcSecTraderHandler::OnStockWithdrawOrderRtn(DFITCStockWithdrawOrderRtnField * pData){
m_pUserCb->xOnStockWithdrawOrderRtn(m_parent, pData);
}

/**
* SOP-��¼��Ӧ
* @param pRspUserLogin:ָ�����ǿ�,�����û���¼��Ӧ��Ϣ�ṹ��ַ,������¼����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ��������¼����ʧ��
*/
void CDfitcSecTraderHandler::OnRspSOPUserLogin(DFITCSECRspUserLoginField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspSOPUserLogin(m_parent, pData, pRspInfo);
}
/**
 * SOP-�ǳ���Ӧ
 * @param pData:ָ�����ǿ�,�����û��ǳ���Ӧ��Ϣ�ṹ��ַ,�����ǳ�����ɹ�
 * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ǳ�����ʧ��
 */
void CDfitcSecTraderHandler::OnRspSOPUserLogout(DFITCSECRspUserLogoutField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspSOPUserLogout(m_parent, pData, pRspInfo);
}
/**
* SOP-�û����������Ӧ
* @param pData:ָ�����ǿ�,�����û����������Ӧ��Ϣ�ṹ��ַ,�����û������������ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������û������������ʧ��
*/
void CDfitcSecTraderHandler::OnRspSOPUserPasswordUpdate(DFITCSECRspPasswordUpdateField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspSOPUserPasswordUpdate(m_parent, pData, pRspInfo);
}
/**
* SOP-������Ӧ
* @param pData:ָ�����ǿ�,�����û�������Ӧ��Ϣ�ṹ��ַ,������������ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ��������������ʧ��
*/
void CDfitcSecTraderHandler::OnRspSOPEntrustOrder(DFITCSOPRspEntrustOrderField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspSOPEntrustOrder(m_parent, pData, pRspInfo);
}
/**
* SOP-��ϲ��ί����Ӧ
* @param pData:ָ�����ǿ�,�����û���Ӧ��Ϣ�ṹ��ַ,������ϲ��ί������ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ��������ϲ��ί������ʧ��
*/
void CDfitcSecTraderHandler::OnRspSOPGroupSplit(DFITCSOPRspEntrustOrderField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspSOPGroupSplit(m_parent, pData, pRspInfo);
}
/**
* SOP-��ѯ�ͻ���ϳֲ���ϸ��Ӧ
* @param pData:ָ�����ǿ�,�����û���ѯ�ͻ���ϳֲ���ϸ��Ӧ�ṹ��ַ,������ѯ�ͻ���ϳֲ���ϸ����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ��������ѯ�ͻ���ϳֲ���ϸ����ʧ��
* @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
*/
void CDfitcSecTraderHandler::OnRspSOPQryGroupPosition(DFITCSOPRspQryGroupPositionField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspSOPQryGroupPosition(m_parent, pData, pRspInfo, bIsLast);
}
/**
* SOP-֤ȯ����������Ӧ
* @param pData:ָ�����ǿ�,�����û�֤ȯ����������Ӧ��Ϣ�ṹ��ַ,����֤ȯ������������ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ������֤ȯ������������ʧ��
* @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
*/
void CDfitcSecTraderHandler::OnRspSOPLockOUnLockStock(DFITCSOPRspLockOUnLockStockField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspSOPLockOUnLockStock(m_parent, pData, pRspInfo, bIsLast);
}
/**
* SOP-������Ӧ
* @param pData:ָ�����ǿ�,�����û�������Ӧ��Ϣ�ṹ��ַ,������������ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ��������������ʧ��
*/
void CDfitcSecTraderHandler::OnRspSOPWithdrawOrder(DFITCSECRspWithdrawOrderField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspSOPWithdrawOrder(m_parent, pData, pRspInfo);
}
/**
* SOP-ί�в�ѯ��Ӧ
* @param pData:ָ�����ǿ�,�����û�ί�в�ѯ��Ӧ��Ϣ�ṹ��ַ,����ί�в�ѯ����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ������ί�в�ѯ����ʧ��
* @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
*/
void CDfitcSecTraderHandler::OnRspSOPQryEntrustOrder(DFITCSOPRspQryEntrustOrderField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspSOPQryEntrustOrder(m_parent, pData, pRspInfo, bIsLast);
}
/**
* SOP-�ֱʳɽ���ѯ��Ӧ
* @param pData:ָ�����ǿ�,�����û��ֱʳɽ���ѯ��Ӧ��Ϣ�ṹ��ַ,�����ֱʳɽ���ѯ����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ֱʳɽ���ѯ����ʧ��
* @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
*/
void CDfitcSecTraderHandler::OnRspSOPQrySerialTrade(DFITCSOPRspQrySerialTradeField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspSOPQrySerialTrade(m_parent, pData, pRspInfo, bIsLast);
}
/**
* SOP-�ֲֲ�ѯ��Ӧ
* @param pData:ָ�����ǿ�,�����û��ֲֲ�ѯ��Ӧ��Ϣ�ṹ��ַ,�����ֲֲ�ѯ����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ֲֲ�ѯ����ʧ��
* @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
*/
void CDfitcSecTraderHandler::OnRspSOPQryPosition(DFITCSOPRspQryPositionField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspSOPQryPosition(m_parent, pData, pRspInfo, bIsLast);
}
/**
* SOP-�ͻ������ֲֲ�ѯ��Ӧ
* @param pData:ָ�����ǿ�,�����û��ͻ������ֲֲ�ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ������ֲֲ�ѯ����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ������ֲֲ�ѯ����ʧ��
* @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
*/
void CDfitcSecTraderHandler::OnRspSOPQryCollateralPosition(DFITCSOPRspQryCollateralPositionField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspSOPQryCollateralPosition(m_parent, pData, pRspInfo, bIsLast);
}
/**
* SOP-�ͻ��ʽ��ѯ��Ӧ
* @param pData:ָ�����ǿ�,�����û��ͻ��ʽ��ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ͻ��ʽ��ѯ����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ͻ��ʽ��ѯ����ʧ��
*/
void CDfitcSecTraderHandler::OnRspSOPQryCapitalAccountInfo(DFITCSOPRspQryCapitalAccountField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspSOPQryCapitalAccountInfo(m_parent, pData, pRspInfo);
}
/**
* SOP-�ͻ���Ϣ��ѯ��Ӧ
* @param pData:ָ�����ǿ�,�����û��ͻ���Ϣ��ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ͻ���Ϣ��ѯ����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ͻ���Ϣ��ѯ����ʧ��
*/
void CDfitcSecTraderHandler::OnRspSOPQryAccountInfo(DFITCSOPRspQryAccountField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspSOPQryAccountInfo(m_parent, pData, pRspInfo);
}
/**
* SOP-�ɶ���Ϣ��ѯ��Ӧ
* @param pData:ָ�����ǿ�,�����û��ɶ���Ϣ��ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ɶ���Ϣ��ѯ����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ɶ���Ϣ��ѯ����ʧ��
*/
void CDfitcSecTraderHandler::OnRspSOPQryShareholderInfo(DFITCSOPRspQryShareholderField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspSOPQryShareholderInfo(m_parent, pData, pRspInfo);
}
/**
* SOP-��ί��������ѯ��Ӧ
* @param pData:ָ�����ǿ�,�����û���ί��������ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ���ί��������ѯ����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ���ί��������ѯ����ʧ��
*/
void CDfitcSecTraderHandler::OnRspSOPCalcAbleEntrustQty(DFITCSOPRspCalcAbleEntrustQtyField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspSOPCalcAbleEntrustQty(m_parent, pData, pRspInfo);
}
/**
* SOP-�ͻ�������֤ȯ��ѯ��Ӧ
* @param pData:ָ�����ǿ�,�����û��ͻ�������֤ȯ��ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ͻ�������֤ȯ��ѯ����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ͻ�������֤ȯ��ѯ����ʧ��
* @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
*/
void CDfitcSecTraderHandler::OnRspSOPQryAbleLockStock(DFITCSOPRspQryAbleLockStockField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspSOPQryAbleLockStock(m_parent, pData, pRspInfo, bIsLast);
}
/**
* SOP-��Ȩ��Լ�����ѯ��Ӧ
* @param pData:ָ�����ǿ�,�����û���Ȩ��Լ�����ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ���Ȩ��Լ�����ѯ����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ���Ȩ��Լ�����ѯ����ʧ��
* @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
*/
void CDfitcSecTraderHandler::OnRspSOPQryContactInfo(DFITCSOPRspQryContactField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspSOPQryContactInfo(m_parent, pData, pRspInfo, bIsLast);
}
/**
* SOP-ִ��ί����Ӧ
* @param pData:ָ�����ǿ�,�����û�ִ��ί����Ӧ��Ϣ�ṹ��ַ,�����ͻ�ִ��ί������ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ�ִ��ί������ʧ��
*/
void CDfitcSecTraderHandler::OnRspSOPExectueOrder(DFITCSOPRspExectueOrderField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspSOPExectueOrder(m_parent, pData, pRspInfo);
}
/**
* SOP-�ͻ���Ȩָ����Ϣ��ѯ��Ӧ
* @param pData:ָ�����ǿ�,�����û��ͻ���Ȩָ����Ϣ��ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ͻ���Ȩָ����Ϣ��ѯ����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ͻ���Ȩָ����Ϣ��ѯ����ʧ��
* @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
*/
void CDfitcSecTraderHandler::OnRspSOPQryExecAssiInfo(DFITCSOPRspQryExecAssiInfoField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspSOPQryExecAssiInfo(m_parent, pData, pRspInfo, bIsLast);
}
/**
* SOP-��ѯ����ʱ����Ӧ
* @param pData:ָ�����ǿ�,�����û���ѯ����ʱ����Ӧ��Ϣ�ṹ��ַ,�����ͻ���ѯ����ʱ������ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ���ѯ����ʱ������ʧ��
* @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
*/
void CDfitcSecTraderHandler::OnRspSOPQryTradeTime(DFITCSOPRspQryTradeTimeField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspSOPQryTradeTime(m_parent, pData, pRspInfo, bIsLast);
}
/**
* SOP-��ȡ���н�����������Ӧ
* @param pData:ָ�����ǿ�,�����û���ȡ���н�����������Ӧ��Ϣ�ṹ��ַ,�����ͻ���ȡ���н�������������ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ���ȡ���н�������������ʧ��
* @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
*/
void CDfitcSecTraderHandler::OnRspSOPQryExchangeInfo(DFITCSOPRspQryExchangeInfoField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspSOPQryExchangeInfo(m_parent, pData, pRspInfo, bIsLast);
}
/**
* SOP-��ѯ�����Ѳ�����Ӧ
* @param pData:ָ�����ǿ�,�����û���ѯ�����Ѳ�����Ӧ��Ϣ�ṹ��ַ,�����ͻ���ѯ�����Ѳ�������ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ���ѯ�����Ѳ�������ʧ��
* @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
*/
void CDfitcSecTraderHandler::OnRspSOPQryCommission(DFITCSOPRspQryCommissionField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspSOPQryCommission(m_parent, pData, pRspInfo, bIsLast);
}
/**
* SOP-��ѯ��֤���ʲ�����Ӧ
* @param pData:ָ�����ǿ�,�����û���ѯ��֤���ʲ�����Ӧ��Ϣ�ṹ��ַ,�����ͻ���ѯ��֤���ʲ�������ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ���ѯ��֤���ʲ�������ʧ��
* @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
*/
void CDfitcSecTraderHandler::OnRspSOPQryDeposit(DFITCSOPRspQryDepositField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspSOPQryDeposit(m_parent, pData, pRspInfo, bIsLast);
}
/**
* SOP-��Ȩ�����Ϣ��ѯ��Ӧ
* @param pData:ָ�����ǿ�,�����û���Ȩ�����Ϣ��ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ���Ȩ�����Ϣ��ѯ����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ���Ȩ�����Ϣ��ѯ����ʧ��
* @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
*/
void CDfitcSecTraderHandler::OnRspSOPQryContractObjectInfo(DFITCSOPRspQryContractObjectField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspSOPQryContractObjectInfo(m_parent, pData, pRspInfo, bIsLast);
}
/**
* SOP-ί�лر���Ӧ
* @param pData:����ί�лر��ṹ��ĵ�ַ
*/
void CDfitcSecTraderHandler::OnSOPEntrustOrderRtn(DFITCSOPEntrustOrderRtnField * pData){
m_pUserCb->xOnSOPEntrustOrderRtn(m_parent, pData);
}
/**
* SOP-�ɽ��ر���Ӧ
* @param pData:���سɽ��ر��ṹ��ĵ�ַ
*/
void CDfitcSecTraderHandler::OnSOPTradeRtn(DFITCSOPTradeRtnField * pData){
m_pUserCb->xOnSOPTradeRtn(m_parent, pData);
}
/**
* SOP-�����ر���Ӧ
* @param pData:���س����ر��ṹ��ĵ�ַ
*/
void CDfitcSecTraderHandler::OnSOPWithdrawOrderRtn(DFITCSOPWithdrawOrderRtnField * pData){
m_pUserCb->xOnSOPWithdrawOrderRtn(m_parent, pData);
}

/**
* FASL-��¼��Ӧ
* @param pData:ָ�����ǿ�,�����û�������ȯ��¼��Ӧ��Ϣ�ṹ��ַ,�����ͻ�������ȯ��¼����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ�������ȯ��¼����ʧ��
*/
void CDfitcSecTraderHandler::OnRspFASLUserLogin(DFITCSECRspUserLoginField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspFASLUserLogin(m_parent, pData, pRspInfo);
}
/**
* FASL-�ǳ���Ӧ
* @param pData:ָ�����ǿ�,�����û�������ȯ�ǳ���Ӧ��Ϣ�ṹ��ַ,�����ͻ�������ȯ�ǳ�����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ�������ȯ�ǳ�����ʧ��
*/
void CDfitcSecTraderHandler::OnRspFASLUserLogout(DFITCSECRspUserLogoutField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspFASLUserLogout(m_parent, pData, pRspInfo);
}
/**
* FASL-�ͻ���������Ϣ��Ӧ
* @param pData:ָ�����ǿ�,�����û��ͻ���������Ϣ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ͻ���������Ϣ����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ͻ���������Ϣ����ʧ��
*/
void CDfitcSecTraderHandler::OnRspFASLQryAbleFinInfo(DFITCFASLRspAbleFinInfoField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspFASLQryAbleFinInfo(m_parent, pData, pRspInfo);
}
/**
* FASL-�ͻ�����ȯ��Ϣ��Ӧ
* @param pData:ָ�����ǿ�,�����û��ͻ�����ȯ��Ϣ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ͻ�����ȯ��Ϣ����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ͻ�����ȯ��Ϣ����ʧ��
* @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
*/
void CDfitcSecTraderHandler::OnRspFASLQryAbleSloInfo(DFITCFASLRspAbleSloInfoField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspFASLQryAbleSloInfo(m_parent, pData, pRspInfo, bIsLast);
}
/**
* FASL-�����ﻮת��Ӧ
* @param pData:ָ�����ǿ�,�����û������ﻮת��Ӧ��Ϣ�ṹ��ַ,�����ͻ������ﻮת����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ������ﻮת����ʧ��
*/
void CDfitcSecTraderHandler::OnRspFASLTransferCollateral(DFITCFASLRspTransferCollateralField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspFASLTransferCollateral(m_parent, pData, pRspInfo);
}
/**
* FASL-ֱ�ӻ�����Ӧ
* @param pData:ָ�����ǿ�,�����û�ֱ�ӻ�����Ӧ��Ϣ�ṹ��ַ,�����ͻ�ֱ�ӻ�������ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ�ֱ�ӻ�������ʧ��
*/
void CDfitcSecTraderHandler::OnRspFASLDirectRepayment(DFITCFASLRspDirectRepaymentField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspFASLDirectRepayment(m_parent, pData, pRspInfo);
}
/**
* FASL-��ȯ��ת��Ӧ
* @param pData:ָ�����ǿ�,�����û���ȯ��ת��Ӧ��Ϣ�ṹ��ַ,�����ͻ���ȯ��ת����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ���ȯ��ת����ʧ��
*/
void CDfitcSecTraderHandler::OnRspFASLRepayStockTransfer(DFITCFASLRspRepayStockTransferField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspFASLRepayStockTransfer(m_parent, pData, pRspInfo);
}
/**
* FASL-���ý�����Ӧ
* @param pData:ָ�����ǿ�,�����û����ý�����Ӧ��Ϣ�ṹ��ַ,�����ͻ����ý�������ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ����ý�������ʧ��
*/
void CDfitcSecTraderHandler::OnRspFASLEntrustCrdtOrder(DFITCFASLRspEntrustCrdtOrderField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspFASLEntrustCrdtOrder(m_parent, pData, pRspInfo);
}
/**
* FASL-������ȯ������Ӧ
* @param pData:ָ�����ǿ�,�����û�������ȯ������Ӧ��Ϣ�ṹ��ַ,�����ͻ�������ȯ��������ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ�������ȯ��������ʧ��
*/
void CDfitcSecTraderHandler::OnRspFASLEntrustOrder(DFITCFASLRspEntrustOrderField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspFASLEntrustOrder(m_parent, pData, pRspInfo);
}
/**
* FASL-���ÿ�ί��������ѯ��Ӧ
* @param pData:ָ�����ǿ�,�����û����ÿ�ί��������ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ����ÿ�ί��������ѯ����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ����ÿ�ί��������ѯ����ʧ��
*/
void CDfitcSecTraderHandler::OnRspFASLCalcAbleEntrustCrdtQty(DFITCFASLRspCalcAbleEntrustCrdtQtyField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspFASLCalcAbleEntrustCrdtQty(m_parent, pData, pRspInfo);
}
/**
* FASL-��ѯ�����ʽ���Ӧ
* @param pData:ָ�����ǿ�,�����û���ѯ�����ʽ���Ӧ��Ϣ�ṹ��ַ,�����ͻ���ѯ�����ʽ�����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ���ѯ�����ʽ�����ʧ��
*/
void CDfitcSecTraderHandler::OnRspFASLQryCrdtFunds(DFITCFASLRspQryCrdtFundsField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspFASLQryCrdtFunds(m_parent, pData, pRspInfo);
}
/**
* FASL-���ú�Լ��Ϣ��Ӧ
* @param pData:ָ�����ǿ�,�����û����ú�Լ��Ϣ��Ӧ��Ϣ�ṹ��ַ,�����ͻ����ú�Լ��Ϣ����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ����ú�Լ��Ϣ����ʧ��
*/
void CDfitcSecTraderHandler::OnRspFASLQryCrdtContract(DFITCFASLRspQryCrdtContractField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspFASLQryCrdtContract(m_parent, pData, pRspInfo, bIsLast);
}
/**
* FASL-���ú�Լ�䶯��Ϣ��ѯ��Ӧ
* @param pData:ָ�����ǿ�,�����û����ú�Լ�䶯��Ϣ��ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ����ú�Լ�䶯��Ϣ��ѯ����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ����ú�Լ�䶯��Ϣ��ѯ����ʧ��
* @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
*/
void CDfitcSecTraderHandler::OnRspFASLQryCrdtConChangeInfo(DFITCFASLRspQryCrdtConChangeInfoField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspFASLQryCrdtConChangeInfo(m_parent, pData, pRspInfo, bIsLast);
}
/**
* FASL-�ʽ��ת��Ӧ
* @param pData:ָ�����ǿ�,�����û��ʽ��ת��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ʽ��ת����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ʽ��ת����ʧ��
*/
void CDfitcSecTraderHandler::OnRspFASLTransferFunds(DFITCStockRspTransferFundsField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspFASLTransferFunds(m_parent, pData, pRspInfo);
}
/**
* FASL-�ͻ���Ϣ��ѯ��Ӧ
* @param pData:ָ�����ǿ�,�����û��ͻ���Ϣ��ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ͻ���Ϣ��ѯ����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ͻ���Ϣ��ѯ����ʧ��
*/
void CDfitcSecTraderHandler::OnRspFASLQryAccountInfo(DFITCStockRspQryAccountField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspFASLQryAccountInfo(m_parent, pData, pRspInfo);
}
/**
* FASL-�ͻ��ʽ��ѯ��Ӧ
* @param pData:ָ�����ǿ�,�����û��ͻ��ʽ��ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ͻ��ʽ��ѯ����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ͻ��ʽ��ѯ����ʧ��
* @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
*/
void CDfitcSecTraderHandler::OnRspFASLQryCapitalAccountInfo(DFITCStockRspQryCapitalAccountField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspFASLQryCapitalAccountInfo(m_parent, pData, pRspInfo, bIsLast);
}
/**
* FASL-�ɶ���Ϣ��ѯ��Ӧ
* @param pData:ָ�����ǿ�,�����û��ɶ���Ϣ��ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ɶ���Ϣ��ѯ����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ɶ���Ϣ��ѯ����ʧ��
* @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
*/
void CDfitcSecTraderHandler::OnRspFASLQryShareholderInfo(DFITCStockRspQryShareholderField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspFASLQryShareholderInfo(m_parent, pData, pRspInfo, bIsLast);
}
/**
* FASL-�ֲֲ�ѯ��Ӧ
* @param pData:ָ�����ǿ�,�����û��ֲֲ�ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ֲֲ�ѯ����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ֲֲ�ѯ����ʧ��
* @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
*/
void CDfitcSecTraderHandler::OnRspFASLQryPosition(DFITCStockRspQryPositionField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspFASLQryPosition(m_parent, pData, pRspInfo, bIsLast);
}
/**
* FASL-ί�в�ѯ��Ӧ
* @param pData:ָ�����ǿ�,�����û�ί�в�ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ�ί�в�ѯ����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ�ί�в�ѯ����ʧ��
* @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
*/
void CDfitcSecTraderHandler::OnRspFASLQryEntrustOrder(DFITCStockRspQryEntrustOrderField * pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspFASLQryEntrustOrder(m_parent, pData, pRspInfo, bIsLast);
}
/**
* FASL-�ֱʳɽ���ѯ��Ӧ
* @param pData:ָ�����ǿ�,�����û��ֱʳɽ���ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ֱʳɽ���ѯ����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ֱʳɽ���ѯ����ʧ��
* @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
*/
void CDfitcSecTraderHandler::OnRspFASLQrySerialTrade(DFITCStockRspQrySerialTradeField * pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspFASLQrySerialTrade(m_parent, pData, pRspInfo, bIsLast);
}
/**
* FASL-ʵʱ�ɽ���ѯ��Ӧ
* @param pData:ָ�����ǿ�,�����û�ʵʱ�ɽ���ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ�ʵʱ�ɽ���ѯ����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ�ʵʱ�ɽ���ѯ����ʧ��
* @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
*/
void CDfitcSecTraderHandler::OnRspFASLQryRealTimeTrade(DFITCStockRspQryRealTimeTradeField * pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspFASLQryRealTimeTrade(m_parent, pData, pRspInfo, bIsLast);
}
/**
* FASL-�ʽ𶳽���ϸ��ѯ��Ӧ
* @param pData:ָ�����ǿ�,�����û��ʽ𶳽���ϸ��ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ʽ𶳽���ϸ��ѯ����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ʽ𶳽���ϸ��ѯ����ʧ��
* @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
*/
void CDfitcSecTraderHandler::OnRspFASLQryFreezeFundsDetail(DFITCStockRspQryFreezeFundsDetailField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspFASLQryFreezeFundsDetail(m_parent, pData, pRspInfo, bIsLast);
}
/**
* FASL-֤ȯ������ϸ��ѯ��Ӧ
* @param pData:ָ�����ǿ�,�����û�֤ȯ������ϸ��ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ�֤ȯ������ϸ��ѯ����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ�֤ȯ������ϸ��ѯ����ʧ��
* @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
*/
void CDfitcSecTraderHandler::OnRspFASLQryFreezeStockDetail(DFITCStockRspQryFreezeStockDetailField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspFASLQryFreezeStockDetail(m_parent, pData, pRspInfo, bIsLast);
}
/**
* FASL-�ʽ������ϸ��ѯ��Ӧ
* @param pData:ָ�����ǿ�,�����û��ʽ������ϸ��ѯ��Ӧ��Ϣ�ṹ��ַ,�����ͻ��ʽ������ϸ��ѯ����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ��ʽ������ϸ��ѯ����ʧ��
* @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
*/
void CDfitcSecTraderHandler::OnRspFASLQryTransferFundsDetail(DFITCStockRspQryTransferFundsDetailField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspFASLQryTransferFundsDetail(m_parent, pData, pRspInfo, bIsLast);
}
/**
* FASL-������Ӧ
* @param pData:ָ�����ǿ�,�����û�������Ӧ��Ϣ�ṹ��ַ,������������ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ��������������ʧ��
*/
void CDfitcSecTraderHandler::OnRspFASLWithdrawOrder(DFITCFASLRspWithdrawOrderField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspFASLWithdrawOrder(m_parent, pData, pRspInfo);
}
/**
* FASL-��ǰϵͳʱ���ѯ������Ӧ
* @param pData:ָ�����ǿ�,�����û�ϵͳʱ���ѯ��Ӧ��Ϣ�ṹ��ַ,����ϵͳʱ���ѯ����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ������ϵͳʱ���ѯ����ʧ��
*/
void CDfitcSecTraderHandler::OnRspFASLQrySystemTime(DFITCFASLRspQryTradeTimeField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspFASLQrySystemTime(m_parent, pData, pRspInfo);
}
/**
* FASL-��ת�뵣��֤ȯ��ѯ������Ӧ
* @param pData:ָ�����ǿ�,���ؿ�ת�뵣��֤ȯ��ѯ��Ӧ��Ϣ�ṹ��ַ,������ת�뵣��֤ȯ��ѯ����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ��������ת�뵣��֤ȯ��ѯ����ʧ��
* @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
*/
void CDfitcSecTraderHandler::OnRspFASLQryTransferredContract(DFITCFASLRspQryTransferredContractField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspFASLQryTransferredContract(m_parent, pData, pRspInfo, bIsLast);
}
/**
* FASL-�ͻ���ȡ�ʽ����������Ӧ
* @param pData:ָ�����ǿ�,���ؿͻ���ȡ�ʽ������Ӧ��Ϣ�ṹ��ַ,�����ͻ���ȡ�ʽ��������ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ͻ���ȡ�ʽ��������ʧ��
*/
void CDfitcSecTraderHandler::OnRspFASLDesirableFundsOut(DFITCFASLRspDesirableFundsOutField *pData, DFITCSECRspInfoField *pRspInfo) {
m_pUserCb->xOnRspFASLDesirableFundsOut(m_parent, pData, pRspInfo);
}
/**
* FASL-����֤ȯ��ѯ������Ӧ
* @param pData:ָ�����ǿ�,���ص���֤ȯ��ѯ��Ӧ��Ϣ�ṹ��ַ,��������֤ȯ��ѯ����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ����������֤ȯ��ѯ����ʧ��
* @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
*/
void CDfitcSecTraderHandler::OnRspFASLQryGuaranteedContract(DFITCFASLRspQryGuaranteedContractField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspFASLQryGuaranteedContract(m_parent, pData, pRspInfo, bIsLast);
}
/**
* FASL-���֤ȯ��ѯ������Ӧ
* @param pData:ָ�����ǿ�,���ر��֤ȯ��ѯ��Ӧ��Ϣ�ṹ��ַ,�������֤ȯ��ѯ����ɹ�
* @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ���������֤ȯ��ѯʧ��
* @param bIsLast:����ֵ�����Ƿ������һ����Ӧ��Ϣ(0-��,1-��)
*/
void CDfitcSecTraderHandler::OnRspFASLQryUnderlyingContract(DFITCFASLRspQryUnderlyingContractField *pData, DFITCSECRspInfoField *pRspInfo, bool bIsLast) {
m_pUserCb->xOnRspFASLQryUnderlyingContract(m_parent, pData, pRspInfo, bIsLast);
}
/**
* FASL-ί�лر���Ӧ
* @param pData:����ί�лر��ṹ��ĵ�ַ
*/
void CDfitcSecTraderHandler::OnFASLEntrustOrderRtn(DFITCStockEntrustOrderRtnField *pData){
m_pUserCb->xOnFASLEntrustOrderRtn(m_parent, pData);
}
/**
* FASL-�ɽ��ر���Ӧ
* @param pData:���سɽ��ر��ṹ��ĵ�ַ
*/
void CDfitcSecTraderHandler::OnFASLTradeRtn(DFITCStockTradeRtnField *pData){
m_pUserCb->xOnFASLTradeRtn(m_parent, pData);
}
/**
* FASL-�����ر���Ӧ
* @param pData:���س����ر��ṹ��ĵ�ַ
*/
void CDfitcSecTraderHandler::OnFASLWithdrawOrderRtn(DFITCStockWithdrawOrderRtnField *pData){
m_pUserCb->xOnFASLWithdrawOrderRtn(m_parent, pData);
}


