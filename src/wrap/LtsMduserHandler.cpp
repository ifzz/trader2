
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SecurityFtdcUserApiDataType.h"
#include "SecurityFtdcUserApiStruct.h"
#include "SecurityFtdcMdApi.h"

#include "LtsMduserHandler.h"

CLtsMduserHandler::CLtsMduserHandler(lts_mduser_api_cb *pUserCb, void * parent)
	: m_pUserCb(pUserCb) , m_parent(parent)
{

}

CLtsMduserHandler::~CLtsMduserHandler() 
{

}

// ���ͻ��������鷢��������������ͨ�����ӣ��ͻ�����Ҫ���е�¼
void CLtsMduserHandler::OnFrontConnected()
{
	m_pUserCb->xOnFrontConnected(m_parent);
}

// ���ͻ��������鷢��������ͨ�����ӶϿ�ʱ���÷���������
void CLtsMduserHandler::OnFrontDisconnected(int nReason) 
{
	// ��������������API���Զ��������ӣ��ͻ��˿ɲ�������
	m_pUserCb->xOnFrontDisconnected(m_parent, nReason);
}

// ���ͻ��˷�����¼����֮�󣬸÷����ᱻ���ã�֪ͨ�ͻ��˵�¼�Ƿ�ɹ�
void CLtsMduserHandler::OnRspUserLogin(CSecurityFtdcRspUserLoginField *pRspUserLogin, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	m_pUserCb->xOnRspUserLogin(m_parent, pRspUserLogin, pRspInfo, nRequestID, bIsLast);
}

///�û��˳�Ӧ��
void CLtsMduserHandler::OnRspUserLogout(CSecurityFtdcUserLogoutField *pRspUserLogout, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	m_pUserCb->xOnRspUserLogout(m_parent, pRspUserLogout, pRspInfo, nRequestID, bIsLast);
}

// �������֪ͨ�����������������֪ͨ�ͻ���
void CLtsMduserHandler::OnRtnDepthMarketData(CSecurityFtdcDepthMarketDataField *pMarketData) 
{
	m_pUserCb->xOnRtnDepthMarketData(m_parent, pMarketData);
}

// ����û�����ĳ���֪ͨ
void CLtsMduserHandler::OnRspError(CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	m_pUserCb->xOnRspError(m_parent, pRspInfo, nRequestID, bIsLast);
}

///���ĺ�Լ�������Ϣ
void CLtsMduserHandler::OnRspSubMarketData(CSecurityFtdcSpecificInstrumentField *pSpecificInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	m_pUserCb->xOnRspSubMarketData(m_parent, pSpecificInstrument, pRspInfo, nRequestID, bIsLast);
}

///���ĺ�Լ�������Ϣ
void CLtsMduserHandler::OnRspUnSubMarketData(CSecurityFtdcSpecificInstrumentField *pSpecificInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	m_pUserCb->xOnRspSubMarketData(m_parent, pSpecificInstrument, pRspInfo, nRequestID, bIsLast);
}


