/*------------------------------------------------------------------------------------------
// GRPHIGHDINAMICRANGEPASS.CPP
*/
/**
// \class
//
// High Dinamic Range Pass
//
//  @author: Diego Martinez Ruiz de Gaona
//
//  Date of Creation : 28/11/2017 13:52:35
//  Last Modification :
*/
/* GEN  Copyright (C).  All right reserved.
/*------------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPHighDinamicRangePass.h"
#include "GRPScene.h"
#include "GRPViewport.h"
#include "GRPFactory.h"
#include "GRPMaterialLibrary.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

/*---- CLASS -----------------------------------------------------------------------------*/

/*-------------------------------------------------------------------
//  GRPHIGHDINAMICRANGEPASS::GRPHIGHDINAMICRANGEPASS
*/
/**
//
//  constructor
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  28/11/2017 13:52:50
//  @return    :
//
//  @param     :
//
*//*-----------------------------------------------------------------*/
GRPHIGHDINAMICRANGEPASS::GRPHIGHDINAMICRANGEPASS()
{

}

/*-------------------------------------------------------------------
//  GRPHIGHDINAMICRANGEPASS::~GRPHIGHDINAMICRANGEPASS
*/
/**
//
//  destructor
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  28/11/2017 13:54:32
//  @return    :
//
//  @param     :
//
*//*-----------------------------------------------------------------*/
GRPHIGHDINAMICRANGEPASS::~GRPHIGHDINAMICRANGEPASS()
{
    delete(texture);
}


/*-------------------------------------------------------------------
//   GRPHIGHDINAMICRANGEPASS::SetViewport
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  05/12/2017 12:14:47
//  @return   void :
//
//  @param    GRPVIEWPORT* :
//
*//*-----------------------------------------------------------------*/
void GRPHIGHDINAMICRANGEPASS::SetViewport(GRPVIEWPORT*  viewport)
{
    this->viewport = viewport;

    GLFLOAT w, h;

    this->viewport->GetRealWindowSize(w, h);

    texture = new GRPTEXTURE();
    texture->width = w;
    texture->height = h;
    texture->isloaded = true;

    this->material = GRPMATERIALLIBRARY::Get()->Create(texture);
}

/*-------------------------------------------------------------------
//   GRPHIGHDINAMICRANGEPASS::Begin
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  05/12/2017 12:14:42
//  @return   void :
//
//  @param     :
//
*//*-----------------------------------------------------------------*/
void GRPHIGHDINAMICRANGEPASS::Begin()
{
    this->DisableStencil();

    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);

    this->Clear();
    GRPPASS::Begin();
}

/*-------------------------------------------------------------------
//   GRPHIGHDINAMICRANGEPASS::End
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  05/12/2017 12:14:37
//  @return   void :
//
//  @param     :
//
*//*-----------------------------------------------------------------*/
void GRPHIGHDINAMICRANGEPASS::End()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    GRPPASS::End();
}

/*-------------------------------------------------------------------
//   GRPHIGHDINAMICRANGEPASS::CreateRenderTarget
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  05/12/2017 12:14:33
//  @return   void :
//
//  @param     :
//
*//*-----------------------------------------------------------------*/
void GRPHIGHDINAMICRANGEPASS::CreateRenderTarget()
{
    GLFLOAT w = 512;
    GLFLOAT h = 512;

    if (this->viewport)
        this->viewport->GetRealWindowSize(w, h);
    else
        this->holderscene->GetViewport()->GetRealWindowSize(w, h);

    //-----------------

    glGenFramebuffers(1, &hdrFBO);

    glGenTextures(1, &colorBuffer);
    glBindTexture(GL_TEXTURE_2D, colorBuffer);

#ifndef GRPOPENGLES
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, GLsizei(w), GLsizei(h), 0, GL_RGBA, GL_FLOAT, NULL);
#else
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, GLsizei(w), GLsizei(h), 0, GL_RGBA, GL_FLOAT, NULL);
#endif

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    texture->GetHandles()->Add(colorBuffer);

    // create depth buffer (renderbuffer)

    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, GLsizei(w), GLsizei(h));

    // attach buffers
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->GetHandle(), 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        return;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}