#pragma once

#include "NetworkAsio/TcpServer.h"

namespace kiiik {

using namespace network_asio;

// 报价服务器
class QuoteServer : public TcpServerConnSpi, public SocketReaderSpi
{
public:
	QuoteServer(int port);
	~QuoteServer(void);

private:
	virtual void OnAccept(TcpSession* sock);
	virtual void OnDiscon(TcpSession *tcp_sock);
	virtual void OnReceive(TcpSession *tcp_sock, char* buf, int len);

	TcpServer srv_;
};

}