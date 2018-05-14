/*------------------------------------------------------------------------------------------
//  GRPOBJECT.H
*/
/**
// \class
//
//  Graphics Base for Object class
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 12/05/2014 12:58:17
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _GRPOBJECT_H_
#define _GRPOBJECT_H_

/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XVector.h"
#include "XMap.h"

#include "GRPColor.h"
#include "GRPMatrix.h"
#include "GRPMaterial.h"
#include "GRPShader.h"
#include "GRPVertex.h"
#include "GRPElement.h"
#include "GRPAnimation.h"

#include "XMemory.h"

#include "AILogic.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

/*---- CLASS -----------------------------------------------------------------------------*/



class GRPMATERIAL;
class GRPVIEWPORT;
class GRPPOINT;
class GRPFACE;
class GRPSCENE;
class GRPCAMERA;
class GRPLIGHT;
class GRPTEXTELEMENT;
class LOGIC;
class GRPPASS;

class GRPOBJECT : public virtual GRPNAMED, public GRPOBSERVER, public GRPSUBJECT
{
  public:
    virtual   XDWORD              GetSubjectType          () { return GRPNAMED_TYPE_GRPOBJECT; }

                                  GRPOBJECT               ();
    virtual                      ~GRPOBJECT               ();
    void                          SetTag                  (XQWORD tag);

    virtual bool                  Draw                    (GRPMATRIX&,GRPSCENE*);

    GRPMATERIAL*                  GetMaterial             ();
    void                          SetMaterial             (GRPMATERIAL* material);

    void                          SetProgram              (GRPSHADERPROGRAM* p);
    void                          SetRenderMode           (GRPRENDERMODE mode);

    GRPELEMENT*                   FindByName              (XCHAR* name);
    bool                          AddElement              (GRPELEMENT* element);

    bool                          AddText                 (GRPTEXTELEMENT* tbox);

    XVECTOR<GRPELEMENT*>*         GetElements             ();
    GRPSHADERPROGRAM*             GetProgram              ();

    GRPNODE*                      GetNode                 ();
    virtual void                  SetNode                 (GRPNODE* node);

    bool                          MergeElements           ();
    bool                          MergeAnimations         ();

    void                          SetVisible              (bool visible);
    GRPANIMATIONCONTROL*          GetAnimationControl     ();
    void                          SetAnimationControl     (GRPANIMATIONCONTROL*);
    void                          PlayAll                 ();

    bool                          Add(GRPPASS*     pass);
    bool                          Add(GRPNODE*     node);
    bool                          Add(GRPCAMERA*   camera);
    bool                          Add(GRPLIGHT*  light);
    bool                          Add(GRPELEMENT* element);
    bool                          Add(GRPTEXTELEMENT* textbox);

    GRPNODE*                      FindNode                (XCHAR* name);
    GRPELEMENT*                   FindElementByNodeName   (XCHAR* name);

    GRPELEMENT*                   FindElement             (XCHAR* name);
    GRPCAMERA*                    FindCamera              (XCHAR* name);
    GRPLIGHT*                     FindLight               (XCHAR* name);
    GRPTEXTELEMENT*               FindTextBox             (XCHAR* name);

    GRPNODE*                      CreateNode              ();
    GRPLIGHT*                     CreateLight             ();
    GRPCAMERA*                    CreateCamera            ();
    bool                          CreateTextLocators      ();
    GRPTEXTELEMENT*               CreateTextFromLocator   (GRPNODE* node=NULL);

    virtual bool                  Notify                  (GRPMSGTYPE msg, GRPSUBJECT* notifier);

    void                          SetCurrentCamera        (XDWORD n)                { currentcamera = n; }
    XDWORD                        GetCurrentCamera        ()                        { return currentcamera; }
    GRPCAMERA*                    GetCamera               ()                        { return cameras.Get(currentcamera); }

    XVECTOR<GRPNODE*          >*  GetNodes                ()                        { return &nodes;                                    }
    XVECTOR<GRPCAMERA*        >*  GetCameras              ()                        { return &cameras;                                  }
    XVECTOR<GRPLIGHT*         >*  GetLights               ()                        { return &lights;                                   }
    XVECTOR<GRPTEXTELEMENT*   >*  GetTexts                ()                        { return &textboxes;                                }

    AI::LOGIC*                    GetLogic                ()                        { return logic;                                     }
    void                          SetLogic                (AI::LOGIC* logic)        { this->logic=logic;                                }
    GRPBB*                        GetBoundingBox          ()                        { return &this->boundingbox;;                       }
    XVECTOR<GRPPASS*>*            GetPasses               ()                        { return &this->passes; }


    void                          UpdateBoundingBox       ();
    GRPVECTOR                     UpVector;


  protected:

    AI::LOGIC*                    logic;

    GRPANIMATIONCONTROL*          AnimationControl;

    GRPNODE*                      node;

    XVECTOR<GRPNODE*>             nodes;
    XVECTOR<GRPELEMENT*>          elements;
    XVECTOR<GRPLIGHT*>            lights;
    XVECTOR<GRPCAMERA*>           cameras;
    XVECTOR<GRPTEXTELEMENT*>      textboxes;

    XDWORD                        currentcamera;

    GRPSHADERPROGRAM*             program;
    GRPMATERIAL*                  material;
    GRPANIMATIONLAYER*            animation;

    XMUTEX*                       lock;

    GRPBB                         boundingbox;

    XVECTOR<GRPPASS*>             passes;

  private:

    void                          Clean                   ()
                                  {
                                    animation           = NULL;
                                    material            = NULL;
                                    node                = NULL;
                                    program             = NULL;
                                    AnimationControl    = NULL;
                                    lock                = NULL;
                                    currentcamera       = 0;
                                    logic               = NULL;
                                  }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif


