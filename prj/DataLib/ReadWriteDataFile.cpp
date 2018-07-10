#include "DataLib/ReadWriteDataFile.h"
#include <sstream>
#include <QFile>
#include "common/AppLog.h"
#include "DataLib/KlineArrayMerger.h"

namespace zhon_gan {

	using namespace common;


std::string CountPeriodName(short dim_count, DimensionType dim) {
	std::stringstream ss;
	ss << dim_count;
	switch(dim) {
	case DIMENSION_TICK:
		return "tick";
	case DIMENSION_SECOND:
		ss << "s";
		break;
	case DIMENSION_MINUTE:
		ss << "m";
		break;
	case DIMENSION_HOUR:
		ss << "h";
		break;
	case DIMENSION_DAY:
		ss << "d";
		break;
	case DIMENSION_WEEK:
		ss << "w";
		break;
	case DIMENSION_MONTH:
		ss << "o"; // 跟分钟重复了，没办法用 o 代表月线.
		break;
	case DIMENSION_YEAR:
		ss << "y";
		break;
	default:
		return "";
	}
	return ss.str();
}

std::string OnePeriodName(DimensionType dim) {
	switch(dim) {
	case DIMENSION_TICK:
	case DIMENSION_SECOND:
		return "tick";
	case DIMENSION_MINUTE:
	case DIMENSION_HOUR:
		return "1m";
	case DIMENSION_DAY:
	case DIMENSION_WEEK:
	case DIMENSION_MONTH:
	case DIMENSION_YEAR:
		return "1d";
	default:
		return "";
	}
}

std::string GetDateFromFile(std::string file_name, bool is_begin) {
	size_t pos = file_name.find_last_of("_"); // 第二个.
	if (is_begin) return file_name.substr(pos+1, 8);
	else return file_name.substr(pos+9, 8);
}

std::string GetDateFromFile(long long mode_data, bool is_begin) {
	stringstream ss; ss << mode_data;
	if (is_begin) return ss.str().substr(0, 8);
	else return ss.str().substr(8, 8);
}

HisDataType GetHisDataType(DimensionType dimension, ProductIdType product) {
	switch(dimension) {
	case DIMENSION_TICK:
		{
			switch (product) 
			{
			case PRODUCT_STOCK:
			case PRODUCT_FUND:
				return HIS_STOCK_TICK;
			case PRODUCT_FUTURE:
				return HIS_FUTURE_TICK;
			case PRODUCT_INDEX:
				return HIS_INDEX_TICK;
			case PRODUCT_SPOT:
				return HIS_SPOT_TICK;
			default:
				return HIS_STOCK_TICK;
			}
		}
		break;
	case DIMENSION_SECOND:
	case DIMENSION_MINUTE:
	case DIMENSION_HOUR:
	case DIMENSION_DAY:
	case DIMENSION_WEEK:
	case DIMENSION_MONTH:
	case DIMENSION_YEAR:
		{
			switch (product) 
			{
			case PRODUCT_FUTURE:
			case PRODUCT_OPTION:
				return HIS_KLINE_EXT_2;
			default:
				return HIS_KLINE_EXT_1;
			}
		}
		break;
	default:
		return HIS_STOCK_TICK;
	}
}

template<class T>
QFile* MapFile(const std::string& path, T** data, int* num)
{
	if (!QFile::exists(path.c_str())) { return NULL; }
	QFile* file = new QFile(path.c_str());
	if (!file->open(QIODevice::ReadOnly))
	{
		APP_LOG(LOG_LEVEL_ERROR) << "open file error: " << path;
		delete file;
		return NULL;
	}
	if (file->size() == 0) {
		*num = 0;
		return file;
	}
	if ((file->size() % sizeof(T) != 0) || ((*data = (T*)file->map(0, file->size())) == NULL))
	{
		APP_LOG(LOG_LEVEL_ERROR) << "file size or map error: " << path;
		file->close();
		delete file;
		return NULL;
	}

	*num = file->size() / sizeof(T);
	return file;
}

template<typename T>
const common::SimpleDateTime& GetDateTime(const T& data)
{
	return data.date_time;
}

template<>
const common::SimpleDateTime& GetDateTime<KlineExt1>(const KlineExt1& data)
{
	return data.b_time;
}

template<>
const common::SimpleDateTime& GetDateTime<KlineExt2>(const KlineExt2& data)
{
	return data.b_time;
}

template<>
const common::SimpleDateTime& GetDateTime<Kline>(const Kline& data)
{
	return data.b_time;
}


void ClearDatas(std::deque<void*>& datas)
{
	for (std::deque<void*>::iterator iter = datas.begin(); iter != datas.end(); ++iter)
		delete *iter;

	datas.clear();
}

void GetDataFromLocal(std::deque<void*> &datas, const std::string& path, HisDataType data_type, const common::Date &begin, const common::Date &end) {
	switch (data_type)
	{
	case HIS_STOCK_TICK:
		{
			std::deque<StockTick> ticks;
			ReadHisDatasWithDate(ticks, path, begin, end);
			for (std::deque<StockTick>::iterator iter = ticks.begin(); iter != ticks.end(); ++iter)
				datas.push_back((void*)new StockTick(*iter));
		}
		break;

	case HIS_FUTURE_TICK:
		{
			std::deque<FutureTick> ticks;
			ReadHisDatasWithDate(ticks, path, begin, end);
			for (std::deque<FutureTick>::iterator iter = ticks.begin(); iter != ticks.end(); ++iter)
				datas.push_back((void*)new FutureTick(*iter));
		}
		break;

	case HIS_INDEX_TICK:
		{
			std::deque<IndexTick> ticks;
			ReadHisDatasWithDate(ticks, path, begin, end);
			for (std::deque<IndexTick>::iterator iter = ticks.begin(); iter != ticks.end(); ++iter)
				datas.push_back((void*)new IndexTick(*iter));
		}
		break;

	//case HIS_OPTION_TICK:
	//	{
	//		std::deque<OptionTick> ticks;
	//		ReadHisDatasWithDate(ticks, path, begin, end);
	//		for (std::deque<OptionTick>::iterator iter = ticks.begin(); iter != ticks.end(); ++iter)
	//			datas.push_back((void*)new OptionTick(*iter));
	//	}
	//	break;

	case HIS_KLINE_EXT_1:
		{
			std::deque<KlineExt1> klines;
			ReadHisDatasWithDate(klines, path, begin, end);
			for (std::deque<KlineExt1>::iterator iter = klines.begin(); iter != klines.end(); ++iter)
				datas.push_back((void*)new KlineExt1(*iter));
		}
		break;

	case HIS_KLINE_EXT_2:
		{
			std::deque<KlineExt2> klines;
			ReadHisDatasWithDate(klines, path, begin, end);
			for (std::deque<KlineExt2>::iterator iter = klines.begin(); iter != klines.end(); ++iter)
				datas.push_back((void*)new KlineExt2(*iter));
		}
		break;

	default:
		APP_LOG(LOG_LEVEL_ERROR) << "receive unknow HisDataType: " << data_type;
		break;
	}
}

template<class T>
void ReadHisDatasWithDate(
		std::deque<T> &deq_datas, // 返回值.
		const std::string& path, 
		const common::Date& begin, 
		const common::Date& end) 
{
	int len; T* data;
	QFile* file = MapFile(path, &data, &len);
	if (file == NULL) { return; }

	common::Date pre_beging_trading_day = begin.PreTradingDay();
	common::SimpleDateTime begin_time(common::SimpleDate((short)pre_beging_trading_day.year
		, (char)pre_beging_trading_day.month, (char)pre_beging_trading_day.day), common::SimpleTime(18)); 
	
	common::Date ending = end;
	if (end.WeekDay() == 6 || end.WeekDay() == 7)
		ending = ending.PreTradingDay();
	common::SimpleDate end_date; 
	end_date.year = (short)ending.year; end_date.month = (char)ending.month; end_date.day = (char)ending.day;

	for (int i=len-1; i>=0; --i)
	{
		if (end == COMMON_DATA_END || GetDateTime(data[i]).date <= end_date)
		{
			//if (GetDateTime(data[i]).date < begin_date && GetDateTime(data[i]).time.hour > 3 && GetDateTime(data[i]).time.hour < 18)
			//	break;
			if (GetDateTime(data[i]) < begin_time) { break; }

			if (GetDateTime(data[i]).date == end_date && GetDateTime(data[i]).time.hour >= 18)
				continue; // 过滤掉夜盘.

			deq_datas.push_front(data[i]);
		}
	}

	file->unmap((uchar*)data);
	file->close();
	delete file;
}

template<class T> 
void ReadHisDatasWithNum(std::deque<T> &deq_datas, const std::string& path, int num)
{
	int len; T* data;
	QFile* file = MapFile(path, &data, &len);
	if (file == NULL) { return; }

	if (-1 == num) num = len;
	for (int i=len-1; i>=0; --i)
	{
		if (num <= 0) { break; }
		--num;
		deq_datas.push_front(data[i]);
	}

	file->unmap((uchar*)data);
	file->close();
	delete file;
}

void WriteHisDatas(const std::deque<void*> &datas, HisDataType data_type, const std::string& path) {
	switch (data_type)
	{
	case HIS_STOCK_TICK:
		{
			std::deque<StockTick> write_datas;
			for (std::deque<void*>::const_iterator iter = datas.begin(); iter != datas.end(); ++iter) {
				write_datas.push_back(*(StockTick*)(*iter));
			}
			WriteHisDatas(write_datas, path);
		}
		break;

	case HIS_FUTURE_TICK:
		{
			std::deque<FutureTick> write_datas;
			for (std::deque<void*>::const_iterator iter = datas.begin(); iter != datas.end(); ++iter) {
				write_datas.push_back(*(FutureTick*)(*iter));
			}
			WriteHisDatas(write_datas, path);
		}
		break;

	case HIS_INDEX_TICK:
		{
			std::deque<IndexTick> write_datas;
			for (std::deque<void*>::const_iterator iter = datas.begin(); iter != datas.end(); ++iter) {
				write_datas.push_back(*(IndexTick*)(*iter));
			}
			WriteHisDatas(write_datas, path);
		}
		break;

	//case HIS_OPTION_TICK:
	//	{
	//		std::deque<OptionTick> write_datas;
	//		for (std::deque<void*>::const_iterator iter = datas.begin(); iter != datas.end(); ++iter) {
	//			write_datas.push_back(*(OptionTick*)(*iter));
	//		}
	//		WriteHisDatas(write_datas, path);
	//	}
	//	break;

	case HIS_KLINE_EXT_1:
		{
			std::deque<KlineExt1> write_datas;
			for (std::deque<void*>::const_iterator iter = datas.begin(); iter != datas.end(); ++iter) {
				write_datas.push_back(*(KlineExt1*)(*iter));
			}
			WriteHisDatas(write_datas, path);
		}
		break;

	case HIS_KLINE_EXT_2:
		{
			std::deque<KlineExt2> write_datas;
			for (std::deque<void*>::const_iterator iter = datas.begin(); iter != datas.end(); ++iter) {
				write_datas.push_back(*(KlineExt2*)(*iter));
			}
			WriteHisDatas(write_datas, path);
		}
		break;

	default:
		APP_LOG(LOG_LEVEL_ERROR) << "receive unknow HisDataType: " << data_type;
		return;
	}
}

/*
void KlineMerge(std::deque<void*> &datas, HisDataType &out_type, 
	HisDataType data_type, const KlineInfo &k_info, XmlConfig *conf) {
	
	switch (data_type)
	{
	case HIS_STOCK_TICK:
		{
			std::deque<StockTick> merge_datas;
			for (std::deque<void*>::const_iterator iter = datas.begin(); iter != datas.end(); ++iter) {
				merge_datas.push_back(*(StockTick*)(*iter));
			}
			ClearDatas(datas);
			std::deque<KlineExt1> klines;
			TickMerge(merge_datas, k_info, conf, klines);
			for (std::deque<KlineExt1>::iterator iter = klines.begin(); iter != klines.end(); ++iter) {
				datas.push_back((void*)new KlineExt1(*iter));
			}
			out_type = HIS_KLINE_EXT_1;
		}
		break;

	case HIS_FUTURE_TICK:
		{
			std::deque<FutureTick> merge_datas;
			for (std::deque<void*>::const_iterator iter = datas.begin(); iter != datas.end(); ++iter) {
				merge_datas.push_back(*(FutureTick*)(*iter));
			}
			ClearDatas(datas);
			std::deque<KlineExt2> klines;
			TickMerge(merge_datas, k_info, conf, klines);
			for (std::deque<KlineExt2>::iterator iter = klines.begin(); iter != klines.end(); ++iter) {
				datas.push_back((void*)new KlineExt2(*iter));
			}
			out_type = HIS_KLINE_EXT_2;
		}
		break;

	case HIS_INDEX_TICK:
		{
			std::deque<IndexTick> merge_datas;
			for (std::deque<void*>::const_iterator iter = datas.begin(); iter != datas.end(); ++iter) {
				merge_datas.push_back(*(IndexTick*)(*iter));
			}
			ClearDatas(datas);
			std::deque<KlineExt1> klines;
			TickMerge(merge_datas, k_info, conf, klines);
			for (std::deque<KlineExt1>::iterator iter = klines.begin(); iter != klines.end(); ++iter) {
				datas.push_back((void*)new KlineExt1(*iter));
			}
			out_type = HIS_KLINE_EXT_1;
		}
		break;

	//case HIS_OPTION_TICK:
	//	{
	//		std::deque<OptionTick> merge_datas;
	//		for (std::deque<void*>::const_iterator iter = datas.begin(); iter != datas.end(); ++iter) {
	//			merge_datas.push_back(*(OptionTick*)(*iter));
	//		}
	//		ClearDatas(datas);
	//		std::deque<KlineExt2> klines;
	//		TickMerge(merge_datas, k_info, conf, klines);
	//		for (std::deque<KlineExt2>::iterator iter = klines.begin(); iter != klines.end(); ++iter) {
	//			datas.push_back((void*)new KlineExt2(*iter));
	//		}
	//		out_type = HIS_KLINE_EXT_2;
	//	}
	//	break;

	case HIS_KLINE_EXT_1:
		{
			std::deque<KlineExt1> merge_datas;
			for (std::deque<void*>::const_iterator iter = datas.begin(); iter != datas.end(); ++iter) {
				merge_datas.push_back(*(KlineExt1*)(*iter));
			}
			ClearDatas(datas);
			KlineMerge(merge_datas, k_info, conf);
			for (std::deque<KlineExt1>::const_iterator iter = merge_datas.begin(); iter != merge_datas.end(); ++iter) {
				datas.push_back((void*)new KlineExt1(*iter));
			}
			out_type = HIS_KLINE_EXT_1;
		}
		break;

	case HIS_KLINE_EXT_2:
		{
			std::deque<KlineExt2> merge_datas;
			for (std::deque<void*>::const_iterator iter = datas.begin(); iter != datas.end(); ++iter) {
				merge_datas.push_back(*(KlineExt2*)(*iter));
			}
			ClearDatas(datas);
			KlineMerge(merge_datas, k_info, conf);
			for (std::deque<KlineExt2>::const_iterator iter = merge_datas.begin(); iter != merge_datas.end(); ++iter) {
				datas.push_back((void*)new KlineExt2(*iter));
			}
			out_type = HIS_KLINE_EXT_2;
		}
		break;

	default:
		APP_LOG(LOG_LEVEL_ERROR) << "receive unknow HisDataType: " << data_type;
		return;
	}
}

template<class T> 
void KlineMerge(std::deque<T> &klines, const KlineInfo &k_info, XmlConfig* conf) {

	KlineArrayMerger<T> array_merger(k_info, conf);
	for (int i=0; i<klines.size(); ++i)
	{
		array_merger.PushKline(&klines[i]);
	}
	array_merger.GetData(klines);
}

template<class iT, class oT> 
void TickMerge(const std::deque<iT> &ticks, const KlineInfo &k_info, XmlConfig *conf, std::deque<oT> &klines) {
	KlineArrayMerger<oT> array_merger(k_info, conf);
	for (int i=0; i<ticks.size(); ++i)
	{
		array_merger.PushTick((iT*)&ticks[i]);
	}
	array_merger.GetData(klines);
}


*/

void KlineMerge(std::deque<KlineExt2> &klines, const KlineInfo &k_info) {

	KlineArrayMerger array_merger(k_info);
	for (int i=0; i<klines.size(); ++i)
	{
		array_merger.PushKline(&klines[i], i==klines.size()-1);
	}

	klines.clear();
	std::deque<KlineExt2>& merger_klines = array_merger.Data();
	klines.insert(klines.end(), merger_klines.begin(), merger_klines.end());
}

template<class T>
void WriteHisDatas(const std::deque<T> &deq_datas, const std::string& path) {
	// 如果文件不存在 则创建文件并写入所有数据.
	if (!QFile::exists(path.c_str())) {
		FILE *fp = fopen(path.c_str(), "ab+");
		if (fp == NULL) {
			APP_LOG(LOG_LEVEL_ERROR) << "open file error: " << path;
			return;
		}				
		for (typename std::deque<T>::const_iterator iter = deq_datas.begin(); iter != deq_datas.end(); ++iter)
			fwrite(&(*iter), sizeof(T), 1, fp);		
		fclose(fp);
		return;
	}

	if (deq_datas.empty()) return;

	int len; T* data;
	QFile* file = MapFile(path, &data, &len);
	if (file == NULL) { return; }

	if (0 == len) {
		file->close();
		delete file;
		FILE *fp = fopen(path.c_str(), "ab+");
		if (fp == NULL) { APP_LOG(LOG_LEVEL_ERROR) << "open file error: " << path; return; }
		for (typename std::deque<T>::const_iterator iter = deq_datas.begin(); iter != deq_datas.end(); ++iter)
			fwrite(&(*iter), sizeof(T), 1, fp);
		fclose(fp);
	}
	else if (GetDateTime(deq_datas.back()) <= GetDateTime(*data)) {
		file->unmap((uchar*)data);
		file->close();
		delete file;

		std::deque<T> end_datas;
		ReadHisDatasWithNum(end_datas, path);

		FILE *fp = fopen(path.c_str(), "wb+");
		if (fp == NULL) { APP_LOG(LOG_LEVEL_ERROR) << "open file error: " << path; return; }
		for (typename std::deque<T>::const_iterator iter = deq_datas.begin(); iter != deq_datas.end(); ++iter)
			fwrite(&(*iter), sizeof(T), 1, fp);
		for (typename std::deque<T>::const_iterator iter = end_datas.begin(); iter != end_datas.end(); ++iter)
			fwrite(&(*iter), sizeof(T), 1, fp);
		fclose(fp);
	}
	else if (GetDateTime(deq_datas.front()) >= GetDateTime(*(data + (len-1))) ) {
		file->unmap((uchar*)data);
		file->close();
		delete file;

		FILE *fp = fopen(path.c_str(), "ab+");
		if (fp == NULL) { APP_LOG(LOG_LEVEL_ERROR) << "open file error: " << path; return; }
		for (typename std::deque<T>::const_iterator iter = deq_datas.begin(); iter != deq_datas.end(); ++iter)
			fwrite(&(*iter), sizeof(T), 1, fp);
		fclose(fp);
	}
	else {
		APP_LOG(LOG_LEVEL_ERROR) << "写入数据的范围有误.";
		file->close();
		return;
	}
}

template<class T>
void MergeHisDatas(const std::deque<T> &deq_datas, const std::string& path) {
	if (deq_datas.empty()) return;

	// 如果文件不存在 则创建文件并写入所有数据.
	if (!QFile::exists(path.c_str())) {
		FILE *fp = fopen(path.c_str(), "ab+");
		if (fp == NULL) {
			APP_LOG(LOG_LEVEL_ERROR) << "open file error: " << path;
			return;
		}				
		for (typename std::deque<T>::const_iterator iter = deq_datas.begin(); iter != deq_datas.end(); ++iter)
			fwrite(&(*iter), sizeof(T), 1, fp);		
		fclose(fp);
		return;
	}

	// 如果文件存在，则补入.
	int len; T* data;
	QFile* file = MapFile(path, &data, &len);
	if (file == NULL) { return; }

	//取出文件最后一个data到deq_datas最后一个data之间的数据.
	std::deque<T> end_datas;
	for (int j = len-1; j >= 0; --j) {
		T *cur_data = (T*)(data + j*sizeof(T));
		if (GetDateTime(*cur_data) <= GetDateTime(deq_datas.back()))
			break;
		end_datas.push_front(*cur_data);
	}

	int i = 0; // 开始写数据的偏移位置.
	for (; i < len; ++i) {
		T *cur_data = (T*)(data + i*sizeof(T));
		if (GetDateTime(*cur_data) >= GetDateTime(deq_datas.front()))
			break;
	}
	file->unmap((uchar*)data);
	file->close();
	delete file;

	FILE *fp = fopen(path.c_str(), "rb+");
	if (fp == NULL) {
		APP_LOG(LOG_LEVEL_ERROR) << "open file error: " << path;
		return;
	}
	
	fseek(fp, i * sizeof(T), SEEK_SET); // 移到距离文件头 i*k_size 的位置.
	for (typename std::deque<T>::const_iterator iter = deq_datas.begin(); iter != deq_datas.end(); ++iter)
		fwrite(&(*iter), sizeof(T), 1, fp);
	for (typename std::deque<T>::const_iterator iter = end_datas.begin(); iter != end_datas.end(); ++iter)
		fwrite(&(*iter), sizeof(T), 1, fp);
	fclose(fp);
}

}