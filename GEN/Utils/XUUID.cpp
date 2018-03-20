/*------------------------------------------------------------------------------------------
//	XUUID.CPP
//	
//	XUUID class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 07/05/2013 22:02:27
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "XUUID.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//  XUUID::XUUID
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			07/05/2013 22:08:25
//	
//	@return 			
//	*/
/*-----------------------------------------------------------------*/
XUUID::XUUID()
{
	Clean();
}


/*-------------------------------------------------------------------
//  XUUID::~XUUID
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			07/05/2013 22:08:33
//	
//	@return 			
//	*/
/*-----------------------------------------------------------------*/
XUUID::~XUUID()
{
	Clean();
}
		


/*-------------------------------------------------------------------
//  XUUID::GetData1
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			07/05/2013 22:09:03
//	
//	@return 			XDWORD : 
//	*/
/*-----------------------------------------------------------------*/
XDWORD XUUID::GetData1()
{
	return data1;
}
		


/*-------------------------------------------------------------------
//  XUUID::GetData2
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			07/05/2013 22:09:24
//	
//	@return 			XWORD : 
//	*/
/*-----------------------------------------------------------------*/
XWORD XUUID::GetData2()
{
	return data2;
}



/*-------------------------------------------------------------------
//  XUUID::GetData3
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			07/05/2013 22:10:10
//	
//	@return 			XWORD : 
//	*/
/*-----------------------------------------------------------------*/
XWORD XUUID::GetData3()
{
	return data3;
}



/*-------------------------------------------------------------------
//  XUUID::GetData4
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			07/05/2013 22:10:48
//	
//	@return 			XBYTE* : 
//	*/
/*-----------------------------------------------------------------*/
XBYTE* XUUID::GetData4()
{
	return data4;
}



/*-------------------------------------------------------------------
//  XUUID::SetData1
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			07/05/2013 22:10:58
//	
//	@return 			bool : 
//	@param				data : 
*/
/*-----------------------------------------------------------------*/
bool XUUID::SetData1(XDWORD data)
{
	this->data1 = data;

	return true;
}


/*-------------------------------------------------------------------
//  XUUID::SetData2
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			07/05/2013 22:12:14
//	
//	@return 			bool : 
//	@param				data : 
*/
/*-----------------------------------------------------------------*/
bool XUUID::SetData2(XWORD data)
{
	this->data2 = data;

	return true;
}



/*-------------------------------------------------------------------
//  XUUID::SetData3
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			07/05/2013 22:14:10
//	
//	@return 			bool : 
//	@param				data : 
*/
/*-----------------------------------------------------------------*/
bool XUUID::SetData3(XWORD data)
{
	this->data3 = data;
	
	return true;
}



/*-------------------------------------------------------------------
//  XUUID::SetData4
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			07/05/2013 22:14:38
//	
//	@return 			bool : 
//	@param				data : 
*/
/*-----------------------------------------------------------------*/
bool XUUID::SetData4(XBYTE* data)
{
	for(int c=0; c<XUUIDMAXDATA4; c++)
		{
			this->data4[c] = data[c];
		}

	return true;
}




/*-------------------------------------------------------------------
//  XUUID::SetData
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			07/05/2013 22:15:58
//	
//	@return 			bool : 
//	@param				data1 : 
//  @param				data2 : 
//  @param				data3 : 
//  @param				data4 : 
*/
/*-----------------------------------------------------------------*/
bool XUUID::Set(XDWORD data1, XWORD data2, XWORD data3, XBYTE* data4)
{
	SetData1(data1);
	SetData2(data2);
	SetData3(data3);
	SetData4(data4);

	return true;
}



/*-------------------------------------------------------------------
//  XUUID::CopyFrom
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			07/05/2013 22:25:48
//	
//	@return 			bool : 
//	@param				uuid : 
*/
/*-----------------------------------------------------------------*/
bool XUUID::CopyFrom(XUUID& uuid)
{
	return Set(uuid.GetData1(), uuid.GetData2(), uuid.GetData3(), uuid.GetData4());
}



/*-------------------------------------------------------------------
//  XUUID::CopyTo
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			07/05/2013 22:25:53
//	
//	@return 			bool : 
//	@param				uuid : 
*/
/*-----------------------------------------------------------------*/
bool XUUID::CopyTo(XUUID& uuid)
{
	return uuid.Set(data1, data2, data3, data4);
}



/*-------------------------------------------------------------------
//  XUUID::GetString
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			07/05/2013 22:35:30
//	
//	@return 			bool : 
//	@param				string : 
*/
/*-----------------------------------------------------------------*/
bool XUUID::GetString(XSTRING& string)
{
	string.Format(__L("%08X-%04X-%04X-%02X%02X%02X%02X%02X%02X%02X%02X"), data1, data2, data3, data4[0]
																																													 , data4[1]
																																													 , data4[2]
																																													 , data4[3]
																																													 , data4[4]
																																													 , data4[5]
																																													 , data4[6]
																																													 , data4[7]);

	return true;
}




/*-------------------------------------------------------------------
//  XUUID::Clean
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			07/05/2013 22:16:16
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
void XUUID::Clean()
{	
	data1 = 0;
	data2 = 0;
	data3 = 0;
	
	for(int c=0; c<XUUIDMAXDATA4; c++)
		{
			this->data4[c] = 0;
		}

}
	