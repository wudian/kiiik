/*命名规则：
1 所有数据结构均采用英文单词的小写
2 函数的命名采用驼峰式
3 类的命名采用英文单词小写
*/


#pragma once
#include "selfdatastruct.h"
#include <iostream>
#include <deque>
#include <vector>
#include "Trade.h"
#include "Quote.h"
#include "strategy.h"
#include <condition_variable>
#include <mutex>
#include <thread>
#pragma  comment(lib,"WinMM.Lib")
#pragma warning(disable : 4018)

#define BACKTEST 1

#ifdef _DEBUG
	#define SIMNOW 1
	#define LOG 1
#else
	#define SIMNOW 1
	#define LOG 0
#endif

using namespace std;
typedef vector<strategy*> pStr;
struct sqlite3;
/** 查询SQlite数据库的回调函数 */
typedef int(*fnOnQuerySQLite)(void*, int, char**, char**);

class common
{
public:
	/// 配置文件全局唯一实例
	static common& GetInstance();
	~common();
	bool Start(const char *szfile);
	bool IsMyOrder(orderkey key);
	bool IsMyTrade(tradekey key);
	bool AddStrategy(string ppinst,strategy *pstr);
	int  BeforeMktOpen();
	int  AfterMktClose();
	void Warning();
	void Strategy();
	void CtpFun();
	void CheckOrder2();
	void CreateThread();
	void ReleaseThread();
	void StartSqlite();
	void StopSqlite();
	void SendOrder(inputorder& porder);
	void SendOrder2(inputorder& porder);
	int  CheckOrder(string strname, signtype signal);
	void CancelOrder(order_t *order);
	void CalPos(CThostFtdcTradeField *pTrade,postype type);
	void CalFrozen(CThostFtdcOrderField& order,order_t *porder);

	int iNumber=0;//合约数目
	int dayofweek = 0;//星期几，防止周末发送乱数据
	double moneyavaible;//可用资金
	char* ppInst[MAX_CODE_SIZE];//合约数组
	bool qryflag = true;//查询控制变量


	//配置参数

	TThostFtdcInvestorIDType INVESTOR_ID;			// 投资者代码
	TThostFtdcBrokerIDType	BROKER_ID;				// 经纪公司代码
	TThostFtdcPasswordType  PASSWORD;			// 用户密码
	char tdfront[MAX_CODE_SIZE];                              // 交易前置
	char tdfront1[MAX_CODE_SIZE];                            // 交易前置
	char mdfront[MAX_CODE_SIZE];                         // 行情前置
	char mdfront1[MAX_CODE_SIZE];                            // 行情前置

	//订单和持仓队列

	vector<CThostFtdcDepthMarketDataField> mktdata; //行情队列
	deque<TR>trseq; //订单和成交队列
	map<string, CThostFtdcDepthMarketDataField>LatestMarket;//最新行情
	map<string, pStr>pStrmap;//合约与其关联的策略对象指针vector构成的map
	map<StrategyID, strategy*>pStrID;//策略ID与策略对象指针构成的map
	orderinfo_map orderinfo; //按订单唯一标识记录的订单map
	tradeinfo_map tradeinfo; //按每笔成交唯一标识记录的成交map
	orderinfo_map orderbook; //只保存正在交易的订单的指针,也即在委托队列中未成交的订单
	strpos_map strpos; //按策略分类记录的持仓map
	instpos_map instpos; //按品种分类记录的持仓map
	cfg_map cfgfile;//配置文件
	trade2order_map tmapo; //trade key到order key的映射.
	tradeid2strategyid_map map_tradeid_straid_; //trade key到strategy id的映射 用于分析隔夜仓.
	open_permit openpermit;//合约能否开仓
	map<string, char>inststatus;//合约交易状态
	std::condition_variable c_strategy, c_ctp, c_warn, c_checkorder,c_main;
	std::mutex m_strategy, m_ctp, m_warn, m_checkorder,m_main;
	bool ctpflag = false, strflag = false, warnflag = false, checkflag = false;
	std::thread T_ctp, T_str, T_warn, T_checkorder;
	CRITICAL_SECTION cs_strategy;

	// 会话参数

	TThostFtdcFrontIDType	FRONT_ID;	//前置编号
	TThostFtdcSessionIDType	SESSION_ID;	//会话编号
	TThostFtdcOrderRefType	ORDER_REF;	//报单引用
	TThostFtdcDirectionType Direction;  //交易方向
	int nReq=0;                         //请求会话编号
	typedef pair<string, string>Entry;
	CThostFtdcMdApi* md;
	CThostFtdcTraderApi* api;
	Trade *t;//交易类回调
	Quote *q;//行情类回调

	sqlite3 *sqlite3_;
	void SQLite_TradekeyStrid_Callback(int num, char **values, char **colName);
private:

	bool SQLite_Query_TradekeyStrid();
	bool SQLite_Insert_StraPos(const poskey&, position_t&);
	bool SQLite_Insert_TradekeyStrid(TThostFtdcTradeIDType &, StrategyID &);
	bool _SQLite_EXEC(const char *SQL, fnOnQuerySQLite cb = nullptr, void *p = nullptr);

	bool loaded;

	common() :loaded(false) { }
};