
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmn_log.h"
#include "cmn_util.h"
#include "trader_strategy.h"
#include "trader_data.h"
#include "trader_msg_struct.h"
#include "femas_trader_api.h"

#include "trader_strategy_engine.h"

static int trader_strategy_engine_init(trader_strategy_engine* self);
static int trader_strategy_engine_update_strategy(trader_strategy_engine* self, struct trader_cmd_update_req_def* strategy_data);
static int trader_strategy_engine_update_tick(trader_strategy_engine* self, struct trader_msg_mduser_struct_def * tick_data);
static int trader_strategy_engine_update_trade(trader_strategy_engine* self, struct trader_msg_trader_struct_def* trade_data);
static int trader_strategy_engine_update_order(trader_strategy_engine* self, struct trader_msg_trader_struct_def* order_data);
static int trader_strategy_engine_send_order(trader_strategy_engine* self, trader_strategy* strategy, char* contract, char direction, char offset, double price, int volume, char* user_local_id);
static int trader_strategy_engine_cancel_order(trader_strategy_engine* self, char* contract, char* user_local_id, char* org_user_local_id);
static int trader_strategy_engine_set_timer(trader_strategy_engine* self, char* contract, char* user_local_id, char* org_user_local_id);

static int trader_strategy_engine_trading_day_set(trader_strategy_engine* self, char* day);
static int trader_strategy_engine_local_user_id_set(trader_strategy_engine* self, char* user_id);
static int trader_strategy_engine_gen_local_id(trader_strategy_engine* self, char type, char* local_id);
static char trader_strategy_engine_get_order_type(trader_strategy_engine* self, char* local_id);

static int trader_strategy_engine_update_amount(trader_strategy_engine* self, int vol);
static int trader_strategy_engine_get_rest(trader_strategy_engine* self);

static int trader_strategy_engine_close_all(trader_strategy_engine* self);
static int trader_strategy_engine_reset_position(trader_strategy_engine* self, int stage_id, char buy_sell, int volume);
static int trader_strategy_engine_query_position(trader_strategy_engine* self, int stage_id, char buy_sell, trade_position* p_position);

static int trader_strategy_engine_save_order(trader_strategy_engine* self, trader_order* pTraderOrder);
static int trader_strategy_engine_save_trade(trader_strategy_engine* self, trader_trade* pTraderTrade);

static trader_strategy_engine_method* trader_strategy_engine_method_get();

static void  trader_strategy_engine_timeout_cb(evutil_socket_t fd, short event, void *arg);

trader_strategy_engine_method* trader_strategy_engine_method_get()
{
  static trader_strategy_engine_method trader_strategy_engine_method_st = {
    trader_strategy_engine_init,
    trader_strategy_engine_update_strategy,
    trader_strategy_engine_update_tick,
    trader_strategy_engine_update_trade,
    trader_strategy_engine_update_order,
    trader_strategy_engine_send_order,
    trader_strategy_engine_cancel_order,
    trader_strategy_engine_set_timer,
    trader_strategy_engine_trading_day_set,
    trader_strategy_engine_local_user_id_set,
    trader_strategy_engine_gen_local_id,
    trader_strategy_engine_get_order_type,
    trader_strategy_engine_update_amount,
    trader_strategy_engine_get_rest,
    trader_strategy_engine_close_all,
    trader_strategy_engine_reset_position,
    trader_strategy_engine_query_position,
    trader_strategy_engine_save_order,
    trader_strategy_engine_save_trade
  };
  return &trader_strategy_engine_method_st;
}

int trader_strategy_engine_init(trader_strategy_engine* self)
{
  return 0;
}

int trader_strategy_engine_update_strategy(trader_strategy_engine* self, struct trader_cmd_update_req_def* strategy_data)
{
  struct trader_cmd_update_req_def *pUpdate = strategy_data;
  int i;
  int idx;
  trader_strategy* pStrategy;
  
  for(i = 0; i < pUpdate->num; i++){
    idx = pUpdate->stage[i].StageId;
    pStrategy = self->trader_strategys[idx];

  	CMN_DEBUG("StageId[%d]\n", pUpdate->stage[i].StageId);
  	CMN_DEBUG("T1[%s]\n", pUpdate->stage[i].T1);
  	CMN_DEBUG("T2[%s]\n", pUpdate->stage[i].T2);
  	CMN_DEBUG("MP[%d]\n", pUpdate->stage[i].MP);
  	CMN_DEBUG("Hold[%d]\n", pUpdate->stage[i].Hold);
  	CMN_DEBUG("Mult[%d]\n", pUpdate->stage[i].Mult);
  	CMN_DEBUG("KTOpen[%lf]\n", pUpdate->stage[i].KTOpen);
  	CMN_DEBUG("KTClose[%lf]\n", pUpdate->stage[i].KTClose);
  	CMN_DEBUG("DTOpen[%lf]\n", pUpdate->stage[i].DTOpen);
  	CMN_DEBUG("DTClose[%lf]\n", pUpdate->stage[i].DTClose);
  	CMN_DEBUG("T1Over[%d]\n", pUpdate->stage[i].T1Over);
  	CMN_DEBUG("T1Wait[%d]\n", pUpdate->stage[i].T1Wait);
  	CMN_DEBUG("T2Over[%d]\n", pUpdate->stage[i].T2Over);
  	CMN_DEBUG("T2Wait[%d]\n", pUpdate->stage[i].T2Wait);
  	CMN_DEBUG("PermitVol[%d]\n", pUpdate->stage[i].PermitVol);
  	CMN_DEBUG("STG[%d]\n", pUpdate->stage[i].STG);
  	CMN_DEBUG("Used[%d]\n", pUpdate->stage[i].Used);

    
    pStrategy->idx = pUpdate->stage[i].StageId;
    strcpy(pStrategy->T1, pUpdate->stage[i].T1);
    strcpy(pStrategy->T2, pUpdate->stage[i].T2);
    pStrategy->MP = pUpdate->stage[i].MP;
    pStrategy->hold = pUpdate->stage[i].Hold;
    pStrategy->Mult = pUpdate->stage[i].Mult;
    pStrategy->KTOpen = pUpdate->stage[i].KTOpen;
    pStrategy->KTClose = pUpdate->stage[i].KTClose;
    pStrategy->DTOpen = pUpdate->stage[i].DTOpen;
    pStrategy->DTClose = pUpdate->stage[i].DTClose;
    pStrategy->T1Over = pUpdate->stage[i].T1Over;
    pStrategy->T1Wait = pUpdate->stage[i].T1Wait;
    pStrategy->T2Over = pUpdate->stage[i].T2Over;
    pStrategy->T2Wait = pUpdate->stage[i].T2Wait;
    pStrategy->PermitVol = pUpdate->stage[i].PermitVol;
    pStrategy->STG = pUpdate->stage[i].STG;
    pStrategy->used = pUpdate->stage[i].Used; 

    strcpy(pStrategy->oBuyPosition.T1, pStrategy->T1);
    strcpy(pStrategy->oBuyPosition.T2, pStrategy->T2);
    strcpy(pStrategy->oSellPosition.T1, pStrategy->T1);
    strcpy(pStrategy->oSellPosition.T2, pStrategy->T2);

  }
  
  return 0;
}


int trader_strategy_engine_update_tick(trader_strategy_engine* self, struct trader_msg_mduser_struct_def * tick_data)
{
  int i = 0;
  trader_strategy* pStrategy;
  
  trader_tick oTick;
  
  strcpy(oTick.InstrumentID, tick_data->InstrumentID);
  strcpy(oTick.TradingDay, tick_data->TradingDay);
  strcpy(oTick.UpdateTime, tick_data->UpdateTime);
  oTick.UpdateMillisec = tick_data->UpdateMillisec;
  oTick.BidPrice1 = tick_data->BidPrice1;
  oTick.BidVolume1 = tick_data->BidVolume1;
  oTick.AskPrice1 = tick_data->AskPrice1;
  oTick.AskVolume1 = tick_data->AskVolume1;

  CMN_DEBUG("推送Tick数据\n");
  for(i = 0; i < TRADER_STRATEGY_ENGINE_SIZE; i++){
    pStrategy = self->trader_strategys[i];
    pStrategy->pMethod->xOnTick(pStrategy, &oTick);
  }

  return 0;
}

int trader_strategy_engine_update_trade(trader_strategy_engine* self, struct trader_msg_trader_struct_def* trade_data)
{
  trader_strategy* pStrategy;
  char* pLocalId = trade_data->UserOrderLocalID;
  
  pStrategy = (trader_strategy*)self->orderStrategyMap->pMethod->xGet(self->orderStrategyMap, pLocalId);
  if(!pStrategy){
    CMN_ERROR("trader_strategy not found pLocalId=[%s]\n", pLocalId);
    return -1;
  }
  trader_trade oTrade;
  trader_trade* pTrade = &oTrade;
  strcpy(pTrade->InstrumentID, trade_data->InstrumentID);
  strcpy(pTrade->UserOrderLocalID, trade_data->UserOrderLocalID);
  strcpy(pTrade->TradingDay, trade_data->TradingDay);
  strcpy(pTrade->TradeTime, trade_data->TradeTime);
  pTrade->TradePrice = trade_data->TradePrice;
  pTrade->TradeVolume = trade_data->TradeVolume;
  strcpy(pTrade->TradeID, trade_data->TradeID);
  
  CMN_DEBUG("推送Trade数据\n");
  pStrategy->pMethod->xOnTrade(pStrategy, pTrade);

  return 0;

}


int trader_strategy_engine_update_order(trader_strategy_engine* self, struct trader_msg_trader_struct_def* order_data)
{
  trader_strategy* pStrategy;
  char* pLocalId = order_data->UserOrderLocalID;
  
  pStrategy = (trader_strategy*)self->orderStrategyMap->pMethod->xGet(self->orderStrategyMap, pLocalId);
  if(!pStrategy){
    CMN_ERROR("trader_strategy not found pLocalId=[%s]\n", pLocalId);
    return -1;
  }

  
  trader_order oOrder;
  trader_order* pOrder = &oOrder;  
  strcpy(pOrder->InstrumentID, order_data->InstrumentID);
  strcpy(pOrder->UserOrderLocalID, order_data->UserOrderLocalID);
  pOrder->Direction = order_data->Direction;
  pOrder->OffsetFlag = order_data->OffsetFlag;
  pOrder->HedgeFlag = order_data->HedgeFlag;
  pOrder->LimitPrice = order_data->LimitPrice;
  pOrder->VolumeOriginal = order_data->VolumeOriginal;
  pOrder->VolumeTraded = order_data->VolumeTraded;
  pOrder->OrderStatus = order_data->OrderStatus;
  strcpy(pOrder->InsertTime, order_data->InsertTime);

  CMN_DEBUG("推送pOrder数据\n");
  pStrategy->pMethod->xOnOrder(pStrategy, pOrder);
}


int trader_strategy_engine_send_order(trader_strategy_engine* self, trader_strategy* strategy, char* contract, char direction, char offset, double price, int volume, char* user_local_id)
{
  // 下单
  self->pFemasTraderApi->pMethod->xOrderInsert(self->pFemasTraderApi, 
    contract, user_local_id, direction, offset, price, volume);
  
  self->orderStrategyMap->pMethod->xPut(self->orderStrategyMap, user_local_id, (void*)strategy);

  return 0;
}

int trader_strategy_engine_cancel_order(trader_strategy_engine* self, char* contract, char* user_local_id, char* org_user_local_id)
{
  // 撤单
  self->pFemasTraderApi->pMethod->xOrderAction(self->pFemasTraderApi, 
    contract, user_local_id, org_user_local_id);

  return 0;

}

int trader_strategy_engine_set_timer(trader_strategy_engine* self, char* contract, char* user_local_id, char* org_user_local_id)
{
  trader_strategy_engine_action_param* param = (trader_strategy_engine_action_param*)malloc(sizeof(trader_strategy_engine_action_param));
  param->parent = self;
  
  param->contract = (char*)malloc(sizeof(char)*(strlen(contract) + 1));
  strcpy(param->contract, contract);
  
  param->user_local_id = (char*)malloc(sizeof(char)*(strlen(user_local_id) + 1));
  strcpy(param->user_local_id, user_local_id);
  
  param->org_user_local_id = (char*)malloc(sizeof(char)*(strlen(org_user_local_id) + 1));
  strcpy(param->org_user_local_id, org_user_local_id);

  param->evTimer = evtimer_new(self->pBase, trader_strategy_engine_timeout_cb, (void*)param);

  
  static struct timeval t1_timeout = {
    0, 300*1000
  };
  evtimer_add(param->evTimer, &t1_timeout);
  
  return 0;
}

int trader_strategy_engine_trading_day_set(trader_strategy_engine* self, char* day)
{
  strncpy(self->TradingDay, day, sizeof(self->TradingDay));
  return 0;
}

int trader_strategy_engine_local_user_id_set(trader_strategy_engine* self, char* user_id)
{
  char sSequence[8];
  int nSeq = 0;
  int nCmp;
  if(strlen(user_id)>0){
    nCmp = memcmp(self->TradingDay, &user_id[0], 8);
    if(nCmp <= 0){
      memcpy(sSequence, &user_id[8], 7);
      sSequence[7] ='\0';
      nSeq = atoi(sSequence);

      if(self->nSequence > nSeq){
        nSeq = self->nSequence;
      }
    }
    self->nSequence = nSeq + 1;
  }

  return 0;
}

int trader_strategy_engine_gen_local_id(trader_strategy_engine* self, char type, char* local_id)
{
  sprintf(local_id, "%8s%07d%c", self->TradingDay, self->nSequence, type);
  self->nSequence++;
  return 0;
}

char trader_strategy_engine_get_order_type(trader_strategy_engine* self, char* local_id)
{
  return local_id[15];
}

int trader_strategy_engine_update_amount(trader_strategy_engine* self, int vol)
{
  self->nPositionSize += vol;
  return 0;
}

int trader_strategy_engine_get_rest(trader_strategy_engine* self)
{
  return self->nPositionSize;
}

int trader_strategy_engine_close_all(trader_strategy_engine* self)
{
  int i;
  trader_strategy* pTraderStrategy;
  for(i = 0; i < TRADER_STRATEGY_ENGINE_SIZE; i++){
    pTraderStrategy = self->trader_strategys[i];
    pTraderStrategy->used = 0;
  }
  return 0;
}

int trader_strategy_engine_reset_position(trader_strategy_engine* self, int stage_id, char buy_sell, int volume)
{
  int nRet = 0;
  trader_strategy* pTraderStrategy;
  pTraderStrategy = self->trader_strategys[stage_id];

  nRet = pTraderStrategy->pMethod->xResetPosition(pTraderStrategy, buy_sell, volume);
  
  return nRet;
}

int trader_strategy_engine_query_position(trader_strategy_engine* self, int stage_id, char buy_sell, trade_position* p_position)
{
  CMN_DEBUG("Enter!\n");
  int nRet = 0;
  trader_strategy* pTraderStrategy;
  pTraderStrategy = self->trader_strategys[stage_id];

  nRet = pTraderStrategy->pMethod->xQueryPosition(pTraderStrategy, buy_sell, p_position);
  
  return nRet;
}

int trader_strategy_engine_save_order(trader_strategy_engine* self, trader_order* pTraderOrder)
{
  CMN_DEBUG("Enter!\n");
  struct trader_db_order_inf_def oOrderInf;
  int nRet;
  int nSqlCode = 0;

  strncpy(oOrderInf.UserOrderLocalID, pTraderOrder->UserOrderLocalID, sizeof(oOrderInf.UserOrderLocalID));
  strncpy(oOrderInf.InstrumentID, pTraderOrder->InstrumentID, sizeof(oOrderInf.InstrumentID));
  snprintf(oOrderInf.Direction, sizeof(oOrderInf.Direction), "%c", pTraderOrder->Direction);
  snprintf(oOrderInf.OffsetFlag, sizeof(oOrderInf.OffsetFlag), "%c", pTraderOrder->OffsetFlag);
  snprintf(oOrderInf.HedgeFlag, sizeof(oOrderInf.HedgeFlag), "%c", pTraderOrder->HedgeFlag);
  snprintf(oOrderInf.OrderStatus, sizeof(oOrderInf.OrderStatus), "%c", pTraderOrder->OrderStatus);
  oOrderInf.LimitPrice = pTraderOrder->LimitPrice;
  oOrderInf.Volume = pTraderOrder->VolumeOriginal;
  strncpy(oOrderInf.InsertTime, pTraderOrder->InsertTime, sizeof(oOrderInf.InsertTime));
  cmn_util_timeval_conv(oOrderInf.LocalCreateTime, &pTraderOrder->CreateTime);
  cmn_util_timeval_conv(oOrderInf.LocalUpdateTime, &pTraderOrder->UpdateTime);

  nRet = self->pTraderDb->pMethod->xOrderInsert(self->pTraderDb, &oOrderInf, &nSqlCode);

  return nRet;
}

int trader_strategy_engine_save_trade(trader_strategy_engine* self, trader_trade* pTraderTrade)
{
  CMN_DEBUG("Enter!\n");
  struct trader_db_trade_inf_def oTradeInf;
  int nRet;
  int nSqlCode = 0;

  strcpy(oTradeInf.InstrumentID, pTraderTrade->InstrumentID);
  strcpy(oTradeInf.TradingDay, pTraderTrade->TradingDay);
  strcpy(oTradeInf.TradeID, pTraderTrade->TradeID);
  strcpy(oTradeInf.UserOrderLocalID, pTraderTrade->UserOrderLocalID);
  oTradeInf.TradePrice = pTraderTrade->TradePrice;
  oTradeInf.TradeVolume = pTraderTrade->TradeVolume;
  strcpy(oTradeInf.TradeTime, pTraderTrade->TradeTime);
  cmn_util_timeval_conv(oTradeInf.LocalUpdateTime, &pTraderTrade->UpdateTime);

  nRet = self->pTraderDb->pMethod->xTradeInsert(self->pTraderDb, &oTradeInf, &nSqlCode);

  return nRet;
}

void trader_strategy_engine_timeout_cb(evutil_socket_t fd, short event, void* arg)
{
  trader_strategy_engine_action_param* param = (trader_strategy_engine_action_param*)arg;
  trader_strategy_engine* self = param->parent;
  self->pMethod->xCancelOrder(self, param->contract, param->user_local_id, param->org_user_local_id);

  free(param->contract);
  free(param->user_local_id);
  free(param->org_user_local_id);
  evtimer_del(param->evTimer);
  event_free(param->evTimer);

  free(param);

  return;
}

trader_strategy_engine* trader_strategy_engine_new()
{
  trader_strategy_engine* self = (trader_strategy_engine*)malloc(sizeof(trader_strategy_engine));

  self->orderStrategyMap = cmn_util_map_new();

  //trader_strategys
  int i;
  trader_strategy* pTraderStrategy;
  for(i = 0; i < TRADER_STRATEGY_ENGINE_SIZE; i++){
    pTraderStrategy = trader_strategy_new();
    pTraderStrategy->pMethod->xInit(pTraderStrategy);
    pTraderStrategy->pEngine = self;
    pTraderStrategy->idx = i;
    self->trader_strategys[i] = pTraderStrategy;
  }

  self->pMethod = trader_strategy_engine_method_get();

  return self;

}

void trader_strategy_engine_free(trader_strategy_engine* self)
{  
  if(self->orderStrategyMap){
    cmn_util_map_free(self->orderStrategyMap);
  }
  
  int i;
  trader_strategy* pTraderStrategy;
  for(i = 0; i < TRADER_STRATEGY_ENGINE_SIZE; i++){
    pTraderStrategy = self->trader_strategys[i];
    trader_strategy_free(pTraderStrategy);
  }
  
  if(self){
    free(self);
  }
  
}


