#pragma once
#include "backtest.h"
#include "windows.h"
//#if BACKTEST.
//#include "lft_ru.h".
//#endif.


backtest::backtest(string file_addr) :file_addr(file_addr)
{
}

backtest::~backtest()
{
}

int backtest::readfile(string file)
{
	CThostFtdcDepthMarketDataField Dtest;
	memset(&Dtest, 0, sizeof(Dtest));
	char buffer[256];
	//ifstream out("E:\\期货数据\\20150318_jd1505.txt");.
	ifstream out(file);
	if (!out)
		cout << "failure" << endl;
	else
	{
		//%交易日，合约代码，时间，最新价，开盘价，昨收价，昨结算价，跌停价，涨停价，成交量，.
		//成交额，持仓量，昨持仓量，卖一价，卖一量，买一价，买一量.
		//cout<<"20141231_jd1505.txt"<<" 的内容如下:"<<endl;.
		while (!out.eof())
		{
			memset(buffer, 0, sizeof(buffer));
			out.getline(buffer, 512, '\n');//getline(char *,int,char) 表示该行字符达到256个或遇到换行就结束.
			if (strlen(buffer) < 10)
			{
				continue;
			}
			char *ptr(nullptr);
			char *buf(nullptr);
			//printf("before strtok:  str=%s\n",buffer);  .
			//printf("begin:\n");  .
			ptr = strtok_s(buffer, ",", &buf);
			//printf("str=%s\n",buffer);  .
			//printf("ptr=%s\n",ptr);  .
			if (ptr)
				strcpy_s(Dtest.TradingDay, ptr);
			ptr = strtok_s(NULL, ",", &buf);
			if (ptr)
				strcpy_s(Dtest.InstrumentID, ptr);
			ptr = strtok_s(NULL, ",", &buf);
			if (ptr)
				strcpy_s(Dtest.UpdateTime, ptr);
			ptr = strtok_s(NULL, ",", &buf);
			if (ptr)
				Dtest.LastPrice = atof(ptr);
			ptr = strtok_s(NULL, ",", &buf);
			if (ptr)
				Dtest.OpenPrice = atof(ptr);
			ptr = strtok_s(NULL, ",", &buf);
			if (ptr)
				Dtest.PreClosePrice = atof(ptr);
			ptr = strtok_s(NULL, ",", &buf);
			if (ptr)
				Dtest.PreSettlementPrice = atof(ptr);
			ptr = strtok_s(NULL, ",", &buf);
			if (ptr)
				Dtest.LowerLimitPrice = atof(ptr);
			ptr = strtok_s(NULL, ",", &buf);
			if (ptr)
				Dtest.UpperLimitPrice = atof(ptr);
			ptr = strtok_s(NULL, ",", &buf);
			if (ptr)
				Dtest.Volume = atoi(ptr);
			ptr = strtok_s(NULL, ",", &buf);
			if (ptr)
				Dtest.Turnover = atof(ptr);
			ptr = strtok_s(NULL, ",", &buf);
			if (ptr)
				Dtest.OpenInterest = atof(ptr);
			ptr = strtok_s(NULL, ",", &buf);
			if (ptr)
				Dtest.PreOpenInterest = atof(ptr);
			ptr = strtok_s(NULL, ",", &buf);
			if (ptr)
				Dtest.AskPrice1 = atof(ptr);
			ptr = strtok_s(NULL, ",", &buf);
			if (ptr)
				Dtest.AskVolume1 = atoi(ptr);
			ptr = strtok_s(NULL, ",", &buf);
			if (ptr)
				Dtest.BidPrice1 = atof(ptr);
			ptr = strtok_s(NULL, ",", &buf);
			if (ptr)
				Dtest.BidVolume1 = atoi(ptr);
			ptr = strtok_s(NULL, ",", &buf);
			if (ptr)
				Dtest.ClosePrice = atof(ptr);
			if (ptr)
				Dtest.AveragePrice = atof(ptr);
			ptr = strtok_s(NULL, ",", &buf);
			if (ptr)
				Dtest.LowestPrice = atof(ptr);
			ptr = strtok_s(NULL, ",", &buf);
			if (ptr)
				Dtest.HighestPrice = atof(ptr);
			ptr = strtok_s(NULL, ",", &buf);
			if (ptr)
				Dtest.UpdateMillisec = atoi(ptr);
			ptr=NULL;
			common& comm = common::GetInstance();
			comm.q->OnRtnDepthMarketData(&Dtest);
			Sleep(5);
			//cout << "updatetime:" << Dtest.UpdateTime << endl;.
		}
		out.close();
	}
	cout << "game over!" << endl;
	return 0;
}

