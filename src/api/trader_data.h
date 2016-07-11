#ifndef _TRADER_DATA_H_
#define _TRADER_DATA_H_

typedef struct trader_tick_def trader_tick;
typedef struct trader_order_def trader_order;
typedef struct trader_trade_def trader_trade;

struct trader_tick_def {
	///��Լ����
	char InstrumentID [31];
	///������
	char	TradingDay [9];
	///����޸�ʱ��
	char UpdateTime [9];
	///����޸ĺ���
	int	UpdateMillisec;
	///�����һ
	double	BidPrice1;
	///������һ
	int	BidVolume1;
	///������һ
	double	AskPrice1;
	///������һ
	int	AskVolume1;
};

///ȫ���ɽ�
#define TRADER_ORDER_OS_ALLTRADED '0'
///���ֳɽ����ڶ�����
#define TRADER_ORDER_OS_PARTTRADEDQUEUEING '1'
///���ֳɽ����ڶ�����
#define TRADER_ORDER_OS_PARTTRADEDNOTQUEUEING '2'
///δ�ɽ����ڶ�����
#define TRADER_ORDER_OS_NOTRADEQUEUEING '3'
///δ�ɽ����ڶ�����
#define TRADER_ORDER_OS_NOTRADENOTQUEUEING '4'
///����
#define TRADER_ORDER_OS_CANCELED '5'
///�����ѱ��뽻����δӦ��
#define TRADER_ORDER_OS_ACCEPTEDNOREPLY '6'
///�����ύʧ��
#define TRADER_ORDER_OS_FAILED '7'

struct trader_order_def {
  // ��Լ����
  char InstrumentID [31];
  // ���ر������
  char UserOrderLocalID[21];
  // ����
  char	Direction;
  // ��ƽ
  char	OffsetFlag;
  ///Ͷ���ױ���־
  char HedgeFlag;
  // �����۸�
  double LimitPrice;
  // ��������
  int VolumeOriginal;
  // �ɽ�����
  int VolumeTraded;
  // ����״̬
  char OrderStatus;
  ///����ʱ��
  char	InsertTime[9];
  // ����ʱ��
  struct timeval CreateTime;
  // �ر�ʱ��
  struct timeval UpdateTime;
  
};

struct trader_trade_def {
  ///��Լ����
  char InstrumentID [31];
  ///���ر������
  char UserOrderLocalID[21];
  ///������
  char	TradingDay[9];
  ///�ɽ�ʱ��
  char	TradeTime[9];
  ///�ɽ��۸�
  double	TradePrice;
  ///�ɽ�����
  int	TradeVolume;
  //�ɽ����
  char TradeID[21];
  // �ر�ʱ��
  struct timeval UpdateTime;
};

#define TRADER_POSITION_BUY '0'
#define TRADER_POSITION_SELL '1'
#define TRADER_POSITION_OPEN '0'
#define TRADER_POSITION_CLOSE '1'

#define TRADER_POSITION_TYPE_OPEN_T1 '0'
#define TRADER_POSITION_TYPE_OPEN_T2 '1'
#define TRADER_POSITION_TYPE_CLOSE_T1 '2'
#define TRADER_POSITION_TYPE_CLOSE_T2 '3'
#define TRADER_POSITION_TYPE_OPEN_T1_ACTION '4'
#define TRADER_POSITION_TYPE_CLOSE_T1_ACTION '5'
#define TRADER_POSITION_TYPE_OPEN_T2_ACTION '6'
#define TRADER_POSITION_TYPE_CLOSE_T2_ACTION '7'


#endif //_TRADER_DATA_H_

