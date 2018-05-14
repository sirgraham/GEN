/*------------------------------------------------------------------------------------------
//  HASH.CPP
//
//  Hash Generic Class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 02/03/2013 12:07:38
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XFactory.h"
#include "XFile.h"

#include "Hash.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/




/*-------------------------------------------------------------------
//  HASH::HASH
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      02/03/2013 12:43:10
//
//  @return

*/
/*-----------------------------------------------------------------*/
HASH::HASH()
{
  Clean();

  result = new XBUFFER();
}




/*-------------------------------------------------------------------
//  HASH::~HASH
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      02/03/2013 12:09:01
//
//  @return
//  */
/*-----------------------------------------------------------------*/
HASH::~HASH()
{
  delete result;

  Clean();
}




/*-------------------------------------------------------------------
//  HASH::Do
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/03/2013 16:23:56
//
//  @return       bool :
//  @param        input :
//  @param        size :
*/
/*-----------------------------------------------------------------*/
bool HASH::Do(XBYTE* input, int size)
{
  return false;
}



/*-------------------------------------------------------------------
//  HASH::Do
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/03/2013 16:24:03
//
//  @return       bool :
//  @param        input :
*/
/*-----------------------------------------------------------------*/
bool HASH::Do(XBUFFER& input)
{
  return Do(input.Get(), (int)input.GetSize());
}




/*-------------------------------------------------------------------
//  HASH::Do
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/03/2013 16:24:28
//
//  @return       bool :
//  @param        xpath :
//  @param        size :
//  @param        pos :
*/
/*-----------------------------------------------------------------*/
bool HASH::Do(XPATH& xpath, int size, int pos)
{
  XFILE* file;
  bool   status;

  file = xfactory->Create_File();
  if(!file) return false;

  ResetResult();

  if(file->Open(xpath,true))
    {
      status = Do(file,size,pos);

      file->Close();

    }  else status=false;

  if(file) xfactory->Delete_File(file);

  return status;
}




/*-------------------------------------------------------------------
//  HASH::Do
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/03/2013 16:24:46
//
//  @return       bool :
//  @param        xfile :
//  @param        size :
//  @param        pos :
*/
/*-----------------------------------------------------------------*/
bool HASH::Do(XFILE* xfile, int size, int pos)
{
  if(!xfile)           return false;
  if(!xfile->IsOpen()) return false;

  XBUFFER   xbuffer;
  int       block   = (((size==HASHALLFILESIZE)?xfile->GetSize():size) / HASHMAXFILESIZEBUFFER);
  int       mod     = (((size==HASHALLFILESIZE)?xfile->GetSize():size) % HASHMAXFILESIZEBUFFER);
  bool      status  = true;

  if(!xfile->SetPosition(pos)) return false;

  xbuffer.Resize((XDWORD)HASHMAXFILESIZEBUFFER);

  for(int c=0;c<block;c++)
    {
      if(xfile->Read(xbuffer.Get(),HASHMAXFILESIZEBUFFER))
        {
          if(!Do(xbuffer))
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

  if(status)
    {
      xbuffer.Resize(mod);
      if(xfile->Read(xbuffer.Get(),mod))
        {
          if(!Do(xbuffer))
            {
              status = false;
            }
        }
       else
        {
          status = false;
        }
    }

  return status;
}




/*-------------------------------------------------------------------
//  HASH::ResetResult
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      02/03/2013 12:35:35
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool HASH::ResetResult()
{
  if(!result) return false;

  result->Delete();

  return true;
}



/*-------------------------------------------------------------------
//  HASH::GetDefaultSize
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/04/2013 0:46:03
//
//  @return       int :
//  */
/*-----------------------------------------------------------------*/
int HASH::GetDefaultSize()
{
  return 0;
}



/*-------------------------------------------------------------------
//  HASH::GetResult
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      02/03/2013 12:35:39
//
//  @return       XBUFFER* :
//  */
/*-----------------------------------------------------------------*/
XBUFFER* HASH::GetResult()
{
  return result;
}




/*-------------------------------------------------------------------
//  HASH::GetResult
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      02/03/2013 12:38:50
//
//  @return       XBYTE* :
//  @param        resultsize :
*/
/*-----------------------------------------------------------------*/
XBYTE* HASH::GetResult(int& resultsize)
{
  resultsize = 0;

  if(!result) return NULL;

  resultsize  = result->GetSize();

  return result->Get();
}



/*-------------------------------------------------------------------
//  HASH::GetResult
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      02/03/2013 21:08:16
//
//  @return       bool :
//  @param        stringhex :
*/
/*-----------------------------------------------------------------*/
bool HASH::GetResultString(XSTRING& stringhex)
{
  stringhex.Empty();

  XBUFFER* xbuffer = GetResult();
  if(!xbuffer) return false;

  if(!xbuffer->GetSize()) return false;

  for(int c=0;c<(int)xbuffer->GetSize();c++)
    {
      XSTRING strbyte;

      strbyte.Format(__L("%02x"), xbuffer->GetByte(c));

      stringhex += strbyte;
    }

  return true;
}



/*-------------------------------------------------------------------
//  HASH::Clean
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      02/03/2013 12:09:56
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void HASH::Clean()
{
  type     = HASHTYPE_NONE;
  result   = NULL;
}
