//------------------------------------------------------------------------------------------
//	GRPLINUXSCREENX11.H
//	
/**	
// \class 
//   
//  LINUX graphics screen class
//   
//	@author	 Abraham J. Velez
//	@version 28/06/2002
*/	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#ifndef _GRPLINUXSCREENX11_H_
#define _GRPLINUXSCREENX11_H_
	
	
//---- INCLUDES ----------------------------------------------------------------------------


#include "XDebug.h"

#include "GRPOpenGL.h"
#include "GRPScreen.h"
	
//---- DEFINES & ENUMS  --------------------------------------------------------------------


typedef struct
{
   unsigned long   flags;
   unsigned long   functions;
   unsigned long   decorations;
   long            inputMode;
   unsigned long   status;

} HINTS;



//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XPUBLISHER;

class GRPLINUXSCREENX11 : public GRPSCREEN
{
	public:	
																					GRPLINUXSCREENX11						();
		virtual															 ~GRPLINUXSCREENX11						();
		

		void																	SetIsDesktop								(bool isdesktop)												{ this->isdesktop = isdesktop;	}
		bool																	IsDesktop										()																			{ return this->isdesktop;				}

		virtual bool													Create											(void* handle = NULL);
		virtual bool													Update											() {return true;};
		bool																	Delete											();
	
		Display*															GetDisplay									();
		Window*																GetWindow										();
		XVisualInfo**													GetVisualInfo								();
		Window*																GetRootWindow								();

		bool																	Resize											(int width, int height);
		bool																	HasFocus										();

		bool																	OpenDisplay									();
		bool																	CreateX11Window							();

		void																	ShowCursor									(bool);

protected:

		Display*															display;
		Window																window;	
		XVisualInfo*													vi;
		Window											          root;

		bool																	isdesktop;

	private:

		void																	Clean												();	
};


	
//---- INLINE FUNCTIONS --------------------------------------------------------------------
	
#endif



