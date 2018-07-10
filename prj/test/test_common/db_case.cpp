#include "db_case.h"
#include <QtCore/QFile>
#include <QtCore/QDateTime>
#include "baselib/SimpleMath.h"

#ifdef WIN32
#include <float.h>

#define is_nan(x) _isnan(x)

#ifndef NAN
static const unsigned long __nan[2] = { 0xffffffff, 0x7fffffff };
#define NAN (*(const double *) __nan)
#endif
#endif

static const char *Table_industry_fundamentals = // 行业基本面.
	"drop table industry_fundamentals;"
	"create table industry_fundamentals("
	"industry_id int, industry_name varchar(31), yearmonth varchar(8),"
	"commodity_index_actual double(20,2), commodity_index_predict double(20,2),"
	"PPI1_actual double(20,2), PPI1_predict double(20,2), PPI2_actual double(20,2), PPI2_predict double(20,2),"
	"yield_actual double(20,2), yield_predict double(20,2), YoY_growth_of_yield_actual double(20,2), YoY_growth_of_yield_predict double(20,2),"
	"gross_profit_rate_actual double(20,2), gross_profit_rate_predict double(20,2), YoY_growth_of_gross_profit_rate_actual double(20,2), YoY_growth_of_gross_profit_rate_predict double(20,2),"	
	"YoY_growth_of_profit_actual double, YoY_growth_of_profit_predict double,"
	"primary key(industry_id, yearmonth)); ";

static const char *Table_corporate_finance = // 公司财务.
	"create table if not exists corporate_finance("
	"corporate_name varchar(31), corporate_code varchar(31), particular_year varchar(8), quarter int, index_name varchar(128),"
	"mean_value_predict double(20,2), mean_absolute_value_deviation double(20,4), value_actual double(20,2), value_predict double(20,2), absolute_value_deviation double(20,4),"
	"primary key(corporate_code, particular_year, quarter, index_name));";

DbCase::DbCase()
{
	s_industry_id_name.insert(1, QStringLiteral("煤炭"));
	s_industry_id_name.insert(2, QStringLiteral("钢铁"));
	s_industry_id_name.insert(3, QStringLiteral("电力"));
	s_industry_id_name.insert(4, QStringLiteral("有色金属"));
	s_industry_id_name.insert(5, QStringLiteral("农林牧渔"));
	s_industry_id_name.insert(6, QStringLiteral("化工"));
}


DbCase::~DbCase()
{
}

#define DATETIMEFORMAT1 "yyyy-MM-dd hh:mm:ss"
#define DATETIMEFORMAT2 "yyyy/MM/dd h:mm"
industry_fundamentals::industry_fundamentals(QString &str) {
	memset(this, 0, sizeof(industry_fundamentals));
	QStringList str_list = str.split(",");
	if (str_list[0] == "")return;
	QDateTime date_time = QDateTime::fromString(str_list[0], DATETIMEFORMAT2);
	strcpy(yearmonth, date_time.toString("yyyyMM").toStdString().c_str());
	bool ok = false;
	YoY_growth_of_profit_actual = str_list[1].toDouble(&ok);
	YoY_growth_of_profit_predict = str_list[2].toDouble(&ok);
	gross_profit_rate_actual = str_list[3].toDouble(&ok);
	gross_profit_rate_predict = str_list[4].toDouble(&ok);
	PPI1_actual = str_list[5].toDouble(&ok);
	PPI1_predict = str_list[6].toDouble();
	PPI2_actual = str_list[7].toDouble();
	PPI2_predict = str_list[8].toDouble();
	commodity_index_actual = str_list[9].toDouble();
	commodity_index_predict = str_list[10].toDouble();
	YoY_growth_of_gross_profit_rate_actual = str_list[11].toDouble();
	YoY_growth_of_gross_profit_rate_predict = str_list[12].toDouble();
	yield_actual = str_list[13].toDouble();
	yield_predict = str_list[14].toDouble();
	YoY_growth_of_yield_actual = str_list[15].toDouble();
	YoY_growth_of_yield_predict = str_list[16].toDouble();

	/*memset(this, 0, sizeof(industry_fundamentals));
	if (str[0] == ',')return;
	char year[4] = { 0 }, month[2] = {0};
	char tmp[30];
	sscanf(str,
		"%[^-]-%[^-]%[^,],"
		"%lf,%lf,%lf,%lf,"
		"%lf,%lf,%lf,%lf,"
		"%lf,%lf,%lf,%lf,"
		"%lf,%lf,%lf,%lf",
		year, month,tmp,
		&YoY_growth_of_profit_actual, &YoY_growth_of_profit_predict,
		&gross_profit_rate_actual, &gross_profit_rate_predict,
		&PPI1_actual, &PPI1_predict, &PPI2_actual, &PPI2_predict,
		&commodity_index_actual, &commodity_index_predict,
		&YoY_growth_of_gross_profit_rate_actual, &YoY_growth_of_gross_profit_rate_predict,
		&yield_actual, &yield_predict, &YoY_growth_of_yield_actual, &YoY_growth_of_yield_predict);
	
	char *p = yearmonth;
	strcpy(p, year);
	p += 4;
	strcpy(p, month);*/
}

#define PREPARE_QVariantList(field) \
if (baselib::UnEqual(node.field, 0.)) \
	field << node.field; \
else \
	field << QVariant(QVariant::Double);


void DbCase::insert_industry_fundamentals(const char *path, int id){
	QSqlQuery query;
	query.prepare("replace into industry_fundamentals values("
		"?, ?, ?,"
		"?, ?, ?, ?,"
		"?, ?, ?, ?,"
		"?, ?, ?, ?,"
		"?, ?, ?, ? );");
	QVariantList industry_id, industry_name, yearmonth,
		commodity_index_actual, commodity_index_predict,
		PPI1_actual, PPI1_predict, PPI2_actual, PPI2_predict,
		yield_actual, yield_predict, YoY_growth_of_yield_actual, YoY_growth_of_yield_predict,
		gross_profit_rate_actual, gross_profit_rate_predict, YoY_growth_of_gross_profit_rate_actual, YoY_growth_of_gross_profit_rate_predict,
		YoY_growth_of_profit_actual, YoY_growth_of_profit_predict;

	QFile industry_csv(path);
	if (industry_csv.open(QIODevice::ReadOnly)) {
		QTextStream stream(&industry_csv);
		while (!stream.atEnd()) {
			QString line = stream.readLine();
			industry_fundamentals node(line);
			if (strcmp(node.yearmonth, "")){
				industry_id << id;
				industry_name << s_industry_id_name[id];
				yearmonth << node.yearmonth;

				/*if (baselib::UnEqual(node.commodity_index_actual, 0.))
					commodity_index_actual << node.commodity_index_actual;
				else
					commodity_index_actual << QVariant(QVariant::Double);*/
				
				PREPARE_QVariantList(commodity_index_actual)					
				PREPARE_QVariantList(commodity_index_predict)
				PREPARE_QVariantList(PPI1_actual)
				PREPARE_QVariantList(PPI1_predict)
				PREPARE_QVariantList(PPI2_actual)
				PREPARE_QVariantList(PPI2_predict)
				PREPARE_QVariantList(yield_actual)
				PREPARE_QVariantList(yield_predict)
				PREPARE_QVariantList(YoY_growth_of_yield_actual)
				PREPARE_QVariantList(YoY_growth_of_yield_predict)
				PREPARE_QVariantList(gross_profit_rate_actual)
				PREPARE_QVariantList(gross_profit_rate_predict)
				PREPARE_QVariantList(YoY_growth_of_gross_profit_rate_actual)
				PREPARE_QVariantList(YoY_growth_of_gross_profit_rate_predict)
				PREPARE_QVariantList(YoY_growth_of_profit_actual)
				PREPARE_QVariantList(YoY_growth_of_profit_predict)
					
				/*commodity_index_predict << node.commodity_index_predict;
				PPI1_actual << node.PPI1_actual;
				PPI1_predict << node.PPI1_predict;
				PPI2_actual << node.PPI2_actual;
				PPI2_predict << node.PPI2_predict;
				yield_actual << node.yield_actual;
				yield_predict << node.yield_predict;
				YoY_growth_of_yield_actual << node.YoY_growth_of_yield_actual;
				YoY_growth_of_yield_predict << node.YoY_growth_of_yield_predict;
				gross_profit_rate_actual << node.gross_profit_rate_actual;
				gross_profit_rate_predict << node.gross_profit_rate_predict;
				YoY_growth_of_gross_profit_rate_actual << node.YoY_growth_of_gross_profit_rate_actual;
				YoY_growth_of_gross_profit_rate_predict << node.YoY_growth_of_gross_profit_rate_predict;
				YoY_growth_of_profit_actual << node.YoY_growth_of_profit_actual;
				YoY_growth_of_profit_predict << node.YoY_growth_of_profit_predict;*/
			}
		}
	}

	query.addBindValue(industry_id);
	query.addBindValue(industry_name);
	query.addBindValue(yearmonth);
	query.addBindValue(commodity_index_actual);
	query.addBindValue(commodity_index_predict);
	query.addBindValue(PPI1_actual);
	query.addBindValue(PPI1_predict);
	query.addBindValue(PPI2_actual);
	query.addBindValue(PPI2_predict);
	query.addBindValue(yield_actual);
	query.addBindValue(yield_predict);
	query.addBindValue(YoY_growth_of_yield_actual);
	query.addBindValue(YoY_growth_of_yield_predict);
	query.addBindValue(gross_profit_rate_actual);
	query.addBindValue(gross_profit_rate_predict);
	query.addBindValue(YoY_growth_of_gross_profit_rate_actual);
	query.addBindValue(YoY_growth_of_gross_profit_rate_predict);
	query.addBindValue(YoY_growth_of_profit_actual);
	query.addBindValue(YoY_growth_of_profit_predict);

	if (!query.execBatch()) {
		qDebug() << query.lastError();
		return;
	}


	/*
	query.exec("SELECT name, age FROM student");
	while (query.next()) {
	QString name = query.value(0).toString();
	int age = query.value(1).toInt();
	qDebug() << name << ": " << age;
	}*/
}

void DbCase::insertBadDatas(){
	insert_industry_fundamentals("C:\\Users\\za-wudian\\Desktop\\tmp_wrok\\meitan_production2.csv", 1);
	
}

bool DbCase::connDB()
{
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
	if (!query.exec(Table_industry_fundamentals)){
		qDebug() << db.lastError();
		return false;
	}
	if (!query.exec(Table_corporate_finance)){
		qDebug() << db.lastError();
		return false;
	}

	return true;
}