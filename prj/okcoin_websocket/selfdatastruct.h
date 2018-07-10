#pragma once
#include <stdlib.h>
#include <map>
#include <vector>
#include "baselib/SimpleMath.h"

using namespace std;
using namespace baselib;
//using namespace lidian::common;

typedef char String32[32];
typedef char StringDepth[512];
#define DEPTH 5
#define EXCHANGE_OKCOIN "OKCOIN"
#define SYMBOL_BTC "btc"
#define SYMBOL_LTC "ltc"
#define SYMBOL_ETH "eth"
#define SYMBOL_ETC "etc"
#define SYMBOL_BCC "bcc"

#define SYMBOL_NUM 5
#define INDEX_BTC 0
#define INDEX_LTC 1
#define INDEX_ETH 2
#define INDEX_ETC 3
#define INDEX_BCC 4

#define DrTickDataFields 35 
#define DrKlineDataFields 9

#pragma pack(push,1)



struct DrKlineData
{
	String32 symbol;
	String32 exchange;

	long long timestamp;//时间戳.
	String32 datetime;//20170906 10:09:08 根据timestamp换算而来.
	double open;//开盘价.
	double high;//最高价.
	double low;//最低价.
	double close;//收盘价.
	double vol;//成交量.

	DrKlineData() { memset(this, 0, sizeof(DrKlineData)); strcpy_s(exchange, EXCHANGE_OKCOIN); }
};

/*{"high":1970, "vol" : 553763.718, "last" : 1771, "low" : 1555, "buy" : 1771.01, "change" : 0.63, 
"sell" : 1779, "dayLow" : 1555, "close" : 1771, "dayHigh" : 1895, "open" : 1766, "timestamp" : 1504603013085}*/
struct DrTickData
{
	String32 symbol;
	String32 exchange;

	double last;//最新价.
	double vol;//成交量(最近的24小时).
	double buy;//买一价.
	double sell;//卖一价.
	double open;
	double high;//最高价.
	double low;//最低价.
	double close;
	double dayHigh;
	double dayLow;
	double change;
	long long timestamp;//时间戳.
	String32 datetime;//20170906 10:09:08 根据timestamp换算而来.
	double bidPrice[DEPTH];
	double bidVol[DEPTH];
	double askPrice[DEPTH];
	double askVol[DEPTH];
	/*StringDepth bids;
	StringDepth asks;*/

	DrTickData() { memset(this, 0, sizeof(DrTickData)); strcpy_s(exchange, EXCHANGE_OKCOIN); }
	bool operator==(const DrTickData &rhs){
		for (int i = 0; i < DEPTH; ++i){
			if (PriceUnEqual(bidPrice[i], rhs.bidPrice[i])
				|| PriceUnEqual(bidVol[i], rhs.bidVol[i])
				|| PriceUnEqual(askPrice[i], rhs.askPrice[i])
				|| PriceUnEqual(askVol[i], rhs.askVol[i]))
				return false;
		}
		return PriceEqual(last, rhs.last)
			&& PriceEqual(vol, rhs.vol)
			&& PriceEqual(buy, rhs.buy)
			&& PriceEqual(sell, rhs.sell)
			&& PriceEqual(open, rhs.open)
			&& PriceEqual(high, rhs.high)
			&& PriceEqual(low, rhs.low)
			&& PriceEqual(close, rhs.close)
			&& PriceEqual(dayHigh, rhs.dayHigh)
			&& PriceEqual(dayLow, rhs.dayLow)
			&& PriceEqual(change, rhs.change)
			&& timestamp == rhs.timestamp;
	}
	bool operator!=(const DrTickData &rhs) { return !(*this == rhs); }
};

#pragma pack(pop)
