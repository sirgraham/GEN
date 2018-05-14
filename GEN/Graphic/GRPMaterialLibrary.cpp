/*------------------------------------------------------------------------------------------
//  GRPMATERIALIBRARY.CPP
*/
/**
// \class
//
//  Creacion de materiales para opengl
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 29/04/2014 13:53:14
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPOpengl.h"
#include "GRPFileBitmapPNG.h"
#include "GRPFileBitmapTGA.h"
#include "GRPFileBitmapJPG.h"
#include "GRPBitmap.h"
#include "GRPTexture.h"
#include "GRPMaterial.h"
#include "GRPMaterialLibrary.h"
#include "GRPFactory.h"

#include "XMemory.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/


GRPMATERIALLIBRARY*     GRPMATERIALLIBRARY::instance=NULL;

/*-------------------------------------------------------------------
//  GRPMATERIALLIBRARY::GRPMATERIALLIBRARY
*/
/**
//
//  Class Constructor GRPMATERIALLIBRARY
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/05/2014 9:36:02
//

//  @param        grpfactory :

*/
/*-----------------------------------------------------------------*/
GRPMATERIALLIBRARY::GRPMATERIALLIBRARY()
{
  Clean();
}



/*-------------------------------------------------------------------
//  GRPMATERIALLIBRARY::~GRPMATERIALLIBRARY
*/
/**
//
//   Class Destructor GRPMATERIALLIBRARY
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      07/05/2014 17:48:41
//
*/
/*-----------------------------------------------------------------*/
GRPMATERIALLIBRARY::~GRPMATERIALLIBRARY()
{
  library.DeleteElementContents();
  library.DeleteAll();
  textures.DeleteElementContents();
  textures.DeleteAll();
  Clean();
}



/*-------------------------------------------------------------------
//  GRPMATERIALLIBRARY::GetLibrary
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      16/06/2016 0:09:00
//
//  @return       XMAP<XSTRING*,GRPMATERIAL*>* :
//
*/
/*-----------------------------------------------------------------*/
XMAP<XFSTRING*,GRPMATERIAL*>* GRPMATERIALLIBRARY::GetLibrary()
{
  return &this->library;
}

/*-------------------------------------------------------------------
//  GRPMATERIALLIBRARY::Load
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      06/03/2015 11:03:02
//
//  @return       GRPMATERIALLOADRESULT_ERROR :
//
//  @param        xpath :
//  @param        resourcePath :
//  @param        resourceName :
//  @param        start :
//  @param        end :
*/
/*-----------------------------------------------------------------*/
GRPMATERIALLOADRESULT GRPMATERIALLIBRARY::Load(XCHAR* resourcepath, XCHAR* resourcename,int start,int end)
{

  XSTRING xs;
  xs.Set(resourcepath);

  XSTRING name;

  if (resourcename!=NULL)
    name=resourcename;
  else
    name=resourcepath;



  if (xs.Find(__L("%"),true)==XSTRING_NOTFOUND)
  {
      for (XDWORD e=0;e<this->library.GetSize();e++)
      {
        if (library.GetKey(e)->Compare(name)==0)
          return GRPMATERIALLOADRESULT_DUPLICATED; //already in library
      }

      for (XDWORD e=0;e<this->textures.GetSize();e++)
      {
      if (textures.GetElement(e))
        if (textures.GetElement(e)->GetFileName()->Compare(resourcepath)==0)
          {
            GRPMATERIAL* material = new GRPMATERIAL();
                      if(material)
                        {
                          material->GetName()->Set(resourcename);
                          material->AddLayer      (textures.GetElement(e));
                          Add                     (material);
                        }
            return GRPMATERIALLOADRESULT_DUPLICATED; //already in library
          }
      }
  }

  //--------------------- sprite sequence
  if (xs.Find(__L("%"),true)!=XSTRING_NOTFOUND) //sequence
  {
      GRPFILEBITMAP* fileSequence= new GRPFILEBITMAP();
      fileSequence->SetPath(xs.Get());
      GRPBITMAPSEQUENCE* SpriteSequence=fileSequence->LoadSequence(resourcepath,start,end);

      if (!Create(resourcepath,SpriteSequence))
      {
        delete(fileSequence);
        fileSequence=NULL;
        delete(SpriteSequence);
        SpriteSequence=NULL;
        return GRPMATERIALLOADRESULT_ERROR;
      }
      XDEBUG_PRINTCOLOR(1,__L("Sprite loaded: %s"),resourcepath);
      delete(fileSequence);
      fileSequence=NULL;
  }
  else
  {
  //------------------------------- Normal texture
          if (!Create(resourcepath,resourcename))
              return GRPMATERIALLOADRESULT_ERROR;

      XDEBUG_PRINTCOLOR(1,__L("Texture created: %s"),resourcepath);
  }

  //GRPMATERIAL* material = Select(resourcename);
  //if(!material)
  //    return GRPMATERIALLOADRESULT_ERROR;



  return GRPMATERIALLOADRESULT_SUCCESS;
}


/*-------------------------------------------------------------------
//  GRPMATERIALLIBRARY::Load
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2016 16:11:06
//
//  @return       GRPMATERIALLOADRESULT :
//
//  @param        faces :
//  @param        resourcename :
//  @param        start :
//  @param        end :
*/
/*-----------------------------------------------------------------*/
GRPMATERIALLOADRESULT GRPMATERIALLIBRARY::Load(XVECTOR<XFSTRING*>* faces, XCHAR* resourcename,int start,int end)
{
  GRPMATERIALLOADRESULT result=GRPMATERIALLOADRESULT_SUCCESS;

  GRPFILEBITMAP*    file    = new GRPFILEBITMAP();
  GRPBITMAPSEQUENCE* Sequence=new GRPBITMAPSEQUENCE();
  for (XDWORD e=0;e<faces->GetSize();e++)
  {

    GRPBITMAP*        bitmap  = file->Load(faces->Get(e)->Get());
    if (!bitmap)
      return GRPMATERIALLOADRESULT_ERROR;

      Sequence->AddFrame(bitmap);
  }

      if (!CreateCubeMap(resourcename,Sequence))
      {
        result=GRPMATERIALLOADRESULT_ERROR;
      }

      XDEBUG_PRINTCOLOR(1,__L("Sequence loaded: %s"),resourcename);
      delete(file);
      file=NULL;

      return result;
}


GRPMATERIALLOADRESULT GRPMATERIALLIBRARY::Load(XVECTOR<XSTRING*>* faces, XCHAR* resourcename, int start, int end)
{
    GRPMATERIALLOADRESULT result = GRPMATERIALLOADRESULT_SUCCESS;

    GRPFILEBITMAP*    file = new GRPFILEBITMAP();
    GRPBITMAPSEQUENCE* Sequence = new GRPBITMAPSEQUENCE();
    for (XDWORD e = 0; e<faces->GetSize(); e++)
    {

        GRPBITMAP*        bitmap = file->Load(faces->Get(e)->Get());
        if (!bitmap)
            return GRPMATERIALLOADRESULT_ERROR;

        Sequence->AddFrame(bitmap);
    }

    if (!CreateCubeMap(resourcename, Sequence))
    {
        result = GRPMATERIALLOADRESULT_ERROR;
    }

    XDEBUG_PRINTCOLOR(1, __L("Sequence loaded: %s"), resourcename);
    delete(file);
    file = NULL;

    return result;
}


/*-------------------------------------------------------------------
//   GRPMATERIALLIBRARY::Load
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  16/10/2017 11:23:38
//  @return   GRPMATERIAL* :
//
//  @param    XCHAR* :
//  @param    ... :
//
*//*-----------------------------------------------------------------*/
GRPMATERIAL* GRPMATERIALLIBRARY::Load(XCHAR* material_name, int n, ...)
{
    //----------- first check if already exists
    GRPMATERIAL* material = this->FindByName(material_name);
    if (material)
        return material;

    //---------- create the material
         material = new GRPMATERIAL();
    if (!material)
        return NULL;


    material->SetName(material_name);

    va_list arg;
    va_start(arg, n);

    GRPTEXTURELAYERTYPE type = GRPTEXTURELAYERTYPE_DIFFUSE;
    XCHAR*              name = NULL;

    //-------- iterate layers
    for (int e=0;e<n;e++)
    {
        type =  (GRPTEXTURELAYERTYPE) va_arg(arg, XDWORD);
        name =  (XCHAR*)              va_arg(arg, XCHAR*);

        //------------ check if exists
        GRPTEXTURE* texture = GRPMATERIALLIBRARY::Get()->GetTexture(name);
        if (!texture)
        {
            XPATH xpath;
            xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, name);
            texture = GRPMATERIALLIBRARY::Get()->LoadTexture(name, xpath.Get());
            if (!texture)
            {
                XDEBUG_PRINTCOLOR(4, __L("Unable to load %s"), xpath.Get());
                delete(material);
                return NULL;
            }
        }

        material->SetLayer(type, texture);
    }

    this->library.Add(material->GetName(), material);

    va_end(arg);
    return material;
}

/*-------------------------------------------------------------------
//  GRPMATERIALLIBRARY::Create
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      25/10/2016 12:37:10
//
//  @return       bool :
//
//  @param        resourcename :
*/
/*-----------------------------------------------------------------*/
GRPMATERIAL* GRPMATERIALLIBRARY::Create( XCHAR* resourcename)
{
  GRPMATERIAL*  material  = NULL;
                material = new GRPMATERIAL();

                if(material)
                  {
                    material->GetName()->Set(resourcename);
                    Add(material);
                    return material;
                  }

  return NULL;
}


/*-------------------------------------------------------------------
//  GRPMATERIALLIBRARY::Create
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      07/05/2014 17:52:33
//
//  @param        name :
//  @param        xpath :
*/
/*-----------------------------------------------------------------*/
bool GRPMATERIALLIBRARY::Create(XCHAR* name, XCHAR * resourcename)
{
    GRPMATERIAL* material = NULL;

    GRPTEXTURE* texture=LoadTexture(resourcename, name);
    if (texture)
    {
        material = new GRPMATERIAL();
        if (material)
        {
            material->GetName()->Set(resourcename);
            material->AddLayer(GRPTEXTURELAYERTYPE_DIFFUSE, texture);
            Add(material);
        }
    }
    else
    {
      delete material;
      delete texture;
      return false;
    }

  return true;
}






/*-------------------------------------------------------------------
//  GRPMATERIALLIBRARY::Create
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      19/09/2014 9:56:19
//
//  @return       bool :
//
//  @param        name :
//  @param        seq :
*/
/*-----------------------------------------------------------------*/
bool GRPMATERIALLIBRARY::Create(XCHAR* name,GRPBITMAPSEQUENCE* seq)
{
  if(!seq) return false;

  GRPBITMAP*    bitmap    = NULL;
  GRPMATERIAL*  material  = NULL;
  GRPTEXTURE*   texture   = NULL;
  bool          status    = false;

  int nFrames=0;


      material = new GRPMATERIAL();
      if(material)
        {
          material->GetName()->Set(name);

          texture = new GRPTEXTURE();
          if(texture)
            {
              status = false;
              if (texture->Create(seq))
              {
                material->AddLayer(texture);
                Add(material);
                status = true;
              }
              else
                XDEBUG_PRINTCOLOR(1,__L("Error loading File Bitmap Sequence %s"),name);
            }
        }


  if(!status)
    {
      delete bitmap;
      delete material;
      delete texture;
    }

  return status;
}

/*-------------------------------------------------------------------
//  GRPMATERIALLIBRARY::Create
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      19/09/2014 9:56:19
//
//  @return       bool :
//
//  @param        name :
//  @param        seq :
*/
/*-----------------------------------------------------------------*/
bool GRPMATERIALLIBRARY::CreateCubeMap(XCHAR* name,GRPBITMAPSEQUENCE* seq)
{
  if(!seq) return false;

  GRPBITMAP*    bitmap    = NULL;
  GRPMATERIAL*  material  = NULL;
  GRPTEXTURE*   texture   = NULL;
  bool          status    = false;

  int nFrames=0;


      material = new GRPMATERIAL();
      if(material)
        {
          material->GetName()->Set(name);

          texture = new GRPTEXTURE();
          if(texture)
            {
              if (texture->CreateCubeMap(seq))
              {
                material->AddLayer(texture);
                Add(texture);
                status = true;
              }
              else
                XDEBUG_PRINTCOLOR(1,__L("Error loading File Bitmap Sequence %s"),name);
            }
          Add(material);
        }


  if(!status)
    {
      delete bitmap;
      delete material;
      delete texture;
    }

  return status;
}


/*-------------------------------------------------------------------
//  GRPMATERIALLIBRARY::Create
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      18/05/2015 17:30:24
//
//  @return       bool :
//
//  @param        texture :
*/
/*-----------------------------------------------------------------*/
GRPMATERIAL*   GRPMATERIALLIBRARY::Create (GRPTEXTURE* texture)
{
  GRPMATERIAL*  material  = NULL;
  bool          status    = false;

      material = new GRPMATERIAL();
      if(material)
        {
          material->GetName()->Set(*texture->GetName());
          if(texture)
            {
              material->AddLayer(texture);
              Add(material);

              status = true;
            }
        }

  if(!status)
    {
      delete material;
      delete texture;
      return NULL;
    }

  return material;
}


/*-------------------------------------------------------------------
//  GRPMATERIALLIBRARY::FindByName
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      12/01/2016 9:58:58
//
//  @return       GRPMATERIAL* :
//
//  @param        name :
*/
/*-----------------------------------------------------------------*/
GRPMATERIAL*   GRPMATERIALLIBRARY::FindByName(XCHAR* name)
{
  for (XDWORD e=0;e<this->library.GetSize();e++)
  {
    if (library.GetKey(e)->Compare(name)==0)
      return library.GetElement(e);
  }
  return NULL;
}


/*-------------------------------------------------------------------
//  GRPMATERIALLIBRARY::Get
*/
/**
//
//  gets a material from name
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      30/11/2015 12:40:16
//
//  @return       GRPMATERIAL* :
//
//  @param        name :
*/
/*-----------------------------------------------------------------*/
GRPMATERIAL* GRPMATERIALLIBRARY::Get(XCHAR* name)
{
  GRPMATERIAL* material = NULL;

  XDWORD materials = library.GetSize();
  for(XDWORD i = 0; i < materials; i++)
  {
    if(library.GetKey(i)->Compare(name) == 0)
    {
      material = library.GetElement(i);
      break;
    }
  }

  return material;
}





/*-------------------------------------------------------------------
//  GRPMATERIALLIBRARY::Delete
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      30/05/2016 17:27:50
//
//  @return       bool :
//
//  @param        name :
*/
/*-----------------------------------------------------------------*/
bool GRPMATERIALLIBRARY::Delete(XCHAR* name)
{
  if(!name) return false;

  XFSTRING xstr(name);

  XDWORD size = library.GetSize();

  for(XDWORD i = 0; i < size; i++)
    {
      XFSTRING* xstr = library.FastGetKey(i);
      if(xstr->Compare(name) == 0)
        {
          // found the element
          return library.Delete(xstr);
        }
    }

  return false;
}



/*-------------------------------------------------------------------
//  GRPMATERIALLIBRARY::GetTexture
*/
/**
//
//  gets a texture by name
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      30/11/2015 12:59:08
//
//  @return       GRPTEXTURE* :
//
//  @param        name :
*/
/*-----------------------------------------------------------------*/
GRPTEXTURE* GRPMATERIALLIBRARY::GetTexture(XCHAR* name)
{
  GRPTEXTURE* texture = NULL;

  XDWORD materials = textures.GetSize();
  for(XDWORD i = 0; i < materials; i++)
  {
    if(textures.GetKey(i)->Compare(name) == 0)
    {
      texture = textures.GetElement(i);
      break;
    }
  }

  return texture;
}


/*-------------------------------------------------------------------
//   GRPMATERIALLIBRARY::LoadTexture
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  16/10/2017 11:37:23
//  @return   GRPTEXTURE* :
//
//  @param    XCHAR* :
//  @param    XCHAR* :
//
*//*-----------------------------------------------------------------*/
GRPTEXTURE* GRPMATERIALLIBRARY::LoadTexture(XCHAR* resourcename, XCHAR* filename)
{

    GRPFILEBITMAP* filebitmap = new GRPFILEBITMAP();
    if (!filebitmap)
    {
        XDEBUG_PRINTCOLOR(1, __L("GRPMATERIALLIBRARY::Create : Unable to open file %s"), filename);
        delete(filebitmap);
        return NULL;
    }

    GRPBITMAP*    bitmap = NULL;
    GRPMATERIAL*  material = NULL;
    GRPTEXTURE*   texture = NULL;
    bool          status = false;

    bitmap = filebitmap->Load(filename);
    if (!bitmap)
    {
        XDEBUG_PRINTCOLOR(4, __L("GRPMATERIALLIBRARY::Create : Unable to open file %s"), filename);
        delete(filebitmap);
        return NULL;
    }

    texture = new GRPTEXTURE();
    if (texture)
    {
        texture->Create(bitmap->GetBuffer(), bitmap->GetBufferSize(), bitmap->GetWidth(), bitmap->GetHeight(), bitmap->GetBytesperPixel(), bitmap->GetBitsperPixel());
        texture->GetName()->Set(resourcename);
        texture->SetFileName(filename);
        this->textures.Add(texture->GetName(), texture);
        status = true;
        delete(bitmap);
    }
    delete(filebitmap);
    return texture;
}


/*-------------------------------------------------------------------
//  GRPMATERIALLIBRARY::Select
*/
/**
//
//  Selects material from library by name
//
//  @authorDiego Martinez Ruiz de Gaona
//  @version      29/04/2014 14:04:04
//
//  @return       GRPMATERIAL* : NULL on failure
//
//  @paramname : material name
*/
/*-----------------------------------------------------------------*/
GRPMATERIAL* GRPMATERIALLIBRARY::Select(XCHAR* name)
{
  for(XDWORD c=0; c<library.GetSize(); c++)
    {
      GRPMATERIAL* mat=library.GetElement(c);
      if(!mat->GetName()->Compare(name))
        {
          int id=0;
          library.GetElement(c)->Select();
          return library.GetElement(c);
        }
    }

  XDEBUG_PRINTCOLOR(3,__L("Material Not found : %s"),name);
  return NULL;
}