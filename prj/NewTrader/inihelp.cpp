#pragma once;
#include "Inihelp.h"  
#include <iostream>
#include "tchar.h"


inihelp::inihelp(const char *szfile)
{
	memset(m_file, 0x00, sizeof(m_file));
	CharToTchar(szfile, m_file);
}

void inihelp::TcharToChar(const TCHAR * tchar, char * _char)
{
	int iLength;
	//获取字节长度    ..
    iLength = WideCharToMultiByte(CP_ACP, 0, tchar, -1, NULL, 0, NULL, NULL);
	//将tchar值赋给_char      ..
	WideCharToMultiByte(CP_ACP, 0, tchar, -1, _char, iLength, NULL, NULL);
}


void inihelp::CharToTchar(const char * _char, TCHAR * tchar)
{
	int iLength;
	iLength = MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, NULL, 0);
	MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, tchar, iLength*sizeof(wchar_t));
}


int inihelp::ReadInteger(LPCTSTR szSection, LPCTSTR szKey, int iDefaultValue)
{
	int iResult = GetPrivateProfileInt(szSection, szKey, iDefaultValue, m_file);
	return iResult;
}

double inihelp::ReadFloat(LPCTSTR szSection, LPCTSTR szKey, double fltDefaultValue)
{
	TCHAR szResult[255];
	TCHAR szDefault[255];
	double fltResult;
	_stprintf_s(szDefault, 255, TEXT("%f"), fltDefaultValue);
	GetPrivateProfileString(szSection, szKey, szDefault, szResult, 255, m_file);
	DWORD errcode = GetLastError();
	fltResult = _wtof(szResult);
	return fltResult;
}

bool inihelp::ReadBoolean(LPCTSTR szSection, LPCTSTR szKey, bool bolDefaultValue)
{
	TCHAR szResult[255];
	TCHAR szDefault[255];
	bool bolResult;
	_stprintf_s(szDefault, 255, TEXT("%s"), bolDefaultValue ? TEXT("True") : TEXT("False"));
	GetPrivateProfileString(szSection, szKey, szDefault, szResult, 255, m_file);
	bolResult = (_tcscmp(szResult, TEXT("True")) == 0 ||_tcscmp(szResult, TEXT("true")) == 0) ? true : false;
	return bolResult;
}

LPTSTR inihelp::ReadString(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szDefaultValue)
{
	LPTSTR szResult = new TCHAR[255];
	memset(szResult, 0x00, sizeof(szResult));
	GetPrivateProfileString(szSection, szKey, szDefaultValue, szResult, 255, m_file);
	return szResult;
}

void inihelp::WriteInteger(LPCTSTR szSection, LPCTSTR szKey, int iValue)
{
	TCHAR szValue[255];
	_stprintf_s(szValue, 255, TEXT("%d"), iValue);
	WritePrivateProfileString(szSection, szKey, szValue, m_file);
}

void inihelp::WriteFloat(LPCTSTR szSection, LPCTSTR szKey, double fltValue)
{
	TCHAR szValue[255];
	_stprintf_s(szValue, 255, TEXT("%f"), fltValue);
	WritePrivateProfileString(szSection, szKey, szValue, m_file);
}

void inihelp::WriteBoolean(LPCTSTR szSection, LPCTSTR szKey, bool bolValue)
{
	TCHAR szValue[255];
	_stprintf_s(szValue, 255, TEXT("%s"), bolValue ? TEXT("True") : TEXT("False"));
	WritePrivateProfileString(szSection, szKey, szValue, m_file);
}

void inihelp::WriteString(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szValue)
{
	WritePrivateProfileString(szSection, szKey, szValue, m_file);
}

int inihelp::CalCount(LPCTSTR szfile)
{
	TCHAR       chSectionNames[2048] = { 0 };       //所有节名组成的字符数组..
	TCHAR * pSectionName; //保存找到的某个节名字符串的首地址..
	int i;       //i指向数组chSectionNames的某个位置，从0开始，顺序后移..
	int j = 0;      //j用来保存下一个节名字符串的首地址相对于当前i的位置偏移量..
	int count = 0;      //统计节的个数..

	GetPrivateProfileSectionNames(chSectionNames, 2048, szfile);
	for (i = 0; i<2048; i++, j++)
	{
		if (chSectionNames[0] == '\0')
			break;       //如果第一个字符就是0，则说明ini中一个节也没有..
		if (chSectionNames[i] == '\0')
		{
			pSectionName = &chSectionNames[i - j]; //找到一个0，则说明从这个字符往前，减掉j个偏移量，就是一个节名的首地址..
			count++;
			j = -1;         //找到一个节名后，j的值要还原，以统计下一个节名地址的偏移量..
			//赋成-1是因为节名字符串的最后一个字符0是终止符，不能作为节名的一部分..
			if (chSectionNames[i + 1] == 0)
			{
				break;      //当两个相邻的字符都是0时，则所有的节名都已找到，循环终止..
			}
		}
	}
	return count;
}