//------------------------------------------------------------------------------------------
//	XFILEDBF.CPP
//	
//	DBF file functions (DBF file Format)
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 12/04/2002
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include <string.h>

#include "XFactory.h"
#include "XDateTime.h"

#include "XFileDBF.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------

		
//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  XFILEDBF_FIELD::XFILEDBF_FIELD
/**
//
//	
//	@author				Abraham J. Velez
//	@version			15/04/2002 9:23:55
//	
//	@return 			
//	*/
//-------------------------------------------------------------------
XFILEDBF_FIELD::XFILEDBF_FIELD()
{
	Clean();
}
												 

//-------------------------------------------------------------------
//  XFILEDBF_FIELD::~XFILEDBF_FIELD
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/04/2002 9:24:15
//	
//	@return
//	*/
//-------------------------------------------------------------------
XFILEDBF_FIELD::~XFILEDBF_FIELD()
{
	Clean();
}


//-------------------------------------------------------------------
//  XFILEDBF_FIELD::Clean
/**
//
//	
//	@author				Abraham J. Velez
//	@version			15/04/2002 9:25:15
//	
//	@return 			void : 
//	*/
//-------------------------------------------------------------------
void XFILEDBF_FIELD::Clean()
{
	name.Empty();
	
	type = XFILEDBF_FIELDTYPE_UNKNOWN;
	size = 0;
}



//-------------------------------------------------------------------
//  XFILEDBF_RECORD::XFILEDBF_RECORD
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/04/2002 10:56:23
//	
//	@return 			
//	*/
//-------------------------------------------------------------------
XFILEDBF_RECORD::XFILEDBF_RECORD(XVECTOR<XFILEDBF_FIELD*>* fields)
{
	Clean();

	size=0;

	this->fields = fields;
	
	for(XDWORD c=0;c<fields->GetSize();c++)
		{
			XFILEDBF_FIELD* field = fields->Get(c);
			if(field) size+=field->size;
		}

	size++;

	if(size) 
		{ 
			datarec = new XBYTE [size];
			if(datarec) memset(datarec,' ',size);									
		}
}



//-------------------------------------------------------------------
//  XFILEDBF_RECORD::~XFILEDBF_RECORD
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/04/2002 10:56:53
//	
//	@return 			
//	*/
//-------------------------------------------------------------------
XFILEDBF_RECORD::~XFILEDBF_RECORD()
{
	delete[] datarec;
	Clean();
}



//-------------------------------------------------------------------
//  XFILEDBF_RECORD::IsDelete
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/04/2002 10:58:15
//	
//	@return 			bool : 
//	*/
//-------------------------------------------------------------------
bool XFILEDBF_RECORD::IsDelete()
{
	if(!datarec) return true;

	if(datarec[0]==' ') return false;

  return true;
}



/*-------------------------------------------------------------------
//  XFILEDBF_RECORD::IsFieldEmpty
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			09/12/2013 17:49:28
//	
//	@return 			bool : 
//	@param				indexfield : 
*/
/*-----------------------------------------------------------------*/
bool XFILEDBF_RECORD::IsFieldEmpty(int indexfield)
{
	if(!fields) return true;
  int nfields=(fields->GetSize()-1);

	if(indexfield>nfields) return true;

	XFILEDBF_FIELD* field = (XFILEDBF_FIELD*)fields->Get(indexfield);
	if(!field) return true;

	XBYTE* data = GetData(indexfield);
	if(!data) return true;

	for(int c=0; c<field->size; c++)
		{
			if(data[c]!=0x20) return false;
		}

	return true;
}



//-------------------------------------------------------------------
//  XFILEDBF_RECORD::GetData
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/04/2002 11:01:00
//	
//	@return 			XBYTE* : 
//	@param				nfield : 
*/
//-------------------------------------------------------------------
XBYTE* XFILEDBF_RECORD::GetData(int indexfield)
{
	if(!fields)  return NULL;
  int nfields=(fields->GetSize()-1);

	if(indexfield>nfields) return NULL;

	XFILEDBF_FIELD* field = (XFILEDBF_FIELD*)fields->Get(indexfield);
	if(!field) return NULL;

	if(!field->size) return NULL;

	int offset = GetOffset(indexfield);
	if(offset==-1) return NULL;

	return (XBYTE*)(&datarec[offset]);
}


//-------------------------------------------------------------------
//  XFILEDBF_RECORD::SetData
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/04/2002 15:21:46
//	
//	@return 			bool : 
//	@param				indexfield :
//  @param				data : 
//  @param				limitsize :
*/
//-------------------------------------------------------------------
bool XFILEDBF_RECORD::SetData(int indexfield, XBYTE* data, int limitsize)
{
	if(!data) return false;

	int offset = GetOffset(indexfield);
	if(offset==-1) return false;

	XFILEDBF_FIELD* field = (XFILEDBF_FIELD*)fields->Get(indexfield);
	if(!field)		 return false;
	int size = field->size;

  memset((XBYTE*)(&datarec[offset]), ' '  , field->size);
	memcpy((XBYTE*)(&datarec[offset]), data , (limitsize==-1)?size:limitsize);

	return true;
}



/*-------------------------------------------------------------------
//  XFILEDBF_RECORD::GetDataString
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			11/12/2013 12:36:39
//	
//	@return 			bool : 
//	@param				indexfield : 
//  @param				string : 
*/
/*-----------------------------------------------------------------*/
bool XFILEDBF_RECORD::GetDataString(int indexfield, XSTRING& string)
{	
	string.Empty();

	if(!fields)  return false;
  int nfields=(fields->GetSize()-1);

	if(indexfield>nfields) return false;

	XFILEDBF_FIELD* field = (XFILEDBF_FIELD*)fields->Get(indexfield);
	if(!field) return false;

	if(!field->size)																return false;
	if(field->type != XFILEDBF_FIELDTYPE_CHARACTER) return false;

	int offset = GetOffset(indexfield);
	if(offset==-1) return false;

	char  value[_MAXSTR];
	int   size = field->size;

	memset(value, 0, _MAXSTR);
	if(size>_MAXSTR) size = _MAXSTR;

	memcpy((XBYTE*)(value), (XBYTE*)(&datarec[offset]), size);

	string = value;

	return true;
}



/*-------------------------------------------------------------------
//  XFILEDBF_RECORD::SetDataString
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			11/12/2013 13:06:49
//	
//	@return 			bool : 
//	@param				indexfield : 
//  @param				string : 
*/
/*-----------------------------------------------------------------*/
bool XFILEDBF_RECORD::SetDataString(int indexfield, XSTRING& string)
{	
	int offset = GetOffset(indexfield);
	if(offset==-1) return false;

	XFILEDBF_FIELD* field = (XFILEDBF_FIELD*)fields->Get(indexfield);
	if(!field)		 return false;

	if(!field->size)																return false;
	if(field->type != XFILEDBF_FIELDTYPE_CHARACTER) return false;
	
	int sizestring = string.GetSize();
	if(sizestring > field->size) sizestring = field->size;
		
	XSTRING_CREATEOEM(string, charstr);

	memset((XBYTE*)(&datarec[offset]), ' '		 , field->size);
//memcpy((XBYTE*)(&datarec[offset]), charstr , size);

	for(int c=0; c<sizestring; c++)
		{
			datarec[offset+c] = charstr[c];
		}

	XSTRING_DELETEOEM(charstr)

	return true;
}



//-------------------------------------------------------------------
//  XFILEDBF_RECORD::SetCharacterType
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			01/10/2002 15:30:09
//	
//	@return 			bool : 
//	@param				field :
//  @param				data : 
*/
//-------------------------------------------------------------------
bool XFILEDBF_RECORD::SetCharacterType(int indexfield,XCHAR* data)
{
	int offset = GetOffset(indexfield);
	if(offset==-1) return false;

	XFILEDBF_FIELD* field = (XFILEDBF_FIELD*)fields->Get(indexfield);
	if(!field)		 return false;

	XSTRING _data(data);
	int				 size = field->size;
	
	XSTRING_CREATEOEM(_data, charstr);

	memset((XBYTE*)(&datarec[offset]), ' ', size);
	memcpy((char*)(&datarec[offset]), charstr, _data.GetSize());

	XSTRING_DELETEOEM(charstr)

	return true;
}



//-------------------------------------------------------------------
//  XFILEDBF_RECORD::SetData
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/04/2002 15:28:26
//
//	@return 			bool : 
//	@param				data : 
//  @param				size : 
*/
//-------------------------------------------------------------------
bool XFILEDBF_RECORD::SetData(XBYTE* data,int size)
{
	memcpy(datarec,data,size);

	return true;	
}


//-------------------------------------------------------------------
//  XFILEDBF_RECORD::GetSize
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/04/2002 11:02:26
//	
//	@return 			int : 
//	*/
//-------------------------------------------------------------------
int XFILEDBF_RECORD::GetSize()
{
	return size;
}


//-------------------------------------------------------------------
//  XFILEDBF_RECORD::GetSizeField
/**
//
//	
//	@author				Abraham J. Velez
//	@version			01/10/2002 16:09:47
//	
//	@return 			int : 
//	@param				indexfield : 
*/
//-------------------------------------------------------------------
int XFILEDBF_RECORD::GetSizeField(int indexfield)
{
	if(!fields) return 0;
	int nfields=(fields->GetSize()-1);	
	if(indexfield>nfields) return 0;

	XFILEDBF_FIELD* field = (XFILEDBF_FIELD*)fields->Get(indexfield);
	if(!field)							return 0;

	return field->size;
}


//-------------------------------------------------------------------
//  XFILEDBF_RECORD::Clean
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/04/2002 11:06:31
//	
//	@return 			void : 
//	*/
//-------------------------------------------------------------------
void XFILEDBF_RECORD::Clean()
{
	size 		= 0;
	datarec = NULL;
}


