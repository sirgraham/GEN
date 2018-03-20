/*------------------------------------------------------------------------------------------
//	DIOWEBHEADER.H
*/	
/**	
// \class 
//   
//  DIO WEB (HTTP) Header class
//   
//	@author	 Abraham J. Velez
//	@version 05/04/2013 19:31:19
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _DIOWEBHEADER_H_
#define _DIOWEBHEADER_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XVector.h"
#include "XString.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
#define DIOWEBHEADER_MAXLINE   1024



enum DIOWEBHEADER_RESULT
{
	DIOWEBHEADER_RESULT_CONTINUE                            = 100 ,
	DIOWEBHEADER_RESULT_SWITCHINGPROTOCOLS												,
                                       
	DIOWEBHEADER_RESULT_OK                                  = 200 ,
	DIOWEBHEADER_RESULT_CREATED																		,
	DIOWEBHEADER_RESULT_ACCEPTED																	,
	DIOWEBHEADER_RESULT_NONAUTHORITATIVEINFORMATION								,
	DIOWEBHEADER_RESULT_NOCONTENT																	,
	DIOWEBHEADER_RESULT_RESETCONTENT															,	
	DIOWEBHEADER_RESULT_PARTIALCONTENT														,
                                       
	DIOWEBHEADER_RESULT_MULTIPLECHOICES                     = 300 ,
	DIOWEBHEADER_RESULT_MOVEDPERMANENTLY													,
	DIOWEBHEADER_RESULT_FOUND																			,
	DIOWEBHEADER_RESULT_SEEOTHER																	,	
	DIOWEBHEADER_RESULT_NOTMODIFIED																,
	DIOWEBHEADER_RESULT_USEPROXY																	,
	DIOWEBHEADER_RESULT_UNUSED																		,
	DIOWEBHEADER_RESULT_TEMPORARYREDIRECT													,                                       
                                       
	DIOWEBHEADER_RESULT_BADREQUEST													= 400	,
	DIOWEBHEADER_RESULT_UNAUTHORIZED															,
	DIOWEBHEADER_RESULT_PAYMENTREQUIRED														,	
	DIOWEBHEADER_RESULT_FORBIDDEN																	,
	DIOWEBHEADER_RESULT_NOTFOUND																	,
	DIOWEBHEADER_RESULT_METHODNOTALLOWED													,
	DIOWEBHEADER_RESULT_NOTACCEPTABLE															,
	DIOWEBHEADER_RESULT_PROXYAUTHENTICATIONREQUIRED								,
	DIOWEBHEADER_RESULT_REQUESTTIMEOUT														,
	DIOWEBHEADER_RESULT_CONFLICT																	,
	DIOWEBHEADER_RESULT_GONE																			,
	DIOWEBHEADER_RESULT_LENGTHREQUIRED														,
	DIOWEBHEADER_RESULT_PRECONDITIONFAILED												,
	DIOWEBHEADER_RESULT_REQUESTENTITYTOOLARGE											,
	DIOWEBHEADER_RESULT_REQUESTURITOOLONG													,
	DIOWEBHEADER_RESULT_UNSUPPORTEDMEDIATYPE											,
	DIOWEBHEADER_RESULT_REQUESTEDRANGENOTSATISFIABLE							,
	DIOWEBHEADER_RESULT_EXPECTATIONFAILED													,
                                       
	DIOWEBHEADER_RESULT_INTERNALSERVERERROR			            = 500	,
	DIOWEBHEADER_RESULT_NOTIMPLEMENTED														,
	DIOWEBHEADER_RESULT_BADGATEWAY																,
	DIOWEBHEADER_RESULT_SERVICEUNAVAILABLE												,
	DIOWEBHEADER_RESULT_GATEWAYTIMEOUT														,
	DIOWEBHEADER_RESULT_HTTPVERSIONNOTSUPPORTED										,
};

	
/*---- CLASS -----------------------------------------------------------------------------*/

class XTIMER;
class DIOSTREAMTCPIP;
class DIOWEBCLIENT_HEADER;
class DIOWEBSERVER;
class DIOWEBSERVER_CONNEXION; 
class DIOWEBSERVER_REQUEST;


class DIOWEBHEADER
{
	public:	
																DIOWEBHEADER											();
		virtual										 ~DIOWEBHEADER											();		

		bool												Read															(DIOSTREAMTCPIP*	diostream, XTIMER* xtimer, int timeout);
		bool												Write															(DIOSTREAMTCPIP*	diostream,  int timeout);

		bool												AddLine														(XCHAR* line);
		bool												AddLine														(XSTRING& line);

		XVECTOR<XSTRING*>*					GetLines													();	

		XCHAR*											GetFieldValue											(XCHAR* field);	
		XCHAR*											GetFieldValue											(XSTRING& field);	
		
		bool												DeleteAllLines										();	

	protected:

		XVECTOR<XSTRING*>						lines;	
		
		bool												GetFormat													(bool isserver, int type, void* data);			

	private:
		
		void												Clean															()
																{
			
																}
};






	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

