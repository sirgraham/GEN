
/*------------------------------------------------------------------------------------------
//	TESTDIVS.CPP
//	
//	Manages space divisions
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 17/08/2017 10:36:27
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPFontManager.h"
#include "GRPMaterialLibrary.h"
#include "GRPRenderer.h"
#include "GRPFont.h"
#include "testdivs.h"
#include "XRand.h"
#include "GRPPass.h"
#include "GRPScene.h"
#include "GRPWidgetPage.h"
#include "GRPWidgetPass.h"
#include "GRPObjectBuilder.h"

#include "math.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/

/*-------------------------------------------------------------------
//	TESTDIVS::TESTDIVS
*/	
/**	
//	
//	Class Constructor TESTDIVS
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/08/2017 10:40:05
//	
//  @param				xapplication : 
*/
/*-----------------------------------------------------------------*/
TESTDIVS::TESTDIVS(GRPAPPLICATION* xapplication) : TESTBASE(xapplication)
{
		PathManagement(__L("divs"));
		GRPOBJECTBUILDER::Get()->GetFontManager()->Init();

		loadpage = __L("test_divs.html");
		//loadpage = __L("virtualRouletteInterface.html");

		pass = (GRPWIDGETPASS*)GRPPASSMANAGER::Get()->Create(__L("widget"));
		this->xapplication->GetMainScene()->Add(pass);

		currentpage = 2;
}

/*-------------------------------------------------------------------
//	TESTDIVS::IniResources
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/08/2017 10:40:12
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTDIVS::IniResources()
{
XDEBUG_PRINTCOLOR(0, __L("TESTDIVS::IniResources()"));

scene = xapplication->GetMainScene();

GRPWIDGETPAGE*		page = NULL;

PathManagement(__L("divs"));


										loadpage = __L("test_divs.html");
									  page = pass->AddPage(loadpage.Get());
									  page->Add(mouse);
										this->Subscribe(page);

										
										loadpage = __L("virtualRouletteInterface.html");
										page = pass->AddPage(loadpage.Get());
										page->Add(mouse);
										this->Subscribe(page);
										//*/

										loadpage = __L("virtualRouletteInterface_v2.html");
										page = pass->AddPage(loadpage.Get());
										page->Add(mouse);
										this->Subscribe(page);

										pass->SetCurrentPage(currentpage);
										
	XDEBUG_PRINTCOLOR(0, __L("TESTDIVS::IniResources() pass->AddPage"));
	return true;
}

/*-------------------------------------------------------------------
//	TESTDIVS::EndResources
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/08/2017 10:40:17
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTDIVS::EndResources()
{			
	this->pass->GetPages()->DeleteContents();	
	return false;
}

/*-------------------------------------------------------------------
//	TESTDIVS::Do
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/08/2017 10:40:24
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTDIVS::Do()
{
		static float i = 0.0;
		i += 0.05f;
		GRPWIDGET* widget = pass->GetPage()->GetByClass(__L("text_holder"));
		if (widget)
		{
				XSTRING s;
				int v=int(i);
				s.ConvertFromInt(v);
				widget->SetValue(s.Get());
		}

		if (keyboard)
				if (keyboard->GetButton(INPBUTTON_ID_F5))
						if (keyboard->GetButton(INPBUTTON_ID_F5)->GetState() == INPBUTTON_STATE_RELEASED)
						{
								EndResources();
								IniResources();
								pass->OnAdded();
						}

		if (keyboard)
				if (keyboard->GetButton(INPBUTTON_ID_1))
						if (keyboard->GetButton(INPBUTTON_ID_1)->GetState() == INPBUTTON_STATE_RELEASED)
						{
								loadpage = __L("virtualRouletteInterface.html");
								currentpage = 0;
								pass->SetCurrentPage(0);								
						}

		if (keyboard)
				if (keyboard->GetButton(INPBUTTON_ID_2))
						if (keyboard->GetButton(INPBUTTON_ID_2)->GetState() == INPBUTTON_STATE_RELEASED)
						{
								loadpage = __L("test_divs.html");
								currentpage = 1;
								pass->SetCurrentPage(1);
						}

		if (keyboard)
				if (keyboard->GetButton(INPBUTTON_ID_3))
						if (keyboard->GetButton(INPBUTTON_ID_3)->GetState() == INPBUTTON_STATE_RELEASED)
						{
								loadpage = __L("virtualRouletteInterface_v2.html");
								currentpage = 2;
								pass->SetCurrentPage(2);
						}

		if (keyboard)
				if (keyboard->GetButton(INPBUTTON_ID_4))
						if (keyboard->GetButton(INPBUTTON_ID_4)->GetState() == INPBUTTON_STATE_RELEASED)
						{
								GRPWIDGET* widget = pass->GetPage()->GetByClass(__L("credito"));
								if (widget)
								{
										widget->SetValue(__L("8.888"));
								}
						}

		if (keyboard->GetButton(INPBUTTON_ID_Q))
				if (keyboard->GetButton(INPBUTTON_ID_Q)->GetState() == INPBUTTON_STATE_RELEASED)
				{
						GRPWIDGET* widget = pass->GetPage()->GetByClass(__L("text_holder"));
						if (widget)
						{
								widget->SetValue(__L("8.000"));
						}
				}
		if (keyboard->GetButton(INPBUTTON_ID_ADD))
				if (keyboard->GetButton(INPBUTTON_ID_ADD)->GetState() == INPBUTTON_STATE_RELEASED)
				{
						
				}

		if (keyboard->GetButton(INPBUTTON_ID_F))
				if (keyboard->GetButton(INPBUTTON_ID_F)->GetState() == INPBUTTON_STATE_RELEASED)
						pass->GetPage()->FitInterfaceToScreen();


		return true;
}

/*-------------------------------------------------------------------
//	TESTDIVS::Notify
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/08/2017 10:40:30
//	
//	@return 			bool : 
//
//  @param				msg : 
//  @param				notifier : 
*/
/*-----------------------------------------------------------------*/
bool TESTDIVS::Notify(GRPMSGTYPE msg, GRPSUBJECT* notifier)
{
		switch (msg)
		{
				case UI_WIDGET_ACTIVE:
				{
						if (notifier->GetName()->Compare(__L("convertir")) == 0)
						{
								GRPWIDGET*w = dynamic_cast<GRPWIDGET*>(notifier);
								if (w)
								{
										GRPWIDGET* saldo = this->pass->GetPage()->FindByName(__L("saldo"));
										if (saldo)
										{
												GRPWIDGET* puntos = this->pass->GetPage()->FindByName(__L("puntos"));
												if (puntos)
												{
														XSTRING s = saldo->GetValue();
														puntos->SetValue(s.Get());
												}
										}
								}
						}
						break;
				}
		}
	return false;
}