/*------------------------------------------------------------------------------------------
//	TESTBASEINPUT.H
*/	
/**	
// \class 
//   
//  Test Input 
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 30/05/2016 16:16:37
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _TESTBASEINPUT_H_
#define _TESTBASEINPUT_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "TestBase.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/

class HOTSPOT
{
public:
	XDWORD	id;
	GRPPOINT start;
	GRPPOINT end;

	HOTSPOT(XDWORD id, float xo, float yo, float xf, float yf)
	{
		this->id	= id;
		start.x		= xo;
		start.y		= yo;
		end.x			= xf;
		end.y			= yf;
	}
	bool IsInside(float x, float y)
	{
		if (x >= start.x && x <= end.x)
			if (y >= start.y && y <= end.y)
			{
				return true;
			}
		return false;
	}
};

class GESTURE
{
public:
	XVECTOR<int>							 	  queue;
	XMAP<XDWORD, XVECTOR<int>* >  sequences;
	XMAP<XDWORD, XVECTOR<int>* >  combination;
	XVECTOR<HOTSPOT*> 						hotspots;

	bool AddSequence(XDWORD id, XDWORD n, ...)
	{
		XSTRING string;		
		va_list arg;

		va_start(arg, n);
		
		this->sequences.Add(id, new XVECTOR<int>());

		for (XDWORD i = 0; i < n; i++)
		{
			int hot_spot=va_arg(arg, int);
			this->sequences.Get(id)->Add(hot_spot);
		}				
		va_end(arg);		

		return true;
	}

	bool	AddHotspot(XDWORD id, float xo, float yo, float xf, float yf)
	{
		return this->hotspots.Add(new HOTSPOT(id, xo, yo, xf, yf));
	}

	void Clear()
	{
		 queue.DeleteAll();
	}

	XDWORD  CheckSequence()
	{
		for (XDWORD e = 0; e < this->sequences.GetSize(); e++)
		{
			XDWORD id				= this->sequences.GetKey(e);
			XVECTOR<int>* v = this->sequences.GetElement(e);

			XDWORD c = 0;

			if (queue.GetSize()==v->GetSize())
			for (XDWORD i = 0; i < v->GetSize(); i++)
			{
				if (queue.Get(i) == v->Get(i))
				{
					c++;
				}
				else
					break;
			}
			if (c == v->GetSize())
			{
				this->Clear();
				return id;
			}
		}
		return 0;
	}

	XDWORD	Update(float x,float y)
	{
		for (XDWORD e = 0; e < hotspots.GetSize(); e++)
		{
			HOTSPOT* hp = hotspots.FastGet(e);
			if (hp->IsInside(x, y))
			{
				if (hp->id != queue.GetLast())
				{
					queue.Add(hp->id);

					XSTRING s;
					for (XDWORD u = 0; u < queue.GetSize(); u++)
						s.AddFormat(__L("%d"), queue.Get(u));
					XDEBUG_PRINTCOLOR(0, s.Get());
				}
			}
		}

		return 0;
	}


};


class TESTBASEINPUT : public TESTBASE								
{																								
	public:																				
																	TESTBASEINPUT							(GRPAPPLICATION* xapplication);
																 ~TESTBASEINPUT							();

		virtual bool									IniResources							();
		virtual bool									EndResources							();
		virtual bool									InputManagement						();
		virtual bool									Do												();     
		
		GESTURE gesture;

	private:

		void													Clean                     ()
																	{																		

																	}

		XPATH													xpath;				
		

};

#endif