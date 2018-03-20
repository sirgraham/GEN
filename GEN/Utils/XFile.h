//------------------------------------------------------------------------------------------
//	XFILE.H
//	
/**	
// \class 
//   
//  XFILE
//   
//	@author	 Abraham J. Velez
//	@version 13/03/2002
*/	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#ifndef _XFILE_H_
#define _XFILE_H_
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "XVector.h"
#include "XPath.h"
#include "XBuffer.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

enum
{
	XFILE_SEEKINI	  =  0,
	XFILE_SEEKEND	  = -1,
};


#define XFILE_MAXBUFFER			8192 

#ifdef XDEBUG
	#define XFILE_DISPLAYNOTCLOSEFILES		XFileOpenList.DisplayAll();
#else
	#define XFILE_DISPLAYNOTCLOSEFILES
#endif


#define XFILE_GETFILESIZE(xpathfile, size)		{ size = -1;																		\
																								XFILE* xfile = xfactory->Create_File();				\
																								if(xfile)																			\
																									{                                           \
																										if(xfile->Open(xpathfile))                \
																											{                                       \
																												sizefile = xfile->GetSize();          \
																												xfile->Close();                       \
																											}                                       \
																										xfactory->Delete_File(xfile);             \
																									}                                           \
																							}


//---- CLASS -------------------------------------------------------------------------------

class CIPHER;
class XFACTORY;

class XFILE 
{
	public:
														XFILE									();
		virtual								 ~XFILE									();
		
		virtual bool						Exist									(XCHAR* xpath)																							= 0;

		virtual bool						Open									(XCHAR* xpath, bool isreadonly = true)											= 0;
		virtual bool						Create								(XCHAR* xpath)																							= 0;	 
		
		int											GetSize								()																			{ return cachesize;																		}
		virtual bool						SetSize								(int size)                                                  = 0;
		virtual bool						GetPosition						(int& position)                                             = 0;
		virtual bool						SetPosition						(int position)                                              = 0;
		
		virtual bool						Read									(XBYTE* buffer, int size , CIPHER* cipher = NULL)					  = 0;
		virtual bool						Read									(XBYTE* buffer, int* size, CIPHER* cipher = NULL)						= 0;

		virtual bool						Write				 					(XBYTE* buffer, int size , CIPHER* cipher = NULL)						= 0;
						bool						Write				 					(XBUFFER &xbuffer)											{ return Write(xbuffer.Get(), xbuffer.GetSize());    }

		virtual bool						Flush									()																													= 0;

		virtual bool						Close									()																			{ return false;																			 }

		virtual bool						Erase									(XCHAR* xpath, bool overwrite = false)											= 0;
		virtual bool						Rename								(XCHAR* xpathold, XCHAR* xpathnew)													= 0;
		
		virtual FILE*						GetFileStructHandle		()																													= 0;	

		bool										Exist									(XPATH& xpath)													{ return Exist(xpath.Get());													}
		
		bool										Open									(XPATH& xpath,bool readonly = true)			{ return Open(xpath.Get(), readonly);									} 
		bool										Create								(XPATH& xpath)													{ return Create(xpath.Get());													}		
		
		bool										Erase									(XPATH& xpath,bool overwrite = false)		{ return Erase(xpath.Get(), overwrite);								}
		bool										Rename								(XPATH& xpathold,XPATH& xpathnew)				{ return Rename(xpathold.Get(), xpathnew.Get());			}
		bool										Rename								(XCHAR* xpathold,XPATH& xpathnew)				{ return Rename(xpathold, xpathnew.Get());						}
		bool										Rename								(XPATH& xpathold,XCHAR* xpathnew)				{ return Rename(xpathold.Get(), xpathnew);						}			
		
		bool										IsOpen      					()																			{ return isopen;																			}
		bool										IsReadOnly   					()																			{ return isreadonly;																	}	
		XCHAR*									GetPathNameFile				()																			{ return xpathnamefile.Get();													}	
		
		bool										EraseBlock						(int size);
		bool										InsertBlock						(unsigned char* buffer,int size);
		
		virtual void						ChangeAttributes			(XCHAR* attributes)											{																											}

	protected:
		
		virtual bool						ActualizeSize					()																													= 0;			
		bool										OverwriteContent			(XCHAR* xpath);		

		bool										isopen;
		bool										isreadonly;
		XPATH										xpathnamefile;
		int											cachesize;

	private:

		void										Clean									()
														{
															isopen				= false;
															isreadonly		= false;
															cachesize			= 0;
														}
};



class XFILECONTAINER
{
  public:
														XFILECONTAINER				()																			{  Clean();																					 }
		virtual								 ~XFILECONTAINER				()                                      {  Clean();																					 }

    bool										CreatePrimaryFile			();
    XFILE*									GetPrimaryFile				();
    bool										DeletePrimaryFile			();

	protected:
	
		XFILE*									file;
    bool										eraseprimaryfile;

	private:
	
		void										Clean									()
														{
															file							= NULL;
															eraseprimaryfile	= false;
														}
};


#ifdef XDEBUG
class XFILEOPENLIST
{
	public:

		virtual								 ~XFILEOPENLIST					();

		bool										Add										(XFILE* xfile);
		bool										Del										(XFILE* xfile);

		bool										CloseAll							();
		bool										DisplayAll						();

	private:

		XVECTOR<XFILE*>					filelist;
};



extern XFILEOPENLIST	XFileOpenList;

#endif



//---- INLINE FUNCTIONS --------------------------------------------------------------------


#endif

