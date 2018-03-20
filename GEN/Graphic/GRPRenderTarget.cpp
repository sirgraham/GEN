
/*------------------------------------------------------------------------------------------
//	GRPRENDERTARGET.CPP
//	
//	Creates a render target for receiving opengl buffer
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 26/06/2014 16:07:44
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XFactory.h"
#include "XFile.h"	
#include "GRPRenderTarget.h"
#include "GRPTexture.h"
#include "GRPOpenGL.h"

#include "XMemory.h"


/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//	GRPRENDERTARGET::GRPRENDERTARGET
*/	
/**	
//	
//	Class Constructor GRPRENDERTARGET
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			26/06/2014 16:23:09
//	
*/
/*-----------------------------------------------------------------*/
GRPRENDERTARGET::GRPRENDERTARGET(int width, int height)
{
	Clean();

	// could need to check this, maybe use current windows size for it
	CaptureWidth		=width;
	CaptureHeight		=height;
	CaptureChannels	=4;
	Size						=CaptureWidth*CaptureHeight*CaptureChannels;
	FrameBufferData	=NULL;
}

/*-------------------------------------------------------------------
//	GRPRENDERTARGET::~GRPRENDERTARGET
*/	
/**	
//	
//	 Class Destructor GRPRENDERTARGET
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			26/06/2014 16:23:33
//	
*/
/*-----------------------------------------------------------------*/
GRPRENDERTARGET::~GRPRENDERTARGET() 
{
 	if (FrameBufferData!=NULL)  delete(FrameBufferData);
	FrameBufferData=NULL;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDeleteFramebuffers(1,&frameBufferTargetID);
	glDeleteFramebuffers(1,&DepthBufferTargetID);

	delete(colorbuffer);

	Clean();
}

/*-------------------------------------------------------------------
//	GRPRENDERTARGET::CaptureFrameData
*/	
/**	
//	
//	Gets current frame buffer data and puts it on the FrameBufferData
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			26/06/2014 16:24:07
//	
*/
/*-----------------------------------------------------------------*/
void GRPRENDERTARGET::CaptureFrameData()
{
			if (FrameBufferData==NULL)
				FrameBufferData=new XBYTE[Size];
								 
			if (FrameBufferData!=NULL)
			{
				glReadPixels(0, 0, CaptureWidth, CaptureHeight, GL_RGBA, GL_UNSIGNED_BYTE,  FrameBufferData);
				OPENGLCHECKERROR(__L("glReadPixels"));
			}
}
	

/*-------------------------------------------------------------------
//	GRPRENDERTARGET::InitFrameBuffers
*/	
/**	
//	
//	Initializes Frame Buffers
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			26/06/2014 16:25:01
//	
*/
/*-----------------------------------------------------------------*/
void GRPRENDERTARGET::InitFrameBuffers(GRPTEXTURE* colorbuffer, GRPRENDERTARGETCOMPONENT component)
{
		OPENGLCHECKERROR(__L("InitFrameBuffers"));

	//------------------------- RGBA8 RenderBuffer, 32 bit depth RenderBuffer, CaptureWidth x CaptureHeight
	glGenFramebuffers							(1,										&frameBufferTargetID);																							OPENGLCHECKERROR(__L("glGenFramebuffer"));
	glBindFramebuffer							(GL_FRAMEBUFFER,			frameBufferTargetID);																								OPENGLCHECKERROR(__L("glBindFramebuffer"));
			
	//------------------------- Attach color buffer to FBO
	this->colorbuffer = colorbuffer;

	if (!colorbuffer->GetIsCubeMap())		
			glFramebufferTexture2D(GL_FRAMEBUFFER, component, GL_TEXTURE_2D,									this->colorbuffer->GetHandle(), 0);											
	else																
			glFramebufferTexture2D(GL_FRAMEBUFFER, component, GL_TEXTURE_CUBE_MAP_POSITIVE_X, this->colorbuffer->GetHandle(), 0);										
	
	OPENGLCHECKERROR(__L("glFramebufferRenderbuffer"));

	this->component=component;

	//------------------------- Generate Buffers
	if (component==GRPRENDERTARGETCOMPONENT_COLOR)
	{
	glGenRenderbuffers						(1,										&DepthBufferTargetID);																							OPENGLCHECKERROR(__L("glGenRenderbuffers"));	
	glBindRenderbuffer						(GL_RENDERBUFFER,			DepthBufferTargetID);																								OPENGLCHECKERROR(__L("glBindRenderbuffer"));	
	glRenderbufferStorage					(GL_RENDERBUFFER,			GL_DEPTH_COMPONENT24,			CaptureWidth,			CaptureHeight);					OPENGLCHECKERROR(__L("glRenderbufferStorage"));
	
	//------------------------- Attach depth buffer to FBO
	glFramebufferRenderbuffer			(GL_FRAMEBUFFER,			GL_DEPTH_ATTACHMENT,			GL_RENDERBUFFER,	DepthBufferTargetID);		OPENGLCHECKERROR(__L("glFramebufferRenderbuffer"));
	}
	else 
	if (component==GRPRENDERTARGETCOMPONENT_DEPTH)
	{		
		glBindFramebuffer(GL_FRAMEBUFFER, 0); 		
	}
	//-------------------------Does the GPU support current FBO configuration?
	CheckFramebuffer();
}



/*-------------------------------------------------------------------
//	GRPRENDERTARGET::SelectAsTarget
*/	
/**	
//	
//	Selects this render target as current render buffer
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			26/06/2014 16:25:18
//	
*/
/*-----------------------------------------------------------------*/
void  GRPRENDERTARGET::SelectAsTarget(XDWORD face)
{
		OPENGLCHECKERROR(__L("Pre FBO bind"));
		if (!colorbuffer) return; //newly created lights might not have the buffer ready

		glBindFramebuffer					(GL_FRAMEBUFFER, frameBufferTargetID);						
		if (!colorbuffer->GetIsCubeMap())					
		glFramebufferTexture2D(GL_FRAMEBUFFER, component, GL_TEXTURE_2D, this->colorbuffer->GetHandle(), 0);										//OPENGLCHECKERROR(__L("glFramebufferRenderbuffer"));					
		else					
		glFramebufferTexture2D(GL_FRAMEBUFFER, component, GL_TEXTURE_CUBE_MAP_POSITIVE_X+face, this->colorbuffer->GetHandle(), 0);										OPENGLCHECKERROR(__L("glFramebufferRenderbuffer"));
			
}



/*-------------------------------------------------------------------
//	GRPRENDERTARGET::Deselect
*/	
/**	
//	
//	Sets the BackBuffer as current target, again
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			26/06/2014 16:25:38
//	
*/
/*-----------------------------------------------------------------*/
void  GRPRENDERTARGET::Deselect()
{		
		//Bind 0, which means render to back buffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
}



/*-------------------------------------------------------------------
//	GRPRENDERTARGET::SaveAsRaw
*/	
/**	
//	
//	Saves raw data of captured frame
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			26/06/2014 16:25:58
//	
//  @param				factory : 
//  @param				Name : 
*/
/*-----------------------------------------------------------------*/
void  GRPRENDERTARGET::SaveAsRaw(XCHAR* Name)		
{

	XFILE* File= xfactory->Create_File();	

	if (!File->Exist(Name))
			File->Create(Name);
		else
			File->Open(Name,false);

	if(File->IsOpen())
		{
			int stridesize=4*CaptureWidth;

			for (int e=this->CaptureHeight;e>=0;e--)		 //reverse de data for visualization
				File->Write(FrameBufferData+(stridesize*e),stridesize);
			
			File->Close();
		}
	 else
		{
			XDEBUG_PRINTCOLOR(1,__L("Unable to save Frame"));
		}
}

	


/*-------------------------------------------------------------------
//	GRPRENDERTARGET::CheckFramebuffer
*/	
/**		
//	Checks if Framebuffer is OK
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			10/07/2014 10:15:39
//	
//	@return 			bool : 
*/
/*-----------------------------------------------------------------*/
bool GRPRENDERTARGET::CheckFramebuffer()
{
	GLUINT status=glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if(status != GL_FRAMEBUFFER_COMPLETE)
		{
			switch(status)
				{	
					case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT					:	XDEBUG_PRINTCOLOR(0,__L("GRPRENDERTARGET::Ini Unable to create Framebuffer : GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"));					break;
					case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT	:	XDEBUG_PRINTCOLOR(0,__L("GRPRENDERTARGET::Ini Unable to create Framebuffer : GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"));	break;
					case GL_FRAMEBUFFER_UNSUPPORTED										:	XDEBUG_PRINTCOLOR(0,__L("GRPRENDERTARGET::Ini Unable to create Framebuffer : GL_FRAMEBUFFER_UNSUPPORTED"));										break;					
					case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS					:	XDEBUG_PRINTCOLOR(0,__L("GRPRENDERTARGET::Ini Unable to create Framebuffer : GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS"));					break;
																			default								:	XDEBUG_PRINTCOLOR(0,__L("GRPRENDERTARGET::Ini Unable to create Framebuffer : Unknown Reason"));																break;						
					}
			return false;
		}


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	return true;
}

