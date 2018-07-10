using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.ComponentModel;
using System.Threading.Tasks;
using System.Xml.Serialization;
using System.Runtime.InteropServices;

namespace wtpmduser_csharp_api.Callback
{
    public class MarketDataApi
    {
        private object locker = new object();

        private fnOnFrontConnected OnConnect_1;
        private fnOnFrontDisconnected OnDisconnect_1;
        private fnOnRspUserLogin OnRspUserLogin_1;
        private fnOnRspUserLogout OnRspUserLogout_1;
        private fnOnRspError OnRspError_1;
        private fnOnRspQrySymbol OnRspQrySymbol_1;
        private fnOnRspQryQuotation OnRspQryQuotation_1;
        private fnOnRtnQuotation OnRtnQuotation_1;

        private wtpmduser_csharp_api.fnOnFrontConnected OnConnect_2;
        private wtpmduser_csharp_api.fnOnFrontDisconnected OnDisconnect_2;
        private wtpmduser_csharp_api.fnOnRspUserLogin OnRspUserLogin_2;
        private wtpmduser_csharp_api.fnOnRspUserLogout OnRspUserLogout_2;
        private wtpmduser_csharp_api.fnOnRspError OnRspError_2;
        private wtpmduser_csharp_api.fnOnRspQrySymbol OnRspQrySymbol_2;
        private wtpmduser_csharp_api.fnOnRspQryQuotation OnRspQryQuotation_2;
        private wtpmduser_csharp_api.fnOnRtnQuotation OnRtnQuotation_2; 

        public MarketDataApi()
        {

        }

        ~MarketDataApi()
        {
            Dispose();
        }

        public void Dispose()
        {
            lock (locker)
            {
                MdApi.Release();
                GC.SuppressFinalize(this);
            }
        }

        public void WTP_RegMD()
        {
            MdApi.WTP_RegOnFrontConnected(OnConnect_2);
            MdApi.WTP_RegOnFrontDisconnected(OnDisconnect_2);
            MdApi.WTP_RegOnRspUserLogin(OnRspUserLogin_2);
            MdApi.WTP_RegOnRspUserLogout(OnRspUserLogout_2);
            MdApi.WTP_RegOnRspError(OnRspError_2);
            MdApi.WTP_RegOnRspQrySymbol(OnRspQrySymbol_2);
            MdApi.WTP_RegOnRspQryQuotation(OnRspQryQuotation_2);
            MdApi.WTP_RegOnRtnQuotation(OnRtnQuotation_2);
        }

        public void RegisterFront(string pszFrontAddress)
        {
            lock (locker)
            {
                MdApi.RegisterFront(pszFrontAddress);
            }            
        }
        public void ReqUserLogin(ref CWtpReqUserLoginField pReqUserLoginField)
        {
            lock (locker)
            {
                MdApi.ReqUserLogin(ref pReqUserLoginField);
            }            
        }
        public void ReqUserLogout(ref CWtpUserLogoutField pReqUserLogoutField)
        {
            lock (locker)
            {
                MdApi.ReqUserLogout(ref pReqUserLogoutField);
            }
        }
        public void ReqQrySymbol()
        {
            lock (locker)
            {
                MdApi.ReqQrySymbol();
            }
        }
        public void ReqQryQuotation(ref CWtpSymbolField pInstrumentID)
        {
            lock (locker)
            {
                MdApi.ReqQryQuotation(ref pInstrumentID);
            }
        }
        public void ReqSubQuotation(ref CWtpSymbolField[] pInstrumentID)
        {
            lock (locker)
            {
                MdApi.ReqSubQuotation(pInstrumentID, pInstrumentID.Length);
            }
        }
        public void ReqUnSubQuotation(ref CWtpSymbolField[] pInstrumentID)
        {
            lock (locker)
            {
                MdApi.ReqUnSubQuotation(pInstrumentID, pInstrumentID.Length);
            }
        }

        public fnOnFrontConnected OnConnect
        {
            set
            {
                OnConnect_1 = value;
                OnConnect_2 = OnConnect_3;
            }
        }
        public fnOnFrontDisconnected OnDisconnect
        {
            set
            {
                OnDisconnect_1 = value;
                OnDisconnect_2 = OnDisconnect_3;
            }
        }
        public fnOnRspUserLogin OnRspUserLogin
        {
            set
            {
                OnRspUserLogin_1 = value;
                OnRspUserLogin_2 = OnRspUserLogin_3;
            }
        }
        public fnOnRspUserLogout OnRspUserLogout
        {
            set
            {
                OnRspUserLogout_1 = value;
                OnRspUserLogout_2 = OnRspUserLogout_3;
            }
        }
        public fnOnRspError OnRspError
        {
            set
            {
                OnRspError_1 = value;
                OnRspError_2 = OnRspError_3;
            }
        }
        public fnOnRspQrySymbol OnRspQrySymbol
        {
            set
            {
                OnRspQrySymbol_1 = value;
                OnRspQrySymbol_2 = OnRspQrySymbol_3;
            }
        }
        public fnOnRspQryQuotation OnRspQryQuotation
        {
            set
            {
                OnRspQryQuotation_1 = value;
                OnRspQryQuotation_2 = OnRspQryQuotation_3;
            }
        }
        public fnOnRtnQuotation OnRtnQuotation
        {
            set
            {
                OnRtnQuotation_1 = value;
                OnRtnQuotation_2 = OnRtnQuotation_3;
            }
        }

       

        private void OnConnect_3()
        {
            OnConnect_1(this);
        }
        private void OnDisconnect_3()
        {
            OnDisconnect_1(this);
        }
        private void OnRspUserLogin_3(ref CWtpRspUserLoginField pRspUserLogin, ref CWtpRspInfoField pRspInfo)
        {
            OnRspUserLogin_1(this, ref pRspUserLogin, ref pRspInfo);
        }
        private void OnRspUserLogout_3(ref CWtpUserLogoutField pUserLogout, ref CWtpRspInfoField pRspInfo)
        {
            OnRspUserLogout_1(this, ref pUserLogout, ref pRspInfo);
        }
        private void OnRspError_3(ref CWtpRspInfoField pRspInfo)
        {
            OnRspError_1(this, ref pRspInfo);
        }
        private void OnRspQrySymbol_3(IntPtr pSymbol, int nCount)
        {         
            CWtpSymbolField[] Symbols = new CWtpSymbolField[nCount];
            for (int i=0;i<nCount;++i){

                Symbols[i] = (CWtpSymbolField)Marshal.PtrToStructure(
                    (IntPtr)pSymbol.ToInt32() + i * Marshal.SizeOf(typeof(CWtpSymbolField)), typeof(CWtpSymbolField));
            }
            OnRspQrySymbol_1(this, ref Symbols, nCount);
        }

        private void OnRspQryQuotation_3(ref CWtpQuotationField pDepthMarketData)
        {
            OnRspQryQuotation_1(this, ref pDepthMarketData);
        }

        private void OnRtnQuotation_3(ref CWtpQuotationField pDepthMarketData)
        {
            OnRtnQuotation_1(this, ref pDepthMarketData);
        }
    }
}