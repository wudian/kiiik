
#include <stdlib.h>
#include <iostream>
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <gtest/gtest.h>
#include "baselib/Mutex.h"
#include "baselib/Global.h"
//#include "baselib/Directory.h"
#include "baselib/AppLog.h"
#include "baselib/QueueBuffer.h"
#include "db_case.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include <boost/timer.hpp>
#include <boost/asio.hpp>
#include <thread>
#include <functional>
#include <boost/date_time.hpp>
#include "boost/date_time/local_time_adjustor.hpp"
#include "boost/date_time/c_local_time_adjustor.hpp"
#include "boost/thread.hpp"
#include "timer/Timer.h"
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
using namespace std;
//using namespace lidian;
using namespace baselib;

TEST(GlobalUT, name)
{
	EXPECT_STREQ("123", "12") << "456";
	//EXPECT_EQ(Global::GetInstance()->GetAppName(), "test_common");
};

class IntQueueBuffer : public QueueBuffer<int, 50>{
	virtual bool Consume(const int& val){
		cout << val << endl;
		return true;
	}
};

class IntQueueBufferUT : public ::testing::Test {
	
public:
	void SetUp(){
		
		int_queue_.Push(1);
		int_queue_.Push(1);
		int_queue_.Push(1);
	}
	void TearDown(){
		//int_queue_.Terminate();
	}


	IntQueueBuffer int_queue_;
};

TEST_F(IntQueueBufferUT, A)
{
	int_queue_.Start();
	//EXPECT_TRUE(int_queue_.siz)
}




#include <QtCore/QJsonArray>
int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	/*testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();*/

#if 0
	DbCase case1;
	case1.connDB();
	case1.createTables();
	case1.insertBadDatas();
#elif 1
	QJsonParseError json_error;
	const char *json = "[{\"data\":{\"asks\":[[\"26050.0\",\"1.444\"],[\"26049.9\",\"5.892\"],[\"26047.91\",\"1.0\"],[\"26025.4\",\"0.095\"],[\"26025.3\",\"0.012\"],[\"26024.0\",\"0.01\"],[\"26000.02\",\"0.055\"],[\"26000.0\",\"0.252\"],[\"25999.9\",\"1.103\"],[\"25999.0\",\"0.9\"],[\"25998.0\",\"0.582\"],[\"25997.0\",\"0.08\"],[\"25976.0\",\"0.785\"],[\"25975.97\",\"0.04\"],[\"25963.88\",\"0.06\"],[\"25954.0\",\"0.08\"],[\"25880.0\",\"0.183\"],[\"25872.0\",\"0.228\"],[\"25855.19\",\"0.009\"],[\"25855.0\",\"0.156\"]],\"bids\":[[\"25840.0\",\"9.69\"],[\"25833.95\",\"0.171\"],[\"25825.76\",\"0.1\"],[\"25811.0\",\"0.1\"],[\"25810.0\",\"0.2\"],[\"25807.0\",\"0.2\"],[\"25805.0\",\"0.108\"],[\"25801.0\",\"1.0\"],[\"25800.76\",\"0.1\"],[\"25800.4\",\"0.2\"],[\"25800.1\",\"1.151\"],[\"25800.01\",\"1.324\"],[\"25800.0\",\"15.916\"],[\"25798.99\",\"0.088\"],[\"25798.98\",\"0.18\"],[\"25796.0\",\"0.025\"],[\"25755.08\",\"0.07\"],[\"25750.13\",\"0.1\"],[\"25750.1\",\"5.96\"],[\"25750.01\",\"0.035\"]],\"timestamp\":1504603019275},\"channel\":\"ok_sub_spotcny_btc_depth_20\"}]";
	QJsonDocument doucment = QJsonDocument::fromJson(json, &json_error);
	if (json_error.error == QJsonParseError::NoError)
	{
		if (doucment.isArray()) { // JSON 文档为数组
			QJsonArray array = doucment.array();  // 转化为数组
			int nSize = array.size();  // 获取数组大小
			for (int i = 0; i < nSize; ++i) {  // 遍历数组
				QJsonObject data = array.at(i).toObject();
				QJsonValue val = data.value("channel");
				QString channel = val.toString();
			}
		}
	}  
#else
	using namespace boost::asio;
	using namespace boost::gregorian;
	using namespace boost::posix_time;

	io_service io_service_;
	deadline_timer timer_(io_service_);
	thread t([&](){ 
		cout << " io_service_.run() begin;\n"; 
		boost::asio::io_service::work work(io_service_);
		io_service_.run(); 
		cout << " io_service_.run() end;\n";
	});

	function<ptime(ptime&)> local_to_utc = [&](ptime &local_tm){
		typedef boost::date_time::local_adjustor< boost::posix_time::ptime, +8, boost::posix_time::no_dst> sct_shz;
		return sct_shz::local_to_utc(local_tm);
	};


	ptime start = time_from_string("2017-06-27 10:33:00");
	cout << start << "\t";
	ptime end = local_to_utc(start);
	time_iterator titr(end, milliseconds(5000)); //increment by 15 minutes

	cout << end << "\n";
	
	boost::system::error_code ec;
	timer_.expires_at(end, ec);
	function<void()> func = [&]{
		timer_.async_wait([&](const boost::system::error_code &ec){
			++titr;
			cout << *titr << "\n";
			boost::this_thread::sleep(seconds(2));
			boost::system::error_code error_code;
			timer_.expires_at(*titr, error_code);
			func();
		});
	};
	func();
	
#endif
	

	return a.exec();
}
