/*------------------------------------------------------------------------------------------
//	GRPTEXTBOX.H
*/	
/**	
// \class 
//   
//  Creates 3d text box
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 10/11/2014 17:23:15
//	Last Modification	:	
*/	
//*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPTEXTBOX_H_
#define _GRPTEXTBOX_H_
	

	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPObject.h"
#include "GRPArrayAlignTool.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

enum GRPTEXTBOX_ALIGNTYPE
{
	GRPTEXTBOX_ALIGNLEFT	=0,
	GRPTEXTBOX_ALIGNCENTER=1,	
	GRPTEXTBOX_ALIGNRIGHT =2,		
};

enum GRPTEXTBOX_DIRECTIONTYPE
{
GRPTEXTBOX_HORIZONTAL	=0,	
GRPTEXTBOX_VERTICAL		
};

class GRPFONT;
class GRPSCENE;

/*---- CLASS -----------------------------------------------------------------------------*/

class GRPTEXTCONTAINER : public GRPINTERFACEANIMABLE
{
public:
															 			GRPTEXTCONTAINER	();
virtual															~GRPTEXTCONTAINER	();

virtual		bool											UpdateContent			(bool				mergeelements = true)																=0;
virtual		bool											SetValue					(GRPANIMATIONCURVETYPE,	GRPANIMATIONCURVEAXIS,	double,	double)	=0;

					bool											SetContent				(XSTRING*		data,		XDWORD * size=NULL);
					bool											SetContent				(XCHAR*			mask,		...);					
					bool											SetContentMask		(XCHAR*			mask);						 
					int												FindGlyphIndex		(XCHAR			character);
					void											SetFont						(GRPFONT*		font);
					GRPFONT*									GetFont						();
					void											SetWidth					(float			fwidth);
					void											SetAlign					(XDWORD			xalign);
					void											SetDistribution		(XDWORD			distribution);
					GRPARRAYALIGNTOOL*				GetAlignTool			();
					XSTRING *									GetContent				();					
					XSTRING*									GetContentMask		();
					void											SetBoundaries			(GRPAABB* boundingbox)				{ this->bound = boundingbox; }
					void											SetDirection			(GRPTEXTBOX_DIRECTIONTYPE d)	{ this->direction= d;	}
					double										GetValue					()														{ return value;				}
					float											GetHeight					()														{ return lineheight;}

protected:

					XSTRING										lastValue;

					float											width;
					XSTRING										contentmask;
					GRPARRAYALIGNTOOL					aligntool;
					GRPSCENE*									scene;
					XDWORD										align;
					GRPTEXTBOX_DIRECTIONTYPE	direction;
					XDWORD										distribution;
					GRPFONT*									font;
					XSTRING										Content;
			
					GRPELEMENT*								GetIndexedGlyph	(XCHAR character);

					bool											isready;
	
					double										value;

virtual		void											Clear						()=0;

					XMUTEX*										xmutex;

					
					float											lineheight;

					GRPAABB*									bound;

private:

		void	Clean	()
		{
			lineheight=0.0f;

			Content.Empty();
			align										= GRPTEXTBOX_ALIGNCENTER;			
			aligntool.distribution	= GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_GLYPHS; 
			direction								= GRPTEXTBOX_HORIZONTAL;
			this->font							=	NULL;			
			width										=	0.0f;
			scene										=	NULL;
			contentmask							= __L("%.02f");
			isready									= true;
			xmutex									= NULL;
			value										= 0.0f;
			bound										= NULL;
		}
};

class GRPTEXTBOX : public GRPOBJECT,public GRPTEXTCONTAINER
{
public:	

															GRPTEXTBOX			(GRPSCENE*	scene);
virtual												~GRPTEXTBOX			();

					void								SetTag					(XQWORD tag)  { GRPOBJECT::SetTag(tag);}

					bool								UpdateContent		(bool				mergeelements = true);

virtual		void								SetNode					(GRPNODE*		n);	 

					bool								SetValue				(GRPANIMATIONCURVETYPE type,GRPANIMATIONCURVEAXIS axis,double value,double base);
					void								Clear						();

protected:		



private:

		void	Clean	()
		{

		}
		
};

class GRPTEXTELEMENT : public GRPTEXTCONTAINER, public GRPELEMENT
{
public:
	virtual		bool								UpdateContent			(bool				mergeelements = true);		
	virtual		bool								SetValue					(GRPANIMATIONCURVETYPE,	GRPANIMATIONCURVEAXIS,	double,	double);
	virtual		void								Clear							();
};




/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

