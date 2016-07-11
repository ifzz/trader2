
#ifndef _TRADER_MDUSER_API_LTS_H_
#define _TRADER_MDUSER_API_LTS_H_

//#include "lts/SecurityFtdcMdApi.h"

#ifdef __cplusplus
extern "C" {
#endif


#include "trader_mduser_api.h"
//#include "wrap/LtsMduserHandler.h"

typedef struct trader_mduser_api_lts_def  trader_mduser_api_lts;
struct trader_mduser_api_lts_def {
  void* pMdApi;
  void* pHandler;
  int nRequestID;
};

extern trader_mduser_api_method* trader_mduser_api_lts_method_get();

#ifdef __cplusplus
}
#endif


#endif //_TRADER_MDUSER_API_LTS_H_

