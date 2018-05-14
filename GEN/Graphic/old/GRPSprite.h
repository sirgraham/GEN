/*------------------------------------------------------------------------------------------
//  GRPSPRITE.H
*/
/**
// \class
//
//  Sprite objects
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 02/09/2014 12:42:50
//  Last Modification :
*/
//* GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _GRPSPRITE_H_
#define _GRPSPRITE_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

#include "GRPMatrix.h"
#include "GRPElement.h"

/*---- CLASS -----------------------------------------------------------------------------*/

class GRPSPRITE : public GRPELEMENT
{
public:

  enum TYPE
  {
    POINT=0,
    QUAD,
    STRIP,
    BILLBOARD
  };

                GRPSPRITE ();

          float frameStep;
          bool  ScreenSpaceAligned;
          bool  FixedSize;

  virtual bool  Draw              (GRPMATRIX& viewmatrix, GRPMATRIX* viewprojection,  GRPMATRIX* projection)=0;
private:

  void  Clean()
        {
            frameStep           =1.0f;
            renderpriority      =1;
            ScreenSpaceAligned  =true;
            FixedSize           =false;
            this->rendermode    =GRPRENDERMODE_POINTS;
        }
};


class GRPSPRITEPOINT : public GRPSPRITE
{
public:
  GRPSPRITEPOINT()
  {
    //GRPNAMED::classtype = GRPNAMED_TYPE_GRPSPRITEPOINT;
  }
        bool    Draw              (GRPMATRIX& viewmatrix, GRPMATRIX* viewprojection,  GRPMATRIX* projection);
};

class GRPSPRITEQUAD : public GRPSPRITE
{
public:
                GRPSPRITEQUAD     ()
                {
                  this->rendermode    =GRPRENDERMODE_TRIANGLE_STRIP;
                  //GRPNAMED::classtype = GRPNAMED_TYPE_GRPSPRITEQUAD;
                }
        bool    Draw              (GRPMATRIX& viewmatrix, GRPMATRIX* viewprojection,  GRPMATRIX* projection);
};

class GRPSPRITESTRIP  : public GRPSPRITEQUAD
{
public:
                GRPSPRITESTRIP    ()
                {
                  //GRPNAMED::classtype = GRPNAMED_TYPE_GRPSPRITESTRIP;
                }
        bool    Draw              (GRPMATRIX* viewmatrix, GRPMATRIX* viewprojection,  GRPMATRIX* projection);
};

class GRPBILLBOARD : public GRPSPRITEQUAD
{
public:
                GRPBILLBOARD      ()
                {
                  GRPNAMED::classtype = GRPNAMED_TYPE_GRPBILLBOARD;
                }

        bool    Draw              (GRPMATRIX* viewmatrix, GRPMATRIX* viewprojection,  GRPMATRIX* projection);
};
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

