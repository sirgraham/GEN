
/*------------------------------------------------------------------------------------------
//  SNDLINUXFACTORY.CPP
//
//  windows sound system
//
//  Author            : Imanol Celaya Ruiz de Alegria
//  Date Of Creation  : 11/11/2015 10:52:15
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "SNDLinuxFactory.h"
#include "SNDOpenAL.h"
#include "SNDLINUXResampler.h"
#include "SNDFileFFMPEG.h"

#include "XDebug.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/




/*-------------------------------------------------------------------
//  SNDLINUXFACTORY::SNDLINUXFACTORY
*/
/**
//
//  Class Constructor SNDLINUXFACTORY
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      11/11/2015 10:57:23
//


//  @param        SNDFACTORY(xfactory :


*/
/*-----------------------------------------------------------------*/
SNDLINUXFACTORY::SNDLINUXFACTORY() : SNDFACTORY()
{
  Clean();

  sndopenal = new SNDOPENAL(this);
}






/*-------------------------------------------------------------------
//  SNDLINUXFACTORY::~SNDLINUXFACTORY
*/
/**
//
//   Class Destructor SNDLINUXFACTORY
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      11/11/2015 10:57:28
//
*/
/*-----------------------------------------------------------------*/
SNDLINUXFACTORY::~SNDLINUXFACTORY()
{
  delete sndopenal;
  Clean();
}




/*-------------------------------------------------------------------
//   SNDLINUXFACTORY::AddFile
*/
/**
//
//
//  @author   Abraham J. Velez
//  @version  22/02/2018 9:29:51
//
//  @return   SNDELEMENT* :
//
//  @param    XPATH& :
//  @param    XSTRING* :
//  @param    bool :
//
*//*-----------------------------------------------------------------*/
SNDELEMENT* SNDLINUXFACTORY::AddFile(XPATH& xpath, XSTRING* namefile, bool stream)
{
  return sndopenal->AddFile(xpath, namefile, stream);
}





/*-------------------------------------------------------------------
//  SNDLINUXFACTORY::AddFile
*/
/**
//
//  loads a file into the system
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      11/11/2015 10:57:56
//
//  @return       SNDELEMENT* :
//
//  @param        namefile :
*/
/*-----------------------------------------------------------------*/
SNDELEMENT* SNDLINUXFACTORY::AddFile(XPATH& xpath, XCHAR* namefile, bool stream)
{
  return sndopenal->AddFile(xpath, namefile, stream);
}





/*-------------------------------------------------------------------
//  SNDLINUXFACTORY::GetFile
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
//  @param        namefile :
*/
/*-----------------------------------------------------------------*/
SNDELEMENT* SNDLINUXFACTORY::GetFile(XSTRING* namefile, bool stream)
{
  return sndopenal->GetFile(namefile, stream);
}





/*-------------------------------------------------------------------
//  SNDLINUXFACTORY::GetFile
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
//  @param        namefile :
*/
/*-----------------------------------------------------------------*/
SNDELEMENT* SNDLINUXFACTORY::GetFile(XCHAR* namefile, bool stream)
{
  return sndopenal->GetFile(namefile, stream);
}




/*-------------------------------------------------------------------
//   SNDLINUXFACTORY::RemoveFile
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
bool SNDLINUXFACTORY::RemoveFile(SNDELEMENT* element)
{
  return sndopenal->RemoveFile(element);
}


/*-------------------------------------------------------------------
//  SNDLINUXFACTORY::GetStreamer
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      20/07/2016 11:56:06
//
//  @return       SNDSTREAMELEMENT* :
//
*/
/*-----------------------------------------------------------------*/
SNDSTREAMELEMENT* SNDLINUXFACTORY::GetStreamer()
{
  return sndopenal->GetStreamer();
}



/*-------------------------------------------------------------------
//  SNDLINUXFACTORY::PlaySound
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
SNDINSTANCE* SNDLINUXFACTORY::PlaySound(SNDELEMENT* element)
{
  //XDEBUG_PRINTCOLOR(4, __L("Play audio file"));
  return sndopenal->PlaySound(element);
}



/*-------------------------------------------------------------------
//  SNDLINUXFACTORY::StopSound
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
void SNDLINUXFACTORY::StopSound(SNDELEMENT* element)
{
  sndopenal->StopSound(element);
}





/*-------------------------------------------------------------------
//  SNDLINUXFACTORY::PauseSound
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
SNDINSTANCE* SNDLINUXFACTORY::PauseSound(SNDELEMENT* element)
{
  return sndopenal->PauseSound(element);
}




/*-------------------------------------------------------------------
//  SNDLINUXFACTORY::Update
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
void SNDLINUXFACTORY::ImpUpdate()
{
  sndopenal->Update();
}





/*-------------------------------------------------------------------
//  SNDLINUXFACTORY::IsAnyPlaying
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
bool SNDLINUXFACTORY::IsAnyPlaying()
{
  return sndopenal->IsAnyPlaying();
}




/*-------------------------------------------------------------------
//  SNDLINUXFACTORY::StopAll
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
void SNDLINUXFACTORY::StopAll()
{
  sndopenal->StopAll();
}




/*-------------------------------------------------------------------
//  SNDLINUXFACTORY::SetMasterVolume
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
void SNDLINUXFACTORY::SetMasterVolume(float mastervolume)
{
  sndopenal->SetMasterVolume(mastervolume);
}




/*-------------------------------------------------------------------
//  SNDLINUXFACTORY::GetMasterVolume
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
float SNDLINUXFACTORY::GetMasterVolume()
{
  return sndopenal->GetMasterVolume();
}



/*-------------------------------------------------------------------
//  SNDLINUXFACTORY::GetResampler
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      25/11/2016 13:00:01
//
//  @return       SNDRESAMPLER* :
//
*/
/*-----------------------------------------------------------------*/
SNDRESAMPLER* SNDLINUXFACTORY::GetResampler()
{
  return new SNDLINUXRESAMPLER();
  //return new SNDDUMMYRESAMPLER();
}



/*-------------------------------------------------------------------
//  SNDLINUXFACTORY::CreateFile
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      16/03/2017 16:36:27
//
//  @return       SNDFILE* :
//
*/
/*-----------------------------------------------------------------*/
SNDFILE* SNDLINUXFACTORY::CreateSNDFile()
{
  return new SNDFILEFFMPEG();
}




/*-------------------------------------------------------------------
//  SNDLINUXFACTORY::DeleteFile
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      16/03/2017 16:36:34
//
//  @param        file :
*/
/*-----------------------------------------------------------------*/
void SNDLINUXFACTORY::DeleteSNDFile(SNDFILE* file)
{
  delete file;
}
