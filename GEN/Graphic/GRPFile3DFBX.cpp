/*------------------------------------------------------------------------------------------
//	GRPFILE3DFBX.CPP
*/	
/**	
// \class 
//   
//  Loads FilmBox Mesh File
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 12/01/2015 16:44:44
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XFileTXT.h"
#include "Xtimer.h"

#include "GRPFile3DFBX.h"
#include "GRPObject.h"
#include "GRPScene.h"
#include "GRPMatrix.h"
#include "GRPMaterialLibrary.h"
#include "GRPObjectBuilder.h"
#include "GRPCamera.h"
#include "GRPLight.h"
#include "GRPNode.h"
#include "GRPAnimation.h"
#include "GRPDeformer.h"
#include "GRPSkinning.h"

#include "GRPLoader.h"
#include "XMemory.h"

#include "GRPTextBox.h"

/*---- CLASS -----------------------------------------------------------------------------*/

/*-------------------------------------------------------------------
//	FBXData::FBXData
*/	
/**	
//	
//	Class Constructor FBXData
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			01/06/2015 9:45:45
//	
*/
/*-----------------------------------------------------------------*/
FBXData::FBXData() 
{	
	property						=	false;
	ArrayIndex					=	NULL;
	ArrayValues					=	NULL;
}

/*-------------------------------------------------------------------
//	FBXData::~FBXData
*/	
/**	
//	
//	 Class Destructor FBXData
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			01/06/2015 9:45:49
//	
*/
/*-----------------------------------------------------------------*/
FBXData::~FBXData()
{
Delete();
}

/*-------------------------------------------------------------------
//	FBXData::GetArrayValues
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			15/02/2016 12:05:43
//	
//	@return 			XVECTOR<double>* : 
//
*/
/*-----------------------------------------------------------------*/
/*
FBXVECTOR<double>* FBXData::GetArrayValues()
{
	return this->ArrayValues;
}
*/

/*-------------------------------------------------------------------
//	FBXData::GetArrayIndex
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			15/02/2016 12:05:50
//	
//	@return 			XVECTOR<long : 
//
*/
/*-----------------------------------------------------------------*/
/*
FBXVECTOR<long long>*	FBXData::ArrayIndex
{
		return this->ArrayIndex;
}*/


void	FBXData::SetArrayValues(FBXVECTOR<double>*	v)
{
	this->ArrayValues=v;
}

void	FBXData::SetArrayIndex(FBXVECTOR<long long>* v)
{
	this->ArrayIndex=v;
}

/*-------------------------------------------------------------------
//	Delete
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			15/02/2016 12:01:28
//	
*/
/*-----------------------------------------------------------------*/
void FBXData::Delete()
{
	if (ArrayValues)
	{
		ArrayValues->DeleteAll();
		delete(ArrayValues);
		ArrayValues=NULL;
	}

	if (ArrayIndex)
	{
		ArrayIndex->DeleteAll();
		delete(ArrayIndex);
		ArrayIndex=NULL;
	}
}


/*-------------------------------------------------------------------
//	FBXNode::FBXNode
*/	
/**	
//	
//	Class Constructor FBXNode
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			01/06/2015 9:47:14
//	
*/
/*-----------------------------------------------------------------*/
FBXNode::FBXNode	() 
{ 
	data=NULL; 
	Children=NULL;
	LastIndex=0;
};

/*-------------------------------------------------------------------
//	FBXNode::~FBXNode
*/	
/**	
//	
//	 Class Destructor FBXNode
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			01/06/2015 9:47:34
//	
*/
/*-----------------------------------------------------------------*/
FBXNode::~FBXNode () 
{
Delete();
}


/*-------------------------------------------------------------------
//	FBXNode::Show
*/	
/**	
//	
//	Shows the FBX Tree Nodes on the debugger
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			01/06/2015 9:47:50
//	
//  @param				level : 
*/
/*-----------------------------------------------------------------*/
void FBXNode::Show(XDWORD level)
{
	if (data!=NULL)
	{
			FBXSTRING string;
		for (XDWORD e=0;e<level;e++)
		{
			string.Add(__C(' '));
			string.Add(__C(' '));
		}

		string.Add(data->name);
		string.Add(__L(" Type:"));
		string.Add(data->type);
		string.Add(__L(" Label:"));
		string.Add(data->label);
		string.Add(__L(" Value:"));
		string.Add(data->value);
					
		FBX_DEBUG(0,__L("%s"),string.Get());
		string.Empty();

		if (data->ArrayIndex!=NULL)
			for (XDWORD e=0;e<data->ArrayIndex->GetSize();e++)
			{
					FBXSTRING a;
				a.Format(__L("%u,"),data->ArrayIndex->FastGet(e));
				string.Add(a);
			}
		FBX_DEBUG(0,__L("%s"),string.Get());
		string.Empty();

		if (data->ArrayValues!=NULL)
			for (XDWORD e=0;e<data->ArrayValues->GetSize();e++)
			{
					FBXSTRING a;
				a.Format(__L("%f,"),data->ArrayValues->FastGet(e));
				string.Add(a);
			}		
		FBX_DEBUG(0,__L("%s"),string.Get());
	}
		
	if (Children==NULL) 
		return;

	FBXSTRING string;
	for (XDWORD e=0;e<level;e++)
		{
			string.Add(__C(' '));
			string.Add(__C(' '));
		}
	FBX_DEBUG(0,__L("%s{"),string.Get());

	for (XDWORD e=0;e<Children->GetSize();e++)
		Children->FastGet(e)->Show(level+1);
	FBX_DEBUG(0,__L("%s}"),string.Get());

}

/*-------------------------------------------------------------------
//	OPERATORS
*/	
/**	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			01/06/2015 9:50:36
//	
//	@return 			FBXNode::operator : 
//------------------------------------------------------------------
*/
FBXNode::operator XDWORD		()	
{
	return data->value.Fast_ConvertToDWord();	
}

/*-------------------------------------------------------------------
//	GLFLOAT
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			16/11/2015 13:26:50
//	
//	@return 			FBXNode::operator : 
//
*/
/*-----------------------------------------------------------------*/
FBXNode::operator GLFLOAT		()	
{
	return data->value.Fast_ConvertToFloat();	
}

/*-------------------------------------------------------------------
//	GRPVECTOR
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			16/11/2015 13:26:36
//	
//	@return 			FBXNode::operator : 
//
*/
/*-----------------------------------------------------------------*/
FBXNode::operator GRPVECTOR	()	
{
	float x,y,z;
#ifdef USE_XFSTRING
	data->value.scan(__L("%f,%f,%f"),&x,&y,&z);
#else
	data->value.UnFormat(__L("%f,%f,%f"), &x, &y, &z);
#endif
	GRPVECTOR p((float)x,(float)y,(float)z);		
	return p;
}

/*-------------------------------------------------------------------
//	GRPPOINT
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			16/11/2015 13:26:31
//	
//	@return 			FBXNode::operator : 
//
*/
/*-----------------------------------------------------------------*/
FBXNode::operator GRPPOINT()
{
		float x,y,z;

#ifdef USE_XFSTRING
	  data->value.scan(__L("%f,%f,%f"),&x,&y,&z);
#else
		data->value.UnFormat(__L("%f,%f,%f"), &x, &y, &z);
#endif

	GRPPOINT p((float)x,(float)y,(float)z);		
	return p;
}

/*-------------------------------------------------------------------
//	GRPMATRIX
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			16/11/2015 13:26:26
//	
//	@return 			FBXNode::operator : 
//
*/
/*-----------------------------------------------------------------*/
FBXNode::operator GRPMATRIX()
{
	GRPMATRIX m;
	GLFLOAT tmp[16];

	if (this->data)
	if (this->data->ArrayValues)
	for (XDWORD e=0;e<16;e++)
		tmp[e]=(GLFLOAT)this->data->ArrayValues->FastGet(e);
	m.BuildMatrixFromFloatArray(tmp);
	return m;

}

/*-------------------------------------------------------------------
//	GRPCOLOR
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			16/11/2015 13:26:21
//	
//	@return 			FBXNode::operator : 
//
*/
/*-----------------------------------------------------------------*/
FBXNode::operator GRPCOLOR()
{
	float x,y,z;
#ifdef USE_XFSTRING
	data->value.scan(__L("%f,%f,%f"),&x,&y,&z);
#else
	data->value.UnFormat(__L("%f,%f,%f"), &x, &y, &z);
#endif
	
	GRPCOLOR p((float)x,(float)y,(float)z,1.0f);		
	return p;
}


/*-------------------------------------------------------------------
//	GRPFILE3DFBX::~GRPFILE3DFBX
*/	
/**	
//	 Class Destructor GRPFILE3DFBX
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			27/01/2015 12:03:31
//	
*/
/*-----------------------------------------------------------------*/
GRPFILE3DFBX::~GRPFILE3DFBX										()
{
	if (object)
	for (XDWORD e=0;e<NodeMap.GetSize();e++)
	{
		GRPNODE* node=NodeMap.GetElement(e);
		if (object->GetNodes()->Find(node)==NOTFOUND)
			if (object->GetCameras()->Find((GRPCAMERA*)node)==NOTFOUND)
				if (object->GetLights()->Find((GRPLIGHT*)node)==NOTFOUND)
					delete(node);
	}
	this->NodeMap.DeleteAll													();

	if (object)
	for (XDWORD e = 0; e<CameraMap.GetSize(); e++)
	{
			GRPCAMERA* node = CameraMap.GetElement(e);
			if (object->GetCameras()->Find((GRPCAMERA*)node) == NOTFOUND)
			if (object->GetNodes()->Find(node) == NOTFOUND)			
			if (object->GetLights()->Find((GRPLIGHT*)node) == NOTFOUND)
									delete(node);
	}
	if (object)
	for (XDWORD e = 0; e<LightMap.GetSize(); e++)
	{
			GRPCAMERA* node = CameraMap.GetElement(e);
			if (object->GetLights()->Find((GRPLIGHT*)node) == NOTFOUND)
			if (object->GetNodes()->Find(node) == NOTFOUND)
			if (object->GetCameras()->Find((GRPCAMERA*)node) == NOTFOUND)							
									delete(node);
	}

	this->NodeAttributeMap.DeleteAll								();
	this->GeometricMap.DeleteAll										();	
	this->TextureMap.DeleteAll											();
	this->MaterialMap.DeleteAll											();
	this->CameraMap.DeleteAll												();

	this->AnimationStackMap.DeleteAll								();
	this->AnimationNodeMap.DeleteAll								();
	this->AnimationCurveMap.DeleteAll								();
	
	if (object)
	for (XDWORD e = 0; e < LightAttributeMap.GetSize(); e++)
	{
			GRPLIGHT* light = LightAttributeMap.GetElement(e);			
			if (object->GetNodes()->Find(light->target) == NOTFOUND)			
					delete(light->target);				
			
			if (object->GetNodes()->Find(light) == NOTFOUND)
					delete(light);
	}
	this->LightAttributeMap.DeleteAll								();	

	if (object)
	for (XDWORD e = 0; e < CameraAttributeMap.GetSize(); e++)
	{
			GRPCAMERA* cam = CameraAttributeMap.GetElement(e);			
			if (object->GetNodes()->Find(cam->target) == NOTFOUND)			
					delete(cam->target);					
			
			if (object->GetCameras()->Find(cam) == NOTFOUND)
					delete(cam);
	}
	this->CameraAttributeMap.DeleteAll							();

	this->LocatorMap.DeleteAll											();
		
	this->IgnoredNodes.DeleteContents								();
	this->IgnoredNodes.DeleteAll										();

	this->DeformerMap.DeleteAll											();
	this->SubdeformerMap.DeleteAll									();
	this->SubdeformerShapeMap.DeleteAll							();

	this->SkinLimbMap.DeleteAll											();
	this->SkinPoseMap.DeleteAll											();
	this->SkinMap.DeleteAll													();
	this->SkinClusterMap.DeleteAll									();
	this->SkinLimbAttributeMap.DeleteElementContents();
	this->SkinLimbAttributeMap.DeleteAll						();

	this->TagMap.DeleteAll													();
	this->ObjectNameMap.DeleteAll										();

	ExcessNodes.DeleteContents();
	ExcessNodes.DeleteAll();
	ExcessDatas.DeleteContents();
	ExcessDatas.DeleteAll();

	if (xfiletxt!=NULL)		delete(xfiletxt);		xfiletxt=NULL;

	if (chronometer)
		{
			xfactory->DeleteTimer(chronometer);
			chronometer=NULL;
		}
		


}

/*-------------------------------------------------------------------
//	GRPFILE3DFBX::Abort
*/	
/**	
//	Clears the Elements
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			18/05/2015 14:05:29
//	
*/
/*-----------------------------------------------------------------*/
void GRPFILE3DFBX::Abort()
{

	if (loadFlags & GRPFILE3D_INCLUDE_ALL)
	{
			this->NodeMap.DeleteAll													();
			this->LocatorMap.DeleteAll											();
			this->CameraAttributeMap.DeleteAll							();
			this->LightAttributeMap.DeleteAll								();	
	}

	if (loadFlags & GRPFILE3D_INCLUDE_ANIMATION)
	{
		this->AnimationStackMap.DeleteElementContents		();
		this->AnimationLayerMap.DeleteElementContents		();
		this->AnimationNodeMap.DeleteElementContents		();
		this->AnimationCurveMap.DeleteElementContents		();
	}

	this->CameraAttributeMap.DeleteAll							();
	this->SkinLimbMap.DeleteAll											();
	this->SkinPoseMap.DeleteAll											();
	this->SkinMap.DeleteAll													();	
	this->LightAttributeMap.DeleteElementContents		();
	this->LightAttributeMap.DeleteAll								();	
	this->LocatorMap.DeleteAll											();
	this->SkinClusterMap.DeleteAll									();	

	
	this->GeometricMap.DeleteElementContents();
	this->GeometricMap.DeleteAll();

	delete(this->object);

	this->object=NULL;
			
}


/*-------------------------------------------------------------------
//	GRPFILE3DFBX::CreateFBXNode
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			16/12/2015 9:41:29
//	
//	@return 			FBXNode* : 
//
*/
/*-----------------------------------------------------------------*/
FBXNode* GRPFILE3DFBX::CreateFBXNode()
{
	if (FBXNodeIndex==MAX_FBX_NODES)
	{
	//	FBX_DEBUG(4,__L("----- GRPFILE3DFBX::CreateFBXNode: ERROR! maximun memory overpassed! Performance slowdown %d"),FBXNodeIndex+ExcessNodes.GetSize());
	}

	if (FBXNodeIndex>=MAX_FBX_NODES)
	{		
		FBXNode * node = new FBXNode();
		ExcessNodes.Add(node);
		FBXNodeIndex++;
		return node;
	}
	return &this->FBXNodes[FBXNodeIndex++];
}

/*-------------------------------------------------------------------
//	GRPFILE3DFBX::CreateFBXData
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			16/12/2015 9:41:35
//	
//	@return 			FBXData* : 
//
*/
/*-----------------------------------------------------------------*/
FBXData* GRPFILE3DFBX::CreateFBXData()
{

	if (FBXDataIndex==MAX_FBX_NODES)
		{
			FBX_DEBUG(4,__L("----- GRPFILE3DFBX::CreateFBXData: ERROR! maximun memory overpassed! Performance slowdown"));
		}
		
	if (FBXDataIndex>=MAX_FBX_NODES)
	{
		//FBX_DEBUG(4,__L("----- GRPFILE3DFBX::CreateFBXData: ERROR! maximun memory overpassed! Performance slowdown"));
		FBXData * data = new FBXData();
		ExcessDatas.Add(data);
		FBXDataIndex++;
		return data;
	}

	return &this->FBXDatas[FBXDataIndex++];
}

/*-------------------------------------------------------------------
//	GRPFILE3DFBX::Load
*/	
/**	
//	
//	Load the file and parse it
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/01/2015 12:26:44
//	
//	@return 			GRPOBJECT* : created object
//
//  @param				XCHAR* name :  file name
*/
/*-----------------------------------------------------------------*/
GRPOBJECT*	GRPFILE3DFBX::Load			(XCHAR* name)
{
	Clean();

	XPATH			xpath;


	chronometer->Reset();

		Root			=		CreateFBXNode();
		
		if (Root==NULL)			
			return NULL;

		xfiletxt	=		new FBXFILETXT();
		xpath			=		name;

		FBXfile		=		xpath;

		FBXSTRING s;
		xpath.GetNamefileExt(s);

		


	if(!xfiletxt) return NULL;
	if(!xfiletxt->Open(xpath)) 
	{

		FBX_DEBUG(FBX_ERROR,__L("\t\tUnable to open FBX file : %s"),s.Get());
		return NULL;	
	}
			
	if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_STATUS)
		{
			FBX_DEBUG(FBX_STATUS,__L("Reading FBX File %s"),s.Get());
		}


		CurrentStep=GRPFILE3DFBX_STEP_LOADINGFILE;
		if (!xfiletxt->ReadAllFile())
		{
			if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_ERRORS)
				{
					FBX_DEBUG(FBX_ERROR,__L("Unable to open File"));
				}
			
			xfiletxt->Close();
			return NULL;
		}
	
		chronometer->Reset();

		nlines=xfiletxt->GetNLines();
		xfiletxt->Close();		

		CurrentLine=0;
		TypesMap.SetIsMulti(true);


	if (!(loadFlags & GRPFILE3D_INCLUDE_GEOMETRY) && !(loadFlags & GRPFILE3D_INCLUDE_NEW_GEOMETRY) )
	{
			FBXSTRING* geo=new FBXSTRING(__L("Geometry"));
			IgnoredNodes.Add(geo);
	}	
	
	if (!(loadFlags & GRPFILE3D_INCLUDE_MATERIALS))
	{
			FBXSTRING* material=new FBXSTRING(__L("Material"));
			IgnoredNodes.Add(material);
			FBXSTRING* Texture=new FBXSTRING(__L("Texture"));
			IgnoredNodes.Add(Texture);
			FBXSTRING* Video=new FBXSTRING(__L("Video"));
			IgnoredNodes.Add(Video);
	}
	

	if (!(loadFlags & GRPFILE3D_INCLUDE_ANIMATION))
	{
			FBXSTRING* astack=new FBXSTRING(__L("AnimationStack"));
			IgnoredNodes.Add(astack);
			FBXSTRING* anode=new FBXSTRING(__L("AnimationCurveNode"));
			IgnoredNodes.Add(anode);
			FBXSTRING* acurve=new FBXSTRING(__L("AnimationCurve"));
			IgnoredNodes.Add(acurve);
			FBXSTRING* alayer=new FBXSTRING(__L("AnimationLayer"));
			IgnoredNodes.Add(alayer);
	}
	
	
			CurrentStep=GRPFILE3DFBX_STEP_PARSING;
			ReadNode(Root);
			
			chronometer->Reset();

			if (LoadDictionary()!=NULL)
			{
				chronometer->Reset();

				if (!Setup(object))
				{
					FBX_DEBUG(FBX_ERROR,__L("FBX Failed during setup phase"));		
					Abort();
					return NULL;
				}
				chronometer->Reset();


				if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_STATUS)
					{
						FBX_DEBUG(FBX_NOTIFY,__L("******* FBX loaded in %llu    nodes:%d,datas:%d"), chronometer->GetMeasureMilliSeconds(),this->FBXNodeIndex,this->FBXDataIndex);		
					}
			}		
			else
			{
				Abort();
			}

	xfiletxt->DeleteAllLines();
	return object;
}

/*-------------------------------------------------------------------
//	GRPFILE3DFBX::SetupIgnoredNodes
*/	
/**	
//	
//	Sets a collection of elements we discard right now
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			01/06/2015 9:36:06
//	
*/
/*-----------------------------------------------------------------*/
void	GRPFILE3DFBX::SetupIgnoredNodes()
{
		FBXSTRING* in[10];
	for(int i=0;i<10;i++)
		in[i]=NULL;

	IgnoredNodes.DeleteContents();
	IgnoredNodes.DeleteAll();

	int a=-1;
	
	in[++a]=new FBXSTRING();in[a]->Set("Documents");
	in[++a]=new FBXSTRING();in[a]->Set("Definitions");
	in[++a]=new FBXSTRING();in[a]->Set("LayerElementVisibility");
	in[++a]=new FBXSTRING();in[a]->Set("LayerElementMaterial");
	in[++a]=new FBXSTRING();in[a]->Set("Takes");
	in[++a]=new FBXSTRING();in[a]->Set("Edges");
	in[++a]=new FBXSTRING();in[a]->Set("Layer");
	//*/
	for (int e=0;e<=a;e++)
		IgnoredNodes.Add(in[e]);
}
/*-------------------------------------------------------------------
//	GRPFILE3DFBX::IsIgnoredNode
*/	
/**	
//	Checks if the node name is in the ignored collection
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			01/06/2015 9:36:50
//	
//	@return 			bool : true if the node is ignored
//
//  @param				name : FBXSTRING& node name to check
*/
/*-----------------------------------------------------------------*/
bool	GRPFILE3DFBX::IsIgnoredNode		(FBXSTRING& name)
{
	XDWORD size=IgnoredNodes.GetSize();
	static XDWORD last=0;

	for (XDWORD e=last;e<size;e++)
	{
		if (name.Compare(IgnoredNodes.FastGet(e)->Get(),true)==0)
		{
			last=e;
			return true;
		}
	}
	for (XDWORD e=0;e<last && e<size;e++)
	{
		if (name.Compare(IgnoredNodes.FastGet(e)->Get(),true)==0)
		{
		last=e;
		return true;
		}
	}
	return false;
}
/*-------------------------------------------------------------------
//	GRPFILE3DFBX::Setup
*/	
/**	
//	Sets the Object for use. Sets cameras if they dont have Target. Setups and initializes animations. 
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			11/05/2015 11:27:17
//	
//  @param				obj : GRPOBJECT* to setup
*/
/*-----------------------------------------------------------------*/
bool	GRPFILE3DFBX::Setup(GRPOBJECT* obj)
{		
		//----------------------------setup animation
		if (obj!=NULL)
		if (obj->GetAnimationControl()!=NULL)
		{
			for (XDWORD e=0;e<this->AnimationCurveMap.GetSize();e++)	
				this->AnimationCurveMap.GetElement(e)->Init();
																
			obj->GetAnimationControl()->SetupFrames();
		}

		//-------------------------------------- deformers
		if (obj!=NULL)
		{
				for (XDWORD e=0;e<obj->GetElements()->GetSize();e++)
				{
						//---------------------------- init Skins
						if (obj->GetElements()->FastGet(e)->GetSkin()!=NULL)
						{								
							for (XDWORD p=0;p<this->SkinPoseMap.GetSize();p++)
							{
								GRPSKINPOSE* pose=this->SkinPoseMap.elements.FastGet(p);
								obj->GetElements()->FastGet(e)->GetSkin()->AddPose(pose);
							}
									obj->GetElements()->FastGet(e)->GetSkin()->Init();
						}
						//---------------------------- init Morphers
						if (obj->GetElements()->FastGet(e)->GetDeformer()!=NULL)																					
									obj->GetElements()->FastGet(e)->GetDeformer()->Init();						
				}
		}

		for (XDWORD e = 0; e<obj->GetElements()->GetSize(); e++)
		{
				GRPELEMENT* element = obj->GetElements()->FastGet(e);
				element->SetProgram(element->ChooseShader());
		}



		//-------------------------------------- try to set default camera
		bool FoundDefaultCamera=false;
		FBXSTRING s;
		s.Format(__L("Model::%s"), DefaultCamera.Get());

		object->SetCurrentCamera(-1);
		for (XDWORD e=0;e<this->object->GetCameras()->GetSize();e++)					
			if (object->GetCameras()->FastGet(e)->GetName()->Compare(s,false)==0)
				{
					object->SetCurrentCamera(e);
					FoundDefaultCamera=true;
					break;
				}
		
		if (!FoundDefaultCamera)
		{
			if (this->object->GetCameras()->GetSize()>0)
				object->SetCurrentCamera(0);
		}

		for (XDWORD e = 0; e < obj->GetLights()->GetSize(); e++)
		{
				float m = obj->GetLights()->Get(e)->GetMultiplier();
				obj->GetLights()->Get(e)->SetMultiplier(m);
		}

		//------------------------------------- create Text Locators
		this->object->CreateTextLocators();

		//for (XDWORD e=0;e<this->object->GetNodes()->GetSize();e++)
		//this->object->GetNode()->SetScale(1.0f/UnitScaleFactor, 1.0f/UnitScaleFactor, 1.0f/UnitScaleFactor);
			
		return true;
}
/*-------------------------------------------------------------------
//	GRPFILE3DFBX::ConsumeBlock
*/	
/**	
//	Skips current block
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			20/01/2015 10:30:37
//	
*/
/*-----------------------------------------------------------------*/
void GRPFILE3DFBX::ConsumeBlock()
{
	CurrentLine++;	
	for(; CurrentLine<nlines; CurrentLine++)
	{
			XSTRING* line=xfiletxt->GetLine(CurrentLine);
		//UpdateTask();
		if (line->FindCharacter(__C('{'))!=XSTRING_NOTFOUND)
			ConsumeBlock();
		if (line->FindCharacter(__C('}'),0,true)!=XSTRING_NOTFOUND)
			break;
	}
}

/*-------------------------------------------------------------------
//	GRPFILE3DFBX::ReadNode
*/	
/**	
//	Parse current node
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			20/01/2015 13:05:06
//
//  @param				node : FBXNode* recursive node
*/
/*-----------------------------------------------------------------*/
bool GRPFILE3DFBX::ReadNode(FBXNode* node)
{
	static int nNod=0;
	
	level++;
	
	if (node->data)
	{
		XQWORD tag=node->data->value.ConvertToQWord();
		this->TagMap.Add(tag,&node->data->label);
		if (IsIgnoredNode(node->data->name))
		{
				FBXSTRING type;
			type=node->data->type;

			if (name.Compare(__L("Material"),8,false)==0)
			{
				//GRPMATERIAL* material=this->scene->GetMaterialLibrary()->FindByName(node->data->label.Get());
				GRPMATERIAL* material=GRPMATERIALLIBRARY::Get()->FindByName(node->data->label.Get());
				if (material!=NULL)					 
				{
						if	(!this->MaterialMap.Set(tag,material))
								  this->MaterialMap.Add(tag,material);

						if	(!this->TypesMap.Set(tag,GRPFILE3DFBX_CONNECTION_TYPE_MATERIAL))
									this->TypesMap.Add(tag,GRPFILE3DFBX_CONNECTION_TYPE_MATERIAL);
				}				
			}

			ConsumeBlock();
			return false;
		}
	}
	//*/

	//-------------------------- new node creation
	if (node->Children==NULL)
		node->Children=new FBXVECTOR<FBXNode*>();

	if (node->data==NULL)
		node->data=CreateFBXData();

	//-------------------------- read node
	CurrentLine++;
	for(; CurrentLine<nlines; CurrentLine++)
	{
		XSTRING* pline=xfiletxt->GetLine(CurrentLine);
		
		int e=0;
		XCHAR* txt=pline->Get();

		while (txt[e]==__C(' ') || txt[e]==__C('\t'))
			e++;
		if (txt[e]==__C('}'))
			break;
		if (txt[e]==__C(';'))
			continue;
		
		line=*pline;
		
		line.Fast_DeleteNoCharacters(XSTRINGCONTEXT_ATFIRST);
		XCHAR firstcharacter=line.Get()[0];					


			XDWORD FindColon=line.FindCharacter(__C(':'));
			line.Copy(0,FindColon,name);		


			XDWORD pos=line.FindCharacter(__C('*'),FindColon);			
			if (pos!=XSTRING_NOTFOUND)  //------------------------ IS ARRAY
			{
				XDWORD end=line.FindCharacter(__C(' '),pos);
				if (end!=XSTRING_NOTFOUND)
				{			
			
				FBXNode*	newnode=CreateFBXNode();
									if (newnode==NULL) return false;

									newnode->data=CreateFBXData();
									if (newnode->data==NULL) return false;;
									newnode->data->name.Set(name.Get());

										XDWORD arraylength;

											line.Copy(pos+1,end,newnode->data->value);
											arraylength=newnode->data->value.Fast_ConvertToInt();
								
				if (!node->Children->Add(newnode)) return false;;

				//----------------------- Values

					if (name.Compare(__L("KeyValueFloat"),13,false)==0)
					{
						FBXVECTOR<double>* array	=ReadArrayDouble(arraylength);
						newnode->data->SetArrayValues(array);
					}					
					else
					if (name.Compare(__L("KeyTime"),7,false)==0)
					{
						FBXVECTOR<long long>* array	=ReadArrayIndex(arraylength);
						newnode->data->SetArrayIndex(array);
					}
					else
					if (name.Compare(__L("Normals"),7,false)==0)
					{
						if (!(this->loadFlags & GRPFILE3D_INCLUDE_GEOMETRY) && !(this->loadFlags & GRPFILE3D_INCLUDE_NEW_GEOMETRY))							
							SkipArray();
						else
						{
						FBXVECTOR<double>* array	=ReadArrayDouble(arraylength);
						newnode->data->SetArrayValues(array);
						}
					}
					else
					if (name.Compare(__L("KeyAttrFlags"),12,false)==0)
					{
						FBXVECTOR<long long>* array	=ReadArrayIndex(arraylength);
						newnode->data->SetArrayIndex(array);
					}
					else					
					if (name.Compare(__L("KeyAttrDataFloat"),16,false)==0)
					{
						FBXVECTOR<long long>* array	=ReadArrayIndex(arraylength);
						newnode->data->SetArrayIndex(array);
					}
					else
					if (name.Compare(__L("KeyAttrRefCount"),15,false)==0)
					{
						FBXVECTOR<long long>* array	=ReadArrayIndex(arraylength);
						newnode->data->SetArrayIndex(array);
					}
					else
					if (name.Compare(__L("Vertices"),8,false)==0)
					{
						if (!(this->loadFlags & GRPFILE3D_INCLUDE_GEOMETRY) && !(this->loadFlags & GRPFILE3D_INCLUDE_NEW_GEOMETRY))							
							SkipArray();
						else
						{
						FBXVECTOR<double>* array	=ReadArrayDouble(arraylength);
						newnode->data->SetArrayValues(array);
						}
					}
					else					
					if (name.Compare(__L("NormalsW"),8,false)==0)
					{
					  if (!(this->loadFlags & GRPFILE3D_INCLUDE_GEOMETRY) && !(this->loadFlags & GRPFILE3D_INCLUDE_NEW_GEOMETRY))							
							SkipArray();
						else
						{
							FBXVECTOR<double>* array	=ReadArrayDouble(arraylength);
							newnode->data->SetArrayValues(array);
						}
					}
					else
					if (name.Compare(__L("UV"),2,false)==0)
					{
						if (!(this->loadFlags & GRPFILE3D_INCLUDE_GEOMETRY) && !(this->loadFlags & GRPFILE3D_INCLUDE_NEW_GEOMETRY))							
							SkipArray();
						else
						{
						FBXVECTOR<double>* array	=ReadArrayDouble(arraylength);
						newnode->data->SetArrayValues(array);
						}
					}
					else				
					if (name.Compare(__L("PolygonVertexIndex"),18,false)==0)
					{
					  if (!(this->loadFlags & GRPFILE3D_INCLUDE_GEOMETRY) && !(this->loadFlags & GRPFILE3D_INCLUDE_NEW_GEOMETRY))							
							SkipArray();
						else
						{
						FBXVECTOR<long long>* array	=ReadArrayIndex(arraylength);
						newnode->data->SetArrayIndex(array);
						}
					}
#ifdef GRPVERTEX_EXTENDED
					else
					if (name.Compare(__L("Binormals"),9,false)==0)
					{
						if (!(this->loadFlags & GRPFILE3D_INCLUDE_GEOMETRY) && !(this->loadFlags & GRPFILE3D_INCLUDE_NEW_GEOMETRY))							
							SkipArray();
						else
						{
						FBXVECTOR<double>* array	=ReadArrayDouble(arraylength);
						newnode->data->SetArrayValues(array);
						}
					}
					else
					if (name.Compare(__L("Tangents"),8,false)==0)
					{
						if (!(this->loadFlags & GRPFILE3D_INCLUDE_GEOMETRY) && !(this->loadFlags & GRPFILE3D_INCLUDE_NEW_GEOMETRY))							
						SkipArray();
						else
						{
						FBXVECTOR<double>* array	=ReadArrayDouble(arraylength);
						newnode->data->SetArrayValues(array);
						}
					}
#endif
					else
					if (name.Compare(__L("Edges"),5,false)==0)
					{
					//	if (!(this->loadFlags & GRPFILE3D_INCLUDE_GEOMETRY) && !(this->loadFlags & GRPFILE3D_INCLUDE_NEW_GEOMETRY))							SkipArray();
						SkipArray();

						//XVECTOR<long long>* array	=ReadArrayIndex(arraylength);
						//newnode->data->SetArrayIndex(array);
					}
					else
					if (name.Compare(__L("UVIndex"),7,false)==0)
					{
						if (!(this->loadFlags & GRPFILE3D_INCLUDE_GEOMETRY) && !(this->loadFlags & GRPFILE3D_INCLUDE_NEW_GEOMETRY))							
							SkipArray();
						else
						{
						FBXVECTOR<long long>* array	=ReadArrayIndex(arraylength);
						newnode->data->SetArrayIndex(array);
						}
					}					
					else
					if (name.Compare(__L("Matrix"),6,false)==0)
					{
						FBXVECTOR<double>* array	=ReadArrayDouble(arraylength);
						newnode->data->SetArrayValues(array);
					}
					else
					if (name.Compare(__L("Weights"),7,false)==0)
					{
						if (!(this->loadFlags & GRPFILE3D_INCLUDE_DEFORMERS))							
							SkipArray();
						else
						{
						FBXVECTOR<double>* array	=ReadArrayDouble(arraylength);
						newnode->data->SetArrayValues(array);
						}
					}
					else
					if (name.Compare(__L("Indexes"),7,false)==0)
					{
						if (!(this->loadFlags & GRPFILE3D_INCLUDE_GEOMETRY) && !(this->loadFlags & GRPFILE3D_INCLUDE_NEW_GEOMETRY))							
							SkipArray();
						else
						{
						FBXVECTOR<long long>* array	=ReadArrayIndex(arraylength);
						newnode->data->SetArrayIndex(array);
						}
					}
					else
					if (name.Compare(__L("Transform"),9,false)==0)
					{
						if (!(this->loadFlags & GRPFILE3D_INCLUDE_DEFORMERS))							
							SkipArray();
						else
						{
						FBXVECTOR<double>* array	=ReadArrayDouble(arraylength);
						newnode->data->SetArrayValues(array);
						}
					}
					else
					if (name.Compare(__L("TransformLink"),13,false)==0)
					{
						if (!(this->loadFlags & GRPFILE3D_INCLUDE_DEFORMERS))							
							SkipArray();
						else
						{
						FBXVECTOR<double>* array	=ReadArrayDouble(arraylength);
						newnode->data->SetArrayValues(array);
						}
					}
					else		
					{
						ConsumeBlock();
					}					
					


					continue;
				//------------------------ 					
				}
				continue;
			}


		if (line.FindCharacter(__C('{'),FindColon)!=XSTRING_NOTFOUND)
		{	
			FBXNode*	newnode=CreateFBXNode();
								if (newnode==NULL) return false;
								newnode->data=CreateFBXData();

								if (newnode->data==NULL) return false;
								newnode->data->name.Set(name.Get());	

								int quoteposition=NOTFOUND;
								int colonposition=NOTFOUND;
								int spaceposition=line.FindCharacter(__C(' '));
								if (line.Get()[spaceposition+1]==__C('"'))
										quoteposition=spaceposition+1;
								else
										colonposition=line.FindCharacter(__C(','));							

								if (colonposition!=NOTFOUND || quoteposition!=NOTFOUND)
								{
									if (quoteposition==NOTFOUND) //viene un tag de numero
									{										
											int st	=spaceposition;//line.FindCharacter(__C(' '));
											line.Copy(st+1,colonposition,newnode->data->value);
											st=colonposition;

											quoteposition	=line.FindCharacter(__C('"'),st);
											if (quoteposition!=NOTFOUND)
											{
													int quoteend			=line.FindCharacter(__C('"'),quoteposition+1);
													line.Copy(quoteposition+1,quoteend,newnode->data->label);

													quoteposition	=line.FindCharacter(__C('"'),quoteend+1);
													if (quoteposition!=NOTFOUND)
													{
														int quoteend			=line.FindCharacter(__C('"'),quoteposition+1);
														line.Copy(quoteposition+1,quoteend,newnode->data->subtype);
													}
											}
									}
									else
									{
											int st	=line.FindCharacter(__C('"'));
											int end	=line.FindCharacter(__C('"'),st+1);
											line.Copy(st+1,end,newnode->data->value);
											st=end+1;

											quoteposition	=line.FindCharacter(__C('"'),st);
											if (quoteposition!=NOTFOUND)
											{
													int quoteend			=line.FindCharacter(__C('"'),quoteposition+1);
													line.Copy(quoteposition+1,quoteend,newnode->data->label);
											}
									}
								}

										
			if (ReadNode(newnode))
			if (!node->Children->Add(newnode)) 
				return false;
			continue;
		}

	

		if (FindColon!=XSTRING_NOTFOUND)
		{			
			if (FindColon==1 && firstcharacter=='P')
			{
								FBXNode* nnode=CreateFBXNode();
									if (nnode==NULL) return false;;
								 nnode->data=CreateFBXData();
								 if (nnode->data==NULL) return false;;

				XDWORD st=0,end=0,prev=0;
				st=line.FindCharacter(__C('"'));			
				end=line.FindCharacter(__C('"'),st+1);				
				line.Copy(st+1, end, nnode->data->name);
				st=line.FindCharacter(__C('"'),end+2);
				end=line.FindCharacter(__C('"'),st+1);

				line.Copy(st+1,end,nnode->data->type);
				st=line.FindCharacter(__C('"'),end+1);
				end=line.FindCharacter(__C('"'),st+1);

				line.Copy(st+1,end,nnode->data->subtype);
				st=line.FindCharacter(__C('"'),end+1);
				end=line.FindCharacter(__C('"'),st+1);
				
				line.Copy(st+1,end,nnode->data->label);
				prev=st;
				st=line.FindCharacter(__C('"'),end+1);
				if (st!=XSTRING_NOTFOUND)		
				{
					end=line.FindCharacter(__C('"'),st+1);
					line.Copy(st+1,end,nnode->data->value);
				}
				else
				{
					st=line.FindCharacter(__C(','),prev);

					if (st!=XSTRING_NOTFOUND)											
						line.Copy(st+1,line.GetSize(),nnode->data->value);
					
				}
				if (!node->Children->Add(nnode)) return false;;
				
				continue;
			}
			else
			//if (ConnectionString.Compare(type,true)==0) //connections C:
			if (FindColon==1 && firstcharacter=='C')
			{
				FBXNode* nnode=CreateFBXNode();
								 nnode->data=CreateFBXData();
						

				XDWORD st=0,end; //OPT: connection name is not used
				st=line.FindCharacter(__C(' '));
				end=line.FindCharacter(__C(','),st+1);

				line.Copy(st+2,end-1, nnode->data->name);
			//	nnode->data->name.DeleteCharacter(__C('"'));

				st=line.FindCharacter(__C(','),end);
				end=line.FindCharacter(__C(','),st+1);
				line.Copy(st+1,end, nnode->data->value);
				
				st=line.FindCharacter(__C(','),end);
				if ((end=line.FindCharacter(__C(','),st+1))==XSTRING_NOTFOUND)				
				{
					end=line.GetSize();
					line.Copy(st+1,end, nnode->data->label);
				}			
				else
				{
					line.Copy(st+1,end, nnode->data->label);
					st=line.FindCharacter(__C(','),end);
					if ((end=line.FindCharacter(__C(','),st+1))==XSTRING_NOTFOUND)
						end=line.GetSize();
					line.Copy(st+3,end-1, nnode->data->type);
					//nnode->data->type.DeleteCharacter(__C('"'));

				}
				if (!node->Children->Add(nnode)) return false;;


			}
			else
			{
				FBXNode* nnode=CreateFBXNode();
									if (nnode==NULL) return false;;

								 nnode->data=CreateFBXData();
								 if (nnode->data==NULL) return false;;

								 nnode->data->name.Set(name.Get());
								 line.Copy(FindColon+2, nnode->data->value);
								 //nnode->data->value.DeleteCharacter(__C(' '),XSTRINGCONTEXT_ATFIRST);
								 //nnode->data->value.AdjustSize();

				if (!node->Children->Add(nnode)) return false;
				continue;			
			}
		}

	
	}
	return true;

}


/*-------------------------------------------------------------------
//	GRPFILE3DFBX::ReadArray
*/	
/**	
//	Reads a Double data array
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			20/01/2015 13:05:26
//	
//	@return 			XVECTOR<double>* : Double array
//
//  @param				XDWORD size : num of elements
//
*/
/*-----------------------------------------------------------------*/
void GRPFILE3DFBX::SkipArray()
{
	XSTRING* line=NULL;
	line=xfiletxt->GetLine(++CurrentLine);
	UpdateTask();
	XDWORD endposition=0;

	while (true)
	{
		endposition=line->FindCharacter(__C('}'),0,true);

		if (endposition==XSTRING_NOTFOUND)		
		{
			line=xfiletxt->GetLine(++CurrentLine);			
			UpdateTask();
		}
		else
			return;
	}
	return;
}

double naive(XCHAR *p) {
    double r = 0.0;
    bool neg = false;
		unsigned long long exp=1;
    if (*p == '-') {
        neg = true;
        ++p;
    }
    while (*p >= '0' && *p <= '9') {
        r = (r*10.0) + (*p - '0');
        ++p;
    }
    if (*p == '.') {
        double f = 0.0;
        int n = 0;
        ++p;
				
        while (*p >= '0' && *p <= '9') {
            f = (f*10.0) + (*p - '0');
            ++p;
            ++n;
						exp*=10;
        }
        r += f /pow(10.0, n);
    }
    if (neg) {
        r = -r;
    }
    return r;
}

#define white_space(c) ((c) == ' ' || (c) == '\t')
#define valid_digit(c) ((c) >= '0' && (c) <= '9')

double atof (XCHAR *p)
{
    int frac;
    double sign, value, scale;

    // Skip leading white space, if any.

    while (white_space(*p) ) {
        p++;
    }

    // Get sign, if any.

    sign = 1.0;
    if (*p == '-') {
        sign = -1.0;
        p++;

    } else if (*p == '+') {
        p++;
    }

    // Get digits before decimal point or exponent, if any.

    for (value = 0.0; valid_digit(*p); p++) {
        value = value * 10.0 + (*p - '0');
    }

    // Get digits after decimal point, if any.

    if (*p == '.') {
        double pow10 = 10.0;
        p += 1;
        while (valid_digit(*p)) {
            value += (*p - '0') / pow10;
            pow10 *= 10.0;
            p++;
        }
    }

    // Handle exponent, if any.

    frac = 0;
    scale = 1.0;
    if ((*p == 'e') || (*p == 'E')) {
        unsigned int expon;

        // Get sign of exponent, if any.

        p++;
        if (*p == '-') {
            frac = 1;
            p += 1;

        } else if (*p == '+') {
            p++;
        }

        // Get digits of exponent, if any.

        for (expon = 0; valid_digit(*p); p++) {
            expon = expon * 10 + (*p - '0');
        }
        if (expon > 308) expon = 308;

        // Calculate scaling factor.

        while (expon >= 50) { scale *= 1E50; expon -= 50; }
        while (expon >=  8) { scale *= 1E8;  expon -=  8; }
        while (expon >   0) { scale *= 10.0; expon -=  1; }
    }

    // Return signed and scaled floating point result.

    return sign * (frac ? (value / scale) : (value * scale));
}

FBXVECTOR<double>* GRPFILE3DFBX::ReadArrayDouble(XDWORD size)
{
	FBXVECTOR<double>* array=new FBXVECTOR<double>();
	array->SetAddInLimit(size);
	//array->Resize(size);
	
	XSTRING* line=NULL;
	line=xfiletxt->GetLine(++CurrentLine);
	UpdateTask();

	XDWORD position=0;
	position=line->FindCharacter(__C(' '))+1;

	XDWORD e=0;
	XDWORD start=0,endposition=0;
	XDWORD linesize=line->GetSize();
	while (array->GetSize()<size)
	{
		start=position;
		endposition=line->FindCharacter(__C(','),position);

		if (endposition==XSTRING_NOTFOUND)			
			endposition=linesize;			
		
		//	line->Copy(start,endposition,value);		
		//	double v=value.Fast_ConvertToDouble();
			double v=atof(&(line->Get()[start]));
			//double v=line->Fast_ConvertToDouble(start);
			//double v=naive(&(line->Get()[start]));
			array->Add(v);
			

			position=endposition+1;

			if (position>=linesize)
			{
			line=xfiletxt->GetLine(++CurrentLine);
			UpdateTask();
			linesize=line->GetSize();
			position=0;
			}
	}
	return array;
}





/*-------------------------------------------------------------------
//	GRPFILE3DFBX::ReadArray
*/	
/**	
//	Reads a long long data array
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			20/01/2015 13:05:26
//	
//	@return 			XVECTOR<long long>* : Double array
//
//  @param				XDWORD size : num of elements
//
*/
/*-----------------------------------------------------------------*/
#include <stdlib.h> 


long long convert(XCHAR* c)
{
long long x=0;
bool sign=false;
if (*c==__C('-'))
{
		sign=true;
		c++;
}

for(; (*c>47)&&(*c<58);c++)
  x = (long long)((((x)<<1) + ((x)<<3)) + *c - 48);
return sign ? -x : x;
}


FBXVECTOR<long long>*	GRPFILE3DFBX::ReadArrayIndex(XDWORD size)
{
	FBXVECTOR<long long>* array=new FBXVECTOR<long long>();	
	array->SetAddInLimit(size);
	array->SetIsMulti(true);
	
	XSTRING* line;
	line=xfiletxt->GetLine(++CurrentLine);
	UpdateTask();

	XSTRING value;

	XDWORD position=0,endposition=0;

	XDWORD e=0;
	XDWORD start=0;
	position=line->FindCharacter(__C(' '))+1;


	while (array->GetSize()<size)
	{
		start=position;
		endposition=line->FindCharacter(__C(','),position);

		if (endposition==XSTRING_NOTFOUND)			
			endposition=line->GetSize();			
		
			//line->Copy(start,endposition,value);		
			//long long v= (long long)value.Fast_ConvertToQWord();
			//long long v= (long long)line->Fast_ConvertToSQWord(start);
				long long v=(long long)convert(&(line->Get()[start]));
			//long long v=(long long)atoi((char*)&(line->Get()[start]));
			array->Add(v);

			position=endposition+1;

			if (position>=line->GetSize())
			{
			line=xfiletxt->GetLine(++CurrentLine);
			UpdateTask();
			position=0;
			}
	}
	return array;
}

//*/
/*-------------------------------------------------------------------
//	GRPFILE3DFBX::FindNode
*/	
/**	
//	Search recursively for a node inside a node
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			20/01/2015 13:58:14
//	
//	@return 			bool : TRUE if found
//
//  @param				name : Node name searching
//  @param				FBXNode** : Current Node 
*/
/*-----------------------------------------------------------------*/
bool	GRPFILE3DFBX::FindNode(XCHAR* name,int size,FBXNode** node,bool Case)
{
	*node=NULL;
	/*
	if (XSTRING::GetSize(name)!=size)
	{
		XDEBUG_PRINTCOLOR(4,__L("error size"));
	}
	*/

	return FindNode(name,size,Root,node);
}

bool	GRPFILE3DFBX::FindNode(XCHAR* name,int size, FBXNode* CurrentNode,FBXNode** node,bool Case)
{
	/*
	if (XSTRING::GetSize(name)!=size)
	{
		XDEBUG_PRINTCOLOR(4,__L("error size"));
	}
	*/

	if (CurrentNode->data!=NULL)
			if (CurrentNode->data->name.Compare(name,size,Case)==0)
			{
				*node=CurrentNode;
				return true;
			}

	if (CurrentNode->Children!=NULL)
	{
		XDWORD nChildren=CurrentNode->Children->GetSize();
		for(XDWORD e=CurrentNode->LastIndex;e<nChildren;e++)		
			if (FindNode(name,size,CurrentNode->Children->FastGet(e),node,Case))
			{
				CurrentNode->LastIndex=e+1;
				return true;		
			}
		for(XDWORD e=0;e<CurrentNode->LastIndex;e++)		
			if (FindNode(name,size,CurrentNode->Children->FastGet(e),node,Case))
			{
				CurrentNode->LastIndex=e+1;
				return true;		
			}
	}
	
	return false;
}

bool	GRPFILE3DFBX::FindChildNode(XCHAR* name,int size,FBXNode* CurrentNode,FBXNode** node,bool Case)
{
	if (CurrentNode->data!=NULL)
			if (CurrentNode->data->name.Compare(name,size,Case)==0)
			{
				*node=CurrentNode;
				return true;
			}

	if (CurrentNode->Children!=NULL)
	{
		XDWORD nChildren=CurrentNode->Children->GetSize();
		for(XDWORD e=CurrentNode->LastIndex;e<nChildren;e++)	
		{
			FBXNode* child=CurrentNode->Children->FastGet(e);
			if (child->data->name.Compare(name,size,Case)==0)
			{
				*node=child;
					CurrentNode->LastIndex=e+1;
				return true;
			}
		}

		for(XDWORD e=0;e<CurrentNode->LastIndex;e++)		
		{
			FBXNode* child=CurrentNode->Children->FastGet(e);
					if (child->data->name.Compare(name,size,Case)==0)
					{
						*node=child;
							CurrentNode->LastIndex=e+1;
						return true;
					}
		}
	}
	
	return false;
}


