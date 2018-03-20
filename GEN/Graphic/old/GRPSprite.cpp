/*------------------------------------------------------------------------------------------
//	GRPSPRITE.H
*/	
/**	
// \class 
//   
//  Sprite objects
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 02/09/2014 12:42:50
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "GRPSprite.h"
#include "GRPScene.h"
#include "GRPObjectBuilder.h"
#include "GRPShader.h"

#include "XMemory.h"

/*-------------------------------------------------------------------
//	GRPSPRITE::GRPSPRITE
*/	
/**	
//	
//	Class Constructor GRPSPRITE
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			07/10/2015 17:08:03
//	
*/
/*-----------------------------------------------------------------*/
GRPSPRITE::GRPSPRITE()
{
	Clean();
	GRPNAMED::classtype = GRPNAMED_TYPE_GRPSPRITE;
}

/*-------------------------------------------------------------------
//	GRPSPRITEPOINT::Draw
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			07/10/2015 17:08:08
//	
//	@return 			bool : 
//
//  @param				m : 
//  @param				mv : 
//  @param				projection : 
*/
/*-----------------------------------------------------------------*/
bool GRPSPRITEPOINT::Draw(GRPMATRIX& viewmatrix,GRPMATRIX* viewprojection,GRPMATRIX* projection)
{
	GRPTEXTURE* texture				=this->GetNode()->GetMaterial()->GetLayer(0);
							textureframe	=(float)((float)textureframe+(float)frameStep);
							texture->SetFrame(textureframe);
	
				bool				ret			=	GRPELEMENT::Draw				(viewmatrix,viewprojection,projection);		
	return ret;
}

/*-------------------------------------------------------------------
//	GRPSPRITEQUAD::Draw
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			09/10/2015 14:10:56
//	
//	@return 			bool : 
//
//  @param				viewmatrix : 
//  @param				viewprojection : 
//  @param				projection : 
//  @param				extshaderlink : 
*/
/*-----------------------------------------------------------------*/
bool GRPSPRITEQUAD::Draw(GRPMATRIX& viewmatrix,GRPMATRIX* viewprojection,GRPMATRIX* projection)
{
	if (this->GetNode()->GetMaterial()!=NULL)
	{
		GRPTEXTURE* texture				=this->GetNode()->GetMaterial()->GetLayer(0);
								textureframe	=(float)((float)textureframe+(float)frameStep);
								texture->SetFrame(textureframe);
	}

	return GRPELEMENT::Draw				(viewmatrix,viewprojection,projection);		
}
	
bool GRPSPRITESTRIP::Draw	(GRPMATRIX* viewmatrix,GRPMATRIX* viewprojection,GRPMATRIX* projection)
{
	return false;
}

bool GRPBILLBOARD::Draw		(GRPMATRIX* viewmatrix,GRPMATRIX* viewprojection,GRPMATRIX* projection)
{
	return false;
}

/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
