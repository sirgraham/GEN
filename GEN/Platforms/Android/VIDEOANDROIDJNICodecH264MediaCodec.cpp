
/*------------------------------------------------------------------------------------------
//	VIDEOANDROIDJNICODECH264MEDIACODEC.CPP
//	
//	android JNI interface for mediacodec
//   
//	Author						: Imanol Celaya Ruiz de Alegria
//	Date Of Creation	: 27/04/2016 17:29:24
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "VIDEOANDROIDJNICodecH264MediaCodec.h"
#include "ANDROIDJNI.h"	

#include "MEDIAH264FrameReader.h"

#include "GRPRenderer.h"
#include "GRPContext.h"

#include "XFactory.h"
#include "XTimer.h"

#include "XVector.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/
VIDEOANDROIDJNICODECH264MEDIACODEC::VIDEOANDROIDJNICODECH264MEDIACODEC(VIDEOPLAYER* videoplayer) : VIDEOCODECH264(videoplayer),
	ANDROIDJNIOBJECT("gen/endorasoft/com/ANDROIDVideo", "()V"),
	jniIni(GetClassMethod("IniCodec", "([B[BLjava/lang/String;III)Z")),
	jniSendFrame(GetClassMethod("SendFrame", "([BJIZ)Z"))
{
}

VIDEOANDROIDJNICODECH264MEDIACODEC::~VIDEOANDROIDJNICODECH264MEDIACODEC()
{
}


bool VIDEOANDROIDJNICODECH264MEDIACODEC::Ini()
{
	xtimer = xfactory->CreateTimer();

	videoplayer->GetRenderer()->GetContext()->Destroy();
	xtimer->Reset();
	return true;
}

bool VIDEOANDROIDJNICODECH264MEDIACODEC::IniCodec()
{
	// need to pass correct arguments like the SPS and the PPS
	JNIEnv* env = ANDROIDJNI::GetJNIEnv();

	jbyteArray jsps = env->NewByteArray(SPS.GetSize());
	env->SetByteArrayRegion(jsps, 0, SPS.GetSize(), (const jbyte*)SPS.GetPointer(0));

	jbyteArray jpps = env->NewByteArray(PPS.GetSize());
	env->SetByteArrayRegion(jpps, 0, PPS.GetSize(), (const jbyte*)PPS.GetPointer(0));

	//jstring jmime =	env->NewStringUTF("video/avc");
	jstring jmime = ANDROIDJNIOBJECT::GetJString("video/avc");
	jint jwidth = width;
	jint jheight = height;
	jint jfps = (jint)fps;

	bool status = CallMethod<bool>(jniIni, jsps, jpps, jmime, jwidth, jheight, jfps);

	//env->ReleaseByteArrayElements(jsps, (jbyte*)SPS.GetPointer(0), 0);
	//env->ReleaseByteArrayElements(jpps, (jbyte*)PPS.GetPointer(0), 0);
	env->DeleteLocalRef(jsps);
	env->DeleteLocalRef(jpps);

	flags = 1;

	initialized = true;
	isplaying = true;

	return status;
}

bool VIDEOANDROIDJNICODECH264MEDIACODEC::SendFrame(XVECTOR<XBYTE>& frame)
{
	// need to take the xvector to a java byte[] parameter
	
	JNIEnv*env = ANDROIDJNI::GetJNIEnv();

	jbyteArray jframe = env->NewByteArray(frame.GetSize());
	env->SetByteArrayRegion(jframe, 0, frame.GetSize(), (const jbyte*)frame.GetPointer(0));

	// need to get and store the timestamp and usage flags

	jint jflags = flags;
	jlong jtimestamp = timestamp;

	bool status = CallMethod<bool>(jniSendFrame, jframe, jtimestamp, jflags, true);
	flags = 0;
	//env->ReleaseByteArrayElements(jframe, (jbyte*)frame.GetPointer(0), 0);
	env->DeleteLocalRef(jframe);

	return status;
}

void VIDEOANDROIDJNICODECH264MEDIACODEC::Decode()
{
	if(!initialized)
	{
		IniCodec();
	}

	if(timestamp + xtimer->GetMeasureMicroSeconds() > (((1.0/fps)*1000*1000) + timestamp))
		{
			XDEBUG_PRINTCOLOR(4, __L("Decode"));
	
			XVECTOR<XBYTE> frame;
			framereader->ReadFrame(frame);
			SendFrame(frame);

			XDEBUG_PRINTCOLOR(4, __L("Decode Finish"));
			timestamp += xtimer->GetMeasureMicroSeconds();
			xtimer->Reset();
		}
}

