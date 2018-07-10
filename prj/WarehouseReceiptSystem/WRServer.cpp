#include "WRServer.h"
#include "Baselib/AppLog.h"

namespace kiiik {

WRServer::WRServer(int port)
{
	srv_ = new TcpServer(port, this, this);
	login_service_ = new LoginService;
	rw_data_service_ = new RWDataService;
}


WRServer::~WRServer(void)
{
	if (srv_) delete srv_;
	if(login_service_) delete login_service_;
	if(rw_data_service_) delete rw_data_service_;
}

void WRServer::OnReceive(TcpSession *tcp_sock, char* buf, int len){
	if (len < sizeof(ProtocolHead)) { /*ÓÐÎó*/ return; }
	ProtocolHead *req_head = (ProtocolHead*)buf;
	switch(req_head->type) {
	case REQ_LOGIN:
		{
			if (len != sizeof(LoginRequest)) return;
			LoginRequest *req = (LoginRequest*)buf;
			if(login_service_)login_service_->HandleReq(req, tcp_sock);
			APP_LOG(LOG_LEVEL_INFO) << "REQ_LOGIN";
		}
		break;
	case REQ_QUOTE:
		{
			if (len != sizeof(QuoteReq)) return;
			QuoteReq *req = (QuoteReq*)buf;
			if (rw_data_service_)rw_data_service_->HandleQuoteReq(req,tcp_sock);
			APP_LOG(LOG_LEVEL_INFO) << "REQ_QUOTE";
		}
		break;
	case REQ_WRITE:
		{
			if (len != sizeof(WtiteReq)) return;
			WtiteReq *req = (WtiteReq*)buf;
			if (rw_data_service_ && login_service_ && login_service_->CanWrite(tcp_sock))rw_data_service_->HandleWriteReq(req,tcp_sock);
			//APP_LOG(LOG_LEVEL_INFO) << "REQ_WRITE";
		}
		break;
	case REQ_SUBSCRIBE:
		{
			if (len <= sizeof(SubsReq)) return;
			SubsReq *req = (SubsReq*)buf;
			if (req->num<=0 || len != req->num*sizeof(CSymbol)+sizeof(SubsReq)) return;
			if (rw_data_service_)rw_data_service_->HandleSubsReq(req,tcp_sock);
			APP_LOG(LOG_LEVEL_INFO) << "REQ_SUBSCRIBE";
		}
		break;
	case REQ_UNSUB:
		{
			if (len <= sizeof(UnsubReq)) return;
			UnsubReq *req = (UnsubReq*)buf;
			if (req->num<=0 || len != req->num*sizeof(CSymbol)+sizeof(UnsubReq)) return;
			if (rw_data_service_)rw_data_service_->HandleUnsubsReq(req,tcp_sock);
			APP_LOG(LOG_LEVEL_INFO) << "REQ_UNSUB";
		}
		break;
	case REQ_SYMBOLS:
		{
			if (len != sizeof(SymbolsReq)) return;
			SymbolsReq *req = (SymbolsReq*)buf;
			
			if (rw_data_service_)rw_data_service_->HandleSymbosReq(tcp_sock);
			APP_LOG(LOG_LEVEL_INFO) << "REQ_SYMBOLS";
		}
		break;
	case REQ_VERSION:
		{
			if (len != sizeof(VersionRequest)) return;
			VersionRequest *req = (VersionRequest*)buf;
			if (login_service_)login_service_->HandleReq(req, tcp_sock);
			APP_LOG(LOG_LEVEL_INFO) << "REQ_VERSION";
		}
		break;
	default:
		break;
	}

}

void WRServer::OnAccept(TcpSession* sock){
	APP_LOG(LOG_LEVEL_INFO) << "OnAccept: " << sock->socket().remote_endpoint().address().to_string() << "\t" << sock->socket().remote_endpoint().port();
	
	if (rw_data_service_)rw_data_service_->OnAccept(sock);
}

void WRServer::OnDiscon(TcpSession *sock){
	try{
		APP_LOG(LOG_LEVEL_INFO) << "OnDiscon: " << sock->socket().remote_endpoint().address().to_string() << "\t" << sock->socket().remote_endpoint().port();
	}
	catch (...){
		APP_LOG(LOG_LEVEL_INFO) << "OnDiscon: " << "illeagle connect";
	}
	
	if (rw_data_service_)rw_data_service_->OnDiscon(sock);
	if (login_service_)login_service_->OnDiscon(sock);
}


}