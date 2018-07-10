#pragma once

#include <memory.h>
#include <string.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include "Baselib/QueueBuffer.h"
#include "Baselib/SpendTime.h"

#define APP_LOG(LEVEL) baselib::AppLogFinisher() = baselib::AppLogInput(baselib::LEVEL, __FILE__, __LINE__)

namespace baselib {

extern SpendTime spend_time;

/*
 * @brief LogLevel 日志层级枚举值.
 * @note 关闭层级不会打印任何东西；日志当前层表示只打印当前及以上的层级的信息.
 * 如警告层级只打印警告和错误的日志、调试层级打印所有日志信息.
 */
typedef enum {
	LOG_LEVEL_OFF         = 0,	/**< 关闭层级 */
	LOG_LEVEL_ERROR       = 1,	/**< 错误层级 */
	LOG_LEVEL_WARN        = 2,	/**< 警告层级 */
	LOG_LEVEL_INFO		  = 3,	/**< 信息层级 */
	LOG_LEVEL_SUCCESS     = 4,	/**< 成功层级 */
	LOG_LEVEL_DEBUG		  = 5	/**< 调试层级 */
} LogLevel;

struct LogData {
	LogLevel level;
	char file_name[128];
	int line;
	char message[128];

	LogData() : level(LOG_LEVEL_INFO), line(0) {
		memset(file_name, 0, 128);
		memset(message, 0, 128);
	}

	LogData(LogLevel l, const char* filename, int line_, const char* msg)
		: level(l)
		, line(line_)
	{
		memcpy(file_name, filename, 128);
		memcpy(message, msg, 128);
	}
};

class BASELIB_API AppLog : public QueueBuffer<LogData, 50> {
public:
	enum{
		DEFAULT_BUF_LEN			= 50,				/**< 默认缓冲区的长度 */
		DEFAULT_LOG_MAX_SIZE	= 1000 * 1024,		/**< 默认日志容量,1M */
		DEFAULT_LOG_MAX_NUM		= 10				/**< 默认日志数量 */
	};

public:
	~AppLog();
	static AppLog* GetInstance();

	void set_level(LogLevel level) {
		if (m_is_running) { return; }	//日志线程开始运行后不能再改变值.
		m_level = level;
	}

	LogLevel get_level() const {
		return m_level;
	}

	void set_max_size(int size) {
		if (m_is_running) { return; }	//日志线程开始运行后不能再改变值.
		m_max_size = size > 0 ? size : m_max_size;	//大于0才有效.
	}

	int get_max_size() const {
		return m_max_size;
	}

	void set_max_num(int num) {
		if (m_is_running) { return; }	//日志线程开始运行后不能再改变值.
		m_max_num = num > 0 ? num : m_max_num;	//大于0才有效.
	}

	int get_max_num() const {
		return m_max_num;
	}

	/*
	 * @brief InitLogConfig 有配置文件初始化日志的各个属性.
	 * @param config 配置文件对象.
	 */
	void InitLog();

private:
	AppLog();

	virtual bool Consume(const LogData& val);

	LogLevel StringToLevel(const char* value);
	const char*  LevelToString(LogLevel level);

	/*
	 * @brief GetNowTime 获取当前时间.
	 * @return 返回时间格式 2013-11-13 09:03:18.189
	 */
	std::string GetNowTime();

	void OpenLogFile();

	/*
	 * @brief RollLogFile 如果文件大小超过设置，则滚动重命名日志文件.
	 */
	void RollLogFile();

private:
	static AppLog* m_instance;

	LogLevel m_level;			/**< 日志层级 */
	int m_max_size;				/**< 日志最大容量 */
	int m_max_num;				/**< 循环日志的最大个数，超过这个数则自动删除最早的日志文件 */
	FILE* m_file;				/**< 日志文件对象 */
	bool m_is_running;			/**< 日志线程是否已经在运行 */

	string m_file_path;
	string m_err_file_path;
	bool m_append_console; // 是否打印到客户端
};

class AppLogInput
{    
private:
	char m_file_name[128];
	int m_line;
	char m_message[128];
	LogLevel m_level;
public:
	AppLogInput(LogLevel l, const char* filename, int line) : m_level(l), m_line(line)
	{  
		memcpy(m_file_name, filename, 128);
		memset(m_message, 0, 128);
	};

	friend class AppLogFinisher;

	AppLogInput& operator<<(const string& msg) { 
		strcat(m_message, msg.c_str()); 
		return *this; 
	}

	AppLogInput& operator<<(const char* msg) { 
		strcat(m_message, msg); 
		return *this; 
	}

	AppLogInput& operator<<(char c) 
	{ 
		sprintf(m_message, "%s%c", m_message, c);
		return *this;
	}

	AppLogInput& operator<<(int n) {
		sprintf(m_message, "%s%d", m_message, n);
		return *this;
	}

	AppLogInput& operator<<(unsigned int n) {
		sprintf(m_message, "%s%d", m_message, n);
		return *this;
	}

	AppLogInput& operator<<(double d) {
		sprintf(m_message, "%s%f", m_message, d);
		return *this;
	}

	AppLogInput& operator<<(long l) {
		sprintf(m_message, "%s%ld", m_message, l);
		return *this;
	}

	AppLogInput& operator<<(long long l) {
		sprintf(m_message, "%s%ld", m_message, l);
		return *this;
	}
};

class AppLogFinisher
{
public:
	void operator=(AppLogInput& input) {
		AppLog::GetInstance()->Push(LogData(input.m_level, input.m_file_name, input.m_line, input.m_message));
	};
};

}