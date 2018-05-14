/*------------------------------------------------------------------------------------------
//  GRPTEXTURE.H
*/
/**
// \class
//
//  Grpahics Texture class
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 07/05/2014 14:17:35
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _GRPTEXTURE_H_
#define _GRPTEXTURE_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPNamed.h"
#include "GRPOpenGL.h"
#include "GRPBitmap.h"
#include "GRPBitmapSequence.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

enum
{
    GRPTEXTURE_CUBEMAP_RIGHT=0,
    GRPTEXTURE_CUBEMAP_LEFT,
    GRPTEXTURE_CUBEMAP_BOTTOM,
    GRPTEXTURE_CUBEMAP_TOP,
    GRPTEXTURE_CUBEMAP_FRONT,
    GRPTEXTURE_CUBEMAP_BACK
};

enum GRPTEXTURELAYERTYPE
{
  GRPTEXTURELAYERTYPE_DIFFUSE           ,
  GRPTEXTURELAYERTYPE_SPECULAR          ,
  GRPTEXTURELAYERTYPE_NORMALMAP         ,
  GRPTEXTURELAYERTYPE_REFLEXION         ,
  GRPTEXTURELAYERTYPE_DEPTH             ,
  GRPTEXTURELAYERTYPE_OPACITY           ,
  GRPTEXTURELAYERTYPE_BUFFER            ,
  GRPTEXTURELAYERTYPE_CUSTOM            ,
  GRPTEXTURELAYERTYPE_EMISIVE           ,
  GRPTEXTURELAYERTYPE_BUMPMAP           ,
  GRPTEXTURELAYERTYPE_ENVMAP            ,
  GRPTEXTURELAYERTYPE_MAX
};



/*---- CLASS -----------------------------------------------------------------------------*/


class GRPTEXTURE : public GRPNAMED
{
  public:



                                GRPTEXTURE        ();
    virtual                    ~GRPTEXTURE        ();

    bool                        Create            (XBYTE*             buffer=NULL,  XQWORD              buffersize=0, XDWORD width=512, XDWORD height=512, XDWORD channels=4, XDWORD bits=8, GRPTEXTURELAYERTYPE layertype = GRPTEXTURELAYERTYPE_DIFFUSE);
    bool                        Create            (GRPBITMAP*         bitmap,       GRPTEXTURELAYERTYPE layertype = GRPTEXTURELAYERTYPE_DIFFUSE);
    bool                        Create            (GRPBITMAPSEQUENCE* bitmap,       GRPTEXTURELAYERTYPE layertype = GRPTEXTURELAYERTYPE_DIFFUSE);
    bool                        CreateCubeMap     (GRPBITMAPSEQUENCE* bitmap,       GRPTEXTURELAYERTYPE layertype = GRPTEXTURELAYERTYPE_DIFFUSE);
    bool                        CreateCubeMap     (XDWORD             width=512,    XDWORD              height=512, XDWORD channels=4, XDWORD bits=8, GRPTEXTURELAYERTYPE layertype = GRPTEXTURELAYERTYPE_DIFFUSE);
    bool                        CreateCubeMapFace (XDWORD             faceindex,    XBYTE*              buffer);

    bool                        Bind              (GRPTEXTURELAYERTYPE type);

    bool                        LoadToGPU         ();
    bool                        LoadToGPUTexture2D();
    bool                        LoadToGPUCubemap  ();

    GRPTEXTUREHANDLE            GetHandle         ();
    XVECTOR<GRPTEXTUREHANDLE>*  GetHandles        ();

    GRPBITMAP*                  GetBitmap         ();

    float                       GetFrame          ();
    void                        SetFrame          (float f);


    GRPTEXTURELAYERTYPE         GetLayertype      ();
    void                        SetLayertype      (GRPTEXTURELAYERTYPE p);

    void                        SetName           (XCHAR* name);
    XFSTRING*                   GetName           ();


    void                        SetFileName       (XCHAR* name);
    XSTRING*                    GetFileName       ();

    void                        ReleaseMemory     ();
    void                        DeleteHandles     ();

    GLFLOAT                     GetWidth          ();
    GLFLOAT                     GetHeight         ();

    bool                        GetIsCubeMap      ();
    void                        SetIsCubeMap      (bool iscube);

    bool                        GetIsReflective   ();
    void                        SetIsReflective   (bool isReflective);


    bool                        isloaded;

    XBYTE*                      buffer;
    GLFLOAT                     width;
    GLFLOAT                     height;

  protected:


    XQWORD                      buffersize;
    XBYTE                       channels;
    XBYTE                       bitsperpixel;

    XSTRING                     filename;

    float                       sframe;

    GRPTEXTURELAYERTYPE         layertype;

    XVECTOR<GRPTEXTUREHANDLE>   handles;

    bool                        isCubeMap;
    bool                        isReflective;

    GRPBITMAP*                  bitmap;

    GRPBITMAPSEQUENCE*          bitmapSequence;
    XDWORD                      frame;



  private:


    void                        Clean             ()
                                {
                                  layertype       = GRPTEXTURELAYERTYPE_DIFFUSE;
                                  bitmap          = (GRPBITMAP*)NULL;
                                  frame           = 0;
                                  bitmapSequence  = NULL;
                                  sframe          = 0.0f;
                                  width           = 0;
                                  height          = 0;
                                  isCubeMap       = false;
                                  isloaded        = false;
                                  buffer          = NULL;
                                  isReflective    = false;
                                }




};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

