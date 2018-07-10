// CTP_Demo.cpp : 定义控制台应用程序的入口点。
//
#pragma once;
#include "stdafx.h"
#include <iostream>
#include <vector>

#include <thread>
#pragma  comment(lib,"WinMM.Lib")

using namespace std;
string strname = "str_back";

strategy *pstr = NULL;

#if  BACKTEST
backtest *pback = NULL;
#endif

int main(int argc, _TCHAR* argv[])
{
#if !BACKTEST
	SYSTEMTIME ct;
	GetLocalTime(&ct);
	int weekofday = ct.wDayOfWeek;
	if (weekofday<1 || weekofday>5)
	{
		//return 0;
	}
#endif
	common& comm = common::GetInstance();
	bool load = comm.Start("./config.cfg");
	pstr = new intra_trend("./inifiles/intra_trend_pp.ini");
	comm.AddStrategy("pp",pstr);
	comm.BeforeMktOpen();
#if BACKTEST
	comm.q = new Quote();
	pback = new backtest("D:/data/");
	cout << "please input filename!" << endl;
	char s[30];
	cin.get(s,30);
	pback->run(s);
#endif


	delete pstr;
	pstr = NULL;

#if BACKTEST
	delete pback;
	pback = NULL;
	delete comm.q;
	comm.q = NULL;
#endif
	comm.AfterMktClose();
	return 0;
}
