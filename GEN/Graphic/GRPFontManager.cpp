
/*------------------------------------------------------------------------------------------
//  GRPFONTMANAGER.CPP
//
//  sdsad
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 18/08/2014 14:20:12
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPFileTTF.h"
#include "GRPFontManager.h"
#include "XDir.h"
#include "XFactory.h"
#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/




/*-------------------------------------------------------------------
//  GRPFONTMANAGER::GRPFONTMANAGER
*/
/**
//
//  Class Constructor GRPFONTMANAGER
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      18/08/2014 15:59:38
//
*/
/*-----------------------------------------------------------------*/
GRPFONTMANAGER::GRPFONTMANAGER()
{
  Clean();

}




/*-------------------------------------------------------------------
//  GRPFONTMANAGER::~GRPFONTMANAGER
*/
/**
//
//   Class Destructor GRPFONTMANAGER
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      18/08/2014 15:59:49
//
*/
/*-----------------------------------------------------------------*/
GRPFONTMANAGER::~GRPFONTMANAGER()
{
  fonts.DeleteKeyContents();
  fonts.DeleteElementContents();
  fonts.DeleteAll();

  for (XDWORD e = 0; e < families.GetSize(); e++)
      families.GetElement(e)->DeleteElementContents();

  families.DeleteElementContents();
  families.DeleteKeyContents();
  families.DeleteAll();

  Clean();
}

/*-------------------------------------------------------------------
//   GRPFONTMANAGER::Init
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  25/01/2018 16:54:01
//  @return   bool :
//
//  @param     :
//
*//*-----------------------------------------------------------------*/
bool GRPFONTMANAGER::Init()
{
    XPATH xpath;
    XPATHSMANAGER::GetInstance().GetPathOfSection(XPATHSMANAGERSECTIONTYPE_FONTS, xpath);

    XDIR* dir = xfactory->Create_Dir();
    XDIRELEMENT search;
    bool   continuesearch = dir->FirstSearch(xpath.Get(), __L("*.ttf"), &search);
    while (continuesearch)
    {
        XPATH fontfile;
              fontfile.Create(XPATHSMANAGERSECTIONTYPE_FONTS, 1, search.GetNameFile()->Get());

        if (!this->IndexFont(fontfile.Get()))
            return false;

        continuesearch=dir->NextSearch(&search);
    }

    delete(dir);
    return true;
}


/*-------------------------------------------------------------------
//  GRPFONTMANAGER::Load
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      18/08/2014 16:05:51
//
//  @return       bool :
//

//  @param        xpath :
//  @param        name :
//  @param        attributes :
*/
/*-----------------------------------------------------------------*/
bool GRPFONTMANAGER::Load(XCHAR* filename, XCHAR* fontname, GRPFONTATTRIBUTES* fontattributes)
{
  if (fontname!=NULL)
  {
    for (XDWORD e=0;e<fonts.GetSize();e++)
    {
      if (fonts.GetKey(e)->Compare(fontname)==0)
        return true;
    }
  }

  GRPFONT* font=new GRPFONT();
  if(!font)
    {
      lasterror= GRPFONTSTATUS_ERROR_FAILEDMEMORY;
      return false;
    }

  GRPFONTATTRIBUTES *attributes = NULL;
  GRPFONTATTRIBUTES tmp;

    if (fontattributes!=NULL)
    {
      attributes=fontattributes;
    }
    else
    {
      attributes = &tmp;
    }

  XSTRING name;

  if (fontname!=NULL)
    name=fontname;
  else
    name=filename;

  if(!font->LoadTTF(filename))
    {
      lasterror=font->GetLastError();
      delete(font);
      font=NULL;
      return false;
    }

  font->SetAttributes(attributes);

  XSTRING* namefont = new XSTRING(name);
  if(!namefont)
    {
      lasterror= GRPFONTSTATUS_ERROR_FAILEDMEMORY;
      delete(font);
      font=NULL;
      return false;
    }

  XDEBUG_PRINTCOLOR(1,__L("[%d] Font loaded: %s"),fonts.GetSize(),name.Get());
  font->SetName(namefont->Get());
  fonts.Add(namefont, font);
  return true;
}



/*-------------------------------------------------------------------
//  GRPFONTMANAGER::GetFont
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      18/08/2014 16:07:44
//
//  @return       GRPFONT* :
//
//  @param        name :
*/
/*-----------------------------------------------------------------*/
GRPFONT* GRPFONTMANAGER::GetFont(const XCHAR* name)
{
  for(XDWORD e=0; e<fonts.GetSize(); e++)
    {
      if(!fonts.GetKey(e)->Compare(name)) return fonts.GetElement(e);
    }

  lasterror = GRPFONTSTATUS_ERROR_FILENOTFOUND;

  return NULL;
}

/*-------------------------------------------------------------------
//   GRPFONTMANAGER::GetFontByFamily
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  26/01/2018 10:43:27
//  @return   GRPFONT* :
//
//  @param    XCHAR* :
//  @param    GRPFONT_STYLE :
//
*//*-----------------------------------------------------------------*/
GRPFONT* GRPFONTMANAGER::GetFontByFamily(XCHAR* familyname,GRPFONT_STYLE style)
{
    XDWORD size = this->families.GetSize();
    for (XDWORD e = 0; e < size; e++)
        if (this->families.GetKey(e)->Compare(familyname, true) == 0)
        {
            GRPFONT*  font= this->families.GetElement(e)->Get(style);           //we have the font
            if (!font)
                font = this->families.GetElement(e)->GetElement(0);
            return font;            //we have the font
        }

    return NULL;
}

/*-------------------------------------------------------------------
//   GRPFONTMANAGER::IndexFont
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  26/01/2018 10:21:01
//  @return   bool :
//
//  @param    XCHAR* :
//
*//*-----------------------------------------------------------------*/
bool GRPFONTMANAGER::IndexFont(XCHAR* filename)
{
    GRPFONT* font = new GRPFONT();

    if (!font->LoadTTF(filename))
    {
        delete(font);
        return false;
    }

    if (!font->GetTTF()->Init())
    {
        delete(font);
        return false;
    }

    XSTRING family_name  = font->GetTTF()->GetString(GRPFILETTF_STRINGID_FAMILY);
    XSTRING family_style = font->GetTTF()->GetString(GRPFILETTF_STRINGID_STYLE);

    GRPFONT_STYLE style=font->SetStyle(family_style.Get());

    XDEBUG_PRINTCOLOR(0, __L("FontManager: Indexing font Family(%s) style(%s)"), family_name.Get(), family_style.Get());

    for (XDWORD e = 0; e < this->families.GetSize(); e++)
    {
        if (this->families.GetKey(e)->Compare(family_name.Get()) == 0)
        {
                GRPFONTFAMILY* f = this->families.GetElement(e);
                if (!f->Add(style, font))
                    return false;
                return true;
        }
    }
    //--- not found
    GRPFONTFAMILY* family = new GRPFONTFAMILY();
                   family->Add(style, font);
    XSTRING*       name = new XSTRING();
                   name->Set(family_name);

                   if (!this->families.Set(name, family))
                   if (!this->families.Add(name, family))
                   {
                       delete(name);
                       delete(family);
                       return false;
                   }

    return true;
}

