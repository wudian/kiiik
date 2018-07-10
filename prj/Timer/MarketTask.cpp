#include "Timer/MarketTask.h"
#include "baselib/AppLog.h"

using namespace baselib;

TimedStateTaskManager::TimedStateTaskManager(int _InterMilsec, int _StaMilsec)
	: StateTaskManager(), interval_milsec(_StaMilsec), start_milsec_(_StaMilsec)
	, day_open_time_(DAY_OPEN_TIME), day_close_time_(DAY_CLOSE_TIME)
	, night_open_time_(NIGHT_OPEN_TIME), night_close_time_(NIGHT_CLOSE_TIME)
{
	try {
		InsertTask(new NonTradingTask(this));
		InsertTask(new DayOpenTask(this));
		InsertTask(new DayCloseTask(this));
		InsertTask(new NightOpenTask(this));
		InsertTask(new NightCloseTask(this));
	}
	catch (Exception& err)
	{
		cout << err.what() << endl;
	}
	
	SetCurTask(NON_TRADING_ID);

	timer_api_ = new TimerApi(_InterMilsec, this);
}

TimedStateTaskManager::~TimedStateTaskManager()
{
	delete timer_api_;
}


void TimedStateTaskManager::StartTimer()
{
	timer_api_->Start();
}

void TimedStateTaskManager::OnTimer(int id)
{
	try
	{
		HandleNextTask();
	}
	catch (Exception* e)
	{
		cout << e->what() << endl;
		APP_LOG(LOG_LEVEL_ERROR) << e->what();
		timer_api_->Stop();
	}
	catch (...) {
		assert(false);
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////

bool NonTradingTask::IsNextHoliday(baselib::Date date)
{
	date++;
	while (date.WeekDay() > 5)
	{
		if (date.IsHoliday()) return true;
		date++;
	}
	return date.IsHoliday();
}

int NonTradingTask::ToNextState()
{
	baselib::DateTime now(NULL);
	if (now.time > manager_->day_open_time_ && now.time < manager_->day_close_time_ && now.IsTradingDay())
	{
		return DAY_OPEN_ID;
	}

	if ((now.time > manager_->night_open_time_ && now.IsTradingDay() && !IsNextHoliday(now.date)))
	{
		return NIGHT_OPEN_ID;
	}

	//对于凌晨2.30以前，需要退回到前一日再验证
	if (now.time < manager_->night_close_time_)
	{
		baselib::Date date = now.date;
		date--;
		if (date.IsTradingDay() && !IsNextHoliday(date))
		{
			return NIGHT_OPEN_ID;
		}
	}

	return -1;
}

int DayOpenTask::ToNextState()
{
	baselib::DateTime now(NULL);
	return now.time > manager_->day_close_time_ ? DAY_CLOSE_ID : -1;
}

int DayCloseTask::ToNextState()
{
	return NON_TRADING_ID;
}

int NightOpenTask::ToNextState()
{
	baselib::DateTime now(NULL);
	return now.time > manager_->night_close_time_ && now.time < manager_->day_open_time_ ? NIGHT_CLOSE_ID : -1;
}

int NightCloseTask::ToNextState()
{
	return NON_TRADING_ID;
}

