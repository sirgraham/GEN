/**-------------------------------------------------------------------------------------------------------------------
*
* @file        XBuffer.cpp
*
* @class       XBUFFER
* @brief       eXtended buffer functions class
* @ingroup     UTILS
*
* @author      Abraham J. Velez 
* @date        25/05/2018 18:16:02
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
#include <string.h>

#include "XFactory.h"
#include "XTimer.h"
#include "XVariant.h"
#include "XSystem.h"

#include "XBuffer.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/


/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XBUFFER::XBUFFER(bool threadsafe)
* @brief      Constructor
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 18:26:02
*
* @param[in]  bool : Is thread safe buffer functions
*
* @return     Does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
XBUFFER::XBUFFER(bool threadsafe)
{
  Clean();

  hardwareuselittleendian = XSYSTEM::GetInstance().HardwareUseLittleEndian();

  if(threadsafe && xfactory) xmutex = xfactory->Create_Mutex();
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XBUFFER::XBUFFER(XDWORD size, bool threadsafe)
* @brief      Constructor
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 18:26:37
*
* @param[in]  size : initial size of the buffer
* @param[in]  threadsafe : true is thread safe operative buffer
*
* @return     Does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
XBUFFER::XBUFFER(XDWORD size, bool threadsafe)
{
  Clean();

  hardwareuselittleendian = XSYSTEM::GetInstance().HardwareUseLittleEndian();

  if(threadsafe && xfactory) xmutex = xfactory->Create_Mutex();

  Resize(size);
  FillBuffer();
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XBUFFER::~XBUFFER()
* @brief      Destructor
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 18:29:45
*
* @return     Does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
XBUFFER::~XBUFFER()
{
  Delete();

  if(xmutex) xfactory->Delete_Mutex(xmutex);

  Clean();
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD XBUFFER::GetSize()
* @brief      Get actual size of buffer
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 18:29:59
*
* @return     XDWORD : size actual of buffer
*
*---------------------------------------------------------------------------------------------------------------------*/
XDWORD XBUFFER::GetSize()
{
  SetBlocked(true);

  XDWORD rsize = size;

  SetBlocked(false);

  return rsize;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::SetSize(XDWORD size)
* @brief      Set size of buffer (not allocated space)
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 18:30:28
*
* @param[in]  size : new size of buffer
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::SetSize(XDWORD size)
{
  SetBlocked(true);

  this->size = size;

  SetBlocked(false);

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::IsEmpty()
* @brief      Check If buffer is empty
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       27/05/2018 10:47:09
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::IsEmpty()
{ 
  return (!GetSize())?true:false;                                
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XBUFFER::ResetPosition()
* @brief      Set pointer position of buffer to zero.
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 18:32:37
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XBUFFER::ResetPosition()
{
  SetPosition(0);
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD XBUFFER::GetPosition()
* @brief      Get Pointer position of buffer
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 18:33:07
*
* @return     XDWORD : Pointer position of buffer
*
*---------------------------------------------------------------------------------------------------------------------*/
XDWORD XBUFFER::GetPosition()
{
  return position;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::SetPosition(XDWORD position)
* @brief      Set Pointer to position
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 18:35:41
*
* @param[in]  position : new position of pointer.
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::SetPosition(XDWORD position)
{
  this->position = position;

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::IsBlocked()
* @brief      Get if this buffer is blocked
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 18:37:26
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::IsBlocked()
{
  if(!xmutex) return false;

  return xmutex->IsLock();
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::SetBlocked(bool blocked)
* @brief      Set Blocked buffer
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 18:38:17
*
* @param[in]  blocked : true to blocked buffer.
*
* @return     bool : true if is blocked
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::SetBlocked(bool blocked)
{
  if(!xmutex) return false;

  return (blocked)?xmutex->Lock():xmutex->UnLock();
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Add(XBYTE* pbuffer, XDWORD psize)
* @brief      Add array of bytes to buffer
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 18:43:11
*
* @param[in]  pbuffer : pointer of bytes to add
* @param[in]  psize : size of array of bytes
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Add(XBYTE* pbuffer, XDWORD psize)
{
  if(!pbuffer)  return false;
  if(!psize)    return false;

  SetBlocked(true);

  XDWORD ppos     = size;
  bool   status   = true;

  if(!Resize(size+psize,false))
    {
      SetBlocked(false);
      return false;
    }

  if(buffer && (size+psize))
         memcpy(&buffer[ppos], pbuffer, psize);
    else status = false;

  SetBlocked(false);

  return status;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Add(XBUFFER* buffer)
* @brief      Add buffer to buffer
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 18:44:25
*
* @param[in]  buffer : pointer of buffer to add
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Add(XBUFFER* buffer)
{
  return Add(buffer->buffer, buffer->size);
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Add(XBUFFER& buffer)
* @brief      Add buffer to buffer
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 18:45:11
*
* @param[in]  buffer : buffer to add
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Add(XBUFFER& buffer)
{
  return Add(buffer.buffer, buffer.size);
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Add(XBYTE data)
* @brief      Add one byte to buffer
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 18:46:25
*
* @param[in]  data : byte to add 
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Add(XBYTE data)
{
  return Add((XBYTE*)&data, sizeof(XBYTE));
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Add(bool data)
* @brief      Add one boolean to buffer 
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 18:46:49
*
* @param[in]  data : boolean to add
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Add(bool data)
{
  XBYTE _data = data?1:0;

  return Add((XBYTE*)&_data, sizeof(XBYTE));
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Add(XWORD data)
* @brief      Add one WORD to buffer (16 bits)
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 18:47:51
*
* @param[in]  data : WORD to add.
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Add(XWORD data)
{
  if(hardwareuselittleendian) SWAPWORD(data);

  return Add((XBYTE*)&data,sizeof(XWORD));
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Add(XDWORD data)
* @brief      Add one Doble WORD to buffer (32 bits)
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 18:48:40
*
* @param[in]  data : DWORD to Add
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Add(XDWORD data)
{
  if(hardwareuselittleendian) SWAPDWORD(data);

  return Add((XBYTE*)&data, sizeof(XDWORD));
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Add(XQWORD data)
* @brief      Add one Quad WORD to buffer (64 bits)
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 18:49:23
*
* @param[in]  data : QWORD to add 
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Add(XQWORD data)
{
  if(hardwareuselittleendian) SWAPQWORD(data);

  return Add((XBYTE*)&data, sizeof(XQWORD));
}




/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Add(float data)
* @brief      Add one float to buffer
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 18:50:11
*
* @param[in]  data : float to add
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Add(float data)
{
  return Add((XBYTE*)&data, sizeof(XDWORD));
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Add(double data)
* @brief      Add one double to buffer
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 18:52:20
*
* @param[in]  data : double to add
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Add(double data)
{
  return Add((XBYTE*)&data, sizeof(XQWORD));
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Add(XSTRING& string)
* @brief      Add one string to buffer
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 18:53:53
*
* @param[in]  string : string to add
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Add(XSTRING& string)
{
  if(string.IsEmpty()) return false;

  for(XDWORD c=0; c<string.GetSize(); c++)
    {
      XDWORD data = (XDWORD)string.Get()[c];

      if(hardwareuselittleendian) SWAPDWORD(data);

      Add((XDWORD)data);
    }

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::AddWithMask(XCHAR* mask, ...)
* @brief      Add variant data with a mask
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 18:54:38
*
* @param[in]  mask : pointer XCHAR with a mask to use in add
* @param[in]  ... : params with a variant data
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::AddWithMask(XCHAR* mask, ...)
{
  if(!mask) return false;

  XSTRING  _mask    = mask;
  XBUFFER  xbuffer;
  va_list  arg;

  va_start(arg, mask);

  AddXBufferWithMask(xbuffer, &_mask, arg);

  va_end(arg);

  return Add(xbuffer);
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::AddWithMask(XSTRING* mask, ...)
* @brief      Add variant data with a mask
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 18:59:23
*
* @param[in]  mask : string with a mask to use in add
* @param[in]  ... : params with a variant data
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::AddWithMask(XSTRING* mask, ...)
{
  XBUFFER  xbuffer;
  va_list  arg;

  va_start(arg, mask);

  AddXBufferWithMask(xbuffer, mask, arg);

  va_end(arg);

  return Add(xbuffer);
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Insert(XBYTE* pbuffer, XDWORD psize, int frompos)
* @brief      Insert array of bytes into buffer
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 19:02:14
*
* @param[in]  pbuffer : pointer to bytes to add
* @param[in]  psize : size of bytes to add
* @param[in]  frompos : position in buffer to insert
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Insert(XBYTE* pbuffer, XDWORD psize, int frompos)
{
  if(!pbuffer)                  return false;
  if(!psize)                    return false;

  if(frompos < 0)               return false;
  if(frompos >= (int)GetSize()) return false;

  XBUFFER xbuffertempo;

  if(frompos) xbuffertempo.Add(Get(), frompos);

  xbuffertempo.Add(pbuffer, psize);
  xbuffertempo.Add(&Get()[frompos], (GetSize()-frompos));

  Empty();

  Add(xbuffertempo);

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Insert(XBUFFER* xbuffer, int frompos)
* @brief      Insert to buffer into buffer
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 19:04:26
*
* @param[in]  xbuffer : pointer of buffer to insert into buffer
* @param[in]  frompos : position to insert
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Insert(XBUFFER* xbuffer, int frompos)
{
  if(!xbuffer)  return false;

  return Insert(xbuffer->Get(), xbuffer->GetSize(), frompos);
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Insert(XBUFFER& xbuffer, int frompos)
* @brief      Insert to buffer into buffer
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 19:07:07
*
* @param[in]  xbuffer : buffer to insert into buffer
* @param[in]  frompos : position to insert
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Insert(XBUFFER& xbuffer, int frompos)
{
  return Insert(xbuffer.Get(), xbuffer.GetSize(), frompos);
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Insert(XBYTE data, int frompos)
* @brief      Insert one byte into buffer
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 19:08:19
*
* @param[in]  data : byte to insert
* @param[in]  frompos : position of insert
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Insert(XBYTE data, int frompos)
{
  return Insert(&data, 1, frompos);
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Insert(bool data, int frompos)
* @brief      Insert one boolean into buffer
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 19:13:52
*
* @param[in]  data : boolean to insert
* @param[in]  frompos : position of insert
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Insert(bool data, int frompos)
{
  XBYTE _data = data?1:0;

  return Insert(&_data, 1, frompos);
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Insert(XWORD data, int frompos)
* @brief      Insert one WORD into buffer (16 bits)
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 19:15:35
*
* @param[in]  data : WORD to insert
* @param[in]  frompos : position of insert
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Insert(XWORD data, int frompos)
{
  if(hardwareuselittleendian) SWAPWORD(data);

  return Insert((XBYTE*)&data, sizeof(XWORD), frompos);
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Insert(XDWORD data, int frompos)
* @brief      Insert one doble WORD into buffer (32 bits)
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 19:16:59
*
* @param[in]  data : Double word to insert
* @param[in]  frompos : position of insert 
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Insert(XDWORD data, int frompos)
{
  if(hardwareuselittleendian) SWAPDWORD(data);

  return Insert((XBYTE*)&data, sizeof(XDWORD), frompos);
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Insert(XQWORD data, int frompos)
* @brief      Insert quad WORD into buffer (64 bits)
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 19:17:55
*
* @param[in]  data : Quad word to insert
* @param[in]  frompos : position of insert
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Insert(XQWORD data, int frompos)
{
  if(hardwareuselittleendian) SWAPQWORD(data);

  return Insert((XBYTE*)&data, sizeof(XQWORD), frompos);
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Insert(float data, int frompos)
* @brief      Insert float into buffer 
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 19:19:17
*
* @param[in]  data : float to insert
* @param[in]  frompos : position of insert
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Insert(float data, int frompos)
{
  return Insert((XBYTE*)&data, sizeof(XDWORD), frompos);
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Insert(double data, int frompos)
* @brief      Insert double into buffer 
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 19:20:03
*
* @param[in]  data : double to insert
* @param[in]  frompos : position of insert
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Insert(double data, int frompos)
{
  return Insert((XBYTE*)&data, sizeof(XQWORD), frompos);
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Insert(XSTRING& string, int frompos)
* @brief      Insert string into buffer 
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 19:20:54
*
* @param[in]  string : string to insert
* @param[in]  frompos : position of insert
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Insert(XSTRING& string, int frompos)
{
  if(string.IsEmpty()) return false;

  for(XDWORD c=0; c<string.GetSize(); c++)
    {
      XDWORD data = (XDWORD)string.Get()[c];

      if(hardwareuselittleendian) SWAPDWORD(data);

      Insert((XDWORD)data, frompos+c);
    }

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::InsertWithMask(XCHAR* mask, int frompos, ...)
* @brief      Add variant data with a mask
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 19:22:22
*
* @param[in]  mask : pointer XCHAR with a mask to use in insert
* @param[in]  frompos : position of insert
* @param[in]  ... : params with a variant data
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::InsertWithMask(XCHAR* mask, int frompos, ...)
{
  if(!mask) return false;

  XSTRING  _mask = mask;
  XBUFFER  xbuffer;
  va_list  arg;

  va_start(arg, frompos);

  AddXBufferWithMask(xbuffer, &_mask, arg);

  va_end(arg);

  return Insert(xbuffer, frompos);
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::InsertWithMask(XSTRING* mask, int frompos, ...)
* @brief      Add variant data with a mask
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 19:24:11
*
* @param[in]  mask : string with a mask to use in insert
* @param[in]  frompos : position of insert
* @param[in]  ... : params with a variant data
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::InsertWithMask(XSTRING* mask, int frompos, ...)
{
  XBUFFER  xbuffer;
  va_list  arg;

  va_start(arg, frompos);

  AddXBufferWithMask(xbuffer, mask, arg);

  va_end(arg);

  return Insert(xbuffer, frompos);
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD XBUFFER::Extract(XBYTE* pbuffer, XDWORD ppos, XDWORD psize)
* @brief      Extract to array of bytes from buffer
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 19:32:58
*
* @param[out] pbuffer : pointer to array of bytes.
* @param[in]  ppos : position of buffer to extract
* @param[in]  psize : size bytes to extract
*
* @return     XDWORD : number bytes extracted.
*
*---------------------------------------------------------------------------------------------------------------------*/
XDWORD XBUFFER::Extract(XBYTE* pbuffer, XDWORD ppos, XDWORD psize)
{
  SetBlocked(true);

  if((ppos>=size)||(!size))
    {
      SetBlocked(false);
      return 0;
    }

  int esize = psize;

  if((ppos+psize)>=size) esize = (size-ppos);

  if(pbuffer) memcpy(pbuffer,&buffer[ppos],esize);

  // #Imanol changed to avoid copyng overlapped memory, as that's undefined
  memmove(&buffer[ppos],&buffer[ppos+esize],size-(ppos+esize)); 
  
  //memcpy(&buffer[ppos],&buffer[ppos+esize],size-(ppos+esize));

  Resize(size-esize,false);

  SetBlocked(false);

  return esize;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Extract(XBYTE& data, XDWORD ppos)
* @brief      Extract one byte from buffer
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 19:38:07
*
* @param[out] data : byte extracted 
* @param[in]  ppos : position of buffer to extract
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Extract(XBYTE& data, XDWORD ppos)
{
  XDWORD size = Extract((XBYTE*)&data, ppos, sizeof(XBYTE));
  if(size!=sizeof(XBYTE)) return false;

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Extract(bool& data, XDWORD ppos)
* @brief      Extract one boolean from buffer
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 19:41:13
*
* @param[out] data : boolean extracted 
* @param[in]  ppos : position of buffer to extract
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Extract(bool& data, XDWORD ppos)
{
  XBYTE _data = 0;

  XDWORD size = Extract((XBYTE*)&_data, ppos, sizeof(XBYTE));
  if(size!=sizeof(XBYTE)) return false;

  data = _data?true:false;

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Extract(XWORD& data, XDWORD ppos)
* @brief      Extract one WORD from buffer (16 bits)
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 19:47:07
*
* @param[out] data : WORD extracted 
* @param[in]  ppos : position of buffer to extract
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Extract(XWORD& data, XDWORD ppos)
{
  XDWORD size = Extract((XBYTE*)&data, ppos, sizeof(XWORD));
  if(size!=sizeof(XWORD)) return false;

  if(hardwareuselittleendian) SWAPWORD(data);

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Extract(XDWORD& data, XDWORD ppos)
* @brief      Extract one Double WORD from buffer (32 bits)
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 19:49:35
*
* @param[out] data : DWORD extracted 
* @param[in]  ppos : position of buffer to extract
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Extract(XDWORD& data, XDWORD ppos)
{
  XDWORD size = Extract((XBYTE*)&data, ppos, sizeof(XDWORD));
  if(size!=sizeof(XDWORD)) return false;

  if(hardwareuselittleendian) SWAPDWORD(data);

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Extract(XQWORD& data, XDWORD ppos)
* @brief      Extract one Quad WORD from buffer (64 bits)
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 19:51:15
*
* @param[out] data : QWORD extracted 
* @param[in]  ppos : position of buffer to extract
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Extract(XQWORD& data, XDWORD ppos)
{
  XDWORD size = Extract((XBYTE*)&data, ppos, sizeof(XQWORD));
  if(size!=sizeof(XQWORD)) return false;

  if(hardwareuselittleendian) SWAPQWORD(data);

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Extract(float& data,XDWORD ppos)
* @brief      Extract one float from buffer 
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 19:53:59
*
* @param[out] data : float extracted
* @param[in]  ppos : position of buffer to extract
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Extract(float& data,XDWORD ppos)
{
  XDWORD size = Extract((XBYTE*)&data, ppos, sizeof(XDWORD));
  if(size!=sizeof(XDWORD)) return false;

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Extract(double& data, XDWORD ppos)
* @brief      Extract one double from buffer 
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 19:54:51
*
* @param[out] data : double extracted
* @param[in]  ppos : position of buffer to extract
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Extract(double& data, XDWORD ppos)
{
  XDWORD size = Extract((XBYTE*)&data, ppos, sizeof(XQWORD));
  if(size!=sizeof(XQWORD)) return false;

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Extract(XSTRING& string, XDWORD ppos, XDWORD psize)
* @brief      Extract one string from buffer 
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 19:57:08
*
* @param[in]  string : string extracted
* @param[in]  ppos : position of buffer to extract
* @param[in]  psize : size of string to extract
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Extract(XSTRING& string, XDWORD ppos, XDWORD psize)
{
  string.Empty();

  XDWORD* _string = new XDWORD[psize];
  int     _size   = psize * sizeof(XDWORD);

  if(!_string) return false;

  int rsize = Extract((XBYTE*)_string, ppos, _size);
  if(rsize == _size)
    {
      for(int c=0; c<(int)psize; c++)
        {
          XDWORD data = (XDWORD)_string[c];
          string.Add((XCHAR)data);
        }
    }

  delete [] _string;

  return true;
}




/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::ExtractWithMask(XCHAR* mask, int frompos, ...)
* @brief      Extract variant data with a mask
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 19:59:35
*
* @param[in]  mask : pointer XCHAR with a mask to use in extract
* @param[in]  frompos : position of buffer to extract
* @param[out] ... : params with a variant data
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::ExtractWithMask(XCHAR* mask, int frompos, ...)
{
  if(!mask) return false;

  XSTRING  _mask = mask;
  va_list  arg;

  va_start(arg, frompos);

  bool status = ExtractXBufferWithMask((*this), &_mask, arg);

  va_end(arg);

  return status;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::ExtractWithMask(XSTRING* mask, int frompos, ...)
* @brief      Extract variant data with a mask
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 20:03:47
*
* @param[in]  mask : string with a mask to use in extract
* @param[in]  frompos : position of buffer to extract
* @param[in]  ... : params with a variant data
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::ExtractWithMask(XSTRING* mask, int frompos, ...)
{
  va_list  arg;

  va_start(arg, frompos);

  bool status = ExtractXBufferWithMask((*this), mask, arg);

  va_end(arg);

  return status;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XBYTE* XBUFFER::Get()
* @brief      Get Pointer to array of bytes of buffer
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 20:14:21
*
* @return     XBYTE* : Array of bytes of buffer 
*
*---------------------------------------------------------------------------------------------------------------------*/
XBYTE* XBUFFER::Get()
{
  if(!size) return NULL;

  return buffer;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XBYTE XBUFFER::GetByte(XDWORD index)
* @brief      Get a byte from buffer
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 20:15:34
*
* @param[in]  index : index of byte into buffer
*
* @return     XBYTE : 
*
*---------------------------------------------------------------------------------------------------------------------*/
XBYTE XBUFFER::GetByte(XDWORD index)
{
  SetBlocked(true);

  XBYTE data;

  if(index > size)
    {
      SetBlocked(false);
      return 0;
    }

  data = buffer[index];

  position++;

  SetBlocked(false);

  return data;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Get(XBYTE* pbuffer,int psize,int frompos)
* @brief      Get array of bytes from buffer
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 20:21:09
*
* @param[out] pbuffer : byte array obtained
* @param[in]  psize : size of array
* @param[in]  frompos : position of buffer to get
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Get(XBYTE* pbuffer,int psize,int frompos)
{
  if(!pbuffer) return false;

  SetBlocked(true);

  int pos = frompos;
  if(frompos == XBUFFER_INVALIDPOSITION) pos = position;

  if((!buffer) || (pos>(int)size))
    {
      SetBlocked(false);
      return false;
    }

  if(buffer)
    {
      memcpy(pbuffer,&buffer[pos],psize);
      position = pos + psize;
    }

  SetBlocked(false);

  if(!buffer) return false;

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Get(XBYTE& data, int frompos)
* @brief      Get byte from buffer
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 20:24:44
*
* @param[out] data : byte obtained 
* @param[in]  frompos : position of buffer to get
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Get(XBYTE& data, int frompos)
{
  SetBlocked(true);

  int pos = frompos;
  if(frompos == XBUFFER_INVALIDPOSITION) pos = position;

  if(pos>(int)size)
    {
      SetBlocked(false);
      return false;
    }

  data = buffer[pos];

  position = pos + 1;

  SetBlocked(false);

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Get(bool& data, int frompos)
* @brief      Get boolean from buffer
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 20:27:30
*
* @param[in]  data : boolean obtained 
* @param[in]  frompos : position of buffer to get
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Get(bool& data, int frompos)
{
  XBYTE _data = 0;
  bool  status;

  status = Get(_data, frompos);

  if(status) data = _data?true:false;

  return status;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Get(XWORD& data, int frompos)
* @brief      Get WORD from buffer. (16 bits)
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 20:28:24
*
* @param[in]  data : WORD obtained
* @param[in]  frompos : position of buffer to get
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Get(XWORD& data, int frompos)
{
  if(!Get((XBYTE*)&data,sizeof(XWORD), frompos)) return false;

  if(hardwareuselittleendian) SWAPWORD(data);

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Get(XDWORD& data, int frompos)
* @brief      Get doble WORD from buffer. (32 bits)
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 20:30:07
*
* @param[in]  data : double WORD obtained
* @param[in]  frompos : position of buffer to get
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Get(XDWORD& data, int frompos)
{
  if(!Get((XBYTE*)&data,sizeof(XDWORD), frompos)) return false;

  if(hardwareuselittleendian) SWAPDWORD(data);

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Get(XQWORD& data, int frompos)
* @brief      Get quad WORD from buffer. (64 bits)
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 20:31:12
*
* @param[in]  data : quad WORD obtained
* @param[in]  frompos : position of buffer to get
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Get(XQWORD& data, int frompos)
{
  if(!Get((XBYTE*)&data,sizeof(XQWORD), frompos)) return false;

  if(hardwareuselittleendian) SWAPQWORD(data);

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Get(float& data, int frompos)
* @brief      Get float from buffer. 
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 20:32:24
*
* @param[in]  data : float obtained
* @param[in]  frompos : position of buffer to get
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Get(float& data, int frompos)
{
  if(!Get((XBYTE*)&data, sizeof(XDWORD), frompos)) return false;

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Get(double& data, int frompos)
* @brief      Get double from buffer. 
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 20:33:28
*
* @param[in]  data : double obtained.
* @param[in]  frompos : position of buffer to get 
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Get(double& data, int frompos)
{
  if(!Get((XBYTE*)&data, sizeof(XQWORD), frompos)) return false;

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Get(XSTRING& data,  int psize, int frompos)
* @brief      Get string from buffer.
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 20:35:15
*
* @param[out] data : strint obtained
* @param[in]  psize : size n bytes to obtain
* @param[in]  frompos : position of buffer to get
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Get(XSTRING& data,  int psize, int frompos)
{
  XDWORD pos = frompos;
  if(frompos == XBUFFER_INVALIDPOSITION) pos = position;

  XCHAR* ptr = data.Get();

  XDWORD c;
  for(XDWORD i = 0; i < (XDWORD)psize; i++)
  {
    if(!Get(c, pos)) return false;

    *(ptr+i) = (XCHAR)c;

    pos += sizeof(XDWORD);
  }

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::GetWithMask(XCHAR* mask, int frompos, ...)
* @brief      Get variant data with a mask
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       27/05/2018 11:19:56
*
* @param[in]  mask : pointer to string with a mask to use in get
* @param[in]  frompos : position of buffer to get
* @param[in]  ... : params with a variant data
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::GetWithMask(XCHAR* mask, int frompos, ...)
{
  if(!mask) return false;

  XSTRING  _mask   = mask;
  XBUFFER  xbuffer;
  va_list  arg;

  xbuffer.Add((*this));

  va_start(arg, frompos);

  bool status = ExtractXBufferWithMask(xbuffer, &_mask, arg);

  va_end(arg);

  return status;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::GetWithMask(XSTRING* mask, int frompos, ...)
* @brief      Get variant data with a mask
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 20:39:38
*
* @param[in]  mask : pointer to string with a mask to use in get
* @param[in]  frompos : position of buffer to get
* @param[in]  ... : params with a variant data
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::GetWithMask(XSTRING* mask, int frompos, ...)
{
  XBUFFER  xbuffer;
  va_list  arg;

  xbuffer.Add((*this));

  va_start(arg, frompos);

  bool status = ExtractXBufferWithMask(xbuffer, mask, arg);

  va_end(arg);

  return status;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Set(XBYTE* pbuffer, int psize, int topos)
* @brief      Set array of bytes into buffer
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 20:57:53
*
* @param[in]  pbuffer : pointer of buffer to set
* @param[in]  psize : size of buffer to set.
* @param[in]  topos : position of buffer to set
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Set(XBYTE* pbuffer, int psize, int topos)
{
  SetBlocked(true);

  int pos = topos;
  if(topos == XBUFFER_INVALIDPOSITION) pos = position;

  if((pos+psize) > (int)size)
    {
      SetBlocked(false);
      return false;
    }

  if(pbuffer)
          memcpy(&buffer[pos], pbuffer, psize);
    else  memset(&buffer[pos], 0, psize);

  if(pos==(int)position)  position += psize;

  SetBlocked(false);

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Set(XBYTE data, int topos)
* @brief      Set byte to buffer
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 21:01:32
*
* @param[in]  data : byte to set
* @param[in]  topos : position of buffer to set
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Set(XBYTE data, int topos)
{
  SetBlocked(true);

  int pos = topos;
  if(topos == XBUFFER_INVALIDPOSITION) pos = position;

  if(pos>(int)size)
    {
      SetBlocked(false);
      return false;
    }

  buffer[pos]= data;

  if(pos==(int)position) position++;

  SetBlocked(false);

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Set(bool data, int topos)
* @brief      Set boolean to buffer
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 21:03:56
*
* @param[in]  data : boolean to set
* @param[in]  topos : position of buffer to set
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Set(bool data, int topos)
{
  XBYTE _data = data?1:0;

  return Set(_data, topos);

}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Set(XWORD data, int topos)
* @brief      Set WORD to buffer. (16 bits) 
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 21:04:45
*
* @param[in]  data : WORD to set
* @param[in]  topos : position of buffer to set
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Set(XWORD data, int topos)
{
  if(hardwareuselittleendian) SWAPWORD(data);

  return Set((XBYTE*)&data, sizeof(XWORD), topos);
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Set(XDWORD data, int topos)
* @brief      Set double WORD to buffer. (32 bits) 
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 21:06:16
*
* @param[in]  data : DWORD to set
* @param[in]  topos : position of buffer to set
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Set(XDWORD data, int topos)
{
  if(hardwareuselittleendian) SWAPDWORD(data);

  return Set((XBYTE*)&data, sizeof(XDWORD), topos);
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Set(XQWORD data, int topos)
* @brief      Set quad WORD to buffer. (64 bits) 
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 21:09:17
*
* @param[in]  data : QWORD to set
* @param[in]  topos : position of buffer to set
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Set(XQWORD data, int topos)
{
  if(hardwareuselittleendian) SWAPQWORD(data);

  return Set((XBYTE*)&data, sizeof(XQWORD), topos);
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Set(float data, int topos)
* @brief      Set float buffer. 
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 21:10:17
*
* @param[in]  data : float to set 
* @param[in]  topos : position of buffer to set
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Set(float data, int topos)
{
  return Set((XBYTE*)&data, sizeof(XDWORD), topos);
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Set(double data, int topos)
* @brief      Set double to set
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 21:15:35
*
* @param[in]  data : double to set
* @param[in]  topos : position of buffer to set
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Set(double data, int topos)
{
  return Set((XBYTE*)&data, sizeof(XQWORD), topos);
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Set(XSTRING& data, int topos)
* @brief      Set string to buffer (normalizes string before set)
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 21:16:28
*
* @param[in]  data : string to set 
* @param[in]  topos : position of buffer to set 
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Set(XSTRING& data, int topos)
{
  // should create temporary array and then put that
  // but on stack it's too much?

  XDWORD* tmp = new XDWORD[data.GetSize()];
  if(!tmp) return false;

  for(XDWORD i = 0; i < data.GetSize(); i++)
    {
      XDWORD _data = data[i];
      if(hardwareuselittleendian) SWAPDWORD(_data);
      tmp[i] = _data;
    }

  if(!Set((XBYTE*)tmp, data.GetSize()*sizeof(XDWORD), topos)) return false;

  delete [] tmp;

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD XBUFFER::SetWithMask(XCHAR* mask, int topos, ...)
* @brief      Set variant data with a mask
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 21:18:38
*
* @param[in]  mask : pointer XCHAR with a mask to use in extract
* @param[in]  topos : position of buffer to set
* @param[in]  ... : params with a variant data
*
* @return     XDWORD : size of byte set
*
*---------------------------------------------------------------------------------------------------------------------*/
XDWORD XBUFFER::SetWithMask(XCHAR* mask, int topos, ...)
{
  if(!mask) return 0;

  XSTRING  _mask  = mask;
  XBUFFER  xbuffer;
  va_list  arg;

  va_start(arg, topos);

  AddXBufferWithMask(xbuffer, &_mask, arg);

  va_end(arg);

  Delete();
  Add(xbuffer);

  return xbuffer.GetSize();
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD XBUFFER::SetWithMask(XSTRING* mask, int topos, ...)
* @brief      Set variant data with a mask
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 21:21:59
*
* @param[in]  mask : string with a mask to use in extract
* @param[in]  topos : position of buffer to set
* @param[in]  ... : params with a variant data
*
* @return     XDWORD : size of byte set
*
*---------------------------------------------------------------------------------------------------------------------*/
XDWORD XBUFFER::SetWithMask(XSTRING* mask, int topos, ...)
{
  XBUFFER  xbuffer;
  va_list  arg;

  va_start(arg, topos);

  AddXBufferWithMask(xbuffer, mask, arg);

  va_end(arg);

  Delete();
  Add(xbuffer);

  return xbuffer.GetSize();
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Resize(XDWORD newsize, bool setblocked)
* @brief      Resize buffer
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 21:25:50
*
* @param[in]  newsize : new size of bufffer.
* @param[in]  setblocked : blocked this operation.
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Resize(XDWORD newsize, bool setblocked)
{
  bool status = true;

  if(setblocked) SetBlocked(true);

  if(!newsize)
    {
      size     = 0;
      position = 0;

      if(sizeassign) memset(buffer, 0,  sizeassign);
    }
   else
    {
      XBYTE* newbuffer;

      if(newsize > sizeassign)
        {
          newbuffer = new XBYTE[newsize];
          if(newbuffer)
            {
              memset(newbuffer , 0,  newsize);

              if(buffer)
                {
                  memcpy(newbuffer , buffer, size);

                  delete[] buffer;

                  buffer   = NULL;
                  size     = 0;
                  position = 0;
                }

              buffer      = newbuffer;
              sizeassign  = newsize;
              size        = newsize;
              position    = 0;
            }
           else
            {
              status = false;
            }
        }
       else
        {
          size        = newsize;
          position    = 0;

          if(sizeassign > size)
            {
              memset(&buffer[size], 0, (sizeassign-size));
            }
        }

      if(size)
        {
          if(position>=size) position = (size-1);
        }
   }

  if(setblocked) SetBlocked(false);

  return status;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Delete(bool setblocked)
* @brief      Delete buffer
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 21:26:55
*
* @param[in]  setblocked : blocked this operation.
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Delete(bool setblocked)
{
  if(setblocked) SetBlocked(true);

  if(buffer) delete[] buffer;

  buffer        = NULL;
  size          = 0;
  sizeassign    = 0;
  position      = 0;

  if(setblocked) SetBlocked(false);

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::FillBuffer(XBYTE fillchar)
* @brief      Fill the buffer with a byte
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 18:40:52
*
* @param[in]  fillchar : byte to fill buffer
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::FillBuffer(XBYTE fillchar)
{
  SetBlocked(true);

  bool status = false;

  if(buffer && size)
    {
      memset(buffer, fillchar,  size);
      status = true;
    }

  SetBlocked(false);

  return status;
}



 /**-------------------------------------------------------------------------------------------------------------------
 *
 *  @fn         bool XBUFFER::Empty()
 *  @brief      Delete Buffer (set blocked default)
 *  @ingroup    UTILS
 *
 *  @author     Abraham J. Velez 
 *  @date       27/05/2018 10:48:49
 *
 *  @return     bool : true if is succesful. 
 *
 *---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Empty()
 { 
   return Delete();                                               
 }



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Swap()
* @brief      Swap buffer (last to first byte and viceversa)
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 21:28:12
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Swap()
{
  SetBlocked(true);

  for(int c=0; c<(int)(size/2); c++)
    {
      XBYTE tembyte;

      tembyte          = buffer[c];
      buffer[c]        = buffer[size-c-1];
      buffer[size-c-1] = tembyte;
    }

  SetBlocked(false);

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Compare(XBYTE* pbuffer,XDWORD psize)
* @brief      Compare buffer to array of bytes
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 20:06:13
*
* @param[in]  pbuffer : pointer to buffer to compare
* @param[in]  psize : size of buffer to compare
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Compare(XBYTE* pbuffer,XDWORD psize)
{
  if(!pbuffer)  return false;
  if(!psize)    return false;

  bool status;

  SetBlocked(true);

  if(psize != size)
    {
      SetBlocked(false);
      return false;
    }

  status = (memcmp(pbuffer, buffer, psize) == 0);

  SetBlocked(false);

  return status;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Compare(XBUFFER* buffer)
* @brief      Compare buffer to buffer 
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 20:08:05
*
* @param[in]  buffer : pointer to buffer
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Compare(XBUFFER* buffer)
{
  if(!buffer) return false;

  return Compare(buffer->Get(), buffer->GetSize());
}




/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Padding_Add(XBYTE bitsadjust, XBUFFER_PADDINGTYPE type)
* @brief      Add Padding (bits) to buffer
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 19:26:25
*
* @param[in]  bitsadjust : n bits of padding
* @param[in]  type : type of padding
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Padding_Add(XBYTE bitsadjust, XBUFFER_PADDINGTYPE type)
{
  if(paddinghas) return false;

  paddingbitadjust = bitsadjust;
  paddingsize     = bitsadjust - (size % bitsadjust);

  switch(type)
    {
      case XBUFFER_PADDINGTYPE_ZEROS        :
      case XBUFFER_PADDINGTYPE_SPACES       :
      case XBUFFER_PADDINGTYPE_PKCS7        :
      case XBUFFER_PADDINGTYPE_PKCS5        : for(int c=0;c<paddingsize;c++)
                                                {
                                                  switch(type)
                                                    {
                                                      case XBUFFER_PADDINGTYPE_ZEROS        : Add((XBYTE)0x00);
                                                                                              break;

                                                      case XBUFFER_PADDINGTYPE_SPACES       : Add((XBYTE)0x20);
                                                                                              break;

                                                      case XBUFFER_PADDINGTYPE_PKCS7        :
                                                      case XBUFFER_PADDINGTYPE_PKCS5        : Add((XBYTE)paddingsize);
                                                                                              break;
                                                      default: break;
                                                    }
                                                }
                                              break;

      case XBUFFER_PADDINGTYPE_ANSIX923     : for(int c=0;c<paddingsize-1;c++)
                                                {
                                                  Add((XBYTE)0x00);
                                                }
                                              Add((XBYTE)paddingsize);
                                              break;

      case XBUFFER_PADDINGTYPE_ISO10126     : { XRAND* xrand = xfactory->CreateRand();
                                                if(!xrand) return false;

                                                for(int c=0;c<paddingsize-1;c++)
                                                  {
                                                    Add((XBYTE)0x00);
                                                  }

                                                Add((XBYTE)paddingsize);
                                                xfactory->DeleteRand(xrand);
                                              }
                                              break;

      case XBUFFER_PADDINGTYPE_ISOIEC7816_4 : Add((XBYTE)0x80);
                                              for(int c=0;c<paddingsize-1;c++)
                                                {
                                                  Add((XBYTE)0x00);
                                                }
                                              break;
      default: break;
    }

  paddinghas      = true;

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Padding_Has()
* @brief      Check if buffer has padding
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 19:28:36
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Padding_Has()
{
  return paddinghas;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XBUFFER_PADDINGTYPE XBUFFER::Padding_GetType()
* @brief      GetType of padding
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 19:28:59
*
* @return     XBUFFER_PADDINGTYPE : type of padding 
*
*---------------------------------------------------------------------------------------------------------------------*/
XBUFFER_PADDINGTYPE XBUFFER::Padding_GetType()
{
  return paddingtype;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XBYTE XBUFFER::Padding_GetSize()
* @brief      Get size of padding
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 19:30:57
*
* @return     XBYTE : size of padding
*
*---------------------------------------------------------------------------------------------------------------------*/
XBYTE XBUFFER::Padding_GetSize()
{
  return paddingsize;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::Padding_Delete()
* @brief      Delete padding
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 19:31:35
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Padding_Delete()
{
  if(!paddinghas) return false;

  int newsize = GetSize() - Padding_GetSize();

  Resize(newsize , true);

  if((int)GetSize() != newsize)  return false;

  paddinghas        = false;
  paddingbitadjust    = 0;
  paddingsize       = 0;

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::AddNBits(XWORD data)
* @brief      Add bits to buffer
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 19:00:57
*
* @param[in]  data : 16bits to add
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::AddNBits(XWORD data)
{
  if(!lastnbitsfree)
    {
      XWORD _data = data;

      _data <<=nbitsfree;
      lastnbitsfree = nbitsfree;

      Add((XWORD)_data);
    }
   else
    {
      SetBlocked(true);

      XWORD _data[2];

      lastnbitsfree = nbitsfree - (8-lastnbitsfree);

      _data[0] = (buffer[size-1]<<8);
      _data[1] = (data<<lastnbitsfree);

      _data[0] |= _data[1];

      buffer[size-1] = (XBYTE)(_data[0]>>8);

      SetBlocked(false);

      Add((XBYTE)_data[0]);
    }


  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::AdjustToNBits(int nbits)
* @brief      Ajust buffer to nbits.
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 21:29:28
*
* @param[in]  nbits : nbits to adjust
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::AdjustToNBits(int nbits)
{
  this->nbits     = nbits;
  this->nbitsfree = (16-nbits);

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XBYTE XBUFFER::GetNBits()
* @brief      Get NBits used
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       27/05/2018 10:51:59
*
* @return     XBYTE : n bits used
*
*---------------------------------------------------------------------------------------------------------------------*/
XBYTE XBUFFER::GetNBits()
{ 
  return nbits;                                                  
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::SetNBits(XWORD data, int topos)
* @brief      Set WORD in bits to buffer.
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 21:24:06
*
* @param[in]  data : 16 bits WORD to set
* @param[in]  topos : position of buffer to set
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::SetNBits(XWORD data, int topos)
{
  if(!lastnbitsfree)
    {
      XWORD _data = data;

      _data <<=nbitsfree;
      lastnbitsfree = nbitsfree;

      Set((XWORD)_data);
    }
   else
    {
      SetBlocked(true);

      XWORD _data[2];

      lastnbitsfree = nbitsfree - (8-lastnbitsfree);

      _data[0] = (buffer[position-1]<<8);
      _data[1] = (data<<lastnbitsfree);

      _data[0] |= _data[1];

      buffer[position-1] = (XBYTE)(_data[0]>>8);

      SetBlocked(false);

      Set((XBYTE)_data[0], topos);
    }

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XBYTE XBUFFER::GetNBitsFree()
* @brief      Get Nbits free in the buffer
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 21:31:24
*
* @return     XBYTE : number bits free.
*
*---------------------------------------------------------------------------------------------------------------------*/
XBYTE XBUFFER::GetNBitsFree()
{
  return lastnbitsfree;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::SetNBitsFree(XBYTE nbitsfree)
* @brief      Set NBits free
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 21:34:19
*
* @param[in]  nbitsfree : number bits free.
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::SetNBitsFree(XBYTE nbitsfree)
{
  lastnbitsfree = nbitsfree;

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::ConvertFromBase64(XSTRING& stringbase64)
* @brief      Convert from string in Base 64.
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 21:34:58
*
* @param[in]  stringbase64 : string origin to convert.
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::ConvertFromBase64(XSTRING& stringbase64)
{
  XSTRING stringbin;

  Delete();

  stringbin.ConvertFromBase64(stringbase64);

  if(stringbin.IsEmpty()) return false;

  for(int c=0; c<(int)stringbin.GetSize(); c++)
    {
      XBYTE data = (XBYTE)stringbin.Get()[c];

      Add(data);
    }

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::ConvertToBase64(XSTRING& stringbase64)
* @brief      Set String to base 64 from buffer
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 21:36:57
*
* @param[out] stringbase64 : string in base 64.
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::ConvertToBase64(XSTRING& stringbase64)
{
  XSTRING stringbin;

  stringbase64.Empty();

  stringbin.Add((*this));
  stringbin.ConvertToBase64(stringbase64);

  return stringbase64.GetSize()?true:false;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD XBUFFER::DecodeBCD(XDWORD ppos, XDWORD psize)
* @brief      Decode DWORD from buffer BCD
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 21:38:25
*
* @param[in]  ppos : position of DWORD
* @param[in]  psize : size of decode
*
* @return     XDWORD : result od decode.
*
*---------------------------------------------------------------------------------------------------------------------*/
XDWORD XBUFFER::DecodeBCD(XDWORD ppos, XDWORD psize)
{
  XDWORD result   = 0;
  int    position = ppos;

  while(psize--)
    {
      XBYTE nibble1 = ( buffer[position] & 0x0F);
      XBYTE nibble2 = ((buffer[position] & 0xF0)>>4);

      if(nibble1 > 9)  return 0;
      if(nibble2 > 9)  return 0;

      result = (result * 100) + ((buffer[position] >> 4) * 10) + (buffer[position] & 0x0F);
      position++;
    }

  return result;
}




/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XQWORD XBUFFER::DecodeBCDLong(XDWORD ppos,XDWORD psize)
* @brief      Decode long from buffer BCD
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 21:44:07
*
* @param[in]  ppos : 
* @param[in]  psize : 
*
* @return     XQWORD : 
*
*---------------------------------------------------------------------------------------------------------------------*/
XQWORD XBUFFER::DecodeBCDLong(XDWORD ppos,XDWORD psize)
{
  XQWORD result   = 0;
  int    position = ppos;

  while(psize--)
    {
      XBYTE nibble1 = ( buffer[position] & 0x0F);
      XBYTE nibble2 = ((buffer[position] & 0xF0)>>4);

      if(nibble1 > 9)  return 0;
      if(nibble2 > 9)  return 0;

      result  = (result * 100);
      result += ((buffer[position] >> 4) * 10);
      result += (buffer[position] & 0x0F);

      position++;
    }

  return result;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::AddXBufferWithMask(XBUFFER& xbuffer, XSTRING* mask, va_list& arg)
* @brief      Add buffer with MASK (INTERNAL)
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 21:45:18
*
* @param[out] xbuffer : buffer obtain
* @param[in]  mask : string mask to obtain buffer
* @param[in]  arg : arguments to create buffer
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::AddXBufferWithMask(XBUFFER& xbuffer, XSTRING* mask, va_list& arg)
{
  if(!mask)           return false;
  if(mask->IsEmpty()) return false;

  int c = 0;

  while(mask->Get()[c])
    {
      switch(mask->Get()[c])
        {
          case 'W': { XWORD wvar  = (XWORD)va_arg(arg, int);
                      xbuffer.Add((XWORD)wvar);
                    }
                    break;

          case 'B': { XBYTE bvar  = (XBYTE)va_arg(arg, int);
                      xbuffer.Add((XBYTE)bvar);
                    }
                    break;

          case 'D': { XDWORD dvar = va_arg(arg, XDWORD);
                      xbuffer.Add((XDWORD)dvar);
                    }
                    break;

          case 'Q': { XQWORD dvar = va_arg(arg, XQWORD);
                      xbuffer.Add((XQWORD)dvar);
                    }
                    break;

          case 'F': { float fvar = (float)va_arg(arg, double);
                      xbuffer.Add((float)fvar);
                    }
                    break;

          case 'L': { double lvar = va_arg(arg, double);
                      xbuffer.Add((double)lvar);
                    }
                    break;

          case 'O': { int _bvar = va_arg(arg, int);
                      XBYTE bvar  = (_bvar?1:0);
                      xbuffer.Add((XBYTE)bvar);
                    }
                    break;

          case 'A': { XBYTE* avar = va_arg(arg, XBYTE*);
                      int    size;
                      c++;

                      if(mask->Get()[c]==__C('X'))
                        {
                          size = va_arg(arg, XDWORD);
                        }
                       else
                        {
                          XSTRING sizestr;

                          while((mask->Get()[c]>=__C('0')) && (mask->Get()[c]<=__C('9')))
                            {
                              sizestr.Add(mask->Get()[c]);
                              c++;
                            }

                          c--;

                          size = sizestr.ConvertToInt();
                        }

                      if(size && avar) xbuffer.Add(avar, size);
                    }
                    break;

          case 'S': { XSTRING* svar = va_arg(arg, XSTRING*);
                      XDWORD size = (svar->GetSize()+1)*sizeof(XWORD);

                      XSTRING_CREATENORMALIZE((*svar), buffnormalize, false)
                      xbuffer.Add((XBYTE*)buffnormalize, size);
                      XSTRING_DELETENORMALIZE(buffnormalize)
                    }
                    break;

          case 'V': { XVARIANT* svar        = va_arg(arg, XVARIANT*);
                      XDWORD    type        = svar->GetType();
                      XBYTE     isnull      = svar->IsNull();
                      XDWORD    sizevariant = svar->GetSize();

                      xbuffer.Add((XDWORD)type);
                      xbuffer.Add((XBYTE)isnull);
                      xbuffer.Add((XDWORD)sizevariant);

                      if(svar->GetData() && sizevariant) xbuffer.Add((XBYTE*)svar->GetData(), sizevariant);
                    }
                    break;

        }

       c++;
    }

  return xbuffer.GetSize()?true:false;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFER::ExtractXBufferWithMask(XBUFFER& xbuffer, XSTRING* mask, va_list& arg)
* @brief      Extract from buffer with MASK (INTERNAL)
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 21:47:44
*
* @param[in]  xbuffer : buffer to extract
* @param[in]  mask :  string mask to extract buffer
* @param[in]  arg : arguments to create buffer
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::ExtractXBufferWithMask(XBUFFER& xbuffer, XSTRING* mask, va_list& arg)
{
  if(!mask)           return false;
  if(mask->IsEmpty()) return false;

  int c = 0;

  while(mask->Get()[c])
    {
      switch(mask->Get()[c])
        {
          case 'B': { XBYTE* bvar  = va_arg(arg, XBYTE*);
                      xbuffer.Extract((XBYTE&)(*bvar));
                    }
                    break;

          case 'W': { XWORD* wvar  = va_arg(arg, XWORD*);
                      xbuffer.Extract((XWORD&)(*wvar));
                    }
                    break;

          case 'D': { XDWORD* dvar = va_arg(arg, XDWORD*);
                      xbuffer.Extract((XDWORD&)(*dvar));
                    }
                    break;

          case 'Q': { XQWORD* dvar = va_arg(arg, XQWORD*);
                      xbuffer.Extract((XQWORD&)(*dvar));
                    }
                    break;

          case 'F': { double* fvar = va_arg(arg, double*);
                      xbuffer.Extract((float&)(*fvar), 0);
                    }
                    break;

          case 'L': { double* dvar = va_arg(arg, double*);
                      xbuffer.Extract((double&)(*dvar), 0);
                    }
                    break;

          case 'O': { bool*  bvar  = va_arg(arg, bool*);
                      XBYTE _bvar  = 0;
                      xbuffer.Extract(_bvar);
                      (*bvar) = _bvar?true:false;
                    }
                    break;

          case 'A': { XBYTE* avar = va_arg(arg, XBYTE*);

                      XDWORD size = 0;
                      c++;

                      if(mask->Get()[c]==__C('X'))
                        {
                          XDWORD* dvar = va_arg(arg, XDWORD*);

                          if(dvar) size = *dvar;
                        }
                       else
                        {
                          XSTRING sizestr;

                          while((mask->Get()[c]>=__C('0')) && (mask->Get()[c]<=__C('9')))
                            {
                              sizestr.Add(mask->Get()[c]);
                              c++;
                            }
                          c--;
                          size = sizestr.ConvertToInt();
                        }

                      if(size && avar) xbuffer.Extract(avar, 0, size);
                    }
                    break;

          case 'S': { XSTRING* svar = va_arg(arg, XSTRING*);

                      svar->Set((XWORD*)(xbuffer.Get()));
                      XDWORD size = (svar->GetSize()+1)*sizeof(XWORD);

                      xbuffer.Extract(NULL, 0, size);
                    }
                    break;

          case 'V': { XVARIANT* svar        = va_arg(arg, XVARIANT*);
                      XDWORD    type        = 0;
                      XBYTE     isnull      = svar->IsNull();
                      XDWORD    sizevariant = 0;

                      xbuffer.Extract(type);
                      xbuffer.Extract(isnull);
                      xbuffer.Extract(sizevariant);


                      // Imanol : if xbuffer.GetSize() - 9 != sizevariant something went wront and this variant might not be complete(but a full data array is allocated anyways)
                      svar->Set((XVARIANT_TYPE)type, xbuffer.Get(), sizevariant);
                      if(sizevariant) xbuffer.Extract(NULL, 0, sizevariant);
                    }
                    break;
        }

       c++;
    }

  return xbuffer.IsEmpty();
}




/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XBUFFER::Clean()
* @brief      Clean the attributes of the class: Default initialice
* @note       INTERNAL
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       29/05/2018 13:12:19
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XBUFFER::Clean()
{
  buffer              = NULL;
  size                = 0;
  sizeassign          = 0;
  position            = 0;

  nbits               = 0;
  nbitsfree           = 0;
  lastnbitsfree       = 0;

  paddingtype         = XBUFFER_PADDINGTYPE_NONE;
  paddinghas          = false;
  paddingbitadjust    = 0;
  paddingsize         = 0;

  xmutex              = NULL;
}