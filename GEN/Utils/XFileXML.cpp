//------------------------------------------------------------------------------------------
//  XFILEXML.CPP
//
//  XML File functions (XML format)
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 04/06/2004 12:02:06
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>

#include "XFileXML.h"
#include "XDebugTrace.h"
#include "XFactory.h"
#include "XTimer.h"

#include "XMemory.h"


//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------


/*-------------------------------------------------------------------
//  XFILEXMLATTRIBUTE::XFILEXMLATTRIBUTE
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/02/2011 22:18:46
//
//  @return
//  */
/*-----------------------------------------------------------------*/
XFILEXMLATTRIBUTE::XFILEXMLATTRIBUTE()
{
  Clean();
}


/*-------------------------------------------------------------------
//  XFILEXMLATTRIBUTE::XFILEXMLATTRIBUTE
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/02/2011 10:23:28
//
//  @return
//  @param        name :
*/
/*-----------------------------------------------------------------*/
XFILEXMLATTRIBUTE::XFILEXMLATTRIBUTE(XSTRING& name)
{
  Clean();

  this->name = name;
}



/*-------------------------------------------------------------------
//  XFILEXMLATTRIBUTE::XFILEXMLATTRIBUTE
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/02/2011 12:45:11
//
//  @return
//  @param        name :
*/
/*-----------------------------------------------------------------*/
XFILEXMLATTRIBUTE::XFILEXMLATTRIBUTE(XCHAR* name)
{
  Clean();

  this->name = name;
}



/*-------------------------------------------------------------------
//  XFILEXMLATTRIBUTE::XFILEXMLATTRIBUTE
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/02/2011 10:23:36
//
//  @return
//  @param        name :
//  @param        value :
*/
/*-----------------------------------------------------------------*/
XFILEXMLATTRIBUTE::XFILEXMLATTRIBUTE(XSTRING& name,XSTRING& value)
{
  Clean();

  this->name  = name;
  this->value = value;
}


/*-------------------------------------------------------------------
//  XFILEXMLATTRIBUTE::XFILEXMLATTRIBUTE
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/02/2011 12:46:11
//
//  @return
//  @param        name :
//  @param        value :
*/
/*-----------------------------------------------------------------*/
XFILEXMLATTRIBUTE::XFILEXMLATTRIBUTE(XCHAR* name,XCHAR* value)
{
  Clean();

  this->name  = name;
  this->value = value;
}



/*-------------------------------------------------------------------
//  XFILEXMLATTRIBUTE::~XFILEXMLATTRIBUTE
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/02/2011 22:19:13
//
//  @return
//  */
/*-----------------------------------------------------------------*/
XFILEXMLATTRIBUTE::~XFILEXMLATTRIBUTE()
{
  Clean();
}


/*-------------------------------------------------------------------
//  XFILEXMLATTRIBUTE::GetName
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/02/2011 10:25:49
//
//  @return       XSTRING& :
//  */
/*-----------------------------------------------------------------*/
XSTRING& XFILEXMLATTRIBUTE::GetName()
{
  return name;
}



/*-------------------------------------------------------------------
//  XFILEXMLATTRIBUTE::SetName
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/02/2011 10:26:21
//
//  @return       bool :
//  @param        name :
*/
/*-----------------------------------------------------------------*/
bool XFILEXMLATTRIBUTE::SetName(XSTRING& name)
{
  this->name = name;

  return true;
}


/*-------------------------------------------------------------------
//  XFILEXMLATTRIBUTE::SetName
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/02/2011 12:47:36
//
//  @return       bool :
//  @param        name :
*/
/*-----------------------------------------------------------------*/
bool XFILEXMLATTRIBUTE::SetName(XCHAR* name)
{
  this->name = name;

  return true;
}


/*-------------------------------------------------------------------
//  XFILEXMLATTRIBUTE::GetValue
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/02/2011 10:26:38
//
//  @return       XSTRING& :
//  */
/*-----------------------------------------------------------------*/
XSTRING& XFILEXMLATTRIBUTE::GetValue()
{
  return value;
}



/*-------------------------------------------------------------------
//  XFILEXMLATTRIBUTE::SetValue
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/02/2011 10:27:20
//
//  @return       bool :
//  @param        value :
*/
/*-----------------------------------------------------------------*/
bool XFILEXMLATTRIBUTE::SetValue(XSTRING& value)
{
  this->value = value;

  return true;
}


/*-------------------------------------------------------------------
//  XFILEXMLATTRIBUTE::SetValue
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/02/2011 12:47:59
//
//  @return       bool :
//  @param        value :
*/
/*-----------------------------------------------------------------*/
bool XFILEXMLATTRIBUTE::SetValue(XCHAR* value)
{
  this->value = value;

  return true;
}



/*-------------------------------------------------------------------
//  XFILEXMLATTRIBUTE::Clean
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/02/2011 22:19:31
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void XFILEXMLATTRIBUTE::Clean()
{
  name  = "";
  value = "";
}




/*-------------------------------------------------------------------
//  XFILEXMLELEMENT::XFILEXMLELEMENT
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      18/07/2009 03:16:57 p.m.
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
XFILEXMLELEMENT::XFILEXMLELEMENT()
{
  Clean();
}


/*-------------------------------------------------------------------
//  XFILEXMLELEMENT::XFILEXMLELEMENT
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/02/2011 11:19:14
//
//  @return
//  @param        name :
//  @param        type :
*/
/*-----------------------------------------------------------------*/
XFILEXMLELEMENT::XFILEXMLELEMENT(XSTRING& name, XFILEXMLELEMENTTYPE type)
{
  Clean();

  this->name  = name;
  this->type  = type;
}


/*-------------------------------------------------------------------
//  XFILEXMLELEMENT::XFILEXMLELEMENT
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/02/2011 12:49:36
//
//  @return
//  @param        name :
//  @param        type :
*/
/*-----------------------------------------------------------------*/
XFILEXMLELEMENT::XFILEXMLELEMENT(XCHAR* name, XFILEXMLELEMENTTYPE type)
{
  Clean();

  this->name  = name;
  this->type  = type;
}



/*-------------------------------------------------------------------
//  XFILEXMLELEMENT::XFILEXMLELEMENT
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/02/2011 11:19:18
//
//  @return
//  @param        name :
//  @param        value :
//  @param        type :
*/
/*-----------------------------------------------------------------*/
XFILEXMLELEMENT::XFILEXMLELEMENT(XSTRING& name,XSTRING& value, XFILEXMLELEMENTTYPE type)
{
  Clean();

  this->name  = name;
  this->value = value;
  this->type  = type;
}



/*-------------------------------------------------------------------
//  XFILEXMLELEMENT::XFILEXMLELEMENT
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/02/2011 12:50:11
//
//  @return
//  @param        name :
//  @param        value :
//  @param        type :
*/
/*-----------------------------------------------------------------*/
XFILEXMLELEMENT::XFILEXMLELEMENT(XCHAR* name,XCHAR* value, XFILEXMLELEMENTTYPE type)
{
  Clean();

  this->name  = name;
  this->value = value;
  this->type  = type;
}



/*-------------------------------------------------------------------
//  XFILEXMLELEMENT::~XFILEXMLELEMENT
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      18/07/2009 03:17:05 p.m.
//
//  @return       virtual :
//  */
/*-----------------------------------------------------------------*/
XFILEXMLELEMENT::~XFILEXMLELEMENT()
{
  DeleteAllAtributes();
  DeleteAllElements();

  Clean();
}



/*-------------------------------------------------------------------
//  XFILEXMLELEMENT::GetName
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/02/2011 10:32:27
//
//  @return       XSTRING& :
//  */
/*-----------------------------------------------------------------*/
XSTRING& XFILEXMLELEMENT::GetName()
{
  return name;
}


/*-------------------------------------------------------------------
//  XFILEXMLELEMENT::SetName
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/02/2011 10:33:08
//
//  @return       bool :
//  @param        name :
*/
/*-----------------------------------------------------------------*/
bool XFILEXMLELEMENT::SetName(XSTRING& name)
{
  this->name = name;

  return true;
}



/*-------------------------------------------------------------------
//  XFILEXMLELEMENT::SetName
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/02/2011 12:36:58
//
//  @return       bool :
//  @param        name :
*/
/*-----------------------------------------------------------------*/
bool XFILEXMLELEMENT::SetName(XCHAR* name)
{
  this->name = name;

  return true;
}



/*-------------------------------------------------------------------
//  XFILEXMLELEMENT::AddAtribute
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/02/2011 11:25:08
//
//  @return       XFILEXMLATTRIBUTE* :
//  @param        name :
//  @param        value :
*/
/*-----------------------------------------------------------------*/
XFILEXMLATTRIBUTE* XFILEXMLELEMENT::AddAtribute(XSTRING& name,XSTRING& value)
{
  XFILEXMLATTRIBUTE* attribute = new XFILEXMLATTRIBUTE(name,value);
  if(!attribute) return NULL;

  attributes.Add(attribute);

  return attribute;
}



/*-------------------------------------------------------------------
//  XFILEXMLELEMENT::AddAtribute
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/02/2011 12:51:33
//
//  @return       XFILEXMLATTRIBUTE* :
//  @param        name :
//  @param        value :
*/
/*-----------------------------------------------------------------*/
XFILEXMLATTRIBUTE* XFILEXMLELEMENT::AddAtribute(XCHAR* name,XCHAR* value)
{
  XFILEXMLATTRIBUTE* attribute = new XFILEXMLATTRIBUTE(name,value);
  if(!attribute) return NULL;

  attributes.Add(attribute);

  return attribute;
}


/*-------------------------------------------------------------------
//  XFILEXMLELEMENT::GetNAttributes
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      14/02/2011 10:35:58
//
//  @return       int :
//  */
/*-----------------------------------------------------------------*/
int XFILEXMLELEMENT::GetNAttributes()
{
  return attributes.GetSize();
}



/*-------------------------------------------------------------------
//  XFILEXMLELEMENT::GetAttribute
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      14/02/2011 12:18:38
//
//  @return       XFILEXMLATTRIBUTE* :
//  @param        index :
*/
/*-----------------------------------------------------------------*/
XFILEXMLATTRIBUTE* XFILEXMLELEMENT::GetAttribute(int index)
{
  if(attributes.IsEmpty()) return NULL;

  XFILEXMLATTRIBUTE* attribute = (XFILEXMLATTRIBUTE*)attributes.Get(index);
  return attribute;
}




/*-------------------------------------------------------------------
//  XFILEXMLELEMENT::GetValueAttribute
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      08/11/2011 17:03:21
//
//  @return       XCHAR* :
//  @param        name :
*/
/*-----------------------------------------------------------------*/
XCHAR* XFILEXMLELEMENT::GetValueAttribute(XSTRING& name)
{
  return GetValueAttribute(name.Get());
}



/*-------------------------------------------------------------------
//  XFILEXMLELEMENT::GetValueAttribute
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      08/11/2011 17:03:25
//
//  @return       XCHAR* :
//  @param        name :
*/
/*-----------------------------------------------------------------*/
XCHAR* XFILEXMLELEMENT::GetValueAttribute(XCHAR* name)
{
  int nattributes = GetNAttributes();

  for(int c=0; c<nattributes; c++)
    {
      XFILEXMLATTRIBUTE* attribute =  GetAttribute(c);
      if(attribute)
        {
          if(!attribute->GetName().Compare(name))
            {
              return attribute->GetValue().Get();
            }
        }
    }

  return NULL;
}



/*-------------------------------------------------------------------
//  XFILEXMLELEMENT::GetValueAttribute
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      08/11/2011 17:03:21
//
//  @return       bool :
//  @param        name :
*/
/*-----------------------------------------------------------------*/
bool XFILEXMLELEMENT::GetValueAttribute(XSTRING& name, XSTRING& value)
{
  return GetValueAttribute(name.Get(),value);
}



/*-------------------------------------------------------------------
//  XFILEXMLELEMENT::GetValueAttribute
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      08/11/2011 17:03:25
//
//  @return       bool :
//  @param        name :
*/
/*-----------------------------------------------------------------*/
bool XFILEXMLELEMENT::GetValueAttribute(XCHAR* name,XSTRING& value)
{
  for(int c=0;c<GetNAttributes();c++)
    {
      XFILEXMLATTRIBUTE* attribute =  GetAttribute(c);
      if(attribute)
        {
          if(!attribute->GetName().Compare(name))
            {
              value = attribute->GetValue();

              return true;
            }
        }
    }

  return false;
}



