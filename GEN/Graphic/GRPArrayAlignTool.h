/*------------------------------------------------------------------------------------------
//	GRPARRAYALIGNTOOL.H
*/	
/**	
// \class 
//   
//  Align helper for Objects and elements
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 16/04/2015 16:37:20
//	Last Modification	:	
*/	
//*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPARRAYALIGNTOOL_H_
#define _GRPARRAYALIGNTOOL_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "XBase.h"

#include "GRPOpenGL.h"
#include "GRPVector.h"
#include "GRPMatrix.h"
#include "GRPBoundingBox.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


enum GRPARRAYALIGNTOOL_ALIGNFLAGS
{
	GRPARRAYALIGNTOOL_ALIGNFLAGS_NONE				= 0			,
	GRPARRAYALIGNTOOL_ALIGNFLAGS_LEFT				= 1<<0	,
	GRPARRAYALIGNTOOL_ALIGNFLAGS_TOP				= 1<<1	,
	GRPARRAYALIGNTOOL_ALIGNFLAGS_BOTTOM			= 1<<2	,
	GRPARRAYALIGNTOOL_ALIGNFLAGS_RIGHT			= 1<<3	,
	GRPARRAYALIGNTOOL_ALIGNFLAGS_FRONT			= 1<<4	,
	GRPARRAYALIGNTOOL_ALIGNFLAGS_BACK				= 1<<5	,
	GRPARRAYALIGNTOOL_ALIGNFLAGS_CENTER			= 1<<6	,
};

enum GRPARRAYALIGNTOOL_AXIS_DIRECTION_TYPE 
{
	GRPARRAYALIGNTOOL_ALIGNFLAGS_HORIZONTAL =0,
	GRPARRAYALIGNTOOL_ALIGNFLAGS_VERTICAL,		
	GRPARRAYALIGNTOOL_ALIGNFLAGS_DEPTH			
};

enum GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE 
{
	GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_NONE													= 0			,
	GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_DIVIDEEVENLY									= 1<<0	,
	GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_OBJECTSATTHEEND								= 1<<1	,
	GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_CENTERED											= 1<<2	,
	GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_STARTOFFSET										= 1<<3	,
	GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_ENDOFFSET											= 1<<4	,
	GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_SPECIFYSPACING								= 1<<5	,		
	GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_FOLLOW												= 1<<6	,	
	GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_FIXED													= 1<<7	,
	GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_MARGINS												= 1<<8	,	
	GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_GLYPHS												= 1<<9	,

	GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_FREECENTER										= (GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_STARTOFFSET		| GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_ENDOFFSET					|			GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_SPECIFYSPACING),
	GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_DIVIDEEVENLY_OBJECTSATTHEEND	= (GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_DIVIDEEVENLY		| GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_OBJECTSATTHEEND),
	GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_CENTERED_SPECIFYSPACING				= (GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_CENTERED				| GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_SPECIFYSPACING)	,
	GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_CENTERED_DIVIDEEVENLY					= (GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_CENTERED				| GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_DIVIDEEVENLY)		,
	GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_ENDOFFSET_DIVIDEEVENLY				= (GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_ENDOFFSET			| GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_DIVIDEEVENLY)		,
	GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_ENDOFFSET_SPECIFYSPACING			= (GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_ENDOFFSET			| GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_SPECIFYSPACING)
};



/*---- CLASS -----------------------------------------------------------------------------*/

class GRPOBJECT;
class GRPPATH;

class GRPARRAYALIGNTOOL 
{
	public:

																						GRPARRAYALIGNTOOL					()									{	Clean();					}
		virtual												 				 ~GRPARRAYALIGNTOOL					()									{	Clean();					}

		void																		Align											(GRPOBJECT* obj);
		void																		Align											(GRPOBJECT* obj,GRPAABB*);

		GRPBB																		totalboundingbox;

		XDWORD																	numberofcopies;

		bool																		copiesareinstances;
					
		bool																		rotationactive;
		bool																		translationactive;
		bool																		scaleactive;

		bool																		incremental;
		bool																		total;


		GRPPOINT																caret;

		GRPPATH*																path;

		GRPPOINT																startingoffset;
		GRPPOINT																endingoffset;

		GRPPOINT																translationincrement;
		GRPPOINT																rotationincrement;
		GRPPOINT																scaleincrement;

		GRPARRAYALIGNTOOL_ALIGNFLAGS						globalalign;
		GRPARRAYALIGNTOOL_ALIGNFLAGS						align;
		GRPARRAYALIGNTOOL_AXIS_DIRECTION_TYPE		direction;	
		XDWORD																	distribution;

		GRPPOINT																cellspacing;
		GRPPOINT																cellsize;
		GRPMATRIX																cellmatrix;			

protected:

private:

		void																		Clean()
																						{
																							numberofcopies				= 0;
																							copiesareinstances		= true;

																							rotationactive				= false;
																							translationactive			= false;
																							scaleactive						= false;

																							incremental						= true;
																							total									= false;

																							startingoffset				= GRPPOINT(0.0f,0.0f,0.0f);
																							endingoffset					= GRPPOINT(0.0f,0.0f,0.0f);

																							cellspacing						= GRPPOINT(0.0f,0.0f,0.0f);
																							cellsize							= GRPPOINT(0.0f,0.0f,0.0f);

																							translationincrement	= GRPPOINT(0.0f,0.0f,0.0f);
																							rotationincrement			= GRPPOINT(0.0f,0.0f,0.0f);
																							scaleincrement				=	GRPPOINT(0.0f,0.0f,0.0f);

																							align									=	GRPARRAYALIGNTOOL_ALIGNFLAGS_NONE;
																							distribution					= GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_NONE;
																							direction							= GRPARRAYALIGNTOOL_ALIGNFLAGS_HORIZONTAL;
																						}

};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif



