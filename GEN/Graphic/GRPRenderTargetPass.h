/*------------------------------------------------------------------------------------------
// GRPRenderTargetPass.HPP
*/
/**
// \class
//
// Render to offscreen pass
//
//	@author: Diego Martinez Ruiz de Gaona 
//
//	Date of Creation : 23/11/2017 13:44:39 
//	Last Modification : 
*/
/* GEN  Copyright (C).  All right reserved.
/*------------------------------------------------------------------------------------------*/


#ifndef _GRPRENDERTARGETPASS_HPP
#define _GRPRENDERTARGETPASS_HPP
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPPass.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

/*---- CLASS -----------------------------------------------------------------------------*/

class GRPRENDERTARGETPASS : public GRPPASS
{
public:

														GRPRENDERTARGETPASS	();
		virtual									~GRPRENDERTARGETPASS();
		virtual bool						OnAdded							();		
		virtual void						Begin								();
		virtual void						End									();
		virtual GRPTEXTURE*			GetTexture					();
		virtual GRPMATERIAL*		GetMaterial					();
		virtual void						SetViewport					(GRPVIEWPORT*	viewport);

protected:

		GRPTEXTURE* texture;
		GRPMATERIAL* material;

private:

		void Clean()
		{

		}

};
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif