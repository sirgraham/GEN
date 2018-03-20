
/*------------------------------------------------------------------------------------------
//	GRPCONSOLE.CPP
//	
//	Graphic console implementation
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 09/10/2015 13:11:26
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XThread.h"

#include "GRPConsole.h"
#include "GRPMaterial.h"
#include "GRPMaterialLibrary.h"
#include "GRPShaderLibrary.h"
#include "GRPScene.h"
#include "GRPRenderer.h"
#include "GRPViewport.h"
#include "GRPFontManager.h"
#include "GRPFont.h"
#include "GRPFileTTF.h"
#include "GRPCamera.h"
#include "GRPScreen.h"

#include "GRPBlendingPass.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/




/*-------------------------------------------------------------------
//	GRPCONSOLE::GRPCONSOLE
*/	
/**	
//	
//	Class Constructor GRPCONSOLE
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/03/2017 10:56:36
//	
*/
/*-----------------------------------------------------------------*/
GRPCONSOLE::GRPCONSOLE() : XCONSOLE()
{
		this->Clean();


		scene = NULL;
}

/*-------------------------------------------------------------------
//	GRPCONSOLE::CARET::CARET
*/	
/**	
//	
//	Class Constructor CARET
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/03/2017 10:56:36
//	
*/
/*-----------------------------------------------------------------*/
GRPCONSOLE::CARET::CARET()
{
	Clean(); 
}

/*-------------------------------------------------------------------
//	GRPCONSOLE::CARET::Clean
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			21/03/2017 13:38:43
//	
*/
/*-----------------------------------------------------------------*/
void GRPCONSOLE::CARET::Clean()
{
	color=GRPCOLOR::WHITE;
	size=0.12f;
	font=NULL;
	lastpos.x			= 0.0f; lastpos.y			= 0.0f;lastpos.z			= 0.0f;
	textposition.x= 0.0f; textposition.y= 0.0f;textposition.z = 0.0f;
	character			= __C('_');
	blinking			= false;
	visible				= false;
}
				

GRPCONSOLE::PRINTABLE::PRINTABLE(XCHAR* str,GRPCOLOR color,float size, GRPFONT* font)	 	
{	
		this->color	=color;
		this->str		=NULL;		
		this->font	=font;	
		this->size	=size;	
		if (str)
		{
			this->str=new XSTRING();
			this->str->Set(str);
		}
}

GRPCONSOLE::PRINTABLE::~PRINTABLE()
{
		if (str)	delete(str);
}
			


/*-------------------------------------------------------------------
//	GRPCONSOLE::SetScreen
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/03/2017 12:34:20
//	
//  @param				screen : 
*/
/*-----------------------------------------------------------------*/
void GRPCONSOLE::SetScreen(GRPSCREEN* screen)
{
	this->screen=screen;
}

GRPCONSOLE::GRPCONSOLE( GRPSCREEN* screen)
{
	this->Clean();
	scene = NULL;
	SetScreen(screen);
}

GRPCONSOLE::GRPCONSOLE(GRPSCENE* scene,GRPSCREEN* screen)
{
	this->Clean();	
	SetScreen(screen);
	SetScene(scene);
}


/*-------------------------------------------------------------------
//	GRPCONSOLE::GRPCONSOLE
*/	
/**	
//	
//	Class Constructor GRPCONSOLE
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			14/10/2015 11:50:23
//	
//  @param				scenein : 
*/
/*-----------------------------------------------------------------*/
GRPCONSOLE::GRPCONSOLE( GRPSCENE* scenein) : XCONSOLE()
{
	Clean();
	
	scene = scenein;
}



/*-------------------------------------------------------------------
//	GRPCONSOLE::~GRPCONSOLE
*/	
/**	
//	
//	 Class Destructor GRPCONSOLE
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			14/10/2015 11:50:29
//	
*/
/*-----------------------------------------------------------------*/
GRPCONSOLE::~GRPCONSOLE()
{
	if (scene)
		if (scene->GetScreen())
	UnSubscribeEvent(GRPXEVENTTYPE_SCREEN_CHANGESIZE, scene->GetScreen());

	Destroy();	

	if(xmutex) 
	{
		xfactory->Delete_Mutex(xmutex);
		xmutex=NULL;
	}
	colors.DeleteElementContents();
	colors.DeleteAll();

	attachments.DeleteContentsInstanced();
	attachments.DeleteAll();	

	elements.DeleteAll();

	if (this->scene && this->ownsScene)
	{
		this->scene->GetObjects()->DeleteAll();
	}

	//linenodes.DeleteContents();
	//linenodes.DeleteAll();
}


/*-------------------------------------------------------------------
//	GRPCONSOLE::Destroy
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/03/2017 11:13:41
//	
*/
/*-----------------------------------------------------------------*/
void GRPCONSOLE::Destroy()
{
	
	xmutex->Lock();
	caret.lastpos.y = 0;		
	line=0;

	scene->Lock();
	for (XDWORD e=0;e<this->lineelements.GetSize();e++)
	{
		this->scene->RemoveFromRenderOrder(	this->lineelements.Get(e));
	}
	
	this->scene->Lock();

	for (XDWORD e=0;e<this->lineelements.GetSize();e++)
	{
			delete(this->lineelements.Get(e)->GetNode());			
	}	
	lineelements.DeleteContents();

	toprintvector.DeleteContents();
	toprintvector.DeleteAll();

	this->scene->UnLock();
	xmutex->UnLock();
	
}



/*-------------------------------------------------------------------
//	GRPCONSOLE::Create
*/	
/**	
//	
//	cterated the console on debug
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			27/10/2015 9:21:17
//	
//  @param				debug : 
*/
/*-----------------------------------------------------------------*/
bool GRPCONSOLE::Create(XDWORD flags)
{
	caret.color = GRPCOLOR::WHITE;

	if (!screen) 			
			return false;
	

	InitDefaultColors();
	
	xmutex = xfactory->Create_Mutex();
	if(!xmutex) return false;

	//--------------------------- font
	if (!font || (flags & CREATE_FONT))
	{
			XPATH xpath;
			xpath.Create(XPATHSMANAGERSECTIONTYPE_FONTS, 1, __L("arial.ttf"));

			GRPFONTATTRIBUTES att;
												att.extrude		= 0;
												att.monospace = true;
												att.quality		= 3;
												att.back			= false;

			GRPFONTMANAGER* fontmanager =GRPOBJECTBUILDER::Get()->GetFontManager();						
			if(!fontmanager->Load(xpath.Get(),		__L("Console") ,&att)) //load default
				{
					XDEBUG_PRINTCOLOR(1,__L("TTF file not loaded! arial.ttf"));
					return false;
				}

			font = fontmanager->GetFont(__L("Console"));
		
			ownsFont=true;
	}

	//--------------------------- scene
	if ((!scene && screen) || (flags & CREATE_SCENE))
	{
		scene=new GRPSCENE(this->screen);
		if (!scene)
			return false;

		this->ownsScene=true;
		scene->SetName(__L("Console"));

		scene->Ini();		

		GRPBLENDINGPASS* pass=new GRPBLENDINGPASS();										 
		pass->SetBlending(true,GRPBLENDINGMODE_ALPHABLEND);
		scene->Add(pass);
		
		if (!(flags & CREATE_PERSPECTIVE))
		{
		scene->GetCamera()->SetPosition(0.0f, 0.0f, 150.0f);
		scene->GetCamera()->GetTarget()->SetPosition(0.0f, 0.0f, 0.0f);
		scene->GetPass()->SetClear(GRPPASS_CLEAR_FLAG_NONE);
		scene->GetPass()->SetCamera(scene->GetCamera());


			
	GRPVIEWPORT*	viewport = scene->GetViewport();
								viewport->SetSize(1.0f, 1.0f);
								viewport->SetPosition(0.0f, 0.0f);
								viewport->SetProjection(GRPPROJECTIONTYPE_ORTHO);
								viewport->SelectProjectionOrtho();								
								viewport->Select();			
		}
								
	}
	
	if (!(flags & CREATE_PERSPECTIVE))
	{
	//--------------------------- ortho settings
	scene->GetCamera()->GetTarget()->SetPosition(0.0f, 0.0f, 0.0f);;
	scene->GetCamera()->SetPosition(0.0f, 0.0f, 150.0f);

	scene->GetViewport()->SetProjection(GRPPROJECTIONTYPE_ORTHO);
	scene->GetViewport()->SelectProjectionOrtho();
	}
	
	scene->Add(this);
	SetNode(scene->CreateNode());
	SubscribeEvent(GRPXEVENTTYPE_SCREEN_CHANGESIZE, scene->GetScreen());
	
	//---------------------------- elements
	if (flags & CREATE_BACKGROUND)
	if (!CreateBackgroundPlane())
		return false;



	//--------------------------- text
	textparent = scene->CreateNode();
	textparent->SetPosition	(0.0f, 0.0f, 0.0f);
	textparent->SetScale		(1.0f, 1.0f, 1.0f);
	textparent->LinkTo			(this->GetNode());

	this->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"STX"));
	this->Maximize();
	
	this->SetTextProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"SDF"));

	initialized=true;
	return true;
}

/*-------------------------------------------------------------------
//	GRPCONSOLE::InitDefaultColors
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			06/03/2017 9:49:25
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPCONSOLE::InitDefaultColors()
{
	GRPCOLOR* color=new GRPCOLOR(GRPCOLOR::WHITE);
	if	(!colors.Set(XDEBUG_COLORBLACK,	color))
				colors.Add(XDEBUG_COLORBLACK,	color);

				color=new GRPCOLOR(GRPCOLOR(0.5f,0.5f,1.0f));
	if	(!colors.Set(XDEBUG_COLORBLUE,	color))
				colors.Add(XDEBUG_COLORBLUE,	color);

				color=new GRPCOLOR(GRPCOLOR(0.5f,1.0f,0.5f));
	if	(!colors.Set(XDEBUG_COLORGREEN,	color))
				colors.Add(XDEBUG_COLORGREEN,	color);

				color=new GRPCOLOR(GRPCOLOR::PURPLE);
	if	(!colors.Set(XDEBUG_COLORPURPLE,color))
				colors.Add(XDEBUG_COLORPURPLE,color);

				color=new GRPCOLOR(GRPCOLOR(1.0f,0.5f,0.5f));
	if	(!colors.Set(XDEBUG_COLORRED,color))
				colors.Add(XDEBUG_COLORRED,color);

				color=new GRPCOLOR(GRPCOLOR::GRAY);
	if	(!colors.Set(XDEBUG_COLORGRAY,color))
				colors.Add(XDEBUG_COLORGRAY,color);

	return true;
}


/*-------------------------------------------------------------------
//	GRPCONSOLE::CreateBackgroundPlane
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/03/2017 11:14:42
//	
*/
/*-----------------------------------------------------------------*/
bool GRPCONSOLE::CreateBackgroundPlane()
{
	if (!scene)	return false;

	background	= scene->CreateNode();	
				plane = GRPOBJECTBUILDER::Get()->CreatePlane(GRPOBJECTBUILDER::AXIS_X);				if	(!plane)	return false;
			

	background->SetParent(this->node);
												plane->SetNode(background);
												plane->SetName(__L("Console Background"));
												plane->GetNode()->SetName(__L("Console Background"));	
	GetAttachments()->Add(plane);
	
	AddBackground(plane);
								plane->UpdateBoundingBox();
								plane->GetNode()->SetPosition	(	0.0f, 0.0f, -0.001f	);
								plane->GetNode()->SetScale		(	1.0f, 
																								1.0f/scene->GetViewport()->GetAspectRatio(), 
																								1.0f	);
								plane->GetNode()->SetRotation	(	0.0f, 0.0f, 0.0f	);
								plane->SetVisible		(true);
								plane->SetVisibility(1.0f);
								plane->SetProgram	(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"STX"));

								

								if (background_material)
								{
									plane->GetNode()->SetMaterial(background_material);
									plane->GetNode()->GetMaterial()->GetAttributes()->SetTransparency(0.5f);
								}

	this->scene->Add(plane);

	static_cast<GRPBLENDINGPASS*>(this->scene->GetPasses()->GetLast())->GetBlendedObjects();

	return true;
}

/*-------------------------------------------------------------------
//	GRPCONSOLE::Maximize
*/	
/**	
//	
//	maximizes the console to the window
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			14/10/2015 12:26:22
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPCONSOLE::Maximize()
{
	//this->SetFontScale(0.5f);
	this->GetNode()->SetScale(1.0f, 1.0f, 1.0f);
	this->SetArea(0.0f, 0.0f, 1.0f, 1.0f);
	this->GetNode()->SetPosition(0.0f, 0.0f, 0.0f);
	return true;
}




/*-------------------------------------------------------------------
//	GRPCONSOLE::Resize
*/	
/**	
//	
//	resizes the console according to new screen resolution
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			18/11/2015 10:15:20
//	
*/
/*-----------------------------------------------------------------*/
void GRPCONSOLE::Resize()
{
//	SetArea(xs, ys, xe, ye);
	SetArea(area.minpoint.x, area.minpoint.y, area.maxpoint.x, area.maxpoint.y);
	if (plane)
	plane->GetNode()->SetScale(1.0f, 1.0f/scene->GetViewport()->GetAspectRatio(), 1.0f);
	caret.lastpos.y = 0;
	
	for(XDWORD i = 0; i < lineelements.GetSize(); i++)
	{
		elements.Delete(lineelements.Get(i));
		lineelements.Get(i)->GetNode()->Dettach();
		delete lineelements.Get(i)->GetNode();
		delete lineelements.Get(i);
	}

	lineelements.DeleteAll();
}





/*-------------------------------------------------------------------
//	GRPCONSOLE::Update
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			14/06/2016 13:52:44
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPCONSOLE::Update()
{
	if(!xmutex)				return false;
	if(!initialized)	return false;

		xmutex->Lock();

	XDWORD size = toprintvector.GetSize();

	if (scroll_start!=current_scroll)
	{
		ScrollLines();
	}

	for(XDWORD i = 0; i < size; i++)
		{
			GRPCONSOLE::PRINTABLE* ptr = toprintvector.FastGet(i);
			
			this->SetColor(ptr->color);
			this->PrintImp(ptr->str->Get());
			
			delete ptr;
		}

	toprintvector.DeleteAll();
	xmutex->UnLock();

	return true;
}




/*-------------------------------------------------------------------
//	GRPCONSOLE::PrintImp
*/	
/**	
//	
//	prints an already formatted string
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			14/10/2015 11:51:05
//	
//	@return 			bool : 
//
//  @param				string : */
/*-----------------------------------------------------------------*/

bool GRPCONSOLE::PrintImp(XCHAR* string)
{
	scene->Lock();
	//------------------------------------------ create material if color not present
	GRPMATERIAL*			material=NULL;

	material=materials.Get(caret.color);

	if(material == NULL)
		{
			material = new GRPMATERIAL();				if(!material)	return false;
			material->GetAttributes()->SetDiffuse(caret.color);
			material->GetAttributes()->isupdated = true;						
			materials.Add(caret.color, material);

			GRPMATERIALLIBRARY::Get()->Add(material);
		}
	
	//------------------------------------------- Create the textbox

			GRPTEXTELEMENT* text = new GRPTEXTELEMENT			();			if(!text)	return false;					
											text->SetAlign								(GRPARRAYALIGNTOOL_ALIGNFLAGS_NONE);																									
											text->SetNode									(scene->CreateNode());
											text->SetFont									(font);
											
											text->SetProgram							(shaderprogram);
											text->SetContent							(string);

											text->GetNode()->SetMaterial	(material);
											text->GetNode()->GetBaseMaterial()->SetDiffuse(caret.color);
											text->GetNode()->GetMaterial()->GetAttributes()->SetDiffuse(caret.color);

											//text->GetNode()->SetScale			(1.0f,				1.0f,							1.0f);
											text->GetNode()->SetScale			(caret.size,	caret.size,				1.0f);
											//text->GetNode()->SetPosition	(0.0f,				0.0f,							0.0f);	
											text->GetNode()->SetPosition	(0.0f,				-(lineelements.GetSize()*line_height),							0.0f);	
											text->GetNode()->SetParent		(this->textparent);
											text->UpdateContent();
									
											/*
											if ((line-scroll_start) < scroll_size)
												text->GetNode()->SetParent				(linenodes.Get(line-scroll_start));
											else
											{
												text->GetNode()->Dettach();
												text->SetVisibility(0.0f);
												text->SetVisible(false);
											}
											*/

											if (!this->scene->Add(text))
												delete text;
											else
											if (!this->lineelements.Add(text))
												delete(text);
						
	  if (auto_scroll)
		ScrollBottom();					
				line++;					

				scene->UnLock();
	return true;
}

/*-------------------------------------------------------------------
//	GRPCONSOLE::Notify
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			06/03/2017 10:20:13
//	
//	@return 			bool : 
//
//  @param				type : 
//  @param				subject : 
*/
/*-----------------------------------------------------------------*/
bool GRPCONSOLE::Notify(GRPMSGTYPE type,GRPSUBJECT* subject)
{
	switch(type)
	{
	case GRPMSG_ADDED:   //---------------------------------------- added to scene
		if (subject->GetSubjectType()==GRPNAMED_TYPE_GRPSCENE)
		{
			GRPSCENE* scene=dynamic_cast<GRPSCENE*>(subject);
			if (this->scene!=scene && scene && screen)
			{
				scene->GetRenderer()->AddScene(this->scene);
				return false;
			}
		}
		break;

	case GRPMSG_UP:				ScrollUp		();	break;
	case GRPMSG_DOWN:			ScrollDown	();	break;
	case GRPMSG_TOP:			ScrollTop		();		break;
	case GRPMSG_BOTTOM:		ScrollBottom();		break;
	}
	return true;
}
/*-------------------------------------------------------------------
//	GRPCONSOLE::DeleteLine
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			20/03/2017 17:35:54
//	
//  @param				line : 
*/
/*-----------------------------------------------------------------*/
void	GRPCONSOLE::DeleteLine(int line)
{
	
	/*
	this->scene->Lock();
	GRPELEMENT* element=this->lineelements.GetLast();
	GRPNODE* node=element->GetNode();
	this->scene->RemoveFromRenderOrder(element);			
	lines.Delete(lines.GetLast());
	delete(this->lineelements.GetLast());	
	this->scene->UnLock();

	this->scene->RemoveFromRenderOrder();		
	this->lineelements.Delete(this->lineelements.GetLast());
	delete(this->lineelements.GetLast());
	//this->lineelements.DeleteIndex(this->lineelements.GetSize()-1);			
	lines.Delete(lines.GetLast());
	
	if (lines.GetSize()>0)
	{
		caret.color=lines.GetLast()->color;	
		caret.lastpos.y=lines.GetLast()->lastpos.y;
	}
	
	this->scene->UnLock();
	ScrollLines();
	*/
}

/*-------------------------------------------------------------------
//	GRPCONSOLE::SetProgram
*/	
/**	
//	
//	sets the shader program to use
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			15/10/2015 11:03:34
//	
//  @param				program : 
*/
/*-----------------------------------------------------------------*/
void GRPCONSOLE::SetTextProgram(GRPSHADERPROGRAM* program)
{
	shaderprogram = program;
}




/*-------------------------------------------------------------------
//	GRPCONSOLE::SetFont
*/	
/**	
//	
//	sets the font to use
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			15/10/2015 12:53:51
//	
//  @param				font : 
*/
/*-----------------------------------------------------------------*/
void GRPCONSOLE::SetFont(GRPFONT* font)
{
	this->font = font;
}





/*-------------------------------------------------------------------
//	GRPCONSOLE::Print
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			14/06/2016 13:59:55
//	
//	@return 			bool : 
//
//  @param				str : 
*/
/*-----------------------------------------------------------------*/
bool GRPCONSOLE::Print(XCHAR* string)
{
	if (!initialized)	return false;
	if (!xmutex)			return false;

	
	xmutex->Lock();
	
		XSTRING original,trimmed,rest;
		original.Set(string);
	
		float width=line_width*2.0f;
	
		this->font->MeasureWidth(width/caret.size,original,trimmed,rest);		
		GRPCONSOLE::PRINTABLE*	ptr = new GRPCONSOLE::PRINTABLE(trimmed.Get(),caret.color,caret.size,this->font);	if(!ptr) return false;																											
		toprintvector.Add(ptr);

		/*
		XSTRING* text;
		text=new XSTRING();
		text->Set(*ptr->str);
		this->lines.Add(text);
		*/
					
		if (rest.GetSize())
		{
				if (rest.Get()[0]==__C('\n'))
					rest.DeleteCharacters(0,1);
			Print(rest.Get());
		}


		
	Publish(GRPMSG_ADDED);
	xmutex->UnLock();

	return true;
}

/*-------------------------------------------------------------------
//	GRPCONSOLE::Print
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/03/2017 15:46:47
//	
//	@return 			bool : 
//
//  @param				color : 
//  @param				str : 
*/
/*-----------------------------------------------------------------*/
bool GRPCONSOLE::Print(XDWORD color, XCHAR* str)
{
	if (colors.Find(color)!=NOTFOUND)
		SetColor(*colors.Get(color));
	return Print(str);
}

bool GRPCONSOLE::Print(GRPCOLOR color, XCHAR* str)
{
	SetColor(color);
	return Print(str);
}

/*-------------------------------------------------------------------
//	GRPCONSOLE::ScrollLines
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			20/03/2017 11:29:39
//	
//	@return 			bool : 
//
//  @param				lines : 
*/
/*-----------------------------------------------------------------*/
bool GRPCONSOLE::ScrollLines()
{	
	if (scroll_start<0)														scroll_start=0;
	
	scroll_end=(scroll_start+scroll_size);	

	const GRPVECTOR* tpv=textparent->GetPosition();

	for (int e=0;e<(int)lineelements.GetSize();e++)
	{
		float linepos=0.0f;
		linepos=e*line_height-(scroll_start*line_height);
		lineelements.Get(e)->GetNode()->SetPosition(0.0f,-linepos,0.0f);
		
		lineelements.Get(e)->SetVisible(true);

		if (e<scroll_start)
			lineelements.Get(e)->SetVisible(false);

		if (e>=scroll_end)
			lineelements.Get(e)->SetVisible(false);
		/*
		if (linepos>(tpv->vector[1]+(scroll_size*line_height)))
			lineelements.Get(e)->SetVisible(false);
			*/
	}

	/*
	if (scroll_start<0)														scroll_start=0;
	
	scroll_end=scroll_start+scroll_size;	
	if (scroll_end > (int)lineelements.GetSize())
	{
		scroll_start=lineelements.GetSize()-scroll_size;
		scroll_end	=lineelements.GetSize();
	}
	

	for (int e=0;e<scroll_start  && e<(int)lineelements.GetSize() ;e++)
	{
		lineelements.Get(e)->GetNode()->Dettach();
		lineelements.Get(e)->SetVisibility(0.0f);
		lineelements.Get(e)->SetVisible		(false);
	}

	
	for (int e=scroll_start,n=0;e<scroll_end && e<(int)lineelements.GetSize();e++,n++)
	{
		lineelements.Get(e)->GetNode()->Dettach();
		lineelements.Get(e)->GetNode()->LinkTo(linenodes.Get(n));
		lineelements.Get(e)->SetVisibility(1.0f);
		lineelements.Get(e)->SetVisible		(true);
	}
		
	for (int e=scroll_end;e<(int)lineelements.GetSize();e++)
	{
		lineelements.Get(e)->GetNode()->Dettach();
		lineelements.Get(e)->SetVisibility(0.0f);
		lineelements.Get(e)->SetVisible		(false);
	}	
	*/
	return true;
}


/*-------------------------------------------------------------------
//	GRPCONSOLE::ScrollDown
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			22/03/2017 13:14:45
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPCONSOLE::ScrollDown()
{
	//ScrollLines(+this->lineelements.GetSize());
	scroll_start++;
	return true;
}

/*-------------------------------------------------------------------
//	GRPCONSOLE::ScrollUp
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			22/03/2017 13:14:49
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPCONSOLE::ScrollUp()
{
	scroll_start--;
	return true;
}

bool GRPCONSOLE::ScrollTop()
{
	scroll_start=0;

	return true;
}

bool GRPCONSOLE::ScrollBottom()
{
	scroll_start=(lineelements.GetSize()-scroll_size+1);
	if (scroll_start<0) scroll_start=0;

	return true;
}

/*-------------------------------------------------------------------
//	GRPCONSOLE::SetColor
*/	
/**	
//	
//	sets the color to print form now on
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			15/10/2015 13:16:35
//	
//  @param				color : 
*/
/*-----------------------------------------------------------------*/
void GRPCONSOLE::SetColor(GRPCOLOR color)
{
	caret.color = color;
}

/*-------------------------------------------------------------------
//	GRPCONSOLE::Clear
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/03/2017 16:01:20
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPCONSOLE::Clear()
{	
	this->xmutex->Lock();
	Destroy();
	scroll_start=0;
	this->line=0;
	SetArea(&area);
	this->xmutex->UnLock();
	return true;
}



/*-------------------------------------------------------------------
//	GRPCONSOLE::SetArea
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			13/03/2017 10:34:47
//	
//  @param				area : 
*/
/*-----------------------------------------------------------------*/
void GRPCONSOLE::SetArea(GRPBB* area)
{
	if (!area)
		return;	

	if (!screen)
		return;


	area->Update();

	if(area->maxpoint.x < area->minpoint.x) return;
	if(area->maxpoint.y < area->minpoint.y) return;

	float xe=area->maxpoint.x;
	float ye=area->maxpoint.y;

	float xs=area->minpoint.x;
	float ys=area->minpoint.y;

	float w =area->width;
	float h =area->height;

	float x = xs + w/2.0f;
	float y = ys + h/2.0f;

	h/=scene->GetViewport()->GetAspectRatio();

	// convert to screen space
	x *= 2.0f;
	x -= 1.0f;

	y *= 2.0f;
	y -= 1.0f;

	y/=scene->GetViewport()->GetAspectRatio();

		if (attachments.GetSize())
		{
			const GRPVECTOR* scale=attachments.Get(0)->GetNode()->GetScale();
			attachments.Get(0)->GetNode()->SetScale(w,h,scale->vector[2]);

			const GRPVECTOR* position=attachments.Get(0)->GetNode()->GetPosition();
			attachments.Get(0)->GetNode()->SetPosition(0,0,-0.0001f);
		}
	
	this->GetNode()->SetPosition(x,y,-0.001f);
	this->GetNode()->SetScale		(1.0f,1.0f,1.0f);

	x=(area->leftpadding)*w;		
	y=-(area->toppadding)*h;	

	w-=(area->rightpadding*w);
	h-=(area->toppadding*h);

	float z=(font) ? -font->GetAttributes()->extrude : 0.0f;
	this->textparent->SetPosition	(-w, +h-line_height, +z);					
	this->textparent->SetScale		(1.0f, 1.0f, 1.0f);

	this->area=*area;

	line_height	=(font->GetLineHeight()*caret.size);
	line_width	= w;
	//scroll_size	=	(int)float(((area->height)-(area->toppadding*area->height)-(area->bottompadding*area->height))/line_height);
	scroll_size=(int)((h*2.0f)/line_height);

	scene->Lock();

	//linenodes.DeleteContents();
	//linenodes.DeleteAll();

	y=0.0f;
	/*
	for (int e=0;e<scroll_size;e++)
	{
		y-=line_height;
		GRPNODE*	node=this->scene->CreateNode();
							node->SetScale			(1.0f,1.0f,1.0f);
							node->SetPosition		(0.0,y,0.0);
							node->SetParent			(this->textparent);
		linenodes.Add(node);
	}
	*/

	scene->UnLock();
	
}


/*-------------------------------------------------------------------
//	SetArea
*/	
/**	
//	
//	sets the area to render the console on
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 9:46:24
//	
//  @param				xs : 
//  @param				ys : 
//  @param				xe : 
//  @param				ye : 
*/
/*-----------------------------------------------------------------*/
void GRPCONSOLE::SetArea(float xs, float ys, float xe, float ye)
{
	GRPBB bb;

	bb.minpoint.x=xs;	bb.minpoint.y=ys;
	bb.maxpoint.x=xe;	bb.maxpoint.y=ye;

	this->SetArea(&bb);
}



/*-------------------------------------------------------------------
//	GRPCONSOLE::SetViewport
*/	
/**	
//	
//	sets the viewport to draw into
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			29/10/2015 10:13:44
//	
//  @param				x : 
//  @param				y : 
//  @param				width : 
//  @param				height : 
*/
/*-----------------------------------------------------------------*/
void GRPCONSOLE::SetViewport(float x, float y, float width, float height)
{
	scene->GetViewport()->SetPosition(x, y);
	scene->GetViewport()->SetSize(width, height);
}



/*-------------------------------------------------------------------
//	GRPCONSOLE::SetBackgroundMaterial
*/	
/**	
//	
//	sets the material of the background object
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			29/10/2015 10:14:20
//	
//  @param				material : 
*/
/*-----------------------------------------------------------------*/
void GRPCONSOLE::SetBackgroundMaterial(GRPMATERIAL* material)
{
	if (!material)	return;

	this->background_material=material;

	if (!plane)
		CreateBackgroundPlane();

	if (plane)
	{
		plane->GetNode()->SetMaterial(material);
		plane->SetVisible		(true);
		plane->SetVisibility(1.0f);
	}
	
}


