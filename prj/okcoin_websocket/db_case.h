#pragma once

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/qsqlerror.h>
#include <QtCore/QVariantList>
#include <QtCore/QDebug>
#include <QtCore/QVector>
#include "selfdatastruct.h"



class DbCase
{
public:
	DbCase();
	~DbCase();

	bool connDB();

	bool createTables();

	void insertTick(DrTickData &);
	void insertKline(DrKlineData &);

private:
	QSqlDatabase db;//
	QSqlDatabase db_prd;//Éú²ú.

	QVariantList ticks_[SYMBOL_NUM][DrTickDataFields];
	QVariantList klines_[SYMBOL_NUM][DrKlineDataFields];
};

