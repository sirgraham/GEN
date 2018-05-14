
/*------------------------------------------------------------------------------------------
//  GRPPASSMANAGER.CPP
//
//  Manages passes
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 28/07/2016 11:07:07
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPPassManager.h"
#include "GRPPass.h"
#include "GRPWidgetPass.h"

#include "GRPSkyboxPass.h"
#include "GRPBackgroundPass.h"
#include "GRPBlendingPass.h"
#include "GRPReflectionPass.h"
#include "GRPStencilPass.h"
#include "GRPShadowPass.h"
#include "GRPEnvMapPass.h"
#include "GRPViewportPass.h"
#include "GRPOverlayPass.h"
#include "GRPDepthPass.h"
#include "GRPRenderTargetPass.h"
#include "GRPHighDinamicRangePass.h"

#include "GRPVisualDebugger.h"


/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/

GRPPASSMANAGER* GRPPASSMANAGER::Instance=NULL;

/*-------------------------------------------------------------------
//  GRPPASSMANAGER::Get
*/
/**
//  Singleton instance get
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/07/2016 11:08:51
//
//  @return       GRPPASSMANAGER* :
//
*/
/*-----------------------------------------------------------------*/
GRPPASSMANAGER* GRPPASSMANAGER::Get()
{
  if (GRPPASSMANAGER::Instance==NULL)
    GRPPASSMANAGER::Instance=new GRPPASSMANAGER();

  return GRPPASSMANAGER::Instance;
}


/*-------------------------------------------------------------------
//  GRPPASSMANAGER::Create
*/
/**
//  Create a Pass object and add to the passes list
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/07/2016 11:15:46
//
//  @param        XCHAR*    : pass name
//  @return       GRPPASS*  : created pass
//
*/
/*-----------------------------------------------------------------*/
GRPPASS* GRPPASSMANAGER::Create(XCHAR* passtype, XDWORD priority)
{
  GRPPASS*  pass=NULL;
  if (passtype!=NULL)
  {
    if (XSTRING::Compare(passtype,__L("skybox"),      true)==0)   {   pass=new GRPSKYBOXPASS            (); } else
    if (XSTRING::Compare(passtype,__L("blending"),    true)==0)   {   pass=new GRPBLENDINGPASS          (); } else
    if (XSTRING::Compare(passtype,__L("background"),  true)==0)   {   pass=new GRPBACKGROUNDPASS        (); } else
    if (XSTRING::Compare(passtype,__L("reflection"),  true)==0)   {   pass=new GRPREFLECTIONPASS        (); } else
    if (XSTRING::Compare(passtype,__L("environment"), true)==0)   {   pass=new GRPENVIRONMENTMAPPASS    (); } else
    if (XSTRING::Compare(passtype,__L("shadows"),     true)==0)   {   pass=new GRPSHADOWPASS            (); } else
    if (XSTRING::Compare(passtype,__L("pointshadows"),true)==0)   {   pass=new GRPSHADOWPOINTLIGHTPASS  (); } else
    if (XSTRING::Compare(passtype,__L("stencil"),     true)==0)   {   pass=new GRPSTENCILPASS           (); } else
    if (XSTRING::Compare(passtype,__L("debug"),       true)==0)   {   pass=new GRPDEBUGGERPASS          (); } else
    if (XSTRING::Compare(passtype,__L("viewport"),    true)==0)   {   pass=new GRPVIEWPORTPASS          (); } else
    if (XSTRING::Compare(passtype,__L("overlay"),     true)==0)   {   pass=new GRPOVERLAYPASS           (); } else
    if (XSTRING::Compare(passtype,__L("depth"),       true)==0)   {   pass=new GRPDEPTHPASS             (); } else
    if (XSTRING::Compare(passtype,__L("offscreen"),   true)==0)   {   pass=new GRPRENDERTARGETPASS      (); } else
    if (XSTRING::Compare(passtype,__L("hdr"),         true)==0)   {   pass=new GRPHIGHDINAMICRANGEPASS  (); } else
    if (XSTRING::Compare(passtype,__L("widget"),      true)==0)   {   pass=new GRPWIDGETPASS            (); }
  }

  if (pass==NULL)
            pass=new GRPPASS(__L("pass"));

            pass->SetPriority(priority);
            Passes.Add(pass);
  return    pass;
}

/*-------------------------------------------------------------------
//  GRPPASSMANAGER::Destroy
*/
/**
//  Destroys pass manager instance
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      01/08/2016 12:15:02
//
*/
/*-----------------------------------------------------------------*/
void GRPPASSMANAGER::Destroy()
{
  if (GRPPASSMANAGER::Instance!=NULL)
    delete GRPPASSMANAGER::Instance;

   GRPPASSMANAGER::Instance=NULL;
}


/*-------------------------------------------------------------------
//  GRPPASSMANAGER::GRPPASSMANAGER
*/
/**
//
//  Class Constructor GRPPASSMANAGER
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/07/2016 11:11:18
//
*/
/*-----------------------------------------------------------------*/
GRPPASSMANAGER::GRPPASSMANAGER()
{
  Clean();
}

/*-------------------------------------------------------------------
//  GRPPASSMANAGER::~GRPPASSMANAGER
*/
/**
//
//   Class Destructor GRPPASSMANAGER
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/07/2016 11:11:23
//
*/
/*-----------------------------------------------------------------*/
GRPPASSMANAGER::~GRPPASSMANAGER()
{
    this->Passes.DeleteContents ();
    this->Passes.DeleteAll      ();
}


/*-------------------------------------------------------------------
//  GRPPASSMANAGER::Notify
*/
/**
//  GRPObserver interface notification.
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/07/2016 11:19:30
//
//  @return       bool : if the msg is already processed
//
//  @param        msg :  GRPMSGTYPE type if message
//  @param        subject : GRPSUBJECT* caller
*/
/*-----------------------------------------------------------------*/
bool  GRPPASSMANAGER::Notify(GRPMSGTYPE msg,GRPSUBJECT* subject)
{
  return true;
}

GRPPASS*  GRPPASSMANAGER::GetPass(XCHAR* name)
{
    for (XDWORD e = 0; e < this->Passes.GetSize(); e++)
    {
        if (this->Passes.Get(e)->GetName()->Compare(name, false) == 0)
            return this->Passes.Get(e);
    }
    return NULL;
}