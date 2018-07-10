#include "newllt.h"
#include "inihelp.h"
#include "common.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#pragma warning(disable : 4244)
using namespace std;

newllt::newllt(string addr) :strategy(addr)
{
	inited = false;
	memset(&tpar, 0, sizeof(tpar));
	memset(&preData, 0, sizeof(preData));

	//inited = Init();..
	//GetComValue(addr, "cpars");..
	GetIniValue(addr, "tpars");
}

void newllt::GetIniValue(string addr, char *sect)
{
	inihelp *pini = new inihelp(addr.c_str());

	/*****趋势部分初始化*******/
	TCHAR section[30];
	pini->CharToTchar(sect, section);
	tpar.bar0 = pini->ReadFloat(section, TEXT("bar0"), 0);
	tpar.bar1 = pini->ReadFloat(section, TEXT("bar1"), 0);
	tpar.bar2 = pini->ReadFloat(section, TEXT("bar2"), 0);
	tpar.llt0 = pini->ReadFloat(section, TEXT("llt0"), 0);
	tpar.llt1 = pini->ReadFloat(section, TEXT("llt1"), 0);
	tpar.llt2 = pini->ReadFloat(section, TEXT("llt2"), 0);

	tpar.closeswitch = pini->ReadInteger(section, TEXT("closeswitch"), 0);
	tpar.openswitch = pini->ReadInteger(section, TEXT("openswitch"), 0);
	tpar.klinecount = pini->ReadInteger(section, TEXT("klinecount"), 0);
	tpar.openkline = pini->ReadInteger(section, TEXT("openkline"), 0);
	tpar.vol = pini->ReadInteger(section, TEXT("vol"), 1);

	delete pini;
	pini = NULL;
}

void newllt::SetIniValue(string addr, char *sect)
{
	inihelp *pini = new inihelp(ini_addr.c_str());

	/*****趋势部分初始化*******/
	TCHAR section[30];
	pini->CharToTchar(sect, section);
	pini->WriteFloat(section, TEXT("bar0"), tpar.bar0);
	pini->WriteFloat(section, TEXT("bar1"), tpar.bar1);
	pini->WriteFloat(section, TEXT("bar2"), tpar.bar2);
	pini->WriteFloat(section, TEXT("llt0"), tpar.llt0);
	pini->WriteFloat(section, TEXT("llt1"), tpar.llt1);
	pini->WriteFloat(section, TEXT("llt2"), tpar.llt2);
	
	pini->WriteInteger(section, TEXT("closeswitch"), tpar.closeswitch);
	pini->WriteInteger(section, TEXT("openswitch"), tpar.openswitch);
	pini->WriteInteger(section, TEXT("klinecount"), tpar.klinecount);
	pini->WriteInteger(section, TEXT("openkline"), tpar.openkline);
	pini->WriteInteger(section, TEXT("vol"), tpar.vol);

	delete pini;
	pini = NULL;
}

void newllt::ReSet()
{
	/*****趋势部分初始化*******/
}

void newllt::OnTick(CThostFtdcDepthMarketDataField& pData)
{
	Llt_base(pData);
}



void newllt::OnOrder(CThostFtdcOrderField& pOrder)
{
	common& comm = common::GetInstance();
	poskey pkey;
	pkey.strID = strname;
	strcpy_s(pkey.InstrumentID, pOrder.InstrumentID);
	strcpy_s(cpar.pos.InstrumentID, pOrder.InstrumentID);
	strcpy_s(cpar.pos.InvestorID, pOrder.InvestorID);
	position_t& rpos = comm.strpos[pkey];
	cpar.pos.CLongFrozen = rpos.CLongFrozen;
	cpar.pos.CShortFrozen = rpos.CShortFrozen;
	cpar.pos.OLongFrozen = rpos.OLongFrozen;
	cpar.pos.OShortFrozen = rpos.OShortFrozen;
}

void newllt::OnTrade(CThostFtdcTradeField& pTrade)
{
	common& comm = common::GetInstance();
	poskey pkey;
	pkey.strID = strname;
	strcpy_s(pkey.InstrumentID, pTrade.InstrumentID);
	position_t& rpos = comm.strpos[pkey];
	cpar.pos.PosCostLong = rpos.PosCostLong;
	cpar.pos.PosCostShort = rpos.PosCostShort;
	cpar.pos.PosLong = rpos.PosLong;
	cpar.pos.PosShort = rpos.PosShort;
	cpar.pos.YdPosLong = rpos.YdPosLong;
	cpar.pos.YdPosShort = rpos.YdPosShort;
}

void newllt::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

}

void newllt::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

}


void newllt::Llt_base(CThostFtdcDepthMarketDataField& pData)
{
	char stime[9];
	strncpy_s(stime, pData.UpdateTime, 9);
	int hh[3] = { 0 };
	char *str;
	char *tokenPtr = strtok_s(stime, ":", &str);
	for (int i = 0; i < 3; i++)
	{
		if (tokenPtr != NULL)
		{
			hh[i] = atoi(tokenPtr);
			tokenPtr = strtok_s(NULL, ":", &str);
		}
	}
	int timenum = 10000 * hh[0] + 100 * hh[1] + hh[2];
	bool barflag = false;
	barflag = CreateBar(pData, preData);
	if (barflag && tpar.klinecount - tpar.lastbarnum >= tpar.period)
	{
		//cout<<klinecount<<endl;..
		tpar.lastbarnum = tpar.klinecount;
		tpar.bar2 = tpar.bar1;
		tpar.bar1 = tpar.bar0;
		tpar.bar0 = preData.LastPrice;
		tpar.llt2 = tpar.llt1;
		tpar.llt1 = tpar.llt0;
		double v1, v2;
		v1 = 2 * (1 - tpar.coef)*tpar.llt1 - pow(1 - tpar.coef,2)*tpar.llt2;
		v2 = tpar.coef*(1 - 0.2*tpar.coef)*tpar.bar0 + 0.5*tpar.coef*tpar.coef*tpar.bar1 - tpar.coef*(1 - 0.5*tpar.coef)*tpar.bar2;
		tpar.llt0 = v1 + v2;
		if (tpar.llt0 > tpar.llt1)
		{
			common& comm = common::GetInstance();
			if (tpar.openswitch == 0)
			{
				/*多方开仓*/
				inputorder torder;
				memset(&torder, 0, sizeof(torder));
				torder.strID = cpar.pos.strID;
				strcpy_s(torder.InstrumentID, pData.InstrumentID);
				torder.LimitPrice = pData.AskPrice1 + cpar.UnitTick;
				torder.Direction = THOST_FTDC_D_Buy;
				torder.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
				torder.Vol = tpar.vol;
				torder.Purchasing = 1;
				torder.UnitTick = cpar.UnitTick;
				torder.WaitSecond = 1;
				torder.RequestID = cpar.RequestID;
				comm.SendOrder(torder);
				tpar.openkline = tpar.klinecount;
				tpar.openswitch = 1;

			}
			else if (tpar.openswitch == -1)
			{
				/*空方平仓*/
				inputorder torder;
				memset(&torder, 0, sizeof(torder));
				torder.strID = cpar.pos.strID;
				strcpy_s(torder.InstrumentID, pData.InstrumentID);
				torder.LimitPrice = pData.BidPrice1 - cpar.UnitTick;
				torder.Direction = THOST_FTDC_D_Sell;
				torder.CombOffsetFlag[0] = THOST_FTDC_OF_Close;
				torder.Vol = cpar.pos.YdPosShort + cpar.pos.PosShort;
				torder.Purchasing = 1;
				torder.UnitTick = cpar.UnitTick;
				torder.WaitSecond = 1;
				torder.RequestID = cpar.RequestID;
				comm.SendOrder(torder);

				/*多方开仓*/
				strcpy_s(torder.InstrumentID, pData.InstrumentID);
				torder.LimitPrice = pData.AskPrice1 + cpar.UnitTick;
				torder.Direction = THOST_FTDC_D_Buy;
				torder.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
				comm.SendOrder(torder);
				tpar.openkline = tpar.klinecount;
				tpar.openswitch = 1;
			}
		}
		else if (tpar.llt0 < tpar.llt1)
		{
			common& comm = common::GetInstance();
			if (tpar.openswitch == 0)
			{
				/*空方开仓*/
				inputorder torder;
				memset(&torder, 0, sizeof(torder));
				torder.strID = cpar.pos.strID;
				strcpy_s(torder.InstrumentID, pData.InstrumentID);
				torder.LimitPrice = pData.BidPrice1 - cpar.UnitTick;
				torder.Direction = THOST_FTDC_D_Sell;
				torder.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
				torder.Vol = tpar.vol;
				torder.Purchasing = 1;
				torder.UnitTick = cpar.UnitTick;
				torder.WaitSecond = 1;
				torder.RequestID = cpar.RequestID;
				comm.SendOrder(torder);
				tpar.openkline = tpar.klinecount;
				tpar.openswitch = -1;

			}
			else if (tpar.openswitch == 1)
			{
				/*多方平仓*/
				inputorder torder;
				memset(&torder, 0, sizeof(torder));
				torder.strID = cpar.pos.strID;
				strcpy_s(torder.InstrumentID, pData.InstrumentID);
				torder.LimitPrice = pData.AskPrice1 + cpar.UnitTick;
				torder.Direction = THOST_FTDC_D_Sell;
				torder.CombOffsetFlag[0] = THOST_FTDC_OF_Close;
				torder.Vol = cpar.pos.YdPosShort + cpar.pos.PosShort;
				torder.Purchasing = 1;
				torder.UnitTick = cpar.UnitTick;
				torder.WaitSecond = 1;
				torder.RequestID = cpar.RequestID;
				comm.SendOrder(torder);

				/*空方开仓*/
				strcpy_s(torder.InstrumentID, pData.InstrumentID);
				torder.LimitPrice = pData.BidPrice1 - cpar.UnitTick;
				torder.Direction = THOST_FTDC_D_Sell;
				torder.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
				comm.SendOrder(torder);
				tpar.openkline = tpar.klinecount;
				tpar.openswitch = -1;
			}
		}
	}

	if (barflag)
	{
		tpar.klinecount++;
	}
	memcpy(&preData, &pData, sizeof(pData));
	//更换上一跳数据..
}

newllt::~newllt()
{
	ReSet();
	SetIniValue(ini_addr, "tpars");
}