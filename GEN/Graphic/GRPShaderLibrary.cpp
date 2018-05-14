
/*------------------------------------------------------------------------------------------
//  GRPSHADERLIBRARY.CPP
//
//  Library for Vertex/Fragment shaders
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 01/10/2014 9:56:01
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPShader.h"
#include "GRPShaderLibrary.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/
//#define HIGHQUALITY

const char * GRPSHADERLIBRARY::ProgramList[][GRPSHADERLIBRARY_MAX]=
{
  { "NIL", "Null",              GRPSHADERLIBRARY::VertexSimple,                 GRPSHADERLIBRARY::NullShader,                                 "Null"                                                                },

  { "BLU", "Blue",              GRPSHADERLIBRARY::VertexSimple,                 GRPSHADERLIBRARY::BasicBlue,                                  "Basic Blue"                                                          },
  { "RED", "Red",               GRPSHADERLIBRARY::VertexSimple,                 GRPSHADERLIBRARY::BasicRed,                                   "Basic Red"                                                           },
  { "GRN", "Green",             GRPSHADERLIBRARY::VertexSimple,                 GRPSHADERLIBRARY::BasicGreen,                                 "Basic Green"                                                         },
  { "YLW", "Yellow",            GRPSHADERLIBRARY::VertexSimple,                 GRPSHADERLIBRARY::BasicYellow,                                "Basic Yellow"                                                        },
  { "PNK", "Pink",              GRPSHADERLIBRARY::VertexSimple,                 GRPSHADERLIBRARY::BasicPink,                                  "Basic Pink"                                                          },
  { "GRY", "Gray",              GRPSHADERLIBRARY::VertexSimple,                 GRPSHADERLIBRARY::BasicGray,                                  "Basic Gray"                                                          },
  { "BAS", "Basic",             GRPSHADERLIBRARY::VertexSimple,                 GRPSHADERLIBRARY::Basic,                                      "Basic"                                                               },
  { "BLK", "Black",             GRPSHADERLIBRARY::VertexSimple,                 GRPSHADERLIBRARY::BasicBlack,                                 "Basic Black" },

  { "STX", "Texture",           GRPSHADERLIBRARY::Vertex,                       GRPSHADERLIBRARY::SimpleTexture,                              "Simple Texture"                                                      },
  { "SDF", "Diffuse",           GRPSHADERLIBRARY::VertexSimple,                 GRPSHADERLIBRARY::DiffuseShader,                              "Simple Diffuse"                                                      },
  { "TXO", "TextureOperation",  GRPSHADERLIBRARY::VertexTextureOperation,       GRPSHADERLIBRARY::SimpleTexture,                              "Simple Texture" },


  { "NMP", "NormalMapping",     GRPSHADERLIBRARY::VertexShaderNormalMapping,    GRPSHADERLIBRARY::FragmentShaderNormalMapping,                "DOT per fragment Texture" },
  { "OPA", "Opacity",           GRPSHADERLIBRARY::Vertex,                       GRPSHADERLIBRARY::Opacity,                                    "Opacity" },
  { "VID", "Video",             GRPSHADERLIBRARY::Vertex,                       GRPSHADERLIBRARY::VideoRender,                                "Video" },
  { "PST", "PostProcessing",    GRPSHADERLIBRARY::PostProcessing,               GRPSHADERLIBRARY::SimpleTexture,                              "postProcess Show" },
  { "OVL", "Overlay",           GRPSHADERLIBRARY::PostProcessing,               GRPSHADERLIBRARY::Mask,                                       "Overlay Mask" },
  { "MSK", "Mask",              GRPSHADERLIBRARY::PostProcessing,               GRPSHADERLIBRARY::InverseMask,                                "Overlay Inverse Mask" },

  { "NRL", "Normal",            GRPSHADERLIBRARY::Vertex,                       GRPSHADERLIBRARY::Normal,                                     "Normals" },
  { "CUB", "CubeMap",           GRPSHADERLIBRARY::CubeMapVertex,                GRPSHADERLIBRARY::CubeMapFragment,                            "CubeMap" },
  { "TST", "Test",              GRPSHADERLIBRARY::PostProcessing,               GRPSHADERLIBRARY::Basic,                                      "postProcess Show" },

  { "BLR", "Blur",              GRPSHADERLIBRARY::PostProcessing,               GRPSHADERLIBRARY::Blur,                                       "blur" },


  //on GRPLoadingInterface.cpp
  { "LDR", "LoadingRing",       GRPSHADERLIBRARY::Vertex,                       GRPSHADERLIBRARY::LoadingRing,                                "Loading" },
  { "LDI", "LoadingBar",        GRPSHADERLIBRARY::Vertex,                       GRPSHADERLIBRARY::LoadingBar,                                 "Loading" },

  //on GRPSkyboxPass.cpp
  { "SKB", "Skybox",            GRPSHADERLIBRARY::SkyboxVertex,                 GRPSHADERLIBRARY::SkyboxFragment,                             "skybox" },

  //on GRPEnvMapPass.cpp
  { "RFD", "ReflectiveDiffuse", GRPSHADERLIBRARY::ReflectiveVertex,             GRPSHADERLIBRARY::ReflectiveFragmentDiffuse,                  "ReflectiveDiffuse" },
  { "RFT", "ReflectiveTexture", GRPSHADERLIBRARY::ReflectiveVertex,             GRPSHADERLIBRARY::ReflectiveFragmentTexture,                  "ReflectiveTexture" },

  //on GRPShaderDOT.cpp
#ifdef HIGHQUALITY
  { "DDF", "DOTDiff",           GRPSHADERLIBRARY::VertexShaderDOTPerFragment,   GRPSHADERLIBRARY::DiffuseFragmentShaderDOTPerFragment,        "DOT per vertex Diffuse" },
  { "DOT", "DOT3",              GRPSHADERLIBRARY::VertexShaderDOTPerFragment,   GRPSHADERLIBRARY::FragmentShaderDOTPerFragment,               "DOT per vertex Texture" },
  { "DDP", "DOTDiffPerFragment",GRPSHADERLIBRARY::VertexShaderDOTPerFragment,   GRPSHADERLIBRARY::DiffuseFragmentShaderDOTPerFragment,        "DOT per fragment Diffuse" },
  { "DDT", "DOT3PerFragment",   GRPSHADERLIBRARY::VertexShaderDOTPerFragment,   GRPSHADERLIBRARY::FragmentShaderDOTPerFragment,               "DOT per fragment Texture" },
  { "DIT", "DOT3Int",           GRPSHADERLIBRARY::VertexShaderDOT,              GRPSHADERLIBRARY::FragmentShaderDOTTextureInterpolated,       "DOT per vertex Texture Interpolated" },
#else
  { "DDF", "DOTDiff",           GRPSHADERLIBRARY::VertexShaderDOTDiff,          GRPSHADERLIBRARY::FragmentShaderDOTDiffuse,                   "DOT per vertex Diffuse"                                              },
  { "DOT", "DOT3",              GRPSHADERLIBRARY::VertexShaderDOT,              GRPSHADERLIBRARY::FragmentShaderDOTTexture,                   "DOT per vertex Texture"                                              },
  { "DDP", "DOTDiffPerFragment",GRPSHADERLIBRARY::VertexShaderDOTPerFragment,   GRPSHADERLIBRARY::DiffuseFragmentShaderDOTPerFragment,        "DOT per fragment Diffuse"                                              },
  { "DDT", "DOT3PerFragment",   GRPSHADERLIBRARY::VertexShaderDOTPerFragment,   GRPSHADERLIBRARY::FragmentShaderDOTPerFragment,               "DOT per fragment Texture"                                              },
  { "DIT", "DOT3Int",           GRPSHADERLIBRARY::VertexShaderDOT,              GRPSHADERLIBRARY::FragmentShaderDOTTextureInterpolated,       "DOT per vertex Texture Interpolated"                                 },
#endif

  //on GRPShadowPass
  { "LSP", "LightSpace",        GRPSHADERLIBRARY::LightSpaceVertex,             GRPSHADERLIBRARY::LightSpaceFragment,                         "LightSpace"},
  { "SHC", "ShadowCasting",     GRPSHADERLIBRARY::ShadowCastingVertex,          GRPSHADERLIBRARY::ShadowCastingFragment,                      "ShadowCast"},
  { "SCF", "ShadowCastingDiffuse",  GRPSHADERLIBRARY::ShadowCastingVertex,      GRPSHADERLIBRARY::ShadowCastingFragmentDiffuse,               "ShadowCastDiffuse"},

  //on GRPDeformer.cpp
#ifdef GRPDEFORMERS_ACTIVE
  { "MPH", "Morph",             GRPSHADERLIBRARY::Morph,                        GRPSHADERLIBRARY::SimpleTexture,                              "Morphing"},
  { "MPD", "MorphDiffuse",      GRPSHADERLIBRARY::Morph,                        GRPSHADERLIBRARY::FragmentShaderDOTDiffuse,                   "Morphing Diffuse"},
#endif

  //on GRPSkinning.cpp
  { "SKD", "SkinningDiffuse",   GRPSHADERLIBRARY::SkinVertex,                   GRPSHADERLIBRARY::DiffuseFragmentShaderDOTPerFragment,        "Skinning Diffuse"},
  { "SKN", "Skinning",          GRPSHADERLIBRARY::SkinVertex,                   GRPSHADERLIBRARY::FragmentShaderDOTTexture,                   "Skinning"},

  { "INV", "Invert",            GRPSHADERLIBRARY::PostProcessing,               GRPSHADERLIBRARY::Invert,                                     "Invert" },
  { "HDR", "hdr",               GRPSHADERLIBRARY::PostProcessing,               GRPSHADERLIBRARY::Hdr,                                        "HDR" },

  { "2DS", "2DShadow",          GRPSHADERLIBRARY::Vertex,                       GRPSHADERLIBRARY::Gradient,                                   "Gradient for shadows" },


  NULL
};


GRPSHADERLIBRARY* GRPSHADERLIBRARY::Instance=NULL;


/*-------------------------------------------------------------------
//  GRPSHADERLIBRARY::Get
*/
/**
//
//  singleton
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      07/09/2016 10:48:09
//
//  @return       GRPSHADERLIBRARY* :
//
*/
/*-----------------------------------------------------------------*/
GRPSHADERLIBRARY* GRPSHADERLIBRARY::Get()
{
  if (Instance==NULL)
      Instance=new GRPSHADERLIBRARY();
  return Instance;
}

/*-------------------------------------------------------------------
//  GRPSHADERLIBRARY::Destroy
*/
/**
//
//  Destroys library
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      19/09/2016 13:55:01
//
*/
/*-----------------------------------------------------------------*/
void GRPSHADERLIBRARY::Destroy()
{
  if (GRPSHADERLIBRARY::Instance!=NULL)
  delete(GRPSHADERLIBRARY::Instance);

  GRPSHADERLIBRARY::Instance=NULL;
}

/*-------------------------------------------------------------------
//  GRPSHADERLIBRARY::GRPSHADERLIBRARY
*/
/**
//
//  Class Constructor GRPSHADERLIBRARY
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      07/09/2016 10:48:16
//
*/
/*-----------------------------------------------------------------*/
GRPSHADERLIBRARY::GRPSHADERLIBRARY()
{

};


/*-------------------------------------------------------------------
//  GRPSHADERLIBRARY::~GRPSHADERLIBRARY
*/
/**
//
//   Class Destructor GRPSHADERLIBRARY
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      07/09/2016 10:48:21
//
*/
/*-----------------------------------------------------------------*/
GRPSHADERLIBRARY::~GRPSHADERLIBRARY()
{
  CompiledPrograms.DeleteContents();
  CompiledPrograms.DeleteAll();
  Instance=NULL;
}

/*-------------------------------------------------------------------
//  GRPSHADERLIBRARY::LoadShaders
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      07/09/2016 10:48:31
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool  GRPSHADERLIBRARY::LoadShaders()
{
  int e=0;
  if (this->CompiledPrograms.GetSize()==0)
    while (ProgramList[e][0]!=0)
    {
    LoadShader( e,
                (char*)ProgramList[e][GRPSHADERLIBRARY_ACRONYM],
                (char*)ProgramList[e][GRPSHADERLIBRARY_NAME],
                (char*)ProgramList[e][GRPSHADERLIBRARY_VERTEXSHADER],
                (char*)ProgramList[e][GRPSHADERLIBRARY_FRAGMENTSHADER],
                (char*)ProgramList[e][GRPSHADERLIBRARY_DESCRIPTION]);
    e++;
    }

  return true;
}

/*-------------------------------------------------------------------
//  FRAMERENDER::LoadShader
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      15/09/2014 10:48:35
//
//  @return       bool :
//
//  @param        VertexShaderSTR :
//  @param        FragmentShaderSTR :
*/
/*-----------------------------------------------------------------*/
bool GRPSHADERLIBRARY::LoadShader (int index, char key[],char name[],char VertexShaderSTR[],char FragmentShaderSTR[],char Description[])
{
  OPENGLCHECKERROR(__L("LoadShader"));
  XSTRING string;

  GRPSHADERPROGRAM*   program=new GRPSHADERPROGRAM();
                      if (program)
                      {
                      program->AddShader(new GRPSHADER(GRPSHADERPROGRAMTYPE_VERTEX,     XSTRING(VertexShaderSTR).Get()));
                      program->AddShader(new GRPSHADER(GRPSHADERPROGRAMTYPE_FRAGMENT,   XSTRING(FragmentShaderSTR).Get()));
                      program->Description=Description;
                      program->SetKey(key);

                      XSTRING sname;
                              sname.Set(name);
                      program->SetName(sname.Get());
                      program->SetIndex(index);

                      CompiledPrograms.Add(program);
                      }


  /*
  GRPSHADER* vertexshader = new GRPSHADER();
  if(vertexshader)
    {
      string = VertexShaderSTR;
      vertexshader->Set(GRPSHADERPROGRAMTYPE_VERTEX, string);
      OPENGLCHECKERROR(__L("LoadShader::vertexshader->Set"));

      GRPSHADER* fragmentshader = new GRPSHADER();
      if(fragmentshader)
        {
          string=FragmentShaderSTR;
          fragmentshader->Set(GRPSHADERPROGRAMTYPE_FRAGMENT, string);
          OPENGLCHECKERROR(__L("LoadShader::fragmentshader->Set"));

          GRPSHADERPROGRAM* shader = new GRPSHADERPROGRAM();
          if(shader)
            {
              memcpy(shader->name, key, 3*sizeof(char));
              shader->Create(vertexshader, fragmentshader,Description);
              CompiledPrograms.Add(shader);
              OPENGLCHECKERROR(__L("LoadShader::program->shader Create"));
            }

          delete(vertexshader);
          vertexshader=NULL;
        }

      delete(fragmentshader);
      fragmentshader=NULL;
    }
  */

  return true;
}


