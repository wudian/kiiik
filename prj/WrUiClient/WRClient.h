//负责与服务器交互。主要实现写、查、订阅等功能

#pragma once

#include <qstring>
#include <QObject> 
#include "NetworkAsio/TcpClient.h"
#include "Protocol.h"
#include "WRStruct.h"
#include "Timer/Timer.h"

using namespace kiiik;
using namespace network_asio;

class SubsTblModel;

class WRClient : public QObject, public SocketReaderSpi, public ReConnSpi, public TimerSpi
{
	Q_OBJECT

public:
	WRClient();
	virtual ~WRClient();

	// test
	void Testing();

	void Login(UserIdType id, PwdType pwd);
	void Logout();
	void TestTimerWrite(); // 用定时器随机写 for test
	void Quote(const CSymbol &);
	void Sub(std::vector<CSymbol> &inst_vec); // 订阅
	void UnSub(std::vector<CSymbol> &inst_vec);
	void CheckVersion();
	void DownloadVersion();
	//将update文件夹下的文件copy到正确的位置。
	void CopyFilesInUpdateFolder();

public slots:
	void GetSymbols(); // 查合约列表

private:
	virtual void OnReceive(TcpSession *tcp_sock, char* buf, int len);

	//网络第一次连接。 如果网络断开，则success为false
	virtual void SockConn(bool success);
	//网络断开
	virtual void SockDisconn();
	//断线重连
	virtual void SockReConn();

	virtual void OnTimer();

	list<string> m_symbols; // 已经订阅过的
	void Split(const std::string& s, const std::string delim,std::vector<std::string>* ret);
	void Trim(std::string &s);

	TcpClient *client_;
	bool is_login_;
	UserIdType id_;
	PwdType pwd_;

	string update_folder_;//放新版本客户端
	string bin_folder_;//放VersionUpdate.exe

	// 储存所有服务器、端口
	struct IP_PORT{
		string ip;
		string port;
	};
	vector<IP_PORT> ip_ports_;
	string config_file_;
	string version_cfg_file_;
	float version_; // 客户端版本号
	void readxml();

signals:
	void SignalNetErr(const QString &);//网络断了
	void SignalLogin(const QString &);
	void SignalSymbols(CSymbol *, int);
	void SignalQuote(const CQuotation *);
	void SignalTickRsp(const CQuotation *); // tick推送
};

