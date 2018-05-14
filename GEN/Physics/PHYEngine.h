/*------------------------------------------------------------------------------------------
//  PHYENGINE.H
*/
/**
// \class
//
//  Physics abstract engine
//
//  @author Diego Martinez Ruiz de Gaona

//  Date Of Creation  : 26/05/2016 17:57:05
//  Last Modification :
*/
/*  (C) Copyright All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _PHYENGINE_H_
#define _PHYENGINE_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPObserver.h"
#include "GRPSubject.h"
#include "XVector.h"



/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

class GRPSCENE;
class PHYENTITY;
class PHYWORLD;

/*---- CLASS -----------------------------------------------------------------------------*/
class PHYENGINE : public GRPOBSERVER,public GRPSUBJECT
{
public:

    static  PHYENGINE* Create(XCHAR*);

             PHYENGINE  ()
             {
                 classtype=GRPNAMED_TYPE_GRPLAYER;
                 name.Set("PHYENGINE");
             }

    virtual ~PHYENGINE  () {}

    virtual bool        Init            (GRPSCENE* scene)=0;
    virtual bool        Destroy         ()=0;
    virtual bool        End             ()=0;
    virtual bool        Step            ()=0;
    virtual PHYWORLD*   CreateWorld     ()=0;

            PHYWORLD*   GetWorld        ();
    virtual bool        AddWorld        (PHYWORLD* world);
    bool                Notify          (GRPMSGTYPE msg,GRPSUBJECT* notifier);


protected:

    GRPSCENE*           scene;
    bool                active;
    PHYWORLD*           world;
    XVECTOR<PHYWORLD*>  Worlds;

private:

};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

