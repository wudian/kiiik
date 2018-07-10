#pragma once

#include "Baselib/Thread.h"
#include <deque>

namespace baselib{

// 队列大小不设限，Push动作不阻塞 

template <class T>
class NonBlockQueueBuffer : public Thread 
{
public:
	NonBlockQueueBuffer() {}

	virtual void Consume(T& item) = 0;

	virtual void Push(const T& item)
	{
		Locker locker(&queue_lock_);
		eve_queue_.push_back(item);
		cond_.Signal();
	}

	virtual void Terminate()
	{
		Stop();
		cond_.Signal();
	}

private:
	virtual void OnRun() 
	{
		//启动的时候先检查有没有任务
		if (IsRuning())
		{
			T item;
			while (PopEvent(item))
			{
				Consume(item);
			}
		}

		while (IsRuning()) 
		{
			mutex_.Lock();
			cond_.Wait(&mutex_);
			mutex_.Unlock();
			if (!IsRuning()) { break; }

			T item;
			while (PopEvent(item))
			{
				Consume(item);
			}
		}
	}

	bool PopEvent(T& item)
	{
		Locker locker(&queue_lock_);
		if (eve_queue_.size() == 0)
		{
			return false;
		} 
		else
		{
			item = eve_queue_.front();
			eve_queue_.pop_front();
			return true;
		}
	}

protected:
	Mutex mutex_;
	Condition cond_;
	std::deque<T> eve_queue_;
	SpinLock queue_lock_;
};

}