#include "WRClient.h"
#include "assert.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp> 
#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>
#include <boost/shared_ptr.hpp>
#include "Baselib/AppLog.h"
#include "Baselib/SimpleMath.h"
#include "Baselib/Global.h"
#include "wtpmduserapi/WtpMdCApi.h"
#include "GlobalVars.h"

using namespace std;
using namespace baselib;
using boost::property_tree::ptree;

static const string str_node_name[] = { "ip", "port" };

WRClient *instance = NULL;
CWtpMdApi *CWtpMdApi::CreateWtpMdApi() {
	if (!instance) {
		instance = new WRClient;
	}		
	return instance;
}

WRClient::WRClient() : is_login_(false), client_(NULL), spi_(NULL), nRequestID_(0)
{
	Start();
	
}

WRClient::~WRClient(){
	if (client_) delete client_;
}

///删除接口对象本身
///@remark 不再使用本接口对象时,调用该函数删除接口对象
void WRClient::Release() {
	delete this;
	instance = NULL;
}



///注册前置机网络地址
///@param pszFrontAddress：前置机网络地址。
///@remark 网络地址的格式为：“protocol://ipaddress:port”，如：”tcp://127.0.0.1:17001”。 
///@remark “tcp”代表传输协议，“127.0.0.1”代表服务器地址。”17001”代表服务器端口号。
void WRClient::RegisterFront(char *pszFrontAddress) {
	string FrontAddress = pszFrontAddress;
	size_t i = FrontAddress.find("//");
	size_t j = FrontAddress.find_last_of(":");
	if (string::npos == i || string::npos == j || i > j) {
		// 格式输入错误
		CWtpRspInfoField err = { 0 };
		strcpy(err.m_ErrMsg, "pszFrontAddress's format is not protocol://ipaddress:port");
		Push(std::make_shared<RspMsgItem>(E_fnOnRspError, &err));
	}

	IP_PORT ip_port;
	ip_port.ip = FrontAddress.substr(i + 2, j - i - 2);
	ip_port.port = FrontAddress.substr(j + 1, FrontAddress.size() - j);
	ip_ports_.push_back(ip_port);
}


///注册回调接口
///@param pSpi 派生自回调接口类的实例
void WRClient::RegisterSpi(CWtpMdSpi *pSpi) {
	spi_ = pSpi;
}


///用户登录请求
int WRClient::ReqUserLogin(CWtpReqUserLoginField *pReqUserLoginField) {
	if (!pReqUserLoginField) return -1;
	if (!is_login_) {
		strcpy(id_, pReqUserLoginField->m_UserId);
		strcpy(pwd_, pReqUserLoginField->m_Password);
		//
		if (ip_ports_.empty()) return -1;
		string ip = ip_ports_.front().ip;
		string port = ip_ports_.front().port;
		if (!client_) {
			client_ = new TcpClient(ip.c_str(), port.c_str(), this, this);
			client_->StartUp();
		}
		else {
			LoginRequest loginReq;
			memcpy(loginReq.user_id, id_, sizeof(loginReq.user_id));
			strcpy(loginReq.pwd, pwd_);
			client_->Send((char*)&loginReq, sizeof(LoginRequest));
		}
	}
	return 0;
}


///登出请求
int WRClient::ReqUserLogout(CWtpUserLogoutField *pUserLogout) {
	is_login_ = false;

	RspUserLogout logoutItem = { 0 };
	strcpy(logoutItem.pUserLogout.m_UserID, pUserLogout->m_UserID);
	Push(std::make_shared<RspMsgItem>(E_fnOnRspUserLogout, &logoutItem));
	return 0;
}


int WRClient::ReqQryQuotation(CWtpSymbolField *pInstrumentID) {
	if (!is_login_) return -1;
	QuoteReq quoteReq;
	quoteReq.sym = *pInstrumentID;

	client_->Send((char*)&quoteReq, sizeof(QuoteReq));
	return 0;
}

int WRClient::ReqSubQuotation(CWtpSymbolField *pInstrumentID, int nCount){
	if (!is_login_) return -1;
	int len = sizeof(SubsReq)+nCount*sizeof(CWtpSymbolField);
	SubsReq *req = (SubsReq*)malloc(len);
	memset(req, 0, len);
	req->type = REQ_SUBSCRIBE;
	for (int i = 0; i < nCount; ++i){
		memcpy(req->sym + req->num++, &(pInstrumentID[i]), sizeof(CWtpSymbolField));
	}
	client_->Send((char*)req, len);

	free(req);
	return 0;
}

int WRClient::ReqUnSubQuotation(CWtpSymbolField *pInstrumentID, int nCount){
	if (!is_login_) return -1;
	int len = sizeof(UnsubReq)+nCount*sizeof(CWtpSymbolField);
	UnsubReq *req = (UnsubReq*)malloc(len);
	memset(req, 0, len);
	req->type = REQ_UNSUB;
	for (int i = 0; i < nCount; ++i){
		memcpy(req->sym + req->num++, &(pInstrumentID[i]), sizeof(CWtpSymbolField));
	}
	client_->Send((char*)req, len);

	free(req);
	return 0;
}


int WRClient::ReqQrySymbol(){
	if (!is_login_) return -1;
	SymbolsReq symsReq;
	client_->Send((char*)&symsReq, sizeof(SymbolsReq));
	return 0;
}


void WRClient::SockReConn(){
	is_login_ = true;
	if (spi_)spi_->OnFrontConnected();
	if (g_fnOnFrontConnected) g_fnOnFrontConnected();
}

void WRClient::SockConn(bool success){
	if (success) {
		if (spi_)spi_->OnFrontConnected();
		if (g_fnOnFrontConnected) g_fnOnFrontConnected();		
		LoginRequest loginReq;
		memcpy(loginReq.user_id, id_, sizeof(loginReq.user_id));
		strcpy(loginReq.pwd, pwd_);
		client_->Send((char*)&loginReq, sizeof(LoginRequest));
	}
	else
	{
		if (spi_)spi_->OnFrontDisconnected();
		if (g_fnOnFrontDisconnected) g_fnOnFrontDisconnected();
	}
}
//网络断开
void WRClient::SockDisconn(){
	is_login_ = false;
	
	if (spi_)spi_->OnFrontDisconnected();
	if (g_fnOnFrontDisconnected) g_fnOnFrontDisconnected();
}


bool WRClient::Consume(const std::shared_ptr<RspMsgItem>& val){
	switch (val->get_type())
	{
	case E_fnOnRspUserLogin:
	{
		RspUserLogin *loginItem = (RspUserLogin*)*val;
		if (spi_)spi_->OnRspUserLogin(&loginItem->pRspUserLogin, &loginItem->pRspInfo);
		if (g_fnOnRspUserLogin) g_fnOnRspUserLogin(&loginItem->pRspUserLogin, &loginItem->pRspInfo);
	}
		break;
	case E_fnOnRspUserLogout:{
			RspUserLogout *logoutItem = (RspUserLogout*)*val;
			if (spi_)spi_->OnRspUserLogout(&logoutItem->pUserLogout, &logoutItem->pRspInfo);
			if (g_fnOnRspUserLogout)g_fnOnRspUserLogout(&logoutItem->pUserLogout, &logoutItem->pRspInfo);
	}
		break;
	case E_fnOnRspError:
	{
						   CWtpRspInfoField *errItem = (CWtpRspInfoField*)*val;
						   if (spi_) spi_->OnRspError(errItem);
						   if (g_fnOnRspError) g_fnOnRspError(errItem);
	}
		break;
	case E_fnOnRspQrySymbol:{
								RspQrySymbol *symItem = (RspQrySymbol*)*val;
								if (spi_)spi_->OnRspQrySymbol(symItem->pSymbol, symItem->nCount);
								if (g_fnOnRspQrySymbol)g_fnOnRspQrySymbol(symItem->pSymbol, symItem->nCount);
	}
		break;
	case E_fnOnRspQryQuotation:{
								   CWtpQuotationField*quotatonItem = (CWtpQuotationField*)*val;
								   if (spi_)spi_->OnRspQryQuotation(quotatonItem);
								   if (g_fnOnRspQryQuotation)g_fnOnRspQryQuotation(quotatonItem);
	}
		break;
	case E_fnOnRtnQuotation:{
								CWtpQuotationField*quotatonItem = (CWtpQuotationField*)*val;
								if (spi_)spi_->OnRtnQuotation(quotatonItem);
								if (g_fnOnRtnQuotation)g_fnOnRtnQuotation(quotatonItem);
	}
		break;
	default:
		break;
	}
	return true;
}

void WRClient::OnReceive(TcpSession *tcp_sock, char* buf, int len){
	ProtocolHead *head = (ProtocolHead *)buf;
	switch(head->type){
	case RSP_LOGIN:
		{
			assert(len == sizeof(LoginResponse));
			LoginResponse* login_rsp = (LoginResponse*)buf;
			is_login_ = login_rsp->succeed;
			if (login_rsp->succeed) {
				RspUserLogin loginItem = {0};
				loginItem.pRspUserLogin.m_Success = true;

				Push(std::make_shared<RspMsgItem>(E_fnOnRspUserLogin, &loginItem));
			}
			else {
				RspUserLogin loginItem = { 0 };
				loginItem.pRspUserLogin.m_Success = false;
				
				string errMsg = login_rsp->msg;
				if (errMsg == "0")
					strcpy(loginItem.pRspInfo.m_ErrMsg, "userId not exist");
				else
					strcpy(loginItem.pRspInfo.m_ErrMsg, "password error");
				Push(std::make_shared<RspMsgItem>(E_fnOnRspUserLogin, &loginItem));				
			}			
		}
		break;
	case RSP_VERSION:
		{
			/*
			VersionResponse* ver_rsp = (VersionResponse*)buf;
			if (ver_rsp->file_length > 0){	
				string file_name = ver_rsp->file_name;
				if ("VersionUpdate.exe" == file_name){
					ofstream ofs(bin_folder_ + ver_rsp->file_name, ios::binary | ios::trunc);
					ofs.write(ver_rsp->content, ver_rsp->file_length);
					ofs.close();
				}
				else{
					ofstream ofs(update_folder_ + ver_rsp->file_name, ios::binary | ios::trunc);
					ofs.write(ver_rsp->content, ver_rsp->file_length);
					ofs.close();
				}
			}
			else if (WANT_TO_DOWNLOAD == ver_rsp->file_length) {
				emit SignalNetErr(QStringLiteral("  有新版本，请更新"));
			}
			else if (IS_NEW_VERSION == ver_rsp->file_length) {
				emit SignalNetErr(QStringLiteral("  目前是最新版本"));
			}
			else if (UPDATE_END == ver_rsp->file_length){
				emit SignalNetErr(QStringLiteral("  下载完毕，是否安装"));
			}
			*/
		}
		break;
	case RSP_QUOTE_DATA:
		{
			DataRsp *data = (DataRsp*)buf;
			Push(std::make_shared<RspMsgItem>(E_fnOnRspQryQuotation, &data->data));			
		}
		break;
	case RSP_SUB_DATA:
		{
			DataRsp *data = (DataRsp*)buf;
			Push(std::make_shared<RspMsgItem>(E_fnOnRtnQuotation, &data->data));
		}
		break;
	case RSP_SYMBOLS:
		{
			SymbolsRsp *sym_rsp = (SymbolsRsp*)buf;
			if (sym_rsp->num > 0) {
				Push(std::make_shared<RspMsgItem>(E_fnOnRspQrySymbol, sym_rsp));
			}			
		}
		break;
	default:
		break;
	}

}

void WRClient::Trim(std::string &s) 
{
	if (s.empty()) 
	{
		return;
	}

	s.erase(0,s.find_first_not_of(" "));
	s.erase(s.find_last_not_of(" ") + 1);
}

void WRClient::Split(const std::string& s, const std::string delim,std::vector<std::string>* ret)
{
	size_t last = 0;  
	size_t index=s.find_first_of(delim,last);  
	while (index!=std::string::npos)  
	{  
		std::string part_str = s.substr(last,index-last);
		Trim(part_str);
		if (!part_str.empty() && std::find(m_symbols.end(), m_symbols.end(), part_str) == m_symbols.end()) 
		{ 
			ret->push_back(part_str); 
		}
		last=index+1;  
		index=s.find_first_of(delim,last);  
	}  
	if (index - last > 0)  
	{  
		std::string part_str = s.substr(last,index-last);
		Trim(part_str);
		if (!part_str.empty() && std::find(m_symbols.end(), m_symbols.end(), part_str) == m_symbols.end()) 
		{ 
			ret->push_back(part_str); 
		}
	}  
}