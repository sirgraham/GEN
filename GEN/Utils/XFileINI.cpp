//------------------------------------------------------------------------------------------
//	XFILEINI.CPP
//	
//	INI File functions (INI format)
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 04/06/2004 12:02:06
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>

#include "XPath.h"

#include "XFileINI.h"
	
#include "XMemory.h"


//---- GENERAL VARIABLE --------------------------------------------------------------------
	
	
//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  XFILEINIKEY::XFILEINIKEY
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			12/07/2001 12:35:00
//	
//	@return 			
//	*/
//-------------------------------------------------------------------
XFILEINIKEY::XFILEINIKEY()
{
	Clean();
}


//-------------------------------------------------------------------
//  XFILEINIKEY::~XFILEINIKEY
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			12/07/2001 12:35:37
//	
//	@return 			
//	*/
//-------------------------------------------------------------------
XFILEINIKEY::~XFILEINIKEY()
{
	Clean();
}


//-------------------------------------------------------------------
//  XFILEINIKEY::GetKey
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			12/07/2001 12:36:27
//	
//	@return 			XSTRING& : 
//	*/
//-------------------------------------------------------------------
XSTRING* XFILEINIKEY::GetKey()
{  
	return &key;
}


//-------------------------------------------------------------------
//  XFILEINIKEY::SetKey
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			12/07/2001 12:36:48
//	
//	@return 			bool : 
//	@param				key : 
*/
//-------------------------------------------------------------------
bool XFILEINIKEY::SetKey(XSTRING& key)
{
	this->key = key;

  return true;
}


//-------------------------------------------------------------------
//  XFILEINIKEY::GetValue
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			12/07/2001 12:37:22
//	
//	@return 			XSTRING& : 
//	*/
//-------------------------------------------------------------------
XSTRING* XFILEINIKEY::GetValue()
{
	return &value;
}
	

//-------------------------------------------------------------------
//  XFILEINIKEY::SetValue
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			12/07/2001 12:37:44
//	
//	@return 			bool : 
//	@param				value : 
*/
//-------------------------------------------------------------------
bool XFILEINIKEY::SetValue(XSTRING& value)
{
	this->value = value;

	return true;
}


//-------------------------------------------------------------------
//  XFILEINIKEY::Clean
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			04/09/2001 12:45:31
//	
//	@return 			void : 
//	*/
//-------------------------------------------------------------------
void XFILEINIKEY::Clean()
{
	
}
	


//-------------------------------------------------------------------
//  XFILEINISECTION::XFILEINISECTION
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			12/07/2001 12:44:29
//	
//	@return 			
//	*/
//-------------------------------------------------------------------
XFILEINISECTION::XFILEINISECTION()
{
	Clean();
}

//-------------------------------------------------------------------
//  XFILEINISECTION::~XFILEINISECTION
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			12/07/2001 12:45:00
//	
//	@return 			
//	*/
//-------------------------------------------------------------------
XFILEINISECTION::~XFILEINISECTION()
{
	Clean();
}



//-------------------------------------------------------------------
//  XFILEINISECTION::GetName
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			12/07/2001 12:45:59
//	
//	@return 			XSTRING& : 
//	*/
//-------------------------------------------------------------------
XSTRING* XFILEINISECTION::GetName()
{
	return &section;
}


//-------------------------------------------------------------------
//  XFILEINISECTION::SetName
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			12/07/2001 12:46:27
//	
//	@return 			bool : 
//	@param				section : 
*/
//-------------------------------------------------------------------
bool XFILEINISECTION::SetName(XSTRING& section)
{
	this->section = section;

	return true;
}


