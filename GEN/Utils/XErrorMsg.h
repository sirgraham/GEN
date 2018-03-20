//------------------------------------------------------------------------------------------
//	XERRORMSG.H
//	
/**	
// \class XERRORMSG
//
//  
//   
//	@author	 Abraham J. Velez
//	@version 28/05/2001
*/	
//	
//	GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------
	
#ifndef _XERRORMSG_H_
#define _XERRORMSG_H_
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>	

#include "XBase.h"
#include "XVector.h"
#include "XString.h"
	
//---- DEFINES & ENUMS  --------------------------------------------------------------------

enum XERRORMSG_RETURN
{
	XERRORMSG_RETURN_NONE   = 0x00 ,
	XERRORMSG_RETURN_OK			= 0x01 ,
	XERRORMSG_RETURN_CANCEL = 0x02 ,
	XERRORMSG_RETURN_RETRY  = 0x04 ,
};

//---- CLASS -------------------------------------------------------------------------------

class XERRORMSG
{
	public:
														    XERRORMSG						  ();
														    XERRORMSG						  (int code,XCHAR* title,XCHAR* msg);
		virtual								     ~XERRORMSG						  ();

		XERRORMSG*							    Get								    ();
		int											    GetCode						    ();
		int											    GetLevel						  ();
		int											    GetImage						  ();
    int											    GetSound						  ();
		XSTRING*										GetTitle						  ();
		XSTRING*										GetMsg							  ();

		bool										    Set								    (int code,XCHAR* title,XCHAR* msg);
		bool										    Set								    (int code,int level,int image,int sound,XCHAR* title,XCHAR* msg);
		bool										    SetCode						    (int code);
		bool										    SetLevel						  (int level);
		bool										    SetImage						  (int image);
    bool										    SetSound   				    (int sound);

		bool										    SetTitle						  (XCHAR* title);
		bool										    SetTitle						  (XSTRING& title);

		bool										    SetMsg							  (XCHAR* msg);
		bool										    SetMsg							  (XSTRING& msg);

	private:

		bool										    Clean								  ();

		int											    code;
		int											    level;
		int											    image;
		int                         sound;
		XSTRING											title;
		XSTRING											msg;

};



class XERRORSMSG
{
	public:
                                XERRORSMSG					  ();
														    XERRORSMSG					  (XCHAR* titleApp,XCHAR* version);
		virtual			     					 ~XERRORSMSG					  ();


    XSTRING*										GetTitleApplication   ();
    XSTRING*										GetVersion            ();

		bool                        SetTitleApplication   (XCHAR* titleapp);
    bool                        SetTitleApplication   (XSTRING& titleapp);

    bool                        SetVersion            (XCHAR* version);
		bool                        SetVersion            (XSTRING& version);

		XERRORMSG*							    FindMsg      			    (int code);
		bool										    AddMsg     				    (int code,int level,int image,int sound,XCHAR* title,XCHAR* msg);
		bool										    EraseMsg       		    (int code);
		bool										    EraseAllMsg      	    ();

		virtual XERRORMSG_RETURN    ShowMsg    				    (int code,bool statusline=false,XBYTE returns=0x01,XCHAR* addstr=NULL,bool sound=false);

	private:

    bool                        Clean                 ();

  protected:
  
    XSTRING											titleapp;
    XSTRING											version;
		XVECTOR<XERRORMSG*>			    errors;

};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

	
#endif

