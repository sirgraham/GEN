
/*------------------------------------------------------------------------------------------
//  GRPSHADOWPASS.CPP
//
//  Shadow Pass
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 27/07/2016 12:23:20
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPShadowPass.h"
#include "GRPLight.h"
#include "GRPScene.h"
#include "GRPViewport.h"
#include "GRPCamera.h"
#include "GRPRenderTarget.h"
#include "GRPShaderLibrary.h"
#include "GRPProfiler.h"


/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



const char GRPSHADERLIBRARY::LightSpaceVertex[] =
"attribute vec3 a_position;\n"
"uniform mat4 modelMat;\n"
//"uniform mat4 viewMat;\n"
//"uniform mat4 projectionMat;\n"
//"uniform mat4 modelviewprojectionMat;\n"
"uniform mat4 lightSpaceMatrix;\n"

"void main()\n"
"{\n"

"    gl_Position =  lightSpaceMatrix* modelMat * vec4(a_position, 1.0); \n"
//ViewMatrixLoc
//"    gl_Position = modelviewprojectionMat * vec4(a_position, 1.0); \n"
//"    gl_Position = projectionMat * viewMat * modelMat * vec4(a_position, 1.0); \n"
"}  \n";


const char GRPSHADERLIBRARY::LightSpaceFragment[] =
"void main()\n"
"{      \n"
" gl_FragColor=vec4(vec3(0.0),1.0);\n"
"}  \n";


const char GRPSHADERLIBRARY::ShadowCastingVertex[] =
"attribute vec3 a_position;\n"
"attribute vec3 a_normal;\n"
"attribute vec3 a_binormal;\n"
"attribute vec3 a_tangent;\n"
"attribute vec2 a_texCoord;\n"

"uniform mat4 modelMat;\n"
"uniform mat4 modelviewMat;\n"
"uniform mat4 modelviewprojectionMat;\n"
"uniform mat4 lightSpaceMatrix;\n"


"uniform mat4 lightDataMatrix[4];\n"
"uniform mat4 lightColorMatrix[4];\n"
"uniform mat4 materialDataMat;\n"

//------------- varying

"varying vec4 v_position;\n"
"varying vec3 v_normal;\n"
"varying vec2 v_texCoord;\n"
"varying vec4 specularColor,lambertianColor; \n"
"varying vec4 v_FragPosLightSpace;\n"

"uniform sampler2D s_texture; \n"
"uniform sampler2D textureChannel5; \n"

"uniform float bias;\n"

" void main()\n"
"{ \n"

" v_FragPosLightSpace = lightSpaceMatrix * modelMat * vec4(a_position, 1.0);\n"

"   vec3 normalInterp;      \n"
"   vec3 vertPos;           \n"
"   vec3  L;                \n"
"   float fspecular=0.0;    \n"
"   float flambertian=0.0;  \n"
"   float fattenuattion=1.0;\n"

"   float Tlambertian=0.0;\n"
"   float Tspecular=0.0;\n"

"    gl_Position    = modelviewprojectionMat * vec4(a_position, 1.0);         \n"
"    v_texCoord     = a_texCoord;\n"

"    v_position = modelviewMat * vec4(a_position, 1.0);\n"

//----------------- light0

"     L                 = (v_position.xyz-lightDataMatrix[0][0].xyz).xyz;                               \n" //precalculated lightposition*viewMat
"     float distance    = length (L);                                                                   \n"
"     fattenuattion     = (materialDataMat[3][1]*(lightDataMatrix[0][1][3])) / ((((lightDataMatrix[0][1][2]*distance)+lightDataMatrix[0][1][1])*distance)+lightDataMatrix[0][1][0]);\n"

//"     normalInterp      = mat3(modelviewMat) * vec4(a_normal, 0.0).xyz;                                 \n"
"     normalInterp      = (modelviewMat * vec4(a_normal, 0.0)).xyz;                                           \n"
"     vec3 normal       = normalize(normalInterp);                                                      \n"
"     v_normal  = normal;\n"
"     vec3 lightDir     = normalize(-L);                                                                \n"
"     lambertianColor   = materialDataMat[0]*max(dot(lightDir,normal), 0.0)*lightColorMatrix[0][1]*fattenuattion;         \n"

"     vec4 Emissive     = materialDataMat[2];                                                         \n"
"     vec3 reflectDir   = reflect(-lightDir, normal);                                                   \n"
"     float specAngle   = max(dot(reflectDir, normalize(-v_position.xyz)), 0.0);                        \n"
"       specularColor   = (materialDataMat[1]*lightColorMatrix[0][2]*fattenuattion*pow(specAngle, materialDataMat[3][0]))+Emissive;   \n"

//----------------- final

"}";


const char GRPSHADERLIBRARY::ShadowCastingFragment[] =
"varying vec4 v_position;\n"
"varying vec2 v_texCoord;\n"
"varying vec3 v_normal;\n"
"varying vec4 v_FragPosLightSpace;\n"

"uniform mat4 modelviewMat;\n"
"uniform mat4 modelviewprojectionMat;\n"

"uniform sampler2D    s_texture;  \n"
"uniform sampler2D    textureChannel5;  \n"
"uniform vec2         textureSize5;\n"

"uniform mat4 lightDataMatrix[4];\n"
"varying vec4 specularColor,lambertianColor; \n"

"uniform float bias;\n"

" void main()\n"
"{ \n"
"     vec3 L            = (v_position.xyz-lightDataMatrix[0][0].xyz).xyz;                               \n" //precalculated lightposition*viewMat
"     vec3 lightDir     = normalize(-L);                                                                \n"

" vec3 projCoords = v_FragPosLightSpace.xyz / v_FragPosLightSpace.w;\n"     // Transform to [0,1] range
"projCoords = projCoords * 0.5 + 0.5;\n"                                    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)

"float closestDepth = texture2D(textureChannel5, projCoords.xy).r; \n"      // Get depth of current fragment from light's perspective
"float currentDepth = projCoords.z;\n"

"float tbias = max(bias * (1.0 - dot(v_normal, lightDir)), bias); \n"       // Check whether current frag pos is in shadow




"float shadow = 1.0;\n"
" vec2 texelSize = vec2(1.0 / textureSize5.x);\n"

//----------------- poisson like

"for(int x = -1; x <= 1; ++x)\n"
"{\n"
"   for(int y = -1; y <= 1; ++y)\n"
"   {\n"
"       float pcfDepth = texture2D(textureChannel5, projCoords.xy + vec2(x, y) * texelSize).r; \n"
"       shadow += (currentDepth - tbias < pcfDepth) ? 1.0 : 0.1;        \n"
"   }    \n"
"}\n"
"shadow /= 9.0;\n"


"   vec4  surfaceColor    = texture2D(s_texture,v_texCoord)*shadow;\n"

"   vec3  totalColor      = (lambertianColor.rgb*surfaceColor.rgb) + specularColor.rgb*shadow;  \n"
//  "   vec4  totalColor      =   lambertianColor*texture2D(s_texture,v_texCoord)*shadow + specularColor;  \n"
"         gl_FragColor    = vec4(totalColor,1.0);                                                             \n"


"}\n";


const char GRPSHADERLIBRARY::ShadowCastingFragmentDiffuse[] =
"varying vec4 v_position;\n"
"varying vec2 v_texCoord;\n"
"varying vec3 v_normal;\n"
"varying vec4 v_FragPosLightSpace;\n"

"uniform mat4 modelviewMat;\n"
"uniform mat4 modelviewprojectionMat;\n"

"uniform sampler2D    s_texture;  \n"
"uniform sampler2D    textureChannel5;  \n"
"uniform vec2         textureSize5;\n"

"uniform mat4 lightDataMatrix[4];\n"
"varying vec4 specularColor,lambertianColor; \n"

"uniform float bias;\n"

" void main()\n"
"{ \n"
"     vec3 L            = (v_position.xyz-lightDataMatrix[0][0].xyz).xyz;                               \n" //precalculated lightposition*viewMat
"     vec3 lightDir     = normalize(-L);                                                                \n"

" vec3 projCoords = v_FragPosLightSpace.xyz / v_FragPosLightSpace.w;\n"     // Transform to [0,1] range
"projCoords = projCoords * 0.5 + 0.5;\n"                                    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)

"float closestDepth = texture2D(textureChannel5, projCoords.xy).r; \n"      // Get depth of current fragment from light's perspective
"float currentDepth = projCoords.z;\n"

"float tbias = max(bias * (1.0 - dot(v_normal, lightDir)), bias); \n"       // Check whether current frag pos is in shadow




"float shadow = 1.0;\n"
" vec2 texelSize = vec2(1.0 / textureSize5.x);\n"

//----------------- poisson like

"for(int x = -1; x <= 1; ++x)\n"
"{\n"
"   for(int y = -1; y <= 1; ++y)\n"
"   {\n"
"       float pcfDepth = texture2D(textureChannel5, projCoords.xy + vec2(x, y) * texelSize).r; \n"
"       shadow += currentDepth - tbias < pcfDepth ? 1.0 : 0.1;        \n"
"   }    \n"
"}\n"
"shadow /= 9.0;\n"

//"   vec3  totalColor      = lambertianColor.rgb*shadow + specularColor.rgb*shadow;  \n"
"   vec3  totalColor      = lambertianColor.rgb*shadow;// + specularColor.rgb*shadow;  \n"
//  "   vec4  totalColor      =   lambertianColor*texture2D(s_texture,v_texCoord)*shadow + specularColor;  \n"
"         gl_FragColor    = vec4(totalColor,1.0);                                                             \n"

"}\n";




/*-------------------------------------------------------------------
//  GRPSHADOWPASS::GRPSHADOWPASS
*/
/**
//
//  Class Constructor GRPSHADOWPASS
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      27/07/2016 12:23:42
//
//  @param        name :
*/
/*-----------------------------------------------------------------*/
GRPSHADOWPASS::GRPSHADOWPASS () : GRPPASS(__L("shadowpass"))
{
  Clean();
  Clear();
  type                        = GRPPASS_TYPE_CULLFACE | GRPPASS_TYPE_COLORWRITE | GRPPASS_TYPE_DEPTHWRITE | GRPPASS_TYPE_DEPTHTEST;
  BackFaceCullingIsActive     = true;
  DepthWritingIsActive        = true;
  DepthTestIsActive           = true;

  this->globalshaderoverride  = true;
  lightingIsEnabled           = true;
  this->shader                = GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"LSP");
  this->priority              = 1;
  this->hasToClear            = GRPPASS_CLEAR_FLAG_DEPTH;

  if (light!=NULL)
    SetLight(light);

  passtype                    = GRPPASS_SHADOW_SPOT;
}

/*-------------------------------------------------------------------
//  GRPSHADOWPASS::SetLight
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      27/07/2016 12:43:14
//
//  @param        light :
*/
/*-----------------------------------------------------------------*/
void  GRPSHADOWPASS::SetLight(GRPLIGHT* light)
{
  this->light=light;
}

/*-------------------------------------------------------------------
//  GRPSHADOWPASS::OnAdded
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/06/2017 17:30:39
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool  GRPSHADOWPASS::OnAdded()
{
  if (!this->light)
  {
      GRPLIGHT* l = holderscene->FindLight(this->light_name.Get());
      this->SetLight(l);
  }

  if (this->light)
  {
    shadowsize=(512.0f*this->light->shadowquality);

    SetViewport     (this->light->GetViewport());
    this->viewport->SetWindowSize (shadowsize,shadowsize);
    this->viewport->SetPosition   (0.0,0.0);
    this->viewport->SetSize       (1.0,1.0);
    this->light->CreateDepthBuffer((int)shadowsize,(int)shadowsize);

    SetCamera       (this->light);
    this->camera->SetViewport(this->viewport);

    if (this->light->GetType()==GRPLIGHTTYPE_POINT)
    {
        this->camera->SetRotation(0.0,0.0,0.0);
        this->camera->Setinterestposition(0.0,0.0,1.0);
        this->camera->Update();
        this->camera->UpdateWorldMatrix();
        this->camera->BuildLookAt();
    }

  this->GetShadowCasters();
  this->SetShadowReceivers();
  }
  return true;
}


/*-------------------------------------------------------------------
//  GRPSHADOWPASS::SetRenderTarget
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      27/07/2016 12:43:19
//
//  @param        rt :
*/
/*-----------------------------------------------------------------*/
void  GRPSHADOWPASS::SetRenderTarget(GRPRENDERTARGET* rt)
{
  this->rendertarget=rt;
}


