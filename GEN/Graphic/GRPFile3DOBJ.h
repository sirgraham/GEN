/*------------------------------------------------------------------------------------------
//  GRPFILE3DOBJ.H
*/
/**
// \class
//
//  Loads Wavefront Mesh File
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 12/01/2015 16:44:44
//  Last Modification :
*/
//* GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _GRPFILE3DOBJ_H_
#define _GRPFILE3DOBJ_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPFile3D.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
enum GRPFILE3DWAVEFRONTFILESECTION
{
GRPFILE3DWAVEFRONTFILESECTION_UNKNOWN=0,
GRPFILE3DWAVEFRONTFILESECTION_COMMENT,
GRPFILE3DWAVEFRONTFILESECTION_ELEMENT_VERTEX,
GRPFILE3DWAVEFRONTFILESECTION_ELEMENT_NORMAL,
GRPFILE3DWAVEFRONTFILESECTION_ELEMENT_TEXTCOORD,
GRPFILE3DWAVEFRONTFILESECTION_ELEMENT_FACE,
GRPFILE3DWAVEFRONTFILESECTION_GROUP_NAME,
GRPFILE3DWAVEFRONTFILESECTION_ELEMENT_NAME,
GRPFILE3DWAVEFRONTFILESECTION_NAME,
GRPFILE3DWAVEFRONTFILESECTION_ELEMENT_MATERIAL,
GRPFILE3DWAVEFRONTFILESECTION_ELEMENT_MATERIAL_LIBRARY,
GRPFILE3DWAVEFRONTFILESECTION_SMOOTH_SHADING
};

/*---- CLASS -----------------------------------------------------------------------------*/
class GRPFILE3DOBJ : public GRPFILE3D
{
public:
          GRPFILE3DOBJ  () : GRPFILE3D()
          {
          };
  virtual ~GRPFILE3DOBJ () {}

protected:

  virtual GRPOBJECT*  Load      (XCHAR* file);
          bool        LoadMtl   (XCHAR* file,XPATH*);

private:

};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

