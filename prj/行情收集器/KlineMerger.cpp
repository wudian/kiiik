#include "KlineMerger.h"
#include <fstream>

extern ofstream outbar[300];
extern map<string, kline> bar;
extern SpinLock bar_spinlock;
extern int iNumber;
extern char* ppInst[300];

KlineMergerApi::KlineMergerApi()
: timer_(io_service_)
{
	baselib::Thread::Start();

	timer_.expires_from_now(boost::posix_time::milliseconds(GetMilSecNow2NextMin()));
	timer_.async_wait(boost::bind(&KlineMergerApi::OnTimer, this, boost::asio::placeholders::error));
}


KlineMergerApi::~KlineMergerApi()
{
	boost::system::error_code ec;
	timer_.cancel(ec);
	io_service_.stop();

	baselib::Thread::Stop();
}

void KlineMergerApi::CreateBar(){
	Locker lock(&bar_spinlock);
	for (int i = 0; i < iNumber; ++i){
		if (outbar[i]){
			string InstrumentID = ppInst[i];
			outbar[i] << bar[InstrumentID].tday << ",";
			outbar[i] << bar[InstrumentID].updatetime << ",";
			outbar[i] << bar[InstrumentID].open << ",";
			outbar[i] << bar[InstrumentID].high << ",";
			outbar[i] << bar[InstrumentID].low << ",";
			outbar[i] << bar[InstrumentID].close << ",";
			outbar[i] << bar[InstrumentID].vol << ",";
			outbar[i] << bar[InstrumentID].amt << ",";
			outbar[i] << bar[InstrumentID].ccl << endl;
			//bar[InstrumentID].open
			bar[InstrumentID] = kline();
		}
	}
	
}
/*
void KlineMergerApi::PushTick(CThostFtdcDepthMarketDataField *tick){

}*/