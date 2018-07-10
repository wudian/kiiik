#include "strategy.h"
#include "common.h"
//#include <iostream>..
//#include <algorithm>..
#pragma warning(disable : 4244)
//using namespace std;..

strategy::strategy(string addr) :ini_addr(addr)
{
	memset(&preData, 0, sizeof(preData));
	Init();
}

bool strategy::Init()
{
	if (inited)
	{
		return inited;
	}
	GetComValue(ini_addr,"cpars");
	//strname = cpar.pos.strID;.
	inited = true;
	return inited;
}


static //字符串分割函数.
std::vector<std::string> split(std::string str, std::string pattern)
{
	std::string::size_type pos;
	std::vector<std::string> result;
	str += pattern;//扩展字符串以方便操作.
	int size = str.size();

	for (int i = 0; i < size; i++)
	{
		pos = str.find(pattern, i);
		if (pos < size)
		{
			std::string s = str.substr(i, pos - i);
			string r = trim(s);
			if (r != "")
				result.push_back(r);
			i = pos + pattern.size() - 1;
		}
	}
	return result;
}

void strategy::GetComValue(string addr,char *sect)
{
	inihelp *pini = new inihelp(addr.c_str());

	/*****公共部分初始化*******/
	TCHAR section[30];
	pini->CharToTchar(sect,section);
	//LPCTSTR section = TEXT("cpars");..
	/*cpar.MarginRate = pini->ReadFloat(section, TEXT("marginrate"), 0.09);
	
	cpar.UnitTick = pini->ReadFloat(section, TEXT("unittick"), 5);
	
	cpar.Multiple = pini->ReadInteger(section, TEXT("multiple"), 10);
	cpar.pos.CLongFrozen = pini->ReadInteger(section, TEXT("pos.CLongFrozen"), 0);
	cpar.pos.CShortFrozen = pini->ReadInteger(section, TEXT("pos.CShortFrozen"), 0);
	cpar.pos.OLongFrozen = pini->ReadInteger(section, TEXT("pos.OLongFrozen"), 0);
	cpar.pos.OShortFrozen = pini->ReadInteger(section, TEXT("pos.OShortFrozen"), 0);
	cpar.pos.PosCostLong = pini->ReadInteger(section, TEXT("pos.PosCostLong"), 0);
	cpar.pos.PosCostShort = pini->ReadInteger(section, TEXT("pos.PosCostShort"), 0);
	cpar.pos.PosLong = pini->ReadInteger(section, TEXT("pos.PosLong"), 0);
	cpar.pos.PosShort = pini->ReadInteger(section, TEXT("pos.PosShort"), 0);
	cpar.pos.YdPosLong = pini->ReadInteger(section, TEXT("pos.PosYdLong"), 0);
	cpar.pos.YdPosShort = pini->ReadInteger(section, TEXT("pos.PosYdShort"), 0);*/
	LPCTSTR investid=pini->ReadString(section, TEXT("InvestorID"), TEXT("00"));
	char investor[15];
	pini->TcharToChar(investid,investor);
	if (strcmp(investor, "00") != 0)
	{
		InvestorID = investor;
	}
	else
	{
		common& comm = common::GetInstance();
		InvestorID = comm.INVESTOR_ID;
	}

	LPCTSTR lstr = pini->ReadString(section, TEXT("strID"), TEXT("00"));
	char strid[15];
	pini->TcharToChar(lstr, strid);
	strname = strid;

	LPCTSTR lppinst = pini->ReadString(section, TEXT("InstrumentID"), TEXT("00"));
	char ppinst[31];
	pini->TcharToChar(lppinst, ppinst);
	if (strcmp(investor, "00") == 0)
	{
		cerr << strname.c_str() <<": Instrument Error!"<< endl;
		InstrumentID="000000";
	}
	else
	{
		InstrumentID = ppinst;
	}

	cpar.MoneyRate = pini->ReadFloat(section, TEXT("moneyrate"), 0.2);
	cpar.RequestID = pini->ReadInteger(section, TEXT("RequestID"), 100203);

	vecInsts_ = split(InstrumentID, ",");
	cpar.pos_num = vecInsts_.size();
	cpar.pos = new position_t[cpar.pos_num];
	for (int i = 0; i < cpar.pos_num;++i)
	{
		TCHAR section[30];
		pini->CharToTchar(vecInsts_[i].c_str(), section);
		cpar.pos[i].strID = strname;
		strcpy_s(cpar.pos[i].InvestorID, investor);
		strcpy_s(cpar.pos[i].InstrumentID, vecInsts_[i].c_str());
		cpar.pos[i].CLongFrozen = pini->ReadInteger(section, TEXT("CLongFrozen"), 0);
		cpar.pos[i].CShortFrozen = pini->ReadInteger(section, TEXT("CShortFrozen"), 0);
		cpar.pos[i].OLongFrozen = pini->ReadInteger(section, TEXT("OLongFrozen"), 0);
		cpar.pos[i].OShortFrozen = pini->ReadInteger(section, TEXT("OShortFrozen"), 0);
		cpar.pos[i].PosCostLong = pini->ReadInteger(section, TEXT("PosCostLong"), 0);
		cpar.pos[i].PosCostShort = pini->ReadInteger(section, TEXT("PosCostShort"), 0);
		cpar.pos[i].PosLong = pini->ReadInteger(section, TEXT("PosLong"), 0);
		cpar.pos[i].PosShort = pini->ReadInteger(section, TEXT("PosShort"), 0);
		cpar.pos[i].YdPosLong = pini->ReadInteger(section, TEXT("PosYdLong"), 0);
		cpar.pos[i].YdPosShort = pini->ReadInteger(section, TEXT("PosYdShort"), 0);
		cpar.pos[i].MarginRate = pini->ReadFloat(section, TEXT("marginrate"), 0.09);
		cpar.pos[i].UnitTick = pini->ReadFloat(section, TEXT("unittick"), 5.);
		cpar.pos[i].Multiple = pini->ReadFloat(section, TEXT("multiple"), 10);
	}
	
	delete pini;
	pini = NULL;
}

void strategy::OnOrder(CThostFtdcOrderField& pOrder)
{
	common& comm = common::GetInstance();
	poskey pkey;
	pkey.strID = strname;
	strcpy_s(pkey.InstrumentID, pOrder.InstrumentID);
	/*strcpy_s(cpar.pos.InstrumentID, pOrder.InstrumentID);
	strcpy_s(cpar.pos.InvestorID, pOrder.InvestorID);
	position_t& rpos = comm.strpos[pkey];
	cpar.pos.CLongFrozen = rpos.CLongFrozen;
	cpar.pos.CShortFrozen = rpos.CShortFrozen;
	cpar.pos.OLongFrozen = rpos.OLongFrozen;
	cpar.pos.OShortFrozen = rpos.OShortFrozen;*/
}

void strategy::OnTrade(CThostFtdcTradeField& pTrade)
{
	// +算持仓成本.
	
	/*common& comm = common::GetInstance();
	poskey pkey;
	pkey.strID = strname;
	strcpy_s(pkey.InstrumentID, pTrade.InstrumentID);
	position_t& rpos = comm.strpos[pkey];
	cpar.pos.PosCostLong = rpos.PosCostLong;
	cpar.pos.PosCostShort = rpos.PosCostShort;
	// 这里需要做净值计算？..
	cpar.pos.PosLong = rpos.PosLong;
	cpar.pos.PosShort = rpos.PosShort;
	cpar.pos.YdPosLong = rpos.YdPosLong;
	cpar.pos.YdPosShort = rpos.YdPosShort;*/
}

