//------------------------------------------------------------------------------------------
//  XFILEZIP.CPP
//
//  File Class For the function of ZIP
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 27/11/2003 17:33:41
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include "XFactory.h"
#include "XDateTime.h"
#include "XPath.h"

#include "XFileZIP.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------



/*-------------------------------------------------------------------
//  XFILECMPZIP::XFILECMPZIP
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/02/2010 11:40:54
//
//  @return

//  @param        compress :
//  @param        filehdl :
*/
/*-----------------------------------------------------------------*/
XFILECMPZIP::XFILECMPZIP(bool compress,void* filehdl)
{
  Clean();

  this->filehdl  = filehdl;

  zipoper = compress;

  if(!zipoper)
    {
      unz_file_info info;
      char*         _name    = new char[_MAXPATH];
      char*         _comment = new char[_MAXPATH];

      if(_name && _comment)
        {

          int err=unzGetCurrentFileInfo(filehdl,&info,_name,_MAXPATH,NULL,0,_comment,_MAXSTR);
          if(err==UNZ_OK)
            {
              name           = _name;
              comment        = _comment;

              method         = info.compression_method;

              uncompresssize = info.uncompressed_size;
              compresssize   = info.compressed_size;

              CRC32          = info.crc;

              on = true;
            }
        }

      delete [] _name;
      delete [] _comment;

    } else  on = true;


}


/*-------------------------------------------------------------------
//  XFILECMPZIP::~XFILECMPZIP
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/08/2009 06:58:09 p.m.
//
//  @return       virtual :
//  */
/*-----------------------------------------------------------------*/
XFILECMPZIP::~XFILECMPZIP()
{
  if(open) Close();

  Clean();
}


/*-------------------------------------------------------------------
//  XFILECMPZIP::Open
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/08/2009 06:58:44 p.m.
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XFILECMPZIP::Open(XSTRING& password)
{
  int err;

  if(!on) return false;

  if(!zipoper)
    {
       if(!SelectCurrenFile()) return false;

       if(!password.IsEmpty())
         {
           XSTRING_CREATEOEM(password, charstr)
           err = unzOpenCurrentFilePassword((unzFile)filehdl, charstr);
           XSTRING_DELETEOEM(charstr)
         }
        else err = unzOpenCurrentFile((unzFile)filehdl);

       if(err!=UNZ_OK) return false;
    }
   else return false;

  open = true;

  return open;
}


/*-------------------------------------------------------------------
//  XFILECMPZIP::Create
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/02/2010 06:06:29 p.m.
//
//  @return       bool :
//  @param        crcfile :
//  @param        zfinfo :
//  @param        password :
*/
/*-----------------------------------------------------------------*/
bool XFILECMPZIP::Create(XDWORD crcfile,zip_fileinfo* zfinfo,XCHAR* password)
{
  int err;
  int opt_compress_level = Z_DEFAULT_COMPRESSION;

  if(!on) return false;

  XSTRING _password(password);


  XSTRING_CREATEOEM(name, namestr)
  XSTRING_CREATEOEM(_password, passwordstr)

  err = zipOpenNewFileInZip3(filehdl, namestr
                                    , zfinfo
                                    , NULL,0,NULL,0,NULL /* comment*/
                                    ,(opt_compress_level != 0) ? Z_DEFLATED : 0
                                    , opt_compress_level,0
                                    , -MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY
                                    , password?passwordstr:NULL
                                    , crcfile);

  XSTRING_DELETEOEM(passwordstr)
  XSTRING_DELETEOEM(namestr)

  if(err!=ZIP_OK) return false;

  return true;
}



/*-------------------------------------------------------------------
//  XFILECMPZIP::IsActive
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/08/2009 07:32:06 p.m.
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XFILECMPZIP::IsActive()
{
  return on;
}


/*-------------------------------------------------------------------
//  XFILECMPZIP::GetName
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/08/2009 08:09:26 p.m.
//
//  @return       XCHAR* :
//  */
/*-----------------------------------------------------------------*/
XCHAR* XFILECMPZIP::GetName()
{
  if(!on) return NULL;

  return name.Get();
}



