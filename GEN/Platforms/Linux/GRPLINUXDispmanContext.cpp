
/*------------------------------------------------------------------------------------------
//	GRPLINUXDISPMANCONTEXT.CPP
//	
//	linux dispman graphics context
//   
//	Author						: Imanol Celaya Ruiz de Alegria
//	Date Of Creation	: 05/04/2016 12:33:16
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "GRPLINUXDispmanContext.h"
#include "GRPLINUXScreenDispman.h"	

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/




/*-------------------------------------------------------------------
//	GRPLINUXDISPMANCONTEXT::GRPLINUXDISPMANCONTEXT
*/	
/**	
//	
//	Class Constructor GRPLINUXDISPMANCONTEXT
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			13/04/2016 17:53:08
//	
*/
/*-----------------------------------------------------------------*/
GRPLINUXDISPMANCONTEXT::GRPLINUXDISPMANCONTEXT() : GRPCONTEXT()
{
}





/*-------------------------------------------------------------------
//	GRPLINUXDISPMANCONTEXT::~GRPLINUXDISPMANCONTEXT
*/	
/**	
//	
//	 Class Destructor GRPLINUXDISPMANCONTEXT
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			13/04/2016 17:53:01
//	
*/
/*-----------------------------------------------------------------*/
GRPLINUXDISPMANCONTEXT::~GRPLINUXDISPMANCONTEXT()
{
}





/*-------------------------------------------------------------------
//	GRPLINUXDISPMANCONTEXT::Create
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			13/04/2016 17:52:55
//	
//	@return 			bool : 
//
//  @param				handle : 
*/
/*-----------------------------------------------------------------*/
bool GRPLINUXDISPMANCONTEXT::Create(void* handle)
{
	EGLBoolean										result;
  EGLint												num_config;

	GRPLINUXSCREENDISPMAN* dispmanscreen = (GRPLINUXSCREENDISPMAN*)this->screen;

	dispmanscreen->Create();



	static const EGLint						attribute_list[]			=	{	EGL_RED_SIZE				, 8,
																													EGL_GREEN_SIZE			, 8,
																													EGL_BLUE_SIZE				, 8,
																													EGL_ALPHA_SIZE			, 8,
																													EGL_SURFACE_TYPE		, EGL_WINDOW_BIT,
																													EGL_DEPTH_SIZE			, 16,
																													EGL_STENCIL_SIZE    , 8,	 
																													EGL_SAMPLE_BUFFERS	, (this->multisampling != 0) ? 1 : 0,
																													EGL_SAMPLES					,	this->multisampling,
																													EGL_NONE
																												};
	
	// get an EGL display connection
  display = eglGetDisplay(EGL_DEFAULT_DISPLAY);


	int major=0,minor=0;
  // initialize the EGL display connection
  result = eglInitialize(display, &major, &minor);
	XDEBUG_PRINTCOLOR(0,__L("EGL version %d.%d"),major,minor);
  
	
  // get an appropriate EGL frame buffer configuration
  result = eglChooseConfig(display, attribute_list, &config, 1, &num_config);
	
			
	EGLint stencilsize=0;
	eglGetConfigAttrib(display,config,EGL_STENCIL_SIZE,&stencilsize);


	surface = eglCreateWindowSurface(display, config, dispmanscreen->GetDispManHandle(), NULL);
	if (surface==EGL_NO_SURFACE) 
	{
			XDEBUG_PRINTCOLOR(4,__L("GRPLINUXSCREENDISPMAN::Init : unable to create surface"));
			return false;
	}
	
	
	// bind the OpenGL API to the EGL
	result = eglBindAPI(EGL_OPENGL_ES_API);
	if(result ==EGL_FALSE)
	{
				XDEBUG_PRINTCOLOR(4,__L("GRPLINUXSCREENDISPMAN::Init : unable to bind api"));
			return false;
	}
	
	static const EGLint						context_attributes[] =	{ EGL_CONTEXT_CLIENT_VERSION, 2,
																													EGL_NONE
																												};
  // create an EGL rendering context
	context = eglCreateContext(display, config, EGL_NO_CONTEXT, context_attributes);
  if(context ==EGL_NO_CONTEXT)
	{
				XDEBUG_PRINTCOLOR(4,__L("GRPLINUXSCREENDISPMAN::Init : unable to create rendering context"));
			return false;
	}

	// connect the context to the surface
	result = eglMakeCurrent(display, surface, surface, context);
	if (result==EGL_FALSE)	
	{
			XDEBUG_PRINTCOLOR(4,__L("GRPLINUXSCREENDISPMAN::Init : unable to select current surface context"));
			return false;
	}
	
	openglctrl.Ini();
	
	  // Switches off alpha blending problem with desktop - is there a bug in the driver?   
	//glColorMask(1.0, 1.0, 1.0, 0);
	XDEBUG_PRINTCOLOR(0,__L("Dispman ready"),dispmanscreen->GetWidth(),dispmanscreen->GetHeight());

	return true;
}





/*-------------------------------------------------------------------
//	GRPLINUXDISPMANCONTEXT::Destroy
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			13/04/2016 17:52:31
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPLINUXDISPMANCONTEXT::Destroy()
{
	XDEBUG_PRINTCOLOR(4, __L("Destroying Context"));
	GRPLINUXSCREENDISPMAN* androidscreen = (GRPLINUXSCREENDISPMAN*)this->screen;


	eglDestroySurface(display, surface);

	// Release OpenGL resources
	 eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT );
	 eglDestroyContext(display, this->context);
	  
  eglTerminate(display);

	return true;
}





/*-------------------------------------------------------------------
//	GRPLINUXDISPMANCONTEXT::IsLost
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			13/04/2016 17:52:39
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPLINUXDISPMANCONTEXT::IsLost()
{
	return eglGetCurrentContext() == EGL_NO_CONTEXT;
}





/*-------------------------------------------------------------------
//	GRPLINUXDISPMANCONTEXT::Update
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			13/04/2016 17:52:45
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPLINUXDISPMANCONTEXT::Update()
{
	eglSwapBuffers(display, surface);
	return true;
}
