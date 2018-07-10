// CTP_Demo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Quote.h"
#include "Trade.h"
#include <iomanip>
#include <iostream>
#include <fstream>
#include <deque>
#include <map>
#include <process.h>
#include <direct.h>
#include "KlineMerger.h"
#include<windows.h>

using namespace std;


CThostFtdcMdApi* md;
CThostFtdcTraderApi* api;
Quote* q = new Quote();
Trade* t = new Trade();

// 会话参数

TThostFtdcFrontIDType	FRONT_ID;	//前置编号
TThostFtdcSessionIDType	SESSION_ID;	//会话编号
TThostFtdcOrderRefType	ORDER_REF;	//报单引用

//配置参数

TThostFtdcBrokerIDType	BROKER_ID = "8000";				// 经纪公司代码
TThostFtdcInvestorIDType INVESTOR_ID = "80700221";			// 投资者代码
TThostFtdcPasswordType  PASSWORD = "190205";//"800477";			// 用户密码
map<string, CThostFtdcDepthMarketDataField> preData;
int iNumber = 0;
char filename[300][80];
char barname[300][80];
int nReq = 0;
deque<CThostFtdcDepthMarketDataField> mktdata; //行情队列
char* ppInst[300];//合约数组
HANDLE hEvent = NULL, writehandle = NULL,mdEvent=NULL;
CRITICAL_SECTION critical;
bool flag = true;
ofstream out[300],outbar[300];
//const int local = 0;//程序放置地点，0为外网，1为内网
string md_addr = "tcp://180.168.212.75:41213" ;
string td_addr = "tcp://180.168.212.75:41205";

/*
struct kline
{
	char tday[9];//日期
	char updatetime[9];//时间
	double open;//开盘价
	double high;//最高价
	double low;//最低价
	double close;//收盘价
	int    vol;//成交量
	double amt;//成交额
	int    ccl;//持仓量
};
*/
map<string, kline> bar;
SpinLock bar_spinlock;


bool CreateBar(CThostFtdcDepthMarketDataField& pData, CThostFtdcDepthMarketDataField& preData)
{
	if ((strcmp(pData.UpdateTime, "02:30:00") > 0 && strcmp(pData.UpdateTime, "09:00:00") < 0) ||
		(strcmp(pData.UpdateTime, "15:15:00") > 0 && strcmp(pData.UpdateTime, "21:00:00") < 0))
	{
		return false;
	}

	if ((strcmp(preData.UpdateTime, "11:30:00") >= 0 && strcmp(pData.UpdateTime, "13:00:00") < 0)) //(strcmp(pData.UpdateTime, "10:15:00") >= 0 && strcmp(pData.UpdateTime, "10:15:02") < 0) ||
	{
		return false;
	}

	if ((strcmp(preData.UpdateTime, "02:30:00") >= 0 && strcmp(preData.UpdateTime, "09:00:00") <= 0) ||
		(strcmp(preData.UpdateTime, "15:15:00") >= 0 && strcmp(preData.UpdateTime, "21:00:00") <= 0) || strcmp(preData.UpdateTime, "00:00:00") <= 0)
	{
		return false;
	}

	char lastupdate[9];
	char preupdate[9];
	strncpy_s(lastupdate, pData.UpdateTime, 9);
	strncpy_s(preupdate, preData.UpdateTime, 9);
	int lasttime[3] = { 0 }, pretime[3] = { 0 };
	char *laststr, *prestr;
	char *lasttokenPtr = strtok_s(lastupdate, ":", &laststr);
	char *pretokenPtr = strtok_s(preupdate, ":", &prestr);
	for (int i = 0; i < 3; i++)
	{
		if (lasttokenPtr != NULL)
		{
			lasttime[i] = atoi(lasttokenPtr);
			lasttokenPtr = strtok_s(NULL, ":", &laststr);
		}
		if (pretokenPtr != NULL)
		{
			pretime[i] = atoi(pretokenPtr);
			pretokenPtr = strtok_s(NULL, ":", &prestr);
		}
	}

	if (abs(lasttime[0] - pretime[0]) > 1 && abs(lasttime[0] - pretime[0]) < 20)
	{
		return false;
	}

	if ((lasttime[2] < pretime[2] && pretime[2] >= 50) || lasttime[0] != pretime[0] || lasttime[1] != pretime[1])
	{
		int hm = lasttime[0] * 100 + lasttime[1];
		//if (hm != 1015 && hm != 1130 && strlen(preData.InstrumentID)>0)
		//极度不活跃的品种的情况，可能碰到刚好两个分钟整点两条行情，这个是个潜在bug
		if (strlen(preData.InstrumentID)>0)
		{
			return true;
		}
	}
	return false;
}

DWORD WINAPI WJ(LPVOID lpParam)
{
	while (flag)
	{
		//cout << 1 << endl;
		WaitForSingleObject(mdEvent, INFINITE);
		while (!mktdata.empty())
		{
			//cout << 1 << endl;
			CThostFtdcDepthMarketDataField mkt;
			memset(&mkt, 0, sizeof(mkt));
			CThostFtdcDepthMarketDataField *pDepthMarketData = &mkt;//mktdata.front();
			EnterCriticalSection(&critical);
			try
			{
				mkt = mktdata.front();
				mktdata.pop_front();
			}
			catch (std::exception &ex)
			{
				cout << "deque error:" << ex.what() << endl;
			}
			LeaveCriticalSection(&critical);
			//cout << "行情线程" << endl;
			for (int i = 0; i < iNumber; i++)
			{
				if (strcmp(pDepthMarketData->InstrumentID, ppInst[i]) == 0 && 
				(strcmp(pDepthMarketData->UpdateTime, "21:00:00") >= 0 || strcmp(pDepthMarketData->UpdateTime, "02:30:00") <= 0
				|| (strcmp(pDepthMarketData->UpdateTime, "09:00:00") >= 0 && strcmp(pDepthMarketData->UpdateTime, "15:15:00") <= 0)))
				{
					//cout << "md" << endl;
					if (!out[i])
					{
						cout << "Open File Failure 2!" << endl;
						out[i].open(filename[i], ios_base::app);
						/*
						if (local == 0)
						{
							api->RegisterFront("tcp://180.168.212.75:41205");
							api->RegisterFront("tcp://180.168.212.76:41205");
						}
						else if (local == 1)
						{
							api->RegisterFront("tcp://10.3.3.239:41205");
						}
						api->Init();
						*/

					}
					
					if (out[i])
					{
						out[i] << pDepthMarketData->TradingDay << ",";
						out[i] << pDepthMarketData->InstrumentID << ",";
						out[i] << pDepthMarketData->UpdateTime << ",";
						out[i] << pDepthMarketData->LastPrice << ",";
						out[i] << pDepthMarketData->OpenPrice << ",";
						out[i] << pDepthMarketData->PreClosePrice << ",";
						out[i] << pDepthMarketData->PreSettlementPrice << ",";
						out[i] << pDepthMarketData->LowerLimitPrice << ",";
						out[i] << pDepthMarketData->UpperLimitPrice << ",";
						out[i] << pDepthMarketData->Volume << ",";
						out[i] << setprecision(15) << pDepthMarketData->Turnover << ",";
						out[i] << pDepthMarketData->OpenInterest << ",";
						out[i] << pDepthMarketData->PreOpenInterest << ",";
						out[i] << pDepthMarketData->AskPrice1 << ",";
						out[i] << pDepthMarketData->AskVolume1 << ",";
						out[i] << pDepthMarketData->BidPrice1 << ",";
						out[i] << pDepthMarketData->BidVolume1 << ",";
						out[i] << pDepthMarketData->ClosePrice << ",";
						out[i] << pDepthMarketData->AveragePrice << ",";
						out[i] << pDepthMarketData->LowestPrice << ",";
						out[i] << pDepthMarketData->HighestPrice << ",";
						out[i] << pDepthMarketData->UpdateMillisec << endl;
					}
					if (!outbar[i])
					{
						cout << "open barfile failure!" << endl;
						outbar[i].open(barname[i], ios_base::app);
					}
					/*if (CreateBar(mkt, preData[mkt.InstrumentID]) && outbar[i])
					{
						outbar[i] << bar[mkt.InstrumentID].tday << ",";
						outbar[i] << bar[mkt.InstrumentID].updatetime << ",";
						outbar[i] << bar[mkt.InstrumentID].open << ",";
						outbar[i] << bar[mkt.InstrumentID].high << ",";
						outbar[i] << bar[mkt.InstrumentID].low << ",";
						outbar[i] << bar[mkt.InstrumentID].close << ",";
						outbar[i] << bar[mkt.InstrumentID].vol << ",";
						outbar[i] << bar[mkt.InstrumentID].amt << ",";
						outbar[i] << bar[mkt.InstrumentID].ccl << endl;
						c[mkt.InstrumentID].open = mkt.LastPrice;
					}*/

					bar_spinlock.Lock();

					if (bar[mkt.InstrumentID].open <= 0)
					{
						bar[mkt.InstrumentID].open = mkt.LastPrice;
					}
					if (bar[mkt.InstrumentID].high < mkt.LastPrice)
					{
						bar[mkt.InstrumentID].high = mkt.LastPrice;
					}
					if (bar[mkt.InstrumentID].low <= 0 || bar[mkt.InstrumentID].low>mkt.LastPrice)
					{
						bar[mkt.InstrumentID].low = mkt.LastPrice;
					}
					bar[mkt.InstrumentID].close = mkt.LastPrice;
					bar[mkt.InstrumentID].vol = mkt.Volume;
					bar[mkt.InstrumentID].amt = mkt.Turnover;
					bar[mkt.InstrumentID].ccl = mkt.OpenInterest;
					strcpy_s(bar[mkt.InstrumentID].tday, mkt.TradingDay);
					strcpy_s(bar[mkt.InstrumentID].updatetime, mkt.UpdateTime);

					bar_spinlock.Unlock();

					preData[mkt.InstrumentID] = mkt;

					if ((strcmp(pDepthMarketData->UpdateTime, "02:29:59") >= 0 && strcmp(pDepthMarketData->UpdateTime, "09:00:00") < 0)
						|| (strcmp(pDepthMarketData->UpdateTime, "15:14:59") >= 0 && strcmp(pDepthMarketData->UpdateTime, "21:00:00") < 0))
					{
						SYSTEMTIME ct;
						GetLocalTime(&ct);
						int h, m, s;
						h = ct.wHour;
						m = ct.wMinute;
						s = ct.wSecond;
						double ss = 10000 * h + 100 * m + s;
						if ((ss >= 143000 && ss < 160000) || (ss >= 20000 && ss < 30000))
						{
							SetEvent(hEvent);
						}
					}
					break;
				}
			}
		}
	}
	return 0;
}

void readfile(string filepath)
{
	ifstream infile(filepath);
	if (!infile)
		cout << "Open File Failure!" << endl;
	else
	{
		char temp[100];
		while (!infile.eof())
		{
			infile.getline(temp, 100);
			string line(temp);
			size_t pos = line.find('=');//找到每行的“=”号位置，之前是key之后是value   
			if (pos == string::npos) continue;
			string key = line.substr(0, pos);//取=号之前
			string value = line.substr(pos + 1);
			
			if (key=="md_front")
			{
				md_addr = value;
			}
			else if (key == "td_front")
			{
				td_addr = value;
			}
		}
		infile.close();
	}
}


int main(int argc, _TCHAR* argv[])
{
	KlineMergerApi kline_merger;

	SYSTEMTIME ct;
	GetLocalTime(&ct);
	int weekofday = ct.wDayOfWeek;
	if (weekofday < 1 || weekofday>5)
	{
		return 0;
	}
    InitializeCriticalSection(&critical);
	readfile("front_addr.txt");
	api = CThostFtdcTraderApi::CreateFtdcTraderApi("./tlog");
	api->RegisterSpi((CThostFtdcTraderSpi*)t);
	char ptd[100] = {"0"};
	strcpy(ptd,td_addr.c_str());
	api->RegisterFront(ptd);
	api->Init();

	hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	mdEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	writehandle = CreateThread(NULL, 0, WJ, NULL, 0, NULL);


	WaitForSingleObject(hEvent, INFINITE);
	Sleep(5000);
	flag = false;
	Sleep(1000);
	DeleteCriticalSection(&critical);
	for (int i = 0; i < iNumber; i++)
	{
		out[i].close();
		outbar[i].close();
	}
	return 0;
}





//将trade.h文件中对应On函数后面的 {};注销掉即可

void Trade::OnFrontConnected()
{
	//if (local == 0)
	{
		CThostFtdcReqUserLoginField f;
		memset(&f, 0, sizeof(f));
		strcpy_s(f.BrokerID, BROKER_ID);
		strcpy_s(f.UserID, INVESTOR_ID);
		strcpy_s(f.Password, PASSWORD);
		api->ReqUserLogin(&f, ++nReq);
	}
	/*
	else if(local==1)
	{
		CThostFtdcReqAuthenticateField f;
		memset(&f, 0, sizeof(f));
		strcpy_s(f.BrokerID, BROKER_ID);
		strcpy_s(f.UserID, INVESTOR_ID);
		strcpy_s(f.UserProductInfo, "N161115WLG");
		strcpy_s(f.AuthCode, "20161115WLGT0002");
		api->ReqAuthenticate(&f, ++nReq);
	}
	*/
}

