
#include "arb1.h"
#include "inihelp.h"
#include <fstream>
#include <cmath>
#include "common.h"
#pragma warning(disable : 4244)
//using namespace std;..

arb1::arb1(string addr) :strategy(addr)
{
	memset(&cpar,0,sizeof(cpar));
	spar.closetime = 145500;
	cpar.RequestID = 100201;
	spar.terminatejudgetime=145500;
	spar.vol = 1;
	memset(&preData1,0,sizeof(preData1));
	memset(&preData2,0,sizeof(preData2));
	getinivalue(addr);
	cal_spread(close1, close2);
}


bool arb1::init(string symbol_addr)
{
	if (inited)
	{
		return inited;
	}
	getinivalue(ini_addr);  // 读取策略配置文件..
	getsymbol(symbol_addr); //读取策略关联合约文件..
	inited = true;
	return inited;
}

void arb1::getsymbol(string addr)
{
}

double arb1::cal_mean(darray data, int n)
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

double arb1::cal_std(darray data, int n)
{
	int length = data.size();
	if (length <= 0)
		cerr << "array is empty!" << endl;
	if (length < n)
	{
		n = length;
	}
	double avg = cal_mean(data, n);
	double std = 0;
	for (int i = length - n; i < length; i++)
	{
		std += (data[i] - avg)*(data[i] - avg);
	}
	std = std / (n - 1);
	std = sqrt(std);
	return std;
}

double arb1::cal_cov(darray data1, darray data2, int n)
{
	int length1 = data1.size();
	int length2 = data2.size();
	if (min(length1, length2) <= 0)
		cerr << "array is empty!" << endl;
	if (min(length1, length2) < n)
	{
		n = min(length1, length2);
	}
	double avg1 = cal_mean(data1, n);
	double avg2 = cal_mean(data2, n);
	double cov = 0;
	for (int i = 1; i <= n; i++)
	{
		cov += (data1[length1 - i] - avg1)*(data2[length2 - i] - avg2);
	}
	cov = cov / (n - 1);
	return cov;
}

void arb1::cal_spread(darray data1, darray data2)
{
	int length1 = data1.size();
	int length2 = data2.size();
	if (min(length1, length2) <= 0)
	{
		cerr << "array is empty!" << endl;
		return;
	}
	int n = min(length1,length2);
	for (int i = 0; i < n; i++)
	{
		spread.push_back(data1[i]-data2[i]);
	}
}

double arb1::cal_corr(darray data1, darray data2, int n)
{
	int length1 = data1.size();
	int length2 = data2.size();
	if (min(length1, length2) <= 0)
		cerr << "array is empty!" << endl;
	if (min(length1, length2) < n)
	{
		n = min(length1, length2);
	}
	double std1 = cal_std(data1, n);
	double std2 = cal_std(data2, n);
	double cov = cal_cov(data1, data2, n);
	double corr = cov / (std1*std2);
	return corr;
}

void arb1::getinivalue(string addr)
{
	inihelp *pini = new inihelp(addr.c_str());

	/*****公共部分初始化*******/
	LPCTSTR section = TEXT("arb_par");
	inipar.basis = pini->ReadFloat(section, TEXT("basis"), 0.6);
	inipar.max_coe = pini->ReadFloat(section, TEXT("max_coe"), 0.99);
	inipar.min_coe = pini->ReadFloat(section, TEXT("min_coe"), 0.55);
	inipar.N = pini->ReadInteger(section, TEXT("llambda"), 100);
	inipar.trackLose = pini->ReadFloat(section, TEXT("trackLose"), 0.2);
	inipar.upLimit = pini->ReadFloat(section, TEXT("uplimit"), 2.9);
	inipar.upLimit_lose = pini->ReadFloat(section, TEXT("uplimit_lose"), 3.5);
	inipar.upLimit_sell = pini->ReadFloat(section, TEXT("uplimit_sell"), 0);
	inipar.period = pini->ReadInteger(section, TEXT("period"), 15);
	inipar.lastbarnum = pini->ReadInteger(section, TEXT("lastbarnum"), 0);
	cpar.UnitTick=pini->ReadInteger(section, TEXT("unittick"), 1);
	char  inst1[15],inst2[15];
	LPCTSTR inst = pini->ReadString(section,TEXT("inst1"),TEXT("pp1701"));
	pini->TcharToChar(inst, inst1);
	strcpy_s(preData1.InstrumentID, inst1);
	inst = pini->ReadString(section,TEXT("inst2"),TEXT("pp1705"));
	pini->TcharToChar(inst, inst2);
	strcpy_s(preData2.InstrumentID, inst2);
	inst = pini->ReadString(section, TEXT("file1"), TEXT("closeru1.txt"));
	char file1[30], file2[30];
	pini->TcharToChar(inst, file1);
	inipar.file1 = file1;
	inst = pini->ReadString(section, TEXT("file2"), TEXT("closeru2.txt"));
	pini->TcharToChar(inst, file2);
	inipar.file2 = file2;
	delete pini;
	pini = NULL;
	for (int i = 0; i < 5; i++)
	{
		ifstream infile(inipar.file1);
		if (!infile)
			cout << "Open File Failure!" << endl;
		else
		{
			double price;
			while (!infile.eof())
			{
				infile >> price;
				close1.push_back(price);
			}
			infile.close();
			break;
		}
	}

	for (int i = 0; i < 5; i++)
	{
		ifstream infile(inipar.file2);
		if (!infile)
			cout << "Open File Failure!" << endl;
		else
		{
			double price;
			while (!infile.eof())
			{
				infile >> price;
				close2.push_back(price);
			}
			infile.close();
			break;
		}
	}
}



void arb1::arb_strategy(CThostFtdcDepthMarketDataField& pData)
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
			//cout << tokenPtr << '\n';..
			hh[i] = atoi(tokenPtr);
			tokenPtr = strtok_s(NULL, ":", &str);
		}
	}
	int timenum = 10000 * hh[0] + 100 * hh[1] + hh[2];
	poskey pkey1,pkey2;
	pkey1.strID = cpar.pos.strID;
	strcpy_s(pkey1.InstrumentID, preData1.InstrumentID);
	pkey2.strID = cpar.pos.strID;
	strcpy_s(pkey2.InstrumentID,preData2.InstrumentID);
	common& comm = common::GetInstance();
	position_t *pos1 = NULL,*pos2=NULL;
	int vol1=0, vol2=0;
	if (comm.strpos.count(pkey1) > 0) //提取策略持仓数据..
	{
		pos1 = &comm.strpos[pkey1];
		vol1 = pos1->PosLong + pos1->PosShort;
	}
	
	if(comm.strpos.count(pkey2)>0)
	{
		pos2 = &comm.strpos[pkey2];
		vol2 = pos2->PosLong + pos2->PosShort;
	}
	spar.holdvol = vol1 + vol2;

	//comm.CheckOrder(cpar.pos.strID,CTP_None);//检查在途订单..
	bool barflag = false;
	if (strcmp(pData.InstrumentID, preData1.InstrumentID) == 0)
	{
		barflag = CreateBar(pData, preData1);
	}
	if (barflag && preData1.LastPrice>=pData.LowerLimitPrice && spar.klinecount-inipar.lastbarnum>=inipar.period)
	{
		//cout<<klinecount<<endl;..
		inipar.lastbarnum = spar.klinecount;
		close1.push_back(preData1.LastPrice);
		close2.push_back(preData2.LastPrice);
		spread.push_back(preData1.LastPrice - preData2.LastPrice);
		/*分钟数据导出*/
		ofstream out(inipar.file1, ios_base::app);
		if (!out)
		{
			cerr << "open errfile failure" << endl;
		}
		else
		{
			out << preData1.LastPrice << endl;
			out.close();
		}

		ofstream out1(inipar.file2, ios_base::app);
		if (!out1)
		{
			cerr << "open errfile failure" << endl;
		}
		else
		{
			out1 << preData2.LastPrice << endl;
			out1.close();
		}
		if (close1.size() >= inipar.N)
		{
			double lastspread = (preData1.LastPrice - preData2.LastPrice);
			double avgspread = cal_mean(close1, inipar.N) - cal_mean(close2, inipar.N);
			double corr = cal_corr(close1, close2, inipar.N);
			double std = cal_std(spread, inipar.N);
			bool valid_coe = (corr >= inipar.min_coe) && (corr <= inipar.max_coe);
			bool valid_basis = abs(lastspread) >= inipar.basis*pData.LastPrice / 100;
			if (status == 0 && spar.holdvol <= 0 && valid_coe && valid_basis)
			{
				if (lastspread > avgspread + inipar.upLimit*std)
				{
					/*套利空方开仓*/
					inputorder torder;
					memset(&torder, 0, sizeof(torder));
					torder.strID = cpar.pos.strID;
					strcpy_s(torder.InstrumentID, preData1.InstrumentID);
					torder.LimitPrice = preData1.BidPrice1 - cpar.UnitTick;
					torder.Direction = THOST_FTDC_D_Sell;
					torder.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
					torder.Vol = spar.vol;
					torder.Purchasing = 1;
					torder.UnitTick = cpar.UnitTick;
					torder.WaitSecond = 1;
					torder.RequestID = cpar.RequestID;
					comm.SendOrder(torder);
					spar.openkline = spar.klinecount;

					/*套利多方开仓*/
					strcpy_s(torder.InstrumentID, preData2.InstrumentID);
					torder.LimitPrice = preData2.AskPrice1 + cpar.UnitTick;
					torder.Direction = THOST_FTDC_D_Buy;
					comm.SendOrder(torder);
					status = 1;
				}
				else if (lastspread < avgspread - inipar.upLimit*std)
				{
					/*套利空方开仓*/
					inputorder torder;
					memset(&torder, 0, sizeof(torder));
					torder.strID = cpar.pos.strID;
					strcpy_s(torder.InstrumentID, preData2.InstrumentID);
					torder.LimitPrice = preData2.BidPrice1 - cpar.UnitTick;
					torder.Direction = THOST_FTDC_D_Sell;
					torder.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
					torder.Vol = spar.vol;
					torder.Purchasing = 1;
					torder.UnitTick = cpar.UnitTick;
					torder.WaitSecond = 1;
					torder.RequestID = cpar.RequestID;
					comm.SendOrder(torder);
					spar.openkline = spar.klinecount;

					/*套利多方开仓*/
					strcpy_s(torder.InstrumentID, preData1.InstrumentID);
					torder.LimitPrice = preData1.AskPrice1 + cpar.UnitTick;
					torder.Direction = THOST_FTDC_D_Buy;
					comm.SendOrder(torder);
					status = 2;
				}
			}

			else if (status == 1 && pos1 != NULL && pos2 != NULL)
			{
				int flag = 0;
				//止损..
				if (lastspread > avgspread + inipar.upLimit_lose*std)
				{
					flag = 1;
					status = 3;
				} //止盈..
				else if (lastspread < avgspread + inipar.upLimit_sell*std)
				{
					flag = 2;
					status = 0;
				}

				//尾盘平仓..
				/*
				if (timenum >= cpar.closetime && timenum < 160000)
				{
					flag = 3;
					status = 0;
				}
				*/
				if (flag > 0)
				{
					/*套利空方平仓*/
					inputorder torder;
					memset(&torder, 0, sizeof(torder));
					torder.strID = cpar.pos.strID;
					strcpy_s(torder.InstrumentID, preData1.InstrumentID);
					torder.LimitPrice = preData1.AskPrice1 + cpar.UnitTick;
					torder.Direction = THOST_FTDC_D_Buy;
					torder.CombOffsetFlag[0] = THOST_FTDC_OF_CloseToday;
					torder.Vol = pos1->PosShort;
					torder.Purchasing = 1;
					torder.UnitTick = cpar.UnitTick;
					torder.WaitSecond = 1;
					torder.RequestID = cpar.RequestID;
					comm.SendOrder(torder);
					spar.openkline = spar.klinecount;

					/*套利多方平仓*/
					strcpy_s(torder.InstrumentID, preData2.InstrumentID);
					torder.LimitPrice = preData2.BidPrice1 - cpar.UnitTick;
					torder.Direction = THOST_FTDC_D_Sell;
					torder.Vol = pos2->PosLong;
					comm.SendOrder(torder);
				}
			}

			else if (status == 2 && pos1 != NULL && pos2 != NULL)
			{
				int flag = 0;

				//止损..
				if (lastspread < avgspread - inipar.upLimit_lose*std)
				{
					flag = 1;
					status = 0;
				} //止盈..
				else if (lastspread > avgspread - inipar.upLimit_sell*std)
				{
					flag = 2;
					status = 0;
				}
				//尾盘平仓..
				/*
				if (timenum >= cpar.closetime && timenum < 160000)
				{
				flag = 3;
				status = 0;
				}
				*/

				if (flag > 0)
				{
					/*套利空方平仓*/
					inputorder torder;
					memset(&torder, 0, sizeof(torder));
					torder.strID = cpar.pos.strID;
					strcpy_s(torder.InstrumentID, preData2.InstrumentID);
					torder.LimitPrice = preData2.AskPrice1 + cpar.UnitTick;
					torder.Direction = THOST_FTDC_D_Buy;
					torder.CombOffsetFlag[0] = THOST_FTDC_OF_CloseToday;
					torder.Vol = pos2->PosShort;;
					torder.Purchasing = 1;
					torder.UnitTick = cpar.UnitTick;
					torder.WaitSecond = 1;
					torder.RequestID = cpar.RequestID;
					comm.SendOrder(torder);
					spar.openkline = spar.klinecount;

					/*套利多方平仓*/
					strcpy_s(torder.InstrumentID, preData1.InstrumentID);
					torder.LimitPrice = preData1.BidPrice1 - cpar.UnitTick;
					torder.Direction = THOST_FTDC_D_Sell;
					torder.Vol = pos1->PosLong;
					comm.SendOrder(torder);
				}
			}
		}
	}//计算分钟收盘价..
	if (barflag)
	{
		spar.klinecount++;
	}
	if (strcmp(pData.InstrumentID, preData1.InstrumentID) == 0)
	{
		memcpy(&preData1, &pData, sizeof(pData));
	}//更换上一跳数据..
	else if (strcmp(pData.InstrumentID, preData2.InstrumentID) == 0)
	{
		memcpy(&preData2, &pData, sizeof(pData));
	}//更换上一跳数据..
}

void arb1::tf_strategy(CThostFtdcDepthMarketDataField& pData)
{

}


void arb1::OnTick(CThostFtdcDepthMarketDataField& pData)
{
	arb_strategy(pData);
}

void arb1::OnOrder(CThostFtdcOrderField& pOrder)
{
	
}

void arb1::OnTrade(CThostFtdcTradeField& pTrade)
{
	
}

void arb1::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	
}

void arb1::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	
}

arb1::~arb1()
{
}