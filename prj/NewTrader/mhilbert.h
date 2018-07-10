#pragma once
#include "strategy.h"
#include <deque>

using namespace std;
#define KLINE 500

class mhilbert : public strategy
{
	struct Tpars //单边策略参数..
	{
		double prema;//上一个周期的均价 ..
		int malen;//均线周期..
		int obvlen;//观察窗口周期..
		int    period;//交易的分钟周期..
		int    lastbarnum;//上一次的分钟bar计数，便于计算分钟bar差..
		int klinecount;//K线计数..
		int openkline;//开仓分钟..
		int openswitch;//开仓开关..
		int closeswitch;//平仓开关..
		int vol;//交易量 ..
		string file;//历史价格数据存储文件地址..
	};

public:
	mhilbert(string addr);
	~mhilbert();
	void ReSet();
	double cal_mean(deque<double>data,int n);
	void GetIniValue(string addr, char *sect);
	void SetIniValue(string addr, char *sect);
	virtual void OnTick(CThostFtdcDepthMarketDataField& pData);
	void Hlt_base(CThostFtdcDepthMarketDataField& pData);
	virtual void OnOrder(CThostFtdcOrderField& pOrder);
	virtual void OnTrade(CThostFtdcTradeField& pTrade);
	virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

private:
	Tpars tpar;
	deque<double>cprice,diffprice;
};