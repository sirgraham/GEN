
/*------------------------------------------------------------------------------------------
//  GRPFONT.CPP
//
//  Font 3D class
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 18/08/2014 12:41:59
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/


#include "GRPVector.h"
#include "GRPObjectBuilder.h"
#include "GRPObjectBuilderAlgorithmTriangulationEar.h"
#include "GRPObjectBuilderAlgorithmExtrude.h"
#include "GRPObjectBuilderAlgorithmStroke.h"
#include "GRPMappingProjectionCubic.h"
#include "GRPFileTTF.h"
#include "GRPFont.h"
#include "GRPShape.h"
#include "GRPScene.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/




/*-------------------------------------------------------------------
//  GRPFONT::GRPFONT
*/
/**
//
//  Class Constructor GRPFONT
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      18/08/2014 12:44:23
//
*/
/*-----------------------------------------------------------------*/
GRPFONT::GRPFONT()
{
  Clean();
}




/*-------------------------------------------------------------------
//  GRPFONT::~GRPFONT
*/
/**
//
//   Class Destructor GRPFONT
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      18/08/2014 12:44:29
//
*/
/*-----------------------------------------------------------------*/
GRPFONT::~GRPFONT()
{
  shapes.DeleteElementContents();
  shapes.DeleteAll();

  letters.DeleteElementContents();
  letters.DeleteAll();

  if (fileTTF)  delete fileTTF;

  Clean();
}



/*-------------------------------------------------------------------
//  GRPFONT::LoadTFF
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      18/08/2014 12:57:28
//
//  @return       GRPFILETTF* :
//

//  @param        xpath :
//  @param        name :
//  @param        attributes :
*/
/*-----------------------------------------------------------------*/
bool GRPFONT::LoadTTF( XCHAR* xpath)
{
  fileTTF = new GRPFILETTF();
  if(!fileTTF)
    {
      lasterror = GRPFONTSTATUS_ERROR_FAILEDMEMORY;
      return false;
    }

  if (!fileTTF->LoadFile(xpath))
    {
      delete(fileTTF);
      fileTTF   = NULL;
      lasterror = GRPFONTSTATUS_ERROR_FILENOTFOUND;
      return false;
    }

  this->filename=xpath;



  XDWORD nGlyphs=fileTTF->GetInfo()->numGlyphs;
  for(XDWORD e=0; e<(XDWORD)nGlyphs; e++)
    {
      GRPSHAPE* shape= fileTTF->GetGlyphShape(e);
      shapes.Add(e,shape);
    }

  GetMetrics();
  return true;
}

/*-------------------------------------------------------------------
//   GRPFONT::SetAttributes
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  26/01/2018 10:12:30
//  @return   bool :
//
//  @param    GRPFONTATTRIBUTES* :
//
*//*-----------------------------------------------------------------*/
bool GRPFONT::SetAttributes(GRPFONTATTRIBUTES* attributes)
{
    this->attributes = *attributes;
    return true;
}


/*-------------------------------------------------------------------
//   GRPFONT::GetMetrics
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  25/01/2018 17:45:38
//  @return   bool :
//
//  @param     :
//
*//*-----------------------------------------------------------------*/
bool GRPFONT::GetMetrics()
{
    fileTTF->GetGlyphMetrics(__C(' '), &spaceadvance, &leftSideBearing, &ascend, &descend, &linegap);

    //fileTTF->GetGlyphAdvance(__C(' '), &spaceadvance);
    monospacewidth = spaceadvance;

    // obtain the spacing we need to add between lines
    lineheight = ascend - descend + linegap;

    // get the meximum advance of an element(useful for monospace)
    maxadvance = spaceadvance;

    return true;
}



/*-------------------------------------------------------------------
//  GRPFONT::GetGlyphShape
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      18/08/2014 12:59:03
//
//  @return       GRPSHAPE* :
//
//  @param        letter :
*/
/*-----------------------------------------------------------------*/
GRPSHAPE* GRPFONT::GetGlyphShape(XCHAR letter)
{
  int shapeIndex  = fileTTF->FindGlyphIndex(letter);
  int index       = shapes.Find(shapeIndex);

  if(index == NOTFOUND)
    {
      lasterror = GRPFONTSTATUS_ERROR_GLYPH_UNDEFINED;
      return NULL;
    }
  else
      lasterror = GRPFONTSTATUS_OK;

  GRPSHAPE*   shape = shapes.GetElement(index);
  return      shape;
}




