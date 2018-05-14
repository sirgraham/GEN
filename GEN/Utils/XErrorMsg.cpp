//------------------------------------------------------------------------------------------
//  XERRORMSG.CPP
//
//  MGERROR Class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 17/11/2001
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------




//---- INCLUDES ----------------------------------------------------------------------------

#include "XErrorMsg.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  XERRORMSG::XERRORMSG
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/11/2001 11:53:59
//
//  @return
//  */
//-------------------------------------------------------------------
XERRORMSG::XERRORMSG()
{
  Clean();
}


//-------------------------------------------------------------------
//  XERRORMSG::XERRORMSG
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/11/2001 11:54:28
//
//  @return
//  @param        code :
//  @param        title :
//  @param        msg :
*/
//-------------------------------------------------------------------
XERRORMSG::XERRORMSG(int code,XCHAR* title,XCHAR* msg)
{
  Clean();

  Set(code,title,msg);
}



//-------------------------------------------------------------------
//  XERRORMSG::~XERRORMSG
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/11/2001 11:55:15
//
//  @return
//  */
//-------------------------------------------------------------------
XERRORMSG::~XERRORMSG()
{
  Clean();
}


//-------------------------------------------------------------------
//  XERRORMSG::Get
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/11/2001 12:01:35
//
//  @return       XERRORMSG* :
//  */
//-------------------------------------------------------------------
XERRORMSG* XERRORMSG::Get()
{
  return this;
}


//-------------------------------------------------------------------
//  XERRORMSG::GetCode
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/11/2001 12:01:38
//
//  @return       int :
//  */
//-------------------------------------------------------------------
int XERRORMSG::GetCode()
{
  return code;
}

//-------------------------------------------------------------------
//  XERRORMSG::GetLevel
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/11/2001 12:01:40
//
//  @return       int :
//  */
//-------------------------------------------------------------------
int XERRORMSG::GetLevel()
{
  return level;
}


//-------------------------------------------------------------------
//  XERRORMSG::GetImage
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/11/2001 12:01:44
//
//  @return       int :
//  */
//-------------------------------------------------------------------
int XERRORMSG::GetImage()
{
  return image;
}


//-------------------------------------------------------------------
//  XERRORMSG::GetSound
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/11/2001 12:02:07
//
//  @return       int :
//  */
//-------------------------------------------------------------------
int XERRORMSG::GetSound()
{
  return sound;
}


/*-------------------------------------------------------------------
//  XERRORMSG::GetTitle
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/09/2011 10:19:51
//
//  @return       XSTRING* :
//  */
/*-----------------------------------------------------------------*/
XSTRING* XERRORMSG::GetTitle()
{
  return &title;
}


/*-------------------------------------------------------------------
//  XERRORMSG::GetMsg
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/09/2011 10:20:20
//
//  @return       XSTRING* :
//  */
/*-----------------------------------------------------------------*/
XSTRING* XERRORMSG::GetMsg()
{
  return &msg;
}


//-------------------------------------------------------------------
//  XERRORMSG::Set
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/11/2001 12:02:18
//
//  @return       bool :
//  @param        code :
//  @param        title :
//  @param        msg :
*/
//-------------------------------------------------------------------
bool XERRORMSG::Set(int code,XCHAR* title,XCHAR* msg)
{
  if(!SetCode(code))   return false;
  if(!SetTitle(title)) return false;
  if(!SetMsg(msg))     return false;

  return true;
}


//-------------------------------------------------------------------
//  XERRORMSG::Set
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/11/2001 12:02:22
//
//  @return       bool :
//  @param        code :
//  @param        level :
//  @param        image :
//  @param        sound :
//  @param        title :
//  @param        msg :
*/
//-------------------------------------------------------------------
bool XERRORMSG::Set(int code,int level,int image,int sound,XCHAR* title,XCHAR* msg)
{
  if(!SetCode(code))   return false;
  if(!SetLevel(level)) return false;
  if(!SetImage(image)) return false;
  if(!SetSound(sound)) return false;
  if(!SetTitle(title)) return false;
  if(!SetMsg(msg))     return false;

  return true;
}


//-------------------------------------------------------------------
//  XERRORMSG::SetCode
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/11/2001 12:02:26
//
//  @return       bool :
//  @param        code :
*/
//-------------------------------------------------------------------
bool XERRORMSG::SetCode(int code)
{
  this->code=code;

  return true;
}


//-------------------------------------------------------------------
//  XERRORMSG::SetLevel
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/11/2001 12:02:29
//
//  @return       bool :
//  @param        level :
*/
//-------------------------------------------------------------------
bool XERRORMSG::SetLevel(int level)
{
  this->level=level;

  return true;
}


//-------------------------------------------------------------------
//  XERRORMSG::SetImage
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/11/2001 12:02:32
//
//  @return       bool :
//  @param        image :
*/
//-------------------------------------------------------------------
bool XERRORMSG::SetImage(int image)
{
  this->image=image;

  return true;
}



//-------------------------------------------------------------------
//  XERRORMSG::SetSound
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/11/2001 12:02:36
//
//  @return       bool :
//  @param        sound :
*/
//-------------------------------------------------------------------
bool XERRORMSG::SetSound(int sound)
{
  this->sound=sound;

  return true;
}



//-------------------------------------------------------------------
//  XERRORMSG::SetTitle
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/11/2001 12:02:40
//
//  @return       bool :
//  @param        title :
*/
//-------------------------------------------------------------------
bool XERRORMSG::SetTitle(XCHAR* title)
{
  if(!title) return false;

  this->title = title;

  return true;
}



/*-------------------------------------------------------------------
//  XERRORMSG::SetTitle
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/09/2011 10:23:50
//
//  @return       bool :
//  @param        title :
*/
/*-----------------------------------------------------------------*/
bool XERRORMSG::SetTitle(XSTRING& title)
{
  return SetTitle(title.Get());
}



//-------------------------------------------------------------------
//  XERRORMSG::SetMsg
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/11/2001 12:02:43
//
//  @return       bool :
//  @param        msg :
*/
//-------------------------------------------------------------------
bool XERRORMSG::SetMsg(XCHAR* msg)
{
  if(!msg) return false;

  this->msg = msg;

  return true;
}



/*-------------------------------------------------------------------
//  XERRORMSG::SetMsg
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/09/2011 10:24:32
//
//  @return       bool :
//  @param        msg :
*/
/*-----------------------------------------------------------------*/
bool XERRORMSG::SetMsg(XSTRING& msg)
{
  return SetMsg(msg.Get());
}



//-------------------------------------------------------------------
//  XERRORMSG::Clean
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/11/2001 12:02:47
//
//  @return       bool :
//  */
//-------------------------------------------------------------------
bool XERRORMSG::Clean()
{
  code  = 0;
  level = 0;
  image = 0;
  sound = 0;

  title.Empty();
  msg.Empty();

  return true;
}



//-------------------------------------------------------------------
//  XERRORSMSG::XERRORSMSG
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/11/2001 12:02:53
//
//  @return
//  */
//-------------------------------------------------------------------
XERRORSMSG::XERRORSMSG()
{

}


//-------------------------------------------------------------------
//  XERRORSMSG::XERRORSMSG
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/11/2001 12:03:07
//
//  @return
//  @param        titleapp :
//  @param        version :
*/
//-------------------------------------------------------------------
XERRORSMSG::XERRORSMSG(XCHAR* titleapp,XCHAR* version)
{
  Clean();

  SetTitleApplication(titleapp);
  SetVersion(version);
}



//-------------------------------------------------------------------
//  XERRORSMSG::~XERRORSMSG
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/11/2001 12:03:17
//
//  @return
//  */
//-------------------------------------------------------------------
XERRORSMSG::~XERRORSMSG()
{
  EraseAllMsg();

  Clean();
}



/*-------------------------------------------------------------------
//  XERRORSMSG::GetTitleApplication
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/09/2011 10:29:22
//
//  @return       XSTRING* :
//  */
/*-----------------------------------------------------------------*/
XSTRING* XERRORSMSG::GetTitleApplication()
{
  return &titleapp;
}



//-------------------------------------------------------------------
//  XERRORSMSG::GetVersion
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/11/2001 12:03:30
//
//  @return       char* :
//  */
//-------------------------------------------------------------------
XSTRING* XERRORSMSG::GetVersion()
{
  return &version;
}



//-------------------------------------------------------------------
//  XERRORSMSG::SetTitleApplication
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/11/2001 12:03:45
//
//  @return       bool :
//  @param        titleapp :
*/
//-------------------------------------------------------------------
bool XERRORSMSG::SetTitleApplication(XCHAR* titleapp)
{
  if(!titleapp) return false;

  this->titleapp = titleapp;

  return true;
}


/*-------------------------------------------------------------------
//  XERRORSMSG::SetTitleApplication
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/09/2011 10:30:39
//
//  @return       bool :
//  @param        titleapp :
*/
/*-----------------------------------------------------------------*/
bool XERRORSMSG::SetTitleApplication(XSTRING& titleapp)
{
  return SetTitleApplication(titleapp.Get());
}




//-------------------------------------------------------------------
//  XERRORSMSG::SetVersion
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/11/2001 12:03:48
//
//  @return       bool :
//  @param        version :
*/
//-------------------------------------------------------------------
bool XERRORSMSG::SetVersion(XCHAR* version)
{
  if(!version) return false;

  this->version = version;

  return true;
}



/*-------------------------------------------------------------------
//  XERRORSMSG::SetVersion
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/09/2011 10:31:53
//
//  @return       bool :
//  @param        version :
*/
/*-----------------------------------------------------------------*/
bool XERRORSMSG::SetVersion(XSTRING& version)
{
  return SetVersion(version.Get());
}



//-------------------------------------------------------------------
//  XERRORSMSG::FindMsg
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/11/2001 12:03:53
//
//  @return       XERRORMSG* :
//  @param        code :
*/
//-------------------------------------------------------------------
XERRORMSG* XERRORSMSG::FindMsg(int code)
{
  for(XDWORD c=0;c<errors.GetSize();c++)
    {
      XERRORMSG* error = errors.Get(c);
      if(error)
        {
          if(error->GetCode()==code) return error;
        }
    }

  return NULL;
}


//-------------------------------------------------------------------
//  XERRORSMSG::AddMsg
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/11/2001 12:03:58
//
//  @return       bool :
//  @param        code :
//  @param        level :
//  @param        image :
//  @param        sound :
//  @param        title :
//  @param        msg :
*/
//-------------------------------------------------------------------
bool XERRORSMSG::AddMsg(int code,int level,int image,int sound, XCHAR* title,XCHAR* msg)
{
  XERRORMSG* error;

  if(FindMsg(code)) return false;

  error = new XERRORMSG(code,title,msg);
  if(!error) return false;

  error->SetLevel(level);
  error->SetImage(image);
  error->SetSound(sound);

  errors.Add(error);

  return true;
}


//-------------------------------------------------------------------
//  XERRORSMSG::EraseMsg
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/11/2001 12:04:02
//
//  @return       bool :
//  @param        code :
*/
//-------------------------------------------------------------------
bool XERRORSMSG::EraseMsg(int code)
{
  if(errors.IsEmpty()) return false;

  for(XDWORD c=0; c<errors.GetSize(); c++)
    {
      XERRORMSG* error = errors.Get(c);

      if(!error)
        {
          if(error->GetCode()==code)
            {
              errors.Delete(error);

              delete error;

              return true;
            }
        }
    }

  return false;
}




//-------------------------------------------------------------------
//  XERRORSMSG::EraseAllMsg
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/11/2001 12:04:28
//
//  @return       bool :
//  */
//-------------------------------------------------------------------
bool XERRORSMSG::EraseAllMsg()
{
  if(errors.IsEmpty()) return false;

  errors.DeleteContents();

  errors.DeleteAll();

  return true;
}


//-------------------------------------------------------------------
//  XERRORSMSG::ShowMsg
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/11/2001 12:04:33
//
//  @return       XERRORMSG_RETURN :
//  @param        code :
//  @param        returns :
//  @param        addstr :
//  @param        sound :
*/
//-------------------------------------------------------------------
XERRORMSG_RETURN XERRORSMSG::ShowMsg(int code,bool statusline,XBYTE returns,XCHAR* addstr,bool sound)
{
  return XERRORMSG_RETURN_NONE;
}



//-------------------------------------------------------------------
//  XERRORSMSG::Clean
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/11/2001 12:04:37
//
//  @return       bool :
//  */
//-------------------------------------------------------------------
bool XERRORSMSG::Clean()
{
  titleapp.Empty();
  version.Empty();

  return true;
}

