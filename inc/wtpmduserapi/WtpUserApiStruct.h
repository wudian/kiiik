/////////////////////////////////////////////////////////////////////////
///@system 仓单综合交易所系统
///@company 东航金控有限责任公司
///@file WtpUserApiStruct.h
///@brief 定义了客户端接口使用的业务数据结构
///@history 
///20161122	陈挺、吴典		创建该文件
/////////////////////////////////////////////////////////////////////////

#if !defined(WTP_STRUCT_H)
#define WTP_STRUCT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "wtpmduserapi/WtpUserApiDataType.h"

#if defined(WIN32)
#ifdef LIB_MD_API_EXPORT
#define MD_API_EXPORT __declspec(dllexport)
#else
#define MD_API_EXPORT __declspec(dllimport)
#endif
#else
#define MD_API_EXPORT 
#endif

/// 仓单交易合约
struct CWtpSymbolField
{
	/// 交易所代码，例如：'SHFE' / 'DCE' / 'CZCE'
	TWtpExchangeIdType		m_ExchangeId;
	/// 交易品种代码，例如：'CU' / 'AL' / 'ZN'
	TWtpProductIdType		m_ProductId;
	/// 贴现/回购周期天数	，0为贴现，正整数为回购
	TWtpDaysType			m_ContractDays;
};

/// 仓单报价
struct CWtpQuotationField
{
	/// 仓单交易合约
	CWtpSymbolField			m_Symbol;
	/// 系统日期
	TWtpDateType			m_SystemDate;
	/// 系统时间						
	TWtpTimeType			m_SystemTime;
	/// 分钟数据日期						
	TWtpDateType			m_MinuteDate;
	/// 分钟数据时间						
	TWtpTimeType			m_MinuteTime;
	/// 逐笔数据日期						
	TWtpDateType			m_FinestDate;
	/// 逐笔数据时间						
	TWtpTimeType			m_FinestTime;						
	/// 逆回购日期
	TWtpDateType			m_ContractDate;
	/// 灵活逆回购截止日期						
	TWtpDateType			m_DeadlineDate;
	/// 有效交割日期					
	TWtpDateType			m_DeliveryDate;
	/// 第一期货合约						
	TWtpInstrumentIdType	m_AlphaInstrumentId;
	/// 第二期货合约			
	TWtpInstrumentIdType	m_BravoInstrumentId;
	/// 第一期货最新价			
	TWtpPriceType			m_AlphaLastPrice;
	/// 第二期货最新价					
	TWtpPriceType			m_BravoLastPrice;
	/// 贴现比率
	TWtpRatioType			m_DiscountRatio;
	/// 正回购比率					
	TWtpRatioType			m_PosiRepoRatio;
	/// 到期逆回购比率					
	TWtpRatioType			m_ReveRepoRatio;
	/// 灵活逆回购比率					
	TWtpRatioType			m_FlexReveRepoRatio;
	/// 贴现价格				
	TWtpPriceType			m_DiscountPrice;
	/// 正回购价格					
	TWtpPriceType			m_PosiRepoPrice;
	/// 灵活逆回购价格					
	TWtpPriceType			m_FlexReveRepoPrice;
};

/// 错误消息
struct CWtpRspInfoField {
	int						m_ErrId;
	TWtpStatusMsgType		m_ErrMsg;
};

/// 用户登录请求
struct CWtpReqUserLoginField {
	TWtpUserIdType			m_UserId;
	TWtpPasswordType		m_Password;
};

/// 用户登录响应
struct CWtpRspUserLoginField {
	bool					m_Success;
};

/// 用户登出请求/响应
struct CWtpUserLogoutField {
	TWtpUserIdType			m_UserID;
};

#endif
