#include "network/TcpServer.h"
#include <algorithm>
#include <stdio.h>
#include <assert.h>
#include <string.h>

namespace network {

TcpServer::TcpServer(int port, SocketReaderSpi* spi, TcpServerConnSpi* conn_spi)
	: read_spi_(spi), ev_base_(NULL), listener_(NULL), is_init_(false), conn_spi_(conn_spi)
{
	memset(&sock_addr_, 0, sizeof(sock_addr_));
	sock_addr_.sin_family = AF_INET;
	sock_addr_.sin_port = htons(port);
}

TcpServer::~TcpServer(void)
{
	DleSocket();
	Stop();
}

void TcpServer::OnRun()
{
	while (IsRuning())
	{
		if (event_base_dispatch(ev_base_) < 0)
		{
			fprintf(stderr, "event_base_dispatch error\n");
		}
		baselib::Thread::Sleep(3000);
	}
}

bool TcpServer::StartUp(std::string& err)
{
	if (is_init_) { return true; }

	if (!init_net(err)) { return false; }

	if (!ev_base_) {
		ev_base_ = event_base_new();
		if (!ev_base_) {
			err = "event_base_new error";
			return false;
		}
	}

	if (!listener_) {
		listener_ = evconnlistener_new_bind(ev_base_, handle_accept, (void *)this,
			LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1, (struct sockaddr*)&sock_addr_, sizeof(sock_addr_));
		if (!listener_) {
			err = "evconnlistener_new_bind error";
			return false;
		}
	}

	Start();

	return true;
}

void TcpServer::OnAccept(TcpSession* sock)
{
	if (NULL != conn_spi_)
	{
		conn_spi_->OnAccept(sock);
	}
}

void TcpServer::PushSocket(TcpSession* sock)
{
	baselib::Locker locker(&socks_mtx_);
	invalid_socks_.push_back(sock);
}

void TcpServer::DleSocket()
{
	baselib::Locker locker(&socks_mtx_);
	std::list<TcpSession*>::iterator iter = invalid_socks_.begin();
	while (iter != invalid_socks_.end())
	{
		if (!(*iter)->is_connect())
		{
			std::list<TcpSession*>::iterator tmp = iter;
			iter++;
			TcpSession* sock = (*tmp);
			delete sock;
			invalid_socks_.erase(tmp);
		}
		else
		{
			iter++;
		}
	}
}

void TcpServer::handle_accept(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *sa, int socklen, void *user_data)
{
	printf("new session: %d\n", fd);

	TcpServer *this_ptr = (TcpServer*)user_data;
	struct event_base *base = this_ptr->ev_base_;
	TcpSession* tcp_sock = new TcpSession(&base, this_ptr->read_spi_, this_ptr);
	std::string err;
	if (!tcp_sock->bind_libevent(fd, err))
	{
		fprintf(stderr, err.c_str());
		return;
	}

	if (!tcp_sock->SetBufSize(false, 1024000, err))	//设置0的缓冲区，可以无延迟发送，而且避免send函数的10035错误
	{
		fprintf(stderr, err.c_str());
		return;
	}

	this_ptr->OnAccept(tcp_sock);
}

void TcpServer::OnDisconnect(TcpSession *tcp_sock)
{
	if (NULL != conn_spi_)
	{
		conn_spi_->OnDiscon(tcp_sock);
	}

	DleSocket();	//删除之前的无效socket(注：无法删除当前的socket)
	std::list<TcpSession*>::iterator iter = std::find(invalid_socks_.begin(), invalid_socks_.end(), tcp_sock);
	PushSocket(tcp_sock);	//放入列表，等到后面再删除
}

}
