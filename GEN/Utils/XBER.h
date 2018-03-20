/*------------------------------------------------------------------------------------------
//	XBER.H
*/	
/**	
// \class 
//   
//  BER (Basic Encoding Rules) encoding class 
//   
//	@author	 Abraham J. Velez
//	@version 03/09/2012 17:00:32
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _XBER_H_
#define _XBER_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "XBase.h"
#include "XVector.h"
#include "XString.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

#define XBERTYPE_EXTENDED									0x01
#define XBERTYPE_INTEGER									0x02
#define XBERTYPE_BITSTRING								0x03
#define XBERTYPE_OCTETSTRING							0x04
#define XBERTYPE_NULL											0x05
#define XBERTYPE_OID											0x06
#define XBERTYPE_SEQOFSEQ									0x10
#define XBERTYPE_SETOFSET									0x11
#define XBERTYPE_PRINTABLESTRING					0x13
#define XBERTYPE_IA5STRING								0x16
#define XBERTYPE_UTCTIME									0x17

#define XBERTYPE_ISUNIVERSAL							0x00
#define XBERTYPE_ISAPPLICATION						0x40
#define XBERTYPE_ISCONTEXTSPECIFIC				0x80
#define XBERTYPE_ISPRIVATE								0xC0

#define XBERTYPE_ISCONSTRUCTED						0x20

	
/*---- CLASS -----------------------------------------------------------------------------*/
	
class XFACTORY;
class XBUFFER;

class XBER
{
	public:
			                        XBER											();
		virtual									 ~XBER											();

		XBYTE											GetType										();		
		XDWORD										GetSize										();
		XBUFFER*									GetData										();	

		XDWORD										GetSizeBER								();

		bool											GetDump										(XBUFFER& xbuffer);

		bool											SetType										(XBYTE type);

		bool											SetNULL										();
		
		bool											SetINTEGER								(XDWORD integer);
		bool											SetINTEGER								(XBUFFER& xbufferdata);

	//bool											SetBITSTRING							(XCHAR* string);
		bool											SetBITSTRING							(XBUFFER& databuffer);
		bool											SetBITSTRING							(XBYTE* buffer, int size);

		bool											SetOCTETSTRING						(XCHAR* string);
		bool											SetOCTETSTRING						(XBUFFER& databuffer);
		bool											SetOCTETSTRING						(XBYTE* buffer, int size);

		bool											SetOID										(XCHAR* OIDstring);
		
		bool											Copy											(XBER* newber);
		
		virtual bool							Sequence_AddTo						(XBER& xber);		
		XDWORD										Sequence_GetSize					();
		bool											Sequence_DeleteAll				();

	protected:

		XBYTE											type;
		XDWORD										size;											
		XBUFFER*									data;
		XVECTOR<XBER*>						sequence;

	private:

		void											Clean											();

		bool											CodeBigNumber							(XDWORD number,XBUFFER& data);
    bool                      CodeSize                  (XDWORD number,XBUFFER& data);
		
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

