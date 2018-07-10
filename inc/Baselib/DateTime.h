#ifndef BASELIB_DATE_TIME_H_
#define BASELIB_DATE_TIME_H_

#include <stdio.h>
#include <string>
#include <sstream>
#include <iomanip>
#include "Baselib/Global.h"

namespace baselib {

#define COMMON_DATA_END baselib::Date()

#pragma pack(1)

struct BASELIB_API Date 
{
	int year;
	int month;
	int day;

	Date(int y = 1900, int m = 1, int d = 1) : year(y), month(m), day(d) {}
	Date(char* date_str);

	bool operator==(const Date& date) const;
	bool operator>(const Date& date) const;
	bool operator<(const Date& date) const;
	bool operator<=(const Date& date) const;
	bool operator>=(const Date& date) const;
	bool operator!=(const Date& date) const;
	void operator--(); // 昨天
	void operator++(); // 明天

	// 格式化输出
	long toLong() const;
	std::string Str() const;
	std::string FolderStr() const;

	bool IsHoliday() const; // 是否是法定节假日(不包括非节假日的周六日,但包括是节假日的周六日)
	bool IsTradingDay() const;
	Date PreTradingDay() const; //上一交易日
	Date NextTradingDay() const;

	int WeekDay() const; //星期一 ~ 星期日为 1 ~ 7 .  日期无效返回0
	void AddDays(int ndays);
	int operator-(const Date& date); // 两个日期差多少天

private:
	bool isLeapYear() const; // 闰年
	bool isValid() const; // 例如2月30号就是无效的
	// Gregorian calendar starting from October 15, 1582
	// Algorithm from Henry F. Fliegel and Thomas C. Van Flandern
	int JD() const;
};

struct BASELIB_API Time 
{
	int hour;
	int minute;
	int sec;
	int milsec;

	Time(int h = 0, int m = 0, int s = 0, int ms = 0) : hour(h), minute(m), sec(s), milsec(ms) {}
	Time(char* time_str, int mil);

	Time& operator=(const Time& t);
	bool operator==(const Time& t) const;
	bool operator>(const Time& t) const;
	bool operator<(const Time& t) const;
	bool operator<=(const Time& t) const;
	bool operator>=(const Time& t) const;
	bool operator!=(const Time& t) const;

	long toLong() const;
	std::string Str() const;

	int AddSec(int s);
	int AddMin(int min);
	double operator-(const Time& time);
};

struct BASELIB_API DateTime 
{
	Date date;
	Time time;

	DateTime() : date(), time() {}
	DateTime(void* p);  //获取当前时间
	DateTime(const Date& d, const Time& t) : date(d), time(t) {}

	DateTime(const DateTime& date_time) :date(date_time.date), time(date_time.time) {}

	DateTime& operator=(const DateTime& date_time);

	bool operator==(const DateTime& date_time) const;
	bool operator>(const DateTime& date_time) const;
	bool operator<(const DateTime& date_time) const;
	bool operator!=(const DateTime& date_time) const;
	bool operator>=(const DateTime& date_time) const;
	bool operator<=(const DateTime& date_time) const;

	std::string Str() const;
	bool IsTradingDay();

	DateTime& AddSec(int s);
	DateTime& AddMin(int min);
	double operator-(const DateTime& date_time);

	//返回当前时间所在的交易日
	static Date CurrentTradingDay(DateTime time = DateTime(NULL));
};

//BASELIB_API DateTime operator+(const DateTime& t1, const Time& t2);
//BASELIB_API DateTime operator+(const Time& t1, const DateTime& t2);
//BASELIB_API DateTime operator-(const DateTime& t1, const Time& t2);

#pragma pack()

}
#endif	//BASELIB_DATE_TIME_H_