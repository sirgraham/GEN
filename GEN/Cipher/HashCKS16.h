/*------------------------------------------------------------------------------------------
//	HASHCKS16.H
*/	
/**	
// \file 
//   
//  implement the TCP header checksum algorithm
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 17/02/2016 11:18:07
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _HASHCKS16_H_
#define _HASHCKS16_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
#include "Hash.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
class HASHCKS16 : public HASH
{
public:

											HASHCKS16								();
	virtual						 ~HASHCKS16								();

	using								HASH::Do;		
	bool								Do											(XBYTE* input, int size);		

	bool								ResetResult							();

	int									GetDefaultSize					();
	
	XBUFFER*						GetResult								();						
	XBYTE*							GetResult								(int& resultsize);
	
	XWORD								GetResultCKS16					();

protected:

private:
		void							Clean										()
											{
												resulttmp = NULL;
											}

	void								Ini											(void);
	XWORD								Update									(XWORD CKS16, XWORD c);

	XBUFFER*						resulttmp;	
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

