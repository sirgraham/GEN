/*------------------------------------------------------------------------------------------
// XFSTRING.CPP
*/
/**
// \class
//
// Fast Unicode String
//
//  @author: Diego Martinez Ruiz de Gaona
//
//  Date of Creation : 01/03/2018 16:18:24
//  Last Modification :
*/
/* GEN  Copyright (C).  All right reserved.
/*------------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XFString.h"
#include "XMemory.h"


/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/

/*-------------------------------------------------------------------
//   XFSTRING::scan
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  05/03/2018 11:20:20
//  @return   int :
//
//  @param    const :
//  @param    ... :
//
*//*-----------------------------------------------------------------*/
int XFSTRING::scan(const XCHAR* mask, ...)
{
    va_list ap;
    va_start(ap, mask);
    int n = vswscanf(this->text, mask, ap);
    va_end(ap);
    return n;
}

/*-------------------------------------------------------------------
//   XFSTRING::Explode
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  05/03/2018 11:20:31
//  @return
bool :
//
//  @param    XCHAR :
//  @param    XVECTOR<XFSTRING*>* :
//
*//*-----------------------------------------------------------------*/
bool XFSTRING::Explode(XCHAR token, XVECTOR<XFSTRING*>* receive)
{
    int start = 0;
    do
    {
        int end = this->FindCharacter(token, start);
        if (end == NOTFOUND)
            end = this->size;

        if ((end - start)>0)
        {
            XFSTRING* tok = new XFSTRING();
            if (!tok)
                return false;

            this->Copy(start, end, (*tok));

            if (!receive->Add(tok))
            {
                delete(tok);
                return false;
            }
        }
        start = end + 1;
    } while (start<(int)size);

    return true;
}