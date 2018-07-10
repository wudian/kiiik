#pragma once

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/qsqlerror.h>
#include <QtCore/QVariantList>
#include <QtCore/QDebug>

struct industry_fundamentals 
{
	int industry_id;
	char industry_name[31];
	char yearmonth[8];
	double commodity_index_actual;
	double commodity_index_predict;
	double PPI1_actual; 
	double PPI1_predict;
	double PPI2_actual;
	double PPI2_predict ;
	double yield_actual ;
	double yield_predict ;
	double YoY_growth_of_yield_actual ;
	double YoY_growth_of_yield_predict ;
	double gross_profit_rate_actual ;
	double gross_profit_rate_predict ;
	double YoY_growth_of_gross_profit_rate_actual ;
	double YoY_growth_of_gross_profit_rate_predict ;
	double YoY_growth_of_profit_actual ;
	double YoY_growth_of_profit_predict ;

	industry_fundamentals(QString &);
};

class DbCase
{
public:
	DbCase();
	~DbCase();

	bool connDB();

	bool createTables();

	void insertSimulaDatas(){}

	void insertBadDatas();

private:
	QSqlDatabase db;//
	QSqlDatabase db_prd;//Éú²ú.
	QMap<int, QString> s_industry_id_name;

	void insert_industry_fundamentals(const char *path, int id);
};

