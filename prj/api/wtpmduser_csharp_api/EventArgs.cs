using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace wtpmduser_csharp_api
{
    public class OnConnectArgs : EventArgs
    {
        public OnConnectArgs()
        {            
        }
    }
    public class OnDisconnectArgs : EventArgs
    {
        public OnDisconnectArgs()
        {
        }
    }

    public class OnRspUserLoginArgs : EventArgs
    {
        public readonly CWtpRspUserLoginField pRspUserLogin;
        public readonly CWtpRspInfoField pRspInfo;
        public OnRspUserLoginArgs(ref CWtpRspUserLoginField pRspUserLogin, ref CWtpRspInfoField pRspInfo)
        {
            this.pRspUserLogin = pRspUserLogin;
            this.pRspInfo = pRspInfo;
        }
    }

    public class OnRspUserLogoutArgs : EventArgs
    {
        public readonly CWtpUserLogoutField pUserLogout;
        public readonly CWtpRspInfoField pRspInfo;
        public OnRspUserLogoutArgs(ref CWtpUserLogoutField pUserLogout, ref CWtpRspInfoField pRspInfo)
        {
            this.pUserLogout = pUserLogout;
            this.pRspInfo = pRspInfo;
        }
    }

    public class OnRspErrorArgs : EventArgs
    {
        public readonly CWtpRspInfoField pRspInfo;
        public OnRspErrorArgs(ref CWtpRspInfoField pRspInfo)
        {
            this.pRspInfo = pRspInfo;
        }
    }

    public class OnRspQrySymbolArgs : EventArgs
    {
        public readonly CWtpSymbolField []pSymbol;
        public readonly int nCount;
        public OnRspQrySymbolArgs(ref CWtpSymbolField[] pSymbol, int nCount)
        {
            this.pSymbol = pSymbol;
            this.nCount = nCount;
        }
    }

    public class OnRspQryQuotationArgs : EventArgs
    {
        public readonly CWtpQuotationField pDepthMarketData;
        public OnRspQryQuotationArgs(ref CWtpQuotationField pDepthMarketData)
        {
            this.pDepthMarketData = pDepthMarketData;
        }
    }

    public class OnRtnQuotationArgs : EventArgs
    {
        public readonly CWtpQuotationField pDepthMarketData;
        public OnRtnQuotationArgs(ref CWtpQuotationField pDepthMarketData)
        {
            this.pDepthMarketData = pDepthMarketData;
        }
    }
}