void strategy::SetComValue(string addr,char *sect)
{
	inihelp *pini = new inihelp(ini_addr.c_str());
	TCHAR section[30];
	pini->CharToTchar(sect, section);

	/*****趋势部分初始化*******/
	//LPCTSTR section = TEXT("cpars");..
	TCHAR investor[15];
	pini->CharToTchar(InvestorID.c_str(), investor);
	pini->WriteString(section, TEXT("InvestorID"), investor);
	TCHAR strid[15];
	pini->CharToTchar(strname.c_str(), strid);
	pini->WriteString(section, TEXT("strID"), strid);
	TCHAR inst[31];
	pini->CharToTchar(InstrumentID.c_str(), inst);
	pini->WriteString(section, TEXT("InstrumentID"), inst);
	pini->WriteFloat(section, TEXT("moneyrate"), cpar.MoneyRate);
	pini->WriteInteger(section, TEXT("RequestID"), cpar.RequestID);
	

	for (int i = 0; i < cpar.pos_num; ++i)
	{
		TCHAR section[30];
		pini->CharToTchar(cpar.pos[i].InstrumentID, section);
		pini->WriteInteger(section, TEXT("CLongFrozen"), cpar.pos[i].CLongFrozen);
		pini->WriteInteger(section, TEXT("CShortFrozen"), cpar.pos[i].CShortFrozen);
		pini->WriteInteger(section, TEXT("OLongFrozen"), cpar.pos[i].OLongFrozen);
		pini->WriteInteger(section, TEXT("OShortFrozen"), cpar.pos[i].OShortFrozen);
		pini->WriteInteger(section, TEXT("PosCostLong"), cpar.pos[i].PosCostLong);
		pini->WriteInteger(section, TEXT("PosCostShort"), cpar.pos[i].PosCostShort);
		pini->WriteInteger(section, TEXT("PosLong"), cpar.pos[i].PosLong);
		pini->WriteInteger(section, TEXT("PosShort"), cpar.pos[i].PosShort);
		pini->WriteInteger(section, TEXT("PosYdLong"), cpar.pos[i].YdPosLong);
		pini->WriteInteger(section, TEXT("PosYdShort"), cpar.pos[i].YdPosShort);
		pini->WriteFloat(section, TEXT("marginrate"), cpar.pos[i].MarginRate);
		pini->WriteFloat(section, TEXT("unittick"), cpar.pos[i].UnitTick);
		pini->WriteInteger(section, TEXT("multiple"), cpar.pos[i].Multiple);
	}
	
	delete pini;
	pini = NULL;
}


bool strategy::CreateBar(CThostFtdcDepthMarketDataField& pData, CThostFtdcDepthMarketDataField& preData)
{
	if ((strcmp(pData.UpdateTime, "02:30:00") > 0 && strcmp(pData.UpdateTime, "09:00:00") < 0) ||
		(strcmp(pData.UpdateTime, "15:15:00") > 0 && strcmp(pData.UpdateTime, "21:00:00") < 0))
	{
		return false;
	}

	if ((strcmp(preData.UpdateTime, "02:30:00") > 0 && strcmp(preData.UpdateTime, "09:00:00") < 0) ||
		(strcmp(preData.UpdateTime, "15:15:00") > 0 && strcmp(preData.UpdateTime, "21:00:00") < 0))
	{
		return false;
	}

	char lastupdate[9];
	char preupdate[9];
	strncpy_s(lastupdate, pData.UpdateTime, 9);
	strncpy_s(preupdate, preData.UpdateTime, 9);
	int lasttime[3] = { 0 }, pretime[3] = {0};
	char *laststr,*prestr;
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

	if (abs(lasttime[0] - pretime[0]) > 1 && abs(lasttime[0] - pretime[0]) <23)
	{
		return false;
	}
	if (abs(lasttime[1] - pretime[1]) > 10 && abs(lasttime[1] - pretime[1]) < 50)
	{
		return false;
	}

	if ((lasttime[2] < pretime[2] && pretime[2] >= 50) || lasttime[0] != pretime[0] || lasttime[1] != pretime[1])
	{
		int hm = lasttime[0] * 100 + lasttime[1];
		//if (hm != 1015 && hm != 1130 && strlen(preData.InstrumentID)>0)..
		//极度不活跃的品种的情况，可能碰到刚好两个分钟整点两条行情，这个是个潜在bug..
		if (strlen(preData.InstrumentID)>0)
		{
			return true;
		}
	}
	return false;
}


strategy::~strategy() 
{
	SetComValue(ini_addr, "cpars");
}