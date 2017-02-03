#ifndef _DFITC_SEC_MDUSER_HANDLER_H_
#define _DFITC_SEC_MDUSER_HANDLER_H_
#include "DFITCSECApiDataType.h"
#include "DFITCSECApiStruct.h"
#include "DFITCSECMdApi.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct dfitc_sec_mduser_api_cb_def dfitc_sec_mduser_api_cb;

struct dfitc_sec_mduser_api_cb_def{
  void (*xOnFrontConnected)(void* arg);
  void (*xOnFrontDisconnected)(void* arg, int nReason);
  void (*xOnRspUserLogin)(void* arg, DFITCSECRspUserLoginField * pRspUserLogin, DFITCSECRspInfoField * pRspInfo);
  void (*xOnRspUserLogout)(void* arg, DFITCSECRspUserLogoutField * pRspUsrLogout, DFITCSECRspInfoField * pRspInfo);
  void (*xOnRtnDepthMarketData)(void* arg, DFITCSOPDepthMarketDataField * pMarketDataField);
  void (*xOnRspError)(void* arg, DFITCSECRspInfoField *pRspInfo);
  void (*xOnRspSubMarketData)(void* arg, DFITCSECSpecificInstrumentField * pSpecificInstrument, DFITCSECRspInfoField * pRspInfo);
  void (*xOnRspUnSubMarketData)(void* arg, DFITCSECSpecificInstrumentField * pSpecificInstrument, DFITCSECRspInfoField * pRspInfo);
};
#ifdef __cplusplus
}
#endif

class CDfitcSecMduserHandler : public DFITCSECMdSpi
{
public:
  CDfitcSecMduserHandler(dfitc_sec_mduser_api_cb *pUserCb, void * parent);
  ~CDfitcSecMduserHandler();

  /** 
  * ��������������Ӧ
  */
  void OnFrontConnected();

  /**
  * �������Ӳ�������Ӧ
  */
  void OnFrontDisconnected(int nReason);

  /**
  * SEC-��Ϣ֪ͨ
  */
  virtual void OnRtnNotice(DFITCSECRspNoticeField *pNotice) {};

  /**
  * ERR-����Ӧ��
  * @param pRspInfo:������ʧ�ܣ����ش�����Ϣ��ַ
  */
  void OnRspError(DFITCSECRspInfoField *pRspInfo);

  /**
  * STOCK-��¼��Ӧ
  * @param pRspUserLogin:ָ�����ǿ�,�����û���¼��Ӧ��Ϣ�ṹ��ַ,������¼����ɹ�
  * @param pRspInfo::ָ�����ǿգ����ش�����Ϣ��ַ��������¼����ʧ��
  */
  virtual void OnRspStockUserLogin(DFITCSECRspUserLoginField * pRspUserLogin, DFITCSECRspInfoField * pRspInfo) {}

  /**
  * STOCK-�ǳ���Ӧ
  * @param pRspUsrLogout:ָ�����ǿ�,�����û��ǳ���Ӧ��Ϣ�ṹ��ַ,�����ǳ�����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ǳ�����ʧ��
  */
  virtual void OnRspStockUserLogout(DFITCSECRspUserLogoutField * pRspUsrLogout, DFITCSECRspInfoField * pRspInfo) {}

  /**
  * SOP-��¼��Ӧ
  * @param pRspUserLogin:ָ�����ǿ�,�����û���¼��Ӧ��Ϣ�ṹ��ַ,������¼����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ��������¼����ʧ��
  */
  void OnRspSOPUserLogin(DFITCSECRspUserLoginField * pRspUserLogin, DFITCSECRspInfoField * pRspInfo);

  /**
  * SOP-�ǳ���Ӧ
  * @param pRspUsrLogout:ָ�����ǿ�,�����û��ǳ���Ӧ��Ϣ�ṹ��ַ,�����ǳ�����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ǳ�����ʧ��
  */
  void OnRspSOPUserLogout(DFITCSECRspUserLogoutField * pRspUsrLogout, DFITCSECRspInfoField * pRspInfo);

  /**
  * FASL-��¼��Ӧ
  * @param pRspUserLogin:ָ�����ǿ�,�����û���¼��Ӧ��Ϣ�ṹ��ַ,������¼����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ��������¼����ʧ��
  */
  virtual void OnRspFASLUserLogin(DFITCSECRspUserLoginField * pRspUserLogin, DFITCSECRspInfoField * pRspInfo) {}

  /**
  * FASL-�ǳ���Ӧ
  * @param pRspUsrLogout:ָ�����ǿ�,�����û��ǳ���Ӧ��Ϣ�ṹ��ַ,�����ǳ�����ɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�������ǳ�����ʧ��
  */
  virtual void OnRspFASLUserLogout(DFITCSECRspUserLogoutField * pRspUsrLogout, DFITCSECRspInfoField * pRspInfo) {}

  /**
  * STOCK-���鶩����Ӧ
  * @param pSpecificInstrument:ָ�����ǿ�,�����û�ָ����Լ���鶩����Ӧ�ṹ��ַ,����ָ����Լ���鶩�ĳɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ������ָ����Լ���鶩��ʧ��
  */
  virtual void OnRspStockSubMarketData(DFITCSECSpecificInstrumentField * pSpecificInstrument, DFITCSECRspInfoField * pRspInfo) {}

  /**
  * STOCK-ȡ������������Ӧ
  * @param pSpecificInstrument:ָ�����ǿ�,�����û�ָ����Լ����ȡ��������Ӧ�ṹ��ַ,����ָ����Լ����ȡ�����ĳɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ������ָ����Լ����ȡ������ʧ��
  */
  virtual void OnRspStockUnSubMarketData(DFITCSECSpecificInstrumentField * pSpecificInstrument, DFITCSECRspInfoField * pRspInfo) {}

  /**
  * SOP-���鶩����Ӧ
  * @param pSpecificInstrument:ָ�����ǿ�,�����û�ָ����Լ���鶩����Ӧ�ṹ��ַ,����ָ����Լ���鶩�ĳɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ������ָ����Լ���鶩��ʧ��
  */
  void OnRspSOPSubMarketData(DFITCSECSpecificInstrumentField * pSpecificInstrument, DFITCSECRspInfoField * pRspInfo);

  /**
  * SOP-ȡ������������Ӧ
  * @param pSpecificInstrument:ָ�����ǿ�,�����û�ָ����Լ����ȡ��������Ӧ�ṹ��ַ,����ָ����Լ����ȡ�����ĳɹ�
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ������ָ����Լ����ȡ������ʧ��
  */
  void OnRspSOPUnSubMarketData(DFITCSECSpecificInstrumentField * pSpecificInstrument, DFITCSECRspInfoField * pRspInfo);

  /**
  * STOCK-����������Ӧ
  * @param pMarketDataField:ָ�����ǿ�,��������������Ӧ�ṹ��ַ
  */
  virtual void OnStockMarketData(DFITCStockDepthMarketDataField * pMarketDataField) {}

  /**
  * SOP-����������Ӧ
  * @param pMarketDataField:ָ�����ǿ�,��������������Ӧ�ṹ��ַ
  */
  void OnSOPMarketData(DFITCSOPDepthMarketDataField * pMarketDataField);

  /**
  * STOCK-����������Ӧ
  * @param pAvailableQuotInfo:ָ�����ǿ�,���ؿ��õ�������Ϣ
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ������ָ����Լ����ȡ������ʧ��
  * @param flag  Ϊ���Ǳ�ʾ���һ����Ϊ��ʱ��ʾ���к�����
  */
  virtual void OnRspStockAvailableQuot(DFITCRspQuotQryField * pAvailableQuotInfo, DFITCSECRspInfoField * pRspInfo,bool flag) {}

  /**
  * SOP-����������Ӧ
  * @param pAvailableQuotInfo:ָ�����ǿ�,���ؿ��õ�������Ϣ
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ������ָ����Լ����ȡ������ʧ��
  * @param flag  Ϊ���Ǳ�ʾ���һ����Ϊ��ʱ��ʾ���к�����
  */
  virtual void OnRspSopAvailableQuot(DFITCRspQuotQryField * pAvailableQuotInfo, DFITCSECRspInfoField * pRspInfo,bool flag) {}

  /**
  * �������������Ӧ
  * @param pMDPasswordUpdate:ָ�����ǿ�,�����û�����������Ӧ��Ϣ�ṹ��ַ,���������޸ĳɹ���
  * @param pRspInfo:ָ�����ǿգ����ش�����Ϣ��ַ�����������޸�ʧ�ܡ�
  */
  virtual void OnRspUserMDPasswordUpdate(DFITCSECRspMDPasswordUpdateField *pMDPasswordUpdate,DFITCSECRspInfoField * pRspInfo){}

  private:
  dfitc_sec_mduser_api_cb *m_pUserCb;
  void * m_parent;
};

#endif // _DFITC_SEC_MDUSER_HANDLER_H_
