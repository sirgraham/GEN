/*------------------------------------------------------------------------------------------
//	GRPFONT.H
*/	
/**	
// \class 
//   
//  Font 3D class
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 18/08/2014 12:16:10
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPFONT_H_
#define _GRPFONT_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include <stdio.h>

#include "GRPTextBox.h"
#include "GRPObjectBuilderAlgorithmStroke.h"

#include "XMap.h"
#include "XString.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

#define TTFGLOBALSCALE 6853.0f

	
enum GRPFONTSTATUS 
{
	GRPFONTSTATUS_OK													=	0 ,
	GRPFONTSTATUS_ERROR_FAILEDMEMORY						  ,
	GRPFONTSTATUS_ERROR_FILENOTFOUND							,
	GRPFONTSTATUS_ERROR_GLYPH_UNDEFINED						,
	GRPFONTSTATUS_ERROR_GPU_LOAD_FAILED
};	


/*---- CLASS -----------------------------------------------------------------------------*/

class XFACTORY;
class XPATH;

class GRPSHAPE;
class GRPMATERIAL;
class GRPELEMENT;
class GRPOBJECT;

class GRPFILETTF;


enum GRPFONT_STYLE
{
		GRPFONT_STYLE_REGULAR=0,
		GRPFONT_STYLE_BOLD	 =1,
		GRPFONT_STYLE_ITALIC =2,
		GRPFONT_STYLE_ITALIC_BOLD = 3,
		GRPFONT_STYLE_BOLD_ITALIC = 3
};


class GRPFONTATTRIBUTES
{
	public:
	
																GRPFONTATTRIBUTES					()													{  Clean();																}
		virtual										 ~GRPFONTATTRIBUTES					()													{  Clean();																}

		int													quality;
		
		float																			borderextrusion;
		float																			innerborder;
		float																			outerborder;
		GRPOBJECTBUILDERALGORITHMSTROKEJOINTTYPE	jointtype;
		GRPOBJECTBUILDERALGORITHMSTROKEPENTYPE		pentype;


		 
		bool												front;
		bool												back;
		float												extrude;
		float												bevel;
		bool												monospace;

		GRPMATERIAL*								material;		

		int													shaderprogram;
		XDWORD											align;
		bool												isReversed;

		bool												unifynormals;
		bool												reversenormals;

	private:

		void												Clean											()
																{
																	quality					=	4;

																	innerborder			=	0.0f;
																	outerborder			=	0.0f;
																	borderextrusion = 0.0f;

																	front						=	true;
																	back						=	false;
																	extrude					= -0.5f;
																	material				=	NULL;
																	shaderprogram		=	0;
																	isReversed			= false;
																	pentype					= GRPOBJECTBUILDERALGORITHMSTROKEPENTYPE_NORMAL;
																	jointtype				= GRPOBJECTBUILDERALGORITHMSTROKEJOINTTYPE_BEVEL;
																	align						= 0;
																	bevel						= 0.0f;
																	unifynormals		= false;
																	reversenormals  = false;
																}
};
	
	


class GRPFONT : public GRPNAMED
{
	public:			
																GRPFONT										();
															 ~GRPFONT										();		
															
		bool												LoadTTF										(XCHAR* xpath);
		bool												SetAttributes							(GRPFONTATTRIBUTES* attributes);
						
		bool												GetMetrics								();
		GRPSHAPE*										GetGlyphShape							(XCHAR letter);		
		GRPELEMENT*									CreateGlyph								(XCHAR letter);
									
		GRPTEXTBOX*									TextBox										(GRPSCENE*	scene,	XCHAR*		mask,							...);
		GRPTEXTBOX*									TextBox										(GRPSCENE*	scene,	int				alignment_type,		XCHAR* mask , ...);		
							
		GRPTEXTBOX*									CreateTextBox							(GRPSCENE*	scene,	XCHAR*		word,				int			alignment_type=GRPTEXTBOX_ALIGNRIGHT);
		GRPTEXTBOX*									CreateTextBox							(GRPSCENE*	scene,	GRPNODE*	node,				XCHAR*	word,				int alignment_type=GRPTEXTBOX_ALIGNRIGHT);
		GRPTEXTELEMENT*							CreateTextElement					(GRPNODE*		node,		XCHAR*		word,				int			alignment_type=GRPTEXTBOX_ALIGNRIGHT);
		GRPTEXTELEMENT*							CreateTextBlock						(GRPNODE*		node,		XCHAR*		word,				GRPAABB*box);

		bool												MeasureWidth							(float			width,	XSTRING&	original,	XSTRING& trimed, XSTRING& rest);
		GRPFONT_STYLE								SetStyle									(XCHAR*);

		int													GetLastError							() { return lasterror; }
		void												SetLastError							(int error) { this->lasterror = error; }
		GRPNAMED_TYPE								GetType										() { return GRPNAMED_TYPE_FONT; }

		float												GetSpaceWidth							()													{ return spacewidth;											}

		XMAP<XCHAR,GRPELEMENT*>*		GetLetters								()													{ return &letters;												}
		GRPFONTATTRIBUTES*					GetAttributes							()													{ return &attributes;											}
		
		bool												GetIsMonospace						()													{ return	UseAsMonospace;									}
		void												SetIsMonospace						(bool b)										{					UseAsMonospace=b;								}
		float												GetMonospaceWidth					()													{ return	monospacewidth;									}
		float												GetLineHeight							()													{ return  lineheight;											}
		float												GetLineGap								()													{ return  linegap; }
		float												GetBaseLine								()													{ return  baseline;														}
		float												GetMaxAdvance							()													{ return  maxadvance;											}

		GRPFILETTF*									GetTTF										()													{	return fileTTF;													}

		XSTRING*										GetFile										()													{	return &filename;												}

		float												lineheight;
		float												baseline;
		float												maxadvance;
		float												spaceadvance;
		float												leftSideBearing;
		float												ascend;
		float												descend;
		float												linegap;

	protected:

		XDWORD											style;

		XSTRING											filename;
		
		GRPFILETTF*									fileTTF;
		int													lasterror;
		
		XMAP<XCHAR,GRPSHAPE*>				shapes;
		XMAP<XCHAR,GRPELEMENT*>			letters;

		bool												UseAsMonospace;

		GRPFONTATTRIBUTES						attributes;
		float												spacewidth;
		float												monospacewidth;



	private:

		void												Clean											()
																{
																	lasterror				=	GRPFONTSTATUS_OK;
																	spacewidth			=	1.0f;
																	monospacewidth	=	0.0f;
																	baseline				= 0.0f;
																	ascend					= 0.0f;
																	descend					= 0.0f;
																	linegap					= 0.0f;
																	UseAsMonospace	= false;
																}
};



/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

