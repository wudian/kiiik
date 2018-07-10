using wtpmduser_csharp_api.Callback;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;

namespace wtpmduser_csharp_api
{
    public class WtpMdApiWrapper : IDisposable
    {
        public event OnFrontConnectedHandler OnFrontConnected;
        public event OnFrontDisconnectedHandler OnFrontDisconnected;
        public event OnRspUserLoginHandler OnRspUserLogin;
        public event OnRspUserLogoutHandler OnRspUserLogout;
        public event OnRspErrorHandler OnRspError;

        public event OnRspQrySymbolHandler OnRspQrySymbol;
        public event OnRspQryQuotationHandler OnRspQryQuotation;
        public event OnRtnQuotationHandler OnRtnQuotation;

        private MarketDataApi m_Api;

        public WtpMdApiWrapper()
        {
            m_Api = new MarketDataApi();

            m_Api.OnConnect = OnFrontConnected_callback;
            m_Api.OnDisconnect = OnFrontDisconnected_callback;
            m_Api.OnRspUserLogin = OnRspUserLogin_callback;
            m_Api.OnRspUserLogout = OnRspUserLogout_callback;
            m_Api.OnRspError = OnRspError_callback;

            m_Api.OnRspQrySymbol = OnRspQrySymbol_callback;
            m_Api.OnRspQryQuotation = OnRspQryQuotation_callback;
            m_Api.OnRtnQuotation = OnRtnQuotation_callback;

            m_Api.WTP_RegMD();
        }
        // 析构
        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        private bool m_disposed = false;

        protected virtual void Dispose(bool disposing)
        {
            if (!m_disposed)
            {
                if (disposing)
                {
                    // Release managed resources
                }
                // Release unmanaged resources
                m_Api.Dispose();
                m_disposed = true;
            }
        }

        ~WtpMdApiWrapper()
        {            
            Dispose(true);
        }

        public void RegisterFront(string pszFrontAddress)
        {
            m_Api.RegisterFront(pszFrontAddress);
        }
        public void ReqUserLogin(ref CWtpReqUserLoginField pReqUserLoginField)
        {
            m_Api.ReqUserLogin(ref pReqUserLoginField);
        }
        public void ReqUserLogout(ref CWtpUserLogoutField pReqUserLogoutField)
        {
            m_Api.ReqUserLogout(ref pReqUserLogoutField);
        }
        public void ReqQrySymbol()
        {
            m_Api.ReqQrySymbol();
        }
        public void ReqQryQuotation(ref CWtpSymbolField pInstrumentID)
        {
            m_Api.ReqQryQuotation(ref pInstrumentID);
        }
        public void ReqSubQuotation(ref CWtpSymbolField[] pInstrumentID)
        {
            m_Api.ReqSubQuotation(ref pInstrumentID);
        }
        public void ReqUnSubQuotation(ref CWtpSymbolField[] pInstrumentID)
        {
            m_Api.ReqUnSubQuotation(ref pInstrumentID);
        }

        //////////////////////////////////////////////////////////////////////////
        //matlab无法传递结构体的数组，故将订阅/取消订阅接口拆分
        public void AddSubWtpSymbol(ref CWtpSymbolField pInstrumentID)
        {
            SubWtpSymbols.Add(pInstrumentID);
        }
        public void ReqSubQuotation()
        {
            CWtpSymbolField[] pInstrumentID = SubWtpSymbols.ToArray();
            ReqSubQuotation(ref pInstrumentID);
            SubWtpSymbols.Clear();
        }
        public void AddUnSubWtpSymbol(ref CWtpSymbolField pInstrumentID)
        {
            UnSubWtpSymbols.Add(pInstrumentID);
        }
        public void ReqUnSubQuotation()
        {
            CWtpSymbolField[] pInstrumentID = UnSubWtpSymbols.ToArray();
            ReqUnSubQuotation(ref pInstrumentID);
            UnSubWtpSymbols.Clear();
        }
        List<CWtpSymbolField> SubWtpSymbols = new List<CWtpSymbolField>();
        List<CWtpSymbolField> UnSubWtpSymbols = new List<CWtpSymbolField>();
        //////////////////////////////////////////////////////////////////////////

