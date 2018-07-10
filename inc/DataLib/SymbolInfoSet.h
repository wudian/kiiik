#pragma once

#include <string>
#include <vector>
#include <map>
#include <set>
#include "datalib/MarketDefine.h"
#include "datalib/DataServerStruct.h"
#include "common/Global.h"
#include "common/SpinLock.h"
#include "common/XmlConfig.h"
#include "common/SimpleDateTime.h"

namespace zhon_gan{

struct TradeSectionTime 
{
	common::SimpleTime begin;
	common::SimpleTime end;

	//bool IsLess(const common::SimpleTime& time)
	//{
	//	if (begin.hour > 20) { return time.hour >= 18 && time < begin; }
	//	else { return time < begin; }
	//}
	//bool IsGreater(const common::SimpleTime& time)
	//{
	//	if (begin.hour > 20) 
	//	{ 
	//		return time >=end && time.hour < 18;
	//	}
	//	else { return time >= end; }
	//}

	static bool GreaterWithoutMilsec(const common::SimpleTime& time1, common::SimpleTime& time2)
	{
		return time1.hour > time2.hour || (time1.hour == time2.hour && time1.minute > time2.minute)
			|| (time1.hour == time2.hour && time1.minute == time2.minute && time1.sec > time2.sec);
	}

	bool IsIn(const common::SimpleTime& time)
	{
		if (begin < end) { return time >= begin && !GreaterWithoutMilsec(time, end); }
		else { return time >= begin || !GreaterWithoutMilsec(time, end); }
	}

	bool IsStrictIn(const common::SimpleTime& time)
	{
		if (begin < end) { return time >= begin && time < end; }
		else { return time >= begin || time < end; }
	}

	static common::SimpleTime StrToTime(const std::string& str)
	{
		int hour, minute;
		sscanf(str.c_str(), "%d:%02d", &hour, &minute);
		return common::SimpleTime(hour, minute);
	}

	TradeSectionTime(common::SimpleTime& b, common::SimpleTime& e) : begin(b), end(e) {}
	TradeSectionTime() {}
};

class DATALIB_API SymbolInfoSet
{
public:
	SymbolInfoSet(void);
	~SymbolInfoSet(void);

	bool Init(std::string& err);
	void Deinit();
	const std::vector<Symbol>& FutureSymbols() { return future_symbols_; }
	const std::vector<Symbol>& NightFutureSymbols() { return night_future_symbols_; }

	std::map<std::string, std::string>& SymbolNames() { return symbol_names_; }

	double GetPriceTick(const Symbol& sym);
	int GetVolMulti(const Symbol& sym);

	ExchangeIdType GetFutureExchange(const std::string& inst);


	std::string GetProductName(const char* inst);

	
	//static void GetTradingTime(const Symbol& sym, common::XmlConfig* conf, std::vector<TradeSectionTime>& times);

	static SymbolInfoSet* GetInstance();
	
	static void SymbolInfoSet::GetFutureTradingTime(const Symbol& sym, common::XmlConfig* conf, std::vector<TradeSectionTime>& times);


private:
	bool IsNightTrading(Symbol& sym, common::XmlConfig* settings);
	void InseerFutureInfo(Symbol& sym, InstrumentInfo& info);

	bool GetInfo(const Symbol& sym, InstrumentInfo& info);


	static std::string ExchangeName(ExchangeIdType exchange);
	static void InsertSectionTime(common::XmlNode& node, std::vector<TradeSectionTime>& times);
	
private:
	static SymbolInfoSet* inst_;

	std::vector<Symbol> future_symbols_;	//所有合约.
	std::vector<Symbol> night_future_symbols_;	//夜盘合约.
	std::map<std::string, std::string> symbol_names_;//12_IC1705 -> 500股指1705
	//std::map<std::string, std::string> product_names_;
	std::map<std::string, InstrumentInfo> future_info_;//key为IC
	
	bool is_init_;

	common::SpinLock mutex_;
};

}


