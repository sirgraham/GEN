
/*------------------------------------------------------------------------------------------
//	GRPLINUXSCREENDISPMAN.CPP
//	
//	LINUX graphics screen class
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 18/03/2014 10:39:29
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <unistd.h>
#include <iostream>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include "XBase.h"
#include "XDebug.h"

#include "GRPLINUXScreenDispman.h"

#include "XMemory.h"

	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//	GRPLINUXSCREENDISPMAN::GRPLINUXSCREENDISPMAN
*/	
/**	
//	
//	Class Constructor GRPLINUXSCREENDISPMAN
//	
//	@author				Abraham J. Velez
//	@version			21/05/2014 15:59:15
//	
 
 
*/
/*-----------------------------------------------------------------*/
GRPLINUXSCREENDISPMAN::GRPLINUXSCREENDISPMAN() : GRPSCREEN()
{
	Clean();			

	type = GRPSCREENTYPE_LINUX_DISPMAN;
}


/*-------------------------------------------------------------------
//	GRPLINUXSCREENDISPMAN::~GRPLINUXSCREENDISPMAN
*/	
/**	
//	
//	 Class Destructor GRPLINUXSCREENDISPMAN
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			18/03/2014 10:46:12
//	
*/
/*-----------------------------------------------------------------*/
GRPLINUXSCREENDISPMAN::~GRPLINUXSCREENDISPMAN()
{
	Delete();

	Clean();	 
}



/*-------------------------------------------------------------------
//	GRPLINUXSCREENDISPMAN::Create
*/	
/**	
//	
//	Creates the buffer
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			18/03/2014 10:46:32
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPLINUXSCREENDISPMAN::Create(void* handle)
{																							
	isactive = true;	
						  
  //XDEBUG_PRINTCOLOR(0,__L("Dispman: Creating"));
	
	bcm_host_init();
   	
	Init();

	return true;
}




/*-------------------------------------------------------------------
//	GRPLINUXSCREENDISPMAN::Update
*/	
/**	
//	
//	refreshes the buffer on screen
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			18/03/2014 10:47:12
//	
//	@return 			bool : true = success
//
*/
/*-----------------------------------------------------------------*/
bool GRPLINUXSCREENDISPMAN::Update()
{					
	return true;
}



/*-------------------------------------------------------------------
//	GRPLINUXSCREENDISPMAN::Delete
*/	
/**	
//	
//	Sets the Buffer has inactive for destruction
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			18/03/2014 10:47:45
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPLINUXSCREENDISPMAN::Delete()
{																																																																		
	isactive = false;


	vc_dispmanx_element_remove(dispman_update,dispman_display);
	vc_dispmanx_display_close(this->dispman_display);
	vc_dispmanx_stop();
	bcm_host_deinit();
	return true;
}


EGL_DISPMANX_WINDOW_T* GRPLINUXSCREENDISPMAN::GetDispManHandle()
{
	return &this->handle;
}



/*-------------------------------------------------------------------
//	GRPLINUXSCREENDISPMAN::Clean
*/	
/**	
//	
//	Disables the handle and cleans
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			18/03/2014 10:48:05
//	
*/
/*-----------------------------------------------------------------*/
void GRPLINUXSCREENDISPMAN::Clean()
{
	
}




/*-------------------------------------------------------------------
//	GRPLINUXSCREENDISPMAN::Init
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			16/04/2014 16:52:02
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPLINUXSCREENDISPMAN::Init()
{

	int32_t												success = 0;


  
  	
	

	
	unsigned int currentWidth=0,currentHeight=0;
  // create an EGL window surface

	if (isfullscreen)	
	{
  success = graphics_get_display_size(0 /* LCD*/, &width, &height);	
	positionx=positiony=0;
	}
	

	XDEBUG_PRINTCOLOR(0,__L("EGL %d,%d"),width,height);

	//success = graphics_get_display_size(0 /* LCD*/, &width, &height);	
	XDEBUG_PRINTCOLOR(0,__L("Creating dispman with %d,%d"),width,height);
	

	dst_rect.x			= positionx;
  dst_rect.y			= positiony;	
  dst_rect.width	= width;
  dst_rect.height = height;
      
  src_rect.x			= 0;
  src_rect.y			= 0;
 // src_rect.width	= width		<< 16;
 // src_rect.height = height	<< 16;  
	 src_rect.width	= width		<< 16;
   src_rect.height = height	<< 16;  


    VC_DISPMANX_ALPHA_T alpha =
    {
        DISPMANX_FLAGS_ALPHA_FIXED_ALL_PIXELS, 
        255, /*alpha 0->255*/
        0
    }; 

  dispman_display = vc_dispmanx_display_open(0/*LCD*/);
  dispman_update	= vc_dispmanx_update_start(0);         
  dispman_element = vc_dispmanx_element_add(dispman_update, 
																						dispman_display, 
																						0/*layer*/, 
																						&dst_rect, 
																						0/*src*/, 
																						&src_rect, 
																						DISPMANX_PROTECTION_NONE,  
																						&alpha,//0 /*alpha*/, 
																						0/*clamp*/, 
																						(DISPMANX_TRANSFORM_T)0/*transform*/);
  

  handle.element	= dispman_element;
  handle.width		= width;
  handle.height		= height;

  vc_dispmanx_update_submit_sync( dispman_update );


  

 
return true;
}

