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
#include "Baselib/AppLog.h"
#include "Baselib/SimpleMath.h"
#include "Baselib/Global.h"

using namespace std;
using namespace baselib;
using boost::property_tree::ptree;

static const string str_node_name[] = { "ip", "port" };

WRClient::WRClient() : is_login_(false), client_(NULL)
{
	config_file_ = KiiikHome() + "config\\Client.xml";
	version_cfg_file_ = KiiikHome() + "config\\version.xml";
#ifdef DEBUG
	update_folder_ = KiiikHome() + "update\\";
	bin_folder_ = KiiikHome() + "bin\\Debug\\";
#else
	update_folder_ = KiiikHome() + "update\\";
	bin_folder_ = KiiikHome() + "bin\\";
#endif
	if (!IsDirExist(update_folder_))
		MakeDir(update_folder_);
	readxml();
}

WRClient::~WRClient(){
	//if (client_) delete client_;
}

void WRClient::readxml(){
	IP_PORT xml_node;
	ptree root;
	ptree version_root;
	try
	{
		read_xml(config_file_, root);
		read_xml(version_cfg_file_, version_root);
	}
	catch (std::exception& e)
	{
		return;
	}

	string str_version = version_root.get_child("version").data();
	version_ = atof(str_version.c_str()); // 客户端版本号

	for (ptree::iterator itr = root.begin(); itr != root.end(); itr++)
	{
		ptree ip_port = itr->second;

		for (int i = 0; i < 2; i++)
		{
			ptree pt;
			string str_value;
			try
			{
				pt = ip_port.get_child(str_node_name[i]);
				str_value = pt.data();
			}
			catch (std::exception& e)
			{
				return;
			}
			switch (i)
			{
			case 0:
				xml_node.ip = str_value;
				break;
			case 1:
				xml_node.port = str_value;
				break;			
			}

		}
		ip_ports_.push_back(xml_node);
	}
}


// 测试写客户端: 每隔500ms, 写数据
void WRClient::OnTimer(){
	WtiteReq writeReq1;
	baselib::DateTime now(NULL);
	writeReq1.data.m_SystemDate = now.date.toLong();
	writeReq1.data.m_SystemTime = now.time.toLong();
	writeReq1.data.symbol = CSymbol("SHFE", "CU", 100);
	writeReq1.data.m_AlphaLastPrice = rand()%100;
	client_->Send((char*)&writeReq1, sizeof(WtiteReq));
	//APP_LOG(LOG_LEVEL_INFO) << "write symbol : " << writeReq1.data.symbol.Str();
	

	WtiteReq writeReq2;
	writeReq2.data.m_SystemDate = now.date.toLong();
	writeReq2.data.m_SystemTime = now.time.toLong();
	writeReq2.data.symbol = CSymbol("CZCE", "CF", 100);
	writeReq2.data.m_AlphaLastPrice = rand() % 100;
	client_->Send((char*)&writeReq2, sizeof(WtiteReq));	
	//APP_LOG(LOG_LEVEL_INFO) << "write symbol : " << writeReq2.data.symbol.Str();
	
}

void WRClient::Login(UserIdType id, PwdType pwd){
	if (!is_login_) {
		strcpy(id_, id);
		strcpy(pwd_, pwd);
		//
		if (ip_ports_.empty()) return;
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
}

void WRClient::Logout(){
	is_login_ = false;
	client_->TearDown();
}

void WRClient::TestTimerWrite(){
#if 1
	WtiteReq writeReq1;
	baselib::DateTime now(NULL);
	writeReq1.data.m_SystemDate = now.date.toLong();
	writeReq1.data.m_SystemTime = now.time.toLong();
	cout << "input symbol's m_ExchangeId m_ProductId and m_ContractDays\n";
	cout << "m_ExchangeId:"; cin >> writeReq1.data.symbol.m_ExchangeId;
	cout << "m_ProductId:"; cin >> writeReq1.data.symbol.m_ProductId;
	cout << "m_ContractDays:"; cin >> writeReq1.data.symbol.m_ContractDays;

	client_->Send((char*)&writeReq1, sizeof(WtiteReq));
#else
	TimerApi *timer_manager = new TimerApi(500, this);
	timer_manager->Start();
#endif
}

void WRClient::Quote(const CSymbol &sym){
	if (!is_login_) return;
	QuoteReq quoteReq;
	quoteReq.sym = sym;

	client_->Send((char*)&quoteReq, sizeof(QuoteReq));
	APP_LOG(LOG_LEVEL_INFO) << "Quote";
}

void WRClient::Sub(std::vector<CSymbol> &inst_vec){
	if (!is_login_) return;
	int len = sizeof(SubsReq)+inst_vec.size()*sizeof(CSymbol);
	SubsReq *req = (SubsReq*)malloc(len);
	memset(req, 0, len);
	req->type = REQ_SUBSCRIBE;
	for (std::vector<CSymbol>::iterator iter = inst_vec.begin(); iter != inst_vec.end(); ++iter){
		memcpy(req->sym + req->num++, &(*iter), sizeof(CSymbol));
	}
	client_->Send((char*)req, len);

	free(req);
	APP_LOG(LOG_LEVEL_INFO) << "Sub";
}

void WRClient::UnSub(std::vector<CSymbol> &inst_vec){
	if (!is_login_) return;
	int len = sizeof(UnsubReq)+inst_vec.size()*sizeof(CSymbol);
	UnsubReq *req = (UnsubReq*)malloc(len);
	memset(req, 0, len);
	req->type = REQ_UNSUB;
	for (std::vector<CSymbol>::iterator iter = inst_vec.begin(); iter != inst_vec.end(); ++iter){
		memcpy(req->sym + req->num++, &(*iter), sizeof(CSymbol));
	}
	client_->Send((char*)req, len);

	free(req);
	APP_LOG(LOG_LEVEL_INFO) << "UnSub";
}

void WRClient::CopyFilesInUpdateFolder(){
	
}

void WRClient::CheckVersion(){
	if (!is_login_) return;
	VersionRequest verReq(version_);
	client_->Send((char*)&verReq, sizeof(VersionRequest));
	APP_LOG(LOG_LEVEL_INFO) << "CheckVersion";
}

void WRClient::DownloadVersion(){
	if (!is_login_) return;
	VersionRequest verReq(WANT_TO_DOWNLOAD);
	client_->Send((char*)&verReq, sizeof(VersionRequest));
	APP_LOG(LOG_LEVEL_INFO) << "DownloadVersion";
}

void WRClient::GetSymbols(){
	if (!is_login_) return;
	SymbolsReq symsReq;
	client_->Send((char*)&symsReq, sizeof(SymbolsReq));
	APP_LOG(LOG_LEVEL_INFO) << "GetSymbols";
}

void WRClient::Testing(){
	/*if (!client_->StartUp()){
		assert(false);
	}*/

	while (1){
		baselib::Thread::Sleep(300);
		cout << "\n\ninput test case : 1.login  2.write  3.quote  4.sub  5.unsub  6.get symbols  other:quit\n";
		int test_case;
		cin >> test_case;
		switch(test_case){
		case 1:
			{
				  Login("admin", "123456");
				
			}
			break;
		case 2:
			{				
				  TestTimerWrite();
			}
			break;
		case 3:
			{
				  cout << "input symbol's instrument\n";
				  char	ProductId[31];
				  cout << "instrument:";
				  cin >> ProductId;
				  CSymbol sym("", ProductId, 100);
				  Quote(sym);

			}
			break;
		case 4:
			{
				cout << "input symbol list\n";
				string sym_list; cin >> sym_list;
				std::vector<std::string> inst_vec;
				Split(sym_list, ",", &inst_vec);

				std::vector<CSymbol> sym_vec;
				for (int i = 0; i < inst_vec.size(); ++i){
					sym_vec.push_back(CSymbol("", inst_vec[i].c_str(),100));
				}
				Sub(sym_vec);
			}
			break;
		case 5:
			{
				cout << "input symbol list\n";
				string sym_list; cin >> sym_list;
				std::vector<std::string> inst_vec;
				Split(sym_list, ",", &inst_vec);

				std::vector<CSymbol> sym_vec;
				for (int i = 0; i < inst_vec.size(); ++i){
					sym_vec.push_back(CSymbol("", inst_vec[i].c_str(), 100));
				}
				UnSub(sym_vec);
			}
			break;
		case 6:
			{
				  GetSymbols();
			}
			break;
		default:
			return;
		}
	}
	
}


void WRClient::SockReConn(){
	is_login_ = true;
	emit SignalNetErr(QStringLiteral("  网络重新连上"));
	APP_LOG(LOG_LEVEL_INFO) << "SockReConn";
}

void WRClient::SockConn(bool success){
	if (success) {
		emit SignalNetErr(QStringLiteral("网络连上"));
		LoginRequest loginReq;
		memcpy(loginReq.user_id, id_, sizeof(loginReq.user_id));
		strcpy(loginReq.pwd, pwd_);
		client_->Send((char*)&loginReq, sizeof(LoginRequest));
	}
	else
	{
		emit SignalNetErr(QStringLiteral("网络断了"));
	}
}
//网络断开
void WRClient::SockDisconn(){
	is_login_ = false;
	emit SignalNetErr(QStringLiteral("  网络断了"));
	APP_LOG(LOG_LEVEL_INFO) << "OnDisconnect";
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
				emit SignalLogin(QStringLiteral("登陆成功"));
				APP_LOG(LOG_LEVEL_INFO)<<"login success";
			}
			else {
				QString errMsg = login_rsp->msg;
				if (errMsg == "0")
					emit SignalLogin(QStringLiteral("账号不存在"));
				else
					emit SignalLogin(QStringLiteral("密码错误"));
				APP_LOG(LOG_LEVEL_INFO) << "login fail";
			}			
		}
		break;
	case RSP_VERSION:
		{
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
		}
		break;
	case RSP_QUOTE_DATA:
		{
			DataRsp *data = (DataRsp*)buf;

			CQuotation *tick = new CQuotation(data->data);
			emit SignalQuote(tick);

			stringstream ss; ss << data->data.symbol.Str() << " "
				<< data->data.m_SystemDate << " " << data->data.m_SystemTime
				<< " " << data->data.m_AlphaLastPrice;
			APP_LOG(LOG_LEVEL_INFO)<<"RSP_QUOTE_DATA"<<ss.str();
		}
		break;
	case RSP_SUB_DATA:
		{
			DataRsp *data = (DataRsp*)buf;
			CQuotation *tick = new CQuotation(data->data);
			emit SignalTickRsp(tick);
			//APP_LOG(LOG_LEVEL_INFO) << "RSP_SUB_DATA";
		}
		break;
	case RSP_SYMBOLS:
		{
			SymbolsRsp *sym_rsp = (SymbolsRsp*)buf;
			CSymbol *syms = new CSymbol[sym_rsp->num];
			stringstream ss;
			for (int i = 0; i < sym_rsp->num; ++i) {
				syms[i] = sym_rsp->sym[i];
				ss << syms[i].Str() << " ";
			}
			//cout << ss.str() + "\n";
			APP_LOG(LOG_LEVEL_INFO)<<"RSP_SYMBOLS"<<ss.str();
			emit SignalSymbols(syms, sym_rsp->num);
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