/*-------------------------------------------------------------------
//	GRPFILE3DFBX::LoadObjects
*/	
/**	
//	Parses [Objects] node
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			20/01/2015 16:31:29
//	
//	@return 			bool : TRUE if success
//
*/
/*-----------------------------------------------------------------*/
bool GRPFILE3DFBX::LoadObjects()
{
			FBXNode* connectionsNode=NULL;
			FBXNode* node=NULL;
		
			LoadGlobalSettings();		
			


			if (Objects->Children->GetSize()==0)
			{
				if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_ERRORS)
					{
						FBX_DEBUG(FBX_ERROR,__L("FBX: Error, No Objects Node in File"));
					}
				return false;
			}

			int size=(int)Objects->Children->GetSize();
			
			CurrentStep=GRPFILE3DFBX_STEP_LOADOBJECTS;
			currentObject=0;

			//for (int e=size-1;e>=0;e--)
			for (int e=0;e<size;e++)
			{
				currentObject++;
				UpdateTask();
				FBXNode* node	=Objects->Children->FastGet(e);
				FBXSTRING* type	=&node->data->name;

				XQWORD id			=node->data->value.Fast_ConvertToQWord();		

				//------------------------------------------ Load AnimationLayer								
				if (type->Compare(__L("AnimationCurve"),14,false)==0)
				{										
							if (!LoadAnimationCurve(id,node))
									{
										if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_ERRORS)				FBX_DEBUG(FBX_ERROR,__L("FBX: Error loading Animation Curve %u"),id);
										return false;
									}	

							TypesMap.Add(id,GRPFILE3DFBX_CONNECTION_TYPE_ANIMATION_CURVE);
							if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_PROCESSING)					FBX_DEBUG(FBX_PROCESS,__L("Processed AnimationCurve %u"),id);
								continue;
				}			
				//------------------------------------------ Load AnimationCurveNode
				if (type->Compare(__L("AnimationCurveNode"),18,false)==0)
				{															
							if (LoadAnimationNode(id,node))
							{
								TypesMap.Add(id,GRPFILE3DFBX_CONNECTION_TYPE_ANIMATION_NODE);
								if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_PROCESSING)				FBX_DEBUG(FBX_PROCESS,__L("Processed AnimationCurveNode %u"),id);
								continue;
							}					
							else
							{

							}
				}					
				
				//------------------------------------------ Load Node					
				if (type->Compare(__L("Model"),5,false)==0)
				{												
				if (node->data->subtype.Compare(__L("Mesh"),4,false)==0)			
					{
						TypesMap.Add(id,GRPFILE3DFBX_CONNECTION_TYPE_MESH);
						
						XCHAR* name=node->data->label.Get();											
						GRPNODE* scene_node=object->FindNode(name);
						if (scene_node)
						{
							this->NodeMap.Add(id,scene_node);
							 scene_node->IsOrphan=false;
							continue;
						}						
						
							GRPNODE* newnode= object->CreateNode();//new GRPNODE();//object->CreateNode();
											 newnode->IsOrphan=true;

							newnode->SetName(name);					

							if (!this->NodeMap.Add(id,newnode))
							{
								delete(newnode);
							}
							else
							if (!LoadModelNode(id,node))
									{
										if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_ERRORS)    FBX_DEBUG(FBX_ERROR,__L("FBX: Error loading model %u"),id);
										return false;
									}			
							if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_PROCESSING)			FBX_DEBUG(FBX_PROCESS,__L("Processed Model %u"),id);

							continue;
					}				

					if (node->data->subtype.Compare(__L("Camera"),6,false)==0)			
					{
						TypesMap.Add(id,GRPFILE3DFBX_CONNECTION_TYPE_CAMERA);						
						XCHAR* name=node->data->label.Get();
												
						GRPCAMERA* camera_node=object->FindCamera(name);
						if (camera_node)
						{
							this->CameraMap.Add(id,camera_node);
							 camera_node->IsOrphan=false;
							continue;
						}

							GRPCAMERA* element=new GRPCAMERA();
							//GRPCAMERA* element = object->CreateCamera();
												 element->SetName(node->data->label.Get());			
												 element->SetUpVector(object->UpVector.vector[0], object->UpVector.vector[1], object->UpVector.vector[2]);
							this->CameraMap.Add(id,element);
							
							if (!LoadModelNode(id,node))
									{
										if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_ERRORS)		FBX_DEBUG(FBX_ERROR,__L("FBX: Error loading Camera node %u"),id);
										return false;
									}							

							if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_PROCESSING)			FBX_DEBUG(FBX_PROCESS,__L("Processed Camera %u"),id);
							continue;
					}

					if (node->data->subtype.Compare(__L("Light"),5,false)==0)			
					{
						TypesMap.Add(id,GRPFILE3DFBX_CONNECTION_TYPE_LIGHT);
						
						XCHAR* name=node->data->label.Get();
						
						GRPLIGHT* scene_node=object->FindLight(name);
						if (scene_node)
						{
							this->LightMap.Add(id,scene_node);
							 scene_node->IsOrphan=false;
							continue;
						}

							GRPLIGHT* element=new GRPLIGHT();
							//GRPLIGHT* element=object->CreateLight();
												element->SetName(node->data->label.Get());														
												TypesMap.Add(id,GRPFILE3DFBX_CONNECTION_TYPE_LIGHT);
							this->LightMap.Add(id, element);

							if (!LoadModelNode(id,node))
									{
										if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_ERRORS)   FBX_DEBUG(FBX_ERROR,__L("FBX: Error loading light node %u"),id);
										return false;
									}			

							//if (loadFlags & GRPFILE3D_INCLUDE_LOCATORS)
							//		object->Add(element);

							if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_PROCESSING)			FBX_DEBUG(FBX_PROCESS,__L("Processed Light %u"),id);
							continue;
					}

					if (node->data->subtype.Compare(__L("LimbNode"),8,false)==0)			
					{
							GRPSKINLIMB* limb=new GRPSKINLIMB();
							limb->SetName(node->data->label.Get());	
							this->SkinLimbMap.Add(id,limb);
							TypesMap.Add(id,GRPFILE3DFBX_CONNECTION_TYPE_SKIN_LIMB);
							if (!LoadModelNode(id,node))
									{
										if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_ERRORS)		FBX_DEBUG(FBX_ERROR,__L("FBX: Error loading limb node %u"),id);
										return false;
									}	
							if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_PROCESSING) 			FBX_DEBUG(FBX_PROCESS,__L("Processed Limbnode %u"),id);
							continue;
					}


						if (node->data->subtype.Compare(__L("Root"),4,false)==0)			
						{
						TypesMap.Add(id,GRPFILE3DFBX_CONNECTION_TYPE_SKIN_ROOT);
						XCHAR* name=node->data->label.Get();
						
						GRPNODE* scene_node=object->FindNode(name);
						if (scene_node)
						{
							this->LocatorMap.Add(id,scene_node);
							this->SkinRootMap.Add(id,scene_node);
							 scene_node->IsOrphan=false;
							continue;
						}

								GRPNODE* location=object->CreateNode();;
								//GRPNODE* location=new GRPNODE();
								location->SetName(node->data->label.Get());	
								if (!this->LocatorMap.Add(id,location))
								{
									delete(location);
								}
								else
								this->SkinRootMap.Add(id,location);

								if (!LoadModelNode(id,node))
										{
											if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_ERRORS)		FBX_DEBUG(FBX_ERROR,__L("FBX: Error loading Null Node %u"),id);
											return false;
										}	
								if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_PROCESSING)			FBX_DEBUG(FBX_PROCESS,__L("Processed Locator %u"),id);
								continue;
						}			

						if (node->data->subtype.Compare(__L("Null"),4,false)==0)			
						{
						TypesMap.Add(id,GRPFILE3DFBX_CONNECTION_TYPE_LOCATOR);
						XCHAR* name=node->data->label.Get();
						
						GRPNODE* scene_node=object->FindNode(name);
						if (scene_node)
						{
							this->LocatorMap.Add(id,scene_node);
							 scene_node->IsOrphan=false;
							continue;
						}

									GRPNODE*	location=new GRPNODE();									
									//GRPNODE*	location=object->CreateNode();									
														location->SetName(node->data->label.Get());	
								if (!this->LocatorMap.Add(id,location))
								{
									delete(location);
								}
								
								if (!LoadModelNode(id,node))
										{
											if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_ERRORS)		FBX_DEBUG(FBX_ERROR,__L("FBX: Error loading Null Node %u"),id);
											return false;
										}	
								if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_PROCESSING)			FBX_DEBUG(FBX_PROCESS,__L("Processed Locator %u"),id);
								continue;
						}												
						continue;
				}	
					//------------------------------------------ Load Geometry
				if (type->Compare(__L("Geometry"),8,false)==0)
				{						
						if (node->data->subtype.Compare(__L("Mesh"),4,false)==0)
						{
							if (!LoadGeometryNode(id,node))
									{
										if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_ERRORS)		FBX_DEBUG(FBX_ERROR,__L("FBX: Error loading geometry %u"),id);
										return false;
									}		
							TypesMap.Add(id,GRPFILE3DFBX_CONNECTION_TYPE_GEOMETRY);		
							if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_PROCESSING)			FBX_DEBUG(FBX_PROCESS,__L("Processed Mesh Geometry %u"),id);
						}
						else							
							if (node->data->subtype.Compare(__L("Shape"),5,false)==0)
							{
								if (!LoadMorpherShapeNode(id,node))
								{
										if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_ERRORS)		FBX_DEBUG(FBX_ERROR,__L("FBX: Error loading geometry morph shape %u"),id);
										return false;
								}		
								TypesMap.Add(id,GRPFILE3DFBX_CONNECTION_TYPE_SUBDEFORMER_SHAPE);	

								if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_PROCESSING)			FBX_DEBUG(FBX_PROCESS,__L("Processed Morph Geometry %u"),id);
							}
						else
							if (node->data->subtype.Compare(__L("NurbsSurface"),12,false)==0)
							{
										if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_ERRORS)		FBX_DEBUG(FBX_ERROR,__L("FBX: Error! NurbsSurfaces Not Supported %u"),id);
										return false;
							}
							else
									 if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_WARNINGS)		FBX_DEBUG(FBX_WARNING, __L("FBX: Warning! Unknown geometry type %u"), id);

						continue;
				}
	
				//------------------------------------------ Load Material					
				if (type->Compare(__L("Material"),8,false)==0)
				{																					

							if (!LoadMaterialNode(id,node))
									{
										if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_ERRORS)		FBX_DEBUG(FBX_ERROR,__L("FBX: Error loading Material %u"),id);
										return false;
									}				

							if (!	TypesMap.Set(id,GRPFILE3DFBX_CONNECTION_TYPE_MATERIAL))
										TypesMap.Add(id,GRPFILE3DFBX_CONNECTION_TYPE_MATERIAL);

							if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_PROCESSING)
							FBX_DEBUG(FBX_PROCESS,__L("Processed Material %u"),id);
						continue;
				}		

				//------------------------------------------ Load Material					
				if (type->Compare(__L("Texture"),7,false)==0)
				{																																																										
							if (!LoadTextureNode(id,node))
									{
										if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_ERRORS)			FBX_DEBUG(FBX_ERROR,__L("FBX: Error loading texture %u"),id);
										return false;
									}			

							TypesMap.Add(id,GRPFILE3DFBX_CONNECTION_TYPE_TEXTURE);

							if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_PROCESSING)				FBX_DEBUG(FBX_PROCESS,__L("Processed Texture %u"),id);

						continue;
				}		

				//------------------------------------------ Load Video	
				if (type->Compare(__L("Video"),5,false)==0)
				{																																																																	
							TypesMap.Add(id,GRPFILE3DFBX_CONNECTION_TYPE_VIDEO);
							if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_PROCESSING)
							FBX_DEBUG(FBX_PROCESS,__L("Processed Video %u"),id);
						continue;
				}		

				//------------------------------------------ Load Node Attribute (camera,light)					
				if (type->Compare(__L("NodeAttribute"),13,false)==0)
				{								
							if (!LoadNodeAttribute(id,node))
									{
										if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_ERRORS)				FBX_DEBUG(FBX_ERROR,__L("FBX: Error loading Node Attribute %u"),id);
										return false;
									}								
							if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_PROCESSING)					FBX_DEBUG(FBX_PROCESS,__L("Processed NodeAttribute %s %u "),node->data->subtype.Get(),id);
									continue;
				}	

				/*
				//------------------------------------------ Load Constraints				
				if (type->Compare(__L("Constraint"),false)==0)
				{
							if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_WARNINGS)						FBX_DEBUG(FBX_WARNING,__L("\t\tFBX: Warning Constraints are unsupported: %u %s%s - Skipping"), id, node->data->label.Get(),node->data->subtype.Get());													

							TypesMap.Add(id,GRPFILE3DFBX_CONNECTION_TYPE_CONSTRAINT);

							if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_PROCESSING)					FBX_DEBUG(FBX_PROCESS,__L("Processed Constraint %u"),id);
								continue;
				
				}
				*/

				//------------------------------------------ Load AnimationStack								
				if (type->Compare(__L("AnimationStack"),14,false)==0)
				{										
							if (!LoadAnimationStackNode(id,node))
									{
										if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_ERRORS)				FBX_DEBUG(FBX_ERROR,__L("FBX: Error loading Animation Stack %u"),id);
										return false;
									}					
							
							TypesMap.Add(id,GRPFILE3DFBX_CONNECTION_TYPE_ANIMATION_STACK);

							if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_PROCESSING)					FBX_DEBUG(FBX_PROCESS,__L("Processed AnimationStack %u"),id);
								continue;
				}	
				//------------------------------------------ Load AnimationLayer								
				if (type->Compare(__L("AnimationLayer"),14,false)==0)
				{										
							if (!LoadAnimationLayerNode(id,node))
									{
										if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_ERRORS)				FBX_DEBUG(FBX_ERROR,__L("FBX: Error loading Animation Layer %u"),id);
										return false;
									}																		

							TypesMap.Add(id,GRPFILE3DFBX_CONNECTION_TYPE_ANIMATION_LAYER);
							if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_PROCESSING)					FBX_DEBUG(FBX_PROCESS,__L("Processed AnimationLayer %u"),id);
						continue;
				}		

				//------------------------------------------ Load Deformers	
				if (type->Compare(__L("Deformer"),8,false)==0)
				{
					if (node->data->subtype.Compare(__L("BlendShape"))==0)
					{
							LoadDeformer(id,node);
							TypesMap.Add(id,GRPFILE3DFBX_CONNECTION_TYPE_DEFORMER_MORPH);
							if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_PROCESSING)						FBX_DEBUG(FBX_PROCESS,__L("Processed BlendShape %u"),id);
								continue;
					}					
					if (node->data->subtype.Compare(__L("BlendShapeChannel"),17,false)==0)
					{
							LoadSubdeformer(id,node);
							TypesMap.Add(id,GRPFILE3DFBX_CONNECTION_TYPE_SUBDEFORMER_MORPH);
							if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_PROCESSING)						FBX_DEBUG(FBX_PROCESS,__L("Processed BlendShapeChannel %u"),id);
								continue;
					}
					
					if (node->data->subtype.Compare(__L("Skin"),4,false)==0)
					{
							LoadSkinDeformer(id,node);
							TypesMap.Add(id,GRPFILE3DFBX_CONNECTION_TYPE_DEFORMER_SKIN);

						if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_PROCESSING)						FBX_DEBUG(FBX_PROCESS,__L("Processed Skin %u"),id);
								continue;
					}
					
					if (node->data->subtype.Compare(__L("Cluster"),7,false)==0)
					{				
							LoadSkinCluster(id,node);
							TypesMap.Add(id,GRPFILE3DFBX_CONNECTION_TYPE_SUBDEFORMER_CLUSTER);

						if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_PROCESSING)						FBX_DEBUG(FBX_PROCESS,__L("Processed Cluster %u"),id);
								continue;
					}				

				
				}
				else
					if (type->Compare(__L("Pose"),4,false)==0)
					{
							LoadSkinPose(id,node);									
							TypesMap.Add(id,GRPFILE3DFBX_CONNECTION_TYPE_SKIN_POSE);

								if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_PROCESSING)				FBX_DEBUG(FBX_PROCESS,__L("Processed Pose %u"),id);
							continue;
					}	
				else
					if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_WARNINGS)
						FBX_DEBUG(FBX_WARNING,__L("\t\tFBX: x  Warning unrecognized or unsupported type: %lld %s %s %s - Skipping"), id, node->data->name.Get(),node->data->label.Get(),node->data->subtype.Get());															
			}

			return true;
}


