
/*------------------------------------------------------------------------------------------
//  SNDMANAGER.CPP
//
//  Manages sounds and playlists
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 15/06/2017 11:19:00
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XDebug.h"

#include "SNDFactory.h"
#include "SNDElement.h"

#include "SNDManager.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


SNDMANAGER* SNDMANAGER::instance=NULL;


/*-------------------------------------------------------------------
//  SNDMANAGER::SNDMANAGER
*/
/**
//
//  constructor
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  20/12/2017 10:40:42
//  @return    :
//
//  @param     :
//
*//*-----------------------------------------------------------------*/
SNDMANAGER::SNDMANAGER()
{
  // XDEBUG_PRINTCOLOR(0, __L("Creating Sound Manager"));
}

/*-------------------------------------------------------------------
//  SNDMANAGER::~SNDMANAGER
*/
/**
//
//   Class Destructor SNDMANAGER
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      12/05/2017 17:18:54
//
*/
/*-----------------------------------------------------------------*/
SNDMANAGER::~SNDMANAGER()
{
  playlists.DeleteKeyContents     ();
  playlists.DeleteElementContents ();
  playlists.DeleteAll             ();
}



/*-------------------------------------------------------------------
//  SNDANIMATIONMANAGER::GetSample
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      12/05/2017 11:59:29
//
//  @return       SNDELEMENT* :
//
//  @param        name :
//  @param        playlist :
*/
/*-----------------------------------------------------------------*/
SNDELEMENT* SNDMANAGER::GetSample(XSTRING& name,  XSTRING& playlist)
{
  for(XDWORD e=0;e<this->playlists.GetSize();e++)
    {
      XSTRING* playlist_name=playlists.GetKey(e);
      if(playlist_name->Compare(playlist.Get())==0)
        {
          XVECTOR<SNDELEMENT*>* playlist_samplelist=playlists.GetElement(e);
          for(XDWORD i=0;i<playlist_samplelist->GetSize();i++)
            {
              SNDELEMENT* smp=playlist_samplelist->Get(i);
              if(smp)
                {
                  if(smp->GetNameFile()->Compare(name.Get())==0)
                    {
                      return smp;
                    }
                }
            }

          return NULL;
        }
    }

  return NULL;
}



/*-------------------------------------------------------------------
//  SNDMANAGER::Destroy
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      12/05/2017 17:17:17
//
*/
/*-----------------------------------------------------------------*/
void SNDMANAGER::Destroy()
{
  if(instance) delete(instance);
  instance = NULL;
}


/*-------------------------------------------------------------------
//  SNDMANAGER::Get
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      12/05/2017 17:17:13
//
//  @return       SNDMANAGER* :
//
*/
/*-----------------------------------------------------------------*/
SNDMANAGER* SNDMANAGER::Get()
{
  if (instance==NULL) instance=new SNDMANAGER();
  return instance;
}


/*-------------------------------------------------------------------
//  SNDMANAGER::GetPlaylist
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      12/05/2017 17:14:53
//
//  @return       XVECTOR<SNDELEMENT*>* :
//
//  @param        name :
*/
/*-----------------------------------------------------------------*/
SNDPLAYLIST* SNDMANAGER::GetPlaylist(XCHAR* name)
{
  for (XDWORD e=0;e<playlists.GetSize();e++)
  {
    if (playlists.GetKey(e)->Compare(name)==0)
      return playlists.GetElement(e);
  }

  return NULL;
}




/*-------------------------------------------------------------------
//  SNDMANAGER::Add
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      12/05/2017 13:02:57
//
//  @param        samplename :
//  @param        playlist :
*/
/*-----------------------------------------------------------------*/
void SNDMANAGER::Add(XPATH& xpath, XSTRING samplename, XSTRING  playlist)
{
  for(XDWORD e=0;e<this->playlists.GetSize();e++)
    {
      if(playlists.GetKey(e)->Compare(playlist.Get())==0)
        {
          XVECTOR<SNDELEMENT*>* list=playlists.GetElement(e);

          for(XDWORD i=0;i<list->GetSize();i++)
            {
              if(list->Get(i)->GetNameFile()->Compare(samplename.Get())==0) //el sample ya esta en la lista
                {
                  return;
                }
            }

          SNDELEMENT* snd = SNDFACTORY::Get()->AddFile(xpath, samplename.Get());
          if(!snd)
            {
              XDEBUG_PRINTCOLOR(4,__L("SNDMANAGER::Add(%s,%s) : unable to add sound"),samplename.Get(),playlist.Get());
              return;
            }

          list->Add(snd);
          return;
        }
    }

  XSTRING* listname = new XSTRING();
  if(listname) listname->Set(playlist);

  XVECTOR<SNDELEMENT*>* vector = new XVECTOR<SNDELEMENT*>();
  if(playlists.Add(listname,vector))
    {
      Add(xpath, samplename, playlist);
    }
}



/*-------------------------------------------------------------------
//  SNDMANAGER::Play
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      12/05/2017 11:58:00
//
//  @param        samplename :
//  @param        playlist :
*/
/*-----------------------------------------------------------------*/
void  SNDMANAGER::Play(XSTRING samplename, XSTRING playlist, float volume)
{
  SNDELEMENT* snd = GetSample(samplename,playlist);
  if(snd)
    {
      snd->SetVolume(volume);
      if(SNDFACTORY::Get())  SNDFACTORY::Get()->PlaySound(snd);
    }
   else
    {
      XDEBUG_PRINTCOLOR(4,__L("SNDANIMATIONMANAGER::Play sonido desconocido"));
      XDEBUG_PRINTCOLOR(4,__L("%s::%s"),playlist.Get(),samplename.Get());
      return;
    }

}

/*-------------------------------------------------------------------
//  SNDMANAGER::Stop
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      12/05/2017 11:57:54
//
//  @param        samplename :
//  @param        playlist :
*/
/*-----------------------------------------------------------------*/
void  SNDMANAGER::Stop(XSTRING  samplename, XSTRING playlist)
{
        SNDELEMENT* snd=GetSample(samplename,playlist);
        if (snd)
        {
          if (SNDFACTORY::Get())
            SNDFACTORY::Get()->StopSound(snd);
        }
        else
        {
          XDEBUG_PRINTCOLOR(4,__L("SNDMANAGER::Stop sonido desconocido %s::%s"),playlist.Get(),samplename.Get());
          return;
        }
}