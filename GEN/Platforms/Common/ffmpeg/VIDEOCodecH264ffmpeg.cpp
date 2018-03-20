
/*------------------------------------------------------------------------------------------
//	VIDEOCODECH264FFMPEG.CPP
//	
//	decodes an h264 using ffmpeg
//   
//	Author						: Imanol Celaya Ruiz de Alegria
//	Date Of Creation	: 27/05/2016 17:56:13
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "VIDEOCodecH264ffmpeg.h"
#include "MEDIAFrameReaderLibAV.h"	

#include "GRPRenderer.h"
#include "GRPScene.h"
#include "GRPNode.h"
#include "GRPCamera.h"
#include "GRPViewport.h"
#include "GRPObject.h"
#include "GRPElement.h"
#include "GRPShaderLibrary.h"
#include "GRPMaterialLibrary.h"
#include "GRPObjectBuilder.h"
#include "GRPTextureEXT.h"

#ifdef SND_ACTIVE
#include "SNDFactory.h"
#include "SNDStreamElement.h"
#endif

#include "XTimer.h"

#include "XDebug.h"
#include "XMemory.h"

#define BUFFEREDSIZEMAX 10*1024*1024

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


struct FFMPEGPACKET
{
	XVECTOR<XBYTE>* frame;
	long long pts;
	long long dts;
};

/*-------------------------------------------------------------------
//	VIDEOCODECH264FFMPEG::VIDEOCODECH264FFMPEG
*/	
/**	
//	
//	Class Constructor VIDEOWINDOWSCODECH264FFMPEG
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			30/05/2016 9:30:26
//	
//  @param				videoplayer : 
*/
/*-----------------------------------------------------------------*/
VIDEOCODECH264FFMPEG::VIDEOCODECH264FFMPEG(VIDEOPLAYER* videoplayer) : VIDEOCODECH264(videoplayer)
{
	Clean();

	framereader = new MEDIAFRAMEREADERLIBAV();

	xtimer = xfactory->CreateTimer();
	xtimer->Reset();

	xmutex = xfactory->Create_Mutex();

	//videoplayer->GetRenderer()->GetScene(1)->AddObserver(this);

	Y = new XBUFFER(false);
	U = new XBUFFER(false);
	V = new XBUFFER(false);

#ifdef SND_ACTIVE
	if(videoplayer->GetSoundSystem())
	{
		audiostreamer = videoplayer->GetSoundSystem()->GetStreamer();
	}
#endif
}




/*-------------------------------------------------------------------
//	VIDEOWINDOWSCODECH264FFMPEG::~VIDEOWINDOWSCODECH264FFMPEG
*/	
/**	
//	
//	 Class Destructor VIDEOWINDOWSCODECH264FFMPEG
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			30/05/2016 9:30:33
//	
*/
/*-----------------------------------------------------------------*/
VIDEOCODECH264FFMPEG::~VIDEOCODECH264FFMPEG()
{
	delete framereader;

	XLIST<MEDIAPACKET*>::XITERATOR it;
	//for(XDWORD i = 0; i < packets.GetSize(); i++)
	for(it = packets.Begin(); it != packets.End(); it++)
		{
			//XDEBUG_PRINTCOLOR(2, __L("PACKET DELETE"));
			MEDIAPACKET* p = *it;
			delete [] p->buffer;
			delete p;
		}
	
	packets.DeleteAll();


	xfactory->DeleteTimer(xtimer);
	xfactory->Delete_Mutex(xmutex);

	EndGL();

	delete Y;
	delete U;
	delete V;

	Clean();
}




/*-------------------------------------------------------------------
//	VIDEOCODECH264FFMPEG::Ini
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			30/05/2016 9:30:40
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool VIDEOCODECH264FFMPEG::Ini()
{
	// use the codec form the framereader to configure the codec
	MEDIAFRAMEREADERLIBAV* avframereader = (MEDIAFRAMEREADERLIBAV*)framereader;
	
	AVCodec* codec = avframereader->GetVideoCodec();
	if(!codec) return false;
	
	 
	
	if(codec->capabilities & CODEC_CAP_TRUNCATED)
		{
			avframereader->GetVideoCodecContext()->flags |= CODEC_FLAG_TRUNCATED;
		}
	
	if(avcodec_open2(avframereader->GetVideoCodecContext(), codec, NULL) < 0)
		{
			XDEBUG_PRINTCOLOR(4, __L("Error Opening AVCodec"));
		}
	
	if(avframereader->GetAudioCodec() != NULL)
		{
			if(avcodec_open2(avframereader->GetAudioCodecContext(), avframereader->GetAudioCodec(), NULL) < 0)
				{
					XDEBUG_PRINTCOLOR(4, __L("Error Opening Audio Codec"));
				}
		}

	int check = codec->capabilities & CODEC_CAP_DELAY;
	
	// init parser form codec id
	//parsercontext = av_parser_init(codec->id);
	
	if(fps > EPSILON)
		{
			frame_delay = (XQWORD)((1.0f/fps) * 1000ull * 1000ull * 1000ull);
			frame_timeout = (xtimer->GetMeasureMicroSeconds()*1000ull) + frame_delay;
		}

	AVCodecContext* audiocontext = avframereader->GetAudioCodecContext();

	if(audiocontext)
		{
			if(audiocontext->channel_layout != AV_CH_LAYOUT_STEREO)
				{
					return false; // or maybe sett he swrcontext to output it
				}

			//swrcontext = swr_alloc_set_opts(NULL, audiocontext->channel_layout, AV_SAMPLE_FMT_S16, audiocontext->sample_rate,
			//														audiocontext->channel_layout, audiocontext->sample_fmt, audiocontext->sample_rate, 0, NULL);
			//
			//int swr_status = swr_init(swrcontext);
		}

	// Initialize the swresampler
	
	

	// need to check for return type
	IniGL();

	XDEBUG_PRINTCOLOR(3, __L("Playing True"));
	isplaying = true;
	initialized = true;

	return true;
}





/*-------------------------------------------------------------------
//	VIDEOCODECH264FFMPEG::End
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			30/05/2016 9:30:48
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool VIDEOCODECH264FFMPEG::End()
{
	return true;
}




/*-------------------------------------------------------------------
//	VIDEOCODECH264FFMPEG::SendFrame
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			30/05/2016 9:30:54
//	
//	@return 			bool : 
//
//  @param				frame : 
*/
/*-----------------------------------------------------------------*/
bool VIDEOCODECH264FFMPEG::SendFrame(XVECTOR<XBYTE> & frame)
{
	return true;
}




/*-------------------------------------------------------------------
//	VIDEOCODECH264FFMPEG::Decode
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			30/05/2016 9:31:01
//	
*/
/*-----------------------------------------------------------------*/
void VIDEOCODECH264FFMPEG::Decode()
{
	
	// Only do this frame has been shown, or not, if not shown we skip

		// if hasn't passed enough time, return
		// Consider how we are computing the frame timeout
	XQWORD now = xtimer->GetMeasureMicroSeconds()*1000ull;
	if(now < frame_timeout)
		{
			//XDEBUG_PRINTCOLOR(4, __L("frame timeout"));
			return;
		}

		
	//if(framereader->IsEOF())
	//	{
	//		eos = true;
	//		isplaying = false;
	//		return; // might as well return, as our whole video finished playing
	//	}

		// Obtain the packet from the list
	xmutex->Lock();
	if((packets.GetSize() == 0) && (!framereader->IsEOF()))
		{
			xmutex->UnLock();
			return;
		}
		
	AVPacket pkt;
	MEDIAPACKET* packet = NULL;
		
	bool manualpktfree = false;
		
	if((packets.GetSize()))
		{
			packet = packets.GetFirst();
			packets.DeleteIndex(0);
	
			av_init_packet(&pkt);
			pkt.data = packet->buffer;
			pkt.size = packet->size;
			pkt.dts = packet->dts;
			pkt.pts = packet->pts;
		
			bufferedsize -= packet->size;
		}
	else
		{
			if(framereader->IsEOF())
				{
					av_init_packet(&pkt);
					pkt.data = NULL;
					pkt.size = 0;
				}
		}
		
	xmutex->UnLock();
		
	int gotpicture = 0;
	int error = 0;
		
	AVFrame* picture = av_frame_alloc();
		
	MEDIAFRAMEREADERLIBAV* avframereader = (MEDIAFRAMEREADERLIBAV*)framereader;

	error = avcodec_decode_video2(avframereader->GetVideoCodecContext(), picture, &gotpicture, &pkt);
	if(error < 0)
		{
			//char errstr[1024];
			//av_make_error_string(errstr, 1024, error);
			
			XDEBUG_PRINTCOLOR(4, __L("Error Codeding frame"));	
		}
		
	//avcodec_send_packet();
		
	// delete allocated data
	av_packet_unref(&pkt);
		
	if(packet)
		{
			delete [] packet->buffer;
			delete packet;
		}
	//
	if(framereader->IsEOF() && (gotpicture == 0))
		{
			XDEBUG_PRINTCOLOR(3, __L("Playing False"));
			eos = true;
			isplaying = false;
			return; // might as well return, as our whole video finished playing
		}
		
	// after decoding we have either a YUV420 or an RGB picture on picture
	if(picture->format == AV_PIX_FMT_YUV420P)
		{
			xmutex->Lock();
			if(!updatetexture)
				{
					width = picture->width;
					height = picture->height;
	
					// check buffer sizes and resize if needed
					if(Y->GetSize() != width*height*sizeof(XBYTE))
						{
							Y->Resize(width*height*sizeof(XBYTE));
						}
		
					if(U->GetSize() != (width/2)*(height/2)*sizeof(XBYTE))
						{
							U->Resize((width/2)*(height/2)*sizeof(XBYTE));
						}
		
					if(V->GetSize() != (width/2)*(height/2)*sizeof(XBYTE))
						{
							V->Resize((width/2)*(height/2)*sizeof(XBYTE));
						}
		
					// copy over framedata to the buffers for textures
					// if linesize is different we must be carefull with padding, copy line by line for now
					if(picture->linesize[0] == width)
						{
							memcpy(Y->Get(), picture->data[0], width*height*sizeof(XBYTE));
						}
					else
						{
							for(XDWORD i = 0; i < height; i++)
								{
									memcpy(Y->Get() + (i*width), picture->data[0] + (i*picture->linesize[0]), width*sizeof(XBYTE));
								}
						}
		
					if(picture->linesize[1] == (width/2))
						{
							memcpy(U->Get(), picture->data[1], (width/2)*(height/2)*sizeof(XBYTE));
						}
					else
						{
							for(XDWORD i = 0; i < (height/2); i++)
								{
									memcpy(U->Get() + (i*(width/2)), picture->data[1] + (i*picture->linesize[1]), (width/2)*sizeof(XBYTE));
								}
						}
		
					if(picture->linesize[2] == (width/2))
						{
							memcpy(V->Get(), picture->data[2], (width/2)*(height/2)*sizeof(XBYTE));
						}
					else
						{
							for(XDWORD i = 0; i < (height/2); i++)
								{
									memcpy(V->Get() + (i*(width/2)), picture->data[2] + (i*picture->linesize[2]), (width/2)*sizeof(XBYTE));
								}
						}
		
					updatetexture = true;
				}
			xmutex->UnLock();
		}
	else
		{
			//XDEBUG_PRINTCOLOR(4, __L("Unknown Picture Format"));
		}
		
	av_frame_free(&picture);
		
		
	// it may take some 'reads' before we can set the fps
  if(frame_timeout == 0 && frame_delay == 0)
		{
			double framerate = 25.0f;
			double dfps = 1.0/framerate;
#if LIBAVCODEC_VERSION_MAJOR <= 56
			dfps = 2.0*av_q2d(avframereader->GetVideoCodecContext()->time_base); // presentation time in seconds
#else
			framerate = av_q2d(avframereader->GetVideoStream()->r_frame_rate); // quick ugly hack that won't be perfect for videos even remotely unregular
			dfps = 1.0/framerate;
#endif
			
			
			if(dfps > 0.0f)
				{
					frame_delay = (XQWORD)(dfps * 1000ull * 1000ull * 1000ull);
					fps = (float)dfps;
				}
		}
	
	
	if(frame_delay > 0)
		{
			frame_timeout = xtimer->GetMeasureMicroSeconds()*1000ull + frame_delay;
		}
}




/*-------------------------------------------------------------------
//	VIDEOCODECH264FFMPEG::Update
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			30/05/2016 9:31:07
//	
*/
/*-----------------------------------------------------------------*/
void VIDEOCODECH264FFMPEG::Update()
{
	//LIBAVPACKET* packet = new LIBAVPACKET;
	//if(!packet) return;
	//
	//packet->buffer = NULL;
	//packet->size = 0;
	//packet->pts = 0;
	//packet->dts = 0;
	//VIDEOH264FRAMEREADERLIBAV * avframereader = (VIDEOH264FRAMEREADERLIBAV*)framereader;
	//if(!avframereader->ReadFrame(packet))
	//{
	//
	//}
	//
	//delete packet->buffer;
	//delete packet;

	// need to read a frame and add it to the packets list
	if(bufferedsize < BUFFEREDSIZEMAX)
		{
			MEDIAPACKET* packet = new MEDIAPACKET;
			if(!packet) return;
	
			packet->buffer = NULL;
			packet->size = 0;
			packet->pts = 0;
			packet->dts = 0;
	
			MEDIAFRAMEREADERLIBAV * avframereader = (MEDIAFRAMEREADERLIBAV*)framereader;
	
			if(!avframereader->ReadFrame(packet))
				{
					delete packet;
				}
			else
				{
					if(xmutex)
						{
							//XDEBUG_PRINTCOLOR(0, __L("Update Lock"));
							if(!packet->buffer)
								{
									// delete the package and don't add it to the list
									delete packet;
								}
							else
								{
									xmutex->Lock();

									if(packet->packettype == MEDIAPACKETTYPE_VIDEO)
										{
											packets.Add(packet);
											bufferedsize += packet->size;
										}
									else if(packet->packettype == MEDIAPACKETTYPE_AUDIO)
										{
											//audiopackets.Add(packet);

											// or maybe send it to openal right away? Not sure how expensive could that be
											// (or if it would even be recomendable to do it in another thread like Decode)

											// decode and send it to the OpenAL stream
											//avframereader->GetCodecContext(), picture, &gotpicture, &pkt);
											AVFrame* audioframe = av_frame_alloc();
											int audioframefinished;

											AVPacket pkt;
											av_init_packet(&pkt);
											pkt.data	= packet->buffer;
											pkt.size	= packet->size;
											pkt.dts		= packet->dts;
											pkt.pts		= packet->pts;

											int len = avcodec_decode_audio4(avframereader->GetAudioCodecContext(), audioframe, &audioframefinished, &pkt);

											//if(len < 0)
											//	{
											//		// ERROR
											//		XDEBUG_PRINTCOLOR(4, __L("ERROR decoding audio, code: %d"), len);
											//	}
											//else
											//	{
											//		XDEBUG_PRINTCOLOR(2, __L("Frame Decoded OK"));
											//	}

#ifdef SND_ACTIVE
											if(audioframefinished)
												{
													// send the decoded buffer to the audio streamer
													if(audiostreamer)
														{
															//audiostreamer->SetAttributes(audioframe->channels, audioframe->sample_rate);
															//
															//// resample the audio to the format that we want
															//
															//XBYTE** buffer = NULL;
															//int buffersize;
															//
															//av_samples_alloc_array_and_samples(&buffer, &buffersize, audioframe->channels, audioframe->nb_samples, AV_SAMPLE_FMT_S16, 0); // 0 = default aligment, 1 = no aligment
															//
															//swr_convert(swrcontext, buffer, buffersize, (const uint8_t**)audioframe->extended_data, audioframe->nb_samples);
															//
															//audiostreamer->AddData(buffersize, *buffer);
														}
												}
#endif

											av_frame_free(&audioframe);

											// free the buffer
											MEDIAPACKET* p = packet;
											delete [] p->buffer;
											delete p;
										}

									xmutex->UnLock();
								}
						}
				}
		}
	else
		{
			//XDEBUG_PRINTCOLOR(4, __L("HAHAHA"));
		}
	
	// update the textures, need a way to signal if textures are avaiblable to update
	xmutex->Lock();
	if(!updatetexture)
		{
			xmutex->UnLock();
			return;
		}

	// we are using YUV420 as picture format
	textureY->Resize(width, height);
	textureU->Resize(width/2, height/2);
	textureV->Resize(width/2, height/2);
	
	// sloooooow
	// need to use double/triple buffering
	textureY->LoadToGPU(Y);
	textureU->LoadToGPU(U);
	textureV->LoadToGPU(V);
	GRPOPENGLCTRL::TEXTURE::CurrentTexture = NULL;
	updatetexture = false;
	
	// video aspect ratio, actually useful when rendering fullscreen video
	if(target == VIDEOPLAYER_TARGET_FULLSCREEN)
		{
			if(width != 0)
				{
					float vaspect = ((float)height)/((float)width);
					float paspect = 1.0f/viewport->GetAspectRatio();
			
					float xs = 1.0f;
					float ys = 1.0f;
			
					if(vaspect <= paspect)
						{
							// perfect for vertical bars or just fit
							ys = vaspect;
						}
					else
						{
							// video is convined and need horizontal bars(ugh ugh ugh)
							// need to touch up xs
							ys = paspect;
							xs = ys/vaspect; // ??
						}
			
					surface->GetNode()->SetScale(xs, ys, 1.0f);
				}
		}
	else if(target == VIDEOPLAYER_TARGET_TEXTURE)
		{
			// nothing really?
			// at the end, we have the VideoRender material created
		}
	

	xmutex->UnLock();
}




/*-------------------------------------------------------------------
//	VIDEOCODECH264FFMPEG::Notify
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			31/05/2016 16:02:20
//	
//	@return 			bool : 
//
//  @param				msg : 
//  @param				notifier : 
*/
/*-----------------------------------------------------------------*/
//bool VIDEOCODECH264FFMPEG::Notify(GRPMSGTYPE msg,GRPSUBJECT* notifier)
//{
//	if(msg == GRPMSG_DESTROYED)
//	{
//		// we are being warned the scene is getting destroyed, take care of it
//		//EndGL();
//	}
//
//	return true;
//}



/*-------------------------------------------------------------------
//	VIDEOCODECH264FFMPEG::IniGL
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			30/05/2016 13:27:00
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool VIDEOCODECH264FFMPEG::IniGL()
{
	textureY = new GRPTEXTUREEXT();
	textureU = new GRPTEXTUREEXT();
	textureV = new GRPTEXTUREEXT();

	if(!textureY || !textureU || !textureV)
		{
			delete textureY;
			delete textureU;
			delete textureV;

			return false;
		}

	textureY->Create(NULL, width,		height,		GRPTEXTURE_FORMAT_R);
	textureU->Create(NULL, width/2, height/2, GRPTEXTURE_FORMAT_R);
	textureV->Create(NULL, width/2, height/2, GRPTEXTURE_FORMAT_R);

		// now create the material and quad to render to the scene
	//GRPMATERIALLIBRARY* matlibrary = videoplayer->GetRenderer()->GetScene(1)->GetMaterialLibrary();
	GRPMATERIALLIBRARY* matlibrary = GRPMATERIALLIBRARY::Get();
	if(!matlibrary) return false;
	mat = new GRPMATERIAL();

	mat->SetName(__L("VideoRender"));
	mat->AddLayer(0, textureY);
	mat->AddLayer(1, textureU);
	mat->AddLayer(2, textureV);

	matlibrary->Add(mat);

	quad = new GRPOBJECT();
	quad->SetNode(this->videoplayer->GetRenderer()->GetScene(1)->CreateNode());

	surface = GRPOBJECTBUILDER::Get()->CreatePlane(GRPOBJECTBUILDER::AXIS_X);
	surface->SetNode(quad->GetNode());

	surface->GetNode()->Init();
	surface->GetNode()->SetMaterial(mat);
	surface->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"VID"));

	quad->AddElement(surface);

	viewport = videoplayer->GetRenderer()->GetScene(1)->GetViewport();

	videoplayer->GetRenderer()->GetScene(1)->Add(quad);
	//videoplayer->GetRenderer()->GetScene(1)->Add(surface);

	
	viewport->SetSize(1.0f, 1.0f);
	viewport->SetPosition(0.0f, 0.0f);
	viewport->SetProjection(GRPPROJECTIONTYPE_ORTHO);
	viewport->SelectProjectionOrtho();
	viewport->Select();

	// Setup the Viewport

	videoplayer->GetRenderer()->GetScene(1)->GetCamera()->SetPosition(0.0f, 0.0f, 140.0f);
	videoplayer->GetRenderer()->GetScene(1)->GetCamera()->GetTarget()->SetPosition(0.0f, 0.0f, 0.0f);

	return true;
}





/*-------------------------------------------------------------------
//	VIDEOCODECH264FFMPEG::EndGL
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			22/06/2016 9:58:53
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool VIDEOCODECH264FFMPEG::EndGL()
{
	// need to take out the video elements from the scene
	// need to move the video destroy to before the renderer destroy
	if(quad)
		{
			delete quad;

			videoplayer->GetRenderer()->GetScene(1)->GetObjects()->Delete(quad);

			quad = NULL;
		}


	// need to destroy all of the graphics elements
	if(mat)
		{
			GRPMATERIALLIBRARY::Get()->Delete(__L("VideoRender"));

			delete mat;
			mat = NULL;
		}

	if(textureY)
		{
			delete textureY;
			delete textureU;
			delete textureV;

			textureY = NULL;
			textureU = NULL;
			textureV = NULL;
		}

	return true;
}
