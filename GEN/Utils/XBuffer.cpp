//------------------------------------------------------------------------------------------
//  XBUFFER.CPP
//
//  eXtended buffer
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 10/02/2004 17:44:22
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>

#include "XFactory.h"
#include "XTimer.h"
#include "XVariant.h"

#include "XBuffer.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------

bool XBUFFER::hardwareuselittleendian = true;

//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  XBUFFER::XBUFFER
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/02/2007 12:13:55
//
//  @return

//  */
//-------------------------------------------------------------------
XBUFFER::XBUFFER(bool threadsafe)
{
  Clean();

  if(threadsafe && xfactory) xmutex = xfactory->Create_Mutex();
}


//-------------------------------------------------------------------
//  XBUFFER::XBUFFER
/**
//
//
//  @author       Abraham J. Velez
//  @version      10/02/2004 17:45:42
//
//  @return

//  @param        size :
*/
//-------------------------------------------------------------------
XBUFFER::XBUFFER(XDWORD size, bool threadsafe)
{
  Clean();

  if(threadsafe && xfactory) xmutex = xfactory->Create_Mutex();

  Resize(size);
  FillBuffer();
}



//-------------------------------------------------------------------
//  XBUFFER::~XBUFFER
/**
//
//
//  @author       Abraham J. Velez
//  @version      10/02/2004 17:46:06
//
//  @return
//  */
//-------------------------------------------------------------------
XBUFFER::~XBUFFER()
{
  Delete();

  if(xmutex) xfactory->Delete_Mutex(xmutex);

  Clean();
}




//-------------------------------------------------------------------
//  XBUFFER::GetSize
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/02/2007 12:48:16
//
//  @return       XDWORD :
//  */
//-------------------------------------------------------------------
XDWORD XBUFFER::GetSize()
{
  SetBlocked(true);

  XDWORD rsize = size;

  SetBlocked(false);

  return rsize;
}



/*-------------------------------------------------------------------
//  XBUFFER::SetSize
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/08/2011 22:50:40
//
//  @return       bool :
//  @param        size :
*/
/*-----------------------------------------------------------------*/
bool XBUFFER::SetSize(XDWORD size)
{
  SetBlocked(true);

  this->size = size;

  SetBlocked(false);

  return true;
}



//-------------------------------------------------------------------
//  XBUFFER::ResetPosition
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/02/2007 18:09:51
//
//  @return       void :
//  */
//-------------------------------------------------------------------
void XBUFFER::ResetPosition()
{
  SetPosition(0);
}


//-------------------------------------------------------------------
//  XBUFFER::GetPosition
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/02/2007 12:46:17
//
//  @return       XDWORD :
//  */
//-------------------------------------------------------------------
XDWORD XBUFFER::GetPosition()
{
  return position;
}


//-------------------------------------------------------------------
//  XBUFFER::SetPosition
/**
//
//
//  @author       Abraham J. Velez
//  @version      24/11/2004 12:31:49
//
//  @return       bool :
//  @param        position :
//  */
//-------------------------------------------------------------------
bool XBUFFER::SetPosition(XDWORD position)
{
  this->position = position;

  return true;
}


/*-------------------------------------------------------------------
//  XBUFFER::IsBlocked
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      08/12/2012 11:21:29
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XBUFFER::IsBlocked()
{
  if(!xmutex) return false;

  return xmutex->IsLock();
}



/*-------------------------------------------------------------------
//  XBUFFER::SetBlocked
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/08/2011 22:55:40
//
//  @return       bool :
//  @param        blocked :
*/
/*-----------------------------------------------------------------*/
bool XBUFFER::SetBlocked(bool blocked)
{
  if(!xmutex) return false;

  return (blocked)?xmutex->Lock():xmutex->UnLock();
}




//-------------------------------------------------------------------
//  XBUFFER::FillBuffer
/**
//
//
//  @author       Abraham J. Velez
//  @version      24/11/2004 12:22:48
//
//  @return       bool :
//  @param        fillchar :
*/
//-------------------------------------------------------------------
bool XBUFFER::FillBuffer(XBYTE fillchar)
{
  SetBlocked(true);

  bool status = false;

  if(buffer && size)
    {
      memset(buffer , fillchar,  size);
      status = true;
    }

  SetBlocked(false);

  return status;
}