/*-------------------------------------------------------------------
//  GRPSHADERLIBRARY::GetCompiledPrograms
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      07/09/2016 11:05:55
//
//  @return       XVECTOR<GRPSHADERPROGRAM*>* :
//
*/
/*-----------------------------------------------------------------*/
XVECTOR<GRPSHADERPROGRAM*>* GRPSHADERLIBRARY::GetCompiledPrograms()
{
  return &CompiledPrograms;
}


/*-------------------------------------------------------------------
//  GRPSHADERLIBRARY::GetProgramByName
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/05/2017 16:20:05
//
//  @return       GRPSHADERPROGRAM* :
//
//  @param        name :
*/
/*-----------------------------------------------------------------*/
GRPSHADERPROGRAM*   GRPSHADERLIBRARY::GetProgramByName    (XCHAR* name)
{
  for (XDWORD e=0;e<this->CompiledPrograms.GetSize();e++)
  {
    GRPSHADERPROGRAM* program=this->CompiledPrograms.Get(e);
    if (program)
    {
      if (XSTRING::Compare(program->GetName()->Get(),name)==0)
        return program;
    }
  }
  return NULL;
}


/*-------------------------------------------------------------------
//  GRPSHADERLIBRARY::GetProgramByKey
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      01/12/2014 13:46:33
//
//  @return       GLUINT :
//
//  @param        key[3] :
*/
/*-----------------------------------------------------------------*/
GRPSHADERPROGRAM*   GRPSHADERLIBRARY::GetProgramByKey(char key[3])
  {
    XDWORD index=GRPSHADERLIBRARY::CompiledPrograms.GetSize();

      if (index==0)
        return NULL;

      for (int e = 0; e < (int)index; e++)
      {
        if (ProgramList[e][0] == NULL) break;

        if (ProgramList[e][GRPSHADERLIBRARY_ACRONYM][0] == key[0] &&
            ProgramList[e][GRPSHADERLIBRARY_ACRONYM][1] == key[1] &&
            ProgramList[e][GRPSHADERLIBRARY_ACRONYM][2] == key[2])
        {
            return GetProgram(e);
        }
      }

      XDEBUG_PRINTCOLOR(0,__L("GRPSHADERLIBRARY::GetProgramByKey : Key not found %c%c%c, returning %d instead"),key[0],key[1],key[2], GRPSHADERLIBRARY::CompiledPrograms.Get(0)->GetID());
      return GRPSHADERLIBRARY::CompiledPrograms.Get(0);
  }

/*-------------------------------------------------------------------
//  GRPSHADERLIBRARY::GetProgram
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      01/12/2014 13:46:38
//
//  @return       GLUINT :
//
//  @param        i :
*/
/*-----------------------------------------------------------------*/
GRPSHADERPROGRAM*   GRPSHADERLIBRARY::GetProgram(XDWORD i)
    {
      XDWORD index=GRPSHADERLIBRARY::CompiledPrograms.GetSize();

      if (index==0)
        return NULL;

      GRPSHADERPROGRAM* p=0;

      if (i>=index)
        p=GRPSHADERLIBRARY::CompiledPrograms.GetLast();
      else
        p=GRPSHADERLIBRARY::CompiledPrograms.Get(i);

      return p;
    }
const char GRPSHADERLIBRARY::NullShader[] =
                            "\nvoid main()                    \n"
                            "{                              \n"
                            " discard;                      \n"
                            "}                              \n";
const char GRPSHADERLIBRARY::Basic[] =
                            "void main()                    \n"
                            "{                              \n"
                            "   gl_FragColor =vec4(1.0,1.0,1.0,1.0); \n"
                            "}                              \n";
const char GRPSHADERLIBRARY::BasicBlack[] =
                            "void main()                    \n"
                            "{                              \n"
                            "   gl_FragColor =vec4(0.0,0.0,0.0,1.0); \n"
                            "}                              \n";
const char GRPSHADERLIBRARY::BasicRed[] =
                            "void main()                    \n"
                            "{                              \n"
                            "   gl_FragColor =vec4(1.0,0.3,0.3,1.0); \n"
                            "}                              \n";

const char GRPSHADERLIBRARY::BasicBlue[] =
                            "void main()                    \n"
                            "{                              \n"
                            "   gl_FragColor =vec4(0.3,0.3,1.0,1.0); \n"
                            "}                              \n";

const char GRPSHADERLIBRARY::BasicGreen[] =
                            "void main()                    \n"
                            "{                              \n"
                            "   gl_FragColor =vec4(0.3,1.0,0.3,1.0); \n"
                            "}                              \n";

const char GRPSHADERLIBRARY::BasicYellow[] =
                            "void main()                    \n"
                            "{                              \n"
                            "   gl_FragColor =vec4(1.0,0.6,0.2,1.0); \n"
                            "}                              \n";

const char GRPSHADERLIBRARY::BasicPink[] =
                            "void main()                    \n"
                            "{                              \n"
                            "   gl_FragColor =vec4(1.0,0.7,1.0,1.0); \n"
                            "}                              \n";

const char GRPSHADERLIBRARY::BasicGray[] =
                            "void main()                    \n"
                            "{                              \n"
                            "   gl_FragColor =vec4(0.4,0.4,0.4,1.0); \n"
                            "}                              \n";



const char GRPSHADERLIBRARY::PostProcessing[] =
                            "attribute vec3 a_position;     \n"
                            "attribute vec2 a_texCoord;     \n"

                            "varying vec4 v_position;       \n"
                            "varying vec3 v_normal;         \n"
                            "varying vec2 v_texCoord;       \n"

                            "uniform sampler2D  s_texture;  \n"

                            "void main()                    \n"
                            "{                              \n"
                            "   v_position  = vec4(a_position,1.0);           \n"
                            "   v_texCoord  = a_texCoord;   \n"
                            "   gl_Position = v_position.xyww;    \n"
                            "}                              \n";




const char GRPSHADERLIBRARY::SimpleTexture[] =
                            "varying vec2 v_texCoord;\n"
                            "uniform sampler2D s_texture; \n"
                            "uniform mat4 materialDataMat;\n"

                            "void main()                                    \n"
                            "{          \n"
                              "   gl_FragColor=texture2D( s_texture, v_texCoord )*materialDataMat[3][1];\n"
                            " }                                             \n";



const char GRPSHADERLIBRARY::Normal[] =
                          "varying vec3 v_normal;\n"

                          "void main(void)\n"
                          "{\n"
                          "  gl_FragColor =vec4(v_normal.x,v_normal.y,v_normal.z,1.0); \n"
                          "};\n";


const char GRPSHADERLIBRARY::VideoRender[]=
  "uniform sampler2D textureChannel1; \n"
  "uniform sampler2D textureChannel2; \n"
  "uniform sampler2D textureChannel3; \n"
  "varying vec2 v_texCoord;\n"
  "void main() {\n"
  "vec2 t = v_texCoord;\n"
  "t.y = 1.0-t.y;\n"
  "float Y = texture2D(textureChannel1,t).r;\n"
  "float U = texture2D(textureChannel2,t).r;\n"
  "float V = texture2D(textureChannel3,t).r;\n"
  "vec4 color = vec4(Y, U, V, 1.0);\n"
  "vec4 rgbc = vec4(0.0, 0.0, 0.0, 1.0);\n"
  "U = U-0.5;\n"
  "V = V-0.5;\n"
  "rgbc.r = 2.0*(Y/2.0 + 1.402/2.0 * V);\n"
  "rgbc.g = 2.0*(Y/2.0 - 0.344136 * U/2.0 - 0.714136 * V/2.0);\n"
  "rgbc.b = 2.0*(Y/2.0 + 1.773/2.0 * U);\n"
  "    gl_FragColor   =   rgbc; \n"
  "}\n";

const char GRPSHADERLIBRARY::Opacity  []=
"uniform sampler2D textureChannel3; \n"
"uniform sampler2D s_texture; \n"
"varying vec2 v_texCoord;\n"

"void main() {\n"
"    vec4 alpha=texture2D(textureChannel3,v_texCoord);\n"
"    vec4 color=texture2D(s_texture,v_texCoord);\n"
"    gl_FragColor   =   vec4(alpha*color); \n"
"}\n";

const char GRPSHADERLIBRARY::Mask[] =
"uniform sampler2D textureChannel6; \n"
"uniform sampler2D s_texture; \n"
"varying vec2 v_texCoord;\n"

"void main() {\n"
"    vec4 mask=texture2D(textureChannel6,v_texCoord);\n"
"   if (mask.r<=0.5) \n"
"           discard; \n"

"    vec4 color=texture2D(s_texture,v_texCoord);\n"
"    gl_FragColor = color;\n"
"}\n";

const char GRPSHADERLIBRARY::Blur[] =
//in attributes from our vertex shader\n"
"varying vec2 v_texCoord;\n"

//declare uniforms
"uniform sampler2D s_texture;\n"

"void main() {\n"
//this will be our RGBA sum
"   vec4 sum = vec4(0.0);\n"
"   vec2 dir = vec2(1.0,0.0);\n"

//our original texcoord for this fragment
"   vec2 tc = v_texCoord;\n"

//the amount to blur, i.e. how far off center to sample from
//1.0 -> blur by one pixel
//2.0 -> blur by two pixels, etc.
"   float blur = 2.0/512.0; \n" //radius / resolution;\n"

//the direction of our blur
//(1.0, 0.0) -> x-axis blur
//(0.0, 1.0) -> y-axis blur
"   float hstep = dir.x;\n"
"   float vstep = dir.y;\n"

//apply blurring, using a 9-tap filter with predefined gaussian weights

"   sum += texture2D(s_texture, vec2(tc.x - 4.0*blur*hstep, tc.y - 4.0*blur*vstep)) * 0.0162162162;\n"
"   sum += texture2D(s_texture, vec2(tc.x - 3.0*blur*hstep, tc.y - 3.0*blur*vstep)) * 0.0540540541;\n"
"   sum += texture2D(s_texture, vec2(tc.x - 2.0*blur*hstep, tc.y - 2.0*blur*vstep)) * 0.1216216216;\n"
"   sum += texture2D(s_texture, vec2(tc.x - 1.0*blur*hstep, tc.y - 1.0*blur*vstep)) * 0.1945945946;\n"

"   sum += texture2D(s_texture, vec2(tc.x, tc.y)) * 0.2270270270;\n"

"   sum += texture2D(s_texture, vec2(tc.x + 1.0*blur*hstep, tc.y + 1.0*blur*vstep)) * 0.1945945946;\n"
"   sum += texture2D(s_texture, vec2(tc.x + 2.0*blur*hstep, tc.y + 2.0*blur*vstep)) * 0.1216216216;\n"
"   sum += texture2D(s_texture, vec2(tc.x + 3.0*blur*hstep, tc.y + 3.0*blur*vstep)) * 0.0540540541;\n"
"   sum += texture2D(s_texture, vec2(tc.x + 4.0*blur*hstep, tc.y + 4.0*blur*vstep)) * 0.0162162162;\n"

//discard alpha for our simple demo, multiply by vertex color and return
"   gl_FragColor = vec4(sum.rgb, 1.0);\n"
"}\n";

const char GRPSHADERLIBRARY::InverseMask[] =
"uniform sampler2D textureChannel6; \n"
"uniform sampler2D s_texture; \n"
"varying vec2 v_texCoord;\n"

"void main() {\n"
"    vec4 mask=texture2D(textureChannel6,v_texCoord);\n"
"   if (mask.r>=0.5) \n"
"           discard; \n"

"    vec4 color=texture2D(s_texture,v_texCoord);\n"
"    gl_FragColor = color;\n"
"}\n";


const char GRPSHADERLIBRARY::CubeMapVertex[] =
"attribute vec3 a_position;\n"
"attribute vec3 a_normal;\n"
"attribute vec2 a_texCoord;\n"
"uniform samplerCube s_texture;               \n"

"varying vec4 v_position;\n"
"varying vec2 v_texCoord;\n"
"varying vec3 v_normal;\n"

"uniform mat4 modelMat;\n"
"uniform mat4 invViewMat;\n"
"uniform mat4 modelviewMat;\n"
"uniform mat4 invmodelviewMat;\n"
"uniform mat4 modelviewprojectionMat;\n"

"void main(){\n"

"    v_position         = modelviewprojectionMat * vec4(a_position, 1.0);         \n"
"    v_normal           = (modelviewMat*vec4(a_normal, 0.0)).xyz;               \n"
"    v_texCoord         = a_texCoord;                                             \n"

"    gl_Position        = v_position;         \n"
"}";

const char GRPSHADERLIBRARY::CubeMapFragment[] =
"uniform samplerCube s_texture;               \n"
"varying vec2 v_texCoord;                     \n"
"varying vec3 v_normal;                       \n"
"varying vec4 v_position;\n"
"uniform mat4 invViewMat;\n"

"void main() {\n"
"  vec3 r=reflect((v_position.xyz), normalize(v_normal.xyz)).xyz;\n"
"       r=(invViewMat * vec4(r, 0.0)).xyz;\n"
"  gl_FragColor   = textureCube(s_texture,normalize(r));  \n"
"}\n";


const char GRPSHADERLIBRARY::Invert[] =
"uniform sampler2D s_texture; \n"
"varying vec2 v_texCoord;\n"

"void main() {\n"
"    vec4 color=texture2D(s_texture,v_texCoord);\n"
"    gl_FragColor = vec4(1.0-color.r,1.0-color.g,1.0-color.b,1.0);\n"
"}\n";

const char GRPSHADERLIBRARY::Hdr[] =
"varying vec2 v_texCoord;\n"

"uniform sampler2D s_texture;\n"
"uniform float gamma;\n"
"uniform float exposure;\n"

"void main()\n"
"{\n"
"   vec3 hdrColor = texture2D(s_texture, v_texCoord).rgb;\n"
// reinhard
//"  vec3 result = hdrColor / (hdrColor + vec3(1.0)); \n"
// exposure
"       vec3 result = vec3(1.0) - exp(-hdrColor * exposure); \n"
// also gamma correct while we're at it
//" float exponent=1.0/gamma;\n" //precompute it please
"       result = pow(result, vec3(gamma)); \n"
"       gl_FragColor = vec4(result, 1.0); \n"
"}\n";

const char GRPSHADERLIBRARY::Gradient[] =
"varying vec2 v_texCoord;\n"
"uniform mat4 materialDataMat;  \n"
"uniform float gradient_factor; \n"

"void main()\n"
"{\n"
"       float x = smoothstep(gradient_factor, 1.0, v_texCoord.x);"
"       gl_FragColor = mix(materialDataMat[0],vec4(materialDataMat[0].rgb,0.0),x); \n"
"};\n";
