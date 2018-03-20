//------------------------------------------------------------------------------------------
//	XFILECSV.CPP
//	
//	File CSV functions
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 02/09/2002
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>

#include "XFactory.h"	
#include "XPath.h"

#include "XFileCSV.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	
	
//---- CLASS MEMBERS -----------------------------------------------------------------------



/*-------------------------------------------------------------------
//  XFILECSV_RECORD::XFILECSV_RECORD
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			06/05/2011 16:37:51
//	
//	@return				
//	*/
/*-----------------------------------------------------------------*/
XFILECSV_RECORD::XFILECSV_RECORD()
{
	Clean();
}
		

/*-------------------------------------------------------------------
//  XFILECSV_RECORD::~XFILECSV_RECORD
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			06/05/2011 16:37:55
//	
//	@return				
//	*/
/*-----------------------------------------------------------------*/
XFILECSV_RECORD::~XFILECSV_RECORD()
{
	DeleteAllElements();

	Clean();
}



/*-------------------------------------------------------------------
//  XFILECSV_RECORD::GetNElements
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			06/05/2011 17:13:41
//	
//	@return				int : 
//	*/
/*-----------------------------------------------------------------*/
int XFILECSV_RECORD::GetNElements()
{
	return elements.GetSize();
}




/*-------------------------------------------------------------------
//  XFILECSV_RECORD::GetElement
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			06/05/2011 17:24:58
//	
//	@return				bool : 
//	@param				index : 
//  @param				element : 
*/
/*-----------------------------------------------------------------*/
bool XFILECSV_RECORD::GetElement(int index, XSTRING& element)
{
	XSTRING* string = (XSTRING*)elements.Get(index);
	if(!string) return false;

	//if(string->IsEmpty()) return false;

	element = (*string);
	
	return true;
}



/*-------------------------------------------------------------------
//  XFILECSV_RECORD::GetElement
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			06/05/2011 17:16:30
//	
//	@return				XCHAR* : 
//	@param				index : 
*/
/*-----------------------------------------------------------------*/
XCHAR* XFILECSV_RECORD::GetElement(int index)
{	
	XSTRING* string = (XSTRING*)elements.Get(index);
	if(!string) return NULL;

	//if(string->IsEmpty()) return NULL;

	return string->Get();
}



/*-------------------------------------------------------------------
//  XFILECSV_RECORD::AddElement
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			06/05/2011 17:19:13
//	
//	@return				bool : 
//	@param				element : 
*/
/*-----------------------------------------------------------------*/
bool XFILECSV_RECORD::AddElement(XSTRING& element)
{
	//if(element.IsEmpty()) return false;

	XSTRING* string = new XSTRING();
	if(!string) return false;

	(*string) = element;

	elements.Add(string);

	return true;
}



/*-------------------------------------------------------------------
//  XFILECSV_RECORD::AddElement
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			06/05/2011 17:20:09
//	
//	@return				bool : 
//	@param				element : 
*/
/*-----------------------------------------------------------------*/
bool XFILECSV_RECORD::AddElement(XCHAR* element)
{
	if(!element)		return false;
	//if(!element[0]) return false;

	XSTRING* string = new XSTRING();
	if(!string) return false;

	(*string) = element;

	elements.Add(string);

	return true;
}



/*-------------------------------------------------------------------
//  XFILECSV_RECORD::Copy
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			08/05/2011 16:50:08
//	
//	@return				bool : 
//	@param				header : 
*/
/*-----------------------------------------------------------------*/
bool XFILECSV_RECORD::Copy(XFILECSV_RECORD* header)
{
	if(!header)								 return false;
	if(!header->GetNElements()) return false;

	for(int c=0;c<header->GetNElements();c++)
		{
			XSTRING* string = new XSTRING();
			if(string)
				{
					XCHAR* string2 =  header->GetElement(c);
					(*string) = string2;
				}
 			 else
				{
					DeleteAllElements();
					return false;
				}
		}

	return true;
}



