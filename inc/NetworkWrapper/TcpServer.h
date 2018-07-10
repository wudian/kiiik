#pragma once

#include "NetworkWrapper/TcpSocket.h"

namespace network_wrapper {

class TcpServerConnSpi {
public:
	virtual void OnAccept(TcpSocket* sock) = 0;
	virtual void OnDiscon(TcpSocket *tcp_sock) = 0;
};

class TcpServerWrapper;

class NETWORK_WRAPPER_API TcpServer
{
public:
	TcpServer(int port, SocketReaderSpi* spi, TcpServerConnSpi* conn_spi);
	~TcpServer(void);

	bool StartUp(std::string& err);

	

private:
	TcpServerWrapper *server_wrapper_;

};

}