/*------------------------------------------------------------------------------------------
//	GRPAPPLICATION.CPP
//	
//	Graphic application
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 02/09/2016 11:01:56
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "GRPApplication.h"
#include "GRPLoaderInterface.h"
#include "GRPApplicationCFG.h"
#include "GRPFactory.h"
#include "GRPContext.h"
#include "GRPScene.h"
#include "GRPRenderer.h"
#include "GRPOpenGL.h"
#include "GRPProfiler.h"
#include "GRPMaterialLibrary.h"
#include "SNDManager.h"

#include "SNDFactory.h"

#include "INPFactory.h"
#include "INPManager.h"

#include "XLog.h"
#include "XDebug.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


#ifdef GRP_ACTIVE

/*-------------------------------------------------------------------
//	GRPAPPLICATION::IniRenderer
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			16/07/2014 21:01:13
//	
//	@return 			bool : 
//
//  @param				screen : 
//  @param				viewport : 
//  @param				scene : 
*/
/*-----------------------------------------------------------------*/
bool GRPAPPLICATION::IniRenderer(GRPSCREEN*	screen, GRPSCENE* scene) 
{	

	return true;										
}				



/*-------------------------------------------------------------------
//	GRPAPPLICATION::IniMainScreen
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			23/04/2014 10:10:48
//	
//	@return 			bool : 
//
//  @parammainscreenproperties : 
//  @parammainscreenhandle : 
*/
/*-----------------------------------------------------------------*/
bool GRPAPPLICATION::IniMainScreen(GRPPROPERTIES* mainscreenproperties, void* mainscreenhandle)
{	
	mainscreen = grpfactory->CreateScreen();
	if(!mainscreen) 
		{
			XLOG::GetInstance().AddEntry(XLOGLEVEL_ERROR,  __L("Generic"), false, __L("Could not initialize the screen ..."));	
			return false;
		}

	maincontext = grpfactory->CreateContext();
	if(!maincontext) 
		{
			XLOG::GetInstance().AddEntry(XLOGLEVEL_ERROR,  __L("Generic"), false, __L("Could not initialize the Graphics Context ..."));	
			return false;
		}
	maincontext->SetScreen(mainscreen);

	SubscribeEvent(GRPXEVENTTYPE_SCREEN_CHANGESIZE, (XSUBJECT*)mainscreen);
		
	mainscreen->CopyPropertysFrom(mainscreenproperties);
	mainscreen->SetIsFullScreen(IsFullScreen());
	
	maincontext->SetMultiSampling(multisampling);  // 4

	if(!mainscreen->IsFullScreen())
		{									
			XDEBUG_PRINTCOLOR(0,__L("WINDOWED   ScreenX %d ScreenY %d"),resolutionx,resolutiony);
			mainscreen->SetPosition(positionx,positiony);
			mainscreen->SetWidth(resolutionx);   mainscreen->SetHeight(resolutiony);						
		}
	else
		{
			XDEBUG_PRINTCOLOR(0,__L("FULLSCREEN ScreenX %d ScreenY %d"),resolutionx,resolutiony);
		}

	if(!maincontext->Create(mainscreenhandle)) return false;

	OPENGLCHECKERROR(__L("ScreenCreate"));

	return WindowCreated();	
}




/*-------------------------------------------------------------------
//  GRPAPPLICATION::EndMainScreen
*/ 
/**
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			22/10/2013 16:35:25
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool GRPAPPLICATION::EndMainScreen()
{	
	if(!grpfactory || !mainscreen) return false;

	UnSubscribeEvent(GRPXEVENTTYPE_SCREEN_CHANGESIZE, (XSUBJECT*)mainscreen);
		
	grpfactory->DeleteScreen(mainscreen);
	grpfactory->DeleteContext(maincontext);

	maincontext = NULL;
	mainscreen = NULL;
	
	return true;		
}	



/*-------------------------------------------------------------------
//	 GRPAPPLICATION::InitLog
*/
/**
//
//	Inits log default place
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version		
//	@return		bool : 
//
//	@param		XCHAR* : 
//
*//*-----------------------------------------------------------------*/
bool GRPAPPLICATION::InitLog(XCHAR* name)
{
	XPATH xpathroot;
	XPATH xpath;

	if(GetApplicationName()->GetSize() == 0) return false;

	XPATHSMANAGER::GetInstance().GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpathroot);
	xpath.Create(3, xpathroot.Get(), name, __L(".log"));

	XDEBUG_PRINTCOLOR(2, __L("Log path: %s"), xpath.Get());

	if (!XLOG::GetInstance().Ini(xpath, (*GetApplicationName()), true)) return false;

	XLOG::GetInstance().SetLimit(XLOGTYPELIMIT_SIZE, 1024 * 1024 * 16, 10);
	XLOG::GetInstance().SetBackup(true, 10, true);
	XLOG::GetInstance().AddEntry(XLOGLEVEL_INFO, __L("Generic"), false, __L("Initializing application ..."));
	
	return true;
}




/*-------------------------------------------------------------------
//	 GRPAPPLICATION::InitDebug
*/
/**
//  Inits debugger default places
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version		
//	@return		bool : 
//
//	@param		const : 
//	@param		const : 
//	@param		const : 
//
*//*-----------------------------------------------------------------*/
bool GRPAPPLICATION::InitDebug(const int VERSION, const int SUBVERSION, const int SUBVERSIONERR)
{			
	XDEBUG_SETTARGET(0, XDEBUGCTRLTYPE_NET, XDEBUG_DEFAULT_NETAIM1);
	XDEBUG_SETTARGET(1, XDEBUGCTRLTYPE_NET, XDEBUG_DEFAULT_NETAIM2);
	XDEBUG_SETTARGET(2, XDEBUGCTRLTYPE_NET, XDEBUG_DEFAULT_NETAIM3);
	XDEBUG_SETTARGET(3, XDEBUGCTRLTYPE_NET, XDEBUG_DEFAULT_NETAIM4);
	XDEBUG_SETTARGET(4, XDEBUGCTRLTYPE_NET, XDEBUG_DEFAULT_NETAIM5);

	XDEBUG_SETAPPLICATIONNAME(this->GetApplicationName()->Get());
	XDEBUG_SETAPPLICATIONVERSION(VERSION, SUBVERSION, SUBVERSIONERR);

	return true;
}



/*-------------------------------------------------------------------
//	 GRPAPPLICATION::LoadConfigurationFromFile
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	23/10/2017 17:07:22
//	@return		bool : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
bool GRPAPPLICATION::LoadConfigurationFromFile()
{	
	if(configurationfile.GetSize() > 0)
		{
			graphicsettings = new GRPAPPLICATIONCONFIG(this->configurationfile.Get());
			if(graphicsettings)
				{
					if(graphicsettings->Load())
						{
							graphicsettings->GetScreenPosition(&positionx,		&positiony);
							graphicsettings->GetResolution(&resolutionx,	&resolutiony);

							isfullscreen								= graphicsettings->IsFullScreen();
							SNDFACTORY::SoundIsActive		= graphicsettings->Sound_IsActive();
								
							return OnConfigurationFileLoaded();
						}						
				}

			return false;
		}

	return true;
}



/*-------------------------------------------------------------------
//	 GRPAPPLICATION::InitPaths
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version		
//	@return		bool : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
bool GRPAPPLICATION::InitPaths()
{
	if(!XPATHSMANAGER::GetInstance().AdjustRootPathDefault((XCHAR*)this->GetApplicationName()->Get(), __L("resources")))	return false;
		
	if(!XPATHSMANAGER::GetInstance().AddPathSection(XPATHSMANAGERSECTIONTYPE_GRAPHICS	, __L("graphics")))		return false;
	if(!XPATHSMANAGER::GetInstance().AddPathSection(XPATHSMANAGERSECTIONTYPE_FONTS		,	__L("fonts")))			return false;
	if(!XPATHSMANAGER::GetInstance().AddPathSection(XPATHSMANAGERSECTIONTYPE_SOUNDS		,	__L("sounds")))			return false;

	XPATH xpath;
	xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L(""));
	GRPMATERIALLIBRARY::Get()->SetResourceLocation(xpath);

	return true;
}


/*-------------------------------------------------------------------
//	 GRPAPPLICATION::InitLoader
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	17/10/2017 11:41:42
//	@return		bool : 
//
//	@param		XCHAR* : 
//	@param		XCHAR* : 
//
*//*-----------------------------------------------------------------*/
bool GRPAPPLICATION::InitLoader(XCHAR* xml, XCHAR* material, XCHAR* shader)
{																																								
	OPENGLCHECKERROR(__L("GRPAPPLICATION::InitLoader()"));

	XPATH xpath;
	xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, xml);

	loaderInterface = new GRPLOADERINTERFACE(mainscene, material, GRPSHADERLIBRARY::Get()->GetProgramByName(shader));
	if(!loaderInterface)												return false;		
	
	OPENGLCHECKERROR(__L("GRPAPPLICATION::InitLoader() GRPLOADERINTERFACE"));

	loaderInterface->AddObserver(this);
	if(!loaderInterface->Init())								return false; 	
	
	OPENGLCHECKERROR(__L("GRPAPPLICATION::InitLoader() loaderInterface->Init"));

	if(!loaderInterface->AddTask(xpath.Get()))	return false;

	if(!loaderInterface->GetLoader())						return false;
					
	loaderInterface->GetLoader()->Start();
		
	return true;
}




/*-------------------------------------------------------------------
//	 GRPAPPLICATION::SetVersion
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version		
//	@return		bool : 
//
//	@param		XDWORD : 
//	@param		XDWORD : 
//	@param		XDWORD : 
//
*//*-----------------------------------------------------------------*/
bool GRPAPPLICATION::SetVersion(XDWORD v, XDWORD sv, XDWORD ve)
{
	this->version[0] = v;
	this->version[1] = sv;
	this->version[2] = ve;

	return true;
}




/*-------------------------------------------------------------------
//	 GRPAPPLICATION::LoadObject
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version		
//	@return		GRPOBJECT* : 
//
//	@param		GRPOBJECT* : 
//	@param		XCHAR* : 
//	@param		XDWORD : 
//	@param		XDWORD : 
//	@param		XDWORD : 
//
*//*-----------------------------------------------------------------*/
GRPOBJECT* GRPAPPLICATION::LoadObject(GRPOBJECT* obj, XCHAR* path, XDWORD loadflags, XDWORD verbose, XDWORD NextStackID)
{
	if(!GetMainScene())		 return NULL;

	bool ownedobj = false;

	XPATH	xpath;
	
	xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, path);
				
	if(obj == NULL)
		{
			obj = new GRPOBJECT();
			ownedobj = true;
		}

	GRPFILE3D* grpfile3d = NULL;
						
	grpfile3d = new GRPFILE3D();						
	if (!grpfile3d) return NULL;

	grpfile3d->SetObject(obj);
	grpfile3d->SetScene(GetMainScene());
	grpfile3d->SetVerboseLevel(verbose);
	grpfile3d->SetLoadFlags(loadflags);
	grpfile3d->SetNextStackID(NextStackID);

	GRPOBJECT* object = grpfile3d->Load(xpath.Get());
	
	OPENGLCHECKERROR(__L("LoadObj"));

	delete(grpfile3d);
	grpfile3d = NULL;

	return object;
}



/*-------------------------------------------------------------------
//	 GRPAPPLICATION::IniApplication
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	17/10/2017 15:54:59
//	@return		bool : 
//
//	@param		XSYSTEM* : 
//	@param		XVECTOR<XSTRING*>* : 
//
*//*-----------------------------------------------------------------*/
bool GRPAPPLICATION::IniApplication(XSYSTEM* xsystem, XVECTOR<XSTRING*>* execparams)
{
	if (!xfactory)	return false;
	if (!xsystem)		return false;

	this->xsystem				= xsystem;
	this->execparams		= execparams;

	xtimerglobal = xfactory->CreateTimer();
	if (!xtimerglobal) return false;

	xtimerglobal->Reset();

	#ifdef GRP_ACTIVE
	if (!Ini()) return false;
	#endif

	return true;
}




/*-------------------------------------------------------------------
//	 GRPAPPLICATION::Notify
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	25/10/2017 11:46:45
//	@return		bool : 
//
//	@param		GRPMSGTYPE : 
//	@param		GRPSUBJECT* : 
//
*//*-----------------------------------------------------------------*/
bool GRPAPPLICATION::Notify(GRPMSGTYPE type, GRPSUBJECT* subject) 
{ 
	switch (type)
		{
			case GRPMSG_EXIT	:	if(subject->GetSubjectType() == GRPNAMED_TYPE_TASK)					this->OnLoadFail((GRPLOADERTASK*)subject);			  
													break;

			case GRPMSG_END		:	if(subject->GetSubjectType() == GRPNAMED_TYPE_TASKMANAGER)	this->OnResourcesLoaded();													
													break;
		}

	return	true; 
}



/*-------------------------------------------------------------------
//	GRPAPPLICATION::PathManagement
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			14/06/2017 10:20:29
//	
//	@return 			bool : 
//
//  @param				addpath : 
*/
/*-----------------------------------------------------------------*/
bool GRPAPPLICATION::PathManagement(XCHAR* addpath)
{ 
	XPATH xpathroot;		
	XPATH xpath;

	if(rootpath.IsEmpty())
	XPATHSMANAGER::GetInstance().GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, rootpath);

	if(addpath)
		{
			xpath=rootpath;
			xpath.Slash_Add();
			xpath += addpath;
		}

	XPATHSMANAGER::GetInstance().AddPathSection(XPATHSMANAGERSECTIONTYPE_ROOT, rootpath);	
	
	XPATHSMANAGER::GetInstance().AddPathSection(XPATHSMANAGERSECTIONTYPE_GRAPHICS , __L("graphics"));
	XPATHSMANAGER::GetInstance().AddPathSection(XPATHSMANAGERSECTIONTYPE_FONTS		, __L("fonts"));
	XPATHSMANAGER::GetInstance().AddPathSection(XPATHSMANAGERSECTIONTYPE_SOUNDS		, __L("sounds"));
	
	xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1 ,__L(""));
	GRPMATERIALLIBRARY::Get()->SetResourceLocation(xpath);

	return true;
}




/*-------------------------------------------------------------------
//	GRPAPPLICATION::Update
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			01/09/2016 13:21:01
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool	GRPAPPLICATION::Update()
{
	if(this->exitstatus != XAPPLICATIONEXITTYPE_NONE) return false;

	switch (state)
		{
			case GRPAPPLICATION_STATE_INIT								:	return FirstUpdate();
																											break;

			case GRPAPPLICATION_STATE_WAITING_FOR_WINDOW	:	return true;
																											break;
		
			case GRPAPPLICATION_STATE_RENDERING						:
																											#ifdef INP_ACTIVE
																											INPMANAGER::GetInstance()->Update();
																											#endif

																											if(SNDFACTORY::Get())
																												{
																													if(SNDFACTORY::Get()->IsInit()) SNDFACTORY::Get()->Update();
																												}

																											if(mainscene)		mainscene->renderer->Render();

																											if(mainscreen)	
																												{
																													if(!mainscreen->Update()) return false;
																												}

																											if(maincontext)	
																												{
																													if(!maincontext->Update()) return false;
																												}

																											GRPPROFILER::ShowStatistics();

																											return OnUpdate();
																											break;

		}
	
	return false;
}

/*-------------------------------------------------------------------
//	GRPAPPLICATION::IniInputManager
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			01/09/2016 13:21:42
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPAPPLICATION::IniInputManager()
{	
	#ifdef INP_ACTIVE

	if(!INPFACTORY::GetInstance())			return true;
	if(!INPMANAGER::GetInstance()) 			return true;

	if(!INPMANAGER::GetInstance()->GetDevice(INPDEVICE_TYPE_KEYBOARD))
		{
			keyboard = INPFACTORY::GetInstance()->CreateDevice(__L("keyboard"), mainscreen);
			if(keyboard)	INPMANAGER::GetInstance()->AddDevice(keyboard);
		}
	 else keyboard = INPMANAGER::GetInstance()->GetDevice(INPDEVICE_TYPE_KEYBOARD);

	if(!INPMANAGER::GetInstance()->GetDevice(INPDEVICE_TYPE_MOUSE))
		{
			mouse = INPFACTORY::GetInstance()->CreateDevice(__L("mouse"), mainscreen);
			if(mouse) INPMANAGER::GetInstance()->AddDevice(mouse);
		}
		else mouse= INPMANAGER::GetInstance()->GetDevice(INPDEVICE_TYPE_MOUSE);

	#endif

	return true;
}




/*-------------------------------------------------------------------
//	 GRPAPPLICATION::Init
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version		
//	@return		bool : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
bool GRPAPPLICATION::Ini()
{		
	if(!xsystem)																					return false;
	if(!InitPaths())																			return false;

	if(!OnConfig())																				return false;

	if(initoptions & GRPAPPLICATION_OPTIONS_DEBUG)					
		{
			if(!InitDebug(version[0], version[1], version[2])) return false;
		}

	if(initoptions & GRPAPPLICATION_OPTIONS_LOADCONFIG)		
		{
			if(!LoadConfigurationFromFile())	return false;
		}

	if(initoptions & GRPAPPLICATION_OPTIONS_LOG)						
		{
			if(!InitLog(GetApplicationName()->Get()))	 return false;
		}

	state = GRPAPPLICATION_STATE_WAITING_FOR_WINDOW;

	#ifndef ANDROID
	if(initoptions & GRPAPPLICATION_OPTIONS_SCREEN)				
		{
			if(!IniMainScreen()) return false;
		}
	#endif		

	if(!(initoptions & GRPAPPLICATION_OPTIONS_SCREEN) && (initoptions & GRPAPPLICATION_OPTIONS_INPUT))				
		{
			if(!IniInputManager()) return false;
		}

	if(!XAPPLICATION::Ini())  return false;

	return true;
}


/*-------------------------------------------------------------------
//	 GRPAPPLICATION::End
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version		
//	@return		bool : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
bool GRPAPPLICATION::End()
{
	OnEnd();

	//--------------------------------------------------------------------------------------

	switch (GetExitStatus())
		{
			case XAPPLICATIONEXITTYPE_NONE				: XLOG::GetInstance().AddEntry(XLOGLEVEL_ERROR	, __L("General"), false, __L("No information closing."));											break;
			case XAPPLICATIONEXITTYPE_APPERROR		:	XLOG::GetInstance().AddEntry(XLOGLEVEL_ERROR	, __L("General"), false, __L("Application closed by a serious error."));			break;
			case XAPPLICATIONEXITTYPE_APPEND			: XLOG::GetInstance().AddEntry(XLOGLEVEL_INFO		, __L("General"), false, __L("Completed application."));											break;
			case XAPPLICATIONEXITTYPE_BYUSER			: XLOG::GetInstance().AddEntry(XLOGLEVEL_INFO		, __L("General"), false, __L("Application closed by user."));									break;
			case XAPPLICATIONEXITTYPE_SOSHUTDOWN	: XLOG::GetInstance().AddEntry(XLOGLEVEL_WARNING, __L("General"), false, __L("Application closed by S.O. shutdown."));				break;
																		default	: break;
		}

	//--------------------------------------------------------------------------------------

	#ifdef GRP_ACTIVE
	DeleteRenderer();
	EndMainScreen();
	#endif

	//--------------------------------------------------------------------------------------

	if(loaderInterface) delete(loaderInterface);

	//--------------------------------------------------------------------------------------

	delete(this->graphicsettings);

	//--------------------------------- 
	
	XLOG::GetInstance().AddEntry(XLOGLEVEL_INFO, __L("General"), false, __L("Closing the application ..."));
	
	//--------------------------------- sound		

	SNDMANAGER::Get()->Destroy();

	return OnEnded();
}




/*-------------------------------------------------------------------
//	 GRPAPPLICATION::FirstUpdate
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version		
//	@return		bool : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
bool GRPAPPLICATION::FirstUpdate()
{
	if(!OnInit())			return false;
	if(!mainrenderer) return false;
		
	mainrenderer->PrepareScenes();
	mainrenderer->SetClearFlags(GRPSCENE_CLEAR_ALL);

	state = GRPAPPLICATION::GRPAPPLICATION_STATE_RENDERING;

	return OnStartRendering();
}



/*-------------------------------------------------------------------
//	 GRPAPPLICATION::LoadResources
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	16/10/2017 15:54:54
//	@return		bool : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
bool GRPAPPLICATION::LoadResources()
{		
	return OnResourcesLoaded();		
}


/*-------------------------------------------------------------------
//	 GRPAPPLICATION::WindowCreated
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version		
//	@return		bool : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
bool GRPAPPLICATION::WindowCreated()
{		
	if(!CreateRenderer())	return false;

	if(initoptions & GRPAPPLICATION_OPTIONS_INPUT)			
		{
			if(!IniInputManager())	return false;
		}
				

	state = GRPAPPLICATION::GRPAPPLICATION_STATE_INIT;

	return OnCreateWindow();
}



/*-------------------------------------------------------------------
//	GRPAPPLICATION::CreateRenderer
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			10/07/2014 16:50:04
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPAPPLICATION::CreateRenderer()
{				
	if(mainrenderer != NULL)	delete(mainrenderer);
	
	mainrenderer = new GRPRENDERER(mainscreen, maincontext);	
	if(!mainrenderer)		
		{	
			return false;	
		}	
	
	XDEBUG_PRINTCOLOR(0, __L("Creating Renderer")); 

	mainscene	= new GRPSCENE(mainscreen);								
	if(!mainscene)			
		{ 
			delete(mainrenderer); 
			return false; 
		} 
	
	XDEBUG_PRINTCOLOR(0, __L("Creating Scene"));

	mainscene->SetName(__L("MainScene"));
																			
	if(!IniRenderer(mainscreen, mainscene))	return false;	
	
	XDEBUG_PRINTCOLOR(0, __L("Initializing Renderer"));

	mainrenderer->AddScene(mainscene);
	if(!mainrenderer->Ini()) return false;				

	return true;
}





/*-------------------------------------------------------------------
//	GRPAPPLICATION::EndRenderer
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			16/07/2014 21:00:31
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPAPPLICATION::EndRenderer()
{
	if(mainrenderer) mainrenderer->End();	
		
	UnSubscribeEvent(GRPXEVENTTYPE_SCENE_FRAMEINI , (XSUBJECT*)mainscene);
	UnSubscribeEvent(GRPXEVENTTYPE_SCENE_FRAMEEND , (XSUBJECT*)mainscene);
	UnSubscribeEvent(GRPXEVENTTYPE_SCENE_ENTITYINI, (XSUBJECT*)mainscene);
	UnSubscribeEvent(GRPXEVENTTYPE_SCENE_ENTITYEND, (XSUBJECT*)mainscene);
	UnSubscribeEvent(GRPXEVENTTYPE_SCENE_INPUT,			(XSUBJECT*)mainscene);
	
	return true;
}


/*-------------------------------------------------------------------
//	GRPAPPLICATION::DeleteRenderer
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			10/07/2014 16:51:02
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPAPPLICATION::DeleteRenderer()
{
	if(!mainrenderer) return true;

	mainrenderer->SetClearFlags (GRPSCENE_CLEAR_ALL);
	mainrenderer->Clear					();

	if(!EndRenderer()) return false;
	
	delete mainrenderer;
	mainrenderer = NULL;
	mainscene		 = NULL;
	
	GRPPROFILER::Delete();

	return true;
}

#endif
