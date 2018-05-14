
/*------------------------------------------------------------------------------------------
//  GRPFILE3DOBJ.CPP
//
//  Loads Wavefront Mesh
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 12/01/2015 17:02:49
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "grpfile3dobj.h"
#include "GRPObject.h"
#include "XFileTXT.h"
#include "GRPScene.h"
#include "GRPMaterialLibrary.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/

/*---- CLASS MEMBERS ---------------------------------------------------------------------*/

/*-------------------------------------------------------------------
//  GRPFILE3DOBJ::Load
*/
/**
//  Loads wavefront object data
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      12/01/2015 17:03:29
//
//  @return       GRPOBJECT*  : generated object
//
//  @param        file        : XCHAR* , name of the file to load
*/
/*-----------------------------------------------------------------*/
GRPOBJECT* GRPFILE3DOBJ::Load(XCHAR* name)
{
  if (object==NULL)
  {
      object=new GRPOBJECT();
  }

  if (object->GetNode()==NULL)
  object->SetNode(object->CreateNode());

  element=NULL;

  XVECTOR<GRPPOINT*> vertices;
  XVECTOR<GRPPOINT*> normals;
  XVECTOR<GRPPOINT*> tcoords;

  GLUINT CurrentMaterial=0;


  XFILETXT* xfiletxtdevices =  new XFILETXT();
  XPATH     xpath;
  xpath=name;


  if(!xfiletxtdevices) return NULL;

  if(!xfiletxtdevices->Open(xpath)) return NULL;

  xfiletxtdevices->ReadAllFile();

  XDWORD lastElementVertexOffset=0;
  XDWORD lastElementNormalOffset=0;
  XDWORD lastElementTCoordsOffset=0;

  for(int e=0; e<xfiletxtdevices->GetNLines(); e++)
  {
    XSTRING line(xfiletxtdevices->GetLineText(e));
    XSTRING type;

    GLUINT t=GRPFILE3DWAVEFRONTFILESECTION_UNKNOWN;

    type.AdjustSize(50);
    line.UnFormat(__L("%s "),type.Get());
    type.AdjustSize();

    if (type.Compare(__L("#"))==0)
    {
      XSTRING sname;
      if (line.Find(__L("object"),true)!=NOTFOUND)
            t=GRPFILE3DWAVEFRONTFILESECTION_ELEMENT_NAME;
      else
      t=GRPFILE3DWAVEFRONTFILESECTION_COMMENT;
    }
    else
    if (type.Compare(__L("v"))==0)
      t=GRPFILE3DWAVEFRONTFILESECTION_ELEMENT_VERTEX;
    else
    if (type.Compare(__L("vn"))==0)
      t=GRPFILE3DWAVEFRONTFILESECTION_ELEMENT_NORMAL;
    else
    if (type.Compare(__L("vt"))==0)
      t=GRPFILE3DWAVEFRONTFILESECTION_ELEMENT_TEXTCOORD;
    else
    if (type.Compare(__L("g"))==0)
      t=GRPFILE3DWAVEFRONTFILESECTION_GROUP_NAME;
    else
    if (type.Compare(__L("mtllib"))==0)
    t=GRPFILE3DWAVEFRONTFILESECTION_ELEMENT_MATERIAL_LIBRARY;
    else
    if (type.Compare(__L("usemtl"))==0)
      t=GRPFILE3DWAVEFRONTFILESECTION_ELEMENT_MATERIAL;
    else
    if (type.Compare(__L("f"))==0)
      t=GRPFILE3DWAVEFRONTFILESECTION_ELEMENT_FACE;
    else
    if (type.Compare(__L("s"))==0)
      t=GRPFILE3DWAVEFRONTFILESECTION_SMOOTH_SHADING;

    switch(t)
    {
    case GRPFILE3DWAVEFRONTFILESECTION_SMOOTH_SHADING:
    case GRPFILE3DWAVEFRONTFILESECTION_COMMENT:
                                    continue;
    case GRPFILE3DWAVEFRONTFILESECTION_ELEMENT_NAME:
                                        {
                                          XDEBUG_PRINTCOLOR(0,__L("GRPFILE3DWAVEFRONTFILESECTION_ELEMENT_NAME"));
                                          XSTRING name;
                                          name.AdjustSize(100);
                                          line.UnFormat(__L("# object %s"),name.Get());
                                          if (element!=NULL)
                                            element->GetMesh()->SetFaceBuffers();

                                          lastElementVertexOffset+=vertices.GetSize();
                                          lastElementNormalOffset+=normals.GetSize();
                                          lastElementTCoordsOffset+=tcoords.GetSize();

                                          vertices.DeleteContents();
                                          vertices.DeleteAll();
                                          normals.DeleteContents();
                                          normals.DeleteAll();
                                          tcoords.DeleteContents();
                                          tcoords.DeleteAll();


                                          element=new GRPELEMENT();
                                          element->SetMesh(new GRPMESH());
                                          object->AddElement(element);
                                          element->SetName(name.Get());
                                          element->SetNode(object->CreateNode());
                                          XDEBUG_PRINTCOLOR(0,__L("GRPFILE3DWAVEFRONTFILESECTION_ELEMENT_NAME : %s "),name.Get());
                                          continue;
                                        }
                                        break;
    case GRPFILE3DWAVEFRONTFILESECTION_GROUP_NAME:
                                        {
                                          XSTRING name;
                                          name.AdjustSize(100);
                                          line.UnFormat (__L("%s %s"),type.Get(),name.Get());
                                          element->SetName(name.Get());
                                          XDEBUG_PRINTCOLOR(0,__L("GRPFILE3DWAVEFRONTFILESECTION_GROUP_NAME : %s "),name.Get());

                                          continue;
                                        }
                                        break;

    case GRPFILE3DWAVEFRONTFILESECTION_ELEMENT_MATERIAL:
                                        {
                                          XSTRING name;
                                          name.AdjustSize(_MAXSTR);
                                          line.UnFormat (__L("%s %s"),type.Get(),name.Get());
                                          name.AdjustSize();
                                          GRPMATERIAL* material=GRPMATERIALLIBRARY::Get()->FindByName(name.Get());
                                          element->GetNode()->SetMaterial(material);
                                          CurrentMaterial=0;
                                          XDEBUG_PRINTCOLOR(0,__L("GRPFILE3DWAVEFRONTFILESECTION_ELEMENT_MATERIAL : %s "),name.Get());

                                        }
                                        break;

    case GRPFILE3DWAVEFRONTFILESECTION_ELEMENT_MATERIAL_LIBRARY:
                                        {
                                          XSTRING name;
                                          name.AdjustSize(100);
                                          line.UnFormat (__L("%s %s"),type.Get(),name.Get());
                                          LoadMtl(name.Get(),&xpath);
                                          XDEBUG_PRINTCOLOR(0,__L("GRPFILE3DWAVEFRONTFILESECTION_ELEMENT_MATERIAL_LIBRARY : %s "),name.Get());

                                        }
                                        break;
    case GRPFILE3DWAVEFRONTFILESECTION_ELEMENT_TEXTCOORD:
                                        {
        float x,y,z;
                                        //v  0.7000 1.2000 -0.0000
                                        line.UnFormat (__L("%s %f %f %f"),type.Get(),&x,&y,&z);
                                        tcoords.Add (new GRPPOINT((float)x,(float)y,(float)z,tcoords.GetSize()));
                                        XDEBUG_PRINTCOLOR(0,__L("GRPFILE3DWAVEFRONTFILESECTION_ELEMENT_TEXTCOORD  %s "),type.Get());

                                        }
                                        break;
    case GRPFILE3DWAVEFRONTFILESECTION_ELEMENT_NORMAL:
                                        {
        float x,y,z;
                                        //v  0.7000 1.2000 -0.0000
                                        line.UnFormat (__L("%s %f %f %f"),type.Get(),&x,&y,&z);
                                        normals.Add (new GRPPOINT((float)x,(float)y,(float)z,normals.GetSize()));
                                        XDEBUG_PRINTCOLOR(0,__L("GRPFILE3DWAVEFRONTFILESECTION_ELEMENT_NORMAL  %s "),type.Get());

                                        }
                                        break;
    case GRPFILE3DWAVEFRONTFILESECTION_ELEMENT_VERTEX:
                                        {
        float x,y,z;
                                        //v  0.7000 1.2000 -0.0000
                                        line.UnFormat (__L("%s  %f %f %f"),type.Get(),&x,&y,&z);
                                        vertices.Add  (new GRPPOINT((float)x,(float)y,(float)z,vertices.GetSize()));
                                        XDEBUG_PRINTCOLOR(0,__L("GRPFILE3DWAVEFRONTFILESECTION_ELEMENT_VERTEX  %s "),type.Get());

                                        }
                                        break;
    case GRPFILE3DWAVEFRONTFILESECTION_ELEMENT_FACE:
                                        {
                                          int av,an,at;
                                          int bv,bn,bt;
                                          int cv,cn,ct;
                                          line.UnFormat (__L("%s %d/%d/%d %d/%d/%d %d/%d/%d"),type.Get(),
                                            &av,&at,&an,
                                            &bv,&bt,&bn,
                                            &cv,&ct,&cn);

                                          if (av<0) av=vertices.GetSize()+av+1;
                                          if (an<0) an=normals.GetSize()+an+1;
                                          if (at<0) at=tcoords.GetSize()+at+1;

                                          if (bv<0) bv=vertices.GetSize()+bv+1;
                                          if (bn<0) bn=normals.GetSize() +bn+1;
                                          if (bt<0) bt=tcoords.GetSize() +bt+1;

                                          if (cv<0) cv=vertices.GetSize()+cv+1;
                                          if (cn<0) cn=normals.GetSize() +cn+1;
                                          if (ct<0) ct=tcoords.GetSize() +ct+1;

                                          /*
                                          av-=lastElementVertexOffset;
                                          bv-=lastElementVertexOffset;
                                          cv-=lastElementVertexOffset;
                                          at-=lastElementTCoordsOffset;
                                          bt-=lastElementTCoordsOffset;
                                          ct-=lastElementTCoordsOffset;
                                          an-=lastElementNormalOffset;
                                          bn-=lastElementNormalOffset;
                                          cn-=lastElementNormalOffset;
                                          */
                                          GRPFACE* face=new GRPFACE();
                                          face->vertices[0]           =new GRPPOINT(vertices.Get(av-1)->x,vertices.Get(av-1)->y,vertices.Get(av-1)->z,av-1);
                                          face->vertices[1]           =new GRPPOINT(vertices.Get(bv-1)->x,vertices.Get(bv-1)->y,vertices.Get(bv-1)->z,bv-1);
                                          face->vertices[2]           =new GRPPOINT(vertices.Get(cv-1)->x,vertices.Get(cv-1)->y,vertices.Get(cv-1)->z,cv-1);

                                          face->normals[0]            =new GRPPOINT(normals.Get(an-1)->x,normals.Get(an-1)->y,normals.Get(an-1)->z,an-1);
                                          face->normals[1]            =new GRPPOINT(normals.Get(bn-1)->x,normals.Get(bn-1)->y,normals.Get(bn-1)->z,bn-1);
                                          face->normals[2]            =new GRPPOINT(normals.Get(cn-1)->x,normals.Get(cn-1)->y,normals.Get(cn-1)->z,cn-1);

                                          face->texturecoordinates[0] =new GRPPOINT(tcoords.Get(at-1)->x,tcoords.Get(at-1)->y,tcoords.Get(at-1)->z,at-1);
                                          face->texturecoordinates[1] =new GRPPOINT(tcoords.Get(bt-1)->x,tcoords.Get(bt-1)->y,tcoords.Get(bt-1)->z,bt-1);
                                          face->texturecoordinates[2] =new GRPPOINT(tcoords.Get(ct-1)->x,tcoords.Get(ct-1)->y,tcoords.Get(ct-1)->z,ct-1);

                                          face->normal=GRPVECTOR(*face->normals[0]);
                                          face->a=av-1;
                                          face->b=bv-1;
                                          face->c=cv-1;

                                          face->materialID=CurrentMaterial;

                                          element->GetMesh()->GetFaces()->Add(face);
                                          XDEBUG_PRINTCOLOR(0,__L("GRPFILE3DWAVEFRONTFILESECTION_ELEMENT_FACE  %s "),type.Get());


                                        }
                                        break;
    }
  }
  element->GetMesh()->SetFaceBuffers();
  xfiletxtdevices->Close();

  object->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"STX"));

  return object;
};

/*-------------------------------------------------------------------
//  GRPFILE3DOBJ::LoadMtl
*/
/**
//
//  Loads wavefront material data
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      14/09/2016 13:54:01
//
//  @return       bool : return true on success , false otherwise
//
//  @param        file : aditional file to load (the mtl and obj are separate files)
//  @param        path : path of the file
*/
/*-----------------------------------------------------------------*/
bool  GRPFILE3DOBJ::LoadMtl   (XCHAR* file,XPATH* path)
{
  //Ns = Phong specular component. Ranges from 0 to 1000. (I've seen various statements about this range (see below))
  //Kd = Diffuse color weighted by the diffuse coefficient.
  //Ka = Ambient color weighted by the ambient coefficient.
  //Ks = Specular color weighted by the specular coefficient.
  //d = Dissolve factor (pseudo-transparency). Values are from 0-1. 0 is completely transparent, 1 is opaque.
  //Ni = Refraction index. Values range from 1 upwards. A value of 1 will cause no refraction. A higher value implies refraction.
  //illum = (0, 1, or 2) 0 to disable lighting, 1 for ambient & diffuse only (specular color set to black), 2 for full lighting (see below)
  //sharpness = ? (see below)
  //map_Kd = Diffuse color texture map.
  //map_Ks = Specular color texture map.
  //map_Ka = Ambient color texture map.
  //map_Bump = Bump texture map.
  //map_d = Opacity texture map.
  //refl = reflection type and filename (?)

  XFILETXT* xfiletxtdevices =  new XFILETXT();

  if(!xfiletxtdevices) return false;

  if(!xfiletxtdevices->Open(*path)) return false;

  xfiletxtdevices->ReadAllFile();

  GRPMATERIAL* material=NULL;

  for(int e=0; e<xfiletxtdevices->GetNLines(); e++)
  {
    XSTRING line(xfiletxtdevices->GetLineText(e));
    XSTRING type;

    GLUINT t=GRPFILE3DWAVEFRONTFILESECTION_UNKNOWN;
    type.AdjustSize(_MAXSTR);
    line.UnFormat(__L("%s "),type.Get());
    type.AdjustSize();

    if (type.Compare(__L("newmtl"))==0)  //entrada de nuevo material
    {

      material=new GRPMATERIAL();
      material->GetName()->AdjustSize(100);
      line.UnFormat(__L("%s %s"),type.Get(),material->GetName()->Get());
      material->GetName()->AdjustSize();
      GRPMATERIALLIBRARY::Get()->Add(material);

        for(e++; e<xfiletxtdevices->GetNLines(); e++)
        {
              XSTRING line(xfiletxtdevices->GetLineText(e));
              XSTRING type;
              GLUINT t=GRPFILE3DWAVEFRONTFILESECTION_UNKNOWN;
              type.AdjustSize(50);
              line.DeleteCharacter(__C('\t'),XSTRINGCONTEXT_ATFIRST);
              line.UnFormat(__L("%s "),type.Get());
              type.AdjustSize();

              if (line.Find(__L("newmtl"),true)!=NOTFOUND)
              {
                e--;
                break;
              }

              if (type.Compare(__L("Ns"))==0) continue; //phong data, not needed
              else
              if (type.Compare(__L("Ni"))==0) continue; //phong data, not needed
              else
              if (type.Compare(__L("d"))==0)
              {
                  float d=0.0;
                line.UnFormat(__L("%s %f"),type.Get(),&d);
                material->GetAttributes()->SetDissolve((float)d);
              }
              else
              if (type.Compare(__L("Tr"))==0) continue; //undocumented
              else
              if (type.Compare(__L("illum"))==0)
              {
                int illum=0;
                line.UnFormat(__L("%s %d"),type.Get(),&illum);
                material->GetAttributes()->SetIsIlluminated(illum!=0);
              }
              else
              if (type.Compare(__L("Ka"))==0)
              {
                  float r=0.0,g=0.0,b=0.0;
                line.UnFormat(__L("%s %f %f %f"),type.Get(),&r,&g,&b);
                material->GetAttributes()->SetAmbient(GRPCOLOR((float)r,(float)g,(float)b,1.0f));
              }
              else
              if (type.Compare(__L("Kd"))==0)
              {
                  float r=0.0,g=0.0,b=0.0;
                line.UnFormat(__L("%s %f %f %f"),type.Get(),&r,&g,&b);
                material->GetAttributes()->SetDiffuse(GRPCOLOR((float)r,(float)g,(float)b,1.0f));
              }
              else
              if (type.Compare(__L("Ks"))==0)
              {
                  float r=0.0,g=0.0,b=0.0;
                line.UnFormat(__L("%s %f %f %f"),type.Get(),&r,&g,&b);
                material->GetAttributes()->SetSpecular(GRPCOLOR((float)r,(float)g,(float)b,1.0f));
              }
              else
              if (type.Compare(__L("Ke"))==0)
              {
                  float r=0.0,g=0.0,b=0.0;
                line.UnFormat(__L("%s %f %f %f"),type.Get(),&r,&g,&b);
                material->GetAttributes()->SetEmission(GRPCOLOR((float)r,(float)g,(float)b,1.0f));
              }
              else
              if (type.Compare(__L("map_Kd"))==0)
              {
                XSTRING filename;
                        filename.AdjustSize(200);
                        line.UnFormat(__L("%s %s"),type.Get(),filename.Get());
                        filename.AdjustSize();

                XPATH   p;
                XSTRING fpath;
                XSTRING file;

                        p.Set(filename);
                        p.AdjustSize();
                        p.GetPath(fpath);
                        fpath.AdjustSize();
                        p.GetNamefileExt(file);
                        file.AdjustSize();


              GRPFILEBITMAP* grpfilebitmap = new GRPFILEBITMAP();
                if (grpfilebitmap)
                {
                  GRPBITMAP* bitmap = grpfilebitmap->Load(*path);
                  if (bitmap)
                  {
                  GRPTEXTURE* texture = new GRPTEXTURE();
                  if(texture)
                    {
                      texture->Create(bitmap);
                      material->AddLayer(texture);
                    }
                  delete(grpfilebitmap);
                  grpfilebitmap=NULL;
                  }
                }
              }
        }
    }
  }

  xfiletxtdevices->Close();
  return true;
}