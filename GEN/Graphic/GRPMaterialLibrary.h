/*------------------------------------------------------------------------------------------
//	GRPMATERIAL.H
*/	
/**	
// \class 
//   
//  Graphics Material Library (for OpenGL)
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 29/04/2014 13:53:14
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.	
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPMATERIALLIBRARY_H_
#define _GRPMATERIALLIBRARY_H_
		
/*---- INCLUDES --------------------------------------------------------------------------*/
#include "XFString.h"
#include "XPath.h"

#include "GRPOpengl.h"
#include "GRPFileBitmapPNG.h"
#include "GRPFileBitmapTGA.h"
#include "GRPBitmap.h"
#include "GRPBitmapSequence.h"

#include "GRPMaterial.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	enum GRPTEXTURETYPE
	{
		GRPTEXTURETYPE_UNKNOWN=-1,
		GRPTEXTURETYPE_TGA,
		GRPTEXTURETYPE_PNG,
		GRPTEXTURETYPE_JPG,
		GRPTEXTURETYPE_BMP
	};

	enum GRPMATERIALLOADRESULT
	{		
		GRPMATERIALLOADRESULT_ERROR=false,
		GRPMATERIALLOADRESULT_SUCCESS=1,
		GRPMATERIALLOADRESULT_DUPLICATED
	};
/*---- CLASS -----------------------------------------------------------------------------*/

class XFACTORY;
class GRPFACTORY;

class GRPMATERIALLIBRARY
{
	public:

		GLUINT													CurrentMaterialID;



		
		static GRPMATERIALLIBRARY*			instance;
		static GRPMATERIALLIBRARY*			Get()
		{
				if (instance==NULL)
						instance=new GRPMATERIALLIBRARY();
				return instance;
		}
		static void								 			Destroy()
		{
				if (instance!=NULL)
					{
						delete instance;				
						instance = NULL;
					}
		}

		XMAP<XFSTRING*,	GRPMATERIAL*>*	GetLibrary									();
		XMAP<XFSTRING*,	GRPTEXTURE*>*		GetTextures									()																	{ return &textures;																		}

		GRPMATERIALLOADRESULT						Load								        (XCHAR* filename, XCHAR* resourcename=NULL,int start=1,int end=-1);
		GRPMATERIALLOADRESULT						Load								        (XVECTOR<XFSTRING*>* faces, XCHAR* resourcename=NULL,int start=1,int end=-1);
		GRPMATERIALLOADRESULT						Load												(XVECTOR<XSTRING*>* faces, XCHAR* resourcename = NULL, int start = 1, int end = -1);
		GRPMATERIAL*										Load												(XCHAR* resourcename,int n, ...);

		GRPMATERIAL*										Create											(XCHAR* name);
		bool														Create											(XCHAR* name, XCHAR* resourcename);
		bool														Create											(XCHAR* name, GRPBITMAPSEQUENCE*);
		bool														CreateCubeMap								(XCHAR* name, GRPBITMAPSEQUENCE*);		
		GRPMATERIAL*										Create											(GRPTEXTURE* texture);

		bool														SetResourceLocation					(XPATH resourcelocationpath)
																		{				
																			this->defaultresourcelocation=resourcelocationpath;
																			return true;
																		}

		XPATH*													GetResourceLocation					()
																		{							
																			return &this->defaultresourcelocation;
																		}


		GRPMATERIAL*										FindByName									(XCHAR* name);

		bool														Add													(GRPMATERIAL* material)							{ return library.Add(material->GetName(), material);	}
		bool														Add													(GRPTEXTURE*	texture)							{ return textures.Add(texture->GetName(), texture);	}
		GRPMATERIAL*										Get													(XCHAR* name);
		bool														Delete											(XCHAR* name);
		GRPTEXTURE*											GetTexture									(XCHAR* name);
		GRPTEXTURE*											LoadTexture									(XCHAR* name, XCHAR* filename);

		GRPMATERIAL*										Select											(XCHAR* name);
		
		void														ClearTextures								() 
																		{
																			textures.DeleteElementContents();
																			textures.DeleteAll();

																			library.DeleteElementContents();
																			library.DeleteAll();
																		}
protected:
		
																		GRPMATERIALLIBRARY					();
		virtual												 ~GRPMATERIALLIBRARY					();

	private:		
		
		void														Clean												()
																		{																			
																			CurrentMaterialID	=-1;	
																		}		


		XPATH														defaultresourcelocation;
		
		XMAP<XFSTRING*,GRPMATERIAL*>			library;
		XMAP<XFSTRING*,GRPTEXTURE*>			textures;
};

	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif


