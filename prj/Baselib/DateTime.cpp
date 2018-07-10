#include "baselib/DateTime.h"
#include <map>

using namespace std;

#ifdef WIN32
#include <Windows.h>
#else
#include <sys/time.h>
#endif

namespace baselib {

enum {
	FIRST_YEAR = -4713,
	FIRST_MONTH = 1,
	FIRST_DAY = 2,  // ### Qt 5: make FIRST_DAY = 1, by support jd == 0 as valid
	SECS_PER_DAY = 86400,
	MSECS_PER_DAY = 86400000,
	SECS_PER_HOUR = 3600,
	MSECS_PER_HOUR = 3600000,
	SECS_PER_MIN = 60,
	MSECS_PER_MIN = 60000,
	JULIAN_DAY_FOR_EPOCH = 2440588 // result of julianDayFromGregorianDate(1970, 1, 1)
};

static const char monthDays[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

map<int, string>::value_type holiday_init_data[] =
{
	map<int, string>::value_type(2010, "0101,0215,0216,0217,0218,0219,0405,0503,0614,0615,0616,0922,0923,0924,1001,1004,1005,1006,1007"),
	map<int, string>::value_type(2011, "0103,0202,0203,0204,0207,0208,0404,0405,0502,0606,0912,1003,1004,1005,1006,1007"),
	map<int, string>::value_type(2012, "0102,0103,0123,0124,0125,0126,0127,0402,0403,0404,0430,0501,0622,1001,1002,1003,1004,1005"),
	map<int, string>::value_type(2013, "0101,0102,0103,0211,0212,0213,0214,0215,0404,0405,0429,0430,0501,0610,0611,0612,0919,0920,1001,1002,1003,1004,1007"),
	map<int, string>::value_type(2014, "0101,0131,0203,0204,0205,0206,0407,0501,0502,0602,0908,1001,1002,1003,1006,1007"),
	map<int, string>::value_type(2015, "0101,0102,0218,0219,0220,0223,0224,0406,0501,0622,0903,0904,0927,1001,1002,1005,1006,1007"),
	map<int, string>::value_type(2016, "0101,0207,0208,0209,0210,0211,0212,0213,0404,0502,0609,0610,0611,0915,0916,0917,1003,1004,1005,1006,1007")
};

static map<int, string> s_Holidays(holiday_init_data, holiday_init_data+7);

Date::Date(char* date_str) 
{
	if (strlen(date_str) != 8) {
		year = 1900;
		month = 1;
		day = 1;
		return;
	}

	sscanf(date_str, "%4d%2d%2d", &year, &month, &day);
}

bool Date::isLeapYear() const{
	if (year < 1582) {
		return year % 4 == 0;
	} else {
		return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
	}
}

bool Date::isValid()const  {
	if (year < FIRST_YEAR
		|| (year == FIRST_YEAR &&
		(month < FIRST_MONTH
		|| (month == FIRST_MONTH && day < FIRST_DAY)))
		|| year == 0) // there is no year 0 in the Julian calendar
		return false;

	return (day > 0 && month > 0 && month <= 12) &&
		(day <= monthDays[month] || (day == 29 && month == 2 && isLeapYear()));
}

bool Date::operator==(const Date& date) const
{
	if(year == date.year && month == date.month && day == date.day) { return true; }

	return false;
}

bool Date::operator>(const Date& date) const
{
	if(year > date.year || (year == date.year && month > date.month) || (year == date.year && month == date.month && day > date.day)) {
		return true;
	}

	return false;
}

bool Date::operator<(const Date& date) const
{
	if(year < date.year || (year == date.year && month < date.month) || (year == date.year && month == date.month && day < date.day)) {
		return true;
	}

	return false;
}

bool Date::operator<=(const Date& date) const
{
	return !(*this > date);
}

bool Date::operator>=(const Date& date) const
{
	return !(*this < date);
}

bool Date::operator!=(const Date& date) const
{
	return !(*this == date);
}

long Date::toLong() const{
	char buf[20];
	sprintf(buf, "%04d%02d%02d", year, month, day);
	return atol(buf);
}
std::string Date::Str() const 
{
	std::stringstream ss;
	ss<<year<<"-"
		<<std::setfill('0')<<std::setw(2)<<month<<"-"
		<<std::setfill('0')<<std::setw(2)<<day;
	return ss.str();
}

std::string Date::FolderStr() const {
	std::stringstream ss;
	ss<<year
		<<std::setfill('0')<<std::setw(2)<<month
		<<std::setfill('0')<<std::setw(2)<<day;
	return ss.str();
}


bool Date::IsHoliday() const
{
	if (s_Holidays[year] == "") return false;

	string mmdd_list = s_Holidays[year];
	char mmdd[10];
	sprintf(mmdd, "%02d%02d", month, day);
	return mmdd_list.find(mmdd) != string::npos;
}

bool Date::IsTradingDay() const
{
	return !IsHoliday() && WeekDay() < 6;
}

Date Date::PreTradingDay() const
{ 
	Date pre(year,month,day);
	do{
	--pre;
	}
	while (!pre.IsTradingDay());
	return pre;
}

Date Date::NextTradingDay() const
{
	Date next(year,month,day);
	do{
		++next;
	}
	while (!next.IsTradingDay());
	return next;
}


int Date::WeekDay() const
{
	if (!isValid()) return 0;
	// Gregorian calendar starting from October 15, 1582
	// Algorithm from Henry F. Fliegel and Thomas C. Van Flandern
	int jd = JD();
	return (jd % 7) + 1;
}

void Date::operator--()
{
	AddDays(-1);
}

void Date::operator++()
{
	AddDays(1);
}

int Date::JD() const {
	// Gregorian calendar starting from October 15, 1582
	// Algorithm from Henry F. Fliegel and Thomas C. Van Flandern
	return (1461 * (year + 4800 + (month - 14) / 12)) / 4
		+ (367 * (month - 2 - 12 * ((month - 14) / 12))) / 12
		- (3 * ((year + 4900 + (month - 14) / 12) / 100)) / 4
		+ day - 32075;
}

void Date::AddDays(int ndays)
{
	// Gregorian calendar starting from October 15, 1582
	// Algorithm from Henry F. Fliegel and Thomas C. Van Flandern
	int jd = JD();

	// this is basically "d.jd = jd + ndays" with checks for integer overflow
	if (ndays >= 0)
		jd = (jd + ndays >= jd) ? jd + ndays : 0;
	else
		jd = (jd + ndays < jd) ? jd + ndays : 0;
	
	if (jd) {
		if (jd >= 2299161) {
			// Gregorian calendar starting from October 15, 1582
			// This algorithm is from Henry F. Fliegel and Thomas C. Van Flandern
			long long ell, n, i, j;
			ell = long long(jd) + 68569;
			n = (4 * ell) / 146097;
			ell = ell - (146097 * n + 3) / 4;
			i = (4000 * (ell + 1)) / 1461001;
			ell = ell - (1461 * i) / 4 + 31;
			j = (80 * ell) / 2447;
			day = ell - (2447 * j) / 80;
			ell = j / 11;
			month = j + 2 - (12 * ell);
			year = 100 * (n - 49) + i + ell;
		} else {
			// Julian calendar until October 4, 1582
			// Algorithm from Frequently Asked Questions about Calendars by Claus Toendering
			jd += 32082;
			int dd = (4 * jd + 3) / 1461;
			int ee = jd - (1461 * dd) / 4;
			int mm = ((5 * ee) + 2) / 153;
			day = ee - (153 * mm + 2) / 5 + 1;
			month = mm + 3 - 12 * (mm / 10);
			year = dd - 4800 + (mm / 10);
			if (year <= 0)
				--year;
		}
	}
}

int Date::operator-(const Date& date)
{
	return JD() - date.JD();
}
////////////////////////////////////////////////////////////////////////////////////

Time::Time(char* time_str, int mil) {
	milsec = mil;
	if (strlen(time_str) != 8) {
		hour = 0;
		minute = 0;
		sec = 0;
		return;
	}

	sscanf(time_str, "%2d:%2d:%2d", &hour, &minute, &sec);
}

Time& Time::operator=(const Time& t) {
	if (this != &t) {
		hour = t.hour;
		minute = t.minute;
		sec = t.sec;
		milsec = t.milsec;
	}

	return *this;
}

bool Time::operator==(const Time& t) const
{
	if(hour == t.hour && minute == t.minute && sec == t.sec && milsec == t.milsec) { return true; }

	return false;
}

bool Time::operator>(const Time& t) const
{
	if(hour > t.hour || (hour == t.hour && minute > t.minute) || (hour == t.hour && minute == t.minute && sec > t.sec)
		|| (hour == t.hour && minute == t.minute && sec == t.sec && milsec > t.milsec)) {
			return true;
	}

	return false;
}

bool Time::operator<(const Time& t) const
{
	if(hour < t.hour || (hour == t.hour && minute < t.minute) || (hour == t.hour && minute == t.minute && sec < t.sec)
		|| (hour == t.hour && minute == t.minute && sec == t.sec && milsec < t.milsec)) {
			return true;
	}

	return false;
}

bool Time::operator<=(const Time& t) const
{
	return !(*this > t);
}

bool Time::operator>=(const Time& t) const
{
	return !(*this < t);
}

bool Time::operator!=(const Time& t) const
{
	return !(*this == t);
}

//Time operator+(const Time& t1, const Time& t2) {
//	long t1_milsecs = ((t1.hour * 60 + t1.minute) * 60 + t1.sec) * 1000 + t1.milsec;
//	long t2_milsecs = ((t2.hour * 60 + t2.minute) * 60 + t2.sec) * 1000 + t2.milsec;
//	long ret_milsecs = t1_milsecs + t2_milsecs;
//	int ret_hour = ret_milsecs / (60 * 60 * 1000);
//	int ret_min = (ret_milsecs % (60 * 60 * 1000)) / (60 * 1000);
//	int ret_sec = (ret_milsecs % (60 * 1000)) / 1000;
//	int ret_milsec = ret_milsecs % 1000;
//
//	return Time(ret_hour, ret_min, ret_sec, ret_milsec);
//}
//
//Time operator-(const Time& t1, const Time& t2) {
//	long t1_milsecs = ((t1.hour * 60 + t1.minute) * 60 + t1.sec) * 1000 + t1.milsec;
//	long t2_milsecs = ((t2.hour * 60 + t2.minute) * 60 + t2.sec) * 1000 + t2.milsec;
//	long ret_milsecs = t1_milsecs - t2_milsecs;
//	int ret_hour = ret_milsecs / (60 * 60 * 1000);
//	int ret_min = (ret_milsecs % (60 * 60 * 1000)) / (60 * 1000);
//	int ret_sec = (ret_milsecs % (60 * 1000)) / 1000;
//	int ret_milsec = ret_milsecs % 1000;
//
//	return Time(ret_hour, ret_min, ret_sec, ret_milsec);
//}

int Time::AddSec(int s)
{
	sec += s;
	if (sec >= 60)
	{
		int min = sec / 60;
		sec %= 60;
		return AddMin(min);
	}
	else if (sec < 0)
	{
		int min = sec / 60 - 1;
		do { sec += 60; } while (sec < 0);
		sec %= 60;
		return AddMin(min);
	}
	return 0;
}

int Time::AddMin(int min) 
{
	minute += min;
	if (minute >= 60)
	{
		hour += minute / 60;
		minute %= 60;
		if (hour >= 24)
		{
			int day = hour / 24;
			hour %= 24;
			return day;
		}
	}
	else if (minute < 0)
	{
		hour += minute / 60 - 1;
		do { minute += 60; } while (minute < 0);
		minute %= 60;
		if (hour < 0)
		{
			int day = hour / 24 - 1;
			hour %= 24;
			return day;
		}
	}
	return 0;
}

long Time::toLong() const{
	char buf[20];
	sprintf(buf, "%04d%02d%02d%03d", hour, minute, sec, milsec);
	return atol(buf);
}

std::string Time::Str() const 
{
	std::stringstream ss;
	ss<<std::setfill('0')<<std::setw(2)<<hour<<":"
		<<std::setfill('0')<<std::setw(2)<<minute<<":"
		<<std::setfill('0')<<std::setw(2)<<sec<<"."
		<<std::setfill('0')<<std::setw(3)<<std::setiosflags(std::ios::right)<<milsec;
	return ss.str();
}

double Time::operator-(const Time& time)
{
	double self_secs = (hour * 60 + minute) * 60 + sec + milsec / 1000.0;
	double secs = (time.hour * 60 + time.minute) * 60 + time.sec + time.milsec / 1000.0;
	return self_secs - secs;
}
////////////////////////////////////////////////////////////////////////////////////

DateTime::DateTime(void* p) 
{
#ifdef WIN32
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	date = Date(sys.wYear,sys.wMonth, sys.wDay);
	time = Time(sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
#else
	struct timeval t_time;
	gettimeofday(&t_time, NULL);
	time_t t_date;
	::time(&t_date);
	tm* local_t = localtime(&t_date);
	date = Date(local_t->tm_year+1900, local_t->tm_mon+1, local_t->tm_mday);
	time = Time(local_t->tm_hour, local_t->tm_min, local_t->tm_sec, t_time.tv_usec/1000);
#endif
}

DateTime& DateTime::operator=(const DateTime& date_time) {
	if (this != &date_time) {
		date = date_time.date;
		time = date_time.time;
	}

	return *this;
}

bool DateTime::operator==(const DateTime& date_time) const
{
	if(date == date_time.date && time == date_time.time) { return true; }

	return false;
}

bool DateTime::operator>(const DateTime& date_time) const
{
	if(date > date_time.date || (date == date_time.date && time > date_time.time)) { return true; }

	return false;
}

bool DateTime::operator<(const DateTime& date_time) const
{
	if(date < date_time.date || (date == date_time.date && time < date_time.time)) { return true; }

	return false;
}

bool DateTime::operator!=(const DateTime& date_time) const 
{
	return !(*this == date_time);
}

bool DateTime::operator>=(const DateTime& date_time) const 
{
	return !(*this < date_time);
}

bool DateTime::operator<=(const DateTime& date_time) const 
{
	return !(*this > date_time);
}

std::string DateTime::Str() const 
{
	return date.Str() + time.Str();
}

//DateTime operator+(const DateTime& t1, const Time& t2) 
//{
//	Time t3 = t1.time + t2;
//	return DateTime(t1.date, t3);
//}
//
//DateTime operator+(const Time& t1, const DateTime& t2) 
//{
//	return t2 + t1;
//}
//
//DateTime operator-(const DateTime& t1, const Time& t2) 
//{
//	Time t3 = t1.time - t2;
//	return DateTime(t1.date, t3);
//}

bool DateTime::IsTradingDay() { return date.IsTradingDay(); }

DateTime& DateTime::AddSec(int s)
{
	int plus_day = time.AddSec(s);
	if (plus_day != 0)
	{
		date.AddDays(plus_day);
	}

	return *this;
}

DateTime& DateTime::AddMin(int min)
{
	int plus_day = time.AddMin(min);
	if (plus_day != 0)
	{
		date.AddDays(plus_day);
	}

	return *this;
}

double DateTime::operator-(const DateTime& date_time)
{
	return (date - date_time.date) * 86400.0 + (time - date_time.time);
}

Date DateTime::CurrentTradingDay(DateTime time)
{
	if (time.time.hour >= 3 && time.time.hour < 18)
	{
		return time.date.IsTradingDay() ? time.date : time.date.NextTradingDay();
	}
	else if (time.time.hour < 3)
	{
		time.date.AddDays(-1);
	}

	return time.date.NextTradingDay();
}

}
