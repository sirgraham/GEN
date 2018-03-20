/*------------------------------------------------------------------------------------------
//	GRPOBJECTBUILDERALGORITHMEXTRUDE.cpp
*/	
/**	
// \class 
//   
//  Extrudes a 2D spline into 3d
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 12/06/2014 11:22:22
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	

/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPShape.h"
#include "GRPObjectBuilder.h"
#include "GRPObjectBuilderAlgorithmExtrude.h"
#include "GRPElement.h"

#include "XMemory.h"
		
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/



/*---- CLASS -----------------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//	GRPOBJECTBUILDERALGORITHMEXTRUDE::GRPOBJECTBUILDERALGORITHMEXTRUDE
*/	
/**	
//	
//	Class Constructor GRPOBJECTBUILDERALGORITHMEXTRUDE
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			23/06/2014 9:53:26
//	
//  @param				builder : 
*/
/*-----------------------------------------------------------------*/
GRPOBJECTBUILDERALGORITHMEXTRUDE::GRPOBJECTBUILDERALGORITHMEXTRUDE(GRPOBJECTBUILDER* builder) 
{ 
	this->builder		=	builder;
	closeSpline			=	true;
	this->nsegments	=1;
	this->depth			=1.0f;
	doublesided			=false;
	UnifyNormals		=true;				
	ReverseNormals  =false;
}




/*-------------------------------------------------------------------
//	GRPOBJECTBUILDERALGORITHMEXTRUDE::Triangulize
*/	
/**	
//	
//	Executes the Extrusion algorithm
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			23/06/2014 9:53:40
//	
//	@return 			GRPELEMENT* : 
//
//  @param				sShape : 
*/
/*-----------------------------------------------------------------*/
GRPELEMENT*	GRPOBJECTBUILDERALGORITHMEXTRUDE::Execute(GRPSHAPE* shape)
{
if(!builder) return NULL;
GRPELEMENT* element = new GRPELEMENT();
GRPMESH* mesh=new GRPMESH();
if (element)
{
element->SetMesh(mesh);
element->SetName(__L("Extruded shape"));
}

	GRPSHAPE*					 _shape = builder->MergeInnerSplines(shape);							
	XVECTOR<GRPFACE*>*  faces = element->GetMesh()->GetFaces();;	


	GLFLOAT						 pernsegmentdepth = depth / nsegments;
	
	XDWORD nv=_shape->contours.Get(0)->points.GetSize();

	faces->SetAddInLimit(nv*2*nsegments);

	XVECTOR<GRPPOINT*> seg1;
	XVECTOR<GRPPOINT*> seg2;	



	 	for(int nsegment=0; nsegment<nsegments; nsegment++)
		{
			GLFLOAT newdepth =(nsegment * pernsegmentdepth);

			int newIndex=0;
			for(XDWORD e=0; e<nv; e++)
			{					
					GRPPOINT* point1	= new GRPPOINT(_shape->contours.Get(0)->points.Get(e )->value);
					point1->index			=	(nsegment*nv)+e;										
					point1->z=newdepth;
					seg1.Add(point1);

					GRPPOINT* point2	= new GRPPOINT(_shape->contours.Get(0)->points.Get(e )->value);
					point2->index			=	((nsegment+1)*nv)+e;										
					point2->z=newdepth+pernsegmentdepth;
					seg2.Add(point2);
			}
			
			

			//nv=(nv-(closeSpline ? 1 : 0));

			for(XDWORD e=0; e<nv-1; e++)
			{					
			GRPPOINT * point1 = seg1.Get(e);
			GRPPOINT * point3 = seg2.Get(e);

			XDWORD i=(e+1)%(nv);

			GRPPOINT * point2 = seg1.Get(i); 			
			GRPPOINT * point4 = seg2.Get(i);

			GRPVECTOR normal1;
			GRPVECTOR normal3;
			GRPVECTOR normal2;
			GRPVECTOR normal4;

			GRPFACE*	face=new GRPFACE();		
			if(face)
				{		
					face->Add(new GRPPOINT(*point1));	
					face->Add(new GRPPOINT(*point2));
					face->Add(new GRPPOINT(*point3));
											
					face->CalculateNormal();

					if (!UnifyNormals)
					{
					face->normals[0]=new GRPPOINT(face->normal.vector[0],face->normal.vector[1],face->normal.vector[2]);
					face->normals[1]=new GRPPOINT(face->normal.vector[0],face->normal.vector[1],face->normal.vector[2]);
					face->normals[2]=new GRPPOINT(face->normal.vector[0],face->normal.vector[1],face->normal.vector[2]);
					}
					else
					{
						normal1=GRPPOINT(-(point2->y-point1->y),(point2->x-point1->x),0.0);						
						if (i+1<nv)
						normal2=GRPPOINT(-(seg1.Get(i+1)->y-point2->y),(seg1.Get(i+1)->x-point2->x),0.0);						
						else
							normal2=GRPPOINT(-(seg1.Get(1)->y-point2->y),(seg1.Get(1)->x-point2->x),0.0);						

						if (ReverseNormals)
						{
							normal1.vector[0]*=-1;normal1.vector[1]*=-1;
							normal2.vector[0]*=-1;normal2.vector[1]*=-1;
						}
						normal1.Normalize();						
						normal2.Normalize();
						
					face->normals[0]=new GRPPOINT(normal1);
					face->normals[1]=new GRPPOINT(normal2);
					face->normals[2]=new GRPPOINT(normal1);
					}
					
					face->texturecoordinates[0]=new GRPPOINT(0.0f,0.0f,0.0f);
					face->texturecoordinates[1]=new GRPPOINT(0.0f,0.0f,0.0f);

					face->a=point1->index;
					face->b=point2->index;
					face->c=point3->index;

					faces->Add(face);

				}

					GRPVECTOR* n=&face->normal;
					face = new GRPFACE();									
					if(face)
						{
									face->Add(new GRPPOINT(*point2));	
									face->Add(new GRPPOINT(*point4));
									face->Add(new GRPPOINT(*point3));		
				
									face->normal=*n;

									if (!UnifyNormals)
									{													
										face->normals[0]=new GRPPOINT(face->normal.vector[0],face->normal.vector[1],face->normal.vector[2]);
										face->normals[1]=new GRPPOINT(face->normal.vector[0],face->normal.vector[1],face->normal.vector[2]);
										face->normals[2]=new GRPPOINT(face->normal.vector[0],face->normal.vector[1],face->normal.vector[2]);
									}
									else
									{
										face->normals[0]=new GRPPOINT(normal2);
										face->normals[1]=new GRPPOINT(normal2);
										face->normals[2]=new GRPPOINT(normal1);
									}
											
									face->texturecoordinates[0]=new GRPPOINT(0.0f,0.0f,0.0f);
									face->texturecoordinates[1]=new GRPPOINT(0.0f,0.0f,0.0f);

								face->a=point2->index;
								face->b=point4->index;
								face->c=point3->index;					

							faces->Add(face);
							
						}
			}

		
		}
	

	delete _shape;
	seg1.DeleteContents();
	seg2.DeleteContents();
	seg1.DeleteAll();
	seg2.DeleteAll();
									
	element->GetMesh()->SetMaxindex(nv*2*nsegments);
	return element;
}
//*/