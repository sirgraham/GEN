/*------------------------------------------------------------------------------------------
//	TESTMICRO.CPP
//	
//	 Organic I2C Module
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 13/11/2013 12:20:38
//	Last Mofificacion	:	
//	 
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>

#include "XApplication.h"
#include "XSleep.h"
#include "XRand.h"
#include "XDebug.h"

#include "DIOFactory.h"
#include "DIOGPIO.h"
#include "DIOStreamUARTConfig.h"
#include "DIOStreamUART.h"
#include "DIOStreamUSBConfig.h"
#include "DIOStreamUSB.h"
#include "DIOStreamI2CConfig.h"
#include "DIOStreamI2C.h"
#include "DIOStreamSPIConfig.h"
#include "DIOStreamSPI.h"
#include "DIOGPIO.h"

#include "DIOI2CLightSensorTSL2561.h"
#include "DIOI2C6AxisTrackingLSM303DLHC.h"
#include "DIOSPILCDScreenPCF8833.h"

 #if defined(STM32F407xx) || defined(STM32F446xx)
#include "XSTM32FxxxSRAM.h"
#endif 

#include "TestMicro.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	
TESTMICRO* testmicro = NULL;
	
//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  TESTMICRO::TESTMICRO
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			13/08/2002 13:15:15
//	
//	@return 			
//	*/
//-------------------------------------------------------------------
TESTMICRO::TESTMICRO() 
{		
	Clean();
}


//-------------------------------------------------------------------
//  TESTMICRO::~TESTMICRO
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			13/08/2002 13:15:40
//	
//	@return 			
//	*/
//-------------------------------------------------------------------
TESTMICRO::~TESTMICRO()
{
	Clean();
}


//-------------------------------------------------------------------
//  XAPPLICATION::Create
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			13/08/2002 11:02:38
//	
//	@return 			XAPPLICATION* : 
//	*/
//-------------------------------------------------------------------
XAPPLICATION* XAPPLICATION::Create()
{
	testmicro = new TESTMICRO();

	return testmicro;
}



//-------------------------------------------------------------------
//  TESTMICRO::Ini
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			13/08/2002 13:17:31
//	
//	@return 			bool : 
//	*/
//-------------------------------------------------------------------
bool TESTMICRO::Ini()
{	
	XSTRING string;			

  //XDEBUG_SETTARGET(0, XDEBUGCTRLTYPE_FILE, XDEBUG_DEFAULT_SPECIALAIM);	
   
	string = TESTMICRO_NAMEAPP;			
  XDEBUG_SETAPPLICATIONNAME(string);																				
	XDEBUG_SETAPPLICATIONVERSION(TESTMICRO_VERSION, TESTMICRO_SUBVERSION, TESTMICRO_SUBVERSIONERR);


  XDEBUG_PRINTCOLOR(1, __L("Start Application"));
    
	return true;
}
  



//-------------------------------------------------------------------
//  TESTMICRO::FirstUpdate
/**
//	
//	@author        Abraham J. Velez
//	@version       20/2/2003   16:26:12
//	
//	@return        bool : 
*/
//-------------------------------------------------------------------
bool TESTMICRO::FirstUpdate()
{	
  //Test_Random();
  Test_DIOStreamUART();
	//Test_DIOStreamUSB();
	//Test_DIOStreamI2C();
	//Test_DIOStreamSPI();
  //Test_DIOGPIO();
  //Test_SRAM();

	return true;		
}



//-------------------------------------------------------------------
//  TESTMICRO::Update
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/03/2004 16:53:03
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool TESTMICRO::Update()
{      
	return true;  
}





/*-------------------------------------------------------------------
//  TESTMICRO::End
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			14/05/2012 19:02:07
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool TESTMICRO::End()
{	
	  
	return true;
}




/*-------------------------------------------------------------------
//  TESTMICRO::Test_Random
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			14/05/2012 19:02:07
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool TESTMICRO::Test_Random()
{
  bool status = false;
  
  XRAND* xrand = xfactory->CreateRand();
  if(xrand)
    {     
      if(xrand->Max(50) == 5) status = false;    
      
      xfactory->DeleteRand(xrand);
    }
  
  if(status)
    {
    
    }
 
  return true;
}





/*-------------------------------------------------------------------
//	TESTMICRO::Test_DIOStreamUART
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			23/09/2016 9:41:36
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTMICRO::Test_DIOStreamUART()
{
	DIOSTREAMUARTCONFIG	diostreamuartconfig;
  DIOSTREAMUART*      diostreamuart;

	diostreamuartconfig.SetPort(1);		
	diostreamuartconfig.SetBaudRate(19200);
	diostreamuartconfig.SetDataBits(DIOSTREAMUARTDATABIT_8);
	diostreamuartconfig.SetParity(DIOSTREAMUARTPARITY_ODD);
	diostreamuartconfig.SetStopBits(DIOSTREAMUARTSTOPBITS_ONE);
	diostreamuartconfig.SetFlowControl(DIOSTREAMUARTFLOWCONTROL_NONE);		

	diostreamuart = (DIOSTREAMUART*)diofactory->CreateStreamIO(&diostreamuartconfig);
	if(!diostreamuart) return false;
	
	if(!diostreamuart->Open()) return false;
	
  #define MAXSIZEMSG 64
  
	XBYTE  data[MAXSIZEMSG];  
	int    br;
  bool   exit = false;

	while(!exit)
		{
			br = diostreamuart->GetInXBuffer()->GetSize();           
			if(br)
				{ 
					if(br > MAXSIZEMSG) br = MAXSIZEMSG;
          
					br = diostreamuart->Read(data, br);          
					diostreamuart->Write(data, br); 
          
					XDEBUG_PRINTDATABLOCKCOLOR(1, data, br);           
          
          /*
          for(int c=0; c<br; c++)
            {
              if(data[c] == 0x1B) 
                {
                  exit = true;
                  break;
                }
            }
          */
				}
		}

	diostreamuart->Close();
  diofactory->DeleteStreamIO(diostreamuart);
  
	return true;
}




/*-------------------------------------------------------------------
//	TESTMICRO::Test_DIOStreamUSBReadCommand
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			23/09/2016 9:41:58
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTMICRO::Test_DIOStreamUSBReadCommand(DIOSTREAM*	diostream, XBUFFER& command)
{
	#define IDLITTLEPROTOCOL 0xFE

	if(!diostream) return false;

	if(diostream->GetInXBuffer()->GetSize()<2) return false;
	 					
	XBYTE ID    = 0;
	XBYTE size  = 0;

	ID		= diostream->GetInXBuffer()->Get()[0];
	size	= diostream->GetInXBuffer()->Get()[1];

	if(ID != IDLITTLEPROTOCOL) return false;
	if(!size)									 return false;

	if(!diostream->WaitToFilledReadingBuffer(size, 4)) return false;

	command.Resize(size);
	
	if(diostream->Read(command) != size) return false;
																							 
	return true;
}



/*-------------------------------------------------------------------
//	TESTMICRO::Test_DIOStreamUSB
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			23/09/2016 9:41:58
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTMICRO::Test_DIOStreamUSB()
{
    
	DIOSTREAMUSBCONFIG	diostreamusbconfig;
  DIOSTREAMUSB*			  diostreamusb;      

	diostreamusbconfig.SetMode(DIOSTREAMMODE_CLIENT);	
	diostreamusb = (DIOSTREAMUSB*)diofactory->CreateStreamIO(&diostreamusbconfig);
	if(!diostreamusb) return false;
	
	if(!diostreamusb->Open()) return false;
	  
  XDEBUG_PRINTCOLOR(1, __L("Open USB ..."));
  
  
  /*
  #define IDLITTLEPROTOCOL 0xFE  
  
  XBUFFER xbuffer;
	bool    exit = false;

	while(!exit)
		{       
      if(Test_DIOStreamUSBReadCommand(diostreamusb, xbuffer))
        {
          diostreamusb->Write(xbuffer);  
                                       
        } 
    }
  */
 
  
  #define MAXSIZEMSG 64
  
	XBYTE  data[MAXSIZEMSG];  
	int    br;
  bool   exit = false;
   
	while(!exit)
		{
			br = diostreamusb->GetInXBuffer()->GetSize();           
			if(br)
				{ 
					if(br >= MAXSIZEMSG) br = MAXSIZEMSG-1;
          
          memset(data, 0 , MAXSIZEMSG);
          
					br = diostreamusb->Read(data, br);                    
					diostreamusb->Write(data, br); 
          				
                   
				}
		}  
    
	diostreamusb->Close();
  diofactory->DeleteStreamIO(diostreamusb);
  
	return true;
}
		


/*-------------------------------------------------------------------
//	TESTMICRO::Test_DIOStreamI2C
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			23/09/2016 9:42:03
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTMICRO::Test_DIOStreamI2C()
{ 

	//----------------------------------------------------------------------------
  
	DIOSTREAMI2CCONFIG	diostreami2cconfig;
  DIOSTREAMI2C*			  diostreami2c;      
    
  diostreami2cconfig.SetMode(DIOSTREAMMODE_SLAVE);
  diostreami2cconfig.SetPort(1);
  diostreami2cconfig.SetLocalAddress(0x30);  
  
	diostreami2c = (DIOSTREAMI2C*)diofactory->CreateStreamIO(&diostreami2cconfig);
	if(!diostreami2c) return false;
	
	if(!diostreami2c->Open()) return false;
    
 	XBYTE  data[16];  
	int    br;
  bool   exit = false;

	while(!exit)
		{       
      if(diostreami2c->WaitToFilledReadingBuffer(8, 1))
        {               
          memset(data, 0, 16);
          
          br = diostreami2c->Read(data, 8); 
          if(br)
            {
              XDEBUG_PRINTDATABLOCKCOLOR(2, data, br);     
              
              //diostreami2c->Write(data, 4);   
              //diostreami2c->WaitToFlushOutXBuffer(3);
              //XDEBUG_PRINTDATABLOCK(4 , data, 4);     
              
              if(data[0] == 0x1B) 
                {
                  exit = true;
                  break;
                }
            }
        } 
    }
    
	diostreami2c->Close();
  diofactory->DeleteStreamIO(diostreami2c);


	//----------------------------------------------------------------------------

  /*
	DIOI2CLIGHTSENSORTSL2561*	lightsensor;    

	lightsensor =  new DIOI2CLIGHTSENSORTSL2561();
  if(!lightsensor) return false;
    
  if(!lightsensor->Ini(1, 0x49, 1)) return false;	

	lightsensor->SetIntegrationTime(DIOI2CLIGHTSENSORTSL2561INTEGRATIONTIME_101MS);
	lightsensor->SetGain(DIOI2CLIGHTSENSORTSL2561GAIN_1X);

	XWORD		fullspectrum;
	XWORD		infrared;
	XDWORD	lux;
	bool		status;
	
	while(1)
		{ 		
			status = lightsensor->Get(fullspectrum, infrared, lux, true);
					
			XDEBUG_PRINTCOLOR(1, __L("Broadband: %d - Inflared: %d - Lux: %d  [%s]"), fullspectrum, infrared, lux, status?__L("Ok."):__L("Error!"));				

			//xsleep->Seconds(1);      
      //if(!status) break;
		}

  lightsensor->End();
  delete lightsensor;
  */
 
	//----------------------------------------------------------------------------
  /*
  DIOI2C6AXISTRACKINGLSM303DLHC * lsm303dlhc = new DIOI2C6AXISTRACKINGLSM303DLHC();
	if(lsm303dlhc) 
		{
			//if(lsm303dlhc->Ini(1, 0x18, 0x1e, 3))
      if(lsm303dlhc->Ini(1, 0x19, 0x1e, 3))      
				{
					lsm303dlhc->SetOffset(29.50f, -0.50f, 4.00f); 
					lsm303dlhc->SetScale(1.00f, 1.03f, 1.21f);		
          
          int counter = 0;
															
					while(1)
						{			               					
							float ypos = lsm303dlhc->Heading();
							
              XDEBUG_PRINTCOLOR(1,__L(" %6.2f "), ypos);	
              
              xsleep->MilliSeconds(200);
              
              counter++;
              if(counter>100) break;
							
						} 

					lsm303dlhc->End();												
				}

			delete lsm303dlhc;
		}
	*/  
  //----------------------------------------------------------------------------
 
	return true;
}
	


/*-------------------------------------------------------------------
//	TESTMICRO::Test_DIOStreamSPI
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			28/10/2016 8:35:41
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTMICRO::Test_DIOStreamSPI()
{
	bool status = false;
	
	//----------------------------------------------------------------------------

	DIOGPIO* diogpio = diofactory->CreateGPIO();
  if(diogpio) 
		{
			if(diogpio->Ini())
				{							
					DIOSPILCDSCREENPCF8833* lcdscreen = new DIOSPILCDSCREENPCF8833(xsystem);
					if(lcdscreen)     
						{
							lcdscreen->SetDIOGPIO(diogpio);					
              lcdscreen->SetResetPinsGroup(DIOGPIO_PINSGROUP_A);   
							lcdscreen->SetResetPin(TEST_PINLCD_RESET);   

							status  = lcdscreen->Ini(1, 0, 10);
							XDEBUG_PRINTCOLOR(1, __L("Send screen INI   : %s "), status?__L("Ok!"):__L("Error!"));
							if(status) 
								{
                  
									//status = lcdscreen->Clear(DIOSPILCDSCREENPHILIPSPCF8833_RED);
									//XDEBUG_PRINTCOLOR(1, __L("Send screen Clean : %s "), status?__L("Ok!"):__L("Error!"));		

									//xsleep->Seconds(1);
									if(status) 
										{
											//status = lcdscreen->Clear(DIOSPILCDSCREENPHILIPSPCF8833_GREEN);
											//XDEBUG_PRINTCOLOR(1, __L("Send screen Clean : %s "), status?__L("Ok!"):__L("Error!"));				

											//xsleep->Seconds(1);

											if(status) 
												{
													//status = lcdscreen->Clear(DIOSPILCDSCREENPHILIPSPCF8833_BLUE);
													//XDEBUG_PRINTCOLOR(1, __L("Send screen Clean : %s "), status?__L("Ok!"):__L("Error!"));				

													//xsleep->Seconds(1);
                          XRAND*  rnd = xfactory->CreateRand();																
                          if(rnd)
                            {
                              for(int y=0; y< lcdscreen->GetHeight(); y++)
                                {
                                  for(int x=0; x< lcdscreen->GetWidth(); x++)
                                    {
                                       lcdscreen->PutPixel(x , y, rnd->Between(0,65535));                                       
                                    }
                                } 
                              
                               xfactory->DeleteRand(rnd);
                            }
                          
                          
                          /*
													XWORD*  buffer = NULL;
													XRAND*  rnd = xfactory->CreateRand();
																	
                          if(rnd)
                            {
                              XDEBUG_PRINTCOLOR(1, __L("Create Random..."));	
                              
                              buffer = new XWORD[lcdscreen->GetNPixels()];
                              if(buffer) 
                                {
                                  XDEBUG_PRINTCOLOR(1, __L("Create Buffer Random..."));	
                                  
                                  int nframes = 0;

                                  while(nframes<100)
                                    {	
                                      for(XDWORD c=0; c<lcdscreen->GetNPixels(); c++)
                                        {
                                          buffer[c] = 23; //rnd->Between(0,65535);
                                        }																																																	
                                      
                                      lcdscreen->Update((XBYTE*)buffer);
                                      XDEBUG_PRINTCOLOR(1, __L("Send screen Update: %05d  %s    "), nframes,  status?__L("Ok!"):__L("Error!"));												

                                      nframes++;                                     
                                   }
                                  
                                } else XDEBUG_PRINTCOLOR(1, __L("Error Create Buffer Random..."));	

                              delete buffer;												
                              
                           } else XDEBUG_PRINTCOLOR(1, __L("Error Create Random..."));	
            
												 xfactory->DeleteRand(rnd);
                          
                        */
                                                    
											}
                                                                       
                        
										}
									
									lcdscreen->End();
								}											

							delete lcdscreen;
						}

					diogpio->End();
				}

			diofactory->DeleteGPIO(diogpio);
		}
		
	//----------------------------------------------------------------------------
	
	
	return true;
	
}





/*-------------------------------------------------------------------
//	TESTMICRO::Test_DIOGPIO
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			25/09/2016 13:03:25
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTMICRO::Test_DIOGPIO()
{
  DIOGPIO* diogpio = diofactory->CreateGPIO();
  if(!diogpio) return false;
  
  diogpio->Ini();
  
  int                   pin   = 0;
  DIOGPIO_PINSGROUP block = DIOGPIO_PINSGROUP_A;
  
  #ifdef STM32F072xB
  pin   = 9;
  block = DIOGPIO_PINSGROUP_C;
  #endif
  
  #ifdef STM32F303xC 
  pin   = 8;
  block = DIOGPIO_PINSGROUP_E;
  #endif
  
  #ifdef STM32F303xE
  pin   = 5;
  block = DIOGPIO_PINSGROUP_A;
  #endif
  
  #ifdef STM32F411xE
  pin   =15;
  block =DIOGPIO_PINSGROUP_D;
  #endif
   
  diogpio->SetMode(pin, false, block);
  
  int counter = 0;
  while(1)
    {
      diogpio->Set(pin, true, block);
      xsleep->Seconds(1);
      
      diogpio->Set(pin, false, block);
      xsleep->Seconds(1);
      
      if(counter >= 100) break;
      counter++;
    }
  
  diogpio->End();  
  diofactory->DeleteGPIO(diogpio);
  
  return true;
}


/*-------------------------------------------------------------------
//	TESTMICRO::Test_SRAM
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			25/09/2016 13:03:25
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTMICRO::Test_SRAM()
{
  #define SRAMMARK 0xAA55CAFE
   
  #if defined(STM32F407xx) || defined(STM32F446xx)
  
  XSTM32FXXXSRAM    sram;
  XDWORD            mark = 0;
  XBYTE             data[8];
          
  sram.Ini();
          
  //sram.EraseAll();
  
  memset(data, 0, 8);
          
  sram.Read(0, mark);
  if(mark != SRAMMARK)
    { 
      XDEBUG_PRINTCOLOR(3, __L("SRAM Mark NOT found 0x%X."), mark);
      
      XBYTE data2[8] = { 1,2,3,4,5,6,7,8 };
              
      mark = SRAMMARK;
                  
      sram.Write(0, mark);                                        
      sram.Write(4, data2, 8);                            
           
      
      mark = 0;     
      sram.Read(0, mark);
      
      XDEBUG_PRINTCOLOR(2, __L("SRAM Mark Set 0x%X."), mark);
      
    }
   else 
    {
      XDEBUG_PRINTCOLOR(2, __L("SRAM Mark FOUND  !!!"));                        
    }
                              
  sram.Read(4, data, 8);
  
  XDEBUG_PRINTCOLOR(2, __L("SRAM data:"));                        
  XDEBUG_PRINTDATABLOCKCOLOR(2, data, 8);                        
          
  sram.End();
  
  #endif
    
  return true;
}

