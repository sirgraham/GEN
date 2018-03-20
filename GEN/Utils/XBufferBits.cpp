
/*------------------------------------------------------------------------------------------
//	XBUFFERBITS.CPP
//	
//	reads bits from an xbuffer
//   
//	Author						: Imanol Celaya Ruiz de Alegria
//	Date Of Creation	: 01/04/2016 14:27:11
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	

#include "XBuffer.h"

#include "XBufferBits.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/




/*-------------------------------------------------------------------
//	XBUFFERBITS::XBUFFERBITS
*/	
/**	
//	
//	Class Constructor XBUFFERBITS
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/04/2016 16:10:10
//	
//  @param				xbuffer : 
*/
/*-----------------------------------------------------------------*/
XBUFFERBITS::XBUFFERBITS(XBUFFER* xbuffer)
{
	Clean();

	this->xbuffer = xbuffer;
	this->p = xbuffer->Get();
	this->bits_left = 8;
}





/*-------------------------------------------------------------------
//	XBUFFERBITS::~XBUFFERBITS
*/	
/**	
//	
//	 Class Destructor XBUFFERBITS
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/04/2016 16:10:16
//	
*/
/*-----------------------------------------------------------------*/
XBUFFERBITS::~XBUFFERBITS()
{
	Clean();
}





/*-------------------------------------------------------------------
//	XBUFFERBITS::IsByteAligned
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/04/2016 16:10:23
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool XBUFFERBITS::IsByteAligned()
{
	return bits_left == 8;
}






/*-------------------------------------------------------------------
//	XBUFFERBITS::IsEOF
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/04/2016 16:10:31
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool XBUFFERBITS::IsEOF()
{
	return p >= (xbuffer->Get()+xbuffer->GetSize());
}





/*-------------------------------------------------------------------
//	XBUFFERBITS::IsOverrun
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/04/2016 16:10:38
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool XBUFFERBITS::IsOverrun()
{
	return (p >= (xbuffer->Get()+xbuffer->GetSize())) && (bits_left < 8);
}





/*-------------------------------------------------------------------
//	XBUFFERBITS::GetBytePosition
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/04/2016 16:10:45
//	
//	@return 			XDWORD : 
//
*/
/*-----------------------------------------------------------------*/
XDWORD XBUFFERBITS::GetBytePosition()
{
	return (XDWORD)(xbuffer->Get() - p);
}





/*-------------------------------------------------------------------
//	XBUFFERBITS::GetBitPosition
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/04/2016 16:10:52
//	
//	@return 			XDWORD : 
//
*/
/*-----------------------------------------------------------------*/
XDWORD XBUFFERBITS::GetBitPosition()
{
	return 8 - bits_left;
}




/*-------------------------------------------------------------------
//	XBUFFERBITS::SetPosition
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/04/2016 16:10:59
//	
//  @param				index : 
//  @param				bitindex : 
*/
/*-----------------------------------------------------------------*/
void XBUFFERBITS::SetPosition(XDWORD index, XDWORD bitindex)
{
	p = xbuffer->Get() + index;
	bits_left = 8 - ((bitindex >= 8) ? 7 : 0); // zero base index, can't be 8
}





/*-------------------------------------------------------------------
//	XBUFFERBITS::SkipU1
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/04/2016 16:11:07
//	
*/
/*-----------------------------------------------------------------*/
void XBUFFERBITS::SkipU1()
{
	bits_left--;

  if(bits_left == 0)
		{
			p++;
			bits_left = 8;
		}
}





/*-------------------------------------------------------------------
//	XBUFFERBITS::PeekU1
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/04/2016 16:11:13
//	
//	@return 			XDWORD : 
//
*/
/*-----------------------------------------------------------------*/
XDWORD XBUFFERBITS::PeekU1()
{
	XDWORD r = 0;

  if(!IsEOF())
		{
			r = ((*(p)) >> ( bits_left - 1 )) & 0x01;
		}

  return r;
}





/*-------------------------------------------------------------------
//	XBUFFERBITS::ReadU1
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/04/2016 16:11:20
//	
//	@return 			XDWORD : 
//
*/
/*-----------------------------------------------------------------*/
XDWORD XBUFFERBITS::ReadU1()
{
	XDWORD r = 0;
    
  bits_left--;

  if (! IsEOF())
		{
      r = ((*(p)) >> bits_left) & 0x01;
		}

  if (bits_left == 0)
		{
			p++;
			bits_left = 8;
		}

  return r;
}





/*-------------------------------------------------------------------
//	XBUFFERBITS::SkipU
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/04/2016 16:11:28
//	
//  @param				n : 
*/
/*-----------------------------------------------------------------*/
void XBUFFERBITS::SkipU(XDWORD n)
{
	XDWORD i;
  for(i = 0; i < n; i++) 
		{
			SkipU1();
		}
}





