#pragma once

#include <map>
#include <list>
#include "WRStruct.h"
#include "Baselib/SpinLock.h"
#include "Protocol.h"
#include "NetworkAsio/TcpSession.h"

namespace kiiik {

using namespace network_asio;
using namespace baselib;

// 订阅了某个symbol的session集
class SessionContainer
{
public:
	SessionContainer():tick_(NULL){}

	void Append(TcpSession*);
	void Remove(TcpSession*);

	void SetTick(CQuotation*);
	CQuotation* GetTick() ;

	void Send(char *buf, int len);

private:
	CQuotation *tick_;
	SpinLock tick_mutex_;

	list<TcpSession*> sub_sessions_; // 订阅了的session
	SpinLock sub_sess_mutex_;
};

typedef map<CSymbol, SessionContainer*> Symbol2Sessions;
typedef map<TcpSession*, list<CSymbol> > Session2Symbols;

class RWDataService
{
public:
	RWDataService(void);
	~RWDataService(void);

	void OnAccept(TcpSession* sock);
	void OnDiscon(TcpSession *tcp_sock);

	void HandleQuoteReq(QuoteReq*,TcpSession*);
	void HandleWriteReq(WtiteReq *,TcpSession*);
	void HandleSubsReq(SubsReq*,TcpSession*);
	void HandleUnsubsReq(UnsubReq*,TcpSession*);
	void HandleSymbosReq(TcpSession*);

private:
	void Subscribe(const CSymbol& _Sym, TcpSession *_Sess);
	void UnSubscribe(const CSymbol& _Sym, TcpSession *_Sess);

	SessionContainer* GetSessionContainer(const CSymbol& _Sym, bool _Wri_Sub = true);

	//map<CSymbol, CQuotation> ticks_; // 最新的报价
	//baselib::SpinLock ticks_mutex_;

	Symbol2Sessions sub_sym_sessions_;// 订阅了某个symbol的session集
	baselib::SpinLock sub_sym_sessions_mutex_;

	Session2Symbols sess_symbols_;//每个socket维护一个订阅列表，当socket断开时能快速取消订阅该socket的行情
	baselib::SpinLock sess_symbols_mutex;
};

}