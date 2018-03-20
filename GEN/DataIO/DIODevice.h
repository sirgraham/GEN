/*------------------------------------------------------------------------------------------
//	DIODEVICE.H
*/	
/**	
// \class 
//   
//  DIO Generic Device
//   
//	@author	 Abraham J. Velez
//
//	Date Of Creation	: 24/09/2014 8:49:43
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _DIODEVICE_H_
#define _DIODEVICE_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/


class DIODEVICE
{
	public:

													DIODEVICE							()						{ Clean();														  }
		virtual							 ~DIODEVICE							()						{ Clean();															}
    
		virtual bool					Ini                   ()            
													{ 
														isinitialized = true; 
														return true;													
													}
    
    
		bool									IsInitialized         ()						{ return isinitialized;                 }

    
		virtual bool					End                   ()            
													{ 
														isinitialized = false;
														return true;													
													}
    

	protected:

		bool									isinitialized;		

	private:

		void									Clean									()
													{
														isinitialized = false;
													}
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

