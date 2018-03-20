/*------------------------------------------------------------------------------------------
//	INPPATH.CPP
//	
//	INPUT Path class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 27/03/2012 17:19:23
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.		 	 
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "INPPath.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//  INPPATHPOINT::INPPATHPOINT
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			27/03/2012 17:20:15
//	
//	@return 			
//	*/
/*-----------------------------------------------------------------*/
INPPATHPOINT::INPPATHPOINT()
{
	Clean();
}



/*-------------------------------------------------------------------
//  INPPATHPOINT::~INPPATHPOINT
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			27/03/2012 17:21:00
//	
//	@return 			
//	*/
/*-----------------------------------------------------------------*/
INPPATHPOINT::~INPPATHPOINT()
{
	Clean();	
}



/*-------------------------------------------------------------------
//  INPPATHPOINT::GetX
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			27/03/2012 17:21:54
//	
//	@return 			float : 
//	*/
/*-----------------------------------------------------------------*/
float INPPATHPOINT::GetX()
{
	return x;
}
		


/*-------------------------------------------------------------------
//  INPPATHPOINT::GetY
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			27/03/2012 17:22:18
//	
//	@return 			float : 
//	*/
/*-----------------------------------------------------------------*/
float INPPATHPOINT::GetY()
{
	return y;
}



/*-------------------------------------------------------------------
//  INPPATHPOINT::GetZ
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			27/03/2012 17:22:39
//	
//	@return 			float : 
//	*/
/*-----------------------------------------------------------------*/
float INPPATHPOINT::GetZ()
{
	return z;
}



/*-------------------------------------------------------------------
//  INPPATHPOINT::Set
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			27/03/2012 17:23:48
//	
//	@return 			bool : 
//	@param				x : 
//  @param				y : 
//  @param				z : 
*/
/*-----------------------------------------------------------------*/
void INPPATHPOINT::Set(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;

}



/*-------------------------------------------------------------------
//  INPPATHPOINT::SetX
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			27/03/2012 17:24:35
//	
//	@return 			bool : 
//	@param				x : 
*/
/*-----------------------------------------------------------------*/
void INPPATHPOINT::SetX(float x)
{
	this->x = x;
}




/*-------------------------------------------------------------------
//  INPPATHPOINT::SetY
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			27/03/2012 17:26:58
//	
//	@return 			bool : 
//	@param				y : 
*/
/*-----------------------------------------------------------------*/
void INPPATHPOINT::SetY(float y)
{
	this->y = y;
}



/*-------------------------------------------------------------------
//  INPPATHPOINT::SetZ
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			27/03/2012 17:27:02
//	
//	@return 			bool : 
//	@param				z : 
*/
/*-----------------------------------------------------------------*/
void INPPATHPOINT::SetZ(float z)
{
	this->z = z;
}




/*-------------------------------------------------------------------
//  INPPATHPOINT::Clean
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			27/03/2012 17:27:07
//	
//	@return 			void : 
//	*/
/*-----------------------------------------------------------------*/
void INPPATHPOINT::Clean()
{
	x = 0;
	y = 0;
	z = 0;
}




/*-------------------------------------------------------------------
//  INPPATH::INPPATH
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			27/03/2012 17:32:23
//	
//	@return 			
//	*/
/*-----------------------------------------------------------------*/
INPPATH::INPPATH()
{
	Clean();
}



/*-------------------------------------------------------------------
//  INPPATH::~INPPATH
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			27/03/2012 17:32:36
//	
//	@return 			
//	*/
/*-----------------------------------------------------------------*/
INPPATH::~INPPATH()
{
	DeleteAllPoints();

	Clean();
}



/*-------------------------------------------------------------------
//  INPPATH::Get
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			27/03/2012 17:53:14
//	
//	@return 			XVECTOR* : 
//	*/
/*-----------------------------------------------------------------*/
XVECTOR<INPPATHPOINT*>* INPPATH::Get()
{
	return &points;	
}



/*-------------------------------------------------------------------
//  INPPATH::GetNPoints
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			06/04/2012 23:57:25
//	
//	@return 			int : 
//	*/
/*-----------------------------------------------------------------*/
int INPPATH::GetNPoints()
{
	return points.GetSize();
}




