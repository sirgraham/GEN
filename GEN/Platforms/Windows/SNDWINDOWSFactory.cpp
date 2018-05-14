
/*------------------------------------------------------------------------------------------
//  SNDWINDOWSFACTORY.CPP
//
//  windows sound factory
//
//  Author            : Imanol Celaya Ruiz de Alegria
//  Date Of Creation  : 11/11/2015 10:52:15
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/


#include "SNDOpenAL.h"
#include "SNDWINDOWSResampler.h"
#include "SNDFileFFMPEG.h"

#include "SNDWINDOWSFactory.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/




/*-------------------------------------------------------------------
//  SNDWINDOWSFACTORY::SNDWINDOWSFACTORY
*/
/**
//
//  Class Constructor SNDWINDOWSFACTORY
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      11/11/2015 10:57:23
//


//  @param        SNDFACTORY(xfactory :


*/
/*-----------------------------------------------------------------*/
SNDWINDOWSFACTORY::SNDWINDOWSFACTORY() : SNDFACTORY()
{
  Clean();
  sndopenal = new SNDOPENAL(this);
}






/*-------------------------------------------------------------------
//  SNDWINDOWSFACTORY::~SNDWINDOWSFACTORY
*/
/**
//
//   Class Destructor SNDWINDOWSFACTORY
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      11/11/2015 10:57:28
//
*/
/*-----------------------------------------------------------------*/
SNDWINDOWSFACTORY::~SNDWINDOWSFACTORY()
{
  delete sndopenal;
  Clean();
}




/*-------------------------------------------------------------------
//   SNDWINDOWSFACTORY::AddFile
*/
/**
//
//
//  @author   Abraham J. Velez
//  @version  21/02/2018 17:21:48
//
//  @return   SNDELEMENT* :
//
//  @param    XPATH& :
//  @param    XSTRING* :
//  @param    bool :
//
*//*-----------------------------------------------------------------*/
SNDELEMENT* SNDWINDOWSFACTORY::AddFile(XPATH& xpath, XSTRING* namefile, bool stream)
{
  return sndopenal->AddFile(xpath, namefile, stream);
}




/*-------------------------------------------------------------------
//   SNDWINDOWSFACTORY::AddFile
*/
/**
//
//
//  @author   Abraham J. Velez
//  @version  21/02/2018 17:22:30
//
//  @return   SNDELEMENT* :
//
//  @param    XPATH& :
//  @param    XCHAR* :
//  @param    bool :
//
*//*-----------------------------------------------------------------*/
SNDELEMENT* SNDWINDOWSFACTORY::AddFile(XPATH& xpath, XCHAR* namefile, bool stream)
{
  return sndopenal->AddFile(xpath, namefile, stream);
}





/*-------------------------------------------------------------------
//  SNDWINDOWSFACTORY::GetFile
*/
/**
//
//  gets an already loaded file
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      11/11/2015 10:58:08
//
//  @return       SNDELEMENT* :
//
//  @param        file :
*/
/*-----------------------------------------------------------------*/
SNDELEMENT* SNDWINDOWSFACTORY::GetFile(XSTRING* namefile, bool stream)
{
  return sndopenal->GetFile(namefile, stream);
}





/*-------------------------------------------------------------------
//  SNDWINDOWSFACTORY::GetFile
*/
/**
//
//  gets an already loaded file
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      11/11/2015 10:58:18
//
//  @return       SNDELEMENT* :
//
//  @param        file :
*/
/*-----------------------------------------------------------------*/
SNDELEMENT* SNDWINDOWSFACTORY::GetFile(XCHAR* namefile, bool stream)
{
  return sndopenal->GetFile(namefile, stream);
}



/*-------------------------------------------------------------------
//   SNDWINDOWSFACTORY::RemoveFile
*/
/**
//
//
//
//  @author   Imanol Celaya Ruiz de Alegria
//  @version

//  @return   bool :
//
//  @param    SNDELEMENT* :
//
*//*-----------------------------------------------------------------*/
bool SNDWINDOWSFACTORY::RemoveFile(SNDELEMENT* element)
{
  return sndopenal->RemoveFile(element);
}


/*-------------------------------------------------------------------
//  SNDWINDOWSFACTORY::GetStreamer
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      20/07/2016 11:54:59
//
//  @return       SNDSTREAMELEMENT* :
//
*/
/*-----------------------------------------------------------------*/
SNDSTREAMELEMENT* SNDWINDOWSFACTORY::GetStreamer()
{
  return sndopenal->GetStreamer();
}



