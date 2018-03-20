
/*------------------------------------------------------------------------------------------
//	GRPMESH.CPP
//	
//	Mesh holder
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 14/07/2016 13:36:09
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "GRPMesh.h"

#include "GRPFace.h"
#include "GRPVertex.h"
#include "GRPProfiler.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/

/*-------------------------------------------------------------------
//	GRPMESH::GRPMESH
*/	
/**	
//	
//	Class Constructor GRPMESH
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			14/07/2016 17:02:03
//	
*/
/*-----------------------------------------------------------------*/
GRPMESH::GRPMESH()
{
	Clean();
}

/*-------------------------------------------------------------------
//	GRPMESH::~GRPMESH
*/	
/**	
//	
//	 Class Destructor GRPMESH
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			14/07/2016 17:02:07
//	
*/
/*-----------------------------------------------------------------*/
GRPMESH::~GRPMESH()
{
	Destroy();
	Clean();
}

void GRPMESH::Destroy()
{	
			GRPPROFILER::vertexmemory-=(nvertexdata*sizeof(GRPVERTEX)*nvertexdata)+(sizeof(unsigned short)*nindexdata);

			faces.DeleteContents();
			faces.DeleteAll(); 			
		
			vertexlayers.DeleteAll();

			if(vertexdata)			
			{
				delete[] vertexdata;
				vertexdata=NULL;
			}

			if(indexdata)			
			{
				delete[] indexdata;
				indexdata=NULL;
			}
	
			if (vertexVBOID)
			glDeleteBuffers(1, &vertexVBOID);

			if (indexVBOID)
			glDeleteBuffers(1, &indexVBOID);
}

/*-------------------------------------------------------------------
//	 GRPMESH::SetVertexVBOID
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	25/10/2017 16:10:35
//	@return		void : 
//
//	@param		GLUINT : 
//
*//*-----------------------------------------------------------------*/
void GRPMESH::SetVertexVBOID(GLUINT i)
{
		this->vertexVBOID = i;
}

/*-------------------------------------------------------------------
//	 GRPMESH::SetIndexVBOID
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	25/10/2017 16:10:41
//	@return		void : 
//
//	@param		GLUINT : 
//
*//*-----------------------------------------------------------------*/
void GRPMESH::SetIndexVBOID(GLUINT i)
{
		this->indexVBOID = i;
}

/*-------------------------------------------------------------------
//	GRPMESH::SetNvertexperface
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			21/09/2015 11:21:36
//	
//  @param				p : 
*/
/*-----------------------------------------------------------------*/
void GRPMESH::SetNvertexperface(int p)        																  
{ 
	this->nvertexperface=p; 											
}


/*-------------------------------------------------------------------
//	GRPMESH::GetNvertexData
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			21/09/2015 11:23:49
//	
//	@return 			XDWORD : 
//
*/
/*-----------------------------------------------------------------*/
XDWORD GRPMESH::GetNvertexData()            																  
{ 
	return nvertexdata;														
}


/*-------------------------------------------------------------------
//	GRPMESH::SetNvertexData
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			21/09/2015 11:23:54
//	
//  @param				p : 
*/
/*-----------------------------------------------------------------*/
void GRPMESH::SetNvertexData(XDWORD p)
{ 
	this->nvertexdata=p;
}



/*-------------------------------------------------------------------
//	GRPMESH::GetVertexData
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			21/09/2015 11:24:01
//	
//	@return 			GRPVERTEX* : 
//
*/
/*-----------------------------------------------------------------*/
GRPVERTEX* GRPMESH::GetVertexData()
{ 
	return vertexdata;														
}

/*-------------------------------------------------------------------
//	GRPMESH::SetVertexData
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			21/09/2015 11:24:08
//	
//  @param				p : 
*/
/*-----------------------------------------------------------------*/
void GRPMESH::SetVertexData(GRPVERTEX* p)
{ 
	this->vertexdata=p; 													
}

/*-------------------------------------------------------------------
//	GRPMESH::SetIndexData
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			21/09/2015 11:24:15
//	
//  @param				p : 
*/
/*-----------------------------------------------------------------*/
void GRPMESH::SetIndexData(XWORD* p)
{ 
	this->indexdata=p; 														
}	

/*-------------------------------------------------------------------
//	GRPMESH::GetNindexData
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			21/09/2015 11:24:20
//	
//	@return 			XDWORD : 
//
*/
/*-----------------------------------------------------------------*/
XDWORD GRPMESH::GetNindexData()
{ 
	return nindexdata;														
}

/*-------------------------------------------------------------------
//	GRPMESH::SetNindexData
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			21/09/2015 11:24:26
//	
//  @param				p : 
*/
/*-----------------------------------------------------------------*/
void GRPMESH::SetNindexData(XDWORD p)
{ 
	this->nindexdata=p; 
}


/*-------------------------------------------------------------------
//	GRPMESH::GetNVertex
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			21/09/2015 11:14:15
//	
//	@return 			XDWORD : 
//
*/
/*-----------------------------------------------------------------*/
XDWORD GRPMESH::GetNVertex	()																							
{
	return nvertexdata;															
}


/*-------------------------------------------------------------------
//	GRPMESH::SetNVertex
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			21/09/2015 11:14:20
//	
//  @param				nvertex : 
*/
/*-----------------------------------------------------------------*/
void GRPMESH::SetNVertex		(int nvertex)																		
{
	this->nvertexdata=nvertex;												
}


/*-------------------------------------------------------------------
//	GRPMESH::GetIndexData
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			21/09/2015 11:14:57
//	
//	@return 			XWORD* : 
//
*/
/*-----------------------------------------------------------------*/
XWORD* GRPMESH::GetIndexData	()
{ 
	return this->indexdata;												
}


/*-------------------------------------------------------------------
//	GRPMESH::GetNIndex
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			21/09/2015 11:15:11
//	
//	@return 			XDWORD : 
//
*/
/*-----------------------------------------------------------------*/
XDWORD GRPMESH::GetNIndex		()
{
	return this->nindexdata;											
}

/*-------------------------------------------------------------------
//	GRPMESH::GetFaces
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			21/09/2015 11:16:27
//	
//	@return 			XVECTOR<GRPFACE*>* : 
//
*/
/*-----------------------------------------------------------------*/
XVECTOR<GRPFACE*>* GRPMESH::GetFaces	()
{ 
	return &faces;																
}


/*-------------------------------------------------------------------
//	GRPMESH::GetNVertexPerFace
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			21/09/2015 11:17:27
//	
//	@return 			int : 
//
*/
/*-----------------------------------------------------------------*/
int GRPMESH::GetNVertexPerFace()
{
	return nvertexperface;											
}


/*-------------------------------------------------------------------
//	GRPMESH::LoadModel
*/	
/**	
//	
//	Loads model from arrays
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			30/05/2014 13:19:29
//	
//  @param				programID : 
//  @param				nvertex : 
//  @param				vertexpositions : 
//  @param				vertexnormals : 
//  @param				vertextexturecoords : 
//  @param				Material : 
*/
/*-----------------------------------------------------------------*/
bool GRPMESH::LoadModel (int	nvertex, GLFLOAT*	vertexpositions, GLFLOAT*	vertexnormals, GLFLOAT*	vertextexturecoords)
{	
	this->nvertexdata=nvertex;
	
	if (this->vertexdata!=NULL) delete[] vertexdata;
	if (this->indexdata!=NULL)	delete[] indexdata;

	this->vertexdata=new GRPVERTEX[nvertexdata];
	if (this->vertexdata==NULL)
	{
		XDEBUG_PRINTCOLOR(4,__L("GRPELEMENT::LoadModel Not enought memory"));	
		return false;
	}

	this->indexdata=new XWORD[nvertexdata];
	if (this->indexdata==NULL)
	{
		XDEBUG_PRINTCOLOR(4,__L("GRPELEMENT::LoadModel Not enought memory"));	
		return false;
	}

	nindexdata	=	nvertexdata;
	//nvertexdata	=	nvertexdata; // not effect

	for (XDWORD e=0;e<nvertexdata;e++)
	{
		indexdata	[e]=e;

		vertexdata[e].position					[0]=vertexpositions			[e*3+0];
		vertexdata[e].position					[1]=vertexpositions			[e*3+1];
		vertexdata[e].position					[2]=vertexpositions			[e*3+2];

		vertexdata[e].normal						[0]=vertexnormals				[e*3+0];
		vertexdata[e].normal						[1]=vertexnormals				[e*3+1];
		vertexdata[e].normal						[2]=vertexnormals				[e*3+2];

		vertexdata[e].texturecoordinates[0]=vertextexturecoords	[e*3+0];
		vertexdata[e].texturecoordinates[1]=vertextexturecoords	[e*3+1];		
	}

	isloaded	=	false;

	return true;	
}





/*-------------------------------------------------------------------
//	GRPMESH::SetIsLoaded
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			21/09/2015 11:19:36
//	
//  @param				p : 
*/
/*-----------------------------------------------------------------*/
void GRPMESH::SetIsLoaded(bool p)																				
{ 
	this->isloaded=p;															
}	

/*-------------------------------------------------------------------
//	GRPMESH::SetMaxindex
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			21/09/2015 11:19:41
//	
//  @param				x : 
*/
/*-----------------------------------------------------------------*/
void GRPMESH::SetMaxindex(int x)																					
{ 
	this->maxindex=x;															
}


/*-------------------------------------------------------------------
//	GRPMESH::LoadModel
*/	
/**	
//	
//	Load Model from GRPFACE XVector
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			30/05/2014 13:19:44
//	
//	@return 			bool : 
//
//  @param				programID : 
//  @param				Faces : 
//  @param				Material : 
*/
/*-----------------------------------------------------------------*/
bool GRPMESH::LoadModel(XVECTOR<GRPFACE*>* _faces)
{			
	int nfaces=_faces->GetSize();
	faces.DeleteContents();
	faces.DeleteAll();
	for(int e=nfaces-1; e>=0; e--)
		{
			GRPFACE* newface = new GRPFACE(_faces->Get(e));	
			faces.Add(newface);		
			}

	SetFaceBuffers();	
	return true; 
}

/*-------------------------------------------------------------------
//	GRPMESH::GetNFaces
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			26/10/2015 11:33:03
//	
//	@return 			XWORD : 
//
*/
/*-----------------------------------------------------------------*/
XWORD	GRPMESH::GetNFaces()										
{ 
	return this->GetNIndex()/GetNVertexPerFace(); 
}



/*-------------------------------------------------------------------
//	GRPMESH::SetFaceBuffers
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			24/06/2014 9:36:00
//	
*/
/*-----------------------------------------------------------------*/
bool GRPMESH::SetFaceBuffers()
{
	XDWORD npfaces = faces.GetSize();	

	if (npfaces==0) 
		return false;

	if (indexdata!=NULL)
		delete[] indexdata;

	indexdata = new XWORD[npfaces*3];
	nindexdata = npfaces*3;

	if (indexdata==NULL)
	{
		XDEBUG_PRINTCOLOR(2,__L("GRPELEMENT::SetFaceBuffers : Critical error, not enough memory!"));
		return false;
	}

	if (vertexdata!=NULL)
		delete[] vertexdata;

	vertexdata = new GRPVERTEX[npfaces*3];
	nvertexdata = npfaces*3;

	if (vertexdata==NULL)
	{
		XDEBUG_PRINTCOLOR(2,__L("GRPELEMENT::SetFaceBuffers : Critical error, not enough memory!"));
		return false;
	}

	memset(vertexdata,0,sizeof(GRPVERTEX)*npfaces*3);


	XDWORD i=0;
			for (XDWORD e=0;e<npfaces;e++)
			{
				GRPFACE* face=faces.FastGet(e);

				for (int j=0;j<3;j++)
					{
						vertexdata[i].position[0]=face->vertices[j]->x;
						vertexdata[i].position[1]=face->vertices[j]->y;
						vertexdata[i].position[2]=face->vertices[j]->z;						
						
						if (face->normals)
						if (face->normals[j])
						{
						vertexdata[i].normal[0]=face->normals[j]->x;
						vertexdata[i].normal[1]=face->normals[j]->y;
						vertexdata[i].normal[2]=face->normals[j]->z;
						}
#ifdef GRPVERTEX_EXTENDED						
						if (face->binormals)
						if (face->binormals[j])
						{
						vertexdata[i].binormal[0]=face->binormals[j]->x;
						vertexdata[i].binormal[1]=face->binormals[j]->y;
						vertexdata[i].binormal[2]=face->binormals[j]->z;
						}

						if (face->tangents)
						if (face->tangents[j])
						{
						vertexdata[i].tangent[0]=face->tangents[j]->x;
						vertexdata[i].tangent[1]=face->tangents[j]->y;
						vertexdata[i].tangent[2]=face->tangents[j]->z;
						}
#endif
						if (face->texturecoordinates)
						if (face->texturecoordinates[j] != NULL)
						{
						vertexdata[i].texturecoordinates	[0]=face->texturecoordinates[j]->x;
						vertexdata[i].texturecoordinates	[1]=face->texturecoordinates[j]->y;
						}
				
						indexdata[i]=i;
						i++;
					}			
			}		

			this->maxindex=i;
	return true;
}

/*-------------------------------------------------------------------
//	GRPMESH::Merge
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			27/09/2016 11:31:29
//	
//  @param				mesh : 
*/
/*-----------------------------------------------------------------*/
void GRPMESH::Merge(GRPMESH* mesh)
{

}

/*-------------------------------------------------------------------
//	GRPMESH::Create
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/03/2015 16:50:30
//	
//	@return 			bool : 
//
//  @param				Vertexes : 
//  @param				Indexes : 
//  @param				Normales : 
*/
/*-----------------------------------------------------------------*/
bool GRPMESH::Create(XVECTOR<GLFLOAT>* vertexarray,XVECTOR<GLINT>* polygonarray,XVECTOR<GLFLOAT>* normalarray)
{
		
				XVECTOR<GRPPOINT*>	vertices;
				XVECTOR<GRPPOINT*>	normals;
				XVECTOR<GRPPOINT*>	tcoords;
		
		//------------------------------------------- load vertices
			if (vertexarray==NULL) 
				return false;

			vertices.SetAddInLimit(vertexarray->GetSize()/3);
			for (XDWORD e=0,i=0;e<vertexarray->GetSize();e+=3,i++)
			{
				GRPPOINT* p=NULL;

				double x=0.0f;
				double y=0.0f;
				double z=0.0f;

				x=vertexarray->Get(e);
				y=vertexarray->Get(e);
				z=vertexarray->Get(e);

				p=new GRPPOINT((float)x,(float)y,(float)z,i);
				
				if (p) 
					vertices.Add(p);				
			}

		//------------------------------------------- load normals
		
			if (normalarray==NULL) 
				return false;
			normals.SetAddInLimit(normalarray->GetSize()/3);

			for (XDWORD e=0,i=0;e<normalarray->GetSize();e+=3,i++)
			{
				GRPPOINT* p=NULL;
				double x=0.0f;
				double y=0.0f;
				double z=0.0f;

				x=normalarray->Get(e);
				y=normalarray->Get(e);
				z=normalarray->Get(e);

				p=new GRPPOINT((float)x,(float)y,(float)z,i);
				
				if (p) 
					normals.Add(p);						
			}

		//------------------------------------------ Create faces
		
			this->GetFaces()->SetAddInLimit(polygonarray->GetSize()/3);

			for (XDWORD e=0;e<polygonarray->GetSize();e+=3)
			{
				int a=(int)polygonarray->Get(e+0);
				int b=(int)polygonarray->Get(e+1);
				int c=(int)polygonarray->Get(e+2);
					 
				GRPFACE*	face=new GRPFACE();

									face->vertices[0]=new GRPPOINT(vertices.Get(a));
									face->vertices[1]=new GRPPOINT(vertices.Get(b));
									face->vertices[2]=new GRPPOINT(vertices.Get(c));

									face->normals[0]=new GRPPOINT(normals.Get(a));
									face->normals[1]=new GRPPOINT(normals.Get(b));
									face->normals[2]=new GRPPOINT(normals.Get(c));

									face->texturecoordinates[0]=new GRPPOINT(0.0,0.0);
									face->texturecoordinates[1]=new GRPPOINT(0.0,0.0);
									face->texturecoordinates[2]=new GRPPOINT(0.0,0.0);

									face->a=vertices.Get(a)->index;
									face->b=vertices.Get(b)->index;
									face->c=vertices.Get(c)->index;

				this->GetFaces()->Add(face);
			}
		
		this->SetFaceBuffers();

	return true;
}


/*-------------------------------------------------------------------
//	GRPMESH::PopulatePerVertexNormals
*/	
/**	
//	
//	Populates the perVertexNormals with the computation of common face normals
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			22/07/2014 14:27:02
//	
*/
/*-----------------------------------------------------------------*/
void GRPMESH::PopulateVertices ()
{
	
	XDWORD npvertex=  nvertexdata;
	XDWORD npfaces = faces.GetSize();
	XDWORD n=0;

	bool show=false;
		
	// #Imanol: changed order as v being XDWORD can't be smaller than 0
	// the change is not tested
	//for(XDWORD v=npvertex-1; v>=0; v--)
	for(XDWORD v = 0; v < npvertex; v++)
	{
		GRPVERTEX * vertex=&(vertexdata[v]);
		n=0;
		
		//for(XDWORD f=npfaces-1; f>=0; f--)
		for(XDWORD f= 0; f < npfaces; f++)
			{
				GRPFACE*	face=faces.Get(f);				

				if (face->a==v ||  face->b==v || face->c==v)
				{
					vertex->normal[0] += face->normal.vector[0];
					vertex->normal[1] += face->normal.vector[1];
					vertex->normal[2] += face->normal.vector[2];
					n++;
				}							
			}
		if (n!=0) 
		{
			vertex->normal[0] /=n;
			vertex->normal[1] /=n;
			vertex->normal[2] /=n;
		}

	}

	return;
}

	/*-------------------------------------------------------------------
//	GRPMESH::ReassignUVs
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
void GRPMESH::ReassignUVs()
{
	this->SetFaceBuffers();
	if (this->GetFaces()<=0) 
		return;

	XDWORD vsize		=this->GetFaces()->GetSize();
	GRPVERTEX* vdata=this->GetVertexData();

	for(XDWORD e=0; e<vsize; e++)
		{
			GRPFACE* face = this->GetFaces()->Get(e);

					vdata[face->a].texturecoordinates[0] = face->texturecoordinates[0]->x;
					vdata[face->a].texturecoordinates[1] = face->texturecoordinates[0]->y;
					vdata[face->b].texturecoordinates[0] = face->texturecoordinates[1]->x;
					vdata[face->b].texturecoordinates[1] = face->texturecoordinates[1]->y;
					vdata[face->c].texturecoordinates[0] = face->texturecoordinates[2]->x;
					vdata[face->c].texturecoordinates[1] = face->texturecoordinates[2]->y;	
			
		}			
	this->SetIsLoaded(false);
}

