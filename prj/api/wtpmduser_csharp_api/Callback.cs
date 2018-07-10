using System;

namespace wtpmduser_csharp_api.Callback
{
    public delegate void fnOnFrontConnected(object sender);
    public delegate void fnOnFrontDisconnected(object sender);
    public delegate void fnOnRspUserLogin(object sender, ref CWtpRspUserLoginField pRspUserLogin, ref CWtpRspInfoField pRspInfo);
    public delegate void fnOnRspUserLogout(object sender, ref CWtpUserLogoutField pUserLogout, ref CWtpRspInfoField pRspInfo);
    public delegate void fnOnRspError(object sender, ref CWtpRspInfoField pRspInfo);

    public delegate void fnOnRspQrySymbol(object sender, ref CWtpSymbolField[] pSymbol, int nCount);
    public delegate void fnOnRspQryQuotation(object sender, ref CWtpQuotationField pDepthMarketData);
    public delegate void fnOnRtnQuotation(object sender, ref CWtpQuotationField pDepthMarketData);
}