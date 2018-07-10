// CTP_Demo.cpp : 定义控制台应用程序的入口点。..
//..
#pragma once;
#include "stdafx.h"
#include <fstream>
#include "backtest.h"


using namespace std;
string strname = "str_back";

strategy *pstr = NULL;

#if  BACKTEST
	backtest *pback = NULL;
#endif

map<string,string>inifile;

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
			size_t pos = line.find('=');//找到每行的“=”号位置，之前是key之后是value   ..
			if (pos == string::npos) continue;
			string key = line.substr(0, pos);//取=号之前..
			string value = line.substr(pos + 1);
			auto check = inifile.insert(make_pair(key,value));
			if (!check.second)
			{
				cerr <<"insert inifile failure!" << endl;
			}
		}
		infile.close();
	}
}


int main(int argc, _TCHAR* argv[])
{
#if !BACKTEST
	SYSTEMTIME ct;
	GetLocalTime(&ct);
	int weekofday = ct.wDayOfWeek;
	if (weekofday < 1 || weekofday>5)
	{
		return 0;
	}
#endif
	common& comm = common::GetInstance();
	bool load = comm.Start("./config.cfg");
	
#if !BACKTEST
	readfile("./inifilepath.txt");
	strategy *pstr = NULL;
	for (map<string, string>::iterator iter = inifile.begin(); iter != inifile.end(); iter++)
	{
		string strclass = iter->first.substr(0,iter->first.length()-3);
		/*if (strclass == "intra_mixc")
		{
			pstr = new intra_mixc(iter->second);
			string inst = pstr->GetInst();
			comm.AddStrategy(inst, pstr);
		}
		else if (strclass == "intra_mixs")
		{
			pstr = new intra_mixs(iter->second);
			string inst = pstr->GetInst();
			comm.AddStrategy(inst, pstr);
		}
		else if (strclass == "intra_swing")
		{
			pstr = new intra_swing(iter->second);
			string inst = pstr->GetInst();
			comm.AddStrategy(inst, pstr);
		}
		else if (strclass == "polystr")
		{
			pstr = new polystr(iter->second);
			string inst = pstr->GetInst();
			comm.AddStrategy(inst, pstr);
		}
		else if (strclass == "newllt")
		{
			pstr = new newllt(iter->second);
			string inst = pstr->GetInst();
			comm.AddStrategy(inst, pstr);
		}
		else */if (strclass == "mhilbert")
		{
			pstr = new mhilbert(iter->second);
			vector<string> insts = pstr->GetInst();
			for (int i = 0; i < insts.size();++i)
				comm.AddStrategy(insts[i], pstr);
		}
	}
	pstr = NULL;
	comm.BeforeMktOpen();
	//comm.pStrID.begin()->second->SetComValue("./mhilbert_ru.ini", "cpars");.
	comm.AfterMktClose();
#else
	pstr = new mhilbert("./inifiles/mhilbert_ru.ini");
	comm.AddStrategy("ru", pstr);
	comm.BeforeMktOpen();
	comm.q = new Quote();
	pback = new backtest("D:\\RA-sharedoc\\wudian\\lidian\\prj\\NewTrader\\data\\");
	/*cout << "please input filename!" << endl;
	char s[30];
	cin.get(s,30);*/
	pback->run("20170822_ru1709.txt");
	delete pstr;
	pstr = NULL;
	delete pback;
	pback = NULL;
	delete comm.q;
	comm.q = NULL;
	comm.AfterMktClose();
#endif
	return 0;
}

