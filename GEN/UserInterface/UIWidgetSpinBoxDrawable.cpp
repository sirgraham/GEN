
/*------------------------------------------------------------------------------------------
//	UIWIDGETSPINBOXDRAWABLE.CPP
//	
//	draws a spinbox
//   
//	Author						: Imanol Celaya Ruiz de Alegria
//	Date Of Creation	: 02/11/2015 12:42:03
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "UIWidgetSpinBoxDrawable.h"
#include "UIManager.h"
#include "UIInput.h"

#include "GRPRenderer.h"
#include "GRPObjectBuilder.h"
#include "GRPMaterialLibrary.h"
#include "GRPFontManager.h"
#include "GRPViewport.h"
#include "GRPScene.h"
#include "GRPObject.h"
#include "GRPElement.h"
#include "GRPTextBox.h"
#include "GRPShape.h"
#include "GRPObjectBuilderAlgorithmTriangulationEar.h"

#include "GRP2DCollisionDetection.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/





/*-------------------------------------------------------------------
//	UIWIDGETSPINBOXDRAWABLE::UIWIDGETSPINBOXDRAWABLE
*/	
/**	
//	
//	Class Constructor UIWIDGETSPINBOXDRAWABLE
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			10/02/2016 10:29:24
//	
//  @param				name : 
//  @param				uimanagerarg : 
 
//  @param				scene : 
*/
/*-----------------------------------------------------------------*/
UIWIDGETSPINBOXDRAWABLE::UIWIDGETSPINBOXDRAWABLE(XCHAR* name, UIMANAGER * uimanagerarg, GRPSCENE* scene) : UIWIDGETSPINBOX(name, uimanagerarg)
{
	Clean();
	// Create the elements
	this->scene = scene;

	// will need spline to make the arrows
	spinbox = new GRPOBJECT();
	if(!spinbox)
		{
			return;
		}

	spinbox ->SetNode(scene->CreateNode());

	background = GRPOBJECTBUILDER::Get()->CreatePlane(GRPOBJECTBUILDER::AXIS_X);
	background->SetNode(scene->CreateNode());
	background->GetNode()->LinkTo(spinbox->GetNode());

	GRPMATERIAL * materialproc = new GRPMATERIAL();
	if(!materialproc)
		{
			return;
		}

	materialproc->GetAttributes()->SetDiffuse(GRPCOLOR(0.95f, 0.95f, 0.95f, 1.0f));
	materialproc->GetAttributes()->SetEmission(GRPCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	materialproc->GetAttributes()->isupdated = true;
	GRPMATERIALLIBRARY::Get()->Add(materialproc);
	background->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"SDF"));
	background->GetNode()->SetMaterial(materialproc);
	spinbox->AddElement(background);

	GRPFONTMANAGER* fontmanager = GRPOBJECTBUILDER::Get()->GetFontManager();
	font = fontmanager->GetFont(__L("UI"));
	
	// need to add the textbox for painting
	textdisplay = new GRPTEXTBOX(scene);
	if(!textdisplay)
		{
			return;
		}
	
	textdisplay->SetNode(scene->CreateNode());
	textdisplay->GetNode()->LinkTo(spinbox->GetNode());
	
	materialproc = new GRPMATERIAL();
	if(!materialproc)
		{
			return;
		}
	
	materialproc->GetAttributes()->SetDiffuse(GRPCOLOR(0.05f, 0.05f, 0.05f, 1.0f));
	materialproc->GetAttributes()->SetEmission(GRPCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	materialproc->GetAttributes()->isupdated = true;
	GRPMATERIALLIBRARY::Get()->Add(materialproc);
	
	textdisplay->SetFont(font);
	textdisplay->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"SDF"));
	textdisplay->SetMaterial(materialproc);
	textdisplay->GetNode()->SetMaterial(materialproc);
	textdisplay->SetAlign(GRPTEXTBOX_ALIGNLEFT);
	
	XSTRING str;
	
	if(isinteger)
		{
			str.Format(__L("%d"), (XDWORD)value);
		}
	else
		{
			str.Format(__L("%f"), value);
		}
	
	textdisplay->SetContent(str.Get());
	textdisplay->MergeElements();
	//scene->InsertNewElementInRenderOrder(textdisplay->GetElements()->Get(0));
	
	// add boxes to click and change the value
	
	upbutton = GRPOBJECTBUILDER::Get()->CreatePlane(GRPOBJECTBUILDER::AXIS_X);
	upbutton->SetNode(scene->CreateNode());
	upbutton->GetNode()->LinkTo(spinbox->GetNode());
	materialproc = new GRPMATERIAL();
	if(!materialproc)
		{
			return;
		}
	
	materialproc->GetAttributes()->SetDiffuse(GRPCOLOR(0.3f, 0.3f, 0.3f, 1.0f));
	materialproc->GetAttributes()->SetEmission(GRPCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	materialproc->GetAttributes()->isupdated = true;
	GRPMATERIALLIBRARY::Get()->Add(materialproc);
	upbutton->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"SDF"));
	upbutton->GetNode()->SetMaterial(materialproc);
	
	spinbox->AddElement(upbutton);
	
	downbutton = GRPOBJECTBUILDER::Get()->CreatePlane(GRPOBJECTBUILDER::AXIS_X);
	downbutton->SetNode(scene->CreateNode());
	downbutton->GetNode()->LinkTo(spinbox->GetNode());
	materialproc = new GRPMATERIAL();
	if(!materialproc)
		{
			return;
		}
	
	materialproc->GetAttributes()->SetDiffuse(GRPCOLOR(0.3f, 0.3f, 0.3f, 1.0f));
	materialproc->GetAttributes()->SetEmission(GRPCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	materialproc->GetAttributes()->isupdated = true;
	GRPMATERIALLIBRARY::Get()->Add(materialproc);
	downbutton->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"SDF"));
	downbutton->GetNode()->SetMaterial(materialproc);
	
	spinbox->AddElement(downbutton);
	

	scene->Add(textdisplay);
	scene->Add(spinbox);
}





/*-------------------------------------------------------------------
//	UIWIDGETSPINBOXDRAWABLE::InternalDraw
*/	
/**	
//	
//	Draws the widget
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			10/02/2016 10:29:54
//	
//	@return 			bool : 
//
//  @param				widgetlevel : 
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETSPINBOXDRAWABLE::InternalDraw(XDWORD& widgetlevel)
{
	float scalex = gpt1x - gpt0x;
	float scaley = gpt1y - gpt0y;
	
	scaley /= scene->GetViewport()->GetAspectRatio();
	
	AdjustText(gpt0x + 0.01f, gpt0y+0.01f, gpt1x - scaley - 0.01f, gpt1y-0.01f, widgetlevel + 0.5f);
	
	if(modified)
	{
		GRPPOINT pt = scene->GetViewport()->GetViewPortFromInverseNormalized(gpt0x, gpt0y, 0.0f);
	
		float x = pt.x + scalex;
		float y = pt.y - scaley;
	
		background->GetNode()->SetPosition(x, y, widgetlevel + 0.1f);
		background->GetNode()->SetScale(scalex, scaley, 1.0f);
		background->SetRenderPriority(widgetlevel);
	
		//// Draw the control boxes
		upbutton->GetNode()->SetScale(scaley, scaley/2.0f, 1.0f);
		//upbutton->GetNode()->SetPosition(pt.x+scalex-scaley, pt.y + scaley/2.0f, widgetlevel + 0.3f);
		upbutton->GetNode()->SetPosition(x+scalex - scaley, y+scaley/2.0f, widgetlevel + 0.3f);
		upbutton->SetRenderPriority(widgetlevel);
		//
		downbutton->GetNode()->SetScale(scaley, scaley/2.0f, 1.0f);
		downbutton->GetNode()->SetPosition(x+scalex - scaley, y-scaley/2.0f, widgetlevel + 0.3f);
		downbutton->SetRenderPriority(widgetlevel);
	
		GRPPOINT realpt = scene->GetViewport()->GetViewPortFromInverseNormalized(gpt0x, gpt1y, 0.0f);
	
		background->GetNode()->Update();
		background->GetNode()->UpdateWorldMatrix();
		
		upbutton->GetNode()->Update();
		upbutton->GetNode()->UpdateWorldMatrix();
		
		downbutton->GetNode()->Update();
		downbutton->GetNode()->UpdateWorldMatrix();
		
		//textbox->GetNode()->SetPosition(x, y, widgetlevel + 0.1f);
		//textdisplay->GetNode()->SetPosition(realpt.x, realpt.y, widgetlevel + 0.5f);
		//float lineheight = font->GetLineHeight();
		//float scale = (gpt1y - gpt0y)/lineheight;
		//
		//// set appropiate scale
		//textdisplay->GetNode()->SetScale(scale*1.0f, scale*1.0f, 1.0f);
	
		this->SetActive(this->isactive);
	}
	
	background->Publish(GRPMSG_ORDER);
	upbutton->Publish(GRPMSG_ORDER);
	downbutton->Publish(GRPMSG_ORDER);

	return true;
}





/*-------------------------------------------------------------------
//	UIWIDGETSPINBOXDRAWABLE::InternalUpdate
*/	
/**	
//	
//	Behavior of the widget
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			10/02/2016 10:30:30
//	
//	@return 			bool : 
//
//  @param				widgetlevel : 
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETSPINBOXDRAWABLE::InternalUpdate(XDWORD widgetlevel)
{
	float cy = (gpt1y - gpt0y);
	float cyx = cy / scene->GetViewport()->GetAspectRatio();

	if(uimanager->GetInput()->IsTriggered(INPBUTTON_ID_MOUSE_LEFT))
		{
			// check collision against up and down
			float x, y;
			uimanager->GetInput()->GetCursorPosition(x, y);
			
			GRPPOINT pt = scene->GetViewport()->GetViewPortFromPixels(x, y);
			
			pt = scene->GetViewport()->GetInverseNormalizeFromViewPort(pt.x, pt.y);
			
			// need to compute the corners of the up and down buttons
			
			float lpt0x = gpt1x - cyx;
			float lpt0y = gpt0y;
			float lpt1x = gpt1x;
			float lpt1y = gpt0y + cy/2.0f;
			
			// collision with upper button
			if(PointRectangle(pt.x, pt.y, lpt0x, lpt0y, lpt1x, lpt1y))
				{
					this->Increment();
					UIEVENT event(NULL, XEVENTTYPE_UI);
					event.SetUIEventType(UIXEVENTTYPE_VALUE_CHANGE);
					event.SetSender(this);
					this->HandleEvent(&event);
				}
			
			// collision with lower button
			lpt0x = gpt1x - cyx;
			lpt0y = gpt0y + cy/2.0f;
			lpt1x = gpt1x;
			lpt1y = gpt1y;
			
			// collision with lower button
			if(PointRectangle(pt.x, pt.y, lpt0x, lpt0y, lpt1x, lpt1y))
				{
					this->Decrement();
					UIEVENT event(NULL, XEVENTTYPE_UI);
					event.SetUIEventType(UIXEVENTTYPE_VALUE_CHANGE);
					event.SetSender(this);
					this->HandleEvent(&event);
				}
		}

	if(upwidget)
		{
			// need to compute correctly origin x
			upwidget->SetPosition(GetWidth()-GetHeight(), 0);
			upwidget->SetSize(GetHeight(), GetHeight()/2);
		}

	if(downwidget)
		{
			upwidget->SetPosition(GetWidth()-GetHeight(), GetHeight()/2);
			upwidget->SetSize(GetHeight(), GetHeight()/2);
		}

	return UIWIDGETSPINBOX::InternalUpdate(widgetlevel);
}





/*-------------------------------------------------------------------
//	UIWIDGETSPINBOXDRAWABLE::SetActive
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			10/06/2016 13:25:07
//	
//  @param				active : 
*/
/*-----------------------------------------------------------------*/
void UIWIDGETSPINBOXDRAWABLE::SetActive(bool active)
{
	spinbox->SetVisible(active);
	textdisplay->SetVisible(active);

	if(upwidget)
		{
			upwidget->SetActive(active);
			upbutton->SetVisible(false);
		}

	if(downwidget)
		{
			downwidget->SetActive(active);
			downbutton->SetVisible(false);
		}

	//background->SetVisible(active);
	//upbutton->SetVisible(active);
	////uparrow->SetVisible(active);
	//downbutton->SetVisible(active);
	////downarrow->SetVisible(active);

	UIWIDGET::SetActive(active);
}




