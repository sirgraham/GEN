/*------------------------------------------------------------------------------------------
//	DIOSNMP.CPP
//	
//	Data IO SNMP Class
//   
//	Author				: Abraham J. Velez
//	Date Of Creation	: 03/09/2012 11:17:10
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.	
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XFactory.h"
#include "XBuffer.h"
#include "XDebug.h"

#include "DIOFactory.h"
#include "DIOStreamUDPConfig.h"
#include "DIOStreamUDP.h"

#include "DIOSNMP.h"
	
#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//  DIOSNMPXBER::DIOSNMPXBER
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			04/09/2012 16:07:20
//	
//	@return 			

*/
/*-----------------------------------------------------------------*/
DIOSNMPXBER::DIOSNMPXBER() : XBER()
{

}



/*-------------------------------------------------------------------
//  DIOSNMPXBER::~DIOSNMPXBER
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			04/09/2012 16:08:01
//	
//	@return 			
//	*/
/*-----------------------------------------------------------------*/
DIOSNMPXBER::~DIOSNMPXBER()
{

}



/*-------------------------------------------------------------------
//  DIOSNMPXBER::SetIPADDRESS
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			04/09/2012 16:08:32
//	
//	@return 			bool : 
//	@param				IPstring : 
*/
/*-----------------------------------------------------------------*/
bool DIOSNMPXBER::SetIPADDRESS(XCHAR* IPstring)
{
	if(!IPstring)			return false;
	if(!IPstring[0])  return false;
	if(!data)					return false;

	data->Delete();

	XSTRING				string;
	XVECTOR<int>	numbers;
	int						start = 0;
	int						end		= 0;
	
	string = IPstring;

	while(1)
		{	
			XSTRING numberstring;
			int			indexstring = string.Find(__L("."),true,start);

			if(indexstring != XSTRING_NOTFOUND)
				{					
					end = indexstring;
					string.Copy(start,end,numberstring);
					numbers.Add(numberstring.ConvertToInt());
					start = end+1;
				}
			 else
			  {
					string.Copy(end+1,numberstring);
					numbers.Add(numberstring.ConvertToInt());

					break;
				}
		}

	if(!numbers.GetSize()) return false;

	for(XDWORD c=0;c<numbers.GetSize();c++)
		{
			size_t _number = (size_t)(numbers.Get(c));
			int number = (int)(_number);
			data->Add((XBYTE)(number));	
		}
	
	this->type = (XBYTE)(DIOSNMPBERTYPE_IPADDRESS | XBERTYPE_ISAPPLICATION);
	this->size = data->GetSize();

	return true;
}



/*-------------------------------------------------------------------
//  DIOSNMPXBER::SetTIMETICKS
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			04/09/2012 16:36:22
//	
//	@return 			bool : 
//	@param				ticks : 
*/
/*-----------------------------------------------------------------*/
bool DIOSNMPXBER::SetTIMETICKS(XDWORD ticks)
{
	bool status = SetINTEGER(ticks);

	this->type = (XBYTE)(DIOSNMPBERTYPE_TIMETICKS | XBERTYPE_ISAPPLICATION);
	
	return status;
}



/*-------------------------------------------------------------------
//  DIOSNMPXBER::Sequence_AddTo
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			04/09/2012 16:29:02
//	
//	@return 			bool : 
//	@param				newxber : 
*/
/*-----------------------------------------------------------------*/
bool DIOSNMPXBER::Sequence_AddTo(XBER& xber)
{
	bool status = XBER::Sequence_AddTo(xber);

	this->type |= (XBYTE)(XBERTYPE_ISCONSTRUCTED);

	return status;
}



/*-------------------------------------------------------------------
//  DIOSNMPXBER::AddToTrapData
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			04/09/2012 16:58:08
//	
//	@return 			bool : 
//	@param				newxber : 
*/
/*-----------------------------------------------------------------*/
bool DIOSNMPXBER::TrapData_AddTo(XBER& xber)
{
	bool status = XBER::Sequence_AddTo(xber);

	this->type = (XBYTE)(DIOSNMPBERTYPE_TRAPPDU | XBERTYPE_ISCONTEXTSPECIFIC | XBERTYPE_ISCONSTRUCTED);

	return status;
}



/*-------------------------------------------------------------------
//  DIOSNMPTRAP::DIOSNMPTRAP
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			03/09/2012 12:34:54
//	
//	@return 			

 
//  @param				xpublisher : 
*/
/*-----------------------------------------------------------------*/
DIOSNMPTRAP::DIOSNMPTRAP()
{
	Clean();

	
	;
}



/*-------------------------------------------------------------------
//  DIOSNMPTRAP::~DIOSNMPTRAP
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			03/09/2012 11:26:27
//	
//	@return 			
//	*/
/*-----------------------------------------------------------------*/
DIOSNMPTRAP::~DIOSNMPTRAP()
{
	Clean();
}



/*-------------------------------------------------------------------
//  DIOSNMPTRAP::Open
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      14/09/2012 8:30:16
//  
//  @return       bool : 
//  @param        IPtarget : 
//  @param        port : 
//  @param        checkavailableNMS : 
*/
/*-----------------------------------------------------------------*/
bool DIOSNMPTRAP::Open(XCHAR* IPtarget, int port, bool checkavailableNMS, XCHAR* localIP)
{
	if(!IPtarget)			return false;
	if(!IPtarget[0])  return false;
	
	diostreamudpcfg = new DIOSTREAMUDPCONFIG();
	if(!diostreamudpcfg)  return false;
	
	diostreamudpcfg->SetMode(DIOSTREAMMODE_CLIENT);
  
	if(localIP) diostreamudpcfg->GetLocalIP()->Set(localIP);
	
	diostreamudpcfg->GetRemoteURL()->Set(IPtarget);				
  diostreamudpcfg->SetRemotePort(port);

	bool status = false;

	diostreamudp = (DIOSTREAMUDP*)diofactory->CreateStreamIO(diostreamudpcfg);
	if(diostreamudp) 
		{
			if(diostreamudp->Open()) 
        { 
          if(checkavailableNMS)
            {
              //--------------------------------------------
              // Write something to received a ICMP result.
              // and check if something is the other side.
     
              status = diostreamudp->WaitToConnected(3);
              if(status)
                {              
                  XBYTE data[] = { 0xAA,0x55 };              
                  diostreamudp ->Write(data,sizeof(data));

                  status = diostreamudp->WaitToDisconnected(3);
                }

            } else status = true;
        }
		} 

  if(!status) Close();
  
	return status;
}




/*-------------------------------------------------------------------
//  DIOSNMPTRAP::Send
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			06/09/2012 10:56:46
//	
//	@return 			bool : 
//	@param				community : 
//  @param				enterprise : 
//  @param				agentip : 
//  @param				generictrap : 
//  @param				specifictrap : 
//  @param				timestamp : 
//  @param				nvar : 
//  @param				... : 
*/
/*-----------------------------------------------------------------*/
bool DIOSNMPTRAP::Send(XCHAR* community,XCHAR* enterprise,XCHAR* agentip,int generictrap,int specifictrap,XDWORD timestamp,int nvar,...)
{
	if(!xfactory)					return false;
	if(!diostreamudpcfg)	return false;
	if(!diostreamudp)			return false;

 	XBUFFER			 xbuffer;	
	DIOSNMPXBER  trap;
	DIOSNMPXBER  trapdata;
	DIOSNMPXBER  trapvars;
	DIOSNMPXBER  trapvar;
	DIOSNMPXBER  xber;

	// ----------------------------------------------------------------
	// Trap 
	xber.SetINTEGER(0);								trap.Sequence_AddTo(xber);				// Version SNMPv1
	xber.SetOCTETSTRING(community);		trap.Sequence_AddTo(xber);				// Community

	// ----------------------------------------------------------------
	// Trap Data

  xber.SetOID(enterprise);					trapdata.TrapData_AddTo(xber);		// Enterprise
	xber.SetIPADDRESS(agentip);				trapdata.TrapData_AddTo(xber);		// IP Address
	xber.SetINTEGER(generictrap);			trapdata.TrapData_AddTo(xber);		// Generic-trap
	xber.SetINTEGER(specifictrap);		trapdata.TrapData_AddTo(xber);		// Specific-trap	
	xber.SetTIMETICKS(timestamp);			trapdata.TrapData_AddTo(xber);		// timestamp	

	// ----------------------------------------------------------------
	// Trap Data Variable
	// (only available, for the moment, the next types: INTEGER, STRING and OIDs
	//
	if(nvar)
		{
			if(!(nvar%2))
				{
					va_list	arg;
	
					va_start(arg,nvar);  

					for(int c=0;c<(nvar/2);c++)
						{					
							trapvar.Sequence_DeleteAll();

						 for(int d=0;d<2;d++)
								{
									int type = (int)va_arg(arg, int);

									switch(type)
										{
											case XBERTYPE_INTEGER			: { int integer = (int)va_arg(arg, int);
																										xber.SetINTEGER(integer);
																										trapvar.Sequence_AddTo(xber);																							
																									}
																									break;
							
											case XBERTYPE_OCTETSTRING	: { XCHAR* OCTETstring = (XCHAR*)va_arg(arg, XCHAR*);
																										if(OCTETstring) 
																											{
																												xber.SetOCTETSTRING(OCTETstring);
																												trapvar.Sequence_AddTo(xber);	
																											}
																									}
																									break;

											case XBERTYPE_OID					: { XCHAR* OIDstring = (XCHAR*)va_arg(arg, XCHAR*);
																										if(OIDstring) 
																											{
																												xber.SetOID(OIDstring);
																												trapvar.Sequence_AddTo(xber);	
																											}
																									}
																									break;
										}
								}	

							trapvars.Sequence_AddTo(trapvar);	
						}

					va_end(arg);

					trapdata.TrapData_AddTo(trapvars);
			 }
		}

	trap.Sequence_AddTo(trapdata);

	trap.GetDump(xbuffer);

	//DEBUG_PRINTDATABLOCK(xbuffer.Get(),xbuffer.GetSize(),2);

	XDWORD bw = diostreamudp->Write(xbuffer.Get(),xbuffer.GetSize());
	if(bw!= xbuffer.GetSize()) return false;
	
	return true;
}




/*-------------------------------------------------------------------
//  DIOSNMPTRAP::End
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			03/09/2012 12:26:22
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool DIOSNMPTRAP::Close()
{
	diostreamudp->WaitToFlushXBuffers();

	if(diostreamudp)
	  {
			diostreamudp->Close();	
		  diofactory->DeleteStreamIO(diostreamudp);
		  diostreamudp = NULL;
		}

	if(diostreamudpcfg)
		{
			delete diostreamudpcfg;
			diostreamudpcfg = NULL;
		}

	return true;
}



