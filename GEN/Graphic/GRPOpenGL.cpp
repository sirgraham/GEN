
/*------------------------------------------------------------------------------------------
//	GRPOPENGL.CPP
//	
//	  
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 02/10/2014 17:39:17
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPShader.h"	
#include "GRPOpenGL.h"
#include "GRPProfiler.h"

#include "XLog.h"

#include "XMemory.h"

#define _CRT_SECURE_NO_WARNINGS

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	bool													GRPOPENGLCTRL::COLOR::ColorWritingIsActive		=false;
	bool													GRPOPENGLCTRL::COLOR::RedMask									=GL_TRUE;
	bool													GRPOPENGLCTRL::COLOR::GreenMask								=GL_TRUE;
	bool													GRPOPENGLCTRL::COLOR::BlueMask								=GL_TRUE;
	bool													GRPOPENGLCTRL::COLOR::AlphaMask								=GL_TRUE;

	GLFLOAT 											GRPOPENGLCTRL::COLOR::ClearColorRed						=0.0f;
	GLFLOAT 											GRPOPENGLCTRL::COLOR::ClearColorBlue					=0.0f;
	GLFLOAT 											GRPOPENGLCTRL::COLOR::ClearColorGreen					=0.0f;
	GLFLOAT												GRPOPENGLCTRL::COLOR::ClearColorAlpha					=1.0f;

	bool													GRPOPENGLCTRL::TEXTURE::TextureIsActive				=false;	
	void*													GRPOPENGLCTRL::TEXTURE::CurrentTexture				=NULL;
	GLUINT												GRPOPENGLCTRL::TEXTURE::CurrentActiveTexture  =0;
	GRPOPENGLCTRL::TEXTURE::UNIT	GRPOPENGLCTRL::TEXTURE::units[32];

	bool													GRPOPENGLCTRL::DITHER::DitherIsActive					=false;

	float													GRPOPENGLCTRL::DEPTH::ClearDepth							=1.0f;
	bool													GRPOPENGLCTRL::DEPTH::TestIsActive						=false;
	bool													GRPOPENGLCTRL::DEPTH::WritingIsActive					=false;
	GRPDEPTHFUNC									GRPOPENGLCTRL::DEPTH::Function								=GRPDEPTHFUNC_LESS;

	bool													GRPOPENGLCTRL::CULLING::IsActive							=false;
	GRPCULLMODE										GRPOPENGLCTRL::CULLING::Mode									=GRPCULLMODE_BACK;
		
	bool													GRPOPENGLCTRL::SCISSOR::IsActive							=false;
	GRPRECTINT										GRPOPENGLCTRL::SCISSOR::Rect									=GRPRECTINT(0,0,0,0);
	
	bool													GRPOPENGLCTRL::BLENDING::IsActive							=false;
	GRPBLENDINGFUNCTION						GRPOPENGLCTRL::BLENDING::FunctionSrc					=GRPBLENDINGFUNCTION_UNKNOWN;
	GRPBLENDINGFUNCTION						GRPOPENGLCTRL::BLENDING::FunctionDst					=GRPBLENDINGFUNCTION_UNKNOWN;
	GRPBLENDINGEQUATION						GRPOPENGLCTRL::BLENDING::Equation							=GRPBLENDINGEQUATION_ADD;
	GRPBLENDINGMODE								GRPOPENGLCTRL::BLENDING::Mode									=GRPBLENDINGMODE_ALPHABLEND;

	GLFLOAT												GRPOPENGLCTRL::VIEWPORT::pixelwidth						=	0.0f;
	GLFLOAT												GRPOPENGLCTRL::VIEWPORT::pixelheight					=	0.0f;
	GLFLOAT												GRPOPENGLCTRL::VIEWPORT::x										=	0.0f;
	GLFLOAT												GRPOPENGLCTRL::VIEWPORT::y										=	0.0f;
	GLFLOAT												GRPOPENGLCTRL::VIEWPORT::width								=	0.0f;
	GLFLOAT												GRPOPENGLCTRL::VIEWPORT::height								=	0.0f;

	bool													GRPOPENGLCTRL::STENCIL::IsActive							=	false;
	GRPSTENCILOPERATION						GRPOPENGLCTRL::STENCIL::StencilFailOperation	=	GRPSTENCILOPERATION_ZERO;
	GRPSTENCILOPERATION						GRPOPENGLCTRL::STENCIL::DepthFailOperation		= GRPSTENCILOPERATION_ZERO;
	GRPSTENCILOPERATION						GRPOPENGLCTRL::STENCIL::BothPassOperation			= GRPSTENCILOPERATION_ZERO;
	GRPSTENCILFUNCTION						GRPOPENGLCTRL::STENCIL::Function							=	GRPSTENCILFUNCTION_ALWAYS;
	XDWORD												GRPOPENGLCTRL::STENCIL::Mask									=	0xFF;
	XDWORD												GRPOPENGLCTRL::STENCIL::Value									=	0;
	GRPSTENCILBUFFER							GRPOPENGLCTRL::STENCIL::Buffers								= GRPSTENCILBUFFER_BOTH;

	bool													GRPOPENGLCTRL::VBO::channel_active[32]				= { false,		false,	false,	false,	false,	false,	false,	false,			false,	false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false };
	GLUINT												GRPOPENGLCTRL::VBO::CurrentVBO								= -1;
	GLUINT												GRPOPENGLCTRL::VBO::CurrentVBOIndex						= -1;
	GLINT													GRPOPENGLCTRL::VBO::MaxAttributes							= -1;


	/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



	/*-------------------------------------------------------------------
	//	GRPOPENGLCTRL::TEXTURE::SetCurrentTexture
	*/	
	/**	
	//	
	//	Sets active texture
	//	
	//	@author				Diego Martinez Ruiz de Gaona
	//	@version			22/09/2016 16:08:45
	//	
	//  @param				texturelevel : 
	*/
	/*-----------------------------------------------------------------*/
void		GRPOPENGLCTRL::TEXTURE::SetCurrentTexture(GLUINT texturelevel)
{
		if (GRPOPENGLCTRL::TEXTURE::CurrentActiveTexture == texturelevel)
				return;

			switch(texturelevel) //for debugging
			{
			case 0:		glActiveTexture(GL_TEXTURE0); break;
			case 1:		glActiveTexture(GL_TEXTURE1); break;
			case 2:		glActiveTexture(GL_TEXTURE2); break;
			case 3:		glActiveTexture(GL_TEXTURE3); break;
			case 4:		glActiveTexture(GL_TEXTURE4); break;
			case 5:		glActiveTexture(GL_TEXTURE5); break;
			case 6:		glActiveTexture(GL_TEXTURE6); break;
			case 7:		glActiveTexture(GL_TEXTURE7); break;
			case 8:		glActiveTexture(GL_TEXTURE8); break;
			case 9:		glActiveTexture(GL_TEXTURE9); break;
			case 10:	glActiveTexture(GL_TEXTURE10); break;
			case 11:	glActiveTexture(GL_TEXTURE11); break;
			case 12:	glActiveTexture(GL_TEXTURE12); break;
			case 13:	glActiveTexture(GL_TEXTURE13); break;
			case 14:	glActiveTexture(GL_TEXTURE14); break;
			case 15:	glActiveTexture(GL_TEXTURE15); break;
			case 16:	glActiveTexture(GL_TEXTURE16); break;
			case 17:	glActiveTexture(GL_TEXTURE17); break;
			case 18:	glActiveTexture(GL_TEXTURE18); break;
			case 19:	glActiveTexture(GL_TEXTURE19); break;
			case 20:	glActiveTexture(GL_TEXTURE20); break;
			case 21:	glActiveTexture(GL_TEXTURE21); break;
			case 22:	glActiveTexture(GL_TEXTURE22); break;
			case 23:	glActiveTexture(GL_TEXTURE23); break;
			case 24:	glActiveTexture(GL_TEXTURE24); break;
			case 25:	glActiveTexture(GL_TEXTURE25); break;
			case 26:	glActiveTexture(GL_TEXTURE26); break;
			case 27:	glActiveTexture(GL_TEXTURE27); break;
			case 28:	glActiveTexture(GL_TEXTURE28); break;
			case 29:	glActiveTexture(GL_TEXTURE29); break;
			case 30:	glActiveTexture(GL_TEXTURE30); break;
			case 31:	glActiveTexture(GL_TEXTURE31); break;
			default:		
								glActiveTexture(GL_TEXTURE0); break;
			}
			GRPOPENGLCTRL::TEXTURE::CurrentActiveTexture = texturelevel;
}

/*-------------------------------------------------------------------
//	GRPOPENGLCTRL::SetTexture2D
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			30/10/2015 16:17:45
//	
//  @param				b : 
*/
/*-----------------------------------------------------------------*/
void		GRPOPENGLCTRL::TEXTURE::SetTextureMode			(bool		b, GRPTEXTUREMODE m)
{
	if (b && !GRPOPENGLCTRL::TEXTURE::TextureIsActive)
	{
		glEnable(m);
		GRPOPENGLCTRL::TEXTURE::TextureIsActive=true;
		GRPPROFILER::nRegisterAccess++;
	}
	else
	if (!b && GRPOPENGLCTRL::TEXTURE::TextureIsActive)
	{
		glDisable(m);
		GRPPROFILER::nRegisterAccess++;
		GRPOPENGLCTRL::TEXTURE::TextureIsActive=false;
	}
}


/*-------------------------------------------------------------------
//	 GRPOPENGLCTRL::TEXTURE::BindTexture
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	26/10/2017 13:54:16
//	@return		void : 
//
//	@param		GLUINT : 
//	@param		GLUINT : 
//
*//*-----------------------------------------------------------------*/
void		GRPOPENGLCTRL::TEXTURE::BindTexture(GLUINT target, GLUINT mode, GLUINT handle)
{				
		if (TEXTURE::units[target].active						&&
				TEXTURE::units[target].type			== mode &&
				TEXTURE::units[target].handle		== handle)
				return;
						
		TEXTURE::units[target].type   = mode;
		TEXTURE::units[target].handle = handle;
		TEXTURE::units[target].active = 1;
		
		GRPOPENGLCTRL::TEXTURE::SetCurrentTexture(target);
		glBindTexture(mode, handle);
		GRPPROFILER::nTextureChanges++;		
}


/*-------------------------------------------------------------------
//	GRPOPENGLCTRL::SetDepthTest
*/	
/**	
//	
//	Sets if the Depth is going to be tested to discard the fragment
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			21/09/2015 14:41:29
//	
//  @param				b : bool active/unactive
*/
/*-----------------------------------------------------------------*/
void		GRPOPENGLCTRL::DEPTH::SetDepthTest(bool b) 
{
	if (b && !GRPOPENGLCTRL::DEPTH::TestIsActive)
	{
		glEnable(GL_DEPTH_TEST);
		GRPOPENGLCTRL::DEPTH::TestIsActive=true;
		GRPPROFILER::nRegisterAccess++;
	}
	else
	if (!b && GRPOPENGLCTRL::DEPTH::TestIsActive)
	{
		glDisable(GL_DEPTH_TEST);
		GRPPROFILER::nRegisterAccess++;
		GRPOPENGLCTRL::DEPTH::TestIsActive=false;
	}
}

/*-------------------------------------------------------------------
//	GRPOPENGLCTRL::SetDepthWriting
*/	
/**	
//	
//	Sets if the current fragment depth is going to be written to the depth buffer
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			21/09/2015 14:42:21
//	
//  @param				b : 
*/
/*-----------------------------------------------------------------*/
void		GRPOPENGLCTRL::DEPTH::SetDepthWriting(bool b) 
{
	if (b && !GRPOPENGLCTRL::DEPTH::WritingIsActive)
	{
		glDepthMask(GL_TRUE);
		GRPPROFILER::nRegisterAccess++;
		GRPOPENGLCTRL::DEPTH::WritingIsActive=true;
	}
	else
	if (!b && GRPOPENGLCTRL::DEPTH::WritingIsActive)
	{
		glDepthMask(GL_FALSE);
		GRPPROFILER::nRegisterAccess++;
		GRPOPENGLCTRL::DEPTH::WritingIsActive=false;
	}
}

/*-------------------------------------------------------------------
//	GRPOPENGLCTRL::COLOR::Clear
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			24/11/2016 13:32:52
//	
*/
/*-----------------------------------------------------------------*/
void		GRPOPENGLCTRL::COLOR::Clear(XDWORD hastoclear)
{
	glClear(hastoclear);
	OPENGLCHECKERROR(__L("GRPOPENGLCTRL::COLOR::Clear"));
}

/*-------------------------------------------------------------------
//	GRPOPENGLCTRL::COLOR::SetClearColor
*/	
/**	
//	Sets GPU color for clear
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			24/11/2016 13:27:51
//	
//  @param				r : red
//  @param				g : green
//  @param				b : blue
//  @param				a : alpha
*/
/*-----------------------------------------------------------------*/
void		GRPOPENGLCTRL::COLOR::SetClearColor(GLFLOAT r,GLFLOAT g,GLFLOAT b,GLFLOAT a)
{
	if (r!=GRPOPENGLCTRL::COLOR::ClearColorRed || g!=GRPOPENGLCTRL::COLOR::ClearColorGreen || b!=GRPOPENGLCTRL::COLOR::ClearColorBlue || a!=GRPOPENGLCTRL::COLOR::ClearColorAlpha)
	{
		GRPOPENGLCTRL::COLOR::ClearColorRed=r; 
		GRPOPENGLCTRL::COLOR::ClearColorBlue=b;
		GRPOPENGLCTRL::COLOR::ClearColorGreen=g;
		GRPOPENGLCTRL::COLOR::ClearColorAlpha=a;
		glClearColor(r,g,b,a);
	}
}

/*-------------------------------------------------------------------
//	GRPOPENGLCTRL::SetColorWriting
*/	
/**	
//	
//	Disables Color Frame Writing
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			21/09/2015 14:42:39
//	
//  @param				b : 
*/
/*-----------------------------------------------------------------*/
void		GRPOPENGLCTRL::COLOR::SetColorWriting(bool b, bool	RedMask, bool BlueMask, bool GreenMask, bool AlphaMask) 
{
	if (b && !GRPOPENGLCTRL::COLOR::ColorWritingIsActive)
	{
		glColorMask(RedMask,GreenMask,BlueMask,AlphaMask);	//(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
		GRPPROFILER::nRegisterAccess++;

		GRPOPENGLCTRL::COLOR::ColorWritingIsActive=true;

		GRPOPENGLCTRL::COLOR::RedMask		=RedMask;
		GRPOPENGLCTRL::COLOR::GreenMask	=GreenMask;
		GRPOPENGLCTRL::COLOR::BlueMask	=BlueMask;
		GRPOPENGLCTRL::COLOR::AlphaMask	=AlphaMask;	
	}
	else
	if (!b && GRPOPENGLCTRL::COLOR::ColorWritingIsActive)
	{
		glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
		GRPPROFILER::nRegisterAccess++;
		GRPOPENGLCTRL::COLOR::ColorWritingIsActive=false;
	}
}
		


/*-------------------------------------------------------------------
//	GRPOPENGLCTRL::SetBlendingMode
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			09/06/2016 11:37:12
//	
//  @param				src : 
*/
/*-----------------------------------------------------------------*/
void		GRPOPENGLCTRL::BLENDING::SetBlendingMode(GRPBLENDINGMODE src)
{
	switch(src)
	{
	case GRPBLENDINGMODE_UNKNOWN:				break;
	case GRPBLENDINGMODE_NONE:					SetBlendingFunction(GRPBLENDINGFUNCTION_ZERO,				GRPBLENDINGFUNCTION_ZERO);								SetBlendingEquation(GRPBLENDINGEQUATION_ADD);				break;
	case GRPBLENDINGMODE_ADDITIVE:			SetBlendingFunction(GRPBLENDINGFUNCTION_ONE,				GRPBLENDINGFUNCTION_ONE);									SetBlendingEquation(GRPBLENDINGEQUATION_ADD);				break;
	case GRPBLENDINGMODE_MULTIPLICATIVE:SetBlendingFunction(GRPBLENDINGFUNCTION_DST_COLOR,	GRPBLENDINGFUNCTION_ONE);									SetBlendingEquation(GRPBLENDINGEQUATION_ADD);				break;
	case GRPBLENDINGMODE_SUBSTRACTIVE:	SetBlendingFunction(GRPBLENDINGFUNCTION_ONE,				GRPBLENDINGFUNCTION_ONE);									SetBlendingEquation(GRPBLENDINGEQUATION_SUB);				break;
	case GRPBLENDINGMODE_ALPHABLEND:		SetBlendingFunction(GRPBLENDINGFUNCTION_SRC_ALPHA,	GRPBLENDINGFUNCTION_ONE_MINUS_SRC_ALPHA);	SetBlendingEquation(GRPBLENDINGEQUATION_ADD);				break;
	}
	GRPOPENGLCTRL::BLENDING::Mode=src;
}

/*-------------------------------------------------------------------
//	GRPOPENGLCTRL::SetBlendingFunction
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			08/06/2016 16:31:00
//	
//  @param				src : 
//  @param				dst : 
*/
/*-----------------------------------------------------------------*/
void		GRPOPENGLCTRL::BLENDING::SetBlendingFunction(GRPBLENDINGFUNCTION src,GRPBLENDINGFUNCTION dst)
{
	if (BLENDING::IsActive)
	if (BLENDING::FunctionSrc!=src || BLENDING::FunctionDst!=dst)
	{
		BLENDING::FunctionSrc=src;
		BLENDING::FunctionDst=dst;

		glBlendFunc(src,dst);
		GRPPROFILER::nRegisterAccess++;	
	}
}

/*-------------------------------------------------------------------
//	GRPOPENGLCTRL::SetBlendingEquation
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			09/06/2016 11:55:24
//	
//  @param				eq : 
*/
/*-----------------------------------------------------------------*/
void		GRPOPENGLCTRL::BLENDING::SetBlendingEquation(GRPBLENDINGEQUATION eq)
{
	if (BLENDING::IsActive)
	if (BLENDING::Equation!=eq)
	{
		BLENDING::Equation=eq;
		glBlendEquation(eq);
		GRPPROFILER::nRegisterAccess++;	
	}
}

/*-------------------------------------------------------------------
//	GRPOPENGLCTRL::SetScissor
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			13/07/2016 11:19:14
//	
//  @param				onoff : 
//  @param				xo : 
//  @param				yo : 
//  @param				xd : 
//  @param				yd : 
*/
/*-----------------------------------------------------------------*/
void		GRPOPENGLCTRL::SCISSOR::SetScissor(bool onoff, int xo, int yo, int xd, int yd)
{
		if (onoff && !GRPOPENGLCTRL::SCISSOR::IsActive)
		{
				glEnable(GL_SCISSOR_TEST);				
				GRPOPENGLCTRL::SCISSOR::IsActive = true;				
		}
		else
		if (!onoff && GRPOPENGLCTRL::SCISSOR::IsActive)
		{
				glDisable(GL_SCISSOR_TEST);
				GRPOPENGLCTRL::SCISSOR::IsActive = false;
		}

		if (GRPOPENGLCTRL::SCISSOR::IsActive)
				if (GRPOPENGLCTRL::SCISSOR::Rect.x1 != xo ||
						GRPOPENGLCTRL::SCISSOR::Rect.x2 != xd ||
						GRPOPENGLCTRL::SCISSOR::Rect.y1 != yo ||
						GRPOPENGLCTRL::SCISSOR::Rect.y2 != yd)
				{
						glScissor(xo, yo, xd - xo, yd - yo);
						GRPOPENGLCTRL::SCISSOR::Rect = GRPRECTINT(xo, yo, xd, yd);
				}

}

/*-------------------------------------------------------------------
//	GRPOPENGLCTRL::STENCIL::SetStencil
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			09/08/2016 12:48:57
//	
//  @param				onoff : 
//  @param				operation : 
//  @param				function : 
//  @param				value : 
*/
/*-----------------------------------------------------------------*/
void	GRPOPENGLCTRL::STENCIL::SetStencil		(bool	onoff)
{
	if (!onoff)
	{
		if (GRPOPENGLCTRL::STENCIL::IsActive)
		{
			glDisable(GL_STENCIL_TEST);
			GRPOPENGLCTRL::STENCIL::IsActive=false;
		}
	}
	else
	{		
		if (!GRPOPENGLCTRL::STENCIL::IsActive)
		{
			glEnable(GL_STENCIL_TEST);
			GRPOPENGLCTRL::STENCIL::IsActive=true;
		}
	}
}

/*-------------------------------------------------------------------
//	GRPOPENGLCTRL::STENCIL::SetStencilOperation
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			09/08/2016 13:19:07
//	
//  @param				stencil_fails : 
//  @param				depth_fails : 
//  @param				both_passes : 
*/
/*-----------------------------------------------------------------*/
void	GRPOPENGLCTRL::STENCIL::SetStencilOperation	(GRPSTENCILBUFFER buffer,	GRPSTENCILOPERATION stencil_fails,	GRPSTENCILOPERATION depth_fails,	GRPSTENCILOPERATION both_passes)
{
	if (GRPOPENGLCTRL::STENCIL::Buffers!=buffer || GRPOPENGLCTRL::STENCIL::StencilFailOperation!=stencil_fails || GRPOPENGLCTRL::STENCIL::DepthFailOperation!=depth_fails || GRPOPENGLCTRL::STENCIL::BothPassOperation!=both_passes)
	{
		glStencilOp(stencil_fails,depth_fails,both_passes);
		//glStencilOpSeparate(GRPSTENCILBUFFER_FRONT,stencil_fails,depth_fails,both_passes);

		GRPOPENGLCTRL::STENCIL::StencilFailOperation=stencil_fails;
		GRPOPENGLCTRL::STENCIL::DepthFailOperation	=depth_fails;
		GRPOPENGLCTRL::STENCIL::BothPassOperation		=both_passes;
		GRPOPENGLCTRL::STENCIL::Buffers							=buffer;
	}
}

/*-------------------------------------------------------------------
//	GRPOPENGLCTRL::STENCIL::SetStencilFunction
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			09/08/2016 13:19:22
//	
//  @param				function : 
//  @param				value : 
*/
/*-----------------------------------------------------------------*/
void	GRPOPENGLCTRL::STENCIL::SetStencilFunction	( GRPSTENCILFUNCTION function, XDWORD value)
{
	if (GRPOPENGLCTRL::STENCIL::Function!=function || GRPOPENGLCTRL::STENCIL::Value!=value)
	{
		glStencilFunc(function, value, 0xFF);
		GRPOPENGLCTRL::STENCIL::Function=function;
		GRPOPENGLCTRL::STENCIL::Value		=value;
	}
}

/*-------------------------------------------------------------------
//	GRPOPENGLCTRL::STENCIL::SetStencilMask
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			09/08/2016 14:50:16
//	
//  @param				mask : 
*/
/*-----------------------------------------------------------------*/
void GRPOPENGLCTRL::STENCIL::SetStencilMask(GLUINT mask)
{
	if (GRPOPENGLCTRL::STENCIL::Mask!=mask)
	{
		glStencilMask(mask);
		GRPOPENGLCTRL::STENCIL::Mask=mask;
	}
}


/*-------------------------------------------------------------------
//	 GRPOPENGLCTRL::VBO::EnableVertexAttribArray
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	25/10/2017 17:47:46
//	@return		void : 
//
//	@param		GLUINT : 
//
*//*-----------------------------------------------------------------*/
void GRPOPENGLCTRL::VBO::EnableVertexAttribArray(GLUINT channel)
{
		if (!GRPOPENGLCTRL::VBO::channel_active[channel])
		{				
				glEnableVertexAttribArray(channel);
				GRPOPENGLCTRL::VBO::channel_active[channel] = true;
		}
}

/*-------------------------------------------------------------------
//	 GRPOPENGLCTRL::VBO::DisableVertexAttribArray
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	25/10/2017 17:47:53
//	@return		void : 
//
//	@param		GLUINT : 
//
*//*-----------------------------------------------------------------*/
void GRPOPENGLCTRL::VBO::DisableVertexAttribArray(GLUINT channel)
{
		
		if (GRPOPENGLCTRL::VBO::channel_active[channel])
		{
				glDisableVertexAttribArray(channel);
				GRPOPENGLCTRL::VBO::channel_active[channel] = false;
		}
	
}

/*-------------------------------------------------------------------
//	 GRPOPENGLCTRL::VBO::VertexAttributePointer
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	25/10/2017 17:48:01
//	@return		void : 
//
//	@param		GLUINT : 
//	@param		GLUINT : 
//	@param		GLUINT : 
//	@param		GLUINT : 
//	@param		GLUINT : 
//	@param		void* : 
//
*//*-----------------------------------------------------------------*/
void GRPOPENGLCTRL::VBO::VertexAttributePointer(GLUINT channel, GLUINT channelsize, GLUINT datatype, GLUINT isnormalized, GLUINT stride, void* offset)
{
		if (!GRPSHADER::CurrentProgram) return;

		//somehow , if the normal and texture are not enabled, it crashes on rpi

		if (channel > GLUINT(GRPOPENGLCTRL::VBO::MaxAttributes))
				return;

		bool reject = false;
		/*
				switch (channel)
				{										
				//case GRPVERTEX_CHANNEL_NORMAL:							if (GRPSHADER::CurrentProgram->attributeNormal          == -1)	return;	break;
				case GRPVERTEX_CHANNEL_TEXTURE:							if (GRPSHADER::CurrentProgram->attributeTextCoord       == -1)	reject = true;;	break;
				case GRPVERTEX_CHANNEL_BINORMAL:						if (GRPSHADER::CurrentProgram->attributeBinormal        == -1)	reject = true;	break;
				case GRPVERTEX_CHANNEL_TANGENT:							if (GRPSHADER::CurrentProgram->attributeTangent         == -1)	reject = true;	break;
				case GRPVERTEX_CHANNEL_BONE_INDEX:					if (GRPSHADER::CurrentProgram->attributeBoneIndices     == -1)	reject = true;	break;
				case GRPVERTEX_CHANNEL_BONE_WEIGHTS:				if (GRPSHADER::CurrentProgram->attributeBoneWeights     == -1)	reject = true;	break;
				case GRPVERTEX_CHANNEL_MORPH_POSITION1:			if (GRPSHADER::CurrentProgram->attributePositionMorph1  == -1)	reject = true;	break;
				case GRPVERTEX_CHANNEL_MORPH_POSITION2:			if (GRPSHADER::CurrentProgram->attributePositionMorph2  == -1)	reject = true;	break;
				case GRPVERTEX_CHANNEL_MORPH_POSITION3:			if (GRPSHADER::CurrentProgram->attributePositionMorph3  == -1)	reject = true;	break;
				case GRPVERTEX_CHANNEL_MORPH_POSITION4:			if (GRPSHADER::CurrentProgram->attributePositionMorph4  == -1)	reject = true;	break;
				case GRPVERTEX_CHANNEL_MORPH_POSITION5:			if (GRPSHADER::CurrentProgram->attributePositionMorph5  == -1)	reject = true;	break;
				case GRPVERTEX_CHANNEL_MORPH_POSITION6:			if (GRPSHADER::CurrentProgram->attributePositionMorph6  == -1)	reject = true;	break;
				case GRPVERTEX_CHANNEL_MORPH_POSITION7:			if (GRPSHADER::CurrentProgram->attributePositionMorph7  == -1)	reject = true;	break;
				case GRPVERTEX_CHANNEL_MORPH_POSITION8:			if (GRPSHADER::CurrentProgram->attributePositionMorph8  == -1)	reject = true;	break;
				case GRPVERTEX_CHANNEL_MORPH_POSITION9:			if (GRPSHADER::CurrentProgram->attributePositionMorph9  == -1)	reject = true;	break;
				case GRPVERTEX_CHANNEL_MORPH_POSITION10:		if (GRPSHADER::CurrentProgram->attributePositionMorph10 == -1)  reject = true; break;
				case GRPVERTEX_CHANNEL_MORPH_POSITION11:		if (GRPSHADER::CurrentProgram->attributePositionMorph11 == -1)  reject = true; break;
				case GRPVERTEX_CHANNEL_MORPH_POSITION12:		if (GRPSHADER::CurrentProgram->attributePositionMorph12 == -1)  reject = true; break;
				}		 
			
				if (reject)
				{
						GRPOPENGLCTRL::VBO::DisableVertexAttribArray(channel);
						return;
				}
*/

		/*
		if (GRPSHADER::CurrentProgram)
				switch (channel)
				{
						//position debe estar activo siempre, o si no, los shader que no usen attributos no podrán subir los vertices a la gpu
				case GRPVERTEX_CHANNEL_POSITION:						if (GRPSHADER::CurrentProgram->attributeVertex == -1)	return GRPOPENGLCTRL::VBO::DisableVertexAttribArray(GRPVERTEX_CHANNEL_POSITION);	break;
				//case GRPVERTEX_CHANNEL_POSITION:						GRPOPENGLCTRL::VBO::EnableVertexAttribArray(GRPVERTEX_CHANNEL_POSITION); break;
				case GRPVERTEX_CHANNEL_NORMAL:							if (GRPSHADER::CurrentProgram->attributeNormal == -1)	return GRPOPENGLCTRL::VBO::DisableVertexAttribArray(GRPVERTEX_CHANNEL_NORMAL);	break;
				case GRPVERTEX_CHANNEL_BINORMAL:						if (GRPSHADER::CurrentProgram->attributeBinormal == -1)	return GRPOPENGLCTRL::VBO::DisableVertexAttribArray(GRPVERTEX_CHANNEL_BINORMAL);	break;
				case GRPVERTEX_CHANNEL_TANGENT:							if (GRPSHADER::CurrentProgram->attributeTangent == -1)	return GRPOPENGLCTRL::VBO::DisableVertexAttribArray(GRPVERTEX_CHANNEL_TANGENT);	break;
				case GRPVERTEX_CHANNEL_TEXTURE:							if (GRPSHADER::CurrentProgram->attributeTextCoord == -1)	return GRPOPENGLCTRL::VBO::DisableVertexAttribArray(GRPVERTEX_CHANNEL_TEXTURE);		break;
				case GRPVERTEX_CHANNEL_BONE_INDEX:					if (GRPSHADER::CurrentProgram->attributeBoneIndices == -1)	return GRPOPENGLCTRL::VBO::DisableVertexAttribArray(GRPVERTEX_CHANNEL_BONE_INDEX);	break;
				case GRPVERTEX_CHANNEL_BONE_WEIGHTS:				if (GRPSHADER::CurrentProgram->attributeBoneWeights == -1)	return GRPOPENGLCTRL::VBO::DisableVertexAttribArray(GRPVERTEX_CHANNEL_BONE_WEIGHTS);	break;
				case GRPVERTEX_CHANNEL_MORPH_POSITION1:			if (GRPSHADER::CurrentProgram->attributePositionMorph1 == -1)	return GRPOPENGLCTRL::VBO::DisableVertexAttribArray(GRPVERTEX_CHANNEL_MORPH_POSITION1);	break;
				case GRPVERTEX_CHANNEL_MORPH_POSITION2:			if (GRPSHADER::CurrentProgram->attributePositionMorph2 == -1)	return GRPOPENGLCTRL::VBO::DisableVertexAttribArray(GRPVERTEX_CHANNEL_MORPH_POSITION2);		break;
				case GRPVERTEX_CHANNEL_MORPH_POSITION3:			if (GRPSHADER::CurrentProgram->attributePositionMorph3 == -1)	return GRPOPENGLCTRL::VBO::DisableVertexAttribArray(GRPVERTEX_CHANNEL_MORPH_POSITION3);			break;
				case GRPVERTEX_CHANNEL_MORPH_POSITION4:			if (GRPSHADER::CurrentProgram->attributePositionMorph4 == -1)	return GRPOPENGLCTRL::VBO::DisableVertexAttribArray(GRPVERTEX_CHANNEL_MORPH_POSITION4);		break;
				case GRPVERTEX_CHANNEL_MORPH_POSITION5:			if (GRPSHADER::CurrentProgram->attributePositionMorph5 == -1)	return GRPOPENGLCTRL::VBO::DisableVertexAttribArray(GRPVERTEX_CHANNEL_MORPH_POSITION5);			break;
				case GRPVERTEX_CHANNEL_MORPH_POSITION6:			if (GRPSHADER::CurrentProgram->attributePositionMorph6 == -1)	return GRPOPENGLCTRL::VBO::DisableVertexAttribArray(GRPVERTEX_CHANNEL_MORPH_POSITION6);		break;
				case GRPVERTEX_CHANNEL_MORPH_POSITION7:			if (GRPSHADER::CurrentProgram->attributePositionMorph7 == -1)	return GRPOPENGLCTRL::VBO::DisableVertexAttribArray(GRPVERTEX_CHANNEL_MORPH_POSITION7);			break;
				case GRPVERTEX_CHANNEL_MORPH_POSITION8:			if (GRPSHADER::CurrentProgram->attributePositionMorph8 == -1)	return GRPOPENGLCTRL::VBO::DisableVertexAttribArray(GRPVERTEX_CHANNEL_MORPH_POSITION8);			break;
				case GRPVERTEX_CHANNEL_MORPH_POSITION9:			if (GRPSHADER::CurrentProgram->attributePositionMorph9 == -1)	return GRPOPENGLCTRL::VBO::DisableVertexAttribArray(GRPVERTEX_CHANNEL_MORPH_POSITION9);			break;
				case GRPVERTEX_CHANNEL_MORPH_POSITION10:		if (GRPSHADER::CurrentProgram->attributePositionMorph10 == -1)	return GRPOPENGLCTRL::VBO::DisableVertexAttribArray(GRPVERTEX_CHANNEL_MORPH_POSITION10);			break;
				case GRPVERTEX_CHANNEL_MORPH_POSITION11:		if (GRPSHADER::CurrentProgram->attributePositionMorph11 == -1)	return GRPOPENGLCTRL::VBO::DisableVertexAttribArray(GRPVERTEX_CHANNEL_MORPH_POSITION11);			break;
				case GRPVERTEX_CHANNEL_MORPH_POSITION12:		if (GRPSHADER::CurrentProgram->attributePositionMorph12 == -1)	return GRPOPENGLCTRL::VBO::DisableVertexAttribArray(GRPVERTEX_CHANNEL_MORPH_POSITION12);			break;
				}
				*/

		if (!GRPOPENGLCTRL::VBO::channel_active[channel])
				GRPOPENGLCTRL::VBO::EnableVertexAttribArray(channel);
				
		
				glVertexAttribPointer(channel, channelsize, datatype, isnormalized, stride, offset);				
		
}

/*-------------------------------------------------------------------
//	GRPOPENGLCTRL::SetDither
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			13/07/2016 17:43:56
//	
//  @param				b : 
*/
/*-----------------------------------------------------------------*/
void		GRPOPENGLCTRL::DITHER::SetDither(bool	b)
{
	if (b && !GRPOPENGLCTRL::DITHER::DitherIsActive)
	{
		glEnable(GL_DITHER);
		GRPOPENGLCTRL::DITHER::DitherIsActive=true;
	}
	else
	if (!b && GRPOPENGLCTRL::DITHER::DitherIsActive)
	{
		glDisable(GL_DITHER);
		GRPOPENGLCTRL::DITHER::DitherIsActive=false;
	}
}

/*-------------------------------------------------------------------
//	GRPOPENGLCTRL::SetBackFaceCulling
*/	
/**	
//	
//	Enables/disables Back Face Culling
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			21/09/2015 14:42:56
//	
//  @param				b : 
*/
/*-----------------------------------------------------------------*/
void		GRPOPENGLCTRL::CULLING::SetBackFaceCulling(bool b,GRPCULLMODE mode) 
{
	if (b)
	{
		if (GRPOPENGLCTRL::CULLING::IsActive==false || mode!=GRPOPENGLCTRL::CULLING::Mode)
		{
			glEnable(GL_CULL_FACE);
			GRPPROFILER::nRegisterAccess++;
			GRPOPENGLCTRL::CULLING::IsActive=true;

			if (GRPOPENGLCTRL::CULLING::Mode!=mode)
			{
				switch(mode)
				{
				case GRPCULLMODE_BACK:	glCullFace(GL_BACK);	break;
				case GRPCULLMODE_FRONT: glCullFace(GL_FRONT); break;
				}
				GRPOPENGLCTRL::CULLING::Mode=mode;
			}
		}
	}
	else
	if (!b)
	{
		if (GRPOPENGLCTRL::CULLING::IsActive)
		{
			glDisable(GL_CULL_FACE);
			GRPPROFILER::nRegisterAccess++;
			GRPOPENGLCTRL::CULLING::IsActive=false;
		}
	}
}

/*-------------------------------------------------------------------
//	GRPOPENGLCTRL::SetDepthFunc
*/	
/**	
//	
//	Sets Depth pass function
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			21/09/2015 14:43:09
//	
*/
/*-----------------------------------------------------------------*/
void		GRPOPENGLCTRL::DEPTH::SetDepthFunc (GRPDEPTHFUNC type)
{
	if (GRPOPENGLCTRL::DEPTH::Function!=type)
	{
		GRPOPENGLCTRL::DEPTH::Function=type;
		glDepthFunc(type);			    // The Type Of Depth Test To Do 	
		GRPPROFILER::nRegisterAccess++;
	}
}

/*-------------------------------------------------------------------
//	GRPOPENGLCTRL::SetClearDepth
*/	
/**	
//	
//	Sets the Depth Buffer Clear Depth
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			21/09/2015 14:43:29
//	
//  @param				dep : 
*/
/*-----------------------------------------------------------------*/
void		GRPOPENGLCTRL::DEPTH::SetClearDepth(float dep)
{
	if (GRPOPENGLCTRL::DEPTH::ClearDepth!=dep)
	{
		GRPOPENGLCTRL::DEPTH::ClearDepth=dep;
		GRPGLCLEARDEPTH(dep);			    // depth of depthbuffer cleaning
		GRPPROFILER::nRegisterAccess++;
	}
}

/*-------------------------------------------------------------------
//	GRPOPENGLCTRL::SetAlphaBlending
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			18/03/2016 13:28:43
//	
//  @param				alpha : 
*/
/*-----------------------------------------------------------------*/
void		GRPOPENGLCTRL::BLENDING::SetAlphaBlending			(bool	alpha,GRPBLENDINGMODE mode)
{
	if (alpha)
	{
		if (!BLENDING::IsActive || (BLENDING::IsActive && mode!=GRPOPENGLCTRL::BLENDING::Mode))
		{
				if (!BLENDING::IsActive)
				{
						glEnable(GL_BLEND);
						GRPOPENGLCTRL::BLENDING::IsActive = true;
				}				

			SetBlendingMode(mode);			
			GRPPROFILER::nRegisterAccess++;
		}
	}
	else
	{
			if (GRPOPENGLCTRL::BLENDING::IsActive)
			{
				GRPOPENGLCTRL::BLENDING::IsActive=false;
				glDisable (GL_BLEND);
				
				GRPPROFILER::nRegisterAccess++;
			}
	}

}

/*-------------------------------------------------------------------
//	GRPOPENGLCTRL::Ini
*/	
/**	
//	
//	Inits the OpenGL machine
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			21/09/2015 14:43:44
//	
*/
/*-----------------------------------------------------------------*/
void	GRPOPENGLCTRL::Ini()
	{		
		
		STENCIL::SetStencil					(false);																																			OPENGLCHECKERROR(__L("Opengl Stencil Test"));
		DITHER::SetDither						(false);
		glDisable										(GL_POLYGON_OFFSET_FILL);																											OPENGLCHECKERROR(__L("Opengl Polygon Offset Test"));
		glDisable										(GL_SAMPLE_ALPHA_TO_COVERAGE);																								OPENGLCHECKERROR(__L("Opengl Sample Alpha Test"));

		SCISSOR::SetScissor					(false);																																			OPENGLCHECKERROR(__L("Opengl Scissor Test"));
		DEPTH::SetDepthTest					(true);																																				OPENGLCHECKERROR(__L("Opengl Depth Test"));
		DEPTH::SetDepthWriting			(true);																																				OPENGLCHECKERROR(__L("Opengl Depth Writing Test"));
		DEPTH::SetClearDepth				(1.0f);																																				OPENGLCHECKERROR(__L("Opengl Depth Clear"));				
		DEPTH::SetDepthFunc					(GRPDEPTHFUNC_LESS);																													OPENGLCHECKERROR(__L("Opengl Depth Func"));				

		CULLING::SetBackFaceCulling	(true);																																				OPENGLCHECKERROR(__L("Opengl BackFaceCulling Test"));
		COLOR::SetColorWriting			(true);																																				OPENGLCHECKERROR(__L("Opengl ColorWriting Test"));

		BLENDING::SetAlphaBlending  (false,GRPBLENDINGMODE_ALPHABLEND);
		
		glGetIntegerv								(GL_MAX_VERTEX_ATTRIBS, &VBO::MaxAttributes);		

		//glEnable(GL_POINT_SPRITE);																																		OPENGLCHECKERROR(__L("Opengl enable point sprite"));		
    //glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);																												OPENGLCHECKERROR(__L("Openglvertex program point size"));	
		//glPointSize(120.0f);
		OPENGLCHECKERROR(__L("Opengl Ini"));																													
	
		for (XDWORD e = 0; e < 32; e++)
		{
				GRPOPENGLCTRL::VBO::channel_active[e] = 0;
				TEXTURE::units[e].active = 0;
				TEXTURE::units[e].type	 = 0;
				TEXTURE::units[e].handle = 0;

		}
	}


/*-------------------------------------------------------------------
//	GRPOPENGLCTRL::ClearErrors
*/	
/**	
//	
//	Clears the gl state errors
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			21/09/2015 14:44:01
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool  GRPOPENGLCTRL::ClearErrors()
{
while (glGetError()!=GL_NO_ERROR);
return true;
}

/*-------------------------------------------------------------------
//	GRPOPENGLCTRL::Log
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/05/2017 10:39:13
//	
//	@return 			bool : 
//
//  @param				text : 
*/
/*-----------------------------------------------------------------*/
bool  GRPOPENGLCTRL::Log(char*	text)
{

			
	return true;
}

/*-------------------------------------------------------------------
//	GRPOPENGLCTRL::CheckError
*/	
/**	
//	
//	Shows GL errors
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			21/09/2015 14:44:10
//	
//	@return 			bool : 
//
//  @param				text : 
//  @param				file : 
//  @param				function : 
//  @param				line : 
*/
/*-----------------------------------------------------------------*/
bool		GRPOPENGLCTRL::CheckError(XCHAR* text,const char* file,const char* function,int line) 
{
#ifdef CHECK_OPENGL_ERRORS

	static int limit=10;

	if (limit==0) 
		return false;



	
	GLenum err (glGetError());
	bool ret=false;

	
	while(err!=GL_NO_ERROR) 
		{
			XSTRING functionname;
			functionname = (function);
			XSTRING filename;
			filename = (file);

			limit--;
			XSTRING error;								 
			switch(err) 
				{
					case GL_INVALID_OPERATION							:	error.Set(__L("INVALID_OPERATION"));																	break;
					case GL_INVALID_ENUM									: error.Set(__L("INVALID_ENUM"));																				break;
					case GL_INVALID_VALUE									: error.Format(__L("INVALID_VALUE (MaxAttrib %d)"), VBO::MaxAttributes);break;
					case GL_OUT_OF_MEMORY									:	error.Set(__L("OUT_OF_MEMORY"));																			break;
					case GL_INVALID_FRAMEBUFFER_OPERATION	: error.Set(__L("INVALID_FRAMEBUFFER_OPERATION"));											break;					
											default										: error.Set(__L("unknown"));																						break;           
				}
 
			int id=0;glGetIntegerv(GL_CURRENT_PROGRAM,&id);													

			XSTRING s;
			s.Format(__L("%d %s : %s %i in File %s in Function %s:%d using shader(%d - [%d])\r\n"),limit,text, error.Get(), err,filename.Get(),functionname.Get(),line,id,GRPSHADER::CurrentShader);
			XDEBUG_PRINTCOLOR(4,s.Get());
			XLOG::GetInstance().AddEntry(XLOGLEVEL_ERROR		, __L("GUI"), false, s.Get());											
				
			XSTRING_CREATEOEM(s,lin);
			GRPOPENGLCTRL::Log(lin);			
			XSTRING_DELETEOEM(lin);


					err=glGetError();   
					ret		=	true;		

				if (limit==0) 				
				{
					XDEBUG_PRINTCOLOR(4, __L("\t\t------------------------- TOO MANY ERRORS! stopping checks"));
					break;
				}
			
		}
	
			
	return ret;
#else
	return true;
#endif
}


