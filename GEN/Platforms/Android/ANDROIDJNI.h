/*------------------------------------------------------------------------------------------
//	ANDROIDJNI.H
*/	
/**	
// \file 
//   
//  Android JNI Interface
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 27/04/2016 16:03:52
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _ANDROIDJNI_H_
#define _ANDROIDJNI_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include <jni.h>
	
#include "XString.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
struct android_app;

class ANDROIDJNI
{
public:
	static void											InitializeJNI(android_app* application, jint javaversion);
	static JNIEnv*									GetJNIEnv();
	static void											DetachJNIEnv();
	static jclass										FindJNIClass(const XSTRING & name);
	static jclass										FindJNIClass(XSTRING * name);
	static jclass										FindJNIClass(XCHAR * name);
	static jclass										FindJNIClass(const char * name);
	static jclass										FindJNIEnvClassWrapper(const char * name);
	static bool											CheckJavaException();
protected:

private:
	static JavaVM* currentjavaVM;
	static jint currentjavaversion;
	static jobject ClassLoader;
	static jmethodID FindClassMethod;
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

