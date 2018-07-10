#include "LoginService.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp> 
#include <boost/foreach.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <string>
#include <set>
#include <vector>
#include <exception>
#include <iostream>
#include <fstream>
#include "Baselib/SimpleMath.h"

using namespace std;
using boost::property_tree::ptree;
namespace  fs = boost::filesystem;

static const string str_node_name[] = {"user","password","authority"};

namespace kiiik {


LoginService::LoginService()
{
	acc_cofig_file_ = KiiikHome() + "config\\LoginServer.xml";
	version_cfg_file_ = KiiikHome() + "config\\version.xml";
	update_folder_ = KiiikHome() + "update\\";

	readVersionXml();
}


LoginService::~LoginService(void)
{

	//if (account_config_) delete account_config_;
}

void LoginService::OnDiscon(network_asio::TcpSession *tcp_sock){
	map_session_authority_.erase(tcp_sock);
}

void LoginService::readVersionXml(){
	ptree version_root;
	try
	{
		read_xml(version_cfg_file_, version_root);
	}
	catch (std::exception& e)
	{
		return;
	}
	string str_version = version_root.get_child("version").data();
	version_ = atof(str_version.c_str()); // 客户端最新版本号
}

void LoginService::readLoginXml()
{	
	ptree root;
	try
	{
		read_xml(acc_cofig_file_, root);
	}
	catch (std::exception& e)
	{
		return;
	}

	acc_details_.clear();
	for (ptree::iterator itr = root.begin();itr!=root.end();itr++)
	{
		ptree account = itr->second;

		xmlAccDetail xml_acc_detail;
		for (int i = 0;i<3;i++)
		{
			ptree pt;
			string str_value;
			try
			{				
				pt = account.get_child(str_node_name[i]);
				str_value = pt.data();
			}
			catch (std::exception& e)
			{
				return ;
			}
			switch (i)
			{
			case 0:
				xml_acc_detail.user = str_value;
				break;
			case 1:
				xml_acc_detail.password = str_value;
				break;
			case 2:
			{
					  if (str_value[0] == 'a' || str_value[0] == 'A')
						  xml_acc_detail.authority = E_Admin;
					  else if (str_value[0] == 'w' || str_value[0] == 'W')
						  xml_acc_detail.authority = E_Write;
					  else
						  xml_acc_detail.authority = E_Read;//权限最低					  
			}
				break;
			}

		}
		acc_details_.push_back(xml_acc_detail);
	}
}
void LoginService::writexml(){
	ptree pt; 
	for (vector<xmlAccDetail>::iterator iter = acc_details_.begin(); iter!=acc_details_.end(); ++iter)
	{
		//pt.put<string>()
	}
}

bool LoginService::loginSuccess(UserIdType usr, PwdType pwd, string& msg, network_asio::TcpSession *_Sock){
	//string tmp = account_config_->stringValue((string)"/Account/"+usr);
	//return account_config_->stringValue((string)"/Account/"+usr) == pwd;
	for (vector<xmlAccDetail>::iterator iter = acc_details_.begin(); iter!=acc_details_.end(); ++iter){
		if (iter->user == usr) {
			if (iter->password == pwd){
				map_session_authority_[_Sock] = iter->authority;
				return true;
			}
			else
			{
				msg = "1";// "密码错误";
				return false;
			}
		}
	}
	msg = "0";// "账号不存在";
	return false;
}

bool LoginService::CanWrite(network_asio::TcpSession *_Sock){
	map<network_asio::TcpSession *, E_Authority>::iterator iter = map_session_authority_.find(_Sock);
	if (iter != map_session_authority_.end() && iter->second > E_Read)
		return true;
	else
		return false;
}

void LoginService::HandleReq( LoginRequest *_Req,  network_asio::TcpSession *_Sock){
	readLoginXml();

	LoginResponse login_rsp;
	string msg;
	if (!loginSuccess(_Req->user_id, _Req->pwd, msg, _Sock)) {
		login_rsp.succeed = false;
		strcpy(login_rsp.msg, msg.c_str());
	}		
	_Sock->Send((char*)&login_rsp, sizeof(LoginResponse));
}

void LoginService::HandleReq(VersionRequest *_Req, network_asio::TcpSession *_Sock){

	// 如果客户端的版本号小，则将新版客户端发过去
	if (_Req->version == WANT_TO_DOWNLOAD) {
		fs::path update_path(update_folder_.c_str());
		if (fs::exists(update_path))
		{
			fs::directory_iterator item_begin(update_path);
			fs::directory_iterator item_end;
			for (; item_begin != item_end; item_begin++)
			{
				string file_path = item_begin->path().string();
				string file_name = item_begin->path().filename().string();//文件名
				ifstream ifs(file_path, ios::binary);
				if (!ifs.bad()) {
					ifs.seekg(0, ios_base::end);
					int file_length = ifs.tellg();//文件长度
					ifs.seekg(0, ios_base::beg);

					int len = sizeof(VersionResponse)+file_length;
					VersionResponse *version_rsp = (VersionResponse*)malloc(len);
					memset(version_rsp, 0, len);
					version_rsp->type = RSP_VERSION;
					strcpy(version_rsp->file_name, file_name.c_str());
					version_rsp->file_length = file_length;
					ifs.read(version_rsp->content, file_length);//文件内容
					ifs.close();
					_Sock->Send((char*)version_rsp, len);
					free(version_rsp);
				}
			}
			VersionResponse version_rsp;
			memset(&version_rsp, 0, sizeof(VersionResponse));
			version_rsp.type = RSP_VERSION;
			version_rsp.file_length = UPDATE_END;//下载完毕
			_Sock->Send((char*)&version_rsp, sizeof(VersionResponse));
		}
		return;
	}

	if (UnEqual(version_, _Req->version)) {
		VersionResponse version_rsp;
		memset(&version_rsp, 0, sizeof(VersionResponse));
		version_rsp.type = RSP_VERSION;
		version_rsp.file_length = WANT_TO_DOWNLOAD;//想下载吗
		_Sock->Send((char*)&version_rsp, sizeof(VersionResponse));
	}
	else {
		// 目前是最新版本，不需要更新
		VersionResponse version_rsp;
		memset(&version_rsp, 0, sizeof(VersionResponse));
		version_rsp.type = RSP_VERSION;
		version_rsp.file_length = IS_NEW_VERSION;
		_Sock->Send((char*)&version_rsp, sizeof(VersionResponse));
	}
	
}

}