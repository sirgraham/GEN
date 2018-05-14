/*------------------------------------------------------------------------------------------
//  GRPENVMAPPASS.H
*/
/**
// \class
//
//  Manages environment texture rendering
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 03/07/2017 11:53:33
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _GRPENVMAPPASS_H_
#define _GRPENVMAPPASS_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPPass.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/
class GRPENVIRONMENTMAPPASS : public GRPPASS
{
public:

                                 GRPENVIRONMENTMAPPASS  ();
    virtual                     ~GRPENVIRONMENTMAPPASS  ()              {                         }
              void              SetNode                 (GRPNODE* node) { this->node=node;        }
    virtual   GRPTEXTURE*       GetTexture              ()              { return texture; }
    virtual   bool              OnAdded                 ();
    virtual   void              Start                   ();
    virtual   void              Begin                   ();
    virtual   void              End                     ();


protected:

            GRPNODE*      node;
            GRPTEXTURE*   texture;
            GRPMATERIAL*  material;

private:

  void Clean()
  {
    node    =NULL;
    camera  =NULL;
    texture =NULL;
    material=NULL;
  }

};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

