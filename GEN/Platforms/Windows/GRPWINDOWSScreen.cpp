
/*------------------------------------------------------------------------------------------
//	GRPWINDOWSSCREEN.CPP
//	
//	WINDOWS Graphics Screen class
//   
//	Author						: Diego Martinez
//	Date Of Creation	: 13/03/2014 12:32:29
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPWINDOWSScreen.h"


#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//	GRPWINDOWSSCREEN::GRPWINDOWSSCREEN
*/	
/**	
//	
//	Class Constructor GRPWINDOWSSCREEN
//	
//	@author				Abraham J. Velez
//	@version			21/05/2014 15:57:54
//	 
*/
/*-----------------------------------------------------------------*/
GRPWINDOWSSCREEN::GRPWINDOWSSCREEN() : GRPSCREEN()
{
	Clean();		
																						
	RECT rect;		

	type = GRPSCREENTYPE_WINDOWS;

	if(EnumDisplaySettings(NULL,ENUM_CURRENT_SETTINGS, &devmode))
		{
			SetWidth(devmode.dmPelsWidth);
			SetHeight(devmode.dmPelsHeight);	
		}
    
	if(GetClientRect(GetDesktopWindow(),&rect))
		{	
			SetWidth(rect.right);
			SetHeight(rect.bottom);																									
			SetMode(GRPPROPERTYMODE_32_BGRA_8888);
																																															
			SetIsBufferInverse(false);																									
		}
}



/*-------------------------------------------------------------------
// GRPWINDOWSSCREEN::~GRPWINDOWSSCREEN
*/ 
/**
//	Destructor de la Clase GRPWINDOWSSCREEN
//	
//	@author				Diego Martinez
//	@version			13/03/2014 12:35:27
//	
//	/*-----------------------------------------------------------------*/
GRPWINDOWSSCREEN::~GRPWINDOWSSCREEN()
{
	Delete();																							
	Clean();
}							




/*-------------------------------------------------------------------
//	GRPWINDOWSSCREEN::Create
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			27/03/2014 13:27:47
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPWINDOWSSCREEN::Create(void* handle)
{
	wndclass.lpfnWndProc		= DummyWndProc;
	if(!CreateWIN32Window(handle))
		{
			return false;
		}
	return true;
}
																			



/*-------------------------------------------------------------------
//  GRPWINDOWSSCREEN::Update
*/ 
/**
//	Update Window
//	
//	@author				Diego Martinez
//	@version			13/03/2014 12:46:56
//	
//	@return 			bool : éxito
//	/*-----------------------------------------------------------------*/
bool GRPWINDOWSSCREEN::Update()
{	
	if (!hdc) return false;

	//glFlush();	
	SwapBuffers(hdc);
	

	return true;
}
		


/*-------------------------------------------------------------------
//	GRPWINDOWSSCREEN::Delete
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			27/03/2014 13:24:36
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPWINDOWSSCREEN::Delete()
{																						
	if(hwnd) 
		{
			if(IsFullScreen()) ChangeDisplaySettings(NULL,0);

			DestroyWindow(hwnd);																																														
		}		

	isactive = false;

	return true;
}
		




/*-------------------------------------------------------------------
//	GRPWINDOWSSCREEN::Resize
*/	
/**	
//	
//	resizes an already created window
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			18/11/2015 10:53:06
//	
//	@return 			bool : 
//
//  @param				width : 
//  @param				height : 
*/
/*-----------------------------------------------------------------*/
bool GRPWINDOWSSCREEN::Resize(int width, int height)
{
	RECT   rect;
	XDWORD  style;	

	GetClientRect(hwnd,&rect);

	rect.right  = rect.left + width;		//+ 16;
  rect.bottom = rect.top  + height;		//+ 16;
			
	style = GetWindowLong(hwnd, GWL_STYLE);
	
	AdjustWindowRect(&rect,style,false);

  SetWindowPos(hwnd,NULL,0,0, (rect.right-rect.left), (rect.bottom-rect.top) , SWP_NOMOVE | SWP_NOZORDER);

	return UpdateSize(width, height);
}



/*-------------------------------------------------------------------
//	GRPWINDOWSSCREEN::CreateWIN32Window
*/	
/**	
//	
//	creates a windows, but doesn't show it
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			08/01/2016 11:43:33
//	
//	@return 			bool : 
//
//  @param				handle : 
*/
/*-----------------------------------------------------------------*/
bool GRPWINDOWSSCREEN::CreateWIN32Window(void * handle)
{
	// need to move actual windows creation code to it's own function
	// add a function to destroy the window
	XSTRING unistrclassname(classname);

	wndclass.lpszClassName = unistrclassname.Get();
	wndclass.hInstance		 = hinstance;

	RegisterClassEx(&wndclass);																						

	if(IsFullScreen())
		{
			DEVMODE devmode;															

			memset(&devmode,0,sizeof(devmode));	

			devmode.dmSize			 = sizeof(devmode);				
			devmode.dmPelsWidth	 = width;								
			devmode.dmPelsHeight = height;							
			devmode.dmBitsPerPel = GetBitsperPixel();														
			devmode.dmFields		 = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
	
			if(ChangeDisplaySettings(&devmode,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL) return NULL;			
		 
			hwnd=CreateWindow(unistrclassname.Get() ,
												NULL									,
												WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP ,											
												0,0										,
												width,height					,
												NULL									,
												NULL									,
												hinstance							,
												(void*)this);
	
		}
	 else
		{
  		RECT   rect;
    	XDWORD  style;																										

    	GetClientRect(GetDesktopWindow(),&rect);

    	rect.right  = (rect.right - width)/2;
    	rect.bottom = (rect.bottom- height)/2;

    	hwnd=CreateWindowEx(0															, //WS_EX_CLIENTEDGE				, 
													unistrclassname.Get()					,
													NULL													,    												
													WS_OVERLAPPED									, //WS_POPUP|WS_VISIBLE|WS_SYSMENU ,
		  										rect.right,rect.bottom				,
			 										width ,height									,
			    								NULL													,
					    						NULL													,
    											hinstance											,
		  										(void*)this);

    	if(!hwnd) return false;

    	GetClientRect(hwnd,&rect);

			rect.right  = rect.left + width;		//+ 16;
    	rect.bottom = rect.top  + height;		//+ 16;
					
			style = GetWindowLong(hwnd, GWL_STYLE);
			
			AdjustWindowRect(&rect,style,false);
    	SetWindowPos(hwnd,NULL,0,0, (rect.right-rect.left), (rect.bottom-rect.top) , SWP_NOMOVE | SWP_NOZORDER);					
			GetClientRect(hwnd, &rect);
			this->width		= rect.right - rect.left;
			this->height	= rect.bottom - rect.top;
		}
				
	if(!hwnd) return false;
	
	this->handle = (void*)(hwnd);
			
	//ShowWindow(hwnd, SW_SHOW);

	SetForegroundWindow(hwnd);
  
	SetFocus(hwnd);

	hdc=GetDC(hwnd);
	if(!hdc) return false;
																					
	isactive = true;

	return true;
}



/*-------------------------------------------------------------------
//	GRPWINDOWSSCREEN::ChooseValidPixelFormat
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			04/04/2016 17:30:04
//	
*/
/*-----------------------------------------------------------------*/
void GRPWINDOWSSCREEN::ChooseValidPixelFormat()
{
	int										 pixelformat;
																						
	// Define pixel format for our window
	ZeroMemory(&pixelformatdescriptor, sizeof(PIXELFORMATDESCRIPTOR));

	pixelformatdescriptor.nSize				=	sizeof (PIXELFORMATDESCRIPTOR);
	pixelformatdescriptor.nVersion		=	1;
	pixelformatdescriptor.dwFlags			= PFD_DRAW_TO_WINDOW |	PFD_SUPPORT_OPENGL |	PFD_DOUBLEBUFFER;
	pixelformatdescriptor.iPixelType	=	PFD_TYPE_RGBA;
	pixelformatdescriptor.cColorBits	=	GetBitsperPixel(); // Color depth as specified in arguments
	pixelformatdescriptor.cDepthBits	=	16;
	pixelformatdescriptor.iLayerType	=	PFD_MAIN_PLANE;

	pixelformat = ChoosePixelFormat(hdc, &pixelformatdescriptor);
	if(pixelformat)
		{
			// Set pixel format 
			::SetPixelFormat(hdc, pixelformat, &pixelformatdescriptor);
		}
	else
		XDEBUG_PRINTCOLOR(4,__L("Unable to find a suitable pixel format!!"));
}


/*-------------------------------------------------------------------
//	GRPWINDOWSSCREEN::CreateFinalWIN32Window
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona ¿? yo esta no la he hecho
//	@version			30/05/2016 10:03:04
//	
*/
/*-----------------------------------------------------------------*/
void GRPWINDOWSSCREEN::CreateFinalWIN32Window()
{
	// get the correct pixel format with a dummy context created
	
	wndclass.lpfnWndProc		= BaseWndProc;
	this->CreateWIN32Window(NULL);
	ShowWindow(hwnd, SW_SHOW); // to make the window visible now
	// need to reconsider how this is handled

	::SetPixelFormat(hdc, nPixelFormat, &pixelformatdescriptor);
}


/*-------------------------------------------------------------------
//	GRPWINDOWSSCREEN::GetHDC
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			04/04/2016 17:20:58
//	
//	@return 			HDC : 
//
*/
/*-----------------------------------------------------------------*/
HDC GRPWINDOWSSCREEN::GetHDC()
{
	return hdc;
}



/*-------------------------------------------------------------------
//	GRPWINDOWSSCREEN::SetPixelFormat
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			06/04/2016 16:27:35
//	
//  @param				nPixelFormat : 
*/
/*-----------------------------------------------------------------*/
void GRPWINDOWSSCREEN::SetPixelFormat(int nPixelFormat)
{
	this->nPixelFormat = nPixelFormat;
}

/*-------------------------------------------------------------------
//	 GRPWINDOWSSCREEN::HideCursor
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	07/11/2017 13:52:46
//	@return		void : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
void GRPWINDOWSSCREEN::ShowCursor(bool b)
{
		::ShowCursor(b);
}

/*-------------------------------------------------------------------
//  GRPWINDOWSSCREEN::Clean
*/ 
/**
//	
//	
//	@author				Diego Martinez
//	@version			13/03/2014 12:54:19
//	
//	@return 			void : 
//	
/*-----------------------------------------------------------------*/
void	GRPWINDOWSSCREEN::Clean()
{
	classname								= __L("WINDOWSScreen");
	title										= __L(" ");

	wndclass.cbSize					= sizeof(wndclass);
	wndclass.style					= CS_HREDRAW | CS_VREDRAW| CS_OWNDC;
	wndclass.lpfnWndProc		= BaseWndProc;
	wndclass.cbClsExtra			= 0;
	wndclass.cbWndExtra			= 0;
	wndclass.hInstance			= NULL;
	wndclass.hIcon					= NULL;
	wndclass.hCursor				= LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszMenuName		= NULL;
	wndclass.hIconSm				= NULL;

	hinstance								= NULL;
	hwnd										= NULL;
	hdc											= NULL;																								
}



/*-------------------------------------------------------------------
//  LRESULT CALLBACK BaseWndProc
*/ 
/**
//	
//	
//	@author				Diego Martinez
//	@version			13/03/2014 12:54:52
//	
//	@return 			LRESULT : 
//	
/*-----------------------------------------------------------------*/
LRESULT CALLBACK GRPWINDOWSSCREEN::BaseWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg)
		{
			case WM_SYSCOMMAND :	switch(wparam)
															{
																case SC_CLOSE					: PostQuitMessage(XAPPLICATIONEXITTYPE_BYUSER);
																												break;
																case SC_SCREENSAVE		:
																case SC_MONITORPOWER	:	return 0;
															}
														break;

			case WM_ENDSESSION :	PostQuitMessage(XAPPLICATIONEXITTYPE_SOSHUTDOWN);
														break;
			case WM_CLOSE			 :	PostQuitMessage(XAPPLICATIONEXITTYPE_BYUSER);
														break;
			case WM_DESTROY		 :	PostQuitMessage(XAPPLICATIONEXITTYPE_BYUSER);
														break;
		}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}




/*-------------------------------------------------------------------
//	GRPWINDOWSSCREEN::DummyWndProc
*/	
/**	
//	
//	dummy event handler for dummy window
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			08/01/2016 11:43:19
//	
//	@return 			LRESULT : 
//
//  @param				hwnd : 
//  @param				msg : 
//  @param				wparam : 
//  @param				lparam : 
*/
/*-----------------------------------------------------------------*/
LRESULT CALLBACK GRPWINDOWSSCREEN::DummyWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, msg, wparam, lparam);
}



