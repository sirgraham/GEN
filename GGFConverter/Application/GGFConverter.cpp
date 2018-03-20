
/*------------------------------------------------------------------------------------------
//	GGFCONVERTER.CPP
//	
//	Convers FBX to GGF
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 13/06/2017 13:17:49
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "GGFConverter.h"
#include "GRPRenderer.h"
#include "GRPScene.h"
#include "GRPMaterialLibrary.h"
#include "GRPFile3D.h"
#include "GRPFile3DGGF.h"
#include "GRPBlendingPass.h"
#include "GRPConsole.h"
#include "GRPFontManager.h"


#include "SNDAnimationListener.h"
#include "SNDManager.h"

#include "XConsole.h"
#include "XFileXML.h"
#include "XDebug.h"
#include "XLog.h"
#include "XTimer.h"
#include "XDir.h"

#include "INPManager.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//	XAPPLICATION::Create
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			13/06/2017 13:22:16
//	
//	@return 			XAPPLICATION* : 
//
*/
/*-----------------------------------------------------------------*/
XAPPLICATION* XAPPLICATION::Create()
{
	GGFCONVERTER* app;
	app = new GGFCONVERTER();

	return app;
}


/*-------------------------------------------------------------------
//	GGFCONVERTER_TASK::Print
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			14/06/2017 16:58:25
//	
//  @param				priority : 
//  @param				text : 
*/
/*-----------------------------------------------------------------*/
void GGFCONVERTER_TASK::Print(XDWORD priority,XCHAR* text)
{	
	if (priority!=4)
		if (!showheader)
			return;

	XDEBUG_PRINTCOLOR(priority,text);
	if (this->xconsole)
	{
			
		XSTRING s;
		if (priority==0)
		{
		this->xconsole->Print(__L("\33[2K\r"));
		this->xconsole->Print(__L("                                                                           "));
		this->xconsole->Print(__L("\33[2K\r"));
		s.Format(__L("%02.f%%%% "),(this->current/this->total));
		this->xconsole->Print(s.Get());
		XDWORD nSections=8;
			for (XDWORD e=0;e<nSections;e++)
			{
			if (e<(this->current/this->total)/(100.0f/nSections))
				this->xconsole->Printf(__L("#"));
			else
					this->xconsole->Print(__L("-"));
			}
		this->xconsole->Print(__L(" "));
		}
		else			
			this->xconsole->Print(__L("\n"));						
			
		this->xconsole->Print(text);
		//this->xconsole->Print(__L("\n"));
	}
}

/*-------------------------------------------------------------------
//	GGFCONVERTER_TASK::Output
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			14/06/2017 16:58:37
//	
//  @param				priority : 
//  @param				mask : 
//  @param				... : 
*/
/*-----------------------------------------------------------------*/
void GGFCONVERTER_TASK::Output(XDWORD priority, XCHAR* mask,...)
{
	XSTRING string;	
	va_list arg;			
	va_start(arg, mask);	
	string.FormatArg(mask, &arg);		
	va_end(arg);
	xconsole->Print(string.Get());
}


/*-------------------------------------------------------------------
//	GGFCONVERTER::~GGFCONVERTER
*/	
/**	
//	
//	 Class Destructor GGFCONVERTER
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			14/06/2017 17:49:02
//	
*/
/*-----------------------------------------------------------------*/
GGFCONVERTER::~GGFCONVERTER()
{
	delete(xconsole);
	delete(file3d);	
	delete(scene);	
	delete(task);
	delete(copier);

	dependencies.DeleteContents();
	dependencies.DeleteAll();

	GRPOBJECTBUILDER::Destroy();
	GRPPASSMANAGER::Destroy();
	GRPSHADERLIBRARY::Destroy();
	GRPMATERIALLIBRARY::Destroy();
	SNDMANAGER::Destroy();
}

/*-------------------------------------------------------------------
//	GGFCONVERTER::Ini
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			13/06/2017 13:20:19
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GGFCONVERTER::Ini()
{

	XSTRING string;

	if(!xsystem) return false;

		//------------------------------------------------------ LOAD SHADERS
	GRPSHADERLIBRARY::Get()->LoadShaders();

	scene=new GRPSCENE();
	scene->Ini(GRPSCENE_INIT_FLAGS_CREATE_PASS);


	//------------------------------------------------------ copier
	copier=xfactory->Create_Dir();

	//-------------------------------------------------------------------------------------------------

	GetApplicationName()->Set(GGFCONVERTER_NAMEAPP);

	//--------------------------------------------------------------------------------------	

		xconsole = xfactory->CreateConsole();
	if(!xconsole) return false;

	//--------------------------------------------------------------------------------------

	if(!XPATHSMANAGER::GetInstance().AdjustRootPathDefault(GGFCONVERTER_NAMEAPP, __L("resources"))) 
	{
		if (xconsole)
		{
		xconsole->Clear();
		string.Format(__L("\n\nError: no resources folder on local folder\n\n\n"));
		xconsole->Print(string.Get());
		xconsole->WaitKey(__L(""),0,false,10);
		}
		return false;
	}
				  
	XDEBUG_SETTARGET(0 , XDEBUGCTRLTYPE_SPECIAL, XDEBUG_DEFAULT_NETAIM	);		  
	XDEBUG_SETTARGET(1 , XDEBUGCTRLTYPE_NET, XDEBUG_DEFAULT_NETAIM2	);
	XDEBUG_SETTARGET(2 , XDEBUGCTRLTYPE_NET, XDEBUG_DEFAULT_NETAIM3	);
	XDEBUG_SETTARGET(3 , XDEBUGCTRLTYPE_NET, XDEBUG_DEFAULT_NETAIM4	);
	XDEBUG_SETTARGET(4 , XDEBUGCTRLTYPE_SPECIAL,XDEBUG_DEFAULT_SPECIALAIM);

	string = GGFCONVERTER_NAMEAPP;		XDEBUG_SETAPPLICATIONNAME(string);
	XDEBUG_SETAPPLICATIONVERSION(GGFCONVERTER_VERSION, GGFCONVERTER_SUBVERSION, GGFCONVERTER_SUBVERSIONERR);

	//--------------------------------------------------------------------------------------	

	 const char usage[]=
	 {
				"usage: \r\n"
				"	EX: -p multiwildpot_v1 -i JackpotCasinoRoulette.xml -o ../output\r\n"

				"  -p path \r\n"
				"  -i input \r\n"
				"  -o output \r\n"
				"  -h showheaders \r\n\r\n\r\n"
	 };
			
	const char GENLogo[]={
				"                                                                               \r\n"
				"                        .odMMNh/`                                              \r\n"
				"                      .odMMMMMMNh/`                                            \r\n"
				"                    `omMMMMMMMMMMMd/                                           \r\n"
				"                   `hMMMMMms+ymMMMMMs                                          \r\n"
				"                  .++hNNy+ohNhoohNmy++`                                        \r\n"
				"                  hMNy++yNMMMMMms+ohNM+                                        \r\n"
				"                 -MMMMMMMMMMMMMMMMMMMMm                                        \r\n"
				"                 /MMMMMMMMMMMMMMMMMMMMN.                                       \r\n"
				"                 :MMMMMMMMMMMMMMMMMMMMN`                                       \r\n"
				"                 `mMMMMMMMMMMMMMMMMMMMs                                        \r\n"
				"                  :MMMMMMMMMMMMMMMMMMd`                                        \r\n"
				"                   :mMMMMMMMMMMMMMMMh`                                         \r\n"
				"                    `oNMMMMMMMMMMMd/                                           \r\n"
				"                      `:ohmNNNdy+.                                             \r\n"
				"                          ````                                                 \r\n"
				"                                                                               \r\n"
				"                                                                               \r\n"
				"               ````       ````       `````                                     \r\n" 
				"            .sdhhhh:    /dhhhhds`    mdhhhds.                                  \r\n"
				"           `md.```.     mm+//+dm`    Ms```.hm.                                 \r\n"
				"           `Nh`  `m+    mNo++++.     Ms    /M-                                 \r\n"
				"            :hhyyyNo    omyyyyys`    No    /N.                                 \r\n"
				"             `.----`     .------     -`    `-\n"};
																	 
	//string.Format(__L("**************************************************************************\r\n*\tGGFConverter %d.%d.%d (Beta)\r\n*\r\n*\t by: Diego Martinez Ruiz de Gaona\r\n*\r\n**************************************************************************\r\n"),GGFCONVERTER_VERSION,GGFCONVERTER_SUBVERSION, GGFCONVERTER_SUBVERSIONERR);
	
	
	//--------------------------------------------------------------------------------------	
	// Get Params 
	if(GetExecParams() && GetExecParams()->GetSize()>1)
		{
			for (XDWORD e=0;e<GetExecParams()->GetSize();e+=2)
			{
			XSTRING* key = (XSTRING*)GetExecParams()->Get(e);				
			XSTRING* value = (XSTRING*)GetExecParams()->Get(e+1);	
			this->options.Add(key,value);
			}
		}
	else
	{
		xconsole->Clear();
		string=usage;
		xconsole->Print(string.Get());
		xconsole->WaitKey(__L(""),0,false,10);
		return false;
	}

	// EX: -p multiwildpot_v1 -i JackpotCasinoRoulette.xml -o ../output

	// -p path
	// -i input
	// -o output
	// -h showheaders

	inputdir = __L("");

		for (XDWORD e=0;e<options.GetSize();e++)
		{
			if (options.GetKey(e)->Compare(__L("-p")) == 0)
					inputdir = options.GetElement(e)->Get();

			if (options.GetKey(e)->Compare(__L("-i"))==0)
					origin=options.GetElement(e)->Get();

			if (options.GetKey(e)->Compare(__L("-o"))==0)
					output=options.GetElement(e)->Get();

			if (options.GetKey(e)->Compare(__L("-h"))==0)
					showheader=false;
		}
		
	//-------------------------------------------------------------------------------------

		if (output.GetSize()==0 || origin.GetSize()==0)
		{		
			string=usage;
			xconsole->Print(string.Get());
			xconsole->WaitKey(__L(""),0,false,10);
			return false;
		}

		
	//-------------------------------------------------------------------------------------

	if (showheader)
	{
	xconsole->Clear();

	XSTRING logo;
	logo.Set(GENLogo);
	xconsole->Print(logo.Get());
	string.Format(__L("\n\n**************************************************************************\r\n*\tGGFConverter %d.%d.%d (Beta)\r\n*\r\n*\t by: Diego Martinez Ruiz de Gaona\r\n*\r\n**************************************************************************\r\n"),GGFCONVERTER_VERSION,GGFCONVERTER_SUBVERSION, GGFCONVERTER_SUBVERSIONERR);
	xconsole->Print(string.Get());
	}
	


	//--------------------------------------------------------------------------------------
		
	
	{ XPATH xpathroot;
		XPATH xpath;

		XPATHSMANAGER::GetInstance().GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpathroot);
		
		xpath.Create(3, xpathroot.Get(), GGFCONVERTER_NAMEAPP,__L(".log"));	

		XDEBUG_PRINTCOLOR(2, __L("Log path: %s"), xpath.Get());

		if(!XLOG::GetInstance().Ini		(xpath, (*GetApplicationName()), true)) return false;
		
		XLOG::GetInstance().SetLimit	(XLOGTYPELIMIT_SIZE, 1024*1024*16, 10);
		XLOG::GetInstance().SetBackup	(true, 10, true);
		XLOG::GetInstance().AddEntry	(XLOGLEVEL_INFO, GGFCONVERTER_LOGSECTIONIDGENERIC, false, __L("Initializing application ..."));
	} 
	
	//--------------------------------------------------------------------------------------

	PathManagement(NULL);

	return true;
}


/*-------------------------------------------------------------------
//	GGFCONVERTER::Update
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			13/06/2017 14:05:30
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool	GGFCONVERTER::Update()
{
	static INPDEVICE* keyboard=NULL;

	if (keyboard==NULL)
	if (INPMANAGER::GetInstance())		
		keyboard=INPMANAGER::GetInstance()->GetDevice(INPDEVICE_TYPE_KEYBOARD);
	
	if (keyboard)
	if (keyboard->GetButton(INPBUTTON_ID_ESCAPE)->GetState()==INPBUTTON_STATE_PRESSED)
	{
		this->SetExitStatus(XAPPLICATIONEXITTYPE_BYUSER); 
		return false;
	}

	return true;
}

/*-------------------------------------------------------------------
//	GGFCONVERTER::End
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			13/06/2017 13:20:24
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GGFCONVERTER::End()
{
return true;
}

/*-------------------------------------------------------------------
//	GGFCONVERTER::LoadObject
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			13/06/2017 17:19:09
//	
//	@return 			GRPOBJECT* : 
//
//  @param				obj : 
//  @param				path : 
//  @param				flags : 
//  @param				verboseLevel : 
//  @param				NextStackID : 
*/
/*-----------------------------------------------------------------*/
GRPOBJECT* GGFCONVERTER::LoadObject(GRPOBJECT* obj,XCHAR* path,XDWORD flags, XDWORD verboseLevel,XDWORD NextStackID)
{
	//if(!GetMainScene())		 return NULL;

	XPATH			 xpath;
	GRPFILE3D* grpfile3d	=	NULL;
	
	xpath = path; 

	if (obj==NULL)
		obj=new GRPOBJECT();
	
	grpfile3d = new GRPFILE3D();
	if(!grpfile3d) return NULL;

	task=new GGFCONVERTER_TASK(this->xconsole);
	if (!task) return NULL;

	task->ShowHeader(showheader);

	grpfile3d->SetObject				(obj);
	grpfile3d->SetScene					(GetMainScene());
	grpfile3d->SetVerboseLevel	(verboseLevel);
	grpfile3d->SetLoadFlags			(flags);
	grpfile3d->SetNextStackID		(NextStackID);
	

	
	XDEBUG_PRINTCOLOR(0,__L("Starting Load"));
	GRPOBJECT* object = grpfile3d->Load(xpath.Get());
	
	OPENGLCHECKERROR(__L("LoadObj"));
		delete grpfile3d;	
	grpfile3d=NULL;

	return object;
}


/*-------------------------------------------------------------------
//	GGFCONVERTER::PathManagement
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			14/06/2017 16:03:30
//	
//	@return 			bool : 
//
//  @param				addpath : 
*/
/*-----------------------------------------------------------------*/
bool GGFCONVERTER::PathManagement(XCHAR* addpath)
{ 
	XPATH			 xpathroot;		
	XPATH			 xpath;

	if (rootpath.IsEmpty())
	XPATHSMANAGER::GetInstance().GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, rootpath);

	xpath=rootpath;

	if (addpath)
	{	
	xpath.Slash_Add();
	xpath += addpath;
	}
	XPATHSMANAGER::GetInstance().AddPathSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpath);	

	
	XPATHSMANAGER::GetInstance().AddPathSection(XPATHSMANAGERSECTIONTYPE_GRAPHICS , __L("graphics"));
	XPATHSMANAGER::GetInstance().AddPathSection(XPATHSMANAGERSECTIONTYPE_FONTS		, __L("fonts"));
	XPATHSMANAGER::GetInstance().AddPathSection(XPATHSMANAGERSECTIONTYPE_SOUNDS		, __L("sounds"));
	
	xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1 ,__L(""));
	GRPMATERIALLIBRARY::Get()->SetResourceLocation(xpath);

	return true;
}
/*-------------------------------------------------------------------
//	GGFCONVERTER::LoadInput
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			14/06/2017 10:44:18
//	
//	@return 			bool : 
//
-------------------------------------------------------------------*/
bool GGFCONVERTER::LoadInput()
{
	GRPSCENE* scene  = GetMainScene();

	PathManagement(inputdir.Get());

	XPATH xpath;
	xpath.Create( XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1,origin.Get());
	
	XDEBUG_PRINTCOLOR(1,__L("\nLoading %s"),xpath.Get());

	XSTRING string;
	string.Format(__L("\nLoading: %s"),xpath.Get());

	if (showheader)
	xconsole->Print(string.Get());

	XTIMER* timer=xfactory->CreateTimer();
	timer->Reset();			
	object= LoadObject(object,xpath.Get(),GRPFILE3D_INCLUDE_ALL,GRPFILE3D_VERBOSE_LEVEL_ERRORS);
	if(!object)		
		{
			XDEBUG_PRINTCOLOR(4,__L("\n! Loading Failed!"));
			xconsole->Print(__L("\n! Loading Failed!"));			
			xconsole->GetChar();
			return false;
		}		
	
	string.Format(__L("\n%d milliseconds"),timer->GetMeasureMilliSeconds());
	XDEBUG_PRINTCOLOR(1,string.Get());
	if (showheader)
	xconsole->Print(string.Get());

	
		xml.SetIgnoreComments(true);

		//------------------- Open the XML
		if (!xml.Open(xpath,true))
				return false;

		if (!xml.ReadAndDecodeAllLines())
				return false;

	scene->Add(object);	
			delete(timer);

	return true;
}

