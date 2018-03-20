/*------------------------------------------------------------------------------------------
//	GRPVIEWPORT.H
*/	
/**	
// \class 
//   
//  Graphics View Port class
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 14/04/2014 17:27:41
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPVIEWPORT_H_
#define _GRPVIEWPORT_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include <stdio.h>

#include "GRPOpenGL.h"
#include "GRPMatrix.h"

#include "XSubject.h"
#include "GRPXEvent.h"
#include "GRPNamed.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	

enum GRPPROJECTIONTYPE
{
	GRPPROJECTIONTYPE_UNKNOWN		=	0	,
	GRPPROJECTIONTYPE_ORTHO					,
	GRPPROJECTIONTYPE_FRUSTUM				,
	GRPPROJECTIONTYPE_PIXELS				,
};


/*---- CLASS -----------------------------------------------------------------------------*/

class GRPSCREEN;
class XFACTORY;

class GRPVIEWPORT : public XSUBJECT,public GRPNAMED
{
	public:
													GRPVIEWPORT													( );
													
		virtual							 ~GRPVIEWPORT													();
		
		void									GetPosition													(GLFLOAT& x, GLFLOAT& y)							{ x = this->x;  y = this->y;											}
		void									SetPosition													(GLFLOAT  x, GLFLOAT  y)							{ this->x = x;  this->y = y;											}
	
		void									GetSize															(GLFLOAT& width, GLFLOAT& height)			{ width = this->width;   height = this->height;		}
		void									SetSize															(GLFLOAT  width, GLFLOAT  height)			{ this->width = width;   this->height = height;		}
	
		GRPPROJECTIONTYPE			GetProjection												()																		{ return projectiontype;													}
		void									SetProjection												(GRPPROJECTIONTYPE projectiontype)		{ this->projectiontype = projectiontype;	modified=true;				}


		void									GetProjectionFrustum								(GLFLOAT& projectionfrustumnear, GLFLOAT& projectionfrustumfar);
	  void									SetProjectionFrustum								(GLFLOAT projectionfrustumnear,	 GLFLOAT projectionfrustumfar);
		bool									Select															();
		void									SetFullScreen												();
		void									SetRealScreen												(GRPSCREEN * sc);
		void									SetViewport													();
		void									SelectProjectionOrtho								();
		void									SelectProjectionFrustum							();
		void									SelectProjectionPixels							(GLFLOAT sc=1.0f);

		void									SetWindowSize												(GLFLOAT width, GLFLOAT height)
													{ 
														this->realpixelwidth = width;
														this->realpixelheight = height;

														if(forceaspectratio)
															{
																this->pixelwidth	=	width;
																this->pixelheight	=	forcedaspectratio*height;
															}
														else
															{
																this->pixelwidth	=	width;
																this->pixelheight	=	height;
															}

														AspectRatio	= ((GLfloat)pixelwidth) / ((GLfloat)pixelheight); // IMANOL : test to try

														GRPXEVENT event(this, GRPXEVENTTYPE_VIEWPORT);
														event.SetSize((int)pixelwidth, (int)pixelheight);

														modified=true;

														PostEvent(&event);
													}

		void									GetWindowSize												(GLFLOAT &width, GLFLOAT &height)
													{
														width		= pixelwidth;
														height	= pixelheight;
													}

		void									GetRealWindowSize										(GLFLOAT& width, GLFLOAT& height)
													{
														width		= realpixelwidth;
														height	= realpixelheight;
													}


		void									SetOrthoScale												(float orthoscale)										{ this->orthoscale = orthoscale;		}
		float									GetOrthoScale												()																		{ return this->orthoscale;					}

		void									SetFieldOfView											(GLFLOAT fov)													{ this->fieldofview=fov;	 modified=true;		}
		float									GetFieldOfView											()																		{ return this->fieldofview;					}


		float									GetAspectRatio											()																		{ return AspectRatio;								}
		void									SetAspectRatio											(float aspectratio)										{ this->AspectRatio = aspectratio;modified=true;	}

		bool									IsForcedAspectRatio									()																		{ return forceaspectratio;					}
		float									GetForcedAspectRatio								()																		{ return forcedaspectratio;					}

		void									ForceAspectRatio										(bool force = false, float aspectratio = 1.0f)
													{
														if(force == true)
															{
																forceaspectratio = force;
																forcedaspectratio = aspectratio;
																SetWindowSize(realpixelwidth, realpixelheight);

																// set the viewportposition automatically
																float difference = realpixelheight - pixelheight;
																difference = difference/* / 2.0f*/;
																float yreal = 0.0f;

																yreal = difference/pixelheight;

																SetPosition(x, yreal);
															}
													}
		

		GRPPOINT							GetViewPortFromNormalized						(GRPPOINT pt);
		GRPPOINT							GetViewPortFromNormalized						(float x, float y, float z);

		GRPPOINT							GetViewPortFromInverseNormalized		(GRPPOINT pt);
		GRPPOINT							GetViewPortFromInverseNormalized		(float x, float y, float z);

		GRPPOINT							GetNormalizedFromViewPort						(float x, float y);
		GRPPOINT							GetInverseNormalizeFromViewPort			(float x, float y);

		GRPPOINT							GetViewPortFromPixels								(GRPPOINT pt);
		GRPPOINT							GetViewPortFromPixels								(float px, float py);

													operator GRPMATRIX*									()
													{
														switch(projectiontype)
														{
														case GRPPROJECTIONTYPE_FRUSTUM:
															return &FrustumMatrix;

														case GRPPROJECTIONTYPE_ORTHO:
															return &OrthographicMatrix;
														case GRPPROJECTIONTYPE_PIXELS:
															return &PixelMatrix;
														}
														return NULL;
													}

		bool									HasCameraAttached										()																		{ return attachedCam!=-1; }
		int										GetAttachedCam											()																		{ return attachedCam;			}
		void									AttachCam														(int n)																{ attachedCam=n;					}

		void									SetIsActive													(bool active)													{ this->active = active;	}
		bool									IsActive														()																		{ return this->active;		}

		bool									modified;

	protected:		
		
		GLFLOAT								x;
		GLFLOAT								y;
		GLFLOAT								width;
		GLFLOAT								height;

		GLFLOAT								pixelwidth;
		GLFLOAT								pixelheight;

		GLFLOAT								realpixelwidth;
		GLFLOAT								realpixelheight;

		GRPPROJECTIONTYPE			projectiontype;		 
		GLFLOAT								projectionfrustumnear;
		GLFLOAT								projectionfrustumfar;

		GLFLOAT								orthoscale;
		GLFLOAT								fieldofview;
		float									AspectRatio;
		
		GRPMATRIX							OrthographicMatrix;
		GRPMATRIX							FrustumMatrix;
		GRPMATRIX							PixelMatrix;
				

		bool									forceaspectratio;
		float									forcedaspectratio;

		int										attachedCam;

		bool									active;

	private:

		void									Clean											()
													{
														x												= 0.0f;
														y												= 0.0f;	
														width				 						= 0.0f;
														height									= 0.0f;
	
														pixelwidth							= 0.0f;
														pixelheight							= 0.0f;

														realpixelwidth					=	0;
														realpixelheight					=	0;

														this->projectiontype		= GRPPROJECTIONTYPE_UNKNOWN;
														projectionfrustumnear		= 0.0f;
														projectionfrustumfar	  = 0.0f;
														orthoscale							= 1.0f;
														fieldofview							=	54.0f;
														AspectRatio							= 0.0f;
														
														forceaspectratio				= false;
														attachedCam							=-1;
														active									= true;
														modified								= true;
													}


	
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

