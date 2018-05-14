
/*------------------------------------------------------------------------------------------
//  ANDROIDJNIOBJECT.CPP
//
//  android jni object wrapper
//
//  Author            : Imanol Celaya Ruiz de Alegria
//  Date Of Creation  : 27/04/2016 16:28:46
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "ANDROIDJNIObject.h"
#include "ANDROIDJNI.h"

#include <stdarg.h>

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/
ANDROIDJNIOBJECT::ANDROIDJNIOBJECT(XSTRING ClassName, XSTRING constructorsignature, ...)
{
  JNIEnv* JEnv = ANDROIDJNI::GetJNIEnv();

  char* cstr = NULL;
  ClassName.CreateOEM(cstr);

  jniclass = ANDROIDJNI::FindJNIClass(cstr);
  //check(Class);
  delete cstr;
  constructorsignature.CreateOEM(cstr);
  jmethodID constructor = JEnv->GetMethodID(jniclass, "<init>", cstr);
  delete cstr;
  //check(Constructor);
  va_list params;
  va_start(params, constructorsignature);
  jobject object = JEnv->NewObjectV(jniclass, constructor, params);
  va_end(params);
  ANDROIDJNI::CheckJavaException();
  //check(object);

  // Promote local references to global
  jniobject = JEnv->NewGlobalRef(object);
  JEnv->DeleteLocalRef(object);
}

ANDROIDJNIOBJECT::~ANDROIDJNIOBJECT()
{
}

ANDROIDJNIMETHOD ANDROIDJNIOBJECT::GetClassMethod(XSTRING method, XSTRING signature)
{
  ANDROIDJNIMETHOD m;
  m.name = method;
  m.signature = signature;

  JNIEnv* env = ANDROIDJNI::GetJNIEnv();

  char* cmethod;
  char* csignature;

  method.CreateOEM(cmethod);
  signature.CreateOEM(csignature);

  m.method = env->GetMethodID(jniclass, cmethod, csignature);

  delete cmethod;
  delete csignature;

  return m;
}

jstring ANDROIDJNIOBJECT::GetJString(const XSTRING& str)
{
  jstring r;

  char* cstr;
  str.CreateOEM(cstr);

  JNIEnv* env = ANDROIDJNI::GetJNIEnv();

  r = env->NewStringUTF(cstr);

  delete cstr;

  return r;
}

template<>
void ANDROIDJNIOBJECT::CallMethod<void>(ANDROIDJNIMETHOD Method, ...)
{
  JNIEnv* JEnv = ANDROIDJNI::GetJNIEnv();
  va_list Params;
  va_start(Params, Method);
  JEnv->CallVoidMethodV(jniobject, Method.method, Params);
  va_end(Params);
  ANDROIDJNI::CheckJavaException();
}


template<>
bool ANDROIDJNIOBJECT::CallMethod<bool>(ANDROIDJNIMETHOD Method, ...)
{
  JNIEnv* JEnv = ANDROIDJNI::GetJNIEnv();
  va_list Params;
  va_start(Params, Method);
  bool RetVal = JEnv->CallBooleanMethodV(jniobject, Method.method, Params);
  va_end(Params);
  ANDROIDJNI::CheckJavaException();
  return RetVal;
}

template<>
int ANDROIDJNIOBJECT::CallMethod<int>(ANDROIDJNIMETHOD Method, ...)
{
  JNIEnv* JEnv = ANDROIDJNI::GetJNIEnv();
  va_list Params;
  va_start(Params, Method);
  int RetVal = JEnv->CallIntMethodV(jniobject, Method.method, Params);
  va_end(Params);
  ANDROIDJNI::CheckJavaException();
  return RetVal;
}

template<>
jobject ANDROIDJNIOBJECT::CallMethod<jobject>(ANDROIDJNIMETHOD Method, ...)
{
  JNIEnv* JEnv = ANDROIDJNI::GetJNIEnv();
  va_list Params;
  va_start(Params, Method);
  jobject val = JEnv->CallObjectMethodV(jniobject, Method.method, Params);
  va_end(Params);
  ANDROIDJNI::CheckJavaException();
  jobject RetVal = JEnv->NewGlobalRef(val);
  JEnv->DeleteLocalRef(val);
  return RetVal;
}

template<>
long long ANDROIDJNIOBJECT::CallMethod<long long>(ANDROIDJNIMETHOD Method, ...)
{
  JNIEnv* JEnv = ANDROIDJNI::GetJNIEnv();
  va_list Params;
  va_start(Params, Method);
  long long RetVal = JEnv->CallLongMethodV(jniobject, Method.method, Params);
  va_end(Params);
  ANDROIDJNI::CheckJavaException();
  return RetVal;
}

template<>
XSTRING ANDROIDJNIOBJECT::CallMethod<XSTRING>(ANDROIDJNIMETHOD Method, ...)
{
  JNIEnv* JEnv = ANDROIDJNI::GetJNIEnv();
  va_list Params;
  va_start(Params, Method);
  jstring RetVal = static_cast<jstring>(JEnv->CallObjectMethodV(jniobject, Method.method, Params));
  va_end(Params);
  ANDROIDJNI::CheckJavaException();
  const char * UTFString = JEnv->GetStringUTFChars(RetVal, NULL);
  XSTRING Result(UTFString);
  JEnv->ReleaseStringUTFChars(RetVal, UTFString);
  return Result;
}

