
/*------------------------------------------------------------------------------------------
//	UIWIDGETTEXT.CPP
//	
//	basic text widget(more of a label)
//   
//	Author						: Imanol Celaya Ruiz de Alegria
//	Date Of Creation	: 08/10/2015 10:05:34
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "XDebug.h"

#include "UIManager.h"
#include "UIWidgetText.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/




/*-------------------------------------------------------------------
//	UIWIDGETTEXT::SetText
*/	
/**	
//	
//	sets the text of the widget
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			05/10/2015 10:06:18
//	
//	@return 			bool : true if the text was set
//
//  @param				texttoset : text to set to the widget
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETTEXT::SetText(XCHAR * texttoset)
{
	textchanged = true;
	return text.Set(texttoset);
}

/*-------------------------------------------------------------------
//	UIWIDGETTEXT::GetText
*/	
/**	
//	
//	Gets the text of the widget
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			05/10/2015 10:06:30
//	
//	@return 			XSTRING* : string that is to be printed(internal reference)
//
*/
/*-----------------------------------------------------------------*/
XSTRING* UIWIDGETTEXT::GetText()
{
	return &text;
}

/*-------------------------------------------------------------------
//	UIWIDGETTEXT::InternalDraw
*/	
/**	
//	
//	Draws the specific widget
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			05/10/2015 10:06:44
//	
//	@return 			bool : true if succeeded
//
//  @param				widgetlevel : level of the widget in the hierarchy
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETTEXT::InternalDraw(XDWORD& widgetlevel)
{
	// it originally also printed it's contents

	/*XSTRING drawname;
	drawname.Set(__L(""));

	for(XDWORD i = 0; i < widgetlevel; i++)
		{
			drawname.Add(__L("  "));
		}

	XSTRING drawtext;
	drawtext.Set(__L(""));
	drawtext.Add(drawname);
	drawtext.Add(text);
	drawname.Add(__L("WIDGETTEXT DRAW"));

	XDEBUG_PRINTCOLOR(0,drawname.Get());
	XDEBUG_PRINTCOLOR(0,drawtext.Get());*/

	//uimanager->GetUISkin()->Draw(widgetlevel, this);

	return false;
}

/*-------------------------------------------------------------------
//	UIWIDGETTEXT::InternalUpdate
*/	
/**	
//	
//	Updates the specific widget
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			05/10/2015 10:06:55
//	
//	@return 			bool :  true if succeeded
//
//  @param				widgetlevel :  level of the widget in the hierarchy
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETTEXT::InternalUpdate(XDWORD widgetlevel)
{
	return true;
}
