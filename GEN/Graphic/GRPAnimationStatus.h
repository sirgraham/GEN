/*------------------------------------------------------------------------------------------
// GRPAnimationStatus.HPP
*/
/**
// \class
//
// Animation Status
//
//  @author: Diego Martinez Ruiz de Gaona
//
//  Date of Creation : 28/11/2017 11:12:14
//  Last Modification :
*/
/* GEN  Copyright (C).  All right reserved.
/*------------------------------------------------------------------------------------------*/


#ifndef _GRPANIMATIONSTATUS_HPP
#define _GRPANIMATIONSTATUS_HPP
/*---- INCLUDES --------------------------------------------------------------------------*/

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

/*---- CLASS -----------------------------------------------------------------------------*/

enum GRPANIMATION_STATUS
{
    GRPANIMATION_STATUS_UNKNOWN = 0,
    GRPANIMATION_STATUS_STARTED,
    GRPANIMATION_STATUS_PLAYING,
    GRPANIMATION_STATUS_STOPPED,
    GRPANIMATION_STATUS_FINISHED,
    GRPANIMATION_STATUS_PAUSED,
    GRPANIMATION_STATUS_INACTIVE,
    GRPANIMATION_STATUS_WAITING,
    GRPANIMATION_STATUS_NOTIFY,
    GRPANIMATION_STATUS_ERROR
};

/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif