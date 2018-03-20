/*------------------------------------------------------------------------------------------
//	XMPINTEGER.CPP
//	
//	eXtended Multi-Precision Integer class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 25/02/2014 14:29:24
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>

#include "XMPInteger.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	

static const int SmallPrime[] =	{	  3,    5,    7,   11,   13,   17,   19,   23,	29,   31,   37,   41,   43,   47,   53,   59,
																	 61,   67,   71,   73,   79,   83,   89,   97,	101,  103,  107,  109,  113,  127,  131,  137,
																	139,  149,  151,  157,  163,  167,  173,  179,	181,  191,  193,  197,  199,  211,  223,  227,
																	229,  233,  239,  241,  251,  257,  263,  269,	271,  277,  281,  283,  293,  307,  311,  313,
																	317,  331,  337,  347,  349,  353,  359,  367,	373,  379,  383,  389,  397,  401,  409,  419,
																	421,  431,  433,  439,  443,  449,  457,  461,	463,  467,  479,  487,  491,  499,  503,  509,
																	521,  523,  541,  547,  557,  563,  569,  571,	577,  587,  593,  599,  601,  607,  613,  617,
																	619,  631,  641,  643,  647,  653,  659,  661,	673,  677,  683,  691,  701,  709,  719,  727,
																	733,  739,  743,  751,  757,  761,  769,  773,	787,  797,  809,  811,  821,  823,  827,  829,
																	839,  853,  857,  859,  863,  877,  881,  883,	887,  907,  911,  919,  929,  937,  941,  947,
																	953,  967,  971,  977,  983,  991,  997, -103
																};



	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  XMPINTEGER::Ini
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			25/02/2014 15:33:24
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool XMPINTEGER::Ini()
{    
	sign		= 1;
	nlimbs  = 0;
	limbs   = NULL;

	return true;
}

		

/*-------------------------------------------------------------------
//  XMPINTEGER::Grow
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			25/02/2014 16:06:24
//	
//	@return 			bool : 
//	@param				nblimbs : 
*/
/*-----------------------------------------------------------------*/
bool XMPINTEGER::Grow(XDWORD nblimbs)
{
	XLIMB* newlimbs;

  if(nblimbs >  XMPINTEGER_MAXLIMBS) return false;
  
  if(GetNLimbs() < nblimbs)
    {
			newlimbs = (XLIMB*) new XBYTE[nblimbs * XMPINTEGER_CHARSINLIMB];
      if(!newlimbs) return false;

      memset(newlimbs, 0, nblimbs * XMPINTEGER_CHARSINLIMB );

      if(limbs)
        {
          memcpy(newlimbs, limbs, nlimbs * XMPINTEGER_CHARSINLIMB);
          memset(limbs, 0, nlimbs * XMPINTEGER_CHARSINLIMB);
          delete [] limbs;
        }

      nlimbs = nblimbs;
      limbs = newlimbs;
    } 

  return true;
}




/*-------------------------------------------------------------------
//  XMPINTEGER::Shrink
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			25/02/2014 16:21:14
//	
//	@return 			bool : 
//	@param				nblimbs : 
*/
/*-----------------------------------------------------------------*/
bool XMPINTEGER::Shrink(XDWORD nblimbs)
{	
	XLIMB*  newlimbs;
  XDWORD	c;

  // Actually resize up in this case 
	if( nlimbs <= nblimbs)  return Grow(nblimbs);

  for(c=nlimbs-1; c>0; c-- )
		{
      if(limbs[c]) break;
		}
		
	c++;

	if(c<nblimbs ) c = nblimbs; 

	newlimbs = (XLIMB*)new XBYTE[ c * XMPINTEGER_CHARSINLIMB];
	if(newlimbs) return false;
      
	memset(newlimbs, 0, (c * XMPINTEGER_CHARSINLIMB));

  if(limbs)
	  {
		  memcpy(newlimbs, limbs, (c * XMPINTEGER_CHARSINLIMB));
	    memset(limbs, 0, (nlimbs * XMPINTEGER_CHARSINLIMB));
	    delete [] limbs;
	  }

	nlimbs = c;
	limbs = newlimbs;

	return true;
}




/*-------------------------------------------------------------------
//  XMPINTEGER::CopyFrom
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			25/02/2014 17:41:57
//	
//	@return 			bool : 
//	@param				xmpinteger : 
*/
/*-----------------------------------------------------------------*/
bool XMPINTEGER::CopyFrom(XMPINTEGER* xmpinteger)
{
	XDWORD c;

	if(this == xmpinteger) return true;

	if(!xmpinteger->GetLimbs())
		{
			End();
			return true;
		}

	for(c=xmpinteger->GetNLimbs()- 1; c>0; c--)
    {
			if(xmpinteger->GetLimbs()[c] != 0 ) break;
		}
	
	c++;

	this->SetSign(xmpinteger->GetSign());

	Grow(c);

	memset(GetLimbs(), 0, (GetNLimbs() * XMPINTEGER_CHARSINLIMB));
	memcpy( GetLimbs(), xmpinteger->GetLimbs(), (c * XMPINTEGER_CHARSINLIMB));

	return true;
}




/*-------------------------------------------------------------------
//  XMPINTEGER::Swap
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			25/02/2014 17:56:17
//	
//	@return 			void : 
//	@param				xmpinteger : 
*/
/*-----------------------------------------------------------------*/
void XMPINTEGER::Swap(XMPINTEGER* xmpinteger)
{
	XMPINTEGER xmpitempo;

	xmpitempo.Ini();

	xmpitempo.CopyFrom(this);
	
	CopyFrom(xmpinteger);

	xmpinteger->CopyFrom(&xmpitempo);
}




/*-------------------------------------------------------------------
//  XMPINTEGER::LeftSet
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			26/02/2014 10:01:35
//	
//	@return 			bool : 
//	@param				z : 
*/
/*-----------------------------------------------------------------*/
bool XMPINTEGER::LeftSet(int z)
{
	if(!Grow(1)) return false;

  memset(GetLimbs(), 0, (GetNLimbs() * XMPINTEGER_CHARSINLIMB));

  limbs[0] = (XLIMB)(( z < 0 ) ?-z : z);
  sign		 = (( z < 0 ) ? -1 : 1);

  return true;
}




/*-------------------------------------------------------------------
//  XMPINTEGER::GetBit
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			26/02/2014 11:26:47
//	
//	@return 			int : 
//	@param				position : 
*/
/*-----------------------------------------------------------------*/
int XMPINTEGER::GetBit(XDWORD position)
{
	if(nlimbs * XMPINTEGER_BITSINLIMB <= position) return 0;
  
  return ( limbs[position / XMPINTEGER_BITSINLIMB] >> (position % XMPINTEGER_BITSINLIMB) ) & 0x01;
}




/*-------------------------------------------------------------------
//  XMPINTEGER::SetBit
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			26/02/2014 11:28:30
//	
//	@return 			int : 
//	@param				position : 
//  @param				value : 
*/
/*-----------------------------------------------------------------*/
bool XMPINTEGER::SetBit(XDWORD position, XBYTE value)
{
	XDWORD off = position / XMPINTEGER_BITSINLIMB;
  XDWORD idx = position % XMPINTEGER_BITSINLIMB;

  if((value != 0) && (value != 1)) return false;
        
  if(nlimbs * XMPINTEGER_BITSINLIMB <= position)
    {
      if(value == 0) return 0;
      
      if(!Grow(off + 1)) return false;
    }

  limbs[off] &= ~((XLIMB)(0x01)  << idx);
  limbs[off] |=  ((XLIMB)(value) << idx);
    
  return true;
}




/*-------------------------------------------------------------------
//  XMPINTEGER::GetLSB
*/ 
/**
//	Return the number of least significant bits	
//	
//	@author				Abraham J. Velez
//	@version			26/02/2014 11:43:54
//	
//	@return 			int : 
//	*/
/*-----------------------------------------------------------------*/
int XMPINTEGER::GetLSB()
{
  XDWORD i; 
	XDWORD j;
	XDWORD count = 0;

  for(i=0; i<nlimbs; i++)
		{
      for( j = 0; j<XMPINTEGER_BITSINLIMB; j++, count++ )
				{
          if( ( (limbs[i] >> j ) & 1 ) != 0 ) return count;
				}
		}

  return 0 ;
}



/*-------------------------------------------------------------------
//  XMPINTEGER::GetMSB
*/ 
/**
//	Return the number of most significant bits
//	
//	@author				Abraham J. Velez
//	@version			26/02/2014 11:44:40
//	
//	@return 			int : 
//	*/
/*-----------------------------------------------------------------*/
int XMPINTEGER::GetMSB()
{
	XDWORD i;
	XDWORD j;

  for(i=nlimbs-1; i>0; i--)
		{
			if(limbs[i] != 0) break;
		}

  for(j = XMPINTEGER_BITSINLIMB; j>0; j--)
		{
			if(((limbs[i] >> ( j - 1 ) ) & 1 ) != 0 ) break;				
		}

   return ((i * XMPINTEGER_BITSINLIMB) + j);
}




/*-------------------------------------------------------------------
//  XMPINTEGER::GetSize
*/ 
/**
//	Return the total size in bytes
//	
//	@author				Abraham J. Velez
//	@version			26/02/2014 12:04:35
//	
//	@return 			XDWORD : 
//	*/
/*-----------------------------------------------------------------*/
XDWORD XMPINTEGER::GetSize()
{
  return ((GetMSB()+7) >> 3);
}




/*-------------------------------------------------------------------
//  XMPINTEGER::ImportFromBinary
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			26/02/2014 12:19:00
//	
//	@return 			int : 
//	@param				buffer : 
//  @param				size : 
*/
/*-----------------------------------------------------------------*/
int XMPINTEGER::ImportFromBinary(XBYTE* buffer, XDWORD size)
{
	XDWORD i;
	XDWORD j;
	XDWORD n;

  for(n=0; n<size; n++)
		{
      if(buffer[n] != 0) break;
		}

  if(Grow(XMPINTEGER_CHARSTOLIMBS(size- n)))
		{
			if(!LeftSet(0)) return false;

		} else return false;

  for(i=size, j=0; i>nlimbs; i--, j++ )
    {
			limbs[j / XMPINTEGER_CHARSINLIMB] |= ((XLIMB) buffer[i-1]) << ((j % XMPINTEGER_CHARSINLIMB) << 3);
		}

  return true;
}




/*-------------------------------------------------------------------
//  XMPINTEGER::ExportToBinary
*/ 
/**
//	Export X into unsigned binary data, big endian
//	
//	@author				Abraham J. Velez
//	@version			26/02/2014 12:19:16
//	
//	@return 			int : 
//	@param				buffer : 
//  @param				size : 
*/
/*-----------------------------------------------------------------*/
int XMPINTEGER::ExportToBinary(XBYTE* buffer, XDWORD size)
{
	XDWORD i;
	XDWORD j;
	XDWORD n;

  n = GetSize();

  if(size < n) return false;
    
  memset((void*)buffer, 0, size);

  for( i = size - 1, j = 0; n > 0; i--, j++, n-- )
		{
			buffer[i] = (XBYTE)( limbs[j / XMPINTEGER_CHARSINLIMB] >> ((j % XMPINTEGER_CHARSINLIMB) << 3));
		}

  return true;
}



/*-------------------------------------------------------------------
//  XMPINTEGER::LeftShift
*/ 
/**
//	Left-shift: X <<= count
//	
//	@author				Abraham J. Velez
//	@version			26/02/2014 12:34:15
//	
//	@return 			bool : 
//	@param				count : 
*/
/*-----------------------------------------------------------------*/
bool XMPINTEGER::LeftShift(XDWORD count)
{   
	XDWORD i;
	XDWORD v0;
	XDWORD t1;
  XLIMB  r0 = 0;
	XLIMB  r1;

  v0 = count / (XMPINTEGER_BITSINLIMB);
  t1 = count & (XMPINTEGER_BITSINLIMB - 1);

  i = GetMSB() + count;

  if(nlimbs * XMPINTEGER_BITSINLIMB < i)
		{
      if(!Grow(XMPINTEGER_BITSTOLIMBS(i))) return false;
		}

 
  // shift by count / limb_size
  if(v0>0)
    {
			for( i=nlimbs; i>v0; i--)
				{
					limbs[i - 1] = limbs[i - v0 - 1];
				}

      for( ; i > 0; i-- )
				{
					limbs[i - 1] = 0;
				}
    }

  // shift by count % limb_size
  if(t1 > 0)
    {
			for( i = v0; i < nlimbs; i++ )
        {
          r1 = limbs[i] >> (XMPINTEGER_BITSINLIMB - t1);
          limbs[i] <<= t1;
          limbs[i] |= r0;
          r0 = r1;
        }
    }

  return true;
}




/*-------------------------------------------------------------------
//  XMPINTEGER::RightShift
*/ 
/**
//	Right-shift: X >>= count
//	
//	@author				Abraham J. Velez
//	@version			26/02/2014 12:34:40
//	
//	@return 			bool : 
//	@param				count : 
*/
/*-----------------------------------------------------------------*/
bool XMPINTEGER::RightShift(XDWORD count)
{
	XDWORD i;
	XDWORD v0;
	XDWORD v1;    
	XLIMB  r0 = 0;
	XLIMB  r1;

  v0 = count /  XMPINTEGER_BITSINLIMB;
  v1 = count & (XMPINTEGER_BITSINLIMB - 1);

  if((v0 > nlimbs) || (v0 == nlimbs && v1 > 0)) return LeftSet(0);

  // shift by count / limb_size
  if(v0>0)
    {
			for(i=0; i<nlimbs-v0; i++)
				{
					limbs[i] = limbs[i + v0];
				}

      for( ; i<nlimbs; i++)
				{
          limbs[i] = 0;
				}
    }

    
  // shift by count % limb_size  
  if(v1>0)
    {
      for(i=nlimbs; i>0; i--)
        {
				  r1 = limbs[i - 1] << (XMPINTEGER_BITSINLIMB - v1);
          limbs[i - 1] >>= v1;
          limbs[i - 1] |= r0;
          r0 = r1;
        }
    }

	return true;
}





/*-------------------------------------------------------------------
//  XMPINTEGER::CompareABSValues
*/ 
/**
//	Compare unsigned values
//	
//	@author				Abraham J. Velez
//	@version			26/02/2014 12:59:01
//	
//	@return 			int : 
//	@param				xmpinteger : 
*/
/*-----------------------------------------------------------------*/
int XMPINTEGER::CompareABSValues(XMPINTEGER& xmpinteger)
{
	XDWORD i;
	XDWORD j;

  for(i=nlimbs; i>0; i--)
		{
      if(limbs[i - 1] != 0) break;
		}

	for(j = xmpinteger.GetNLimbs(); j>0; j--)
		{
			if(xmpinteger.GetLimbs()[j - 1] != 0) break;
		}

  if((i == 0) && (j == 0)) return 0;

  if(i > j) return  1;
  if(j > i) return -1;

  for( ; i>0; i--)
    {
			if(limbs[i - 1] > xmpinteger.GetLimbs()[i - 1] ) return  1;
      if(limbs[i - 1] < xmpinteger.GetLimbs()[i - 1] ) return -1;
    }

  return 0;
}




/*-------------------------------------------------------------------
//  XMPINTEGER::CompareSignedValues
*/ 
/**
//	Compare signed values	
//	
//	@author				Abraham J. Velez
//	@version			26/02/2014 13:06:51
//	
//	@return 			int : 
//	@param				xmpinteger : 
*/
/*-----------------------------------------------------------------*/
int XMPINTEGER::CompareSignedValues(XMPINTEGER& xmpinteger)
{
	XDWORD i;
	XDWORD j;

  for(i=nlimbs; i>0; i--)
		{
      if( limbs[i - 1] != 0) break;
		}

	for(j=xmpinteger.GetNLimbs(); j>0; j--)
		{
			if(xmpinteger.GetLimbs()[j - 1] != 0) break;
		}

	if((i == 0) && (j == 0))  return 0;

  if( i > j ) return  GetSign();
  if( j > i ) return  -xmpinteger.GetSign();

  if((GetSign() > 0) && xmpinteger.GetSign() < 0 )		return  1;
  if((xmpinteger.GetSign() > 0) &&  (GetSign() < 0))	return -1;

  for( ; i > 0; i-- )
    {
      if(limbs[i - 1] > xmpinteger.GetLimbs()[i - 1] ) return  GetSign();
      if(limbs[i - 1] < xmpinteger.GetLimbs()[i - 1] ) return -GetSign();
    }

  return 0;
}



/*-------------------------------------------------------------------
//  XMPINTEGER::CompareSignedValues
*/ 
/**
//	Compare signed values
//	
//	@author				Abraham J. Velez
//	@version			26/02/2014 13:14:13
//	
//	@return 			int : 
//	@param				integer : 
*/
/*-----------------------------------------------------------------*/
int XMPINTEGER::CompareSignedValues(int integer)
{
 XMPINTEGER xmpinteger;
 XLIMB*			pointer = new XLIMB();

 (*pointer)  = ( integer < 0 ) ? -integer : integer;
 
 xmpinteger.SetSign((integer < 0 ) ? -1 : 1);
 
 xmpinteger.SetNLimbs(1);
 xmpinteger.SetLimbs(pointer);
	
 return CompareSignedValues(xmpinteger);
}




