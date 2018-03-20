
/*------------------------------------------------------------------------------------------
//	GRPSLOTNUMBER.CPP
//	
//	Manages Slot Numbers
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 07/11/2016 16:49:25
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "GRPSlotNumber.h"
#include "GRPScene.h"
#include "GRPObjectBuilder.h"
#include "GRPMaterialLibrary.h"
#include "GRPFontManager.h"
	

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/

/*-------------------------------------------------------------------
//	GRPSLOTNUMBER::GRPSLOTNUMBER
*/	
/**	
//	
//	Class Constructor GRPSLOTNUMBER
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			08/11/2016 9:53:09
//	
*/
/*-----------------------------------------------------------------*/
GRPSLOTNUMBER::GRPSLOTNUMBER(XDWORD id) 
{
	Clean();
	align	=GRPARRAYALIGNTOOL_ALIGNFLAGS_RIGHT;
	GRPNAMED::classtype = GRPNAMED_TYPE_GRPTEXTBOX;

	//Content				=__L("01234567890");
	Content				=__L("01234567890");
	contentmask		=__L("%s");
	this->id=id;
}

/*-------------------------------------------------------------------
//	GRPSLOTNUMBER::~GRPSLOTNUMBER
*/	
/**	
//	
//	 Class Destructor GRPSLOTNUMBER
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			08/11/2016 9:53:18
//	
*/
/*-----------------------------------------------------------------*/
GRPSLOTNUMBER::~GRPSLOTNUMBER() 
{	
	Clean();
}


/*-------------------------------------------------------------------
//	GRPSLOTNUMBER::Init
*/	
/**	
//	Initializes the text items
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			08/11/2016 9:53:26
//	
//  @param				nSlots : XDWORD number of slots
//	@return				bool : success
*/
/*-----------------------------------------------------------------*/
bool	GRPSLOTNUMBER::Init(XDWORD nSlots,GRPSCENE* scene,XCHAR* fontname, GRPSTENCILPASS* reusestencil)
{

	// ----------- CleanUp , just in case is a re-init

	if (obj!=NULL)
					delete(obj);

	if (stencilpass!=NULL)
					delete(stencilpass);

	this->slots.DeleteAll();


	//------------- procedural creation of textbox

							obj=new GRPOBJECT();
							obj->SetNode(scene->CreateNode());																
							

							if (reusestencil==NULL)
							{
									stencilpass=new GRPSTENCILPASS();
									scene->Add(stencilpass);
									scene->SelectPass(stencilpass);
									stencilpass->SetCamera(scene->GetCamera());
							}
							else
							{
									stencilpass=reusestencil;
							}
															
	GRPTEXTELEMENT* text=NULL;		
						
	this->nSlots=nSlots;

	
	GRPFONT* font=GRPOBJECTBUILDER::Get()->GetFontManager()->GetFont(fontname);	


	text		=new GRPTEXTELEMENT();	
									
									text->SetNode								(obj->CreateNode());
									text->SetFont								(font);
									text->SetDirection					(GRPTEXTBOX_VERTICAL);
									text->GetAlignTool()->cellsize.x		= CellWidth;
									text->GetAlignTool()->cellsize.y		= CellHeight;
									text->GetAlignTool()->cellspacing.x = CellSpace;
									text->GetAlignTool()->cellspacing.y = CellGap;
									text->SetAlign							(GRPTEXTBOX_ALIGNLEFT);									
																	
									text->SetDistribution				(GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_FIXED | GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_SPECIFYSPACING | GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_STARTOFFSET);
								//	text->GetAlignTool()->startingoffset= -CellWidth/2.0f;
									text->SetContent						(Content.Get());
									text->SetContentMask				(contentmask.Get());
									obj->AddText								(text);																
									//text->UpdateContent					(true);
									obj->AddElement							(text);
									text->GetNode()->SetParent	(obj->GetNode());
									this->slots.Add(text);


	for (XDWORD e=1;e<nSlots;e++)
	{

									GRPNODE* node=obj->CreateNode();
									GRPELEMENT* element=new GRPELEMENT();
															element->SetNode(node);
															element->Reference(text);
								/*					 
								  text		=new GRPTEXTELEMENT();	

									text->SetName								(node->GetName()->Get());
									text->SetNode								(node);
									text->SetFont								(font);
									text->SetDirection					(GRPTEXTBOX_VERTICAL);
									text->GetAlignTool()->cellsize.x		= CellWidth;
									text->GetAlignTool()->cellsize.y		= CellHeight;
									text->GetAlignTool()->cellspacing.x = CellSpace;
									text->GetAlignTool()->cellspacing.y = CellGap;
									text->SetAlign							(GRPTEXTBOX_ALIGNLEFT);									
																	
									text->SetDistribution				(GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_FIXED | GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_SPECIFYSPACING | GRPARRAYALIGNTOOL_DISTRIBUTION_TYPE_STARTOFFSET);
								//	text->GetAlignTool()->startingoffset= -CellWidth/2.0f;
									text->SetContent						(Content.Get());
									text->SetContentMask				(contentmask.Get());
																											
									//text->UpdateContent					(true);
									*/
									node->SetPosition						(e*(CellWidth/2.0f+CellSpace),0.0f,0.0f);
									
									//obj->AddText								(text);
									//obj->AddElement							(text);

									obj->AddElement							(element);
									element->GetNode()->SetParent(obj->GetNode());
									//text->GetNode()->SetParent	(obj->GetNode());

									this->slots.Add(element);

	}

	SlotWidth				=CellWidth+CellSpace*2.0f;
	TotalWidth			=nSlots*SlotWidth;
	NumberHeight		=CellHeight+CellGap;
	SlotTotalHeight	=NumberHeight*text->GetContent()->GetSize();
	
	scene->Add(obj->GetNode());
	scene->Add(obj);

									GRPELEMENT*	box			= new GRPELEMENT();
									GRPMESH*    boxmesh = GRPOBJECTBUILDER::Get()->CreateCube();

															box->Reference(boxmesh);
															box->SetNode(obj->CreateNode());
															box->GetNode()->SetParent(obj->GetNode());															
															box->GetNode()->SetPosition	(0.25f*TotalWidth+CellSpace/4.0f,	NumberHeight/4.0f + text->GetAlignTool()->cellspacing.y*0.5f,0.0f);															
															box->GetNode()->SetScale		(TotalWidth/2.0f,		NumberHeight      - text->GetAlignTool()->cellspacing.y/2.0f,0.01f);										

															obj->Add(box);
															obj->SetTag(id);

									scene->Add(box->GetNode());
	switch(align)
	{
	case GRPARRAYALIGNTOOL_ALIGNFLAGS_LEFT	:	obj->GetNode()->SetPosition(0.0f,							0.0f,	0.0f); break;
	case GRPARRAYALIGNTOOL_ALIGNFLAGS_CENTER:	obj->GetNode()->SetPosition(-TotalWidth/4.0f,	0.0f,	0.0f); break;
	case GRPARRAYALIGNTOOL_ALIGNFLAGS_RIGHT	:	obj->GetNode()->SetPosition(-TotalWidth/2.0f,	0.0f,	0.0f); break;
	}
	//*/


return true;
}

/*-------------------------------------------------------------------
//	GRPSLOTNUMBER::SetValue
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			18/11/2016 14:15:04
//	
//	@return 			bool : 
//
//  @param				 : 
//  @param				GRPANIMATIONCURVEAXIS : 
//  @param				double : 
//  @param				double : 
*/
/*-----------------------------------------------------------------*/
bool GRPSLOTNUMBER::SetValue(GRPANIMATIONCURVETYPE type,	GRPANIMATIONCURVEAXIS axis,	double value,	double base)
{
		
	xmutex->Lock();

	if (this->isready)
	{
		this->isready=false;
			switch (type)
			{

			case GRPANIMATIONCURVETYPE_USERVALUE:
														{
																this->interpolator->SetCurrent((XDWORD)value);
																this->UpdateContent		(true);																															
														}
														break;

			case GRPANIMATIONCURVETYPE_DIFFUSEFACTOR:
														this->obj->GetNode()->SetValue(type,axis,value,base);			
														break;				

			default:
														this->obj->GetNode()->SetValue(type,axis,value,base);			
														break;

			}
	}
			this->isready=true;

			xmutex->UnLock();

			return true;
	return true;
}

/*-------------------------------------------------------------------
//	GRPSLOTNUMBER::UpdateContent
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			18/11/2016 14:15:16
//	
//	@return 			bool : 
//
//  @param				mergeelements : 
*/
/*-----------------------------------------------------------------*/
bool		GRPSLOTNUMBER::UpdateContent	(bool				mergeelements)
{
	Update(0);
	return true;
}


/*-------------------------------------------------------------------
//	GRPSLOTNUMBER::Clear
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			18/11/2016 14:16:18
//	
*/
/*-----------------------------------------------------------------*/
void		GRPSLOTNUMBER::Clear()
{

}

/*-------------------------------------------------------------------
//	GRPSLOTNUMBER::GetObj
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			09/11/2016 12:14:51
//	
//	@return 			GRPOBJECT* : 
//
*/
/*-----------------------------------------------------------------*/
GRPOBJECT*	GRPSLOTNUMBER::GetObj()
{
	return this->obj;
}

/*-------------------------------------------------------------------
//	GRPSLOTNUMBER::SetValue
*/	
/**	
//	Set current hold number
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			08/11/2016 9:53:38
//	
//  @param				value:  XDWORD current hold value
*/
/*-----------------------------------------------------------------*/
void  GRPSLOTNUMBER::SetCurrent(XDWORD value)
{
	for (XDWORD e=0;e<nSlots;e++)
	{
		int digit=int(this->interpolator->GetCurrent()/pow(double(10),int(nSlots-e)))%10;

		const GRPVECTOR* v=	slots.Get(e)->GetNode()->GetPosition();						
												slots.Get(e)->GetNode()->SetPosition(v->vector[0],v->vector[1]+(NumberHeight*(digit)),v->vector[2]);																			
	}
}


/*-------------------------------------------------------------------
//	GRPSLOTNUMBER::GetSlots
*/	
/**	
//	returns the slot vector
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			08/11/2016 10:04:15
//	
//	@return 			XVECTOR<GRPTEXTELEMENT*>* : pointer to slot vector
//
*/
/*-----------------------------------------------------------------*/
XVECTOR<GRPELEMENT*>* GRPSLOTNUMBER::GetSlots	()
{
	return &this->slots;
}


/*-------------------------------------------------------------------
//	GRPSLOTNUMBER::SetCell
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			08/11/2016 10:49:32
//	
//  @param				cellwidth : 
//  @param				cellspace : 
*/
/*-----------------------------------------------------------------*/
void		GRPSLOTNUMBER::SetCell(float cellwidth,		float cellspace)
{
	this->CellWidth=cellwidth;
	this->CellSpace=cellspace;
}


/*-------------------------------------------------------------------
//	GRPSLOTNUMBER::GetCell
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			08/11/2016 10:49:41
//	
//  @param				*cellwidth : 
//  @param				*cellspace : 
*/
/*-----------------------------------------------------------------*/
void		GRPSLOTNUMBER::GetCell(float *cellwidth,	float *cellspace)
{
	*cellwidth=CellWidth;
	*cellspace=CellSpace;
}

/*-------------------------------------------------------------------
//	GRPSLOTNUMBER::SetAlign
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			11/11/2016 14:39:34
//	
//  @param				align : 
*/
/*-----------------------------------------------------------------*/
void		GRPSLOTNUMBER::SetAlign  (GRPARRAYALIGNTOOL_ALIGNFLAGS align)
{
	this->align=align;
}

/*-------------------------------------------------------------------
//	GRPSLOTNUMBER::Update
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			08/11/2016 11:03:14
//	
*/
/*-----------------------------------------------------------------*/
bool		GRPSLOTNUMBER::Update(XQWORD delta)
{
	double advance=0;		
	double current= this->interpolator->GetCurrent();

	float carry=this->interpolator->GetCarry();
	
	if (multiplicator!=1)
	{	
	if ((this->interpolator->GetCurrent()%multiplicator) == (multiplicator-1))		
			 advance=carry;
	}	
	else
		advance=carry;
	
	for (int e=nSlots-1,d=this->multiplicator;e>=0;e--,d*=10)
	{		
			int number=(int(current/d)%10);						
			
			const GRPVECTOR* v=GetSlots()->Get(e)->GetNode()->GetPosition();

			if (this->interpolator->GetCurrent()<=this->interpolator->GetTarget())
			{
			GetSlots()->Get(e)->GetNode()->SetPosition	(v->vector[0],
																									(float)((number+advance)*NumberHeight),
																									v->vector[2]);	
			}
			else
			{
			if (advance<=0.0001f)
				  advance=+1.0f;
			GetSlots()->Get(e)->GetNode()->SetPosition	(v->vector[0],
																									(float)((1.0f+number-advance)*NumberHeight),
																									v->vector[2]);	
			}

				if (number!=9)
					advance=0.0f;
				

			if (v->vector[1]>	 -GetSlots()->Get(e)->GetBoundingBox()->minpoint.y)
													GetSlots()->Get(e)->GetNode()->SetPosition(v->vector[0],0.0f,v->vector[2]);
			if (v->vector[1]<		GetSlots()->Get(e)->GetBoundingBox()->maxpoint.y-NumberHeight)
													GetSlots()->Get(e)->GetNode()->SetPosition(v->vector[0],GetSlots()->Get(e)->GetBoundingBox()->minpoint.y-NumberHeight,v->vector[2]);
	}	

	return true;
}
				

/*-------------------------------------------------------------------
//	GRPSLOTNUMBER::GetStencilPass
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			23/11/2016 16:46:15
//	
//	@return 			GRPSTENCILPASS* : 
//
*/
/*-----------------------------------------------------------------*/
GRPSTENCILPASS*	GRPSLOTNUMBER::GetStencilPass	()
{
	return this->stencilpass;
}