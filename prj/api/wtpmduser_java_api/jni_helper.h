#include <jni.h>
#include <windows.h>

#define J_FAIL	-1
#define J_OK	0

#ifndef NELEM
#define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))
#endif

#define CORE_SAFEDELETEARRAY(p) if ((p) != NULL) { delete [] (p); (p) = NULL; }
typedef void(*FPSetJniObjectArray)(JNIEnv *env, jobject jo, jobject odata, void* pObjectArr, int dwIdx);

char* jstringTostring(JNIEnv* env, jstring jstr);
jstring stoJstring(JNIEnv* env, const char* pat);

int SetJniObjectReference(JNIEnv *env, jobject jo, jobject odata, const char *szFieldName, const char *szFieldType, jobject oref);
void SetJniObjectArray(JNIEnv *env, jobject jo, jobject odata, const char* szFieldName, const char *szFieldType, void* pObjectArr, int dwObjectNum, FPSetJniObjectArray pFunc);

int GetJniInt(JNIEnv *env, jobject jo, jobject odata, const char *szFieldName, int &nValue);
int SetJniInt(JNIEnv *env, jobject jo, jobject odata, const char *szFieldName, int nValue);
int SetJniBool(JNIEnv *env, jobject jo, jobject odata, const char *szFieldName, bool bValue);
int SetJniDouble(JNIEnv *env, jobject jo, jobject odata, const char *szFieldName, double dValue);

int GetJniStringUTF(JNIEnv *env, jobject jo, jobject odata, const char *szFieldName, char *szValue);
int SetJniStringUTF(JNIEnv *env, jobject jo, jobject odata, const char *szFieldName, char *szValue);