/*-------------------------------------------------------------------
//	GRPFILE3DFBX::LoadHeaderData
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			23/11/2015 10:43:06
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPFILE3DFBX::LoadHeaderData()
{
			FBXNode* Header=NULL;

		if (this->FindNode(__L("FBXHeaderExtension"),18,Root,&Header))
		{
			FBXNode* appnode=NULL;
			if (this->FindNode(__L("Original|ApplicationName"),24,Header,&appnode))
			{
					FBXSTRING app;
					app.Set(appnode->data->value);

					if (app.Compare(__L("Maya"),4,false)==0)
					{
						this->Application=GRPFILE3DFBX_APPLICATION_TYPE_MAYA;
						if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_STATUS)						FBX_DEBUG(FBX_STATUS,__L("FBX mode: Maya"));
					}
					else
					if (app.Compare(__L("3ds Max"),7,false)==0)
					{
						this->Application=GRPFILE3DFBX_APPLICATION_TYPE_3DSMAX;
						if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_STATUS)						FBX_DEBUG(FBX_STATUS,__L("FBX mode: 3DS Max"));
					}
					return true;
			}			
		}

		return false;		
}


/*-------------------------------------------------------------------
//	GRPFILE3DFBX::LoadNameData
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			23/11/2015 10:44:36
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPFILE3DFBX::LoadNameData()
{
	//-------------- get Objects
			if (!this->FindChildNode(__L("Objects"),7,Root,&Objects))
			{
				if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_ERRORS)  FBX_DEBUG(FBX_ERROR,__L("FBX: Error, No Objects Node in File"));
				return false;
			}

					XDWORD size		=Objects->Children->GetSize();
					for (XDWORD i=0;i<size;i++)
					{
						FBXData* data=Objects->Children->FastGet(i)->data;
						XQWORD id			=data->value.Fast_ConvertToQWord();		
						FBXSTRING* name	=&data->label;

						this->ObjectNameMap.Add(id,name);
						//XDEBUG_PRINTCOLOR(0,__L("ID:%lld Name:%s"),id,name->Get());
					}
			
	return true;
}

/*-------------------------------------------------------------------
//	GRPFILE3DFBX::LoadConnectionsAndProperties
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			23/11/2015 11:21:48
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPFILE3DFBX::LoadConnectionsAndProperties	()
{
		FBXNode* node=NULL;
			
			if (!this->FindChildNode(__L("Connections"),11,Root,&Connections))
										{
										if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_ERRORS)
										FBX_DEBUG(FBX_ERROR,__L("FBX: Error no Connections Node"));
										return false;
										}	

			
			XDWORD connectionSize=Connections->Children->GetSize();
			
			if (Connections->Children==NULL || Connections->Children->GetSize()==0)
										{
										if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_ERRORS)
										FBX_DEBUG(FBX_ERROR,__L("FBX: Error no Connections on File %s!"),this->xfiletxt->GetPrimaryFile()->GetPathNameFile());
										return false;
										}

			
			for (XDWORD e=0;e<connectionSize;e++)
			{
					FBXData* data=Connections->Children->FastGet(e)->data;
					XQWORD id			=data->value.Fast_ConvertToQWord();	
					XQWORD nodeid	=data->label.Fast_ConvertToQWord();	
					
					if (data->name.Get()[1]==__C('O'))
						this->ObjectConnectionsMap.Add(id,nodeid);
				else						
				{
						this->PropertyConnectionsMap.Add(id,nodeid);
						this->PropertyConnectionsTypeMap.Add(id,&data->type);
				}

			}

						
	return true;
}

/*-------------------------------------------------------------------
//	GRPFILE3DFBX::LoadDictionary
*/	
/**	
//	Parses Connections
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			26/01/2015 10:41:46
//	
//	@return 			GRPOBJECT* : Created Object
//
*/
/*-----------------------------------------------------------------*/
GRPOBJECT* GRPFILE3DFBX::LoadDictionary()
{
		//object=new GRPOBJECT();
		//if (!object)
	//		return NULL;

		if (loadFlags & GRPFILE3D_INCLUDE_GEOMETRY || loadFlags & GRPFILE3D_INCLUDE_NEW_GEOMETRY)
		{
		if (object->GetNode()==NULL)
			object->SetNode(object->CreateNode());
		object->GetNode()->SetName(__L("RootObject"));
		}
	

		//----------------------------------------------------load Headers
		if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_STATUS)
			FBX_DEBUG(0,__L("processing Dictionary : Headers"));
				if (!LoadHeaderData())
										{
										if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_ERRORS)
										FBX_DEBUG(FBX_ERROR,__L("\tFBX: Error loading header data %s!"),this->xfiletxt->GetPrimaryFile()->GetPathNameFile());
										return NULL;
										}

		//----------------------------------------------------load Properties
		if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_STATUS)
			FBX_DEBUG(0,__L("processing Dictionary : Properties"));
				if (!LoadConnectionsAndProperties())
										{
										if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_ERRORS)
										FBX_DEBUG(FBX_ERROR,__L("\tFBX: Error loading connections %s!"),this->xfiletxt->GetPrimaryFile()->GetPathNameFile());
										return NULL;
										}

		//----------------------------------------------------load Names
		if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_STATUS)
			FBX_DEBUG(0,__L("processing Dictionary : Names"));
			if (!LoadNameData	())
										{
										if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_ERRORS)
										FBX_DEBUG(FBX_ERROR,__L("\tFBX: Error loading model names %s!"),this->xfiletxt->GetPrimaryFile()->GetPathNameFile());
										return NULL;
										}

		//----------------------------------------------------load Objects
		if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_STATUS)
			FBX_DEBUG(0,__L("processing Dictionary : Objects"));
			if (!LoadObjects())
										{
										if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_ERRORS)
										FBX_DEBUG(FBX_ERROR,__L("\tFBX: Error loading data on File %s!"),this->xfiletxt->GetPrimaryFile()->GetPathNameFile());
										return NULL;
										}

			//-------------------------------------------------------------------- PROCESS CONNECTIONS
			if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_STATUS)
			FBX_DEBUG(0,__L("processing Dictionary : Connections"));
			
			CurrentStep=GRPFILE3DFBX_STEP_LOADINGDICTIONARY;
			currentDictionary=0;

			XDWORD ObjectConnectionsMapSize=ObjectConnectionsMap.GetSize();
			for (XDWORD e=0;e<ObjectConnectionsMapSize;e++)
			{						
					currentDictionary++;
					UpdateTask();

					GRPFILE3DFBX_CONNECTION_TYPE type=GRPFILE3DFBX_CONNECTION_TYPE_UNKNOWN;
					bool isProperty=false;

					XQWORD id=(XQWORD)this->ObjectConnectionsMap.GetKey(e); //Fast_ConvertToDWord(&connectionsNode->Children->Get(e)->data->value);
					XQWORD nodeid=this->ObjectConnectionsMap.GetElement(e);
					type=IsType(id);				

					FBXNode* fbxnode=Connections->Children->FastGet(e);

					if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_CONNECTIONS)
					FBX_DEBUG(FBX_STATUS,__L("Attaching %15u -> %15u"),id,fbxnode->data->label.Fast_ConvertToDWord());

					

					//--------------------------------------- Parse connection type
					switch(type)
					{
					  case GRPFILE3DFBX_CONNECTION_TYPE_SKIN_ROOT:
						case GRPFILE3DFBX_CONNECTION_TYPE_LIGHT:			
						case GRPFILE3DFBX_CONNECTION_TYPE_LOCATOR:
						case GRPFILE3DFBX_CONNECTION_TYPE_MESH:
						case GRPFILE3DFBX_CONNECTION_TYPE_MODEL:	
						case GRPFILE3DFBX_CONNECTION_TYPE_CAMERA:
																											{																												
																										//	FBXData* data	= Connections->Children->FastGet(e)->data;
																													FBXData* data=		fbxnode->data;

																													GRPNODE* ModelNode	=NULL;
																													GRPNODE* ParentNode	=NULL;																															

																												if (this->NodeMap.Find(id)!=NOTFOUND)																													
																													ModelNode=this->NodeMap.Get(id);
																												else
																												{
																												if (this->LocatorMap.Find(id)!=NOTFOUND)																													
																													ModelNode=this->LocatorMap.Get(id);
																												else
																														if (this->LightMap.Find(id) != NOTFOUND)
																														{
																																ModelNode = this->LightMap.Get(id);
																																object->Add(dynamic_cast<GRPLIGHT*>(ModelNode));
																														}
																													else
																															if (this->CameraMap.Find(id) != NOTFOUND)
																															{
																																	ModelNode = this->CameraMap.Get(id);
																																	object->Add(dynamic_cast<GRPCAMERA*>(ModelNode)); 
																															}
																																else
																																		continue;
																												}

																												if (nodeid==0) //the node is attached!																												
																												{
																														FBXSTRING * name=this->ObjectNameMap.Get(id);
																													
																													if (name!=NULL && type!=GRPFILE3DFBX_CONNECTION_TYPE_SKIN_ROOT)
																													{
																														GRPNODE* existing_node=object->FindNode(name->Get());
																														if (existing_node!=NULL)
																														{
																															if (existing_node->GetParent()==NULL)
																																existing_node->LinkTo(object->GetNode());
																															
																															/*
																															switch (type)
																															{
																															case  GRPFILE3DFBX_CONNECTION_TYPE_CAMERA:
																																	object->Add(dynamic_cast<GRPCAMERA*>(ModelNode)); break;

																															case  GRPFILE3DFBX_CONNECTION_TYPE_LIGHT:
																																	object->Add(dynamic_cast<GRPLIGHT*>(ModelNode)); break;
																															}
																															*/

																															break;		
																														}																													
																													}
																														
																													if (object)
																													{
																														ModelNode->LinkTo(object->GetNode());
																														object->GetNode()->IsOrphan=false;
																														object->Add(ModelNode);
																													}
																													break;
																												}

																												switch(IsType(nodeid))
																												{
																													case GRPFILE3DFBX_CONNECTION_TYPE_SKIN_ROOT:
																																																	break;
																													case GRPFILE3DFBX_CONNECTION_TYPE_MESH:
																													case GRPFILE3DFBX_CONNECTION_TYPE_MODEL:
																																																	if (this->NodeMap.Find(nodeid)!=NOTFOUND)																													
																																																			ParentNode=this->NodeMap.Get(nodeid);
																																																	break;
																													case GRPFILE3DFBX_CONNECTION_TYPE_LOCATOR:
																																																		if (this->LocatorMap.Find(nodeid)!=NOTFOUND)																													
																																																			ParentNode=this->LocatorMap.Get(nodeid);
																																																		break;
																													case GRPFILE3DFBX_CONNECTION_TYPE_CAMERA:
																																																		if (this->CameraMap.Find(nodeid)!=NOTFOUND)																													
																																																			ParentNode=this->CameraMap.Get(nodeid);
																																																		break;
																													case GRPFILE3DFBX_CONNECTION_TYPE_LIGHT:
																																																		if (this->LightMap.Find(nodeid)!=NOTFOUND)																													
																																																			ParentNode=this->LightMap.Get(nodeid);
																																																		break;
																													case GRPFILE3DFBX_CONNECTION_TYPE_SKIN_LIMB:
																																																		if (this->SkinLimbMap.Find(nodeid)!=NOTFOUND)																													
																																																			ParentNode=this->SkinLimbMap.Get(nodeid);
																																																		break;
																													default: break;
																												}

																												if (ParentNode!=NULL)
																													ModelNode->LinkTo(ParentNode);
																												if (ModelNode)
																													ModelNode->IsOrphan=false;
																											}
																											break;
	
						case GRPFILE3DFBX_CONNECTION_TYPE_GEOMETRY:		
																											{		
																												if (!(loadFlags & GRPFILE3D_INCLUDE_GEOMETRY) && !(loadFlags & GRPFILE3D_INCLUDE_NEW_GEOMETRY))
																													break;

																											GRPELEMENT* element=NULL;
																											element=this->GeometricMap.Get(id);

																											if (element==NULL)
																																						{
																																							if (!(loadFlags & GRPFILE3D_INCLUDE_NEW_GEOMETRY))
																																							{
																																							if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_ERRORS)
																																							FBX_DEBUG(FBX_ERROR,__L("FBX: Error, Invalid Geometry node %u"),nodeid);
																																							return NULL;
																																							}
																																							else //
																																							{																																																																																
																																								continue;
																																							}
																																						}	

																											//---- Get Element
																											
																												if (object->GetElements()->Find(element)!=NOTFOUND)//instancia
																												{
																													GRPELEMENT* instance=new GRPELEMENT();
																													instance->Reference(element);																												
																													element=instance;
																												}
																											
																											//---- Set Node			

																													if (ObjectNameMap.Find(nodeid)!=NOTFOUND)
																													{
																															FBXSTRING* nodename=ObjectNameMap.Get(nodeid);
																														GRPNODE* node=object->FindNode(nodename->Get());
																															if (node!=NULL)
																															{
																																//hay un nodo con ese nombre en la escena ya
																																	element->SetNode(node);	
																																	node->IsOrphan=false;

																																	if (object)
																																  this->object->AddElement(element);		
																																	break;
																															}						
																													}

																													if (this->NodeMap.Find(nodeid)!=NOTFOUND)
																													{										
																													GRPNODE* node=this->NodeMap.Get(nodeid);																											
																													element->SetNode(node);	
																													if (object)
																														this->object->AddElement(element);	
																													node->IsOrphan=false;
																													break;
																													}		

																														if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_ERRORS)
																															FBX_DEBUG(FBX_ERROR,__L("FBX: Error, Invalid Geometry node target %u"),nodeid);
																																return NULL;																																																																																							
																											}
																											break;
						case GRPFILE3DFBX_CONNECTION_TYPE_DEFORMER_MORPH:
																											{
																												if (this->DeformerMap.Find(id)!=NOTFOUND)
																												{
																													if (this->GeometricMap.Find(nodeid)!=NOTFOUND)
																													{
																													this->GeometricMap.Get(nodeid)->SetDeformer(this->DeformerMap.Get(id));
																													this->DeformerMap.Get(id)->SetTarget(this->GeometricMap.Get(nodeid));
																													}
																													else
																													{
																														if (!(loadFlags & GRPFILE3D_INCLUDE_DEFORMERS))
																														{
																														if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_ERRORS)
																																FBX_DEBUG(FBX_ERROR,__L("FBX: Unknown Geometry target %u "),id);
																														return NULL;
																														}		
																														 //----------- la geometria podria existir ya, para ello vamos a iterar la scene buscando objetos con deformers que se llamen como los de este
																														for (XDWORD e=0;e<this->object->GetElements()->GetSize();e++)
																														{
																															if (this->object->GetElements()->Get(e)->GetDeformer()!=NULL)
																															{																																
																																GRPDEFORMER* def=this->DeformerMap.Get(id);
																																delete(def);
																																this->DeformerMap.Delete(id);																																
																																this->DeformerMap.Add(id,this->object->GetElements()->Get(e)->GetDeformer());
																															}
																														}

																													}
																												}
																												else
																													{
																														if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_ERRORS)
																																FBX_DEBUG(FBX_ERROR,__L("FBX: Unknown Deformer %u "),id);
																														return NULL;
																													}
																											}
																											break;

						case GRPFILE3DFBX_CONNECTION_TYPE_SUBDEFORMER_SHAPE:
																											{
																												if (this->SubdeformerShapeMap.Find(id)==NOTFOUND)
																												{
																														if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_ERRORS)
																														FBX_DEBUG(FBX_ERROR,__L("FBX: Error Attaching Unknown Subdeformer Shape %u to SubDeformer layer: %u "),id,nodeid);
																														return NULL;
																												}
																												else
																												if (this->SubdeformerMap.Find(nodeid)==NOTFOUND)
																												{
																														if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_ERRORS)
																														FBX_DEBUG(FBX_ERROR,__L("FBX: Attaching Subdeformer %u to Deformer layer: %u "),id,nodeid);
																														return NULL;
																												}
																												else
																												{
																													this->SubdeformerMap.Get(nodeid)->SetShape(this->SubdeformerShapeMap.Get(id));	
																													if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_WARNINGS)
																													FBX_DEBUG(FBX_WARNING,__L("FBX: Attaching Subdeformer Shape %u to SubDeformer: %u "),id,nodeid);
																												}

																											}
																											break;
						case GRPFILE3DFBX_CONNECTION_TYPE_SUBDEFORMER_MORPH:
																											{
																													if (this->SubdeformerMap.Find(id)==NOTFOUND)
																													{
																														if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_ERRORS)
																														FBX_DEBUG(FBX_WARNING,__L("FBX: Attaching Subdeformer %u to Deformer layer: %u "),id,nodeid);
																														return NULL;
																													}

																													if (this->DeformerMap.Find(nodeid)!=NOTFOUND)
																													{
																														if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_WARNINGS)
																														FBX_DEBUG(FBX_WARNING,__L("FBX: Attaching Subdeformer %u to Deformer layer: %u "),id,nodeid);

																														GRPDEFORMER*		deformer		=this->DeformerMap.Get(nodeid);
																														GRPSUBDEFORMER* subdeformer	=this->SubdeformerMap.Get(id);

																														for (XDWORD e=0;e<deformer->GetSubdeformers()->GetSize();e++)
																														{
																															if (deformer->GetSubdeformers()->GetElement(e)->GetName()->Compare(subdeformer->GetName()->Get())==0)
																															{
																																//hemos encontrado el subdef
																																GRPSUBDEFORMER* def=this->SubdeformerMap.Get(id);
																																delete(def);
																																this->SubdeformerMap.Delete(id);
																																this->SubdeformerMap.Add(id,deformer->GetSubdeformers()->GetElement(e));
																																break;
																															}
																														}


																														deformer->Add(this->SubdeformerMap.Get(id));
																													}
																											}
																											break;
						case GRPFILE3DFBX_CONNECTION_TYPE_NODEATTRIBUTE_CAMERA:
																													{
																														GRPCAMERA* cam=this->CameraMap.Get(nodeid);
																														GRPCAMERA* att=this->CameraAttributeMap.Get(id);

																														cam->CopyFrom(att);

																														NodeAttributeMap.Add(id,nodeid);
																													}
																												break;


							case GRPFILE3DFBX_CONNECTION_TYPE_NODEATTRIBUTE_LIGHT:
																													{
																														//XDEBUG_PRINTCOLOR(3,__L("Light %lld -> %lld"),id,nodeid);
																														GRPLIGHT* light=this->LightMap.Get(nodeid);

																														if (light==NULL)
																														{
																															FBX_DEBUG(FBX_ERROR,__L("Light %u not found"),nodeid);
																															continue;
																														}

																														if (this->LightAttributeMap.Find(id)==NOTFOUND)
																														{
																															FBX_DEBUG(FBX_ERROR,__L("Light %u not found"),id);
																															continue;
																														}
																																																												
																														GRPLIGHT* att=this->LightAttributeMap.Get(id);

																														if (!light->CopyFrom(att))
																														{
																															FBX_DEBUG(FBX_ERROR,__L("Error copying light state %lld"),id);
																															continue;
																														}
																														
																														NodeAttributeMap.Add(id,nodeid);

																														//while (TagMap.Delete(id));
																														TagMap.Set(id,light->GetName());
																														TagMap.Set(nodeid,light->GetName());																																
																													}
																													
																													break;
							case GRPFILE3DFBX_CONNECTION_TYPE_SKIN_LIMB:
																													{																														
																														if (nodeid==0)	
																														{
																															GRPSKINLIMB* limb=this->SkinLimbMap.Get(id);
																															if (object)
																															limb->LinkTo(this->object->GetNode());																															
																															object->Add(limb); //probably already attached, but scene checks it anyway
																														//	FBX_DEBUG(0,__L("Attaching Limb %u to Root"),id);
																														}
																														else
																														switch(IsType(nodeid))
																														{
																														case GRPFILE3DFBX_CONNECTION_TYPE_SKIN_ROOT:
																																			{
																																			GRPSKINLIMB* limb=this->SkinLimbMap.Get(id);
																																			GRPNODE* target=this->LocatorMap.Get(nodeid);
																																			limb->LinkTo(target);
																																	//		FBX_DEBUG(0,__L("Attaching Limb %u %s to Limb %u %s"),id,limb->GetName()->Get(),nodeid,target->GetName()->Get());
																																			break;
																																			}
																														case GRPFILE3DFBX_CONNECTION_TYPE_SKIN_LIMB:
																																			{
																																			GRPSKINLIMB* limb=this->SkinLimbMap.Get(id);
																																			GRPSKINLIMB* target=this->SkinLimbMap.Get(nodeid);
																																			limb->LinkTo(target);
																																	//		FBX_DEBUG(0,__L("Attaching Limb %u %s to Limb %u %s"),id,limb->GetName()->Get(),nodeid,target->GetName()->Get());
																																			break;
																																			}
																														case GRPFILE3DFBX_CONNECTION_TYPE_DEFORMER_SKIN:																																																													
																																			break;																																			
																														case GRPFILE3DFBX_CONNECTION_TYPE_SKIN_POSE:
																																			break;
																														case GRPFILE3DFBX_CONNECTION_TYPE_SUBDEFORMER_CLUSTER:
																																			{
																																			GRPSKINLIMB* limb=this->SkinLimbMap.Get(id);
																																			GRPSKINCLUSTER* target=this->SkinClusterMap.Get(nodeid);	
																																											target->AddLimb(limb);
																																			//								FBX_DEBUG(0,__L("Attaching Limb %u to Cluster %u"),id,nodeid);
																																			break;
																																			}
																														default:
																															{
																																  int t=0;
																																	break;
																															}
																																
																														}
																													}
																													break;
			case GRPFILE3DFBX_CONNECTION_TYPE_SUBDEFORMER_CLUSTER:
																													{																													
																														if (this->SkinClusterMap.Find(id)!=NOTFOUND)
																														{
																															GRPSKINCLUSTER* cluster	=this->SkinClusterMap.Get(id);	
																															if (this->SkinMap.Find(nodeid)!=NOTFOUND)
																															{
																															GRPSKIN* Skinning				=this->SkinMap.Get(nodeid);
																															Skinning->AddCluster(cluster);	
																															//	FBX_DEBUG(0,__L("FBX: Attaching Cluster %u to skin %u"),id,nodeid);
																															}
																														}																														
																													}
																													break;
		case GRPFILE3DFBX_CONNECTION_TYPE_ANIMATION_LAYER:
																											{			
																											if (!(loadFlags & GRPFILE3D_INCLUDE_ANIMATION))	break;
																											if (this->AnimationLayerMap.Find(id)==NOTFOUND)
																																						{
																																							if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_ERRORS)
																																							FBX_DEBUG(FBX_ERROR,__L("FBX: Error, Invalid Animation Layer %u"),id);
																																							return NULL;
																																						}	

																											if (this->AnimationStackMap.Find(nodeid)==NOTFOUND)
																																						{
																																							if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_ERRORS)
																																							FBX_DEBUG(FBX_ERROR,__L("FBX: Error, Invalid Animation Stack %u"),nodeid);
																																							return NULL;
																																						}																													
																											
																											this->AnimationStackMap.Get(nodeid)->AddLayer(this->AnimationLayerMap.Get(id));
																											this->AnimationLayerMap.Get(id)->SetMaxTime(this->AnimationStackMap.Get(nodeid)->GetMaxTime());
																											this->AnimationLayerMap.Get(id)->SetMinTime(this->AnimationStackMap.Get(nodeid)->GetMinTime());
																											}
																											break;
			case GRPFILE3DFBX_CONNECTION_TYPE_MATERIAL:		
																											{																																																																																		
																												if (this->MaterialMap.Find(id)==NOTFOUND)
																												{
																													if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_ERRORS)
																													FBX_DEBUG(FBX_ERROR,__L("FBX: Error, Unknown Material id %u"),nodeid);
																													return NULL;
																												}		
																												
																												if (ObjectNameMap.Find(nodeid)!=NOTFOUND)
																													{
																														FBXSTRING* nodename=ObjectNameMap.Get(nodeid);
																														GRPNODE* node=object->FindNode(nodename->Get());
																															if (node!=NULL)
																															{
																																//hay un nodo con ese nombre en la escena ya
																																		node->SetMaterial(this->MaterialMap.Get(id));				
																																	break;
																															}						
																													}

																												if (NodeMap.Find(nodeid)!=NOTFOUND)
																												{
																												GRPNODE* SceneNode=NodeMap.Get(nodeid);
																												SceneNode->SetMaterial(this->MaterialMap.Get(id));																												
																												}
																											}
																											break;
	case GRPFILE3DFBX_CONNECTION_TYPE_ANIMATION_NODE:
																											{																											
																											if (nodeid==0) //es una curva de root
																												break;

																											if (this->AnimationNodeMap.Find(id)==NOTFOUND)
																											{
																												if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_ERRORS)
																												FBX_DEBUG(FBX_ERROR,__L("FBX: Error, Invalid Animation Node %u"),nodeid);
																												return NULL;
																											}	

																											GRPANIMATIONNODE* anode=this->AnimationNodeMap.Get(id);
																											
																											if (nodeid==0)
																											{
																												if (object)
																														anode->SetTarget(object->GetNode());	 
																											}
																											else																											
																												switch(this->TypesMap.Get(nodeid))
																												{
																												case 0: //desconocido										
																															FBX_DEBUG(FBX_ERROR,__L("FBX: Error, Unknown Animation Target Type %u "),nodeid);
																															return NULL;																																						
																												case GRPFILE3DFBX_CONNECTION_TYPE_ANIMATION_LAYER:
																															this->AnimationLayerMap.Get(nodeid)->AddAnimationNode(anode);

																															break;	
																												default:
																															FBX_DEBUG(FBX_ERROR,__L("FBX: Warning, Unexpected Object-to-Object Animation Target Type %u "),nodeid);																															
																															continue;
																															break;
																												}
																											continue;
																											break;
																											}

						case GRPFILE3DFBX_CONNECTION_TYPE_DEFORMER_SKIN:
																											{				
																												if (!(loadFlags & GRPFILE3D_INCLUDE_DEFORMERS))	break;
																												if (this->SkinMap.Find(id)==NOTFOUND)																														
																												{
																													if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_ERRORS)
																														FBX_DEBUG(FBX_ERROR,__L("FBX: Error, Unknown Skin Deformer Connection %u"),id);
																														return NULL;
																												}

																												if (this->GeometricMap.Find(nodeid)==NOTFOUND)
																												{
																													if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_ERRORS)
																														FBX_DEBUG(FBX_ERROR,__L("FBX: Error, Unknown Skin Deformer Target Geometry Connection %u"),id);
																														return NULL;
																												}

																												GRPELEMENT* element=this->GeometricMap.Get(nodeid);
																																		element->SetSkin(this->SkinMap.Get(id));																																																																				
																											}
																											break;
							default: break;
					}
			}

			if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_STATUS)
			FBX_DEBUG(FBX_STATUS,__L("processing Dictionary : Properties"));


			//------------- PROPERTIES

			XDWORD PropertyConnectionsMapSize=PropertyConnectionsMap.GetSize();

			for (XDWORD e=0;e<PropertyConnectionsMapSize;e++)
			{								
					currentDictionary++;
					UpdateTask();
					XQWORD type=GRPFILE3DFBX_CONNECTION_TYPE_UNKNOWN;



					XQWORD		id			=PropertyConnectionsMap.GetKey(e);
					XQWORD		nodeid	=PropertyConnectionsMap.GetElement(e);
					FBXSTRING*subtype	=PropertyConnectionsTypeMap.GetElement(e);
										type		=IsType(id);

						if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_PROPERTIES)
							FBX_DEBUG(FBX_PROCESS,__L(" \tProperty %15u -> %15u"),id,nodeid);
					
					//--------------------------------------- Parse connection type
					switch(type)
					{												
					case GRPFILE3DFBX_CONNECTION_TYPE_LOCATOR:
																											{
																													if (subtype->GetSize()>0)
																															if (subtype->Compare("LookAtProperty")==0)
																															{
																																	GRPNODE* locator=LocatorMap.Get(id);

																																	if (locator)
																																	switch (IsType(nodeid))
																																	{
																																	case GRPFILE3DFBX_CONNECTION_TYPE_CAMERA:
																																			{
																																			GRPCAMERA* cam=		CameraMap.Get		(nodeid);
																																				if (cam)
																																				{
																																									cam->SetTarget	(locator);
																																									cam->SetType		(GRPCAMERA_TYPE_TARGET);
																																				}
																																				else
																																				{
																																					if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_ERRORS)
																																					FBX_DEBUG(FBX_ERROR,__L("The camera owner of the locator %lld does not exist!"),nodeid);
																																				}

																																			}
																																			break;
																																	case GRPFILE3DFBX_CONNECTION_TYPE_LIGHT:
																																			{
																																			GRPLIGHT* light=	LightMap.Get		(nodeid);
																																				if (light)
																																				{
																																												light->SetTarget(locator);
																																												light->SetType	(GRPLIGHTTYPE_SPOTLIGHT);
																																				}
																																				else
																																				{
																																					if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_ERRORS)
																																						FBX_DEBUG(FBX_ERROR,__L("The light owner of the locator %lld does not exist!"),nodeid);
																																				}
																																			}
																																			break;
																																	}
																															}
																											}
																											break;
					case GRPFILE3DFBX_CONNECTION_TYPE_MATERIAL:		
																											{																																																																																		
																												if (this->MaterialMap.Find(id)==NOTFOUND)
																												{
																													if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_ERRORS)
																													FBX_DEBUG(FBX_ERROR,__L("FBX: Error, Unknown Material id %u"),nodeid);
																													return NULL;
																												}		
																												
																												if (LocatorMap.Find(nodeid)!=NOTFOUND)
																												{		
																												GRPMATERIAL* material=this->MaterialMap.Get(id);

																												GRPNODE* SceneNode=LocatorMap.Get(nodeid);
																												SceneNode->SetMaterial(material);																												
																												}
																											}
																											break;

					case GRPFILE3DFBX_CONNECTION_TYPE_ANIMATION_CURVE:
																											{																												
																											GRPANIMATIONCURVE* curve=this->AnimationCurveMap.Get(id);

																											if (curve==NULL)
																											{
																												if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_ERRORS)
																													FBX_DEBUG(FBX_ERROR,__L("\t\t FBX: Error, Unknown Animation Curve %u"),id);
																													return NULL;
																											}

																											
																											GRPANIMATIONNODE* anode=this->AnimationNodeMap.Get(nodeid);																											
																											//if (this->AnimationNodeMap.Find(nodeid)==NOTFOUND)
																											if(anode==NULL)
																											{
																												if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_ERRORS)
																													FBX_DEBUG(FBX_WARNING,__L("\t\t FBX: Error, Unknown Animation Node %lld for %s -- Skiping"),nodeid,curve->GetName()->Get());																													

																													delete(curve);			
																													this->AnimationCurveMap.Delete(id);
																													break;
																											}																												
																											
																											GRPANIMATIONCURVEAXIS axis=GetPropertyTypeByName(subtype);

																											if (axis==GRPANIMATIONCURVEAXIS_UNKNOWN)
																											{
																												if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_WARNINGS)
																													FBX_DEBUG(FBX_WARNING,__L("\t\t FBX: Warning! Unsupported Curve Axis %lld Type: %s"),id,subtype->Get());	
																													delete(curve);			
																													this->AnimationCurveMap.Delete(id);
																												break;
																											}

																											curve->GetName()->Add(anode->GetName()->Get());
																											curve->GetName()->Add(__L(" "));
																											curve->GetName()->Add(subtype->Get());
																											
																											curve->ParentNode=anode;
																											if (curve->keytime.GetSize()>0)
																											{
																											curve->firsttime=curve->keytime.Get(0);
																											curve->lasttime	=curve->keytime.GetLast();
																											}
																											anode->AddConnection(axis,curve);//this->AnimationCurveMap.Get(id));
																											}
																											break;

					case GRPFILE3DFBX_CONNECTION_TYPE_ANIMATION_NODE:
																											{	
																											if (!(loadFlags & GRPFILE3D_INCLUDE_ANIMATION))	
																												break;
																																																					
																											GRPANIMATIONNODE* anode=this->AnimationNodeMap.Get(id);

																											if (anode==NULL)
																											{																																																				
																												if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_ERRORS)
																												FBX_DEBUG(FBX_ERROR,__L("\t\t FBX: Error, Invalid Animation Node %u"),nodeid);
																												return NULL;																												
																											}
																												anode->SetTargetTag(nodeid);

																														if (nodeid==0) //es una curva de root
																														{																												
																															break;																															
																														}

																														if (subtype->GetSize()>0)
																															{
																															if (subtype->Compare(__L("Visibility"),false)==0)																																																															
																																anode->SetType(GRPANIMATIONCURVETYPE_VISIBILITY);																															
																															else
																															if (subtype->Compare(__L("Lcl Translation"),false)==0)
																																anode->SetType(GRPANIMATIONCURVETYPE_TRANSLATION);
																															else
																															if (subtype->Compare(__L("Lcl Rotation"),false)==0)
																																anode->SetType(GRPANIMATIONCURVETYPE_ROTATION);
																															else
																															if (subtype->Compare(__L("Lcl Scaling"),false)==0)
																																anode->SetType(GRPANIMATIONCURVETYPE_SCALE);
																															else																															
																															if (subtype->Compare(__L("DiffuseColor"),false)==0)
																																anode->SetType(GRPANIMATIONCURVETYPE_DIFFUSECOLOR);
																															else
																															if (subtype->Compare(__L("Color"),false)==0)
																																anode->SetType(GRPANIMATIONCURVETYPE_DIFFUSECOLOR);
																															else																															
																															if (subtype->Compare(__L("DiffuseFactor"),false)==0)
																																anode->SetType(GRPANIMATIONCURVETYPE_DIFFUSEFACTOR);
																															else
																															if (subtype->Compare(__L("Intensity"),false)==0)
																																anode->SetType(GRPANIMATIONCURVETYPE_INTENSITY);																																
																															else
																															if (subtype->Compare(__L("EmissiveColor"),false)==0)
																																anode->SetType(GRPANIMATIONCURVETYPE_EMISSIVECOLOR);																																
																															else
																															if (subtype->Compare(__L("AmbientColor"),false)==0)
																																anode->SetType(GRPANIMATIONCURVETYPE_AMBIENTCOLOR);	
																															else
																															if (subtype->Compare(__L("TransparencyFactor"),false)==0)
																																anode->SetType(GRPANIMATIONCURVETYPE_TRANSPARENCYFACTOR);	
																															else
																															if (subtype->Compare(__L("ShininessExponent"),false)==0)
																																anode->SetType(GRPANIMATIONCURVETYPE_SHININESSEXPONENT);	
																															else
																															if (subtype->Compare(__L("SpecularColor"),false)==0)
																																anode->SetType(GRPANIMATIONCURVETYPE_SPECULARCOLOR);	
																															else
																															if (subtype->Compare(__L("SpecularFactor"),false)==0)
																																anode->SetType(GRPANIMATIONCURVETYPE_SPECULARFACTOR);	
																															else
																															if (subtype->Compare(__L("FieldOfView"),false)==0)
																																anode->SetType(GRPANIMATIONCURVETYPE_FOV);	
																															else
																															if (subtype->Compare(__L("FieldOfViewX"),false)==0)
																																anode->SetType(GRPANIMATIONCURVETYPE_FIELDOFVIEWX);	
																															else
																															if (subtype->Compare(__L("FieldOfViewY"),false)==0)
																																anode->SetType(GRPANIMATIONCURVETYPE_FIELDOFVIEWY);	
																															else
																															if (subtype->Compare(__L("DeformPercent"),false)==0)	
																																anode->SetType(GRPANIMATIONCURVETYPE_DEFORMPERCENT);
																															else
																															if (subtype->Compare(__L("UserValue"),false)==0)	
																																anode->SetType(GRPANIMATIONCURVETYPE_USERVALUE);
																															else
																															if (subtype->Compare(__L("value"),false)==0)	
																																anode->SetType(GRPANIMATIONCURVETYPE_USERVALUE);
																															else
																																{
																																	if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_WARNINGS)
																																	FBX_DEBUG(FBX_WARNING,__L("\t\t FBX: Warning!, Unknown Curve Type %u %s  - Skipping"),nodeid,subtype->Get());																															
																																	continue; //ignore unknown types
																																}
																															}
																										
																											if (this->TagMap.Find(nodeid)!=NOTFOUND)
																												anode->SetTargetName(this->TagMap.Get(nodeid)->Get());
																											else
																											{
																												if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_WARNINGS)
																												FBX_DEBUG(FBX_WARNING,__L("\t\t FBX: Error, Unnamed target node %lld"),nodeid);
																											}
																														
	
																											switch(this->TypesMap.Get(nodeid))
																											{
																											case 0: //desconocido										
																														if ((loadFlags & ~GRPFILE3D_INCLUDE_ANIMATION))	 //is there something more than anim
																														{
																														FBX_DEBUG(FBX_ERROR,__L("\t\t FBX: Error, Unknown Animation Node Target Type %u"),nodeid);
																														//return NULL;																																					
																														}
																														break;

																											case GRPFILE3DFBX_CONNECTION_TYPE_CAMERA:
																														if ((loadFlags & ~GRPFILE3D_INCLUDE_ANIMATION))	 //is there something more than anim																											
																														anode->SetTarget(this->CameraMap.Get(nodeid));	 
																														break;

																											case GRPFILE3DFBX_CONNECTION_TYPE_NODEATTRIBUTE_LIGHT:
																														if (NodeAttributeMap.Find(nodeid)!=NOTFOUND)
																														{
																															if (this->LightMap.Find(NodeAttributeMap.Get(nodeid))!=NOTFOUND)
																															{
																															XQWORD lightTag=NodeAttributeMap.Get(nodeid);
																															GRPLIGHT * light=this->LightMap.Get(lightTag);

																															if ((loadFlags & ~GRPFILE3D_INCLUDE_ANIMATION))	 //is there something more than anim																											
																																		anode->SetTarget(light);

																															anode->SetTargetTag(lightTag);
																															anode->SetTargetName(light->GetName()->Get());
																															TagMap.Delete(nodeid);
																															TagMap.Set(nodeid,light->GetName());
																															break;
																															}
																														}
																														break;
																											case GRPFILE3DFBX_CONNECTION_TYPE_LIGHT: 	
																														if ((loadFlags & ~GRPFILE3D_INCLUDE_ANIMATION))	 //is there something more than anim																											
																														anode->SetTarget(this->LightMap.Get(nodeid));	 
																														break;

																											case GRPFILE3DFBX_CONNECTION_TYPE_SKIN_LIMB: 
																														if ((loadFlags & ~GRPFILE3D_INCLUDE_ANIMATION))	 //is there something more than anim																											
																														anode->SetTarget(this->SkinLimbMap.Get(nodeid));
																														break;
																											case GRPFILE3DFBX_CONNECTION_TYPE_SKIN_ROOT:
																											case GRPFILE3DFBX_CONNECTION_TYPE_LOCATOR: 
																												if ((loadFlags & ~GRPFILE3D_INCLUDE_ANIMATION))	 //is there something more than anim																											
																														anode->SetTarget(this->LocatorMap.Get(nodeid));	 
																														break;
																											case GRPFILE3DFBX_CONNECTION_TYPE_MESH: 
																											case GRPFILE3DFBX_CONNECTION_TYPE_MODEL: 
																												if ((loadFlags & ~GRPFILE3D_INCLUDE_ANIMATION))	 //is there something more than anim																											
																														anode->SetTarget(this->NodeMap.Get(nodeid));	 
																														break;			

																											case GRPFILE3DFBX_CONNECTION_TYPE_MATERIAL:
																												if ((loadFlags & ~GRPFILE3D_INCLUDE_ANIMATION))	 //is there something more than anim																											
																														anode->SetTarget(this->MaterialMap.Get(nodeid));	 break;	

																											case GRPFILE3DFBX_CONNECTION_TYPE_SUBDEFORMER_MORPH:
																												if ((loadFlags & ~GRPFILE3D_INCLUDE_ANIMATION))	 //is there something more than anim																											
																														anode->SetTarget(this->SubdeformerMap.Get(nodeid));	 break;	

																											case GRPFILE3DFBX_CONNECTION_TYPE_NODEATTRIBUTE_CAMERA:
																												if ((loadFlags & ~GRPFILE3D_INCLUDE_ANIMATION))	 //is there something more than anim																											
																														if (NodeAttributeMap.Find(nodeid)!=NOTFOUND)
																														{
																															GRPCAMERA* cam=this->CameraMap.Get(NodeAttributeMap.Get(nodeid));
																															if (cam)
																																anode->SetTarget(cam);	
																														}
																														break;
																											default:
																												if ((loadFlags & ~GRPFILE3D_INCLUDE_ANIMATION))	 //is there something more than anim
																												{																											
																														FBX_DEBUG(FBX_WARNING,__L("\t\t FBX: Warning, Unsupported Animation Node Target Type %u"),nodeid);				
																														delete(anode);
																														this->AnimationNodeMap.Delete(id);
																														continue;
																												}
																														break;
																											}
																																																																						
																											}
																											break;

			
						case GRPFILE3DFBX_CONNECTION_TYPE_TEXTURE:
																											{
																												if (this->TextureMap.Find(id)!=NOTFOUND)
																												{										
																													
																													if (subtype->Compare(__L("Background Texture"),false)==0 && CameraMap.Find(nodeid)!=NOTFOUND)																														
																													{																											
																														if (this->TextureMap.Find(id)!=NOTFOUND)
																														{
																															if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_WARNINGS)
																															FBX_DEBUG(2,__L("\t\t FBX: Warning Image Plane is unsupported: %u %s - Skipping"), id, subtype->Get());																																																																													
																														}
																													}
																																																										
																													if (subtype->Compare(__L("DiffuseColor"),false)==0 && this->MaterialMap.Find(nodeid)!=NOTFOUND)
																													{
																														GRPMATERIAL* m=MaterialMap.Get(nodeid);
																														GRPTEXTURE* tex=this->TextureMap.Get(id);
																														tex->SetLayertype(GRPTEXTURELAYERTYPE_DIFFUSE);
																														m->AddLayer(GRPTEXTURELAYERTYPE_DIFFUSE,tex);																														
																													}
																													else
																													if (subtype->Compare(__L("TransparentColor"),false)==0 && this->MaterialMap.Find(nodeid)!=NOTFOUND)
																													{
																														GRPMATERIAL* m=MaterialMap.Get(nodeid);
																														GRPTEXTURE* tex=this->TextureMap.Get(id);
																														tex->SetLayertype(GRPTEXTURELAYERTYPE_OPACITY);
																														m->AddLayer(GRPTEXTURELAYERTYPE_OPACITY,tex);	
																														FBX_DEBUG(FBX_STATUS,__L("Adding layer as OPACITY %lld : %lld"),nodeid);
																													}
																													else
																													if (subtype->Compare(__L("SpecularFactor"),false)==0 && this->MaterialMap.Find(nodeid)!=NOTFOUND)
																													{
																														GRPMATERIAL* m=MaterialMap.Get(nodeid);
																														GRPTEXTURE* tex=this->TextureMap.Get(id);
																														tex->SetLayertype(GRPTEXTURELAYERTYPE_SPECULAR);
																														m->AddLayer(GRPTEXTURELAYERTYPE_SPECULAR,tex);																														
																													}
																													else
																													if (subtype->Compare(__L("Bump"),false)==0 && this->MaterialMap.Find(nodeid)!=NOTFOUND)
																													{
																														GRPMATERIAL* m=MaterialMap.Get(nodeid);
																														GRPTEXTURE* tex=this->TextureMap.Get(id);
																														tex->SetLayertype(GRPTEXTURELAYERTYPE_NORMALMAP);
																														m->AddLayer(GRPTEXTURELAYERTYPE_NORMALMAP,tex);
																													}
																													else
																													if (subtype->Compare(__L("NormalMap"),false)==0 && this->MaterialMap.Find(nodeid)!=NOTFOUND)
																													{
																														GRPMATERIAL* m=MaterialMap.Get(nodeid);
																														GRPTEXTURE* tex=this->TextureMap.Get(id);
																														tex->SetLayertype(GRPTEXTURELAYERTYPE_NORMALMAP);
																														m->AddLayer(GRPTEXTURELAYERTYPE_NORMALMAP,tex);																														
																													}
																													else
																													if (subtype->Compare(__L("EmissiveColor"),false)==0 && this->MaterialMap.Find(nodeid)!=NOTFOUND)
																													{
																														GRPMATERIAL* m=MaterialMap.Get(nodeid);
																														GRPTEXTURE* tex=this->TextureMap.Get(id);
																														tex->SetLayertype(GRPTEXTURELAYERTYPE_EMISIVE);
																														m->AddLayer(GRPTEXTURELAYERTYPE_EMISIVE,tex);																														
																													}
																													else
																													if (subtype->Compare(__L("ReflectionColor"),false)==0 && this->MaterialMap.Find(nodeid)!=NOTFOUND)
																													{
																														GRPMATERIAL* m=MaterialMap.Get(nodeid);
																														GRPTEXTURE* tex=this->TextureMap.Get(id);
																														tex->SetLayertype(GRPTEXTURELAYERTYPE_ENVMAP);
																														m->AddLayer(GRPTEXTURELAYERTYPE_ENVMAP,tex);																														
																													}
																												}
																											}
																											break;

						case GRPFILE3DFBX_CONNECTION_TYPE_SUBDEFORMER_MORPH:
																											{
																												if (!(loadFlags & GRPFILE3D_INCLUDE_DEFORMERS))	break;
																												if (this->DeformerMap.Find(nodeid)==NOTFOUND)
																													{
																														if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_ERRORS)
																														FBX_DEBUG(FBX_ERROR,__L("FBX: Error, Invalid Deformer Layer %u"),id);																														
																													}	

																												this->DeformerMap.Get(nodeid)->Add(this->SubdeformerMap.Get(id));
																											}
																											break;
						case GRPFILE3DFBX_CONNECTION_TYPE_DEFORMER_MORPH:
																											{
																												if (!(loadFlags & GRPFILE3D_INCLUDE_DEFORMERS))	break;
																												if (this->DeformerMap.Find(id)!=NOTFOUND)
																												{
																													if (this->GeometricMap.Find(nodeid)==NOTFOUND)
																													{																														
																														
																														if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_ERRORS)
																														FBX_DEBUG(FBX_ERROR,__L("\t\t FBX: Error! Unreferenced Deformer %u,%u"), id, nodeid);																														

																													}
																													else
																													{
																														this->GeometricMap.Get(nodeid)->SetDeformer(this->DeformerMap.Get(id));
																													}
																												}
																												break;
																											}


						case GRPFILE3DFBX_CONNECTION_TYPE_SUBDEFORMER_CLUSTER:
																											{		
																											if (!(loadFlags & GRPFILE3D_INCLUDE_DEFORMERS))	break;
																											if (this->SkinClusterMap.Find(id)==NOTFOUND)																														
																												{
																													if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_ERRORS)
																														FBX_DEBUG(FBX_ERROR,__L("\t\t FBX: Error, Unknown Skin Cluster Connection %u"),id);																														
																												}

																											if (this->SkinMap.Find(nodeid)==NOTFOUND)
																												{
																													if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_ERRORS)
																														FBX_DEBUG(FBX_ERROR,__L("\t\t FBX: Error, Unknown Skin Cluster Target Deformer Connection %u"),id);																																																											
																												}

																											//DeformerMap.Get(nodeid)->Add(this->SkinClusterMap.GetElement(id));
																											}
																											break;
						case GRPFILE3DFBX_CONNECTION_TYPE_NODEATTRIBUTE_LIMB:
																											{
																												if (!(loadFlags & GRPFILE3D_INCLUDE_DEFORMERS))	break;
																												if (this->SkinLimbMap.Find(nodeid)!=NOTFOUND)																													
																													if (this->SkinLimbAttributeMap.Find(id)!=NOTFOUND)																													
																														this->SkinLimbMap.Get(nodeid)->SetSize(this->SkinLimbAttributeMap.Get(id)->Size);																																																																																					
																											}
																											break;

					}
			}
			if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_STATUS)
			FBX_DEBUG(FBX_STATUS,__L("...Connections Finished!"));



			//-------------------------------------------------- Add the animation control tree to the scene			

			if (this->AnimationStackMap.GetSize()>0)
			{
				GRPANIMATIONSTACK* astack=this->AnimationStackMap.GetElement(0);
				if (this->object->GetAnimationControl()!=NULL)
				{
				this->object->GetAnimationControl()->AddStack(astack,this->NextStackID);				
				this->object->GetAnimationControl()->SetControlTimeFPS((GRPANIMATIONCONTROLTIMEFPS)this->TimeMode);
				this->object->GetAnimationControl()->SetControlTimeProtocolType((GRPANIMATIONCONTROLTIMEPROTOCOLTYPE)this->TimeProtocol);				
				}
			}

			XDWORD nnodes=object->GetNodes()->GetSize();
			for (XDWORD n=0;n<nnodes;n++)
			{
				GRPNODE* node=object->GetNodes()->FastGet(n);
				if (node!=NULL)
					node->Init();
			}

			for (int j=nnodes-1;j>=0;j--)	
					object->GetNodes()->FastGet(j)->modified=true;

		return object;
}


