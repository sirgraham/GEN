/*------------------------------------------------------------------------------------------
//	GRPMATRIX.H
*/	
/**	
// \class 
//   
// 	Graphics Generic matrix Operations classs 
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 07/05/2014 14:04:09
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPMATRIX_H_
#define _GRPMATRIX_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XBase.h"
#include "XStack.h"

#include "GRPOpengl.h"
#include "GRPVector.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
#define GRPMATRIX_FLT_EPSILON		1.19209290E-07F
#define GRPMATRIX_DEG_TO_RAD		0.0174532924F
#define GRPMATRIX_RAD_TO_DEG		57.2957802F


enum
{	
	GRPMATRIX_ROTATION_ORDER_XYZ=0,
	GRPMATRIX_ROTATION_ORDER_XZY,
	GRPMATRIX_ROTATION_ORDER_YZX,
	GRPMATRIX_ROTATION_ORDER_YXZ,
	GRPMATRIX_ROTATION_ORDER_ZXY,
	GRPMATRIX_ROTATION_ORDER_ZYX
};

/*---- CLASS -----------------------------------------------------------------------------*/

class GRPRAY
{
public:
		GRPPOINT		start;
		GRPPOINT		end;
		GRPVECTOR		direction;
		GLFLOAT			length;
};



class GRPMATRIX
{
	public:

		static GRPVECTOR Zero;
		static GRPMATRIX Identity;
		static GRPMATRIX ortho;

													GRPMATRIX												(int isidentity)
													{
														if (isidentity==1)
															this->LoadIdentity();															
														this->modified=false;
													}

													GRPMATRIX												(GLFLOAT m[4][4])
													{
														memcpy(this->matrix, m, 4*4*sizeof(GLFLOAT));

														this->modified=false;
													}

													GRPMATRIX												(	GLFLOAT a00, GLFLOAT a01, GLFLOAT a02, GLFLOAT a03,
																														GLFLOAT a10, GLFLOAT a11, GLFLOAT a12, GLFLOAT a13,
																														GLFLOAT a20, GLFLOAT a21, GLFLOAT a22, GLFLOAT a23,
																														GLFLOAT a30, GLFLOAT a31, GLFLOAT a32, GLFLOAT a33)
													{
														matrix[0][0] = a00;
														matrix[0][1] = a01;
														matrix[0][2] = a02;
														matrix[0][3] = a03;

														matrix[1][0] = a10;
														matrix[1][1] = a11;
														matrix[1][2] = a12;
														matrix[1][3] = a13;

														matrix[2][0] = a20;
														matrix[2][1] = a21;
														matrix[2][2] = a22;
														matrix[2][3] = a23;

														matrix[3][0] = a30;
														matrix[3][1] = a31;
														matrix[3][2] = a32;
														matrix[3][3] = a33;
															
														this->modified=false;
													}

						  						GRPMATRIX												();
		virtual						 	 ~GRPMATRIX												();

		GLFLOAT*							GetMatrixPointer								();

		void									LoadIdentity										();	

		void									Inverse													();
		void									Inverse3x3											();
		void									Transpose												();
		void									Transpose3x3										();
				
		inline void						Copy														(GRPMATRIX& m)	
		{
			memcpy(this->matrix,m.matrix,sizeof(GL_FLOAT)*16);
		}

		GRPMATRIX*						BuildDirectionMatrix						(GRPVECTOR* position,GRPVECTOR* destination);								
		GRPMATRIX*						BuildQuaternion									(GRPVECTOR* obj,GRPVECTOR* eye);

		GRPMATRIX*						BuildMatrixFromFloatArray				(GLFLOAT* matrix);
		GRPMATRIX*						BuildMatrixFromFloatArray3x3		(GLFLOAT* matrix);
		GRPMATRIX*						BuildMatrixFromFloatArray3x3		(GLfloat r[3][3]);
		
		GRPMATRIX*						BuildOrthoProjectionMatrix			(GLFLOAT left,	GLFLOAT right,	GLFLOAT bottom,	GLFLOAT top,	GLFLOAT znear,	GLFLOAT zfar);
		GRPMATRIX*						BuildFrustumProjectionMatrix		(GLFLOAT left,	GLFLOAT right,	GLFLOAT bottom, GLFLOAT top,	GLFLOAT znear,	GLFLOAT zfar);
		GRPMATRIX*						BuildPixelProjectionMatrix			(GLFLOAT left,	GLFLOAT right,	GLFLOAT bottom, GLFLOAT top,	GLFLOAT znear,	GLFLOAT zfar);
			
		GRPMATRIX*						BuildScaleMatrix								(GLFLOAT x,GLFLOAT y,GLFLOAT z);
		GRPMATRIX*						BuildScaleMatrix								(const GLFLOAT* matrix);
	

		void									BuildTranslationMatrix					(GRPVECTOR v);		
		void									BuildTranslationMatrix					(GLFLOAT	 v[3]);
		void									BuildTranslationMatrix					(GLFLOAT x,GLFLOAT y,GLFLOAT z);

		
		void									fastBuildRotationMatrix					(const GLFLOAT rot[3],int RotationAxisOrder);

		void			            BuildRotationMatrix							(const GLFLOAT axis[3],int RotationAxisOrder=GRPMATRIX_ROTATION_ORDER_XYZ);
		void									BuildRotationMatrix							(GLFLOAT degrees,const GLFLOAT axis[3]);

		GRPMATRIX*						BuildRotationMatrixX						(GLFLOAT degrees);
		GRPMATRIX*						BuildRotationMatrixY						(GLFLOAT degrees);
		GRPMATRIX*						BuildRotationMatrixZ						(GLFLOAT degrees);

		void									BuildRotationMatrix							(GLFLOAT degrees, GLFLOAT x, GLFLOAT y, GLFLOAT z);
		void									BuildRotationMatrixQ						(GLFLOAT i,GLFLOAT j,GLFLOAT k,GLFLOAT r);
		void									CalculateQ											(GRPPOINT& q);
		void									BuildRotationFromQuat						(GLFLOAT w, GLFLOAT x, GLFLOAT y, GLFLOAT z);
		void									BuildRotationFromQuat						(const GLFLOAT axis[3]);



		//-------- maya matrixes

		GRPMATRIX*						BuildPivotMatrix								(GLFLOAT x,GLFLOAT y,GLFLOAT z);
		GRPMATRIX*						BuildShearMatrix								(GLFLOAT x,GLFLOAT y,GLFLOAT z);
		GRPMATRIX*						BuildScalePivotTranslationMatrix(GLFLOAT x,GLFLOAT y,GLFLOAT z);





		const GRPMATRIX				operator	=											(GLFLOAT m[4][4]);

		const GRPMATRIX				operator	*											(const GRPMATRIX& b);
		const GRPMATRIX				operator	*											(GRPMATRIX& b);
		const GRPMATRIX				operator	=											(const GRPMATRIX& m);			
		const GRPVECTOR				operator	*											(const GRPVECTOR v);
		
		void									Extract3x3NormalMatrix					(GLFLOAT (*r)[3][3]);

		GRPVECTOR							TransformPoint									(GRPVECTOR v);	
		GRPVECTOR							TransformVector									(GRPVECTOR v);
		void									GetPosition											(GRPPOINT* p);

		void									GetMultiplicationMatrix					(const GRPMATRIX*,const GRPMATRIX*);
		void									GetMultiplicationMatrix					(const GRPMATRIX&,const GRPMATRIX&);
		void									GetMultiplicationMatrix2				(const GRPMATRIX&,const GRPMATRIX&);

		void									CopyTo													(GLFLOAT (*m));
		void									CopyTo													(GLFLOAT (*m)[16]);
		void									CopyTo													(GLFLOAT (*m)[4][4]);
		void									CopyTo													(GLFLOAT (*m)[3][3]);

		void									GetNormalMatrix									(GLFLOAT (*m)[3][3]);	
		void									GetNormalMatrix									(GLFLOAT (*m)[4][4]);	
		


		GLFLOAT								matrix[4][4];	
		bool									modified;	
static GRPVECTOR							result;
	
};
	



class GRPMATRIXSTACK : public XSTACK<GRPMATRIX*>
{
	public:
					
		void									Clear()
		{
			matrix.LoadIdentity();

					for(int e=stack.GetSize()-1; e>=0; e--)
					{
						stack.Get(e)->LoadIdentity();
					}									
		}

		GRPMATRIX*						Get															()
													{		
														matrix.LoadIdentity();

														for(int e=stack.GetSize()-1; e>=0; e--)
															{																
																matrix = matrix * (*stack.Get(e));
															}														
														return &matrix;
													}
		
		GRPMATRIX							matrix;
};


enum
{
	GRPMATRIX_MODEL,
	GRPMATRIX_VIEW,
	GRPMATRIX_PROJECTION,
	GRPMATRIX_MODELVIEW,
	GRPMATRIX_VIEWPROJECTION,
	GRPMATRIX_MVP,
	GRPMATRIX_INVMODEL,
	GRPMATRIX_INVVIEW,
	GRPMATRIX_INVPROJECTION,
	GRPMATRIX_INVVIEWPROJECTION,
	GRPMATRIX_INVMVP,
	GRPMATRIX_MAX
};

class GRPMATRIXHOLDER
{
public:	
	static GRPMATRIX matrices[GRPMATRIX_MAX];
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/



#endif

