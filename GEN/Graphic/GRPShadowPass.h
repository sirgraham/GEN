/*------------------------------------------------------------------------------------------
//  GRPSHADOWPASS.H
*/
/**
// \class
//
//  Controls shadow pass
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 27/07/2016 12:20:47
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _GRPSHADOWPASS_H_
#define _GRPSHADOWPASS_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPPass.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
class GRPLIGHT;

/*---- CLASS -----------------------------------------------------------------------------*/
class GRPSHADOWPASS : public GRPPASS
{
public:
                                GRPSHADOWPASS       ();

              void              SetLight            (GRPLIGHT*        light);
              void              SetRenderTarget     (GRPRENDERTARGET* rt);

    virtual   void              Begin               ();
    virtual   void              End                 ();

    virtual   bool              OnAdded             ();

              int               GetShadowCasters    ();
              int               SetShadowReceivers  ();

protected:

              GRPLIGHT*         light;
              GRPRENDERTARGET*  rendertarget;
              float shadowsize;
private:

      void    Clean()
      {
          light         =NULL;
          rendertarget  =NULL;
          shadowsize    = 512;
      }



};

class GRPSHADOWPOINTLIGHTPASS : public GRPSHADOWPASS
{
public:
                                GRPSHADOWPOINTLIGHTPASS     ();
              void              SetRenderTarget             (GRPRENDERTARGET* rt);
              void              Start                       ();
              void              Begin                       ();
              void              End                         ();
    virtual   bool              OnAdded                     ();

protected:

private:

      void    Clean()
      {
        shadowsize    = 512;
      }



};

/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