/*-------------------------------------------------------------------
//  GRPFONT::CreateGlyph
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      18/08/2014 13:54:20
//
//  @return       GRPELEMENT* :
//
//  @param        letter :
*/
/*-----------------------------------------------------------------*/
GRPELEMENT* GRPFONT::CreateGlyph(XCHAR letter)
{

  //XDEBUG_PRINTCOLOR(0,__L("Creating Glyph %c"),letter);
  float advance = 0.0f, bearing=0.0f, height =0.0f, descend=0.0f, ascend =0.0f, gap =0.0f;

  GRPSHAPE* glyph = GetGlyphShape(letter);
  if(!glyph) return NULL;
  if(glyph->contours.IsEmpty())
  {

    GRPELEMENT* blank=new GRPELEMENT();
                blank->SetMesh(new GRPMESH());


    this->GetTTF()->GetGlyphMetrics(letter,&advance,&bearing,&ascend,&descend,&gap);
    blank->GetBoundingBox()->blocked=true;
    blank->GetBoundingBox()->advance  = advance;


    return blank;
  }


  GRPOBJECTBUILDERALGORITHMTRIANGULIZATIONEAR   triangulizationearalgorimth (GRPOBJECTBUILDER::Get());
  GRPOBJECTBUILDERALGORITHMEXTRUDE              extruderalgorimth           (GRPOBJECTBUILDER::Get());
  GRPOBJECTBUILDERALGORITHMSTROKE               strokeralgorimth            (GRPOBJECTBUILDER::Get());


  GRPSHAPE*     processedglyph  = NULL;


  GRPELEMENT*   element         = NULL;

  GRPELEMENT*   front           = NULL;
  GRPELEMENT*   back            = NULL;
  GRPELEMENT*   extrusion       = NULL;
  GRPELEMENT*   border          = NULL;


  processedglyph = GRPOBJECTBUILDER::Get()->ProcessShapeToLines(glyph, attributes.quality);

  GRPSHAPE* processedglyph2=processedglyph->CreateCopy();



  GRPSHAPE*     mergedglyph;
                mergedglyph    = GRPOBJECTBUILDER::Get()->MergeInnerSplines(processedglyph2);

  delete(processedglyph2);
  processedglyph2=NULL;

  element=new GRPELEMENT();
  element->SetMesh(new GRPMESH());

  if (mergedglyph->contours.GetSize()>0)
  {


  if(attributes.front)
    {
      triangulizationearalgorimth.normal  = GRPVECTOR(0.0, 0.0, +1.0);
      triangulizationearalgorimth.z       = 0.0;
      front = GRPOBJECTBUILDER::Get()->TriangulizeShape( mergedglyph, &triangulizationearalgorimth);
      if (front==NULL)
        return NULL;
    }


  if(attributes.back)
    {
      triangulizationearalgorimth.normal  = GRPVECTOR(0.0, 0.0, -1.0);
      triangulizationearalgorimth.z       = attributes.extrude;
      back = GRPOBJECTBUILDER::Get()->TriangulizeShape( mergedglyph, &triangulizationearalgorimth);
      if (back==NULL)
        return NULL;
    }
  }

  if (processedglyph->contours.GetSize()>0)
  {
  if(attributes.extrude != 0.0)
    {
      extruderalgorimth.closeSpline   = false;
      extruderalgorimth.nsegments     = 1;
      extruderalgorimth.depth         = attributes.extrude;
      extruderalgorimth.UnifyNormals  = attributes.unifynormals;
      extruderalgorimth.ReverseNormals= attributes.reversenormals;

      extrusion=  GRPOBJECTBUILDER::Get()->TriangulizeShape( processedglyph,  &extruderalgorimth);
    }

  if (attributes.innerborder!=0.0f || attributes.outerborder!=0.0f)
    {
      strokeralgorimth.SetCloseSpline (true);
      strokeralgorimth.SetInnerWidth  (attributes.innerborder);
      strokeralgorimth.SetOuterWidth  (attributes.outerborder);
      strokeralgorimth.SetExtrude     (attributes.borderextrusion);
      strokeralgorimth.SetJointType   (attributes.jointtype);
      strokeralgorimth.SetPenType     (attributes.pentype);
      strokeralgorimth.SetBevel       (attributes.bevel);
      strokeralgorimth.SetMitterWidth ((attributes.innerborder+attributes.outerborder));

      border =GRPOBJECTBUILDER::Get()->TriangulizeShape(processedglyph, &strokeralgorimth);
      if (border==NULL)
        return NULL;
    }
  }

  GRPMAPPINGPROJECTIONCUBIC cubic;

    if(element && front)
      element->MergeElement(front);

  if(element && back)
      element->MergeElement(back);

  if(element && extrusion)
      element->MergeElement(extrusion);

  if(element && border)
      element->MergeElement(border);

  element->SetRenderMode(GRPRENDERMODE_TRIANGLES);
  cubic.ProjectMapping(element);

  this->GetTTF()->GetGlyphMetrics(letter,&advance,&bearing,&ascend,&descend,&gap);

  element->UpdateBoundingBox();

  GRPBB* bb=element->GetBoundingBox();

  /*
  bb->rightmargin   = (advance+bearing)-bb->maxpoint.x;
  bb->bottommargin  = descend;
  bb->topmargin     = ascend;
  bb->leftmargin    = bearing;
  */
  bb->rightmargin   = 0.0f;
  bb->bottommargin  = 0.0f;
  bb->topmargin     = 0.0f;
  bb->leftmargin    = 0.0f;

  bb->bearing       = bearing;
  bb->advance       = advance;
  bb->ascend        = ascend;
  bb->descend       = descend;
  bb->lineheight    = lineheight;

  bb->kerning = 0.0f;


  delete mergedglyph;
  delete processedglyph;

  if(front)     delete front;     front       =NULL;
  if(back)      delete back;      back        =NULL;
  if(extrusion) delete extrusion; extrusion   =NULL;
  if(border)    delete border;    border      =NULL;

  element->GetName()->Format(__L("Glyph [%c]"),letter);
  element->GetMesh()->isprocedural=true;
  return element;
}




/*-------------------------------------------------------------------
//  GRPFONT::TextBox
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      18/08/2014 13:59:41
//
//  @return       GRPOBJECT* :
//
//  @param         :
//  @param        ... :
*/
/*-----------------------------------------------------------------*/
GRPTEXTBOX* GRPFONT::TextBox(GRPSCENE* scene,int alignment_type,XCHAR* mask , ...)
{
  XSTRING word;

  va_list arg;
  va_start(arg, mask);

  word.FormatArg(mask, &arg);

  va_end(arg);

  GRPTEXTBOX* element=CreateTextBox(scene,word.Get(),alignment_type);

  return element;
}

GRPTEXTBOX* GRPFONT::TextBox(GRPSCENE* scene,XCHAR* mask , ...)
{
  XSTRING word;

  va_list arg;
  va_start(arg, mask);

  word.FormatArg(mask, &arg);

  va_end(arg);

  return CreateTextBox(scene,word.Get());
}


/*-------------------------------------------------------------------
//  GRPFONT::CreateTextBox
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      18/08/2014 14:04:38
//
//  @return       GRPOBJECT* :
//
//  @param        word :
*/
/*-----------------------------------------------------------------*/
GRPTEXTBOX* GRPFONT::CreateTextBox(GRPSCENE* scene,XCHAR* word,int alignment_type)
{
  return CreateTextBox(scene,scene->CreateNode(),word,alignment_type);
}

