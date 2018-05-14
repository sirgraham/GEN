/*------------------------------------------------------------------------------------------
//  GRPLOADERINTERFACE.H
*/
/**
// \class
//
//  Loader interface
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 21/09/2016 10:12:11
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _GRPLOADERINTERFACE_H_
#define _GRPLOADERINTERFACE_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPLoader.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


class GRPSCREEN;
class GRPSCENE;
class GRPSHADERPROGRAM;
class GRPCONSOLE;


/*---- CLASS -----------------------------------------------------------------------------*/
class GRPLOADERINTERFACE : public GRPOBSERVER, public GRPSUBJECT
{
public:
                      GRPLOADERINTERFACE  ();
                      GRPLOADERINTERFACE  (GRPSCENE*            scene,
                                           XCHAR*               name,
                                           GRPSHADERPROGRAM*    program=NULL);
                      ~GRPLOADERINTERFACE ();


  virtual bool        AddTask             (XCHAR*               toload ,
                                           XDWORD               loadflags=GRPFILE3D_INCLUDE_ALL);

  virtual bool        Notify              (GRPMSGTYPE           msg,
                                           GRPSUBJECT*          subject);
  virtual XDWORD      GetSubjectType      ();
  virtual bool        Init                ();
  virtual bool        Do                  ();

          bool        End                 ();
          void        Abort               ();
          void        AttachConsole       (GRPCONSOLE*          console);

          GRPLOADER*  GetLoader           () { return loader; }

          float       total;



          void        SetScene            (GRPSCENE*            scene   );
          void        SetScreen           (GRPSCREEN*           screen  );
          void        SetBitmap           (XCHAR*               bitmap  );
          void        SetShader           (GRPSHADERPROGRAM*    shader  );

protected:

  XSTRING             bitmap;

  GRPLOADER*          loader;
  GRPSCENE*           scene;
  GRPELEMENT*         element;
  GRPOBJECT*          object;
  GRPSCREEN*          screen;
  GRPSHADERPROGRAM*   shader;
  GRPCONSOLE*         console;

  bool                isaborted;

private:

  void Clean()
  {
    console =NULL;
    loader  =NULL;
    scene   =NULL;
    element =NULL;
    object  =NULL;
    screen  =NULL;
    shader  =NULL;
    total   =0.0f;
    isaborted = false;
  }

};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

