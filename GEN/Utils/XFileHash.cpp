//------------------------------------------------------------------------------------------
//  XFILEHASH.CPP
//
//  File HASH funtions
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 22/04/2002
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include "XBuffer.h"

#include "XFileHash.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  XFILEHASH::XFILEHASH
/**
//
//
//  @author       Abraham J. Velez
//  @version      22/04/2002 12:04:07
//
//  @return
//  @param    factory :
//  */
//-------------------------------------------------------------------
XFILEHASH::XFILEHASH(HASH* hash) : XFILECONTAINER()
{
  Clean();

  this->hash = hash;

  if(!CreatePrimaryFile()) return;
}



//-------------------------------------------------------------------
//  XFILEHASH::~XFILEHASH
/**
//
//
//  @author       Abraham J. Velez
//  @version      22/04/2002 12:04:42
//
//  @return       void :
//  */
//-------------------------------------------------------------------
XFILEHASH::~XFILEHASH()
{
  Close();

  if(!DeletePrimaryFile()) return;

  Clean();
}



/*-------------------------------------------------------------------
//  XFILEHASH::Open
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      30/08/2008 08:50:48 p.m.
//
//  @return       bool :
//  @param        pathname :
//  @param        readonly :
//  @param        checkhash :
//  @param        checkversion :
*/
/*-----------------------------------------------------------------*/
bool XFILEHASH::Open(XPATH& pathname, bool readonly, bool checkhash, bool checkversion)
{
  if(!hash) return false;
  if(!file) return false;

  if(!hash->GetDefaultSize()) return false;

  if(file->Exist(pathname)!=true) return false;

  bool status = file->Open(pathname,readonly);

  XSTRING IDini(16);
  XWORD   _ID      = 0;
  XWORD   _type    = 0;
  XWORD   _version = 0;

  if(!file->Read((XBYTE*)IDini.Get(), XSTRING::GetSize(XFILE_ID)*sizeof(XWORD)+1)) return false;

  IDini.AdjustSize();

  if(!file->Read((XBYTE*)&_ID, sizeof(XWORD))) return false;
  if(_ID!=ID) return false;

  if(!file->Read((XBYTE*)&_type, sizeof(XWORD))) return false;
  if(_type!=type) return false;

  if(!file->Read((XBYTE*)&_version,sizeof(XWORD))) return false;
  if(checkversion)
    {
      if(version!=_version)  return false;
    }

  version = _version;

  XDWORD sizeIDstring = 0;

  if(!file->Read((XBYTE*)&sizeIDstring, sizeof(XDWORD))) return false;
  if(sizeIDstring)
    {
      XWORD* IDstr = new XWORD[sizeIDstring+1];
      if(IDstr)
        {
          memset((XBYTE*)IDstr, 0, sizeof(XWORD)*(sizeIDstring+1));

          if(!file->Read((XBYTE*)IDstr, sizeof(XWORD)*sizeIDstring))
            {
              delete [] IDstr;
              return false;
            }

          IDstring.Set((XWORD*)IDstr);

          delete [] IDstr;
        }
    }

  XBUFFER hashbuffer((XDWORD)hash->GetDefaultSize(), true);
  if(!file->Read((XBYTE*)hashbuffer.Get(), hash->GetDefaultSize())) return false;

  file->GetPosition(dataposition);

  XDWORD size =  file->GetSize() - GetDataPosition();

  hash->ResetResult();

  if(!hash->Do(file, size, GetDataPosition())) return false;

  if(checkhash)
    {
      if(!hashbuffer.Compare(hash->GetResult()))
        {
          return false;
        }
    }

  if(readonly) hashisupdate = false;

  file->SetPosition(dataposition);

  return status;
}




//-------------------------------------------------------------------
//  XFILEHASH::Create
/**
//
//
//  @author       Abraham J. Velez
//  @version      10/01/2001 17:11:16
//
//  @return       bool :
//  @param        pathname :
//  @param        checkHASH :
*/
//-------------------------------------------------------------------
bool XFILEHASH::Create(XPATH& pathname, bool checkHASH)
{
  if(!hash) return false;
  if(!file) return false;

  if(!hash->GetDefaultSize()) return false;

  bool status=file->Create(pathname);

  hash->ResetResult();

  XBUFFER hashbuffer((XDWORD)hash->GetDefaultSize(), true);

  XDWORD sizeIDstring  = IDstring.GetSize();

  XSTRING IDini;

  IDini = XFILE_ID;

  XSTRING_CREATENORMALIZE(IDini, buffnormalize, false)
  if(!file->Write((XBYTE*)buffnormalize, IDini.GetSize()*sizeof(XWORD)+1)) return false;
  XSTRING_DELETENORMALIZE(buffnormalize)

  type |= XFILE_FEATURES_HASH;

  if(!file->Write((XBYTE*)&ID               , sizeof(XWORD)))   return false;
  if(!file->Write((XBYTE*)&type             , sizeof(XWORD)))   return false;
  if(!file->Write((XBYTE*)&version          , sizeof(XWORD)))   return false;
  if(!file->Write((XBYTE*)&sizeIDstring     , sizeof(XDWORD)))  return false;
  if(sizeIDstring)
    {
      XSTRING_CREATENORMALIZE(IDstring, buffnormalize, false)
      if(!file->Write((XBYTE*)buffnormalize, sizeof(XWORD)*sizeIDstring))   return false;
      XSTRING_DELETENORMALIZE(buffnormalize)
    }

  if(!file->Write((XBYTE*)hashbuffer.Get()  , hash->GetDefaultSize()))  return false;

  file->GetPosition(dataposition);

  hashisupdate = false;

  return status;
}


