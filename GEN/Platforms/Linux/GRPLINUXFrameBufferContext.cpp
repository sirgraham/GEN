
/*------------------------------------------------------------------------------------------
//	GRPLINUXFRAMEBUFFERCONTEXT.CPP
//	
//	graphics context for framebuffers
//   
//	Author						: Imanol Celaya Ruiz de Alegria
//	Date Of Creation	: 06/04/2016 12:58:32
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "GRPLINUXFrameBufferContext.h"
#include "GRPLINUXScreenFrameBuffer.h"	

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/




/*-------------------------------------------------------------------
//	GRPLINUXFRAMEBUFFERCONTEXT::GRPLINUXFRAMEBUFFERCONTEXT
*/	
/**	
//	
//	Class Constructor GRPLINUXFRAMEBUFFERCONTEXT
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			06/04/2016 16:34:18
//	
*/
/*-----------------------------------------------------------------*/
GRPLINUXFRAMEBUFFERCONTEXT::GRPLINUXFRAMEBUFFERCONTEXT() : GRPCONTEXT()
{
}







/*-------------------------------------------------------------------
//	GRPLINUXFRAMEBUFFERCONTEXT::~GRPLINUXFRAMEBUFFERCONTEXT
*/	
/**	
//	
//	 Class Destructor GRPLINUXFRAMEBUFFERCONTEXT
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			06/04/2016 16:34:24
//	
*/
/*-----------------------------------------------------------------*/
GRPLINUXFRAMEBUFFERCONTEXT::~GRPLINUXFRAMEBUFFERCONTEXT()
{
}







/*-------------------------------------------------------------------
//	GRPLINUXFRAMEBUFFERCONTEXT::Create
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			06/04/2016 16:34:32
//	
//	@return 			bool : 
//
//  @param				handle : 
*/
/*-----------------------------------------------------------------*/
bool GRPLINUXFRAMEBUFFERCONTEXT::Create(void* handle)
{
	GRPLINUXSCREENFRAMEBUFFER* framebufferscreen = (GRPLINUXSCREENFRAMEBUFFER*)screen;

	static const EGLint						attribute_list[]			=	{	EGL_RED_SIZE			, 8,
																													EGL_GREEN_SIZE		, 8,
																													EGL_BLUE_SIZE			, 8,
																													EGL_ALPHA_SIZE		, 8,
																													EGL_SURFACE_TYPE	, EGL_WINDOW_BIT,
																													EGL_DEPTH_SIZE	  , 16						,
																													EGL_SAMPLE_BUFFERS, (this->multisampling != 0) ? 1 : 0,
																													EGL_SAMPLES, this->multisampling,
																													EGL_NONE
																												};
   
	static const EGLint						context_attributes[] =	{ EGL_CONTEXT_CLIENT_VERSION, 2,
																													EGL_NONE
																												};

	const EGLNativeWindowType native_win = (EGLNativeWindowType) NULL;
  EGLint major, minor;
  EGLConfig											config;
	EGLint												num_config;
	EGLBoolean										result;

	// here to make MESA happy it seems
	// (taken from one of their official demos with the comment "make Mesa/EGL happy")
	setenv("EGL_PLATFORM", "fbdev", 0);

	//this->display = eglGetDisplay((EGLNativeDisplayType) handlefb);
	this->display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

	result = eglInitialize(this->display, &major, &minor);
	XDEBUG_PRINTCOLOR(0,__L("EGL version %d.%d"),major,minor);


	struct fbdev_window nwin;


	nwin.width = framebufferscreen->GetWidth();
	nwin.height = framebufferscreen->GetHeight(),

	result = eglChooseConfig(this->display, attribute_list, &config, 1, &num_config);
	this->context = eglCreateContext(this->display, config, EGL_NO_CONTEXT, context_attributes);
	//this->surface = eglCreateWindowSurface(this->display, config, native_win, NULL);
	this->surface = eglCreateWindowSurface(this->display, config, &nwin, NULL);
	eglMakeCurrent(this->display, this->surface, this->surface, this->context);

	return true;
}







/*-------------------------------------------------------------------
//	GRPLINUXFRAMEBUFFERCONTEXT::Destroy
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			06/04/2016 16:34:42
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPLINUXFRAMEBUFFERCONTEXT::Destroy()
{
	eglDestroyContext(this->display, this->context);
}








/*-------------------------------------------------------------------
//	GRPLINUXFRAMEBUFFERCONTEXT::IsLost
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			06/04/2016 16:34:50
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPLINUXFRAMEBUFFERCONTEXT::IsLost()
{
	return eglGetCurrentContext() == EGL_NO_CONTEXT;
}

