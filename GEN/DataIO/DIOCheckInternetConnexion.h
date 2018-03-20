/*------------------------------------------------------------------------------------------
//	DIOCHECKINTERNETCONNEXION.H
*/	
/**	
// \class 
//   
//  Data IO Check Internet Connexion
//   
//	@author	 Abraham J. Velez
//	@version 05/11/2007 16:20:28
*/	
/*	GEN  Copyright (C).  All right reserved.	
//----------------------------------------------------------------------------------------*/
	
#ifndef _DIOCHECKINTERNETCONNEXION_H_
#define _DIOCHECKINTERNETCONNEXION_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "XVector.h"

#include "DIOCheckConnexions.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

#define DIOCHECKINTERNETCONNEXION_MAXCONNEXIONS		16

/*---- CLASS -----------------------------------------------------------------------------*/
	
class XFACTORY;
class XPUBLISHER;
class DIOFACTORY;
class DIOCHECKCONNEXIONS;


class DIOCHECKINTERNETCONNEXION
{
	public:	
																						DIOCHECKINTERNETCONNEXION							(DIOCHECKCONNEXIONS* checkconnexions);
		virtual																 ~DIOCHECKINTERNETCONNEXION							();

		DIOCHECKCONNEXIONS*											GetCheckConnexions										()																{ return checkconnexions;																								}
	
		bool																		Check																	();

		
		bool																		AddConnexionCut												(DIOCHECKCONNEXIONCUT& connexioncut)
																						{
																							DIOCHECKCONNEXIONCUT* chekconnexioncut = new DIOCHECKCONNEXIONCUT();
																							if(!chekconnexioncut) return false;
	
																							chekconnexioncut->CopyFrom(&connexioncut);

																							if(connexioncuts.GetSize() >= DIOCHECKCONNEXIONS_MAXNCONNEXIONCUTS) connexioncuts.Delete(connexioncuts.Get(0));
	
																							connexioncuts.Add(chekconnexioncut);

																							return true;
																						}
		

		bool																		DeleteAllConnexionCuts								()
																						{
																							if(connexioncuts.IsEmpty()) return false;

																							if(!xmutexconnexioncuts) return false;

																							xmutexconnexioncuts->Lock();

																							connexioncuts.DeleteContents();	
																							connexioncuts.DeleteAll();

																							xmutexconnexioncuts->UnLock();

																							return true;
																						}

		XVECTOR<DIOCHECKCONNEXIONCUT*>*					GetConnexionCuts											()																{ return &connexioncuts;																							}
		DIOCHECKCONNEXIONCUT*										GetLastConnexionsCut									()																{	return GetConnexionCuts()->Get(GetConnexionCuts()->GetSize()-1);		}

		bool																		IsChangeConnexionStatus								()																{ return ischangeconnexionstatus;																			}
	
	private:

		void																		Clean																	()
																						{																																														
																							checkconnexions					= NULL;

																							for(int c=0; c<DIOCHECKINTERNETCONNEXION_MAXCONNEXIONS; c++)
																								{
																									connexionID[c] = 0;
																								}

																							ischangeconnexionstatus = false;
																							xmutexconnexioncuts			= NULL;
																						}

		DIOCHECKCONNEXIONS*											checkconnexions;

		XDWORD																	connexionID[DIOCHECKINTERNETCONNEXION_MAXCONNEXIONS];
		
		bool																		ischangeconnexionstatus;

		XMUTEX*																	xmutexconnexioncuts;
		XVECTOR<DIOCHECKCONNEXIONCUT*>					connexioncuts;			
};

	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

