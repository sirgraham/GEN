//------------------------------------------------------------------------------------------
//	IFASKINMANAGER.H
//	
/**	
// \class 
//   
//  Interface Skin Manager Class
//   
//	@author	 Abraham J. Velez
//	@version 20/02/2004 10:31:55
*/	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
	
#ifndef _IFASKINMANAGER_H_
#define _IFASKINMANAGER_H_
	
	
//---- INCLUDES ----------------------------------------------------------------------------
	
	
//---- DEFINES & ENUMS  --------------------------------------------------------------------
	
enum IFASKINTYPE
{
	IFASKINTYPE_DEFAULT		= 0	,
	IFASKINTYPE_COMIC					,
	IFASKINTYPE_BEVEL					,
};

//---- CLASS -------------------------------------------------------------------------------

class IFAOBJ;

class IFASKINOBJ
{
	public:

											IFASKINOBJ				();
		virtual					 ~IFASKINOBJ				();

		IFASKINTYPE				GetType						();
		bool							SetType						(IFASKINTYPE type);

		virtual	bool			Draw							(IFAOBJ* obj,int x, int y);			

	private:

		void							Clean							();	
		
		IFASKINTYPE				skintype;	
};




class IFASKINMANAGER
{
	public:

											IFASKINMANAGER		();
											IFASKINMANAGER		(IFASKINTYPE skintype);
		virtual					 ~IFASKINMANAGER		();

		IFASKINTYPE				GetType						();
		bool							SetType						(IFASKINTYPE type);

		IFASKINOBJ*				GetSkinObj				(IFAOBJ *obj);			
			
	private:

		void							Clean							();

		IFASKINTYPE				skintype;
};

	
//---- INLINE FUNCTIONS --------------------------------------------------------------------
	
#endif

