/*------------------------------------------------------------------------------------------
//	UIWIDGETSPINBOX.H
*/	
/**	
// \file 
//   
//  creates a spinbox
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 02/11/2015 11:58:44
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _UIWIDGETSPINBOX_H_
#define _UIWIDGETSPINBOX_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
#include "UIWidgetForm.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/

class UIWIDGETIMAGE;
class UIWIDGETTEXT;
class UIWIDGETBUTTON;

enum UIWIDGETSPINBOX_STYLE
{
	UIWIDGETSPINBOX_STYLE_SPINBOX = 0,
	UIWIDGETSPINBOX_STYLE_PICKERHORIZONTAL,
	UIWIDGETSPINBOX_STYLE_PICKERVERTICAL,
};

class UIWIDGETSPINBOX : public UIWIDGETFORM
{
public:
																						UIWIDGETSPINBOX							(XCHAR* name, UIMANAGER * uimanagerarg);
																						UIWIDGETSPINBOX							(UIWIDGETSPINBOX* rhs);
	virtual																	 ~UIWIDGETSPINBOX							();

	virtual				UIWIDGETSPINBOX*						Clone												()
																						{
																							return new UIWIDGETSPINBOX(this);
																						}

								bool												IsInteger										();
								void												SetIsInteger								(bool isinteger);
								void												SetDelta										(double delta);
								double											GetDelta										();
								void												SetValue										(double value);
								double											GetValue										();

								void												Increment										();
								void												Decrement										();
								void												SetRange										(double minimum, double maximum);
								double											GetMinimum									();
								double											GetMaximum									();

								// visual aspect
								void												SetUpImages									(XCHAR* normal, XCHAR* hover, XCHAR* clicked);
								void												SetDownImages								(XCHAR* normal, XCHAR* hover, XCHAR* clicked);
								void												SetBackgroundImages					(XCHAR* background);

	
	virtual				bool												HandleInternalEvent					(XEVENT* event);

								void												SetColor										(XBYTE R, XBYTE G, XBYTE B);

								XBYTE												GetColorR										()																				{ return this->R;											}
								XBYTE												GetColorG										()																				{ return this->G;											}
								XBYTE												GetColorB										()																				{ return this->B;											}

								void												SetBold											(bool bold);
								void												SetItalic										(bool italic);

								bool												GetBold											()																				{ return this->bold;									}
								bool												GetItalic										()																				{ return this->italic;								}

								void												SetSpinboxStyle							(UIWIDGETSPINBOX_STYLE spinboxstyle)			{ this->spinboxstyle = spinboxstyle;	}
								UIWIDGETSPINBOX_STYLE				GetSpinboxStyle							()																				{ return this->spinboxstyle;					}

protected:
	virtual				bool												InternalDraw								(XDWORD& widgetlevel)											{ return false;												}
	virtual				bool												InternalUpdate							(XDWORD widgetlevel);

								UIWIDGETIMAGE*							background;
								UIWIDGETTEXT*								text;
								UIWIDGETBUTTON*							up;
								UIWIDGETBUTTON*							down;

								UIWIDGETIMAGE*							upimage;
								UIWIDGETIMAGE*							downimage;

								bool												isinteger;
								double											value;
								double											delta;
								double											minimum;
								double											maximum;

								bool												contentmodified;

								XBYTE												R;
								XBYTE												G;
								XBYTE												B;

								bool												bold;
								bool												italic;


								UIWIDGETSPINBOX_STYLE				spinboxstyle;
private:

								void												SetSpinboxStyle							();
								void												SetPickerStyleHorizontal		();
								void												SetPickerStyleVertical			();

								void												Clean												()
																						{
																							background					= NULL;
																							text								= NULL;
																							up									= NULL;
																							down								= NULL;

																							isinteger						= false;
																							value								= 0.0;
																							delta								= 0.0;
																							minimum							= 0.0;
																							maximum							= 0.0;
																							contentmodified			= false;

																							R										= 0;
																							G										= 0;
																							B										= 0;

																							bold								= false;
																							italic							= false;

																							spinboxstyle				= UIWIDGETSPINBOX_STYLE_SPINBOX;
																						}

};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

