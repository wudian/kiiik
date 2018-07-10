/////////////////////////////////////////////////////////////////////////
///@system 仓单综合交易所系统
///@company 东航金控有限责任公司
///@file WtpMdCApi.h
///@brief 定义了客户端接口使用的业务数据结构
///@history 
///20161122	陈挺、吴典		创建该文件
/////////////////////////////////////////////////////////////////////////

#ifndef WTP_MD_C_API_H
#define WTP_MD_C_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include "wtpmduserapi/WtpUserApiStruct.h"

	//! 连接、断开的响应
	typedef void(__stdcall *fnOnFrontConnected)();
	typedef void(__stdcall *fnOnFrontDisconnected)();

	typedef void(__stdcall *fnOnRspUserLogin)(CWtpRspUserLoginField *pRspUserLogin, CWtpRspInfoField *pRspInfo);//登陆响应
	typedef void(__stdcall *fnOnRspUserLogout)(CWtpUserLogoutField *pUserLogout, CWtpRspInfoField *pRspInfo);//登出响应

	//! 错误信息
	typedef void(__stdcall *fnOnRspError)(CWtpRspInfoField *pRspInfo);

	//! 数据到来触发的回调类型
	typedef void(__stdcall *fnOnRspQrySymbol)(CWtpSymbolField *pSymbol, int nCount);//查合约列表响应
	typedef void(__stdcall *fnOnRspQryQuotation)(CWtpQuotationField *pQuotation);//查单个合约响应
	typedef void(__stdcall *fnOnRtnQuotation)(CWtpQuotationField *pQuotation);//订阅行情响应



	/*!	\brief 注册行情回调 ***********************************************************************************/
	MD_API_EXPORT void __stdcall WTP_RegOnFrontConnected(fnOnFrontConnected);
	MD_API_EXPORT void __stdcall WTP_RegOnFrontDisconnected(fnOnFrontDisconnected);
	MD_API_EXPORT void __stdcall WTP_RegOnRspUserLogin(fnOnRspUserLogin);
	MD_API_EXPORT void __stdcall WTP_RegOnRspUserLogout(fnOnRspUserLogout);
	MD_API_EXPORT void __stdcall WTP_RegOnRspError(fnOnRspError);
	MD_API_EXPORT void __stdcall WTP_RegOnRspQrySymbol(fnOnRspQrySymbol);
	MD_API_EXPORT void __stdcall WTP_RegOnRspQryQuotation(fnOnRspQryQuotation);
	MD_API_EXPORT void __stdcall WTP_RegOnRtnQuotation(fnOnRtnQuotation);


	/*!	\brief 行情接口 ***********************************************************************************/

	///注册前置机网络地址
	///@param pszFrontAddress：前置机网络地址。
	///@remark 网络地址的格式为：“protocol://ipaddress:port”，如：”tcp://127.0.0.1:17001”。 
	///@remark “tcp”代表传输协议，“127.0.0.1”代表服务器地址。”17001”代表服务器端口号。
	MD_API_EXPORT void __stdcall RegisterFront(char *pszFrontAddress);
	///用户登录请求
	MD_API_EXPORT int __stdcall ReqUserLogin(CWtpReqUserLoginField *pReqUserLoginField);
	///登出请求
	MD_API_EXPORT int __stdcall ReqUserLogout(CWtpUserLogoutField *pUserLogout);
	///查询合约列表
	MD_API_EXPORT int __stdcall ReqQrySymbol();
	///查询单个合约价格
	MD_API_EXPORT int __stdcall ReqQryQuotation(CWtpSymbolField *pSymbol);
	///订阅行情。
	///@param pSymbol 仓单交易合约  
	///@param nCount 要订阅/退订行情的合约个数
	///@remark 
	MD_API_EXPORT int __stdcall ReqSubQuotation(CWtpSymbolField *pSymbol, int nCount);
	///退订行情。
	///@param pSymbol 仓单交易合约   
	///@param nCount 要订阅/退订行情的合约个数
	///@remark 
	MD_API_EXPORT int __stdcall ReqUnSubQuotation(CWtpSymbolField *pSymbol, int nCount);

	MD_API_EXPORT void __stdcall Release();

#ifdef __cplusplus
}
#endif

#endif
