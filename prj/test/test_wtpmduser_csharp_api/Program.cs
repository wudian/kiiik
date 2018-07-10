using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using wtpmduser_csharp_api;
using System.Threading;

namespace test_wtpmduser_csharp_api
{
    class Program
    {
        public static void OnConnect(object sender, OnConnectArgs e)
        {
            Console.WriteLine("OnConnect");
        }

        public static void OnDisconnect(object sender, OnDisconnectArgs e)
        {
            Console.WriteLine("OnDisconnect");
        }
        public static void OnRspUserLogin(object sender, OnRspUserLoginArgs e)
        {
            Console.WriteLine("OnRspUserLogin : {0} {1}", e.pRspUserLogin.m_Success, e.pRspInfo.m_ErrMsg);
        }
        public static void OnRspUserLogout(object sender, OnRspUserLogoutArgs e)
        {
            Console.WriteLine("OnRspUserLogout : {0} {1}", e.pUserLogout.m_UserID, e.pRspInfo.m_ErrMsg);
        }
        public static void OnRspError(object sender, OnRspErrorArgs e)
        {
            Console.WriteLine("OnRspError : {0}", e.pRspInfo.m_ErrMsg);
        }
        public static void OnRspQrySymbol(object sender, OnRspQrySymbolArgs e)
        {
            for (int i = 0; i < e.nCount; ++i )
                Console.Write("{0}_{1}_{2},", e.pSymbol[i].m_ExchangeId, e.pSymbol[i].m_ProductId, e.pSymbol[i].m_ContractDays);
            Console.WriteLine("");
        }
        public static void OnRspQryQuotation(object sender, OnRspQryQuotationArgs e)
        {
            Console.WriteLine("OnRspQryQuotation : {0}_{1}_{2} : {3}",
                e.pDepthMarketData.m_Symbol.m_ExchangeId, e.pDepthMarketData.m_Symbol.m_ProductId, e.pDepthMarketData.m_Symbol.m_ContractDays,
                 e.pDepthMarketData.m_SystemTime);
        }
        public static void OnRtnQuotation(object sender, OnRtnQuotationArgs e)
        {
            Console.WriteLine("OnRtnQuotation : {0}_{1}_{2} : {3}",
                 e.pDepthMarketData.m_Symbol.m_ExchangeId, e.pDepthMarketData.m_Symbol.m_ProductId, e.pDepthMarketData.m_Symbol.m_ContractDays,
                 e.pDepthMarketData.m_SystemTime);
        }

        static void Main(string[] args)
        {
            //Thread.Sleep(10000);


            WtpMdApiWrapper md = new WtpMdApiWrapper();
            md.OnFrontConnected += new OnFrontConnectedHandler(OnConnect);
            md.OnFrontDisconnected += new OnFrontDisconnectedHandler(OnDisconnect);
            md.OnRspUserLogin += new OnRspUserLoginHandler(OnRspUserLogin);
            md.OnRspUserLogout += new OnRspUserLogoutHandler(OnRspUserLogout);
            md.OnRspError += new OnRspErrorHandler(OnRspError);

            md.OnRspQrySymbol += new OnRspQrySymbolHandler(OnRspQrySymbol);
            md.OnRspQryQuotation += new OnRspQryQuotationHandler(OnRspQryQuotation);
            md.OnRtnQuotation += new OnRtnQuotationHandler(OnRtnQuotation);

            md.RegisterFront("tcp://121.42.157.92:8863");

            Console.WriteLine(" 1.登录\n 2.登出\n 3.查合约列表\n 4.询价\n 5.订阅\n 6.取消订阅\n or.退出系统\n 请输入你的操作:");
	int chose;

	while ((chose = Convert.ToInt32(Console.ReadLine())) != 0)
	{
		//getchar();
		switch (chose)
		{
		case 1:
		{
				  CWtpReqUserLoginField ReqUserLoginField = new CWtpReqUserLoginField();
                  ReqUserLoginField.m_UserId = "kiiik";
                  ReqUserLoginField.m_Password = "888888";
                  md.ReqUserLogin(ref ReqUserLoginField);
				  
		}
			break;
            case 2:
            {
                CWtpUserLogoutField ReqUserLogoutField = new CWtpUserLogoutField();
                ReqUserLogoutField.m_UserID = "kiiik";
                md.ReqUserLogout(ref ReqUserLogoutField);
            }
            break;
		case 3:
		{
				  md.ReqQrySymbol();
		}
			break;
		case 4:
		{
				    CWtpSymbolField sym = new CWtpSymbolField();
                    sym.m_ExchangeId = "SHFE";
                    sym.m_ProductId = "CU";
                    sym.m_ContractDays = 30;
				    md.ReqQryQuotation(ref sym);
		}
			break;
		case 5:
		{
				  CWtpSymbolField[] syms = new CWtpSymbolField[7];
                  syms[0].m_ExchangeId = "SHFE";
                  syms[0].m_ProductId = "CU";
                  syms[0].m_ContractDays = 0;
                  syms[1].m_ExchangeId = "SHFE";
                  syms[1].m_ProductId = "CU";
                  syms[1].m_ContractDays = 30;
                  syms[2].m_ExchangeId = "SHFE";
                  syms[2].m_ProductId = "CU";
                  syms[2].m_ContractDays = 60;
                  syms[3].m_ExchangeId = "SHFE";
                  syms[3].m_ProductId = "CU";
                  syms[3].m_ContractDays = 90;
                  syms[4].m_ExchangeId = "SHFE";
                  syms[4].m_ProductId = "CU";
                  syms[4].m_ContractDays = 120;
                  syms[5].m_ExchangeId = "SHFE";
                  syms[5].m_ProductId = "CU";
                  syms[5].m_ContractDays = 150;
                  syms[6].m_ExchangeId = "SHFE";
                  syms[6].m_ProductId = "CU";
                  syms[6].m_ContractDays = 180;
				  md.ReqSubQuotation(ref syms);
		}
			break;
            case 6:
            {
                CWtpSymbolField[] syms = new CWtpSymbolField[7];
                syms[0].m_ExchangeId = "SHFE";
                syms[0].m_ProductId = "CU";
                syms[0].m_ContractDays = 0;
                syms[1].m_ExchangeId = "SHFE";
                syms[1].m_ProductId = "CU";
                syms[1].m_ContractDays = 30;
                syms[2].m_ExchangeId = "SHFE";
                syms[2].m_ProductId = "CU";
                syms[2].m_ContractDays = 60;
                syms[3].m_ExchangeId = "SHFE";
                syms[3].m_ProductId = "CU";
                syms[3].m_ContractDays = 90;
                syms[4].m_ExchangeId = "SHFE";
                syms[4].m_ProductId = "CU";
                syms[4].m_ContractDays = 120;
                syms[5].m_ExchangeId = "SHFE";
                syms[5].m_ProductId = "CU";
                syms[5].m_ContractDays = 150;
                syms[6].m_ExchangeId = "SHFE";
                syms[6].m_ProductId = "CU";
                syms[6].m_ContractDays = 180;
                md.ReqUnSubQuotation(ref syms);
            }
            break;
		default:
            {
                md.Dispose();
            }
			return;
		}

        Thread.Sleep(1000);
        Console.WriteLine(" 1.登录\n 2.登出\n 3.查合约列表\n 4.询价\n 5.订阅\n 6.取消订阅\n or.退出系统\n 请输入你的操作:");

	} while (false);

            while (true)
                ;
        }
    }
}
