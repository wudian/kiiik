#include "network/TcpClient.h"

extern "C"{
#include <event2/bufferevent.h>
#include <event2/listener.h>
};

namespace network {

TcpClient::TcpClient(const char *ip, int port, SocketReaderSpi* spi, ReConnSpi* re_conn_spi)
	: read_spi_(spi), ev_base_(NULL), tcp_sock_(NULL), ip_(ip), port_(port), m_is_start_on_read(false)
	, re_conn_spi_(re_conn_spi), is_connect_(false)
{
	
}


TcpClient::~TcpClient(void)
{
	if (NULL != ev_base_) { event_base_free(ev_base_); ev_base_ = NULL; }
	if (NULL != tcp_sock_) { delete tcp_sock_; tcp_sock_ = NULL; }
}

void TcpClient::OnDisconnect(TcpSession *tcp_sock)
{
	printf("Reconnect\n");
	is_connect_ = false;

	while (1)
	{
		{
			baselib::Locker locker(&mutex_);
			if (!m_is_start_on_read)
			{
				return;
			}
		}
		if (ReConnect())
		{
			break;
		}
		baselib::Thread::Sleep(3000);
	}

	is_connect_ = true;
	if (NULL != re_conn_spi_) { re_conn_spi_->SockReConn(); }
	printf("connect succed\n");

}

bool TcpClient::ReConnect()
{
	std::string err;
	if (!tcp_sock_->Connect(ip_.c_str(), port_, err))
	{
		fprintf(stderr, "connect error: %s\n", err.c_str());
		return false;
	}

	return true;
}

bool TcpClient::StartUp(std::string& err)
{
	if (m_is_start_on_read) { return true; }

	if (!init_net(err)) { return false; }

	ev_base_ = event_base_new();
	if (!ev_base_) {
		event_base_free(ev_base_);
		ev_base_ = NULL;
		err = "Could not initialize libevent";
		return false;
	}

	tcp_sock_ = new TcpSession(&ev_base_, read_spi_, this);
	if (!tcp_sock_->Connect(ip_.c_str(), port_, err)) { 
		event_base_free(ev_base_);
		ev_base_ = NULL;
		delete tcp_sock_;
		tcp_sock_ = NULL;
		return false; 
	}

	Start();
	is_connect_ = true;
	m_is_start_on_read = true;
	return true;
}

void TcpClient::TearDown()
{
	baselib::Locker locker(&mutex_);
	if (!m_is_start_on_read) { return; }
	m_is_start_on_read = false;

	if (NULL != tcp_sock_)
	{
		delete tcp_sock_;
		tcp_sock_ = NULL;
	}

	Stop();
	if (NULL != ev_base_) { 
		event_base_loopbreak(ev_base_); 
		//event_base_free(ev_base_); //¼ÓÁË»á±¼À£
		ev_base_ = NULL;
	}
	is_connect_ = false;
}

bool TcpClient::Send(char* buf, int len, std::string& err)
{
	if (!is_connect_) 
	{
		err = "socket is not connected";
		return false;
	}
	return tcp_sock_->Send(buf, len, err);
}

void TcpClient::OnRun()
{
	while (IsRuning())
	{
		if (event_base_dispatch(ev_base_) < 0)
		{
			if (NULL == tcp_sock_ || NULL == ev_base_ || !m_is_start_on_read)
			{
				break;
			}
			fprintf(stderr, "Dispatch event error\n");
		}
		//baselib::Thread::Sleep(3000);
	}
	//if (NULL != ev_base_)
	//{
	//	event_base_free(ev_base_);
	//	ev_base_ = NULL;
	//}
}

}
