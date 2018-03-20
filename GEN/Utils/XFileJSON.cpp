//------------------------------------------------------------------------------------------
//	XFILEJSON.CPP
//	
//	JSON File functions (JSON format)
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

#include "XDebug.h"

#include "XFileJSON.h"
	
#include "XMemory.h"


//---- GENERAL VARIABLE --------------------------------------------------------------------
	
	
//---- CLASS MEMBERS -----------------------------------------------------------------------



/*-------------------------------------------------------------------
//  XFILEJSONVALUE::XFILEJSONVALUE
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      10/10/2012 15:25:55
//  
//  @return       
//  */
/*-----------------------------------------------------------------*/
XFILEJSONVALUE::XFILEJSONVALUE()
{
  Clean();
}



/*-------------------------------------------------------------------
//  XFILEJSONVALUE::~XFILEJSONVALUE
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      10/10/2012 15:27:15
//  
//  @return       
//  */
/*-----------------------------------------------------------------*/
XFILEJSONVALUE::~XFILEJSONVALUE()
{
  Delete();

  Clean();
}



/*-------------------------------------------------------------------
//  XFILEJSONVALUE::GetType
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      10/10/2012 15:28:02
//  
//  @return       XFILEJSONVALUETYPE : 
//  */
/*-----------------------------------------------------------------*/
XFILEJSONVALUETYPE XFILEJSONVALUE::GetType()
{
  return type;
}



/*-------------------------------------------------------------------
//  XFILEJSONVALUE::GetName
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      10/10/2012 15:28:43
//  
//  @return       XSTRING* : 
//  */
/*-----------------------------------------------------------------*/
XSTRING* XFILEJSONVALUE::GetName()
{
  return name;
}



/*-------------------------------------------------------------------
//  XFILEJSONVALUE::GetValue
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      10/10/2012 15:29:12
//  
//  @return       void* : 
//  */
/*-----------------------------------------------------------------*/
void* XFILEJSONVALUE::GetValuePointer()
{
  return value.pointer;
}



/*-------------------------------------------------------------------
//  XFILEJSONVALUE::GetValueInteger
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      11/10/2012 16:15:42
//  
//  @return       int : 
//  */
/*-----------------------------------------------------------------*/
int XFILEJSONVALUE::GetValueInteger()
{
  return value.integer;
}



/*-------------------------------------------------------------------
//  XFILEJSONVALUE::GetValueFloating
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      11/10/2012 16:16:28
//  
//  @return       float : 
//  */
/*-----------------------------------------------------------------*/
float XFILEJSONVALUE::GetValueFloating()
{
  return value.floating;
}



/*-------------------------------------------------------------------
//  XFILEJSONVALUE::GetValueBoolean
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      11/10/2012 16:16:35
//  
//  @return       bool : 
//  */
/*-----------------------------------------------------------------*/
bool XFILEJSONVALUE::GetValueBoolean()
{
  return value.boolean;
}




/*-------------------------------------------------------------------
//  XFILEJSONVALUE::SetName
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      10/10/2012 15:30:13
//  
//  @return       bool : 
//  @param        name : 
*/
/*-----------------------------------------------------------------*/
bool XFILEJSONVALUE::SetName(XSTRING& name)
{
  XSTRING* _name = new XSTRING(name);
  if(!_name) return false;

  this->name = _name;

  return true;
}



/*-------------------------------------------------------------------
//  XFILEJSONVALUE::SetName
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      10/10/2012 15:30:47
//  
//  @return       bool : 
//  @param        name : 
*/
/*-----------------------------------------------------------------*/
bool XFILEJSONVALUE::SetName(XCHAR* name)
{  
  XSTRING* _name = new XSTRING(name);
  if(!_name) return false;

  this->name = _name;

  return true;
}

    

/*-------------------------------------------------------------------
//  XFILEJSONVALUE::Set
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      10/10/2012 15:31:21
//  
//  @return       bool : 
//  @param        number : 
*/
/*-----------------------------------------------------------------*/
bool XFILEJSONVALUE::Set(int number)
{
  type =  XFILEJSONVALUETYPE_NUMBER;

  value.integer = number;

  return true;
}




/*-------------------------------------------------------------------
//  XFILEJSONVALUE::Set
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      10/10/2012 15:33:00
//  
//  @return       bool : 
//  @param        number : 
*/
/*-----------------------------------------------------------------*/
bool XFILEJSONVALUE::Set(float number)
{
  //if(sizeof(float) != sizeof(void*)) return false;

  type =  XFILEJSONVALUETYPE_NUMBERSPECIAL;
  
  value.floating = number;

  return true;
}



/*-------------------------------------------------------------------
//  XFILEJSONVALUE::Set
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      10/10/2012 15:35:14
//  
//  @return       bool : 
//  @param        string : 
*/
/*-----------------------------------------------------------------*/
bool XFILEJSONVALUE::Set(XSTRING& string)
{
  return Set(string.Get());
}



/*-------------------------------------------------------------------
//  XFILEJSONVALUE::Set
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      10/10/2012 15:36:12
//  
//  @return       bool : 
//  @param        string : 
*/
/*-----------------------------------------------------------------*/
bool XFILEJSONVALUE::Set(XCHAR* string)
{
  XSTRING* _string = new XSTRING(string);
  if(!_string) return false;

  type =  XFILEJSONVALUETYPE_STRING;

  value.pointer = (void*)_string;

  return true;
}



