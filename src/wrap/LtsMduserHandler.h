#ifndef _LTS_MDUSER_HANDLER_H_
#define _LTS_MDUSER_HANDLER_H_

#include "SecurityFtdcUserApiDataType.h"
#include "SecurityFtdcUserApiStruct.h"
#include "SecurityFtdcMdApi.h"

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
  ///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
  void OnFrontConnected();

  ///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
  ///@param nReason ����ԭ��
  ///        0x1001 �����ʧ��
  ///        0x1002 ����дʧ��
  ///        0x2001 ����������ʱ
  ///        0x2002 ��������ʧ��
  ///        0x2003 �յ�������
  void OnFrontDisconnected(int nReason);
  	
  ///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
  ///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
  virtual void OnHeartBeatWarning(int nTimeLapse){};


  ///��¼������Ӧ
  void OnRspUserLogin(CSecurityFtdcRspUserLoginField *pRspUserLogin, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///�ǳ�������Ӧ
  void OnRspUserLogout(CSecurityFtdcUserLogoutField *pUserLogout, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///����Ӧ��
  void OnRspError(CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

  ///��������Ӧ��
  void OnRspSubMarketData(CSecurityFtdcSpecificInstrumentField *pSpecificInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;

  ///ȡ����������Ӧ��
  void OnRspUnSubMarketData(CSecurityFtdcSpecificInstrumentField *pSpecificInstrument, CSecurityFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

  ///�������֪ͨ
  void OnRtnDepthMarketData(CSecurityFtdcDepthMarketDataField *pDepthMarketData) ;

  private:
    void* m_parent;
    lts_mduser_api_cb* m_pUserCb;
    
};

#endif //_LTS_MDUSER_HANDLER_H_

