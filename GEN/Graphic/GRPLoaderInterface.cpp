/*------------------------------------------------------------------------------------------
//  GRPLOADERINTERFACE.CPP
//
//  Loader interface
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 21/09/2016 10:16:52
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPLoaderInterface.h"
#include "GRPObjectBuilder.h"
#include "GRPObjectLibrary.h"
#include "GRPLoader.h"
#include "GRPScene.h"
#include "GRPScreen.h"
#include "GRPMaterialLibrary.h"
#include "GRPCamera.h"
#include "GRPObject.h"
#include "GRPProfiler.h"
#include "GRPConsole.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/

const char GRPSHADERLIBRARY::LoadingRing[] = {
    "uniform sampler2D s_texture;   \n"
    "uniform vec2 iResolution;      \n"
    "varying vec2 v_texCoord;       \n"
    "uniform float iGlobalTime;     \n"
    "uniform float iProgress;       \n"
    "uniform vec2 textureSize5;     \n"

    "#define SMOOTH(r) (mix(1.0, 0.0, smoothstep(0.9,1.0, r)))\n"
    "#define M_PI 3.1415926535897932384626433832795\n"

    "float movingRing(vec2 uv, vec2 center, float r1, float r2)\n"
    "{\n"
    "    vec2 d = uv - center;\n"
    "    float r = length(d);\n"
    //"   r= sqrt( dot( d, d ) );\n"
    "    d = normalize(d);\n"

    "    float theta = (-atan(d.x,-d.y));\n"
    "    if (abs(theta)<(iProgress*M_PI))\n"
    "        if (r>r1 && r<r2)   \n"
    "       return 0.3+(0.7*abs(sin(iGlobalTime*0.01))*smoothstep(r1,r2,r));\n"

    "    return 0.0;\n"
    "}\n"

    "void main()\n"
    "{\n"
    " vec2 pos = gl_FragCoord.xy;\n"
    " float size=textureSize5.x;\n"
    " float sizex=textureSize5.x;\n"
    " float sizey=textureSize5.y;\n"
    " float width=20.0;\n"


    " float ring = movingRing(pos, vec2(iResolution.x/2.0,iResolution.y/2.0), size/2.0-width,size/2.0);\n"

    " vec4 color=vec4(0.0,0.0,0.0,1.0);\n"
    " float i=0.0;\n"

    " vec2 start=vec2((iResolution.x/2.0-size/2.0),(iResolution.y/2.0-size/2.0));\n"
    " vec2 end  =vec2((iResolution.x/2.0+size/2.0),(iResolution.y/2.0+size/2.0));\n"

    " vec2 uv=vec2(0.0,0.0);\n"

    " if (gl_FragCoord.x>start.x)\n"
    "   if (gl_FragCoord.y>start.y)\n"
    "     if (gl_FragCoord.x<end.x)\n"
    "       if (gl_FragCoord.y<end.y)\n"
    "       {\n"
    "         color = vec4(1.0,1.0,1.0,1.0);\n"
    "         uv.x    = gl_FragCoord.x - start.x;\n"
    "         uv.x   /= size;\n"
    "         uv.y    = gl_FragCoord.y - start.y;\n"
    "         uv.y   /= size;\n"
    "       }\n"

    "    gl_FragColor = ring + color*texture2D(s_texture,uv);\n"

    "}\n"
};



const char GRPSHADERLIBRARY::LoadingBar[] = {
    "uniform sampler2D s_texture;   \n"
    "uniform vec2 iResolution;      \n"
    "varying vec2 v_texCoord;       \n"
    "uniform float iGlobalTime;     \n"
    "uniform float iProgress;       \n"
    "uniform vec2 textureSize0;     \n"

    "void main()\n"
    "{\n"
    " vec2 pos = gl_FragCoord.xy;\n"
    " float size=textureSize0.x;\n"
    " float sizex=textureSize0.x;\n"
    " float sizey=textureSize0.y;\n"
    " float width=10.0;\n"
    " float height=32.0;\n"


    "  vec4 ring=vec4(0.0);\n"


    " if ((pos.x/iResolution.x)<iProgress)\n"
    " if (pos.y<height)\n"
    "    ring=vec4(0.58,0.00,0.37,1.0)*clamp(abs(sin(iGlobalTime*0.01)),0.5,1.0)*tan(pos.y/height); \n"

    " float i=0.0;\n"
    " if (pos.x>(iResolution.x/2.0-sizex/2.0) &&   \n"
    "     pos.x<(iResolution.x/2.0+sizex/2.0) &&   \n"
    "     pos.y>(iResolution.y/2.0-sizey/2.0) &&   \n"
    "     pos.y<(iResolution.y/2.0+sizey/2.0))     \n"
    "   i=1.0; \n"

    "  vec2 uv = vec2((gl_FragCoord.x-(iResolution.x/2.0-sizex/2.0))/sizex , (gl_FragCoord.y-(iResolution.y/2.0-sizey/2.0))/sizey); \n"

  //  "  vec2 uv=vec2(-(iResolution.x/2.0-gl_FragCoord.x)/sizex,(gl_FragCoord.y)/sizey);\n"
    "    gl_FragColor = vec4(0.0,0.0,0.0,1.0) + ring + i*texture2D(s_texture,uv);\n"
    "}\n"
};




/*---- CLASS MEMBERS ---------------------------------------------------------------------*/

/*-------------------------------------------------------------------
//  GRPLOADERINTERFACE::GRPLOADERINTERFACE
*/
/**
//
//  Class Constructor GRPLOADERINTERFACE
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/03/2017 12:13:24
//
*/
/*-----------------------------------------------------------------*/
GRPLOADERINTERFACE::GRPLOADERINTERFACE()
{
  Clean();
  GRPOBJECTLIBRARY::Get()->GetLoader()->AddObserver(this);
}

/*-------------------------------------------------------------------
//  GRPLOADERINTERFACE::GRPLOADERINTERFACE
*/
/**
//
//  Class Constructor GRPLOADERINTERFACE
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2016 10:18:31
//
*/
/*-----------------------------------------------------------------*/
GRPLOADERINTERFACE::GRPLOADERINTERFACE(GRPSCENE* scene, XCHAR* name, GRPSHADERPROGRAM* shader)
{
  Clean();
  SetScene  (scene);
  SetScreen (scene->GetScreen());
  SetBitmap (name);
  this->shader=shader;

  GRPOBJECTLIBRARY::Get()->GetLoader()->AddObserver(this);
}

/*-------------------------------------------------------------------
//  GRPLOADERINTERFACE::~GRPLOADERINTERFACE
*/
/**
//
//  destructor
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  04/12/2017 14:48:57
//  @return    :
//
//  @param     :
//
*//*-----------------------------------------------------------------*/
GRPLOADERINTERFACE::~GRPLOADERINTERFACE()
{
    this->Unsubscribe();
}

/*-------------------------------------------------------------------
//  GRPLOADERINTERFACE::AddTask
*/
/**
//
//  Add file to load task
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      10/01/2017 11:47:54
//
//  @return       bool :
//
//  @param        toload :
*/
/*-----------------------------------------------------------------*/
bool  GRPLOADERINTERFACE::AddTask(XCHAR* toload, XDWORD loadflags)
{
  if (!GRPOBJECTLIBRARY::Get()->GetLoader())
  {
    XDEBUG_PRINTCOLOR(4,__L("Unable to add task, loader is not defined"));
    return false;
  }

  GRPLOADERTASK*  task=(GRPLOADERTASK*)GRPOBJECTLIBRARY::Get()->GetLoader()->CreateTask   (toload);
  if (task)
  {
      GRPOBJECTLIBRARY::Get()->GetLoader()->AddTask(task);
  }
  else
      return false;


  return true;
}

/*-------------------------------------------------------------------
//  GRPLOADERINTERFACE::SetScene
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2016 10:20:50
//
//  @param        scene :
*/
/*-----------------------------------------------------------------*/
void GRPLOADERINTERFACE::SetScene(GRPSCENE* scene)
{
  this->scene=scene;
  scene->AddObserver(this);
}

/*-------------------------------------------------------------------
//  GRPLOADERINTERFACE::SetScreen
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2016 10:25:38
//
//  @param        screen :
*/
/*-----------------------------------------------------------------*/
void GRPLOADERINTERFACE::SetScreen(GRPSCREEN* screen)
{
  this->screen=screen;
}

/*-------------------------------------------------------------------
//  GRPLOADERINTERFACE::SetBitmap
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2016 10:39:30
//
//  @param        bitmapname :
*/
/*-----------------------------------------------------------------*/
void GRPLOADERINTERFACE::SetBitmap(XCHAR* bitmapname)
{
  this->bitmap.Set(bitmapname);
}

/*-------------------------------------------------------------------
//   GRPLOADERINTERFACE::SetShader
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  04/12/2017 14:49:15
//  @return   void :
//
//  @param    GRPSHADERPROGRAM* :
//
*//*-----------------------------------------------------------------*/
void GRPLOADERINTERFACE::SetShader(GRPSHADERPROGRAM*  shader)
{
    this->shader=shader;
}

/*-------------------------------------------------------------------
//  GRPLOADERINTERFACE::Init
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2016 10:18:49
//
*/
/*-----------------------------------------------------------------*/
bool GRPLOADERINTERFACE::Init()
{
    if (element)
    {
        GRPNODE* node=element->GetNode();
        element->Dettach();
        delete(node);
        if (scene)
            scene->RemoveFromRenderOrder(element);
        delete(element);
    }

  element=GRPOBJECTBUILDER::Get()->CreatePlane(GRPOBJECTBUILDER::AXIS_SCREEN);
  if (!element)
      return false;

  element->SetNode(new GRPNODE());
  element->GetNode()->SetScale(1.0);

  element->SetName(__L("LoaderScreenPlane"));
  element->GetNode()->SetName(__L("LoaderScreenPlaneNode"));

  if (!shader)
        shader=GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"LDB");

  //--------------------------------

  if (shader && this->screen)
  {
    GRPSHADER::UseTmpShader(shader->GetID());

    GLUINT pos=glGetUniformLocation(shader->GetID(),"iResolution");

          if (pos!=-1)
              glUniform2f(pos,(GLFLOAT)this->screen->GetWidth(),(GLFLOAT)this->screen->GetHeight());

           pos=glGetUniformLocation(shader->GetID(),"iGlobalTime");

           if (pos!=-1)
             glUniform1f(pos,0.0f);

    GRPSHADER::PopShader();

  element->SetProgram(shader);
  }

  //----------------------------------- material default location

  XPATH xpath;
        xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1 ,__L(""));
        GRPMATERIALLIBRARY::Get()->SetResourceLocation(xpath);
        xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, bitmap.Get());

  if(!GRPMATERIALLIBRARY::Get()->Load(xpath.Get(),  bitmap.Get()))    { XDEBUG_PRINTCOLOR(4, __L("Error loading Texture data : %s"),bitmap.Get());  }

  GRPMATERIAL * material=GRPMATERIALLIBRARY::Get()->Select( bitmap.Get());

    if (material)
    if (material->GetLayer(0))
    {
            glUniform2f(shader->itextureSize0, (GLFLOAT)material->GetLayer(0)->width, (GLFLOAT)material->GetLayer(0)->height);
            scene->GetPass()->SetClear(GRPPASS_CLEAR_FLAG_ALL);
            element->GetNode()->SetMaterial(material);
            element->SetBlending(true);
    }

  GRPCAMERA*  cam=scene->GetCamera();
              cam->SetPosition(0.0,0.0,+1.0);

  if (loader==NULL)
              loader=GRPOBJECTLIBRARY::Get()->GetLoader();

  loader->AddObserver (this);

  scene->AddObserver  (this);
  scene->Add(element);

  return true;
}

/*-------------------------------------------------------------------
//  GRPLOADERINTERFACE::Do
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2016 10:18:58
//
*/
/*-----------------------------------------------------------------*/
bool GRPLOADERINTERFACE::Do()
{
  OPENGLCHECKERROR(__L("GRPLOADERINTERFACE::Do"));

  static int p=0;
  if (shader)
  {

    if (shader->IsLoaded && GRPPROFILER::frames%2 && p!=(int)total)
    {
      GRPSHADER::UseTmpShader(shader->GetID());
      glUniform1f(glGetUniformLocation(shader->GetID(),"iProgress"),(total/100.0f));  OPENGLCHECKERROR(__L("GRPLOADERINTERFACE::Do"));
      GRPSHADER::PopShader();
      p=(int)total;
    }
  }
  if (console)
    console->Update();
  return true;
}

/*-------------------------------------------------------------------
//  GRPLOADERINTERFACE::End
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2016 10:19:10
//
*/
/*-----------------------------------------------------------------*/
bool GRPLOADERINTERFACE::End()
{
  this->Abort();

  this->Unsubscribe();
  this->Publish(GRPMSG_DESTROYED);
  this->Hide();

  delete(element->GetNode());
if (this->element!=NULL)
{
    if (scene)
    {
        scene->RemoveFromRenderOrder(element);
        int n = scene->GetNodes()->Find(this->element->GetNode());
        scene->GetNodes()->Delete(this->element->GetNode());
    }
  delete(this->element);
  this->element=NULL;


//*/
}
return true;
}

/*-------------------------------------------------------------------
//  GRPLOADERINTERFACE::Abort
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/02/2017 14:28:03
//
*/
/*-----------------------------------------------------------------*/
void  GRPLOADERINTERFACE::Abort()
{
    if (!isaborted)
    {
        isaborted = true;
        this->Publish(GRPMSG_EXIT);
        this->GetLoader()->Abort();
        XDEBUG_PRINTCOLOR(0, __L(" -------------- ABORTED -----------------"));
    }
}

/*-------------------------------------------------------------------
//  GRPLOADERINTERFACE::AttachConsole
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      02/03/2017 17:40:48
//
//  @param        console :
*/
/*-----------------------------------------------------------------*/
void  GRPLOADERINTERFACE::AttachConsole(GRPCONSOLE* console)
{
  this->console=console;
  this->loader->AttachConsole(console);
}


/*-------------------------------------------------------------------
//  GRPLOADERINTERFACE::GetSubjectType
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2016 11:06:22
//
//  @return       XDWORD :
//
*/
/*-----------------------------------------------------------------*/
XDWORD GRPLOADERINTERFACE::GetSubjectType()
{
  return GRPNAMED_TYPE_TASKMANAGER;
}

/*-------------------------------------------------------------------
//  GRPLOADERINTERFACE::Notify
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2016 10:47:09
//
//  @return       bool :
//
//  @param        msg :
//  @param        subject :
*/
/*-----------------------------------------------------------------*/
bool GRPLOADERINTERFACE::Notify(GRPMSGTYPE msg, GRPSUBJECT* subject)
{

  switch(msg)
  {
  case GRPMSG_DESTROYED:
      if (subject->GetSubjectType() == GRPNAMED_TYPE_GRPSCENE)
          this->scene = NULL;
          this->Unsubscribe(subject);
        break;

  case GRPMSG_STEP:
      switch (subject->GetSubjectType())
      {
      case GRPNAMED_TYPE_TASK:
        {
          GRPLOADERTASK* task=dynamic_cast<GRPLOADERTASK*>(subject);
          if (task)
          {
          task->Update();
          total=task->GetPercent();
          this->Publish(GRPMSG_STEP,task);
          }
          else
          {
            GRPFILE3D* f3d = dynamic_cast<GRPFILE3D*>(subject);
            if (f3d)
            {
                if (this->isaborted)
                    f3d->SetIsAborted(true);

                if (int(f3d->GetPercent()) != int(total))
                {
                    //      XDEBUG_PRINTCOLOR(4, __L("---- %02d"), int(total));
                    total = f3d->GetPercent();
                    this->Publish(GRPMSG_STEP, this);
                }
            }
          }
        }
        break;

      case GRPNAMED_TYPE_GRPSCENE:  Do(); break;
      }

    break;

  case GRPMSG_EXIT:  //crashed load
        if (subject->GetSubjectType()==GRPNAMED_TYPE_TASK)
        {
          GRPLOADERTASK* task=static_cast<GRPLOADERTASK*>(subject);
          if (task)
          {
            this->Publish(GRPMSG_EXIT,task);
            return false;
          }
        }
        break;

  case GRPMSG_END:
    if (subject->GetSubjectType()==GRPNAMED_TYPE_TASK)
    {
      GRPLOADERTASK* task=static_cast<GRPLOADERTASK*>(subject);
      if (task)
      {
         total  = 100.0f;

         element->SetVisibility(0.0f);

         scene->Add(task->object);

         for (XDWORD e=0;e<task->object->GetPasses()->GetSize();e++)
         scene->Add(task->object->GetPasses()->Get(e));

         GRPCAMERA* cam = task->object->GetCamera();
         scene->SetCurrentCamera(scene->GetCameras()->Find(cam));

         this->Publish(GRPMSG_END);
         this->Unsubscribe();
      }
    }
    break;
  }
  return true;
}
