/*------------------------------------------------------------------------------------------
//  XBUFFERBITS.H
*/
/**
// \file
//
//  reads a bit stream from a xbuffer
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 01/04/2016 14:12:10
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _XBUFFERBITS_H_
#define _XBUFFERBITS_H_


/*---- INCLUDES --------------------------------------------------------------------------*/
#include "XBase.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/

class XBUFFER;

class XBUFFERBITS
{
public:

                          XBUFFERBITS                     (XBUFFER* xbuffer);
  virtual                ~XBUFFERBITS                     ();

  bool                    IsByteAligned                   ();
  bool                    IsEOF                           ();
  bool                    IsOverrun                       ();

  XDWORD                  GetBytePosition                 ();
  XDWORD                  GetBitPosition                  ();
  void                    SetPosition                     (XDWORD index, XDWORD bitindex = 0);


  void                    SkipU1                          ();
  XDWORD                  PeekU1                          ();
  XDWORD                  ReadU1                          ();

  void                    SkipU                           (XDWORD n);
  XDWORD                  PeekU                           (XDWORD n);
  XDWORD                  ReadU                           (XDWORD n);

  XDWORD                  ReadUE                          ();
  int                     ReadSE                          ();

  void                    WriteU1                         (XDWORD v);
  void                    WriteU                          (XDWORD n, XDWORD v);

  void                    WriteUE                         (XDWORD v);
  void                    WriteSE                         (int v);

protected:
  XBUFFER*                xbuffer;

  XBYTE*                  p;
  XDWORD                  bits_left;

private:
  void                    Clean                           ()
                          {
                            xbuffer     = NULL;
                            p           = NULL;
                            bits_left   = 0;
                          }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

