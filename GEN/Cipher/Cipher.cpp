//------------------------------------------------------------------------------------------
//  CIPHER.CPP
//
//  Cipher Interface
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 22/04/2002
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include <string.h>

#include "XFactory.h"

#include "Cipher.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------


/*-------------------------------------------------------------------
// CIPHERKEYSYMMETRICAL::CIPHERKEYSYMMETRICAL
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/03/2014 17:16:36
//

*/
/*-----------------------------------------------------------------*/
CIPHERKEYSYMMETRICAL::CIPHERKEYSYMMETRICAL() : CIPHERKEY()
{
  Clean();

  type = CIPHERKEYTYPE_SYMMETRICAL;

  xbufferkey = new XBUFFER();
}




/*-------------------------------------------------------------------
// CIPHERKEYSYMMETRICAL::~CIPHERKEYSYMMETRICAL
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/03/2014 17:18:17
//
*/
/*-----------------------------------------------------------------*/
CIPHERKEYSYMMETRICAL::~CIPHERKEYSYMMETRICAL()
{
  if(xbufferkey)  delete xbufferkey;

  Clean();
}




/*-------------------------------------------------------------------
// CIPHERKEYSYMMETRICAL::Get
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/03/2014 17:20:53
//
//  @return       XBYTE* :
//  @param        size :
*/
/*-----------------------------------------------------------------*/
XBYTE* CIPHERKEYSYMMETRICAL::Get(int& size)
{
  if(!xbufferkey) return NULL;

  size = xbufferkey->GetSize();

  return xbufferkey->Get();
}





/*-------------------------------------------------------------------
// CIPHERKEYSYMMETRICAL::Get
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/03/2014 17:21:02
//
//  @return       XBUFFER* :
*/
/*-----------------------------------------------------------------*/
XBUFFER* CIPHERKEYSYMMETRICAL::Get()
{
  return xbufferkey;
}




/*-------------------------------------------------------------------
// CIPHERKEYSYMMETRICAL::Set
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/03/2014 17:21:14
//
//  @return       bool :
//  @param        key :
//  @param        size :
*/
/*-----------------------------------------------------------------*/
bool CIPHERKEYSYMMETRICAL::Set(XBYTE* key, int size)
{
  if(!key) return false;

  this->xbufferkey->Delete();

  this->xbufferkey->Add(key, (XDWORD)size);

  return true;
}




/*-------------------------------------------------------------------
// CIPHERKEYSYMMETRICAL::Set
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/03/2014 17:21:25
//
//  @return       bool :
//  @param        key :
*/
/*-----------------------------------------------------------------*/
bool CIPHERKEYSYMMETRICAL::Set(XBUFFER& key)
{
  return Set(key.Get(), key.GetSize());
}










//-------------------------------------------------------------------
//  CIPHER::CIPHER
/**
//
//
//  @author       Abraham J. Velez
//  @version      22/04/2002 14:50:01
//
//  @return       void :
//  */
//-------------------------------------------------------------------
CIPHER::CIPHER()
{
  Clean();

  type             = CIPHERTYPE_XOR;
  paddingadjustsize = 0;

  inivector = new XBUFFER();
  result    = new XBUFFER();
}



//-------------------------------------------------------------------
//  CIPHER::~CIPHER
/**
//
//
//  @author       Abraham J. Velez
//  @version      22/04/2002 14:52:00
//
//  @return       void :
//  */
//-------------------------------------------------------------------
CIPHER::~CIPHER()
{

  if(inivector)
    {
      delete inivector;
      inivector = NULL;
    }

  if(result)
    {
      delete result;
      result = NULL;
    }

  Clean();
}



/*-------------------------------------------------------------------
// CIPHER::GetKey
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/03/2014 16:51:14
//
//  @return       CIPHERKEY* :
//  @param        type :
*/
/*-----------------------------------------------------------------*/
CIPHERKEY* CIPHER::GetKey(CIPHERKEYTYPE type)
{
  int index;

  switch(type)
    {
      case CIPHERKEYTYPE_SYMMETRICAL :
      case CIPHERKEYTYPE_PUBLIC      : index = 0;   break;
      case CIPHERKEYTYPE_PRIVATE     : index = 1;   break;
                      default        : return NULL;
    }

  return keys[index];
}



/*-------------------------------------------------------------------
//  CIPHER::SetKey
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      18/03/2014 13:50:42
//
//  @return       bool :
//
//  @param        key :
//  @param        integritycheck :
*/
/*-----------------------------------------------------------------*/
bool CIPHER::SetKey(CIPHERKEY* key, bool integritycheck)
{
  if(!key) return false;

  int index;

  switch(key->GetType())
    {
      case CIPHERKEYTYPE_SYMMETRICAL :
      case CIPHERKEYTYPE_PUBLIC      : index = 0;   break;
      case CIPHERKEYTYPE_PRIVATE     : index = 1;   break;
                default              : return false;
    }

  return SetKey(index, key);
}




/*-------------------------------------------------------------------
//  CIPHER::GetInitializationVector
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      12/03/2013 22:41:29
//
//  @return       XBYTE* :
//
*/
/*-----------------------------------------------------------------*/
XBYTE* CIPHER::GetInitVector()
{
  if(!inivector) return NULL;

  return inivector->Get();
}




/*-------------------------------------------------------------------
//  CIPHER::SetInitVector
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      12/03/2013 22:50:04
//
//  @return       bool :
//  @param        vector :
//  @param        size :
*/
/*-----------------------------------------------------------------*/
bool CIPHER::SetInitVector(XBYTE* vector, int size)
{
  if(!vector)    return false;
  if(!inivector) return false;

  inivector->Delete();
  inivector->Add(vector,size);

  return true;
}



/*-------------------------------------------------------------------
//  CIPHER::SetIniVector
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      12/03/2013 22:41:43
//
//  @return       bool :
//  @param        vector :
*/
/*-----------------------------------------------------------------*/
bool CIPHER::SetInitVector(XBUFFER& vector)
{
  inivector->Delete();
  inivector->Add(vector);

  return true;
}




//-------------------------------------------------------------------
//  CIPHER::Cipher
/**
//
//
//  @author       Abraham J. Velez
//  @version      22/04/2002 15:14:48
//
//  @return       bool :
//  @param        input :
//  @param        size :
*/
//-------------------------------------------------------------------
bool CIPHER::Cipher(XBYTE* input,int size)
{
  result->Delete();
  result->Add(input, size);

  CIPHERKEYSYMMETRICAL* key = (CIPHERKEYSYMMETRICAL*)GetKey(0);
  if(!key)                return false;
  if(!key->Get()->Get())  return false;

  int indexkey = 0;

  for(int c=0;c<size;c++)
    {
      result->Get()[c] ^= key->Get()->GetByte(indexkey);

      indexkey++;
      if(indexkey >= (int)key->Get()->GetSize()) indexkey = 0;
    }

  return true;
}



/*-------------------------------------------------------------------
//  CIPHER::Cipher
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      06/03/2013 19:35:53
//
//  @return       bool :
//  @param        input :
*/
/*-----------------------------------------------------------------*/
bool CIPHER::Cipher(XBUFFER& input)
{
  return Cipher(input.Get(), input.GetSize());
}




//-------------------------------------------------------------------
//  CIPHER::Uncipher
/**
//
//
//  @author       Abraham J. Velez
//  @version      22/04/2002 15:16:47
//
//  @return       bool :
//  @param        input :
//  @param        size :
*/
//-------------------------------------------------------------------
bool CIPHER::Uncipher(XBYTE* input,int size)
{
  return Cipher(input,size);
}




/*-------------------------------------------------------------------
//  CIPHER::Uncipher
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      06/03/2013 19:35:53
//
//  @return       bool :
//  @param        input :
*/
/*-----------------------------------------------------------------*/
bool CIPHER::Uncipher(XBUFFER& input)
{
  return Uncipher(input.Get(), input.GetSize());
}




/*-------------------------------------------------------------------
//  GetResult
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      06/03/2013 19:37:30
//
//  @return       XBYTE* :
//  @param        resultsize :
*/
/*-----------------------------------------------------------------*/
XBYTE* CIPHER::GetResult(int& resultsize)
{
  if(!result) return NULL;

  resultsize = result->GetSize();

  return result->Get();
}




/*-------------------------------------------------------------------
//  CIPHER::GetResult
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      06/03/2013 19:37:34
//
//  @return       XBUFFER* :
//
*/
/*-----------------------------------------------------------------*/
XBUFFER* CIPHER::GetResult()
{
  return result;
}



/*-------------------------------------------------------------------
//  CIPHER::GetResultString
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      16/03/2013 17:36:40
//
//  @return       bool :
//  @param        stringhex :
*/
/*-----------------------------------------------------------------*/
bool CIPHER::GetResultString(XSTRING& stringhex)
{
  stringhex.Empty();

  XBUFFER* xbuffer = GetResult();
  if(!xbuffer) return false;

  if(!xbuffer->GetSize()) return false;

  for(int c=0;c<(int)xbuffer->GetSize();c++)
    {
      XSTRING strbyte;

      strbyte.Format(__L("%02x"),xbuffer->GetByte(c));

      stringhex += strbyte;
    }

  return true;
}



//-------------------------------------------------------------------
//  CIPHER::Clean
/**
//
//
//  @author       Abraham J. Velez
//  @version      22/04/2002 15:16:47
//
//  @return
*/
//-------------------------------------------------------------------
void CIPHER::Clean()
{
  paddingtype  = XBUFFER_PADDINGTYPE_NONE;
  chainingmode = CIPHERCHAININGMODE_UNKNOWN;

  for(int c=0; c<CIPHERMAXKEYS; c++)
    {
      keys[c] = NULL;
    }

  inivector = NULL;
  result    = NULL;
}



/*-------------------------------------------------------------------
//  CIPHER::GetKey
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      06/03/2013 22:46:50
//
//  @return       CIPHERKEY* :
//  @param        index :
*/
/*-----------------------------------------------------------------*/
CIPHERKEY* CIPHER::GetKey(int index)
{
  if(index < 0)              return NULL;
  if(index >= CIPHERMAXKEYS) return NULL;

  return keys[index];
}



/*-------------------------------------------------------------------
//  CIPHER::SetKey
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      06/03/2013 22:47:46
//
//  @return       bool :
//  @param        index :
//  @param        key :
*/
/*-----------------------------------------------------------------*/
bool CIPHER::SetKey(int index, CIPHERKEY* key)
{
  if(index < 0)               return false;
  if(index >= CIPHERMAXKEYS) return false;

  this->keys[index] = key;

  return true;
}


