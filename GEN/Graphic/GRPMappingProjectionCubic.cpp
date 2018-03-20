
/*------------------------------------------------------------------------------------------
//	GRPMAPPINGPROJECTIONCUBIC.CPP
//	
//	Cubic U,Vs projection
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 18/08/2014 17:07:33
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include <math.h>	

#include "GRPElement.h"

#include "GRPMappingProjectionCubic.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


bool GRPMAPPINGPROJECTIONCUBIC::ProjectMapping(GRPELEMENT* object)
{

	object->UpdateBoundingBox();

	double width		=	object->GetBoundingBox()->maxpoint.x	-	object->GetBoundingBox()->minpoint.x;
	double height		=	object->GetBoundingBox()->maxpoint.y	-	object->GetBoundingBox()->minpoint.y;
	double depth		=	object->GetBoundingBox()->maxpoint.z	-	object->GetBoundingBox()->minpoint.z;


	GRPVERTEX* data=object->GetMesh()->GetVertexData();
	XDWORD nv=object->GetMesh()->GetNVertex();

	double minx=object->GetBoundingBox()->minpoint.x;
	double miny=object->GetBoundingBox()->minpoint.y;
	double minz=object->GetBoundingBox()->minpoint.z;
	

	for (XDWORD e=0;e<nv;e++)
	{
		GRPVERTEX* vertex=&data[e];

		double x=vertex->position[0];
		double y=vertex->position[1];
		double z=vertex->position[2];

		double u=0.0;
		double v=0.0;

		int looks=0;

		double nx=fabs(vertex->normal[0]);
		double ny=fabs(vertex->normal[1]);
		double nz=fabs(vertex->normal[2]);

		if ((nz>nx) && (nz>ny))		
		{
			u=(x-minx)/(width);
			v=(y-miny)/(height);
		}
		else
		if ((nx>nz) && (nx>ny))		
		{
			u=(x-minx)/(width);
			v=(z-minz)/(depth);
		}			
		else
		if ((ny>nx) && (ny>nz))		
		{
			u=(z-minz)/(depth);
			v=(y-miny)/(height);
		}		

		vertex->texturecoordinates[0]=(float)u;
		vertex->texturecoordinates[1]=(float)v;
	}


	return true;
}
