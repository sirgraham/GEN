/*------------------------------------------------------------------------------------------
//  PHYWORLD.H
*/
/**
// \class
//
//  Abstract physical world
//
//  @author  No se ha podido encontrar el fichero 'MacrosAuthor.txt' en el directorio del IDE. No se han rellenado los datos para las macros.
//
//  Date Of Creation  : 26/05/2016 19:37:50
//  Last Modification :
*/
/*  (C) Copyright No se ha podido encontrar el fichero 'MacrosAuthor.txt' en el directorio del IDE. No se han rellenado los datos para las macros.. All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _PHYWORLD_H_
#define _PHYWORLD_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XVector.h"
#include "GRPVector.h"
#include "PHYEntity.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

class PHYENGINE;
class GRPOBJECT;

/*---- CLASS -----------------------------------------------------------------------------*/
class PHYWORLD
{
public:

                    PHYWORLD    ();
virtual             ~PHYWORLD   ();


virtual bool        Init            (PHYENGINE* engine)=0;
virtual bool        Step            ()=0;
virtual bool        End             ()=0;
virtual bool        Destroy         ();
virtual bool        DestroyEntities ();
virtual bool        DestroyWorld    ()=0;
        bool        AddEntity       (PHYENTITY* entity);

        PHYENTITY*  GetEntity       (int e) { return this->Entities.Get(e);}
XVECTOR<PHYENTITY*>*GetEntities     ()      { return &this->Entities;}


virtual PHYENTITY*  CreateStatic    (GRPELEMENT* element=NULL,PHYENVELOPETYPE envelope=PHYENVELOPETYPE_BOX, PHYENTITYTYPE type=PHYENTITYTYPE_DYNAMIC)=0;
virtual PHYENTITY*  CreateEntity    (GRPELEMENT* element=NULL,PHYENVELOPETYPE envelope=PHYENVELOPETYPE_BOX, PHYENTITYTYPE type=PHYENTITYTYPE_DYNAMIC)=0;

virtual bool        SetGravity      (GRPVECTOR vector)=0;
        GRPVECTOR*  GetGravity      ();

              GRPOBJECT* obj;

protected:

    GRPVECTOR           Gravity;

    XVECTOR<PHYENTITY*> Entities;

private:

    void Clean()
    {

    }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

