#pragma once
//#include "common.h"..
//#include <iostream>..
#include "selfdatastruct.h"
#include "sqlite3.h"
#include <string>
#include <vector>
#include "inihelp.h"
#pragma comment(lib,"sqlite3.lib")
//using namespace std;..

class strategy
{
public:
	strategy(string addrs);
	virtual ~strategy();
	virtual bool Init();
	virtual void GetComValue(string addr,char *section);
	virtual void SetComValue(string addr,char *section);
	virtual void OnTick(CThostFtdcDepthMarketDataField& pData)=0;
	virtual void OnOrder(CThostFtdcOrderField& pOrder);
	virtual void OnTrade(CThostFtdcTradeField& pTrade);
	virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) = 0;
	virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) = 0;
	virtual string GetName() const { return strname; }
	virtual vector<string> GetInst() const { return vecInsts_; }
	//virtual position_t GetPos() const { return cpar.pos; };.
	virtual bool CreateBar(CThostFtdcDepthMarketDataField& pData, CThostFtdcDepthMarketDataField& preData);
protected:
	bool inited = false;
	compar cpar;
	string ini_addr, strname, InvestorID, InstrumentID;//ini文件地址,策略名称 ...
	std::vector<std::string> vecInsts_;
	CThostFtdcDepthMarketDataField preData;
	char *errMsg;
	sqlite3 *db;
};