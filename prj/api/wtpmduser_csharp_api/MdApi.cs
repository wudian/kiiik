using System;
using System.Runtime.InteropServices;

namespace wtpmduser_csharp_api
{
    public class MdApi
    {
        // 行情API
        [DllImport("wtpmduserapi.dll", EntryPoint = "RegisterFront")]
        public static extern void RegisterFront(string pszFrontAddress);

        [DllImport("wtpmduserapi.dll", EntryPoint = "ReqUserLogin")]
        public static extern void ReqUserLogin(ref CWtpReqUserLoginField pReqUserLoginField);

        [DllImport("wtpmduserapi.dll", EntryPoint = "ReqUserLogout")]
        public static extern void ReqUserLogout(ref CWtpUserLogoutField pUserLogout);

        [DllImport("wtpmduserapi.dll", EntryPoint = "ReqQrySymbol")]
        public static extern void ReqQrySymbol();

        [DllImport("wtpmduserapi.dll", EntryPoint = "ReqQryQuotation")]
        public static extern void ReqQryQuotation(ref CWtpSymbolField pInstrumentID);

        [DllImport("wtpmduserapi.dll", EntryPoint = "ReqSubQuotation")]
        public static extern void ReqSubQuotation(CWtpSymbolField[] pInstrumentID, int nCount);

        [DllImport("wtpmduserapi.dll", EntryPoint = "ReqUnSubQuotation")]
        public static extern void ReqUnSubQuotation(CWtpSymbolField[] pInstrumentID, int nCount);

        [DllImport("wtpmduserapi.dll", EntryPoint = "Release")]
        public static extern void Release();

        // 注册行情回调
        [DllImport("wtpmduserapi.dll", EntryPoint = "WTP_RegOnFrontConnected")]
        public static extern void WTP_RegOnFrontConnected(fnOnFrontConnected pCallback);

        [DllImport("wtpmduserapi.dll", EntryPoint = "WTP_RegOnFrontDisconnected")]
        public static extern void WTP_RegOnFrontDisconnected(fnOnFrontDisconnected pCallback);

        [DllImport("wtpmduserapi.dll", EntryPoint = "WTP_RegOnRspUserLogin")]
        public static extern void WTP_RegOnRspUserLogin(fnOnRspUserLogin pCallback);

        [DllImport("wtpmduserapi.dll", EntryPoint = "WTP_RegOnRspUserLogout")]
        public static extern void WTP_RegOnRspUserLogout(fnOnRspUserLogout pCallback);

        [DllImport("wtpmduserapi.dll", EntryPoint = "WTP_RegOnRspError")]
        public static extern void WTP_RegOnRspError(fnOnRspError pCallback);

        [DllImport("wtpmduserapi.dll", EntryPoint = "WTP_RegOnRspQrySymbol")]
        public static extern void WTP_RegOnRspQrySymbol(fnOnRspQrySymbol pCallback);

        [DllImport("wtpmduserapi.dll", EntryPoint = "WTP_RegOnRspQryQuotation")]
        public static extern void WTP_RegOnRspQryQuotation(fnOnRspQryQuotation pCallback);

        [DllImport("wtpmduserapi.dll", EntryPoint = "WTP_RegOnRtnQuotation")]
        public static extern void WTP_RegOnRtnQuotation(fnOnRtnQuotation pCallback);
    }
}