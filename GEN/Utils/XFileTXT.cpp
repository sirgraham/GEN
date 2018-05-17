//------------------------------------------------------------------------------------------
//  XFILETXT.CPP
//
//  TXT File Class
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

#include "XVector.h"
#include "XBuffer.h"

#include "XFileTXT.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------



//-------------------------------------------------------------------
//  XFILETXT::XFILETXT
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/02/2004 11:27:46
//
//  @return

*/
//-------------------------------------------------------------------
XFILETXT::XFILETXT() : XFILECONTAINER()
{
  Clean();

  lines.SetIsMulti(true);

  if(!CreatePrimaryFile()) return;
}


//-------------------------------------------------------------------
//  XFILETXT::~XFILETXT
/**
//
//
//  @author       Abraham J. Velez
//  @version      19/11/2002 18:35:37
//
//  @return       void :
//  */
//-------------------------------------------------------------------
XFILETXT::~XFILETXT()
{
  if(!DeletePrimaryFile()) return;

  DeleteFormatFile();

  DeleteAllLines();

  Clean();
}



//-------------------------------------------------------------------
//  XFILETXT::Open
/**
//
//
//  @author       Abraham J. Velez
//  @version      19/11/2002 18:36:57
//
//  @return       bool :
//  @param        pathname :
//  @param        readonly :
*/
//-------------------------------------------------------------------
bool XFILETXT::Open(XPATH& xpath,bool readonly)
{
  if(!file) return false;

  if(!file->Open(xpath,readonly)) return false;

  GetFormatCharFromFile(NULL);

  if(formatchar == XFILETXTFORMATCHAR_UNKNOWN) formatchar = XFILETXTFORMATCHAR_ASCII;
  typeLF = XFILETXTTYPELF_DEFAULT;

  return true;
}



/*-------------------------------------------------------------------
//  XFILETXT::Create
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      04/02/2011 21:43:38
//
//  @return       bool :
//  @param        xpath :
*/
/*-----------------------------------------------------------------*/
bool XFILETXT::Create(XPATH& xpath,XFILETXTFORMATCHAR formatchar, XFILETXTTYPELF typeLF)
{
  if(!file) return false;

  SetFormatChar(formatchar);
  SetTypeLF(typeLF);

  if(!file->Create(xpath)) return false;

  return true;
}



//-------------------------------------------------------------------
//  XFILETXT::IsOpen
/**
//
//
//  @author       Abraham J. Velez
//  @version      10/01/2001 17:11:16
//
//  @return       bool :
*/
//-------------------------------------------------------------------
bool XFILETXT::IsOpen()
{
  if(!file) return false;

  return file->IsOpen();
}


//-------------------------------------------------------------------
//  XFILETXT::Close
/**
//
//
//  @author       Abraham J. Velez
//  @version      19/11/2002 18:37:22
//
//  @return       bool :
*/
//-------------------------------------------------------------------
bool XFILETXT::Close()
{
  if(!file)          return false;
  if(!file->Close()) return false;

  return true;
}



/*-------------------------------------------------------------------
//  XFILETXT::GetFormatChar
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      04/02/2011 9:49:38
//
//  @return       XFILETXTFORMATCHAR :
//  */
/*-----------------------------------------------------------------*/
XFILETXTFORMATCHAR XFILETXT::GetFormatChar()
{
  return formatchar;
}



/*-------------------------------------------------------------------
//  XFILETXT::SetFormatChar
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      04/02/2011 9:49:46
//
//  @return       bool :
//  @param        formatchar :
*/
/*-----------------------------------------------------------------*/
bool XFILETXT::SetFormatChar(XFILETXTFORMATCHAR formatchar)
{
  this->formatchar = formatchar;

  return true;
}



/*-------------------------------------------------------------------
//  XFILETXT::GetFormatCharFromFile
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      01/02/2011 17:08:20
//
//  @return       XFILETXTFORMATCHAR :
//  @param        sizeBOM :
*/
/*-----------------------------------------------------------------*/
XFILETXTFORMATCHAR XFILETXT::GetFormatCharFromFile(XDWORD* sizeBOM)
{
  XFILETXTFORMATCHAR formatchar;

  XBYTE   BOM[3];
  XDWORD  position = 0;

  file->GetPosition(position);
  file->SetPosition(0);

  if(sizeBOM) (*sizeBOM) = 0;

  if(file->Read((XBYTE*)&BOM,3))
    {
      if((BOM[0]==0xEF)&&(BOM[1]==0xBB)&&(BOM[2]==0xBF))
        {
          formatchar = XFILETXTFORMATCHAR_UTF8;
          if(sizeBOM) (*sizeBOM) = 3;
        }
       else
        {
          if((BOM[0]==0xFE)&&(BOM[1]==0xFF))
            {
              formatchar = XFILETXTFORMATCHAR_UTF16_BE;
              if(sizeBOM) (*sizeBOM) = 2;
            }
           else
            {
              if((BOM[0]==0xFF)&&(BOM[1]==0xFE))
                {
                  formatchar = XFILETXTFORMATCHAR_UTF16_LE;
                  if(sizeBOM) (*sizeBOM) = 2;
                }
               else formatchar = XFILETXTFORMATCHAR_ASCII;
            }
        }

    } else formatchar = XFILETXTFORMATCHAR_ASCII;

  file->SetPosition(position);

  return formatchar;
}



/*-------------------------------------------------------------------
//  XFILETXT::CreateBOMFormatChar
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/02/2011 16:56:30
//
//  @return       bool :
//  @param        formatchar :
//  @param        BOM :
//  @param        sizeBOM :
*/
/*-----------------------------------------------------------------*/
bool XFILETXT::CreateBOMFormatChar(XFILETXTFORMATCHAR formatchar, XBYTE* BOM, XDWORD& sizeBOM)
{
  if(!BOM) return false;

  switch(formatchar)
    {
      case XFILETXTFORMATCHAR_UNKNOWN   :
      case XFILETXTFORMATCHAR_ASCII     : sizeBOM = 0;
                                          break;

      case XFILETXTFORMATCHAR_UTF8      : BOM[0]  = 0xEF;
                                          BOM[1]  = 0xBB;
                                          BOM[2]  = 0xBF;
                                          sizeBOM = 3;
                                          break;

      case XFILETXTFORMATCHAR_UTF16_BE  : BOM[0]  = 0xFE;
                                          BOM[1]  = 0xFF;
                                          sizeBOM = 2;
                                          break;

      case XFILETXTFORMATCHAR_UTF16_LE  : BOM[0]  = 0xFF;
                                          BOM[1]  = 0xFE;
                                          sizeBOM = 2;
                                          break;
    }

  return sizeBOM?true:false;
}



/*-------------------------------------------------------------------
//  XFILETXT::GetTypeLF
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      04/02/2011 9:58:38
//
//  @return       XFILETXTTYPELF :
//  */
/*-----------------------------------------------------------------*/
XFILETXTTYPELF XFILETXT::GetTypeLF()
{
  return typeLF;
}



/*-------------------------------------------------------------------
//  XFILETXT::SetTypeLF
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      04/02/2011 10:00:05
//
//  @return       bool :
//  @param        typeLF :
*/
/*-----------------------------------------------------------------*/
bool XFILETXT::SetTypeLF(XFILETXTTYPELF typeLF)
{
  this->typeLF = typeLF;

  return true;
}


/*-------------------------------------------------------------------
//  XFILETXT::CreateTypeLF
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      04/02/2011 10:02:08
//
//  @return       bool :
//  @param        typeLF :
//  @param        LF :
//  @param        sizeLF :
*/
/*-----------------------------------------------------------------*/
bool XFILETXT::CreateTypeLF(XFILETXTFORMATCHAR formatchar,XFILETXTTYPELF typeLF, XBYTE* LF, XDWORD& sizeLF)
{
  if(!LF) return false;

  sizeLF = 0;

  switch(formatchar)
    {
      case XFILETXTFORMATCHAR_UNKNOWN   : break;
      case XFILETXTFORMATCHAR_ASCII     :
      case XFILETXTFORMATCHAR_UTF8      : switch(typeLF)
                                            {
                                              case XFILETXTTYPELF_UNKNOWN : break;

                                              case XFILETXTTYPELF_0A      : LF[0]  = 0x0A;
                                                                            sizeLF = 1;
                                                                            break;

                                              case XFILETXTTYPELF_0D      : LF[0]  = 0x0D;
                                                                            sizeLF = 1;
                                                                            break;

                                              case XFILETXTTYPELF_0A0D    : LF[0]  = 0xA0;
                                                                            LF[1]  = 0x0D;
                                                                            sizeLF = 2;
                                                                            break;

                                              case XFILETXTTYPELF_0D0A    : LF[0]  = 0x0D;
                                                                            LF[1]  = 0x0A;
                                                                            sizeLF = 2;
                                                                            break;
                                            }
                                          break;

      case XFILETXTFORMATCHAR_UTF16_BE  : switch(typeLF)
                                            {
                                              case XFILETXTTYPELF_UNKNOWN : break;

                                              case XFILETXTTYPELF_0A      : LF[0]  = 0x00;
                                                                            LF[1]  = 0x0A;
                                                                            sizeLF = 2;
                                                                            break;

                                              case XFILETXTTYPELF_0D      : LF[0]  = 0x00;
                                                                            LF[1]  = 0x0D;
                                                                            sizeLF = 2;
                                                                            break;

                                              case XFILETXTTYPELF_0A0D    : LF[0]  = 0x00;
                                                                            LF[1]  = 0x0A;
                                                                            LF[2]  = 0x00;
                                                                            LF[3]  = 0x0D;
                                                                            sizeLF = 4;
                                                                            break;

                                              case XFILETXTTYPELF_0D0A    : LF[0]  = 0x00;
                                                                            LF[1]  = 0x0D;
                                                                            LF[2]  = 0x00;
                                                                            LF[3]  = 0x0A;
                                                                            sizeLF = 4;
                                                                            break;
                                            }
                                          break;

      case XFILETXTFORMATCHAR_UTF16_LE  : switch(typeLF)
                                            {
                                              case XFILETXTTYPELF_UNKNOWN : break;

                                              case XFILETXTTYPELF_0A      : LF[0]  = 0x0A;
                                                                            LF[1]  = 0x00;
                                                                            sizeLF = 2;
                                                                            break;

                                              case XFILETXTTYPELF_0D      : LF[0]  = 0x0D;
                                                                            LF[1]  = 0x00;
                                                                            sizeLF = 2;
                                                                            break;

                                              case XFILETXTTYPELF_0A0D    : LF[0]  = 0x0A;
                                                                            LF[1]  = 0x00;
                                                                            LF[2]  = 0x0D;
                                                                            LF[3]  = 0x00;
                                                                            sizeLF = 4;
                                                                            break;

                                              case XFILETXTTYPELF_0D0A    : LF[0]  = 0x0D;
                                                                            LF[1]  = 0x00;
                                                                            LF[2]  = 0x0A;
                                                                            LF[3]  = 0x00;
                                                                            sizeLF = 4;
                                                                            break;
                                            }
                                          break;
    }

  return true;
}



/*-------------------------------------------------------------------
//  XFILETXT::GetLF
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      05/10/2012 8:47:58
//
//  @return       bool :
//  @param        lfdata :
*/
/*-----------------------------------------------------------------*/
bool XFILETXT::GetLF(XBUFFER& lfdata)
{
  switch(typeLF)
    {
      case XFILETXTTYPELF_UNKNOWN  : return false;

      case XFILETXTTYPELF_0A      : lfdata.Add((XBYTE)0x0A);
                                    break;

      case XFILETXTTYPELF_0D      : lfdata.Add((XBYTE)0x0D);
                                    break;

      case XFILETXTTYPELF_0A0D    : lfdata.Add((XBYTE)0x0A);
                                    lfdata.Add((XBYTE)0x0D);
                                    break;

      case XFILETXTTYPELF_0D0A    : lfdata.Add((XBYTE)0x0D);
                                    lfdata.Add((XBYTE)0x0A);
                                    break;

    }

  return true;
}




/*-------------------------------------------------------------------
//  XFILETXT::GetLF
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      10/01/2014 12:11:07
//
//  @return       XSTRING* :
//  */
/*-----------------------------------------------------------------*/
XSTRING* XFILETXT::GetLF()
{
  LF.Empty();

  switch(typeLF)
    {
      case XFILETXTTYPELF_UNKNOWN  : break;
      case XFILETXTTYPELF_0A      : LF = __L("\n");    break;
      case XFILETXTTYPELF_0D      : LF = __L("\r");    break;
      case XFILETXTTYPELF_0A0D    : LF = __L("\n\r");  break;
      case XFILETXTTYPELF_0D0A    : LF = __L("\r\n");  break;
    }

  return &LF;
}




/*-------------------------------------------------------------------
//  XFILETXT::GetNLines
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/02/2011 12:32:37
//
//  @return       int :
//  */
/*-----------------------------------------------------------------*/
int XFILETXT::GetNLines()
{
  return lines.GetSize();
}



/*-------------------------------------------------------------------
//  XFILETXT::GetLine
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      01/02/2011 16:41:37
//
//  @return       XSTRING* :
//  @param        index :
*/
/*-----------------------------------------------------------------*/
XSTRING* XFILETXT::GetLine(int index)
{
  //if(index<0)                     return NULL;
  //if(index>=(int)lines.GetSize()) return NULL;
  //si index es <0 o index es >GetSize Get retorna NULL

  //XSTRING* string = (XSTRING*)lines.Get(index); //y ese casting?
  //return string;

  return lines.FastGet(index);
}



/*-------------------------------------------------------------------
//  XFILETXT::GetLineText
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      01/02/2011 16:47:30
//
//  @return       XCHAR* :
//  @param        index :
*/
/*-----------------------------------------------------------------*/
XCHAR* XFILETXT::GetLineText(int index)
{
  XSTRING* string = GetLine(index);
  if(!string) return NULL;

  return string->Get();
}



/*-------------------------------------------------------------------
//  XFILETXT::GetAllInOneLine
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      19/03/2014 14:21:56
//
//  @return       bool :
//
//  @param        alllines :
//  @param        start :
//  @param        end :
*/
/*-----------------------------------------------------------------*/
bool XFILETXT::GetAllInOneLine(XSTRING& alllines, XDWORD start, XDWORD end)
{
  alllines.Empty();

  int _end = end;
  if(_end == XFILETXT_TOLASTLINE) _end = GetNLines();

  for(int c=start; c<_end; c++)
    {
      XSTRING* line = GetLine(c);
      if(line) alllines += line->Get();
    }

  if(alllines.IsEmpty()) return false;

  return true;
}




/*-------------------------------------------------------------------
//  XFILETXT::GetAllInBuffer
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      19/05/2014 9:58:57
//
//  @return       bool :
//
//  @param        xbuffer :
//  @param        start :
//  @param        end :
*/
/*-----------------------------------------------------------------*/
bool XFILETXT::GetAllInBuffer(XBUFFER& xbuffer, XDWORD start, XDWORD end)
{
  xbuffer.Delete();

  int _end = end;
  if(_end == XFILETXT_TOLASTLINE) _end = GetNLines();

  for(int c=start; c<_end; c++)
    {
      XSTRING* line = GetLine(c);

      if(line) xbuffer.Add((*line));
    }

  if(!xbuffer.GetSize()) return false;

  return true;
}





/*-------------------------------------------------------------------
//  XFILETXT::ReadAllFile
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      01/02/2011 16:56:57
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
/*
bool XFILETXT::ReadAllFile()
{
  if(!file)           return false;
  if(!file->IsOpen()) return false;

  DeleteAllLines();

  XQWORD              file_pointer_position=0;
  int                sizeBOM = 0;
  XFILETXTFORMATCHAR formatchar    = GetFormatCharFromFile(&sizeBOM);

  if(this->formatchar==XFILETXTFORMATCHAR_UNKNOWN) this->formatchar = formatchar;

  file->SetPosition(sizeBOM);


  XBYTE  xbuffer;
  XBYTE* buffer = new XBYTE[XFILETXT_MAXBUFFER];
  if(buffer)
    {
      int br;

      do{ br = XFILETXT_MAXBUFFER;
          memset(buffer,0,XFILETXT_MAXBUFFER);

          bool endfile = !file->Read(buffer, &br);
          if(!br) break;

          file_pointer_position += br;

          int nchar = 0;

          do{ XFILETXTTYPELF typeLF;
              int            sizeLF   = 0;
              int            sizeline = 0;

              bool endline = GetSizeOfLine(formatchar,&buffer[nchar],typeLF,sizeLF,sizeline, (br-nchar));
              if((endline) || ((!endline) && (endfile)))
                {
                  if((!endline)&&(endfile)) sizeline = (br-nchar);

                  if(this->typeLF == XFILETXTTYPELF_UNKNOWN) this->typeLF = typeLF;

                  switch(formatchar)
                    {
                      case XFILETXTFORMATCHAR_UNKNOWN   :
                      case XFILETXTFORMATCHAR_ASCII     :
                      case XFILETXTFORMATCHAR_UTF8      : if(sizeLF)   buffer[nchar+sizeline]   = 0;
                                                          if(sizeLF>1) buffer[nchar+sizeline+1] = 0;
                                                          break;

                      case XFILETXTFORMATCHAR_UTF16_BE  :
                      case XFILETXTFORMATCHAR_UTF16_LE  : if(sizeLF)
                                                            {
                                                              buffer[nchar+(sizeline*2)]   = 0;
                                                              buffer[nchar+(sizeline*2)+1] = 0;
                                                            }
                                                          if(sizeLF>1)
                                                            {
                                                              buffer[nchar+(sizeline*2)+2] = 0;
                                                              buffer[nchar+(sizeline*2)+3] = 0;
                                                            }
                                                          break;
                    }

                  AddLine(formatchar,(XBYTE*)&buffer[nchar], sizeline);

                  switch(formatchar)
                    {
                      case XFILETXTFORMATCHAR_UNKNOWN   :
                      case XFILETXTFORMATCHAR_ASCII     :
                      case XFILETXTFORMATCHAR_UTF8      : nchar+=(sizeline+(endline?sizeLF:0));
                                                          break;

                      case XFILETXTFORMATCHAR_UTF16_BE  :
                      case XFILETXTFORMATCHAR_UTF16_LE  : nchar+=((sizeline*2)+(endline?(sizeLF*2):0));
                                                          break;
                    }
                }
               else
                {
                  file_pointer_position += (br-nchar);
                  file->SetPosition((int)file_pointer_position);

                  AddLine(formatchar,(XBYTE*)&buffer[nchar], (br-nchar));

                  break;
                }

            } while(nchar<br);

        } while(br == XFILETXT_MAXBUFFER);


      delete [] buffer;
    }

  return true;
}
*/


