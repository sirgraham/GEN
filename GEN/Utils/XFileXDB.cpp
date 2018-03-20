//------------------------------------------------------------------------------------------
//	XFILEXDB.CPP
//	
//	Dialog Sentence Class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 25/05/2004 18:55:54
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include <string.h>

#include "XBuffer.h"
#include "XFactory.h"
#include "XDebug.h"

#include "Hash.h"

#include "XFileXDB.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	
	
//---- CLASS MEMBERS -----------------------------------------------------------------------



/*-------------------------------------------------------------------
//  XFILEXDB::XFILEXDB
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			24/04/2013 7:06:05
//	
//	@return 			void : 
//	@param				xfactory :
//  @param				hash : 
*/
/*-----------------------------------------------------------------*/
XFILEXDB::XFILEXDB(HASH* hash) : XFILEHASH(hash)
{
	Clean();

	this->hash		 = hash;

	Set(XFILE_XDB_ID, XFILEXDB_TYPE, XFILEXDB_VERSION, XFILEXDB_IDSTRING);
}



/*-------------------------------------------------------------------
//  XFILEXDB::XFILEXDB
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			24/04/2013 7:08:56
//	
//	@return 		

//  @param				hash : 
//  @param				xpath : 
*/
/*-----------------------------------------------------------------*/
XFILEXDB::XFILEXDB(HASH* hash, XPATH& xpath) : XFILEHASH(hash)
{
	Clean();

	this->hash		 = hash;

	Set(XFILE_XDB_ID, XFILEXDB_TYPE, XFILEXDB_VERSION, XFILEXDB_IDSTRING);

	SetPath(xpath);
}


//-------------------------------------------------------------------
//  XFILEXDB::~XFILEXDB
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			25/05/2004 19:09:18
//	
//	@return				void : 
//	*/
//-------------------------------------------------------------------
XFILEXDB::~XFILEXDB()
{
	End();

	Clean();
}




//-------------------------------------------------------------------
//  XFILEXDB::SetPath
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			05/10/2004 10:43:53
//	
//	@return				bool : 
//	@param				xpath : 
*/
//-------------------------------------------------------------------
bool XFILEXDB::SetPath(XPATH& xpath)
{
	if(xpath.IsEmpty()) return false;

	this->xpath = xpath;

	return true;
}


//-------------------------------------------------------------------
//  XFILEXDB::Ini
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/06/2004 12:53:48
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool XFILEXDB::Ini()
{
	return OpenFile();	
}


//-------------------------------------------------------------------
//  XFILEXDB::End
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/06/2004 12:53:51
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool XFILEXDB::End()
{
	return CloseFile();	
}



/*-------------------------------------------------------------------
//  XFILEXDB::GetIndexMap
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			26/04/2013 17:40:44
//	
//	@return 			XMAP* : 
//	*/
/*-----------------------------------------------------------------*/
XMAP<XDWORD, XDWORD>* XFILEXDB::GetIndexMap()
{
	return &indexmap;
}




//-------------------------------------------------------------------
//  XFILEXDB::OpenFile
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			27/05/2004 16:50:22
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool XFILEXDB::OpenFile()
{
	if(!Open(this->xpath)) return false;
	
	GetPrimaryFile()->Read((XBYTE*)&nrecords,sizeof(XDWORD));

	if(!nrecords) 
		{
			Close();
			return false;
		}

	indexmap.SetIsMulti(true);
	for(int c=0;c<nrecords;c++)
		{
			XDWORD ID			=	0;
			XDWORD filepos	= 0;

			GetPrimaryFile()->Read((XBYTE*)&ID			, sizeof(XDWORD));
			GetPrimaryFile()->Read((XBYTE*)&filepos	, sizeof(XDWORD));

			//XDEBUG_PRINTCOLOR(1,__L("ID: %08X"), ID);
						
			indexmap.Add(ID,filepos);		
		}

	indexmap.SetIsMulti(false);
	
	isopen=true;

	return true;
}




//-------------------------------------------------------------------
//  XFILEXDB::GetNumberRecords
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			27/05/2004 17:50:11
//	
//	@return				int : 
//	*/
//-------------------------------------------------------------------
int XFILEXDB::GetNumberRecords()
{
	return nrecords;
}



//-------------------------------------------------------------------
//  XFILEXDB::GetRecord
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/06/2004 12:21:35
//	
//	@return				XBYTE* : 
//	@param				ID : 
//  @param				size : 
*/
//-------------------------------------------------------------------
XBUFFER* XFILEXDB::GetRecord(XDWORD ID)
{
	if(!isopen) 
		{
			if(!OpenFile()) return NULL;
		}

  if(!nrecords) return NULL;

	XDWORD filepos = GetPosition(ID);
	if(!filepos) return NULL;

	XDWORD size;

	if(!GetPrimaryFile()->SetPosition(filepos)) return NULL;

	if(!GetPrimaryFile()->Read((XBYTE*)&size ,sizeof(XDWORD))) return NULL;

	XBUFFER* xbuffer = new XBUFFER();
	if(xbuffer)
		{
			if(size) 
				{
					xbuffer->Resize(size);
					xbuffer->FillBuffer();
				}
	
			if(!GetPrimaryFile()->Read((XBYTE*)xbuffer->Get() , size)) 
				{
					delete xbuffer;
					return NULL;
				}

			return xbuffer;
		}
	
	return NULL;
}




/*-------------------------------------------------------------------
//  XFILEXDB::GetRecordIndex
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			26/04/2013 17:20:36
//	
//	@return 			XBUFFER* : 
//	@param				index : 
*/
/*-----------------------------------------------------------------*/
XBUFFER* XFILEXDB::GetRecordIndex(XDWORD index)
{
	if(!isopen) 
		{
			if(!OpenFile()) return NULL;
		}

  if(!nrecords) return NULL;

	XDWORD filepos = indexmap.GetElement(index);
	if(!filepos) return NULL;

	XDWORD size;

	if(!GetPrimaryFile()->SetPosition(filepos)) return NULL;

	if(!GetPrimaryFile()->Read((XBYTE*)&size ,sizeof(XDWORD))) return NULL;

	XBUFFER* xbuffer = new XBUFFER();
	if(xbuffer)
		{
			if(size) 
				{
					xbuffer->Resize(size);
					xbuffer->FillBuffer();
				}
	
			if(!GetPrimaryFile()->Read((XBYTE*)xbuffer->Get() , size)) 
				{
					delete xbuffer;
					return NULL;
				}

			return xbuffer;
		}
	
	return NULL;
}





//-------------------------------------------------------------------
//  XFILEXDB::GetPosition
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			28/06/2004 13:59:08
//	
//	@return				XDWORD : 
//	@param				index : 
*/
//-------------------------------------------------------------------
XDWORD XFILEXDB::GetPosition(XDWORD ID)
{
	if(!isopen) 
		{
			if(!OpenFile()) return 0;
		}	

  if(!nrecords) return 0;

	int indexfilepos = indexmap.Find(ID);
	if(indexfilepos==-1) return 0;

	XDWORD filepos = indexmap.GetElement(indexfilepos);
	
	return filepos;
}




//-------------------------------------------------------------------
//  XFILEXDB::CloseFile
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			27/05/2004 16:50:53
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool XFILEXDB::CloseFile()
{
	Close();
	
	nrecords = 0;

	indexmap.DeleteAll();

	isopen = false;

	return true;
}


//-------------------------------------------------------------------
//  XFILEXDB::Clean
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			26/05/2004 10:31:04
//	
//	@return				void : 
//	*/
//-------------------------------------------------------------------
void XFILEXDB::Clean()
{
	xpath.Empty();

	isopen				= false;	
	nrecords			= 0;

	indexmap.DeleteAll();
}

