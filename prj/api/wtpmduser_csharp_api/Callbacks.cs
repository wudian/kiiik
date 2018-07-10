using System;

namespace wtpmduser_csharp_api
{
    public delegate void fnOnFrontConnected();
    public delegate void fnOnFrontDisconnected();
    public delegate void fnOnRspUserLogin(ref CWtpRspUserLoginField pRspUserLogin, ref CWtpRspInfoField pRspInfo);
    public delegate void fnOnRspUserLogout(ref CWtpUserLogoutField pUserLogout, ref CWtpRspInfoField pRspInfo);
    public delegate void fnOnRspError(ref CWtpRspInfoField pRspInfo);

    public delegate void fnOnRspQrySymbol(IntPtr pSymbol, int nCount);
    public delegate void fnOnRspQryQuotation(ref CWtpQuotationField pDepthMarketData);
    public delegate void fnOnRtnQuotation(ref CWtpQuotationField pDepthMarketData);
}