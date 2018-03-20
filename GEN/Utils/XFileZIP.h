//------------------------------------------------------------------------------------------
//	XFILEZIP.H
//	
/**	
// \class 
//   
//  File fuctions for GIF
//   
//	@author	 Abraham J. Velez
//	@version 27/11/2003 17:24:42
*/	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
	
#ifndef _XFILEZIP_H_
#define _XFILEZIP_H_
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include <zlib.h>
#include <zip.h>
#include <unzip.h>

#include "XBase.h"
#include "XFile.h"	
#include "XPath.h"	


//---- DEFINES & ENUMS  --------------------------------------------------------------------

#define XFILEZIP_MAXBLOCKFILE  1024
	
//---- CLASS -------------------------------------------------------------------------------

class XDATETIME;

class XFILECMPZIP
{
	public:
															XFILECMPZIP								(bool compress, void* filehdl);				
		virtual									 ~XFILECMPZIP								();
	
		bool											Open											(XSTRING& password);
		bool											Create										(XDWORD crcfile, zip_fileinfo* zfinfo, XCHAR* password = NULL);

		bool											IsActive							  	();

		XCHAR*										GetName										();
		void											SetName										(XSTRING& name);

		bool											IsDirectory               ();

		int												Size											();
    int                       UnCompressSize            ();

		XDWORD                    GetCRC32                  ();
    
		bool											GetSeek										(int *position);

		bool											Read											(XBYTE* buffer,int size);
		bool											Read											(XBYTE* buffer,int* size);
		bool											Write											(XBYTE* buffer,int size);

		bool											IsEOF											();
			
		bool											Close											();

	private:

		void											Clean											();
		
		bool											SelectCurrenFile					();

		void*											filehdl;

		bool											on;		
		bool											zipoper; 							
		XSTRING										name;
		XSTRING										comment;
		XDWORD                    method;
		int												uncompresssize;
		int												compresssize;
		XDWORD                    CRC32;
		bool											open;
};


class XFILEZIP 
{
	public:
															XFILEZIP									();				
		virtual									 ~XFILEZIP									();
	
		bool											Open											(XPATH& xpath);		
		
		XFILECMPZIP*							CreateFileCmp							(XPATH& xpath);
		XDWORD										GetFileCRC								(XFILE*	xfile);

		bool											AddFile										(XPATH& xpath,XPATH& xpathnew,XCHAR* password = NULL);		
		bool											Close											();

	private:

		void											Clean											();

		XDATETIME*								xdatetime;
		zipFile										filehdl;

		XPATH											xpathzip;
};




class XFILEUNZIP 
{
	public:
															XFILEUNZIP								();				
		virtual									 ~XFILEUNZIP								();
	
		bool											Open											(XPATH& xpath);		
		
		int												GetNFiles                 ();
		
		XFILECMPZIP*							GotoFile									(bool first=true);				
		XFILECMPZIP*							GotoFile									(XPATH& xpath);			
		XFILECMPZIP*							GotoFile									(int index);

		bool											DelFile										(int index, XCHAR* password = NULL);

		bool											DecompressFile						(int sourceindex ,XPATH& xpathtarget,XCHAR* nametarget = NULL ,XCHAR* password = NULL);
		bool											DecompressFile						(XSTRING& namesource,XPATH& xpathtarget,XCHAR* nametarget = NULL ,XCHAR* password = NULL);
		bool											DecompressFile						(XFILECMPZIP* filecmp,XPATH& xpathtarget,XCHAR* nametarget = NULL,XCHAR* password = NULL);
		
		bool											Close											();

	private:

		void											Clean											();
		XFILECMPZIP*							CreateCurrentFile					();
		
		unzFile										filehdl;

		XPATH											xpathunzip;
};


	
//---- INLINE FUNCTIONS --------------------------------------------------------------------
	
#endif

