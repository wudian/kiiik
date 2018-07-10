#include "Baselib/Global.h"
#include "Baselib/QueueBuffer.h"
#include "Baselib/NonBlockQueueBuffer.h"
#include "Baselib/SpendTime.h"

namespace baselib {

string g_KiiikHome;
string g_AppName;

void InitKiiik(char *argv[]) {
	string _KiiikHome = argv[0];
	size_t _bin = _KiiikHome.find("bin");
	g_KiiikHome = _KiiikHome.substr(0, _bin);

	string _AppName = argv[0];
	size_t _bgn = _AppName.find_last_of("\\") + 1;
	size_t _end = _AppName.find_last_of(".");
	g_AppName = _AppName.substr(_bgn, _end-_bgn);
}

string KiiikHome() { return g_KiiikHome == "" ? "E:\\Kiiik\\" : g_KiiikHome; }
string AppName() { return g_AppName == "" ? "App" : g_AppName; }
void SetAppName(string name) { g_AppName = name; }

bool IsDirExist(const string& dir_path)
{
	return ACCESS(dir_path.c_str(),0) != -1;
}

bool MakeDir(const string& dirpath)
{	
	string pathtemp;
	pathtemp=dirpath;
	string tempdir;
	while(pathtemp.find('\\')!=string::npos) {
		pathtemp.replace(pathtemp.find('\\'),1,"/");
	}

	if(pathtemp[pathtemp.length()-1]!='/') {
		pathtemp+="/";
	}

	while(pathtemp.find('/')!=string::npos)
	{
		tempdir.insert(tempdir.length(),pathtemp,0,pathtemp.find('/')+1);
		pathtemp=pathtemp.substr(pathtemp.find('/')+1);
		int result=0;
		if(!IsDirExist(tempdir) && (0 != MKDIR(tempdir.c_str()))) { return false; }
	}

	return true;
}

void Trim(string &s)
{
	if (s.empty())
	{
		return;
	}

	s.erase(0, s.find_first_not_of(" "));
	s.erase(s.find_last_not_of(" ") + 1);
}


void Split(const std::string& s, const std::string delim, std::vector<std::string>* ret)
{
	size_t last = 0;
	size_t index = s.find_first_of(delim, last);
	while (index != std::string::npos)
	{
		std::string part_str = s.substr(last, index - last);
		Trim(part_str);
		if (!part_str.empty())
		{
			ret->push_back(part_str);
		}
		last = index + 1;
		index = s.find_first_of(delim, last);
	}
	if (index - last > 0)
	{
		std::string part_str = s.substr(last, index - last);
		Trim(part_str);
		if (!part_str.empty())
		{
			ret->push_back(part_str);
		}
	}
}
}