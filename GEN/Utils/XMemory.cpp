//------------------------------------------------------------------------------------------
//  XMEMORY.CPP
//
//  XMemory
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 13/03/2002
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifdef XMEM

//---- INCLUDES ----------------------------------------------------------------------------

#include <stdlib.h>
#include <string.h>

#ifdef WINDOWS
#include <windows.h>
#include <process.h>
#endif

#include "XString.h"
#include "XDebug.h"

#include "XMemory.h"


//---- DEFINES & ENUMS  --------------------------------------------------------------------

#define CRC32ENTRY1   { 0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL, 0x076dc419L, 0x706af48fL, 0xe963a535L, 0x9e6495a3L, 0x0edb8832L, 0x79dcb8a4L }
#define CRC32ENTRY2   { 0xe0d5e91eL, 0x97d2d988L, 0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L, 0x90bf1d91L, 0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL }
#define CRC32ENTRY3   { 0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L, 0x136c9856L, 0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL, 0x14015c4fL, 0x63066cd9L }
#define CRC32ENTRY4   { 0xfa0f3d63L, 0x8d080df5L, 0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L, 0xa2677172L, 0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL }
#define CRC32ENTRY5   { 0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L, 0x32d86ce3L, 0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L, 0x26d930acL, 0x51de003aL }
#define CRC32ENTRY6   { 0xc8d75180L, 0xbfd06116L, 0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L, 0xb8bda50fL, 0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L }
#define CRC32ENTRY7   { 0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL, 0x76dc4190L, 0x01db7106L, 0x98d220bcL, 0xefd5102aL, 0x71b18589L, 0x06b6b51fL }
#define CRC32ENTRY8   { 0x9fbfe4a5L, 0xe8b8d433L, 0x7807c9a2L, 0x0f00f934L, 0x9609a88eL, 0xe10e9818L, 0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L }
#define CRC32ENTRY9   { 0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL, 0x6c0695edL, 0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L, 0x65b0d9c6L, 0x12b7e950L }
#define CRC32ENTRY10  { 0x8bbeb8eaL, 0xfcb9887cL, 0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L, 0xfbd44c65L, 0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L }
#define CRC32ENTRY11  { 0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL, 0x4369e96aL, 0x346ed9fcL, 0xad678846L, 0xda60b8d0L, 0x44042d73L, 0x33031de5L }
#define CRC32ENTRY12  { 0xaa0a4c5fL, 0xdd0d7cc9L, 0x5005713cL, 0x270241aaL, 0xbe0b1010L, 0xc90c2086L, 0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL }
#define CRC32ENTRY13  { 0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L, 0x59b33d17L, 0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL, 0xedb88320L, 0x9abfb3b6L }
#define CRC32ENTRY14  { 0x03b6e20cL, 0x74b1d29aL, 0xead54739L, 0x9dd277afL, 0x04db2615L, 0x73dc1683L, 0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L }
#define CRC32ENTRY15  { 0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L, 0xf00f9344L, 0x8708a3d2L, 0x1e01f268L, 0x6906c2feL, 0xf762575dL, 0x806567cbL }
#define CRC32ENTRY16  { 0x196c3671L, 0x6e6b06e7L, 0xfed41b76L, 0x89d32be0L, 0x10da7a5aL, 0x67dd4accL, 0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L }
#define CRC32ENTRY17  { 0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L, 0xd1bb67f1L, 0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL, 0xd80d2bdaL, 0xaf0a1b4cL }
#define CRC32ENTRY18  { 0x36034af6L, 0x41047a60L, 0xdf60efc3L, 0xa867df55L, 0x316e8eefL, 0x4669be79L, 0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L }
#define CRC32ENTRY19  { 0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL, 0xc5ba3bbeL, 0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L, 0xc2d7ffa7L, 0xb5d0cf31L }
#define CRC32ENTRY20  { 0x2cd99e8bL, 0x5bdeae1dL, 0x9b64c2b0L, 0xec63f226L, 0x756aa39cL, 0x026d930aL, 0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L }
#define CRC32ENTRY21  { 0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L, 0x92d28e9bL, 0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L, 0x86d3d2d4L, 0xf1d4e242L }
#define CRC32ENTRY22  { 0x68ddb3f8L, 0x1fda836eL, 0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L, 0x18b74777L, 0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL }
#define CRC32ENTRY23  { 0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L, 0xa00ae278L, 0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L, 0xa7672661L, 0xd06016f7L }
#define CRC32ENTRY24  { 0x4969474dL, 0x3e6e77dbL, 0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L, 0x37d83bf0L, 0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L }
#define CRC32ENTRY25  { 0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L, 0xbad03605L, 0xcdd70693L, 0x54de5729L, 0x23d967bfL, 0xb3667a2eL, 0xc4614ab8L }
#define CRC32ENTRY26  { 0x5d681b02L, 0x2a6f2b94L, 0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL, 0x2d02ef8dL, 0x00000000L, 0x00000000L, 0x00000000L, 0x00000000L }

#define CRC32ENTRY(ind) { XDWORD data[10] = CRC32ENTRY##ind;    \
                          for(int d=0;d<10;d++)               \
                            {                                 \
                              CRC32table[index] = data[d];    \
                              index++;                        \
                            }                                 \
                          }

#define _CRC32(result, value)   result = (XDWORD)CRC32table[(XBYTE)result^(XBYTE)value]^(result>>8)

//---- GENERAL VARIABLE --------------------------------------------------------------------

XMEMORY XMemory;

//---- CLASS MEMBERS -----------------------------------------------------------------------




/*-------------------------------------------------------------------
//  XMEMORY_ASSIGN::SetNameModule
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      31/05/2011 10:41:56
//
//  @return       bool :
//  @param        namemodule :
*/
/*-----------------------------------------------------------------*/
bool XMEMORY_ASSIGN::SetNameModule(char* _namemodule)
{
  int sizenamemodule = strlen(_namemodule)+1;

  this->namemodule = (char*)malloc(sizenamemodule);
  if(!this->namemodule) return false;

  //memset(this->namemodule, 0, sizenamemodule);
  //memcpy(this->namemodule, _namemodule, sizenamemodule-1);

  memcpy(this->namemodule, _namemodule, sizenamemodule);

  return true;

}




/*-------------------------------------------------------------------
//  XMEMORY_ASSIGN::Delete
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      31/05/2011 10:48:25
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XMEMORY_ASSIGN::Delete()
{
  free(namemodule);
  free(ptr);

  Clean();

  return true;
}


//-------------------------------------------------------------------
//  XMEMORY_ASSIGN::Clean
/**
//
//
//  @author       Abraham J. Velez
//  @version      14/03/2002 17:45:19
//
//  @return       void :
//  */
//-------------------------------------------------------------------
void XMEMORY_ASSIGN::Clean()
{
  ptr        = NULL;
  size       = 0;
  namemodule = NULL;
  linemodule = 0;
}



//-------------------------------------------------------------------
//  XMEMORY::~XMEMORY
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/03/2002 16:45:47
//
//  @return
//  */
//-------------------------------------------------------------------
XMEMORY::XMEMORY()
{
  Clean();

  int index = 0;

  for(int c=0;c<30;c++)
    {
      switch(c)
        {
          case  1: CRC32ENTRY(1)  break;
          case  2: CRC32ENTRY(2)  break;
          case  3: CRC32ENTRY(3)  break;
          case  4: CRC32ENTRY(4)  break;
          case  5: CRC32ENTRY(5)  break;
          case  6: CRC32ENTRY(6)  break;
          case  7: CRC32ENTRY(7)  break;
          case  8: CRC32ENTRY(8)  break;
          case  9: CRC32ENTRY(9)  break;
          case 10: CRC32ENTRY(10) break;
          case 11: CRC32ENTRY(11) break;
          case 12: CRC32ENTRY(12) break;
          case 13: CRC32ENTRY(13) break;
          case 14: CRC32ENTRY(14) break;
          case 15: CRC32ENTRY(15) break;
          case 16: CRC32ENTRY(16) break;
          case 17: CRC32ENTRY(17) break;
          case 18: CRC32ENTRY(18) break;
          case 19: CRC32ENTRY(19) break;
          case 20: CRC32ENTRY(20) break;
          case 21: CRC32ENTRY(21) break;
          case 22: CRC32ENTRY(22) break;
          case 23: CRC32ENTRY(23) break;
          case 24: CRC32ENTRY(24) break;
          case 25: CRC32ENTRY(25) break;
          case 26: CRC32ENTRY(26) break;
        }
    }



  #ifdef WINDOWS
  mutexhandle = (XDWORD)CreateMutex( NULL, FALSE, NULL );
  if(!mutexhandle) return;
  #endif

  #ifdef LINUX
  pthread_mutexattr_init(&attr);
  pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
  if(!pthread_mutex_init(&mutex, &attr)) mutexhandle = 1;

  #endif

  for(XDWORD c=0;c<XMEMORY_MAXASSIGN;c++)
    {
      assignlist[c] = NULL;
    }


}




//-------------------------------------------------------------------
//  XMEMORY::~XMEMORY
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/03/2002 16:45:47
//
//  @return
//  */
//-------------------------------------------------------------------
XMEMORY::~XMEMORY()
{
  FreeAll();

  #ifdef WINDOWS
  if(mutexhandle)
    {
      CloseHandle((HANDLE)mutexhandle);
      mutexhandle = NULL;
    }
  #endif

  #ifdef LINUX
  if(mutexhandle)
    {
      pthread_mutex_destroy(&mutex);
      mutexhandle = 0;
    }
  #endif


  Clean();
}




//-------------------------------------------------------------------
//  XMEMORY::Assign
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/03/2002 15:26:14
//
//  @return       void* :
//  @param        size :
//  @param        pathfile :
//  @param        line :
*/
//-------------------------------------------------------------------
void* XMEMORY::Assign(size_t size,char* pathfile,int line)
{
  if(!size) return NULL;

  Lock();

  void* ptr = NULL;
  ptr = malloc(size);
  if(!ptr)
    {
      UnLock();
      return NULL;
    }


  XDWORD  start = 0;
  XDWORD  index = 0;

  start = CRC32((XBYTE*)&ptr, sizeof(XDWORD));
  start =  (start & (XMEMORY_MAXASSIGN-1)) ^ (start >> 19);

  if(SearchNeighbour(start, NULL, index))
    {
      if(nassigns+1 >= (XMEMORY_MAXASSIGN-10))
        {
          // XDEBUG_PRINTCOLOR(4, __L("XMEMORY ALERT: It will exceed the maximum of possible assignments [%d Nblocks]!!!"), nblocks);

          free(ptr);
          ptr = NULL;
        }
       else
        {
          XMEMORY_ASSIGN* assign = (XMEMORY_ASSIGN*)malloc(sizeof(XMEMORY_ASSIGN));
          if(assign)
            {
              assign->Clean();

              assign->ptr = ptr;
              assign->size = size;

              assign->SetNameModule(pathfile);
              assign->linemodule = line;

              nassigns++;
              maxnassigns = __MAX(nassigns, maxnassigns);

              used += assign->size;
              maxused  = __MAX(used, maxused);

              assignlist[index] = assign;
            }
           else
            {
              // XDEBUG_PRINTCOLOR(4, __L("XMEMORY ALERT: Block without control [%d Nblocks]!!!"), nblocks);

              free(ptr);
              ptr = NULL;
            }
        }
    }
   else
    {
      // XDEBUG_PRINTCOLOR(4, __L("XMEMORY ALERT: Insufficient memory [%d Nblocks ]!!!"), nblocks);

      free(ptr);
      ptr = NULL;
    }

  UnLock();

  return ptr;
}




//-------------------------------------------------------------------
//  XMEMORY::Free
/**
//
//
//  @author       Abraham J. Velez
//  @version      14/03/2002 16:15:03
//
//  @return       void :
//  @param        ptr :
*/
//-------------------------------------------------------------------
void XMEMORY::Free(void* ptr)
{
  if(!ptr) return;

  Lock();

  XDWORD  start = 0;
  XDWORD  index = 0;

  start = CRC32((XBYTE*)&ptr, sizeof(XDWORD));
  start =  (start & (XMEMORY_MAXASSIGN-1)) ^ (start >> 19);

  if(SearchNeighbour(start, ptr, index))
    {
      nassigns--;
      used -= assignlist[index]->size;

      assignlist[index]->Delete();

      free(assignlist[index]);

      assignlist[index] = NULL;
    }
   else
    {
      // XDEBUG_PRINTCOLOR(4, __L("XMEMORY ALERT: Pointer not found!! [%d Nblocks]!!!"), nblocks);
    }

  UnLock();
}



//-------------------------------------------------------------------
//  XMEMORY::FreeAll
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/03/2002 16:44:09
//
//  @return       bool :
//  */
//-------------------------------------------------------------------
bool XMEMORY::FreeAll()
{
  Lock();

  for(XDWORD c=0; c<XMEMORY_MAXASSIGN; c++)
    {
      if(assignlist[c])
        {
          nassigns--;
          used -= assignlist[c]->size;

          assignlist[c]->Delete();
          free(assignlist[c]);

          assignlist[c] = NULL;
        }
    }

  memset(assignlist,0,XMEMORY_MAXASSIGN*sizeof(XMEMORY_ASSIGN*));

  UnLock();

  return true;
}




/*-------------------------------------------------------------------
//  XMEMORY::DisplayAll
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      01/06/2011 18:34:35
//
//  @return       bool :
//  @param        displaydata :
*/
/*-----------------------------------------------------------------*/
bool XMEMORY::DisplayAll(bool displaydata)
{
  int nassign = 0;

  for(XDWORD c=0;c<XMEMORY_MAXASSIGN;c++)
    {
      if(assignlist[c]) nassign++;
    }
#ifdef XDEBUG
  XDEBUG_PRINTHEADER((!nassign)?__L("ALL FREE MEMORY RESOURCES"):__L("NOT FREE MEMORY RESOURCES"));
  XDEBUG_PRINT(__L(" "));
  XDEBUG_PRINT(__L("Maximum allocated blocks at a time : %d") , maxnassigns);
  XDEBUG_PRINT(__L("Maximum memory used at a time      : %dk"), maxused/1024);
  if(nassign)
    {
      XDEBUG_PRINT(__L("Number memory blocks not free      : %d"), nassign);
      XDEBUG_PRINT(__L(" "));
      XDEBUG_PRINT(__L("         Address  Size    Line  Module"));
      XDWORD count = 0;
      for(XDWORD c=0;c<XMEMORY_MAXASSIGN;c++)
        {
          if(assignlist[c])
            {
              if(assignlist[c]->ptr)
                {
                  XPATH   xpathnamemodule(assignlist[c]->namemodule);
                  XSTRING namemodule;

                  xpathnamemodule.GetNamefileExt(namemodule);

                  size_t  address = (size_t)assignlist[c]->ptr;
                  XDWORD _address = (XDWORD)address;

                  XDEBUG_PRINTCOLOR(XDEBUG_COLORRED, __L("%08d %08X %07d %05d %s")  , c
                                                                                    , _address
                                                                                    , assignlist[c]->size
                                                                                    , assignlist[c]->linemodule
                                                                                    , namemodule.Get());
                  //-----------------------------------------------------------------


                  int datablocksize  = assignlist[c]->size;
                  if(datablocksize>XMEMORY_SHOWDATABLOCKSIZE) datablocksize = XMEMORY_SHOWDATABLOCKSIZE;

                  if(displaydata)
                  {
                    XDEBUG_PRINTDATABLOCK(XDEBUGLEVELCOLOR(XDEBUG_COLORRED), (XBYTE*)assignlist[c]->ptr, datablocksize, 1);
                  }

                  //-----------------------------------------------------------------
                  count++;
                  if (count > maximunLeaksToDisplay)
                  {
                      XDEBUG_PRINTCOLOR(XDEBUGLEVELCOLOR(XDEBUG_COLORRED), __L("\n\n [...]\t\t\t\t too many leaks, only the first %d of %d shown\n\n\n\n "), maximunLeaksToDisplay, nassign);
                      break;
                  }
                }
            }
        }

      XDEBUG_PRINTHEADER(NULL);
    }
#endif
  return true;
}



/*-------------------------------------------------------------------
//  XMEMORY::Clean
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      26/05/2011 21:18:06
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void XMEMORY::Clean()
{
  nassigns     = 0;
  maxnassigns  = 0;

  used         = 0;
  maxused      = 0;

  mutexhandle  = 0;
}



/*-------------------------------------------------------------------
//  XMEMORY::Lock
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      01/06/2011 10:48:04
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XMEMORY::Lock()
{
  #ifdef WINDOWS
  if(!mutexhandle) return false;
  if(WaitForSingleObject((HANDLE)mutexhandle, INFINITE) == WAIT_FAILED) return false;
  #endif

  #ifdef LINUX
  if(!mutexhandle)               return false;
  if(pthread_mutex_lock(&mutex)) return false;
  #endif

  return true;
}



/*-------------------------------------------------------------------
//  XMEMORY::UnLock
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      01/06/2011 10:48:08
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XMEMORY::UnLock()
{
  #ifdef WINDOWS
  if(!mutexhandle)                       return false;
  if(!ReleaseMutex((HANDLE)mutexhandle)) return false;
  #endif

  #ifdef LINUX
  if(!mutexhandle)                       return false;
  if(pthread_mutex_unlock(&mutex))       return false;
  #endif

  return true;
}





/*-------------------------------------------------------------------
//  XMEMORY::CRC16
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      16/06/2014 23:08:33
//
//  @return       XWORD :
//
//  @param        buffer :
//  @param        size :
*/
/*-----------------------------------------------------------------*/
XWORD XMEMORY::CRC16(XBYTE* buffer, XWORD size)
{
  #define POLY 0x8408

  XBYTE i;
  XDWORD data;
  XDWORD crc = 0xffff;

  if(!size) return (~crc);

  do{ for(i=0, data=(XDWORD)0xff & *buffer++; i<8; i++, data >>= 1)
        {
          if ((crc & 0x0001) ^ (data & 0x0001)) crc = (crc >> 1) ^ POLY;  else  crc >>= 1;
        }

    } while (--size);

  crc   = ~crc;
  data  = crc;
  crc   = (crc << 8) | (data >> 8 & 0xff);

  return crc;
}




/*-------------------------------------------------------------------
//  XMEMORY::CRC32
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      07/07/2015 11:59:40
//
//  @return       bool :
//
//  @param        buffer :
//  @param        size :
*/
/*-----------------------------------------------------------------*/
XDWORD XMEMORY::CRC32(XBYTE* buffer, XWORD size)
{
  if(!size) return true;

  XDWORD value = 0;

  while(size--)
    {
      _CRC32(value, *buffer++);
    }

  return value;
}



/*-------------------------------------------------------------------
//  XMEMORY::SearchNeighbour
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      08/07/2015 9:31:50
//
//  @return       XDWORD :
//
//  @param        start :
//  @param        ptr :
*/
/*-----------------------------------------------------------------*/
bool XMEMORY::SearchNeighbour(XDWORD start, void* ptr, XDWORD& index)
{
  bool   found      = false;
  bool   endpart[2] = { false, false };
  int    tozero     = start-1;
  int    totop      = start;
  XDWORD counter    = 0;

  index = 0;

  if(!ptr)
    {
      counter = 0;
      while(counter < XMEMORY_MAXASSIGN)
        {
          if(totop < XMEMORY_MAXASSIGN)
            {
              if(!assignlist[totop])
                {
                  found = true;
                  index = totop;
                  break;
                }

              totop++;

            } else endpart[0] = true;

          if(tozero>=0 && tozero<XMEMORY_MAXASSIGN)
            {
              if(!assignlist[tozero])
                {
                  found = true;
                  index = tozero;
                  break;
                }

              tozero--;

            } else endpart[1] = true;

          if(endpart[0] && endpart[1]) break;

          counter++;
       }
    }
   else
    {
      counter = 0;

      while(counter < XMEMORY_MAXASSIGN)
        {
          if(totop < XMEMORY_MAXASSIGN)
            {
              if(assignlist[totop])
                {
                  if(assignlist[totop]->ptr == ptr)
                    {
                      found = true;
                      index = totop;
                      break;
                    }
                }

              totop++;

            } else endpart[0] = true;

          if(tozero>=0 && tozero<XMEMORY_MAXASSIGN)
            {
              if(assignlist[tozero])
                {
                  if(assignlist[tozero]->ptr == ptr)
                    {
                      found = true;
                      index = tozero;
                      break;
                    }
                }

              tozero--;

            } else endpart[1] = true;

          if(endpart[0] && endpart[1]) break;

          counter++;
        }
    }

  return found;
}



#endif

