//------------------------------------------------------------------------------------------
//	DIOSTREAMICMP.H
//	
/**	
// \class 
//   
//  Data IO Stream ICMP class
//   
//	@author	 Abraham J. Velez
//	@version 23/05/2002
*/	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#ifndef _DIOSTREAMICMP_H_
#define _DIOSTREAMICMP_H_
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "XVector.h"
#include "XBuffer.h"

#include "DIOStream.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

#define DIOSTREAMICMP_NOTFOUND						-1

//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XTIMER;
class XMUTEX;
class DIOFACTORY;
class DIOURL;
class DIOSTREAMICMPCONFIG;


class DIOSTREAMICMPDATAGRAM
{
	public:
																			DIOSTREAMICMPDATAGRAM											()
																			{
																				Clean();
																				
																				data = new XBUFFER();																				
																			}

		virtual													 ~DIOSTREAMICMPDATAGRAM											()
																			{
																				if(data) delete data;

																				Clean();
																			}


		bool															IsToSend																	()																{ return istosend;                      }
		void															SetIsToSend																(bool istosend)										{ this->istosend = istosend;            }
		
		XSTRING*													GetAddress																()																{	return &address;											}					
		bool															SetAddress																(XCHAR* address)
																			{
																				if(!address) return false;
																				this->address = address;
																				return true;
																			}	
		bool															SetAddress																(XSTRING& address)								{	return SetAddress(address.Get());			}
		
		XBUFFER*													GetData																		()																{ return data;													}
		bool															SetData																		(XBYTE* data,int size)
																			{
																				if(!this->data) return false;
																				this->data->Delete();
																				this->data->Add(data,size);

																				return true;
																			}

		void  														SetData																		(XBUFFER& data)										{ SetData(data.Get(), data.GetSize());  }
		
	private:

		void															Clean																			()
																			{
																				istosend = false;
																				address.Empty();
																				data		 = NULL;
																			}

		bool															istosend;
		XSTRING														address;
		XBUFFER*													data;
};




class DIOSTREAMICMP : public DIOSTREAM
{
	public:
																			DIOSTREAMICMP															();
		virtual													 ~DIOSTREAMICMP															();		

		DIOSTREAMCONFIG*									GetConfig																	();
		bool															SetConfig																	(DIOSTREAMCONFIG* config);
		
		virtual bool											Open																			() 																= 0;

		XDWORD														Read																			(XBYTE* buffer, XDWORD size);		
		XDWORD														Write																			(XBYTE* buffer, XDWORD size);

		bool															ReadDatagram															(XSTRING& address, XBUFFER& xbuffer);

		bool															WriteDatagram															(XSTRING& address, XBYTE* buffer, int size);
		bool															WriteDatagram															(XSTRING& address, XBUFFER& xbuffer);

		virtual bool											Disconnect																()																= 0;
		virtual bool											Close																			() 																= 0;	

		bool															ResetXBuffers															();
		bool															ResetInXBuffer														();
		bool															ResetOutXBuffer														();

		DIOURL*														GetHost																		()																{ return host;														};

    XVECTOR<DIOSTREAMICMPDATAGRAM*>*	GetDatagramsVector												()																{ return &datagrams;                      };	
		bool															DeleteAllDatagrams												();

	protected:
	
		bool															AddDatagram																(bool istosend, XCHAR* address, XBYTE* data, int size);
		bool															AddDatagram																(bool istosend, XSTRING& address, XBYTE* data, int size);
		
		DIOSTREAMICMPDATAGRAM*						GetDatagram																(int index);
		int																GetFirstDatagram													(bool issend);

		bool															DeleteDatagram														(int index);
	

		DIOSTREAMICMPCONFIG*							config;
		DIOURL*														host;	

	private:
		
		void															Clean																			();

		XMUTEX*														datagramsmutex;
		XVECTOR<DIOSTREAMICMPDATAGRAM*>		datagrams;
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif
