#pragma once

#include <assert.h> 
#include <direct.h>
#include <string>
#include <vector>

#ifdef WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <direct.h>
#include <io.h>
#include <windows.h>
#include <tchar.h>
#else
#include <stdarg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#endif

#ifdef WIN32
#define ACCESS _access
#define MKDIR(a) _mkdir((a))
#else
#define ACCESS access
#define MKDIR(a) mkdir((a),0755)
#endif

using namespace std;

#ifdef BASELIB_LIB
#define BASELIB_API  __declspec(dllexport)
#else 
#define BASELIB_API  __declspec(dllimport)
#endif

#ifdef NETWORK_EXPORTS
#define NETWORK_API  __declspec(dllexport)
#else 
#define NETWORK_API  __declspec(dllimport)
#endif

namespace baselib {


void BASELIB_API InitKiiik(char *argv[]);//初始化KiiikHome和AppName,在main函数开始时进行
string BASELIB_API KiiikHome();
string BASELIB_API AppName();
void BASELIB_API SetAppName(string);

bool BASELIB_API IsDirExist(const string& dir_path);
bool BASELIB_API MakeDir(const string& dirpath);

void BASELIB_API Trim(string &s);//去除左右空格
void BASELIB_API Split(const std::string& s, const std::string delim, std::vector<std::string>* ret);
}