/*------------------------------------------------------------------------------------------
//  DIOWEBPAGEHTMLCREATOR.H
*/
/**
// \class
//
//  DIO Web Page HTML Creator class
//
//  @author  Abraham J. Velez
//  @version 08/01/2014 17:52:51
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _DIOWEBPAGEHTMLCREATOR_H_
#define _DIOWEBPAGEHTMLCREATOR_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "XString.h"
#include "XPath.h"
#include "XEvent.h"
#include "XSubject.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


enum DIOWEBPAGEHTMLCREATORXEVENTTYPE
{
  DIOWEBPAGEHTMLCREATORXEVENTTYPE_UNKNOW                = XEVENTTYPE_WEBSERVER  ,
  DIOWEBPAGEHTMLCREATORXEVENTTYPE_XTAGFOUND                                     ,
};


enum DIOWEBPAGEHTMLCREATORALIGN
{
  DIOWEBPAGEHTMLCREATORALIGN_NONE                       =                     0 ,
  DIOWEBPAGEHTMLCREATORALIGN_LEFT                                               ,
  DIOWEBPAGEHTMLCREATORALIGN_RIGHT                                              ,
  DIOWEBPAGEHTMLCREATORALIGN_CENTER                                             ,
};



enum DIOWEBPAGEHTMLCREATORTAGXENGINESTATUS
{
  DIOWEBPAGEHTMLCREATORTAGXENGINESTATUS_NOTFOUND        =                     0 ,
  DIOWEBPAGEHTMLCREATORTAGXENGINESTATUS_FOUNDNOTEND                             ,
  DIOWEBPAGEHTMLCREATORTAGXENGINESTATUS_FOUNDEND
};


#define DIOWEBPAGEHTMLCREATOR_TAGXENGINE          __L("GEN")




//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class DIOFACTORY;
class XPATHS;
class XPUBLISHER;
class DIOWEBSERVER_CONNEXION;
class DIOWEBPAGEHTMLCREATOR;



class DIOWEBPAGEHTMLCREATORXEVENT : public XEVENT
{
  public:
                                            DIOWEBPAGEHTMLCREATORXEVENT   (XSUBJECT* subject, XDWORD type = DIOWEBPAGEHTMLCREATORXEVENTTYPE_UNKNOW) : XEVENT(subject,type)      {  Clean();                                   }
    virtual                                ~DIOWEBPAGEHTMLCREATORXEVENT   ()                                                                                                    {  Clean();                                   }

    DIOWEBPAGEHTMLCREATOR*                  GetPageHTMLCreator            ()                                                                                                    { return pageHTMLcreator;                     }
    void                                    SetPageHTMLCreator            (DIOWEBPAGEHTMLCREATOR* pageHTMLcreator)                                                              { this->pageHTMLcreator = pageHTMLcreator;    }

    XSTRING*                                GetXTAG                       ()                                                                                                    { return &XTAG;                               }
    void                                    SetXTAG                       (XSTRING* XTAG)                                                                                       { this->XTAG = XTAG->Get();                   }

  private:

    void                                    Clean                         ()
                                            {
                                              pageHTMLcreator = NULL;
                                            }

    DIOWEBPAGEHTMLCREATOR*                  pageHTMLcreator;
    XSTRING                                 XTAG;
};




class DIOWEBPAGEHTMLCREATOR : public XSTRING, public XSUBJECT
{
  public:
                                            DIOWEBPAGEHTMLCREATOR         (DIOWEBSERVER_CONNEXION* connexion = NULL);
    virtual                                ~DIOWEBPAGEHTMLCREATOR         ();

    void                                    operator =                    (const char* htlmpage)              { Set(htlmpage);                            }
    void                                    operator =                    (const XCHAR* htlmpage)             { Set(htlmpage);                            }
    void                                    operator =                    (DIOWEBPAGEHTMLCREATOR& htlmpage)   { Set(htlmpage);                            }
    void                                    operator =                    (XSTRING& string)                   { Set(string);                              }
    void                                    operator =                    (XWORD* htlmpage)                   { Set(htlmpage);                            }

    void                                    operator +=                   (const char* htlmpage)              { Add(htlmpage);                            }
    void                                    operator +=                   (const XCHAR* htlmpage)             { Add(htlmpage);                            }
    void                                    operator +=                   (XPATH& htlmpage)                   { Add(htlmpage);                            }
    void                                    operator +=                   (XSTRING& string)                   { Add(string);                              }
    void                                    operator +=                   (XCHAR htlmpage)                    { Add(htlmpage);                            }

    bool                                    operator <                    (DIOWEBPAGEHTMLCREATOR& htlmpage)   { return (bool)(Compare(htlmpage) == -1);   }
    bool                                    operator >                    (DIOWEBPAGEHTMLCREATOR& htlmpage)   { return (bool)(Compare(htlmpage) ==  1);   }
    bool                                    operator <=                   (DIOWEBPAGEHTMLCREATOR& htlmpage)   { return (bool)(Compare(htlmpage) !=  1);   }
    bool                                    operator >=                   (DIOWEBPAGEHTMLCREATOR& htlmpage)   { return (bool)(Compare(htlmpage) != -1);   }
    bool                                    operator ==                   (DIOWEBPAGEHTMLCREATOR& htlmpage)   { return (bool)(Compare(htlmpage) ==  0);   }
    bool                                    operator !=                   (DIOWEBPAGEHTMLCREATOR& htlmpage)   { return (bool)(Compare(htlmpage) !=  0);   }

    bool                                    operator <                    (XSTRING& string)                   { return (bool)(Compare(string) == -1); }
    bool                                    operator >                    (XSTRING& string)                   { return (bool)(Compare(string) ==  1); }
    bool                                    operator <=                   (XSTRING& string)                   { return (bool)(Compare(string) !=  1); }
    bool                                    operator >=                   (XSTRING& string)                   { return (bool)(Compare(string) != -1); }
    bool                                    operator ==                   (XSTRING& string)                   { return (bool)(Compare(string) ==  0); }
    bool                                    operator !=                   (XSTRING& string)                   { return (bool)(Compare(string) !=  0); }

    XCHAR                                   operator []                   (int position)
                                            {
                                              if(IsEmpty())           return 0;
                                              if(position<0)          return text[0];
                                              if(position>=(int)size) return text[size-1];
                                              return text[position];
                                            }

    bool                                    Printf                        (XCHAR* mask,...);
    bool                                    Print                         (XCHAR* text);

    bool                                    Table_Ini                     (int border = 0);
    bool                                    Table_Line                    (int columns,...);
    bool                                    Table_Line                    (XSTRING& colorbackground, int columns,...);
    bool                                    Table_Line                    (XCHAR* colorbackground, int columns,...);
    bool                                    Table_LineColor               (int columns, ...);
    bool                                    Table_End                     ();

    bool                                    AddAutoRefresh                (int refresh, XSTRING* url = NULL);

    bool                                    AddFile                       (XPATH& xpath);

  private:

    void                                    Clean                         ()
                                            {
                                              tableborder = false;
                                            }

    DIOWEBPAGEHTMLCREATORTAGXENGINESTATUS   CheckXTAG                     (XSTRING* originalline,bool& intag, XSTRING& inioutputline,XSTRING& endoutputline);

    DIOWEBSERVER_CONNEXION*                 connexion;

    XSTRING                                 lastXTAG;
    int                                     tableborder;

};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