/*-------------------------------------------------------------------
//	GRPFILE3DFBX::IsType
*/	
/**	
//	Get wich type ID  is
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			20/02/2015 16:17:24
//	
//	@return 			GRPFILE3DFBX_CONNECTION_TYPE : 
//
//  @param				id : checking XDWORD id
//  @param				Objects : [OBJECTS] node for property check  
//  @param				isProperty : 
*/
/*-----------------------------------------------------------------*/
GRPFILE3DFBX_CONNECTION_TYPE		GRPFILE3DFBX::IsType					(XQWORD id)
{
	GRPFILE3DFBX_CONNECTION_TYPE type=GRPFILE3DFBX_CONNECTION_TYPE_UNKNOWN;	

	int d=TypesMap.Find(id);
	
	if (d==NOTFOUND)
				return GRPFILE3DFBX_CONNECTION_TYPE_UNKNOWN;

				type=(GRPFILE3DFBX_CONNECTION_TYPE)TypesMap.GetElement(d);

				return type;
}


/*-------------------------------------------------------------------
//	GRPFILE3DFBX::LoadModelNode
*/	
/**	
//	Loads a GRPNode Maya or Max
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			23/01/2015 13:33:30
//	
//	@return 			bool : TRUE if Success
//
//  @param				XDWORD : id of the Model node
//  @param				FBXNode*node : node for data
*/
/*-----------------------------------------------------------------*/
bool	GRPFILE3DFBX::LoadModelNode			(XQWORD id,FBXNode* modelnode)
{
	GRPNODE* element=NULL;

	XQWORD type=IsType(id);

	FBXSTRING * name=this->ObjectNameMap.Get(id);
		
	switch(type)
	{
		case GRPFILE3DFBX_CONNECTION_TYPE_MESH:						
		case GRPFILE3DFBX_CONNECTION_TYPE_MODEL:					element=this->NodeMap.Get(id);					break;
		case GRPFILE3DFBX_CONNECTION_TYPE_SKIN_ROOT:		  
		case GRPFILE3DFBX_CONNECTION_TYPE_LOCATOR:				element=this->LocatorMap.Get(id);				break;
		case GRPFILE3DFBX_CONNECTION_TYPE_CAMERA:					element=this->CameraMap.Get(id);				break;
		case GRPFILE3DFBX_CONNECTION_TYPE_LIGHT:					element=this->LightMap.Get(id);					break;
		case GRPFILE3DFBX_CONNECTION_TYPE_SKIN_LIMB:			element=this->SkinLimbMap.Get(id);			break;
		default:																																									return false;
	}
									if (element==NULL)
									{
									FBX_DEBUG(FBX_ERROR,__L("GRPFILE3DFBX::LoadModelNode : Not Enought Memory"));
									return false;
									}

	LoadModelNode(id,modelnode,element);
	return true;
}


/*-------------------------------------------------------------------
//	GRPFILE3DFBX::LoadMaxModelNode
*/	
/**	
//	Loads a GRPNode 3DSMax
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			23/01/2015 13:33:30
//	
//	@return 			bool : TRUE if Success
//
//  @param				XDWORD : id of the Model node
//  @param				FBXNode*node : node for data 
*/
/*-----------------------------------------------------------------*/
bool	GRPFILE3DFBX::LoadModelNode			(XQWORD id,FBXNode* modelnode,GRPNODE* element)
{

		//modelnode->data->label.DeleteCharacter(' ',XSTRINGCONTEXT_ATFIRST);
		element->SetName(modelnode->data->label.Get());		
		element->GetModelMatrix()->LoadIdentity();
		element->SetRotation(0.0f,0.0f,0.0f);;
		FBXNode* vector=NULL;

		modelnode->LastIndex=0;

						switch(this->Application)
						{
							case GRPFILE3DFBX_APPLICATION_TYPE_3DSMAX:	element->NodeMode=GRPNODE_MODE_MAX;		break;
							case GRPFILE3DFBX_APPLICATION_TYPE_MAYA:		element->NodeMode=GRPNODE_MODE_MAYA;	break;
							default:																		element->NodeMode=GRPNODE_MODE_OGL;		break;
						}

	//--------------------------------------- Inherit type
					if (FindNode(__L("InheritType"),11,	modelnode,&vector))	
					{
						element->InheritType=(int)(XDWORD)(*vector);
						switch(element->InheritType)
						{
						case GRPNODE_INHERIT_TYPE_RrSs:						
						case GRPNODE_INHERIT_TYPE_Rrs: 
																					FBX_DEBUG(FBX_WARNING,__L("FBX: Warning! Unsupported Hierarchy mode on node %d, the model could be wrongly rendered. "),id);
																					break;
								
						case GRPNODE_INHERIT_TYPE_RSrs:break;
						}
					}									
	//--------------------------------------- Local translation	
					if (FindNode(__L("GeometricTranslation"),20,	modelnode,&vector))		
					{
						GRPPOINT GeometricTranslation=*vector;
						GRPVECTOR v=GeometricTranslation;
						float x=0.0f;
						float y=0.0f;
						float z=0.0f;

						x=v.vector[RightAxis]*RightAxisSign;
						y=v.vector[UpAxis]*UpAxisSign;
						z=v.vector[FrontAxis]*FrontAxisSign;

						*element->GetGeometricPosition()=GRPVECTOR(x,y,z);
						element->UsesGeometricTranslation=true;
					}

//--------------------------------------- Local rotation

						if (FindNode(__L("GeometricRotation"),17,	modelnode,&vector))		
						{
							GRPPOINT GeometricRotation=*vector;
							GRPVECTOR v=GeometricRotation;
							float x=0.0f;
							float y=0.0f;
							float z=0.0f;

							x=v.vector[RightAxis]*RightAxisSign;
							y=v.vector[UpAxis]*UpAxisSign;
							z=v.vector[FrontAxis]*FrontAxisSign;

							*element->GetGeometricRotation()=GRPVECTOR(x,y,z);
							element->UsesGeometricRotation=true;
						}

//--------------------------------------- Local Scaling				

						if (FindNode(__L("GeometricScaling"),16,	modelnode,&vector))		
						{
						GRPPOINT p=*vector;						
						GRPVECTOR v=p;

						float x=0.0f;
						float y=0.0f;
						float z=0.0f;

						x=v.vector[RightAxis]*RightAxisSign;
						y=v.vector[UpAxis]*UpAxisSign;
						z=v.vector[FrontAxis]*FrontAxisSign;
												
						*element->GetGeometricScale()=GRPVECTOR(x,y,z);						
						element->UsesGeometricScale=true;
						}


						//--------------------------------------- Local Translation
					if (FindNode(__L("Lcl Translation"),15,				modelnode,&vector))		
					{
						GRPPOINT p=*vector;
											
						GRPVECTOR v=p;
						float x=0.0f;
						float y=0.0f;
						float z=0.0f;

						x=v.vector[RightAxis]*RightAxisSign;
						y=v.vector[UpAxis]*UpAxisSign;
						z=v.vector[FrontAxis]*FrontAxisSign;
												
						element->SetPosition(x,y,z);
					}

//--------------------------------------- Local Rotation
					if (FindNode(__L("Lcl Rotation"),12,					modelnode,&vector))		
					{
						GRPPOINT Rotation=*vector;
						GRPVECTOR v=Rotation;

						float x=0.0f;
						float y=0.0f;
						float z=0.0f;

						x=v.vector[RightAxis]*RightAxisSign;
						y=v.vector[UpAxis]*UpAxisSign;
						z=v.vector[FrontAxis]*FrontAxisSign;
					
						element->SetRotation(x,y,z);						
					}
						
//--------------------------------------- Local Scaling
					if (FindNode(__L("Lcl Scaling"),11,						modelnode,&vector))		
					{
						GRPPOINT p=*vector;

						
						GRPVECTOR v=p;
						float x=0.0f;
						float y=0.0f;
						float z=0.0f;

						x=v.vector[RightAxis]*RightAxisSign;
						y=v.vector[UpAxis]*UpAxisSign;
						z=v.vector[FrontAxis]*FrontAxisSign;
												
						element->SetScale(x,y,z);
					}
					/*
//----------------------------------------- rotation offsets		
					if (FindNode(__L("RotationOffset"),	modelnode,&vector))		
					{
						GRPPOINT RotationOffset=*vector;
						GRPVECTOR v=RotationOffset;
						float x=0.0f;
						float y=0.0f;
						float z=0.0f;

						x=v.vector[RightAxis]*RightAxisSign;
						y=v.vector[UpAxis]*UpAxisSign;
						z=v.vector[FrontAxis]*FrontAxisSign;

						element->RotationOffset=GRPVECTOR(x,y,z);
						element->UsesRotationOffset=true;
					}

//----------------------------------------- rotation offsets					
					if (FindNode(__L("RotationPivot"),	modelnode,&vector))		
					{
						GRPPOINT RotationPivot=*vector;
						GRPVECTOR v=RotationPivot;
						float x=0.0f;
						float y=0.0f;
						float z=0.0f;

						x=v.vector[RightAxis]*RightAxisSign;
						y=v.vector[UpAxis]*UpAxisSign;
						z=v.vector[FrontAxis]*FrontAxisSign;

						element->RotationPivot=GRPVECTOR(x,y,z);
						element->UsesRotationPivot=true;
					}

//----------------------------------------- rotation offsets					
					if (FindNode(__L("ScalingOffset"),	modelnode,&vector))		
					{
						GRPPOINT ScalingOffset=*vector;
						GRPVECTOR v=ScalingOffset;
						float x=0.0f;
						float y=0.0f;
						float z=0.0f;

						x=v.vector[RightAxis]*RightAxisSign;
						y=v.vector[UpAxis]*UpAxisSign;
						z=v.vector[FrontAxis]*FrontAxisSign;

						element->ScalingOffset=GRPVECTOR(x,y,z);
						element->UsesScalingOffset=true;
					}

//----------------------------------------- rotation offsets					
					if (FindNode(__L("ScalingPivot"),	modelnode,&vector))		
					{
						GRPPOINT ScalingPivot=*vector;
						GRPVECTOR v=ScalingPivot;
						float x=0.0f;
						float y=0.0f;
						float z=0.0f;

						x=v.vector[RightAxis]*RightAxisSign;
						y=v.vector[UpAxis]*UpAxisSign;
						z=v.vector[FrontAxis]*FrontAxisSign;

						element->ScalingPivot=GRPVECTOR(x,y,z);
						element->UsesScalingPivot=true;
					}*/

//----------------------------------------- prerotation
						if (FindNode(__L("PreRotation"),11,	modelnode,&vector))		
						{
							GRPPOINT PreRotation=*vector;						
							GRPVECTOR v=PreRotation;

							float x=0.0f;
							float y=0.0f;
							float z=0.0f;

							x=v.vector[0];
							y=v.vector[1];
							z=v.vector[2];

							x=v.vector[RightAxis]*RightAxisSign;
							y=v.vector[UpAxis]*UpAxisSign;
							z=v.vector[FrontAxis]*FrontAxisSign;

							element->prerotation=GRPVECTOR(x,y,z);		
							element->UsesPreRotation=true;

						}

//--------------------------------------- Local PostRotation
						if (FindNode(__L("PostRotation"),12,	modelnode,&vector))		
						{
							GRPPOINT PostRotation=*vector;						
							GRPVECTOR v=PostRotation;

							float x=0.0f;
							float y=0.0f;
							float z=0.0f;

							x=v.vector[0];
							y=v.vector[1];
							z=v.vector[2];

							x=v.vector[RightAxis]*RightAxisSign;
							y=v.vector[UpAxis]*UpAxisSign;
							z=v.vector[FrontAxis]*FrontAxisSign;	

							element->UsesPostRotation=true;
							element->postrotation=GRPVECTOR(x,y,z);			
						}
						
					if (FindNode(__L("Visibility"),	10,					modelnode,&vector))		
					{
						element->visibility=vector->data->value.ConvertToFloat();								
					}
					else				
					{
						
						element->visibility=1.0f;						
					}
						
	
					if (FindNode(__L("Visibility Inheritance"),	22,	modelnode,&vector))		 
					{
						element->visibility_inheritance=(bool)(vector->data->value.ConvertToInt()==1);
					}
					else
						element->visibility_inheritance=(bool)1;
															
					if (FindNode(__L("UserValue"),9,						modelnode,&vector))		
					{
							FBXSTRING* key=new FBXSTRING();
							*key=__L("UserValue");
	

							FBXSTRING* value=new FBXSTRING();
						if (vector->data->type.Compare(__L("KString"))==0)
						{
							value->Set(vector->data->value);
							element->Value=0;
						}
						else
						{
							XDWORD whereComma=vector->data->value.FindCharacter(__C(','));
							if (whereComma!=NOTFOUND)
							{
								vector->data->value.Copy(0,whereComma,*value);
								element->Value=vector->data->value.ConvertToFloat();		
							}
						}			
						if (!element->GetUserData()->Get()->Add(key,value))
							{
								FBX_DEBUG(4,__L("Unable to add data"));
							}					
					}
						
						
			
						if (FindNode(__L("UserMask"),8,						modelnode,&vector,true))		
						{
							FBXSTRING* key=new FBXSTRING();
							*key=__L("UserMask");

							FBXSTRING* value=new FBXSTRING();
							*value=vector->data->value;

							if (!element->GetUserData()->Get()->Add(key,value))
							{
								FBX_DEBUG(4,__L("Unable to add data"));
							}														
						}

						if (FindNode(__L("Font"),4,						modelnode,&vector,true))		
						{
							FBXSTRING* key=new FBXSTRING();
							*key=__L("Font");

							FBXSTRING* value=new FBXSTRING();
							*value=vector->data->value;

							if (!element->GetUserData()->Get()->Add(key,value))
							{
								FBX_DEBUG(4,__L("Unable to add data"));
							}											
						}

						if (FindNode(__L("Align"),5,						modelnode,&vector,true))		
						{
							FBXSTRING* key=new FBXSTRING();
							*key=__L("Align");

							FBXSTRING* value=new FBXSTRING();
							*value=vector->data->value;

							XDWORD whereComma=vector->data->value.FindCharacter(__C(','));
							if (whereComma!=NOTFOUND)							
								vector->data->value.Copy(0,whereComma,*value);							
							else
								value->Set(vector->data->value);

							if (!element->GetUserData()->Get()->Add(key,value))
							{
								FBX_DEBUG(4,__L("Unable to add data"));
							}
						}

			
					
					
					
					

			element->tag=(XDWORD)id;			

			if (object) 	object->Add(element);
	return true;
}

/*-------------------------------------------------------------------
//	GRPFILE3DFBX::LoadNodeAttribute
*/	
/**	
//	Loads a Camera/light/LimbNode/Locator for data
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			23/01/2015 13:33:30
//	
//	@return 			bool : TRUE for success
//
//  @param				XDWORD : id of the node attribute
//  @param				FBXNode*node : node with the data
*/
/*-----------------------------------------------------------------*/
bool	GRPFILE3DFBX::LoadNodeAttribute		(XQWORD id,FBXNode* node)
{
		FBXNode* attnode=NULL;
		FBXSTRING value;

	if (FindNode(__L("TypeFlags"),9, node,&attnode))
	{
		value=attnode->data->value;
		value.DeleteCharacter(__C('"'));
	}
	else
		value=node->data->subtype;

		if (value.Compare(__L("Camera"),6,false)==0)			//--------------------------------------------------------camera
			return LoadNodeAttributeCamera(id,node);
		
		if (value.Compare(__L("Light"),5,false)==0)				//-------------------------------------------------------light
			return LoadNodeAttributeLight(id,node);	
	
		if (value.Compare(__L("Skeleton"),8,false)==0)		//-----------------------------------------------------Skin limb node
			return LoadNodeAttributeLimb(id,node);				
		
	if (node->data->label.Compare(__L("Null"),4,false)==0)				//------------------------------------------------------dummy
			TypesMap.Add(id,GRPFILE3DFBX_CONNECTION_TYPE_NODEATTRIBUTE_NULL);			
											
	return true;
}

/*-------------------------------------------------------------------
//	GRPFILE3DFBX::LoadNodeAttributeCamera
*/	
/**	
//	Loads a camera attributes
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			11/05/2015 11:42:43
//	
//	@return 			bool : TRUE for success
//
//  @param				XDWORD : id of the node attribute
//  @param				FBXNode*node : node with the data: 
*/
/*-----------------------------------------------------------------*/
bool	GRPFILE3DFBX::LoadNodeAttributeCamera (XQWORD id,FBXNode* node)
{
	//Properties70:  {
//			P: "Color", "ColorRGB", "Color", "",0.341176470588235,0.470588235294118,0.8
//			P: "Position", "Vector", "", "A",0,-50,0
//			P: "BackgroundColor", "Color", "", "A",0,0,0
//			P: "AspectWidth", "double", "Number", "",640
//			P: "AspectHeight", "double", "Number", "",480
//			P: "FilmWidth", "double", "Number", "",1.41732287406921
//			P: "FilmHeight", "double", "Number", "",1.06299209594727
//			P: "FilmAspectRatio", "double", "Number", "",1.33333340809669
//			P: "ApertureMode", "enum", "", "",1
//			P: "FieldOfView", "FieldOfView", "", "A",45.0000012522391
//			P: "FocalLength", "Number", "", "A",43.4558439883016
//			P: "AutoComputeClipPanes", "bool", "", "",1
//		}
//		TypeFlags: "Camera"
//		GeometryVersion: 124
//		Position: 0,-50,0
//		Up: 0,1,0
//		LookAt: 0,0,0
//		ShowInfoOnMoving: 1
//		ShowAudio: 0
//		AudioColor: 0,1,0
//		CameraOrthoZoom: 1

			FBXNode* attnode=NULL;
			FBXSTRING s;

			node->LastIndex=0;

			GRPCAMERA*	camera=new GRPCAMERA();									
			//GRPCAMERA*	camera = object->CreateCamera();
									if (camera==NULL)
									{
									FBX_DEBUG(FBX_ERROR,__L("GRPFILE3DFBX::LoadNodeAttributeCamera : Not Enought Memory"));
									return false;
									}
									camera->tag=id;

									camera->SetName(__L("Camera Attribute"));
									CameraAttributeMap.Add(id,camera);
			
			

							if (FindNode(__L("Position"), 8, node,&attnode))
							{								
								
								GRPVECTOR position=*attnode;
								camera->EyePosition=position;

								camera->SetPosition(position.vector[RightAxis]*RightAxisSign,
																		position.vector[UpAxis]*UpAxisSign,
																		position.vector[FrontAxis]*FrontAxisSign);
								
							}

							if (FindNode(__L("AspectWidth"), 11, node,&attnode))
							{
								GLFLOAT aspectWidth=*attnode;
								camera->AspectWidth=aspectWidth;
							}	

							if (FindNode(__L("AspectHeight"), 12, node,&attnode))
							{
								GLFLOAT aspectHeight=*attnode;
								camera->AspectHeight=aspectHeight;
							}	

							if (FindNode(__L("FieldOfView"),11, node,&attnode))
							{
								GLFLOAT fov=*attnode;
								camera->fov=fov;
							}


							if (FindNode(__L("InterestPosition"),16, node,&attnode))
							{								
								GRPVECTOR interest=*attnode;
								GRPVECTOR	v;

								v.vector[0]=interest.vector[RightAxis]*RightAxisSign;
								v.vector[1]=interest.vector[UpAxis]*UpAxisSign;
								v.vector[2]=interest.vector[FrontAxis]*FrontAxisSign;

								camera->interestposition=v;
								camera->SetType(GRPCAMERA_TYPE_FREE);
							}
							else
							if (FindNode(__L("LookAt"),6, node,&attnode))
							{								
								GRPVECTOR interest=*attnode;
								GRPVECTOR	v;

								v.vector[0]=interest.vector[RightAxis]*RightAxisSign;
								v.vector[1]=interest.vector[UpAxis]*UpAxisSign;
								v.vector[2]=interest.vector[FrontAxis]*FrontAxisSign;

								camera->interestposition=v;
								camera->SetType(GRPCAMERA_TYPE_FREE);
							}
								

							if (FindNode(__L("FocalLength"),11, node,&attnode))
							{								
								GLFLOAT fl=*attnode;
								camera->FocalLength=fl;		
								camera->UseFocalLength=true;
							}
							
							if (FindNode(__L("Up"),2, node,&attnode))
							{						
								GRPVECTOR upvector=*attnode;	
								GRPVECTOR v;

								v.vector[0]=upvector.vector[RightAxis]*RightAxisSign;
								v.vector[1]=upvector.vector[UpAxis]*UpAxisSign;
								v.vector[2]=upvector.vector[FrontAxis]*FrontAxisSign;								
								
								camera->SetUpVector(&v);								
							}
							
						
							TypesMap.Add(id,GRPFILE3DFBX_CONNECTION_TYPE_NODEATTRIBUTE_CAMERA);

							return true;
}

