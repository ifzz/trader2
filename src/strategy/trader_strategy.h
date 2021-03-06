#ifndef _TRADER_STRATEGY_H_
#define _TRADER_STRATEGY_H_

#include "queue.h"
#include "cmn_util.h"

#include "trader_data.h"
#include "trader_msg_struct.h"
#include "trader_strategy_engine.h"


typedef struct trader_strategy_order_def trader_strategy_order;
typedef struct trader_strategy_trade_def trader_strategy_trade;
typedef struct trader_strategy_plan_def trader_strategy_plan;
typedef struct trader_strategy_position_def trader_strategy_position;

struct trader_strategy_order_def {
  TAILQ_ENTRY(trader_strategy_order_def) next;
  char ref[21];
  trader_order* order;
  
};

struct trader_strategy_trade_def{
  TAILQ_ENTRY(trader_strategy_trade_def) next;
  char T2Direction;
  int TradeVolume;
  double T1Price;
  double T2Price;
};


// 开仓/平仓计划
struct trader_strategy_plan_def {
  TAILQ_ENTRY(trader_strategy_plan_def) next;
  // T1报单号
  char UserOrderLocalID[21];
  char cOpenClose;
  char cT1Direction;
  char cT2Direction;
  int nPlanVol;
  double fT1Price;
  double fT2Price;
};

// 持仓情况
struct trader_strategy_position_def {
  TAILQ_ENTRY(trader_strategy_position_def) next;
  // T2报单号
  char UserOrderLocalID[21];
  char cOpenClose;
  char cT1Direction;
  char cT2Direction;
  int nTradeVol;
  double fT1Price;
  double fT2Price;
};


typedef struct trader_strategy_def trader_strategy;
typedef struct trader_strategy_method_def trader_strategy_method;

#define TRADER_STG_GUZHI 0
#define TRADER_STG_GUZHI_MM 1
#define TRADER_STG_SHANGPIN 2

struct trader_strategy_def{
  // 策略详情
  int idx;
  char T1[31];
  char T2[31];
  int MP;
  int hold;
  int Mult;
  double KTOpen;
  double KTClose;
  double DTOpen;
  double DTClose;
  int T1Over;
  int T1Wait;
  int T2Over;
  int T2Wait;
  int PermitVol;
  int STG;
  int used;

  // T1 行情
  trader_tick oT1Tick;
  // T2 行情
  trader_tick oT2Tick;
  
  // 订单队列
  cmn_util_map* mapOrder;
  
  // 计划队列
  cmn_util_map* mapStrategyPlan;
  
  // 待成交队列
  TAILQ_HEAD(trader_strategy_order_def_list_not_finished, trader_strategy_order_def) listNotFinishedOrder;

  // T1成交队列
  cmn_util_map* mapHalfTraded;

  // 当前策略持仓
  int nPositionBuy;
  int nPositionSell;
  int nOrderSize;

  trade_position oBuyPosition;
  trade_position oSellPosition;

  
  struct trader_strategy_engine_def* pEngine;
  trader_strategy_method* pMethod;
};


struct trader_strategy_method_def{
  int (*xInit)(trader_strategy* self);
  int (*xOnTick)(trader_strategy* self, trader_tick* tick_data);
  int (*xOnOrder)(trader_strategy* self, trader_order* order_data);
  int (*xOnTrade)(trader_strategy* self, trader_trade* trade_data);
  // 重置仓位
  int (*xResetPosition)(trader_strategy* self, char buy_sell, int volume);
  // 查询仓位
  int (*xQueryPosition)(trader_strategy* self, char buy_sell, trade_position* p_position);
};

extern trader_strategy* trader_strategy_new();
extern void trader_strategy_free(trader_strategy* self);

#endif //_TRADER_STRATEGY_H_

