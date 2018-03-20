/*------------------------------------------------------------------------------------------
//	INPPATH.H
*/	
/**	
// \class 
//   
//  INPUT Path class
//   
//	@author	 Abraham J. Velez
//	@version 27/03/2012 17:12:43
*/	
/*	GEN  Copyright (C).  All right reserved.			 
//----------------------------------------------------------------------------------------*/
	
#ifndef _INPPATH_H_
#define _INPPATH_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "XVector.h"

	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
enum INPPATHREDUCEDMODE
{
	INPPATHREDUCEDMODE_NONE						= 0	,
	INPPATHREDUCEDMODE_BYSTEPS						,
	INPPATHREDUCEDMODE_NMAX								,
	INPPATHREDUCEDMODE_NMAXPERCENT				,
};
	
/*---- CLASS -----------------------------------------------------------------------------*/


class INPPATHPOINT
{
	public:
																	INPPATHPOINT								();
virtual														~INPPATHPOINT								();

virtual float											GetX												();
virtual float											GetY												();
virtual float											GetZ												();

virtual void											Set													(float x = 0, float y = 0, float z = 0);
virtual void											SetX												(float x = 0);
virtual void											SetY												(float y = 0);
virtual void											SetZ												(float z = 0);

protected:

				float											x;
				float											y;
				float											z;

	private:

				void											Clean												();		 
};




class INPPATH
{
	public:
															INPPATH											();
		virtual									 ~INPPATH											();

		XVECTOR<INPPATHPOINT*>*		Get													();	

		int												GetNPoints									();		
				
		INPPATHPOINT*							GetFirstPoint								();		
		INPPATHPOINT*							GetLastPoint								();		

		bool											AddPoint										(float x=0, float y=0, float z=0,bool notduplicate=false);
		bool											DeleteAllPoints							();

		bool											AddFromLine									(INPPATHREDUCEDMODE reducedmode,int reducedfactor,int x1, int y1, int x2, int y2);
		bool											AddFromCircle								(INPPATHREDUCEDMODE reducedmode,int reducedfactor,int x, int y, int radio);
		bool											AddFromElipse								(INPPATHREDUCEDMODE reducedmode,int reducedfactor, int x, int y, int rx, int ry);

		float											GetAngle										();
		int												GetModule										();

		bool											IsInRect										(int x, int y, int width,int height);

	protected:
		
		XVECTOR<INPPATHPOINT*>		points;

	private:

		void											Clean												();	
		
		int												CalculateReducedModeSteps		(INPPATHREDUCEDMODE mode,int factor,int npixels);
		int												Round												(double a);

		
};



	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

