
/*------------------------------------------------------------------------------------------
//	GRPBUFFEROPENGLRENDER.CPP
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

#include "GRPFactory.h"

#include "GRPBufferOpenGLRender.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//	GRPBUFFEROPENGLRENDER::GRPBUFFEROPENGLRENDER
*/	
/**	
//	
//	Class Constructor GRPBUFFEROPENGLRENDER
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			08/07/2014 16:51:35
//	
 
//  @param				grpfactory : 
*/
/*-----------------------------------------------------------------*/
GRPBUFFEROPENGLRENDER::GRPBUFFEROPENGLRENDER() 
{
	Clean();	
}



/*-------------------------------------------------------------------
//	GRPBUFFEROPENGLRENDER::~GRPBUFFEROPENGLRENDER
*/	
/**	
//	
//	 Class Destructor GRPBUFFEROPENGLRENDER
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			26/06/2014 16:23:33
//	
*/
/*-----------------------------------------------------------------*/
GRPBUFFEROPENGLRENDER::~GRPBUFFEROPENGLRENDER() 
{
	End();

	Clean();
}




/*-------------------------------------------------------------------
//	GRPBUFFEROPENGLRENDER::Ini
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/07/2014 10:08:46
//	
//	@return 			bool : 
//
//  @param				width : 
//  @param				height : 
//  @param				nchannels : 
*/
/*-----------------------------------------------------------------*/
bool GRPBUFFEROPENGLRENDER::Ini(int width, int height, int nchannels)
{
	this->width	 =	width;
	this->height =	height;
	
	isini	=	false;
	
	bitmap = grpfactory->CreateBitmap(width, height, GRPPROPERTYMODE_32_RGBA_8888);
	if(!bitmap) return false;
	
	

	//---------------- create target texture

	glBindTexture		(GL_TEXTURE_2D, 0); //unbind texture
	glGenTextures		(1,&textureID);
	//glActiveTexture	(GL_TEXTURE0);
	GRPOPENGLCTRL::TEXTURE::SetCurrentTexture(0);
	glBindTexture		(GL_TEXTURE_2D, textureID); 

	GLBOOLEAN istexture	=	glIsTexture(textureID);
	if(!istexture) 
		{			
			grpfactory->DeleteBitmap(bitmap);
			return false;
		}

	//--------------------- generate Buffers

	glGenFramebuffers	(1, &framebufferID);
	OPENGLCHECKERROR(__L("glGenFramebuffers"));
	
	glIsFramebuffer(framebufferID);

	glBindFramebuffer	(GL_FRAMEBUFFER, framebufferID);
	OPENGLCHECKERROR(__L("glBindFramebuffer"));
 

	glTexImage2D(GL_TEXTURE_2D,	0 , GL_RGBA
																, width
																, height
																, 0
																, GL_RGBA
																, GL_UNSIGNED_BYTE
																, (void*)NULL);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S		,	GL_CLAMP_TO_EDGE);	// Warning: Android OpenGL GLES MUST and can ONLY receive this param
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T		,	GL_CLAMP_TO_EDGE);  
	
	glGenRenderbuffers(1,	&depthbufferID);
	OPENGLCHECKERROR(__L("glGenRenderbuffers"));

	glBindRenderbuffer(GL_RENDERBUFFER, depthbufferID);
	OPENGLCHECKERROR(__L("glBindRenderbuffer"));
				
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
	OPENGLCHECKERROR(__L("glRenderbufferStorage"));

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);
	if(!CheckFramebuffer()) return false;

	glFramebufferRenderbuffer(GL_FRAMEBUFFER,	GL_DEPTH_ATTACHMENT,	GL_RENDERBUFFER, depthbufferID);
	
	if(!CheckFramebuffer()) 
    {
			grpfactory->DeleteBitmap(bitmap);
			return false;
    }
						
	//----------------------- recover Opengl FSM

	glBindFramebuffer(GL_FRAMEBUFFER,0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	isini = true;

	return true;
}



/*-------------------------------------------------------------------
//	GRPBUFFEROPENGLRENDER::CheckFramebuffer
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
bool GRPBUFFEROPENGLRENDER::CheckFramebuffer()
{
	GLUINT status=glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if(status != GL_FRAMEBUFFER_COMPLETE)
		{
			switch(status)
				{	
					case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT					:	XDEBUG_PRINTCOLOR(0,__L("GRPBUFFEROPENGLRENDER::Ini Unable to create Framebuffer : GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"));
																															break;

					case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT	:	XDEBUG_PRINTCOLOR(0,__L("GRPBUFFEROPENGLRENDER::Ini Unable to create Framebuffer : GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"));						
																															break;

					case GL_FRAMEBUFFER_UNSUPPORTED										:	XDEBUG_PRINTCOLOR(0,__L("GRPBUFFEROPENGLRENDER::Ini Unable to create Framebuffer : GL_FRAMEBUFFER_UNSUPPORTED"));						
																															break;

																			default								:	XDEBUG_PRINTCOLOR(0,__L("GRPBUFFEROPENGLRENDER::Ini Unable to create Framebuffer : Unknown Reason"));
																															break;						
					}

			glBindFramebuffer(GL_FRAMEBUFFER,0);

			return false;
		}

	return true;
}




/*-------------------------------------------------------------------
//	GRPBUFFEROPENGLRENDER::SetActive
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			10/07/2014 12:43:03
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPBUFFEROPENGLRENDER::SetActive(bool t)
{	
	if(!isini) return false;
	
	if (t)	
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);	
	else
		glBindFramebuffer(GL_FRAMEBUFFER,0);	

	OPENGLCHECKERROR(__L("glBindFramebuffer"));		

	return true;
}



/*-------------------------------------------------------------------
//	GRPBUFFEROPENGLRENDER::GetFrameBuffer
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/07/2014 10:17:29
//	
//	@return 			XBYTE* : 
//
*/
/*-----------------------------------------------------------------*/
XBYTE* GRPBUFFEROPENGLRENDER::GetFrameBuffer()
{			
	if(!isini) return NULL;
	
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
	glReadPixels(0, 0, bitmap->GetWidth(), bitmap->GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE,  bitmap->GetBuffer());	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);		

	return bitmap->GetBuffer();
}




/*-------------------------------------------------------------------
//	GRPBUFFEROPENGLRENDER::End
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/07/2014 10:34:13
//	
//	@return 			bool : 
*/
/*-----------------------------------------------------------------*/
bool GRPBUFFEROPENGLRENDER::End()
{
	if(!isini) return false;

	//Bind 0, which means render to back buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDeleteFramebuffers(1, &framebufferID);
	glDeleteFramebuffers(1, &colorbufferID);
	glDeleteFramebuffers(1, &depthbufferID);

	isini = false;

	if (bitmap) delete bitmap;

	return true;
}




