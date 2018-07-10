#include "common.h"
#include "windows.h"
#include "backtest.h"
#include <fstream>

#pragma warning(disable : 4996)

#if BACKTEST
extern backtest *pback;
#endif

using namespace std;
extern CThostFtdcTraderApi* api;

common& common::GetInstance()
{
	static common info;
	return info;
}

void common::CalFrozen(CThostFtdcOrderField& order, order_t& myorder)
{
	poskey pkey;
	pkey.strID = myorder.strID;
	strcpy_s(pkey.InstrumentID, order.InstrumentID);
	if (strpos.count(pkey) <= 0)
	{
		position_t pos;
		memset(&pos, 0, sizeof(pos));
		strcpy_s(pos.InstrumentID, myorder.InstrumentID);
		strcpy_s(pos.InvestorID, myorder.InvestorID);
		pos.strID = myorder.strID;
		auto check = strpos.insert(make_pair(pkey, pos));
		if (!check.second)
		{
			cerr << "insert to strpos failure!" << endl;
		}
	}
	position_t& rpos = strpos[pkey];
	if (order.OrderSubmitStatus == THOST_FTDC_OSS_InsertSubmitted && order.OrderStatus == THOST_FTDC_OST_Unknown) //订单提交，冻结数量为委托数量
	{
		if (order.Direction == THOST_FTDC_D_Buy)
		{
			if (order.CombOffsetFlag[0] == THOST_FTDC_OF_Open)
			{
				rpos.OLongFrozen += (order.VolumeTotalOriginal - myorder.VolumeTotal);
			}
			else
			{
				rpos.CLongFrozen += (order.VolumeTotalOriginal - myorder.VolumeTotal);
			}
		}
		else
		{
			if (order.CombOffsetFlag[0] == THOST_FTDC_OF_Open)
			{
				rpos.OShortFrozen += (order.VolumeTotalOriginal - myorder.VolumeTotal);
			}
			else
			{
				rpos.CShortFrozen += (order.VolumeTotalOriginal - myorder.VolumeTotal);
			}
		}
	}
	else if (order.OrderStatus == THOST_FTDC_OST_PartTradedNotQueueing || order.OrderStatus == THOST_FTDC_OST_Canceled ||
		order.OrderStatus == THOST_FTDC_OST_AllTraded || order.OrderStatus == THOST_FTDC_OST_NoTradeNotQueueing) //订单已经为最终状态，不存在冻结
	{
		if (order.Direction == THOST_FTDC_D_Buy)
		{
			if (order.CombOffsetFlag[0] == THOST_FTDC_OF_Open)
			{
				rpos.OLongFrozen -= myorder.VolumeTotal;
			}
			else
			{
				rpos.CLongFrozen -= myorder.VolumeTotal;
			}
		}
		else
		{
			if (order.CombOffsetFlag[0] == THOST_FTDC_OF_Open)
			{
				rpos.OShortFrozen -= myorder.VolumeTotal;
			}
			else
			{
				rpos.CShortFrozen -= myorder.VolumeTotal;
			}
		}
	}
	else //其他中间状态，需要对两次剩余数量进行差额计算来得到最新冻结数量
	{
		if (order.Direction == THOST_FTDC_D_Buy)
		{
			if (order.CombOffsetFlag[0] == THOST_FTDC_OF_Open)
			{
				rpos.OLongFrozen += (order.VolumeTotal - myorder.VolumeTotal);
			}
			else
			{
				rpos.CLongFrozen += (order.VolumeTotal - myorder.VolumeTotal);
			}
		}
		else
		{
			if (order.CombOffsetFlag[0] == THOST_FTDC_OF_Open)
			{
				rpos.OShortFrozen += (order.VolumeTotal - myorder.VolumeTotal);
			}
			else
			{
				rpos.CShortFrozen += (order.VolumeTotal - myorder.VolumeTotal);
			}
		}
	}
}


int common::CheckOrder(string strname, signtype signal)
{
	int onordernum = 0;
	std::unique_lock<std::mutex> lock(m_ctp);
	if (!orderinfo.empty())
	{
		for (orderinfo_map::iterator itor = orderinfo.begin(); itor != orderinfo.end(); itor++)
		{
			order_t* order = itor->second;
			if ((order->OrderStatus != THOST_FTDC_OST_PartTradedNotQueueing) &&
				(order->OrderStatus != THOST_FTDC_OST_Canceled) &&
				(order->OrderStatus != THOST_FTDC_OST_AllTraded) &&
				(order->OrderStatus != THOST_FTDC_OST_NoTradeNotQueueing) &&
				!order->strID.empty())
			{
				switch (signal)
				{
				case CTP_Buy: //买开
				{
					if (order->Direction == THOST_FTDC_D_Buy && order->CombOffsetFlag[0] == THOST_FTDC_OF_Open && strname == order->strID)
					{
						CancelOrder(order);
					}
					break;
				}
				case CTP_Sell: //卖平
				{
					if (order->Direction == THOST_FTDC_D_Sell && order->CombOffsetFlag[0] == THOST_FTDC_OF_CloseToday && strname == order->strID)
					{
						CancelOrder(order);
					}
					break;
				}
				case CTP_Short: //卖开
				{
					if (order->Direction == THOST_FTDC_D_Sell && order->CombOffsetFlag[0] == THOST_FTDC_OF_Open && strname == order->strID)
					{
						CancelOrder(order);
					}
					break;
				}
				case CTP_Cover: //买平
				{
					if (order->Direction == THOST_FTDC_D_Buy && order->CombOffsetFlag[0] == THOST_FTDC_OF_CloseToday && strname == order->strID)
					{
						CancelOrder(order);
					}
					break;
				}
				case CTP_All: //买平
				{
					CancelOrder(order);
					break;
				}
				default:
				{
					break;
				}
				}
				if (strname == order->strID) //如果该订单为当前策略的在途订单，则计入统计
				{
					onordernum++;
				}
			}
		}
	}
	return onordernum; //指定策略在途订单统计结果
}

bool common::Start(const char *szfile)
{
	if (loaded) return false;

	ifstream cfgFile(szfile);
	if (!cfgFile)
	{
		cout << "can not open cfg file!" << endl;
		return false;
	}
	char tmp[200];
	while (!cfgFile.eof())//循环读取每一行   
	{
		cfgFile.getline(tmp, 200);//每行读取前200个字符，200个应该足够了   
		string line(tmp);
		size_t pos = line.find('=');//找到每行的“=”号位置，之前是key之后是value   
		if (pos == string::npos) continue;
		string key = line.substr(0, pos);//取=号之前
		string value = line.substr(pos + 1);
		if (strcmp(key.c_str(), "ppInst") == 0)
		{
			string::size_type pos;
			string pattern = ",";
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
					auto check = inststatus.insert(make_pair(s, '2'));
					if (!check.second)
					{
						cerr << "insert to instrument status failure!" << endl;
					}
					iNumber++;
					openpermit[s] = true;
					i = pos + pattern.size() - 1;
				}
			}
		}
		else if (strcmp(key.c_str(), "INVESTOR_ID") == 0)
		{
			strcpy(INVESTOR_ID, value.c_str());
		}
		else if (strcmp(key.c_str(), "PASSWORD") == 0)
		{
			strcpy(PASSWORD, value.c_str());
		}
		else if (strcmp(key.c_str(), "BROKER_ID") == 0)
		{
			strcpy(BROKER_ID, value.c_str());
		}
		else if (strcmp(key.c_str(), "tdfront1") == 0)
		{
			strcpy(tdfront1, value.c_str());
		}
		else if (strcmp(key.c_str(), "tdfront") == 0)
		{
			strcpy(tdfront, value.c_str());
		}
		else if (strcmp(key.c_str(), "mdfront") == 0)
		{
			strcpy(mdfront, value.c_str());
		}
		else if (strcmp(key.c_str(), "mdfront1") == 0)
		{
			strcpy(mdfront1, value.c_str());
		}
		Entry p = make_pair(key, value);
		auto insertvalue = cfgfile.insert(p);
		if (!insertvalue.second)
		{
			cerr << "insert failure!" << endl;
		}
	}
	FRONT_ID = 0;
	SESSION_ID = 0;
	strcpy_s(ORDER_REF, "00");
	loaded = true;
	return true;
}

int common::AfterMktClose()
{
	std::unique_lock<std::mutex> lock(m_main);
	c_main.wait(lock);
	ReleaseThread();
	for (int i = 0; i < iNumber; i++)
	{
		delete[] ppInst[i];
		ppInst[i] = NULL;
	}

	for (map<orderkey, order_t*>::iterator it = orderinfo.begin(); it != orderinfo.end(); it++)
	{
		delete it->second;
		it->second = NULL;
	}

	for (map<orderkey, order_t*>::iterator it = orderbook.begin(); it != orderbook.end(); it++)
	{
		it->second = NULL;
	}

	return 0;
}
int common::BeforeMktOpen()
{
	t = new Trade();
	CreateThread();
#if !BACKTEST
	t->Start();
#endif
	return 0;
}

bool common::IsMyOrder(orderkey key)
{
	return (orderinfo.count(key) > 0);
}

bool common::IsMyTrade(tradekey key)
{
	return (tmapo.count(key) > 0);
}

void common::CalPos(CThostFtdcTradeField *pTrade, postype type)
{
	tradekey key;
	strcpy_s(key.ExchangeID, pTrade->ExchangeID);
	strcpy_s(key.OrderLocalID, pTrade->OrderLocalID);
	strcpy_s(key.TraderID, pTrade->TraderID);

	orderkey& okey = tmapo[key];
	order_t* order = orderinfo[okey];
	position_t *ppos = NULL;
	if (type == postype::InstPos)
	{
		if (instpos.count(pTrade->InstrumentID) <= 0)
		{
			position_t pos;
			memset(&pos, 0, sizeof(pos));
			strcpy_s(pos.InstrumentID, pTrade->InstrumentID);
			strcpy_s(pos.InvestorID, pTrade->InvestorID);
			pos.strID = order->strID;
			auto check = instpos.insert(make_pair(pTrade->InstrumentID, pos));
			if (!check.second)
			{
				cerr << "insert to strpos failure!" << endl;
			}
		}
		ppos = &instpos[pTrade->InstrumentID];
	}
	else if (type == postype::StrPos)
	{
		poskey pkey;
		pkey.strID = order->strID;
		strcpy_s(pkey.InstrumentID, pTrade->InstrumentID);
		if (strpos.count(pkey) <= 0)
		{
			position_t pos;
			memset(&pos, 0, sizeof(pos));
			strcpy_s(pos.InstrumentID, pkey.InstrumentID);
			strcpy_s(pos.InvestorID, pTrade->InvestorID);
			pos.strID = pkey.strID;
			auto check = strpos.insert(make_pair(pkey, pos));
			if (!check.second)
			{
				cerr << "insert to strpos failure!" << endl;
			}
		}
		ppos = &strpos[pkey];
	}
	if (pTrade->OffsetFlag == THOST_FTDC_OF_Open)
	{
		if (pTrade->Direction == THOST_FTDC_D_Buy)
		{
			double amount = (ppos->PosCostLong*(ppos->PosLong + ppos->YdPosLong) + pTrade->Volume*pTrade->Price);
			TThostFtdcVolumeType vol = (ppos->PosLong + ppos->YdPosLong + pTrade->Volume);
			ppos->PosCostLong = amount / vol;
			cout << "Otype:" << type << endl;
			cout << "Ooriginallomg:" << ppos->PosLong << endl;
			ppos->PosLong += pTrade->Volume;
			cout << "Onewlong:" << ppos->PosLong << endl;
		}
		else if (pTrade->Direction == THOST_FTDC_D_Sell)
		{
			double amount = (ppos->PosCostShort*(ppos->PosShort + ppos->YdPosShort) + pTrade->Volume*pTrade->Price);
			TThostFtdcVolumeType vol = (ppos->PosShort + ppos->YdPosShort + pTrade->Volume);
			ppos->PosCostShort = amount / vol;
			cout << "Otype:" << type << endl;
			cout << "Ooriginalshort:" << ppos->PosShort << endl;
			ppos->PosShort += pTrade->Volume;
			cout << "Onewshort:" << ppos->PosShort << endl;
		}
	}
	if (pTrade->OffsetFlag == THOST_FTDC_OF_Close || pTrade->OffsetFlag == THOST_FTDC_OF_CloseToday)
	{
		if (pTrade->Direction == THOST_FTDC_D_Buy)
		{
			double amount = (ppos->PosCostShort*(ppos->PosShort + ppos->YdPosShort) - pTrade->Volume*pTrade->Price);
			TThostFtdcVolumeType vol = (ppos->PosShort + ppos->YdPosShort - pTrade->Volume);
			if (vol >= 1)
			{
				ppos->PosCostShort = amount / vol;
			}
			else
			{
				ppos->PosCostShort = 0;
			}
			cout << "Ctype:" << type << endl;
			cout << "Coriginalshort:" << ppos->PosShort << endl;
			ppos->PosShort -= pTrade->Volume;
			cout << "Cnewshort:" << ppos->PosShort << endl;
		}
		else if (pTrade->Direction == THOST_FTDC_D_Sell)
		{
			double amount = (ppos->PosCostLong*(ppos->PosLong + ppos->YdPosLong) - pTrade->Volume*pTrade->Price);
			TThostFtdcVolumeType vol = (ppos->PosLong + ppos->YdPosLong - pTrade->Volume);
			if (vol >= 1)
			{
				ppos->PosCostLong = amount / vol;
			}
			else
			{
				ppos->PosCostLong = 0;
			}
			cout << "Ctype:" << type << endl;
			cout << "Coriginallomg:" << ppos->PosLong << endl;
			ppos->PosLong -= pTrade->Volume;
			cout << "Cnewlong:" << ppos->PosLong << endl;
		}
	}
	if (pTrade->OffsetFlag == THOST_FTDC_OF_CloseYesterday)
	{
		if (pTrade->Direction == THOST_FTDC_D_Buy)
		{
			double amount = (ppos->PosCostShort*(ppos->PosShort + ppos->YdPosShort) - pTrade->Volume*pTrade->Price);
			TThostFtdcVolumeType vol = (ppos->PosShort + ppos->YdPosShort - pTrade->Volume);
			if (vol >= 1)
			{
				ppos->PosCostShort = amount / vol;
			}
			else
			{
				ppos->PosCostShort = 0;
			}
			ppos->YdPosShort -= pTrade->Volume;
		}
		else if (pTrade->Direction == THOST_FTDC_D_Sell)
		{
			double amount = (ppos->PosCostLong*(ppos->PosLong + ppos->YdPosLong) - pTrade->Volume*pTrade->Price);
			TThostFtdcVolumeType vol = (ppos->PosLong + ppos->YdPosLong - pTrade->Volume);
			if (vol >= 1)
			{
				ppos->PosCostLong = amount / vol;
			}
			else
			{
				ppos->PosCostLong = 0;
			}
			ppos->YdPosLong -= pTrade->Volume;
		}
	}
	if (type == postype::InstPos)
	{
		ofstream out("instpos.txt", ios_base::app);
		if (!out)
		{
			cerr << "open errfile failure" << endl;
		}
		else
		{
			out << "strname:" << ppos->strID.c_str() << ",";
			out << "inst:" << ppos->InstrumentID << ",";
			out << "tradedate:" << pTrade->TradeDate << ",";
			out << "tradetime: " << pTrade->TradeTime << ",";
			out << "OC:" << pTrade->OffsetFlag << ",";
			out << "direction: " << pTrade->Direction << ",";
			out << "PosLong: " << ppos->PosLong << ",";
			out << "PosShort: " << ppos->PosShort << ",";
			out << "YdPosLong: " << ppos->YdPosLong << ",";
			out << "YdPosShort: " << ppos->YdPosShort << ",";
			out << "PosCostLong: " << ppos->PosCostLong << ",";
			out << "PosCostShort: " << ppos->PosCostShort << endl;
			out.close();
		}
	}
	if (type == postype::StrPos)
	{
		ofstream out("strpos.txt", ios_base::app);
		if (!out)
		{
			cerr << "open errfile failure" << endl;
		}
		else
		{
			out << "strname:" << ppos->strID.c_str() << ",";
			out << "inst:" << ppos->InstrumentID << ",";
			out << "tradedate:" << pTrade->TradeDate << ",";
			out << "tradetime: " << pTrade->TradeTime << ",";
			out << "OC:" << pTrade->OffsetFlag << ",";
			out << "direction: " << pTrade->Direction << ",";
			out << "PosLong: " << ppos->PosLong << ",";
			out << "PosShort: " << ppos->PosShort << ",";
			out << "YdPosLong: " << ppos->YdPosLong << ",";
			out << "YdPosShort: " << ppos->YdPosShort << ",";
			out << "PosCostLong: " << ppos->PosCostLong << ",";
			out << "PosCostShort: " << ppos->PosCostShort << endl;
			out.close();
		}
	}
}

void common::CancelOrder(order_t *order)
{
	if (order->OrderStatus != THOST_FTDC_OST_PartTradedNotQueueing &&  order->OrderStatus != THOST_FTDC_OST_Canceled
		&& order->OrderStatus != THOST_FTDC_OST_AllTraded && order->OrderStatus != THOST_FTDC_OST_NoTradeNotQueueing)
	{
		CThostFtdcInputOrderActionField req;
		memset(&req, 0, sizeof(req));
		///经纪公司代码
		strcpy_s(req.BrokerID, BROKER_ID);
		///投资者代码
		strcpy_s(req.InvestorID, order->InvestorID);
		///报单引用
		strcpy_s(req.OrderRef, order->OrderRef);
		///前置编号
		req.FrontID = order->FrontID;
		///会话编号
		req.SessionID = order->SessionID;
		///操作标志
		req.ActionFlag = THOST_FTDC_AF_Delete;
		strcpy_s(req.InstrumentID, order->InstrumentID);
		strcpy_s(req.ExchangeID, order->ExchangeID);
		//strcpy_s(req.OrderLocalID, order.OrderLocalID);
#if !BACKTEST
		if (inststatus[order->InstrumentID]== THOST_FTDC_IS_Continous || inststatus[order->InstrumentID] == THOST_FTDC_IS_AuctionOrdering)
		{
			int iResult = api->ReqOrderAction(&req, ++nReq);
			cerr << "--->>> OrderAction: " << ((iResult == 0) ? "Success" : "Failure") << endl;
		}
		else if (inststatus[order->InstrumentID] == THOST_FTDC_IS_BeforeTrading || inststatus[order->InstrumentID] == THOST_FTDC_IS_NoTrading)
		{
			CThostFtdcParkedOrderActionField f;
			memset(&f, 0, sizeof(f));
			///经纪公司代码
			strcpy_s(f.BrokerID, BROKER_ID);
			///投资者代码
			strcpy_s(f.InvestorID, order->InvestorID);
			///报单引用
			strcpy_s(f.OrderRef, order->OrderRef);
			///前置编号
			f.FrontID = order->FrontID;
			///会话编号
			f.SessionID = order->SessionID;
			///操作标志
			f.ActionFlag = THOST_FTDC_AF_Delete;
			strcpy_s(f.InstrumentID, order->InstrumentID);
			strcpy_s(f.ExchangeID, order->ExchangeID);
			int iResult = api->ReqParkedOrderAction(&f, ++nReq);
			cerr << "--->>> ParkedOrderAction: " << ((iResult == 0) ? "Success" : "Failure") << endl;
		}
#endif
		ofstream out("cancelorder.txt", ios_base::app);
		if (!out)
		{
			cerr << "open errfile failure" << endl;
		}
		else
		{
			out << "strname:" << order->strID.c_str() << ",";
			out << "inst:" << order->InstrumentID << ",";
			out << "insertdate:" << LatestMarket[order->InstrumentID].TradingDay << ",";
			out << "orderinserttime: " << order->InsertTime << ",";
			out << "ordeactiontime:" << LatestMarket[order->InstrumentID].UpdateTime << ",";
			out << "price:" << order->LimitPrice << ",";
			out << "vol:" << order->VolumeTotalOriginal << ",";
			out << "direction:" << order->Direction << ",";
			out << "status:" << order->OrderStatus << ",";
			out << "OC:" << order->CombOffsetFlag[0] << endl;
			out.close();
		}
	}
}


void common::SendOrder(inputorder& porder)
{
	common& comm = common::GetInstance();
	CThostFtdcInputOrderField req;
	memset(&req, 0, sizeof(req));
	///经纪公司代码
	strcpy_s(req.BrokerID, comm.BROKER_ID);
	///投资者代码
	strcpy_s(req.InvestorID, comm.INVESTOR_ID);
	///合约代码
	strcpy_s(req.InstrumentID, porder.InstrumentID);
	///报单引用
	int iNextOrderRef = atoi(comm.ORDER_REF);
	iNextOrderRef++;
	sprintf_s(comm.ORDER_REF, "%d", iNextOrderRef);
	strcpy_s(req.OrderRef, comm.ORDER_REF);
	///买卖方向: 
	req.Direction = porder.Direction;
	///组合开平标志: 开仓
	req.CombOffsetFlag[0] = porder.CombOffsetFlag[0];
	///组合投机套保标志
	req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
	///数量: 1
	req.VolumeTotalOriginal = porder.Vol;
	if (porder.CombOffsetFlag[0] == THOST_FTDC_OF_Open)
	{
		req.TimeCondition = THOST_FTDC_TC_GFD;
		///报单价格条件: 限价
		req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
		///价格
		req.LimitPrice = porder.LimitPrice;
	}
	else
	{
		//if (porder->pur_num <= porder->max_purching)
		{
			req.TimeCondition = THOST_FTDC_TC_GFD;
			///报单价格条件: 限价
			req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			///价格
			req.LimitPrice = porder.LimitPrice;
		}
		//else if (porder->pur_num >= porder->max_purching)
		//{
		//req.TimeCondition = THOST_FTDC_TC_IOC;
		///报单价格条件: 市价
		//req.OrderPriceType = THOST_FTDC_OPT_AnyPrice;
		///价格
		//req.LimitPrice = 0;
		//}
	}
	///成交量类型: 任何数量
	req.VolumeCondition = THOST_FTDC_VC_AV;
	///最小成交量: 1
	req.MinVolume = 1;
	///触发条件: 立即
	req.ContingentCondition = THOST_FTDC_CC_Immediately;
	///强平原因: 非强平
	req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	///自动挂起标志: 否
	req.IsAutoSuspend = 0;
	///用户强评标志: 否
	req.UserForceClose = 0;
	req.RequestID = porder.RequestID;

	//先保存，再发单，保证订单能对上
	orderkey key;
	key.FrontID = comm.FRONT_ID;
	key.SessionID = comm.SESSION_ID;
	strcpy_s(key.OrderRef, req.OrderRef);
	order_t *order = new order_t;
	memset(order, 0, sizeof(order_t));
	order->strID = porder.strID;
	order->CombOffsetFlag[0] = req.CombOffsetFlag[0];
	order->Direction = req.Direction;
	strcpy_s(order->InstrumentID, req.InstrumentID);
	order->FrontID = key.FrontID;
	strcpy_s(order->InvestorID, req.InvestorID);
	order->LimitPrice = req.LimitPrice;
	strcpy_s(order->OrderRef, req.OrderRef);
	order->SessionID = key.SessionID;
	order->VolumeTotalOriginal = req.VolumeTotalOriginal;
	order->Purchasing = porder.Purchasing;
	order->WaitSecond = porder.WaitSecond;
	SYSTEMTIME ct;
	GetLocalTime(&ct);
	int h, m, s, ms;
	h = ct.wHour;
	m = ct.wMinute;
	s = ct.wSecond;
	ms = ct.wMilliseconds;
	double ss = 3600 * h + 60 * m + s + ms / 1000;
	order->LocalSec = ss;
	strcpy_s(order->InsertTime, comm.LatestMarket[order->InstrumentID].UpdateTime);
	order->OrderStatus = '#';
	order->UnitTick = porder.UnitTick;
	auto check1 = orderinfo.insert(make_pair(key, order));
	auto check2 = orderbook.insert(make_pair(key, order));
	if (!check1.second || !check2.second)
	{
		cerr << "order insert failure!" << endl;
	}
	else
	{
		c_checkorder.notify_all();
	}
#if !BACKTEST
	//发送报单
	if ((inststatus[order->InstrumentID] == THOST_FTDC_IS_Continous || inststatus[order->InstrumentID] == THOST_FTDC_IS_AuctionOrdering) && openpermit[porder.InstrumentID])
	{
		int iResult = comm.api->ReqOrderInsert(&req, ++comm.nReq);
		cerr << "--->>> 报单录入请求: " << ((iResult == 0) ? "成功" : "失败") << endl;
	}
	else if ((inststatus[order->InstrumentID] == THOST_FTDC_IS_BeforeTrading || inststatus[order->InstrumentID] == THOST_FTDC_IS_NoTrading) && openpermit[porder.InstrumentID])
	{
		CThostFtdcParkedOrderField f;
		memset(&f, 0, sizeof(f));
		///经纪公司代码
		strcpy_s(f.BrokerID, comm.BROKER_ID);
		///投资者代码
		strcpy_s(f.InvestorID, comm.INVESTOR_ID);
		///合约代码
		strcpy_s(f.InstrumentID, porder.InstrumentID);
		///报单引用
		int iNextOrderRef = atoi(comm.ORDER_REF);
		iNextOrderRef++;
		sprintf_s(comm.ORDER_REF, "%d", iNextOrderRef);
		strcpy_s(f.OrderRef, comm.ORDER_REF);
		///买卖方向: 
		f.Direction = porder.Direction;
		///组合开平标志: 开仓
		f.CombOffsetFlag[0] = porder.CombOffsetFlag[0];
		///组合投机套保标志
		f.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
		///数量: 1
		f.VolumeTotalOriginal = porder.Vol;
		if (porder.CombOffsetFlag[0] == THOST_FTDC_OF_Open)
		{
			f.TimeCondition = THOST_FTDC_TC_GFD;
			///报单价格条件: 限价
			f.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			///价格
			f.LimitPrice = porder.LimitPrice;
		}
		else
		{
			f.TimeCondition = THOST_FTDC_TC_GFD;
			///报单价格条件: 限价
			f.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			///价格
			f.LimitPrice = porder.LimitPrice;
		}
		///成交量类型: 任何数量
		f.VolumeCondition = THOST_FTDC_VC_AV;
		///最小成交量: 1
		f.MinVolume = 1;
		///触发条件: 立即
		f.ContingentCondition = THOST_FTDC_CC_Immediately;
		///强平原因: 非强平
		f.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
		///自动挂起标志: 否
		f.IsAutoSuspend = 0;
		///用户强评标志: 否
		f.UserForceClose = 0;
		int iResult = comm.api->ReqParkedOrderInsert(&f, ++comm.nReq);
		cerr << "--->>> 预埋单录入请求: " << ((iResult == 0) ? "成功" : "失败") << endl;
	}
	else if (inststatus[order->InstrumentID] == THOST_FTDC_IS_Closed)
	{
		openpermit[porder.InstrumentID] = false;
	}
#endif
	ofstream out("sendorder.txt", ios_base::app);
	if (!out)
	{
		cerr << "open errfile failure" << endl;
	}
	else
	{
		out << "strname:" << order->strID.c_str() << ",";
		out << "inst:" << order->InstrumentID << ",";
		out << "insertdate:" << ct.wYear * 10000 + ct.wMonth * 100 + ct.wDay << ",";
		out << "orderinserttime: " << order->InsertTime << ",";
		out << "orderlocalss:" << order->LocalSec << ",";
		out << "orderref" << order->OrderRef << ",";
		out << "price:" << order->LimitPrice << ",";
		out << "vol:" << order->VolumeTotalOriginal << ",";
		out << "direction:" << order->Direction << ",";
		out << "status:" << order->OrderStatus << ",";
		out << "OC:" << order->CombOffsetFlag[0] << endl;
		out.close();
	}

#if BACKTEST
	pback->OnRtnOrder(order);
	pback->OnRtnTrade(order);
#endif
}

bool common::AddStrategy(string ppinst, strategy *pstr)
{
	bool res = false;
	if (pStrmap.count(ppinst) <= 0)
	{
		pStr pstrategy;
		auto check = pStrmap.insert(make_pair(ppinst, pstrategy));
		if (!check.second)
		{
			cerr << "insert to pstrmap failure!" << endl;
			return res;
		}
	}
	pStr& rvect = pStrmap[ppinst];
	rvect.push_back(pstr);
	StrategyID strID = pstr->GetName();
	auto check = pStrID.insert(make_pair(strID, pstr));
	if (!check.second)
	{
		cerr << "insert to pstrid failure!" << endl;
		return res;
	}
	return true;
}


void common::CreateThread()
{
	ctpflag = true;
	strflag = true;
	warnflag = true;
	checkflag = true;
	T_ctp = std::thread(std::mem_fn(&common::CtpFun), this);
	T_str = std::thread(std::mem_fn(&common::Strategy), this);
	T_warn = std::thread(std::mem_fn(&common::Warning), this);
	T_checkorder = std::thread(std::mem_fn(&common::CheckOrder2), this);
}

void common::ReleaseThread()
{
	strflag = false;
	c_strategy.notify_all();
	T_str.join();
	warnflag = false;
	c_warn.notify_all();
	T_warn.join();
	ctpflag = false;
	c_ctp.notify_all();
	T_ctp.join();
	checkflag = false;
	c_checkorder.notify_all();
	T_checkorder.join();
	api->Release();
	api = NULL;
	md->Release();
	md = NULL;
	delete t;
	t = NULL;
	delete q;
	q = NULL;
	for (map<string, strategy*>::iterator it = pStrID.begin(); it != pStrID.end(); it++)
	{
		it->second = NULL;
	}

	for (map<string, pStr>::iterator iter = pStrmap.begin(); iter != pStrmap.end(); iter++)
	{
		pStr pvect = iter->second;
		for (vector<strategy*>::iterator it = pvect.begin(); it != pvect.end(); it++)
		{
			delete (*it);
			(*it) = NULL;
		}
	}
}

void common::CheckOrder2()
{
	while (checkflag)
	{
		common& comm = common::GetInstance();
		std::unique_lock<std::mutex> lock(m_checkorder);
		while (comm.orderbook.empty() && checkflag)
		{
			c_checkorder.wait(lock);
		}
		if (!comm.orderbook.empty())
		{
			for (orderinfo_map::iterator iter = comm.orderbook.begin(); iter != comm.orderbook.end();)
			{
				if ((iter->second->OrderStatus != THOST_FTDC_OST_PartTradedNotQueueing) &&
					(iter->second->OrderStatus != THOST_FTDC_OST_Canceled) &&
					(iter->second->OrderStatus != THOST_FTDC_OST_AllTraded) &&
					(iter->second->OrderStatus != THOST_FTDC_OST_NoTradeNotQueueing))
				{
					SYSTEMTIME ct;
					GetLocalTime(&ct);
					int h, m, s, ms;
					h = ct.wHour;
					m = ct.wMinute;
					s = ct.wSecond;
					ms = ct.wMilliseconds;
					double ss = 3600 * h + 60 * m + s + ms / 1000;
					bool timeflag = ss - iter->second->LocalSec > iter->second->WaitSecond;
					if (timeflag)
					{
						iter->second->LocalSec = ss;
						comm.CancelOrder(iter->second);
					}
					iter++;
					Sleep(20);
				}
				else
				{
					comm.orderbook.erase(iter++);
				}
			}
		}
	}
}

void common::CtpFun()
{
	while (ctpflag)
	{
		std::unique_lock<std::mutex> lock(m_ctp);
		while (trseq.empty() && ctpflag)
		{
			c_ctp.wait(lock);
		}
		if (!trseq.empty())
		{
			TR *tr = &trseq.front();
			if (tr->flag == ord)
			{
				CThostFtdcOrderField *pOrder = &tr->tuo.order;
				orderkey key;
				key.FrontID = pOrder->FrontID;
				key.SessionID = pOrder->SessionID;
				strcpy_s(key.OrderRef, pOrder->OrderRef);
				if (IsMyOrder(key))
				{
					order_t *order = orderinfo[key];
					CalFrozen(tr->tuo.order, *order);
					order->OrderStatus = pOrder->OrderStatus;
					if (order->Purchasing == 1 && pOrder->OrderStatus == THOST_FTDC_OST_Canceled)//撤单成功，并且需要追单的，重新下单
					{
						double price = LatestMarket[pOrder->InstrumentID].LastPrice;
						if (pOrder->Direction == THOST_FTDC_D_Buy)
						{
							price = LatestMarket[pOrder->InstrumentID].AskPrice1;
						}
						else if (pOrder->Direction == THOST_FTDC_D_Sell)
						{
							price = LatestMarket[pOrder->InstrumentID].BidPrice1;
						}
						inputorder torder;
						memset(&torder, 0, sizeof(torder));
						torder.strID = order->strID;
						strcpy_s(torder.InstrumentID, order->InstrumentID);
						torder.LimitPrice = price;
						torder.Direction = pOrder->Direction;
						torder.CombOffsetFlag[0] = pOrder->CombOffsetFlag[0];
						torder.Vol = pOrder->VolumeTotalOriginal - pOrder->VolumeTraded;
						torder.Purchasing = order->Purchasing;
						torder.PurNum = order->PurNum + 1;
						SendOrder(torder);
						order->Purchasing = 0;//避免重复追单
					}
					strcpy_s(order->ExchangeID, pOrder->ExchangeID);
					strcpy_s(order->OrderLocalID, pOrder->OrderLocalID);
					strcpy_s(order->TraderID, pOrder->TraderID);
					strcpy_s(order->InsertTime, pOrder->InsertTime);
					//order.VolumeTotalOriginal = pOrder->VolumeTotalOriginal;
					order->VolumeTotal = pOrder->VolumeTotal;
					order->VolumeTraded = pOrder->VolumeTraded;
					if (strlen(pOrder->ExchangeID) > 0 && strlen(pOrder->OrderLocalID) > 0 && strlen(pOrder->TraderID) > 0)
					{
						tradekey tkey;
						strcpy_s(tkey.ExchangeID, pOrder->ExchangeID);
						strcpy_s(tkey.TraderID, pOrder->TraderID);
						strcpy_s(tkey.OrderLocalID, pOrder->OrderLocalID);
						if (tmapo.count(tkey) <= 0)
						{
							auto check = tmapo.insert(make_pair(tkey, key));
							if (!check.second)
							{
								cerr << "insert to tradekey&orderkey failure!" << endl;
							}
						}
						if (tradeinfo.count(tkey) > 0)
						{
							trade_t& trade = tradeinfo[tkey];
							trade.strID = order->strID;
						}
						else
						{
							trade_t trade;
							memset(&trade, 0, sizeof(trade));
							trade.strID = order->strID;
							tradeinfo.insert(make_pair(tkey, trade));
						}
					}
					//回调策略相关函数
					pStrID[order->strID]->OnOrder(tr->tuo.order);
#if LOG
					ofstream out("order.txt", ios_base::app);
					if (!out)
					{
						cerr << "open errfile failure" << endl;
					}
					else
					{
						out << "strname:" << order->strID.c_str() << ",";
						out << "inst:" << order->InstrumentID << ",";
						out << "insertdate:" << pOrder->InsertDate << ",";
						out << "orderinserttime: " << order->InsertTime << ",";
						out << "orderlocalss:" << order->LocalSec << ",";
						out << "price:" << order->LimitPrice << ",";
						out << "vol:" << order->VolumeTotalOriginal << ",";
						out << "tradevol:" << order->VolumeTraded << ",";
						out << "remainvol:" << order->VolumeTotal << ",";
						out << "direction:" << order->Direction << ",";
						out << "status:" << order->OrderStatus << ",";
						out << "OC:" << order->CombOffsetFlag[0] << endl;
						out.close();
					}
#endif
				}
			}
			else if (tr->flag == trd)
			{
				CThostFtdcTradeField *pTrade = &tr->tuo.trade;
				//cout << "成交时间：" << pTrade->TradeTime << endl;
				tradekey key;
				strcpy_s(key.ExchangeID, pTrade->ExchangeID);
				strcpy_s(key.TraderID, pTrade->TraderID);
				strcpy_s(key.OrderLocalID, pTrade->OrderLocalID);
				if (IsMyTrade(key))
				{
					CalPos(pTrade, postype::InstPos);
					CalPos(pTrade, postype::StrPos);
					string strname;
					if (tradeinfo.count(key) > 0)
					{
						trade_t& trade = tradeinfo[key];
						trade.Price = (trade.Price*trade.Volume + pTrade->Price*pTrade->Volume) / (trade.Volume + pTrade->Volume);
						trade.Volume += pTrade->Volume;
						strcpy_s(trade.TradeTime, pTrade->TradeTime);
						strcpy_s(trade.TradingDay, pTrade->TradingDay);
						strcpy_s(trade.InstrumentID, pTrade->InstrumentID);
						strcpy_s(trade.InvestorID, pTrade->InvestorID);
						trade.Direction = pTrade->Direction;
						trade.HedgeFlag = pTrade->HedgeFlag;
						trade.OffsetFlag = pTrade->OffsetFlag;
						strcpy_s(trade.OrderLocalID, pTrade->OrderLocalID);
						strcpy_s(trade.ExchangeID, pTrade->ExchangeID);
						strcpy_s(trade.TraderID, pTrade->TraderID);
						if (trade.strID.empty())
						{
							orderkey& okey = tmapo[key];
							order_t *order = orderinfo[okey];
							trade.strID = order->strID;
						}
						strname = trade.strID;
					}
					else
					{
						trade_t trade;
						memset(&trade, 0, sizeof(trade));
						trade.Price = (trade.Price*trade.Volume + pTrade->Price*pTrade->Volume) / (trade.Volume + pTrade->Volume);
						trade.Volume += pTrade->Volume;
						strcpy_s(trade.TradeTime, pTrade->TradeTime);
						strcpy_s(trade.TradingDay, pTrade->TradingDay);
						strcpy_s(trade.InstrumentID, pTrade->InstrumentID);
						strcpy_s(trade.InvestorID, pTrade->InvestorID);
						trade.Direction = pTrade->Direction;
						trade.HedgeFlag = pTrade->HedgeFlag;
						trade.OffsetFlag = pTrade->OffsetFlag;
						strcpy_s(trade.OrderLocalID, pTrade->OrderLocalID);
						strcpy_s(trade.ExchangeID, pTrade->ExchangeID);
						strcpy_s(trade.TraderID, pTrade->TraderID);
						strname = trade.strID;
						tradeinfo.insert(make_pair(key, trade));
					}
					//回调策略相关函数
					pStrID[strname]->OnTrade(tr->tuo.trade);
#if LOG
					ofstream out("trade.txt", ios_base::app);
					if (!out)
					{
						cerr << "open errfile failure" << endl;
					}
					else
					{
						out << "strname:" << strname.c_str() << ",";
						out << "inst:" << pTrade->InstrumentID << ",";
						out << "tradedate:" << pTrade->TradeDate << ",";
						out << "tradetime: " << pTrade->TradeTime << ",";
						out << "price:" << pTrade->Price << ",";
						out << "tradevol:" << pTrade->Volume << ",";
						out << "direction:" << pTrade->Direction << ",";
						out << "OC:" << pTrade->OffsetFlag << endl;
						out.close();
					}
#endif
				}
			}
			trseq.pop_front();
		}
	}
}

void common::Strategy()
{
	while (strflag)
	{
		std::unique_lock<std::mutex> lock(m_strategy);

		while (mktdata.empty() && strflag)
		{
			c_strategy.wait(lock);
		}
		if (!mktdata.empty())
		{
			CThostFtdcDepthMarketDataField  mkt = mktdata.front();
			char temp[9];
			strncpy_s(temp, mkt.UpdateTime, 9);
			string pkey = mkt.InstrumentID;
#if BACKTEST
			pkey = pkey.substr(0, 2);
#endif
			map<string, pStr>::iterator iter = pStrmap.find(pkey);
			if (iter != pStrmap.end())
			{
				pStr pvect = iter->second;
				for (vector<strategy*>::iterator it = pvect.begin(); it != pvect.end(); it++)
				{
					//回调策略相关函数
					if ((*it) != NULL)
					{
						(*it)->OnTick(mkt);
					}
				}
			}
#if !BACKTEST
			if (strcmp(temp, "14:59:59") >= 0 && strcmp(temp, "21:00:00") < 0)
			{
				SYSTEMTIME ct;
				GetLocalTime(&ct);
				int h, m, s;
				h = ct.wHour;
				m = ct.wMinute;
				s = ct.wSecond;
				double ss = 10000 * h + 100 * m + s;
				if (ss >= 143000 && ss < 205000)
				{
					common& comm = common::GetInstance();
					CThostFtdcQryTradingAccountField req;
					memset(&req, 0, sizeof(req));
					strcpy_s(req.BrokerID, comm.cfgfile["BROKER_ID"].c_str());
					strcpy_s(req.InvestorID, comm.cfgfile["INVESTOR_ID"].c_str());
					int Res = comm.api->ReqQryTradingAccount(&req, ++comm.nReq);
					std::cout << "资金查询：" << ((Res == 0) ? "成功" : "失败") << std::endl;//资金查询
					comm.c_main.notify_all();
				}
			}
#endif
			mktdata.pop_front();
		}
	}
}

void common::Warning()
{
	while (warnflag)
	{
		std::unique_lock<std::mutex> lock(m_warn);
		c_warn.wait(lock);
		LPCWSTR lpcwStr = TEXT("by.wav");
		PlaySound(lpcwStr, NULL, SND_FILENAME | SND_SYNC);
	}
}