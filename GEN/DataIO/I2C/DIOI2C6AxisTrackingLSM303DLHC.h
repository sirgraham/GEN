/*------------------------------------------------------------------------------------------
//	DIOI2C6AXISTRACKINGLSM303DLHC.H
*/	
/**	
// \class 
//   
//  Data IO I2C PCA9685 (16-channel, 12-bit PWM controller) class
//   
//	@author	 Abraham J. Velez
//
//	Date Of Creation	: 03/05/2014 16:12:18
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _DIOI2C6AXISTRACKINGLSM303DLHC_H_
#define _DIOI2C6AXISTRACKINGLSM303DLHC_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include <stdio.h>
#include <math.h>

#include "XBase.h"

#include "DIODeviceI2C.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

enum REG_ADDRS 
{
	// ----- Compass -------------------------------

	DIOI2CLSM303DLHC_CRA_REG_M		= 0x00,
  DIOI2CLSM303DLHC_CRB_REG_M		= 0x01,
  DIOI2CLSM303DLHC_MR_REG_M			= 0x02,
  DIOI2CLSM303DLHC_OUT_X_M			= 0x03,
  DIOI2CLSM303DLHC_OUT_Y_M			= 0x05,
  DIOI2CLSM303DLHC_OUT_Z_M			= 0x07,

	// ----- Accelerometer -------------------------

  DIOI2CLSM303DLHC_CTRL_REG1_A	= 0x20,
  DIOI2CLSM303DLHC_CTRL_REG4_A	= 0x23,
  DIOI2CLSM303DLHC_OUT_X_A			= 0x28,
  DIOI2CLSM303DLHC_OUT_Y_A			= 0x2A,
  DIOI2CLSM303DLHC_OUT_Z_A			= 0x2C,
};


#define DIOI2CLSM303DLHC_FILTERSHIFT		6 


/*---- CLASS -----------------------------------------------------------------------------*/


class XFACTORY;
class XPUBLISHER;
class XTIMER;
class DIOFACTORY;
class DIOSTREAMI2CCONFIG;
class DIOSTREAMI2C;


class DIOI2CLSM303DLHCVECTOR
{
	public:
																				DIOI2CLSM303DLHCVECTOR                      ()		{ Clean();                  }
		virtual														 ~DIOI2CLSM303DLHCVECTOR                      ()		{ Clean();                  }		
  
						
		static void													Cross																				(const DIOI2CLSM303DLHCVECTOR& a, const DIOI2CLSM303DLHCVECTOR& b, DIOI2CLSM303DLHCVECTOR& out)
																				{
																				  out.x = a.y * b.z - a.z * b.y;
																					out.y = a.z * b.x - a.x * b.z;
																					out.z = a.x * b.y - a.y * b.x;
																				}
	
		static float												Dot																					(const DIOI2CLSM303DLHCVECTOR& a, const DIOI2CLSM303DLHCVECTOR& b)
																				{		
																					return a.x * b.x + a.y * b.y + a.z * b.z;
																				}

		static void													Normalize																		(DIOI2CLSM303DLHCVECTOR& a)
																				{
																					float mag = (float)sqrt(Dot(a,a));

																					a.x /= float(mag);
																					a.y /= float(mag);
																					a.z /= float(mag);
																				}

		float																x;
		float																y;
		float                               z;
	
	private:
			
		void																Clean																				()
																				{																																
																					x = 0;
																					y = 0;
																					z = 0;
																				}		



};





class DIOI2CLSM303DLHC : public DIODEVICEI2C
{
	public:

																				DIOI2CLSM303DLHC														();
		virtual														 ~DIOI2CLSM303DLHC														();
			
		bool																End																					();

	protected:
	
		virtual bool												IniDevice																		();

		void																Clean																				()
																				{																																
																					diostream	      = NULL;																																											
																				}		

		bool																ReadRegister																(XBYTE reg, XBYTE& value);
		bool																ReadRegister																(XBYTE reg, XWORD& value);
		bool																WriteRegister																(XBYTE reg, XBYTE  value);		
		
		DIOSTREAMI2C*												diostream;
};
	


class DIOI2C3DCOMPASSLSM303DLHC : public DIOI2CLSM303DLHC
{
	public:

																				DIOI2C3DCOMPASSLSM303DLHC										()    {														}
		virtual														 ~DIOI2C3DCOMPASSLSM303DLHC										()		{														}

		bool																Configure																		();
		
		bool																Read																				(XWORD& c_x, XWORD& c_y, XWORD& c_z);		
};



class DIOI2C3DACCELEROMETERLSM303DLHC : public DIOI2CLSM303DLHC
{
	public:

																				DIOI2C3DACCELEROMETERLSM303DLHC							()		{														}
		virtual														 ~DIOI2C3DACCELEROMETERLSM303DLHC							()		{														}

		bool																Configure																		();

		bool																Read																				(XWORD& a_x, XWORD& a_y, XWORD& a_z);
};




class DIOI2C6AXISTRACKINGLSM303DLHC 
{
	public:

																				DIOI2C6AXISTRACKINGLSM303DLHC			()     
																				{ 
																					Clean();                 

																					accelerometer = new DIOI2C3DACCELEROMETERLSM303DLHC(); 
																					compass				= new DIOI2C3DCOMPASSLSM303DLHC();
																				}


		virtual														 ~DIOI2C6AXISTRACKINGLSM303DLHC			()		 
																				{ 
																					delete accelerometer; 
																					delete compass;

																					Clean();
																				}	

		bool																Ini																					(XCHAR* localdevicename, int remotedeviceaddressaccelerometer, int remotedeviceaddresscompass, int timeout)
																				{
																					if(!accelerometer)	return false;
																					if(!compass)				return false;

																					if(!accelerometer->Ini(localdevicename, remotedeviceaddressaccelerometer, timeout)) return false;						
																					if(!compass->Ini(localdevicename, remotedeviceaddresscompass, timeout))             return false;
										
																					if(!accelerometer->Configure()) return false;											
																					if(!compass->Configure())				return false;  

																					return true;
																				}


		bool																Ini																					(int port,  int remotedeviceaddressaccelerometer, int remotedeviceaddresscompass, int timeout)
																				{	
																					if(!accelerometer)	return false;
																					if(!compass)				return false;


																					filter_az				= 6000;

																					if(!accelerometer->Ini(port, remotedeviceaddressaccelerometer, timeout)) return false;						
																					if(!compass->Ini(port, remotedeviceaddresscompass, timeout))             return false;
										
																					if(!accelerometer->Configure()) return false;											
																					if(!compass->Configure())				return false;  

																					return true;
																				}

		void																SetOffset																		(float x, float y, float z)
																				{
																					offset_x = x;
																					offset_y = y;
																					offset_z = z;   
																				}

		void																SetScale																		(float x, float y, float z)
																				{
																					scale_x = x;
																					scale_y = y;
																					scale_z = z;
																				}

		bool																Read																				(XWORD& a_x, XWORD& a_y, XWORD& a_z, XWORD& c_x, XWORD& c_y, XWORD& c_z)
																				{
																					if(!accelerometer)	return false;
																					if(!compass)				return false;
																				
																					bool status;

																					status = accelerometer->Read(a_x, a_y, a_z);
																					if(status) status = compass->Read(c_x, c_y, c_z);

																					return status;

																				}

		bool																Read																				(DIOI2CLSM303DLHCVECTOR& a, DIOI2CLSM303DLHCVECTOR& c)
																				{
																					if(!accelerometer)	return false;
																					if(!compass)				return false;

																					XWORD ax = 0;
																					XWORD ay = 0;
																					XWORD az = 0;
																					XWORD cx = 0;
																					XWORD cy = 0;
																					XWORD cz = 0;

																					if(!Read(ax, ay, az, cx, cy, cz)) return false;
																					
																					// Perform simple lowpass filtering Intended to stabilize heading despite device vibration such as on a UGV
																					filter_ax += ax - (filter_ax >> DIOI2CLSM303DLHC_FILTERSHIFT);
																					filter_ay += ay - (filter_ay >> DIOI2CLSM303DLHC_FILTERSHIFT);
																					filter_az += az - (filter_az >> DIOI2CLSM303DLHC_FILTERSHIFT);

																					a.x = (float)(filter_ax >> DIOI2CLSM303DLHC_FILTERSHIFT);
																					a.y = (float)(filter_ay >> DIOI2CLSM303DLHC_FILTERSHIFT);
																					a.z = (float)(filter_az >> DIOI2CLSM303DLHC_FILTERSHIFT);
    
																					// offset and scale
																					c.x = (cx + offset_x) * scale_x;
																					c.y = (cy + offset_y) * scale_y;
																					c.z = (cz + offset_z) * scale_z;
																				
																					return true;
																				}


		float																Heading																			()
																				{
																					DIOI2CLSM303DLHCVECTOR vector;

																					vector.x =  0;
																					vector.y = -1;
																					vector.z =  0;

																					return Heading(vector);
																				}

		float																Heading																			(DIOI2CLSM303DLHCVECTOR& from)
																				{
																					DIOI2CLSM303DLHCVECTOR a;
																					DIOI2CLSM303DLHCVECTOR c;

																					if(!Read(a, c)) return 0.0f;
   
																					DIOI2CLSM303DLHCVECTOR temp_a;
																					
																					temp_a.x = a.x;
																					temp_a.y = a.y;
																					temp_a.z = a.z;
																					
																					// normalize
																					DIOI2CLSM303DLHCVECTOR::Normalize(temp_a);
																					
																					// compute E and N
																					DIOI2CLSM303DLHCVECTOR e;
																					DIOI2CLSM303DLHCVECTOR n;

																					DIOI2CLSM303DLHCVECTOR::Cross(c ,temp_a, e);

																					DIOI2CLSM303DLHCVECTOR::Normalize(e);

																					DIOI2CLSM303DLHCVECTOR::Cross(temp_a, e , n);
    
																					// compute heading
																					float heading = (float)atan2( DIOI2CLSM303DLHCVECTOR::Dot(e , from), DIOI2CLSM303DLHCVECTOR::Dot(n , from)) * (180/PI);
																					if (heading < 0) heading += 360;
    
																					return float(heading);
																				}

		bool																End																					()
																				{
																					if(!accelerometer)	return false;
																					if(!compass)				return false;

																					accelerometer->End();
																					compass->End();

																					return true;
																				}			
	private:

		void																Clean																				()
																				{		
																					accelerometer		= NULL;
																					compass					= NULL;	

																					offset_x				= 0.0f;
																					offset_y				= 0.0f;
																					offset_z				= 0.0f;

																					scale_x					= 0.0f;
																					scale_y					= 0.0f;
																					scale_z					= 0.0f;
    
																					filter_ax				= 0;
																					filter_ay				= 0;
																					filter_az				= 0;
																				}		

		DIOI2C3DACCELEROMETERLSM303DLHC*		accelerometer;
		DIOI2C3DCOMPASSLSM303DLHC*					compass;						
		
		float																offset_x;
		float																offset_y;
    float																offset_z;

    float																scale_x;
    float																scale_y;
    float																scale_z;
    
		long																filter_ax;
    long																filter_ay;
    long																filter_az;

};
	



/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	

#endif

