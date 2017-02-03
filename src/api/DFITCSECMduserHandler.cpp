
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

// 当客户端与行情发布服务器建立起通信连接，客户端需要进行登录
void CDfitcSecMduserHandler::OnFrontConnected()
{
	m_pUserCb->xOnFrontConnected(m_parent);
}

// 当客户端与行情发布服务器通信连接断开时，该方法被调用
void CDfitcSecMduserHandler::OnFrontDisconnected(int nReason) 
{
	// 当发生这个情况后，API会自动重新连接，客户端可不做处理
	m_pUserCb->xOnFrontDisconnected(m_parent, nReason);
}

// 当客户端发出登录请求之后，该方法会被调用，通知客户端登录是否成功
void CDfitcSecMduserHandler::OnRspSOPUserLogin(struct DFITCSECRspUserLoginField * pRspUserLogin, struct DFITCSECRspInfoField * pRspInfo) 
{
	m_pUserCb->xOnRspUserLogin(m_parent, pRspUserLogin, pRspInfo);
}

///用户退出应答
void CDfitcSecMduserHandler::OnRspSOPUserLogout(struct DFITCSECRspUserLogoutField * pRspUsrLogout, struct DFITCSECRspInfoField * pRspInfo) 
{
	m_pUserCb->xOnRspUserLogout(m_parent, pRspUsrLogout, pRspInfo);
}

// 深度行情通知，行情服务器会主动通知客户端
void CDfitcSecMduserHandler::OnSOPMarketData(struct DFITCSOPDepthMarketDataField * pMarketDataField) 
{
	m_pUserCb->xOnRtnDepthMarketData(m_parent, pMarketDataField);
}

// 针对用户请求的出错通知
void CDfitcSecMduserHandler::OnRspError(struct DFITCSECRspInfoField *pRspInfo)
{
	m_pUserCb->xOnRspError(m_parent, pRspInfo);
}

///订阅合约的相关信息
void CDfitcSecMduserHandler::OnRspSOPSubMarketData(struct DFITCSECSpecificInstrumentField * pSpecificInstrument, struct DFITCSECRspInfoField * pRspInfo)
{
	m_pUserCb->xOnRspSubMarketData(m_parent, pSpecificInstrument, pRspInfo);
}

///订阅合约的相关信息
void CDfitcSecMduserHandler::OnRspSOPUnSubMarketData(struct DFITCSECSpecificInstrumentField * pSpecificInstrument, struct DFITCSECRspInfoField * pRspInfo)
{
	m_pUserCb->xOnRspSubMarketData(m_parent, pSpecificInstrument, pRspInfo);
}

