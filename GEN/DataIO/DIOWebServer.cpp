//------------------------------------------------------------------------------------------
//	DIOWEBSERVER.CPP
//	
//	DIO Web Server class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 20/01/2007 21:09:36
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
	

//---- INCLUDES ----------------------------------------------------------------------------

#include "XFactory.h"	
#include "XSleep.h"
#include "XTimer.h"
#include "XBuffer.h"
#include "XPath.h"
#include "XLog.h"
#include "XFileTXT.h"
#include "XDebug.h"

#include "CompressManager.h"

#include "DIOFactory.h"
#include "DIOURL.h"
#include "DIOStreamXEvent.h"
#include "DIOStreamTCPIPConfig.h"
#include "DIOStreamTCPIP.h"
#include "DIOWebServerXEvent.h"

#include "DIOWebServer.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------




/*-------------------------------------------------------------------
//  DIOWEBSERVER_HEADER::DIOWEBSERVER_HEADER
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			11/04/2013 23:05:36
//	
//	@return 			
//	*/
/*-----------------------------------------------------------------*/
DIOWEBSERVER_HEADER::DIOWEBSERVER_HEADER()
{
	Clean();
}




/*-------------------------------------------------------------------
//  DIOWEBSERVER_HEADER::~DIOWEBSERVER_HEADER
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			11/04/2013 23:05:46
//	
//	@return 			
//	*/
/*-----------------------------------------------------------------*/
DIOWEBSERVER_HEADER::~DIOWEBSERVER_HEADER()
{
	DeleteAllLines();

	Clean();
}



/*-------------------------------------------------------------------
//  DIOWEBSERVER_HEADER::GetVersion
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			11/04/2013 23:06:10
//	
//	@return 			int : 
//	*/
/*-----------------------------------------------------------------*/
int DIOWEBSERVER_HEADER::GetVersion()
{
	int data = 0;
	
	if(!GetFormat(true, 0, &data)) return false;

	return data;	
}




/*-------------------------------------------------------------------
//  DIOWEBSERVER_HEADER::GetSubVersion
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			11/04/2013 23:06:15
//	
//	@return 			int : 
//	*/
/*-----------------------------------------------------------------*/
int DIOWEBSERVER_HEADER::GetSubVersion()
{
	int data = 0;
	
	if(!GetFormat(true, 1, &data)) return false;

	return data;	
}




/*-------------------------------------------------------------------
//  DIOWEBSERVER_HEADER::GetResult
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			11/04/2013 23:06:28
//	
//	@return 			int : 
//	*/
/*-----------------------------------------------------------------*/
int DIOWEBSERVER_HEADER::GetResult()
{
	int data = 0;
	
	if(!GetFormat(true, 2, &data)) return false;

	return data;	
}




/*-------------------------------------------------------------------
//  DIOWEBSERVER_HEADER::GetResultString
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			11/04/2013 23:06:34
//	
//	@return 			bool : 
//	@param				resultstring : 
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSERVER_HEADER::GetResultString(XSTRING& resultstring)
{	
	if(!GetFormat(true, 3, &resultstring)) return false;

	return true;	
}






/*-------------------------------------------------------------------
//	DIOWEBSERVER_HEADER::Create
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			17/07/2017 12:57:36
//	
//	@return 			bool : 
//
//  @param				webserver : 
//  @param				connexion : 
//  @param				request : 
//  @param				resource : 
//  @param				version : 
//  @param				subversion : 
//  @param				result : 
//  @param				size : 
//  @param				entitytag : 
//  @param				isplayable : 
//  @param				password : 
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSERVER_HEADER::Create(DIOWEBSERVER* webserver, DIOWEBSERVER_CONNEXION* connexion, DIOWEBSERVER_REQUEST* request, XSTRING* resource, int version, int subversion, DIOWEBHEADER_RESULT result, int size, XSTRING* entitytag, bool isplayable, bool password)
{
		/*
				Accept - Ranges:bytes
				Connection : Keep - Alive
				Content - Length : 6
				Content - Type : text / html
				Date : Mon, 06 Nov 2017 11 : 21 : 24 GMT
				ETag : "6-55d4e3b3b9f20"
				Keep - Alive : timeout = 5, max = 93
				Last - Modified : Mon, 06 Nov 2017 10 : 51 : 06 GMT
				Server : Apache / 2.4.23 (Win64)PHP / 5.6.25
		*/

		XSTRING line;		
		line.Format(__L("HTTP/%d.%d %d "), version, subversion, result);

		if (password)
		{
				line += __L("Unauthorised");
				AddLine(line);
				AddLine(__L("WWW-Authenticate: Basic"));
		}
		else
		{
				XSTRING description;

				GetDescriptionResult(result, description);

				line += description;
				AddLine(line);
		}

		AddLine(line.Get());
		AddLine(__L("Accept-Ranges:bytes"));
		AddLine(__L("Connection:Keep-Alive"));


//				line.Format(__L("Content-Length:%d"), size);	AddLine(line.Get());


		//XDEBUG_PRINTCOLOR(4, line.Get());
		
		line.Set(__L("Content-Type:"));
				 if (resource->Find(__L(".htm"),		true) != XSTRING_NOTFOUND)  line += __L("text/html");
		else if (resource->Find(__L(".exe"),		true) != XSTRING_NOTFOUND)  line += __L("application/octet-stream");
		else if (resource->Find(__L(".gif"),		true) != XSTRING_NOTFOUND)  line += __L("image/gif");
		else if (resource->Find(__L(".jpg"),		true) != XSTRING_NOTFOUND)  line += __L("image/jpeg");
		else if (resource->Find(__L(".jpeg"),		true) != XSTRING_NOTFOUND)  line += __L("image/jpeg");
		else if (resource->Find(__L(".wav"),		true) != XSTRING_NOTFOUND)  line += __L("audio/x-wav");
		else if (resource->Find(__L(".mp3"),		true) != XSTRING_NOTFOUND)  line += __L("audio/mp3");
		else if (resource->Find(__L(".gz"),			true) != XSTRING_NOTFOUND)  line += __L("application/x-gzip");
		else if (resource->Find(__L(".mpg"),		true) != XSTRING_NOTFOUND)  line += __L("video/mpeg");
		else if (resource->Find(__L(".pdf"),		true) != XSTRING_NOTFOUND)  line += __L("application/pdf");
		else if (resource->Find(__L(".avi"),		true) != XSTRING_NOTFOUND)  line += __L("video/x-msvideof");
		else if (resource->Find(__L(".css"),		true) != XSTRING_NOTFOUND)  line += __L("text/css");
		else if (resource->Find(__L(".json"),		true) != XSTRING_NOTFOUND)  line += __L("application/json");
		else if (resource->Find(__L("."),				true) == XSTRING_NOTFOUND)  line += __L("text/html");
		else line += __L("text/html");
		AddLine(line.Get());

		//AddLine(__L("Date : Mon, 06 Nov 2017 14:21:24 GMT"));
		XDATETIME* datetime = xfactory->CreateDateTime();
		XSTRING date, day, month, hour;
		datetime->Read();
		datetime->GetDateTimeToString(XDATETIME_FORMAT_TIMEWITHSECONDS, hour);
		datetime->GetMonthString(month);
		switch (datetime->GetDayOfWeek())
		{
		case 1: day.Set(__L("Mon")); break;
		case 2: day.Set(__L("Tue")); break;
		case 3: day.Set(__L("Wed")); break;
		case 4: day.Set(__L("Thu")); break;
		case 5: day.Set(__L("Fri")); break;
		case 6: day.Set(__L("Sat")); break;
		case 0:
		case 7: day.Set(__L("Sun")); break;
		}

		switch (datetime->GetMonth())
		{
		case 1:		month.Set(__L("Jan")); break;
		case 2:		month.Set(__L("Feb")); break;
		case 3:		month.Set(__L("Mar")); break;
		case 4:		month.Set(__L("Apr")); break;
		case 5:		month.Set(__L("May")); break;
		case 6:		month.Set(__L("Jun")); break;
		case 7:		month.Set(__L("Jul")); break;
		case 8:		month.Set(__L("Aug")); break;
		case 9:		month.Set(__L("Sep")); break;
		case 10:	month.Set(__L("Oct")); break;
		case 11:	month.Set(__L("Nov")); break;
		case 12:	month.Set(__L("Dec")); break;
		}

		//Date: Wed, 21 Oct 2015 07:28:00 GMT
		date.Format(__L("%s, %d %s %d %s GMT"), day.Get(), datetime->GetDay(), month.Get(), datetime->GetYear(), hour.Get());
		line.Format(__L("Date:%s"), date.Get());	AddLine(line);
		delete(datetime);

		if (entitytag)
		if (!entitytag->IsEmpty())
		{
				line.Format(__L("ETag : \"%s\""), entitytag->Get());
				AddLine(line.Get());
		}
		
		AddLine(__L("Keep-Alive:timeout=5,max=93"));		
		line.Format(__L("Last-Modified:%s"), date.Get());				  AddLine(line);		
		line.Format(__L("Server: %s"), DIOWEBSERVER_IDSERVER);		AddLine(line);

		bool validsize = false;

		if (size > 100)  validsize = true;
		if (size == -1)  validsize = true;

		AddLine(__L("Access-Control-Allow-Headers:DNT,X-CustomHeader,Keep-Alive,User-Agent,X-Requested-With,If-Modified-Since,Cache-Control,Content-Type,Content-Encoding,Vary"));		
		AddLine(__L("Vary:Accept-Encoding"));

		connexion->SetActualEncodingFlag(DIOWEBSERVER_ENCODINGFLAGS_NONE);
		if (request->IsAcceptedEncodingFlag(DIOWEBSERVER_ENCODINGFLAGS_GZIP))
		{
				if (webserver->IsIncludedEncodingFlag(DIOWEBSERVER_ENCODINGFLAGS_GZIP))
				{
						if ((validsize) && (isplayable))
						{
								connexion->SetActualEncodingFlag(DIOWEBSERVER_ENCODINGFLAGS_GZIP);
								AddLine(__L("Content-Encoding:gzip"));																
						}
				}
		}
		/*
	XSTRING line;

	line.Format(__L("HTTP/%d.%d %d "), version, subversion, result);

	if(password)
		{
			line += __L("Unauthorised");
			AddLine(line);		 		
			AddLine(__L("WWW-Authenticate: Basic"));
		}
	 else
	  {
			XSTRING description;

			GetDescriptionResult(result, description);

			line += description;
			AddLine(line);
		}


	line.Format(__L("Server: %s"),DIOWEBSERVER_IDSERVER);
	AddLine(line);



	line = __L("Access-Control-Max-Age:1728000");			
	AddLine(line);
	line = __L("Access-Control-Allow-Origin:");
	line += request->GetGuest()->Get();
	AddLine(line);
	line = __L("Accept-Language:");// en-us,en;q=0.5");
	line += connexion->GetAcceptLenguage();
	AddLine(line);
	line = __L("Access-Control-Allow-Methods:POST,GET,OPTIONS");
	AddLine(line);
	line = __L("Access-Control-Allow-Headers:*,origin,x-requested-with");
	AddLine(line);
	line = __L("Connection:Keep-Alive");
	//line = __L("Connection:Close");
	AddLine(line);	
	line = __L("Keep-Alive:timeout=5,max=93");
	AddLine(line);

	line = __L("Server: ECS (fll/0761)");
	AddLine(line);

	line = __L("Vary: Accept-Encoding");
	AddLine(line);

	line = __L("X-Cache: HIT");
	AddLine(line);



	if((result != 404) && resource)
		{			
			line = __L("Content-Type: ");
			
					 if(resource->Find(__L(".htm" ),true)!=XSTRING_NOTFOUND)  line += __L("text/html");
			else if(resource->Find(__L(".exe" ),true)!=XSTRING_NOTFOUND)  line += __L("application/octet-stream");
			else if(resource->Find(__L(".gif" ),true)!=XSTRING_NOTFOUND)  line += __L("image/gif");
			else if(resource->Find(__L(".jpg" ),true)!=XSTRING_NOTFOUND)  line += __L("image/jpeg");
			else if(resource->Find(__L(".jpeg"),true)!=XSTRING_NOTFOUND)  line += __L("image/jpeg");
			else if(resource->Find(__L(".wav" ),true)!=XSTRING_NOTFOUND)  line += __L("audio/x-wav");
			else if(resource->Find(__L(".mp3" ),true)!=XSTRING_NOTFOUND)  line += __L("audio/mp3");
			else if(resource->Find(__L(".gz"	),true)!=XSTRING_NOTFOUND)  line += __L("application/x-gzip");
			else if(resource->Find(__L(".mpg" ),true)!=XSTRING_NOTFOUND)  line += __L("video/mpeg");
			else if(resource->Find(__L(".pdf" ),true)!=XSTRING_NOTFOUND)  line += __L("application/pdf");
			else if(resource->Find(__L(".avi" ),true)!=XSTRING_NOTFOUND)  line += __L("video/x-msvideof");
			else if(resource->Find(__L(".css" ),true)!=XSTRING_NOTFOUND)  line += __L("text/css");
			else if(resource->Find(__L(".json"),true)!=XSTRING_NOTFOUND)  line += __L("application/json");
			else if(resource->Find(__L("."	  ),true)==XSTRING_NOTFOUND)  line += __L("text/html");
			//else line += __L("application/octet-stream");
			else 
					line += __L("text/html");
			

			AddLine(line);

			if(size!=-1)
				{
					line.Format(__L("Content-Length: %d"), size);					
					AddLine(line);	
				}
			
		}


	

	XSTRING pragma;
	XSTRING cachecontrol;

	pragma = __L("Pragma: no-cache");	
	//cachecontrol = __L("Cache-Control: max-age=2592000, public");
	cachecontrol = __L("Cache-Control: no-cache");

	if(entitytag)
		{
			if(!entitytag->IsEmpty())
				{
					line.Format(__L("ETag: \"%s\""), entitytag->Get());					
					AddLine(line);	

					//pragma = __L("Pragma: cache");										
					//AddLine(cachecontrol);	
				}			 
		}
	
	AddLine(cachecontrol);
	AddLine(pragma);	

	line.Format(__L("Accept-Ranges: bytes"));
	AddLine(line);
	
	//line.Format(__L("Keep - Alive: timeout = 5, max = 100"));
	//AddLine(line);

	bool validsize = false;

	if(size > 100)  validsize = true;
	if(size == -1)  validsize = true;
		
	connexion->SetActualEncodingFlag(DIOWEBSERVER_ENCODINGFLAGS_NONE);
	
	if(request->IsAcceptedEncodingFlag(DIOWEBSERVER_ENCODINGFLAGS_GZIP))
		{		 
			if(webserver->IsIncludedEncodingFlag(DIOWEBSERVER_ENCODINGFLAGS_GZIP))
				{
					if((validsize) && (isplayable))		
						{	
							connexion->SetActualEncodingFlag(DIOWEBSERVER_ENCODINGFLAGS_GZIP);
							AddLine(__L("Content-Encoding: gzip"));
						}	
				}
		}
	//*/
	return true;	
}



/*-------------------------------------------------------------------
//  DIOWEBSERVER_HEADER::Clean
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			11/04/2013 23:06:44
//	
//	@return 			void : 
//	*/
/*-----------------------------------------------------------------*/
void DIOWEBSERVER_HEADER::Clean()
{


}



