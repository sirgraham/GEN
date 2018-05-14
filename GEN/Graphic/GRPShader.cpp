
/*------------------------------------------------------------------------------------------
//  GRPSHADER.CPP
//
//  Shader and Fragment wrapper
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 21/03/2014 13:23:17
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XFactory.h"
#include "XFileTXT.h"
#include "XDebug.h"

#include "GRPShader.h"
#include "GRPProfiler.h"
#include "GRPTexture.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/

GRPSHADERPROGRAM* GRPSHADER::CurrentProgram=NULL;
GLUINT            GRPSHADER::CurrentShader=-1;
GLUINT            GRPSHADER::LastShader=-1;
GLUINT            GRPSHADER::MaxLights=4;

/*-------------------------------------------------------------------
//  GRPSHADER::UseShader
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      12/09/2016 16:08:53
//
//  @return       GLUINT :
//
//  @param        s :
*/
/*-----------------------------------------------------------------*/
GLUINT GRPSHADER::UseShader(GRPSHADERPROGRAM* s)
{
    OPENGLCHECKERROR(__L("> GRPSHADER::UseShader"));
  if (s!=NULL)
  {
    GLINT id=s->GetID();
    if (id<0)
      return 0;

    if (id!=CurrentShader)
    {
      CurrentShader=id;
      glUseProgram(id);
      GRPPROFILER::nShaderChanges++;
    }
    CurrentProgram=s;
  }


  return CurrentShader;
}

/*-------------------------------------------------------------------
//  GRPSHADER::UseShader
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      12/09/2016 16:08:46
//
//  @return       GLUINT :
//
//  @param        s :
*/
/*-----------------------------------------------------------------*/
GLUINT GRPSHADER::UseShader(GLUINT s)
{
  if (s!=CurrentShader)
  {
    CurrentShader=s;
    CurrentProgram=NULL;
    glUseProgram(s);
    GRPPROFILER::nShaderChanges++;
  }
  return CurrentShader;
}

/*-------------------------------------------------------------------
//  GRPSHADER::UseTmpShader
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      12/09/2016 16:08:38
//
//  @return       GLUINT :
//
//  @param        s :
*/
/*-----------------------------------------------------------------*/
GLUINT GRPSHADER::UseTmpShader(GLUINT s)
{
  if (s!=CurrentShader)
  glUseProgram(s);
  GRPPROFILER::nShaderChanges++;
  LastShader=s;
  return CurrentShader;
}

/*-------------------------------------------------------------------
//  GRPSHADER::PopShader
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      12/09/2016 16:08:31
//
//  @return       GLUINT :
//
*/
/*-----------------------------------------------------------------*/
GLUINT GRPSHADER::PopShader()
{
  if (LastShader!=CurrentShader)
  glUseProgram(CurrentShader);
  GRPPROFILER::nShaderChanges++;
  return CurrentShader;
}



/*-------------------------------------------------------------------
//  GRPSHADER::GRPSHADER
*/
/**
//
//  Class Constructor GRPSHADER
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/05/2014 12:28:10
//

*/
/*-----------------------------------------------------------------*/
GRPSHADER::GRPSHADER(GRPSHADERPROGRAMTYPE type, XCHAR* source)
{
  Clean();
  this->shadertype=type;
  this->source    =source;
}



/*-------------------------------------------------------------------
//  GRPSHADER::~GRPSHADER
*/
/**
//
//   Class Destructor GRPSHADER
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/05/2014 12:28:29
//
*/
/*-----------------------------------------------------------------*/
GRPSHADER::~GRPSHADER()
{
  if (this->ID)
  glDeleteShader(this->ID);
  Clean();

}


/*-------------------------------------------------------------------
//  GRPSHADER::GetID
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      12/09/2016 16:39:05
//
//  @return       GLUINT :
//
*/
/*-----------------------------------------------------------------*/
GLUINT GRPSHADER::GetID()
{
  if (!IsLoaded)
  {
    Compile();
  }
  return ID;
}

/*-------------------------------------------------------------------
//  GRPSHADER::Compile
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      13/09/2016 10:35:33
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool GRPSHADER::Compile()
{
  GLint compiled;

  Set(shadertype,source);

  ID = glCreateShader(shadertype);

  XSTRING_CREATEOEM(source, charstr);
  glShaderSource(ID, 1, (const GLchar**)&charstr, 0);

  glCompileShader(ID);
  glGetShaderiv(ID, GL_COMPILE_STATUS, &compiled);

  char      errorcompiler[500] = {'\0'};
  XSTRING   errorstring;

  glGetShaderInfoLog(ID, 500, 0, errorcompiler);
  errorstring = errorcompiler;

  if (errorstring.GetSize()==0) //some compilers doesnt give a success response, but a NULL
    errorstring.Set(__L("Success"));

  XDEBUG_PRINTCOLOR(1,__L("[%d] %s Shader Compilation : %s"),ID, (shadertype == GL_FRAGMENT_SHADER)?__L("Fragment"):__L("Vertex"), errorstring.Get());

  if (compiled!=GL_TRUE)
  {
    int errorline=0;
    XFSTRING errorstring(errorcompiler);
    int i=0;
    errorstring.scan(__L("%d(%d)"),&i,&errorline);
    int e=0;
    char* linea=new char[strlen(charstr)];
    int nlinea=1;
    int a=0;
    //vamos a mostrar el shader marcando la linea
    while (charstr[e]!=0)
    {
      if (charstr[e]=='\n')
      {
        linea[a]='\0';
        int lvl=(nlinea==errorline)? 4 : 0;
        if (nlinea>errorline-3 && nlinea<errorline+3)
          {
            XDEBUG_PRINTCOLOR(lvl,__L("\t\t[%d] %s"),nlinea,XSTRING(linea).Get());
          }
        nlinea++;
        a=0;
      }
      else
      linea[a++]=charstr[e];
      e++;
    }
    delete(linea);
    linea=NULL;
  }

  XSTRING_DELETEOEM(charstr);

  return true;
}

/*-------------------------------------------------------------------
//  GRPSHADER::Set
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/05/2014 13:08:58
//
//  @return       bool :
//
//  @param        shadertype :
//  @param        source :
*/
/*-----------------------------------------------------------------*/
bool GRPSHADER::Set(GRPSHADERPROGRAMTYPE shadertype, XSTRING& _source)
{
  if(_source.IsEmpty()) return false;

  XSTRING version;
  version.Format(__L("#version %d\n\n"),GRPSHADERVERSION);



  XSTRING precisionString;
  precisionString.Set(__L("precision highp float;\n\n"));

#ifndef HW_PC
  source.Insert(precisionString,0);
#endif
  source.Insert(version, 0);

  return true;
}

/*-------------------------------------------------------------------
//  GRPSHADER::Set
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/05/2014 14:15:20
//
//  @return       bool :
//
//  @param        shadertype :
//  @param        source :
*/
/*-----------------------------------------------------------------*/
bool GRPSHADER::Set(GRPSHADERPROGRAMTYPE shadertype, XCHAR* source)
{
  if(!source) return false;

  XSTRING _source;

  _source = source;

  return Set(shadertype, _source);
}








/*-------------------------------------------------------------------
//  GRPSHADERPROGRAM::GRPSHADERPROGRAM
*/
/**
//
//  Class Constructor GRPSHADERPROGRAM
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/05/2014 14:32:34
//
*/
/*-----------------------------------------------------------------*/
GRPSHADERPROGRAM::GRPSHADERPROGRAM()
{
  Clean();
}



/*-------------------------------------------------------------------
//  GRPSHADERPROGRAM::~GRPSHADERPROGRAM
*/
/**
//
//   Class Destructor GRPSHADERPROGRAM
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/05/2014 14:32:38
//
*/
/*-----------------------------------------------------------------*/
GRPSHADERPROGRAM::~GRPSHADERPROGRAM()
{

  OPENGLCHECKERROR(__L("GRPSHADERPROGRAM::Destroy"));

  if (this->IsLoaded && this->id!=0)
  {
  //glDeleteProgram(this->id);
  OPENGLCHECKERROR(__L("GRPSHADERPROGRAM::Deleted"));
  }

  this->shaders.DeleteElementContents();

  Clean();

}


/*-------------------------------------------------------------------
//  GRPSHADERPROGRAM::AddShader
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      12/09/2016 16:26:04
//
//  @return       bool :
//
//  @param        shader :
*/
/*-----------------------------------------------------------------*/
bool GRPSHADERPROGRAM::AddShader(GRPSHADER* shader)
{
  return this->shaders.Add(shader->GetType(),shader);
}

/*-------------------------------------------------------------------
//  GRPSHADERPROGRAM::Create
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/05/2014 13:49:13
//
//  @return       bool :
//
//  @param        vertexshader :
//  @param        fragmentshader :
*/
/*-----------------------------------------------------------------*/
bool GRPSHADERPROGRAM::Create(GRPSHADER* vertexshader, GRPSHADER* fragmentshader, char Description[])
{
  OPENGLCHECKERROR(__L("LoadShader::Create"));

  if(!vertexshader || !fragmentshader) return false;

  this->vertexshader    = vertexshader;
  this->fragmentshader  = fragmentshader;
  this->Description     = Description;

  Compile();

  return true;
}

/*-------------------------------------------------------------------
//  GRPSHADERPROGRAM::GetID
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      12/09/2016 14:50:12
//
//  @return       GLUINT :
//
*/
/*-----------------------------------------------------------------*/
GLUINT GRPSHADERPROGRAM::GetID()
{
  if (!IsLoaded)
    Compile();
  return id;
}


/*-------------------------------------------------------------------
//  GRPSHADERPROGRAM::SetIndex
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      22/09/2016 13:46:03
//
//  @param        index :
*/
/*-----------------------------------------------------------------*/
void GRPSHADERPROGRAM::SetIndex(XDWORD index)
{
  this->index=index;
}

/*-------------------------------------------------------------------
//  GRPSHADERPROGRAM::GetIndex
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      22/09/2016 13:46:12
//
//  @return       XDWORD :
//
*/
/*-----------------------------------------------------------------*/
XDWORD GRPSHADERPROGRAM::GetIndex()
{
  return this->index;
}

/*-------------------------------------------------------------------
//  GRPSHADERPROGRAM::Compile
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      22/09/2016 13:46:19
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool GRPSHADERPROGRAM::Compile()
{
  XDEBUG_PRINTCOLOR(0,__L("GRPSHADERPROGRAM::Compile() %s"),this->name.Get());

  OPENGLCHECKERROR(__L("> LoadShader::glCreateProgram"));

  //shaders.Get(GRPSHADERPROGRAMTYPE_VERTEX)->GetID();
  //shaders.Get(GRPSHADERPROGRAMTYPE_FRAGMENT)->GetID();

  id = glCreateProgram();
  if (id==0)
    return false;

  OPENGLCHECKERROR(__L("LoadShader::glCreateProgram"));

  //------------ locations
  int n=0;

  glBindAttribLocation(id,GRPVERTEX_CHANNEL_POSITION,       "a_position");
  glBindAttribLocation(id,GRPVERTEX_CHANNEL_NORMAL,         "a_normal");

#ifdef GRPVERTEX_EXTENDED
  glBindAttribLocation(id,GRPVERTEX_CHANNEL_BINORMAL,       "a_binormal");
  glBindAttribLocation(id,GRPVERTEX_CHANNEL_TANGENT,        "a_tangent");
#endif

  glBindAttribLocation(id,GRPVERTEX_CHANNEL_TEXTURE,        "a_texCoord");
  //glBindAttribLocation(id,n++,"a_instanceID");

  OPENGLCHECKERROR(__L("LoadShader::Vertex::Channels"));

  glBindAttribLocation(id, GRPVERTEX_CHANNEL_BONE_INDEX,   "a_boneIndices");
  glBindAttribLocation(id, GRPVERTEX_CHANNEL_BONE_WEIGHTS, "a_boneWeights");

  OPENGLCHECKERROR(__L("LoadShader::Skin::Attributes"));

#ifdef GRPDEFORMERS_ACTIVE

  if (GRPOPENGLCTRL::VBO::MaxAttributes>GRPVERTEX_CHANNEL_MORPH_POSITION1)
  {
  glBindAttribLocation(id,GRPVERTEX_CHANNEL_MORPH_POSITION1,"a_positionMorph1");    OPENGLCHECKERROR(__L("LoadShader::Morph::Attributes 1"));
  }
  if (GRPOPENGLCTRL::VBO::MaxAttributes>GRPVERTEX_CHANNEL_MORPH_POSITION2)
  {
  glBindAttribLocation(id,GRPVERTEX_CHANNEL_MORPH_POSITION2,"a_positionMorph2");    OPENGLCHECKERROR(__L("LoadShader::Morph::Attributes 2"));
  }
  if (GRPOPENGLCTRL::VBO::MaxAttributes>GRPVERTEX_CHANNEL_MORPH_POSITION3)
  {
  glBindAttribLocation(id,GRPVERTEX_CHANNEL_MORPH_POSITION3,"a_positionMorph3");    OPENGLCHECKERROR(__L("LoadShader::Morph::Attributes 3"));
  }
  if (GRPOPENGLCTRL::VBO::MaxAttributes>GRPVERTEX_CHANNEL_MORPH_POSITION4)
  {
  glBindAttribLocation(id,GRPVERTEX_CHANNEL_MORPH_POSITION4,"a_positionMorph4");    OPENGLCHECKERROR(__L("LoadShader::Morph::Attributes 4"));
  }
  if (GRPOPENGLCTRL::VBO::MaxAttributes>GRPVERTEX_CHANNEL_MORPH_POSITION5)
  {
  glBindAttribLocation(id,GRPVERTEX_CHANNEL_MORPH_POSITION5,"a_positionMorph5");    OPENGLCHECKERROR(__L("LoadShader::Morph::Attributes 5"));
  }
  if (GRPOPENGLCTRL::VBO::MaxAttributes>GRPVERTEX_CHANNEL_MORPH_POSITION6)
  {
  glBindAttribLocation(id,GRPVERTEX_CHANNEL_MORPH_POSITION6,"a_positionMorph6");    OPENGLCHECKERROR(__L("LoadShader::Morph::Attributes 6"));
  }
  if (GRPOPENGLCTRL::VBO::MaxAttributes>GRPVERTEX_CHANNEL_MORPH_POSITION7)
  {
  glBindAttribLocation(id,GRPVERTEX_CHANNEL_MORPH_POSITION7,"a_positionMorph7");    OPENGLCHECKERROR(__L("LoadShader::Morph::Attributes 7"));
  }
  if (GRPOPENGLCTRL::VBO::MaxAttributes>GRPVERTEX_CHANNEL_MORPH_POSITION8)
  {
  glBindAttribLocation(id,GRPVERTEX_CHANNEL_MORPH_POSITION8,"a_positionMorph8");    OPENGLCHECKERROR(__L("LoadShader::Morph::Attributes 8"));
  }

  if (GRPOPENGLCTRL::VBO::MaxAttributes>GRPVERTEX_CHANNEL_MORPH_POSITION9)
  {
  glBindAttribLocation(id,GRPVERTEX_CHANNEL_MORPH_POSITION9,"a_positionMorph9");    OPENGLCHECKERROR(__L("LoadShader::Morph::Attributes 9"));
  }
  if (GRPOPENGLCTRL::VBO::MaxAttributes>GRPVERTEX_CHANNEL_MORPH_POSITION10)
  {
  glBindAttribLocation(id,GRPVERTEX_CHANNEL_MORPH_POSITION10,"a_positionMorph10");  OPENGLCHECKERROR(__L("LoadShader::Morph::Attributes 10"));
  }
  if (GRPOPENGLCTRL::VBO::MaxAttributes>GRPVERTEX_CHANNEL_MORPH_POSITION11)
  {
  glBindAttribLocation(id,GRPVERTEX_CHANNEL_MORPH_POSITION11,"a_positionMorph11");  OPENGLCHECKERROR(__L("LoadShader::Morph::Attributes 11"));
  }
  if (GRPOPENGLCTRL::VBO::MaxAttributes>GRPVERTEX_CHANNEL_MORPH_POSITION12)
  {
  glBindAttribLocation(id,GRPVERTEX_CHANNEL_MORPH_POSITION12,"a_positionMorph12");  OPENGLCHECKERROR(__L("LoadShader::Morph::Attributes 12"));
  }

  OPENGLCHECKERROR(__L("LoadShader::Morph::Attributes"));

#endif

  //glAttachShader(id, vertexshader->GetID());
  //glAttachShader(id, fragmentshader->GetID());
  glAttachShader(id, shaders.Get(GRPSHADERPROGRAMTYPE_VERTEX)->GetID());
  glAttachShader(id, shaders.Get(GRPSHADERPROGRAMTYPE_FRAGMENT)->GetID());
  OPENGLCHECKERROR(__L("glAttachShader"));


  //------------ link

  glLinkProgram(id);
  OPENGLCHECKERROR(__L("glLinkProgram"));
  XDEBUG_PRINTCOLOR(0,__L("\t\t...Linking [%d] : %s"),id,XSTRING(Description).Get());

  Link();


  return IsLoaded;

}

/*-------------------------------------------------------------------
//  GRPSHADERPROGRAM::Link
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/09/2015 9:54:58
//
*/
/*-----------------------------------------------------------------*/
bool GRPSHADERPROGRAM::Link()
{
    int n = 0;
  OPENGLCHECKERROR(__L("link"));
  GLUINT program=id;


  GRPSHADER::UseShader(id);
  OPENGLCHECKERROR(__L("UseShader "));

  GLUINT loc=glGetAttribLocation(program, "a_position");
  OPENGLCHECKERROR(__L("a_position loc"));
  attributeVertex=loc;

  loc=glGetAttribLocation(program, "a_normal");
  OPENGLCHECKERROR(__L("a_normal loc"));
  attributeNormal=loc;

  loc=glGetAttribLocation(program, "a_binormal");
  OPENGLCHECKERROR(__L("a_binormal loc"));
  attributeBinormal=loc;

  loc=glGetAttribLocation(program, "a_tangent");
  OPENGLCHECKERROR(__L("a_tangent loc"));
  attributeTangent=loc;

  loc=glGetAttribLocation(program, "a_texCoord");
  OPENGLCHECKERROR(__L("a_texCoord loc"));
  attributeTextCoord=loc;

  loc=glGetAttribLocation(program, "a_boneIndices");
  OPENGLCHECKERROR(__L("a_boneIndices loc"));
  attributeBoneIndices=loc;

  loc=glGetAttribLocation(program, "a_boneWeights");
  OPENGLCHECKERROR(__L("a_boneWeights loc"));
  attributeBoneWeights=loc;

#ifdef GRPDEFORMERS_ACTIVE
                                  loc=glGetAttribLocation(program, "a_positionMorph1"); OPENGLCHECKERROR(__L("a_positionMorph1 loc"));
  attributePositionMorph1=loc;

  if (loc!=-1)
  {
                                  loc=glGetAttribLocation(program, "a_positionMorph2"); OPENGLCHECKERROR(__L("a_positionMorph1 loc"));
  attributePositionMorph2=loc;
                                  loc=glGetAttribLocation(program, "a_positionMorph3"); OPENGLCHECKERROR(__L("a_positionMorph3 loc"));
  attributePositionMorph3=loc;
                                  loc=glGetAttribLocation(program, "a_positionMorph4"); OPENGLCHECKERROR(__L("a_positionMorph4 loc"));
  attributePositionMorph4=loc;
                                  loc=glGetAttribLocation(program, "a_positionMorph5"); OPENGLCHECKERROR(__L("a_positionMorph5 loc"));
  attributePositionMorph5=loc;
                                  loc=glGetAttribLocation(program, "a_positionMorph6"); OPENGLCHECKERROR(__L("a_positionMorph6 loc"));
  attributePositionMorph6=loc;
                                  loc=glGetAttribLocation(program, "a_positionMorph7"); OPENGLCHECKERROR(__L("a_positionMorph7 loc"));
  attributePositionMorph7=loc;

    attributePositionMorph8         =glGetAttribLocation(program, "a_positionMorph8");  OPENGLCHECKERROR(__L("a_positionMorph8"));
    attributePositionMorph9         =glGetAttribLocation(program, "a_positionMorph9");  OPENGLCHECKERROR(__L("a_positionMorph9"));
    attributePositionMorph10        =glGetAttribLocation(program, "a_positionMorph10"); OPENGLCHECKERROR(__L("a_positionMorph10"));
    attributePositionMorph11        =glGetAttribLocation(program, "a_positionMorph11"); OPENGLCHECKERROR(__L("a_positionMorph11"));
    attributePositionMorph12        =glGetAttribLocation(program, "a_positionMorph12"); OPENGLCHECKERROR(__L("a_positionMorph12"));


    iChannelMorph                   = glGetUniformLocation  (id, "channelsMorph");
    iChannelMorph2                  = glGetUniformLocation  (id, "channelsMorph2");
    iChannelMorph3                  = glGetUniformLocation  (id, "channelsMorph3");
  }
#endif

  /*
  GRPTEXTURELAYERTYPE_DIFFUSE           ,
  GRPTEXTURELAYERTYPE_SPECULAR          ,
  GRPTEXTURELAYERTYPE_NORMALMAP         ,
  GRPTEXTURELAYERTYPE_REFLEXION         ,
  GRPTEXTURELAYERTYPE_DEPTH             ,
  GRPTEXTURELAYERTYPE_OPACITY           ,
  GRPTEXTURELAYERTYPE_BUFFER            ,
  GRPTEXTURELAYERTYPE_EMISIVE           ,
  GRPTEXTURELAYERTYPE_BUMPMAP           ,
  GRPTEXTURELAYERTYPE_ENVMAP            ,
  GRPTEXTURELAYERTYPE_CUSTOM            ,
  GRPTEXTURELAYERTYPE_MAX
  */

  GRPSHADER::UseShader(id);

    OPENGLCHECKERROR(__L("matrices"));

    loc = glGetUniformLocation(id, "s_texture");
    attributeSampler = loc;
    glUniform1i(loc, GRPTEXTURELAYERTYPE_DIFFUSE);
    OPENGLCHECKERROR(__L("s_texture"));

    loc = glGetUniformLocation(id, "textureChannel1");
    attributeSampler1 = loc;
    if (loc != -1)
    glUniform1i(loc, GRPTEXTURELAYERTYPE_DIFFUSE);
    OPENGLCHECKERROR(__L("textureChannel1"));

    loc = glGetUniformLocation(id, "textureChannel2");
    attributeSampler2 = loc;
    if (loc != -1)
    glUniform1i(loc, GRPTEXTURELAYERTYPE_SPECULAR);
    OPENGLCHECKERROR(__L("textureChannel2"));

    loc = glGetUniformLocation(id, "textureChannel3");
    attributeSampler3 = loc;
    if (loc!=-1)
    glUniform1i(loc, GRPTEXTURELAYERTYPE_NORMALMAP);
    OPENGLCHECKERROR(__L("textureChannel3"));

    loc = glGetUniformLocation(id, "textureChannel4");
    attributeSampler4 = loc;
    if (loc != -1)
    glUniform1i(loc, GRPTEXTURELAYERTYPE_REFLEXION);
    OPENGLCHECKERROR(__L("textureChannel4"));

    loc = glGetUniformLocation(id, "textureChannel5");
    attributeSampler5 = loc;
    if (loc != -1)
    glUniform1i(loc, GRPTEXTURELAYERTYPE_DEPTH);
    OPENGLCHECKERROR(__L("textureChannel5"));

    loc = glGetUniformLocation(id, "textureChannel6");
    attributeSampler6 = loc;
    if (loc != -1)
    glUniform1i(loc, GRPTEXTURELAYERTYPE_OPACITY);

    loc = glGetUniformLocation(id, "textureChannel7");
    attributeSampler7 = loc;
    if (loc != -1)
    glUniform1i(loc, GRPTEXTURELAYERTYPE_BUFFER);

    loc = glGetUniformLocation(id, "textureChannel8");
    attributeSampler8 = loc;
    if (loc != -1)
    glUniform1i(loc, GRPTEXTURELAYERTYPE_EMISIVE);

    loc = glGetUniformLocation(id, "textureChannel9");
    attributeSampler9 = loc;
    if (loc != -1)
    glUniform1i(loc, GRPTEXTURELAYERTYPE_BUMPMAP);

    loc = glGetUniformLocation(id, "textureChannel10");
    attributeSampler10 = loc;
    if (loc != -1)
    glUniform1i(loc, GRPTEXTURELAYERTYPE_ENVMAP);

    loc = glGetUniformLocation(id, "textureChannel11");
    attributeSampler11 = loc;
    if (loc != -1)
    glUniform1i(loc, GRPTEXTURELAYERTYPE_CUSTOM);

    loc = glGetUniformLocation(id, "textureChannel12");
    attributeSampler12 = loc;
    if (loc != -1)
    glUniform1i(loc, 12);


    OPENGLCHECKERROR(__L("textureChannel"));



    itextureSize0     = glGetUniformLocation(id, "textureSize0");
    itextureSize1     = glGetUniformLocation(id, "textureSize1");
    itextureSize2     = glGetUniformLocation(id, "textureSize2");
    itextureSize3     = glGetUniformLocation(id, "textureSize3");
    itextureSize4     = glGetUniformLocation(id, "textureSize4");
    itextureSize5     = glGetUniformLocation(id, "textureSize5");
    itextureSize6     = glGetUniformLocation(id, "textureSize6");
    itextureSize7     = glGetUniformLocation(id, "textureSize7");
    itextureSize8     = glGetUniformLocation(id, "textureSize8");
    itextureSize9     = glGetUniformLocation(id, "textureSize9");
    itextureSize10    = glGetUniformLocation(id, "textureSize10");
    itextureSize11    = glGetUniformLocation(id, "textureSize11");
    itextureSize12    = glGetUniformLocation(id, "textureSize12");



    OPENGLCHECKERROR(__L("textureSize5"));

    iBoneMatrices     = glGetUniformLocation(id, "boneMatrices");
    iBonePoseMatrices = glGetUniformLocation(id, "bonePoseMatrices");
    iBoneRootMatrix   = glGetUniformLocation(id, "boneRootMatrix");


  //loc=glGetAttribLocation(program, "a_instanceID");   OPENGLCHECKERROR(__L("a_instanceID loc"));
  //attributeInstanceID=loc;

    iResolutionHandle               = glGetUniformLocation  (id, "iResolution");

    iMaterialMatrix                 = glGetUniformLocation  (id, "materialDataMat");
    ModelMatrixLoc                  = glGetUniformLocation  (id, "modelMat");
    ViewMatrixLoc                   = glGetUniformLocation  (id, "viewMat");
    invViewMatrixLoc                = glGetUniformLocation  (id, "invViewMat");
    ProjectionMatrixLoc             = glGetUniformLocation  (id, "projectionMat");
    invProjectionMatrixLoc          = glGetUniformLocation  (id, "invProjectionMat");
    NormalMatrixLoc                 = glGetUniformLocation  (id, "normalMat");


    TextureMatrixLoc                = glGetUniformLocation  (id, "textureMat");

    ModelViewMatrixLoc              = glGetUniformLocation  (id, "modelviewMat");
    ModelViewProjectionMatrixLoc    = glGetUniformLocation  (id, "modelviewprojectionMat");

    iLightDataMatrix                = glGetUniformLocation  (id, "lightDataMatrix");
    iLightColorMatrix               = glGetUniformLocation  (id, "lightColorMatrix");
    iLightSpaceMatrix               = glGetUniformLocation  (id, "lightSpaceMatrix");



    iGlobalTimeHandle         = glGetUniformLocation  (id, "iGlobalTime");


  IsLoaded=true;
  return true;
}


/*-------------------------------------------------------------------
//  GRPSHADERPROGRAM::SetName
*/
/**
//
//  sets name
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2016 16:37:26
//
//  @return       bool :
//
//  @param        name :
*/
/*-----------------------------------------------------------------*/
bool GRPSHADERPROGRAM::SetKey(char* key)
{
  this->key[0]=key[0];
  this->key[1]=key[1];
  this->key[2]=key[2];

  return true;
}


bool GRPSHADERPROGRAM::Set(const char* name, float value)
{
    GLUINT location = glGetUniformLocation(this->id, (char*)name);

    if (location!=-1)
    glUniform1f(location, value);

    return (location != -1);
}

bool GRPSHADERPROGRAM::Set(const char* name, GLUINT value)
{
    GLUINT location = glGetUniformLocation(this->id, (char*)name);

    if (location != -1)
    glUniform1i(location, value);

    return (location != -1);
}