/*------------------------------------------------------------------------------------------
//	GRPMESH.H
*/	
/**	
// \class 
//   
//  Mesh holder
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 14/07/2016 13:32:28
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPMESH_H_
#define _GRPMESH_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
#include "XVector.h"
#include "GRPVertex.h"
#include "GRPBoundingBox.h"
#include "GRPOpenGL.h"
#include "GRPNamed.h"

	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

class GRPFACE;
struct GRPVERTEX;
	

enum GRPELEMENTRENDERFLAGS
{
	GRPELEMENTRENDERFLAGS_UNKNOWN						= 0,
	GRPELEMENTRENDERFLAGS_UPDATEMATRIX			= 1<<0,
	GRPELEMENTRENDERFLAGS_LOADMATRIXTOGPU		= 1<<1,
	GRPELEMENTRENDERFLAGS_LOADMATERIALTOGPU	= 1<<2,
	GRPELEMENTRENDERFLAGS_DRAW							= 1<<3,
	GRPELEMENTRENDERFLAGS_PROCESSMORPHS			= 1<<4,
	GRPELEMENTRENDERFLAGS_DEPTHPASS					= GRPELEMENTRENDERFLAGS_PROCESSMORPHS			| GRPELEMENTRENDERFLAGS_UPDATEMATRIX			| GRPELEMENTRENDERFLAGS_LOADMATRIXTOGPU		| GRPELEMENTRENDERFLAGS_DRAW,
	GRPELEMENTRENDERFLAGS_COLORPASS					=	GRPELEMENTRENDERFLAGS_PROCESSMORPHS			|	GRPELEMENTRENDERFLAGS_LOADMATRIXTOGPU		| GRPELEMENTRENDERFLAGS_LOADMATERIALTOGPU | GRPELEMENTRENDERFLAGS_DRAW,
	GRPELEMENTRENDERFLAGS_FULL							= GRPELEMENTRENDERFLAGS_PROCESSMORPHS			| GRPELEMENTRENDERFLAGS_UPDATEMATRIX			| GRPELEMENTRENDERFLAGS_LOADMATRIXTOGPU		| GRPELEMENTRENDERFLAGS_LOADMATERIALTOGPU	| GRPELEMENTRENDERFLAGS_DRAW
};



/*---- CLASS -----------------------------------------------------------------------------*/
class GRPMESH	: public virtual GRPNAMED 
{
public:

														GRPMESH										();
virtual											~GRPMESH									();

				void								Destroy										();
				bool								IsInstance								();

				GLUINT							GetVertexVBOID						();
				GLUINT							GetIndexVBOID							();

				void								SetVertexVBOID						(GLUINT i);
				void								SetIndexVBOID							(GLUINT i);

				void   							SetNvertexperface					(	int					nvertexperface);

				XDWORD 							GetNvertexData						();
				void   							SetNvertexData						(	XDWORD			nvertexdata);

				GRPVERTEX* 					GetVertexData							();
				void    						SetVertexData							(	GRPVERTEX*	vertexdatapointer);

				void   							SetIndexData							(	XWORD*			indexdatapointer);

				XDWORD  						GetNindexData							();
				void   							SetNindexData							(	XDWORD			nindexdata);

		
				XDWORD							GetNVertex								();
				void								SetNVertex								(	int				  nvertex);
				XWORD*							GetIndexData							();

				
				XDWORD							GetNIndex									();
				XWORD								GetNFaces									();
				XVECTOR<GRPFACE*>*	GetFaces									();
				int									GetNVertexPerFace					();

				bool								GetIsLoaded								();
				void								SetIsLoaded								(	bool				loadstatus);	
				void								SetMaxindex								(	int					maxindex);	
				int									GetMaxindex								() { return maxindex; }

				bool								LoadModel									(	int					nvertex, 
																												GLFLOAT*		vertexpositions, 
																												GLFLOAT*		vertexnormals, 
																												GLFLOAT*		vertextexturecoords 																																
																											);

				bool								LoadModel									(	XVECTOR<GRPFACE*>*_faces);
				bool								Create										(	XVECTOR<GLFLOAT>* Vertexes,
																												XVECTOR<GLINT>*		Indexes,
																												XVECTOR<GLFLOAT>* Normals
																											);
				bool								SetFaceBuffers						();
				void								PopulateVertices					();
				void								ReassignUVs								();	


				void								Merge											(GRPMESH* mesh);

XVECTOR<GRPVERTEX_CHANNEL>* GetVertexLayers						()				 { return &this->vertexlayers; }
				XDWORD							GetNumberOfLayers					()				 { return this->numberoflayers; }
				void								SetNumberOfLayers					(XDWORD n) { this->numberoflayers=n; }



			//-------------------------- Mesh Lattice

		int															nvertexperface;
		XDWORD													nvertexdata;		
		GRPVERTEX*											vertexdata;
		XWORD*													indexdata;
		XDWORD													nindexdata;

		XVECTOR<GRPFACE*>								faces;
		XVECTOR<XDWORD>									indices;
		XDWORD													maxindex;
		

		GLUINT													vertexVBOID;
		GLUINT													indexVBOID;
				
		GLUINT													VBA;
		
		//--------------------------- status

		bool														isloaded;		
		
		bool														isprocedural;
		XBYTE														numberoflayers; //the number of vertex layers is limited by the GPU hardware , by default max is 255, we use around 20

		XVECTOR<GRPVERTEX_CHANNEL>			vertexlayers;
		
private:

		  void													Clean											()
																		{																				
																		nvertexperface														=3;
																		nvertexdata																=0;
																		vertexdata																=NULL;
																		indexdata																	=0;
																		nindexdata																=0;																		
																		maxindex																	=0;																	
																		vertexVBOID																=0;
																		indexVBOID																=0;																																					
																		isloaded																	=false;
																		isprocedural															=true;
																		numberoflayers														=0;
																		VBA																				=0;
																		}
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

