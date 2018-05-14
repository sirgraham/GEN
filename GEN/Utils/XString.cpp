/*------------------------------------------------------------------------------------------
//  XSTRING.CPP
//
//  Unicode String Class
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 23/11/2010 9:54:11
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#include "XBase.h"
#include "XBuffer.h"
#include "XDebug.h"

#include "XString.h"

#include "XMemory.h"


/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


XCHAR XSTRING::table64bits[XSTRING_SIZETABLE64BITS];
bool  XSTRING::istable64bitsinit=false;

/*-------------------------------------------------------------------
//  XSTRING::XSTRING
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      23/11/2010 9:55:01
//
//  @return
//  */
/*-----------------------------------------------------------------*/
XSTRING::XSTRING()
{
  Clean();

  //AllocBuffer(0);
}



/*-------------------------------------------------------------------
//  XSTRING::XSTRING
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      13/01/2011 9:49:59
//
//  @return
//  @param        initsize :
*/
/*-----------------------------------------------------------------*/
XSTRING::XSTRING(int initsize)
{
  Clean();

  ReAllocBuffer(initsize);
}



/*-------------------------------------------------------------------
//  XSTRING::XSTRING
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      23/11/2010 9:57:03
//
//  @return
//  @param        string :
*/
/*-----------------------------------------------------------------*/
XSTRING::XSTRING(const char* string)
{
  Clean();

  Set(string);
}



/*-------------------------------------------------------------------
//  XSTRING::XSTRING
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      23/11/2010 9:57:03
//
//  @return
//  @param        string :
*/
/*-----------------------------------------------------------------*/
XSTRING::XSTRING(const XCHAR* string)
{
  Clean();

  Set(string);
}


/*-------------------------------------------------------------------
//  XSTRING::XSTRING
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      01/02/2011 19:58:00
//
//  @return
//  @param        string :
//  @param        size :
*/
/*-----------------------------------------------------------------*/
XSTRING::XSTRING(const XCHAR* string,int size)
{
  Clean();

  Set(string,size);
}


/*-------------------------------------------------------------------
//  XSTRING::XSTRING
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      15/10/2011 12:03:22
//
//  @return
//  @param        string :
*/
/*-----------------------------------------------------------------*/
XSTRING::XSTRING(const XSTRING& string)
{
  Clean();

  Set(string);
}



/*-------------------------------------------------------------------
//  XSTRING::XSTRING
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      12/05/2011 18:57:50
//
//  @return
//  @param        string :
*/
/*-----------------------------------------------------------------*/
XSTRING::XSTRING(XWORD* string)
{
  Clean();

  Set(string);
}



/*-------------------------------------------------------------------
//  XSTRING::~XSTRING
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      23/11/2010 10:00:06
//
//  @return
//  */
/*-----------------------------------------------------------------*/
XSTRING::~XSTRING()
{
  FreeBuffer();

  Clean();
}



/*-------------------------------------------------------------------
//  XSTRING::Get
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      23/11/2010 10:06:17
//
//  @return       XCHAR* :
//  */
/*-----------------------------------------------------------------*/
XCHAR* XSTRING::Get() const
{
  //if(!text) ReAllocBuffer(0);
  if(!text) return __L("");

  return text;
}



/*-------------------------------------------------------------------
//  XSTRING::GetSize
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      23/11/2010 10:06:39
//
//  @return       XDWORD :
//  */
/*-----------------------------------------------------------------*/
XDWORD XSTRING::GetSize() const
{
  if(!text) return 0;

  return size;
}


/*-------------------------------------------------------------------
//  XSTRING::GetSize
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      27/04/2011 12:10:06
//
//  @return       XDWORD :
//  @param        string :
*/
/*-----------------------------------------------------------------*/
XDWORD XSTRING::GetSize(const XCHAR* string)
{
  int size = 0;

  while(string[size])
    {
      size++;
    }

  return size;
}



/*-------------------------------------------------------------------
//  XSTRING::GetSize
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      12/05/2011 19:04:39
//
//  @return       XDWORD :
//  @param        string :
*/
/*-----------------------------------------------------------------*/
XDWORD XSTRING::GetSize(XWORD* string)
{
  int size = 0;

  while(string[size])
    {
      size++;
    }

  return size;
}



/*-------------------------------------------------------------------
//  XSTRING::IsOEM
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      09/05/2013 18:46:57
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XSTRING::IsOEM()
{
  for(int c=0;c<(int)size;c++)
    {
      if((text[c]<0x20) || (text[c]>0x7F)) return false;
    }

  return true;
}



/*-------------------------------------------------------------------
//  XSTRING::CreateOEM
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      14/02/2014 13:09:13
//
//  @return       bool :
//  @param        text :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::CreateOEM(char*& _textOEM) const
{
  int sizeOEM = GetSize();

  _textOEM = new char[sizeOEM+1];
  if(!_textOEM) return false;

  for(int c=sizeOEM-1;c>=0;c--)
    {
      _textOEM[c] = (XBYTE)text[c];
    }

  _textOEM[sizeOEM]=0;

  return true;
}



/*-------------------------------------------------------------------
//  XSTRING::CreateNormalize
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      14/02/2014 13:12:15
//
//  @return       bool :
//  @param        _textnormalize :
//  @param        inverse :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::CreateNormalize(XWORD*& _textnormalize, bool inverse)
{
  int sizenormalize = GetSize()+1;

  _textnormalize = new XWORD[sizenormalize];
  if(!_textnormalize) return false;

  memset(_textnormalize, 0, (sizenormalize)*sizeof(XWORD));

  if(sizenormalize>1)
    {
      for(int c=0; c<sizenormalize; c++)
        {
          if(inverse)
            {
              _textnormalize[c]  = (text[c]>>8);
              _textnormalize[c] |= ((text[c]&0x00FF)<<8);

            }
           else
            {
              _textnormalize[c] = (XWORD)text[c];
            }
        }
    }

  return true;

}



/*-------------------------------------------------------------------
//  XSTRING::Set
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      13/01/2011 10:08:47
//
//  @return       bool :
//  @param        size :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::Set(int size)
{
  //if(!size) Empty();

  if(!ReAllocBuffer(size)) return false;

  FillChar(0);

  return true;
}



/*-------------------------------------------------------------------
//  XSTRING::Set
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      23/11/2010 21:58:51
//
//  @return       bool :
//  @param        string :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::Set(const char* string)
{
  Empty();
  if(!string) return false;

  XDWORD tsize = (XDWORD)strlen(string);

  //if(!tsize) Empty(); //DIEGO: if tsize==0 ReAllocBuffer calls Empty wich, btw, was already called just 2 lines ago! 3 calls to freebuffer for 1 assignment, really?

  if(!ReAllocBuffer(tsize)) return false;

  if(tsize)
    {
      for(XDWORD c=0;c<tsize;c++)
        {
          text[c] = (XCHAR)string[c];
        }
    }

  return true;
}




/*-------------------------------------------------------------------
//  XSTRING::Set
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      27/03/2012 12:37:55
//
//  @return       bool :
//  @param        string :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::Set(const XCHAR* string)
{
  /*
  Empty();
  if(!string) return false;

  XDWORD c      = XSTRING::GetSize((XCHAR*)string);
  XDWORD tsize = c;

  if(!tsize) Empty();

  if(!ReAllocBuffer(tsize)) return false;

  if(tsize)
    {
      memcpy(text, string, sizeof(XCHAR)*tsize);


      //for(c=0;c<tsize;c++)
      //  {
      //    text[c] = string[c];
      //  }
    }
  */


  if(!string)
    {
      Empty();
      return false;
    }

  XDWORD c     = XSTRING::GetSize((XCHAR*)string);
  XDWORD tsize = c;

  if(tsize)
    {
      FreeBuffer();

      if(!ReAllocBuffer(tsize)) return false;

      memcpy(text, string, sizeof(XCHAR)*tsize);
    }
   else Empty();

  return true;
}




/*-------------------------------------------------------------------
//  XSTRING::Set
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      01/02/2011 19:56:30
//
//  @return       bool :
//  @param        string :
//  @param        size :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::Set(const XCHAR* string, int size)
{
  Empty();
  if(!string) return false;

  XDWORD tsize = size;

  if(!tsize) Empty();

  if(!ReAllocBuffer(tsize)) return false;

  if(tsize)
    {
      memcpy(text, string, sizeof(XCHAR)*tsize);

      /*
      for(int c=0;c<(int)tsize;c++)
        {
          text[c] = string[c];
        }
      */
    }

  return true;
}



/*-------------------------------------------------------------------
//  XSTRING::Set
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      15/10/2011 12:01:40
//
//  @return       bool :
//  @param        string :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::Set(const XSTRING& string)
{
  XDWORD tsize = string.GetSize();

  if(!tsize) Empty();

  if(!ReAllocBuffer(tsize)) return false;

  XCHAR* _text = string.Get();

  if(tsize)
    {
      memcpy(text, _text, sizeof(XCHAR)*tsize);

      /*
      for(XDWORD c=0;c<tsize;c++)
        {
          text[c] = _text[c];
        }
      */
    }

  return true;
}



/*-------------------------------------------------------------------
//  XSTRING::Set
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      12/05/2011 18:53:53
//
//  @return       bool :
//  @param        string :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::Set(XWORD* string)
{
  //Empty();
  if(!string)
  {
    Empty();
    return false;
  }

  XDWORD tsize = GetSize(string);
  if(!tsize) Empty();

  if(!ReAllocBuffer(tsize)) return false;

  if(tsize)
    {
      for(XDWORD c=0;c<tsize;c++)
        {
          text[c] = (XCHAR)string[c];
        }
    }

  return true;
}


/*-------------------------------------------------------------------
//  XSTRING::Add
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2011 18:42:08
//
//  @return       bool :
//  @param        character :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::Set(XCHAR character)
{
  XCHAR characters[2] = { 0, 0};

  characters[0] = character;

  return Set(characters);
}



/*-------------------------------------------------------------------
//  XSTRING::Set
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2011 19:01:09
//
//  @return       bool :
//  @param        str1 :
//  @param        str2 :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::Set(XCHAR* str1,const XCHAR* str2)
{
  if(!str1) return false;
  if(!str2) return false;

  int c=0;
  while(str2[c])
    {
      str1[c] = str2[c];
      c++;
    }

  str1[c] = 0;

  return true;
}


/*-------------------------------------------------------------------
//  XSTRING::Set
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      30/07/2012 11:12:23
//
//  @return       bool :
//  @param        buffer :
//  @param        size :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::Set(const XBYTE* buffer, int size)
{
  Empty();
  if(!buffer) return false;

  XDWORD tsize = size;

  if(!tsize) Empty();

  if(!ReAllocBuffer(tsize)) return false;

  if(tsize)
    {
      for(int c=0;c<(int)tsize;c++)
        {
          text[c] = buffer[c];
        }
    }

  return true;

}

/*-------------------------------------------------------------------
//   XSTRING::Set
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  26/01/2018 12:02:22
//  @return   bool :
//
//  @param    const :
//  @param    int :
//
*//*-----------------------------------------------------------------*/
bool XSTRING::Set(const XDWORD* buffer, int size)
{
    Empty();
    if (!buffer) return false;

    XDWORD tsize = size;

    if (!tsize) Empty();

    if (!ReAllocBuffer(tsize)) return false;

    if (tsize)
        for (int c = 0; c<(int)tsize; c++)
            text[c] = buffer[c];


    return true;

}


/*-------------------------------------------------------------------
//  XSTRING::Set
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      29/05/2014 13:39:03
//
//  @return       bool :
//
//  @param        buffer :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::Set(XBUFFER& buffer)
{
  return Set(buffer.Get(), buffer.GetSize());
}




/*-------------------------------------------------------------------
//  XSTRING::Add
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      09/01/2011 17:14:06
//
//  @return       bool :
//  @param        string :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::Add(const char* string)
{
  XDWORD tsize  = (XDWORD)strlen(string);
  XDWORD ssize  = GetSize();
  XDWORD  ini;

  if(!tsize) return false;

  ini = 0;

  if(text)
    {
      while(text[ini])
        {
          ini++;
          if(ini>=(ssize+tsize))
            {
              ini--;
              break;
            }
        }
    }

  if(!ReAllocBuffer(ssize+tsize)) return false;

  int d=0;
  for(XDWORD c=0;c<tsize;c++)
    {
      text[c+ini] = (XCHAR)string[d];
      d++;
    }


  return true;
}



/*-------------------------------------------------------------------
//  XSTRING::Add
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      09/01/2011 17:13:29
//
//  @return       bool :
//  @param        string :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::Add(const XCHAR* string)
{
  if(!string[0]) return false;

  XSTRING* xstring = new XSTRING(string);
  if(!xstring) return false;

  Add((*xstring));

  delete xstring;

  return true;
}



/*-------------------------------------------------------------------
//  XSTRING::Add
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      09/01/2011 17:14:02
//
//  @return       bool :
//  @param        string :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::Add(XSTRING& string)
{
  XDWORD tsize = string.GetSize();
  XDWORD ssize = GetSize();
  XDWORD  ini;

  if(string.IsEmpty()) return false;

  if(string.Get())
    {
      if(!string.Get()[0]) return false;
    }

  if(!tsize) return false;

  ini = 0;

  if(text)
    {
      while(text[ini])
        {
          ini++;
          if(ini>=(ssize+tsize))
            {
              ini--;
              break;
            }
        }
  }

  if(!ReAllocBuffer(ssize+tsize)) return false;

  memcpy(&text[ini], string.Get(), sizeof(XCHAR)*tsize);

  /*
  int d=0;
  for(XDWORD c=0;c<tsize;c++)
    {
      text[c+ini] = (XCHAR)string[d];
      d++;
    }
  */

  return true;
}



/*-------------------------------------------------------------------
//  XSTRING::Add
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2011 18:42:08
//
//  @return       bool :
//  @param        character :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::Add(XCHAR character)
{
  XCHAR characters[2] = { 0, 0};

  characters[0] = character;

  return Add(characters);
}



/*-------------------------------------------------------------------
//  XSTRING::Add
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2011 19:01:09
//
//  @return       bool :
//  @param        str1 :
//  @param        str2 :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::Add(XCHAR* str1,const XCHAR* str2)
{
  if(!str1) return false;
  if(!str2) return false;

  int sizestr1 = XSTRING::GetSize(str1);
  int c        = 0;

  while(str2[c])
    {
      str1[sizestr1+c] = str2[c];
      c++;
    }

  str1[sizestr1+c] = 0;

  return true;
}



/*-------------------------------------------------------------------
//  XSTRING::Add
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      29/05/2014 13:40:42
//
//  @return       bool :
//
//  @param        buffer :
//  @param        size :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::Add(const XBYTE* buffer, int size)
{
  int ini   = this->size;
  int tsize = this->size + size;

  AdjustSize(tsize);

  for(int c=0;c<size;c++)
    {
      text[ini+c] = (XCHAR)buffer[c];
    }

  return true;
}



/*-------------------------------------------------------------------
//  XSTRING::Add
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      29/05/2014 13:41:05
//
//  @return       bool :
//
//  @param        buffer :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::Add(XBUFFER& buffer)
{
  return Add(buffer.Get(), buffer.GetSize());
}





/*-------------------------------------------------------------------
//  XSTRING::FillChar
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      13/01/2011 10:01:47
//
//  @return       bool :
//  @param        character :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::FillChar(XCHAR character)
{
  if(IsEmpty()) return false;

  for(XDWORD c=0;c<size;c++)
    {
      text[c] = character;
    }

  return true;
}



/*-------------------------------------------------------------------
//  XSTRING::Empty
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      26/11/2010 11:59:07
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XSTRING::Empty()
{
  if(IsEmpty()) return false;

  FreeBuffer();

  return true;
}



/*-------------------------------------------------------------------
//  XSTRING::IsEmpty
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      26/11/2010 11:59:07
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XSTRING::IsEmpty()
{
  if((!text)||(!size)) return true;
  if(!text[0])         return true;

  return false;
}



/*-------------------------------------------------------------------
//  XSTRING::HaveCharacters
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/02/2011 16:36:39
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XSTRING::HaveCharacters()
{
  if(IsEmpty()) return false;

  for(XDWORD c=0;c<size;c++)
    {
      if(!text[c]) break;
      if(!Character_IsNOT(text[c])) return true;
    }

  return false;
}




/*-------------------------------------------------------------------
//  XSTRING::HaveOnlyNumbers
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      04/08/2014 10:10:42
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool XSTRING::HaveOnlyNumbers()
{
  if(IsEmpty()) return false;

  for(XDWORD c=0;c<size;c++)
    {
      if(!text[c]) break;
      if(!Character_IsNumber(text[c])) return false;
    }

  return true;
}




/*-------------------------------------------------------------------
//  XSTRING::DeleteCharacters
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/01/2011 19:26:04
//
//  @return       bool :
//  @param        index :
//  @param        ncharacteres :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::DeleteCharacters(XDWORD index, XDWORD ncharacteres)
{
  if(!ncharacteres)      return true;
  if(IsEmpty())          return false;

  if(index >= size) return false;
  //if(index <0)           return false;

  //if(!text[index])       return false;

  int nchar = ncharacteres;
  if((index+nchar) > size) nchar = (int)size-index;

  int c = index;
  while(text[c+nchar])
    {
      text[c] = text[c+nchar];
      c++;
    }
  text[c] = text[c + nchar];

  return ReAllocBuffer(size - nchar);
}




/*-------------------------------------------------------------------
//  XSTRING::DeleteCharacter
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/02/2011 12:02:58
//
//  @return       bool :
//  @param        character :
//  @param        context :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::DeleteCharacter(XCHAR character,XSTRINGCONTEXT context)
{
  if(IsEmpty()) return false;

  switch(context)
    {
      case XSTRINGCONTEXT_ATFIRST   :
                                        /*
                                        while(character == text[0])
                                        {
                                          DeleteCharacters(0,1);
                                          if(!size) break;
                                        }
                                        */
                                        {
                                          XDWORD n=0;
                                          while(character == text[n] && n<size)
                                          {
                                          n++;
                                          }
                                          if (n)
                                          DeleteCharacters(0,n);
                                        }
                                      break;

      case XSTRINGCONTEXT_ATEND     :
                                      {
                                          XDWORD n=size-1;
                                          while(character == text[n] && n)
                                          {
                                          n--;
                                          }
                                          if (size!=n+1)
                                          ReAllocBuffer(n+1);
                                          /*
                                        while(character == text[size-1])
                                        {
                                          DeleteCharacters(size-1,1);
                                          if(!size) break;
                                        }       */
                                      }
                                      break;

      case XSTRINGCONTEXT_ALLSTRING : { XDWORD c = 0;
                                        XDWORD a = 0;

                                        while (c < size)
                                        {
                                            while (text[c]==character)
                                                c++;
                                            text[a] = text[c];
                                                c++;
                                                if (!text[a])
                                                    break;
                                                a++;
                                        }
                                        ReAllocBuffer(a);
                                        size = a;
                                        /*
                                        while(c<size)
                                          {
                                            if(text[c]==character)
                                                   DeleteCharacters(c,1);
                                              else c++;
                                          }
                                          */
                                      }
                                      break;
    }

  return true;
}



/*-------------------------------------------------------------------
//  XSTRING::DeleteNoCharacters
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/02/2011 16:47:05
//
//  @return       bool :
//  @param        context :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::DeleteNoCharacters(XSTRINGCONTEXT context)
{
  bool status = DeleteCharacter(__C('\x09'),context);

  if(status) status = DeleteCharacter(__C('\x0A'),context);
  if(status) status = DeleteCharacter(__C('\x0D'),context);
  if(status) status = DeleteCharacter(__C('\x20'),context);

  return status;
}


/*-------------------------------------------------------------------
//   XSTRING::DeleteNoCharacters
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  29/01/2018 12:31:08
//  @return   bool :
//
//  @param    XCHAR* :
//  @param    int :
//  @param    XSTRINGCONTEXT :
//
*//*-----------------------------------------------------------------*/
int XSTRING::DeleteNoCharacters(XCHAR* n, int start, XSTRINGCONTEXT context)
{
    int a = 0;
    XSTRING needle;
    needle.Set(n);
    if (!this->GetSize())
        return 0;

    switch (context)
    {
    case XSTRINGCONTEXT_ATFIRST:
    {
        XDWORD n;
        for (n = start; n < size; n++)
        {
            XCHAR character = text[n];
            bool found = false;

            for (XDWORD e = 0; needle.Get()[e] != __C('\0'); e++)
                if (character == needle.Get()[e])
                {
                    found = true;
                    break;
                }

            if (!found)
                break;
        }
        this->DeleteCharacters(0, n);
        return n;
    }
    //break;

    case XSTRINGCONTEXT_ATEND:
    {
        int n;
        for (n = size- start-1; n >=0; n--)
        {
            XCHAR character = text[n];
            bool found = false;

            for (XDWORD e = 0;  needle.Get()[e] != __C('\0'); e++)
                if (character == needle.Get()[e])
                {
                    found = true;
                    break;
                }

            if (!found)
                break;
        }
        this->DeleteCharacters(n+1, size -1 - n);
        return n;
    }
    //break;

    case XSTRINGCONTEXT_ALLSTRING:
        for (XDWORD n = start; n < size; n++)
        {
            XCHAR character = text[n];
            bool found = false;

            for (XDWORD e = 0; needle.Get()[e] != __C('\0'); e++)
                if (character == needle.Get()[e])
                {
                    found = true;
                    break;
                }
            if (found)
                continue;

            text[a] = character;
            a++;
        }
        text[a] = __C('\0');
    }
    this->AdjustSize();
    return true;
}




/*-------------------------------------------------------------------
//  XSTRING::Fast_DeleteNoCharacters
*/
/**
//
//
//
//  @author       Diego
//  @version      18/01/2017 10:25:54
//
//  @return       bool :
//
//  @param        context :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::Fast_DeleteNoCharacters(XSTRINGCONTEXT context)
{
  switch(context)
    {
      case XSTRINGCONTEXT_ATFIRST   :
      case XSTRINGCONTEXT_ATEND     :  return XSTRING::DeleteNoCharacters(context);

      case XSTRINGCONTEXT_ALLSTRING : { XSTRING copy;

                                        copy.ReAllocBuffer(size);

                                        int c=0;

                                        for(XDWORD n=0; n<size; n++)
                                          {
                                            XCHAR character=text[n];

                                            if(character ==__C('\x09')) continue;
                                            if(character ==__C('\x0A')) continue;
                                            if(character ==__C('\x0D')) continue;
                                            if(character ==__C('\x20')) continue;

                                            copy.Get()[c]=character;
                                            c++;
                                          }

                                        copy.Get()[c]=__C('\0');

                                        Set(copy);

                                      }
                                      break;

      }

  return true;
}




/*-------------------------------------------------------------------
//  XSTRING::Character_IsAlpha
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      26/11/2010 16:40:09
//
//  @return       bool :
//  @param        character :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::Character_IsAlpha(XCHAR character) const
{
  XBYTE _char = (XBYTE)character;

  if(((_char>='a') && (_char<='z')) ||
     ((_char>='A') && (_char<='Z'))) return true;

  return false;
}

/*-------------------------------------------------------------------
//  XSTRING::Character_IsUpperCase
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      26/11/2010 16:40:09
//
//  @return       bool :
//  @param        character :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::Character_IsUpperCase(XCHAR character)
{
  XBYTE _char = (XBYTE)character;

  if(((_char>='A') && (_char<='Z'))) return true;

  return false;
}

/*-------------------------------------------------------------------
//  XSTRING::Character_IsLowerCase
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      26/11/2010 16:40:09
//
//  @return       bool :
//  @param        character :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::Character_IsLowerCase(XCHAR character)
{
  XBYTE _char = (XBYTE)character;

  if(((_char>='a') && (_char<='z'))) return true;

  return false;
}

/*-------------------------------------------------------------------
//  XSTRING::Character_IsNumber
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      04/08/2014 10:08:38
//
//  @return       bool :
//
//  @param        character :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::Character_IsNumber(XCHAR character)
{
  XBYTE _char = (XBYTE)character;

  if((_char>='0') && (_char<='9'))  return true;

  return false;
}



/*-------------------------------------------------------------------
//  XSTRING::Character_IsNOT
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      09/02/2011 13:05:48
//
//  @return       bool :
//  @param        character :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::Character_IsNOT(XCHAR character)
{
  switch(character)
    {
      case 0x0000: return true;
      case 0x0009: return true; // Tab
      case 0x000A: return true; // Return
      case 0x000D: return true; // Return
      case 0x0020: return true; // Space
    }

  return false;
}


/*-------------------------------------------------------------------
//  XSTRING::Character_ToUpper
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      26/11/2010 17:04:36
//
//  @return       XCHAR :
//  @param        character :
*/
/*-----------------------------------------------------------------*/
XCHAR XSTRING::Character_ToUpper(XCHAR character) const
{
  //if(!Character_IsUpperCase(character)) return character; //no hace falta, el if de abajo solo cambia la letra si es minuscula

  XCHAR _character = character;
  XBYTE _char      = (XBYTE)_character;

  if((_char>='a')&&(_char<='z'))
    {
       _char      -= 32;
       _character  = (XCHAR)_char;
    }

  return _character;
}



/*-------------------------------------------------------------------
//  XSTRING::Character_ToUpper
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      26/11/2010 17:04:36
//
//  @return       XCHAR :
//  @param        character :
*/
/*-----------------------------------------------------------------*/
XCHAR XSTRING::Character_ToLower(XCHAR character) const
{
  //if(!Character_IsCase(character)) return character; //no hace falta, el if de abajo solo cambia la letra si es minuscula

  XCHAR _character = character;
  XBYTE _char      = (XBYTE)_character;

  if((_char>='A')&&(_char<='Z'))
    {
       _char      += 32;
       _character  = (XCHAR)_char;
    }

  return _character;
}



/*-------------------------------------------------------------------
//  XSTRING::Character_GetFirst
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      23/03/2011 10:15:03
//
//  @return       XCHAR :
//  */
/*-----------------------------------------------------------------*/
XCHAR XSTRING::Character_GetFirst()
{
  if(IsEmpty()) return 0;

  return text[0];
}



/*-------------------------------------------------------------------
//  XSTRING::Character_GetLast
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      15/03/2011 16:16:10
//
//  @return       XCHAR :
//  */
/*-----------------------------------------------------------------*/
XCHAR XSTRING::Character_GetLast()
{
  int c;

  if(IsEmpty()) return 0;

  for(c=0;c<(int)size;c++)
    {
      if(!text[c]) break;
    }

  if(c-1) return text[c-1];

  return 0;
}



/*-------------------------------------------------------------------
//  XSTRING::Character_Change
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      14/03/2011 10:43:40
//
//  @return       bool :
//  @param        source :
//  @param        target :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::Character_Change(XCHAR source,XCHAR target)
{
  if(IsEmpty()) return false;

  bool status = false;

  for(XDWORD c=0;c<size;c++)
    {
      if(text[c]==source)
        {
          text[c] = target;
          status  = true;
        }
    }

  return status;
}




/*-------------------------------------------------------------------
//  XSTRING::ToUpperCase
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      26/11/2010 18:24:52
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XSTRING::ToUpperCase()
{
  if(IsEmpty()) return false;

  for(XDWORD c=0;c<size;c++)
    {
      XCHAR _char = text[c];

      if(Character_IsAlpha(text[c])) _char = Character_ToUpper(text[c]);

      text[c] = _char;
    }

  return true;
}



/*-------------------------------------------------------------------
//  XSTRING::ToLowerCase
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      26/11/2010 18:24:52
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XSTRING::ToLowerCase()
{
  if(IsEmpty()) return false;

  for(XDWORD c=0;c<size;c++)
    {
      XCHAR _char = text[c];

      if(Character_IsAlpha(text[c])) _char = Character_ToLower(text[c]);

      text[c] = _char;
    }

  return true;
}



/*-------------------------------------------------------------------
//  XSTRING::Compare
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      18/02/2011 12:30:29
//
//  @return       int :
//  @param        string :
//  @param        ignorecase :
*/
/*-----------------------------------------------------------------*/
/*
int XSTRING::Compare(const XCHAR* string, bool ignorecase)
{
  XSTRING stringtemp(string);

  return Compare(stringtemp, ignorecase);
}*/

int XSTRING::Compare(const XCHAR* string, int csize, bool ignorecase)
{
  XDWORD stringsize=csize;

  if(size!=stringsize)
    {
      if(size>stringsize) return  1;
      if(size<stringsize) return -1;
    }

  if(!size && !stringsize) return 0;

  XCHAR* _text = (XCHAR*)string;

  if(ignorecase)
    {
      for(int c=size-1;c>=0;c--)
        {
          XCHAR a=_text[c];
          XCHAR b=text[c];

          if(Character_IsAlpha(a))
            {
              XCHAR c1 = Character_ToUpper(a);
              XCHAR c2 = Character_ToUpper(b);

              if(c1 == c2) continue;
              if(c1 < c2) return  1;
              if(c1 > c2) return -1;
            }
           else
            {
              if(a == b)  continue;
              if(a < b)   return  1;
              if(a > b)   return -1;
            }
        }
    }
   else
    {
      for(int c=size-1;c>=0;c--)
        {
          if(_text[c] == text[c]) continue;
          if(_text[c] < text[c])  return  1;
          if(_text[c] > text[c])  return -1;
        }
    }

  return 0;
}

int XSTRING::Compare(const XCHAR* string, bool ignorecase)
{
  XDWORD stringsize=XSTRING::GetSize(string);

  if(size!=stringsize)
    {
      if(size>stringsize) return  1;
      if(size<stringsize) return -1;
    }

  if(!size && !stringsize) return 0;

  XCHAR* _text = (XCHAR*)string;

  if(ignorecase)
    {
      for(int c=size-1;c>=0;c--)
        {
          XCHAR a=_text[c];
          XCHAR b=text[c];

          if(Character_IsAlpha(a))
            {
              XCHAR c1 = Character_ToUpper(a);
              XCHAR c2 = Character_ToUpper(b);

              if(c1 == c2) continue;
              if(c1 < c2) return  1;
              if(c1 > c2) return -1;
            }
           else
            {
              if(a == b)  continue;
              if(a < b)   return  1;
              if(a > b)   return -1;
            }
        }
    }
   else
    {
      for(int c=size-1;c>=0;c--)
        {
          if(_text[c] == text[c]) continue;
          if(_text[c] < text[c])  return  1;
          if(_text[c] > text[c])  return -1;
        }
    }

  return 0;
}


/*-------------------------------------------------------------------
//  XSTRING::Compare
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      20/04/2011 0:41:41
//
//  @return       int :
//  @param        string :
//  @param        ignorecase :
*/
/*-----------------------------------------------------------------*/
int XSTRING::Compare(const char* string, bool ignorecase)
{
  XSTRING stringtemp(string);

  return Compare(stringtemp, ignorecase);
}


/*-------------------------------------------------------------------
//  XSTRING::Compare
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      15/03/2016 10:58:15
//
//  @return       int :
//
//  @param        string :
//  @param        string2 :
//  @param        ignorecase :
*/
/*-----------------------------------------------------------------*/
int XSTRING::Compare(const XCHAR* string, const XCHAR* string2, bool ignorecase)
{
  XSTRING a;
  a.Set(string);
  return a.Compare(string2,ignorecase);
}




/*-------------------------------------------------------------------
//  XSTRING::Compare
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      26/11/2010 12:14:14
//
//  @return       int :
//  @param        string :
//  @param        ignorecase :
*/
/*-----------------------------------------------------------------*/
/*int XSTRING::Compare(XSTRING& string, bool ignorecase)
{
  bool iEmpty=IsEmpty();
  bool sempty=string.IsEmpty();

  if(iEmpty && (!sempty))
    {
      return 1;
    }
   else
    {
      if((!iEmpty) && sempty)
        {
          return -1;
        }
       else
        {
          if(iEmpty && sempty)
            {
              return 0;
            }
        }
    }

  XDWORD stringsize=string.GetSize();

  if(size>stringsize) return  1;
  if(size<stringsize) return -1;

  XCHAR* _text = string.Get();

  if(ignorecase)
    {
      for(long long c=size-1;c>=0;c--)
        {
          if(Character_IsCase(_text[c]))
            {
              XCHAR c1 = Character_ToUpper(_text[c]);
              XCHAR c2 = Character_ToUpper(text[c]);

              if(c1 < c2) return  1;
              if(c1 > c2) return -1;
            }
           else
            {
              if(_text[c] < text[c]) return  1;
              if(_text[c] > text[c]) return -1;
            }
        }
    }
   else
    {
      for(long long c=size-1;c>=0;c--)
        {
          if(_text[c] == text[c]) continue;
          if(_text[c] < text[c])  return  1;
          if(_text[c] > text[c])  return -1;
        }
    }

  return 0;
}*/

int XSTRING::Compare(const XSTRING& string, bool ignorecase) const
{

  XDWORD stringsize = string.GetSize();

  if(size != stringsize)
    {
      if(size > stringsize) return  1;
      if(size < stringsize) return -1;
    }

  if(!size && !stringsize) return 0;

  XCHAR* _text = string.Get();

  if(ignorecase)
    {
      for(int c=size-1;c>=0;c--)
        {
          if(Character_IsAlpha(_text[c]))
            {
              XCHAR c1 = Character_ToUpper(_text[c]);
              XCHAR c2 = Character_ToUpper(text[c]);

              if(c1 == c2) continue;
              if(c1 < c2) return  1;
              if(c1 > c2) return -1;
            }
           else
            {
              if(_text[c] == text[c]) continue;
              if(_text[c] < text[c]) return  1;
              if(_text[c] > text[c]) return -1;
            }
        }
    }
   else
    {
      for(int c=size-1;c>=0;c--)
        {
          if(_text[c] == text[c]) continue;
          if(_text[c] < text[c])  return  1;
          if(_text[c] > text[c])  return -1;
        }
    }

  return 0;
}



/*-------------------------------------------------------------------
//  XSTRING::Find
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      07/02/2011 12:45:03
//
//  @return       int :
//  @param        string :
//  @param        ignorecase :
//  @param        startindex :
*/
/*-----------------------------------------------------------------*/
int XSTRING::Find(const XCHAR* string, bool ignorecase, int startindex)
{
  XSTRING unistring(string);

  return Find(unistring,ignorecase,startindex);
}



/*-------------------------------------------------------------------
//  XSTRING::Find
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      07/02/2011 11:48:39
//
//  @return       int :
//  @param        string :
//  @param        ignorecase :
//  @param        startindex :
*/
/*-----------------------------------------------------------------*/
int XSTRING::Find(XSTRING& string, bool ignorecase,int startindex)
{
  if(IsEmpty())                      return XSTRING_NOTFOUND;
  if(string.IsEmpty())               return XSTRING_NOTFOUND;

  XCHAR* _text = string.Get();
  XDWORD _size = string.GetSize();

  if(_size > size)                    return XSTRING_NOTFOUND;
  if(startindex>=(int)(size-_size)+1) return XSTRING_NOTFOUND;
  if(startindex<0)                    return XSTRING_NOTFOUND;

  bool found;

  for(XDWORD c=startindex;c<(size-_size)+1;c++)
    {
      found = true;

      for(XDWORD d=0;d<_size;d++)
        {
          if(ignorecase && Character_IsAlpha(_text[d]))
            {
              XCHAR c1 = Character_ToUpper(_text[d]);
              XCHAR c2 = Character_ToUpper(text[c+d]);

              if(c1!=c2)
                {
                  found = false;
                  break;
                }
            }
           else
            {
              if(_text[d]!=text[c+d])
                {
                  found = false;
                  break;
                }
            }
        }

      if(found) return c;
    }

  return XSTRING_NOTFOUND;
}



/*-------------------------------------------------------------------
//  XSTRING::FindCharacter
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/02/2011 22:39:05
//
//  @return       int :
//  @param        character :
//  @param        startindex :
//  @param        endtostart :
*/
/*-----------------------------------------------------------------*/
int XSTRING::FindCharacter(XCHAR character, XDWORD startindex, bool endtostart)
{
  if(startindex>=(XDWORD)size)
    return XSTRING_NOTFOUND;

//if(startindex<0)          return XSTRING_NOTFOUND;

  if(endtostart)
    {

      //---------- BUG!
      //for(c=size-startindex;c>0;c--)
      for(int c=size-startindex-1;c>=0;c--)
        {
          if(text[c]==character) return c;
        }
    }
   else
    {
      for(XDWORD c=startindex;c<size;c++)
        {
          if(text[c]==character)
            return c;
        }
    }

  return XSTRING_NOTFOUND;
}



/*-------------------------------------------------------------------
//  XSTRING::FindCharacter
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/02/2011 22:39:05
//
//  @return       int :
//  @param        character :
//  @param        startindex :
//  @param        endtostart :
*/
/*-----------------------------------------------------------------*/
XDWORD XSTRING::CountCharacter(XCHAR character, int startindex, bool endtostart)
{
  XDWORD c;

  XDWORD count=0;

  if(startindex>=(int)size) return 0;
  if(startindex<0)          return 0;

  if(endtostart)
    {
      for(c=size-startindex;c>0;c--)
        {
          if(text[c]==character) count++;
        }
    }
   else
    {
      for(c=startindex;c<size;c++)
        {
          if(text[c]==character) count++;
        }
    }

  return count;
}



/*-------------------------------------------------------------------
//  XSTRING::FindDiferentCharacter
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      12/09/2012 10:08:42
//
//  @return       int :
//  @param        character :
//  @param        startindex :
//  @param        endtostart :
*/
/*-----------------------------------------------------------------*/
int XSTRING::FindDiferentCharacter(XCHAR character, int startindex, bool endtostart)
{
  XDWORD c;

  if(startindex>=(int)size) return XSTRING_NOTFOUND;
  if(startindex<0)          return XSTRING_NOTFOUND;

  if(endtostart)
    {
      for(c=size-startindex;c>0;c--)
        {
          if(text[c]!=character) return c;
        }
    }
   else
    {
      for(c=startindex;c<size;c++)
        {
          if(text[c]!=character) return c;
        }
    }

  return XSTRING_NOTFOUND;
}




/*-------------------------------------------------------------------
//  XSTRING::GetNCharacters
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      11/05/2014 11:07:29
//
//  @return       int :
//
//  @param        character :
*/
/*-----------------------------------------------------------------*/
int XSTRING::GetNCharacters(XCHAR character)
{
  int count = 0;

  for(XDWORD c=0; c<GetSize(); c++)
    {
      if(text[c]==character) count++;
    }

  return count;
}




/*-------------------------------------------------------------------
//  XSTRING::AreValidCharacters
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      12/09/2014 11:54:48
//
//  @return       bool :
//
//  @param        validcharacters :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::AreValidCharacters(XCHAR* validcharacterslist)
{
  if(!validcharacterslist) return false;

  XSTRING vcl;

  vcl = validcharacterslist;

  for(int d=0; d<(int)GetSize(); d++)
    {
      bool isvalid = false;

      for(int c=0; c<(int)vcl.GetSize(); c++)
        {
          if(Get()[d] == vcl.Get()[c])
            {
              isvalid = true;
              break;
            }
        }

      if(!isvalid)
        return false;
    }

  return true;
}



/*-------------------------------------------------------------------
//  XSTRING::Copy
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      31/03/2013 23:14:13
//
//  @return       int :
//  @param        startmark :
//  @param        endmark :
//  @param        ignorecase :
//  @param        addstartindex :
//  @param        string :
*/
/*-----------------------------------------------------------------*/
int XSTRING::Copy(const XCHAR* startmark, const XCHAR* endmark, bool ignorecase, int addstartindex, XSTRING& string)
{
  int startindex  = 0;
  int endindex    = GetSize();
  int sizeendmark = 0;

  string.Empty();

  if(startmark)
    {
      XSTRING mark;

      mark = startmark;
      startindex = Find(startmark, ignorecase, addstartindex);
      if(startindex==XSTRING_NOTFOUND) return XSTRING_NOTFOUND;

      startindex+= mark.GetSize();
    }

  if(endmark)
    {
      XSTRING mark;

      mark = endmark;
      endindex = Find(endmark, ignorecase, startindex);
      if(endindex==XSTRING_NOTFOUND) endindex = size;

      sizeendmark = mark.GetSize();
    }

  for(int c=startindex;c<endindex;c++)
    {
      XCHAR str[2] = { text[c], 0 };

      string+=str;
    }

  int indexnext = (endindex+sizeendmark);

  if(indexnext>(int)size) indexnext = (int)size;

  return indexnext;
}





/*-------------------------------------------------------------------
//  XSTRING::Copy
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      31/03/2013 23:05:26
//
//  @return       int :
//  @param        startindex :
//  @param        endmark :
//  @param        ignorecase :
//  @param        string :
*/
/*-----------------------------------------------------------------*/
int XSTRING::Copy(int startindex, const XCHAR* endmark, bool ignorecase, XSTRING& string)
{
  int endindex    = GetSize();
  int sizeendmark = 0;

  string.Empty();

  if(endmark)
    {
      XSTRING mark;

      mark = endmark;
      endindex = Find(endmark, ignorecase, startindex);
      if(endindex==XSTRING_NOTFOUND) endindex = size;

      sizeendmark = mark.GetSize();
    }

  if(startindex>=endindex) return XSTRING_NOTFOUND;

  for(int c=startindex;c<endindex;c++)
    {
      XCHAR str[2] = { text[c], 0 };

      string+=str;
    }

  int indexnext = (endindex+sizeendmark);

  if(indexnext>(int)size) indexnext = (int)size;

  return indexnext;
}





/*-------------------------------------------------------------------
//  XSTRING::Copy
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      31/03/2013 23:16:03
//
//  @return       int :
//  @param        startmark :
//  @param        endindex :
//  @param        ignorecase :
//  @param        addstartindex :
//  @param        string :
*/
/*-----------------------------------------------------------------*/
int XSTRING::Copy(const XCHAR* startmark, int endindex, bool ignorecase, int addstartindex, XSTRING& string)
{
  int startindex    = 0;
  int sizeendmark = 0;

  string.Empty();

  if(startmark)
    {
      XSTRING mark;

      mark = startmark;
      startindex = Find(startmark, ignorecase, addstartindex);
      if(startindex==XSTRING_NOTFOUND) return XSTRING_NOTFOUND;

      startindex+= mark.GetSize();
    }

  if(startindex>=endindex) return XSTRING_NOTFOUND;

  for(int c=startindex;c<endindex;c++)
    {
      XCHAR str[2] = { text[c], 0 };

      string+=str;
    }

  int indexnext = (endindex+sizeendmark);

  if(indexnext>(int)size) indexnext = (int)size;

  return indexnext;
}




/*-------------------------------------------------------------------
//  XSTRING::Copy
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      31/03/2013 23:16:15
//
//  @return       int :
//  @param        startindex :
//  @param        endindex :
//  @param        string :
*/
/*-----------------------------------------------------------------*/
int XSTRING::Copy(int startindex, int endindex, XSTRING& string)
{
  /*
  XCHAR str[2] = { 0 , 0 };


  string.Empty();

  if(startindex>=endindex) return XSTRING_NOTFOUND;

  string.Empty();
  string.ReAllocBuffer(endindex-startindex);

  for(int c=startindex;c<endindex;c++)
    {
      str[0] = text[c];
      string.Add(str);
    }
    */

  if(startindex>=endindex)
    return XSTRING_NOTFOUND;

  string.ReAllocBuffer(endindex-startindex);
  memcpy(string.Get(),&(this->text[startindex]),(endindex-startindex)*sizeof(XCHAR));

  return size;
}




/*-------------------------------------------------------------------
//  XSTRING::Copy
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      31/03/2013 23:16:29
//
//  @return       int :
//  @param        startindex :
//  @param        string :
*/
/*-----------------------------------------------------------------*/
int XSTRING::Copy(int startindex, XSTRING& string)
{
  /*
  XCHAR str[2]   = { 0 , 0 };
  int   endindex = GetSize();

  string.Empty();

  if(startindex>=endindex) return XSTRING_NOTFOUND;

  for(int c=startindex;c<endindex;c++)
    {
      str[0] = text[c];
      string.Add(str);
    }

  return (endindex-startindex);
  */
  return Copy(startindex, GetSize(),string);
}


/*-------------------------------------------------------------------
//  XSTRING::Replace
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      09/09/2015 12:50:16
//
//  @return       int :
//
//  @param        findWhat :
//  @param        replaceBy :
*/
/*-----------------------------------------------------------------*/
int XSTRING::Replace(XCHAR* findwhat, XCHAR* replaceby)
{
  XSTRING what;
  XSTRING replace;

  what.Set(findwhat);
  replace.Set(replaceby);

  int found,coincidences=0;
  do{
      found = ReplaceFirst(what.Get(),replace.Get());
      if (found!=NOTFOUND)
        coincidences++;
      else
        break;
    } while(1);

  return coincidences;
}


/*-------------------------------------------------------------------
//  XSTRING::ReplaceFirst
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      09/09/2015 13:34:14
//
//  @return       int :
//
//  @param        findWhat :
//  @param        replaceBy :
*/
/*-----------------------------------------------------------------*/
int XSTRING::ReplaceFirst(XCHAR* findwhat, XCHAR* replaceby)
{
  XSTRING what;
  XSTRING replace;

  what.Set(findwhat);
  replace.Set(replaceby);

  int whatsize = what.GetSize();
  int found    = 0;

  found         = this->Find(findwhat, false, found);
  if(found!=NOTFOUND)
    {
      DeleteCharacters(found, whatsize);
      Insert(replace, found);
    }

  return found;
}


/*-------------------------------------------------------------------
//  XSTRING::Insert
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      09/09/2015 13:19:19
//
//  @return       int :
//
//  @param        str :
//  @param        position :
*/
/*-----------------------------------------------------------------*/
bool  XSTRING::Insert (XSTRING str,XDWORD position)
{
  XSTRING tmp;
  if (position> this->size)
  {
    return this->Add(str);
  }

  this->Copy(0,position,tmp);
  tmp.Add(str);
  XSTRING trail;
  this->Copy(position,trail);
  tmp.Add(trail);

  this->Set(tmp);

  return true;
}

/*-------------------------------------------------------------------
//  XSTRING::ConvertFromInt
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      03/04/2012 13:43:23
//
//  @return       bool :
//  @param        value :
//  @param        mask :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::ConvertFromInt(int value, const XCHAR* mask)
{
  char*     str;
  XSTRING  _mask;

  str = new char[XSTRING_MAXTEMPOSTR];
  if(!str) return false;

  memset(str,0,XSTRING_MAXTEMPOSTR);

  if(mask) _mask = mask; else _mask = __L("%d");

  XSTRING_CREATEOEM(_mask, charstr)
  SPRINTF(str, charstr, value);
  XSTRING_DELETEOEM(charstr)

  Set(str);

  delete [] str;

  return true;
}




/*-------------------------------------------------------------------
//  XSTRING::ConvertFromDWord
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      03/04/2012 13:43:23
//
//  @return       bool :
//  @param        value :
//  @param        mask :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::ConvertFromDWord(XDWORD value,const XCHAR* mask)
{
  char*    str;
  XSTRING  _mask;

  str = new char[XSTRING_MAXTEMPOSTR];
  if(!str) return false;

  memset(str,0,XSTRING_MAXTEMPOSTR);

  if(mask) _mask = mask; else _mask = __L("%d");

  XSTRING_CREATEOEM(_mask, charstr)
  SPRINTF(str, charstr, (XDWORD)value);
  XSTRING_DELETEOEM(charstr)

  Set(str);

  delete [] str;

  return true;
}


/*-------------------------------------------------------------------
//  XSTRING::ConvertFromDWord
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      03/04/2012 13:43:23
//
//  @return       bool :
//  @param        value :
//  @param        mask :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::ConvertFromQWord(XQWORD value,const XCHAR* mask)
{
  char*    str;
  XSTRING  _mask;

  str = new char[XSTRING_MAXTEMPOSTR];
  if(!str) return false;

  memset(str,0,XSTRING_MAXTEMPOSTR);

  if(mask) _mask = mask; else _mask = __L("%u");

  XSTRING_CREATEOEM(_mask, charstr)
  SPRINTF(str, charstr, (XDWORD)value);
  XSTRING_DELETEOEM(charstr)

  Set(str);

  delete [] str;

  return true;
}


/*-------------------------------------------------------------------
//  XSTRING::ConvertFromLongLong
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      04/06/2015 12:18:37
//
//  @return       bool :
//
//  @param        value :
//  @param        NULL :
*/
/*-----------------------------------------------------------------*/
bool   XSTRING::ConvertFromLongLong (long long value, const XCHAR* mask)
{
  char*    str;
  XSTRING  _mask;

  str = new char[XSTRING_MAXTEMPOSTR];
  if(!str) return false;

  memset(str,0,XSTRING_MAXTEMPOSTR);

  if(mask) _mask = mask; else _mask = __L("%lld");

  XSTRING_CREATEOEM(_mask, charstr)
  SPRINTF(str, charstr, (long long)value);
  XSTRING_DELETEOEM(charstr)

  Set(str);

  delete [] str;

  return true;
}

/*-------------------------------------------------------------------
//  XSTRING::ConvertFromFloat
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      03/04/2012 13:43:30
//
//  @return       bool :
//  @param        value :
//  @param        mask :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::ConvertFromFloat(float value,const XCHAR* mask)
{
  char*    str;
  XSTRING  _mask;

  str = new char[XSTRING_MAXTEMPOSTR];
  if(!str) return false;

  memset(str,0,XSTRING_MAXTEMPOSTR);

  if(mask) _mask = mask; else _mask = __L("%f");

  XSTRING_CREATEOEM(_mask, charstr);
  SPRINTF(str, charstr, value);
  XSTRING_DELETEOEM(charstr)

  Set(str);

  delete [] str;

  return true;
}



/*-------------------------------------------------------------------
//  XSTRING::ConvertFromDouble
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/12/2012 11:10:01
//
//  @return       bool :
//  @param        value :
//  @param        mask :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::ConvertFromDouble(double value,const XCHAR* mask)
{
  char*    str;
  XSTRING  _mask;

  str = new char[XSTRING_MAXTEMPOSTR];
  if(!str) return false;

  memset(str,0,XSTRING_MAXTEMPOSTR);

  if(mask) _mask = mask; else _mask = __L("%g");

  XSTRING_CREATEOEM(_mask, charstr)
  SPRINTF(str, charstr, value);
  XSTRING_DELETEOEM(charstr)

  Set(str);

  delete [] str;

  return true;
}



/*-------------------------------------------------------------------
//  XSTRING::ConvertFromBoolean
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      04/06/2015 11:10:16
//
//  @return       bool :
//
//  @param        boolean :
//  @param        uppercase :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::ConvertFromBoolean(bool boolean, bool uppercase)
{
  if(boolean)
        Set(uppercase? __L("YES") : __L("yes"));
   else Set(uppercase? __L("NO")  : __L("no"));

  return true;
}





/*-------------------------------------------------------------------
//  XSTRING::ConvertFromXBUFFER
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      23/11/2015 12:45:15
//
//  @return       bool :
//
//  @param        buffer :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::ConvertFromXBUFFER(XBUFFER& buffer)
{
  if(!this->AdjustSize(buffer.GetSize()/sizeof(XDWORD))) return false;
  if(!buffer.Get(*this, this->size, 0)) return false;

  return true;
}



/*-------------------------------------------------------------------
//  XSTRING::ConvertHexFormatChars
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      04/04/2013 7:13:36
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XSTRING::ConvertHexFormatChars()
{
  XSTRING string;

  string.Empty();

  if(IsEmpty()) return false;

  for(int c=0;c<(int)GetSize();c++)
    {
      XCHAR character[3] = { 0 , 0 , 0 };

      character[0] = Get()[c];
      character[1] = Get()[c+1];

      if((character[0]==__C('\\')) && (character[1]==__C('x')))
        {
          XSTRING   numberhex;
          XCHAR part[2] = { 0 , 0 };

          c+=2;

          for(int d=0;d<2;d++)
            {
              part[0] = Get()[c+d];
              numberhex.Add(part);
            }

          numberhex.UnFormat(__L("%02X"),&part[0]);

          part[0]&=0x00FF;
          string.Add(part);

          c++;
        }
       else string.Add((XCHAR)character[0]);
    }

  if(string.IsEmpty()) return false;

  Set(string);

  return true;
}




/*-------------------------------------------------------------------
//  XSTRING::ConvertToHexString
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      12/03/2014 10:25:30
//
//  @return       bool :
//  @param        string :
//  @param        uppercase :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::ConvertToHexString(XSTRING& string, bool uppercase)
{
  XSTRING datastr;

  string.Empty();

  for(int c=0;c<(int)GetSize(); c++)
    {
      datastr.Format(uppercase?__L("%02X"):__L("%02x"), Get()[c]);
      string.Add(datastr);
    }

  return true;
}



/*-------------------------------------------------------------------
//  XSTRING::ConvertHexStringToBuffer
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      20/03/2014 10:35:16
//
//  @return       bool :
//
//  @param        xbuffer :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::ConvertHexStringToBuffer(XBUFFER& xbuffer)
{
  xbuffer.Delete();

  for(int c=0; c<(int)GetSize(); c+=2)
    {
      XCHAR    hexbytechar[3] = { 0, 0, 0 };
      XSTRING  hexbytestr;
      int      data;

      hexbytechar[0] = Get()[c];
      hexbytechar[1] = Get()[c+1];

      hexbytestr = hexbytechar;

      hexbytestr.UnFormat(__L("%02X"), &data);
      xbuffer.Add((XBYTE)data);
    }

  return true;
}



/*-------------------------------------------------------------------
//  XSTRING::ConvertHexStringFromBuffer
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      20/03/2014 12:07:06
//
//  @return       bool :
//
//  @param        xbuffer :
//  @param        uppercase :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::ConvertHexStringFromBuffer(XBUFFER& xbuffer, bool uppercase)
{
  Empty();

  for(int c=0; c<(int)xbuffer.GetSize(); c++)
    {
      XSTRING  hexbytestr;

      hexbytestr.Format(uppercase?__L("%02X"):__L("%02x"), xbuffer.Get()[c]);
      Add(hexbytestr);
    }

  return true;

}



/*-------------------------------------------------------------------
//  XSTRING::ConvertToInt
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      12/09/2014 12:43:53
//
//  @return       int :
//
//  @param        index :
//  @param        mask :
//  @param        checkvalidchars :
*/
/*-----------------------------------------------------------------*/
int XSTRING::ConvertToInt(int index, const XCHAR* mask, bool checkvalidchars)
{
  if(IsEmpty()) return 0;

  if(checkvalidchars)
    {
      XSTRING  validcharacters;
      validcharacters  = XSTRING_VALIDCHARSINT;
      if(!AreValidCharacters(validcharacters.Get())) return 0;
    }

  XSTRING  _mask;
  int      data = 0;

  if(mask)
         _mask = mask;
    else _mask = __L("%d");

  XSTRING_CREATEOEM(_mask, charstr)
  XSTRING_CREATEOEM((*this), charstr2)

  SSCANF(&charstr2[index], charstr, &data);

  XSTRING_DELETEOEM(charstr2)
  XSTRING_DELETEOEM(charstr)

  return data;
}




/*-------------------------------------------------------------------
//  XSTRING::ConvertToDWord
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      12/09/2014 12:44:23
//
//  @return       XDWORD :
//
//  @param        index :
//  @param        mask :
//  @param        checkvalidchars :
*/
/*-----------------------------------------------------------------*/
XDWORD XSTRING::ConvertToDWord(int index, const XCHAR* mask, bool checkvalidchars)
{
  if(IsEmpty()) return 0;

  if(checkvalidchars)
    {
      XSTRING  validcharacters;
      validcharacters  = XSTRING_VALIDCHARSINT
      if(!AreValidCharacters(validcharacters.Get())) return 0;
    }

  XSTRING  _mask;
  XDWORD   data = 0;

  if(mask) _mask = mask; else _mask = __L("%d");

  XSTRING_CREATEOEM(_mask, charstr)
  XSTRING_CREATEOEM((*this), charstr2)

  SSCANF(&charstr2[index], charstr, &data);

  XSTRING_DELETEOEM(charstr2)
  XSTRING_DELETEOEM(charstr)

  return data;
}


/*-------------------------------------------------------------------
//  XSTRING::ConvertToQWord
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      12/09/2014 12:44:23
//
//  @return       XQWORD :
//
//  @param        index :
//  @param        mask :
//  @param        checkvalidchars :
*/
/*-----------------------------------------------------------------*/
XQWORD XSTRING::ConvertToQWord(int index, const XCHAR* mask, bool checkvalidchars)
{
  if(IsEmpty()) return 0;

  if(checkvalidchars)
    {
      XSTRING  validcharacters;
      validcharacters  = XSTRING_VALIDCHARSINT
      if(!AreValidCharacters(validcharacters.Get())) return 0;
    }

  XSTRING  _mask;
  long long  data = 0;

  if(mask) _mask = mask; else _mask = __L("%lld");

  XSTRING_CREATEOEM(_mask, charstr)
  XSTRING_CREATEOEM((*this), charstr2)

  SSCANF(&charstr2[index], charstr, &data);

  XSTRING_DELETEOEM(charstr2)
  XSTRING_DELETEOEM(charstr)

  return data;
}



/*-------------------------------------------------------------------
//  XSTRING::ConvertToFloat
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      12/09/2014 12:45:00
//
//  @return       float :
//
//  @param        index :
//  @param        mask :
//  @param        checkvalidchars :
*/
/*-----------------------------------------------------------------*/
float XSTRING::ConvertToFloat(int index, const XCHAR* mask, bool checkvalidchars)
{
  if(IsEmpty()) return 0.0f;

  if(checkvalidchars)
    {
      XSTRING  validcharacters;
      validcharacters  = XSTRING_VALIDCHARSFLOAT;
      if(!AreValidCharacters(validcharacters.Get())) return .0f;
    }

  XSTRING  _mask;
  float  data = 0;

  if(mask) _mask = mask; else _mask = __L("%f");

  XSTRING_CREATEOEM(_mask, charstr)
  XSTRING_CREATEOEM((*this), charstr2)

  SSCANF(&charstr2[index], charstr, &data);

  XSTRING_DELETEOEM(charstr2)
  XSTRING_DELETEOEM(charstr)

  return data;
}




/*-------------------------------------------------------------------
//  XSTRING::ConvertToDouble
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      12/09/2014 12:45:25
//
//  @return       double :
//
//  @param        index :
//  @param        mask :
//  @param        checkvalidchars :
*/
/*-----------------------------------------------------------------*/
double XSTRING::ConvertToDouble(int index,const XCHAR* mask, bool checkvalidchars)
{
  if(IsEmpty()) return 0;

  if(checkvalidchars)
    {
      XSTRING  validcharacters;
      validcharacters  = XSTRING_VALIDCHARSFLOAT;
      if(!AreValidCharacters(validcharacters.Get())) return .0f;
    }

  XSTRING  _mask;
  double     data = 0;

  if(mask) _mask = mask; else _mask = __L("%lf");

  XSTRING_CREATEOEM(_mask, charstr)
  XSTRING_CREATEOEM((*this), charstr2)

  SSCANF(&charstr2[index], charstr, &data);

  XSTRING_DELETEOEM(charstr2)
  XSTRING_DELETEOEM(charstr)

  return data;
}





/*-------------------------------------------------------------------
//  XSTRING::ConvertToXBUFFER
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      23/11/2015 12:46:42
//
//  @return       bool :
//
//  @param        buffer :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::ConvertToXBUFFER(XBUFFER& buffer)
{
  if(IsEmpty()) return true;

  if(!buffer.Resize(GetSize()*sizeof(XDWORD))) return false;
  if(!buffer.Set(*this)) return false;

  return true;
}



/*-------------------------------------------------------------------
//  XSTRING::ConvertToBoolean
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      04/06/2015 11:33:58
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool XSTRING::ConvertToBoolean()
{
  bool result = false;

  if(!Compare(__L("yes") , true) || !Compare(__L("true") , true))
    {
      result = true;
    }
   else
    {
      if(!Compare(__L("no") , true) || !Compare(__L("false") , true))
        {
          result = false;
        }
       else
        {
          if(Compare(__L("0")  , true)) result = true;
        }
    }

  return result;
}




/*-------------------------------------------------------------------
//  XSTRING::ConvertFromUTF8
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      02/02/2011 11:31:28
//
//  @return       bool :
//  @param        data :
//  @param        maxsize :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::ConvertFromUTF8(XBYTE* data,int size)
{
  //Empty();

  int     sizeutf8  = 0;
  int     index     = 0;
  XBYTE*  ptr;
  int     c;

  c   = 0;
  ptr = data;
  while(c<size)
    {
      if(((XBYTE)*ptr)<128 || (*ptr&192)==192) sizeutf8++;
      ptr++;
      c++;
    }

  if(!sizeutf8) return false;

  if(sizeutf8>size)  sizeutf8 = size;

  //Set(sizeutf8);
  if(!ReAllocBuffer(sizeutf8)) return false;

  c   = 0;
  ptr = data;
  while(c<size)
    {
      XBYTE b=*ptr;
      //Byte represents an ASCII character. Direct copy will do.
      if(!(b&128))
        {
          text[index]=b;
        }
       else
        {
           //Byte is the middle of an encoded character. Ignore.
           if((b&192)==128)
             {
               ptr++;
               continue;
             }
            else
             {
               //Byte represents the start of an encoded character in the range
               //U+0080 to U+07FF
               if((b&224)==192)
                 {
                   text[index]=((*ptr&31)<<6)|ptr[1]&63;
                 }
                else
                 {
                    //Byte represents the start of an encoded character in the range
                    //U+07FF to U+FFFF
                    if((b&240)==224)
                      {
                        text[index]=((*ptr&15)<<12)|((ptr[1]&63)<<6)|ptr[2]&63;
                      }
                     else
                      {
                        //Byte represents the start of an encoded character beyond the
                        //U+FFFF limit of 16-bit integers
                        if((b&248)==240)
                          {
                            text[index]='?';
                          }
                      }
                 }
             }
        }

      index++;
      ptr++;
      c++;

      if(index>=sizeutf8) break;
    }

  return true;
}



/*-------------------------------------------------------------------
//  XSTRING::ConvertFromUTF8
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      30/05/2014 10:51:05
//
//  @return       bool :
//
//  @param        xbuffer :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::ConvertFromUTF8(XBUFFER& xbuffer)
{
  return ConvertFromUTF8(xbuffer.Get(), xbuffer.GetSize());
}



/*-------------------------------------------------------------------
//  XSTRING::GetSizeConvertToUTF8
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      30/05/2014 10:53:55
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
int XSTRING::GetSizeConvertToUTF8()
{
  int size = 0;

  if(IsEmpty()) return size;

  int i = 0;
  int j = 0;
  int c = (int)text[i++];

  while(c)
    {
      if(c<0)
        {
          j++;
        }
       else
        {
          if(c<128)
            {
              j++;
            }
           else
            {
                if(c< 2048)
                  {
                    j++;
                    j++;
                  }
                 else
                  {
                    if(c<65536)
                      {
                        j++;
                        j++;
                        j++;
                      }
                     else
                      {
                        j++;
                        j++;
                        j++;
                        j++;
                      }
                  }
            }
        }

      c = text[i++];
    }


  size = j;

  return size;
}




/*-------------------------------------------------------------------
//  XSTRING::ConvertToUTF8
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      04/02/2011 17:52:37
//
//  @return       bool :
//  @param        data :
//  @param        size :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::ConvertToUTF8(XBYTE* data, int& size)
{
  size = 0;
  if(!data) return false;

  if(IsEmpty()) return false;

  int i = 0;
  int j = 0;
  int c = (int)text[i++];

  while(c)
    {
      if(c<0)
        {
          data[j++]=1;
        }
       else
        {
          if(c<128)
            {
              data[j++] = (XBYTE)c;
            }
           else
            {
                if(c< 2048)
                  {
                    data[j++]= (XBYTE)(192+ c/64);
                    data[j++]= (XBYTE)(128+(c&63));
                  }
                 else
                  {
                    if(c<65536)
                      {
                        data[j++]=  (XBYTE)(224+ c/4096);
                        data[j++]=  (XBYTE)(128+(c&4095)/64);
                        data[j++]=  (XBYTE)(128+(c&63));
                      }
                     else
                      {
                        data[j++]=  (XBYTE)(240+ c/262144);
                        data[j++]=  (XBYTE)(128+(c&262143)/4096);
                        data[j++]=  (XBYTE)(128+(c&4095)/64);
                        data[j++]=  (XBYTE)(128+(c&63));
                      }


                  }
            }
        }

      c = text[i++];
    }

  data[j]=0;

  size = j;

  return true;
}


/*-------------------------------------------------------------------
//  XSTRING::ConvertToUTF8
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      30/05/2014 10:55:10
//
//  @return       bool :
//
//  @param        xbuffer :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::ConvertToUTF8(XBUFFER& xbuffer)
{
  if(IsEmpty()) return false;

  int sizeUTF8 = GetSizeConvertToUTF8()+1;

  xbuffer.Delete();
  xbuffer.Resize(sizeUTF8);
  xbuffer.FillBuffer();

  int _size  = xbuffer.GetSize();

  return ConvertToUTF8(xbuffer.Get(),  _size);

}



/*-------------------------------------------------------------------
//  XSTRING::ConvertToBase64
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      11/03/2014 16:19:52
//
//  @return       bool :
//  @param        string :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::ConvertToBase64(XSTRING& string)
{
  int sizebase64  = 4 * ((GetSize() + 2) / 3);
  int mod_table[] = { 0, 2, 1 };

  string.Empty();
  string.ReAllocBuffer(sizebase64);

  for(int i=0, j=0; i<(int)GetSize();)
    {
      XDWORD octetA = (i < (int)GetSize()) ? (XBYTE)Get()[i++] : 0;
      XDWORD octetB = (i < (int)GetSize()) ? (XBYTE)Get()[i++] : 0;
      XDWORD octetC = (i < (int)GetSize()) ? (XBYTE)Get()[i++] : 0;

      XDWORD triple = (octetA << 0x10) + (octetB << 0x08) + octetC;

      string.Get()[j++] = (ConvertIndexBase64ToXCHAR((triple >> 3 * 6) & 0x3F));
      string.Get()[j++] = (ConvertIndexBase64ToXCHAR((triple >> 2 * 6) & 0x3F));
      string.Get()[j++] = (ConvertIndexBase64ToXCHAR((triple >> 1 * 6) & 0x3F));
      string.Get()[j++] = (ConvertIndexBase64ToXCHAR((triple >> 0 * 6) & 0x3F));
    }

  for(int i = 0; i < mod_table[GetSize() % 3]; i++)
    {
      string.Get()[sizebase64 - 1 - i] = __C('=');
    }

  return true;
}




/*-------------------------------------------------------------------
//  XSTRING::::ConvertFromBase64
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      11/03/2014 15:59:47
//
//  @return       bool :
//  @param        string :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::ConvertFromBase64(XSTRING& string)
{
  XDWORD  data;
  int     size  = string.GetSize();
  int     c     = 0;
  int     d;

  Empty();

  if(!size) return false;

  XSTRING_CREATEOEM(string, charstr)

  do{ data = 0;

      for(d=0;d<4;d++)
        {
          data |= ConvertXCHARToBase64Index((char)charstr[c+d]);
          if(d!=3) data<<=6;
        }


      data<<=8;

      for(d=0;d<3;d++)
        {
          XCHAR character[2] = { 0 , 0 };

          character[0]  = ((data & 0xFF000000)>>24);
          data<<=8;

          Add(character);
        }

      c+=4;

    } while(c<size);

  XSTRING_DELETEOEM(charstr)

  return true;
}




/*-------------------------------------------------------------------
//  XSTRING::ConvertToPascal
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      11/11/2013 10:50:13
//
//  @return       bool :
//  @param        pascalstring :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::ConvertToPascal(SHORTSTRING& pascalstring)
{
  memset(&pascalstring, 0, sizeof(SHORTSTRING));

  pascalstring.size = (XBYTE)(GetSize()>255)?255:GetSize();

  XSTRING_CREATEOEM((*this), charstr)
  memcpy(&pascalstring.data, charstr, pascalstring.size);
  XSTRING_DELETEOEM(charstr)

  return true;
}




/*-------------------------------------------------------------------
//  XSTRING::ConvertFromPascal
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      20/12/2013 9:39:40
//
//  @return       bool :
//  @param        pascalstring :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::ConvertFromPascal(SHORTSTRING& pascalstring)
{
  if(!pascalstring.size) return false;

  // no effect
  //pascalstring.data[pascalstring.size];

  Set(pascalstring.data);

  return true;
}





/*-------------------------------------------------------------------
//  XSTRING::ConvertFromDoubleToSpanishText
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      30/06/2015 9:52:18
//
//  @return       bool :
//
//  @param        value :
//  @param        withintegerpart :
//  @param        withdecimalpart :
//  @param        integerpart :
//  @param        decimalpart :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::ConvertFromDoubleToSpanishText(double value, bool withintegerpart, bool withdecimalpart, double& integerpart, double& decimalpart)
{
  Empty();

  integerpart = Truncate(value);
  decimalpart = Fraction(value);

  XSTRING tmp;

  if(withintegerpart)
    {
      double _integerpart;
      double _decimalpart;

      if(integerpart == 0) Set(__L("cero"));
        else if(integerpart ==  1) Set(__L("uno"));
        else if(integerpart ==  2) Set(__L("dos"));
        else if(integerpart ==  3) Set(__L("tres"));
        else if(integerpart ==  4) Set(__L("cuatro"));
        else if(integerpart ==  5) Set(__L("cinco"));
        else if(integerpart ==  6) Set(__L("seis"));
        else if(integerpart ==  7) Set(__L("siete"));
        else if(integerpart ==  8) Set(__L("ocho"));
        else if(integerpart ==  9) Set(__L("nueve"));
        else if(integerpart == 10) Set(__L("diez"));
        else if(integerpart == 11) Set(__L("once"));
        else if(integerpart == 12) Set(__L("doce"));
        else if(integerpart == 13) Set(__L("trece"));
        else if(integerpart == 14) Set(__L("catorce"));
        else if(integerpart == 15) Set(__L("quince"));
        else if(integerpart < 20)
               {
                  Set(__L("dieci"));
                  tmp.ConvertFromDoubleToSpanishText(integerpart - 10, true, false, _integerpart, _decimalpart);
                  Add(tmp);
                }
               else if(integerpart == 20) Set(__L("veinte"));
                      else if(integerpart < 30)
                            {
                              Set(__L("veinti"));
                              tmp.ConvertFromDoubleToSpanishText(integerpart - 20, true, false, _integerpart, _decimalpart);
                              Add(tmp);
                             }
                            else if(integerpart == 30) Set(__L("treinta"));
                            else if(integerpart == 40) Set(__L("cuarenta"));
                            else if(integerpart == 50) Set(__L("cincuenta"));
                            else if(integerpart == 60) Set(__L("sesenta"));
                            else if(integerpart == 70) Set(__L("setenta"));
                            else if(integerpart == 80) Set(__L("ochenta"));
                            else if(integerpart == 90) Set(__L("noventa"));
                            else if(integerpart < 100)
                                   {
                                     ConvertFromDoubleToSpanishText(Truncate(integerpart/10) * 10, true, false, _integerpart, _decimalpart);
                                     Add(__L(" y "));
                                     tmp.ConvertFromDoubleToSpanishText(fmod(integerpart, (double)10), true, false, _integerpart, _decimalpart);
                                     Add(tmp);
                                   }
                                  else if(integerpart == 100) Set(__L("cien"));
                                  else if(integerpart < 200)
                                         {
                                           Set(__L("ciento "));
                                           tmp.ConvertFromDoubleToSpanishText(integerpart - 100, true, false, _integerpart, _decimalpart);
                                           Add(tmp);
                                         }
                                        else if((integerpart == 200) || (integerpart == 300) || (integerpart == 400) || (integerpart == 600) || (integerpart == 800))
                                               {
                                                 ConvertFromDoubleToSpanishText(Truncate(integerpart / 100), true, false, _integerpart, _decimalpart);
                                                 Add(__L("cientos"));
                                               }
                                              else if(integerpart == 500) Set(__L("quinientos"));
                                              else if(integerpart == 700) Set(__L("setecientos"));
                                              else if(integerpart == 900) Set(__L("novecientos"));
                                              else if(integerpart < 1000)
                                                   {
                                                     ConvertFromDoubleToSpanishText(Truncate(integerpart / 100) * 100, true, false, _integerpart, _decimalpart);
                                                     Add(__L(" "));
                                                     tmp.ConvertFromDoubleToSpanishText(fmod(integerpart, (double)100), true, false, _integerpart, _decimalpart);
                                                     Add(tmp);
                                                   }
                                                  else if(integerpart == 1000) Set(__L("mil"));
                                                  else if(integerpart < 2000)
                                                        {
                                                          Set(__L("mil "));
                                                          tmp.ConvertFromDoubleToSpanishText(fmod(integerpart, (double)1000), true, false, _integerpart, _decimalpart);
                                                          Add(tmp);
                                                        }
                                                      else if(integerpart < 1000000)
                                                              {
                                                                ConvertFromDoubleToSpanishText(Truncate(integerpart / 1000), true, false, _integerpart, _decimalpart);
                                                                Add(__L(" mil"));
                                                                if((fmod(integerpart, (double)1000)) > 0)
                                                                  {
                                                                    Add(" ");
                                                                    tmp.ConvertFromDoubleToSpanishText(fmod(integerpart, (double)1000), true, false, _integerpart, _decimalpart);
                                                                    Add(tmp);
                                                                  }
                                                              }
                                                            else if(integerpart == 1000000) Set(__L("un millon"));
                                                            else if(integerpart < 2000000)
                                                                    {
                                                                      Set(__L("un millon"));
                                                                      tmp.ConvertFromDoubleToSpanishText(fmod(integerpart, (double)1000000), true, false, _integerpart, _decimalpart);
                                                                      Add(tmp);
                                                                    }
                                                                   else if(integerpart < 1000000000000LL)
                                                                          {
                                                                             ConvertFromDoubleToSpanishText((integerpart / 1000000), true, false, _integerpart, _decimalpart);
                                                                             Add(__L(" millones"));
                                                                             if((integerpart - Truncate(integerpart / 1000000) * 1000000) > 0)
                                                                               {
                                                                                 Add(__L(" "));
                                                                                 tmp.ConvertFromDoubleToSpanishText(integerpart - Truncate(integerpart / 1000000) * 1000000, true, false, _integerpart, _decimalpart);
                                                                                 Add(tmp);
                                                                               }
                                                                          }
                                                                         else
                                                                          {
                                                                            if(integerpart == 1000000000000LL) Set(__L("un billon"));
                                                                              else if(integerpart < 2000000000000LL)
                                                                                {
                                                                                  Set(__L("un billon"));
                                                                                  tmp.ConvertFromDoubleToSpanishText(Truncate(integerpart / 1000000000000LL) * 1000000000000LL, true, false, _integerpart, _decimalpart);
                                                                                  Add(tmp);
                                                                                }
                                                                               else
                                                                                {
                                                                                   ConvertFromDoubleToSpanishText(Truncate(integerpart / 1000000000000LL), true, false, _integerpart, _decimalpart);
                                                                                   Add(__L(" billones"));

                                                                                   if((integerpart - Truncate(integerpart / 1000000000000LL) * 1000000000000LL) > 0)
                                                                                     {
                                                                                       Add(__L(" "));
                                                                                       tmp.ConvertFromDoubleToSpanishText(integerpart - Truncate(integerpart / 1000000000000LL) * 1000000000000LL, true, false, _integerpart, _decimalpart);
                                                                                       Add(tmp);
                                                                                     }
                                                                                }
                                                                          }
      if(!integerpart) return false;
    }

  if(withdecimalpart)
    {
      double _integerpart;
      double _decimalpart;

      double ipart = Truncate(decimalpart*100);
      double dpart = Fraction(decimalpart*100);

      if(dpart>=0.9f) ipart++;

      if(ipart)
        {
          if(withintegerpart) Add(__L(" con "));
          tmp.ConvertFromDoubleToSpanishText(ipart, true, false, _integerpart, _decimalpart);
          Add(tmp);

        } else return false;
    }

  return true;
}





/*-------------------------------------------------------------------
//  XSTRING::Swab
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      31/03/2011 13:00:31
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XSTRING::Swab()
{
  if(IsEmpty()) return false;

  for(int c=0;c<(int)GetSize();c+=2)
    {
      XCHAR data;

      data      = text[c+1];
      text[c+1] = text[c];
      text[c]   = data;
    }

  return true;
}




/*-------------------------------------------------------------------
//  XSTRING::FormatArg
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      14/02/2013 21:01:40
//
//  @return       bool :
//  @param        mask :
//  @param        arg :
//  @param        isspecialweb :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::FormatArg(const XCHAR* mask, va_list* arg, bool isspecialweb)
{
  XSTRING str;
  bool    openlabel = false;

  Empty();

  if(!mask) return false;

  int c  = 0;

  XCHAR* param = new XCHAR[16];
  if(param)
    {
      while(mask[c])
        {
          switch(mask[c])
            {
              case __C('%')   : {
                                  int  nparam = 1;
                                  bool end    = false;

                                  memset(param,0, 16*sizeof(XCHAR));
                                  param[0] = __C('%');

                                  c++;

                                  do{ param[nparam] = mask[c];

                                      nparam++;

                                      switch(mask[c])
                                        {
                                          case __C('c')   :
                                          case __C('C')   : { XCHAR character;

                                                              #if defined(LINUX) || defined(ANDROID) || defined(MICROCONTROLLER)
                                                              character = (XCHAR)va_arg((*arg), XDWORD);
                                                              #else
                                                              character = va_arg((*arg), XCHAR);
                                                              #endif

                                                              str.Set(character);
                                                              end  = true ;
                                                            }
                                                            break;

                                          case __C('d')   :
                                          case __C('i')   :
                                          case __C('o')   :
                                          case __C('u')   :
                                          case __C('x')   :
                                          case __C('X')   : { bool large = false;
                                                              for(int l=0; l<(int)XSTRING::GetSize(param); l++)
                                                                {
                                                                  if(param[l] == __C('l')) large = true;
                                                                }

                                                              if(!large)
                                                                {
                                                                  int value = (int)va_arg((*arg), int);

                                                                  str.ConvertFromInt(value, param);
                                                                }
                                                               else
                                                                {
                                                                  str.ConvertFromDouble((double)va_arg((*arg), double), param);
                                                                }
                                                              end  = true;
                                                            }
                                                            break;

                                          case __C('f')   : str.ConvertFromFloat((float)va_arg((*arg), double), param);
                                                            end  = true;
                                                            break;

                                          case __C('g')   :
                                          case __C('G')   : str.ConvertFromDouble((double)va_arg((*arg), double), param);
                                                            end  = true;
                                                            break;

                                          case __C('e')   :
                                          case __C('E')   :

                                          case __C('n')   :
                                          case __C('p')   : end = true;
                                                            break;

                                          case __C('s')   :
                                          case __C('S')   : { str = (XCHAR*)va_arg((*arg),XCHAR*);

                                                              XSTRING sizestr;

                                                              sizestr = param;
                                                              if(sizestr.GetSize()>2)
                                                                {
                                                                  sizestr.ToUpperCase();
                                                                  sizestr.DeleteCharacter(__C('%'));
                                                                  sizestr.DeleteCharacter(__C('S'));

                                                                  ///------------------ TODO : implementar la escificación de sprintf / dejar de usarlo para strings...

                                                                  if(!sizestr.IsEmpty())
                                                                    {
                                                                      int sizeadjust = sizestr.ConvertToInt();
                                                                      if(abs(sizeadjust)>(int)str.GetSize())
                                                                        {
                                                                          str.AdjustSize(abs(sizeadjust),(sizeadjust>0)?true:false, isspecialweb?__L("&nbsp"):__L(" "), isspecialweb);
                                                                        }
                                                                    }
                                                                }

                                                              end = true;
                                                            }
                                                            break;

                                          case __C('%')   : str = __L("%");
                                                            end = true;
                                                            break;

                                          case __C('\x0') : end = true;
                                                            break;

                                              default     : break;
                                        }

                                      c++;

                                    } while(!end);
                                }
                                break;

              case __C('<')   : { openlabel = true;
                                  XCHAR character[2]={ mask[c], 0 };
                                  str.Set(character);
                                  c++;
                                }
                                break;

              case __C('>')   : { openlabel = false;
                                  XCHAR character[2]={ mask[c], 0 };
                                  str.Set(character);
                                  c++;
                                }
                                break;

              case 0x0A       : if(isspecialweb && (!openlabel))
                                  {
                                    str = __L("<br />");
                                  }
                                 else
                                  {
                                    XCHAR character[2]={ mask[c], 0 };
                                    str.Set(character);
                                  }
                                c++;
                                break;

              case 0x20       : if(isspecialweb && (!openlabel))
                                  {
                                    str = __L("&nbsp");
                                  }
                                 else
                                  {
                                    XCHAR character[2]={ mask[c], 0 };
                                    str.Set(character);
                                  }
                                c++;
                                break;

                 default      : { XCHAR   character[2]={ mask[c], 0 };
                                  XDWORD  value       = (XDWORD)mask[c];

                                  if(isspecialweb && (!openlabel) && (value > 128))
                                        str.Format(__L("&#%d"), value);
                                   else str.Set(character);

                                  c++;
                                }
                                break;
            }

          Add(str);
        }

      delete [] param;
    }

  return true;
}



/*-------------------------------------------------------------------
//  XSTRING::Format
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      07/02/2013 18:31:18
//
//  @return       bool :
//  @param        mask :
//  @param        ... :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::Format(const XCHAR* mask, ...)
{
  va_list arg;

  va_start(arg, mask);

  FormatArg(mask, &arg);

  va_end(arg);

  return 0;
}


/*-------------------------------------------------------------------
//  XSTRING::AddFormat
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      07/02/2013 18:31:18
//
//  @return       bool :
//  @param        mask :
//  @param        ... :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::AddFormat(const XCHAR* mask, ...)
{
  XSTRING concat;

  va_list arg;

  va_start(arg, mask);

  concat.FormatArg(mask, &arg);

  va_end(arg);

  Add(concat.Get());

  return 0;
}




/*-------------------------------------------------------------------
//  XSTRING::UnFormat
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      31/03/2011 12:47:05
//
//  @return       bool :
//  @param        mask :
//  @param        ... :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::UnFormat(const XCHAR* mask,...)
{
  if(!size)    return false;
  if(!mask)    return false;
  if(!mask[0]) return false;
  if(!text)    return false;
  if(!text[0]) return false;

  va_list arg;
  va_start(arg, mask);

  XCHAR* param = new XCHAR[16];
  if(param)
    {
      int   nparam;
      bool  end;
      int   indextext = 0;
      int   c         = 0;
      bool  inbrackets  = false;

      while(mask[c])
        {
          switch(mask[c])
            {
              case __C('%')   : memset(param,0,16*sizeof(XCHAR));
                                param[0] = __C('%');

                                nparam = 1;
                                end    = false;

                                c++;

                                do{ param[nparam] = mask[c];

                                    nparam++;

                                    switch(mask[c])
                                      {
                                        case __C('c')   :
                                        case __C('C')   : { xcharvalue = (XCHAR*)va_arg(arg, XCHAR*);
                                                            if(xcharvalue) (*xcharvalue) = Get()[indextext];
                                                            indextext++;
                                                            end  = true;
                                                          }
                                                          break;

                                        case __C('d')   :
                                        case __C('i')   :
                                        case __C('o')   :
                                        case __C('u')   :
                                        case __C('x')   :
                                        case __C('X')   : { intvalue = (int*)va_arg(arg, int*);

                                                            if(intvalue) (*intvalue) = ConvertToInt(indextext,(const XCHAR*)param, false);
                                                            end  = true;
                                                          }
                                                          break;

                                        case __C('f')   : { doublevalue = (double*)va_arg(arg, double*);

                                                            if(doublevalue) (*doublevalue) = ConvertToFloat(indextext,(const XCHAR*)param, false);

                                                            end  = true;
                                                          }
                                                          break;

                                        case __C('g')   :
                                        case __C('G')   : { doublevalue = (double*)va_arg(arg, double*);

                                                            if(doublevalue) (*doublevalue) = ConvertToDouble(indextext,(const XCHAR*)param, false);

                                                            end  = true;
                                                          }
                                                          break;

                                        case __C('e')   :
                                        case __C('E')   :

                                        case __C('n')   :
                                        case __C('p')   : end = true;
                                                          break;


                                        case __C('s')   :
                                        case __C('S')   : { xcharvalue = (XCHAR*)va_arg(arg,XCHAR*);
                                                            if(!xcharvalue) break;

                                                            XSTRING string;

                                                            int d = 0;
                                                            while(text[d+indextext]!=mask[c+1] /*&& text[d+indextext]!=__C(' ')*/)
                                                              {
                                                                if(!text[indextext+d]) break;
                                                                d++;
                                                              }

                                                            Copy(indextext, indextext+d, string);

                                                            if(nparam>2)
                                                              {
                                                                XSTRING strsizeparam = param;
                                                                int     sizeparam;

                                                                strsizeparam.DeleteCharacters(0,1);
                                                                strsizeparam.DeleteCharacters(strsizeparam.GetSize()-1,1);

                                                                sizeparam = abs((int)strsizeparam.ConvertToInt());
                                                                if(sizeparam)
                                                                  {
                                                                    if(string.GetSize()>(XDWORD)sizeparam)  string.DeleteCharacters(sizeparam, (string.GetSize()-sizeparam));
                                                                  }
                                                              }
                                                            //DIEGO: ya que aquí tenemos el XSTRING y tenemos el size... por que no comprobar si el
                                                            //string tiene tamaño para recibir el dato y hacer un realloc?
                                                            memcpy((XBYTE*)xcharvalue, (XBYTE*)string.Get(), (string.GetSize()*sizeof(XCHAR)));
                                                            xcharvalue[string.GetSize()] = 0;
                                                            /*
                                                            if(mask[c+1]!=__C('%'))
                                                              {
                                                                if(text[indextext+string.GetSize()]==__C(' ')) indextext++;
                                                              }
                                                            */
                                                            indextext += string.GetSize();

                                                            end = true;
                                                          }
                                                          break;

                                        case __C('%')   : end = true;
                                                          break;

                                        case __C('[')   : inbrackets  = true;
                                                          break;

                                        case __C(']')   : if(inbrackets)
                                                            {
                                                              xcharvalue = (XCHAR*)va_arg(arg, XCHAR*);

                                                              ConvertStringWithMask(param, &text[indextext], xcharvalue);
                                                              indextext += GetSize(xcharvalue);

                                                              if((mask[c+1] == __C('s')) || (mask[c+1] == __C('S'))) c++;
                                                              end = true;
                                                            }
                                                          break;

                                        case __C('\x0') : end = true;
                                                          break;

                                            default     : break;
                                      }

                                    c++;

                                    if(end)
                                      {
                                        if(mask[c]!=__C('%'))
                                          {
                                            while(text[indextext] != mask[c])
                                              {
                                                if(!text[indextext]) break;
                                                indextext++;
                                              }
                                          }
                                         else
                                          {
                                            //if(text[indextext] ==__C(' ')) indextext++;
                                          }
                                      }

                                  } while(!end);

                              break;

                    default : c++;
                              indextext++;
                              break;
            }
        }

     delete [] param;
   }

  va_end(arg);

  return true;
}





/*-------------------------------------------------------------------
//   XSTRING::IsSpace
*/
/**
//
//
//  @author   Abraham J. Velez
//  @version  21/03/2018 9:16:26
//
//  @return   bool :
//
//  @param    XCHAR :
//
*//*-----------------------------------------------------------------*/
bool XSTRING::IsSpace(XCHAR xchar)
{
  if(xchar == 0x20) return true;  //  space
  if(xchar == 0x09) return true;  //  horizontal tab(TAB)
  if(xchar == 0x0a) return true;  //  newline(LF)
  if(xchar == 0x0b) return true;  //  vertical tab(VT)
  if(xchar == 0x0c) return true;  //  feed(FF)
  if(xchar == 0x0d) return true;  //  carriage return (CR)

  return false;
}



/*-------------------------------------------------------------------
//   XSTRING::IsDigit
*/
/**
//
//
//  @author   Abraham J. Velez
//  @version  21/03/2018 9:17:15
//
//  @return   bool :
//
//  @param    XCHAR :
//
*//*-----------------------------------------------------------------*/
bool XSTRING::IsDigit(XCHAR xchar)
{
  return (xchar >= __C('0') && xchar <= __C('9'));
}



/*-------------------------------------------------------------------
//   XSTRING::FindCharacterFromSet
*/
/**
//
//
//  @author   Abraham J. Velez
//  @version  21/03/2018 9:16:59
//
//  @return   int :
//
//  @param    const :
//
*//*-----------------------------------------------------------------*/
int XSTRING::FindCharacterFromSet(const XCHAR* chars)
{
  XCHAR         c;
  XCHAR*        s;
  const XCHAR*  p;

  for(s = text, c = *s; c != 0; s++, c = *s)
    {
      for(p = chars; *p != 0; p++)
        {
          if(c == *p)
            {
              return s - text;
            }
        }
    }

  return s - text;
}



/*-------------------------------------------------------------------
//  XSTRING::AdjustSize
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      11/09/2013 21:34:44
//
//  @return       bool :
//  @param        size :
//  @param        ahead :
//  @param        character :
//  @param        specialweb :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::AdjustSize(XDWORD size,bool ahead, XCHAR* characters, bool addstring)
{
  if(this->size == size) return false;

  int diference;

  if(this->size > size)
    {
       diference = (this->size- size);
       DeleteCharacters(size,diference);
    }
   else
    {
      if(characters)
        {
          if(ahead)
            {
              XSTRING string;

              diference = (size-this->size);
              for(int c=0;c<diference;c++)
                {
                  if(addstring) string.Add(characters); else string.Add(characters[0]);
                }

              string+=Get();

              Set(string);

            }
           else
            {
              diference = ((size)-this->size);
              for(int c=0;c<diference;c++)
                {
                  if(addstring) Add(characters);  else Add(characters[0]);
                }
            }
        }
       else
        {
          diference = ((size)-this->size);

          ReAllocBuffer(size);
          if(ahead)
            {
              for(int c=0;c<diference;c++)
                {
                  text[c+diference] = text[c];
                  text[c] = __C('-');
                }
            }
        }
    }

  return true;
}




/*-------------------------------------------------------------------
//  XSTRING::AdjustSize
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      11/09/2013 21:35:30
//
//  @return       bool :
//  @param        specialweb :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::AdjustSize()
{
  int c;

  for(c=0;c<(int)size;c++)
    {
      if(!text[c]) break;
    }

  if(!c) return false;

  return AdjustSize(c, false, __L(" "), false);
}





/*-------------------------------------------------------------------
//  XSTRING::BinaryCompare
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      09/12/2015 15:45:06
//
//  @return       bool :
//
//  @param        string :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::BinaryCompare(XSTRING& string)
{
  if(size != string.size)
    {
      return false;
    }

  return memcmp(text, string.text, size*sizeof(XCHAR)) == 0;
}




/*-------------------------------------------------------------------
//  XSTRING::Explode
*/
/**
//  Explode the string in vector using token
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      11/01/2017 11:55:34
//
//  @return       bool : success
//
//  @param        character : XCHAR , needle to find
//  @param        receive : XVECTOR<XSTRING*>* NOT NULL , vector to hold the tokens
*/
/*-----------------------------------------------------------------*/
bool XSTRING::Explode(XCHAR token, XVECTOR<XSTRING*>* receive)
{
  int start=0;
  do
  {
    int end=this->FindCharacter(token,start);
    if (end==NOTFOUND)
        end=this->size;

    if ((end-start)>0)
    {
    XSTRING* tok=new XSTRING();
    if (!tok)
      return false;

    this->Copy(start,end,(*tok));

        if (!receive->Add(tok))
        {
        delete(tok);
        return false;
        }
    }
    start=end+1;
  }
  while (start<(int)size);

  return true;
}



/*-------------------------------------------------------------------
//  XSTRING::ReAllocBuffer
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      23/11/2010 21:48:59
//
//  @return       bool :
//  @param        size :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::ReAllocBuffer(XDWORD size)
{
  if(!size)
    {
      FreeBuffer();
    }
   else
    {
      int nblocks  = (size / XSTRING_BLOCKMEM) + 1;

      XDWORD sizemem = (nblocks * XSTRING_BLOCKMEM);

      if(!text)
        {
          text = new XCHAR[sizemem+1];
          if(!text) return false;

          memset(text, 0, (sizemem+1) * sizeof(XCHAR));

          this->sizemem = sizemem;
        }
       else
        {
          if(size >= this->sizemem)
            {
              XCHAR* ttext = new XCHAR[sizemem+1];
              if(!ttext) return false;

              memset(ttext, 0, (sizemem+1) * sizeof(XCHAR));

              XDWORD tsize = size;
              if(tsize>this->size) tsize = this->size;

              memcpy(ttext, text, tsize * sizeof(XCHAR));

              FreeBuffer();

              text = ttext;

              this->sizemem = sizemem;
            }
           else
            {
              // culd we have an off by one error here?
              // size is index zero, not reall an off by one problem here
               memset(&text[size], 0, (this->sizemem - size)* sizeof(XCHAR));
               text[size] = '\0';
            }
        }

      this->size    = size;
    }

  return true;
}



/*-------------------------------------------------------------------
//  XSTRING::FreeBuffer
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      23/11/2010 21:42:50
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XSTRING::FreeBuffer()
{
  if(!text) return false;

  delete [] text;

  text    = NULL;
  size    = 0;
  sizemem = 0;

  return true;
}



/*-------------------------------------------------------------------
//  XSTRING::ConvertIndexBase64ToXCHAR
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      11/03/2014 17:05:29
//
//  @return       XCHAR :
//  @param        index :
*/
/*-----------------------------------------------------------------*/
XCHAR XSTRING::ConvertIndexBase64ToXCHAR(int index)
{
  if(index>=(int)XSTRING_SIZETABLE64BITS) return 0;
  if(index<0)                             return 0;

  return table64bits[index];
}




/*-------------------------------------------------------------------
//  XSTRING::ConvertXCHARToBase64Index
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      11/03/2014 17:05:25
//
//  @return       XBYTE :
//  @param        character :
*/
/*-----------------------------------------------------------------*/
XBYTE XSTRING::ConvertXCHARToBase64Index(XCHAR character)
{
  for(XBYTE c=0;c<(XBYTE)XSTRING_SIZETABLE64BITS;c++)
    {
      if(table64bits[c]==character) return c;
    }

  return 0;
}



/*-------------------------------------------------------------------
//  XSTRING::ConvertStringWithMask
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      02/06/2016 11:37:35
//
//  @return       bool :
//
//  @param        mask :
//  @param        string :
//  @param        result :
*/
/*-----------------------------------------------------------------*/
bool XSTRING::ConvertStringWithMask(XCHAR* mask, XCHAR* string, XCHAR* result)
{
 #ifdef MICROCONTROLLER

  XSTRING  _mask;
  XSTRING  _string;
  XSTRING  _result;

  _mask   = mask;
  _string = string;


  char* resultchar = new char[XSTRING_MAXTEMPOSTR];
  if(!resultchar) return false;

  memset(resultchar, 0, XSTRING_MAXTEMPOSTR);

  XSTRING_CREATEOEM(_mask   , maskchar)
  XSTRING_CREATEOEM(_string , stringchar)

  SSCANF(stringchar, maskchar, resultchar, XSTRING_MAXTEMPOSTR);

  XSTRING_DELETEOEM(maskchar)
  XSTRING_DELETEOEM(stringchar)

  _result = resultchar;

  memcpy(result, _result.Get(), ((_result.GetSize()+1)*sizeof(XCHAR)));

  #else

  XSTRING  _mask;
  XSTRING  add;

  add = __L("l");

  _mask = mask;
  _mask.Insert(add,1);

  UNICODESSCANF(string, _mask.Get(), result, XSTRING_STANDARDSIZE);

  #endif

  return true;
}
