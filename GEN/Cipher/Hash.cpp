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
bool HASH::Do(XBYTE* input, XDWORD size)
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
  return Do(input.Get(), input.GetSize());
}





/**-------------------------------------------------------------------------------------------------------------------
* @fn         HASH::Do
* @brief      Hash do from file
* @ingroup    CIPHER
* @date       15/05/2018 19:31:43
* @param[in]  xpath : xpath from the file to hash
* @param[in]  size : size of the part of file 
* @param[in]  pos : position from file
* @return     bool : true if is succesfull
*---------------------------------------------------------------------------------------------------------------------*/
bool HASH::Do(XPATH& xpath, XDWORD size, XDWORD pos)
{
  XFILE* xfile;
  bool   status = false;

  xfile = xfactory->Create_File();
  if(!xfile) return false;

  ResetResult();

  if(xfile->Open(xpath, true))
    {
      XDWORD _size;

      if(size == -1) _size = xfile->GetSize(); else _size = size;

      status = Do(xfile, _size, pos);

      xfile->Close();
    }  

  if(xfile) xfactory->Delete_File(xfile);

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
bool HASH::Do(XFILE* xfile, XDWORD size, XDWORD pos)
{
  if(!xfile)           return false;
  if(!xfile->IsOpen()) return false;

  XBUFFER   xbuffer;
  XDWORD    block   = (((size==HASHALLFILESIZE)?xfile->GetSize():size) / HASHMAXFILESIZEBUFFER);
  XDWORD    mod     = (((size==HASHALLFILESIZE)?xfile->GetSize():size) % HASHMAXFILESIZEBUFFER);
  bool      status  = true;

  if(!xfile->SetPosition(pos)) return false;

  xbuffer.Resize((XDWORD)HASHMAXFILESIZEBUFFER);

  for(XDWORD c=0;c<block;c++)
    {
      if(xfile->Read(xbuffer.Get(), HASHMAXFILESIZEBUFFER))
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
XBYTE* HASH::GetResult(XDWORD& resultsize)
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
