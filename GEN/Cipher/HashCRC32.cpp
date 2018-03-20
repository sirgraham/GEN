/*------------------------------------------------------------------------------------------
//	HASH.CPP
//	
//	Hash CRC 32 class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 02/03/2013 12:07:38
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include <string.h>

#include "XDebug.h"
#include "XFactory.h"
#include "XBuffer.h"

#include "HashCRC32.h"

#include "XMemory.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

#define  HASHCRC32_INI      0xEDB88320
	

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  HASHCRC32::HASHCRC32
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/03/2013 12:43:10
//	
//	@return 			
*/
/*-----------------------------------------------------------------*/
HASHCRC32::HASHCRC32() : HASH()
{
	Clean();

	type = HASHTYPE_CRC32;

	resulttmp = new XBUFFER();
	
	XDWORD value = 0xffffffff;
	resulttmp->Add((XDWORD)value);

	Ini();	
}



/*-------------------------------------------------------------------
//  HASHCRC32::~HASHCRC32
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/03/2013 12:09:01
//	
//	@return 			
//	*/
/*-----------------------------------------------------------------*/
HASHCRC32::~HASHCRC32()
{
	if(resulttmp) 
		{
			delete resulttmp;
			resulttmp = NULL;
		}

	Clean();
}
		
	



/*-------------------------------------------------------------------
//  HASHCRC32::Do
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			03/03/2013 16:30:03
//	
//	@return 			bool : 
//	@param				input : 
//  @param				size : 
*/
/*-----------------------------------------------------------------*/
bool HASHCRC32::Do(XBYTE* input, int size)
{	
	if(!size) return false;

	XDWORD	value = 0;
	
	resulttmp->Extract(value);

	for(int c=0;c<size;c++)
		{				
			value = Update(value, (*input));	
			input++;
		}

	resulttmp->Add((XDWORD)value);

	return true;
}


/*-------------------------------------------------------------------
//	HASHCRC32::ResetResult
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			21/11/2014 13:48:58
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool HASHCRC32::ResetResult()
{
	HASH::ResetResult();

	resulttmp->Delete();
	XDWORD value = 0xffffffff;
	resulttmp->Add((XDWORD)value);

	return true;
}



/*-------------------------------------------------------------------
//  HASHCRC32::GetDefaultSize
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			21/04/2013 0:54:19
//	
//	@return 			int : 
//	*/
/*-----------------------------------------------------------------*/
int HASHCRC32::GetDefaultSize()
{
	return sizeof(XDWORD);
}



/*-------------------------------------------------------------------
//  HASHCRC32::GetResult
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			03/03/2013 23:30:36
//	
//	@return 			XBUFFER* : 
//	*/
/*-----------------------------------------------------------------*/
XBUFFER* HASHCRC32::GetResult()
{
	GetResultCRC32();

	return result;
}




/*-------------------------------------------------------------------
//  HASHCRC32::GetResult
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			03/03/2013 23:30:44
//	
//	@return 			XBYTE* : 
//	@param				resultsize : 
*/
/*-----------------------------------------------------------------*/
XBYTE* HASHCRC32::GetResult(int& resultsize)
{
	GetResultCRC32();

	resultsize	= result->GetSize();

	return result->Get();
}



/*-------------------------------------------------------------------
//  HASHCRC32::GetResultCRC32
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/03/2013 14:04:01
//	
//	@return 			XDWORD : 
//	*/
/*-----------------------------------------------------------------*/
XDWORD HASHCRC32::GetResultCRC32()
{
	XDWORD value = 0;

	result->Delete();

	resulttmp->Extract(value);

	value ^= 0xffffffff;
	
	result->Add((XDWORD)value);

	return value;
}




/*-------------------------------------------------------------------
//  HASHCRC32::Clean
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/03/2013 12:09:56
//	
//	@return 			void : 
//	*/
/*-----------------------------------------------------------------*/
void HASHCRC32::Clean()
{	
	resulttmp = NULL;
}
	




/*-------------------------------------------------------------------
//  HASHCRC32::Ini
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			03/03/2013 18:45:46
//	
//	@return 			void : 
//	*/
/*-----------------------------------------------------------------*/
void HASHCRC32::Ini()
{	
	XDWORD crc; 
	int		 i;
	int		 j;

  for(i=0; i<256; i++) 
		{
	    crc = (XDWORD) i;

      for(j=0; j<8; j++) 
				{
           if(crc & 0x00000001) 
									crc = ( crc >> 1 ) ^ HASHCRC32_INI;
            else  crc =   crc >> 1;
        }

      table[i] = crc;
    }    
}




/*-------------------------------------------------------------------
//  UpdateCRC32
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			03/03/2013 18:45:51
//	
//	@return 			XDWORD : 
//	@param				crc : 
//  @param				c : 
*/
/*-----------------------------------------------------------------*/
XDWORD HASHCRC32::Update(XDWORD CRC32, XBYTE data) 
{
	XDWORD tmp;		
	XDWORD long_c;

  long_c = 0x000000FF & (XDWORD) data;  
  tmp		 = CRC32 ^ long_c;
    
	CRC32 = (CRC32 >> 8) ^ table[ tmp & 0xff ];

  return CRC32;

} 