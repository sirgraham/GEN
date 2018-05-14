
/*------------------------------------------------------------------------------------------
//  UIFILEBROWSER.CPP
//
//  UI File Browser
//
//  Author            : Imanol Celaya Ruiz de Alegria
//  Date Of Creation  : 27/01/2017 12:35:41
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "UIFileBrowser.h"
#include "XString.h"

#ifdef WINDOWS
#include <Windows.h>
#endif

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/
UIFILEBROWSER::UIFILEBROWSER(UIMANAGER* uimanager)
{
  Clean();

  this->uimanager = uimanager;
}

UIFILEBROWSER::~UIFILEBROWSER()
{
  Clean();
}

bool UIFILEBROWSER::SelectFile(XSTRING* str)
{

  // SHITTY HACK THAT DESERVES THE FIRES OF HELL (Sorry Abraham)
  // Imanol
#ifdef WINDOWS
  OPENFILENAME open;
  ZeroMemory(&open, sizeof(open));
  XCHAR* filename = new XCHAR[1024];
  open.lStructSize = sizeof ( open );
  open.hwndOwner = NULL  ;
  open.lpstrFile = filename ;
  open.lpstrFile[0] = '\0';
  open.nMaxFile = 1024;
  open.lpstrFilter = __L("Totem Media\0*.PNG;*.MP4\0All\0*.*\0");
  open.nFilterIndex =1;
  open.lpstrFileTitle = NULL ;
  open.nMaxFileTitle = 0 ;
  open.lpstrInitialDir=NULL ;
  open.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST|OFN_NOCHANGEDIR;

  GetOpenFileName(&open);

  str->Set(filename);
  delete filename;
#endif
  return true;
}
