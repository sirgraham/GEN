
/*------------------------------------------------------------------------------------------
//  GRPTEXTBOX.CPP
//
//  Creates 3d text
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 10/11/2014 17:37:18
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include <ctype.h>

#include "GRPTextBox.h"
#include "GRPFont.h"
#include "GRPFileTTF.h"
#include "GRPScene.h"
#include "GRPProfiler.h"

#include "XFactory.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//  GRPTEXTBOX::GRPTEXTBOX
*/
/**
//
//  Class Constructor GRPTEXTBOX
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      11/05/2015 10:56:24
//
*/
/*-----------------------------------------------------------------*/
GRPTEXTBOX::GRPTEXTBOX(GRPSCENE* scene)
{
  Clean();
  this->scene=scene;
}

/*-------------------------------------------------------------------
//  GRPTEXTBOX::~GRPTEXTBOX
*/
/**
//
//   Class Destructor GRPTEXTBOX
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      11/05/2015 10:56:20
//
*/
/*-----------------------------------------------------------------*/
GRPTEXTBOX::~GRPTEXTBOX()
{
  xmutex->Lock();

  isready=false;
  elements.DeleteContents();
  elements.DeleteAll();

  if(this->GetAnimationControl())
    {
      this->GetAnimationControl()->DettachAnimation(this);
    }

  xmutex->UnLock();

  if (xmutex)
  xfactory->Delete_Mutex(xmutex);
  xmutex=NULL;
  Clean();
}



/*-------------------------------------------------------------------
//  GRPTEXTCONTAINER::GRPTEXTCONTAINER
*/
/**
//
//  Class Constructor GRPTEXTCONTAINER
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      26/09/2016 16:04:40
//
*/
/*-----------------------------------------------------------------*/
GRPTEXTCONTAINER::GRPTEXTCONTAINER()
{
  Clean();
  if (xmutex==NULL)
  xmutex=xfactory->Create_Mutex();
  GRPNAMED::classtype = GRPNAMED_TYPE_GRPTEXTBOX;
}

/*-------------------------------------------------------------------
//  GRPTEXTCONTAINER::~GRPTEXTCONTAINER
*/
/**
//
//   Class Destructor GRPTEXTCONTAINER
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      26/09/2016 16:04:46
//
*/
/*-----------------------------------------------------------------*/
GRPTEXTCONTAINER::~GRPTEXTCONTAINER()
{
  if (xmutex)
  xfactory->Delete_Mutex(xmutex);
  xmutex=NULL;
  Clean();
}



/*-------------------------------------------------------------------
//  GRPTEXTCONTAINER::Update
*/
/**
//
//  Updates text field
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      10/11/2014 17:39:05
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool  GRPTEXTBOX::UpdateContent(bool mergeelements)
{
  int i=0;

  xmutex->Lock();

  if (isready==false)
  {
    XDEBUG_PRINTCOLOR(4,__L("GRPTextbox busy when updating"));
    xmutex->UnLock();
    return true;
  }

  isready=false;
  width=0.0f;

  GRPELEMENT*   glyph           = NULL;

  int len       =Content.GetSize();
  XCHAR* buffer =Content.Get();

  GRPMATERIALATTRIBUTES nodeattributes;

  if (this->node!=NULL)
  nodeattributes=*this->node->GetBaseMaterial();

  GRPMATERIALATTRIBUTES elementattributes;
  GRPSHADERPROGRAM*     Program=this->GetProgram();
  GRPMATERIAL*          material=NULL;

  if (this->GetElements()->GetSize()>0)
  {
    Program=this->GetElements()->Get(0)->GetProgram();
    material=this->GetElements()->Get(0)->GetNode()->GetMaterial();

    if (this->GetElements()->Get(0))
    if (this->GetElements()->Get(0)->GetNode())
    elementattributes=*this->GetElements()->FastGet(0)->GetNode()->GetBaseMaterial();
  }

  for (int n=this->GetElements()->GetSize()-1;n>=0;n--)
  {
    if (this->GetElements()->Get(n))
    if (this->GetElements()->FastGet(n)->GetNode())
    if (this->GetElements()->FastGet(n)->GetNode()!=this->node)
    {
              this->GetElements()->FastGet(n)->Dettach();
      delete( this->GetElements()->FastGet(n)->GetNode());
              this->GetElements()->FastGet(n)->SetNode(NULL);
    }
  }

  this->Clear();
  this->node->Update();



  for(int e=0; e<len; e++)
  {
      float kerning=0.0f;

      XCHAR       character     = buffer[e];

      GRPELEMENT* Indexedglyph  = GetIndexedGlyph(character);
      if (!Indexedglyph)
      {
        continue;
      }

      if (e<len)
        kerning=font->GetTTF()->GetKerning(buffer[e],buffer[e+1]);

      if (kerning>0.0f)
      {
        int a=0;
      }

          glyph = new GRPELEMENT();

          if (glyph==NULL)
          {
            isready=true;
            xmutex->UnLock();
            return false;
          }

          GRPNODE* glyph_node=scene->CreateNode();

          if (glyph_node==NULL)
          {
              delete(glyph);
              isready=true;
              xmutex->UnLock();
              return false;
          }

          glyph->SetNode(glyph_node);
          glyph->GetNode()->SetName(Indexedglyph->GetName()->Get());
          glyph->GetNode()->NodeMode=GRPNODE_MODE_OGL;
          glyph->GetNode()->Init();

          this->AddElement(glyph);

        glyph->Reference(Indexedglyph);

        glyph->UpdateBoundingBox();
        glyph->GetBoundingBox()->bearing  =Indexedglyph->GetBoundingBox()->bearing;
        glyph->GetBoundingBox()->ascend   =Indexedglyph->GetBoundingBox()->ascend;
        glyph->GetBoundingBox()->descend  =Indexedglyph->GetBoundingBox()->descend;
        glyph->GetBoundingBox()->advance  =Indexedglyph->GetBoundingBox()->advance;
        glyph->GetBoundingBox()->blocked  =true;
        glyph->GetBoundingBox()->kerning  =kerning;

        glyph->GetNode()->LinkTo(this->GetNode());
        glyph->GetNode()->worldmatrix_is_pending=true;

        glyph->SetRenderPriority(GRPRENDERPRIORITY_HIGHEST);
        glyph->SetProgram(Program);
  }


      switch(align)
      {
      case GRPTEXTBOX_ALIGNRIGHT:   aligntool.align=GRPARRAYALIGNTOOL_ALIGNFLAGS_RIGHT;   break;
      case GRPTEXTBOX_ALIGNLEFT:    aligntool.align=GRPARRAYALIGNTOOL_ALIGNFLAGS_LEFT;    break;
      case GRPTEXTBOX_ALIGNCENTER:  aligntool.align=GRPARRAYALIGNTOOL_ALIGNFLAGS_CENTER;  break;
      }

      switch(direction)
      {
      case GRPTEXTBOX_HORIZONTAL:   aligntool.direction=GRPARRAYALIGNTOOL_ALIGNFLAGS_HORIZONTAL;    break;
      case GRPTEXTBOX_VERTICAL  :   aligntool.direction=GRPARRAYALIGNTOOL_ALIGNFLAGS_VERTICAL;    break;
      }

    aligntool.Align(this);

    this->SetMaterial(this->material);

    this->GetNode()->SetBaseMaterial(&elementattributes);

      for (XDWORD e=0;e<this->GetElements()->GetSize();e++)
        this->GetElements()->Get(e)->GetNode()->SetBaseMaterial(&elementattributes);

  if(mergeelements)
    {
      if (!this->MergeElements())
      {
        xmutex->UnLock();
        isready=true;
        return false;
      }

      if (this->GetElements()->Get(0))
      {
        this->GetElements()->Get(0)->SetRenderPriority(GRPRENDERPRIORITY_HIGHEST);
        this->GetElements()->Get(0)->GetNode()->SetBaseMaterial(&elementattributes);
      }
    }

    this->SetProgram(Program);

    lineheight=font->GetLineHeight();

xmutex->UnLock();
isready=true;
return true;
}


/*-------------------------------------------------------------------
//  GRPTEXTBOX::GetIndexedGlyph
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      16/04/2015 14:38:39
//
//  @return       GRPELEMENT* :
//
//  @param        character :
*/
/*-----------------------------------------------------------------*/
GRPELEMENT* GRPTEXTCONTAINER::GetIndexedGlyph(XCHAR character)
{

        int           indexglyph    = this->FindGlyphIndex(character);
        XDWORD        letter        = font->GetLetters()->Find(character);
        GRPELEMENT*   indexedglyph  = 0;

          if(letter == NOTFOUND)
          {
              indexedglyph = font->CreateGlyph  (character);
              font->GetLetters()->Add           (XDWORD(character), indexedglyph);
              return indexedglyph;
          }

          return font->GetLetters()->Get(XDWORD(character));
}


/*-------------------------------------------------------------------
//  GRPTEXTBOX::SetValue
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/09/2015 17:20:32
//
//  @return       bool :
//
//  @param        GRPANIMATIONCURVETYPE :
//  @param        GRPANIMATIONCURVEAXIS :
//  @param        value :
//  @param        double :
*/
/*-----------------------------------------------------------------*/
bool GRPTEXTBOX::SetValue (GRPANIMATIONCURVETYPE type,GRPANIMATIONCURVEAXIS axis,double value,double base)
{
  xmutex->Lock();

  if (this->isready)
  {
    this->isready=false;
      switch (type)
      {

      case GRPANIMATIONCURVETYPE_USERVALUE:
                            {
                            int n=GRPPROFILER::nNodes;
                              this->Content.Format(contentmask.Get(),value);
                              if (lastValue.Compare(this->Content)!=0)
                              {
                                  if (this->GetNode()->GetName()->Compare(__L("Model::3_02_04_countdowm"))==0)
                                  {
                                    XDEBUG_PRINTCOLOR(0,__L("Countdown: %.f"),value);
                                    if (value<6.0)
                                    {
                                      int a=0;
                                    }
                                  }
                                lastValue             = this->Content;
                                this->isready         = true;
                                this->UpdateContent (true);
                                this->isready         = false;
                                this->scene->Add    (this);
                              }

                              n=GRPPROFILER::nNodes;
                            }
                            break;
      case GRPANIMATIONCURVETYPE_DIFFUSEFACTOR:                           break;
      default:

                            {
                            GetNode()->SetValue(type,axis,value,base);

                            for (XDWORD e=0;e<this->GetElements()->GetSize();e++)
                              if (this->GetElements()->Get(e))
                              if (this->GetElements()->Get(e)->GetNode())
                              this->GetElements()->Get(e)->GetNode()->SetValue(type,axis,value,base);
                            }
                            break;
      }
  }
      this->isready=true;

      xmutex->UnLock();

      return true;
}



/*-------------------------------------------------------------------
//  GRPTEXTBOX::Clear
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/09/2015 17:21:40
//
*/
/*-----------------------------------------------------------------*/
void GRPTEXTBOX::Clear()
{
  xmutex->Lock();

  if (this->elements.GetSize()>0)
    {
      this->elements.DeleteContents();
      this->elements.DeleteAll();
    }

  xmutex->UnLock();

}



/*-------------------------------------------------------------------
//  GRPTEXTBOX::SetContent
*/
/**
//
//  Sets content text
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      10/11/2014 17:39:23
//
//  @return       bool :
//
//  @param        data :
//  @param        size :
-----------------------------------------------------------------*/
bool  GRPTEXTCONTAINER::SetContent(XSTRING * data,XDWORD * size)
{
  xmutex->Lock();

  this->Content.Set(data->Get());

  xmutex->UnLock();

  return true;
}

/*-------------------------------------------------------------------
//  GRPTEXTCONTAINER::SetContentMask
*/
/**
//
//  sets content mask
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      27/09/2016 10:50:33
//
//  @return       bool :
//
//  @param        mask :
*/
/*-----------------------------------------------------------------*/
bool  GRPTEXTCONTAINER::SetContentMask    (XCHAR* mask)
{
  this->contentmask=mask;
  return true;
}

/*-------------------------------------------------------------------
//  GRPTEXTBOX::SetContent
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      16/04/2015 14:39:46
//
//  @return       bool :
//
//  @param        mask :
//  @param        ...  :
*/
/*-----------------------------------------------------------------*/
bool  GRPTEXTCONTAINER::SetContent    (XCHAR* mask , ...)
{

  XSTRING word;

  va_list arg;
  va_start(arg, mask);

  word.FormatArg(mask, &arg);

  va_end(arg);

  if (word.Compare(this->Content) != 0)
  {
    this->Content.Set(word.Get());
    UpdateContent();
  }

return true;
}

/*-------------------------------------------------------------------
//  GRPTEXTBOX::SetNode
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      24/09/2015 16:32:18
//
//  @param        n :
*/
/*-----------------------------------------------------------------*/
void  GRPTEXTBOX::SetNode         (GRPNODE* n)
{
  xmutex->Lock();

  if (n)
  {
  GRPOBJECT::SetNode(n);
  for (XDWORD e=0;e<elements.GetSize();e++)
    elements.Get(e)->GetNode()->LinkTo(n);
  }
  xmutex->UnLock();
}


/*-------------------------------------------------------------------
//  GRPTEXTCONTAINER::FindGlyphIndex
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      11/05/2015 10:56:36
//
//  @return       int :
//
//  @param        character :
*/
/*-----------------------------------------------------------------*/
int   GRPTEXTCONTAINER::FindGlyphIndex(XCHAR character)
{
  return this->font->GetTTF()->FindGlyphIndex(character);
}


/*-------------------------------------------------------------------
//  GRPTEXTCONTAINER::SetFont
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/03/2016 10:15:00
//
//  @param        f :
*/
/*-----------------------------------------------------------------*/
void    GRPTEXTCONTAINER::SetFont(GRPFONT* f)
{
  this->font=f;

}



/*-------------------------------------------------------------------
//  GRPTEXTCONTAINER::GetFont
*/
/**
//
//  gets the font
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      18/10/2016 11:20:03
//
//  @return       GRPFONT* :
//
*/
/*-----------------------------------------------------------------*/
GRPFONT* GRPTEXTCONTAINER::GetFont()
{
  return this->font;
}




/*-------------------------------------------------------------------
//  GRPTEXTCONTAINER::SetWidth
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/03/2016 10:15:04
//
//  @param        fwidth :
*/
/*-----------------------------------------------------------------*/
void    GRPTEXTCONTAINER::SetWidth(float  fwidth)
{
  this->width=fwidth;
}

/*-------------------------------------------------------------------
//  GRPTEXTBOX::SetAlign
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/03/2016 10:15:09
//
//  @param        xalign :
*/
/*-----------------------------------------------------------------*/
void    GRPTEXTCONTAINER::SetAlign(XDWORD xalign)
{
  this->align=xalign;
}

/*-------------------------------------------------------------------
//  GRPTEXTBOX::SetDistribution
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/03/2016 10:15:14
//
//  @param        distribution :
*/
/*-----------------------------------------------------------------*/
void    GRPTEXTCONTAINER::SetDistribution (XDWORD distribution)
{
  this->distribution      = distribution;
  aligntool.distribution  = distribution;
}

/*-------------------------------------------------------------------
//  GRPTEXTBOX::GetAlignTool
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/03/2016 10:15:22
//
//  @return       GRPARRAYALIGNTOOL* :
//
*/
/*-----------------------------------------------------------------*/
GRPARRAYALIGNTOOL*    GRPTEXTCONTAINER::GetAlignTool()
{
  return &this->aligntool;
}

/*-------------------------------------------------------------------
//  GRPTEXTBOX::GetContent
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/03/2016 10:15:28
//
//  @return       XSTRING* :
//
*/
/*-----------------------------------------------------------------*/
XSTRING*  GRPTEXTCONTAINER::GetContent()
{
  return &Content;
}


/*-------------------------------------------------------------------
//  GRPTEXTBOX::GetContentMask
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/03/2016 10:15:38
//
//  @return       XSTRING* :
//
*/
/*-----------------------------------------------------------------*/
XSTRING*  GRPTEXTCONTAINER::GetContentMask()
{
  return &contentmask;
}



/*-------------------------------------------------------------------
//  GRPTEXTELEMENT::UpdateContent
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      27/09/2016 10:31:03
//
//  @return       bool :
//
//  @param        mergeelements :
*/
/*-----------------------------------------------------------------*/
bool GRPTEXTELEMENT::UpdateContent(bool mergeelements)
{

  int i=0;

  xmutex->Lock();

  isready=false;

  width=0.0f;

  bool          isPunctuation   = false;
  GRPELEMENT*   glyph           = NULL;

  int len=Content.GetSize();
  XCHAR* buffer=Content.Get();
  GRPMATERIALATTRIBUTES nodeattributes;
  GRPSHADERPROGRAM*     Program=this->GetProgram();
  GRPMATERIAL*          material=NULL;


  if  (this->node!=NULL)
  {
      nodeattributes=*this->node->GetBaseMaterial();
      material=this->node->GetMaterial();
  }

  GRPELEMENT element;
  GRPOBJECT obj;
            obj.SetNode(new GRPNODE());
            obj.GetNode()->Init();
            obj.GetNode()->Update();
            obj.GetNode()->UpdateWorldMatrix();

  //---------------------------------- processing glyphs
      for(int e=0; e<len; e++)
      {
          XCHAR       character     = buffer[e];
          float kerning=0.0f;

            GRPELEMENT* Indexedglyph  = GetIndexedGlyph(character);
            if (Indexedglyph)
            {
            if (e<len)
            kerning=font->GetTTF()->GetKerning(buffer[e],buffer[e+1]);

            GRPELEMENT* element =   new GRPELEMENT();
            GRPNODE*    node    =   new GRPNODE   ();

                        element->Reference(Indexedglyph);
                        element->SetNode(node);
                        node->LinkTo(obj.GetNode());

                        node->modified=true;
                        node->worldmatrix_is_pending=true;

                        GRPBB* bb=element->GetBoundingBox();
                        GRPBB* ib=Indexedglyph->GetBoundingBox();

                        bb->width       =ib->width;
                        bb->height      =ib->height;
                        bb->depth       =ib->depth;

                        bb->bearing     =ib->bearing;
                        bb->ascend      =ib->ascend;
                        bb->descend     =ib->descend;
                        bb->advance     =ib->advance;
                        bb->blocked     =true;
                        bb->kerning     =kerning;
                        bb->lineheight  =font->lineheight;

            obj.Add(element);
            }
      }

    //---------------------------------- merguing
      switch(align)
      {
      case GRPTEXTBOX_ALIGNRIGHT:   aligntool.align=GRPARRAYALIGNTOOL_ALIGNFLAGS_RIGHT;   break;
      case GRPTEXTBOX_ALIGNLEFT:    aligntool.align=GRPARRAYALIGNTOOL_ALIGNFLAGS_LEFT;    break;
      case GRPTEXTBOX_ALIGNCENTER:  aligntool.align=GRPARRAYALIGNTOOL_ALIGNFLAGS_CENTER;  break;
      }

      switch(direction)
      {
      case GRPTEXTBOX_HORIZONTAL:   aligntool.direction=GRPARRAYALIGNTOOL_ALIGNFLAGS_HORIZONTAL;    break;
      case GRPTEXTBOX_VERTICAL  :   aligntool.direction=GRPARRAYALIGNTOOL_ALIGNFLAGS_VERTICAL;    break;
      }


    float ascend        = 0.0f;
    float descend       = 0.0f;
    float advance       = 0.0f;
    float lineheight    = 0.0f;

    if (obj.GetElements()->GetLast())
    {
      ascend        =obj.GetElements()->GetLast()->GetBoundingBox()->ascend;
      descend       =obj.GetElements()->GetLast()->GetBoundingBox()->descend;
      advance       =obj.GetElements()->GetLast()->GetBoundingBox()->advance;
      lineheight    =obj.GetElements()->GetLast()->GetBoundingBox()->lineheight;
    }


    this->aligntool.Align(&obj,bound);
    obj.MergeElements();

    //-----------------------------------claiming mesh
    if (obj.GetElements()->GetSize())
    this->SetMesh(obj.GetElements()->FastGet(0)->GetMesh());

    this->GetNode()->SetMaterial(material);
    this->GetNode()->SetBaseMaterial(&nodeattributes);
    this->SetProgram(Program);


    //--------------------------- clean up

    if (obj.GetElements()->GetSize())
    {
    obj.GetElements()->FastGet(0)->SetIsInstance(true);
    obj.GetElements()->FastGet(0)->SetMesh(NULL);
    }

    XDWORD n = obj.GetElements()->GetSize();
    for (XDWORD e=0;e<n;e++)
    {
      delete(obj.GetElements()->FastGet(e)->GetNode());
    }

    obj.GetElements()->DeleteContents();
    obj.GetElements()->DeleteAll();

    delete(obj.GetNode());

        this->SetRenderPriority(GRPRENDERPRIORITY_HIGHEST);

    xmutex->UnLock();
  isready=true;

  this->UpdateBoundingBox();
  this->GetBoundingBox()->ascend      = ascend;
  this->GetBoundingBox()->descend     = descend;
  this->GetBoundingBox()->advance     = advance;
  this->GetBoundingBox()->lineheight  = lineheight;

  return true;
}


/*-------------------------------------------------------------------
//  GRPTEXTELEMENT::SetValue
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      27/09/2016 10:31:34
//
//  @return       bool :
//
//  @param        curvetype :
//  @param        curveaxis :
//  @param        value :
//  @param        base :
*/
/*-----------------------------------------------------------------*/
bool GRPTEXTELEMENT::SetValue(GRPANIMATIONCURVETYPE type, GRPANIMATIONCURVEAXIS axis, double value, double base)
{

  xmutex->Lock();

  if (this->isready)
  {
    this->isready=false;
      switch (type)
      {

      case GRPANIMATIONCURVETYPE_USERVALUE:
                            {
                              this->Content.Format(contentmask.Get(),value);
                              if (lastValue.Compare(this->Content)!=0)
                              {
                                lastValue             = this->Content;
                                this->isready         = true;
                                this->UpdateContent   (true);
                                this->isready         = false;
                                this->value           = value;
                              }
                            }
                            break;

      case GRPANIMATIONCURVETYPE_DIFFUSEFACTOR:
                            this->GetNode()->SetValue(type,axis,value,base);
                            break;

      default:
                            this->GetNode()->SetValue(type,axis,value,base);
                            break;

      }
  }
      this->isready=true;

      xmutex->UnLock();

      return true;
  return true;
}

/*-------------------------------------------------------------------
//  GRPTEXTELEMENT::Clear
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      27/09/2016 10:30:50
//
*/
/*-----------------------------------------------------------------*/
void GRPTEXTELEMENT::Clear()
{

}