/*-------------------------------------------------------------------
//  SNDWINDOWSFACTORY::PlaySound
*/
/**
//
//  plays a sund
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      11/11/2015 10:58:27
//
//  @return       SNDSOURCE* :
//
//  @param        element :
*/
/*-----------------------------------------------------------------*/
SNDINSTANCE* SNDWINDOWSFACTORY::PlaySound(SNDELEMENT* element)
{
  return sndopenal->PlaySound(element);
}



/*-------------------------------------------------------------------
//  SNDWINDOWSFACTORY::StopSound
*/
/**
//
//  stops a sound
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      11/11/2015 10:58:38
//
//  @param        element :
*/
/*-----------------------------------------------------------------*/
void SNDWINDOWSFACTORY::StopSound(SNDELEMENT* element)
{
  sndopenal->StopSound(element);
}





/*-------------------------------------------------------------------
//  SNDWINDOWSFACTORY::PauseSound
*/
/**
//
//  pauses a sound
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      11/11/2015 10:58:47
//
//  @return       SNDSOURCE* :
//
//  @param        element :
*/
/*-----------------------------------------------------------------*/
SNDINSTANCE* SNDWINDOWSFACTORY::PauseSound(SNDELEMENT* element)
{
  return sndopenal->PauseSound(element);
}




/*-------------------------------------------------------------------
//  SNDWINDOWSFACTORY::Update
*/
/**
//
//  system update
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      11/11/2015 10:58:55
//
*/
/*-----------------------------------------------------------------*/
void SNDWINDOWSFACTORY::ImpUpdate()
{
  sndopenal->Update();
}





/*-------------------------------------------------------------------
//  SNDWINDOWSFACTORY::IsAnyPlaying
*/
/**
//
//  checks if any sound is playing
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      11/11/2015 10:59:06
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool SNDWINDOWSFACTORY::IsAnyPlaying()
{
  return sndopenal->IsAnyPlaying();
}




/*-------------------------------------------------------------------
//  SNDWINDOWSFACTORY::StopAll
*/
/**
//
//  stops all sounds
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      11/11/2015 10:59:15
//
*/
/*-----------------------------------------------------------------*/
void SNDWINDOWSFACTORY::StopAll()
{
  sndopenal->StopAll();
}




/*-------------------------------------------------------------------
//  SNDWINDOWSFACTORY::SetMasterVolume
*/
/**
//
//  sets the master volume
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      11/11/2015 10:59:25
//
//  @param        mastervolume :
*/
/*-----------------------------------------------------------------*/
void SNDWINDOWSFACTORY::SetMasterVolume(float mastervolume)
{
  sndopenal->SetMasterVolume(mastervolume);
}




/*-------------------------------------------------------------------
//  SNDWINDOWSFACTORY::GetMasterVolume
*/
/**
//
//  gets the master volume
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      11/11/2015 10:59:33
//
//  @return       float :
//
*/
/*-----------------------------------------------------------------*/
float SNDWINDOWSFACTORY::GetMasterVolume()
{
  return sndopenal->GetMasterVolume();
}



/*-------------------------------------------------------------------
//  SNDWINDOWSFACTORY::GetResampler
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      12/01/2017 12:12:24
//
//  @return       SNDRESAMPLER* :
//
*/
/*-----------------------------------------------------------------*/
SNDRESAMPLER* SNDWINDOWSFACTORY::GetResampler()
{
  return new SNDWINDOWSRESAMPLER();
}



/*-------------------------------------------------------------------
//  SNDWINDOWSFACTORY::CreateFile
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      16/03/2017 16:35:04
//
//  @return       SNDFILE* :
//
*/
/*-----------------------------------------------------------------*/
SNDFILE* SNDWINDOWSFACTORY::CreateSNDFile()
{
  return new SNDFILEFFMPEG();
}


/*-------------------------------------------------------------------
//  SNDWINDOWSFACTORY::DeleteFile
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      16/03/2017 16:35:10
//
//  @param        file :
*/
/*-----------------------------------------------------------------*/
void SNDWINDOWSFACTORY::DeleteSNDFile(SNDFILE* file)
{
  delete file;
}
