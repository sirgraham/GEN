/*------------------------------------------------------------------------------------------
// DIODynDNSManager.HPP
*/
/**
// \class
//
//  DynDNS Manager DNS List
//
//	@author: Abraham J. Velez 
//
//	Date of Creation : 09/01/2018 20:27:50 
//	Last Modification : 
*/
/* (C) Copyright EndoraSoft S.L . All right reserved.
/*------------------------------------------------------------------------------------------*/


#ifndef _DIODYNDNSMANAGER_HPP
#define _DIODYNDNSMANAGER_HPP

/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XVector.h"
#include "DIODynDNS.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

/*---- CLASS -----------------------------------------------------------------------------*/

class DIODYNDNSMANAGER
{
	public:
																DIODYNDNSMANAGER     ()										{ Clean();												}
															 ~DIODYNDNSMANAGER		 ()										
															  { 
																  DeleteAllDNS();
																  Clean();												
															  }

		XSTRING*                    GetLogin						 ()										{ return &login;									}
		XSTRING*                    GetPassword					 ()										{ return &password;								}


		bool												AddDNS							 (XSTRING& DNS)
																{
																	if(DNS.IsEmpty()) return false;

																	XSTRING* _DNS = new XSTRING();
																	if(!_DNS) return false;

																	_DNS->Set(DNS);

																	DNSlist.Add(_DNS);

																	return true;
																}

		bool												DeleteAllDNS				 ()
																{
																	if(DNSlist.IsEmpty()) return false;

																	DNSlist.DeleteContents();
																	DNSlist.DeleteAll();

																	return true;	
																}

		XDWORD                      GetNDNS              ()
																{
																	return DNSlist.GetSize();
																}
		
		bool												AssingAll						 ()
																{
																	DIODYNDNS_STATUSCODE	statusupdate;
																	DIOURL								url;
																	DIOIP 								IP;																	
																	int										result = 0;
																	bool									status = true;
																	
																	DIODYNDNS* dyndns = new DIODYNDNS(); 
																	if(!dyndns) return false;
												
																	dyndns->GetLogin()->Set(login.Get());
																	dyndns->GetPassword()->Set(password.Get());

																	if(!DNSlist.GetSize()) status = false;

																	for(XDWORD c=0; c<DNSlist.GetSize(); c++)
																		{	
																			XSTRING* DNS = DNSlist.Get(c);
																			if(DNS)
																				{
																					url.Set(DNS->Get());

																					bool ischanged = false;
						
																					if(dyndns->IsChangedPublicIP(url, ischanged, &IP))
																						{
																							if(ischanged)
																								{																									
																									statusupdate = dyndns->Update(url, IP);
																									if(statusupdate == DIODYNDNS_STATUSCODE_GOOD) result = 1; 
				
																								} else result = 2;

																						} else result = 0;
																				}

																			if(!result) status = false;
																		}

																	delete dyndns;

																	return status;
																}

	private:

		void												Clean								()
																{

																}

		XVECTOR<XSTRING*>						DNSlist;

		XSTRING                     login;
		XSTRING                     password;
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif