
/*------------------------------------------------------------------------------------------
//  DIOSTREAMCIPHER.CPP
//
//  Data IO Stream with Cipher classs
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 24/11/2014 13:32:35
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XFactory.h"
#include "XBuffer.h"

#include "DIOFactory.h"
#include "DIOStream.h"

#include "HashCRC32.h"
#include "Cipher.h"

#include "DIOStreamCipher.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  DIOSTREAMCIPHER::DIOSTREAMCIPHER
*/
/**
//
//  Class Constructor DIOSTREAMCIPHER
//
//  @author       Abraham J. Velez
//  @version      24/11/2014 16:20:05
//



//  @param        config :
//  @param        cipher :
*/
/*-----------------------------------------------------------------*/
DIOSTREAMCIPHER::DIOSTREAMCIPHER(DIOSTREAMCONFIG* config, CIPHER* cipher) : DIOSTREAM()
{
  Clean();

  this->config      = config;
  this->cipher      = cipher;

  hashcrc32 = new HASHCRC32();
  if(hashcrc32)
    {
      diostream = (DIOSTREAM*)diofactory->CreateStreamIO(config);

      mutexread   = xfactory->Create_Mutex();
      mutexwrite  = xfactory->Create_Mutex();

      if(diostream && mutexread && mutexwrite)
        {
          xthreadconnexion  = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAMCIPHER, __L("DIOSTREAMCIPHER::DIOSTREAMCIPHER"), ThreadRunFunction, (void*)this);
          if(xthreadconnexion)
            {
              xthreadconnexion->Ini();
            }
        }
    }
}



/*-------------------------------------------------------------------
//  DIOSTREAMCIPHER::~DIOSTREAMCIPHER
*/
/**
//
//   Class Destructor DIOSTREAMCIPHER
//
//  @author       Abraham J. Velez
//  @version      24/11/2014 13:34:46
//
*/
/*-----------------------------------------------------------------*/
DIOSTREAMCIPHER::~DIOSTREAMCIPHER()
{
  if(xthreadconnexion)
    {
      xthreadconnexion->End();
      DELETEXTHREAD(XTHREADGROUPID_DIOSTREAMCIPHER, xthreadconnexion);
    }

  if(mutexread)   xfactory->Delete_Mutex(mutexread);
  if(mutexwrite)  xfactory->Delete_Mutex(mutexwrite);

  if(diostream) diofactory->DeleteStreamIO(diostream);

  if(hashcrc32) delete hashcrc32;

  Clean();
}



/*-------------------------------------------------------------------
//  DIOSTREAMCIPHER::Read
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      10/09/2015 16:56:54
//
//  @return       XDWORD :
//
//  @param        buffer :
//  @param        size :
*/
/*-----------------------------------------------------------------*/
XDWORD DIOSTREAMCIPHER::Read(XBYTE* buffer, XDWORD size)
{
  if(mutexread)   mutexread->Lock();

  XDWORD br = DIOSTREAM::Read(buffer, size);

  if(mutexread)   mutexread->UnLock();

  return br;
}



/*-------------------------------------------------------------------
//  DIOSTREAMCIPHER::Write
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      10/09/2015 16:57:00
//
//  @return       XDWORD :
//
//  @param        buffer :
//  @param        size :
*/
/*-----------------------------------------------------------------*/
XDWORD DIOSTREAMCIPHER::Write(XBYTE* buffer, XDWORD size)
{
  if(mutexwrite)  mutexwrite->Lock();

  XDWORD wr = DIOSTREAM::Write(buffer, size);

  if(mutexwrite)  mutexwrite->UnLock();

  return wr;
}




/*-------------------------------------------------------------------
//  DIOSTREAMCIPHER::ThreadRunFunction
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      24/11/2014 17:15:30
//
//  @param        param :
*/
/*-----------------------------------------------------------------*/
void DIOSTREAMCIPHER::ThreadRunFunction(void* param)
{
  DIOSTREAMCIPHER* diostreamcipher = (DIOSTREAMCIPHER*)param;
  if(!diostreamcipher) return;

  DIOSTREAM* diostream = diostreamcipher->diostream;
  if(!diostream) return;

  CIPHER* cipher   = diostreamcipher->cipher;
  if(!cipher) return;

  int br = diostream->GetInXBuffer()->GetSize();
  if(br>DIOSTREAMCIPHER_HEAD_SIZE)
    {
      if(diostreamcipher->mutexread)  diostreamcipher->mutexread->UnLock();

      XDWORD  magicnumber;
      XBYTE   ciphertype;
      XBYTE   cipherpadding;
      XDWORD  sizebuffer;
      XDWORD  CRC32[2];
      XBUFFER input;

      input.Resize(DIOSTREAMCIPHER_HEAD_SIZE);

      diostream->GetInXBuffer()->Get(input.Get(), DIOSTREAMCIPHER_HEAD_SIZE, 0);

      diostreamcipher->hashcrc32->ResetResult();
      CRC32[0] = diostreamcipher->hashcrc32->Do(input.Get(), DIOSTREAMCIPHER_HEAD_SIZE - sizeof(XDWORD));

      input.Extract(magicnumber   , 0);
      input.Extract(ciphertype    , 0);
      input.Extract(cipherpadding , 0);
      input.Extract(sizebuffer    , 0);
      input.Extract(CRC32[1]      , 0);

      if((magicnumber == DIOSTREAMCIPHER_HEAD_MAGICNUMBER) &&  (CRC32[0] == CRC32[1]))
        {
          if(diostream->GetInXBuffer()->GetSize() >= sizebuffer + DIOSTREAMCIPHER_HEAD_SIZE)
            {
              input.Empty();

              input.Resize(DIOSTREAMCIPHER_HEAD_SIZE + sizebuffer);

              diostream->GetInXBuffer()->Extract(input.Get(), 0, sizebuffer + DIOSTREAMCIPHER_HEAD_SIZE);
              input.Extract(NULL, 0, DIOSTREAMCIPHER_HEAD_SIZE);

              cipher->Uncipher(input);

              diostreamcipher->GetInXBuffer()->Add(cipher->GetResult()->Get(), cipher->GetResult()->GetSize() - cipherpadding);
            }
        }

      if(diostreamcipher->mutexread)  diostreamcipher->mutexread->UnLock();
    }


  int  bw = diostreamcipher->GetOutXBuffer()->GetSize();
  if(bw)
    {
      if(diostreamcipher->mutexwrite) diostreamcipher->mutexwrite->Lock();

      XBUFFER output;

      cipher->Cipher(diostreamcipher->GetOutXBuffer()->Get(), diostreamcipher->GetOutXBuffer()->GetSize());

      output.Add((XDWORD)DIOSTREAMCIPHER_HEAD_MAGICNUMBER);
      output.Add((XBYTE)cipher->GetType());
      output.Add((XBYTE)(cipher->GetResult()->GetSize() - bw));
      output.Add((XDWORD)cipher->GetResult()->GetSize());

      diostreamcipher->hashcrc32->ResetResult();
      output.Add((XDWORD)diostreamcipher->hashcrc32->Do(output));
      output.Add((XBYTE*)cipher->GetResult()->Get(), cipher->GetResult()->GetSize());

      if(diostream->Write(output) == output.GetSize())
        {
          //if(diostream->WaitToFlushOutXBuffer())
          diostreamcipher->GetOutXBuffer()->Extract(NULL, 0 , bw);
        }

      if(diostreamcipher->mutexwrite) diostreamcipher->mutexwrite->UnLock();
    }
}
