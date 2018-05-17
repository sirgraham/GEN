//------------------------------------------------------------------------------------------
//  XDEBUG.CPP
//
//  Debug Funtions
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 05/04/2002
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include "DIOFactory.h"
#include "DIOURL.h"
#include "DIOStreamEnumDevices.h"
#include "DIOStreamDeviceIP.h"
#include "DIOStreamIPLocalEnumDevices.h"
#include "DIOWebScraperPublicIP.h"

#include "XDebug.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------

XDEBUGCTRL* XDebugCtrl = NULL;


//---- CLASS MEMBERS -----------------------------------------------------------------------



/*-------------------------------------------------------------------
//  XDEBUGCTRLTARGET::ResolvedIPTarget
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      09/05/2016 11:43:22
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool XDEBUGCTRLTARGET::ResolvedIPTarget()
{
  if(!diofactory)                return false;
  if(type != XDEBUGCTRLTYPE_NET) return false;

  DIOURL*  URL;
  XSTRING  remoteIP;

  URL = diofactory->CreateURL();
  if(!URL) return false;

  URL->Set(aim);
  URL->ResolveURL(remoteIP);
  diofactory->DeleteURL(URL);

  XSTRING_CREATEOEM(remoteIP, charstr)
  memcpy(IP, charstr, strlen(charstr)+1);
  XSTRING_DELETEOEM(charstr)

  return true;
}





/*-------------------------------------------------------------------
//  XDEBUGCTRL::SetTarget
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      09/05/2016 10:39:16
//
//  @return       bool :
//
//  @param        index :
//  @param        type :
//  @param        aim :
*/
/*-----------------------------------------------------------------*/
bool XDEBUGCTRL::SetTarget(int index, XDEBUGCTRLTYPE type, XCHAR* aim)
{
  if(index >= XDEBUG_MAXNTARGETS) return false;

  if(!aim) return false;

  targets[index].SetType(type);

  if(type == XDEBUGCTRLTYPE_NET)
    {
      XSTRING string;
      XSTRING url;
      int     port;

      string = aim;
      url.AdjustSize(_MAXSTR);
      string.UnFormat(__L("%s:%d"), url.Get(), &port);
      url.AdjustSize();

      targets[index].SetAim(url.Get());
      targets[index].SetPort(port);
      targets[index].ResolvedIPTarget();

      if(targets[index].GetNETHandle()) CloseHandleNet(&targets[index]);
      GetHandleNet(&targets[index]);

      if(!localIP)    ObtainLocalIP();
      if(!publicIP)   ObtainPublicIP();
    }
   else
    {
      targets[index].SetAim(aim);
    }

  return true;
}



/*-------------------------------------------------------------------
//  XDEBUGCTRL::AddTarget
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      09/05/2016 10:39:49
//
//  @return       bool :
//
//  @param        type :
//  @param        aim :
*/
/*-----------------------------------------------------------------*/
bool XDEBUGCTRL::AddTarget(XDEBUGCTRLTYPE type, XCHAR* aim)
{
  for(int c=0; c<XDEBUG_MAXNTARGETS; c++)
    {
      if(targets[c].GetType() == XDEBUGCTRLTYPE_NONE)  return SetTarget(c, type, aim);
    }

  return false;
}





//-------------------------------------------------------------------
//  XDEBUGCTRL::PrintHeader
/**
//
//
//  @author       Abraham J. Velez
//  @version      10/01/2001 17:04:55
//
//  @return       void :
//  @param        *header :
*/
//-------------------------------------------------------------------
void XDEBUGCTRL::PrintHeader(XCHAR* header)
{
  XSTRING line;

  line.Empty();

  line  = __L("---");

  if(header)
    {
      line += __L(" ");
      line += header;
      line += __L(" ");
    }

  while(line.GetSize()<64)
    {
      line += __L("-");
    }

  Print(0,line.Get());

  openheader = header?true:false;
}




/*-------------------------------------------------------------------
//  XDEBUGCTRL::Print
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      22/08/2012 11:54:07
//
//  @return       bool :
//  @param        level :
//  @param        mask :
//  @param        ... :
*/
/*-----------------------------------------------------------------*/
bool XDEBUGCTRL::Print(XBYTE level, XCHAR* mask,...)
{
  XSTRING string;
  XSTRING outstring;

  string.Add(applicationname);
  if(string.GetSize())
    {
      string.AdjustSize(XDEBUG_MAXAPPLICATIONNAMESIZE, false, __L(" "));
      outstring += string;
    }

  string.Format(__L("%d.%d.%d"), applicationversion, applicationsubversion, applicationsubversionerr);
  if(string.GetSize())
    {
      string.AdjustSize(XDEBUG_MAXAPPLICATIONVERSIONSIZE, false, __L(" "));
      outstring += string;
    }

  string = applicationID;
  if(string.GetSize())
    {
      string.AdjustSize(XDEBUG_MAXAPPLICATIONIDSIZE, false, __L(" "));
      outstring += string;
    }

  va_list arg;

  va_start(arg, mask);

  string.FormatArg(mask, &arg);

  va_end(arg);

  sequence++;

  outstring += string;

  for(int c=0; c<XDEBUG_MAXNTARGETS ; c++)
    {
      switch(targets[c].GetType())
        {
          case XDEBUGCTRLTYPE_SPECIAL   : PrintSpecial(&targets[c], level, outstring.Get());  break;
          case XDEBUGCTRLTYPE_FILE      : PrintFile(&targets[c], level, outstring.Get());     break;
          case XDEBUGCTRLTYPE_NET       : PrintNet(&targets[c], level, outstring.Get());      break;
                          default       : break;
        }
    }

  return true;
}





/*-------------------------------------------------------------------
//  XDEBUGCTRL::PrintDataBlock
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      26/09/2012 10:47:23
//
//  @return       bool :
//  @param        level :
//  @param        data :
//  @param        size :
//  @param        marginsize :
//  @param        sizeline :
//  @param        showoffset :
//  @param        showtext :
*/
/*-----------------------------------------------------------------*/
bool XDEBUGCTRL::PrintDataBlock(XBYTE level, XBYTE* data, XDWORD size, XDWORD marginsize, XDWORD sizeline, bool showoffset, bool showtext)
{
  XSTRING    margin;
  XSTRING    strdata;
  XDWORD     _size     = 0;
  XDWORD     _sizeline = sizeline;
  int        index     = 0;

  for(XDWORD c=0; c<marginsize; c++)
    {
      margin += __L(" ");
    }

  while(_size<size)
    {
      XSTRING string;

      string   = margin;
      if(_sizeline > (size - _size))
         _sizeline = (size - _size);

      if(showoffset)
        {
          strdata.Format(__L("%04X"),index);
          string += strdata;
          string += __L("   ");
        }

      for(XDWORD c=0; c<_sizeline; c++)
        {
          strdata.Format(__L("%02X "),data[index]);
          string += strdata;
          _size++;
          index++;
        }

      //if(_sizeline != sizeline)
        {
          for(XDWORD c=_sizeline; c<sizeline; c++)
            {
              string += __L("   ");
            }
        }


      if(showtext)
        {
          index -= _sizeline;
          string += __L(" ");

          for(XDWORD c=0; c<_sizeline; c++)
            {
              XCHAR character = (XCHAR)data[index];

              if((character<0x20) || (character>0x80) || (character==__C('%')) || (character==__C('\\'))) character = __C('.');

              strdata.Format(__L("%c"),character);
              string += strdata;
              index++;
            }
        }

      Print(level, string.Get());

      if(_size>XDEBUG_MAXSIZEDATABLOCK) break;
    }

  return true;
}



/*-------------------------------------------------------------------
//  XDEBUGCTRL::PrintDataBlock
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/11/2012 1:28:12
//
//  @return       bool :
//  @param        level :
//  @param        data :
//  @param        marginsize :
//  @param        sizeline :
//  @param        showoffset :
//  @param        showtext :
*/
/*-----------------------------------------------------------------*/
bool XDEBUGCTRL::PrintDataBlock(XBYTE level, XBUFFER& data, XDWORD marginsize, XDWORD sizeline, bool showoffset, bool showtext)
{
  return PrintDataBlock(level,data.Get(), data.GetSize(), marginsize, sizeline, showoffset, showtext);
}



/*-------------------------------------------------------------------
//  XDEBUGCTRL::GetDebugFromXBuffer
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      03/01/2017 21:49:44
//
//  @return       bool :
//
//  @param        xbufferpacket :
//  @param        publicIP :
//  @param        localIP :
//  @param        level :
//  @param        sequence :
//  @param        xtime :
//  @param        string :
*/
/*-----------------------------------------------------------------*/
XDWORD XDEBUGCTRL::GetDebugFromXBuffer(XBUFFER& xbufferpacket, XDWORD& publicIP, XDWORD& localIP, XBYTE& level, XDWORD& sequence, XDATETIME* xtime, XSTRING& string)
{
  if(!xtime)                   return 1;
  if(!xbufferpacket.GetSize()) return 1;

  XDWORD    IDpacket        = 0;
  XDWORD    sizepacketread  = 0;
  XDWORD    dword;
  XDWORD    sizeinbuffer    = xbufferpacket.GetSize();
  XBUFFER   xbuffer;
  XDWORD    sizeheader      = (sizeof(XDWORD)    +    // ID.
                               sizeof(XDWORD)    +    // SizePacket.
                               sizeof(XDWORD)    +    // public IP address origin.
                               sizeof(XDWORD)    +    // local  IP address origin.
                               sizeof(XBYTE)     +    // Level.
                               sizeof(XDWORD)    +    // Secuence.
                              (sizeof(XDWORD)*7) +    // Date Time.
                               sizeof(XWORD));        // CRC16

  if(!sizeinbuffer)             return 1;
  if(sizeinbuffer < sizeheader) return 1;


  while(sizeinbuffer-4)
    {
      xbufferpacket.Get(IDpacket, 0);
      if(IDpacket == XDEBUG_IDPACKET) break;

      xbufferpacket.Extract(NULL, 0, 1);
      sizeinbuffer--;
    }

  if(IDpacket != XDEBUG_IDPACKET) return 1;

  if(!sizeinbuffer)               return 1;
  if(sizeinbuffer < sizeheader)   return 1;

  if(!xbufferpacket.Get(sizepacketread, sizeof(IDpacket)))
    {
      return 1;
    }

  if(!sizepacketread)
    {
      return 2;
    }

  if(sizepacketread > sizeinbuffer)
    {
      return 1;
    }

  //xbuffer.Resize(sizepacketread);

  XDWORD sizeassing    = sizepacketread;
  XDWORD sizeassingmin = 5*1024;

  if(sizeassing < sizeassingmin) sizeassing = sizeassingmin;
  xbuffer.Resize(sizeassing);



  xbufferpacket.Extract(xbuffer.Get(), 0, sizepacketread);

  XWORD crc16;
  XDEBUG_CRC16(xbuffer.Get(), (xbuffer.GetSize()-sizeof(XWORD)), crc16)

  XWORD crc16read;

  xbuffer.Get(crc16read, xbuffer.GetSize()-sizeof(XWORD));
  if(crc16 != crc16read)
    {
      return 2;
    }

  xbuffer.SetPosition(0);

  xbuffer.Get(IDpacket);
  xbuffer.Get(sizepacketread);
  xbuffer.Get(publicIP);
  xbuffer.Get(localIP);
  xbuffer.Get(level);
  xbuffer.Get(sequence);

  xbuffer.Get(dword); xtime->SetYear(dword);
  xbuffer.Get(dword); xtime->SetMonth(dword);
  xbuffer.Get(dword); xtime->SetDay(dword);
  xbuffer.Get(dword); xtime->SetHours(dword);
  xbuffer.Get(dword); xtime->SetMinutes(dword);
  xbuffer.Get(dword); xtime->SetSeconds(dword);
  xbuffer.Get(dword); xtime->SetMilliSeconds(dword);

  XDWORD sizestr = sizepacketread - sizeheader;        // CRC16
  sizestr /= sizeof(XWORD);

  for(XDWORD c=0; c<(sizestr-1); c++)
    {
      XWORD  word;
      if(!xbuffer.Get(word)) break;

      string.Add((XCHAR)word);
    }

  return 0;
}




/*-------------------------------------------------------------------
//  XDEBUGCTRL::GetDebugFromStreamIO
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      23/01/2017 20:45:51
//
//  @return       XDWORD :
//
//  @param        streamIO :
//  @param        publicIP :
//  @param        localIP :
//  @param        level :
//  @param        sequence :
//  @param        xtime :
//  @param        string :
*/
/*-----------------------------------------------------------------*/
XDWORD XDEBUGCTRL::GetDebugFromStreamIO(DIOSTREAM* streamIO, XDWORD& publicIP, XDWORD& localIP, XBYTE& level, XDWORD& sequence, XDATETIME* xtime, XSTRING& string)
{
  if(!streamIO) return false;
  if(!xtime)    return false;

  return GetDebugFromXBuffer((*streamIO->GetInXBuffer()), publicIP, localIP, level, sequence, xtime, string);
}






/*-------------------------------------------------------------------
//  XDEBUGCTRL::SetDebugToXBuffer
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      03/01/2017 21:50:18
//
//  @return       bool :
//
//  @param        publicIP :
//  @param        localIP :
//  @param        level :
//  @param        sequence :
//  @param        xtime :
//  @param        string :
//  @param        xbufferpacket :
*/
/*-----------------------------------------------------------------*/
bool XDEBUGCTRL::SetDebugToXBuffer(XDWORD publicIP, XDWORD localIP, XBYTE level, XDWORD sequence, XDATETIME* xtime, XCHAR* string, XBUFFER& xbufferpacket)
{
  if(!xtime)  return false;
  if(!string) return false;

  XSTRING  line;
  XDWORD   IDpacket;
  XDWORD   sizepacket;

  xbufferpacket.Empty();

  line        = string;
  IDpacket    = XDEBUG_IDPACKET;
  sizepacket  = sizeof(XDWORD) +                        // ID.
                sizeof(XDWORD) +                        // SizePacket.
                sizeof(XDWORD) +                        // public IP address origin.
                sizeof(XDWORD) +                        // local  IP address origin.
                sizeof(XBYTE)  +                        // Level.
                sizeof(XDWORD) +                        // Secuence.
                (sizeof(XDWORD)*7) +                    // Date Time.
                ((line.GetSize()+1)*sizeof(XWORD)) +    // Line (WORD) each character.
                sizeof(XWORD);                          // CRC16

  xbufferpacket.Resize(sizepacket);

  xbufferpacket.Set(IDpacket);
  xbufferpacket.Set(sizepacket);
  xbufferpacket.Set(publicIP);
  xbufferpacket.Set(localIP);
  xbufferpacket.Set(level);
  xbufferpacket.Set(sequence);

  xbufferpacket.Set((XDWORD)xtime->GetYear());
  xbufferpacket.Set((XDWORD)xtime->GetMonth());
  xbufferpacket.Set((XDWORD)xtime->GetDay());
  xbufferpacket.Set((XDWORD)xtime->GetHours());
  xbufferpacket.Set((XDWORD)xtime->GetMinutes());
  xbufferpacket.Set((XDWORD)xtime->GetSeconds());
  xbufferpacket.Set((XDWORD)xtime->GetMilliSeconds());

  XSTRING_CREATENORMALIZE(line, charstr, false)

  for(int c=0; c<(int)line.GetSize()+1; c++)
    {
      xbufferpacket.Set((XWORD)charstr[c]);
    }

  XSTRING_DELETENORMALIZE(charstr)

  XWORD crc16;
  XDEBUG_CRC16(xbufferpacket.Get(), (xbufferpacket.GetSize()-sizeof(XWORD)), crc16)

  xbufferpacket.Set((XWORD)crc16);

  return true;
}




/*-------------------------------------------------------------------
//  XDEBUGCTRL::ReConnectedAllNetTargets
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/01/2017 22:35:54
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool XDEBUGCTRL::ReConnectedAllNetTargets()
{
  for(int c=0; c<XDEBUG_MAXNTARGETS; c++)
    {
      if(targets[c].GetType() == XDEBUGCTRLTYPE_NET)
        {
          if(targets[c].GetNETHandle()) CloseHandleNet(&targets[c]);

          GetHandleNet(&targets[c]);
        }
    }

  return true;
}





/*-------------------------------------------------------------------
//  XDEBUGCTRL::ResolvedAllNetTargets
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      09/05/2016 12:06:08
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool XDEBUGCTRL::ResolvedAllNetTargets(bool& changed)
{
  changed = false;

  for(int c=0; c<XDEBUG_MAXNTARGETS; c++)
    {
      if(targets[c].GetType() == XDEBUGCTRLTYPE_NET)
        {
          XSTRING IP;

          IP = targets[c].GetIP();

          targets[c].ResolvedIPTarget();

          if(IP.Compare(targets[c].GetIP())) changed = true;
        }
    }

  return true;
}



/*-------------------------------------------------------------------
//  XDEBUGCTRL::ObtainLocalIP
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      14/01/2017 21:09:37
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool XDEBUGCTRL::ObtainLocalIP()
{
  bool status = false;

  #ifndef MICROCONTROLLER

  DIOSTREAMIPLOCALENUMDEVICES* enumdevices = (DIOSTREAMIPLOCALENUMDEVICES*)diofactory->CreateStreamEnumDevices(DIOSTREAMENUMTYPE_IP_LOCAL);
  if(enumdevices)
    {
      DIOSTREAMDEVICEIP* device = (DIOSTREAMDEVICEIP*)enumdevices->GetFirstActiveDevice();
      if(device)
        {
          XSTRING string;

          device->GetIP()->GetXString(string);
          SetLocalIPString(&string);

          status = true;
        }

      diofactory->DeleteStreamEnumDevices(enumdevices);
    }

  #endif

  return status;
}



/*-------------------------------------------------------------------
//  XDEBUGCTRL::ObtainPublicIP
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      14/01/2017 21:27:29
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool XDEBUGCTRL::ObtainPublicIP()
{
  bool  status = false;

  #ifndef MICROCONTROLLER

  DIOIP ip;
  DIOWEBSCRAPERPUBLICIP* publicip = new DIOWEBSCRAPERPUBLICIP();
  if(!publicip) return false;

  publicip->GetURLDownload()->Set(webscraperscripturldownload);

  if(publicip->Get(ip, 5))
    {
      XSTRING string;

      ip.GetXString(string);
      SetPublicIPString(&string);

    }

  delete publicip;

  #endif

  return status;
}



/*-------------------------------------------------------------------
//  XDEBUGCTRL::GenerateTab
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      12/01/2013 15:04:22
//
//  @return       bool :
//  @param        ntab :
//  @param        tab :
*/
/*-----------------------------------------------------------------*/
bool XDEBUGCTRL::GenerateTab(int ntab,XSTRING& tab)
{
  tab.Empty();

  for(int c=0;c<ntab;c++)
    {
      tab += __L("  ");
    }

  return true;
}






