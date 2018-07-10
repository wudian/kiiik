#pragma once
#include "Trade.h"
#include "common.h"
#include "browsdir.h"
#include <fstream>

class backtest
{
public:
	backtest(string file_addr);
	~backtest();
	int readfile(string file);//读取一天的行情数据.
	bool getfilenames(string filetype,deque<string>&);//找出某个文件夹下指定类型所有文件的名称.
	bool getfilenames(string filetype);
	bool run(string filetype);//启动回测.
	void OnRtnOrder(order_t *pOrder);//回测用的报单回报.
	void OnRtnTrade(order_t *pOrder);//回测用的成交回报.
private:
	string file_addr;
	deque<string>filenames;
};


