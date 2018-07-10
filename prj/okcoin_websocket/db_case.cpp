#include "db_case.h"
#include <QtCore/QFile>
#include <QtCore/QDateTime>
#include <QtCore/QCoreApplication>
#include <QtCore/QMutex>
#include "baselib/SimpleMath.h"
#include "baselib/Global.h"

#ifdef WIN32
#include <float.h>

#define is_nan(x) _isnan(x)

#ifndef NAN
static const unsigned long __nan[2] = { 0xffffffff, 0x7fffffff };
#define NAN (*(const double *) __nan)
#endif
#endif



static QString Table_tick_format =
"create table if not exists tick_%1("
"symbol varchar(32), exchange_ varchar(32),"
"last double, vol double, buy double, sell double,"
"open double, high double, low double, close_ double,"
"dayHigh double, dayLow double, change_ double,"
"timestamp_ bigint, datetime_ varchar(32),"
"bidPrice1 double, bidVol1 double,"
"bidPrice2 double, bidVol2 double,"
"bidPrice3 double, bidVol3 double,"
"bidPrice4 double, bidVol4 double,"
"bidPrice5 double, bidVol5 double,"
"askPrice1 double, askVol1 double,"
"askPrice2 double, askVol2 double,"
"askPrice3 double, askVol3 double,"
"askPrice4 double, askVol4 double,"
"askPrice5 double, askVol5 double,"
"primary key(symbol, exchange_, timestamp_));";

static QString Table_kline_format =
"create table if not exists kline_%1("
"symbol varchar(32), exchange_ varchar(32),"
"timestamp_ bigint, datetime_ varchar(32),"
"open double, high double, low double, close_ double,"
"vol double, primary key(symbol, exchange_, timestamp_));";

static QString g_tick_sql = "replace into tick_%1 values("
"?, ?, "
"?, ?, ?, ?,"
"?, ?, ?, ?,"
"?, ?, ?,"
"?, ?,"
"?, ?, ?, ?, ?, ?, ?, ?, ?, ?,"
"?, ?, ?, ?, ?, ?, ?, ?, ?, ?"
");";

static QString g_kline_sql = "replace into kline_%1 values("
"?, ?, "
"?, ?, "
"?, ?, ?, ?,"
"? );";

DbCase::DbCase()
{
//	s_industry_id_name.insert(1, QStringLiteral("煤炭"));

}


DbCase::~DbCase()
{
}

#define DATETIMEFORMAT1 "yyyy-MM-dd hh:mm:ss"
#define DATETIMEFORMAT2 "yyyy/MM/dd h:mm"

#define PREPARE_QVariantList(field) \
if (lidianUnEqual(node.field, 0.)) \
	field << node.field; \
else \
	field << QVariant(QVariant::Double);

static QMutex g_db_mutex;

void DbCase::insertKline(DrKlineData &kline){
	g_db_mutex.lock();
	QSqlQuery query;
	query.prepare(g_kline_sql.arg(kline.symbol));
	query.bindValue(0, kline.symbol);
	query.bindValue(1, kline.exchange);
	query.bindValue(2, kline.timestamp);
	query.bindValue(3, kline.datetime);
	query.bindValue(4, kline.open);
	query.bindValue(5, kline.high);
	query.bindValue(6, kline.low);
	query.bindValue(7, kline.close);
	query.bindValue(8, kline.vol);
	query.exec();
	g_db_mutex.unlock();
}

void DbCase::insertTick(DrTickData &tick){
	QString channel = tick.symbol;
	QVariantList *pTick;
	//qDebug() << channel;
	if (channel == ("btc"))
		pTick = ticks_[INDEX_BTC];
	else if (channel == ("ltc"))
		pTick = ticks_[INDEX_LTC];
	else if (channel == ("eth"))
		pTick = ticks_[INDEX_ETH];
	else if (channel == ("etc"))
		pTick = ticks_[INDEX_ETC];
	else if (channel == ("bcc"))
		pTick = ticks_[INDEX_BCC];
	else
		return;


	pTick[0] << tick.symbol; pTick[1] << tick.exchange;
	pTick[2] << tick.last;	pTick[3] << tick.vol; pTick[4] << tick.buy;	pTick[5] << tick.sell;
	pTick[6] << tick.open;	pTick[7] << tick.high; pTick[8] << tick.low;	pTick[9] << tick.close;
	pTick[10] << tick.dayHigh;	pTick[11] << tick.dayLow; pTick[12] << tick.change;	
	pTick[13] << tick.timestamp;	pTick[14] << tick.datetime;
	for (int i = 0; i < 5; ++i){
		pTick[15 + i * 2] << tick.bidPrice[i];
		pTick[16 + i * 2] << tick.bidVol[i];
		pTick[25 + i * 2] << tick.askPrice[i];
		pTick[26 + i * 2] << tick.askVol[i];
	}

	g_db_mutex.lock();
	if (pTick[0].size() > 100){
		QSqlQuery query;
		query.prepare(g_tick_sql.arg(tick.symbol));
		for (int i = 0; i < DrTickDataFields; ++i){
			query.addBindValue(pTick[i]);
			pTick[i].clear();
		}
		
		if (!query.execBatch()) {
			qDebug() << query.lastError();
			return;
		}
	}
	g_db_mutex.unlock();
}

bool DbCase::connDB()
{
	QCoreApplication::addLibraryPath("E:\\kiiik\\bin\\plugins");
	qDebug() << QSqlDatabase::drivers();
	qDebug() << QCoreApplication::libraryPaths();

	db = QSqlDatabase::addDatabase("QMYSQL");//添加驱动.
	db.setHostName("10.91.14.154"); //设置主机名.

	db.setUserName("fundpre_tu_tst"); //设置用户名.
	db.setPassword("fundpre_tu_tst_d3b934"); //设置用户密码.

	db.setDatabaseName("tech_fund_predict_00"); //设置数据库名.
	//发送连接.
	if (!db.open())
	{
		qDebug() << db.lastError();
		return false;
	}

	/*db_prd = QSqlDatabase::addDatabase("QMYSQL");//添加驱动.
	db_prd.setHostName("10.91.26.163"); //设置主机名.

	db_prd.setUserName("fundpre_tu_prd"); //设置用户名.
	db_prd.setPassword("fundpre_tu_prd_f00b5c"); //设置用户密码.

	db_prd.setDatabaseName("tech_fund_predict_00"); //设置数据库名.

	//发送连接.
	if (!db_prd.open())
	{
		qDebug() << db_prd.lastError();
		return false;
	}*/
	return true;
}

bool DbCase::createTables(){
	QSqlQuery query;
	if (!query.exec(Table_tick_format.arg(SYMBOL_BTC))){
		qDebug() << db.lastError();
		return false;
	}
	if (!query.exec(Table_tick_format.arg(SYMBOL_LTC))){
		qDebug() << db.lastError();
		return false;
	}
	if (!query.exec(Table_tick_format.arg(SYMBOL_ETH))){
		qDebug() << db.lastError();
		return false;
	}
	if (!query.exec(Table_tick_format.arg(SYMBOL_ETC))){
		qDebug() << db.lastError();
		return false;
	}
	if (!query.exec(Table_tick_format.arg(SYMBOL_BCC))){
		qDebug() << db.lastError();
		return false;
	}
	//kline
	if (!query.exec(Table_kline_format.arg(SYMBOL_BTC))){
		qDebug() << db.lastError();
		return false;
	}
	if (!query.exec(Table_kline_format.arg(SYMBOL_LTC))){
		qDebug() << db.lastError();
		return false;
	}
	if (!query.exec(Table_kline_format.arg(SYMBOL_ETH))){
		qDebug() << db.lastError();
		return false;
	}
	if (!query.exec(Table_kline_format.arg(SYMBOL_ETC))){
		qDebug() << db.lastError();
		return false;
	}
	if (!query.exec(Table_kline_format.arg(SYMBOL_BCC))){
		qDebug() << db.lastError();
		return false;
	}

	return true;
}