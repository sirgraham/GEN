/**-------------------------------------------------------------------------------------------------------------------
* @file     XBER.cpp
*
* @class    XBER
* @brief    Basic Encoding Rules (BER) encoding class (https://en.wikipedia.org/wiki/X.690)
* @ingroup  UTILS
*
* @author   Abraham J. Velez 
* @date     13/05/2018 13:29:43
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

#include "XBuffer.h"

#include "XBER.h"

#include "XMemory.h"


/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/



/**-------------------------------------------------------------------------------------------------------------------
* @fn         XBER::XBER
* @brief      Constructor
* @ingroup    UTILS
* @date       13/05/2018 13:59:23
* @param[]     : 
* @return      : 
*---------------------------------------------------------------------------------------------------------------------*/
XBER::XBER()
{
  Clean();

  data = new XBUFFER();
}



/**-------------------------------------------------------------------------------------------------------------------
* @fn         XBER::~XBER
* @brief      Destructor
* @ingroup    UTILS
* @date       13/05/2018 13:59:09
* @param[]     : 
* @return      : 
*---------------------------------------------------------------------------------------------------------------------*/
XBER::~XBER()
{
  if(data) delete data;

  Sequence_DeleteAll();

  Clean();
}



/**-------------------------------------------------------------------------------------------------------------------
* @fn         XBER::GetType
* @brief      Get Type BER
* @ingroup    UTILS
* @date       13/05/2018 13:59:48
* @param[]     : 
* @return     XBYTE : Type of BER
*---------------------------------------------------------------------------------------------------------------------*/
XBYTE XBER::GetType()
{
  return (XBYTE)(type&0x3F);
}



/**-------------------------------------------------------------------------------------------------------------------
* @fn         XBER::GetSize
* @brief      Get size BER
* @ingroup    UTILS
* @date       13/05/2018 14:00:16
* @param[]     : 
* @return     XDWORD : size of BER
*---------------------------------------------------------------------------------------------------------------------*/
XDWORD XBER::GetSize()
{
  return size;
}



/**-------------------------------------------------------------------------------------------------------------------
* @fn         XBER::GetData
* @brief      Get data of BER
* @ingroup    UTILS
* @date       13/05/2018 14:00:58
* @param[]     : 
* @return     XBUFFER* : buffer of data 
*---------------------------------------------------------------------------------------------------------------------*/
XBUFFER* XBER::GetData()
{
  return data;
}



/**-------------------------------------------------------------------------------------------------------------------
* @fn         XBER::GetDump
* @brief      Get dump od data
* @ingroup    UTILS
* @date       13/05/2018 14:01:22
* @param[in]  xbuffer : true if is succesful
* @return     bool : buffer of data to dump
*---------------------------------------------------------------------------------------------------------------------*/
bool XBER::GetDump(XBUFFER& xbuffer)
{
  xbuffer.Add((XBYTE)type);

  XBUFFER sizedata;
  if(CodeSize(size,sizedata) && type!=XBERTYPE_NULL) xbuffer.Add(&sizedata);

  if(data->GetSize()) xbuffer.Add(data->Get(),data->GetSize());

  for(XDWORD c=0;c<sequence.GetSize();c++)
    {
      XBER* xber = (XBER*)sequence.Get(c);
      if(xber) xber->GetDump(xbuffer);
    }

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
* @fn         XBER::SetType
* @brief      Set type of BER
* @ingroup    UTILS
* @date       13/05/2018 14:01:59
* @param[in]  type : new type of BER
* @return     bool : true if is succesful
*---------------------------------------------------------------------------------------------------------------------*/
bool XBER::SetType(XBYTE type)
{
  this->type = type;

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
* @fn         XBER::SetNULL
* @brief      Set BER to NULL
* @ingroup    UTILS
* @date       13/05/2018 14:02:38
* @param[]     : 
* @return     bool : true if is succesful
*---------------------------------------------------------------------------------------------------------------------*/
bool XBER::SetNULL()
{
  if(!data) return false;

  data->Delete();

  data->Add((XBYTE)0);

  this->type = XBERTYPE_NULL;
  this->size = 0;

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
* @fn         XBER::SetINTEGER
* @brief      Set BER to integer
* @ingroup    UTILS
* @date       13/05/2018 14:03:18
* @param[in]  integer : true if is succesful
* @return     bool : integer to assign
*---------------------------------------------------------------------------------------------------------------------*/
bool XBER::SetINTEGER(XDWORD integer)
{
  if(!data) return false;

  data->Delete();

  XDWORD size = 0;

  if(integer&0xFF800000)
    {
      size = 4;
    }
   else
    {
      if(integer&0x00FF8000)
        {
          size = 3;
        }
       else
        {
          if(integer&0x0000FF80)
            {
              size = 2;
            }
           else
            {
              size = 1;
            }
        }
    }

  if(!size) return false;

  for(int c=(size-1);c>=0;c--)
    {
      XBYTE byte = (XBYTE)((c?(integer>>(c*8)):(integer))&0xFF);

      data->Add((XBYTE)byte);
    }

  this->type = XBERTYPE_INTEGER;
  this->size = size;

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
* @fn         XBER::SetINTEGER
* @brief      Set BER to integer
* @ingroup    UTILS
* @date       13/05/2018 14:04:16
* @param[in]  xbufferdata : buffer to assign
* @return     bool : true if is succesful
*---------------------------------------------------------------------------------------------------------------------*/
bool XBER::SetINTEGER(XBUFFER& xbufferdata)
{
  if(!data) return false;

  data->Delete();

  data->Add(xbufferdata);

  this->type = XBERTYPE_INTEGER;
  this->size = xbufferdata.GetSize();

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
* @fn         XBER::SetBITSTRING
* @brief      Set Bit String to BER
* @ingroup    UTILS
* @date       13/05/2018 14:04:48
* @param[in]  xbuffer : buffer to assign
* @return     bool : true if is succesful
*---------------------------------------------------------------------------------------------------------------------*/
bool XBER::SetBITSTRING(XBUFFER& xbuffer)
{
  if(!xbuffer.GetSize()) return false;
  if(!data)              return false;

  data->Delete();

  data->Add(xbuffer);

  this->type = XBERTYPE_BITSTRING;
  this->size = xbuffer.GetSize();

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
* @fn         XBER::SetBITSTRING
* @brief      Set Bit String to BER
* @ingroup    UTILS
* @date       13/05/2018 14:05:24
* @param[in]  buffer : pointer to buffer bytes
* @param[in]  size : size buffer
* @return     bool : true if is succesful
*---------------------------------------------------------------------------------------------------------------------*/
bool XBER::SetBITSTRING(XBYTE* buffer, XDWORD size)
{
  if(!buffer) return false;
  if(!size)   return false;
  if(!data)   return false;

  data->Delete();

  data->Add(buffer, size);

  this->type = XBERTYPE_BITSTRING;
  this->size = size;

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
* @fn         XBER::SetOCTETSTRING
* @brief      Set Octet string to BER
* @ingroup    UTILS
* @date       13/05/2018 14:06:09
* @param[in]  string : string with octets
* @return     bool : true if is succesful
*---------------------------------------------------------------------------------------------------------------------*/
bool XBER::SetOCTETSTRING(XCHAR* string)
{
  if(!string)    return false;
  if(!string[0]) return false;
  if(!data)      return false;

  data->Delete();

  XSTRING _string;

  _string = string;

  XSTRING_CREATEOEM(_string, charOEM)
  data->Add((XBYTE*)charOEM, _string.GetSize());
  XSTRING_DELETEOEM(charOEM)

  this->type = XBERTYPE_OCTETSTRING;
  this->size = _string.GetSize();

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
* @fn         XBER::SetOCTETSTRING
* @brief      Set Octet string to BER
* @ingroup    UTILS
* @date       13/05/2018 14:06:42
* @param[in]  xbuffer : buffer with octets
* @return     bool : true if is succesful
*---------------------------------------------------------------------------------------------------------------------*/
bool XBER::SetOCTETSTRING(XBUFFER& xbuffer)
{
  if(!xbuffer.GetSize()) return false;
  if(!data)              return false;

  data->Delete();

  data->Add(xbuffer);

  this->type = XBERTYPE_OCTETSTRING;
  this->size = xbuffer.GetSize();

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
* @fn         XBER::SetOCTETSTRING
* @brief      Set Octet string to BER
* @ingroup    UTILS
* @date       13/05/2018 14:07:21
* @param[in]  buffer : pointer buffer with octets
* @param[in]  size : size of buffer
* @return     bool : true if is succesful
*---------------------------------------------------------------------------------------------------------------------*/
bool XBER::SetOCTETSTRING(XBYTE* buffer, XDWORD size)
{
  if(!buffer) return false;
  if(!size)   return false;
  if(!data)   return false;

  data->Delete();

  data->Add(buffer, size);

  this->type = XBERTYPE_OCTETSTRING;
  this->size = size;

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
* @fn         XBER::SetOID
* @brief      Set OID
* @ingroup    UTILS
* @date       13/05/2018 14:07:59
* @param[in]  OIDstring : OID to assign
* @return     bool : true if is succesful
*---------------------------------------------------------------------------------------------------------------------*/
bool XBER::SetOID(XCHAR* OIDstring)
{
  if(!OIDstring)    return false;
  if(!OIDstring[0]) return false;
  if(!data)         return false;

  data->Delete();

  XSTRING       string;
  XVECTOR<int>  numbers;
  int           start = 0;
  int           end   = 0;

  string = OIDstring;

  while(1)
    {
      XSTRING numberstring;
      int     indexstring = string.Find(__L("."),true,start);

      if(indexstring != XSTRING_NOTFOUND)
        {
          end = indexstring;
          string.Copy(start,end,numberstring);
          numbers.Add(numberstring.ConvertToInt());
          start = end+1;
        }
       else
        {
          string.Copy(end+1,numberstring);
          numbers.Add(numberstring.ConvertToInt());

          break;
        }
    }

  if(!numbers.GetSize()) return false;

  //---------------------------------------------------
  // The firts 2 bytes are special
  //

  size_t _first = (size_t)(numbers.Get(0));
  int first = (int)_first;

  first *= (40);
  _first = (size_t)(numbers.Get(1));
  first += (int)(_first);

  data->Add((XBYTE)first);

  for(XDWORD c=2;c<numbers.GetSize();c++)
    {
      XBUFFER _data;

      size_t _number = (size_t)(numbers.Get(c));
      XDWORD number = (XDWORD)(_number);

      if(CodeBigNumber((XDWORD)(number),_data))
        {
          data->Add(_data.Get(),_data.GetSize());
        }
    }

  this->type = XBERTYPE_OID;
  this->size = data->GetSize();

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
* @fn         XBER::Copy
* @brief      Copy XBER from pointer *BER
* @ingroup    UTILS
* @date       13/05/2018 14:08:32
* @param[in]  newxber : pointer to new BER to assign
* @return     bool : true if is succesful
*---------------------------------------------------------------------------------------------------------------------*/
bool XBER::Copy(XBER* newxber)
{
  if(!newxber)       return false;
  if(!newxber->data) return false;

  newxber->type = type;
  newxber->size = size;

  newxber->data->Delete();
  newxber->data->Add(data->Get(),data->GetSize());

  for(XDWORD c=0;c<sequence.GetSize();c++)
    {
      XBER* xbertmp = (XBER*)sequence.Get(c);
      if(xbertmp)
        {
          XBER* newxberseq = new XBER();
          if(newxberseq)
            {
              if(xbertmp->Copy(newxberseq))  newxber->sequence.Add(newxberseq);
            }
        }
    }

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
* @fn         XBER::Sequence_AddTo
* @brief      Add BER to secuence
* @ingroup    UTILS
* @date       13/05/2018 14:09:08
* @param[in]  xber : XBER to add in secuence
* @return     bool : true if is succesful
*---------------------------------------------------------------------------------------------------------------------*/
bool XBER::Sequence_AddTo(XBER& xber)
{
  if(!data)      return false;

  data->Delete();

  XBER* newxber = new XBER();
  if(!newxber) return false;

  if(!xber.Copy(newxber))
    {
      delete newxber;
      return false;
    }

  sequence.Add(newxber);

  this->type = XBERTYPE_SEQOFSEQ;
  this->size = 0;

  for(XDWORD c=0;c<sequence.GetSize();c++)
    {
      XBER* xber = (XBER*)sequence.Get(c);
      if(xber) this->size += xber->Sequence_GetSize();
    }

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
* @fn         XBER::Sequence_GetSize
* @brief      Get Secuence size
* @ingroup    UTILS
* @date       13/05/2018 14:09:50
* @param[]     : 
* @return     XDWORD : size secuence
*---------------------------------------------------------------------------------------------------------------------*/
XDWORD XBER::Sequence_GetSize()
{
  XDWORD  sizeall = 0;

  // Size type
  if(type != XBERTYPE_NULL) sizeall++;

  // Size of size;
  XBUFFER sizedata;

  CodeSize(size,sizedata);
  sizeall += sizedata.GetSize();

  sizeall += data->GetSize();

  for(XDWORD c=0;c<sequence.GetSize();c++)
    {
      XBER* xber = (XBER*)sequence.Get(c);
      if(xber) sizeall += xber->Sequence_GetSize();
    }

  return sizeall;
}



/**-------------------------------------------------------------------------------------------------------------------
* @fn         XBER::Sequence_DeleteAll
* @brief      Delete all secuence
* @ingroup    UTILS
* @date       13/05/2018 14:10:11
* @param[]     : 
* @return     bool : true if is succesful
*---------------------------------------------------------------------------------------------------------------------*/
bool XBER::Sequence_DeleteAll()
{
  if(sequence.IsEmpty()) return false;

  sequence.DeleteContents();

  sequence.DeleteAll();

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
* @fn         XBER::CodeBigNumber
* @brief      Code Big number
* @ingroup    UTILS
* @date       13/05/2018 14:10:37
* @param[in]  number : number to code
* @param[out] data : buffer big numer coded
* @return     bool : true if is succesful
*---------------------------------------------------------------------------------------------------------------------*/
bool XBER::CodeBigNumber(XDWORD number, XBUFFER& data)
{
  data.Delete();

  int value = number;
  int length = 0;

  if(value >= (268435456))
    { // 2 ^ 28
      length = 5;
    }
   else
    {
      if(value >= (2097152))
        { // 2 ^ 21
          length = 4;
        }
       else
        {
          if(value >= 16384)
            { // 2 ^ 14
              length = 3;
            }
           else
            {
              if(value >= 128)
                { // 2 ^ 7
                  length = 2;
                }
               else
                {
                  length = 1;
                }
            }
        }
    }


  int j = 0;
  for(j = length - 1; j >= 0; j--)
   {
     if(j)
      {
        int p = ((value >> (7 * j)) & 0x7F) | 0x80;
        data.Add((XBYTE)p);
      }
     else
      {
        int p = ((value >> (7 * j)) & 0x7F);
        data.Add((XBYTE)p);
      }
    }

  return true;
 }



/**-------------------------------------------------------------------------------------------------------------------
* @fn         XBER::CodeSize
* @brief      Get Code size
* @ingroup    UTILS
* @date       13/05/2018 14:11:43
* @param[in]  integer : integer to code
* @param[out] data : buffer size coded
* @return     bool : true if is succesful
*---------------------------------------------------------------------------------------------------------------------*/
bool XBER::CodeSize(XDWORD integer, XBUFFER& data)
{
  data.Delete();

  XBYTE size = 0;

  if(integer&0xFF800000)
    {
      size = 4;
    }
   else
    {
      if(integer&0x00FF8000)
        {
          size = 3;
        }
       else
        {
          if(integer&0x0000FF80)
            {
              size = 2;
            }
           else
            {
              size = 1;
            }
        }
    }

  if(!size) return false;

  if(size>1) data.Add((XBYTE)((size-1)|0x80));

  for(int c=(size-data.GetSize()-1);c>=0;c--)
    {
      XBYTE byte = (XBYTE)((c?(integer>>(c*8)):(integer))&0xFF);

      data.Add((XBYTE)byte);
    }

  return true;
 }



/**-------------------------------------------------------------------------------------------------------------------
* @fn         XBER::Clean
* @brief      Clean class variables
* @ingroup    UTILS
* @date       13/05/2018 14:12:34
* @param[]     : 
* @return     void : 
*---------------------------------------------------------------------------------------------------------------------*/
void XBER::Clean()
{
  type      = 0;
  size      = 0;
  data      = NULL;
}

