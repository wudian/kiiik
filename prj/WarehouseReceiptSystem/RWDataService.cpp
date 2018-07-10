#include "RWDataService.h"
#include "Baselib/Mutex.h"

namespace kiiik {

using namespace baselib;

void SessionContainer::Append(TcpSession* _Session){
	if (tick_){
		Locker lock(&tick_mutex_);
		//_Session->Send((char*)tick_, sizeof(CQuotation));
		DataRsp data_rsp;
		data_rsp.type = RSP_SUB_DATA;
		data_rsp.data = *tick_;
		_Session->Send((char*)&data_rsp, sizeof(DataRsp));
	}

	{
		Locker lock(&sub_sess_mutex_);
		list<TcpSession*>::iterator iter = find(sub_sessions_.begin(), sub_sessions_.end(), _Session);
		if (iter == sub_sessions_.end())
			sub_sessions_.push_back(_Session);
	}
}

void SessionContainer::Remove(TcpSession* _Session){
	Locker lock(&sub_sess_mutex_);
	list<TcpSession*>::iterator iter = find(sub_sessions_.begin(), sub_sessions_.end(), _Session);
	if (iter != sub_sessions_.end())
		sub_sessions_.erase(iter);
}

void SessionContainer::SetTick(CQuotation* _WrTick){
	Locker locker(&tick_mutex_);
	if (NULL != tick_) { free(tick_); }
	tick_ = new CQuotation(*_WrTick);
}

CQuotation* SessionContainer::GetTick() {
	Locker locker(&tick_mutex_);
	if (tick_){
		return new CQuotation(*tick_);
	}
	else
		return NULL;
}

void SessionContainer::Send(char *buf, int len) {
	Locker lock(&sub_sess_mutex_);
	list<TcpSession*>::iterator iter;
	for (iter = sub_sessions_.begin(); iter!=sub_sessions_.end(); ++iter){
		(*iter)->Send(buf, len);
	}
}


RWDataService::RWDataService(void)
{
}


RWDataService::~RWDataService(void)
{
}

void RWDataService::OnAccept(TcpSession* tcp_sock){
	Locker lock(&sess_symbols_mutex);
	sess_symbols_[tcp_sock] = list<CSymbol>();
}
void RWDataService::OnDiscon(TcpSession *tcp_sock){
	Locker lock(&sess_symbols_mutex);
	Session2Symbols::iterator iter = sess_symbols_.find(tcp_sock);
	if (iter != sess_symbols_.end()) {
		list<CSymbol> &sym_list = iter->second;
		for (list<CSymbol>::iterator sym_iter = sym_list.begin(); sym_iter!=sym_list.end(); ++sym_iter){
			SessionContainer* sess_cont = GetSessionContainer(*sym_iter);
			if(sess_cont)sess_cont->Remove(tcp_sock);
		}
		sess_symbols_.erase(iter);
	}
}

void RWDataService::HandleWriteReq(WtiteReq *_WriReq ,TcpSession* _Sess){

	SessionContainer* sess_cont = GetSessionContainer(_WriReq->data.symbol);
	CQuotation* tick = &_WriReq->data;
	if (sess_cont){
		sess_cont->SetTick(tick);
		// 向订阅了该合约行情的客户端推送过去
		DataRsp data_rsp;
		data_rsp.data = *tick;
		//_Sess->Send((char*)&data_rsp, sizeof(DataRsp));
		sess_cont->Send((char*)&data_rsp, sizeof(DataRsp));
	}
}

void RWDataService::HandleQuoteReq(QuoteReq* _QuoteReq,TcpSession* _Sess){
	SessionContainer* sess_cont = GetSessionContainer(_QuoteReq->sym, false);
	if (sess_cont){
		CQuotation *tick = sess_cont->GetTick();
		if (tick){
			DataRsp data_rsp;
			data_rsp.type = RSP_QUOTE_DATA;
			data_rsp.data = *tick;
			free(tick);
			_Sess->Send((char*)&data_rsp, sizeof(DataRsp));
		}		
	}	
}

void RWDataService::HandleSubsReq(SubsReq* _SubsReq,TcpSession* _Sess){
	// 订阅某个合约的行情
	for (int i=0; i<_SubsReq->num; ++i){
		Subscribe(_SubsReq->sym[i], _Sess);
	}
	
}
void RWDataService::HandleUnsubsReq(UnsubReq* _UnsubsReq,TcpSession* _Sess){
	for (int i=0; i<_UnsubsReq->num; ++i){
		UnSubscribe(_UnsubsReq->sym[i], _Sess);
	}
}

void RWDataService::HandleSymbosReq(TcpSession* _Sess){
	Locker lock(&sub_sym_sessions_mutex_);
	int len = sizeof(SymbolsRsp)+sub_sym_sessions_.size()*sizeof(CSymbol);
	SymbolsRsp *syms_rsp = (SymbolsRsp*)malloc(len);
	memset(syms_rsp, 0, len);
	syms_rsp->type = RSP_SYMBOLS;
	for (Symbol2Sessions::iterator iter = sub_sym_sessions_.begin(); iter!=sub_sym_sessions_.end(); ++iter){
		memcpy(syms_rsp->sym + syms_rsp->num++, &iter->first, sizeof(CSymbol));
	}
	_Sess->Send((char*)syms_rsp, len);
	free(syms_rsp);
}

void RWDataService::Subscribe(const CSymbol& _Sym, TcpSession *_Sess){
	Locker lock(&sess_symbols_mutex);
	Session2Symbols::iterator iter = sess_symbols_.find(_Sess);
	if (iter == sess_symbols_.end()) return;// 未连接的session

	list<CSymbol> &sym_list = iter->second;
	std::list<CSymbol>::iterator sym_iter = std::find(sym_list.begin(), sym_list.end(), _Sym);
	if (sym_iter != sym_list.end()) { return; }  //不重复订阅

	SessionContainer *sess = GetSessionContainer(_Sym);
	assert(sess);
	sess->Append(_Sess);
	sym_list.push_back(_Sym);
}

void RWDataService::UnSubscribe(const CSymbol& _Sym, TcpSession *_Sess){
	Locker lock(&sess_symbols_mutex);
	Session2Symbols::iterator iter = sess_symbols_.find(_Sess);
	if (iter == sess_symbols_.end()) return;// 未连接的session

	list<CSymbol> &sym_list = iter->second;
	std::list<CSymbol>::iterator sym_iter = std::find(sym_list.begin(), sym_list.end(), _Sym);
	if (sym_iter == sym_list.end()) { return; }  //没有订阅
	sym_list.erase(sym_iter);

	SessionContainer *sess = GetSessionContainer(_Sym, false);
	if(sess){
		sess->Remove(_Sess);
	}
	
}

SessionContainer* RWDataService::GetSessionContainer(const CSymbol& _Sym,  bool _Wri_Sub){
	Symbol2Sessions::iterator iter = sub_sym_sessions_.find(_Sym);
	if (iter == sub_sym_sessions_.end()) {
		if (_Wri_Sub){
			SessionContainer* sess = new SessionContainer;
			sub_sym_sessions_[_Sym] = sess;
			return sess;
		}
		else
			return NULL;
	}
	else
		return iter->second;
}

}