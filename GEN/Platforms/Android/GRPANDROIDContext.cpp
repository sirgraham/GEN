
/*------------------------------------------------------------------------------------------
//	GRPANDROIDCONTEXT.CPP
//	
//	ANdroid Graphics Context
//   
//	Author						: Imanol Celaya Ruiz de Alegria
//	Date Of Creation	: 04/04/2016 16:19:06
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "GRPANDROIDContext.h"
#include "GRPANDROIDScreen.h"

#include "XDebug.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/






/*-------------------------------------------------------------------
//	GRPANDROIDCONTEXT::GRPANDROIDCONTEXT
*/	
/**	
//	
//	Class Constructor GRPANDROIDCONTEXT
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			06/04/2016 14:30:33
//	
*/
/*-----------------------------------------------------------------*/
GRPANDROIDCONTEXT::GRPANDROIDCONTEXT() : GRPCONTEXT()
{
}






/*-------------------------------------------------------------------
//	GRPANDROIDCONTEXT::~GRPANDROIDCONTEXT
*/	
/**	
//	
//	 Class Destructor GRPANDROIDCONTEXT
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			06/04/2016 14:30:28
//	
*/
/*-----------------------------------------------------------------*/
GRPANDROIDCONTEXT::~GRPANDROIDCONTEXT()
{
}






/*-------------------------------------------------------------------
//	GRPANDROIDCONTEXT::Create
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			06/04/2016 14:30:23
//	
//	@return 			bool : 
//
//  @param				handle : 
*/
/*-----------------------------------------------------------------*/
bool GRPANDROIDCONTEXT::Create(void* handle)
{
	GRPANDROIDSCREEN* androidscreen = (GRPANDROIDSCREEN*)this->screen;

	androidscreen->Create(handle);
	// EGL
	
	// initialize OpenGL ES and EGL
   
  //* Here specify the attributes of the desired configuration.
  //* Below, we select an EGLConfig with at least 8 bits per color
  //* component compatible with on-screen windows
   /*
	static const EGLint		attribute_list[] =	{ EGL_RED_SIZE			, 8,
																							EGL_GREEN_SIZE		, 8,
																							EGL_BLUE_SIZE			, 8,
																							EGL_ALPHA_SIZE		, 8,
																							EGL_SURFACE_TYPE	, EGL_WINDOW_BIT,
																							EGL_NONE
																						};
	*/

  const EGLint attribute_list[] = { 
																		EGL_RENDERABLE_TYPE	,	EGL_OPENGL_ES2_BIT	,
																		EGL_BLUE_SIZE				,   8,
																		EGL_GREEN_SIZE			,	  8,
																		EGL_RED_SIZE				,   8,
																		EGL_DEPTH_SIZE			,  24,
																		EGL_SURFACE_TYPE		,	EGL_WINDOW_BIT,
																		EGL_SAMPLE_BUFFERS, (this->multisampling != 0) ? 1 : 0,
																		EGL_SAMPLES, this->multisampling,
																		EGL_NONE
																 };
   

	EGLint			dummy;
	EGLint			format = WINDOW_FORMAT_RGBA_8888;
  EGLint			nconfig;
	EGLBoolean	result;

	// get an EGL display connection
  display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
  	
  // initialize the EGL display connection
  result = eglInitialize(display, NULL, NULL);
  
  // get an appropriate EGL frame buffer configuration
  result = eglChooseConfig(display, attribute_list, &config, 1, &nconfig);

  // create an EGL rendering context
  //context = eglCreateContext(display, config, EGL_NO_CONTEXT, NULL);
  //if(nconfig <= 0) return false;
		
  // EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
  //* guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
  //* As soon as we picked a EGLConfig, we can safely reconfigure the
  //* ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID.

  if(EGL_FALSE == eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format)) return false;

	ANativeWindow_setBuffersGeometry(androidscreen->GetAndroidHandle(), 0, 0, format);

	surface = eglCreateWindowSurface(display, config, (EGLNativeWindowType)handle, NULL);

  //int attrib_list[3] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
  //context = eglCreateContext(display, config, EGL_NO_CONTEXT, attrib_list);
	//
  //if(eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) return false;

	int width;
	int height;

	eglQuerySurface(display, surface, EGL_WIDTH,  (EGLint*)&width);
  eglQuerySurface(display, surface, EGL_HEIGHT, (EGLint*)&height);

	androidscreen->SetWidth(width);
	androidscreen->SetHeight(height);

	//const char* str = (const char*)glGetString(GL_VERSION);
	//
	//XSTRING xstr(str);
	//
	//XDEBUG_PRINTCOLOR(2, __L("OpenGL Version: %s"), xstr.Get());	
	//XDEBUG_PRINTCOLOR(2, __L("Ini screen  Width: %d, Height: %d")	, width, height);	
	//OPENGLCHECKERROR(__L("Context Creation Error, PreIni"));
	//
	//openglctrl.Ini();	
	//
	//GLint depthBufferBits;
	//glGetIntegerv(GL_DEPTH_BITS, &depthBufferBits );
	//XDEBUG_PRINTCOLOR(3, __L("Depth buffer bits: %d"), depthBufferBits);	

	OPENGLCHECKERROR(__L("OpenGLctrl Error"));

	// Context

	int attrib_list[3] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
	context = eglCreateContext(this->display, this->config, EGL_NO_CONTEXT, attrib_list);

	if(eglMakeCurrent(this->display, this->surface, this->surface, context) == EGL_FALSE) return false;

	const char* str = (const char*)glGetString(GL_VERSION);

	XSTRING xstr(str);

	XDEBUG_PRINTCOLOR(2, __L("OpenGL Version: %s"), xstr.Get());
	XDEBUG_PRINTCOLOR(2, __L("Ini screen  Width: %d, Height: %d")	, androidscreen->GetWidth(), androidscreen->GetHeight());
	OPENGLCHECKERROR(__L("Context Creation Error, PreIni"));

	openglctrl.Ini();

	GLint depthBufferBits;
	glGetIntegerv(GL_DEPTH_BITS, &depthBufferBits );
	XDEBUG_PRINTCOLOR(3, __L("Depth buffer bits: %d"), depthBufferBits);

	OPENGLCHECKERROR(__L("OpenGLctrl Error"));

	return true;
}






/*-------------------------------------------------------------------
//	GRPANDROIDCONTEXT::Destroy
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			06/04/2016 14:30:15
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPANDROIDCONTEXT::Destroy()
{
	GRPANDROIDSCREEN* androidscreen = (GRPANDROIDSCREEN*)this->screen;
	eglDestroyContext(this->display, this->context);
	eglDestroySurface(this->display, this->surface);
	eglTerminate(this->display);
	return true;
}





/*-------------------------------------------------------------------
//	GRPANDROIDCONTEXT::IsLost
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			06/04/2016 14:30:08
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPANDROIDCONTEXT::IsLost()
{
	return false;
	//return eglGetCurrentContext() == EGL_NO_CONTEXT;
}





/*-------------------------------------------------------------------
//	GRPANDROIDCONTEXT::Update
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			06/04/2016 16:33:54
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPANDROIDCONTEXT::Update()
{
	if(!IsLost())
	{
		eglSwapBuffers(display, surface);
	}

	return true;
}


