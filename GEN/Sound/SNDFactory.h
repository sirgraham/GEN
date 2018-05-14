/*------------------------------------------------------------------------------------------
//  SNDFACTORY.H
*/
/**
// \file
//
//  base sound system class
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 11/11/2015 10:45:14
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _SNDFACTORY_H_
#define _SNDFACTORY_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XBase.h"
#include "XVector.h"
#include "XList.h"
#include "XString.h"
#include "XPath.h"

#include "XSubject.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/

class SNDELEMENT;
class SNDSTREAMELEMENT;
class SNDSOURCE;
class SNDINSTANCE;
class SNDRESAMPLER;
class SNDFILE;
class XFACTORY;
class XPATHS;
class XPUBLISHER;

class SNDFACTORY
{
  public:

    static  bool                              SoundIsActive;

    static  SNDFACTORY*                       Instance;
    static  SNDFACTORY*                       Get                       ()                                                      { return Instance;                  }  //wtf!
    static  void                              Destroy                   ()                                                      { delete Instance;  Instance=NULL;  }

                                              SNDFACTORY                ()                                                      { Clean();                          }
    virtual                                  ~SNDFACTORY                ()                                                      { Clean();                          }

    virtual SNDELEMENT*                       AddFile                   (XPATH& xpath, XSTRING* namefile, bool stream = false)  { return NULL;                      }
    virtual SNDELEMENT*                       AddFile                   (XPATH& xpath, XCHAR* namefile, bool stream = false)    { return NULL;                      }
    virtual SNDELEMENT*                       GetFile                   (XSTRING* namefile, bool stream = false)                { return NULL;                      }
    virtual SNDELEMENT*                       GetFile                   (XCHAR* namefile, bool stream = false)                  { return NULL;                      }

    virtual bool                              RemoveFile                (SNDELEMENT* element)                                   { return false;                     }

    virtual SNDSTREAMELEMENT*                 GetStreamer               ()                                                      { return NULL;                      }

    virtual SNDINSTANCE*                      PlaySound                 (SNDELEMENT* element)                                   { return NULL;                      }
    virtual void                              StopSound                 (SNDELEMENT* element)                                   {                                   }
    virtual SNDINSTANCE*                      PauseSound                (SNDELEMENT* element)                                   { return NULL;                      }

    virtual bool                              IsAnyPlaying              ()                                                      { return false;                     }

    virtual void                              StopAll                   ()                                                      {                                   }

    XSUBJECT*                                 GetXSUBJECT               ()                                                      { return this->xsubject;            }
    void                                      SetXSUBJECT               (XSUBJECT* xsubject)                                    { this->xsubject = xsubject;        }

    virtual void                              SetMasterVolume           (float mastervolume)                                    {                                   }
    virtual float                             GetMasterVolume           ()                                                      { return 0.0f;                      }

    void                                      MarkInstanceForDeletion   (SNDINSTANCE* sndinstance)                              {todeleteinstances.Add(sndinstance);}

    bool                                      IsInit                    ()                                                      { return isinit;                    }
    void                                      SetPublisher              ()                                                      {                                   }

    virtual void                              IniEvents                 ()                                                      {                                   }
    virtual void                              EndEvents                 ()                                                      {                                   }

    void                                      Update                    ();

    virtual SNDRESAMPLER*                     GetResampler              () = 0;
    virtual SNDFILE*                          CreateSNDFile             () = 0;
    virtual void                              DeleteSNDFile             (SNDFILE* file) = 0;

  protected:

    virtual void                              ImpUpdate                 ()                                                      {                                   }

    XSUBJECT*                                 xsubject;
    XVECTOR<SNDINSTANCE*>                     instances;
    XLIST<SNDINSTANCE*>                       todeleteinstances;

    bool                                      isinit;

  private:

    void                                      Clean                     ()
                                              {
                                                xsubject    = NULL;
                                                todeleteinstances.SetIsMulti(false);
                                                isinit      =false;
                                              }

};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

