
/*------------------------------------------------------------------------------------------
//	TESTBASECONSOLE.CPP
//	
//	Test Base Input
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 30/05/2016 16:19:29
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "XApplication.h"

#include "GRPFile3DFBX.h"
#include "GRPFile3DOBJ.h"
#include "GRPObject.h"
#include "GRPScene.h"
#include "GRPMaterialLibrary.h"
#include "GRPShaderLibrary.h"
#include "GRPObjectBuilder.h"
#include "GRPFont.h"
#include "GRPFontManager.h"
#include "GRPConsole.h"
#include "GRPRenderer.h"
#include "GRPViewport.h"
#include "GRPCamera.h"
#include "GRPVisualDebugger.h"

#include "TestBaseConsole.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/





/*-------------------------------------------------------------------
//	TESTBASECONSOLE::TESTBASECONSOLE
*/	
/**	
//	
//	Class Constructor TESTBASECONSOLE
//	
//	@author				Abraham J. Velez
//	@version			31/05/2016 10:44:17
//	
//  @param				xapplication : 
*/
/*-----------------------------------------------------------------*/
TESTBASECONSOLE::TESTBASECONSOLE(GRPAPPLICATION* xapplication) :  TESTBASE(xapplication)
{
	Clean();

	timer = xfactory->CreateTimer();
}




/*-------------------------------------------------------------------
//	TESTBASECONSOLE::~TESTBASECONSOLE
*/	
/**	
//	
//	 Class Destructor TESTBASECONSOLE
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			04/11/2015 10:54:21
//	
*/
/*-----------------------------------------------------------------*/
TESTBASECONSOLE::~TESTBASECONSOLE()
{
	if (timer)
	xfactory->DeleteTimer(timer);

	Clean();
}



/*-------------------------------------------------------------------
//	TESTBASECONSOLE::IniResources
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			27/10/2015 11:25:28
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASECONSOLE::IniResources()
{
	if(!xapplication)										 return NULL;
	if(!xapplication->GetMainScene())		 return NULL;

	GRPSCENE* scene  = xapplication->GetMainScene();

	PathManagement(__L("console"));

	XPATH xpath;
	xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1 ,__L(""));
	GRPMATERIALLIBRARY::Get()->SetResourceLocation(xpath);
	
	GRPOBJECT* obj=NULL;
	
	xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("test-cube.fbx"));
	if(!(obj = LoadObject(obj,xpath.Get())))		
		{
			XDEBUG_PRINTCOLOR(4,__L("Loading FBX Failed!"));
			return false;
		}
	scene->Add(obj);
	obj->PlayAll();
		
	XDEBUG_PRINTCOLOR(0,__L("Resources Initialized"));	
	
	

	//----------------------------------- Cargar un material		
	
	xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("txt_gen.png"));	XDEBUG_PRINTCOLOR(1,__L("%s"),xpath.Get());

	
	if(GRPMATERIALLIBRARY::Get()->Load(xpath.Get(),	__L("txt_gen.png"))!=GRPMATERIALLOADRESULT_SUCCESS)
		{
			XDEBUG_PRINTCOLOR(1,__L("Error loading Texture data"));
			return false;
		}				
	GRPMATERIAL* bkgmat=GRPMATERIALLIBRARY::Get()->Select(__L("txt_gen.png"));
	
		//----------------------------------- Cargar una fuente
	//xpath.Create(XPATHSMANAGERSECTIONTYPE_FONTS, 1, __L("Arial Black Italic.ttf"));
	//xpath.Create(XPATHSMANAGERSECTIONTYPE_FONTS, 1, __L("SourceCodePro-Regular.ttf"));
	xpath.Create(XPATHSMANAGERSECTIONTYPE_FONTS, 1, __L("Roboto-Bold.ttf"));

	GRPFONTATTRIBUTES att;
										att.back			= false;										
										att.front			= true;																	
										att.extrude		= 0.0f;
										att.monospace = false;
										att.quality		= 7;
										att.back			= false;

	if(!GRPOBJECTBUILDER::Get()->GetFontManager()->Load(xpath.Get(),		__L("Console") ,&att)) //load default
		{
			XDEBUG_PRINTCOLOR(1,__L("TTF file not loaded! arial.ttf"));
			return false;
		}

	//------------------------------------ Create Font

	GRPFONTMANAGER* fontmanager =GRPOBJECTBUILDER::Get()->GetFontManager();
	
	GRPFONT*	font = fontmanager->GetFont(__L("Console"));
	
	//----------------------------------- Crear la consola
	
	
						console = new GRPCONSOLE();						
						if(!console)	{	return false;		}

					
						console->SetScreen(xapplication->GetMainScreen());

						//console->Create();
						//console->Create(GRPCONSOLE::CREATE_PERSPECTIVE); //creates console as 3D perspective object on the scene
						console->Create(GRPCONSOLE::CREATE_FONT & GRPCONSOLE::CREATE_SCENE);
												
						console->SetBackgroundMaterial(bkgmat);
						console->SetFont(font);	
						console->SetFontScale(0.12f);													

						GRPBB bb;												
									bb.minpoint.x			=0.0f;
									bb.minpoint.y			=0.0f;
									bb.maxpoint.x			=0.5f;
									bb.maxpoint.y			=0.5f;
												
									bb.toppadding			=0.05f;
									bb.leftpadding		=0.05f;
									bb.bottompadding	=0.05f;
									bb.rightpadding		=0.05f;
												
						console->SetArea(&bb);							
						
	//InitConsole();
						/*
						console->SetColor(GRPCOLOR::WHITE);						console->Print(__L("VAjqe")); //kerning test

						//--------------- pintar linea seleccionando color								
						console->Print(GRPCOLOR::RED,									__L("Line Rojo"));		
						console->Print(GRPCOLOR::BLUE,								__L("Line Blue"));	
						console->Print(GRPCOLOR(0.2f,0.5f,0.3f,1.0f),	__L("Line Custom"));	
						console->Print(2,															__L("Line Color From Palette"));	

						//---------- kerning test
						console->SetColor(GRPCOLOR::YELLOW);					console->Print(__L("VAjqe"));		 

						//---------- long paragraph test
						console->SetColor(GRPCOLOR::WHITE);						console->Print(__L("Lorem isum dolor sit amet, \nconsectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.\n Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident,\n\n sunt in culpa qui officia deserunt mollit anim id est laborum."));
						//console->SetColor(GRPCOLOR::WHITE);						console->Print(__L("Lorem isum dolor sit amet, \nconsectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.\n Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident,\n\n sunt in culpa qui officia deserunt mollit anim id est laborum."));						
					
						//---------- scroll test
						console->Print(GRPCOLOR::RED,									__L("Press F2 to auto scroll test"));								
						console->Print(GRPCOLOR::RED,									__L("Press F3-F4 to toggle auto scroll"));								
						console->Print(GRPCOLOR::RED,									__L("Press F5 to test paragraph"));								
						console->Print(GRPCOLOR::RED,									__L("Write to test input"));								
						*/
						
						for (int e=0;e<20;e++)
						{
								XSTRING s;
								s.Format(__L("testLine%d"),e);
								console->Print(GRPCOLOR::RED,	s.Get());	
						}
												
						//console->Print(GRPCOLOR::WHITE,	__L("Start"));	

						//console->SetColor(GRPCOLOR::WHITE);						console->Print(__L("Lorem isum dolor sit amet, \nconsectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.\n\n Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."));
						//console->SetColor(GRPCOLOR::WHITE);						console->Print(__L("Lorem isum dolor sit amet, \nconsectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.\n\n Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."));						

						XSTRING name;
						xpath.GetNamefileExt(name);
						this->xapplication->GetMainScene()->Add(console);
						


	change = true;
	
	return true;
}


/*-------------------------------------------------------------------
//	TESTBASECONSOLE::EndResources
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			03/06/2016 10:29:41
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASECONSOLE::EndResources()
{
	delete(console);
	return false;
}

/*-------------------------------------------------------------------
//	TESTBASECONSOLE::Do
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			27/10/2015 11:25:39
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASECONSOLE::Do()
{
	static bool ding=false;

	if (console)
	console->Update();

	static int d=0;

	if (auto_scroll)
	if (timer->GetMeasureMilliSeconds() > 500)
	{		
		XSTRING s;s.Format(__L("Line%d"),d++);
		console->SetColor(GRPCOLOR::BLUE);
		console->Print(s.Get());		
		timer->Reset();
	}

	if (mouse)
	{
		if (!mouse->GetCursors()->Get(0)->GetX() && !mouse->GetCursors()->Get(0)->GetY())
			return TESTBASE::InputManagement();
		
			for (XDWORD e=0;e<mouse->GetButtons()->GetSize();e++)
			{
				INPBUTTON* button=mouse->GetButtons()->Get(e);
				if (button)		
				{
						XSTRING s;
						s.Set(__L("test"));
						float x=0.0f;
						float y=0.0f;

						if (mouse->GetCursors()->GetSize()>e)
						{
							x=mouse->GetCursors()->Get(e)->GetX();
							y=mouse->GetCursors()->Get(e)->GetY();
						}

						switch(button->state)
						{
						case INPBUTTON_STATE_UP: break;
						case INPBUTTON_STATE_PRESSED: 									
							s.Format(__L("Mouse PRESS %d [%f,%f]"),e,x,y);																									
							if (console)	console->Print(e,s.Get());
						break;
						case INPBUTTON_STATE_HOLD:														
							s.Format(__L("Mouse HOLD %d [%f,%f]"),e,x,y);							
							if (console)	console->Print(e,s.Get());
						break;
						case INPBUTTON_STATE_RELEASED:				
							s.Format(__L("Mouse RELEASED %d [%f,%f]"),e,x,y);							
							if (console)	console->Clear();
							if (console)	console->Print(e,s.Get());
							
						break;
						}
						//if (button->state!=INPBUTTON_STATE_UP)
								
						
				}
			}
	}
		
	return true;
}

/*-------------------------------------------------------------------
//	 TESTBASECONSOLE::InputManagement
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	26/02/2018 9:56:25
//	@return		bool : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
bool TESTBASECONSOLE::InputManagement()
{
	static float dragx=0.0f;
	static float dragy=0.0f;

	if (mouse)
	{		
		if (!mouse->GetButton(0)->IsPressed())
		{
			dragx=mouse->GetCursors()->Get(0)->GetX();
			dragy=mouse->GetCursors()->Get(0)->GetY();
		}

		if (mouse->GetButton(1)->IsPressed())
		{
			float x=mouse->GetCursors()->Get(0)->GetX();
			float y=mouse->GetCursors()->Get(0)->GetY();			

			console->SetColor(GRPCOLOR::BLUE);
			//XSTRING s;
			//s.Format(__L("Mouse %d [%f,%f]"),1,x,y);
			//console->Print(s.Get());
		}
		
		if (mouse->GetButton(0)->IsPressed())
		{
			
			float x=mouse->GetCursors()->Get(0)->GetX();
			float y=mouse->GetCursors()->Get(0)->GetY();			

			console->SetColor(GRPCOLOR::WHITE);
			//XSTRING s;
			//s.Format(__L("Mouse %d [%f,%f]"),0,x,y);
			//console->Print(s.Get());

			x/=this->xapplication->GetMainScreen()->GetWidth();
			y/=this->xapplication->GetMainScreen()->GetHeight();
			

			if (y>=0.9f)
			{
				int l=console->GetLines()->GetSize();
				
				static int delay=0;
				delay++;

				if (delay>5)
				{
					delay=0;
					if (x>0.5f)				
					console->ScrollUp();
					else
					console->ScrollDown();
				}

				return true;
			}

			if (x>=0.9f)
			{
				x-=0.5f;				

			float sc=y*4.0f;
			const GRPVECTOR* scale		=	this->console->GetNode()->GetScale();
																	this->console->GetNode()->SetScale(sc,sc,scale->vector[2]);
			return true;
			}

			if (x<=0.1f)
			{
			y-=0.5f;
		  float r=-y;
			const GRPVECTOR* rotation	=	this->console->GetNode()->GetRotation();
																	this->console->GetNode()->SetRotation(rotation->vector[0],rotation->vector[1],r*180.0f);
			return true;
			}			
		}

		if (mouse->GetButton(0)->IsPressed())
		{
				float x=mouse->GetCursors()->Get(0)->GetX();
				float y=mouse->GetCursors()->Get(0)->GetY();
				
				x/=this->xapplication->GetMainScreen()->GetWidth();
				y/=this->xapplication->GetMainScreen()->GetHeight();
							
				x-=0.5f;
				y-=0.5f;

				x*=2.0f;
				y*=2.0f*(float(this->xapplication->GetMainScreen()->GetHeight())/float(this->xapplication->GetMainScreen()->GetWidth()));
				const GRPVECTOR* position		=	this->console->GetNode()->GetPosition();
																			this->console->GetNode()->SetPosition(x,y,0.0f);
			return true;
		}
	}

	if(keyboard->GetButton(INPBUTTON_ID_ESCAPE))
	if(keyboard->GetButton(INPBUTTON_ID_ESCAPE)->IsPressedWithRelease()) 			
	{
		xapplication->SetExitStatus(XAPPLICATIONEXITTYPE_BYUSER); 
		return false;
	}

	bool mayus=false;


	if(keyboard->GetButton(INPBUTTON_ID_UP))
	if(keyboard->GetButton(INPBUTTON_ID_UP)->GetState()	==INPBUTTON_STATE_PRESSED)
	{
		console->Notify(GRPMSG_UP,NULL);		return true;
	}
	if(keyboard->GetButton(INPBUTTON_ID_DOWN))
	if(keyboard->GetButton(INPBUTTON_ID_DOWN)->GetState()==INPBUTTON_STATE_PRESSED)
	{
		console->Notify(GRPMSG_DOWN,NULL);	return true;
	}
	if(keyboard->GetButton(INPBUTTON_ID_HOME))
	if(keyboard->GetButton(INPBUTTON_ID_HOME)->GetState()==INPBUTTON_STATE_PRESSED)
	{
		console->Notify(GRPMSG_TOP,NULL);		return true;
	}
	if(keyboard->GetButton(INPBUTTON_ID_END))
	if(keyboard->GetButton(INPBUTTON_ID_END)->GetState()	==INPBUTTON_STATE_PRESSED)
	{
		console->Notify(GRPMSG_BOTTOM,NULL);return true;
	}
	

	if(keyboard->GetButton(INPBUTTON_ID_F1))
	if(keyboard->GetButton(INPBUTTON_ID_F1)->GetState()	==INPBUTTON_STATE_PRESSED)
	{
		console->Clear();
		return true;
	}

	if(keyboard->GetButton(INPBUTTON_ID_F2))
	if(keyboard->GetButton(INPBUTTON_ID_F2)->GetState()	==INPBUTTON_STATE_PRESSED)
	{
		for (int e=0;e<20;e++)
		{
			XSTRING s;
			s.Format(__L("[%d]Line"),console->GetLines()->GetSize()+e);
			if (e%2) 
				console->SetColor(GRPCOLOR::BLACK);
			else
				console->SetColor(GRPCOLOR::WHITE);
			console->Print(s.Get());
		}
		return true;
	}
	
	if(keyboard->GetButton(INPBUTTON_ID_F3))
	if(keyboard->GetButton(INPBUTTON_ID_F3)->GetState()	==INPBUTTON_STATE_PRESSED)
	{
		auto_scroll=true;
	}
	if(keyboard->GetButton(INPBUTTON_ID_F4))
	if(keyboard->GetButton(INPBUTTON_ID_F4)->GetState()	==INPBUTTON_STATE_PRESSED)
	{
		auto_scroll=false;
	}

	if(keyboard->GetButton(INPBUTTON_ID_F5))
	if(keyboard->GetButton(INPBUTTON_ID_F5)->GetState()	==INPBUTTON_STATE_PRESSED)
	{
	console->SetColor(GRPCOLOR::WHITE);						console->Print(__L("Lorem isum dolor sit amet, \nconsectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.\n Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident,\n\n sunt in culpa qui officia deserunt mollit anim id est laborum."));
	return true;
	}
						
	if(keyboard->GetButton(INPBUTTON_ID_F8))
	if(keyboard->GetButton(INPBUTTON_ID_F8)->GetState()	==INPBUTTON_STATE_PRESSED)
	{
			for (XDWORD e=0;e<console->GetLines()->GetSize();e++)
			{
				XDEBUG_PRINTCOLOR(0,console->GetLines()->Get(e)->Get());
			}
	}

	if(keyboard->GetButton(INPBUTTON_ID_SHIFT_LEFT))
	if(keyboard->GetButton(INPBUTTON_ID_SHIFT_LEFT)->GetState()==INPBUTTON_STATE_HOLD)
	{
		mayus=true;
	}

	if(keyboard->GetButton(INPBUTTON_ID_SHIFT_RIGHT))
	if(keyboard->GetButton(INPBUTTON_ID_SHIFT_RIGHT)->GetState()==INPBUTTON_STATE_HOLD)
	{
		mayus=true;
	}

	for (XDWORD e=0;e<keyboard->GetButtons()->GetSize();e++)
	if(keyboard->GetButton(e))
	if(keyboard->GetButton(e)->GetState()==INPBUTTON_STATE_PRESSED) 		
	{		
		XSTRING s;
		XCHAR c=keyboard->GetButton(e)->GetSymbol();				s.Set(c);		

		if (!mayus)
			s.ToLowerCase();

		if (console)
			console->Add(s.Get());			
	}

	return TESTBASE::InputManagement();
}
