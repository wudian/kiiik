#pragma once
//#include "common.h"..
#include "strategy.h"
#include <iostream>
#include <deque>
#include <map>
using namespace std;

struct pars //策略参数..
{
	int	N;//相关系数计算周期..
	double basis;//价差最小值..
	double min_coe;//最小相关系数..
	double max_coe;//最大相关系数..
	double upLimit;//偏离标准差倍数..
	double upLimit_sell;//止盈系数..
	double upLimit_lose;//止损系数..
	double trackLose;//套利机会界限系数..
	int    period;//交易的分钟周期..
	int    lastbarnum;//上一次的分钟bar计数，便于计算分钟bar差..
	string file1;//合约1的历史行情数据文件名..
	string file2;//合约2的历史行情数据文件名..
};

struct Tpars //基础参数..
{
	double moneyrate;//资金比例..
	double marginrate;//保证金比例..
	double unittick;//合约单位tick值..
	int klinecount;//K线计数..
	int openkline;//开仓分钟..
	int holdvol;//持仓量..
	int startjudgetime;//启动开仓判断时间..
	int terminatejudgetime;//结束开仓判断时间..
	int closetime;//尾盘平仓时间..
	int vol;//交易量 ..
};

typedef deque<double> darray;

class arb1 : public strategy
{
public:
	arb1(string addr);
	virtual ~arb1();
	virtual bool init(string symbol_addr);//策略初始化，参入参数为合约代码文件地址，功能包括读取配置文件，获取历史信息..
	virtual void getsymbol(string addr);//获取合约代码信息..
	virtual void getinivalue(string addr);//读取配置文件信息..
	virtual double  cal_mean(darray data,int n=0);//计算均值..
	virtual void  arb_strategy(CThostFtdcDepthMarketDataField& pData);//套利策略信号..
	virtual void  tf_strategy(CThostFtdcDepthMarketDataField& pData);//趋势策略信号..
	virtual double  cal_std(darray data,int n=0);//计算标准差..
	virtual double cal_corr(darray data1, darray data2, int n = 0);//计算相关系数..
	virtual void   cal_spread(darray data1, darray data2);//计算价差..
	virtual double cal_cov(darray data1, darray data2,int n=0);//计算协方差..
	virtual void OnTick(CThostFtdcDepthMarketDataField& pData);
	virtual void OnOrder(CThostFtdcOrderField& pOrder);
	virtual void OnTrade(CThostFtdcTradeField& pTrade);
	virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

protected:
	bool inited = false, SignUpdateFlag = false;
	string strinfo_addr = "./arb1info.ini";//策略信号数据地址..
	pars inipar;//保存ini文件中的参数..
	darray close1,close2,spread;
	CThostFtdcDepthMarketDataField preData1,preData2;
	Tpars spar;
	int  status = 0;
};