bool XFILETXT::ReadAllFile()
{
  if(!file)           return false;
  if(!file->IsOpen()) return false;

  DeleteAllLines();

  XDWORD             sizeBOM              = 0;
  XFILETXTFORMATCHAR formatchar           = GetFormatCharFromFile(&sizeBOM);
  XDWORD             sizebytescharacter   = SizeOfCharacter(formatchar);

  if(this->formatchar==XFILETXTFORMATCHAR_UNKNOWN) this->formatchar = formatchar;

  file->SetPosition(sizeBOM);

  bool    endfile;
  XDWORD  br;
  XBUFFER dataline(false);

  XBYTE*  readbuffer = new XBYTE[XFILETXT_MAXBUFFER];
  if(!readbuffer) return false;

  memset(readbuffer, 0, XFILETXT_MAXBUFFER);

  do{ XDWORD bufferpos = 0;

      br            = XFILETXT_MAXBUFFER;
      endfile = !file->Read(readbuffer, &br);
      if(!br) break;

      do{ XFILETXTTYPELF  _typeLF   = XFILETXTTYPELF_UNKNOWN;
          XDWORD          sizeLF    = 0;
          XDWORD          sizeline  = 0;
          bool            endline   = GetSizeOfLine(formatchar, &readbuffer[bufferpos], _typeLF, sizeLF, sizeline, (br-bufferpos));

          if(typeLF == XFILETXTTYPELF_UNKNOWN && _typeLF != XFILETXTTYPELF_UNKNOWN) typeLF = _typeLF;

          if(endline)
            {
              dataline.Add(&readbuffer[bufferpos], sizebytescharacter*sizeline);
              AddLine(formatchar, dataline.Get(), (int)(dataline.GetSize()/sizebytescharacter));
              dataline.Delete();

              sizeline  *= sizebytescharacter;
              sizeline  += (sizeLF*sizebytescharacter);
              bufferpos += sizeline;
            }
           else
            {
              int lack = (br-bufferpos);
              dataline.Add(&readbuffer[bufferpos], lack);
              bufferpos += lack;

              if(endfile && (bufferpos == br))
                {
                  AddLine(formatchar, dataline.Get(), (int)(lack/sizebytescharacter));
                  dataline.Delete();
                }
            }

        } while(bufferpos < br);

    } while(!endfile);

  delete [] readbuffer;

  return true;
}





/*-------------------------------------------------------------------
//  XFILETXT::WriteAllFile
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/02/2011 16:53:42
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XFILETXT::WriteAllFile()
{
  if(!file)           return false;
  if(!file->IsOpen()) return false;

  if(lines.IsEmpty()) return false;

  XBYTE   BOM[3]  = { 0,0,0 };
  XDWORD  sizeBOM = 0;

  XBYTE   LF[4]   = { 0,0,0,0 };
  XDWORD  sizeLF  = 0;

  CreateBOMFormatChar(formatchar, BOM, sizeBOM);

  CreateTypeLF(formatchar, typeLF, LF, sizeLF);

  file->SetPosition(0);
  if(sizeBOM)
    {
      if(!file->Write(BOM,sizeBOM)) return false;
    }


  XBUFFER xbuffer;
  XBYTE*  buffer;
  bool    status    = true;

  buffer    = new XBYTE[XFILETXT_MAXBUFFER];
  if(buffer)
    {
      int bw;
      memset(buffer,0,XFILETXT_MAXBUFFER);

      for(int c=0;c<(int)lines.GetSize();c++)
        {
          XSTRING* string = (XSTRING*)lines.Get(c);
          bw = 0;

          if(string)
            {
              switch(formatchar)
                {
                  case XFILETXTFORMATCHAR_UNKNOWN   : break;

                  case XFILETXTFORMATCHAR_ASCII     : { bw = string->GetSize();

                                                        XSTRING_CREATEOEM((*string), charstr);
                                                        memcpy(buffer,(XBYTE*)charstr, bw);
                                                        XSTRING_DELETEOEM(charstr)
                                                      }
                                                      break;

                  case XFILETXTFORMATCHAR_UTF8      : string->ConvertToUTF8(buffer,bw);
                                                      break;

                  case XFILETXTFORMATCHAR_UTF16_BE  : { XWORD* bufferw = (XWORD*)buffer;

                                                        bw = (string->GetSize()*2);

                                                        for(int d=0;d<(int)string->GetSize();d++)
                                                          {
                                                            bufferw[d] = (XWORD)string->Get()[d];
                                                            bufferw[d] = (bufferw[d]<<8) | (bufferw[d]>>8);
                                                          }
                                                      }
                                                      break;

                  case XFILETXTFORMATCHAR_UTF16_LE  : { XWORD* bufferw = (XWORD*)buffer;

                                                        bw = (string->GetSize()*2);

                                                        for(int d=0;d<(int)string->GetSize();d++)
                                                          {
                                                            bufferw[d] = (XWORD)string->Get()[d];
                                                          }
                                                      }
                                                      break;
                }

              if(bw) xbuffer.Add(buffer, bw);
              xbuffer.Add(LF    ,sizeLF);
            }
        }

      status = file->Write(xbuffer);

      delete [] buffer;
    }

  if(status)
    {
      XDWORD position = 0;
      file->GetPosition(position);
      status = file->SetSize(position);
    }

  return status;
}



/*-------------------------------------------------------------------
//  XFILETXT::DeleteFormatFile
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      14/02/2011 10:20:18
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XFILETXT::DeleteFormatFile()
{
  formatchar = XFILETXTFORMATCHAR_UNKNOWN;
  typeLF     = XFILETXTTYPELF_UNKNOWN;

  return true;
}



/*-------------------------------------------------------------------
//  XFILETXT::AddLine
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      04/02/2011 21:28:11
//
//  @return       bool :
//  @param        line :
*/
/*-----------------------------------------------------------------*/
bool XFILETXT::AddLine(XCHAR* line)
{
  if(!line) return false;

  XSTRING string(line);

  return AddLine(string);
}



/*-------------------------------------------------------------------
//  XFILETXT::AddLine
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      04/02/2011 21:29:08
//
//  @return       bool :
//  @param        line :
*/
/*-----------------------------------------------------------------*/
bool XFILETXT::AddLine(XSTRING& line)
{
  XSTRING* string = new XSTRING(line);
  if(!string) return false;

  lines.Add(string);

  return true;
}




/*-------------------------------------------------------------------
//  XFILETXT::GenerateLineFromBuffer
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      08/01/2014 9:47:17
//
//  @return       bool :
//  @param        formatchar :
//  @param        line :
//  @param        sizeline :
//  @param        string :
*/
/*-----------------------------------------------------------------*/
bool XFILETXT::GenerateLineFromBuffer(XFILETXTFORMATCHAR formatchar, XBYTE* line, XDWORD sizeline, XSTRING& string)
{
  if(!line)     return true;
  if(!sizeline) return true;

  switch(formatchar)
    {
      case XFILETXTFORMATCHAR_UNKNOWN     : break;

      case XFILETXTFORMATCHAR_ASCII       :
      case XFILETXTFORMATCHAR_UTF8        :
                                            string.ConvertFromUTF8((XBYTE*)line, sizeline);
                                            break;

      case XFILETXTFORMATCHAR_UTF16_BE    : { XCHAR* unibuffer = new XCHAR[sizeline];
                                              XWORD* wline     = (XWORD*)line;

                                              if(unibuffer)
                                                {
                                                  for(XDWORD c=0; c<sizeline; c++)
                                                    {
                                                      wline[c]     = (wline[c]<<8) | (wline[c]>>8);
                                                      unibuffer[c] = wline[c];
                                                    }

                                                  string.Set(unibuffer,sizeline);

                                                  delete [] unibuffer;
                                                }
                                             }
                                             break;

      case XFILETXTFORMATCHAR_UTF16_LE   :  {  XCHAR* unibuffer = new XCHAR[sizeline];
                                               XWORD*          wline     = (XWORD*)line;
                                               if(unibuffer)
                                                 {
                                                   for(XDWORD c=0; c<sizeline; c++)
                                                     {
                                                       unibuffer[c] = wline[c];
                                                     }

                                                   string.Set(unibuffer,sizeline);

                                                   delete [] unibuffer;
                                                 }
                                            }
                                            break;
    }

  return true;
}




/*-------------------------------------------------------------------
//  XFILETXT::AddLine
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      01/02/2011 22:29:17
//
//  @return       bool :
//  @param        formatchar :
//  @param        line :
//  @param        sizeline :
*/
/*-----------------------------------------------------------------*/
bool XFILETXT::AddLine(XFILETXTFORMATCHAR formatchar, XBYTE* line, XDWORD sizeline)
{
  XSTRING* string = new XSTRING();
  if(!string) return false;

  if(GenerateLineFromBuffer(formatchar, line, sizeline, (*string)))
    {
      lines.Add(string);
    }
   else
    {
      delete string;
      string = NULL;
    }

  return string?true:false;
}





/*-------------------------------------------------------------------
//  XFILETXT::AddLineAlready
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      08/01/2014 10:49:22
//
//  @return       bool :
//  @param        line :
//  @param        resultsizeline :
//  @param        resultsizeLF :
*/
/*-----------------------------------------------------------------*/
bool XFILETXT::AddLineAlready(XCHAR* line, XDWORD* resultsizeline, XDWORD* resultsizeLF)
{
  if(!line) return false;

  XSTRING string(line);

  return AddLineAlready(string, resultsizeline, resultsizeLF);
}





/*-------------------------------------------------------------------
//  XFILETXT::AddLineAlready
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      08/01/2014 10:48:50
//
//  @return       bool :
//  @param        line :
//  @param        resultsizeline :
//  @param        resultsizeLF :
*/
/*-----------------------------------------------------------------*/
bool XFILETXT::AddLineAlready(XSTRING& line, XDWORD* resultsizeline, XDWORD* resultsizeLF)
{
  if(!file)           return false;
  if(!file->IsOpen()) return false;

  XBYTE   LF[4]   = { 0,0,0,0 };
  XDWORD  sizeLF  = 0;

  CreateTypeLF(formatchar, typeLF, LF, sizeLF);

  file->SetPosition(XFILE_SEEKEND);

  bool status = true;

  XBYTE* buffer = new XBYTE[XFILETXT_MAXBUFFER];
  if(buffer)
    {
      int bw = 0;
      memset(buffer,0,XFILETXT_MAXBUFFER);

      switch(formatchar)
        {
          case XFILETXTFORMATCHAR_UNKNOWN   : break;

          case XFILETXTFORMATCHAR_ASCII     : { bw = line.GetSize();

                                                XSTRING_CREATEOEM(line, charstr)
                                                memcpy(buffer, (XBYTE*)charstr, bw);
                                                XSTRING_DELETEOEM(charstr)
                                              }
                                              break;

          case XFILETXTFORMATCHAR_UTF8      : line.ConvertToUTF8(buffer,bw);
                                              break;

          case XFILETXTFORMATCHAR_UTF16_BE  : { XWORD* bufferw = (XWORD*)buffer;

                                                bw = (line.GetSize()*2);

                                                for(XDWORD d=0; d<(int)line.GetSize(); d++)
                                                  {
                                                    bufferw[d] = (XWORD)line.Get()[d];
                                                    bufferw[d] = (bufferw[d]<<8) | (bufferw[d]>>8);
                                                  }
                                              }
                                              break;

          case XFILETXTFORMATCHAR_UTF16_LE  : { XWORD* bufferw = (XWORD*)buffer;

                                                bw = (line.GetSize()*2);

                                                for(XDWORD d=0; d<(int)line.GetSize(); d++)
                                                  {
                                                    bufferw[d] = (XWORD)line.Get()[d];
                                                  }
                                              }
                                              break;
        }

      if(bw)
        {
          if(file->Write(buffer,bw))
            {
              if(file->Write(LF,sizeLF))
                {
                  if(resultsizeline) (*resultsizeline) = bw;
                  if(resultsizeLF)   (*resultsizeLF)   = sizeLF;

                }  else status = false;

            } else status = false;
        }

      delete [] buffer;
    }

  return status;
}




/*-------------------------------------------------------------------
//  XFILETXT::InsertLine
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      15/10/2012 13:28:32
//
//  @return       bool :
//  @param        index :
//  @param        line :
*/
/*-----------------------------------------------------------------*/
bool XFILETXT::InsertLine(int index, XCHAR* line)
{
  if(!line) return false;

  XSTRING string(line);

  return InsertLine(index, string);
}




/*-------------------------------------------------------------------
//  XFILETXT::InsertLine
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      15/10/2012 13:28:42
//
//  @return       bool :
//  @param        index :
//  @param        line :
*/
/*-----------------------------------------------------------------*/
bool XFILETXT::InsertLine(int index, XSTRING& line)
{
  XSTRING* string = new XSTRING(line);
  if(!string) return false;

  return lines.Insert(index,string);
}



/*-------------------------------------------------------------------
//  XFILETXT::DeleteLine
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      04/02/2011 21:37:27
//
//  @return       bool :
//  @param        index :
*/
/*-----------------------------------------------------------------*/
bool XFILETXT::DeleteLine(int index)
{
  if(index<0)         return false;
  if(lines.IsEmpty()) return false;

  XSTRING* line = (XSTRING*)lines.Get(index);
  if(!line) return false;

  lines.Delete(line);

  delete line;

  return true;
}



/*-------------------------------------------------------------------
//  XFILETXT::DeleteAllLines
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      01/02/2011 12:05:14
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XFILETXT::DeleteAllLines()
{
  if(lines.IsEmpty())  return false;

  lines.DeleteContents();
  lines.DeleteAll();

  return true;
}



/*-------------------------------------------------------------------
//  XFILETXT::AddBufferLines
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      01/04/2013 11:13:22
//
//  @return       bool :
//  @param        xbuffer :
*/
/*-----------------------------------------------------------------*/
bool XFILETXT::AddBufferLines(XFILETXTFORMATCHAR formatchar, XBUFFER& xbuffer)
{
  if(xbuffer.IsEmpty()) return false;

  XBYTE* buffer = xbuffer.Get();
  if(!buffer) return false;

  int br    = xbuffer.GetSize();
  int nchar = 0;

  this->formatchar = formatchar;

  do{ XFILETXTTYPELF typeLF;
      XDWORD         sizeLF   = 0;
      XDWORD         sizeline = 0;

      bool endline = GetSizeOfLine(formatchar, &buffer[nchar], typeLF, sizeLF, sizeline, (br-nchar));

      if(!endline) sizeline = (br-nchar);

      if(this->typeLF == XFILETXTTYPELF_UNKNOWN) this->typeLF = typeLF;

      /*
      switch(formatchar)
        {
          case XFILETXTFORMATCHAR_UNKNOWN   :
          case XFILETXTFORMATCHAR_ASCII     :
          case XFILETXTFORMATCHAR_UTF8      : if(sizeLF)   buffer[nchar+sizeline]   = 0;
                                              if(sizeLF>1) buffer[nchar+sizeline+1] = 0;
                                              break;

          case XFILETXTFORMATCHAR_UTF16_BE  :
          case XFILETXTFORMATCHAR_UTF16_LE  : if(sizeLF)
                                                {
                                                  buffer[nchar+(sizeline*2)]   = 0;
                                                  buffer[nchar+(sizeline*2)+1] = 0;
                                                }
                                              if(sizeLF>1)
                                                {
                                                  buffer[nchar+(sizeline*2)+2] = 0;
                                                  buffer[nchar+(sizeline*2)+3] = 0;
                                                }
                                              break;
        }
      */

      AddLine(formatchar, (XBYTE*) &buffer[nchar], sizeline);

      switch(formatchar)
        {
          case XFILETXTFORMATCHAR_UNKNOWN   :
          case XFILETXTFORMATCHAR_ASCII     :
          case XFILETXTFORMATCHAR_UTF8      : nchar+=(sizeline+(endline?sizeLF:0));
                                              break;

          case XFILETXTFORMATCHAR_UTF16_BE  :
          case XFILETXTFORMATCHAR_UTF16_LE  : nchar+=((sizeline*2)+(endline?(sizeLF*2):0));
                                              break;
        }

      } while(nchar<br);

  return true;
}



/*-------------------------------------------------------------------
//  XFILETXT::GetSizeOfLine
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      08/01/2014 9:35:24
//
//  @return       bool :
//  @param        formatchar :
//  @param        buffer :
//  @param        typelf :
//  @param        sizeLF :
//  @param        size :
//  @param        maxsize :
*/
/*-----------------------------------------------------------------*/
bool XFILETXT::GetSizeOfLine(XFILETXTFORMATCHAR formatchar, XBYTE* buffer, XFILETXTTYPELF& typeLF, XDWORD& sizeLF, XDWORD& size, XDWORD maxsize)
{
  bool    status = false;
  XDWORD  c      = 0;

  sizeLF = 0;

  typeLF = XFILETXTTYPELF_UNKNOWN;

  XWORD* bufw = (XWORD*) buffer;
  XBYTE* bufb = (XBYTE*) buffer;

  switch(formatchar)
    {
      case XFILETXTFORMATCHAR_UNKNOWN   :
      case XFILETXTFORMATCHAR_ASCII     :
      case XFILETXTFORMATCHAR_UTF8      : for(c=0; c<maxsize; c++)
                                            {
                                              if(bufb[c]==0x0D)
                                                {
                                                  status = true;
                                                  sizeLF = 1;
                                                  typeLF = XFILETXTTYPELF_0D;
                                                  if(bufb[c+1]==0x0A)
                                                    {
                                                      typeLF = XFILETXTTYPELF_0D0A;
                                                      sizeLF++;
                                                    }
                                                  break;
                                                }

                                              if(bufb[c]==0x0A)
                                                {
                                                  status = true;
                                                  sizeLF = 1;
                                                  typeLF = XFILETXTTYPELF_0A;
                                                  if((c+1) < maxsize)
                                                    {
                                                      if(bufb[c+1]==0x0D)
                                                        {
                                                          typeLF = XFILETXTTYPELF_0A0D;
                                                          sizeLF++;
                                                        }
                                                    }
                                                  break;
                                                }
                                            }
                                          break;

      case XFILETXTFORMATCHAR_UTF16_LE  : for(c=0;c<maxsize/2;c++)
                                            {
                                              if(bufw[c]==0x000D)
                                                {
                                                  status = true;
                                                  sizeLF = 1;
                                                  typeLF = XFILETXTTYPELF_0D;
                                                  if(bufw[c+1]==0x000A)
                                                    {
                                                      typeLF = XFILETXTTYPELF_0D0A;
                                                      sizeLF++;
                                                    }
                                                  break;
                                                }

                                              if(bufw[c]==0x000A)
                                                {
                                                  status = true;
                                                  sizeLF = 1;
                                                  typeLF = XFILETXTTYPELF_0A;
                                                  if(bufw[c+1]==0x000D)
                                                    {
                                                      typeLF = XFILETXTTYPELF_0A0D;
                                                      sizeLF++;
                                                    }
                                                  break;
                                                }
                                            }
                                          break;

      case XFILETXTFORMATCHAR_UTF16_BE  : for(c=0; c<maxsize/2; c++)
                                            {
                                              if(bufw[c]==0x0D00)
                                                {
                                                  status = true;
                                                  sizeLF = 1;
                                                  typeLF = XFILETXTTYPELF_0D;
                                                  if(bufw[c+1]==0x0A00)
                                                    {
                                                      typeLF = XFILETXTTYPELF_0D0A;
                                                      sizeLF++;
                                                    }
                                                  break;
                                                }

                                              if(bufw[c]==0x0A00)
                                                {
                                                  status = true;
                                                  sizeLF = 1;
                                                  typeLF = XFILETXTTYPELF_0A;
                                                  if(bufw[c+1]==0x0D00)
                                                    {
                                                      typeLF = XFILETXTTYPELF_0A0D;
                                                      sizeLF++;
                                                    }
                                                  break;
                                                }
                                            }
                                          break;
    }

  if(status) size = c;

  return status;
}




//-------------------------------------------------------------------
//  XFILETXT::Clean
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/11/2002 11:08:06
//
//  @return       void :
//  */
//-------------------------------------------------------------------
void XFILETXT::Clean()
{
  file       = NULL;
  formatchar = XFILETXTFORMATCHAR_UNKNOWN;
  typeLF     = XFILETXTTYPELF_UNKNOWN;
}


