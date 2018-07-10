#include "wtpmduserapi/WtpMdCApi.h"
#include "wtpmduserapi/WtpMdApi.h"
#include "GlobalVars.h"

/*!	\brief 注册行情回调 ***********************************************************************************/
MD_API_EXPORT void __stdcall WTP_RegOnFrontConnected(fnOnFrontConnected f){
	g_fnOnFrontConnected = f;
}
MD_API_EXPORT void __stdcall WTP_RegOnFrontDisconnected(fnOnFrontDisconnected f){
	g_fnOnFrontDisconnected = f;
}
MD_API_EXPORT void __stdcall WTP_RegOnRspUserLogin(fnOnRspUserLogin f){
	g_fnOnRspUserLogin = f;
}
MD_API_EXPORT void __stdcall WTP_RegOnRspUserLogout(fnOnRspUserLogout f){
	g_fnOnRspUserLogout = f;
}
MD_API_EXPORT void __stdcall WTP_RegOnRspError(fnOnRspError f){
	g_fnOnRspError = f;
}
MD_API_EXPORT void __stdcall WTP_RegOnRspQrySymbol(fnOnRspQrySymbol f){
	g_fnOnRspQrySymbol = f;
}
MD_API_EXPORT void __stdcall WTP_RegOnRspQryQuotation(fnOnRspQryQuotation f){
	g_fnOnRspQryQuotation = f;
}
MD_API_EXPORT void __stdcall WTP_RegOnRtnQuotation(fnOnRtnQuotation f){
	g_fnOnRtnQuotation = f;
}


/*!	\brief 行情接口 ***********************************************************************************/

MD_API_EXPORT void __stdcall RegisterFront(char *pszFrontAddress){
	CWtpMdApi::CreateWtpMdApi()->RegisterFront(pszFrontAddress);
}
///用户登录请求
MD_API_EXPORT int __stdcall ReqUserLogin(CWtpReqUserLoginField *pReqUserLoginField){
	return CWtpMdApi::CreateWtpMdApi()->ReqUserLogin(pReqUserLoginField);
}
///登出请求
MD_API_EXPORT int __stdcall ReqUserLogout(CWtpUserLogoutField *pUserLogout){
	return CWtpMdApi::CreateWtpMdApi()->ReqUserLogout(pUserLogout);
}
///查询合约列表
MD_API_EXPORT int __stdcall ReqQrySymbol(){
	return CWtpMdApi::CreateWtpMdApi()->ReqQrySymbol();
}
///查询单个合约价格
MD_API_EXPORT int __stdcall ReqQryQuotation(CWtpSymbolField *pInstrumentID){
	return CWtpMdApi::CreateWtpMdApi()->ReqQryQuotation(pInstrumentID);
}
///订阅行情。
///@param pInstrumentID 合约ID  
///@param nCount 要订阅/退订行情的合约个数
///@remark 
MD_API_EXPORT int __stdcall ReqSubQuotation(CWtpSymbolField *pInstrumentID, int nCount){
	return CWtpMdApi::CreateWtpMdApi()->ReqSubQuotation(pInstrumentID, nCount);
}
///退订行情。
///@param pInstrumentID 合约ID  
///@param nCount 要订阅/退订行情的合约个数
///@remark 
MD_API_EXPORT int __stdcall ReqUnSubQuotation(CWtpSymbolField *pInstrumentID, int nCount){
	return CWtpMdApi::CreateWtpMdApi()->ReqUnSubQuotation(pInstrumentID, nCount);
}

MD_API_EXPORT void __stdcall Release(){
	CWtpMdApi::CreateWtpMdApi()->Release();
}