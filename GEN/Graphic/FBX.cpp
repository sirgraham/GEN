
/*------------------------------------------------------------------------------------------
//  FBX.CPP
//
//  Reads FBX Contents
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 17/02/2016 10:16:30
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "fbx.h"
#include "XPath.h"
#include "XFile.h"
#include "XFactory.h"
#include "XDebug.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/

/*-------------------------------------------------------------------
//  FBX::FBX
*/
/**
//
//  Class Constructor FBX
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      17/02/2016 10:17:21
//
*/
/*-----------------------------------------------------------------*/
FBX::FILE::FILE()
{
  Clean();
}

/*-------------------------------------------------------------------
//  FBX::~FBX
*/
/**
//
//   Class Destructor FBX
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      17/02/2016 10:17:25
//
*/
/*-----------------------------------------------------------------*/
FBX::FILE::~FILE()
{
  Clean();
}

/*-------------------------------------------------------------------
//  Load
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      17/02/2016 10:17:29
//
//  @return       GRPOBJECT* :
//
//  @param        file :
*/
/*-----------------------------------------------------------------*/
GRPOBJECT*  FBX::FILE::Load(XCHAR* filename)
{
  XPATH xpath;
  xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, filename);

  XFILE* file;
  file=xfactory->Create_File();

  bool r=file->Open(xpath,true);

  if (!r)
  {
    XDEBUG_PRINTCOLOR(4,__L("Error loading data"));
  }

  XBUFFER buffer;
  buffer.Resize(file->GetSize());
  if (!file->Read(buffer.Get(),file->GetSize()))
  {
    FBXDEBUG(4,__L("FBX::FILE::Load : Error opening file %s"),filename);
  }

  this->filesize=file->GetSize();

  data.AdjustSize(filesize);
  data.Set(buffer);

  ParseDataBlocks();


  return NULL;
}





/*-------------------------------------------------------------------
//  FBX::FILE::ParseData
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      17/02/2016 10:31:58
//
*/
/*-----------------------------------------------------------------*/
void    FBX::FILE::ParseDataBlocks()
{
  XSTRING word;
  while (ReadWord(word))
  {
    if (word.Get()[0]==__C('{'))
      SkipBlock();
  }
}


/*-------------------------------------------------------------------
//  FBX::FILE::ReadWord
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      06/04/2016 12:43:29
//
//  @return       bool :
//
//  @param        s :
*/
/*-----------------------------------------------------------------*/
bool    FBX::FILE::ReadWord(XSTRING& s)
{
  XDWORD end=cursor;
  while (end<filesize)
  {
    XCHAR c=data.Get()[end];
    if (c==';')   end=SkipLine();
    if (c==' ')   break;
    if (c=='\n')  break;
    if (c=='\r')  break;
    end++;
  }

  if (end>=filesize) return false;

  data.Copy(cursor,end,s);

  cursor=end+1;

  return true;
}

/*-------------------------------------------------------------------
//  FBX::FILE::SkipBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      06/04/2016 13:02:30
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool    FBX::FILE::SkipBlock()
{
  int nbrackets=0;
  while (cursor<this->filesize)
  {
    XCHAR c=this->data.Get()[cursor];
    if (c==__C('{')) nbrackets++;
    if (c==__C('}')) { nbrackets--; cursor++; if (nbrackets==0) return true; }
    cursor++;
  }

  return false;
}


/*-------------------------------------------------------------------
//  FBX::FILE::SkipLine
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      06/04/2016 13:34:21
//
//  @return       XDWORD :
//
*/
/*-----------------------------------------------------------------*/
XDWORD      FBX::FILE::SkipLine()
{
  //while (cursor<this->filesize);
  return 0;
}

/*-------------------------------------------------------------------
//  FBX::FILE::ReadLine
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      17/02/2016 10:35:52
//
*/
/*-----------------------------------------------------------------*/
bool    FBX::FILE::ReadLine()
{
return false;
}

/*-------------------------------------------------------------------
//  FBX::FILE::Clean
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      17/02/2016 10:24:07
//
*/
/*-----------------------------------------------------------------*/
void    FBX::FILE::Clean()
{
  cursor=0;
}