/**-------------------------------------------------------------------------------------------------------------------
*
* @file        XBufferBits.cpp
*
* @class       XBUFFERBITS
* @brief       Reads bits from an xbuffer
* @ingroup     UTILS
*
* @author      Imanol Celaya Ruiz de Alegria 
* @date        28/05/2018 20:12:24
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

#include "XBuffer.h"

#include "XBufferBits.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XBUFFERBITS::XBUFFERBITS(XBUFFER* xbuffer)
* @brief      Constructor
* @ingroup    UTILS
*
* @author     Imanol Celaya Ruiz de Alegria 
* @date       28/05/2018 20:13:01
*
* @param[in]  XBUFFER* : buffer of origin
*
* @return     Does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
XBUFFERBITS::XBUFFERBITS(XBUFFER* xbuffer)
{
  Clean();

  this->xbuffer = xbuffer;
  this->p = xbuffer->Get();
  this->bits_left = 8;
}




/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XBUFFERBITS::~XBUFFERBITS()
* @brief      Destructor
* @note       VIRTUAL
* @ingroup    UTILS
*
* @author     Imanol Celaya Ruiz de Alegria 
* @date       29/05/2018 13:14:44
*
* @return     Does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
XBUFFERBITS::~XBUFFERBITS()
{
  Clean();
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFERBITS::IsByteAligned()
* @brief      Is buffer byte aligned
* @ingroup    UTILS
*
* @author     Imanol Celaya Ruiz de Alegria 
* @date       28/05/2018 20:14:13
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFERBITS::IsByteAligned()
{
  return (bits_left == 8);
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFERBITS::IsEOF()
* @brief      Is EOF of buffer stream
* @ingroup    UTILS
*
* @author     Imanol Celaya Ruiz de Alegria 
* @date       28/05/2018 20:14:49
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFERBITS::IsEOF()
{
  return p >= (xbuffer->Get()+xbuffer->GetSize());
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XBUFFERBITS::IsOverrun()
* @brief      Check Is buffer overrun
* @ingroup    UTILS
*
* @author     Imanol Celaya Ruiz de Alegria 
* @date       28/05/2018 20:15:28
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XBUFFERBITS::IsOverrun()
{
  return (p >= (xbuffer->Get()+xbuffer->GetSize())) && (bits_left < 8);
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD XBUFFERBITS::GetBytePosition()
* @brief      Get Byte position into buffer
* @ingroup    UTILS
*
* @author     Imanol Celaya Ruiz de Alegria 
* @date       28/05/2018 20:15:52
*
* @return     XDWORD : position byte into buffer
*
*---------------------------------------------------------------------------------------------------------------------*/
XDWORD XBUFFERBITS::GetBytePosition()
{
  return (XDWORD)(xbuffer->Get() - p);
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD XBUFFERBITS::GetBitPosition()
* @brief      Get bit position into buffer
* @ingroup    UTILS
*
* @author     Imanol Celaya Ruiz de Alegria 
* @date       28/05/2018 20:16:41
*
* @return     XDWORD : position bit into buffer
*
*---------------------------------------------------------------------------------------------------------------------*/
XDWORD XBUFFERBITS::GetBitPosition()
{
  return 8 - bits_left;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XBUFFERBITS::SetPosition(XDWORD index, XDWORD bitindex)
* @brief      Set Position into buffer
* @ingroup    UTILS
*
* @author     Imanol Celaya Ruiz de Alegria 
* @date       28/05/2018 20:16:58
*
* @param[in]  index : index into buffer
* @param[in]  bitindex : bit index into buffer
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XBUFFERBITS::SetPosition(XDWORD index, XDWORD bitindex)
{
  p = xbuffer->Get() + index;
  bits_left = 8 - ((bitindex >= 8) ? 7 : 0); // zero base index, can't be 8
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XBUFFERBITS::SkipU1()
* @brief      Skip one bit
* @ingroup    UTILS
*
* @author     Imanol Celaya Ruiz de Alegria 
* @date       28/05/2018 20:19:05
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XBUFFERBITS::SkipU1()
{
  bits_left--;

  if(bits_left == 0)
    {
      p++;
      bits_left = 8;
    }
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD XBUFFERBITS::PeekU1()
* @brief      Peek one bit
* @ingroup    UTILS
*
* @author     Imanol Celaya Ruiz de Alegria 
* @date       28/05/2018 20:19:37
*
* @return     XDWORD : bit peek.
*
*---------------------------------------------------------------------------------------------------------------------*/
XDWORD XBUFFERBITS::PeekU1()
{
  XDWORD r = 0;

  if(!IsEOF())
    {
      r = ((*(p)) >> ( bits_left - 1 )) & 0x01;
    }

  return r;
}




/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD XBUFFERBITS::ReadU1()
* @brief      Read one bit
* @ingroup    UTILS
*
* @author     Imanol Celaya Ruiz de Alegria 
* @date       28/05/2018 20:20:02
*
* @return     XDWORD : Read U1
*
*---------------------------------------------------------------------------------------------------------------------*/
XDWORD XBUFFERBITS::ReadU1()
{
  XDWORD r = 0;

  bits_left--;

  if (! IsEOF())
    {
      r = ((*(p)) >> bits_left) & 0x01;
    }

  if (bits_left == 0)
    {
      p++;
      bits_left = 8;
    }

  return r;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XBUFFERBITS::SkipU(XDWORD n)
* @brief      Skip n bits
* @ingroup    UTILS
*
* @author     Imanol Celaya Ruiz de Alegria 
* @date       28/05/2018 20:20:47
*
* @param[in]  n : n bits
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XBUFFERBITS::SkipU(XDWORD n)
{
  XDWORD i;
  for(i = 0; i < n; i++)
    {
      SkipU1();
    }
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD XBUFFERBITS::PeekU(XDWORD n)
* @brief      Peek n bits (max 32 bits)
* @ingroup    UTILS
*
* @author     Imanol Celaya Ruiz de Alegria 
* @date       28/05/2018 20:22:06
*
* @param[in]  n : n bits
*
* @return     XDWORD : n bits Peek (max 32 bits)
*
*---------------------------------------------------------------------------------------------------------------------*/
XDWORD XBUFFERBITS::PeekU(XDWORD n)
{
  XBYTE* orig_byte = p;
  XDWORD origin_bits_left = bits_left;

  XDWORD r = 0;

  r = ReadU(n);

  p = orig_byte;
  bits_left = origin_bits_left;

  return r;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD XBUFFERBITS::ReadU(XDWORD n)
* @brief      Read n bits (max 32 bits)
* @ingroup    UTILS
*
* @author     Imanol Celaya Ruiz de Alegria 
* @date       28/05/2018 20:22:21
*
* @param[in]  n : n bits
*
* @return     XDWORD : n bits read (max 32 bits)
*
*---------------------------------------------------------------------------------------------------------------------*/
XDWORD XBUFFERBITS::ReadU(XDWORD n)
{
  XDWORD r = 0;
  XDWORD i;
  for (i = 0; i < n; i++)
    {
      r |= ( ReadU1() << ( n - i - 1 ) );
    }

  return r;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XDWORD XBUFFERBITS::ReadUE()
* @brief      
* @ingroup    UTILS
*
* @author     Imanol Celaya Ruiz de Alegria 
* @date       28/05/2018 20:23:15
*
* @return     XDWORD : 
*
*---------------------------------------------------------------------------------------------------------------------*/
XDWORD XBUFFERBITS::ReadUE()
{
  XDWORD r = 0;
  int i = 0;

  while( (ReadU1() == 0) && (i < 32) && (!IsEOF()) )
    {
      i++;
    }

  r = ReadU(i);
  r += (1 << i) - 1;

  return r;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int XBUFFERBITS::ReadSE()
* @brief      Read SE
* @ingroup    UTILS
*
* @author     Imanol Celaya Ruiz de Alegria 
* @date       28/05/2018 20:23:44
*
* @return     int : 
*
*---------------------------------------------------------------------------------------------------------------------*/
int XBUFFERBITS::ReadSE()
{
  int r = ReadUE();
  if (r & 0x01)
    {
      r = (r+1)/2;
    }
  else
    {
      r = -(r/2);
    }

  return r;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XBUFFERBITS::WriteU1(XDWORD v)
* @brief      Write one bit
* @ingroup    UTILS
*
* @author     Imanol Celaya Ruiz de Alegria 
* @date       28/05/2018 20:24:07
*
* @param[in]  v : bit to write
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XBUFFERBITS::WriteU1(XDWORD v)
{
  bits_left--;

  if(! IsEOF())
    {
      // FIXME this is slow, but we must clear bit first
      // is it better to memset(0) the whole buffer during bs_init() instead?
      // if we don't do either, we introduce pretty nasty bugs
      (*(p)) &= ~(0x01 << bits_left);
      (*(p)) |= ((v & 0x01) << bits_left);
    }

  if(bits_left == 0)
    {
      p++;
      bits_left = 8;
    }
}




/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XBUFFERBITS::WriteU(XDWORD n, XDWORD v)
* @brief      Write n bits
* @ingroup    UTILS
*
* @author     Imanol Celaya Ruiz de Alegria 
* @date       28/05/2018 20:24:28
*
* @param[in]  n : n bits
* @param[in]  v : bits to write
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XBUFFERBITS::WriteU(XDWORD n, XDWORD v)
{
  XDWORD i;
  for(i = 0; i < n; i++)
    {
      WriteU1((v >> ( n - i - 1 ))&0x01);
    }
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XBUFFERBITS::WriteUE(XDWORD v)
* @brief      
* @ingroup    UTILS
*
* @author     Imanol Celaya Ruiz de Alegria 
* @date       28/05/2018 20:24:49
*
* @param[in]  v : 
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XBUFFERBITS::WriteUE(XDWORD v)
{
  static const int len_table[256] =
  {
    1,
    1,
    2,2,
    3,3,3,3,
    4,4,4,4,4,4,4,4,
    5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
    6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
    6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
    7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
    8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
  };

  XDWORD len;

  if (v == 0)
    {
      WriteU1(1);
    }
  else
    {
      v++;

      if (v >= 0x01000000)
        {
          len = 24 + len_table[ v >> 24 ];
        }
      else if(v >= 0x00010000)
        {
          len = 16 + len_table[ v >> 16 ];
        }
      else if(v >= 0x00000100)
        {
          len =  8 + len_table[ v >>  8 ];
        }
      else
        {
          len = len_table[ v ];
        }

      WriteU(2*len-1, v);
    }
}




/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XBUFFERBITS::WriteSE(int v)
* @brief      
* @ingroup    UTILS
*
* @author     Imanol Celaya Ruiz de Alegria 
* @date       28/05/2018 20:25:17
*
* @param[in]  v : 
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XBUFFERBITS::WriteSE(int v)
{
  if (v <= 0)
    {
      WriteUE(-v*2);
    }
    else
    {
      WriteUE(v*2 - 1);
    }
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XBUFFERBITS::Clean()
* @brief      Clean the attributes of the class: Default initialice
* @note       INTERNAL
* @ingroup    UTILS
*
* @author     Imanol Celaya Ruiz de Alegria 
* @date       29/05/2018 13:13:44
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XBUFFERBITS::Clean()
{
  xbuffer     = NULL;
  p           = NULL;
  bits_left   = 0;
}