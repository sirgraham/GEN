/*------------------------------------------------------------------------------------------
//  GRPTEXTURE.CPP
//
//  Graphics Texture class
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 07/05/2014 14:19:02
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPTexture.h"
#include "GRPProfiler.h"
#include "GRPFactory.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  GRPTEXTURE::GRPTEXTURE
*/
/**
//
//  Class Constructor GRPTEXTURE
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      07/05/2014 14:25:43
//
*/
/*-----------------------------------------------------------------*/
GRPTEXTURE::GRPTEXTURE()
{
  bitmapSequence = NULL;
  Clean();
}


/*-------------------------------------------------------------------
//  GRPTEXTURE::~GRPTEXTURE
*/
/**
//
//   Class Destructor GRPTEXTURE
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      07/05/2014 14:26:17
//
*/
/*-----------------------------------------------------------------*/
GRPTEXTURE::~GRPTEXTURE()
{

//  XDEBUG_PRINTCOLOR(0,__L("Deleting Texture: %d %s"),this->handles.Get(0),this->name.Get());
  OPENGLCHECKERROR(__L("Destroying Texture"));

  if (GRPOPENGLCTRL::TEXTURE::CurrentTexture==this)
     GRPOPENGLCTRL::TEXTURE::CurrentTexture=NULL;

  if (this->bitmap)
    GRPPROFILER::texturememory-=this->bitmap->GetBufferSize();

  if (bitmapSequence!=NULL)
    delete(bitmapSequence);

  if (!isCubeMap)
  if (bitmap!=NULL)
    delete bitmap;

  bitmap        =NULL;
  bitmapSequence=NULL;

  if (buffer)
  delete(buffer);
  buffer=NULL;
  /*
  for (XDWORD e=0;e<handles.GetSize();e++)
  {
    GLUINT handle=this->handles.Get(e);
    if (handle!=-1)
      glDeleteTextures(1,&handle);
  }
  */

  handles.DeleteAll();
  Clean();


  OPENGLCHECKERROR(__L("Deleting Texture"));
};


/*-------------------------------------------------------------------
//  GRPTEXTURE::Create
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/06/2017 14:40:14
//
//  @return       bool :
//
//  @param        buffer :
//  @param        buffersize :
//  @param        width :
//  @param        height :
//  @param        channels :
//  @param        layertype :
*/
/*-----------------------------------------------------------------*/
bool GRPTEXTURE::Create(XBYTE* buffer, XQWORD buffersize, XDWORD width, XDWORD height, XDWORD channels,XDWORD bits, GRPTEXTURELAYERTYPE layertype)
{
  this->bitmap          = NULL;
  this->bitmapSequence  = NULL;

  this->layertype = layertype;

  isloaded=false;

  GLFLOAT prop=((float)width) / ((float)height);

  this->width       =(GLFLOAT)width;
  this->height      =(GLFLOAT)height;

  this->buffer      =new XBYTE[(unsigned int)buffersize];
  if (buffer)
    memcpy(this->buffer, buffer,(unsigned int)buffersize);
  else
    memset(this->buffer, 0,(unsigned int)buffersize);

  this->buffersize  = buffersize;
  this->channels    = channels;
  this->bitsperpixel= bits;
  return true;
}

/*-------------------------------------------------------------------
//  GRPTEXTURE::Create
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      07/05/2014 14:31:51
//
//  @return       bool :
//
//  @param        bitmap :
//  @param        layertype :
*/
/*-----------------------------------------------------------------*/
bool GRPTEXTURE::Create(GRPBITMAP* bitmap, GRPTEXTURELAYERTYPE layertype)
{
  if(!bitmap) return false;
  isloaded = false;

  this->bitmap          = bitmap;
  this->layertype       = layertype;
  this->bitmapSequence  = NULL;
  this->width           = (GLFLOAT)this->bitmap->GetWidth();
  this->height          = (GLFLOAT)this->bitmap->GetHeight();
  this->buffer          = bitmap->GetBuffer();
  this->buffersize      = bitmap->GetBufferSize();
  this->channels        = bitmap->GetBytesperPixel();
  this->bitsperpixel    = bitmap->GetBitsperPixel();

  return true;
}

/*-------------------------------------------------------------------
//  GRPTEXTURE::CreateCubeMap
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/07/2016 14:43:19
//
//  @return       bool :
//
//  @param        bitmap :
//  @param        layertype :
*/
/*-----------------------------------------------------------------*/
bool GRPTEXTURE::CreateCubeMap(GRPBITMAPSEQUENCE* bitmapSq, GRPTEXTURELAYERTYPE layertype)
{
  this->layertype = GRPTEXTURELAYERTYPE_DIFFUSE;

  if(!bitmapSq) return false;
  if (bitmapSq->GetNFrames()<=0)
    return false;

  this->layertype = layertype;
  isCubeMap       = true;

  this->bitmap         = bitmapSq->GetFrame(0)->GetBitmap();
  this->bitmapSequence = bitmapSq;

  if (!this->bitmap)
    return false;

  XDWORD xw = (bitmap->GetWidth());
  XDWORD xh = (bitmap->GetHeight());

    width = float((int)(xw));
    height= float((int)(xh));

    GRPPROFILER::texturememory+=bitmap->GetBufferSize();

    isloaded = false;
    return true;
}



/*-------------------------------------------------------------------
//  GRPTEXTURE::CreateCubeMap
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/07/2016 14:43:19
//
//  @return       bool :
//
//  @param        bitmap :
//  @param        layertype :
*/
/*-----------------------------------------------------------------*/
bool GRPTEXTURE::CreateCubeMap(XDWORD width, XDWORD height, XDWORD channels, XDWORD bits, GRPTEXTURELAYERTYPE layertype)
{
  this->layertype = layertype;
  isCubeMap       = true;

  this->bitmap         = NULL;
  this->bitmapSequence = NULL;

  this->width = (GLFLOAT)width;
  this->height= (GLFLOAT)height;
  bitsperpixel = bits*channels;
  this->channels = channels;

  isloaded=true;
  return true;
}

/*-------------------------------------------------------------------
//   GRPTEXTURE::CreateCubeMapFace
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version
//  @return   bool :
//
//  @param    XDWORD :
//  @param    XBYTE* :
//
*//*-----------------------------------------------------------------*/
bool GRPTEXTURE::CreateCubeMapFace(XDWORD index, XBYTE* buffer)
{
    switch (layertype)
    {
    case GRPTEXTURELAYERTYPE_REFLEXION:   XDEBUG_PRINTCOLOR(4, __L("GRPTEXTURE: Unsupported texture type: reflexion"));   return false;
    case GRPTEXTURELAYERTYPE_DEPTH:
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + index
            , 0
            , GL_DEPTH_COMPONENT
            , (GLsizei)width
            , (GLsizei)height
            , 0
            , GL_DEPTH_COMPONENT
            , GL_FLOAT
            , NULL);
    }
    break;
    case GRPTEXTURELAYERTYPE_CUSTOM:
    case GRPTEXTURELAYERTYPE_BUFFER:
    case GRPTEXTURELAYERTYPE_NORMALMAP:
    case GRPTEXTURELAYERTYPE_DIFFUSE:
    case GRPTEXTURELAYERTYPE_OPACITY:
    case GRPTEXTURELAYERTYPE_BUMPMAP:
    case GRPTEXTURELAYERTYPE_SPECULAR:
    case GRPTEXTURELAYERTYPE_EMISIVE:
    default:
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + index
            , 0
            , ((bitsperpixel) == 24) ? GL_RGB : GL_RGBA
            , (GLsizei)width
            , (GLsizei)height
            , 0
            , ((bitsperpixel) == 24) ? GL_RGB : GL_RGBA
            , GL_UNSIGNED_BYTE
            , buffer);
    }
    break;
    }
    return true;
}

/*-------------------------------------------------------------------
//  GRPTEXTURE::Bind
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      12/06/2017 16:58:37
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool GRPTEXTURE::Bind(GRPTEXTURELAYERTYPE layer)
{
/*
GRPOPENGLCTRL::TEXTURE::SetCurrentTexture(layer);

if (!this->GetIsCubeMap())
  glBindTexture(GL_TEXTURE_2D,this->GetHandle());
      else
        glBindTexture(GL_TEXTURE_CUBE_MAP,this->GetHandle());
*/


  if (!this->GetIsCubeMap())
      GRPOPENGLCTRL::TEXTURE::BindTexture(layer, GL_TEXTURE_2D,       this->GetHandle());
  else
      GRPOPENGLCTRL::TEXTURE::BindTexture(layer, GL_TEXTURE_CUBE_MAP, this->GetHandle());

  return true;
}

/*-------------------------------------------------------------------
//  GRPTEXTURE::LoadToGPU
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      15/09/2016 15:43:25
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool GRPTEXTURE::LoadToGPU()
{
    if (isCubeMap)
        return LoadToGPUCubemap();
    else
        return LoadToGPUTexture2D();

    return true;
}

/*-------------------------------------------------------------------
//   GRPTEXTURE::LoadToGPUTexture2D
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  08/11/2017 12:23:44
//  @return   bool :
//
//  @param     :
//
*//*-----------------------------------------------------------------*/
bool GRPTEXTURE::LoadToGPUTexture2D()
{
    GRPPROFILER::texturememory += (unsigned int)this->buffersize;

    GLUINT handle = 0;
    glGenTextures(1, &handle);
    if (handle == 0)
        return false;

    this->handles.Add(handle);

    /*
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, handle);
    */
    GRPOPENGLCTRL::TEXTURE::BindTexture(0, GL_TEXTURE_2D, handle);

    GLboolean isTex = glIsTexture(handle);

    OPENGLCHECKERROR(__L("LoadToGPU"));

    switch (layertype)
    {
    case GRPTEXTURELAYERTYPE_REFLEXION:   XDEBUG_PRINTCOLOR(4, __L("GRPTEXTURE: Unsupported texture type: reflexion"));   return false;
    case GRPTEXTURELAYERTYPE_DEPTH:
    {
        OPENGLCHECKERROR(__L("Depth Texture creating"));
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT
            , (GLsizei)width
            , (GLsizei)height
            , 0
            , GL_DEPTH_COMPONENT
            , GL_FLOAT
            , NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        OPENGLCHECKERROR(__L("Depth Texture creation"));
    }
    break;
    case GRPTEXTURELAYERTYPE_CUSTOM:
    case GRPTEXTURELAYERTYPE_BUFFER:
    case GRPTEXTURELAYERTYPE_NORMALMAP:
    case GRPTEXTURELAYERTYPE_DIFFUSE:
    case GRPTEXTURELAYERTYPE_OPACITY:
    case GRPTEXTURELAYERTYPE_BUMPMAP:
    case GRPTEXTURELAYERTYPE_SPECULAR:
    case GRPTEXTURELAYERTYPE_EMISIVE:
    default:
    {
        OPENGLCHECKERROR(__L("Texture creating"));
        glTexImage2D(GL_TEXTURE_2D, 0, (bitsperpixel == 24) ? GL_RGB : GL_RGBA
            , (GLsizei)width
            , (GLsizei)height
            , 0
            , (bitsperpixel == 24) ? GL_RGB : GL_RGBA
            , GL_UNSIGNED_BYTE
            , buffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Warning: Android OpenGL
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        OPENGLCHECKERROR(__L("Texture creation"));
    }
    break;
    }
    isloaded = true;
    return true;
}

bool GRPTEXTURE::LoadToGPUCubemap()
{
    GLUINT handle = 0;
    glGenTextures(1, &handle);
    OPENGLCHECKERROR(__L("Generate texture cubemap"));

    this->handles.Add(handle);

    GRPOPENGLCTRL::TEXTURE::BindTexture(0, GL_TEXTURE_CUBE_MAP, handle);

    //glBindTexture(GL_TEXTURE_CUBE_MAP, handle);
    OPENGLCHECKERROR(__L("Bind texture cubemap"));

    GLFLOAT prop = ((float)this->bitmap->GetWidth()) / ((float)this->bitmap->GetHeight());

    width = (GLFLOAT)this->bitmap->GetWidth();
    height = (GLFLOAT)this->bitmap->GetHeight();

    float       width = (GLFLOAT)this->bitmapSequence->GetFrame(0)->GetBitmap()->GetWidth();
    float       height = (GLFLOAT)this->bitmapSequence->GetFrame(0)->GetBitmap()->GetWidth();

    for (int e = 0; e<6 && e<this->bitmapSequence->GetNFrames(); e++)
    {
        GRPBITMAP*  bitmap = this->bitmapSequence->GetFrame(e)->GetBitmap();
        CreateCubeMapFace(e, bitmap->GetBuffer());
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    //glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    isloaded = true;
    return true;
}

/*-------------------------------------------------------------------
//  GRPTEXTURE::GetHandle
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      15/09/2016 16:04:54
//
//  @return       GRPTEXTUREHANDLE :
//
*/
/*-----------------------------------------------------------------*/

GRPTEXTUREHANDLE  GRPTEXTURE::GetHandle ()
{
  if (!isloaded)
    LoadToGPU();
  return handles.Get(frame);
}

/*-------------------------------------------------------------------
//  GRPTEXTURE::GetHandles
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      15/09/2016 16:04:44
//
//  @return       XVECTOR<GRPTEXTUREHANDLE>* :
//
*/
/*-----------------------------------------------------------------*/
XVECTOR<GRPTEXTUREHANDLE>*  GRPTEXTURE::GetHandles()
{
  if (!isloaded)
    LoadToGPU();
  return &handles;
}

/*-------------------------------------------------------------------
//  GRPTEXTURE::GetBitmap
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      25/09/2017 14:06:04
//
//  @return       GRPBITMAP* :
//
*/
/*-----------------------------------------------------------------*/
GRPBITMAP*  GRPTEXTURE::GetBitmap()
{
  return this->bitmap;
}

/*-------------------------------------------------------------------
//  GRPTEXTURE::GetFrame
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      25/09/2017 14:06:22
//
//  @return       float :
//
*/
/*-----------------------------------------------------------------*/
float GRPTEXTURE::GetFrame()
{
  return this->sframe;
}

/*-------------------------------------------------------------------
//  GRPTEXTURE::SetFrame
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      25/09/2017 14:06:28
//
//  @param        f :
*/
/*-----------------------------------------------------------------*/
void  GRPTEXTURE::SetFrame(float f)
{
  if (handles.GetSize()==0) return;

  sframe=f;
  frame=((int)floor(sframe))%handles.GetSize();
}


/*-------------------------------------------------------------------
//  GRPTEXTURE::GetLayertype
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      25/09/2017 14:07:38
//
//  @return       GRPTEXTURELAYERTYPE :
//
*/
/*-----------------------------------------------------------------*/
GRPTEXTURELAYERTYPE GRPTEXTURE::GetLayertype()
{
  return layertype;
}

/*-------------------------------------------------------------------
//  GRPTEXTURE::SetLayertype
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      25/09/2017 14:07:44
//
//  @param        p :
*/
/*-----------------------------------------------------------------*/
void  GRPTEXTURE::SetLayertype(GRPTEXTURELAYERTYPE p)
{
  this->layertype=p;
}

/*-------------------------------------------------------------------
//  GRPTEXTURE::SetName
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      25/09/2017 14:07:49
//
//  @param        name :
*/
/*-----------------------------------------------------------------*/
void  GRPTEXTURE::SetName(XCHAR* name)
{
  this->name.Set(name);
}

/*-------------------------------------------------------------------
//  GRPTEXTURE::GetName
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      25/09/2017 14:07:54
//
//  @return       XSTRING* :
//
*/
/*-----------------------------------------------------------------*/
XFSTRING* GRPTEXTURE::GetName()
{
  return &name;
}

/*-------------------------------------------------------------------
//  GRPTEXTURE::SetFileName
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      25/09/2017 14:08:00
//
//  @param        name :
*/
/*-----------------------------------------------------------------*/
void  GRPTEXTURE::SetFileName(XCHAR* name)
{
  this->filename.Set(name);
}

/*-------------------------------------------------------------------
//  GRPTEXTURE::GetFileName
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      25/09/2017 14:08:11
//
//  @return       XSTRING* :
//
*/
/*-----------------------------------------------------------------*/
XSTRING*  GRPTEXTURE::GetFileName()
{
  return &filename;
}


/*-------------------------------------------------------------------
//  GRPTEXTURE::Create
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      07/05/2014 14:31:51
//
//  @return       bool :
//
//  @param        bitmap :
//  @param        layertype :
*/
/*-----------------------------------------------------------------*/
bool GRPTEXTURE::Create(GRPBITMAPSEQUENCE* bitmapSq, GRPTEXTURELAYERTYPE layertype)
{
  if (!bitmapSq) return false;
  if (bitmapSq->GetNFrames()<=0)
    return false;

  this->layertype = layertype;

  this->bitmapSequence = bitmapSq;

  GLUINT* handleList=new GLUINT[bitmapSq->GetNFrames()];

  glGenTextures(bitmapSq->GetNFrames(), handleList);


  for (int e=0;e<bitmapSq->GetNFrames();e++)
  {
  this->bitmap    = bitmapSq->GetFrame(e)->GetBitmap();

  /*
  //glActiveTexture(GL_TEXTURE0);
  GRPOPENGLCTRL::TEXTURE::SetCurrentTexture(0);
  glBindTexture(GL_TEXTURE_2D, handle);
  */

  GLUINT handle = handleList[e];
  this->handles.Add(handle);

  GRPOPENGLCTRL::TEXTURE::BindTexture(0, GL_TEXTURE_2D, handle);

  GLboolean isTex=glIsTexture(handle);

  glTexImage2D(GL_TEXTURE_2D,  0, (bitmap->GetBitsperPixel() == 24)?GL_RGB:GL_RGBA
                                , bitmap->GetWidth()
                                , bitmap->GetHeight()
                                , 0
                                , (bitmap->GetBitsperPixel() == 24)?GL_RGB:GL_RGBA
                                , GL_UNSIGNED_BYTE
                                , bitmap->GetBuffer());

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S    , GL_CLAMP_TO_EDGE);  // Warning: Android OpenGL GLES MUST and can ONLY receive this param
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T    , GL_CLAMP_TO_EDGE);
  }
  width=(GLFLOAT)this->bitmap->GetWidth();
  height=(GLFLOAT)this->bitmap->GetHeight();

  delete  (handleList);
  return true;
}


/*-------------------------------------------------------------------
//  GRPTEXTURE::ReleaseMemory
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      15/09/2016 16:04:31
//
*/
/*-----------------------------------------------------------------*/
void      GRPTEXTURE::ReleaseMemory()
{
  if (this->bitmap)
  {
    grpfactory->DeleteBitmap(bitmap);
    this->bitmap=NULL;
  }
  if (bitmapSequence!=NULL)
  {
    delete(bitmapSequence);
    this->bitmapSequence=NULL;
  }
}




/*-------------------------------------------------------------------
//  GRPTEXTURE::DeleteHandles
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      14/07/2017 11:41:54
//
*/
/*-----------------------------------------------------------------*/
void GRPTEXTURE::DeleteHandles()
{
  for(XDWORD e=0;e<handles.GetSize();e++)
    {
      GLUINT handle = this->handles.Get(e);
      if(handle!=-1)
        {
          glDeleteTextures(1,&handle);
        }
    }
}


/*-------------------------------------------------------------------
//  GRPTEXTURE::GetWidth
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      25/09/2017 14:18:12
//
//  @return       GLFLOAT :
//
*/
/*-----------------------------------------------------------------*/
GLFLOAT GRPTEXTURE::GetWidth()
{
  return this->width;
}

/*-------------------------------------------------------------------
//  GRPTEXTURE::GetHeight
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      25/09/2017 14:18:17
//
//  @return       GLFLOAT :
//
*/
/*-----------------------------------------------------------------*/
GLFLOAT GRPTEXTURE::GetHeight()
{
  return this->height;
}

/*-------------------------------------------------------------------
//  GRPTEXTURE::GetIsCubeMap
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      25/09/2017 14:18:23
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool GRPTEXTURE::GetIsCubeMap()
{
  return this->isCubeMap;
}

/*-------------------------------------------------------------------
//  GRPTEXTURE::SetIsCubeMap
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      25/09/2017 14:18:28
//
//  @param        iscube :
*/
/*-----------------------------------------------------------------*/
void GRPTEXTURE::SetIsCubeMap(bool iscube)
{
  this->isCubeMap=iscube;
}

/*-------------------------------------------------------------------
//  GRPTEXTURE::GetIsReflective
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      25/09/2017 14:18:34
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool GRPTEXTURE::GetIsReflective()
{
  return this->isReflective;
}

/*-------------------------------------------------------------------
//  GRPTEXTURE::SetIsReflective
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      25/09/2017 14:18:39
//
//  @param        isReflective :
*/
/*-----------------------------------------------------------------*/
void GRPTEXTURE::SetIsReflective(bool isReflective)
{
  this->isReflective=isReflective;
}


