/*------------------------------------------------------------------------------------------
//	GRPPATH.H
*/	
/**	
// \class 
//   
//  Paths for 3d deformations
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 28/11/2014 10:26:38
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPPATH_H_
#define _GRPPATH_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "GRPSpline.h"	
#include "GRPNode.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
enum GRPPATH_LOOP_TYPE
{
	GRPPATH_LOOP_NONE=0,
	GRPPATH_LOOP_REPEAT,
	GRPPATH_LOOP_OSCILLATE
};
	
/*---- CLASS -----------------------------------------------------------------------------*/
class GRPPATH : public GRPNODE
{
public:
						
											GRPPATH							() ;

			virtual					~GRPPATH						();

			virtual		bool	Update							();

			virtual		void	Set									(float f);

			virtual		bool	OnLinkedTo					(GRPNODE* node);

								void  GetSegment					(float step, GRPVECTOR* position,GRPVECTOR* rotation,GRPVECTOR* scale);

								void	SetPositionSpline		(GRPSPLINE* splinePosition);

								void	SetRotationSpline		(GRPSPLINE* splineRotation,int axis);

								void	SetTravelSpline			(GRPSPLINE* spline);
								void	SetScaleSpline			(GRPSPLINE* spline);

								void	SetLoop							(float st=0.0f,float end=1.0f,GRPPATH_LOOP_TYPE b=GRPPATH_LOOP_REPEAT);
								void	SetStep							(float b)
								{
									this->step=b;
								}
								

protected:

					GRPSPLINE*	splineTravel;
					GRPSPLINE*	splinePosition;

					GRPSPLINE*	splineRotationX;
					GRPSPLINE*	splineRotationY;
					GRPSPLINE*	splineRotationZ;

					GRPSPLINE*	splineScale;

						bool			active;

						int				segment;
						float			step;				
						float			currentstep;
						GRPPATH_LOOP_TYPE			loopType;					
						float     loopStart;
						float     loopEnd;
						bool			loopDirection;

private:

						

						virtual void Clean()
						{
							splineTravel				=NULL;
							splinePosition			=NULL;

							splineRotationX			=NULL;
							splineRotationY			=NULL;
							splineRotationZ			=NULL;
							
							splineScale					=NULL;

							loopStart						=0.0f;
							loopEnd							=1.0f;
							loopType						=GRPPATH_LOOP_NONE;
							loopDirection       =true;

							step								=0.01f;
							currentstep					=0.0f;
							active							=false;						
						}
						

};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