void Trade::OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	CThostFtdcReqUserLoginField f;
	memset(&f, 0, sizeof(f));
	strcpy_s(f.BrokerID, BROKER_ID);
	strcpy_s(f.UserID, INVESTOR_ID);
	strcpy_s(f.Password, PASSWORD);
	api->ReqUserLogin(&f, ++nReq);
}

void Trade::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	
	ifstream in("code.txt");
	if (!in)
	{
		cout << "Open codeFile Failure!" << endl;
	}
	else
	{
		char buffer[256];
		while (!in.eof())
		{
			in.getline(buffer, 255);
			string::size_type pos;
			string pattern = ",";
			string value(buffer);
			value += pattern;//扩展字符串以方便操作
			unsigned int size = value.size();
			for (unsigned int i = 0; i < size; i++)
			{
				pos = value.find(pattern, i);
				if (pos < size)
				{
					std::string s = value.substr(i, pos - i);
					ppInst[iNumber] = new char[10];
					strcpy(ppInst[iNumber], s.c_str());
					CThostFtdcDepthMarketDataField d;
					memset(&d, 0, sizeof(CThostFtdcDepthMarketDataField));
					preData.insert(make_pair(s,d));
					kline b;
					memset(&b,0,sizeof(kline));
					bar.insert(make_pair(s,b));
					iNumber++;
					i = pos + pattern.size() - 1;
				}
			}
		}
	}
	
	const char * cday = api->GetTradingDay();
	for (int i = 0; i < iNumber; i++)
	{
		char temp[50] = "D:/wlg/ytickdata/";
		strcat_s(temp, ppInst[i]);
		int dr = _mkdir(temp);
		if (dr == 0)
		{
			cout << "create success!" << endl;
		}
		strcpy_s(filename[i], temp);
		strcat_s(filename[i], "/");
		strcat_s(filename[i], cday);
		strcat_s(filename[i], "_");
		strcat_s(filename[i], ppInst[i]);
		strcat_s(filename[i], ".txt");
		out[i].open(filename[i], ios_base::app);
		if (!out[i])
			cout << "Open File Failure!" << i << endl;

		char tmp[50] = "D:/wlg/bar/";
		strcat_s(tmp, ppInst[i]);
		dr = _mkdir(tmp);
		if (dr == 0)
		{
			cout << "create success!" << endl;
		}
		strcpy_s(barname[i], tmp);
		strcat_s(barname[i], "/");
		strcat_s(barname[i], cday);
		strcat_s(barname[i], "_");
		strcat_s(barname[i], ppInst[i]);
		strcat_s(barname[i], ".txt");
		outbar[i].open(barname[i], ios_base::app);
		if (!outbar[i])
			cout << "Open barFile Failure!" << i << endl;
	}

	if (pRspInfo->ErrorID == 0)
	{
		//启动行情
		md = CThostFtdcMdApi::CreateFtdcMdApi("./qlog");
		md->RegisterSpi((CThostFtdcMdSpi*)q);
		char pmd[100] = { "0" };
		strcpy(pmd, md_addr.c_str());
		md->RegisterFront(pmd);
		md->Init();
	}
}

void Trade::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	
}


//将quote.h文件中对应On函数后面的 {};注销掉即可

void Quote::OnFrontConnected()
{
	CThostFtdcReqUserLoginField f;
	memset(&f, 0, sizeof(f));
	strcpy_s(f.BrokerID, BROKER_ID);
	strcpy_s(f.UserID, INVESTOR_ID);
	strcpy_s(f.Password, PASSWORD);
	md->ReqUserLogin(&f, ++nReq);
};

void Quote::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cout << "mq:" << pRspInfo->ErrorMsg << endl;
	int iResult = md->SubscribeMarketData(ppInst, iNumber);
	cerr << "--->>> 发送行情订阅请求: " << ((iResult == 0) ? "成功" : "失败") << endl;
}

void Quote::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	CThostFtdcDepthMarketDataField mkt;
	memcpy(&mkt, pDepthMarketData, sizeof(mkt));
	//cout << mkt.InstrumentID << endl;
	EnterCriticalSection(&critical);
	mktdata.push_back(mkt);
	LeaveCriticalSection(&critical);
	SetEvent(mdEvent);
}

void Trade::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cout << pRspInfo->ErrorMsg << endl;
	cout << pInputOrder->InstrumentID << endl;
}

void Trade::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
}

void Trade::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
}
