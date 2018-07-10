#include "network/TcpSession.h"
#include <sstream>
#include <baselib/Thread.h>
#include <string.h>

extern "C"{
#include <event2/bufferevent.h>
#include <event2/listener.h>
};

namespace network {


static bool bWinStartUp = false;


////////////////////////////////////////////////////////////////////////////////////



TcpSession::TcpSession(struct event_base **p_base, SocketReaderSpi* spi, SocketDissConnSpi* dis_conn_spi)
	: pp_ev_base_(p_base), buf_ev_(NULL), read_spi_(spi), m_dis_conn_spi(dis_conn_spi)
	, connect_(false)
{
	message_ = new TcpMessage(this);
}

TcpSession::~TcpSession(void)
{
	if (buf_ev_) { bufferevent_free(buf_ev_); }
	delete message_;
}

bool init_net(std::string& err)
{
#if defined(WIN32)  
	if (bWinStartUp) { return true; }

	WSADATA  Ws; 
	if (WSAStartup(MAKEWORD(2,2), &Ws) == 0)  
	{  
		bWinStartUp = true;
		return true;
	}
	err = "init_net error";
	return false;
#else
	return true;
#endif 
}

void denit_net()
{
#if defined(WIN32)  
	if (!bWinStartUp) { return; }
	WSACleanup();  
	bWinStartUp = false;
#endif  
}

bool init_sockaddr(sockaddr_in *addr, const char *ip, int port, std::string& err)
{
	if (ip == NULL || port <= 0)
	{
		err = "invalid address/port";
		return false;
	}
	memset(addr, 0, sizeof(sockaddr_in));  
    addr->sin_family = AF_INET;           
	addr->sin_addr.s_addr = inet_addr(ip); 
    addr->sin_port = htons(port);  
	return true;
}

socket_t create_socket(std::string& err)
{
	int protocol = 0;  
	socket_t sock;  
#if defined(WIN32)  
	protocol=IPPROTO_TCP;  
#endif  
	sock = socket(AF_INET, SOCK_STREAM, protocol);  
	if (sock == NAN_SOCKET) {
#if defined(WIN32) 
		int err_code = WSAGetLastError();
		switch(err_code)
		{			
		case WSAEAFNOSUPPORT:
			{
				err = "WSAEAFNOSUPPORT";
				break;
			}			
		case WSAEINVAL:
			{
				err = "WSAEINVAL";
				break;
			}	
		default:
			{
				err = "create_socket error";
				break;
			}
		}
#else
		err = "create_socket error";
#endif  
	}
	return sock;  
}

void close_socket(socket_t &handle)  
{  
#if defined(WIN32)  
	closesocket(handle);  
#else
	close(handle);  
#endif  
} 

bool TcpSession::Connect(const char *ip, int port, std::string& err)
{
	if (!init_net(err)) { return false; }

	if (*pp_ev_base_ == NULL)
	{
		err = "*pp_ev_base_ == NULL";
		return false;
	}

	struct sockaddr_in sin;
	if (!init_sockaddr(&sin, ip, port, err)) { return false; }

	socket_ = create_socket(err);
	if (socket_ == INVALID_SOCKET) { return false; }

	int ret = connect(socket_,(struct sockaddr *)&sin,sizeof(sin));
	if (ret < 0) {
#if defined(WIN32)  
		int err_code = WSAGetLastError();
		switch(err_code)
		{
		case WSAEAFNOSUPPORT:
			{
				err = "WSAEAFNOSUPPORT";
				break;
			}
		case WSAEFAULT:
			{
				err = "WSAEFAULT";
				break;
			}
		case WSAEINVAL:
			{
				err = "WSAEINVAL";
				break;
			}
		default:
			{
				err = "connect error";
				break;
			}
		}
#else
		err = "connect error";
#endif
		close_socket(socket_);
		socket_ = NAN_SOCKET; 
		return false;
	}

	if (NULL != buf_ev_) { bufferevent_free(buf_ev_); buf_ev_ = NULL; }

	buf_ev_ = bufferevent_socket_new(*pp_ev_base_, socket_, BEV_OPT_CLOSE_ON_FREE);
	if (!buf_ev_) {
		err = "Error constructing bufferevent\n";
		return false;
	}

	bufferevent_setcb(buf_ev_, handle_read, NULL, handle_disconnect, (void*)this);
	if (bufferevent_enable(buf_ev_, EV_WRITE) != 0 || bufferevent_enable(buf_ev_, EV_READ) != 0)
	{
		err = "bufferevent_enable read/write error";
		bufferevent_free(buf_ev_);
		buf_ev_ = NULL;
		return false;
	}

	connect_ = true;
	return true;
}

bool TcpSession::SetBufSize(bool is_recv, int size, std::string& err)
{
	int opt_name = SO_RCVBUF;
	if (!is_recv) {
		opt_name = SO_SNDBUF;
	}

	int ret = 0;
#if defined(WIN32)  
	ret = setsockopt(socket_, SOL_SOCKET, opt_name, (char*) &size, sizeof(size));
	if (ret == SOCKET_ERROR)
	{      
		int err_code = WSAGetLastError();
		std::stringstream ss;
		ss << "err code: " << err_code;
		err = ss.str();
		return false;
	}
#else
	ret = setsockopt(socket_, SOL_SOCKET, opt_name, (const void*) &size, (socklen_t)sizeof(size));
	if (-1 == ret)
	{
		err = "error on setsockopt";
		return false;
	}
#endif

	int new_size = 0;
	int size_len = sizeof(new_size);
#if defined(WIN32)
	ret = getsockopt(socket_, SOL_SOCKET, opt_name, (char*) &new_size, &size_len);
	if (ret == SOCKET_ERROR) { 
		err = "error on getsockopt";
		return false;
	}
#else
	ret = getsockopt(socket_, SOL_SOCKET, opt_name, (void*) &new_size, (socklen_t*)&size_len);
	if (ret == -1) { 
		err = "error on getsockopt";
		return false;
	}
#endif

	if (new_size != size)
	{
		err = "set error";
		return false;
	}

	return true;
}





bool TcpSession::bind_libevent(socket_t socket, std::string& err)
{
	if (socket == INVALID_SOCKET) { 
		err = "INVALID_SOCKET";
		return false; 
	}

	if (*pp_ev_base_ == NULL)
	{
		err = "*pp_ev_base_ == NULL";
		return false;
	}

	if (NULL != buf_ev_) { bufferevent_free(buf_ev_); buf_ev_ = NULL; }

	buf_ev_ = bufferevent_socket_new(*pp_ev_base_, socket, BEV_OPT_CLOSE_ON_FREE);
	if (!buf_ev_) {
		err = "bufferevent_socket_new error";
		return false;
	}

	bufferevent_setcb(buf_ev_, handle_read, NULL, handle_disconnect, (void*)this);
	if (bufferevent_enable(buf_ev_, EV_WRITE) != 0 || bufferevent_enable(buf_ev_, EV_READ) != 0)
	{
		err = "bufferevent_enable read/write";
		bufferevent_free(buf_ev_);
		buf_ev_ = NULL;
		return false;
	}

	socket_ = socket;
	connect_ = true;
	return true;
}

extern const char *CHECK_CODE;

bool TcpSession::Send(char* buf, int len, std::string& err)
{
	if (NULL == buf_ev_ || !is_connect())
	{
		err = "NULL == buf_ev_ || !is_connect()";
		return false;
	}

	if (NULL == buf || len <= 0)
	{
		return false;
	}


	char* send_buf = new char[TcpMessage::header_length + TcpMessage::body_length_value + len];
	memset(send_buf, 0, sizeof(send_buf));
	memcpy(send_buf, CHECK_CODE, TcpMessage::header_length);
	sprintf(send_buf + TcpMessage::header_length, "%12d", len);
	memcpy(send_buf + TcpMessage::header_length + TcpMessage::body_length_value, buf, len);
	

	while(send(socket_, send_buf, TcpMessage::header_length + TcpMessage::body_length_value + len, 0) <= 0)
	{
#if defined(WIN32) 
		int err_code = WSAGetLastError();
		printf("err code: %d\n", err_code);
		if (err_code == 10035)
		{
			baselib::Thread::Sleep(100);
		}
		else
		{
			err = "socket write error";
			delete [] send_buf;
			return false;
		}
#else
		err = "socket write error";
		delete [] send_buf;
		return false;
#endif
	}

	delete [] send_buf;
	return true;
}

void TcpSession::handle_read(struct bufferevent *bev, void *user_data)
{
	TcpSession *this_ptr = (TcpSession*)user_data;
	int now_read = 0;
	char buf[TcpMessage::max_buf_length];
	do 
	{
		memset(buf, 0, TcpMessage::max_buf_length);
		now_read = bufferevent_read(bev, buf, TcpMessage::max_buf_length);
		this_ptr->message_->decode(buf, now_read);
	} while (now_read == TcpMessage::max_buf_length);
}

void TcpSession::handle_disconnect(struct bufferevent *bev, short events, void *user_data)
{
	TcpSession *this_ptr = (TcpSession*)user_data;

	this_ptr->connect_ = false;
	SocketDissConnSpi* dis_conn_spi = this_ptr->m_dis_conn_spi;
	if (NULL != dis_conn_spi)
	{
		dis_conn_spi->OnDisconnect(this_ptr);
	}
}

}