/*-------------------------------------------------------------------
//	GRPFILE3DFBX::LoadNodeAttributeLight
*/	
/**	
//	Loads Light attributes
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			11/05/2015 11:45:01
//	
//	@return 			bool : TRUE for success
//
//  @param				XDWORD : id of the node attribute
//  @param				FBXNode*node : node with the data
*/
/*-----------------------------------------------------------------*/
bool	GRPFILE3DFBX::LoadNodeAttributeLight (XQWORD id,FBXNode* node)
{
			//P: "Color", "Color", "", "A",0.298039227724075,0.231372565031052,0.647058844566345
			//P: "DrawGroundProjection", "bool", "", "",0
			//P: "Fog", "Number", "", "A",0
			//P: "DecayStart", "Number", "", "A",40
			//P: "NearAttenuationEnd", "Number", "", "A",40
			//P: "FarAttenuationStart", "Number", "", "A",80
			//P: "FarAttenuationEnd", "Number", "", "A",200
			//P: "3dsMax", "Compound", "", ""
			//P: "3dsMax|ClassIDa", "int", "Integer", "",4113
			//P: "3dsMax|ClassIDb", "int", "Integer", "",0
			//P: "3dsMax|SuperClassID", "int", "Integer", "",48
			//P: "3dsMax|ParamBlock_0", "Compound", "", ""
			//P: "3dsMax|ParamBlock_0|Color", "Color", "", "A",0.298039227724075,0.231372565031052,0.647058844566345
			//P: "3dsMax|ParamBlock_0|Multiplier", "Float", "", "A",1
			//P: "3dsMax|ParamBlock_0|Contrast", "Float", "", "A",0
			//P: "3dsMax|ParamBlock_0|Diffuse Soften", "Float", "", "A",0
			//P: "3dsMax|ParamBlock_0|Attenuation Near Start", "Float", "", "A",0
			//P: "3dsMax|ParamBlock_0|Attenuation Near End", "Float", "", "A",40
			//P: "3dsMax|ParamBlock_0|Attenuation Far Start", "Float", "", "A",80
			//P: "3dsMax|ParamBlock_0|Attenuation Far End", "Float", "", "A",200
			//P: "3dsMax|ParamBlock_0|Decay Falloff", "Float", "", "A",40
			//P: "3dsMax|ParamBlock_0|Shadow Color", "Color", "", "A",0,0,0
			//P: "3dsMax|ParamBlock_0|_Unnamed_Parameter_10", "int", "Integer", "",0
			//P: "3dsMax|ParamBlock_0|Atmosphere Opacity", "Float", "", "A",1
			//P: "3dsMax|ParamBlock_0|Atmosphere Color Amount", "Float", "", "A",1
			//P: "3dsMax|ParamBlock_0|Shadow Density", "Float", "", "A",1
			//P: "3dsMax|ParamBlock_0|_Unnamed_Parameter_14", "int", "Integer", "",0

			FBXNode* attnode=NULL;

			node->LastIndex=0;

								GRPLIGHT* light=new GRPLIGHT();
								if (light==NULL)
								{
								FBX_DEBUG(FBX_ERROR,__L("GRPFILE3DFBX::LoadNodeAttributeLimb : Not Enought Memory"));
								return false;
								}
									light->SetName(__L("Light Attribute"));
									this->LightAttributeMap.Add(id,light);		

									FBXNode* properties=NULL;
									if (FindNode(__L("Properties70"),12, node,&properties))
									{
											if (FindNode(__L("Color"),5, properties,&attnode))											
													light->SetDiffuse((GRPCOLOR)*attnode);
													
											if (FindNode(__L("DecayType"),9, properties,&attnode))
											{
												XDWORD DecayType=*attnode;
												switch (DecayType)
												{
												case 0: DecayType=GRPLIGHTDECAYTYPE_NONE;				break;
												case 1: DecayType=GRPLIGHTDECAYTYPE_LINEAR;			break;
												case 2: DecayType=GRPLIGHTDECAYTYPE_QUADRATIC;	break;
												case 3: DecayType=GRPLIGHTDECAYTYPE_CUBIC;			break;
												}
												light->SetDecayType((GRPLIGHTDECAYTYPE)DecayType);
											}			
											
											//GLFLOAT scale=1.0f/this->UnitScaleFactor;
											GLFLOAT scale = UnitScaleFactor;

											if (this->Application==GRPFILE3DFBX_APPLICATION_TYPE_3DSMAX)	
											{
												if (light->GetDecayType()!=GRPLIGHTDECAYTYPE_NONE)
															light->SetAttenuation(1.00f,0.30f,0.10f,0.10f);
												else
															light->SetAttenuation(0.00f,0.00f,0.00f,0.00f);
											}
											else
												if (this->Application==GRPFILE3DFBX_APPLICATION_TYPE_MAYA)											
															light->SetAttenuation(0.00f,0.00f,0.00f,0.00f);
																				
											if (FindNode(__L("Intensity"), 9, properties,&attnode))
											{												
												GLFLOAT strength=*attnode;
												light->SetStrength((GLFLOAT)strength);			
												light->SetMultiplier((GLFLOAT)1.0f);	
											}	
											else
											if (FindNode(__L("3dsMax|ParamBlock_0|Multiplier"),30, properties,&attnode))
											{												
												GLFLOAT mult=*attnode;
												light->SetStrength((GLFLOAT)100.0f);
												light->SetMultiplier((GLFLOAT)mult);	
											}											
											else
											{
												light->SetStrength((GLFLOAT)100.0f);
												light->SetMultiplier((GLFLOAT)1.0f);	
											}
																						
											light->SetScaleFactor(scale);
											
											if (FindNode(__L("LightType"),9, properties,&attnode))
												switch((XDWORD)*attnode)
												{
														//----- Point light
													case 0: light->SetType(GRPLIGHTTYPE_POINT);				break;

														//----- directional
													case 1: {
																			light->SetType(GRPLIGHTTYPE_DIRECTIONAL);
																		
																			GRPNODE*		node = object->CreateNode();

																									switch (this->UpAxis)
																									{
																									case 0:	node->SetPosition(1.0f, 0.0f, 0.0f); break;
																									case 1:	node->SetPosition(0.0f, 1.0f, 0.0f); break;
																									case 2:	node->SetPosition(0.0f, 0.0f, 1.0f); break;
																									}
																									node->SetParent(light);
																									light->SetUpNode(node);
																									
																									switch (this->UpAxis)
																									{
																									case 0:	light->SetUpVector(1.0f, 0.0f, 0.0f); break;
																									case 1:	light->SetUpVector(0.0f, 1.0f, 0.0f); break;
																									case 2:	light->SetUpVector(0.0f, 0.0f, 1.0f); break;
																									}
																			
														
																			GRPNODE*			target = object->CreateNode();
																										target->SetPosition(0.0, 0.0, -100.0f);
																										target->SetParent(light);
																										light->SetTarget(target);
													
																			if (FindNode(__L("3dsMax|ParamBlock_0|Target Distance"), 35, properties, &attnode))
																			{
																					target->SetPosition(0.0, 0.0, -attnode->data->value.ConvertToFloat());
																			}
																		
																	}
																	break;
																	
														//----- spotlight
													case 2: light->SetType(GRPLIGHTTYPE_SPOTLIGHT);	
														
																	if (FindNode(__L("InnerAngle"), 10, properties,&attnode))
																		light->SetInnerSpot((GLFLOAT)*attnode);

																	if (FindNode(__L("OuterAngle"),10, properties,&attnode))
																		light->SetOuterSpot((GLFLOAT)*attnode);;																 

																		//light->bias/=UnitScaleFactor;

																		GRPNODE*	node=object->CreateNode();
																							node->SetPosition(0.0,1.0,0.0);																							
																							node->SetParent(light);					
																							light->SetUpNode(node);
/*
																							switch(this->UpAxis)
																							{
																								case 0:	node->SetPosition(1.0f,0.0f,0.0f); break;
																								case 1:	node->SetPosition(0.0f,1.0f,0.0f); break;
																								case 2:	node->SetPosition(0.0f,0.0f,1.0f); break;
																							}
																							*/
						
																	break;
												}											
									}
				
					if (!TypesMap.Add(id,GRPFILE3DFBX_CONNECTION_TYPE_NODEATTRIBUTE_LIGHT))
					{
						FBX_DEBUG(FBX_ERROR,__L("Unable to Add Light to map %u"),id);
					}

			return true;

}

/*-------------------------------------------------------------------
//	GRPFILE3DFBX::LoadNodeAttributeLimb
*/	
/**	
//	Loads limb attributes
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			21/05/2015 14:56:19
//	
//	@return 			bool : TRUE for success
//
//  @param				XDWORD : id of the node attribute
//  @param				FBXNode*node : node with the data
*/
/*-----------------------------------------------------------------*/
bool	GRPFILE3DFBX::LoadNodeAttributeLimb	(XQWORD id,FBXNode* node)
{
		FBXNode* properties=NULL;
		FBXNode* limbnode=NULL;

		GRPSKINLIMB* limb=new GRPSKINLIMB();
		if (limb==NULL)
		{
		FBX_DEBUG(FBX_ERROR,__L("GRPFILE3DFBX::LoadNodeAttributeLimb : Not Enought Memory"));
		return false;
		}
		node->LastIndex=0;

									if (FindNode(__L("Properties70"),12, node,&properties))
									{
											if (FindNode(__L("Size"),4, properties,&limbnode))
											{
												GLFLOAT size=*limbnode;
												limb->SetSize(size);
											}		
									}
									
		this->SkinLimbAttributeMap.Add(id,limb);
		TypesMap.Add(id,GRPFILE3DFBX_CONNECTION_TYPE_NODEATTRIBUTE_LIMB);

			return true;

}

/*-------------------------------------------------------------------
//	GRPFILE3DFBX::LoadGeometryNode
*/	
/**	
//	Loads Geometry data
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			23/01/2015 13:58:24
//	
//	@return 			bool : TRUE for success
//
//  @param				XDWORD : id of the node attribute
//  @param				FBXNode*node : node with the data
*/
/*-----------------------------------------------------------------*/
bool	GRPFILE3DFBX::LoadGeometryNode	(XQWORD id,FBXNode*currentObject)
{	

	bool hasPosition	=false,
			 hasNormals		=false,
			 hasBinormals	=false,
			 hasTangents	=false,
			 hasTextures	=false;			 

	 GRPFILE3DFBX_INDEXING_TYPE		NormalsIndexingType			=GRPFILE3DFBX_INDEXING_TYPE_UNKNOWN;
	 GRPFILE3DFBX_REFERENCE_TYPE	NormalsReferenceType		=GRPFILE3DFBX_REFERENCE_TYPE_UNKNOWN;
	 
	 GRPFILE3DFBX_INDEXING_TYPE		BinormalsIndexingType		=GRPFILE3DFBX_INDEXING_TYPE_UNKNOWN;
	 GRPFILE3DFBX_REFERENCE_TYPE	BinormalsReferenceType	=GRPFILE3DFBX_REFERENCE_TYPE_UNKNOWN;

	 GRPFILE3DFBX_INDEXING_TYPE		TangentsIndexingType		=GRPFILE3DFBX_INDEXING_TYPE_UNKNOWN;
	 GRPFILE3DFBX_REFERENCE_TYPE	TangentsReferenceType		=GRPFILE3DFBX_REFERENCE_TYPE_UNKNOWN;

	 GRPFILE3DFBX_INDEXING_TYPE		UvIndexingType					=GRPFILE3DFBX_INDEXING_TYPE_UNKNOWN;
	 GRPFILE3DFBX_REFERENCE_TYPE	UvReferenceType					=GRPFILE3DFBX_REFERENCE_TYPE_UNKNOWN;
	
		FBXNode* node=NULL;

			
		
		XQWORD modelNodeId=0;

		if (this->ObjectConnectionsMap.Find(id)!=NOTFOUND)
		{
			modelNodeId=this->ObjectConnectionsMap.Get(id);
			if (this->ObjectNameMap.Find(modelNodeId)!=NOTFOUND)
			{
				FBXSTRING* objectName=this->ObjectNameMap.Get(modelNodeId);
				if (object->FindElement(objectName->Get())!=NULL)
				{
					//ya esta en la scene
						if (!(loadFlags & GRPFILE3D_INCLUDE_NEW_GEOMETRY))						
								FBX_DEBUG(FBX_ERROR,__L("Duplicated Object Found:%lld %s"),modelNodeId,objectName->Get());											
						return true;
				}
			}		
		}		


			GRPELEMENT* element=new GRPELEMENT();		
									
			FBXSTRING* objectName=this->ObjectNameMap.Get(modelNodeId);
									if (objectName)
										element->SetName(objectName->Get());
									else
										element->SetName(currentObject->data->name.Get());


			GRPMESH* mesh=new GRPMESH();
			element->SetMesh(mesh);
			MeshMap.Add(mesh);
			this->GeometricMap.Add(id,element);

			if (this->GeometricMap.Find(id)==NOTFOUND)
			return false;


		if (element==NULL)
		{
		FBX_DEBUG(FBX_ERROR,__L("GRPFILE3DFBX::LoadGeometryNode : Not Enought Memory"));
		return false;
		}

				mesh->SetName(currentObject->data->label.Get());

				FBXSTRING* name=&currentObject->data->name;		
				FBXSTRING* s	 =&currentObject->data->value;

				XVECTOR<GRPPOINT*>	vertices;
				XVECTOR<GRPPOINT*>	normals;			
				XVECTOR<GRPPOINT*>	tcoords;

		//------------------------------------------ BaseColor
		if (FindNode(__L("Color"),5,currentObject,&node))
		{
			element->SetDefaultColor((GRPCOLOR)*node);
		}

		//------------------------------------------- load vertices
		if (FindNode(__L("Vertices"),8,currentObject,&node))
		{
			FBXVECTOR<double>* vertexarray=node->data->ArrayValues;
			if (vertexarray==NULL) 
				return false;

			vertices.SetAddInLimit(vertexarray->GetSize()/3);
			for (XDWORD e=0,i=0;e<vertexarray->GetSize();e+=3,i++)
			{
				GRPPOINT* p=NULL;

				double x=0.0f;
				double y=0.0f;
				double z=0.0f;

				x=vertexarray->FastGet(e+RightAxis)*RightAxisSign;
				y=vertexarray->FastGet(e+UpAxis)*UpAxisSign;
				z=vertexarray->FastGet(e+FrontAxis)*FrontAxisSign;

				p=new GRPPOINT((float)x,(float)y,(float)z,i);
				
				if (p) 
					vertices.Add(p);				
			}			
			 hasPosition=true;
		}
		else
			{
				FBX_DEBUG(FBX_ERROR,__L("FBX: Error : The model has no vertexes!"));
				delete(object);
				object=NULL;
				return false;
			}

		//---------------------------------------- NORMAL LAYER
		if (FindNode(__L("LayerElementNormal"),18,currentObject,&node))
		{
			FBXNode* IndexingTypeNode;
			
				
			if (FindNode(__L("MappingInformationType"),22,node,&IndexingTypeNode))
			{
				IndexingTypeNode->data->value.DeleteCharacter(__C('\"'));
				if (IndexingTypeNode->data->value.Compare(__L("ByVertice"),9,false)==0)
						NormalsIndexingType=GRPFILE3DFBX_INDEXING_TYPE_BYVERTEX;
				else
				if (IndexingTypeNode->data->value.Compare(__L("ByPolygonVertex"),15,false)==0)
						NormalsIndexingType=GRPFILE3DFBX_INDEXING_TYPE_BYPOLYGON;
				else
				if (IndexingTypeNode->data->value.Compare(__L("AllSame"),7,false)==0)
						NormalsIndexingType=GRPFILE3DFBX_INDEXING_TYPE_ALLSAME;
			}

			if (FindNode(__L("ReferenceInformationType"),24,node,&IndexingTypeNode))			
			{
				IndexingTypeNode->data->value.DeleteCharacter(__C('\"'));
				if (IndexingTypeNode->data->value.Compare(__L("Direct"),6,false)==0)
						NormalsReferenceType=GRPFILE3DFBX_REFERENCE_TYPE_DIRECT;
				else
				if (IndexingTypeNode->data->value.Compare(__L("IndexToDirect"),13,false)==0)
						NormalsReferenceType=GRPFILE3DFBX_REFERENCE_TYPE_INDEXTODIRECT;
			}
			FBXNode* NormalsNode=0;

						//------------------------------------------- load normals
			if (FindNode(__L("Normals"),7,node,&NormalsNode))
			{
				FBXVECTOR<double>* normalarray=NormalsNode->data->ArrayValues;
				if (normalarray==NULL) 
					return false;
				normals.SetAddInLimit(normalarray->GetSize()/3);

				for (XDWORD e=0,i=0;e<normalarray->GetSize();e+=3,i++)
				{
					GRPPOINT* p=NULL;

					double		x=0.0f;
					double		y=0.0f;
					double		z=0.0f;

					x=normalarray->FastGet(e+RightAxis)*RightAxisSign;
					y=normalarray->FastGet(e+UpAxis		)*UpAxisSign;
					z=normalarray->FastGet(e+FrontAxis)*FrontAxisSign;

					p=new GRPPOINT((float)x,(float)y,(float)z,i);
				
					if (p) 
						normals.Add(p);						
				}
				hasNormals=true;
			}
		}
#ifdef GRPVERTEX_EXTENDED
			XVECTOR<GRPPOINT*>	binormals;
			XVECTOR<GRPPOINT*>	tangents;
		//---------------------------------------- BINORMAL LAYER
		if (FindNode(__L("LayerElementBinormal"),20,currentObject,&node))
		{
			FBXNode* IndexingTypeNode;
			
				
			if (FindNode(__L("MappingInformationType"),22,node,&IndexingTypeNode))
			{
				IndexingTypeNode->data->value.DeleteCharacter(__C('\"'));
				if (IndexingTypeNode->data->value.Compare(__L("ByVertice"),false)==0)
						BinormalsIndexingType=GRPFILE3DFBX_INDEXING_TYPE_BYVERTEX;
				else
				if (IndexingTypeNode->data->value.Compare(__L("ByPolygonVertex"),false)==0)
						BinormalsIndexingType=GRPFILE3DFBX_INDEXING_TYPE_BYPOLYGON;
				else
				if (IndexingTypeNode->data->value.Compare(__L("AllSame"),false)==0)
						BinormalsIndexingType=GRPFILE3DFBX_INDEXING_TYPE_ALLSAME;
			}

			if (FindNode(__L("ReferenceInformationType"),24,node,&IndexingTypeNode))			
			{
				IndexingTypeNode->data->value.DeleteCharacter(__C('\"'));
				if (IndexingTypeNode->data->value.Compare(__L("Direct"),false)==0)
						BinormalsReferenceType=GRPFILE3DFBX_REFERENCE_TYPE_DIRECT;
				else
				if (IndexingTypeNode->data->value.Compare(__L("IndexToDirect"),false)==0)
						BinormalsReferenceType=GRPFILE3DFBX_REFERENCE_TYPE_INDEXTODIRECT;
			}
			FBXNode* BinormalsNode=0;


						//------------------------------------------- load normals
			if (FindNode(__L("Binormals"),9,node,&BinormalsNode))
			{
				FBXVECTOR<double>* binormalarray=BinormalsNode->data->ArrayValues;
				if (binormalarray==NULL) 
					return false;
				binormals.SetAddInLimit(binormalarray->GetSize()/3);

				for (XDWORD e=0,i=0;e<binormalarray->GetSize();e+=3,i++)
				{
					GRPPOINT* p=NULL;

					double		x=0.0f;
					double		y=0.0f;
					double		z=0.0f;

					x=binormalarray->FastGet(e+RightAxis)*RightAxisSign;
					y=binormalarray->FastGet(e+UpAxis		)*UpAxisSign;
					z=binormalarray->FastGet(e+FrontAxis)*FrontAxisSign;

					p=new GRPPOINT((float)x,(float)y,(float)z,i);
				
					if (p) 
						binormals.Add(p);						
				}
				hasBinormals=true;
			}
		}

//---------------------------------------- TANGENT LAYER
		if (FindNode(__L("LayerElementTangent"),19,currentObject,&node))
		{
			FBXNode* IndexingTypeNode;
			
				
			if (FindNode(__L("MappingInformationType"),22,node,&IndexingTypeNode))
			{
				IndexingTypeNode->data->value.DeleteCharacter(__C('\"'));
				if (IndexingTypeNode->data->value.Compare(__L("ByVertice"),false)==0)
						TangentsIndexingType=GRPFILE3DFBX_INDEXING_TYPE_BYVERTEX;
				else
				if (IndexingTypeNode->data->value.Compare(__L("ByPolygonVertex"),false)==0)
						TangentsIndexingType=GRPFILE3DFBX_INDEXING_TYPE_BYPOLYGON;
				else
				if (IndexingTypeNode->data->value.Compare(__L("AllSame"),false)==0)
						TangentsIndexingType=GRPFILE3DFBX_INDEXING_TYPE_ALLSAME;
			}

			if (FindNode(__L("ReferenceInformationType"),24,node,&IndexingTypeNode))			
			{
				IndexingTypeNode->data->value.DeleteCharacter(__C('\"'));
				if (IndexingTypeNode->data->value.Compare(__L("Direct"),false)==0)
						TangentsReferenceType=GRPFILE3DFBX_REFERENCE_TYPE_DIRECT;
				else
				if (IndexingTypeNode->data->value.Compare(__L("IndexToDirect"),false)==0)
						TangentsReferenceType=GRPFILE3DFBX_REFERENCE_TYPE_INDEXTODIRECT;
			}
			FBXNode* TangentsNode=0;

						//------------------------------------------- load normals
			if (FindNode(__L("Tangents"),8,node,&TangentsNode))
			{
				FBXVECTOR<double>* tangentarray=TangentsNode->data->ArrayValues;
				if (tangentarray==NULL) 
					return false;
				tangents.SetAddInLimit(tangentarray->GetSize()/3);

				for (XDWORD e=0,i=0;e<tangentarray->GetSize();e+=3,i++)
				{
					GRPPOINT* p=NULL;

					double		x=0.0f;
					double		y=0.0f;
					double		z=0.0f;

					x=tangentarray->FastGet(e+RightAxis)*RightAxisSign;
					y=tangentarray->FastGet(e+UpAxis		)*UpAxisSign;
					z=tangentarray->FastGet(e+FrontAxis)*FrontAxisSign;

					p=new GRPPOINT((float)x,(float)y,(float)z,i);
				
					if (p) 
						tangents.Add(p);						
				}
				hasTangents=true;
			}
		}

#endif

		//---------------------------------------- UV LAYER
		if (FindNode(__L("LayerElementUV"),14,currentObject,&node))
		{
					FBXNode* IndexingTypeNode;
		
					if (FindNode(__L("MappingInformationType"),22,node,&IndexingTypeNode))
					{
						IndexingTypeNode->data->value.DeleteCharacter(__C('\"'));
						if (IndexingTypeNode->data->value.Compare(__L("ByVertice"),false)==0)
								UvIndexingType=GRPFILE3DFBX_INDEXING_TYPE_BYVERTEX;
						else
						if (IndexingTypeNode->data->value.Compare(__L("ByPolygonVertex"),false)==0)
								UvIndexingType=GRPFILE3DFBX_INDEXING_TYPE_BYPOLYGON;
						else
						if (IndexingTypeNode->data->value.Compare(__L("AllSame"),false)==0)
								UvIndexingType=GRPFILE3DFBX_INDEXING_TYPE_ALLSAME;
					}

					if (FindNode(__L("ReferenceInformationType"),24,node,&IndexingTypeNode))			
					{
						IndexingTypeNode->data->value.DeleteCharacter(__C('\"'));
						if (IndexingTypeNode->data->value.Compare(__L("Direct"),false)==0)
								UvReferenceType=GRPFILE3DFBX_REFERENCE_TYPE_DIRECT;
						else
						if (IndexingTypeNode->data->value.Compare(__L("IndexToDirect"),false)==0)
								UvReferenceType=GRPFILE3DFBX_REFERENCE_TYPE_INDEXTODIRECT;
					}

				//------------------------------------------- load texture coordinates
				if (FindNode(__L("UV"),2,currentObject,&node))
				{
					FBXVECTOR<double>* uvarray=node->data->ArrayValues;
					if (uvarray==NULL) 
						return false;
					tcoords.SetAddInLimit(uvarray->GetSize()/2);
					for (XDWORD e=0,i=0;e<uvarray->GetSize();e+=2,i++)
					{
						GRPPOINT* p=NULL;				
						p=new GRPPOINT((float)uvarray->FastGet(e),(float)uvarray->FastGet(e+1),0,i);
				
						if (p) 
							tcoords.Add(p);
					}				
					hasTextures=true;
				}
		}

		//------------------------------------------- EDGES
		/*			
			regarding Edges, These are not vertex indices's, They are direct references to the PolygonVertexIndex array. 
			Each edge is defined by that vertex and the next one in that face (if the index is < 0, it wraps around to the 
			first index in that polygon). (I found this while writing an importer, if you load these values as edge-index-pairs 
			you end up with scrambled edges all over the mesh) –  ideasman42 Sep 2 '13 at 6:15 
		*/
		node->LastIndex=2;
		//------------------------------------------ Create faces

		if (FindNode(__L("PolygonVertexIndex"),18,currentObject,&node))
		{
			FBXNode* uvindexnode=NULL;

			if (UvIndexingType==GRPFILE3DFBX_INDEXING_TYPE_BYPOLYGON && !FindNode(__L("UVIndex"),7,currentObject,&uvindexnode))  //we get the dictionary in case is ordered by indexes
			{
				FBX_DEBUG(FBX_ERROR,__L("FBX: Error : The model has no texture coordinate indexes!"));												
				return false;
			}			

			FBXVECTOR<long long>* polygonarray=node->data->ArrayIndex;
			FBXVECTOR<long long>* uvindexarray=NULL;

			if (UvIndexingType==GRPFILE3DFBX_INDEXING_TYPE_BYPOLYGON)
				uvindexarray=uvindexnode->data->ArrayIndex;

			GRPMESH* mesh=new GRPMESH();
			element->SetMesh(mesh);
			element->GetMesh()->GetFaces()->SetAddInLimit((polygonarray->GetSize()/3)+1); //prevent new being called per vertex



			XDWORD n=0;

			bool UnmappedWarning=false;
			bool NotTriangles=false;
				
			//---------------- check if triangles

	
				if (polygonarray->GetSize()<2 )
				{
				FBX_DEBUG(FBX_ERROR,__L("FBX: Error : The model has no polygons!"));												
				return false;
				}	
				int a=(int)polygonarray->FastGet(0);
				int b=(int)polygonarray->FastGet(1);
				int c=(int)polygonarray->FastGet(2);
					 		
				if (c>0)
				{
				FBX_DEBUG(FBX_ERROR,__L("FBX: Error : The model has non-triangle faces!"));												
				return false;
				}		

				XDWORD pSize=polygonarray->GetSize();
			for (XDWORD e=0;e<pSize;e+=3)
			{				

				int a=(int)polygonarray->FastGet(e+0);
				int b=(int)polygonarray->FastGet(e+1);
				int c=(int)polygonarray->FastGet(e+2);
					 									
							c=~c; // c=(c*-1)-1;  //the last index is XORed to distinguish between quads, triangles and polygons , we consider only triangles (FBX plugin: triangulize)

				int av=0;
				int bv=0;
				int cv=0;

				XDWORD auv=0,buv=0,cuv=0;


				switch (UvIndexingType)
				{
				case GRPFILE3DFBX_INDEXING_TYPE_BYVERTEX: //	the index coincides with the Vertex
								av=a;
								bv=b;
								cv=c;		
								auv=av;
								buv=bv;
								cuv=cv;
								break;

				case GRPFILE3DFBX_INDEXING_TYPE_BYPOLYGON://	the index is in the dicctionary
				default:
								av=e+0;
								bv=e+1;
								cv=e+2;			
								if (uvindexarray)
								{
								auv=(XDWORD)uvindexarray->FastGet(av);
								buv=(XDWORD)uvindexarray->FastGet(bv);
								cuv=(XDWORD)uvindexarray->FastGet(cv);
								}
								break;
				}

							
				if ((auv==-1) || (buv==-1) || (cuv==-1))
				{
					if (!UnmappedWarning)
						FBX_DEBUG(FBX_WARNING,__L("Warning! Mesh %u:%s has unmapped faces!!"),id,currentObject->data->name.Get());
					UnmappedWarning=true;
					continue;
				}
			

				XDWORD an=0;
				XDWORD bn=0;
				XDWORD cn=0;
				XDWORD ab=0;
				XDWORD bb=0;
				XDWORD cb=0;
				XDWORD at=0;
				XDWORD bt=0;
				XDWORD ct=0;

				if (normals.GetSize()>0)
				switch (NormalsIndexingType)
				{
				case GRPFILE3DFBX_INDEXING_TYPE_BYVERTEX:					
					{
							if (NormalsReferenceType==GRPFILE3DFBX_REFERENCE_TYPE_DIRECT)							
							{
								an=a;
								bn=b;
								cn=c;
							}
					}break;
				case GRPFILE3DFBX_INDEXING_TYPE_BYPOLYGON:					
					{
							if (NormalsReferenceType==GRPFILE3DFBX_REFERENCE_TYPE_DIRECT)							
							{
								an=e+0;
								bn=e+1;
								cn=e+2;								
							}
					}break;
					default: break;
				}


#ifdef GRPVERTEX_EXTENDED

				if (binormals.GetSize()>0)
				switch (BinormalsIndexingType)
				{
				case GRPFILE3DFBX_INDEXING_TYPE_BYVERTEX:					
					{
							if (BinormalsReferenceType==GRPFILE3DFBX_REFERENCE_TYPE_DIRECT)							
							{
								ab=a;
								bb=b;
								cb=c;
							}
					}break;
				case GRPFILE3DFBX_INDEXING_TYPE_BYPOLYGON:					
					{
							if (BinormalsReferenceType==GRPFILE3DFBX_REFERENCE_TYPE_DIRECT)							
							{
								ab=e+0;
								bb=e+1;
								cb=e+2;								
							}
					}break;
					default: break;
				}

				if (tangents.GetSize()>0)
				switch (TangentsIndexingType)
				{
				case GRPFILE3DFBX_INDEXING_TYPE_BYVERTEX:					
					{
							if (TangentsReferenceType==GRPFILE3DFBX_REFERENCE_TYPE_DIRECT)							
							{
								at=a;
								bt=b;
								ct=c;
							}
					}break;
				case GRPFILE3DFBX_INDEXING_TYPE_BYPOLYGON:					
					{
							if (TangentsReferenceType==GRPFILE3DFBX_REFERENCE_TYPE_DIRECT)							
							{
								at=e+0;
								bt=e+1;
								ct=e+2;								
							}
					}break;
					default: break;
				}
#endif

				GRPFACE*	face=new GRPFACE();

									face->vertices[0]=new GRPPOINT(vertices.FastGet(a));
									face->vertices[1]=new GRPPOINT(vertices.FastGet(b));
									face->vertices[2]=new GRPPOINT(vertices.FastGet(c));

									if (normals.GetSize()>0)
									{
										face->normals[0]=new GRPPOINT(normals.FastGet(an));
										face->normals[1]=new GRPPOINT(normals.FastGet(bn));
										face->normals[2]=new GRPPOINT(normals.FastGet(cn));
									}
									else
									{
										face->normals[0]=new GRPPOINT(0.0,0.0,0.0);
										face->normals[1]=new GRPPOINT(0.0,0.0,0.0);
										face->normals[2]=new GRPPOINT(0.0,0.0,0.0);
									}

#ifdef GRPVERTEX_EXTENDED

									if (binormals.GetSize()>0)
									{
										face->binormals[0]=new GRPPOINT(binormals.FastGet(ab));
										face->binormals[1]=new GRPPOINT(binormals.FastGet(bb));
										face->binormals[2]=new GRPPOINT(binormals.FastGet(cb));
									}
									else
									{
										face->binormals[0]=new GRPPOINT(0.0,0.0,0.0);
										face->binormals[1]=new GRPPOINT(0.0,0.0,0.0);
										face->binormals[2]=new GRPPOINT(0.0,0.0,0.0);
									}

									if (tangents.GetSize()>0)
									{
										face->tangents[0]=new GRPPOINT(tangents.FastGet(ab));
										face->tangents[1]=new GRPPOINT(tangents.FastGet(bb));
										face->tangents[2]=new GRPPOINT(tangents.FastGet(cb));
									}
									else
									{
										face->tangents[0]=new GRPPOINT(0.0,0.0,0.0);
										face->tangents[1]=new GRPPOINT(0.0,0.0,0.0);
										face->tangents[2]=new GRPPOINT(0.0,0.0,0.0);
									}
#endif

									if (tcoords.GetSize()>0) //case is mapped
									{
									face->texturecoordinates[0]=new GRPPOINT(tcoords.FastGet(auv));
									face->texturecoordinates[1]=new GRPPOINT(tcoords.FastGet(buv));
									face->texturecoordinates[2]=new GRPPOINT(tcoords.FastGet(cuv));
									}
									else
									{
										face->texturecoordinates[0]=new GRPPOINT(0.0,0.0,0.0);
										face->texturecoordinates[1]=new GRPPOINT(0.0,0.0,0.0);
										face->texturecoordinates[2]=new GRPPOINT(0.0,0.0,0.0);
									}

									face->_a=a;
									face->_b=b;
									face->_c=c;
									face->a=vertices.FastGet(a)->index;
									face->b=vertices.FastGet(b)->index;
									face->c=vertices.FastGet(c)->index;

				element->GetMesh()->GetFaces()->Add(face);
			}
		}

		vertices.DeleteContents();
		vertices.DeleteAll();
		normals.DeleteContents();
		normals.DeleteAll();
		tcoords.DeleteContents();
		tcoords.DeleteAll();

#ifdef GRPVERTEX_EXTENDED
		binormals.DeleteContents();
		binormals.DeleteAll();
		tangents.DeleteContents();
		tangents.DeleteAll();
#endif

		element->GetMesh()->SetFaceBuffers();
		element->GetMesh()->isprocedural=false;

		element->GetMesh()->vertexlayers.DeleteAll();

		if (hasPosition)	element->GetMesh()->vertexlayers.Add(GRPVERTEX_CHANNEL_POSITION);
		if (hasNormals)		element->GetMesh()->vertexlayers.Add(GRPVERTEX_CHANNEL_NORMAL);

#ifdef GRPVERTEX_EXTENDED
		if (hasBinormals)	element->GetMesh()->vertexlayers.Add(GRPVERTEX_CHANNEL_BINORMAL);
		if (hasTangents)	element->GetMesh()->vertexlayers.Add(GRPVERTEX_CHANNEL_TANGENT);
#endif

		if (hasTextures)	element->GetMesh()->vertexlayers.Add(GRPVERTEX_CHANNEL_TEXTURE);		

		element->GetMesh()->numberoflayers=element->GetMesh()->vertexlayers.GetSize();

	return true;
}


/*-------------------------------------------------------------------
//	GRPFILE3DFBX::LoadMorpherShapeNode
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			16/07/2015 20:12:10
//	
//	@return 			bool : 
//
//  @param				id : 
//  @param				FBXNode*currentObject : 
*/
/*-----------------------------------------------------------------*/
bool	GRPFILE3DFBX::LoadMorpherShapeNode	(XQWORD id,FBXNode*currentObject)
{
	GRPSUBDEFORMERSHAPE* shape=new GRPSUBDEFORMERSHAPE();
	this->SubdeformerShapeMap.Add(id,shape);
	if (shape==NULL)
	{
		FBX_DEBUG(FBX_ERROR,__L("GRPFILE3DFBX::LoadMorpherShapeNode : Not Enought Memory"));
		return false;
	}

	/*
		Geometry: 688604192, "Geometry::Captured", "Shape" {
		Version: 100
		Indexes: *4 {
			a: 4,5,6,7
		} 
		Vertices: *12 {
			a: 21.8603515625,19.6904220581055,-10.4733047485352,0,14.2865447998047,-28.4562454223633,0,-17.6259918212891,0,0,0,-16.6248016357422
		} 
		Normals: *12 {
			a: 0,0,0,0,0,0,0,0,0,0,0,0
		} 
	}
	*/
	FBXNode* Indexes;
	FBXNode* Vertices;
	FBXNode* Normals;

	if (FindNode(__L("Indexes"),7,currentObject,&Indexes))
	{
		if (Indexes->data->ArrayIndex!=NULL)
		{
		FBXVECTOR<long long>* iv=Indexes->data->ArrayIndex;
		for (XDWORD e=0;e<iv->GetSize();e++)
			shape->indices.Add(iv->FastGet(e));
		}
	}
	else
		return false;


	if (FindNode(__L("Vertices"),8,currentObject,&Vertices))
	{
		int e=0;
		if (Indexes->data->ArrayIndex!=NULL)
		{
		FBXVECTOR<double>* iv=Vertices->data->ArrayValues;
		for (XDWORD i=0;i<iv->GetSize();i+=3)
			{
				GRPVERTEX*	vertex=new GRPVERTEX; 
				memset(vertex,0,sizeof(GRPVERTEX));

										vertex->position[0]=(GLFLOAT)iv->FastGet(i+0);
										vertex->position[1]=(GLFLOAT)iv->FastGet(i+1);
										vertex->position[2]=(GLFLOAT)iv->FastGet(i+2);

				shape->vertices.Add(vertex);
			}		
		}
	}
	else
		return false;

	if (FindNode(__L("Normals"),7,currentObject,&Normals))
	{
		int e=0;
		if (Indexes->data->ArrayIndex!=NULL)
		{
		FBXVECTOR<double>* iv=Normals->data->ArrayValues;

		for (XDWORD i=0,e=0;i<iv->GetSize();i+=3,e++)
			{
				GRPVERTEX*	vertex=shape->vertices.FastGet(e);
										vertex->normal[0]=(GLFLOAT)iv->FastGet(i+0);
										vertex->normal[1]=(GLFLOAT)iv->FastGet(i+1);
										vertex->normal[2]=(GLFLOAT)iv->FastGet(i+2);				
			}		
		}
	}

	return true;
}


/*-------------------------------------------------------------------
//	GRPFILE3DFBX::LoadMaterialNode
*/	
/**	
//	Loads a material
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			23/01/2015 15:50:48
//	
//	@return 			bool : TRUE for success
//
//  @param				XDWORD : id of the node attribute
//  @param				FBXNode*node : node with the data
*/
/*-----------------------------------------------------------------*/
bool	GRPFILE3DFBX::LoadMaterialNode	(XQWORD id,FBXNode* currentMaterial)
{

		GRPMATERIAL* existing=GRPMATERIALLIBRARY::Get()->FindByName(currentMaterial->data->label.Get());
		if (existing!=NULL) //ya existe
		{
			if (!this->MaterialMap.Set(id,existing))
				this->MaterialMap.Add(id,existing);
			return true;
		}

		GRPMATERIAL* material=new GRPMATERIAL();
		this->MaterialMap.Add(id,material);
							
		
		material->GetName()->Set(currentMaterial->data->label.Get());

		//scene->GetMaterialLibrary()->Add(material);
		GRPMATERIALLIBRARY::Get()->Add(material);
		FBXNode* node=0;

		this->TagMap.Add(id,&currentMaterial->data->label);


		/*	
		Max
			P: "ShadingModel", "KString", "", "", "phong"
      P: "EmissiveFactor", "double", "Number", "",0
      P: "AmbientColor", "ColorRGB", "Color", "",0.587999999523163,0.587999999523163,0.587999999523163
      P: "DiffuseColor", "ColorRGB", "Color", "",0.587999999523163,0.587999999523163,0.587999999523163
      P: "TransparentColor", "ColorRGB", "Color", "",1,1,1
      P: "SpecularColor", "ColorRGB", "Color", "",0.899999976158142,0.899999976158142,0.899999976158142
      P: "SpecularFactor", "double", "Number", "",0
      P: "ShininessExponent", "double", "Number", "",2.0000000206574
      P: "Emissive", "Vector3D", "Vector", "",0,0,0
      P: "Ambient", "Vector3D", "Vector", "",0.587999999523163,0.587999999523163,0.587999999523163
      P: "Diffuse", "Vector3D", "Vector", "",0.587999999523163,0.587999999523163,0.587999999523163
      P: "Specular", "Vector3D", "Vector", "",0,0,0
      P: "Shininess", "double", "Number", "",2.0000000206574
      P: "Opacity", "double", "Number", "",1
      P: "Reflectivity", "double", "Number", "",0
			*/
		/*
		maya
			P: "AmbientColor", "Color", "", "A",0.587999999523163,0.587999999523163,0.587999999523163
			P: "DiffuseColor", "Color", "", "A",1,1,1
			P: "TransparencyFactor", "Number", "", "A",1
			P: "SpecularColor", "Color", "", "A",0.620999991893768,0.620999991893768,0.620999991893768
			P: "ShininessExponent", "Number", "", "A",12.9960384368896
			P: "Emissive", "Vector3D", "Vector", "",0,0,0
			P: "Ambient", "Vector3D", "Vector", "",0.587999999523163,0.587999999523163,0.587999999523163
			P: "Diffuse", "Vector3D", "Vector", "",1,1,1
			P: "Specular", "Vector3D", "Vector", "",0.620999991893768,0.620999991893768,0.620999991893768
			P: "Shininess", "double", "Number", "",12.9960384368896
			P: "Opacity", "double", "Number", "",1
			P: "Reflectivity", "double", "Number", "",0
			*/
		if (FindNode(__L("ShadingModel"),12,currentMaterial,&node))
			material->SetShadingModelName(node->data->value);
		
		if (FindNode(__L("AmbientColor"),12,currentMaterial,&node))		
			material->GetAttributes()->SetAmbient((GRPCOLOR)*node);

		if (FindNode(__L("DiffuseColor"),12,currentMaterial,&node))		
			material->GetAttributes()->SetDiffuse((GRPCOLOR)*node);		

		if (FindNode(__L("EmissiveColor"),13,currentMaterial,&node))
			material->GetAttributes()->SetEmission((GRPCOLOR)*node);
	
		if (FindNode(__L("SpecularColor"),13,currentMaterial,&node))		
			material->GetAttributes()->SetSpecular((GRPCOLOR)*node);	

		if (FindNode(__L("SpecularFactor"),14,currentMaterial,&node))		
			material->GetAttributes()->SetSpecularFactor((GLFLOAT)*node);

		if (FindNode(__L("ShininessExponent"),17,currentMaterial,&node))		
			material->GetAttributes()->SetShininess((GLFLOAT)*node);
		
		if (FindNode(__L("Emissive"),8,currentMaterial,&node))
			material->GetAttributes()->SetEmission((GRPCOLOR)*node);
		
		if (FindNode(__L("Opacity"),7,currentMaterial,&node))		
			material->GetAttributes()->SetTransparency((GLFLOAT)*node);		

		if (FindNode(__L("BumpFactor"),7,currentMaterial,&node))		
			material->GetAttributes()->SetBumpFactor((GLFLOAT)*node);		


		if (FindNode(__L("DiffuseFactor"),13,currentMaterial,&node))		
			material->GetAttributes()->SetIntensity((GLFLOAT)*node);	
		else
			material->GetAttributes()->SetIntensity(1.0f);	

		material->GetAttributes()->isupdated=true;
		material->GetAttributes()->UpdateMaterialMatrix();

		//TODO: Multimaterial support , multilayer support

	return true;
}

/*-------------------------------------------------------------------
//	GRPFILE3DFBX::LoadTextureNode
*/	
/**	
//	Loads a texture
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			23/01/2015 15:51:01
//	
//	@return 			bool : TRUE for success
//
//  @param				XDWORD : id of the node attribute
//  @param				FBXNode*node : node with the data 
*/
/*-----------------------------------------------------------------*/
bool	GRPFILE3DFBX::LoadTextureNode		(XQWORD id,FBXNode* currentTexture)
{
	/*
	 Texture: 748873456, "Texture::Map #134", "" {
        Type: "TextureVideoClip"
        Version: 202
        TextureName: "Texture::Map #134"
        Properties70:  {
            P: "UVSet", "KString", "", "", "UVChannel_1"
            P: "UseMaterial", "bool", "", "",1
        }
        Media: "Video::Map #134"
        FileName: "D:\Projects\TestGraphics\resources\ironcrate.jpg"
        RelativeFilename: "ironcrate.jpg"
        ModelUVTranslation: 0,0
        ModelUVScaling: 1,1
        Texture_Alpha_Source: "Alpha_Black"
        Cropping: 0,0,0,0
	*/

		FBXNode* node=0;
		
		if (FindNode(__L("FileName"),8,currentTexture,&node))		
		{
						
			//------------------------------------------------- we got a full resource path

			

			XPATH		xpath;
			XPATH		FinalResourceFile;
			XPATH		fullfilename;
			XPATH		PathAndFileOnResource;
			XPATH		FileOnly;
			FBXSTRING texturename;


							PathAndFileOnResource=GRPMATERIALLIBRARY::Get()->GetResourceLocation()->Get();
			
							fullfilename.Set(node->data->value.Get());
							fullfilename.DeleteCharacter(__C('"'));		

							if (fullfilename.GetSize()==0)//es un tipo diferente de textura
							{
								//---- podria ser un cubemap								
								if (FindNode(__L("3dsMax|parameters|bitmapName|bitmapName[0]"),42,currentTexture,&node))	
								{
									if (node->data->value.GetSize()==0)
									{
										//es un environment mapping en tiempo real
										return true;
									}

									/*
									#define GL_TEXTURE_CUBE_MAP_POSITIVE_X 0x8515  0
									#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X 0x8516	 1
									#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y 0x8517	 2 
									#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y 0x8518  3
									#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z 0x8519  4
									#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z 0x851A	 5
									*/

									XVECTOR<FBXSTRING*> faces;
									XVECTOR<XDWORD> realpositions;
																	realpositions.Add(2); //Up
																	realpositions.Add(3); //Down
																	realpositions.Add(1); //Left
																	realpositions.Add(0); //Right
																	realpositions.Add(5); //Front
																	realpositions.Add(4); //Back

									FBXSTRING facelabel;
									for (int e=0;e<6;e++)
									{										
										facelabel.Format(__L("3dsMax|parameters|bitmapName|bitmapName[%d]"),realpositions.Get(e));
										if (FindNode(facelabel.Get(),42,currentTexture,&node))
										{	
											PathAndFileOnResource=GRPMATERIALLIBRARY::Get()->GetResourceLocation()->Get();
			
											fullfilename.Set(node->data->value.Get());
											fullfilename.DeleteCharacter(__C('"'));		
											PathAndFileOnResource.Slash_Add();						
											fullfilename.GetNamefileExt(FileOnly);
											PathAndFileOnResource+=FileOnly;

											faces.Add(new FBXSTRING(PathAndFileOnResource.Get()));											

											if (e==0)
												texturename=FileOnly;

										}
									}

									if (GRPMATERIALLIBRARY::Get()->Load(&faces,texturename.Get())==GRPMATERIALLOADRESULT_ERROR)
										return false;

									this->TextureMap.SetIsMulti(true);						
									GRPMATERIAL* material=GRPMATERIALLIBRARY::Get()->FindByName(texturename.Get());
									if (material)
									{
									GRPTEXTURE* texture = material->GetLayer(0);
										if (texture)
										{
											this->TextureMap.Add(id,texture);
										}
									}

									GRPOPENGLCTRL::TEXTURE::CurrentTexture=NULL;
																					realpositions.DeleteAll();
																					faces.DeleteContents();
																					faces.DeleteAll();
								}
								return true;
							}
							
			XFILE* xfilecheck=xfactory->Create_File();
						if (xfilecheck==NULL) return false;

			
						PathAndFileOnResource=GRPMATERIALLIBRARY::Get()->GetResourceLocation()->Get();
						PathAndFileOnResource.Slash_Add();						
							fullfilename.GetNamefileExt(FileOnly);
						PathAndFileOnResource+=FileOnly;
						
			FinalResourceFile=PathAndFileOnResource;

			if (!xfilecheck->Exist(PathAndFileOnResource))//------------ check if full file on resource location exists
			{					
				if (!xfilecheck->Exist(fullfilename)) //------------ check if full path exists
				{		
					if (!xfilecheck->Exist(FileOnly))
					{
					FBX_DEBUG(FBX_ERROR,__L("GRPFILE3DFBX::LoadTexture : Error Texture not found on location or default material path %s"),fullfilename.Get());
					FBX_DEBUG(FBX_ERROR,__L("\t\t\t Searched: %s"),node->data->value.Get());
					FBX_DEBUG(FBX_ERROR,__L("\t\t\t Searched: %s"),PathAndFileOnResource.Get());
					delete(xfilecheck);
					return false;
					}
					else
						FinalResourceFile=FileOnly;			
				}
				else
					FinalResourceFile=fullfilename;									
			}
							
				//GRPMATERIALLOADRESULT result=GRPMATERIALLIBRARY::Get()->Load(FinalResourceFile.Get(),currentTexture->data->label.Get());
			GRPMATERIALLOADRESULT result=GRPMATERIALLIBRARY::Get()->Load(FinalResourceFile.Get(),FileOnly.Get());
				if (result==GRPMATERIALLOADRESULT_ERROR)
					return false;		
			
		
			delete(xfilecheck);

			
			this->TextureMap.SetIsMulti(true);
			//this->TextureMap.Add(id,GRPMATERIALLIBRARY::Get()->Select(currentTexture->data->label.Get())->GetLayer(0));
			GRPMATERIAL* material=GRPMATERIALLIBRARY::Get()->FindByName(FileOnly.Get());
			if (material)
			{
			GRPTEXTURE* texture = material->GetLayer(0);
				if (texture)
				{
					this->TextureMap.Add(id,texture);
				}
			}

			GRPOPENGLCTRL::TEXTURE::CurrentTexture=NULL;
		}
		
	return true;
}



/*-------------------------------------------------------------------
//	GRPFILE3DFBX::LoadDeformer
*/	
/**	
//	
//	Loads a Deformer
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			08/05/2015 14:16:00
//	
//	@return 			bool : TRUE for success
//
//  @param				XDWORD : id of the node attribute
//  @param				FBXNode*node : node with the data
*/
/*-----------------------------------------------------------------*/
bool	GRPFILE3DFBX::LoadDeformer							(XQWORD id,	FBXNode* node)
{
		GRPDEFORMER*	deformer=new GRPDEFORMER();
									if (deformer==NULL)
									{
										FBX_DEBUG(FBX_ERROR,__L(" GRPFILE3DFBX::LoadDeformer : Not enought memory"));
										return false;
									}
									deformer->GetName	()->Set(node->data->label.Get());
									deformer->SetTag		(id);

		this->DeformerMap.Add(id,deformer);

	return true;
}

/*-------------------------------------------------------------------
//	GRPFILE3DFBX::LoadSubdeformer
*/	
/**	
//	
//	Loads a subdeformer
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			08/05/2015 14:16:06
//	
//	@return 			bool : TRUE for success
//
//  @param				XDWORD : id of the node attribute
//  @param				FBXNode*node : node with the data 
*/
/*-----------------------------------------------------------------*/
bool	GRPFILE3DFBX::LoadSubdeformer						(XQWORD id,	FBXNode* node)
{

	GRPSUBDEFORMER*	subdeformer=new GRPSUBDEFORMER();
									if (subdeformer==NULL)
									{
										FBX_DEBUG(FBX_ERROR,__L(" GRPFILE3DFBX::LoadSubdeformer : Not enought memory"));
										return false;
									}
									subdeformer->GetName	()->Set(node->data->label.Get());
									subdeformer->SetTag		(id);

		this->SubdeformerMap.Add(id,subdeformer);
	return true;
}


/*-------------------------------------------------------------------
//	GRPFILE3DFBX::LoadAnimationStackNode
*/	
/**	
//	Loads animation stack
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			10/02/2015 14:35:21
//	
//	@return 			bool : TRUE for success
//
//  @param				XDWORD : id of the node attribute
//  @param				FBXNode*node : node with the data 
*/
/*-----------------------------------------------------------------*/
bool		GRPFILE3DFBX::LoadAnimationStackNode(XQWORD id,FBXNode* animNode)
{
		GRPANIMATIONSTACK* astack=new GRPANIMATIONSTACK();
							if (astack==NULL)
							{
								FBX_DEBUG(FBX_ERROR,__L(" GRPFILE3DFBX::LoadAnimationStackNode : Not enought memory"));
								return false;
							}
		//astack->GetName	()->Set(animNode->data->label.Get());
		FBXSTRING file;
		this->FBXfile.GetNamefile	(file);
		astack->SetName						(file.Get());
		astack->SetTag						(id);

		this->AnimationStackMap.Add(id,astack);
		

		FBXNode* node=0;

		astack->SetMinFrame(0);
		astack->SetMaxFrame(0);

		if (FindNode(__L("LocalStart"),10,animNode,&node))
		{
			XQWORDSIG minframe=node->data->value.Fast_ConvertToSQWord();
			astack->SetMinFrame(minframe);
			astack->SetMinTime(minframe);
		//	FBX_DEBUG(FBX_STATUS,__L("LocalStart: %lld"),minframe);
		}

		if (FindNode(__L("LocalStop"),9,animNode,&node))
		{
			XQWORDSIG maxframe=node->data->value.Fast_ConvertToSQWord();
			astack->SetMaxTime	(maxframe);		
			astack->SetMaxFrame	(maxframe);
		}



return true;
}

/*-------------------------------------------------------------------
//	GRPFILE3DFBX::LoadAnimationLayerNode
*/	
/**	
//	Loads Animation Layer
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			10/02/2015 14:35:21
//	
//	@return 			bool : TRUE for success
//
//  @param				XDWORD : id of the node attribute
//  @param				FBXNode*node : node with the data
*/
/*-----------------------------------------------------------------*/
bool		GRPFILE3DFBX::LoadAnimationLayerNode(XQWORD id,FBXNode* animNode)
{
		GRPANIMATIONLAYER* alayer=new GRPANIMATIONLAYER();
							if (alayer==NULL)
							{
								FBX_DEBUG(FBX_ERROR,__L(" GRPFILE3DFBX::LoadAnimationLayerNode : Not enought memory"));
								return false;
							}
		alayer->GetName	()->Set(animNode->data->label.Get());
		alayer->SetTag		(id);		

		this->AnimationLayerMap.Add(id,alayer);
		
return true;
}

/*-------------------------------------------------------------------
//	GRPFILE3DFBX::LoadAnimationCurveNode
*/	
/**	
//	Loads Animation Curve
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			10/02/2015 14:35:21
//	
//	@return 			bool : TRUE for success
//
//  @param				XDWORD : id of the node attribute
//  @param				FBXNode*node : node with the data 
*/
/*-----------------------------------------------------------------*/

bool GRPFILE3DFBX::IsConnected(XQWORD id)
{		
	if (ObjectConnectionsMap.Find(id)==NOTFOUND)
		return false;
	
	return true;
}

