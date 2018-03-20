
/*------------------------------------------------------------------------------------------
//	GRPARRAYALIGNTOOL.CPP
//	
//	Align tool for bounding box objects
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 16/04/2015 17:38:23
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "GRPArrayAlignTool.h"
#include "GRPObject.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//	GRPARRAYALIGNTOOL::Align
*/	
/**	
//	Applies spacing functions
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			16/04/2015 17:43:00
//	
//  @param				obj : 
*/
/*-----------------------------------------------------------------*/
void GRPARRAYALIGNTOOL::Align(GRPOBJECT* obj)
{
		//---------------------- check for elements to align
	XDWORD nelements = obj->GetElements()->GetSize();  
	if(nelements == 0) return;

	/*
	GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_FREECENTER						//use margins, spacing	and both offsets
	GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_STARTOFFSET						//offset from start			
	GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_ENDOFFSET							//offset from end			
	GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_SPECIFYSPACING				//specify space between objects			
	
	GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_FIXED									//use cell size
	GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_MARGINS							  //use bounding box and margins				

	#GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_DIVIDEEVENLY					//space depending on target size			
	#GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_OBJECTSATTHEEND			//no offsets			
	#GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_CENTERED							//offsets set on middle				
	#GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_FOLLOW								//follow path			
	*/

	GRPPOINT Caret; // current space point

	if (distribution & GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_STARTOFFSET) //---------------------------------------- starting offset 
		Caret+=this->startingoffset;

	//---------------------------------------- align elements one by one
	for (XDWORD e=0;e<nelements;e++)
	{
		GRPELEMENT* element			= obj->GetElements()->FastGet(e);
		GRPNODE*		node				= element->GetNode();
		GRPBB*			bb					=	element->GetBoundingBox();

		if (e!=0)
		if (distribution & GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_MARGINS)    	//---------------------------------------- add defined first margin
		{
			switch(this->direction)
			{
			case GRPARRAYALIGNTOOL_ALIGNFLAGS_HORIZONTAL: Caret.x+=bb->leftmargin;	break;
			case GRPARRAYALIGNTOOL_ALIGNFLAGS_VERTICAL:		Caret.y-=bb->topmargin;		break;
			case GRPARRAYALIGNTOOL_ALIGNFLAGS_DEPTH:			Caret.z+=bb->depthmargin;	break;
			}			
		}

		node->SetPosition(Caret.x,Caret.y,Caret.z);

		if (distribution & GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_SPECIFYSPACING) //---------------------------------------- add specified spacing
		{
			switch(this->direction)
			{
			case GRPARRAYALIGNTOOL_ALIGNFLAGS_HORIZONTAL: Caret.x+=cellspacing.x;	break;
			case GRPARRAYALIGNTOOL_ALIGNFLAGS_VERTICAL:		Caret.y-=cellspacing.y;	break;
			case GRPARRAYALIGNTOOL_ALIGNFLAGS_DEPTH:			Caret.z+=cellspacing.z;	break;
			}			
		}
			

		if (distribution & GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_FIXED)         //---------------------------------------- use cell space instead of bounding box
		{
			switch(this->direction)
			{
			case GRPARRAYALIGNTOOL_ALIGNFLAGS_HORIZONTAL: Caret.x+=cellsize.x;	break;
			case GRPARRAYALIGNTOOL_ALIGNFLAGS_VERTICAL:		Caret.y-=cellsize.y;	break;
			case GRPARRAYALIGNTOOL_ALIGNFLAGS_DEPTH:			Caret.z+=cellsize.z;	break;
			}			
		}
		else if (distribution & GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_GLYPHS) 
		{					
			Caret.x+=bb->advance+bb->kerning;	
		}
		else
		{
			switch(this->direction)                                            //---------------------------------------- use bounding box space
			{
			case GRPARRAYALIGNTOOL_ALIGNFLAGS_HORIZONTAL: Caret.x+=bb->width;		break;
			case GRPARRAYALIGNTOOL_ALIGNFLAGS_VERTICAL:		Caret.y-=bb->height;	break;
			case GRPARRAYALIGNTOOL_ALIGNFLAGS_DEPTH:			Caret.z+=bb->depth;		break;
			}			
		}

		if (distribution & GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_MARGINS)       //---------------------------------------- use last margin
		{
			switch(this->direction)
			{
			case GRPARRAYALIGNTOOL_ALIGNFLAGS_HORIZONTAL: Caret.x+=bb->rightmargin;	break;
			case GRPARRAYALIGNTOOL_ALIGNFLAGS_VERTICAL:		Caret.y-=bb->bottommargin;break;
			case GRPARRAYALIGNTOOL_ALIGNFLAGS_DEPTH:			Caret.z+=bb->frontmargin;	break;
			}			
		}


	}


	if (distribution & GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_ENDOFFSET)      //---------------------------------------- ending offset for centering 
		Caret+=this->endingoffset;


	//------------------------------------------------------------------------------ ALIGNMENT

	switch(this->direction)
	{
	case GRPARRAYALIGNTOOL_ALIGNFLAGS_HORIZONTAL:

		for (XDWORD e=0;e<obj->GetElements()->GetSize();e++)
		{
			switch(this->align)
			{
			case GRPARRAYALIGNTOOL_ALIGNFLAGS_RIGHT	: 
				{
				const GRPVECTOR* position=obj->GetElements()->Get(e)->GetNode()->GetPosition();
				obj->GetElements()->Get(e)->GetNode()->SetPosition(position->vector[0]-Caret.x,position->vector[1],position->vector[2]);
				break;
				}				
			case GRPARRAYALIGNTOOL_ALIGNFLAGS_CENTER:
				{
				const GRPVECTOR* position=obj->GetElements()->Get(e)->GetNode()->GetPosition();
				obj->GetElements()->Get(e)->GetNode()->SetPosition(position->vector[0]-(Caret.x/2.0f),position->vector[1],position->vector[2]);
				break;
				}			
			}
		}
		break;
	}


}

/*-------------------------------------------------------------------
//	 GRPARRAYALIGNTOOL::Align
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	30/01/2018 14:11:44
//	@return		void : 
//
//	@param		GRPOBJECT* : 
//	@param		GRPAABB* : 
//
*//*-----------------------------------------------------------------*/
void GRPARRAYALIGNTOOL::Align(GRPOBJECT* obj, GRPAABB* bound)
{
		if (!bound)
				return Align(obj);

		//---------------------- check for elements to align
		XDWORD nelements = obj->GetElements()->GetSize();
		if (nelements == 0) return;
		
		GRPPOINT Caret; // current space point

		//---------------------------------------- align elements one by one
		for (XDWORD e = 0; e<nelements; e++)
		{
				GRPELEMENT* element = obj->GetElements()->FastGet(e);
				GRPNODE*		node = element->GetNode();
				GRPBB*			bb = element->GetBoundingBox();

				node->SetPosition(Caret.x, Caret.y, Caret.z);
				Caret.x += bb->advance + bb->kerning;
				if (Caret.x > bound->width)
				{
						Caret.x = 0.0;
						Caret.y += bb->lineheight;
				}
		}

}