/*-------------------------------------------------------------------
//  XFILEXMLELEMENT::GetIntValueAttribute
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      08/11/2011 17:27:52
//
//  @return       int :
//  @param        name :
*/
/*-----------------------------------------------------------------*/
int XFILEXMLELEMENT::GetIntValueAttribute(XSTRING& name)
{
  return GetIntValueAttribute(name.Get());
}



/*-------------------------------------------------------------------
//  XFILEXMLELEMENT::GetIntValueAttribute
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      08/11/2011 17:28:18
//
//  @return       int :
//  @param        name :
*/
/*-----------------------------------------------------------------*/
int XFILEXMLELEMENT::GetIntValueAttribute(XCHAR* name)
{
  XSTRING value;

  if(GetValueAttribute(name,value)) return value.ConvertToInt();

  return 0;
}

/*-------------------------------------------------------------------
//  XFILEXMLELEMENT::GetBoolValueAttribute
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      30/11/2016 10:53:52
//
//  @return       bool :
//
//  @param        name :
*/
/*-----------------------------------------------------------------*/
bool XFILEXMLELEMENT::GetBoolValueAttribute(XCHAR* name)
{
  XSTRING value;

  if(GetValueAttribute(name,value))
    return value.ConvertToBoolean();

  return false;
}

/*-------------------------------------------------------------------
//  XFILEXMLELEMENT::GetFloatValueAttribute
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      30/11/2016 10:53:42
//
//  @return       double :
//
//  @param        name :
*/
/*-----------------------------------------------------------------*/
double XFILEXMLELEMENT::GetFloatValueAttribute(XCHAR* name)
{
  XSTRING value;

  if(GetValueAttribute(name,value))
    return value.ConvertToDouble();

  return 0.0;
}

/*-------------------------------------------------------------------
//  XFILEXMLELEMENT::DeleteAtribute
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/02/2011 10:40:08
//
//  @return       bool :
//  @param        index :
*/
/*-----------------------------------------------------------------*/
bool XFILEXMLELEMENT::DeleteAtribute(int index)
{
  XFILEXMLATTRIBUTE* attribute = (XFILEXMLATTRIBUTE*)attributes.Get(index);
  if(!attribute)  return false;

  attributes.Delete(attribute);

  delete attribute;

  return true;
}



/*-------------------------------------------------------------------
//  XFILEXMLELEMENT::DeleteAllAtributes
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/02/2011 10:43:23
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XFILEXMLELEMENT::DeleteAllAtributes()
{
  if(attributes.IsEmpty()) return false;

  attributes.DeleteContents();

  attributes.DeleteAll();

  return true;
}


/*-------------------------------------------------------------------
//  XFILEXMLELEMENT::GetValue
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/02/2011 10:44:00
//
//  @return       XSTRING& :
//  */
/*-----------------------------------------------------------------*/
XSTRING& XFILEXMLELEMENT::GetValue()
{
  return value;
}



/*-------------------------------------------------------------------
//  XFILEXMLELEMENT::SetValue
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/02/2011 10:44:33
//
//  @return       bool :
//  @param        value :
*/
/*-----------------------------------------------------------------*/
bool XFILEXMLELEMENT::SetValue(XSTRING& value)
{
  this->value = value;

  return true;
}


/*-------------------------------------------------------------------
//  XFILEXMLELEMENT::SetValue
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/02/2011 12:37:50
//
//  @return       bool :
//  @param        value :
*/
/*-----------------------------------------------------------------*/
bool XFILEXMLELEMENT::SetValue(XCHAR* value)
{
  this->value = value;

  return true;
}



/*-------------------------------------------------------------------
//  XFILEXMLELEMENT::AddElement
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/02/2011 22:52:11
//
//  @return       XFILEXMLELEMENT* :
//  */
/*-----------------------------------------------------------------*/
XFILEXMLELEMENT* XFILEXMLELEMENT::AddElement()
{
  XFILEXMLELEMENT* element = new XFILEXMLELEMENT();
  if(!element) return NULL;

  element->SetFather(this);

  elements.Add(element);

  return element;

}


/*-------------------------------------------------------------------
//  XFILEXMLELEMENT::AddElement
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/02/2011 11:23:48
//
//  @return       XFILEXMLELEMENT* :
//  @param        name :
*/
/*-----------------------------------------------------------------*/
XFILEXMLELEMENT* XFILEXMLELEMENT::AddElement(XSTRING& name)
{
  XFILEXMLELEMENT* element = new XFILEXMLELEMENT(name);
  if(!element) return NULL;

  element->SetFather(this);

  elements.Add(element);

  return element;
}



/*-------------------------------------------------------------------
//  XFILEXMLELEMENT::AddElement
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/02/2011 12:52:21
//
//  @return       XFILEXMLELEMENT* :
//  @param        name :
*/
/*-----------------------------------------------------------------*/
XFILEXMLELEMENT* XFILEXMLELEMENT::AddElement(XCHAR* name)
{
  XFILEXMLELEMENT* element = new XFILEXMLELEMENT(name);
  if(!element) return NULL;

  element->SetFather(this);

  elements.Add(element);

  return element;
}



/*-------------------------------------------------------------------
//  XFILEXMLELEMENT::AddElement
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/02/2011 11:24:11
//
//  @return       XFILEXMLELEMENT* :
//  @param        name :
//  @param        value :
*/
/*-----------------------------------------------------------------*/
XFILEXMLELEMENT* XFILEXMLELEMENT::AddElement(XSTRING& name,XSTRING& value)
{
  XFILEXMLELEMENT* element = new XFILEXMLELEMENT(name,value);
  if(!element) return NULL;

  element->SetFather(this);

  elements.Add(element);

  return element;
}


/*-------------------------------------------------------------------
//  XFILEXMLELEMENT::AddElement
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/02/2011 12:52:49
//
//  @return       XFILEXMLELEMENT* :
//  @param        name :
//  @param        value :
*/
/*-----------------------------------------------------------------*/
XFILEXMLELEMENT* XFILEXMLELEMENT::AddElement(XCHAR* name,XCHAR* value)
{
  XFILEXMLELEMENT* element = new XFILEXMLELEMENT(name,value);
  if(!element) return NULL;

  element->SetFather(this);

  elements.Add(element);


  return element;
}