/*-------------------------------------------------------------------
//  GRPFONT::CreateTextBox
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/03/2016 10:34:17
//
//  @return       GRPTEXTBOX* :
//
//  @param        scene :
//  @param        node :
//  @param        word :
//  @param        alignment_type : (default TBOX_ALIGNRIGHT)
*/
/*-----------------------------------------------------------------*/
GRPTEXTBOX* GRPFONT::CreateTextBox(GRPSCENE* scene,GRPNODE* node, XCHAR* word,int alignment_type)
{

  XSTRING s;
  s.Set(word);
  GRPTEXTBOX* text=new GRPTEXTBOX (scene);

              text->SetNode       (node);
              text->SetFont       (this);
              text->SetContent    (&s);
              text->SetAlign      (alignment_type);
            //  text->UpdateContent ();

  return text;
}

/*-------------------------------------------------------------------
//   GRPFONT::CreateTextElement
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  30/01/2018 12:29:54
//  @return   GRPTEXTELEMENT* :
//
//  @param    GRPNODE* :
//  @param    XCHAR* :
//  @param    int :
//
*//*-----------------------------------------------------------------*/
GRPTEXTELEMENT* GRPFONT::CreateTextElement(GRPNODE* node, XCHAR* word, int alignment_type)
{

  XSTRING s;
  s.Set(word);
  GRPTEXTELEMENT* text=new GRPTEXTELEMENT ();
                  text->SetNode           (node);
                  text->SetFont           (this);
                  text->SetContent        (&s);
                  text->SetAlign          (alignment_type);
                  text->UpdateContent     ();
  return text;
}


/*-------------------------------------------------------------------
//   GRPFONT::CreateTextBlock
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  30/01/2018 12:30:08
//  @return   GRPTEXTELEMENT* :
//
//  @param    XCHAR* :
//  @param    GRPAABB* :
//
*//*-----------------------------------------------------------------*/
GRPTEXTELEMENT* GRPFONT::CreateTextBlock(GRPNODE* node,XCHAR* word, GRPAABB*  box)
{
    XSTRING s;
    s.Set(word);
    GRPTEXTELEMENT* text = new GRPTEXTELEMENT();
                    text->SetNode(node);
                    text->SetFont(this);
                    text->SetContent(&s);
                    text->SetAlign(GRPTEXTBOX_ALIGNLEFT);
                    text->SetBoundaries(box);
                    text->UpdateContent();
             return text;
}

/*-------------------------------------------------------------------
//  GRPFONT::MeasureWidth
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      17/03/2017 18:02:27
//
//  @return       bool :
//
//  @param        width :
//  @param        original :
//  @param        trimed :
//  @param        rest :
*/
/*-----------------------------------------------------------------*/
bool   GRPFONT::MeasureWidth(float width,XSTRING& original, XSTRING& trimmed, XSTRING& rest)
{

  float currentWidth=0.0f;
  int n=original.GetSize();
  int e=0;
  bool exit=false;
  for (;e<n;e++)
  {
    if (original.Get()[e]==__C('\b'))
      e++;

    if (original.Get()[e]==__C('\n'))
    {
      e--;
      break;
    }

    float advance=0.0f,kerning=0.0f;
    this->GetTTF()->GetGlyphMetrics(original[e],&advance);

    if (e<n-1)
      kerning=this->GetTTF()->GetKerning(original.Get()[e],original.Get()[e+1]);

    trimmed.Add(original.Get()[e]);

    currentWidth+=(advance+kerning);
    if (currentWidth>width) break;
  }

      original.Copy(e+1,n,rest);
      return true;
}

/*-------------------------------------------------------------------
//   GRPFONT::SetStyle
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  26/01/2018 10:36:12
//  @return   GRPFONT_STYLE :
//
//  @param    XCHAR* :
//
*//*-----------------------------------------------------------------*/
GRPFONT_STYLE  GRPFONT::SetStyle(XCHAR* style)
{
    this->style = GRPFONT_STYLE_REGULAR;

    XSTRING st = style;
    if (st.Find(__L("bold"), true)!=NOTFOUND)
        this->style |= GRPFONT_STYLE_BOLD;

    if (st.Find(__L("italic"), true) != NOTFOUND)
        this->style |= GRPFONT_STYLE_ITALIC;

    return (GRPFONT_STYLE)this->style;
}


