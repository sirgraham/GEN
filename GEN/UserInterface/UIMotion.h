/*------------------------------------------------------------------------------------------
// UIMotion.HPP
*/
/**
// \class
//
// describes an input motion in the UI
//
//  @author: Imanol Celaya Ruiz de Alegria
//
//  Date of Creation : 17/10/2017 10:45:19
//  Last Modification :
*/
/* GEN  Copyright (C).  All right reserved.
/*------------------------------------------------------------------------------------------*/


#ifndef _UIMOTION_HPP
#define _UIMOTION_HPP
/*---- INCLUDES --------------------------------------------------------------------------*/
#include "XBase.h"
#include "XVector.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

/*---- CLASS -----------------------------------------------------------------------------*/

enum UIMOTION_TYPE
{
  UIMOTION_TYPE_UNKNOWN = 0 ,
  UIMOTION_TYPE_NONE        ,
  UIMOTION_TYPE_DOWN        ,
  UIMOTION_TYPE_MOVE        ,
  UIMOTION_TYPE_UP          ,
  UIMOTION_TYPE_END
};

class UIMOTION
{
public:
  struct UIMOTIONENTRY
  {
    UIMOTION_TYPE                           type;
    float                                   x;
    float                                   y;
    XQWORD                                  timestamp;
  };

                                            UIMOTION                                  ();
  virtual                                  ~UIMOTION                                  ();

  bool                                      AddMotion                                 (UIMOTIONENTRY* entry);

  UIMOTIONENTRY*                            GetLastMotion                             ();
  XDWORD                                    GetMotionNumber                           ();
  UIMOTIONENTRY*                            GetMotion                                 (XDWORD index);

  float                                     GetCurrentX                               ();
  float                                     GetCurrentY                               ();
  XQWORD                                    GetCurrentTimeStamp                       ();

  float                                     GetMotionDiffX                            ();
  float                                     GetMotionDiffY                            ();
  XQWORD                                    GetTimeStampDiffX                         ();

  void                                      SetCurrentX                               (float x);
  void                                      SetCurrentY                               (float y);
  void                                      SetCurrentTimeStamp                       (XQWORD timestamp);

  void                                      Clear                                     ();

protected:

  XVECTOR<UIMOTIONENTRY*>                   entries;

  float                                     x;
  float                                     y;
  XQWORD                                    timestamp;

private:
  void                                      Clean                                     ()
                                            {
                                              x = 0.0f;
                                              y = 0.0f;
                                              timestamp = 0;
                                            }
};
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif