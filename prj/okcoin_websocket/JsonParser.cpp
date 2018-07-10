#include "JsonParser.h"
#include <mutex>
#include <QtCore/QDateTime>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QVariant>
#include <iostream>
using namespace std;

bool Json2Ticker(const QJsonObject &data, DrTickData &ticker){
	ticker.last = data.value("last").toDouble();
	ticker.vol = data.value("vol").toDouble();
	ticker.buy = data.value("buy").toDouble();
	ticker.sell = data.value("sell").toDouble();
	ticker.open = data.value("open").toDouble();
	ticker.high = data.value("high").toDouble();
	ticker.low = data.value("low").toDouble();
	ticker.close = data.value("close").toDouble();
	ticker.dayHigh = data.value("dayHigh").toDouble();
	ticker.dayLow = data.value("dayLow").toDouble();
	ticker.change = data.value("change").toDouble();
	ticker.timestamp = (long long)data.value("timestamp").toDouble();
	QDateTime datetime = QDateTime::fromMSecsSinceEpoch(ticker.timestamp);
	strcpy_s(ticker.datetime, sizeof(String32), datetime.toString("yyyyMMdd hh:mm:ss.zzz").toStdString().c_str());
	return true;
}

bool Json2Depth(const QJsonObject &data, DrTickData &depth){
	static const int OK_SUB_DEPTH = 20;
	QVariant asks = data.value("asks").toVariant();
	QVariant bids = data.value("bids").toVariant();
	static const int IDX_PRI = 0, IDX_VOL = 1;
	for (int i = 0; i < DEPTH; ++i){
		depth.askPrice[i] = asks.toList()[OK_SUB_DEPTH -1- i].toList()[IDX_PRI].toDouble();
		depth.askVol[i] = asks.toList()[OK_SUB_DEPTH -1- i].toList()[IDX_VOL].toDouble();
		depth.bidPrice[i] = bids.toList()[i].toList()[IDX_PRI].toDouble();
		depth.bidVol[i] = bids.toList()[i].toList()[IDX_VOL].toDouble();
	}
	depth.timestamp = (long long)data.value("timestamp").toDouble();
	QDateTime datetime = QDateTime::fromMSecsSinceEpoch(depth.timestamp);
	strcpy_s(depth.datetime, sizeof(String32), datetime.toString("yyyyMMdd hh:mm:ss.zzz").toStdString().c_str());
	return true;
}

static KlineBuffer kline_buffer;

void Json2Kline(const QJsonValue &data, String32 &symbol){
	QVariant kline_array = data.toVariant();
	QVariantList kline_list = kline_array.toList();
	for (int i = 0; i < kline_list.size(); ++i){
		DrKlineData kline;
		strcpy_s(kline.symbol, symbol);
		QVariantList d = kline_list[i].toList();
		kline.timestamp = (long long)d[0].toDouble();
		QDateTime datetime = QDateTime::fromMSecsSinceEpoch(kline.timestamp);
		strcpy_s(kline.datetime, sizeof(String32), datetime.toString("yyyyMMdd hh:mm:ss.zzz").toStdString().c_str());
		kline.open = d[1].toDouble();
		kline.high = d[2].toDouble();
		kline.low = d[3].toDouble();
		kline.close = d[4].toDouble();
		kline.vol = d[5].toDouble();
		kline_buffer.Push(kline);
	}
}

void addChannel(const QJsonObject &data) {
	bool result = data.value("result").toBool();
	QString channel = data.value("channel").toString();
	if (!result){
		g_err_ofs << channel.toStdString() << " fail\n";
	}
	else {
		g_log_ofs << channel.toStdString() << " sucess\n";
	}
}

// 只有在得到新的ticker后才return true
void Json2DrData(const char *json){
	QJsonParseError jsonError;
	QJsonDocument doucment = QJsonDocument::fromJson(json, &jsonError);
	if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError)) {
		QJsonArray array = doucment.array();
		QJsonObject obj = array.at(0).toObject();
		QJsonObject data = obj.value("data").toObject();
		if (data.contains("result")){
			if (!data.value("result").toBool())
				return;
		}
		QJsonValue val = obj.value("channel");
		QString channel = val.toString();
		if (channel == "addChannel") {
			addChannel(data);
			return;
		}
		else {
			static DrTickData g_ticks[SYMBOL_NUM];
			//static DrKlineData g_klines[SYMBOL_NUM];
			static TickBuffer tick_buffer;
			static std::once_flag flag;
			std::call_once(flag, []{
				strcpy_s(g_ticks[INDEX_BTC].symbol, sizeof(String32), SYMBOL_BTC);
				strcpy_s(g_ticks[INDEX_LTC].symbol, sizeof(String32), SYMBOL_LTC);
				strcpy_s(g_ticks[INDEX_ETH].symbol, sizeof(String32), SYMBOL_ETH);
				strcpy_s(g_ticks[INDEX_ETC].symbol, sizeof(String32), SYMBOL_ETC);
				strcpy_s(g_ticks[INDEX_BCC].symbol, sizeof(String32), SYMBOL_BCC);
				g_db_case.connDB();
				g_db_case.createTables();
				tick_buffer.Start();
				kline_buffer.Start();
			});

			if (channel.contains("ticker") || channel.contains("depth")){
				DrTickData *tick;
				if (channel.contains(SYMBOL_BTC))
					tick = &g_ticks[INDEX_BTC];
				else if (channel.contains(SYMBOL_LTC))
					tick = &g_ticks[INDEX_LTC];
				else if (channel.contains(SYMBOL_ETH))
					tick = &g_ticks[INDEX_ETH];
				else if (channel.contains(SYMBOL_ETC))
					tick = &g_ticks[INDEX_ETC];
				else if (channel.contains(SYMBOL_BCC))
					tick = &g_ticks[INDEX_BCC];
				else
					return;

				DrTickData this_tick = *tick;
				if ((channel.contains("ticker") && Json2Ticker(data, this_tick)) ||
					(channel.contains("depth") && Json2Depth(data, this_tick)))
				{
					if (this_tick == *tick) {
						//cout << "same tick\n";
					}
					else {
						*tick = this_tick;
						tick_buffer.Push(this_tick);
					}
				}
				else {
					return;
				}
			}
			else if (channel.contains("kline")){
				String32 symbol = { 0 };
				if (channel.contains(SYMBOL_BTC))
					strcpy_s(symbol, SYMBOL_BTC);
				else if (channel.contains(SYMBOL_LTC))
					strcpy_s(symbol, SYMBOL_LTC);
				else if (channel.contains(SYMBOL_ETH))
					strcpy_s(symbol, SYMBOL_ETH);
				else if (channel.contains(SYMBOL_ETC))
					strcpy_s(symbol, SYMBOL_ETC);
				else if (channel.contains(SYMBOL_BCC))
					strcpy_s(symbol, SYMBOL_BCC);
				else
					return;
				Json2Kline(obj.value("data"), symbol);
			}
		}	
	}
}