/*-------------------------------------------------------------------
//  INPPATH::GetFirstPoint
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			27/03/2012 18:00:23
//	
//	@return 			INPPATHPOINT* : 
//	*/
/*-----------------------------------------------------------------*/
INPPATHPOINT* INPPATH::GetFirstPoint()
{
	if(!points.GetSize()) return NULL;

	INPPATHPOINT* point = (INPPATHPOINT*)points.Get(0);

	return point;
}



/*-------------------------------------------------------------------
//  INPPATH::GetLastPoint
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			27/03/2012 18:00:39
//	
//	@return 			INPPATHPOINT* : 
//	*/
/*-----------------------------------------------------------------*/
INPPATHPOINT* INPPATH::GetLastPoint()
{
	if(points.GetSize()<1) return NULL;

	INPPATHPOINT* point = (INPPATHPOINT*)points.Get((int)points.GetSize()-1);

	return point;
}




/*-------------------------------------------------------------------
//  INPPATH::AddPoint
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			27/03/2012 17:50:02
//	
//	@return 			bool : 
//	@param				x : 
//  @param				y : 
//  @param				z : 
//  @param				notduplicate : 
*/
/*-----------------------------------------------------------------*/
bool INPPATH::AddPoint(float x, float y, float z,bool notduplicate)
{	
	/*
	if(notduplicate)
		{
			for(int c=0;c<(int)points.GetSize();c++)
				{
					INPPATHPOINT* point = (INPPATHPOINT*)points.Get(c);
					if(point) 
						{
							if((point->GetX()==x)&&(point->GetY()==y)&&(point->GetZ()==z)) return false;
						}					
				}
		}

		*/

	INPPATHPOINT* point = new INPPATHPOINT();
	if(!point) return false;

	point->Set(x,y,z);
	
	points.Add(point);

	return true;
}




/*-------------------------------------------------------------------
//  INPPATH::DeleteAllPoints
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			27/03/2012 17:42:41
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool INPPATH::DeleteAllPoints()
{
	if(points.IsEmpty())  return false;

	points.DeleteContents();
	points.DeleteAll();

	return true;
}




/*-------------------------------------------------------------------
//  INPPATH::AddFromLine
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			27/03/2012 23:19:31
//	
//	@return 			bool : 
//	@param				reducedmode : 
//  @param				reduceddata : 
//  @param				x1 : 
//  @param				y1 : 
//  @param				x2 : 
//  @param				y2 : 
*/
/*-----------------------------------------------------------------*/
bool INPPATH::AddFromLine(INPPATHREDUCEDMODE reducedmode,int reducedfactor,int x1, int y1, int x2, int y2)
{
	int x, xinc1, xinc2;
	int y, yinc1, yinc2;
	int i, deltax, deltay, npixels;
	int d, dinc1, dinc2;
		
  // Calculate deltax and deltay for initialisation
  deltax = abs(x2 - x1);
  deltay = abs(y2 - y1);

  // Initialize all vars based on which is the independent variable 
  if(deltax>=deltay)
		{ 
      // x is independent variable }
      npixels		= deltax + 1;
      d					= (2 * deltay) - deltax;
      dinc1			= deltay >> 1;
      dinc2			= (deltay - deltax) >> 1;
      xinc1			= 1;
      xinc2			= 1;
      yinc1			= 0;
      yinc2			= 1;
		}
   else
		{
      // y is independent variable }
      npixels		= deltay + 1;
      d					= (2 * deltax) - deltay;
      dinc1			= deltax >> 1;
      dinc2			= (deltax - deltay) >> 1;
      xinc1			= 0;
      xinc2			= 1;
      yinc1			= 1;
      yinc2			= 1;
		}

  // Make sure x and y move in the right directions 
  if(x1>x2)
		{
      xinc1 = -xinc1;
      xinc2 = -xinc2;
		}

  if(y1>y2)
		{
      yinc1 = -yinc1;
      yinc2 = -yinc2;
		}

  // Start drawing at  
  x = x1;
  y = y1;

  
	int  steps		= CalculateReducedModeSteps(reducedmode,reducedfactor,npixels);
	int  incstep  = steps;
	bool addpoint = false;

  // Draw the pixels }
  for(i=0;i<npixels;i++)
		{ 		
			if(incstep == steps)				
				{
					addpoint = true;
					incstep  = 0;
				}

			if(addpoint) 
						AddPoint((float)x,(float)y,0,true);				
			 else incstep++;

			addpoint = false;
			
      if(d<0)
				{
          d = d + dinc1;
          x = x + xinc1;
          y = y + yinc1;
				}
       else
				{
          d = d + dinc2;
          x = x + xinc2;
          y = y + yinc2;
				}
		}

	return false;
}



/*-------------------------------------------------------------------
//  INPPATH::AddFromCircle
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			28/03/2012 0:23:53
//	
//	@return 			bool : 
//	@param				reducedmode : 
//  @param				reducedfactor : 
//  @param				x : 
//  @param				y : 
//  @param				radio : 
*/
/*-----------------------------------------------------------------*/
bool INPPATH::AddFromCircle(INPPATHREDUCEDMODE reducedmode,int reducedfactor,int xc, int yc, int radio)
{
  float deg			= 0;
	int   npixels = 0;
	
  do{ deg+=0.005f;
	    
			npixels++;

     } while (deg <= 6.4);

	deg = 0;

	int  steps		= CalculateReducedModeSteps(reducedmode,reducedfactor,npixels);
	int  incstep  = steps;
	bool addpoint = false;

  do { int x = Round(radio * cos(deg));
       int y = Round(radio * sin(deg));

			 if(incstep == steps)				
				 {
					 addpoint = true;
					 incstep  = 0;
				 }

			 if(addpoint) 
					 		AddPoint((float)xc+x ,(float)yc+y , true);			
				 else incstep++;

			 addpoint = false;			 
         
       deg += 0.005f;

     } while (deg <= 6.4);

	return true;
}



/*-------------------------------------------------------------------
//  INPPATH::AddFromElipse
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			28/03/2012 10:39:49
//	
//	@return 			bool : 
//	@param				reducedmode : 
//  @param				reducedfactor : 
//  @param				x : 
//  @param				y : 
//  @param				rx : 
//  @param				ry : 
*/
/*-----------------------------------------------------------------*/
bool INPPATH::AddFromElipse(INPPATHREDUCEDMODE reducedmode,int reducedfactor, int x, int y, int rx, int ry)
{		
	int ix, iy;
  int h, i, j, k;
  int oh, oi, oj, ok;

  if(rx < 1)  rx = 1; 
  if(ry < 1)  ry = 1;

  h = i = j = k = 0xFFFF;

  if(rx > ry) 
		{
      ix = 0; 
      iy = rx * 64;

      do{ 	oh = h;
						oi = i;
						oj = j;
						ok = k;

						h = (ix + 32) >> 6; 
						i = (iy + 32) >> 6;
						j = (h * ry) / rx; 
						k = (i * ry) / rx;

					if(((h != oh) || (k != ok)) && (h < oi))
						{							
							
							AddPoint((float)x+h, (float)y+k,true);
							
							if(h) AddPoint((float)x-h, (float)y+k,true);
							
							if(k) 
								{
									AddPoint((float)x+h, (float)y-k,true);
								
									if(h) AddPoint((float)x-h, (float)y-k,true);								
								}												
						
						}

					if((i!=oi)||(j!=oj) &&(h < i)) 
						{							
							
							AddPoint((float)x+i, (float)y+j,true);
							
							if(i) AddPoint((float)x-i, (float)y+j,true);

							if(j)
								{
									AddPoint((float)x+i, (float)y-j,true);
									
									if(i) AddPoint((float)x-i, (float)y-j,true);									
								}											
							
						}

					ix = ix + iy / rx; 
					iy = iy - ix / rx;

				} while (i > h);
				
		} 
   else 
		{
      ix = 0; 
      iy = ry * 64;

      do{		oh = h;
						oi = i;
						oj = j;
						ok = k;

						h = (ix + 32) >> 6; 
						i = (iy + 32) >> 6;
						j = (h * rx) / ry; 
						k = (i * rx) / ry;

						if(((j != oj) || (i != oi)) && (h < i)) 
							{		
								
								AddPoint((float)x+j, (float)y+i,true);
								
								if(j) AddPoint((float)x-j, (float)y+i,true);

								if(i) 
									{
										AddPoint((float)x+j, (float)y-i,true);
										
										if(j) AddPoint((float)x-j, (float)y-i,true);
										
									}																					
							}

					 if(((k != ok) || (h != oh)) && (h < oi)) 
						 {		
									
							 AddPoint((float)x+k, (float)y+h,true);
							 
								if(k) AddPoint((float)x-k, (float)y+h,true);
								
								if(h) 
									{
										AddPoint((float)x+k, (float)y-h,true);

										if(k) AddPoint((float)x-k, (float)y-h,true);										
									}												
									
						 }

					 ix = ix + iy / ry; 
					 iy = iy - ix / ry;

				} while(i > h);
		}

	return 0;
}




/*-------------------------------------------------------------------
//  INPPATH::GetAngle
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			26/12/2011 17:45:11
//	
//	@return				float : 
//	*/
/*-----------------------------------------------------------------*/
float INPPATH::GetAngle()
{
	if(points.GetSize()<2) return 0;

	INPPATHPOINT* point[2];
		
	point[0] = GetFirstPoint();
	point[1] = GetLastPoint();

	double radians = atan2(point[1]->GetY()-point[0]->GetY() , point[1]->GetX()-point[0]->GetX()); 

	float angle = (float)((180*radians)/PI);

	if(angle<0) angle = 180 + (180+angle);

	return angle;
}




/*-------------------------------------------------------------------
//  INPCURSORMOTION::GetModule
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			26/12/2011 18:51:32
//	
//	@return				float : 
//	*/
/*-----------------------------------------------------------------*/
int INPPATH::GetModule()
{	
	if(points.GetSize()<2) return 0;

	INPPATHPOINT* point[2];
		
	point[0] = GetFirstPoint();
	point[1] = GetLastPoint();

	float difx = point[1]->GetX() - point[0]->GetX();
	float dify = point[1]->GetY() - point[0]->GetY();

	int module = (int) (((int)difx^2) + ((int)dify^2)) ^ (1/2);

	return module;
}



/*-------------------------------------------------------------------
//  INPPATH::IsPathInRect
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			26/12/2011 21:17:21
//	
//	@return				bool : 
//	@param				x : 
//  @param				y : 
//  @param				width : 
//  @param				height : 
*/
/*-----------------------------------------------------------------*/
bool INPPATH::IsInRect(int x, int y, int width,int height)
{
	if(points.IsEmpty()) return false;
	
	int npoints = 0;

	for(int c=0;c<(int)points.GetSize();c++)
		{
			INPPATHPOINT* point = (INPPATHPOINT*)points.Get(c);
			if(point)
				{
					if((point->GetX() >= x) && 	(point->GetX() <= (x+width)) && 
						 (point->GetY() >= y) &&	(point->GetY() <= (y+height))) npoints++;						
				}
		}

	if(npoints==points.GetSize()) return true;

	return false;
}



/*-------------------------------------------------------------------
//  INPPATH::Clean
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			27/03/2012 17:33:16
//	
//	@return 			void : 
//	*/
/*-----------------------------------------------------------------*/
void INPPATH::Clean()
{

}

		

/*-------------------------------------------------------------------
//  INPPATH::CalculateReducedModeSteps
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			28/03/2012 0:05:19
//	
//	@return 			int : 
//	@param				mode : 
//  @param				factor : 
//  @param				npixels : 
*/
/*-----------------------------------------------------------------*/
int INPPATH::CalculateReducedModeSteps(INPPATHREDUCEDMODE mode,int factor,int npixels)
{
	int steps = 0;

	switch(mode)
		{
			case INPPATHREDUCEDMODE_NONE						:																	break;
			case INPPATHREDUCEDMODE_BYSTEPS					: steps = factor;									break;
			case INPPATHREDUCEDMODE_NMAX						:	steps =	(npixels/factor);				break;
			case INPPATHREDUCEDMODE_NMAXPERCENT			: steps =	(npixels*factor)/100;		break;
		}

	return steps;
}


/*-------------------------------------------------------------------
//  INPPATH::Round
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			28/03/2012 1:37:00
//	
//	@return 			int : 
//	@param				a : 
*/
/*-----------------------------------------------------------------*/
int INPPATH::Round(double a) 
{
	return int(a + 0.5);
}