//-------------------------------------------------------------------
//  XFILEHASH::Close
/**
//
//
//  @author       Abraham J. Velez
//  @version      10/01/2001 17:12:38
//
//  @return       bool :
*/
//-------------------------------------------------------------------
bool XFILEHASH::Close()
{
  if(!file)           return false;
  if(!file->IsOpen()) return false;

  file->Flush();

  if(!hashisupdate) UpdateHash();

  file->Close();

  return true;
}




/*-------------------------------------------------------------------
//  XFILEHASH::GetID
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/04/2013 2:53:22
//
//  @return       XWORD :
//  */
/*-----------------------------------------------------------------*/
XWORD XFILEHASH::GetID()
{
  return ID;
}



/*-------------------------------------------------------------------
//  XFILEHASH::GetType
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      24/04/2013 23:31:47
//
//  @return       XWORD :
//  */
/*-----------------------------------------------------------------*/
XWORD XFILEHASH::GetType()
{
  return type;
}



/*-------------------------------------------------------------------
//  XFILEHASH::GetVersion
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      30/08/2008 08:29:03 p.m.
//
//  @return       XWORD :
//  */
/*-----------------------------------------------------------------*/
XWORD XFILEHASH::GetVersion()
{
  return version;
}


/*-------------------------------------------------------------------
//  XFILEHASH::GetIDString
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/04/2013 2:50:19
//
//  @return       XSTRING* :
//  */
/*-----------------------------------------------------------------*/
XSTRING* XFILEHASH::GetIDString()
{
  return &IDstring;
}



/*-------------------------------------------------------------------
//  XFILEHASH::Set
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      24/04/2013 23:27:07
//
//  @return       bool :
//  @param        ID :
//  @param        type :
//  @param        version :
//  @param        IDstring :
*/
/*-----------------------------------------------------------------*/
bool XFILEHASH::Set(XWORD ID, XWORD type, XWORD version, XCHAR* IDstring)
{
  SetID(ID);
  SetType(type);
  SetVersion(version);
  SetIDString(IDstring);

  return true;
}



/*-------------------------------------------------------------------
//  XFILEHASH::SetID
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      24/04/2013 23:28:44
//
//  @return       bool :
//  @param        ID :
*/
/*-----------------------------------------------------------------*/
bool XFILEHASH::SetID(XWORD ID)
{
  this->ID = ID;

  return true;
}



/*-------------------------------------------------------------------
//  XFILEHASH::SetType
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      24/04/2013 23:29:00
//
//  @return       bool :
//  @param        type :
*/
/*-----------------------------------------------------------------*/
bool XFILEHASH::SetType(XWORD type)
{
  this->type = type;

  return true;
}



/*-------------------------------------------------------------------
//  XFILEHASH::SetVersion
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      24/04/2013 23:29:23
//
//  @return       bool :
//  @param        version :
*/
/*-----------------------------------------------------------------*/
bool XFILEHASH::SetVersion(XWORD version)
{
  this->version = version;

  return true;
}



/*-------------------------------------------------------------------
//  XFILEHASH::SetIDString
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      24/04/2013 23:30:26
//
//  @return       bool :
//  @param        IDstring :
*/
/*-----------------------------------------------------------------*/
bool XFILEHASH::SetIDString(XCHAR* IDstring)
{
  this->IDstring = IDstring;

  return true;
}




/*-------------------------------------------------------------------
//  XFILEHASH::SetIDString
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      24/04/2013 23:30:58
//
//  @return       bool :
//  @param        IDstring :
*/
/*-----------------------------------------------------------------*/
bool XFILEHASH::SetIDString(XSTRING& IDstring)
{
  this->IDstring = IDstring;

  return true;
}




/*-------------------------------------------------------------------
//  XFILEHASH::GetDataPosition
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/04/2013 4:05:18
//
//  @return       int :
//  */
/*-----------------------------------------------------------------*/
int XFILEHASH::GetDataPosition()
{
  return dataposition;
}




/*-------------------------------------------------------------------
//  XFILEHASH::UpdateHash
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/04/2013 1:37:34
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XFILEHASH::UpdateHash()
{
  if(!hash) return false;
  if(!file) return false;

  file->Flush();

  hash->ResetResult();

  XDWORD size =  file->GetSize() - GetDataPosition();

  if(!hash->Do(file, size, GetDataPosition())) return false;

  file->SetPosition(dataposition-hash->GetDefaultSize());

  if(!file->Write((XBYTE*)hash->GetResult()->Get(), hash->GetDefaultSize()))    return false;

  hashisupdate = true;

  return false;
}


