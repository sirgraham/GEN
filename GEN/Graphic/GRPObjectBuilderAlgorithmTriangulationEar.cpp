
/*-----------------------------------------------------------------------------------------
//  GRPOBJECTBUILDERTRIANGULIZEALGORITHMEAR.CPP
//
//  Algorithm for EAR triangulation of convex shapes
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 06/06/2014 16:06:18
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include <math.h>
#include "GRPShape.h"
#include "GRPElement.h"
#include "GRPFace.h"
#include "GRPObjectBuilder.h"
#include "GRPObjectBuilderAlgorithmTriangulationEar.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  GRPOBJECTBUILDERALGORITHMTRIANGULIZEEAR::GetIsEar
*/
/**
//
//  Returns if vertex a from GRPPoint Set
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      12/06/2014 11:47:29
//
//  @return       bool :
//
//  @param        vertexlist :
//  @param        point :
*/
/*-----------------------------------------------------------------*/
bool GRPOBJECTBUILDERALGORITHMTRIANGULIZATIONEAR::GetIsEar(XVECTOR<GRPPOINT*>* vertexlist, GRPPOINT* point)
{
  if(!builder) return false;

  GRPPOINT* b;
  GRPPOINT* c;

  int na = 0;
  int nb = 0;
  int nc = 0;


  builder->GetAdyacentPoints(vertexlist, point , &b, &c, &na, &nb, &nc);

  if(!GetIsConvex(point, b, c))   return false;

  XDWORD listSize = vertexlist->GetSize();

  for(XDWORD e=0; e<listSize; e++)
    {
      if (e == na || e == nb || e == nc)
          continue;

      GRPPOINT* _point = vertexlist->FastGet(e);
      if(builder->CheckPointInsideTriangle((_point),(point), (b), (c)))
        return false;
      GRPPOINT result;
    }

  return true;
}





/*-------------------------------------------------------------------
//  GRPOBJECTBUILDERALGORITHMTRIANGULIZATIONEAR::ClipEar
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      26/06/2014 11:59:47
//
//  @param        vertexlist :
//  @param        faceList :
*/
/*-----------------------------------------------------------------*/

class GRPEDGE
{
  GRPPOINT* a;
  GRPPOINT* b;
  float length;
};

int num_clips=0;

bool GRPOBJECTBUILDERALGORITHMTRIANGULIZATIONEAR::ClipEar(XVECTOR<GRPPOINT*>* vertexlist, XVECTOR<GRPFACE*>* facelist)
{
  if(!builder) return false;

  GRPPOINT* point = NULL;

  static GRPPOINT* left= NULL;
  static GRPPOINT* right= NULL;
  static GRPPOINT* top= NULL;
  static GRPPOINT* bottom= NULL;

  builder->GetBoundingVertexesFromPointArray(vertexlist, &top, &left, &bottom, &right);

  if(GetIsEar(vertexlist, left))
    {
      point = left;
    }
   else
    {
      if(GetIsEar(vertexlist, right))
        {
          point = right;
        }
       else
        {
          if(GetIsEar(vertexlist, top))
            {
              point = top;
            }
           else
             {
               if(GetIsEar(vertexlist, bottom))
                 {
                   point = bottom;
                 }
                else
                 {
                   for(XDWORD e=0; e<vertexlist->GetSize(); e++)
                     {
                        if(GetIsEar(vertexlist, vertexlist->Get(e)))
                          point = vertexlist->Get(e);
                     }

                   if(!point)
                   {
                     return false;
                     int i=vertexlist->Find(left);
                     delete vertexlist->Get(i);
                     vertexlist->DeleteIndex(i);
                     if (vertexlist->GetSize()>=3)
                     {
                        ClipEar(vertexlist, facelist);
                        return true;
                     }
                     else return false;
                   }
                 }
              }
          }
      }

  GRPPOINT* b=0;
  GRPPOINT* c=0;

  int na=0,nb=0,nc=0;

  builder->GetAdyacentPoints(vertexlist, point, &b, &c,&na,&nb,&nc);

  if (!((point->x==c->x && point->y==c->y) || (point->x==b->x && point->y==b->y)))
  {
      GRPFACE* face = new GRPFACE();
      if (this->normal.vector[2]>0.0)
      {
        face->Add (new GRPPOINT(point->x, point->y, point->z+z, point->index));
        face->Add (new GRPPOINT(c->x      , c->y      , c->z      +z, c->index));
        face->Add (new GRPPOINT(b->x      , b->y      , b->z      +z, b->index));

        face->a = point->index;
        face->b = c->index;
        face->c = b->index;

        face->normals[0]=new GRPPOINT(0.0f,0.0f,1.0f);
        face->normals[1]=new GRPPOINT(0.0f,0.0f,1.0f);
        face->normals[2]=new GRPPOINT(0.0f,0.0f,1.0f);
      }
      else
      {
        face->Add (new GRPPOINT(point->x, point->y, point->z+z, point->index));
        face->Add (new GRPPOINT(b->x      , b->y      , b->z      +z, b->index));
        face->Add (new GRPPOINT(c->x      , c->y      , c->z      +z, c->index));

        face->a = point->index;
        face->b = b->index;
        face->c = c->index;

        face->normals[0]=new GRPPOINT(0.0f,0.0f,-1.0f);
        face->normals[1]=new GRPPOINT(0.0f,0.0f,-1.0f);
        face->normals[2]=new GRPPOINT(0.0f,0.0f,-1.0f);
      }


      face->normal=GRPVECTOR(0.0,0.0,1.0);
      facelist->Add(face);
  }

  vertexlist->Delete(point);
  delete(point);
  point=NULL;

  int n=vertexlist->GetSize();
  if(n>=3)
    {
      ClipEar(vertexlist, facelist);
    }


  return true;
}





/*-------------------------------------------------------------------
//  GRPOBJECTBUILDERALGORITHMTRIANGULIZATIONEAR::Execute
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      26/06/2014 12:14:36
//
//  @return       GRPELEMENT* :
//
//  @param        shape :
*/
/*-----------------------------------------------------------------*/
GRPELEMENT* GRPOBJECTBUILDERALGORITHMTRIANGULIZATIONEAR::Execute(GRPSHAPE* merged)
{
  XVECTOR<GRPPOINT*>  vertex;

  num_clips=0;
  GRPELEMENT* element     = new GRPELEMENT();
  if (element==NULL) return NULL;
  GRPMESH* mesh           = new GRPMESH();
  if (mesh==NULL) return NULL;
  element->SetMesh(mesh);

  int deep=0;

  XDWORD              nvertex = merged->contours.Get(0)->points.GetSize();
  if (nvertex>=3)
  {
    XDWORD e=0;
    for(e=0; e<nvertex-1; e++)
    {
      GRPPOINT* point = new GRPPOINT(merged->contours.Get(0)->points.Get(e)->value);
      if(point)
      {
              point->index=e;
              if (!vertex.Add(point))
                delete point;
      }
      else
        break;
    }
    if (e<nvertex-1) //not full
      return NULL;

  element->GetMesh()->SetMaxindex(e);

    //recursive call
  if(!ClipEar(&vertex, element->GetMesh()->GetFaces()))
    {
      vertex.DeleteContents();
      vertex.DeleteAll();
      delete  merged;
      return NULL;
    }
  }

  this->material = material;

  vertex.DeleteContents   ();
  vertex.DeleteAll        ();

  return element;
}




/*-------------------------------------------------------------------
//  GRPOBJECTBUILDERALGORITHMTRIANGULIZATIONEAR::DotProduct
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      26/06/2014 12:26:47
//
//  @return       float :
//
//  @param        ax :
//  @param        ay :
//  @param        bx :
//  @param        by :
//  @param        cx :
//  @param        cy :
*/
/*-----------------------------------------------------------------*/
float GRPOBJECTBUILDERALGORITHMTRIANGULIZATIONEAR::DotProduct(GLFLOAT ax,GLFLOAT ay,GLFLOAT bx,GLFLOAT by,GLFLOAT cx,GLFLOAT cy)
{
  GLFLOAT bax = ax - bx;
  GLFLOAT bay = ay - by;
  GLFLOAT bcx = cx - bx;
  GLFLOAT bcy = cy - by;

  GLFLOAT dotproduct = (bax * bcx) + (bay * bcy);

  return dotproduct;
}



/*-------------------------------------------------------------------
//  GRPOBJECTBUILDERALGORITHMTRIANGULIZATIONEAR::CrossProductLength
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      26/06/2014 12:26:52
//
//  @return       float :
//
//  @param        ax :
//  @param        ay :
//  @param        bx :
//  @param        by :
//  @param        cx :
//  @param        cy :
*/
/*-----------------------------------------------------------------*/
float GRPOBJECTBUILDERALGORITHMTRIANGULIZATIONEAR::CrossProductLength(GLFLOAT ax,GLFLOAT ay,GLFLOAT bx,GLFLOAT by,GLFLOAT cx,GLFLOAT cy)
{
  GLFLOAT bax = ax - bx;
  GLFLOAT bay = ay - by;
  GLFLOAT bcx = cx - bx;
  GLFLOAT bcy = cy - by;

  GLFLOAT crossproductlength = (bax * bcy) - (bay * bcx);

  return crossproductlength;
}




/*-------------------------------------------------------------------
//  GRPOBJECTBUILDERALGORITHMTRIANGULIZATIONEAR::GetAngleToHorizontalaxis
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      26/06/2014 12:28:05
//
//  @return       float :
//
//  @param        *point1 :
//  @param        *point2 :
*/
/*-----------------------------------------------------------------*/
float GRPOBJECTBUILDERALGORITHMTRIANGULIZATIONEAR::GetAngleToHorizontalaxis(GRPPOINT *point1,GRPPOINT *point2)
{
  float xdiff = point2->x - point1->x;
  float ydiff = point2->y - point1->y;

  float angleAB= (float)atan2(ydiff, xdiff) * (180 / PI);

  if (angleAB<0) return 360.0f+angleAB;

  return angleAB;
}
