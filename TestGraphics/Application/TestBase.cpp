
/*------------------------------------------------------------------------------------------
//	TESTBASE.CPP
//	
//	Test BASE + Test Graphics functions 
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 30/04/2015 13:14:35
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XRand.h"
#include "XPath.h"
#include "XDebug.h"
#include "XApplication.h"
#include "XThreadCollected.h"

#include "INPManager.h"

#include "GRPFile3DFBX.h"
#include "GRPFile3DOBJ.h"
#include "GRPObject.h"
#include "GRPScene.h"
#include "GRPMaterialLibrary.h"
#include "GRPShaderLibrary.h"

#include "GRPFontManager.h"

#include "GRPScreen.h"
#include "GRPCamera.h"
#include "GRPProfiler.h"

#include "TestGraphics.h"
#include "TestBase.h"

#include "XMemory.h"


/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//	TESTBASE::TESTBASE
*/	
/**	
//	
//	Class Constructor TESTBASE
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			01/09/2016 12:50:55
//	
//  @param				xapplication : 
*/
/*-----------------------------------------------------------------*/
TESTBASE::TESTBASE(GRPAPPLICATION* xapplication)				
{	
	Clean();		

	GRPPROFILER::showFrameEvery = 1;
																
	this->xapplication	= xapplication;

	if(!keyboard) keyboard = INPMANAGER::GetInstance()->GetDevice(INPDEVICE_TYPE_KEYBOARD);	
	if(!mouse)		mouse		 = INPMANAGER::GetInstance()->GetDevice(INPDEVICE_TYPE_MOUSE);		
}




/*-------------------------------------------------------------------
//	TESTBASE::PathManagement
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			30/05/2016 22:12:55
//	
//	@return 			bool : 
//
//  @param				addpath : 
*/
/*-----------------------------------------------------------------*/
bool TESTBASE::PathManagement(XCHAR* addpath)
{ 
	XPATH			 xpathroot;		
	XPATH			 xpath;

	if (rootpath.IsEmpty())
	XPATHSMANAGER::GetInstance().GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, rootpath);

	xpath=rootpath;
	xpath.Slash_Add();
	xpath += addpath;

	XPATHSMANAGER::GetInstance().AddPathSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpath);	
		
	XPATHSMANAGER::GetInstance().AddPathSection(XPATHSMANAGERSECTIONTYPE_GRAPHICS , __L("graphics"));
	XPATHSMANAGER::GetInstance().AddPathSection(XPATHSMANAGERSECTIONTYPE_FONTS		, __L("fonts"));
	XPATHSMANAGER::GetInstance().AddPathSection(XPATHSMANAGERSECTIONTYPE_SOUNDS		, __L("sounds"));
	
	xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1 ,__L(""));
	GRPMATERIALLIBRARY::Get()->SetResourceLocation(xpath);

	return true;
}




/*-------------------------------------------------------------------
//	TESTBASE::InputManagement
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			30/05/2016 17:48:41
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASE::InputManagement()
{ 
	keyboard = INPMANAGER::GetInstance()->GetDevice(INPDEVICE_TYPE_KEYBOARD);
	if(!keyboard) return false;

	mouse = INPMANAGER::GetInstance()->GetDevice(INPDEVICE_TYPE_MOUSE);
	if (!mouse) mouse = INPMANAGER::GetInstance()->GetDevice(INPDEVICE_TYPE_TOUCHSCREEN);
	if (!mouse) return false;


	if(keyboard->GetButton(INPBUTTON_ID_ESCAPE))
	if(keyboard->GetButton(INPBUTTON_ID_ESCAPE)->GetState()==INPBUTTON_STATE_RELEASED) 			
	xapplication->SetExitStatus(XAPPLICATIONEXITTYPE_BYUSER); 

	if(keyboard->GetButton(INPBUTTON_ID_F1))												
	if(keyboard->GetButton(INPBUTTON_ID_F1)->GetState()==INPBUTTON_STATE_RELEASED) 			
	xapplication->GetMainScene()->ShowDebug(GRPDEBUG_FLAG_NORMAL);

	if(keyboard->GetButton(INPBUTTON_ID_F2))
	if(keyboard->GetButton(INPBUTTON_ID_F2)->GetState()==INPBUTTON_STATE_RELEASED) 			
	xapplication->GetMainScene()->ShowDebug(GRPDEBUG_FLAG_BOTH);	

	if(keyboard->GetButton(INPBUTTON_ID_F3))
	if(keyboard->GetButton(INPBUTTON_ID_F3)->GetState()==INPBUTTON_STATE_RELEASED) 			
	xapplication->GetMainScene()->ShowDebug(GRPDEBUG_FLAG_DEBUG);	

	float speed=1.0f;

	if(keyboard->GetButton(INPBUTTON_ID_UP))
		{
			if(keyboard->GetButton(INPBUTTON_ID_UP)->GetState()==INPBUTTON_STATE_HOLD) 			
				{
					GRPCAMERA* cam		  =	xapplication->GetMainScene()->GetCamera();
					const GRPVECTOR* v	= cam->GetPosition();
			
					cam->SetPosition(v->vector[0],v->vector[1]+speed,v->vector[2]);		
				}
		}

	if(keyboard->GetButton(INPBUTTON_ID_DOWN))
		{
			if(keyboard->GetButton(INPBUTTON_ID_DOWN)->GetState()==INPBUTTON_STATE_HOLD) 			
				{
					GRPCAMERA*			 cam	=	xapplication->GetMainScene()->GetCamera();
					const GRPVECTOR* v		=	cam->GetPosition();

					cam->SetPosition(v->vector[0],v->vector[1]-speed,v->vector[2]);
				}
		}

	if(keyboard->GetButton(INPBUTTON_ID_RIGHT))
		{
			if(keyboard->GetButton(INPBUTTON_ID_RIGHT)->GetState()==INPBUTTON_STATE_HOLD) 			
				{
					GRPCAMERA*			 cam = xapplication->GetMainScene()->GetCamera();
					const GRPVECTOR* v   = cam->GetPosition();

					cam->SetPosition(v->vector[0]+speed,v->vector[1],v->vector[2]);
				}
		}

	if(keyboard->GetButton(INPBUTTON_ID_LEFT))
		{
			if(keyboard->GetButton(INPBUTTON_ID_LEFT)->GetState()==INPBUTTON_STATE_HOLD) 			
				{
					GRPCAMERA*			 cam	=	xapplication->GetMainScene()->GetCamera();
					const GRPVECTOR* v		=	cam->GetPosition();

					cam->SetPosition(v->vector[0]-speed,v->vector[1],v->vector[2]);
				}
		}

	if(keyboard->GetButton(INPBUTTON_ID_PAGE_UP))
		{
			if(keyboard->GetButton(INPBUTTON_ID_PAGE_UP)->GetState()==INPBUTTON_STATE_HOLD) 			
				{
					GRPCAMERA*			 cam = xapplication->GetMainScene()->GetCamera();
					const GRPVECTOR* v	 = cam->GetPosition();

					cam->SetPosition(v->vector[0],v->vector[1],v->vector[2]+speed);
				}
		}

	if(keyboard->GetButton(INPBUTTON_ID_PAGE_DOWN))
		{
			if(keyboard->GetButton(INPBUTTON_ID_PAGE_DOWN)->GetState()==INPBUTTON_STATE_HOLD) 			
				{
					GRPCAMERA*			 cam = xapplication->GetMainScene()->GetCamera();
					const GRPVECTOR* v   = cam->GetPosition();
		
					cam->SetPosition(v->vector[0],v->vector[1],v->vector[2]-speed);
				}
		}

	static const GRPVECTOR oldposition=*xapplication->GetMainScene()->GetCamera()->GetPosition();
	
	static GRPVECTOR lastmouse;

	if(!lockcamera)
		{
			if(mouse && (mouse->GetButton(2)->IsPressed() || mouse->GetButton(1)->IsPressed() || mouse->GetButton(0)->IsPressed()))
				{
					float x = mouse->GetCursor(0)->GetX();
					float y = mouse->GetCursor(0)->GetY();
					float z	=	mouse->GetCursor(0)->GetZ();

					lastmouse.vector[0]=x;
					lastmouse.vector[1]=y;
					lastmouse.vector[2]=z;	

					float rx = (x) / xapplication->GetMainScreen()->GetWidth();
					float ry=  (y) / xapplication->GetMainScreen()->GetHeight();

					rx*=2.0f;	rx-=1.0f;
					ry*=2.0f;	ry-=1.0f;

					GRPCAMERA*			 cam =	xapplication->GetMainScene()->GetCamera();
					const GRPVECTOR* v   = cam->GetPosition();
		
					if(!(x==0.0f && y==0.0f))
						{
							const GRPVECTOR* rot=cam->GetRotation();
				
							if (cam->GetCameraType()==GRPCAMERA_TYPE_TARGET)
								  	cam->Orbit(rx,ry);
							 else cam->SetRotation(ry/2.0f+rot->vector[0],0.0f,-rx/2.0f+rot->vector[2]);
						}
			  }
		}

	return true;							
}




/*-------------------------------------------------------------------
//	TESTBASE::LoadObject
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			30/04/2015 16:25:04
//	
//	@return 			GRPOBJECT* : 
//
//  @param				path : 
*/
/*-----------------------------------------------------------------*/
GRPOBJECT* TESTBASE::LoadObject(GRPOBJECT* obj,XCHAR* path,XDWORD loadflags,XDWORD verbose,XDWORD NextStackID)
{
	if(!xapplication)										 return NULL;
	if(!xapplication->GetMainScene())		 return NULL;

	XPATH			 xpath;
	GRPFILE3D* grpfile3d	=	NULL;
	
	xpath = path; 

	bool ownedobj=false;
	if (obj==NULL)
	{
		obj=new GRPOBJECT();
		ownedobj=true;
	}
	
	grpfile3d = new GRPFILE3D();
	if(!grpfile3d) return NULL;

	grpfile3d->SetObject				(obj);
	grpfile3d->SetScene					(xapplication->GetMainScene());
	grpfile3d->SetVerboseLevel	(verbose);
	grpfile3d->SetLoadFlags			(loadflags);
	grpfile3d->SetNextStackID		(NextStackID);

	XDEBUG_PRINTCOLOR(0,__L("Starting Load"));
	GRPOBJECT* object = grpfile3d->Load(xpath.Get());
	OPENGLCHECKERROR(__L("LoadObj"));
	if(!object) 
		{			
			delete(grpfile3d);
			grpfile3d=NULL;			
			return NULL;	
		}	

	delete grpfile3d;	

	return object;
}



/*-------------------------------------------------------------------
//	TESTBASE::InitConsole
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			03/03/2017 13:09:57
//	
*/
/*-----------------------------------------------------------------*/
void TESTBASE::InitConsole(float size)
{
	console = new GRPCONSOLE();	
	if(!console)	
		{	
			return;		
		}												

	console->SetScreen(xapplication->GetMainScreen());

 	if(!console->Create(GRPCONSOLE::CREATE_FONT & GRPCONSOLE::CREATE_SCENE))
		{
			delete(console);
			console=NULL;
	
			return;
		}

	console->SetFontScale(size);													

	GRPBB bb;												
	
	bb.minpoint.x			=0.0f;
	bb.minpoint.y			=0.0f;
	bb.maxpoint.x			=1.0f;
	bb.maxpoint.y			=1.0f;
												
	bb.toppadding			=0.05f;
	bb.leftpadding		=0.05f;
	bb.bottompadding	=0.05f;
	bb.rightpadding		=0.05f;
												
	console->SetArea(&bb);							
												
	XSTRING id;	id.Format(__L("%s %d.%d.%d"),	TESTGRAPHICS_NAMEAPP,TESTGRAPHICS_VERSION,TESTGRAPHICS_SUBVERSION,TESTGRAPHICS_SUBVERSIONERR);
	console->Print(0,id.Get());
	
	xapplication->GetMainScene()->Add(console);
				
	return;
}

		
