/*------------------------------------------------------------------------------------------
//	HASHWHIRLPOOL.H
*/	
/**	
// \class 
//   
//  Hash Whirlpool Class
//   
//	@author	 Abraham J. Velez
//	@version 02/03/2013 12:02:40
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _HASHWHIRLPOOL_H_
#define _HASHWHIRLPOOL_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "Hash.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

#define HASHWHIRLPOOL_DIGESTBYTES							64
#define HASHWHIRLPOOL_DIGESTBITS							(8*HASHWHIRLPOOL_DIGESTBYTES) /* 512 */

#define HASHWHIRLPOOL_WBLOCKBYTES							64
#define HASHWHIRLPOOL_WBLOCKBITS							(8*HASHWHIRLPOOL_WBLOCKBYTES) /* 512 */

#define HASHWHIRLPOOL_LENGTHBYTES							32
#define HASHWHIRLPOOL_LENGTHBITS							(8*HASHWHIRLPOOL_LENGTHBYTES) /* 256 */

#define HASHWHIRLPOOL_ROUNDS									10

typedef struct 
{
	XBYTE		bitlength[HASHWHIRLPOOL_LENGTHBYTES];		// global number of hashed bits (256-bit counter) 
	XBYTE		buffer[HASHWHIRLPOOL_WBLOCKBYTES];			// buffer of data to hash 
	int			bufferbits;								// current number of bits on the buffer 
	int			bufferpos;								// current (possibly incomplete) byte slot on the buffer 
	XQWORD	hash[HASHWHIRLPOOL_DIGESTBYTES/8];			// the hashing state 

} WHIRLPOOL;


/*---- CLASS -----------------------------------------------------------------------------*/


class HASHWHIRLPOOL : public HASH
{
	public:
												HASHWHIRLPOOL					();
		virtual			 			 ~HASHWHIRLPOOL	 				();

		using								HASH::Do;		
		bool								Do										(XBYTE* input, int size);	

		int									GetDefaultSize				();
								
	private:

		static XQWORD				C0[256];
		static XQWORD				C1[256];
		static XQWORD				C2[256];
		static XQWORD				C3[256];
		static XQWORD				C4[256];
		static XQWORD				C5[256];
		static XQWORD				C6[256];
		static XQWORD				C7[256];
		static XQWORD				rc[HASHWHIRLPOOL_ROUNDS + 1];
		WHIRLPOOL						w;
						 
		void								Clean									();		

		void								ProcessBuffer					(WHIRLPOOL* structpointer);

		void								Ini										(WHIRLPOOL* structpointer);
		void								Add										(XBYTE* source, XDWORD sourceBits, WHIRLPOOL* structpointer);
		void								End										(WHIRLPOOL* structpointer, XBYTE* result);

};	

	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

