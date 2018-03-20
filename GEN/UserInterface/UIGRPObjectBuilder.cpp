
/*------------------------------------------------------------------------------------------
//	UIGRPOBJECTBUILDER.CPP
//	
//	build objects that are used by GRP interface
//   
//	Author						: Imanol Celaya Ruiz de Alegria
//	Date Of Creation	: 09/08/2016 17:11:26
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "UIGRPObjectBuilder.h"
#include "UI3DObjectLoader.h"

#include "GRPObject.h"
#include "GRPElement.h"
#include "GRPNode.h"
#include "GRPTextBox.h"
#include "GRPScene.h"
#include "GRPRenderer.h"
#include "GRPObjectBuilder.h"
#include "GRPMaterialLibrary.h"
#include "GRPFontManager.h"
#include "GRPViewport.h"
#include "GRPTextBox.h"
#include "GRP2DCollisionDetection.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//	operator<
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			10/08/2016 9:51:54
//	
//	@return 			bool : 
//
//  @param				lhs : 
//  @param				rhs : 
*/
/*-----------------------------------------------------------------*/
bool operator<(const UIGRPOBJECTBUILDER::PROPERTIES lhs, const UIGRPOBJECTBUILDER::PROPERTIES rhs)
{
	return lhs.cornerradious < rhs.cornerradious;
}


/*-------------------------------------------------------------------
//	operator<
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			10/08/2016 9:51:54
//	
//	@return 			bool : 
//
//  @param				lhs : 
//  @param				rhs : 
*/
/*-----------------------------------------------------------------*/
bool operator<(const UIGRPOBJECTBUILDER::TEXTPROPERTIES& lhs, const UIGRPOBJECTBUILDER::TEXTPROPERTIES& rhs)
{
	if(*lhs.string == *rhs.string)
		{
			if(lhs.bold == rhs.bold)
				{
					return (lhs.italic < rhs.italic);
				}
			else if(lhs.italic == rhs.italic)
				{
					return (lhs.bold < rhs.bold);
				}

			return (lhs.bold < rhs.bold) && (lhs.italic < rhs.italic);
		}

	return (*lhs.string < *rhs.string); 
}


/*-------------------------------------------------------------------
//	UIGRPOBJECTBUILDER::PROPERTIES::PROPERTIES
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			09/08/2016 17:14:58
//	
//	@return 			UIGRPOBJECTBUILDER::PROPERTIES::PROPERTIES(const : 
//
//  @param				rhs : 
*/
/*-----------------------------------------------------------------*/
UIGRPOBJECTBUILDER::PROPERTIES::PROPERTIES()
{
	Clean();
}


/*-------------------------------------------------------------------
//	UIGRPOBJECTBUILDER::PROPERTIES::PROPERTIES
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			09/08/2016 17:14:58
//	
//	@return 			UIGRPOBJECTBUILDER::PROPERTIES::PROPERTIES(const : 
//
//  @param				rhs : 
*/
/*-----------------------------------------------------------------*/
UIGRPOBJECTBUILDER::PROPERTIES::PROPERTIES(const PROPERTIES& rhs)
{
	Clean();

	this->cornerradious = rhs.cornerradious;
}



/*-------------------------------------------------------------------
//	UIGRPOBJECTBUILDER::PROPERTIES::operator=
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			09/08/2016 17:15:05
//	
//	@return 			UIGRPOBJECTBUILDER::PROPERTIES& : 
//
//  @param				rhs : 
*/
/*-----------------------------------------------------------------*/
UIGRPOBJECTBUILDER::PROPERTIES& UIGRPOBJECTBUILDER::PROPERTIES::operator=(const PROPERTIES& rhs)
{
	this->cornerradious = rhs.cornerradious;

	return *this;
}




/*-------------------------------------------------------------------
//	UIGRPOBJECTBUILDER::PROPERTIES::operator==
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			09/08/2016 17:15:12
//	
//	@return 			bool : 
//
//  @param				rhs : 
*/
/*-----------------------------------------------------------------*/
bool UIGRPOBJECTBUILDER::PROPERTIES::operator==(const PROPERTIES& rhs)
{
	return this->cornerradious == rhs.cornerradious;
}



