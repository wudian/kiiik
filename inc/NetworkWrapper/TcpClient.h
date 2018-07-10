#pragma once

#include "NetworkWrapper/TcpSocket.h"

 
namespace network_wrapper {

class ReConnSpi
{
public:
	virtual void SockReConn() = 0;
};

class TcpClientWrapper;

class NETWORK_WRAPPER_API TcpClient 
{
public:
	TcpClient(const char *ip, int port, SocketReaderSpi* spi = NULL, ReConnSpi* re_conn_spi = NULL);
	~TcpClient(void);

	bool StartUp(std::string& err);
	void TearDown();

	bool Send(char* buf, int len, std::string& err);

private:
	TcpClientWrapper *client_wrapper_;
};

}

