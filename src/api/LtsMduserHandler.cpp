
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

// 当客户端与行情发布服务器建立起通信连接，客户端需要进行登录
void CLtsMduserHandler::OnFrontConnected()
{
	m_pUserCb->xOnFrontConnected(m_parent);
}

// 当客户端与行情发布服务器通信连接断开时，该方法被调用
void CLtsMduserHandler::OnFrontDisconnected(int nReason) 
{
	// 当发生这个情况后，API会自动重新连接，客户端可不做处理
	m_pUserCb->xOnFrontDisconnected(m_parent, nReason);
}

// 当客户端发出登录请求之后，该方法会被调用，通知客户端登录是否成功
void CLtsMduserHandler::OnRspUserLogin(CSecurityFtdcRspUserLoginField *pRspUserLogin, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	m_pUserCb->xOnRspUserLogin(m_parent, pRspUserLogin, pRspInfo, nRequestID, bIsLast);
}

///用户退出应答
void CLtsMduserHandler::OnRspUserLogout(CSecurityFtdcUserLogoutField *pRspUserLogout, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) 
{
	m_pUserCb->xOnRspUserLogout(m_parent, pRspUserLogout, pRspInfo, nRequestID, bIsLast);
}

// 深度行情通知，行情服务器会主动通知客户端
void CLtsMduserHandler::OnRtnDepthMarketData(CSecurityFtdcDepthMarketDataField *pMarketData) 
{
	m_pUserCb->xOnRtnDepthMarketData(m_parent, pMarketData);
}

// 针对用户请求的出错通知
void CLtsMduserHandler::OnRspError(CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	m_pUserCb->xOnRspError(m_parent, pRspInfo, nRequestID, bIsLast);
}

///订阅合约的相关信息
void CLtsMduserHandler::OnRspSubMarketData(CSecurityFtdcSpecificInstrumentField *pSpecificInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	m_pUserCb->xOnRspSubMarketData(m_parent, pSpecificInstrument, pRspInfo, nRequestID, bIsLast);
}

///订阅合约的相关信息
void CLtsMduserHandler::OnRspUnSubMarketData(CSecurityFtdcSpecificInstrumentField *pSpecificInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	m_pUserCb->xOnRspSubMarketData(m_parent, pSpecificInstrument, pRspInfo, nRequestID, bIsLast);
}


