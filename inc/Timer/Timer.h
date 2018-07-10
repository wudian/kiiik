#ifndef KIIIK_TIMER_H_  
#define KIIIK_TIMER_H_  

#ifdef TIMER_EXPORTS
#define TIMER_API  __declspec(dllexport)
#else 
#define TIMER_API  __declspec(dllimport)
#endif

#include "boost/asio.hpp"
#include "boost/bind.hpp" 
#include "boost/date_time/posix_time/posix_time.hpp"

#include "Baselib/Thread.h"

class TimerSpi
{
public:
	virtual void OnTimer() = 0;
};


class TIMER_API TimerApi : public baselib::Thread
{
public:
	TimerApi(int ms/*多久触发一次*/, TimerSpi *spi);
	~TimerApi();
	void Start();
	void Stop();

private:
	virtual void OnRun(); 
	void OnTimer(const boost::system::error_code&);

	boost::asio::io_service io_;
	int ms_;
	boost::asio::deadline_timer t_;
	TimerSpi *spi_;

	bool is_running_;
};

#endif