/*-------------------------------------------------------------------
//	GRPCONSOLE::HandleEvent
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/03/2017 10:34:59
//	
//  @param				xevent : 
*/
/*-----------------------------------------------------------------*/
void GRPCONSOLE::HandleEvent(XEVENT* xevent)
{
	if(xevent->GetEventType() == GRPXEVENTTYPE_SCREEN_CHANGESIZE)
		{
			Resize();																										
		}
}

/*-------------------------------------------------------------------
//	GRPCONSOLE::Add
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/03/2017 16:04:33
//	
//  @param				st : 
*/
/*-----------------------------------------------------------------*/
void GRPCONSOLE::Add(XCHAR* st)
{	
	if (!st)
		return;

	GRPTEXTELEMENT* element=static_cast<GRPTEXTELEMENT*>(this->lineelements.GetLast());
	if (!element) 
	{
		 this->PrintImp(st);
		 return;
	}
	XSTRING* text=element->GetContent();
	if (st[0]==__C('\b'))
					{
					if (text->GetSize()==0)	
					{	
						return;											
					}
					text->DeleteCharacters(text->GetSize()-1,1);					
					}
		else
		{
					XSTRING original,trimmed,rest;
					text->Add(st);		
					original.Set(text->Get());
	
					float width=line_width*2.0f;
	
					this->font->MeasureWidth(width/caret.size,original,trimmed,rest);	
					text->Set(trimmed.Get());
					if (rest.GetSize()>0)
					{						
							if (rest.Get()[0]==__C('\n'))
								rest.DeleteCharacters(0,1);
							this->Print(rest.Get());
					}
		}
	element->UpdateContent();
	

}

/*-------------------------------------------------------------------
//	GRPCONSOLE::AddBackground
*/	
/**	
//	
//	Adds background
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/03/2017 10:35:17
//	
//  @param				element : 
*/
/*-----------------------------------------------------------------*/
void GRPCONSOLE::AddBackground(GRPELEMENT* element)
{
	GetAttachments()->Add(element);
	this->AddElement(element);
}



/*-------------------------------------------------------------------
//	GRPCONSOLE::MoveTextElementToConsole
*/	
/**	
//	
//	moves the elements of a textbox to the console, right now only the first one, need to change it
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			04/11/2015 10:21:39
//	
//  @param				textbox : 
*/
/*-----------------------------------------------------------------*/
void GRPCONSOLE::MoveTextElementToConsole(GRPTEXTBOX* textbox)
{
	if (!textbox->GetElements()->GetSize()) return;

	//--------------------------- add text as it's a new element
	lineelements.Add(textbox->GetElements()->Get(0));

	//--------------------------- Extract the letters form the textbox
	GRPELEMENT* textline = textbox->GetElements()->Get(0);
	textbox->GetElements()->Delete(textline);

	textline->GetNode()->Dettach();

	//--------------------------- no need to create new node as merge creates a new node itself
	textline->GetNode()->LinkTo(this->GetNode());
	textline->GetNode()->GetName()->Format(__L("Line %d"),lineelements.GetSize());

	//--------------------------- assign old textbox position
	const GRPVECTOR* pos=textbox->GetNode()->GetPosition();
	const GRPVECTOR* sc	=textbox->GetNode()->GetScale();
	
	textline->GetNode()->SetPosition	(	pos->vector[0],	
																			pos->vector[1],	
																			pos->vector[2]);

	textline->GetNode()->SetScale			(	sc->vector[0],			
																			sc->vector[1],		
																			sc->vector[2]);

	//---------------------------sets the line material
	textline->GetNode()->SetMaterial	(	materials.Get(caret.color));

	//---------------------------add to the console and to the scene
	// Problem: even if the console is not added to the scene the elements are
	this->AddElement(textline);
	textline->AddObserver(scene);
	
	//scene->Add(this);
	scene->Add(textline); 
	//DIEGO: la scene puede añadir elementos suelto, es mejor añadir SOLO la nueva linea, 
	//si no, se actualizan en la escena TODOS los elementos, esto explicaria el bajón de 
	//rendimiento cada vez que se añadia una linea
	
}
