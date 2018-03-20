/*------------------------------------------------------------------------------------------
//	PHYMATERIAL.H
*/	
/**	
// \class 
//   
//  Physics material abstract entity
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 26/05/2016 19:29:49
//	Last Modification	:	
*/	
/*	(C) Copyright GEN. All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _PHYMATERIAL_H_
#define _PHYMATERIAL_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

class PHYMATERIAL
{
public:
		PHYMATERIAL() { Clean(); }


		float friction;
		float dynamicfriction;
		float restitution;

		void Clean()
		{
		 friction				=0;
		 dynamicfriction=0;
		 restitution		=0;
		}
};

#endif