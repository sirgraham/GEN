
/*------------------------------------------------------------------------------------------
//	GRPANDROIDSCREEN.CPP
//	
//	
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 22/04/2014 17:53:23
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XDebug.h"
#include "ANDROIDMain.h"

#include "GRPANDROIDScreen.h"

#include "XMemory.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

extern ANDROIDMAIN		androidmain;

//---- CLASS -------------------------------------------------------------------------------


/*-------------------------------------------------------------------
//	GRPANDROIDSCREEN::GRPANDROIDSCREEN
*/	
/**	
//	
//	Class Constructor GRPANDROIDSCREEN
//	
//	@author				Abraham J. Velez
//	@version			22/04/2014 17:53:53
//	
*/
/*-----------------------------------------------------------------*/
GRPANDROIDSCREEN::GRPANDROIDSCREEN( ): GRPSCREEN()
{													
	Clean();

	type = GRPSCREENTYPE_ANDROID;

	isvalid = true;
}




/*-------------------------------------------------------------------
//	GRPANDROIDSCREEN::~GRPANDROIDSCREEN
*/	
/**	
//	
//	 Class Destructor GRPANDROIDSCREEN
//	
//	@author				Abraham J. Velez
//	@version			22/04/2014 17:54:17
//	
*/
/*-----------------------------------------------------------------*/
GRPANDROIDSCREEN::~GRPANDROIDSCREEN()
{
	Delete();											
	Clean();
}



/*-------------------------------------------------------------------
//	GRPANDROIDSCREEN::Create
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			22/04/2014 17:54:36
//	
//	@return 			bool : 
//
//  @param				handle : 
*/
/*-----------------------------------------------------------------*/
bool GRPANDROIDSCREEN::Create(void* _handle)
{	
	this->handle = (ANativeWindow*)_handle;																																																													
			
	//isactive = true;
	//		
  //// initialize OpenGL ES and EGL
  // 
  ////* Here specify the attributes of the desired configuration.
  ////* Below, we select an EGLConfig with at least 8 bits per color
  ////* component compatible with on-screen windows
  // /*
	//static const EGLint		attribute_list[] =	{ EGL_RED_SIZE			, 8,
	//																						EGL_GREEN_SIZE		, 8,
	//																						EGL_BLUE_SIZE			, 8,
	//																						EGL_ALPHA_SIZE		, 8,
	//																						EGL_SURFACE_TYPE	, EGL_WINDOW_BIT,
	//																						EGL_NONE
	//																					};
  // 
	//
	//
	//*/
	//
  //const EGLint attribute_list[] = { 
	//																	EGL_RENDERABLE_TYPE	,	EGL_OPENGL_ES2_BIT	,
	//																	EGL_BLUE_SIZE				,   8,
	//																	EGL_GREEN_SIZE			,	  8,
	//																	EGL_RED_SIZE				,   8,
	//																	EGL_DEPTH_SIZE			,  24,
	//																	EGL_SURFACE_TYPE		,	EGL_WINDOW_BIT,
	//																	EGL_SAMPLE_BUFFERS, (this->multisampling != 0) ? 1 : 0,
	//																	EGL_SAMPLES, this->multisampling,
	//																	EGL_NONE
	//															 };
  // 
	//
	//EGLint			dummy;
	//EGLint			format = WINDOW_FORMAT_RGBA_8888;
  //EGLint			nconfig;
	//EGLBoolean	result;
	//
	//// get an EGL display connection
  //display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
  //	
  //// initialize the EGL display connection
  //result = eglInitialize(display, NULL, NULL);
  //
  //// get an appropriate EGL frame buffer configuration
  //result = eglChooseConfig(display, attribute_list, &config, 1, &nconfig);
	//
  //// create an EGL rendering context
  ////context = eglCreateContext(display, config, EGL_NO_CONTEXT, NULL);
  ////if(nconfig <= 0) return false;
	//	
  //// EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
  ////* guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
  ////* As soon as we picked a EGLConfig, we can safely reconfigure the
  ////* ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID.
	//
  //if(EGL_FALSE == eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format)) return false;
	//
  //ANativeWindow_setBuffersGeometry(handle, 0, 0, format);
	//
  //surface = eglCreateWindowSurface(display, config, handle, NULL);
	//
  ////int attrib_list[3] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
  ////context = eglCreateContext(display, config, EGL_NO_CONTEXT, attrib_list);
	////
  ////if(eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) return false;
	//
	//eglQuerySurface(display, surface, EGL_WIDTH,  (EGLint*)&width);
  //eglQuerySurface(display, surface, EGL_HEIGHT, (EGLint*)&height);
	//
	////const char* str = (const char*)glGetString(GL_VERSION);
	////
	////XSTRING xstr(str);
	////
	////XDEBUG_PRINTCOLOR(2, __L("OpenGL Version: %s"), xstr.Get());	
	////XDEBUG_PRINTCOLOR(2, __L("Ini screen  Width: %d, Height: %d")	, width, height);	
	////OPENGLCHECKERROR(__L("Context Creation Error, PreIni"));
	////
	////openglctrl.Ini();	
	////
	////GLint depthBufferBits;
	////glGetIntegerv(GL_DEPTH_BITS, &depthBufferBits );
	////XDEBUG_PRINTCOLOR(3, __L("Depth buffer bits: %d"), depthBufferBits);	
	//
	//OPENGLCHECKERROR(__L("OpenGLctrl Error"));

	return true;
}


/*-------------------------------------------------------------------
//	GRPANDROIDSCREEN::Update
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			22/04/2014 17:56:40
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPANDROIDSCREEN::Update()
{
	//if(!handle) return false;

	//eglSwapBuffers(display, surface);
				
	//XDEBUG_PRINTCOLOR(1,__L("GRPANDROIDSCREEN: Update "));
		
	return true;
}



/*-------------------------------------------------------------------
//	GRPANDROIDSCREEN::Delete
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			22/04/2014 17:56:53
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPANDROIDSCREEN::Delete()
{																						
	handle = NULL;
	isactive = false;

	return true;	
}



/*-------------------------------------------------------------------
//	GRPANDROIDSCREEN::UpdateSize
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			21/05/2014 12:28:29
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPANDROIDSCREEN::UpdateSize()
{
	/*
	eglQuerySurface(display, surface, EGL_WIDTH,  (EGLint*)&width);
  eglQuerySurface(display, surface, EGL_HEIGHT, (EGLint*)&height);

	UpdateViewports(width, height);

	XDEBUG_PRINTCOLOR(2, __L("Change screen  Width: %d, Height: %d")	, width, height);	
	*/
	return true;
}



/*-------------------------------------------------------------------
//	GRPANDROIDSCREEN::Clean
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			22/04/2014 17:57:21
//	
*/
/*-----------------------------------------------------------------*/
void GRPANDROIDSCREEN::Clean()
{
	handle	= NULL;																						
}
				
