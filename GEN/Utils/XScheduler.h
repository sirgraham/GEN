/*------------------------------------------------------------------------------------------
//  XSCHEDULER.H
*/
/**
// \class
//
//  Scheduler for cyclic tasks
//
//  @author  Abraham J. Velez
//  @version 27/12/2012 8:41:25
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _XSCHEDULER_H_
#define _XSCHEDULER_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XDateTime.h"
#include "XVector.h"
#include "XEvent.h"
#include "XTimer.h"
#include "XSubject.h"
#include "XThreadCollected.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

enum XSCHEDULERXEVENTTYPE
{
  XSCHEDULERXEVENTTYPE_UNKNOWN          = XEVENTTYPE_SCHEDULER ,
};

#define XSCHEDULER_CYCLEFOREVER    -1

#define XSCHEDULER_DAYWEEK_NONE           0x00
#define XSCHEDULER_DAYWEEK_ISSUNDAY       0x01
#define XSCHEDULER_DAYWEEK_ISMONDAY       0x02
#define XSCHEDULER_DAYWEEK_ISTUESDAY      0x04
#define XSCHEDULER_DAYWEEK_ISWEDNESDAY    0x08
#define XSCHEDULER_DAYWEEK_ISTHURSDAY     0x10
#define XSCHEDULER_DAYWEEK_ISFRIDAY       0x20
#define XSCHEDULER_DAYWEEK_ISSATURDAY     0x40
#define XSCHEDULER_DAYWEEK_ISWORKDAY      (XSCHEDULER_DAYWEEK_ISMONDAY   | XSCHEDULER_DAYWEEK_ISTUESDAY |  XSCHEDULER_DAYWEEK_ISWEDNESDAY | XSCHEDULER_DAYWEEK_ISTHURSDAY | XSCHEDULER_DAYWEEK_ISFRIDAY)
#define XSCHEDULER_DAYWEEK_ISWEEKEND      (XSCHEDULER_DAYWEEK_ISSATURDAY | XSCHEDULER_DAYWEEK_ISSUNDAY)


/*---- CLASS -----------------------------------------------------------------------------*/

class XFACTORY;
class XPUBLISHER;
class XMUTEX;
class XTHREADCOLLECTED;
class XTIMER;
class XSCHEDULERTASK;
class XSCHEDULER;


class XSCHEDULERXEVENT : public XEVENT
{
  public:
                                XSCHEDULERXEVENT                (XSUBJECT* subject, XDWORD type = XSCHEDULERXEVENTTYPE_UNKNOWN, XDWORD family = XEVENTTYPE_SCHEDULER) : XEVENT(subject, type, family)
                                {
                                  Clean();
                                }

    virtual                    ~XSCHEDULERXEVENT                ()                                                               { Clean();                           }

    XSCHEDULER*                 GetScheduler                    ()                                                               { return xscheduler;                 }
    void                        SetScheduler                    (XSCHEDULER* xscheduler)                                         { this->xscheduler = xscheduler;     }

    XSCHEDULERTASK*             GetTask                         ()                                                               { return xtask;                      }
    void                        SetTask                         (XSCHEDULERTASK* xtask)                                          { this->xtask = xtask;               }

    XDATETIME*                  GetDateTime                     ()                                                               { return xdatetime;                  }
    void                        SetDateTime                     (XDATETIME* xdatetime)                                           { this->xdatetime = xdatetime;       }

  private:

    void                        Clean                           ()
                                {
                                  xscheduler  = NULL;
                                  xtask       = NULL;
                                  xdatetime   = NULL;
                                  xtimer      = NULL;
                                }

    XSCHEDULER*                 xscheduler;
    XSCHEDULERTASK*             xtask;
    XDATETIME*                  xdatetime;
    XTIMER*                     xtimer;
};




class XSCHEDULERTASK
{
  public:
                                XSCHEDULERTASK                  (XSCHEDULER* xscheduler);
    virtual                    ~XSCHEDULERTASK                  ();

    bool                        IsActive                        ()                                                                  { return active;                                                      }
    bool                        SetIsActive                     (bool active)
                                {
                                  this->active  = active;
                                  if(xtimer) xtimer->Reset();

                                  return true;
                                }


    XDWORD                      GetID                           ()                                                                  { return ID;                                                          }
    void                        SetID                           (XDWORD ID)                                                         { this->ID = ID;                                                      }

    XTIMER*                     GetXTimer                       ()                                                                  { return xtimer;                                                      }

    bool                        IsInValidTimeLimit              ()                                                                  { return isinvalidtimelimit;                                          }
    XDATETIME*                  GetTimeLimitStart               ()                                                                  { return &xdatetimelimitstart;                                        }
    XDATETIME*                  GetTimeLimitEnd                 ()                                                                  { return &xdatetimelimitend;                                          }

    bool                        IsCyclic                        ()                                                                  { return (!ncyclestodo)?false:true;                                   }
    int                         GetNCyclesTodo                  ()                                                                  { return ncyclestodo;                                                 }
    bool                        SetNCycles                      (int ncyclestodo = XSCHEDULER_CYCLEFOREVER, XDATETIME* xtimecadence = NULL);
    bool                        SetNCycles                      (int ncyclestodo, XQWORD cadenceinseconds);
    bool                        SetNCycles                      (int ncyclestodo, int cadenceinseconds)                             {  return SetNCycles(ncyclestodo, (XQWORD)cadenceinseconds);          }

    bool                        IsStartImmediatelyCycles        ()                                                                  { return isstartimmediatelycycles;                                    }
    void                        SetIsStartImmediatelyCycles     (bool isstartimmediatelycycles)                                     { this->isstartimmediatelycycles = isstartimmediatelycycles;          }

    bool                        SetTimeLimits                   (XDATETIME* xtimelimitstart = NULL, XDATETIME* xtimelimitend = NULL);

    void                        SetConditionDayWeek             (XBYTE mask)                                                        { conditiondayweekmask =  mask;                                       }

    virtual bool                CheckCondition                  (XDATETIME* xtimeactual, XTIMER* xtimeractual);


  private:

    void                        Clean                       ()
                                {
                                  xscheduler                = NULL;
                                  xtimer                    = NULL;

                                  ID                        = 0;

                                  active                    = false;

                                  isstartimmediatelycycles  = false;
                                  ncyclesmade               = 0;
                                  ncyclestodo               = 0;
                                  ncyclesactual             = 0;

                                  isinvalidtimelimit        = false;

                                  conditiondayweekmask      = XSCHEDULER_DAYWEEK_NONE;
                                }


    XBYTE                       GetDayOfWeekMask                (XDATETIME* xtimeactual);

    XSCHEDULER*                 xscheduler;

    XTIMER*                     xtimer;

    XDWORD                      ID;

    bool                        active;

    bool                        isstartimmediatelycycles;
    int                         ncyclesmade;
    int                         ncyclesactual;
    int                         ncyclestodo;

    XDATETIME                   xdatetimecadence;

    XDATETIME                   xdatetimelimitstart;
    XDATETIME                   xdatetimelimitend;

    bool                        isinvalidtimelimit;

    XBYTE                       conditiondayweekmask;
};





class XSCHEDULER  : public XSUBJECT
{
  public:
                                XSCHEDULER                      ();
    virtual                    ~XSCHEDULER                      ();

    bool                        Ini                             ();

    XWORD                       GetIDProgress                   ();

    bool                        IsActive                        ()                                                                  { return isactive;                                                    }
    void                        Activate                        (bool on)                                                           { this->isactive = on;                                                }

    bool                        Task_Add                        (XSCHEDULERTASK* task);
    XSCHEDULERTASK*             Task_Get                        (int index);
    XSCHEDULERTASK*             Task_GetForID                   (XDWORD ID);
    bool                        Task_Del                        (int index);
    bool                        Task_DelForID                   (XDWORD ID);
    bool                        Task_DelAll                     ();

    bool                        End                             ();

  private:

    void                        Clean                           ()
                                {
                                  xmutexscheduler     = NULL;
                                  xthreadscheduler    = NULL;
                                  xtimerwait          = NULL;

                                  isactive            = false;

                                  xdatetimeactual     = NULL;
                                  indextask           = 0;
                                }

    static void                 ThreadScheduler                 (void* data);

    XMUTEX*                     xmutexscheduler;
    XTHREADCOLLECTED*           xthreadscheduler;
    XTIMER*                     xtimerwait;

    bool                        isactive;

    XDATETIME*                  xdatetimeactual;
    XVECTOR<XSCHEDULERTASK*>    tasks;
    int                         indextask;
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