/*-------------------------------------------------------------------
//	UIGRPOBJECTBUILDER::PROPERTIES::operator!=
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			09/08/2016 17:15:18
//	
//	@return 			bool : 
//
//  @param				rhs : 
*/
/*-----------------------------------------------------------------*/
bool UIGRPOBJECTBUILDER::PROPERTIES::operator!=(const PROPERTIES& rhs)
{
	return !(*this == rhs);
}



/*-------------------------------------------------------------------
//	UIGRPOBJECTBUILDER::PROPERTIES::operator<
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			09/08/2016 17:15:32
//	
//	@return 			bool : 
//
//  @param				rhs : 
*/
/*-----------------------------------------------------------------*/
bool UIGRPOBJECTBUILDER::PROPERTIES::operator<(const PROPERTIES& rhs)
{
	return this->cornerradious < rhs.cornerradious;
}




/*-------------------------------------------------------------------
//	UIGRPOBJECTBUILDER::PROPERTIES::TEXTPROPERTIES
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			09/08/2016 17:15:32
//	
*/
/*-----------------------------------------------------------------*/
UIGRPOBJECTBUILDER::TEXTPROPERTIES::TEXTPROPERTIES()
{
	Clean();

	if(!string)
	{
		string = new XSTRING;
	}
}

/*-------------------------------------------------------------------
//	UIGRPOBJECTBUILDER::PROPERTIES::TEXTPROPERTIES
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			09/08/2016 17:15:32
//	
//  @param				rhs : 
//
*/
/*-----------------------------------------------------------------*/
UIGRPOBJECTBUILDER::TEXTPROPERTIES::TEXTPROPERTIES(const UIGRPOBJECTBUILDER::TEXTPROPERTIES& rhs)
{
	Clean();

	if(!string)
	{
		string = new XSTRING();
	}

	*this = rhs;
}

/*-------------------------------------------------------------------
//	UIGRPOBJECTBUILDER::PROPERTIES::~TEXTPROPERTIES
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			09/08/2016 17:15:32
//	
*/
/*-----------------------------------------------------------------*/
UIGRPOBJECTBUILDER::TEXTPROPERTIES::~TEXTPROPERTIES()
{
	delete string;

	Clean();
}



/*-------------------------------------------------------------------
//	UIGRPOBJECTBUILDER::TEXTPROPERTIES::operator=
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			21/10/2016 9:58:33
//	
//	@return 			UIGRPOBJECTBUILDER::TEXTPROPERTIES& : 
//
//  @param				rhs : 
*/
/*-----------------------------------------------------------------*/
UIGRPOBJECTBUILDER::TEXTPROPERTIES& UIGRPOBJECTBUILDER::TEXTPROPERTIES::operator=(const UIGRPOBJECTBUILDER::TEXTPROPERTIES& rhs)
{
	*string = *rhs.string;
	bold = rhs.bold;
	italic = rhs.italic;

	return *this;
}



/*-------------------------------------------------------------------
//	UIGRPOBJECTBUILDER::TEXTPROPERTIES::operator==
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			21/10/2016 9:58:43
//	
//	@return 			bool : 
//
//  @param				rhs : 
*/
/*-----------------------------------------------------------------*/
bool UIGRPOBJECTBUILDER::TEXTPROPERTIES::operator==(const UIGRPOBJECTBUILDER::TEXTPROPERTIES& rhs)
{
	return (*string == *rhs.string) && (bold == rhs.bold) && (italic == rhs.italic);
}



/*-------------------------------------------------------------------
//	UIGRPOBJECTBUILDER::TEXTPROPERTIES::operator!=
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			21/10/2016 9:58:50
//	
//	@return 			bool : 
//
//  @param				rhs : 
*/
/*-----------------------------------------------------------------*/
bool UIGRPOBJECTBUILDER::TEXTPROPERTIES::operator!=(const UIGRPOBJECTBUILDER::TEXTPROPERTIES& rhs)
{
	return !(*this == rhs);
}



/*-------------------------------------------------------------------
//	UIGRPOBJECTBUILDER::TEXTPROPERTIES::operator<
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			21/10/2016 9:58:57
//	
//	@return 			bool : 
//
//  @param				rhs : 
*/
/*-----------------------------------------------------------------*/
bool UIGRPOBJECTBUILDER::TEXTPROPERTIES::operator<(const UIGRPOBJECTBUILDER::TEXTPROPERTIES& rhs)
{
	return (*string < *rhs.string) && (bold < rhs.bold) && (italic < rhs.italic);
}



/*-------------------------------------------------------------------
//	UIGRPOBJECTBUILDER::TEXTPROPERTIES::operator<
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			21/10/2016 9:59:05
//	
//	@return 			bool : 
//
//  @param				rhs : 
*/
/*-----------------------------------------------------------------*/
bool UIGRPOBJECTBUILDER::TEXTPROPERTIES::operator<(UIGRPOBJECTBUILDER::TEXTPROPERTIES rhs)
{
	return (*string < *rhs.string) && (bold < rhs.bold) && (italic < rhs.italic);
}




/*-------------------------------------------------------------------
//	UIGRPOBJECTBUILDER::OBJECTENTRY::OBJECTENTRY
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			15/02/2017 10:20:17
//	
//	@return 			UIGRPOBJECTBUILDER::OBJECTENTRY::OBJECTENTRY(const : 
//
//  @param				rhs : 
*/
/*-----------------------------------------------------------------*/
UIGRPOBJECTBUILDER::OBJECTENTRY::OBJECTENTRY(const OBJECTENTRY& rhs)
{
	*this = rhs;
}



/*-------------------------------------------------------------------
//	UIGRPOBJECTBUILDER::OBJECTENTRY::operator=
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			15/02/2017 10:20:23
//	
//	@return 			UIGRPOBJECTBUILDER::OBJECTENTRY& : 
//
//  @param				rhs : 
*/
/*-----------------------------------------------------------------*/
UIGRPOBJECTBUILDER::OBJECTENTRY& UIGRPOBJECTBUILDER::OBJECTENTRY::operator=(const UIGRPOBJECTBUILDER::OBJECTENTRY& rhs)
{
	obj = rhs.obj;
	bb	= rhs.bb;

	return *this;
}



/*-------------------------------------------------------------------
//	UIGRPOBJECTBUILDER::OBJECTENTRY::operator==
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			15/02/2017 10:20:29
//	
//	@return 			bool : 
//
//  @param				rhs : 
*/
/*-----------------------------------------------------------------*/
bool UIGRPOBJECTBUILDER::OBJECTENTRY::operator==(const UIGRPOBJECTBUILDER::OBJECTENTRY& rhs)
{
	return (obj == rhs.obj) && (bb = rhs.bb);
}



/*-------------------------------------------------------------------
//	UIGRPOBJECTBUILDER::OBJECTENTRY::operator!=
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			15/02/2017 10:20:34
//	
//	@return 			bool : 
//
//  @param				rhs : 
*/
/*-----------------------------------------------------------------*/
bool UIGRPOBJECTBUILDER::OBJECTENTRY::operator!=(const UIGRPOBJECTBUILDER::OBJECTENTRY& rhs)
{
	return !(*this == rhs);
}



/*-------------------------------------------------------------------
//	UIGRPOBJECTBUILDER::OBJECTENTRY::operator<
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			15/02/2017 10:20:41
//	
//	@return 			bool : 
//
//  @param				rhs : 
*/
/*-----------------------------------------------------------------*/
bool UIGRPOBJECTBUILDER::OBJECTENTRY::operator<(const UIGRPOBJECTBUILDER::OBJECTENTRY& rhs)
{
	return obj < rhs.obj;
}



/*-------------------------------------------------------------------
//	UIGRPOBJECTBUILDER::OBJECTENTRY::operator<
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			15/02/2017 10:20:54
//	
//	@return 			bool : 
//
//  @param				rhs : 
*/
/*-----------------------------------------------------------------*/
bool UIGRPOBJECTBUILDER::OBJECTENTRY::operator<(UIGRPOBJECTBUILDER::OBJECTENTRY rhs)
{
	return obj < rhs.obj;
}



/*-------------------------------------------------------------------
//	UIGRPOBJECTBUILDER::UIGRPOBJECTBUILDER
*/	
/**	
//	
//	Class Constructor UIGRPOBJECTBUILDER
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			09/08/2016 17:31:13
//	
*/
/*-----------------------------------------------------------------*/
UIGRPOBJECTBUILDER::UIGRPOBJECTBUILDER(GRPSCENE* scene)
{
	Clean();

	this->scene = scene;
	this->objectloader = new UI3DOBJECTLOADER();

	// keep this material as a common one better to avoid memory growth
	material = new GRPMATERIAL();
	material->GetAttributes()->SetDiffuse(GRPCOLOR(0.8f, 0.0f, 0.8f, 1.0f));
	material->GetAttributes()->SetEmission(GRPCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	material->GetAttributes()->isupdated = true;
	GRPMATERIALLIBRARY::Get()->Add(material);
}



/*-------------------------------------------------------------------
//	UIGRPOBJECTBUILDER::~UIGRPOBJECTBUILDER
*/	
/**	
//	
//	 Class Destructor UIGRPOBJECTBUILDER
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			09/08/2016 17:31:18
//	
*/
/*-----------------------------------------------------------------*/
UIGRPOBJECTBUILDER::~UIGRPOBJECTBUILDER()
{
	// iterate all the elements and delete them(also the node, as no scene is controlling it)
	XBINARYMAP<PROPERTIES, GRPELEMENT*>::XITERATOR it;
	for(it = elements.Begin(); it != elements.End(); it++)
		{
			delete it.GetElement();
		}

	XBINARYMAP<TEXTPROPERTIES, GRPTEXTBOX*>::XITERATOR tit;
	for(tit = texts.Begin(); tit != texts.End(); tit++)
		{
			delete tit.GetElement();
		}

	XBINARYMAP<XSTRING, OBJECTENTRY*>::XITERATOR eit;
	for(eit = objects.Begin(); eit != objects.End(); eit++)
		{
			delete (*eit)->bb;
			delete (*eit)->obj;
			delete eit.GetElement();
		}

	delete objectloader;

	Clean();
}




/*-------------------------------------------------------------------
//	UIGRPOBJECTBUILDER::GetElement
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			09/08/2016 17:31:27
//	
//	@return 			GRPELEMENT* : 
//
//  @param				properties : 
*/
/*-----------------------------------------------------------------*/
GRPELEMENT* UIGRPOBJECTBUILDER::GetElement(PROPERTIES properties)
{
	GRPELEMENT* element =  elements.Get(properties);

	if(element == NULL)
		{
			// build the object

			// use a spline creator?

			if(properties.cornerradious == 0)
				{
					// normal square
					element = GRPOBJECTBUILDER::Get()->CreatePlane(GRPOBJECTBUILDER::AXIS_X);
					// assign any texture( have default texture for rendering?)
					element->SetNode(scene->CreateNode());
					element->GetNode()->Init();
					element->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"STX"));
					element->GetNode()->SetMaterial(material);
				}
			else
				{
				}

			elements.Add(properties, element);
		}

	return element;
}


GRPTEXTBOX* UIGRPOBJECTBUILDER::GetText(TEXTPROPERTIES properties)
{
	GRPTEXTBOX* textbox = NULL;

	if(properties.string->GetSize() == 0)
		{
			return NULL;
		}

	// try to find if the text has already been computed and take that one
	// if not generate it




	textbox = texts.Get(properties);

	if(!textbox)
		{
			// but missing what font to use!
			textbox = new GRPTEXTBOX(scene);
			textbox->SetNode(scene->CreateNode());
			textbox->SetMaterial(material);
			textbox->GetNode()->SetMaterial(material);

			GRPFONTMANAGER* fontmanager = GRPOBJECTBUILDER::Get()->GetFontManager();
				// need struct for text properties, as same text could be normal/bold/italic at the same time
			GRPFONT* font = fontmanager->GetFont(__L("UI"));
			if(properties.bold)
				{
					if(properties.italic)
						{
							font = fontmanager->GetFont(__L("UI-Bold-Italic"));
							textbox->SetFont(font);
						}
					else
						{
							font = fontmanager->GetFont(__L("UI-Bold"));
							textbox->SetFont(font);
						}
				}
			else if(properties.italic)
				{
					font = fontmanager->GetFont(__L("UI-Italic"));
					textbox->SetFont(font);
				}

			textbox->SetFont(font);
			textbox->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"SDF"));

			textbox->SetAlign(GRPTEXTBOX_ALIGNLEFT);
			textbox->SetContent(properties.string->Get());
			textbox->MergeElements();
			if(textbox->GetElements()->GetSize())
				{
					if(textbox->GetElements()->FastGet(0)->GetMesh())
						{
							textbox->GetElements()->FastGet(0)->UpdateBoundingBox();
						}
				}

			textbox->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"SDF")); // really necessary here?, shouldn't merge elements save previously used program

			texts.Add(properties, textbox);
		}

	return textbox;
}



/*-------------------------------------------------------------------
//	UIGRPOBJECTBUILDER::Get3DObject
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			14/02/2017 9:36:35
//	
//	@return 			GRPOBJECT* : 
//
//  @param				path : 
*/
/*-----------------------------------------------------------------*/
UIGRPOBJECTBUILDER::OBJECTENTRY* UIGRPOBJECTBUILDER::Get3DObject(XCHAR* path)
{
	if(path == NULL)
		{
			return NULL;
		}

	OBJECTENTRY* ret = NULL;

	XSTRING str;
	str.Set(path);

	ret = objects.Get(str);
	if(ret == NULL)
		{
			// load the object
			ret = new OBJECTENTRY();
			ret->obj = objectloader->Load(path);
			//// setup nodes as managed to avoid memory leaks
			//XDWORD nsize = ret->obj->GetNodes()->GetSize();
			//for(XDWORD i = 0; i < nsize; i++)
			//	{
			//		GRPNODE* n = ret->obj->GetNodes()->FastGet(i);
			//		if(n->managed == false)
			//			{
			//				scene->GetNodes()->Add(n);
			//				n->AddObserver(scene);
			//				n->managed = true;
			//			}
			//	}

			if(ret->obj)
				{
					GRPOBJECT* obj = new GRPOBJECT();
			
					// need to copy the nodes, but they don't have a clone!(position, scale and rotation for now)
					obj->SetNode(scene->CreateNode());
					const GRPVECTOR* pos = ret->obj->GetNode()->GetPosition();
					const GRPVECTOR* scale = ret->obj->GetNode()->GetScale();
					const GRPVECTOR* rotation = ret->obj->GetNode()->GetRotation();
					//const GRPVECTOR* geomscale = ret->obj->GetNode()->GetGeometricScale();
					obj->GetNode()->SetPosition(pos->vector[0], pos->vector[1], pos->vector[2]);
					obj->GetNode()->SetScale(scale->vector[0], scale->vector[1], scale->vector[2]);
					obj->GetNode()->SetRotation(rotation->vector[0], rotation->vector[1], rotation->vector[2]);
					//obj->GetNode()->SetGeometricScale(geomscale->vector[0], geomscale->vector[1], geomscale->vector[2]);
					//obj->GetNode()->UsesGeometricScale = true;

					//obj->GetNode()->Copy(ret->obj->GetNode());

					XDWORD size = ret->obj->GetElements()->GetSize();
			
					for(XDWORD i = 0; i < size; i++)
						{
							GRPELEMENT* e = ret->obj->GetElements()->FastGet(i);
							GRPELEMENT* n = new GRPELEMENT();
							if(!n) return NULL;
			
							n->Reference(e);
			
							n->SetNode(scene->CreateNode());
							//n->GetNode()->Copy(e->GetNode());
							const GRPVECTOR* pos = e->GetNode()->GetPosition();
							const GRPVECTOR* scale = e->GetNode()->GetScale();
							const GRPVECTOR* rotation = e->GetNode()->GetRotation();
							//const GRPVECTOR* geomscale = e->GetNode()->GetGeometricScale();
							n->GetNode()->SetPosition(pos->vector[0], pos->vector[1], pos->vector[2]);
							n->GetNode()->SetScale(scale->vector[0], scale->vector[1], scale->vector[2]);
							n->GetNode()->SetRotation(rotation->vector[0], rotation->vector[1], rotation->vector[2]);
							//n->GetNode()->SetGeometricScale(geomscale->vector[0], geomscale->vector[1], geomscale->vector[2]);
							//n->GetNode()->UsesGeometricScale = true;

							n->CollapseTransforms();
			
							obj->AddElement(n);
							n->GetNode()->SetParent(obj->GetNode()); // ugly and maybe not necessary?
						}
			
					obj->MergeElements();
					GRPELEMENT* element = obj->GetElements()->Get(0);
					if(!element) return NULL;
					element->UpdateBoundingBox();
					GRPBB* bb = new GRPBB(element->GetBoundingBox()); // no, we will need to compute it ourselves as it's wrong
					ret->bb = bb;

					delete obj;
			
					//ret->obj->MergeElements();

					objects.Add(str, ret);
				}
		}

	return ret;
}
