#pragma once
#include <fstream>
#include "baselib/NonBlockQueueBuffer.h"
#include "db_case.h"


#include "selfdatastruct.h"

static std::ofstream g_err_ofs("error.txt"), g_log_ofs("log.txt");

static DbCase g_db_case;

void Json2DrData(const char *json);

class TickBuffer : public NonBlockQueueBuffer<DrTickData>
{
public:
	virtual void Consume(DrTickData& tick) {
		/*if (string(tick.symbol) == "btc"){
			
		}
		cout << tick.symbol << "," << tick.datetime
			<< "," << tick.last << "," << tick.vol
			<< "," << tick.bidPrice[0] << "," << tick.bidVol[0]
			<< endl;*/

		g_db_case.insertTick(tick);
	}

	
};

class KlineBuffer : public NonBlockQueueBuffer<DrKlineData>
{
public:
	virtual void Consume(DrKlineData& kline) {
		g_db_case.insertKline(kline);
	}

};
