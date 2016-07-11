#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h> 

#include "cmn_log.h"
#include "cmn_util.h"
#include "trader_strategy_engine.h"
#include "trader_data.h"
#include "trader_msg_struct.h"
#include "femas_trader_api.h"


#include "trader_strategy.h"

static int trader_strategy_init(trader_strategy* self);
static int trader_strategy_on_tick(trader_strategy* self, trader_tick* tick_data);
static int trader_strategy_on_order(trader_strategy* self, trader_order* order_data);
static int trader_strategy_on_trade(trader_strategy* self, trader_trade* trade_data);

static int trader_strategy_tick_not_finished(trader_strategy* self);
static int trader_strategy_tick_finished(trader_strategy* self);
static int trader_strategy_tick_open(trader_strategy* self);

// ���ò�λ
static int trader_strategy_reset_position(trader_strategy* self, char buy_sell, int volume);
// ��ѯ��λ
static int trader_strategy_query_position(trader_strategy* self, char buy_sell, trade_position* p_position);

// ���²�λ
static int trader_strategy_update_position(trader_strategy* self, char buy_sell, int volume, double real_price);

static int trader_strategy_order_update(trader_strategy* self,  trader_order* order_data);
static int trader_strategy_order_t1_open_cancel(trader_strategy* self,  trader_order* order_data);
static int trader_strategy_order_t1_close_cancel(trader_strategy* self,  trader_order* order_data);
static int trader_strategy_order_t2_open_cancel(trader_strategy* self,  trader_order* order_data);
static int trader_strategy_order_t2_close_cancel(trader_strategy* self,  trader_order* order_data);
static int trader_strategy_order_t2_open_failed(trader_strategy* self,  trader_order* order_data);
static int trader_strategy_order_t2_close_failed(trader_strategy* self,  trader_order* order_data);

static int trader_strategy_trade_t1_open(trader_strategy* self, trader_trade* trade_data);
static int trader_strategy_trade_t2_open(trader_strategy* self, trader_trade* trade_data);
static int trader_strategy_trade_t1_close(trader_strategy* self, trader_trade* trade_data);
static int trader_strategy_trade_t2_close(trader_strategy* self, trader_trade* trade_data);


static int trader_strategy_judge_buy_open(trader_strategy* self);
static int trader_strategy_judge_buy_close(trader_strategy* self);
static int trader_strategy_judge_sell_open(trader_strategy* self);
static int trader_strategy_judge_sell_close(trader_strategy* self);

static double trader_strategy_t1_price(trader_strategy* self, char buy_sell, char open_close);
static double trader_strategy_t2_price(trader_strategy* self, char buy_sell, char open_close);

static int trader_strategy_t1_open(trader_strategy* self, char buy_sell);
static int trader_strategy_t1_close(trader_strategy* self, char buy_sell);
static int trader_strategy_t2_open(trader_strategy* self, trader_strategy_trade* strategy_trade);
static int trader_strategy_t2_close(trader_strategy* self, trader_strategy_trade* strategy_trade);

static int trader_strategy_cancel_order(trader_strategy* self, char* user_order_id);
static int trader_strategy_timed_cancel(trader_strategy* self, char* user_order_id);

static int trader_strategy_release_order(trader_strategy* self, trader_order* order_data);

static int trader_strategy_print_tick(trader_strategy* self);

static trader_strategy_method* trader_strategy_method_get();

trader_strategy_method* trader_strategy_method_get()
{
  static trader_strategy_method trader_strategy_method_st = {
    trader_strategy_init,
    trader_strategy_on_tick,
    trader_strategy_on_order,
    trader_strategy_on_trade,
    trader_strategy_reset_position,
    trader_strategy_query_position
  };

  return &trader_strategy_method_st;
}

int trader_strategy_init(trader_strategy* self)
{
  // T1 ����
  memset(&self->oT1Tick, 0, sizeof(self->oT1Tick));
  // T2 ����
  memset(&self->oT2Tick, 0, sizeof(self->oT2Tick));
  
  // ��������
  self->mapOrder = cmn_util_map_new();
  
  // �ƻ�����
  self->mapStrategyPlan = cmn_util_map_new();

  // ���ɽ�����
  TAILQ_INIT(&self->listNotFinishedOrder);

  // T1�ɽ�����
  self->mapHalfTraded = cmn_util_map_new();
  
  // ��ǰ���Գֲ�
  // �൥
  self->nPositionBuy = 0;
  // �յ�
  self->nPositionSell = 0;

  // ��ǰί����
  self->nOrderSize = 0;

  trader_strategy_reset_position(self, TRADER_POSITION_BUY, 0);
  trader_strategy_reset_position(self, TRADER_POSITION_SELL, 0);
  
  return 0;
}


int trader_strategy_on_tick(trader_strategy* self, trader_tick* tick_data)
{
  
  if(!strcmp(self->T1, tick_data->InstrumentID)){
    memcpy(&self->oT1Tick, tick_data, sizeof(self->oT1Tick));
  }else if(!strcmp(self->T2, tick_data->InstrumentID)){
    memcpy(&self->oT2Tick, tick_data, sizeof(self->oT2Tick));
  }else{
    // �Ǳ����Թ�ע��Լ��ֱ�Ӻ���
    CMN_DEBUG("Not Focused[%s]!\n", tick_data->InstrumentID);
    return 0;
  }

  if(strcmp(self->oT1Tick.TradingDay, self->oT2Tick.TradingDay) 
  ||strcmp(self->oT1Tick.UpdateTime, self->oT2Tick.UpdateTime)
  ||(self->oT1Tick.UpdateMillisec != self->oT2Tick.UpdateMillisec)){
    // �ȴ��ڶ�����Լ����Ϣ
    CMN_DEBUG("Waiting for another tick[%s]!\n", tick_data->InstrumentID);
    return 0;
  }

  // ���ɽ����д���
  trader_strategy_tick_not_finished(self);

  // �ɽ����д���
  trader_strategy_tick_finished(self);

  // �½�ί���ж�
  trader_strategy_tick_open(self);
  
  return 0;
}

int trader_strategy_on_order(trader_strategy* self, trader_order* order_data)
{
  // ���¶���״̬
  trader_strategy_order_update(self, order_data);

  // �ж϶���״̬
  if(TRADER_ORDER_OS_CANCELED == order_data->OrderStatus){
    // �ж϶�������
    char cOrderType;
    cOrderType = self->pEngine->pMethod->xGetOrderType(self->pEngine, order_data->UserOrderLocalID);
    
    if(TRADER_POSITION_TYPE_OPEN_T1 == cOrderType){
      // T1������ɣ����¼ƻ���λ
      trader_strategy_order_t1_open_cancel(self, order_data);
    }else if(TRADER_POSITION_TYPE_CLOSE_T1 == cOrderType){
      // T1������ɣ����¼ƻ���λ
      trader_strategy_order_t1_close_cancel(self, order_data);
    }else if(TRADER_POSITION_TYPE_OPEN_T2 == cOrderType){
      // T2������ɣ������µ�T2
      trader_strategy_order_t2_open_cancel(self, order_data);
    }else if(TRADER_POSITION_TYPE_CLOSE_T2 == cOrderType){
      // T2������ɣ����¼ƻ���λ
      trader_strategy_order_t2_close_cancel(self, order_data);
    }else{
      CMN_ERROR("cannot reached!\n");
    }
  }

  // �ύ����ʧ��
  if(TRADER_ORDER_OS_ACCEPTEDNOREPLY == order_data->OrderStatus){
    char cOrderType;
    cOrderType = self->pEngine->pMethod->xGetOrderType(self->pEngine, order_data->UserOrderLocalID);
    if(TRADER_POSITION_TYPE_OPEN_T1 == cOrderType){
      // T1������ɣ����¼ƻ���λ
      trader_strategy_order_t1_open_cancel(self, order_data);
    }else if(TRADER_POSITION_TYPE_CLOSE_T1 == cOrderType){
      // T1������ɣ����¼ƻ���λ
      trader_strategy_order_t1_close_cancel(self, order_data);
    }else if(TRADER_POSITION_TYPE_OPEN_T2 == cOrderType){
      // �����ύʧ�ܵ��µ���
      trader_strategy_order_t2_open_failed(self, order_data);
    }else if(TRADER_POSITION_TYPE_CLOSE_T2 == cOrderType){
      // �����ύʧ�ܵ��µ���
      trader_strategy_order_t2_close_failed(self, order_data);
    }else{
      CMN_ERROR("cannot reached!\n");
    }

  }
  

  if((TRADER_ORDER_OS_CANCELED == order_data->OrderStatus)
  ||(TRADER_ORDER_OS_ACCEPTEDNOREPLY == order_data->OrderStatus)
  ||(TRADER_ORDER_OS_ALLTRADED == order_data->OrderStatus)){
    trader_strategy_release_order(self, order_data);
  }
  
  return 0;
}

int trader_strategy_on_trade(trader_strategy* self, trader_trade* trade_data)
{
  // �ж϶�������
  char cOrderType;
  cOrderType = self->pEngine->pMethod->xGetOrderType(self->pEngine, trade_data->UserOrderLocalID);

  if(TRADER_POSITION_TYPE_OPEN_T1 == cOrderType){
    // ����T1 �ɽ����µ�T2
    trader_strategy_trade_t1_open(self, trade_data);
  }else if(TRADER_POSITION_TYPE_CLOSE_T1 == cOrderType){
    // ƽ��T1 �ɽ����µ�T2
    trader_strategy_trade_t1_close(self, trade_data);
  }else if(TRADER_POSITION_TYPE_OPEN_T2 == cOrderType){
    // ����T2 �ɽ������²�λ��Ϣ
    trader_strategy_trade_t2_open(self, trade_data);
  }else if(TRADER_POSITION_TYPE_CLOSE_T2 == cOrderType){
    // ƽ��T2 �ɽ������²�λ��Ϣ������������Ϣ
    trader_strategy_trade_t2_close(self, trade_data);
  }else{
    CMN_ERROR("cannot reached!\n");
  }

  // ����ɽ���¼
  gettimeofday(&trade_data->UpdateTime, NULL);
  self->pEngine->pMethod->xSaveTrade(self->pEngine, trade_data);

  return 0;
}

// ���ò�λ
int trader_strategy_reset_position(trader_strategy* self, char buy_sell, int volume)
{
  CMN_DEBUG("before nOrderSize[%d]nPositionBuy[%d]nPositionSell[%d]\n");
  int nTemp = 0;
  trade_position* pPosition;
  if(TRADER_POSITION_BUY == buy_sell){
    self->nOrderSize = self->nOrderSize + volume - self->nPositionBuy;
    self->nPositionBuy = volume;
    pPosition = &self->oBuyPosition;
    
  }else{
    self->nOrderSize = self->nOrderSize + volume - self->nPositionSell;
    self->nPositionSell = volume;
    pPosition = &self->oSellPosition;
  }
  CMN_DEBUG("after nOrderSize[%d]nPositionBuy[%d]nPositionSell[%d]\n");


  strcpy(pPosition->T1, self->T1);
  strcpy(pPosition->T2, self->T2);
  pPosition->StageID = self->idx;
  pPosition->Direction = buy_sell;
  pPosition->RealPrice = 0.0f;
  pPosition->ExpPrice = 0.0f;
  pPosition->Volume = volume;

  return 0;
}

// ��ѯ��λ
int trader_strategy_query_position(trader_strategy* self, char buy_sell, trade_position* p_position)
{
  trade_position* pPosition;

  if(TRADER_POSITION_BUY == buy_sell){
    pPosition = &self->oBuyPosition;
  }else{
    pPosition = &self->oSellPosition;
  }

  memcpy(p_position, pPosition, sizeof(trade_position));

  return 0;
}


// ���²�λ
int trader_strategy_update_position(trader_strategy* self, char buy_sell, int volume, double real_price)
{
  trade_position* pPosition;
  if(TRADER_POSITION_BUY == buy_sell){
    pPosition = &self->oBuyPosition;
    pPosition->ExpPrice = self->DTOpen;
  }else{
    pPosition = &self->oSellPosition;
    pPosition->ExpPrice = self->KTOpen;
  }
  

  if(volume > 0){
    pPosition->RealPrice = (pPosition->RealPrice * pPosition->Volume + real_price * volume) / (pPosition->Volume + volume);
  }
  
  pPosition->Volume += volume;
  return 0;
}


int trader_strategy_tick_not_finished(trader_strategy* self)
{
  CMN_DEBUG("Enter!\n");

  trader_strategy_order* pStrategyOrder;

  trader_order* pOrder;

  char cOrderType;

  double planPrice;
  double currentPrice;
  int wait;

  int bCancel = 0;
  
  // ���ɽ����д���
  // ��ȡδ�ɽ�����
  TAILQ_FOREACH(pStrategyOrder, &self->listNotFinishedOrder,  next){
    cOrderType = self->pEngine->pMethod->xGetOrderType(self->pEngine, 
      pStrategyOrder->ref);
    pOrder = pStrategyOrder->order;
    bCancel = 0;
    // �����T1��T1�ȴ��ж�
    if((TRADER_POSITION_TYPE_OPEN_T1 == cOrderType)
    ||(TRADER_POSITION_TYPE_CLOSE_T1 == cOrderType)){
      if(self->T1Wait == 0){
        bCancel = 1;
      }else{
        planPrice = pOrder->LimitPrice;
        if(TRADER_POSITION_BUY == pOrder->Direction){
          currentPrice = self->oT1Tick.BidPrice1;
          currentPrice += 0.2 * self->T1Over;
          planPrice += 0.2 * self->T1Wait;
          if(planPrice < currentPrice){
            bCancel = 1;
          }
          
        }else{
          currentPrice = self->oT1Tick.AskPrice1;
          currentPrice -= 0.2 * self->T1Over;
          planPrice -= 0.2 * self->T1Wait;
          if(planPrice > currentPrice){
            bCancel = 1;
          }
        }

      }
    }else if ((TRADER_POSITION_TYPE_OPEN_T2 == cOrderType)
    ||(TRADER_POSITION_TYPE_CLOSE_T2 == cOrderType)){
    // �����T2��T2�ȴ��ж�
      if(self->T2Wait == 0){
        bCancel = 1;
      }else{
        planPrice = pOrder->LimitPrice;
        if(TRADER_POSITION_BUY == pOrder->Direction){
          currentPrice = self->oT2Tick.BidPrice1;
          currentPrice += 0.2 * self->T2Over;
          planPrice += 0.2 * self->T2Wait;
          if(planPrice < currentPrice){
            bCancel = 1;
          }
          
        }else{
          currentPrice = self->oT2Tick.AskPrice1;
          currentPrice -= 0.2 * self->T2Over;
          planPrice -= 0.2 * self->T2Wait;
          if(planPrice > currentPrice){
            bCancel = 1;
          }
        }
      }
    }else {
      continue;
    }
    
    // �����ȴ�����������
    if(bCancel){
      trader_strategy_cancel_order(self, pStrategyOrder->ref);
    }
  }

  return 0;
}

int trader_strategy_tick_finished(trader_strategy* self)
{
  CMN_DEBUG("Enter!\n");
  int nSize = 0;

  if(!self->used){
    CMN_DEBUG("not used!\n");
    return 0;
  }

  // �޲��ж�
  if(self->hold > 0){
    nSize = self->nPositionBuy + self->nPositionSell;
    if(nSize <= self->hold){
      CMN_DEBUG("hold[%d]<=[%d]\n", nSize, self->hold);
      return 0;
    }
  }

  trader_strategy_position* pStrategyPosition;
  int bClose = 0;
  // ƽ�൥
  if(self->nPositionBuy > 0){
    bClose =  trader_strategy_judge_buy_close(self);
    if(bClose){
      // �½�ƽ�ּƻ�
      trader_strategy_t1_close(self, TRADER_POSITION_SELL);
    }
  }
  
  // ƽ�յ�
  if(self->nPositionSell > 0){
    bClose =  trader_strategy_judge_sell_close(self);
    if(bClose){
      // �½�ƽ�ּƻ�
      trader_strategy_t1_close(self, TRADER_POSITION_BUY);
    }
  }

  return 0;
}

int trader_strategy_tick_open(trader_strategy* self)
{
  CMN_DEBUG("Enter!\n");

  if(!self->used){
    CMN_DEBUG("not used!\n");
    return 0;
  }

  int nSize = 0;
  // �ֲܳ��ж�

  // ������ί���ж�
  nSize = self->nOrderSize;
  if(nSize >= self->MP){
    CMN_DEBUG("nOrderSize[%d]>=MP[%d]!\n", nSize, self->MP);
    return 0;
  }

  int nBuy = 0;
  int nSell = 0;
  double lBuyDiff;
  double lSellDiff;
  char cDirection;
  trader_tick* pT1MduserInf = &self->oT1Tick;
  trader_tick* pT2MduserInf = &self->oT2Tick;
  
  // �����ж�
  // ���������ж�
  do {
    //���ж�
    nBuy = trader_strategy_judge_buy_open(self);
    if(nBuy){
      //��
      cDirection = TRADER_POSITION_BUY;
      break;
    }
    
    //�����ж�
    nSell = trader_strategy_judge_sell_open(self);
    if(nSell){
      // ����
      cDirection = TRADER_POSITION_SELL;
      break;
    }
    
    CMN_DEBUG("cannot open\n");
    return 0;
  }while(0);

  // �½����ּƻ�  
  // �µ�T1
  trader_strategy_t1_open(self, cDirection);

  return 0;
}

int trader_strategy_order_update(trader_strategy* self, trader_order* order_data)
{
  CMN_DEBUG("Enter!\n");
  trader_order* pOrder;
  // ��ȡ����
  pOrder = self->mapOrder->pMethod->xGet(self->mapOrder, order_data->UserOrderLocalID);
  if(!pOrder){
    CMN_WARN("[%s]not in order list\n", order_data->UserOrderLocalID);
    return -1;
  }
  
  // ���¶�������
  pOrder->OrderStatus = order_data->OrderStatus;
  pOrder->VolumeTraded = order_data->VolumeTraded;
  
  // �ж��Ƿ���Ҫ���¶���ʱ��
  if((!pOrder->UpdateTime.tv_sec)
  && (!pOrder->UpdateTime.tv_usec)){
    pOrder->HedgeFlag = order_data->HedgeFlag;
    strcpy(pOrder->InsertTime, order_data->InsertTime);
    gettimeofday(&pOrder->UpdateTime, NULL);
  }
  
  return 0;
}

int trader_strategy_order_t1_open_cancel(trader_strategy* self, trader_order* order_data)
{
  CMN_DEBUG("Enter!\n");
  int nReset = order_data->VolumeOriginal - order_data->VolumeTraded;

  // ���¿��µ�����
  self->nOrderSize -= nReset;
  CMN_DEBUG("self->nOrderSize=[%d]\n", self->nOrderSize);
  
  return 0;
}

int trader_strategy_order_t1_close_cancel(trader_strategy* self,  trader_order* order_data)
{
  CMN_DEBUG("Enter!\n");

  // ���¿��µ�����
  int nReset = order_data->VolumeOriginal - order_data->VolumeTraded;
  if(TRADER_POSITION_BUY == order_data->Direction){
    self->nPositionSell += nReset;
    CMN_DEBUG("self->nPositionSell=[%d]!\n", self->nPositionSell);
  }else{
    self->nPositionBuy += nReset;
    CMN_DEBUG("self->nPositionBuy=[%d]!\n", self->nPositionBuy);
  }

  return 0;
}

int trader_strategy_order_t2_open_cancel(trader_strategy* self,  trader_order* order_data)
{
  CMN_DEBUG("Enter!\n");

  // ��ѯStrategyTrade
  trader_strategy_trade* pStrategyTrade = (trader_strategy_trade*)self->mapHalfTraded->pMethod->xGet(
    self->mapHalfTraded, order_data->UserOrderLocalID);
  if(!pStrategyTrade){
    CMN_WARN("order_data->UserOrderLocalID=[%s]\n", order_data->UserOrderLocalID);
    return -1;
  }

  // ��T1�������Ƴ�
  self->mapHalfTraded->pMethod->xRemove(
    self->mapHalfTraded, order_data->UserOrderLocalID);

  // ���¼���T2�۸�
  int nReset = order_data->VolumeOriginal - order_data->VolumeTraded;
  double fT2Price = trader_strategy_t2_price(self, order_data->Direction, TRADER_POSITION_OPEN);
  pStrategyTrade->T2Price = fT2Price;
  pStrategyTrade->TradeVolume = nReset;
  
  // �µ�T2
  trader_strategy_t2_open(self, pStrategyTrade);
  
  return 0;
}

int trader_strategy_order_t2_close_cancel(trader_strategy* self,  trader_order* order_data)
{
  CMN_DEBUG("Enter!\n");
  // ��ѯStrategyTrade
  trader_strategy_trade* pStrategyTrade = self->mapHalfTraded->pMethod->xGet(
    self->mapHalfTraded, order_data->UserOrderLocalID);
  if(!pStrategyTrade){
    CMN_WARN("order_data->UserOrderLocalID=[%s]\n", order_data->UserOrderLocalID);
    return -1;
  }

  // ��T1�������Ƴ�
  self->mapHalfTraded->pMethod->xRemove(
    self->mapHalfTraded, order_data->UserOrderLocalID);

  // ���¼���T2�۸�
  int nReset = order_data->VolumeOriginal - order_data->VolumeTraded;
  double fT2Price = trader_strategy_t2_price(self, order_data->Direction, TRADER_POSITION_CLOSE);
  pStrategyTrade->T2Price = fT2Price;
  pStrategyTrade->TradeVolume = nReset;
  
  // �µ�T2
  trader_strategy_t2_close(self, pStrategyTrade);
  
  return 0;
}

int trader_strategy_order_t2_open_failed(trader_strategy* self,  trader_order* order_data)
{
  CMN_DEBUG("Enter!\n");

  // ��ѯStrategyTrade
  trader_strategy_trade* pStrategyTrade = (trader_strategy_trade*)self->mapHalfTraded->pMethod->xGet(
    self->mapHalfTraded, order_data->UserOrderLocalID);
  if(!pStrategyTrade){
    CMN_WARN("order_data->UserOrderLocalID=[%s]\n", order_data->UserOrderLocalID);
    return -1;
  }

  // ��T1�������Ƴ�
  self->mapHalfTraded->pMethod->xRemove(
    self->mapHalfTraded, order_data->UserOrderLocalID);

  // ���¼���T2�۸�
  int nReset = order_data->VolumeOriginal - order_data->VolumeTraded;

  
  CMN_ERROR("t2_open_failed[%d]!\n", nReset);
  
  // ���¿��µ�����
  self->nOrderSize -= nReset;
  CMN_DEBUG("self->nOrderSize=[%d]!\n", self->nOrderSize);
  
  return 0;
}

int trader_strategy_order_t2_close_failed(trader_strategy* self,  trader_order* order_data)
{
  CMN_DEBUG("Enter!\n");
  // ��ѯStrategyTrade
  trader_strategy_trade* pStrategyTrade = self->mapHalfTraded->pMethod->xGet(
    self->mapHalfTraded, order_data->UserOrderLocalID);
  if(!pStrategyTrade){
    CMN_WARN("order_data->UserOrderLocalID=[%s]\n", order_data->UserOrderLocalID);
    return -1;
  }

  // ��T1�������Ƴ�
  self->mapHalfTraded->pMethod->xRemove(
    self->mapHalfTraded, order_data->UserOrderLocalID);

  // ���¼���T2�۸�
  int nReset = order_data->VolumeOriginal - order_data->VolumeTraded;
  
  CMN_ERROR("t2_close_failed[%d]!\n", nReset);

  // ���¿��µ�����
  self->nOrderSize -= nReset;
  CMN_DEBUG("self->nOrderSize=[%d]!\n", self->nOrderSize);
  
  return 0;
}



int trader_strategy_trade_t1_open(trader_strategy* self, trader_trade* trade_data)
{
  CMN_DEBUG("Enter!\n");
  trader_order* pOrder = NULL;
  char cBuySell;
  int nVol = trade_data->TradeVolume;
  double lPrice;
  trader_strategy_plan* pStrategyPlan = NULL;
  
  // ��ȡ�µ��ƻ�
  pStrategyPlan = (trader_strategy_plan*)self->mapStrategyPlan->pMethod->xGet(self->mapStrategyPlan, trade_data->UserOrderLocalID);
  if(!pStrategyPlan){
    //û�ҵ�����
    CMN_WARN("trade_data->UserOrderLocalID=[%s]\n", trade_data->UserOrderLocalID);
    return -1;
  }

  // ��ȡT2�۸�
  if(TRADER_POSITION_BUY == pStrategyPlan->cT1Direction){
    cBuySell = TRADER_POSITION_SELL;
  }else{
    cBuySell = TRADER_POSITION_BUY;
  }
  lPrice = pStrategyPlan->fT2Price;
  
  // �µ�T2
  trader_strategy_trade* pStrategyTrade = (trader_strategy_trade*)malloc(sizeof(trader_strategy_trade));
  pStrategyTrade->T2Direction = cBuySell;
  pStrategyTrade->TradeVolume = trade_data->TradeVolume;
  pStrategyTrade->T1Price = trade_data->TradePrice;
  pStrategyTrade->T2Price = lPrice;
  
  trader_strategy_t2_open(self, pStrategyTrade);

  return 0;
}

int trader_strategy_trade_t1_close(trader_strategy* self, trader_trade* trade_data)
{
  CMN_DEBUG("Enter!\n");
  trader_order* pOrder = NULL;
  trader_strategy_plan* pStrategyPlan = NULL;
  char cBuySell;
  int nVol = trade_data->TradeVolume;
  double lPrice;

  // ��ȡ�µ��ƻ�
  pStrategyPlan = (trader_strategy_plan*)self->mapStrategyPlan->pMethod->xGet(self->mapStrategyPlan, trade_data->UserOrderLocalID);
  if(!pStrategyPlan){
    //û�ҵ�����
    CMN_WARN("trade_data->UserOrderLocalID=[%s]\n", trade_data->UserOrderLocalID);
    return -1;
  }

  // ��ȡT2�۸�
  if(TRADER_POSITION_BUY == pStrategyPlan->cT1Direction){
    cBuySell = TRADER_POSITION_SELL;
  }else{
    cBuySell = TRADER_POSITION_BUY;
  }
  lPrice = pStrategyPlan->fT2Price;
  
  // �µ�T2
  trader_strategy_trade* pStrategyTrade = (trader_strategy_trade*)malloc(sizeof(trader_strategy_trade));
  pStrategyTrade->T2Direction = cBuySell;
  pStrategyTrade->TradeVolume = trade_data->TradeVolume;
  pStrategyTrade->T1Price = trade_data->TradePrice;
  pStrategyTrade->T2Price = lPrice;
  
  trader_strategy_t2_close(self, pStrategyTrade);

  return 0;
}

