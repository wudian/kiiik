#include "datalib/KlineMerger.h"
#include <QDate>
#include "common/AppLog.h"
#include "common/XmlConfig.h"
#include "common/ExceptionErr.h"
#include "common/DateTime.h"

namespace zhon_gan {
using namespace common;

KlineMerger::KlineMerger(MergerSpi* hdl)
	: spi_(hdl), sum_vol_(0), pre_kline_(NULL)
	, begin_section_idx_(-1), end_section_idx_(-1), push_kline_num_(0)
{
	time_conf_ = new common::XmlConfig(Global::GetInstance()->its_home+"/config/TradingTime.xml");
	if (!time_conf_->Load()){ APP_LOG(LOG_LEVEL_ERROR)<<"TradingTime.xml load fail";return;}

	kline_.volume = -1;
}

/*
void KlineMerger::Reset(){
	sum_vol_ = 0;
	begin_section_idx_ = -1;
	end_section_idx_ = -1;
	push_kline_num_ = 0;
	kline_.clear();
	kline_.volume = -1;
}*/

void KlineMerger::SetInfo(const KlineInfo &info){ 
	info_ = info; 
	SymbolInfoSet::GetFutureTradingTime(info_.symbol, time_conf_, section_time_);
	if (section_time_.size() == 0)
	{
		//ThrowError("获取" << info_.symbol.instrument << "交易时间错误");
		APP_LOG(LOG_LEVEL_ERROR)<<"获取" << info_.symbol.instrument << "交易时间错误";
	}
}

KlineExt2* KlineMerger::GetKline()
{
	if (kline_.volume < 0)
	{
		return NULL;
	}

	return &kline_;
}

void KlineMerger::CalcDayKlineTime(const common::SimpleDateTime& time, KlineExt2* kline)
{
	if (section_time_[0].begin.hour > 18)
	{
		if (time.time.hour > 18)
		{
			kline->b_time.date = time.date;
			kline->b_time.time = section_time_[0].begin;
			kline->e_time.date = time.date;
			NextTradingDay(&kline->e_time.date);
			kline->e_time.time = section_time_.back().end;
		}
		else if (time.time.hour < 3)
		{
			kline->b_time.date = time.date;
			kline->b_time.date.AddDays(-1);
			kline->b_time.time = section_time_[0].begin;
			kline->e_time.date = kline->b_time.date;
			NextTradingDay(&kline->e_time.date);
			kline->e_time.time = section_time_.back().end;
		}
		else
		{
			Date c_date(time.date.year, time.date.month, time.date.day);
			do 
			{
				c_date.AddDays(-1);
			} while (c_date.WeekDay() > 5);
			if (c_date.IsTradingDay(time_conf_))
			{
				kline->b_time.date.year = c_date.year;
				kline->b_time.date.month = c_date.month;
				kline->b_time.date.day = c_date.day;
				kline->b_time.time = section_time_[0].begin;
			} 
			else
			{
				kline->b_time.date = time.date;
				kline->b_time.time = section_time_[1].begin;
			}

			kline->e_time.date = time.date;
			kline->e_time.time = section_time_.back().end;
		}
	}
	else
	{
		kline->b_time.date = time.date;
		kline->b_time.time = section_time_[0].begin;
		kline->e_time.date = time.date;
		kline->e_time.time = section_time_.back().end;
	}
}

bool KlineMerger::GetDayKline(KlineExt2* kline)
{
	if (begin_section_idx_ < 0)
	{
		return false;
	}

	*kline = kline_;
	kline->open = today_open_;
	kline->high = today_high_;
	kline->low = today_low_;
	kline->volume = sum_vol_;
	CalcDayKlineTime(last_time_, kline);

	//成交量为0，价格无效的情况
	if (kline->high < 0.001)
	{
		switch (info_.symbol.product)
		{
		case PRODUCT_FUTURE:
		case PRODUCT_SPOT:
		case PRODUCT_OPTION:
			kline->high = kline->pre_settle_price;
			break;
			
		default:
			kline->high = kline->pre_close;
		}
		kline->open = kline->high;
		kline->low = kline->high;
		kline->close = kline->high;
	}

	return true;
}

void KlineMerger::Reset(const KlineExt2& kline, float vol)
{
	kline_ = kline;
	sum_vol_ = vol;

	//调用该接口一般是先请求K线，后面用tick生成，所以需要设置begin_section_idx_
	for (int i=0; i < section_time_.size(); ++i)
	{
		if (section_time_[i].IsStrictIn(kline.b_time.time))
		{
			begin_section_idx_ = i;
			NextEndTime(); //end_section_idx_在里面计算
			return;
		}
	}

	ThrowError("K线开始交易时间不在交易时间段, " << info_.symbol.instrument << ":" << kline.b_time.time.Str());
}

void KlineMerger::CheckTick(const BaseTick* tick)
{
	if (!tick->date_time.IsValid()) 
	{ 
		ThrowError("invalid time: " << tick->symbol.instrument <<", " << tick->date_time.Str());
	}

	if (tick->date_time < last_time_) 
	{ 
		ThrowError("error time: "<< tick->symbol.instrument <<" , " << tick->date_time.Str() << " < " << last_time_.Str());
	}

	if (tick->last_price < 0.0001 || tick->volume < sum_vol_) 
	{ 
		ThrowError("error price or volume: "<< tick->symbol.instrument <<", price="  << tick->last_price << ", sum_vol=" << sum_vol_ << ", tick_vol=" << tick->volume);
	}

	for (int i=0; i < section_time_.size(); ++i)
	{
		if (section_time_[i].IsIn(tick->date_time.time))
		{
			return;
		}
	}

	ThrowError("not in the trading time: " << tick->symbol.instrument <<", " << tick->date_time.Str());
}

void KlineMerger::PushTick(const BaseTick* tick)
{
	//清除脏数据,保证tick交易时间段正常
	CheckTick(tick);

	if (TimeToAdd(tick->date_time))
	{
		do 
		{
			AddWithTick(tick);
		} while (TimeToAdd(tick->date_time));
	}
	else {
		UpdateWithTick(tick);
	}

	last_time_ = tick->date_time;
	//存储日线数据
	today_open_ = tick->today_open;
	today_high_ = tick->today_high;
	today_low_ = tick->today_low;
}

bool KlineMerger::InitKlineInDay(const common::SimpleDateTime* date_time)
{
	sum_vol_ = 0;
	kline_.b_time.date = date_time->date;
	if (date_time->time.hour < 3)
	{
		kline_.b_time.date.AddDays(-1);
	}

	begin_section_idx_ = 0;
	if (section_time_[0].IsIn(date_time->time))
	{
		kline_.b_time.time = section_time_[0].begin;
	}
	else
	{
		if (section_time_[0].begin.hour > 18) //有夜盘
		{
			begin_section_idx_ = 1;
			kline_.b_time.time = section_time_[1].begin;
		} 
		else //无夜盘，取白天的开盘时间
		{
			kline_.b_time.time = section_time_[0].begin;
		}
	}

	return true;
}

bool KlineMerger::NextStartTime(const common::SimpleDateTime* date_time)
{
	//只有在第一个tick（e_time为初始值）和e_time为交易区间终点时，
	//将b_time设为交易区间的起点。初始值和最后一个交易区间终点相等表示
	//新的交易日，需要将总成交量置为0
	//由外部保证一定在交易区间里面
	if (begin_section_idx_ < 0 || (end_section_idx_ == (section_time_.size() - 1)))
	{
		return InitKlineInDay(date_time);
	} 

	if (end_section_idx_ < 0)
	{
		begin_section_idx_ = -1 - end_section_idx_;
		kline_.b_time = kline_.e_time;	//常规情况
	}
	else
	{
		kline_.b_time.date = kline_.e_time.date;
		if (end_section_idx_ == 0 && section_time_[0].begin.hour > 18) //夜盘结尾
		{
			//考虑补白天数据
			if (kline_.e_time.time.hour < 3)
			{
				kline_.b_time.date.AddDays(-1);
			}
			NextTradingDay(&kline_.b_time.date);
		} 

		begin_section_idx_ = end_section_idx_ + 1;
		kline_.b_time.time = section_time_[end_section_idx_ + 1].begin;
	}

	return false;
}

void KlineMerger::NextTradingDay(common::SimpleDate* date)
{
	Date c_date(date->year, date->month, date->day);
	c_date = c_date.NextTradingDay(time_conf_);
	date->year = c_date.year;
	date->month = c_date.month;
	date->day = c_date.day;
}

void KlineMerger::CalcSecOrMinEndTime(char dim, int plus_cont)
{
	common::SimpleTime e_time = kline_.b_time.time;
	common::SimpleTime pre_time = e_time;
	int plus_day = dim == DIMENSION_SECOND ? e_time.AddSec(plus_cont) : e_time.AddMin(plus_cont);
	int i;
	bool adjust_date = false;
	for (i=begin_section_idx_; i<section_time_.size() - 1; ++i)
	{
		if (section_time_[i].IsIn(e_time))
		{
			end_section_idx_ = e_time == section_time_[i].end ? i : (-1 - i);
			break;
		}

		if (i == 0 && section_time_[0].begin.hour > 18)
		{
			adjust_date = true;
		}

		e_time = section_time_[i + 1].begin;
		if (dim == DIMENSION_SECOND)
		{
			int sec_sec = (((section_time_[i].end.hour - pre_time.hour) % 24) * 60 + (section_time_[i].end.minute - pre_time.minute)) * 60 +
				section_time_[i].end.sec - pre_time.sec;
			plus_cont -= sec_sec;
			e_time.AddSec(plus_cont);
		} 
		else
		{
			int sec_min = ((section_time_[i].end.hour - pre_time.hour) % 24) * 60 + section_time_[i].end.minute - pre_time.minute;
			plus_cont -= sec_min;
			e_time.AddMin(plus_cont);
		}
		pre_time = section_time_[i + 1].begin;
	}

	if (i == section_time_.size() - 1)
	{
		if (!section_time_.back().IsIn(e_time))
		{
			end_section_idx_ = section_time_.size() - 1;
			e_time = section_time_.back().end;
		}
		else
		{
			end_section_idx_ = e_time == section_time_.back().end ? (section_time_.size() - 1) : (-section_time_.size());
		}
	}

	kline_.e_time.date = kline_.b_time.date;
	kline_.e_time.time = e_time;

	if (adjust_date)
	{
		if (kline_.b_time.time.hour < 5)
		{
			kline_.e_time.date.AddDays(-1);
		}
		NextTradingDay(&kline_.e_time.date);
	} 
	else
	{
		if (plus_day > 0)
		{
			//夜盘24点交界，往后延一天
			kline_.e_time.date.AddDays(1);
		}
	}
}

void KlineMerger::NextEndTime()
{
	switch (info_.dimension)
	{
	case DIMENSION_SECOND:
		CalcSecOrMinEndTime(DIMENSION_SECOND, info_.dimen_cnt);
		break;

	case DIMENSION_MINUTE:
		CalcSecOrMinEndTime(DIMENSION_MINUTE, info_.dimen_cnt);
		break;

	case DIMENSION_HOUR:
		CalcSecOrMinEndTime(DIMENSION_MINUTE, info_.dimen_cnt * 60);
		break;

	default:
		end_section_idx_ = section_time_.size() - 1;
		kline_.e_time.date = kline_.b_time.date;
		kline_.e_time.time = section_time_.back().end;
		if (kline_.e_time.time < kline_.b_time.time)	//将日期设为下一个交易日
		{
			NextTradingDay(&kline_.e_time.date);
		}
	}
}

void KlineMerger::UpdateWithTick(const BaseTick* tick)
{
	if (kline_.high < tick->last_price)
	{
		kline_.high = tick->last_price;
	}
	if (kline_.low > tick->last_price)
	{
		kline_.low = tick->last_price;
	}
	kline_.close = tick->last_price;
	kline_.volume += tick->volume - sum_vol_;
	sum_vol_ = tick->volume;

	//其他几个都是昨日的变量，不会变，所以不用重新赋值
	kline_.amount = tick->amount;
	if (tick->symbol.product == PRODUCT_FUTURE)
	{
		FutureTick* future_tick = (FutureTick*)tick;
		kline_.open_interest = future_tick->open_interest;
	}
	else if (tick->symbol.product == PRODUCT_SPOT)
	{
		SpotTick* spot_tick = (SpotTick*)tick;
		kline_.open_interest = spot_tick->open_interest;
	}
	else if (tick->symbol.product == PRODUCT_OPTION)
	{
		OptionTick* option_tick = (OptionTick*)tick;
		kline_.open_interest = option_tick->open_interest;
	}

	if (spi_)
	{
		spi_->OnUpdateKline(this, &kline_);
	}
}

void KlineMerger::AddWithTick(const BaseTick* tick)
{
	if (pre_kline_)
	{
		memcpy(pre_kline_, &kline_, sizeof(KlineExt2));
	}

	kline_.volume = 0;
	if (NextStartTime(&tick->date_time)) 
	{
		//新的交易日
		kline_.open = tick->last_price;
		kline_.high = tick->last_price;
		kline_.low = tick->last_price;
		kline_.close = tick->last_price;

		kline_.amount = 0;
		kline_.pre_close = tick->pre_close;
		switch (tick->symbol.product)
		{
		case PRODUCT_STOCK:
		case PRODUCT_FUND:
			{
				StockTick* stock_tick = (StockTick*)tick;
				kline_.up_limit = stock_tick->up_limit;
				kline_.drop_limit = stock_tick->drop_limit;
			}
			break;

		case PRODUCT_FUTURE:
			{
				FutureTick* future_tick = (FutureTick*)tick;
				kline_.up_limit = future_tick->up_limit;
				kline_.drop_limit = future_tick->drop_limit;
				kline_.pre_settle_price = future_tick->pre_settlement;
				kline_.pre_open_interest = future_tick->pre_open_interest;
			}
			break;

		

		default:
			break;
		}

		sum_vol_ = 0;
	}
	else
	{
		kline_.open = kline_.close;
		kline_.high = kline_.close;
		kline_.low = kline_.close;
	}
	NextEndTime();
	//tick落在新加的K线内
	if ((end_section_idx_ < 0 && tick->date_time < kline_.e_time) || 
		(end_section_idx_ >= 0 && tick->date_time.date <= kline_.e_time.date && 
		!TradeSectionTime::GreaterWithoutMilsec(tick->date_time.time, kline_.e_time.time)))
	{
		kline_.open = tick->last_price;
		kline_.high = tick->last_price;
		kline_.low = tick->last_price;
		kline_.close = tick->last_price;

		kline_.volume = tick->volume - sum_vol_;
		sum_vol_ = tick->volume;

		kline_.amount = tick->amount;
		if (tick->symbol.product == PRODUCT_FUTURE)
		{
			FutureTick* future_tick = (FutureTick*)tick;
			kline_.open_interest = future_tick->open_interest;
		}
		else if (tick->symbol.product == PRODUCT_OPTION)
		{
			OptionTick* option_tick = (OptionTick*)tick;
			kline_.open_interest = option_tick->open_interest;
		}
		else if (tick->symbol.product == PRODUCT_SPOT)
		{
			SpotTick* spot_tick = (SpotTick*)tick;
			kline_.open_interest = spot_tick->open_interest;
		}
	}

	if (spi_)
	{
		spi_->OnAddKline(this, &kline_);
	}
}

void KlineMerger::PushKline(KlineExt1* kline)
{
	//插入K线没有对e_time进行计算，用最新K线的e_time来赋值，所以此处的e_time是严格递增的
	if (kline->e_time < kline_.e_time)
	{
		ThrowError("error time, "<< info_.symbol.instrument <<" : " << kline->e_time.Str() << " < " << kline_.e_time.Str());
	}

	if (IsNewKline(kline->e_time))
	{
		if (pre_kline_)
		{
			memcpy(pre_kline_, &kline_, sizeof(KlineExt2));
		}

		memcpy(&kline_, kline, sizeof(KlineExt1));
		if (spi_)
		{
			spi_->OnAddKline(this, &kline_);
		}
	}
	else
	{
		if (kline_.high < kline->high)
		{
			kline_.high = kline->high;
		}
		if (kline_.low > kline->low)
		{
			kline_.low = kline->low;
		}
		kline_.close = kline->close;
		kline_.volume += kline->volume;
		kline_.amount = kline->amount;
		kline_.pre_close = kline->pre_close;
		kline_.e_time = kline->e_time;
		kline_.up_limit = kline->up_limit;
		kline_.drop_limit = kline->drop_limit;
		if (spi_)
		{
			spi_->OnUpdateKline(this, &kline_);
		}
	}
}

void KlineMerger::PushKline(KlineExt2* kline, bool end)
{
	//插入K线没有对e_time进行计算，用最新K线的e_time来赋值，所以此处的e_time是严格递增的
	if (kline->e_time < kline_.e_time)
	{
		ThrowError("error time, "<< info_.symbol.instrument <<" : " << kline->e_time.Str() << " < " << kline_.e_time.Str());
		//APP_LOG(LOG_LEVEL_ERROR)<<"error time, "<< info_.symbol.instrument <<" : " << kline->e_time.Str() << " < " << kline_.e_time.Str();
		//return;
	}

	if (IsNewKline(kline->e_time))
	{
		if (pre_kline_)
		{
			memcpy(pre_kline_, &kline_, sizeof(KlineExt2));
		}

		memcpy(&kline_, kline, sizeof(KlineExt2));
		if (spi_)
		{
			spi_->OnAddKline(this, &kline_);
		}
	}
	else
	{
		if (kline_.high < kline->high)
		{
			kline_.high = kline->high;
		}
		if (kline_.low > kline->low)
		{
			kline_.low = kline->low;
		}
		kline_.close = kline->close;
		kline_.volume += kline->volume;
		kline_.amount = kline->amount;
		kline_.pre_close = kline->pre_close;
		kline_.e_time = kline->e_time;
		kline_.up_limit = kline->up_limit;
		kline_.drop_limit = kline->drop_limit;
		kline_.open_interest = kline->open_interest;
		kline_.pre_settle_price = kline->pre_settle_price;
		kline_.pre_open_interest = kline->pre_open_interest;
		if (spi_)
		{
			spi_->OnUpdateKline(this, &kline_);
			if (end){
				if (pre_kline_)
				{
					memcpy(pre_kline_, &kline_, sizeof(KlineExt2));
				}
				if (spi_)
				{
					spi_->OnAddKline(this, &kline_);
				}
			}
		}
	}
}

bool KlineMerger::CalcKlineNum(int num, bool is_min)
{
	if (push_kline_num_ == 0 || kline_.e_time.time == section_time_.back().end)	//第一根K线
	{
		push_kline_num_ = 1;
		return true;
	}
	if (is_min && kline_.e_time.time == section_time_.back().end)
	{
		push_kline_num_ = 1;
		return true;
	}
	push_kline_num_++;
	if (push_kline_num_ > num)
	{
		push_kline_num_ = 1;
		return true;
	}

	return false;
}

bool KlineMerger::IsNewKline(const SimpleDateTime& date_time)
{
	switch (info_.dimension)
	{
	case DIMENSION_MINUTE:
		if (1 == info_.dimen_cnt) { ThrowError("不能用1分钟K线去生成1分钟K线"); }
		return CalcKlineNum(info_.dimen_cnt, true);

	case DIMENSION_HOUR:
		return CalcKlineNum(info_.dimen_cnt * 60, true);

	case DIMENSION_DAY:
		if (1 == info_.dimen_cnt) //用1分钟线去合成1日线
		{
			if (push_kline_num_ == 0) //第一根
			{
				push_kline_num_ = 1;
				return true;
			}

			//新的一个交易日
			if (kline_.e_time.time == section_time_.back().end)
			{
				return true;
			}
		} 
		else //从1日线去合成多日线
		{
			return CalcKlineNum(info_.dimen_cnt, false);
		}

	case DIMENSION_WEEK:
		{
			if (1 != info_.dimen_cnt) { ThrowError("周线只支持维数1"); }
			QDate date1(date_time.date.year, date_time.date.month, date_time.date.day);
			QDate date2(kline_.e_time.date.year, kline_.e_time.date.month, kline_.e_time.date.day);
			return date1.weekNumber() != date2.weekNumber();
		}

	case DIMENSION_MONTH:
		if (1 != info_.dimen_cnt) { ThrowError("月线只支持维数1"); }
		return date_time.date.year != kline_.e_time.date.year || date_time.date.month != kline_.e_time.date.month;

	case DIMENSION_YEAR:
		if (1 != info_.dimen_cnt) { ThrowError("年线只支持维数1"); }
		return date_time.date.year != kline_.e_time.date.year;

	default:
		ThrowError("未知K线维度:" << info_.dimension);
	}
}

void KlineMerger::ExtendToClose(bool is_night)
{
	if (1 != info_.dimen_cnt || info_.dimension != DIMENSION_MINUTE) 
	{ 
		ThrowError("该接口只支持1分钟K线"); 
	}
	if (begin_section_idx_ < 0) { return; }

	if (is_night)
	{
		if (section_time_[0].begin.hour < 18) { return; }

		if (end_section_idx_ != 0)
		{
			SimpleDateTime end_date_time;
			end_date_time.time = section_time_[0].end;
			end_date_time.date = last_time_.date;
			if (end_date_time < last_time_)
			{
				end_date_time.date.AddDays(1);
			}

			kline_.open = kline_.close;
			kline_.high = kline_.close;
			kline_.low = kline_.close;
			kline_.volume = 0;
			do 
			{
				NextStartTime(&end_date_time);
				NextEndTime();
				if (spi_) { spi_->OnAddKline(this, &kline_); }
			} while (end_section_idx_ != 0);
		}
	} 
	else
	{
		if (last_time_.time.hour > 18) { return; }
		if (end_section_idx_ != (section_time_.size() - 1))
		{
			SimpleDateTime end_date_time;
			end_date_time.time = section_time_.back().end;
			end_date_time.date = last_time_.date;
			if (end_date_time < last_time_)
			{
				return;
			}

			kline_.open = kline_.close;
			kline_.high = kline_.close;
			kline_.low = kline_.close;
			kline_.volume = 0;
			do 
			{
				NextStartTime(&end_date_time);
				NextEndTime();
				if (spi_) { spi_->OnAddKline(this, &kline_); }
			} while (end_section_idx_ != (section_time_.size() - 1));
		}
	}
}


}