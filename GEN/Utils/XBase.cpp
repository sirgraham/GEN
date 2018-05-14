/**-------------------------------------------------------------------------------------------------------------------
* @file     XBase.cpp
*
* @class    XBASE
* @brief    Basic functions and defines 
* @ingroup  UTILS
*
* @author   Abraham J. Velez 
* @date     13/05/2018 13:17:54
*
* Copyright(c) 2005 - 2018 GEN Group.
*
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
*---------------------------------------------------------------------------------------------------------------------*/

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include <math.h>

#include "XBase.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/



/**-------------------------------------------------------------------------------------------------------------------
* @fn         Truncate
* @brief      Trucate double
* @ingroup    UTILS
* @date       13/05/2018 13:53:01
* @param[in]  value : double to truncate
* @return     double : double truncate
*---------------------------------------------------------------------------------------------------------------------*/
double Truncate(double value)
{
  double truncate = 0;

  modf(value, &truncate);

  return truncate;
}



/**-------------------------------------------------------------------------------------------------------------------
* @fn         Fraction
* @brief      Return the fractional part of double
* @ingroup    UTILS
* @date       13/05/2018 13:54:13
* @param[in]  value : double value to obtain fractional part
* @return     double : fractional part of value
*---------------------------------------------------------------------------------------------------------------------*/
double Fraction(double value)
{
  double truncate = 0;

  return modf(value, &truncate);
}



/**-------------------------------------------------------------------------------------------------------------------
* @fn         SwapWORD
* @brief      Return the swap of word (2 bytes) hi-lo  > lo-hi
* @ingroup    UTILS
* @date       13/05/2018 13:54:58
* @param[in]  data : word to swap
* @return     XWORD : word swaped
*---------------------------------------------------------------------------------------------------------------------*/
XWORD SwapWORD(XWORD data)
{
  const XWORD hi = (XWORD)((data&0xFF00)>>8);
  const XWORD lo = (XWORD)((data&0x00FF)<<8);

  return (XWORD)(hi|lo);
}



/**-------------------------------------------------------------------------------------------------------------------
* @fn         SwapDWORD
* @brief      Swap double word (4 bytes) hi1 lo1 h2 lo2 >  lo2 hi2 lo1 hi1
* @ingroup    UTILS
* @date       13/05/2018 13:55:38
* @param[in]  data : 
* @return     XDWORD : double word swaped
*---------------------------------------------------------------------------------------------------------------------*/
XDWORD SwapDWORD(XDWORD data)
{
  const XDWORD hi1 = (data&0xFF000000)>>24;
  const XDWORD lo1 = (data&0x00FF0000)>>8;
  const XDWORD hi2 = (data&0x0000FF00)<<8;
  const XDWORD lo2 = (data&0x000000FF)<<24;

  return (hi1|lo1|hi2|lo2);
}



/**-------------------------------------------------------------------------------------------------------------------
* @fn         SwapQWORD
* @brief      Swap quad word (8 bytes) hi1 lo1 h2 lo2 hi3 lo3 h4 lo4  > lo3 hi3 lo4 hi4 lo2 hi2 lo1 hi1
* @ingroup    UTILS
* @date       13/05/2018 13:56:10
* @param[in]  data : quad word to swap
* @return     XQWORD : quad word swaped
*---------------------------------------------------------------------------------------------------------------------*/
XQWORD SwapQWORD(XQWORD data)
{
  const XQWORD hi1 = (data&0xFF00000000000000ULL) >> (24 + 32);
  const XQWORD lo1 = (data&0x00FF000000000000ULL) >> ( 8 + 32);
  const XQWORD hi2 = (data&0x0000FF0000000000ULL) >> (     24);
  const XQWORD lo2 = (data&0x000000FF00000000ULL) >> (      8);

  const XQWORD hi3 = (data&0x00000000FF000000ULL) << (      8);
  const XQWORD lo3 = (data&0x0000000000FF0000ULL) << (     24);
  const XQWORD hi4 = (data&0x000000000000FF00ULL) << ( 8 + 32);
  const XQWORD lo4 = (data&0x00000000000000FFULL) << (24 + 32);

  return (hi1|lo1|hi2|lo2|hi3|lo3|hi4|lo4);
}



/**-------------------------------------------------------------------------------------------------------------------
* @fn         RoundFloat
* @brief      Round float
* @ingroup    UTILS
* @date       13/05/2018 13:56:45
* @param[]    x : float to round
* @return     float : rounded float
*---------------------------------------------------------------------------------------------------------------------*/
float RoundFloat(float x)
{
  return float(x >= 0.0f ? floor(x + 0.5f) : ceil(x - 0.5f));
}



/**-------------------------------------------------------------------------------------------------------------------
* @fn         DWORDToBCD
* @brief      Convert double word to BCD
* @ingroup    UTILS
* @date       13/05/2018 13:57:23
* @param[in]    dword : double word to convert
* @return     XQWORD : Quad word converted BCD
*---------------------------------------------------------------------------------------------------------------------*/
XQWORD DWORDToBCD(XDWORD dword)
{
  XQWORD shift  = 16;
  XQWORD result = (dword % 10);

  while((dword = (dword/10)))
    {
      result += (dword % 10) * shift;
      shift *= 16; // weirdly, it's not possible to left shift more than 32 bits
    }

  return result;
}



/**-------------------------------------------------------------------------------------------------------------------
* @fn         BCDToDWORD
* @brief      Convert BCD to double word
* @ingroup    UTILS
* @date       13/05/2018 13:57:56
* @param[in]  bcd : BCD to convert
* @return     XDWORD : doble word converted
*---------------------------------------------------------------------------------------------------------------------*/
XDWORD BCDToDWORD(XQWORD bcd)
{
  XQWORD mask = 0x000f;
  XQWORD pwr  = 1;
  XQWORD i    = (bcd & mask);

  while((bcd = (bcd >> 4)))
    {
      pwr *= 10;
      i += (bcd & mask) * pwr;
    }

  return (XDWORD)i;
}


