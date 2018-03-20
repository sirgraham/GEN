/*------------------------------------------------------------------------------------------
//	HASHCOMP128V1.H
*/	
/**	
// \class 
//   
//  Hash COMP128 (version 1) Class
//   
//	@author	 Abraham J. Velez
//	@version 02/03/2013 12:02:40
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _HASHCOMP128V1_H_
#define _HASHCOMP128V1_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "Hash.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

#define HASHCOMP128V1_RAND_SIZE			16
#define HASHCOMP128V1_KI_SIZE				16

#define HASHCOMP128V1_SRES_SIZE			4
#define HASHCOMP128V1_KC_SIZE				8
#define HASHCOMP128V1_RESULT_SIZE		(HASHCOMP128V1_SRES_SIZE + HASHCOMP128V1_KC_SIZE)

/*---- CLASS -----------------------------------------------------------------------------*/


class HASHCOMP128V1 : public HASH
{
	public:
													HASHCOMP128V1							();
		virtual			 				 ~HASHCOMP128V1							();

		bool									SetInput									(XBYTE* rand, XBYTE* ki, XBUFFER& xbuffer);
		
		using									HASH::Do;	
		bool									Do												(XBYTE* input, int size);	

		int										GetDefaultSize						();

		bool									GetSRES										(XBUFFER& SRES);
		bool									GetKc											(XBUFFER& Kc);
								
	private:
 
		
		void									Clean											();	

		void									Make											(XBYTE* input);

		static const XBYTE		table0[512];
		static const XBYTE		table1[256];
		static const XBYTE		table2[128]; 
		static const XBYTE		table3[64];	
		static const XBYTE		table4[32];	
		static const XBYTE*		table[5];		

		XBYTE									digest[HASHCOMP128V1_RESULT_SIZE];
};	

	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

