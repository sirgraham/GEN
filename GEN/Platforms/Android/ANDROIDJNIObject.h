/*------------------------------------------------------------------------------------------
//  ANDROIDJNIOBJECT.H
*/
/**
// \file
//
//  wrapper around a java class accessed through JNI
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 27/04/2016 16:21:44
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _ANDROIDJNIOBJECT_H_
#define _ANDROIDJNIOBJECT_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include <jni.h>
#include "XString.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/
struct ANDROIDJNIMETHOD
{
  XSTRING name;
  XSTRING signature;
  jmethodID method;
};


class ANDROIDJNIOBJECT
{
public:

  ANDROIDJNIOBJECT(XSTRING name, XSTRING constructorsignature, ...);
  virtual ~ANDROIDJNIOBJECT();

  ANDROIDJNIMETHOD GetClassMethod(XSTRING method, XSTRING signature);

  template<typename T>
  T CallMethod(ANDROIDJNIMETHOD method, ...);

  jobject GetObject() { return jniobject; }

  static jstring GetJString(const XSTRING& str);

protected:

  jobject     jniobject;
  jclass      jniclass;

private:
  void Clean()
  {
    jniobject = 0;
    jniclass  = 0;
  }

  ANDROIDJNIOBJECT(const ANDROIDJNIOBJECT& rhs);
  ANDROIDJNIOBJECT& operator = (const ANDROIDJNIOBJECT& rhs);
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

//template<void>
//void ANDROIDJNIOBJECT::CallMethod<void>(ANDROIDJNIOBJECT Method, ...);
//
//template<bool>
//bool ANDROIDJNIOBJECT::CallMethod<bool>(ANDROIDJNIOBJECT Method, ...);
//
//template<int>
//int ANDROIDJNIOBJECT::CallMethod<int>(ANDROIDJNIOBJECT Method, ...);
//
//template<jobject>
//jobject ANDROIDJNIOBJECT::CallMethod<jobject>(ANDROIDJNIOBJECT Method, ...);
//
//template<long long>
//long long ANDROIDJNIOBJECT::CallMethod<long long>(ANDROIDJNIOBJECT Method, ...);
//
//template<XSTRING>
//XSTRING ANDROIDJNIOBJECT::CallMethod<XSTRING>(ANDROIDJNIOBJECT Method, ...);

#endif

