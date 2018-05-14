
/*------------------------------------------------------------------------------------------
//	TESTBASEFONTTTF.CPP
//	
//	Test Base (User Interface)
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 30/05/2016 16:19:29
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "XApplication.h"

#include "GRPScreen.h"
#include "GRPCamera.h"
#include "GRPFile3DFBX.h"
#include "GRPFile3DOBJ.h"
#include "GRPObject.h"
#include "GRPScene.h"
#include "GRPMaterialLibrary.h"
#include "GRPShaderLibrary.h"
#include "GRPFont.h"
#include "GRPFontManager.h"
#include "GRPRenderer.h"
#include "GRPViewport.h"
#include "GRPVisualDebugger.h"
#include "GRPPass.h"

#include "GRP2DRendererPrimitives.h"
#include "GRPBitmap.h"
#include "GRPTexture.h"
#include "GRPBufferColorRGB.h"

#include "SNDFactory.h"
#include "SNDElement.h"
#include "SNDSource.h"

#include "UIManager.h"
#include "UIWIdgetCreatorHelperDrawable.h"
#include "UIFactory.h"
#include "UIWidgetForm.h"
#include "UIWidgetButton.h"
#include "UIWidgetText.h"
#include "UIWidgetProgressBar.h"
#include "UIWidgetSpinBox.h"
#include "UIWidgetImage.h"
//#include "UIWidgetImageButton.h"
#include "UIWidgetListContainer.h"
#include "UIWidgetDialog.h"
#include "UIWidgetToast.h"
#include "UIWidgetMenu.h"
#include "UIWidgetExclusiveContainer.h"
#include "UIWidgetComboBox.h"
#include "UIWidgetScissorContainer.h"


#include "UIListAdapterText.h"
#include "UIWidgetLayoutFitParent.h"

#include "UIStyleDrawer.h"

#include "TestBaseUI.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/
//class UICOMPARATOR : public GRPELEMENT::COMPARATOR
class UICOMPARATOR: public XCOMPARATOR<GRPDRAWABLE>
{
public:
	int Compare(GRPDRAWABLE* test1,GRPDRAWABLE* test2)
	{
		GRPDRAWABLE* element1=static_cast<GRPDRAWABLE*>(test1);
		GRPDRAWABLE* element2=static_cast<GRPDRAWABLE*>(test2);
		
		if (test1->GetRenderPriority() < test2->GetRenderPriority())	return 1; // need to test only with render priority as we could have hetereogeneus objects

		//if (!element1 || !element2)
		//	return 0;
		//
		//GRPNODE* node1 = element1->GetNode();
		//GRPNODE* node2 = element2->GetNode();
		//
		//node1->GetWorldPosition().vector;
		//node2->GetWorldPosition().vector;
		//
		//
		//if(node1->GetWorldPosition().vector[2] < node2->GetWorldPosition().vector[2])
		//	{
		//		return 1;
		//	}
		//else
		//	{
		//		return -1;
		//	}

		return -1;
	}
};


/*-------------------------------------------------------------------
//	TESTBASEUI::TESTBASEUI
*/	
/**	
//	
//	Class Constructor TESTBASEUI
//	
//	@author				Abraham J. Velez
//	@version			29/12/2015 17:56:15
//	
//  @param				inpmanager : 
//  @param				screen : 
//  @param				scene : 
//  @param				xapplication : 
*/
/*-----------------------------------------------------------------*/
TESTBASEUI::TESTBASEUI(GRPAPPLICATION* xapplication) :  TESTBASE(xapplication) 
{
	Clean();
}



/*-------------------------------------------------------------------
//	TESTBASEUI::~TESTBASEUI
*/	
/**	
//	
//	 Class Destructor TESTBASEUI
//	
//	@author				Abraham J. Velez
//	@version			29/12/2015 17:56:20
//	
*/
/*-----------------------------------------------------------------*/
TESTBASEUI::~TESTBASEUI()
{
	//delete uimanager->GetRootWidget();
	UnSubscribeEvent(XEVENTTYPE_UI, uimanager);
	
	delete widgetcreator;
	if(uimanager)
		{
			delete uimanager;
		}

	Clean();
}




