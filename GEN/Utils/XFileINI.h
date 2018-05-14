//------------------------------------------------------------------------------------------
//  XFILEINI.H
//
/**
// \class
//
//  INI File functions (INI format)
//
//  @author  Abraham J. Velez
//  @version 04/06/2004 11:39:38
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _XFILEINI_H_
#define _XFILEINI_H_

//---- INCLUDES ----------------------------------------------------------------------------

#include "XBase.h"
#include "XVector.h"
#include "XFileTXT.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

typedef struct
{
  XSTRING key;
  XSTRING value;

} XFILEINIKEYMAP;

#define XFILEININKEYMAP(map)    (sizeof(map)/sizeof(XFILEINIKEYMAP))

//---- CLASS -------------------------------------------------------------------------------

class XFILEINIKEY
{
  public:
                                XFILEINIKEY             ();
                               ~XFILEINIKEY             ();

    XSTRING*                    GetKey                  ();
    bool                        SetKey                  (XSTRING& key);

    XSTRING*                    GetValue                ();
    bool                        SetValue                (XSTRING& key);

  private:

    void                        Clean                   ();

    XSTRING                     key;
    XSTRING                     value;
};


class XFILEINISECTION
{
  public:
                                XFILEINISECTION         ();
                               ~XFILEINISECTION         ();

    XSTRING*                    GetName                 ();
    bool                        SetName                 (XSTRING& section);
    bool                        IsStrSectionName        (XSTRING& line,XSTRING& section);

    XFILEINIKEY*                FindKey                 (XSTRING& key);

    bool                        GetKey                  (XSTRING& key,XSTRING& value);
    bool                        SetKey                  (XSTRING& key,XSTRING& value);

    bool                        DeleteKey               (XSTRING& key);

    bool                        GetNextKey              (XSTRING& key,XSTRING& value);
    bool                        ResetSelectionKey       ();

    void                        Clean                   ();

  private:

    XSTRING                     section;

    XVECTOR<XFILEINIKEY*>       keys;
    XDWORD                      selectkey;
};


class XFILEINI : public XFILETXT
{
  public:

                                XFILEINI                ();
                                XFILEINI                (XPATH& xpath);
                                XFILEINI                (XFILE* file);
    virtual                    ~XFILEINI                ();

    bool                        Open                    (XPATH& xpath,bool readonly=true);
    bool                        Close                   (void);

    bool                        SelectSection           (XSTRING& section);
    bool                        SelectSection           (XCHAR* section);
    bool                        CreateSection           (XSTRING& section);
    bool                        CreateSection           (XCHAR* section);
    bool                        DeleteSection           (XSTRING& section);
    bool                        DeleteSection           (XCHAR* section);
    bool                        DeleteKey               (XSTRING& section,XSTRING& key);
    bool                        DeleteKey               (XCHAR* section,XSTRING& key);
    bool                        DeleteKey               (XSTRING& section,XCHAR* key);
    bool                        DeleteKey               (XCHAR* section,XCHAR* key);

    bool                        ReadValue               (XSTRING& section,XSTRING& key,XSTRING& value);
    bool                        ReadValue               (XCHAR* section,XSTRING& key,XSTRING& value);
    bool                        ReadValue               (XSTRING& section,XCHAR* key,XSTRING& value);
    bool                        ReadValue               (XCHAR* section,XCHAR* key,XSTRING& value);
    bool                        ReadValue               (XSTRING& key,XSTRING& value);
    bool                        ReadValue               (XCHAR* key,XSTRING& value);

    bool                        WriteValue              (XSTRING& section,XSTRING& key,XSTRING& value);
    bool                        WriteValue              (XCHAR* section,XSTRING& key,XSTRING& value);
    bool                        WriteValue              (XSTRING& section,XCHAR* key,XSTRING& value);
    bool                        WriteValue              (XCHAR* section,XCHAR* key,XSTRING& value);
    bool                        WriteValue              (XSTRING& key,XSTRING& value);
    bool                        WriteValue              (XCHAR* key,XSTRING& value);

    bool                        GetNextKey              (XSTRING& key,XSTRING& value);

    bool                        ReadKeyMap              (XSTRING& section,XFILEINIKEYMAP* keymap,int nkeymap);
    bool                        ReadKeyMap              (XCHAR* section,XFILEINIKEYMAP* keymap,int nkeymap);

    bool                        ConvertFromLines        ();
    bool                        ConvertToLines          ();

    bool                        Serialization           (bool read);

    bool                        DeleteAllSections       ();

  private:

    void                        Clean                   ();

    bool                        LineIsSection           (XSTRING& line,XSTRING& section);
    bool                        LineIsKey               (XSTRING& line,XSTRING& key,XSTRING& value);


    XVECTOR<XFILEINISECTION*>   sections;
    XDWORD                      selectsection;
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------


#endif