        private void OnFrontConnected_callback(object sender)
        {
            if (null != OnFrontConnected)
            {
                try
                {
                    OnFrontConnected(this, new OnConnectArgs());
                }
                catch (System.Exception ex)
                {
                    Console.WriteLine("MdApiWrapper::OnConnect_callback:" + ex.ToString());
                }
            }
        }
        private void OnFrontDisconnected_callback(object sender)
        {
            if (null != OnFrontDisconnected)
            {
                try
                {
                    OnFrontDisconnected(this, new OnDisconnectArgs());
                }
                catch (System.Exception ex)
                {
                    Console.WriteLine("MdApiWrapper::OnFrontDisconnected_callback:" + ex.ToString());
                }
            }
        }
        private void OnRspUserLogin_callback(object sender, ref CWtpRspUserLoginField pRspUserLogin, ref CWtpRspInfoField pRspInfo)
        {
            if (null != OnRspUserLogin)
            {
                try
                {
                    OnRspUserLogin(this, new OnRspUserLoginArgs(ref pRspUserLogin, ref pRspInfo));
                }
                catch (System.Exception ex)
                {
                    Console.WriteLine("MdApiWrapper::OnRspUserLogin_callback:" + ex.ToString());
                }
            }
        }
        private void OnRspUserLogout_callback(object sender, ref CWtpUserLogoutField pUserLogout, ref CWtpRspInfoField pRspInfo)
        {
            if (null != OnRspUserLogout)
            {
                try
                {
                    OnRspUserLogout(this, new OnRspUserLogoutArgs(ref pUserLogout, ref pRspInfo));
                }
                catch (System.Exception ex)
                {
                    Console.WriteLine("MdApiWrapper::OnRspUserLogout_callback:" + ex.ToString());
                }
            }
        }
        private void OnRspError_callback(object sender, ref CWtpRspInfoField pRspInfo)
        {
            if (null != OnRspError)
            {
                try
                {
                    OnRspError(this, new OnRspErrorArgs(ref pRspInfo));
                }
                catch (System.Exception ex)
                {
                    Console.WriteLine("MdApiWrapper::OnRspError_callback:" + ex.ToString());
                }
            }
        }
        private void OnRspQrySymbol_callback(object sender, ref CWtpSymbolField[] pSymbol, int nCount)
        {
            if (null != OnRspQrySymbol)
            {
                try
                {
                    OnRspQrySymbol(this, new OnRspQrySymbolArgs(ref pSymbol, nCount));
                }
                catch (System.Exception ex)
                {
                    Console.WriteLine("MdApiWrapper::OnRspQrySymbol_callback:" + ex.ToString());
                }
            }
        }
        private void OnRspQryQuotation_callback(object sender, ref CWtpQuotationField pDepthMarketData)
        {
            if (null != OnRspQryQuotation)
            {
                try
                {
                    OnRspQryQuotation(this, new OnRspQryQuotationArgs(ref pDepthMarketData));
                }
                catch (System.Exception ex)
                {
                    Console.WriteLine("MdApiWrapper::OnRspQryQuotation_callback:" + ex.ToString());
                }
            }
        }
        private void OnRtnQuotation_callback(object sender, ref CWtpQuotationField pDepthMarketData)
        {
            if (null != OnRtnQuotation)
            {
                try
                {
                    OnRtnQuotation(this, new OnRtnQuotationArgs(ref pDepthMarketData));
                    Thread.Sleep(1);
                }
                catch (System.Exception ex)
                {
                    Console.WriteLine("MdApiWrapper::OnRtnQuotation_callback:" + ex.ToString());
                }
            }
        }
    }
}