/*-------------------------------------------------------------------
//  XFILECMPZIP::SetName
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/08/2009 08:09:50 p.m.
//
//  @return       void :
//  @param        name :
*/
/*-----------------------------------------------------------------*/
void XFILECMPZIP::SetName(XSTRING& name)
{
  if(name.IsEmpty()) return;

  this->name = name;
}




/*-------------------------------------------------------------------
//  XFILECMPZIP::IsDirectory
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      10/07/2015 13:35:37
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool XFILECMPZIP::IsDirectory()
{
  XPATH xpath;

  xpath = name.Get();

  if((xpath.Slash_HaveAtLast()) && (!Size()) && (!UnCompressSize())) return true;

  return false;
}




/*-------------------------------------------------------------------
//  XFILECMPZIP::Size
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/08/2009 08:10:06 p.m.
//
//  @return       int :
*/
/*-----------------------------------------------------------------*/
int XFILECMPZIP::Size()
{
  if(!on) return 0;

  return compresssize;
}


/*-------------------------------------------------------------------
//  XFILECMPZIP::UnCompressSize
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/08/2009 08:10:06 p.m.
//
//  @return       int :
*/
/*-----------------------------------------------------------------*/
int XFILECMPZIP::UnCompressSize()
{
  if(!on) return 0;

  return uncompresssize;
}



/*-------------------------------------------------------------------
//  XFILECMPZIP::GetCRC32
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      10/07/2015 14:30:13
//
//  @return       XDWORD :
//
*/
/*-----------------------------------------------------------------*/
XDWORD XFILECMPZIP::GetCRC32()
{
  if(!on) return 0;

  return CRC32;
}



/*-------------------------------------------------------------------
//  XFILECMPZIP::GetSeek
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/08/2009 08:11:12 p.m.
//
//  @return       bool :
//  @param        *position :
*/
/*-----------------------------------------------------------------*/
bool XFILECMPZIP::GetSeek(int *position)
{
  if(!on) return false;

  if(!zipoper)
    {
      if(!open) return false;

      (*position)=unztell(filehdl);

    } else return false;

  return true;
}


/*-------------------------------------------------------------------
//  XFILECMPZIP::Read
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/08/2009 08:11:45 p.m.
//
//  @return       bool :
//  @param        buffer :
//  @param        size :
*/
/*-----------------------------------------------------------------*/
bool XFILECMPZIP::Read(XBYTE* buffer,XDWORD size)
{
  if(!on)     return false;
  if(zipoper) return false;

  if(unzReadCurrentFile(filehdl,buffer,size)!=size) return false;

  return true;
}



/*-------------------------------------------------------------------
//  XFILECMPZIP::Read
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/08/2009 08:12:25 p.m.
//
//  @return       bool :
//  @param        buffer :
//  @param        size :
*/
/*-----------------------------------------------------------------*/
bool XFILECMPZIP::Read(XBYTE* buffer,int* size)
{
  int  _size;
  bool status = true;

  if(!on)     return false;
  if(zipoper) return false;

  _size=unzReadCurrentFile(filehdl,buffer,(*size));
  if(_size!=(*size))  status=false;
  (*size)=_size;

  return status;
}



/*-------------------------------------------------------------------
//  XFILECMPZIP::Write
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/08/2009 08:12:44 p.m.
//
//  @return       bool :
//  @param        buffer :
//  @param        size :
*/
/*-----------------------------------------------------------------*/
bool XFILECMPZIP::Write(XBYTE* buffer,XDWORD size)
{
  if(!on)      return false;
  if(!zipoper) return false;

  if(zipWriteInFileInZip(filehdl,buffer,size)!=ZIP_OK) return false;

  return true;
}



/*-------------------------------------------------------------------
//  XFILECMPZIP::IsEOF
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/08/2009 08:14:44 p.m.
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XFILECMPZIP::IsEOF()
{
  if(zipoper) return false;

  if(unzeof(filehdl)) return true;

  return false;
}



/*-------------------------------------------------------------------
//  XFILECMPZIP::Close
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/08/2009 06:59:02 p.m.
//
//  @return       bool :
//  @param        void :
*/
/*-----------------------------------------------------------------*/
bool XFILECMPZIP::Close()
{
  int err;

  if(!on) return false;

  if(!zipoper)
    {
       if(!SelectCurrenFile()) return false;

       err=unzCloseCurrentFile(filehdl);
       if(err!=UNZ_OK) return false;
    }
   else
    {
      err=zipCloseFileInZip(filehdl);
      if(err!=ZIP_OK) return false;
    }

  open =false;

  return true;
}



/*-------------------------------------------------------------------
//  XFILECMPZIP::Clean
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/08/2009 06:59:23 p.m.
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void XFILECMPZIP::Clean()
{
  filehdl         = NULL;

  on              = false;
  zipoper         = false;

  method          = 0;

  uncompresssize  = 0;
  compresssize    = 0;

  CRC32           = 0;

  open            = false;
}




/*-------------------------------------------------------------------
//  XFILECMPZIP::SelectCurrenFile
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/08/2009 07:53:52 p.m.
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XFILECMPZIP::SelectCurrenFile()
{
  int err;

  XSTRING_CREATEOEM(name, charstr)

  err=unzLocateFile((unzFile)filehdl, charstr, 0);
  if(err!=UNZ_OK) return false;

  XSTRING_DELETEOEM(charstr)

  return true;
}



//---------------------------------------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------------------------------------


/*-------------------------------------------------------------------
//  XFILEZIP::XFILEZIP
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/08/2009 07:01:15 p.m.
//
//  @return

*/
/*-----------------------------------------------------------------*/
XFILEZIP::XFILEZIP()
{
  Clean();

  xdatetime = xfactory->CreateDateTime();
}




/*-------------------------------------------------------------------
//  XFILEZIP::~XFILEZIP
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/08/2009 07:01:11 p.m.
//
//  @return
//  */
/*-----------------------------------------------------------------*/
XFILEZIP::~XFILEZIP()
{
  Close();

  xfactory->DeleteDateTime(xdatetime);

  Clean();
}


/*-------------------------------------------------------------------
//  XFILEZIP::Open
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/08/2009 07:01:38 p.m.
//
//  @return       bool :
//  @param        xpath :
*/
/*-----------------------------------------------------------------*/
bool XFILEZIP::Open(XPATH& xpath)
{
  XSTRING_CREATEOEM(xpath, charstr)
  filehdl=zipOpen(charstr, APPEND_STATUS_ADDINZIP);
  if(!filehdl)  filehdl=zipOpen(charstr, APPEND_STATUS_CREATE);
  XSTRING_DELETEOEM(charstr)

  if(!filehdl) return false;

  xpathzip = xpath;

  return true;
}




/*-------------------------------------------------------------------
//  XFILEZIP::CreateFileCmp
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/08/2009 09:11:06 p.m.
//
//  @return       XFILECMPZIP* :
//  @param        xpath :
*/
/*-----------------------------------------------------------------*/
XFILECMPZIP* XFILEZIP::CreateFileCmp(XPATH& xpath)
{
  XFILECMPZIP* filecmp = new XFILECMPZIP(true,filehdl);
  if(filecmp) filecmp->SetName(xpath);

  return filecmp;
}



/*-------------------------------------------------------------------
//  XFILEZIP::GetFileCRC
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/02/2010 12:41:28
//
//  @return       XDWORD :
//  @param        XFILE*  xfile :
*/
/*----------------------------------------------------------------*/
XDWORD XFILEZIP::GetFileCRC(XFILE*  xfile)
{
  if(!xfile)           return 0;
  if(!xfile->IsOpen()) return 0;

  int     blocksize = 1024;
  XBYTE*  buffer    = new XBYTE[blocksize];
  XDWORD  size;
  XDWORD  crcfile       = 0;

  if(!buffer) return 0;

  XDWORD position = 0;

  xfile->GetPosition(position);

  do{ size=blocksize;

      xfile->Read(buffer,&size);
      if(size) crcfile = crc32(crcfile,buffer,size);

    } while(size==blocksize);

  delete [] buffer;

  xfile->SetPosition(position);

  return crcfile;
}



/*-------------------------------------------------------------------
//  XFILEZIP::AddFile
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/02/2010 08:29:02 p.m.
//
//  @return       bool :
//  @param        pathname :
//  @param        newpathname :
//  @param        password :
*/
/*-----------------------------------------------------------------*/
bool XFILEZIP::AddFile(XPATH& xpath,XPATH& xpathnew,XCHAR* password)
{
  XFILECMPZIP* filecmp;
  XFILE*       xfile;
  int          blocksize = 1024;
  bool         status    = false;

  xfile = xfactory->Create_File();
  if(!xfile) return status;

  if(xfile->Open(xpath))
    {
      filecmp = CreateFileCmp((xpathnew.IsEmpty()?xpath:xpathnew));
      if(filecmp)
        {
          zip_fileinfo   zfinfo;
          XDWORD         crcfile = GetFileCRC(xfile);

          memset(&zfinfo,0,sizeof(zip_fileinfo));

          if(xdatetime) xdatetime->GetFileDateTime(xpath,&zfinfo.tmz_date,(XDWORD*)&zfinfo.dosDate);

          if(filecmp->Create(crcfile,&zfinfo,password))
            {
              XBYTE* buffer = new XBYTE[blocksize];
              XDWORD size;

              if(buffer)
                {
                  do{ size=blocksize;

                      xfile->Read(buffer,&size);

                      if(size)
                        {
                          status = filecmp->Write(buffer,size);
                          if(!status) break;
                        }

                    } while(size==blocksize);
                }

              delete [] buffer;

              filecmp->Close();
            }

          delete filecmp;
        }

      xfile->Close();
    }

  xfactory->Delete_File(xfile);

  return status;
}



/*-------------------------------------------------------------------
//  XFILEZIP::Close
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/08/2009 07:02:03 p.m.
//
//  @return       bool :
//  @param        void :
*/
/*-----------------------------------------------------------------*/
bool XFILEZIP::Close()
{
  if(!filehdl) return false;

  if(zipClose(filehdl,NULL)!=ZIP_OK) return false;

  filehdl = NULL;

  return true;
}



/*-------------------------------------------------------------------
//  XFILEZIP::Clean
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/08/2009 07:02:32 p.m.
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void XFILEZIP::Clean()
{
  xdatetime = NULL;

  filehdl   = NULL;
}


//---------------------------------------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------------------------------------


/*-------------------------------------------------------------------
//  XFILEUNZIP::XFILEUNZIP
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/08/2009 07:04:10 p.m.
//
//  @return

*/
/*-----------------------------------------------------------------*/
XFILEUNZIP::XFILEUNZIP()
{
  Clean();
}


/*-------------------------------------------------------------------
//  XFILEUNZIP::~XFILEUNZIP
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/08/2009 07:05:32 p.m.
//
//  @return
//  */
/*-----------------------------------------------------------------*/
XFILEUNZIP::~XFILEUNZIP()
{
  Close();

  Clean();
}



/*-------------------------------------------------------------------
//  XFILEUNZIP::Open
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/08/2009 07:06:02 p.m.
//
//  @return       bool :
//  @param        pathname :
*/
/*-----------------------------------------------------------------*/
bool XFILEUNZIP::Open(XPATH& xpath)
{
  if(xpath.IsEmpty()) return false;

  XSTRING_CREATEOEM(xpath, charstr)
  filehdl=unzOpen(charstr);
  XSTRING_DELETEOEM(charstr)

  if(!filehdl) return false;

  xpathunzip = xpath;

  return true;
}




/*-------------------------------------------------------------------
//  XFILEUNZIP::GetNFiles
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      06/05/2014 17:29:45
//
//  @return       int :
//
*/
/*-----------------------------------------------------------------*/
int XFILEUNZIP::GetNFiles()
{
  int nfiles = 0;

  while(1)
    {
      XFILECMPZIP* filecmp = GotoFile(nfiles);
      if(!filecmp) break;

      nfiles++;

      delete filecmp;
    }

  return nfiles;
}



/*-------------------------------------------------------------------
//  XFILEUNZIP::GotoFile
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/08/2009 07:21:09 p.m.
//
//  @return       XFILECMPZIP* :
//  @param        first :
*/
/*-----------------------------------------------------------------*/
XFILECMPZIP* XFILEUNZIP::GotoFile(bool first)
{
  int err;

  err =first?unzGoToFirstFile(filehdl):unzGoToNextFile(filehdl);
  if(err!=UNZ_OK) return NULL;

  return CreateCurrentFile();
}



/*-------------------------------------------------------------------
//  XFILEUNZIP::GotoFile
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/08/2009 07:22:26 p.m.
//
//  @return       XFILECMPZIP* :
//  @param        xpath :
*/
/*-----------------------------------------------------------------*/
XFILECMPZIP* XFILEUNZIP::GotoFile(XPATH& xpath)
{
  int err;

  XSTRING_CREATEOEM(xpath, charstr)
  err=unzLocateFile(filehdl, charstr, 0);
  XSTRING_DELETEOEM(charstr)

  if(err!=UNZ_OK) return NULL;

  return CreateCurrentFile();
}




/*-------------------------------------------------------------------
//  XFILEUNZIP::GotoFile
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      19/03/2012 12:40:39
//
//  @return       XFILECMPZIP* :
//  @param        index :
*/
/*-----------------------------------------------------------------*/
XFILECMPZIP* XFILEUNZIP::GotoFile(int index)
{
  XFILECMPZIP* filecmp = GotoFile(true);

  int c=0;
  do{
      if(index==c) break;

      delete filecmp;

      c++;
      filecmp = GotoFile(false);

    } while(filecmp);

  return filecmp;
}




/*-------------------------------------------------------------------
//  XFILEUNZIP::DelFile
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      06/05/2014 16:19:31
//
//  @return       bool :
//
//  @param        index :
//  @param        password :
*/
/*-----------------------------------------------------------------*/
bool XFILEUNZIP::DelFile(int index, XCHAR* password)
{
  XPATH     xpathorigin;
  XPATH     xpathtarget;
  bool      status = true;

  XFILE* xfile = xfactory->Create_File();
  if(!xfile) return false;

  XFILEZIP* filezip  = new XFILEZIP();
  if(!filezip)
    {
      xfactory->Delete_File(xfile);
      return false;
    }

  xpathorigin  = xpathunzip;
  xpathorigin.SetOnlyDriveAndPath();

  xpathtarget = xpathunzip;
  xpathtarget.AddToNameFile(true, __L("#TEMPO#_"));

  if(!filezip->Open(xpathtarget))
    {
      delete filezip;
      xfactory->Delete_File(xfile);

      return false;
    }

  int _index =0;

  while(1)
    {
      if(index != _index)
        {
          XPATH xpathuncmpfile;
          XPATH cmpfilename;

          XFILECMPZIP* filecmp = GotoFile(_index);
          if(!filecmp) break;

          cmpfilename = filecmp->GetName();
          delete filecmp;
          filecmp = NULL;

          xpathuncmpfile = cmpfilename;
          xpathuncmpfile.AddToNameFile(true, __L("#TEMPO#_"));
          xpathuncmpfile.DeleteCharacter(__C('/'));

          if(DecompressFile(_index , xpathorigin, xpathuncmpfile.Get(), password))
            {
              XPATH xpathtozip;

              xpathtozip = xpathorigin;
              xpathtozip.Slash_Add();
              xpathtozip += xpathuncmpfile;

              if(filezip->AddFile(xpathtozip, cmpfilename, password))
                {
                  if(xfile) xfile->Erase(xpathtozip);
                }
               else
                {
                  status = false;
                  break;
                }
            }
           else
            {
              status = false;
              break;
            }
        }

      _index++;
    }

  filezip->Close();

  delete filezip;

  Close();

  if(status)
    {
      xpathorigin  = xpathunzip;
      if(xfile)
        {
          status = xfile->Erase(xpathorigin);
          if(status) status = xfile->Rename(xpathtarget, xpathorigin);
        }
    }

  if(xfile) xfactory->Delete_File(xfile);

  xpathorigin  = xpathunzip;
  return Open(xpathorigin);

}




/*-------------------------------------------------------------------
//  XFILEUNZIP::DecompressFile
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/04/2011 10:59:45
//
//  @return       bool :
//  @param         :
//  @param        xpathtarget :
//  @param         :
//  @param        password :
*/
/*-----------------------------------------------------------------*/
bool XFILEUNZIP::DecompressFile(int sourceindex ,XPATH& xpathtarget, XCHAR* nametarget, XCHAR* password)
{
  XFILECMPZIP* filecmp = GotoFile(true);

  int c=0;
  do{ if(sourceindex==c) break;

      delete filecmp;

      c++;
      filecmp = GotoFile(false);

    } while(filecmp);

  if(!filecmp) return false;

  bool status = DecompressFile(filecmp,xpathtarget,nametarget,password);

  delete filecmp;

  return status;
}



/*-------------------------------------------------------------------
//  XFILEUNZIP::DecompressFile
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/02/2010 09:55:30 p.m.
//
//  @return       bool :
//  @param        sourcename :
//  @param        targetpath :
//  @param        targetname :
//  @param        password :
*/
/*-----------------------------------------------------------------*/
bool XFILEUNZIP::DecompressFile(XSTRING& namesource,XPATH& xpathtarget,XCHAR* nametarget, XCHAR* password)
{
  XPATH xpath(namesource);

  XFILECMPZIP* filecmp = GotoFile(xpath);
  if(!filecmp) return false;

  bool status = DecompressFile(filecmp,xpathtarget,nametarget,password);

  delete filecmp;

  return status;
}



/*-------------------------------------------------------------------
//  XFILEUNZIP::DecompressFile
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/04/2011 11:03:13
//
//  @return       bool :
//  @param        filecmp :
//  @param        xpathtarget :
//  @param        nametarget :
//  @param        password :
*/
/*-----------------------------------------------------------------*/
bool XFILEUNZIP::DecompressFile(XFILECMPZIP* filecmp,XPATH& xpathtarget,XCHAR* nametarget,XCHAR* password)
{
  bool status = false;

  if(!filecmp) return false;

  XPATH xpath;

  xpath = xpathtarget;

  if(!nametarget)
    {
      xpath += filecmp->GetName();
    }
   else
    {
      if(!nametarget[0])
        {
          xpath += filecmp->GetName();

        } else xpath += nametarget;
    }

  XSTRING _password(password);

  if(filecmp->Open(_password))
    {
      XFILE* xfile = xfactory->Create_File();
      if(xfile)
        {
          if(xfile->Create(xpath))
            {
              XBYTE* buffer= new XBYTE[XFILEZIP_MAXBLOCKFILE];
              if(buffer)
                {
                  int bytesread;

                  do { bytesread = XFILEZIP_MAXBLOCKFILE;
                       memset(buffer,0,XFILEZIP_MAXBLOCKFILE);

                       filecmp->Read(buffer,&bytesread);
                       if(bytesread) xfile->Write(buffer,bytesread);

                     } while(bytesread==XFILEZIP_MAXBLOCKFILE);

                  delete [] buffer;
                }

              if(filecmp->UnCompressSize()==xfile->GetSize()) status = true;

              xfile->Close();
            }

          delete xfile;

        }

      filecmp->Close();
    }

  return status;
}




/*-------------------------------------------------------------------
//  XFILEUNZIP::Close
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/08/2009 07:06:17 p.m.
//
//  @return       bool :
//  @param        void :
*/
/*-----------------------------------------------------------------*/
bool XFILEUNZIP::Close()
{
  if(!filehdl) return false;

  unzClose(filehdl);
  filehdl = NULL;

  return true;
}



/*-------------------------------------------------------------------
//  XFILEUNZIP::Clean
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/08/2009 07:06:47 p.m.
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void XFILEUNZIP::Clean()
{
  filehdl  = NULL;
}



/*-------------------------------------------------------------------
//  XFILEUNZIP::CreateCurrentFile
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/08/2009 07:26:56 p.m.
//
//  @return       XFILECMPZIP* :
//  */
/*-----------------------------------------------------------------*/
XFILECMPZIP* XFILEUNZIP::CreateCurrentFile()
{
  XFILECMPZIP* filecmp;

  if(!filehdl) return NULL;

  filecmp = new XFILECMPZIP(false,(void*)filehdl);
  if(!filecmp) return NULL;

  if(!filecmp->IsActive()) return NULL;

  return filecmp;
}


