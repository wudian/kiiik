#pragma once

#include "Protocol.h"
#include "NetworkAsio/TcpServer.h"
#include "LoginService.h"
#include "RWDataService.h"

namespace kiiik {

using namespace baselib;
using namespace network_asio;

class WRServer : public SocketReaderSpi, public TcpServerConnSpi
{
public:
	WRServer(int port);
	~WRServer(void);

private:
	virtual void OnReceive(TcpSession *tcp_sock, char* buf, int len);
	virtual void OnAccept(TcpSession* sock);
	virtual void OnDiscon(TcpSession *tcp_sock);

	TcpServer *srv_;

	LoginService *login_service_;
	RWDataService *rw_data_service_;
};

}