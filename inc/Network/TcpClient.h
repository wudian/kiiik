#pragma once

#include "network/TcpSession.h"
#include "baselib/Thread.h"

namespace network {

class ReConnSpi
{
public:
	virtual void SockReConn() = 0;
};

class NETWORK_API TcpClient : public baselib::Thread, private SocketDissConnSpi
{
public:
	TcpClient(const char *ip, int port, SocketReaderSpi* spi = NULL, ReConnSpi* re_conn_spi = NULL);
	~TcpClient(void);

	bool StartUp(std::string& err);
	void TearDown();

	bool Send(char* buf, int len, std::string& err);

private:
	virtual void OnRun();

	bool ReConnect();
	virtual void OnDisconnect(TcpSession *tcp_sock);

private:
	std::string ip_;
	int port_;
	baselib::SpinLock mutex_;
	bool m_is_start_on_read;
	bool is_connect_;

	SocketReaderSpi* read_spi_;
	ReConnSpi* re_conn_spi_;

	struct event_base *ev_base_;
	TcpSession* tcp_sock_;
};

}

