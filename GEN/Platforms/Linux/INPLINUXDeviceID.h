/*------------------------------------------------------------------------------------------
//  INPLINUXDEVICEID.H
*/
/**
// \class
//
//  sdsa
//
//  @author  Abraham J. Velez
//
//  Date Of Creation  : 10/04/2014 11:04:16
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _INPLINUXDEVICEID_H_
#define _INPLINUXDEVICEID_H_


/*---- INCLUDES --------------------------------------------------------------------------*/



/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

#define INPLINUXDEVICEID_HANDLEFILE   __L("/proc/bus/input/devices")

#define INPLINUXDEVICEID_INVALID      -1

/*---- CLASS -----------------------------------------------------------------------------*/


class INPLINUXDEVICEID
{
  public:

                    INPLINUXDEVICEID              ()                      { Clean();                                }
    virtual        ~INPLINUXDEVICEID              ()                      { Clean();                                }

    int             GetFileDescriptor             ()                      { return filedescriptor;                  }
    void            SetFileDescriptor             (int filedescriptor)    { this->filedescriptor=filedescriptor;    }

    XSTRING*        GetName                       ()                      { return &name;                           }


  private:

    void            Clean                         ()
                    {
                      filedescriptor  = INPLINUXDEVICEID_INVALID;
                    }

    int             filedescriptor;
    XSTRING         name;
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/


#endif

