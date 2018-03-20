
/*------------------------------------------------------------------------------------------
//	TESTBASEDIOFUNCTIONS.CPP
//	
//	Test Base DataIO Functions
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 30/05/2016 16:19:29
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XRand.h"
#include "XSleep.h"
#include "XThreadCollected.h"
#include "XApplication.h"

#include "GRPFile3DFBX.h"
#include "GRPFile3DOBJ.h"
#include "GRPObject.h"
#include "GRPScene.h"
#include "GRPMaterialLibrary.h"
#include "GRPShaderLibrary.h"
#include "GRPFont.h"
#include "GRPFontManager.h"
#include "GRPConsole.h"
#include "GRPRenderer.h"
#include "GRPVisualDebugger.h"
#include "GRPViewPort.h"
#include "GRPScene.h"
#include "GRPCamera.h"

#include "DIOFactory.h"
#include "DIOPing.h"
#include "DIOStreamDeviceIP.h"
#include "DIOStreamIPLocalEnumDevices.h"
#include "DIOStreamUDPConfig.h"
#include "DIOStreamUDP.h"
#include "DIOStreamUDPLocalEnumServers.h"
#include "DIOStreamTCPIPConfig.h"
#include "DIOStreamTCPIP.h"

#include "DIOStreamUSBLocalEnumDevices.h"
#include "DIOStreamDeviceUSB.h"
#include "DIOStreamUSBConfig.h"
#include "DIOStreamUSB.h"

#include "TestBaseDIOFunctions.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//	TESTBASEDIOFUNCTIONS::TESTBASEDIOFUNCTIONS
*/	
/**	
//	
//	Class Constructor TESTBASEDIOFUNCTIONS
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			04/11/2015 10:54:11
//	
//  @param				inpmanager : 
//  @param				screen : 
//  @param				scene : 
*/
/*-----------------------------------------------------------------*/
TESTBASEDIOFUNCTIONS::TESTBASEDIOFUNCTIONS(GRPAPPLICATION* xapplication) :  TESTBASE(xapplication)
{	
	xtimer	= xfactory->CreateTimer();
	xthread = CREATEXTHREAD(XTHREADGROUPID_APPOWNER, __L("TESTBASEDIOFUNCTIONS::TESTBASEDIOFUNCTIONS"), ThreadRunFunction, (void*)this);
	xrand   = xfactory->CreateRand();
}




/*-------------------------------------------------------------------
//	TESTBASEDIOFUNCTIONS::~TESTBASEDIOFUNCTIONS
*/	
/**	
//	
//	 Class Destructor TESTBASEDIOFUNCTIONS
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			04/11/2015 10:54:21
//	
*/
/*-----------------------------------------------------------------*/
TESTBASEDIOFUNCTIONS::~TESTBASEDIOFUNCTIONS()
{
	if(xthread)
		{
			xthread->End();
			DELETEXTHREAD(XTHREADGROUPID_APPOWNER, xthread);
		}

	xfactory->DeleteTimer(xtimer);

	xfactory->DeleteRand(xrand);
}



/*-------------------------------------------------------------------
//	TESTBASEDIOFUNCTIONS::IniResources
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			27/10/2015 11:25:28
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASEDIOFUNCTIONS::IniResources()
{
	if(!xapplication)										 return NULL;
	if(!xapplication->GetMainScene())		 return NULL;
	if(!xapplication->GetMainScreen())		 return NULL;

	GRPSCENE*  scene			= xapplication->GetMainScene();
	GRPRENDERER* renderer = xapplication->GetMainRenderer();
	GRPSCREEN* screen			= xapplication->GetMainScreen();

	PathManagement(__L("diofunctions"));


	XPATH xpath;

	xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1 ,__L(""));
	GRPMATERIALLIBRARY::Get()->SetResourceLocation(xpath);
	
	
	//----------------------------------- Cargar un material		
	xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("txt_gen.jpg"));	
	XDEBUG_PRINTCOLOR(1,__L("%s"),xpath.Get());

	if(!GRPMATERIALLIBRARY::Get()->Load(xpath.Get(),	__L("txt_gen.jpg")))
		{
			XDEBUG_PRINTCOLOR(1,__L("Error loading Texture data"));
			return false;
		}				

	xpath.Create(XPATHSMANAGERSECTIONTYPE_FONTS, 1, __L("SourceCodePro-Regular.ttf"));
	
	GRPFONTATTRIBUTES att;
	att.extrude		= 0;
	att.monospace = true;
	att.quality		= 1;
	att.back			= false;

	if(!GRPOBJECTBUILDER::Get()->GetFontManager()->Load(xpath.Get(), __L("Console") ,&att)) //load default
		{
			XDEBUG_PRINTCOLOR(1,__L("TTF file not loaded! arial.ttf"));
			return false;
		}

	GRPFONTMANAGER* fontmanager = GRPOBJECTBUILDER::Get()->GetFontManager();
	
	GRPFONT* font = fontmanager->GetFont(__L("Console"));
	font->SetIsMonospace(true);


	GRPSCENE* consolescene = new GRPSCENE(scene->GetScreen());
	if(!consolescene)
		{
			return false;
		}
	
	consolescene->Ini();

	GRPVIEWPORT* viewport = consolescene->GetViewport();

	viewport->SetSize(1.0f, 1.0f);
	viewport->SetPosition(0.0f, 0.0f);
	viewport->SetProjection(GRPPROJECTIONTYPE_ORTHO);
	viewport->SelectProjectionOrtho();
	viewport->Select();
	//consolescene->GetViewports()->Add(viewport);
	//consolescene->GetScreen()->AddViewport(viewport);

	//xapplication->GetMainScene()->Add(viewport);
	
	consolescene->GetCamera()->SetPosition(0.0f, 0.0f, 150.0f);
	consolescene->GetCamera()->GetTarget()->SetPosition(0.0f, 0.0f, 0.0f);

	// add camera
	scene->renderer->AddScene(consolescene);


	console = new GRPCONSOLE( scene->renderer->GetScene(1));
	if(!console)
		{
			return false;
		}

	console->SetFont(font);
	console->Create();

	if(screen->IsFullScreen())
					console->SetFontScale(0.5f);
		else  console->SetFontScale(0.15f);
	
	console->SetArea(0.0f, 0.0f, 1.0f, 1.0f);
	console->GetNode()->modified = true;
	console->GetNode()->SetVisible(true);

	console->SetTextProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"SDF"));
	console->SetArea(0.0f, 0.0f, 1.0f, 1.0f);
	
	console->SetViewport(0.0f, 0.0f, 1.0f, 1.0f);

	console->SetBackgroundMaterial(GRPMATERIALLIBRARY::Get()->Select(__L("txt_gen.jpg")));

	scene->renderer->GetScene(1)->Add(console);
	
	console->SetColor(GRPCOLOR::BLACK);

	console->Printf(__L("Esto es una prueba..."));
	console->Printf(__L("Esto es una prueba..."));
				
	if(xthread) xthread->Ini();
		
	return true;
}

/*-------------------------------------------------------------------
//	TESTBASEDIOFUNCTIONS::EndResources
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			03/06/2016 10:30:22
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASEDIOFUNCTIONS::EndResources()
{
	return false;
}

/*-------------------------------------------------------------------
//	TESTBASEDIOFUNCTIONS::Test_DIOStreamTCPIP
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			03/01/2016 17:44:06
//	
//	@return 			bool : 
//
//  @param				modeserver : 
*/
/*-----------------------------------------------------------------*/
bool TESTBASEDIOFUNCTIONS::Test_DIOStreamTCPIP(bool modeserver)
{	
	DIOSTREAMTCPIPCONFIG * diostreamcfg;
	DIOSTREAMTCPIP*				 diostream;
	
	diostreamcfg = new DIOSTREAMTCPIPCONFIG (); 
	if(diostreamcfg)
		{
			XDEBUG_PRINTCOLOR(1,__L("Modo Servidor: %s\n"), modeserver?__L("Si"):__L("No"));

			if(modeserver)
				{
					diostreamcfg->SetMode(DIOSTREAMMODE_SERVER);
				}
			else
				{
					diostreamcfg->SetMode(DIOSTREAMMODE_CLIENT);
				}
			
			diostreamcfg->SetRemotePort(4000);

			diostream = (DIOSTREAMTCPIP*)diofactory->CreateStreamIO(diostreamcfg);
			if(diostream)
				{
					DIOSTREAMUDPLOCALENUMSERVERS* localenumservers = new DIOSTREAMUDPLOCALENUMSERVERS();
					if(localenumservers)
						{
							localenumservers->SetID(0xFAAFFAFA);
							localenumservers->SetPort(4001);

							if(diostreamcfg->IsServer())
								{
									diostream->SetEnumServers(localenumservers);			

									if(diostream->Open())
										{
											XSTRING line;

											while(diostream->GetConnectStatus() != DIOSTREAMSTATUS_CONNECTED)
												{
													if(console->KBHit()) break;

													xsleep->MilliSeconds(50);
												}

											XDEBUG_PRINTCOLOR(1,__L("Open Server..."));

											while(!console->KBHit())
												{
													diostream->ReadStr(line);
													if(!line.IsEmpty()) 
														{	
															XDEBUG_PRINTCOLOR(1,__L("< Line: %s "), line.Get());
															break;
														}
												}

											XDEBUG_PRINTCOLOR(1,__L("Close..."));

											diostream->Close();
										}
								}
							 else
							  {
									localenumservers->Search();
																			
									while(localenumservers->IsSearching())
										{
											if(console->KBHit()) break;

											xsleep->MilliSeconds(50);
										}	

									localenumservers->StopSearch(true);

									if(localenumservers->AreDevicesAvailable())
										{
											DIOSTREAMDEVICEIP* device = (DIOSTREAMDEVICEIP*)localenumservers->GetDevices()->Get(0);
											if(device) 
												{
													XSTRING IPstring;

													device->GetIP()->GetXString(IPstring);

													XDEBUG_PRINTCOLOR(1,__L("Found Server in: %s "), IPstring.Get());
													diostreamcfg->GetRemoteURL()->Set(IPstring.Get());												
												
													if(diostream->Open())
														{
															XSTRING line;

															while(diostream->GetConnectStatus() != DIOSTREAMSTATUS_CONNECTED)
															{
																if(console->KBHit()) break;

																xsleep->MilliSeconds(50);
															}

															XDEBUG_PRINTCOLOR(1,__L("Open Client..."));

															line = __L("Hola radiola\n");
									
															diostream->WriteStr(line.Get());
									
															XDEBUG_PRINTCOLOR(1,__L("> Line: %s "), line.Get());

															XDEBUG_PRINTCOLOR(1,__L("Close..."));

															xsleep->MilliSeconds(1000);

															diostream->Close();

														}
												}
										} 									 
								}

							delete localenumservers;
						}
					
					diofactory->DeleteStreamIO(diostream);
				}

			delete diostreamcfg;
		}
		
	return true;
}
	


/*-------------------------------------------------------------------
//	TESTBASEDIOFUNCTIONS::Test_DIOStreamUDP
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			16/01/2016 20:13:25
//	
//	@return 			bool : 
//
//  @param				modeserver : 
*/
/*-----------------------------------------------------------------*/
bool TESTBASEDIOFUNCTIONS::Test_DIOStreamUDP(bool modeserver)
{
	XSTRING							 autoaddress;
	DIOSTREAMUDPCONFIG * diostreamcfg;
	DIOSTREAMUDP*				 diostream;
	
	diostreamcfg = new DIOSTREAMUDPCONFIG();
	if(!diostreamcfg) return false;

	DIOSTREAMIPLOCALENUMDEVICES* enumdevices = (DIOSTREAMIPLOCALENUMDEVICES*)diofactory->CreateStreamEnumDevices( DIOSTREAMENUMTYPE_IP_LOCAL);
	if(enumdevices)
		{			
			DIOSTREAMDEVICEIP* device = (DIOSTREAMDEVICEIP*)enumdevices->GetFirstActiveDevice();
			if(device) device->GetIP()->GetXString(autoaddress);
				
			diofactory->DeleteStreamEnumDevices(enumdevices);				
		}	
	
	XDEBUG_PRINTCOLOR(1, __L("Modo Servidor: %s\n"), modeserver?__L("Si"):__L("No"));

	if(modeserver)
		{
			diostreamcfg->SetMode(DIOSTREAMMODE_SERVER);
		}
	else
		{
			diostreamcfg->SetMode(DIOSTREAMMODE_CLIENT);
		}

	if(!modeserver) diostreamcfg->GetRemoteURL()->Set(__L("192.168.1.3")  /*autoaddress*/ );		
	diostreamcfg->SetRemotePort(4000);	

	diostream = (DIOSTREAMUDP*)diofactory->CreateStreamIO(  diostreamcfg);
	if(diostream)
		{					
			if(diostream->Open())
				{
					if(diostream->WaitToConnected(5))
						{	
							XBUFFER xbuffer;
							XDWORD  br;
							XDWORD  bw;
							int			sizeforbuffer = 200;
							bool	  status;

							XDEBUG_PRINTCOLOR(1, __L("Open ...\n"));

							if(!modeserver)
								{
									//while(!xconsole->KBHit())
									while(1)
										{								
											xbuffer.Delete();
											for(int c=0; c<sizeforbuffer; c++)
												{
													xbuffer.Add((XBYTE)xrand->Between(0,255));
												}

											bw = diostream->Write(xbuffer.Get(), xbuffer.GetSize());
											status = (bw == xbuffer.GetSize())?true:false; 
											if(status) status = diostream->WaitToFlushOutXBuffer(5);
													
											if(!status)
												{
													XDEBUG_PRINTCOLOR(1, __L("Error to Write ...\n"));							
											}
										}
								}
							 else
								{
									//while(!xconsole->KBHit())
									while(1)
										{								
											if(diostream->WaitToFilledReadingBuffer(sizeforbuffer, 1))
												{																												
 													xbuffer.Delete();
													xbuffer.Resize(sizeforbuffer);
									
													br = diostream->Read(xbuffer.Get(), xbuffer.GetSize());
													status = (br == xbuffer.GetSize())?true:false; 
													if(status)
														{
															XDEBUG_PRINTDATABLOCK(modeserver?1:2, xbuffer);								

														}	else XDEBUG_PRINTCOLOR(1, __L("Error to Read ...\n"));																																				
												}
										}
								}

							XDEBUG_PRINTCOLOR(1, __L("Close...\n"));

							diostream->Close();
						}
				}
			
			diofactory->DeleteStreamIO(diostream);
		}
					
	delete diostreamcfg;
		
	return true;
}




/*-------------------------------------------------------------------
//	TESTBASEDIOFUNCTIONS::Test_DIOPing
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			09/01/2016 12:47:54
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASEDIOFUNCTIONS::Test_DIOPing()
{
	int counter = 0;
	
	XDEBUG_PRINTCOLOR(1, __L("do..."));

	DIOPING* ping = new DIOPING();
	if(ping)
		{
			while(counter<5)
				{			
					ping->DeleteAllReplys();

					//ping->Set(__L("www.google.com"));
					//ping->Set(__L("google.es"));

					ping->Set(__L("192.168.1.3"));

					ping->Do(3);
				
					XDEBUG_PRINTCOLOR(1,__L("Respuestas %d "), ping->GetReplys()->GetSize());

					for(int c=0;c<(int)ping->GetReplys()->GetSize();c++)
						{
							DIOPINGREPLY* reply = (DIOPINGREPLY*)ping->GetReplys()->Get(c);
							if(reply)
								{
									XDEBUG_PRINTCOLOR(1, __L("Respuesta desde %s: bytes=%d tiempo=%dms TTL=%d ")	, reply->GetFromIP()->Get()
																																																	,	reply->GetSizeRequest()
																																																	, reply->GetTimeElapsed()
																																																	,	reply->GetTTL());									
								}
						}

					counter++;
				}

			delete ping;
		}

	return true;
}		
	


/*-------------------------------------------------------------------
//	TESTBASEDIOFUNCTIONS::Test_DIOStreamUSB
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			16/01/2016 20:11:52
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASEDIOFUNCTIONS::Test_DIOStreamUSB()
{
	DIOSTREAMUSBLOCALENUMDEVICES* enumdevices;
	DIOSTREAMDEVICEUSB*						device = NULL;
	XSTRING												deviceID;
	XDWORD												PID = 0x0000;
	XDWORD												VID = 0x0000;

	/*
	if(GetExecParams())
		{
			for(int c=0; c<2; c++)
				{
					XSTRING* param = (XSTRING*)GetExecParams()->Get(c);	
					if(param) 
						{
							param->ToUpperCase(); 					

							switch(c)
								{
									case 0: param->UnFormat(__L("%08X"), &PID); 
													break;

									case 1: param->UnFormat(__L("%08X"), &VID); 
													break;
								}
						}
				}
		}
	

	if(!PID || !VID) 
		{
			XDEBUG_PRINTCOLOR(1, __L("Invalid Product ID or Vendor ID!"));	
			return false;
		}
	*/

	XDEBUG_PRINTCOLOR(1,__L("Search USB device: Product ID %08X, Vendor ID %08X"), PID, VID);	

  XDEBUG_PRINTCOLOR(1, __L("-----------------------------------------------------"));	

	enumdevices = (DIOSTREAMUSBLOCALENUMDEVICES*)diofactory->CreateStreamEnumDevices( DIOSTREAMENUMTYPE_USB_LOCAL);
	if(!enumdevices) return false;	

	enumdevices->Search();
		
	XDEBUG_PRINTCOLOR(1, __L("USB enum:"));	

	for(int c=0;c<(int)enumdevices->GetDevices()->GetSize();c++)
		{				
			device = (DIOSTREAMDEVICEUSB*)enumdevices->GetDevices()->Get(c);
			if(device) 
				{				
					device->DebugPrintInfo(NULL);	

					if((device->GetProductID() == PID) && (device->GetVendorID() == VID))
						{																				
							 break;

						} else device = NULL;
				}
		}
	

	if(device) 
		{
			DIOSTREAMUSBCONFIG * diostreamcfg = new DIOSTREAMUSBCONFIG();
			if(diostreamcfg) 
				{			
					diostreamcfg->SetMode(DIOSTREAMMODE_CLIENT);
			
					//diostreamcfg->GetDeviceID()->Set(__L("\\\\?\\usb#vid_0dd4&pid_015d#vkp80_200num.:_0#{28d78fad-5a12-11d1-ae5b-0000f803a8c2}"));
					//diostreamcfg->GetDeviceID()->Set(__L("/dev/usb/lp0"));
					//diostreamcfg->GetDeviceID()->Set(__L("/dev/bus/usb/001/002"));

					diostreamcfg->GetResource()->Set(device->GetResource()->Get());
				

					XDEBUG_PRINTCOLOR(1, __L("Create USB CFG"));
			
					DIOSTREAM* diostream = (DIOSTREAM*)diofactory->CreateStreamIO(  diostreamcfg);
					if(diostream) 
						{	
							XDEBUG_PRINTCOLOR(1, __L("Create USB Stream"));

							if(diostream->Open())
								{	
									XDEBUG_PRINTCOLOR(1, __L("Open USB Stream"));

									XBUFFER xbuffer;

									while(1)
										{		
											/*
											if(xconsole->KBHit())
												{
													XBYTE character = xconsole->GetChar();
													if(character == 27) break;
													if(!character)			break;
													
													xbuffer.Add(character);

													if(diostream->Write(xbuffer)) diostream->WaitToFlushOutXBuffer(5);	
													xbuffer.Delete();												

													XDEBUG_PRINTCOLOR(1, __L("%c"),  character);																																			
												}
											*/

											int size = diostream->GetInXBuffer()->GetSize();
											if(size)
												{
													xbuffer.Resize(size);

													if(diostream->Read(xbuffer))
														{	
															for(XDWORD c=0; c<xbuffer.GetSize(); c++)
																{
																	XDEBUG_PRINTCOLOR(1, __L("%c"), xbuffer.Get()[c]);			
																}																			
														}			

													xbuffer.Delete();
												}									
										}
															
									diostream->Close();		
								
									XDEBUG_PRINTCOLOR(1, __L("Close USB Stream"));							
								}

							diofactory->DeleteStreamIO(diostream);
						}
		
					delete diostreamcfg;
				}

		} 
   else 
		{
			XDEBUG_PRINTCOLOR(1, __L("\nDevice not found!"));	
		}

	diofactory->DeleteStreamEnumDevices(enumdevices);			

	return true;
}




/*-------------------------------------------------------------------
//	TESTBASEDIOFUNCTIONS::Do
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			27/10/2015 11:25:39
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASEDIOFUNCTIONS::Do()
{
	console->Update();

	console->Update();

	return true;
}




/*-------------------------------------------------------------------
//	TESTBASEDIOFUNCTIONS::ThreadRunFunction
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			08/01/2016 23:19:37
//	
//  @param				param : 
*/
/*-----------------------------------------------------------------*/
void TESTBASEDIOFUNCTIONS::ThreadRunFunction(void* param)
{
	TESTBASEDIOFUNCTIONS* testDIOfunctions = (TESTBASEDIOFUNCTIONS*)param;
	if(!testDIOfunctions) return;

	//testDIOfunctions->Test_DIOStreamTCPIP(false);
	testDIOfunctions->Test_DIOStreamUDP(false);
	//testDIOfunctions->Test_DIOPing();
	//testDIOfunctions->Test_DIOStreamUSB();

	if(testDIOfunctions->xthread) testDIOfunctions->xthread->Run(false);
}

