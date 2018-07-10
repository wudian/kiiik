#include "jni_helper.h"

char* jstringTostring(JNIEnv* env, jstring jstr)
{
	char* rtn = NULL;
	jclass clsstring = env->FindClass("java/lang/String");
	jstring strencode = env->NewStringUTF("utf-8");
	jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
	jbyteArray barr = (jbyteArray)env->CallObjectMethod(jstr, mid, strencode);
	jsize alen = env->GetArrayLength(barr);
	jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
	if (alen > 0)
	{
		rtn = (char*)malloc(alen + 1);

		memcpy(rtn, ba, alen);
		rtn[alen] = 0;
	}
	env->ReleaseByteArrayElements(barr, ba, 0);
	return rtn;
}
jstring stoJstring(JNIEnv* env, const char* pat)
{
	jclass strClass = env->FindClass("Ljava/lang/String;");
	jmethodID ctorID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
	jbyteArray bytes = env->NewByteArray(strlen(pat));
	env->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte*)pat);
	jstring encoding = env->NewStringUTF("utf-8");
	return (jstring)env->NewObject(strClass, ctorID, bytes, encoding);
}

int SetJniObjectReference(JNIEnv *env, jobject jo, jobject odata, const char *szFieldName, const char *szFieldType, jobject oref)
{
	/* Get a reference to obj's class */
	jfieldID fid;
	jclass cls = env->GetObjectClass(odata);
	/* Look for the instance field s in cls */
	fid = (env)->GetFieldID(cls, szFieldName, szFieldType);
	if (fid == NULL)
	{
		return J_FAIL; /* failed to find the field */
	}
	else
	{
		env->SetObjectField(odata, fid, oref);
	}
	env->DeleteLocalRef(cls);
	return J_OK;
}

char* FieldType2ObjArrFieldType(const char *szFieldType)
{
	char* pszArrFieldType = new char[strlen(szFieldType) + 4];
	strcpy(pszArrFieldType, "[L");
	strcpy(pszArrFieldType + strlen(pszArrFieldType), szFieldType);
	strcpy(pszArrFieldType + strlen(pszArrFieldType), ";");
	return pszArrFieldType;
}

void SetJniObjectArray(JNIEnv *env, jobject jo, jobject odata, const char* szFieldName, const char *szFieldType, void* pObjectArr, int dwObjectNum, FPSetJniObjectArray pFunc)
{
	jclass objectClass = env->FindClass(szFieldType);
	if (objectClass == NULL)
	{
		return;
	}
	jmethodID m_mid = env->GetMethodID(objectClass, "<init>", "()V");
	jobjectArray jobj_arr = env->NewObjectArray((jsize)dwObjectNum, objectClass, 0);
	if (dwObjectNum > 0)
	{
		for (int i = 0; i < dwObjectNum; ++i)
		{
			jobject jobj = env->NewObject(objectClass, m_mid);
			pFunc(env, jo, jobj, pObjectArr, i);
			env->SetObjectArrayElement(jobj_arr, i, jobj);
			env->DeleteLocalRef(jobj);
		}
	}

	char* pszArrFieldType = FieldType2ObjArrFieldType(szFieldType);
	SetJniObjectReference(env, jo, odata, szFieldName, pszArrFieldType, jobj_arr);
	CORE_SAFEDELETEARRAY(pszArrFieldType);
	env->DeleteLocalRef(jobj_arr);
	env->DeleteLocalRef(objectClass);
}

int GetJniInt(JNIEnv *env, jobject jo, jobject odata, const char *szFieldName, int &nValue)
{
	/* Get a reference to obj¡¯s class */
	jfieldID fid;
	jclass cls = env->GetObjectClass(odata);
	/* Look for the instance field s in cls */
	fid = env->GetFieldID(cls, szFieldName, "I");
	if (fid == NULL)
	{
		return J_FAIL; /* failed to find the field */
	}
	else
	{
		jint iValue = env->GetIntField(odata, fid);
		nValue = (int)iValue;
	}
	env->DeleteLocalRef(cls);
	return J_OK;
}

int SetJniInt(JNIEnv *env, jobject jo, jobject odata, const char *szFieldName, int nValue)
{
	/* Get a reference to obj¡¯s class */
	jfieldID fid;
	jclass cls = env->GetObjectClass(odata);
	/* Look for the instance field s in cls */
	fid = env->GetFieldID(cls, szFieldName, "I");
	if (fid == NULL)
	{
		return J_FAIL; /* failed to find the field */
	}
	else
	{
		jint iValue = nValue;
		env->SetIntField(odata, fid, iValue);
	}
	env->DeleteLocalRef(cls);
	return J_OK;
}

int SetJniBool(JNIEnv *env, jobject jo, jobject odata, const char *szFieldName, bool bValue)
{
	/* Get a reference to obj¡¯s class */
	jfieldID fid;
	jclass cls = env->GetObjectClass(odata);
	/* Look for the instance field s in cls */
	fid = env->GetFieldID(cls, szFieldName, "Z");
	if (fid == NULL)
	{
		return J_FAIL; /* failed to find the field */
	}
	else
	{
		jboolean iValue = bValue;
		env->SetBooleanField(odata, fid, iValue);
	}
	env->DeleteLocalRef(cls);
	return J_OK;
}

int SetJniDouble(JNIEnv *env, jobject jo, jobject odata, const char *szFieldName, double dValue){
	/* Get a reference to obj¡¯s class */
	jfieldID fid;
	jclass cls = env->GetObjectClass(odata);
	/* Look for the instance field s in cls */
	fid = env->GetFieldID(cls, szFieldName, "D");
	if (fid == NULL)
	{
		return J_FAIL; /* failed to find the field */
	}
	else
	{
		jdouble iValue = dValue;
		env->SetDoubleField(odata, fid, iValue);
	}
	env->DeleteLocalRef(cls);
	return J_OK;
}

int GetJniStringUTF(JNIEnv *env, jobject jo, jobject odata, const char *szFieldName, char *szValue)
{
	/* Get a reference to obj¡¯s class */
	jfieldID fid;
	jclass cls = env->GetObjectClass(odata);
	/* Look for the instance field s in cls */
	fid = env->GetFieldID(cls, szFieldName, "Ljava/lang/String;");
	if (fid == NULL)
	{
		return J_FAIL; /* failed to find the field */
	}
	else
	{
		jstring str = (jstring)env->GetObjectField(odata, fid);
		char *temp = jstringTostring(env, str);
		if (NULL == temp)return J_FAIL;
		memcpy(szValue, temp, strlen(temp));
		free(temp);
	}
	env->DeleteLocalRef(cls);
	return J_OK;
}

int SetJniStringUTF(JNIEnv *env, jobject jo, jobject odata, const char *szFieldName, char *szValue)
{
	/* Get a reference to obj¡¯s class */
	jfieldID fid;
	jclass cls = env->GetObjectClass(odata);
	/* Look for the instance field s in cls */
	fid = env->GetFieldID(cls, szFieldName, "Ljava/lang/String;");
	if (fid == NULL)
	{
		return J_FAIL; /* failed to find the field */
	}
	else
	{
		jstring string = env->NewStringUTF(szValue);
		env->SetObjectField(odata, fid, string);
		env->DeleteLocalRef(string);
	}
	env->DeleteLocalRef(cls);
	return J_OK;
}