/*------------------------------------------------------------------------------------------
//	APPLICATIONDATA.H
*/	
/**	
// \class 
//   
//  Test Protocol Aplication Data
//   
//	@author	 Abraham J. Velez
//	@version 02/03/2012 10:25:01
*/	
/*	GEN (C) Copyright  (All right reserved).			 
//----------------------------------------------------------------------------------------*/
	
#ifndef _TESTPROTOCOLAPPLICATIONDATA_H_
#define _TESTPROTOCOLAPPLICATIONDATA_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XBase.h"
#include "XVariant.h"
#include "XFactory.h"
#include "XMap.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
	
class TESTPROTOCOLAPPLICATIONDATA
{
	public:
															TESTPROTOCOLAPPLICATIONDATA		()						
															{ 
																Clean();		
																xmutexdata = xfactory->Create_Mutex();
															}

		virtual									 ~TESTPROTOCOLAPPLICATIONDATA		()					
															{ 
																DeleteAllData();
																xfactory->Delete_Mutex(xmutexdata);

																Clean();																			
															}

		
		bool											GetVersion										(XBYTE& version, XBYTE& subversion, XBYTE& subversionerr)
															{
																LockData();

																version				 = this->version;
																subversion		 = this->subversion;
																subversionerr  = this->subversionerr;

																UnLockData();
	
																return true;
															}

		bool											SetVersion										(XBYTE version, XBYTE subversion, XBYTE subversionerr)
															{
																LockData();

																this->version				= version;
																this->subversion		= subversion;
																this->subversionerr = subversionerr;

																UnLockData();
	
																return true;
															}

		bool											GetString											(XSTRING& string)
															{
																LockData();

																string = this->string;
																
																UnLockData();
	
																return true;
															}

		bool											SetString											(XSTRING& string)
															{
																LockData();

																this->string = string;
																
																UnLockData();
	
																return true;
															}




		XVARIANT*									GetData												(XDWORD variantID)
															{
																LockData();

																XVARIANT* variant = variants.Get(variantID);
																if(!variant) 
																	{
																		AddData(variantID);	
																		variant = variants.Get(variantID);
																	}

																UnLockData();

																return variant;
															}

		bool											SetData												(XDWORD variantID, XVARIANT* variant)
															{
																LockData();

																XVARIANT* _variant = variants.Get(variantID);
																if(!_variant) 
																	{			
																		AddData(variantID);
																		_variant = variants.Get(variantID);			
																	}

																bool status = false;

																if(_variant)
																	{
																		(*_variant) = (*variant);
																		status = variants.Set(variantID, _variant);	
																	}

																UnLockData();

																return status;
															}

		bool											SetData												(XDWORD variantID, XVARIANT& variant)
															{
																return SetData(variantID, &variant);
															}

		template < class T > bool	SetData												(XDWORD variantID, T variant);					

		bool											AddData												(XDWORD variantID)
															{	
																bool status = false;
	
																XVARIANT* variant = new XVARIANT();
																if(variant) 		
																		status = variants.Add(variantID, variant);
	
																return status;
															}

		bool											DeleteData										(XDWORD variantID)
															{
																if(variants.IsEmpty()) return false;

																LockData();

																XVARIANT* _variant = variants.Get(variantID);
																if(!_variant) 
																	{
																		delete _variant;
																		variants.Delete(variantID);
																	}

																UnLockData();

																return true;
															}

		bool											DeleteAllData									()
															{
																if(variants.IsEmpty()) return false;

																LockData();

																variants.DeleteElementContents();
																variants.DeleteAll();

																UnLockData();

																return true;
															}

	
protected:

		bool											LockData											()														
															{ 
																if(!xmutexdata) return false;
																xmutexdata->Lock();

																return true;            
															}

		bool											UnLockData										()														
															{ 
																if(!xmutexdata) return false;
																xmutexdata->UnLock();

																return true;            
															}

	private:

		void											Clean													()
															{
																xmutexdata  = NULL;
																version				 = 0;
																subversion		 = 0;
																subversionerr  = 0;
															}


		XMUTEX*										xmutexdata;

		XBYTE											version;
		XBYTE											subversion;
		XBYTE											subversionerr;

		XSTRING										string;
		
		XMAP<XDWORD,XVARIANT*>    variants;
};



template <class T>
bool TESTPROTOCOLAPPLICATIONDATA::SetData(XDWORD variantID, T variant)
{
  LockData();

	XVARIANT* _variant = variants.Get(variantID);
	if(!_variant) 
		{			
			AddData(variantID);
			_variant = variants.Get(variantID);			
		}

	bool status = false;

	if(_variant)
		{
			(*_variant) = variant;
			status = variants.Set(variantID, _variant);	
	}

	UnLockData();

	return status;
}

	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

