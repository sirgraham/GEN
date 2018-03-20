/*------------------------------------------------------------------------------------------
//	XBER.CPP
//	
//	BER (Basic Encoding Rules) encoding class 
//   
//	Author				: Abraham J. Velez
//	Date Of Creation	: 03/09/2012 17:01:15
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "XBuffer.h"

#include "XBER.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  XBER::XBER
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			03/09/2012 17:13:19
//	
//	@return 			
//	*/
/*-----------------------------------------------------------------*/
XBER::XBER()
{
	Clean();

	data = new XBUFFER();
	
}



/*-------------------------------------------------------------------
//  XBER::~XBER
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			03/09/2012 17:13:44
//	
//	@return 			virtual : 
//	*/
/*-----------------------------------------------------------------*/
XBER::~XBER()
{
	if(data) delete data;

	Sequence_DeleteAll();

	Clean();
}



/*-------------------------------------------------------------------
//	XBER::GetType
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			03/04/2014 16:31:49
//	
//	@return 			XBYTE : 
//
*/
/*-----------------------------------------------------------------*/
XBYTE XBER::GetType()
{
	return (XBYTE)(type&0x3F);
}




/*-------------------------------------------------------------------
//  XBER::GetSize
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			04/09/2012 9:06:54
//	
//	@return 			XDWORD : 
//	*/
/*-----------------------------------------------------------------*/
XDWORD XBER::GetSize()
{
	return size;
}


/*-------------------------------------------------------------------
//  XBER::GetData
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			10/09/2012 13:04:09
//	
//	@return 			XBUFFER* : 
//	*/
/*-----------------------------------------------------------------*/
XBUFFER* XBER::GetData()
{
	return data;
}



/*-------------------------------------------------------------------
//  XBER::GetDump
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			03/09/2012 17:35:28
//	
//	@return 			bool : 
//	@param				xbuffer : 
*/
/*-----------------------------------------------------------------*/
bool XBER::GetDump(XBUFFER& xbuffer)
{
	xbuffer.Add((XBYTE)type);
	
	XBUFFER sizedata;	
	if(CodeSize(size,sizedata) && type!=XBERTYPE_NULL) xbuffer.Add(&sizedata);

	if(data->GetSize()) xbuffer.Add(data->Get(),data->GetSize());

	for(XDWORD c=0;c<sequence.GetSize();c++)
		{
			XBER* xber = (XBER*)sequence.Get(c);
			if(xber) xber->GetDump(xbuffer);
		}

	return true;
}



/*-------------------------------------------------------------------
//  XBER::SetType
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			03/09/2012 17:36:32
//	
//	@return 			bool : 
//	@param				type : 
*/
/*-----------------------------------------------------------------*/
bool XBER::SetType(XBYTE type)
{
	this->type = type;

	return true;
}



/*-------------------------------------------------------------------
//	XBER::SetNULL
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			03/04/2014 16:02:41
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool XBER::SetNULL()
{
	if(!data)			 return false;

	data->Delete();
	
	data->Add((XBYTE)0);
	
	this->type = XBERTYPE_NULL;
	this->size = 0;

	return true;
}





/*-------------------------------------------------------------------
//  XBER::SetINTEGER
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			06/09/2012 11:16:03
//	
//	@return 			bool : 
//	@param				integer : 
*/
/*-----------------------------------------------------------------*/
bool XBER::SetINTEGER(XDWORD integer)
{
	if(!data) return false;

	data->Delete();

	int size = 0;

	if(integer&0xFF800000)	
		{	
			size = 4;
		}
	 else
		{
			if(integer&0x00FF8000)  
			  {
					size = 3;
				}
			 else
			  {
					if(integer&0x0000FF80)  
						{
							size = 2;
						}
					 else
						{
							size = 1;							
						}
				}
		}

	if(!size) return false;

	for(int c=(size-1);c>=0;c--)
		{
			XBYTE byte = (XBYTE)((c?(integer>>(c*8)):(integer))&0xFF);

			data->Add((XBYTE)byte);
		}

	this->type = XBERTYPE_INTEGER;
	this->size = size;

	return true;
}




/*-------------------------------------------------------------------
//	XBER::SetINTEGER
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			04/04/2014 14:17:33
//	
//	@return 			bool : 
//
//  @param				xbufferdata : 
*/
/*-----------------------------------------------------------------*/
bool XBER::SetINTEGER(XBUFFER& xbufferdata)
{
	if(!data) return false;

	data->Delete();
	
	data->Add(xbufferdata);
		
	this->type = XBERTYPE_INTEGER;
	this->size = xbufferdata.GetSize();

	return true;
}



/*-------------------------------------------------------------------
//	XBER::SetBITSTRING
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			07/04/2014 11:37:09
//	
//	@return 			bool : 
//
//  @param				string : 
*/
/*-----------------------------------------------------------------*/
/*
bool XBER::SetBITSTRING(XCHAR* string)
{	
	if(!string)		 return false;
	if(!string[0]) return false;
	if(!data)			 return false;

	data->Delete();
	
	XSTRING _string;
	int			paddingsize;

	_string = string;

	paddingsize = _string.GetSize()%8;

	for(int c=0;c<paddingsize;c++)
		{
			_string += __C('0');
		}

	XBUFFER sizedata();	
	if(CodeSize(paddingsize, sizedata))  data->Add(&sizedata);


	int index  = 0;
	int bitpos = 0;
	
	while(index<(int)_string.GetSize())
		{
			XBYTE bytedata = 0;

			for(int c=0; c<8; c++)
				{
					if(_string.Get()[index] == __C('1')) bytedata |= 0x01;

					if(c!=7) bytedata <<= 1;
					
					index++;
				}

			data->Add(bytedata);
		}

	this->type = XBERTYPE_BITSTRING;
	this->size = data->GetSize();
	
	return true;
}
*/



/*-------------------------------------------------------------------
//	XBER::SetBITSTRING
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			07/04/2014 12:46:24
//	
//	@return 			bool : 
//
//  @param				xbuffer : 
*/
/*-----------------------------------------------------------------*/
bool XBER::SetBITSTRING(XBUFFER& xbuffer)
{
	if(!xbuffer.GetSize()) return false;
	if(!data)							 return false;

	data->Delete();

	data->Add(xbuffer);

	this->type = XBERTYPE_BITSTRING;
	this->size = xbuffer.GetSize();

	return true;
}
	



/*-------------------------------------------------------------------
//	XBER::SetBITSTRING
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			07/04/2014 12:46:56
//	
//	@return 			bool : 
//
//  @param				buffer : 
//  @param				size : 
*/
/*-----------------------------------------------------------------*/
bool XBER::SetBITSTRING(XBYTE* buffer, int size)
{
	if(!buffer) return false;
	if(!size)		return false;
	if(!data)		return false;

	data->Delete();

	data->Add(buffer, size);

	this->type = XBERTYPE_BITSTRING;
	this->size = size;

	return true;
}






/*-------------------------------------------------------------------
//  XBER::SetOCTETSTRING
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			04/09/2012 8:24:50
//	
//	@return 			bool : 
//	@param				string : 
*/
/*-----------------------------------------------------------------*/
bool XBER::SetOCTETSTRING(XCHAR* string)
{
	if(!string)		 return false;
	if(!string[0]) return false;
	if(!data)			 return false;

	data->Delete();
	
	XSTRING _string;

	_string = string;

	XSTRING_CREATEOEM(_string, charOEM)
	data->Add((XBYTE*)charOEM, _string.GetSize());
	XSTRING_DELETEOEM(charOEM)

	this->type = XBERTYPE_OCTETSTRING;
	this->size = _string.GetSize();

	return true;
}
	



/*-------------------------------------------------------------------
//	XBER::SetOCTETSTRING
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			04/04/2014 9:52:14
//	
//	@return 			bool : 
//
//  @param				databuffer : 
*/
/*-----------------------------------------------------------------*/
bool XBER::SetOCTETSTRING(XBUFFER& xbuffer)
{
	if(!xbuffer.GetSize()) return false;
	if(!data)							 return false;

	data->Delete();

	data->Add(xbuffer);

	this->type = XBERTYPE_OCTETSTRING;
	this->size = xbuffer.GetSize();

	return true;
}
	


/*-------------------------------------------------------------------
//	XBER::SetOCTETSTRING
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			04/04/2014 10:03:44
//	
//	@return 			bool : 
//
//  @param				buffer : 
//  @param				size : 
*/
/*-----------------------------------------------------------------*/
bool XBER::SetOCTETSTRING(XBYTE* buffer, int size)
{
	if(!buffer) return false;
	if(!size)		return false;
	if(!data)		return false;

	data->Delete();

	data->Add(buffer, size);

	this->type = XBERTYPE_OCTETSTRING;
	this->size = size;

	return true;
}





/*-------------------------------------------------------------------
//  XBER::SetOID
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			04/09/2012 9:19:05
//	
//	@return 			bool : 
//	@param				OIDstring : 
*/
/*-----------------------------------------------------------------*/
bool XBER::SetOID(XCHAR* OIDstring)
{
	if(!OIDstring)		return false;
	if(!OIDstring[0]) return false;
	if(!data)					return false;

	data->Delete();

	XSTRING				string;
	XVECTOR<int>	numbers;
	int						start = 0;
	int						end		= 0;
	
	string = OIDstring;

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

	//---------------------------------------------------
	// The firts 2 bytes are special
	//
	
	size_t _first = (size_t)(numbers.Get(0));
	int first = (int)_first;

	first *= (40);
	_first = (size_t)(numbers.Get(1));
	first += (int)(_first);

	data->Add((XBYTE)first);

	for(XDWORD c=2;c<numbers.GetSize();c++)
		{
			XBUFFER _data;

			size_t _number = (size_t)(numbers.Get(c));
			XDWORD number = (XDWORD)(_number);

			if(CodeBigNumber((XDWORD)(number),_data))
				{
					data->Add(_data.Get(),_data.GetSize());	
				}					
		}
	
	this->type = XBERTYPE_OID;
	this->size = data->GetSize();

	return true;
}






/*-------------------------------------------------------------------
//  XBER::Copy
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			04/09/2012 17:15:35
//	
//	@return 			bool : 
//	@param				newber : 
*/
/*-----------------------------------------------------------------*/
bool XBER::Copy(XBER* newxber)
{
	if(!newxber)			 return false;
	if(!newxber->data) return false;

	newxber->type = type;
	newxber->size = size;

	newxber->data->Delete();
	newxber->data->Add(data->Get(),data->GetSize());

	for(XDWORD c=0;c<sequence.GetSize();c++)
		{
			XBER* xbertmp = (XBER*)sequence.Get(c);
			if(xbertmp) 
				{
					XBER* newxberseq = new XBER();
					if(newxberseq) 
						{
							if(xbertmp->Copy(newxberseq))  newxber->sequence.Add(newxberseq);
						}
				}
		}


	return true;
}




/*-------------------------------------------------------------------
//  XBER::AddToSequence
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			04/09/2012 8:24:54
//	
//	@return 			bool : 
//	@param				newxber : 
*/
/*-----------------------------------------------------------------*/
bool XBER::Sequence_AddTo(XBER& xber)
{
	if(!data)			 return false;

	data->Delete();

	XBER* newxber = new XBER();
	if(!newxber) return false;

	if(!xber.Copy(newxber)) 
		{
			delete newxber;
			return false;
		}

	sequence.Add(newxber);
	
	this->type = XBERTYPE_SEQOFSEQ;
	this->size = 0;

	for(XDWORD c=0;c<sequence.GetSize();c++)
		{
			XBER* xber = (XBER*)sequence.Get(c);
			if(xber) this->size += xber->Sequence_GetSize();
		}
  
	return true;
}



/*-------------------------------------------------------------------
//  XBER::Sequence_GetSize
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			04/09/2012 9:00:23
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
XDWORD XBER::Sequence_GetSize()
{  
	XDWORD  sizeall = 0;

  // Size type
	if(type != XBERTYPE_NULL) sizeall++;		

  // Size of size;
  XBUFFER sizedata;

  CodeSize(size,sizedata);
	sizeall += sizedata.GetSize();

	sizeall += data->GetSize();
	
	for(XDWORD c=0;c<sequence.GetSize();c++)
		{
			XBER* xber = (XBER*)sequence.Get(c);
			if(xber) sizeall += xber->Sequence_GetSize();
		}
	
	return sizeall;
}




/*-------------------------------------------------------------------
//  XBER::Sequence_DeleteAll
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			03/09/2012 17:53:02
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool XBER::Sequence_DeleteAll()
{
	if(sequence.IsEmpty()) return false;
	
	sequence.DeleteContents();

	sequence.DeleteAll();

	return true;
}



/*-------------------------------------------------------------------
//  XBER::CodeBigNumber
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			04/09/2012 11:03:40
//	
//	@return 			bool : 
//	@param				number : 
//  @param				data : 
*/
/*-----------------------------------------------------------------*/
bool XBER::CodeBigNumber(XDWORD number,XBUFFER& data)
{	
	data.Delete();

  int value = number;
  int length = 0;
 
  if(value >= (268435456)) 
		{ // 2 ^ 28
      length = 5;
    } 
	 else 
		{
			if(value >= (2097152)) 
				{ // 2 ^ 21
	        length = 4;
				} 
			 else 
				{
					if(value >= 16384) 
						{ // 2 ^ 14
					    length = 3;
				    } 
					 else 
						{
							if(value >= 128) 
								{ // 2 ^ 7
							    length = 2;
				        } 
						   else 
								{
							    length = 1;
								}
						}
				}
		}


	int j = 0;
  for(j = length - 1; j >= 0; j--) 
	 {
		 if(j) 
			{
				int p = ((value >> (7 * j)) & 0x7F) | 0x80;        
				data.Add((XBYTE)p);
			} 
		 else 
			{
				int p = ((value >> (7 * j)) & 0x7F);				
				data.Add((XBYTE)p);
		  }			
		}

	return true;
 }
 


/*-------------------------------------------------------------------
//  XBER::CodeSize
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      10/09/2012 17:58:45
//  
//  @return       bool : 
//  @param        number : 
//  @param        data : 
*/
/*-----------------------------------------------------------------*/
bool XBER::CodeSize(XDWORD integer,XBUFFER& data)
{	
	data.Delete();

	XBYTE size = 0;

	if(integer&0xFF800000)	
		{	
			size = 4;
		}
	 else
		{
			if(integer&0x00FF8000)  
			  {
					size = 3;
				}
			 else
			  {
					if(integer&0x0000FF80)  
						{
							size = 2;
						}
					 else
						{
							size = 1;							
						}
				}
		}

	if(!size) return false;

  if(size>1) data.Add((XBYTE)((size-1)|0x80));

	for(int c=(size-data.GetSize()-1);c>=0;c--)
		{
			XBYTE byte = (XBYTE)((c?(integer>>(c*8)):(integer))&0xFF);

			data.Add((XBYTE)byte);
		}
  
	return true;
 }
 



/*-------------------------------------------------------------------
//  XBER::Clean
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			03/09/2012 17:16:10
//	
//	@return 			void : 
//	*/
/*-----------------------------------------------------------------*/
void XBER::Clean()
{
	type			= 0;
	size			= 0;											
	data			= NULL;
}

