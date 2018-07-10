#include "wtpmduserapi/WtpUserApiStruct.h"

#include <string>
#include <sstream>
#include <map>
#include <iomanip>
#include <iostream>

using namespace std;

struct CSymbol : public CWtpSymbolField
{
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
		return (string)m_ExchangeId + "_" + m_ProductId + "_" + day;
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
			(strcmp(m_ExchangeId, other.m_ExchangeId) == 0 && strcmp(m_ProductId, other.m_ProductId) == 0 && m_ContractDays > other.m_ContractDays);
	}
	bool operator<=(const CSymbol& other) const { return !(*this > other); }
	bool operator>=(const CSymbol& other) const { return !(*this < other); }
};

struct CQuotation : public CWtpQuotationField {
	CQuotation() { memset(this, 0, sizeof(CQuotation)); }
};