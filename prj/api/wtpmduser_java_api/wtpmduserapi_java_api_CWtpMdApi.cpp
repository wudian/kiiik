#include "jni_helper.h"
#include "wtpmduserapi_java_api_CWtpMdApi.h"
#include "wtpmduserapi/WtpMdApi.h"


JavaVM* gs_jvm;
class MyCWtpMdSpi;
MyCWtpMdSpi *spi = NULL;
CWtpMdApi *api = NULL;

jobject jSpi; // 全局的java对象 CWtpMdSpi

/*The JNIEnv pointer, passed as the first argument to every native method, can only be used in
the thread with which it is associated. It is wrong to cache the JNIEnv interface pointer 
obtained from one thread, and use that pointer in another thread.
*/
class MyCWtpMdSpi : public CWtpMdSpi {
	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	virtual void OnFrontConnected(){	
		JNIEnv *env;
		gs_jvm->AttachCurrentThread((void **)&env, NULL);
		jclass cls = env->FindClass("wtpmduserapi_java_api/CWtpMdSpi");
		jmethodID mid = env->GetMethodID(cls, "OnFrontConnected", "()V");
		env->DeleteLocalRef(cls);
		env->CallVoidMethod(jSpi, mid);
	};

	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	virtual void OnFrontDisconnected(){
		JNIEnv *env;
		gs_jvm->AttachCurrentThread((void **)&env, NULL);
		jclass cls = env->FindClass("wtpmduserapi_java_api/CWtpMdSpi");
		jmethodID mid = env->GetMethodID(cls, "OnFrontDisconnected", "()V");
		env->DeleteLocalRef(cls);
		env->CallVoidMethod(jSpi, mid);
	};

	///登录请求响应
	virtual void OnRspUserLogin(CWtpRspUserLoginField *pRspUserLogin, CWtpRspInfoField *pRspInfo) {
		JNIEnv *env;
		gs_jvm->AttachCurrentThread((void **)&env, NULL);
		jclass cls = env->FindClass("wtpmduserapi_java_api/CWtpMdSpi");
		jmethodID mid = env->GetMethodID(cls, "OnRspUserLogin",
			"(Lwtpmduserapi_java_api/CWtpRspUserLoginField;Lwtpmduserapi_java_api/CWtpRspInfoField;)V");
		env->DeleteLocalRef(cls);
		
		jclass clsLogin = env->FindClass("wtpmduserapi_java_api/CWtpRspUserLoginField");
		jmethodID midLogin = env->GetMethodID(clsLogin, "<init>", "()V");
		jobject objLogin = env->NewObject(clsLogin, midLogin);
		env->DeleteLocalRef(clsLogin);
		SetJniBool(env, objLogin, objLogin, "m_Success", pRspUserLogin->m_Success);
		
		jclass clsInfo = env->FindClass("wtpmduserapi_java_api/CWtpRspInfoField");
		jmethodID midInfo = env->GetMethodID(clsInfo, "<init>", "()V");
		jobject objInfo = env->NewObject(clsInfo, midInfo);
		env->DeleteLocalRef(clsInfo);
		SetJniInt(env, objInfo, objInfo, "m_ErrId", pRspInfo->m_ErrId);
		SetJniStringUTF(env, objInfo, objInfo, "m_ErrMsg", pRspInfo->m_ErrMsg);

		env->CallVoidMethod(jSpi, mid, objLogin, objInfo);
		env->DeleteLocalRef(objLogin);
		env->DeleteLocalRef(objInfo);
	};


	///登出请求响应
	virtual void OnRspUserLogout(CWtpUserLogoutField *pUserLogout, CWtpRspInfoField *pRspInfo) {
		JNIEnv *env;
		gs_jvm->AttachCurrentThread((void **)&env, NULL);
		jclass cls = env->FindClass("wtpmduserapi_java_api/CWtpMdSpi");
		jmethodID mid = env->GetMethodID(cls, "OnRspUserLogout",
			"(Lwtpmduserapi_java_api/CWtpUserLogoutField;Lwtpmduserapi_java_api/CWtpRspInfoField;)V");
		env->DeleteLocalRef(cls);

		jclass clsLogout = env->FindClass("wtpmduserapi_java_api/CWtpUserLogoutField");
		jmethodID midLogout = env->GetMethodID(clsLogout, "<init>", "()V");
		jobject objLogout = env->NewObject(clsLogout, midLogout);
		env->DeleteLocalRef(clsLogout);
		SetJniStringUTF(env, objLogout, objLogout, "m_UserID", pUserLogout->m_UserID);

		jclass clsInfo = env->FindClass("wtpmduserapi_java_api/CWtpRspInfoField");
		jmethodID midInfo = env->GetMethodID(clsInfo, "<init>", "()V");
		jobject objInfo = env->NewObject(clsInfo, midInfo);
		env->DeleteLocalRef(clsInfo);
		SetJniInt(env, objInfo, objInfo, "m_ErrId", pRspInfo->m_ErrId);
		SetJniStringUTF(env, objInfo, objInfo, "m_ErrMsg", pRspInfo->m_ErrMsg);

		env->CallVoidMethod(jSpi, mid, objLogout, objInfo);
		env->DeleteLocalRef(objLogout);
		env->DeleteLocalRef(objInfo);
	};

	///错误应答
	virtual void OnRspError(CWtpRspInfoField *pRspInfo) {
		JNIEnv *env;
		gs_jvm->AttachCurrentThread((void **)&env, NULL);
		jclass cls = env->FindClass("wtpmduserapi_java_api/CWtpMdSpi");
		jmethodID mid = env->GetMethodID(cls, "OnRspError",
			"(Lwtpmduserapi_java_api/CWtpRspInfoField;)V");
		env->DeleteLocalRef(cls);

		jclass clsInfo = env->FindClass("wtpmduserapi_java_api/CWtpRspInfoField");
		jmethodID midInfo = env->GetMethodID(clsInfo, "<init>", "()V");
		jobject objInfo = env->NewObject(clsInfo, midInfo);
		env->DeleteLocalRef(clsInfo);
		SetJniInt(env, objInfo, objInfo, "m_ErrId", pRspInfo->m_ErrId);
		SetJniStringUTF(env, objInfo, objInfo, "m_ErrMsg", pRspInfo->m_ErrMsg);

		env->CallVoidMethod(jSpi, mid, objInfo);
		env->DeleteLocalRef(objInfo);
	};

	///查询合约列表响应
	virtual void OnRspQrySymbol(CWtpSymbolField *pSymbol, int nCount) {
		
		JNIEnv *env;
		gs_jvm->AttachCurrentThread((void **)&env, NULL);
		jclass cls = env->FindClass("wtpmduserapi_java_api/CWtpMdSpi");
		jmethodID mid = env->GetMethodID(cls, "OnRspQrySymbol",
			"([Lwtpmduserapi_java_api/CWtpSymbolField;)V");
		env->DeleteLocalRef(cls);

		jobjectArray result;
		jclass clsSym = env->FindClass("wtpmduserapi_java_api/CWtpSymbolField");
		jmethodID midSym = env->GetMethodID(clsSym, "<init>", "(Ljava/lang/String;Ljava/lang/String;I)V");
		result = env->NewObjectArray(nCount, clsSym, 0);
		for (int i = 0; i < nCount; ++i){			
			jobject objSym = env->NewObject(clsSym, midSym);		
			SetJniStringUTF(env, objSym, objSym, "m_ExchangeId", pSymbol[i].m_ExchangeId);
			SetJniStringUTF(env, objSym, objSym, "m_ProductId", pSymbol[i].m_ProductId);
			SetJniInt(env, objSym, objSym, "m_ContractDays", pSymbol[i].m_ContractDays);
			env->SetObjectArrayElement(result, i, objSym);
			env->DeleteLocalRef(objSym);
		}
		env->DeleteLocalRef(clsSym);
		

		env->CallVoidMethod(jSpi, mid, result);
		env->DeleteLocalRef(result);
	};

	///查询单个合约价格响应
	virtual void OnRspQryQuotation(CWtpQuotationField *pDepthMarketData) {
		JNIEnv *env;
		gs_jvm->AttachCurrentThread((void **)&env, NULL);
		jclass cls = env->FindClass("wtpmduserapi_java_api/CWtpMdSpi");
		jmethodID mid = env->GetMethodID(cls, "OnRspQryQuotation",
			"(Lwtpmduserapi_java_api/CWtpQuotationField;)V");
		env->DeleteLocalRef(cls);

		jclass clsQuotation = env->FindClass("wtpmduserapi_java_api/CWtpQuotationField");
		jmethodID midQuotation = env->GetMethodID(clsQuotation, "<init>", "()V");
		jobject objQuotation = env->NewObject(clsQuotation, midQuotation);
		env->DeleteLocalRef(clsQuotation);

		SetJniStringUTF(env, objQuotation, objQuotation, "m_ExchangeId", pDepthMarketData->m_Symbol.m_ExchangeId);
		SetJniStringUTF(env, objQuotation, objQuotation, "m_ProductId", pDepthMarketData->m_Symbol.m_ProductId);
		SetJniInt(env, objQuotation, objQuotation, "m_ContractDays", pDepthMarketData->m_Symbol.m_ContractDays);
		SetJniInt(env, objQuotation, objQuotation, "m_SystemDate", pDepthMarketData->m_SystemDate);
		SetJniInt(env, objQuotation, objQuotation, "m_SystemTime", pDepthMarketData->m_SystemTime);
		SetJniInt(env, objQuotation, objQuotation, "m_MinuteDate", pDepthMarketData->m_MinuteDate);
		SetJniInt(env, objQuotation, objQuotation, "m_MinuteTime", pDepthMarketData->m_MinuteTime);
		SetJniInt(env, objQuotation, objQuotation, "m_FinestDate", pDepthMarketData->m_FinestDate);
		SetJniInt(env, objQuotation, objQuotation, "m_FinestTime", pDepthMarketData->m_FinestTime);
		SetJniInt(env, objQuotation, objQuotation, "m_ContractDate", pDepthMarketData->m_ContractDate);
		SetJniInt(env, objQuotation, objQuotation, "m_DeadlineDate", pDepthMarketData->m_DeadlineDate);
		SetJniInt(env, objQuotation, objQuotation, "m_DeliveryDate", pDepthMarketData->m_DeliveryDate);
		SetJniStringUTF(env, objQuotation, objQuotation, "m_AlphaInstrumentId", pDepthMarketData->m_AlphaInstrumentId);
		SetJniStringUTF(env, objQuotation, objQuotation, "m_BravoInstrumentId", pDepthMarketData->m_BravoInstrumentId);
		SetJniDouble(env, objQuotation, objQuotation, "m_AlphaLastPrice", pDepthMarketData->m_AlphaLastPrice);
		SetJniDouble(env, objQuotation, objQuotation, "m_BravoLastPrice", pDepthMarketData->m_BravoLastPrice);
		SetJniDouble(env, objQuotation, objQuotation, "m_DiscountRatio", pDepthMarketData->m_DiscountRatio);
		SetJniDouble(env, objQuotation, objQuotation, "m_PosiRepoRatio", pDepthMarketData->m_PosiRepoRatio);
		SetJniDouble(env, objQuotation, objQuotation, "m_ReveRepoRatio", pDepthMarketData->m_ReveRepoRatio);
		SetJniDouble(env, objQuotation, objQuotation, "m_FlexReveRepoRatio", pDepthMarketData->m_FlexReveRepoRatio);
		SetJniDouble(env, objQuotation, objQuotation, "m_DiscountPrice", pDepthMarketData->m_DiscountPrice);
		SetJniDouble(env, objQuotation, objQuotation, "m_PosiRepoPrice", pDepthMarketData->m_PosiRepoPrice);
		SetJniDouble(env, objQuotation, objQuotation, "m_FlexReveRepoPrice", pDepthMarketData->m_FlexReveRepoPrice);

		env->CallVoidMethod(jSpi, mid, objQuotation);
		env->DeleteLocalRef(objQuotation);
	};

	///深度行情通知
	virtual void OnRtnQuotation(CWtpQuotationField *pDepthMarketData) {
		JNIEnv *env;
		gs_jvm->AttachCurrentThread((void **)&env, NULL);
		jclass cls = env->FindClass("wtpmduserapi_java_api/CWtpMdSpi");
		jmethodID mid = env->GetMethodID(cls, "OnRtnQuotation",
			"(Lwtpmduserapi_java_api/CWtpQuotationField;)V");
		env->DeleteLocalRef(cls);

		jclass clsQuotation = env->FindClass("wtpmduserapi_java_api/CWtpQuotationField");
		jmethodID midQuotation = env->GetMethodID(clsQuotation, "<init>", "()V");
		jobject objQuotation = env->NewObject(clsQuotation, midQuotation);
		env->DeleteLocalRef(clsQuotation);

		SetJniStringUTF(env, objQuotation, objQuotation, "m_ExchangeId", pDepthMarketData->m_Symbol.m_ExchangeId);
		SetJniStringUTF(env, objQuotation, objQuotation, "m_ProductId", pDepthMarketData->m_Symbol.m_ProductId);
		SetJniInt(env, objQuotation, objQuotation, "m_ContractDays", pDepthMarketData->m_Symbol.m_ContractDays);
		SetJniInt(env, objQuotation, objQuotation, "m_SystemDate", pDepthMarketData->m_SystemDate);
		SetJniInt(env, objQuotation, objQuotation, "m_SystemTime", pDepthMarketData->m_SystemTime);
		SetJniInt(env, objQuotation, objQuotation, "m_MinuteDate", pDepthMarketData->m_MinuteDate);
		SetJniInt(env, objQuotation, objQuotation, "m_MinuteTime", pDepthMarketData->m_MinuteTime);
		SetJniInt(env, objQuotation, objQuotation, "m_FinestDate", pDepthMarketData->m_FinestDate);
		SetJniInt(env, objQuotation, objQuotation, "m_FinestTime", pDepthMarketData->m_FinestTime);
		SetJniInt(env, objQuotation, objQuotation, "m_ContractDate", pDepthMarketData->m_ContractDate);
		SetJniInt(env, objQuotation, objQuotation, "m_DeadlineDate", pDepthMarketData->m_DeadlineDate);
		SetJniInt(env, objQuotation, objQuotation, "m_DeliveryDate", pDepthMarketData->m_DeliveryDate);
		SetJniStringUTF(env, objQuotation, objQuotation, "m_AlphaInstrumentId", pDepthMarketData->m_AlphaInstrumentId);
		SetJniStringUTF(env, objQuotation, objQuotation, "m_BravoInstrumentId", pDepthMarketData->m_BravoInstrumentId);
		SetJniDouble(env, objQuotation, objQuotation, "m_AlphaLastPrice", pDepthMarketData->m_AlphaLastPrice);
		SetJniDouble(env, objQuotation, objQuotation, "m_BravoLastPrice", pDepthMarketData->m_BravoLastPrice);
		SetJniDouble(env, objQuotation, objQuotation, "m_DiscountRatio", pDepthMarketData->m_DiscountRatio);
		SetJniDouble(env, objQuotation, objQuotation, "m_PosiRepoRatio", pDepthMarketData->m_PosiRepoRatio);
		SetJniDouble(env, objQuotation, objQuotation, "m_ReveRepoRatio", pDepthMarketData->m_ReveRepoRatio);
		SetJniDouble(env, objQuotation, objQuotation, "m_FlexReveRepoRatio", pDepthMarketData->m_FlexReveRepoRatio);
		SetJniDouble(env, objQuotation, objQuotation, "m_DiscountPrice", pDepthMarketData->m_DiscountPrice);
		SetJniDouble(env, objQuotation, objQuotation, "m_PosiRepoPrice", pDepthMarketData->m_PosiRepoPrice);
		SetJniDouble(env, objQuotation, objQuotation, "m_FlexReveRepoPrice", pDepthMarketData->m_FlexReveRepoPrice);

		env->CallVoidMethod(jSpi, mid, objQuotation);
		env->DeleteLocalRef(objQuotation);
	};
};


JNIEXPORT void JNICALL Java_wtpmduserapi_1java_1api_CWtpMdApi_RegisterSpi
(JNIEnv *env, jobject, jobject spi)
{
	jSpi = env->NewGlobalRef(spi);
}

JNIEXPORT void JNICALL Java_wtpmduserapi_1java_1api_CWtpMdApi_RegisterFront
(JNIEnv *env, jobject, jstring jstr)
{
	if (NULL == spi) {
		env->GetJavaVM(&gs_jvm);//来获取JavaVM指针.获取了这个指针后,将该JavaVM保存起来。
		spi = new MyCWtpMdSpi;
		api = CWtpMdApi::CreateWtpMdApi();
		api->RegisterSpi(spi);

		
	}

	char *pszFrontAddress = jstringTostring(env, jstr);
	api->RegisterFront(pszFrontAddress);
}

