#ifndef RSP_MSG_H
#define RSP_MSG_H

#include "wtpmduserapi/WtpUserApiStruct.h"

struct RspUserLogin{
	CWtpRspUserLoginField pRspUserLogin;
	CWtpRspInfoField pRspInfo;
};
struct RspUserLogout {
	CWtpUserLogoutField pUserLogout;
	CWtpRspInfoField pRspInfo;
};
struct RspQrySymbol{
	int nCount;
	CWtpSymbolField *pSymbol;
	~RspQrySymbol(){ delete []pSymbol; }
};

enum E_RspMsgType {
	E_fnOnFrontConnected,
	E_fnOnFrontDisconnected,
	E_fnOnRspUserLogin,
	E_fnOnRspUserLogout,
	E_fnOnRspError,
	E_fnOnRspQrySymbol,
	E_fnOnRspQryQuotation,
	E_fnOnRtnQuotation
};

class RspMsgItem {
	E_RspMsgType m_type;
	union 
	{
		RspUserLogin *loginItem;
		RspUserLogout *logoutItem;
		CWtpRspInfoField *errItem;
		RspQrySymbol *symbolItem;
		CWtpQuotationField *quotationItem;
	};
	
public:	
	E_RspMsgType get_type() const { return m_type; }
	operator RspUserLogin*() { return loginItem; }
	operator RspUserLogout*() { return logoutItem; }
	operator CWtpRspInfoField*() { return errItem; }
	operator RspQrySymbol*() { return symbolItem; }
	operator CWtpQuotationField*() { return quotationItem; }

	RspMsgItem() : loginItem(NULL) {};
	RspMsgItem(E_RspMsgType type, void *pItem) : m_type(type), loginItem(NULL) {
		switch (m_type)
		{
		case E_fnOnFrontConnected:
			break;
		case E_fnOnFrontDisconnected:
			break;
		case E_fnOnRspUserLogin:
			loginItem = new RspUserLogin(*(RspUserLogin*)pItem);
			break;
		case E_fnOnRspUserLogout:
			logoutItem = new RspUserLogout(*(RspUserLogout*)pItem);
			break;
		case E_fnOnRspError:
			errItem = new CWtpRspInfoField(*(CWtpRspInfoField*)pItem);
			break;
		case E_fnOnRspQrySymbol:{
									SymbolsRsp *sym_rsp = (SymbolsRsp*)pItem;
									symbolItem = new RspQrySymbol;
									symbolItem->nCount = sym_rsp->num;
									symbolItem->pSymbol = new CWtpSymbolField[symbolItem->nCount];
									memcpy(symbolItem->pSymbol, sym_rsp->sym, sizeof(CWtpSymbolField)*symbolItem->nCount);
		}
			break;
		case E_fnOnRspQryQuotation:
			quotationItem = new CWtpQuotationField(*(CWtpQuotationField*)pItem);
			break;
		case E_fnOnRtnQuotation:
			quotationItem = new CWtpQuotationField(*(CWtpQuotationField*)pItem);
			break;
		default:
			break;
		}
	}
	~RspMsgItem(){
		switch (m_type)
		{
		case E_fnOnFrontConnected:
			break;
		case E_fnOnFrontDisconnected:
			break;
		case E_fnOnRspUserLogin:
			delete loginItem;
			break;
		case E_fnOnRspUserLogout:
			delete logoutItem;
			break;
		case E_fnOnRspError:
			delete errItem;
			break;
		case E_fnOnRspQrySymbol:
			delete symbolItem;
			break;
		case E_fnOnRspQryQuotation:
			delete quotationItem;
			break;
		case E_fnOnRtnQuotation:
			delete quotationItem;
			break;
		default:
			break;
		}
		loginItem = NULL;
	}
};

#endif