/*-------------------------------------------------------------------
//  GRPSHADOWPASS::Begin
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      27/07/2016 12:26:25
//
*/
/*-----------------------------------------------------------------*/
void GRPSHADOWPASS::Begin()
{
    if (this->light)
    {
        if (!this->light->rendertarget)
            this->light->BuildShadowMap();
        else
            this->light->rendertarget->SelectAsTarget(currentiteration);
        GRPPASS::Begin();
    }
}




/*-------------------------------------------------------------------
//  GRPSHADOWPASS::End
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      27/07/2016 12:26:25
//
*/
/*-----------------------------------------------------------------*/
void GRPSHADOWPASS::End()
{
    if(this->light)
    {
        GRPPASS::End();

        GRPOPENGLCTRL::CULLING::SetBackFaceCulling(true, GRPCULLMODE_BACK);

        this->light->rendertarget->Deselect();
    }
}


/*-------------------------------------------------------------------
//  GRPSHADOWPASS::GetShadowCasters
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      27/07/2016 14:10:21
//
//  @return       int :
//
*/
/*-----------------------------------------------------------------*/
int GRPSHADOWPASS::GetShadowCasters()
{
  if (holderscene)
    for (XDWORD e=0;e<this->holderscene->GetPass()->GetRenderOrder()->GetSize();e++)
    {
      GRPELEMENT* element=static_cast<GRPELEMENT*>(this->holderscene->GetPass()->GetRenderOrder()->Get(e));
      if (element) //es un element con mesh
        if (element->GetCastsShadows())
          this->RenderOrder->Add(element);
    }
    return this->RenderOrder->GetSize();
}

int GRPSHADOWPASS::SetShadowReceivers()
{
    /*
    int n = 0;
    if (holderscene)
        for (XDWORD e = 0; e<this->holderscene->GetPass()->GetRenderOrder()->GetSize(); e++)
        {
            GRPELEMENT* element = static_cast<GRPELEMENT*>(this->holderscene->GetPass()->GetRenderOrder()->Get(e));
            if (element) //es un element con mesh
                if (element->GetReceivesShadows())
                {
                    element->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByName(__L("ShadowCastingDiffuse")));
                    if (element->GetNode())
                        if (element->GetNode()->GetMaterial())
                            if (element->GetNode()->GetMaterial()->GetLayers())
                                if (element->GetNode()->GetMaterial()->GetLayer(GRPTEXTURELAYERTYPE_DIFFUSE))
                                    element->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByName(__L("ShadowCasting")));
                }
        }
        */
    return 0;
}



/*-------------------------------------------------------------------
//  GRPSHADOWPASS::GRPSHADOWPASS
*/
/**
//
//  Class Constructor GRPSHADOWPASS
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      27/07/2016 12:23:42
//
//  @param        name :
*/
/*-----------------------------------------------------------------*/
GRPSHADOWPOINTLIGHTPASS::GRPSHADOWPOINTLIGHTPASS ()
{
  Clean();
  shadowsize    = 512;
  this->iterations=6;
  passtype = GRPPASS_SHADOW_POINT;

  type                        = GRPPASS_TYPE_CULLFACE | GRPPASS_TYPE_COLORWRITE | GRPPASS_TYPE_DEPTHWRITE | GRPPASS_TYPE_DEPTHTEST;
  BackFaceCullingIsActive     = true;
  DepthWritingIsActive        = true;
  DepthTestIsActive           = true;

  this->globalshaderoverride  = true;
  lightingIsEnabled           = false;
  this->shader                = GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"LSP");
  this->priority              = 1;
  this->hasToClear            = GRPPASS_CLEAR_FLAG_DEPTH;

  if (light!=NULL)
    SetLight(light);

}

