/*------------------------------------------------------------------------------------------
//	XUUID.H
*/	
/**	
// \class 
//   
//  UUID class
//   
//	@author	 Abraham J. Velez
//	@version 07/05/2013 20:25:52
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _XUUID_H_
#define _XUUID_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "XString.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
#define XUUIDMAXDATA4		8
	
/*---- CLASS -----------------------------------------------------------------------------*/
	

class XUUID
{ 
	public:

												XUUID						();
		virtual						 ~XUUID						();
		
		XDWORD							GetData1				();
		XWORD								GetData2				();
		XWORD								GetData3				();
		XBYTE*							GetData4				();

		bool								SetData1				(XDWORD data);
		bool								SetData2				(XWORD data);
		bool								SetData3				(XWORD data);
		bool								SetData4				(XBYTE* data);

		bool								Set							(XDWORD data1, XWORD data2, XWORD data3, XBYTE* data4);

		bool								CopyFrom				(XUUID& uuid);
		bool								CopyTo					(XUUID& uuid);

		bool								GetString				(XSTRING& string);

	protected:		

		XDWORD							data1;
		XWORD								data2;
		XWORD								data3;
		XBYTE								data4[XUUIDMAXDATA4];

	private:	

		void								Clean				();		
};
 
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

