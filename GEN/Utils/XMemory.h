//------------------------------------------------------------------------------------------
//  XMEMORY.H
//
/**
// \class XMemory
//
//
//
//  @author  Abraham J. Velez
//  @version 10/01/2001
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _XMEMORY_H_
#define _XMEMORY_H_

#ifdef XMEM

//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>

#ifdef LINUX
#include <pthread.h>
#endif

#include "XBase.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

#ifdef _MSC_VER
  #pragma warning (disable:4291 4786)
#endif

#ifdef MICROCONTROLLER
#define XMEMORY_MAXASSIGN              (XDWORD)128
#else
#define XMEMORY_MAXASSIGN              (XDWORD)10485760
#endif

#define XMEMORY_SHOWDATABLOCKSIZE       16

#define XMEMORY_DISPLAYMEMORYLEAKS      XMemory.DisplayAll(true);
#define XMEMORY_GETMEMORYUSED           XMemory.GetMemoryUsed()
#define XMEMORY_GETMEMORYNBLOCKS        XMemory.GetMemoryNBlocks()

//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XMUTEX;

class XMEMORY_ASSIGN
{
  public:

    void*                 GetPtr                      ()                                      { return ptr;                       }
    int                   GetSize                     ()                                      { return (int)size;                 }
    char*                 GetNameModule               ()                                      { return namemodule;                }
    int                   GetLineModule               ()                                      { return linemodule;                }

    bool                  SetPtr                      (void* ptr)                             { this->ptr        = ptr; return true;  }
    bool                  SetSize                     (int size)                              { this->size       = size; return true; }
    bool                  SetNameModule               (char* namemodule);
    bool                  SetLineModule               (int linemodule)                        { this->linemodule = linemodule; return true;   }

    bool                  Delete                      ();

    void                  Clean                       ();

    void*                 ptr;
    size_t                size;
    char*                 namemodule;
    int                   linemodule;
};



class XMEMORY
{
  public:

    static const XDWORD   maximunLeaksToDisplay = 50;

                          XMEMORY                     ();
    virtual              ~XMEMORY                     ();

    char*                 GetNameFile                 (char* pathfile);

    void*                 Assign                      (size_t size,char* namefile,int line);
    void                  Free                        (void* ptr);

    bool                  FreeAll                     ();

    XDWORD                GetMemoryUsed               ()                                    { return used;                        }
    XDWORD                GetMemoryNAssigns           ()                                    { return nassigns;                    }

    bool                  DisplayAll                  (bool displaydata = false);

  private:

    void                  Clean                       ();

    bool                  Lock                        ();
    bool                  UnLock                      ();

    XWORD                 CRC16                       (XBYTE* buffer, XWORD size);
    XDWORD                CRC32                       (XBYTE* buffer, XWORD size);

    bool                  SearchNeighbour             (XDWORD start, void* ptr, XDWORD& index);

    XMEMORY_ASSIGN*       assignlist[XMEMORY_MAXASSIGN];
    XDWORD                nassigns;
    XDWORD                maxnassigns;

    XDWORD                used;
    XDWORD                maxused;

    XDWORD                mutexhandle;

    XDWORD                CRC32table[300];


    #ifdef LINUX
    pthread_mutex_t       mutex;
    pthread_mutexattr_t   attr;
    #endif
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------


extern XMEMORY XMemory;


inline void* operator new(size_t size,char const* namefile, int line)
{
  return XMemory.Assign(size,(char*)namefile,line);
}


inline void operator delete(void* ptr)
{
  XMemory.Free(ptr);
}


inline void* operator new[](size_t size,char const* namefile, int line)
{
  return XMemory.Assign(size,(char*)namefile,line);
}


inline void operator delete [](void* ptr)
{
  XMemory.Free(ptr);
}


#define new    new(__FILE__, __LINE__)

#else

#define XMEMORY_DISPLAYMEMORYLEAKS
#define XMEMORY_GETMEMORYUSED           0
#define XMEMORY_GETMEMORYNBLOCKS        0


#endif

#endif


