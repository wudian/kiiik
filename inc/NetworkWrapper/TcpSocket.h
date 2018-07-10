#pragma once

#ifdef NETWORKWRAPPER_EXPORTS
#define NETWORK_WRAPPER_API  __declspec(dllexport)
#else 
#define NETWORK_WRAPPER_API   __declspec(dllimport)
#endif

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
typedef SOCKET HSocket; 
#define NAN_SOCKET INVALID_SOCKET
#else
#include <unistd.h>
#include <netinet/in.h>  
#include <sys/socket.h>  
#include <sys/types.h>  
typedef int HSocket;  
#define SOCKET_ERROR  (-1)  
#define INVALID_SOCKET  0 
#include <arpa/inet.h>
#define NAN_SOCKET eINVALID
#endif

struct event_base;
struct bufferevent;

namespace network_wrapper {

class TcpSocket;
class TcpSocketWrapper;

class SocketReaderSpi {
public:
	virtual void OnReceive(TcpSocket *tcp_sock, char* buf, int len) = 0;
};

class SocketDissConnSpi {
public:
	virtual void OnDisconnect(TcpSocket *tcp_sock) = 0;
};


class NETWORK_WRAPPER_API TcpSocket
{
public:
	TcpSocket(struct event_base **p_base, SocketReaderSpi* spi, SocketDissConnSpi* dis_conn_spi);
	~TcpSocket(void);

	static bool WinStartUp(std::string& err);
	static void WinClean();

	bool Connect(const char *ip, int port, std::string& err);
	bool SetSocket(HSocket socket, std::string& err);

	bool SetBufSize(bool is_recv, int size, std::string& err);

	bool Send(char* buf, int len, std::string& err);

	

private:
	TcpSocketWrapper *socket_wrapper_;

};

}

