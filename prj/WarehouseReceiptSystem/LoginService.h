#pragma once

#include <vector>
#include <map>
#include "MarketDefine.h"
//#include "Baselib/IniConfig.h"
#include "Protocol.h"
#include "NetworkAsio/TcpSession.h"

#include "Baselib/Global.h"

namespace kiiik {

using namespace std;
using namespace baselib;

enum E_Authority // 用户的权限
{
	E_Read = 0,//谁都可以查、订阅
	E_Write, //
	E_Admin, //备用，可以修改配置文件
};

class LoginService
{
public:
	LoginService();
	~LoginService(void);

	void OnDiscon(network_asio::TcpSession *tcp_sock);//清理map_session_authority_

	void HandleReq( LoginRequest *,  network_asio::TcpSession *);
	void HandleReq(VersionRequest *, network_asio::TcpSession *);

	bool CanWrite(network_asio::TcpSession *);//要有写的权限才行

private:

	bool loginSuccess(UserIdType, PwdType, string&, network_asio::TcpSession *);

	void readVersionXml();//服务器启动时读一次即可
	void readLoginXml();//每次有新的客户端连进来时读
	void writexml();

	map<network_asio::TcpSession *, E_Authority> map_session_authority_;

	struct xmlAccDetail 
	{
		string user;
		string password;
		E_Authority authority;//"admin" "read" "write"
	};
	vector<xmlAccDetail> acc_details_;
	string acc_cofig_file_;
	string version_cfg_file_;
	string update_folder_; // 新版客户端目录
	float version_; // 客户端最新版本号
};

}
