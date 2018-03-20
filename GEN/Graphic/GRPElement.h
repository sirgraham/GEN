/*------------------------------------------------------------------------------------------
//	GRPELEMENT.H
*/	
/**	
// \class 
//   
//  Object ElementData
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 27/05/2014 10:53:18
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPELEMENT_H_
#define _GRPELEMENT_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XVector.h"
#include "XOrderedList.h"

#include "GRPDrawable.h"
#include "GRPVertex.h"
#include "GRPFace.h"
#include "GRPMesh.h"

#include "GRPNode.h"

#include "GRPMatrix.h"
#include "GRPSubject.h"

#include "XMemory.h"
	
#include "GRPMaterial.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
enum GRPRENDERMODE
{
	GRPRENDERMODE_UNKNOWN						=	-1								,
	GRPRENDERMODE_POINTS						=	GL_POINTS					,
	GRPRENDERMODE_LINES							=	GL_LINES					,
	GRPRENDERMODE_LINE_STRIP				=	GL_LINE_STRIP			,
	GRPRENDERMODE_LINE_LOOP					= GL_LINE_LOOP			,
	GRPRENDERMODE_TRIANGLES					=	GL_TRIANGLES			,
	GRPRENDERMODE_TRIANGLE_STRIP		=	GL_TRIANGLE_STRIP	,
	GRPRENDERMODE_TRIANGLE_FAN			=	GL_TRIANGLE_FAN   ,
};

enum GRPRENDERPRIORITY
{
	GRPRENDERPRIORITY_HIGHEST			=0,
	GRPRENDERPRIORITY_HIGH				=50,
	GRPRENDERPRIORITY_NORMAL			=100,
	GRPRENDERPRIORITY_LOW					=150,
	GRPRENDERPRIORITY_LOWEST			=200,
	GRPRENDERPRIORITY_BACKGROUND	=400,
	GRPRENDERPRIORITY_OVERLAY			=500,
};
	
/*---- CLASS -----------------------------------------------------------------------------*/

class GRPSHADERPROGRAM;
class GRPMATERIAL;
class GRPPOINT;
class GRPDEFORMER;
class GRPVISUALDEBUGGER;
class GRPSKIN;




class GRPELEMENT :  public GRPATTACHEABLE, public GRPDRAWABLE
{
	friend class GRPVISUALDEBUGGER;
  friend class GRPNODE;	

	public:		

		GRPBB														boundingbox;	
		GRPMESH*												mesh;

		class COMPARATOR : public XCOMPARATOR<GRPELEMENT> 	{ public:		int Compare(GRPELEMENT* test1,GRPELEMENT* test2); 	};


																		GRPELEMENT								(	GLUINT							tag=-1);
		virtual												 ~GRPELEMENT								();
		
		bool														SetMesh										(GRPMESH* mesh);
		GRPMESH*												GetMesh										(int index_lod=0);

		
		virtual GRPSHADERPROGRAM*				ChooseShader							();

		virtual bool										Draw											();
		virtual bool										Draw											( GRPPASS* pass);
		virtual bool										Draw											(	const GRPMATRIX*				view,		
																																const GRPMATRIX*				viewprojection,	
																																const GRPMATRIX*				projection
																															);

		virtual void										LoadMatricesToGPU					(	const GRPMATRIX* 				view,		
																																const GRPMATRIX* 				viewprojection,	
																																const GRPMATRIX* 				projection,
																																const GRPSHADERPROGRAM* link
																															);

		virtual void										ComputeMatrices						(	const GRPMATRIX* 				viewmatrix,	
																																const GRPMATRIX* 				viewprojection,	
																																const GRPMATRIX* 				projection
																															);

		void														LoadMaterialToGPU					(GRPSHADERPROGRAM* program=NULL);		
		void														ProcessMorphs							();
		void														DrawVBO										(GRPRENDERMODE			mode);
		void														DrawVBA										(GRPRENDERMODE			mode);
		bool														Reference									(GRPELEMENT* 				ref);	
		bool														Reference									(GRPMESH* 					ref);	
		bool														LoadToGPU									();		

		GRPDEFORMER*										GetDeformer								();
		void														SetDeformer								(GRPDEFORMER*				pdeformer);	
		GRPSKIN*												GetSkin										();	
		void														SetSkin										(GRPSKIN* 					pskin);		

		
		bool														MergeElement							(GRPELEMENT*				mergeable);

		void														UpdateBoundingBox					();
		GRPBB*													GetBoundingBox						()	{ return &this->boundingbox;}

		bool														Collapse									(bool translation_only=false);
		bool														CollapseTranslations			();
		bool														CollapseTransforms				();
		bool														Disembody									();

		void														SetProgram								(GRPSHADERPROGRAM*	programID);
		GRPSHADERPROGRAM*								GetProgram								();

		void														SetVisible								(bool								isvisible);
		bool														IsVisible									();

		void 														SetIsCulled								(bool b);
		bool														IsCulled									();
		
		bool														GetIsReferible						();
		GRPRENDERMODE										GetRenderMode							();
		void														SetRenderMode							(GRPRENDERMODE			rendermode);													

		
		void														SetTextureFrame						(float							frame);
		void														Finish										();		

		GRPNODE*												GetNode										();
		void														SetNode										(GRPNODE*						node);

		void														SetDefaultColor						(GRPCOLOR						color);		
		void														SetRenderFlags						(XDWORD							renderflags);
		XDWORD													GetRenderFlags						();
		XDWORD													GetRenderedFaces					();
		GRPSHADERPROGRAM*								GetShader									();		

		void														SetRenderPriority					(int lowerpriorityfirst);
		int															GetRenderPriority					();

		float														GetVisibility							()																															{ return visibility;					}

		void														SetVisibility							(float	v)																											{ visibility = v;							}

		virtual bool										Notify										(GRPMSGTYPE msg,GRPSUBJECT* notifier);
		virtual	XDWORD									GetSubjectType						();

		void														SetBlending								(bool b);
		void														SetBlendingMode						(GRPBLENDINGMODE mode);
		GRPBLENDINGMODE									GetBlendingMode						() { return blendingmode; }
		bool														IsBlending								();
		
		bool														GetIsLoaded								();

		bool														IsInstance								();
		void														SetIsInstance							(bool s)																												{ this->isinstance=s;						}
			
		bool														GetReceivesShadows				()																															{ return receivesshadows;				}
		void														SetReceivesShadows				(bool rs)																												{ this->receivesshadows=rs;			}
		bool														GetReceivesReflections		()																															{ return receivesreflections;		}
		void														SetReceivesReflections		(bool rs)																												{ this->receivesreflections=rs;	}
		
		bool														GetCastsShadows						()																															{ return castsshadows;					}
		void														SetCastsShadows						(bool sc)																												{ this->castsshadows=sc;				}
		bool														GetCastsReflections				()																															{ return castsreflections;			}
		void														SetCastsReflections				(bool sc)																												{ this->castsreflections=sc;		}
		
		GRPCOLOR*												GetDefaultColor						()																															{ return &this->defaultcolor;		}

		float														textureframe;
		
			//----------------------------- matrix accelerators
		
		GRPMATRIX 											modelmatrix;
		GRPMATRIX 											modelviewmatrix;
		GRPMATRIX 											modelviewprojectionmatrix;

		GRPMATRIX 											texturematrix;

protected:

		//----------------------------- world status
		GRPNODE*												node;

		float														visibility;
		bool														blending;
		GRPBLENDINGMODE									blendingmode;

		bool														castsreflections;
		bool														castsshadows;
		bool														receivesshadows;
		bool														receivesreflections;
		bool														haslightning;

		bool														isinstance;	

				//------------------------------ Referencing
		//GRPELEMENT*											instance;						//Referenced instance
	
		bool														isreferible;

				
		//-------------------------- Deformer
		GRPDEFORMER*										deformer;
		GRPSKIN*												skin;

		XVECTOR<GRPDEFORMER*>						morphs;
							
		//-------------------------- Rendering

		GRPRENDERMODE										rendermode;
		XDWORD													renderflags;	
		

		GRPCOLOR												defaultcolor;

		bool														iswireframe;

		//--------------------------- shader data
		GRPSHADERPROGRAM*								program;
		bool														isCulled;

private:

	
	  void														Clean											()
																		{				
																		blending																	= false;
																		isCulled																	= false;
																		visibility																= 1.0f;
		
																		isinstance																=false;
																		isreferible																=true;
																		textureframe															=0.0f;
																		
																		blendingmode															=GRPBLENDINGMODE_ADDITIVE;

																		rendermode																=GRPRENDERMODE_TRIANGLES;
																		renderflags																=GRPELEMENTRENDERFLAGS_FULL;

																		node																			=NULL;
																		mesh																			=NULL;
																		defaultcolor															=GRPCOLOR::WHITE;	
																		
																		deformer																	=NULL;
																		skin																			=NULL;			
																		iswireframe																=false;
																	
																		program																		=NULL;

																		castsshadows															= true;
																		receivesshadows														= true;
																		castsreflections													= false;
																		receivesreflections												= false;
																		haslightning															= true;
																		renderpriority														=	GRPRENDERPRIORITY_NORMAL;

																		modelmatrix.LoadIdentity									();
																		modelviewmatrix.LoadIdentity							();
																		modelviewprojectionmatrix.LoadIdentity		();
																		texturematrix.LoadIdentity								();
																		}
};
	

/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif







