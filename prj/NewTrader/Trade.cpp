#include "Trade.h"
#include "Quote.h"
#include "common.h"
#include <process.h>
#include <windows.h>
#include <iostream>
#include <fstream>

using namespace std;


Trade::Trade(void)
{
}


Trade::~Trade(void)
{
}


//将trade.h文件中对应On函数后面的 {};注销掉即可.

void Trade::Start()
{
	common& com = common::GetInstance();
	com.api = CThostFtdcTraderApi::CreateFtdcTraderApi("./ftlog");
	com.api->RegisterSpi(this);
	com.api->SubscribePrivateTopic(THOST_TERT_QUICK);
	com.api->SubscribePublicTopic(THOST_TERT_QUICK);
	//com.api->RegisterFront("tcp://180.168.212.75:41205");.
	//com.api->RegisterFront("tcp://180.168.212.76:41205");.
	//com.api->RegisterFront("tcp://59.46.173.139:41205");.
	com.api->RegisterFront(com.tdfront1);
	com.api->Init();
}

/*
void Stop()
{
	common& com = common::GetInstance();
	com.api->Release();
}*/

void Trade::OnFrontConnected()
{
#if SIMNOW

	CThostFtdcReqUserLoginField f;
	memset(&f, 0, sizeof(f));
	common& comm = common::GetInstance();
	strcpy_s(f.BrokerID, comm.cfgfile["BROKER_ID"].c_str());
	strcpy_s(f.UserID, comm.cfgfile["INVESTOR_ID"].c_str());
	strcpy_s(f.Password, comm.cfgfile["PASSWORD"].c_str());
	comm.api->ReqUserLogin(&f, ++comm.nReq);

#else
	
	CThostFtdcReqAuthenticateField f;
	memset(&f, 0, sizeof(f));
	common& comm = common::GetInstance();
	strcpy_s(f.BrokerID, comm.cfgfile["BROKER_ID"].c_str());
	strcpy_s(f.UserID, comm.cfgfile["INVESTOR_ID"].c_str());
	strcpy_s(f.UserProductInfo,"N161115WLG");
	strcpy_s(f.AuthCode,"20161115WLGT0002");
	comm.api->ReqAuthenticate(&f, ++comm.nReq);

#endif
}

void Trade::OnRspAuthenticate(CThostFtdcRspAuthenticateField *pRspAuthenticateField, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	CThostFtdcReqUserLoginField f;
	memset(&f, 0, sizeof(f));
	common& comm = common::GetInstance();
	strcpy_s(f.BrokerID, comm.cfgfile["BROKER_ID"].c_str());
	strcpy_s(f.UserID, comm.cfgfile["INVESTOR_ID"].c_str());
	strcpy_s(f.Password, comm.cfgfile["PASSWORD"].c_str());
	comm.api->ReqUserLogin(&f, ++comm.nReq);
}

void Trade::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (bIsLast && pRspInfo->ErrorID == 0)
	{
		// 保存会话参数.
		common& comm = common::GetInstance();
		comm.FRONT_ID = pRspUserLogin->FrontID;
		comm.SESSION_ID = pRspUserLogin->SessionID;
		int iNextOrderRef = atoi(pRspUserLogin->MaxOrderRef);
		iNextOrderRef++;
		sprintf_s(comm.ORDER_REF, "%d", iNextOrderRef);
		CThostFtdcSettlementInfoConfirmField f;
		memset(&f, 0, sizeof(f));
		strcpy_s(f.BrokerID, comm.cfgfile["BROKER_ID"].c_str());
		strcpy_s(f.InvestorID, comm.cfgfile["INVESTOR_ID"].c_str());
		int result = comm.api->ReqSettlementInfoConfirm(&f, ++comm.nReq);
		cout << "结算确认：" << ((result == 0) ? "成功" : "失败") << endl;//结算确认.
	}
}

void Trade::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (bIsLast && pRspInfo->ErrorID == 0)
	{
		CThostFtdcQryTradingAccountField req;
		memset(&req, 0, sizeof(req));
		common& comm = common::GetInstance();
		strcpy_s(req.BrokerID, comm.cfgfile["BROKER_ID"].c_str());
		strcpy_s(req.InvestorID, comm.cfgfile["INVESTOR_ID"].c_str());
		int Res = comm.api->ReqQryTradingAccount(&req, ++comm.nReq);
		cout << "资金查询：" << ((Res == 0) ? "成功" : "失败") << endl;//资金查询.
	}
}

void Trade::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	common& comm = common::GetInstance();
	if (pTradingAccount && bIsLast)
	{
		
		comm.moneyavaible = pTradingAccount->Available;
		
		ofstream out("account.txt", ios_base::app);
		if (!out)
		{
			cerr << "open errfile failure" << endl;
		}
		else
		{
			out << "tradingday:" << pTradingAccount->TradingDay << ",";
			out << "accountid:" << pTradingAccount->AccountID << ",";
			out << "closeprofit:" << pTradingAccount->CloseProfit << ",";
			out << "commission:" << pTradingAccount->Commission << ",";
			out << "posprofit:" << pTradingAccount->PositionProfit << ",";
			out << "available:" << pTradingAccount->Available << endl;
			out.close();
		}
		/*CThostFtdcQryInvestorPositionDetailField req = { 0 };
		strcpy_s(req.BrokerID, comm.cfgfile["BROKER_ID"].c_str());
		strcpy_s(req.InvestorID, comm.cfgfile["INVESTOR_ID"].c_str());
		int Res = 0;
		do {
			Sleep(2000);
			Res = comm.api->ReqQryInvestorPositionDetail(&req, ++comm.nReq);
			cout << "持仓明细查询：" << ((Res == 0) ? "成功" : "失败") << endl;
		} while (Res != 0 && comm.qryflag);*/
	}

	//启动行情..
	comm.md = CThostFtdcMdApi::CreateFtdcMdApi("./qlog");
	comm.q = new Quote();
	comm.md->RegisterSpi((CThostFtdcMdSpi*)comm.q);
	comm.md->RegisterFront(comm.mdfront1);
	//com.md->RegisterFront("tcp://180.168.212.76:41213");..
	//com.md->RegisterFront(com.mdfront1);..
	comm.md->Init();
}

void Trade::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (bIsLast && pRspInfo && pRspInfo->ErrorID != 0)
	{
		orderkey key;
		common& comm = common::GetInstance();
		key.FrontID = comm.FRONT_ID;
		key.SessionID = comm.SESSION_ID;
		strcpy_s(key.OrderRef, pInputOrder->OrderRef);
		if (comm.IsMyOrder(key))
		{
			comm.c_warn.notify_all();
			cout << "报单错误：" << pRspInfo->ErrorMsg << endl;
			order_t *order = comm.orderinfo[key];
			//回调策略相关函数.
			comm.pStrID[order->strID]->OnRspOrderInsert(pInputOrder, pRspInfo, nRequestID, bIsLast);
			switch (pRspInfo->ErrorID)
			{
			case 15:
			{
				double price = comm.LatestMarket[pInputOrder->InstrumentID].LastPrice;
				if (pInputOrder->Direction == THOST_FTDC_D_Buy)
				{
					price = comm.LatestMarket[pInputOrder->InstrumentID].AskPrice1;
				}
				else if (pInputOrder->Direction == THOST_FTDC_D_Sell)
				{
					price = comm.LatestMarket[pInputOrder->InstrumentID].BidPrice1;
				}
				inputorder torder;
				memset(&torder, 0, sizeof(torder));
				torder.strID = order->strID;
				strcpy_s(torder.InstrumentID, order->InstrumentID);
				torder.LimitPrice = price;
				torder.Direction = pInputOrder->Direction;
				torder.CombOffsetFlag[0] = pInputOrder->CombOffsetFlag[0];
				if (pInputOrder->VolumeTotalOriginal <= 0)
				{
					pInputOrder->VolumeTotalOriginal++;
				}
				torder.Vol = pInputOrder->VolumeTotalOriginal;
				torder.Purchasing = order->Purchasing;
				comm.SendOrder(torder);
				break;
			}
			case 22:
			{
				inputorder torder;
				memset(&torder, 0, sizeof(torder));
				torder.strID = order->strID;
				strcpy_s(torder.InstrumentID, pInputOrder->InstrumentID);
				torder.LimitPrice = pInputOrder->LimitPrice;
				torder.Direction = pInputOrder->Direction;
				torder.CombOffsetFlag[0] = pInputOrder->CombOffsetFlag[0];
				torder.Vol = pInputOrder->VolumeTotalOriginal;
				torder.Purchasing = order->Purchasing;
				comm.SendOrder(torder);
				break;
			}
			case 17:
			case 29:
			{
				comm.openpermit[order->InstrumentID] = false;
				break;
			}
			case 30:
			{
				poskey pkey;
				pkey.strID = order->strID;
				strcpy_s(pkey.InstrumentID, order->InstrumentID);
				if (comm.strpos.count(pkey) > 0)
				{
					position_t& pos = comm.strpos[pkey];
					if (order->Direction == THOST_FTDC_D_Buy && pos.PosLong - pos.CLongFrozen > 0)
					{
						inputorder torder;
						memset(&torder, 0, sizeof(torder));
						torder.strID = order->strID;
						strcpy_s(torder.InstrumentID, pInputOrder->InstrumentID);
						torder.LimitPrice = pInputOrder->LimitPrice;
						torder.Direction = pInputOrder->Direction;
						torder.CombOffsetFlag[0] = pInputOrder->CombOffsetFlag[0];
						torder.Vol = pos.PosLong - pos.CLongFrozen;
						torder.Purchasing = order->Purchasing;
						comm.SendOrder(torder);
					}
					else if (order->Direction == THOST_FTDC_D_Sell && pos.PosShort - pos.CShortFrozen > 0)
					{
						inputorder torder;
						memset(&torder, 0, sizeof(torder));
						torder.strID = order->strID;
						strcpy_s(torder.InstrumentID, pInputOrder->InstrumentID);
						torder.LimitPrice = pInputOrder->LimitPrice;
						torder.Direction = pInputOrder->Direction;
						torder.CombOffsetFlag[0] = pInputOrder->CombOffsetFlag[0];
						torder.Vol = pos.PosShort - pos.CShortFrozen;
						torder.Purchasing = order->Purchasing;
						comm.SendOrder(torder);
					}
				}
				break;
			}
			case 50:
			{
				inputorder torder;
				memset(&torder, 0, sizeof(torder));
				torder.strID = order->strID;
				strcpy_s(torder.InstrumentID, pInputOrder->InstrumentID);
				torder.LimitPrice = pInputOrder->LimitPrice;
				torder.Direction = pInputOrder->Direction;
				torder.CombOffsetFlag[0] = THOST_FTDC_OF_CloseYesterday;
				poskey pkey;
				pkey.strID = order->strID;
				strcpy_s(pkey.InstrumentID, order->InstrumentID);
				position_t& rp = comm.strpos[pkey];
				if (order->Direction == THOST_FTDC_D_Sell)
				{
					torder.Vol = rp.YdPosShort - rp.CShortFrozen;
				}
				else if (order->Direction == THOST_FTDC_D_Buy)
				{
					torder.Vol = rp.YdPosLong - rp.CLongFrozen;
				}
				torder.Purchasing = order->Purchasing;
				if (torder.Vol > 0)
				{
					comm.SendOrder(torder);
				}
				break;
			}
			case 51:
			{
				inputorder torder;
				memset(&torder, 0, sizeof(torder));
				torder.strID = order->strID;
				strcpy_s(torder.InstrumentID, pInputOrder->InstrumentID);
				torder.LimitPrice = pInputOrder->LimitPrice;
				torder.Direction = pInputOrder->Direction;
				torder.CombOffsetFlag[0] = THOST_FTDC_OF_CloseToday;
				poskey pkey;
				pkey.strID = order->strID;
				strcpy_s(pkey.InstrumentID, order->InstrumentID);
				position_t& rp = comm.strpos[pkey];
				if (order->Direction == THOST_FTDC_D_Sell)
				{
					torder.Vol = rp.PosShort - rp.CShortFrozen;
				}
				else if (order->Direction == THOST_FTDC_D_Buy)
				{
					torder.Vol = rp.PosLong - rp.CLongFrozen;
				}
				torder.Purchasing = order->Purchasing;
				if (torder.Vol > 0)
				{
					comm.SendOrder(torder);
				}
				break;
			}
			case 42:
			{
				CThostFtdcSettlementInfoConfirmField f;
				memset(&f, 0, sizeof(f));
				strcpy_s(f.BrokerID, comm.cfgfile["BROKER_ID"].c_str());
				strcpy_s(f.InvestorID, comm.cfgfile["INVESTOR_ID"].c_str());
				int result = comm.api->ReqSettlementInfoConfirm(&f, ++comm.nReq);
				cout << "结算确认：" << ((result == 0) ? "成功" : "失败") << endl;//结算确认.
				break;
			}
			case 21:
			case 91:
			{
				cout << "exchange error" << endl;
				break;
			}
			default: break;
			}
			ofstream out("err.txt", ios_base::app);
			if (!out)
			{
				cerr << "open errfile failure" << endl;
			}
			else
			{
				out << "strID:" << order->strID.c_str() << ",";
				out << "inst:" << pInputOrder->InstrumentID << ",";
				out << "errid: " << pRspInfo->ErrorID << ",";
				out << "errmsg: " << pRspInfo->ErrorMsg << ",";
				out << "tradeday:" << comm.LatestMarket[pInputOrder->InstrumentID].TradingDay << ",";
				out << "orderinserttime: " << comm.LatestMarket[order->InstrumentID].UpdateTime << ",";
				out << "orderlocalss:" << order->LocalSec << ",";
				out << "frontid:" << key.FrontID << ",";
				out << "sessionid:" << key.SessionID << ",";
				out << "orderref:" << key.OrderRef << ",";
				out << "orderstatus:" << order->OrderStatus << endl;
				out.close();
			}
		}
	}
}

void Trade::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (bIsLast && pRspInfo && pRspInfo->ErrorID != 0)
	{
		common& comm = common::GetInstance();
		cout << "撤单错误：" << pRspInfo->ErrorMsg << endl;
		cout << pInputOrderAction->InstrumentID << endl;
		orderkey key;
		key.FrontID = pInputOrderAction->FrontID;
		key.SessionID = pInputOrderAction->SessionID;
		strcpy_s(key.OrderRef, pInputOrderAction->OrderRef);
		if (comm.IsMyOrder(key))
		{
			comm.c_warn.notify_all();
			order_t *order = comm.orderinfo[key];
			//回调策略相关函数.
			comm.pStrID[order->strID]->OnRspOrderAction(pInputOrderAction, pRspInfo, nRequestID, bIsLast);
			switch (pRspInfo->ErrorID)
			{
			case 25:
			case 26:
			{
				order->OrderStatus = THOST_FTDC_OST_Canceled;
				if (order->Purchasing != 0)
				{
					order->Purchasing = 0;
					cout << "stop purchasing" << endl;
				}
				break;
			}
			case 91:
			{
				cout << "exchange error" << endl;
				break;
			}
			default: break;
			}
			ofstream out("err.txt", ios_base::app);
			if (!out)
			{
				cerr << "open errfile failure" << endl;
			}
			else
			{
				out << "inst:" << order->InstrumentID << ",";
				out << "errid: " << pRspInfo->ErrorID << ",";
				out << "errmsg: " << pRspInfo->ErrorMsg << ",";
				out << "tradeday:" << comm.LatestMarket[order->InstrumentID].TradingDay << ",";
				out << "orderinserttime: " << comm.LatestMarket[order->InstrumentID].UpdateTime << ",";
				out << "orderlocalss:" << order->LocalSec << ",";
				out << "frontid:" << key.FrontID << ",";
				out << "sessionid:" << key.SessionID << ",";
				out << "orderref:" << key.OrderRef << ",";
				out << "orderstatus:" << order->OrderStatus << endl;
				out.close();
			}
		}
	}
}

void Trade::OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	common& comm = common::GetInstance();
	if (bIsLast && pRspInfo && pRspInfo->ErrorID != 0)
	{
		cout << pRspInfo->ErrorMsg << endl;
		cout << pRspInfo->ErrorID << endl;
		//common& comm = common::GetInstance();.
		ofstream out("err.txt", ios_base::app);
		if (!out)
		{
			cerr << "open errfile failure" << endl;
		}
		else
		{
			out << "inst:" << pOrder->InstrumentID << ",";
			out << "errid: " << pRspInfo->ErrorID << ",";
			out << "errmsg: " << pRspInfo->ErrorMsg << ",";
			out << "tradeday:" << comm.LatestMarket[pOrder->InstrumentID].TradingDay << ",";
			out << "orderinserttime: " << comm.LatestMarket[pOrder->InstrumentID].UpdateTime << endl;
			out.close();
		}
	}
	else if (bIsLast && pOrder != NULL && (pOrder->OrderStatus == THOST_FTDC_OST_PartTradedQueueing || pOrder->OrderStatus == THOST_FTDC_OST_NoTradeQueueing))
	{
		CThostFtdcInputOrderActionField req;
		memset(&req, 0, sizeof(req));
		///经纪公司代码..
		strcpy_s(req.BrokerID, pOrder->BrokerID);
		///投资者代码..
		strcpy_s(req.InvestorID, pOrder->InvestorID);
		///报单引用..
		strcpy_s(req.OrderRef, pOrder->OrderRef);
		///前置编号..
		req.FrontID = pOrder->FrontID;
		///会话编号..
		req.SessionID = pOrder->SessionID;
		///操作标志..
		req.ActionFlag = THOST_FTDC_AF_Delete;
		strcpy_s(req.InstrumentID, pOrder->InstrumentID);
		strcpy_s(req.ExchangeID, pOrder->ExchangeID);
		strcpy_s(req.OrderSysID, pOrder->OrderSysID);
		//common& comm = common::GetInstance();.
		int iResult = comm.api->ReqOrderAction(&req, ++comm.nReq);
		cerr << "--->>> OrderAction: " << ((iResult == 0) ? "Success" : "Failure") << endl;
	}
	if (bIsLast)
	{
		comm.qryflag = true;
	}
}

void Trade::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
	common& comm = common::GetInstance();
	TR tr;
	memset(&tr, 0, sizeof(tr));
	tr.flag = ord;
	memcpy(&tr.tuo.order, pOrder, sizeof(CThostFtdcOrderField));
	std::lock_guard<std::mutex> lock(comm.m_ctp);
	comm.trseq.push_back(tr);
	comm.c_ctp.notify_all();
}

void Trade::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
	common& comm = common::GetInstance();
	TR tr;
	memset(&tr, 0, sizeof(tr));
	tr.flag = trd;
	memcpy(&tr.tuo.order, pTrade, sizeof(CThostFtdcTradeField));
	std::lock_guard<std::mutex> lock(comm.m_ctp);
	comm.trseq.push_back(tr);
	comm.c_ctp.notify_all();
}

void Trade::OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus)
{
	common& comm = common::GetInstance();
	if (comm.inststatus.count(pInstrumentStatus->InstrumentID) <= 0)
	{
		auto check = comm.inststatus.insert(make_pair(pInstrumentStatus->InstrumentID, pInstrumentStatus->InstrumentStatus));
		if (!check.second)
		{
			cerr << "insert to instrument status failure!" << endl;
		}
	}
	else
	{
		comm.inststatus[pInstrumentStatus->InstrumentID] = pInstrumentStatus->InstrumentStatus;
	}
}


static void CalPos(position_t *ppos, CThostFtdcInvestorPositionDetailField *pInvestorPosition)
{
	if (pInvestorPosition->Direction == THOST_FTDC_D_Buy){
		double amount = (ppos->PosCostLong*(ppos->PosLong + ppos->YdPosLong) + pInvestorPosition->Volume*pInvestorPosition->OpenPrice);
		TThostFtdcVolumeType vol = (ppos->PosLong + ppos->YdPosLong + pInvestorPosition->Volume);
		ppos->PosCostLong = amount / vol;
		if (strcmp(pInvestorPosition->OpenDate, pInvestorPosition->TradingDay) == 0)
			ppos->PosLong += pInvestorPosition->Volume;
		else
			ppos->YdPosLong += pInvestorPosition->Volume;
	}
	else if (pInvestorPosition->Direction == THOST_FTDC_D_Sell){
		double amount = (ppos->PosCostShort*(ppos->PosShort + ppos->YdPosShort) + pInvestorPosition->Volume*pInvestorPosition->OpenPrice);
		TThostFtdcVolumeType vol = (ppos->PosShort + ppos->YdPosShort + pInvestorPosition->Volume);
		ppos->PosCostShort = amount / vol;
		if (strcmp(pInvestorPosition->OpenDate, pInvestorPosition->TradingDay) == 0)
			ppos->PosShort += pInvestorPosition->Volume;
		else
			ppos->YdPosShort += pInvestorPosition->Volume;
	}
}


void Trade::OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	common& comm = common::GetInstance();
	if (bIsLast && pRspInfo && pRspInfo->ErrorID != 0)
	{
		cout << pRspInfo->ErrorMsg << endl;
		cout << pRspInfo->ErrorID << endl;
		ofstream out("err.txt", ios_base::app);
		if (!out)
		{
			cerr << "open errfile failure" << endl;
		}
		else
		{
			out << "errid: " << pRspInfo->ErrorID << ",";
			out << "errmsg: " << pRspInfo->ErrorMsg << endl;
			out.close();
		}
	}
	else if (pInvestorPosition)
	{
		tradeidkey key;
		strcpy_s(key.TradingDay, pInvestorPosition->TradingDay);
		string TradeID = trim(pInvestorPosition->TradeID);
		strcpy_s(key.TradeID, TradeID.c_str());
		cout << key.TradingDay << ","<< TradeID << "," << pInvestorPosition->InstrumentID << "," << pInvestorPosition->Direction << ",";
		cout << "Volume:" << pInvestorPosition->Volume << ",OpenPrice:" << pInvestorPosition->OpenPrice
			<< ",CloseVolume:" << pInvestorPosition->CloseVolume << ",CloseAmount:" << pInvestorPosition->CloseAmount << "\n";

		if (pInvestorPosition->Volume > 0)
		{			
			if (comm.map_tradeid_straid_.count(key) <= 0)
			{
				cerr << "no item in sqlite TradeID:" << TradeID << endl;
				//if (bIsLast)comm.c_reqPosdetail.notify_one();
				if (bIsLast) goto END;
				return;
			}
			poskey pkey;
			pkey.strID = comm.map_tradeid_straid_[key];
			strcpy_s(pkey.InstrumentID, pInvestorPosition->InstrumentID);
			if (comm.strpos.count(pkey) <= 0)
			{
				position_t pos;
				memset(&pos, 0, sizeof(pos));
				strcpy_s(pos.InstrumentID, pkey.InstrumentID);
				strcpy_s(pos.InvestorID, pInvestorPosition->InvestorID);
				pos.strID = pkey.strID;
				auto check = comm.strpos2.insert(make_pair(pkey, pos));
				if (!check.second)
				{
					cerr << "insert to strpos2 failure!" << endl;
				}
			}

			if (comm.instpos.count(pkey.InstrumentID) <= 0)
			{
				position_t pos;
				memset(&pos, 0, sizeof(pos));
				strcpy_s(pos.InstrumentID, pkey.InstrumentID);
				strcpy_s(pos.InvestorID, pInvestorPosition->InvestorID);
				pos.strID = pkey.strID;
				auto check = comm.instpos.insert(make_pair(pkey.InstrumentID, pos));
				if (!check.second)
				{
					cerr << "insert to strpos2 failure!" << endl;
				}
			}
			position_t &pos_inst = comm.instpos[pkey.InstrumentID];
			position_t &pos_stra = comm.strpos[pkey];

			CalPos(&pos_inst, pInvestorPosition);
			CalPos(&pos_stra, pInvestorPosition);		

			TradeidStraidItem item;
			strcpy_s(item.TradingDay, pInvestorPosition->TradingDay);
			strcpy_s(item.TradeID, TradeID.c_str());
			item.StraId = pkey.strID;
			comm.vec_tradeid_straid_.push_back(item);
		}
	}
	if (bIsLast)
	{
	END:
		comm.UpdateTradeid2Strid();
		/*if (!comm.Insert_Pos()){
			cerr << "Insert_Pos fail" << endl;
		}
		if (!comm.ComparePos_Mem2Ctp()){
			ofstream out("err.txt", ios_base::app);
			if (!out)
			{
				cerr << "open errfile failure" << endl;
			}
			else
			{
				out << "pos in memory and ctp ,is not same" << endl;
				out.close();
			}
		}

		comm.c_reqPosdetail.notify_one();*/
	}
}

void Trade::OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo)
{
	common& comm = common::GetInstance();
	if (pRspInfo && pRspInfo->ErrorID != 0)
	{
		cout << pRspInfo->ErrorMsg << endl;
		cout << pRspInfo->ErrorID << endl;
		ofstream out("err.txt", ios_base::app);
		if (!out)
		{
			cerr << "open errfile failure" << endl;
		}
		else
		{
			out << "inst:" << pInputOrder->InstrumentID << ",";
			out << "errid: " << pRspInfo->ErrorID << ",";
			out << "errmsg: " << pRspInfo->ErrorMsg << ",";
			out << "tradeday:" << comm.LatestMarket[pInputOrder->InstrumentID].TradingDay << ",";
			out << "orderinserttime: " << comm.LatestMarket[pInputOrder->InstrumentID].UpdateTime << endl;
			out.close();
		}
	}
}

void Trade::OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo)
{
	common& comm = common::GetInstance();
	if (pRspInfo && pRspInfo->ErrorID != 0)
	{
		cout << pRspInfo->ErrorMsg << endl;
		cout << pRspInfo->ErrorID << endl;
		ofstream out("err.txt", ios_base::app);
		if (!out)
		{
			cerr << "open errfile failure" << endl;
		}
		else
		{
			out << "inst:" << pOrderAction->InstrumentID << ",";
			out << "errid: " << pRspInfo->ErrorID << ",";
			out << "errmsg: " << pRspInfo->ErrorMsg << ",";
			out << "tradeday:" << comm.LatestMarket[pOrderAction->InstrumentID].TradingDay << ",";
			out << "orderinserttime: " << comm.LatestMarket[pOrderAction->InstrumentID].UpdateTime << endl;
			out.close();
		}
	}
}