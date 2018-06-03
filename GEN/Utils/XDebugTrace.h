/**-------------------------------------------------------------------------------------------------------------------
*
* @file        XDebugTrace.h
*
* @class       XDEBUGTRACE
* @brief       XDebug Trace class
* @ingroup     UTILS
*
* @author      Abraham J. Velez 
* @date        30/05/2018 15:56:10
*
* @copyright   Copyright(c) 2005 - 2018 GEN Group.
*
* @cond
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
* documentation files(the "Software"), to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense, and/ or sell copies of the Software,
* and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of
* the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
* THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
* @endcond
*
*---------------------------------------------------------------------------------------------------------------------*/

#ifndef _XDEBUGTRACE_H_
#define _XDEBUGTRACE_H_

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "XBase.h"
#include "XDateTime.h"
#include "XString.h"
#include "XBuffer.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

class XDEBUGTRACE;

enum XDEBUGTRACE_TYPE
{
   XDEBUGTRACE_TYPE_NONE          = 0 ,
   XDEBUGTRACE_TYPE_SPECIAL           ,
   XDEBUGTRACE_TYPE_FILE              ,
   XDEBUGTRACE_TYPE_NET               ,
};


#define XDEBUGTRACE_LEVEL_WITHCOLOR              0x80
#define XDEBUGTRACE_LEVEL_WITHTAB                0x40
#define XDEBUGTRACE_LEVEL_WITHCODE               0x20

#define XDEBUGTRACE_LEVEL_COLOR(level)           (level|XDEBUGTRACE_LEVEL_WITHCOLOR)
#define XDEBUGTRACE_LEVEL_TAB(level)             (level|XDEBUGTRACE_LEVEL_WITHTAB)
#define XDEBUGTRACE_LEVEL_CODE(level)            (level|XDEBUGTRACE_LEVEL_WITHCODE)

#ifdef MICROCONTROLLER
  #define XDEBUGTRACE_SIZELIMITDEFAULT           512       // Bytes
  #define XDEBUGTRACE_MAXNTARGETS                1
  #define XDEBUGTRACE_MAXSIZEDATABLOCK           512
  #define XDEBUGTRACE_MAXNETBUFFER               512
#else
  #define XDEBUGTRACE_SIZELIMITDEFAULT           64*1024   // Bytes
  #define XDEBUGTRACE_MAXNTARGETS                5
  #define XDEBUGTRACE_MAXSIZEDATABLOCK           10240
  #define XDEBUGTRACE_MAXNETBUFFER               512
#endif

#define XDEBUGTRACE_DEFAULT_SPECIALAIM           __L("Debug Monitor")

#define XDEBUGTRACE_DEFAULT_NETAIM1              __L("xdebug1.kicks-ass.org:10021")
#define XDEBUGTRACE_DEFAULT_NETAIM2              __L("xdebug2.kicks-ass.org:10022")
#define XDEBUGTRACE_DEFAULT_NETAIM3              __L("xdebug3.kicks-ass.org:10023")
#define XDEBUGTRACE_DEFAULT_NETAIM4              __L("xdebug4.kicks-ass.org:10024")
#define XDEBUGTRACE_DEFAULT_NETAIM5              __L("xdebug5.kicks-ass.org:10025")

#define XDEBUGTRACE_LIMITCTRLNETRESURCES         25

#define XDEBUGTRACE_MAXAPPLICATIONNAMESIZE       20
#define XDEBUGTRACE_MAXAPPLICATIONVERSIONSIZE    10
#define XDEBUGTRACE_MAXAPPLICATIONIDSIZE         36

#define XDEBUGTRACE_IDPACKET                     0xAADEB055
#define XDEBUGTRACE_MAXCACHEREMOTEUDPIP          100

#define XDEBUGTRACE_COLOR_BLACK                  0
#define XDEBUGTRACE_COLOR_BLUE                   1
#define XDEBUGTRACE_COLOR_GREEN                  2
#define XDEBUGTRACE_COLOR_PURPLE                 3
#define XDEBUGTRACE_COLOR_RED                    4
#define XDEBUGTRACE_COLOR_GRAY                   5

#define XDEBUGTRACE_COLOR_INFO                   XDEBUGTRACE_COLOR_BLACK
#define XDEBUGTRACE_COLOR_WARNING                XDEBUGTRACE_COLOR_PURPLE
#define XDEBUGTRACE_COLOR_ERROR                  XDEBUGTRACE_COLOR_RED



#define XDEBUGTRACE_CRC16(ptrbuffer, buffersize, crc) { XBYTE*   buffer = ptrbuffer;                                                   \
                                                        XDWORD   size   = buffersize;                                                  \
                                                        XBYTE    x;                                                                    \
                                                        crc = 0xFFFF;                                                                  \
                                                        while(size--)                                                                  \
                                                          {                                                                            \
                                                            x = crc >> 8 ^ * buffer++;                                                 \
                                                            x ^= x>>4;                                                                 \
                                                            crc = (crc << 8) ^ ((XWORD)(x << 12)) ^ ((XWORD)(x <<5)) ^ ((XWORD)x);     \
                                                          }                                                                            \
                                                      }


#ifdef XDEBUG_TRACE

  //extern XDEBUGTRACE*  XDebugTrace;

  #define XDEBUGTRACE_SETTARGET(index, type , aim)                                                                  XDEBUGTRACE::instance->SetTarget(index, type , aim)
  #define XDEBUGTRACE_ADDTARGET(type , aim)                                                                         XDEBUGTRACE::instance->AddTarget(type, aim)
  #define XDEBUGTRACE_SETSIZELIMIT(sizelimit)                                                                       XDEBUGTRACE::instance->SetSizeLimit(sizelimit)
  #define XDEBUGTRACE_SETAPPLICATIONNAME(name)                                                                      XDEBUGTRACE::instance->SetApplicationName(name)
  #define XDEBUGTRACE_SETAPPLICATIONVERSION(version, subversion, subversionerr)                                     XDEBUGTRACE::instance->SetApplicationVersion(version, subversion, subversionerr)
  #define XDEBUGTRACE_SETAPPLICATIONID(ID)                                                                          XDEBUGTRACE::instance->SetApplicationID(ID)
  #define XDEBUGTRACE_SETWEBSCRAPERSCRIPURLDOWNLOAD(URL)                                                            XDEBUGTRACE::instance->SetWebScraperScriptURLDownload(URL);
  #define XDEBUGTRACE_SETPUBLICIP(IP)                                                                               XDEBUGTRACE::instance->SetPublicIPString(IP)
  #define XDEBUGTRACE_SETLOCALIP(IP)                                                                                XDEBUGTRACE::instance->SetLocalIPString(IP)
  #define XDEBUGTRACE_RESOLVEDALLRESOURCES                                                                          XDEBUGTRACE::instance->ResolvedAllResources()
  #define XDEBUGTRACE_PRINTHEADER(header)                                                                           XDEBUGTRACE::instance->PrintHeader(header)

  #ifdef BUILDER

    #define XDEBUGTRACE_PRINT
    #define XDEBUGTRACE_PRINTLEVEL
    #define XDEBUGTRACE_PRINTCOLOR
    #define XDEBUGTRACE_PRINTTAB
    #define XDEBUGTRACE_PRINTCODE

    #define XDEBUGTRACE_PRINTDATABLOCK
    #define XDEBUGTRACE_PRINTDATABLOCKLEVEL
    #define XDEBUGTRACE_PRINTDATABLOCKCOLOR
    #define XDEBUGTRACE_PRINTDATABLOCKTAB
    #define XDEBUGTRACE_PRINTDATABLOCKCODE

  #else

    #define XDEBUGTRACE_PRINT(mask, ...)                                                                            XDEBUGTRACE::instance->Print(0                                             , mask , ## __VA_ARGS__)
    #define XDEBUGTRACE_PRINTLEVEL(level, mask, ...)                                                                XDEBUGTRACE::instance->Print(level                                         , mask , ## __VA_ARGS__)
    #define XDEBUGTRACE_PRINTCOLOR(level, mask, ...)                                                                XDEBUGTRACE::instance->Print((level|XDEBUGTRACE_LEVEL_WITHCOLOR)           , mask , ## __VA_ARGS__)
    #define XDEBUGTRACE_PRINTTAB(level, mask, ...)                                                                  XDEBUGTRACE::instance->Print((level|XDEBUGTRACE_LEVEL_WITHTAB)             , mask , ## __VA_ARGS__)
    #define XDEBUGTRACE_PRINTCODE(level, mask, ...)                                                                 XDEBUGTRACE::instance->Print((level|XDEBUGTRACE_LEVEL_WITHCODE)            , mask , ## __VA_ARGS__)

    #define XDEBUGTRACE_PRINTDATABLOCK(level, data, ...)                                                            XDEBUGTRACE::instance->PrintDataBlock(0                                    , data, ## __VA_ARGS__)
    #define XDEBUGTRACE_PRINTDATABLOCKLEVEL(level, data, ...)                                                       XDEBUGTRACE::instance->PrintDataBlock(level                                , data, ## __VA_ARGS__)
    #define XDEBUGTRACE_PRINTDATABLOCKCOLOR(level, data, ...)                                                       XDEBUGTRACE::instance->PrintDataBlock((level|XDEBUGTRACE_LEVEL_WITHCOLOR)  , data, ## __VA_ARGS__)
    #define XDEBUGTRACE_PRINTDATABLOCKTAB(level, data, ...)                                                         XDEBUGTRACE::instance->PrintDataBlock((level|XDEBUGTRACE_LEVEL_WITHTAB)    , data, ## __VA_ARGS__)
    #define XDEBUGTRACE_PRINTDATABLOCKCODE(level, data, ...)                                                        XDEBUGTRACE::instance->PrintDataBlock((level|XDEBUGTRACE_LEVEL_WITHCODE)   , data, ## __VA_ARGS__)

  #endif

  #define XDEBUGTRACE_GETDEBUGFROMXBUFFER(xbufferpacket, publicIP, localIP, level, sequence, xtime, string)         XDEBUGTRACE::instance->GetDebugFromXBuffer(xbufferpacket, publicIP, localIP, level, sequence, xtime, string)
  #define XDEBUGTRACE_GETDEBUGFROMSTREAMIO(streamIO, publicIP, localIP, level, sequence, xtime, string)             XDEBUGTRACE::instance->GetDebugFromStreamIO(streamIO, publicIP, localIP, level, sequence, xtime, string)
  #define XDEBUGTRACE_SETDEBUGTOXBUFFER(publicIP, localIP, level, sequence, xtime, string, xbufferpacket)           XDEBUGTRACE::instance->SetDebugToXBuffer(publicIP, localIP, level, sequence, xtime, string, xbufferpacket)

#else

  #define XDEBUGTRACE_SETTARGET(index, type , aim)
  #define XDEBUGTRACE_ADDTARGET(type, aim)
  #define XDEBUGTRACE_SETSIZELIMIT(sizelimit)
  #define XDEBUGTRACE_SETAPPLICATIONNAME(name)
  #define XDEBUGTRACE_SETAPPLICATIONVERSION(version, subversion, subversionerr)
  #define XDEBUGTRACE_SETAPPLICATIONID(ID)
  #define XDEBUGTRACE_SETWEBSCRAPERSCRIPURLDOWNLOAD(URL)
  #define XDEBUGTRACE_SETPUBLICIP(IP)
  #define XDEBUGTRACE_SETLOCALIP(IP)
  #define XDEBUGTRACE_RESOLVEDALLRESOURCES
  #define XDEBUGTRACE_PRINTHEADER(header)

  #ifdef BUILDER

    #define XDEBUGTRACE_PRINT
    #define XDEBUGTRACE_PRINTLEVEL
    #define XDEBUGTRACE_PRINTCOLOR
    #define XDEBUGTRACE_PRINTTAB
    #define XDEBUGTRACE_PRINTCODE

    #define XDEBUGTRACE_PRINTDATABLOCK
    #define XDEBUGTRACE_PRINTDATABLOCKLEVEL
    #define XDEBUGTRACE_PRINTDATABLOCKCOLOR
    #define XDEBUGTRACE_PRINTDATABLOCKTAB
    #define XDEBUGTRACE_PRINTDATABLOCKCODE

  #else

    #define XDEBUGTRACE_PRINT(level, mask, ...)
    #define XDEBUGTRACE_PRINTLEVEL(level, mask, ...)
    #define XDEBUGTRACE_PRINTCOLOR(level, mask, ...) {}
    #define XDEBUGTRACE_PRINTTAB(level, mask, ...)
    #define XDEBUGTRACE_PRINTCODE(level, mask, ...)

    #define XDEBUGTRACE_PRINTDATABLOCK(level, data, ...)
    #define XDEBUGTRACE_PRINTDATABLOCKLEVEL(level, data, ...)
    #define XDEBUGTRACE_PRINTDATABLOCKCOLOR(level, data, ...)
    #define XDEBUGTRACE_PRINTDATABLOCKTAB(level, data, ...)
    #define XDEBUGTRACE_PRINTDATABLOCKCODE(level, data, ...)

  #endif

  #define XDEBUGTRACE_GETDEBUGFROMXBUFFER(xbufferpacket, publicIP, localIP, level, sequence, xtime, string)         XDEBUGTRACE::instance->GetDebugFromXBuffer(xbufferpacket, publicIP, localIP, level, sequence, xtime, string)
  #define XDEBUGTRACE_GETDEBUGFROMSTREAMIO(streamIO, publicIP, localIP, level, sequence, xtime, string)             XDEBUGTRACE::instance->GetDebugFromStreamIO(streamIO, publicIP, localIP, level, sequence, xtime, string)
  #define XDEBUGTRACE_SETDEBUGTOXBUFFER(publicIP, localIP, level, sequence, xtime, string, xbufferpacket)           XDEBUGTRACE::instance->SetDebugToXBuffer(publicIP, localIP, level, sequence, xtime, string, xbufferpacket)


#endif

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XFACTORY;
class XMUTEX;
class DIOSTREAM;


class XDEBUGTRACE_TARGET
{
  public:
                            XDEBUGTRACE_TARGET                ();                           
    virtual                ~XDEBUGTRACE_TARGET                ();

    XDEBUGTRACE_TYPE        GetType                           ();
    void                    SetType                           (XDEBUGTRACE_TYPE type);

    XCHAR*                  GetAim                            ();
    bool                    GetAim                            (XSTRING& aim);
                           
    void                    SetAim                            (XSTRING& aim);
    void                    SetAim                            (XCHAR* aim);
                            

    XWORD                   GetPort                           ();
    void                    SetPort                           (XWORD port);

    bool                    ResolvedIPTarget                  ();
                                                    
    char*                   GetIP                             (); 

    XQWORD                  GetNETHandle                      (); 
    void                    SetNETHandle                      (XQWORD NEThandle); 
    
    XDWORD                  GetNSendings                      ();                                                               
    void                    AddNSendings                      ();               


  private:

    void                    Clean                             ();
                          

    XDEBUGTRACE_TYPE        type;
    XCHAR                   aim[_MAXSTR];
    XWORD                   port;
    char                    IP[_MAXSTR];
    XDWORD                  nsendings;
    XQWORD                  NEThandle;
};



class XDEBUGTRACE
{
  public: 
                            XDEBUGTRACE                       ();
    virtual                ~XDEBUGTRACE                       ();
    
    bool                    SetTarget                         (int index, XDEBUGTRACE_TYPE type , XCHAR* aim);
    bool                    AddTarget                         (XDEBUGTRACE_TYPE type , XCHAR* aim);

    XDWORD                  GetSizeLimit                      ();
    bool                    SetSizeLimit                      (XDWORD sizelimit = XDEBUGTRACE_SIZELIMITDEFAULT);
                            

    XCHAR*                  GetApplicationName                ();
    void                    SetApplicationName                (XCHAR* applicationname);                            
    void                    SetApplicationName                (XCHAR* applicationname, XDWORD size);                            
    void                    SetApplicationName                (XSTRING& applicationname);
    
    void                    GetApplicationVersion             (int& applicationversion, int& applicationsubversion, int& applicationsubversionerr);
                            
    void                    SetApplicationVersion             (int applicationversion, int applicationsubversion, int applicationsubversionerr);
                            

    XCHAR*                  GetApplicationID                  ();

    void                    SetApplicationID                  (XCHAR* applicationID, XDWORD size);    
    void                    SetApplicationID                  (XSTRING& applicationID);

    XSTRING*                GetPublicIPString                 ();
    bool                    SetPublicIPString                 (XSTRING* publicIPstring);
                            
    XSTRING*                GetLocalIPString                  ();
    bool                    SetLocalIPString                  (XSTRING* localIPstring);
    
    XDWORD                  GetPublicIP                       ();
    XDWORD                  GetLocalIP                        ();


    XCHAR*                  GetWebScraperScriptURLDownload    ();
    void                    SetWebScraperScriptURLDownload    (XCHAR* webscraperscripturldownload);
                           

    void                    PrintHeader                       (XCHAR* header);
    bool                    Print                             (XBYTE level, XCHAR* mask,...);
    bool                    PrintDataBlock                    (XBYTE level, XBYTE* data, XDWORD _size, XDWORD marginsize = 1, XDWORD sizeline = 32, bool showoffset = true, bool showtext = true);
    bool                    PrintDataBlock                    (XBYTE level, XBUFFER& data, XDWORD marginsize = 1, XDWORD sizeline = 32, bool showoffset = true, bool showtext = true);

    virtual void            PrintSpecial                      (XDEBUGTRACE_TARGET* target, XBYTE level, XCHAR* string);
    virtual void            PrintFile                         (XDEBUGTRACE_TARGET* target, XBYTE level, XCHAR* string);
    virtual void            PrintNet                          (XDEBUGTRACE_TARGET* target, XBYTE level, XCHAR* string);

    virtual bool            GetHandleNet                      (XDEBUGTRACE_TARGET* target);
    virtual bool            CloseHandleNet                    (XDEBUGTRACE_TARGET* target);

    XDWORD                  GetDebugFromXBuffer               (XBUFFER& xbufferpacket, XDWORD& publicIP, XDWORD& localIP, XBYTE& level, XDWORD& sequence, XDATETIME* xtime, XSTRING& string);
    XDWORD                  GetDebugFromStreamIO              (DIOSTREAM* streamIO, XDWORD& publicIP, XDWORD& localIP, XBYTE& level, XDWORD& sequence, XDATETIME* xtime, XSTRING& string);
    bool                    SetDebugToXBuffer                 (XDWORD publicIP, XDWORD localIP, XBYTE level, XDWORD sequence, XDATETIME* xtime, XCHAR* string, XBUFFER& xbufferpacket);

    bool                    ResolvedAllNetTargets             (bool& changed);
    bool                    ReConnectedAllNetTargets          ();

    bool                    ObtainLocalIP                     ();
    bool                    ObtainPublicIP                    ();

    bool                    ObtainResourcesIP                 ();
                           

    bool                    ResolvedAllResources              ();
    
    static XDEBUGTRACE*     instance;

  protected:

    bool                    GenerateTab                       (int ntab,XSTRING& tab);

    XDEBUGTRACE_TARGET      targets[XDEBUGTRACE_MAXNTARGETS];

    XDWORD                  sizelimit;

    XCHAR                   applicationname[_MAXSTR];
    int                     applicationversion;
    int                     applicationsubversion;
    int                     applicationsubversionerr;
    XCHAR                   applicationID[_MAXSTR];
    XDWORD                  publicIP;
    XSTRING                 publicIPstring;
    XDWORD                  localIP;
    XSTRING                 localIPstring;
    XCHAR                   webscraperscripturldownload[_MAXSTR];

    bool                    openheader;
    XDWORD                  sequence;

  private:

    void                    Clean                             ();   
};

/*---- INLINE FUNCTIONS ----------------------------------------------------------------------------------------------*/


#endif





