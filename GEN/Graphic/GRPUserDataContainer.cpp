/*------------------------------------------------------------------------------------------
//  GRPUSERDATACONTAINER.CPP
*/
/**
// \class
//
//  Container for user data
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 24/06/2015 13:46:27
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPUserDataContainer.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

/*---- CLASS -----------------------------------------------------------------------------*/

/*-------------------------------------------------------------------
//  GRPUSERDATACONTAINER::GRPUSERDATACONTAINER
*/
/**
//
//  Class Constructor GRPUSERDATACONTAINER
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      16/05/2017 15:56:25
//
*/
/*-----------------------------------------------------------------*/
GRPUSERDATACONTAINER::GRPUSERDATACONTAINER()
{
  Clear();
}

/*-------------------------------------------------------------------
//  GRPUSERDATACONTAINER::~GRPUSERDATACONTAINER
*/
/**
//
//   Class Destructor GRPUSERDATACONTAINER
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      16/05/2017 15:56:48
//
*/
/*-----------------------------------------------------------------*/
GRPUSERDATACONTAINER::~GRPUSERDATACONTAINER()
{
  Clear();
}

/*-------------------------------------------------------------------
//  GRPUSERDATACONTAINER::GetSize
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      16/05/2017 15:58:44
//
//  @return       XDWORD :
//
*/
/*-----------------------------------------------------------------*/
XDWORD GRPUSERDATACONTAINER::GetSize()
{
  return this->UserData.GetSize();
}

/*-------------------------------------------------------------------
//  GRPUSERDATACONTAINER::Get
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      16/05/2017 15:58:39
//
//  @return       XMAP<XSTRING*,XSTRING*>* :
//
*/
/*-----------------------------------------------------------------*/
XMAP<XSTRING*,XSTRING*>*  GRPUSERDATACONTAINER::Get()
{
  return &this->UserData;
}

/*-------------------------------------------------------------------
//  GRPUSERDATACONTAINER::GetUserDataByKey
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      16/05/2017 15:58:35
//
//  @return       XSTRING* :
//
//  @param        k :
*/
/*-----------------------------------------------------------------*/
XSTRING* GRPUSERDATACONTAINER::GetUserDataByKey(XCHAR* k)
{
  XDWORD n = UserData.GetSize();
  for (XDWORD e=0;e<n;e++)
  {
    if (UserData.GetKey(e)->Compare(k)==0)
      return UserData.GetElement(e);
  }
  return NULL;
}

/*-------------------------------------------------------------------
//  GRPUSERDATACONTAINER::Add
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      16/05/2017 15:58:29
//
//  @param        k :
//  @param        v :
*/
/*-----------------------------------------------------------------*/
void GRPUSERDATACONTAINER::Add(XCHAR* k, XCHAR* v)
{
  XSTRING* key=new XSTRING();
  if (key)
  {
    XSTRING* value=new XSTRING();
    if (value)
    {
    key->Set(k);
    value->Set(v);
    if  (!this->UserData.Set(key,value))
          this->UserData.Add(key,value);
    }
    else
      delete key;
  }
}

/*-------------------------------------------------------------------
//  GRPUSERDATACONTAINER::SetUserData
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      16/05/2017 17:09:25
//
//  @param        dc :
*/
/*-----------------------------------------------------------------*/
void GRPUSERDATACONTAINER::SetUserData(GRPUSERDATACONTAINER* dc)
{
  this->Clear();
  for (XDWORD e=0;e<dc->GetSize();e++)
  {
    this->Add(dc->Get()->GetKey(e)->Get(),dc->Get()->GetElement(e)->Get());
  }
}


/*-------------------------------------------------------------------
//  GRPUSERDATACONTAINER::Clear
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      16/05/2017 15:58:23
//
*/
/*-----------------------------------------------------------------*/
void GRPUSERDATACONTAINER::Clear()
{
  UserData.DeleteKeyContents();
  UserData.DeleteElementContents();
  UserData.DeleteAll();
}


