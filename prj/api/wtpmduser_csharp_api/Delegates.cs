using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace wtpmduser_csharp_api
{
    public delegate void OnFrontConnectedHandler(object sender, OnConnectArgs e);
    public delegate void OnFrontDisconnectedHandler(object sender, OnDisconnectArgs e);
    public delegate void OnRspUserLoginHandler(object sender, OnRspUserLoginArgs e);
    public delegate void OnRspUserLogoutHandler(object sender, OnRspUserLogoutArgs e);
    public delegate void OnRspErrorHandler(object sender, OnRspErrorArgs e);

    public delegate void OnRspQrySymbolHandler(object sender, OnRspQrySymbolArgs e);
    public delegate void OnRspQryQuotationHandler(object sender, OnRspQryQuotationArgs e);
    public delegate void OnRtnQuotationHandler(object sender, OnRtnQuotationArgs e);
}