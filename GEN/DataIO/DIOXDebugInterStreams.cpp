
/*------------------------------------------------------------------------------------------
//  DIOXDEBUGINTERSTREAMS.CPP
//
//  DIO XDebug UART to Net
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 16/05/2017 17:09:09
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XLog.h"

#include "DIOXDebugInterStreams.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//  DIOXDEBUGINTERSTREAMS::Process
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      16/05/2017 17:18:58
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOXDEBUGINTERSTREAMS::Process()
{
  if(!streamIO) return false;

  #ifdef XDEBUG

  XDWORD    publicIP;
  XDWORD    localIP;
  XBYTE     level;
  XDWORD    sequence;
  XDATETIME xtime;
  XSTRING   string;


  if(!XDEBUG_GETDEBUGFROMSTREAMIO(streamIO, publicIP, localIP, level, sequence, &xtime, string))
    {
      if(iswithlog)
        {
          XLOGLEVEL xloglevel = (XLOGLEVEL)0;

          switch(level & 0x0F)
            {

              case XDEBUG_COLORBLACK  :
              case XDEBUG_COLORBLUE   :
              case XDEBUG_COLORGREEN  :
              case XDEBUG_COLORGRAY   :
                              default : xloglevel = (XLOGLEVEL)XLOGLEVEL_INFO;      break;
              case XDEBUG_COLORPURPLE : xloglevel = (XLOGLEVEL)XLOGLEVEL_WARNING;   break;
              case XDEBUG_COLORRED    : xloglevel = (XLOGLEVEL)XLOGLEVEL_ERROR;     break;
            }

          XLOG::GetInstance().AddEntry(xloglevel, DIOXDEBUGINTERSTREAMS_LOGSECTIONID, false, string.Get());
        }
       else
        {
          if(XDebugCtrl)
            {
              XDebugCtrl->Print(level, string.Get());
            }
        }
    }

  #endif

  return true;
}
