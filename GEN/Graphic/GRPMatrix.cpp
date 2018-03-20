
/*------------------------------------------------------------------------------------------
//	GRPMATRIX.CPP
//	
//	Graphics Generic matrix Operations classs
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 13/05/2014 12:04:12
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


#if defined(LINUX) 
//#pragma GCC diagnostic ignored "-Wreturn-local-addr"
#endif

/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include <math.h>
#include "GRPMatrix.h"
#include "GRPProfiler.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/

GRPVECTOR	GRPMATRIX::result;
GRPVECTOR GRPMATRIX::Zero(0.0f,0.0f,0.0f);
GRPMATRIX GRPMATRIX::Identity(1);
GRPMATRIX GRPMATRIX::ortho = GRPMATRIX(1.0f, 0.0f,  0.0f,  0.0f,
																			 0.0f, 1.0f,  0.0f,  0.0f,
																			 0.0f, 0.0f, -1.0f, -3.0f,
																			 0.0f, 0.0f,  0.0f,  1.0f);

GRPMATRIX GRPMATRIXHOLDER::matrices[GRPMATRIX_MAX];

/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//	GRPMATRIX::GRPMATRIX
*/	
/**	
//	
//	Class Constructor GRPMATRIX
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			09/05/2014 12:48:09
//	
*/
/*-----------------------------------------------------------------*/
GRPMATRIX::GRPMATRIX()		
{  
	modified = false; 
}



/*-------------------------------------------------------------------
//	GRPMATRIX::~GRPMATRIX
*/	
/**	
//	
//	Class Destructor GRPMATRIX
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			09/05/2014 12:48:46
//	
*/
/*-----------------------------------------------------------------*/
GRPMATRIX::~GRPMATRIX()	
{
	
}



/*-------------------------------------------------------------------
//	GRPMATRIX::GetmatrixPointer
*/	
/**	
//	
//	return matrix as GLFLOAT array
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			09/05/2014 12:49:08
//	
//	@return 			GLFLOAT* : 
//
*/
/*-----------------------------------------------------------------*/
GLFLOAT* GRPMATRIX::GetMatrixPointer() 
{ 
	modified = false;

	return &(matrix[0][0]); 
}



/*-------------------------------------------------------------------
//	GRPMATRIX::LoadIdentity
*/	
/**	
//	
//	Loads identity on matrix
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			09/05/2014 12:49:54
//	
*/
/*-----------------------------------------------------------------*/

void GRPMATRIX::LoadIdentity()
{
	matrix[0][0] = 1.0f;
	matrix[0][1] = 0.0f;
	matrix[0][2] = 0.0f;
	matrix[0][3] = 0.0f;

	matrix[1][0] = 0.0f;
	matrix[1][1] = 1.0f;
	matrix[1][2] = 0.0f;
	matrix[1][3] = 0.0f;

	matrix[2][0] = 0.0f;
	matrix[2][1] = 0.0f;
	matrix[2][2] = 1.0f;
	matrix[2][3] = 0.0f;

	matrix[3][0] = 0.0f;
	matrix[3][1] = 0.0f;
	matrix[3][2] = 0.0f;
	matrix[3][3] = 1.0f;

	modified=true;
	}

/*-------------------------------------------------------------------
//	GRPMATRIX::Inverse
*/	
/**	
//	
//	Inverses matrix
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			11/09/2014 17:03:17
//	
*/
/*-----------------------------------------------------------------*/
	void	GRPMATRIX::Inverse()
{
/*		
		GLFLOAT m					[16];
		GLFLOAT inv				[16];
		GLFLOAT inverse		[16];

		CopyTo(&m);


    inv[0] = m[5]*m[10]*m[15]-m[5]*m[11]*m[14]-m[9]*m[6]*m[15]+
             m[9]*m[7]*m[14]+m[13]*m[6]*m[11]-m[13]*m[7]*m[10];

    inv[4] = -m[4]*m[10]*m[15]+m[4]*m[11]*m[14]+m[8]*m[6]*m[15]-
             m[8]*m[7]*m[14]-m[12]*m[6]*m[11]+m[12]*m[7]*m[10];

    inv[8] = m[4]*m[9]*m[15]-m[4]*m[11]*m[13]-m[8]*m[5]*m[15]+
             m[8]*m[7]*m[13]+m[12]*m[5]*m[11]-m[12]*m[7]*m[9];

    inv[12]= -m[4]*m[9]*m[14]+m[4]*m[10]*m[13]+m[8]*m[5]*m[14]-
             m[8]*m[6]*m[13]-m[12]*m[5]*m[10]+m[12]*m[6]*m[9];

    inv[1] = -m[1]*m[10]*m[15]+m[1]*m[11]*m[14]+m[9]*m[2]*m[15]-
             m[9]*m[3]*m[14]-m[13]*m[2]*m[11]+m[13]*m[3]*m[10];

    inv[5] = m[0]*m[10]*m[15]-m[0]*m[11]*m[14]-m[8]*m[2]*m[15]+
             m[8]*m[3]*m[14]+m[12]*m[2]*m[11]-m[12]*m[3]*m[10];

    inv[9] = -m[0]*m[9]*m[15]+m[0]*m[11]*m[13]+m[8]*m[1]*m[15]-
             m[8]*m[3]*m[13]-m[12]*m[1]*m[11]+m[12]*m[3]*m[9];

    inv[13]= m[0]*m[9]*m[14]-m[0]*m[10]*m[13]-m[8]*m[1]*m[14]+
             m[8]*m[2]*m[13]+m[12]*m[1]*m[10]-m[12]*m[2]*m[9];

    inv[2] = m[1]*m[6]*m[15]-m[1]*m[7]*m[14]-m[5]*m[2]*m[15]+
             m[5]*m[3]*m[14]+m[13]*m[2]*m[7]-m[13]*m[3]*m[6];

    inv[6] = -m[0]*m[6]*m[15]+m[0]*m[7]*m[14]+m[4]*m[2]*m[15]-
             m[4]*m[3]*m[14]-m[12]*m[2]*m[7]+m[12]*m[3]*m[6];

    inv[10]= m[0]*m[5]*m[15]-m[0]*m[7]*m[13]-m[4]*m[1]*m[15]+
             m[4]*m[3]*m[13]+m[12]*m[1]*m[7]-m[12]*m[3]*m[5];

    inv[14]= -m[0]*m[5]*m[14]+m[0]*m[6]*m[13]+m[4]*m[1]*m[14]-
             m[4]*m[2]*m[13]-m[12]*m[1]*m[6]+m[12]*m[2]*m[5];

    inv[3] = -m[1]*m[6]*m[11]+m[1]*m[7]*m[10]+m[5]*m[2]*m[11]-
             m[5]*m[3]*m[10]-m[9]*m[2]*m[7]+m[9]*m[3]*m[6];

    inv[7] = m[0]*m[6]*m[11]-m[0]*m[7]*m[10]-m[4]*m[2]*m[11]+
             m[4]*m[3]*m[10]+m[8]*m[2]*m[7]-m[8]*m[3]*m[6];

    inv[11]= -m[0]*m[5]*m[11]+m[0]*m[7]*m[9]+m[4]*m[1]*m[11]-
             m[4]*m[3]*m[9]-m[8]*m[1]*m[7]+m[8]*m[3]*m[5];

    inv[15]= m[0]*m[5]*m[10]-m[0]*m[6]*m[9]-m[4]*m[1]*m[10]+
             m[4]*m[2]*m[9]+m[8]*m[1]*m[6]-m[8]*m[2]*m[5];


    GLFLOAT det = m[0]*inv[0]+m[1]*inv[4]+m[2]*inv[8]+m[3]*inv[12];

    if (det == 0) 
			return;

    det = 1.0f / det;

    for (XDWORD i = 0; i < 16; i++) 
			inverse[i] = inv[i] * det;

		this->BuildMatrixFromFloatArray(inverse);
		*/

		float invOut[16];
	  float inv[16], det;
    int i;

		float m[16];
		this->CopyTo(&m);

		

    inv[0] = m[5]  * m[10] * m[15] - 
             m[5]  * m[11] * m[14] - 
             m[9]  * m[6]  * m[15] + 
             m[9]  * m[7]  * m[14] +
             m[13] * m[6]  * m[11] - 
             m[13] * m[7]  * m[10];

    inv[4] = -m[4]  * m[10] * m[15] + 
              m[4]  * m[11] * m[14] + 
              m[8]  * m[6]  * m[15] - 
              m[8]  * m[7]  * m[14] - 
              m[12] * m[6]  * m[11] + 
              m[12] * m[7]  * m[10];

    inv[8] = m[4]  * m[9] * m[15] - 
             m[4]  * m[11] * m[13] - 
             m[8]  * m[5] * m[15] + 
             m[8]  * m[7] * m[13] + 
             m[12] * m[5] * m[11] - 
             m[12] * m[7] * m[9];

    inv[12] = -m[4]  * m[9] * m[14] + 
               m[4]  * m[10] * m[13] +
               m[8]  * m[5] * m[14] - 
               m[8]  * m[6] * m[13] - 
               m[12] * m[5] * m[10] + 
               m[12] * m[6] * m[9];

    inv[1] = -m[1]  * m[10] * m[15] + 
              m[1]  * m[11] * m[14] + 
              m[9]  * m[2] * m[15] - 
              m[9]  * m[3] * m[14] - 
              m[13] * m[2] * m[11] + 
              m[13] * m[3] * m[10];

    inv[5] = m[0]  * m[10] * m[15] - 
             m[0]  * m[11] * m[14] - 
             m[8]  * m[2] * m[15] + 
             m[8]  * m[3] * m[14] + 
             m[12] * m[2] * m[11] - 
             m[12] * m[3] * m[10];

    inv[9] = -m[0]  * m[9] * m[15] + 
              m[0]  * m[11] * m[13] + 
              m[8]  * m[1] * m[15] - 
              m[8]  * m[3] * m[13] - 
              m[12] * m[1] * m[11] + 
              m[12] * m[3] * m[9];

    inv[13] = m[0]  * m[9] * m[14] - 
              m[0]  * m[10] * m[13] - 
              m[8]  * m[1] * m[14] + 
              m[8]  * m[2] * m[13] + 
              m[12] * m[1] * m[10] - 
              m[12] * m[2] * m[9];

    inv[2] = m[1]  * m[6] * m[15] - 
             m[1]  * m[7] * m[14] - 
             m[5]  * m[2] * m[15] + 
             m[5]  * m[3] * m[14] + 
             m[13] * m[2] * m[7] - 
             m[13] * m[3] * m[6];

    inv[6] = -m[0]  * m[6] * m[15] + 
              m[0]  * m[7] * m[14] + 
              m[4]  * m[2] * m[15] - 
              m[4]  * m[3] * m[14] - 
              m[12] * m[2] * m[7] + 
              m[12] * m[3] * m[6];

    inv[10] = m[0]  * m[5] * m[15] - 
              m[0]  * m[7] * m[13] - 
              m[4]  * m[1] * m[15] + 
              m[4]  * m[3] * m[13] + 
              m[12] * m[1] * m[7] - 
              m[12] * m[3] * m[5];

    inv[14] = -m[0]  * m[5] * m[14] + 
               m[0]  * m[6] * m[13] + 
               m[4]  * m[1] * m[14] - 
               m[4]  * m[2] * m[13] - 
               m[12] * m[1] * m[6] + 
               m[12] * m[2] * m[5];

    inv[3] = -m[1] * m[6] * m[11] + 
              m[1] * m[7] * m[10] + 
              m[5] * m[2] * m[11] - 
              m[5] * m[3] * m[10] - 
              m[9] * m[2] * m[7] + 
              m[9] * m[3] * m[6];

    inv[7] = m[0] * m[6] * m[11] - 
             m[0] * m[7] * m[10] - 
             m[4] * m[2] * m[11] + 
             m[4] * m[3] * m[10] + 
             m[8] * m[2] * m[7] - 
             m[8] * m[3] * m[6];

    inv[11] = -m[0] * m[5] * m[11] + 
               m[0] * m[7] * m[9] + 
               m[4] * m[1] * m[11] - 
               m[4] * m[3] * m[9] - 
               m[8] * m[1] * m[7] + 
               m[8] * m[3] * m[5];

    inv[15] = m[0] * m[5] * m[10] - 
              m[0] * m[6] * m[9] - 
              m[4] * m[1] * m[10] + 
              m[4] * m[2] * m[9] + 
              m[8] * m[1] * m[6] - 
              m[8] * m[2] * m[5];

    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

    if (det == 0)
        return;

    det = 1.0f / det;

    for (i = 0; i < 16; i++)
        invOut[i] = inv[i] * det;
    
			for (int y=0;y<4;y++)
				for (int x=0;x<4;x++)
					this->matrix[x][y]=invOut[(y*4)+x];

    return;
 }

void GRPMATRIX::Inverse3x3()
{
			GLfloat m[4][4];

			CopyTo(&m);

			GLfloat r[3][3];

    GLfloat d =
         m[0][0]*m[1][1]*m[2][2] +
         m[0][1]*m[1][2]*m[2][0] +
         m[0][2]*m[1][0]*m[2][1] +
        -m[0][0]*m[1][2]*m[2][1] +
        -m[0][1]*m[1][0]*m[2][2] +
        -m[0][2]*m[1][1]*m[2][0];

    (r)[0][0] = (m[1][1]*m[2][2]-m[1][2]*m[2][1]) / d;
    (r)[0][1] = (m[0][2]*m[2][1]-m[0][1]*m[2][2]) / d;
    (r)[0][2] = (m[0][1]*m[1][2]-m[0][2]*m[1][1]) / d;

    (r)[1][0] = (m[1][2]*m[2][0]-m[1][0]*m[2][2]) / d;
    (r)[1][1] = (m[0][0]*m[2][2]-m[0][2]*m[2][0]) / d;
    (r)[1][2] = (m[0][2]*m[1][0]-m[0][0]*m[1][2]) / d;

    (r)[2][0] = (m[1][0]*m[2][1]-m[1][1]*m[2][0]) / d;
    (r)[2][1] = (m[0][1]*m[2][0]-m[0][0]*m[2][1]) / d;
    (r)[2][2] = (m[0][0]*m[1][1]-m[0][1]*m[1][0]) / d;

		this->BuildMatrixFromFloatArray3x3(r);
}
/*-------------------------------------------------------------------
//	GRPMATRIX::Transpose
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			15/09/2014 17:53:42
//	
*/
/*-----------------------------------------------------------------*/
void	GRPMATRIX::Transpose()
{
	GRPMATRIX t;

	/*
	for (int j=0;j<4;j++)
	for (int i=0;i<4;i++)
		t.matrix[j][i]=this->matrix[i][j];
	*/
	t.matrix[0][0]=this->matrix[0][0];
	t.matrix[1][1]=this->matrix[1][1];
	t.matrix[2][2]=this->matrix[2][2];
	t.matrix[3][3]=this->matrix[3][3];

	t.matrix[0][1]=this->matrix[1][0];
	t.matrix[0][2]=this->matrix[2][0];
	t.matrix[0][3]=this->matrix[3][0];
	
	t.matrix[1][0]=this->matrix[0][1];
	t.matrix[1][2]=this->matrix[2][1];
	t.matrix[1][3]=this->matrix[3][1];
	
	t.matrix[2][0]=this->matrix[0][2];
	t.matrix[2][1]=this->matrix[1][2];	
	t.matrix[2][3]=this->matrix[3][2];
	
	t.matrix[3][0]=this->matrix[0][3];
	t.matrix[3][1]=this->matrix[1][3];	
	t.matrix[3][2]=this->matrix[2][3];

	*this=t;	
}

void	GRPMATRIX::Transpose3x3()
{
	
	GLFLOAT r[3][3];
	
	for (int j=0;j<3;j++)
	for (int i=0;i<3;i++)	
		(r)[j][i]		=this->matrix[i][j];	
	
	this->BuildMatrixFromFloatArray3x3(r);
	
}



GRPMATRIX*	GRPMATRIX::BuildDirectionMatrix(GRPVECTOR* obj,GRPVECTOR* eye)
{
	GRPVECTOR ev;
	GRPVECTOR z;
	GRPVECTOR x_tmp;
	GRPVECTOR x;
	GRPVECTOR y;

	GRPVECTOR up(0.0f,1.0f,0.0f);

	ev = *eye - *obj;

	ev.Normalize();
	z=ev;

	x_tmp.CrossProduct(&up,&z);				
	x_tmp.Normalize();
	x=x_tmp;

	y.CrossProduct(&z,&x);

	this->matrix[0][0] = x.vector[0];
	this->matrix[0][1] = y.vector[0];
	this->matrix[0][2] = z.vector[0];
	this->matrix[0][3] = 0.0f;

	this->matrix[1][0] = x.vector[1];
	this->matrix[1][1] = y.vector[1];
	this->matrix[1][2] = z.vector[1];
	this->matrix[1][3] = 0.0f;

	this->matrix[2][0] = x.vector[2];
	this->matrix[2][1] = y.vector[2];
	this->matrix[2][2] = z.vector[2];
	this->matrix[2][3] = 0.0f;

	this->matrix[3][0] = -x.vector[0] * eye->vector[0] + -x.vector[1] * eye->vector[1] + -x.vector[2] * eye->vector[2];
	this->matrix[3][1] = -y.vector[0] * eye->vector[0] + -y.vector[1] * eye->vector[1] + -y.vector[2] * eye->vector[2];
	this->matrix[3][2] = -z.vector[0] * eye->vector[0] + -z.vector[1] * eye->vector[1] + -z.vector[2] * eye->vector[2];
	this->matrix[3][3] = 1.0f;

	//GRPMATRIX Translate;
	//Translate.BuildTranslationMatrix	(-obj->vector[0],-obj->vector[1],-obj->vector[2]);	
		//				GetMultiplicationMatrix		(this,&Translate);

	return this;
}
GRPMATRIX*	GRPMATRIX::BuildQuaternion(GRPVECTOR* obj,GRPVECTOR* eye)
{
	/*
	GRPVECTOR ev;
	GRPVECTOR z;
	GRPVECTOR x_tmp;
	GRPVECTOR x;
	GRPVECTOR y;

	GRPVECTOR up(0.0f,1.0f,0.0f);

	ev = *eye - *obj;

	ev.Normalize();
	z=ev;

	x_tmp.CrossProduct(&up,&z);				
	x_tmp.Normalize();
	x=x_tmp;

	y.CrossProduct(&z,&x);

	this->matrix[0][0] = x.vector[0];
	this->matrix[0][1] = y.vector[0];
	this->matrix[0][2] = z.vector[0];
	this->matrix[0][3] = 0.0f;

	this->matrix[1][0] = x.vector[1];
	this->matrix[1][1] = y.vector[1];
	this->matrix[1][2] = z.vector[1];
	this->matrix[1][3] = 0.0f;

	this->matrix[2][0] = x.vector[2];
	this->matrix[2][1] = y.vector[2];
	this->matrix[2][2] = z.vector[2];
	this->matrix[2][3] = 0.0f;

	this->matrix[3][0] = -x.vector[0] * eye->vector[0] + -x.vector[1] * eye->vector[1] + -x.vector[2] * eye->vector[2];
	this->matrix[3][1] = -y.vector[0] * eye->vector[0] + -y.vector[1] * eye->vector[1] + -y.vector[2] * eye->vector[2];
	this->matrix[3][2] = -z.vector[0] * eye->vector[0] + -z.vector[1] * eye->vector[1] + -z.vector[2] * eye->vector[2];
	this->matrix[3][3] = 1.0f;

	GRPMATRIX Translate;
	Translate.BuildTranslationMatrix	(-obj->vector[0],-obj->vector[1],-obj->vector[2]);	
						GetMultiplicationMatrix		(this,&Translate);
*/
	return this;
}

/*-------------------------------------------------------------------
//	GRPMATRIX::BuildmatrixFromFloatArray
*/	
/**	
//	
//	Gets matrix data from a [4][4] float array
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			09/05/2014 12:50:13
//	
//	@return 			GRPMATRIX : this after modification
//
//  @param				_matrix : m(float)[4,4] new matrix data 
*/
/*-----------------------------------------------------------------*/
GRPMATRIX* GRPMATRIX::BuildMatrixFromFloatArray( GLFLOAT* _matrix)
{
	matrix[0][0] = _matrix[0];
	matrix[0][1] = _matrix[1];
	matrix[0][2] = _matrix[2];
	matrix[0][3] = _matrix[3];

	matrix[1][0] = _matrix[4];
	matrix[1][1] = _matrix[5];
	matrix[1][2] = _matrix[6];
	matrix[1][3] = _matrix[7];

	matrix[2][0] = _matrix[8];
	matrix[2][1] = _matrix[9];
	matrix[2][2] = _matrix[10];
	matrix[2][3] = _matrix[11];

	matrix[3][0] = _matrix[12];
	matrix[3][1] = _matrix[13];
	matrix[3][2] = _matrix[14];
	matrix[3][3] = _matrix[15];

	return this;
}
		
GRPMATRIX*	GRPMATRIX::BuildMatrixFromFloatArray3x3		(GLFLOAT* _matrix)
{
	matrix[0][0] = _matrix[0];
	matrix[0][1] = _matrix[1];
	matrix[0][2] = _matrix[2];

	matrix[1][0] = _matrix[3];
	matrix[1][1] = _matrix[4];
	matrix[1][2] = _matrix[5];

	matrix[2][0] = _matrix[6];
	matrix[2][1] = _matrix[7];
	matrix[2][2] = _matrix[8];

	matrix[3][0] = _matrix[9];
	matrix[3][1] = _matrix[10];
	matrix[3][2] = _matrix[11];

	return this;
}

GRPMATRIX*	GRPMATRIX::BuildMatrixFromFloatArray3x3		(GLFLOAT _matrix[3][3])
{
	//memset(this->matrix,0,sizeof(GLFLOAT)*4*4);
	matrix[0][0] = _matrix[0][0];
	matrix[0][1] = _matrix[0][1];
	matrix[0][2] = _matrix[0][2];
	matrix[0][3] = 0;

	matrix[1][0] = _matrix[1][0];
	matrix[1][1] = _matrix[1][1];
	matrix[1][2] = _matrix[1][2];
	matrix[1][3] = 0;

	matrix[2][0] = _matrix[2][0];
	matrix[2][1] = _matrix[2][1];
	matrix[2][2] = _matrix[2][2];
	matrix[2][3] = 0;
	
	matrix[3][0] = 0;
	matrix[3][1] = 0;
	matrix[3][2] = 0;
	matrix[3][3] = 0;

	return this;
}

/*-------------------------------------------------------------------
//	GRPMATRIX::BuildOrthoProjectionMatrix
*/	
/**	
//	
//	Builds a Ortho metric Projection matrix
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			09/05/2014 12:51:21
//	
//	@return 			GRPMATRIX* : Proyection matrix for Ortho perspective MVP multiply
//
//  @param				left		: clipping left plane x value
//  @param				right		: clipping right plane x value
//  @param				bottom	: clipping bottom plane y value
//  @param				top			: cliping top plane y value
//  @param				znear		: near plane z value (MUST BE POSITIVE)
//  @param				zfar		: far plane z value 
*/
/*-----------------------------------------------------------------*/
GRPMATRIX* GRPMATRIX::BuildOrthoProjectionMatrix(GLFLOAT left,GLFLOAT right,GLFLOAT bottom,GLFLOAT top,GLFLOAT znear,GLFLOAT zfar)
{
	GLFLOAT const sx = 2.0f / (right - left);
	GLFLOAT const sy = 2.0f / (top - bottom);
	GLFLOAT const sz = -2.0f / (zfar - znear);

	GLFLOAT const tx = -(right + left) / (right - left);
	GLFLOAT const ty = -(top + bottom) / (top - bottom);
	GLFLOAT const tz = -(zfar + znear) / (zfar - znear);

	this->matrix[0][0] = sx;
	this->matrix[0][1] = 0.0f;
	this->matrix[0][2] = 0.0f;
	this->matrix[0][3] = 0.0f;

	this->matrix[1][0] = 0.0f;
	this->matrix[1][1] = sy;
	this->matrix[1][2] = 0.0f;
	this->matrix[1][3] = 0.0f;

	this->matrix[2][0] = 0.0f;
	this->matrix[2][1] = 0.0f;
	this->matrix[2][2] = sz;
	this->matrix[2][3] = 0.0f;

	this->matrix[3][0] = tx;
	this->matrix[3][1] = ty;
	this->matrix[3][2] = tz;
	this->matrix[3][3] = 1.0f;

	modified					 = true;

	return this;
}



/*-------------------------------------------------------------------
//	GRPMATRIX::BuildFrustumProjectionmatrix
*/	
/**	
//	
//	Builds a Frustum Projection matrix
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			09/05/2014 12:51:21
//	
//	@return 			GRPMATRIX* : Proyection matrix for Frustum perspective MVP multiply
//
//  @param				left : clipping left plane x value
//  @param				right : clipping right plane x value
//  @param				bottom : clipping bottom plane y value
//  @param				top : cliping top plane y value
//  @param				znear : near plane z value (MUST BE POSITIVE)
//  @param				zfar : far plane z value 
*/
/*-----------------------------------------------------------------*/

GRPMATRIX* GRPMATRIX::BuildFrustumProjectionMatrix	(GLFLOAT left, GLFLOAT right, GLFLOAT bottom, GLFLOAT top, GLFLOAT znear, GLFLOAT zfar)
{

	GLFLOAT const m00 = znear * 2.0f / (right - left);
	GLFLOAT const m11 = znear * 2.0f / (top - bottom);
	GLFLOAT const m22 = -(zfar + znear) / (zfar - znear);

	GLFLOAT const m20 = (right + left) / (right - left);
	GLFLOAT const m21 = (top + bottom) / (top - bottom);

	GLFLOAT const m32 = -(2.0f * zfar * znear) / (zfar - znear);
	GLFLOAT const m23 = -1.0f;



	matrix[0][0] = m00;
	matrix[0][1] = 0.0f;
	matrix[0][2] = 0.0f;
	matrix[0][3] = 0.0f;

	matrix[1][0] = 0.0f;
	matrix[1][1] = m11;
	matrix[1][2] = 0.0f;
	matrix[1][3] = 0.0f;

	matrix[2][0] = m20;
	matrix[2][1] = m21;
	matrix[2][2] = m22;
	matrix[2][3] = m23;

	matrix[3][0] = 0.0f;
	matrix[3][1] = 0.0f;
	matrix[3][2] = m32;
	matrix[3][3] = 0.0f;

	modified=true;
	return this;
}


GRPMATRIX* GRPMATRIX::BuildPixelProjectionMatrix	(GLFLOAT left, GLFLOAT right, GLFLOAT bottom, GLFLOAT top, GLFLOAT znear, GLFLOAT zfar)
{

	GLFLOAT const sx = 2.0f / (right - left);
	GLFLOAT const sy = 2.0f / (top - bottom);
	GLFLOAT const sz = -2.0f / (zfar - znear);

	GLFLOAT const tx = -(right + left) / (right - left);
	GLFLOAT const ty = -(top + bottom) / (top - bottom);
	GLFLOAT const tz = -(zfar + znear) / (zfar - znear);

	this->matrix[0][0] = sx;
	this->matrix[0][1] = 0.0f;
	this->matrix[0][2] = 0.0f;
	this->matrix[0][3] = 0.0f;

	this->matrix[1][0] = 0.0f;
	this->matrix[1][1] = -sy;
	this->matrix[1][2] = 0.0f;
	this->matrix[1][3] = 0.0f;

	this->matrix[2][0] = 0.0f;
	this->matrix[2][1] = 0.0f;
	this->matrix[2][2] = sz;
	this->matrix[2][3] = 0.0f;

	this->matrix[3][0] = tx;
	this->matrix[3][1] = -ty;
	this->matrix[3][2] = tz;
	this->matrix[3][3] = 1.0f;

	modified					 = true;
	return this;

}




/*-------------------------------------------------------------------
//	GRPMATRIX::BuildScalematrix
*/	
/**	
//	
//	Build Scale matrix by values
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			09/05/2014 12:54:57
//	
//	@return 			GRPMATRIX* : Scale matrix for Model matrix Multiply
//
//  @param				x : x axis scale
//  @param				y : y axis scale
//  @param				z : z axis scale
*/
/*-----------------------------------------------------------------*/
GRPMATRIX* GRPMATRIX::BuildScaleMatrix(GLFLOAT x, GLFLOAT y, GLFLOAT z)
{
	GLFLOAT r[3];

	r[0]=x;
	r[1]=y;
	r[2]=z;

	BuildScaleMatrix(r);

	modified=true;
	return this;
}




/*-------------------------------------------------------------------
//	GRPMATRIX::BuildScalematrix
*/	
/**	
//	
//	Builds Scale matrix from GLFLOAT array
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			09/05/2014 12:56:03
//	
//	@return 			 GRPMATRIX*:  this
//
//  @param				m : GLFLOAT array [x,y,z] 
*/
/*-----------------------------------------------------------------*/
GRPMATRIX* GRPMATRIX::BuildScaleMatrix(const GLFLOAT* m)
{
	matrix[0][0] = m[0];
	matrix[0][1] = 0.0f;
	matrix[0][2] = 0.0f;
	matrix[0][3] = 0.0f;

	matrix[1][0] = 0.0f;
	matrix[1][1] = m[1];
	matrix[1][2] = 0.0f;
	matrix[1][3] = 0.0f;

	matrix[2][0] = 0.0f;
	matrix[2][1] = 0.0f;
	matrix[2][2] = m[2];
	matrix[2][3] = 0.0f;

	matrix[3][0] = 0.0f;
	matrix[3][1] = 0.0f;
	matrix[3][2] = 0.0f;
	matrix[3][3] = 1.0f;

	modified=true;
	return this;
}



/*-------------------------------------------------------------------
//	GRPMATRIX::BuildRotationmatrix
*/	
/**	
//	
//	Creates Rotational matrix for Model Multiply
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			09/05/2014 12:57:23
//	
//	@return 			GRPMATRIX* : 
//
//  @param				degrees : value of degrees to rotate the selected axis
//  @param				x : x axis factor
//  @param				y : y axis factor
//  @param				z : z axis factor
*/
/*-----------------------------------------------------------------*/
void GRPMATRIX::BuildRotationMatrix(GLFLOAT degrees, GLFLOAT x, GLFLOAT y, GLFLOAT z)
{
	GLFLOAT axis[3];

	axis[0]=x;
	axis[1]=y;
	axis[2]=z;

	 BuildRotationMatrix(degrees,axis);				
}


/*-------------------------------------------------------------------
//	GRPMATRIX::BuildRotationFromQuat
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			01/06/2016 10:14:18
//	
//  @param				axis[4] : 
*/
/*-----------------------------------------------------------------*/
void GRPMATRIX::BuildRotationFromQuat(GLFLOAT w, GLFLOAT x, GLFLOAT y, GLFLOAT z)
{
	//http://www.cprogramming.com/tutorial/3d/quaternions.html
	
	GLFLOAT w2=w*w;
	GLFLOAT x2=x*x;
	GLFLOAT y2=y*y;
	GLFLOAT z2=z*z;

	GLFLOAT _2x2=2*x2;
	GLFLOAT _2y2=2*y2;
	GLFLOAT _2z2=2*z2;
	
	GLFLOAT _2xy=2*x*y;
	GLFLOAT _2xz=2*x*z;
	GLFLOAT _2yz=2*y*z;
	GLFLOAT _2wx=2*w*x;
	GLFLOAT _2wy=2*w*y;
	GLFLOAT _2wz=2*w*z;


	matrix[0][0]=1-_2y2-_2z2;
	matrix[0][1]=_2xy-_2wz;
	matrix[0][2]=_2xz+_2wy;
	matrix[0][3]=0.0f;

	matrix[1][0]=_2xy+_2wz;
	matrix[1][1]=1-_2x2-_2z2;
	matrix[1][2]=_2yz+_2wx;
	matrix[1][3]=0.0f;

	matrix[2][0]=_2xz+_2wy;
	matrix[2][1]=_2yz-_2wx;
	matrix[2][2]=1-_2x2-_2y2;
	matrix[2][3]=0.0f;

	matrix[3][0]=0.0f;
	matrix[3][1]=0.0f;
	matrix[3][2]=0.0f;
	matrix[3][3]=1.0f;

}




/*-------------------------------------------------------------------
//	GRPMATRIX::BuildScalePivotMatrix
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			27/02/2015 11:30:31
//	
//	@return 			GRPMATRIX* : 
//
//  @param				x : 
//  @param				y : 
//  @param				z : 
*/
/*-----------------------------------------------------------------*/
GRPMATRIX* GRPMATRIX::BuildPivotMatrix								(GLFLOAT spx,GLFLOAT spy,GLFLOAT spz)
{
	//	 Sp = |  1    0    0    0 |     
  //        |  0    1    0    0 |     
  //        |  0    0    1    0 |     
  //        | spx  spy  spz   1 |

	matrix[0][0] = 1.0f;
	matrix[0][1] = 0.0f;
	matrix[0][2] = 0.0f;
	matrix[0][3] = 0.0f;

	matrix[1][0] = 0.0f;
	matrix[1][1] = 1.0f;
	matrix[1][2] = 0.0f;
	matrix[1][3] = 0.0f;

	matrix[2][0] = 0.0f;
	matrix[2][1] = 0.0f;
	matrix[2][2] = 1.0f;
	matrix[2][3] = 0.0f;

	matrix[3][0] = spx;
	matrix[3][1] = spy;
	matrix[3][2] = spz;
	matrix[3][3] = 1.0f;

		modified=true;
	return this;
}

/*-------------------------------------------------------------------
//	GRPMATRIX::BuildScaleShearMatrix
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			27/02/2015 11:30:55
//	
//	@return 			GRPMATRIX* : 
//
//  @param				x : 
//  @param				y : 
//  @param				z : 
*/
/*-----------------------------------------------------------------*/
GRPMATRIX* GRPMATRIX::BuildShearMatrix								(GLFLOAT x,GLFLOAT y,GLFLOAT z)
{
	//     Sh = |  1    0    0    0 |
  //		      | shxy  1    0    0 |
  //          | shxz shyz  1    0 |
  //			    |  0    0    0    1 |


	GLFLOAT shxy=x*y;
	GLFLOAT shxz=x*z;
	GLFLOAT shyz=y*z;

	matrix[0][0] = 1.0f;
	matrix[0][1] = 0.0f;
	matrix[0][2] = 0.0f;
	matrix[0][3] = 0.0f;

	matrix[1][0] = shxy;
	matrix[1][1] = 1.0f;
	matrix[1][2] = 0.0f;
	matrix[1][3] = 0.0f;

	matrix[2][0] = shxz;
	matrix[2][1] = shyz;
	matrix[2][2] = 1.0f;
	matrix[2][3] = 0.0f;

	matrix[3][0] = 0.0f;
	matrix[3][1] = 0.0f;
	matrix[3][2] = 0.0f;
	matrix[3][3] = 1.0f;

	modified=true;
	return this;
}
/*-------------------------------------------------------------------
//	GRPMATRIX::BuildScalePivotTranslationMatrix
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			27/02/2015 11:31:04
//	
//	@return 			GRPMATRIX* : 
//
//  @param				x : 
//  @param				y : 
//  @param				z : 
*/
/*-----------------------------------------------------------------*/
GRPMATRIX* GRPMATRIX::BuildScalePivotTranslationMatrix		(GLFLOAT x,GLFLOAT y,GLFLOAT z)
{
		modified=true;
	return this;
}


/*-------------------------------------------------------------------
//	GRPMATRIX::BuildTranslationmatrix
*/	
/**	
//	
//	Builds Translation matrix form Position vector
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			09/05/2014 12:58:46
//	
//	@return 			GRPMATRIX* : this
//
//  @param				v : GRPVECTOR with new Spatial Position
*/
/*-----------------------------------------------------------------*/
void GRPMATRIX::BuildTranslationMatrix(GRPVECTOR vector)
{
	 BuildTranslationMatrix(vector.vector[0], vector.vector[1], vector.vector[2]);
}



/*-------------------------------------------------------------------
//	GRPMATRIX::BuildTranslationmatrix
*/	
/**	
//	
//	Builds Translation matrix from GLFLOAT array
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			09/05/2014 12:59:23
//	
//	@return 			GRPMATRIX* : this 
//
//  @param				v[3] :GLFLOAT array with new Spatial Position
*/
/*-----------------------------------------------------------------*/
void GRPMATRIX::BuildTranslationMatrix(GLFLOAT vector[3])
{
 BuildTranslationMatrix(vector[0], vector[1], vector[2]);
}



/*-------------------------------------------------------------------
//	GRPMATRIX::BuildTranslationmatrix
*/	
/**	
//	
//	Builds Translation matrix by Values
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			09/05/2014 13:00:11
//	
//	@return 			GRPMATRIX* : this
//
//  @param				x : new x plane position
//  @param				y : new y plane position 
//  @param				z : new z plane position
*/
/*-----------------------------------------------------------------*/
void GRPMATRIX::BuildTranslationMatrix(GLFLOAT x, GLFLOAT y, GLFLOAT z)
{
	matrix[0][0] = 1.0f;
	matrix[0][1] = 0.0f;
	matrix[0][2] = 0.0f;
	matrix[0][3] = 0.0f;

	matrix[1][0] = 0.0f;
	matrix[1][1] = 1.0f;
	matrix[1][2] = 0.0f;
	matrix[1][3] = 0.0f;
	
	matrix[2][0] = 0.0f;
	matrix[2][1] = 0.0f;
	matrix[2][2] = 1.0f;
	matrix[2][3] = 0.0f;

	matrix[3][0] = x;
	matrix[3][1] = y;
	matrix[3][2] = z;
	matrix[3][3] = 1.0f;

	//modified		 = true;

}



/*-------------------------------------------------------------------
//	GRPMATRIX::BuildRotationmatrix
*/	
/**	
//	
//	Builds Rotational matrix by Degrees, on selected axis factor
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			09/05/2014 13:01:23
//	
//	@return 			GRPMATRIX* : this
//
//  @param				degrees : GLFLOAT degrees
//  @param				axis[3] : axis factor GLFLOAT array
*/
/*-----------------------------------------------------------------*/
void GRPMATRIX::BuildRotationMatrix(GLFLOAT degrees, const GLFLOAT axis[3])
{
	// build a quat first 
	GLFLOAT radians=degrees*GRPMATRIX_DEG_TO_RAD;

	GLFLOAT i, j, k, r;

	// should be 
	GLFLOAT dst_l = sinf(radians/2.0f);

	// actually is 
	GLFLOAT const src_l = sqrtf(axis[0] * axis[0] +
															axis[1] * axis[1] +
															axis[2] * axis[2]);

	if(src_l <= GRPMATRIX_FLT_EPSILON) 
		{
			i = 0.0f;
			j = 0.0f;
			k = 0.0f;
			r = 1.0f;
		} 
	 else 
	  {
			GLFLOAT const s = dst_l / src_l;

			i = axis[0] * s;
			j = axis[1] * s;
			k = axis[2] * s;
			r = cosf(radians/2.0f);
		}

	// build a matrix from the quat 

	matrix[0][0] = 1.0f - 2.0f * (j * j + k * k);
	matrix[0][1] = 2.0f * (i * j + r * k);
	matrix[0][2] = 2.0f * (i * k - r * j);
	matrix[0][3] = 0.0f;

	matrix[1][0] = 2.0f * (i * j - r * k);
	matrix[1][1] = 1.0f - 2.0f * (i * i + k * k);
	matrix[1][2] = 2.0f * (j * k + r * i);
	matrix[1][3] = 0.0f;

	matrix[2][0] = 2.0f * (i * k + r * j);
	matrix[2][1] = 2.0f * (j * k - r * i);
	matrix[2][2] = 1.0f - 2.0f * (i * i + j * j);
	matrix[2][3] = 0.0f;

	matrix[3][0] = 0.0f;
	matrix[3][1] = 0.0f;
	matrix[3][2] = 0.0f;
	matrix[3][3] = 1.0f;

}

void GRPMATRIX::BuildRotationMatrixQ(GLFLOAT i,GLFLOAT j,GLFLOAT k,GLFLOAT r)
{
	// build a matrix from the quat 

	matrix[0][0] = 1.0f - 2.0f * (j * j + k * k);
	matrix[0][1] = 2.0f * (i * j + r * k);
	matrix[0][2] = 2.0f * (i * k - r * j);
	matrix[0][3] = 0.0f;

	matrix[1][0] = 2.0f * (i * j - r * k);
	matrix[1][1] = 1.0f - 2.0f * (i * i + k * k);
	matrix[1][2] = 2.0f * (j * k + r * i);
	matrix[1][3] = 0.0f;

	matrix[2][0] = 2.0f * (i * k + r * j);
	matrix[2][1] = 2.0f * (j * k - r * i);
	matrix[2][2] = 1.0f - 2.0f * (i * i + j * j);
	matrix[2][3] = 0.0f;

	matrix[3][0] = 0.0f;
	matrix[3][1] = 0.0f;
	matrix[3][2] = 0.0f;
	matrix[3][3] = 1.0f;


}

void GRPMATRIX::CalculateQ( GRPPOINT& q ) 
{
  float trace = matrix[0][0] + matrix[1][1] + matrix[2][2]; // I removed + 1.0f; see discussion with Ethan
  if( trace > 0 ) {// I changed M_EPSILON to 0
    float s = 0.5f / sqrtf(trace+ 1.0f);
    q.w = 0.25f / s;
    q.x = ( matrix[2][1] - matrix[1][2] ) * s;
    q.y = ( matrix[0][2] - matrix[2][0] ) * s;
    q.z = ( matrix[1][0] - matrix[0][1] ) * s;
  } else {
    if ( matrix[0][0] > matrix[1][1] && matrix[0][0] > matrix[2][2] ) {
      float s = 2.0f * sqrtf( 1.0f + matrix[0][0] - matrix[1][1] - matrix[2][2]);
      q.w = (matrix[2][1] - matrix[1][2] ) / s;
      q.x = 0.25f * s;
      q.y = (matrix[0][1] + matrix[1][0] ) / s;
      q.z = (matrix[0][2] + matrix[2][0] ) / s;
    } else if (matrix[1][1] > matrix[2][2]) {
      float s = 2.0f * sqrtf( 1.0f + matrix[1][1] - matrix[0][0] - matrix[2][2]);
      q.w = (matrix[0][2] - matrix[2][0] ) / s;
      q.x = (matrix[0][1] + matrix[1][0] ) / s;
      q.y = 0.25f * s;
      q.z = (matrix[1][2] + matrix[2][1] ) / s;
    } else {
      float s = 2.0f * sqrtf( 1.0f + matrix[2][2] - matrix[0][0] - matrix[1][1] );
      q.w = (matrix[1][0] - matrix[0][1] ) / s;
      q.x = (matrix[0][2] + matrix[2][0] ) / s;
      q.y = (matrix[1][2] + matrix[2][1] ) / s;
      q.z = 0.25f * s;
    }
  }
}


void GRPMATRIX::fastBuildRotationMatrix(const GLFLOAT rot[3],int RotationAxisOrder)
{
		float radiansx=GRPMATRIX_DEG_TO_RAD*(rot[0]);
		float radiansy=GRPMATRIX_DEG_TO_RAD*(rot[1]);
		float radiansz=GRPMATRIX_DEG_TO_RAD*(rot[2]);

	  GLfloat const sina = sinf(radiansx);
    GLfloat const cosa = cosf(radiansx);

	  GLfloat const sinb = sinf(radiansy);
    GLfloat const cosb = cosf(radiansy);

		GLfloat const siny = sinf(radiansz);
    GLfloat const cosy = cosf(radiansz);

		if (RotationAxisOrder==GRPMATRIX_ROTATION_ORDER_XYZ)
		{
			matrix[0][0]=cosb*cosy;
			matrix[0][1]=-cosb*siny;
			matrix[0][2]=sinb;
			matrix[0][3]=0;

			matrix[1][0]=cosa*siny + sina*sinb*cosy;
			matrix[1][1]=cosa*cosy - sina*sinb*siny;
			matrix[1][2]=-sina*cosb;
			matrix[1][3]=0;

			matrix[2][0]=sina*siny - cosa*sinb*cosy;
			matrix[2][1]=sina*cosy + cosa*sinb*siny;
			matrix[2][2]=cosa*cosb;
			matrix[2][3]=0;

			matrix[3][0]=0;
			matrix[3][1]=0;
			matrix[3][2]=0;
			matrix[3][3]=1;			
		}
		else
			if (RotationAxisOrder==GRPMATRIX_ROTATION_ORDER_ZYX)
		{
			
			matrix[0][0]=cosb*cosy;
			matrix[0][1]=cosy*sina*sinb - cosa*siny;
			matrix[0][2]=cosa*cosy*sinb + sina*siny;
			matrix[0][3]=0;

			matrix[1][0]=cosb*siny;
			matrix[1][1]=cosa*cosy + sina*sinb*siny;
			matrix[1][2]=-cosy*sina + cosa*sinb*siny;
			matrix[1][3]=0;

			matrix[2][0]=-sinb;
			matrix[2][1]=cosb*sina;
			matrix[2][2]=cosa*cosb;
			matrix[2][3]=0;

			matrix[3][0]=0;
			matrix[3][1]=0;
			matrix[3][2]=0;
			matrix[3][3]=1;			
			
		
		}
		
}

/*-------------------------------------------------------------------
//	GRPMATRIX::BuildRotationMatrix
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			12/05/2015 15:44:10
//	
//  @param				rot[3] : 
//  @param				RotationAxisOrder : 
*/
/*-----------------------------------------------------------------*/
void GRPMATRIX::BuildRotationMatrix(const GLFLOAT rot[3],int RotationAxisOrder)
{
	//return fastBuildRotationMatrix(rot,RotationAxisOrder);
	
	GRPMATRIX rotx;
	GRPMATRIX roty;
	GRPMATRIX rotz;
	GRPMATRIX rotm;

	rotx.BuildRotationMatrixX(rot[0]);
	roty.BuildRotationMatrixY(rot[1]);	
	rotz.BuildRotationMatrixZ(rot[2]);
	
	//ZYX
	rotm.GetMultiplicationMatrix(rotz,roty);
	this->GetMultiplicationMatrix(rotm,rotx);

	/*
	switch(RotationAxisOrder)
	{	
		case GRPMATRIX_ROTATION_ORDER_XZY:																
																(*this)=rotx*rotz;
																(*this)=(*this)*roty;
																break;
		case GRPMATRIX_ROTATION_ORDER_YXZ:
																(*this)=roty*rotx;
																(*this)=(*this)*rotz;
																break;
		case GRPMATRIX_ROTATION_ORDER_YZX:
																(*this)=roty*rotz;
																(*this)=(*this)*rotx;
																break;
		case GRPMATRIX_ROTATION_ORDER_ZXY:
																(*this)=rotz*rotx;
																(*this)=(*this)*roty;
																break;
		case GRPMATRIX_ROTATION_ORDER_ZYX:
																rotm.GetMultiplicationMatrix(rotz,roty);
																this->GetMultiplicationMatrix(rotm,rotx);
																break;
		default:
		case GRPMATRIX_ROTATION_ORDER_XYZ:
																(*this)=rotx*roty;
																(*this)=(*this)*rotz;
																break;
	}
	*/


}

/*-------------------------------------------------------------------
//	GRPMATRIX::BuildRotationFromQuat
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			22/01/2015 15:53:18
//	
//	@return 			GRPMATRIX* : 
//
//  @param				axis[4] : 
*/
/*-----------------------------------------------------------------*/
/*
void GRPMATRIX::BuildRotationFromQuat(const GLFLOAT axis[4])
{
		// build a matrix from the quat 
	GLFLOAT i, j, k, r;
	i=axis[0];
	j=axis[1];
	k=axis[2];
	r=axis[3];

	matrix[0][0] = 1.0f - 2.0f * (j * j + k * k);
	matrix[0][1] = 2.0f * (i * j + r * k);
	matrix[0][2] = 2.0f * (i * k - r * j);
	matrix[0][3] = 0.0f;

	matrix[1][0] = 2.0f * (i * j - r * k);
	matrix[1][1] = 1.0f - 2.0f * (i * i + k * k);
	matrix[1][2] = 2.0f * (j * k + r * i);
	matrix[1][3] = 0.0f;

	matrix[2][0] = 2.0f * (i * k + r * j);
	matrix[2][1] = 2.0f * (j * k - r * i);
	matrix[2][2] = 1.0f - 2.0f * (i * i + j * j);
	matrix[2][3] = 0.0f;

	matrix[3][0] = 0.0f;
	matrix[3][1] = 0.0f;
	matrix[3][2] = 0.0f;
	matrix[3][3] = 1.0f;
	
}
*/

/*-------------------------------------------------------------------
//	GRPMATRIX::BuildRotationMatrixX
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			22/01/2015 15:53:25
//	
//	@return 			GRPMATRIX* : 
//
//  @param				degrees : 
*/
/*-----------------------------------------------------------------*/
GRPMATRIX* GRPMATRIX::BuildRotationMatrixX(GLFLOAT degrees)
{
		float radians=GRPMATRIX_DEG_TO_RAD*degrees;

	  GLfloat const s = sinf(radians);
    GLfloat const c = cosf(radians);

    matrix[0][0] = 1.0f;
    matrix[0][1] = 0.0f;
    matrix[0][2] = 0.0f;
    matrix[0][3] = 0.0f;

    matrix[1][0] = 0.0f;
    matrix[1][1] = c;
    matrix[1][2] = s;
    matrix[1][3] = 0.0f;

    matrix[2][0] = 0.0f;
    matrix[2][1] = -s;
    matrix[2][2] = c;
    matrix[2][3] = 0.0f;

    matrix[3][0] = 0.0f;
    matrix[3][1] = 0.0f;
    matrix[3][2] = 0.0f;
    matrix[3][3] = 1.0f;

	return this;
}
/*-------------------------------------------------------------------
//	GRPMATRIX::BuildRotationMatrixY
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			22/01/2015 15:53:31
//	
//	@return 			GRPMATRIX* : 
//
//  @param				degrees : 
*/
/*-----------------------------------------------------------------*/
GRPMATRIX* GRPMATRIX::BuildRotationMatrixY(GLFLOAT degrees)
{
	float radians=GRPMATRIX_DEG_TO_RAD*degrees;


	  GLfloat const s = sinf(radians);
    GLfloat const c = cosf(radians);

    matrix[0][0] = c;
    matrix[0][1] = 0.0f;
    matrix[0][2] = -s;
    matrix[0][3] = 0.0f;

    matrix[1][0] = 0.0f;
    matrix[1][1] = 1.0f;
    matrix[1][2] = 0.0f;
    matrix[1][3] = 0.0f;

    matrix[2][0] = s;
    matrix[2][1] = 0.0f;
    matrix[2][2] = c;
    matrix[2][3] = 0.0f;

    matrix[3][0] = 0.0f;
    matrix[3][1] = 0.0f;
    matrix[3][2] = 0.0f;
    matrix[3][3] = 1.0f;

	return this;

/*-------------------------------------------------------------------
//	GRPMATRIX::BuildRotationMatrixZ
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			22/01/2015 15:53:36
//	
//	@return 			GRPMATRIX* : 
//
//  @param				degrees : 
*/
/*-----------------------------------------------------------------*/}
GRPMATRIX* GRPMATRIX::BuildRotationMatrixZ(GLFLOAT degrees)
{
		float radians=GRPMATRIX_DEG_TO_RAD*degrees;

	  GLfloat const s = sinf(radians);
    GLfloat const c = cosf(radians);

    matrix[0][0] = c;
    matrix[0][1] = s;
    matrix[0][2] = 0.0f;
    matrix[0][3] = 0.0f;

    matrix[1][0] = -s;
    matrix[1][1] = c;
    matrix[1][2] = 0.0f;
    matrix[1][3] = 0.0f;

    matrix[2][0] = 0.0f;
    matrix[2][1] = 0.0f;
    matrix[2][2] = 1.0f;
    matrix[2][3] = 0.0f;

    matrix[3][0] = 0.0f;
    matrix[3][1] = 0.0f;
    matrix[3][2] = 0.0f;
    matrix[3][3] = 1.0f;

	return this;
}

/*-------------------------------------------------------------------
//	GRPMATRIX::operator=
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			13/05/2014 12:49:50
//	
//	@return 			GRPMATRIX& : 
//
//  @param				m[4][4] : 
*/
/*-----------------------------------------------------------------*/
const GRPMATRIX GRPMATRIX::operator=(GLFLOAT _matrix[4][4])
{			
	/*
	matrix[0][0] = _matrix[0][0];
	matrix[0][1] = _matrix[0][1];
	matrix[0][2] = _matrix[0][2];
	matrix[0][3] = _matrix[0][3];

	matrix[1][0] = _matrix[1][0];
	matrix[1][1] = _matrix[1][1];
	matrix[1][2] = _matrix[1][2];
	matrix[1][3] = _matrix[1][3];

	matrix[2][0] = _matrix[2][0];
	matrix[2][1] = _matrix[2][1];
	matrix[2][2] = _matrix[2][2];
	matrix[2][3] = _matrix[2][3];

	matrix[3][0] = _matrix[3][0];
	matrix[3][1] = _matrix[3][1];
	matrix[3][2] = _matrix[3][2];
	matrix[3][3] = _matrix[3][3];
	*/

	memcpy(&(matrix[0][0]),&(_matrix[0][0]),16*sizeof(GLFLOAT));	
	return (*this);
}
		


/*-------------------------------------------------------------------
//	GRPMATRIX::operator*
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			13/05/2014 12:50:07
//	
//	@return 			GRPMATRIX& : 
//
//  @param				matrix : 
*/
/*-----------------------------------------------------------------*/
const GRPMATRIX GRPMATRIX::operator*(GRPMATRIX& matrix)
{   
	GRPMATRIX tmp;
	tmp.GetMultiplicationMatrix(*this,matrix);
	return tmp;
}



/*-------------------------------------------------------------------
//	GRPMATRIX::operator*
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			23/08/2014 19:51:29
//	
//	@return 			const : 
//
//  @param				b : 
*/
/*-----------------------------------------------------------------*/
const GRPMATRIX	GRPMATRIX::operator*(const GRPMATRIX& b)
{
	GRPMATRIX tmp;
	
	tmp.GetMultiplicationMatrix(this,(GRPMATRIX*)&b);

	return tmp;
}



/*-------------------------------------------------------------------
//	GRPMATRIX::operator=
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			13/05/2014 12:46:14
//	
//	@return 			GRPMATRIX& : 
//
//  @param				m : 
*/
/*-----------------------------------------------------------------*/
const GRPMATRIX GRPMATRIX::operator=(const GRPMATRIX& m)
{
	
	matrix[0][0] = m.matrix[0][0];
	matrix[0][1] = m.matrix[0][1];
	matrix[0][2] = m.matrix[0][2];
	matrix[0][3] = m.matrix[0][3];

	matrix[1][0] = m.matrix[1][0];
	matrix[1][1] = m.matrix[1][1];
	matrix[1][2] = m.matrix[1][2];
	matrix[1][3] = m.matrix[1][3];

	matrix[2][0] = m.matrix[2][0];
	matrix[2][1] = m.matrix[2][1];
	matrix[2][2] = m.matrix[2][2];
	matrix[2][3] = m.matrix[2][3];

	matrix[3][0] = m.matrix[3][0];
	matrix[3][1] = m.matrix[3][1];
	matrix[3][2] = m.matrix[3][2];
	matrix[3][3] = m.matrix[3][3];
	
	//memcpy(matrix,m.matrix,4*4*sizeof(GLFLOAT));
	modified = true;

	return (*this);
}



/*-------------------------------------------------------------------
//	GRPMATRIX::operator*
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			13/05/2014 12:46:00
//	
//	@return 			GRPVECTOR& : 
//
//  @param				v : 
*/
/*-----------------------------------------------------------------*/
const GRPVECTOR GRPMATRIX::operator*(GRPVECTOR v)
{
	
	GRPVECTOR result=this->TransformPoint(v);

	return result;
}



/*-------------------------------------------------------------------
//	GRPMATRIX::Extract3x3
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			13/05/2014 12:49:02
//	
//  @param				r[3][3] : 
*/
/*-----------------------------------------------------------------*/
void GRPMATRIX::Extract3x3NormalMatrix(GLFLOAT (*r)[3][3])
{
	(*r)[0][0] = matrix[0][0];
	(*r)[0][1] = matrix[0][1];
	(*r)[0][2] = matrix[0][2];

	(*r)[1][0] = matrix[1][0];
	(*r)[1][1] = matrix[1][1];
	(*r)[1][2] = matrix[1][2];

	(*r)[2][0] = matrix[2][0];
	(*r)[2][1] = matrix[2][1];
	(*r)[2][2] = matrix[2][2];
	
}


void	GRPMATRIX::GetNormalMatrix									(GLFLOAT (*r)[3][3])
{
	GRPMATRIX i;
	i=*this;
	i.Inverse3x3();
	i.Transpose3x3();	
	i.Extract3x3NormalMatrix(r);	
}

void	GRPMATRIX::GetNormalMatrix									(GLFLOAT (*r)[4][4])
{
	/*
	GRPMATRIX inverse;
	GRPMATRIX transpose;

	transpose=*this;
	transpose.Transpose();

	inverse=transpose;
	inverse.Inverse();

	inverse.CopyTo(r);
	*/

	(*this).Transpose();
	(*this).Inverse();
	(*this).CopyTo(r);
}




/*-------------------------------------------------------------------
//	GRPMATRIX::TransformPoint
*/	
/**	
//	
//	Transform Point by matrix
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			09/05/2014 13:04:04
//	
//	@return 			GRPVECTOR : new Spatial vector after transform
//
//  @param				v : Spatial vector to transform
*/
/*-----------------------------------------------------------------*/
GRPVECTOR GRPMATRIX::TransformPoint(GRPVECTOR v)
{	
	/*
	GLFLOAT inv_w = 1.0f / (this->matrix[0][3]*v.vector[0] + this->matrix[1][3]*v.vector[1] + this->matrix[2][3]*v.vector[2] + this->matrix[3][3]);

	result.vector[0] = this->matrix[0][0] * v.vector[0] + this->matrix[1][0] * v.vector[1] + this->matrix[2][0] * v.vector[2] + this->matrix[3][0] * inv_w;
	result.vector[1] = this->matrix[0][1] * v.vector[0] + this->matrix[1][1] * v.vector[1] + this->matrix[2][1] * v.vector[2] + this->matrix[3][1] * inv_w;
	result.vector[2] = this->matrix[0][2] * v.vector[0] + this->matrix[1][2] * v.vector[1] + this->matrix[2][2] * v.vector[2] + this->matrix[3][2] * inv_w;
*/
	GLFLOAT v0=v.vector[0];
	GLFLOAT v1=v.vector[1];
	GLFLOAT v2=v.vector[2];

	GLFLOAT inv_w = 1.0f / (this->matrix[0][3]*v0 + this->matrix[1][3]*v1 + this->matrix[2][3]*v2 + this->matrix[3][3]);

	GLFLOAT m00=this->matrix[0][0];
	GLFLOAT m01=this->matrix[0][1];
	GLFLOAT m02=this->matrix[0][2];
	GLFLOAT m03=this->matrix[0][3];

	GLFLOAT m10=this->matrix[1][0];	
	GLFLOAT m11=this->matrix[1][1];
	GLFLOAT m12=this->matrix[1][2];
	GLFLOAT m13=this->matrix[1][3];

	GLFLOAT m20=this->matrix[2][0];	
	GLFLOAT m21=this->matrix[2][1];
	GLFLOAT m22=this->matrix[2][2];
	GLFLOAT m23=this->matrix[2][3];

	GLFLOAT m30=this->matrix[3][0];
	GLFLOAT m31=this->matrix[3][1];
	GLFLOAT m32=this->matrix[3][2];
	GLFLOAT m33=this->matrix[3][3];

	/*
	result.vector[0] = this->matrix[0][0] * v0 + this->matrix[1][0] * v1 + this->matrix[2][0] * v2 + this->matrix[3][0] * inv_w;
	result.vector[1] = this->matrix[0][1] * v0 + this->matrix[1][1] * v1 + this->matrix[2][1] * v2 + this->matrix[3][1] * inv_w;
	result.vector[2] = this->matrix[0][2] * v0 + this->matrix[1][2] * v1 + this->matrix[2][2] * v2 + this->matrix[3][2] * inv_w;
	*/

	result.vector[0] = m00 * v0 + m10 * v1 + m20 * v2 + m30 * inv_w;
	result.vector[1] = m01 * v0 + m11 * v1 + m21 * v2 + m31 * inv_w;
	result.vector[2] = m02 * v0 + m12 * v1 + m22 * v2 + m32 * inv_w;
	
	return result;
}		



/*-------------------------------------------------------------------
//	GRPMATRIX::Transformvector
*/	
/**	
//	
//	Transform Normalized vector
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			09/05/2014 13:04:54
//	
//	@return 			GRPVECTOR : new Spatial position
//
//  @param				v : Normalized GRPVECTOR
*/
/*-----------------------------------------------------------------*/
GRPVECTOR GRPMATRIX::TransformVector(GRPVECTOR v)
{
	result.vector[0] = this->matrix[0][0] * v.vector[0] + this->matrix[1][0] * v.vector[1] + this->matrix[2][0] * v.vector[2];
	result.vector[1] = this->matrix[0][1] * v.vector[0] + this->matrix[1][1] * v.vector[1] + this->matrix[2][1] * v.vector[2];
	result.vector[2] = this->matrix[0][2] * v.vector[0] + this->matrix[1][2] * v.vector[1] + this->matrix[2][2] * v.vector[2];

	return result;
}

void GRPMATRIX::GetPosition(GRPPOINT* v)
{
	v->x=this->matrix[0][3];
	v->y=this->matrix[1][3];
	v->z=this->matrix[2][3];
}
/*-------------------------------------------------------------------
//	GRPMATRIX::GetMulplicationtMatrix
*/	
/**	
//	
//	Gets the matrix result from M(4,4) * M(4,4)
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			09/05/2014 13:02:46
//	
//	@return 			GRPMATRIX* : this M(4,4) * M(4,4)
//
//  @param				a : M(4,4) GRPMATRIX* homogenous matrix
//  @param				b : M(4,4) GRPMATRIX* homogenous matrix
*/
/*-----------------------------------------------------------------*/
void GRPMATRIX::GetMultiplicationMatrix(  const  GRPMATRIX* __restrict a, const GRPMATRIX* __restrict b)
{			
	//GRPPROFILER::nMatrixOperations++;

	const float a00=a->matrix[0][0];
	const float a01=a->matrix[0][1];
	const float a02=a->matrix[0][2];
	const float a03=a->matrix[0][3];

	const float a10=a->matrix[1][0];
	const float a11=a->matrix[1][1];
	const float a12=a->matrix[1][2];
	const float a13=a->matrix[1][3];

	const float a20=a->matrix[2][0];
	const float a21=a->matrix[2][1];
	const float a22=a->matrix[2][2];
	const float a23=a->matrix[2][3];

	const float a30=a->matrix[3][0];
	const float a31=a->matrix[3][1];
	const float a32=a->matrix[3][2];
	const float a33=a->matrix[3][3];

	const float b00=b->matrix[0][0];
	const float b01=b->matrix[0][1];
	const float b02=b->matrix[0][2];
	const float b03=b->matrix[0][3];

	const float b10=b->matrix[1][0];
	const float b11=b->matrix[1][1];
	const float b12=b->matrix[1][2];
	const float b13=b->matrix[1][3];

	const float b20=b->matrix[2][0];
	const float b21=b->matrix[2][1];
	const float b22=b->matrix[2][2];
	const float b23=b->matrix[2][3];

	const float b30=b->matrix[3][0];
	const float b31=b->matrix[3][1];
	const float b32=b->matrix[3][2];
	const float b33=b->matrix[3][3];

	matrix[0][0] = a00*b00+a10*b01+a20*b02+a30*b03;
	matrix[0][1] = a01*b00+a11*b01+a21*b02+a31*b03;
	matrix[0][2] = a02*b00+a12*b01+a22*b02+a32*b03;
	matrix[0][3] = a03*b00+a13*b01+a23*b02+a33*b03;

	matrix[1][0] = a00*b10+a10*b11+a20*b12+a30*b13;
	matrix[1][1] = a01*b10+a11*b11+a21*b12+a31*b13;
	matrix[1][2] = a02*b10+a12*b11+a22*b12+a32*b13;
	matrix[1][3] = a03*b10+a13*b11+a23*b12+a33*b13;

	matrix[2][0] = a00*b20+a10*b21+a20*b22+a30*b23;
	matrix[2][1] = a01*b20+a11*b21+a21*b22+a31*b23;
	matrix[2][2] = a02*b20+a12*b21+a22*b22+a32*b23;
	matrix[2][3] = a03*b20+a13*b21+a23*b22+a33*b23;

	matrix[3][0] = a00*b30+a10*b31+a20*b32+a30*b33;
	matrix[3][1] = a01*b30+a11*b31+a21*b32+a31*b33;
	matrix[3][2] = a02*b30+a12*b31+a22*b32+a32*b33;
	matrix[3][3] = a03*b30+a13*b31+a23*b32+a33*b33;

	/*
	matrix[0][0] = a.matrix[0][0]*b.matrix[0][0]+a.matrix[1][0]*b.matrix[0][1]+a.matrix[2][0]*b.matrix[0][2]+a.matrix[3][0]*b.matrix[0][3];
	matrix[0][1] = a.matrix[0][1]*b.matrix[0][0]+a.matrix[1][1]*b.matrix[0][1]+a.matrix[2][1]*b.matrix[0][2]+a.matrix[3][1]*b.matrix[0][3];
	matrix[0][2] = a.matrix[0][2]*b.matrix[0][0]+a.matrix[1][2]*b.matrix[0][1]+a.matrix[2][2]*b.matrix[0][2]+a.matrix[3][2]*b.matrix[0][3];
	matrix[0][3] = a.matrix[0][3]*b.matrix[0][0]+a.matrix[1][3]*b.matrix[0][1]+a.matrix[2][3]*b.matrix[0][2]+a.matrix[3][3]*b.matrix[0][3];

	matrix[1][0] = a.matrix[0][0]*b.matrix[1][0]+a.matrix[1][0]*b.matrix[1][1]+a.matrix[2][0]*b.matrix[1][2]+a.matrix[3][0]*b.matrix[1][3];
	matrix[1][1] = a.matrix[0][1]*b.matrix[1][0]+a.matrix[1][1]*b.matrix[1][1]+a.matrix[2][1]*b.matrix[1][2]+a.matrix[3][1]*b.matrix[1][3];
	matrix[1][2] = a.matrix[0][2]*b.matrix[1][0]+a.matrix[1][2]*b.matrix[1][1]+a.matrix[2][2]*b.matrix[1][2]+a.matrix[3][2]*b.matrix[1][3];
	matrix[1][3] = a.matrix[0][3]*b.matrix[1][0]+a.matrix[1][3]*b.matrix[1][1]+a.matrix[2][3]*b.matrix[1][2]+a.matrix[3][3]*b.matrix[1][3];

	matrix[2][0] = a.matrix[0][0]*b.matrix[2][0]+a.matrix[1][0]*b.matrix[2][1]+a.matrix[2][0]*b.matrix[2][2]+a.matrix[3][0]*b.matrix[2][3];
	matrix[2][1] = a.matrix[0][1]*b.matrix[2][0]+a.matrix[1][1]*b.matrix[2][1]+a.matrix[2][1]*b.matrix[2][2]+a.matrix[3][1]*b.matrix[2][3];
	matrix[2][2] = a.matrix[0][2]*b.matrix[2][0]+a.matrix[1][2]*b.matrix[2][1]+a.matrix[2][2]*b.matrix[2][2]+a.matrix[3][2]*b.matrix[2][3];
	matrix[2][3] = a.matrix[0][3]*b.matrix[2][0]+a.matrix[1][3]*b.matrix[2][1]+a.matrix[2][3]*b.matrix[2][2]+a.matrix[3][3]*b.matrix[2][3];

	matrix[3][0] = a.matrix[0][0]*b.matrix[3][0]+a.matrix[1][0]*b.matrix[3][1]+a.matrix[2][0]*b.matrix[3][2]+a.matrix[3][0]*b.matrix[3][3];
	matrix[3][1] = a.matrix[0][1]*b.matrix[3][0]+a.matrix[1][1]*b.matrix[3][1]+a.matrix[2][1]*b.matrix[3][2]+a.matrix[3][1]*b.matrix[3][3];
	matrix[3][2] = a.matrix[0][2]*b.matrix[3][0]+a.matrix[1][2]*b.matrix[3][1]+a.matrix[2][2]*b.matrix[3][2]+a.matrix[3][2]*b.matrix[3][3];
	matrix[3][3] = a.matrix[0][3]*b.matrix[3][0]+a.matrix[1][3]*b.matrix[3][1]+a.matrix[2][3]*b.matrix[3][2]+a.matrix[3][3]*b.matrix[3][3];
	return this;
	*/
	//return this;
}



/*-------------------------------------------------------------------
//	GRPMATRIX::GetMulplicationtMatrix
*/	
/**	
//	
//	Gets the matrix result from M(4,4) * M(4,4)
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			09/05/2014 13:02:46
//	
//	@return 			GRPMATRIX* : this M(4,4) * M(4,4)
//
//  @param				a : M(4,4) GRPMATRIX* homogenous matrix
//  @param				b : M(4,4) GRPMATRIX* homogenous matrix
*/
/*-----------------------------------------------------------------*/
void GRPMATRIX::GetMultiplicationMatrix(const GRPMATRIX&  a, const  GRPMATRIX& b)
{			
	//GRPPROFILER::nMatrixOperations++;

	const float a00=a.matrix[0][0];
	const float a01=a.matrix[0][1];
	const float a02=a.matrix[0][2];
	const float a03=a.matrix[0][3];

	const float a10=a.matrix[1][0];
	const float a11=a.matrix[1][1];
	const float a12=a.matrix[1][2];
	const float a13=a.matrix[1][3];

	const float a20=a.matrix[2][0];
	const float a21=a.matrix[2][1];
	const float a22=a.matrix[2][2];
	const float a23=a.matrix[2][3];

	const float a30=a.matrix[3][0];
	const float a31=a.matrix[3][1];
	const float a32=a.matrix[3][2];
	const float a33=a.matrix[3][3];

	const float b00=b.matrix[0][0];
	const float b01=b.matrix[0][1];
	const float b02=b.matrix[0][2];
	const float b03=b.matrix[0][3];

	const float b10=b.matrix[1][0];
	const float b11=b.matrix[1][1];
	const float b12=b.matrix[1][2];
	const float b13=b.matrix[1][3];

	const float b20=b.matrix[2][0];
	const float b21=b.matrix[2][1];
	const float b22=b.matrix[2][2];
	const float b23=b.matrix[2][3];

	const float b30=b.matrix[3][0];
	const float b31=b.matrix[3][1];
	const float b32=b.matrix[3][2];
	const float b33=b.matrix[3][3];
	
	matrix[0][0] = a00*b00+a10*b01+a20*b02+a30*b03;
	matrix[0][1] = a01*b00+a11*b01+a21*b02+a31*b03;
	matrix[0][2] = a02*b00+a12*b01+a22*b02+a32*b03;
	matrix[0][3] = a03*b00+a13*b01+a23*b02+a33*b03;

	matrix[1][0] = a00*b10+a10*b11+a20*b12+a30*b13;
	matrix[1][1] = a01*b10+a11*b11+a21*b12+a31*b13;
	matrix[1][2] = a02*b10+a12*b11+a22*b12+a32*b13;
	matrix[1][3] = a03*b10+a13*b11+a23*b12+a33*b13;

	matrix[2][0] = a00*b20+a10*b21+a20*b22+a30*b23;
	matrix[2][1] = a01*b20+a11*b21+a21*b22+a31*b23;
	matrix[2][2] = a02*b20+a12*b21+a22*b22+a32*b23;
	matrix[2][3] = a03*b20+a13*b21+a23*b22+a33*b23;

	matrix[3][0] = a00*b30+a10*b31+a20*b32+a30*b33;
	matrix[3][1] = a01*b30+a11*b31+a21*b32+a31*b33;
	matrix[3][2] = a02*b30+a12*b31+a22*b32+a32*b33;
	matrix[3][3] = a03*b30+a13*b31+a23*b32+a33*b33;
	
	modified=true;

	/*
	float m[][] ={  {a00*b00+a10*b01+a20*b02+a30*b03, a01*b00+a11*b01+a21*b02+a31*b03, a02*b00+a12*b01+a22*b02+a32*b03, a03*b00+a13*b01+a23*b02+a33*b03,
											a00*b10+a10*b11+a20*b12+a30*b13, a01*b10+a11*b11+a21*b12+a31*b13, a02*b10+a12*b11+a22*b12+a32*b13, a03*b10+a13*b11+a23*b12+a33*b13,
											a00*b20+a10*b21+a20*b22+a30*b23, a01*b20+a11*b21+a21*b22+a31*b23, a02*b20+a12*b21+a22*b22+a32*b23, a03*b20+a13*b21+a23*b22+a33*b23,
											a00*b30+a10*b31+a20*b32+a30*b33, a01*b30+a11*b31+a21*b32+a31*b33, a02*b30+a12*b31+a22*b32+a32*b33, a03*b30+a13*b31+a23*b32+a33*b33 };
											*/

	/*
	matrix[0][0] = a.matrix[0][0]*b.matrix[0][0]+a.matrix[1][0]*b.matrix[0][1]+a.matrix[2][0]*b.matrix[0][2]+a.matrix[3][0]*b.matrix[0][3];
	matrix[0][1] = a.matrix[0][1]*b.matrix[0][0]+a.matrix[1][1]*b.matrix[0][1]+a.matrix[2][1]*b.matrix[0][2]+a.matrix[3][1]*b.matrix[0][3];
	matrix[0][2] = a.matrix[0][2]*b.matrix[0][0]+a.matrix[1][2]*b.matrix[0][1]+a.matrix[2][2]*b.matrix[0][2]+a.matrix[3][2]*b.matrix[0][3];
	matrix[0][3] = a.matrix[0][3]*b.matrix[0][0]+a.matrix[1][3]*b.matrix[0][1]+a.matrix[2][3]*b.matrix[0][2]+a.matrix[3][3]*b.matrix[0][3];

	matrix[1][0] = a.matrix[0][0]*b.matrix[1][0]+a.matrix[1][0]*b.matrix[1][1]+a.matrix[2][0]*b.matrix[1][2]+a.matrix[3][0]*b.matrix[1][3];
	matrix[1][1] = a.matrix[0][1]*b.matrix[1][0]+a.matrix[1][1]*b.matrix[1][1]+a.matrix[2][1]*b.matrix[1][2]+a.matrix[3][1]*b.matrix[1][3];
	matrix[1][2] = a.matrix[0][2]*b.matrix[1][0]+a.matrix[1][2]*b.matrix[1][1]+a.matrix[2][2]*b.matrix[1][2]+a.matrix[3][2]*b.matrix[1][3];
	matrix[1][3] = a.matrix[0][3]*b.matrix[1][0]+a.matrix[1][3]*b.matrix[1][1]+a.matrix[2][3]*b.matrix[1][2]+a.matrix[3][3]*b.matrix[1][3];

	matrix[2][0] = a.matrix[0][0]*b.matrix[2][0]+a.matrix[1][0]*b.matrix[2][1]+a.matrix[2][0]*b.matrix[2][2]+a.matrix[3][0]*b.matrix[2][3];
	matrix[2][1] = a.matrix[0][1]*b.matrix[2][0]+a.matrix[1][1]*b.matrix[2][1]+a.matrix[2][1]*b.matrix[2][2]+a.matrix[3][1]*b.matrix[2][3];
	matrix[2][2] = a.matrix[0][2]*b.matrix[2][0]+a.matrix[1][2]*b.matrix[2][1]+a.matrix[2][2]*b.matrix[2][2]+a.matrix[3][2]*b.matrix[2][3];
	matrix[2][3] = a.matrix[0][3]*b.matrix[2][0]+a.matrix[1][3]*b.matrix[2][1]+a.matrix[2][3]*b.matrix[2][2]+a.matrix[3][3]*b.matrix[2][3];

	matrix[3][0] = a.matrix[0][0]*b.matrix[3][0]+a.matrix[1][0]*b.matrix[3][1]+a.matrix[2][0]*b.matrix[3][2]+a.matrix[3][0]*b.matrix[3][3];
	matrix[3][1] = a.matrix[0][1]*b.matrix[3][0]+a.matrix[1][1]*b.matrix[3][1]+a.matrix[2][1]*b.matrix[3][2]+a.matrix[3][1]*b.matrix[3][3];
	matrix[3][2] = a.matrix[0][2]*b.matrix[3][0]+a.matrix[1][2]*b.matrix[3][1]+a.matrix[2][2]*b.matrix[3][2]+a.matrix[3][2]*b.matrix[3][3];
	matrix[3][3] = a.matrix[0][3]*b.matrix[3][0]+a.matrix[1][3]*b.matrix[3][1]+a.matrix[2][3]*b.matrix[3][2]+a.matrix[3][3]*b.matrix[3][3];
		*/
	//modified=true;

	//return *this;
}


void GRPMATRIX::GetMultiplicationMatrix2(const GRPMATRIX&  a, const  GRPMATRIX&  b)
{			
	//GRPPROFILER::nMatrixOperations++;

	const float * __restrict m1=a.matrix[0];

	const float a00=m1[0];
	const float a01=m1[1];
	const float a02=m1[2];
	const float a03=m1[3];

	const float * __restrict m2=a.matrix[1];

	const float a10=m2[0];
	const float a11=m2[1];
	const float a12=m2[2];
	const float a13=m2[3];

	const float * __restrict m3=a.matrix[2];

	const float a20=m3[0];
	const float a21=m3[1];
	const float a22=m3[2];
	const float a23=m3[3];

	const float * __restrict m4=a.matrix[3];

	const float a30=m4[0];
	const float a31=m4[1];
	const float a32=m4[2];
	const float a33=m4[3];

	
	const float * __restrict b0=b.matrix[0];

	const float b00=b0[0];
	const float b01=b0[1];
	const float b02=b0[2];
	const float b03=b0[3];

	const float * __restrict b1=b.matrix[1];

	const float b10=b1[0];
	const float b11=b1[1];
	const float b12=b1[2];
	const float b13=b1[3];

	const float * __restrict b2=b.matrix[2];

	const float b20=b2[0];
	const float b21=b2[1];
	const float b22=b2[2];
	const float b23=b2[3];
	
	const float * __restrict b3=b.matrix[3];

	const float b30=b3[0];
	const float b31=b3[1];
	const float b32=b3[2];
	const float b33=b3[3];
	
	matrix[0][0] = a00*b00+a10*b01+a20*b02+a30*b03;
	matrix[0][1] = a01*b00+a11*b01+a21*b02+a31*b03;
	matrix[0][2] = a02*b00+a12*b01+a22*b02+a32*b03;
	matrix[0][3] = a03*b00+a13*b01+a23*b02+a33*b03;

	matrix[1][0] = a00*b10+a10*b11+a20*b12+a30*b13;
	matrix[1][1] = a01*b10+a11*b11+a21*b12+a31*b13;
	matrix[1][2] = a02*b10+a12*b11+a22*b12+a32*b13;
	matrix[1][3] = a03*b10+a13*b11+a23*b12+a33*b13;

	matrix[2][0] = a00*b20+a10*b21+a20*b22+a30*b23;
	matrix[2][1] = a01*b20+a11*b21+a21*b22+a31*b23;
	matrix[2][2] = a02*b20+a12*b21+a22*b22+a32*b23;
	matrix[2][3] = a03*b20+a13*b21+a23*b22+a33*b23;

	matrix[3][0] = a00*b30+a10*b31+a20*b32+a30*b33;
	matrix[3][1] = a01*b30+a11*b31+a21*b32+a31*b33;
	matrix[3][2] = a02*b30+a12*b31+a22*b32+a32*b33;
	matrix[3][3] = a03*b30+a13*b31+a23*b32+a33*b33;
	
	modified=true;
}


void	 GRPMATRIX::CopyTo	(GLFLOAT (*m)[16])
{
	/*
	for (int y=0;y<4;y++)
	for (int x=0;x<4;x++)
		(*m)[x+(y*4)]=this->matrix[x][y];
		*/
	
	(*m)[0]=this->matrix[0][0];
	(*m)[1]=this->matrix[1][0];
	(*m)[2]=this->matrix[2][0];
	(*m)[3]=this->matrix[3][0];
	
	(*m)[4]=this->matrix[0][1];
	(*m)[5]=this->matrix[1][1];
	(*m)[6]=this->matrix[2][1];
	(*m)[7]=this->matrix[3][1];
	
	(*m)[8]=this->matrix[0][2];
	(*m)[9]=this->matrix[1][2];
	(*m)[10]=this->matrix[2][2];
	(*m)[11]=this->matrix[3][2];
	
	(*m)[12]=this->matrix[0][3];
	(*m)[13]=this->matrix[1][3];
	(*m)[14]=this->matrix[2][3];
	(*m)[15]=this->matrix[3][3];
	
}

void	 GRPMATRIX::CopyTo	(GLFLOAT (*m))
{
	/*
	for (int y=0;y<4;y++)
	for (int x=0;x<4;x++)
		(*m)[x+(y*4)]=this->matrix[x][y];
		*/
	
	(*(m+0))	=this->matrix[0][0];
	(*(m+1))	=this->matrix[0][1];
	(*(m+2))	=this->matrix[0][2];
	(*(m+3))	=this->matrix[0][3];
	
	(*(m+4))	=this->matrix[1][0];
	(*(m+5))	=this->matrix[1][1];
	(*(m+6))	=this->matrix[1][2];
	(*(m+7))	=this->matrix[1][3];
	
	(*(m+8))	=this->matrix[2][0];
	(*(m+9))	=this->matrix[2][1];
	(*(m+10))	=this->matrix[2][2];
	(*(m+11))	=this->matrix[2][3];
	
	(*(m+12))	=this->matrix[3][0];
	(*(m+13))	=this->matrix[3][1];
	(*(m+14))	=this->matrix[3][2];
	(*(m+15))	=this->matrix[3][3];
	
}	

void	 GRPMATRIX::CopyTo	(GLFLOAT (*m)[4][4])
{
	
	for (int y=0;y<4;y++)
	for (int x=0;x<4;x++)
		(*m)[x][y]=this->matrix[x][y];
		/*
	(*m)[0][0]=this->matrix[0][0];
	(*m)[0][1]=this->matrix[0][1];
	(*m)[0][2]=this->matrix[0][2];
	(*m)[0][3]=this->matrix[0][3];
	
	(*m)[1][0]=this->matrix[1][0];
	(*m)[1][1]=this->matrix[1][1];
	(*m)[1][2]=this->matrix[1][2];
	(*m)[1][3]=this->matrix[1][3];
	
	(*m)[2][0]=this->matrix[2][0];
	(*m)[2][1]=this->matrix[2][1];
	(*m)[2][2]=this->matrix[2][2];
	(*m)[2][3]=this->matrix[2][3];
	
	(*m)[3][0]=this->matrix[3][0];
	(*m)[3][1]=this->matrix[3][1];
	(*m)[3][2]=this->matrix[3][2];
	(*m)[3][3]=this->matrix[3][3];
	*/
}

void	 GRPMATRIX::CopyTo	(GLFLOAT (*m)[3][3])
{
	
	for (int y=0;y<3;y++)
	for (int x=0;x<3;x++)
		(*m)[x][y]=this->matrix[x][y];
		/*
	(*m)[0][0]=this->matrix[0][0];
	(*m)[0][1]=this->matrix[0][1];
	(*m)[0][2]=this->matrix[0][2];

	
	(*m)[1][0]=this->matrix[1][0];
	(*m)[1][1]=this->matrix[1][1];
	(*m)[1][2]=this->matrix[1][2];
	
	
	(*m)[2][0]=this->matrix[2][0];
	(*m)[2][1]=this->matrix[2][1];
	(*m)[2][2]=this->matrix[2][2];

	
	(*m)[3][0]=this->matrix[3][0];
	(*m)[3][1]=this->matrix[3][1];
	(*m)[3][2]=this->matrix[3][2];
	*/
}