/*-------------------------------------------------------------------
//  XMPINTEGER::AdditionABS
*/ 
/**
//	Unsigned addition: X = |xmpinteger1| + |xmpinteger2|  (HAC 14.7)
//	
//	@author				Abraham J. Velez
//	@version			26/02/2014 13:51:53
//	
//	@return 			bool : 
//	@param				xmpinteger1 : 
//  @param				xmpinteger2 : 
*/
/*-----------------------------------------------------------------*/
bool XMPINTEGER::AdditionABS(XMPINTEGER* xmpinteger1, XMPINTEGER* xmpinteger2)
{
	int		 i;
	int		 j;
  XLIMB* o;
	XLIMB* p;
	XLIMB  c;

  if( this == xmpinteger2)
    {
      XMPINTEGER* tempoxmpinteger = xmpinteger1; 
				
			xmpinteger1 = this; 
			xmpinteger2 = tempoxmpinteger;
    }


	if(this != xmpinteger1) CopyFrom(xmpinteger1);
	 
  // X should always be positive as a result of unsigned additions.
     
  SetSign(1);

  for(j=xmpinteger2->GetNLimbs(); j>0; j--)
		{
			if(xmpinteger2->GetLimbs()[j - 1] != 0 ) break;
		}

	if(!Grow(j)) return false;

	o = xmpinteger2->GetLimbs(); 
	p = GetLimbs(); 
	c = 0;

  for(i=0; i<j; i++, o++, p++ )
    {
      *p +=  c; c  = ( *p <  c );
      *p += *o; c += ( *p < *o );
    }

  while(c != 0)
    {
      if( i >= (int)GetNLimbs())
        {
          if(!Grow(i + 1)) return false;
          p = GetLimbs() + i;
        }

      *p += c; c = ( *p < c ); i++; p++;
    }

  return true;
}



/*-------------------------------------------------------------------
//  XMPINTEGER::AddABS
*/ 
/**
//	Unsigned subtraction: X = |xmpinteger1| - |xmpinteger2|  (HAC 14.9)
//	
//	@author				Abraham J. Velez
//	@version			26/02/2014 13:45:13
//	
//	@return 			bool : 
//	@param				xmpinteger1 : 
//  @param				xmpinteger2 : 
*/
/*-----------------------------------------------------------------*/
bool XMPINTEGER::SubtractionABS(XMPINTEGER* xmpinteger1, XMPINTEGER* xmpinteger2)
{
  XMPINTEGER	tb;
  int					n;

  if(xmpinteger1->CompareABSValues((*xmpinteger2)) < 0 ) return false;

	tb.Ini();

  if(this == xmpinteger2)
    {
			tb.CopyFrom(xmpinteger2);
      xmpinteger2 = &tb;
    }

  if(this != xmpinteger1) CopyFrom(xmpinteger1);
	    
  // X should always be positive as a result of unsigned subtractions.    
	SetSign(1);
  
  for( n = xmpinteger2->GetNLimbs(); n > 0; n-- )
		{
			if(xmpinteger2->GetLimbs()[n - 1] != 0) break;
		}

  SubtractionHelper( n, xmpinteger2->GetLimbs(), GetLimbs());

	tb.End();

  return true;
}




/*-------------------------------------------------------------------
//  XMPINTEGER::AdditionSigned
*/ 
/**
//	Signed addition: X = xmpinteger1 + xmpinteger2
//	
//	@author				Abraham J. Velez
//	@version			26/02/2014 13:59:07
//	
//	@return 			bool : 
//	@param				xmpinteger1 : 
//  @param				xmpinteger2 : 
*/
/*-----------------------------------------------------------------*/
bool XMPINTEGER::AdditionSigned(XMPINTEGER* xmpinteger1, XMPINTEGER* xmpinteger2)
{
	int tempsign = xmpinteger1->GetSign();

  if((xmpinteger1->GetSign() * xmpinteger2->GetSign()) < 0)
    {
      if(xmpinteger1->CompareABSValues((*xmpinteger2)) >= 0)
        {
          if(!SubtractionABS(xmpinteger1, xmpinteger2)) return false;
          SetSign(tempsign);
        }
       else
        {
					if(!SubtractionABS(xmpinteger2, xmpinteger1)) return false; 
				  SetSign(-tempsign);          
        }
    }
   else
    {
			if(!AdditionABS(xmpinteger1, xmpinteger2)) return false;
      SetSign(tempsign);          
    }

	return true;
}

 


/*-------------------------------------------------------------------
//  XMPINTEGER::SubtractionSigned
*/ 
/**
//	Signed subtraction: X = xmpinteger1 - xmpinteger2
//	
//	@author				Abraham J. Velez
//	@version			26/02/2014 14:08:08
//	
//	@return 			bool : 
//	@param				xmpinteger1 : 
//  @param				xmpinteger2 : 
*/
/*-----------------------------------------------------------------*/
bool XMPINTEGER::SubtractionSigned(XMPINTEGER* xmpinteger1, XMPINTEGER* xmpinteger2)
{  
	int tempsign = xmpinteger1->GetSign();

  if((xmpinteger1->GetSign() * xmpinteger2->GetSign()) > 0)
    {
			if(xmpinteger1->CompareABSValues((*xmpinteger2)) >= 0)
        {
          if(!SubtractionABS(xmpinteger1, xmpinteger2)) return false;
          SetSign(tempsign);
        }
       else
        {
					if(!SubtractionABS(xmpinteger2, xmpinteger1)) return false; 
				  SetSign(-tempsign);          
        }
    }
   else
    {
      if(!AdditionABS(xmpinteger1, xmpinteger2)) return false;
      SetSign(tempsign);       
    }

	return true;
}





/*-------------------------------------------------------------------
//  XMPINTEGER::AdditionSigned
*/ 
/**
//	Signed addition: X = xmpinteger + integer
//	
//	@author				Abraham J. Velez
//	@version			26/02/2014 14:16:32
//	
//	@return 			bool : 
//	@param				xmpinteger : 
//  @param				integer : 
*/
/*-----------------------------------------------------------------*/
bool XMPINTEGER::AdditionSigned(XMPINTEGER* xmpinteger, int integer)
{
	XMPINTEGER _xmpinteger2;
	XLIMB*			pointer = new XLIMB();

  (*pointer) = ( integer < 0 ) ? -integer : integer;

	_xmpinteger2.SetSign(( integer < 0 ) ? -1 : 1);
  _xmpinteger2.SetNLimbs(1);
  _xmpinteger2.SetLimbs(pointer);

  return AdditionSigned(xmpinteger, &_xmpinteger2);
}



/*-------------------------------------------------------------------
//  XMPINTEGER::SubtractionSigned
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			07/03/2014 17:16:46
//	
//	@return 			bool : 
//	@param				xmpinteger : 
//  @param				integer : 
*/
/*-----------------------------------------------------------------*/
bool XMPINTEGER::SubtractionSigned(XMPINTEGER* xmpinteger, int integer)
{
  XMPINTEGER	_xmpinteger2;
  XLIMB*			pointer = new XLIMB();

 (*pointer) = (integer < 0 ) ? -integer : integer;

  _xmpinteger2.SetSign(( integer < 0 ) ? -1 : 1);
  _xmpinteger2.SetNLimbs(1);
  _xmpinteger2.SetLimbs(pointer);

  return(SubtractionSigned(xmpinteger, &_xmpinteger2));
}




/*-------------------------------------------------------------------
//  XMPINTEGER::Multiplication
*/ 
/**
//	Baseline multiplication: X = xmpinteger1 * xmpinteger2  (HAC 14.12)
//	
//	@author				Abraham J. Velez
//	@version			26/02/2014 17:42:14
//	
//	@return 			bool : 
//	@param				xmpinteger1 : 
//  @param				xmpinteger2 : 
*/
/*-----------------------------------------------------------------*/
bool XMPINTEGER::Multiplication(XMPINTEGER* xmpinteger1, XMPINTEGER* xmpinteger2)
{
	int					i;
	int					j;
	XMPINTEGER	TA;
	XMPINTEGER	TB;
	bool				status = true;

	TA.Ini();
	TB.Ini();
  
	if(this == xmpinteger1) 
		{ 
			TA.CopyFrom(xmpinteger1); 
			xmpinteger1 = &TA; 
		}
		
  if(this == xmpinteger2) 
		{ 
			TB.CopyFrom(xmpinteger2);
			xmpinteger2 = &TB; 
		}

  for(i=xmpinteger1->GetNLimbs(); i>0; i--)
    {
			if(xmpinteger1->GetLimbs()[i - 1] != 0) break;
		}

  for(j=xmpinteger2->GetNLimbs(); j>0; j--)
		{
      if(xmpinteger2->GetLimbs()[j - 1] != 0) break;
		}

   if(!Grow(i + j)) return false;
	 if(!LeftSet(0))	return false;

	for(i++; j > 0; j--)
	  {
			MultiplicationHelper( i - 1, xmpinteger1->GetLimbs(), GetLimbs() + j - 1, xmpinteger2->GetLimbs()[j - 1]);
		}

	SetSign(xmpinteger1->GetSign() * xmpinteger2->GetSign());

	TA.End();
	TB.End();
	
	return true;
}



/*-------------------------------------------------------------------
//  XMPINTEGER::Multiplication
*/ 
/**
//	Baseline multiplication: X = xmpinteger * integer
//	
//	@author				Abraham J. Velez
//	@version			26/02/2014 17:37:20
//	
//	@return 			bool : 
//	@param				xmpinteger : 
//  @param				integer : 
*/
/*-----------------------------------------------------------------*/
bool XMPINTEGER::Multiplication(XMPINTEGER* xmpinteger, int integer)
{
  XMPINTEGER _xmpinteger;
  XLIMB*			pointer = new XLIMB();

  _xmpinteger.SetSign(1);
  _xmpinteger.SetNLimbs(1);
  _xmpinteger.SetLimbs(pointer);
  (*pointer) = integer;

  return Multiplication(xmpinteger, &_xmpinteger);
}




/*-------------------------------------------------------------------
//  XMPINTEGER::Division
*/ 
/**
//	Division by mpi: A = Q * B + R  (HAC 14.20)
//	
//	@author				Abraham J. Velez
//	@version			27/02/2014 16:18:38
//	
//	@return 			bool : 
//	@param				xmpintegerQ : 
//  @param				xmpintegerR : 
//  @param				xmpintegerA : 
//  @param				xmpintegerB : 
*/
/*-----------------------------------------------------------------*/
bool XMPINTEGER::Division(XMPINTEGER* xmpintegerQ, XMPINTEGER* xmpintegerR, XMPINTEGER* xmpintegerA, XMPINTEGER* xmpintegerB)
{
	int i;
	int n;
	int t;
	int k;
 
	XMPINTEGER X;
	XMPINTEGER Y;
	XMPINTEGER Z;
	XMPINTEGER T1;
	XMPINTEGER T2;

  if(!xmpintegerB->CompareSignedValues(0)) return false;
  
  X.Ini();
	Y.Ini(); 
	Z.Ini();

  T1.Ini();  
	T2.Ini(); 

  if(xmpintegerA->CompareABSValues((*xmpintegerB)) < 0 )
    {
			if( xmpintegerQ != NULL ) 
				{
					if(!xmpintegerQ->LeftSet(0)) return false;
				}

			if( xmpintegerR != NULL )
				{
					if(!xmpintegerR->CopyFrom(xmpintegerA)) return false;
				}
      
			return true;
    }

	X.CopyFrom(xmpintegerA);
	Y.CopyFrom(xmpintegerB);
   
	X.SetSign(1); 
	Y.SetSign(1);

  if(!Z.Grow(xmpintegerA->GetNLimbs() + 2 )) return false;
	if(!Z.LeftSet(0)) return false;  
	if(!T1.Grow(2))		return false;
  if(!T2.Grow(3))		return false;

  k = (Y.GetMSB() % XMPINTEGER_BITSINLIMB);
  if(k < (XMPINTEGER_BITSINLIMB - 1))
    {
			k = XMPINTEGER_BITSINLIMB - 1 - k;

      if(!X.LeftShift(k)) return false;
      if(!Y.LeftShift(k)) return false;

	  } else k = 0;

  n = X.GetNLimbs() - 1;
  t = Y.GetNLimbs() - 1;
  if(!Y.LeftShift(XMPINTEGER_BITSINLIMB * (n - t))) return false;

	while(X.CompareSignedValues(Y) >= 0)
    {
		   Z.GetLimbs()[n - t]++;
			 if(!X.SubtractionSigned(&X, &Y)) return true;
    }
    
	Y.RightShift(XMPINTEGER_BITSINLIMB * (n - t));

  for( i = n; i > t ; i-- )
    {
      if( X.GetLimbs()[i] >= Y.GetLimbs()[t] ) 
				{
					Z.GetLimbs()[i - t - 1] = ~0;
				}
       else
        { 
					XUDBL r;

          r  = (XUDBL) X.GetLimbs()[i] <<  XMPINTEGER_BITSINLIMB;
          r |= (XUDBL) X.GetLimbs()[i - 1];
          r /= Y.GetLimbs()[t];

          if(r>((XUDBL) 1 <<  XMPINTEGER_BITSINLIMB)-1)  r = ((XUDBL) 1 <<  XMPINTEGER_BITSINLIMB) - 1;

					Z.GetLimbs()[i - t - 1] = (XLIMB) r;					
        }

      Z.GetLimbs()[i - t - 1]++;

      do{	Z.GetLimbs()[i - t - 1]--;

          if(!T1.LeftSet(0)) return false;

          T1.GetLimbs()[0] = (t < 1) ? 0 : Y.GetLimbs()[t - 1];
          T1.GetLimbs()[1] = Y.GetLimbs()[t];
          
					if(!T1.Multiplication(&T1, Z.GetLimbs()[i - t - 1])) return false;

          if(!T2.LeftSet(0)) return false;

          T2.GetLimbs()[0] = (i < 2) ? 0 : X.GetLimbs()[i - 2];
          T2.GetLimbs()[1] = (i < 1) ? 0 : X.GetLimbs()[i - 1];
          T2.GetLimbs()[2] = X.GetLimbs()[i];

        } while(T1.CompareSignedValues(T2) > 0 );

        if(!T1.Multiplication(&Y, Z.GetLimbs()[i - t - 1])) return false;

        if(!T1.LeftShift(XMPINTEGER_BITSINLIMB * (i - t - 1))) return false;

        if(!X.SubtractionSigned(&X, &T1 )) return false;

        if(X.CompareSignedValues(0) < 0 )
					{
            if(!T1.CopyFrom(&Y)) return false;
            if(!T1.LeftShift(XMPINTEGER_BITSINLIMB * (i - t - 1))) return false;

            if(!X.AdditionSigned(&X, &T1)) return false;

            Z.GetLimbs()[i - t - 1]--;
					}
    }

   if(xmpintegerQ != NULL )
    {
	    if(!xmpintegerQ->CopyFrom(&Z)) return false;        
			xmpintegerQ->SetSign(xmpintegerA->GetSign() * xmpintegerB->GetSign());
    }

   if(xmpintegerR != NULL )
    {
			if(!X.RightShift(k)) return false;
      X.SetSign(xmpintegerA->GetSign());

      if(!xmpintegerR->CopyFrom(&X)) return false;

      if(xmpintegerR->CompareSignedValues(0)==0)  xmpintegerR->SetSign(1);
    }

	X.End(); 
	Y.End(); 
	Z.End();
	T1.End();
	T2.End();

  return true;
}




/*-------------------------------------------------------------------
//  XMPINTEGER::Division
*/ 
/**
//	Division by int: A = Q * b + R
//	
//	@author				Abraham J. Velez
//	@version			27/02/2014 17:00:16
//	
//	@return 			bool : 
//	@param				xmpintegerQ : 
//  @param				xmpintegerR : 
//  @param				xmpintegerA : 
//  @param				integerb : 
*/
/*-----------------------------------------------------------------*/
bool XMPINTEGER::Division(XMPINTEGER* xmpintegerQ, XMPINTEGER* xmpintegerR, XMPINTEGER* xmpintegerA, int integerb)
{
  XMPINTEGER _B;
  XLIMB*			pointer = new XLIMB();

  (*pointer) = ( integerb < 0 ) ? -integerb : integerb;
  _B.SetSign(( integerb < 0 ) ? -1 : 1);
  _B.SetNLimbs(1);
  _B.SetLimbs(pointer);

	return Division( xmpintegerQ, xmpintegerR, xmpintegerA, &_B );
}





/*-------------------------------------------------------------------
//  XMPINTEGER::Module
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			27/02/2014 17:19:18
//	
//	@return 			bool : 
//	@param				xmpintegerR : 
//  @param				xmpintegerA : 
//  @param				xmpintegerB : 
*/
/*-----------------------------------------------------------------*/
bool XMPINTEGER::Module(XMPINTEGER* xmpintegerR, XMPINTEGER* xmpintegerA, XMPINTEGER* xmpintegerB)
{ 
	if(xmpintegerB->CompareSignedValues(0) < 0) return false;
    
  if(!Division( NULL, xmpintegerR, xmpintegerA, xmpintegerB )) return false;

  while(xmpintegerR->CompareSignedValues(0) < 0)
		{
			if(!xmpintegerR->AdditionSigned(xmpintegerR, xmpintegerB)) return false;
		}

	while(xmpintegerR->CompareSignedValues((*xmpintegerB)) >= 0 )
		{
      if(!xmpintegerR->SubtractionSigned(xmpintegerR, xmpintegerB)) return false;
		}

  return true;
}



/*-------------------------------------------------------------------
//  XMPINTEGER::Module
*/ 
/**
//	Modulo: R = A mod b
//	
//	@author				Abraham J. Velez
//	@version			27/02/2014 17:21:23
//	
//	@return 			bool : 
//	@param				xmpintegerR : 
//  @param				xmpintegerA : 
//  @param				integerb : 
*/
/*-----------------------------------------------------------------*/
bool XMPINTEGER::Module(XLIMB* r, XMPINTEGER* xmpintegerA, int integerb)
{
	XLIMB  x, y, z;
	XDWORD i;
  
  if(integerb == 0) return false;  
  if(integerb  < 0) return false;
  
  // handle trivial cases
  
  if(integerb == 1 )
    {
      (*r) = 0;
      return true;
    }

  if(integerb == 2 )
    {
      (*r) = xmpintegerA->GetLimbs()[0] & 1;
      return true;
    }

  // general case    
  for(i = xmpintegerA->GetNLimbs(), y=0; i>0; i--)
    {
		  x  = xmpintegerA->GetLimbs()[i - 1];
      y  = ( y << XMPINTEGER_HALFLIMBSIZE ) | ( x >> XMPINTEGER_HALFLIMBSIZE );
      z  = y / integerb;
      y -= z * integerb;

      x <<= XMPINTEGER_HALFLIMBSIZE;
      y  = ( y << XMPINTEGER_HALFLIMBSIZE ) | ( x >> XMPINTEGER_HALFLIMBSIZE );
      z  = y / integerb;
      y -= z * integerb;
    }

	// If A is negative, then the current y represents a negative value. Flipping it to the positive side.
    
  if( xmpintegerA->GetSign() < 0 && y != 0 ) y = (integerb - y);

  *r = y;

  return true;
}




/*-------------------------------------------------------------------
//  XMPINTEGER::MontgomeryIni
*/ 
/**
//	Fast Montgomery initialization (thanks to Tom St Denis)
//	
//	@author				Abraham J. Velez
//	@version			27/02/2014 17:33:07
//	
//	@return 			void : 
//	@param				*mm : 
//  @param				N : 
*/
/*-----------------------------------------------------------------*/
void XMPINTEGER::MontgomeryIni(XLIMB *mm, XMPINTEGER* N)
{
	XLIMB x;
	XLIMB m0 = N->GetLimbs()[0];

  x  = m0;
  x += ( ( m0 + 2 ) & 4 ) << 1;
  x *= ( 2 - ( m0 * x ) );

  if( XMPINTEGER_BITSINLIMB >= 16 ) x *= ( 2 - ( m0 * x ) );
  if( XMPINTEGER_BITSINLIMB >= 32 ) x *= ( 2 - ( m0 * x ) );
  if( XMPINTEGER_BITSINLIMB >= 64 ) x *= ( 2 - ( m0 * x ) );

  *mm = ~x + 1;
}




/*-------------------------------------------------------------------
//  XMPINTEGER::MontgomeryMultiplication
*/ 
/**
//	Montgomery multiplication: A = A * B * R^-1 mod N  (HAC 14.36)
//	
//	@author				Abraham J. Velez
//	@version			27/02/2014 17:37:16
//	
//	@return 			void : 
//	@param				A : 
//  @param				B : 
//  @param				N : 
//  @param				mm : 
//  @param				T : 
*/
/*-----------------------------------------------------------------*/
void XMPINTEGER::MontgomeryMultiplication(XMPINTEGER* A, XMPINTEGER* B, XMPINTEGER* N, XLIMB mm, XMPINTEGER* T)
{  
	XLIMB		u0, u1, *d; 
	XDWORD  i , n , m;

  memset( T->GetLimbs(), 0, T->GetNLimbs() * XMPINTEGER_CHARSINLIMB );

  d = T->GetLimbs();
  n = N->GetNLimbs();
  m = (B->GetNLimbs() < n) ? B->GetNLimbs() : n;

  for(i=0; i<n; i++)
    {
      // T = (T + u0*B + u1*N) / 2^XMPINTEGER_BITSINLIMB
        
      u0 = A->GetLimbs()[i];
      u1 = ( d[0] + u0 * B->GetLimbs()[0] ) * mm;

      MultiplicationHelper( m, B->GetLimbs(), d, u0 );
      MultiplicationHelper( n, N->GetLimbs(), d, u1 );

       *d++ = u0; 
			  d[n + 1] = 0;
    }

  memcpy(A->GetLimbs(), d, (n + 1) * XMPINTEGER_CHARSINLIMB );

  if(A->CompareABSValues((*N))>= 0)				
			   SubtractionHelper(n, N->GetLimbs(), A->GetLimbs());
		else SubtractionHelper(n, A->GetLimbs(), T->GetLimbs());/* prevent timing attacks */
}





/*-------------------------------------------------------------------
//  XMPINTEGER::MontgomeryReduction
*/ 
/**
//	Montgomery reduction: A = A * R^-1 mod N
//	
//	@author				Abraham J. Velez
//	@version			27/02/2014 17:44:43
//	
//	@return 			void : 
//	@param				A : 
//  @param				N : 
//  @param				mm : 
//  @param				T : 
*/
/*-----------------------------------------------------------------*/
void XMPINTEGER::MontgomeryReduction(XMPINTEGER* A, XMPINTEGER* N, XLIMB mm, XMPINTEGER* T)
{
	XLIMB*		 z = new XLIMB();
  XMPINTEGER U;

	(*z) = 1;

  U.SetNLimbs(1);
	U.SetSign(1);
  U.SetLimbs(z);

  MontgomeryMultiplication( A, &U, N, mm, T );
}




/*-------------------------------------------------------------------
//  XMPINTEGER::SlidingWindowExponentiation
*/ 
/**
//	Sliding-window exponentiation: X = A^E mod N  (HAC 14.85)
//	
//	@author				Abraham J. Velez
//	@version			28/02/2014 14:30:03
//	
//	@return 			bool : 
//  @param				A : 
//  @param				E : 
//  @param				N : 
//  @param				_RR : 
*/
/*-----------------------------------------------------------------*/
bool XMPINTEGER::SlidingWindowExponentiation(XMPINTEGER* A, XMPINTEGER* E, XMPINTEGER* N, XMPINTEGER* _RR)
{
	XDWORD			wbits, wsize, one = 1;
  XDWORD			i, j, nblimbs;
  XDWORD		  bufsize, nbits;
	XLIMB				ei, mm, state;
  XMPINTEGER	RR, T, W[2 << XMPINTEGER_WINDOWSIZE], Apos;
  int					neg;

	if(N->CompareSignedValues(0) < 0 || ( N->GetLimbs()[0] & 1 ) == 0 ) return false;  
  if(E->CompareSignedValues(0) < 0)																		return false;
  
  //Init temps and window size
     
  MontgomeryIni( &mm, N );
  RR.Ini();
	T.Ini();
  Apos.Ini();

  memset( W, 0, sizeof(W));

  i = E->GetMSB();

  wsize = ( i > 671 ) ? 6 : ( i > 239 ) ? 5 :
          ( i >  79 ) ? 4 : ( i >  23 ) ? 3 : 1;

  if(wsize > XMPINTEGER_WINDOWSIZE) wsize = XMPINTEGER_WINDOWSIZE;

	j = N->GetNLimbs() + 1;

	if(!Grow(j))		return false;
  if(!W[1].Grow(j)) return false;
  if(!T.Grow(j*2))	return false;

  // Compensate for negative A (and correct at the end)
     
  neg = ( A->GetSign() == -1 );
  if(neg)
    {
			if(!Apos.CopyFrom(A)) return false;
      Apos.SetSign(1);
      A = &Apos;
    }

  // If 1st call, pre-compute R^2 mod N
     
  if( _RR == NULL || _RR->GetLimbs() == NULL )
    {
			if(!RR.LeftSet(1))																						return false;
      if(!RR.LeftShift(N->GetNLimbs() * 2 * XMPINTEGER_BITSINLIMB)) return false;
      if(!RR.Module(&RR, &RR, N))																		return false;

			_RR->CopyFrom(&RR);

      if(_RR != NULL) _RR->CopyFrom(&RR);

    } else RR.CopyFrom(_RR);

    // W[1] = A * R^2 * R^-1 mod N = A * R mod N
     
  if(A->CompareSignedValues((*N)) >= 0)
		{
      if(!Module( &W[1], A, N )) return false;
		}
   else
		{
      if(!W[1].CopyFrom(A)) return false;
		}

  MontgomeryMultiplication(&W[1], &RR, N, mm, &T);

    // X = R^2 * R^-1 mod N = R mod N
     
  if(!CopyFrom(&RR)) return false;
  MontgomeryReduction(this, N, mm, &T);

  if(wsize > 1)
		{
			// W[1 << (wsize - 1)] = W[1] ^ (wsize - 1)
         
      j =  one << (wsize - 1);

			if(!W[j].Grow(N->GetNLimbs() + 1))  return false;

      if(!W[j].CopyFrom(&W[1])) return false;

      for(i = 0; i < wsize - 1; i++)
				{
          MontgomeryMultiplication( &W[j], &W[j], N, mm, &T);
				}

      // W[i] = W[i - 1] * W[1]
         
      for(i = j + 1; i < (one << wsize); i++)
				{
          if(!W[i].Grow(N->GetNLimbs() + 1)) return false;
          if(!W[i].CopyFrom(&W[i-1]))				 return false;

          MontgomeryMultiplication( &W[i], &W[1], N, mm, &T);
				}
		}

	nblimbs = E->GetNLimbs();
  bufsize = 0;
  nbits   = 0;
  wbits   = 0;
  state   = 0;

  while(1)
		{
      if(bufsize == 0)
			  {
			    if(nblimbs == 0) break;
          nblimbs--;
          bufsize = sizeof(XLIMB ) << 3;
				}

      bufsize--;

      ei = (E->GetLimbs()[nblimbs] >> bufsize) & 1;

      // skip leading 0s         
      if(ei == 0 && state == 0) continue;
      if(ei == 0 && state == 1)
				{
          // out of window, square X             
          MontgomeryMultiplication(this, this, N, mm, &T );
          continue;
				}

      // add ei to current window         
      state = 2;

      nbits++;
      wbits |= (ei << (wsize - nbits));

      if(nbits == wsize)
				{
					// X = X^wsize R^-1 mod N             
          for( i = 0; i < wsize; i++ )
						{
							MontgomeryMultiplication(this, this, N, mm, &T);
						}

          // X = X * W[wbits] R^-1 mod N             
          MontgomeryMultiplication(this, &W[wbits], N, mm, &T);

          state--;
          nbits = 0;
          wbits = 0;
				}
		}

	// process the remaining bits     
  for(i=0; i<nbits; i++)
		{
			MontgomeryMultiplication(this, this, N, mm, &T);
      wbits <<= 1;
      if((wbits & (one << wsize)) != 0) MontgomeryMultiplication(this, &W[1], N, mm, &T);
    }

	// X = A^E * R * R^-1 mod N = A^E mod N     
  MontgomeryReduction(this, N, mm, &T);

  if(neg)
		{
			SetSign(-1);
      if(!AdditionSigned(N, this)) return false;
		}

  for(i = (one << (wsize - 1)); i < (one << wsize); i++)
		{
			W[i].End();
		}

  W[1].End(); 
	T.End(); 
	Apos.End();

  if(_RR == NULL) RR.End();
  
  return true;
}




/*-------------------------------------------------------------------
//  XMPINTEGER::GreatestCommonDivisor
*/ 
/**
//	Greatest common divisor: G = gcd(A, B)  (HAC 14.54)
//	
//	@author				Abraham J. Velez
//	@version			07/03/2014 13:47:32
//	
//	@return 			int : 
//	@param				G : 
//  @param				A : 
//  @param				B : 
*/
/*-----------------------------------------------------------------*/
bool XMPINTEGER::GreatestCommonDivisor(XMPINTEGER* G, XMPINTEGER* A, XMPINTEGER* B)
{
	XDWORD			lz;
	XDWORD			lzt;
  XMPINTEGER	TG; 
	XMPINTEGER	TA;
	XMPINTEGER	TB;

  TG.Ini(); 
	TA.Ini(); 
	TB.Ini();

	if(!TA.CopyFrom(A)) return false;
  if(!TB.CopyFrom(B))	return false;

  lz	= TA.GetLSB();
  lzt = TB.GetLSB();

  if(lzt<lz) lz = lzt;

  if(!TA.RightShift(lz)) return false;
  if(!TB.RightShift(lz)) return false;

  TA.SetSign(1);
	TB.SetSign(1);

  while(TA.CompareSignedValues(0) != 0)
		{
		  if(!TA.RightShift(TA.GetLSB()))	return false;
      if(!TB.RightShift(TB.GetLSB()))	return false;

      if(TA.CompareSignedValues(TB) >= 0)
        {
          if(!TA.SubtractionABS(&TA, &TB)) return false;
          if(!TA.RightShift(1))						 return false;
        }
       else
        {
					if(!TB.SubtractionABS(&TB, &TA)) return false;
					if(!TB.RightShift(1))						 return false;
        }
    }

	if(!TB.LeftShift(lz))	return false;
  if(!G->CopyFrom(&TB))	return false;

  TG.End();
	TA.End(); 
	TB.End();

  return true;
}




/*-------------------------------------------------------------------
//  XMPINTEGER::FillRandom
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			10/03/2014 17:16:54
//	
//	@return 			int : 
//	@param				size : 
//  @param				funcrandom : 
//  @param				paramrandom : 
*/
/*-----------------------------------------------------------------*/
int XMPINTEGER::FillRandom(XDWORD size, XMPINTEGER_FUNCRANDOM funcrandom, void* paramrandom)
{ 
	if(!Grow(XMPINTEGER_CHARSTOLIMBS(size))) return false;
  if(!LeftSet(0))	return false;

	if(!funcrandom) return false;
  if(!funcrandom((unsigned char *)GetLimbs(), size, paramrandom)) return false;

  return true;
}





/*-------------------------------------------------------------------
//  XMPINTEGER::ModularInverse
*/ 
/**
//	Modular inverse: X = A^-1 mod N  (HAC 14.61 / 14.64)
//	
//	@author				Abraham J. Velez
//	@version			07/03/2014 13:55:31
//	
//	@return 			int : 
//  @param				A : 
//  @param				N : 
*/
/*-----------------------------------------------------------------*/
int XMPINTEGER::ModularInverse(XMPINTEGER* A, XMPINTEGER* N)
{
	XMPINTEGER G;
	XMPINTEGER TA;
	XMPINTEGER TU;
	XMPINTEGER U1;
	XMPINTEGER U2;
	XMPINTEGER TB;
	XMPINTEGER TV;
	XMPINTEGER V1;
	XMPINTEGER V2;

	if(N->CompareSignedValues(0) <= 0) return false;
  
	TA.Ini(); 
	TU.Ini(); 
	U1.Ini(); 
	U2.Ini();
  G.Ini(); 
	TB.Ini(); 
	TV.Ini();
  V1.Ini(); 
	V2.Ini();

	if(!G.GreatestCommonDivisor( &G, A, N ))	return false;

  if(G.CompareSignedValues(1) != 0) return false;
  
  if(!TA.Module(&TA, A, N))	return false;

  if(!TU.CopyFrom(&TA))	return false;
  if(!TB.CopyFrom(N))		return false;
  if(!TV.CopyFrom(N))		return false;

  if(!U1.LeftSet(1))		return false;
  if(!U2.LeftSet(0))		return false;
  if(!V1.LeftSet(0))		return false;
  if(!V2.LeftSet(1))		return false;

	do{	while((TU.GetLimbs()[0] & 1 ) == 0)
        {
				  if(!TU.RightShift(1))	return false;

          if((U1.GetLimbs()[0] & 1 )!= 0 || (U2.GetLimbs()[0] & 1 ) != 0)
            {
							if(!U1.AdditionSigned(&U1, &TB))		return false;
              if(!U2.SubtractionSigned(&U2, &TA))	return false;
            }

          if(!U1.RightShift(1))	return false;
          if(!U2.RightShift(1))	return false;
        }

      while((TV.GetLimbs()[0] & 1) == 0)
        {
				  if(!TV.RightShift(1)) return false;

          if(((V1.GetLimbs()[0] & 1 ) != 0) || ((V2.GetLimbs()[0] & 1 ) != 0))
            {
                if(!V1.AdditionSigned(&V1, &TB ))			return false;
                if(!V2.SubtractionSigned(&V2, &TA ))	return false;
            }

            if(!V1.RightShift(1))	return false;
            if(!V2.RightShift(1))	return false;
        }

      if(TU.CompareSignedValues(TV) >= 0)
        {
				  if(!TU.SubtractionSigned(&TU, &TV))	return false;
          if(!U1.SubtractionSigned(&U1, &V1))	return false;
          if(!U2.SubtractionSigned(&U2, &V2)) return false;
        }
       else
        {
          if(!TV.SubtractionSigned(&TV, &TU))	return false;
          if(!V1.SubtractionSigned(&V1, &U1))	return false;
          if(!V2.SubtractionSigned(&V2, &U2))	return false;
        }

		} while(TU.CompareSignedValues(0) != 0);

	while(V1.CompareSignedValues(0) < 0 )
		{
			if(!V1.AdditionSigned(&V1, N))	return false;
		}

	while(V1.CompareSignedValues((*N)) >= 0)
		{
			if(!V1.SubtractionSigned(&V1, N))	return false;
		}

  if(!CopyFrom(&V1)) return false;

	TA.End();
	TU.End(); 
	U1.End();
	U2.End();
	G.End();
	TB.End();
	TV.End();
  V1.End();
	V2.End();

	return true;
}