/*-------------------------------------------------------------------
//	GGFCONVERTER::SaveOutput
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			14/06/2017 10:44:50
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GGFCONVERTER::SaveOutput()
{		
		if (file3d!=NULL)
		delete(file3d);

					//-------------------

		 file3d=new GRPFILE3DGGF();
		 if (file3d)
		 {
			XSTRING string;
			XTIMER* timer=xfactory->CreateTimer();		
		 timer->Reset();

		 file3d->SetObject(object);
		 file3d->SetScene	(GetMainScene());
		 file3d->Init			();
		 
		 PathManagement(output.Get());


					 //---------------------

		 XPATH	xpath,outggf,outxml,filename;

		 xpath.Create( XPATHSMANAGERSECTIONTYPE_GRAPHICS,1,__L(""));
		 
		 if (!copier->Exist(xpath.Get()))
			 if (!copier->Make(xpath))
			 {
				 xconsole->Printf(__L("Unable to create output directory! %s"),xpath.Get());
				 return false;
			 }

			 			filename.Set(origin.Get());
						filename.GetNamefile(outggf);						
						outggf+=__L(".ggf");

			  xpath.Create( XPATHSMANAGERSECTIONTYPE_GRAPHICS,1,outggf.Get());
			  file3d->Save			(xpath.Get());

					 string.Format(__L("\n\nSaving %s"),xpath.Get());
					 XDEBUG_PRINTCOLOR(1,string.Get());
					 if (showheader)
					 xconsole->Print(string.Get());


					//--------------------
		 
						filename.Set(origin.Get());
						filename.GetNamefile(outxml);						
						outxml+=__L(".xml");

		 				xpath.Create( XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1,outxml.Get());						
					
					//--------------

					 
					

					 string.Format(__L("\n %d milliseconds"),timer->GetMeasureMilliSeconds());
					 XDEBUG_PRINTCOLOR(1,string.Get());
					 if (showheader)
					 xconsole->Print(string.Get());
		 				delete(timer);
			
				
					//---------------we remove unnecessary information from the loaded xml, and add creation data on head

			if (xml.Create(xpath,XFILETXTFORMATCHAR_UTF8))
			{
				for (int e=0;e<xml.GetRoot()->GetNElements();e++)
				{
					XFILEXMLELEMENT* item=xml.GetRoot()->GetElement(e);
					

					if (item->GetName().Compare(__L("head"))==0)  //-------------------- add creation to head
					{												
						XFILEXMLELEMENT* converter=item->AddElement(__L("converter"));
						XSTRING version;
										version.Format(__L("%d.%d.%d"),GGFCONVERTER_VERSION,GGFCONVERTER_SUBVERSION,GGFCONVERTER_SUBVERSIONERR);
						converter->AddAtribute(__L("version"),version.Get());
						XSTRING datestring;
						XDATETIME* date=xfactory->CreateDateTime();
										date->GetDateTimeToString(XDATETIME_FORMAT_STANDARD,datestring);
										delete(date);
						
						converter->AddAtribute(__L("date"),datestring.Get());
					}

					if (item->GetName().Compare(__L("layers"))==0) //-------------------- remove layers an set our ggf
					{
						item->DeleteAllElements();
						XFILEXMLELEMENT* layer=item->AddElement(__L("layer"));
						//<layer file="base.FBX" id="BASE" flags="All"/>
						XSTRING baseggf;						
						filename.GetNamefile(baseggf);						
						baseggf+=__L(".ggf");
						layer->AddAtribute(__L("file"),baseggf.Get());
						layer->AddAtribute(__L("id"),__L("BASE"));
						layer->AddAtribute(__L("flags"),__L("All"));
					}
					//-------------------- remove config
					if (item->GetName().Compare(__L("config"))==0)						{						item->DeleteAllElements();					}

					//-------------------- remove states
					if (item->GetName().Compare(__L("states"))==0)						{						item->DeleteAllElements();					}

					//-------------------- remove commands
					if (item->GetName().Compare(__L("commands"))==0)					{						item->DeleteAllElements();					}	

					//-------------------- remove texts
					if (item->GetName().Compare(__L("texts"))==0)							{						item->DeleteAllElements();					}

					//-------------------- remove milestones
					if (item->GetName().Compare(__L("milestones"))==0)				{						item->DeleteAllElements();					}

					//--------------------- add dependencies, then remove
					if (item->GetName().Compare(__L("dependencies"))==0)			
					{	
						for (int d=0;d<item->GetNElements();d++)
						{
							XSTRING* value=new XSTRING();
							value->Set(item->GetElement(d)->GetValueAttribute(__L("name")));
							dependencies.Add(value);
						}
						item->DeleteAllElements();					
						
					}					
				}
					xml.WriteAndEncodeAllLines();
					
					string.Format(__L("\n\nSaving %s"),filename.Get());
					XDEBUG_PRINTCOLOR(1,string.Get());
					if (showheader)
					xconsole->Print(string.Get());
			}
			else
			{
				string.Format(__L("\nFailed saving XML output: %s"),outxml.Get());
				XDEBUG_PRINTCOLOR(4,string.Get());
				xconsole->Print(string.Get());
			}
					 if (showheader)
						 xconsole->Print(__L("\n\n\n\n"));
			return true;
		 }

		 return false;


}



/*-------------------------------------------------------------------
//	GGFCONVERTER::CopyFiles
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			15/06/2017 11:04:44
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GGFCONVERTER::CopySoundFiles()
{
	xconsole->Print(__L("\nCopying sound files\r\n"));

	XVECTOR<XSTRING*> files;
	XMAP<XSTRING*,SNDPLAYLIST*>* playlists=SNDMANAGER::Get()->GetPlaylists();

	XPATH xpath;

		 xpath.Create( XPATHSMANAGERSECTIONTYPE_SOUNDS,1,__L(""));		
		 if (!copier->Exist(xpath.Get()))
			 if (!copier->Make(xpath))
			 {
				 xconsole->Printf(__L("Unable to create output directory! %s"),xpath.Get());
				 return false;
			 }

	//-------------------------- get files used

	XDWORD n=playlists->GetSize();
	for (XDWORD k=0;k<n;k++)
	{
		SNDPLAYLIST* playlist=playlists->GetElement(k);
		XDWORD psize=playlist->GetSize();
		for(XDWORD e=0;e<psize;e++)
		{
			SNDELEMENT* sound=playlist->Get(e);			
			files.Add(sound->GetNameFile());
		}
	}

	//------------------------- process the copies

	for(XDWORD e=0;e<files.GetSize();e++)
	{
		XPATH source;						
				  PathManagement(inputdir.Get());										
					source.Create(XPATHSMANAGERSECTIONTYPE_SOUNDS, 1, files.Get(e)->Get());					

		XPATH target;
				  PathManagement(output.Get());
					target.Create( XPATHSMANAGERSECTIONTYPE_SOUNDS,1,files.Get(e)->Get());
					
	
		if (!copier->Copy(source,target))
		{
			xconsole->Printf(__L("failed copying file: %s to %s"),source.Get(),target.Get());
			return false;
		}		
	}

	files.DeleteAll();
	return true;
}


/*-------------------------------------------------------------------
//	GGFCONVERTER::CopyGraphicFiles
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			15/06/2017 14:13:52
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GGFCONVERTER::CopyGraphicFiles()
{
	xconsole->Print(__L("\nCopying graphic files\r\n"));
	XVECTOR<XSTRING*> files;
	
	XPATH xpath;

		 xpath.Create( XPATHSMANAGERSECTIONTYPE_GRAPHICS,1,__L(""));		
		 if (!copier->Exist(xpath.Get()))
			 if (!copier->Make(xpath))
			 {
				 xconsole->Printf(__L("Unable to create output directory! %s"),xpath.Get());
				 return false;
			 }

	//-------------------------- get files used

	XDWORD n=GRPMATERIALLIBRARY::Get()->GetTextures()->GetSize();
	for (XDWORD k=0;k<n;k++)
	{
		GRPTEXTURE* texture=GRPMATERIALLIBRARY::Get()->GetTextures()->GetElement(k);		
		files.Add(texture->GetFileName());		
	}

	//------------------------- process the copies

	for(XDWORD e=0;e<files.GetSize();e++)
	{
		XSTRING* texturename=files.Get(e);
		XPATH fullpath,namefile;

		fullpath = files.Get(e)->Get();
		fullpath.GetNamefileExt(namefile);
		XPATH source;
		PathManagement(inputdir.Get());
		source.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, namefile.Get());

		XPATH target;
		PathManagement(output.Get());
		target.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, namefile.Get());
	
		if (!copier->Copy(source,target))
		{
			xconsole->Printf(__L("failed copying file: %s to %s"),source.Get(),target.Get());
			return false;
		}		
	}

	return true;
	return false;
}



/*-------------------------------------------------------------------
//	GGFCONVERTER::CopyFontFiles
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			15/06/2017 14:24:50
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GGFCONVERTER::CopyFontFiles()
{
	xconsole->Print(__L("\nCopying font files\r\n"));
	XVECTOR<XSTRING*> files;
	
	XPATH xpath;

		 xpath.Create( XPATHSMANAGERSECTIONTYPE_FONTS,1,__L(""));		 
		 if (!copier->Exist(xpath.Get()))
			 if (!copier->Make(xpath))
			 {
				 xconsole->Printf(__L("Unable to create output directory! %s"),xpath.Get());
				 return false;
			 }

	//-------------------------- get files used

	XDWORD n=GRPOBJECTBUILDER::Get()->GetFontManager()->GetFonts()->GetSize();
	for (XDWORD k=0;k<n;k++)
	{
		GRPFONT* font=GRPOBJECTBUILDER::Get()->GetFontManager()->GetFonts()->GetElement(k);		
		files.Add(font->GetFile());		
	}

	//------------------------- process the copies

	for(XDWORD e=0;e<files.GetSize();e++)
	{
		XSTRING* font=files.Get(e);
		XPATH fullpath, namefile;

		fullpath = files.Get(e)->Get();
		fullpath.GetNamefileExt(namefile);
		XPATH source;
		PathManagement(inputdir.Get());
		source.Create(XPATHSMANAGERSECTIONTYPE_FONTS, 1, namefile.Get());

		XPATH target;
		PathManagement(output.Get());
		target.Create(XPATHSMANAGERSECTIONTYPE_FONTS, 1, namefile.Get());
	
		if (!copier->Copy(source,target))
		{
			xconsole->Printf(__L("failed copying file: %s to %s"),source.Get(),target.Get());
			return false;
		}		
	}

	return true;
	return false;
}

/*-------------------------------------------------------------------
//	GGFCONVERTER::CopyDependencies
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			15/06/2017 15:46:52
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GGFCONVERTER::CopyDependencies()
{	
	xconsole->Print(__L("\nCopying dependencies files\r\n"));

	//------------------------- process the copies	
	for(XDWORD e=0;e<dependencies.GetSize();e++)
	{
		XSTRING* file=dependencies.Get(e);

		XPATH source;
		PathManagement(inputdir.Get());
		source.Create(XPATHSMANAGERSECTIONTYPE_ROOT, 1, file->Get());

		XPATH target;
		PathManagement(output.Get());
		target.Create(XPATHSMANAGERSECTIONTYPE_ROOT, 1, file->Get());
						
		if (!copier->Copy(source,target))
		{
			xconsole->Printf(__L("failed copying file: %s to %s"),source.Get(),target.Get());			
			return false;
		}								
	}

	return true;
}


/*-------------------------------------------------------------------
//	GGFCONVERTER::FirstUpdate
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			13/06/2017 13:20:29
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GGFCONVERTER::FirstUpdate()
{
	if(!GetMainScene())				return NULL;

		//-------- load
	if (!LoadInput())					return false;

	//---------- Save	
	if (!SaveOutput())				return false;

	//---------- Copy Files
	if (!CopySoundFiles())		return false;
	if (!CopyGraphicFiles())	return false;
	if (!CopyFontFiles())			return false;
	if (!CopyDependencies())	return false;

	xconsole->Print(__L("\r\n\r\n ---- Finished -----\r\n\r\n"));

	this->SetExitStatus(XAPPLICATIONEXITTYPE_APPEND); 

return true;
}



/*-------------------------------------------------------------------
//	GGFCONVERTER::HandleEvent
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			13/06/2017 13:20:40
//	
//  @param				event : 
*/
/*-----------------------------------------------------------------*/
void  GGFCONVERTER::HandleEvent(XEVENT* event)
{

}

