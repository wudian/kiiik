#pragma once

#include <vector>
#include "datalib/DataServerStruct.h"
#include "common/Global.h"
#include "datalib/SymbolInfoSet.h"

namespace zhon_gan {

class KlineMerger;
class MergerSpi 
{
public:
	virtual void OnAddKline(KlineMerger* merger, const KlineExt2* kline) = 0;
	virtual void OnUpdateKline(KlineMerger* merger, const KlineExt2* kline) = 0;
};

/*
使用例子:
class MyHdl : public MergerSpi { ...}
MyHdl *hdl = new MyHdl;

KlineInfo kline_info;
kline_info.symbol = Symbol(PRODUCT_FUTURE, EXCHANGE_DCE, "i1802");
kline_info.dimension = DIMENSION_MINUTE;
kline_info.dimen_cnt = 1;

KlineMerger *merger = new KlineMerger(hdl);
merger->SetInfo(kline_info);
*/
class DATALIB_API KlineMerger
{
public:
	KlineMerger(MergerSpi* hdl);
	void SetInfo(const KlineInfo &info);
	KlineInfo GetInfo() const { return info_;}


	std::vector<TradeSectionTime>& GetSectionTime() { return section_time_; }
	void Reset(const KlineExt2& kline, float vol); //已对e_time做调整
	void SetConfig(common::XmlConfig* conf) { time_conf_ = conf; }
	void SetPreKline(KlineExt2* pre_kline) { pre_kline_ = pre_kline; }

	void PushTick(const BaseTick* tick);
	void PushKline(KlineExt1* kline);
	void PushKline(KlineExt2* kline, bool end = false);
	void ExtendToClose(bool is_night);
	void NextEndTime();

	
	KlineExt2* GetKline();

	void CalcDayKlineTime(const common::SimpleDateTime& time, KlineExt2* kline);
	bool GetDayKline(KlineExt2* kline);

private:
	void CheckTick(const BaseTick* tick);

	bool NextStartTime(const common::SimpleDateTime* date_time);
	void CalcSecOrMinEndTime(char dim, int plus_cont);
	//void NextEndTime();
	void UpdateWithTick(const BaseTick* tick);
	void AddWithTick(const BaseTick* tick);
	bool CalcKlineNum(int num, bool is_min);
	bool IsNewKline(const common::SimpleDateTime& date_time);

	void NextTradingDay(common::SimpleDate* date);

	bool InitKlineInDay(const common::SimpleDateTime* date_time);

	bool TimeToAdd(const common::SimpleDateTime& date_time)
	{
		return end_section_idx_ < 0 ? date_time >= kline_.e_time : 
			(date_time.date > kline_.e_time.date || TradeSectionTime::GreaterWithoutMilsec(date_time.time, kline_.e_time.time));
	}

private:
	KlineInfo info_;
	KlineExt2 kline_;
	KlineExt2* pre_kline_;
	common::SimpleDateTime last_time_;
	std::vector<TradeSectionTime> section_time_;
	VolumeType sum_vol_;
	MergerSpi* spi_;
	common::XmlConfig* time_conf_;

	int push_kline_num_;

	int begin_section_idx_;
	int end_section_idx_;

	//存储tick的日线数据
	PriceType today_open_;
	PriceType today_high_;
	PriceType today_low_;
};

}