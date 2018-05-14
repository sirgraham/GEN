
/*------------------------------------------------------------------------------------------
//  GRPMAPPINGPROJECTIONCYLINDER.CPP
//
//  Cylindrical U,V projection
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 18/08/2014 17:00:48
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include <math.h>

#include "GRPElement.h"

#include "GRPMappingProjectionCylinder.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  GRPMAPPINGPROJECTIONCYLINDER::GRPMAPPINGPROJECTIONCYLINDER
*/
/**
//
//  Class Constructor GRPMAPPINGPROJECTIONCYLINDER
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      18/08/2014 17:02:36
//
*/
/*-----------------------------------------------------------------*/
GRPMAPPINGPROJECTIONCYLINDER::GRPMAPPINGPROJECTIONCYLINDER() : GRPMAPPINGPROJECTION()
{
  radius = 1.0;
}



/*-------------------------------------------------------------------
//  GRPMAPPINGPROJECTIONCYLINDER::GetAngleToHorizontalAxis
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      18/08/2014 17:02:54
//
//  @return       float :
//
//  @param        *p1 :
//  @param        *p2 :
*/
/*-----------------------------------------------------------------*/
float GRPMAPPINGPROJECTIONCYLINDER::GetAngleToHorizontalAxis(GRPPOINT *p1, GRPPOINT *p2)
{
  float xdiff   = p2->x - p1->x;
  float ydiff   = p2->y - p1->y;

  float angleAB = (float)atan2(ydiff, xdiff) * (180 / PI);

  if (angleAB<0) return 360.0f+angleAB;

  return angleAB;
}



/*-------------------------------------------------------------------
//  GRPMAPPINGPROJECTIONCYLINDER::ProjectMapping
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      18/08/2014 17:07:19
//
//  @return       bool :
//
//  @param        object :
*/
/*-----------------------------------------------------------------*/
bool GRPMAPPINGPROJECTIONCYLINDER::ProjectMapping(GRPELEMENT* object)
{
  object->UpdateBoundingBox();

  GLFLOAT width   = object->GetBoundingBox()->maxpoint.x - object->GetBoundingBox()->minpoint.x;
  GLFLOAT height  = object->GetBoundingBox()->maxpoint.y - object->GetBoundingBox()->minpoint.y;
  GLFLOAT depth   = object->GetBoundingBox()->maxpoint.z - object->GetBoundingBox()->minpoint.z;

  for(XDWORD e=0;e<object->GetMesh()->GetFaces()->GetSize();e++)
    {
      GLFLOAT u = 0.0f;
      GLFLOAT v = 0.0f;

      for(int i=0; i<3; i++)
        {
          //GRPPOINT* a = object->GetFaces()->Get(e)->vertices.Get(i);
          GRPPOINT* a = object->GetMesh()->GetFaces()->Get(e)->vertices[i];
          GLFLOAT   x = a->x;
          GLFLOAT   y = a->y;
          GLFLOAT   z = a->z;

          GRPPOINT b(a->x,a->z,a->y);  //swap z and y for calculation
          GRPPOINT o(0,0);             //angle to horizontal

          float angle = GetAngleToHorizontalAxis(&o,&b); //angle in degrees

          x = (x - object->GetBoundingBox()->minpoint.x) / width;
          y = (y - object->GetBoundingBox()->minpoint.y) / height;
          z = (z - object->GetBoundingBox()->minpoint.z) / depth;

          u = angle/360.0f; //angle in degrees to 0-1 uv range
          v = y;            // v is normalized

          /*object->GetFaces()->Get(e)->texturecoordinates.Get(i)->x=1-u;
          object->GetFaces()->Get(e)->texturecoordinates.Get(i)->y=v; */
          object->GetMesh()->GetFaces()->Get(e)->texturecoordinates[i]->x=1-u;
          object->GetMesh()->GetFaces()->Get(e)->texturecoordinates[i]->y=v;
        }
    }

  ReassignUVs(object);

  return true;
}
