/*------------------------------------------------------------------------------------------
//	GRPSLOTNUMBER.H
*/	
/**	
// \class 
//   
//  Manages slot scores
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 07/11/2016 16:45:24
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPSLOTNUMBER_H_
#define _GRPSLOTNUMBER_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPTextBox.h"

#include "GRPStencilPass.h"

#include "XBase.h"
#include "XVector.h"

#include "XInterpolator.h"


	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/

class GRPSLOTNUMBER : public GRPTEXTCONTAINER
{
public:

				 GRPSLOTNUMBER	(XDWORD id);
virtual	~GRPSLOTNUMBER	();

				
				bool											Init						(XDWORD nSlots,GRPSCENE*	sc,XCHAR* fontname=__L("arialblack.ttf"),GRPSTENCILPASS* reusestencil=NULL);
				bool											SetValue				(GRPANIMATIONCURVETYPE,GRPANIMATIONCURVEAXIS,double,double);
				bool											UpdateContent		(bool				mergeelements = true);
				void											Clear						();

				GRPOBJECT*								GetObj					();

				void											SetCurrent			(XDWORD current);
				void											SetSpeed  			(float f);								
				XVECTOR<GRPELEMENT*>* GetSlots				();
				GRPFONT*									GetFont					();
				void											SetCell   			(float cellwidth,		float cellspace);
				void											GetCell   			(float *cellwidth,	float *cellspace);

				void											SetMultiplicator(XDWORD mult) { this->multiplicator=mult;}
				void											SetAlign				(GRPARRAYALIGNTOOL_ALIGNFLAGS align);

				bool											Update					(XQWORD delta);
				GRPSTENCILPASS*						GetStencilPass	();	

				 XINTERPOLATOR*						interpolator;
				 XDWORD										multiplicator;

				 double carry;



protected:	
				XDWORD										id;


				XDWORD										nSlots;

				GRPSTENCILPASS*						stencilpass;
				GRPSCENE*									scene;

				GRPOBJECT*								obj;				

				float											SlotWidth;
				float											TotalWidth;
				float											SlotTotalHeight;
				float											NumberHeight;

				XVECTOR<GRPELEMENT*>			slots;			  

				GRPARRAYALIGNTOOL_ALIGNFLAGS align;

				float											CellWidth;
				float											CellSpace;
				float											CellGap;
				float											CellHeight;

private:

				void Clean()
				{					
					carry							=0.0f;
					nSlots						=0;	
					multiplicator			=1;
					interpolator			=NULL;
					stencilpass				=NULL;
					scene							=NULL;
					obj								=NULL;

					SlotWidth					=0.0f;
					TotalWidth				=0.0f;
					SlotTotalHeight		=0.0f;
					NumberHeight			=0.0f;

					CellWidth					=0.22f;
					CellSpace					=0.08f;
					CellHeight				=0.20f;
					CellGap						=0.07f;

					align							=GRPARRAYALIGNTOOL_ALIGNFLAGS_RIGHT;

					slots.SetIsMulti	(false);

				}
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