/*-------------------------------------------------------------------
//	UIWIDGETSPINBOXDRAWABLE::SetBoxWidgets
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			16/06/2016 15:53:30
//	
//  @param				upwidget : 
//  @param				downwidget : 
*/
/*-----------------------------------------------------------------*/
void UIWIDGETSPINBOXDRAWABLE::SetBoxWidgets(UIWIDGETCONTAINER* upwidget, UIWIDGETCONTAINER* downwidget)
{
	this->upwidget = upwidget;
	this->downwidget = downwidget;

	if(upwidget)
		{
			this->AddChild(upwidget);
			upbutton->SetVisible(false);
		}

	if(downwidget)
		{
			this->AddChild(downwidget);
			downbutton->SetVisible(false);
		}
}



/*-------------------------------------------------------------------
//	UIWIDGETSPINBOXDRAWABLE::AdjustText
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			14/06/2016 16:09:15
//	
//  @param				lpt0x : 
//  @param				lpt0y : 
//  @param				lpt1x : 
//  @param				lpt1y : 
*/
/*-----------------------------------------------------------------*/
void UIWIDGETSPINBOXDRAWABLE::AdjustText(float lpt0x, float lpt0y, float lpt1x, float lpt1y, float widgetlevel)
{
	// update the text contents
	if(contentmodified)
		{
			XSTRING str;
			
			if(isinteger)
				{
					str.Format(__L("%d"), (XDWORD)value);
				}
			else
				{
					str.Format(__L("%f"), value);
				}

			textdisplay->SetContent(str.Get());
			textdisplay->MergeElements();
			textdisplay->GetElements()->FastGet(0)->UpdateBoundingBox();

			contentmodified = false;
			modified = true;
		}

	float maxpointx = textdisplay->GetElements()->FastGet(0)->GetBoundingBox()->maxpoint.x;
	float minpointx = textdisplay->GetElements()->FastGet(0)->GetBoundingBox()->minpoint.x;

	float maxpointy = textdisplay->GetElements()->FastGet(0)->GetBoundingBox()->maxpoint.y;
	float minpointy = textdisplay->GetElements()->FastGet(0)->GetBoundingBox()->minpoint.y;

	GRPPOINT nmax = scene->GetViewport()->GetInverseNormalizeFromViewPort(maxpointx, maxpointy);
	GRPPOINT nmin = scene->GetViewport()->GetInverseNormalizeFromViewPort(minpointx, minpointy);

	float nscalex = nmax.x - nmin.x;
	float nscaley = nmin.y - nmax.y;

	// probably not
	float gscalex = lpt1x - lpt0x;
	float gscaley = lpt1y - lpt0y;

	float scalex = lpt1x - lpt0x;
	float scaley = lpt1y - lpt0y;

	if(nscalex > scalex)
		{
			// adjust in horizontal, which is not this, as this way we arre adjusting the size we were passed instead of our size
			float ratiox = scalex/nscalex;
			nscalex *= ratiox;
			nscaley *= ratiox;
		}

	if(nscaley > scaley)
		{
			// adjust in vertical
			float ratioy = scaley/nscaley;
			nscalex *= ratioy;
			nscaley *= ratioy;
		}

	scalex = nscalex;


	GRPPOINT pt = scene->GetViewport()->GetViewPortFromInverseNormalized(lpt0x, lpt0y, 0.0f);
	
	//scaley /= scene->GetViewport()->GetAspectRatio();

	float x = pt.x;// + scalex;
	float y = pt.y;// - gscaley;

	//XSTRING v = __L("t");

	float normalizationscale = maxpointx - minpointx;

	float verticalsize = maxpointy - minpointy;
	verticalsize /= normalizationscale;

	// I will need vertical normalized box to compute how to place the text vertivally

	float haligment = 0.0f;
	float valigment = 0.0f;

	// now we need to align if we have wiggle room
	//if(horizontalaligment == UIWIDGET_ALIGNFLAGS_CENTER)
	//	{
	//		float diff = gscalex - nscalex;
	//		haligment = diff/2.0f;
	//	}
	//else if(horizontalaligment == UIWIDGET_ALIGNFLAGS_RIGHT)
		{
			float diff = gscalex - nscalex;
			haligment = diff;
		}


	//if(verticalaligment == UIWIDGET_ALIGNFLAGS_CENTER)
		{
			float diff = gscaley - nscaley;
			valigment = diff/2.0f;
		}
	//else if(verticalaligment == UIWIDGET_ALIGNFLAGS_TOP)
	//	{
	//		float diff = gscaley - nscaley;
	//		valigment = diff;
	//	}

	GRPPOINT realpt = scene->GetViewport()->GetViewPortFromInverseNormalized(lpt0x + haligment, lpt1y - valigment, 0.0f);

	textdisplay->GetNode()->SetPosition(realpt.x - 2.0f*minpointx*scalex/normalizationscale, realpt.y, widgetlevel); // need to really calculate this correctly


	// set appropiate scale
	textdisplay->GetNode()->SetScale(2.0f*scalex/normalizationscale, 2.0f*scalex/normalizationscale, 1.0f);
	
	textdisplay->GetNode()->Update();
	textdisplay->GetNode()->UpdateWorldMatrix();

	scene->Add(textdisplay);
}
