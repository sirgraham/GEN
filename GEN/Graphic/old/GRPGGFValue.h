/*------------------------------------------------------------------------------------------
//	GRPGGFVALUE.H
*/	
/**	
// \file 
//   
//  GGFVALUE
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 17/11/2015 9:46:57
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPGGFVALUE_H_
#define _GRPGGFVALUE_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
#include "XBase.h"
#include "XBuffer.h"
	
#include "GRPVector.h"
#include "GRPColor.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
class GGFVALUE
{
	public:
																			GGFVALUE																												() : buffer( false)
																			{
																				Clean();
																			}

																		 ~GGFVALUE																												()
																			{
																				Clean();
																			}

		bool															GetBOOL																													()
																			{
																				XBYTE value;
																				buffer.Get(&value, sizeof(value), 0);
																				return value != 0;
																			}

		//casting functions to get values
		XBYTE															GetXBYTE																												()
																			{
																				XBYTE value;
																				buffer.Get(&value, sizeof(value), 0);
																				return value;
																			}

		XWORD															GetXWORD																												()
																			{
																				XWORD value;
																				buffer.Get((XBYTE*)&value, sizeof(value), 0);
																				return value;
																			}

		XDWORD														GETXDWORD																												()
																			{
																				XDWORD value;
																				buffer.Get((XBYTE*)&value, sizeof(value), 0);
																				return value;
																			}

		XQWORD														GETXQWORD																												()
																			{
																				XQWORD value;
																				buffer.Get((XBYTE*)&value, sizeof(XQWORD), 0);
																				return value;
																			}

		GRPVECTOR													GetGRPVECTOR																										()
																			{
																				GRPVECTOR value;
																				buffer.Get((XBYTE*)value.vector, 3*sizeof(float), 0);
																				return value;
																			}

		GRPCOLOR													GetGRPCOLOR																											()
																			{
																				GRPCOLOR value;
																				float array[4];
																				buffer.Get((XBYTE*)array, 4*sizeof(float), 0);
																				value.Set(array[0], array[1], array[2], array[3]);
																				return value;
																			}

		GRPPOINT													GetGRPPOINT																											()
																			{
																				GRPPOINT point;
																				float array[4];
																				buffer.Get((XBYTE*)&(point.index), sizeof(XDWORD), 0);
																				buffer.Get((XBYTE*)array, 4*sizeof(float), sizeof(XDWORD));
																				point.x = array[0];
																				point.y = array[1];
																				point.z = array[2];
																				point.w = array[3];
																				return point;
																			}

		float															Getfloat																												()
																			{
																				float value;
																				buffer.Get((XBYTE*)&value, sizeof(float), 0);
																				return value;
																			}

		double														Getdouble																												()
																			{
																				double value;
																				buffer.Get((XBYTE*)&value, sizeof(double), 0);
																				return value;
																			}

		int																Getint																													()
																			{
																				int value;
																				buffer.Get((XBYTE*)&value, sizeof(int), 0);
																				return value;
																			}

		template <class T>
		T																	GetValue																												()
																			{
																				T value;
																				buffer.Get((XBYTE*)&value, sizeof(T), 0);
																				return value;
																			}

		void															GetCharString																										(char* str, XDWORD size)
																			{
																				buffer.Get((XBYTE*)str, size*sizeof(char), 0);
																			}

		bool															SetBOOL																													(bool b)
																			{
																				buffer.Resize(sizeof(XBYTE));
																				XBYTE insert = (b) ? 1 : 0;
																				size = sizeof(XBYTE);
																				return buffer.Set((XBYTE*)&insert, sizeof(XBYTE), 0);
																			}

		bool															SetXBYTE																												(XBYTE byte)
																			{
																				buffer.Resize(sizeof(byte));
																				size = sizeof(XBYTE);
																				return buffer.Set((XBYTE*)&byte, sizeof(byte), 0);
																			}

		bool															SetXWORD																												(XWORD xword)
																			{
																				buffer.Resize(sizeof(xword));
																				size = sizeof(XWORD);
																				return buffer.Set((XBYTE*)&xword, sizeof(xword), 0);
																			}

		bool															SetXDWORD																												(XDWORD xdword)
																			{
																				buffer.Resize(sizeof(xdword));
																				size = sizeof(XDWORD);
																				return buffer.Set((XBYTE*)&xdword, sizeof(xdword), 0);
																			}

		bool															SetXQWORD																												(XQWORD xqword)
																			{
																				buffer.Resize(sizeof(xqword));
																				size = sizeof(XQWORD);
																				return buffer.Set((XBYTE*)&xqword, sizeof(xqword), 0);
																			}

		bool															SetGRPVECTOR																										(GRPVECTOR vector)
																			{
																				buffer.Resize(3*sizeof(float));
																				size = 3*sizeof(float);
																				return buffer.Set((XBYTE*)vector.vector, 3*sizeof(float), 0);
																			}

		/*bool															SetGRPCOLOR																											(GRPCOLOR& color)
																			{
																				float array[4];
																				array[0] = color.GetRed();
																				array[1] = color.GetGreen();
																				array[2] = color.GetBlue();
																				array[3] = color.GetAlpha();
																				size = 4*sizeof(float);
																				buffer.Resize(size);
																				return buffer.Put((XBYTE*)array, size, 0);
																			}*/

			bool														SetGRPCOLOR																											(GRPCOLOR color)
																			{
																				float array[4];
																				array[0] = color.GetRed();
																				array[1] = color.GetGreen();
																				array[2] = color.GetBlue();
																				array[3] = color.GetAlpha();
																				size = 4*sizeof(float);
																				buffer.Resize(size);
																				return buffer.Set((XBYTE*)array, size, 0);
																			}


		bool															SetGRPPOINT																											(GRPPOINT& point)
																			{
																				size = sizeof(XDWORD) + 4*sizeof(float);
																				buffer.Resize(size);
																				float array[4];
																				array[0] = point.x;
																				array[1] = point.y;
																				array[2] = point.z;
																				array[3] = point.w;
																				if(!buffer.Set((XBYTE*)&(point.index), sizeof(XDWORD), 0)) return false;
																				return buffer.Set((XBYTE*)array, 4*sizeof(float));
																			}

		float															Setfloat																												(float value)
																			{
																				buffer.Resize(sizeof(float));
																				size = sizeof(float);
																				return buffer.Set((XBYTE*)& value, sizeof(float), 0);
																			}

		double														Setdouble																												(double value)
																			{
																				buffer.Resize(sizeof(double));
																				size = sizeof(double);
																				return buffer.Set((XBYTE*)& value, sizeof(double), 0);
																			}

		int																Setint																													(int value)
																			{
																				buffer.Resize(sizeof(int));
																				size = sizeof(int);
																				return buffer.Set((XBYTE*)& value, sizeof(int), 0);
																			}

		template <class T>
		bool															SetValue																												(T v)
																			{
																				buffer.Resize(sizeof(T));
																				size = sizeof(T);
																				return buffer.Set((XBYTE*)&v, sizeof(T), 0);
																			}

		bool															SetCharString																									(char* str, XDWORD size)
																			{
																				buffer.Resize(size*sizeof(char));
																				this->size = size;
																				return buffer.Set((XBYTE*)str, size*sizeof(char), 0);
																			}

		XBUFFER														buffer;
		XDWORD														size;

	protected:

	private:
		void															Clean																	()
																			{
																				size = 0;
																			}
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

