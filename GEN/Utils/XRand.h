//------------------------------------------------------------------------------------------
//  XRAND.H
//
/**
// \class
//
//  Random Functions
//
//  @author  Abraham J. Velez
//  @version 22/04/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _XRAND_H_
#define _XRAND_H_

//---- INCLUDES ----------------------------------------------------------------------------

#include <stdlib.h>

//---- DEFINES & ENUMS  --------------------------------------------------------------------

#define GETRANDOM(min,max)  ((rand()%(int)(((max)+1)-(min)))+(min))

//---- CLASS -------------------------------------------------------------------------------

class XRAND
{
  public:
                    XRAND       ()                        {                                 }
    virtual        ~XRAND       ()                        {                                 }

    virtual bool    Ini         ()                  = 0;
    virtual int     MaxElements (int max)           = 0;
    virtual int     Max         (int max)           = 0;
    virtual int     Between     (int min, int max)  = 0;

    float           Between     (float  min,  float max)
                    {
                      int   randomvalue = rand();
                      float delta       = ((float)randomvalue)/(float)RAND_MAX;
                      return ((max-min)*delta)+min;
                    }

    bool            Percent     (int percent)
                    {
                      int random = Between(1,100);
                      if(random<=percent)
                        {
                          return true;
                        }

                      return false;
                    }

};

//---- INLINE FUNCTIONS --------------------------------------------------------------------


#endif

