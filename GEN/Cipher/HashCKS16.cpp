
/*------------------------------------------------------------------------------------------
//	HASHCKS16.CPP
//	
//	computes a TCP checksum
//   
//	Author						: Imanol Celaya Ruiz de Alegria
//	Date Of Creation	: 17/02/2016 11:34:15
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "XBuffer.h"
#include "XFactory.h"

#include "HashCKS16.h"
	
#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/





/*-------------------------------------------------------------------
//	HASHCKS16::HASHCKS16
*/	
/**	
//	
//	Class Constructor HASHCKS16
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			17/02/2016 12:00:00
//	
*/
/*-----------------------------------------------------------------*/
HASHCKS16::HASHCKS16() : HASH()
{
	Clean();

	type = HASHTYPE_CKS16;

	resulttmp = new XBUFFER();
	
	XWORD value = 0x0000;
	resulttmp->Add((XWORD)value);

	Ini();
}





/*-------------------------------------------------------------------
//	HASHCKS16::~HASHCKS16
*/	
/**	
//	
//	 Class Destructor HASHCKS16
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			17/02/2016 11:59:54
//	
*/
/*-----------------------------------------------------------------*/
HASHCKS16::~HASHCKS16()
{
		if(resulttmp) 
		{
			delete resulttmp;
			resulttmp = NULL;
		}

	Clean();
}





/*-------------------------------------------------------------------
//	HASHCKS16::Do
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			17/02/2016 11:59:48
//	
//	@return 			bool : 
//
//  @param				input : 
//  @param				size : 
*/
/*-----------------------------------------------------------------*/
bool HASHCKS16::Do(XBYTE* input, int size)
{
	if(!size) return false;

	XWORD	value = 0;
	
	resulttmp->Extract(value);

	for(int c=0;c<size;c++)
		{				
			value = Update(value, (*(XWORD*)input));
			input = input + sizeof(XWORD); // need to offset the input by 16 bits, as we are adding 16 by 16
		}

	value = ~value;

	resulttmp->Add((XWORD)value);

	return true;
}




/*-------------------------------------------------------------------
//	HASHCKS16::ResetResult
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			17/02/2016 11:59:40
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool HASHCKS16::ResetResult()
{
	HASH::ResetResult();

	resulttmp->Delete();

	XWORD value = 0x0000;
	resulttmp->Add((XWORD)value);

	return true;
}





/*-------------------------------------------------------------------
//	HASHCKS16::GetDefaultSize
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			17/02/2016 11:59:33
//	
//	@return 			int : 
//
*/
/*-----------------------------------------------------------------*/
int HASHCKS16::GetDefaultSize()
{
	return sizeof(XWORD);
}




/*-------------------------------------------------------------------
//	HASHCKS16::GetResult
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			17/02/2016 11:59:27
//	
//	@return 			XBUFFER* : 
//
*/
/*-----------------------------------------------------------------*/
XBUFFER* HASHCKS16::GetResult()
{
	GetResultCKS16();

	return result;
}




/*-------------------------------------------------------------------
//	HASHCKS16::GetResult
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			17/02/2016 11:59:21
//	
//	@return 			XBYTE* : 
//
//  @param				resultsize : 
*/
/*-----------------------------------------------------------------*/
XBYTE* HASHCKS16::GetResult(int& resultsize)
{
	GetResultCKS16();

	resultsize = result->GetSize();

	return result->Get();
}




/*-------------------------------------------------------------------
//	HASHCKS16::GetResultCKS16
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			17/02/2016 11:59:16
//	
//	@return 			XWORD : 
//
*/
/*-----------------------------------------------------------------*/
XWORD HASHCKS16::GetResultCKS16()
{
	XWORD value = 0;

	result->Delete();

	resulttmp->Extract(value);

	value ^= 0x0000;
	
	result->Add((XWORD)value);

	return value;
}



/*-------------------------------------------------------------------
//	HASHCKS16::Ini
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			17/02/2016 11:59:10
//	
*/
/*-----------------------------------------------------------------*/
void HASHCKS16::Ini(void)
{
}



/*-------------------------------------------------------------------
//	HASHCKS16::Update
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			17/02/2016 11:59:04
//	
//	@return 			XWORD : 
//
//  @param				CKS16 : 
//  @param				c : 
*/
/*-----------------------------------------------------------------*/
XWORD HASHCKS16::Update(XWORD CKS16, XWORD c)
{
	XDWORD tmp = (XDWORD)CKS16 + (XDWORD)c;

	// loop while we have a carry
	while(tmp > 0xFFFF)
		{
			XWORD carry = (tmp >> 16) & (0xFFFF);
			tmp = tmp + carry;
		}

	return (XWORD)tmp;
}

