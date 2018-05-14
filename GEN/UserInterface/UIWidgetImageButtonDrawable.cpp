
/*------------------------------------------------------------------------------------------
//  UIWIDGETIMAGEBUTTONDRAWABLE.CPP
//
//  draws an image correctly as a button
//
//  Author            : Imanol Celaya Ruiz de Alegria
//  Date Of Creation  : 23/06/2016 16:32:33
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "UIWidgetImageButtonDrawable.h"

#include "GRPScene.h"
#include "GRPRenderer.h"
#include "GRPObjectBuilder.h"
#include "GRPMaterialLibrary.h"
#include "GRPViewport.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  UIWIDGETIMAGEBUTTONDRAWABLE::UIWIDGETIMAGEBUTTONDRAWABLE
*/
/**
//
//  Class Constructor UIWIDGETIMAGEBUTTONDRAWABLE
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      24/06/2016 11:31:02
//
//  @param        name :
//  @param        uimanager :
//  @param        scene :
*/
/*-----------------------------------------------------------------*/
UIWIDGETIMAGEBUTTONDRAWABLE::UIWIDGETIMAGEBUTTONDRAWABLE(XCHAR* name, UIMANAGER * uimanager, GRPSCENE* scene) : UIWIDGETIMAGEDRAWABLE(name, uimanager, scene)
{
  Clean();

  // need to create the different elements for the widget (9 in total)
  this->scene = scene;

  imageloaded = false;

  // Top Left
  etl = GRPOBJECTBUILDER::Get()->CreatePlane(GRPOBJECTBUILDER::AXIS_X);
  etl->SetNode(scene->CreateNode());
  etl->GetNode()->Init();
  etl->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"SDF"));
  GRPMATERIAL * materialproc = new GRPMATERIAL();
  materialproc->GetAttributes()->SetDiffuse(GRPCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
  materialproc->GetAttributes()->SetEmission(GRPCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
  materialproc->GetAttributes()->isupdated = true;
//  this->scene->GetMaterialLibrary()->Add(materialproc);
  GRPMATERIALLIBRARY::Get()->Add(materialproc);
  etl->GetNode()->SetMaterial(materialproc);
  scene->Add(etl);

  // Top Middle
  etm = GRPOBJECTBUILDER::Get()->CreatePlane(GRPOBJECTBUILDER::AXIS_X);
  etm->SetNode(scene->CreateNode());
  etm->GetNode()->Init();
  etm->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"SDF"));
  materialproc = new GRPMATERIAL();
  materialproc->GetAttributes()->SetDiffuse(GRPCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
  materialproc->GetAttributes()->SetEmission(GRPCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
  materialproc->GetAttributes()->isupdated = true;
  GRPMATERIALLIBRARY::Get()->Add(materialproc);
  etm->GetNode()->SetMaterial(materialproc);
  scene->Add(etm);

  // Top Right
  etr = GRPOBJECTBUILDER::Get()->CreatePlane(GRPOBJECTBUILDER::AXIS_X);
  etr->SetNode(scene->CreateNode());
  etr->GetNode()->Init();
  etr->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"SDF"));
  materialproc = new GRPMATERIAL();
  materialproc->GetAttributes()->SetDiffuse(GRPCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
  materialproc->GetAttributes()->SetEmission(GRPCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
  materialproc->GetAttributes()->isupdated = true;
  GRPMATERIALLIBRARY::Get()->Add(materialproc);
  etr->GetNode()->SetMaterial(materialproc);
  scene->Add(etr);

  // Middle Left
  eml = GRPOBJECTBUILDER::Get()->CreatePlane(GRPOBJECTBUILDER::AXIS_X);
  eml->SetNode(scene->CreateNode());
  eml->GetNode()->Init();
  eml->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"SDF"));
  materialproc = new GRPMATERIAL();
  materialproc->GetAttributes()->SetDiffuse(GRPCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
  materialproc->GetAttributes()->SetEmission(GRPCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
  materialproc->GetAttributes()->isupdated = true;
  GRPMATERIALLIBRARY::Get()->Add(materialproc);
  eml->GetNode()->SetMaterial(materialproc);
  scene->Add(eml);

  // Middle Middle
  emm = GRPOBJECTBUILDER::Get()->CreatePlane(GRPOBJECTBUILDER::AXIS_X);
  emm->SetNode(scene->CreateNode());
  emm->GetNode()->Init();
  emm->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"SDF"));
  materialproc = new GRPMATERIAL();
  materialproc->GetAttributes()->SetDiffuse(GRPCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
  materialproc->GetAttributes()->SetEmission(GRPCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
  materialproc->GetAttributes()->isupdated = true;
  GRPMATERIALLIBRARY::Get()->Add(materialproc);
  emm->GetNode()->SetMaterial(materialproc);
  scene->Add(emm);


  // Middle Right
  emr = GRPOBJECTBUILDER::Get()->CreatePlane(GRPOBJECTBUILDER::AXIS_X);
  emr->SetNode(scene->CreateNode());
  emr->GetNode()->Init();
  emr->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"SDF"));
  materialproc = new GRPMATERIAL();
  materialproc->GetAttributes()->SetDiffuse(GRPCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
  materialproc->GetAttributes()->SetEmission(GRPCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
  materialproc->GetAttributes()->isupdated = true;
  GRPMATERIALLIBRARY::Get()->Add(materialproc);
  emr->GetNode()->SetMaterial(materialproc);
  scene->Add(emr);


  // Bottom Left
  ebl = GRPOBJECTBUILDER::Get()->CreatePlane(GRPOBJECTBUILDER::AXIS_X);
  ebl->SetNode(scene->CreateNode());
  ebl->GetNode()->Init();
  ebl->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"SDF"));
  materialproc = new GRPMATERIAL();
  materialproc->GetAttributes()->SetDiffuse(GRPCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
  materialproc->GetAttributes()->SetEmission(GRPCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
  materialproc->GetAttributes()->isupdated = true;
  GRPMATERIALLIBRARY::Get()->Add(materialproc);
  ebl->GetNode()->SetMaterial(materialproc);
  scene->Add(ebl);


  // Bottom Middle
  ebm = GRPOBJECTBUILDER::Get()->CreatePlane(GRPOBJECTBUILDER::AXIS_X);
  ebm->SetNode(scene->CreateNode());
  ebm->GetNode()->Init();
  ebm->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"SDF"));
  materialproc = new GRPMATERIAL();
  materialproc->GetAttributes()->SetDiffuse(GRPCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
  materialproc->GetAttributes()->SetEmission(GRPCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
  materialproc->GetAttributes()->isupdated = true;
  GRPMATERIALLIBRARY::Get()->Add(materialproc);
  ebm->GetNode()->SetMaterial(materialproc);
  scene->Add(ebm);


  // Bottom Right
  ebr = GRPOBJECTBUILDER::Get()->CreatePlane(GRPOBJECTBUILDER::AXIS_X);
  ebr->SetNode(scene->CreateNode());
  ebr->GetNode()->Init();
  ebr->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"SDF"));
  materialproc = new GRPMATERIAL();
  materialproc->GetAttributes()->SetDiffuse(GRPCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
  materialproc->GetAttributes()->SetEmission(GRPCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
  materialproc->GetAttributes()->isupdated = true;
  GRPMATERIALLIBRARY::Get()->Add(materialproc);
  ebr->GetNode()->SetMaterial(materialproc);
  scene->Add(ebr);


  etl->SetBlending(true);
  etl->SetBlendingMode(GRPBLENDINGMODE_ALPHABLEND);
  etm->SetBlending(true);
  etm->SetBlendingMode(GRPBLENDINGMODE_ALPHABLEND);
  etr->SetBlending(true);
  etr->SetBlendingMode(GRPBLENDINGMODE_ALPHABLEND);
  eml->SetBlending(true);
  eml->SetBlendingMode(GRPBLENDINGMODE_ALPHABLEND);
  emm->SetBlending(true);
  emm->SetBlendingMode(GRPBLENDINGMODE_ALPHABLEND);
  emr->SetBlending(true);
  emr->SetBlendingMode(GRPBLENDINGMODE_ALPHABLEND);
  ebl->SetBlending(true);
  ebl->SetBlendingMode(GRPBLENDINGMODE_ALPHABLEND);
  ebm->SetBlending(true);
  ebm->SetBlendingMode(GRPBLENDINGMODE_ALPHABLEND);
  ebr->SetBlending(true);
  ebr->SetBlendingMode(GRPBLENDINGMODE_ALPHABLEND);
}





/*-------------------------------------------------------------------
//  UIWIDGETIMAGEBUTTONDRAWABLE::~UIWIDGETIMAGEBUTTONDRAWABLE
*/
/**
//
//   Class Destructor UIWIDGETIMAGEBUTTONDRAWABLE
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      24/06/2016 11:30:35
//
*/
/*-----------------------------------------------------------------*/
UIWIDGETIMAGEBUTTONDRAWABLE::~UIWIDGETIMAGEBUTTONDRAWABLE()
{
  Clean();
}





/*-------------------------------------------------------------------
//  UIWIDGETIMAGEBUTTONDRAWABLE::SetImages
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      24/06/2016 11:30:25
//
//  @return       bool :
//
//  @param        tl :
//  @param        tm :
//  @param        tr :
//  @param        ml :
//  @param        mm :
//  @param        mr :
//  @param        bl :
//  @param        bm :
//  @param        br :
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETIMAGEBUTTONDRAWABLE::SetImages(XCHAR* tl, XCHAR* tm, XCHAR* tr, XCHAR* ml, XCHAR* mm, XCHAR* mr, XCHAR* bl, XCHAR* bm, XCHAR* br)
{
  // load the textures
  imageloaded = false;

  stl.Set(tl);
  stm.Set(tm);
  str.Set(tr);

  sml.Set(ml);
  smm.Set(mm);
  smr.Set(mr);

  sbl.Set(bl);
  sbm.Set(bm);
  sbr.Set(br);

  return true;
}




/*-------------------------------------------------------------------
//  UIWIDGETIMAGEBUTTONDRAWABLE::InternalDraw
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      24/06/2016 11:30:15
//
//  @return       bool :
//
//  @param        widgetlevel :
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETIMAGEBUTTONDRAWABLE::InternalDraw(XDWORD& widgetlevel)
{
  if(!imageloaded)
    {
      GRPFILEBITMAP* grpfilebitmap = new GRPFILEBITMAP();

      XPATH xpath;

      // Top Left
      xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, stl.Get());
      GRPBITMAP* grpbitmap = grpfilebitmap->Load(xpath);
      ttl = new GRPTEXTURE();
      ttl->Create(grpbitmap);
      etl->GetNode()->GetMaterial()->AddLayer(GRPMATERIAL_LAYER_DIFFUSECOLOR, ttl);
      GRPMATERIALLIBRARY::Get()->GetTextures()->Add(&stl, ttl);
      etl->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"STX"));

      // Top Middle
      xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, stm.Get());
      grpbitmap = grpfilebitmap->Load(xpath);
      ttm = new GRPTEXTURE();
      ttm->Create(grpbitmap);
      etm->GetNode()->GetMaterial()->AddLayer(GRPMATERIAL_LAYER_DIFFUSECOLOR, ttm);
      GRPMATERIALLIBRARY::Get()->GetTextures()->Add(&stm, ttm);
      etm->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"STX"));

      // Top Right
      xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, str.Get());
      grpbitmap = grpfilebitmap->Load(xpath);
      ttr = new GRPTEXTURE();
      ttr->Create(grpbitmap);
      etr->GetNode()->GetMaterial()->AddLayer(GRPMATERIAL_LAYER_DIFFUSECOLOR, ttr);
      GRPMATERIALLIBRARY::Get()->GetTextures()->Add(&str, ttr);
      etr->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"STX"));

      // Middle Left
      xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, sml.Get());
      grpbitmap = grpfilebitmap->Load(xpath);
      tml = new GRPTEXTURE();
      tml->Create(grpbitmap);
      eml->GetNode()->GetMaterial()->AddLayer(GRPMATERIAL_LAYER_DIFFUSECOLOR, tml);
      GRPMATERIALLIBRARY::Get()->GetTextures()->Add(&sml, tml);
      eml->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"STX"));

      // Middle Middle
      xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, smm.Get());
      grpbitmap = grpfilebitmap->Load(xpath);
      tmm = new GRPTEXTURE();
      tmm->Create(grpbitmap);
      emm->GetNode()->GetMaterial()->AddLayer(GRPMATERIAL_LAYER_DIFFUSECOLOR, tmm);
      GRPMATERIALLIBRARY::Get()->GetTextures()->Add(&smm, tmm);
      emm->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"STX"));

      // Middle Right
      xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, smr.Get());
      grpbitmap = grpfilebitmap->Load(xpath);
      tmr = new GRPTEXTURE();
      tmr->Create(grpbitmap);
      emr->GetNode()->GetMaterial()->AddLayer(GRPMATERIAL_LAYER_DIFFUSECOLOR, tmr);
      GRPMATERIALLIBRARY::Get()->GetTextures()->Add(&smr, tmr);
      emr->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"STX"));

      // Bottom Left
      xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, sbl.Get());
      grpbitmap = grpfilebitmap->Load(xpath);
      tbl = new GRPTEXTURE();
      tbl->Create(grpbitmap);
      ebl->GetNode()->GetMaterial()->AddLayer(GRPMATERIAL_LAYER_DIFFUSECOLOR, tbl);
      GRPMATERIALLIBRARY::Get()->GetTextures()->Add(&sbl, tbl);
      ebl->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"STX"));

      // Bottom Middle
      xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, sbm.Get());
      grpbitmap = grpfilebitmap->Load(xpath);
      tbm = new GRPTEXTURE();
      tbm->Create(grpbitmap);
      ebm->GetNode()->GetMaterial()->AddLayer(GRPMATERIAL_LAYER_DIFFUSECOLOR, tbm);
      GRPMATERIALLIBRARY::Get()->GetTextures()->Add(&sbm, tbm);
      ebm->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"STX"));

      // Bottom Right
      xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, sbr.Get());
      grpbitmap = grpfilebitmap->Load(xpath);
      tbr = new GRPTEXTURE();
      tbr->Create(grpbitmap);
      ebr->GetNode()->GetMaterial()->AddLayer(GRPMATERIAL_LAYER_DIFFUSECOLOR, tbr);
      GRPMATERIALLIBRARY::Get()->GetTextures()->Add(&sbr, tbr);
      ebr->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"STX"));

      delete grpfilebitmap;

      imageloaded = true;
    }

  // or maybe place them as child widgets, should make the whole position handling easier
  if(modified)
    {
      // edit the nodes of ALL elements
      // need to compute the global margin size from the pixel size of the images(take the top left as a reference?)
      float scalex = gpt1x - gpt0x;
      float scaley = gpt1y - gpt0y;

      if(!ttl)
        {
          return false;
        }

      XDWORD tpxwidth = (XDWORD)ttl->GetWidth();
      XDWORD tpxheight = (XDWORD)ttl->GetHeight();

      float screenw;
      float screenh;

      scene->GetViewport()->GetWindowSize(screenw, screenh);

      float utpxwidth = tpxwidth/screenw;
      float utpxheight = tpxheight/screenh;

      float marginscalex = utpxwidth;
      float marginscaley = utpxheight;

      float middlescalex = scalex - 2.0f*marginscalex;
      float middlescaley = scaley - 2.0f*marginscaley;

      float adjustedmarginscaley = marginscaley / scene->GetViewport()->GetAspectRatio();
      float adjustedmiddlescaley = middlescaley / scene->GetViewport()->GetAspectRatio();

      // Compute global coordinates for the different elements, then transforms and apply
      // Top Left
      float tlpt0x = gpt0x;
      float tlpt0y = gpt0y;

      GRPPOINT tlpt = scene->GetViewport()->GetViewPortFromInverseNormalized(tlpt0x, tlpt0y, 0.0f);

      etl->GetNode()->SetPosition(tlpt.x + marginscalex, tlpt.y - adjustedmarginscaley, widgetlevel + 0.1f);
      etl->SetRenderPriority(widgetlevel);
      etl->GetNode()->SetScale(marginscalex, adjustedmarginscaley, 1.0f);
      etl->GetNode()->Update();
      etl->GetNode()->UpdateWorldMatrix();

      // Top Middle
      float tmpt0x = gpt0x + marginscalex;
      float tmpt0y = gpt0y;

      GRPPOINT tmpt = scene->GetViewport()->GetViewPortFromInverseNormalized(tmpt0x, tmpt0y, 0.0f);

      etm->GetNode()->SetPosition(tmpt.x + middlescalex, tmpt.y - adjustedmarginscaley, widgetlevel + 0.1f);
      etm->SetRenderPriority(widgetlevel);
      etm->GetNode()->SetScale(middlescalex, adjustedmarginscaley, 1.0f);
      etm->GetNode()->Update();
      etm->GetNode()->UpdateWorldMatrix();

      // Top Right
      float trpt0x = gpt1x - marginscalex;
      float trpt0y = gpt0y;

      GRPPOINT trpt = scene->GetViewport()->GetViewPortFromInverseNormalized(trpt0x, trpt0y, 0.0f);

      etr->GetNode()->SetPosition(trpt.x + marginscalex, trpt.y - adjustedmarginscaley, widgetlevel + 0.1f);
      etr->SetRenderPriority(widgetlevel);
      etr->GetNode()->SetScale(marginscalex, adjustedmarginscaley, 1.0f);
      etr->GetNode()->Update();
      etr->GetNode()->UpdateWorldMatrix();

      // Middle Left
      float mlpt0x = gpt0x;
      float mlpt0y = gpt0y + marginscaley;

      GRPPOINT mlpt = scene->GetViewport()->GetViewPortFromInverseNormalized(mlpt0x, mlpt0y, 0.0f);

      eml->GetNode()->SetPosition(mlpt.x + marginscalex, mlpt.y - adjustedmiddlescaley, widgetlevel + 0.1f);
      eml->SetRenderPriority(widgetlevel);
      eml->GetNode()->SetScale(marginscalex, adjustedmiddlescaley, 1.0f);
      eml->GetNode()->Update();
      eml->GetNode()->UpdateWorldMatrix();

      // Middle Middle
      float mmpt0x = gpt0x + marginscalex;
      float mmpt0y = gpt0y + marginscaley;

      GRPPOINT mmpt = scene->GetViewport()->GetViewPortFromInverseNormalized(mmpt0x, mmpt0y, 0.0f);

      emm->GetNode()->SetPosition(mmpt.x + middlescalex, mmpt.y - adjustedmiddlescaley, widgetlevel + 0.1f);
      emm->SetRenderPriority(widgetlevel);
      emm->GetNode()->SetScale(middlescalex, adjustedmiddlescaley, 1.0f);
      emm->GetNode()->Update();
      emm->GetNode()->UpdateWorldMatrix();

      // Middle Right
      float mrpt0x = gpt1x - marginscalex;
      float mrpt0y = gpt0y + marginscaley;

      GRPPOINT mrpt = scene->GetViewport()->GetViewPortFromInverseNormalized(mrpt0x, mrpt0y, 0.0f);

      emr->GetNode()->SetPosition(mrpt.x + marginscalex, mrpt.y - adjustedmiddlescaley, widgetlevel + 0.1f);
      emr->SetRenderPriority(widgetlevel);
      emr->GetNode()->SetScale(marginscalex, adjustedmiddlescaley, 1.0f);
      emr->GetNode()->Update();
      emr->GetNode()->UpdateWorldMatrix();

      // Bottom Left
      float blpt0x = gpt0x;
      float blpt0y = gpt1y - marginscaley;

      GRPPOINT blpt = scene->GetViewport()->GetViewPortFromInverseNormalized(blpt0x, blpt0y, 0.0f);

      ebl->GetNode()->SetPosition(blpt.x + marginscalex, blpt.y - adjustedmarginscaley, widgetlevel + 0.1f);
      ebl->SetRenderPriority(widgetlevel);
      ebl->GetNode()->SetScale(marginscalex, adjustedmarginscaley, 1.0f);
      ebl->GetNode()->Update();
      ebl->GetNode()->UpdateWorldMatrix();

      // Bottom Middle
      float bmpt0x = gpt0x + marginscalex;
      float bmpt0y = gpt1y - marginscaley;

      GRPPOINT bmpt = scene->GetViewport()->GetViewPortFromInverseNormalized(bmpt0x, bmpt0y, 0.0f);

      ebm->GetNode()->SetPosition(bmpt.x + middlescalex, bmpt.y - adjustedmarginscaley, widgetlevel + 0.1f);
      ebm->SetRenderPriority(widgetlevel);
      ebm->GetNode()->SetScale(middlescalex, adjustedmarginscaley, 1.0f);
      ebm->GetNode()->Update();
      ebm->GetNode()->UpdateWorldMatrix();

      // Bottom Right
      float brpt0x = gpt1x - marginscalex;
      float brpt0y = gpt1y - marginscaley;

      GRPPOINT brpt = scene->GetViewport()->GetViewPortFromInverseNormalized(brpt0x, brpt0y, 0.0f);

      ebr->GetNode()->SetPosition(brpt.x + marginscalex, brpt.y - adjustedmarginscaley, widgetlevel + 0.1f);
      ebr->SetRenderPriority(widgetlevel);
      ebr->GetNode()->SetScale(marginscalex, adjustedmarginscaley, 1.0f);
      ebr->GetNode()->Update();
      ebr->GetNode()->UpdateWorldMatrix();
    }

  etl->Publish(GRPMSG_ORDER);
  etm->Publish(GRPMSG_ORDER);
  etr->Publish(GRPMSG_ORDER);

  eml->Publish(GRPMSG_ORDER);
  emm->Publish(GRPMSG_ORDER);
  emr->Publish(GRPMSG_ORDER);

  ebl->Publish(GRPMSG_ORDER);
  ebm->Publish(GRPMSG_ORDER);
  ebr->Publish(GRPMSG_ORDER);

  return true;
}


bool UIWIDGETIMAGEBUTTONDRAWABLE::InternalUpdate(XDWORD widgetlevel)
{
  return true;
}

void UIWIDGETIMAGEBUTTONDRAWABLE::SetActive(bool active)
{
  if(etl) etl->SetVisible(active);
  if(etm) etm->SetVisible(active);
  if(etr) etr->SetVisible(active);

  if(eml) eml->SetVisible(active);
  if(emm) emm->SetVisible(active);
  if(emr) emr->SetVisible(active);

  if(ebl) ebl->SetVisible(active);
  if(ebm) ebm->SetVisible(active);
  if(ebr) ebr->SetVisible(active);

  UIWIDGETIMAGEDRAWABLE::SetActive(active);
}