/*-------------------------------------------------------------------
//  XFILEXMLELEMENT::GetNElements
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      14/02/2011 12:21:55
//
//  @return       int :
//  */
/*-----------------------------------------------------------------*/
int XFILEXMLELEMENT::GetNElements()
{
  return elements.GetSize();
}




/*-------------------------------------------------------------------
//  XFILEXMLELEMENT::GetElement
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      14/02/2011 12:22:21
//
//  @return       XFILEXMLELEMENT* :
//  @param        index :
*/
/*-----------------------------------------------------------------*/
XFILEXMLELEMENT* XFILEXMLELEMENT::GetElement(int index)
{
  if(elements.IsEmpty()) return NULL;

  XFILEXMLELEMENT* element = (XFILEXMLELEMENT*)elements.Get(index);
  return element;
}



/*-------------------------------------------------------------------
//  XFILEXMLELEMENT::DeleteElement
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/02/2011 10:48:50
//
//  @return       bool :
//  @param        index :
*/
/*-----------------------------------------------------------------*/
bool XFILEXMLELEMENT::DeleteElement(int index)
{
  XFILEXMLELEMENT* element = (XFILEXMLELEMENT*)elements.Get(index);
  if(!element)  return false;

  elements.Delete(element);

  delete element;

  return true;
}



/*-------------------------------------------------------------------
//  XFILEXMLELEMENT::DeleteAllElements
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/02/2011 10:45:32
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XFILEXMLELEMENT::DeleteAllElements()
{
  if(elements.IsEmpty()) return false;

  elements.DeleteContents();
  elements.DeleteAll();

  return true;
}


/*-------------------------------------------------------------------
//  XFILEXMLELEMENT::GetFather
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/02/2011 12:26:32
//
//  @return       XFILEXMLELEMENT* :
//  */
/*-----------------------------------------------------------------*/
XFILEXMLELEMENT* XFILEXMLELEMENT::GetFather()
{
  return father;
}



/*-------------------------------------------------------------------
//  XFILEXMLELEMENT::SetFather
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/02/2011 12:26:29
//
//  @return       bool :
//  @param        father :
*/
/*-----------------------------------------------------------------*/
bool XFILEXMLELEMENT::SetFather(XFILEXMLELEMENT* father)
{
  this->father = father;

  return true;
}



/*-------------------------------------------------------------------
//  XFILEXMLELEMENT::GetType
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/02/2011 22:48:11
//
//  @return       XFILEXMLELEMENTTYPE :
//  */
/*-----------------------------------------------------------------*/
XFILEXMLELEMENTTYPE XFILEXMLELEMENT::GetType()
{
  return type;
}



/*-------------------------------------------------------------------
//  XFILEXMLELEMENT::SetType
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/02/2011 22:48:14
//
//  @return       bool :
//  @param        type :
*/
/*-----------------------------------------------------------------*/
bool XFILEXMLELEMENT::SetType(XFILEXMLELEMENTTYPE type)
{
  this->type = type;

  return true;
}



/*-------------------------------------------------------------------
//  XFILEXMLELEMENT::Clean
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      18/07/2009 03:17:37 p.m.
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void XFILEXMLELEMENT::Clean()
{
  type   = XFILEXMLELEMENTTYPE_UNKNOWN;
  father = NULL;
  name   = __L("");
  value  = __L("");
}



//-------------------------------------------------------------------
//  XFILEXML::XFILEXML
/**
//
//
//  @author       Abraham J. Velez
//  @version      15/07/2002 19:11:13
//
//  @return
*/
//-------------------------------------------------------------------
XFILEXML::XFILEXML() : XFILETXT()
{
  Clean();
}



//-------------------------------------------------------------------
//  XFILEXML::~XFILEXML
/**
//
//
//  @author       Abraham J. Velez
//  @version      15/04/2002 9:48:21
//
//  @return
//  */
//-------------------------------------------------------------------
XFILEXML::~XFILEXML()
{
  DeleteAllCFGAtributes();

  DeleteAllElements();

  Clean();
}



/*-------------------------------------------------------------------
//  XFILEXML::GetRoot
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      08/11/2011 16:45:37
//
//  @return       XFILEXMLELEMENT* :
//  */
/*-----------------------------------------------------------------*/
XFILEXMLELEMENT* XFILEXML::GetRoot()
{
  return root;
}



/*-------------------------------------------------------------------
//  XFILEXML::SetRoot
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/09/2012 23:11:35
//
//  @return       bool :
//  @param        root :
*/
/*-----------------------------------------------------------------*/
bool XFILEXML::SetRoot(XFILEXMLELEMENT* root)
{
  if(!root) return false;

  this->root = root;

  return true;
}




/*-------------------------------------------------------------------
//  XFILEXML::EncodeAllLines
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      15/10/2012 9:08:23
//
//  @return       bool :
//  @param        istabulatedline :
*/
/*-----------------------------------------------------------------*/
bool XFILEXML::EncodeAllLines(bool istabulatedline)
{
  DeleteAllLines();

  XSTRING line;

  EncodeConfig(line);
  AddLine(line);

  //XDEBUGTRACE_PRINTCOLOR(1,line.Get());

  encodelevel  = -1;

  if(root) EncodeElements(istabulatedline,root);

  return true;
}



/*-------------------------------------------------------------------
//  XFILEXML::DecodeAllLines
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/09/2012 23:00:58
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XFILEXML::DecodeAllLines()
{
  DeleteAllElements();

  XSTRING   string;
  XSTRING*  stringml;
  bool      quote      =  false;
  bool      cdata      =  false;
  int       c          =  0;

  string.Empty();

  while(c<GetNLines())
    {
      stringml = GetLine(c);

      for(int d=0;d<(int)stringml->GetSize();d++)
        {
          XCHAR letter[2] = { 0, 0 };
          letter[0] = (*stringml)[d];

          if((*stringml)[d]==__C('\"')) quote=!quote;

          if((*stringml)[d]==__C('<')&&(!quote))
            {
              if(string.Find(__L("[CDATA["),false)!=XSTRING_NOTFOUND) cdata=true;

              if(!string.IsEmpty() && string.HaveCharacters() && (!cdata))
                {
                  DecodeLine(string,false);
                  string.Empty();
                }

              quote = false;
              string += letter;
            }
           else
            {
              if(((*stringml)[d]==__C('>'))&&(!quote))
                {
                  if(string.Find(__L("]]"),false)!=XSTRING_NOTFOUND) cdata=false;

                  string += letter;
                  if(!string.IsEmpty() && string.HaveCharacters() && (!cdata))
                    {
                      DecodeLine(string,true);
                      string.Empty();
                    }
                }
               else string += letter;
            }
        }

      c++;
    }

  DeleteAllLines();

  return true;
}




/*-------------------------------------------------------------------
//  XFILEXML::ReadAndDecodeAllLines
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/02/2011 11:18:26
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XFILEXML::ReadAndDecodeAllLines()
{
  if(!IsOpen())      return false;

  if(!ReadAllFile()) return false;

  DecodeAllLines();

  return true;
}




/*-------------------------------------------------------------------
//  XFILEXML::WriteAndEncodeAllLines
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      15/10/2012 9:09:35
//
//  @return       bool :
//  @param        istabulatedline :
//  @param        formatchar :
//  @param        XFILETXTTYPELF  typeLF :
*/
/*-----------------------------------------------------------------*/
bool XFILEXML::WriteAndEncodeAllLines(bool istabulatedline,XFILETXTFORMATCHAR formatchar, XFILETXTTYPELF  typeLF)
{
  SetFormatChar(formatchar);
  SetTypeLF(typeLF);

  return WriteAndEncodeAllLines(istabulatedline);
}




/*-------------------------------------------------------------------
//  XFILEXML::WriteAndEncodeAllLines
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      15/10/2012 9:09:23
//
//  @return       bool :
//  @param        istabulatedline :
*/
/*-----------------------------------------------------------------*/
bool XFILEXML::WriteAndEncodeAllLines(bool istabulatedline)
{
  if(!IsOpen()) return false;

  file->SetPosition(0);

  EncodeAllLines(istabulatedline);

  bool status = WriteAllFile();

  return status;
}


/*-------------------------------------------------------------------
//  XFILEXML::SearchChild
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      03/03/2017 16:08:22
//
//  @return       XFILEXMLELEMENT* :
//
//  @param        name :
//  @param        NULL :
*/
/*-----------------------------------------------------------------*/
XFILEXMLELEMENT* XFILEXML::SearchChild(XCHAR* name, XFILEXMLELEMENT* father)
{
  XFILEXMLELEMENT* _father = NULL;
  XFILEXMLELEMENT* element = NULL;

  if(!_father) _father = root;
  if(!_father) return element;

  for(int c=0;c<_father->GetNElements();c++)
    {
      XFILEXMLELEMENT* element = _father->GetElement(c);
      if(element->GetName().Compare(name)==0)
        return element;
    }

  return NULL;
}


/*-------------------------------------------------------------------
//  XFILEXML::SearchElement
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      15/02/2011 11:52:35
//
//  @return       XFILEXMLELEMENT* :
//  @param        name :
//  @param        index :
//  @param        father :
*/
/*-----------------------------------------------------------------*/
XFILEXMLELEMENT* XFILEXML::SearchElement(XSTRING& name, int& index, XFILEXMLELEMENT* father)
{
  return SearchElement(name.Get(), index, father);
}



/*-------------------------------------------------------------------
//  XFILEXML::SearchElement
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      15/02/2011 11:44:39
//
//  @return       XFILEXMLELEMENT* :
//  @param        name :
//  @param        father :
*/
/*-----------------------------------------------------------------*/
XFILEXMLELEMENT* XFILEXML::SearchElement(XCHAR* name, int& index, XFILEXMLELEMENT* father)
{
  XFILEXMLELEMENT* _father = father;
  XFILEXMLELEMENT* element = NULL;

  if(!_father) _father = root;
  if(!_father) return element;

  XSTRING nameelement(name);

  for(int c=index;c<_father->GetNElements();c++)
    {
      XFILEXMLELEMENT* etemp = _father->GetElement(c);
      if(etemp)
        {
          if(etemp->GetName()==nameelement)
            {
              index = c;
              return etemp;
            }
           else
            {
              XFILEXMLELEMENT* etemp2 = SearchElement(name, index, etemp);
              if(etemp2) return etemp2;
            }
        }
    }

  return element;
}



/*-------------------------------------------------------------------
//  XFILEXML::SearchElement
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      15/02/2011 11:58:06
//
//  @return       XFILEXMLELEMENT* :
//  @param        name :
//  @param        index :
//  @param        namefather :
*/
/*-----------------------------------------------------------------*/
XFILEXMLELEMENT* XFILEXML::SearchElement(XCHAR* name, int& index, XSTRING& namefather)
{
  return SearchElement(name, index, namefather.Get());
}



/*-------------------------------------------------------------------
//  XFILEXML::SearchElement
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      15/02/2011 11:58:15
//
//  @return       XFILEXMLELEMENT* :
//  @param        name :
//  @param        index :
//  @param        namefather :
*/
/*-----------------------------------------------------------------*/
XFILEXMLELEMENT* XFILEXML::SearchElement(XCHAR* name, int& index, XCHAR* namefather)
{
  int              _index  = 0;
  XFILEXMLELEMENT* father  = SearchElement(namefather, _index);
  if(!father) return NULL;

  return SearchElement(name, index, father);
}




/*-------------------------------------------------------------------
//  XFILEXML::GetAttributeValue
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      15/02/2011 12:15:44
//
//  @return       bool :
//  @param        element :
//  @param        nameattribute :
//  @param        value :
*/
/*-----------------------------------------------------------------*/
bool XFILEXML::SearchAttributeValue(XFILEXMLELEMENT* element,XSTRING& nameattribute,XSTRING& value)
{
  return SearchAttributeValue(element,nameattribute.Get(),value);
}



/*-------------------------------------------------------------------
//  XFILEXML::SearchAttributeValue
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      15/02/2011 12:19:22
//
//  @return       bool :
//  @param        element :
//  @param        nameattribute :
//  @param        value :
*/
/*-----------------------------------------------------------------*/
bool XFILEXML::SearchAttributeValue(XFILEXMLELEMENT* element,XCHAR* nameattribute,XSTRING& value)
{
  if(!element)                   return false;
  if(!element->GetNAttributes()) return false;

  for(int c=0;c<element->GetNAttributes();c++)
    {
      XFILEXMLATTRIBUTE* attribute = element->GetAttribute(c);
      if(attribute)
        {
          XSTRING _nameattribute(nameattribute);
          if(attribute->GetName() == _nameattribute)
            {
               value = attribute->GetValue();
               return true;
            }
        }
    }

  return false;
}



/*-------------------------------------------------------------------
//  XFILEXML::GetCFGAtributes
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      12/09/2012 21:30:39
//
//  @return       XVECTOR* :
//  */
/*-----------------------------------------------------------------*/
XVECTOR<XFILEXMLATTRIBUTE*>* XFILEXML::GetCFGAtributes()
{
  return &cfgattributes;
}




/*-------------------------------------------------------------------
//  XFILEXML::DeleteAllCFGAtributes
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/02/2011 10:43:23
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XFILEXML::DeleteAllCFGAtributes()
{
  if(cfgattributes.IsEmpty()) return false;

  cfgattributes.DeleteContents();
  cfgattributes.DeleteAll();

  return true;
}



/*-------------------------------------------------------------------
//  XFILEXML::DeleteAllElements
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/02/2011 12:31:49
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XFILEXML::DeleteAllElements()
{
  actualelement = NULL;

  if(root)
    {
      delete root;
      root = NULL;
    }

  return true;
}



//-------------------------------------------------------------------
//  XFILEXML::Clean
/**
//
//
//  @author       Abraham J. Velez
//  @version      04/09/2001 12:46:19
//
//  @return       void :
//  */
//-------------------------------------------------------------------
void XFILEXML::Clean()
{
  root          = NULL;
  actualelement = NULL;
  encodelevel   = -1;
  ignorecomments= false;
}




/*-------------------------------------------------------------------
//  XFILEXML::DecodeLine
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      08/02/2011 16:50:02
//
//  @return       int :
//  @param        string :
//  @param        iselement :
*/
/*-----------------------------------------------------------------*/
int XFILEXML::DecodeLine(XSTRING& string,bool iselement)
{
  XFILEXMLELEMENTTYPE     elementtype  = XFILEXMLELEMENTTYPE_UNKNOWN;
  XFILEXMLELEMENTTYPELINE typeline     = XFILEXMLELEMENTTYPELINE_UNKNOWN;

  if(iselement)
    {
      //string.DeleteNoCharacters(XSTRINGCONTEXT_ATFIRST);
      //string.DeleteNoCharacters(XSTRINGCONTEXT_ATEND);

      //int n=string.FindCharacter(__C('<'));
      //string.DeleteCharacters(0,n);

      XSTRING str = string;

      //str.DeleteNoCharacters(XSTRINGCONTEXT_ALLSTRING);
      str.Fast_DeleteNoCharacters(XSTRINGCONTEXT_ALLSTRING);

      int iini = str.FindCharacter(__C('<'));
      int iend = str.FindCharacter(__C('>'),0,true);

      if(iini!=XSTRING_NOTFOUND) iini++;
      if(iend!=XSTRING_NOTFOUND) iend--;

      switch(str[iini])
        {
          case __C('?') : if(str[iend]==__C('?')) elementtype = XFILEXMLELEMENTTYPE_CONFIG;
                          break;

          case __C('/') : elementtype = XFILEXMLELEMENTTYPE_NORMAL;
                          typeline    = XFILEXMLELEMENTTYPELINE_END;
                          break;

          case __C('!') : if((str[iini+1]==__C('-')) && (str[iini+2]==__C('-')) && (str[iend]  ==__C('-')) && (str[iend-1]==__C('-')))
                            elementtype = XFILEXMLELEMENTTYPE_COMMENT;
                          else
                          if(str.Find(__L("[CDATA["),false)!=XSTRING_NOTFOUND)
                            elementtype = XFILEXMLELEMENTTYPE_CDATA;
                          else
                          {
                            if((str[iini+1]==__C('-')) && (str[iini+2]==__C('-')))
                              is_multicomment=true;
                          }

                          break;

               default  : elementtype  = XFILEXMLELEMENTTYPE_NORMAL;
                          typeline     = XFILEXMLELEMENTTYPELINE_START;
                          if(str[iend]==__C('/'))  typeline    = XFILEXMLELEMENTTYPELINE_STARTEND;
                          break;
        }

      switch(elementtype)
        {
          case XFILEXMLELEMENTTYPE_UNKNOWN    : break;
          case XFILEXMLELEMENTTYPE_CONFIG   : DecodeConfig(string);             break;
          case XFILEXMLELEMENTTYPE_COMMENT  : DecodeComent(string);             break;
          case XFILEXMLELEMENTTYPE_CDATA    : DecodeCDATA(string);              break;
          case XFILEXMLELEMENTTYPE_NORMAL   : DecodeElement(string,typeline);   break;
        }
    }
   else
    {
      if(actualelement)
        {
          actualelement->SetValue(string);
        }
    }


  return 0;
}



/*-------------------------------------------------------------------
//  XFILEXML::DecodeConfig
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      10/02/2011 12:09:44
//
//  @return       bool :
//  @param        string :
*/
/*-----------------------------------------------------------------*/
bool XFILEXML::DecodeConfig(XSTRING& string)
{
  XSTRING values;

  if(string.Copy(__L("<?xml"),__L("?>"),false,0,values)==XSTRING_NOTFOUND)  return false;

  XSTRING name;
  XSTRING value;

  int indexname = 0;

  while(1)
    {
      indexname = values.Copy(indexname, __L("="),false,name);
      if(indexname==XSTRING_NOTFOUND) break;

      name.DeleteNoCharacters(XSTRINGCONTEXT_ALLSTRING);

      indexname = values.Copy(__L("\""),__L("\""),false,indexname,value);
      if(indexname==XSTRING_NOTFOUND) break;

      XFILEXMLATTRIBUTE* attribute = new XFILEXMLATTRIBUTE(name,value);
      if(!attribute) break;

      cfgattributes.Add(attribute);
    }

  return true;
}



/*-------------------------------------------------------------------
//  XFILEXML::DecodeComent
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/02/2011 22:33:20
//
//  @return       int :
//  @param        elementtype :
//  @param        string :
*/
/*-----------------------------------------------------------------*/
bool XFILEXML::DecodeComent(XSTRING& string)
{
  if(!actualelement) return false;
  //if(ignorecomments) return true;

  XSTRING value;

  if(string.Copy(__L("<!--"),__L("-->"),false,0,value)==XSTRING_NOTFOUND)  return false;

  XFILEXMLELEMENT* element = actualelement->AddElement();
  if(!element) return false;

  element->SetType(XFILEXMLELEMENTTYPE_COMMENT);
  element->SetValue(value);

  return true;
}



/*-------------------------------------------------------------------
//  XFILEXML::DecodeCDATA
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      10/02/2011 12:41:45
//
//  @return       bool :
//  @param        elementtype :
//  @param        string :
*/
/*-----------------------------------------------------------------*/
bool XFILEXML::DecodeCDATA(XSTRING& string)
{
  if(!actualelement) return false;

  XSTRING value;

  if(string.Copy(__L("[CDATA["),__L("]]"),false,0,value)==XSTRING_NOTFOUND)  return false;

  XFILEXMLELEMENT* element = actualelement->AddElement();
  if(!element) return false;

  element->SetType(XFILEXMLELEMENTTYPE_CDATA);
  element->SetValue(value);

  return true;
}



/*-------------------------------------------------------------------
//  XFILEXML::DecodeElement
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/02/2011 10:36:15
//
//  @return       bool :
//  @param        string :
//  @param        typeline :
*/
/*-----------------------------------------------------------------*/
bool XFILEXML::DecodeElement(XSTRING& string,XFILEXMLELEMENTTYPELINE typeline)
{
  switch(typeline)
    {
      case XFILEXMLELEMENTTYPELINE_UNKNOWN    : return false;

      case XFILEXMLELEMENTTYPELINE_START    :
      case XFILEXMLELEMENTTYPELINE_STARTEND : { XSTRING values;
                                                XSTRING name;
                                                XSTRING value;

                                                if(string.Copy(__L("<"),__L(" "),false,0,name)==XSTRING_NOTFOUND) return false;

                                                //name.Set(string);
                                                name.DeleteCharacter(__C('<'),XSTRINGCONTEXT_ALLSTRING);
                                                name.DeleteCharacter(__C('>'),XSTRINGCONTEXT_ALLSTRING);

                                                name.DeleteNoCharacters();
                                                name.DeleteNoCharacters(__L("\20"),0, XSTRINGCONTEXT_ATFIRST);
                                                name.DeleteNoCharacters(__L("\20"),0, XSTRINGCONTEXT_ATEND);


                                                XFILEXMLELEMENT* element;

                                                if(!root)
                                                  {
                                                    element = new XFILEXMLELEMENT();
                                                    if(!element) return false;
                                                    root          = element;
                                                    actualelement = root;
                                                  }
                                                 else element = actualelement->AddElement();

                                                if(!element) return false;

                                                element->SetType(XFILEXMLELEMENTTYPE_NORMAL);
                                                element->SetName(name);

                                                if(string.Copy(name.Get(), (const XCHAR*)NULL, false, 0, values)!=XSTRING_NOTFOUND)
                                                  {
                                                    int indexname = 0;

                                                    while(1)
                                                      {
                                                        indexname = values.Copy(indexname, __L("="), false, name);
                                                        if(indexname==XSTRING_NOTFOUND) break;

                                                        name.DeleteNoCharacters(XSTRINGCONTEXT_ALLSTRING);

                                                        indexname = values.Copy(__L("\""),__L("\""),false,indexname,value);
                                                        if(indexname==XSTRING_NOTFOUND) break;

                                                        element->AddAtribute(name,value);

                                                        name.Empty();
                                                        value.Empty();

                                                      }
                                                  }

                                                if(typeline==XFILEXMLELEMENTTYPELINE_START) actualelement = element;

                                              }
                                              break;

      case XFILEXMLELEMENTTYPELINE_END      : if(actualelement->GetFather()) actualelement = actualelement->GetFather();
                                              break;
    }

  return true;
}



/*-------------------------------------------------------------------
//  XFILEXML::AddTabs
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      14/02/2011 22:44:23
//
//  @return       bool :
//  @param        level :
//  @param        spacetabs :
//  @param        string :
*/
/*-----------------------------------------------------------------*/
bool XFILEXML::AddTabs(int level,int spacetabs,XSTRING& string)
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
//  XFILEXML::EncodeConfig
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      14/02/2011 9:57:31
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XFILEXML::EncodeConfig(XSTRING& string)
{
  string.Empty();

  string += __L("<?xml ");

  for(int c=0;c<(int)cfgattributes.GetSize();c++)
    {
      XFILEXMLATTRIBUTE* attribute = (XFILEXMLATTRIBUTE*)cfgattributes.Get(c);
      if(!attribute) break;

      string+=attribute->GetName();
      string+=__L("=");
      string+=__L("\"");
      string+=attribute->GetValue();
      string+=__L("\" ");
    }

  string += __L("?>");

  return true;
}




/*-------------------------------------------------------------------
//  XFILEXML::EncodeElements
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      15/10/2012 9:09:58
//
//  @return       bool :
//  @param        istabulatedline :
//  @param        element :
*/
/*-----------------------------------------------------------------*/
bool XFILEXML::EncodeElements(bool istabulatedline, XFILEXMLELEMENT* element)
{
  if(!element) return false;

  XSTRING string;
  XSTRING value  = element->GetValue();
  int     c;

  string.Empty();
  if(istabulatedline)
    {
      encodelevel++;
      AddTabs(encodelevel,XFILEXML_SPACETABS,string);
    }

  switch(element->GetType())
    {
      case XFILEXMLELEMENTTYPE_UNKNOWN    : break;

      case XFILEXMLELEMENTTYPE_CONFIG   : break;

      case XFILEXMLELEMENTTYPE_COMMENT  : { string += __L("<!--");
                                            string += value;
                                            string += __L("-->");
                                            AddLine(string);
                                            //XDEBUGTRACE_PRINTCOLOR(1,string.Get());
                                          }
                                          break;

      case XFILEXMLELEMENTTYPE_CDATA    : { string += __L("<![CDATA[");
                                            string += value;
                                            string += __L("]]>");
                                            AddLine(string);
                                            //XDEBUGTRACE_PRINTCOLOR(1,string.Get());
                                          }
                                          break;

      case XFILEXMLELEMENTTYPE_NORMAL   : { bool isclose = false;

                                            string += __L("<");
                                            string += element->GetName();
                                            if(element->GetNAttributes()) string += __L(" ");

                                            for(c=0;c<(int)element->GetNAttributes();c++)
                                              {
                                                XFILEXMLATTRIBUTE* attribute = (XFILEXMLATTRIBUTE*)element->GetAttribute(c);
                                                if(!attribute) break;

                                                string+=attribute->GetName();
                                                string+=__L("=");
                                                string+=__L("\"");
                                                string+=attribute->GetValue();
                                                string+=__L("\"");
                                                if(c!=element->GetNAttributes()-1) string+=__L(" ");
                                              }

                                            if(!element->GetNElements() && value.IsEmpty())
                                              {
                                                string +=__L("/>");
                                                isclose = true;
                                              }
                                             else string +=__L(">");

                                            AddLine(string);
                                            //XDEBUGTRACE_PRINTCOLOR(1,string.Get());

                                            if(!value.IsEmpty())
                                              {
                                                string.Empty();
                                                if(istabulatedline) AddTabs(encodelevel+1,XFILEXML_SPACETABS,string);
                                                string+=value;

                                                AddLine(string);
                                                //XDEBUGTRACE_PRINTCOLOR(1,string.Get());
                                              }

                                            for(c=0;c<(int)element->GetNElements();c++)
                                              {
                                                XFILEXMLELEMENT* subelement = element->GetElement(c);
                                                if(subelement)  EncodeElements(istabulatedline,subelement);
                                              }

                                            if(!isclose)
                                              {
                                                string.Empty();

                                                if(istabulatedline) AddTabs(encodelevel,XFILEXML_SPACETABS,string);

                                                string += __L("</");
                                                string += element->GetName();
                                                string += __L(">");

                                                AddLine(string);
                                                //XDEBUGTRACE_PRINTCOLOR(1,string.Get());
                                              }
                                          }
                                          break;


    }

  if(istabulatedline) encodelevel--;

  return true;
}


/*-------------------------------------------------------------------
//  XFILEXML2::ReadAllFile
*/
/**
//
//  Read all lines in one string
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      11/01/2017 16:36:23
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool  XFILEXML2::ReadAllFile()
{

  if(!file)           return false;
  if(!file->IsOpen()) return false;

  DeleteAllLines();

  XDWORD             sizeBOM              = 0;
  XFILETXTFORMATCHAR formatchar           = GetFormatCharFromFile(&sizeBOM);
  int                sizebytescharacter   = SizeOfCharacter(formatchar);

  if(this->formatchar==XFILETXTFORMATCHAR_UNKNOWN) this->formatchar = formatchar;

  file->SetPosition(sizeBOM);


  XDWORD filesize=file->GetSize();

  XDWORD    br;
  XBUFFER   dataline(false);

  XBYTE*    readbuffer = new XBYTE[filesize];
  if(!readbuffer) return false;

  memset(readbuffer, 0, filesize);

  XDWORD bufferpos = 0;

  br = filesize;
  file->Read(readbuffer, &br);
  if(!br) return false;

  do{ XFILETXTTYPELF  _typeLF   = XFILETXTTYPELF_UNKNOWN;
  
      if(typeLF == XFILETXTTYPELF_UNKNOWN && _typeLF != XFILETXTTYPELF_UNKNOWN) typeLF = _typeLF;


      XDWORD lack = (br-bufferpos);
      dataline.Add(&readbuffer[bufferpos], lack);
      bufferpos += lack;

      if(bufferpos == br)
        {
          AddLine(formatchar, dataline.Get(), (int)(lack/sizebytescharacter));
          dataline.Delete();
        }

    } while(bufferpos < br);

  delete [] readbuffer;

  return true;
}




/*-------------------------------------------------------------------
//  XFILEXML2::ReadAndDecodeAllLines
*/
/**
//
//  Reads all lines and decodes them
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      11/01/2017 16:36:05
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool  XFILEXML2::ReadAndDecodeAllLines()
{

  if(!file)           return false;
  if(!file->IsOpen()) return false;

  XTIMER* timer=xfactory->CreateTimer();


    ReadAllFile();

    DeleteAllElements();

    XSTRING   string;
    XSTRING*  stringml=new XSTRING();
    bool      quote      =  false;
    bool      cdata      =  false;

    string.Empty();

    if (this->lines.GetSize()==0)
      return false;

    stringml->Set(this->lines.Get(0)->Get());

        for(int d=0;d<(int)stringml->GetSize();d++)
          {
            XCHAR letter[2] = { 0, 0 };
            letter[0] = (*stringml)[d];

            if((*stringml)[d]==__C('\"')) quote=!quote;

            if((*stringml)[d]==__C('<')&&(!quote)&&((*stringml)[d+1]==__C('!'))&&((*stringml)[d+2]==__C('-'))&&((*stringml)[d+3]==__C('-')))
            {
              int n=stringml->Find(__L("-->"),true,d);
              if (n!=NOTFOUND)
                  d=n+3;
              continue;
            }

            if((*stringml)[d]==__C('<')&&(!quote))
              {
                if(string.Find(__L("[CDATA["),false)!=XSTRING_NOTFOUND) cdata=true;

                if(!string.IsEmpty() && string.HaveCharacters() && (!cdata))
                  {
                    DecodeLine(string,false);
                    string.Empty();
                  }

                quote = false;
                string.Add(letter[0]);//
              }
              else
              {
                if(((*stringml)[d]==__C('>'))&&(!quote))
                  {
                    if(string.Find(__L("]]"),false)!=XSTRING_NOTFOUND) cdata=false;

                    string.Add(letter[0]);//
                    if(!string.IsEmpty() && string.HaveCharacters() && (!cdata))
                      {
                        DecodeLine(string,true);
                        string.Empty();
                      }
                  }
                  else
                    string.Add(letter[0]);//
              }
          }


    DeleteAllLines();
    delete(stringml);

   XDEBUGTRACE_PRINTCOLOR(0,__L("XML Decoded in %d"),timer->GetMeasureMilliSeconds());
    delete(timer);
    return true;
}

