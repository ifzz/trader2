
#ifndef _TRADER_TRADER_API_LTS_H_
#define _TRADER_TRADER_API_LTS_H_

#ifdef __cplusplus
extern "C" {
#endif

//#include "lts/SecurityFtdcTraderApi.h"
//#include "lts/SecurityFtdcQueryApi.h"

#include "trader_trader_api.h"

typedef struct trader_trader_api_lts_def trader_trader_api_lts;

struct trader_trader_api_lts_def {
  void* pTraderApi;
  void* pQueryApi;
  void* pTraderHandler;
  void* pQueryHandler;
  int nTraderRequestID;
  int nQueryRequestID;
	///«∞÷√±‡∫≈
	int nFrontID;
	///ª·ª∞±‡∫≈
	int nSessionID;
};


extern trader_trader_api_method* trader_trader_api_lts_method_get();

#ifdef __cplusplus
}
#endif


#endif //_TRADER_TRADER_API_LTS_H_

