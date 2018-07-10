#pragma once
#include "strategy.h"

using namespace std;
#define KLINE 500

class newllt : public strategy
{
	struct Tpars //单边策略参数..
	{
		double coef;//平滑系数..
		double bar0;//当前价格..
		double bar1;//上一个周期价格..
		double bar2;//上两个周期价格..
		double llt0;//当前平滑值..
		double llt1;//上一个周期平滑值..
		double llt2;//上两个周期平滑值..
		int    period;//交易的分钟周期..
		int    lastbarnum;//上一次的分钟bar计数，便于计算分钟bar差..
		int klinecount;//K线计数..
		int openkline;//开仓分钟..
		int openswitch;//开仓开关..
		int closeswitch;//平仓开关..
		int vol;//交易量 ..
	};

public:
	newllt(string addr);
	~newllt();
	void ReSet();
	virtual void GetIniValue(string addr, char *sect);
	virtual void SetIniValue(string addr, char *sect);
	virtual void OnTick(CThostFtdcDepthMarketDataField& pData);
	virtual void Llt_base(CThostFtdcDepthMarketDataField& pData);
	virtual void OnOrder(CThostFtdcOrderField& pOrder);
	virtual void OnTrade(CThostFtdcTradeField& pTrade);
	virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

private:
	Tpars tpar;
};