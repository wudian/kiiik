#pragma once
#include "ThostFtdcUserApiStruct.h"
#include <map>

using namespace std;
typedef string StrategyID;
#define MAX_CODE_SIZE 100


#pragma pack(push,1)
struct orderkey
{
	///前置编号..
	TThostFtdcFrontIDType	FrontID;
	///会话编号..
	TThostFtdcSessionIDType	SessionID;
	///报单引用..
	TThostFtdcOrderRefType	OrderRef;
	bool operator<(const orderkey& rhs) const
	{
		if (FrontID != rhs.FrontID)
		{
			return (FrontID < rhs.FrontID);
		}
		else if (SessionID != rhs.SessionID)
		{
			return (SessionID < rhs.SessionID);
		}
		else
		{
			return (strcmp(OrderRef, rhs.OrderRef)<0);
		}

	}

	orderkey(){ memset(this, 0, sizeof(orderkey)); }
};

struct tradekey
{
	///交易所代码..
	TThostFtdcExchangeIDType	ExchangeID;
	///本地报单编号..
	TThostFtdcOrderLocalIDType	OrderLocalID;
	///交易所交易员代码..
	TThostFtdcTraderIDType	TraderID;
	bool operator<(const tradekey& rhs) const
	{
		if (strcmp(ExchangeID, rhs.ExchangeID) != 0)
		{
			return (strcmp(ExchangeID, rhs.ExchangeID)<0);
		}
		else if (strcmp(TraderID, rhs.TraderID) != 0)
		{
			return (strcmp(TraderID, rhs.TraderID)<0);
		}
		else
		{
			return (strcmp(OrderLocalID, rhs.OrderLocalID)<0);
		}
	}

	tradekey(){ memset(this, 0, sizeof(tradekey)); }
};

struct tradeidkey
{
	///交易日
	TThostFtdcDateType	TradingDay;
	///成交编号
	TThostFtdcTradeIDType	TradeID;

	tradeidkey(){ memset(this, 0, sizeof(tradeidkey)); }
	bool operator<(const tradeidkey& rhs) const
	{
		if (strcmp(TradingDay, rhs.TradingDay) != 0)
		{
			return (strcmp(TradingDay, rhs.TradingDay) < 0);
		}
		else
		{
			return (strcmp(TradeID, rhs.TradeID) < 0);
		}
	}
};

struct poskey//即使区分持仓，也是区分参数的key..
{
	//策略ID..
	StrategyID strID;
	///合约代码..
	TThostFtdcInstrumentIDType	InstrumentID;
	bool operator<(const poskey& rhs) const
	{
		if (strID != rhs.strID)
		{
			return (strID<rhs.strID);
		}
		else
		{
			return (strcmp(InstrumentID, rhs.InstrumentID)<0);
		}

	}
	poskey(){ memset(this, 0, sizeof(poskey)); }
};

struct inputorder //报单结构体..
{
	///策略ID..
	StrategyID strID;
	///合约代码..
	TThostFtdcInstrumentIDType InstrumentID;
	///报单价格..
	TThostFtdcPriceType LimitPrice;
	///报单数量..
	TThostFtdcVolumeType Vol;
	///报单方向..
	TThostFtdcDirectionType Direction;
	///组合开平仓标志..
	TThostFtdcCombOffsetFlagType CombOffsetFlag;
	///请求编号..
	TThostFtdcRequestIDType	RequestID;
	///追单标志..
	int Purchasing;
	///已经追单次数..
	int PurNum;
	///最大追单次数..
	int MaxPurching;
	///追单时间间隔/秒..
	double WaitSecond;
	///单位价格变动值..
	double UnitTick;
	///是否改变开平方向标志.
	bool changeflag;

	inputorder(){ memset(this, 0, sizeof(inputorder)); }
};

struct order_t
{
	///策略ID..
	StrategyID strID;
	///投资者代码..
	TThostFtdcInvestorIDType	InvestorID;
	///合约代码..
	TThostFtdcInstrumentIDType	InstrumentID;
	///报单引用..
	TThostFtdcOrderRefType	OrderRef;
	///买卖方向..
	TThostFtdcDirectionType	Direction;
	///组合开平标志..
	TThostFtdcCombOffsetFlagType	CombOffsetFlag;
	///价格..
	TThostFtdcPriceType	LimitPrice;
	///数量..
	TThostFtdcVolumeType	VolumeTotalOriginal;
	///报单状态..
	TThostFtdcOrderStatusType	OrderStatus;
	///委托时间..
	TThostFtdcTimeType	InsertTime;
	///前置编号..
	TThostFtdcFrontIDType	FrontID;
	///会话编号..
	TThostFtdcSessionIDType	SessionID;
	///交易所代码..
	TThostFtdcExchangeIDType	ExchangeID;
	///本地报单编号..
	TThostFtdcOrderLocalIDType	OrderLocalID;
	///交易所交易员代码..
	TThostFtdcTraderIDType	TraderID;
	///成交数量..
	TThostFtdcVolumeType VolumeTraded;
	///剩余数量..
	TThostFtdcVolumeType VolumeTotal;
	///请求编号..
	TThostFtdcRequestIDType	RequestID;
	///追单标志，0不追；1追..
	int                     Purchasing;
	///已经追单次数..
	int PurNum;
	///最大追单次数..
	int MaxPurching;
	///本地报单时间..
	double LocalSec;
	///追单时间间隔/秒..
	double WaitSecond;
	///单位价格变动值..
	double UnitTick;
	///是否经过转换 用于处理平今手续费过高..
	bool Change;

	order_t(){ memset(this, 0, sizeof(order_t)); }
};

struct trade_t
{
	///策略ID..
	StrategyID strID;
	///投资者代码..
	TThostFtdcInvestorIDType	InvestorID;
	///合约代码..
	TThostFtdcInstrumentIDType	InstrumentID;
	///交易所代码..
	TThostFtdcExchangeIDType	ExchangeID;
	///买卖方向..
	TThostFtdcDirectionType	Direction;
	///本地报单编号..
	TThostFtdcOrderLocalIDType	OrderLocalID;
	///交易所交易员代码..
	TThostFtdcTraderIDType	TraderID;
	///开平标志..
	TThostFtdcOffsetFlagType	OffsetFlag;
	///投机套保标志..
	TThostFtdcHedgeFlagType	HedgeFlag;
	///价格..
	TThostFtdcPriceType	Price;
	///数量..
	TThostFtdcVolumeType	Volume;
	///成交时间..
	TThostFtdcTimeType	TradeTime;
	///交易日..
	TThostFtdcDateType	TradingDay;

	trade_t(){ memset(this, 0, sizeof(trade_t)); }
};

struct position_t
{
	///策略ID..
	StrategyID strID;
	///投资者代码..
	TThostFtdcInvestorIDType	InvestorID;
	///合约代码..
	TThostFtdcInstrumentIDType	InstrumentID;
	///多头持仓成本..
	TThostFtdcMoneyType	PosCostLong;
	///上日多头持仓..
	TThostFtdcVolumeType	YdPosLong;
	///今日多头持仓..
	TThostFtdcVolumeType	PosLong;
	///空头持仓成本..
	TThostFtdcMoneyType	PosCostShort;
	///上日空头持仓..
	TThostFtdcVolumeType	YdPosShort;
	///今日空头持仓..
	TThostFtdcVolumeType	PosShort;
	///多头开仓冻结..
	TThostFtdcVolumeType	OLongFrozen;
	///空头开仓冻结..
	TThostFtdcVolumeType	OShortFrozen;
	///多头平仓冻结..
	TThostFtdcVolumeType	CLongFrozen;
	///空头平仓冻结..
	TThostFtdcVolumeType	CShortFrozen;
	///保证金率.
	TThostFtdcRatioType	MarginRate;
	///最小变动价位.
	TThostFtdcPriceType UnitTick;
	///合约数量乘数.
	TThostFtdcVolumeMultipleType	Multiple;

	position_t(){ memset(this, 0, sizeof(position_t)); }
	bool operator==(const position_t &rhs) { return YdPosLong == rhs.YdPosLong && PosLong == rhs.PosLong && YdPosShort == rhs.YdPosShort && PosShort == rhs.PosShort; }
	bool operator!=(const position_t &rhs) { return !(*this == rhs); }
};

struct compar  //通用参数..
{
	position_t *pos; //策略持仓信息..
	int pos_num;//持仓合约的个数..
	//double MarginRate;//保证金比例..
	double MoneyRate;//分配的资金比例..
	//double UnitTick;//单位价格..
	//int Multiple;//合约乘数..
	int RequestID;//请求编号..
	bool changeflag;//是否需要调整开平标志.
};

enum postype :char
{
	InstPos = '0',//合约持仓..
	StrPos = '1',//策略持仓..

};

enum signtype :char //交易信号类型..
{
	CTP_Buy = '0',//买开..
	CTP_Sell = '1',//卖平..
	CTP_Short = '2',//卖开..
	CTP_Cover = '3',//买平..
	CTP_None = 'n',//非交易信号..
	CTP_All = 'c',// 全部撤单..
};

enum membertype
{
	ord = 0,//联合体存储order..
	trd = 1,//联合体存储trade..
};

struct TR
{
	membertype flag;
	union ot
	{
		CThostFtdcOrderField order;
		CThostFtdcTradeField trade;
	} tuo;
};

struct kline
{
	char updatetime[9];//时间.
	double open;//开盘价.
	double high;//最高价.
	double low;//最低价.
	double close;//收盘价.
	int  vol;//成交量.
};

struct TradeidStraidItem
{
	TThostFtdcDateType TradingDay;
	TThostFtdcTradeIDType TradeID;
	StrategyID StraId;
};

#pragma pack(pop)

typedef map<orderkey, order_t*> orderinfo_map;
typedef map<tradekey, trade_t> tradeinfo_map;
typedef map<string, position_t> instpos_map;
typedef map<poskey, position_t> strpos_map;
typedef map<string, string> cfg_map;
typedef map<tradekey, orderkey> trade2order_map;//orderkey VS tradekey..
typedef map<tradeidkey, StrategyID> tradeid2strategyid_map;
typedef map<string, bool>open_permit;