//负责与服务器交互。主要实现写、查、订阅等功能

#ifndef WR_CLIENT_H
#define WR_CLIENT_H

#include <memory>
#include "NetworkAsio/TcpClient.h"
#include "Protocol.h"
#include "wtpmduserapi/WtpMdApi.h"
#include "Baselib/QueueBuffer.h"
#include "RspMsg.h"

using namespace baselib;
using namespace network_asio;



class WRClient : public CWtpMdApi, public SocketReaderSpi, public ReConnSpi, public QueueBuffer<std::shared_ptr<RspMsgItem>, 50>
{

public:
	WRClient();
	virtual ~WRClient();

	///删除接口对象本身
	///@remark 不再使用本接口对象时,调用该函数删除接口对象
	virtual void Release();	

	///注册前置机网络地址
	///@param pszFrontAddress：前置机网络地址。
	///@remark 网络地址的格式为：“protocol://ipaddress:port”，如：”tcp://127.0.0.1:17001”。 
	///@remark “tcp”代表传输协议，“127.0.0.1”代表服务器地址。”17001”代表服务器端口号。
	virtual void RegisterFront(char *pszFrontAddress);

	///注册回调接口
	///@param pSpi 派生自回调接口类的实例
	virtual void RegisterSpi(CWtpMdSpi *pSpi);

	///用户登录请求
	virtual int ReqUserLogin(CWtpReqUserLoginField *pReqUserLoginField);

	///登出请求
	virtual int ReqUserLogout(CWtpUserLogoutField *pUserLogout);

	///查询合约列表
	virtual int ReqQrySymbol();

	///查询单个合约价格
	virtual int ReqQryQuotation(CWtpSymbolField *pInstrumentID);

	///订阅行情。
	///@param vecInstrumentID 合约ID  
	///@remark 
	virtual int ReqSubQuotation(CWtpSymbolField *pInstrumentID, int nCount);

	///退订行情。
	///@param vecInstrumentID 合约ID  
	///@remark 
	virtual int ReqUnSubQuotation(CWtpSymbolField *pInstrumentID, int nCount);
	

private:
	virtual void OnReceive(TcpSession *tcp_sock, char* buf, int len);

	//网络第一次连接。 如果网络断开，则success为false
	virtual void SockConn(bool success);
	//网络断开
	virtual void SockDisconn();
	//断线重连
	virtual void SockReConn();

	virtual bool Consume(const std::shared_ptr<RspMsgItem>& val);

	list<string> m_symbols; // 已经订阅过的
	void Split(const std::string& s, const std::string delim,std::vector<std::string>* ret);
	void Trim(std::string &s);

	TcpClient *client_;
	bool is_login_;
	UserIdType id_;
	PwdType pwd_;

	

	// 储存所有服务器、端口
	struct IP_PORT{
		string ip;
		string port;
	};
	vector<IP_PORT> ip_ports_;
	
	
	CWtpMdSpi *spi_;
	int nRequestID_;

};

#endif // !WR_CLIENT_H