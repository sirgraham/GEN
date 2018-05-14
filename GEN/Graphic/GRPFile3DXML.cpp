
/*------------------------------------------------------------------------------------------
//  GRPFILE3DXML.CPP
//
//  Loads layers from XML
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 28/11/2016 13:06:14
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPFile3DXML.h"
#include "GRPObject.h"
#include "GRPLoader.h"
#include "GRPLight.h"
#include "GRPShaderLibrary.h"

#include "GRPFile3DGGF.h"
#include "GRPMaterialLibrary.h"
#include "GRPPassManager.h"
#include "GRPScene.h"

#include "SNDAnimationListener.h"
#include "SNDManager.h"

#include "AILogic.h"
#include "AIState.h"
#include "AIAnimation.h"

#include "XFileXML.h"
#include "XDebug.h"
#include "XVariant.h"
#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/

float GRPFILE3DXML::SOUND::mastervolume=1.0f;



/*-------------------------------------------------------------------
//  GRPFILE3DXML::Load
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/11/2016 13:07:25
//
//  @return       GRPOBJECT* :
//
//  @param        file :
*/
/*-----------------------------------------------------------------*/
GRPOBJECT*    GRPFILE3DXML::Load(XCHAR* file)
{
  GRPFILE3DXML::SOUND::mastervolume=1.0f;

  if (this->object->GetLogic())
    delete(this->object->GetLogic());

  this->object->SetLogic(NULL);

  this->font.SetIsMulti(true);
  this->texts.SetIsMulti(true);


  showdebug=false;

  timer=xfactory->CreateTimer();
  timer->Reset();

  XPATH     xpath;
            xpath=file;

    xmlfile.SetIgnoreComments(true);

    //------------------- Open the XML
    if (!xmlfile.Open(xpath,true))
    {
      Output(4,__L("Unable to open XML file %s"),file);
      Release();
      return NULL;
    }

      if (this->object==NULL)
      {
            object                  =new GRPOBJECT();
            if (!object)
              return NULL;
      }

      if (!this->object->GetLogic())
          object->SetLogic        (new AI::LOGIC());

      if (!this->object->GetLogic())
          return NULL;

      if (!this->object->GetLogic()->GetControllers()->Get(0))
      {
            AI::CONTROLLER* controller=new AI::CONTROLLER();
            if (!controller)
              return NULL;
            controller->SetOwner(object);
            object->GetLogic()->Add (controller);
      }


    //------------------- Read the XML
    if (!xmlfile.ReadAndDecodeAllLines())
    {
      Output(4,__L("Unable to decode XML file %s"),file);
      Release();
      return NULL;
    }

    if (!xmlfile.Close())
    {
      Output(4,__L("Unable to close XML file %s"),file);
      Release();
      return NULL;
    }

    if (!xmlfile.GetRoot())
    {
      Output(4,__L("Unable to access XML root"));
      Release();
      return NULL;
    }

    //------------------- Parse the xml
    if (!ParseFile())
    {
      Output(4,__L("Error parsing XML"));
      Release();
      delete(object);
      object=NULL;
      return NULL;
    }

  Output(0,__L("XML Parsed in %lld"),timer->GetMeasureMilliSeconds());

    //---------- load model

    if (!LoadModel())
    {
          Abort();
          return NULL;
    }


  Output(0,__L("-------------------------------------- loaded in  %lld"),timer->GetMeasureMilliSeconds());

  Release();
  delete(timer);
  timer=NULL;

  return object;
}


/*-------------------------------------------------------------------
//  GRPFILE3DXML::Release
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      29/11/2016 12:23:20
//
*/
/*-----------------------------------------------------------------*/
void  GRPFILE3DXML::Abort()
{
  Release();
}

/*-------------------------------------------------------------------
//  GRPFILE3DXML::Release
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      06/03/2017 16:10:12
//
*/
/*-----------------------------------------------------------------*/
void  GRPFILE3DXML::Release()
{
  this->header.DeleteKeyContents();
  this->header.DeleteElementContents();
  this->header.DeleteAll();

  this->definitions.DeleteKeyContents();
  this->definitions.DeleteAll();

  this->languages.DeleteElementContents();
  this->languages.DeleteAll();

  this->layers.DeleteElementContents();
  this->layers.DeleteAll();

  this->lights.DeleteElementContents();
  this->lights.DeleteAll();

  this->xmlfile.DeleteAllCFGAtributes();
  this->xmlfile.DeleteAllElements();
  this->xmlfile.DecodeAllLines();

  this->texts.DeleteElementContents();
  this->texts.DeleteAll();

  this->commands.DeleteElementContents();
  this->commands.DeleteAll();

  this->font.DeleteElementContents();
  this->font.DeleteAll();

  this->overlays.DeleteElementContents();
  this->overlays.DeleteAll();

  this->media.DeleteElementContents();
  this->media.DeleteAll();

  this->sounds.DeleteContents();
  this->sounds.DeleteAll();

  this->music.DeleteElementContents();
  this->music.DeleteAll();

  this->colors.DeleteElementContents();
  this->colors.DeleteAll();

  this->layerOptions.DeleteKeyContents      ();
  this->layerOptions.DeleteElementContents  ();

  this->materials.DeleteElementContents     ();
  this->passes.DeleteElementContents        ();

  this->elements.DeleteElementContents      ();
  this->elements.DeleteAll                  ();

  if (timer)
  {
    delete(timer);
    timer=NULL;
  }

  if (chronometer)
  {
    xfactory->DeleteTimer(chronometer);
    chronometer=NULL;
  }

  xmlfile.Close();
  delete(this->file3dfactory);
  this->file3dfactory = NULL;
}


/*-------------------------------------------------------------------
//  GRPFILE3DXML::ParseFile
*/
/**
//
//  parses file
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/11/2016 14:04:06
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool  GRPFILE3DXML::ParseFile ()
{
  int i=0;
  XFILEXMLELEMENT* root   = xmlfile.GetRoot();
  xmlfile.SetIgnoreComments(true);

        //-----------head
        XFILEXMLELEMENT* head=xmlfile.SearchChild(__L("head"),root);
        if (head!=NULL)
        {
          i=0;
          if (!ProcessHead(head))
            return false;
        }
        else
        {
          Output(4,__L("No head present on document: %s"),this->file.Get());
        }


        //-----------config
        XFILEXMLELEMENT* config=xmlfile.SearchChild(__L("config"),root);
        if (config!=NULL)
        {
          if (!ProcessConfig(config))
            return false;
        }
        else
        {
          Output(4,__L("No config present on document: %s"),this->file.Get());
        }

        i=0;
        //-----------definitions
        XFILEXMLELEMENT* definitions=xmlfile.SearchChild(__L("definitions"),root);
        if (definitions!=NULL)
        {
          if (!ProcessDefinitions(definitions))
            return false;
        }
        else
        {
          Output(4,__L("No definitions present on document: %s"),this->file.Get());
        }

        i=0;
        //------------ strings
        XFILEXMLELEMENT* strings=xmlfile.SearchChild(__L("strings"),root);
        if (strings!=NULL)
        {
          if (!ProcessStrings(strings))
            return false;
        }
        else
        {
          Output(4,__L("No strings present on document: %s"),this->file.Get());
        }

        i=0;
        //----------- layers
        XFILEXMLELEMENT* layers=xmlfile.SearchChild(__L("layers"),root);
        if (layers!=NULL)
        {
          if (!ProcessLayers(layers))
            return false;
        }
        else
        {
          Output(4,__L("No layers present on document: %s"),this->file.Get());
        }

        i=0;
        //----------- states
        XFILEXMLELEMENT* states=xmlfile.SearchChild(__L("states"),root);
        if (states!=NULL)
        {
          if (!ProcessStates(states))
            return false;
        }
        else
        {
          Output(4,__L("No states present on document: %s"),this->file.Get());
        }

        i=0;
        //----------- milestones
        XFILEXMLELEMENT* milestones=xmlfile.SearchChild(__L("milestones"),root);
        if (milestones!=NULL)
        {
          if (!ProcessMilestones(milestones))
            return false;
        }
        else
        {
          Output(4,__L("No milestones present on document: %s"),this->file.Get());
        }

        i=0;
        //----------- overlays
        XFILEXMLELEMENT* overlays=xmlfile.SearchChild(__L("overlays"),root);
        if (overlays!=NULL)
        {
          if (!ProcessOverlays(overlays))
            return false;
        }
        else
        {
          Output(4,__L("No overlays present on document: %s"),this->file.Get());
        }

        //----------- elements
        XFILEXMLELEMENT* elements = xmlfile.SearchChild(__L("elements"), root);
        if (elements != NULL)
        {
            if (!ProcessElements(elements))
                return false;
        }

        i=0;
        //----------- texts
        XFILEXMLELEMENT* texts=xmlfile.SearchChild(__L("texts"),root);
        if (texts!=NULL)
        {
          if (!ProcessTexts(texts))
            return false;
        }
        else
        {
          Output(0,__L("No texts present on document: %s"),this->file.Get());
        }

        i=0;
        //----------- commands
        XFILEXMLELEMENT* commands=xmlfile.SearchChild(__L("commands"),root);
        if (commands!=NULL)
        {
          if (!ProcessCommands(commands))
            return false;
        }
        else
        {
          Output(0,__L("No commands present on document: %s"),this->file.Get());
        }

        i = 0;
        //-----------materials
        XFILEXMLELEMENT* materials = xmlfile.SearchChild(__L("materials"), root);
        if (materials != NULL)
        {
            if (!ProcessMaterials(materials))
                return false;
        }

        i = 0;
        //-----------passes
        XFILEXMLELEMENT* passes = xmlfile.SearchChild(__L("passes"), root);
        if (passes != NULL)
        {
            if (!ProcessPasses(passes))
                return false;
        }
        i=0;
        //----------- audio
        XFILEXMLELEMENT* audio=xmlfile.SearchChild(__L("audio"),root);
        if (audio!=NULL)
        {
          if (!ProcessAudio(audio))
            return false;
        }
        else
        {
          Output(4,__L("No audio present on document: %s"),this->file.Get());
        }


  return true;
}


/*-------------------------------------------------------------------
//  GRPFILE3DXML::ProcessHead
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/11/2016 14:29:34
//
//  @return       bool :
//
//  @param        element :
*/
/*-----------------------------------------------------------------*/
bool  GRPFILE3DXML::ProcessHead(XFILEXMLELEMENT* head)
{
  int n=head->GetNElements();

  this->header.SetIsMulti(true);

  for (int e=0;e<n;e++)
  {
    XFILEXMLELEMENT* item=head->GetElement(e);
    if (item->GetType()==XFILEXMLELEMENTTYPE_COMMENT)
      continue;
    XSTRING* key  =new XSTRING(item->GetName());
    XSTRING* value=new XSTRING(item->GetValue());
    if (!this->header.Add(key,value))
    {
      delete(key);
      delete(value);
      continue;
    }
  }

  if (showdebug)
  for (XDWORD e=0;e<header.GetSize();e++)
  {
    Output(1,__L("Head: %s  :  %s"),header.GetKey(e)->Get(),header.GetElement(e)->Get());
  }

  return true;
}

/*-------------------------------------------------------------------
//  GRPFILE3DXML::ProcessConfig
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      30/11/2016 10:14:01
//
//  @return       bool :
//
//  @param        element :
*/
/*-----------------------------------------------------------------*/
bool  GRPFILE3DXML::ProcessConfig(XFILEXMLELEMENT* config)
{
  int n=config->GetNElements();

  for (int e=0;e<n;e++)
  {
    XFILEXMLELEMENT* item=config->GetElement(e);
    if (item->GetType()==XFILEXMLELEMENTTYPE_COMMENT)
      continue;
        XSTRING key;
        key=item->GetName();

        if (key.Compare(__L("font"))==0)
        {
        GRPFILE3DXML::FONT* value =new GRPFILE3DXML::FONT();
        if (!value)
          return false;

        if (!item->GetValueAttribute(__L("name"),value->name))
          return false;

        if (!item->GetValueAttribute(__L("file"),value->file))
          return false;

        if (item->GetValueAttribute(__L("quality")))
            value->attributes.quality=item->GetIntValueAttribute(__L("quality"));

        if (item->GetValueAttribute(__L("back")))
            value->attributes.back=item->GetBoolValueAttribute(__L("back"));

        if (item->GetValueAttribute(__L("extrude")))
            value->attributes.extrude=(float)item->GetFloatValueAttribute(__L("extrude"));

        if (item->GetValueAttribute(__L("front")))
            value->attributes.front=item->GetBoolValueAttribute(__L("front"));

        if (item->GetValueAttribute(__L("reversenormals")))
            value->attributes.reversenormals=item->GetBoolValueAttribute(__L("reversenormals"));

        if (item->GetValueAttribute(__L("unifynormals")))
            value->attributes.unifynormals=item->GetBoolValueAttribute(__L("unifynormals"));

        if (!this->font.Add(&value->name,value))
          delete(value);
        }
        else
        if (key.Compare(__L("light"))==0)
        {
          GRPFILE3DXML::LIGHT* light=new GRPFILE3DXML::LIGHT();
          if (!light)
            return false;

          light->flags=0;

          item->GetValueAttribute(__L("name"),light->name);

          if (item->GetValueAttribute(__L("multiplier")))
          {
            light->multiplier=(float)item->GetFloatValueAttribute(__L("multiplier"));
            light->flags|=GRPFILE3DXML::LIGHT::MULTIPLIER;
          }

          if (item->GetValueAttribute(__L("constantattenuattion")))
          {
            light->constantAttenuattion=(float)item->GetFloatValueAttribute(__L("constantattenuattion"));
            light->flags|=GRPFILE3DXML::LIGHT::CONSTANT_ATTENUATION;
          }

          if (item->GetValueAttribute(__L("linearattenuattion")))
          {
            light->linearAttenuattion=(float)item->GetFloatValueAttribute(__L("linearattenuattion"));
            light->flags|=GRPFILE3DXML::LIGHT::LINEAR_ATTENUATION;
          }


          if (item->GetValueAttribute(__L("quadraticattenuattion")))
          {
            light->quadricAttenuattion=(float)item->GetFloatValueAttribute(__L("quadraticattenuattion"));
            light->flags|=GRPFILE3DXML::LIGHT::QUADRIC_ATTENUATION;
          }

          if (item->GetValueAttribute(__L("cubicattenuattion")))
          {
            light->cubicAttenuattion=(float)item->GetFloatValueAttribute(__L("cubicattenuattion"));
            light->flags|=GRPFILE3DXML::LIGHT::CUBIC_ATTENUATION;
          }

        if (!this->lights.Add(&light->name,light))
          delete light;

        }
        else
        if (key.Compare(__L("scale"))==0)
        {
          GlobalScale=GRPVECTOR(1.0f,1.0f,1.0f);

          if (item->GetValueAttribute(__L("x")))
            GlobalScale.vector[0]=(float)item->GetFloatValueAttribute(__L("x"));
          if (item->GetValueAttribute(__L("y")))
            GlobalScale.vector[0]=(float)item->GetFloatValueAttribute(__L("y"));
          if (item->GetValueAttribute(__L("z")))
            GlobalScale.vector[0]=(float)item->GetFloatValueAttribute(__L("z"));
        }

  }

  for (XDWORD e=0;e<this->font.GetSize();e++)
  {
    if (showdebug)
    { Output(1,__L("Font: %s  :  %s"),this->font.GetKey(e)->Get(),this->font.GetElement(e)->file.Get());}

      GRPFILE3DXML::FONT* font=this->font.GetElement(e);
      XPATH xpath;
      xpath.Create(XPATHSMANAGERSECTIONTYPE_FONTS, 1,font->file.Get());
      if(!GRPOBJECTBUILDER::Get()->GetFontManager()->Load(xpath.Get(),  font->name.Get(), &font->attributes)) //load default
      {
        Output(1,__L("TTF file not loaded! %s"), xpath.Get());
        return false;
      }
  }

  return true;
}



/*-------------------------------------------------------------------
//  GRPFILE3DXML::ProcessDefinitions
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/11/2016 14:29:46
//
//  @return       bool :
//
//  @param        element :
*/
/*-----------------------------------------------------------------*/
bool  GRPFILE3DXML::ProcessDefinitions(XFILEXMLELEMENT* definitions)
{
  int i=0;

  int n=definitions->GetNElements();

  this->definitions.elements.SetIsMulti(true);

  for (int e=0;e<n;e++)
  {
    XFILEXMLELEMENT* item=definitions->GetElement(e);
    if (item)
    {
    if (item->GetType()==XFILEXMLELEMENTTYPE_COMMENT)
      continue;

    XDWORD value=item->GetIntValueAttribute(__L("value"));
    XSTRING* name=new XSTRING(item->GetValueAttribute(__L("name")));
    if (!name)
      return false;

      if (!this->definitions.Add(name,value))
      {
      Output(4,__L("Error! Redefinition of %s:%d as %s:%d"),name,this->definitions.Get(name),name,value);
      delete(name);
      }
    }
  }

  if (showdebug)
  for (XDWORD e=0;e<this->definitions.GetSize();e++)
  {
    Output(1,__L("\tDefinitions: %d  :  %s"),this->definitions.GetElement(e),this->definitions.GetKey(e)->Get());
  }

return true;
}


/*-------------------------------------------------------------------
//  GRPFILE3DXML::ProcessData
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/02/2017 15:45:23
//
//  @return       bool :
//
//  @param        element :
*/
/*-----------------------------------------------------------------*/
bool  GRPFILE3DXML::ProcessData(XFILEXMLELEMENT* element)
{
  int n=element->GetNElements();
  for (int e=0;e<n;e++)
  {
    XFILEXMLELEMENT* item=element->GetElement(e);
    if (item->GetType()==XFILEXMLELEMENTTYPE_COMMENT)
      continue;

    if (item->GetName().Compare(__L("definitions"))==0)
    {
      if (!ProcessDefinitions(item))
        return false;
    }

    if (item->GetName().Compare(__L("colors"))==0)
    {
      if (!ProcessColors(item))
        return false;
    }


    if (item->GetName().Compare(__L("variables"))==0)
    {
      if (!ProcessVariables(item))
        return false;
    }

  }
  return true;
}


/*-------------------------------------------------------------------
//  GRPFILE3DXML::ProcessColors
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/02/2017 16:12:08
//
//  @return       bool :
//
//  @param        element :
*/
/*-----------------------------------------------------------------*/
bool GRPFILE3DXML::ProcessColors(XFILEXMLELEMENT* element)
{
  int n=element->GetNElements();
  for (int e=0;e<n;e++)
  {
    XFILEXMLELEMENT* item=element->GetElement(e);
    if (item->GetType()==XFILEXMLELEMENTTYPE_COMMENT)
      continue;

    if (item->GetName().Compare(__L("definitions"))==0)
    {
      if (!ProcessDefinitions(item))
        return false;
    }

    if (item->GetName().Compare(__L("color"))==0)
    {
      XSTRING id;
      if (item->GetValueAttribute(__L("id"),id))
      {
        XSTRING encoding,rgba;

        XDWORD value=this->GetValueByDefinition(id.Get());
        if (value==0)
          return false;

          if (item->GetValueAttribute(__L("value"),rgba))
          {
            GRPCOLOR* color=new GRPCOLOR(rgba.Get());
            if (color)
            {
              if (!this->colors.Add(value,color))
                if (!this->colors.Set(value,color))
                  return false;
            }
          }
      }
    }
  }
  return true;
}

/*-------------------------------------------------------------------
//  GRPFILE3DXML::ProcessVariables
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      01/03/2017 10:40:50
//
//  @return       bool :
//
//  @param        element :
*/
/*-----------------------------------------------------------------*/
bool GRPFILE3DXML::ProcessVariables(XFILEXMLELEMENT* element)
{
    int n=element->GetNElements();
  for (int e=0;e<n;e++)
  {
    XFILEXMLELEMENT* item=element->GetElement(e);
    if (item->GetType()==XFILEXMLELEMENTTYPE_COMMENT)   continue;
    if (!item->GetName().Compare(__L("variable"))==0)   continue;

    XSTRING id;
    if (!item->GetValueAttribute(__L("id"),id))  continue;

    XSTRING encoding;
    XDWORD name=this->GetValueByDefinition(id.Get());
    if (name==0) continue;

    XSTRING value;
    if (item->GetValueAttribute(__L("value"),value))
    {
      XVARIANT* variable=new XVARIANT();

      XVECTOR<XSTRING*> split;
      value.Explode(__C(':'),&split);
      if (split.GetSize()==2)
      {
        if (split.Get(0)->Compare(__L("bool"))==0)
          *variable=(bool)split.Get(1)->ConvertToBoolean();
        else
        if (split.Get(0)->Compare(__L("int"))==0)
          *variable=(int)split.Get(1)->ConvertToInt();
        else
        if (split.Get(0)->Compare(__L("float"))==0)
          *variable=(float)split.Get(1)->ConvertToDouble();
        else
        if (split.Get(0)->Compare(__L("string"))==0)
          *variable=split.Get(1)->Get();
        else
        if (split.Get(0)->Compare(__L("color"))==0)
        {
          XDWORD colorID=this->GetValueByDefinition(split.Get(1)->Get());
          if (colorID!=0)
            *variable=((XSTRING*)this->colors.Get(colorID)->GetString())->Get();
        }
      }
      split.DeleteContents();
      split.DeleteAll();
      this->object->GetLogic()->GetControllers()->Get(0)->GetVariables()->Add(name,variable);
    }
  }
  return true;
}

/*-------------------------------------------------------------------
//  GRPFILE3DXML::ProcessStrings
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/02/2017 9:50:10
//
//  @return       bool :
//
//  @param        element :
*/
/*-----------------------------------------------------------------*/
bool  GRPFILE3DXML::ProcessStrings(XFILEXMLELEMENT* element)
{

  XSTRING defaultlanguage=element->GetValueAttribute(__L("default"));

  int n=element->GetNElements();
  for (int e=0;e<n;e++)
  {
    XFILEXMLELEMENT* item=element->GetElement(e);
    if (item->GetType()==XFILEXMLELEMENTTYPE_COMMENT)
      continue;

    if (item->GetName().Compare(__L("definitions"))==0)
    {
      ProcessDefinitions(item);
    }
    else
    if (item->GetName().Compare(__L("language"))==0)
    {
      XSTRING name;
      if (item->GetValueAttribute(__L("name"),name))
      {
        if (name.Compare(defaultlanguage)==0)
        {
          LANGUAGE* language=new LANGUAGE();
          if (ProcessLenguage(item,language))
            this->languages.Add(&language->name,language);
        }
      }
    }
  }

  return true;
}

/*-------------------------------------------------------------------
//  GRPFILE3DXML::ProcessLenguage
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/02/2017 10:01:10
//
//  @return       bool :
//
//  @param        element :
//  @param        language :
*/
/*-----------------------------------------------------------------*/
bool  GRPFILE3DXML::ProcessLenguage(XFILEXMLELEMENT* element,LANGUAGE* language)
{
  int n=element->GetNElements();

  for (int e=0;e<n;e++)
  {
    XFILEXMLELEMENT* item=element->GetElement(e);
    if (item->GetName().Compare(__L("string"))==0)
    {
      XSTRING* id=new XSTRING();
      XSTRING string;
      if (item->GetValueAttribute(__L("id"),string))
      {
        id->Set(string);

        XSTRING* value=new XSTRING();
        if (item->GetValueAttribute(__L("value"),string))
          value->Set(string);

          language->strings.Add(id,value);
      }
    }
  }
    return true;
}

/*-------------------------------------------------------------------
//  GRPFILE3DXML::ProcessLayers
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/11/2016 14:29:55
//
//  @return       bool :
//
//  @param        element :
*/
/*-----------------------------------------------------------------*/
bool  GRPFILE3DXML::ProcessLayers(XFILEXMLELEMENT* layers)
{

  int i=0;

//---------

  int n=layers->GetNAttributes();
  for (int e=0;e<n;e++)
  {
    XSTRING* name =new XSTRING();
    XSTRING* value=new XSTRING();

    name->Set (layers->GetAttribute(e)->GetName ());
    value->Set(layers->GetAttribute(e)->GetValue());

    layerOptions.Add(name,value);
  }

//---------

       n=layers->GetNElements();
  this->definitions.SetIsMulti(true);
  for (int e=0;e<n;e++)
  {
    XFILEXMLELEMENT* item=layers->GetElement(e);
    if (item)
    {
      if (item->GetType()==XFILEXMLELEMENTTYPE_COMMENT)
      continue;

      if (item->GetName().Compare(__L("layer"))!=0)
        continue;

      GRPFILE3DXML::LAYER* layer=new GRPFILE3DXML::LAYER();
      if (!layer)
        return false;

      //--------------------------------- file name, mandatory and minimal

      XCHAR* file=item->GetValueAttribute(__L("file"));

      if (file)
        layer->file=file;
      else
      {
        XSTRING error;
        error.Format(__L("Layer defined without file definition (layer:%)"),e);
        Output(4,error.Get());
        return false;
      }


      //--------------------------------- stack id

      XCHAR* id=item->GetValueAttribute(__L("id"));
      if (id)
      {
        XSTRING layername;
                layername=id;

        layer->layerID=GetValueByDefinition(layername.Get());;
        layer->name=id;
      }
      else
      {
        XSTRING error;
        error.Format(__L("Layer defined without id definition (layer:%)"),e);
        Output(4,error.Get());
        return false;
      }

      //--------------------------------- initial state status

      layer->isActive=false;
      XCHAR* status=item->GetValueAttribute(__L("status"));
      if (status)
      {
        XSTRING b;
                b=status;
        if (b.Compare(__L("true"))==0)
          layer->isActive=true;
      }

      //--------------------------------- base speed

      layer->speed=1.0f;
      XCHAR* speed=item->GetValueAttribute(__L("speed"));
      if (speed)
      {
        XSTRING b;
                b=speed;
        layer->speed=b.ConvertToFloat();
      }

      //--------------------------------- loop modes

      layer->loop=GRPANIMATIONLOOPMODE_NONE;
      XCHAR* loop=item->GetValueAttribute(__L("loop"));
      if (loop)
      {
        XSTRING l;
                l=loop;
        if (l.Compare(__L("repeat"))==0)
          layer->loop=GRPANIMATIONLOOPMODE_REPEAT;
        else
        if (l.Compare(__L("pingpong"))==0)
          layer->loop=GRPANIMATIONLOOPMODE_PINGPONG;
      }

      //--------------------------------- trim animation

      layer->trim=0x00;

      XCHAR* trim=item->GetValueAttribute(__L("trim_end"));
      if (trim)
      {
        XSTRING trimvalues;
        layer->trim|=0x02;
        layer->trimend=item->GetIntValueAttribute(__L("trim_end"));
      }
      trim=item->GetValueAttribute(__L("trim_start"));
      if (trim)
      {
        XSTRING trimvalues;
        layer->trim|=0x01;
        layer->trimend=item->GetIntValueAttribute(__L("trim_start"));
      }


      //--------------------------------- load flags

      layer->loadFlags=GRPFILE3D_INCLUDE_ANIMATION;
      XCHAR* flagsvalue=item->GetValueAttribute(__L("flags"));
      if (flagsvalue)
      {
        XSTRING flags;
                flags=flagsvalue;

        if (flags.Find(__L("All"),false)!=NOTFOUND)
        {
          layer->loadFlags|=GRPFILE3D_INCLUDE_ALL;
        }
        if (flags.Find(__L("Geometry"),false)!=NOTFOUND)
        {
          layer->loadFlags|=GRPFILE3D_INCLUDE_NEW_GEOMETRY;
        }
        if (flags.Find(__L("Materials"),false)!=NOTFOUND)
        {
          layer->loadFlags|=GRPFILE3D_INCLUDE_MATERIALS;
        }
        if (flags.Find(__L("Animation"),false)!=NOTFOUND)
        {
          layer->loadFlags|=GRPFILE3D_INCLUDE_ANIMATION;
        }
        if (flags.Find(__L("Cameras"),false)!=NOTFOUND)
        {
          layer->loadFlags|=GRPFILE3D_INCLUDE_CAMERAS;
        }
        if (flags.Find(__L("Lights"),false)!=NOTFOUND)
        {
          layer->loadFlags|=GRPFILE3D_INCLUDE_LIGHTS;
        }
        if (flags.Find(__L("Deformers"),false)!=NOTFOUND)
        {
          layer->loadFlags|=GRPFILE3D_INCLUDE_DEFORMERS;
        }
        if (flags.Find(__L("Locators"),false)!=NOTFOUND)
        {
          layer->loadFlags|=GRPFILE3D_INCLUDE_LOCATORS;
        }

      }

    if (!this->layers.Add(layer->layerID,layer))
      delete(layer);
    }
  }

  if (showdebug)
  for (XDWORD e=0;e<this->layers.GetSize();e++)
  {

    Output(0,__L("[%03d] : Id : %03d Name: %045s File: %045s Status: %05s Flags: %d Loop:%d Speed: %f"),
      e,
      this->layers.GetElement(e)->layerID,
      this->layers.GetElement(e)->name.Get(),
      this->layers.GetElement(e)->file.Get(),
      this->layers.GetElement(e)->isActive ? __L("true") : __L("false"),
      this->layers.GetElement(e)->loadFlags,
      this->layers.GetElement(e)->loop,
      this->layers.GetElement(e)->speed);
  }


  return true;
}


/*-------------------------------------------------------------------
//  GRPFILE3DXML::ProcessStates
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      01/12/2016 17:19:31
//
//  @return       bool :
//
//  @param        element :
*/
/*-----------------------------------------------------------------*/
bool GRPFILE3DXML::ProcessStates(XFILEXMLELEMENT* element)
{
  AI::CONTROLLER* controller=this->object->GetLogic()->GetControllers()->GetLast();

  if (element->GetNAttributes()>0)
  {
    XCHAR* def;
            def=element->GetValueAttribute(__L("default"));
            if (def)
            {
                XSTRING defs;
                defs = def;
                XDWORD id = GetValueByDefinition(defs.Get());
                controller->SetDefaultState(id);
            }

            def = element->GetValueAttribute(__L("verbose"));
            if (def)
            {
                XSTRING defs;
                defs = def;
                bool value = defs.ConvertToBoolean();
                controller->SetVerbose(value);
            }
  }

  int nStates=element->GetNElements();
  for (int e=0;e<nStates;e++)
  {
    XFILEXMLELEMENT* item=element->GetElement(e);
    if (item)
    {
      if (item->GetType()==XFILEXMLELEMENTTYPE_COMMENT)
      continue;

      XCHAR* idvalue=item->GetValueAttribute(__L("id"));
      if (idvalue)
      {
        XSTRING layername;
                layername = idvalue;
        XDWORD id         = GetValueByDefinition(layername.Get());;

        AI::STATE* state  = new AI::STATE(id,idvalue);
        if (state)
        {
                    state->SetName      (layername.Get());

                    int index=0;

                    //--------------- Animations
                    while (1)
                    {
                    XFILEXMLELEMENT* animationnode=xmlfile.SearchElement(__L("animations"),index,item);
                    if (animationnode)
                      ProcessAnimations   (animationnode,state);
                    else
                      break;
                    index++;
                    }

                    index=0;

                    while (1)
                    {
                    XFILEXMLELEMENT* animationnode=xmlfile.SearchElement(__L("track"),index,item);
                    if (animationnode)
                      ProcessAnimations   (animationnode,state);
                    else
                      break;
                    index++;
                    }

                    index=0;

                    //--------------- Transitions
                    XFILEXMLELEMENT* transitionnode=xmlfile.SearchElement(__L("transitions"),index,item);
                    if (transitionnode)
                      ProcessTransitions  (transitionnode,state);

                    if (controller->GetStates()->Find(id)==NOTFOUND) //el estado no existe
                    {
                        controller->Add(id,state);  //lo añadimos
                    }
                    else
                      {
                        Output(4,__L("GRPFILE3DXML::ProcessStates: state redefinition %s"),state->GetName()->Get());
                        delete(state);
                      }
                    }
      }
    }
  }

  if (showdebug)
  for (XDWORD e=0;e<controller->GetStates()->GetSize();e++)
  {
    AI::STATE* state=controller->GetStates()->GetElement(e);
    if (state)
    {
    Output(1,__L("State: %02d %s"),state->GetIndex(),state->GetName()->Get());
      for (XDWORD t=0;t<state->GetTracks()->GetSize();t++)
      {
        AI::TRACK* track=state->GetTracks()->Get(t);
        for (XDWORD i=0;i<track->animation.GetSize();i++)
        {
          AI::ANIMATION* ani=track->animation.Get(i);
          Output(2,__L("\tAnimation: %02d %s %d %d"),ani->id,ani->name.Get(),ani->loops,ani->transition);
        }
      }
    }
  }

  return true;
}

/*-------------------------------------------------------------------
//  GRPFILE3DXML::ProcessAnimations
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      02/12/2016 12:24:45
//
//  @return       bool :
//
//  @param        element :
//  @param        state :
*/
/*-----------------------------------------------------------------*/
bool GRPFILE3DXML::ProcessAnimations(XFILEXMLELEMENT* element,AI::STATE* state)
{
  AI::TRACK* track=new AI::TRACK();
  if (!track) return false;

  state->Add(track);

  XCHAR* trackname=element->GetValueAttribute(__L("name"));
  if (trackname)
    track->name=trackname;

  int nAnimations=element->GetNElements();

  XSTRING loops,milestone,accept,onend,onstart;

  for (int e=0;e<nAnimations;e++)
  {
    XFILEXMLELEMENT* item=element->GetElement(e);
    if (item)
    {
      if (item->GetType()==XFILEXMLELEMENTTYPE_COMMENT)
      continue;

      XCHAR* idvalue=item->GetValueAttribute(__L("id"));
      if (idvalue)
      {
        XDWORD id = GetValueByDefinition(idvalue);

        int nLoops=0,transition_milestone=-1;

        if (item->GetValueAttribute(__L("loops"),loops))
          nLoops=loops.ConvertToInt();

        if (item->GetValueAttribute(__L("milestone"),milestone))
          transition_milestone=milestone.ConvertToInt();
        else
          transition_milestone=-1;

        AI::ANIMATION* animation=new AI::ANIMATION(id,loops.ConvertToInt(),transition_milestone);
        if (animation)
        {
        animation->name.Set(idvalue);
        state->Add(animation);

        animation->onStartEvent=0;
        animation->onEndEvent=0;

          //-------------------------------------------------- accept
          if (item->GetValueAttribute(__L("accept"),accept))
          {
            if (accept.Compare(__L("always"))==0)
              animation->acceptor=AI::TRANSITION::ACCEPT_ALWAYS;
            else
            if (accept.Compare(__L("finished"))==0)
              animation->acceptor=AI::TRANSITION::ACCEPT_ONLY_IF_FINISHED;
            else
            if (accept.Compare(__L("never"))==0)
              animation->acceptor=AI::TRANSITION::ACCEPT_NEVER;
          }

          //-------------------------------------------------- onstart event
          if (item->GetValueAttribute(__L("onStart"),onstart))
          {
            if (onstart.Find(__L("pause"),false,0)!=NOTFOUND)
              animation->onStartEvent|=AI::TRANSITION::ENDEVENT_PAUSE;
            else
            if (onstart.Find(__L("forward"),false,0)!=NOTFOUND)
              animation->onStartEvent|=AI::TRANSITION::ENDEVENT_FORWARD;
            else
            if (onstart.Find(__L("rewind"),false,0)!=NOTFOUND)
              animation->onStartEvent|=AI::TRANSITION::ENDEVENT_REWIND;
            else
            if (onstart.Find(__L("notify"),false,0)!=NOTFOUND)
              animation->onStartEvent|=AI::TRANSITION::ENDEVENT_MSG;
          }

          //-------------------------------------------------- onend event
          if (item->GetValueAttribute(__L("onEnd"),onend))
          {
            if (onend.Find(__L("pause"),false,0)!=NOTFOUND)
              animation->onEndEvent|=AI::TRANSITION::ENDEVENT_PAUSE;
            else
            if (onend.Find(__L("forward"),false,0)!=NOTFOUND)
              animation->onEndEvent|=AI::TRANSITION::ENDEVENT_FORWARD;
            else
            if (onend.Find(__L("rewind"),false,0)!=NOTFOUND)
              animation->onEndEvent|=AI::TRANSITION::ENDEVENT_REWIND;
            else
            if (onend.Find(__L("notify"),false,0)!=NOTFOUND)
              animation->onEndEvent|=AI::TRANSITION::ENDEVENT_MSG;
          }

          //-------------------------------------------------- process attached anims
          for (int i=0;i<item->GetNElements();i++)
          {
            XFILEXMLELEMENT* attached=item->GetElement(i);
            if (attached)
            {
              XDWORD id,loops,to;

              XCHAR* idvalue=attached->GetValueAttribute(__L("id"));
              if (idvalue)
                id  = GetValueByDefinition(idvalue);
              else
              {
                Output(4,__L("GRPFILE3DXML::ProcessAnimations definicion desconocida : %s"),idvalue);
                continue;
              }

              XCHAR* loopsvalue=attached->GetValueAttribute(__L("passes"));
              if (loopsvalue)
                loops = attached->GetIntValueAttribute(__L("passes"));
              else
                loops=1;

              XCHAR* transition=attached->GetValueAttribute(__L("milestone"));
              if (transition)
                to  = attached->GetIntValueAttribute(__L("milestone"));
              else
                to  =-1;

              AI::ANIMATION* attached_animation=new AI::ANIMATION(id,loops,to);
              if (attached_animation)
                if (!animation->Attachees.Add(attached_animation))
                  delete(attached_animation);
            }
          }
        }
      }
    }
  }
  return true;
}


/*-------------------------------------------------------------------
//  GRPFILE3DXML::ProcessTransitions
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      02/12/2016 12:24:54
//
//  @return       bool :
//
//  @param        element :
//  @param        state :
*/
/*-----------------------------------------------------------------*/
bool GRPFILE3DXML::ProcessTransitions(XFILEXMLELEMENT* element,AI::STATE* state)
{
  int nTransitions=element->GetNElements();

  for (int e=0;e<nTransitions;e++)
  {
    XFILEXMLELEMENT* item=element->GetElement(e);
    if (item)
    {
      if (item->GetType()==XFILEXMLELEMENTTYPE_COMMENT)
      continue;

        XCHAR* idvalue=item->GetValueAttribute(__L("id"));
        if (idvalue)
        {
          XDWORD id = GetValueByDefinition(idvalue);
          if (item->GetName().Compare(__L("transition"))==0)
              state->GetTransitions()->Add(id);
          else
          if (item->GetName().Compare(__L("reject"))==0)
              state->GetRejections()->Add(id);
        }

    }
  }
  return true;
}

/*-------------------------------------------------------------------
//  GRPFILE3DXML::ProcessMilestones
*/
/**
//
//  parse milestones
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      05/12/2016 14:31:18
//
//  @return       bool :
//
//  @param        element :
*/
/*-----------------------------------------------------------------*/
bool GRPFILE3DXML::ProcessMilestones      (XFILEXMLELEMENT* element)
{
  int nLayers=element->GetNElements();

  for (int e=0;e<nLayers;e++)
  {
    XFILEXMLELEMENT* layer=element->GetElement(e);
    if (layer)
    {
      if (layer->GetType()==XFILEXMLELEMENTTYPE_COMMENT)
      continue;

      XCHAR* idvalue=layer->GetValueAttribute(__L("id"));
      if (idvalue)
      {
        XDWORD stack_id = GetValueByDefinition(idvalue);

        for (int i=0;i<layer->GetNElements();i++)
        {
          XFILEXMLELEMENT* milestone=layer->GetElement(i);

          if (milestone->GetType()==XFILEXMLELEMENTTYPE_COMMENT)
            continue;

          MILESTONE* stone=new MILESTONE();
                if (stone)
                {
                  milestone->GetValueAttribute(__L("name"),stone->name);

                  stone->tag    =0;
                  stone->start  =0.0;
                  stone->end    =0.0;
                  stone->passes =0;
                  stone->active =false;


                  stone->tag    =GetValueByDefinition(stone->name.Get());
                  stone->active =(bool) milestone->GetBoolValueAttribute  (__L("active"));
                  stone->start  =(float)milestone->GetFloatValueAttribute (__L("start"));
                  stone->end    =(float)milestone->GetFloatValueAttribute (__L("end"));
                  stone->passes =       milestone->GetIntValueAttribute   (__L("passes"));

                  XSTRING type;
                  milestone->GetValueAttribute(__L("type"),type);

                  //none|hardstop|finish|hardfinish|spring|deccelerate|bounce
                  if (type.Compare(__L("none"))==0)         stone->type=GRPANIMATIONSTACKMILESTONE::NONE;
                  else
                  if (type.Compare(__L("hardstop"))==0)     stone->type=GRPANIMATIONSTACKMILESTONE::HARD_STOP;
                  else
                  if (type.Compare(__L("finish"))==0)       stone->type=GRPANIMATIONSTACKMILESTONE::FINISH;
                  else
                  if (type.Compare(__L("hardfinish"))==0)   stone->type=GRPANIMATIONSTACKMILESTONE::HARD_FINISH;
                  else
                  if (type.Compare(__L("spring"))==0)       stone->type=GRPANIMATIONSTACKMILESTONE::SPRING;
                  else
                  if (type.Compare(__L("deccelerate"))==0)  stone->type=GRPANIMATIONSTACKMILESTONE::DECCELERATE;
                  else
                  if (type.Compare(__L("notify"))==0)       stone->type=GRPANIMATIONSTACKMILESTONE::NOTIFY;
                  else
                  if (type.Compare(__L("bounce"))==0)       stone->type=GRPANIMATIONSTACKMILESTONE::BOUNCE;
                  else
                                                            stone->type=GRPANIMATIONSTACKMILESTONE::NONE;

                  LAYER*  layer=this->layers.Get(stack_id);
                  if (layer)
                  {
                          if (!layer->milestones.Add(stone))
                              delete(stone);
                  }
                  else
                    delete(stone);
                }
        }
      }
    }
  }
  return true;
}

/*-------------------------------------------------------------------
//  GRPFILE3DXML::ProcessOverlays
*/
/**
//
//  process overlays
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      05/12/2016 14:31:27
//
//  @return       bool :
//
//  @param        element :
*/
/*-----------------------------------------------------------------*/
bool GRPFILE3DXML::ProcessOverlays(XFILEXMLELEMENT* element)
{
  for (int e=0;e<element->GetNElements();e++)
  {
    if (element->GetElement(e)->GetType()==XFILEXMLELEMENTTYPE_COMMENT)
        continue;

    GRPFILE3DXML::OVERLAY* overlay=new GRPFILE3DXML::OVERLAY();
    if (!overlay)
      return false;

    XFILEXMLELEMENT* item=element->GetElement(e);

    if (!item)
      continue;

    XCHAR* idvalue=item->GetValueAttribute(__L("id"));
    if (idvalue)
    {
        XDWORD id         = GetValueByDefinition(idvalue);
        overlay->id=id;
        overlay->name=idvalue;
    }

    //inactive|playing|paused|started|stopped|finished
    XCHAR* status=item->GetValueAttribute(__L("status"));
    if (status)
    {
      XSTRING s;
        s=status;
      if (s.Compare(__L("inactive"))==0)
        overlay->status=GRPANIMATION_STATUS_INACTIVE;
      else
      if (s.Compare(__L("playing"))==0)
        overlay->status=GRPANIMATION_STATUS_PLAYING;
      else
      if (s.Compare(__L("started"))==0)
        overlay->status=GRPANIMATION_STATUS_STARTED;
      else
      if (s.Compare(__L("stopped"))==0)
        overlay->status=GRPANIMATION_STATUS_STOPPED;
      else
      if (s.Compare(__L("finished"))==0)
        overlay->status=GRPANIMATION_STATUS_FINISHED;
      else
      if (s.Compare(__L("paused"))==0)
        overlay->status=GRPANIMATION_STATUS_PAUSED;
      else
        overlay->status=GRPANIMATION_STATUS_INACTIVE;
    }

    for (int i=0;i<item->GetNElements();i++)
    {
      XFILEXMLELEMENT* req=item->GetElement(i);

      if (req->GetType()==XFILEXMLELEMENTTYPE_COMMENT)
        continue;

      if (req->GetName().Compare(__L("requires"))==0)
      {
      XCHAR* idvalue=req->GetValueAttribute(__L("id"));
      if (idvalue)
          overlay->requires.Add(this->GetValueByDefinition(idvalue));
      }
      if (req->GetName().Compare(__L("attach"))==0)
      {
      XCHAR* idvalue=req->GetValueAttribute(__L("id"));
        if (idvalue)
        {
            XCHAR* statusvalue=req->GetValueAttribute(__L("status"));
            XSTRING s=statusvalue;
            XDWORD status=GRPANIMATION_STATUS_INACTIVE;

              if (s.Compare(__L("inactive"))==0)
                status=GRPANIMATION_STATUS_INACTIVE;
              else
              if (s.Compare(__L("playing"))==0)
                status=GRPANIMATION_STATUS_PLAYING;
              else
              if (s.Compare(__L("started"))==0)
                status=GRPANIMATION_STATUS_STARTED;
              else
              if (s.Compare(__L("stopped"))==0)
                status=GRPANIMATION_STATUS_STOPPED;
              else
              if (s.Compare(__L("finished"))==0)
                status=GRPANIMATION_STATUS_FINISHED;
              else
              if (s.Compare(__L("paused"))==0)
                status=GRPANIMATION_STATUS_PAUSED;

              overlay->attach.Add(this->GetValueByDefinition(idvalue),status);
        }
      }
    }

    if (!this->overlays.Add(overlay->id,overlay))
      delete(overlay);
  }
  return true;
}


/*-------------------------------------------------------------------
//  GRPFILE3DXML::ProcessTexts
*/
/**
//
//  Processes text locators on texts section
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      07/12/2016 17:20:40
//
//  @return       bool :
//
//  @param        element :
*/
/*-----------------------------------------------------------------*/
bool  GRPFILE3DXML::ProcessTexts(XFILEXMLELEMENT* element)
{
  for (int e=0;e<element->GetNElements();e++)
  {
    if (element->GetElement(e)->GetType()==XFILEXMLELEMENTTYPE_COMMENT)
        continue;

    XFILEXMLELEMENT* item=element->GetElement(e);

    if (!item)
      continue;

    if (item->GetName().Compare(__L("text"))!=0)
      continue;

    GRPFILE3DXML::TEXTBOX* textbox=new GRPFILE3DXML::TEXTBOX();
    if (!textbox)
      return false;

    textbox->distribution = GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_GLYPHS;
    textbox->cellwidth    = 0.20f;
    textbox->cellspacing  = 0.1f;
    textbox->cellspacingrelative  = false;
    textbox->cellwidthrelative    = false;
    textbox->cellpaddingrelative  = false;
    textbox->cellheightrelative   = false;

    XCHAR* namevalue=item->GetValueAttribute(__L("name"));
    if (namevalue)
      textbox->name=namevalue;

    XCHAR* value=item->GetValueAttribute(__L("value"));
    if (value)
      textbox->value=value;

    XCHAR* font=item->GetValueAttribute(__L("font"));
    if (font)
      textbox->font=font;

    XCHAR* align=item->GetValueAttribute(__L("align"));
    if (align)
      textbox->align=align;

    XCHAR* mask=item->GetValueAttribute(__L("mask"));
    if (mask)
      textbox->mask=mask;

    XCHAR* material=item->GetValueAttribute(__L("material"));
    if (material)
      textbox->material=material;

    XCHAR* cellwidth=item->GetValueAttribute(__L("cellwidth"));
    if (cellwidth)
    {
      XSTRING s;
        s=cellwidth;
      if (s.FindCharacter(__C('%'))!=NOTFOUND)
      {
      textbox->cellwidthrelative=true;
      s.DeleteCharacter(__C('%'));
      textbox->cellwidth=s.ConvertToFloat()/100.0f;
      }
      else
      {
      textbox->cellwidthrelative=false;
      textbox->cellwidth=s.ConvertToFloat();
      }
    }

    XCHAR* cellheight=item->GetValueAttribute(__L("cellheight"));
    if (cellheight)
    {
      XSTRING s;
      s=cellheight;
      if (s.FindCharacter(__C('%'))!=NOTFOUND)
      {
      textbox->cellheightrelative=true;
      s.DeleteCharacter(__C('%'));
      textbox->cellheight=s.ConvertToFloat()/100.0f;
      }
      else
      {
      textbox->cellheightrelative=false;
      textbox->cellheight=s.ConvertToFloat();
      }
    }

    XCHAR* cellspacing=item->GetValueAttribute(__L("cellspacing"));
    if (cellspacing)
    {
      XSTRING s;
      s=cellspacing;
      if (s.FindCharacter(__C('%'))!=NOTFOUND)
      {
      textbox->cellspacingrelative=true;
      s.DeleteCharacter(__C('%'));
      textbox->cellspacing=s.ConvertToFloat()/100.0f;
      }
      else
      {
      textbox->cellspacingrelative=false;
      textbox->cellspacing=s.ConvertToFloat();
      }
    }

    XCHAR* cellpadding=item->GetValueAttribute(__L("cellpadding"));
    if (cellpadding)
    {
      XSTRING s;
      s=cellpadding;
      if (s.FindCharacter(__C('%'))!=NOTFOUND)
      {
      textbox->cellpaddingrelative=true;
      s.DeleteCharacter(__C('%'));
      textbox->cellpadding=s.ConvertToFloat()/100.0f;
      }
      else
      {
      textbox->cellpaddingrelative=false;
      textbox->cellpadding=s.ConvertToFloat();
      }
    }


    XCHAR* distribution=item->GetValueAttribute(__L("distribution"));
    if (distribution)
    {
      XSTRING s;
      s=distribution;
      textbox->distribution=0;
          /*
          GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_NONE                          = 0     ,
          GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_DIVIDEEVENLY                  = 1<<0  ,
          GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_OBJECTSATTHEEND               = 1<<1  ,
          GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_CENTERED                      = 1<<2  ,
          GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_STARTOFFSET                   = 1<<3  ,
          GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_ENDOFFSET                     = 1<<4  ,
          GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_SPECIFYSPACING                = 1<<5  ,
          GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_FOLLOW                        = 1<<6  ,
          GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_FIXED                         = 1<<7  ,
          GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_MARGINS                       = 1<<8  ,
          */
      if (s.Find(__L("divideevenly"),false,0)!=NOTFOUND)
        textbox->distribution|=GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_DIVIDEEVENLY;

      if (s.Find(__L("objectsatend"),false,0)!=NOTFOUND)
        textbox->distribution|=GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_OBJECTSATTHEEND;

      if (s.Find(__L("centered"),false,0)!=NOTFOUND)
        textbox->distribution|=GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_CENTERED;

      if (s.Find(__L("startoffset"),false,0)!=NOTFOUND)
        textbox->distribution|=GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_STARTOFFSET;

      if (s.Find(__L("endoffset"),false,0)!=NOTFOUND)
        textbox->distribution|=GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_ENDOFFSET;

      if (s.Find(__L("specifyspacing"),false,0)!=NOTFOUND)
        textbox->distribution|=GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_SPECIFYSPACING;

      if (s.Find(__L("follow"),false,0)!=NOTFOUND)
        textbox->distribution|=GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_FOLLOW;

      if (s.Find(__L("fixed"),false,0)!=NOTFOUND)
        textbox->distribution|=GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_FIXED;

      if (s.Find(__L("margins"),false,0)!=NOTFOUND)
        textbox->distribution|=GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_MARGINS;
    }


    if (!this->texts.Add(&textbox->name,textbox))
      delete(textbox);
  }

  return true;
}



/*-------------------------------------------------------------------
//  GRPFILE3DXML::ProcessCommands
*/
/**
//
//  Process commands and layout
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      09/12/2016 10:55:24
//
//  @return       bool :
//
//  @param        element :
*/
/*-----------------------------------------------------------------*/
bool GRPFILE3DXML::ProcessCommands(XFILEXMLELEMENT* element)
{
  for (int e=0;e<element->GetNElements();e++)
  {
    if (element->GetElement(e)->GetType()==XFILEXMLELEMENTTYPE_COMMENT)
        continue;

    XFILEXMLELEMENT* item=element->GetElement(e);
    if (item->GetName().Compare(__L("layout"))==0)
    {
      ProcessDefinitions(item);
      continue;
    }

    if (item->GetName().Compare(__L("command"))==0)
    {
      AI::COMMAND* command=new AI::COMMAND();
      if (command)
      {
        command->type=AI::COMMAND::CHANGE_STATE;
        command->id=0;
        command->value=0;

        XCHAR* commandid=item->GetValueAttribute(__L("id"));
        if (commandid)
        {
          command->id=this->GetValueByDefinition(commandid);
        }

        XCHAR* commandtype=item->GetValueAttribute(__L("type"));
        if (commandtype)
        {
          XSTRING s;
          s=commandtype;
          if (s.Compare(__L("changestate"))==0)
            ProcessCommandChangeState(item,command);
          else
          if (s.Compare(__L("pushstate"))==0)
            ProcessCommandPushState(item,command);
          else
          if (s.Compare(__L("alterproperty"))==0)
            ProcessCommandAlterProperty(item,command);
          else
          if (s.Compare(__L("changeoverlay"))==0)
            ProcessCommandChangeOverlay(item,command);
        }

        if (!this->object->GetLogic()->GetControllers()->Get(0)->GetCommands()->Add(command->id,command))
          delete(command);
      }
      continue;
    }

  }
  return true;
}

/*-------------------------------------------------------------------
//  GRPFILE3DXML::ProcessCommandChangeState
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      01/03/2017 16:05:04
//
//  @return       bool :
//
//  @param        item :
//  @param        command :
*/
/*-----------------------------------------------------------------*/
bool GRPFILE3DXML::ProcessCommandChangeState(XFILEXMLELEMENT* item,AI::COMMAND* command)
{
  command->type=AI::COMMAND::CHANGE_STATE;
  XCHAR* commandname=item->GetValueAttribute(__L("command"));

  if (commandname)
  {
    command->value=this->GetValueByDefinition(commandname);
  }
  return true;
}

/*-------------------------------------------------------------------
//  GRPFILE3DXML::ProcessCommandPushState
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      04/09/2017 16:24:04
//
//  @return       bool :
//
//  @param        item :
//  @param        command :
*/
/*-----------------------------------------------------------------*/
bool GRPFILE3DXML::ProcessCommandPushState(XFILEXMLELEMENT* item,AI::COMMAND* command)
{
  command->type=AI::COMMAND::PUSH_STATE;
  XCHAR* commandname=item->GetValueAttribute(__L("command"));

  if (commandname)
  {
    command->value=this->GetValueByDefinition(commandname);
  }
  return true;
}

/*-------------------------------------------------------------------
//  GRPFILE3DXML::ProcessCommandChangeOverlay
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      01/03/2017 16:05:11
//
//  @return       bool :
//
//  @param        item :
//  @param        command :
*/
/*-----------------------------------------------------------------*/
bool GRPFILE3DXML::ProcessCommandChangeOverlay(XFILEXMLELEMENT* item,AI::COMMAND* command)
{
  command->type=AI::COMMAND::CHANGE_OVERLAY;

  for (int i=0;i<item->GetNElements();i++)
  {
    XFILEXMLELEMENT* prop=item->GetElement(i);
    if (prop)
    {
      if (prop->GetType()==XFILEXMLELEMENTTYPE_COMMENT)
        continue;

        XSTRING name;
        name=prop->GetValueAttribute(__L("name"));
        XSTRING svalue;
        svalue=prop->GetValueAttribute(__L("value"));

        XDWORD id   =GetValueByDefinition(name.Get());
        XDWORD value=0;

        if (svalue.Compare(__L("started"))==0)
          value=GRPANIMATION_STATUS_STARTED;
        else
        if (svalue.Compare(__L("finished"))==0)
          value=GRPANIMATION_STATUS_FINISHED;
        else
        if (svalue.Compare(__L("stopped"))==0)
          value=GRPANIMATION_STATUS_STOPPED;
        else
        if (svalue.Compare(__L("inactive"))==0)
          value=GRPANIMATION_STATUS_INACTIVE;
        else
        if (svalue.Compare(__L("playing"))==0)
          value=GRPANIMATION_STATUS_PLAYING;
        else
          value=svalue.ConvertToDWord();

      command->properties.Add(id,value);
    }
  }
  return true;
}



/*-------------------------------------------------------------------
//  GRPFILE3DXML::ProcessCommandAlterProperty
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      01/03/2017 16:05:20
//
//  @return       bool :
//
//  @param        item :
//  @param        command :
*/
/*-----------------------------------------------------------------*/
bool GRPFILE3DXML::ProcessCommandAlterProperty(XFILEXMLELEMENT* item,AI::COMMAND* command)
{
  command->type=AI::COMMAND::ALTER_PROPERTY;

  for (int i=0;i<item->GetNElements();i++)
  {
    XFILEXMLELEMENT* prop=item->GetElement(i);
    if (prop)
    {
      if (prop->GetType()==XFILEXMLELEMENTTYPE_COMMENT)
        continue;

      XSTRING name;
      if (prop->GetValueAttribute(__L("name"),name))
      {
        XSTRING* value=new XSTRING();
        if (value)
        if (prop->GetValueAttribute(__L("value")))
        {
          XDWORD id   =GetValueByDefinition(name.Get());
          value->Set(prop->GetValueAttribute(__L("value")));

              XVECTOR<XSTRING*> split;
              value->Explode(__C(':'),&split);
              delete(value);

              XVARIANT* variable=new XVARIANT();

              if (split.GetSize()==2)
              {
                if (split.Get(0)->Compare(__L("bool"))==0)
                  *variable=(bool)split.Get(1)->ConvertToBoolean();
                else
                if (split.Get(0)->Compare(__L("int"))==0)
                  *variable=(int)split.Get(1)->ConvertToInt();
                else
                if (split.Get(0)->Compare(__L("float"))==0)
                  *variable=(float)split.Get(1)->ConvertToDouble();
                else
                if (split.Get(0)->Compare(__L("string"))==0)
                  *variable=split.Get(1)->Get();
                else
                if (split.Get(0)->Compare(__L("color"))==0)
                {
                  XDWORD colorID=this->GetValueByDefinition(split.Get(1)->Get());
                  if (colorID!=0)
                    *variable=((XSTRING*)this->colors.Get(colorID)->GetString())->Get();
                  else
                    *variable=split.Get(1)->Get();
                }
                else
                {
                  delete(variable);
                  continue;
                }
                command->params.Add(id,variable);
              }
              else
                {
                  delete(variable);
                }
              split.DeleteContents();
              split.DeleteAll();
        }
        else
          {
            delete(value);
          }
      }
    }
  }

return true;
}


/*-------------------------------------------------------------------
//   GRPFILE3DXML::ProcessGeometry
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  24/10/2017 12:59:00
//  @return   bool :
//
//  @param    XFILEXMLELEMENT* :
//
*//*-----------------------------------------------------------------*/
bool GRPFILE3DXML::ProcessGeometry(XFILEXMLELEMENT* element)
{
    return true;
}

/*-------------------------------------------------------------------
//   GRPFILE3DXML::ProcessMaterials
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  24/10/2017 12:58:04
//  @return   bool :
//
//  @param    XFILEXMLELEMENT* :
//
*//*-----------------------------------------------------------------*/
bool  GRPFILE3DXML::ProcessMaterials(XFILEXMLELEMENT* item)
{
    for (int i = 0; i < item->GetNElements(); i++)
    {
        XFILEXMLELEMENT* materialblock = item->GetElement(i);

        if (!materialblock)                                             continue;
        if (materialblock->GetType() == XFILEXMLELEMENTTYPE_COMMENT)    continue;

        if (materialblock->GetName().Compare(__L("material")) == 0)
        {
            MATERIAL*   material = new MATERIAL();
                   if (!material) continue;
                        material->name = materialblock->GetValueAttribute(__L("name"));

                        for (int s = 0; s < materialblock->GetNElements(); s++)
                        {
                            MATERIAL::SLOT* slot = new MATERIAL::SLOT();
                            if (!slot) continue;

                            XFILEXMLELEMENT* slotblock = materialblock->GetElement(s);
                            if (slotblock)
                            {
                                slot->id = GRPTEXTURELAYERTYPE_DIFFUSE;

                                XCHAR* slotstring = slotblock->GetValueAttribute(__L("id"));
                                if (slotstring)
                                {
                                    XSTRING type = slotstring;
                                    if (type.Compare(__L("diffuse")) == 0)
                                        slot->id = GRPTEXTURELAYERTYPE_DIFFUSE;
                                    else
                                        if (type.Compare(__L("bumpmap")) == 0)
                                            slot->id = GRPTEXTURELAYERTYPE_BUMPMAP;
                                        else
                                            if (type.Compare(__L("depth")) == 0)
                                                slot->id = GRPTEXTURELAYERTYPE_DEPTH;
                                            else
                                                if (type.Compare(__L("emisive")) == 0)
                                                    slot->id = GRPTEXTURELAYERTYPE_EMISIVE;
                                                else
                                                    if (type.Compare(__L("environment")) == 0)
                                                        slot->id = GRPTEXTURELAYERTYPE_ENVMAP;
                                                    else
                                                        if (type.Compare(__L("normal")) == 0)
                                                            slot->id = GRPTEXTURELAYERTYPE_NORMALMAP;
                                                        else
                                                            if (type.Compare(__L("opacity")) == 0)
                                                                slot->id = GRPTEXTURELAYERTYPE_OPACITY;
                                                            else
                                                                if (type.Compare(__L("reflexive")) == 0)
                                                                    slot->id = GRPTEXTURELAYERTYPE_REFLEXION;
                                                                else
                                                                    if (type.Compare(__L("specular")) == 0)
                                                                        slot->id = GRPTEXTURELAYERTYPE_SPECULAR;
                                }
                                XCHAR* file = slotblock->GetValueAttribute(__L("file"));
                                if (file)
                                {
                                    XSTRING* stringvalue = new XSTRING(); stringvalue->Set(file);
                                    slot->properties.Add(MATERIAL::SLOT::FRONT, stringvalue);
                                }

                                //------------------- process slot
                                for (int e = 0; e < slotblock->GetNElements(); e++)
                                {
                                    XFILEXMLELEMENT* faceblock = slotblock->GetElement(e);
                                    if (faceblock->GetType() == XFILEXMLELEMENTTYPE_COMMENT)
                                        continue;

                                    if (faceblock->GetName().Compare(__L("face")) == 0)
                                    {
                                        XCHAR* facestring = faceblock->GetValueAttribute(__L("id"));
                                        if (!facestring) continue;

                                        XDWORD faceid = MATERIAL::SLOT::FRONT;

                                        XSTRING face = facestring;
                                        if (face.Compare(__L("front"))  == 0) faceid = MATERIAL::SLOT::FRONT;   else
                                        if (face.Compare(__L("back"))   == 0) faceid = MATERIAL::SLOT::BACK;    else
                                        if (face.Compare(__L("top"))    == 0) faceid = MATERIAL::SLOT::TOP;     else
                                        if (face.Compare(__L("bottom")) == 0) faceid = MATERIAL::SLOT::BOTTOM;  else
                                        if (face.Compare(__L("right"))  == 0) faceid = MATERIAL::SLOT::RIGHT;   else
                                        if (face.Compare(__L("left"))   == 0) faceid = MATERIAL::SLOT::LEFT;

                                        XCHAR*  file = faceblock->GetValueAttribute(__L("file"));
                                        if (file)
                                        {
                                            XSTRING*    stringvalue = new XSTRING();
                                            stringvalue->Set(file);

                                            slot->properties.Add(faceid, stringvalue);
                                        }
                                    }
                                }
                                //------------- add slot to material
                                material->slots.Add(slot->id, slot);

                            }
                        }

          this->materials.Add(&material->name,material);
        }
    }

    return true;
}

/*-------------------------------------------------------------------
//   GRPFILE3DXML::ProcessPasses
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  24/10/2017 12:58:11
//  @return   bool :
//
//  @param    XFILEXMLELEMENT* :
//
*//*-----------------------------------------------------------------*/
bool  GRPFILE3DXML::ProcessPasses(XFILEXMLELEMENT* element)
{
    for (int i = 0; i < element->GetNElements(); i++)
    {
        XFILEXMLELEMENT* passblock = element->GetElement(i);
        if (passblock)
        {
            if (passblock->GetType() == XFILEXMLELEMENTTYPE_COMMENT)
                continue;

            XCHAR* name = passblock->GetValueAttribute(__L("name"));
            if (name)
            {
                XCHAR* type = passblock->GetValueAttribute(__L("type"));
                if (type)
                {
                    PASS*           pass            = new PASS();
                                    pass->name      = name;
                                    pass->type      = type;
                                    pass->priority  = 100;
                    XCHAR*          priority        = passblock->GetValueAttribute      (__L("priority"));
                    if (priority)   pass->priority  = passblock->GetIntValueAttribute   (__L("priority"));
                    this->passes.Add(&pass->name,pass);

                    pass->clear = GRPPASS_CLEAR_FLAG_NONE;
                    pass->clone = false;

                    for (int p = 0; p < passblock->GetNElements(); p++)
                    {
                        XFILEXMLELEMENT* option = passblock->GetElement(p);
                        if (!option) continue;
                        if (option->GetType() == XFILEXMLELEMENTTYPE_COMMENT) continue;


                        //--- camera
                        if (option->GetName().Compare(__L("camera")) == 0)
                            pass->camera = option->GetValueAttribute(__L("name"));


                        //---clear
                        if (option->GetName().Compare(__L("clear")) == 0)
                        {
                            XSTRING flags= option->GetValueAttribute(__L("flags"));
                            if (flags.Find(__L("depth"), true) != -1)
                                pass->clear |= GRPPASS_CLEAR_FLAG_DEPTH;

                            if (flags.Find(__L("stencil"), true) != -1)
                                pass->clear |= GRPPASS_CLEAR_FLAG_STENCIL;

                            if (flags.Find(__L("color"), true) != -1)
                                pass->clear |= GRPPASS_CLEAR_FLAG_COLOR;
                        }
                        //---clone
                        if (option->GetName().Compare(__L("clone")) == 0)
                        {
                            pass->clone = option->GetBoolValueAttribute(__L("value"));
                            for (int c = 0; c < option->GetNElements(); c++)
                            {
                                XFILEXMLELEMENT* clone_option = option->GetElement(c);
                                if (!clone_option) continue;

                                if (clone_option->GetName().Compare(__L("accept")) == 0)
                                    if (clone_option->GetValueAttribute(__L("name")))
                                    {
                                        XSTRING* element_name = new XSTRING();
                                        element_name->Set(clone_option->GetValueAttribute(__L("name")));
                                        pass->accept.Add(element_name);
                                    }

                                if (clone_option->GetName().Compare(__L("reject")) == 0)
                                    if (clone_option->GetValueAttribute(__L("name")))
                                    {
                                        XSTRING* element_name = new XSTRING();
                                        element_name->Set(clone_option->GetValueAttribute(__L("name")));
                                        pass->reject.Add(element_name);
                                    }
                            }
                        }
                        //--- rect
                        if (option->GetName().Compare(__L("rect")) == 0)
                        {
                            pass->rect.x1 = pass->rect.x2 = pass->rect.y1 = pass->rect.y2 = 0.0f;

                            for (int c = 0; c < option->GetNElements(); c++)
                            {
                                XFILEXMLELEMENT* rect_option = option->GetElement(c);
                                if (!rect_option) continue;

                                if (rect_option->GetName().Compare(__L("top")) == 0)
                                    pass->rect.y2= (float)rect_option->GetFloatValueAttribute(__L("value"));

                                if (rect_option->GetName().Compare(__L("bottom")) == 0)
                                    pass->rect.y1 = (float)rect_option->GetFloatValueAttribute(__L("value"));

                                if (rect_option->GetName().Compare(__L("left")) == 0)
                                    pass->rect.x1 = (float)rect_option->GetFloatValueAttribute(__L("value"));

                                if (rect_option->GetName().Compare(__L("right")) == 0)
                                    pass->rect.x2 = (float)rect_option->GetFloatValueAttribute(__L("value"));
                            }
                        }

                        //--- material
                        if (option->GetName().Compare(__L("material")) == 0)
                            pass->material = option->GetValueAttribute(__L("name"));

                        //--- viewport
                        if (option->GetName().Compare(__L("viewport")) == 0)
                            pass->viewport = option->GetValueAttribute(__L("name"));

                        //--- light
                        if (option->GetName().Compare(__L("light")) == 0)
                            pass->light = option->GetValueAttribute(__L("name"));

                        //---- options
                        if (option->GetName().Compare(__L("set")) == 0)
                        {
                                XSTRING*  key = new XSTRING();
                                      if (key) key->Set(option->GetValueAttribute(__L("id")));
                                XSTRING*  value = new XSTRING();
                                      if (value) value->Set(option->GetValueAttribute(__L("value")));
                                pass->options.Add(key, value);
                        }
                    }
                }
            }
        }
    }

    return true;
}

/*-------------------------------------------------------------------
//  GRPFILE3DXML::ProcessAudio
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      12/12/2016 10:35:36
//
//  @return       bool :
//
//  @param        element :
*/
/*-----------------------------------------------------------------*/
bool GRPFILE3DXML::ProcessAudio(XFILEXMLELEMENT* element)
{
  for (int e=0;e<element->GetNElements();e++)
  {
    if (element->GetElement(e)->GetType()==XFILEXMLELEMENTTYPE_COMMENT)
        continue;

    if (element->GetElement(e)->GetName().Compare(__L("definitions"))==0)
    {
      if (!ProcessDefinitions(element->GetElement(e)))
        return false;
    }

    if (element->GetElement(e)->GetName().Compare(__L("media"))==0)
    {
      if (!ProcessMedia(element->GetElement(e)))
        return false;
    }

    if (element->GetElement(e)->GetName().Compare(__L("musics")) == 0)
        if (!ProcessMusic(element->GetElement(e)))
            return false;

    if (element->GetElement(e)->GetName().Compare(__L("sounds"))==0)
    {
      XSTRING mastervolume;
      if (element->GetElement(e)->GetValueAttribute(__L("mastervolume"),mastervolume))
      {
        if (mastervolume.FindCharacter(__C('%'))==NOTFOUND)
            SOUND::mastervolume=mastervolume.ConvertToFloat();
        else
        {
            mastervolume.DeleteCharacter(__C('%'));
            SOUND::mastervolume=mastervolume.ConvertToFloat()/100.0f;
        }
      }
      else
        SOUND::mastervolume=1.0f;

      if (!ProcessSound(element->GetElement(e)))
        return false;
    }
  }
  return true;
}


/*-------------------------------------------------------------------
//  GRPFILE3DXML::ProcessMedia
*/
/**
//
//  Processes media files
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      12/12/2016 10:41:47
//
//  @return       bool :
//
//  @param        element :
*/
/*-----------------------------------------------------------------*/
bool GRPFILE3DXML::ProcessMedia(XFILEXMLELEMENT* element)
{
  for (int e=0;e<element->GetNElements();e++)
  {
    XFILEXMLELEMENT* item=element->GetElement(e);

    if (item->GetType()==XFILEXMLELEMENTTYPE_COMMENT)
      continue;

    if (item->GetName().Compare(__L("playlist"))==0)
    {
      XSTRING playlist;
      if (item->GetValueAttribute(__L("name"),playlist))
      {
        ProcessPlaylist(item,playlist);
      }
    }

  };
  return true;
}

/*-------------------------------------------------------------------
//  GRPFILE3DXML::ProcessPlaylist
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      01/03/2017 11:57:48
//
//  @return       bool :
//
//  @param        element :
//  @param        playlist :
*/
/*-----------------------------------------------------------------*/
bool GRPFILE3DXML::ProcessPlaylist(XFILEXMLELEMENT* element,XSTRING& playlist)
{
  for (int e=0;e<element->GetNElements();e++)
  {
    XFILEXMLELEMENT* item=element->GetElement(e);

    if (item->GetType()==XFILEXMLELEMENTTYPE_COMMENT)
      continue;

    //<sample id="number01"       file="01.ogg"/>

    if (item->GetName().Compare(__L("sample"))==0)
    {
      GRPFILE3DXML::MEDIA* media=new GRPFILE3DXML::MEDIA();

      XSTRING sampleName,sampleFile;
      if (item->GetValueAttribute(__L("id"),sampleName))
      {
        media->id=GetValueByDefinition(sampleName.Get());
        media->name=sampleName;
      }

      if (item->GetValueAttribute(__L("file"),sampleFile))
        media->file=sampleFile;

      //--- volume
      XSTRING volumeString;
      if (item->GetValueAttribute(__L("volume"),volumeString))
      {
        if (volumeString.FindCharacter(__C('%'),0,false)==NOTFOUND)
        {
          media->volume=(float)volumeString.ConvertToFloat();
        }
        else
        {
          volumeString.DeleteCharacter(__C('%'));
          media->volume=(float)volumeString.ConvertToFloat()/100.0f;
        }
      }
      else
        media->volume=1.0f;

      media->playlist=playlist;
      if (this->media.Find(media->id)==NOTFOUND)
      {
        if (!this->media.Add(media->id,media))
          delete(media);
      }
      else
        delete(media);

    }
  };

  return true;
}
/*-------------------------------------------------------------------
//  GRPFILE3DXML::ProcessSound
*/
/**
//
//  Processes sounds
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      12/12/2016 10:41:58
//
//  @return       bool :
//
//  @param        element :
*/
/*-----------------------------------------------------------------*/
bool GRPFILE3DXML::ProcessSound(XFILEXMLELEMENT* element)
{
  XDWORD layerID;
  XSTRING layername;

  sounds.SetIsMulti(true);
  for (int e=0;e<element->GetNElements();e++)
  {
    XFILEXMLELEMENT* layer=element->GetElement(e);

    if (layer->GetType()==XFILEXMLELEMENTTYPE_COMMENT)
      continue;

      if (layer->GetValueAttribute(__L("id"),layername))
      {
        layerID=GetValueByDefinition(layername.Get());
      }

      for (int i=0;i<layer->GetNElements();i++)
      {
        XFILEXMLELEMENT* item=layer->GetElement(i);

        if (item->GetType()==XFILEXMLELEMENTTYPE_COMMENT)
          continue;

        if (item->GetName().Compare(__L("sound"))==0)
        {
          GRPFILE3DXML::SOUND* sound=new GRPFILE3DXML::SOUND();
          if (!sound) continue;

          sound->layer=layername;
          sound->layerID=layerID;

          //--- sample id
          sound->type=SNDANIMATIONLISTENER::PLAY;

          XSTRING sampleName,layer,type;
          if (item->GetValueAttribute(__L("sample"),sampleName))
          {
            sound->sampleid=GetValueByDefinition(sampleName.Get());
            sound->name=sampleName;

            if (sound->sampleid==-1)
              continue;

            GRPFILE3DXML::MEDIA* mediafile=media.Get(sound->sampleid);
            if (mediafile)
            {
            sound->samplename   =mediafile->file;
            sound->playlist     =mediafile->playlist;
            sound->samplevolume =mediafile->volume;
            }
          }
          else
          {
            XSTRING playlistName,defaultSample;
            if (item->GetValueAttribute(__L("name"),playlistName))
            {
              sound->samplename.Set (__L(""));
              sound->playlist     =playlistName;
              sound->samplevolume =1.0f;

              if (item->GetValueAttribute(__L("default"),defaultSample))
                sound->sampleid=(int)defaultSample.ConvertToInt();
              else
                sound->sampleid=0;
            }
            else
            {
              sound->samplename.Set (__L(""));
              sound->playlist.Set   (__L(""));
              sound->samplevolume =1.0f;
            }
          }



          //--- type

          if (item->GetValueAttribute(__L("type"),type))
          {
            if (type.Compare(__L("play"))==0)
              sound->type=SNDANIMATIONLISTENER::PLAY;
            else
            if (type.Compare(__L("stop"))==0)
              sound->type=SNDANIMATIONLISTENER::ONLYSTOP;
            else
            if (type.Compare(__L("playlist"))==0)
              sound->type=SNDANIMATIONLISTENER::PLAYLIST;
          }


          //--- frame

          sound->end_isoffset=false;
          sound->start_isoffset = false;

          XSTRING frame;
          if (item->GetValueAttribute(__L("frame"),frame))
          {
            sound->startframe=(float)item->GetFloatValueAttribute(__L("frame"));
            if (frame.FindCharacter(__C('+'),0,false)!=XSTRING_NOTFOUND)
              sound->start_isoffset =true;

            if (frame.FindCharacter(__C('-'), 0, false) != XSTRING_NOTFOUND)
                sound->start_isoffset = true;
          }
          else
            sound->startframe=1.0f;

          //--- end

          sound->endframe=0.0f;
          XSTRING endframe;
          if (item->GetValueAttribute(__L("end"),endframe))
          {
            sound->endframe=(float)item->GetFloatValueAttribute(__L("end"));
            if (frame.FindCharacter(__C('+'),0,false)!=XSTRING_NOTFOUND)
              sound->end_isoffset =true;

            if (frame.FindCharacter(__C('-'), 0, false) != XSTRING_NOTFOUND)
                sound->end_isoffset = true;
          }
          else
            sound->endframe=1.0f;

          //--- volume
          XSTRING volumeString;
          if (item->GetValueAttribute(__L("volume"),volumeString))
          {
            if (volumeString.FindCharacter(__C('%'),0,false)==NOTFOUND)
            {
              sound->volume=(float)item->GetFloatValueAttribute(__L("volume"));
            }
            else
            {
              volumeString.DeleteCharacter(__C('%'));
              sound->volume=(float)volumeString.ConvertToFloat()/100.0f;
            }
          }
          else
            sound->volume=1.0f;

          //--- number of loops

          XSTRING loopString;
          if (item->GetValueAttribute(__L("loop"),loopString))
          {
            sound->loop=(bool)item->GetBoolValueAttribute(__L("loop"));
          }
          else
              sound->loop=false;

          //--- number of passes

          XSTRING passString;
          if (item->GetValueAttribute(__L("passes"),passString))
          {
              sound->passes=(int)item->GetIntValueAttribute(__L("passes"));
          }
          else
              sound->passes=0;

          //--- is remote

          XSTRING remoteString;
          if (item->GetValueAttribute(__L("isremote"),remoteString))
          {
              sound->isremote=(bool)item->GetBoolValueAttribute(__L("isremote"));
          }
          else
              sound->isremote=false;

          XSTRING localString;
          if (item->GetValueAttribute(__L("islocal"),localString))
          {
            sound->islocal=(bool)localString.ConvertToBoolean();
          }
          else
              sound->islocal=true;

        //  if (this->layers.Get(sound->layerID))
        //    this->layers.Get(sound->layerID)->sounds.Add(sound);

          this->sounds.Add(sound);
        }
      }
  };
  return true;
}

/*-------------------------------------------------------------------
//   GRPFILE3DXML::ProcessMusic
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  31/10/2017 13:23:05
//  @return   bool :
//
//  @param    XFILEXMLELEMENT* :
//
*//*-----------------------------------------------------------------*/
bool GRPFILE3DXML::ProcessMusic(XFILEXMLELEMENT* element)
{
    music.SetIsMulti(true);
    for (int e = 0; e<element->GetNElements(); e++)
    {
        XFILEXMLELEMENT* item = element->GetElement(e);

        if (item->GetType() == XFILEXMLELEMENTTYPE_COMMENT)
            continue;

            if (item->GetName().Compare(__L("music")) == 0)
            {
                GRPFILE3DXML::MUSIC* music = new GRPFILE3DXML::MUSIC();
                if (!music) continue;

                XSTRING name;

                //--- sample id

                XSTRING sampleName, layer, type;
                if (item->GetValueAttribute(__L("sample"), sampleName))
                {
                    music->sampleid = GetValueByDefinition(sampleName.Get());
                    music->name = sampleName;

                    if (music->sampleid == -1)
                        continue;

                    GRPFILE3DXML::MEDIA* mediafile = media.Get(music->sampleid);
                    if (mediafile)
                    {
                        music->samplename = mediafile->file;
                        music->volume = mediafile->volume;
                    }
                }
                else
                {
                    XSTRING playlistName, defaultSample;
                    if (item->GetValueAttribute(__L("name"), playlistName))
                    {
                        music->samplename.Set(__L(""));
                        music->playlist = playlistName;
                        music->volume = 1.0f;

                        if (item->GetValueAttribute(__L("default"), defaultSample))
                            music->sampleid = (int)defaultSample.ConvertToInt();
                        else
                            music->sampleid = 0;
                    }
                    else
                    {
                        music->samplename.Set(__L(""));
                        music->playlist.Set(__L(""));
                        music->volume = 1.0f;
                    }
                }

                //--- volume
                XSTRING volumeString;
                if (item->GetValueAttribute(__L("volume"), volumeString))
                {
                    if (volumeString.FindCharacter(__C('%'), 0, false) == NOTFOUND)
                    {
                        music->volume = (float)item->GetFloatValueAttribute(__L("volume"));
                    }
                    else
                    {
                        volumeString.DeleteCharacter(__C('%'));
                        music->volume = (float)volumeString.ConvertToFloat() / 100.0f;
                    }
                }
                else
                    music->volume = 1.0f;

                //--- number of loops

                XSTRING loopString;
                if (item->GetValueAttribute(__L("loop"), loopString))
                {
                    music->loop = (bool)item->GetBoolValueAttribute(__L("loop"));
                }
                else
                    music->loop = false;

                    //--- number of loops

                XSTRING playString;
                if (item->GetValueAttribute(__L("status"), playString))
                {
                    music->status = (bool)item->GetBoolValueAttribute(__L("status"));
                }
                else
                    music->status = false;

                this->music.Add(music->sampleid,music);

        }
    };
    return true;
}

/*-------------------------------------------------------------------
//   GRPFILE3DXML::ProcessElements
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  11/12/2017 12:03:34
//  @return   bool :
//
//  @param    XFILEXMLELEMENT* :
//
*//*-----------------------------------------------------------------*/
bool  GRPFILE3DXML::ProcessElements(XFILEXMLELEMENT* element)
{
    elements.SetIsMulti(true);
    for (int e = 0; e < element->GetNElements(); e++)
    {
        XFILEXMLELEMENT* item = element->GetElement(e);

        if (item->GetType() == XFILEXMLELEMENTTYPE_COMMENT)
            continue;

        if (item->GetName().Compare(__L("element")) == 0)
        {
            GRPFILE3DXML::ELEMENT* element = new GRPFILE3DXML::ELEMENT();
            element->flags = 0;
            if (element)
            {
                if (item->GetValueAttribute(__L("name"), element->name))
                {
                    XSTRING value;
                    if (item->GetValueAttribute(__L("priority"), value))
                    {
                        element->flags |= GRPFILE3DXML::ELEMENT::PRIORITY;
                        element->priority = value.ConvertToDWord();
                    }

                    if (item->GetValueAttribute(__L("visibility"), value))
                    {
                        element->flags |= GRPFILE3DXML::ELEMENT::VISIBILITY;
                        element->priority = value.ConvertToBoolean();
                    }

                    if (item->GetValueAttribute(__L("shader"), value))
                    {
                        element->flags |= GRPFILE3DXML::ELEMENT::SHADER;
                        element->shader = value;
                    }

                    if (item->GetValueAttribute(__L("material"), value))
                    {
                        element->flags |= GRPFILE3DXML::ELEMENT::MATERIAL;
                        element->material = value;
                    }
                }
            }
            this->elements.Add(&element->name, element);
        }
        else
            continue;
    }

    return true;
}

/*-------------------------------------------------------------------
//  GRPFILE3DXML::ApplyConfig
*/
/**
//
//  Applies config
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      07/12/2016 11:44:14
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool GRPFILE3DXML::ApplyConfig()
{
  if (this->object)
  for (XDWORD e=0;e<this->lights.GetSize();e++)
  {
    GRPFILE3DXML::LIGHT* lightconfig=this->lights.GetElement(e);
    XSTRING lightname;
    lightname.Format(__L("Model::%s"),lightconfig->name.Get());

    GRPLIGHT* light=this->object->FindLight(lightname.Get());
    if (light)
    {
      if (lightconfig->flags & GRPFILE3DXML::LIGHT::MULTIPLIER)
        light->SetMultiplier(light->GetMultiplier()*lightconfig->multiplier);

      if (lightconfig->flags & GRPFILE3DXML::LIGHT::CONSTANT_ATTENUATION)
        light->SetConstantAttenuation(light->GetConstantAttenuation()+lightconfig->constantAttenuattion);

      if (lightconfig->flags & GRPFILE3DXML::LIGHT::LINEAR_ATTENUATION)
        light->SetLinearAttenuation(light->GetLinearAttenuation()+lightconfig->linearAttenuattion);

      if (lightconfig->flags & GRPFILE3DXML::LIGHT::QUADRIC_ATTENUATION)
        light->SetQuadraticAttenuation(light->GetQuadraticAttenuation()+lightconfig->quadricAttenuattion);

      if (lightconfig->flags & GRPFILE3DXML::LIGHT::CUBIC_ATTENUATION)
        light->SetCubicAttenuation(light->GetCubicAttenuation()+lightconfig->cubicAttenuattion);
    }
  }
  return true;
}


/*-------------------------------------------------------------------
//  GRPFILE3DXML::GetValueByDefinition
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      29/11/2016 10:00:13
//
//  @return       XDWORDSIG :
//
//  @param        name :
*/
/*-----------------------------------------------------------------*/
XDWORDSIG  GRPFILE3DXML::GetValueByDefinition(XCHAR* name)
{
  XDWORD n=this->definitions.GetSize();
  for (XDWORD e=0;e<n;e++)
  {
    if (this->definitions.GetKey(e)->Compare(name)==0)
      return this->definitions.GetElement(e);
  }

  Output(4,__L("Definition name not found: %s"),name);

  return 0;
}

/*-------------------------------------------------------------------
//   GRPFILE3DXML::GetFlags
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  19/10/2017 9:55:46
//  @return   XDWORD :
//
//  @param     :
//
*//*-----------------------------------------------------------------*/
XDWORD  GRPFILE3DXML::GetFlags()
{
    return this->flags;
}

/*-------------------------------------------------------------------
//   GRPFILE3DXML::SetFlags
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  19/10/2017 9:55:54
//  @return   void :
//
//  @param    XDWORD :
//
*//*-----------------------------------------------------------------*/
void  GRPFILE3DXML::SetFlags(XDWORD flag)
{
    this->flags = flag;
}

/*-------------------------------------------------------------------
//   GRPFILE3DXML::AddFlag
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  19/10/2017 9:56:02
//  @return   void :
//
//  @param    XML_FLAGS :
//
*//*-----------------------------------------------------------------*/
void  GRPFILE3DXML::AddFlag(XML_FLAGS flag)
{
    this->flags = flags | flag;
}

/*-------------------------------------------------------------------
//   GRPFILE3DXML::RemoveFlag
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  19/10/2017 9:56:44
//  @return   void :
//
//  @param    XML_FLAGS :
//
*//*-----------------------------------------------------------------*/
void  GRPFILE3DXML::RemoveFlag(XML_FLAGS flag)
{
    this->flags = flags & ~flag;
}

/*-------------------------------------------------------------------
//  GRPFILE3DXML::LoadModel
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      29/11/2016 10:31:45
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool  GRPFILE3DXML::LoadModel()
{

  this->percent = 0.0f;
  this->Publish(GRPMSG_STEP);

  if (!LoadMaterials()) { return false; }
  if (!LoadLayers   ()) { return false; }
  if (!LoadSounds   ()) { return false; }
  if (!LoadOverlays ()) { return false; }

  this->percent = 100.0f;
  this->Publish(GRPMSG_STEP);

  if (isaborted)
      return true;

  //---------------- aditional texts

  for (XDWORD e=0;e<this->texts.GetSize();e++)
  {
      if (isaborted)
          return true;

    GRPFILE3DXML::TEXTBOX* text=texts.GetElement(e);

    GRPNODE* node=this->object->FindNode(text->name.Get());
    if (node)
    if (!object->FindTextBox(text->name.Get()))
    {
      node->GetUserData()->Add(__L("Font"),       text->font.Get());
      node->GetUserData()->Add(__L("UserMask"),   text->mask.Get());
      node->GetUserData()->Add(__L("Align"),      text->align.Get());
      node->GetUserData()->Add(__L("UserValue"),  text->value.Get());
      node->GetUserData()->Add(__L("Material"),   text->material.Get());
      node->GetUserData()->Add(__L("Color"),      text->color.Get());

      text->assigned=true;
      GRPTEXTELEMENT* element=object->CreateTextFromLocator(node);
      if(element)
      {
        element->SetProgram(element->ChooseShader());

        if (!text->cellspacingrelative)
          element->GetAlignTool()->cellspacing  =text->cellspacing;
        else
          element->GetAlignTool()->cellspacing  =element->GetFont()->GetMonospaceWidth()*text->cellspacing;

        if (!text->cellwidthrelative)
          element->GetAlignTool()->cellsize     =text->cellwidth;
        else
          element->GetAlignTool()->cellspacing  =element->GetFont()->GetMonospaceWidth()*text->cellwidth;

        element->GetAlignTool()->distribution =text->distribution;
      }
    }
    else
      Output(4,__L("Error when creating additional textbox : %s , locator not found on object"),text->name.Get());
  }

  if (isaborted)
      return true;

  ApplyConfig();

  /*
  for (XDWORD e=0;e<object->GetTexts()->GetSize();e++)
  {
      if (isaborted)
          return true;

    if (object->GetTexts()->Get(e))
        object->GetTexts()->Get(e)->UpdateContent();
  }
  */

//----------------------- layer start
  for (XDWORD e=0;e<this->layers.GetSize();e++)
  {
      if (isaborted)
          return true;

    GRPFILE3DXML::LAYER* layer=layers.GetElement(e);

        GRPANIMATIONSTACK* stack=object->GetAnimationControl()->GetStack(layer->layerID);
        if (stack)
        {
          stack->SetActive        (layer->isActive);
          stack->SetInitialActive (layer->isActive);
        }
  }

  LoadLanguages();

  if (object->GetLogic())
  {
    //  object->GetLogic()->GetControllers()->GetLast()->SubscribeToEvents();
      object->GetLogic()->GetControllers()->GetLast()->ChangeToDefaultState();
  }

  object->GetAnimationControl()->Play();


  if (!LoadPasses())
      return false;

  return true;
}


/*-------------------------------------------------------------------
//  GRPFILE3DXML::LoadLayers
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      19/05/2017 17:10:46
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool GRPFILE3DXML::LoadLayers()
{
  if (isaborted)
        return true;

  this->percent = 0.0f;

  GRPFILE3D*    grpfile3d = new GRPFILE3D();
            if(!grpfile3d) return NULL;


  grpfile3d->SetObject        (object);
  grpfile3d->SetVerboseLevel  (VerboseLevel);
  grpfile3d->SetLoadFlags     (loadFlags);
  this->Subscribe(grpfile3d);

  XSTRING output_file;

    for (XDWORD e=0;e<layerOptions.GetSize();e++)
    {
      if (layerOptions.GetKey(e)->Compare(__L("output"))==0)
      {
        output_file=layerOptions.GetElement(e)->Get();
      }
    }

    this->currentask = 0;
    for (XDWORD e = 0; e < this->layers.GetSize(); e++)
        this->tasks.Add(0.0f); //init tasks

#ifdef SND_ACTIVE
    if (SNDFACTORY::SoundIsActive)
        for (XDWORD e = 0; e < this->media.GetSize(); e++)
            this->tasks.Add(0.0f); //add one for sound
#endif

            //---------------------------- load layers
            for (XDWORD e=0;e<this->layers.GetSize() && !isaborted;e++)
            {
              GRPFILE3DXML::LAYER* layer=layers.GetElement(e);

              grpfile3d->SetLoadFlags     (layer->loadFlags);
              grpfile3d->SetNextStackID   (layer->layerID);

              XPATH xpath;
                    xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, layer->file.Get());

              Output(0,__L("Loading layer[%d]: %s"),e,layer->file.Get());

              if (!grpfile3d->Load(xpath.Get()))
              {
                  Output(4,__L("    Error loading layer[%d]: %s"),e,layer->file.Get());
                  delete(grpfile3d);
                  grpfile3d = NULL;
                  return false;
              }
              else
              {
                layer->assigned=true;
              }

              Publish(GRPMSG_STEP);
              this->currentask++;
            }


  for (XDWORD e=0;e<this->layers.GetSize() && !isaborted;e++)
  {
    GRPFILE3DXML::LAYER* layer=layers.GetElement(e);

    //-------------------------- access last added stack, if any
    GRPANIMATIONSTACK* stack=NULL;
    if (object)
        stack=object->GetAnimationControl()->GetStacks()->Get(layer->layerID);

    if (stack)
    {
      stack->SetFrameSpeed  ((float)layer->speed);
      stack->SetLoopMode    (layer->loop);

      switch (layer->trim)
      {
        case 0: break;
        case 1: stack->Trim(layer->trimstart,                               stack->GetFrameOfTime(stack->GetMaxFrame())); break;
        case 2: stack->Trim(stack->GetFrameOfTime(stack->GetMinFrame()),    layer->trimend);        break;
        case 3: stack->Trim(layer->trimstart,                               layer->trimend);        break;
        default: break;
      }

      //----------------------------- set Milestones
      for (XDWORD i=0;i<layer->milestones.GetSize();i++)
      {
        MILESTONE* ms=layer->milestones.Get(i);

        GRPANIMATIONSTACKMILESTONE* stone=new GRPANIMATIONSTACKMILESTONE();
                                if (stone)
                                    {
                                      stone->SetTag         (ms->tag);
                                      stone->SetName        (ms->name.Get());
                                      stone->SetType        (ms->type);
                                      stone->Enable         (ms->active);
                                      stone->SetPasses      (ms->passes);
                                      stone->SetStartFrame  (ms->start);
                                      stone->SetEndFrame    (ms->end);
                                      stone->SetTension     (0.0f);

                                      ms->assigned=true;
                                      stack->AddMilestone   (stone);
                                    }
      }

    }
  }

  if (layers.GetSize() > 1 && !isaborted)
      object->MergeAnimations();


  for (XDWORD e = 0; e < this->layerOptions.GetSize() && !isaborted; e++)
  {
    if (this->layerOptions.GetKey(e)->Compare(__L("order")) == 0) //has order expec
    {
        if (this->layerOptions.GetElement(e)->Compare(__L("NORMAL")) == 0)
            object->GetAnimationControl()->SetOrder(GRPANIMATIONCONTROL::DIRECT);
        else
        if (this->layerOptions.GetElement(e)->Compare(__L("DIRECT")) == 0)
            object->GetAnimationControl()->SetOrder(GRPANIMATIONCONTROL::DIRECT);
        else
        if (this->layerOptions.GetElement(e)->Compare(__L("REVERSED")) == 0)
            object->GetAnimationControl()->SetOrder(GRPANIMATIONCONTROL::REVERSED);
        else
        if (this->layerOptions.GetElement(e)->Compare(__L("REVERSE")) == 0)
            object->GetAnimationControl()->SetOrder(GRPANIMATIONCONTROL::REVERSED);
    }
  }

  //----

  for (XDWORD e = 0; e < this->elements.GetSize() && !isaborted; e++)
  {
      GRPFILE3DXML::ELEMENT* data = this->elements.GetElement(e);
      GRPELEMENT* element = object->FindElementByNodeName(data->name.Get());
      if (element)
      {
          if (data->flags & GRPFILE3DXML::ELEMENT::PRIORITY)
              element->SetRenderPriority(data->priority);

          if (data->flags & GRPFILE3DXML::ELEMENT::VISIBILITY)
              element->SetVisibility(data->visible ? 1.0f : 0.0f);

          if (data->flags & GRPFILE3DXML::ELEMENT::SHADER)
              element->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByName(data->shader.Get()));

          if (data->flags & GRPFILE3DXML::ELEMENT::MATERIAL)
              element->GetNode()->SetMaterial(GRPMATERIALLIBRARY::Get()->Select(data->material.Get()));
      }
  }

  delete(grpfile3d);
  grpfile3d=NULL;

  return true;
}


/*-------------------------------------------------------------------
//  GRPFILE3DXML::LoadSounds
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      09/06/2017 17:46:02
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool GRPFILE3DXML::LoadSounds()
{
    if (isaborted)
        return true;

#ifdef SND_ACTIVE

  if (!SNDFACTORY::SoundIsActive) return true;

  //---------------------------- set sound manager
  XDEBUG_PRINTCOLOR(0,__L("Loading sound manager sounds"));

  SNDMANAGER* sndmanager=SNDMANAGER::Get();
  XPATH soundpath;
  XPATHSMANAGER::GetInstance().GetPathOfSection(XPATHSMANAGERSECTIONTYPE_SOUNDS, soundpath);
  for (XDWORD e=0;e<this->media.GetSize();e++)
  {
    if (sndmanager)
    {
      MEDIA*    smp=this->media.GetElement(e);
      if (smp)
          sndmanager->Add(soundpath,smp->file,smp->playlist);
      this->tasks.Set(currentask, 100.0f);
      this->currentask++;
      this->Publish(GRPMSG_STEP);
    }
  }

//--------------------------- set sounds

  for (XDWORD i=0;i<sounds.GetSize();i++)
      {
        if (isaborted)
          return true;

        SOUND* sound=sounds.Get(i);
        sound->assigned=true;

        if (!this->object->GetAnimationControl())
          break;

        if (this->object->GetAnimationControl()->GetStacks()->Find(sound->layerID)==NOTFOUND)
          continue;
        GRPANIMATIONSTACK* stack=this->object->GetAnimationControl()->GetStack(sound->layerID);
        if (stack==NULL)
          continue;

        if (sound->sampleid<=0 && sound->type!=SNDANIMATIONLISTENER::PLAYLIST)
          continue;

        SNDANIMATIONLISTENER* snd=new SNDANIMATIONLISTENER(); if (!snd) continue;

                              snd->SetName        (sound->name.Get());
                              snd->SetSampleName  (sound->samplename.Get());

                              if (sound->start_isoffset)
                              {
                                  if (sound->startframe > 0)
                                      snd->SetStartFrame(stack->GetFrameOfTime(stack->GetMinFrame()) + sound->startframe);
                                  else
                                      snd->SetStartFrame(stack->GetFrameOfTime(stack->GetMaxFrame()) + sound->startframe);
                              }
                              else
                                snd->SetStartFrame(sound->startframe);

                              if (sound->end_isoffset)
                              {
                                  if (sound->endframe > 0)
                                    snd->SetEndFrame(stack->GetFrameOfTime(stack->GetMinFrame()) + sound->endframe);
                                  else
                                    snd->SetEndFrame(stack->GetFrameOfTime(stack->GetMaxFrame()) + sound->endframe);
                              }
                              else
                                snd->SetEndFrame  (sound->endframe);


                              snd->SetAudioType     (sound->type);

                              if (sound->type==SNDANIMATIONLISTENER::PLAYLIST)
                              {
                                sound->samplevolume=1.0f;

                                if (sound->playlist.Get())
                                for (XDWORD e=0;e<this->media.GetSize();e++)
                                {
                                  MEDIA* item=this->media.GetElement(e);
                                  if (item)
                                  if (item->playlist.Compare(sound->playlist.Get())==0)
                                  {
                                    snd->AddSampleToPlaylist(item->id,&item->file);
                                  }
                                }
                                snd->SetNextPlaylistItem(sound->sampleid);
                              }
                              snd->SetBaseVolume    (sound->volume*SOUND::mastervolume*sound->samplevolume);
                              snd->SetLoop          (sound->loop);
                              snd->SetPasses        (sound->passes);
                              snd->SetIsLocal       (sound->islocal);

                              snd->SetIsRemote      (sound->isremote);
                              snd->SetPlaylistName  (sound->playlist.Get());

                              snd->SetTag           (stack->GetID());

                              Output(3,__L("[%d / %d]  SND->INIT %s %s"),i, sounds.GetSize(),stack->GetName()->Get(),sound->samplename.Get());

                              if (sound->type!=SNDANIMATIONLISTENER::PLAYLIST)
                              {
                                if (!snd->Init    ())
                                {
                                  Output(4,__L(" GRPFILE3DXML::LoadModel() : %s Unable to load sound %s"),stack->GetName()->Get(),sound->samplename.Get());
                                  delete(snd);
                                  continue;
                                }
                              }

                              stack->AddMilestone(snd);

      }

  for (XDWORD e = 0; e < this->music.GetSize(); e++)
  {
      GRPFILE3DXML::MUSIC* music = this->music.GetElement(e);

      SNDELEMENT* sndelement = SNDFACTORY::Get()->GetFile(music->samplename.Get());
      if (!sndelement)
      {
          sndelement = SNDFACTORY::Get()->AddFile(soundpath,music->samplename.Get());
          if (!sndelement)
          {
              Output(4, __L(" GRPFILE3DXML::LoadModel() : Unable to load music %s"),  music->samplename.Get());
              continue;
          }
      }
      sndelement->SetLoop(music->loop);
      sndelement->SetVolume(music->volume);

      if (music->status)
          if (SNDFACTORY::SoundIsActive)
            SNDFACTORY::Get()->PlaySound(sndelement);
  }
  Output(3, __L("Sound finished!"));
#endif
  return true;
}

/*-------------------------------------------------------------------
//  GRPFILE3DXML::LoadSounds
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      09/06/2017 17:48:36
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool GRPFILE3DXML::LoadOverlays()
{
    if (isaborted)
        return true;

  //---------------- overlays

  for (XDWORD e=0;e<this->overlays.GetSize();e++)
  {
    GRPFILE3DXML::OVERLAY* overlay=overlays.GetElement(e);

    AI::OVERLAY* ov=this->object->GetLogic()->GetControllers()->GetLast()->GetOverlays()->Get(overlay->id);

    if (ov==NULL)
       ov=new AI::OVERLAY();

    if (!ov)
        continue;

        ov->SetID             (overlay->id);
        ov->SetStatus         (overlay->status);
        ov->SetInitialStatus  (overlay->status);
        ov->SetName           (overlay->name.Get());


        for (XDWORD i=0;i<overlay->requires.GetSize();i++)
          ov->GetOverlayables()->Add(overlay->requires.Get(i));

        for (XDWORD i=0;i<overlay->attach.GetSize();i++)
        {
          ov->GetAttachees()->Add(overlay->attach.GetKey(i),overlay->attach.GetElement(i));
        }

        if (this->object->GetLogic())
        {
        if (this->object->GetLogic()->GetControllers()->GetLast()->Add(ov->GetID(),ov))
            overlay->assigned=true;
        }
    }
  return true;
}

/*-------------------------------------------------------------------
//  GRPFILE3DXML::LoadLanguages
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      14/06/2017 13:27:04
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool GRPFILE3DXML::LoadLanguages()
{
//-----------------------languages
  GRPUSERDATACONTAINER* data=NULL;

  if (object->GetNode())
  data=object->GetNode()->GetUserData();

  if (data)
  for (XDWORD e=0;e<languages.GetSize();e++)
  {
    for (XDWORD i=0;i<languages.GetElement(e)->strings.GetSize();i++)
    {
    XSTRING* key  =languages.GetElement(e)->strings.GetKey(i);
    XSTRING* value=languages.GetElement(e)->strings.GetElement(i);

    data->Add(key->Get(),value->Get());
    }
  }

return true;
}

/*-------------------------------------------------------------------
//   GRPFILE3DXML::LoadPasses
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  24/10/2017 14:52:46
//  @return   bool :
//
//  @param     :
//
*//*-----------------------------------------------------------------*/
bool GRPFILE3DXML::LoadPasses()
{
    if (isaborted)
        return true;

    for (XDWORD e = 0; e < this->passes.GetSize(); e++)
    {
        PASS*       _pass   = passes.GetElement(e);
        GRPPASS*    pass = NULL;

        if (_pass->name.Compare(__L("main")) != 0)
        {
            GRPPASS*    pass = GRPPASSMANAGER::Get()->Create(_pass->type.Get(), _pass->priority);
                        pass->SetName(_pass->name.Get());
                        pass->SetClear(_pass->clear);

                        pass->SetCamera(_pass->camera.Get());
                        pass->SetLight(_pass->light.Get());
                        pass->SetViewport(_pass->viewport.Get());
                        pass->SetMaterialName(_pass->material.Get());
                        pass->SetClone(_pass->clone);
                        pass->SetRect(_pass->rect.x1, _pass->rect.y1, _pass->rect.x2, _pass->rect.y2);

            for (XDWORD e = 0; e < _pass->options.GetSize(); e++)
            {
                if (_pass->options.GetElement(e)->FindCharacter(__C('.')) != -1)
                    pass->SetValue(_pass->options.GetKey(e)->ConvertToDWord(), _pass->options.GetElement(e)->ConvertToDouble());
                else
                    pass->SetValue(_pass->options.GetKey(e)->ConvertToDWord(), _pass->options.GetElement(e)->ConvertToDWord());
            }

            for (XDWORD e = 0; e < _pass->accept.GetSize(); e++)
            {
                XSTRING name;
                name.Set(_pass->accept.Get(e)->Get());
                for (XDWORD i = 0; i < object->GetElements()->GetSize(); i++)
                {
                    GRPELEMENT* element = object->GetElements()->Get(i);
                    if (element->GetName()->Compare(name.Get(), false) == 0)
                        pass->Add(element);
                }
            }

            if (_pass->reject.GetSize())
                for (XDWORD i = 0; i < object->GetElements()->GetSize(); i++)
                {
                    GRPELEMENT* element = object->GetElements()->Get(i);
                    bool reject = false;
                    for (XDWORD e = 0; e < _pass->reject.GetSize(); e++)
                    {
                        if (_pass->reject.Get(e)->Compare(element->GetName()->Get()) == 0)
                        {
                            reject = true;;
                            break;
                        }
                    }
                    if (!reject)
                        pass->Add(element);
                }

            object->Add(pass);

            if (_pass->camera.GetSize())
                pass->SetCamera(object->FindCamera(_pass->camera.Get()));

            if (_pass->light.GetSize())
                pass->SetLight(object->FindLight(_pass->light.Get()));

        }
        else
        {
            //----- affect main pass
            pass                    = this->scene->GetPass();
            pass->SetClear          (_pass->clear);
            pass->SetCamera         (_pass->camera.Get());
            pass->SetViewport       (_pass->viewport.Get());
            pass->SetMaterialName   (_pass->material.Get());
            pass->SetRect           (_pass->rect.x1, _pass->rect.y1, _pass->rect.x2, _pass->rect.y2);

            for (XDWORD e = 0; e < _pass->options.GetSize(); e++)
            {
                if (_pass->options.GetElement(e)->FindCharacter(__C('.')) != -1)
                    pass->SetValue(_pass->options.GetKey(e)->ConvertToDWord(), _pass->options.GetElement(e)->ConvertToDouble());
                else
                    pass->SetValue(_pass->options.GetKey(e)->ConvertToDWord(), _pass->options.GetElement(e)->ConvertToDWord());
            }
        }

    }

    return true;
}

/*-------------------------------------------------------------------
//   GRPFILE3DXML::LoadMaterials
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  24/10/2017 14:06:04
//  @return   bool :
//
//  @param     :
//
*//*-----------------------------------------------------------------*/
bool GRPFILE3DXML::LoadMaterials()
{
    if (isaborted)
        return true;

    for (XDWORD e = 0; e < this->materials.GetSize(); e++)
    {
        MATERIAL* mat           = materials.GetElement(e);
        GRPMATERIAL* material   = GRPMATERIALLIBRARY::Get()->Create(mat->name.Get());

        for (XDWORD s = 0; s < mat->slots.GetSize();s++)
        {
            MATERIAL::SLOT* slot = mat->slots.GetElement(s);
            XDWORD type = slot->id;
            if (slot->properties.GetSize()==6) //es un cubmap?
            {
                XSTRING * s = NULL;
                XVECTOR<XSTRING*> cube;
                XPATH xpath;

                for (XDWORD i = 0; i < 6; i++)
                {
                    s = new XSTRING();
                    xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, slot->properties.Get(i)->Get());
                    s->Set(xpath.Get());
                    cube.Add(s);
                }

                //-------- no puedo cargar el cubemap desde un hilo por que aun no esta en lazy
                //if (GRPMATERIALLIBRARY::Get()->Load(&cube,mat->name.Get()) == GRPMATERIALLOADRESULT_ERROR)
                //    return false;

                cube.DeleteContents();
                cube.DeleteAll();
            }
            else
            {
                XPATH xpath;xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, slot->properties.GetElement(0)->Get());
                GRPTEXTURE* texture = GRPMATERIALLIBRARY::Get()->LoadTexture(slot->properties.GetElement(0)->Get(), xpath.Get());

                if (!texture)
                    return false;

                if (material)
                    material->AddLayer(slot->id,texture);
            }
        }


    }
    return true;
}

/*-------------------------------------------------------------------
//  GRPFILE3DXML::Output
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      01/03/2017 11:55:40
//
//  @param        priority :
//  @param        mask :
//  @param        ... :
*/
/*-----------------------------------------------------------------*/
void GRPFILE3DXML::Output(XDWORD priority,XCHAR* mask,...)
{

    XSTRING s;
    va_list argptr;
    va_start(argptr, mask);
    s.FormatArg(mask,&argptr);
    XDEBUG_PRINTCOLOR(priority,s.Get());
    va_end(argptr);

}

bool GRPFILE3DXML::Notify(GRPMSGTYPE type, GRPSUBJECT* subject)
{
    switch (type)
    {
    case GRPMSG_DESTROYED:
            return true;
            break;

    case GRPMSG_EXIT:
        if (subject->GetSubjectType() == GRPNAMED_TYPE_TASKMANAGER ||
            subject->GetSubjectType() == GRPNAMED_TYPE_TASK)
        {
            this->SetIsAborted(true);
            return true;
        }
        break;
    case GRPMSG_STEP:
        if (subject->GetSubjectType() == GRPNAMED_TYPE_TASK)
        {
            GRPFILE3D* f3d = dynamic_cast<GRPFILE3D*>(subject);
            if (f3d)
            {
                this->tasks.Set(this->currentask, f3d->GetPercent());
                Publish(GRPMSG_STEP, this);
            }

            return true;
        }
        break;
    }
    Publish(type, subject);
    return true;
}