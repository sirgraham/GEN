/*------------------------------------------------------------------------------------------
//	GRPOBJECTECTBUILDER.H
*/	
/**	
// \class 
//   
//  Creates objects
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 27/05/2014 13:32:41
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPOBJECTECTBUILDER_H_
#define _GRPOBJECTECTBUILDER_H_	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

//#include "GRPSprite.h"
#include "GRPVector.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
#define GRPOBJECT_DEFAULTNSEGMENTS  10

#define GetIsConvex(a,b,c) (EPSILON > (((a->x-c->x)*(b->y-c->y)) - ((a->y-c->y)*(b->x-c->x))))
#define IsPointOnLine(testedpoint,linePointA,linePointB) (fabs(testedpoint->y - (((linePointB->y - linePointA->y) / (linePointB->x - linePointB->x))* testedpoint->x+(linePointA->y - ((linePointB->y - linePointA->y) / (linePointB->x - linePointB->x)) * linePointA->x))) < EPSILON)

/*---- CLASS -----------------------------------------------------------------------------*/


class GRPOBJECTBUILDERALGORITHM;
class GRPFONTMANAGER;
class GRPELEMENT;
class GRPMATERIAL;
class GRPVIEWPORT;
class GRPSPLINE;
class GRPSHAPE;
class GRPANCHORPOINT;
class GRPOBJECT;
class GRPSHADERPROGRAM;
class GRPFACE;
class XTIMER;
class XFACTORY;
class XPATHS;
class GRPMESH;

enum GRPBOXSIDEFLAG
{
		GRPBOXSIDEFLAG_TOP    = 1 << 0,
		GRPBOXSIDEFLAG_LEFT   = 1 << 1,
		GRPBOXSIDEFLAG_RIGHT  = 1 << 2,
		GRPBOXSIDEFLAG_BOTTOM = 1 << 3,
		GRPBOXSIDEFLAG_ALL		= GRPBOXSIDEFLAG_TOP | GRPBOXSIDEFLAG_LEFT | GRPBOXSIDEFLAG_RIGHT | GRPBOXSIDEFLAG_BOTTOM
};


class GRPOBJECTBUILDER
{
	public:

		enum AXIS
		{
			AXIS_NONE=0,
			AXIS_X,
			AXIS_Y,
			AXIS_Z,
			AXIS_NEGX=-AXIS_X,
			AXIS_NEGY=-AXIS_Y,
			AXIS_NEGZ=-AXIS_Z,
			AXIS_SCREEN=AXIS_X
		};

		static  GRPVECTOR 				LeftVector;
		static  GRPVECTOR 				RightVector;
		static  GRPVECTOR 				UpVector;
		static  GRPVECTOR 				DownVector;
		static  GRPVECTOR 				NearVector;
		static  GRPVECTOR 				FarVector;
		static  GRPVECTOR 				OriginVector;
		virtual									 ~GRPOBJECTBUILDER											();

		//------------------ TOOLS

		GRPELEMENT*								CreateElement													(	XVECTOR<GRPFACE*>						faces);
		GRPELEMENT*								Create																( GRPSPLINE*									spline);

		GRPELEMENT*								CreateLineElement											(	GRPPOINT										o,					
																																			GRPPOINT										f);

		GRPELEMENT*								CreateArcElement											(	GRPPOINT										o,					
																																			GRPPOINT										f,			
																																			GRPPOINT										c, 
																																			XDWORD											nsegments									= GRPOBJECT_DEFAULTNSEGMENTS);					

		GRPELEMENT*								CreateBackground											();
			
		GRPELEMENT*								CreateCircle													(	GRPPOINT										o,					
																																			GLFLOAT											radius,																																				
																																			XDWORD											nsegments									= GRPOBJECT_DEFAULTNSEGMENTS);

		GRPELEMENT*								CreatePoint														();
//		GRPSPRITE*								CreateMasterSprite										( GRPSPRITE::TYPE type=GRPSPRITE::POINT);

		GRPELEMENT*								CreateEllipse													(	GRPPOINT										o,					
																																			GLFLOAT											radius, 
																																			float												relation, 																																			
																																			XDWORD											nsegments									= 2);				

		GRPMESH*									CreateCube														();								
		GRPMESH*									CreateGrid														();								

		GRPELEMENT*								CreateFromBuffer											(	int													nvertex,				
																																			GLFLOAT*										vertexpositions, 
																																			GLFLOAT*										vertexnormals, 
																																			GLFLOAT*										vertextexturecoords);

		GRPELEMENT*								CreatePlane														(	GRPOBJECTBUILDER::AXIS axis = GRPOBJECTBUILDER::AXIS_Z, bool doublesided=false);
		
		GRPSPLINE*								CreateLine														(	GRPPOINT										o,									
																																			GRPPOINT										f);

		GRPSPLINE*								CreateArc															(	GRPPOINT										o,									
																																			GRPPOINT										f,					
																																			GRPPOINT										c, 
																																			XDWORD											nsegments									=	3);					

		GRPELEMENT*								CreateBox															( GLFLOAT  w, GLFLOAT h, GLFLOAT texturefactorx=1.0,GLFLOAT texturefactory= 1.0, GLFLOAT textureoffsetx=0.0f, GLFLOAT textureoffsety = 0.0f);
		GRPELEMENT*								CreateBoxRounded											( GLFLOAT  w, GLFLOAT h, GLFLOAT cornerradius=0.0f, GLFLOAT texturefactorx = 1.0, GLFLOAT texturefactory = 1.0, GLFLOAT textureoffsetx = 0.0f, GLFLOAT textureoffsety = 0.0f);
		GRPELEMENT*								CreateBorder													( GLFLOAT  w, GLFLOAT h, GLFLOAT cornerradius=0.0f, GLFLOAT borderwidth=0.02f, XDWORD flags = GRPBOXSIDEFLAG_ALL, GLFLOAT texturefactorx = 1.0f, GLFLOAT texturefactory = 1.0f, GLFLOAT textureoffsetx = 0.0f, GLFLOAT textureoffsety=0.0f);
		GRPELEMENT*								CreateShadowBorder										( GLFLOAT  w, GLFLOAT h, GLFLOAT cornerradius = 0.0f, GLFLOAT borderwidth = 0.02f, XDWORD flags = GRPBOXSIDEFLAG_ALL);


		GRPSPLINE*								CreateBoxSpline												(	GRPPOINT										o,									
																																			GRPPOINT										f,					
																																			float												corner										=	0.0f,
																																			XDWORD flags= GRPBOXSIDEFLAG_ALL);

		GRPSPLINE*								CreateWedgeSpline											(	GRPPOINT										o,									
																																			GLFLOAT											radius,			
																																			GLFLOAT											startingAngle,
																																			GLFLOAT											endingAngle,	
																																			XDWORD											nsegments									=	3);		
		
		GRPSPLINE*								CreateEllipseSpline										(	GRPPOINT										o,									
																																			GLFLOAT											radius,			
																																			float												relation, 
																																			XDWORD											nsegments									= 2);	

		GRPSPLINE*								CreateCircleSpline										(	GRPPOINT										o,									
																																			GLFLOAT											radius,			
																																			XDWORD											nsegments									= 36);		



		//------------------ SPLINE/SHAPE functions
		
		bool											CheckPointInsideContour								(	GRPPOINT										point,							
																																			GRPSPLINE*									processedspline);

		bool											FastCheckPointInsideContour						(	GRPPOINT										point,							
																																			GRPSPLINE*									processedspline);
		bool											CheckPointInsideContour								(	GRPPOINT										testedpoint,				
																																			XVECTOR<GRPPOINT*>*					contour);


		int												IsInnerContour												(	int													index,										
																																			GRPSHAPE*										shape);	

		GRPANCHORPOINT*						FindNearerAnchor											(	GRPSPLINE*									spline,						
																																			GRPSPLINE*									reference,						
																																			GRPANCHORPOINT**						nearer);

		void											FindNearerAnchor											(	GRPSPLINE*									spline,						
																																			GRPSPLINE*									reference,						
																																			int*												insiderIndex,				
																																			int*												contourIndex);

		GRPANCHORPOINT*						FindNearAnchor												(	GRPSPLINE*									spline,						
																																			GRPANCHORPOINT*							reference,			
																																			float*											distance);

		void											FindNearAnchor												(	GRPSPLINE*									spline,						
																																			GRPANCHORPOINT*							reference,			
																																			float*											f,									
																																			int*												index		);

		void											GetBoundingVertexesFromPointArray			(	XVECTOR<GRPPOINT*>*					vertex,	
																																			GRPPOINT**									_top,								
																																			GRPPOINT**									_left,					
																																			GRPPOINT**									_bottom	, 
																																			GRPPOINT**									_right	);// , GRPPOINT** _near , GRPPOINT** _far);

		void											GetBoundingVertexesFromSpline					(	GRPSPLINE*									vertex,						
																																			GRPANCHORPOINT**						_top		=	NULL,		
																																			GRPANCHORPOINT**						_left		=	NULL, 
																																			GRPANCHORPOINT**						_bottom	= NULL, 
																																			GRPANCHORPOINT**						_right	=	NULL, 
																																			GRPANCHORPOINT**						_near		= NULL, 
																																			GRPANCHORPOINT**						_far		= NULL);
			
		float											SignOfBarycentricCoordenate						(	GRPPOINT										testedpoint,				
																																			GRPPOINT										trianglesidevectorA,		
																																			GRPPOINT										trianglesidevectorB);
																					
		bool											CheckPointInsideTriangle							(	GRPPOINT*										testedpoint,				
																																			GRPPOINT*										trianglepointA,				
																																			GRPPOINT*										trianglepointB, 
																																			GRPPOINT*										trianglepointC);
		
		bool											IsClockWiseTriangle										(	GRPPOINT*										a, 
																																			GRPPOINT*										b,		
																																			GRPPOINT*										c);												

		void											GetAdyacentPoints											(	XVECTOR<GRPPOINT*>*					vertex,	
																																			GRPPOINT*										a,										
																																			GRPPOINT**									b,							
																																			GRPPOINT**									c, 
																																			int*												aindex, 
																																			int*												bindex, 
																																			int*												cindex);
	
		GRPSPLINE*								CreateSpline													(	XVECTOR<GRPPOINT*>*					points);

		GRPSPLINE*								ProcessSplineToLines									(	GRPSPLINE*									splinetoprocess,	
																																			XDWORD											nsegments								=GRPOBJECT_DEFAULTNSEGMENTS);		
		GRPSHAPE*									MergeInnerSplines											(	GRPSHAPE*										shape);		
		GRPSHAPE*									WeldVertexes													(	GRPSHAPE*										shape,							
																																			float												treshold);		
		GRPELEMENT*								WeldVertexes													(	GRPELEMENT*									element,					
																																			float												treshold);
		GRPSHAPE*									ProcessShapeToLines										(	GRPSHAPE*										shapetoprocess,		
																																			XDWORD											nsegments								=GRPOBJECT_DEFAULTNSEGMENTS);

		bool											AddShapeAsElement											(	GRPOBJECT*									object,																																																
																																			GRPSHAPE*										shape);				

		GRPELEMENT*								ConvertShapeToElement									(GRPSHAPE*										shape);

		//---------------------- Bezier Curve operations		
		GRPSPLINE*								ResolveCubicBezier										(	GRPSPLINE*									inputSp,					
																																			int													nSegments=25);

		//--------------------- Triangulation

		GRPELEMENT*								TriangulizeShape											(	GRPSHAPE*										shape,							
																																			GRPOBJECTBUILDERALGORITHM*	algorithm);		
		GRPELEMENT*								TriangulizeObject											(	GRPSHAPE*										shape,							
																																			GLFLOAT											extrude);

		//--------------------- Sprites
	
		//GRPSPRITE*								GetMasterSprite												();

		//--------------------- setup
		void											Ini																		();
		void											End																		();

		//--------------------- fonts
		GRPFONTMANAGER*						CreateFontManager											();
		GRPFONTMANAGER*						GetFontManager												();

		XVECTOR<GRPELEMENT*>*     GetReferences													();

	static	GRPOBJECTBUILDER*		Get																		();
	static	void								Destroy																();
	
	
	GRPVECTOR v0;
	GRPVECTOR v1;
	GRPVECTOR v2;

 protected:	 
											 				GRPOBJECTBUILDER											();
	static	GRPOBJECTBUILDER*		Instance;

	private:

	//	XTIMER*										timer;

		float											FastGetDistanceBetweenPoints					(GRPPOINT* a, GRPPOINT* b);
	
		void											Clean																	()
															{
																fontmanager		 = NULL;
																//timer					= NULL;
																xmutex				= NULL;
															}

		GRPFONTMANAGER*						fontmanager;		
		XVECTOR<GRPELEMENT*>      references;
		XMUTEX*										xmutex;
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

