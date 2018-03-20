/*------------------------------------------------------------------------------------------
//	XLOADER.H
*/	
/**	
// \class 
//   
//  manages loads
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 05/09/2016 10:32:39
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _XLOADER_H_
#define _XLOADER_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "XSubject.h"
#include "XObserver.h"	

#include "GRPSubject.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
/*---- CLASS -----------------------------------------------------------------------------*/

class XTIMER;
class XLOADER;

class XLOADERTASKEVENT : public XEVENT
{
public:

			XLOADERTASKEVENT(XSUBJECT* subject, XDWORD type, float value ) : XEVENT(subject,type,XEVENTTYPE_LOADER), progress(value)
			{
			
			}					
			float progress;
};

//TO DO: DIEGO : this class should be a task tree recursively updating subtasks inside a task 

class XLOADERTASK : public XSUBJECT
{
public:
	friend class XLOADER;

											XLOADERTASK			(XCHAR* pname);
				XDWORD				GetSubjectType	() { return 0;}
virtual								~XLOADERTASK		();
				bool					IsFinished			();
				bool					Start						();						

static	void					ThreadFunction	(void* v);
virtual	void					Run							() {};				
virtual void					SetFile					(XSTRING* file);
virtual XCHAR*				GetFile					();

virtual void					SetTotal				(float total);
virtual float					GetTotal				();

virtual void					SetCurrent			(float current);
virtual float					GetCurrent			();

virtual void					SetPercent			(float current);
virtual float					GetPercent			();

virtual	void					Abort						();

virtual	bool					IsAborted				() { return aborted;}
				
				void					Output					(XDWORD priority,XCHAR* string,...);
virtual void					Print						(XDWORD priority,XCHAR*);

virtual void					Update					();

virtual	void					Add							(float n) { this->current+=n; Update(); }

XLOADER*	loader;

protected:
		
				bool			aborted;

				float			total;
				float			current;
				float			percent;

				XSTRING*  file;

				XSTRING		name;

				XTIMER*		xtimer;
				XTHREAD*	thread;
				XMUTEX*		lock;

				
				bool			isfinished;

				void Clean()
				{					
					xtimer			=NULL;
					thread			=NULL;
					lock				=NULL;

					total				=1.0f;
					current			=0.0f;
					percent			=0.0f;
					file				=NULL;
					isfinished	=false;
					aborted			=false;
					loader			=NULL;
				}


};

class XLOADER : public XSUBJECT, public XOBSERVER
{
public:

																	XLOADER						();
	virtual													~XLOADER					();
	
						void									SetName						(XCHAR* name);

						void									Start							();
						void									AddFile						(XCHAR*				path);
						void									AddTask						(XLOADERTASK* task);
						void									HandleEvent				(XEVENT*			ev);
						bool									IsFinished				();
						
						void									Update						();
						void									Abort							();

						float									GetCurrentPercent	();
	virtual		XLOADERTASK*					CreateTask				(XCHAR* pname);

	virtual		XVECTOR<XLOADERTASK*>* GetTasks					() { return &tasks;	} 
	
	virtual		void									Print							(XDWORD, XCHAR*)=0;
	virtual		void									Wait							();
	
protected:

						bool									abort;
						XSTRING								Name;

						float									Total;
						float									Current;
						float									Percent;

						int										nTasks;
						int										currentTask;

						XVECTOR<XSTRING*>			files;
						XVECTOR<XLOADERTASK*> tasks;


						
private:

						void			Clean()
						{		
							Name.Set(__L("unnamed"));
							Total				=0.0f;
							Current			=0.0f;
							Percent			=0.0f;
							nTasks			=0;
							currentTask	=0;
							abort				=false;
						}

};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

