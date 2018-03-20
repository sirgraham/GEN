/*------------------------------------------------------------------------------------------
//	DIOWEBSERVERQUERYSTRING.CPP
//	
//	DIO Web Server Query String
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 02/06/2013 11:41:40
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "XString.h"

#include "DIOWebServerQueryString.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  DIOWEBSERVERQUERYSTRING::DIOWEBSERVERQUERYSTRING
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/06/2013 11:43:22
//	
//	@return 			
*/
/*-----------------------------------------------------------------*/
DIOWEBSERVERQUERYSTRING::DIOWEBSERVERQUERYSTRING()
{
	Clean();
}



/*-------------------------------------------------------------------
//  DIOWEBSERVERQUERYSTRING::~DIOWEBSERVERQUERYSTRING
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/06/2013 11:43:33
//	
//	@return 			
//	*/
/*-----------------------------------------------------------------*/
DIOWEBSERVERQUERYSTRING::~DIOWEBSERVERQUERYSTRING()
{
 	DelAllParam();

	Clean();
}
		


/*-------------------------------------------------------------------
//  DIOWEBSERVERQUERYSTRING::GetNParams
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/06/2013 12:23:01
//	
//	@return 			int : 
//	*/
/*-----------------------------------------------------------------*/
int DIOWEBSERVERQUERYSTRING::GetNParams()
{
	return params.GetSize();
}




/*-------------------------------------------------------------------
//  DIOWEBSERVERQUERYSTRING::GetParams
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/06/2013 13:32:15
//	
//	@return 			XMAP* : 
//	*/
/*-----------------------------------------------------------------*/
XMAP<XSTRING*, XSTRING*>* DIOWEBSERVERQUERYSTRING::GetParams()
{
	return &params;
}



/*-------------------------------------------------------------------
//  DIOWEBSERVERQUERYSTRING::AddParam
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/06/2013 12:23:44
//	
//	@return 			bool : 
//	@param				ID : 
//  @param				value : 
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSERVERQUERYSTRING::AddParam(XSTRING& ID, XSTRING& value)
{
	return AddParam(ID.Get(), value.Get());
}




/*-------------------------------------------------------------------
//  DIOWEBSERVERQUERYSTRING::AddParam
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/06/2013 12:23:53
//	
//	@return 			bool : 
//	@param				ID	 : 
//  @param				value : 
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSERVERQUERYSTRING::AddParam(XCHAR* ID	, XSTRING& value)
{
	return AddParam(ID, value.Get());
}




/*-------------------------------------------------------------------
//  DIOWEBSERVERQUERYSTRING::AddParam
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/06/2013 12:24:00
//	
//	@return 			bool : 
//	@param				ID : 
//  @param				value : 
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSERVERQUERYSTRING::AddParam(XSTRING& ID, XCHAR* value)
{
	return AddParam(ID.Get(), value);
}
	


/*-------------------------------------------------------------------
//  DIOWEBSERVERQUERYSTRING::AddParam
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/06/2013 12:25:16
//	
//	@return 			bool : 
//	@param				ID	 : 
//  @param				value : 
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSERVERQUERYSTRING::AddParam(XCHAR* ID, XCHAR* value)
{
	XSTRING* _ID		= new XSTRING();
	XSTRING* _value = new XSTRING();

	if((!_ID) || (!_value))
		{
			delete _ID;
			delete _value;

			return false;
		}

	_ID->Set(ID);
	_value->Set(value);

	params.Add(_ID, _value);

	return true;
}



/*-------------------------------------------------------------------
//  DIOWEBSERVERQUERYSTRING::GetParam
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/06/2013 12:25:37
//	
//	@return 			XSTRING* : 
//	@param				ID : 
*/
/*-----------------------------------------------------------------*/
XSTRING* DIOWEBSERVERQUERYSTRING::GetParam(XSTRING& ID)
{
	return GetParam(ID.Get());
}
	


/*-------------------------------------------------------------------
//  DIOWEBSERVERQUERYSTRING::GetParam
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/06/2013 12:26:05
//	
//	@return 			XSTRING* : 
//	@param				ID : 
*/
/*-----------------------------------------------------------------*/
XSTRING* DIOWEBSERVERQUERYSTRING::GetParam(XCHAR* ID)
{
	if(params.IsEmpty()) return NULL;

	for(int c=0;c<(int)params.GetSize();c++)
		{
			XSTRING* _ID = params.GetKey(c);
			if(_ID)
				{
					if(!_ID->Compare(ID, true)) 
						{
							XSTRING* _value = params.GetElement(c);
							return _value;
						}
				}
		}

	return NULL;
}




/*-------------------------------------------------------------------
//  DIOWEBSERVERQUERYSTRING::GetParamID
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/06/2013 16:38:06
//	
//	@return 			XSTRING* : 
//	@param				index : 
*/
/*-----------------------------------------------------------------*/
XSTRING* DIOWEBSERVERQUERYSTRING::GetParamID(int index)
{
	if(params.IsEmpty()) return NULL;

	return (XSTRING*)params.GetKey(index);
}




/*-------------------------------------------------------------------
//  DIOWEBSERVERQUERYSTRING::GetParamValue
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/06/2013 16:38:13
//	
//	@return 			XSTRING* : 
//	@param				index : 
*/
/*-----------------------------------------------------------------*/
XSTRING* DIOWEBSERVERQUERYSTRING::GetParamValue(int index)
{
	if(params.IsEmpty()) return NULL;

	return params.GetElement(index);
}




/*-------------------------------------------------------------------
//  DIOWEBSERVERQUERYSTRING::DelParam
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/06/2013 12:26:56
//	
//	@return 			bool : 
//	@param				ID : 
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSERVERQUERYSTRING::DelParam(XSTRING& ID)
{
	return DelParam(ID.Get());
}




/*-------------------------------------------------------------------
//  DIOWEBSERVERQUERYSTRING::DelParam
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/06/2013 12:27:00
//	
//	@return 			bool : 
//	@param				ID : 
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSERVERQUERYSTRING::DelParam(XCHAR* ID)
{
	XSTRING* _ID = DIOWEBSERVERQUERYSTRING::GetParam(ID);
	if(!_ID) return false;

	return params.Delete(_ID);	
}




/*-------------------------------------------------------------------
//  DIOWEBSERVERQUERYSTRING::DelAllParam
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/06/2013 12:27:05
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool DIOWEBSERVERQUERYSTRING::DelAllParam()
{
	if(params.IsEmpty())  return false;

	params.DeleteKeyContents();
	params.DeleteElementContents();

	params.DeleteAll();

	return true;
}





/*-------------------------------------------------------------------
//  DIOWEBSERVERQUERYSTRING::GetParamsFromURL
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/06/2013 16:26:32
//	
//	@return 			int : 
//	@param				url : 
*/
/*-----------------------------------------------------------------*/
int DIOWEBSERVERQUERYSTRING::GetParamsFromURL(DIOURL& url)
{																
	return GetParamsFromURL(url.Get());
}
	  



/*-------------------------------------------------------------------
//  DIOWEBSERVERQUERYSTRING::GetParamsFromURL
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/06/2013 17:19:27
//	
//	@return 			int : 
//	@param				url : 
*/
/*-----------------------------------------------------------------*/
int DIOWEBSERVERQUERYSTRING::GetParamsFromURL(XSTRING& url)
{																
	return GetParamsFromURL(url.Get());
}
	  


/*-------------------------------------------------------------------
//  DIOWEBSERVERQUERYSTRING::GetParamsFromURL
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/06/2013 16:26:37
//	
//	@return 			int : 
//	@param				url : 
*/
/*-----------------------------------------------------------------*/
int DIOWEBSERVERQUERYSTRING::GetParamsFromURL(XCHAR* url)
{
	DIOURL	_url;
	int			start;
	int     separator;
	int			end;
	XSTRING param;

	_url = url;

	start = _url.Find(__L("?"),true);
	if(start != XSTRING_NOTFOUND)
		{
			XSTRING ID;
			XSTRING value;
			
			start++;

			do{	
					value.Empty();
					end = _url.Find(__L("&"),true,start);
					if(end == XSTRING_NOTFOUND)  
								_url.Copy(start, param);							
					 else _url.Copy(start, end, param);	

					separator = param.Find(__L("="),true);
					if(separator == XSTRING_NOTFOUND)
						{
							ID = param;							
						}
					 else
					  {						
							param.Copy(0 , separator,ID);							
							param.Copy(separator+1, value);												
						}

					AddParam(ID, value);

					start = end+1;
	
				} while(end != XSTRING_NOTFOUND);
		}


	return GetNParams();
}



/*-------------------------------------------------------------------
//  DIOWEBSERVERQUERYSTRING::CreateURLFromParams
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/06/2013 16:24:43
//	
//	@return 			bool : 
//	@param				urlpart : 
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSERVERQUERYSTRING::CreateURLFromParams(XSTRING& urlpart)
{
	urlpart.Empty();

	if(params.IsEmpty()) return false;

	for(int c=0;c<(int)params.GetSize();c++)
		{
			XSTRING* _ID = (XSTRING*)params.GetKey(c);
			if(_ID)
				{					
					XSTRING* _value = params.GetElement(c);
					if(_value)
						{
							if(urlpart.IsEmpty())
										urlpart += __L("?");
							 else urlpart += __L("&");

							urlpart += _ID->Get();
							urlpart += __L("=");
							urlpart += _value->Get();
						}
				}
		}

	return true;
}




/*-------------------------------------------------------------------
//  DIOWEBSERVERQUERYSTRING::Clean
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/06/2013 11:43:46
//	
//	@return 			void : 
//	*/
/*-----------------------------------------------------------------*/
void DIOWEBSERVERQUERYSTRING::Clean()
{

}

