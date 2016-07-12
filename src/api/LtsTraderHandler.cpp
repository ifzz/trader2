
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SecurityFtdcUserApiStruct.h"
#include "SecurityFtdcTraderApi.h"
#include "SecurityFtdcQueryApi.h"

#include "LtsTraderHandler.h"


CLtsTraderHandler::CLtsTraderHandler(lts_trader_api_cb* pUserCb, void* parent)
	: m_pUserCb(pUserCb) , m_parent(parent)
{
}

CLtsTraderHandler::~CLtsTraderHandler()
{
}

void CLtsTraderHandler::OnFrontConnected()
{
	m_pUserCb->xOnFrontConnected(m_parent);
}

void CLtsTraderHandler::OnFrontDisconnected(int nReason)
{
	m_pUserCb->xOnFrontDisconnected(m_parent, nReason);
}

void CLtsTraderHandler::OnRspUserLogin(CSecurityFtdcRspUserLoginField *pRspUserLogin, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	m_pUserCb->xOnRspUserLogin(m_parent, pRspUserLogin, pRspInfo, nRequestID, bIsLast);
}

void CLtsTraderHandler::OnRspUserLogout(CSecurityFtdcUserLogoutField *pUserLogout, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	m_pUserCb->xOnRspUserLogout(m_parent, pUserLogout, pRspInfo, nRequestID, bIsLast);
}

void CLtsTraderHandler::OnRspFetchAuthRandCode(CSecurityFtdcAuthRandCodeField *pAuthRandCode, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	m_pUserCb->xOnRspFetchAuthRandCode(m_parent, pAuthRandCode, pRspInfo, nRequestID, bIsLast);
}

void CLtsTraderHandler::OnRspError(CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	m_pUserCb->xOnRspError(m_parent, pRspInfo, nRequestID, bIsLast);
}

void CLtsTraderHandler::OnRspOrderInsert(CSecurityFtdcInputOrderField *pInputOrder, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	m_pUserCb->xOnRspOrderInsert(m_parent, pInputOrder, pRspInfo, nRequestID, bIsLast);
}

void CLtsTraderHandler::OnRspOrderAction(CSecurityFtdcInputOrderActionField *pInputOrderAction, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	m_pUserCb->xOnRspOrderAction(m_parent, pInputOrderAction, pRspInfo, nRequestID, bIsLast);
}

void CLtsTraderHandler::OnRtnOrder(CSecurityFtdcOrderField *pOrder) 
{
	m_pUserCb->xOnRtnOrder(m_parent, pOrder);
}

void CLtsTraderHandler::OnRtnTrade(CSecurityFtdcTradeField *pTrade) 
{
	m_pUserCb->xOnRtnTrade(m_parent, pTrade);
}

void CLtsTraderHandler::OnErrRtnOrderInsert(CSecurityFtdcInputOrderField *pInputOrder, CSecurityFtdcRspInfoField *pRspInfo) 
{
	m_pUserCb->xOnErrRtnOrderInsert(m_parent, pInputOrder, pRspInfo);
}

void CLtsTraderHandler::OnErrRtnOrderAction(CSecurityFtdcOrderActionField *pOrderAction, CSecurityFtdcRspInfoField *pRspInfo) 
{
	m_pUserCb->xOnErrRtnOrderAction(m_parent, pOrderAction, pRspInfo);
}

CLtsQueryHandler::CLtsQueryHandler(lts_query_api_cb* pUserCb, void* parent)
	: m_pUserCb(pUserCb) , m_parent(parent)
{
}

CLtsQueryHandler::~CLtsQueryHandler()
{
}

void CLtsQueryHandler::OnFrontConnected()
{
	m_pUserCb->xOnFrontConnected(m_parent);
}

void CLtsQueryHandler::OnFrontDisconnected(int nReason)
{
	m_pUserCb->xOnFrontDisconnected(m_parent, nReason);
}

void CLtsQueryHandler::OnRspUserLogin(CSecurityFtdcRspUserLoginField *pRspUserLogin, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	m_pUserCb->xOnRspUserLogin(m_parent, pRspUserLogin, pRspInfo, nRequestID, bIsLast);
}

void CLtsQueryHandler::OnRspUserLogout(CSecurityFtdcUserLogoutField *pUserLogout, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	m_pUserCb->xOnRspUserLogout(m_parent, pUserLogout, pRspInfo, nRequestID, bIsLast);
}

void CLtsQueryHandler::OnRspFetchAuthRandCode(CSecurityFtdcAuthRandCodeField *pAuthRandCode, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	m_pUserCb->xOnRspFetchAuthRandCode(m_parent, pAuthRandCode, pRspInfo, nRequestID, bIsLast);
}

void CLtsQueryHandler::OnRspError(CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	m_pUserCb->xOnRspError(m_parent, pRspInfo, nRequestID, bIsLast);
}

void CLtsQueryHandler::OnRspQryInvestor(CSecurityFtdcInvestorField *pInvestor, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	m_pUserCb->xOnRspQryInvestor(m_parent, pInvestor, pRspInfo, nRequestID, bIsLast);
}

void CLtsQueryHandler::OnRspQryInstrument(CSecurityFtdcInstrumentField *pInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	m_pUserCb->xOnRspQryInstrument(m_parent, pInstrument, pRspInfo, nRequestID, bIsLast);
}

void CLtsQueryHandler::OnRspQryTradingAccount(CSecurityFtdcTradingAccountField *pTradingAccount, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	m_pUserCb->xOnRspQryTradingAccount(m_parent, pTradingAccount, pRspInfo, nRequestID, bIsLast);
}

void CLtsQueryHandler::OnRspQryInvestorPosition(CSecurityFtdcInvestorPositionField *pInvestorPosition, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	m_pUserCb->xOnRspQryInvestorPosition(m_parent, pInvestorPosition, pRspInfo, nRequestID, bIsLast);
}