bool backtest::getfilenames(string filetype, deque<string>& filenames)
{
	CBrowseDir *p = new CBrowseDir(file_addr.c_str());
	p->GetDirFilenames(file_addr.c_str(), filetype.c_str(), filenames);
	delete p;
	p = NULL;
	if (filenames.empty())
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool backtest::getfilenames(string filetype)
{
	filenames.clear();//保证初始向量为空.
	return getfilenames(filetype, filenames);
}

bool backtest::run(string filetype)
{
	if (!getfilenames(filetype))
	{
		return false;
	}
	else
	{
		cout << "OK!Start backtest!"<<endl;
		//int num = 0;.
		for (deque<string>::iterator it = filenames.begin(); it!= filenames.end();it++)
		{
			readfile(*it);
			//cout << num << endl;.
			//if (num >= 117).
				//cout << it->c_str() << endl;.
			//num++;.
			//filenames.pop_front();.
		}
		return true;
	}
}

void backtest::OnRtnOrder(order_t *pOrder)
{
	common& comm = common::GetInstance();
	std::lock_guard<std::mutex> lock(comm.m_ctp);
	TR tr;
	memset(&tr, 0, sizeof(tr));
	tr.flag = ord;
	CThostFtdcOrderField order;
	memset(&order, 0, sizeof(order));
	strcpy_s(order.InstrumentID, pOrder->InstrumentID);
	order.FrontID = pOrder->FrontID;
	order.SessionID = pOrder->SessionID;
	strcpy_s(order.OrderRef, pOrder->OrderRef);
	order.OrderStatus = THOST_FTDC_OST_AllTraded;
	order.Direction = pOrder->Direction;
	order.LimitPrice = pOrder->LimitPrice;
	order.CombHedgeFlag[0] = pOrder->CombOffsetFlag[0];
	order.CombOffsetFlag[0] = pOrder->CombOffsetFlag[0];
	order.VolumeTotalOriginal = pOrder->VolumeTotalOriginal;
	order.VolumeTraded = pOrder->VolumeTotalOriginal;
	strcpy_s(order.InsertTime, pOrder->InsertTime);
	strcpy_s(order.ExchangeID, "ctp");
	strcpy_s(order.TraderID, "backtest");
	strcpy_s(order.OrderLocalID, pOrder->OrderRef);
	memcpy(&tr.tuo.order, &order, sizeof(tr.tuo.order));
	comm.trseq.push_back(tr);
	comm.c_ctp.notify_all();
}

void backtest::OnRtnTrade(order_t *pOrder)
{
	common& comm = common::GetInstance();
	std::lock_guard<std::mutex> lock(comm.m_ctp);
	TR tr;
	memset(&tr, 0, sizeof(tr));
	tr.flag = trd;
	CThostFtdcTradeField trade;
	memset(&trade, 0, sizeof(trade));
	strcpy_s(trade.InstrumentID, pOrder->InstrumentID);
	strcpy_s(trade.OrderRef, pOrder->OrderRef);
	trade.Direction = pOrder->Direction;
	trade.Price = pOrder->LimitPrice;
	trade.OffsetFlag = pOrder->CombOffsetFlag[0];
	trade.Volume = pOrder->VolumeTotalOriginal;
	strcpy_s(trade.TradeTime, pOrder->InsertTime);
	strcpy_s(trade.ExchangeID, "ctp");
	strcpy_s(trade.TraderID, "backtest");
	strcpy_s(trade.OrderLocalID, pOrder->OrderRef);
	memcpy(&tr.tuo.trade, &trade, sizeof(tr.tuo.trade));
	comm.trseq.push_back(tr);
	comm.c_ctp.notify_all();
}