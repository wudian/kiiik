#include "BaseLibDefine.h"

QString formatDate(long date){
	if (date < 0.001) return "--";
	QString sdate = QString("%1-%2-%3")
		.arg(date / 10000, 4, 10, QChar('0'))
		.arg(date % 10000 / 100, 2, 10, QChar('0'))
		.arg(date % 100, 2, 10, QChar('0'));
	return sdate;
}

QString formatTime(long time){
	QString stime = QString("%1:%2:%3.%4")
		.arg(time / 10000000, 2, 10, QChar(' '))
		.arg(time % 10000000 / 100000, 2, 10, QChar('0'))
		.arg(time % 100000 / 1000, 2, 10, QChar('0'))
		.arg(time % 1000, 3, 10, QChar('0'));
	return stime;
}