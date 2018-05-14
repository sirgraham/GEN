//------------------------------------------------------------------------------------------
//  XFILEHEX.CPP
//
//  BMP File functions (Bitmap file format)
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 19/11/2002
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>

#include "XFileHEX.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  XFILEHEX::XFILEHEX
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/02/2004 11:27:46
//
//  @return

*/
//-------------------------------------------------------------------
XFILEHEX::XFILEHEX() : XFILETXT()
{
  Clean();
}


//-------------------------------------------------------------------
//  XFILEHEX::~XFILEHEX
/**
//
//
//  @author       Abraham J. Velez
//  @version      19/11/2002 18:35:37
//
//  @return       void :
//  */
//-------------------------------------------------------------------
XFILEHEX::~XFILEHEX()
{
  Clean();
}



//-------------------------------------------------------------------
//  XFILEHEX::GetBinaryBuffer
/**
//
//  @author        Abraham J. Velez
//  @version       24/4/2003   17:25:50
//
//  @return        XBYTE* :
//  @param         size :
*/
//-------------------------------------------------------------------
XBYTE* XFILEHEX::GetBinaryBuffer(int& size)
{
  size = 0;

  if(!ReadAllFile()) return NULL;

  for(int c=0;c<GetNLines();c++)
    {
      XSTRING* line = GetLine(c);
      if(line)
        {
          if(line->Get()[0]==__C(':')) size += GetSizeOfLine((*line));
        }
    }

  XBYTE* buffer = NULL;
  int   index  = 0;

  buffer = new XBYTE[size];
  if(!buffer)  return NULL;

  memset(buffer,0,size);

  for(int c=0;c<GetNLines();c++)
    {
      XSTRING* line = GetLine(c);
      if(line)
        {
          if(line->Get()[0]==__C(':'))
            {
              int  tsize;
              int  taddr;
              int  ttype;
              int  data;
              int  crc;
              int  dcrc = 0;

              tsize = GetBYTEofLine(&line->Get()[1]);
              taddr = GetWORDofLine(&line->Get()[3]);
              ttype = GetBYTEofLine(&line->Get()[7]);

              crc =(tsize&0xFF) + ((taddr>>8)&0xFF) + (taddr&0xFF) + (ttype&0xFF);

              switch(ttype)
                {
                  case 0: {  int d = 0;

                             if(tsize)
                               {
                                 for(d=9;d<(tsize*2)+9;d+=2)
                                   {
                                      data = GetBYTEofLine(&line->Get()[d]);
                                      crc += (data&0xFF);
                                      buffer[index] = data;
                                      index++;
                                   }
                               }

                              dcrc = GetBYTEofLine(&line->Get()[d]);
                          }
                          break;

                  case 1: dcrc = GetBYTEofLine(&line->Get()[9]);
                          break;
                }

              if(((crc&0xFF) + (dcrc&0xFF))&0xFF)
                {
                  delete [] buffer;
                  return NULL;
                }

            }
        }
    }

  return buffer;
}



/*-------------------------------------------------------------------
//  XFILEHEX::GetSizeOfLine
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      14/09/2011 11:25:49
//
//  @return       int :
//  @param        line :
*/
/*-----------------------------------------------------------------*/
int XFILEHEX::GetSizeOfLine(XSTRING& line)
{
  if(line.Get()[0]!=__C(':')) return 0;

  XSTRING sizestring;
  XCHAR      sizestr[3] = { 0 , 0 , 0 };
  int        size;

  sizestr[0] = line[1];
  sizestr[1] = line[2];

  sizestring = sizestr;

  sizestring.UnFormat(__L("%02X"),&size);

  return size;
}




/*-------------------------------------------------------------------
//  XFILEHEX::GetBYTEofLine
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      14/09/2011 11:50:18
//
//  @return       XBYTE :
//  @param        line :
*/
/*-----------------------------------------------------------------*/
XBYTE XFILEHEX::GetBYTEofLine(XCHAR* line)
{
  XSTRING string;
  XCHAR      str[3] = { 0 , 0 , 0 };
  XBYTE      data;

  for(int c=0;c<2;c++)
    {
      str[c] = line[c];
    }

  string = str;

  string.UnFormat(__L("%02X"),&data);

  return data;
}




/*-------------------------------------------------------------------
//  XFILEHEX::GetWORDofLine
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      14/09/2011 11:52:52
//
//  @return       XWORD :
//  @param        line :
*/
/*-----------------------------------------------------------------*/
XWORD XFILEHEX::GetWORDofLine(XCHAR* line)
{
  XSTRING string;
  XCHAR      str[5] = { 0 , 0 , 0 , 0 , 0 };
  XWORD      data;

  for(int c=0;c<4;c++)
    {
      str[c] = line[c];
    }

  string = str;

  string.UnFormat(__L("%04X"),&data);

  return data;
}




//-------------------------------------------------------------------
//  XFILEHEX::Clean
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/11/2002 11:08:06
//
//  @return       void :
//  */
//-------------------------------------------------------------------
void XFILEHEX::Clean()
{

}



