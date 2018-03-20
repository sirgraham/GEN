/*------------------------------------------------------------------------------------------
//	XLICENSE.H
*/	
/**	
// \class 
//   
//  Licence Generator class
//   
//	@author	 Abraham J. Velez
//
//	Date Of Creation	: 15/05/2014 15:56:56
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.	
//----------------------------------------------------------------------------------------*/
	
#ifndef _XLICENSE_H_
#define _XLICENSE_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include <stdio.h>

#include "XBase.h"
#include "XFactory.h"
#include "XString.h"
#include "XDateTime.h"
#include "XEvent.h"
#include "XSubject.h"

#include "DIOURL.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

#define XLICENSE_FILEID										0xAA55
#define XLICENSE_MAXIDPARTS								4
#define XLICENSE_VERSION									1
#define XLICENSE_SUBVERSION								0

#define XLICENSE_FILESECTION							__L("LICENSE")
#define XLICENSE_FILEVALUE_ID							__L("licenseID")
#define XLICENSE_FILEVALUE_LICENSE				__L("license")
#define XLICENSE_FILEVALUE_EXPIRATION			__L("expiration")


enum XLICENSEXEVENTTYPE
{
	XLICENSEXEVENTTYPE_UNKNOWN						   =	XEVENTTYPE_LICENSE	,
	XLICENSEXEVENTTYPE_INVALID							               					,		
  XLICENSEXEVENTTYPE_EXPIRED	  																	,		
};


#define CREATEMASTERLICENSE(xlicense, xpath, xlicenseID, applicationID, expirationseconds)			{	XSTRING appID;																																			\
																																																	appID = applicationID;																															\
																																																	xlicense->CheckMasterCreation(xpath, xlicenseID, appID, expirationseconds);					\
																																																}
	

#ifdef XLICENSE_ACTIVE

	#define CHECKLICENSEFULLLOCAL(xlicense, xpath, applicationID)																		{																																											\
																																																		XLICENSEID  xlicenseID;																															\
																																																		XSTRING			appID;																																	\
																																																		XBUFFER	    license();																											\
																																																		appID = applicationID;																															\
																																																		bool		    status = xlicense->GenerateMachineID(xlicenseID);												\
																																																		if(status)  status = xlicense->Generate(xlicenseID, &appID);												\
																																																		if(status)	status = xlicense->LoadFromFile(xpath, appID, &license);								\
																																																		if(status)  status = license.Compare(xlicense->Get());															\
																																																		if(!status) xlicense->SetEvent(XLICENSEXEVENTTYPE_INVALID);													\
																																																		#endif																																							\
																																																	}

	#define CHECKLICENSEFULLLOCALEXPIRE(xlicense, xpath, applicationID)															{																																											\
																																																		XLICENSEID  xlicenseID;																															\
																																																		XSTRING			appID;																																	\
																																																		XSTRING			expireddate;																														\
																																																		XBUFFER	    license();																											\
																																																		XTIME				xtimeexpired;																														\
																																																		XDATETIME*			xtimeactual;																														\
																																																		appID = applicationID;																															\
																																																		bool		    status = xlicense->GenerateMachineID(xlicenseID);												\
																																																		if(status)  status = xlicense->Generate(xlicenseID, &appID);												\
																																																		if(status)	status = xlicense->LoadFromFile(xpath, appID, &license, &expireddate);	\
																																																		if(status)  status = license.Compare(xlicense->Get());															\
																																																		if(!status) xlicense->SetEvent(XLICENSEXEVENTTYPE_INVALID);													\
																																																		xtimeactual = xfactory->CreateTime();																								\
																																																		if(xtimeactual)																																			\
																																																			{																																									\
																																																				xtimeexpired.GetDateTimeFromString(expireddate, XDATETIME_FORMAT_STANDARD);					\
																																																				xtimeactual->Read();                                                        \
																																																				if(xtimeexpired.GetDay() || xtimeexpired.GetMonth() || xtimeexpired.GetYear())  \
																																																					{                                                                             \
																																																						if(xtimeactual->GetSeconsFromDate() >= xtimeexpired.GetSeconsFromDate())    \
																																																							{                                                                         \
																																																								xlicense->SetEvent(XLICENSEXEVENTTYPE_EXPIRED);                         \
																																																							}                                                                         \
																																																					}																																							\
																																																				xfactory->DeleteTime(xtimeactual);																							\
																																																			}																																									\
																																																	}
#else

	#define CHECKLICENSEFULLLOCAL(xlicense, xpath, applicationID)											//
	#define CHECKLICENSEFULLLOCALEXPIRE(xlicense, xpath, applicationID)								//

#endif





/*---- CLASS -----------------------------------------------------------------------------*/

class XFACTORY;
class XPUBLISHER;
class XLICENSE;
class DIOFACTORY;



class XLICENSEXEVENT : public XEVENT
{
	public:
													XLICENSEXEVENT								(XSUBJECT* subject, XDWORD type = XLICENSEXEVENTTYPE_UNKNOWN, XDWORD family = XEVENTTYPE_LICENSE) : XEVENT(subject, type, family)		
													{ 
														Clean();												
													}

		virtual			 				 ~XLICENSEXEVENT								()																																											
													{
														Clean();												
													}

		XLICENSE*							Get														()													{ return xlicense;								}
		void 									Set														(XLICENSE* xlicense)				{ this->xlicense = xlicense;			}		

	private:

		void									Clean													()
													{
														xlicense	= NULL;
														
													}

		XLICENSE*							xlicense;		
};




class XLICENSEID 
{
	public:

													XLICENSEID										()													{ Clean();											}
		virtual							 ~XLICENSEID										()													{ Clean();											}

		bool									GetPart												(int npart, XDWORD part)									
													{
														if(npart<0)											 return false;
														if(npart>=XLICENSE_MAXIDPARTS)	 return false;

														part = this->part[npart];

														return true;
													}

		bool									SetPart												(int npart, XDWORD part)									
													{
														if(npart<0)											 return false;
														if(npart>=XLICENSE_MAXIDPARTS)	 return false;

														this->part[npart] = part;

														return true;
													}
		
		XDWORD*								GetParts											()													{ return part;									}

		void									GetXBuffer								(XBUFFER& xbuffer)
													{
														for(int c=0; c<XLICENSE_MAXIDPARTS; c++)
															{
																xbuffer.Add((XDWORD)part[c]);
															}					
													}

		void									GetXString										(XSTRING& IDstring)
													{
														XSTRING partstring;

														IDstring.Empty();

														for(int c=0; c<XLICENSE_MAXIDPARTS; c++)
															{
																partstring.Format(__L("%08X"), part[c]);														
																IDstring += partstring;														
																if(c != (XLICENSE_MAXIDPARTS-1))  IDstring +=__C('-');
															}					
													}

	private:

		void									Clean													()
													{
														for(int c=0; c<XLICENSE_MAXIDPARTS; c++)
															{
																part[c] = 0;
															}
													}

		XDWORD								part[XLICENSE_MAXIDPARTS];
};




class XLICENSE  : public XSUBJECT
{
	public:

													XLICENSE											();
		virtual							 ~XLICENSE											();

		virtual bool					GenerateMachineID							(XLICENSEID& xlicenseID);			
		virtual bool					Generate											(XLICENSEID& xlicenseID, XSTRING* applicationID = NULL, XBUFFER* license = NULL);					
		virtual bool					GetBufferKeyFromMachineID			(XSTRING& applicationID, XBUFFER& keybuffer);		
		virtual bool				  CipherExpirationDate          (bool cipher, XSTRING& applicationID, XSTRING& expirationdate);

		XSTRING*							GetApplicationID							()													{ return &applicationID;				}
		XDATETIME*						GetExpirationDateTime					()													{ return &expirationdatetime;		}		
		XBUFFER*							Get														()													{ return license;								}
		bool									Get														(XSTRING& licensestring);

		bool									LoadFromFile									(XPATH& xpath, XSTRING& applicationID, XBUFFER* license = NULL, XSTRING* expirationdate = NULL);		
		bool									LoadFromURL										(DIOURL& url, int timeout, XSTRING* IPlocal, XSTRING& applicationID, XBUFFER* license = NULL, XSTRING* expirationdate = NULL);
		bool									LoadFromBuffer								(XBUFFER& xbuffer, XSTRING& applicationID, XBUFFER* license = NULL, XSTRING* expirationdate = NULL);

		bool									CheckMasterCreation						(XPATH& xpath, XLICENSEID& xlicenseID, XSTRING& applicationID, int expirationseconds = 0);

		bool									SetEvent											(XLICENSEXEVENTTYPE type);
		
	private:

		void									Clean													()
													{														
														license			= NULL;
													}
		
	
		XBUFFER*							license;
		XSTRING								applicationID;
		XDATETIME							expirationdatetime;
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

