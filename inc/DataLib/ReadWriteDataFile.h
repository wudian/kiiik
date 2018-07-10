#ifndef READ_WRITE_DATA_FILE_H
#define READ_WRITE_DATA_FILE_H

#include <string>
#include <deque>
#include "common/Global.h"
#include "DataLib/DataServerStruct.h"
#include "DataLib/Protocol.h"
#include "common/XmlConfig.h"
#include "common/CompressLib.h"
#include "common/ExceptionErr.h"
#include "common/AppLog.h"

namespace zhon_gan {


//3 DIMENSION_MINUTE => "3m"; DIMENSION_TICK => "tick"。月线用 o 代替。 返回""表明无效
DATALIB_API std::string CountPeriodName(short dim_count, DimensionType dim);
// 取单周期名 "tick" "1m" "1d" 取其一
DATALIB_API std::string OnePeriodName(DimensionType dim);
//DIMENSION_MINUTE PRODUCT_STOCK => HIS_KLINE_EXT_1
DATALIB_API HisDataType GetHisDataType(DimensionType dimension, ProductIdType product);
//从文件名中找出开始日期(true)或结束日期(false)
std::string GetDateFromFile(std::string file_name, bool is_begin);
std::string GetDateFromFile(long long mode_data, bool is_begin);

//////////////////////////////////////////////////////////////////////////

// 从本地文件中取出用户需要的数据
void GetDataFromLocal(std::deque<void*> &datas, const std::string& path, HisDataType data_type, const common::Date &begin, const common::Date &end); 
void ClearDatas(std::deque<void*> &datas);

void WriteHisDatas(const std::deque<void*> &datas, HisDataType data_type, const std::string& path);

/*
// datas既是输入，也是输出
void KlineMerge(std::deque<void*> &datas, HisDataType &out_type, HisDataType data_type, const KlineInfo &, common::XmlConfig *);
//////////////////////////////////////////////////////////////////////////


// 输入1m或1d的k线，输出m h、d o y的k线。XmlConfig* 为TradingTime.xml
template<class T> 
void KlineMerge(std::deque<T> &klines, const KlineInfo &, common::XmlConfig *);
// 输入tick数据，输出s级k线
template<class iT, class oT> 
void TickMerge(const std::deque<iT> &ticks, const KlineInfo &, common::XmlConfig *, std::deque<oT> &klines);

*/
// 输入1m k线，输出其他周期分钟线.
DATALIB_API void KlineMerge(std::deque<KlineExt2> &klines, const KlineInfo &);

//根据起始日期和结束日期从本地文件中取历史数据，放入deque中。如果end为
//服务端 读tick：不用该函数
//服务端 读kline：按月份存储kline
//客户端 tick和kline都放在一个文件中，可以读取
template<class T>
void ReadHisDatasWithDate(std::deque<T>&, const std::string& path, const common::Date& begin, const common::Date& end = COMMON_DATA_END);

///从数据文件末尾获取特定根数的数据，以此push_front进deq_datas ; num==-1表示获取所有数据.
//
template<class T> 
void ReadHisDatasWithNum(std::deque<T> &deq_datas, const std::string& path, int num = -1);

//将数据写到文件首或文件尾
template<class T>
void WriteHisDatas(const std::deque<T> &, const std::string& path);

//将数据写到某个文件中，如果文件不存在，则创建文件并写入所有数据；如果文件存在，则补入
template<class T>
void MergeHisDatas(const std::deque<T> &, const std::string& path);

template<class T>
bool UnCompressMarketData(const Symbol& sym, char* buf, int len, T* &datas, int &num, std::string& err)
{
	if (len == 0)
	{
		datas = NULL; 
		num = 0;
		return true;
	}

	int ori_len = *(int*)buf;
	if (ori_len <= 0 || ori_len > 100000000)
	{
		err = "invalid ori_len in tick file";
		return false;
	}

	char* ori_buf = new char[ori_len];
	int out_len = ori_len;
	//APP_LOG(LOG_LEVEL_INFO) << "sym:" << sym.instrument << "   buf_len:" << len << "   ori_len:" << ori_len;
	if (!common::ZlibUnCompress(buf + sizeof(int), len - sizeof(int), ori_buf, &out_len)
		|| out_len != ori_len)
	{
		err = "tick file ZlibUnCompress error";
		return false;
	}

	try {
		KlineCompress kline_compress(sym);
		kline_compress.ChangeToDatas(ori_buf, ori_len, datas, num);
		delete [] ori_buf;
		return true;
	}
	catch(ExceptionErr& e) {
		delete [] ori_buf;
		err = "BufferToTicks error: ";
		return false;
	}
}
//////////////////////////////////////////////////////////////////////////
template void DATALIB_API KlineMerge(std::deque<KlineExt1> &, const KlineInfo &, common::XmlConfig *);
template void DATALIB_API KlineMerge(std::deque<KlineExt2> &, const KlineInfo &, common::XmlConfig *);

template void DATALIB_API TickMerge(const std::deque<StockTick> &, const KlineInfo &, common::XmlConfig *, std::deque<KlineExt1> &);
template void DATALIB_API TickMerge(const std::deque<FutureTick> &, const KlineInfo &, common::XmlConfig *, std::deque<KlineExt2> &);

template void DATALIB_API ReadHisDatasWithDate(std::deque<StockTick>  &, const std::string&, const common::Date&, const common::Date&);
template void DATALIB_API ReadHisDatasWithDate(std::deque<FutureTick> &, const std::string&, const common::Date&, const common::Date&);
template void DATALIB_API ReadHisDatasWithDate(std::deque<IndexTick>  &, const std::string&, const common::Date&, const common::Date&);
template void DATALIB_API ReadHisDatasWithDate(std::deque<OptionTick> &, const std::string&, const common::Date&, const common::Date&);
template void DATALIB_API ReadHisDatasWithDate(std::deque<KlineExt1>  &, const std::string&, const common::Date&, const common::Date&);
template void DATALIB_API ReadHisDatasWithDate(std::deque<KlineExt2>  &, const std::string&, const common::Date&, const common::Date&);
template void DATALIB_API ReadHisDatasWithDate(std::deque<Kline>  &, const std::string&, const common::Date&, const common::Date&);

template void DATALIB_API ReadHisDatasWithNum(std::deque<StockTick>  &, const std::string& path, int num);
template void DATALIB_API ReadHisDatasWithNum(std::deque<FutureTick> &, const std::string& path, int num);
template void DATALIB_API ReadHisDatasWithNum(std::deque<IndexTick>  &, const std::string& path, int num);
template void DATALIB_API ReadHisDatasWithNum(std::deque<OptionTick> &, const std::string& path, int num);
template void DATALIB_API ReadHisDatasWithNum(std::deque<KlineExt1>  &, const std::string& path, int num);
template void DATALIB_API ReadHisDatasWithNum(std::deque<KlineExt2>  &, const std::string& path, int num);
template void DATALIB_API ReadHisDatasWithNum(std::deque<Kline>  &, const std::string& path, int num);

template void DATALIB_API WriteHisDatas(const std::deque<StockTick>  &, const std::string &);
template void DATALIB_API WriteHisDatas(const std::deque<FutureTick> &, const std::string &);
template void DATALIB_API WriteHisDatas(const std::deque<IndexTick>  &, const std::string &);
template void DATALIB_API WriteHisDatas(const std::deque<OptionTick> &, const std::string &);
template void DATALIB_API WriteHisDatas(const std::deque<KlineExt1>  &, const std::string &);
template void DATALIB_API WriteHisDatas(const std::deque<KlineExt2>  &, const std::string &);
template void DATALIB_API WriteHisDatas(const std::deque<Kline>  &, const std::string &);

template void DATALIB_API MergeHisDatas(const std::deque<StockTick>  &, const std::string &);
template void DATALIB_API MergeHisDatas(const std::deque<FutureTick> &, const std::string &);
template void DATALIB_API MergeHisDatas(const std::deque<IndexTick>  &, const std::string &);
template void DATALIB_API MergeHisDatas(const std::deque<OptionTick> &, const std::string &);
template void DATALIB_API MergeHisDatas(const std::deque<KlineExt1>  &, const std::string &);
template void DATALIB_API MergeHisDatas(const std::deque<KlineExt2>  &, const std::string &);
template void DATALIB_API MergeHisDatas(const std::deque<Kline>  &, const std::string &);
}

#endif