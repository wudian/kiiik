// wtpmd.cpp : 定义 DLL 应用程序的导出函数。
//

#include "wtpmd.h"

///-------------------------------------------------------------------------------------
///从Python对象到C++类型转换用的函数
///-------------------------------------------------------------------------------------

void getInt(dict d, string key, int *value)
{
	if (d.has_key(key))		//检查字典中是否存在该键值
	{
		object o = d[key];	//获取该键值
		extract<int> x(o);	//创建提取器
		if (x.check())		//如果可以提取
		{
			*value = x();	//对目标整数指针赋值
		}
	}
};

void getDouble(dict d, string key, double *value)
{
	if (d.has_key(key))
	{
		object o = d[key];
		extract<double> x(o);
		if (x.check())
		{
			*value = x();
		}
	}
};

void getStr(dict d, string key, char *value)
{
	if (d.has_key(key))
	{
		object o = d[key];
		extract<string> x(o);
		if (x.check())
		{
			string s = x();
			const char *buffer = s.c_str();
			//对字符串指针赋值必须使用strcpy_s, vs2013使用strcpy编译通不过
			//+1应该是因为C++字符串的结尾符号？不是特别确定，不加这个1会出错
#ifdef _MSC_VER //WIN32
			strcpy_s(value, strlen(buffer) + 1, buffer);
#elif __GNUC__
			strncpy(value, buffer, strlen(buffer) + 1);
#endif
		}
	}
};

void getChar(dict d, string key, char *value)
{
	if (d.has_key(key))
	{
		object o = d[key];
		extract<string> x(o);
		if (x.check())
		{
			string s = x();
			const char *buffer = s.c_str();
			*value = *buffer;
		}
	}
};


///-------------------------------------------------------------------------------------
///C++的回调函数将数据保存到队列中
///-------------------------------------------------------------------------------------

void MdApi::OnFrontConnected()
{
	Task task = Task();
	task.task_name = ONFRONTCONNECTED;
	this->task_queue.push(task);
};

void MdApi::OnFrontDisconnected()
{
	Task task = Task();
	task.task_name = ONFRONTDISCONNECTED;
	this->task_queue.push(task);
};

void MdApi::OnRspUserLogin(CWtpRspUserLoginField *pRspUserLogin, CWtpRspInfoField *pRspInfo)
{
	Task task = Task();
	task.task_name = ONRSPUSERLOGIN;
	
	if (pRspUserLogin)
	{
		task.task_data = *pRspUserLogin;
	}
	else
	{
		CWtpRspUserLoginField empty_data = CWtpRspUserLoginField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CWtpRspInfoField empty_error = CWtpRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	
	this->task_queue.push(task);
};

void MdApi::OnRspUserLogout(CWtpUserLogoutField *pUserLogout, CWtpRspInfoField *pRspInfo)
{
	Task task = Task();
	task.task_name = ONRSPUSERLOGOUT;
	
	if (pUserLogout)
	{
		task.task_data = *pUserLogout;
	}
	else
	{
		CWtpUserLogoutField empty_data = CWtpUserLogoutField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CWtpRspInfoField empty_error = CWtpRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	this->task_queue.push(task);
};

void MdApi::OnRspError(CWtpRspInfoField *pRspInfo)
{
	Task task = Task();
	task.task_name = ONRSPERROR;

	if (pRspInfo)
	{
		task.task_error = *pRspInfo;
	}
	else
	{
		CWtpRspInfoField empty_error = CWtpRspInfoField();
		memset(&empty_error, 0, sizeof(empty_error));
		task.task_error = empty_error;
	}
	this->task_queue.push(task);
};

///查询合约列表响应
void MdApi::OnRspQrySymbol(CWtpSymbolField *pSymbol, int nCount){
	CStructWtpSymbolField wtp_sym_list;
	wtp_sym_list.pSymbol = new CWtpSymbolField[nCount];
	memcpy(wtp_sym_list.pSymbol, pSymbol, sizeof(CWtpSymbolField)*nCount);
	wtp_sym_list.nCount = nCount;

	Task task = Task();
	task.task_name = ONRSPQRYSYMBOL;
	task.task_data = wtp_sym_list;
	
	this->task_queue.push(task);
}

///查询单个合约价格响应
void MdApi::OnRspQryQuotation(CWtpQuotationField *pQuotation){
	Task task = Task();
	task.task_name = ONRSPQRYQUOTATION;

	if (pQuotation)
	{
		task.task_data = *pQuotation;
	}
	else
	{
		CWtpQuotationField empty_data = CWtpQuotationField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}
	this->task_queue.push(task);
}

///深度行情通知
void MdApi::OnRtnQuotation(CWtpQuotationField *pQuotation){
	Task task = Task();
	task.task_name = ONRTNQUOTATION;

	if (pQuotation)
	{
		task.task_data = *pQuotation;
	}
	else
	{
		CWtpQuotationField empty_data = CWtpQuotationField();
		memset(&empty_data, 0, sizeof(empty_data));
		task.task_data = empty_data;
	}
	this->task_queue.push(task);
}




///-------------------------------------------------------------------------------------
///工作线程从队列中取出数据，转化为python对象后，进行推送
///-------------------------------------------------------------------------------------

void MdApi::processTask()
{
	while (1)
	{
		Task task = this->task_queue.wait_and_pop();

		switch (task.task_name)
		{
		case ONFRONTCONNECTED:
		{
			this->processFrontConnected(task);
			break;
		}

		case ONFRONTDISCONNECTED:
		{
			this->processFrontDisconnected(task);
			break;
		}


		case ONRSPUSERLOGIN:
		{
			this->processRspUserLogin(task);
			break;
		}

		case ONRSPUSERLOGOUT:
		{
			this->processRspUserLogout(task);
			break;
		}

		case ONRSPERROR:
		{
			this->processRspError(task);
			break;
		}

		case ONRSPQRYSYMBOL:
		{
			this->processRspQrySymbol(task);
			break;
		}

		case ONRSPQRYQUOTATION:
		{
			this->processRspQryQuotation(task);
			break;
		}

		case ONRTNQUOTATION:
		{
			this->processRtnQuotation(task);
			break;
		}

		};
	}
};

void MdApi::processFrontConnected(Task task)
{
	PyLock lock;
	this->onFrontConnected();
};

void MdApi::processFrontDisconnected(Task task)
{
	PyLock lock;
	this->onFrontDisconnected();
};


void MdApi::processRspUserLogin(Task task)
{
	PyLock lock;
	CWtpRspUserLoginField task_data = any_cast<CWtpRspUserLoginField>(task.task_data);
	dict data;
	data["m_Success"] = task_data.m_Success;

	this->onRspUserLogin(data);
};

void MdApi::processRspUserLogout(Task task)
{
	PyLock lock;
	CWtpUserLogoutField task_data = any_cast<CWtpUserLogoutField>(task.task_data);
	dict data;
	data["m_UserID"] = task_data.m_UserID;

	this->onRspUserLogout(data);
};

void MdApi::processRspError(Task task)
{
	PyLock lock;
	CWtpRspInfoField task_error = any_cast<CWtpRspInfoField>(task.task_error);
	dict error;
	error["m_ErrId"] = task_error.m_ErrId;
	error["m_ErrMsg"] = task_error.m_ErrMsg;

	this->onRspError(error);
};

void MdApi::processRspQrySymbol(Task task)
{
	PyLock lock;
	CStructWtpSymbolField wtp_sym_list = any_cast<CStructWtpSymbolField>(task.task_data);
	boost::python::list list_data;
	for (int i = 0; i < wtp_sym_list.nCount; ++i){
		dict data;
		data["m_ExchangeId"] = wtp_sym_list.pSymbol[i].m_ExchangeId;
		data["m_ProductId"] = wtp_sym_list.pSymbol[i].m_ProductId;
		data["m_ContractDays"] = wtp_sym_list.pSymbol[i].m_ContractDays;
		list_data.append(data);
	}

	delete[]wtp_sym_list.pSymbol;
	this->onRspQrySymbol(list_data);
};

void MdApi::processRspQryQuotation(Task task)
{
	PyLock lock;
	CWtpQuotationField task_data = any_cast<CWtpQuotationField>(task.task_data);
	dict data;
	data["m_ExchangeId"] = task_data.m_Symbol.m_ExchangeId;
	data["m_ProductId"] = task_data.m_Symbol.m_ProductId;
	data["m_ContractDays"] = task_data.m_Symbol.m_ContractDays;
	data["m_SystemDate"] = task_data.m_SystemDate;
	data["m_SystemTime"] = task_data.m_SystemTime;
	data["m_MinuteDate"] = task_data.m_MinuteDate;
	data["m_MinuteTime"] = task_data.m_MinuteTime;
	data["m_FinestDate"] = task_data.m_FinestDate;
	data["m_FinestTime"] = task_data.m_FinestTime;
	data["m_ContractDate"] = task_data.m_ContractDate;
	data["m_DeadlineDate"] = task_data.m_DeadlineDate;
	data["m_DeliveryDate"] = task_data.m_DeliveryDate;
	data["m_AlphaInstrumentId"] = task_data.m_AlphaInstrumentId;
	data["m_BravoInstrumentId"] = task_data.m_BravoInstrumentId;
	data["m_AlphaLastPrice"] = task_data.m_AlphaLastPrice;
	data["m_BravoLastPrice"] = task_data.m_BravoLastPrice;
	data["m_DiscountRatio"] = task_data.m_DiscountRatio;
	data["m_PosiRepoRatio"] = task_data.m_PosiRepoRatio;
	data["m_ReveRepoRatio"] = task_data.m_ReveRepoRatio;
	data["m_FlexReveRepoRatio"] = task_data.m_FlexReveRepoRatio;
	data["m_DiscountPrice"] = task_data.m_DiscountPrice;
	data["m_PosiRepoPrice"] = task_data.m_PosiRepoPrice;
	data["m_FlexReveRepoPrice"] = task_data.m_FlexReveRepoPrice;

	this->onRspQryQuotation(data);
};

void MdApi::processRtnQuotation(Task task)
{
	PyLock lock;
	CWtpQuotationField task_data = any_cast<CWtpQuotationField>(task.task_data);
	dict data;
	data["m_ExchangeId"] = task_data.m_Symbol.m_ExchangeId;
	data["m_ProductId"] = task_data.m_Symbol.m_ProductId;
	data["m_ContractDays"] = task_data.m_Symbol.m_ContractDays;
	data["m_SystemDate"] = task_data.m_SystemDate;
	data["m_SystemTime"] = task_data.m_SystemTime;
	data["m_MinuteDate"] = task_data.m_MinuteDate;
	data["m_MinuteTime"] = task_data.m_MinuteTime;
	data["m_FinestDate"] = task_data.m_FinestDate;
	data["m_FinestTime"] = task_data.m_FinestTime;
	data["m_ContractDate"] = task_data.m_ContractDate;
	data["m_DeadlineDate"] = task_data.m_DeadlineDate;
	data["m_DeliveryDate"] = task_data.m_DeliveryDate;
	data["m_AlphaInstrumentId"] = task_data.m_AlphaInstrumentId;
	data["m_BravoInstrumentId"] = task_data.m_BravoInstrumentId;
	data["m_AlphaLastPrice"] = task_data.m_AlphaLastPrice;
	data["m_BravoLastPrice"] = task_data.m_BravoLastPrice;
	data["m_DiscountRatio"] = task_data.m_DiscountRatio;
	data["m_PosiRepoRatio"] = task_data.m_PosiRepoRatio;
	data["m_ReveRepoRatio"] = task_data.m_ReveRepoRatio;
	data["m_FlexReveRepoRatio"] = task_data.m_FlexReveRepoRatio;
	data["m_DiscountPrice"] = task_data.m_DiscountPrice;
	data["m_PosiRepoPrice"] = task_data.m_PosiRepoPrice;
	data["m_FlexReveRepoPrice"] = task_data.m_FlexReveRepoPrice;

	this->onRtnQuotation(data);
};



///-------------------------------------------------------------------------------------
///主动函数
///-------------------------------------------------------------------------------------

void MdApi::createFtdcMdApi()
{
	this->api = CWtpMdApi::CreateWtpMdApi();
	this->api->RegisterSpi(this);
};

void MdApi::release()
{
	this->api->Release();
};

void MdApi::registerFront(string pszFrontAddress)
{
	this->api->RegisterFront((char*)pszFrontAddress.c_str());
};

int MdApi::reqUserLogin(dict req)
{
	CWtpReqUserLoginField myreq;
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "m_UserId", myreq.m_UserId);
	getStr(req, "m_Password", myreq.m_Password);
	int i = this->api->ReqUserLogin(&myreq);
	return i;
};

int MdApi::reqUserLogout(dict req)
{
	CWtpUserLogoutField myreq;
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "m_UserID", myreq.m_UserID);
	int i = this->api->ReqUserLogout(&myreq);
	return i;
};

