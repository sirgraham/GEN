/*------------------------------------------------------------------------------------------
//	GRPMAPPINGPROJECTION.CPP
//	
//	Generates U,V,W mapping coordinates
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 18/08/2014 16:24:42
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPElement.h"
#include "GRPMappingProjection.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//	GRPMAPPINGPROJECTION::GRPMAPPINGPROJECTION
*/	
/**	
//	
//	Class Constructor GRPMAPPINGPROJECTION
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			18/08/2014 16:26:30
//	
*/
/*-----------------------------------------------------------------*/
GRPMAPPINGPROJECTION::GRPMAPPINGPROJECTION()
{
	direction.vector	[0] = 0.0f;
	direction.vector	[1] = 0.0f;
	direction.vector	[2] = 1.0f;

	scalefactor.vector[0] = 1.0f;
	scalefactor.vector[1] = 1.0f;
	scalefactor.vector[2] = 1.0f;
}



/*-------------------------------------------------------------------
//	GRPMAPPINGPROJECTION::~GRPMAPPINGPROJECTION
*/	
/**	
//	
//	 Class Destructor GRPMAPPINGPROJECTION
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			18/08/2014 16:28:28
//	
*/
/*-----------------------------------------------------------------*/
GRPMAPPINGPROJECTION::~GRPMAPPINGPROJECTION()
{

}

	

/*-------------------------------------------------------------------
//	GRPMAPPINGPROJECTION::GRPMAPPINGPROJECTION
*/	
/**	
//	
//	Class Constructor GRPMAPPINGPROJECTION
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			18/08/2014 16:28:55
//	
//  @param				object : 
//  @param				material : 
*/
/*-----------------------------------------------------------------*/
GRPMAPPINGPROJECTION::GRPMAPPINGPROJECTION(GRPELEMENT* object, GRPMATERIAL* material)
{
	ProjectMapping(object);
}



/*-------------------------------------------------------------------
//	GRPMAPPINGPROJECTION::ReassignUVs
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			18/08/2014 16:32:54
//	
//  @param				object : 
*/
/*-----------------------------------------------------------------*/
void GRPMAPPINGPROJECTION::ReassignUVs(GRPELEMENT* object)
{
	/*
	object->SetFaceBuffers();

	long long vsize		=object->GetFaces()->GetSize();
	GRPVERTEX* vdata=object->GetVertexData();

	
	for(long long  e=vsize-1; e>=0; e--)
	//for(XDWORD e=0; e<vsize; e++)
		{
			GRPFACE* face = object->GetFaces()->Get((int)e);

					vdata[face->a].texturecoordinates[0] = face->texturecoordinates[0]->x;
					vdata[face->a].texturecoordinates[1] = face->texturecoordinates[0]->y;
					vdata[face->b].texturecoordinates[0] = face->texturecoordinates[1]->x;
					vdata[face->b].texturecoordinates[1] = face->texturecoordinates[1]->y;
					vdata[face->c].texturecoordinates[0] = face->texturecoordinates[2]->x;
					vdata[face->c].texturecoordinates[1] = face->texturecoordinates[2]->y;				
		}			
	object->SetIsLoaded(false);
	*/
}

