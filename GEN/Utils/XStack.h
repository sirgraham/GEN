/*------------------------------------------------------------------------------------------
//  XSTACK.H
*/
/**
// \class
//
//   eXtended Stack Class
//
//  @author  Abraham J. Velez
//
//  Date Of Creation  : 17/08/2014 12:31:44
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _XSTACK_H_
#define _XSTACK_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XVector.h"

#include "XMemory.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/


template<class T>
class XSTACK
{
  public:
                                  XSTACK        ()              {                                         }
    virtual                      ~XSTACK        ()              {                                         }

    int                           GetSize       ()              { return stack.GetSize();                 }
    void                          Clear         ()
                                  {
                                    stack.DeleteContents();
                                    stack.DeleteAll();
                                  }

    void                          Push          (T element)     { stack.Add(element);                     }
    T                             Pop           ()
                                  {
                                    T element = stack.GetLast();
                                    stack.Delete(element);
                                    return element;
                                  }

    T                             Top           ()              { return stack.GetLast();                 }

    XVECTOR<T>                    stack;
};



/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif




