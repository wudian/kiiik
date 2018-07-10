#ifndef PROTOCOL_H
#define PROTOCOL_H


#include "wtpmduserapi/WtpUserApiStruct.h"


#define REQ_LOGIN '0'
#define REQ_QUOTE '1'// 询价 读数据
#define REQ_WRITE '2' // 写数据 
#define REQ_SUBSCRIBE '3'//订阅
#define REQ_UNSUB '4'//取消订阅
#define REQ_SYMBOLS '5'//查合约列表
#define REQ_VERSION '6' // 

#define RSP_LOGIN 'a'
#define RSP_SYMBOLS 'b'//返回合约列表
#define RSP_QUOTE_DATA 'c'//返回一个tick数据,即CWtpQuotationField
#define RSP_SUB_DATA 'd'
//#define RSP_SUBSCRIBE 'c'
//#define RSP_WRITE 'd'
#define RSP_VERSION 'e'

typedef char ClientRequestType;
typedef char UserIdType[20];
typedef char PwdType[20];

#pragma pack(1)
struct ProtocolHead 
{
	ClientRequestType type;
	ProtocolHead (ClientRequestType t = '0') : type(t) {}
};

struct QuoteReq : public ProtocolHead{
	CWtpSymbolField sym;
	QuoteReq():ProtocolHead(REQ_QUOTE){}
};
struct WtiteReq : public ProtocolHead{
	CWtpQuotationField data;
	WtiteReq():ProtocolHead(REQ_WRITE){}
};
struct SubsReq : public ProtocolHead{
	int num;
	CWtpSymbolField sym[0];
	SubsReq():ProtocolHead(REQ_SUBSCRIBE), num(0) {}
};
struct UnsubReq : public ProtocolHead{
	int num;
	CWtpSymbolField sym[0];
	UnsubReq():ProtocolHead(REQ_UNSUB), num(0) {}
};
struct SymbolsReq : public ProtocolHead{
	SymbolsReq():ProtocolHead(REQ_SYMBOLS) {}
};

struct DataRsp : public ProtocolHead {
	CWtpQuotationField data;
	DataRsp():ProtocolHead(RSP_SUB_DATA){}
};
struct SymbolsRsp : public ProtocolHead {
	int num;
	CWtpSymbolField sym[0]; // 返回合约列表
	SymbolsRsp() : ProtocolHead(RSP_SYMBOLS), num(0) {}
};


struct LoginRequest : public ProtocolHead {
	UserIdType user_id;
	PwdType pwd;
	LoginRequest() : ProtocolHead(REQ_LOGIN) {}
};
struct LoginResponse : public ProtocolHead {
	bool succeed;
	char msg[32]; // 0"账号不存在" \\ 1"密码错误";
	LoginResponse() : ProtocolHead(RSP_LOGIN), succeed(true) { memset(msg, 0, 32); }
};

#define IS_NEW_VERSION 0
#define WANT_TO_DOWNLOAD -1
#define UPDATE_END -2
struct VersionRequest : public ProtocolHead {
	float version; // 本地版本号
	VersionRequest(float v) : ProtocolHead(REQ_VERSION), version(v){}
};
struct VersionResponse : public ProtocolHead {
	char file_name[256];
	//字节数。0表示不需更新。>0表示在传文件。-1表示新文件传输完毕。
	int file_length;
	char content[0];
	VersionResponse() :ProtocolHead(RSP_VERSION){ }
};

#endif