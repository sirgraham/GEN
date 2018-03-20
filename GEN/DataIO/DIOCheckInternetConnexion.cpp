/*------------------------------------------------------------------------------------------
//	DIOCHECKINTERNETCONNEXION.CPP
//	
//	Data IO Check Internet Connexion
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 05/11/2007 16:26:37
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.			 
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XFactory.h"
#include "XDateTime.h"

#include "DIOCheckConnexions.h"

#include "DIOCheckInternetConnexion.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/

	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/




/*-------------------------------------------------------------------
//  DIOCHECKINTERNETCONNEXION::DIOCHECKINTERNETCONNEXION
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			27/03/2013 22:33:02
//	
//	@return 			

 
//  @param				publisher : 
*/
/*-----------------------------------------------------------------*/
DIOCHECKINTERNETCONNEXION::DIOCHECKINTERNETCONNEXION(DIOCHECKCONNEXIONS* checkconnexions)
{
	typedef struct { XCHAR* ipaddr;
									 XCHAR* name;		

								 } MAINDNS;

	MAINDNS  maindns[] = {  { __L("8.8.8.8")					, __L("Google DNS 1")																	},
												  { __L("8.8.4.4")					, __L("Google DNS 2")																	},	
													{ __L("198.41.0.4")				, __L("VeriSign, Inc.")																},
													/*			
													{ __L("192.228.79.201")		, __L("University of Southern California (ISI)")			},
													{ __L("192.33.4.12")			, __L("Cogent Communications")												},
													{ __L("199.7.91.13")			, __L("University of Maryland")												},
													{ __L("192.203.230.10")		, __L("NASA (Ames Research Center)")									},	
													{ __L("192.5.5.241")			, __L("Internet Systems Consortium, Inc.")						},
													{ __L("192.112.36.4")			, __L("US Department of Defense (NIC)")								},													
													{ __L("198.97.190.53")		, __L("US Army (Research Lab)")												},
													{ __L("192.36.148.17")		, __L("Netnod")																				},	
													{ __L("192.58.128.30")		, __L("VeriSign, Inc.")																},													
													{ __L("193.0.14.129")			, __L("RIPE NCC")																			},	
													*/
													{ __L("199.7.83.42")			, __L("ICANN")																				},
												  { __L("202.12.27.33")			, __L("WIDE Project")																	}
											 };


	Clean();

	if(!checkconnexions) return;

	this->checkconnexions	= checkconnexions;
	
	xmutexconnexioncuts = xfactory->Create_Mutex();

	checkconnexions->Setup(checkconnexions->GetTimeConnexionChecks(), true, false);

	for(int c=0; c<(sizeof(maindns)/sizeof(MAINDNS)); c++)
		{
			checkconnexions->Connexion_Add(maindns[c].ipaddr, connexionID[c]); 
		}

	checkconnexions->Connexions_WaitToSomeIsChecked(checkconnexions->GetTimeConnexionChecks());
}




/*-------------------------------------------------------------------
//  DIOCHECKINTERNETCONNEXION::~DIOCHECKINTERNETCONNEXION
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			05/11/2007 16:28:53
//	
//	@return
//	*/
/*-----------------------------------------------------------------*/
DIOCHECKINTERNETCONNEXION::~DIOCHECKINTERNETCONNEXION()
{	
	DeleteAllConnexionCuts();

	if(checkconnexions)
		{
			for(int c=0; c<DIOCHECKINTERNETCONNEXION_MAXCONNEXIONS; c++)
				{
					checkconnexions->Connexion_DeleteByID(connexionID[c]);
				}			
		}

	xfactory->Delete_Mutex(xmutexconnexioncuts);
	
	Clean();
}






/*-------------------------------------------------------------------
//  DIOCHECKINTERNETCONNEXION::Check
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			16/02/2014 11:02:01
//	
//	@return 			bool : 
//	@param				nretrys : 
//  @param				timebetweenchecks : 
//  @param				localIP : 
*/
/*-----------------------------------------------------------------*/
bool DIOCHECKINTERNETCONNEXION::Check()
{
	if(!checkconnexions) return false;

	bool				isinternetactive		= false;
	static bool isinternetactiveold = false;
	
	if(checkconnexions) isinternetactive = checkconnexions->Connexions_SomeIsConnected();

	if(isinternetactiveold != isinternetactive)
		{
			if(!isinternetactive)
				{
					DIOCHECKCONNEXIONCUT connexioncut;

					connexioncut.GetStartDateTime()->Read();
					connexioncut.SetMeasureNSeconds(DIOCHECKCONNEXIONS_INFINITECUT);

					AddConnexionCut(connexioncut);						
				}
			 else
				{
					if(GetConnexionCuts()->GetSize())
						{
							DIOCHECKCONNEXIONCUT* connexioncut = GetConnexionCuts()->Get(GetConnexionCuts()->GetSize()-1);
							if(connexioncut)
								{
									if(connexioncut->GetMeasureNSeconds() == DIOCHECKCONNEXIONS_INFINITECUT)
										{
											XDATETIME* xdatetime = xfactory->CreateDateTime();
											if(xdatetime)
												{
													xdatetime->Read();

													connexioncut->SetMeasureNSeconds((int)(xdatetime->GetSeconsFromDate() - connexioncut->GetStartDateTime()->GetSeconsFromDate()));

													xfactory->DeleteDateTime(xdatetime);
												}
										}
								}
						}
				}

			isinternetactiveold = isinternetactive;

			ischangeconnexionstatus = true;

		} else ischangeconnexionstatus = false;

		
	return isinternetactive;
}




