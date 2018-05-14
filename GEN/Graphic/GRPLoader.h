/*------------------------------------------------------------------------------------------
//  GRPLOADER.H
*/
/**
// \class
//
//  Loads graphics
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 14/09/2016 10:38:30
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _GRPLOADER_H_
#define _GRPLOADER_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPFile3D.h"
#include "GRPSubject.h"
#include "XLoader.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

class GRPCONSOLE;
class GRPOBJECT;

class GRPLOADERTASK : public XLOADERTASK, public GRPSUBJECT, public GRPOBSERVER
{
public:
                              GRPLOADERTASK     (XCHAR* name);
                              ~GRPLOADERTASK    ();



virtual XDWORD                AddLayer          (XCHAR* filename, XDWORD id,  XDWORD flags=GRPFILE3D_INCLUDE_ALL);
virtual XDWORD                GetSubjectType    ();
virtual void                  Run               ();
virtual void                  Add               (float n);

virtual void                  Update            ();
virtual void                  SetCurrent        (float current);

virtual bool                  Notify            (GRPMSGTYPE, GRPSUBJECT*);

      GRPOBJECT*  object;
      GRPFILE3D*  grpfile3d;

protected:

      XMAP<XSTRING*, XDWORD>  AdditionalLayers;
      XDWORD                  loadflags;
      XDWORD                  verbose;

      float                   layerTotal;



      void Clean()
      {
        file      =NULL;
        object    =NULL;
        loadflags =0;
        verbose = GRPFILE3D_VERBOSE_LEVEL_ERRORS;
        layerTotal=0.0;
        grpfile3d =NULL;
      }
};


/*---- CLASS -----------------------------------------------------------------------------*/
class GRPLOADER : public XLOADER, public GRPOBSERVER, public GRPSUBJECT
{
public:
                          GRPLOADER       ();
                          ~GRPLOADER      ();

  virtual   XDWORD        GetSubjectType  ();
  virtual   bool          Notify          (GRPMSGTYPE msg, GRPSUBJECT* subject);
  virtual   XLOADERTASK*  CreateTask      (XCHAR* pname);
  virtual   void          Print           (XDWORD color, XCHAR* text);
  virtual   void          AttachConsole   (GRPCONSOLE* c) { this->console=c;}



protected:

            GRPCONSOLE*   console;

private:

};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

