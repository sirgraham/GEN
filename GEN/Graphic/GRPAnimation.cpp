/*------------------------------------------------------------------------------------------
//	GRPANIMATION.CPP
//	
/	Animation control
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 10/02/2015 17:53:16
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	

/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XFactory.h"
#include "XTimer.h"

#include "GRPBezierSpline.h"
#include "GRPNode.h"
#include "GRPSubject.h"
#include "GRPElement.h"
#include "GRPAnimation.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/

//#define GRPANIMATION_FASTCURVES


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/	

/*-------------------------------------------------------------------
//	GRPANIMATIONCURVE::Init
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/02/2015 10:53:12
//	
*/
/*-----------------------------------------------------------------*/

void GRPANIMATIONCURVE::Init()
{
	firsttime=0;
	lasttime=0;
	
	bool Warning=false;

	if (this->keytime.GetSize()>0)
	{
		firsttime=(XQWORDSIG)this->keytime.FastGet(0);
		lasttime=(XQWORDSIG)this->keytime.GetLast();
		mintime = firsttime;
		maxtime = lasttime;
	}
	if (this->keyvalues.GetSize()>0)
	{
		firstvalue=(float)this->keyvalues.FastGet(0);
		lastvalue=(float)this->keyvalues.GetLast();		
	}

	nkeyattrrefcount	=this->keyattributes.GetSize();
	nkeys							=this->keyvalues.GetSize();
	bcsSize						=0;

	if(this->keytime.GetSize()==1)
		{
			keyattributes.Set(0,GRPANIMATIONCURVETANGENTMODE_CONSTANT);
			UnpackedAttributes.Set(0,GRPANIMATIONCURVETANGENTMODE_CONSTANT);							
			return;
		}		

	XDWORD kt=keytime.GetSize()-1;
	for(XDWORD e=0; e<kt; e++)
		{
			XQWORD  offset						= 0;
			XQWORD	att								=	this->GetAttribute(e,&offset);
			XQWORD  offsetNext				= 0;
			XQWORD	attNext						=	this->GetAttribute(e,&offsetNext);

			XQWORD  prevoffset				= 0;
			XQWORD	attPrev						=	0;

			UnpackedAttributes.Add(att & (GRPANIMATIONCURVETANGENTMODE_CUBIC | GRPANIMATIONCURVETANGENTMODE_LINEAR |GRPANIMATIONCURVETANGENTMODE_CONSTANT));

			if (e>0) attPrev					=	this->GetAttribute(e-1,&prevoffset);
			
			XQWORDSIG	ini								= keytime.FastGet(e);
			XQWORDSIG	end								= keytime.FastGet(e+1);
			XQWORDSIG	longitude					= end-ini;				

			int			rightslope		    = 0;
			int			nextleftslope			= 0;
			int			rightweight				= 0;
			int			nextleftweight		= 0;
			int			rightvelocity			= 0;
			int			nextleftvelocity	= 0;

			float		drightslope				= 0.0f;
			float		dnextleftslope		= 0.0f;
			float		drightweight			= 0.0f;
			float		dnextleftweight		= 0.0f;
			float		drightvelocity		= 0.0f;
			float		dnextleftvelocity	= 0.0f;

			if (att & GRPANIMATIONCURVETANGENTMODE_CONSTANT)
			{	

					continue;
			}
						
			if (att & GRPANIMATIONCURVETANGENTMODE_LINEAR)
			{
					double vini = keyvalues.FastGet(e);
					double vend = keyvalues.FastGet(e + 1);
					double	m		= 0.0f;

					if (end  != ini)
					if (vini != vend)
							 m=(vend - vini) / float(end - ini);
					
					keyslopes.Add(m);
					continue;
			}

			//------------------------------------------------------------------------------------------- START SIDE
			if(att & GRPANIMATIONCURVETANGENTMODE_AUTO)
			{
 				drightslope		= 0.0f;
				drightweight	= 0.33f;

				rightvelocity	= (int)keyattrdatafloat.FastGet(GRPANIMATIONCURVEFLAG_ORDER_RIGHTVELOCITY + (int)offset);	
				rightslope		= (int)keyattrdatafloat.FastGet(GRPANIMATIONCURVEFLAG_ORDER_RIGHTSLOPE    + (int)offset);

			}
			else
			if(att & GRPANIMATIONCURVETANGENTMODE_USER)
			{					
				rightslope		  = (int)keyattrdatafloat.FastGet(GRPANIMATIONCURVEFLAG_ORDER_RIGHTSLOPE    + (int)offset);

				if(att & GRPANIMATIONCURVETANGENTMODE_WEIGHTEDRIGHT)	
					rightweight	  = (int)keyattrdatafloat.FastGet(GRPANIMATIONCURVEFLAG_ORDER_RIGHTWEIGHT   + (int)offset);	

					rightvelocity = (int)keyattrdatafloat.FastGet(GRPANIMATIONCURVEFLAG_ORDER_RIGHTVELOCITY + (int)offset);					
											
					memcpy(&drightslope			,	&rightslope		  ,	sizeof(float));					
					drightweight = (float)((rightweight&0xffff)/10000.0f);					
			}
			else
			if(att & GRPANIMATIONCURVETANGENTMODE_TCB)
			{
					XDEBUG_PRINTCOLOR(3,__L("GRPANIMATIONCURVE:: Warning! Unsupported TCB Curve detected on node %lld %s on index %lld"),this->tag,this->name.Get(),e);
					return;
			}	
			else 
			{
				XQWORD q=att;
				if (!Warning)
				{
					XDEBUG_PRINTCOLOR(3,__L("GRPANIMATIONCURVE:: Warning! unknown flag type %lld on curve  %lld:%s on index %lld"),q,this->tag,this->name.Get(),e);
				}
				Warning=true;
				return;
			}
			
			//------------------------------------------------------------------------------------------- END SIDE			
			if(attNext & GRPANIMATIONCURVETANGENTMODE_AUTO)
			{			
				dnextleftslope		= 0.0f;
				dnextleftweight		= 0.33f;
				
				nextleftvelocity	=(int)keyattrdatafloat.FastGet(GRPANIMATIONCURVEFLAG_ORDER_NEXTLEFTVELOCITY + (int)offsetNext);	
				memcpy(&dnextleftvelocity	,	&nextleftvelocity	,	sizeof(float));		
			}
			else
			if(attNext & GRPANIMATIONCURVETANGENTMODE_USER)
			{					
				nextleftslope		= (int)keyattrdatafloat.FastGet(GRPANIMATIONCURVEFLAG_ORDER_NEXTLEFTSLOPE + (int)offsetNext);

				if(att & GRPANIMATIONCURVETANGENTMODE_WEIGHTEDNEXTLEFT)	
					nextleftweight	= (int)keyattrdatafloat.FastGet(GRPANIMATIONCURVEFLAG_ORDER_NEXTLEFTWEIGHT + (int)offsetNext);						
								
					memcpy(&dnextleftslope	, &nextleftslope  ,	sizeof(float));						
					memcpy(&dnextleftweight ,	&nextleftweight ,	sizeof(float));								
					dnextleftweight = (float)(((rightweight>>16)&0xffff)/10000.0f);					
			}
			else 
			{
				XQWORD q=att;
				if (!Warning)
				{
					XDEBUG_PRINTCOLOR(3,__L("GRPANIMATIONCURVE:: Warning! unknown flag type %lld on curve  %lld:%s on index %d"),q,this->tag,this->name.Get(),e);
				}
				Warning=true;
				return;
				continue;
			}
			
			//-------------------------------------- process Curve			
			GRPANCHORPOINT* _ini=new GRPANCHORPOINT(GRPANCHORPOINTTYPE_CUBIC_BEZIER);
			GRPANCHORPOINT* _end=new GRPANCHORPOINT(GRPANCHORPOINTTYPE_CUBIC_BEZIER);
				
			_ini->value.x	= (float)((ini/(float)GRPANIMATION_FBXSECOND)*30.0f);
			_ini->value.y  = (float)keyvalues.FastGet(e);
			_end->value.x	= (float)((end/(float)GRPANIMATION_FBXSECOND)*30.0f);
			_end->value.y	= (float)keyvalues.FastGet(e+1);
			
			GRPANCHORPOINT* anchora=new GRPANCHORPOINT(GRPANCHORPOINTTYPE_CUBIC_BEZIER);			
			GRPANCHORPOINT* anchorb=new GRPANCHORPOINT(GRPANCHORPOINTTYPE_CUBIC_BEZIER);

						
			double f=((double)longitude/(double)GRPANIMATION_FBXSECOND)*30.0;

			anchora->value.x =  (_ini->value.x  + ((float)f  * drightweight));		
			anchora->value.y =  (_ini->value.y  + (longitude * drightweight    * drightslope)/GRPANIMATION_FBXSECOND);
			anchorb->value.x =	(_end->value.x  -	((float)f  * dnextleftweight));
			anchorb->value.y =  (_end->value.y  - (longitude * dnextleftweight * dnextleftslope)/GRPANIMATION_FBXSECOND);

			GRPBEZIERCURVE* bs= new GRPBEZIERCURVE();

			if (!bs)
				return;

			GRPSPLINE sp;

			sp.points.Add(_ini);
					sp.points.Add(anchora);
					sp.points.Add(anchorb);
			sp.points.Add(_end);

			bs->Init(&sp);

			bcs.Add(bs);
	}		
		bcsSize=bcs.GetSize();

}

/*-------------------------------------------------------------------
//	GRPANIMATIONCURVE::GetAttribute
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/07/2015 12:33:16
//	
//	@return 			XQWORD : 
//
//  @param				index : 
//  @param				offset : 
*/
/*-----------------------------------------------------------------*/
XQWORD GRPANIMATIONCURVE::GetAttribute(XQWORD index,XQWORD* offset)
{
	XQWORD ret=0;

	int e=0;
	XQWORD i=index;

	if (this->keyattrrefcount.FastGet(e)==0)
	{
		*offset=0;
		return 0;
	}

	while (i>=this->keyattrrefcount.FastGet(e))
	{
		i-=this->keyattrrefcount.FastGet(e);			
		e++;		
	}

	if (e>=nkeyattrrefcount)
		ret= this->keyattributes.GetLast();
	else
		ret= this->keyattributes.FastGet(e);

	*offset=e*4;

	return ret;
}


/*-------------------------------------------------------------------
//	GRPANIMATIONCURVE::~GRPANIMATIONCURVE
*/	
/**	
//	
//	 Class Destructor GRPANIMATIONCURVE
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/05/2015 12:51:48
//	
*/
/*-----------------------------------------------------------------*/
GRPANIMATIONCURVE::~GRPANIMATIONCURVE()
{
	bcs.DeleteContents();
	bcs.DeleteAll();

	keytime.DeleteAll();
	keyvalues.DeleteAll();
	keyslopes.DeleteAll();
	keyattributes.DeleteAll();
	keyattrdatafloat.DeleteAll();
	UnpackedAttributes.DeleteAll();
}

/*-------------------------------------------------------------------
//	GRPANIMATIONCURVE::Update
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			11/02/2015 10:02:22
//	
//  @param				t : 
*/
/*-----------------------------------------------------------------*/
bool GRPANIMATIONCURVE::Update(XQWORDSIG t)
{
	if (!Active)				
		return false;

					
	
  if( t >= lasttime) 	
	{
		target = lastvalue;		
		if (nkeys==1)
			this->Active=false;
		return true;
	}
	else
	if( t <= firsttime)  
	{
		target = firstvalue;		
		if (nkeys == 1)
			this->Active=false;
		return true;
	}	
	else	
	{
		for(XDWORD e=currentkey; e<nkeys-1; e++)
				{
					XQWORDSIG vini=keytime.FastGet(e);		

					if(vini<=t)
					{
						XQWORDSIG vend=keytime.FastGet(e+1);		

						if(vend > t)
						{							
							initime=(float)vini;
							endtime=(float)vend;
							XQWORD keyattr = UnpackedAttributes.FastGet(e);

							if(keyattr & GRPANIMATIONCURVETANGENTMODE_CUBIC)
													UpdateAsCubic(t,e);
							else
							if(keyattr & GRPANIMATIONCURVETANGENTMODE_LINEAR)  								
													UpdateAsLinear(t,e);
							else
							if(keyattr & GRPANIMATIONCURVETANGENTMODE_CONSTANT)												
												UpdateAsConstant(t,e);	
							currentkey=e;
							return true;
						}

						XQWORD keyattr = UnpackedAttributes.FastGet(e);

						if((keyattr & GRPANIMATIONCURVETANGENTMODE_CONSTANT) && vend ==t)
						{
							if ((e+1)<nkeys)
								UpdateAsConstant(t,e+1);							
							return true;
						}
					}
				}
		for(int e=0; e<currentkey && e<int(nkeys)-1; e++)
				{
					XQWORDSIG vini=keytime.Get(e);
					if(vini<=t)
					{
					XQWORDSIG vend=keytime.Get(e+1);
					if (vend > t)
						{	
							initime=(double)vini;
							endtime=(double)vend;
							XQWORD keyattr =UnpackedAttributes.Get(e); 

							if(keyattr & GRPANIMATIONCURVETANGENTMODE_CUBIC)												
													UpdateAsCubic(t,e);
							else
							if(keyattr & GRPANIMATIONCURVETANGENTMODE_LINEAR)  								
													UpdateAsLinear(t,e);
							else
							if(keyattr & GRPANIMATIONCURVETANGENTMODE_CONSTANT)												
												UpdateAsConstant(t,e);	
							currentkey=e;
							return true;
						}
						
						XQWORD keyattr = UnpackedAttributes.Get(e);
						if((keyattr & GRPANIMATIONCURVETANGENTMODE_CONSTANT) && vend ==t)
						{
							if ((e+1)<int(nkeys))
							UpdateAsConstant(t,e+1);	
							return true;
						}
					}
				}
	}
	currentkey=0;

	return true;
}




