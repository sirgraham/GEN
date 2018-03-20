/*------------------------------------------------------------------------------------------
//	GRPFILE3DGGF.H
*/	
/**	
// \class 
//   
//  Manages 3D file to hold scene info
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 31/03/2017 13:08:49
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPFILE3DGGF_H_
#define _GRPFILE3DGGF_H_
	

/*
      ++----> hierarchy                       ++> settings       +-> Faces
      |                                       |                  |
      +                                       +-> mesh  +--------v-> Vertex
Scene  +----> Settings                        |
      +                                       +-> deformers * +----> Bones
      |                                       |               |
      +----> Objects *   +------>  elements * +               +----> Skinning
      |                  |                                    |
      |                  +------>  locators *                 +----> Deformers
      |                  |                                    |
      |                  +------>  cameras *									+----> Weights
      |                  |                        
      |                  +------>  lights *  
      |                  |
      |                  +------>  data
      |                  |
      |                  +------>  Animation
      |                  |                +
      |                  |                +
      |                  |                |
      |                  |                +---> Stacks      +> Nodes
      |                  |                |       +         |
      |                  |                |       +->Layers +> Curves
      |                  |                |       |         |
      |                  |                |       |         +> Attachments
      |                  |                |       |
      |                  |                |       +->AnimationEvents
      |                  |                |
      |                  |                +--> Blending
      |                  |
      |                  |
      |                  +------>  logic  +----->  States
      |                                   |
      |                                   +----->  Controllers
      |                                   |
      +----->  Materials                  +----->  Commands
      |                                   |
      |                                   +------> Transitions
      +----->  Shaders                    |
      |                                   +------> Blends
      |
      +----->  Physics
      +
      +----->  Sounds
      |
      +----->  Media


+----------------------------------------------------------+      +--------+
|         ||                 ||                 ||           |     |         |
|  Header ||  Hierarchy      ||    Properties   ||  Block    | ... | Block   |
|         ||                 ||                 ||           |     |         |
+----------------------------------------------------------+      +--------+
*/

/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XMap.h"	
#include "GRPFile3D.h"
#include "XList.h"
#include "AIState.h"

class GRPFILE3DGGF;
class GRPNODE;
class GRPMESH;

class GRPDEFORMER;
class GRPSUBDEFORMER;

class GRPSKIN;
class GRPSKINLIMB;
class GRPSKINPOSE;
class GRPSKINCLUSTER;

class GRPELEMENT;
class GRPOBJECT;

class GRPLIGHT;
class GRPCAMERA;

class GRPANIMATIONCONTROL;
class GRPANIMATIONSTACK;
class GRPANIMATIONLAYER;
class GRPANIMATIONNODE;
class GRPANIMATIONCURVE;
class GRPANIMATIONSTACKMILESTONE;

class GRPMATERIAL;
class GRPTEXTURE;
class GRPSHADERPROGRAM;

class GRPUSERDATACONTAINER;


/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

#define		GGF_STATUS	0
#define		GGF_PROCESS 1
#define		GGF_WARNING 2
#define		GGF_NOTIFY	3
#define		GGF_ERROR		4

#ifdef XDEBUG
#define		GGFDEBUG this->Output
#else
#define		GGFDEBUG {}
#endif

#define GGFVECTOR XVECTOR
#define GGFMAP		XMAP


	enum GGF_SECTION_TYPE{
							GGF_SECTION_NONE,							
							GGF_SECTION_ROOT,
							GGF_SECTION_VERSION,
							GGF_SECTION_DICTIONARY,							
							GGF_SECTION_PROPERTY,
							GGF_SECTION_MEDIAFILE,
							GGF_SECTION_HEADER,
							GGF_SECTION_DEFINITIONS,
							GGF_SECTION_SCENE,
							GGF_SECTION_OBJECT,
							GGF_SECTION_ELEMENT,
							GGF_SECTION_MESH,
							GGF_SECTION_MESH_LAYER,
							GGF_SECTION_LOCATOR,
							GGF_SECTION_CAMERA,
							GGF_SECTION_LIGHT,

							GGF_SECTION_DEFORMER,							
							GGF_SECTION_SUBDEFORMER,
							GGF_SECTION_SKIN,							
							GGF_SECTION_SKINCLUSTER,
							GGF_SECTION_SKINPOSE,

							GGF_SECTION_ANIMATION,
							GGF_SECTION_ANIMATION_CONTROL,
							GGF_SECTION_ANIMATION_STACK,
							GGF_SECTION_ANIMATION_LAYER,
							GGF_SECTION_ANIMATION_CURVE,
							GGF_SECTION_ANIMATION_NODE,
							GGF_SECTION_ANIMATION_ATTACHMENT,
							GGF_SECTION_ANIMATION_BLENDING,
							GGF_SECTION_ANIMATION_EVENT,

							GGF_SECTION_LOGIC,
							GGF_SECTION_LOGIC_CONTROLLER,
							GGF_SECTION_LOGIC_ANIMATION,
							GGF_SECTION_LOGIC_STATE,
							GGF_SECTION_LOGIC_OVERLAY,
							GGF_SECTION_LOGIC_TRANSITION,
							GGF_SECTION_LOGIC_COMMAND,
							GGF_SECTION_LOGIC_PATH,
							GGF_SECTION_LOGIC_NAVIGATION,
							GGF_SECTION_LOGIC_NAVIGATION_NAVMESH,
							GGF_SECTION_LOGIC_NAVIGATION_BSP,

							GGF_SECTION_MATERIAL,
							GGF_SECTION_TEXTURE,

							GGF_SECTION_SHADER,

							GGF_SECTION_SOUND,
							GGF_SECTION_SOUND_ANIMATION_LISTENER,
							GGF_SECTION_SOUND_AMBIENT,

							GGF_SECTION_PHYSICS,
							GGF_SECTION_PHYSICS_ENGINE,
							GGF_SECTION_PHYSICS_ENTITY,
							GGF_SECTION_PHYSICS_CONSTRAINT,
							GGF_SECTION_PHYSICS_AFFECTOR,

							GGF_SECTION_MEDIA,		
							GGF_SECTION_DATA,
							GGF_SECTION_FONT,
							GGF_SECTION_MAX
							};
	
	//---------	

	class GGFPROPERTY
	{
	public:
		GGFPROPERTY() {};

		XDWORD		PropertyType;

		XDWORD		ownertype;
		//XSTRING		owner;
		XDWORD		ownertag;		

		XDWORD		referencetype;
		//XSTRING		reference;
		XDWORD		referencetag;
	};


	//--------

	class GGFSECTIONBLOCK 
	{
	public: 

		GGFSECTIONBLOCK() :		
						sectiontype(GGF_SECTION_NONE),
						majorversion	(0),
						minorversion	(0),
						index					(0),
						header				(0),
						size					(0){		}

		~GGFSECTIONBLOCK()
		{
					if (size)
					{
							delete(header);
							header=NULL;
							size=0;
					}

					Blocks.DeleteContents();
					Blocks.DeleteAll();
		}

		XBYTE*													Set					(XQWORD size);
		XQWORD													GetSize			();
		XQWORD													GetHeaderSize();
		void														SetType			(GGF_SECTION_TYPE s);
		
		XBYTE*													Get					();					
		bool														MergeBlocks	(XBYTE** result, XQWORD* size);

		GGFVECTOR<GGFSECTIONBLOCK*>			Blocks;

	protected:

		XBYTE*													header;
		XQWORD													index;
		XQWORD													size;
		
		GGF_SECTION_TYPE								sectiontype;		
		XDWORD													majorversion;
		XDWORD													minorversion;		
	};	

	//--------

	class GGFSECTIONBUILDER
	{
	public: 

		GGFSECTIONBUILDER() :			sectiontype		(GGF_SECTION_NONE),
															majorversion	(0),
															minorversion	(0),
															block_size		(0),
															block_data		(NULL) {		}

		virtual									 ~GGFSECTIONBUILDER() {}

		virtual GGFSECTIONBLOCK*	CreateBlock	(GRPFILE3DGGF*, void* v=NULL)=0;		
		virtual bool							ReadBlock		(GRPFILE3DGGF*,	XBYTE* buffer, XQWORD* position, void * parent=NULL)=0;

	protected:

		XBYTE*										block_data;
		XQWORD										block_size;

		GGF_SECTION_TYPE					sectiontype;		
		XDWORD										majorversion;
		XDWORD										minorversion;		
	};	
	
	//-------	commented sections are on prevision but unimplemented
	
	class GGFROOT_BUILDER																: public GGFSECTIONBUILDER	{		public: 		GGFSECTIONBLOCK* CreateBlock	(GRPFILE3DGGF*, void* v=NULL); 			bool ReadBlock(GRPFILE3DGGF*,XBYTE* buffer, XQWORD* position, void* parent=NULL); };
		class GGFHEADER_BUILDER														:	public GGFSECTIONBUILDER	{		public: 		GGFSECTIONBLOCK* CreateBlock	(GRPFILE3DGGF*, void* v=NULL); 			bool ReadBlock(GRPFILE3DGGF*,XBYTE* buffer, XQWORD* position, void* parent=NULL); };
		class GGFFONT_BUILDER															:	public GGFSECTIONBUILDER	{		public: 		GGFSECTIONBLOCK* CreateBlock	(GRPFILE3DGGF*, void* v=NULL); 			bool ReadBlock(GRPFILE3DGGF*,XBYTE* buffer, XQWORD* position, void* parent=NULL); };
	//class GGFVERSION_BUILDER													:	public GGFSECTIONBUILDER	{		public: 		GGFSECTIONBLOCK* CreateBlock	(GRPFILE3DGGF*, void* v=NULL); 			bool ReadBlock(GRPFILE3DGGF*,XBYTE* buffer, XQWORD* position, void* parent=NULL); };		
		class GGFPROPERTY_BUILDER													: public GGFSECTIONBUILDER	{		public: 		GGFSECTIONBLOCK* CreateBlock	(GRPFILE3DGGF*, void* v=NULL); 			bool ReadBlock(GRPFILE3DGGF*,XBYTE* buffer, XQWORD* position, void* parent=NULL); };				

		class GGFDICTIONARY_BUILDER												: public GGFSECTIONBUILDER	{		public: 		GGFSECTIONBLOCK* CreateBlock	(GRPFILE3DGGF*, void* v=NULL); 			bool ReadBlock(GRPFILE3DGGF*,XBYTE* buffer, XQWORD* position, void* parent=NULL); };				
		class GGFMEDIAFILE_BUILDER												: public GGFSECTIONBUILDER	{		public: 		GGFSECTIONBLOCK* CreateBlock	(GRPFILE3DGGF*, void* v=NULL); 			bool ReadBlock(GRPFILE3DGGF*,XBYTE* buffer, XQWORD* position, void* parent=NULL); };					
	
		class GGFSCENE_BUILDER														: public GGFSECTIONBUILDER	{		public: 		GGFSECTIONBLOCK* CreateBlock	(GRPFILE3DGGF*, void* v=NULL); 			bool ReadBlock(GRPFILE3DGGF*,XBYTE* buffer, XQWORD* position, void* parent=NULL); };
			class GGFOBJECT_BUILDER													: public GGFSECTIONBUILDER	{		public: 		GGFSECTIONBLOCK* CreateBlock	(GRPFILE3DGGF*, void* v=NULL); 			bool ReadBlock(GRPFILE3DGGF*,XBYTE* buffer, XQWORD* position, void* parent=NULL); };
				class GGFELEMENT_BUILDER											: public GGFSECTIONBUILDER	{		public: 		GGFSECTIONBLOCK* CreateBlock	(GRPFILE3DGGF*, void* v=NULL); 			bool ReadBlock(GRPFILE3DGGF*,XBYTE* buffer, XQWORD* position, void* parent=NULL); };
					class GGFMESH_BUILDER												: public GGFSECTIONBUILDER	{		public: 		GGFSECTIONBLOCK* CreateBlock	(GRPFILE3DGGF*, void* v=NULL); 			bool ReadBlock(GRPFILE3DGGF*,XBYTE* buffer, XQWORD* position, void* parent=NULL); };
						class GGFMESH_LAYER_BUILDER								: public GGFSECTIONBUILDER	{		public: 		GGFSECTIONBLOCK* CreateBlock	(GRPFILE3DGGF*, void* v=NULL); 			bool ReadBlock(GRPFILE3DGGF*,XBYTE* buffer, XQWORD* position, void* parent=NULL); XBYTE layertype; GRPMESH* mesh;};										
				class GGFLOCATOR_BUILDER											: public GGFSECTIONBUILDER	{		public: 		GGFSECTIONBLOCK* CreateBlock	(GRPFILE3DGGF*, void* v=NULL); 			bool ReadBlock(GRPFILE3DGGF*,XBYTE* buffer, XQWORD* position, void* parent=NULL); };
				class GGFCAMERA_BUILDER												: public GGFSECTIONBUILDER	{		public: 		GGFSECTIONBLOCK* CreateBlock	(GRPFILE3DGGF*, void* v=NULL); 			bool ReadBlock(GRPFILE3DGGF*,XBYTE* buffer, XQWORD* position, void* parent=NULL); };
				class GGFLIGHT_BUILDER												: public GGFSECTIONBUILDER	{		public: 		GGFSECTIONBLOCK* CreateBlock	(GRPFILE3DGGF*, void* v=NULL); 			bool ReadBlock(GRPFILE3DGGF*,XBYTE* buffer, XQWORD* position, void* parent=NULL); };									

					class GGFDEFORMER_BUILDER										: public GGFSECTIONBUILDER	{		public: 		GGFSECTIONBLOCK* CreateBlock	(GRPFILE3DGGF*, void* v=NULL); 			bool ReadBlock(GRPFILE3DGGF*,XBYTE* buffer, XQWORD* position, void* parent=NULL); };
						class GGFSUBDEFORMER_BUILDER							: public GGFSECTIONBUILDER	{		public: 		GGFSECTIONBLOCK* CreateBlock	(GRPFILE3DGGF*, void* v=NULL); 			bool ReadBlock(GRPFILE3DGGF*,XBYTE* buffer, XQWORD* position, void* parent=NULL); };
						class GGFSKIN_BUILDER											: public GGFSECTIONBUILDER	{		public: 		GGFSECTIONBLOCK* CreateBlock	(GRPFILE3DGGF*, void* v=NULL); 			bool ReadBlock(GRPFILE3DGGF*,XBYTE* buffer, XQWORD* position, void* parent=NULL); };
							class GGFSKINCLUSTER_BUILDER						: public GGFSECTIONBUILDER	{		public: 		GGFSECTIONBLOCK* CreateBlock	(GRPFILE3DGGF*, void* v=NULL); 			bool ReadBlock(GRPFILE3DGGF*,XBYTE* buffer, XQWORD* position, void* parent=NULL); };
							class GGFSKINLIMB_BUILDER								: public GGFSECTIONBUILDER	{		public: 		GGFSECTIONBLOCK* CreateBlock	(GRPFILE3DGGF*, void* v=NULL); 			bool ReadBlock(GRPFILE3DGGF*,XBYTE* buffer, XQWORD* position, void* parent=NULL); };
							class GGFSKINPOSE_BUILDER								: public GGFSECTIONBUILDER	{		public: 		GGFSECTIONBLOCK* CreateBlock	(GRPFILE3DGGF*, void* v=NULL); 			bool ReadBlock(GRPFILE3DGGF*,XBYTE* buffer, XQWORD* position, void* parent=NULL); };							
					
					class GGFANIMATION_BUILDER									: public GGFSECTIONBUILDER	{		public: 		GGFSECTIONBLOCK* CreateBlock	(GRPFILE3DGGF*, void* v=NULL); 			bool ReadBlock(GRPFILE3DGGF*,XBYTE* buffer, XQWORD* position, void* parent=NULL); };
						class GGFANIMATION_STACK_BUILDER					: public GGFSECTIONBUILDER	{		public: 		GGFSECTIONBLOCK* CreateBlock	(GRPFILE3DGGF*, void* v=NULL); 			bool ReadBlock(GRPFILE3DGGF*,XBYTE* buffer, XQWORD* position, void* parent=NULL); };
							class GGFANIMATION_LAYER_BUILDER				: public GGFSECTIONBUILDER	{		public: 		GGFSECTIONBLOCK* CreateBlock	(GRPFILE3DGGF*, void* v=NULL); 			bool ReadBlock(GRPFILE3DGGF*,XBYTE* buffer, XQWORD* position, void* parent=NULL); };									
									class GGFANIMATION_EVENT_BUILDER		: public GGFSECTIONBUILDER	{		public: 		GGFSECTIONBLOCK* CreateBlock	(GRPFILE3DGGF*, void* v=NULL); 			bool ReadBlock(GRPFILE3DGGF*,XBYTE* buffer, XQWORD* position, void* parent=NULL); };
									class GGFANIMATION_NODE_BUILDER			: public GGFSECTIONBUILDER	{		public: 		GGFSECTIONBLOCK* CreateBlock	(GRPFILE3DGGF*, void* v=NULL); 			bool ReadBlock(GRPFILE3DGGF*,XBYTE* buffer, XQWORD* position, void* parent=NULL); };
									class GGFANIMATION_CURVE_BUILDER		: public GGFSECTIONBUILDER	{		public: 		GGFSECTIONBLOCK* CreateBlock	(GRPFILE3DGGF*, void* v=NULL); 			bool ReadBlock(GRPFILE3DGGF*,XBYTE* buffer, XQWORD* position, void* parent=NULL); };
//						class ANIMATION_BLENDER_BUILDER					: public GGFSECTIONBUILDER	{		public: 		GGFSECTIONBLOCK* CreateBlock	(GRPFILE3DGGF*, void* v=NULL); 			bool ReadBlock(GRPFILE3DGGF*,XBYTE* buffer, XQWORD* position); };

					class GGFLOGIC_BUILDER											: public GGFSECTIONBUILDER	{		public: 		GGFSECTIONBLOCK* CreateBlock	(GRPFILE3DGGF*, void* v=NULL); 			bool ReadBlock(GRPFILE3DGGF*,XBYTE* buffer, XQWORD* position, void* parent=NULL); };
						class GGFLOGIC_CONTROLLER_BUILDER					: public GGFSECTIONBUILDER	{		public: 		GGFSECTIONBLOCK* CreateBlock	(GRPFILE3DGGF*, void* v=NULL); 			bool ReadBlock(GRPFILE3DGGF*,XBYTE* buffer, XQWORD* position, void* parent=NULL); };
							class GGFLOGIC_STATE_BUILDER						: public GGFSECTIONBUILDER	{		public: 		GGFSECTIONBLOCK* CreateBlock	(GRPFILE3DGGF*, void* v=NULL); 			bool ReadBlock(GRPFILE3DGGF*,XBYTE* buffer, XQWORD* position, void* parent=NULL); };
							class GGFLOGIC_ANIMATION_BUILDER				: public GGFSECTIONBUILDER	{		public: 		GGFSECTIONBLOCK* CreateBlock	(GRPFILE3DGGF*, void* v=NULL); 			bool ReadBlock(GRPFILE3DGGF*,XBYTE* buffer, XQWORD* position, void* parent=NULL); };
							class GGFLOGIC_OVERLAY_BUILDER					: public GGFSECTIONBUILDER	{		public: 		GGFSECTIONBLOCK* CreateBlock	(GRPFILE3DGGF*, void* v=NULL); 			bool ReadBlock(GRPFILE3DGGF*,XBYTE* buffer, XQWORD* position, void* parent=NULL); };							
						
						/*
						class LOGIC_PATH_BUILDER								  : public GGFSECTIONBUILDER	{		public: 		GGFSECTIONBLOCK* CreateBlock	(GRPFILE3DGGF*, void* v=NULL); 			bool ReadBlock(GRPFILE3DGGF*,XBYTE* buffer, XQWORD* position, void* parent=NULL); };
						class LOGIC_NAVIGATION_BUILDER					  : public GGFSECTIONBUILDER	{		public: 		GGFSECTIONBLOCK* CreateBlock	(GRPFILE3DGGF*, void* v=NULL); 			bool ReadBlock(GRPFILE3DGGF*,XBYTE* buffer, XQWORD* position, void* parent=NULL); };
							class LOGIC_NAVMESH_BUILDER						  : public GGFSECTIONBUILDER	{		public: 		GGFSECTIONBLOCK* CreateBlock	(GRPFILE3DGGF*, void* v=NULL); 			bool ReadBlock(GRPFILE3DGGF*,XBYTE* buffer, XQWORD* position, void* parent=NULL); };
							class LOGIC_BSP_BUILDER								  : public GGFSECTIONBUILDER	{		public: 		GGFSECTIONBLOCK* CreateBlock	(GRPFILE3DGGF*, void* v=NULL); 			bool ReadBlock(GRPFILE3DGGF*,XBYTE* buffer, XQWORD* position, void* parent=NULL); };
						*/
						
			class GGFMATERIAL_BUILDER											: public GGFSECTIONBUILDER	{		public: 		GGFSECTIONBLOCK* CreateBlock	(GRPFILE3DGGF*, void* v=NULL); 			bool ReadBlock(GRPFILE3DGGF*,XBYTE* buffer, XQWORD* position, void* parent=NULL); };			
			class GGFTEXTURE_BUILDER											: public GGFSECTIONBUILDER	{		public: 		GGFSECTIONBLOCK* CreateBlock	(GRPFILE3DGGF*, void* v=NULL); 			bool ReadBlock(GRPFILE3DGGF*,XBYTE* buffer, XQWORD* position, void* parent=NULL); };						
			class GGFSHADER_BUILDER												: public GGFSECTIONBUILDER	{		public: 		GGFSECTIONBLOCK* CreateBlock	(GRPFILE3DGGF*, void* v=NULL); 			bool ReadBlock(GRPFILE3DGGF*,XBYTE* buffer, XQWORD* position, void* parent=NULL); };						

			/*
			class PHYSIC_BUILDER													: public GGFSECTIONBUILDER	{		public: 		GGFSECTIONBLOCK* CreateBlock	(GRPFILE3DGGF*, void* v=NULL); 			bool ReadBlock(GRPFILE3DGGF*,XBYTE* buffer, XQWORD* position); };
				class PHYENGINE_BUILDER											: public GGFSECTIONBUILDER	{		public: 		GGFSECTIONBLOCK* CreateBlock	(GRPFILE3DGGF*, void* v=NULL); 			bool ReadBlock(GRPFILE3DGGF*,XBYTE* buffer, XQWORD* position); };
				class PHYENTITY_BUILDER											: public GGFSECTIONBUILDER	{		public: 		GGFSECTIONBLOCK* CreateBlock	(GRPFILE3DGGF*, void* v=NULL); 			bool ReadBlock(GRPFILE3DGGF*,XBYTE* buffer, XQWORD* position); };
				class PHYCONSTRAINT_BUILDER									: public GGFSECTIONBUILDER	{		public: 		GGFSECTIONBLOCK* CreateBlock	(GRPFILE3DGGF*, void* v=NULL); 			bool ReadBlock(GRPFILE3DGGF*,XBYTE* buffer, XQWORD* position); };
				class PHYAFFECTOR_BUILDER										: public GGFSECTIONBUILDER	{		public: 		GGFSECTIONBLOCK* CreateBlock	(GRPFILE3DGGF*, void* v=NULL); 			bool ReadBlock(GRPFILE3DGGF*,XBYTE* buffer, XQWORD* position); };
			*/

			class GGFSOUNDS_BUILDER												: public GGFSECTIONBUILDER	{		public: 		GGFSECTIONBLOCK* CreateBlock	(GRPFILE3DGGF*, void* v=NULL); 			bool ReadBlock(GRPFILE3DGGF*,XBYTE* buffer, XQWORD* position, void* parent=NULL); };
				class GGFAMBIENT_BUILDER										: public GGFSECTIONBUILDER	{		public: 		GGFSECTIONBLOCK* CreateBlock	(GRPFILE3DGGF*, void* v=NULL); 			bool ReadBlock(GRPFILE3DGGF*,XBYTE* buffer, XQWORD* position, void* parent=NULL); };
				class GGFSOUNDS_ANIMATION_LISTENER_BUILDER	: public GGFSECTIONBUILDER	{		public: 		GGFSECTIONBLOCK* CreateBlock	(GRPFILE3DGGF*, void* v=NULL); 			bool ReadBlock(GRPFILE3DGGF*,XBYTE* buffer, XQWORD* position, void* parent=NULL); };
			
			class GGFDATA_BUILDER													: public GGFSECTIONBUILDER	{		public: 		GGFSECTIONBLOCK* CreateBlock	(GRPFILE3DGGF*, void* v=NULL); 			bool ReadBlock(GRPFILE3DGGF*,XBYTE* buffer, XQWORD* position,void* parent=NULL);	};
		