/*-------------------------------------------------------------------
//	TESTBASEUI::IniResources
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			29/12/2015 17:56:31
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASEUI::IniResources()
{
	if(!xapplication)												 return NULL;
	if(!xapplication->GetMainScene())				 return NULL;
	//if(!xapplication->GetInputManager())		 return NULL;
	
	GRPSCENE* scene  = xapplication->GetMainScene();
	
	PathManagement(__L("UI"));

	GRPFONTATTRIBUTES att;
	att.extrude				= 0;
	att.monospace			= true;
	att.quality				= 5;
	att.back					= false;
	
	xpath.Create(XPATHSMANAGERSECTIONTYPE_FONTS, 1, __L("arial.ttf"));	
	if(!GRPOBJECTBUILDER::Get()->GetFontManager()->Load(xpath.Get(),		__L("UI") ,&att)) //load default
		{
			XDEBUG_PRINTCOLOR(1,__L("TTF file not loaded! %s"), xpath.Get());
			return false;
		}
	
	
	xpath.Create(XPATHSMANAGERSECTIONTYPE_FONTS, 1, __L("GOTHICB.TTF"));

	if(!GRPOBJECTBUILDER::Get()->GetFontManager()->Load(xpath.Get(),		__L("UI-Bold") ,&att)) //load default
		{
			XDEBUG_PRINTCOLOR(1,__L("TTF file not loaded! %s"), xpath.Get());
			return false;
		}
	
	

	xpath.Create(XPATHSMANAGERSECTIONTYPE_FONTS, 1, __L("GOTHICI.TTF"));

	if(!GRPOBJECTBUILDER::Get()->GetFontManager()->Load(xpath.Get(),		__L("UI-Italic") ,&att)) //load default
		{
			XDEBUG_PRINTCOLOR(1,__L("TTF file not loaded! %s"), xpath.Get());
			return false;
		}

	
	xpath.Create(XPATHSMANAGERSECTIONTYPE_FONTS, 1, __L("GOTHICBI.TTF"));
	if(!GRPOBJECTBUILDER::Get()->GetFontManager()->Load(xpath.Get(),		__L("UI-Bold-Italic") ,&att)) //load default
		{
			XDEBUG_PRINTCOLOR(1,__L("TTF file not loaded! %s"), xpath.Get());
			return false;
		}

	GRPFONTMANAGER* fontmanager =GRPOBJECTBUILDER::Get()->GetFontManager();	
	GRPFONT* font = fontmanager->GetFont(__L("UI"));
					 font = fontmanager->GetFont(__L("UI-Bold"));
					 font = fontmanager->GetFont(__L("UI-Italic"));
					 font = fontmanager->GetFont(__L("UI-Bold-Italic"));
	
	// initialize ui scene
	uimanager			= new UIMANAGER							(INPMANAGER::GetInstance());	if(!uimanager)			return false;			
	uiscene				= new GRPSCENE							(scene->GetScreen());															if(!uiscene)				return false;		
	widgetcreator = new UIWIDGETCREATORHELPER	(uimanager);																			if(!widgetcreator)	return false;

	uimanager->SetUIWidgetCreator		(widgetcreator);
	uimanager->SubscribeEvent				(GRPXEVENTTYPE_SCREEN_CHANGESIZE, scene->GetScreen());
	uimanager->SetVirtualScreenSize	(1920, 1080);	
	uimanager->SetDrawMouse					(true); //draw mouse... ¬¬

	uiscene->SetName(__L("UISCENE"));
	uiscene->Ini();
	uiscene->GetPass()->SetRenderComparator<UICOMPARATOR>();
	uiscene->GetCamera()->SetPosition(0.0f, 0.0f, 150.0f);
	uiscene->GetCamera()->GetTarget()->SetPosition(0.0f, 0.0f, 0.0f);

	GRPVIEWPORT*	viewport = uiscene->GetViewport();
								viewport->SetSize(1.0f, 1.0f);
								viewport->SetPosition(0.0f, 0.0f);
								viewport->Select();
								float w, h;
								viewport->GetWindowSize(w, h);
								viewport->SetOrthoScale(w);	
								viewport->SelectProjectionOrtho();

	scene->renderer->SetClearColor(GRPCOLOR(1.0f, 0.1f, 0.1f, 1.0f));
	scene->renderer->AddScene(uiscene);

	UISTYLEDRAWER*	style = new UISTYLEDRAWER(uiscene, __L("UIDRAWER"));
									style->SetPriority(400);
									style->ShowDebug(false);
	
	uimanager->SetRealScreenSize((XDWORD)w, (XDWORD)h);
	uimanager->SetVirtualScreenSize(1920, 1080);
	uimanager->SetDPI(uiscene->GetScreen()->GetDPI());
	uimanager->SetUIStyle(style);

	uiscene->Add(style);

	UIWIDGETFORM* window = new UIWIDGETFORM(__L("MainWindow"), uimanager);	if(!window)	return false;	
								window->SetPosition(0, 0);
								window->SetSize(1, 1);	
								window->SetTitle(__L("MainWindow"));
								window->SetIsOwnVisible(false);
			
	uimanager->SetRootWidget(window);
	
	
	SubscribeEvent(XEVENTTYPE_UI, uimanager);
	
	//UIWIDGETIMAGE*	image = new UIWIDGETIMAGE(__L("Image"), uimanager);	if(!image) return false;	
	//								image->SetImage(__L("Prueba_04.png"));
	//								image->SetPosition(0, 0);
	//								image->SetSize		(1920, 1080);
	//
	//window->AddChild(image);

	
	/*
	
	//UIWIDGETBUTTONDRAWABLE* button = (UIWIDGETBUTTONDRAWABLE*)widgetcreator->CreateButton(__L("BUTTON"), __L("ficha_01_off.png"), NULL, __L("ficha_01_on.png"));
	UIWIDGETIMAGE* button = new UIWIDGETIMAGE(__L("Button"), uimanager);

	UIWIDGETBUTTON* button = (UIWIDGETBUTTON*)widgetcreator->CreateButton(NULL, __L("ficha_01_off.png"), NULL, __L("ficha_01_on.png"));
	//button->SetImage(__L("button-game.tga"));
	//button->SetPosition(100, 100);
	//button->SetSize(64, 64);
	button->SetIsSticky(true);
	
	button->SetPosition(685, 218);
	button->SetSize(167, 162);
	
	window->AddChild(button);



	UIWIDGETLISTCONTAINER* list = new UIWIDGETLISTCONTAINER(__L("list"), uimanager);
	list->SetPosition(35, 97);
	list->SetSize(256, 256);
	list->SetScrolling(0);
	
	for(XDWORD i = 0; i < 10; i++)
		{
			UIWIDGETBUTTON* button = (UIWIDGETBUTTON*)widgetcreator->CreateButton(__L("BUTTON"), __L("button-game.tga"), __L("button-game-hover.tga"), __L("button-game-pressed.tga"));
	
			XSTRING name;
			name.Set(__L("Text"));
			UIWIDGETTEXT* text = (UIWIDGETTEXT*)button->FindChildByNameRec(&name);
			if(text)
				{
					text->SetColor(255, 255, 255);
				}
	
			button->SetPosition(100, 100);
			button->SetSize(256, 64);
	
			list->AddChild(button);
		}
	
	
	window->AddChild(list);
	
	UIWIDGETBUTTON* button0 = (UIWIDGETBUTTON*)widgetcreator->CreateButton(__L("BUTTON"), __L("button-game.tga"), __L("button-game-hover.tga"), __L("button-game-pressed.tga"));
	//UIWIDGETBUTTON* button1 = (UIWIDGETBUTTON*)widgetcreator->CreateButton(__L("SMALL"), __L("button-game.tga"), __L("button-game-hover.tga"), __L("button-game-pressed.tga"));
	UIWIDGETBUTTON* button1 = (UIWIDGETBUTTON*)widgetcreator->CreateButton(__L("BUTTON"), __L("button-game.tga"), __L("button-game-hover.tga"), __L("button-game-pressed.tga"));
	
	button0->SetPosition(256, 256);
	button0->SetSize(512, 512);
	
	//button1->SetPosition(512, 256);
	button1->SetPosition(512, 1080-256);
	button1->SetSize(256, 256);
	
	button0->SetIsClickable(true);
	button1->SetIsClickable(true);
	
	window->AddChild(button0);
	window->AddChild(button1);
	
	UIWIDGETPROGRESSBAR* progressbar = new UIWIDGETPROGRESSBAR(__L("progress"), uimanager);
	progressbar->SetPosition(100, 512);
	//progressbar->SetPosition(100, 0);
	progressbar->SetSize(1024, 128);
	//progressbar->SetSize(128, 800);
	//progressbar->SetHeightDIP(125.0f);
	
	
	//progressbar->SetProgressBarDirection(UIWIDGETPROGRESSBAR_DIRECTION_VERTICAL);
	//progressbar->SetProgressBarDirection(UIWIDGETPROGRESSBAR_DIRECTION_HORIZONTAL);
	
	progressbar->SetProgress(0.75f);
	//progressbar->SetProgress(0.5f);
	progressbar->SetBackgroundImage(__L("progressbar_empty.jpg"));
	progressbar->SetProgressBarImage(__L("progressbar_full.jpg"));
	
	window->AddChild(progressbar);
	
	UIWIDGETIMAGEDRAWABLE* normal = new UIWIDGETIMAGEDRAWABLE(__L("Image"), uimanager, uiscene);
	if(!normal)
		{
			return false;
		}
	
	//image->SetImage(__L("background.png"));
	normal->SetImage(__L("button-game.tga"));
	normal->SetPosition(100, 100);
	normal->SetSize(1024, 512);
	
	UIWIDGETIMAGEDRAWABLE* hover = new UIWIDGETIMAGEDRAWABLE(__L("Image"), uimanager, uiscene);
	if(!hover)
		{
			return false;
		}
	
	//image->SetImage(__L("background.png"));
	hover->SetImage(__L("button-game-hover.tga"));
	hover->SetPosition(100, 100);
	hover->SetSize(1024, 512);
	
	UIWIDGETIMAGEDRAWABLE* pressed = new UIWIDGETIMAGEDRAWABLE(__L("Image"), uimanager, uiscene);
	if(!pressed)
		{
			return false;
		}
	
	//image->SetImage(__L("background.png"));
	pressed->SetImage(__L("button-game-pressed.tga"));
	pressed->SetPosition(100, 100);
	pressed->SetSize(1024, 512);
	
	UIWIDGETBUTTONDRAWABLE* button = new UIWIDGETBUTTONDRAWABLE(__L("BUTTON"), uimanager, uiscene);
	button->SetPosition(0, 128);
	button->SetSize(256, 128);
	button->SetISHoverable(true);
	button->SetIsClickable(true);
	window->AddChild(button);
	
	
	button->SetStateWidgets(normal, hover, pressed);
	
	UIWIDGETTEXTDRAWABLE* text = new UIWIDGETTEXTDRAWABLE(__L("TEXT"), uimanager, uiscene);
	text->SetText(__L("Hello"));
	text->SetVerticalAligment(UIWIDGET_ALIGNFLAGS_CENTER);
	text->SetHorizontalAligment(UIWIDGET_ALIGNFLAGS_CENTER);
	fitparent->AddChild(text);
	
	
	
	UIWIDGETMENU* menu = new UIWIDGETMENU(__L("Menu"), uimanager);
												menu->SetPosition(0, 0);
												//menu->SetEndingPoint(1.0f, 1.0f);
												menu->SetSize(256, 512);
	
	menu->SetMenuOrientation(UIWIDGET_MENU_ORIENTATION_HORIZONTAL);
												//menu->SetMenuOrientation(UIWIDGET_MENU_ORIENTATION_VERTICAL);
	
	menu->SetButtonsImages(__L("button-game.tga"), __L("button-game-hover.tga"), __L("button-game-pressed.tga"));
	
	//
	//
	//UIWIDGETMENUDRAWABLE* submenu = new UIWIDGETMENUDRAWABLE(__L("SubMenu"), uimanager, uiscene);
	//											submenu->SetOrigin(0.0f, 0.0f);
	//											submenu->SetEndingPoint(1.0f, 1.0f);
	//											submenu->SetDIPScaleY(50.0f);
	//											submenu->SetMenuOrientation(UIWIDGET_MENU_ORIENTATION_VERTICAL);
	//											//submenu->SetMenuOrientation(UIWIDGET_MENU_ORIENTATION_HORIZONTAL);
	//											submenu->SetButtonsImages(__L("button-game.tga"), __L("button-game-hover.tga"), __L("button-game-pressed.tga"));
	//
	//											submenu->AddAction(__L("SubItem1"));
	//											submenu->AddAction(__L("SubItem2"));
	//											submenu->AddAction(__L("SubItem3"));
	//											submenu->AddAction(__L("SubItem4"));
	//
	menu->AddAction(__L("Item1"));
	//menu->AddMenu(submenu);
	menu->AddAction(__L("Item2"));
	
	window->AddChild(menu);
	*/
/*
	UIWIDGETEXCLUSIVECONTAINER* exclusive = new UIWIDGETEXCLUSIVECONTAINER(__L("Exclusive"), uimanager);
	exclusive->SetPosition(0, 0);
	exclusive->SetSize(1, 1);
	
	UIWIDGETBUTTON* ficha = (UIWIDGETBUTTON*)widgetcreator->CreateButton(NULL, __L("ficha_01_off.png"), __L("ficha_01_off.png"), __L("ficha_01_on.png"));
	ficha->SetIsClickable(true);
	ficha->SetIsSticky(true);
	
	ficha->SetPosition(685+18, 218+18);
	ficha->SetSize(130, 130);
	
	exclusive->AddChild(ficha);
	
	int margin = -2;

	UIWIDGETBUTTON* ficha2 = (UIWIDGETBUTTON*)widgetcreator->CreateButton(NULL, __L("ficha_02_off.png"), __L("ficha_02_off.png"), __L("ficha_02_on.png"));
	ficha2->SetIsSticky(true);
	
	ficha2->SetPosition(ficha->GetPositionX() + ficha->GetWidth() + margin , ficha->GetPositionY());
	ficha2->SetSize(130, 130);
	
	exclusive->AddChild(ficha2);
	
	UIWIDGETBUTTON* ficha3 = (UIWIDGETBUTTON*)widgetcreator->CreateButton(NULL, __L("ficha_03_off.png"), __L("ficha_03_off.png"), __L("ficha_03_on.png"));
	ficha3->SetIsSticky(true);
	
	ficha3->SetPosition(ficha2->GetPositionX() + ficha2->GetWidth() + margin, ficha->GetPositionY());
	ficha3->SetSize(130, 130);
	
	exclusive->AddChild(ficha3);
	
	UIWIDGETBUTTON* ficha4 = (UIWIDGETBUTTON*)widgetcreator->CreateButton(NULL, __L("ficha_04_off.png"), __L("ficha_04_off.png"), __L("ficha_04_on.png"));
	ficha4->SetIsSticky(true);
	
	ficha4->SetPosition(ficha3->GetPositionX() + ficha3->GetWidth() + margin, ficha->GetPositionY());
	ficha4->SetSize(130, 130);
	
	exclusive->AddChild(ficha4);
	
	window->AddChild(exclusive);

	XDWORD counter = 1;
	for(XDWORD i = 0; i < 3; i++)
		{
			for(XDWORD j = 0; j < 12; j++)
				{
					XSTRING str;
					str.Format(__L("number_%d"), counter);
					UIWIDGETCONTAINER* w = new UIWIDGETCONTAINER(str.Get(), uimanager);
					w->SetPosition(157 + j*(130 + 4), 661 - i*(130 + 4));
					w->SetSize(130, 130);
					w->SetIsClickable(true);
					w->SetISHoverable(true);
					uimanager->GetRootWidget()->AddChild(w);
				}
		}

											borrar = (UIWIDGETBUTTON*)widgetcreator->CreateButton(NULL, __L("borrar_off.png"), __L("borrar_off.png"), __L("borrar_on.png")); 											
											borrar->SetPosition(520, 284);
											borrar->SetSize(161, 60);
											borrar->SetIsSticky(true);
											borrar->SetIsDragable(false);
	uimanager->GetRootWidget()->AddChild(borrar);

	UIWIDGETCONTAINER*	widget = new UIWIDGETCONTAINER(__L("0"), uimanager);
											widget->SetPosition(23, 393);
											widget->SetSize(130, (130+4)*3);
											widget->SetIsClickable(true);
											widget->SetISHoverable(true);
	uimanager->GetRootWidget()->AddChild(widget);
	//squares.Add(widget);

											widget = new UIWIDGETCONTAINER(__L("121"), uimanager);
											widget->SetPosition(1765, 393);
											widget->SetSize(130, 130);
											widget->SetIsClickable(true);
											widget->SetISHoverable(true);
	uimanager->GetRootWidget()->AddChild(widget);
	//squares.Add(widget);

											widget = new UIWIDGETCONTAINER(__L("221"), uimanager);
											widget->SetPosition(1765, 527);
											widget->SetSize(130, 130);
											widget->SetIsClickable(true);
											widget->SetISHoverable(true);
	uimanager->GetRootWidget()->AddChild(widget);
	//squares.Add(widget);


											widget = new UIWIDGETCONTAINER(__L("321"), uimanager);
											widget->SetPosition(1765, 661);
											widget->SetSize(130, 130);
											widget->SetIsClickable(true);
											widget->SetISHoverable(true);
	uimanager->GetRootWidget()->AddChild(widget);
	//squares.Add(widget);

											widget = new UIWIDGETCONTAINER(__L("1D"), uimanager);
											widget->SetPosition(157, 796);
											widget->SetSize((130+4)*4, 130);
											widget->SetIsClickable(true);
											widget->SetISHoverable(true);
	uimanager->GetRootWidget()->AddChild(widget);
	//squares.Add(widget);

											widget = new UIWIDGETCONTAINER(__L("2D"), uimanager);
											widget->SetPosition(693, 796);
											widget->SetSize((130+4)*4, 130);
											widget->SetIsClickable(true);
											widget->SetISHoverable(true);
	uimanager->GetRootWidget()->AddChild(widget);
	//squares.Add(widget);

											widget = new UIWIDGETCONTAINER(__L("3D"), uimanager);
											widget->SetPosition(1229, 796);
											widget->SetSize((130+4)*4, 130);
											widget->SetIsClickable(true);
											widget->SetISHoverable(true);
	uimanager->GetRootWidget()->AddChild(widget);
	//squares.Add(widget);

											widget = new UIWIDGETCONTAINER(__L("118"), uimanager);
											widget->SetPosition(157, 930);
											widget->SetSize((130+4)*2, 130);
											widget->SetIsClickable(true);
											widget->SetISHoverable(true);
	uimanager->GetRootWidget()->AddChild(widget);
	//squares.Add(widget);

											widget = new UIWIDGETCONTAINER(__L("PARES"), uimanager);
											widget->SetPosition(425, 930);
											widget->SetSize((130+4)*2, 130);
											widget->SetIsClickable(true);
											widget->SetISHoverable(true);
	uimanager->GetRootWidget()->AddChild(widget);
	//squares.Add(widget);

											widget = new UIWIDGETCONTAINER(__L("ROJO"), uimanager);
											widget->SetPosition(693, 930);
											widget->SetSize((130+4)*2, 130);
											widget->SetIsClickable(true);
											widget->SetISHoverable(true);
	uimanager->GetRootWidget()->AddChild(widget);
	//squares.Add(widget);

											widget = new UIWIDGETCONTAINER(__L("NEGRO"), uimanager);
											widget->SetPosition(961, 930);
											widget->SetSize((130+4)*2, 130);
											widget->SetIsClickable(true);
											widget->SetISHoverable(true);
	uimanager->GetRootWidget()->AddChild(widget);
	//squares.Add(widget);

											widget = new UIWIDGETCONTAINER(__L("1936"), uimanager);
											widget->SetPosition(1229, 930);
											widget->SetSize((130+4)*2, 130);
											widget->SetIsClickable(true);
											widget->SetISHoverable(true);
	uimanager->GetRootWidget()->AddChild(widget);
	//squares.Add(widget);

											widget = new UIWIDGETCONTAINER(__L("IMPARES"), uimanager);
											widget->SetPosition(1497, 930);
											widget->SetSize((130+4)*2, 130);
											widget->SetIsClickable(true);
											widget->SetISHoverable(true);
	uimanager->GetRootWidget()->AddChild(widget);
	//squares.Add(widget);
	*/

	UIWIDGETSCISSORCONTAINER* scissor = new UIWIDGETSCISSORCONTAINER(__L("SCISSOR"), uimanager);
	scissor->SetPosition(512, 512);
	scissor->SetSize(256, 256);
	uimanager->GetRootWidget()->AddChild(scissor);
	
	UIWIDGETIMAGE* img = new UIWIDGETIMAGE(__L("IMAGE"), uimanager);
	img->SetImage(__L("options.png"));
	img->SetSize(512, 512);
	scissor->AddChild(img);

	UIWIDGETCOMBOBOX* combobox = new UIWIDGETCOMBOBOX(__L("ComboBox"), uimanager);
	combobox->SetPosition(960, 50);
	combobox->SetSize(256, 48);
	combobox->SetUpComboButton(__L("background_move_down_off.png"), __L("background_move_down_on.png"), __L("background_move_down_on.png"));
	
	UILISTADAPTERTEXT adapter(uimanager);
	XSTRING* t0 = new XSTRING();
	t0->Set(__L("Oneg"));
	XSTRING* t1 = new XSTRING();
	t1->Set(__L("Two"));
	XSTRING* t2 = new XSTRING();
	t2->Set(__L("Three"));
	adapter.AddItem(t0);
	adapter.AddItem(t1);
	adapter.AddItem(t2);
	
	combobox->SetAdapter(&adapter);
	combobox->ApplyAdapter();
	
	window->AddChild(combobox);
	
	UIWIDGETIMAGE* todrag = new UIWIDGETIMAGE(__L("ToDrag"), uimanager);
	todrag->SetImage(__L("ficha_03_off.png"));
	todrag->SetSize(167, 162);
	todrag->SetIsDragable(true);
	window->AddChild(todrag);
	
	rotatetest = new UIWIDGETIMAGE(__L("RotateTest"), uimanager);
	rotatetest->SetImage(__L("rotatetest.png"));
	rotatetest->SetPosition(1512, 768);
	rotatetest->SetSize(43, 43);
	rotatetest->SetRotation(0, 0, 0);
	rotatetest->SetRotationPivot(0, 0);
	//rotatetest->SetRotationPivot(((int)(rotatetest->GetWidth())), ((int)(rotatetest->GetHeight())));
	rotatetest->SetRotationPivot(((int)(rotatetest->GetWidth())), ((int)(rotatetest->GetHeight()/2)));
	//rotatetest->SetRotationPivot(0, 0);
	window->AddChild(rotatetest);
	rotation = 0;

	UIWIDGETBUTTON* buttoncheck = (UIWIDGETBUTTON*)widgetcreator->CreateButton(NULL, __L("checkbox.tga"), NULL, __L("checkbox-selected.tga"));
	
	if(!buttoncheck)
		{
			return false;
		}
	
	//buttoncheck->SetOrigin(0.25f, 0.25f);
	//buttoncheck->SetEndingPoint(0.50f, 0.50f);
	//buttoncheck->SetVerticalAspectRatio(1.0f);
	buttoncheck->SetIsClickable(true);
	buttoncheck->SetIsSticky(true);
	buttoncheck->SetIsDragable(false);
	
	window->AddChild(buttoncheck);
	
	/*
	GRPSCISSORELEMENT* scissor = new GRPSCISSORELEMENT();
	scissor->Set(true, 256, 128, 1024, 512);
	scissor->SetRenderPriority(1);
	uiscene->GetPass()->Add(scissor);
	*/

	return true;
}

/*-------------------------------------------------------------------
//	TESTBASEUI::EndResources
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			03/06/2016 10:45:11
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASEUI::EndResources()
{
	delete uimanager;
	uimanager = NULL;
	return true;
}


XDWORD scroll = 0;

/*-------------------------------------------------------------------
//	TESTBASEUI::Do
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			29/12/2015 17:56:01
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASEUI::Do()
{
	// need to rethink how this is done //yes, quite a lot
	if(uimanager)
		{
			float w, h;

			GRPVIEWPORT* viewport = uiscene->GetViewport();

			viewport->GetWindowSize(w, h);
	
			uimanager->SetRealScreenSize((XDWORD)w, (XDWORD)h);

			
			uimanager->Update();
			uimanager->Draw();

			if(uimanager->GetRootWidget())
				{
					XSTRING str(__L("list"));
					UIWIDGETLISTCONTAINER* w = (UIWIDGETLISTCONTAINER*)uimanager->GetRootWidget()->FindChildByNameRec(&str);
			
					scroll = 76;	

					if(w)
						{
							w->SetScrolling(scroll);
							//scroll++;
						}
				}

			

			if(keyboard)
				{
					if(keyboard->GetButton(INPBUTTON_ID_F1))												
						{
							if(keyboard->GetButton(INPBUTTON_ID_F1)->IsPressedWithRelease()) uiscene->ShowDebug(GRPDEBUG_FLAG_BOTH);
						}

					if(keyboard->GetButton(INPBUTTON_ID_F2))
						{
							if(keyboard->GetButton(INPBUTTON_ID_F2)->IsPressedWithRelease()) uiscene->ShowDebug(GRPDEBUG_FLAG_NORMAL);	
						}
				}

			bool pressed = false;
			if((uimanager->GetInput()->IsPressed(INPBUTTON_ID_MOUSE_LEFT)) || (uimanager->GetInput()->IsPressedLong(INPBUTTON_ID_MOUSE_LEFT)))
			{
				pressed = true;
			}

			UIWIDGETCONTAINER* widget = (UIWIDGETCONTAINER*)uimanager->GetRootWidget()->GetHoveredWidget(true);

			//if(pressed)
			//	{
			//		if(!erase)
			//			{
			//				XSTRING* str = widget->GetName();
			//				if((str->Find(__L("number"), true) != XSTRING_NOTFOUND)
			//				|| (str->Find(__L("121"), true) != XSTRING_NOTFOUND)
			//				|| (str->Find(__L("221"), true) != XSTRING_NOTFOUND)
			//				|| (str->Find(__L("321"), true) != XSTRING_NOTFOUND)
			//				|| (str->Find(__L("1D"), true) != XSTRING_NOTFOUND)
			//				|| (str->Find(__L("2D"), true) != XSTRING_NOTFOUND)
			//				|| (str->Find(__L("3D"), true) != XSTRING_NOTFOUND)
			//				|| (str->Find(__L("118"), true) != XSTRING_NOTFOUND)
			//				|| (str->Find(__L("1936"), true) != XSTRING_NOTFOUND)
			//				|| (str->Find(__L("PARES"), true) != XSTRING_NOTFOUND)
			//				|| (str->Find(__L("IMPARES"), true) != XSTRING_NOTFOUND)
			//				|| (str->Find(__L("ROJO"), true) != XSTRING_NOTFOUND)
			//				|| (str->Find(__L("NEGRO"), true) != XSTRING_NOTFOUND)
			//				|| (str->Find(__L("0"), true) != XSTRING_NOTFOUND))
			//					{
			//						//XDWORD number;
			//						//str->UnFormat(__L("number_%d"), &number);
			//						UIWIDGETEXCLUSIVECONTAINER* container = (UIWIDGETEXCLUSIVECONTAINER*)uimanager->GetRootWidget()->FindChildByNameRec(__L("Exclusive"));
			//
			//						
			//								if(container)
			//									{
			//										UIWIDGET* current = container->GetCurrentWidget();
			//										if(current)
			//											{
			//												//UIWIDGETBUTTON* t = (UIWIDGETBUTTON*)current->Clone();
			//												UIWIDGETBUTTON* ficha2 = (UIWIDGETBUTTON*)widgetcreator->CreateButton(NULL, __L("ficha_02_off.png"), __L("ficha_02_off.png"), __L("ficha_02_on.png"));
			//												//ficha2->SetIsSticky(true);
			//												//
			//												//ficha2->SetPosition(ficha->GetPositionX() + ficha->GetWidth() + margin , ficha->GetPositionY());
			//												//ficha2->SetSize(130, 130);
			//												//
			//												//exclusive->AddChild(ficha2);
			//
			//												int marginx = widget->GetWidth() -130;
			//												marginx = marginx/2;
			//
			//												int marginy = widget->GetHeight() -130;
			//												marginy = marginy/2;
			//
			//												ficha2->SetPosition(widget->GetPositionX() + marginx, widget->GetPositionY() + marginy);
			//												ficha2->SetSize(130, 130);
			//												ficha2->ForceWidgetState(UIWIDGET_STATE_NONE);
			//												ficha2->SetIsSticky(false);
			//												ficha2->SubstituteHover(NULL);
			//												ficha2->SubstitutePressed(NULL);
			//
			//												uimanager->GetRootWidget()->AddChild(ficha2);
			//												fiches.Add(ficha2);
			//											}
			//									}
			//					}
			//			}
			//		else
			//			{
			//				if(fiches.Find(widget) != NOTFOUND)
			//					{
			//						UIWIDGETBUTTON* btn = (UIWIDGETBUTTON*)widget;
			//						fiches.Delete(btn);
			//						uimanager->GetRootWidget()->RemoveChildPointer(btn);
			//						delete btn;
			//					}
			//			}
			//	}
		}

	rotation += 1.0f;

	if(rotation > (360-EPSILON))
	{
		rotation = 0.0f;
	}

	if(rotatetest)
	{
		rotatetest->SetRotation(0, 0, rotation);
	}

	return true;
}



/*-------------------------------------------------------------------
//	TESBASETUI::HandleEvent
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			31/05/2016 12:55:05
//	
//  @param				event : 
*/
/*-----------------------------------------------------------------*/
void TESTBASEUI::HandleEvent(XEVENT* event)
{
	if(event->GetEventType() == XEVENTTYPE_UI)
		{

				if(((UIEVENT*)event)->GetSender() == borrar)
					{
						UIWIDGETBUTTON* btn = (UIWIDGETBUTTON*)((UIEVENT*)event)->GetSender();
						if(btn->GetWidgetState() & UIWIDGET_STATE_TRIGGERED)
							{
								erase = true;
							}
						else if(btn->GetWidgetState() & UIWIDGET_STATE_RELEASED)
							{
								erase = false;
							}
					}

			if(((UIEVENT*)event)->GetUIEventType() == UIXEVENTTYPE_CLICK)
				{
					UIWIDGET* widget = ((UIEVENT*)event)->GetSender();
					if(widget->GetName()->Compare(__L("BUTTON")) == 0)
						{
							UIWIDGETTOAST* toast = (UIWIDGETTOAST*)widgetcreator->CreateToast(__L("Toast Test"), __L("toast.png"));
							toast->SetSize(1024, 256);
							toast->SetPosition(1920/2 - toast->GetWidth()/2, 720/2 + toast->GetHeight()/2);
							toast->SetTime(5000);
							uimanager->GetRootWidget()->AddChild(toast);
						}

					//UIWIDGET* widget = ((UIEVENT*)event)->GetSender();
					//if(widget->GetName()->Compare(__L("More")) == 0)
					//	{
					//		XSTRING str;
					//		str.Set(__L("ProgressBar"));
					//		UIWIDGETPROGRESSBAR* bar = (UIWIDGETPROGRESSBAR*)uimanager->GetRootWidget()->GetChildByName(&str);
					//		bar->IncrementProgress(0.015f);
					//	}
	
					//if(widget->GetName()->Compare(__L("Less")) == 0)
					//	{
					//		XSTRING str;
					//		str.Set(__L("ProgressBar"));
					//		UIWIDGETPROGRESSBAR* bar = (UIWIDGETPROGRESSBAR*)uimanager->GetRootWidget()->GetChildByName(&str);
					//		bar->IncrementProgress(-0.015f);
					//	}
					//else if(widget->GetName()->Compare(__L("Item1")) == 0)
					//	{
					//		UIWIDGETTOASTDRAWABLE* toast = new UIWIDGETTOASTDRAWABLE(__L("ME"), uimanager, uiscene);
					//		//toast->SetOrigin(0.2f, 0.8f);
					//		//toast->SetEndingPoint(0.8f, 0.95f);
					//		toast->SetTime(5000);
					//		
					//		UIWIDGETIMAGEDRAWABLE* toastbck = new UIWIDGETIMAGEDRAWABLE(__L("BCK"), uimanager, uiscene);
					//		//toastbck->FitParent(true);
					//		toastbck->SetImage(__L("toast.png"));
					//		toast->AddChild(toastbck);
					//		
					//		UIWIDGETTEXTDRAWABLE* textbck = new UIWIDGETTEXTDRAWABLE(__L("HAHA"), uimanager, uiscene);
					//		textbck->SetText(__L("TIMED TOAST"));
					//		//textbck->SetOrigin(0.1f, 0.1f);
					//		//textbck->SetEndingPoint(0.9f, 0.9f);
					//		
					//		textbck->SetHorizontalAligment(UIWIDGET_ALIGNFLAGS_CENTER);
					//		textbck->SetVerticalAligment(UIWIDGET_ALIGNFLAGS_CENTER);
					//
					//		toast->AddChild(textbck);
					//		
					//		uimanager->GetRootWidget()->AddChild(toast);
					//	}
					//else if(widget->GetName()->Compare(__L("Item2")) == 0)
					//	{
					//		UIWIDGETDIALOGDRAWABLE* dialog = (UIWIDGETDIALOGDRAWABLE*)widgetcreator->CreateMessageDialog(__L("YAY MENU!!!!"), __L("dialog-plain.png"),__L("normal.png"), __L("hover.png"), __L("clicked.png"));
					//
					//		//dialog->SetOrigin(0.25f, 0.4f);
					//		//dialog->SetEndingPoint(0.75f, 0.6f);
					//
					//		uimanager->GetRootWidget()->AddChild(dialog);
					//	}
				}
			//else if(((UIEVENT*)event)->GetSender()->GetWidgetState() == UIWIDGET_STATE_HOVERED)
			//	{
			//		XDEBUG_PRINTCOLOR(4, __L("FUCKING HOVERED"));
			//		if((uimanager->GetInput()->IsTriggered(INPBUTTON_ID_MOUSE_LEFT)) || (uimanager->GetInput()->IsPressed(INPBUTTON_ID_MOUSE_LEFT)) || (uimanager->GetInput()->IsPressedLong(INPBUTTON_ID_MOUSE_LEFT)))
			//			{
			//				XDEBUG_PRINTCOLOR(4, __L("FUCKING PRESSED"));
			//				UIEVENT* uievent = (UIEVENT*)event;
			//				XSTRING* str = uievent->GetSender()->GetName();
			//				UIWIDGETCONTAINER* w = (UIWIDGETCONTAINER*)uievent->GetSender();
			//				if(str->Find(__L("number"), true) != XSTRING_NOTFOUND)
			//					{
			//						//XDWORD number;
			//						//str->UnFormat(__L("number_%d"), &number);
			//
			//						UIWIDGETBUTTON* t = (UIWIDGETBUTTON*)widgetcreator->CreateButton(NULL, __L("ficha_01_off.png"), __L("ficha_01_off.png"), __L("ficha_01_on.png"));
			//						
			//						t->SetPosition(w->GetPositionX(), w->GetPositionY());
			//						t->SetSize(w->GetWidth(), w->GetHeight());
			//						
			//						uimanager->GetRootWidget()->AddChild(t);
			//					}
			//			}
			//		if(uimanager->GetInput()->IsReleased(INPBUTTON_ID_MOUSE_LEFT))
			//			{
			//				XDEBUG_PRINTCOLOR(4, __L("FUCKING RELEASED"));
			//			}
			//	}
		}
}
