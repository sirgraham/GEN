/*------------------------------------------------------------------------------------------
//	GRPCONSOLE.H
*/	
/**	
// \file 
//   
//  graphic console implementation
//   
//	@author	 Diego Martinez Ruiz de Gaona e Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 09/10/2015 13:11:11
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPCONSOLE_H_
#define _GRPCONSOLE_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XConsole.h"
#include "XVector.h"
#include "XMap.h"
#include "XObserver.h"

#include "GRPObject.h"
#include "GRPPoint.h"
#include "GRPScreen.h"

#include "XMemory.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
class XPATHS;
class XMUTEX;

class GRPSCENE;
class GRPFONT;
class GRPTEXTBOX;
class GRPMATERIAL;
class GRPRENDERER;


class GRPSTYLE
{
	class ENTITY		{	public:	virtual bool ApplyStyle	(GRPSTYLE*)						{ return false;	} };
	class SELECTOR	{	public:	virtual bool Select			(GRPSTYLE::ENTITY* )	{ return false; } };
	class ATTRIBUTE { public: virtual bool Apply			(GRPSTYLE::ENTITY* )  { return false; } };

	enum
	{
	TEXT_ALIGN_RIGHT,
	TEXT_ALIGN_LEFT,
	TEXT_ALIGN_CENTER,
	TEXT_ALIGN_JUSTIFY
	};

	enum
	{
	WORD_WRAP_NONE,
	WORD_WRAP_BREAK,
	WORD_WRAP_SPACE
	};

	XVECTOR<XSTRING*>				target;	
	XMAP<XSTRING*,XSTRING*> attributes;

	static GRPSTYLE*				Create(XCHAR*) { return NULL;}
};



class GRPCONSOLE : public XCONSOLE, public GRPOBJECT, public XOBSERVER
{
public:

enum 
{
	CREATE_SCENE			=1<<0,
	CREATE_VIEWPORT		=1<<1,
	CREATE_FONT				=1<<2,
	CREATE_BACKGROUND	=1<<3,
	CREATE_PERSPECTIVE=1<<4,
};


			class CARET
			{
			public:

				CARET();

				GRPCOLOR	color;
				float			size;
				GRPFONT*	font;							
				GRPPOINT	lastpos;
				GRPPOINT	textposition;

				XCHAR			character;
				bool			blinking;
				bool			visible;

			private:

				void			Clean();				
			};


			class PRINTABLE : public CARET
			{
			public:					
					XSTRING*	str;
					GRPBB			area;					
					PRINTABLE		(	XCHAR* str=NULL,
												GRPCOLOR color=GRPCOLOR::WHITE,
												float size=0.12f, 
												GRPFONT* font=NULL);
					~PRINTABLE	();
			};				

																							GRPCONSOLE										();
																							GRPCONSOLE										( GRPSCENE	*);																							
																							GRPCONSOLE										( GRPSCREEN	*);
																							GRPCONSOLE										( GRPSCENE	*,			GRPSCREEN	*);

	virtual																		 ~GRPCONSOLE										();

								bool													Create												(XDWORD flags=0);
								bool													InitDefaultColors							();
								void													Destroy												();
								void													SetScreen											(GRPSCREEN	*	screen);
								bool													CreateBackgroundPlane					();

	virtual				bool													Maximize											();
								void													Resize												();
								bool													Update												();

								void													SetTextProgram								(GRPSHADERPROGRAM*	program);
								void													SetFont												(GRPFONT*						font);

								void													SetArea												(GRPBB*							area);
								void													SetArea												(float							xs,			float ys,		float xe,			float ye);
								void													SetViewport										(float							x,			float y,		float width,	float height);
								void													SetBackgroundMaterial					(GRPMATERIAL* 			material);

								void													HandleEvent										(XEVENT*						xevent);
	
								void													Add														(XCHAR*);

	virtual				bool													Print													(XCHAR*							str);
	virtual				bool													Print													(XDWORD							color,	XCHAR* str);	
								bool													Print													(GRPCOLOR						color,	XCHAR* str);
	virtual				bool													PrintImp											(XCHAR*							string);	


								void													SetColor											(GRPCOLOR						color);								
	virtual				bool													Clear													();									
								void													AddBackground									(GRPELEMENT*				element);
								bool													Notify												(GRPMSGTYPE,								GRPSUBJECT*);

								void													DeleteLine										(int								line);
								bool													ScrollLines										();
								bool													ScrollDown										();
								bool													ScrollUp											();
								bool													ScrollTop											();
								bool													ScrollBottom									();
																						

								XDWORD												GetSubjectType								()													{ return GRPNAMED_TYPE_GRPCONSOLE;	}
								XVECTOR<GRPELEMENT*>*					GetAttachments								()													{ return &attachments;							}
								void													SetFontScale									(float scale)								{ caret.size = scale;								}																																																					
								void													SetScene											(GRPSCENE* scene)						{ this->scene=scene;								}
								XMAP<XDWORD,GRPCOLOR*>*				GetColors											()													{ return &this->colors;							}
								XVECTOR<XSTRING*>*						GetLines											()													{ return &lines; 										}

								void													SetAutoScroll									(bool a)										{ this->auto_scroll=a;							}
																
private:

	void																				Clean													()
																							{

																								GRPNAMED::classtype = GRPNAMED_TYPE_GRPCONSOLE;
																								screen							= NULL;
																								scene								= NULL;
																								defaultmaterial			= NULL;
																								scene								= NULL;
																								font								= NULL;
																								background_material	= NULL;
																								plane								= NULL;
																								shaderprogram				= NULL;																																										
																								
																								caret.size					= 0.2f;
																								initialized					=	false;
																								ownsScene						=	false;
																								ownsViewport				= false;
																								ownsFont						= false;
																								tabsize							= 4;																								
																								scroll_start				= 0;																								
																								line								= 0;
																								scroll_size					= 1;
																								current_scroll			= 0;
																								auto_scroll					= true;
																								caret.lastpos.Clean();
																							}

	
	protected:
								void														MoveTextElementToConsole			(GRPTEXTBOX* textbox);										
								
								GRPMATERIAL*										defaultmaterial;
								GRPSCENE*												scene;
								GRPSCREEN*											screen;
								GRPFONT*												font;
								GRPNODE*												background;
								GRPELEMENT*											plane;
								GRPNODE*												textparent;
								XVECTOR<GRPELEMENT*>						lineelements;
								
								XVECTOR<GRPELEMENT*>						attachments;
								XVECTOR<GRPCONSOLE::PRINTABLE*>	toprintvector;
								XVECTOR<XSTRING*>								lines;								
								XMAP	 <GRPCOLOR, GRPMATERIAL*>	materials;
								
								XMUTEX*													xmutex;

								GRPBB														area;

								GRPSHADERPROGRAM*								shaderprogram;								
								GRPMATERIAL*										background_material;

								XMAP<XDWORD,GRPCOLOR*>					colors;

								int															line;

								int															current_scroll;
								int															scroll_start;
								int															scroll_end;
								XDWORD													scroll_size;
								float														line_height;
								float														line_width;
																					
								CARET														caret;
								int															tabsize;
								

								bool														ownsScene;
								bool														ownsViewport;
								bool														ownsFont;

								bool														auto_scroll;

								bool														initialized;						
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif


