/*------------------------------------------------------------------------------------------
//	GRPFACE.H
*/	
/**	
// \class 
//   
//  Holds face data
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 25/08/2014 11:03:55
//	Last Modification	:	
*/	
//*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPFACE_H_
#define _GRPFACE_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
	
#include "XVector.h"

#include "GRPVector.h"
#include "GRPVertex.h"

#include "XMemory.h"
#include "GRPOpenGL.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/




class GRPFACE
{
	public:		
																		GRPFACE										()																							
																		{                          
 																			Clean();		
																		}

																		GRPFACE										(GRPPOINT* a,GRPPOINT* b,GRPPOINT* c)
																		{
																					Clean();

																					Add(a);
																					Add(b);
																					Add(c);

																					this->a=a->index;
																					this->b=b->index;
																					this->c=c->index;

																					normals[0]							=normals[1]						=normals[2]						=NULL;																			
																					binormals[0]						=binormals[1]					=binormals[2]					=NULL;																			
																					tangents[0]							=tangents[1]					=tangents[2]					=NULL;																			
																					materialID=0;

																		}

																		GRPFACE										(GRPFACE& face)
																		{
																				n=0;
																			for (XDWORD e=0; e<3; e++)
																			{
																				 //------------ position
																					GRPPOINT* point  = new GRPPOINT(*(face.vertices[e]));
																					Add(point);	

																				 //------------ normal
																					if (face.normals[e]!=NULL)
																					{
																					GRPPOINT* normal  = new GRPPOINT((face.normals[e]));																			
																					this->normals			[e]=normal;
																					}
																					else
																						this->normals[e]=NULL;

																					//------------ binormal
																					if (face.binormals[e]!=NULL)
																					{
																					GRPPOINT* binormal  = new GRPPOINT((face.binormals[e]));																			
																					this->binormals			[e]=binormal;
																					}
																					else
																						this->binormals[e]=NULL;

																					//------------ tangents
																					if (face.tangents[e]!=NULL)
																					{
																					GRPPOINT* tangent  = new GRPPOINT((face.tangents[e]));																			
																					this->tangents			[e]=tangent;
																					}
																					else
																						this->tangents[e]=NULL;

																					//------------ textures
																					if (face.texturecoordinates[e]!=NULL)
																					{
																					GRPPOINT* texturecoord=new GRPPOINT((face.texturecoordinates[e]));
																					texturecoordinates[e]=texturecoord;
																					}
																					else
																						this->texturecoordinates[e]=NULL;
																			}

																			materialID	= face.materialID;
																			normal			= face.normal;
																			a						= face.a;
																			b						=	face.b;
																			c						=	face.c;								
																		}

																		GRPFACE										(GRPFACE* face)
																		{
																			if (face==NULL)
																				return;

																			n=0;
																			for (XDWORD e=0; e<3; e++)
																			{
																				//------------ position
																					GRPPOINT* point  = new GRPPOINT((face->vertices[e]));
																					Add(point);	

																					//------------ normal
																					if (face->normals[e]!=NULL)
																					{
																					GRPPOINT* normal  = new GRPPOINT((face->normals[e]));																			
																					this->normals			[e]=normal;
																					}
																					else
																						this->normals[e]=NULL;

																					//------------ binormal
																					if (face->binormals[e]!=NULL)
																					{
																					GRPPOINT* binormal  = new GRPPOINT((face->binormals[e]));																			
																					this->binormals			[e]=binormal;
																					}
																					else
																						this->binormals[e]=NULL;

																					//------------ tangent
																					if (face->tangents[e]!=NULL)
																					{
																					GRPPOINT* tangent  = new GRPPOINT((face->tangents[e]));																			
																					this->tangents			[e]=tangent;
																					}
																					else
																						this->tangents[e]=NULL;

																					//this->binormals		[e]=face->binormals	[e];
																					//this->cotangents	[e]=face->cotangents[e];

																					//------------ texture
																					if (face->texturecoordinates[e]!=NULL)
																					{
																					GRPPOINT* texturecoord=new GRPPOINT((face->texturecoordinates[e]));
																					texturecoordinates[e]=texturecoord;
																					}
																					else
																						this->texturecoordinates[e]=NULL;
																			}

																			materialID	= face->materialID;
																			normal			= face->normal;
																			a						= face->a;
																			b						=	face->b;
																			c						=	face->c;																			
																		}
		
																	 ~GRPFACE										()
																		{																		
																			for (XDWORD e=0;e<3;e++)
																			{
																				if (vertices[e])
																				{
																					delete	vertices[e];
																									vertices[e]=NULL;
																				}
																				
																				if (normals[e])
																				{
																					delete	normals[e];
																									normals[e]=NULL;																																							
																				}

																				if (texturecoordinates[e])
																				{
																					delete	texturecoordinates[e];
																									texturecoordinates[e]=NULL;

																				}

																				if (binormals[e])
																				{
																					delete	binormals[e];
																									binormals[e]=NULL;
																				}

																				if (tangents[e])
																				{
																					delete	tangents[e];																			
																									tangents[e]=NULL;
																				}

																			} //*/
																		}

		void														Add												(GRPPOINT* point)
																		{
																			//vertices.Add(point);	
																			vertices[n++]=point;
																		}

		void														CalculateNormal						()
																		{							

																			float abx=vertices[1]->x-vertices[0]->x;
																			float aby=vertices[1]->y-vertices[0]->y;
																			float abz=vertices[1]->z-vertices[0]->z;

																			float acx=vertices[2]->x-vertices[0]->x;
																			float acy=vertices[2]->y-vertices[0]->y;
																			float acz=vertices[2]->z-vertices[0]->z;

																			normal.vector[0] = (aby*acz) - (abz*acy);
																			normal.vector[1] = (abz*acx) - (abx*acz);
																			normal.vector[2] = (abx*acy) - (aby*acx);																				

																			normal.Normalize();																		  
																		}

		GRPPOINT*												vertices						[3];
		GRPPOINT*												texturecoordinates	[3];
		GRPPOINT*												normals							[3];
		GRPPOINT*												binormals						[3];
		GRPPOINT*												tangents						[3];
		GRPVECTOR												normal;		

		int															materialID;

		XDWORD															a;
		XDWORD															b;
		XDWORD	                            c;

		XDWORD															_a;
		XDWORD															_b;
		XDWORD                              _c;

		int															n;

		void 	Print()
		{
			XDEBUG_PRINTCOLOR(
				0,
				__L("V1: %+f,%+f,%+f V2: %+f,%+f,%+f V3: %+f,%+f,%+f N: %+f,%+f,%+f a:%d b:%d c:%d"),
				vertices[0]->x,vertices[0]->y,vertices[0]->z,
				vertices[1]->x,vertices[1]->y,vertices[1]->z,
				vertices[2]->x,vertices[2]->y,vertices[2]->z,
				normal.vector[0],normal.vector[1],normal.vector[2],
				a,b,c
				);
		}
								

private:
		void														Clean											()
																		{
																			vertices[0]							=vertices[1]					=vertices[2]					=NULL;
																			texturecoordinates[0]		=texturecoordinates[1]=texturecoordinates[2]=NULL;
																			normals[0]							=normals[1]						=normals[2]						=NULL;
																			binormals[0]						=binormals[1]					=binormals[2]					=NULL;
																			tangents[0]							=tangents[1]					=tangents[2]					=NULL;
																			
																			a						= 0;
																			b						= 1;
																			c						= 2;
																			normal			= GRPVECTOR(0.0,0.0,0.0);
																			n						=	0;
																			materialID	= 0;
																		}
};

	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

