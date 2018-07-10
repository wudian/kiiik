#pragma once

#include "network/TcpSession.h"
#include <list>
#include "baselib/Thread.h"

extern "C"{
#include <event2/bufferevent.h>
#include <event2/listener.h>
};

namespace network {

class TcpServerConnSpi {
public:
	virtual void OnAccept(TcpSession* sock) = 0;
	virtual void OnDiscon(TcpSession *tcp_sock) = 0;
};

class NETWORK_API TcpServer : public baselib::Thread, private SocketDissConnSpi
{
public:
	TcpServer(int port, SocketReaderSpi* spi, TcpServerConnSpi* conn_spi);
	~TcpServer(void);

	bool StartUp(std::string& err);


private:
	static void handle_accept(struct evconnlistener *, evutil_socket_t, struct sockaddr *, int, void *);

	virtual void OnDisconnect(TcpSession *tcp_sock);

	virtual void OnRun();

	void PushSocket(TcpSession* sock);
	void DleSocket();

	void OnAccept(TcpSession* sock);

private:
	struct sockaddr_in sock_addr_;
	struct event_base *ev_base_;
	struct evconnlistener *listener_;

	SocketReaderSpi* read_spi_;
	TcpServerConnSpi* conn_spi_;

	bool is_init_;

	baselib::SpinLock socks_mtx_;
	std::list<TcpSession*> invalid_socks_;
};

}