/*-------------------------------------------------------------------
//	GRPANIMATIONCURVE::UpdateAsLinear
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/02/2015 9:33:31
//	
//  @param				t : 
//  @param				e : 
*/
/*-----------------------------------------------------------------*/
//void GRPANIMATIONCURVE::UpdateAsLinear(XQWORD t,int e, float ini, float end)
void GRPANIMATIONCURVE::UpdateAsLinear(XQWORDSIG t,XDWORD e)
{
	//if (endtime-initime)
	{
			double	vini				= keyvalues.FastGet(e);
			double	vend				= keyvalues.FastGet(e+1);	
			double	m						= (vend - vini) / (endtime - initime);
							target			= (float)(vini + ((t - initime)*m));
/*
		const double	vini				=	keyvalues.FastGet(e);		
		const	double	m						= keyslopes.FastGet(e);		
								  target			=	(float)(vini + ((t - initime)*m));
									*/
	}
}




/*-------------------------------------------------------------------
//	GRPANIMATIONCURVE::UpdateAsCubic
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/02/2015 9:33:37
//	
//  @param				t : 
//  @param				e : 
*/
/*-----------------------------------------------------------------*/
//void GRPANIMATIONCURVE::UpdateAsCubic(XQWORD t,int e,float initime, float endtime)
void GRPANIMATIONCURVE::UpdateAsCubic(XQWORDSIG t,XDWORD e)
{			
		  const double length = endtime - initime;
			if (length ==0) return;
					
			
			const float st= (float)((t - initime) / length);						
			
			if (this->bcs.Get(e))
			//target=this->bcs.FastGet(e)->InterpolateBezierAmplitude(st);			
				target=this->bcs.FastGet(e)->InterpolateBezier(st).y;			
}

/*-------------------------------------------------------------------
//	GRPANIMATIONNODE::GRPANIMATIONNODE
*/	
/**	
//	
//	Class Constructor GRPANIMATIONNODE
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:48:15
//	
*/
/*-----------------------------------------------------------------*/
GRPANIMATIONNODE::GRPANIMATIONNODE()																							
{ 
	Clean();																							
	GRPNAMED::classtype = GRPNAMED_TYPE_GRPANIMATIONNODE;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONNODE::~GRPANIMATIONNODE
*/	
/**	
//	
//	 Class Destructor GRPANIMATIONNODE
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/05/2015 12:52:55
//	
*/
/*-----------------------------------------------------------------*/
GRPANIMATIONNODE::~GRPANIMATIONNODE() 
{
	CurveConnections.DeleteAll();
	curves.DeleteAll();
	DefaultNodeValues.DeleteAll();	
}

/*-------------------------------------------------------------------
//	GRPANIMATIONNODE::GetType
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:50:40
//	
//	@return 			GRPANIMATIONCURVETYPE : 
//
*/
/*-----------------------------------------------------------------*/
GRPANIMATIONCURVETYPE GRPANIMATIONNODE::GetType()																							
{ 
	return type;																					
}

/*-------------------------------------------------------------------
//	GRPANIMATIONNODE::SetType
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:58:41
//	
//  @param				type : 
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONNODE::SetType(GRPANIMATIONCURVETYPE type)										
{ 
	this->type = type;																		
}		

/*-------------------------------------------------------------------
//	GRPANIMATIONNODE::GetCurves
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:58:46
//	
//	@return 			XVECTOR<GRPANIMATIONCURVE*>* : 
//
*/
/*-----------------------------------------------------------------*/
XVECTOR<GRPANIMATIONCURVE*>* GRPANIMATIONNODE::GetCurves()																							
{ 
	return &curves;
}	

/*-------------------------------------------------------------------
//	GRPANIMATIONNODE::GetTarget
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:58:50
//	
//	@return 			GRPINTERFACEANIMABLE* : 
//
*/
/*-----------------------------------------------------------------*/
GRPINTERFACEANIMABLE* GRPANIMATIONNODE::GetTarget()																							
{ 
	return target;																				
}

/*-------------------------------------------------------------------
//	GRPANIMATIONNODE::SetTarget
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:58:57
//	
//  @param				target : 
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONNODE::SetTarget(GRPINTERFACEANIMABLE* target)									
{ 
	this->target = target;																
}	


/*-------------------------------------------------------------------
//	GRPANIMATIONNODE::GetTargetTag
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/11/2015 9:52:45
//	
//	@return 			XQWORD : 
//
*/
/*-----------------------------------------------------------------*/
XQWORD GRPANIMATIONNODE::GetTargetTag		()
{
	return this->targetTag;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONNODE::SetTargetTag
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/11/2015 9:53:24
//	
//  @param				tag : 
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONNODE::SetTargetTag			(XQWORD	tag)
{
	this->targetTag=tag;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONNODE::GetTargetName
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/11/2015 9:53:30
//	
//	@return 			XSTRING* : 
//
*/
/*-----------------------------------------------------------------*/
XSTRING* GRPANIMATIONNODE::GetTargetName()
{
	return &this->targetName;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONNODE::SetTargetName
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/11/2015 9:53:34
//	
//  @param				name : 
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONNODE::SetTargetName		(XCHAR*	name)
{
	this->targetName.Set(name);
}

/*-------------------------------------------------------------------
//	GRPANIMATIONNODE::GetDefaultValues
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:59:02
//	
//	@return 			XMAP<XDWORD,GLFLOAT>* : 
//
*/
/*-----------------------------------------------------------------*/
XMAP<XDWORD,GLFLOAT>* GRPANIMATIONNODE::GetDefaultValues()																							
{ 
	return &DefaultNodeValues;														
}	

/*-------------------------------------------------------------------
//	GRPANIMATIONNODE::SetLayer
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:59:10
//	
//  @param				layer : 
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONNODE::SetLayer(GRPANIMATIONLAYER* layer)											
{ 
	this->layer=layer;																		
}

/*-------------------------------------------------------------------
//	GRPANIMATIONNODE::GetLayer
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:59:18
//	
//	@return 			GRPANIMATIONLAYER* : 
//
*/
/*-----------------------------------------------------------------*/
GRPANIMATIONLAYER* GRPANIMATIONNODE::GetLayer()																							
{
	return this->layer;																		
}

/*-------------------------------------------------------------------
//	GRPANIMATIONNODE::GetCurveConnections
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:59:23
//	
//	@return 			XMAP<XDWORD,GRPANIMATIONCURVE*>* : 
//
*/
/*-----------------------------------------------------------------*/
XMAP<XDWORD,GRPANIMATIONCURVE*>* GRPANIMATIONNODE::GetCurveConnections()																							
{ 
	return &this->CurveConnections;												
}
/*-------------------------------------------------------------------
//	GRPANIMATIONNODE::GetOffsets
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			31/12/2015 10:37:43
//	
//	@return 			XMAP<XDWORD,float>* : 
//
*/
/*-----------------------------------------------------------------*/
XMAP<XDWORD,float>*	 GRPANIMATIONNODE::GetOffsets()
{
	return &this->OffsetValues;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONNODE::Update
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			11/02/2015 10:02:17
//	
//  @param				t : 
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONNODE::Update(XQWORDSIG t)
{
	if(!target)	 			
		return;
		
	int nConnections=this->CurveConnections.GetSize();	

	XDWORD defaultnodessize = this->DefaultNodeValues.GetSize();
	for (XDWORD e=0;e<defaultnodessize;e++)
	{
		GRPANIMATIONCURVEAXIS axis=(GRPANIMATIONCURVEAXIS)DefaultNodeValues.FastGetKey(e);
		double value=DefaultNodeValues.FastGetElement(e);
		target->SetValue(type, axis, value, 0.0f);													
	}

	XDWORD ntargets = AdditionalTargets.GetSize();
	for (int e=0;e<nConnections;e++)
		{		
			GRPANIMATIONCURVE* curve	= CurveConnections.FastGetElement(e);
		
			if(curve && curve->Active)
				{
					GRPANIMATIONCURVEAXIS axis	= (GRPANIMATIONCURVEAXIS)CurveConnections.FastGetKey(e);					
					double newValue							= curve->target;
					double baseValue						= this->DefaultNodeValues.Get	(axis);
					double offsetValue					= this->OffsetValues.Get			(axis);	
					
					if (ntargets>0)
					{
						for (XDWORD e=0;e<ntargets;e++)
							AdditionalTargets.Get(e)->SetValue(type,axis,newValue+offsetValue, baseValue);
					}

					if (target->SetValue(type, axis, newValue+offsetValue, baseValue)==false)											
						curve->Active=false;									
				}
		}
}



/*-------------------------------------------------------------------
//	GRPANIMATIONNODE::AddCurve
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			08/07/2015 13:45:39
//	
//  @param				curve : 
*/
/*-----------------------------------------------------------------*/
bool GRPANIMATIONNODE::AddConnection(GRPANIMATIONCURVEAXIS axis,GRPANIMATIONCURVE* curve)
{
	if (this->layer)
		this->layer->AddCurve(curve);
	/*
	if (this->mintime> curve->firsttime)
		this->mintime=curve->firsttime;

	if (this->maxtime> curve->lasttime)
		this->maxtime=curve->lasttime;
		*/
	return this->CurveConnections.Add(axis,curve);	
}

/*-------------------------------------------------------------------
//	GRPANIMATIONNODE::AttachTo
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			24/09/2015 17:14:23
//	
//	@return 			bool : 
//
//  @param				name : 
//  @param				animable : 
*/
/*-----------------------------------------------------------------*/
bool GRPANIMATIONNODE::AttachTo			(XCHAR* name,GRPINTERFACEANIMABLE* animable)
{
	bool status=false;
	if (animable!=NULL)
	for (XDWORD e=0;e<this->CurveConnections.GetSize();e++)
	{
		if (this->CurveConnections.GetElement(e))
		if (this->CurveConnections.GetElement(e)->GetName())
		if (this->CurveConnections.GetElement(e)->GetName()->Find(name,false)!=NOTFOUND)
		{
			this->GetLayer()->GetStack()->GetAnimationControl()->AttachAnimation(animable,this);			
			//this->target=animable;			
			this->AdditionalTargets.Add(animable);
			status=true;
		}
	}


	return status;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONNODE::GetMinFrame
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			25/11/2015 10:46:02
//	
//	@return 			XQWORD : 
//
*/
/*-----------------------------------------------------------------*/
XQWORDSIG GRPANIMATIONNODE::GetMinTime()
{
	return this->mintime;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONNODE::GetMaxFrame
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			25/11/2015 10:46:15
//	
//	@return 			XQWORD : 
//
*/
/*-----------------------------------------------------------------*/
XQWORDSIG GRPANIMATIONNODE::GetMaxTime()
{
	return this->maxtime;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONNODE::SetMinFrame
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			25/11/2015 11:20:54
//	
//	@param 			XQWORD : 
//
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONNODE::SetMinTime(XQWORDSIG f)
{
	this->mintime=f;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONNODE::SetMaxFrame
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			25/11/2015 11:20:59
//	
//	@param 			XQWORD : 
//
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONNODE::SetMaxTime(XQWORDSIG f)
{
	this->maxtime=f;
}


/*-------------------------------------------------------------------
//	GRPANIMATIONLAYER::GRPANIMATIONLAYER
*/	
/**	
//	
//	Class Constructor GRPANIMATIONLAYER
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/05/2015 12:54:02
//	
*/
/*-----------------------------------------------------------------*/
GRPANIMATIONLAYER::GRPANIMATIONLAYER()
{
 
	Clean();

	active					=	true;
	synced					=	false;
	laststacktime		=	0;
	speedmultiplier =	1.0f;
	loopmode				=	GRPANIMATIONLOOPMODE_REPEAT;
	status					=	GRPANIMATION_STATUS_PLAYING;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONLAYER::~GRPANIMATIONLAYER
*/	
/**	
//	
//	 Class Destructor GRPANIMATIONLAYER
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/05/2015 12:53:58
//	
*/
/*-----------------------------------------------------------------*/
GRPANIMATIONLAYER::~GRPANIMATIONLAYER()
{		
	animationnodes.DeleteContents();
	animationnodes.DeleteAll();

	curves.DeleteContents();
	curves.DeleteAll();
}

/*-------------------------------------------------------------------
//	GRPANIMATIONLAYER::GetStatus
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:31:31
//	
//	@return 			XDWORD : 
//
*/
/*-----------------------------------------------------------------*/
XDWORD GRPANIMATIONLAYER::GetStatus()																							
{
	return status;																					
}

/*-------------------------------------------------------------------
//	GRPANIMATIONLAYER::GetIsFinished
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:31:38
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPANIMATIONLAYER::GetIsFinished()																							
{
	return (maxtime <= localtime);
}

/*-------------------------------------------------------------------
//	GRPANIMATIONLAYER::SetMaxFrame
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:31:45
//	
//  @param				maxframe : 
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONLAYER::SetMaxTime(XQWORDSIG maxframe)																
{	
	this->maxtime = maxframe;															
}

/*-------------------------------------------------------------------
//	GRPANIMATIONLAYER::SetMinFrame
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			25/11/2015 10:35:29
//	
//  @param				minframe : 
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONLAYER::SetMinTime(XQWORDSIG minTime)
{
	this->mintime=minTime;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONLAYER::IsSync
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:31:50
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPANIMATIONLAYER::IsSync()																							
{ 
	return synced;																					
}

/*-------------------------------------------------------------------
//	GRPANIMATIONLAYER::SetSync
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:31:57
//	
//  @param				synced : 
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONLAYER::SetSync(bool synced)																		
{	
	this->synced = synced;																	
}

/*-------------------------------------------------------------------
//	GRPANIMATIONLAYER::IsActive
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:32:02
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPANIMATIONLAYER::IsActive()																							
{ 
	return active;																					
}

/*-------------------------------------------------------------------
//	GRPANIMATIONLAYER::SetActive
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:32:10
//	
//  @param				active : 
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONLAYER::SetActive(bool active)																		
{ 
	this->active = active;																	
}

/*-------------------------------------------------------------------
//	GRPANIMATIONLAYER::IsFinished
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			26/11/2015 15:43:25
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPANIMATIONLAYER::IsFinished()
{
	return finished;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONLAYER::SetIsFinished
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			26/11/2015 15:43:21
//	
//  @param				finished : 
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONLAYER::SetIsFinished(bool finished)
{
	this->finished=finished;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONLAYER::GetSpeedMultiplier
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:32:16
//	
//	@return 			float : 
//
*/
/*-----------------------------------------------------------------*/
float GRPANIMATIONLAYER::GetSpeedMultiplier()																							
{ 
	return speedmultiplier;																	
}

/*-------------------------------------------------------------------
//	GRPANIMATIONLAYER::SetSpeedMultiplier
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:33:34
//	
//  @param				speedmultipler : 
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONLAYER::SetSpeedMultiplier(float speedmultiplier)
{
	this->speedmultiplier = speedmultiplier;								
}

/*-------------------------------------------------------------------
//	GRPANIMATIONLAYER::SetLoopMode
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:34:09
//	
//  @param				loopmode : 
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONLAYER::SetLoopMode(GRPANIMATIONLOOPMODE loopmode)
{ 
	this->loopmode = loopmode;															
}

/*-------------------------------------------------------------------
//	GRPANIMATIONLAYER::GetLoopMode
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:34:16
//	
//	@return 			GRPANIMATIONLOOPMODE : 
//
*/
/*-----------------------------------------------------------------*/
GRPANIMATIONLOOPMODE GRPANIMATIONLAYER::GetLoopMode()
{
	return loopmode;																				
}

/*-------------------------------------------------------------------
//	GRPANIMATIONLAYER::Stop
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:34:22
//	
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONLAYER::Stop()												
{ 	
	SetActive	(false);		
	SetSync		(false); 
}	

/*-------------------------------------------------------------------
//	GRPANIMATIONLAYER::Play
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:34:27
//	
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONLAYER::Play()												
{	
	finished=false;
	this->status=GRPANIMATION_STATUS_PLAYING;
	//Rewind();
}

/*-------------------------------------------------------------------
//	GRPANIMATIONLAYER::Pause
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:34:40
//	
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONLAYER::Pause	()
{
	this->status=GRPANIMATION_STATUS_PAUSED;
	SetSync (false);	
	finished=false;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONLAYER::Rewind
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			25/11/2015 14:33:06
//	
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONLAYER::Rewind ()
{
	finished=false;
	this->localtime=this->mintime;	

	for (XDWORD e=0;e<this->animationnodes.GetSize();e++)
	{
		for (XDWORD i=0;i<this->animationnodes.Get(e)->GetDefaultValues()->GetSize();i++)
		{
			this->animationnodes.Get(e)->GetTarget()->SetValue(
																this->animationnodes.Get(e)->GetType(),
				(GRPANIMATIONCURVEAXIS)	this->animationnodes.Get(e)->GetDefaultValues()->GetKey(i),
																this->animationnodes.Get(e)->GetDefaultValues()->GetElement(i),
																0.0f);
		}
	}

	if (this->localtime<stack->GetMinFrame())
		this->localtime=stack->GetMinFrame();


	if (this->status==GRPANIMATION_STATUS_FINISHED)
		this->status=GRPANIMATION_STATUS_PLAYING;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONLAYER::Forward
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			10/12/2015 14:54:33
//	
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONLAYER::Forward()
{
	finished=false;
	this->localtime=this->maxtime;

	if (this->status==GRPANIMATION_STATUS_FINISHED)
		this->status=GRPANIMATION_STATUS_PLAYING;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONLAYER::Reverse
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			11/12/2015 17:26:39
//	
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONLAYER::Reverse()
{
	if (this->direction==GRPANIMATIONPLAYDIRECTION_FORWARD)
		this->direction=GRPANIMATIONPLAYDIRECTION_BACKWARD;
	else
		this->direction=GRPANIMATIONPLAYDIRECTION_FORWARD;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONLAYER::Update
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			03/03/2015 13:46:53
//	
//  @param				t : 
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONLAYER::Update(XQWORDSIG t)
{		
	//---------------- is playing?
	if(active)
		{				
			if (this->status==GRPANIMATION_STATUS_PLAYING)
			{				
				laststacktime  = localtime;	

				if (direction==GRPANIMATIONPLAYDIRECTION_FORWARD)
					localtime     += XQWORD(t*this->speedmultiplier);		
				else
				{
					if (localtime > t)
							localtime  -= XQWORD(t*this->speedmultiplier);		
					else
							localtime=0;
				}		

					finished=false;
			}

					XDWORD n = curves.GetSize();
					for(int e=n-1; e>=0; e--)	
					{
							GRPANIMATIONCURVE* curve=curves.FastGet(e);
							if (curve)
							{
									if (curve->maxtime != curve->lasttime)
									{
											if (localtime > curve->maxtime)
													curve->Update(curve->maxtime);
											else
													curve->Update(localtime);
									}
									else
										curve->Update(localtime);
							}
					}
						
							n = animationnodes.GetSize();
					for(int e=n-1; e>=0; e--)	
					{
							GRPANIMATIONNODE* node=animationnodes.FastGet(e);
							if (node)
								node->Update(localtime);					
					}

					XQWORDSIG	max=0;
					XQWORDSIG	min=0;

					if (stack)
					{						
						max = stack->GetMaxTime();
						min = stack->GetMinTime();
					}

					if(localtime >= max)
					{						
						if (direction==GRPANIMATIONPLAYDIRECTION_FORWARD)
						{
							if (this->loopmode==GRPANIMATIONLOOPMODE_REPEAT)								
							{ 																
									XQWORDSIG tmp=this->localtime;
									Rewind();	
									localtime=tmp;
									//localtime=mintime+(localtime-maxtime);															
									localtime=min+(localtime-max);															
										
							}
							else
							if (this->loopmode==GRPANIMATIONLOOPMODE_PINGPONG)							
							{
									this->direction=GRPANIMATIONPLAYDIRECTION_BACKWARD;
									localtime=max-(min-localtime);
							}
							else
									localtime=max;
						}

						finished=true;
					}		


					if(localtime <= min)
					{						
						if (direction==GRPANIMATIONPLAYDIRECTION_BACKWARD)
						{
							if (this->loopmode==GRPANIMATIONLOOPMODE_REPEAT)								
							{
									Forward();		
									localtime=max-(min-localtime);
									while (localtime>=max)									
										localtime=min+(localtime-max);
							}

							if (this->loopmode==GRPANIMATIONLOOPMODE_PINGPONG)
							{
								this->direction=GRPANIMATIONPLAYDIRECTION_FORWARD;								
							}
						}
						else
							localtime=min;
					}								
		}

}
	


/*-------------------------------------------------------------------
//	GRPANIMATIONLAYER::AddAnimationNode
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:34:55
//	
//  @param				node : 
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONLAYER::AddAnimationNode(GRPANIMATIONNODE* node)												
{ 	
	if (node)
	{
	animationnodes.Add(node);				
	node->SetLayer(this);
	}
}

/*-------------------------------------------------------------------
//	GRPANIMATIONLAYER::AddCurve
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:35:02
//	
//  @param				curve : 
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONLAYER::AddCurve(GRPANIMATIONCURVE* curve)
{
	if (curve)
	curves.Add(curve);	
	
	if (curve->firsttime<this->mintime)
		this->SetMinTime(curve->firsttime);	

	if (curve->lasttime>this->maxtime)
		this->SetMaxTime(curve->lasttime);		
		
}

/*-------------------------------------------------------------------
//	GRPANIMATIONLAYER::GetCurves
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:35:15
//	
//	@return 			XVECTOR<GRPANIMATIONCURVE*>* : 
//
*/
/*-----------------------------------------------------------------*/
XVECTOR<GRPANIMATIONCURVE*>* GRPANIMATIONLAYER::GetCurves()
{
	return &this->curves;																		
}

/*-------------------------------------------------------------------
//	GRPANIMATIONLAYER::GetAnimationNodes
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:35:33
//	
//	@return 			XVECTOR<GRPANIMATIONNODE*>* : 
//
*/
/*-----------------------------------------------------------------*/
XVECTOR<GRPANIMATIONNODE*>*	 GRPANIMATIONLAYER::GetAnimationNodes()
{ 
	return &animationnodes;																	
}

/*-------------------------------------------------------------------
//	GRPANIMATIONLAYER::SetAnimationNodes
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:35:59
//	
//  @param				animationnodes : 
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONLAYER::SetAnimationNodes(XVECTOR<GRPANIMATIONNODE*> animationnodes)			
{	
	this->animationnodes = animationnodes; 									
}

/*-------------------------------------------------------------------
//	GRPANIMATIONLAYER::SetFrame
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/11/2015 10:32:02
//	
//  @param				frame : 
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONLAYER::SetFrame(XQWORDSIG frame)
{	
	GRPANIMATIONCONTROL* ac=this->GetStack()->GetAnimationControl();

		double	maxtime						=	double(this->GetStack()->GetMaxTime());
		XQWORDSIG	totalframes				=	0;

		double	fps								=	ac->GetFramesPerSecondByMode();
						totalframes				=	(XQWORDSIG)(((maxtime*fps)/GRPANIMATION_FBXSECOND));							
						if (totalframes==0)
							totalframes=1;		

						if (frame > totalframes)
							frame=totalframes;
		
		XQWORDSIG	newtime						=	(XQWORDSIG)(maxtime*(double(frame)/double(totalframes)));		
						
						if (newtime > (XQWORDSIG)maxtime)
							newtime=(XQWORDSIG)maxtime;
						if (newtime < (XQWORDSIG)mintime)
							newtime=(XQWORDSIG)mintime;	

						localtime					=	newtime;
						laststacktime			= newtime;

		bool		pre								=	this->active;
						this->active			=	true;
						this->Update(0);
						this->active			=	pre;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONLAYER::GetFrame
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			05/07/2017 16:52:33
//	
//	@return 			XQWORDSIG : 
//
*/
/*-----------------------------------------------------------------*/
XQWORDSIG GRPANIMATIONLAYER::GetFrame()
{	
	return this->stack->GetFrameOfTime(this->localtime);;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONLAYER::SetStack
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/11/2015 10:46:30
//	
//	@return 			bool : 
//
//  @param				stack : 
*/
/*-----------------------------------------------------------------*/
bool GRPANIMATIONLAYER::SetStack(GRPANIMATIONSTACK* stack)
{
	if (stack==NULL)
		return false;

	this->stack=stack;
		return true;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONLAYER::GetStack
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/11/2015 16:49:54
//	
//	@return 			GRPANIMATIONSTACK* : 
//
*/
/*-----------------------------------------------------------------*/
GRPANIMATIONSTACK* GRPANIMATIONLAYER::GetStack()
{
	return this->stack;
}


/*-------------------------------------------------------------------
//	GRPANIMATIONLAYER::SetStatus
*/	
/**	
//	
//	Sets layers play state
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			24/11/2015 11:12:23
//	
//  @param				status : 
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONLAYER::SetStatus(GRPANIMATION_STATUS status)
{
	this->status=status;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONLAYER::GetMinFrame
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			25/11/2015 10:32:44
//	
//	@return 			XQWORD : 
//
*/
/*-----------------------------------------------------------------*/
XQWORDSIG GRPANIMATIONLAYER::GetMinTime()
{
	return this->mintime;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONLAYER::GetMaxFrame
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			25/11/2015 10:32:49
//	
//	@return 			XQWORD : 
//
*/
/*-----------------------------------------------------------------*/
XQWORDSIG GRPANIMATIONLAYER::GetMaxTime()
{
	return this->maxtime;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONLAYER::SetLocalFrame
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			11/12/2015 10:47:41
//	
//  @param				localframe : 
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONLAYER::SetLocalTime	(XQWORDSIG localtime)																					
{
	this->localtime = localtime;					
}

/*-------------------------------------------------------------------
//	GRPANIMATIONLAYER::SetLastStackFrame
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			11/12/2015 10:47:49
//	
//  @param				laststacktime : 
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONLAYER::SetLastStackTime(XQWORDSIG laststacktime)																			
{ 
	this->laststacktime = laststacktime;	
}

/*-------------------------------------------------------------------
//	GRPANIMATIONLAYER::GetLocalFrame
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			11/12/2015 10:47:55
//	
//	@return 			XQWORD : 
//
*/
/*-----------------------------------------------------------------*/
XQWORDSIG	GRPANIMATIONLAYER::GetLocalTime()																													
{ 
	return this->localtime;								
}

/*-------------------------------------------------------------------
//	GRPANIMATIONLAYER::GetLastStackFrame
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			11/12/2015 10:48:00
//	
//	@return 			XQWORD : 
//
*/
/*-----------------------------------------------------------------*/
XQWORDSIG	GRPANIMATIONLAYER::GetLastStackTime()																													
{ 
	return this->laststacktime;						
}


/*-------------------------------------------------------------------
//	GRPANIMATIONSTACKMILESTONE::SetPasses
*/	
/**	
//	number of times the animation can traspass the milestone without activation
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			11/12/2015 10:49:51
//	
//  @param				p : 
*/
/*-----------------------------------------------------------------*/
void	GRPANIMATIONSTACKMILESTONE::SetPasses			(int p)																	
{		
	passes=p;															
}		

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACKMILESTONE::SetCurrentPass
*/	
/**	
//	
//number of times the animation has traspassed this milestone	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			11/12/2015 10:50:01
//	
//  @param				p : 
*/
/*-----------------------------------------------------------------*/
void	GRPANIMATIONSTACKMILESTONE::SetCurrentPass(int p)																	
{		
	currentpass=p;												
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACKMILESTONE::SetTargetFrame
*/	
/**	
//	
//		target frame	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			11/12/2015 10:50:23
//	
//  @param				f : 
*/
/*-----------------------------------------------------------------*/
void	GRPANIMATIONSTACKMILESTONE::SetStartFrame  (float f)																
{		
	start_frame=f;
	if(stack)
	{
		this->SetStartTime();
	}
}					

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACKMILESTONE::SetEndFrame
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			14/12/2015 11:35:19
//	
//  @param				f : 
*/
/*-----------------------------------------------------------------*/
void	GRPANIMATIONSTACKMILESTONE::SetEndFrame  (float f)																
{		
	end_frame=f;
	if(stack)
	{
		this->SetEndTime();
	}
}	




/*-------------------------------------------------------------------
//	GRPANIMATIONSTACKMILESTONE::SetStartFrameWithOffset
*/	
/**	
//	
//	sets the milestone frame taking the offset into account
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			21/01/2016 14:24:19
//	
//  @param				f : 
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONSTACKMILESTONE::SetStartFrameWithOffset(float f)
{
	if(stack)
	{
		// minimum frame and add it with f
		SetStartFrame(stack->GetFrameOfTime(stack->GetMinFrame()) + f);
	}
	else
	{
		// if the stack is not present not sure what to do
		SetStartFrame(f);
	}
}





/*-------------------------------------------------------------------
//	GRPANIMATIONSTACKMILESTONE::SetEndFrameWithOffset
*/	
/**	
//	
//	sets the ending time taking the offset into account
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			21/01/2016 14:24:36
//	
//  @param				f : 
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONSTACKMILESTONE::SetEndFrameWithOffset(float f)
{
	if(stack)
	{
		// minimum frame and add it with f
		SetEndFrame(stack->GetFrameOfTime(stack->GetMinFrame()) + f);
	}
	else
	{
		SetEndFrame(f);
	}
}



/*-------------------------------------------------------------------
//	GRPANIMATIONSTACKMILESTONE::SetTension
*/	
/**	
//	
//	spring tension	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			11/12/2015 10:50:40
//	
//  @param				t : 
*/
/*-----------------------------------------------------------------*/
void	GRPANIMATIONSTACKMILESTONE::SetTension	(float t)																
{		
	tension=t;														
}				


/*-------------------------------------------------------------------
//	GRPANIMATIONSTACKMILESTONE::SetType
*/	
/**	
//	
//		spring tension	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			11/12/2015 10:50:58
//	
//  @param				t : 
*/
/*-----------------------------------------------------------------*/
void	GRPANIMATIONSTACKMILESTONE::SetType			(XDWORD t)		
{		
	type=(GRPANIMATIONSTACKMILESTONE::TYPE)t;																
}						


/*-------------------------------------------------------------------
//	GRPANIMATIONSTACKMILESTONE::SetStack
*/	
/**	
//	
//	stack
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			11/12/2015 10:51:29
//	
//  @param				s : 
*/
/*-----------------------------------------------------------------*/
void	GRPANIMATIONSTACKMILESTONE::SetStack		(GRPANIMATIONSTACK*	s)									
{		
	stack=s;		
	SetStartTime();	
	SetEndTime();	
	
		durationtime=end_time-start_time;
		durationframes=end_frame-start_frame;	
}		

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACKMILESTONE::SetStartTime
*/	
/**	
//	real time frame
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			11/12/2015 10:51:35
//	
*/
/*-----------------------------------------------------------------*/
void	GRPANIMATIONSTACKMILESTONE::SetStartTime			()
{		
	start_time=stack->GetTimeOfFrame((int)start_frame);		
}		
/*-------------------------------------------------------------------
//	GRPANIMATIONSTACKMILESTONE::SetTime
*/	
/**	
//	real time frame
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			11/12/2015 10:51:35
//	
*/
/*-----------------------------------------------------------------*/
void	GRPANIMATIONSTACKMILESTONE::SetEndTime			()
{		
	end_time=stack->GetTimeOfFrame((int)end_frame);		
}		
/*-------------------------------------------------------------------
//	GRPANIMATIONSTACKMILESTONE::GetPasses
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			11/12/2015 10:57:37
//	
//	@return 			int : 
//
*/
/*-----------------------------------------------------------------*/
int	GRPANIMATIONSTACKMILESTONE::GetPasses	()
{
	return passes;
}


/*-------------------------------------------------------------------
//	GRPANIMATIONSTACKMILESTONE::GetCurrentPass
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			11/12/2015 10:57:47
//	
//	@return 			int : 
//
*/
/*-----------------------------------------------------------------*/
int	GRPANIMATIONSTACKMILESTONE::GetCurrentPass()
{
	return currentpass;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACKMILESTONE::GetFrame
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			11/12/2015 10:57:52
//	
//	@return 			float : 
//
*/
/*-----------------------------------------------------------------*/
float	GRPANIMATIONSTACKMILESTONE::GetStartFrame()
{
	return start_frame;
}


/*-------------------------------------------------------------------
//	GRPANIMATIONSTACKMILESTONE::GetEndFrame
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			15/12/2015 13:04:19
//	
//	@return 			float : 
//
*/
/*-----------------------------------------------------------------*/
float	GRPANIMATIONSTACKMILESTONE::GetEndFrame()
{
	return end_frame;
}
/*-------------------------------------------------------------------
//	GRPANIMATIONSTACKMILESTONE::GetStartTime
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			14/12/2015 11:39:21
//	
//	@return 			XQWORD : 
//
*/
/*-----------------------------------------------------------------*/
XQWORD	GRPANIMATIONSTACKMILESTONE::GetStartTime()
{
		return start_time;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACKMILESTONE::GetEndTime
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			14/12/2015 11:39:16
//	
//	@return 			XQWORD : 
//
*/
/*-----------------------------------------------------------------*/
XQWORD	GRPANIMATIONSTACKMILESTONE::GetEndTime()
{
		return end_time;
}


/*-------------------------------------------------------------------
//	GRPANIMATIONSTACKMILESTONE::GetTension
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			11/12/2015 10:58:15
//	
//	@return 			float : 
//
*/
/*-----------------------------------------------------------------*/
float		GRPANIMATIONSTACKMILESTONE::GetTension()
{
	return tension;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACKMILESTONE::GetType
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			11/12/2015 11:02:23
//	
//	@return 			GRPANIMATIONSTACKMILESTONE::TYPE : 
//
*/
/*-----------------------------------------------------------------*/
XDWORD	GRPANIMATIONSTACKMILESTONE::GetType()
{
	return type;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACKMILESTONE::GetStack
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			11/12/2015 11:02:32
//	
//	@return 			GRPANIMATIONSTACK* : 
//
*/
/*-----------------------------------------------------------------*/
GRPANIMATIONSTACK*	GRPANIMATIONSTACKMILESTONE::GetStack()
{
	return stack;
}


/*-------------------------------------------------------------------
//	GRPANIMATIONSTACKMILESTONE::GetDurationFrames
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			14/12/2015 11:46:51
//	
//	@return 			float : 
//
*/
/*-----------------------------------------------------------------*/
float	GRPANIMATIONSTACKMILESTONE::GetDurationFrames	()
{
	return durationframes;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACKMILESTONE::GetDurationTime
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			14/12/2015 11:46:56
//	
//	@return 			XQWORD : 
//
*/
/*-----------------------------------------------------------------*/
XQWORD	GRPANIMATIONSTACKMILESTONE::GetDurationTime	()
{
	return durationtime;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACKMILESTONE::Enable
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			14/12/2015 17:28:10
//	
//  @param				isenabled : 
*/
/*-----------------------------------------------------------------*/
void	GRPANIMATIONSTACKMILESTONE::Enable(bool isenabled)
{
	if (this->type==GRPANIMATIONSTACKMILESTONE::NOTIFY)
			disabled=false;
	else
	{
		disabled=!isenabled;
		this->DoDisable(!isenabled);
		currentpass=0;
	}
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACKMILESTONE::GetIsActive
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			16/12/2015 11:24:20
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool	GRPANIMATIONSTACKMILESTONE::GetIsActive()
{
	return this->isactive;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACKMILESTONE::GetIsDisabled
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			16/12/2015 14:02:46
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool	GRPANIMATIONSTACKMILESTONE::GetIsDisabled()
{
	return this->disabled;
}


/*-------------------------------------------------------------------
//	GRPANIMATIONSTACKMILESTONE::GetCurrentLocalFrame
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/12/2015 14:37:02
//	
//	@return 			XQWORD : 
//
*/
/*-----------------------------------------------------------------*/
XQWORD	GRPANIMATIONSTACKMILESTONE::GetCurrentLocalTime()
{
	return milestonelocaltime;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACKMILESTONE::GetCurrentLocalFrame
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/12/2015 14:37:02
//	
//	@return 			XQWORD : 
//
*/
/*-----------------------------------------------------------------*/
XQWORD	GRPANIMATIONSTACKMILESTONE::GetLastLocalTime()
{
	return milestonelasttime;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACKMILESTONE::SetMilestoneLocalTime
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			25/01/2016 13:04:43
//	
//  @param				nowtime : 
*/
/*-----------------------------------------------------------------*/
void	GRPANIMATIONSTACKMILESTONE::SetMilestoneLocalTime	(XQWORD nowtime)
{
	milestonelocaltime=nowtime;
}


/*-------------------------------------------------------------------
//	GRPANIMATIONSTACKMILESTONE::SetMilestoneLastTime
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			25/01/2016 13:04:38
//	
//  @param				lasttime : 
*/
/*-----------------------------------------------------------------*/
void	GRPANIMATIONSTACKMILESTONE::SetMilestoneLastTime	(XQWORD lasttime)
{
	milestonelasttime=lasttime;
}



/*-------------------------------------------------------------------
//	GRPANIMATIONSTACKMILESTONE::DoEnter
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/12/2015 14:14:41
//	
//  @param				t : 
*/
/*-----------------------------------------------------------------*/
bool	GRPANIMATIONSTACKMILESTONE::DoEnter		(XQWORD t)
{	
	if (this->Publish(GRPMSG_ENTER)!=0)
		this->OnEnter(t);

	return true;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACKMILESTONE::DoContinue
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/12/2015 14:14:33
//	
//  @param				t : 
*/
/*-----------------------------------------------------------------*/
bool	GRPANIMATIONSTACKMILESTONE::DoContinue(XQWORD t)
{
	if (this->Publish(GRPMSG_CONTINUE)!=0)
		this->OnContinue(t);

	return true;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACKMILESTONE::DoExit
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/12/2015 14:14:28
//	
//  @param				t : 
*/
/*-----------------------------------------------------------------*/
bool	GRPANIMATIONSTACKMILESTONE::DoExit		(XQWORD t)
{
	if (this->Publish(GRPMSG_EXIT)!=0)
		this->OnExit(t);
	return true;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACKMILESTONE::DoExit
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/12/2015 14:14:28
//	
//  @param				t : 
*/
/*-----------------------------------------------------------------*/
bool	GRPANIMATIONSTACKMILESTONE::DoDisable	(bool disable)
{
	if (disable)		
		this->Publish(GRPMSG_DISABLE);			
	else
		this->Publish(GRPMSG_ENABLE);

	OnDisable(disable);

	return true;
}


/*-------------------------------------------------------------------
//	GRPANIMATIONSTACKMILESTONE::OnEnter
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			14/12/2015 12:23:17
//	
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONSTACKMILESTONE::OnEnter	(XQWORD t)
{
	if (disabled) return;

	this->currentpass++;
	if (currentpass>=this->passes)
	{
		this->isactive=true;
		this->currentpass=0;


		switch(this->type)
		{
		case GRPANIMATIONSTACKMILESTONE::NONE:																						break;
		case GRPANIMATIONSTACKMILESTONE::NOTIFY:					this->Publish(GRPMSG_NOTIFY);		break;
		case GRPANIMATIONSTACKMILESTONE::BOUNCE:					this->stack->Reverse();					break;
		case GRPANIMATIONSTACKMILESTONE::SPRING:		
		case GRPANIMATIONSTACKMILESTONE::DECCELERATE:			enter_speed=this->stack->GetFrameSpeed(); break;

		
		case GRPANIMATIONSTACKMILESTONE::HARD_FINISH:			this->stack->HaltFrame((XQWORD)this->start_frame);	
																											this->stack->SetIsFinished(true);
																											break;

		case GRPANIMATIONSTACKMILESTONE::HARD_STOP:				this->stack->HaltFrame((XQWORD)this->start_frame);																												
																											break;

		case GRPANIMATIONSTACKMILESTONE::FINISH:					this->stack->Forward();
																											this->stack->SetIsFinished(true); 
																											break;
		}
	}
	else
		this->isactive=false;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACKMILESTONE::OnContinue
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			14/12/2015 12:23:22
//	
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONSTACKMILESTONE::OnContinue(XQWORD t)
{
	if (disabled) return;

	if (!this->isactive) return;


		switch(this->type)
		{
		case GRPANIMATIONSTACKMILESTONE::NONE:																																	break;
		case GRPANIMATIONSTACKMILESTONE::BOUNCE:																																break;
		case GRPANIMATIONSTACKMILESTONE::SPRING:																																break;
		case GRPANIMATIONSTACKMILESTONE::DECCELERATE:		
			{							
				if (!this->durationtime)
					return;

				float mult	=1.0f;
							mult	=1.0f-((float)(t-this->start_time))/((float)this->durationtime);
							mult	*=this->enter_speed;

				if (stack->GetFrameSpeed() > (GRPANIMATIONSTACKMILESTONE_STOPMARGIN*this->enter_speed)) 
						stack->SetFrameSpeed	(mult);
				else 								
						this->stack->SetFrame	((XQWORD)this->end_frame);											
									
			}
			break;
		case GRPANIMATIONSTACKMILESTONE::HARD_FINISH:																														break;
		case GRPANIMATIONSTACKMILESTONE::HARD_STOP:																															break;		
		}
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACKMILESTONE::OnExit
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			14/12/2015 12:23:26
//	
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONSTACKMILESTONE::OnExit		(XQWORD t)
{
	if (disabled) return;

	if (!this->isactive) return;
	this->currentpass=0;
	
	this->isactive=false;

	switch(this->type)
		{
		case GRPANIMATIONSTACKMILESTONE::NONE:																															break;
		case GRPANIMATIONSTACKMILESTONE::BOUNCE:					this->stack->Reverse();														break;
		case GRPANIMATIONSTACKMILESTONE::SPRING:		
		case GRPANIMATIONSTACKMILESTONE::DECCELERATE:			this->stack->HaltFrame((XQWORD)this->end_frame);
																											this->stack->SetFrameSpeed(enter_speed);					
																											this->stack->SetIsFinished(true);
																																																				break;
		case GRPANIMATIONSTACKMILESTONE::HARD_STOP:				this->stack->Pause();															break;
		case GRPANIMATIONSTACKMILESTONE::HARD_FINISH:			this->stack->SetIsFinished(true);									break;
		}
}


/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::GRPANIMATIONSTACK
*/	
/**	
//	
//	Class Constructor GRPANIMATIONSTACK
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:17:27
//	
*/
/*-----------------------------------------------------------------*/
GRPANIMATIONSTACK::GRPANIMATIONSTACK()																							
{ 
	Clean();											
	GRPNAMED::classtype = GRPNAMED_TYPE_GRPANIMATIONSTACK;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::~GRPANIMATIONSTACK
*/	
/**	
//	
//	 Class Destructor GRPANIMATIONSTACK
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/05/2015 12:55:32
//	
*/
/*-----------------------------------------------------------------*/
GRPANIMATIONSTACK::~GRPANIMATIONSTACK()
{
	layers.DeleteContents();
	layers.DeleteAll();

	milestones.DeleteContents();
	milestones.DeleteAll();
}
																		

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::GetLayers
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:20:15
//	
//	@return 			XVECTOR<GRPANIMATIONLAYER*>* : 
//
*/
/*-----------------------------------------------------------------*/
XVECTOR<GRPANIMATIONLAYER*>* GRPANIMATIONSTACK::GetLayers()
{
	return &layers;																					
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::SetLayers
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:20:21
//	
//  @param				layers : 
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONSTACK::SetLayers(XVECTOR<GRPANIMATIONLAYER*> layers)						
{ 
	this->layers = layers;																	
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::AddLayer
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/11/2015 10:48:49
//	
//	@return 			bool : 
//
//  @param				layer : 
*/
/*-----------------------------------------------------------------*/
bool GRPANIMATIONSTACK::AddLayer(GRPANIMATIONLAYER* layer)
{
	if (layer==NULL)
		return false;

	layer->SetStack(this);
	/*
	if (layer->GetMinTime()<this->mintime)
		this->SetMinTime(layer->GetMinTime());

	if (layer->GetMaxTime()>this->maxtime)
		this->SetMaxTime(layer->GetMaxTime());
		*/
	return this->layers.Add(layer);
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::GetLayer
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:20:25
//	
//	@return 			GRPANIMATIONLAYER* : 
//
*/
/*-----------------------------------------------------------------*/
GRPANIMATIONLAYER* GRPANIMATIONSTACK::GetLayer()																							
{ 
	return this->GetLayers()->Get((int)this->currentlayer);			
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::GetLayer
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			10/12/2015 17:41:24
//	
//	@return 			GRPANIMATIONLAYER* : 
//
//  @param				layer : 
*/
/*-----------------------------------------------------------------*/
GRPANIMATIONLAYER* GRPANIMATIONSTACK::GetLayer(int layer)
{	
	return this->GetLayers()->Get(layer);	
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::SetCurrentLayer
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:20:33
//	
//  @param				currentlayer : 
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONSTACK::SetCurrentLayer(int currentlayer)															
{ 
	this->currentlayer = currentlayer;											
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::GetCurrentLayer
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:20:38
//	
//	@return 			XQWORD : 
//
*/
/*-----------------------------------------------------------------*/
XQWORD GRPANIMATIONSTACK::GetCurrentLayer()
{ 
	return currentlayer;																		
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::GetMaxFrames
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:20:42
//	
//	@return 			XQWORD : 
//
*/
/*-----------------------------------------------------------------*/
XQWORDSIG GRPANIMATIONSTACK::GetMaxTime()																							
{
	//return maxframes;																				
	return maxtime;																				
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::SetMaxFrames
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:20:47
//	
//  @param				maxframes : 
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONSTACK::SetMaxTime(XQWORDSIG maxtime)															
{ 
	//this->maxframes = maxframes;														
	this->maxtime=maxtime;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::GetMinTime
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			18/12/2015 17:46:53
//	
//	@return 			XQWORD : 
//
*/
/*-----------------------------------------------------------------*/
XQWORDSIG GRPANIMATIONSTACK::GetMinTime()
{
	return mintime;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::SetMinTime
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			18/12/2015 17:47:06
//	
//  @param				mintime : 
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONSTACK::SetMinTime(XQWORDSIG mintime)
{
	this->mintime=mintime;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::GetFrames
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			18/12/2015 16:18:52
//	
//	@return 			int : 
//
*/
/*-----------------------------------------------------------------*/
int GRPANIMATIONSTACK::GetFrames()
{
	if (this->layers.GetSize()>0)
	{
		GRPANIMATIONLAYER* layer=this->layers.Get(0);
		if (layer)
		{
		XQWORD frames=layer->GetMaxTime();
					 frames=this->GetFrameOfTime(frames);
		return (int)frames+1;
		}
	}
	return 0;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::GetMaxFrame
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			18/01/2016 12:23:19
//	
//	@return 			XQWORD : 
//
*/
/*-----------------------------------------------------------------*/
XQWORDSIG GRPANIMATIONSTACK::GetMaxFrame()																							
{ 	
	return maxframes;																				
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::SetMaxFrame
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			22/04/2016 10:43:14
//	
//  @param				max : 
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONSTACK::SetMaxFrame(XQWORDSIG max)																							
{ 	
	maxframes=max;																				
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::GetMinFrames
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:20:58
//	
//	@return 			XQWORD : 
//
*/
/*-----------------------------------------------------------------*/
XQWORDSIG GRPANIMATIONSTACK::GetMinFrame()																							
{ 	
	return minframes;																				
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::SetMinFrames
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:21:03
//	
//  @param				minframes : 
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONSTACK::SetMinFrame(XQWORDSIG minframes)															
{ 	
	this->minframes	= minframes;														
}


/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::SetFrame
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			30/11/2015 14:26:22
//	
//  @param				frame : 
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONSTACK::SetFrame	(XQWORDSIG frame)
{
	int n=this->layers.GetSize();
	for (int e=0;e<n;e++)
	{
		GRPANIMATIONLAYER*	layer=this->layers.FastGet(e);
										if (layer)
												layer->SetFrame(frame);
	}
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::GetFrame
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			31/05/2017 16:13:38
//	
//	@return 			XQWORDSIG : 
//
*/
/*-----------------------------------------------------------------*/
XQWORDSIG GRPANIMATIONSTACK::GetFrame()
{
	if (this->layers.FastGet(0))
		return this->layers.FastGet(0)->GetFrame();
	return 0;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::HaltFrame
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			16/12/2015 11:53:45
//	
//  @param				frame : 
*/
/*-----------------------------------------------------------------*/
void	 GRPANIMATIONSTACK::HaltFrame	(XQWORDSIG frame)
{
	SetActive(true);
	SetFrame	(frame);	
	Pause			();
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::Update
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:21:09
//	
//  @param				t : 
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONSTACK::Update(XQWORDSIG t)
{
	if (!this->IsActive())
	{
		this->status=GRPANIMATION_STATUS_PAUSED;
		return;
	}

	XQWORD inc=0;

	if (this->status==GRPANIMATION_STATUS_PLAYING)
	{		
			speed						+=	speed_acceleration;
			speed						-=	friction;
			inc							=		((XQWORD)((double)(t*speed)));
			stackFrame			+=	inc;		

			CheckMilestones(inc);

			laststacktime  =		stackFrame;					
	}

	int nlayers=layers.GetSize();
	for(int e=nlayers-1; e>=0; e--)																									
	{
		GRPANIMATIONLAYER* layer=layers.Get(e);
		if (layer)
		{
		if (this->status==GRPANIMATION_STATUS_PLAYING)
			layer->Update(inc);	
		else
			layer->Update(0);	

			if (layer->IsFinished())
				this->finished=true;
			else
			if (layer->GetStatus()!=GRPANIMATION_STATUS_PAUSED)
				this->status=GRPANIMATION_STATUS_PLAYING;
		}
	}	
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::IsActive
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			18/11/2015 17:35:00
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPANIMATIONSTACK::IsActive() 
{ 
	return active;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::SetActive
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			18/11/2015 17:43:46
//	
//  @param				active : 
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONSTACK::SetActive(bool isactive)
{
	this->active=isactive;
	for (XDWORD e=0;e<this->layers.GetSize();e++)
		this->layers.Get(e)->SetActive(isactive);

}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::GetStatus
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			18/11/2015 17:36:50
//	
//	@return 			GRPANIMATION_STATUS : 
//
*/
/*-----------------------------------------------------------------*/
GRPANIMATION_STATUS	 GRPANIMATIONSTACK::GetStatus()
{
	return this->status;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::IsFinished
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			26/11/2015 15:45:38
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool	 GRPANIMATIONSTACK::IsFinished()
{
	return this->finished;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::SetIsFinished
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			26/11/2015 15:45:43
//	
//  @param				finished : 
*/
/*-----------------------------------------------------------------*/
void	 GRPANIMATIONSTACK::SetIsFinished(bool finished)
{
	this->finished=finished;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::SetAnimationControl
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/11/2015 10:43:00
//	
//	@return 			bool : 
//
//  @param				control : 
*/
/*-----------------------------------------------------------------*/
bool	 GRPANIMATIONSTACK::SetAnimationControl(GRPANIMATIONCONTROL* control)
{
	if (control==NULL)
		return false;

	this->animationcontrol=control;

	return true;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::GetAnimationControl
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/11/2015 16:50:42
//	
//	@return 			GRPANIMATIONCONTROL* : 
//
*/
/*-----------------------------------------------------------------*/
GRPANIMATIONCONTROL*	 GRPANIMATIONSTACK::GetAnimationControl()
{
	return this->animationcontrol;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::GetStackFrame
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			10/12/2015 14:59:29
//	
//	@return 			XQWORD : 
//
*/
/*-----------------------------------------------------------------*/
XQWORDSIG	 GRPANIMATIONSTACK::GetStackFrame()																															
{ 
	return stackFrame;				
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::GetLastStackFrame
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			10/12/2015 14:59:24
//	
//	@return 			XQWORD : 
//
*/
/*-----------------------------------------------------------------*/
XQWORDSIG	 GRPANIMATIONSTACK::GetLastStackFrame()																															
{ 
	return laststacktime;		
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::SetStackFrame
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			10/12/2015 14:59:19
//	
//  @param				q : 
*/
/*-----------------------------------------------------------------*/
void	 GRPANIMATIONSTACK::SetStackFrame(XQWORDSIG q)																											
{ 
	this->stackFrame = q;			
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::SetLastStackFrame
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			10/12/2015 14:59:34
//	
//  @param				q : 
*/
/*-----------------------------------------------------------------*/
void	 GRPANIMATIONSTACK::SetLastStackFrame(XQWORDSIG q)																											
{ 
	this->laststacktime = q;	
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::SetLoopMode
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			10/12/2015 15:47:49
//	
//  @param				mode : 
*/
/*-----------------------------------------------------------------*/
void	 GRPANIMATIONSTACK::SetLoopMode	(GRPANIMATIONLOOPMODE mode)
{
	this->loopmode=mode;
		for (XDWORD e=0;e<this->layers.GetSize();e++)		
			this->layers.Get(e)->SetLoopMode(mode);

}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::GetLoopMode
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			13/01/2016 16:36:55
//	
//	@return 			GRPANIMATIONLOOPMODE : 
//
*/
/*-----------------------------------------------------------------*/
GRPANIMATIONLOOPMODE	 GRPANIMATIONSTACK::GetLoopMode()
{
	return this->loopmode;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::Trim
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			18/09/2017 18:21:15
//	
//	@return 			bool : 
//
//  @param				start_frame : 
//  @param				end_frame : 
*/
/*-----------------------------------------------------------------*/
bool GRPANIMATIONSTACK::Trim(XQWORDSIG start_frame, XQWORDSIG end_frame)
{
	
	this->SetMinTime(GetTimeOfFrame((int)start_frame));
	//this->minframes=start_frame;

	this->SetMaxTime(GetTimeOfFrame((int)end_frame));
	//this->maxframes=end_frame;

	return true;
}


/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::Play
*/	
/**	
//	Start animation from start on all layers
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			26/11/2015 12:34:25
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool	 GRPANIMATIONSTACK::Play()
{
	for (XDWORD e=0;e<this->layers.GetSize();e++)
		if (this->layers.Get(e)->IsActive())
			this->layers.Get(e)->Play();
	
return true;
}


/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::Pause
*/	
/**	
//	
//	halts animation  on all layers
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			01/12/2015 13:36:28
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool	 GRPANIMATIONSTACK::Pause()
{
	//if (this->GetStatus()==GRPANIMATION_STATUS_PAUSED)		return true;

	for (XDWORD e=0;e<this->layers.GetSize();e++)		
			this->layers.Get(e)->Pause();

	this->SetStatus(GRPANIMATION_STATUS_PAUSED);
	
	this->Update(0);

return true;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::Stop
*/	
/**	
//	
//	halts animation and returns to start
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			26/11/2015 12:34:20
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool	 GRPANIMATIONSTACK::Stop()
{
	for (XDWORD e=0;e<this->layers.GetSize();e++)
		if (this->layers.Get(e)->IsActive())
			this->layers.Get(e)->Stop();

return true;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::Rewind
*/	
/**	
//	Sets the animation cursor on the beginning
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			26/11/2015 13:02:43
//	
//	@return 			bool : true on success (no fail)
//
*/
/*-----------------------------------------------------------------*/
bool	 GRPANIMATIONSTACK::Rewind()
{
	finished=false;	

		for (XDWORD e=0;e<this->layers.GetSize();e++)
			//if (this->layers.Get(e)->IsActive())
				this->layers.Get(e)->Rewind();
		
return true;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::Forward
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			21/12/2015 12:02:36
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool	 GRPANIMATIONSTACK::Forward()
{
	finished=false;
		for (XDWORD e=0;e<this->layers.GetSize();e++)
			if (this->layers.Get(e)->IsActive())
				this->layers.Get(e)->Forward();
return true;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::Reverse
*/	
/**	
//	reverses play direction on all layers
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			11/12/2015 17:29:18
//	
//	@return 			bool : true on success ( no fail)
//
*/
/*-----------------------------------------------------------------*/
bool	 GRPANIMATIONSTACK::Reverse()
{		
		for (XDWORD e=0;e<this->layers.GetSize();e++)			
				this->layers.Get(e)->Reverse();

return true;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::IsSynced
*/	
/**	
//	returns if the stack is in synch  (deprecated , now, all stacks are unsynch)
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			10/12/2015 14:56:49
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool	 GRPANIMATIONSTACK::IsSynced()
{
	return synced;					
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::SetIsSynced
*/	
/**	
//	Sets if the stack is synced (deprecated , now, all stacks are unsynch)
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			10/12/2015 14:56:56
//	
//  @param				b : bool
*/
/*-----------------------------------------------------------------*/
void	 GRPANIMATIONSTACK::SetIsSynced(bool b)																												
{ 
	this->synced = b;				
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::SetFrameSpeed
*/	
/**	
//	sets frame increment per step
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/12/2015 14:19:57
//	
//  @param				speed : float
*/
/*-----------------------------------------------------------------*/
void	 GRPANIMATIONSTACK::SetFrameSpeed(float speed)
{
	this->speed=speed;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::SetFrameAcceleration
*/	
/**	
//	sets acceleration per frame
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/12/2015 14:24:58
//	
//  @param				acc : float 
*/
/*-----------------------------------------------------------------*/
void	 GRPANIMATIONSTACK::SetFrameAcceleration(float acc)
{
	this->speed_acceleration=acc;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::SetFriction
*/	
/**	
//	sets current friction (decceleration value)
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			11/12/2015 9:13:59
//	
//  @param				fric : float
*/
/*-----------------------------------------------------------------*/
void	 GRPANIMATIONSTACK::SetFriction(float fric)
{
	this->friction=fric;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::GetFriction
*/	
/**	
//	returns friction
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			11/12/2015 9:15:16
//	
//	@return 			float : friction
//
*/
/*-----------------------------------------------------------------*/
float	 GRPANIMATIONSTACK::GetFriction()
{
	return this->friction;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::AddMilestone
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			11/12/2015 9:42:07
//	
//	@return 			bool : 
//
//  @param				mstone : 
*/
/*-----------------------------------------------------------------*/
bool	 GRPANIMATIONSTACK::AddMilestone(GRPANIMATIONSTACKMILESTONE* mstone)
{	
				if (this->milestones.Add(mstone))
				{
					mstone->SetStack(this);
					return true;
				}
							
	return false;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::GetMilestones
*/	
/**	
//	
//	returns milestone vector
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			15/12/2015 11:29:21
//	
//	@return 			XVECTOR<GRPANIMATIONSTACKMILESTONE*>* : 
//
*/
/*-----------------------------------------------------------------*/
XVECTOR<GRPANIMATIONSTACKMILESTONE*>*		 GRPANIMATIONSTACK::GetMilestones()
{
	return &this->milestones;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::GetMilestone
*/	
/**	
//	get id milestone
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			11/12/2015 9:41:57
//	
//	@return 			GRPANIMATIONSTACKMILESTONE* : 
//
//  @param				id : 
*/
/*-----------------------------------------------------------------*/
GRPANIMATIONSTACKMILESTONE*	 GRPANIMATIONSTACK::GetMilestone(int id)
{
	return this->milestones.Get(id);
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::GetMilestone
*/	
/**	
//	get id milestone
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			11/12/2015 9:41:57
//	
//	@return 			GRPANIMATIONSTACKMILESTONE* : 
//
//  @param				id : 
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONSTACK::DisableMilestones()
{
	int n=this->milestones.GetSize();
	for (int e=0;e<n;e++)
	{
		GRPANIMATIONSTACKMILESTONE * stone=this->milestones.Get(e);
		if (stone)
		stone->Enable(false);
	}	
		this->Play();
		this->finished=false;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::CheckMilestones
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			11/12/2015 9:53:54
//	
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONSTACK::CheckMilestones(XQWORDSIG inc)
{
 	const int n=(int)this->milestones.GetSize();
	for (int e=n-1;e>=0;e--)
	{
		GRPANIMATIONSTACKMILESTONE* stone=milestones.Get(e);
		if(stone)
		{
	
		int n=this->layers.GetSize();
		for (int l=n-1;l>=0;l--)	
		{	
			GRPANIMATIONLAYER* layer	=this->layers.Get	(l);
			if (!layer) continue;

			if (!stone->GetIsDisabled() && layer->IsActive())
			{			
			XQWORD lasttime						=layer->GetLastStackTime	();
			XQWORD nowtime						=layer->GetLocalTime			();

			stone->SetMilestoneLocalTime(nowtime);
			stone->SetMilestoneLastTime	(lasttime);
			
				if (!stone->GetIsActive() ) //check if we enter
				{				
							if (nowtime >= stone->GetStartTime())
								if ((lasttime <= stone->GetStartTime()) || (lasttime >= nowtime))								
								{
										stone->DoEnter(nowtime);																											
								}
				}

				if (stone->GetIsActive() ) //check if we exit
				{							
							if (nowtime >= stone->GetEndTime())
								stone->DoExit(nowtime);
							else
								stone->DoContinue(nowtime);								
				}	
			}
		}
		}
	}

}


/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::GetTimeOfFrame
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			11/12/2015 10:04:24
//	
//	@return 			int : 
//
//  @param				frame : 
*/
/*-----------------------------------------------------------------*/
XQWORDSIG GRPANIMATIONSTACK::GetTimeOfFrame(int frame)
{
	/*
	GRPANIMATIONCONTROL* ac=this->GetAnimationControl();
	
		double	f						=double(this->GetMaxTime());
		XQWORDSIG	totalframes	=0;

		double	fps					=ac->GetFramesPerSecondByMode();
						totalframes	=(XQWORDSIG)((f*fps/GRPANIMATION_FBXSECOND));			

		if (totalframes==0) 
				return 0;

		XQWORDSIG	timeofframe	=(XQWORDSIG)(f*(double(frame)/double(totalframes)));		
		return	timeofframe;
		*/

		GRPANIMATIONCONTROL* ac=this->GetAnimationControl();
	
		double	f						=double(this->GetMaxTime());
		XQWORDSIG	totalframes	=0;

		double	fps					=ac->GetFramesPerSecondByMode();
						totalframes	=(XQWORDSIG)((f*fps/GRPANIMATION_FBXSECOND));			

		if (totalframes==0) 
				return 0;

		XQWORDSIG	timeofframe	=(XQWORDSIG)(f*(double(frame)/double(totalframes)));		
		return	timeofframe;
}


/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::FindAnimationNode
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			16/02/2016 16:33:49
//	
//	@return 			GRPANIMATIONNODE* : 
//
//  @param				name : 
//  @param				axis : 
*/
/*-----------------------------------------------------------------*/
GRPANIMATIONNODE*	 GRPANIMATIONSTACK::FindAnimationNode(XCHAR* name,GRPANIMATIONCURVETYPE type,GRPANIMATIONCURVEAXIS axis)
{
	for (XDWORD l=0;l<this->GetLayers()->GetSize();l++)
	{
		GRPANIMATIONLAYER* layer=this->GetLayers()->Get(l);

		if (layer)
		for (XDWORD n=0;n<layer->GetAnimationNodes()->GetSize();n++)
		{
			GRPANIMATIONNODE* node=layer->GetAnimationNodes()->Get(n);
			if (node)
			if (node->GetTargetName()->Compare(name)==0 && node->GetType()==type)	
			{
				GRPANIMATIONCURVE* curve=node->GetCurveConnections()->Get(axis);
				if (curve!=NULL)					
					return node;					
			}
		}
	}
	return NULL;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::GetFrameOfTime
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			11/12/2015 10:04:24
//	
//	@return 			int : 
//
//  @param				frame : 
*/
/*-----------------------------------------------------------------*/
int GRPANIMATIONSTACK::GetFrameOfTime(XQWORDSIG time)
{
	if (maxtime==0) return 0;

	GRPANIMATIONCONTROL* ac	=this->GetAnimationControl();
	
		double	f							=double(this->GetMaxTime());
		XQWORDSIG	totalframes		=0;
		double	fps						=ac->GetFramesPerSecondByMode();
						totalframes		=(XQWORDSIG)((f*fps/GRPANIMATION_FBXSECOND));

		int			FrameOfTime		= (int)((double)totalframes*((double)time/(double)maxtime));

		return FrameOfTime;
}


/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::GetFrameAcceleration
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			09/12/2015 10:48:06
//	
//	@return 			float : 
//
*/
/*-----------------------------------------------------------------*/
float	 GRPANIMATIONSTACK::GetFrameAcceleration()
{
	return this->speed_acceleration;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONSTACK::GetFrameSpeed
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/12/2015 14:20:02
//	
//	@return 			float : 
//
*/
/*-----------------------------------------------------------------*/
float	 GRPANIMATIONSTACK::GetFrameSpeed()
{
	return this->speed;
}


/*-------------------------------------------------------------------
//	GRPANIMATIONCONTROL::GRPANIMATIONCONTROL
*/	
/**	
//	
//	Class Constructor GRPANIMATIONCONTROL
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/05/2015 12:56:43
//	
 
*/
/*-----------------------------------------------------------------*/
GRPANIMATIONCONTROL::GRPANIMATIONCONTROL() 
{
	Clean();

	xtimer = xfactory->CreateTimer();
	xtimer->Reset();
																					
}

/*-------------------------------------------------------------------
//	GRPANIMATIONCONTROL::~GRPANIMATIONCONTROL
*/	
/**	
//	
//	 Class Destructor GRPANIMATIONCONTROL
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/05/2015 12:56:47
//	
*/
/*-----------------------------------------------------------------*/
GRPANIMATIONCONTROL::~GRPANIMATIONCONTROL() 
{
	if(xtimer) xfactory->DeleteTimer(xtimer);

	stacks.DeleteElementContents();
	stacks.DeleteAll();

	Clean();
}




/*-------------------------------------------------------------------
//	GRPANIMATIONCONTROL::GetControlTimeFPS
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:08:43
//	
//	@return 			GRPANIMATIONCONTROLTIMEFPS : 
//
*/
/*-----------------------------------------------------------------*/
GRPANIMATIONCONTROLTIMEFPS GRPANIMATIONCONTROL::GetControlTimeFPS()																							
{ 
	return controltimeFPS;																	
}



/*-------------------------------------------------------------------
//	GRPANIMATIONCONTROL::SetControlTimeFPS
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:08:49
//	
//  @param				controltimeFPS : 
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONCONTROL::SetControlTimeFPS(GRPANIMATIONCONTROLTIMEFPS controltimeFPS)
{ 
	this->controltimeFPS = controltimeFPS;									
}

/*-------------------------------------------------------------------
//	GRPANIMATIONCONTROL::GetControlTimeProtocolType
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:08:53
//	
//	@return 			GRPANIMATIONCONTROLTIMEPROTOCOLTYPE : 
//
*/
/*-----------------------------------------------------------------*/
GRPANIMATIONCONTROLTIMEPROTOCOLTYPE GRPANIMATIONCONTROL::GetControlTimeProtocolType()																							
{ 
	return controltimeprotocoltype;													
}

/*-------------------------------------------------------------------
//	GRPANIMATIONCONTROL::SetControlTimeProtocolType
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:09:23
//	
//  @param				controltimeprotocoltype : 
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONCONTROL::SetControlTimeProtocolType(GRPANIMATIONCONTROLTIMEPROTOCOLTYPE controltimeprotocoltype)  
{ 
	this->controltimeprotocoltype = controltimeprotocoltype;					
}

/*-------------------------------------------------------------------
//	GRPANIMATIONCONTROL::GetFramesPerSecondByMode
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:09:39
//	
//	@return 			double : 
//
*/
/*-----------------------------------------------------------------*/
double GRPANIMATIONCONTROL::GetFramesPerSecondByMode()
{ 
	double fps=30.0f;
		switch(controltimeFPS)
		{
		case	GRPANIMATIONCONTROLTIMEFPS_DEFAULT_MODE		:	 break;
		case  GRPANIMATIONCONTROLTIMEFPS_FRAMES120			:	fps=120.0f;	 break;
		case  GRPANIMATIONCONTROLTIMEFPS_FRAMES100			:	fps=100.0f;	 break;
		case  GRPANIMATIONCONTROLTIMEFPS_FRAMES60				:	fps=60.0f;	 break;
		case  GRPANIMATIONCONTROLTIMEFPS_FRAMES50				:	fps=50.0f;	 break;
		case  GRPANIMATIONCONTROLTIMEFPS_FRAMES48				:	fps=48.0f;	 break;
		case  GRPANIMATIONCONTROLTIMEFPS_FRAMES30				:	fps=30.0f;	 break;
		case  GRPANIMATIONCONTROLTIMEFPS_FRAMES30_DROP	:	fps=30.0f;	 break;
		case  GRPANIMATIONCONTROLTIMEFPS_NTSC_DROP_FRAME:	fps=30.0f;	 break;
		case  GRPANIMATIONCONTROLTIMEFPS_NTSC_FULL_FRAME:	fps=30.0f;   break;
		case  GRPANIMATIONCONTROLTIMEFPS_PAL						:	fps=25.0f;	 break;	
		case  GRPANIMATIONCONTROLTIMEFPS_CINEMA					:	fps=24.0f;	 break;
		case  GRPANIMATIONCONTROLTIMEFPS_FRAMES1000			:	fps=30.0f;	 break;
		case  GRPANIMATIONCONTROLTIMEFPS_CINEMA_ND			:	fps=23.976f; break;
		case  GRPANIMATIONCONTROLTIMEFPS_CUSTOM					:	fps=30.0f;	 break;
		case  GRPANIMATIONCONTROLTIMEFPS_TIME_MODE_COUNT:	fps=30.0f;	 break;


		default:																					break;								
		}		

		return fps; 
}


/*-------------------------------------------------------------------
//	GRPANIMATIONCONTROL::GetStatusType
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:12:17
//	
//	@return 			GRPANIMATIONSTATUSTYPE : 
//
*/
/*-----------------------------------------------------------------*/
GRPANIMATION_STATUS GRPANIMATIONCONTROL::GetStatusType	()																							
{ 
	return statustype;																			
}

/*-------------------------------------------------------------------
//	GRPANIMATIONCONTROL::SetStatusType
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:12:21
//	
//  @param				statustype : 
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONCONTROL::SetStatusType(GRPANIMATION_STATUS statustype)							
{ 
	this->statustype=statustype;														
}

/*-------------------------------------------------------------------
//	GRPANIMATIONCONTROL::GetFrameSpeed
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:12:26
//	
//	@return 			float : 
//
*/
/*-----------------------------------------------------------------*/
float GRPANIMATIONCONTROL::GetFrameSpeed()																							
{ 
	return framespeed;																			
}

/*-------------------------------------------------------------------
//	GRPANIMATIONCONTROL::SetFrameSpeed
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:12:31
//	
//  @param				framespeed : 
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONCONTROL::SetFrameSpeed(float framespeed)															
{ 
	this->framespeed = framespeed;													
}

/*-------------------------------------------------------------------
//	GRPANIMATIONCONTROL::GetCurrentFrame
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:12:37
//	
//	@return 			XQWORD : 
//
*/
/*-----------------------------------------------------------------*/
XQWORD GRPANIMATIONCONTROL::GetCurrentFrame()																							
{ 
	return currentframe;																		
}

/*-------------------------------------------------------------------
//	GRPANIMATIONCONTROL::SetCurrentFrame
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:12:42
//	
//  @param				currentframe : 
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONCONTROL::SetCurrentFrame(XQWORD currentframe)														
{ 
	this->currentframe = currentframe;											
}

/*-------------------------------------------------------------------
//	GRPANIMATIONCONTROL::SetCurrentFrame
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:12:47
//	
//  @param				currentframe : 
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONCONTROL::SetCurrentFrame(float currentframe)																	
{ 
	if (GetStacks()->Get(0))
	{
	XDWORD f = XDWORD( currentframe * (float)GetStacks()->Get(0)->GetMaxTime());
	this->currentframe = f;					
	}
}

/*-------------------------------------------------------------------
//	GRPANIMATIONCONTROL::SetFrame
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			25/11/2015 11:03:44
//	
//  @param				frame : 
*/
/*-----------------------------------------------------------------*/
void	 GRPANIMATIONCONTROL::SetFrame(XQWORD frame)
{
	for (XDWORD s=0;s<this->stacks.GetSize();s++)
		for (XDWORD l=0;l<this->stacks.Get(s)->GetLayers()->GetSize();l++)
			this->stacks.Get(s)->GetLayers()->Get(l)->SetFrame(frame);
}

/*-------------------------------------------------------------------
//	GRPANIMATIONCONTROL::GetTotalFrames
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:12:52
//	
//	@return 			XQWORD : 
//
*/
/*-----------------------------------------------------------------*/
XQWORD GRPANIMATIONCONTROL::GetTotalFrames()  
{ 
	GRPANIMATIONSTACK* stack=GetStacks()->Get(0);
	if (!stack) return 0;

	double f=double(stack->GetMaxTime());
		switch(controltimeFPS)
		{
		case GRPANIMATIONCONTROLTIMEFPS_FRAMES30			 :
		case GRPANIMATIONCONTROLTIMEFPS_NTSC_FULL_FRAME:	return (XQWORD)((f/GRPANIMATION_FBXSECOND)*30.0); 
		case GRPANIMATIONCONTROLTIMEFPS_PAL:							return (XQWORD)((f/GRPANIMATION_FBXSECOND)*25.0); 

		default:																					break;								
		}
		return (XQWORD)((f/GRPANIMATION_FBXSECOND)*30.0); 
}

/*-------------------------------------------------------------------
//	 GRPANIMATIONCONTROL::GetLoopMode
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	20/10/2017 13:16:33
//	@return		GRPANIMATIONLOOPMODE : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
GRPANIMATIONLOOPMODE GRPANIMATIONCONTROL::GetLoopMode()
{
		return this->loopmode; 
}

/*-------------------------------------------------------------------
//	GRPANIMATIONCONTROL::SetLoopMode
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:13:08
//	
//  @param				loopmode : 
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONCONTROL::SetLoopMode(GRPANIMATIONLOOPMODE loopmode)									
{ 
	this->loopmode = loopmode;															
}

/*-------------------------------------------------------------------
//	GRPANIMATIONCONTROL::AddStack
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/11/2015 10:40:37
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPANIMATIONCONTROL::AddStack(GRPANIMATIONSTACK* stack,XDWORD id)
{
	if (stack==NULL)
		return false;

	stack->SetAnimationControl(this);

	stack->SetID(id);
	return this->stacks.Add(id,stack);
}

/*-------------------------------------------------------------------
//	GRPANIMATIONCONTROL::GetStacks
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:13:13
//	
//	@return 			XVECTOR<GRPANIMATIONSTACK*>* : 
//
*/
/*-----------------------------------------------------------------*/
XMAP<XDWORD,GRPANIMATIONSTACK*>* GRPANIMATIONCONTROL::GetStacks()																							
{ 
	return &stacks;																					
}


/*-------------------------------------------------------------------
//	GRPANIMATIONCONTROL::GetStack
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:13:24
//	
//	@return 			GRPANIMATIONSTACK* : 
//
*/
/*-----------------------------------------------------------------*/
GRPANIMATIONSTACK* GRPANIMATIONCONTROL::GetStack()																							
{
	return this->stacks.Get(currentstack);												
}

/*-------------------------------------------------------------------
//	GRPANIMATIONCONTROL::GetStack
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:13:24
//	
//	@return 			GRPANIMATIONSTACK* : 
//
*/
/*-----------------------------------------------------------------*/
GRPANIMATIONSTACK* GRPANIMATIONCONTROL::GetStack(XDWORD i)																							
{
	return this->stacks.Get(i);												
}

/*-------------------------------------------------------------------
//	 GRPANIMATIONCONTROL::GetStateStack
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	20/12/2017 17:26:14
//	@return		bool : 
//
//	@param		int : 
//
*//*-----------------------------------------------------------------*/
bool GRPANIMATIONCONTROL::GetStateStack(int i)
{
		return this->stacks.Get(i)->IsActive();
}

/*-------------------------------------------------------------------
//	GRPANIMATIONCONTROL::SetActiveStack
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/11/2015 11:17:01
//	
//  @param				i : 
*/
/*-----------------------------------------------------------------*/
GRPANIMATIONSTACK*	 GRPANIMATIONCONTROL::SetStateStack(int i,bool status)
{	
	GRPANIMATIONSTACK* stack=this->stacks.Get(i);
	if (stack)
	{
		stack->SetActive(status);
		XDWORD size=stack->GetLayers()->GetSize();

		for (int e=size-1;e>=0;e--)
		{
			GRPANIMATIONLAYER* layer=stack->GetLayers()->Get(e);
												 layer->SetActive(status); 
		}
	}

	return stack;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONCONTROL::SetStateStack
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			18/01/2016 17:22:30
//	
//	@return 			GRPANIMATIONSTACK* : 
//
//  @param				name : 
//  @param				status : 
*/
/*-----------------------------------------------------------------*/
GRPANIMATIONSTACK*	 GRPANIMATIONCONTROL::SetStateStack(XCHAR* name,bool status)
{
	for (XDWORD e=0;e<this->GetStacks()->GetSize();e++)
	{
		GRPANIMATIONSTACK* stack=this->GetStacks()->Get(e);
		if (stack)
		if (stack->GetName()->Compare(name)==0)
		{
			return SetStateStack(e,status);
		}
	}
	
	return NULL;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONCONTROL::DisableStacks
*/	
/**	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			24/11/2015 9:59:38
*/
/*-----------------------------------------------------------------*/
void	 GRPANIMATIONCONTROL::DisableStacks()
{	
	int size=this->GetStacks()->GetSize();

	for (int e=size-1;e>=0;e--)
	{
		GRPANIMATIONSTACK* stack=this->stacks.Get(e);
		if  (stack)
					stack->SetActive(false);
	}
}

/*-------------------------------------------------------------------
//	GRPANIMATIONCONTROL::GetIsStopped
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:07:37
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPANIMATIONCONTROL::GetIsStopped() 																							
{
	return (statustype != GRPANIMATION_STATUS_PLAYING);	
}

/*-------------------------------------------------------------------
//	GRPANIMATIONCONTROL::Play
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:06:49
//	
*/
/*-----------------------------------------------------------------*/
void	GRPANIMATIONCONTROL::Play()																				
{ 
	xtimer->Reset();		

	//currentframe	= this->minTime;
	statustype		=	GRPANIMATION_STATUS_PLAYING;	

	for (XDWORD e=0;e<this->stacks.GetSize();e++)	
		if (stacks.GetElement(e))
		if (stacks.GetElement(e)->IsActive())
				stacks.GetElement(e)->Play();
	
}

/*-------------------------------------------------------------------
//	GRPANIMATIONCONTROL::Stop
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:06:44
//	
*/
/*-----------------------------------------------------------------*/
void	GRPANIMATIONCONTROL::Stop() 																				
{
	xtimer->Reset();	
	currentframe	=	0;
	statustype		=	GRPANIMATION_STATUS_STOPPED;	

		for (XDWORD s=0;s<this->stacks.GetSize();s++)
		for (XDWORD l=0;l<this->stacks.GetElement(s)->GetLayers()->GetSize();l++)
			if (this->stacks.GetElement(s))
					this->stacks.GetElement(s)->GetLayers()->FastGet(l)->SetStatus(GRPANIMATION_STATUS_STOPPED);
																																																	
}

/*-------------------------------------------------------------------
//	GRPANIMATIONCONTROL::Begin
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:06:40
//	
*/
/*-----------------------------------------------------------------*/
void	GRPANIMATIONCONTROL::Begin()																				
{
	currentframe	=	this->minTime;
	statustype	 	=	GRPANIMATION_STATUS_STOPPED;

	this->xtimer->Reset();

	for (XDWORD s=0;s<this->stacks.GetSize();s++)
	{
		if (this->stacks.GetElement(s))
			this->stacks.GetElement(s)->Rewind();		
	}

	
}

/*-------------------------------------------------------------------
//	GRPANIMATIONCONTROL::Pause
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:06:36
//	
*/
/*-----------------------------------------------------------------*/
void	GRPANIMATIONCONTROL::Pause()																				
{																																																		
	statustype	=	GRPANIMATION_STATUS_PAUSED;	

		for (XDWORD s=0;s<this->stacks.GetSize();s++)
		for (XDWORD l=0;l<this->stacks.GetElement(s)->GetLayers()->GetSize();l++)
		{
			if (this->stacks.GetElement(s))
				if (this->stacks.GetElement(s)->GetLayers()->Get(l))
					this->stacks.GetElement(s)->GetLayers()->FastGet(l)->SetStatus(GRPANIMATION_STATUS_PAUSED);
		}

	Update();
}

/*-------------------------------------------------------------------
//	GRPANIMATIONCONTROL::Pause
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			24/11/2015 11:16:56
//	
//  @param				frame : 
*/
/*-----------------------------------------------------------------*/
void	GRPANIMATIONCONTROL::Pause(XDWORD frame)
{
	SetFrame(frame);
	Pause();
}

	
/*-------------------------------------------------------------------
//	GRPANIMATIONCONTROL::Update
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			11/02/2015 13:44:30
//	
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONCONTROL::Update()
{
	if(stacks.GetSize() == 0 || xtimer==NULL)  return;

	XQWORDSIG update=0;

 	switch(statustype)
		{
			case GRPANIMATION_STATUS_STOPPED		: currentframe		=		0;	break;
			case GRPANIMATION_STATUS_PAUSED			:													break;
			case GRPANIMATION_STATUS_PLAYING		:	milis						=		xtimer->GetMeasureMicroSeconds();
																						update					=		(XQWORDSIG)(milis * GRPANIMATION_FBXMICROSECOND);			
																						update					=		(XQWORDSIG)(float(update)*framespeed);
																																break;		
			default: 
								XDEBUG_PRINTCOLOR(0, __L("Unknown animation state"));	break;		
	}	

	if (update < 0)
	{
			XDEBUG_PRINTCOLOR(4, __L("negative update"));
			return;
	}

	xtimer->Reset();																						

	XDWORD n=GetStacks()->GetSize();	
	switch (this->order)
	{
//---------------------- normal order
	case DIRECT:
			for (XDWORD e = 0; e<n; e++)
			{
					GRPANIMATIONSTACK* stack = this->stacks.GetElement(e);
					if (stack)
							stack->Update(update);
			}
			break;
//---------------------- reversed fast order
	default:
	case REVERSED:
			for (int e = n - 1; e >= 0; e--)
			{
					GRPANIMATIONSTACK* stack = this->stacks.GetElement(e);
					if (stack)
							stack->Update(update);
			}
	}
					
}

/*-------------------------------------------------------------------
//	GRPANIMATIONCONTROL::End
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 16:04:44
//	
*/
/*-----------------------------------------------------------------*/
void	GRPANIMATIONCONTROL::End()																							
{ 
	Update();																								
}


/*-------------------------------------------------------------------
//	GRPANIMATIONCONTROL::Reset
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			13/09/2016 11:55:06
//	
*/
/*-----------------------------------------------------------------*/
void	GRPANIMATIONCONTROL::Reset()
{
	this->xtimer->Reset();
}

/*-------------------------------------------------------------------
//	GRPANIMATIONCONTROL::GetTimer
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			11/12/2015 14:59:26
//	
//	@return 			XTIMER* : 
//
*/
/*-----------------------------------------------------------------*/
XTIMER*		GRPANIMATIONCONTROL::GetTimer()
{
	return this->xtimer;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONCONTROL::SetupFrames
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			25/11/2015 11:05:10
//	
*/
/*-----------------------------------------------------------------*/
void	GRPANIMATIONCONTROL::SetupFrames()
{
	/*
	if (GetStacks()->GetSize()>0)
	{
		minTime=GetStack()->GetMinTime();
		maxTime=GetStack()->GetMaxTime();
	}
	else
	{
		minTime=0;
		maxTime=0;
	}
	*/
	
	/*
	XQWORDSIG max=LLONG_MIN;
	XQWORDSIG min=LLONG_MAX;
	XDWORD nStacks=GetStacks()->GetSize();
	for (XDWORD s=0;s<nStacks;s++)
	{
		GRPANIMATIONSTACK* stack=GetStacks()->FastGet(s);
	
		XDWORD nLayers=stack->GetLayers()->GetSize();
		for (XDWORD l=0;l<nLayers;l++)
		{
			GRPANIMATIONLAYER* layer=stack->GetLayers()->FastGet(l);
			XDWORD nCurves=layer->GetCurves()->GetSize();
			for (XDWORD c=0;c<nCurves;c++)
			{
				GRPANIMATIONCURVE* curve=layer->GetCurves()->FastGet(c);
			
				if (max < curve->lasttime)
					max=curve->lasttime;

				if (min > curve->initime)				
					min=curve->firsttime;			
			}
			if (min < layer->GetMinTime())
				layer->SetMinTime(min);

			if (max > layer->GetMaxTime())
				layer->SetMaxTime(max);

			layer->Rewind();
			
		}
		/*
			if (min < stack->GetMinTime())
				stack->SetMinTime(min);

			if (max > stack->GetMaxTime())
				stack->SetMaxTime(max);
				*//*
			stack->Rewind();
		
	}		

	if (min < this->minTime)
	this->minTime=min;
	if (max > this->maxTime)
	this->maxTime=max;	
		*/
}




/*-------------------------------------------------------------------
//	GRPANIMATIONCONTROL::ShowAllData
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			14/01/2016 9:45:13
//	
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONCONTROL::ShowAllData()
{
	XSTRING out;
	XSTRING m;

	out.Add(__L("\n\nShowing Animation Output:"));
	for (XDWORD s=0;s<this->GetStacks()->GetSize();s++)
	{
		GRPANIMATIONSTACK* stack=this->GetStacks()->Get(s);
		m.Format(__L("\n\tStack[%d]:%s"),s,stack->GetName()->Get());		
		out.Add(m);

		for (XDWORD l=0;l<stack->GetLayers()->GetSize();l++)
		{
			GRPANIMATIONLAYER* layer=this->GetStacks()->Get(s)->GetLayers()->Get(l);
			m.Format(__L("\n\t\tLayer[%d]"),l);
			out.Add(m);
			for (XDWORD n=0;n<layer->GetAnimationNodes()->GetSize();n++)
			{
				GRPANIMATIONNODE* node=layer->GetAnimationNodes()->Get(n);
				m.Format(__L("\n\t\t\tNode[%d] %s"),n,node->GetTargetName()->Get());
				out.Add(m);				
				XDEBUG_PRINTCOLOR(0,out.Get());
				out="";
			}
		}

	}
	
}

/*-------------------------------------------------------------------
//	ShowAllNodesAndCurvesAffectingTargetByName
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			14/01/2016 10:05:51
//	
//  @param				targetname : 
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONCONTROL::ShowAllNodesAndCurvesAffectingTargetByName	(XCHAR* targetname)
{
	for (XDWORD s=0;s<this->GetStacks()->GetSize();s++)
	{
		GRPANIMATIONSTACK* stack=this->GetStacks()->GetElement(s);

		for (XDWORD l=0;l<stack->GetLayers()->GetSize();l++)
		{
			GRPANIMATIONLAYER* layer=stack->GetLayers()->Get(l);
			bool found=false;
			for (XDWORD n=0;n<layer->GetAnimationNodes()->GetSize();n++)
			{
				GRPANIMATIONNODE* node=layer->GetAnimationNodes()->Get(n);
				if (node->GetTargetName()->Compare(targetname)==0)
				{
					found=true;
					XDEBUG_PRINTCOLOR(0,__L("Stack[%03d]:%020s Node[%03d] %020s %020s %f"),s,stack->GetName()->Get(),n,node->GetTargetName()->Get(),node->GetName()->Get(),node->GetDefaultValues());						
					for (XDWORD c=0;c<node->GetCurveConnections()->GetSize();c++)
					{
						XDWORD axis=node->GetCurveConnections()->GetKey(c);
						GRPANIMATIONCURVE* curve=node->GetCurveConnections()->GetElement(c);
						XDEBUG_PRINTCOLOR(0,__L("\t %s %d"),curve->GetName()->Get(),curve->bcsSize);			
						for (XDWORD b=0;b<curve->keyvalues.GetSize();b++)
							XDEBUG_PRINTCOLOR(0,__L("\t\t\t [%llu,%f]"),curve->keytime.Get(b),curve->keyvalues.Get(b));			
					}
				}				
			}
		}		
	}	
}

/*-------------------------------------------------------------------
//	ShowAllNodesAndCurvesAffectingTargetByName
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			14/01/2016 10:05:51
//	
//  @param				targetname : 
*/
/*-----------------------------------------------------------------*/
void GRPANIMATIONCONTROL::ShowAllNodesAndCurvesAffectingTargetByName	(XCHAR* targetname,GRPANIMATIONCURVETYPE type,GRPANIMATIONCURVEAXIS axis)
{
	/*
	for (XDWORD s=0;s<this->GetStacks()->GetSize();s++)
	{
		GRPANIMATIONSTACK* stack=this->GetStacks()->Get(s);

		for (XDWORD l=0;l<stack->GetLayers()->GetSize();l++)
		{
			GRPANIMATIONLAYER* layer=this->GetStacks()->Get(s)->GetLayers()->Get(l);
			bool found=false;
			for (XDWORD n=0;n<layer->GetAnimationNodes()->GetSize();n++)
			{
				GRPANIMATIONNODE* node=layer->GetAnimationNodes()->Get(n);
				if (node->GetTargetName()->Compare(targetname)==0)
				{
					
					if (node->GetType()==type)
					for (XDWORD c=0;c<node->GetCurveConnections()->GetSize();c++)
					{
						if (node->GetCurves()->Get(c))
						if (node->GetCurves()->Get(c)->GetType()==axis)
						{
							found=true;
							XDEBUG_PRINTCOLOR(0,__L("Stack[%03d]:%040s Node[%03d] %040s %040s"),s,stack->GetName()->Get(),n,node->GetTargetName()->Get(),node->GetName()->Get());					
						}
					}					
				}				
			}
			if (found)
			XDEBUG_PRINTCOLOR(0,__L("\n"));
		}		
	}	*/
}


/*-------------------------------------------------------------------
//	GRPANIMATIONCONTROL::DettachAnimation
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			25/04/2016 10:58:06
//	
//  @param				element : 
*/
/*-----------------------------------------------------------------*/
bool GRPANIMATIONCONTROL::DettachAnimation(GRPINTERFACEANIMABLE* element)
{
	if (this->attachees.Find(element)!=NOTFOUND)
	{
	GRPANIMATIONNODE* node=this->attachees.Get(element);
	if (node!=NULL)
		{
			GRPNAMED_TYPE type=element->GetType();
			switch(type)
			{
			case GRPNAMED_TYPE_GRPELEMENT:node->SetTarget(NULL);			return true;
			case GRPNAMED_TYPE_GRPNODE:		node->SetTarget(NULL);			return true;							
			}
			this->attachees.Delete(element);
		}
		DettachAnimation(element);
	}
	return true;
}

/*-------------------------------------------------------------------
//	GRPANIMATIONCONTROL::AttachAnimation
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			25/04/2016 10:58:12
//	
//  @param				element : 
//  @param				GRPANIMATIONNODE* : 
*/
/*-----------------------------------------------------------------*/
bool GRPANIMATIONCONTROL::AttachAnimation	(GRPINTERFACEANIMABLE* element,GRPANIMATIONNODE* node)
{
	return this->attachees.Add(element,node);
}


/*-------------------------------------------------------------------
//	 GRPANIMATIONCONTROL::TrimStacks
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	20/10/2017 13:17:18
//	@return		void : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
void  GRPANIMATIONCONTROL::TrimStacks()
{
	for (XDWORD s = 0; s < this->GetStacks()->GetSize(); s++)
	{
		GRPANIMATIONSTACK* stack = this->GetStacks()->GetElement(s);
		for (XDWORD l = 0; l < stack->GetLayers()->GetSize(); l++)
		{
			GRPANIMATIONLAYER* layer = stack->GetLayers()->Get(l);
			XQWORDSIG maxtime = 0;
			for (XDWORD c = 0; c < layer->GetCurves()->GetSize(); c++)
			{
				GRPANIMATIONCURVE* curve = layer->GetCurves()->Get(c);
				XQWORDSIG time = curve->keytime.GetLast();				
				if (time>maxtime)				
				{
					maxtime = time;
					stack->SetMaxTime(time);
					stack->SetMaxFrame(stack->GetFrameOfTime(time));
				}
				
			}
		}

	}
}

/*-------------------------------------------------------------------
//	 GRPANIMATIONCONTROL::SetOrder
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	20/10/2017 13:17:11
//	@return		void : 
//
//	@param		XDWORD : 
//
*//*-----------------------------------------------------------------*/
void GRPANIMATIONCONTROL::SetOrder(XDWORD type) 
{ 
		this->order = type; 
}

/*-------------------------------------------------------------------
//	 GRPANIMATIONCONTROL::GetOwner
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	20/10/2017 13:22:34
//	@return		GRPOBJECT* : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
GRPOBJECT* GRPANIMATIONCONTROL::GetOwner()
{
		return owner;
}

/*-------------------------------------------------------------------
//	 GRPANIMATIONCONTROL::SetOwner
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	20/10/2017 13:22:40
//	@return		void : 
//
//	@param		GRPOBJECT* : 
//
*//*-----------------------------------------------------------------*/
void GRPANIMATIONCONTROL::SetOwner(GRPOBJECT* obj)
{
		this->owner = obj;
}