//-------------------------------------------------------------------
//  XFILEDBF_RECORD::GetOffset
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			27/08/2006 18:11:44
//	
//	@return				int : 
//	@param				indexfield : 
*/
//-------------------------------------------------------------------
int XFILEDBF_RECORD::GetOffset(int indexfield)
{
	if(!fields)							return -1;
  
	int nfields=(fields->GetSize()-1);
	if(indexfield>nfields)	return -1;

	XFILEDBF_FIELD* field = (XFILEDBF_FIELD*)fields->Get(indexfield);
	if(!field)							return -1;
	if(!field->size)				return -1;

	int offset=1;

	for(int c=0;c<indexfield;c++)
		{
			field = (XFILEDBF_FIELD*)fields->Get(c);
		  if(field)  offset+=field->size;
		}

	return offset;
}



//-------------------------------------------------------------------
//  XFILEDBF_HEADER::XFILEDBF_HEADER
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			12/04/2002 14:51:59
//	
//	@return 			void : 
//	*/
//-------------------------------------------------------------------
XFILEDBF_HEADER::XFILEDBF_HEADER()
{
	Clean();	
}



//-------------------------------------------------------------------
//  XFILEDBF_HEADER::XFILEDBF_HEADER
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			03/02/2004 11:29:27
//	
//	@return				

//  @param				fileb : 
*/
//-------------------------------------------------------------------
XFILEDBF_HEADER::XFILEDBF_HEADER(XFILE* fileb)
{
	Clean();
	
	Load(fileb);
}




//-------------------------------------------------------------------
//  XFILEDBF_HEADER::~XFILEDBF_HEADER
/**
//
//
//	@author				Abraham J. Velez
//	@version			12/04/2002 14:53:21
//
//	@return 			void :
//	*/
//-------------------------------------------------------------------
XFILEDBF_HEADER::~XFILEDBF_HEADER()
{
  if(headerdata)
    {
    	delete[] headerdata;
      headerdata = NULL;
    }

  DeleteAllFields();

  Clean();
}




//-------------------------------------------------------------------
//  XFILEDBF_HEADER::Load
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			12/04/2002 14:53:07
//	
//	@return 			bool :
//	@param				fileb : 
*/
//-------------------------------------------------------------------
bool XFILEDBF_HEADER::Load(XFILE* xfile)
{
	// ---------------------------	
	// Read header
	if(!xfile->SetPosition(8)) return false;

	if(!xfile->Read((XBYTE*)&headersize,2)) return false;
	if(!headersize) return 0;

  if(headerdata)
    {
    	delete[] headerdata;
      headerdata = NULL;
    }

	headerdata = new XBYTE[headersize];
	if(!headerdata) return false;

	if(!xfile->SetPosition(0)) return false;
	if(!xfile->Read(headerdata,headersize)) return false;

	int index=XFILEDBF_FIELDSTART;

	while(headerdata[index]!=0x0D)
		{
			XFILEDBF_FIELDTYPE type;

			switch(headerdata[index+11])
				{
					case 'C' : type=XFILEDBF_FIELDTYPE_CHARACTER;	break;
					case 'D' : type=XFILEDBF_FIELDTYPE_DATE;			break;
					case 'F' : type=XFILEDBF_FIELDTYPE_FLOATING;	break;
					case 'N' : type=XFILEDBF_FIELDTYPE_BINARYHEX;	break;
					case 'L' : type=XFILEDBF_FIELDTYPE_LOGICAL;		break;
					case 'M' : type=XFILEDBF_FIELDTYPE_MEMO;			break;
					case 'V' : type=XFILEDBF_FIELDTYPE_VARIABLE;	break;
					case 'P' : type=XFILEDBF_FIELDTYPE_PICTURE;		break;
					case 'B' : type=XFILEDBF_FIELDTYPE_BINARY;		break;
					case 'G' : type=XFILEDBF_FIELDTYPE_GENERAL;		break;
					case '2' : type=XFILEDBF_FIELDTYPE_SHORTINT;	break;
					case '4' : type=XFILEDBF_FIELDTYPE_LONGINT;		break;
					case '8' : type=XFILEDBF_FIELDTYPE_DOUBLE;		break;
					default  : type=XFILEDBF_FIELDTYPE_UNKNOWN;		break;
				}

			if(type!=XFILEDBF_FIELDTYPE_UNKNOWN)
				{
					XSTRING name((char*)&headerdata[index]);

				  if(!CreateField(type, name.Get(), headerdata[index+16])) return false;
				}

			index+=XFILEDBF_FIELDSIZE;
		}

	return true;
}



//-------------------------------------------------------------------
//  XFILEDBF_HEADER::Create
/**
//
//
//	@author				Abraham J. Velez
//	@version			15/04/2002 13:37:09
//	
//	@return 			bool : 
//	@param				fileb : 
*/
//-------------------------------------------------------------------
bool XFILEDBF_HEADER::Create(XFILE* fileb)
{
	headersize = (XWORD)(XFILEDBF_FIELDSTART + fields.GetSize()*XFILEDBF_FIELDSIZE+ 1);
	
	headerdata = new XBYTE [headersize];
	if(!headerdata) return false;

	memset(headerdata,0,headersize);

	headerdata[0] = 0x03;

	XDATETIME* xdatetime = xfactory->CreateDateTime();
	if(xdatetime)
		{
			xdatetime->Read();

			headerdata[1] = xdatetime->GetYear()-1900;
			headerdata[2] = xdatetime->GetMonth();
			headerdata[3] = xdatetime->GetDay();

			delete xdatetime;
		}
	
	XDWORD* nf = (XDWORD*)&headerdata[4];
	(*nf)=0;

	XWORD* hs  = (XWORD*)(&headerdata[8]);
	(*hs)=headersize;

	XWORD* rs  = (XWORD*)(&headerdata[10]);
	(*rs)=GetSizeRecord();


	int index=XFILEDBF_FIELDSTART;

	for(XDWORD c=0;c<fields.GetSize();c++)
		{
			XFILEDBF_FIELD* field  = (XFILEDBF_FIELD*)fields.Get(c);
		  char						type	 = ' ';

			if(field)
				{
					memset(&headerdata[index],0,XFILEDBF_FIELDSIZE);
					
					XSTRING_CREATEOEM(field->name, charstr)

					int size = (int)strlen(charstr);				
					if(size>XFILEDBF_FIELDSIZE) size = XFILEDBF_FIELDSIZE;
			
					memcpy((char*)(&headerdata[index]), charstr, size);
					
					XSTRING_DELETEOEM(charstr)

					switch(field->type)
						{
							case XFILEDBF_FIELDTYPE_CHARACTER : type='C'; break;
							case XFILEDBF_FIELDTYPE_DATE			: type='D'; break;
							case XFILEDBF_FIELDTYPE_FLOATING	: type='F';	break;
							case XFILEDBF_FIELDTYPE_BINARYHEX : type='N';	break;
							case XFILEDBF_FIELDTYPE_LOGICAL		:	type='L'; break;
							case XFILEDBF_FIELDTYPE_MEMO			:	type='M';	break;
							case XFILEDBF_FIELDTYPE_VARIABLE	: type='V';	break;
							case XFILEDBF_FIELDTYPE_PICTURE		: type='P';	break;
							case XFILEDBF_FIELDTYPE_BINARY		: type='B';	break;
							case XFILEDBF_FIELDTYPE_GENERAL		: type='G';	break;
							case XFILEDBF_FIELDTYPE_SHORTINT	: type='2';	break;
							case XFILEDBF_FIELDTYPE_LONGINT		: type='4';	break;
							case XFILEDBF_FIELDTYPE_DOUBLE		: type='8';	break;
						}

					headerdata[index+11]=type;
					headerdata[index+16]=field->size;

					index+=XFILEDBF_FIELDSIZE;
				}
		}

	headerdata[headersize-1] = 0x0D;

	if(!fileb->SetPosition(0)) return false;
	if(!fileb->Write(headerdata,headersize)) return false;

	return true;
}



/*-------------------------------------------------------------------
//  XFILEDBF_HEADER::Update
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			28/06/2008 12:08:22
//	
//	@return				bool : 
//	@param				fileb : 
*/
/*-----------------------------------------------------------------*/
bool XFILEDBF_HEADER::Update(XFILE* fileb)
{
	XDWORD nrecords = GetNRecords();

	if(!fileb->SetPosition(4)) return false;
	if(!fileb->Write((XBYTE*)&nrecords,sizeof(XDWORD)))								return false;

	if(!fileb->SetPosition(12)) return false;
	if(!fileb->Write((XBYTE*)&headerdata[12],XFILEDBF_HEADRESERVED)) return false;

	return true;
}



//-------------------------------------------------------------------
//  XFILEDBF_HEADER::GetType
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			12/04/2002 15:30:13
//	
//	@return 			XFILEDBF_TYPE :
//	*/
//-------------------------------------------------------------------
XFILEDBF_TYPE XFILEDBF_HEADER::GetType()
{
	if(!headerdata) return XFILEDBF_TYPE_UNKNOWN;

	switch(headerdata[0])
		{
			case 0x03:
			case 0x83: return XFILEDBF_TYPE_DBASEIII;
/*
			case 0x00: return XFILEDBF_TYPE_DBASEIV;
			case 0x00: return XFILEDBF_TYPE_DBASEV;
			case 0x00: return XFILEDBF_TYPE_FOXPRO;
			case 0x00: return XFILEDBF_TYPE_CLIPPER;
*/
		}

	return XFILEDBF_TYPE_UNKNOWN;
}


//-------------------------------------------------------------------
//  XFILEDBF_HEADER::GetHeaderSize
/**
//
//	
//	@author				Abraham J. Velez
//	@version			12/04/2002 16:57:48
//	
//	@return 			XWORD : 
//	*/
//-------------------------------------------------------------------
XWORD XFILEDBF_HEADER::GetHeaderSize()
{
	if(!headerdata) return 0;

	return headersize;
}



/*-------------------------------------------------------------------
//  XFILEDBF_HEADER::GetHeaderData
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			12/07/2013 18:50:49
//	
//	@return 			XBYTE* : 
//	*/
/*-----------------------------------------------------------------*/
XBYTE* XFILEDBF_HEADER::GetHeaderData()
{
	return headerdata;
}



/*-------------------------------------------------------------------
//  XFILEDBF_HEADER::GetReservedData
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			28/06/2008 10:26:08
//	
//	@return				XDWORD : 
//	*/
/*-----------------------------------------------------------------*/
XDWORD XFILEDBF_HEADER::GetReservedData()
{
	if(!headerdata) return 0;

	XDWORD* reserved = (XDWORD*)(&headerdata[12]);

	return (*reserved);
}


/*-------------------------------------------------------------------
//  XFILEDBF_HEADER::SetReservedData
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			28/06/2008 10:27:11
//	
//	@return				bool : 
//	@param				data : 
*/
/*-----------------------------------------------------------------*/
bool XFILEDBF_HEADER::SetReservedData(XDWORD data)
{
	if(!headerdata) return false;

	XDWORD* reserved = (XDWORD*)(&headerdata[12]);

	(*reserved) = data;

	return true;

}



//-------------------------------------------------------------------
//  XFILEDBF_HEADER::GetNFields
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/04/2002 10:50:57
//	
//	@return 			int : 
//	*/
//-------------------------------------------------------------------
int XFILEDBF_HEADER::GetNFields()
{
	return fields.GetSize();
}


//-------------------------------------------------------------------
//  XFILEDBF_HEADER::GetNRecords
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			12/04/2002 16:50:12
//	
//	@return 			XDWORD : 
//	*/
//-------------------------------------------------------------------
XDWORD XFILEDBF_HEADER::GetNRecords()
{
	if(!headerdata) return 0;

	XDWORD *nrec = (XDWORD*)((&headerdata[4]));

	return (XDWORD)(*nrec);
}


//-------------------------------------------------------------------
//  XFILEDBF_HEADER::SetNRecords
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/04/2002 13:14:03
//	
//	@return 			bool :
//	@param				nrecords : 
*/
//-------------------------------------------------------------------
bool XFILEDBF_HEADER::SetNRecords(XDWORD nrecords)
{
	if(!headerdata) return false;

	XDWORD* nr=(XDWORD*)(&headerdata[4]);
	(*nr)=nrecords;

	return true;
}



//-------------------------------------------------------------------
//  XFILEDBF_HEADER::GetSizeRecord
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/04/2002 11:18:11
//
//	@return 			int : 
//	*/
//-------------------------------------------------------------------
int XFILEDBF_HEADER::GetSizeRecord()
{
	int size=0;

	if(fields.IsEmpty()) return size;

  int nfields=fields.GetSize();

	for(int c=0;c<nfields;c++)
		{
			XFILEDBF_FIELD* field = (XFILEDBF_FIELD*)fields.Get(c);

			if(field) size+=field->size;
		}
	
	size++;

	return size;
}



//-------------------------------------------------------------------
//  XFILEDBF_HEADER::GetFields
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/04/2002 9:18:16
//	
//	@return 			XVECTOR* : 
//	*/
//-------------------------------------------------------------------
XVECTOR<XFILEDBF_FIELD*>* XFILEDBF_HEADER::GetFields()
{
	return &fields;
}
	


//-------------------------------------------------------------------
//  XFILEDBF_HEADER::GetField
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/04/2002 10:50:01
//	
//	@return 			XFILEDBF_FIELD* : 
//	@param				field : 
*/
//-------------------------------------------------------------------
XFILEDBF_FIELD* XFILEDBF_HEADER::GetField(int indexfield)
{
  int nfields=(fields.GetSize()-1);

	if(indexfield>nfields) return NULL;

	return (XFILEDBF_FIELD*)fields.Get(indexfield);
}



//-------------------------------------------------------------------
//  XFILEDBF_HEADER::CreateField
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/04/2002 9:19:16
//	
//	@return 			bool : 
//	@param				type : 
//  @param				name : 
*/
//-------------------------------------------------------------------
bool XFILEDBF_HEADER::CreateField(XFILEDBF_FIELDTYPE type,XCHAR* name,XBYTE size)
{
	XFILEDBF_FIELD* field;

	field = new XFILEDBF_FIELD();
	if(!field) return false;

	field->name = name;
	field->type = type;	
	field->size = size;

	if(!field->size)
		{
			switch(type)
				{
					case XFILEDBF_FIELDTYPE_UNKNOWN		: field->size=0;   break;
					case XFILEDBF_FIELDTYPE_CHARACTER	:	field->size=0;   break;
					case XFILEDBF_FIELDTYPE_DATE			: field->size=0;   break;
					case XFILEDBF_FIELDTYPE_FLOATING	: field->size=32;  break;
					case XFILEDBF_FIELDTYPE_BINARYHEX : field->size=32;  break;
					case XFILEDBF_FIELDTYPE_LOGICAL		: field->size=0;   break;
					case XFILEDBF_FIELDTYPE_MEMO			: field->size=0;   break;
					case XFILEDBF_FIELDTYPE_VARIABLE	: break;
					case XFILEDBF_FIELDTYPE_PICTURE		: break;
					case XFILEDBF_FIELDTYPE_BINARY		: break;
					case XFILEDBF_FIELDTYPE_GENERAL		: break;
					case XFILEDBF_FIELDTYPE_SHORTINT	: break;
					case XFILEDBF_FIELDTYPE_LONGINT		: break;
					case XFILEDBF_FIELDTYPE_DOUBLE		: break;
				}
		} 

	fields.Add(field);

	return true;
}



/*-------------------------------------------------------------------
//  XFILEDBF_HEADER::CopyFrom
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			12/07/2013 18:38:05
//	
//	@return 			bool : 
//	@param				header : 
*/
/*-----------------------------------------------------------------*/
bool XFILEDBF_HEADER::CopyFrom(XFILEDBF_HEADER* header)
{
	headersize = header->GetHeaderSize();

	if(headerdata) delete [] headerdata;

	headerdata = new XBYTE[headersize];
	if(!headerdata) return false;

	memcpy(headerdata, header->GetHeaderData(), headersize);

	for(int c=0; c<header->GetNFields(); c++)
		{
			XFILEDBF_FIELD* field = new XFILEDBF_FIELD();
			if(field)
				{
					field->name = header->GetField(c)->name;
					field->size = header->GetField(c)->size;
					field->type = header->GetField(c)->type;

					fields.Add(field);
				}			
		}

	return true;
}


	


//-------------------------------------------------------------------
//  XFILEDBF_HEADER::DeleteAllFields
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/04/2002 10:23:50
//	
//	@return 			bool : 
//	*/
//-------------------------------------------------------------------
bool XFILEDBF_HEADER::DeleteAllFields()
{
	if(fields.IsEmpty()) return false;

	fields.DeleteContents();	
	fields.DeleteAll();

	return true;
}



//-------------------------------------------------------------------
//  XFILEDBF_HEADER::Clean
/**
//
//
//	@author				Abraham J. Velez
//	@version			12/04/2002 14:56:56
//
//	@return 			bool :
//	*/
//-------------------------------------------------------------------
void XFILEDBF_HEADER::Clean()
{
	headerdata 	= NULL;
	headersize  = 0;
}



//-------------------------------------------------------------------
//  XFILEDBF::XFILEDBF
/**
//
//
//	@author				Abraham J. Velez
//	@version			15/07/2002 19:11:13
//
//	@return
//	@param				xfactory :
*/
//-------------------------------------------------------------------
XFILEDBF::XFILEDBF()
{
	Clean();

  if(!CreatePrimaryFile()) return;

	CreateHeader();
}


//-------------------------------------------------------------------
//  XFILEDBF::XFILEDBF
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/07/2002 19:15:55
//	
//	@return 			

//  @param				pathname : 
*/
//-------------------------------------------------------------------
XFILEDBF::XFILEDBF(XPATH& xpath)
{
	Clean();	

  if(!CreatePrimaryFile()) return;

	CreateHeader();

	this->xpath = xpath;

	Open(xpath);
}



//-------------------------------------------------------------------
//  XFILEDBF::XFILEDBF
/**
//
//
//	@author				Abraham J. Velez
//	@version			15/07/2002 19:15:02
//
//	@return
//	@param				factory :
//  @param				fileb :
*/
//-------------------------------------------------------------------
XFILEDBF::XFILEDBF(XFILE* fileb)
{
	Clean();
	
	this->xpath = fileb->GetPathNameFile();

	file = fileb;

	CreateHeader();
}



//-------------------------------------------------------------------
//  XFILEDBF::~XFILEDBF
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/04/2002 9:48:21
//
//	@return
//	*/
//-------------------------------------------------------------------
XFILEDBF::~XFILEDBF()
{
	Close();

  if(!DeletePrimaryFile()) return;

	delete header;
	
	Clean();

}


//-------------------------------------------------------------------
//  XFILEDBF::Open
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/04/2002 9:35:54
//	
//	@return 			bool :
//	@param				xpath :
//  @param				readonly :
*/
//-------------------------------------------------------------------
bool XFILEDBF::Open(XPATH& xpath,bool readonly)
{
	if(!header)					return false;
	if(!file)						return false;
	if(xpath.IsEmpty()) return false;

	this->xpath = xpath;

	if(!file->Open(xpath,readonly)) return false;

	return true;
}
		

//-------------------------------------------------------------------
//  XFILEDBF::Create
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/04/2002 9:38:42
//	
//	@return 			bool : 
//	@param				pathname : 
*/
//-------------------------------------------------------------------
bool XFILEDBF::Create(XPATH& xpath)
{
	if(!header)					return false;
	if(!file)						return false;
	if(xpath.IsEmpty()) return false;

	this->xpath = xpath;

	if(!file->Create(xpath))  return false;
	if(!header->Create(file)) return false;

	file->Flush();

	return true;
}


//-------------------------------------------------------------------
//  XFILEDBF::IsOpen
/**
//
//
//	@author				Abraham J. Velez
//	@version			10/01/2001 17:11:16
//
//	@return 			bool :
*/
//-------------------------------------------------------------------
bool XFILEDBF::IsOpen()
{
  if(!file) return false;

  return file->IsOpen();
}


//-------------------------------------------------------------------
//  XFILEDBF::Close
/**
//
//
//	@author				Abraham J. Velez
//	@version			15/04/2002 9:38:08
//	
//	@return 			bool : 
*/
//-------------------------------------------------------------------
bool XFILEDBF::Close()
{
	if(header)
		{
			delete header;		 
			CreateHeader();
		}

	if(!file->Close()) return false;

	return true;
}



/*-------------------------------------------------------------------
//  XFILEDBF::GetXPath
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			12/07/2013 17:39:53
//	
//	@return 			XPATH* : 
//	*/
/*-----------------------------------------------------------------*/
XPATH* XFILEDBF::GetXPath()
{
	return &xpath;
}



//-------------------------------------------------------------------
//  XFILEDBF::GetHeader
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/04/2002 11:08:47
//	
//	@return 			XFILEDBF_HEADER* : 
//	*/
//-------------------------------------------------------------------
XFILEDBF_HEADER* XFILEDBF::GetHeader()
{
	return header;
}



//-------------------------------------------------------------------
//  XFILEDBF::ReadRecord
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/04/2002 11:11:08
//	
//	@return 			XFILEDBF_RECORD* : 
//	@param				nrecord : 
*/
//-------------------------------------------------------------------
XFILEDBF_RECORD* XFILEDBF::ReadRecord(XDWORD nrecord)
{
	if(!IsOpen()) return NULL;
	if(!header)		return NULL;

	XFILEDBF_RECORD* record;
	if(nrecord>=header->GetNRecords()) return NULL;

	int sizerecord = header->GetSizeRecord();

	if(!sizerecord) return NULL;

	record = new XFILEDBF_RECORD(header->GetFields());
	if(!record) return NULL;

	XBYTE* data = new XBYTE [sizerecord];
	if(!data)
		{
			delete record;
			return NULL;
		}	

	int position=header->GetHeaderSize()+(sizerecord*nrecord);

	file->SetPosition(position);

	if(!file->Read(data,sizerecord)) 
		{
			delete record;
			delete[] data;

			return NULL;
		}
		
	record->SetData(data,sizerecord);

	delete[] data;

	return record;
}




//-------------------------------------------------------------------
//  XFILEDBF::WriteRecord
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/04/2002 12:47:49
//	
//	@return 			bool : 
//	@param				nrecord : 
//  @param				record : 
*/
//-------------------------------------------------------------------
bool XFILEDBF::WriteRecord(XDWORD nrecord,XFILEDBF_RECORD* record)
{
	if(!IsOpen()) return false;
	if(!header)		return false;
	
	int position = header->GetHeaderSize()+(header->GetSizeRecord()*nrecord);

	file->SetPosition(position);

	XBYTE delflag=record->IsDelete()?'*':' ';

	if(!file->Write(&delflag,1)) return false;
	if(!file->Write(record->GetData(0),header->GetSizeRecord()-1)) return false;
		
	return true;
}




/*-------------------------------------------------------------------
//	XFILEDBF::InsertRecord
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			06/10/2015 8:44:53
//	
//	@return 			bool : 
//
//  @param				nrecord : 
//  @param				record : 
*/
/*-----------------------------------------------------------------*/
bool XFILEDBF::InsertRecord(XDWORD nrecord, XFILEDBF_RECORD* record)
{
	if(!IsOpen()) return false;
	if(!header)		return false;
	
	int position = header->GetHeaderSize()+(header->GetSizeRecord()*nrecord);

	file->SetPosition(position);

	XBYTE delflag=record->IsDelete()?'*':' ';

	XBUFFER xbuffer;

	xbuffer.Add(delflag);
	xbuffer.Add(record->GetData(0), (header->GetSizeRecord()-1));

	if(!file->InsertBlock(xbuffer.Get(), xbuffer.GetSize()))	 return false;
	
	header->SetNRecords(header->GetNRecords()+1);	
	header->Update(file);
		
	return true;
}





//-------------------------------------------------------------------
//  XFILEDBF::AddRecord
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/04/2002 12:51:57
//	
//	@return 			bool : 
//	@param				record : 
*/
//-------------------------------------------------------------------
bool XFILEDBF::AddRecord(XFILEDBF_RECORD* record)
{
	if(!IsOpen()) return false;
	if(!header)		return false;

	int size = header->GetSizeRecord()-1;

	XBYTE specialend;
	int   position;

	file->SetPosition(XFILE_SEEKEND);
	file->GetPosition(position);
	file->SetPosition(position-1);

	if(file->Read(&specialend,1))
		{
			if(specialend == 0x1A) file->SetPosition(position-1);				
		}

	XBYTE delflag=record->IsDelete()?'*':' ';

	if(!file->Write(&delflag,1)) return false;
	if(!file->Write(record->GetData(0),size)) return false;

	specialend = 0x1A;
	if(!file->Write(&specialend, 1)) return false;

	header->SetNRecords(header->GetNRecords()+1);	

	header->Update(file);
	
	return true;
}



//-------------------------------------------------------------------
//  XFILEDBF::DeleteRecord
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/04/2002 12:53:06
//	
//	@return 			bool : 
//	@param				nrecord : 
*/
//-------------------------------------------------------------------
bool XFILEDBF::DeleteRecord(XDWORD nrecord)
{
	if(!IsOpen()) return false;
	if(!header)		return false;

	char*  data;
	int    size = header->GetSizeRecord()-1;

	data = new char[size+10];
	if(!data) return false;

	memset(data,0,size+10);

	int position = header->GetHeaderSize()+(header->GetSizeRecord()*nrecord);

	file->SetPosition(position);

	XBYTE delflag='*';

	if(!file->Write(&delflag,1)) return false;
	if(!file->Write((XBYTE*)data,size)) return false;

	delete[] data;
		
	return true;
}




//-------------------------------------------------------------------
//  XFILEDBF::Pack
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/04/2002 12:54:21
//	
//	@return 			bool : 
//	*/
//-------------------------------------------------------------------
bool XFILEDBF::Pack()
{
 	if(!IsOpen()) return false;
	if(!header)		return false;

	XPATH xpathtmp;

	xpathtmp  = GetXPath()->Get();
	xpathtmp += __L(".tmp");

	XFILEDBF* dbftmp = new XFILEDBF();
	if(!dbftmp) return false;	
	
	dbftmp->GetHeader()->CopyFrom(header);
	dbftmp->GetHeader()->SetNRecords(0);

	delete dbftmp->GetHeader()->GetHeaderData();

	if(!dbftmp->Create(xpathtmp))
		{
			delete dbftmp;
			return false;
		}

	for(XDWORD c=0; c< header->GetNRecords(); c++)
		{
			XFILEDBF_RECORD* record = ReadRecord(c);
			if(record)
				{					
					if(!record->IsDelete()) 
						{
							dbftmp->AddRecord(record);						
						}

					delete record;
				}
		}

	dbftmp->Close();

	delete dbftmp;

	Close();
	

	XFILE* xfile = xfactory->Create_File();
	if(!xfile) return false;
	
	bool status = true;

	if(xfile->Erase(GetXPath()->Get()))
		{		
			if(!xfile->Rename(xpathtmp.Get(), GetXPath()->Get())) status = false;

		} else status = false;

	xfactory->Delete_File(xfile);

	if(status) 
		{
			status = Open((*GetXPath()), GetPrimaryFile()->IsReadOnly());
			if(status) 
				{
					delete GetHeader();
					CreateHeader();
					GetHeader()->Load(GetPrimaryFile());						
				}
		}
		
	return status;
}



/*-------------------------------------------------------------------
//  XFILEDBF::Reduction
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			09/12/2013 12:39:22
//	
//	@return 			bool : 
//	@param				maxrecords : 
//  @param				reductionpercent : 
*/
/*-----------------------------------------------------------------*/
bool XFILEDBF::Reduction(int maxrecords, int reductionpercent)
{
	if(!IsOpen()) return false;
	if(!header)		return false;

	int nrecords  = header->GetNRecords();
	if(nrecords<maxrecords) return true;
	
	int nrectosave = (nrecords*reductionpercent)/100;

	for(int c=0;c<(nrecords-nrectosave);c++)
		{
			DeleteRecord(c);
		}

	if(!Pack()) return false;				
	
	return true;
}




//-------------------------------------------------------------------
//  XFILEDBF::CreateHeader
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/07/2002 19:17:28
//	
//	@return 			bool : 
//	*/
//-------------------------------------------------------------------
bool XFILEDBF::CreateHeader()
{
	header = new XFILEDBF_HEADER();
	if(!header)  return false;

	return true;
}

	

//-------------------------------------------------------------------
//  XFILEDBF::Clean
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/07/2002 19:17:45
//	
//	@return 			void : 
//	*/
//-------------------------------------------------------------------
void XFILEDBF::Clean()
{
	header		= NULL;
}
				
