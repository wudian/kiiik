#pragma once

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "baselib/Thread.h"
#include "baselib/DateTime.h"
#include "baselib/SpinLock.h"

using namespace baselib;
//using namespace lidian::common;


#include "ThostFtdcUserApiStruct.h"

struct kline
{
	char tday[9];//日期
	char updatetime[9];//时间
	double open;//开盘价
	double high;//最高价
	double low;//最低价
	double close;//收盘价
	int    vol;//成交量
	double amt;//成交额
	int    ccl;//持仓量

	kline() { memset(this, 0, sizeof(kline)); }
};


/*
class KlineMergerSpi {
public:
	virtual void OnKline(kline *) = 0;
};*/

// 计算此刻到下一分钟的毫秒数.
inline int GetMilSecNow2NextMin(){
	DateTime now(NULL);
	DateTime next_min = now;
#if 1
	next_min.AddMin(1);	
	next_min.time.sec = 0;
	next_min.time.milsec = 0;
#else
	next_min.AddSec(1);
#endif
	
	return 1000 * (next_min - now);
}

// 生成1min K线.
class KlineMergerApi : public Thread
{
public:
	KlineMergerApi();
	~KlineMergerApi();

	//void PushTick(CThostFtdcDepthMarketDataField *);

private:
	virtual void OnRun(){
		while (IsRuning())
		{
			boost::asio::io_service::work work(io_service_);
			io_service_.run();
		}
	}
	// 每隔一分钟执行一次.
	void OnTimer(const boost::system::error_code&ec){
		if (ec) return;
		cout << "1\n";
		CreateBar();

		timer_.expires_from_now(boost::posix_time::milliseconds(GetMilSecNow2NextMin()));
		timer_.async_wait(boost::bind(&KlineMergerApi::OnTimer, this, boost::asio::placeholders::error));
	}

	boost::asio::io_service io_service_;
	boost::asio::deadline_timer timer_;

	void CreateBar();
};