int MdApi::reqQrySymbol(){
	int i = this->api->ReqQrySymbol();
	return i;
}

int MdApi::reqQryQuotation(dict req){
	CWtpSymbolField myreq;
	memset(&myreq, 0, sizeof(myreq));
	getStr(req, "m_ExchangeId", myreq.m_ExchangeId);
	getStr(req, "m_ProductId", myreq.m_ProductId);
	getInt(req, "m_ContractDays", &myreq.m_ContractDays);
	int i = this->api->ReqQryQuotation(&myreq);
	return i;
}

int MdApi::reqSubQuotation(boost::python::list req){
	int length = len(req);
	CWtpSymbolField *sym_list = new CWtpSymbolField[length];
	memset(sym_list, 0, sizeof(CWtpSymbolField)*length);

	for (int i = 0; i < length; ++i) {
		boost::python::dict pySym = (dict)req[i];
		getStr(pySym, "m_ExchangeId", sym_list[i].m_ExchangeId);
		getStr(pySym, "m_ProductId", sym_list[i].m_ProductId);
		getInt(pySym, "m_ContractDays", &sym_list[i].m_ContractDays);		
	}

	int i = this->api->ReqSubQuotation(sym_list, length);
	delete[]sym_list;
	return i;
}

int MdApi::reqUnSubQuotation(boost::python::list req){
	int length = len(req);
	CWtpSymbolField *sym_list = new CWtpSymbolField[length];
	memset(sym_list, 0, sizeof(CWtpSymbolField)*length);

	for (int i = 0; i < length; ++i) {
		boost::python::dict pySym = (dict)req[i];
		getStr(pySym, "m_ExchangeId", sym_list[i].m_ExchangeId);
		getStr(pySym, "m_ProductId", sym_list[i].m_ProductId);
		getInt(pySym, "m_ContractDays", &sym_list[i].m_ContractDays);
	}

	int i = this->api->ReqUnSubQuotation(sym_list, length);
	delete[]sym_list;
	return i;
}





///-------------------------------------------------------------------------------------
///Boost.Python封装
///-------------------------------------------------------------------------------------

struct MdApiWrap : MdApi, wrapper < MdApi >
{
	virtual void onFrontConnected()
	{
		//以下的try...catch...可以实现捕捉python环境中错误的功能，防止C++直接出现原因未知的崩溃
		try
		{
			this->get_override("onFrontConnected")();
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onFrontDisconnected()
	{
		try
		{
			this->get_override("onFrontDisconnected")();
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onRspUserLogin(dict data)
	{
		try
		{
			this->get_override("onRspUserLogin")(data);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};
	
	virtual void onRspUserLogout(dict data)
	{
		try
		{
			this->get_override("onRspUserLogout")(data);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onRspError(dict error)
	{
		try
		{
			this->get_override("onRspError")(error);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onRspQrySymbol(boost::python::list list_data)
	{
		try
		{
			this->get_override("onRspQrySymbol")(list_data);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onRspQryQuotation(dict data)
	{
		try
		{
			this->get_override("onRspQryQuotation")(data);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onRtnQuotation(dict data)
	{
		try
		{
			this->get_override("onRtnQuotation")(data);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};
};


BOOST_PYTHON_MODULE(wtpmduser_python_api)
{
	PyEval_InitThreads();	//导入时运行，保证先创建GIL

	class_<MdApiWrap, boost::noncopyable>("MdApi")
		.def("createFtdcMdApi", &MdApiWrap::createFtdcMdApi)
		.def("release", &MdApiWrap::release)
		.def("registerFront", &MdApiWrap::registerFront)
		.def("reqUserLogin", &MdApiWrap::reqUserLogin)
		.def("reqUserLogout", &MdApiWrap::reqUserLogout)		
		.def("reqQrySymbol", &MdApiWrap::reqQrySymbol)
		.def("reqQryQuotation", &MdApiWrap::reqQryQuotation)
		.def("reqSubQuotation", &MdApiWrap::reqSubQuotation)
		.def("reqUnSubQuotation", &MdApiWrap::reqUnSubQuotation)

		.def("onFrontConnected", pure_virtual(&MdApiWrap::onFrontConnected))
		.def("onFrontDisconnected", pure_virtual(&MdApiWrap::onFrontDisconnected))
		.def("onRspError", pure_virtual(&MdApiWrap::onRspError))
		.def("onRspUserLogin", pure_virtual(&MdApiWrap::onRspUserLogin))
		.def("onRspUserLogout", pure_virtual(&MdApiWrap::onRspUserLogout))
		.def("onRspQrySymbol", pure_virtual(&MdApiWrap::onRspQrySymbol))
		.def("onRspQryQuotation", pure_virtual(&MdApiWrap::onRspQryQuotation))
		.def("onRtnQuotation", pure_virtual(&MdApiWrap::onRtnQuotation))
		;
};
