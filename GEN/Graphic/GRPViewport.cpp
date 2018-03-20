
/*------------------------------------------------------------------------------------------
//	GRPVIEWPORT.CPP
//	
//	Graphics View Port class
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 15/04/2014 15:53:56
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include <math.h>

#include "GRPScreen.h"
#include "XDebug.h"
#include "GRPViewport.h"
#include "GRPMatrix.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//	GRPVIEWPORT::GRPVIEWPORT
*/	
/**	
//	
//	Class Constructor GRPVIEWPORT
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			15/04/2014 15:57:30
//	
//  @param				grpscreen : 
*/
/*-----------------------------------------------------------------*/
GRPVIEWPORT::GRPVIEWPORT( )
{
	Clean();
	this->FrustumMatrix.LoadIdentity();
	this->OrthographicMatrix.LoadIdentity();

	width				 						= 1.0f;
	height									= 1.0f;
		
	this->projectiontype		= GRPPROJECTIONTYPE_ORTHO;
	projectionfrustumnear   = 0.1f;
	projectionfrustumfar	  = 10000.0f;


	RegisterEvent(GRPXEVENTTYPE_VIEWPORT);
}
		


/*-------------------------------------------------------------------
//	GRPVIEWPORT::~GRPVIEWPORT
*/	
/**	
//	
//	 Class Destructor GRPVIEWPORT
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			15/04/2014 15:58:17
//	
*/
/*-----------------------------------------------------------------*/
GRPVIEWPORT::~GRPVIEWPORT()
{
	DeRegisterEvent(GRPXEVENTTYPE_VIEWPORT);
	Clean();
}



/*-------------------------------------------------------------------
//	GRPVIEWPORT::Select
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			15/04/2014 17:57:10
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPVIEWPORT::Select()
{ 
	if (modified)
	{
	switch(projectiontype)
		{
			case GRPPROJECTIONTYPE_ORTHO		: SelectProjectionOrtho();		break;
			case GRPPROJECTIONTYPE_FRUSTUM	: SelectProjectionFrustum();	break;
			default: break;
		}
	modified=false;
	}
	return true;
}




/*-------------------------------------------------------------------
//	GRPVIEWPORT::SetViewport
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			13/05/2014 11:18:54
//	
*/
/*-----------------------------------------------------------------*/
void GRPVIEWPORT::SetViewport()
{

	// check state to prevent unneed call
	if(	(pixelwidth		!= GRPOPENGLCTRL::VIEWPORT::pixelwidth)			|| 
			(pixelheight	!= GRPOPENGLCTRL::VIEWPORT::pixelheight)		|| 
			(x						!= GRPOPENGLCTRL::VIEWPORT::x)							|| 
			(y						!= GRPOPENGLCTRL::VIEWPORT::y)							|| 
			(width				!= GRPOPENGLCTRL::VIEWPORT::width)					||
			(height				!= GRPOPENGLCTRL::VIEWPORT::height))
			{
			if (width < 0 || height < 0) 
					return;

			 OPENGLCHECKERROR(__L("GRPVIEWPORT::SetViewport()"));
				glViewport(	(GLint)  (pixelwidth  * x), 
										(GLint)  (pixelheight * y), 
										(GLsizei)(pixelwidth  * width), 
										(GLsizei)(pixelheight * height));

				OPENGLCHECKERROR(__L("GRPVIEWPORT::SetViewport()"));

				GRPOPENGLCTRL::VIEWPORT::pixelwidth		= pixelwidth;
				GRPOPENGLCTRL::VIEWPORT::pixelheight	= pixelheight;
				GRPOPENGLCTRL::VIEWPORT::x						= x;
				GRPOPENGLCTRL::VIEWPORT::y						= y;
				GRPOPENGLCTRL::VIEWPORT::width				=	width;
				GRPOPENGLCTRL::VIEWPORT::height				= height;
			}

	
}



/*-------------------------------------------------------------------
//	GRPVIEWPORT::SelectProjectionOrtho
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			16/04/2014 14:02:14
//	
*/
/*-----------------------------------------------------------------*/
void GRPVIEWPORT::SelectProjectionOrtho()
{
	//if(pixelheight <= EPSILON)
	//	{
	//		AspectRatio = 1.0f;
	//	}
	//else
		{
			AspectRatio	= ((GLfloat)pixelwidth) / ((GLfloat)pixelheight);
		}

	this->SetViewport();
	this->OrthographicMatrix.BuildOrthoProjectionMatrix(-orthoscale, orthoscale,
																											-orthoscale / AspectRatio,		
																											 orthoscale / AspectRatio,
																											 projectionfrustumnear,
																											 projectionfrustumfar);

	// Imanol: TODO, make projection dimensions configurable, to avoid accuracy loss on the UI due to float conversions
	// instead of pixelwidth/height use separate values?
	
	this->projectiontype=GRPPROJECTIONTYPE_ORTHO;
	
}

void GRPVIEWPORT::SelectProjectionPixels(GLFLOAT sc)
{
	this->SetViewport();
	this->PixelMatrix.BuildPixelProjectionMatrix				(	0, float(pixelwidth*sc),
																												0, float(pixelheight*sc),
																											 projectionfrustumnear,
																											 projectionfrustumfar);
	
	this->projectiontype=GRPPROJECTIONTYPE_PIXELS;
}

/*-------------------------------------------------------------------
//	GRPVIEWPORT::SelectProjectionFrustum
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			16/04/2014 14:02:19
//	
*/
/*-----------------------------------------------------------------*/
void GRPVIEWPORT::SelectProjectionFrustum()
{    	
	this->SetViewport();

	GLfloat m_fZNear=this->projectionfrustumnear;
	GLfloat m_fZFar=this->projectionfrustumfar;
	GLfloat m_fFOV=this->fieldofview;	
	GLfloat fRightEdge=0.0f;

	GLFLOAT d=pixelheight;

	// this one checks how stretched it's going to render
	AspectRatio	= ((GLfloat)pixelwidth) / ((GLfloat)pixelheight);	
	fRightEdge		= (GLfloat)(m_fZNear * tan(m_fFOV * PI / 360.0f));

	
	// Calculate Projection Matrix																						
		
	this->FrustumMatrix.BuildFrustumProjectionMatrix(-fRightEdge, fRightEdge,
																									 -fRightEdge / AspectRatio, 
																										fRightEdge / AspectRatio,
																										m_fZNear	, m_fZFar);	

	this->projectiontype=GRPPROJECTIONTYPE_FRUSTUM;
}





/*-------------------------------------------------------------------
//	GRPVIEWPORT::GetProjectionFrustum
*/	
/**	
//	
//	Gets the current projection
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			23/04/2014 11:48:36
//	
//	@return 			GLFLOAT : 
//
//  @param				projectionfrustumnear : 
//  @param				projectionfrustumfar : 
*/
/*-----------------------------------------------------------------*/
void GRPVIEWPORT::GetProjectionFrustum(GLFLOAT& projectionfrustumnear, GLFLOAT& projectionfrustumfar)				
{  
	projectionfrustumnear = this->projectionfrustumnear;
	projectionfrustumfar  = this->projectionfrustumfar;	
}




/*-------------------------------------------------------------------
//	GRPVIEWPORT::SetProjectionFrustum
*/	
/**	
//	
//	Sets the projection type
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			23/04/2014 11:48:49
//	
//  @param				projectionfrustumnear : 
//  @param				projectionfrustumfar : 
*/
/*-----------------------------------------------------------------*/
void GRPVIEWPORT::SetProjectionFrustum(GLFLOAT projectionfrustumnear, GLFLOAT projectionfrustumfar) 
{
	this->projectionfrustumnear = projectionfrustumnear;
	this->projectionfrustumfar  = projectionfrustumfar;
}



/*-------------------------------------------------------------------
//	GRPVIEWPORT::SetFullScreen
*/	
/**	
//	
//	Sets the viewport to screen full size
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/04/2014 11:47:20
//	
*/
/*-----------------------------------------------------------------*/
void GRPVIEWPORT::SetFullScreen()
{
	SetPosition(0.0f, 0.0f);								

  SetSize(1.0f, 1.0f);			
	
}

/*-------------------------------------------------------------------
//	 GRPVIEWPORT::SetRealScreen
*/
/**
//
//	
//
//	@author		Diego Martinez
//	@version		

//	@return		void : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
void GRPVIEWPORT::SetRealScreen(GRPSCREEN* sc)
{
	SetFullScreen();
	SetWindowSize((float)sc->GetWidth(),(float)sc->GetHeight());
	SetPosition(0.0, 0.0);
}




/*-------------------------------------------------------------------
//	GRPVIEWPORT::GetViewPortFromNormalized
*/	
/**	
//	
//	gets viewport coordinates from normalized
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			29/10/2015 17:11:33
//	
//	@return 			GRPPOINT : 
//
//  @param				pt : 
*/
/*-----------------------------------------------------------------*/
GRPPOINT GRPVIEWPORT::GetViewPortFromNormalized(GRPPOINT pt)
{
	return GetViewPortFromNormalized(pt.x, pt.y, pt.z);
}




/*-------------------------------------------------------------------
//	GRPVIEWPORT::GetViewPortFromNormalized
*/	
/**	
//	
//	gets viewport coordinates from normalized
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			29/10/2015 17:11:43
//	
//	@return 			GRPPOINT : 
//
//  @param				x : 
//  @param				y : 
//  @param				z : 
*/
/*-----------------------------------------------------------------*/
GRPPOINT GRPVIEWPORT::GetViewPortFromNormalized(float x, float y, float z)
{
	GRPPOINT pt;

	pt.x = x*2.0f - 1.0f;
	pt.y = y*(1.0f/GetAspectRatio())*2.0f - 1.0f/GetAspectRatio();
	pt.z = z;

	return pt;
}




/*-------------------------------------------------------------------
//	GRPVIEWPORT::GetViewPortFromInverseNormalized
*/	
/**	
//	
//	gets viewport coordinates from inversely normalized (top left corner)
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			29/10/2015 17:12:07
//	
//	@return 			GRPPOINT : 
//
//  @param				pt : 
*/
/*-----------------------------------------------------------------*/
GRPPOINT GRPVIEWPORT::GetViewPortFromInverseNormalized(GRPPOINT pt)
{
	return GetViewPortFromInverseNormalized(pt.x, pt.y, pt.z);
}





/*-------------------------------------------------------------------
//	GRPVIEWPORT::GetViewPortFromInverseNormalized
*/	
/**	
//	
//	gets viewport coordinates from inversely normalized (top left corner)
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			29/10/2015 17:12:17
//	
//	@return 			GRPPOINT : 
//
//  @param				x : 
//  @param				y : 
//  @param				z : 
*/
/*-----------------------------------------------------------------*/
GRPPOINT GRPVIEWPORT::GetViewPortFromInverseNormalized(float x, float y, float z)
{
	GRPPOINT pt;

	pt.x = x*2.0f - 1.0f;
	pt.y = -y*(1.0f/GetAspectRatio())*2.0f + 1.0f/GetAspectRatio();

	pt.z = z;

	return pt;
}




/*-------------------------------------------------------------------
//	GRPVIEWPORT::GetNormalizedFromViewPort
*/	
/**	
//	
//	gets normalized coordinates from viewport ones
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			30/10/2015 13:13:06
//	
//	@return 			GRPPOINT : 
//
//  @param				x : 
//  @param				y : 
*/
/*-----------------------------------------------------------------*/
GRPPOINT GRPVIEWPORT::GetNormalizedFromViewPort(float x, float y)
{
	GRPPOINT pt;

	pt.x = x + 1.0f;
	pt.x /= 2.0f;
	pt.y = y + 1.0f/GetAspectRatio();
	pt.y /= (1.0f/GetAspectRatio())*2.0f;
	pt.z = 0.0;

	return pt;
}




/*-------------------------------------------------------------------
//	GRPVIEWPORT::GetInverseNormalizeFromViewPort
*/	
/**	
//	
//	gets inverse normalize coordinates(top left) form  viewport ones
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			30/10/2015 13:13:28
//	
//	@return 			GRPPOINT : 
//
//  @param				x : 
//  @param				y : 
*/
/*-----------------------------------------------------------------*/
GRPPOINT GRPVIEWPORT::GetInverseNormalizeFromViewPort(float x, float y)
{
	GRPPOINT pt;

	pt.x = x + 1.0f;
	pt.x /= 2.0f;
	pt.y = y - 1.0f/GetAspectRatio();
	pt.y /= -(1.0f/GetAspectRatio())*2.0f;
	pt.z = 0.0;

	return pt;
}



/*-------------------------------------------------------------------
//	GRPVIEWPORT::GetViewPortFromPixels
*/	
/**	
//	
//	gets viewport coordinates from screen pixels
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			29/10/2015 17:42:35
//	
//	@return 			GRPPOINT : 
//
//  @param				pt : 
*/
/*-----------------------------------------------------------------*/
GRPPOINT GRPVIEWPORT::GetViewPortFromPixels(GRPPOINT pt)
{
	return GetViewPortFromPixels(pt.x, pt.y);
}


/*-------------------------------------------------------------------
//	GRPVIEWPORT::GetViewPortFromPixels
*/	
/**	
//	
//	gets viewport coordinates from screen pixels
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			29/10/2015 17:42:49
//	
//	@return 			GRPPOINT : 
//
//  @param				px : 
//  @param				py : 
*/
/*-----------------------------------------------------------------*/
GRPPOINT GRPVIEWPORT::GetViewPortFromPixels(float px, float py)
{
	GRPPOINT pt;
	pt.Clean();

	pt.x = px;
	pt.y = py;

	pt.x /= pixelwidth;
	pt.y /= pixelheight;

	// now on 0 1

	pt.x -= x;
	pt.y -= y;

	pt.x /= width;
	pt.y /= height;

	// not correct, viewport doesn't go form 0 to 1, but form -1 to +1 on x, and -aspect to +aspect on y
	pt.x *= 2.0f;
	pt.x -= 1.0f;

	pt.y *= 1.0f/GetAspectRatio()*2.0f;
	pt.y -= 1.0f/GetAspectRatio();

	return pt;
}

