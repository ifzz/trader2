#ifndef _LTS_MDUSER_HANDLER_H_
#define _LTS_MDUSER_HANDLER_H_

#include "lts/SecurityFtdcUserApiStruct.h"
#include "lts/SecurityFtdcMdApi.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct lts_mduser_api_cb_def lts_mduser_api_cb;

struct lts_mduser_api_cb_def{
  void (*xOnFrontConnected)(void* arg);
  void (*xOnFrontDisconnected)(void* arg, int nReason);
  void (*xOnRspUserLogin)(void* arg, CSecurityFtdcRspUserLoginField *pRspUserLogin, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
  void (*xOnRspUserLogout)(void* arg, CSecurityFtdcUserLogoutField *pRspUserLogout, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
  void (*xOnRtnDepthMarketData)(void* arg, CSecurityFtdcDepthMarketDataField *pMarketData);
  void (*xOnRspError)(void* arg, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
  void (*xOnRspSubMarketData)(void* arg, CSecurityFtdcSpecificInstrumentField *pSpecificInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
  void (*xOnRspUnSubMarketData)(void* arg, CSecurityFtdcSpecificInstrumentField *pSpecificInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, int bIsLast);
};
#ifdef __cplusplus
}
#endif

class CLtsMduserHandler : public CSecurityFtdcMdSpi
{
  public:
  CLtsMduserHandler(lts_mduser_api_cb *pUserCb, void * parent);
  ~CLtsMduserHandler();
  ///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
  void OnFrontConnected();

  ///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
  ///@param nReason 错误原因
  ///        0x1001 网络读失败
  ///        0x1002 网络写失败
  ///        0x2001 接收心跳超时
  ///        0x2002 发送心跳失败
  ///        0x2003 收到错误报文
  void OnFrontDisconnected(int nReason);
  	
  ///心跳超时警告。当长时间未收到报文时，该方法被调用。
  ///@param nTimeLapse 距离上次接收报文的时间
  virtual void OnHeartBeatWarning(int nTimeLapse){};


  ///登录请求响应
  void OnRspUserLogin(CSecurityFtdcRspUserLoginField *pRspUserLogin, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///登出请求响应
  void OnRspUserLogout(CSecurityFtdcUserLogoutField *pUserLogout, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///错误应答
  void OnRspError(CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

  ///订阅行情应答
  void OnRspSubMarketData(CSecurityFtdcSpecificInstrumentField *pSpecificInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

  ///取消订阅行情应答
  void OnRspUnSubMarketData(CSecurityFtdcSpecificInstrumentField *pSpecificInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///深度行情通知
  void OnRtnDepthMarketData(CSecurityFtdcDepthMarketDataField *pDepthMarketData) ;

  private:
    void* m_parent;
    lts_mduser_api_cb* m_pUserCb;
    
};

#endif //_LTS_MDUSER_HANDLER_H_