/*-------------------------------------------------------------------
//	GRPFILE3DFBX::LoadAnimationCurveNode
*/	
/**	
//	Loads Animation Curve Node
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			01/06/2015 10:39:46
//	
//	@return 			bool : TRUE for success
//
//  @param				XDWORD : id of the node attribute
//  @param				FBXNode*node : node with the data 
*/
/*-----------------------------------------------------------------*/
bool		GRPFILE3DFBX::LoadAnimationNode(XQWORD id,FBXNode* animNode)
{
		FBXNode* node=NULL;
		animNode->data->label.Copy(15,animNode->data->label.GetSize(),animationnodename);		
		
		GRPANIMATIONCURVETYPE type;

		if (animationnodename.Compare(__L("T"),1,false)==0)
		{
			type=GRPANIMATIONCURVETYPE_TRANSLATION;
		}
		else
		if (animationnodename.Compare(__L("S"),1,false)==0)
		{
			type=GRPANIMATIONCURVETYPE_SCALE;
		}
		else
		if (animationnodename.Compare(__L("R"),1,false)==0)
		{
			type=GRPANIMATIONCURVETYPE_ROTATION;
		}
		else
		if (animationnodename.Compare(__L("Visibility"),10,false)==0)
		{
			type=GRPANIMATIONCURVETYPE_VISIBILITY;
		}
		else
		if (animationnodename.Compare(__L("DiffuseColor"),12,false)==0)
		{
			type=GRPANIMATIONCURVETYPE_DIFFUSECOLOR;
		}
		else
		if (animationnodename.Compare(__L("AmbientColor"),12,false)==0)
		{
			type=GRPANIMATIONCURVETYPE_AMBIENTCOLOR;
		}
		else
		if (animationnodename.Compare(__L("SpecularColor"),13,false)==0)
		{
			type=GRPANIMATIONCURVETYPE_SPECULARCOLOR;
		}
		else
		if (animationnodename.Compare(__L("EmissiveColor"),13,false)==0)
		{
			type=GRPANIMATIONCURVETYPE_EMISSIVECOLOR;
		}
		else
		if (animationnodename.Compare(__L("Roll"),4,false)==0)
		{
			type=GRPANIMATIONCURVETYPE_ROLL;
		}
		else
		if (animationnodename.Compare(__L("Intensity"),9,false)==0)
		{
			type=GRPANIMATIONCURVETYPE_INTENSITY;
		}
		else
		if (animationnodename.Compare(__L("Color"),5,false)==0)
		{
			type=GRPANIMATIONCURVETYPE_DIFFUSECOLOR;
		}
		else
		if (animationnodename.Compare(__L("ShininessExponent"),17,false)==0)
		{
			type=GRPANIMATIONCURVETYPE_SHININESSEXPONENT;
		}
		else
		if (animationnodename.Compare(__L("SpecularFactor"),14,false)==0)
		{
			type=GRPANIMATIONCURVETYPE_SPECULARFACTOR;
		}
		else
		if (animationnodename.Compare(__L("DiffuseFactor"),13,false)==0)
		{
			type=GRPANIMATIONCURVETYPE_DIFFUSEFACTOR;
		}
		else
		if (animationnodename.Compare(__L("TransparencyFactor"),18,false)==0)
		{
			type=GRPANIMATIONCURVETYPE_TRANSPARENCYFACTOR;
		}
		else
		if (animationnodename.Compare(__L("FieldOfView"),11,false)==0)
		{
			type=GRPANIMATIONCURVETYPE_FOV;
		}
		else
		if (animationnodename.Compare(__L("FieldOfViewX"),12,false)==0)
		{
			type=GRPANIMATIONCURVETYPE_FIELDOFVIEWX;
		}
		else
		if (animationnodename.Compare(__L("FieldOfViewY"),12,false)==0)
		{
			type=GRPANIMATIONCURVETYPE_FIELDOFVIEWY;
		}
		else
		if (animationnodename.Compare(__L("DeformPercent"),13,false)==0)
		{
			type=GRPANIMATIONCURVETYPE_DEFORMPERCENT;
		}
		else
		if (animationnodename.Compare(__L("UserValue"),9,false)==0)
		{
			type=GRPANIMATIONCURVETYPE_USERVALUE;
		}
		else
		if (animationnodename.Compare(__L("value"),5,false)==0)
		{
			type=GRPANIMATIONCURVETYPE_USERVALUE;
		}
		else
		{
			type=GRPANIMATIONCURVETYPE_UNKNOWN;
			return false;				
		}

				GRPANIMATIONNODE* anode=new GRPANIMATIONNODE();
											if (anode==NULL)
											{
												FBX_DEBUG(FBX_ERROR,__L(" GRPFILE3DFBX::LoadAnimationNode : Not enought memory"));
												return false;
											}


		anode->GetName	()->Set(animNode->data->label.Get());
		anode->SetTag		(id);

		TagMap.Set(id,anode->GetName	());

		this->AnimationNodeMap.Add(id,anode);

		anode->SetType(type);
		
		//------------------ load Nodes Default values

		FBXNode* properties_node=animNode->Children->Get(0);
		if (properties_node)
		if (properties_node->data)
			if (properties_node->data->name.Find(__L("Properties"),false,0)!=NOTFOUND)
			{
						for (XDWORD e=0;e<properties_node->Children->GetSize();e++)
						{
							FBXNode* child=properties_node->Children->FastGet(e);
							GRPANIMATIONCURVEAXIS type=GetPropertyTypeByName(&child->data->name);							
							if (type==GRPANIMATIONCURVEAXIS_VISIBILITY)
							{
								value=child->data->value;
								anode->GetDefaultValues()->Add(type,(float)value.ConvertToDouble());
							}
						}
			}





return true;
}


/*-------------------------------------------------------------------
//	GRPFILE3DFBX::LoadAnimationCurve
*/	
/**	
//	Loads animatin Curve
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			10/02/2015 16:05:16
//	
//	@return 			bool : TRUE for success
//
//  @param				XDWORD : id of the node attribute
//  @param				FBXNode*node : node with the data
*/
/*-----------------------------------------------------------------*/
bool		GRPFILE3DFBX::LoadAnimationCurve(XQWORD id,FBXNode* animNode)
{
	//------- Maya is so great, it creates unconnected nodes from deleted keys. 
	//------- If we don't check if the curve is connected to something, we will have garbage being animated, and memory leaks. 
		//if (!IsConnected(id))	return true;


		GRPANIMATIONCURVE* acurve=new GRPANIMATIONCURVE();
								if (acurve==NULL)
							{
								FBX_DEBUG(FBX_ERROR,__L(" GRPFILE3DFBX::LoadAnimationCurve : Not enought memory"));
								return false;
							}
		acurve->GetName	()->Set(animNode->data->label.Get());
		acurve->SetTag		(id);

		this->AnimationCurveMap.Add(id,acurve);
		
		FBXNode* node=NULL;

		if (FindChildNode(__L("KeyTime"),7,animNode,&node))			
		{
			if (node->data->ArrayIndex)
			{			
			acurve->keytime.SetAddInLimit(node->data->ArrayIndex->GetSize());
			XDWORD nsize=node->data->ArrayIndex->GetSize();
			for (XDWORD e=0;e<nsize;e++)	
				acurve->keytime.Add(node->data->ArrayIndex->FastGet(e));			
			//acurve->keytime.Copy(node->data->ArrayIndex);
			}
		}
		if (FindChildNode(__L("KeyValueFloat"),13,animNode,&node))			
		{
			if (node->data->ArrayValues)
			{
			acurve->keyvalues.SetAddInLimit(node->data->ArrayValues->GetSize());
			XDWORD nsize=node->data->ArrayValues->GetSize();
			for (XDWORD e=0;e<nsize;e++)							
			acurve->keyvalues.Add(node->data->ArrayValues->FastGet(e));		
			//acurve->keyvalues.Copy(node->data->GetArrayValues());
			}
		}
		if (FindChildNode(__L("KeyAttrFlags"),12,animNode,&node))			
		{
			if (node->data->ArrayIndex)
			{
			acurve->keyattributes.SetAddInLimit(node->data->ArrayIndex->GetSize());		
			XDWORD nsize=node->data->ArrayIndex->GetSize();
			for (XDWORD e=0;e<nsize;e++)				
				acurve->keyattributes.Add(node->data->ArrayIndex->FastGet(e));						
			}
		}
		if (FindChildNode(__L("KeyAttrDataFloat"),16,animNode,&node))			
		{
			if (node->data->ArrayIndex)	
			{
			acurve->keyattrdatafloat.SetAddInLimit(node->data->ArrayIndex->GetSize());
			XDWORD nsize=node->data->ArrayIndex->GetSize();
			for (XDWORD e=0;e<nsize;e++)
				acurve->keyattrdatafloat.Add(node->data->ArrayIndex->FastGet(e));			
			}
		}
		if (FindChildNode(__L("KeyAttrRefCount"),15,animNode,&node))			
		{
			if (node->data->ArrayIndex)	
			{
				acurve->keyattrrefcount.SetAddInLimit(node->data->ArrayIndex->GetSize());						
				XDWORD nsize=node->data->ArrayIndex->GetSize();
			for (XDWORD e=0;e<nsize;e++)			
				acurve->keyattrrefcount.Add((XDWORD)node->data->ArrayIndex->FastGet(e));			
			}
		}
	return true;
}


/*-------------------------------------------------------------------
//	GRPFILE3DFBX::LoadGlobalSettings
*/	
/**	
//	Loads Global Settings
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			23/01/2015 12:03:00
//	
//	@return 			bool : TRUE if success 
//
*/
/*-----------------------------------------------------------------*/
bool GRPFILE3DFBX::LoadGlobalSettings()
{
	//------------------------- global
	FBXNode* node=NULL;

	XDWORD	UpAxis					=1;
	int			UpAxisSign			=1;
	XDWORD	FrontAxis				=2;
	int			FrontAxisSign		=1;
	XDWORD	RightAxis				=0;
	int			RightAxisSign		=1;

	FBXNode* GlobalSettings=0;

		if (!FindNode(__L("GlobalSettings"),14,&GlobalSettings))
			return false;
		

		if (FindNode(__L("UpAxis"),6,GlobalSettings,&node))
		{
			UpAxis=node->data->value.Fast_ConvertToInt();			
		}else return false;
		
		if (FindNode(__L("UpAxisSign"),10,GlobalSettings,&node))
		{
			UpAxisSign=node->data->value.Fast_ConvertToInt();			
		}else return false;

		if (FindNode(__L("FrontAxis"),9,GlobalSettings,&node))
		{
			FrontAxis=node->data->value.Fast_ConvertToInt();			
		}else return false;

		if (FindNode(__L("FrontAxisSign"),13,GlobalSettings,&node))
		{
			FrontAxisSign=node->data->value.Fast_ConvertToInt();			
		}		else return false;

		if (FindNode(__L("CoordAxis"),9,GlobalSettings,&node))
		{
			RightAxis=node->data->value.Fast_ConvertToInt();			
		}else return false;

		if (FindNode(__L("CoordAxisSign"),13,GlobalSettings,&node))
		{
			RightAxisSign=node->data->value.Fast_ConvertToInt();			
		}else return false;

		if (FindNode(__L("UnitScaleFactor"),15,GlobalSettings,&node))
		{
			UnitScaleFactor=node->data->value.Fast_ConvertToFloat();	
			
			if (FindNode(__L("OriginalUnitScaleFactor"),23,GlobalSettings,&node))
			{
				//UnitScaleFactor*=node->data->value.Fast_ConvertToFloat();	
			}
			
			
		}else return false;

		if (FindNode(__L("TimeMode"),8,GlobalSettings,&node))
		{
			TimeMode=node->data->value.Fast_ConvertToQWord();				
		}

		if (FindNode(__L("TimeProtocol"),12,GlobalSettings,&node))
		{
			TimeProtocol=node->data->value.Fast_ConvertToQWord();					
		}

		if (FindNode(__L("CustomFrameRate"),15,GlobalSettings,&node))
		{
			CustomFrameRate=node->data->value.Fast_ConvertToDWord();		
		}

		if (FindNode(__L("DefaultCamera"),13,GlobalSettings,&node))
		{
			this->DefaultCamera=node->data->value;
		}		
		this->object->UpVector.vector[UpAxis]		=float(UpAxisSign);		
		
		return true;
}

/*-------------------------------------------------------------------
//	GRPFILE3DFBX::LoadSkinDeformer
*/	
/**	
//  Loads a Skin Deformer
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			22/05/2015 12:15:23
//	
//	@return 			bool : TRUE for success
//
//  @param				XDWORD : id of the node attribute
//  @param				FBXNode*node : node with the data
*/
/*-----------------------------------------------------------------*/
bool GRPFILE3DFBX::LoadSkinDeformer				(XQWORD id,FBXNode* skinnode)
{
	
		GRPSKIN*	skin=new GRPSKIN();
							if (skin==NULL)
							{
								FBX_DEBUG(FBX_ERROR,__L(" GRPFILE3DFBX::LoadSkinDeformer : Not enought memory"));
								return false;
							}
							skin->GetName	()->Set(skinnode->data->label.Get());
							skin->SetTag	(id);
		
		this->SkinMap.Add(id,skin);
		
		TypesMap.Add(id,GRPFILE3DFBX_CONNECTION_TYPE_DEFORMER_SKIN);
	
	return true;
}

/*-------------------------------------------------------------------
//	GRPFILE3DFBX::LoadSkinPose
*/	
/**	
//	Loads a Skin Pose	
//
//	@author				Diego Martinez Ruiz de Gaona
//	@version			22/05/2015 12:15:15
//	
//	@return 			bool : TRUE for success
//
//  @param				XDWORD : id of the node attribute
//  @param				FBXNode*node : node with the data 
*/
/*-----------------------------------------------------------------*/
bool GRPFILE3DFBX::LoadSkinPose						(XQWORD id,FBXNode* skinnode)
{
	/*
	Pose: 989322288, "Pose::BIND_POSES", "BindPose" {
		Type: "BindPose"
		Version: 100
		NbPoseNodes: 6
		PoseNode:  {
			Node: 1578388688
			Matrix: *16 {
				a: 0.0168011412024498,3.67327923633809e-009,0.999859094619751,0,-0.999859094619751,-2.18304279542281e-007,0.0168011412024498,0,2.18335117097013e-007,-1,5.00069083364529e-012,0,0.117858961224556,-3.15748275170336e-006,72.2347946166992,1
			} 
		}
		PoseNode:  {
			Node: 1578386368
			Matrix: *16 {
				a: 0.0168011412024498,3.67327923633809e-009,0.999859094619751,0,-0.999859094619751,-2.18304279542281e-007,0.0168011412024498,0,2.18335117097013e-007,-1,5.00069083364529e-012,0,-0.14875765144825,-2.46393028646708e-006,56.3681564331055,1
			} 
			...
			..
			..

		}		
	}
	*/
		FBXNode* node=NULL;
		XDWORD poseStartIndex=0;

		if (skinnode->Children==NULL || skinnode->Children->GetSize()==0)
			return false;

					if (FindNode(__L("NbPoseNodes"),11,skinnode,&node))
					{
						XDWORD nPose=*node;
						FBXNode* posenode=NULL;						

						for (XDWORD p=0;p<nPose;p++)
						if (FindNode(__L("PoseNode"),4,skinnode,&posenode))
						{
							GRPSKINPOSE* Pose=new GRPSKINPOSE();	
							if (Pose==NULL)
							{
								FBX_DEBUG(FBX_ERROR,__L(" GRPFILE3DFBX::LoadSkinPose : Not enought memory"));
								return false;
							}

													if (FindNode(__L("Node"),4,posenode,&node))
													{
														Pose->NodeId=*node;
														if (SkinLimbMap.Find(Pose->NodeId)!=NOTFOUND)														
															Pose->Node=SkinLimbMap.Get(Pose->NodeId);														
														else
														{
															if (LocatorMap.Find(Pose->NodeId)!=NOTFOUND)														
															{																
																Pose->Node=LocatorMap.Get(Pose->NodeId);		
															}
															else
															{
																if (NodeMap.Find(Pose->NodeId)!=NOTFOUND)														
																{
																	Pose->Node=NodeMap.Get(Pose->NodeId);		
																} 

															}
														}
													}

													if (FindNode(__L("Matrix"),6,posenode,&node))
														Pose->Matrix=*node;

							this->SkinPoseMap.Add(Pose->NodeId,Pose);
						}									
					}


	return true;
}
/*-------------------------------------------------------------------
//	GRPFILE3DFBX::LoadSkinCluster
*/	
/**	
//	
//	Loads A Skin Cluster
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			22/05/2015 12:15:04
//	
//	@return 			bool : TRUE for success
//
//  @param				XDWORD : id of the node attribute
//  @param				FBXNode*node : node with the data
*/
/*-----------------------------------------------------------------*/
bool GRPFILE3DFBX::LoadSkinCluster				(XQWORD id,FBXNode* skinnode)
{
	/*
	Deformer: 992258416, "SubDeformer::", "Cluster" {
		Version: 100
		UserData: "", ""
		Indexes: *16 {
			a: 0,1,2,3,8,9,10,11,12,13,14,15,16,17,18,19
		} 
		Weights: *16 {
			a: 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
		} 
		Transform: *16 {
			a: 1.29918310157715e-017,-0.212172701954821,4.63247517131066e-008,0,0,-6.22627660468122e-008,-0.2851706445217,0,0.999999999999951,6.12323399573617e-017,-1.33691701109661e-023,0,2.77909702062593,-0.0938500030584767,-0.211002214233241,1
		} 
		TransformLink: *16 {
			a: 6.12323399573707e-017,0,1.00000000000005,0,-1.00000000000005,-2.18335117035795e-007,6.12323399573707e-017,0,2.18335117035784e-007,-1,-1.33691701109668e-023,0,-0.199859976768494,-1.89809270523256e-008,0.434233009815216,1
		} 
	}
	*/

//They are different because FBX writer will write it as:
//Transform = TransformLink.Inverse() * Transform; TransfromLink is different, so the Transform is different.
//
//In other word, what you can see in FBX file is not the "original" Transform. 
//But then FBX reader will do "Transform = TransformLink * Transform;" to compensate what is done in writer.
//So if you read/importer the fbx file and then check the Transform matrices, they will be the same.
//
//Honestly, I don't know why it is designed like this, and we actually had a similar discussion back then internally and here is one answer from Robert:
//"From what I remember, I think this was because the inversed transform was going right into the graphic card transform in FiLMBOX for real-time rendering. Since the renderer needed the inverse matrix, that's how it got stored in the file. I don't think it really serves a good purpose, but at least it's not harmful."
//
//So if you can not use fbx sdk directly you can process the data in file as what I describe above in the fbx reader to compensate.


	GRPSKINCLUSTER*	skin=new GRPSKINCLUSTER();
							if (skin==NULL)
							{
								FBX_DEBUG(FBX_ERROR,__L(" GRPFILE3DFBX::LoadSkinCluster : Not enought memory"));
								return false;
							}

							skin->GetName	()->Set(skinnode->data->label.Get());
							skin->SetTag	(id);
		
		this->SkinClusterMap.Add(id,skin);

		FBXNode* node;

		if (FindNode(__L("Indexes"),7,skinnode,&node))
		{
			if (node->data->ArrayIndex!=NULL)
			for (XDWORD e=0;e<node->data->ArrayIndex->GetSize();e++)
				skin->Indexes.Add((XDWORD)node->data->ArrayIndex->FastGet(e));
		}
		if (FindNode(__L("Weights"),7,skinnode,&node))
		{
			if (node->data->ArrayValues!=NULL)
			for (XDWORD e=0;e<node->data->ArrayValues->GetSize();e++)
			{
				double w=(double)node->data->ArrayValues->FastGet(e);
				if (w==0.0f) 
						continue;
				skin->Weights.Add((float)w);
			}
		}
		if (FindNode(__L("Transform"),9,skinnode,&node))		
			skin->Transform=*node;
		if (FindNode(__L("TransformLink"),9,skinnode,&node))		
			skin->TransformLink=*node;		

	return true;
}


/*-------------------------------------------------------------------
//	GRPFILE3DFBX::FindNode
*/	
/**	
//	
//	Finds node using comparators
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			21/01/2015 12:59:32
//	
//	@return 			bool : TRUE if found
//
//  @param				CurrentNode : Recursive node
//  @param				Comparator : FBXNodeComparator* comparator strategy object
*/
/*-----------------------------------------------------------------*/
bool		GRPFILE3DFBX::FindNode(FBXNode* currentnode,FBXNodeComparator* comparator,FBXNode** result)
{
		if (currentnode->data!=NULL)
			if (comparator->Compare(currentnode)==0)
			{
				*result=currentnode;
				return true;
			}

	if (currentnode->Children!=NULL)
		for(int e=currentnode->Children->GetSize()-1;e>=0;e--)		
			if (FindNode(currentnode->Children->FastGet(e),comparator,result))
				return true;		
	
	return false;
}

/*-------------------------------------------------------------------
//	GRPFILE3DFBX::DeleteTree
*/	
/**	
//	Clears Tree Recursively
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			10/02/2015 14:28:14
//	
//  @param				FBXNode*  fnode : Recursive node
*/
/*-----------------------------------------------------------------*/
void		GRPFILE3DFBX::DeleteTree				(FBXNode* fnode)
{
	if (fnode->Children)
	for (XDWORD e=0;e<fnode->Children->GetSize();e++)
		DeleteTree(fnode->Children->FastGet(e));

	if (fnode->data)
	{
		delete(fnode->data);
		fnode->data=NULL;
	}

	if (fnode->Children)
	{
		fnode->Children->DeleteAll();
		delete(fnode->Children);
		fnode->Children=NULL;
	}

	delete(fnode);
	fnode=NULL;
}



/*-------------------------------------------------------------------
//	GetPropertyTypeByName
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			16/07/2015 11:45:11
//	
//	@return 			GRPANIMATIONCURVEAXIS : 
//
//  @param				subtype : 
*/
/*-----------------------------------------------------------------*/
GRPANIMATIONCURVEAXIS	GRPFILE3DFBX::GetPropertyTypeByName(FBXSTRING* subtype)
{
	GRPANIMATIONCURVEAXIS axis=GRPANIMATIONCURVEAXIS_UNKNOWN;

				
				if (subtype->Compare(__L("d|X"),3,false)==0)																											
				axis=GRPANIMATIONCURVEAXIS_X;
				else				
				if (subtype->Compare(__L("d|Y"),3,false)==0)																											
				axis=GRPANIMATIONCURVEAXIS_Y;
				else
				if (subtype->Compare(__L("d|Z"),3,false)==0)																											
				axis=GRPANIMATIONCURVEAXIS_Z;
				else
				if (subtype->Compare(__L("d|R"),3,false)==0)	
				axis=GRPANIMATIONCURVECOLOR_R;
				else
				if (subtype->Compare(__L("d|G"),3,false)==0)	
				axis=GRPANIMATIONCURVECOLOR_G;
				else
				if (subtype->Compare(__L("d|B"),3,false)==0)	
				axis=GRPANIMATIONCURVECOLOR_B;
				else
				if (subtype->Compare(__L("d|Visibility"),12,false)==0)																											
				axis=GRPANIMATIONCURVEAXIS_VISIBILITY;				
				else
				if (subtype->Compare(__L("d|DiffuseFactor"),15,false)==0)	
				axis=GRPANIMATIONCURVECOLOR_DIFFUSEFACTOR;
				else
				if (subtype->Compare(__L("d|SpecularFactor"),16,false)==0)	
				axis=GRPANIMATIONCURVECOLOR_SPECULARFACTOR;
				else
				if (subtype->Compare(__L("d|ShininessExponent"),19,false)==0)	
				axis=GRPANIMATIONCURVETYPE_SHININESSFACTOR;	
				else
				if (subtype->Compare(__L("d|Intensity"),11,false)==0)	
				axis=GRPANIMATIONCURVECOLOR_INTENSITY;	
				else
				if (subtype->Compare(__L("d|FieldOfView"),13,false)==0)	
				axis=GRPANIMATIONCURVEFOV_FIELDOFVIEW;	
				else
				if (subtype->Compare(__L("d|FieldOfViewX"),14,false)==0)	
				axis=GRPANIMATIONCURVEFOV_FIELDOFVIEWX;	
				else
				if (subtype->Compare(__L("d|FieldOfViewY"),14,false)==0)	
				axis=GRPANIMATIONCURVEFOV_FIELDOFVIEWX;	
				else
				if (subtype->Compare(__L("d|TransparencyFactor"),20,false)==0)	
				axis=GRPANIMATIONCURVECOLOR_TRANSPARENCYFACTOR;
				else
				if (subtype->Compare(__L("d|DeformPercent"),15,false)==0)	
				axis=GRPANIMATIONCURVEAXIS_DEFORMPERCENT;
				else
				if (subtype->Compare(__L("d|UserValue"),11,false)==0)	
				axis=GRPANIMATIONCURVEAXIS_USERVALUE;
				else
				if (subtype->Compare(__L("d|value"),7,false)==0)	
				axis=GRPANIMATIONCURVEAXIS_USERVALUE;
				else				
				axis=GRPANIMATIONCURVEAXIS_UNKNOWN;																																																																																	
				
		return axis;

}

/*-------------------------------------------------------------------
//	GRPFILE3DFBX::UpdateTask
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			09/02/2017 10:12:59
//	
*/
/*-----------------------------------------------------------------*/
void		GRPFILE3DFBX::UpdateTask()
{
		switch (CurrentStep)
		{
	  case GRPFILE3DFBX_STEP_LOADOBJECTS:				this->percent += 50.0f / Objects->Children->GetSize(); break;
		case GRPFILE3DFBX_STEP_LOADINGDICTIONARY:	this->percent += 50.0f / ((float)this->ObjectConnectionsMap.GetSize() + (float)PropertyConnectionsMap.GetSize()); break;
		}
		this->Publish(GRPMSG_STEP);

		/*
	if (task!=NULL)
	{
		if (this->task->GetTotal()==1.0f)
			this->task->SetTotal(100.0f);

		switch(CurrentStep)
		{
		case GRPFILE3DFBX_STEP_PARSING:
			//this->task->SetCurrent(((float)this->CurrentLine/(float)this->nlines)*33.0f);		
			this->task->Add(33.2f/this->nlines);					
			this->task->Publish(GRPMSG_STEP);
			break;
		case GRPFILE3DFBX_STEP_LOADOBJECTS:
			//this->task->SetCurrent(33.0f + ((float)this->currentObject/(float)Objects->Children->GetSize())*33);		
			//this->task->Add(33.2f/Objects->Children->GetSize());		
			this->task->Add(50.0f/Objects->Children->GetSize());		
			this->task->Publish(GRPMSG_STEP);
			break;
		case GRPFILE3DFBX_STEP_LOADINGDICTIONARY:
			//this->task->SetCurrent(66.0f + ((float)this->currentDictionary/((float)this->ObjectConnectionsMap.GetSize()+(float)PropertyConnectionsMap.GetSize()))*33);		
			//this->task->Add(33.2f/((float)this->ObjectConnectionsMap.GetSize()+(float)PropertyConnectionsMap.GetSize()));		
			this->task->Add(50.0f/((float)this->ObjectConnectionsMap.GetSize()+(float)PropertyConnectionsMap.GetSize()));		
			this->task->Publish(GRPMSG_STEP);
			break;
		}		
	//	XDEBUG_PRINTCOLOR(0,__L("TASK: %f"),task->current);
	}
	*/
}