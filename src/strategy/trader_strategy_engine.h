#ifndef _TRADER_STRATEGY_ENGINE_H_
#define _TRADER_STRATEGY_ENGINE_H_

#include "trader_strategy.h"
#include "trader_data.h"
#include "trader_db.h"
#include "trader_msg_struct.h"
#include "femas_trader_api.h"

#include <event2/util.h>
#include <event2/event.h>

typedef struct trader_strategy_engine_action_param_def trader_strategy_engine_action_param;
struct trader_strategy_engine_action_param_def {
  struct event* evTimer;
  void* parent;
  char* contract;
  char* user_local_id;
  char* org_user_local_id;
};

#define TRADER_STRATEGY_ENGINE_SIZE 6

typedef struct trader_strategy_engine_def trader_strategy_engine;
typedef struct trader_strategy_engine_method_def trader_strategy_engine_method;


struct trader_strategy_engine_def {
  // EventBase
  struct event_base *pBase;
  // ���޲�
  int nPositionSize;
  
  char TradingDay[9];
  int nSequence;
  
  // ����API
  femas_trader_api* pFemasTraderApi;

  // ���ݿ�API
  trader_db* pTraderDb;
  
  cmn_util_map* orderStrategyMap;
  
  struct trader_strategy_def* trader_strategys[TRADER_STRATEGY_ENGINE_SIZE];

  // �ⲿ�ӿ�
  trader_strategy_engine_method* pMethod;
};


struct trader_strategy_engine_method_def {
  int (*xInit)(trader_strategy_engine* self);
  int (*xUpdateStrategy)(trader_strategy_engine* self, struct trader_cmd_update_req_def* strategy_data);
  int (*xUpdateTick)(trader_strategy_engine* self, struct trader_msg_mduser_struct_def * tick_data);
  int (*xUpdateTrade)(trader_strategy_engine* self, struct trader_msg_trader_struct_def* trade_data);
  int (*xUpdateOrder)(trader_strategy_engine* self, struct trader_msg_trader_struct_def* order_data);
  int (*xSendOrder)(trader_strategy_engine* self, struct trader_strategy_def* strategy, 
    char* contract, char direction, char offset, double price, int volume, char* user_local_id);
  int (*xCancelOrder)(trader_strategy_engine* self, char* contract, char* user_local_id, char* org_user_local_id);
  int (*xSetTimer)(trader_strategy_engine* self, char* contract, char* user_local_id, char* org_user_local_id);

  // ���ý�������
  int (*xTradingDaySet)(trader_strategy_engine* self, char* day);
  // ���õ�ǰuser_id
  int (*xLocalUserIdSet)(trader_strategy_engine* self, char* user_id);
  // ��ȡ���ɱ�����
  int (*xGenLocalId)(trader_strategy_engine* self, char type, char* local_id);
  // ����UserLocalId�жϱ�������
  char (*xGetOrderType)(trader_strategy_engine* self, char* local_id);

  int (*xUpdateAmount)(trader_strategy_engine* self, int vol);

  int (*xGetRest)(trader_strategy_engine* self);

  // �ر����в���
  int (*xCloseAll)(trader_strategy_engine* self);

  // ���ò�λ
  int (*xResetPosition)(trader_strategy_engine* self, int stage_id, char buy_sell, int volume);

  // ��ѯ��λ
  int (*xQueryPosition)(trader_strategy_engine* self, int stage_id, char buy_sell, trade_position* p_position);

  // ���汨��
  int (*xSaveOrder)(trader_strategy_engine* self, trader_order* pTraderOrder);
  
  // ���潻��
  int (*xSaveTrade)(trader_strategy_engine* self, trader_trade* pTraderTrade);
};

extern trader_strategy_engine* trader_strategy_engine_new();
extern void trader_strategy_engine_free(trader_strategy_engine* self);

#endif
