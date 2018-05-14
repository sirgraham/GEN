
/*------------------------------------------------------------------------------------------
//  XINTERPOLATOR.CPP
//
//  Manages Interpolator between values
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 08/11/2016 12:00:40
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "xinterpolator.h"
#include "XDebug.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  XINTERPOLATOR::XINTERPOLATOR
*/
/**
//
//  Class Constructor XINTERPOLATOR
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/04/2016 10:34:26
//
//  @param        type :
*/
/*-----------------------------------------------------------------*/
XINTERPOLATOR::XINTERPOLATOR(XINTERPOLATOR_TYPE type)
{
  Clean();
  this->type = type;
  carry =0.0f;
}

/*-------------------------------------------------------------------
//  XINTERPOLATOR::~XINTERPOLATOR
*/
/**
//
//   Class Destructor XINTERPOLATOR
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/04/2016 10:34:30
//
*/
/*-----------------------------------------------------------------*/
XINTERPOLATOR::~XINTERPOLATOR()
{
  Clean();
}


/*-------------------------------------------------------------------
//  XINTERPOLATOR::GetType
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/04/2016 10:34:44
//
//  @return       JACKPOTINTERPOLATORTYPE :
//
*/
/*-----------------------------------------------------------------*/
XINTERPOLATOR_TYPE  XINTERPOLATOR::GetType()
{
  return type;
}

/*-------------------------------------------------------------------
//  XINTERPOLATOR::SetType
*/
/**
//  Set interpolator type
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/04/2016 10:34:49
//
//  @param        type :
*/
/*-----------------------------------------------------------------*/
void  XINTERPOLATOR::SetType(XINTERPOLATOR_TYPE type)
{
  this->type = type;
}


/*-------------------------------------------------------------------
//  XINTERPOLATOR::GetMode
*/
/**
//  return mode
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/04/2016 10:34:53
//
//  @return       XINTERPOLATOR_LIMITMODE :
//
*/
/*-----------------------------------------------------------------*/
XINTERPOLATOR_LIMITMODE XINTERPOLATOR::GetMode()
{
  return mode;
}

/*-------------------------------------------------------------------
//  XINTERPOLATOR::SetMode
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/04/2016 10:35:05
//
//  @param        mode :
*/
/*-----------------------------------------------------------------*/
void  XINTERPOLATOR::SetMode(XINTERPOLATOR_LIMITMODE mode)
{
  this->mode = mode;
}

/*-------------------------------------------------------------------
//  XINTERPOLATOR::Get
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/04/2016 10:35:29
//
//  @param        current :
//  @param        target :
//  @param        speed :
*/
/*-----------------------------------------------------------------*/
void  XINTERPOLATOR::Get(XDWORD& current, XDWORD& target, float& speed)
{
  current = this->current;
  target  = this->target;
  speed   = this->speed;
}

/*-------------------------------------------------------------------
//  XINTERPOLATOR::Set
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/04/2016 10:35:35
//
//  @param        current :
//  @param        target :
//  @param        speed :
*/
/*-----------------------------------------------------------------*/
void  XINTERPOLATOR::Set(XDWORD current,  XDWORD target, float speed)
{
  this->current = current;
  this->target  = target;
  this->speed   = speed;
}

/*-------------------------------------------------------------------
//  XINTERPOLATOR::GetCurrent
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/04/2016 10:36:52
//
//  @return       int :
//
*/
/*-----------------------------------------------------------------*/
XDWORD    XINTERPOLATOR::GetCurrent()
{
  return current;
}


/*-------------------------------------------------------------------
//  XINTERPOLATOR::SetCurrent
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/04/2016 10:36:59
//
//  @param        current :
*/
/*-----------------------------------------------------------------*/
void  XINTERPOLATOR::SetCurrent(XDWORD current)
{
  this->current = current;

  if (this->limits & XINTERPOLATOR_LIMITS_MAXIMUN)
    if(this->current>(XDWORD)(GetMaximun()*this->GetMultiplicator()))
        this->current =(XDWORD)(GetMaximun()*this->GetMultiplicator());

  if (this->limits & XINTERPOLATOR_LIMITS_MINIMUN)
    if(this->current<(XDWORD)(GetMinimun()*this->GetMultiplicator()))
        this->current =(XDWORD)(GetMinimun()*this->GetMultiplicator());
}


/*-------------------------------------------------------------------
//  XINTERPOLATOR::GetTarget
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/04/2016 10:37:04
//
//  @return       float :
//
*/
/*-----------------------------------------------------------------*/
XDWORD  XINTERPOLATOR::GetTarget()
{
  return target;
}

/*-------------------------------------------------------------------
//  XINTERPOLATOR::GetSpeed
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/04/2016 10:37:09
//
//  @return       float :
//
*/
/*-----------------------------------------------------------------*/
float XINTERPOLATOR::GetSpeed()
{
  return speed;
}


/*-------------------------------------------------------------------
//  XINTERPOLATOR::SetSpeed
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/04/2016 10:37:16
//
//  @param        speed :
*/
/*-----------------------------------------------------------------*/
void  XINTERPOLATOR::SetSpeed(float speed)
{
  this->speed = speed;
}


/*-------------------------------------------------------------------
//  XINTERPOLATOR::GetMaximun
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/04/2016 10:37:21
//
//  @return       float :
//
*/
/*-----------------------------------------------------------------*/
XDWORD  XINTERPOLATOR::GetMaximun()
{
  return max;
}


/*-------------------------------------------------------------------
//  XINTERPOLATOR::SetMaximun
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/04/2016 10:37:26
//
//  @param        max :
*/
/*-----------------------------------------------------------------*/
void  XINTERPOLATOR::SetMaximun(XDWORD max)
{
  this->max = max;
}

/*-------------------------------------------------------------------
//  XINTERPOLATOR::GetMinimun
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/04/2016 10:37:38
//
//  @return       float :
//
*/
/*-----------------------------------------------------------------*/
XDWORD  XINTERPOLATOR::GetMinimun()
{
  return min;
}


/*-------------------------------------------------------------------
//  XINTERPOLATOR::SetMinimun
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/04/2016 10:37:49
//
//  @param        min :
*/
/*-----------------------------------------------------------------*/
void  XINTERPOLATOR::SetMinimun(XDWORD min)
{
  this->min = min;
}

/*-------------------------------------------------------------------
//  XINTERPOLATOR::GetMultiplicator
*/
/**
//
//  Get Multiplier
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      02/11/2016 10:19:44
//
//  @return       XDWORD :
//
*/
/*-----------------------------------------------------------------*/
XDWORD  XINTERPOLATOR::GetMultiplicator()
{
  return this->multiplier;
}

/*-------------------------------------------------------------------
//  JACKPOTINTERPOLATOR::SetMultiplicator
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      02/11/2016 10:20:23
//
//  @param        m :
*/
/*-----------------------------------------------------------------*/
void  XINTERPOLATOR::SetMultiplicator(XDWORD m)
{
  this->multiplier=m;
}

/*-------------------------------------------------------------------
//  JACKPOTINTERPOLATOR::SetFactorType
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/04/2016 10:38:23
//
//  @param        factortype :
*/
/*-----------------------------------------------------------------*/
void  XINTERPOLATOR::SetFactorType(XINTERPOLATOR_FACTOR factortype)
{
  this->factortype = factortype;
}

/*-------------------------------------------------------------------
//  XINTERPOLATOR::GetFactorType
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/04/2016 10:38:31
//
//  @return       JACKPOTINTERPOLATORFACTOR :
//
*/
/*-----------------------------------------------------------------*/
XINTERPOLATOR_FACTOR  XINTERPOLATOR::GetFactorType()
{
  return this->factortype;
}


/*-------------------------------------------------------------------
//  XINTERPOLATOR::SetLimitMode
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/04/2016 10:52:23
//
//  @param        l :
*/
/*-----------------------------------------------------------------*/
void  XINTERPOLATOR::SetLimitFlags(XINTERPOLATOR_LIMITS l)
{
  this->limits=l;
}

/*-------------------------------------------------------------------
//  XINTERPOLATOR::GetLimitMode
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/04/2016 10:52:29
//
//  @return       JACKPOTINTERPOLATORLIMITS :
//
*/
/*-----------------------------------------------------------------*/
XINTERPOLATOR_LIMITS  XINTERPOLATOR::GetLimitFlags()
{
  return this->limits;
}


/*-------------------------------------------------------------------
//  XINTERPOLATOR::CopyFrom
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/04/2016 10:40:22
//
//  @return       bool :
//
//  @param        interpolator :
*/
/*-----------------------------------------------------------------*/
bool  XINTERPOLATOR::CopyFrom (XINTERPOLATOR& interpolator)
{
  type          = interpolator.type;
  mode          = interpolator.mode;

  current       = interpolator.current;
  target        = interpolator.target;
  speed         = interpolator.speed;
  max           = interpolator.max;
  min           = interpolator.min;
  limits        = interpolator.limits;

  factortype    = interpolator.factortype;

  Update(0);
  return true;
}

/*-------------------------------------------------------------------
//  XINTERPOLATOR::CopyFrom
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/04/2016 10:40:16
//
//  @return       bool :
//
//  @param        interpolator :
*/
/*-----------------------------------------------------------------*/
bool  XINTERPOLATOR::CopyFrom  (XINTERPOLATOR* interpolator)
{
  if(!interpolator) return false;
  return CopyFrom((*interpolator));
}

/*-------------------------------------------------------------------
//  JACKPOTINTERPOLATOR::CopyTo
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/04/2016 10:40:11
//
//  @return       bool :
//
//  @param        interpolator :
*/
/*-----------------------------------------------------------------*/
bool  XINTERPOLATOR::CopyTo (XINTERPOLATOR& interpolator)
{
  interpolator.type     = type;
  interpolator.mode     = mode;

  interpolator.current  = current;
  interpolator.target   = target;
  interpolator.speed    = speed;
  interpolator.max      = max;
  interpolator.min      = min;
  interpolator.limits   = limits;

  factortype            = interpolator.factortype;

  Update(0);
  return true;
}

/*-------------------------------------------------------------------
//  JACKPOTINTERPOLATOR::CopyTo
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/04/2016 10:40:06
//
//  @return       bool :
//
//  @param        interpolator :
*/
/*-----------------------------------------------------------------*/
bool  XINTERPOLATOR::CopyTo (XINTERPOLATOR* interpolator)
{
  if(!interpolator)
    return false;

  return CopyTo((*interpolator));
}


/*-------------------------------------------------------------------
//  =
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/04/2016 10:40:01
//
//  @return       const :
//
//  @param        interpolator :
*/
/*-----------------------------------------------------------------*/
const XINTERPOLATOR XINTERPOLATOR::operator = (XINTERPOLATOR& interpolator)
{
  CopyFrom(interpolator);
  return (*this);
}

/*-------------------------------------------------------------------
//  XINTERPOLATOR::SetTarget
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/04/2016 10:30:52
//
//  @param        target :
*/
/*-----------------------------------------------------------------*/
void XINTERPOLATOR::SetTarget(XDWORD target)
{
  //XDEBUG_PRINTCOLOR(3,__L("XINTERPOLATOR::SetTarget(%d)"),target);

    if (this->limits & XINTERPOLATOR_LIMITS_MAXIMUN)
    if (target>this->max)
    {
      XDEBUG_PRINTCOLOR(4,__L("Assigned over maximun: %d/%d"),target,max);
      this->target=this->max;
      return;
    }

    if (this->limits & XINTERPOLATOR_LIMITS_MINIMUN)
    if (target<this->min)
    {
      XDEBUG_PRINTCOLOR(4,__L("Assigned under minimun: %d/%d"),target,max);
      this->target=this->min;
      return;
    }

    this->target = target;
}


/*-------------------------------------------------------------------
//  XINTERPOLATOR::Update
*/
/**
//
//  applies limits and interpolation
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      02/02/2016 11:34:57
//
//  @return       bool :
//
//  @param        delta :
*/
/*-----------------------------------------------------------------*/
bool XINTERPOLATOR::Update(XQWORD delta)
{

  switch(type)
    {
      case XINTERPOLATOR_TYPE_LIMIT   :
                                              Interpolate(delta);

                                                if(mode == XINTERPOLATOR_LIMITMODE_INCREMENTAL)
                                                {
                                                  if(target<=current) current=target;
                                                }
                                               else
                                                {
                                                  if(this->mode==XINTERPOLATOR_LIMITMODE_DECREMENTAL)
                                                  if(target>=current) current = target;
                                                }
                                              break;

      case XINTERPOLATOR_TYPE_RELATIVE  :
      case XINTERPOLATOR_TYPE_ABSOLUTE  : if(current > target)        current = target;
                                          if(current < 0)             current = 0;
                                          break;

      case XINTERPOLATOR_TYPE_NONE      : break;
    }

  return true;
}




/*-------------------------------------------------------------------
//  XINTERPOLATOR::Interpolate
*/
/**
//
//  actual interpolating
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      02/02/2016 11:35:10
//
//  @return       bool :
//
//  @param        delta :
*/
/*-----------------------------------------------------------------*/
bool XINTERPOLATOR::Interpolate(XQWORD delta)
{
  switch(factortype)
  {
  case XINTERPOLATOR_FACTOR_RELATIVE: return InterpolateRelative(delta);
  case XINTERPOLATOR_FACTOR_LINEAR:   return InterpolateLinear  (delta);
  case XINTERPOLATOR_FACTOR_NONE:
  default: break;
  }

  return true;
}

/*-------------------------------------------------------------------
//  XINTERPOLATOR::InterpolateLinear
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      11/11/2016 13:08:15
//
//  @return       bool :
//
//  @param        delta :
*/
/*-----------------------------------------------------------------*/
bool XINTERPOLATOR::InterpolateLinear(XQWORD delta)
{
  float   fdelta    = (float)(delta/100.0f);
  int     itarget   = (int)(target);
  float   result    = (float)current;
  int     margin    = 0;

  if(result ==  target)
    return false;

  if(result < itarget)
            {
              if (delta!=0)
              {
                if (carry<0.0000001f)
                  carry=0.0f;

              float inc=(speed*fdelta)+carry;
                  carry=inc-float((int)inc);

              result += (int)inc;
              }

              if(result >= itarget)
              {
                result  = (float)itarget;
                carry   = 0.0f;
              }

            }
           else
            {
              if (delta!=0)
              {
              if (carry<0.0000001f)
                  carry=0.0f;

              float inc=(speed*fdelta)+carry;
                  carry=inc-float((int)inc);

              result -= (int)inc;
              }


              if(result <= float(itarget))
              {
                result = (float)itarget;
                carry  = 0.0f;
              }
            }

  current = (XDWORD)result;

  return true;
}


/*-------------------------------------------------------------------
//  XINTERPOLATOR::InterpolateRelative
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      11/11/2016 13:09:58
//
//  @return       bool :
//
//  @param        delta :
*/
/*-----------------------------------------------------------------*/
bool XINTERPOLATOR::InterpolateRelative(XQWORD delta)
{
  float   fdelta    = (float) (delta/100.0f);
  int     itarget   = (int)   (target);
  float   result    = (float) (current);
  int     margin    = 0;

  if(result ==  target)
    return false;

      if(result < itarget)
      {
        if (delta!=0)
        {
        if (carry<0.0000001f)
                carry=0.0f;

        float dif=(float)(itarget - result);
        float inc=((speed/100.0f)*fdelta*dif)+carry;
        carry =inc - float((int)inc);
        result += (int)inc;
        }

        if(result > (itarget-margin))
        {
            result  = (float)itarget;
            carry   = 0.0f;
        }
      }
     else
      {
        if (delta!=0)
        {
        if (carry<0.0000001f)
                carry=0.0f;

        float dif=(float)(result-itarget);
        float inc=((speed/100.0f)*fdelta*dif)+carry;
        carry =inc - float((int)inc);
        result -= (int)inc;
        }

        if(result < float(itarget+margin))
        {
            result = (float)itarget;
            carry  = 0.0f;
        }
      }



  current = (XDWORD)result;
  return true;
}

/*-------------------------------------------------------------------
//  XINTERPOLATOR::DeSerialize
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      04/02/2016 11:04:26
//
//  @return       bool :
//
//  @param        buffer :
*/
/*-----------------------------------------------------------------*/
bool XINTERPOLATOR::DeSerialize(XBUFFER* xbuffer) const
{
  /*
  xbuffer->Delete();

  xbuffer->Add((XDWORD)type);
  xbuffer->Add((XDWORD)mode);
  xbuffer->Add((XDWORD)current);
  xbuffer->Add((float)target);
  xbuffer->Add((float)speed);
  xbuffer->Add((float)max);
  xbuffer->Add((float)min);
  xbuffer->Add((XDWORD)factortype);
  */


  xbuffer->Resize(9*sizeof(XDWORD) + 1*sizeof(float));

  xbuffer->SetPosition(0);
  xbuffer->Set((XDWORD)mask);
  xbuffer->Set((XDWORD)type);
  xbuffer->Set((XDWORD)mode);
  xbuffer->Set((XDWORD)current);
  xbuffer->Set((XDWORD)target);
  xbuffer->Set((float)speed);
  xbuffer->Set((XDWORD)max);
  xbuffer->Set((XDWORD)min);
  xbuffer->Set((XDWORD)multiplier);
  xbuffer->Set((XDWORD)factortype);

  return true;
}




/*-------------------------------------------------------------------
//  XINTERPOLATOR::Serialize
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      04/02/2016 11:04:33
//
//  @return       bool :
//
//  @param        buffer :
*/
/*-----------------------------------------------------------------*/
bool XINTERPOLATOR::Serialize(XBUFFER* buffer)
{
  XDWORD buffer_type,    buffer_target, buffer_mode,   buffer_factortype,buffer_max,      buffer_min,buffer_current,buffer_multiplier;
  float  buffer_speed;

  buffer->SetPosition(0);
  buffer->Get((XDWORD&) mask);
  buffer->Get((XDWORD&) buffer_type);
  buffer->Get((XDWORD&) buffer_mode);
  buffer->Get((XDWORD&) buffer_current);
  buffer->Get((XDWORD&) buffer_target);
  buffer->Get((float&)  buffer_speed);
  buffer->Get((XDWORD&) buffer_max);
  buffer->Get((XDWORD&) buffer_min);
  buffer->Get((XDWORD&) buffer_multiplier);
  buffer->Get((XDWORD&) buffer_factortype);



  if (mask & XINTERPOLATOR_MASK_SPEED)
    this->speed       =buffer_speed;

  if (mask & XINTERPOLATOR_MASK_MAXIMUN)
    this->max         =buffer_max;

  if (mask & XINTERPOLATOR_MASK_MINIMUN)
    this->min         =buffer_min;

  if (mask & XINTERPOLATOR_MASK_MULTIPLIER)
    this->multiplier  =buffer_multiplier;

  if (mask & XINTERPOLATOR_MASK_TYPE)
    this->type        =(XINTERPOLATOR_TYPE) buffer_type;

  if (mask & XINTERPOLATOR_MASK_FACTOR)
    this->factortype  =(XINTERPOLATOR_FACTOR)buffer_factortype;

  if (mask & XINTERPOLATOR_MASK_TARGET) //  this->target      =buffer_target;
    this->SetTarget(buffer_target);

    if (mask & XINTERPOLATOR_MASK_CURRENT) // this->current     =buffer_current;
      this->SetCurrent(buffer_current);

  Update(0);

  return true;
}


/*-------------------------------------------------------------------
//  XINTERPOLATOR::SetMask
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      17/10/2016 15:57:40
//
//  @return       bool :
//
//  @param        mask :
*/
/*-----------------------------------------------------------------*/
bool XINTERPOLATOR::SetMask (XDWORD mask)
{
  this->mask = mask;

  return true;
}



/*-------------------------------------------------------------------
//  XINTERPOLATOR::GetMask
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      17/10/2016 15:57:47
//
//  @return       XDWORD :
//
*/
/*-----------------------------------------------------------------*/
XDWORD XINTERPOLATOR::GetMask()
{
  return mask;
}


/*-------------------------------------------------------------------
//  XINTERPOLATOR::GetCarry
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      22/11/2016 11:10:09
//
//  @return       float :
//
*/
/*-----------------------------------------------------------------*/
float XINTERPOLATOR::GetCarry()
{
  return this->carry;
}

/*-------------------------------------------------------------------
//  XINTERPOLATOR::SetCarry
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      22/11/2016 11:10:15
//
//  @param        carry :
*/
/*-----------------------------------------------------------------*/
void XINTERPOLATOR::SetCarry(float carry)
{
  this->carry=carry;
}