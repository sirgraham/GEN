
/*------------------------------------------------------------------------------------------
//	ANDROIDJNI.CPP
//	
//	Android JNI interface
//   
//	Author						: Imanol Celaya Ruiz de Alegria
//	Date Of Creation	: 27/04/2016 16:06:08
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "ANDROIDJNI.h"
#include "android_native_app_glue.h"

#include "XDebug.h"

#include <pthread.h>

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/
JavaVM*		ANDROIDJNI::currentjavaVM;
jint			ANDROIDJNI::currentjavaversion;
jobject		ANDROIDJNI::ClassLoader;
jmethodID ANDROIDJNI::FindClassMethod;

/*-------------------------------------------------------------------
//	ANDROIDJNI::InitializeJNI
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			27/04/2016 13:29:27
//	
*/
/*-----------------------------------------------------------------*/
void ANDROIDJNI::InitializeJNI(android_app* application, jint javaversion)
{
	currentjavaVM = application->activity->vm;
	currentjavaversion = javaversion;

	JNIEnv* env = ANDROIDJNI::GetJNIEnv();

	jobject mainactivity = application->activity->clazz;
	//jclass MainClass = env->FindClass("gen/endorasoft/com/GENNativeActivity");
	jclass MainClass = env->GetObjectClass(mainactivity);
	jclass classClass = env->FindClass("java/lang/Class");
	jclass classLoaderClass = env->FindClass("java/lang/ClassLoader");
	jmethodID getClassLoaderMethod = env->GetMethodID(classClass, "getClassLoader", "()Ljava/lang/ClassLoader;");
	jobject classLoader = env->CallObjectMethod(MainClass, getClassLoaderMethod);
	ClassLoader = env->NewGlobalRef(classLoader);
	FindClassMethod = env->GetMethodID(classLoaderClass, "findClass", "(Ljava/lang/String;)Ljava/lang/Class;");
}





/*-------------------------------------------------------------------
//	JavaEnvDestructor
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			27/04/2016 13:42:49
//	
//	@return 			static : 
//
//  @param				void* : 
*/
/*-----------------------------------------------------------------*/
static void JavaEnvDestructor(void*)
{
	ANDROIDJNI::DetachJNIEnv();
}


/*-------------------------------------------------------------------
//	ANDROIDJNI::GetJNIEnv
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			27/04/2016 13:10:07
//	
//	@return 			JNIEnv* : 
//
*/
/*-----------------------------------------------------------------*/
JNIEnv* ANDROIDJNI::GetJNIEnv()
{
	static XDWORD threadslot = 0;
	if (threadslot == 0)
		{
			pthread_key_create((pthread_key_t*)&threadslot, &JavaEnvDestructor);
		}

	JNIEnv* env = NULL;

	if(!currentjavaVM)
	{
		XDEBUG_PRINTCOLOR(4, __L("No Current Java VM"));
	}

	jint getenvresult = currentjavaVM->GetEnv((void**)&env, currentjavaversion);
	if(getenvresult == JNI_EDETACHED)
		{
			jint attachresult = currentjavaVM->AttachCurrentThread(&env, NULL);

			if(attachresult == JNI_ERR)
				{
					return NULL;
				}
		}
	else if(getenvresult != JNI_OK)
		{
			return NULL;
		}

	return env;
}





/*-------------------------------------------------------------------
//	ANDROIDJNI::DetachJNIEnv
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			27/04/2016 13:29:33
//	
*/
/*-----------------------------------------------------------------*/
void ANDROIDJNI::DetachJNIEnv()
{
	currentjavaVM->DetachCurrentThread();
}



/*-------------------------------------------------------------------
//	ANDROIDJNI::FindJNIClass
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			27/04/2016 13:29:40
//	
//	@return 			jclass : 
//
//  @param				name : 
*/
/*-----------------------------------------------------------------*/
jclass ANDROIDJNI::FindJNIClass(const XSTRING & name)
{
	char* cname = NULL;
	name.CreateOEM(cname);
	return ANDROIDJNI::FindJNIClass(cname);
}




/*-------------------------------------------------------------------
//	ANDROIDJNI::FindJNIClass
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			27/04/2016 13:29:49
//	
//	@return 			jclass : 
//
//  @param				name : 
*/
/*-----------------------------------------------------------------*/
jclass ANDROIDJNI::FindJNIClass(XSTRING * name)
{
	return ANDROIDJNI::FindJNIClass(*name);
}




/*-------------------------------------------------------------------
//	ANDROIDJNI::FindJNIClass
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			27/04/2016 13:29:56
//	
//	@return 			jclass : 
//
//  @param				name : 
*/
/*-----------------------------------------------------------------*/
jclass ANDROIDJNI::FindJNIClass(XCHAR * name)
{
	return ANDROIDJNI::FindJNIClass(XSTRING(name));
}




/*-------------------------------------------------------------------
//	ANDROIDJNI::FindJNIClass
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			27/04/2016 13:30:03
//	
//	@return 			jclass : 
//
//  @param				name : 
*/
/*-----------------------------------------------------------------*/
jclass ANDROIDJNI::FindJNIClass(const char * name)
{
	JNIEnv* env = ANDROIDJNI::GetJNIEnv();
	if (!env)
	{
		return NULL;
	}

	jstring ClassNameObj = env->NewStringUTF(name);
	jclass FoundClass = static_cast<jclass>(env->CallObjectMethod(ClassLoader, FindClassMethod, ClassNameObj));
	ANDROIDJNI::CheckJavaException();
	env->DeleteLocalRef(ClassNameObj);

	return FoundClass;
}





/*-------------------------------------------------------------------
//	ANDROIDJNI::FindJNIEnvClassWrapper
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			27/04/2016 13:30:47
//	
//	@return 			jclass : 
//
//  @param				name : 
*/
/*-----------------------------------------------------------------*/
jclass ANDROIDJNI::FindJNIEnvClassWrapper(const char * name)
{
	JNIEnv* env = ANDROIDJNI::GetJNIEnv();
	if (!env)
	{
		return NULL;
	}
	
	return env->FindClass(name);
}




/*-------------------------------------------------------------------
//	ANDROIDJNI::CheckJavaException
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			27/04/2016 16:11:54
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool ANDROIDJNI::CheckJavaException()
{
	JNIEnv* env = ANDROIDJNI::GetJNIEnv();
	if (!env)
	{
		return true;
	}
	if (env->ExceptionCheck())
	{
		env->ExceptionDescribe();
		
		//jthrowable exception = env->ExceptionOccurred();
		env->ExceptionClear();
		//
		//jclass throwable_class = env->FindClass("java/lang/Throwable");
		//
		//jmethodID mid_throwable_getCause =
    //env->GetMethodID(throwable_class,
    //                  "getCause",
    //                  "()Ljava/lang/Throwable;");
		//jmethodID mid_throwable_getStackTrace =
		//    env->GetMethodID(throwable_class,
		//                      "getStackTrace",
		//                      "()[Ljava/lang/StackTraceElement;");
		//jmethodID mid_throwable_toString =
		//    env->GetMethodID(throwable_class,
		//                      "toString",
		//                      "()Ljava/lang/String;");
		//
		//jclass frame_class = env->FindClass("java/lang/StackTraceElement");
		//jmethodID mid_frame_toString =
		//    env->GetMethodID(frame_class,
		//                      "toString",
		//                      "()Ljava/lang/String;");
		//
		// show the string of the exception
		//verify(false && "Java JNI call failed with an exception.");
		return true;
	}
	return false;
}

