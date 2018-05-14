/*------------------------------------------------------------------------------------------
//  GRPFILE3D.H
*/
/**
// \class
//
//  Loads a 3d model
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 12/01/2015 16:43:29
//  Last Modification :
*/
//* GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _GRPFILE3D_H_
#define _GRPFILE3D_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XFile.h"
#include "Xfactory.h"
#include "XTimer.h"
#include "XLoader.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

class GRPSCENE;
class GRPOBJECT;
class GRPELEMENT;
class GRPMATERIALLIBRARY;

/*---- CLASS -----------------------------------------------------------------------------*/

enum
{
  GRPFILE3D_INCLUDE_NONE        =0,
  GRPFILE3D_INCLUDE_GEOMETRY    =1<<0,
  GRPFILE3D_INCLUDE_NEW_GEOMETRY=1<<1,
  GRPFILE3D_INCLUDE_MATERIALS   =1<<2,
  GRPFILE3D_INCLUDE_ANIMATION   =1<<3,
  GRPFILE3D_INCLUDE_LOCATORS    =1<<4,
  GRPFILE3D_INCLUDE_METADATA    =1<<5,
  GRPFILE3D_INCLUDE_LIGHTS      =1<<6,
  GRPFILE3D_INCLUDE_CAMERAS     =1<<7,
  GRPFILE3D_INCLUDE_DEFORMERS   =1<<8,
  GRPFILE3D_INCLUDE_AUDIO       =1<<9,

  GRPFILE3D_INCLUDE_LAST        =1<<10,
  GRPFILE3D_INCLUDE_OBJECTS     =GRPFILE3D_INCLUDE_GEOMETRY     | GRPFILE3D_INCLUDE_MATERIALS | GRPFILE3D_INCLUDE_LOCATORS,
  GRPFILE3D_INCLUDE_NEW_OBJECTS =GRPFILE3D_INCLUDE_NEW_GEOMETRY | GRPFILE3D_INCLUDE_MATERIALS | GRPFILE3D_INCLUDE_LOCATORS,
  GRPFILE3D_INCLUDE_ALL         =GRPFILE3D_INCLUDE_GEOMETRY     | GRPFILE3D_INCLUDE_MATERIALS | GRPFILE3D_INCLUDE_ANIMATION | GRPFILE3D_INCLUDE_LOCATORS | GRPFILE3D_INCLUDE_METADATA | GRPFILE3D_INCLUDE_LIGHTS | GRPFILE3D_INCLUDE_CAMERAS | GRPFILE3D_INCLUDE_DEFORMERS | GRPFILE3D_INCLUDE_AUDIO,

  GRPFILE3D_KEEP_DUPLICATED_OBJECTS     = GRPFILE3D_INCLUDE_LAST<<1, //flag to keep duplicated objects with new name
  GRPFILE3D_IGNORE_DUPLICATED_OBJECTS   = GRPFILE3D_INCLUDE_LAST<<2, //flag to ignore duplicated objects
  GRPFILE3D_CRASH_ON_DUPLICATED_OBJECTS = GRPFILE3D_INCLUDE_LAST<<3, //flag to abort load on duplicated objects
};

enum GRPFILE3D_VERBOSE_LEVEL
{
  GRPFILE3D_VERBOSE_LEVEL_NONE              =0,
  GRPFILE3D_VERBOSE_LEVEL_ERRORS            =(1<<0),
  GRPFILE3D_VERBOSE_LEVEL_WARNINGS          =(1<<1),
  GRPFILE3D_VERBOSE_LEVEL_CONNECTIONS       =(1<<2),
  GRPFILE3D_VERBOSE_LEVEL_PROPERTIES        =(1<<3),
  GRPFILE3D_VERBOSE_LEVEL_PROCESSING        =(1<<4),
  GRPFILE3D_VERBOSE_LEVEL_ASSETS            =(1<<5),
  GRPFILE3D_VERBOSE_LEVEL_STATUS            =(1<<6),
  GRPFILE3D_VERBOSE_LEVEL_LOW               =(GRPFILE3D_VERBOSE_LEVEL_ERRORS),
  GRPFILE3D_VERBOSE_LEVEL_MEDIUM            =(GRPFILE3D_VERBOSE_LEVEL_ERRORS | GRPFILE3D_VERBOSE_LEVEL_WARNINGS),
  GRPFILE3D_VERBOSE_LEVEL_HIGH              =(GRPFILE3D_VERBOSE_LEVEL_PROCESSING | GRPFILE3D_VERBOSE_LEVEL_PROPERTIES | GRPFILE3D_VERBOSE_LEVEL_CONNECTIONS | GRPFILE3D_VERBOSE_LEVEL_MEDIUM),
  GRPFILE3D_VERBOSE_LEVEL_ALL               =GRPFILE3D_VERBOSE_LEVEL_ERRORS | GRPFILE3D_VERBOSE_LEVEL_WARNINGS | GRPFILE3D_VERBOSE_LEVEL_CONNECTIONS | GRPFILE3D_VERBOSE_LEVEL_PROPERTIES | GRPFILE3D_VERBOSE_LEVEL_PROCESSING | GRPFILE3D_VERBOSE_LEVEL_ASSETS
};


class GRPLOADERTASK;


class GRPFILE3D : public GRPOBSERVER, public GRPSUBJECT
{
public:
  XDWORD                    GetSubjectType          (void) { return GRPNAMED_TYPE_TASK; }

                            GRPFILE3D               ();
  virtual                   ~GRPFILE3D              () ;

  virtual void              SetVerboseLevel         (int            lvl);
  virtual void              SetLoadFlags            (XDWORD         lvl);
  virtual void              SetFilename             (XCHAR*         file);
  virtual XSTRING*          GetFilename             ()                      {return &filename;}
  virtual void              SetFile                 (XCHAR*         file);

  virtual void              SetNextStackID          (XDWORD         nextStackId);
  virtual GRPOBJECT*        Load                    (XCHAR*         file);
  virtual bool              Save                    (XCHAR*         file)     { return true;    }
  virtual void              SetObject               (GRPOBJECT*     obj);
  virtual void              SetScene                (GRPSCENE*      scene);

  virtual void              Abort                   ();

  void                      SetIsAborted            (bool b) { this->isaborted = b; }
  bool                      GetIsAborted            ()       { return this->isaborted; }


  virtual bool              Notify                  (GRPMSGTYPE msg, GRPSUBJECT* subject);
  virtual void              Init                    ()                  {}
  virtual float             GetPercent              ();

  GRPFILE3D*                file3dfactory;

  GRPOBJECT*                object;
  GRPSCENE*                 scene;
  GRPELEMENT*               element;


protected:

  XTIMER*                   chronometer;
  XDWORD                    loadFlags;

  XDWORD                    NextStackID;

  int                       VerboseLevel;

  XSTRING                   filename;
  XSTRING                   file;

  bool                      isaborted;

  float                     percent;
  XVECTOR<float>            tasks;
  XDWORD                    currentask;


private:

  void Clean()
  {
    isaborted           = false;
    chronometer         = NULL;
    object              = NULL;
    element             = NULL;
    loadFlags           = GRPFILE3D_INCLUDE_ALL;
    VerboseLevel        = GRPFILE3D_VERBOSE_LEVEL_ERRORS;
    NextStackID         = 0;
    file3dfactory       = NULL;
    percent             = 0.0f;
    currentask          = 0;
  }

};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif


