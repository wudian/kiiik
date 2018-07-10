#include "mhilbert.h"
#include "inihelp.h"
#include "common.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <stdlib.h>
#pragma warning(disable : 4244)
using namespace std;

mhilbert::mhilbert(string addr) :strategy(addr)
{
	inited = false;
	memset(&tpar, 0, sizeof(tpar));
	GetIniValue(addr, "tpars");
	for (int i = 0; i < 5; i++)
	{
		ifstream infile(tpar.file);
		if (!infile)
			cout << "Open hilbertFile Failure!" << endl;
		else
		{
			double coef;
			while (!infile.eof())
			{
				infile >> coef;
				cprice.push_back(coef);
				if (cprice.size() > tpar.malen)
				{
					double ma = cal_mean(cprice,tpar.malen);
					double dvalue = ma - tpar.prema;
					tpar.prema = ma;
					diffprice.push_back(dvalue);
					if (diffprice.size() > 2 * tpar.obvlen + 2)
					{
						diffprice.pop_front();
					}		
				}
			}
			infile.close();
			break;
		}
	}
}

void mhilbert::GetIniValue(string addr, char *sect)
{
	inihelp *pini = new inihelp(addr.c_str());

	/*****趋势部分初始化*******/
	TCHAR section[30];
	pini->CharToTchar(sect, section);
	tpar.prema = pini->ReadFloat(section, TEXT("prema"), 0);
	tpar.malen = pini->ReadInteger(section, TEXT("malen"), 20);
	tpar.obvlen = pini->ReadInteger(section, TEXT("obvlen"), 10);
	tpar.period = pini->ReadInteger(section, TEXT("period"), 10);
	tpar.lastbarnum = pini->ReadInteger(section, TEXT("lastbarnum"), 0);
	tpar.closeswitch = pini->ReadInteger(section, TEXT("closeswitch"), 0);
	tpar.openswitch = pini->ReadInteger(section, TEXT("openswitch"), 0);
	tpar.klinecount = pini->ReadInteger(section, TEXT("klinecount"), 0);
	tpar.openkline = pini->ReadInteger(section, TEXT("openkline"), 0);
	tpar.vol = pini->ReadInteger(section, TEXT("vol"), 1);
	LPCTSTR lfile = pini->ReadString(section, TEXT("fprice"), TEXT("hft_pp.txt"));
	char cfile[30];
	pini->TcharToChar(lfile, cfile);
	tpar.file = cfile;
	delete pini;
	pini = NULL;
}

void mhilbert::SetIniValue(string addr, char *sect)
{
	inihelp *pini = new inihelp(ini_addr.c_str());

	/*****趋势部分初始化*******/
	TCHAR section[30];
	pini->CharToTchar(sect, section);
	pini->WriteFloat(section, TEXT("prema"), tpar.prema);
	pini->WriteInteger(section, TEXT("malen"), tpar.malen);
	pini->WriteInteger(section, TEXT("obvlen"), tpar.obvlen);
	pini->WriteInteger(section, TEXT("period"), tpar.period);
	pini->WriteInteger(section, TEXT("lastbarnum"), tpar.lastbarnum);
	pini->WriteInteger(section, TEXT("closeswitch"), tpar.closeswitch);
	pini->WriteInteger(section, TEXT("openswitch"), tpar.openswitch);
	pini->WriteInteger(section, TEXT("klinecount"), tpar.klinecount);
	pini->WriteInteger(section, TEXT("openkline"), tpar.openkline);
	pini->WriteInteger(section, TEXT("vol"), tpar.vol);

	delete pini;
	pini = NULL;
}

void mhilbert::ReSet()
{
	/*****趋势部分初始化*******/
}

double mhilbert::cal_mean(deque<double>data, int n)
{
	int length = data.size();
	if (length <= 0)
		cerr << "array is empty!" << endl;
	if (length < n)
	{
		n = length;
	}
	double avg = 0;
	for (int i = length - n; i < length; i++)
	{
		avg += data[i];
	}
	avg = avg / n;
	return avg;
}

void mhilbert::OnTick(CThostFtdcDepthMarketDataField& pData)
{
	Hlt_base(pData);
	//cout << pData.InstrumentID << "\t" << pData.LastPrice << endl;

}



void mhilbert::OnOrder(CThostFtdcOrderField& pOrder)
{
	strategy::OnOrder(pOrder);
}

void mhilbert::OnTrade(CThostFtdcTradeField& pTrade)
{
	strategy::OnTrade(pTrade);
}

void mhilbert::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

}

void mhilbert::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

}


void mhilbert::Hlt_base(CThostFtdcDepthMarketDataField& pData)
{	
	/*char stime[9];
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
	if (barflag && tpar.klinecount - tpar.lastbarnum >= tpar.period)*/
	{		
		static int cout = 0;
		if (cout++ < 10)
			return;
		else
			cout = 0;
		int random = rand();
		if (random%2)
		{
			common& comm = common::GetInstance();
			if (tpar.openswitch == 0)
			{
				/*多方开仓*/
				inputorder torder;
				memset(&torder, 0, sizeof(torder));
				torder.strID = cpar.pos[0].strID;
				strcpy_s(torder.InstrumentID, pData.InstrumentID);
				torder.LimitPrice = pData.AskPrice1 + cpar.pos[0].UnitTick;
				torder.Direction = THOST_FTDC_D_Buy;
				torder.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
				torder.Vol = tpar.vol;
				torder.Purchasing = 1;
				torder.UnitTick = cpar.pos[0].UnitTick;
				torder.WaitSecond = 1;
				torder.RequestID = cpar.RequestID;
				torder.changeflag = true;
				comm.SendOrder2(torder);
				tpar.openkline = tpar.klinecount;
				tpar.openswitch = 1;

			}
			else if (tpar.openswitch == -1)
			{
				/*空方平仓*/
				inputorder torder;
				memset(&torder, 0, sizeof(torder));
				torder.strID = cpar.pos[0].strID;
				strcpy_s(torder.InstrumentID, pData.InstrumentID);
				torder.LimitPrice = pData.BidPrice1 - cpar.pos[0].UnitTick;
				torder.Direction = THOST_FTDC_D_Buy;
				torder.CombOffsetFlag[0] = THOST_FTDC_OF_Close;
				torder.Vol = tpar.vol;
				torder.Purchasing = 1;
				torder.UnitTick = cpar.pos[0].UnitTick;
				torder.WaitSecond = 1;
				torder.RequestID = cpar.RequestID;
				torder.changeflag = true;
				comm.SendOrder2(torder);

				/*多方开仓*/
				strcpy_s(torder.InstrumentID, pData.InstrumentID);
				torder.LimitPrice = pData.AskPrice1 + cpar.pos[0].UnitTick;
				torder.Direction = THOST_FTDC_D_Buy;
				torder.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
				torder.changeflag = true;
				comm.SendOrder2(torder);
				tpar.openkline = tpar.klinecount;
				tpar.openswitch = 1;
			}
		}
		else
		{
			common& comm = common::GetInstance();
			if (tpar.openswitch == 0)
			{
				/*空方开仓*/
				inputorder torder;
				memset(&torder, 0, sizeof(torder));
				torder.strID = cpar.pos[0].strID;
				strcpy_s(torder.InstrumentID, pData.InstrumentID);
				torder.LimitPrice = pData.BidPrice1 - cpar.pos[0].UnitTick;
				torder.Direction = THOST_FTDC_D_Sell;
				torder.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
				torder.Vol = tpar.vol;
				torder.Purchasing = 1;
				torder.UnitTick = cpar.pos[0].UnitTick;
				torder.WaitSecond = 1;
				torder.RequestID = cpar.RequestID;
				torder.changeflag = true;
				comm.SendOrder2(torder);
				tpar.openkline = tpar.klinecount;
				tpar.openswitch = -1;

			}
			else if (tpar.openswitch == 1)
			{
				/*多方平仓*/
				inputorder torder;
				memset(&torder, 0, sizeof(torder));
				torder.strID = cpar.pos[0].strID;
				strcpy_s(torder.InstrumentID, pData.InstrumentID);
				torder.LimitPrice = pData.AskPrice1 + cpar.pos[0].UnitTick;
				torder.Direction = THOST_FTDC_D_Sell;
				torder.CombOffsetFlag[0] = THOST_FTDC_OF_Close;
				torder.Vol = tpar.vol;
				torder.Purchasing = 1;
				torder.UnitTick = cpar.pos[0].UnitTick;
				torder.WaitSecond = 1;
				torder.RequestID = cpar.RequestID;
				torder.changeflag = true;
				comm.SendOrder2(torder);

				/*空方开仓*/
				strcpy_s(torder.InstrumentID, pData.InstrumentID);
				torder.LimitPrice = pData.BidPrice1 - cpar.pos[0].UnitTick;
				torder.Direction = THOST_FTDC_D_Sell;
				torder.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
				torder.changeflag = true;
				comm.SendOrder2(torder);
				tpar.openkline = tpar.klinecount;
				tpar.openswitch = -1;
			}
		}
	}


	/*if (barflag)
	{
		tpar.klinecount++;
	}
	memcpy(&preData, &pData, sizeof(pData));
	//更换上一跳数据..*/

	
}

mhilbert::~mhilbert()
{
	ReSet();
	SetIniValue(ini_addr, "tpars");
}