/*-------------------------------------------------------------------
//  int CheckSmallFactors
*/ 
/**
//  0: no small factor (possible prime, more tests needed)
//  1: certain prime
// <0: Error	
//	
//	@author				Abraham J. Velez
//	@version			07/03/2014 15:51:16
//	
//	@return 			static : 
//	@param				X : 
*/
/*-----------------------------------------------------------------*/
int XMPINTEGER::CheckSmallFactors()
{	
  XDWORD r;
	int		 i;
	int    ret = 0;

	if((GetLimbs()[0] & 1 ) == 0) return -1;
  
  for(i=0; SmallPrime[i]>0; i++)
    {
      if(CompareSignedValues(SmallPrime[i] ) <= 0 ) return 1;
     
      if(!Module(&r, this, SmallPrime[i]))	return -1;

      if(r== 0) return -1;
    }

	return  ret;
}




/*-------------------------------------------------------------------
//  XMPINTEGER::MillerRabin
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			10/03/2014 17:16:07
//	
//	@return 			bool : 
//	@param				X : 
//  @param				funcrandom : 
//  @param				paramrandom : 
*/
/*-----------------------------------------------------------------*/
bool XMPINTEGER::MillerRabin(XMPINTEGER* X, XMPINTEGER_FUNCRANDOM funcrandom, void* paramrandom)
{
	XMPINTEGER	W;
	XMPINTEGER	R;
	XMPINTEGER	T;
	XMPINTEGER	A;
	XMPINTEGER	RR;
	int					i;
	int					j;
	int					n;
	int					s;

	W.Ini(); 
	R.Ini(); 
	T.Ini(); 
	A.Ini();
  RR.Ini();
 
	// W = |X| - 1
	// R = W >> lsb( W )
   
  if(!W.SubtractionSigned(this, 1)) return false;
  
	s = W.GetLSB();
  
	if(!R.CopyFrom(&W))	return false;
  if(!R.RightShift(s))	return false;
  
	i = GetMSB();
  
	// HAC, table 4.4

  n = ( ( i >= 1300 ) ?  2 : ( i >=  850 ) ?  3 :
        ( i >=  650 ) ?  4 : ( i >=  350 ) ?  8 :
        ( i >=  250 ) ? 12 : ( i >=  150 ) ? 18 : 27 );

  for(i=0; i<n; i++)
    {        
      // pick a random A, 1 < A < |X| - 1        
      if(!A.FillRandom(GetNLimbs()*XMPINTEGER_CHARSINLIMB, funcrandom, paramrandom))	return false;

      if(A.CompareSignedValues(W) >= 0)
        {
          j = A.GetMSB() - W.GetMSB();
          if(!A.RightShift(j + 1 ))	return false;
        }
        
			A.GetLimbs()[0] |= 3;

      // A = A^R mod |X|
        
			if(!A.SlidingWindowExponentiation(&A, &R, this, &RR ))	return false;

      if(A.CompareSignedValues(W) == 0 || A.CompareSignedValues(1) == 0 )  continue;

      j = 1;
      while( j < s && A.CompareSignedValues(W) != 0 )
        {
					// A = A * A mod |X|             
					if(!T.Multiplication(&A, &A ))	return false;
          if(!A.Module( &A, &T, this))		return false;

          if(A.CompareSignedValues(1) == 0 ) break;

          j++;
        }

      // not prime if A != |X| - 1 or A == 1         
      if(A.CompareSignedValues(W) != 0 || (A.CompareSignedValues(1) == 0)) return false;      
    }

	W.End(); 
	R.End(); 
	T.End(); 
	A.End();
	RR.End();

	return true;
}



/*-------------------------------------------------------------------
//  XMPINTEGER::IsPrime
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			10/03/2014 17:15:28
//	
//	@return 			int : 
//	@param				funcrandom : 
//  @param				paramrandom : 
*/
/*-----------------------------------------------------------------*/
int XMPINTEGER::IsPrime(XMPINTEGER_FUNCRANDOM funcrandom, void* paramrandom)
{
 XMPINTEGER XX;
 int        ret;

 XX.CopyFrom(this);

 XX.SetSign(1);
 //XX.SetNLimbs(GetNLimbs());
 //XX.SetLimbs(GetLimbs());				// Abs(X) 

 if(XX.CompareSignedValues(0) == 0 || XX.CompareSignedValues(1) == 0)  return -1;
 
 if(XX.CompareSignedValues(2) == 0) return 0;

 if((ret = XX.CheckSmallFactors())!= 0)
    {
      if(ret == 1) return 0;            
      return ret;
    }

  return MillerRabin(&XX, funcrandom, paramrandom)?0:-1;
}





/*-------------------------------------------------------------------
//  XMPINTEGER::GeneratePrime
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			10/03/2014 17:20:32
//	
//	@return 			bool : 
//	@param				nbits : 
//  @param				dhflag : 
//  @param				funcrandom : 
//  @param				paramrandom : 
*/
/*-----------------------------------------------------------------*/
bool XMPINTEGER::GeneratePrime(int nbits, int dhflag, XMPINTEGER_FUNCRANDOM funcrandom, void* paramrandom)
{
	int				 k;
	int				 n;
  XDWORD		 r;
  XMPINTEGER Y;

	if( nbits < 3 || nbits > XMPINTEGER_MAXBITS) return false;
  
  Y.Ini();

  n = XMPINTEGER_BITSTOLIMBS(nbits);

  if(!FillRandom(n*XMPINTEGER_CHARSINLIMB, funcrandom, paramrandom)) return false;

  k = GetMSB();
  if(k<nbits) 
		{
			if(!LeftShift(nbits-k))		return false;
		}

  if(k>nbits) 
		{
			if(!RightShift(k-nbits))	return false;
		}

  GetLimbs()[0] |= 3;

  if(dhflag == 0)
    {
			int ret;

      while((ret=IsPrime(funcrandom, paramrandom))!= 0)
        {
					if( ret != -1)  return false;
					if(!AdditionSigned(this, 2)) return false;
        }
    }
   else
    {
			// An necessary condition for Y and X = 2Y + 1 to be prime is X = 2 mod 3 (which is equivalent to Y = 2 mod 3).
      // Make sure it is satisfied, while keeping X = 3 mod 4
        
			if(!Module( &r, this, 3))	return false;
      
			if(r == 0)
				{
					if(!AdditionSigned(this, 8))	return false;
				}
       else 
				{
					if( r == 1 )
						{ 
							if(!AdditionSigned(this, 4 ))	return false;
						}
				}

      // Set Y = (X-1) / 2, which is X / 2 because X is odd 
      if(!Y.CopyFrom(this)) return false;
      if(!Y.RightShift(1))  return false;

      while(1)
        {
					int ret;
					// First, check small factors for X and Y before doing Miller-Rabin on any of them

          if((ret = CheckSmallFactors()) == 0 && (ret = Y.CheckSmallFactors()) == 0 &&  (ret = MillerRabin(this, funcrandom, paramrandom)) == 0 && (ret = MillerRabin(&Y, funcrandom, paramrandom)) == 0)  break;
            

          if(ret != -1) return false;
            
          // Next candidates. We want to preserve Y = (X-1) / 2 and Y = 1 mod 2 and Y = 2 mod 3 (eq X = 3 mod 4 and X = 2 mod 3)  so up Y by 6 and X by 12.
           
          if(!AdditionSigned(this, 12))	return false;
          if(!Y.AdditionSigned(&Y, 6))	return false;
        }
    }

	Y.End();

  return true;
}




/*-------------------------------------------------------------------
//  XMPINTEGER::SetFromString
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			07/03/2014 17:53:10
//	
//	@return 			bool : 
//	@param				radix : 
//  @param				string : 
*/
/*-----------------------------------------------------------------*/
bool XMPINTEGER::SetFromString(int radix, XCHAR* string)
{
	int				 i;
	int			   j;
	int				 slen;
	int				 n;
	XDWORD		 d;
	XMPINTEGER T;

	if((radix < 2) || (radix > 16)) return false;
        
	T.Ini();

	XSTRING _string;

	_string = string;

  slen = _string.GetSize();

	if(radix == 16)
		{
			n = XMPINTEGER_BITSTOLIMBS(slen << 2);

			if(!Grow(n))		return false;
			if(!LeftSet(0))	return false;

			for(i=slen, j=0; i>0; i--, j++ )
				{
					if(i==1 && string[i-1]==__C('-'))
						{
							SetSign(-1);
							break;
						}

					if(!GetDigit(&d, radix, string[i-1])) return false;
					GetLimbs()[j / (2 * XMPINTEGER_CHARSINLIMB)] |= d << ( (j % (2 * XMPINTEGER_CHARSINLIMB)) << 2 );
				}
    }
   else
    {
			if(!LeftSet(0))	return false;

      for(i=0; i<slen; i++)
        {
          if(i==0 && string[i]==__C('-'))
            {
							SetSign(-1);
              continue;
            }

          if(!GetDigit(&d, radix, string[i])) return false;
          
					if(!T.Multiplication(this, radix))	return false;

          if(GetSign()== 1)
            {
							if(!AdditionSigned(&T, d))		return false;
            }
           else
            {
							if(!SubtractionSigned(&T, d))	return false;
            }
        }
    }

	T.End();
	
	return true;
}



/*-------------------------------------------------------------------
//  XMPINTEGER::SetFromString
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			10/03/2014 10:37:45
//	
//	@return 			bool : 
//	@param				radix : 
//  @param				string : 
*/
/*-----------------------------------------------------------------*/
bool XMPINTEGER::SetFromString(int radix, XSTRING& string)
{
	return SetFromString(radix, string.Get());
}




/*-------------------------------------------------------------------
//  XMPINTEGER::GetToString
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			10/03/2014 11:19:30
//	
//	@return 			bool : 
//	@param				radix : 
//  @param				string : 
*/
/*-----------------------------------------------------------------*/
bool XMPINTEGER::GetToString(int radix, XSTRING& string)
{	
	XMPINTEGER	T;
	XSTRING			charshex;
	int					n;  

	charshex = __L("0123456789ABCDEF");

	string.Empty();

	if( radix < 2 || radix > 16 ) return false;
  
  n = GetMSB();
  if( radix >=  4 ) n >>= 1;
  if( radix >= 16 ) n >>= 1;
  n += 3;

	T.Ini();

	if(GetSign() == -1) string.Add(__C('-'));

	if(radix == 16)
    {
      int c;
			int i;
			int j;
			int k;

      for(i = GetNLimbs(), k = 0; i > 0; i-- )
        {
         for(j=XMPINTEGER_CHARSINLIMB; j>0; j--)
            {
							XCHAR charhex;

							c=(GetLimbs()[i-1] >> ((j-1) << 3)) & 0xFF;

              if(c==0 && k==0 && (i+j+3)!= 0) continue;

							charhex = charshex.Get()[c / 16]; string.Add(charhex);
							charhex = charshex.Get()[c % 16]; string.Add(charhex);
							
              k=1;
            }
        }
    }
   else
    {
			if(!T.CopyFrom(this)) return false;

			if(T.GetSign() == -1) T.SetSign(1);

			if(!T.SetToStringHelper(radix, string)) return false;      
    }

  T.End();

  return true;
}




/*-------------------------------------------------------------------
//  XMPINTEGER::SetFromXBuffer
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			12/03/2014 8:58:01
//	
//	@return 			bool : 
//	@param				xbuffer : 
*/
/*-----------------------------------------------------------------*/
bool XMPINTEGER::SetFromXBuffer(XBUFFER& xbuffer)
{
	int i;
	int j;
	int n;

  for(n=0; n<(int)xbuffer.GetSize(); n++)
		{
			if(xbuffer.GetByte(n) != 0) break;
		}

	if(!Grow(XMPINTEGER_CHARSTOLIMBS(xbuffer.GetSize() - n ))) return false;
  if(!LeftSet(0))	return false; 	

  for(i = xbuffer.GetSize(), j=0; i>n; i--, j++)
		{
      GetLimbs()[j / XMPINTEGER_CHARSINLIMB] |= ((XLIMB)xbuffer.GetByte(i-1)) << ((j % XMPINTEGER_CHARSINLIMB) << 3);
		}

  return true;
}



/*-------------------------------------------------------------------
//	XMPINTEGER::GetToXBuffer
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			18/03/2014 11:39:41
//	
//	@return 			bool : 
//
//  @param				xbuffer : 
//  @param				size : 
*/
/*-----------------------------------------------------------------*/
bool XMPINTEGER::GetToXBuffer(XBUFFER& xbuffer, int size)
{
	int i;
	int j;
	int n;

  n = GetSize();

	if(size < n) return false;

	xbuffer.Resize(size);
  
  for(i=size-1, j=0; n>0; i--, j++, n--)
		{
			xbuffer.Set((XBYTE)(GetLimbs()[j / XMPINTEGER_CHARSINLIMB] >> ((j % XMPINTEGER_CHARSINLIMB) << 3)), i);
		}
	
	return true;
}





/*------------------------------------------------------------------
//  XMPINTEGER::End
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			25/02/2014 15:33:20
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool XMPINTEGER::End()
{
  if(!limbs) return false;
    	
  memset(limbs, 0, nlimbs* XMPINTEGER_CHARSINLIMB);       
	delete [] limbs;
   
  sign		= 1;
	nlimbs  = 0;
	limbs   = NULL;

	return true;
}





/*-------------------------------------------------------------------
//  XMPINTEGER::SubtractionHelper
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			26/02/2014 16:15:09
//	
//	@return 			void : 
//	@param				n : 
//  @param				*s : 
//  @param				*d : 
*/
/*-----------------------------------------------------------------*/
void XMPINTEGER::SubtractionHelper(XDWORD n, XLIMB *s, XLIMB *d)
{
  int		 i;
  XLIMB	 c = 0;
	XLIMB	 z;

  for(i=(int)c; i<(int)n; i++, s++, d++)
    {
			 z  = ( *d <  c );      *d -=  c;
       c  = ( *d < *s ) + z;  *d -= *s;
    }

  while( c != 0 )
    {
      z = ( *d < c ); *d -= c;
      c = z; i++; d++;
    }
}




/*-------------------------------------------------------------------
//  XMPINTEGER::MultiplicationHelper
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			26/02/2014 17:31:51
//	
//	@return 			void : 
//	@param				i : 
//  @param				s : 
//  @param				d : 
//  @param				b : 
*/
/*-----------------------------------------------------------------*/
void XMPINTEGER::MultiplicationHelper(XDWORD i, XLIMB* s, XLIMB* d, XLIMB b)
{
	XLIMB c = 0;
	XLIMB t = 0;

  for(; i>= 16; i-= 16)
    {
			XMPINTEGER_MULADDC_INIT
		
      XMPINTEGER_MULADDC_CORE   XMPINTEGER_MULADDC_CORE
      XMPINTEGER_MULADDC_CORE   XMPINTEGER_MULADDC_CORE
      XMPINTEGER_MULADDC_CORE   XMPINTEGER_MULADDC_CORE
      XMPINTEGER_MULADDC_CORE   XMPINTEGER_MULADDC_CORE

      XMPINTEGER_MULADDC_CORE   XMPINTEGER_MULADDC_CORE
      XMPINTEGER_MULADDC_CORE   XMPINTEGER_MULADDC_CORE
      XMPINTEGER_MULADDC_CORE   XMPINTEGER_MULADDC_CORE
      XMPINTEGER_MULADDC_CORE   XMPINTEGER_MULADDC_CORE
		
			XMPINTEGER_MULADDC_STOP
    }

  for( ; i>=8; i-=8)
    {
      XMPINTEGER_MULADDC_INIT

			XMPINTEGER_MULADDC_CORE   XMPINTEGER_MULADDC_CORE
      XMPINTEGER_MULADDC_CORE   XMPINTEGER_MULADDC_CORE

      XMPINTEGER_MULADDC_CORE   XMPINTEGER_MULADDC_CORE
      XMPINTEGER_MULADDC_CORE   XMPINTEGER_MULADDC_CORE
			
      XMPINTEGER_MULADDC_STOP
    }

  for( ; i > 0; i-- )
    {
      XMPINTEGER_MULADDC_INIT
      XMPINTEGER_MULADDC_CORE
      XMPINTEGER_MULADDC_STOP
    }

	t++;

  do{  (*d) += c; 
			 c = ( *d < c ); 
			 d++;

    } while( c != 0 );
}





/*-------------------------------------------------------------------
//  XMPINTEGER::GetDigit
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			07/03/2014 17:31:04
//	
//	@return 			bool : 
//	@param				data : 
//  @param				radix : 
//  @param				character : 
*/
/*-----------------------------------------------------------------*/
bool XMPINTEGER::GetDigit(XDWORD* data, int radix, XCHAR character)
{
	(*data) = 255;

  if(character >= 0x30 && character <= 0x39 ) (*data) = character - 0x30;
  if(character >= 0x41 && character <= 0x46 ) (*data) = character - 0x37;
  if(character >= 0x61 && character <= 0x66 ) (*data) = character - 0x57;

  if( (*data) >= (XDWORD)radix) return false;
  
  return true;
}




/*-------------------------------------------------------------------
//  XMPINTEGER::SetToStringHelper
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			10/03/2014 11:07:23
//	
//	@return 			bool : 
//	@param				radix : 
//  @param				string : 
*/
/*-----------------------------------------------------------------*/
bool XMPINTEGER::SetToStringHelper(int radix, XSTRING& string)
{
	XLIMB r;

	if(radix < 2 || radix > 16) return false;
  
	if(!Module(&r, this, radix)) return false;
  if(!Division(this, NULL, this, radix)) return false;

  if(CompareSignedValues(0)!= 0)
		{
			if(!SetToStringHelper(radix, string)) return false;			
		}

  if(r<10)
			   string.Add((XCHAR)(r + 0x30 ));
    else string.Add((XCHAR)(r + 0x37 ));

  return true;
}