int trader_strategy_trade_t2_open(trader_strategy* self, trader_trade* trade_data)
{
  CMN_DEBUG("Enter!\n");
  // ��ѯStrategyTrade
  trader_strategy_trade* pStrategyTrade = self->mapHalfTraded->pMethod->xGet(
    self->mapHalfTraded, trade_data->UserOrderLocalID);
  if(!pStrategyTrade){
    CMN_WARN("trade_data->UserOrderLocalID=[%s]\n", trade_data->UserOrderLocalID);
    return -1;
  }

  // ���³ֲ���Ϣ
  if(TRADER_POSITION_BUY == pStrategyTrade->T2Direction){
    self->nPositionSell += trade_data->TradeVolume;
    CMN_DEBUG("self->nPositionSell=[%d]!\n", self->nPositionSell);
    trader_strategy_update_position(self, TRADER_POSITION_SELL, 
      trade_data->TradeVolume, pStrategyTrade->T1Price - trade_data->TradePrice); 
  }else{
    self->nPositionBuy += trade_data->TradeVolume;
    CMN_DEBUG("self->nPositionBuy=[%d]!\n", self->nPositionBuy);
    trader_strategy_update_position(self, TRADER_POSITION_BUY, 
      trade_data->TradeVolume, pStrategyTrade->T1Price - trade_data->TradePrice); 
  }

  // ���³ɽ�����
  pStrategyTrade->TradeVolume -= trade_data->TradeVolume;
  if(0 == pStrategyTrade->TradeVolume){
    // ȫ���ɽ�������
    self->mapHalfTraded->pMethod->xRemove(self->mapHalfTraded, trade_data->UserOrderLocalID);
  }

  

  return 0;

}

int trader_strategy_trade_t2_close(trader_strategy* self, trader_trade* trade_data)
{
  CMN_DEBUG("Enter!\n");
  // ��ѯStrategyTrade
  trader_strategy_trade* pStrategyTrade = (trader_strategy_trade*)self->mapHalfTraded->pMethod->xGet(
    self->mapHalfTraded, trade_data->UserOrderLocalID);
  if(!pStrategyTrade){
    CMN_WARN("trade_data->UserOrderLocalID=[%s]\n", trade_data->UserOrderLocalID);
    return -1;
  }

  // ���³ֲ���Ϣ
  if(TRADER_POSITION_BUY == pStrategyTrade->T2Direction){
    trader_strategy_update_position(self, TRADER_POSITION_BUY, 
      -trade_data->TradeVolume, 0.0f); 
  }else{
    trader_strategy_update_position(self, TRADER_POSITION_SELL, 
      -trade_data->TradeVolume, 0.0f); 
  }

  // ����ί����
  self->nOrderSize -= trade_data->TradeVolume;
  CMN_DEBUG("self->nOrderSize=[%d]!\n", self->nOrderSize);
  
  // ����T1�ɽ�����
  pStrategyTrade->TradeVolume -= trade_data->TradeVolume;
  if(0 == pStrategyTrade->TradeVolume){
    // ȫ���ɽ�������
    self->mapHalfTraded->pMethod->xRemove(self->mapHalfTraded, trade_data->UserOrderLocalID);
  }
  return 0;
}


int trader_strategy_judge_buy_open(trader_strategy* self)
{
  trader_tick* t1 = &self->oT1Tick;
  trader_tick* t2 = &self->oT2Tick;
  int nRet = 0;
  double t1set = t1->BidPrice1;
  double t2set = t2->BidPrice1;

  if(TRADER_STG_SHANGPIN == self->STG){
    if(t2->AskVolume1 < self->PermitVol){
      CMN_DEBUG("t2->AskVolume1[%d] < stage->PermitVol[%d]\n", t2->AskVolume1, self->PermitVol);
      return nRet;
    } 
      
  }

  if(TRADER_STG_GUZHI_MM == self->STG){
    long t2l = (long)((t2->BidPrice1 + t2->AskPrice1) * 5 + 1);

    t2set = t2l / 2;

    t2set /= 5;
  }
  
  CMN_DEBUG("T1SET[%.1lf]T2SET[%.1lf]DTOPen[%.1lf]\n",
    t1set, t2set, self->DTOpen);
  if((t1set - t2set) <= self->DTOpen)
  {
    nRet = 1;
  }
  
  return nRet;
}

int trader_strategy_judge_buy_close(trader_strategy* self)
{
  trader_tick* t1 = &self->oT1Tick;
  trader_tick* t2 = &self->oT2Tick;
  int nRet = 0;
  double t1set = t1->AskPrice1;
  double t2set = t2->AskPrice1;

  if(TRADER_STG_GUZHI_MM == self->STG){
    long t2l = (long)((t2->BidPrice1 + t2->AskPrice1) * 5 - 1);

    t2set = t2l / 2;

    t2set /= 5;
  }
  
  CMN_DEBUG("T1SET[%.1lf]T2SET[%.1lf]DTClose[%.1lf]\n",
    t1set, t2set, self->DTClose);
  if((t1set - t2set) >= self->DTClose)
  {
    nRet = 1;
  }
  
  return nRet;

}

int trader_strategy_judge_sell_open(trader_strategy* self)
{
  trader_tick* t1 = &self->oT1Tick;
  trader_tick* t2 = &self->oT2Tick;
  int nRet = 0;
  double t1set = t1->AskPrice1;
  double t2set = t2->AskPrice1;

  if(TRADER_STG_SHANGPIN == self->STG){
    if(t2->BidVolume1 < self->PermitVol){
      CMN_DEBUG("t2->AskVolume1[%d] < stage->PermitVol[%d]\n", t2->BidVolume1, self->PermitVol);
      return nRet;
    } 
  }

  if(TRADER_STG_GUZHI_MM == self->STG){
    long t2l = (long)((t2->BidPrice1 + t2->AskPrice1) * 5 - 1);

    t2set = t2l / 2;

    t2set /= 5;
  }
  
  CMN_DEBUG("T1SET[%.1lf]T2SET[%.1lf]KTOpen[%.1lf]\n",
    t1set, t2set, self->KTOpen);
  if((t1set - t2set) >= self->KTOpen)
  {
    nRet = 1;
  }
  
  return nRet;
}

int trader_strategy_judge_sell_close(trader_strategy* self)
{
  trader_tick* t1 = &self->oT1Tick;
  trader_tick* t2 = &self->oT2Tick;
  
  int nRet = 0;
  double t1set = t1->BidPrice1;
  double t2set = t2->BidPrice1;

  if(TRADER_STG_GUZHI_MM == self->STG){
    long t2l = (long)((t2->BidPrice1 + t2->AskPrice1) * 5 + 1);

    t2set = t2l / 2;

    t2set /= 5;
  }
  
  CMN_DEBUG("T1SET[%.1lf]T2SET[%.1lf]KTClose[%.1lf]\n",
    t1set, t2set, self->KTClose);
  if((t1set - t2set) <= self->KTClose)
  {
    nRet = 1;
  }
  
  return nRet;
}

double trader_strategy_t1_price(trader_strategy* self, char buy_sell, char open_close)
{
  trader_tick* t1 = &self->oT1Tick;
  trader_tick* t2 = &self->oT2Tick;
  double t1set;
  double t2set;
  long t2l;
  if(TRADER_POSITION_OPEN == open_close){
    if(TRADER_POSITION_BUY == buy_sell){
      // �࿪
      t2set = t2->BidPrice1;
      if(TRADER_STG_GUZHI_MM == self->STG){
        long t2l = (long)((t2->BidPrice1 + t2->AskPrice1) * 5 - 1);
        t2set = t2l / 2;
        t2set /= 5;
      }

      t1set = t2set + self->DTOpen + 0.2 * self->T1Over;
    }else{
      // �տ�
      t2set = t2->AskPrice1;
      if(TRADER_STG_GUZHI_MM == self->STG){
        long t2l = (long)((t2->BidPrice1 + t2->AskPrice1) * 5 + 1);
        t2set = t2l / 2;
        t2set /= 5;
      }

      t1set = t2set + self->KTOpen - 0.2 * self->T1Over;
    
    }
  }else{
    if(TRADER_POSITION_BUY == buy_sell){
      // ��ƽ
      t2set = t2->BidPrice1;
      if(TRADER_STG_GUZHI_MM == self->STG){
        long t2l = (long)((t2->BidPrice1 + t2->AskPrice1) * 5 - 1);
        t2set = t2l / 2;
        t2set /= 5;
      }

      t1set = t2set + self->KTClose - 0.2 * self->T1Over;
    }else{
      // ��ƽ 
      t2set = t2->AskPrice1;
      if(TRADER_STG_GUZHI_MM == self->STG){
        long t2l = (long)((t2->BidPrice1 + t2->AskPrice1) * 5 + 1);
        t2set = t2l / 2;
        t2set /= 5;
      }

      t1set = t2set + self->DTClose + 0.2 * self->T1Over;
    
    }
  }

  return t1set;
}

double trader_strategy_t2_price(trader_strategy* self, char buy_sell, char open_close)
{
  trader_tick* t1 = &self->oT1Tick;
  trader_tick* t2 = &self->oT2Tick;
  double t2set;

  
  if(TRADER_POSITION_BUY == buy_sell){
    // ��
    // �տ� ��ƽ
    t2set = t2->AskPrice1;
    if(TRADER_STG_GUZHI_MM == self->STG){
    long t2l = (long)((t2->BidPrice1 + t2->AskPrice1) * 5 + 1);
      t2set = t2l / 2;
      t2set /= 5;
    }
    t2set = t2set + 0.2 * self->T2Over;
 }else{
  // ��
    // �࿪ ��ƽ
    t2set = t2->BidPrice1;
    if(TRADER_STG_GUZHI_MM == self->STG){
      long t2l = (long)((t2->BidPrice1 + t2->AskPrice1) * 5 - 1);
      t2set = t2l / 2;
      t2set /= 5;
    }
    t2set = t2set - 0.2 * self->T2Over;
  }

  return t2set;
}

int trader_strategy_t1_open(trader_strategy* self, char buy_sell)
{
  // ��ӡ��ǰ����
  trader_strategy_print_tick(self);

  CMN_DEBUG("Enter!\n");
  char sLocalUserId[21];

  int nSize1 = 0;
  // �����µ���
  nSize1 = self->MP - self->nOrderSize;
  if(nSize1 > self->Mult){
    nSize1 = self->Mult;
  }
  

  // ��ȡ������
  self->pEngine->pMethod->xGenLocalId(self->pEngine, TRADER_POSITION_TYPE_OPEN_T1, sLocalUserId);

  // �����µ��ƻ�
  trader_strategy_plan* pStrategyPlan = (trader_strategy_plan*)malloc(sizeof(trader_strategy_plan));
  strcpy(pStrategyPlan->UserOrderLocalID, sLocalUserId);
  pStrategyPlan->cOpenClose = TRADER_POSITION_OPEN;
  if(TRADER_POSITION_BUY == buy_sell){
    pStrategyPlan->cT1Direction= TRADER_POSITION_BUY;
    pStrategyPlan->cT2Direction = TRADER_POSITION_SELL;
  }else{
    pStrategyPlan->cT1Direction= TRADER_POSITION_SELL;
    pStrategyPlan->cT2Direction = TRADER_POSITION_BUY;
  }
  pStrategyPlan->nPlanVol = nSize1;
  pStrategyPlan->fT1Price = trader_strategy_t1_price(self, pStrategyPlan->cT1Direction, TRADER_POSITION_OPEN);
  pStrategyPlan->fT2Price = trader_strategy_t2_price(self, pStrategyPlan->cT2Direction, TRADER_POSITION_OPEN);

  // ��ӵ��ƻ�����
  self->mapStrategyPlan->pMethod->xPut(self->mapStrategyPlan, sLocalUserId, (void*)pStrategyPlan);

  // ����ί����
  self->nOrderSize += nSize1;
  CMN_DEBUG("self->nOrderSize=[%d]\n", self->nOrderSize);
  
  // �µ�
  self->pEngine->pMethod->xSendOrder(self->pEngine, self,  self->T1,
    buy_sell, TRADER_POSITION_OPEN, pStrategyPlan->fT1Price, pStrategyPlan->nPlanVol, sLocalUserId);

  // ��������
  trader_order* pOrder = (trader_order*)malloc(sizeof(trader_order));
  strcpy(pOrder->InstrumentID, self->T1);
  strcpy(pOrder->UserOrderLocalID, sLocalUserId);
  pOrder->Direction = buy_sell;
  pOrder->OffsetFlag = TRADER_POSITION_OPEN;
  pOrder->LimitPrice = pStrategyPlan->fT1Price;
  pOrder->VolumeOriginal = pStrategyPlan->nPlanVol;
  pOrder->VolumeTraded = 0;
  pOrder->OrderStatus = '0';
  gettimeofday(&pOrder->CreateTime, NULL);
  memset(&pOrder->UpdateTime, 0, sizeof(pOrder->UpdateTime));

  // ���뵽�����б�
  self->mapOrder->pMethod->xPut(self->mapOrder, sLocalUserId, pOrder);

  // δ�ɽ�����
  trader_strategy_order* pStrategyOrder = (trader_strategy_order*)malloc(sizeof(trader_strategy_order));
  strcpy(pStrategyOrder->ref, sLocalUserId);
  pStrategyOrder->order = pOrder;
  TAILQ_INSERT_TAIL(&self->listNotFinishedOrder, pStrategyOrder, next);

  // 300���볷��
  if(TRADER_STG_SHANGPIN != self->STG){
    trader_strategy_timed_cancel(self, sLocalUserId);
  }
  
  return 0;
}
int trader_strategy_t1_close(trader_strategy* self, char buy_sell)
{
  // ��ӡ��ǰ����
  trader_strategy_print_tick(self);

  CMN_DEBUG("Enter!\n");
  char sLocalUserId[21];

  int nSize = self->Mult;
  if(TRADER_POSITION_BUY == buy_sell){
    if(nSize > self->nPositionSell){
      nSize = self->nPositionSell;
    }
  }else{
    if(nSize > self->nPositionBuy){
      nSize = self->nPositionBuy;
    }
  }

  // ��ȡ������
  self->pEngine->pMethod->xGenLocalId(self->pEngine, TRADER_POSITION_TYPE_CLOSE_T1, sLocalUserId);

  // �����µ��ƻ�
  trader_strategy_plan* pStrategyPlan = (trader_strategy_plan*)malloc(sizeof(trader_strategy_plan));
  strcpy(pStrategyPlan->UserOrderLocalID, sLocalUserId);
  pStrategyPlan->cOpenClose = TRADER_POSITION_CLOSE;
  if(TRADER_POSITION_BUY == buy_sell){
    pStrategyPlan->cT1Direction= TRADER_POSITION_BUY;
    pStrategyPlan->cT2Direction = TRADER_POSITION_SELL;
    self->nPositionSell -= nSize; 
    CMN_DEBUG("self->nPositionSell=[%d]!\n", self->nPositionSell);
  }else{
    pStrategyPlan->cT1Direction= TRADER_POSITION_SELL;
    pStrategyPlan->cT2Direction = TRADER_POSITION_BUY;
    self->nPositionBuy -= nSize; 
    CMN_DEBUG("self->nPositionBuy=[%d]!\n", self->nPositionBuy);
  }
  pStrategyPlan->nPlanVol = nSize;
  pStrategyPlan->fT1Price = trader_strategy_t1_price(self, pStrategyPlan->cT1Direction, TRADER_POSITION_CLOSE);
  pStrategyPlan->fT2Price = trader_strategy_t2_price(self, pStrategyPlan->cT2Direction, TRADER_POSITION_CLOSE);

  // ��ӵ��ƻ�����
  self->mapStrategyPlan->pMethod->xPut(self->mapStrategyPlan, sLocalUserId, pStrategyPlan);
  
  // �µ�
  self->pEngine->pMethod->xSendOrder(self->pEngine, self,  self->T1,
    pStrategyPlan->cT1Direction, TRADER_POSITION_CLOSE,  pStrategyPlan->fT1Price, 
    pStrategyPlan->nPlanVol, sLocalUserId);

  // ��������
  trader_order* pOrder = (trader_order*)malloc(sizeof(trader_order));
  strcpy(pOrder->InstrumentID, self->T1);
  strcpy(pOrder->UserOrderLocalID, sLocalUserId);
  pOrder->Direction = pStrategyPlan->cT1Direction;
  pOrder->OffsetFlag = TRADER_POSITION_CLOSE;
  pOrder->LimitPrice = pStrategyPlan->fT1Price;
  pOrder->VolumeOriginal = pStrategyPlan->nPlanVol;
  pOrder->VolumeTraded = 0;
  pOrder->OrderStatus = '0';
  gettimeofday(&pOrder->CreateTime, NULL);
  memset(&pOrder->UpdateTime, 0, sizeof(pOrder->UpdateTime));

  // ���뵽�����б�
  self->mapOrder->pMethod->xPut(self->mapOrder, sLocalUserId, pOrder);

  // δ�ɽ�����
  trader_strategy_order* pStrategyOrder = (trader_strategy_order*)malloc(sizeof(trader_strategy_order));
  strcpy(pStrategyOrder->ref, sLocalUserId);
  pStrategyOrder->order = pOrder;
  TAILQ_INSERT_TAIL(&self->listNotFinishedOrder, pStrategyOrder, next);  

  // 300���볷��
  if(TRADER_STG_SHANGPIN != self->STG){
    trader_strategy_timed_cancel(self, sLocalUserId);
  }


  return 0;
}

int trader_strategy_t2_open(trader_strategy* self, trader_strategy_trade* strategy_trade)
{
  CMN_DEBUG("Enter!\n");
  char sLocalUserId[21];

  // ��ȡ������
  self->pEngine->pMethod->xGenLocalId(self->pEngine, TRADER_POSITION_TYPE_OPEN_T2, sLocalUserId);

  // �µ�
  self->pEngine->pMethod->xSendOrder(self->pEngine, self,  self->T2,
    strategy_trade->T2Direction, TRADER_POSITION_OPEN, 
    strategy_trade->T2Price, strategy_trade->TradeVolume, sLocalUserId);

  // ����T1����
  self->mapHalfTraded->pMethod->xPut(self->mapHalfTraded, sLocalUserId, strategy_trade);

  // ��������
  trader_order* pOrder = (trader_order*)malloc(sizeof(trader_order));
  strcpy(pOrder->InstrumentID, self->T2);
  strcpy(pOrder->UserOrderLocalID, sLocalUserId);
  pOrder->Direction = strategy_trade->T2Direction;
  pOrder->OffsetFlag = TRADER_POSITION_OPEN;
  pOrder->LimitPrice = strategy_trade->T2Price;
  pOrder->VolumeOriginal = strategy_trade->TradeVolume;
  pOrder->VolumeTraded = 0;
  pOrder->OrderStatus = '0';
  gettimeofday(&pOrder->CreateTime, NULL);
  memset(&pOrder->UpdateTime, 0, sizeof(pOrder->UpdateTime));

  // ���뵽�����б�
  self->mapOrder->pMethod->xPut(self->mapOrder, sLocalUserId, pOrder);

  // δ�ɽ�����
  trader_strategy_order* pStrategyOrder = (trader_strategy_order*)malloc(sizeof(trader_strategy_order));
  strcpy(pStrategyOrder->ref, sLocalUserId);
  pStrategyOrder->order = pOrder;
  TAILQ_INSERT_TAIL(&self->listNotFinishedOrder, pStrategyOrder, next);  

  // 300���볷��
  if(TRADER_STG_SHANGPIN != self->STG){
    if(0 == self->T2Wait){
      trader_strategy_timed_cancel(self, sLocalUserId);
    }
  }

  return 0;
}

int trader_strategy_t2_close(trader_strategy* self, trader_strategy_trade* strategy_trade)
{
  CMN_DEBUG("Enter!\n");
  char sLocalUserId[21];

  // ��ȡ������
  self->pEngine->pMethod->xGenLocalId(self->pEngine, TRADER_POSITION_TYPE_CLOSE_T2, sLocalUserId);

  // �µ�
  self->pEngine->pMethod->xSendOrder(self->pEngine, self,  self->T2,
    strategy_trade->T2Direction, TRADER_POSITION_CLOSE, 
    strategy_trade->T2Price, strategy_trade->TradeVolume, sLocalUserId);

  // ����T1����
  self->mapHalfTraded->pMethod->xPut(self->mapHalfTraded, sLocalUserId, strategy_trade);

  // ��������
  trader_order* pOrder = (trader_order*)malloc(sizeof(trader_order));
  strcpy(pOrder->InstrumentID, self->T2);
  strcpy(pOrder->UserOrderLocalID, sLocalUserId);
  pOrder->Direction = strategy_trade->T2Direction;
  pOrder->OffsetFlag = TRADER_POSITION_CLOSE;
  pOrder->LimitPrice = strategy_trade->T2Price;
  pOrder->VolumeOriginal = strategy_trade->TradeVolume;
  pOrder->VolumeTraded = 0;
  pOrder->OrderStatus = '0';
  gettimeofday(&pOrder->CreateTime, NULL);
  memset(&pOrder->UpdateTime, 0, sizeof(pOrder->UpdateTime));

  // ���뵽�����б�
  self->mapOrder->pMethod->xPut(self->mapOrder, sLocalUserId, pOrder);

  // δ�ɽ�����
  trader_strategy_order* pStrategyOrder = (trader_strategy_order*)malloc(sizeof(trader_strategy_order));
  strcpy(pStrategyOrder->ref, sLocalUserId);
  pStrategyOrder->order = pOrder;
  TAILQ_INSERT_TAIL(&self->listNotFinishedOrder, pStrategyOrder, next);  

  // 300���볷��
  if(TRADER_STG_SHANGPIN != self->STG){
    if(0 == self->T2Wait){
      trader_strategy_timed_cancel(self, sLocalUserId);
    }
  }


  return 0;
}

int trader_strategy_cancel_order(trader_strategy* self, char* user_order_id)
{
  CMN_DEBUG("Enter!\n");
  char sCancelOrderId[21];
  char cOrderType;
  
  trader_order* pOrder = self->mapOrder->pMethod->xGet(self->mapOrder, user_order_id);
  cOrderType = self->pEngine->pMethod->xGetOrderType(self->pEngine, user_order_id);
  if(TRADER_POSITION_TYPE_OPEN_T1 == cOrderType){
    self->pEngine->pMethod->xGenLocalId(self->pEngine, 
      TRADER_POSITION_TYPE_OPEN_T1_ACTION, sCancelOrderId);
  }else if(TRADER_POSITION_TYPE_OPEN_T2 == cOrderType){
    self->pEngine->pMethod->xGenLocalId(self->pEngine, 
      TRADER_POSITION_TYPE_OPEN_T2_ACTION, sCancelOrderId);
  }else if(TRADER_POSITION_TYPE_CLOSE_T1 == cOrderType){
    self->pEngine->pMethod->xGenLocalId(self->pEngine, 
      TRADER_POSITION_TYPE_CLOSE_T1_ACTION, sCancelOrderId);
  }else if(TRADER_POSITION_TYPE_CLOSE_T2 == cOrderType){
    self->pEngine->pMethod->xGenLocalId(self->pEngine, 
      TRADER_POSITION_TYPE_CLOSE_T2_ACTION, sCancelOrderId);
  }

  self->pEngine->pMethod->xCancelOrder(self->pEngine,
    pOrder->InstrumentID, sCancelOrderId, user_order_id);

  return 0;
  
}

int trader_strategy_timed_cancel(trader_strategy* self, char* user_order_id)
{
  CMN_DEBUG("Enter!\n");
  char sCancelOrderId[21];
  char cOrderType;
  
  trader_order* pOrder = self->mapOrder->pMethod->xGet(self->mapOrder, user_order_id);
  cOrderType = self->pEngine->pMethod->xGetOrderType(self->pEngine, user_order_id);
  if(TRADER_POSITION_TYPE_OPEN_T1 == cOrderType){
    self->pEngine->pMethod->xGenLocalId(self->pEngine, 
      TRADER_POSITION_TYPE_OPEN_T1_ACTION, sCancelOrderId);
  }else if(TRADER_POSITION_TYPE_OPEN_T2 == cOrderType){
    self->pEngine->pMethod->xGenLocalId(self->pEngine, 
      TRADER_POSITION_TYPE_OPEN_T2_ACTION, sCancelOrderId);
  }else if(TRADER_POSITION_TYPE_CLOSE_T1 == cOrderType){
    self->pEngine->pMethod->xGenLocalId(self->pEngine, 
      TRADER_POSITION_TYPE_CLOSE_T1_ACTION, sCancelOrderId);
  }else if(TRADER_POSITION_TYPE_CLOSE_T2 == cOrderType){
    self->pEngine->pMethod->xGenLocalId(self->pEngine, 
      TRADER_POSITION_TYPE_CLOSE_T2_ACTION, sCancelOrderId);
  }

  self->pEngine->pMethod->xSetTimer(self->pEngine,
    pOrder->InstrumentID, sCancelOrderId, user_order_id);

  return 0;
}

int trader_strategy_release_order(trader_strategy* self, trader_order* order_data)
{

  // �Ӷ�������ɾ��
  trader_order* pOrder = (trader_order*)self->mapOrder->pMethod->xGet(self->mapOrder, order_data->UserOrderLocalID);
  if(pOrder){
    //���汨������
    self->pEngine->pMethod->xSaveOrder(self->pEngine, pOrder);
    // �ͷſռ�
    free(pOrder);
    self->mapOrder->pMethod->xRemove(self->mapOrder, order_data->UserOrderLocalID);
  }

  // �Ӵ��ɽ�������ɾ��
  trader_strategy_order* pStrategyOrder = NULL;
  trader_strategy_order* pIter;

  TAILQ_FOREACH(pIter, &self->listNotFinishedOrder,  next){
    if(!strcmp(pIter->ref, order_data->UserOrderLocalID)){
      pStrategyOrder = pIter;
    }
  }

  if(!pStrategyOrder){
    // û���ҵ�
    CMN_WARN("[%s] listNotFinishedOrder not found!\n", order_data->UserOrderLocalID);
    return -1;
  }
  
  TAILQ_REMOVE(&self->listNotFinishedOrder, pStrategyOrder, next);

  free(pStrategyOrder);

  return 0;
}

int trader_strategy_print_tick(trader_strategy* self)
{
  trader_tick* t1 = &self->oT1Tick;
  trader_tick* t2 = &self->oT2Tick;
  CMN_DEBUG("InstrumentID[%s]\n", t1->InstrumentID);
  CMN_DEBUG("BidPrice1[%lf]\n", t1->BidPrice1);
  CMN_DEBUG("BidVolume1[%d]\n", t1->BidVolume1);
  CMN_DEBUG("AskPrice1[%lf]\n", t1->AskPrice1);
  CMN_DEBUG("AskVolume1[%d]\n", t1->AskVolume1);

  CMN_DEBUG("InstrumentID[%s]\n", t2->InstrumentID);
  CMN_DEBUG("BidPrice1[%lf]\n", t2->BidPrice1);
  CMN_DEBUG("BidVolume1[%d]\n", t2->BidVolume1);
  CMN_DEBUG("AskPrice1[%lf]\n", t2->AskPrice1);
  CMN_DEBUG("AskVolume1[%d]\n", t2->AskVolume1);

  return 0;
}



trader_strategy* trader_strategy_new()
{
  trader_strategy* self = (trader_strategy*)malloc(sizeof(trader_strategy));

  self->pMethod = trader_strategy_method_get();


  return self;

}

void trader_strategy_free(trader_strategy* self)
{
  // ��������
  if(self->mapOrder){
    cmn_util_map_free(self->mapOrder);
  }
  
  // �ƻ�����
  if(self->mapOrder){
    cmn_util_map_free(self->mapStrategyPlan);
  }

  // T1�ɽ�����
  if(self->mapOrder){
    cmn_util_map_free(self->mapHalfTraded);
  }  
  
  if(self){
    free(self);
  }

}



