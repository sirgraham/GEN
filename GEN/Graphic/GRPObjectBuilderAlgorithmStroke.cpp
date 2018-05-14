
/*------------------------------------------------------------------------------------------
//  GRPOBJECTBUILDERALGORITHMSTROKE.cpp
*/
/**
// \class
//
//  gives width to shape
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 25/11/2014 11:15:51
//  Last Modification :
//
// http://www.codeproject.com/Articles/226569/Drawing-polylines-by-tessellation
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPShape.h"
#include "GRPObjectBuilderAlgorithmStroke.h"
#include "GRPObjectBuilder.h"

#include "XMemory.h"

/*-------------------------------------------------------------------
//  GRPOBJECTBUILDERALGORITHMSTROKE::GRPOBJECTBUILDERALGORITHMSTROKE
*/
/**
//
//  Class Constructor GRPOBJECTBUILDERALGORITHMSTROKE
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      01/12/2014 13:32:20
//
//  @param        builder :
*/
/*-----------------------------------------------------------------*/

GRPOBJECTBUILDERALGORITHMSTROKE::GRPOBJECTBUILDERALGORITHMSTROKE(GRPOBJECTBUILDER* builder)
{
  this->builder=builder;
  Clean();
}

/*-------------------------------------------------------------------
//  GRPOBJECTBUILDERALGORITHMSTROKE::Execute
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      01/12/2014 13:32:15
//
//  @return       GRPELEMENT* :
//
//  @param        shape :
*/
/*-----------------------------------------------------------------*/
GRPELEMENT*   GRPOBJECTBUILDERALGORITHMSTROKE::Execute(GRPSHAPE* shape)
{
  if(!builder) return NULL;
  GRPELEMENT* element = new GRPELEMENT();
  GRPMESH* mesh = new GRPMESH();

  element->SetMesh(mesh);
  XVECTOR<GRPFACE*>*  faces = element->GetMesh()->GetFaces();

  XDWORD nv=shape->contours.Get(0)->points.GetSize();

    faces->SetAddInLimit(nv*2*nsegments);

  XVECTOR<GRPPOINT*> Stack;

  GRPSPLINE* spline=shape->contours.Get(0);
  XDWORD n=shape->contours.Get(0)->points.GetSize();
  int index=0;
  int max=11;
  int start=1;

  for (XDWORD e=start;e<n;e++)
  {
    GRPPOINT p1,p2;

    p1=spline->points.Get((e-1)%n)->value;
    p2=spline->points.Get(e%n)->value;

    GRPVECTOR vl=GRPVECTOR(p2-p1);
    vl.Normalize(); //tangent

    GRPPOINT*  v[4];

    GRPPOINT vp(-(vl.vector[1]), vl.vector[0],0.0f);  // compute the vector perpendicular to tangent

        v[0] = new GRPPOINT(p1 + (vp* innerwidth));
        v[0]->z = p1.z + bevel;

        v[1] = new GRPPOINT(p1 - (vp* outerwidth));
        v[1]->z = p1.z;

        v[2] = new GRPPOINT(p2 + (vp* innerwidth));
        v[2]->z=p2.z+bevel;

        v[3] = new GRPPOINT(p2 - (vp* outerwidth));
        v[3]->z=p2.z;

    if (e==start) //initial block
    {
        v[0]->index=index++;
        v[1]->index=index++;
        v[2]->index=index++;
        v[3]->index=index++;

        Stack.Add(v[0]);
        Stack.Add(v[1]);
        Stack.Add(v[2]);
        Stack.Add(v[3]);

    }
    else
    {
        GRPPOINT IntersectionA;
        GRPPOINT IntersectionB;

        switch(jointtype)
        {
            case GRPOBJECTBUILDERALGORITHMSTROKEJOINTTYPE_AXED:
                v[2]->index=index++;
                v[3]->index=index++;


                Stack.Add(v[2]);
                Stack.Add(v[3]);
                break;
            //------------------------------------------------------------------ BEVEL
            case GRPOBJECTBUILDERALGORITHMSTROKEJOINTTYPE_BEVEL:
                            Bevel(&Stack,e,v,shape,&index);
                            break;
            //------------------------------------------------------------------ MITTER
            case GRPOBJECTBUILDERALGORITHMSTROKEJOINTTYPE_MITTER:
                  {
                    /*
                    Falta pasarlo a una funcion como el bevel, pero aún tengo que mejorar
                    el bevel para algunas excepciones con los ángulos así que cuando termine
                    con el bevel refactorizo el mitter
                    */
                        GRPPOINT* ps0 = Stack.Get(index-4);
                        GRPPOINT* ps1 = Stack.Get(index-3);
                        GRPPOINT* ps2 = Stack.Get(index-2);
                        GRPPOINT* ps3 = Stack.Get(index-1); //we calculate current block


                            //--------------------standard segment intersection with last
                            if (GRPVECTOR::LineIntersects(v[0], v[2], ps0, ps2, &IntersectionA))
                            {
                                //line intersects
                                float z = v[0]->z;
                                v[0]->z = z;
                                Stack.Get(index - 2)->z = z;
                            }

                            if (GRPVECTOR::LineIntersects(v[1], v[3], ps1, ps3, &IntersectionB))
                            {
                                //line intersects
                                float z = v[1]->z;
                                v[1]->z = z;
                                Stack.Get(index - 1)->z = z;
                            }

                        //---------------------- check if angle is so acute by measuring the distance between mitter points using fake hipo
                        //if (mitterTreshold!=0.0f)
                        {
                          float x   =IntersectionB.x-IntersectionA.x;
                          float y   =IntersectionB.y-IntersectionA.y;

                            if ((x*x + y*y) < mitterTreshold)
                            {
                                //--- damos una curva
                              Stack.Add(new GRPPOINT(v[0]->x,v[0]->y,v[0]->z,index++));
                              Stack.Add(new GRPPOINT(v[1]->x,v[1]->y,v[1]->z,index++));
                            }
                            else
                            {
                                Stack.Get(index-2)->x=IntersectionA.x;
                                Stack.Get(index-2)->y=IntersectionA.y;
                                Stack.Get(index-1)->x=IntersectionB.x;
                                Stack.Get(index-1)->y=IntersectionB.y;
                            }
                        }
                }

                v[2]->index=index++;
                v[3]->index=index++;


                Stack.Add(v[2]);
                Stack.Add(v[3]);
                delete(v[0]);
                v[0]=NULL;
                delete(v[1]);
                v[1]=NULL;
                  break;

              //-------------------------------------------------------------- NONE
              default:
                  break;
          }
    }

  }

  int nstack = Stack.GetSize();
//------------- triangulate
  for (int i=0;i<=nstack-2;i+=2)
  {

    GRPPOINT*  v[4];

    v[0]=Stack.Get((i+0) % nstack);
    v[1]=Stack.Get((i+1) % nstack);
    v[2]=Stack.Get((i+2) % nstack);
    v[3]=Stack.Get((i+3) % nstack);

    GRPFACE*  face=new GRPFACE();

    if(face)
      {
        face->Add(new GRPPOINT(*(v[0])));
        face->Add(new GRPPOINT(*(v[1])));
        face->Add(new GRPPOINT(*(v[2])));

        face->normal=GRPVECTOR(0.0,0.0,1.0);

        face->texturecoordinates[0] = new GRPPOINT(1.0, 0.0);
        face->texturecoordinates[1] = new GRPPOINT(0.0, 0.0);
        face->texturecoordinates[2] = new GRPPOINT(1.0, 0.0);
        face->a=v[0]->index;
        face->b=v[1]->index;
        face->c=v[2]->index;

        faces->Add(face);

        if (doublesided)
        {
        face=new GRPFACE();
        face->Add(new GRPPOINT(v[0]->x,v[0]->y,v[0]->z+extrude,index++));
        face->Add(new GRPPOINT(v[2]->x,v[2]->y,v[2]->z+extrude,index++));
        face->Add(new GRPPOINT(v[1]->x,v[1]->y,v[1]->z+extrude,index++));

        face->normal=GRPVECTOR(0.0,0.0,-1.0);

        face->a=face->vertices[0]->index;
        face->b=face->vertices[1]->index;
        face->c=face->vertices[2]->index;

        faces->Add(face);
        }

      }

  face=new GRPFACE();
  if(face)
      {
        face->Add(new GRPPOINT(*(v[1])));
        face->Add(new GRPPOINT(*(v[3])));
        face->Add(new GRPPOINT(*(v[2])));

        face->normal=GRPVECTOR(0.0,0.0,1.0);
        face->texturecoordinates[0] = new GRPPOINT(0.0, 0.0);
        face->texturecoordinates[1] = new GRPPOINT(0.0, 0.0);
        face->texturecoordinates[2] = new GRPPOINT(1.0, 0.0);
        face->a=face->vertices[0]->index;
        face->b=face->vertices[1]->index;
        face->c=face->vertices[2]->index;

        faces->Add(face);

        if (doublesided)
        {
        face=new GRPFACE();
        face->Add(new GRPPOINT(v[1]->x,v[1]->y,v[1]->z+extrude,index++));
        face->Add(new GRPPOINT(v[2]->x,v[2]->y,v[2]->z+extrude,index++));
        face->Add(new GRPPOINT(v[3]->x,v[3]->y,v[3]->z+extrude,index++));

        face->normal=GRPVECTOR(0.0,0.0,-1.0);

        face->a=face->vertices[0]->index;
        face->b=face->vertices[1]->index;
        face->c=face->vertices[2]->index;

        faces->Add(face);
        }
      }
        ///--------- double side
  }

  //---------------------------------------- Extrude
  if (extrude!=0.0)
  for (XDWORD e=1,i=0;e<shape->contours.Get(0)->points.GetSize();e++,i+=2)
  {

    GRPPOINT*  v[4];

    v[0]=Stack.Get(i);
    v[1]=Stack.Get(i+1);
    v[2]=Stack.Get(i+2);
    v[3]=Stack.Get(i+3);

    GRPFACE*  face=new GRPFACE();
    GRPVECTOR* N=0;
    if(face)
      {
        face->Add(new GRPPOINT(*(v[0])));
        face->Add(new GRPPOINT(*(v[2])));
        face->Add(new GRPPOINT(v[0]->x,v[0]->y,v[0]->z+extrude,index++));

        face->CalculateNormal();
        N=&face->normal;

        face->a=face->vertices[0]->index;
        face->b=face->vertices[1]->index;
        face->c=face->vertices[2]->index;

        faces->Add(face);
      }

  face=new GRPFACE();
  if(face)
      {
        face->Add(new GRPPOINT(*(v[2])));
        face->Add(new GRPPOINT(v[2]->x, v[2]->y,v[2]->z+  extrude,index++));
        face->Add(new GRPPOINT(v[0]->x, v[0]->y,v[0]->z+  extrude,index++));

        face->normal=*N;

        face->a=face->vertices[0]->index;
        face->b=face->vertices[1]->index;
        face->c=face->vertices[2]->index;

        faces->Add(face);
      }
  face=new GRPFACE();
  if(face)
      {
        face->Add(new GRPPOINT(*(v[1])));
        face->Add(new GRPPOINT(v[1]->x,v[1]->y,v[1]->z+extrude+bevel,index++));
        face->Add(new GRPPOINT(*(v[3])));

        face->CalculateNormal();
        N=&face->normal;

        face->a=face->vertices[0]->index;
        face->b=face->vertices[1]->index;
        face->c=face->vertices[2]->index;

        faces->Add(face);
      }

  face=new GRPFACE();
  if(face)
      {
        face->Add(new GRPPOINT(*(v[3])));
        face->Add(new GRPPOINT(v[1]->x, v[1]->y,  v[1]->z+extrude+bevel,index++));
        face->Add(new GRPPOINT(v[3]->x, v[3]->y,  v[3]->z+extrude+bevel,index++));

        face->normal=*N;

        face->a=face->vertices[0]->index;
        face->b=face->vertices[1]->index;
        face->c=face->vertices[2]->index;

        faces->Add(face);
      }
  }


  element->GetMesh()->SetMaxindex(index);
  Stack.DeleteContents();
  Stack.DeleteAll();

  return element;
}



/*-------------------------------------------------------------------
//  GRPOBJECTBUILDERALGORITHMSTROKE::Bevel
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      05/12/2014 13:44:01
//
//  @param        Stack :
//  @param        e :
//  @param        v :
//  @param        shape :
//  @param        index :
*/
/*-----------------------------------------------------------------*/
void    GRPOBJECTBUILDERALGORITHMSTROKE::Bevel  (XVECTOR<GRPPOINT*>* Stack, int e, GRPPOINT**  v, GRPSHAPE* shape, int* index)
{
  GRPPOINT IntersectionA,IntersectionB;
  int n=shape->contours.Get(0)->points.GetSize();

  //-------------- lasts

    GRPPOINT* ps0 = Stack->Get((*index)-4);
    GRPPOINT* ps1 = Stack->Get((*index)-3);
    GRPPOINT* ps2 = Stack->Get((*index)-2);
    GRPPOINT* ps3 = Stack->Get((*index)-1);

//-------- test case
    GRPPOINT IntersectsDBDB;
    GRPPOINT IntersectsACDB;
    GRPPOINT IntersectsDBCA;

    bool dbdb=GRPVECTOR::LineIntersects(ps1,ps3,v[1],v[3],&IntersectsDBDB);
    bool acdb=GRPVECTOR::LineIntersects(ps0,ps2,v[1],v[3],&IntersectsACDB);
    bool dbca=GRPVECTOR::LineIntersects(ps1,ps3,v[0],v[1],&IntersectsDBCA);

    if (dbdb && !acdb) //case 2
    {
                ps3->x=IntersectsDBDB.x;
                ps3->y=IntersectsDBDB.y;
                v[2]->index=(*index)++;
                v[3]->index=(*index)++;
                Stack->Add(v[2]);
                Stack->Add(v[3]);
    }
    else
    if (!dbdb && !acdb && !dbca) //case 3
    {
                v[2]->x=IntersectsDBDB.x;
                v[2]->y=IntersectsDBDB.y;
                v[2]->index=(*index)++;
                v[3]->index=(*index)++;
                Stack->Add(v[2]);
                Stack->Add(v[3]);
    }
    else
    if (dbdb && dbca) //case 4
    {

    }
    else //case 1 , is not possible
    {
                v[2]->index=(*index)++;
                v[3]->index=(*index)++;
                Stack->Add(v[2]);
                Stack->Add(v[3]);
      XDEBUG_PRINTCOLOR(1,__L("GRPOBJECTBUILDERALGORITHMSTROKE::Bevel : Imposible condition found!"));
      return;
    }

  /*
              Stack->Add(new GRPPOINT(v[0]->x,v[0]->y,v[0]->z,(*index)++));
              Stack->Add(new GRPPOINT(v[1]->x,v[1]->y,v[1]->z,(*index)++));

                              if(e==n-1 && closeSpline) //last segment (intersects with first one in the stack)
                              {
                                GRPPOINT* st0 = Stack->Get(0);
                                GRPPOINT* st1 = Stack->Get(1);
                                GRPPOINT* st2 = Stack->Get(2);
                                GRPPOINT* st3 = Stack->Get(3);

                                if (GRPVECTOR::LineIntersects(v[0],v[2],st0,st2,&IntersectionA))
                                {
                                //line intersects
                                float z=v[2]->z;
                                *(v[2])=IntersectionA;
                                v[2]->z=z;

                                  Stack->Get(0)->x=IntersectionA.x;
                                  Stack->Get(0)->y=IntersectionA.y;
                                  Stack->Get(0)->z=z;
                                }
                                if (GRPVECTOR::LineIntersects(v[1],v[3],st1,st3,&IntersectionB))
                                {
                                  //line intersects
                                  float z=v[3]->z;
                                  *(v[3])=IntersectionB;
                                  v[3]->z=z;
                                  Stack->Get(1)->x=IntersectionB.x;
                                  Stack->Get(1)->y=IntersectionB.y;
                                  Stack->Get(1)->z=z;
                                }
                            }
                            */

}