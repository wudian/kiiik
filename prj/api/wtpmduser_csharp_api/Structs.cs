using System.Runtime.InteropServices;

namespace wtpmduser_csharp_api
{
    [StructLayout(LayoutKind.Sequential)]
    public struct CWtpSymbolField
    {
        /// 交易所代码，例如：'SHFE' / 'DCE' / 'CZCE'
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 9)]
        public string m_ExchangeId;
        /// 品种代码，例如：'CU' / 'AL' / 'ZN'
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 31)]
        public string m_ProductId;
        /// 约定天数，0为贴现，正整数为回购
        public int m_ContractDays;   
    };

    // 仓单报价
    [StructLayout(LayoutKind.Sequential)]
    public struct CWtpQuotationField
    {
        public CWtpSymbolField m_Symbol;

        public int	m_SystemDate;						/// 系统日期
	    public int	m_SystemTime;						/// 系统时间
	    public int	m_MinuteDate;						/// 分钟数据日期
	    public int	m_MinuteTime;						/// 分钟数据时间
	    public int	m_FinestDate;						/// 逐笔数据日期
	    public int	m_FinestTime;						/// 逐笔数据时间
	    ///
	    public int	m_ContractDate;						/// 逆回购日期
	    public int	m_DeadlineDate;						/// 灵活逆回购截止日期
	    public int	m_DeliveryDate;						/// 有效交割日期
	    [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 31)]
        public string	m_AlphaInstrumentId;			/// 第一期货合约
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 31)]
        public string	m_BravoInstrumentId;			/// 第二期货合约
	    public double	m_AlphaLastPrice;					/// 第一期货最新价
	    public double	m_BravoLastPrice;					/// 第二期货最新价
	    ///
	    public double	m_DiscountRatio;					/// 贴现比率
	    public double	m_PosiRepoRatio;					/// 正回购比率
	    public double	m_ReveRepoRatio;					/// 到期逆回购比率
	    public double	m_FlexReveRepoRatio;				/// 灵活逆回购比率
	    public double	m_DiscountPrice;					/// 贴现价格
	    public double	m_PosiRepoPrice;					/// 正回购价格
	    public double	m_FlexReveRepoPrice;				/// 灵活逆回购价格
    };


    //错误消息
    [StructLayout(LayoutKind.Sequential)]
    public struct CWtpRspInfoField
    {
        public int m_ErrId;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 81)]
        public string m_ErrMsg;
    };

    //用户登录请求
    [StructLayout(LayoutKind.Sequential)]
    public struct CWtpReqUserLoginField
    {
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 20)]
        public string m_UserId;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 20)]
        public string m_Password;
    };
    //用户登录响应
    [StructLayout(LayoutKind.Sequential)]
    public struct CWtpRspUserLoginField
    {
        public bool m_Success;
    };

    //用户登出请求/响应
    [StructLayout(LayoutKind.Sequential)]
    public struct CWtpUserLogoutField
    {
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 20)]
        public string m_UserID;
    };

    
}