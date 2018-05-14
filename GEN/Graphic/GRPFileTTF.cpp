/*------------------------------------------------------------------------------------------
//  GRPTTFTEXT.CPP
//
//  Creates 3d text from vectorial data in TTF
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 19/05/2014 9:49:38
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

 //#include <cmath>
#include <math.h>

#include "XFactory.h"
#include "XPath.h"
#include "XDebug.h"
#include "XBuffer.h"
#include "XFile.h"

#include "GRPFont.h"
#include "GRPShape.h"
#include "GRPObject.h"
#include "GRPObjectBuilder.h"
#include "GRPObjectBuilderAlgorithmTriangulationEar.h"

#include "GRPFileTTF.h"

#include "XMemory.h"


#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"


/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/

/*-------------------------------------------------------------------
//  GRPFILETTF::GRPFILETTF
*/
/**
//
//  Class Constructor GRPFILETTF
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      27/06/2014 9:25:04
//


*/
/*-----------------------------------------------------------------*/
GRPFILETTF::GRPFILETTF()
{
  Clean();
  //xbuffer=NULL;

}



/*-------------------------------------------------------------------
//  GRPFILETTF::~GRPFILETTF
*/
/**
//
//   Class Destructor GRPFILETTF
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      27/06/2014 9:28:59
//
*/
/*-----------------------------------------------------------------*/
GRPFILETTF::~GRPFILETTF()
{
  this->glyphs.DeleteElementContents();
  this->glyphs.DeleteAll();
  this->shapes.DeleteElementContents();
  this->shapes.DeleteAll();

  this->strings.DeleteElementContents();
  this->strings.DeleteAll();

  delete(xbuffer);
  Clean();
}

/*-------------------------------------------------------------------
//   GRPFILETTF::LoadFile
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  25/01/2018 18:15:38
//  @return   bool :
//
//  @param    XCHAR* :
//  @param    XCHAR* :
//
*//*-----------------------------------------------------------------*/
bool GRPFILETTF::OpenFile(XCHAR* path)
{
    XFILE*  xfile = NULL;
    bool    status = false;

    if (xbuffer != NULL)
        delete(xbuffer);

    xbuffer = new XBUFFER();
    xfile = xfactory->Create_File();
    if (!xfile)
    {
        xbuffer->Delete();
        delete(xbuffer);
        xbuffer = NULL;
        return false;
    }

    if (!xfile->Open(path))
    {
        xbuffer->Delete();
        delete(xbuffer);
        xbuffer = NULL;
        delete(xfile);
        xfile = NULL;
        return false;
    }

    xbuffer->Resize(xfile->GetSize());
    status = xfile->Read(xbuffer->Get(), xbuffer->GetSize());
    xfile->Close();

    xfactory->Delete_File(xfile);

    int nfonts = stbtt_GetNumberOfFonts(xbuffer->Get());

    this->state = GRPFILETTF_STATE_OPEN;

    //-------------- is open



    return true;
}


/*-------------------------------------------------------------------
//  GRPFILETTF::LoadFile
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      27/06/2014 10:20:48
//
//  @return       bool :
//
//  @param        path :
//  @param        name :
*/
/*-----------------------------------------------------------------*/
bool GRPFILETTF::LoadFile(XCHAR* path)
{
    if (this->state<GRPFILETTF_STATE_OPEN)
    if (!OpenFile(path))
        return false;

    if (this->state < GRPFILETTF_STATE_INIT)
        if (!Init())
            return false;

  return ParseGlyphs();
}

/*-------------------------------------------------------------------
//   GRPFILETTF::Init
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  26/01/2018 9:47:07
//  @return   bool :
//
//  @param     :
//
*//*-----------------------------------------------------------------*/
bool GRPFILETTF::Init()
{
    int index = stbtt_GetFontOffsetForIndex(xbuffer->Get(), 0);
    if (stbtt_InitFont(&info, xbuffer->Get(), index) == 0)
        return false;

    this->GetFontNameStrings();
    this->state = GRPFILETTF_STATE_INIT;

    return true;
}

/*-------------------------------------------------------------------
//   GRPFILETTF::ParseGlyphs
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  25/01/2018 18:20:45
//  @return   bool :
//
//  @param     :
//
*//*-----------------------------------------------------------------*/
bool GRPFILETTF::ParseGlyphs()
{
    this->glyphs.SetAddInLimit(info.numGlyphs);
    for (int indexglyph = 0; indexglyph<info.numGlyphs; indexglyph++)
    {
        //------------ read and convert all glyphs
        stbtt_vertex *vertices;
        int num_verts = stbtt_GetGlyphShape(&info, indexglyph, &vertices);

        GRPFILETTFGLYPH*      glyph = new GRPFILETTFGLYPH(indexglyph);
        if (!glyph)   return false;

        //----------
        glyph->vertexlist.SetAddInLimit(num_verts);
        for (int e = 0; e<num_verts; e++)
        {
            GRPFILETTFGLYPHVERTEX* vertex = new GRPFILETTFGLYPHVERTEX(&vertices[e]);
            if (vertex)
                if (!glyph->vertexlist.Add(vertex))
                    delete vertex;
        }

        this->glyphs.Add(indexglyph, glyph);
    }

    this->shapes.SetAddInLimit(info.numGlyphs);

    for (int indexglyph = 0; indexglyph<info.numGlyphs; indexglyph++)
    {
        GRPSHAPE* shape = GetGlyphShape(indexglyph);
        this->shapes.Add(indexglyph, shape);
    }

    state = GRPFILETTF_STATE_PARSED;
    return true;
}

/*-------------------------------------------------------------------
//   GRPFILETTF::GetFontNameStrings
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  25/01/2018 16:25:53
//  @return   void :
//
//  @param     :
//
*//*-----------------------------------------------------------------*/
void GRPFILETTF::GetFontNameStrings()
{
    int i, count, stringOffset;
    unsigned   char  *fc = this->info.data;
    unsigned   int offset = this->info.fontstart;
    unsigned   int nm = stbtt__find_table(fc, offset, "name");
    if (!nm) return;

    count = ttUSHORT(fc + nm + 2);
    stringOffset = nm + ttUSHORT(fc + nm + 4);
    for (i = 0; i < count; ++i) {

        stbtt_uint32 loc = nm + 6 + 12 * i;

        XDWORD  encodingID = ttUSHORT(fc + loc + 2);

          if (encodingID != GRPFILETTFUNICODEENCODINGID_UNICODE_1_0 &&
              encodingID != GRPFILETTFUNICODEENCODINGID_UNICODE_1_1)
              continue;

        int     length = ttUSHORT(fc + loc + 8);
        if     (length == 0)
                continue;

        XDWORD platformID = ttUSHORT(fc + loc + 0);
        XDWORD languageID = ttUSHORT(fc + loc + 4);
        XDWORD nameID     = ttUSHORT(fc + loc + 6);

        const char* memory = (const char*)(fc + stringOffset + ttUSHORT(fc + loc + 10));

        XSTRING*    holder = new XSTRING();
        switch (encodingID)
        {
        case GRPFILETTFUNICODEENCODINGID_UNICODE_1_0:
                holder->Set((XBYTE*)memory, length);
                break;
        case GRPFILETTFUNICODEENCODINGID_UNICODE_1_1:
                for (int e = 0,i = 0; e < length/2; e++, i += 2)
                {
                    XCHAR c;
                    c = (memory[i] * 256) + (memory[i + 1]);
                    holder->Add(c);
                }
                break;
        default:
                break;
        }
        if (!this->strings.Add(nameID, holder))
            delete(holder);
        }
}


/*-------------------------------------------------------------------
//  GRPFILETTF::LoadFile
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      27/06/2014 9:45:08
//
//  @return       bool :
//
//  @param        xpath :
*/
/*-----------------------------------------------------------------*/
bool GRPFILETTF::LoadFile(XPATH& xpath)
{
  return LoadFile(xpath.Get());
}


/*-------------------------------------------------------------------
//  GRPFILETTF::GetGlyphShape
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      27/06/2014 14:04:01
//
//  @return       GRPSHAPE* :
//
//  @param        character :
*/
/*-----------------------------------------------------------------*/
GRPSHAPE* GRPFILETTF::GetGlyphShape(XCHAR index)
{

  GRPFILETTFGLYPH*  glyph         = glyphs.GetElement(index);
  if(!glyph) return NULL;

  GRPSHAPE*         glyphshape    = new GRPSHAPE();
  if(!glyphshape)   return NULL;

  GRPSPLINE*              spline        = NULL;
  XDWORD                  size          = glyph->vertexlist.GetSize();
  GLFLOAT                 cx            = 0.0f;
  GLFLOAT                 cy            = 0.0f;
  bool                    lastmoved     = false;
  GRPFILETTFGLYPHVERTEX*  lastvertex    = 0;
  int                     negativevalue = 0;


  for(XDWORD e=0; e<size; e++)
    {
      GRPFILETTFGLYPHVERTEX* glyphvertex= glyph->vertexlist.FastGet(e);
      if(!glyphvertex) break;

      GLFLOAT   x = glyphvertex->x;
      GLFLOAT   y = glyphvertex->y;

      if(x<0) negativevalue=e;

      GRPPOINT point;

      switch(glyphvertex->type)
        {
          case GRPFILETTFANCHOR_VMOVE : spline = new GRPSPLINE();
                                        if(!spline) break;

                                        if(lastvertex) spline->origin = glyphshape->contours.GetLast()->points.GetLast();
                                        glyphshape->AddContour(spline);

                                        spline->nsegments = 5;
                                        point.x           = x;
                                        point.y           = y;
                                        point.z           = 0.0f;
                                        glyphshape->contours.GetLast()->Add(GRPANCHORPOINTTYPE_LINE, point);
                                        lastmoved=true;
                                        break;

          case GRPFILETTFANCHOR_VLINE : point.x = x;
                                        point.y = y;
                                        point.z = 0.0f;
                                        glyphshape->contours.GetLast()->Add(GRPANCHORPOINTTYPE_LINE, point);

                                        lastmoved=false;
                                        break;

          case GRPFILETTFANCHOR_VCURVE:
                                        {
                                        GRPPOINT a,b,c;

                                        glyphvertex = glyph->vertexlist.FastGet(e-1);
                                        x  = glyphvertex->x;
                                        y  = glyphvertex->y;

                                        a.x = x;
                                        a.y = y;
                                        a.z = 0.0f;


                                        glyphvertex = glyph->vertexlist.FastGet(e);

                                        x = glyphvertex->x;
                                        y = glyphvertex->y;

                                        b.x = x;
                                        b.y = y;
                                        b.z = 0.0f;

                                        c.x= glyphvertex->cx;
                                        c.y= glyphvertex->cy;
                                        c.z=0.0f;

                                        if (((a.y == b.y) && (b.y == c.y)) ||
                                            ((a.x == b.x) && (b.x == c.x)))  //horizontal o vertical
                                        {
                                        //glyphshape->contours.GetLast()->Add(GRPANCHORPOINTTYPE_LINE, a);
                                        glyphshape->contours.GetLast()->Add(GRPANCHORPOINTTYPE_LINE, b);
                                        }

                                        glyphshape->contours.GetLast()->Add(GRPANCHORPOINTTYPE_QUADRIC_BEZIER, a);
                                        glyphshape->contours.GetLast()->Add(GRPANCHORPOINTTYPE_QUADRIC_BEZIER, b);
                                        glyphshape->contours.GetLast()->Add(GRPANCHORPOINTTYPE_CONTROLPOINT, c);

                                        lastmoved=false;
                                        }
                                        break;

          case GRPFILETTFANCHOR_VCUBIC:
                                        {
                                        glyphvertex = glyph->vertexlist.FastGet(e-1);
                                        x  = glyphvertex->x;
                                        y  = glyphvertex->y;

                                        GRPPOINT a,b,c,d;
                                        a.x = x;
                                        a.y = y;
                                        a.z = 0.0f;


                                        //---

                                        glyphvertex = glyph->vertexlist.FastGet(e);

                                        x = glyphvertex->cx;
                                        y = glyphvertex->cy;

                                        b.x = x;
                                        b.y = y;
                                        b.z = 0.0f;


                                        //----

                                        x = glyphvertex->cx1;
                                        y = glyphvertex->cy1;

                                        c.x = x;
                                        c.y = y;
                                        c.z = 0.0f;

                                        //------

                                        d.x= glyphvertex->x;
                                        d.y= glyphvertex->y;
                                        d.z=0.0f;

                                        //if is not horizontal
                                        if ((a.y == b.y && b.y== c.y && c.y==d.y) //es horizontal
                                           || (a.x == b.x && b.x==c.x && c.x==d.x)) //es vertical
                                        {
                                          //glyphshape->contours.GetLast()->Add(GRPANCHORPOINTTYPE_LINE, a);
                                          glyphshape->contours.GetLast()->Add(GRPANCHORPOINTTYPE_LINE, d);
                                        }
                                        else //es una curvita
                                        {
                                        glyphshape->contours.GetLast()->Add(GRPANCHORPOINTTYPE_CUBIC_BEZIER, a);
                                        glyphshape->contours.GetLast()->Add(GRPANCHORPOINTTYPE_CONTROLPOINT, b);
                                        glyphshape->contours.GetLast()->Add(GRPANCHORPOINTTYPE_CONTROLPOINT, c);
                                        glyphshape->contours.GetLast()->Add(GRPANCHORPOINTTYPE_CUBIC_BEZIER, d);
                                        }


                                        lastmoved=false;
                                        }
                                        break;


        }

      lastvertex = glyphvertex;
    }

  for (XDWORD c=0;c<glyphshape->contours.GetSize();c++)
  {
    XDWORD np=glyphshape->contours.FastGet(c)->points.GetSize();
    for (XDWORD e=0;e<np;e++)
    {
      GRPANCHORPOINT* p=glyphshape->contours.FastGet(c)->points.FastGet(e);
      p->value.x/=TTFGLOBALSCALE;
      p->value.y/=TTFGLOBALSCALE;
    }
  }

  if (info.cff.size>0)
    glyphshape->Reverse();

  return glyphshape;

}




/*-------------------------------------------------------------------
//  GRPFILETTF::FindGlyphIndex
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      15/03/2017 11:12:56
//
//  @return       int :
//
//  @param        n :
*/
/*-----------------------------------------------------------------*/
int GRPFILETTF::FindGlyphIndex(int unicode_codepoint)
{
  return stbtt_FindGlyphIndex(&info, unicode_codepoint);
}


/*-------------------------------------------------------------------
//  GRPFILETTF::GetGlyphMetrics
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      15/03/2017 12:44:35
//
//  @return       bool :
//
//  @param        index :
//  @param        *advanceWidth :
//  @param        *leftSideBearing :
*/
/*-----------------------------------------------------------------*/
bool  GRPFILETTF::GetGlyphMetrics(int index, float *fadvanceWidth, float *fleftSideBearing,float* fascend,float* fdescent,float * flinegap)
{
  int advanceWidth,  leftSideBearing, ascend, descent, linegap;
  int codepoint=stbtt_FindGlyphIndex(&info,index);

  if (fadvanceWidth || fleftSideBearing)
  stbtt_GetGlyphHMetrics(&info,codepoint,&advanceWidth,&leftSideBearing);

  if (fascend || fdescent  || flinegap)
  stbtt_GetFontVMetrics(&info,&ascend,&descent,&linegap);

  if (fadvanceWidth)    *fadvanceWidth    =float(advanceWidth)    /TTFGLOBALSCALE;
  if (fleftSideBearing) *fleftSideBearing =float(leftSideBearing) /TTFGLOBALSCALE;
  if (fascend)          *fascend          =float(ascend )         /TTFGLOBALSCALE;
  if (fdescent)         *fdescent         =float(descent)         /TTFGLOBALSCALE;
  if (flinegap)         *flinegap         =float(linegap)         /TTFGLOBALSCALE;

  return true;
}



/*-------------------------------------------------------------------
//  GRPFILETTF::GetFontOS2Metrics
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      08/06/2017 9:58:46
//
//  @return       bool :
//
//  @param        ascend :
//  @param        descent :
//  @param        linegap :
*/
/*-----------------------------------------------------------------*/
bool GRPFILETTF::GetFontOS2Metrics(float* ascend,float* descend,float * linegap)
{
  int osascend, osdecend, oslinegap;

  stbtt_GetFontVMetricsOS2(&info, &osascend, &osdecend, &oslinegap);

  if(ascend)  *ascend  = osascend / TTFGLOBALSCALE;
  if(descend) *descend = osdecend / TTFGLOBALSCALE;
  if(linegap) *linegap = oslinegap / TTFGLOBALSCALE;

  return true;
}

/*-------------------------------------------------------------------
//  GRPFILETTF::GetKerning
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      16/03/2017 17:42:12
//
//  @return       bool :
//
//  @param        ch1 :
//  @param        ch2 :
*/
/*-----------------------------------------------------------------*/
float GRPFILETTF::GetKerning(int ch1, int ch2)
{
  float fkern=0.0f;
  int kern=stbtt_GetCodepointKernAdvance(&info,ch1,ch2);

  fkern=float(kern)/TTFGLOBALSCALE;

  return fkern;
}

/*-------------------------------------------------------------------
//   GRPFILETTF::GetString
*/
/**
//  Recover a string from map
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  26/01/2018 10:22:06
//  @return   XCHAR* :
//
//  @param    GRPFILETTF_STRINGID : stringid , the string id we want to retrieve
//
*//*-----------------------------------------------------------------*/
XCHAR*  GRPFILETTF::GetString(GRPFILETTF_STRINGID stringid)
{
    XSTRING * s = this->strings.Get(stringid);
    if (s)
        return s->Get();
    return __L("");
}











