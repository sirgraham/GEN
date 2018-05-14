/*------------------------------------------------------------------------------------------
//  DIOSTREAMCIPHER.H
*/
/**
// \class
//
//  Data IO Stream with Cipher class
//
//  @author  Abraham J. Velez
//
//  Date Of Creation  : 24/11/2014 10:26:28
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _DIOSTREAMCIPHER_H_
#define _DIOSTREAMCIPHER_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XThreadCollected.h"

#include "DIOStream.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

#define DIOSTREAMCIPHER_HEAD_MAGICNUMBER   0xAAE00055
#define DIOSTREAMCIPHER_HEAD_SIZE          0x0e


/*---- CLASS -----------------------------------------------------------------------------*/

class DIOSTREAM;
class CIPHER;
class HASHCRC32;


class DIOSTREAMCIPHER : public DIOSTREAM
{
  public:

                                DIOSTREAMCIPHER                 ( DIOSTREAMCONFIG* config, CIPHER* cipher);
    virtual                    ~DIOSTREAMCIPHER                 ();

    DIOSTREAMCONFIG*            GetConfig                       ()                                { return config;                    }
    bool                        SetConfig                       (DIOSTREAMCONFIG* config)
                                {
                                  this->config = config;
                                  return true;
                                }

    DIOSTREAMSTATUS             GetConnectStatus                ()
                                {
                                  if(!diostream) return DIOSTREAMSTATUS_DISCONNECTED;
                                  return diostream->GetConnectStatus();
                                }

    bool                        Open                            ()
                                {
                                  if(!diostream) return false;
                                  return diostream->Open();
                                }

    bool                        Disconnect                      ()
                                {
                                  if(!diostream) return false;
                                  return diostream->Disconnect();
                                }

    bool                        Close                           ()
                                {
                                  if(!diostream) return false;
                                  return diostream->Close();
                                }



    XDWORD                      Read                            (XBYTE* buffer, XDWORD size);
    XDWORD                      Write                           (XBYTE* buffer, XDWORD size);

    DIOSTREAM*                  GetDIOStreamBase                ()                                { return diostream;                 }
    CIPHER*                     GetCipher                       ()                                { return cipher;                    }

    /*
    XTIMER*                     GetXTimerConnexion              ()
                                {
                                  if(!diostream) return NULL;
                                  return diostream->GetXTimerConnexion();
                                }
    XTIMER*                     GetXTimerNotActivity            ()
                                {
                                  if(!diostream) return NULL;
                                  return diostream->GetXTimerNotActivity();
                                }

    XDWORD                      GetNBytesRead                   ()
                                {
                                  if(!diostream) return 0;
                                  return diostream->GetNBytesRead();
                                }
    XDWORD                      GetNBytesWrite                  ()
                                {
                                  if(!diostream) return 0;
                                  return diostream->GetNBytesWrite();
                                }
    */

  private:

    void                        Clean                           ()
                                {
                                  config            = NULL;
                                  cipher            = NULL;

                                  hashcrc32         = NULL;
                                  diostream         = NULL;

                                  mutexread         = NULL;
                                  mutexwrite        = NULL;

                                  xthreadconnexion  = NULL;
                                }

    static void                 ThreadRunFunction               (void* param);

    DIOSTREAMCONFIG*            config;
    CIPHER*                     cipher;

    HASHCRC32*                  hashcrc32;
    DIOSTREAM*                  diostream;

    XMUTEX*                     mutexread;
    XMUTEX*                     mutexwrite;

    XTHREADCOLLECTED*           xthreadconnexion;
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