bool GRPSHADOWPOINTLIGHTPASS::OnAdded()
{
    OPENGLCHECKERROR(__L(" GRPSHADOWPOINTLIGHTPASS::OnAdded"));

  this->CreateViewport();
  this->viewport->SetName(__L("OmniView"));
  this->viewport->SetWindowSize (shadowsize,shadowsize);
  this->viewport->SetPosition   (0.0,0.0);
    OPENGLCHECKERROR(__L(" GRPSHADOWPOINTLIGHTPASS::viewport->SetPosition"))
  this->viewport->SetSize       (1.0,1.0);
  this->viewport->SetProjection (GRPPROJECTIONTYPE_FRUSTUM);
  this->viewport->SetProjectionFrustum(0.000001f,100000.0f);

  OPENGLCHECKERROR(__L(" GRPSHADOWPOINTLIGHTPASS::viewport->SetProjectionFrustum"))

  this->viewport->SetFieldOfView(90.0f);

  OPENGLCHECKERROR(__L(" GRPSHADOWPOINTLIGHTPASS::viewport->SetFieldOfView"))

  this->viewport->Select();

  OPENGLCHECKERROR(__L(" GRPSHADOWPOINTLIGHTPASS::viewport->Select"))

  this->CreateCamera();
  const GRPVECTOR* v=this->light->GetPosition();
  this->camera->SetPosition(v->vector[0],v->vector[1], v->vector[2]);
  this->camera->fov=90.0f;
  this->camera->SetViewport(this->viewport);
  this->camera->CreateDepthBuffer((int)shadowsize,(int)shadowsize);

  this->GetShadowCasters();

  this->shader                = GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"LSP");

  return true;
}


/*-------------------------------------------------------------------
//  GRPSHADOWPOINTLIGHTPASS::Start
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      11/07/2017 20:13:48
//
*/
/*-----------------------------------------------------------------*/
void GRPSHADOWPOINTLIGHTPASS::Start()
{
    GRPVECTOR pos=this->light->GetWorldPosition();
    this->camera->SetPosition(pos.vector[0],pos.vector[1],pos.vector[2]);
    this->viewport->Select();


    switch(currentiteration)
    {
    case GRPTEXTURE_CUBEMAP_RIGHT:  this->camera->SetRotation(0.0f,     +90.0f, 0.0f);   break;
    case GRPTEXTURE_CUBEMAP_LEFT:   this->camera->SetRotation(0.0f,     -90.0f,   0.0f); break;

    case GRPTEXTURE_CUBEMAP_BOTTOM: this->camera->SetRotation(-90.0f,   +180.0f, 0.0f);  break;
    case GRPTEXTURE_CUBEMAP_TOP:    this->camera->SetRotation(+90.0f,   +180.0f, 0.0f);  break;

    case GRPTEXTURE_CUBEMAP_FRONT:  this->camera->SetRotation(0.0f,     0.0f,   0.0f);   break;
    case GRPTEXTURE_CUBEMAP_BACK:   this->camera->SetRotation(0.0f,     180.0f, 0.0f);   break;
    }


    this->camera->Update();
    this->camera->BuildLookAt();
};

/*-------------------------------------------------------------------
//  GRPSHADOWPOINTLIGHTPASS::Begin
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      27/07/2016 12:26:25
//
*/
/*-----------------------------------------------------------------*/
void GRPSHADOWPOINTLIGHTPASS::Begin()
{
    this->camera->rendertarget->SelectAsTarget(currentiteration);

  GRPPASS::Begin();
}


/*-------------------------------------------------------------------
//  GRPSHADOWPOINTLIGHTPASS::End
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      11/07/2017 20:13:33
//
*/
/*-----------------------------------------------------------------*/
void GRPSHADOWPOINTLIGHTPASS::End()
{
  GRPPASS::End();

  GRPOPENGLCTRL::CULLING::SetBackFaceCulling  (true,  GRPCULLMODE_BACK);

  this->light->rendertarget->Deselect();
}

