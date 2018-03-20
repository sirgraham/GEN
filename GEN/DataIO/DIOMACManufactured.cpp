/*------------------------------------------------------------------------------------------
//	DIOMACMANUFACTURED.CPP
//	
//	Data IO MAC MANUFACTURED class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 30/08/2012 9:14:11
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.	
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include <stdio.h>
#include <string.h>

#include "HashMD5.h"

#include "XFileTXT.h"
#include "XFileXDB.h"
#include "XDebug.h"

#include "DIOWebScraperMACManufacturer.h"

#include "DIOMACManufactured.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//	DIOMACMANUFACTURED::DIOMACMANUFACTURED
*/	
/**	
//	
//	Class Constructor DIOMACMANUFACTURED
//	
//	@author				Abraham J. Velez
//	@version			15/02/2015 21:02:25
//	
 
 
 
//  @param				xpublisher : 
*/
/*-----------------------------------------------------------------*/
DIOMACMANUFACTURED::DIOMACMANUFACTURED()
{
	Clean();		
}



/*-------------------------------------------------------------------
//  DIOMACMANUFACTURED::~DIOMACMANUFACTURED
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			22/04/2013 20:02:11
//	
//	@return 			
//	*/
/*-----------------------------------------------------------------*/
DIOMACMANUFACTURED::~DIOMACMANUFACTURED()
{
	Clean();
}




/*-------------------------------------------------------------------
//  DIOMACMANUFACTURED::Web_GetManufactured
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			22/04/2013 22:13:30
//	
//	@return 			bool : 
//	@param				deviceMAC : 
//  @param				manufactured : 
//  @param				usewebservice : 
*/
/*-----------------------------------------------------------------*/
bool DIOMACMANUFACTURED::Web_GetManufactured(DIOMAC& MAC, XSTRING& manufactured)
{
	bool status = false;

	DIOWEBSCRAPERMACMANUFACTURER* macmanufactured = new DIOWEBSCRAPERMACMANUFACTURER;
	if(macmanufactured)
		{				
			status = macmanufactured->Get(MAC, manufactured);
			delete macmanufactured;
		}
	
	return status;
}





/*-------------------------------------------------------------------
//  DIOMACMANUFACTURED::File_GetManufactured
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			25/04/2013 22:30:28
//	
//	@return 			bool : 
//	@param				xpath : 
//  @param				ID : 
//  @param				manufactured : 
*/
/*-----------------------------------------------------------------*/
bool DIOMACMANUFACTURED::File_GetManufactured(XPATH& xpath, DIOMAC& MAC, XSTRING& manufactured)
{
	if(xpath.IsEmpty())
		{
			XPATH xpathresource;
			
			XPATHSMANAGER::GetInstance().GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpathresource);

			xpath.Format(__L("%s%s"), xpathresource.Get(), DIOMACMANUFACTURED_FILE_NAME);
		}

	HASHMD5		hashmd5; 
	XFILEXDB* xfilexdb = new XFILEXDB( &hashmd5, xpath);
	bool			status;

	if(xfilexdb)
		{
			xfilexdb->Set(XFILE_XDB_ID, XFILEXDB_TYPE, DIOMACMANUFACTURED_FILE_VERSION, DIOMACMANUFACTURED_FILE_IDSTRING);
	
			status = xfilexdb->Ini();
			if(status)
				{
					manufactured.Empty();

					XBUFFER* xbuffer = xfilexdb->GetRecord(MAC.GetManufactured());
					if(xbuffer) 
						{	
							manufactured.Set((XWORD*)xbuffer->Get());
							delete xbuffer;
						}		
				}
			
			xfilexdb->End();

			delete xfilexdb;
		}

	return status;
}




/*-------------------------------------------------------------------
//  DIOMACMANUFACTURED::File_GetManufacturedMACs
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			26/04/2013 17:32:46
//	
//	@return 			bool : 
//	@param				xpath : 
//  @param				manufactured : 
//  @param				MACs : 
*/
/*-----------------------------------------------------------------*/
bool DIOMACMANUFACTURED::File_GetManufacturedMACs(XPATH& xpath, XSTRING& manufactured, XVECTOR<XDWORD>& MACs)
{
	if(xpath.IsEmpty())
		{
			XPATH xpathresource;
			
			XPATHSMANAGER::GetInstance().GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpathresource);

			xpath.Format(__L("%s%s"), xpathresource.Get(), DIOMACMANUFACTURED_FILE_NAME);
		}

	HASHMD5   hashmd5; 
	XFILEXDB* xfilexdb = new XFILEXDB( &hashmd5, xpath);
	bool			status;

	if(xfilexdb)
		{
			xfilexdb->Set(XFILE_XDB_ID, XFILEXDB_TYPE, DIOMACMANUFACTURED_FILE_VERSION, DIOMACMANUFACTURED_FILE_IDSTRING);
	
			status = xfilexdb->Ini();
			if(status)
				{
					for(int c=0;c<xfilexdb->GetNumberRecords();c++)
						{
							XSTRING _manufactured;
							
							_manufactured.Empty();

							XBUFFER* xbuffer = xfilexdb->GetRecordIndex(c);
							if(xbuffer) 
								{	
									_manufactured.Set((XWORD*)xbuffer->Get());

									if(_manufactured.Find(manufactured,true)!=XSTRING_NOTFOUND)
										{
											XDWORD ID =  xfilexdb->GetIndexMap()->GetKey(c);

											MACs.Add(ID);

											//XDEBUG_PRINTCOLOR(2,__L("[%08X] [%s]"), ID, _manufactured.Get());
										}

									delete xbuffer;
								}		
						}
				}
			
			xfilexdb->End();

			delete xfilexdb;
		}

	return status;
}





/*-------------------------------------------------------------------
//  DIOMACMANUFACTURED::File_Download
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			26/04/2013 22:35:29
//	
//	@return 			bool : 
//	@param				xbuffer : 
//  @param				timeout : 
*/
/*-----------------------------------------------------------------*/
bool DIOMACMANUFACTURED::File_Download(XBUFFER& xbuffer, int timeout)
{	
	DIOURL url;
	bool	 status;

	url  = DIOMACMANUFACTURED_URL;
	url += DIOMACMANUFACTURED_URLNAMEFILE;
	
	DIOWEBCLIENT* webclient	= new DIOWEBCLIENT;
	if(!webclient) return false;

	//XDEBUG_PRINTCOLOR(2,__L("Get File %s "), url.Get());
	status = webclient->Get(url, xbuffer, DIOWEBCLIENT_DEFAULTUSERAGENT, timeout);
	
	delete webclient;

	return status;
}
		



/*-------------------------------------------------------------------
//  DIOMACMANUFACTURED::File_Convert
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			26/04/2013 22:38:41
//	
//	@return 			bool : 
//	@param				xbuffer : 
//  @param				xpath : 
*/
/*-----------------------------------------------------------------*/
bool DIOMACMANUFACTURED::File_Convert(XBUFFER& xbuffer, XPATH& xpath)
{
	if(!xfactory)				return false;
	if(xpath.IsEmpty())	return false;

	XFILETXT* xfiletxtoui = new XFILETXT();
	if(!xfiletxtoui) return false;

	XMAP<XDWORD, XSTRING*>	idmap;
	XMAP<XDWORD, int>				idmapindex;		
	XDWORD									c;

	idmap.DeleteAll();
	idmapindex.DeleteAll();

	idmap.SetAddInLimit(10000);
	idmapindex.SetAddInLimit(10000);

	xfiletxtoui->AddBufferLines(XFILETXTFORMATCHAR_ASCII, xbuffer);

	int nlines = xfiletxtoui->GetNLines();
	for(int c=0;c<nlines;c++)
		{				
			XSTRING* line  = xfiletxtoui->GetLine(c);
			if(line)
				{				
					if(!line->IsEmpty())
						{
							int start = line->Find(__L("(base 16)"),true);
							if(start!=XSTRING_NOTFOUND)
								{
									XDWORD	ID = 0;	
									
									//XDEBUG_PRINTCOLOR(2,__L("Line %d / %d"), c, nlines);																						

									start+=11;
									line->UnFormat(__L("%X    (base 16)\t\t"),&ID);
											
									if(idmap.Find(ID)==-1) 
										{		
											XSTRING* name = new XSTRING();											
											if(name)
												{
													line->Copy(start,(*name));																
													idmap.Add(ID,name);		

													//XDEBUG_PRINTCOLOR(2,__L("[%08X] [%s]"), ID, name->Get());
												}
										}												
								}								
						}
				}
		}					
			
	delete xfiletxtoui;
	
	XDWORD		nrecords =	idmap.GetSize();
	HASHMD5		hashmd5; 
	int				initablepos = 0;	
	int				filepos			= 0;

	XFILEXDB* xfilexdb = new XFILEXDB( &hashmd5, xpath);
	if(xfilexdb)
		{
			xfilexdb->Set(XFILE_XDB_ID, XFILEXDB_TYPE, DIOMACMANUFACTURED_FILE_VERSION, DIOMACMANUFACTURED_FILE_IDSTRING);
		
			if(xfilexdb->Create(xpath))
				{
					xfilexdb->GetPrimaryFile()->Write((XBYTE*)&nrecords, sizeof(XDWORD));
					xfilexdb->GetPrimaryFile()->GetPosition(initablepos);

					XDWORD ID = 0;

					for(c=0;c<nrecords;c++)
						{		
							xfilexdb->GetPrimaryFile()->Write((XBYTE*)&ID, sizeof(XDWORD));
							xfilexdb->GetPrimaryFile()->Write((XBYTE*)&ID, sizeof(XDWORD));
						}

					for(c=0;c<nrecords;c++)
						{     	
							size_t	 _name	= (size_t)idmap.GetElement(c);							
							XSTRING* name		= (XSTRING*)_name;
							if(name)
								{ 
									ID = (XDWORD)idmap.GetKey(c);

									xfilexdb->GetPrimaryFile()->GetPosition(filepos);

									XDWORD size = (name->GetSize()+1)*sizeof(XWORD);

									xfilexdb->GetPrimaryFile()->Write((XBYTE*)&size								  ,	sizeof(XDWORD));

									XSTRING_CREATENORMALIZE((*name), buffnormalize, false)
									xfilexdb->GetPrimaryFile()->Write((XBYTE*)buffnormalize	,	size);
									XSTRING_DELETENORMALIZE(buffnormalize)
			
									idmapindex.Add(ID,filepos);

									delete name;
								}
						}

					xfilexdb->GetPrimaryFile()->SetPosition(initablepos-sizeof(XDWORD));
					xfilexdb->GetPrimaryFile()->Write((XBYTE*)&nrecords,sizeof(XDWORD));

					for(c=0;c<nrecords;c++)
						{
							ID			= (XDWORD)idmapindex.GetKey(c);
							filepos = (XDWORD)idmapindex.GetElement(c);

							xfilexdb->GetPrimaryFile()->Write((XBYTE*)&ID			, sizeof(XDWORD));
							xfilexdb->GetPrimaryFile()->Write((XBYTE*)&filepos, sizeof(XDWORD));
						}
	
					xfilexdb->CloseFile();
				}
		}
	
	idmap.DeleteAll();
	idmapindex.DeleteAll();

	return true;	
}