//-------------------------------------------------------------------
//  XFILEINISECTION::FindKey
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			12/07/2001 12:28:27
//	
//	@return 			XFILEINIKEY* : 
//	@param				*key : 
*/
//-------------------------------------------------------------------
XFILEINIKEY* XFILEINISECTION::FindKey(XSTRING& key)
{
	if(keys.IsEmpty()) return NULL;

	for(XDWORD c=0;c<keys.GetSize();c++)
		{
			XFILEINIKEY* okey = (XFILEINIKEY*)keys.Get(c);
			if(okey)
				{					
					if(okey->GetKey())
						{
							if(!okey->GetKey()->Compare(key, true)) return okey;
						}	
				}			
		}

  return NULL;
}



//-------------------------------------------------------------------
//  XFILEINISECTION::GetKey
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			12/07/2001 12:47:54
//	
//	@return 			bool : 
//	@param				key : 
//  @param				value : 
*/
//-------------------------------------------------------------------
bool XFILEINISECTION::GetKey(XSTRING& key,XSTRING& value)	
{  
	XFILEINIKEY* okey=FindKey(key);

	if(!okey) return false;

	if(okey->GetValue()) value = (*okey->GetValue()); else return false;
	
	return true;
}


//-------------------------------------------------------------------
//  XFILEINISECTION::SetKey
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			12/07/2001 12:48:15
//	
//	@return 			bool : 
//	@param				key : 
//  @param				value : 
*/
//-------------------------------------------------------------------
bool XFILEINISECTION::SetKey(XSTRING& key,XSTRING& value)
{
	XFILEINIKEY* okey  = FindKey(key);
	bool				 isnew = false;

	if(!okey) isnew=true;
	
	if(isnew)
		{
			okey = new XFILEINIKEY(); 
			if(!okey) return false;
		}

	okey->SetKey(key);
	okey->SetValue(value);

	if(isnew) keys.Add(okey);
	
	return true;
}


/*-------------------------------------------------------------------
//  XFILEINISECTION::DeleteKey
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			06/10/2010 20:16:32
//	
//	@return				bool : 
//	@param				key : 
*/
/*-----------------------------------------------------------------*/
bool XFILEINISECTION::DeleteKey(XSTRING& key)
{
	XFILEINIKEY* okey  = FindKey(key);
	if(!okey) return false;

	keys.Delete(okey);
	delete okey;
	
	return true;
}



//-------------------------------------------------------------------
//  XFILEINISECTION::GetNextKey
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			12/07/2001 16:37:14
//	
//	@return 			bool : 
//	@param				*key : 
//  @param				value : 
*/
//-------------------------------------------------------------------
bool XFILEINISECTION::GetNextKey(XSTRING& key,XSTRING& value)
{
	if(selectkey>=keys.GetSize()) return false;

	XFILEINIKEY* skey	= (XFILEINIKEY*)keys.Get(selectkey);
	if(!skey) return false;

	if(skey->GetKey())		key   = (*skey->GetKey());	
	if( skey->GetValue()) value = (* skey->GetValue());	

	selectkey++;

	return true;  
}




//-------------------------------------------------------------------
//  XFILEINISECTION::ResetSelectionKey
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			12/07/2001 16:42:43
//	
//	@return 			bool : 
//	*/
//-------------------------------------------------------------------
bool XFILEINISECTION::ResetSelectionKey()
{
	selectkey = 0;

	return true;
}


//-------------------------------------------------------------------
//  XFILEINISECTION::Clean
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			04/09/2001 12:45:54
//	
//	@return 			void : 
//	*/
//-------------------------------------------------------------------
void XFILEINISECTION::Clean()
{
	section.Empty();

	ResetSelectionKey();

	if(keys.IsEmpty()) return;

	keys.DeleteContents();	
	keys.DeleteAll();
}





//-------------------------------------------------------------------
//  XFILEINI::XFILEINI
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
XFILEINI::XFILEINI() : XFILETXT()
{
	Clean();
}


//-------------------------------------------------------------------
//  XFILEINI::XFILEINI
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/07/2002 19:15:55
//	
//	@return 			

//  @param				xpath : 
*/
//-------------------------------------------------------------------
XFILEINI::XFILEINI(XPATH& xpath)  : XFILETXT()
{
	Clean();	

	Open(xpath);
}



//-------------------------------------------------------------------
//  XFILEINI::XFILEINI
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
XFILEINI::XFILEINI(XFILE* file)  : XFILETXT()
{
	Clean();

	this->file = file;

}



//-------------------------------------------------------------------
//  XFILEINI::~XFILEINI
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/04/2002 9:48:21
//
//	@return
//	*/
//-------------------------------------------------------------------
XFILEINI::~XFILEINI()
{
	Close();
	
	Clean();
}


//-------------------------------------------------------------------
//  XFILEINI::Open
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
bool XFILEINI::Open(XPATH& xpath,bool readonly)
{
	if(!XFILETXT::Open(xpath,readonly)) return false;

	Serialization(true);

	return true;
}
	


//-------------------------------------------------------------------
//  XFILEINI::Close
/**
//
//
//	@author				Abraham J. Velez
//	@version			15/04/2002 9:38:08
//	
//	@return 			bool : 
*/
//-------------------------------------------------------------------
bool XFILEINI::Close()
{
	bool status[2] = { false , false };
	
	status[0] = Serialization(false);	
	status[1] = XFILETXT::Close();

	if(status[0] && status[1]) return true;

	return false;
}



//-------------------------------------------------------------------
//  XFILEINI::SelectSection
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			12/07/2001 12:17:19
//	
//	@return 			bool : 
//	@param				*section : 
*/
//-------------------------------------------------------------------
bool XFILEINI::SelectSection(XSTRING& section)
{	
	if(sections.IsEmpty()) return false;

	for(XDWORD c=0;c<sections.GetSize();c++)
		{
			XFILEINISECTION* osection=(XFILEINISECTION*)sections.Get(c);			
			if(osection)
				{
					if(osection->GetName())
						{
							if(!osection->GetName()->Compare(section, true))
								{							
									selectsection = c;
									osection->ResetSelectionKey();
										
									return true;						
								}
						}	
				}
		}

	return false;
}



/*-------------------------------------------------------------------
//  XFILEINI::SelectSection
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/04/2011 16:36:12
//	
//	@return				bool : 
//	@param				section : 
*/
/*-----------------------------------------------------------------*/
bool XFILEINI::SelectSection(XCHAR* section)
{
	XSTRING _section(section);

	return SelectSection(_section);
}



//-------------------------------------------------------------------
//  XFILEINI::CreateSection
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			12/07/2001 12:22:13
//	
//	@return 			bool : 
//	@param				*section : 
*/
//-------------------------------------------------------------------
bool XFILEINI::CreateSection(XSTRING& section)
{
  if(SelectSection(section)) return false;

	XFILEINISECTION* osection;

	osection = new XFILEINISECTION();
	if(!osection) return false;
  
	if(!osection->SetName(section)) return false;

	sections.Add(osection);	
	selectsection = sections.GetSize()-1;

	return true;
}



/*-------------------------------------------------------------------
//  XFILEINI::CreateSection
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/04/2011 16:37:13
//	
//	@return				bool : 
//	@param				section : 
*/
/*-----------------------------------------------------------------*/
bool XFILEINI::CreateSection(XCHAR* section)
{
	XSTRING _section(section);

	return CreateSection(_section);
}



//-------------------------------------------------------------------
//  XFILEINI::DeleteSection
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			12/07/2001 12:17:19
//	
//	@return 			bool : 
//	@param				*section : 
*/
//-------------------------------------------------------------------
bool XFILEINI::DeleteSection(XSTRING& section)
{
	if(sections.IsEmpty()) return false;

	for(XDWORD c=0;c<sections.GetSize();c++)
		{
			XFILEINISECTION* osection=(XFILEINISECTION*)sections.Get(c);

			if(osection)
				{
					if(osection->GetName())
						{
							if(!osection->GetName()->Compare(section, true))
								{
									sections.Delete(osection);
		              
									osection->Clean();
				    			delete osection;

									return true;
								}
						}
				}
		}

	return false;
}



