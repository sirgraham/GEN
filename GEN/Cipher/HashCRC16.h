/*------------------------------------------------------------------------------------------
//	HASHCRC16.H
*/	
/**	
// \file 
//   
//  Computes the CRC16 of a buffer
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 17/02/2016 9:47:13
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _HASHCRC16_H_
#define _HASHCRC16_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
#include "Hash.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
class HASHCRC16 : HASH
{
public:

											HASHCRC16								();
	virtual						 ~HASHCRC16								();

	using								HASH::Do;		
	bool								Do											(XBYTE* input, int size);		

	bool								ResetResult							();

	int									GetDefaultSize					();
	
	XBUFFER*						GetResult								();						
	XBYTE*							GetResult								(int& resultsize);
	
	XWORD								GetResultCRC16					();

protected:

private:
	void								Clean										()
											{
												resulttmp = NULL;
											}

	void								Ini											(void);
	XWORD								Update									(XWORD CRC16, XBYTE c);

	XWORD								table[256];
	XBUFFER*						resulttmp;	
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

