//------------------------------------------------------------------------------------------
//  XFILEJSON.H
//
/**
// \class
//
//  JSON File functions (JSON format)
//
//  @author  Abraham J. Velez
//  @version 04/06/2004 11:39:38
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _XFILEJSON_H_
#define _XFILEJSON_H_

//---- INCLUDES ----------------------------------------------------------------------------

#include "XBase.h"
#include "XVector.h"
#include "XString.h"
#include "XFileTXT.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

enum XFILEJSONVALUETYPE
{
  XFILEJSONVALUETYPE_UNKNOWN          =  0 ,
  XFILEJSONVALUETYPE_NUMBER               ,
  XFILEJSONVALUETYPE_NUMBERSPECIAL        ,
  XFILEJSONVALUETYPE_STRING               ,
  XFILEJSONVALUETYPE_OBJECT               ,
  XFILEJSONVALUETYPE_ARRAY                ,
  XFILEJSONVALUETYPE_BOOLEAN              ,
  XFILEJSONVALUETYPE_NULL                 ,
};


enum XFILEJSONCONTROLCHAR
{
  XFILEJSONCONTROLCHAR_NOTCONTROL         , // All other Not control or number
  XFILEJSONCONTROLCHAR_OPENBRACE          , // {
  XFILEJSONCONTROLCHAR_CLOSEBRACE         , // }
  XFILEJSONCONTROLCHAR_OPENBRACKET        , // [
  XFILEJSONCONTROLCHAR_CLOSEBRACKET       , // ]
  XFILEJSONCONTROLCHAR_QUOTE              , // "
  XFILEJSONCONTROLCHAR_COMMA              , // ,
  XFILEJSONCONTROLCHAR_COLON              , // :
  XFILEJSONCONTROLCHAR_BLACKSLASH         , // (the other slash /)
  XFILEJSONCONTROLCHAR_NUMBER             , // '012345789' '-'
  XFILEJSONCONTROLCHAR_NUMBERSPECIAL      , //  '+' 'E' 'e' '.'
  XFILEJSONCONTROLCHAR_TEXT                 //  (All Chars except control,Space,TAB,Return o Line Feed).
};


#define XFILEJSON_SPACETABS                      2

#define XFILEJSON_ADDVALUE(node, name, value)    {  XFILEJSONVALUE* jsonvalue = new XFILEJSONVALUE();             \
                                                    if(jsonvalue)                                                 \
                                                      {                                                           \
                                                        jsonvalue->SetName(name);                                 \
                                                        jsonvalue->Set(value);                                    \
                                                        node->Add(jsonvalue);                                     \
                                                      }                                                           \
                                                  }




//---- CLASS -------------------------------------------------------------------------------

class XFILEJSONOBJECT;
class XFILEJSONARRAY;


union XFILEJSONVALUEDATA
{
    void*                       pointer;
    int                         integer;
    float                       floating;
    bool                        boolean;
};


class XFILEJSONVALUE
{
  public:
                                XFILEJSONVALUE                ();
    virtual                    ~XFILEJSONVALUE                ();

    XFILEJSONVALUETYPE          GetType                       ();
    XSTRING*                    GetName                       ();

    void*                       GetValuePointer               ();
    int                         GetValueInteger               ();
    float                       GetValueFloating              ();
    bool                        GetValueBoolean               ();

    bool                        SetName                       (XSTRING& name);
    bool                        SetName                       (XCHAR* name);

    bool                        Set                           (int number);
    bool                        Set                           (float number);
    bool                        Set                           (XSTRING& string);
    bool                        Set                           (XCHAR* string);
    bool                        Set                           (XFILEJSONOBJECT* object);
    bool                        Set                           (XFILEJSONARRAY* array);
    bool                        Set                           (bool boolean);
    bool                        Set                           (void);

    bool                        Delete                        ();

  private:

    void                        Clean                         ();

    XFILEJSONVALUETYPE          type;
    XSTRING*                    name;
    XFILEJSONVALUEDATA          value;
};



class XFILEJSONOBJECT
{
  public:
                                XFILEJSONOBJECT               ();
    virtual                    ~XFILEJSONOBJECT               ();

    bool                        Add                           (XFILEJSONVALUE* value);
    bool                        Add                           (XCHAR* name,XFILEJSONARRAY* array);
    bool                        Add                           (XSTRING& name,XFILEJSONARRAY* array);
    bool                        Add                           (XCHAR* name,XFILEJSONOBJECT* object);
    bool                        Add                           (XSTRING& name,XFILEJSONOBJECT* object);

    XVECTOR<XFILEJSONVALUE*>*   GetValues                     ();
    bool                        DeleteAllValues               ();

  protected:

    XVECTOR<XFILEJSONVALUE*>    values;

  private:

    void                        Clean                         ();
};




class XFILEJSONARRAY  : public XFILEJSONOBJECT
{
  public:
                                XFILEJSONARRAY                ();
    virtual                    ~XFILEJSONARRAY                ();

    XSTRING*                    GetName                       ();
    bool                        SetName                       (XSTRING& name);
    bool                        SetName                       (XCHAR* name);

  private:

    void                        Clean                         ();

    XSTRING                     name;
};




class XFILEJSON : public XFILETXT
{
  public:
                                XFILEJSON                     ();
    virtual                    ~XFILEJSON                     ();

    XFILEJSONOBJECT*            GetRoot                       ();
    bool                        SetRoot                       (XFILEJSONOBJECT* root);

    bool                        DecodeAllLines                ();
    bool                        EncodeAllLines                (bool istabulatedline = true);

    bool                        ReadAndDecodeAllLines         ();
    bool                        WriteAndEncodeAllLines        (bool istabulatedline, XFILETXTFORMATCHAR formatchar, XFILETXTTYPELF  typeLF = XFILETXTTYPELF_0D0A);
    bool                        WriteAndEncodeAllLines        (bool istabulatedline = true);

    bool                        DeleteAllObjects              ();

    XFILEJSONVALUE*             GetValue                      (XCHAR* name, XFILEJSONOBJECT* startobject = NULL);
    XFILEJSONVALUE*             GetValue                      (XSTRING& name, XFILEJSONOBJECT* startobject = NULL);

  private:

    void                        Clean                         ();


    bool                        DecodeObject                  (int& position, bool isobject, XFILEJSONOBJECT* object);

    XFILEJSONCONTROLCHAR        CheckControlCharacter         (XCHAR character);
    XCHAR                       GetControlCharacter           (XFILEJSONCONTROLCHAR ctrlchar);
    bool                        AddControlCharacter           (XFILEJSONCONTROLCHAR ctrlchar, XSTRING& line);

    XFILEJSONCONTROLCHAR        SearchNextControlCharacter    (int& position);
    bool                        SearchControlCharacter        (XFILEJSONCONTROLCHAR ctrlchar,int& position);

    bool                        GetNumber                     (int& position,XSTRING& string,bool& special);
    bool                        GetText                       (int& position,XSTRING& string);
    bool                        GetString                     (int& position,XSTRING& string);

    bool                        AddTabs                       (int level,int spacetabs,XSTRING& string);

    bool                        EncodeObject                  (bool isobject, XFILEJSONOBJECT* object, bool istabulatedline = true);

    XFILEJSONOBJECT*            root;
    XSTRING                     all;
    int                         encodelevel;
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------


#endif

