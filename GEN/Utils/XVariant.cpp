
/*------------------------------------------------------------------------------------------
//	XVARIANT.CPP
//	
//	Variant type
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 12/08/2015 13:49:37
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

/*---- INCLUDES --------------------------------------------------------------------------*/
	

#include "XFactory.h"
#include "XSerializable.h"

#include "XVariant.h"
#include "XDebug.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/




/*-------------------------------------------------------------------
//	XVARIANT::XVARIANT
*/	
/**	
//	
//	Class Constructor XVARIANT
//	
//	@author				Abraham J. Velez
//	@version			22/01/2016 16:55:17
//	
//  @param				variant : 
*/
/*-----------------------------------------------------------------*/
XVARIANT::XVARIANT(const XVARIANT& variant)
{
	Clean(); 
	
	(*this) = variant;
}



/*-------------------------------------------------------------------
//	XVARIANT::~XVARIANT
*/	
/**	
//	
//	 Class Destructor XVARIANT
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			12/08/2015 13:50:25
//	
*/
/*-----------------------------------------------------------------*/
XVARIANT::~XVARIANT	()	
{		
	if(data) Destroy();

	Clean();
}



/*-------------------------------------------------------------------
//	=
*/	
/**	
//	operator
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			12/08/2015 13:50:58
//	
//	@return 			XVARIANT& : 
//
//  @param				integer : 
*/
/*-----------------------------------------------------------------*/
const XVARIANT& XVARIANT::operator = (int integer)
{
	if(data) Destroy();

	type	= XVARIANT_TYPE_INTEGER;
	size	= sizeof(int);
	data	= (void*)new int;

	if(data)  
		{
			*((int*)data) = integer;						
		}	

	return *this;
}




/*-------------------------------------------------------------------
//	=
*/	
/**	
//	operator
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			12/08/2015 13:50:58
//	
//	@return 			XVARIANT& : 
//
//  @param				integer : 
*/
/*-----------------------------------------------------------------*/
const XVARIANT& XVARIANT::operator = (XDWORD integer)
{
	if(data)	Destroy();

	type = XVARIANT_TYPE_INTEGER;
	size = sizeof(XDWORD);

	data= (void*)new XDWORD;
	if(data)  
		{
			*((XDWORD*)data)	= integer;
		}

	return (*this);
}





/*-------------------------------------------------------------------
//	 XVARIANT& XVARIANT::operator = 
*/
/**
//	
//
//	@author		Abraham J. Velez 
//	@version	12/03/2018 10:50:45
//
//	@return		const : 
//
//	@param		XQWORD : 
//
*//*-----------------------------------------------------------------*/
const XVARIANT& XVARIANT::operator = (XQWORD integer)
{
	if(data) Destroy();

	type = XVARIANT_TYPE_INTEGER;
	size = sizeof(XQWORD);

	data= (void*)new XQWORD;
	if(data)  
		{
			*((XQWORD*)data)	= integer;
		}

	return (*this);
}




/*-------------------------------------------------------------------
//	 XVARIANT& XVARIANT::operator = 
*/
/**
//	
//
//	@author		Abraham J. Velez 
//	@version	12/03/2018 10:50:36
//
//	@return		const : 
//
//	@param		float : 
//
*//*-----------------------------------------------------------------*/
const XVARIANT& XVARIANT::operator = (float value)
{
	if(data)	Destroy();

	type = XVARIANT_TYPE_FLOAT;
	
	size = sizeof(float);

	data = (void*)new float;
	if(data!=NULL) 
		{
			*((float*)data)=value;
		}

	return (*this);
}



/*-------------------------------------------------------------------
//	 XVARIANT& XVARIANT::operator = 
*/
/**
//	
//
//	@author		Abraham J. Velez 
//	@version	12/03/2018 10:57:07
//
//	@return		const : 
//
//	@param		XCHAR : 
//
*//*-----------------------------------------------------------------*/
const XVARIANT& XVARIANT::operator = (XCHAR value)
{
	if(data)	Destroy();

	type = XVARIANT_TYPE_XCHAR;
	size = sizeof(XCHAR);

	data= (void*)new XCHAR;
	if(data)  
		{
			*((XCHAR*)data)	= value;
		}

	return (*this);
}




/*-------------------------------------------------------------------
//	 XVARIANT& XVARIANT::operator = 
*/
/**
//	
//
//	@author		Abraham J. Velez 
//	@version	12/03/2018 10:45:45
//
//	@return		const : 
//
//	@param		XCHAR* : 
//
*//*-----------------------------------------------------------------*/
const XVARIANT& XVARIANT::operator = (XCHAR* string)
{
	GetDataFromString(string);

	return (*this);
}



/*-------------------------------------------------------------------
//	 XVARIANT& XVARIANT::operator = 
*/
/**
//	
//
//	@author		Abraham J. Velez 
//	@version	12/03/2018 10:50:16
//
//	@return		const : 
//
//	@param		XSTRING& : 
//
*//*-----------------------------------------------------------------*/
const XVARIANT&	XVARIANT::operator = (XSTRING& string)
{
	GetDataFromString(string.Get());

	return (*this);
}




/*-------------------------------------------------------------------
//	 XVARIANT& XVARIANT::operator = 
*/
/**
//	
//
//	@author		Abraham J. Velez 
//	@version	12/03/2018 10:51:51
//
//	@return		const : 
//
//	@param		char* : 
//
*//*-----------------------------------------------------------------*/
const XVARIANT& XVARIANT::operator = (char* string)
{
	if(data) Destroy();	

	data = new XSTRING();
	if(data)
		{
			type = XVARIANT_TYPE_STRING;
			size = sizeof(XSTRING);

			if (static_cast<XSTRING*>(data)) static_cast<XSTRING*>(data)->Set(string);
		}

	return (*this);
}



/*-------------------------------------------------------------------
//	 XVARIANT& XVARIANT::operator = 
*/
/**
//	
//
//	@author		Abraham J. Velez 
//	@version	12/03/2018 10:44:42
//
//	@return		const : 
//
//	@param		char : 
//
*//*-----------------------------------------------------------------*/
const XVARIANT& XVARIANT::operator = (char character)
{
	if(data) Destroy();

	type = XVARIANT_TYPE_CHAR;
	size = sizeof(char);

	data=(void*)new char;
	if(data) 
		{
			*((char*)data) = character;
		}

	return (*this);
}




/*-------------------------------------------------------------------
//	 XVARIANT& XVARIANT::operator = 
*/
/**
//	
//
//	@author		Abraham J. Velez 
//	@version	12/03/2018 10:52:53
//
//	@return		const : 
//
//	@param		const : 
//
*//*-----------------------------------------------------------------*/
const XVARIANT& XVARIANT::operator = (const XVARIANT& origin)
{

	if (&origin==this)
			return (*this);

	if (origin.data==this->data)
			return (*this);

	if(data) Destroy();
		
	size		= origin.size;
	type		= origin.type;
	switch(type)
		{
			case XVARIANT_TYPE_INTEGER				: (*this) = (int)(XVARIANT&)origin;			break;
			case XVARIANT_TYPE_CHAR						: (*this) = (char)(XVARIANT&)origin;		break;
			case XVARIANT_TYPE_FLOAT					:	(*this) = (float)(XVARIANT&)origin;		break;
			case XVARIANT_TYPE_XCHAR					:	(*this) = (XCHAR)(XVARIANT&)origin;		break;
			case XVARIANT_TYPE_STRING					:	(*this) = (XCHAR*)(XVARIANT&)origin;	break;	
			case XVARIANT_TYPE_SERIALIZABLE		:	{ 
																						data = new XBYTE[size];
																						memcpy(data, origin.data, size);																					
																					}	
																					break;
			case XVARIANT_TYPE_TIME						:
			case XVARIANT_TYPE_DATETIME				:
			case XVARIANT_TYPE_DATE						:	{ 
																						(*this) = (XDATETIME)(XVARIANT&)origin;	
																						type		= origin.type;
																					}	
																					break;
		}	
		
	


	return (*this);
}




/*-------------------------------------------------------------------
//	 XVARIANT& XVARIANT::operator=
*/
/**
//	
//
//	@author		Abraham J. Velez 
//	@version	12/03/2018 10:53:25
//
//	@return		const : 
//
//	@param		const : 
//
*//*-----------------------------------------------------------------*/
const XVARIANT& XVARIANT::operator=(const XSERIALIZABLE& serializable)
{
	XBUFFER xbuffer;

	serializable.DeSerialize(&xbuffer);

	if(data) Destroy();
	
	size = xbuffer.GetSize();	
	if(size)
		{
			data = new XBYTE[size];
			if(data)
				{			
					type = XVARIANT_TYPE_SERIALIZABLE;
					xbuffer.Get((XBYTE*)data, size, 0);
				}
		}

	return (*this);
}





/*-------------------------------------------------------------------
//	 XVARIANT& XVARIANT::operator = 
*/
/**
//	
//
//	@author		Abraham J. Velez 
//	@version	12/03/2018 10:54:12
//
//	@return		const : 
//
//	@param		XDATETIME : 
//
*//*-----------------------------------------------------------------*/
const XVARIANT& XVARIANT::operator = (XDATETIME datetime)
{
	if(data) Destroy();
	
	data = (void*)new XDATETIME;
	if(data)
		{
			type = (XVARIANT_TYPE)XVARIANT_TYPE_DATETIME;
			size = sizeof(XDATETIME);
			*((XDATETIME*)data) = datetime;			
		}

	return *this;
}




/*-------------------------------------------------------------------
//	int
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			12/08/2015 13:51:32
//	
//	@return 			XVARIANT::operator : 
//
*/
/*-----------------------------------------------------------------*/
XVARIANT::operator int()
{
	if(IsNull())  return 0;

	return *(int*)data;
}




/*-------------------------------------------------------------------
//	XDWORD
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			12/08/2015 13:52:00
//	
//	@return 			XVARIANT::operator : 
//
*/
/*-----------------------------------------------------------------*/
XVARIANT::operator XDWORD()
{
	if(IsNull()) return 0;

	return *(XDWORD*)data;
}



/*-------------------------------------------------------------------
//	XQWORD
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			12/08/2015 13:52:00
//	
//	@return 			XVARIANT::operator : 
//
*/
/*-----------------------------------------------------------------*/
XVARIANT::operator XQWORD()
{
	if(IsNull()) return 0;

	return *(XQWORD*)data;
}




/*-------------------------------------------------------------------
//	float
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			12/08/2015 13:52:00
//	
//	@return 			XVARIANT::operator : 
//
*/
/*-----------------------------------------------------------------*/
XVARIANT::operator float()
{
	if(IsNull()) return float(0);

	return *(float*)data;
}




/*-------------------------------------------------------------------
//	 XCHAR
*/
/**
//	
//
//	@author		Abraham J. Velez 
//	@version	12/03/2018 12:05:20
//
//	@return		XVARIANT::operator : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
XVARIANT::operator XCHAR()
{
	if(IsNull()) return XCHAR(0);

	return *(XCHAR*)data;
}




/*-------------------------------------------------------------------
//	XCHAR*
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			12/08/2015 13:52:33
//	
//	@return 			XVARIANT::operator : 
//
*/
/*-----------------------------------------------------------------*/
XVARIANT::operator XCHAR* ()
{
	if (!IsNull()) 
	switch(type)
		{
			case XVARIANT_TYPE_STRING :	if (static_cast<XSTRING*>(data)) return static_cast<XSTRING*>(data)->Get();			
		}

	return NULL;
}




/*-------------------------------------------------------------------
//	char
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			12/08/2015 13:52:18
//	
//	@return 			XVARIANT::operator : 
//
*/
/*-----------------------------------------------------------------*/
XVARIANT::operator char()
{
	if(data) return *(char*)data;
		
	return 0;
}




/*-------------------------------------------------------------------
//	XCHAR*
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			22/01/2016 13:34:57
//	
//	@return 			XVARIANT::operator : 
//
*/
/*-----------------------------------------------------------------*/
XVARIANT::operator const XCHAR*()
{
	if (!IsNull())
	switch (type)
		{
			case XVARIANT_TYPE_STRING :	if (static_cast<XSTRING*>(data)) return (const XCHAR*)static_cast<XSTRING*>(data)->Get();			
		}

	return (const XCHAR*)NULL;
}




/*-------------------------------------------------------------------
//	 XDATETIME
*/
/**
//	
//
//	@author		Abraham J. Velez 
//	@version	12/03/2018 10:54:59
//
//	@return		XVARIANT::operator : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
XVARIANT::operator XDATETIME()
{
	XDATETIME time;

	if(static_cast<XDATETIME*>(data))
	time=*(static_cast<XDATETIME*>(this->data));
	
	return time;
}



/*-------------------------------------------------------------------
//	XVARIANT::GetSerializable
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			04/02/2016 11:45:52
//	
//  @param				serializable : 
*/
/*-----------------------------------------------------------------*/
void XVARIANT::GetSerializable(XSERIALIZABLE& serializable)
{
	XBUFFER xbuffer;

	xbuffer.Resize(size);
	xbuffer.Set((XBYTE*)data, size, 0);

	serializable.Serialize(&xbuffer);
}




/*-------------------------------------------------------------------
//	XVARIANT::GetSerializable
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			04/02/2016 11:45:52
//	
//  @param				serializable : 
*/
/*-----------------------------------------------------------------*/
void XVARIANT::GetSerializable(XSERIALIZABLE* serializable)
{
	XBUFFER xbuffer;

	xbuffer.Resize(size);
	xbuffer.Set((XBYTE*)data, size, 0);

	serializable->Serialize(&xbuffer);
}




/*-------------------------------------------------------------------
//	XVARIANT::GetDataFromString
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/08/2015 11:38:31
//	
//	@return 			bool : 
//
//  @param				string : 
*/
/*-----------------------------------------------------------------*/
bool XVARIANT::GetDataFromString(XCHAR* string)
{			
	if(data) Destroy();

	data=new XSTRING();
	if(!data) return false;

	type = XVARIANT_TYPE_STRING;
	size = sizeof(XSTRING);
	

	if(static_cast<XSTRING*>(data)) 
		{
			return static_cast<XSTRING*>(data)->Set(string);
		}

	return false;
}




/*-------------------------------------------------------------------
//	XVARIANT::Set
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			09/02/2016 18:04:47
//	
//	@return 			bool : 
//
//  @param				type : 
//  @param				data : 
//  @param				size : 
*/
/*-----------------------------------------------------------------*/
bool XVARIANT::Set (XVARIANT_TYPE type, void* data, XDWORD size)
{
	if(this->data) 	Destroy();	

	if(type == XVARIANT_TYPE_NULL) 
		return true;
	
	if(size)
		{
			this->data = new XBYTE[size];
			if(!this->data) 
				return false;	
			this->type		= type;
		}
																			
	this->size		= size;

  if(data && size)	
		memcpy(this->data, data, size);

	return true;
}




/*-------------------------------------------------------------------
//	XVARIANT::ToString
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			16/09/2015 11:58:50
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool XVARIANT::ToString(XSTRING& to)
{
	switch(type)
		{
			case XVARIANT_TYPE_SERIALIZABLE	:	to.Format(__L("[Object]"));																																														break;
			case XVARIANT_TYPE_NULL					:	to.Format(__L("NULL")); 																																															break;

			case XVARIANT_TYPE_INTEGER			:	to.Format(__L("%d"),*(int*)this->data);																																								break;
			case XVARIANT_TYPE_CHAR					:	to.Format(__L("%c"),*(int*)this->data);																																								break;
			case XVARIANT_TYPE_FLOAT				:	to.Format(__L("%f"),*(float*)this->data); 																																						break;			
			case XVARIANT_TYPE_STRING				:	to.Format(__L("%s"),((XSTRING*)this->data)->Get());																																		break;	

			case XVARIANT_TYPE_TIME					: ((XDATETIME*)this->data)->GetDateTimeToString(XDATETIME_FORMAT_ADDTIME | XDATETIME_FORMAT_TIMEWITHSECONDS,	to);			break;
			case XVARIANT_TYPE_DATE					: ((XDATETIME*)this->data)->GetDateTimeToString(XDATETIME_FORMAT_YMD,																					to);			break;
			case XVARIANT_TYPE_DATETIME			:	((XDATETIME*)this->data)->GetDateTimeToString(XDATETIME_FORMAT_STANDARD,																		to);			break;

												default				:	return false;
		}

	return true;
}




/*-------------------------------------------------------------------
//	XVARIANT::Destroy
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			21/08/2015 12:28:32
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool XVARIANT::Destroy()
{	
	if(data) 		
		{
			switch(this->type)
				{
					case XVARIANT_TYPE_INTEGER				: delete (int*)data;						break;
					case XVARIANT_TYPE_CHAR						:	delete (char*)(data);					break;
					case XVARIANT_TYPE_FLOAT					:	delete (float*)(data);				break;
					case XVARIANT_TYPE_STRING					:	delete (XSTRING*)(data);			break;
					case XVARIANT_TYPE_SERIALIZABLE		: delete [] (XBYTE*)data;				break;
					
					case XVARIANT_TYPE_DATE						:
					case XVARIANT_TYPE_TIME						:
					case XVARIANT_TYPE_DATETIME				: delete (XDATETIME*)(data);		break;

					case XVARIANT_TYPE_NULL						:																break;
																	default		:	// delete (void*)(data);	break;
																							break;
				}	
		}	
	
	type    = XVARIANT_TYPE_NULL;
	size		=	0;
	data		= NULL;

	return true;
}


