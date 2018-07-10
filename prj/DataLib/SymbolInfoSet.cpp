#include "datalib/SymbolInfoSet.h"
#include <iostream>
#include <fstream>
#include <QSettings>
#include <QStringList>
#include <QDate>
#include "common/Directory.h"
#include "common/SymbolChanger.h"

using namespace std;

namespace zhon_gan{

SymbolInfoSet* SymbolInfoSet::inst_ = NULL;

SymbolInfoSet::SymbolInfoSet(void) : is_init_(false)
{
}

SymbolInfoSet::~SymbolInfoSet(void)
{
}

bool SymbolInfoSet::IsNightTrading(Symbol& sym, common::XmlConfig* settings)
{
	char sym_code[6] = {0};
	sscanf(sym.instrument, "%[^0-9]", sym_code);

	std::string exchange_name = ExchangeName(sym.exchange);
	std::string key = std::string("TradingTimes/") + exchange_name + "/Nights";
	common::XmlNodeVec night_sections = settings->FindChileren(key);
	for (int i=0; i < night_sections.size(); ++i)
	{
		QString insts = night_sections[i].GetValue("InstrumentID").c_str();
		QStringList inst_list = insts.split(',');
		if (inst_list.contains(sym_code))
		{
			return true;
		}
	}

	return false;
}


bool SymbolInfoSet::Init(std::string& err)
{
	if (is_init_) { return true; }

	common::Locker locker(&mutex_);
	if (is_init_) { return true; }

	char* its_home = getenv("ITS_HOME");
	if(NULL == its_home) 
	{ 
		err = "the env_var ITS_HOME is not set";
		return false;
	}

	std::string trading_conf_path = std::string(its_home) + "/config/TradingTime.xml";
	if (!IsDirExist(trading_conf_path.c_str()))
	{
		err = trading_conf_path + " is not exist";
		return false;
	}
	common::XmlConfig settings(trading_conf_path);
	if (!settings.Load())
	{
		err = settings.GetLastError();
		return false;
	}

	std::string path = std::string(its_home) + "/data/FutureTable.txt";
	if (!IsDirExist(path))
	{
		err = path + " is not exist";
		return false;
	}
	ifstream future_file(path.c_str());
	if (!future_file.is_open())
	{
		err = "open file error";
		return false;
	}
	while (!future_file.eof() ) 
	{
		char buf[128] = {0};
		future_file.getline(buf,128);
		InstrumentInfo info;
		info.FromStr(buf);
		if (strlen(info.symbol.instrument) > 0 && strlen(info.symbol.name) > 0)
		{
			Symbol sym(info.symbol.product, info.symbol.exchange, info.symbol.instrument);
			future_symbols_.push_back(sym);
			if (IsNightTrading(sym, &settings))
			{
				night_future_symbols_.push_back(sym);

			}
			char name_key[16] = {0};
			sprintf(name_key, "%c%c_%s", info.symbol.product, info.symbol.exchange, info.symbol.instrument);
			symbol_names_[name_key] = info.symbol.name;
			InseerFutureInfo(sym, info);
		}
	}
	future_file.close();


	is_init_ = true;
	return true;
}

void SymbolInfoSet::Deinit()
{
	common::Locker locker(&mutex_);
	future_symbols_.clear();
	night_future_symbols_.clear();
	symbol_names_.clear();
	future_info_.clear();
	
	is_init_ = false;
}

void SymbolInfoSet::InseerFutureInfo(Symbol& sym, InstrumentInfo& info)
{
	char code_str[6] = {0};
	sscanf(sym.instrument, "%[^0-9]", code_str);

	if (future_info_.find(code_str) == future_info_.end())
	{
		future_info_[code_str] = info;
	}

}

double SymbolInfoSet::GetPriceTick(const Symbol& sym)
{
	InstrumentInfo info;
	if (GetInfo(sym, info))
	{
		return info.price_tick;
	}
}

int SymbolInfoSet::GetVolMulti(const Symbol& sym)
{
	if (sym.product == PRODUCT_FUTURE)
	{
		InstrumentInfo info;
		if (GetInfo(sym, info))
		{
			return info.vol_multi;
		}
		else
		{
			return 0;
		}
	}
	else if (sym.product == PRODUCT_INDEX || sym.product == PRODUCT_STOCK)
	{
		return 1;
	}
	else
	{
		return 1;
	}
}

bool SymbolInfoSet::GetInfo(const Symbol& sym, InstrumentInfo& info)
{
	common::Locker locker(&mutex_);
	if (sym.product != PRODUCT_FUTURE) { return false; }
	char exc_str[6] = {0};
	sscanf(sym.instrument, "%[^0-9]", exc_str);
	std::map<std::string, InstrumentInfo>::iterator iter = future_info_.find(exc_str);
	if (iter == future_info_.end())
	{
		return false;
	}
	info = iter->second;
	return true;
}

ExchangeIdType SymbolInfoSet::GetFutureExchange(const std::string& inst)
{
	common::Locker locker(&mutex_);
	char exc_str[16] = {0};
	sscanf(inst.c_str(), "%[^0-9]", exc_str);
	std::map<std::string, InstrumentInfo>::iterator iter = future_info_.find(exc_str);
	if (iter == future_info_.end())
	{
		return EXCHANGE_OTHER;
	}
	return iter->second.symbol.exchange;
}

SymbolInfoSet* SymbolInfoSet::GetInstance()
{
	if (NULL == inst_) {
		inst_ = new SymbolInfoSet();
	}

	return inst_;
}


std::string SymbolInfoSet::ExchangeName(ExchangeIdType exchange)
{
	switch(exchange)
	{
	case EXCHANGE_SSE:
		return "SSE";
	case EXCHANGE_SZE:
		return "SZE";
	case EXCHANGE_CFFEX:
		return "CFFEX";
	case EXCHANGE_DCE:
		return "DCE";
	case EXCHANGE_CZCE:
		return "CZCE";
	case EXCHANGE_SHFE:
		return "SHFE";
	case EXCHANGE_HK:
		return "HK";
	default:
		return "OTHER";
	}
}

void SymbolInfoSet::InsertSectionTime(common::XmlNode& node, std::vector<TradeSectionTime>& times)
{
	TradeSectionTime trade_time;
	std::string begin_str = node.GetValue("Begin");
	std::string end_str = node.GetValue("End");
	assert(begin_str != "" && end_str != "");
	trade_time.begin = TradeSectionTime::StrToTime(begin_str);
	trade_time.end = TradeSectionTime::StrToTime(end_str);
	times.push_back(trade_time);
}

void SymbolInfoSet::GetFutureTradingTime(const Symbol& sym, common::XmlConfig* conf, std::vector<TradeSectionTime>& times)
{
	times.clear();

	char product[16] = {0};
	sscanf(sym.instrument, "%[^0-9]", product);

	std::string key = std::string("TradingTimes/") + ExchangeName(sym.exchange);
	std::string night_key = key + "/Nights";
	common::XmlNodeVec node_vec = conf->FindChileren(night_key);
	for (int i=0; i < node_vec.size(); ++i)
	{
		QString insts_str = node_vec[i].GetValue("InstrumentID").c_str();
		if (insts_str == "" || insts_str.split(',').contains(product))
		{
			common::XmlNodeVec night_sections = node_vec[i].FindChileren();
			for (int j=0; j < night_sections.size(); ++j)
			{
				InsertSectionTime(night_sections[j], times);
			}
			break;
		}
	}

	common::XmlNodeVec day_nodes = conf->FindChileren(key, "Day");
	for (int i=0; i < day_nodes.size(); ++i)
	{
		QString insts_str = day_nodes[i].GetValue("InstrumentID").c_str();
		if (insts_str == "" || insts_str.split(',').contains(product))
		{
			common::XmlNodeVec day_sections = day_nodes[i].FindChileren();
			for (int j=0; j < day_sections.size(); ++j)
			{
				InsertSectionTime(day_sections[j], times);
			}
			break;
		}
	}

}

}
