/**-------------------------------------------------------------------------------------------------------------------
*
* @file        XConsole.h
*
* @class       XCONSOLE
* @brief       Console base class 
* @ingroup     UTILS
*
* @author      Abraham J. Velez 
* @date        28/05/2018 20:49:25
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

#ifndef _XCONSOLE_H_
#define _XCONSOLE_H_

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XString.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define XCONSOLE_MAXSIZEDATABLOCK             10240

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XFACTORY;

class XCONSOLE
{
  public:

                                XCONSOLE                      ();
    virtual                    ~XCONSOLE                      ();

    virtual bool                GetSize                       (int& width, int& height);
    virtual bool                SetSize                       (int width, int height);

    virtual bool                Maximize                      ();
    virtual bool                Minimize                      ();

    virtual bool                Hide                          ();
    virtual bool                IsHide                        ();
    virtual bool                UnHide                        ();


    virtual bool                Print                         (XCHAR* string);
    bool                        Printf                        (XCHAR* mask,...);

    bool                        PrintDataBlock                (XBYTE* data, XDWORD _size, XDWORD marginsize = 1, XDWORD sizeline = 16, bool showoffset = true, bool showtext = true);
    bool                        PrintDataBlock                (XBUFFER& data, XDWORD marginsize = 1, XDWORD sizeline = 16, bool showoffset = true, bool showtext = true);

    virtual bool                Clear                         ();

    virtual bool                KBHit                         ();
    virtual int                 GetChar                       ();

    bool                        FormatMessage                 (XCHAR* message, XDWORD margin, bool prelude, bool returnline, XSTRING& string);
    bool                        PrintMessage                  (XCHAR* message, XDWORD margin, bool prelude, bool returnline);
    bool                        TipicalHeader_Create          (int yearorigin, XCHAR* nameapp, int version, int subversion, int subversionerr, XCHAR* enterprise, XSTRING& header);
    bool                        TipicalHeader_Show            (int yearorigin, XCHAR* nameapp, int version, int subversion, int subversionerr, XCHAR* enterprise);
    bool                        WaitKey                       (XCHAR* text, XDWORD margin, bool prelude, XDWORD timeout);

  private:

    void                        Clean                         ();
};



/*---- INLINE FUNCTIONS ----------------------------------------------------------------------------------------------*/

#endif


