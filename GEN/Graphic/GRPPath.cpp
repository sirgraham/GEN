
/*------------------------------------------------------------------------------------------
//  GRPPATH.CPP
//
//  Shapes for 3d deformations
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 28/11/2014 10:25:32
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPPath.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/

/*-------------------------------------------------------------------
//  GRPPATH::GRPPATH
*/
/**
//
//  Class Constructor GRPPATH
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      03/12/2014 10:32:59
//
*/
/*-----------------------------------------------------------------*/
GRPPATH::GRPPATH    ()
{
  Clean();
  GRPNAMED::classtype = GRPNAMED_TYPE_GRPPATH;
}

/*-------------------------------------------------------------------
//  GRPPATH::~GRPPATH
*/
/**
//
//   Class Destructor GRPPATH
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      03/12/2014 10:33:16
//
*/
/*-----------------------------------------------------------------*/
GRPPATH::~GRPPATH ()
{

}
/*-------------------------------------------------------------------
//  GRPPATH::Update
*/
/**
//
//  Updates path node acording to current step
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      03/12/2014 10:33:26
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool GRPPATH::Update  ()
{
  if (!active) return false;

  GetSegment(currentstep,&this->position,&this->rotation,&this->scale);
  GRPNODE::Update();

  loopDirection ? currentstep+=step : currentstep-=step;

  //------------------------------------ begin
    if (currentstep>loopEnd)
    {
      switch(loopType)
      {
      case GRPPATH_LOOP_REPEAT:
              currentstep=loopStart;
              break;
      case GRPPATH_LOOP_OSCILLATE:
              currentstep=loopEnd;
              loopDirection=0;
              break;
      default:
              active=false;
              currentstep=loopEnd;
              break;
      }
    }
  //------------------------------------ begin
    if (currentstep<loopStart)
    {
      switch(loopType)
      {
      case GRPPATH_LOOP_REPEAT:
              currentstep=loopEnd;
              break;
      case GRPPATH_LOOP_OSCILLATE:
              currentstep=loopStart;
              loopDirection=1;
              break;
      default:
              active=false;
              currentstep=loopStart;
              break;
      }
    }


  return true;
}

/*-------------------------------------------------------------------
//  GRPPATH::Set
*/
/**
//
//  Set current step
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      03/12/2014 10:33:57
//
//  @param        f :
*/
/*-----------------------------------------------------------------*/
void GRPPATH::Set     (float f)
{
  this->currentstep=f;
}

/*-------------------------------------------------------------------
//  GRPPATH::OnLinkedTo
*/
/**
//
//  On linked event
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      03/12/2014 10:34:11
//
//  @return       bool :
//
//  @param        node :
*/
/*-----------------------------------------------------------------*/
bool GRPPATH::OnLinkedTo    (GRPNODE* node)
{
  GRPNODE::Update();
  this->Push();
  this->GetModelMatrix()->LoadIdentity();
  return true;
}

/*-------------------------------------------------------------------
//  GRPPATH::GetSegment
*/
/**
//
//  Gets current position,rotation,scale acording to current step
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      03/12/2014 10:34:29
//
//  @param        step :
//  @param        position :
//  @param        rotation :
//  @param        scale :
*/
/*-----------------------------------------------------------------*/
void  GRPPATH::GetSegment   (float step, GRPVECTOR* position,GRPVECTOR* rotation,GRPVECTOR* scale)
{
  float t=0.0f;
  float fragment=0;
  XDWORD e=0;

          //----------------------------------- travel
          if (splineTravel && position && rotation)
          {
            float v=0.0f;
            for (;e<splineTravel->percents.GetSize()-1;e++)
            {
              v=splineTravel->percents.Get(e);
              if ((v+t)>currentstep) break;
              t+=v;
            }
            e--;
            fragment=currentstep-t;
            float width=splineTravel->percents.Get(e+1);
            fragment/=width;
            GRPVECTOR p=GRPPOINT::Interpolate(fragment,&(splineTravel->points.Get(e)->value),&(splineTravel->points.Get(e+1)->value));

            GRPVECTOR p2=splineTravel->points.Get(e+1)->value;

            float angle=GRPVECTOR::AngleBetween(&p,&p2);
            if (e<splineTravel->points.GetSize()-3)
            {
              GRPVECTOR p3=splineTravel->points.Get(e+2)->value;

              float angle2=GRPVECTOR::AngleBetween(&p2,&p3);
              angle=GRPPOINT::Interpolate(fragment,angle,angle2);
            }


            this->degrees=angle-90.0f;
            this->rotation.vector[0]=0.0;
            this->rotation.vector[1]=0.0;
            this->rotation.vector[2]=1.0;

          *position=p;
            return;
          }

          //----------------------------------- position
          if (position && splinePosition)
          {
          float v=0.0f;
            for (;e<splinePosition->percents.GetSize()-1;e++)
            {
              v=splinePosition->percents.Get(e);
              if ((v+t)>currentstep) break;
              t+=v;
            }
            e--;
            fragment=currentstep-t;
            float width=splinePosition->percents.Get(e+1);
            fragment/=width;
            GRPPOINT p=GRPPOINT::Interpolate(fragment,&(splinePosition->points.Get(e)->value),&(splinePosition->points.Get(e+1)->value));

          *position=p;
            return;
          }

          //----------------------------------- rotation
          if (rotation && splineRotationX)
          {
            float v=0.0f;
            for (;e<splineRotationX->percents.GetSize()-1;e++)
            {
              v=splineRotationX->percents.Get(e);
              if ((v+t)>currentstep) break;
              t+=v;
            }
            e--;
            fragment=currentstep-t;
            float width=splineRotationX->percents.Get(e+1);
            fragment/=width;
            GRPPOINT p=GRPPOINT::Interpolate(fragment,&(splineRotationX->points.Get(e)->value),&(splineRotationX->points.Get(e+1)->value));

            this->degrees=360.0f*splineRotationX->points.Get(e)->value.y;
            (*rotation).vector[0]=1.0;
            (*rotation).vector[1]=0.0;
            (*rotation).vector[2]=0.0;
          }
          //----------------------------------- rotation
          if (rotation && splineRotationY)
          {
            float v=0.0f;
            for (;e<splineRotationY->percents.GetSize()-1;e++)
            {
              v=splineRotationY->percents.Get(e);
              if ((v+t)>currentstep) break;
              t+=v;
            }
            e--;
            fragment=currentstep-t;
            float width=splineRotationY->percents.Get(e+1);
            fragment/=width;
            GRPPOINT p=GRPPOINT::Interpolate(fragment,&(splineRotationY->points.Get(e)->value),&(splineRotationY->points.Get(e+1)->value));

            this->degrees=360.0f*splineRotationY->points.Get(e)->value.y;
            (*rotation).vector[0]=0.0;
            (*rotation).vector[1]=1.0;
            (*rotation).vector[2]=0.0;
          }
        //----------------------------------- rotation
          if (rotation && splineRotationZ)
          {
            float v=0.0f;
            for (;e<splineRotationZ->percents.GetSize()-1;e++)
            {
              v=splineRotationZ->percents.Get(e);
              if ((v+t)>currentstep) break;
              t+=v;
            }
            e--;
            fragment=currentstep-t;
            float width=splineRotationZ->percents.Get(e+1);
            fragment/=width;
            GRPPOINT p=GRPPOINT::Interpolate(fragment,&(splineRotationZ->points.Get(e)->value),&(splineRotationZ->points.Get(e+1)->value));

            this->degrees=360.0f*splineRotationZ->points.Get(e)->value.y;
            (*rotation).vector[0]=0.0;
            (*rotation).vector[1]=1.0;
            (*rotation).vector[2]=0.0;
          }

          //----------------------------------- scale
          if (scale && splineScale)
          {
            float v=0.0f;
            for (;e<splineScale->percents.GetSize()-1;e++)
            {
              v=splineScale->percents.Get(e);
              if ((v+t)>currentstep) break;
              t+=v;
            }
            e--;
            fragment=currentstep-t;
            float width=splineScale->percents.Get(e+1);
            fragment/=width;
            GRPPOINT p=GRPPOINT::Interpolate(fragment,&(splineScale->points.Get(e)->value),&(splineScale->points.Get(e+1)->value));

            scale->vector[0]=splineScale->points.Get(e)->value.y;
            scale->vector[1]=splineScale->points.Get(e)->value.y;
            scale->vector[2]=splineScale->points.Get(e)->value.y;
          }
}

/*-------------------------------------------------------------------
//  GRPPATH::SetPositionSpline
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      03/12/2014 10:36:59
//
//  @param        splinePosition :
*/
/*-----------------------------------------------------------------*/
void GRPPATH::SetPositionSpline   (GRPSPLINE* splinePosition)
{
  if (splinePosition)
  {
  this->splinePosition    =splinePosition;
  splinePosition->CalculateSegments();
  }
}

/*-------------------------------------------------------------------
//  GRPPATH::SetRotationSpline
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      03/12/2014 10:37:05
//
//  @param        splineRotation :
//  @param        axis :
*/
/*-----------------------------------------------------------------*/
void GRPPATH::SetRotationSpline   (GRPSPLINE* splineRotation,int axis)
{
  splineRotation->CalculateSegments();

  if(splineRotation)
    {
      switch(axis)
        {
          case GRPPOINT_AXIS_X: this->splineRotationX = splineRotation; break;
          case GRPPOINT_AXIS_Y: this->splineRotationY = splineRotation; break;
          case GRPPOINT_AXIS_Z: this->splineRotationZ = splineRotation; break;
        }
    }
}



/*-------------------------------------------------------------------
//  GRPPATH::SetTravelSpline
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      03/12/2014 10:37:11
//
//  @param        spline :
*/
/*-----------------------------------------------------------------*/
void GRPPATH::SetTravelSpline   (GRPSPLINE* spline)
{
  this->splineTravel    =spline;
  splineTravel->CalculateSegments();
}

/*-------------------------------------------------------------------
//  GRPPATH::SetScaleSpline
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      03/12/2014 10:37:15
//
//  @param        spline :
*/
/*-----------------------------------------------------------------*/
void GRPPATH::SetScaleSpline    (GRPSPLINE* spline)
{
  this->splineScale   =spline;
  splineScale->CalculateSegments();
}

/*-------------------------------------------------------------------
//  GRPPATH::SetLoop
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      03/12/2014 10:37:19
//
//  @param        b :
*/
/*-----------------------------------------------------------------*/
void GRPPATH::SetLoop         (float st, float end,GRPPATH_LOOP_TYPE b)
{
  active=true;
  this->loopType=b;

    if (end<st)
    {
      this->loopStart=end;
      this->loopEnd=st;
      this->loopDirection=false;
    }
    else
    {
      this->loopStart=st;
      this->loopEnd=end;
      this->loopDirection=true;
    }

    if (st<0.0f)
      this->loopStart=0.0f;
    if (end>1.0f)
      this->loopEnd=1.0f;

  this->loopEnd-=this->step;
}
