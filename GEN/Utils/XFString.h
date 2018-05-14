/*------------------------------------------------------------------------------------------
//  XSTRING.H
*/
/**
// \class
//
//  Unicode Fast String Class
//
//  @author  Diego Martinez Ruiz de Gaona
//  @version 23/11/2010 9:48:03
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _XFSTRING_H_
#define _XFSTRING_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XString.h"

#ifdef MICROCONTROLLER
#error "this class should not be used on microcontrollers"
#endif

#pragma warning( push )
#pragma warning( disable : 4996)

/*---- CLASS -----------------------------------------------------------------------------*/

class XBUFFER;

class XFSTRING : public XSTRING
{
public:
    XFSTRING()               : XSTRING()  {};
    XFSTRING(int n)          : XSTRING(n) {};
    XFSTRING(const XCHAR* s) : XSTRING(s) {};
    XFSTRING(XCHAR* s)       : XSTRING(s) {};
    XFSTRING(const char* s)  : XSTRING(s) {};
    XFSTRING(XSTRING& s)     : XSTRING(s) {};

    int                   Fast_ConvertToInt(int index = 0)
    {
        int result = 0;
        swscanf(&text[index], __L("%d"), &result);
        return result;
    }

    XDWORD                Fast_ConvertToDWord(int index = 0)
    {
        XDWORD result = 0;
        swscanf(&text[index], __L("%u"), &result);
        return result;
    }

    XQWORD                Fast_ConvertToQWord(int index = 0)
    {
        XQWORD result = 0;
        swscanf(&text[index], __L("%lld"), &result);
        return result;
    }
    XQWORDSIG             Fast_ConvertToSQWord(int index = 0)
    {
        XQWORDSIG result = 0;
        swscanf(&text[index], __L("%lld"), &result);
        return result;
    }
    float                 Fast_ConvertToFloat(int index = 0)
    {
        float result = 0;
        swscanf(&text[index], __L("%f"), &result);
        return result;
    }

    double                Fast_ConvertToDouble(int index = 0)
    {
        double result = 0;
        swscanf(&text[index], __L("%lf"), &result);
        return result;
    }

    bool                  Fast_AddCharacter(XCHAR character)
    {
        if (!ReAllocBuffer(size + 1)) return false;

        memcpy(&text[size - 1], &character, sizeof(XCHAR));

        return true;
    }

    bool                  Fast_Empty()
    {
        if (!size) return true;
        memset(&text[0], 0, size * sizeof(XCHAR));
        size = 0;
        return true;
    }

    int                   scan                            (const XCHAR* mask, ...);
    bool                  Explode                         (XCHAR token, XVECTOR<XFSTRING*>* receive);

    void                  operator =                      (const char* string)          { Set(string); };
    void                  operator =                      (const XCHAR* string)         { Set(string); };
    void                  operator =                      (const XSTRING& string)       { Set(string); };
    void                  operator =                      (XWORD* string)               { Set(string); };
    void                  operator =                      (XBUFFER& string)             { Set(string); };

    void                  operator +=                     (const char* string)          { Add(string); };
    void                  operator +=                     (const XCHAR* string)         { Add(string); };
    void                  operator +=                     (XSTRING& string)             { Add(string); };
    void                  operator +=                     (XCHAR character)             { Add(character); };

    bool                  operator <                      (const XSTRING& string) const { return (bool)(Compare(string) == -1); }
    bool                  operator >                      (XSTRING& string)             { return (bool)(Compare(string) == 1); }
    bool                  operator <=                     (XSTRING& string)             { return (bool)(Compare(string) != 1); }
    bool                  operator >=                     (XSTRING& string)             { return (bool)(Compare(string) != -1); }
    bool                  operator ==                     (const XSTRING& string) const { return (bool)(Compare(string) == 0); }
    bool                  operator !=                     (XSTRING& string)             { return (bool)(Compare(string) != 0); }

    XCHAR                 operator []                     (int position)                { return text[position];  }
};


// Your function
#pragma warning( pop )

/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/


#endif