//-------------------------------------------------------------------
//  XBUFFER::Add
/**
//
//
//  @author       Abraham J. Velez
//  @version      24/11/2004 12:22:48
//
//  @return       bool :
//  @param        pbuffer :
//  @param        psize :
*/
//-------------------------------------------------------------------
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



//-------------------------------------------------------------------
//  XBUFFER::Add
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/02/2007 14:29:13
//
//  @return       bool :
//  @param        buffer :
*/
//-------------------------------------------------------------------
bool XBUFFER::Add(XBUFFER* buffer)
{
  return Add(buffer->buffer, buffer->size);
}




/*-------------------------------------------------------------------
//  XBUFFER::Add
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/12/2012 16:01:49
//
//  @return       bool :
//  @param        buffer :
*/
/*-----------------------------------------------------------------*/
bool XBUFFER::Add(XBUFFER& buffer)
{
  return Add(buffer.buffer, buffer.size);
}




/*-------------------------------------------------------------------
//  XBUFFER::Add
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      18/05/2011 10:05:30
//
//  @return       bool :
//  @param        data :
*/
/*-----------------------------------------------------------------*/
bool XBUFFER::Add(XBYTE data)
{
  return Add((XBYTE*)&data, sizeof(XBYTE));
}



/*-------------------------------------------------------------------
//   XBUFFER::Add
*/
/**
//
//
//
//  @author   Abraham J. Velez
//  @version  08/01/2018 16:11:08
//  @return   bool :
//
//  @param    bool :
//
*//*-----------------------------------------------------------------*/
bool XBUFFER::Add(bool data)
{
  XBYTE _data = data?1:0;

  return Add((XBYTE*)&_data, sizeof(XBYTE));
}




/*-------------------------------------------------------------------
//  XBUFFER::Add
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      19/11/2012 23:27:58
//
//  @return       bool :
//  @param        data :
*/
/*-----------------------------------------------------------------*/
bool XBUFFER::Add(XWORD data)
{
  if(hardwareuselittleendian) SWAPWORD(data);

  return Add((XBYTE*)&data,sizeof(XWORD));
}



/*-------------------------------------------------------------------
//  XBUFFER::Add
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      19/11/2012 23:28:11
//
//  @return       bool :
//  @param        data :
//  @param        checkendian :
*/
/*-----------------------------------------------------------------*/
bool XBUFFER::Add(XDWORD data)
{
  if(hardwareuselittleendian) SWAPDWORD(data);

  return Add((XBYTE*)&data, sizeof(XDWORD));
}



/*-------------------------------------------------------------------
//  XBUFFER::Add
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      14/03/2016 12:53:24
//
//  @return       bool :
//
//  @param        data :
*/
/*-----------------------------------------------------------------*/
bool XBUFFER::Add(XQWORD data)
{
  if(hardwareuselittleendian) SWAPQWORD(data);

  return Add((XBYTE*)&data, sizeof(XQWORD));
}




/*-------------------------------------------------------------------
//  XBUFFER::Add
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      14/03/2016 12:54:17
//
//  @return       bool :
//
//  @param        data :
*/
/*-----------------------------------------------------------------*/
bool XBUFFER::Add(float data)
{
  return Add((XBYTE*)&data, sizeof(XDWORD));
}



/*-------------------------------------------------------------------
//  XBUFFER::Add
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      14/03/2016 12:54:25
//
//  @return       bool :
//
//  @param        data :
*/
/*-----------------------------------------------------------------*/
bool XBUFFER::Add(double data)
{
  return Add((XBYTE*)&data, sizeof(XQWORD));
}




/*-------------------------------------------------------------------
//  XBUFFER::Add
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      16/05/2014 10:24:13
//
//  @return       bool :
//
//  @param        string :
*/
/*-----------------------------------------------------------------*/
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



/*-------------------------------------------------------------------
//  XBUFFER::AddWithMask
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      05/12/2016 16:26:19
//
//  @return       bool :
//
//  @param        mask :
//  @param        ... :
*/
/*-----------------------------------------------------------------*/
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





/*-------------------------------------------------------------------
//  XBUFFER::AddWithMask
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      16/12/2012 1:18:09
//
//  @return       bool :
//  @param        data :
*/
/*-----------------------------------------------------------------*/
bool XBUFFER::AddWithMask(XSTRING* mask,...)
{
  XBUFFER  xbuffer;
  va_list  arg;

  va_start(arg, mask);

  AddXBufferWithMask(xbuffer, mask, arg);

  va_end(arg);

  return Add(xbuffer);
}





/*-------------------------------------------------------------------
//  XBUFFER::AddNBits
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      16/12/2012 1:18:09
//
//  @return       bool :
//  @param        data :
*/
/*-----------------------------------------------------------------*/
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




/*-------------------------------------------------------------------
//  XBUFFER::Insert
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      18/09/2015 15:48:43
//
//  @return       bool :
//
//  @param        pbuffer :
//  @param        psize :
//  @param        frompos :
*/
/*-----------------------------------------------------------------*/
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




/*-------------------------------------------------------------------
//  XBUFFER::Insert
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      18/09/2015 15:48:53
//
//  @return       bool :
//
//  @param        buffer :
//  @param        frompos :
*/
/*-----------------------------------------------------------------*/
bool XBUFFER::Insert(XBUFFER* xbuffer, int frompos)
{
  if(!xbuffer)  return false;

  return Insert(xbuffer->Get(), xbuffer->GetSize(), frompos);
}



/*-------------------------------------------------------------------
//  XBUFFER::Insert
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      18/09/2015 15:49:02
//
//  @return       bool :
//
//  @param        buffer :
//  @param        frompos :
*/
/*-----------------------------------------------------------------*/
bool XBUFFER::Insert(XBUFFER& xbuffer, int frompos)
{
  return Insert(xbuffer.Get(), xbuffer.GetSize(), frompos);
}



/*-------------------------------------------------------------------
//  XBUFFER::Insert
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      18/09/2015 15:49:09
//
//  @return       bool :
//
//  @param        data :
//  @param        frompos :
*/
/*-----------------------------------------------------------------*/
bool XBUFFER::Insert(XBYTE data, int frompos)
{
  return Insert(&data, 1, frompos);
}



/*-------------------------------------------------------------------
//   XBUFFER::Insert
*/
/**
//
//
//
//  @author   Abraham J. Velez
//  @version  08/01/2018 16:14:43
//  @return   bool :
//
//  @param    bool :
//  @param    int :
//
*//*-----------------------------------------------------------------*/
bool XBUFFER::Insert(bool data, int frompos)
{
  XBYTE _data = data?1:0;

  return Insert(&_data, 1, frompos);
}



/*-------------------------------------------------------------------
//  XBUFFER::Insert
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      18/09/2015 15:49:16
//
//  @return       bool :
//
//  @param        data :
//  @param        frompos :
*/
/*-----------------------------------------------------------------*/
bool XBUFFER::Insert(XWORD data, int frompos)
{
  if(hardwareuselittleendian) SWAPWORD(data);

  return Insert((XBYTE*)&data, sizeof(XWORD), frompos);
}



/*-------------------------------------------------------------------
//  XBUFFER::Insert
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      18/09/2015 15:49:21
//
//  @return       bool :
//
//  @param        data :
//  @param        frompos :
*/
/*-----------------------------------------------------------------*/
bool XBUFFER::Insert(XDWORD data, int frompos)
{
  if(hardwareuselittleendian) SWAPDWORD(data);

  return Insert((XBYTE*)&data, sizeof(XDWORD), frompos);
}



/*-------------------------------------------------------------------
//  XBUFFER::Insert
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      14/03/2016 13:02:40
//
//  @return       bool :
//
//  @param        data :
//  @param        frompos :
*/
/*-----------------------------------------------------------------*/
bool XBUFFER::Insert(XQWORD data, int frompos)
{
  if(hardwareuselittleendian) SWAPQWORD(data);

  return Insert((XBYTE*)&data, sizeof(XQWORD), frompos);
}



/*-------------------------------------------------------------------
//  XBUFFER::Insert
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      14/03/2016 13:02:45
//
//  @return       bool :
//
//  @param        data :
//  @param        frompos :
*/
/*-----------------------------------------------------------------*/
bool XBUFFER::Insert(float data, int frompos)
{
  return Insert((XBYTE*)&data, sizeof(XDWORD), frompos);
}



/*-------------------------------------------------------------------
//  XBUFFER::Insert
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      14/03/2016 13:02:50
//
//  @return       bool :
//
//  @param        data :
//  @param        frompos :
*/
/*-----------------------------------------------------------------*/
bool XBUFFER::Insert(double data, int frompos)
{
  return Insert((XBYTE*)&data, sizeof(XQWORD), frompos);
}



/*-------------------------------------------------------------------
//  XBUFFER::Insert
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      18/09/2015 15:49:27
//
//  @return       bool :
//
//  @param        string :
//  @param        frompos :
*/
/*-----------------------------------------------------------------*/
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




/*-------------------------------------------------------------------
//  XBUFFER::InsertWithMask
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      05/12/2016 16:37:23
//
//  @return       bool :
//
//  @param        mask :
//  @param        frompos :
//  @param        ... :
*/
/*-----------------------------------------------------------------*/
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




/*-------------------------------------------------------------------
//  XBUFFER::InsertWithMask
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      05/03/2016 17:51:18
//
//  @return       bool :
//
//  @param        mask :
//  @param        frompos :
//  @param        ... :
*/
/*-----------------------------------------------------------------*/
bool XBUFFER::InsertWithMask(XSTRING* mask, int frompos, ...)
{
  XBUFFER  xbuffer;
  va_list  arg;

  va_start(arg, frompos);

  AddXBufferWithMask(xbuffer, mask, arg);

  va_end(arg);

  return Insert(xbuffer, frompos);
}





/*-------------------------------------------------------------------
//  XBUFFER::Padding_Add
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      14/03/2013 19:25:39
//
//  @return       bool :
//  @param        bitsadjust :
//  @param        type :
*/
/*-----------------------------------------------------------------*/
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




/*-------------------------------------------------------------------
//  XBUFFER::Padding_Has
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      12/03/2013 18:26:00
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XBUFFER::Padding_Has()
{
  return paddinghas;
}



/*-------------------------------------------------------------------
//  XBUFFER::Padding_GetType
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      14/03/2013 19:23:40
//
//  @return       XBUFFER_PADDINGTYPE :
//  */
/*-----------------------------------------------------------------*/
XBUFFER_PADDINGTYPE XBUFFER::Padding_GetType()
{
  return paddingtype;
}




/*-------------------------------------------------------------------
//  XBUFFER::Padding_GetSize
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      12/03/2013 18:43:05
//
//  @return       XBYTE :
//  */
/*-----------------------------------------------------------------*/
XBYTE XBUFFER::Padding_GetSize()
{
  return paddingsize;
}



/*-------------------------------------------------------------------
//  XBUFFER::Padding_Delete
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      12/03/2013 18:26:06
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
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




//-------------------------------------------------------------------
//  XBUFFER::Extract
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/02/2007 14:41:30
//
//  @return       bool :
//  @param        pbuffer :
//  @param        ppos :
//  @param        psize :
*/
//-------------------------------------------------------------------
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

  memmove(&buffer[ppos],&buffer[ppos+esize],size-(ppos+esize)); // #Imanol changed to avoid copyng overlapped memory, as that's undefined
  //memcpy(&buffer[ppos],&buffer[ppos+esize],size-(ppos+esize));

  Resize(size-esize,false);

  SetBlocked(false);

  return esize;
}



/*-------------------------------------------------------------------
//  XBUFFER::Extract
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      18/05/2011 10:15:07
//
//  @return       bool :
//  @param        data :
//  @param        ppos :
*/
/*-----------------------------------------------------------------*/
bool XBUFFER::Extract(XBYTE& data, XDWORD ppos)
{
  XDWORD size = Extract((XBYTE*)&data, ppos, sizeof(XBYTE));
  if(size!=sizeof(XBYTE)) return false;

  return true;
}




/*-------------------------------------------------------------------
//   XBUFFER::Extract
*/
/**
//
//
//
//  @author   Abraham J. Velez
//  @version  08/01/2018 16:16:51
//  @return   bool :
//
//  @param    bool& :
//  @param    XDWORD :
//
*//*-----------------------------------------------------------------*/
bool XBUFFER::Extract(bool& data, XDWORD ppos)
{
  XBYTE _data = 0;

  XDWORD size = Extract((XBYTE*)&_data, ppos, sizeof(XBYTE));
  if(size!=sizeof(XBYTE)) return false;

  data = _data?true:false;

  return true;
}



/*-------------------------------------------------------------------
//  XBUFFER::Extract
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      18/05/2011 10:16:02
//
//  @return       bool :
//  @param        data :
//  @param        ppos :
*/
/*-----------------------------------------------------------------*/
bool XBUFFER::Extract(XWORD& data, XDWORD ppos)
{
  XDWORD size = Extract((XBYTE*)&data, ppos, sizeof(XWORD));
  if(size!=sizeof(XWORD)) return false;

  if(hardwareuselittleendian) SWAPWORD(data);

  return true;
}




/*-------------------------------------------------------------------
//  XBUFFER::Extract
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      18/05/2011 10:17:03
//
//  @return       bool :
//  @param        data :
//  @param        ppos :
*/
/*-----------------------------------------------------------------*/
bool XBUFFER::Extract(XDWORD& data, XDWORD ppos)
{
  XDWORD size = Extract((XBYTE*)&data, ppos, sizeof(XDWORD));
  if(size!=sizeof(XDWORD)) return false;

  if(hardwareuselittleendian) SWAPDWORD(data);

  return true;
}



/*-------------------------------------------------------------------
//  XBUFFER::Extract
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      14/03/2016 13:03:50
//
//  @return       bool :
//
//  @param        data :
//  @param        ppos :
*/
/*-----------------------------------------------------------------*/
bool XBUFFER::Extract(XQWORD& data, XDWORD ppos)
{
  XDWORD size = Extract((XBYTE*)&data, ppos, sizeof(XQWORD));
  if(size!=sizeof(XQWORD)) return false;

  if(hardwareuselittleendian) SWAPQWORD(data);

  return true;
}



/*-------------------------------------------------------------------
//  XBUFFER::Extract
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      14/03/2016 13:04:35
//
//  @return       bool :
//
//  @param        data :
//  @param        ppos :
*/
/*-----------------------------------------------------------------*/
bool XBUFFER::Extract(float& data,XDWORD ppos)
{
  XDWORD size = Extract((XBYTE*)&data, ppos, sizeof(XDWORD));
  if(size!=sizeof(XDWORD)) return false;

  return true;
}



/*-------------------------------------------------------------------
//  XBUFFER::Extract
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      14/03/2016 13:04:41
//
//  @return       bool :
//
//  @param        data :
//  @param        ppos :
*/
/*-----------------------------------------------------------------*/
bool XBUFFER::Extract(double& data, XDWORD ppos)
{
  XDWORD size = Extract((XBYTE*)&data, ppos, sizeof(XQWORD));
  if(size!=sizeof(XQWORD)) return false;

  return true;
}




/*-------------------------------------------------------------------
//  XBUFFER::Extract
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      30/03/2015 16:17:19
//
//  @return       bool :
//
//  @param        string :
//  @param        ppos :
//  @param        size :
*/
/*-----------------------------------------------------------------*/
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



/*-------------------------------------------------------------------
//  XBUFFER::ExtractWithMask
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      05/12/2016 16:38:16
//
//  @return       bool :
//
//  @param        mask :
//  @param        frompos :
//  @param        ... :
*/
/*-----------------------------------------------------------------*/
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




/*-------------------------------------------------------------------
//  XBUFFER::ExtractWithMask
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      05/03/2016 18:26:52
//
//  @return       bool :
//
//  @param        mask :
//  @param        frompos :
//  @param        ... :
*/
/*-----------------------------------------------------------------*/
bool XBUFFER::ExtractWithMask(XSTRING* mask, int frompos, ...)
{
  va_list  arg;

  va_start(arg, frompos);

  bool status = ExtractXBufferWithMask((*this), mask, arg);

  va_end(arg);

  return status;
}




/*-------------------------------------------------------------------
//  XBUFFER::Compare
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      01/06/2012 11:25:47
//
//  @return       bool :
//  @param        pbuffer :
//  @param        psize :
*/
/*-----------------------------------------------------------------*/
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



/*-------------------------------------------------------------------
//  XBUFFER::Compare
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      01/06/2012 11:25:52
//
//  @return       bool :
//  @param        buffer :
*/
/*-----------------------------------------------------------------*/
bool XBUFFER::Compare(XBUFFER* buffer)
{
  if(!buffer) return false;

  return Compare(buffer->Get(), buffer->GetSize());
}



/*-------------------------------------------------------------------
//  XBUFFER::Get
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/08/2011 22:37:09
//
//  @return       XBYTE* :
//  */
/*-----------------------------------------------------------------*/
XBYTE* XBUFFER::Get()
{
  if(!size) return NULL;

  return buffer;
}




/*-------------------------------------------------------------------
//  XBUFFER::GetByte
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      30/01/2017 9:32:53
//
//  @return       XBYTE :
//
//  @param        index :
*/
/*-----------------------------------------------------------------*/
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



//-------------------------------------------------------------------
//  XBUFFER::Get
/**
//
//
//  @author       Abraham J. Velez
//  @version      24/11/2004 12:36:54
//
//  @return       bool :
//  @param        pbuffer :
//  @param        psize :
//  @param        frompos:
*/
//-------------------------------------------------------------------
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




/*-------------------------------------------------------------------
//  XBUFFER::Get
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      12/07/2009 05:30:19 p.m.
//
//  @return       bool :
//  @param        data :
//  @param        fromposition :
*/
/*-----------------------------------------------------------------*/
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




/*-------------------------------------------------------------------
//   XBUFFER::Get
*/
/**
//
//
//
//  @author   Abraham J. Velez
//  @version  08/01/2018 16:19:25
//  @return   bool :
//
//  @param    bool& :
//  @param    int :
//
*//*-----------------------------------------------------------------*/
bool XBUFFER::Get(bool& data, int frompos)
{
  XBYTE _data = 0;
  bool  status;

  status = Get(_data, frompos);

  if(status) data = _data?true:false;

  return status;
}






/*-------------------------------------------------------------------
//  XBUFFER::Get
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/08/2013 16:21:53
//
//  @return       bool :
//  @param        data :
//  @param        frompos :
*/
/*-----------------------------------------------------------------*/
bool XBUFFER::Get(XWORD& data, int frompos)
{
  if(!Get((XBYTE*)&data,sizeof(XWORD), frompos)) return false;

  if(hardwareuselittleendian) SWAPWORD(data);

  return true;
}



/*-------------------------------------------------------------------
//  XBUFFER::Get
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/08/2013 16:22:19
//
//  @return       bool :
//  @param        data :
//  @param        frompos :
*/
/*-----------------------------------------------------------------*/
bool XBUFFER::Get(XDWORD& data, int frompos)
{
  if(!Get((XBYTE*)&data,sizeof(XDWORD), frompos)) return false;

  if(hardwareuselittleendian) SWAPDWORD(data);

  return true;
}



/*-------------------------------------------------------------------
//  XBUFFER::Get
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      14/03/2016 13:06:57
//
//  @return       bool :
//
//  @param        data :
//  @param        frompos :
*/
/*-----------------------------------------------------------------*/
bool XBUFFER::Get(XQWORD& data, int frompos)
{
  if(!Get((XBYTE*)&data,sizeof(XQWORD), frompos)) return false;

  if(hardwareuselittleendian) SWAPQWORD(data);

  return true;
}




/*-------------------------------------------------------------------
//  XBUFFER::Get
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      14/03/2016 13:06:48
//
//  @return       bool :
//
//  @param        data :
//  @param        frompos :
*/
/*-----------------------------------------------------------------*/
bool XBUFFER::Get(float& data, int frompos)
{
  if(!Get((XBYTE*)&data, sizeof(XDWORD), frompos)) return false;

  return true;
}



/*-------------------------------------------------------------------
//  XBUFFER::Get
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      14/03/2016 13:06:52
//
//  @return       bool :
//
//  @param        data :
//  @param        frompos :
*/
/*-----------------------------------------------------------------*/
bool XBUFFER::Get(double& data, int frompos)
{
  if(!Get((XBYTE*)&data, sizeof(XQWORD), frompos)) return false;

  return true;
}




/*-------------------------------------------------------------------
//  XBUFFER::Get
*/
/**
//
//  gets a xstring from a buffer
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      23/11/2015 11:45:54
//
//  @return       bool :
//
//  @param        data :
//  @param        psize : size in characters
//  @param        XBUFFER_INVALIDPOSITION :
*/
/*-----------------------------------------------------------------*/
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



/*-------------------------------------------------------------------
//  XBUFFER::GetWithMask
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      05/03/2016 18:36:15
//
//  @return       bool :
//
//  @param        mask :
//  @param        frompos :
//  @param        ... :
*/
/*-----------------------------------------------------------------*/
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




//-------------------------------------------------------------------
//  XBUFFER::Set
/**
//
//
//  @author       Abraham J. Velez
//  @version      24/11/2004 12:38:18
//
//  @return       bool :
//  @param        pbuffer :
//  @param        psize :
//  @param        toposition :
*/
//-------------------------------------------------------------------
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



/*-------------------------------------------------------------------
//  XBUFFER::Set
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      12/07/2009 05:30:26 p.m.
//
//  @return       bool :
//  @param        data :
//  @param        toposition :
*/
/*-----------------------------------------------------------------*/
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



/*-------------------------------------------------------------------
//   XBUFFER::Set
*/
/**
//
//
//
//  @author   Abraham J. Velez
//  @version  08/01/2018 16:21:52
//  @return   bool :
//
//  @param    bool :
//  @param    int :
//
*//*-----------------------------------------------------------------*/
bool XBUFFER::Set(bool data, int topos)
{
  XBYTE _data = data?1:0;

  return Set(_data, topos);

}




/*-------------------------------------------------------------------
//  XBUFFER::Set
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/08/2013 0:09:32
//
//  @return       bool :
//  @param        data :
//  @param        topos :
*/
/*-----------------------------------------------------------------*/
bool XBUFFER::Set(XWORD data, int topos)
{
  if(hardwareuselittleendian) SWAPWORD(data);

  return Set((XBYTE*)&data, sizeof(XWORD), topos);
}



/*-------------------------------------------------------------------
//  XBUFFER::Set
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/08/2013 0:09:57
//
//  @return       bool :
//  @param        data :
//  @param        topos :
*/
/*-----------------------------------------------------------------*/
bool XBUFFER::Set(XDWORD data, int topos)
{
  if(hardwareuselittleendian) SWAPDWORD(data);

  return Set((XBYTE*)&data, sizeof(XDWORD), topos);
}




/*-------------------------------------------------------------------
//  XBUFFER::Set
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      14/03/2016 13:08:42
//
//  @return       bool :
//
//  @param        data :
//  @param        topos :
*/
/*-----------------------------------------------------------------*/
bool XBUFFER::Set(XQWORD data, int topos)
{
  if(hardwareuselittleendian) SWAPQWORD(data);

  return Set((XBYTE*)&data, sizeof(XQWORD), topos);
}



/*-------------------------------------------------------------------
//  XBUFFER::Set
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      14/03/2016 13:08:46
//
//  @return       bool :
//
//  @param        data :
//  @param        topos :
*/
/*-----------------------------------------------------------------*/
bool XBUFFER::Set(float data, int topos)
{
  return Set((XBYTE*)&data, sizeof(XDWORD), topos);
}




/*-------------------------------------------------------------------
//  XBUFFER::Set
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      14/03/2016 13:08:51
//
//  @return       bool :
//
//  @param        data :
//  @param        topos :
*/
/*-----------------------------------------------------------------*/
bool XBUFFER::Set(double data, int topos)
{
  return Set((XBYTE*)&data, sizeof(XQWORD), topos);
}




/*-------------------------------------------------------------------
//  XBUFFER::Set
*/
/**
//
//  normalizes and puts an xstring on a buffer
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      23/11/2015 11:37:35
//
//  @return       bool :
//
//  @param        data :
//  @param        XBUFFER_INVALIDPOSITION :
*/
/*-----------------------------------------------------------------*/
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


/*-------------------------------------------------------------------
//  XBUFFER::SetWithMask
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      05/12/2016 16:39:15
//
//  @return       XDWORD :
//
//  @param        mask :
//  @param        topos :
//  @param        ... :
*/
/*-----------------------------------------------------------------*/
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




/*-------------------------------------------------------------------
//  XBUFFER::SetWithMask
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      05/03/2016 19:51:46
//
//  @return       bool :
//
//  @param        mask :
//  @param        topos :
//  @param        ... :
*/
/*-----------------------------------------------------------------*/
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




/*-------------------------------------------------------------------
//  XBUFFER::SetNBits
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      08/08/2013 23:45:51
//
//  @return       bool :
//  @param        data :
//  @param        XBUFFER_INVALIDPOSITION :
*/
/*-----------------------------------------------------------------*/
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





/*-------------------------------------------------------------------
//  XBUFFER::Resize
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      12/07/2009 04:26:44 p.m.
//
//  @return       bool :
//  @param        newsize :
//  @param        setblocked :
*/
/*----------------------------------------------------------------*/
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



//-------------------------------------------------------------------
//  XBUFFER::Delete
/**
//
//
//  @author       Abraham J. Velez
//  @version      24/11/2004 11:55:16
//
//  @return       bool :
//  */
//-------------------------------------------------------------------
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



/*-------------------------------------------------------------------
//  XBUFFER::Swap
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      12/03/2014 9:20:28
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
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




/*-------------------------------------------------------------------
//  XBUFFER::AdjustToNBits
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      16/12/2012 1:16:06
//
//  @return       bool :
//  @param        nbits :
*/
/*-----------------------------------------------------------------*/
bool XBUFFER::AdjustToNBits(int nbits)
{
  this->nbits     = nbits;
  this->nbitsfree = (16-nbits);

  return true;
}



/*-------------------------------------------------------------------
//  XBUFFER::GetNBitsFree
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/12/2012 23:45:37
//
//  @return       XBYTE :
//  */
/*-----------------------------------------------------------------*/
XBYTE XBUFFER::GetNBitsFree()
{
  return lastnbitsfree;
}



/*-------------------------------------------------------------------
//  XBUFFER::SetNBitsFree
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/12/2012 23:45:34
//
//  @return       bool :
//  @param        nbitsfree :
*/
/*-----------------------------------------------------------------*/
bool XBUFFER::SetNBitsFree(XBYTE nbitsfree)
{
  lastnbitsfree = nbitsfree;

  return true;
}



/*-------------------------------------------------------------------
//  XBUFFER::ConvertFromBase64
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      19/03/2014 16:01:38
//
//  @return       bool :
//
//  @param        stringbase64 :
*/
/*-----------------------------------------------------------------*/
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



/*-------------------------------------------------------------------
//  XBUFFER::ConvertToBase64
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      20/05/2014 13:24:21
//
//  @return       bool :
//
//  @param        stringbase64 :
*/
/*-----------------------------------------------------------------*/
bool XBUFFER::ConvertToBase64(XSTRING& stringbase64)
{
  XSTRING stringbin;

  stringbase64.Empty();

  stringbin.Add((*this));
  stringbin.ConvertToBase64(stringbase64);

  return stringbase64.GetSize()?true:false;
}




/*-------------------------------------------------------------------
//  XBUFFER::DecodeBCD
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      18/09/2014 16:49:33
//
//  @return       XDWORD :
//
//  @param        ppos :
//  @param        psize :
*/
/*-----------------------------------------------------------------*/
XDWORD XBUFFER::DecodeBCD(XDWORD ppos,XDWORD psize)
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



/*-------------------------------------------------------------------
//  XBUFFER::DecodeBCDLong
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      26/04/2016 10:35:43
//
//  @return       XQWORD :
//
//  @param        ppos :
//  @param        psize :
*/
/*-----------------------------------------------------------------*/
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




/*-------------------------------------------------------------------
//  XBUFFER::AddXBufferWithMask
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      05/03/2016 17:47:09
//
//  @return       bool :
//
//  @param        xbuffer :
//  @param        mask :
//  @param        arg :
*/
/*-----------------------------------------------------------------*/
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




/*-------------------------------------------------------------------
//  XBUFFER::GetXBufferWithMask
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      05/03/2016 17:54:58
//
//  @return       bool :
//
//  @param        xbuffer :
//  @param        mask :
//  @param        arg :
*/
/*-----------------------------------------------------------------*/
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
