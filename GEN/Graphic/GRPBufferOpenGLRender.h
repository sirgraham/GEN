/*------------------------------------------------------------------------------------------ 
//	GRPBUFFEROPENGLRENDER.H
*/	
/**	
// \class 
//   
//  Creates a Render Target Buffer (Render to texture and offscreen rendering)
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 25/06/2014 17:52:52
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPBUFFEROPENGLRENDER_H_
#define _GRPBUFFEROPENGLRENDER_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XBase.h"

#include "GRPOpenGL.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/

class XFACTORY;
class GRPSCENE;
class GRPBITMAP;
class GRPFACTORY;

class GRPBUFFEROPENGLRENDER
{
	public:

															GRPBUFFEROPENGLRENDER			();
		virtual									 ~GRPBUFFEROPENGLRENDER			();

		bool											Ini												(int width, int height, int nchannels = 4);
		
		bool											SetActive									(bool t);

		GRPBITMAP*								GetBitmap									() 
															{
																if(!GetFrameBuffer()) return NULL;
																return bitmap;
															}

		XBYTE*										GetFrameBuffer						();

		bool											End												();

		GRPSCENE*									GetScene									()									{ return this->scene;			}
		void											SetScene									(GRPSCENE* scn)			{ this->scene=scn;				}

		
		GLUINT										textureID;
		
	private:

		void											Clean                    ()
															{																
																isini                 = false;

																width									= 0;
																height								= 0;
																nchannels							= 0;
																size									= 0;

																framebufferID					= 0;
																colorbufferID					= 0;
																depthbufferID					= 0;
													
																scene									= NULL;														
																bitmap								= NULL;
															}	

		bool											CheckFramebuffer			();
	
		
		bool											isini;	
		
		int												width;
		int												height;
		int												nchannels;
		int												size;
		
		GLUINT										framebufferID;
		GLUINT										colorbufferID;
		GLUINT										depthbufferID;

		GRPSCENE*									scene;
		GRPBITMAP*								bitmap;
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

