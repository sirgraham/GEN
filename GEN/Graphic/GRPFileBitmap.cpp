/*------------------------------------------------------------------------------------------
//  GRPFILEBITMAP.CPP
//
//  GRP File Bitmap class
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 08/11/2013 9:47:39
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include <string.h>

#include "XFactory.h"

#include "GRPProperties.h"
#include "GRPBitmap.h"
#include "GRPBitmapSequence.h"

#include "GRPFileBitmapJPG.h"
#include "GRPFileBitmapPNG.h"
#include "GRPFileBitmapBMP.h"
#include "GRPFileBitmapTGA.h"

#include "GRPFileBitmap.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/






/*-------------------------------------------------------------------
//  GRPFILEBITMAPBASE::GRPFILEBITMAPBASE
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      13/11/2013 10:42:07
//
//  @return

//  @param        grpfactory :
*/
/*-----------------------------------------------------------------*/
GRPFILEBITMAPBASE::GRPFILEBITMAPBASE( ) : XFILECONTAINER()
{
  Clean();

  file = xfactory->Create_File();
  if(!file) return;

}


//-------------------------------------------------------------------
//  GRPFILEBITMAPBASE::~GRPFILEBITMAPBASE
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      19/11/2002 18:35:37
//
//  @return       void :
//  */
//-------------------------------------------------------------------
GRPFILEBITMAPBASE::~GRPFILEBITMAPBASE()
{
  if(file) xfactory->Delete_File(file);

  Clean();
}




//-------------------------------------------------------------------
//  GRPFILEBITMAPBASE::Open
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      19/11/2002 18:36:57
//
//  @return       bool :
//  @param        xpath :
//  @param        readonly :
*/
//-------------------------------------------------------------------
bool GRPFILEBITMAPBASE::Open(XPATH& xpath,bool readonly)
{
    if(!file)                                     return false;
    if(!file->Open(xpath,readonly))               return false;
  //#DIEGO
  //  if(!GetPrimaryFile()->GetFileStructHandle())  return false;

  return true;
}



//-------------------------------------------------------------------
//  GRPFILEBITMAPBASE::Create
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      19/11/2002 18:36:57
//
//  @return       bool :
//  @param        xpath :
*/
//-------------------------------------------------------------------
bool GRPFILEBITMAPBASE::Create(XPATH& xpath)
{
  if(!file)                return false;
  if(!file->Create(xpath)) return false;

  return true;
}



//-------------------------------------------------------------------
//  GRPFILEBITMAPBASE::Close
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      19/11/2002 18:37:22
//
//  @return       bool :
*/
//-------------------------------------------------------------------
bool GRPFILEBITMAPBASE::Close()
{
  if(!file->Close()) return false;

  return true;
}






//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------




/*-------------------------------------------------------------------
//  GRPFILEBITMAP::GRPFILEBITMAP
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      20/11/2013 13:02:45
//
//  @return

//  @param        grpfactory :
*/
/*-----------------------------------------------------------------*/
GRPFILEBITMAP::GRPFILEBITMAP( )
{
  Clean();
}




/*-------------------------------------------------------------------
//  GRPFILEBITMAP::GRPFILEBITMAP
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      13/11/2013 10:43:24
//
//  @return

//  @param        grpfactory :
//  @param        xpath :
*/
/*-----------------------------------------------------------------*/
GRPFILEBITMAP::GRPFILEBITMAP(XCHAR* xpath)
{
  Clean();

  this->xpath       = xpath;
}




/*-------------------------------------------------------------------
//  GRPFILEBITMAP::GRPFILEBITMAP
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      13/11/2013 10:43:52
//
//  @return
//
//  @param        xpath :
*/
/*-----------------------------------------------------------------*/
GRPFILEBITMAP::GRPFILEBITMAP(XPATH& xpath)
{
  Clean();

  this->xpath = xpath;
}




/*-------------------------------------------------------------------
//  GRPFILEBITMAP::~GRPFILEBITMAP
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/11/2013 9:53:34
//
//  @return
//  */
/*-----------------------------------------------------------------*/
GRPFILEBITMAP::~GRPFILEBITMAP()
{
  Clean();
}



/*-------------------------------------------------------------------
//  GRPFILEBITMAP::Load
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      20/11/2013 14:11:49
//
//  @return       GRPBITMAP* :
//  */
/*-----------------------------------------------------------------*/
GRPBITMAP* GRPFILEBITMAP::Load(XCHAR* xpath)
{
  if(!xpath) return NULL;

  XPATH   _xpath;
  XSTRING ext;

  _xpath = xpath;

  _xpath.GetExt(ext);
  if(ext.IsEmpty())   return NULL;

  GRPFILEBITMAPBASE*  grpfilebase = NULL;
  GRPBITMAP*    bitmap      = NULL;

  if(!ext.Compare(__L(".jpg"), true)) grpfilebase = new GRPFILEBITMAPJPG();
  else
  if(!ext.Compare(__L(".jpeg"), true)) grpfilebase = new GRPFILEBITMAPJPG();
  else
  if(!ext.Compare(__L(".png"), true)) grpfilebase = new GRPFILEBITMAPPNG();
  else
  if(!ext.Compare(__L(".bmp"), true)) grpfilebase = new GRPFILEBITMAPBMP();
  else
  if(!ext.Compare(__L(".tga"), true)) grpfilebase = new GRPFILEBITMAPTGA();

  if(grpfilebase)
    {
      bitmap = grpfilebase->CreateBitmapFromFile(_xpath);
      delete grpfilebase;
    }

  return bitmap;
}



/*-------------------------------------------------------------------
//  GRPFILEBITMAP::Load
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      20/11/2013 14:13:15
//
//  @return       GRPBITMAP* :
//  @param        xpath :
*/
/*-----------------------------------------------------------------*/
GRPBITMAP* GRPFILEBITMAP::Load(XPATH& xpath)
{
  return Load(xpath.Get());
}




/*-------------------------------------------------------------------
//  GRPFILEBITMAP::Load
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      20/11/2013 14:14:52
//
//  @return       GRPBITMAP* :
//  */
/*-----------------------------------------------------------------*/
GRPBITMAP* GRPFILEBITMAP::Load()
{
  if(xpath.IsEmpty()) return NULL;

  return Load(xpath);
}




/*-------------------------------------------------------------------
//  GRPFILEBITMAP::Save
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      20/11/2013 14:27:30
//
//  @return       bool :
//  @param        xpath :
//  @param        bitmap :
*/
/*-----------------------------------------------------------------*/
bool GRPFILEBITMAP::Save(XCHAR* xpath, GRPBITMAP* bitmap)
{
  if(!xpath) return false;

  XPATH   _xpath;
  XSTRING ext;

  _xpath = xpath;

  _xpath.GetExt(ext);
  if(ext.IsEmpty()) return false;

  GRPFILEBITMAPBASE* grpfilebase  = NULL;
  bool         status       = false;

  if(!ext.Compare(__L(".jpg"), true)) grpfilebase = new GRPFILEBITMAPJPG();
  if(!ext.Compare(__L(".png"), true)) grpfilebase = new GRPFILEBITMAPPNG();
  if(!ext.Compare(__L(".bmp"), true)) grpfilebase = new GRPFILEBITMAPBMP();
  if(!ext.Compare(__L(".tga"), true)) grpfilebase = new GRPFILEBITMAPTGA();

  if(grpfilebase)
    {
      status = grpfilebase->CreateFileFromBitmap(_xpath, bitmap);
      delete grpfilebase;
    }

  return status;
}



/*-------------------------------------------------------------------
//  GRPFILEBITMAP::Save
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      20/11/2013 16:07:40
//
//  @return       bool :
//  @param        xpath :
//  @param        bitmap :
*/
/*-----------------------------------------------------------------*/
bool GRPFILEBITMAP::Save(XPATH& xpath, GRPBITMAP* bitmap)
{
  return Save(xpath.Get(), bitmap);
}



/*-------------------------------------------------------------------
//  GRPFILEBITMAP::Save
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      20/11/2013 16:07:45
//
//  @return       bool :
//  @param        bitmap :
*/
/*-----------------------------------------------------------------*/
bool GRPFILEBITMAP::Save(GRPBITMAP* bitmap)
{
  if(xpath.IsEmpty()) return false;

  return Save(xpath, bitmap);
}





/*-------------------------------------------------------------------
//  GRPFILEBITMAP::LoadSequence
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      20/11/2013 16:40:11
//
//  @return       GRPBITMAPSEQUENCE* :
//  @param        namemask :
//  @param        inisequence :
*/
/*-----------------------------------------------------------------*/
GRPBITMAPSEQUENCE* GRPFILEBITMAP::LoadSequence(XCHAR* namemask, int inisequence, int endsequence)
{
  if(xpath.IsEmpty()) return NULL;

  GRPBITMAPSEQUENCE* bitmapsequence = new GRPBITMAPSEQUENCE();
  if(!bitmapsequence) return NULL;

  XSTRING namefile;

  xpath.GetNamefileExt(namefile);
  if(namefile.IsEmpty())
    {
      // The sequence is in individual files in a directory. Sequence numbers must be continuos
      int nfiles = inisequence;

      while((nfiles < endsequence)|| (endsequence < 0))
        {
          XPATH xpathtemp;
          bool  existfile = false;

          namefile.Format(namemask, nfiles);
          xpathtemp.Create(2, xpath.Get(), namefile.Get());

          XFILE* xfile = xfactory->Create_File();
          if(xfile)
            {
              existfile = xfile->Exist(xpathtemp);
              xfactory->Delete_File(xfile);
            }

          if(!existfile) break;

          GRPBITMAP* bitmap = Load(xpathtemp);
          if(!bitmap) break;

          bitmapsequence->AddFrame(bitmap);

          nfiles++;
        }
    }
   else
    {



    }

  return bitmapsequence;
}




/*-------------------------------------------------------------------
//  GRPFILEBITMAP::Clean
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/11/2013 9:53:17
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void GRPFILEBITMAP::Clean()
{

}

