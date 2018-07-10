/////////////////////////////////////////////////////////////////////////
///@system 仓单综合交易所系统
///@company 东航金控有限责任公司
///@file WtpMdApi.h
///@brief 定义了客户端接口使用的业务数据结构
///@history 
///20161122	陈挺、吴典		创建该文件
/////////////////////////////////////////////////////////////////////////

#if !defined(WTP_MDAPI_H)
#define WTP_MDAPI_H

#include "wtpmduserapi/WtpUserApiStruct.h"



class CWtpMdSpi
{
public:
	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	virtual void OnFrontConnected(){};
	
	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	virtual void OnFrontDisconnected(){};	

	///登录请求响应
	virtual void OnRspUserLogin(CWtpRspUserLoginField *pRspUserLogin, CWtpRspInfoField *pRspInfo) {};

	///登出请求响应
	virtual void OnRspUserLogout(CWtpUserLogoutField *pUserLogout, CWtpRspInfoField *pRspInfo) {};

	///错误应答
	virtual void OnRspError(CWtpRspInfoField *pRspInfo) {};

	///查询合约列表响应
	virtual void OnRspQrySymbol(CWtpSymbolField *pSymbol, int nCount) {};

	///查询单个合约价格响应
	virtual void OnRspQryQuotation(CWtpQuotationField *pQuotation) {};

	///深度行情通知
	virtual void OnRtnQuotation(CWtpQuotationField *pQuotation) {};
};

class MD_API_EXPORT CWtpMdApi
{
public:
	///创建MdApi
	///@return 创建出的UserApi
	static CWtpMdApi *CreateWtpMdApi();
	
	///删除接口对象本身
	///@remark 不再使用本接口对象时,调用该函数删除接口对象
	virtual void Release() = 0;
	
	///注册前置机网络地址
	///@param pszFrontAddress：前置机网络地址。
	///@remark 网络地址的格式为：“protocol://ipaddress:port”，如：”tcp://127.0.0.1:17001”。 
	///@remark “tcp”代表传输协议，“127.0.0.1”代表服务器地址。”17001”代表服务器端口号。
	virtual void RegisterFront(char *pszFrontAddress) = 0;
	
	///注册回调接口
	///@param pSpi 派生自回调接口类的实例
	virtual void RegisterSpi(CWtpMdSpi *pSpi) = 0;

	///用户登录请求
	virtual int ReqUserLogin(CWtpReqUserLoginField *pReqUserLoginField) = 0;

	///登出请求
	virtual int ReqUserLogout(CWtpUserLogoutField *pUserLogout) = 0;
	
	///查询合约列表
	virtual int ReqQrySymbol() = 0;

	///查询单个合约价格
	virtual int ReqQryQuotation(CWtpSymbolField *pSymbol) = 0;

	///订阅行情。
	///@param pSymbol 仓单交易合约  
	///@param nCount 要订阅/退订行情的合约个数
	///@remark 
	virtual int ReqSubQuotation(CWtpSymbolField *pSymbol, int nCount) = 0;

	///退订行情。
	///@param pSymbol 仓单交易合约   
	///@param nCount 要订阅/退订行情的合约个数
	///@remark 
	virtual int ReqUnSubQuotation(CWtpSymbolField *pSymbol, int nCount) = 0;


protected:
	~CWtpMdApi(){};
};

#endif
