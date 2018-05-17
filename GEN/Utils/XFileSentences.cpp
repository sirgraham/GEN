//------------------------------------------------------------------------------------------
//  XFILESENTENCES.CPP
//
//  XFile Sentences Class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 25/05/2004 18:55:54
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include <stdlib.h>
#include <string.h>

#include "XBuffer.h"
#include "XFileDBF.h"
#include "XFactory.h"

#include "XFileSentences.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  XSENTENCE::XSENTENCE
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/05/2004 19:08:46
//
//  @return
//  */
//-------------------------------------------------------------------
/*
XSENTENCE::XSENTENCE()
{
  ID  = 0;
  str = NULL;
}
*/


//-------------------------------------------------------------------
//  XFILESENTENCES::XFILESENTENCES
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/05/2004 19:08:46
//
//  @return
//  */
//-------------------------------------------------------------------
XFILESENTENCES::XFILESENTENCES(HASH* hash, XPATH& xpath) : XFILEXDB(hash, xpath)
{
  Set(XFILE_SEN_ID, XFILESENTENCES_TYPE, XFILESENTENCES_VERSION, XFILESENTENCES_IDSTRING);
}


//-------------------------------------------------------------------
//  XFILESENTENCES::~XFILESENTENCES
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/05/2004 19:09:18
//
//  @return       void :
//  */
//-------------------------------------------------------------------
XFILESENTENCES::~XFILESENTENCES()
{
}



//-------------------------------------------------------------------
//  XFILESENTENCES::ConvertFileFromDBF
/**
//
//
//  @author       Abraham J. Velez
//  @version      26/05/2004 10:31:31
//
//  @return       bool :
//  @param        path :
*/
//-------------------------------------------------------------------
bool XFILESENTENCES::ConvertFileFromDBF(XPATH& xpath)
{
  XFILEDBF*    filedbf;
  XDWORD c;

  if(!xfactory) return false;

  CloseFile();

  filedbf = new XFILEDBF();
  if(!filedbf) return false;

  if(!filedbf->Open(xpath))
    {
      delete filedbf;
      return false;
    }

  XFILEDBF_HEADER*  header = filedbf->GetHeader();

  if((!header)||(!header->Load(filedbf->GetPrimaryFile())))
    {
      delete filedbf;
      return false;
    }

  XDWORD nrecords = header->GetNRecords();

  if(!nrecords)
    {
      delete filedbf;
      return false;
    }

  XDWORD initablepos;

  Create(this->xpath);

  GetPrimaryFile()->Write((XBYTE*)&nrecords,sizeof(XDWORD));

  GetPrimaryFile()->GetPosition(initablepos);

  for(c=0;c<nrecords;c++)
    {
      XDWORD idx=0;
      for(int d=0;d<2;d++)
        {
          GetPrimaryFile()->Write((XBYTE*)&idx,sizeof(XDWORD));
        }
    }

  XDWORD  index;
  XDWORD  fpos  = 0;

  for(c=0;c<nrecords;c++)
    {
      XFILEDBF_RECORD* record    = filedbf->ReadRecord(c);
      XSTRING*         fsentence = new XSTRING(_MAXSTR);

      if(fsentence)
        {
          fsentence->FillChar();
          memcpy(fsentence->Get(),(char*)record->GetData(0),record->GetSizeField(0));
          index = fsentence->ConvertToInt();

          fsentence->FillChar();
          memcpy(fsentence->Get(),(char*)record->GetData(1),record->GetSizeField(1));

          fsentence->DeleteCharacter(__C(' '),XSTRINGCONTEXT_ATEND);

          GetPrimaryFile()->GetPosition(fpos);

          indexmap.Add(index,fpos);

          XDWORD size = (XDWORD)fsentence->GetSize();
          GetPrimaryFile()->Write((XBYTE*)&size,sizeof(XDWORD));

          XSTRING_CREATENORMALIZE((*fsentence), buffnormalize, false)
          GetPrimaryFile()->Write((XBYTE*)buffnormalize, (fsentence->GetSize()+1)*sizeof(XWORD));
          XSTRING_DELETENORMALIZE(buffnormalize)

          delete fsentence;
        }

      delete record;
    }

  GetPrimaryFile()->SetPosition(initablepos);

  XDWORD ifilepos = 0;
  XDWORD  filepos  = 0;

  for(XDWORD c=0;c<nrecords;c++)
    {
      ifilepos = indexmap.GetKey(c);
      filepos  = indexmap.GetElement(c);

      GetPrimaryFile()->Write((XBYTE*)&ifilepos ,sizeof(XDWORD));
      GetPrimaryFile()->Write((XBYTE*)&filepos   ,sizeof(XDWORD));
    }

  filedbf->Close();
  delete filedbf;

  CloseFile();

  return OpenFile();
}



//-------------------------------------------------------------------
//  XFILESENTENCES::GetNumberSentences
/**
//
//
//  @author       Abraham J. Velez
//  @version      27/05/2004 17:50:11
//
//  @return       int :
//  */
//-------------------------------------------------------------------
int XFILESENTENCES::GetNumberSentences()
{
  return GetNumberRecords();
}



/*-------------------------------------------------------------------
//  XFILESENTENCES::GetSentence
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      06/05/2011 11:56:11
//
//  @return       bool :
//  @param        index :
//  @param        sentence :
*/
/*-----------------------------------------------------------------*/
bool XFILESENTENCES::GetSentence(XDWORD index,XSTRING& sentence)
{
  sentence.Empty();

  XBUFFER* xbuffer = GetRecord(index);
  if(!xbuffer) return false;

  sentence.Set((XWORD*)xbuffer->Get());

  delete xbuffer;

  return true;
}




