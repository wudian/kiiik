#include "Quote.h"
#include "common.h"
#include <mutex>
#include <condition_variable> 
#include <iostream>
#pragma warning(disable : 4996)
using namespace std;

Quote::Quote(void)
{
}


Quote::~Quote(void)
{
}

void Quote::OnFrontConnected()
{
	cout << "Quote::OnFrontConnected()";
	CThostFtdcReqUserLoginField f;
	memset(&f, 0, sizeof(f));
	common& comm = common::GetInstance();
	strcpy_s(f.BrokerID, comm.cfgfile["BROKER_ID"].c_str());
	strcpy_s(f.UserID, comm.cfgfile["INVESTOR_ID"].c_str());
	strcpy_s(f.Password, comm.cfgfile["PASSWORD"].c_str());
	comm.md->ReqUserLogin(&f, ++comm.nReq);
};

void Quote::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (bIsLast && pRspInfo->ErrorID == 0)
	{
		common& comm = common::GetInstance();
		int iResult = comm.md->SubscribeMarketData(comm.ppInst, comm.iNumber);
		cerr << "--->>> 发送行情订阅请求: " << ((iResult == 0) ? "成功" : "失败") << endl;
	}
}


void Quote::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	//cout << "mkt" << endl;..
	common& comm = common::GetInstance();
	std::lock_guard<std::mutex> lock(comm.m_strategy);
	CThostFtdcDepthMarketDataField  mkt;
	memcpy(&mkt, pDepthMarketData, sizeof(CThostFtdcDepthMarketDataField));
	comm.mktdata.push_back(mkt);
	string key = mkt.InstrumentID;
	comm.LatestMarket[key]=mkt;
	comm.c_strategy.notify_all();
	//cout << "OnRtnDepthMarketData " << key;..
}
