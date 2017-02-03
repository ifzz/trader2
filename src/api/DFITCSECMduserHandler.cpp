
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "DFITCSECApiDataType.h"
#include "DFITCSECApiStruct.h"
#include "DFITCSECMdApi.h"

#include "DFITCSECMduserHandler.h"

extern char *INI_FILE_NAME;

CDfitcSecMduserHandler::CDfitcSecMduserHandler(femas_mduser_api_cb *pUserCb, void * parent)
	: m_pUserCb(pUserCb) , m_parent(parent)
{

}

CDfitcSecMduserHandler::~CDfitcSecMduserHandler() 
{

}

// ���ͻ��������鷢��������������ͨ�����ӣ��ͻ�����Ҫ���е�¼
void CDfitcSecMduserHandler::OnFrontConnected()
{
	m_pUserCb->xOnFrontConnected(m_parent);
}

// ���ͻ��������鷢��������ͨ�����ӶϿ�ʱ���÷���������
void CDfitcSecMduserHandler::OnFrontDisconnected(int nReason) 
{
	// ��������������API���Զ��������ӣ��ͻ��˿ɲ�������
	m_pUserCb->xOnFrontDisconnected(m_parent, nReason);
}

// ���ͻ��˷�����¼����֮�󣬸÷����ᱻ���ã�֪ͨ�ͻ��˵�¼�Ƿ�ɹ�
void CDfitcSecMduserHandler::OnRspSOPUserLogin(struct DFITCSECRspUserLoginField * pRspUserLogin, struct DFITCSECRspInfoField * pRspInfo) 
{
	m_pUserCb->xOnRspUserLogin(m_parent, pRspUserLogin, pRspInfo);
}

///�û��˳�Ӧ��
void CDfitcSecMduserHandler::OnRspSOPUserLogout(struct DFITCSECRspUserLogoutField * pRspUsrLogout, struct DFITCSECRspInfoField * pRspInfo) 
{
	m_pUserCb->xOnRspUserLogout(m_parent, pRspUsrLogout, pRspInfo);
}

// �������֪ͨ�����������������֪ͨ�ͻ���
void CDfitcSecMduserHandler::OnSOPMarketData(struct DFITCSOPDepthMarketDataField * pMarketDataField) 
{
	m_pUserCb->xOnRtnDepthMarketData(m_parent, pMarketDataField);
}

// ����û�����ĳ���֪ͨ
void CDfitcSecMduserHandler::OnRspError(struct DFITCSECRspInfoField *pRspInfo)
{
	m_pUserCb->xOnRspError(m_parent, pRspInfo);
}

///���ĺ�Լ�������Ϣ
void CDfitcSecMduserHandler::OnRspSOPSubMarketData(struct DFITCSECSpecificInstrumentField * pSpecificInstrument, struct DFITCSECRspInfoField * pRspInfo)
{
	m_pUserCb->xOnRspSubMarketData(m_parent, pSpecificInstrument, pRspInfo);
}

///���ĺ�Լ�������Ϣ
void CDfitcSecMduserHandler::OnRspSOPUnSubMarketData(struct DFITCSECSpecificInstrumentField * pSpecificInstrument, struct DFITCSECRspInfoField * pRspInfo)
{
	m_pUserCb->xOnRspSubMarketData(m_parent, pSpecificInstrument, pRspInfo);
}

