/**-------------------------------------------------------------------------------------------------------------------
*
* @file        XWINDOWSSharedMemory.cpp
*
* @class       XWINDOWSSHAREDMEMORY
* @brief       Shared memory (Exchange between applications)
* @ingroup     PLATFORM_WINDOWS
*
* @author      Abraham J. Velez 
* @date        23/05/2018 13:04:01
*
* @copyright   Copyright(c) 2005 - 2018 GEN Group.
*
* @cond
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
* documentation files(the "Software"), to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense, and/ or sell copies of the Software,
* and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of
* the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
* THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
* @endcond
*
*---------------------------------------------------------------------------------------------------------------------*/

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "XWINDOWSSharedMemory.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XWINDOWSSHAREDMEMORY::Create
* @brief      Create Shared memory for this instance
* @ingroup    PLATFORM_WINDOWS
*
* @author     Abraham J. Velez 
* @date       23/05/2018 16:50:18
*
* @param[in]  ID : Identificador unique of the shared memory
* @param[in]  size : size of the shared memory
*
* @return     XBYTE* : Pointer to shared memory
*
*---------------------------------------------------------------------------------------------------------------------*/
XBYTE* XWINDOWSSHAREDMEMORY::Create(XCHAR* ID, XDWORD size)
{
  // Allocate shared memory from the system by creating a shared memory  
  // pool basing it out of the system paging (swapper) file.   

  this->ID   = ID;
  this->size = (size + sizeof(XDWORD));

  handlefile = CreateFileMapping((HANDLE)NULL,
                                 NULL,
                                 PAGE_READWRITE,
                                 0,
                                 this->size, 
                                 ID);
                               
  if(!handlefile) return NULL;
    
  // Map the file to this process' address space, starting at an address  
  // that should also be available in child processe(s)                   

  base = (XBYTE*)MapViewOfFileEx(handlefile, FILE_MAP_WRITE, 0, 0, 0, 0);
  if(base)
    {     
      XDWORD* ptrsize = (XDWORD*)base;
      (*ptrsize) = this->size;

      pointer  = base;
      pointer += sizeof(XDWORD);
    }
  
  return pointer;
}


 
/**-------------------------------------------------------------------------------------------------------------------
*
*  @fn         XWINDOWSSHAREDMEMORY::Open
*  @brief      Open Shared Memory, created by other proccess
*  @ingroup    PLATFORM_WINDOWS
*
*  @author     Abraham J. Velez 
*  @date       23/05/2018 16:49:53
*
*  @param[in]  ID : Identificador unique of the shared memory
*  @param[out] size : return size of this shared memory
*
*  @return     XBYTE* : Pointer to shared memory
*
*---------------------------------------------------------------------------------------------------------------------*/
XBYTE* XWINDOWSSHAREDMEMORY::Open(XCHAR* ID, XDWORD& size)
{
  // Open the shared memory file by name.  The file is based on the     
  // system paging (swapper) file.   

  this->ID   = ID;
  this->size = 0;

  size = 0;

  handlefile  = OpenFileMapping(FILE_MAP_WRITE, FALSE, ID);
  if(!handlefile) return NULL;
   
  // Figure out where to map this file by looking at the address in the   
  // shared memory where the memory was mapped in the parent process.   

  base = (XBYTE*)MapViewOfFile(handlefile, FILE_MAP_WRITE, 0, 0, sizeof(XDWORD));
  if(base)
    {
       size = (XDWORD)(*base);        
       UnmapViewOfFile(base);                    

       base = (XBYTE*)MapViewOfFile(handlefile, FILE_MAP_WRITE, 0, 0, size);
       if(base)
         {
           this->size = size;

           size -= sizeof(XDWORD);

           pointer  = base;
           pointer += sizeof(XDWORD);
         }
    }

  return pointer;
}


  
 
/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XWINDOWSSHAREDMEMORY::Close
* @brief      Close Shared Memory for this instance
* @ingroup    PLATFORM_WINDOWS
*
* @author     Abraham J. Velez 
* @date       23/05/2018 16:52:36
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSSHAREDMEMORY::Close()
{ 
  if(!base)    return false;
  if(!pointer) return false;
  
  UnmapViewOfFile(base);                    
  
  CloseHandle(handlefile);

  XSHAREDMEMORY::Clean();
 
  return true;
}