/*-------------------------------------------------------------------
//	XBUFFERBITS::PeekU
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/04/2016 16:11:36
//	
//	@return 			XDWORD : 
//
//  @param				n : 
*/
/*-----------------------------------------------------------------*/
XDWORD XBUFFERBITS::PeekU(XDWORD n)
{
	XBYTE* orig_byte = p;
	XDWORD origin_bits_left = bits_left;

	XDWORD r = 0;

	r = ReadU(n);

	p = orig_byte;
	bits_left = origin_bits_left;

	return r;
}





/*-------------------------------------------------------------------
//	XBUFFERBITS::ReadU
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/04/2016 16:11:44
//	
//	@return 			XDWORD : 
//
//  @param				n : 
*/
/*-----------------------------------------------------------------*/
XDWORD XBUFFERBITS::ReadU(XDWORD n)
{
	XDWORD r = 0;
  XDWORD i;
  for (i = 0; i < n; i++)
		{
			r |= ( ReadU1() << ( n - i - 1 ) );
		}

  return r;
}





/*-------------------------------------------------------------------
//	XBUFFERBITS::ReadUE
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/04/2016 16:11:51
//	
//	@return 			XDWORD : 
//
*/
/*-----------------------------------------------------------------*/
XDWORD XBUFFERBITS::ReadUE()
{
	XDWORD r = 0;
  int i = 0;

  while( (ReadU1() == 0) && (i < 32) && (!IsEOF()) )
		{
			i++;
		}

  r = ReadU(i);
  r += (1 << i) - 1;

  return r;
}





/*-------------------------------------------------------------------
//	XBUFFERBITS::ReadSE
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/04/2016 16:11:59
//	
//	@return 			int : 
//
*/
/*-----------------------------------------------------------------*/
int XBUFFERBITS::ReadSE()
{
	int r = ReadUE();
  if (r & 0x01)
		{
			r = (r+1)/2;
		}
  else
		{
			r = -(r/2);
		}

  return r;
}



/*-------------------------------------------------------------------
//	XBUFFERBITS::WriteU1
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/04/2016 16:27:27
//	
//  @param				v : 
*/
/*-----------------------------------------------------------------*/
void XBUFFERBITS::WriteU1(XDWORD v)
{
	bits_left--;

  if(! IsEOF())
	  {
			// FIXME this is slow, but we must clear bit first
			// is it better to memset(0) the whole buffer during bs_init() instead? 
			// if we don't do either, we introduce pretty nasty bugs
			(*(p)) &= ~(0x01 << bits_left);
			(*(p)) |= ((v & 0x01) << bits_left);
		}

  if(bits_left == 0)
		{
			p++;
			bits_left = 8;
		}
}





/*-------------------------------------------------------------------
//	XBUFFERBITS::WriteU
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/04/2016 16:27:35
//	
//  @param				n : 
//  @param				v : 
*/
/*-----------------------------------------------------------------*/
void XBUFFERBITS::WriteU(XDWORD n, XDWORD v)
{
	XDWORD i;
  for(i = 0; i < n; i++)
		{
			WriteU1((v >> ( n - i - 1 ))&0x01);
		}
}





/*-------------------------------------------------------------------
//	XBUFFERBITS::WriteUE
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/04/2016 16:27:42
//	
//  @param				v : 
*/
/*-----------------------------------------------------------------*/
void XBUFFERBITS::WriteUE(XDWORD v)
{
	static const int len_table[256] =
  {
		1,
		1,
		2,2,
		3,3,3,3,
		4,4,4,4,4,4,4,4,
		5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
		6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
		6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
		7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
		7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
		7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
		7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
		8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
		8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
		8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
		8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
		8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
		8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
		8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
		8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
  };

  XDWORD len;

  if (v == 0)
		{
      WriteU1(1);
		}
  else
		{
			v++;

			if (v >= 0x01000000)
				{
					len = 24 + len_table[ v >> 24 ];
				}
			else if(v >= 0x00010000)
				{
					len = 16 + len_table[ v >> 16 ];
				}
			else if(v >= 0x00000100)
				{
					len =  8 + len_table[ v >>  8 ];
				}
			else 
				{
					len = len_table[ v ];
				}

			WriteU(2*len-1, v);
		}
}




/*-------------------------------------------------------------------
//	XBUFFERBITS::WriteSE
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/04/2016 16:27:49
//	
//  @param				v : 
*/
/*-----------------------------------------------------------------*/
void XBUFFERBITS::WriteSE(int v)
{
	if (v <= 0)
    {
			WriteUE(-v*2);
    }
    else
    {
			WriteUE(v*2 - 1);
    }
}