/*---- CLASS -----------------------------------------------------------------------------*/
class GRPFILE3DGGF : public GRPFILE3D
{
public:

	friend class GGFSECTIONBUILDER;
	friend class GGFROOT_BUILDER;
	friend class GGFFONT_BUILDER;
	friend class GGFMESH_BUILDER;
	friend class GGFELEMENT_BUILDER;
	friend class GGFDEFORMER_BUILDER;
	friend class GGFSUBDEFORMER_BUILDER;
	friend class GGFSKIN_BUILDER;
	friend class GGFSKINPOSE_BUILDER;
	friend class GGFSKINCLUSTER_BUILDER;
	friend class GGFLOCATOR_BUILDER;
	friend class GGFCAMERA_BUILDER;
	friend class GGFLIGHT_BUILDER;
	friend class GGFHEADER_BUILDER;
	friend class GGFMATERIAL_BUILDER;
	friend class GGFTEXTURE_BUILDER;	
	friend class GGFANIMATION_BUILDER;
	friend class GGFANIMATION_STACK_BUILDER;
	friend class GGFANIMATION_LAYER_BUILDER;
	friend class GGFANIMATION_NODE_BUILDER;
	friend class GGFANIMATION_CURVE_BUILDER;
	friend class GGFANIMATION_EVENT_BUILDER;
	friend class GGFLOGIC_ANIMATION_BUILDER;
	friend class GGFDATA_BUILDER;
	friend class GGFSHADER_BUILDER;

	enum 
	{
		MAYOR_VERSION = 0,
		MINOR_VERSION = 1		
	};

																													GRPFILE3DGGF						();
	virtual																									~GRPFILE3DGGF						();

	virtual GRPOBJECT*																			Load										(XCHAR*					file);
	virtual bool																						Save										(XCHAR*					file);
					void																						SetScene								(GRPSCENE*			scene);
					GRPSCENE*																				GetScene								();					

					void																						Init										();
					bool																						Setup										();

					void																						Output									(XDWORD				priority, XCHAR* mask,	...);

					GGFMAP	<GGF_SECTION_TYPE,GGFSECTIONBUILDER*>*	GetBuilders							() { return &Builders; }				
					GGFVECTOR	<GGFPROPERTY*>*												GetProperties						() { return &Properties;} 					
					GGFMAP	<XQWORDSIG,	GGF_SECTION_TYPE>*					GetDictionary						() { return &Dictionary;}
					

					XQWORD																					GetSize									() { return size;			 }

					bool																						WriteString							(XSTRING* string,GGFSECTIONBLOCK* block,XQWORD* pos);
					bool																						ReadString							(XSTRING* string,XBYTE* buffer, XQWORD* position, XQWORD* offset);

protected:

					XDWORD																					majorversion,	minorversion;

