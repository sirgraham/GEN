/**-------------------------------------------------------------------------------------------------------------------
*
* @file        XLINUXSharedMemory.cpp
*
* @class       XLINUXSHAREDMEMORY
* @brief       Shared memory (Exchange between applications)
* @ingroup     PLATFORM_LINUX
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
#include <fcntl.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "XBase.h"
#include "XString.h"

#include "XLINUXSharedMemory.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XLINUXSHAREDMEMORY::Create
* @brief      Create Shared memory for this instance
* @ingroup    PLATFORM_LINUX
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
XBYTE* XLINUXSHAREDMEMORY::Create(XCHAR* ID, XDWORD size)
{
  this->ID   = ID;
  this->size = (size +  sizeof(XDWORD));

  // create the shared memory segment 
  XSTRING_CREATEOEM(this->ID, namefile);
	handlefile = shm_open(namefile, O_CREAT | O_RDWR, 0666);
  XSTRING_DELETEOEM(namefile);

  if(handlefile < 0) return NULL;

	// configure the size of the shared memory segment 
	ftruncate(handlefile, size);

	// now map the shared memory segment in the address space of the process 
	base = (XBYTE*)mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, handlefile, 0);
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
*  @fn         XLINUXSHAREDMEMORY::Open
*  @brief      Open Shared Memory, created by other proccess
*  @ingroup    PLATFORM_LINUX
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
XBYTE* XLINUXSHAREDMEMORY::Open(XCHAR* ID, XDWORD& size)
{
  this->ID   = ID;
  this->size = 0;

  // open the shared memory segment as if it was a file 
  XSTRING_CREATEOEM(this->ID, namefile);
  handlefile = shm_open(namefile, O_RDWR, 0666);
  XSTRING_DELETEOEM(namefile);

  if(handlefile < 0) return NULL;

  // map the shared memory segment to the address space of the process */
  base = (XBYTE*)mmap(0, sizeof(XDWORD), PROT_READ | PROT_WRITE, MAP_SHARED, handlefile, 0);

  if(base)
    {
       size = (XDWORD)(*base);        
       munmap(pointer, sizeof(XDWORD));         

       base = (XBYTE*)mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, handlefile, 0);
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
* @fn         XLINUXSHAREDMEMORY::Close
* @brief      Close Shared Memory for this instance
* @ingroup    PLATFORM_LINUX
*
* @author     Abraham J. Velez 
* @date       23/05/2018 16:52:36
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XLINUXSHAREDMEMORY::Close()
{ 
  if(!pointer) return false;
   
  munmap(base, size);

  close(handlefile);

  XSTRING_CREATEOEM(ID, namefile);
  shm_unlink(namefile);
  XSTRING_DELETEOEM(namefile);

  XSHAREDMEMORY::Clean();
 
  return true;
}