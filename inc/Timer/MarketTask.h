#ifndef ITS_STRATEGY_MARKET_TASK_H_
#define ITS_STRATEGY_MARKET_TASK_H_

#include "Baselib/Global.h"
#include "Timer/StateTask.h"
#include "baselib/DateTime.h"



#define DAY_OPEN_TIME baselib::Time(8, 45, 0)
#define DAY_CLOSE_TIME baselib::Time(15, 20, 0)
#define NIGHT_OPEN_TIME baselib::Time(20, 30, 0)
#define NIGHT_CLOSE_TIME baselib::Time(2, 35, 0)

#define NON_TRADING_ID	0
#define DAY_OPEN_ID		1
#define DAY_CLOSE_ID	2
#define NIGHT_OPEN_ID	3
#define NIGHT_CLOSE_ID	4

class TIMER_API TimedStateTaskManager : public StateTaskManager, public TimerSpi
{
public:
	friend class NonTradingTask; 
	friend class DayOpenTask; 
	friend class DayCloseTask;
	friend class NightOpenTask; 
	friend class NightCloseTask;

	TimedStateTaskManager(int _InterMilsec/*定时器多久触发一次*/,int _StaMilsec/*隔一段时间后启动定时器*/=60000);
	virtual ~TimedStateTaskManager();

	void StartTimer();

	virtual bool DoDayOpen() = 0;
	virtual bool DoDayClose() = 0;
	virtual bool DoNightOpen() = 0;
	virtual bool DoNightClose() = 0;

private:
	virtual void OnTimer(int id);

	TimerApi *timer_api_;

protected:
	int start_milsec_;
	int interval_milsec;

	baselib::Time day_open_time_;
	baselib::Time day_close_time_;
	baselib::Time night_open_time_;
	baselib::Time night_close_time_;
};

class NonTradingTask : public StateTask
{
public:
	NonTradingTask(TimedStateTaskManager* manager) : manager_(manager), StateTask(NON_TRADING_ID) {}
	virtual bool Handle(StateTaskManager* manager) { return true; }
	virtual int ToNextState();

	bool IsNextHoliday(baselib::Date date);

private:
	TimedStateTaskManager* manager_;
};

class DayOpenTask : public StateTask
{
public:
	DayOpenTask(TimedStateTaskManager* manager) : StateTask(DAY_OPEN_ID), manager_(manager) {}
	virtual bool Handle(StateTaskManager* manager) { return manager_->DoDayOpen(); }
	virtual int ToNextState();

private:
	TimedStateTaskManager* manager_;
};

class DayCloseTask : public StateTask
{
public:
	DayCloseTask(TimedStateTaskManager* manager) : StateTask(DAY_CLOSE_ID), manager_(manager) {}
	virtual bool Handle(StateTaskManager* manager) { return manager_->DoDayClose(); }
	virtual int ToNextState();

private:
	TimedStateTaskManager* manager_;
};

class NightOpenTask : public StateTask
{
public:
	NightOpenTask(TimedStateTaskManager* manager) : StateTask(NIGHT_OPEN_ID), manager_(manager) {}
	virtual bool Handle(StateTaskManager* manager) { return manager_->DoNightOpen(); }
	virtual int ToNextState();

private:
	TimedStateTaskManager* manager_;
};

class NightCloseTask : public StateTask
{
public:
	NightCloseTask(TimedStateTaskManager* manager) : StateTask(NIGHT_CLOSE_ID), manager_(manager) {}
	virtual bool Handle(StateTaskManager* manager) { return manager_->DoNightClose(); }
	virtual int ToNextState();

private:
	TimedStateTaskManager* manager_;
};


#endif