
#include "wtpmduserapi/WtpMdCApi.h"
#include "GlobalVars.h"
#include <windows.h>


fnOnFrontConnected g_fnOnFrontConnected = NULL;
fnOnFrontDisconnected g_fnOnFrontDisconnected = NULL;
fnOnRspUserLogin g_fnOnRspUserLogin = NULL;
fnOnRspUserLogout g_fnOnRspUserLogout = NULL;
fnOnRspError g_fnOnRspError = NULL;
fnOnRspQrySymbol g_fnOnRspQrySymbol = NULL;
fnOnRspQryQuotation g_fnOnRspQryQuotation = NULL;
fnOnRtnQuotation g_fnOnRtnQuotation = NULL;