//------------------------------------------------------------------------------------------
//  DIOWINDOWSSTREAMSPI.CPP
//
//  Windows Data IO Stream SPI class
//
//
//  @author  Abraham J. Velez
//  @version 12/3/2003
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include <intrin.h>

#include "DIOWINDOWSStreamSPI.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------



/*-------------------------------------------------------------------
//  DIOWINDOWSSTREAMSPI::DIOWINDOWSSTREAMSPI
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      18/02/2013 7:52:28
//
//  @return



//  @param        cipher :
*/
/*-----------------------------------------------------------------*/
DIOWINDOWSSTREAMSPI::DIOWINDOWSSTREAMSPI() : DIOSTREAMSPI()
{
  Clean();
}


//-------------------------------------------------------------------
//  DIOWINDOWSSTREAMSPI::~DIOWINDOWSSTREAMSPI
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/11/2003 10:19:33
//
//  @return
//  */
//-------------------------------------------------------------------
DIOWINDOWSSTREAMSPI::~DIOWINDOWSSTREAMSPI()
{
  Clean();
}



/*-------------------------------------------------------------------
//  DIOWINDOWSSTREAMSPI::Sleep
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/10/2012 12:50:33
//
//  @return       void :
//  @param        count :
*/
/*-----------------------------------------------------------------*/
void DIOWINDOWSSTREAMSPI::Sleep(int count)
{
  for(int i=0;i<count;i++)
    {
      __nop();
    }
}


//-------------------------------------------------------------------
//  DIOWINDOWSSTREAMSPI::Clean
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/11/2003 10:19:50
//
//  @return       void :
//  */
//-------------------------------------------------------------------
void DIOWINDOWSSTREAMSPI::Clean()
{

}