/*-------------------------------------------------------------------
//  XFILEJSONVALUE::Set
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      10/10/2012 15:39:09
//  
//  @return       bool : 
//  @param        object : 
*/
/*-----------------------------------------------------------------*/
bool XFILEJSONVALUE::Set(XFILEJSONOBJECT* object)
{  
  if(!object) return false;

  type = XFILEJSONVALUETYPE_OBJECT;
  
  value.pointer = (void*)object;  
  
  return true;
}


/*-------------------------------------------------------------------
//  XFILEJSONVALUE::Set
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      10/10/2012 15:39:57
//  
//  @return       bool : 
//  @param        array : 
*/
/*-----------------------------------------------------------------*/
bool XFILEJSONVALUE::Set(XFILEJSONARRAY* array)
{
  if(!array) return false;

  type = XFILEJSONVALUETYPE_ARRAY;
  
  value.pointer = (void*)array;  
  
  return true;
}



/*-------------------------------------------------------------------
//  XFILEJSONVALUE::Set
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      10/10/2012 15:40:57
//  
//  @return       bool : 
//  @param        boolean : 
*/
/*-----------------------------------------------------------------*/
bool XFILEJSONVALUE::Set(bool boolean)
{
  type = XFILEJSONVALUETYPE_BOOLEAN;
  
  value.boolean = boolean;

  return true;
}


/*-------------------------------------------------------------------
//  XFILEJSONVALUE::Set
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      10/10/2012 15:41:52
//  
//  @return       bool : 
//  @param        void : 
*/
/*-----------------------------------------------------------------*/
bool XFILEJSONVALUE::Set(void)
{
  type = XFILEJSONVALUETYPE_NULL;

  value.pointer = (void*)NULL;

  return true;
}


/*-------------------------------------------------------------------
//  XFILEJSONVALUE::Delete
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      10/10/2012 15:49:52
//  
//  @return       bool : 
//  */
/*-----------------------------------------------------------------*/
bool XFILEJSONVALUE::Delete()
{ 
  switch(type)
    {
      case XFILEJSONVALUETYPE_UNKNOWN        : return false;    

      case XFILEJSONVALUETYPE_STRING        : { XSTRING* string = (XSTRING*)value.pointer;
                                                delete string;
                                                value.pointer = NULL;
                                              }
                                              break;
      
      case XFILEJSONVALUETYPE_OBJECT        : { XFILEJSONOBJECT* object = (XFILEJSONOBJECT*)value.pointer;
                                                delete object;
                                                value.pointer = NULL;
                                              }
                                              break;  

      case XFILEJSONVALUETYPE_ARRAY         : { XFILEJSONARRAY* array = (XFILEJSONARRAY*)value.pointer;
                                                delete array;
                                                value.pointer = NULL;
                                              }
                                              break;

      case XFILEJSONVALUETYPE_NUMBER        : value.integer = 0;     break;
      case XFILEJSONVALUETYPE_NUMBERSPECIAL : value.integer = 0;     break;
      case XFILEJSONVALUETYPE_BOOLEAN       : value.boolean = false; break;
      case XFILEJSONVALUETYPE_NULL          : value.pointer = NULL;  break;
    }

  if(name) 
    {
      delete name;
      name = NULL;
    } 

  return true;
}




/*-------------------------------------------------------------------
//  XFILEJSONVALUE::Clean
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      10/10/2012 15:42:21
//  
//  @return       void : 
//  */
/*-----------------------------------------------------------------*/
void XFILEJSONVALUE::Clean()
{
  type           = XFILEJSONVALUETYPE_UNKNOWN;
   
  name           = NULL;

  value.pointer  = NULL; 
  value.integer  = 0;
  value.floating = 0L;
  value.boolean  = false;
}







/*-------------------------------------------------------------------
//  XFILEJSONOBJECT::XFILEJSONOBJECT
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      10/10/2012 12:56:49
//  
//  @return       
//  */
/*-----------------------------------------------------------------*/
XFILEJSONOBJECT::XFILEJSONOBJECT()
{
  Clean();
}
	


/*-------------------------------------------------------------------
//  XFILEJSONOBJECT::~XFILEJSONOBJECT
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      10/10/2012 12:56:56
//  
//  @return       
//  */
/*-----------------------------------------------------------------*/
XFILEJSONOBJECT::~XFILEJSONOBJECT()
{
  DeleteAllValues();

  Clean();
}



/*-------------------------------------------------------------------
//  XFILEJSONOBJECT::Add
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      15/10/2012 10:26:42
//  
//  @return       bool : 
//  @param        value : 
*/
/*-----------------------------------------------------------------*/
bool XFILEJSONOBJECT::Add(XFILEJSONVALUE* value)
{
  if(!value) return false;

  values.Add(value);

  return true;
}



/*-------------------------------------------------------------------
//  XFILEJSONOBJECT::Add
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      15/10/2012 10:54:56
//  
//  @return       bool : 
//  @param        name : 
//  @param        array : 
*/
/*-----------------------------------------------------------------*/
bool XFILEJSONOBJECT::Add(XCHAR* name,XFILEJSONARRAY* array)
{
  if(!array) return false;

  XFILEJSONVALUE* value = new XFILEJSONVALUE();
  if(!value) return false;

  value->SetName(name);
  value->Set(array);

  values.Add(value);

  return true;
}




/*-------------------------------------------------------------------
//  XFILEJSONOBJECT::Add
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      15/10/2012 10:55:52
//  
//  @return       bool : 
//  @param        name : 
//  @param        array : 
*/
/*-----------------------------------------------------------------*/
bool XFILEJSONOBJECT::Add(XSTRING& name,XFILEJSONARRAY* array)
{
  return Add(name.Get(),array);    
}




/*-------------------------------------------------------------------
//  XFILEJSONOBJECT::Add
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      15/10/2012 10:35:57
//  
//  @return       bool : 
//  @param        object : 
*/
/*-----------------------------------------------------------------*/
bool XFILEJSONOBJECT::Add(XCHAR* name,XFILEJSONOBJECT* object)
{
  if(!object) return false;

  XFILEJSONVALUE* value = new XFILEJSONVALUE();
  if(!value) return false;

  value->SetName(name);
  value->Set(object);

  values.Add(value);

  return true;
}



/*-------------------------------------------------------------------
//  XFILEJSONOBJECT::Add
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      15/10/2012 10:59:49
//  
//  @return       bool : 
//  @param        name : 
//  @param        object : 
*/
/*-----------------------------------------------------------------*/
bool XFILEJSONOBJECT::Add(XSTRING& name,XFILEJSONOBJECT* object)
{
  return Add(name.Get(),object); 
}




/*-------------------------------------------------------------------
//  XFILEJSONOBJECT::GetValues
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      10/10/2012 16:50:23
//  
//  @return       VECTOR* : 
//  */
/*-----------------------------------------------------------------*/
XVECTOR<XFILEJSONVALUE*>* XFILEJSONOBJECT::GetValues()
{
  return &values;
}
 


/*-------------------------------------------------------------------
//  XFILEJSONOBJECT::DeleteAllValues
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      10/10/2012 16:25:56
//  
//  @return       bool : 
//  */
/*-----------------------------------------------------------------*/
bool XFILEJSONOBJECT::DeleteAllValues()
{
  if(values.IsEmpty())  return false;
	
	values.DeleteContents();
  values.DeleteAll();

  return true;
}



/*-------------------------------------------------------------------
//  XFILEJSONOBJECT::Clean
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      10/10/2012 12:57:17
//  
//  @return       void : 
//  */
/*-----------------------------------------------------------------*/
void XFILEJSONOBJECT::Clean()
{

}






/*-------------------------------------------------------------------
//  XFILEJSONARRAY::XFILEJSONARRAY
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      10/10/2012 16:31:14
//  
//  @return      
//  */
/*-----------------------------------------------------------------*/
XFILEJSONARRAY::XFILEJSONARRAY() :  XFILEJSONOBJECT()
{
  Clean();
}
		



/*-------------------------------------------------------------------
//  XFILEJSONARRAY::~XFILEJSONARRAY
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      10/10/2012 16:31:31
//  
//  @return       
//  */
/*-----------------------------------------------------------------*/
XFILEJSONARRAY::~XFILEJSONARRAY()
{
	DeleteAllValues();

  Clean();
}



/*-------------------------------------------------------------------
//  XFILEJSONARRAY::GetName
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      10/10/2012 15:28:43
//  
//  @return       XSTRING* : 
//  */
/*-----------------------------------------------------------------*/
XSTRING* XFILEJSONARRAY::GetName()
{
  return &name;
}



/*-------------------------------------------------------------------
//  XFILEJSONARRAY::SetName
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      10/10/2012 15:30:13
//  
//  @return       bool : 
//  @param        name : 
*/
/*-----------------------------------------------------------------*/
bool XFILEJSONARRAY::SetName(XSTRING& name)
{
  this->name = name.Get();

  return true;
}



/*-------------------------------------------------------------------
//  XFILEJSONARRAY::SetName
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      10/10/2012 15:30:47
//  
//  @return       bool : 
//  @param        name : 
*/
/*-----------------------------------------------------------------*/
bool XFILEJSONARRAY::SetName(XCHAR* name)
{    
  this->name = name;

  return true;
}



/*-------------------------------------------------------------------
//  XFILEJSONARRAY::Clean
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      10/10/2012 16:32:08
//  
//  @return       void : 
//  */
/*-----------------------------------------------------------------*/
void XFILEJSONARRAY::Clean()
{
  
};






//-------------------------------------------------------------------
//  XFILEJSON::XFILEJSON
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
XFILEJSON::XFILEJSON() : XFILETXT()
{
	Clean();
}



//-------------------------------------------------------------------
//  XFILEJSON::~XFILEJSON
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/04/2002 9:48:21
//
//	@return
//	*/
//-------------------------------------------------------------------
XFILEJSON::~XFILEJSON()
{
	DeleteAllLines();

  DeleteAllObjects();

	Clean();
}



/*-------------------------------------------------------------------
//  XFILEJSON::GetRoot
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      11/10/2012 12:17:07
//  
//  @return       XFILEJSONOBJECT* : 
//  */
/*-----------------------------------------------------------------*/
XFILEJSONOBJECT* XFILEJSON::GetRoot()
{
  return root;
}



/*-------------------------------------------------------------------
//  XFILEJSON::SetRoot
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      15/10/2012 8:32:31
//  
//  @return       bool : 
//  @param        root : 
*/
/*-----------------------------------------------------------------*/
bool XFILEJSON::SetRoot(XFILEJSONOBJECT* root)
{
  DeleteAllObjects();

  this->root = root;

  return true;
}



/*-------------------------------------------------------------------
//  XFILEJSON::DecodeAllLines
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			11/09/2012 23:00:58
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool XFILEJSON::DecodeAllLines()
{ 
  if(root) delete root;

  if(!GetAllInOneLine(all)) return false;

  DeleteAllLines();
 
  //--------------------------------------------------------
  // Search Object Root

  int position = 0;
  if(!SearchControlCharacter(XFILEJSONCONTROLCHAR_OPENBRACE,position)) return false;
  
  position++;
  
  root = new XFILEJSONOBJECT();
  if(!root) return false;

  bool status = DecodeObject(position, true, root);

  all.Empty();

  return status;
}





/*-------------------------------------------------------------------
//  XFILEJSON::EncodeAllLines
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      15/10/2012 8:43:27
//  
//  @return       bool : 
//  @param        istabulatedline : 
*/
/*-----------------------------------------------------------------*/
bool XFILEJSON::EncodeAllLines(bool istabulatedline)
{
  if(!root) return false;

  encodelevel  = 0;
  
  return EncodeObject(true,root,istabulatedline);
}



/*-------------------------------------------------------------------
//  XFILEJSON::ReadAndDecodeAllLines
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			07/02/2011 11:18:26
//	
//	@return				bool : 
//	*/
/*-----------------------------------------------------------------*/
bool XFILEJSON::ReadAndDecodeAllLines()
{
	if(!IsOpen())			 return false;	

	if(!ReadAllFile()) return false;

	return DecodeAllLines();
}



/*-------------------------------------------------------------------
//  XFILEJSON::WriteAndEncodeAllLines
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      15/10/2012 8:43:58
//  
//  @return       bool : 
//  @param        istabulatedline : 
//  @param        formatchar : 
//  @param        typeLF : 
*/
/*-----------------------------------------------------------------*/
bool XFILEJSON::WriteAndEncodeAllLines(bool istabulatedline,XFILETXTFORMATCHAR formatchar, XFILETXTTYPELF	typeLF)
{
	SetFormatChar(formatchar);
	SetTypeLF(typeLF);

	return WriteAndEncodeAllLines(istabulatedline);
}




/*-------------------------------------------------------------------
//  XFILEJSON::WriteAndEncodeAllLines
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      15/10/2012 8:44:40
//  
//  @return       bool : 
//  @param        istabulatedline : 
*/
/*-----------------------------------------------------------------*/
bool XFILEJSON::WriteAndEncodeAllLines(bool istabulatedline)
{
	if(!IsOpen())	return false;	

	file->SetPosition(0);

	EncodeAllLines(istabulatedline);
	
	bool status = WriteAllFile();

	return status;
}




/*-------------------------------------------------------------------
//  XFILEJSON::DeleteAllObjects
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      15/10/2012 8:34:15
//  
//  @return       bool : 
//  */
/*-----------------------------------------------------------------*/
bool XFILEJSON::DeleteAllObjects()
{
  if(!root) return false;
    
  delete root;

  root = NULL;

  return true;
}




/*-------------------------------------------------------------------
//  XFILEJSON::GetValue
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			01/04/2013 12:33:27
//	
//	@return 			XFILEJSONVALUE* : 
//	@param				name : 
//  @param				startobject : 
*/
/*-----------------------------------------------------------------*/
XFILEJSONVALUE* XFILEJSON::GetValue(XCHAR* name, XFILEJSONOBJECT* startobject)
{
	XFILEJSONOBJECT* object = startobject?startobject:GetRoot();
	if(!object) return NULL;	

	if(!object->GetValues()) return NULL;
		
	for(int c=0; c<(int)object->GetValues()->GetSize();c++)
		{
			XFILEJSONVALUE* value = (XFILEJSONVALUE*)object->GetValues()->Get(c);
			if(value)
				{
					XFILEJSONVALUETYPE type = value->GetType();
					if((type == XFILEJSONVALUETYPE_OBJECT) || (type ==  XFILEJSONVALUETYPE_ARRAY))
						{
							value = GetValue(name, (XFILEJSONOBJECT*)value->GetValuePointer());
							if(value) return value;
						}					
					 else
						{
							if(value->GetName())
								{
									if(!value->GetName()->Compare(name)) return value;
								}
						}
				}			
		}

	return NULL;
}




/*-------------------------------------------------------------------
//  XFILEJSON::GetValue
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			01/04/2013 12:33:21
//	
//	@return 			XFILEJSONVALUE* : 
//	@param				name : 
//  @param				startobject : 
*/
/*-----------------------------------------------------------------*/
XFILEJSONVALUE* XFILEJSON::GetValue(XSTRING& name, XFILEJSONOBJECT* startobject)
{
	return GetValue(name.Get(),startobject);
}




//-------------------------------------------------------------------
//  XFILEJSON::Clean
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			04/09/2001 12:46:19
//	
//	@return 			void : 
//	*/
//-------------------------------------------------------------------
void XFILEJSON::Clean()
{
	 root         = NULL;
   encodelevel  = 0;
}





/*-------------------------------------------------------------------
//  XFILEJSON::DecodeObject
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      11/10/2012 16:48:46
//  
//  @return       bool : 
//  @param        position : 
//  @param        isobject : 
//  @param        object : 
*/
/*-----------------------------------------------------------------*/
bool XFILEJSON::DecodeObject(int& position, bool isobject, XFILEJSONOBJECT* object)
{
  if(!object) return false;

  while(1)
    {
      XFILEJSONCONTROLCHAR ctrlchar;  
      
      XFILEJSONVALUE* value = new XFILEJSONVALUE();
      if(!value) 
				{
					return false; 
				}

      if(isobject)
        {
          XSTRING name;
  
          if(!SearchControlCharacter(XFILEJSONCONTROLCHAR_QUOTE,position)) 
						{
							return false;
						}	

			
          if(!GetString(position,name)) 
						{
							return false;
						}
  
          value->SetName(name);

          ctrlchar = SearchNextControlCharacter(position);
          if(ctrlchar!=XFILEJSONCONTROLCHAR_COLON) 
						{
							return false;
						}

          position++;
        }

      ctrlchar = SearchNextControlCharacter(position);
      switch(ctrlchar)
        {
          case XFILEJSONCONTROLCHAR_OPENBRACE   : { XFILEJSONOBJECT* newobject = new XFILEJSONOBJECT();
                                                    if(!object) return false;

                                                    value->Set(newobject);

                                                    position++;

                                                    if(!DecodeObject(position, true, newobject)) 
																											{
																												return false;
																											}

                                                    object->GetValues()->Add(value);
                                                  }
                                                  break;

          case XFILEJSONCONTROLCHAR_OPENBRACKET : { XFILEJSONARRAY* newarray = new XFILEJSONARRAY();
                                                    if(!object) return false;

                                                    value->Set(newarray);

                                                    position++;

                                                    if(!DecodeObject(position,false,(XFILEJSONOBJECT*)newarray)) 
																											{
																												return false;
																											}

                                                    object->GetValues()->Add(value);
                                                  }           
                                                  break;

          case XFILEJSONCONTROLCHAR_NUMBER      : { XSTRING valuestring;
                                                    bool    special = false;
                                                                                                     
																										GetNumber(position,valuestring,special);
                                             
                                                    if(!special)
                                                      {
                                                        int integer = 0;

                                                        valuestring.UnFormat(__L("%d"),&integer);
                                                        value->Set(integer);
                                                      }
                                                     else
                                                      {
                                                        double floating = 0.0f;

                                                        valuestring.UnFormat(__L("%f"), &floating);
                                                        
                                                        value->Set((float)floating);
                                                      }
                                                    
                                                    object->GetValues()->Add(value);                                                    
                                                  }
                                                  break;

          case XFILEJSONCONTROLCHAR_TEXT        : { XSTRING valuestring;
                                                   
																										GetText(position,valuestring);

                                                    valuestring.ToUpperCase();

																										value->Set();

                                                    if(!valuestring.Compare(__L("TRUE")))  value->Set(true);                                                      
                                                    if(!valuestring.Compare(__L("FALSE"))) value->Set(false);
                                                    if(!valuestring.Compare(__L("NULL")))  value->Set();

                                                    //if(value->GetType()==XFILEJSONVALUETYPE_UNKNOWN) return false;

                                                    object->GetValues()->Add(value);
                                                  }
                                                  break;

          case XFILEJSONCONTROLCHAR_QUOTE       : { XSTRING valuestring;
                                                   
																										GetString(position,valuestring);
                                             
                                                    value->Set(valuestring);
                                                    
                                                    object->GetValues()->Add(value);                                                   
                                                  }
                                                  break;

      
        }

      
      ctrlchar = SearchNextControlCharacter(position);
      if(isobject)
        {
          if(ctrlchar==XFILEJSONCONTROLCHAR_CLOSEBRACE)  
            {
              position++;
              break;
            }
        }
       else
        {
          if(ctrlchar==XFILEJSONCONTROLCHAR_CLOSEBRACKET)  
            {
              position++;
              break;
            }
        }
      
      ctrlchar = SearchNextControlCharacter(position);
      if(ctrlchar!=XFILEJSONCONTROLCHAR_COMMA)  
        { 
          return false;
        }
       else
        {
          //-----------------------------------
          // COMMA is ADD to end (BAD JSON format)

          position++;

          ctrlchar = SearchNextControlCharacter(position);
          if(isobject)
            {
              if(ctrlchar==XFILEJSONCONTROLCHAR_CLOSEBRACE)  
                {
                  position++;
                  break;
                }
            }
           else
            {
              if(ctrlchar==XFILEJSONCONTROLCHAR_CLOSEBRACKET)  
                {
                  position++;
                  break;
                }
            }
        }         
    }

	//XDEBUG_PRINTCOLOR(2, __L("%d"), position);
  
  return true;
}



/*-------------------------------------------------------------------
//  XFILEJSON::CheckControlCharacter
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      11/10/2012 8:51:43
//  
//  @return       XFILEJSONCONTROLCHAR : 
//  @param        character : 
*/
/*-----------------------------------------------------------------*/
XFILEJSONCONTROLCHAR XFILEJSON::CheckControlCharacter(XCHAR character)
{
  XFILEJSONCONTROLCHAR ctrlchar;

  switch(character)
    {
      case __C('{')  : ctrlchar = XFILEJSONCONTROLCHAR_OPENBRACE;     break;
      case __C('}')  : ctrlchar = XFILEJSONCONTROLCHAR_CLOSEBRACE;    break;
      case __C('[')  : ctrlchar = XFILEJSONCONTROLCHAR_OPENBRACKET;   break;
      case __C(']')  : ctrlchar = XFILEJSONCONTROLCHAR_CLOSEBRACKET;  break;
      case __C('\"') : ctrlchar = XFILEJSONCONTROLCHAR_QUOTE;         break;  
      case __C(',')  : ctrlchar = XFILEJSONCONTROLCHAR_COMMA;         break;
      case __C(':')  : ctrlchar = XFILEJSONCONTROLCHAR_COLON;         break;
      case __C('\\') : ctrlchar = XFILEJSONCONTROLCHAR_BLACKSLASH;    break;

      case __C('0')  : 
      case __C('1')  : 
      case __C('2')  : 
      case __C('3')  : 
      case __C('4')  : 
      case __C('5')  : 
      case __C('6')  : 
      case __C('7')  : 
      case __C('8')  : 
      case __C('9')  : 
      case __C('-')  : ctrlchar = XFILEJSONCONTROLCHAR_NUMBER;        break;
 
      case __C('.')  :
      case __C('+')  :       
      case __C('e')  : 
      case __C('E')  : ctrlchar = XFILEJSONCONTROLCHAR_NUMBERSPECIAL; break;

      case __C('\t') :  
      case __C('\r') :
      case __C('\n') :
      case __C(' ')  : ctrlchar = XFILEJSONCONTROLCHAR_NOTCONTROL;    break;

           default   : ctrlchar = XFILEJSONCONTROLCHAR_TEXT;          break;
    }

  return ctrlchar;
} 



/*-------------------------------------------------------------------
//  XFILEJSON::GetControlCharacter
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      11/10/2012 15:20:11
//  
//  @return       XCHAR : 
//  @param        ctrlchar : 
*/
/*-----------------------------------------------------------------*/
XCHAR XFILEJSON::GetControlCharacter(XFILEJSONCONTROLCHAR ctrlchar)
{
  switch(ctrlchar)
    {
      case XFILEJSONCONTROLCHAR_OPENBRACE     : return __C('{');
      case XFILEJSONCONTROLCHAR_CLOSEBRACE    : return __C('}');
      case XFILEJSONCONTROLCHAR_OPENBRACKET   : return __C('[');
      case XFILEJSONCONTROLCHAR_CLOSEBRACKET  : return __C(']');
      case XFILEJSONCONTROLCHAR_QUOTE         : return __C('\"');
      case XFILEJSONCONTROLCHAR_COMMA         : return __C(',');
      case XFILEJSONCONTROLCHAR_COLON         : return __C(':');
      case XFILEJSONCONTROLCHAR_BLACKSLASH    : return __C('\\');
    }

  return 0;
}


/*-------------------------------------------------------------------
//  XFILEJSON::AddControlCharacter
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      11/10/2012 12:52:52
//  
//  @return       bool : 
//  @param        ctrlchar : 
//  @param        line : 
*/
/*-----------------------------------------------------------------*/
bool XFILEJSON::AddControlCharacter(XFILEJSONCONTROLCHAR ctrlchar, XSTRING& line)
{
  XCHAR character = GetControlCharacter(ctrlchar);
  if(character)
    {
      line += character;
      return true;
    }
  
  return false;
} 



/*-------------------------------------------------------------------
//  XFILEJSON::SearchNextControlCharacter
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      11/10/2012 8:57:35
//  
//  @return       XFILEJSONCONTROLCHAR : 
//  @param        position : 
*/
/*-----------------------------------------------------------------*/
XFILEJSONCONTROLCHAR XFILEJSON::SearchNextControlCharacter(int& position)
{ 
  for(int c=position;c<(int)all.GetSize();c++)
    {
      XFILEJSONCONTROLCHAR ctrlchar;

      ctrlchar = CheckControlCharacter(all.Get()[c]);
      if(ctrlchar != XFILEJSONCONTROLCHAR_NOTCONTROL)         
        {
          position = c;
          return ctrlchar;
        }
    }

  position = XSTRING_NOTFOUND;

  return XFILEJSONCONTROLCHAR_NOTCONTROL;      
} 




/*-------------------------------------------------------------------
//  XFILEJSON::SearchControlCharacter
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      11/10/2012 9:01:18
//  
//  @return       bool : 
//  @param        ctrlchar : 
//  @param        position : 
*/
/*-----------------------------------------------------------------*/
bool XFILEJSON::SearchControlCharacter(XFILEJSONCONTROLCHAR ctrlchar,int& position)
{ 
  for(int c=position;c<(int)all.GetSize();c++)
    {
      XFILEJSONCONTROLCHAR _ctrlchar;

      _ctrlchar = CheckControlCharacter(all.Get()[c]);
      if(_ctrlchar == ctrlchar)         
        {
          position = c;
          return true;
        }
    }

  position = XSTRING_NOTFOUND;
  return false;      
}




/*-------------------------------------------------------------------
//  XFILEJSON::GetNumber
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      11/10/2012 11:35:14
//  
//  @return       bool : 
//  @param        position : 
//  @param        string : 
//  @param        special : 
*/
/*-----------------------------------------------------------------*/
bool XFILEJSON::GetNumber(int& position,XSTRING& string,bool& special)
{
  string.Empty();

  special = false;

  for(int c=position;c<(int)all.GetSize();c++)
    {
      XFILEJSONCONTROLCHAR ctrlchar;

      ctrlchar = CheckControlCharacter(all.Get()[c]);
      if((ctrlchar == XFILEJSONCONTROLCHAR_NUMBER)||(ctrlchar == XFILEJSONCONTROLCHAR_NUMBERSPECIAL))
        {
          string += all.Get()[c];
          if(ctrlchar == XFILEJSONCONTROLCHAR_NUMBERSPECIAL) special = true;

        } 
       else 
        { 
          position = c;   
          break;
        }
    }

  return string.IsEmpty()?false:true;
}



/*-------------------------------------------------------------------
//  XFILEJSON::GetText
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      17/10/2012 12:04:40
//  
//  @return       bool : 
//  @param        position : 
//  @param        string : 
*/
/*-----------------------------------------------------------------*/
bool XFILEJSON::GetText(int& position,XSTRING& string)
{
  string.Empty();
  
  for(int c=position;c<(int)all.GetSize();c++)
    {
      XFILEJSONCONTROLCHAR ctrlchar;

      ctrlchar = CheckControlCharacter(all.Get()[c]);
      if((ctrlchar == XFILEJSONCONTROLCHAR_TEXT) ||(ctrlchar == XFILEJSONCONTROLCHAR_NUMBERSPECIAL))
        {
          string += all.Get()[c];       
        } 
       else 
        { 
          position = c;   
          break;
        }
    }

  return string.IsEmpty()?false:true;
}




/*-------------------------------------------------------------------
//  XFILEJSON::GetString
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      11/10/2012 9:14:57
//  
//  @return       bool : 
//  @param        position : 
//  @param        string : 
*/
/*-----------------------------------------------------------------*/
bool XFILEJSON::GetString(int& position,XSTRING& string)
{
  string.Empty();

  XFILEJSONCONTROLCHAR ctrlchar;

  //----------------------------------
  // First Quote
  ctrlchar = CheckControlCharacter(all.Get()[position]);
  if(ctrlchar != XFILEJSONCONTROLCHAR_QUOTE) return false;

  position++;

  bool slashcontrol = false;
  bool exit         = false;

  for(int c=position;c<(int)all.GetSize();c++)
    {
      ctrlchar = CheckControlCharacter(all.Get()[c]);  

      switch(ctrlchar)
        {
          case XFILEJSONCONTROLCHAR_NOTCONTROL    :
          case XFILEJSONCONTROLCHAR_OPENBRACE     :
          case XFILEJSONCONTROLCHAR_CLOSEBRACE    :
          case XFILEJSONCONTROLCHAR_OPENBRACKET   :
          case XFILEJSONCONTROLCHAR_CLOSEBRACKET  :
          case XFILEJSONCONTROLCHAR_COMMA         :       
          case XFILEJSONCONTROLCHAR_COLON         : 
          case XFILEJSONCONTROLCHAR_NUMBER        : 
          case XFILEJSONCONTROLCHAR_NUMBERSPECIAL : 
          case XFILEJSONCONTROLCHAR_TEXT          : string += all.Get()[c];
                                                    slashcontrol = false;
                                                    break;    

          case XFILEJSONCONTROLCHAR_QUOTE         : if(slashcontrol) 
                                                      {
                                                        string += all.Get()[c]; 
                                                        slashcontrol = false;

                                                      } 
                                                     else exit = true;                                                                                                             
                                                    break;

          case XFILEJSONCONTROLCHAR_BLACKSLASH    : slashcontrol = true; 
                                                    string += all.Get()[c];
                                                    break;
        }

      if(exit) 
        {
          position = (c + 1);
          break;
        }
    }

  return string.IsEmpty()?false:true;
}



/*-------------------------------------------------------------------
//  XFILEXML::AddTabs
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			14/02/2011 22:44:23
//	
//	@return				bool : 
//	@param				level : 
//  @param				spacetabs : 
//  @param				string : 
*/
/*-----------------------------------------------------------------*/
bool XFILEJSON::AddTabs(int level,int spacetabs,XSTRING& string)
{
	if(spacetabs<=0) return false;

	for(int c=0;c<level;c++)
		{
			for(int d=0;d<spacetabs;d++)
				{
					string += __L(" ");
				}
		}

	return true;
}



/*-------------------------------------------------------------------
//  XFILEJSON::EncodeObject
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      11/10/2012 16:54:15
//  
//  @return       bool : 
//  @param        isobject : 
//  @param        object : 
//  @param        istabulatedline : 
*/
/*-----------------------------------------------------------------*/
bool XFILEJSON::EncodeObject(bool isobject, XFILEJSONOBJECT* object,bool istabulatedline)
{
  if(!object) return false;

  XSTRING line;

  if(istabulatedline)
    {
      AddTabs(encodelevel,XFILEJSON_SPACETABS,line);        
      encodelevel++;
    }
 
  AddControlCharacter(isobject?XFILEJSONCONTROLCHAR_OPENBRACE:XFILEJSONCONTROLCHAR_OPENBRACKET, line);
  AddLine(line);
  
  //XDEBUG_PRINTCOLOR(0,line.Get());
  
  for(int c=0;c<(int)object->GetValues()->GetSize();c++)
    {
      XFILEJSONVALUE* value = (XFILEJSONVALUE*)(object->GetValues()->Get(c));
      if(value)
        {
          if(isobject)
            {
              XSTRING* name = value->GetName();
              if(name)
                {                 
                  line.Empty();
                  AddTabs(encodelevel,XFILEJSON_SPACETABS,line);
                  
                  XSTRING line2;

                  line2.Format(__L("%c%s%c : "), GetControlCharacter(XFILEJSONCONTROLCHAR_QUOTE), name->Get() , GetControlCharacter(XFILEJSONCONTROLCHAR_QUOTE));                                       

                  line+=line2;
                }

            } 
           else 
            { 
              line.Empty();
              AddTabs(encodelevel,XFILEJSON_SPACETABS,line);
            }

          switch(value->GetType())
            {
              case XFILEJSONVALUETYPE_UNKNOWN        : break;

              case XFILEJSONVALUETYPE_NUMBER        : { XSTRING  valuestring;

                                                        valuestring.Format(__L("%d"),value->GetValueInteger());
                                                        line+=valuestring.Get();   
                                                      }
                                                      break; 

              case XFILEJSONVALUETYPE_NUMBERSPECIAL : { XSTRING  valuestring;

                                                        valuestring.Format(__L("%f"),value->GetValueFloating());
                                                        line+=valuestring.Get();   
                                                      }
                                                      break; 

              case XFILEJSONVALUETYPE_STRING        : { XSTRING* ptrvaluestring = (XSTRING*)(value->GetValuePointer());
                                                        XSTRING  valuestring;

                                                        if(!ptrvaluestring) return false;
                                                    
                                                        valuestring.Format(__L("%c%s%c"), GetControlCharacter(XFILEJSONCONTROLCHAR_QUOTE), ptrvaluestring->Get(), GetControlCharacter(XFILEJSONCONTROLCHAR_QUOTE));                                                                       
                                                        line+=valuestring.Get();                                                   
                                                      }                                                
                                                      break;

              case XFILEJSONVALUETYPE_OBJECT        : { XFILEJSONOBJECT* newobject = (XFILEJSONOBJECT*)(value->GetValuePointer());
                                                        if(newobject) 
                                                          {                                                            
                                                            if((!line.IsEmpty()) && line.HaveCharacters())
                                                              {
                                                                AddLine(line); 
                                                                //XDEBUG_PRINTCOLOR(0,line.Get());
                                                              }
                                                            
                                                            if(!EncodeObject(true,newobject,istabulatedline)) return false;
                                                            line.Empty();
                                                          }
                                                      }
                                                      break;

              case XFILEJSONVALUETYPE_ARRAY         : { XFILEJSONARRAY* newarray = (XFILEJSONARRAY*)(value->GetValuePointer());
                                                        if(newarray) 
                                                          {                                                            
                                                            if((!line.IsEmpty()) && line.HaveCharacters())
                                                              {
                                                                AddLine(line); 
                                                                //XDEBUG_PRINTCOLOR(0,line.Get());
                                                              }
                                                            
                                                            if(!EncodeObject(false,(XFILEJSONOBJECT*)newarray,istabulatedline)) return false;
                                                            line.Empty();
                                                          }
                                                      }
                                                      break;

              case XFILEJSONVALUETYPE_BOOLEAN       : line += (value->GetValueBoolean()?__L("true"):__L("false"));                                                      
                                                      break;

              case XFILEJSONVALUETYPE_NULL          : line += __L("NULL");                                                      
                                                      break;
            }
        }
            
      if(c!=(int)(object->GetValues()->GetSize()-1)) 
        {          
          if(line.IsEmpty()) AddTabs(encodelevel,XFILEJSON_SPACETABS,line);
          
          AddControlCharacter(XFILEJSONCONTROLCHAR_COMMA, line);
        }

      if(!line.IsEmpty())
        {         
          AddLine(line);  
          //XDEBUG_PRINTCOLOR(0,line.Get());
        }

      line.Empty();
    }
  
  line.Empty();

  if(istabulatedline)
    { 
      encodelevel--;
      AddTabs(encodelevel,XFILEJSON_SPACETABS,line);      
    }

  AddControlCharacter(isobject?XFILEJSONCONTROLCHAR_CLOSEBRACE:XFILEJSONCONTROLCHAR_CLOSEBRACKET, line);
  AddLine(line);

  //XDEBUG_PRINTCOLOR(0,line.Get());

  return true;
}


