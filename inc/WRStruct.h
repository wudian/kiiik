#pragma once

#include <vector>
#include "MarketDefine.h"
#include "Baselib/DateTime.h"

namespace kiiik {


/*
交易所SHFE、DCE、CZCE，三到四个字符
品种CU、ZN、AL这种，一到两个字符
周期一到三位数字
--- 陈挺
*/
struct CSymbol
{
	char	m_ExchangeId[9];					/// 交易所代码，例如：'SHFE' / 'DCE' / 'CZCE'
	char	m_ProductId[31];					/// 品种代码，例如：'CU' / 'AL' / 'ZN'
	int		m_ContractDays;						/// 约定天数，0为贴现，正整数为回购

	CSymbol() { memset(this, 0, sizeof(CSymbol)); }
	CSymbol(const char *ExchangeId, const char *ProductId, int ContractDays){
		strcpy(m_ExchangeId, ExchangeId);
		strcpy(m_ProductId, ProductId);
		m_ContractDays = ContractDays;
	}
	std::string Str() const
	{
		char day[8] = { 0 };
		sprintf(day, "%d", m_ContractDays);
		return (string)m_ExchangeId + "_" + m_ProductId+"_"+day;
	}
	bool operator==(const CSymbol& other) const
	{
		return (strcmp(m_ExchangeId, other.m_ExchangeId) == 0) && (strcmp(m_ProductId, other.m_ProductId) == 0) && m_ContractDays == other.m_ContractDays;
	}
	bool operator!=(const CSymbol& other) const
	{
		return !(*this == other);
	}

	bool operator<(const CSymbol& other) const
	{
		return strcmp(m_ExchangeId, other.m_ExchangeId) < 0 || 
			(strcmp(m_ExchangeId, other.m_ExchangeId) == 0 && strcmp(m_ProductId, other.m_ProductId) < 0) ||
			(strcmp(m_ExchangeId, other.m_ExchangeId) == 0 && strcmp(m_ProductId, other.m_ProductId) == 0 && m_ContractDays<other.m_ContractDays);
	}

	bool operator>(const CSymbol& other) const
	{
		return strcmp(m_ExchangeId, other.m_ExchangeId) > 0 ||
			(strcmp(m_ExchangeId, other.m_ExchangeId) == 0 && strcmp(m_ProductId, other.m_ProductId) > 0) ||
			(strcmp(m_ExchangeId, other.m_ExchangeId) == 0 && strcmp(m_ProductId, other.m_ProductId) == 0 && m_ContractDays>other.m_ContractDays);
	}
	bool operator<=(const CSymbol& other) const { return !(*this > other); }
	bool operator>=(const CSymbol& other) const { return !(*this < other); }
};

// 仓单报价
struct CQuotation
{
	CSymbol symbol;
	///
	long	m_SystemDate;						/// 系统日期
	long	m_SystemTime;						/// 系统时间
	long	m_MinuteDate;						/// 分钟数据日期
	long	m_MinuteTime;						/// 分钟数据时间
	long	m_FinestDate;						/// 逐笔数据日期
	long	m_FinestTime;						/// 逐笔数据时间
	///
	long	m_ContractDate;						/// 逆回购日期
	long	m_DeadlineDate;						/// 灵活逆回购截止日期
	long	m_DeliveryDate;						/// 有效交割日期
	char	m_AlphaInstrumentId[31];			/// 第一期货合约
	char	m_BravoInstrumentId[31];			/// 第二期货合约
	double	m_AlphaLastPrice;					/// 第一期货最新价
	double	m_BravoLastPrice;					/// 第二期货最新价
	///
	double	m_DiscountRatio;					/// 贴现比率
	double	m_PosiRepoRatio;					/// 正回购比率
	double	m_ReveRepoRatio;					/// 到期逆回购比率
	double	m_FlexReveRepoRatio;				/// 灵活逆回购比率
	double	m_DiscountPrice;					/// 贴现价格
	double	m_PosiRepoPrice;					/// 正回购价格
	double	m_FlexReveRepoPrice;				/// 灵活逆回购价格

	CQuotation() { memset(this, 0, sizeof(CQuotation)); }
};


/*
数据库服务(DBaaS) ： 提供全面数据库功能。

数据服务(DaaS) ： 自己定义数据，通过API从internet访问数据，不执行sql。
*/

}