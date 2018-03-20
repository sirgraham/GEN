/*------------------------------------------------------------------------------------------
//	GRPDEFORMER.H
*/	
/**	
// \class 
//   
//  Surface deformer
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 08/05/2015 13:56:16
//	Last Modification	:	
*/	
//*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPDEFORMER_H_
#define _GRPDEFORMER_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "XMap.h"
#include "GRPVertex.h"
#include "GRPAnimation.h"
#include "GRPInterfaceAnimable.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

class GRPELEMENT;
	

/*---- CLASS -----------------------------------------------------------------------------*/

class GRPSUBDEFORMERSHAPE
{
public:

		~GRPSUBDEFORMERSHAPE() 
		{
			indices.DeleteAll				();
			vertices.DeleteContents	();
			vertices.DeleteAll			();
		}

		XVECTOR<long long>			indices;
		XVECTOR<GRPVERTEX*>			vertices;
};




class GRPSUBDEFORMER : public GRPINTERFACEANIMABLE
{
public:

																	GRPSUBDEFORMER		();
																	~GRPSUBDEFORMER		();

					float										GetFactor					() { return Factor;}
					bool										GetIsActive				() { return Active;}
					GRPSUBDEFORMERSHAPE*		GetShape					() { return shape;}
					void										SetShape					(GRPSUBDEFORMERSHAPE*		shape)		{ this->shape=shape;}
					bool										SetValue					(GRPANIMATIONCURVETYPE	type,			GRPANIMATIONCURVEAXIS subtype,	double	value,		double base);
					bool										Init							(GRPELEMENT*						element,	XDWORD								channel);
					
protected:

					float										Factor;
					bool										Active;
					GRPSUBDEFORMERSHAPE*		shape;					

private:

	void Clean()
	{		
		Factor	=0.0f;
		Active	=true;
		shape		=NULL;
	}
};




class GRPDEFORMER : public virtual GRPNAMED
{
public:
																GRPDEFORMER		()															{		Clean();																												}								
	virtual												~GRPDEFORMER	()															{		subdeformers.DeleteElementContents();	subdeformers.DeleteAll();	}
	virtual GRPNAMED_TYPE					GetType				()															{		return GRPNAMED_TYPE_GRPDEFORMER;																}
	void													Add						(GRPSUBDEFORMER* subdeformer)		{		this->subdeformers.Add(subdeformer->tag,subdeformer);						}
	GRPELEMENT*										GetTarget			()															{		return target;																									}
	void													SetTarget			(GRPELEMENT*	 element)					{		this->target=element;																						}
	XMAP<XQWORD,GRPSUBDEFORMER*>* GetSubdeformers()															{		return &this->subdeformers;																			}


	virtual bool									Init					();	
	virtual bool									Update				();

protected:

	GRPELEMENT*										target;
	XMAP<XQWORD,GRPSUBDEFORMER*>	subdeformers;

private:

	void													Clean					()															{		target=NULL;																										}

};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

