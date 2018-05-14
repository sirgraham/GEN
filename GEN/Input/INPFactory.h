/*------------------------------------------------------------------------------------------
//  INPFACTORY.H
*/
/**
// \class
//
//  input factory
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 05/09/2016 12:16:37
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _INPFACTORY_H_
#define _INPFACTORY_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XString.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

class INPDEVICE;

/*---- CLASS -----------------------------------------------------------------------------*/
class INPFACTORY
{
public:

  virtual            ~INPFACTORY  ()                    {};

  static INPFACTORY*  GetInstance ()                    { return instance; }
  static void         DelInstance ()                    { delete instance;  instance=NULL; }

  template <class T>
  static void         Create      ()                    { if (instance!=NULL) DelInstance(); instance=new T();}

  virtual INPDEVICE* CreateDevice(XCHAR* device,void* v=NULL)=0;

  static INPFACTORY* instance;

private:



};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