/*-------------------------------------------------------------------
//  XFILEINI::DeleteSection
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/04/2011 16:38:30
//	
//	@return				bool : 
//	@param				section : 
*/
/*-----------------------------------------------------------------*/
bool XFILEINI::DeleteSection(XCHAR* section)
{
	XSTRING _section(section);

	return DeleteSection(_section);
}



/*-------------------------------------------------------------------
//  XFILEINI::DeleteKey
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			06/10/2010 20:12:55
//	
//	@return				bool : 
//	@param				*section : 
//  @param				*key : 
*/
/*-----------------------------------------------------------------*/
bool XFILEINI::DeleteKey(XSTRING& section,XSTRING& key)
{
	if(!SelectSection(section)) return false;

	XFILEINISECTION* ssection = (XFILEINISECTION*)sections.Get(selectsection);
	if(!ssection) return false;
		
	return ssection->DeleteKey(key);
}



/*-------------------------------------------------------------------
//  XFILEINI::DeleteKey
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/04/2011 16:40:19
//	
//	@return				bool : 
//	@param				section : 
//  @param				key : 
*/
/*-----------------------------------------------------------------*/
bool XFILEINI::DeleteKey(XCHAR* section,XSTRING& key)
{
	XSTRING _section(section);

	return DeleteKey(_section,key);
}



/*-------------------------------------------------------------------
//  XFILEINI::DeleteKey
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/04/2011 16:40:58
//	
//	@return				bool : 
//	@param				section : 
//  @param				key : 
*/
/*-----------------------------------------------------------------*/
bool XFILEINI::DeleteKey(XSTRING& section,XCHAR* key)
{
	XSTRING _key;

	_key = key;

	return DeleteKey(section,_key);
}



/*-------------------------------------------------------------------
//  XFILEINI::DeleteKey
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/04/2011 16:41:07
//	
//	@return				bool : 
//	@param				section : 
//  @param				key : 
*/
/*-----------------------------------------------------------------*/
bool XFILEINI::DeleteKey(XCHAR* section,XCHAR* key)
{
	XSTRING _section(section);	
	XSTRING _key(key);

	return DeleteKey(_section,_key);
}



/*-------------------------------------------------------------------
//  XFILEINI::ReadValue
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/05/2011 10:55:47
//	
//	@return				bool : 
//	@param				section : 
//  @param				key : 
//  @param				value : 
*/
/*-----------------------------------------------------------------*/
bool XFILEINI::ReadValue(XSTRING& section,XSTRING& key,XSTRING& value)
{
	value.Empty();

	if(!SelectSection(section)) return false;

	XFILEINISECTION* ssection = (XFILEINISECTION*)sections.Get(selectsection);
	if(!ssection) return false;
	
	XFILEINIKEY* okey=ssection->FindKey(key);
	if(!okey) return false;

	if(okey->GetValue()) value = (*okey->GetValue());
	
	return true;	
}



/*-------------------------------------------------------------------
//  XFILEINI::ReadValue
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/04/2011 16:49:07
//	
//	@return				bool : 
//	@param				section : 
//  @param				key : 
//  @param				value : 
*/
/*-----------------------------------------------------------------*/
bool XFILEINI::ReadValue(XCHAR* section,XSTRING& key,XSTRING& value)
{
	XSTRING _section(section);	

	return ReadValue(_section,key,value);
}



/*-------------------------------------------------------------------
//  XFILEINI::ReadValue
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/04/2011 16:49:16
//	
//	@return				bool : 
//	@param				section : 
//  @param				key : 
//  @param				value : 
*/
/*-----------------------------------------------------------------*/
bool XFILEINI::ReadValue(XSTRING& section,XCHAR* key,XSTRING& value)
{
	XSTRING _key(key);

	return ReadValue(section,_key,value);
}



/*-------------------------------------------------------------------
//  XFILEINI::ReadValue
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/04/2011 16:49:28
//	
//	@return				bool : 
//	@param				section : 
//  @param				key : 
//  @param				value : 
*/
/*-----------------------------------------------------------------*/
bool XFILEINI::ReadValue(XCHAR* section,XCHAR* key,XSTRING& value)
{
	XSTRING _section(section);	
	XSTRING _key(key);

	return ReadValue(_section,_key,value);
}




/*-------------------------------------------------------------------
//  XFILEINI::ReadValue
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/05/2011 10:56:03
//	
//	@return				bool : 
//	@param				key : 
//  @param				value : 
*/
/*-----------------------------------------------------------------*/
bool XFILEINI::ReadValue(XSTRING& key,XSTRING& value)
{
	value.Empty();

	if(sections.IsEmpty()) return false;	

	selectsection=0;

	XFILEINISECTION* ssection = (XFILEINISECTION*)sections.Get(selectsection);
	if(!ssection)						 return false;
	if(!ssection->GetName()) return false;

	return ReadValue((*ssection->GetName()),key,value);	
}



/*-------------------------------------------------------------------
//  XFILEINI::ReadValue
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/05/2011 10:55:26
//	
//	@return				bool : 
//	@param				key : 
//  @param				value : 
*/
/*-----------------------------------------------------------------*/
bool XFILEINI::ReadValue(XCHAR* key,XSTRING& value)
{
	XSTRING _key(key);

	return ReadValue(_key,value);
}




/*-------------------------------------------------------------------
//  XFILEINI::WriteValue
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/05/2011 10:56:17
//	
//	@return				bool : 
//	@param				section : 
//  @param				key : 
//  @param				value : 
*/
/*-----------------------------------------------------------------*/
bool XFILEINI::WriteValue(XSTRING& section,XSTRING& key,XSTRING& value)
{ 	
	if(!SelectSection(section)) 
		{
			if(!CreateSection(section)) return false;
			if(!SelectSection(section)) return false;
		}

	XFILEINISECTION* ssection = (XFILEINISECTION*)sections.Get(selectsection);
	if(!ssection) return false;

	return ssection->SetKey(key,value);
}



/*-------------------------------------------------------------------
//  XFILEINI::WriteValue
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/04/2011 16:49:07
//	
//	@return				bool : 
//	@param				section : 
//  @param				key : 
//  @param				value : 
*/
/*-----------------------------------------------------------------*/
bool XFILEINI::WriteValue(XCHAR* section,XSTRING& key,XSTRING& value)
{
	XSTRING _section(section);	
	
	return WriteValue(_section,key,value);
}



/*-------------------------------------------------------------------
//  XFILEINI::WriteValue
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/04/2011 16:49:16
//	
//	@return				bool : 
//	@param				section : 
//  @param				key : 
//  @param				value : 
*/
/*-----------------------------------------------------------------*/
bool XFILEINI::WriteValue(XSTRING& section,XCHAR* key,XSTRING& value)
{
	XSTRING _key(key);

	return WriteValue(section,_key,value);
}



/*-------------------------------------------------------------------
//  XFILEINI::WriteValue
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/04/2011 16:49:28
//	
//	@return				bool : 
//	@param				section : 
//  @param				key : 
//  @param				value : 
*/
/*-----------------------------------------------------------------*/
bool XFILEINI::WriteValue(XCHAR* section,XCHAR* key,XSTRING& value)
{
	XSTRING _section(section);	
	XSTRING _key(key);

	return WriteValue(_section,_key,value);
}




/*-------------------------------------------------------------------
//  XFILEINI::WriteValue
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/05/2011 10:56:30
//	
//	@return				bool : 
//	@param				key : 
//  @param				value : 
*/
/*-----------------------------------------------------------------*/
bool XFILEINI::WriteValue(XSTRING& key,XSTRING& value)
{
	if(sections.IsEmpty()) return false;	

	selectsection=0;

	XFILEINISECTION* ssection = (XFILEINISECTION*)sections.Get(selectsection);
	if(!ssection)						 return false;
	if(!ssection->GetName()) return false;

	return WriteValue((*ssection->GetName()),key,value);	
}



/*-------------------------------------------------------------------
//  XFILEINI::WriteValue
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/04/2011 16:57:34
//	
//	@return				bool : 
//	@param				key : 
//  @param				value : 
*/
/*-----------------------------------------------------------------*/
bool XFILEINI::WriteValue(XCHAR* key,XSTRING& value)
{
	XSTRING _key(key);

	return ReadValue(_key,value);
}




/*-------------------------------------------------------------------
//  XFILEINI::GetNextKey
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/05/2011 10:56:40
//	
//	@return				bool : 
//	@param				key : 
//  @param				value : 
*/
/*-----------------------------------------------------------------*/
bool XFILEINI::GetNextKey(XSTRING& key,XSTRING& value)
{
	if(sections.IsEmpty()) return false;
			
	selectsection=0; 

	XFILEINISECTION* ssection = (XFILEINISECTION*)sections.Get(selectsection);
	if(!ssection) return false;

	return ssection->GetNextKey(key,value);
}



//-------------------------------------------------------------------
//  XFILEINI::ReadKeyMap
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			12/07/2001 13:05:49
//	
//	@return 			bool : 
//	@param				section : 
//  @param				keymap : 
//  @param				nkeymap : 
*/
//-------------------------------------------------------------------
bool XFILEINI::ReadKeyMap(XSTRING& section,XFILEINIKEYMAP* keymap,int nkeymap)
{
	if(!nkeymap)								return false;
	if(!SelectSection(section)) return false;

	XSTRING value;

	for(int c=0;c<nkeymap;c++)
		{
			if(ReadValue(keymap[c].key,value)) keymap[c].value = value;				  									  										
		}

  return true;                 	
}



/*-------------------------------------------------------------------
//  XFILEINI::ReadKeyMap
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/04/2011 16:59:52
//	
//	@return				bool : 
//	@param				section : 
//  @param				keymap : 
//  @param				nkeymap : 
*/
/*-----------------------------------------------------------------*/
bool XFILEINI::ReadKeyMap(XCHAR* section,XFILEINIKEYMAP* keymap,int nkeymap)
{
	XSTRING _section(section);	
	
	return ReadKeyMap(_section,keymap,nkeymap);
}



/*-------------------------------------------------------------------
//	XFILEINI::ConvertFromLines
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			19/05/2014 10:35:17
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool XFILEINI::ConvertFromLines()
{
	XSTRING section;
	XSTRING actualsection;

	DeleteAllSections();

	for(int c=0;c<GetNLines();c++)
		{	
			XSTRING* line = GetLine(c);

			if(line)
				{	
					if(LineIsSection((*line),section))
 						{
							actualsection = section;
  						CreateSection(actualsection);
						}
 					 else
						{
 							XSTRING key;
							XSTRING value;

							if(LineIsKey((*line),key,value)) 
								{
									WriteValue(actualsection,key,value);									
								}
						}
				}
		}
	 	
	return true;	
}




/*-------------------------------------------------------------------
//	XFILEINI::ConvertToLines
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			19/05/2014 10:35:35
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool XFILEINI::ConvertToLines()
{
	if(sections.IsEmpty()) return false;

	for(XDWORD c=0;c<sections.GetSize();c++)
		{
			XFILEINISECTION* section = (XFILEINISECTION*)sections.Get(c);

			if(section)
				{		
					XSTRING line(__L(" "));
					XSTRING key;
					XSTRING value;
									
					AddLine(line.Get());

					line.Format(__L("[%s]"),section->GetName()->Get());
					AddLine(line.Get());
						
					section->ResetSelectionKey();

					while(section->GetNextKey(key,value))
						{
							line.Format(__L("%s=%s"),key.Get(),value.Get());
							AddLine(line.Get());
						}	
				}
		}
							
	return true;	
}




/*-------------------------------------------------------------------
//	XFILEINI::Serialization
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			19/05/2014 10:34:06
//	
//	@return 			bool : 
//
//  @param				read : 
*/
/*-----------------------------------------------------------------*/
bool XFILEINI::Serialization(bool read)
{
	bool status;

	file->SetPosition(XFILE_SEEKINI);

	DeleteAllLines();

	if(read) 
		{
			ReadAllFile();
			status = ConvertFromLines();
	  }
	 else
		{
			status = ConvertToLines();
			if(status)
				{
					WriteAllFile();

					int pos = 0;
					file->GetPosition(pos);

					if(pos) file->SetSize(pos);	
				}
		}

	DeleteAllLines();

	return status;	
}




/*-------------------------------------------------------------------
//  XFILEINI::DeleteAllSections
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			29/04/2011 18:12:35
//	
//	@return				bool : 
//	*/
/*-----------------------------------------------------------------*/
bool XFILEINI::DeleteAllSections()
{
	selectsection = 0;

	if(sections.IsEmpty()) return false;

	for(XDWORD c=0; c<sections.GetSize(); c++)
		{
			XFILEINISECTION* osection = sections.Get(c);
			if(osection)
				{
					osection->Clean();
					delete osection;
				}
		}
	
	sections.DeleteAll();	

	return true;
}




//-------------------------------------------------------------------
//  XFILEINI::Clean
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			04/09/2001 12:46:19
//	
//	@return 			void : 
//	*/
//-------------------------------------------------------------------
void XFILEINI::Clean()
{
	DeleteAllSections();
}



/*-------------------------------------------------------------------
//  XFILEINI::LineIsSection
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/04/2011 13:47:45
//	
//	@return				bool : 
//	@param				line : 
//  @param				section : 
*/
/*-----------------------------------------------------------------*/
bool XFILEINI::LineIsSection(XSTRING& line,XSTRING& section)
{	
	section.Empty();

	if(line.IsEmpty()) return false;
	
	line.Copy(__L("["),__L("]"),true,0,section);

	if(section.IsEmpty()) return false;

	section.ToUpperCase();
	section.DeleteCharacter(__C(' '),XSTRINGCONTEXT_ATFIRST);
	section.DeleteCharacter(__C(' '),XSTRINGCONTEXT_ATEND);

	return true;
}



/*-------------------------------------------------------------------
//  XFILEINI::LineIsKey
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/04/2011 13:47:04
//	
//	@return				bool : 
//	@param				line : 
//  @param				key : 
//  @param				value : 
*/
/*-----------------------------------------------------------------*/
bool XFILEINI::LineIsKey(XSTRING& line,XSTRING& key,XSTRING& value)
{
	key.Empty();
	value.Empty();

	if(line.IsEmpty()) return false;

	if((line.Find(__L("["),true)!=XSTRING_NOTFOUND) &&
		 (line.Find(__L("]"),true)!=XSTRING_NOTFOUND))  return false;

	int indexvalue = line.Find(__L("="),true);
	if(indexvalue == XSTRING_NOTFOUND) return false;

	line.Copy(0,indexvalue,key);
	line.Copy(indexvalue+1,line.GetSize(),value);

	if(key.IsEmpty()) return false;
	
	//key.ToUpperCase();
	key.DeleteCharacter(__C(' '),XSTRINGCONTEXT_ATFIRST);
	key.DeleteCharacter(__C(' '),XSTRINGCONTEXT_ATEND);

	value.DeleteCharacter(__C(' '),XSTRINGCONTEXT_ATFIRST);
	value.DeleteCharacter(__C(' '),XSTRINGCONTEXT_ATEND);

	return true;
}

