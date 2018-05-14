
/*------------------------------------------------------------------------------------------
//  GRPMAPPINGPROJECTIONPLANAR.CPP
//
//  Planar UV's projection
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 18/08/2014 16:42:49
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPElement.h"
#include "GRPMappingProjectionPlanar.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  GRPMAPPINGPROJECTIONPLANAR::ProjectMapping
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      18/08/2014 16:55:53
//
//  @return       bool :
//
//  @param        object :
*/
/*-----------------------------------------------------------------*/
bool GRPMAPPINGPROJECTIONPLANAR::ProjectMapping(GRPELEMENT* object)
{
  object->UpdateBoundingBox();
  GRPBB* boundingbox=object->GetBoundingBox();

  GLFLOAT width  =  boundingbox->maxpoint.x - boundingbox->minpoint.x;
  GLFLOAT height =  boundingbox->maxpoint.y - boundingbox->minpoint.y;
  GLFLOAT depth  =  boundingbox->maxpoint.z - boundingbox->minpoint.z;

  this->node->Update();
  object->Finish();
  for(int e=object->GetMesh()->GetFaces()->GetSize()-1; e>=0; e--)
    {
      for(int i=2; i>=0; i--)
        {
          GRPPOINT* v = object->GetMesh()->GetFaces()->Get(e)->vertices[i];

          GRPMATRIX m = this->node->GetWorldMatrix();
          GRPVECTOR a = m.TransformPoint(*v);


          if (object->GetMesh()->GetFaces()->Get(e)->texturecoordinates[i]==NULL)
            object->GetMesh()->GetFaces()->Get(e)->texturecoordinates[i]=new GRPPOINT();

              if (direction.vector[0]<=direction.vector[1])
              {

              GLFLOAT u=(a.vector[0] - object->GetBoundingBox()->minpoint.x) / width;
              GLFLOAT v=(a.vector[1] - object->GetBoundingBox()->minpoint.y) / height;

              object->GetMesh()->GetFaces()->Get(e)->texturecoordinates[i]->x = (u * scalefactor.vector[0]) + position.vector[0];
              object->GetMesh()->GetFaces()->Get(e)->texturecoordinates[i]->y = (v * scalefactor.vector[1]) + position.vector[1];

              }
              else
              {

              GLFLOAT u = 1.0f  - ( (a.vector[0] - object->GetBoundingBox()->minpoint.x) / width);
              GLFLOAT v =           (a.vector[2] - object->GetBoundingBox()->minpoint.z) / depth;

              object->GetMesh()->GetFaces()->Get(e)->texturecoordinates[i]->x = (u * scalefactor.vector[0]) + position.vector[0];
              object->GetMesh()->GetFaces()->Get(e)->texturecoordinates[i]->y = (v * scalefactor.vector[2]) + position.vector[1];

              }
        }
    }
  object->GetMesh()->SetFaceBuffers();


  return true;
}