JNIEXPORT jint JNICALL Java_wtpmduserapi_1java_1api_CWtpMdApi_ReqUserLogin
(JNIEnv *env, jobject jo, jobject jobj)
{
	CWtpReqUserLoginField pReqUserLoginField = {0};
	GetJniStringUTF(env, jo, jobj, "m_UserId", pReqUserLoginField.m_UserId);
	GetJniStringUTF(env, jo, jobj, "m_Password", pReqUserLoginField.m_Password);
	
	api->ReqUserLogin(&pReqUserLoginField);
	
	return 0;
}


JNIEXPORT jint JNICALL Java_wtpmduserapi_1java_1api_CWtpMdApi_ReqUserLogout
(JNIEnv *env, jobject jo, jobject jobj){
	CWtpUserLogoutField pUserLogoutField = { 0 };
	GetJniStringUTF(env, jo, jobj, "m_UserID", pUserLogoutField.m_UserID);
	api->ReqUserLogout(&pUserLogoutField);
	return 0;
}

/*
* Class:     wtpmduserapi_java_api_CWtpMdApi
* Method:    ReqQrySymbol
* Signature: ()I
*/
JNIEXPORT jint JNICALL Java_wtpmduserapi_1java_1api_CWtpMdApi_ReqQrySymbol
(JNIEnv *, jobject){
	api->ReqQrySymbol();
	return 0;
}

/*
* Class:     wtpmduserapi_java_api_CWtpMdApi
* Method:    ReqQryQuotation
* Signature: (Lwtpmduserapi_java_api/CWtpSymbolField;)I
*/
JNIEXPORT jint JNICALL Java_wtpmduserapi_1java_1api_CWtpMdApi_ReqQryQuotation
(JNIEnv *env, jobject jo, jobject jobj){
	CWtpSymbolField sym = {0};
	GetJniStringUTF(env, jo, jobj, "m_ExchangeId", sym.m_ExchangeId);
	GetJniStringUTF(env, jo, jobj, "m_ProductId", sym.m_ProductId);
	GetJniInt(env, jo, jobj, "m_ContractDays", sym.m_ContractDays);
	api->ReqQryQuotation(&sym);
	return 0;
}

/*
* Class:     wtpmduserapi_java_api_CWtpMdApi
* Method:    ReqSubQuotation
* Signature: ([Lwtpmduserapi_java_api/CWtpSymbolField;)I
*/
JNIEXPORT jint JNICALL Java_wtpmduserapi_1java_1api_CWtpMdApi_ReqSubQuotation
(JNIEnv *env, jobject jo, jobjectArray jobjs){
	int len = env->GetArrayLength(jobjs);
	CWtpSymbolField *syms = new CWtpSymbolField[len];
	memset(syms, 0, sizeof(CWtpSymbolField)*len);
	for (int i = 0; i < len; ++i){
		jobject jobj = env->GetObjectArrayElement(jobjs, i);
		GetJniStringUTF(env, jo, jobj, "m_ExchangeId", syms[i].m_ExchangeId);
		GetJniStringUTF(env, jo, jobj, "m_ProductId", syms[i].m_ProductId);
		GetJniInt(env, jo, jobj, "m_ContractDays", syms[i].m_ContractDays);
	}
	api->ReqSubQuotation(syms, len);
	delete[] syms;
	return 0;
}

/*
* Class:     wtpmduserapi_java_api_CWtpMdApi
* Method:    ReqUnSubQuotation
* Signature: ([Lwtpmduserapi_java_api/CWtpSymbolField;)I
*/
JNIEXPORT jint JNICALL Java_wtpmduserapi_1java_1api_CWtpMdApi_ReqUnSubQuotation
(JNIEnv *env, jobject jo, jobjectArray jobjs){
	int len = env->GetArrayLength(jobjs);
	CWtpSymbolField *syms = new CWtpSymbolField[len];
	memset(syms, 0, sizeof(CWtpSymbolField)*len);
	for (int i = 0; i < len; ++i){
		jobject jobj = env->GetObjectArrayElement(jobjs, i);
		GetJniStringUTF(env, jo, jobj, "m_ExchangeId", syms[i].m_ExchangeId);
		GetJniStringUTF(env, jo, jobj, "m_ProductId", syms[i].m_ProductId);
		GetJniInt(env, jo, jobj, "m_ContractDays", syms[i].m_ContractDays);
	}
	api->ReqUnSubQuotation(syms, len);
	delete[] syms;
	return 0;
}


