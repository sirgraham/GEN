/*------------------------------------------------------------------------------------------
//	GRPFILEBITMAP.H
*/	
/**	
// \class 
//   
//  GRP File Bitmap class
//   
//	@author	 Diego Martinez Ruiz de Gaona
//	@version 08/11/2013 9:46:05
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPFILEBITMAP_H_
#define _GRPFILEBITMAP_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "XPath.h"
#include "XFile.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
	
class GRPFACTORY;
class GRPBITMAP;
class GRPBITMAPSEQUENCE;
class GRPPROPERTY;


class GRPFILEBITMAPBASE : public XFILECONTAINER
{
	public:
																GRPFILEBITMAPBASE				();
		virtual										 ~GRPFILEBITMAPBASE				();
		
		virtual GRPBITMAP*					CreateBitmapFromFile		(XPATH& xpath)												{	return NULL;																			}	
		virtual bool								CreateFileFromBitmap		(XPATH& xpath, GRPBITMAP* bitmap)			{	return false;																			}	

		virtual XBYTE*							ReadImageBlock					(XPATH& xpath)												{	return NULL;																			}	

	protected:
		
		bool												Open										(XPATH& xpath, bool readonly=true);		
		bool												Create									(XPATH& xpath);
		bool												Close										(void);			

		
	private:
		
		void												Clean										()
																{
																	
																};		
};

	


class GRPFILEBITMAP
{
	public:
																GRPFILEBITMAP						();
																GRPFILEBITMAP						(XCHAR* xpath);																
																GRPFILEBITMAP						(XPATH& xpath);																
		virtual						 				 ~GRPFILEBITMAP						();	

		XCHAR*											GetPath									()																		{	return xpath.Get();															}
		void												SetPath									(XPATH& xpath)												{	this->xpath = xpath;														}
		void												SetPath									(XCHAR* xpath)												{	this->xpath = xpath;														}
	
		
		GRPBITMAP*									Load										(XCHAR* xpath);
		GRPBITMAP*									Load										(XPATH& xpath);
		GRPBITMAP*									Load										();

		bool												Save										(XCHAR* xpath, GRPBITMAP* bitmap);
		bool												Save										(XPATH& xpath, GRPBITMAP* bitmap);
		bool												Save										(GRPBITMAP* bitmap);

		GRPBITMAPSEQUENCE*					LoadSequence						(XCHAR* namemask, int inisequence = 0, int endsequence=-1);

	private:

		void												Clean										();		 

		XPATH												xpath;
};
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

