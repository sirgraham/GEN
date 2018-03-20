/*------------------------------------------------------------------------------------------
//  DIOSTREAMI2C.H
*/  
/** 
// \class 
//   
//  Data IO Stream I2C Config class
//   
//  @author  Abraham J. Velez
//  @version 20/09/2012 14:31:53
*/  
/*  GEN  Copyright (C).  All right reserved.		
//----------------------------------------------------------------------------------------*/
  
#ifndef _DIOSTREAMI2CCONFIG_H_
#define _DIOSTREAMI2CCONFIG_H_
  
  
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XString.h"
#include "DIOStreamConfig.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

enum DIOSTREAMI2CCONFIG_ADDRESSSIZE
{
	DIOSTREAMI2CCONFIG_ADDRESSSIZE_DEFAULT		=	0	,
	DIOSTREAMI2CCONFIG_ADDRESSSIZE_7BITS					,
	DIOSTREAMI2CCONFIG_ADDRESSSIZE_10BITS					,
};


#define DIOSTREAMI2CCONFIG_ADDRESSINVALID			-1
  
/*---- CLASS -----------------------------------------------------------------------------*/
  
class XFACTORY;
class XPUBLISHER;


class DIOSTREAMI2CCONFIG : public DIOSTREAMCONFIG
{
	public:					
																			DIOSTREAMI2CCONFIG								();
		virtual													 ~DIOSTREAMI2CCONFIG								();				

		int																GetPort														()															 { return port;																}
		void															SetPort														(int port)									 		 { this->port = port;													}
		
		XSTRING*													GetLocalDeviceName								()                               { return &localdevicename;										}		
		bool															SetLocalDeviceNameByPort					();

		int																GetLocalAddress										()															 { return localaddress;												}
		void															SetLocalAddress										(int localaddress)							 { this->localaddress = localaddress;					}
	  
		int																GetRemoteAddress                  ()															 { return remoteaddress;											}
		void															SetRemoteAddress                  (int remoteaddress)							 { this->remoteaddress = remoteaddress;				}

		DIOSTREAMI2CCONFIG_ADDRESSSIZE		GetAddressSize										()															 { return addresssize;												}
		void															SetAddressSize										(DIOSTREAMI2CCONFIG_ADDRESSSIZE addresssize)							 
																			{ 
																				this->addresssize = addresssize;					
																			}
    
    bool                              IsStretchActive                    ()                              { return isstretchactive;                    }
    void                              SetIsStretchActive                 (bool isstretchactive)          { this->isstretchactive = isstretchactive;   }
	  		
	protected:
    
		int																port;
		XSTRING														localdevicename;		
		int																localaddress;
		int																remoteaddress;
		DIOSTREAMI2CCONFIG_ADDRESSSIZE		addresssize;
    bool                              isstretchactive;
		
	private:

		void													    Clean															()
                                      {  
																				type						= DIOSTREAMTYPE_I2C; 

                                        port						= 0;
																				localaddress    = DIOSTREAMI2CCONFIG_ADDRESSINVALID;
                                        remoteaddress		= DIOSTREAMI2CCONFIG_ADDRESSINVALID;
                                        addresssize     = DIOSTREAMI2CCONFIG_ADDRESSSIZE_DEFAULT; 
                                        
                                        isstretchactive = false;
                                      }
};



//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