						//--------- header data

					XMAP<GGF_SECTION_TYPE,XDWORD>									  contains;
					
					XDWORD																					nfonts;
					XDWORD																					nobjects;
					XDWORD																					nmaterials;
					XDWORD																					nshaders;
					XDWORD																					ntextures;
					XDWORD																					nanimations;
					
					



	//------------------------------ builders

					bool																						InitSectionBuilders				();										
					
					
					GGFSECTIONBUILDER*															CreateSectionBuilder			(	GGF_SECTION_TYPE,
																																										XDWORD majorversion=MAYOR_VERSION,
																																										XDWORD minorversion=MINOR_VERSION);

					GGFVECTOR<GGF_SECTION_TYPE>											LoadableSections;
					GGFVECTOR<GGF_SECTION_TYPE>											SectionBuilder;

					GGFSECTIONBLOCK*																root;

	//----------------------------- containers	

					XVECTOR<GRPNODE*>																nodes;
					XVECTOR<GRPCAMERA*>															cameras;
					XVECTOR<GRPLIGHT*>															lights;
					XVECTOR<GRPELEMENT*>														elements;					
					XVECTOR<GRPMESH*>																meshes;
					XVECTOR<GRPOBJECT*>															objects;

					XVECTOR<GRPMATERIAL*>														materials;
					XVECTOR<GRPTEXTURE*>														textures;
					XVECTOR<GRPSHADERPROGRAM*>											shaders;

					XVECTOR<GRPANIMATIONCONTROL*>										animationcontrols;
					XVECTOR<GRPANIMATIONSTACK*>											animationstacks;
					XVECTOR<GRPANIMATIONLAYER*>											animationlayers;
					XVECTOR<GRPANIMATIONNODE*>											animationnodes;					
					XVECTOR<GRPANIMATIONCURVE*>											animationcurves;
					XVECTOR<GRPANIMATIONSTACKMILESTONE*>						animationevents;

					XVECTOR<GRPDEFORMER*>														deformers;
					XVECTOR<GRPSUBDEFORMER*>												subdeformers;

					XVECTOR<GRPSKIN*>																skins;
					XVECTOR<GRPSKINLIMB*>														skinslimbs;
					XVECTOR<GRPSKINCLUSTER*>												skincluster;
					XVECTOR<GRPSKINPOSE*>														skinpose;					

					XVECTOR<GRPUSERDATACONTAINER*>									datacontainers;

					XVECTOR<AI::ANIMATION*>													ai_animations;


					GGFMAP<GGF_SECTION_TYPE,	GGFSECTIONBUILDER*>		Builders;

	//------------------------------- maps

					GGFMAP<XQWORDSIG,					XQWORDSIG>						Index;		
					GGFMAP<XQWORDSIG,					GGF_SECTION_TYPE>			Dictionary;										
					GGFVECTOR	<GGFPROPERTY*>												Properties;	

	//------------------------------ status

					bool																						isinitialized;
					XQWORD																					size;


					XTIMER*		timer;					
					XQWORD		texture_timer_total;
					XQWORD		ttfs_timer_total;
					XQWORD		animation_total;

					XWORD*		textholder;
					XDWORD		textholdersize;
private:

		void Clear();

		void Clean()
		{
				size=0;
			  nobjects=0;
				majorversion=MAYOR_VERSION;
				minorversion=MINOR_VERSION;
				scene=NULL;
				isinitialized=false;

				nobjects		=0;
				nmaterials	=0;
				nshaders		=0;
				ntextures		=0;
				nanimations	=0;
				nfonts			=0;
				root				=NULL;
				animationcurves.SetIsMulti(true);
				animationnodes.SetIsMulti(true);
				animationstacks.SetIsMulti(true);
				animationlayers.SetIsMulti(true);
				animationevents.SetIsMulti(true);

				timer=NULL;
				texture_timer_total	=0;
				ttfs_timer_total=0;
				animation_total=0;
				textholder=NULL;
				textholdersize=0;
		}

};
	

/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif



