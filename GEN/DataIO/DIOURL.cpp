/*------------------------------------------------------------------------------------------
//  DIOURL.CPP
//
//  Data I/O URL Class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 13/02/2012 10:38:07
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XBuffer.h"

#include "DIOURL.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/




/*-------------------------------------------------------------------
//  DIOURL::GetHTTPServer
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      13/05/2014 10:55:46
//
//  @return       bool :
//
//  @param        server :
//  @param        login :
//  @param        password :
*/
/*-----------------------------------------------------------------*/
bool DIOURL::GetHTTPServer(XSTRING& server, XSTRING& login, XSTRING& password)
{
  server.Empty();
  login.Empty();
  password.Empty();

  if(IsEmpty()) return false;

  Copy(DIOURL_WEBURLID,__L("/"),true,0,server);
  if(server.IsEmpty())
    {
      int index = Find(__L("/"), true);
      if(index != XSTRING_NOTFOUND)
        {
          Copy(0,index,server);
        }
       else server = Get();
    }

  if(server.IsEmpty()) return false;

  int indexlp = server.Find(__L("@"), true);
  if(indexlp!=XSTRING_NOTFOUND)
    {
      XSTRING  lpstring;

      server.Copy(0, indexlp, lpstring);
      server.DeleteCharacters(0,indexlp+1);

      int indexp = lpstring.Find(__L(":"), true);
      if(indexp!=XSTRING_NOTFOUND)
        {
          lpstring.Copy(0, indexp, login);
          lpstring.Copy(indexp+1 , password);
        }
    }

  return true;
}



/*-------------------------------------------------------------------
//  DIOURL::GetHTTPResource
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/03/2011 11:42:36
//
//  @return       bool :
//  @param        resource :
*/
/*-----------------------------------------------------------------*/
bool DIOURL::GetHTTPResource(XSTRING& resource)
{
  resource.Empty();

  if(IsEmpty()) return false;

  XSTRING weburlid;

  weburlid = DIOURL_WEBURLID;

  int index = Find(weburlid.Get(), true);
  if(index == XSTRING_NOTFOUND)
        index  = 0;
   else index += weburlid.GetSize();

  index = Find(__L("/"), true,index);
  if(index != XSTRING_NOTFOUND)
    {
      Copy(index,GetSize(),resource);
    }

  if(resource.IsEmpty()) return false;

  return true;
}


/*-------------------------------------------------------------------
//  DIOURL::IsAURLResolved
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      31/03/2016 13:07:00
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOURL::IsAURLResolved()
{
  for(XDWORD c=0; c<size; c++)
    {
      if(!text[c]) break;

      if((text[c] != __C('.'))  && (!Character_IsNumber(text[c])))
        {
          return false;
        }
    }

  return true;
}



/*-------------------------------------------------------------------
//  DIOURL::ResolveURL
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      19/02/2013 0:05:03
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOURL::ResolveURL()
{
  XSTRING IPstring;

  if(!ResolveURL(IPstring)) return false;

  Set(IPstring.Get());

  return true;
}




/*-------------------------------------------------------------------
//  DIOSTREAMTCPIPCONFIG ::ResolveURL
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      12/02/2013 22:44:28
//
//  @return       bool :
//  @param        IP :
*/
/*-----------------------------------------------------------------*/
bool DIOURL::ResolveURL(XSTRING& IPstring)
{
  IPstring.Empty();

  //return ResolveURL(IPstring);
  return false;
}



/*-------------------------------------------------------------------
//  DIOURL::ResolveURL
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/02/2013 23:49:33
//
//  @return       bool :
//  @param        IP :
*/
/*-----------------------------------------------------------------*/
bool DIOURL::ResolveURL(DIOIP& IP)
{
  XSTRING stringIP;

  stringIP.Empty();

  if(!ResolveURL(stringIP)) return false;

  IP.Set(stringIP);

  return true;
}



/*-------------------------------------------------------------------
//  DIOURL::IsLocalAddress
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/02/2012 9:47:58
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOURL::IsLocalAddress()
{
  DIOIP ip;

  ResolveURL(ip);

  return ip.IsLocal();
}



/*-------------------------------------------------------------------
//  DIOURL::Slash_Add
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      15/03/2011 16:12:51
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOURL::Slash_Add()
{
  int    count[2] = { 0 , 0 };
  XCHAR* str      = Get();
  bool   status   = false;

  int c=0;
  while(str[c])
    {
      if(str[c]==__C('/'))  count[0]++;
      if(str[c]==__C('\\')) count[1]++;

      c++;
    }

  if((Character_GetLast()!=__C('\\')) && (Character_GetLast()!=__C('/')))
    {
      if(count[0]>count[1])
             Add("/");
        else Add("\\");

      Slash_Normalize();

      status = true;
    }

  return status;
}



/*-------------------------------------------------------------------
//  DIOURL::Slash_Normalize
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      15/03/2011 16:37:40
//
//  @return       bool :
*/
/*-----------------------------------------------------------------*/
bool DIOURL::Slash_Normalize()
{
  if(IsEmpty()) return false;

  XCHAR* str = Get();
  int    c   = 0;

  while(str[c])
    {
      if(str[c]==__C('\\')) str[c] = __C('/');
      c++;
    }

  return true;
}



/*-------------------------------------------------------------------
//  DIOURL::Slash_Delete
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/03/2011 12:39:37
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOURL::Slash_Delete()
{
  bool status = false;

  if((Character_GetLast()==__C('\\')) || (Character_GetLast()==__C('/')))
    {
      DeleteCharacter('\\' , XSTRINGCONTEXT_ATEND);
      DeleteCharacter('/'  , XSTRINGCONTEXT_ATEND);

      status = true;
    }

  return status;
}




/*-------------------------------------------------------------------
//  DIOURL::IsUnsafeChar
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      10/05/2013 17:12:03
//
//  @return       bool :
//  @param        character :
*/
/*-----------------------------------------------------------------*/
bool DIOURL::IsUnsafeChar(XCHAR character)
{
  //XSTRING safecharacters(__L("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_.~!*'();:@=+$,/?[]"));
  XSTRING safecharacters(__L("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_/\\."));

  for(int c=0; c<(int)safecharacters.GetSize(); c++)
    {
      if(character == safecharacters.Get()[c])  return false;
    }

  return true;
}




/*-------------------------------------------------------------------
//  DIOURL::EncodeUnsafeChar
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      10/05/2013 17:32:05
//
//  @return       bool :
//  @param        character :
//  @param        encodechar :
*/
/*-----------------------------------------------------------------*/
bool DIOURL::EncodeUnsafeChar(XCHAR character, XSTRING& encodechar)
{
  XWORD normalizechar = (XWORD)character;

  encodechar.Empty();

  if(character&0xFF00)
        encodechar.Format(__L("%%%02X%%%02X"), (normalizechar>>8), (normalizechar&0x00FF));
   else encodechar.Format(__L("%%%02X"), normalizechar);

  return true;
}



/*-------------------------------------------------------------------
//  DIOURL::EncodeUnsafeCharsFromString
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/05/2011 16:18:33
//
//  @return       bool :
//  @param        string :
*/
/*-----------------------------------------------------------------*/
bool DIOURL::EncodeUnsafeCharsFromString(XSTRING& string)
{
  Empty();

  for(int c=0; c<(int)string.GetSize(); c++)
    {
      if(!IsUnsafeChar(string.Get()[c]))
        {
          Add(string.Get()[c]);
        }
       else
        {
          XSTRING encodechar;

          encodechar.Empty();

          switch(string.Get()[c])
            {
              case __C('&')  : if(string.Get()[c+1] == __C(' '))
                                     EncodeUnsafeChar(string.Get()[c], encodechar);
                                else Add(string.Get()[c]);
                               break;

              case __C('\\') :
              case __C('%')  :
                    default  : EncodeUnsafeChar(string.Get()[c], encodechar);
                               break;
            }

          if(!encodechar.IsEmpty()) Add(encodechar);

        }
    }

  if(IsEmpty()) return false;

  return true;
}



/*-------------------------------------------------------------------
//  DIOURL::EncodeUnsafeCharsFromString
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/05/2011 16:18:53
//
//  @return       bool :
//  @param        string :
*/
/*-----------------------------------------------------------------*/
bool DIOURL::EncodeUnsafeCharsFromString(const char* string)
{
  XSTRING string2(string);

  return EncodeUnsafeCharsFromString(string2);
}




/*-------------------------------------------------------------------
//  DIOURL::EncodeUnsafeCharsFromString
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/05/2011 16:19:15
//
//  @return       bool :
//  @param        string :
*/
/*-----------------------------------------------------------------*/
bool DIOURL::EncodeUnsafeCharsFromString(const XCHAR* string)
{
  XSTRING string2(string);

  return EncodeUnsafeCharsFromString(string2);
}




/*-------------------------------------------------------------------
//  DIOURL::EncodeUnsafeCharsFromBuffer
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      10/05/2013 18:18:33
//
//  @return       bool :
//  @param        xbuffer :
*/
/*-----------------------------------------------------------------*/
bool DIOURL::EncodeUnsafeCharsFromBuffer(XBUFFER& xbuffer)
{
  Empty();

  for(int c=0; c<(int)xbuffer.GetSize(); c++)
    {
      if(!IsUnsafeChar(xbuffer.Get()[c]))
        {
          Add(xbuffer.Get()[c]);
        }
       else
        {
          XSTRING encodechar;

          encodechar.Empty();

          switch(xbuffer.Get()[c])
            {
              case __C('&')  : if(xbuffer.GetByte(c+1) == __C(' '))
                                     EncodeUnsafeChar(xbuffer.GetByte(c), encodechar);
                                else Add(xbuffer.GetByte(c));
                               break;

              case __C('\\') :
              case __C('%')  :
                    default  : EncodeUnsafeChar(xbuffer.GetByte(c), encodechar);
                               break;
            }

          if(!encodechar.IsEmpty()) Add(encodechar);

        }
    }

  if(IsEmpty()) return false;

  return true;
}




/*-------------------------------------------------------------------
//  DIOURL::EncodeUnsafeChars
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/05/2011 16:24:19
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOURL::EncodeUnsafeChars()
{
  XSTRING string;

  string = Get();

  if(!EncodeUnsafeCharsFromString(string)) return false;

  return true;

}



/*-------------------------------------------------------------------
//  DIOURL::DecodeUnsafeCharsToString
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/05/2011 16:19:44
//
//  @return       bool :
//  @param        string :
*/
/*-----------------------------------------------------------------*/
bool DIOURL::DecodeUnsafeCharsToString(XSTRING& string)
{
  string.Empty();

  if(IsEmpty()) return false;

  for(int c=0;c<(int)GetSize();c++)
    {
      XCHAR character[2] = { 0 , 0 };

      character[0] = Get()[c];

      if(character[0]==__C('%'))
        {
          XSTRING   numberhex;
          XCHAR part[2] = { 0 , 0 };

          c++;

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
       else
        {
          if(character[0]==__C('+'))
            {
              string.Add(__C(' '));
            }
           else string.Add(character);
        }
    }

  if(string.IsEmpty()) return false;

  return true;
}



/*-------------------------------------------------------------------
//  DIOURL::DecodeUnsafeChars
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/05/2011 16:20:07
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOURL::DecodeUnsafeChars()
{
  XSTRING string;

  string = Get();

  if(!DecodeUnsafeCharsToString(string)) return false;

  Set(string);

  return true;
}




/*-------------------------------------------------------------------
//  DIOURL::Normalize
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      01/04/2011 18:00:13
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOURL::Normalize()
{
  XSTRING string;

  string = Get();

  EncodeUnsafeCharsFromString(string);

  return true;
}



/*-------------------------------------------------------------------
//  DIOURL::HaveHTTPID
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      01/04/2011 18:13:06
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOURL::HaveHTTPID()
{
  if(Find(DIOURL_WEBURLID,true)) return false;

  return true;
}


/*-------------------------------------------------------------------
//  DIOURL::AddHTTPID
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      01/04/2011 18:13:34
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOURL::AddHTTPID()
{
  if(HaveHTTPID()) return false;
  if(IsEmpty())    return false;

  XSTRING url;

  url  = DIOURL_WEBURLID;
  url += Get();

  Set(url);

  return true;
}




/*-------------------------------------------------------------------
//  DIOURL::DeleteHTTPID
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      27/07/2017 16:58:53
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOURL::DeleteHTTPID()
{
  if(!HaveHTTPID()) return false;
  if(IsEmpty())    return false;

  XSTRING url;
  XSTRING urlID;

  urlID = DIOURL_WEBURLID;
  url   = Get();

  if(url.Find(urlID, true) != XSTRING_NOTFOUND) url.DeleteCharacters(0, urlID.GetSize());

  Set(url);

  return true;
}




/*-------------------------------------------------------------------
//  DIOURL::Clean
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/03/2011 11:38:49
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void DIOURL::Clean()
{

}

