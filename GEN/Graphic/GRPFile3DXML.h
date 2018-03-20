/*------------------------------------------------------------------------------------------
//	GRPFILE3DXML.H
*/	
/**	
// \class 
//   
//  Loads multiple layers from xml 
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 28/11/2016 10:28:49
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPFILE3DXML_H_
#define _GRPFILE3DXML_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
		
#include "XString.h"
#include "XVector.h"
#include "XMap.h"
#include "XFileXML.h"
#include "GRPFile3D.h"
#include "GRPFontManager.h"

#include "AILogic.h"
#include "AIState.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
class GRPOBJECT;
class GRPELEMENT;
class GRPLOADERTASK;
class XTIMER;
	
/*---- CLASS -----------------------------------------------------------------------------*/
class GRPFILE3DXML : public GRPFILE3D
{
public:

	enum XML_FLAGS
	{

		XML_FLAG_HEAD					=1<<0,
		XML_FLAG_CONFIG				=1<<1,
		XML_FLAG_STRINGS			=1<<2,		
		XML_FLAG_LAYERS				=1<<3,
		XML_FLAG_STATES				=1<<4,
		XML_FLAG_MILESTONES		=1<<5,
		XML_FLAG_OVERLAYS			=1<<6,
		XML_FLAG_TEXT					=1<<7,
		XML_FLAG_COMMANDS			=1<<8,
		XML_FLAG_AUDIO				=1<<9,
		XML_FLAG_MAX,
		XML_FLAG_ALL					=XML_FLAG_HEAD | XML_FLAG_CONFIG | XML_FLAG_STRINGS | XML_FLAG_LAYERS | XML_FLAG_STATES | XML_FLAG_MILESTONES | XML_FLAG_OVERLAYS | XML_FLAG_TEXT | XML_FLAG_COMMANDS | XML_FLAG_AUDIO	
	};	



	class DATA
	{
	public:
		DATA() { assigned=false;}
		XSTRING														name;
		bool															assigned;
	};

	//------------------------------------------------ fonts -----------------------
	class FONT : public DATA
	{
	public:

		XSTRING														file;		
		GRPFONTATTRIBUTES									attributes;
	};


	//------------------------------------------------texts -------------------------
	class TEXTBOX : public DATA
	{
	public:		
		XSTRING value;
		XSTRING mask;
		XSTRING material;
		XSTRING font;
		XSTRING color;
		XSTRING	align;

		XDWORD	distribution;		

		bool		cellspacingrelative;
		float		cellspacing;
		bool		cellpaddingrelative;
		float		cellpadding;
		bool		cellwidthrelative;
		float		cellwidth;
		bool		cellheightrelative;
		float		cellheight;		
	};

	
	//------------------------------------------------ lights -----------------------
	class LIGHT : public DATA
	{
	public:		
		enum
		{
			NONE=0,
			MULTIPLIER						=1<<0,
			CONSTANT_ATTENUATION	=1<<1,
			LINEAR_ATTENUATION		=1<<2,
			QUADRIC_ATTENUATION		=1<<3,
			CUBIC_ATTENUATION			=1<<4
		} FLAG;

		XDWORD flags;
		
		float multiplier;		
		float constantAttenuattion;
		float linearAttenuattion;
		float quadricAttenuattion;
		float cubicAttenuattion;
	};

	//------------------------------------------------ milestones -----------------------
	class MILESTONE : public DATA
	{
	public:
		bool															active;
		float															start,end;
		XDWORD														tag;
		int																passes;
		GRPANIMATIONSTACKMILESTONE::TYPE	type;
	};

	//---------------------------------------------- audio ------------------------	
	class MEDIA : public DATA
	{
	public:
		XDWORD	id;		
		XSTRING file;
		float		volume;
		XSTRING playlist;
	};

	class MUSIC : public DATA
	{
	public:
			XDWORD	sampleid;
			XSTRING playlist;
			XSTRING samplename;
			float   volume;
			bool		loop;
			bool		status;
	};

	class SOUND : public DATA
	{
	public:

		static float mastervolume;
	
		XDWORD	type;
		XDWORD	sampleid;		

		XSTRING playlist;
		XSTRING samplename;
		float   samplevolume;

		XDWORD	layerID;
		XSTRING layer;

		bool		start_isoffset;
		float		startframe;

		bool		end_isoffset;
		float		endframe;

		float		volume;

		int			passes;
		bool		loop;

		bool		isremote;
		bool		islocal;		
	};

	//------------------------------------------------ layer -----------------------
	class LAYER : public DATA
	{
	public:
		~LAYER()
		{
			milestones.DeleteContents();
			milestones.DeleteAll();
		}

		XSTRING														file;		
		XDWORD														layerID;
		bool															isActive;
		XDWORD														loadFlags;
		GRPANIMATIONLOOPMODE							loop;
		double														speed;

		XQWORD														trimstart;
		XQWORD														trimend;
		XBYTE															trim;

		XVECTOR<MILESTONE*>								milestones;
		XVECTOR<SOUND*>										sounds;
	};

	//------------------------------------------------ overlay -----------------------
	class OVERLAY : public DATA
	{
	public:
		~OVERLAY()			{				requires.DeleteAll();				attach.DeleteAll();			}

		XDWORD														id;
		XDWORD														status;		

		XVECTOR<XDWORD>										requires;
		XMAP<XDWORD,XDWORD>								attach;
	};

	//-----------------------------------------------command ----------------------

	class COMMAND : public DATA
	{
	public:
		XSTRING name;
		XDWORD id;
		XDWORD type;
		XDWORD value;
	};

	//-------------------------------------------- language ---------------------

	class LANGUAGE : public DATA
	{
	public:
		~LANGUAGE() 
		{ 
			strings.DeleteKeyContents();
			strings.DeleteElementContents();
			strings.DeleteAll();
		}
		
		XMAP<XSTRING*,XSTRING*> strings;
	};

	//------------------------------------------------ materials -----------------

	class MATERIAL : public DATA
	{
	public:																
			class SLOT
			{
			public:
					enum
					{
							TYPE_2D,
							TYPE_CUBEMAP,
							TYPE_SEQUENCE
					};
					
					enum 
					{
							FRONT=0,
							BACK,
							LEFT,
							RIGHT,
							TOP,
							BOTTOM
					};
																		~SLOT() { properties.DeleteElementContents(); }
					XDWORD										id;
					XDWORD										slottype;
					XMAP<XDWORD, XSTRING*>		properties;
			};

														~MATERIAL() { slots.DeleteElementContents(); }
			XSTRING								name;
			XMAP<XDWORD, SLOT*>		slots;
	};

	//------------------------------------------------ pass -----------------

	class PASS : public DATA
	{
	public:
			PASS()
			{
					clone			= false;
					rect.x1		= rect.x2 = rect.y1 = rect.y2 = 0.0f;
					priority	= 100;
					clear			= GRPPASS_CLEAR_FLAG_ALL;
			}

			~PASS()
			{
					accept.DeleteContents();
					reject.DeleteContents();

					options.DeleteKeyContents();
					options.DeleteElementContents();
			}

			XSTRING name;
			XSTRING type;
			XDWORD priority;
			XDWORD flags;
			XDWORD clear;

			bool clone;
			XSTRING camera;
			XSTRING light;
			XSTRING viewport;
			XSTRING material;

			GRPRECTFLOAT rect;

			XVECTOR<XSTRING*> accept;
			XVECTOR<XSTRING*> reject;

			XMAP<XSTRING*, XSTRING*> options;
	};

	class ELEMENT
	{
	public:
			enum
			{
					SHADER     = 1 << 0,
					PRIORITY   = 1 << 1,
					MATERIAL   = 1 << 2,
					VISIBILITY = 1 << 3
			};

			XSTRING name;
			XSTRING shader;
			XSTRING material;

			XDWORD  priority;
			XDWORD  flags;

			bool		visible;
	};

	//------------------------------------------------ load -----------------------																																
	virtual			GRPOBJECT*												Load									(XCHAR*	file);
							
protected:

							void															Release								();
							bool															ParseFile							();

							bool															ProcessHead									(XFILEXMLELEMENT* element);
							bool															ProcessConfig								(XFILEXMLELEMENT* element);
							bool															ProcessDefinitions					(XFILEXMLELEMENT* element);
							bool															ProcessData									(XFILEXMLELEMENT* element);
							bool															ProcessColors								(XFILEXMLELEMENT* element);
							bool															ProcessVariables						(XFILEXMLELEMENT* element);
							bool															ProcessStrings							(XFILEXMLELEMENT* element);
							bool															ProcessLenguage							(XFILEXMLELEMENT* element,LANGUAGE*);
							bool															ProcessLayers								(XFILEXMLELEMENT* element);
							bool															ProcessStates								(XFILEXMLELEMENT* element);
							bool															ProcessAnimations						(XFILEXMLELEMENT* element,AI::STATE*);
							bool															ProcessTransitions					(XFILEXMLELEMENT* element,AI::STATE*);
							bool															ProcessMilestones						(XFILEXMLELEMENT* element);
							bool															ProcessOverlays							(XFILEXMLELEMENT* element);
							bool															ProcessTexts								(XFILEXMLELEMENT* element);
							bool															ProcessCommands							(XFILEXMLELEMENT* element);
							bool															ProcessCommandChangeState		(XFILEXMLELEMENT* element,AI::COMMAND*);
							bool															ProcessCommandPushState			(XFILEXMLELEMENT* element,AI::COMMAND*);
							bool															ProcessCommandChangeOverlay	(XFILEXMLELEMENT* element,AI::COMMAND*);
							bool															ProcessCommandAlterProperty	(XFILEXMLELEMENT* element,AI::COMMAND*);
							bool															ProcessGeometry							(XFILEXMLELEMENT* element);
							bool															ProcessMaterials						(XFILEXMLELEMENT* element);
							bool															ProcessPasses								(XFILEXMLELEMENT* element);
							bool															ProcessAudio								(XFILEXMLELEMENT* element);
							bool															ProcessMedia								(XFILEXMLELEMENT* element);
							bool															ProcessPlaylist							(XFILEXMLELEMENT* element,XSTRING& playlist);
							bool															ProcessSound								(XFILEXMLELEMENT* element);
							bool															ProcessMusic								(XFILEXMLELEMENT* element);
							bool															ProcessElements							(XFILEXMLELEMENT* element);

							bool															ApplyConfig									();
							bool															LoadModel										();

							bool															LoadLayers									();
							bool															LoadSounds									();
							bool															LoadOverlays								();
							bool															LoadLanguages								();
							bool															LoadMaterials								();
							bool															LoadPasses									();

							XDWORDSIG													GetValueByDefinition				(XCHAR* name);

							XDWORD														GetFlags										();
							void															SetFlags										(XDWORD flag);
							void															AddFlag											(XML_FLAGS flag);
							void															RemoveFlag									(XML_FLAGS flag);
										
							void															Abort												();
							
							void															Output											(XDWORD			priority,		XCHAR*			mask,...);
							bool															Notify											(GRPMSGTYPE type,				GRPSUBJECT* subject);
	
	XMAP		<XSTRING*,	XSTRING*>									header;
	XMAP		<XSTRING*,	XDWORD>										definitions;
	XMAP		<XDWORD,		GRPCOLOR*>								colors;
	XMAP		<XSTRING*,	XSTRING*>									layerOptions;

	XMAP		<XSTRING*,	GRPFILE3DXML::LANGUAGE*>	languages;
	XMAP		<XSTRING*,	GRPFILE3DXML::FONT*		>		font;
	XMAP		<XSTRING*,	GRPFILE3DXML::TEXTBOX*>		texts;	
	XMAP		<XSTRING*,	GRPFILE3DXML::LIGHT*	>		lights;
	XMAP		<XSTRING*,	GRPFILE3DXML::COMMAND*>		commands;		
	XMAP	  <XSTRING*,	GRPFILE3DXML::ELEMENT*>   elements;
	
	XMAP		<XDWORD,		GRPFILE3DXML::MEDIA*	>		media;
	XMAP		<XDWORD,		GRPFILE3DXML::MUSIC*	>		music;
	XMAP		<XDWORD,		GRPFILE3DXML::LAYER*	>		layers;
	XMAP		<XDWORD,		GRPFILE3DXML::OVERLAY*>		overlays;

	XMAP		<XSTRING*, GRPFILE3DXML::MATERIAL*>   materials;
	XMAP		<XSTRING*, GRPFILE3DXML::PASS*>				passes;
	
	XVECTOR	<GRPFILE3DXML::SOUND*>								sounds;

	GRPVECTOR	GlobalScale;	
	XFILEXML2	xmlfile;
	XTIMER*		timer;
	bool			showdebug;	

	XDWORD		flags;

};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