/*-------------------------------------------------------------------
//  XFILECSV_RECORD::DeleteElement
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			06/05/2011 17:20:58
//	
//	@return				bool : 
//	@param				index : 
*/
/*-----------------------------------------------------------------*/
bool XFILECSV_RECORD::DeleteElement(int index)
{
	XSTRING* string = (XSTRING*)elements.Get(index);
	if(!string) return false;

	elements.Delete(string);

	delete string;

	return true;
}



/*-------------------------------------------------------------------
//  XFILECSV_RECORD::DeleteAllElements
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			06/05/2011 17:20:52
//	
//	@return				bool : 
//	*/
/*-----------------------------------------------------------------*/
bool XFILECSV_RECORD::DeleteAllElements()
{
	if(elements.IsEmpty())  return false;

	elements.DeleteContents();

  elements.DeleteAll();

	return true;
}



/*-------------------------------------------------------------------
//  XFILECSV_RECORD::Clean
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			06/05/2011 16:38:00
//	
//	@return				void : 
//	*/
/*-----------------------------------------------------------------*/
void XFILECSV_RECORD::Clean()
{

}





//-------------------------------------------------------------------
//  XFILECSV::XFILECSV
/**
//
//
//	@author				Abraham J. Velez
//	@version			15/07/2002 19:11:13
//
//	@return
*/
//-------------------------------------------------------------------
XFILECSV::XFILECSV() : XFILETXT()
{
	Clean();
}


//-------------------------------------------------------------------
//  XFILECSV::XFILECSV
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
XFILECSV::XFILECSV(XPATH& xpath)  : XFILETXT()
{
	Clean();	

	Open(xpath);
}



//-------------------------------------------------------------------
//  XFILECSV::XFILECSV
/**
//
//
//	@author				Abraham J. Velez
//	@version			15/07/2002 19:15:02
//
//	@return
//  @param				fileb :
*/
//-------------------------------------------------------------------
XFILECSV::XFILECSV(XFILE* file)  : XFILETXT()
{
	Clean();

	this->file = file;

}



//-------------------------------------------------------------------
//  XFILECSV::~XFILECSV
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/04/2002 9:48:21
//
//	@return
//	*/
//-------------------------------------------------------------------
XFILECSV::~XFILECSV()
{
	DeleteAllRecords();

	Close();
	
	Clean();
}

	


/*-------------------------------------------------------------------
//  XFILECSV::Open
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			06/05/2011 12:51:59
//	
//	@return				bool : 
//	@param				xpath : 
//  @param				readonly : 
//  @param				false : 
*/
/*-----------------------------------------------------------------*/
bool XFILECSV::Open(XPATH& xpath,bool readonly,bool haveheader)
{
	if(!XFILETXT::Open(xpath,readonly)) return false;

	Serialization(true,haveheader);

	return true;
}



//-------------------------------------------------------------------
//  XFILECSV::Close
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/09/2002 11:27:19
//	
//	@return 			bool : 
*/
//-------------------------------------------------------------------
bool XFILECSV::Close()
{
	bool status[2] = { false , false };
		
	status[0] = Serialization(false,HaveHeader());	
	status[1] = XFILETXT::Close();

	if(status[0] && status[1]) return true;

	return false;
}

/*-------------------------------------------------------------------
//  XFILECSV::GetSeparator
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			08/05/2011 17:11:34
//	
//	@return				XCHAR : 
//	*/
/*-----------------------------------------------------------------*/
XCHAR XFILECSV::GetSeparator()
{
	return separator;
}



/*-------------------------------------------------------------------
//  XFILECSV::SetSeparator
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			08/05/2011 17:11:37
//	
//	@return				bool : 
//	@param				separator : 
*/
/*-----------------------------------------------------------------*/
bool XFILECSV::SetSeparator(XCHAR separator)
{
	if((separator!=__C(','))&&(separator!=__C(';'))) return false;

	this->separator = separator;

	return true;
}


/*-------------------------------------------------------------------
//  XFILECSV::HaveHeader
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			08/05/2011 16:46:01
//	
//	@return				bool : 
//	*/
/*-----------------------------------------------------------------*/
bool XFILECSV::HaveHeader()
{
	if(header.GetNElements()) return true;

	return false;
}


/*-------------------------------------------------------------------
//  XFILECSV::GetHeader
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			08/05/2011 16:53:34
//	
//	@return				XFILECSV_RECORD* : 
//	*/
/*-----------------------------------------------------------------*/
XFILECSV_RECORD* XFILECSV::GetHeader()
{
	return &header;
}



/*-------------------------------------------------------------------
//  XFILECSV::SetHeader
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			08/05/2011 16:54:20
//	
//	@return				bool : 
//	@param				header : 
*/
/*-----------------------------------------------------------------*/
bool XFILECSV::SetHeader(XFILECSV_RECORD* header)
{
	if(!header) return false;

	return this->header.Copy(header);
}


/*-------------------------------------------------------------------
//  XFILECSV::GetNRecords
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			04/08/2008 04:26:21 p.m.
//	
//	@return				int : 
//	*/
/*-----------------------------------------------------------------*/
int XFILECSV::GetNRecords()
{
	return records.GetSize();
}


//-------------------------------------------------------------------
//  XFILECSV::ReadRecord
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/09/2002 11:28:07
//	
//	@return 			XFILECSV_RECORD* : 
//	@param				nrecord : 
*/
//-------------------------------------------------------------------
XFILECSV_RECORD* XFILECSV::ReadRecord(XDWORD nrecord)
{	
	if(records.IsEmpty())						return NULL;
	if(nrecord>=records.GetSize())	return NULL;

	XFILECSV_RECORD* record = (XFILECSV_RECORD*)records.Get(nrecord);
	if(!record) return NULL;
	
	return record;
}



//-------------------------------------------------------------------
//  XFILECSV::WriteRecord
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/09/2002 11:28:39
//	
//	@return 			bool : 
//	@param				nrecord : 
//  @param				record : 
*/
//-------------------------------------------------------------------
bool XFILECSV::WriteRecord(XDWORD nrecord,XFILECSV_RECORD* record)
{	
	if(records.IsEmpty())						return false;
	if(nrecord>=records.GetSize())	return false;

	XFILECSV_RECORD* _record = (XFILECSV_RECORD*)records.Get(nrecord);
	if(!_record) return false;

	delete _record;

	records.Set(nrecord,record);

	return true;
}



//-------------------------------------------------------------------
//  XFILECSV::AddRecord
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/09/2002 11:29:01
//	
//	@return 			bool : 
//	@param				record : 
*/
//-------------------------------------------------------------------
bool XFILECSV::AddRecord(XFILECSV_RECORD* record)
{	
	if(!record) return false;

	records.Add(record);

	return true;
}


//-------------------------------------------------------------------
//  XFILECSV::DeleteRecord
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/09/2002 11:29:27
//	
//	@return 			bool : 
//	@param				nrecord : 
*/
//-------------------------------------------------------------------
bool XFILECSV::DeleteRecord(XDWORD nrecord)
{	
	XFILECSV_RECORD* record = (XFILECSV_RECORD*)records.Get(nrecord);
	if(!record) return false;

	records.Delete(record);

	delete record;

	return true;
}



/*-------------------------------------------------------------------
//  XFILECSV::DeleteAllRecords
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			06/05/2011 17:57:09
//	
//	@return				bool : 
//	*/
/*-----------------------------------------------------------------*/
bool XFILECSV::DeleteAllRecords()
{
	if(records.IsEmpty())  return false;

	records.DeleteContents();
  records.DeleteAll();

	return true;
}



//-------------------------------------------------------------------
//  XFILECSV::Clean
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/09/2002 11:30:33
//	
//	@return 			void : 
//	*/
//-------------------------------------------------------------------
void XFILECSV::Clean()
{		
	separator = __C(';');
}




/*-------------------------------------------------------------------
//  XFILECSV::Serialization
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			06/05/2011 12:52:45
//	
//	@return				bool : 
//	@param				read : 
//  @param				header : 
*/
/*-----------------------------------------------------------------*/
bool XFILECSV::Serialization(bool read,bool haveheader)
{
	file->SetPosition(XFILE_SEEKINI);

	DeleteAllLines();

	if(read)
		{			
			int	c	= 0;
			
			DeleteAllRecords();

			ReadAllFile();

			if(haveheader)
				{
					XSTRING line;
					
					if(lines.GetSize()>0)
          if(GetLine(0))
            {
					    line = GetLine(0)->Get();
					    if(!line.IsEmpty()) 
						    {
							    if(CreateRecordOfLine(line,&header)) c++;
						    }
            }
				}
			
			for(;c<GetNLines();c++)				
				{	
					XFILECSV_RECORD* record;
					XSTRING			 line;
					
          if(GetLine(c))
            {
					    line = GetLine(c)->Get();
					    if(!line.IsEmpty()) 
						    {	
							    record = new XFILECSV_RECORD();
							    if(record) 
								    {
									    CreateRecordOfLine(line,record);										
									    AddRecord(record);
								    }
						    }
            }
				}
		}
	 else
	  {			
			if(!records.IsEmpty())
				{	
					XSTRING line;	
					
					if(haveheader)
						{	
							if(CreateLineOfRecord(&header,line)) AddLine(line);							
						}		

					for(int c=0;c<(int)records.GetSize();c++)						
						{
							XFILECSV_RECORD* record = (XFILECSV_RECORD*)records.Get(c);				
							if(record)
								{									
									if(CreateLineOfRecord(record,line)) AddLine(line);								
								}
						}

					WriteAllFile();

					int pos;
					file->GetPosition(pos);

					if(pos) file->SetSize(pos);
				}
		}

	DeleteAllLines();

	return true;
}



/*-------------------------------------------------------------------
//  XFILECSV::CreateRecordOfLine
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			06/05/2011 16:44:20
//	
//	@return				bool : 
//	@param				line : 
//  @param				record : 
*/
/*-----------------------------------------------------------------*/
bool XFILECSV::CreateRecordOfLine(XSTRING& line,XFILECSV_RECORD* record)
{
	if(line.IsEmpty()) return false;

	int start = 0;
	int end	  = 0;

	do{ end = line.Find(__L(";"),true,start);
			if(end == XSTRING_NOTFOUND) 
				{
					end = line.Find(__L(","),true,start);					
					if(end != XSTRING_NOTFOUND) SetSeparator(__C(','));

				} else SetSeparator(__C(';'));

			XSTRING element;

			if(end != XSTRING_NOTFOUND)
				{									
					line.Copy(start,end,element);

					record->AddElement(element);	

					start = end+1;												
				} 
			 else 
				{
					line.Copy(start,element);
					record->AddElement(element);								

					break;
				}

		} while(1);

	return true;
}



/*-------------------------------------------------------------------
//  XFILECSV::CreateLineOfRecord
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			06/05/2011 18:18:04
//	
//	@return				bool : 
//	@param				record : 
//  @param				line : 
*/
/*-----------------------------------------------------------------*/
bool XFILECSV::CreateLineOfRecord(XFILECSV_RECORD* record,XSTRING& line)
{
	line.Empty();
	
	XCHAR	separatorstring[2] = { 0, 0 };
	separatorstring[0]= separator;

	for(int c=0;c<record->GetNElements();c++)
		{
			XSTRING element;
			
			record->GetElement(c,element);
			line += element;
			if(c<(record->GetNElements()-1)) line += separatorstring;			
		}

	return true;
}


