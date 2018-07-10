#pragma once

#ifdef WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#endif

#include <stdlib.h>
#include <ctype.h>
#include <string>
#ifdef WIN32
#include <WinSock2.h>
typedef SOCKET socket_t; 
#define NAN_SOCKET INVALID_SOCKET
#else
#include <unistd.h>
#include <netinet/in.h>  
#include <sys/socket.h>  
#include <sys/types.h>  
typedef int socket_t;  
#define SOCKET_ERROR  (-1)  
#define INVALID_SOCKET  0 
#include <arpa/inet.h>
#define NAN_SOCKET eINVALID
#endif

#include "Network/TcpMessage.h"
#include "baselib/SpinLock.h"


struct event_base;
struct bufferevent;

namespace network {

class TcpSession;

class SocketReaderSpi {
public:
	virtual void OnReceive(TcpSession *tcp_sock, char* buf, int len) = 0;
};

class SocketDissConnSpi {
public:
	virtual void OnDisconnect(TcpSession *tcp_sock) = 0;
};


class NETWORK_API TcpSession
{	
	friend class TcpMessage;

public:
	TcpSession(struct event_base **p_base, SocketReaderSpi* spi, SocketDissConnSpi* dis_conn_spi);
	~TcpSession(void);

	bool Connect(const char *ip, int port, std::string& err);
	bool Send(char* buf, int len, std::string& err);

	bool bind_libevent(socket_t socket, std::string& err);
	bool SetBufSize(bool is_recv, int size, std::string& err);
	bool is_connect() { return connect_; }

private:
	static void handle_read(struct bufferevent *, void *); // 处理读数据的event
	static void handle_disconnect(struct bufferevent *, short, void *);

private:
	TcpMessage *message_;
	socket_t socket_;

	struct event_base **pp_ev_base_;
	struct bufferevent *buf_ev_;

	SocketReaderSpi* read_spi_;
	SocketDissConnSpi* m_dis_conn_spi;
	
	bool connect_; // 长连接是否断开?
};

bool init_net(std::string& err); // 初始化网络环境
void denit_net(); // 关闭网络环境
socket_t create_socket(std::string& err);
void close_socket(socket_t &sock);
bool init_sockaddr(sockaddr_in *addr, const char *ip, int port, std::string& err);

}

