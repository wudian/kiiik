
#pragma once;
#ifndef INIHELP_H  
#define INIHELP_H

#include <windows.h>

class inihelp
{
public:
	inihelp(const char *szfile);
	int ReadInteger(LPCTSTR szSection, LPCTSTR szKey, int iDefaultValue);
	double ReadFloat(LPCTSTR szSection, LPCTSTR szKey, double fltDefaultValue);
	bool ReadBoolean(LPCTSTR szSection, LPCTSTR szKey, bool bolDefaultValue);
	LPTSTR ReadString(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szDefaultValue);

	void WriteInteger(LPCTSTR szSection, LPCTSTR szKey, int iValue);
	void WriteFloat(LPCTSTR szSection, LPCTSTR szKey, double fltValue);
	void WriteBoolean(LPCTSTR szSection, LPCTSTR szKey, bool bolValue);
	void WriteString(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szValue);
	int CalCount(LPCTSTR szfile);
	void CharToTchar(const char * _char, TCHAR * tchar);
	void TcharToChar(const TCHAR * tchar, char * _char);
private:
	TCHAR m_file[255];
};
#endif //INIHELP_H  ..

