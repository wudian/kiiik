#pragma once

#include <stdexcept>
#include <iostream>
#include "Baselib/SpinLock.h"
#include "Baselib/Mutex.h"
#include "Baselib/Condition.h"

#ifdef WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <process.h>
#else
#include <pthread.h>
#endif

using namespace std;

namespace baselib {

#ifdef WIN32
void SleepImp(int mil_sec);
#endif

class BASELIB_API Thread {
public:
	enum ThreadState {
		RUNING,
		REQUEST_STOP,
		STOPED
	};

	typedef enum {
		HIGHEST_PRIORTY,
		ABOVE_PRIORITY,
		NORMAL_PRIORITY,
		BELOW_PRORITY,
		LOWEST_PRIORITY
	}ThreadPriorty;

private:

#ifdef WIN32
	static void ThreadProc(void* arg)
#else
	static void* ThreadProc(void* arg)
#endif
	{
		Thread* thd = (Thread*)arg;
		thd->OnRun();
		thd->SetState(STOPED);
		thd->Awake();
		//cout<<"Thread stoped"<<endl;
	}

public:	
	Thread();
	virtual ~Thread();

	virtual void OnRun() = 0;

	bool IsRuning();

	ThreadState GetState();

	void Start();

	void Join();

	void Stop();

	static void Sleep(int mil_sec) 
	{
#ifdef WIN32
		SleepImp(mil_sec);
#else
		sleep(mil_sec / 1000.0);
#endif
	}

	void Awake();

private:
	void SetState(ThreadState state);
	int TransLatePriority();
	void WaitExit();

private:
	mutable ThreadState m_state;
	SpinLock m_lock;
	ThreadPriorty m_priority;

	Mutex wait_mutex_;
	Condition wait_cond_;
};


}