/*-------------------------------------------------------------------
//  DIOWEBSERVER_HEADER::GetDescriptionResult
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			13/04/2013 17:02:12
//	
//	@return 			bool : 
//	@param				result : 
//  @param				description : 
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSERVER_HEADER::GetDescriptionResult(DIOWEBHEADER_RESULT result, XSTRING& description)
{
	typedef struct
	{
		DIOWEBHEADER_RESULT  result;
		XCHAR*							 description;

	} DESCRIPTION;

	DESCRIPTION  descriptions[] = { { DIOWEBHEADER_RESULT_CONTINUE                     ,  __L("Continue")													} ,
																	{ DIOWEBHEADER_RESULT_SWITCHINGPROTOCOLS					 ,  __L("Switching Protocols")							} ,
                                                                                                   
																	{ DIOWEBHEADER_RESULT_OK                           ,  __L("OK")																} ,
																	{ DIOWEBHEADER_RESULT_CREATED											 ,  __L("Created")													} ,
																	{ DIOWEBHEADER_RESULT_ACCEPTED										 ,  __L("Accepted")													} ,
																	{ DIOWEBHEADER_RESULT_NONAUTHORITATIVEINFORMATION	 ,  __L("Non-Authoritative Information")		} ,
																	{ DIOWEBHEADER_RESULT_NOCONTENT										 ,  __L("No Content")												} ,
																	{ DIOWEBHEADER_RESULT_RESETCONTENT								 ,	__L("Reset Content")										} ,
																	{ DIOWEBHEADER_RESULT_PARTIALCONTENT							 ,  __L("Partial Content")									} ,
                                                                                                   
																	{ DIOWEBHEADER_RESULT_MULTIPLECHOICES              ,  __L("Multiple Choices")									} ,
																	{ DIOWEBHEADER_RESULT_MOVEDPERMANENTLY						 ,  __L("Moved Permanently")								} ,
																	{ DIOWEBHEADER_RESULT_FOUND												 ,  __L("Found")														} ,
																	{ DIOWEBHEADER_RESULT_SEEOTHER										 ,	__L("See Other")												} ,
																	{ DIOWEBHEADER_RESULT_NOTMODIFIED									 ,  __L("Not Modified")											} ,
																	{ DIOWEBHEADER_RESULT_USEPROXY										 ,  __L("Use Proxy")												} ,
																	{ DIOWEBHEADER_RESULT_UNUSED											 ,  __L("(Unused)")													} ,
																	{ DIOWEBHEADER_RESULT_TEMPORARYREDIRECT						 ,  __L("Temporary Redirect")								} ,
                                                                                                   
																	{ DIOWEBHEADER_RESULT_BADREQUEST									 ,  __L("Bad Request")											} ,
																	{ DIOWEBHEADER_RESULT_UNAUTHORIZED								 ,  __L("Unauthorized")											} ,
																	{ DIOWEBHEADER_RESULT_PAYMENTREQUIRED							 ,	__L("Payment Required")									} ,
																	{ DIOWEBHEADER_RESULT_FORBIDDEN										 ,  __L("Forbidden")												} ,
																	{ DIOWEBHEADER_RESULT_NOTFOUND										 ,  __L("Not Found")												} ,
																	{ DIOWEBHEADER_RESULT_METHODNOTALLOWED						 ,  __L("Method Not Allowed")								} ,
																	{ DIOWEBHEADER_RESULT_NOTACCEPTABLE								 ,  __L("Not Acceptable")										} ,
																	{ DIOWEBHEADER_RESULT_PROXYAUTHENTICATIONREQUIRED	 ,  __L("Proxy Authentication Required")		} ,
																	{ DIOWEBHEADER_RESULT_REQUESTTIMEOUT							 ,  __L("Request Timeout")									} ,
																	{ DIOWEBHEADER_RESULT_CONFLICT										 ,  __L("Conflict")													} ,
																	{ DIOWEBHEADER_RESULT_GONE												 ,  __L("Gone")															} ,
																	{ DIOWEBHEADER_RESULT_LENGTHREQUIRED							 ,  __L("Length Required")									} ,
																	{ DIOWEBHEADER_RESULT_PRECONDITIONFAILED					 ,  __L("Precondition Failed")							} ,
																	{ DIOWEBHEADER_RESULT_REQUESTENTITYTOOLARGE				 ,  __L("Request Entity Too Large")					} ,
																	{ DIOWEBHEADER_RESULT_REQUESTURITOOLONG						 ,  __L("Request-URI Too Long")							} ,
																	{ DIOWEBHEADER_RESULT_UNSUPPORTEDMEDIATYPE				 ,  __L("Unsupported Media Type")						} ,
																	{ DIOWEBHEADER_RESULT_REQUESTEDRANGENOTSATISFIABLE ,  __L("Requested Range Not Satisfiable")	} ,
																	{ DIOWEBHEADER_RESULT_EXPECTATIONFAILED						 ,  __L("Expectation Failed")								} ,
                                                                                                   
																	{ DIOWEBHEADER_RESULT_INTERNALSERVERERROR			     ,  __L("Internal Server Error")						} ,
																	{ DIOWEBHEADER_RESULT_NOTIMPLEMENTED							 ,  __L("Not Implemented")									} ,
																	{ DIOWEBHEADER_RESULT_BADGATEWAY									 ,  __L("Bad Gateway")											} ,
																	{ DIOWEBHEADER_RESULT_SERVICEUNAVAILABLE					 ,  __L("Service Unavailable")							} ,
																	{ DIOWEBHEADER_RESULT_GATEWAYTIMEOUT							 ,  __L("Gateway Timeout")									} ,
																	{ DIOWEBHEADER_RESULT_HTTPVERSIONNOTSUPPORTED			 ,  __L("HTTP Version Not Supported")				} 
															  };

	description.Empty();

	for(int c=0;c<sizeof(descriptions)/sizeof(DESCRIPTION);c++)
		{
			if(result == descriptions[c].result)
				{
					description = descriptions[c].description;
					
					return true;
				}
		}

	return false;
}




/*-------------------------------------------------------------------
//  DIOWEBSERVER_REQUEST::SetIfNoneMatch
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			10/01/2014 14:06:33
//	
//	@return 			bool : 
//	@param				ifnonematch : 
*/
/*-----------------------------------------------------------------*/
void DIOWEBSERVER_REQUEST::SetIfNoneMatch(XSTRING& ifnonematch)
{
	XSTRING special;

	special = ifnonematch;

	special.DeleteCharacter(0x20, XSTRINGCONTEXT_ATFIRST);
	special.DeleteCharacter(0x20, XSTRINGCONTEXT_ATEND);
	special.DeleteCharacter(__C('"'), XSTRINGCONTEXT_ATFIRST);
	special.DeleteCharacter(__C('"'), XSTRINGCONTEXT_ATEND);
	
	this->ifnonematch = special;
}





/*-------------------------------------------------------------------
//  DIOWEBSERVER_REQUEST::ConvertFromWeb
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			28/01/2011 19:08:09
//	
//	@return				bool : 
//	@param				string : 
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSERVER_REQUEST::ConvertFromWeb(XSTRING& string)
{
	int c = 0;

	XCHAR* str = string.Get();

	while(str[c])
		{
			if(str[c]==__C('%')) 
				{
					XSTRING string2;
						
					string2.Format(__L("%c%c"),str[c+2],str[c+1]);

					int code = string2.ConvertToInt();
					if(code)
						{
							str[c]=code;
							
							string.DeleteCharacters(c+1,2);
							str = string.Get();						
					  }   

				} else c++;
		}

  return true;
}




//-------------------------------------------------------------------
//  DIOWEBSERVER_REQUEST::GetLoginPassword
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			23/05/2007 0:28:37
//	
//	@return				bool : 
//	@param				login : 
//  @param				pass : 
*/
//-------------------------------------------------------------------
bool DIOWEBSERVER_REQUEST::GetLoginPassword(XSTRING& login,XSTRING& password)
{
	if(loginpassword.IsEmpty()) return false;

	XSTRING lp;

	lp.ConvertFromBase64(loginpassword);

	int index = lp.Find(__L(":"),true);
	if(index ==  XSTRING_NOTFOUND) return false;

	lp.Copy(0,index,login);
	lp.Copy(index+1,password);

	return true;
}




/*-------------------------------------------------------------------
//	DIOWEBSERVER_REQUEST::ProcessAcceptedEncodingFlags
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			14/07/2017 16:56:27
//	
//	@return 			void : 
//
//  @param				string : 
*/
/*-----------------------------------------------------------------*/
void DIOWEBSERVER_REQUEST::ProcessAcceptedEncodingFlags(XSTRING& string)
{
	if(!string.IsEmpty())
		{
			//DIEGO: Achtung! hay que calcular la Q=0 de los headers por si acaso es un navegador Obsoleto

			if (string.Find(__L("gzip"),		false) != NOTFOUND)	 acceptedencodingflags |= DIOWEBSERVER_ENCODINGFLAGS_GZIP;				
			if (string.Find(__L("deflate"),	false) != NOTFOUND)	 acceptedencodingflags |= DIOWEBSERVER_ENCODINGFLAGS_DEFLATE;				
			if (string.Find(__L("compress"),false) != NOTFOUND)	 acceptedencodingflags |= DIOWEBSERVER_ENCODINGFLAGS_COMPRESS;	
			if (string.Find(__L("sdhc"),		false) != NOTFOUND)	 acceptedencodingflags |= DIOWEBSERVER_ENCODINGFLAGS_SHAREDDICTIONARY;				
		}
}




/*-------------------------------------------------------------------
//  DIOWEBSERVER_REQUEST::Reset
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/06/2011 14:16:10
//	
//	@return				bool : 
//	*/
/*-----------------------------------------------------------------*/
bool DIOWEBSERVER_REQUEST::Reset()
{
	type										= DIOWEBSERVER_TYPE_SYMPLE;
	method									= DIOWEBSERVER_METHOD_UNKNOWN;
	size										= -1;
	acceptedencodingflags		=	DIOWEBSERVER_ENCODINGFLAGS_NONE;

	return true;
}






//-------------------------------------------------------------------
//  DIOWEBSERVER_CONNEXION::DIOWEBSERVER_CONNEXION
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/02/2007 17:46:33
//	
//	@return				 
//	*/
//-------------------------------------------------------------------
DIOWEBSERVER_CONNEXION::DIOWEBSERVER_CONNEXION()
{
	Clean();
}


//-------------------------------------------------------------------
//  DIOWEBSERVER_CONNEXION::~DIOWEBSERVER_CONNEXION
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/02/2007 17:46:41
//	
//	@return			 
//	*/
//-------------------------------------------------------------------
DIOWEBSERVER_CONNEXION::~DIOWEBSERVER_CONNEXION()
{
	End();

	Clean();
}



/*-------------------------------------------------------------------
//  DIOWEBSERVER_CONNEXION::Ini
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			07/04/2013 18:20:52
//	
//	@return 			bool : 

 
 
//  @param				xpublisher : 
//  @param				webserver : 
//  @param				DIOSTREAMTCPIPCONFIG *	diostreamcfg : 
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSERVER_CONNEXION::Ini(DIOWEBSERVER* webserver, DIOSTREAMTCPIPCONFIG*	diostreamcfg)
{		
	this->webserver			= webserver;
	this->diostreamcfg	= diostreamcfg;

	xtimer =xfactory->CreateTimer();
	if(!xtimer)							return false;

	xtimersendingpage = xfactory->CreateTimer();
	if(!xtimersendingpage)	return false;

	diostream = (DIOSTREAMTCPIP*)diofactory->CreateStreamIO(diostreamcfg);
	if(!diostream)					return false;

	threadconnexion   = CREATEXTHREAD(XTHREADGROUPID_DIOWEBSERVER, __L("DIOWEBSERVER_CONNEXION::Ini"), ThreadRunFunction, (void*)this);
	if(!threadconnexion)		return false;

  request = new DIOWEBSERVER_REQUEST();
	if(!request)						return false;

	return true;
}




/*-------------------------------------------------------------------
//  DIOWEBSERVER_CONNEXION::Activate
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			28/02/2013 20:01:56
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool DIOWEBSERVER_CONNEXION::Activate()
{
	if(!threadconnexion) return false;

	isactive = true;

	threadconnexion->Ini();

//XDEBUG_PRINTCOLOR(1, __L("WEB Server: Active connexion... [%08X]"), this);
	
	return true;
}





/*-------------------------------------------------------------------
//  DIOWEBSERVER_CONNEXION::Receiver
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			06/02/2008 12:23:58
//	
//	@return				bool : 
//	@param				buffer : 
//  @param				size : 
//  @param				timeout : 
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSERVER_CONNEXION::Receiver(XBYTE* buffer,int& size,int timeout)
{
	if(!xtimer)		 return false;
	if(!diostream) return false;
	if(!size)			 return false;

	int br;

	xtimer->Reset();

	while(1)
		{
			br = diostream->Read(buffer,size);
			if(br) break;

			if(xtimer->GetMeasureSeconds()>=(XDWORD)timeout) break;
		}
			
	if(!br) return false;
	size = br;

	return true;
}



/*-------------------------------------------------------------------
//	DIOWEBSERVER_CONNEXION::Send
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			14/07/2017 17:18:44
//	
//	@return 			bool : 
//
//  @param				buffer : 
//  @param				size : 
//  @param				timeout : 
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSERVER_CONNEXION::Send(XBYTE* buffer, int& size, int timeout)
{
	if(!diostream) return false;
	if(!size)			 return false;

	bool status = true;
	int  bw			= 0;

	bw = diostream->Write(buffer, size);
	if(size != bw) status = false;

	if(status)
		{
			if(!diostream->WaitToFlushOutXBuffer(timeout)) status = false;
		}

	return status;
}



/*-------------------------------------------------------------------
//	DIOWEBSERVER_CONNEXION::Send
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			14/07/2017 17:19:00
//	
//	@return 			bool : 
//
//  @param				xbuffer : 
//  @param				timeout : 
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSERVER_CONNEXION::Send(XBUFFER& xbuffer, int timeout)
{
  int size = xbuffer.GetSize();

  return Send(xbuffer.Get(), size, timeout);
}





/*-------------------------------------------------------------------
//	DIOWEBSERVER_CONNEXION::Send
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			14/07/2017 17:21:53
//	
//	@return 			bool : 
//
//  @param				string : 
//  @param				timeout : 
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSERVER_CONNEXION::Send(XSTRING& string, int timeout)
{
	int _size		 = string.GetSize();
	bool status	 = false;

	switch(actualencodingflag)
		{
			case DIOWEBSERVER_ENCODINGFLAGS_GZIP		:	{ XBUFFER* buffer = new XBUFFER(string.GetSize(),false);			
																									if(buffer)
																										{
																											XSTRING_CREATEOEM(string,oem);
																											buffer->Set((XBYTE*)oem,string.GetSize());
																											XSTRING_DELETEOEM(oem);

																											COMPRESSMANAGER*	manager	= new COMPRESSMANAGER();
																											if(manager)
																													{
																														COMPRESSBASE* compressor =	manager->Create(COMPRESSBASE_TYPE_GZ);				
																														if(compressor)
																															{
																																XBUFFER* compressed = new XBUFFER(string.GetSize(),false);
																																if(compressed)
																																	{
																																		status = compressor->Compress(buffer->Get(),buffer->GetSize(),compressed);	
																																		if(status)	
																																			{
																																				if(!Send(*compressed,timeout))	status = false;
																																			}
																																																								
																																		delete compressed;
																																	}

																																delete compressor;
																															}

																														delete manager;
																													}

																												delete buffer;
																											}																																										
																									}
																									break;

			case DIOWEBSERVER_ENCODINGFLAGS_NONE		:	{ XSTRING_CREATEOEM(string, charstr)
																									status = Send((XBYTE*)charstr, _size, timeout);	
																									XSTRING_DELETEOEM(charstr);																					
																								}
																								break;

		}


	return status;
}



/*-------------------------------------------------------------------
//	DIOWEBSERVER_CONNEXION::Send
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			14/07/2017 17:21:45
//	
//	@return 			bool : 
//
//  @param				string : 
//  @param				timeout : 
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSERVER_CONNEXION::Send(XCHAR* string, int timeout)
{
	XSTRING string2;

	string2 = string;

	return Send(string2, timeout);
}





/*-------------------------------------------------------------------
//	DIOWEBSERVER_CONNEXION::SendFile
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			14/07/2017 17:21:36
//	
//	@return 			bool : 
//
//  @param				xpath : 
//  @param				encodingflag : 
//  @param				timeout : 
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSERVER_CONNEXION::SendFile(XPATH& xpath, int timeout)
{
  XSTRING extension;
  bool    isplayable = false;
	bool    status     = true; 

	XSTRING result;
	
	xpath.GetExt(extension); 	
  if(webserver)
		{
			if(webserver->IsPlayablePage(extension)) isplayable = true;
		}
  	 
	if(isplayable)
		{
			if(!ParseFile(xpath, result, timeout)) return false;
		}
	 else	
	  {
			if(!ProcessFile(xpath, result, timeout)) return false;
		}
		
	return status;
}



/*-------------------------------------------------------------------
//	DIOWEBSERVER_CONNEXION::ParseFile
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			14/07/2017 17:21:27
//	
//	@return 			bool : 
//
//  @param				xpath : 
//  @param				encodingflag : 
//  @param				resultofparsing : 
//  @param				timeout : 
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSERVER_CONNEXION::ParseFile(XPATH& xpath, XSTRING& resultofparsing, int timeout)
{
  XFILETXT  filetxt;      
  bool      intag  = false;

  if(!filetxt.Open(xpath)) return false;

  if(filetxt.ReadAllFile())
    { 
      XBUFFER LFdata;  
      filetxt.GetLF(LFdata);

      for(int c=0;c<filetxt.GetNLines();c++)
        {
          XSTRING* line = filetxt.GetLine(c);
          if(line)
            { 
              XSTRING inioutput;
              XSTRING endoutput;
              int     result;
                                    
              result = SearchTAG(line,intag,inioutput,endoutput);
              switch(result)
                {
                  case DIOWEBSERVER_TAGSTATUS_NOTFOUND							: { if(!line->IsEmpty())
																																					 {
																																						 resultofparsing.Add(line->Get());
																																						 if(LFdata.GetSize()) resultofparsing.Add(LFdata);
																																					 }                                                                             
																																			}
																																			break;

                  case DIOWEBSERVER_TAGSTATUS_TAGFOUNDEND						: { if(!inioutput.IsEmpty())
																																					{
																																						resultofparsing.Add(inioutput.Get());
																																						if(LFdata.GetSize()) resultofparsing.Add(LFdata);
																																					}      

																																				DIOWEBSERVERXEVENT xevent(webserver,DIOWEBSERVERXEVENTTYPE_TAGFOUND);							
	
  																																			xevent.SetServer(webserver);
																																				xevent.SetConnexion(this);
																																				xevent.SetRequest(request);
																																				xevent.GetDataTAG()->Set(lastTAG);
																																				xevent.SetTimeOut(timeout);
	 
																																				webserver->PostEvent(&xevent);  //DIEGO : hay que tener en cuenta que los tags tiene que devolver un ouput;
		                                                                    
																																				resultofparsing.Add(xevent.GetOutput()->Get());

																																				if(!endoutput.IsEmpty())
																																					{
																																						resultofparsing.Add(endoutput.Get());
																																						if(LFdata.GetSize()) resultofparsing.Add(LFdata);																																						
																																					}              
																																			}  
																																			break;

                      case DIOWEBSERVER_TAGSTATUS_TAGFOUNDNOTEND		: { if(!inioutput.IsEmpty())
                                                                          {
																																						resultofparsing.Add(endoutput.Get());
																																						if(LFdata.GetSize())  resultofparsing.Add(LFdata);
                                                                          } 

																																				if(!endoutput.IsEmpty())
                                                                          {
																																						resultofparsing.Add(endoutput.Get());
																																						if(LFdata.GetSize()) resultofparsing.Add(LFdata);																																						
                                                                          }      
                                                                      }
                                                                      break; 
                }																	
            }
        }
     }

  filetxt.Close();
	
	return SendResult(resultofparsing, timeout);
}




/*-------------------------------------------------------------------
//	DIOWEBSERVER_CONNEXION::SendResult
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			14/07/2017 17:21:11
//	
//	@return 			bool : 
//
//  @param				resultofparsing : 
//  @param				timeout : 
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSERVER_CONNEXION::SendResult(XSTRING& resultofparsing, int timeout)
{	
	bool status = false;

	switch(actualencodingflag)
		{
			case DIOWEBSERVER_ENCODINGFLAGS_GZIP		 :	{ while(resultofparsing.GetSize() <= 150)
																											{ 
																												resultofparsing.Add(__C('\n'));
																											}

																										XBUFFER* buffer = new XBUFFER(resultofparsing.GetSize(),false);			
																										if(buffer)
																											{
																												XSTRING_CREATEOEM(resultofparsing,oem);
																												buffer->Set((XBYTE*)oem,resultofparsing.GetSize());
																												XSTRING_DELETEOEM(oem);

																												COMPRESSMANAGER* manager = new COMPRESSMANAGER();
																												if(manager)
																													{
																														COMPRESSBASE* compressor = manager->Create(COMPRESSBASE_TYPE_GZ);
																														if(compressor)
																															{
																																XBUFFER* compressed = new XBUFFER(resultofparsing.GetSize(),false);
																																if(compressed)			
																																	{
																																		status = compressor->Compress(buffer->Get(),buffer->GetSize(),compressed);	
																																		if(status) status = Send(*compressed, timeout);
																															
																																		delete compressed;
																																	}

																																delete compressor;
																															}

																														delete manager;
																													}
				
																												delete(buffer);																
																											}	
																									}
																									break;


			case DIOWEBSERVER_ENCODINGFLAGS_NONE		:		status = Send(resultofparsing, timeout);																					
																									break;

		}

	return status;
}




/*-------------------------------------------------------------------
//	DIOWEBSERVER_CONNEXION::ProcessFile
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			14/07/2017 17:21:01
//	
//	@return 			bool : 
//
//  @param				xpath : 
//  @param				resultofparsing : 
//  @param				timeout : 
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSERVER_CONNEXION::ProcessFile(XPATH& xpath, XSTRING& resultofparsing, int timeout)
{	
	bool status = true;	  	

  XFILE* file = xfactory->Create_File();
  if(!file) return false;		    

	if(file->Open(xpath))
		{
			XDWORD size = file->GetSize();

			XBUFFER* buffer = new XBUFFER(size,true);	
			if(buffer) 
				{
					int sizeread = size;

					file->Read(buffer->Get(), &sizeread);	

			    file->Close();	

					if(Send(*buffer))						
						{
							if(diostream->WaitToFlushOutXBuffer(5)) status = true;
						}
						
					delete(buffer);

		   } else status = false;
		}

	xfactory->Delete_File(file);

	return status;
}




//-------------------------------------------------------------------
//  DIOWEBSERVER_CONNEXION::End
/**
//	@version			02/02/2007 17:44:26
//	
//	
//	@author				Abraham J. Velez
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool DIOWEBSERVER_CONNEXION::End()
{				
	if(request)
		{
			delete request;
			request = NULL;
		}
		
	if(threadconnexion) 
		{
			threadconnexion->End();
			DELETEXTHREAD(XTHREADGROUPID_DIOWEBSERVER, threadconnexion);
			threadconnexion = NULL;
		}
	
	if(diostream)
		{										
			diostream->Close();

			diofactory->DeleteStreamIO(diostream);
			diostream = NULL;
		}
		
	if(xtimer) 
		{
			xfactory->DeleteTimer(xtimer);
			xtimer = NULL;
		}
	
	if(xtimersendingpage) 
		{
			xfactory->DeleteTimer(xtimersendingpage);
			xtimersendingpage = NULL;
		}
	
	isactive = false;
	pagestatus = DIOWEBSERVER_PAGESTATUS_SENT;	

	return true;
}



/*-------------------------------------------------------------------
//	DIOWEBSERVER_CONNEXION::SetActualEncodingFlag
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			17/07/2017 11:05:15
//	
//	@return 			bool : 
//
//  @param				actualencodingflag : 
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSERVER_CONNEXION::SetActualEncodingFlag(DIOWEBSERVER_ENCODINGFLAGS actualencodingflag)
{ 
	if(!webserver) return false;

	if(!webserver->IsIncludedEncodingFlag(actualencodingflag)) 
		{
			actualencodingflag = DIOWEBSERVER_ENCODINGFLAGS_NONE;		
			return false;
		}

	this->actualencodingflag = actualencodingflag;											

	return true;
}

/*-------------------------------------------------------------------
//	 DIOWEBSERVER_CONNEXION::GetAccessControlAllowHeaders
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	03/11/2017 10:46:03
//	@return		XCHAR* : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
XCHAR* DIOWEBSERVER_CONNEXION::GetAccessControlAllowHeaders()
{
	if(request) return request->accesscontrolallowheaders.Get();
	
	return NULL;
}



/*-------------------------------------------------------------------
//	 DIOWEBSERVER_CONNEXION::GetAccessControlAllowMethods
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	03/11/2017 11:02:31
//	@return		XCHAR* : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
XCHAR* DIOWEBSERVER_CONNEXION::GetAccessControlAllowMethods()
{
	if(request) return this->request->accesscontrolallowmethods.Get();
	
	return NULL;
}




/*-------------------------------------------------------------------
//	 DIOWEBSERVER_CONNEXION::GetAcceptLenguage
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	03/11/2017 11:09:04
//	@return		XCHAR* : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
XCHAR* DIOWEBSERVER_CONNEXION::GetAcceptLenguage()
{
	if(request) return request->acceptedlanguage.Get();
	
	return NULL;
}




/*-------------------------------------------------------------------
//	 DIOWEBSERVER_CONNEXION::GetOrigin
*/
/**
//
//	
//
//	@author		Abraham J. Velez 
//	@version	15/11/2017 11:21:55
//	@return		XCHAR* : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
XCHAR* DIOWEBSERVER_CONNEXION::GetOrigin()
{
	if(request) return request->origin.Get();
	
	return NULL;
}




/*-------------------------------------------------------------------
//  DIOWEBSERVER_CONNEXION::GetRequest
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			13/04/2013 10:31:17
//	
//	@return 			bool : 
//	@param				request : 
//  @param				timeout : 
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSERVER_CONNEXION::GetRequest(DIOWEBSERVER_REQUEST& request, int timeout)
{		
	if(!header.Read(diostream, xtimer, DIOWEBSERVER_DEFAULTCONNEXIONTIMEOUT))  return false;
	if(!header.GetLines()->GetSize()) return false;
	
	XSTRING string;
	string = (*(XSTRING*)(header.GetLines()->Get(0)));

 	if(string.Find(__L("HTTP/"),true,4)!=XSTRING_NOTFOUND)
		{
		  request.SetType(DIOWEBSERVER_TYPE_FULL);

		} else request.SetType(DIOWEBSERVER_TYPE_SYMPLE);

	header.GetMethod(string);

  //XDEBUG_PRINTCOLOR(1, __L("method: %s"), string.Get());

	if(!string.Compare(__L("GET"),true)) request.SetMethod(DIOWEBSERVER_METHOD_GET);
	  else if(!string.Compare(__L("POST"),true)) request.SetMethod(DIOWEBSERVER_METHOD_POST);
		  else if(!string.Compare(__L("HEAD"),true)) request.SetMethod(DIOWEBSERVER_METHOD_HEAD);
				else if (!string.Compare(__L("OPTIONS"), true)) 
						request.SetMethod(DIOWEBSERVER_METHOD_OPTIONS);
						else request.SetMethod(DIOWEBSERVER_METHOD_UNKNOWN);		

	header.GetResource(string);																		
	if(string.GetSize()) request.SetResource(string);

	string = header.GetFieldValue(__L("Host:"));													request.SetGuest												(string);
	string = header.GetFieldValue(__L("User-Agent:"));										request.SetUserAgent										(string);
	string = header.GetFieldValue(__L("Content-Length:"));								if(!string.IsEmpty()) request.SetSize		(string.ConvertToInt());
	string = header.GetFieldValue(__L("If-None-Match:"));									request.SetIfNoneMatch									(string);
	string = header.GetFieldValue(__L("Authorization: basic"));						request.SetLoginPassword								(string);
	string = header.GetFieldValue(__L("Accept-Encoding:"));								request.ProcessAcceptedEncodingFlags		(string);
	string = header.GetFieldValue(__L("Accept-Language:"));								request.ProcessAcceptedLanguage					(string);
	string = header.GetFieldValue(__L("Accept-Charset:"));								request.ProcessAcceptedCharset					(string);
	string = header.GetFieldValue(__L("Access-Control-Allow-Origin:"));		request.ProcessAccessControlAllowOrigin	(string);
	string = header.GetFieldValue(__L("Access-Control-Allow-Methods:"));	request.ProcessAccessControlAllowMethods(string);
	string = header.GetFieldValue(__L("Access-Control-Allow-Headers:"));	request.ProcessAccessControlAllowHeaders(string);				 
	string = header.GetFieldValue(__L("origin:"));												request.ProcessOrigin										(string);

	if(request.GetSize()>0) //DIEGO : no seria mejor comprobar esto ANTES de procesar todos estos ^ strings?
		{
			XBUFFER* data = request.GetData();
			if(!data) return false;
	
			int size;

			data->Delete();

			XBYTE* buffer = new XBYTE[DIOWEBSERVER_MAXBUFFER];
			if(!buffer) return false;
	
			do{ memset(buffer,0,DIOWEBSERVER_MAXBUFFER);
					size = (request.GetSize() - data->GetSize());
					if(size>DIOWEBSERVER_MAXBUFFER) size = DIOWEBSERVER_MAXBUFFER;

					if(size)
						{
							Receiver((XBYTE*)buffer,size);
							if(size) data->Add((XBYTE*)buffer,size);
						}

				} while(size);

			delete [] buffer;
		}
			
	return true;	
}




/*-------------------------------------------------------------------
//  DIOWEBSERVER_CONNEXION::DoLogPage
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			12/04/2013 23:14:52
//	
//	@return 			bool : 
//	@param				request : 
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSERVER_CONNEXION::DoLogPage(bool ini, DIOWEBSERVER_REQUEST& request)
{
	if(!webserver) return false;
	
	if(!XLOG::GetInstance().IsActive()) return false;
				
	XSTRING* resource = request.GetResource();
	if(!resource) return false;

	if(resource->IsEmpty())  return false;		

	XSTRING IPstring;
	GetDIOStream()->GetClientIP()->GetXString(IPstring);

	if(ini)	
		{
			XLOG::GetInstance().AddEntry(XLOGLEVEL_INFO, DIOWEBSERVER_LOGSECTIONID	, false, __L("[%08X] Server Page Ini Connexion [%s] %s"), this, IPstring.Get(), request.GetResource()->Get());			
	 }
	else
	 {
			XLOG::GetInstance().AddEntry(XLOGLEVEL_INFO, DIOWEBSERVER_LOGSECTIONID	, false, __L("[%08X] Server Page End Connexion [%s] %s"), this, IPstring.Get(), request.GetResource()->Get());
	 }
	
	return true;
}




//-------------------------------------------------------------------
//  DIOWEBSERVER_CONNEXION::ReceiverLine
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			31/01/2007 17:01:22
//	
//	@return				bool : 
//	@param				str : 
*/
//-------------------------------------------------------------------
bool DIOWEBSERVER_CONNEXION::ReceiverLine(XSTRING& string,int maxlen)
{	
	char data = 0;

	string.Empty();

  for(int c=1;c<maxlen-1;c++) 
		{	
			int size = 1;

			if(Receiver((XBYTE*)&data,size))
				{		
					if((data=='\n')||(data=='\r'))
						{	
							size = 1;

							if(Receiver((XBYTE*)&data,size)) 
								{
									if((data=='\n')||(data=='\r')) return true;
								}
						}
					 else 
						{ 
							XCHAR character[2] = { 0 , 0 };

							character[0] = data;
							string += character;							
						}

				} else return false;			 
		} 

  return true;
}



/*-------------------------------------------------------------------
//  DIOWEBSERVER_CONNEXION::ExistFile
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			24/06/2008 22:17:59
//	
//	@return				bool : 
//	@param				xpath : 
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSERVER_CONNEXION::ExistFile(XPATH& xpath)
{
	bool status = false;

	XFILE* file = xfactory->Create_File();
	if(file) 
		{
			if(file->Open(xpath))
				{					
					file->Close();

					status = true;
				} 

			xfactory->Delete_File(file);
		} 

	return status;
}




/*-------------------------------------------------------------------
//	DIOWEBSERVER_CONNEXION::SendingPage
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			01/09/2015 9:55:38
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSERVER_CONNEXION::SendingPage()
{
	if(!webserver) return false;
	if(!request)	 return false;
	if(!diostream) return false;
  			
	if(GetRequest((*request)))
		{				
			if(request->GetMethod()!=DIOWEBSERVER_METHOD_UNKNOWN)
				{								
					if(request) DoLogPage(true, (*request));
										
					DIOWEBSERVERXEVENT	xevent(webserver,DIOWEBSERVERXEVENTTYPE_REQUEST);							
	
															xevent.SetServer(webserver);
															xevent.SetConnexion(this);
															xevent.SetRequest(request);
	
					webserver->PostEvent(&xevent);
													
					if(request) DoLogPage(false, (*request));				

					return true;
				}	
			 else
				{
					//XDEBUG_PRINTCOLOR(4, __L("DIOWEBSERVER_CONNEXION::SendingPage : Unknown method"));
				}
		}

	return false;
}




/*-------------------------------------------------------------------
//	DIOWEBSERVER_CONNEXION::SearchTAG
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			30/12/2015 20:18:33
//	
//	@return 			DIOWEBSERVER_TAGSTATUS : 
//
//  @param				originalline : 
//  @param				intag : 
//  @param				inioutputline : 
//  @param				endoutputline : 
*/
/*-----------------------------------------------------------------*/
DIOWEBSERVER_TAGSTATUS DIOWEBSERVER_CONNEXION::SearchTAG(XSTRING* originalline,bool& intag, XSTRING& inioutputline,XSTRING& endoutputline)
{
  int index[3];   

  inioutputline.Empty();
  endoutputline.Empty();

  if(!intag)
    {
      index[0] = originalline->Find(__L("<?"),true);
      if(index[0]!=XSTRING_NOTFOUND)
        {
          index[1] = originalline->Find(DIOWEBSERVER_TAGLABEL, true);
          if(index[1]!=XSTRING_NOTFOUND)
            {
              lastTAG.Empty();

              originalline->Copy(0, index[0], inioutputline);

              index[2] = originalline->Find(__L("?>"), true);
              if(index[2]!=XSTRING_NOTFOUND)
                {
                  XSTRING endline;

                  originalline->Copy(index[1]+3, index[2], lastTAG);                  
                  originalline->Copy(index[2]+2, endoutputline);
                  
                  return DIOWEBSERVER_TAGSTATUS_TAGFOUNDEND;
                  
                } 
               else 
                { 
                  originalline->Copy(index[1]+3, lastTAG);
                  intag = true;
                  return DIOWEBSERVER_TAGSTATUS_TAGFOUNDNOTEND;
                }
            }
        }

      inioutputline = originalline->Get();
    }
   else 
    {    
      index[0] = originalline->Find(__L("?>"),true);
      if(index[0]!=XSTRING_NOTFOUND)
        {               
          XSTRING TAGstring;     

          originalline->Copy(0,index[0], TAGstring);
          lastTAG += TAGstring.Get();

          originalline->Copy(index[0]+2, endoutputline);

          intag = false;

          return DIOWEBSERVER_TAGSTATUS_TAGFOUNDEND;
        }
        
      lastTAG += originalline->Get();
      
      inioutputline.Empty();
      endoutputline.Empty();

      return DIOWEBSERVER_TAGSTATUS_TAGFOUNDNOTEND;
    }

  return DIOWEBSERVER_TAGSTATUS_NOTFOUND;
}






/*-------------------------------------------------------------------
//  DIOWEBSERVER_CONNEXION::ThreadRunFunction
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			19/02/2013 23:22:39
//	
//	@return 			void : 
//	@param				thread : 
*/
/*-----------------------------------------------------------------*/
void DIOWEBSERVER_CONNEXION::ThreadRunFunction(void* param)
{	
	DIOWEBSERVER_CONNEXION* webserverconnexion = (DIOWEBSERVER_CONNEXION*)param;
	if(!webserverconnexion) return;
	
	//XDEBUG_PRINTCOLOR(1, __L("WEB Server: Open connexion... [%08X]"), webserverconnexion);	
	
	webserverconnexion->xtimersendingpage->Reset();

	webserverconnexion->pagestatus = DIOWEBSERVER_PAGESTATUS_SENDING;	

	webserverconnexion->SendingPage();	
		
	webserverconnexion->threadconnexion->Run(false);	
	
	webserverconnexion->GetDIOStream()->WaitToFlushOutXBuffer(5);		

	webserverconnexion->GetDIOStream()->Disconnect();
	
	webserverconnexion->pagestatus = DIOWEBSERVER_PAGESTATUS_SENT;
		
	//XDEBUG_PRINTCOLOR(1, __L("WEB Server: Time server page %lu milliseconds"), webserverconnexion->xtimersendingpage->GetMeasureMilliSeconds());
	//XDEBUG_PRINTCOLOR(1, __L("WEB Server: Close connexion... [%08X]"), webserverconnexion);

	return;
}




/*-------------------------------------------------------------------
//	DIOWEBSERVER::DIOWEBSERVER
*/	
/**	
//	
//	Class Constructor DIOWEBSERVER
//	
//	@author				Abraham J. Velez
//	@version			09/05/2014 16:34:16
//	
 
 
 
//  @param				xpublisher : 
*/
/*-----------------------------------------------------------------*/
DIOWEBSERVER::DIOWEBSERVER()
{
	Clean();
}




//-------------------------------------------------------------------
//  DIOWEBSERVER::~DIOWEBSERVER
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			20/01/2007 21:13:27
//	
//	@return				
//	*/
//-------------------------------------------------------------------
DIOWEBSERVER::~DIOWEBSERVER()
{
	End();

	Clean();
}



/*-------------------------------------------------------------------
//	DIOWEBSERVER::Ini
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			12/09/2014 13:06:43
//	
//	@return 			bool : 
//
//  @param				port : 
//  @param				timeoutserverpage : 
//  @param				addrlocal : 
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSERVER::Ini(int port, int timeoutserverpage, XSTRING* addrlocal)	
{
	if(!xfactory) return false;

	isactive				= false;
	doexit					= false;

	diostreamcfg = new DIOSTREAMTCPIPCONFIG();
	if(!diostreamcfg) return false;

	this->port											= port;
	this->timeoutserverpage         = timeoutserverpage;
	if(addrlocal)	this->addrlocal   = (*addrlocal);

	AddPlayablePageExtension(__L(".HTML"));
  AddPlayablePageExtension(__L(".HTM"));

	RegisterEvent(DIOWEBSERVERXEVENTTYPE_REQUEST);
	RegisterEvent(DIOWEBSERVERXEVENTTYPE_TAGFOUND);

	xtimercontrol = xfactory->CreateTimer();
	if(!xtimercontrol) return false;

	diostreamcfg->SetMode(DIOSTREAMMODE_SERVER);
	diostreamcfg->GetLocalIP()->Set(this->addrlocal);
	diostreamcfg->SetRemotePort(port);		
	
	isactive = true;	

	xmutexconnexions = xfactory->Create_Mutex();
	if(!xmutexconnexions) return false;	

	bool status = true;

	// ------------------------------------------------------------------------------------ 
	// TEST Can open socket for WEB server
		
	xtimercontrol->Reset();

	DIOSTREAMTCPIP* diostream = (DIOSTREAMTCPIP*)diofactory->CreateStreamIO(diostreamcfg);
	if(diostream)
		{
			if(diostream->Open())
				{
					while(1)
						{
							if(diostream->GetConnectStatus() == DIOSTREAMSTATUS_DISCONNECTED)							
								{
									status = false;
									break;
								}

							if(xtimercontrol->GetMeasureSeconds() > 3) break;
						}

					diostream->Close();					

				} else status = false;

			diofactory->DeleteStreamIO(diostream);
		}

	if(!status) return false;
	

	// ------------------------------------------------------------------------------------ 
	
	xtimercontrol->Reset();

	threadserver = CREATEXTHREAD(XTHREADGROUPID_DIOWEBSERVER, __L("DIOWEBSERVER::Ini"),ThreadRunFunction,(void*)this);
	if(!threadserver) return false;
	
	if(!threadserver->Ini()) return false;
	
	if(addrlocal) XLOG::GetInstance().AddEntry(XLOGLEVEL_INFO, DIOWEBSERVER_LOGSECTIONID	, false , __L("Web Server [%08X]: %s in %s:%d "), this, status?__L("Active"):__L("Disable"),  addrlocal->IsEmpty()?__L("Default"):addrlocal->Get(), port);				
	
	return status;
}



//-------------------------------------------------------------------
//  DIOWEBSERVER::IsSendingPage
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			31/07/2007 14:49:31
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool DIOWEBSERVER::IsSendingAnyPage()
{		
	bool sending = false;

	if(!connexions.IsEmpty()) 
		{
			for(XDWORD c=0;c<connexions.GetSize();c++)
				{
					DIOWEBSERVER_CONNEXION* connexion = (DIOWEBSERVER_CONNEXION*)connexions.Get(c);
					if(connexion)
						{
							if(connexion->GetPageStatus() == DIOWEBSERVER_PAGESTATUS_SENDING) 
								{
									sending = true;
									break;
								}
						}
				}
		}

	return sending;
}



/*-------------------------------------------------------------------
//	DIOWEBSERVER::UnSubscribeAllPages
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			16/05/2016 12:29:45
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSERVER::UnSubscribeAllPages()
{	
	
	if(!connexions.IsEmpty()) 
		{
			for(XDWORD c=0;c<connexions.GetSize();c++)
				{
					DIOWEBSERVER_CONNEXION* connexion = (DIOWEBSERVER_CONNEXION*)connexions.Get(c);
					if(connexion)
						{
							UnSubscribeEvent(DIOSTREAMXEVENTTYPE_CONNECTED		, connexion->GetDIOStream());
							UnSubscribeEvent(DIOSTREAMXEVENTTYPE_DISCONNECTED , connexion->GetDIOStream());							
						}
				} 
		}

	return true;
}




/*-------------------------------------------------------------------
//  DIOWEBSERVER::AddAuthentication
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/05/2009 11:31:39
//	
//	@return				bool : 
//	@param				guest : 
//  @param				login : 
//  @param				password : 
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSERVER::AddAuthentication(XSTRING& guest,XSTRING& login,XSTRING& password)
{
	DIOWEBSERVER_AUTHENTICATION* authentication = GetAuthentication(guest);
	if(authentication)
		{
			authentication->Set(login,password);			
		}
 	 else
		{
			authentication = new DIOWEBSERVER_AUTHENTICATION();
			if(!authentication) return false;

			authentication->Set(guest,login,password);	
						
			authentications.Add(authentication);
		}

	return true;
}



/*-------------------------------------------------------------------
//  DIOWEBSERVER::GetAuthentication
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/05/2009 11:31:27
//	
//	@return				DIOWEBSERVER_AUTHENTICATION* : 
//	@param				guest : 
*/
/*-----------------------------------------------------------------*/
DIOWEBSERVER_AUTHENTICATION* DIOWEBSERVER::GetAuthentication(XSTRING& guest)
{
	if(authentications.IsEmpty()) return NULL;
	
	DIOWEBSERVER_AUTHENTICATION* authentication = NULL;
	
	for(int c=0;c<(int)authentications.GetSize();c++)
		{
			DIOWEBSERVER_AUTHENTICATION* auth = (DIOWEBSERVER_AUTHENTICATION*)authentications.Get(c);
			if(auth)
			  {
					if(auth->GetGuest())
						{
							XSTRING* tguest = auth->GetGuest();
							if(tguest)
								{
									if((*tguest) == guest)
										{
											authentication = auth;
											break;
										}
								}
						}
			  }
		}

	return authentication;
}



/*-------------------------------------------------------------------
//  DIOWEBSERVER::DelAuthentication
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/05/2009 11:31:19
//	
//	@return				bool : 
//	@param				guest : 
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSERVER::DelAuthentication(XSTRING& guest)
{
	DIOWEBSERVER_AUTHENTICATION* authentication = GetAuthentication(guest);
	if(!authentication) return false;

	authentications.Delete(authentication);
	
	delete authentication;

	return true;
}



/*-------------------------------------------------------------------
//  DIOWEBSERVER::DelAllAuthentication
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/05/2009 11:31:05
//	
//	@return				bool : 
//	*/
/*-----------------------------------------------------------------*/
bool DIOWEBSERVER::DelAllAuthentication()
{
	if(authentications.IsEmpty()) return false;

	authentications.DeleteContents();

	authentications.DeleteAll();

  return true;
}



/*-------------------------------------------------------------------
//  DIOWEBSERVER::IsPlayablePage
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      05/10/2012 9:26:24
//  
//  @return       bool : 
//  @param        extension : 
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSERVER::IsPlayablePage(XCHAR* extension)
{
  if(playablepageextensions.IsEmpty()) return false;

  for(int c=0;c<(int)playablepageextensions.GetSize();c++)
    {
      XSTRING* _extension = (XSTRING*)playablepageextensions.Get(c);
      if(_extension)
        {
          if(!_extension->Compare(extension,true)) return true;
        }
    }  

  return false;
}



/*-------------------------------------------------------------------
//  DIOWEBSERVER::IsPlayablePage
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      05/10/2012 9:27:07
//  
//  @return       bool : 
//  @param        extension : 
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSERVER::IsPlayablePage(XSTRING& extension)
{
  return IsPlayablePage(extension.Get()); 
}




/*-------------------------------------------------------------------
//  DIOWEBSERVER::AddPlayablePageExtension
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      05/10/2012 9:27:13
//  
//  @return       bool : 
//  @param        extension : 
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSERVER::AddPlayablePageExtension(XCHAR* extension)
{
  if(!extension) return false;

  XSTRING* _string = new XSTRING(extension);
  if(!_string) return false;

  playablepageextensions.Add(_string);    

  return true;
}



/*-------------------------------------------------------------------
//  DIOWEBSERVER::AddPlayablePageExtension
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      05/10/2012 9:27:18
//  
//  @return       bool : 
//  @param        extension : 
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSERVER::AddPlayablePageExtension(XSTRING& extension)
{  
  XSTRING* _string = new XSTRING(extension);
  if(!_string) return false;

  playablepageextensions.Add(_string);    

  return true;
}




/*-------------------------------------------------------------------
//  DIOWEBSERVER::DeleteAllPlayablePageExtensions
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      05/10/2012 9:27:43
//  
//  @return       bool : 
//  */
/*-----------------------------------------------------------------*/
bool DIOWEBSERVER::DeleteAllPlayablePageExtensions()
{
  if(playablepageextensions.IsEmpty()) return false;
  
	playablepageextensions.DeleteContents();
  playablepageextensions.DeleteAll();

  return true;
}



//-------------------------------------------------------------------
//  DIOWEBSERVER::End
/**
//
//
//	@author				Abraham J. Velez
//	@version			21/01/2007 19:14:01
//
//	@return				bool :
//	*/
//-------------------------------------------------------------------
bool DIOWEBSERVER::End()
{	
	doexit = true; 
	 
	XLOG::GetInstance().AddEntry(XLOGLEVEL_INFO, DIOWEBSERVER_LOGSECTIONID	, false , __L("Disable web server [%08X]."), this);	
	
	UnSubscribeAllPages();
	
	XLOG::GetInstance().AddEntry(XLOGLEVEL_INFO, NULL , false , __L("Delete main thread ... "));
	
	if(threadserver)
		{
			threadserver->End();
			DELETEXTHREAD(XTHREADGROUPID_DIOWEBSERVER, threadserver);		
			threadserver = NULL;
		}		
			
	XLOG::GetInstance().AddEntry(XLOGLEVEL_INFO, NULL	, false , __L("Wait to send all pages ..."));	
	WaitToSentAllPages();	

	XLOG::GetInstance().AddEntry(XLOGLEVEL_INFO, NULL	, false , __L("Delete connexions not use ..."));			
	Connexions_DeleteUsed();	

	XLOG::GetInstance().AddEntry(XLOGLEVEL_INFO, NULL	, false , __L("Delete connexions waiting ..."));			
	Connexions_DeleteWaiting();

	XLOG::GetInstance().AddEntry(XLOGLEVEL_INFO, NULL , false , __L("Delete all connexions ... "));	
	Connexions_DeleteAll(); 	
	
	if(xmutexconnexions) 
		{
			xfactory->Delete_Mutex(xmutexconnexions);
			xmutexconnexions = NULL;
		}
	
	XLOG::GetInstance().AddEntry(XLOGLEVEL_INFO, NULL , false , __L("Delete resources ... "));
				
	if(xtimercontrol) 
		{
			xfactory->DeleteTimer(xtimercontrol);
			xtimercontrol = NULL;
		}

	DeleteAllPlayablePageExtensions();

	DelAllAuthentication();	

	XLOG::GetInstance().AddEntry(XLOGLEVEL_INFO, NULL , false , __L("Delete events ... "));

	DeRegisterEvent(DIOWEBSERVERXEVENTTYPE_REQUEST);
	DeRegisterEvent(DIOWEBSERVERXEVENTTYPE_TAGFOUND);

	XLOG::GetInstance().AddEntry(XLOGLEVEL_INFO, NULL , false , __L("Delete stream config ... "));

	if(diostreamcfg)
		{
			delete diostreamcfg;		
			diostreamcfg = NULL;
		}

	XLOG::GetInstance().AddEntry(XLOGLEVEL_INFO, NULL , false , __L("End web server [%08X]."), this);

  return true;
}




/*-------------------------------------------------------------------
//  DIOWEBSERVER::WaitToSentPages
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			25/02/2013 21:50:37
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool DIOWEBSERVER::WaitToSentAllPages()
{
	if(!connexions.IsEmpty()) 
		{
			if(xtimercontrol) 
				{					
					do{  xsleep->MilliSeconds(10);
			 
						} while(IsSendingAnyPage());					
				}
		}

	return false;
}




/*-------------------------------------------------------------------
//	DIOWEBSERVER::Connexions_GetNWaiting
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			13/05/2016 9:55:35
//	
//	@return 			XDWORD : 
//
*/
/*-----------------------------------------------------------------*/
XDWORD DIOWEBSERVER::Connexions_GetNWaiting()
{		
	if(xmutexconnexions) xmutexconnexions->Lock();

	int nconnexionswait = 0;

	if(!connexions.IsEmpty()) 
		{			
			for(XDWORD c=0;c<connexions.GetSize();c++)
				{
					DIOWEBSERVER_CONNEXION* connexion = (DIOWEBSERVER_CONNEXION*)connexions.Get(c);
					if(connexion)
						{
							if((connexion->GetPageStatus() == DIOWEBSERVER_PAGESTATUS_NONE)  && (connexion->GetDIOStream()->GetConnectStatus() == DIOSTREAMSTATUS_GETTINGCONNEXION))
								{
									nconnexionswait++;
								}
						}
				}
		}
	
	if(xmutexconnexions) xmutexconnexions->UnLock();

	return nconnexionswait;
}




/*-------------------------------------------------------------------
//	DIOWEBSERVER::Connexions_GetNConnexionsSendingPage
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			19/08/2015 21:47:15
//	
//	@return 			int : 
//
*/
/*-----------------------------------------------------------------*/
int DIOWEBSERVER::Connexions_GetNConnexionsSendingPage()
{		
	int nconnexions = 0;

	if(!connexions.IsEmpty()) 
		{
			for(XDWORD c=0;c<connexions.GetSize();c++)
				{
					DIOWEBSERVER_CONNEXION* connexion = (DIOWEBSERVER_CONNEXION*)connexions.Get(c);
					if(connexion)
						{
							if(connexion->GetPageStatus() == DIOWEBSERVER_PAGESTATUS_SENDING) 
								{
									nconnexions++;
								}					
						}
				}
		}
	
	return nconnexions;
}




/*-------------------------------------------------------------------
//	DIOWEBSERVER::Connexions_CreateNew
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			16/05/2016 17:22:39
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSERVER::Connexions_CreateNew()
{	
	XDWORD nwaiting = Connexions_GetNWaiting();
	if(nwaiting >= DIOWEBSERVER_MAXPAGECONNEXIONS)  return false;
	
	if(xmutexconnexions) xmutexconnexions->Lock();

	DIOWEBSERVER_CONNEXION* connexion = new DIOWEBSERVER_CONNEXION();
	if(connexion)  
		{			
			if(connexion->Ini(this, diostreamcfg)) 
				{									
					if(connexion->GetDIOStream()->Open()) 
						{									
							SubscribeEvent(DIOSTREAMXEVENTTYPE_CONNECTED		, connexion->GetDIOStream());
							SubscribeEvent(DIOSTREAMXEVENTTYPE_DISCONNECTED , connexion->GetDIOStream());

							connexions.Add(connexion);	
							
							if(xmutexconnexions) xmutexconnexions->UnLock();

							return true;
						}

				 	connexion->End();						
				}

			delete connexion;
		}				
	
	if(xmutexconnexions) xmutexconnexions->UnLock();

	return false;
}




/*-------------------------------------------------------------------
//	DIOWEBSERVER::Connexions_DeleteUsed
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			05/05/2015 16:02:39
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSERVER::Connexions_DeleteUsed()
{		
	if(xmutexconnexions) xmutexconnexions->Lock();

  if(!connexions.IsEmpty()) 
		{
			int c = 0;

			do{ DIOWEBSERVER_CONNEXION* connexion = (DIOWEBSERVER_CONNEXION*)connexions.Get(c);
					if(connexion)
						{							
							if((connexion->GetPageStatus() == DIOWEBSERVER_PAGESTATUS_SENT) && (connexion->GetDIOStream()->GetConnectStatus() == DIOSTREAMSTATUS_DISCONNECTED))
								{									
									UnSubscribeEvent(DIOSTREAMXEVENTTYPE_CONNECTED		, connexion->GetDIOStream());
									UnSubscribeEvent(DIOSTREAMXEVENTTYPE_DISCONNECTED , connexion->GetDIOStream());														

									connexions.Delete(connexion);
									delete connexion;
									 																
									
								} else c++;
						}

				} while(c<(int)connexions.GetSize());
		}

	if(xmutexconnexions) xmutexconnexions->UnLock();

	return true;
}





/*-------------------------------------------------------------------
//	DIOWEBSERVER::Connexions_DeleteWaiting
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			15/05/2016 16:36:12
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSERVER::Connexions_DeleteWaiting()
{	
  if(!connexions.IsEmpty()) 
		{
			int c = 0;

			do{ DIOWEBSERVER_CONNEXION* connexion = (DIOWEBSERVER_CONNEXION*)connexions.Get(c);
					if(connexion)
						{							
							if((connexion->GetPageStatus() == DIOWEBSERVER_PAGESTATUS_NONE)  && (connexion->GetDIOStream()->GetConnectStatus() == DIOSTREAMSTATUS_GETTINGCONNEXION))
								{	
									connexions.Delete(connexion); 																																					
									delete connexion;									

								} else c++;
						}

				} while(c<(int)connexions.GetSize());
		}

	return true;
}




/*-------------------------------------------------------------------
//	DIOWEBSERVER::Connexions_DeleteAll
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			05/05/2015 16:02:53
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSERVER::Connexions_DeleteAll()
{		
	if(xmutexconnexions) xmutexconnexions->Lock();

  if(!connexions.IsEmpty()) 
		{
			connexions.DeleteContents();
			connexions.DeleteAll();
		}

	if(xmutexconnexions) xmutexconnexions->UnLock();
			
	return true;
}





/*-------------------------------------------------------------------
//	DIOWEBSERVER::HandleEvent
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			13/05/2016 11:51:45
//	
//  @param				xevent : 
*/
/*-----------------------------------------------------------------*/
void DIOWEBSERVER::HandleEvent(XEVENT* xevent)
{
	if(!xevent) return;

	switch(xevent->GetEventFamily())
		{		
			case XEVENTTYPE_DIOSTREAM		: { DIOSTREAMXEVENT* event = (DIOSTREAMXEVENT*)xevent;
																			if(!event) return;
																		
																			switch(xevent->GetEventType())
																				{	
																					case DIOSTREAMXEVENTTYPE_CONNECTED		: { for(XDWORD c=0; c<connexions.GetSize(); c++)
																																											{
																																												DIOWEBSERVER_CONNEXION* connexion = (DIOWEBSERVER_CONNEXION*)connexions.Get(c);
																																												if(connexion)
																																													{
																																														if(connexion->GetDIOStream() == event->GetDIOStream()) 
																																															{																																																																																																	
																																																connexion->Activate();																																																	
																																																break;
																																															}
																																													}
																																											}																																																							
																																									}
																																									break;

																					case DIOSTREAMXEVENTTYPE_DISCONNECTED : break;
																				}
																		}
																		break;
		}
}




//-------------------------------------------------------------------
//  DIOWEBSERVER::ThreadRunFunction
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			21/01/2007 22:16:45
//	
//	@return				void : 
//	@param				thread : 
*/
//-------------------------------------------------------------------
void DIOWEBSERVER::ThreadRunFunction(void* param)
{	
	DIOWEBSERVER* webserver = (DIOWEBSERVER*)param;
	if(!webserver) return;
	
	if(!webserver->diostreamcfg) return;
	
	if(webserver->doexit) return;
	
	webserver->Connexions_CreateNew();	
	webserver->Connexions_DeleteUsed();			
}

