
/*------------------------------------------------------------------------------------------
//	GRPSERIALIZERGGF.CPP
//	
//	serializes/deserializes a ggf file
//   
//	Author						: Imanol Celaya Ruiz de Alegria
//	Date Of Creation	: 19/11/2015 10:23:49
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "GRPSerializerGGF.h"
#include "GRPFileGGF.h"	
#include "GRPBezierSpline.h"

#include "GRPNode.h"
#include "GRPNamed.h"
#include "GRPElement.h"
#include "GRPObject.h"
#include "GRPScene.h"
#include "GRPViewport.h"
#include "GRPCamera.h"

#include "GRPMaterialLibrary.h"
#include "GRPLight.h"
#include "GRPMaterial.h"
#include "GRPTexture.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::GetDefinition
*/	
/**	
//	
//	creates a new definition with the specified name
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			25/11/2015 11:54:32
//	
//	@return 			GGFOBJECTDEFINITION* : 
//
//  @param				definition : 
*/
/*-----------------------------------------------------------------*/
GGFOBJECTDEFINITION* GRPSERIALIZERGGF::GetDefinition(XCHAR* definition)
{
	XSTRING str(definition);
	return GetDefinition(&str);
}





/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::GetDefinition
*/	
/**	
//	
//	creates a new definition with the specified name
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			25/11/2015 11:54:32
//	
//	@return 			GGFOBJECTDEFINITION* : 
//
//  @param				definition : 
*/
/*-----------------------------------------------------------------*/
GGFOBJECTDEFINITION* GRPSERIALIZERGGF::GetDefinition(XSTRING* definition)
{
	GGFOBJECTDEFINITION* ret = new GGFOBJECTDEFINITION();
	if(!ret)
		{
			return NULL;
		}

	ret->id.FromXSTRING(definition);

	return ret;
}



/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::WriteString
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/12/2015 12:04:08
//	
//  @param				object : 
//  @param				attribute : 
//  @param				string : 
*/
/*-----------------------------------------------------------------*/
void GRPSERIALIZERGGF::WriteString(GGFOBJECT* object, XCHAR* attribute, XCHAR* string)
{
	XSTRING		str;
	GGFVALUE	value;

	GGFSTRING name( string);

	GGFDATA* data = new GGFDATA();

	data->buffer.Resize(name.Size());
	name.WriteTo(data->buffer);
	
	XDWORD index = file->AddData(data);

	value.SetValue(index);
	str.Set(attribute);
	object->Add(&str, &value);
}





/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::WriteString
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/12/2015 12:04:13
//	
//  @param				object : 
//  @param				attribute : 
//  @param				string : 
*/
/*-----------------------------------------------------------------*/
void GRPSERIALIZERGGF::WriteString(GGFOBJECT* object, XCHAR* attribute, GGFSTRING* string)
{
	XSTRING		str;
	GGFVALUE	value;

	GGFSTRING name( string);

	GGFDATA*	data = new GGFDATA();

	data->buffer.Resize(name.Size());
	name.WriteTo(data->buffer);
	
	XDWORD index = file->AddData(data);

	value.SetValue(index);
	str.Set(attribute);
	object->Add(&str, &value);
}




/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::ReadString
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/12/2015 12:10:22
//	
//  @param				object : 
//  @param				attribute : 
//  @param				string : 
*/
/*-----------------------------------------------------------------*/
void GRPSERIALIZERGGF::ReadString(GGFOBJECT* object, XCHAR* attribute, XSTRING* string)
{
	XDWORD	 index	= object->Get(attribute)->GETXDWORD();
	GGFDATA* data		= file->data.Get(index);

	GGFSTRING ggfstring;

	ggfstring.ReadFrom(data->buffer);
	ggfstring.ToXSTRING(string);
}




/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::ReadString
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/12/2015 12:10:17
//	
//  @param				object : 
//  @param				attribute : 
//  @param				string : 
*/
/*-----------------------------------------------------------------*/
void GRPSERIALIZERGGF::ReadString(GGFOBJECT* object, XCHAR* attribute, GGFSTRING* string)
{
	XDWORD index = object->Get(attribute)->GETXDWORD();
	GGFDATA* data = file->data.Get(index);
	string->ReadFrom(data->buffer);
}



/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::GetFilePathTexture
*/	
/**	
//	
//	finds the path of a texture
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			30/11/2015 17:18:54
//	
//	@return 			XPATH : 
//
//  @param				file : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::GetFilePathTexture(XCHAR* file, XPATH& path)
{
	XPATH	FinalResourceFile;
	XPATH filename;

	filename.Set(file);
	filename.DeleteCharacter(__C('"'));		

	XFILE* xfilecheck=xfactory->Create_File();
	if (xfilecheck==NULL) return false;

	if (!xfilecheck->Exist(filename)) //------------ check if full path exists
		{
			XPATH PathAndFileOnResource;

			filename.SetOnlyNamefileExt();

			PathAndFileOnResource=GRPMATERIALLIBRARY::Get()->GetResourceLocation()->Get();
			PathAndFileOnResource.Slash_Add();
			PathAndFileOnResource+=filename;

			if (!xfilecheck->Exist(PathAndFileOnResource))//------------ check if full file on resource location exists
				{
					XDEBUG_PRINTCOLOR(4,__L("GRPFILE3DFBX::LoadTexture : Error Texture not found on location or default material path %s"),filename.Get());
					XDEBUG_PRINTCOLOR(4,__L("\t\t\t Searched: %s"),file);
					XDEBUG_PRINTCOLOR(4,__L("\t\t\t Searched: %s"),PathAndFileOnResource.Get());
					xfactory->Delete_File(xfilecheck);
					return false;
				}
				else
				{
					FinalResourceFile=PathAndFileOnResource;
				}
		}
	else
		{
			FinalResourceFile=filename;
		}

	xfactory->Delete_File(xfilecheck);

	path = FinalResourceFile;
	return true;
}


/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::GetLoadedObject
*/	
/**	
//	
//	gets the pointer to an object from it's tag
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			26/11/2015 12:34:16
//	
//	@return 			void* : 
//
//  @param				tag : 
*/
/*-----------------------------------------------------------------*/
void* GRPSERIALIZERGGF::GetLoadedObject(XQWORD tag)
{
	//XQWORD index = file->seekmap.Find(tag);
	XQWORD index = file->seekmap.seekmaptree.Get(tag);
	if(index != NOTFOUND)
		{
			//index = file->seekmap.GetIndex((XDWORD)index);
			//index = file->seekmap.seekmap.elements.FastGet((XDWORD)index);

			return loadedelements.Get(index);

			/*index = loadedelements.Find(index);
			if(index != NOTFOUND)
				{
					return loadedelements.elements.FastGet((int)index);
					//return loadedelements.GetElement((int)index);
				}*/
		}

	return NULL;
}


/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::RegisterDefinition
*/	
/**	
//	
//	registers a definition
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			24/11/2015 12:30:53
//	
//  @param				name : 
//  @param				size : 
//  @param				definition : 
 
*/
/*-----------------------------------------------------------------*/
void GRPSERIALIZERGGF::RegisterDefinition(XCHAR* name, XDWORD size, GGFOBJECTDEFINITION* definition)
{
	XSTRING str;
	GGFDEFINITIONENTRY* entry = new GGFDEFINITIONENTRY();
	str.Set(name);
	entry->id.FromXSTRING(&str);
	entry->size = size;
	//definition->AddEntry(entry);
	// need to use the definition map in here
	definition->LoadEntry(entry);
}



/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::DefineGRPOBJECT
*/	
/**	
//	
//	defines an object
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			24/11/2015 12:31:16
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::DefineGRPOBJECT(GGFOBJECTDEFINITION* definition)
{
	GGFOBJECTDEFINITION* element = new GGFOBJECTDEFINITION();
	if(!element)
		{
			return false;
		}

	if(!DefineGRPELEMENT(element)) return false;
	file->AddDefinition(element);

	DefineGRPNAMED(definition);

	RegisterDefinition(__L("node"), sizeof(void*), definition);

	// list of elements
	RegisterDefinition(__L("elements"), sizeof(void*), definition);

	RegisterDefinition(__L("IsProgram"), sizeof(XBYTE), definition);
	RegisterDefinition(__L("Program"), 3*sizeof(char), definition);
	RegisterDefinition(__L("material"), sizeof(void*), definition);
	RegisterDefinition(__L("Animation"), sizeof(void*), definition);

	return true;
}



/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::WriteGRPOBJECT
*/	
/**	
//	
//	writes a grpobject
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			26/11/2015 14:18:57
//	
//	@return 			bool : 
//
//  @param				obj : 
//  @param				object : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::WriteGRPOBJECT(GRPOBJECT& obj, GGFOBJECT* object)
{
	WriteGRPNAMED(obj, object);

	XSTRING str;

	GGFVALUE value;

	// for every element
	for(XDWORD i = 0; i < obj.GetElements()->GetSize(); i++)
		{
			value.SetValue(obj.GetElements()->FastGet(i));
			str.Set(__L("elements"));
			object->Add(&str, &value);

			XVECTOR<GGFOBJECTDEFINITION*>* definitions = file->GetDefinitions();
			GGFOBJECTDEFINITION* definition = NULL;
			for(XDWORD j = 0; j < definitions->GetSize(); j++)
			{
				if(definitions->FastGet(j)->id == __L("GRPELEMENT"))
				{
					definition = definitions->FastGet(j);
					break;
				}
			}

			// write the elements in here
			GGFOBJECT* element = new GGFOBJECT( file->info, NULL);
			XSTRING str;
			str.Set(__L("GRPELEMENT"));
			element->id.FromXSTRING(&str);
			element->tag = (XQWORD)obj.GetElements()->FastGet(i);
			element->definition = definition;

			WriteGRPELEMENT(*obj.GetElements()->FastGet(i), element);
			if(!file->AddObject(element))
				{
					XDEBUG_PRINTCOLOR(4,__L("GGF: couldn't add %s"), __L("GRPELEMENT"));
				}
		}

	value.SetBOOL(obj.GetProgram() != NULL);
	object->Add(__L("IsProgram"), &value);

	if(obj.GetProgram())
		{
			value.SetCharString(obj.GetProgram()->name, 3);
			object->Add(__L("Program"), &value);
		}

	value.SetValue(obj.GetMaterial());
	str.Set(__L("material"));
	object->Add(&str, &value);

	value.SetValue(obj.GetAnimation());
	str.Set(__L("Animation"));
	object->Add(&str, &value);

	value.SetValue(obj.GetNode());
	str.Set(__L("node"));
	object->Add(&str, &value);
	
	return true;
}




/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::ReadGRPOBJECT
*/	
/**	
//	
//	reads an object
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			27/11/2015 13:41:31
//	
//	@return 			bool : 
//
//  @param				obj : 
//  @param				object : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::ReadGRPOBJECT(GRPOBJECT& obj, GGFOBJECT* object)
{
	ReadGRPNAMED(obj, object);

	XDWORD attributes = object->indexedvalues.GetSize();//object->values.GetSize();

	bool isprogram = object->Get(__L("IsProgram"))->GetBOOL();

	if(isprogram)
		{
			char key[3];
			object->Get(__L("Program"))->GetCharString(key, 3);
			obj.SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey(key));
		}

	XVECTOR<GRPELEMENT*> elements;

	XLIST<GGFVALUE*> elementslist;
	object->GetMultiple(__L("elements"), elementslist);

	// ELEMENTS

	XLISTDATA<GGFVALUE*>* it = elementslist.GetFirstData();
	while(!IsLast(it))
	{
		// read element into elements vector
		XQWORD ptr = 0;
		// get the ptr from specific element
		
		it->data->buffer.Get((XBYTE*)&ptr, file->info.ptrsize, 0);

		GRPELEMENT* element = (GRPELEMENT*)GetLoadedObject(ptr);

		if((ptr != 0) && (element == NULL))
			{
				// read element
				//XQWORD index = file->seekmap.Find(ptr);
				XQWORD index = file->seekmap.seekmaptree.Get(ptr);
				if(index != NOTFOUND)
					{
						GGFOBJECT* obj = file->objects.Get((XDWORD)index);
						GRPELEMENT* e = new GRPELEMENT();
						ReadGRPELEMENT(*e, obj);
				
						elements.Add(e);
						loadedelements.Add(index, e);
					}
				else
					{
						XDEBUG_PRINTCOLOR(4,__L("ERROR: looking for an object not present in seekmap"));
					}
			}
		else
			{
				elements.Add(element);
			}


		it = Increment(it);
	}

	// ELEMENTS END

	
	// NODE
	GRPNODE* node = NULL;
	XQWORD ptr = 0;
	object->Get(__L("node"))->buffer.Get((XBYTE*)&ptr, file->info.ptrsize, 0);
	node = (GRPNODE*)GetLoadedObject(ptr);

	// load the node form ggf if not in memory
	if((ptr != 0) && (node == NULL))
		{
			//XQWORD index = file->seekmap.Find(ptr);
			XQWORD index = file->seekmap.seekmaptree.Get(ptr);
			if(index == NOTFOUND) return false;

			GGFOBJECT* ggfobj = file->objects.Get((XDWORD)index);
			node = new GRPNODE();
			if(!node) return false;
			ReadGRPNODE(*node, ggfobj);

			if(!(ggfobj->id == __L("GRPNODE")))
				{
					XDEBUG_PRINTCOLOR(4,__L("ERROR: Not a real GRPNODe"));
				}

			obj.SetNode(node);
			loadedelements.Add(index, node);
			scene->Add(node);
		}
	else
		{
			obj.SetNode(node);
		}


	// MaTERIAL
		ptr = 0;
		object->Get(__L("material"))->buffer.Get((XBYTE*)&ptr, file->info.ptrsize, 0);

		GRPMATERIAL* parent = (GRPMATERIAL*)GetLoadedObject(ptr);

		if((ptr != 0) && (parent == NULL))
			{
				// parent recursive read
				//XQWORD index = file->seekmap.Find(ptr);
				XQWORD index = file->seekmap.seekmaptree.Get(ptr);
				if(index != NOTFOUND)
					{
						GRPMATERIAL * material = new GRPMATERIAL();
						if(!material) return false;

						ReadGRPMATERIAL(*material, file->objects.FastGet((int)index));

						GRPMATERIALLIBRARY::Get()->Add(material);
						XQWORD index2 = file->seekmap.Get(file->objects.FastGet((int)index)->tag);
						loadedelements.Add(index2, material);
						obj.SetMaterial(material);
					}
				else
					{
						XDEBUG_PRINTCOLOR(4,__L("ERROR: looking for an object not present in seekmap"));
					}
			}
		else
			{
				obj.SetMaterial(parent);
			}


	//add all elements into the object after reading it completely
	for(XDWORD i = 0; i < elements.GetSize(); i++)
		{
			obj.AddElement(elements.FastGet(i));
		}

	return true;
}


GRPNODE* GRPSERIALIZERGGF::ReadGRPNODEINHERITANCE(GGFOBJECT* object)
{
	GRPNODE* ret = NULL;

	if(object->id == __L("GRPCAMERA"))
		{
			GRPCAMERA* camera = new GRPCAMERA();
			if(!camera) return NULL;
			ret = camera;

			ReadGRPCAMERA(*camera, object);

			XQWORD index = file->seekmap.Get(object->tag);
			loadedelements.Add(index, camera);
			scene->Add(camera);
			scene->Add((GRPNODE*)camera); // needed as the camera is a node and needs to be handled by the scene
			scene->SetCurrentCamera(scene->GetCameras()->Find(camera));
		}
	else if(object->id == __L("GRPNODE"))
		{
			GRPNODE* node = new GRPNODE();
			if(!node) return NULL;
			ret = node;

			ReadGRPNODE(*node, object);

			XQWORD index = file->seekmap.Get(object->tag);
			loadedelements.Add(index, node);
			scene->Add(node);
		}
	else if(object->id == __L("GRPLIGHT"))
		{
			GRPLIGHT* light = new GRPLIGHT();
			if(!light) return NULL;
			ret = light;
			
			ReadGRPLIGHT(*light, object);
					
			XQWORD index = file->seekmap.Get(object->tag);
			loadedelements.Add(index, light);
			scene->Add(light);
			scene->Add((GRPNODE*)light);
		}

	return ret;
}


/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::DefineGRPNODE
*/	
/**	
//	
//	defines a node
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			24/11/2015 12:31:05
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::DefineGRPNODE(GGFOBJECTDEFINITION* definition)
{
	DefineGRPINTERFACEANIMABLE(definition);
	DefineGRPATTACHEABLE(definition);
	DefineGRPUSERDATACONTAINER(definition);

	// First test, add the node definition
	RegisterDefinition(__L("GRPNODE.NodeMode"), sizeof(GRPNODE_MODE), definition);
	RegisterDefinition(__L("GRPNODE.Value"), sizeof(double), definition);
	RegisterDefinition(__L("GRPNODE.managed"), sizeof(XBYTE), definition);
	RegisterDefinition(__L("GRPNODE.tag"), sizeof(XQWORD), definition);
	
	RegisterDefinition(__L("GRPNODE.UsesGeometricTranslation"), sizeof(XBYTE), definition);
	RegisterDefinition(__L("GRPNODE.UsesGeometricScale"), sizeof(XBYTE), definition);
	RegisterDefinition(__L("GRPNODE.UsesGeometricRotation"), sizeof(XBYTE), definition);
	
	RegisterDefinition(__L("GRPNODE.UsesRotationOffset"), sizeof(XBYTE), definition);
	RegisterDefinition(__L("GRPNODE.UsesRotationPivot"), sizeof(XBYTE), definition);
	RegisterDefinition(__L("GRPNODE.UsesScalingOffset"), sizeof(XBYTE), definition);
	RegisterDefinition(__L("GRPNODE.UsesScalingPivot"), sizeof(XBYTE), definition);
	RegisterDefinition(__L("GRPNODE.UsesPreRotation"), sizeof(XBYTE), definition);
	RegisterDefinition(__L("GRPNODE.UsesPostRotation"), sizeof(XBYTE), definition);
	
	RegisterDefinition(__L("GRPNODE.visibility"), sizeof(double), definition);
	RegisterDefinition(__L("GRPNODE.inherited_visibility"), sizeof(double), definition);
	
	RegisterDefinition(__L("GRPNODE.mustinheritvisibility"), sizeof(XBYTE), definition);
	RegisterDefinition(__L("GRPNODE.rendereable"), sizeof(XBYTE), definition);
	
	RegisterDefinition(__L("GRPNODE.material"), sizeof(void*), definition);
	
	DefineGRPMATERIALATTRIBUTES(definition);
	
	RegisterDefinition(__L("GRPNODE.degrees"), sizeof(float), definition);
	
	RegisterDefinition(__L("GRPNODE.RotationOffset"), 3*sizeof(float), definition);
	RegisterDefinition(__L("GRPNODE.RotationPivot"), 3*sizeof(float), definition);
	RegisterDefinition(__L("GRPNODE.ScalingOffset"), 3*sizeof(float), definition);
	RegisterDefinition(__L("GRPNODE.ScalingPivot"), 3*sizeof(float), definition);
	
	RegisterDefinition(__L("GRPNODE.prerotation"), 3*sizeof(float), definition);
	RegisterDefinition(__L("GRPNODE.postrotation"), 3*sizeof(float), definition);
	
	RegisterDefinition(__L("GRPNODE.InheritType"), sizeof(int), definition);
	
	RegisterDefinition(__L("GRPNODE.position"), 3*sizeof(float), definition);
	RegisterDefinition(__L("GRPNODE.rotation"), 3*sizeof(float), definition);
	RegisterDefinition(__L("GRPNODE.scale"), 3*sizeof(float), definition);
	
	RegisterDefinition(__L("GRPNODE.geometricposition"), 3*sizeof(float), definition);
	RegisterDefinition(__L("GRPNODE.geometricrotation"), 3*sizeof(float), definition);
	RegisterDefinition(__L("GRPNODE.geometricscale"), 3*sizeof(float), definition);

	return true;
}



/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::WriteGRPNODE
*/	
/**	
//	
//	writes a grpnode
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			25/11/2015 12:00:28
//	
//	@return 			bool : 
//
//  @param				node : 
//  @param				object : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::WriteGRPNODE(GRPNODE& node, GGFOBJECT* object)
{
	WriteGRPINTERFACEANIMABLE(node, object);
	WriteGRPATTACHEABLE(node, object);
	WriteGRPUSERDATACONTAINER(*node.GetUserData(), object);

	GGFVALUE value;
	value.SetValue(node.NodeMode);
	object->Add(__L("GRPNODE.NodeMode"), &value);
	
	value.SetValue(node.Value);
	object->Add(__L("GRPNODE.Value"), &value);
	
	value.SetValue(node.managed);
	object->Add(__L("GRPNODE.managed"), &value);
	
	value.SetValue(node.tag);
	object->Add(__L("GRPNODE.tag"), &value);
	
	value.SetValue(node.UsesGeometricTranslation);
	object->Add(__L("GRPNODE.UsesGeometricTranslation"), &value);
	
	value.SetValue(node.UsesGeometricScale);
	object->Add(__L("GRPNODE.UsesGeometricScale"), &value);
	
	value.SetValue(node.UsesGeometricRotation);
	object->Add(__L("GRPNODE.UsesGeometricRotation"), &value);
	
	value.SetValue(node.UsesRotationOffset);
	object->Add(__L("GRPNODE.UsesRotationOffset"), &value);
	
	value.SetValue(node.UsesRotationPivot);
	object->Add(__L("GRPNODE.UsesRotationPivot"), &value);
	
	value.SetValue(node.UsesScalingOffset);
	object->Add(__L("GRPNODE.UsesScalingOffset"), &value);
	
	value.SetValue(node.UsesScalingPivot);
	object->Add(__L("GRPNODE.UsesScalingPivot"), &value);
	
	value.SetValue(node.UsesPreRotation);
	object->Add(__L("GRPNODE.UsesPreRotation"), &value);
	
	value.SetValue(node.UsesPostRotation);
	object->Add(__L("GRPNODE.UsesPostRotation"), &value);
	
	value.SetValue(node.visibility);
	object->Add(__L("GRPNODE.visibility"), &value);
	
	value.SetValue(node.inherited_visibility);
	object->Add(__L("GRPNODE.inherited_visibility"), &value);
	
	value.SetValue(node.mustinheritvisibility);
	object->Add(__L("GRPNODE.mustinheritvisibility"), &value);
	
	value.SetValue(node.rendereable);
	object->Add(__L("GRPNODE.rendereable"), &value);
	
	value.SetValue(node.material);
	object->Add(__L("GRPNODE.material"), &value);
	// maybe also write textures in here?


	WriteGRPMATERIALATTRIBUTES(node.BaseMaterial, object);
	
	value.SetValue(node.degrees);
	object->Add(__L("GRPNODE.degrees"), &value);
	
	value.SetGRPVECTOR(node.RotationOffset);
	object->Add(__L("GRPNODE.RotationOffset"), &value);
	
	value.SetGRPVECTOR(node.RotationPivot);
	object->Add(__L("GRPNODE.RotationPivot"), &value);
	
	value.SetGRPVECTOR(node.ScalingOffset);
	object->Add(__L("GRPNODE.ScalingOffset"), &value);
	
	value.SetGRPVECTOR(node.ScalingPivot);
	object->Add(__L("GRPNODE.ScalingPivot"), &value);
	
	value.SetGRPVECTOR(node.prerotation);
	object->Add(__L("GRPNODE.prerotation"), &value);
	
	value.SetGRPVECTOR(node.postrotation);
	object->Add(__L("GRPNODE.postrotation"), &value);
	
	value.SetValue(node.InheritType);
	object->Add(__L("GRPNODE.InheritType"), &value);
	
	value.SetGRPVECTOR(*node.GetPosition());
	object->Add(__L("GRPNODE.position"), &value);
	
	value.SetGRPVECTOR(*node.GetRotation());
	object->Add(__L("GRPNODE.rotation"), &value);
	
	value.SetGRPVECTOR(*node.GetScale());
	object->Add(__L("GRPNODE.scale"), &value);
	
	value.SetGRPVECTOR(*node.GetGeometricPosition());
	object->Add(__L("GRPNODE.geometricposition"), &value);
	
	value.SetGRPVECTOR(*node.GetGeometricRotation());
	object->Add(__L("GRPNODE.geometricrotation"), &value);
	
	value.SetGRPVECTOR(*node.GetGeometricScale());
	object->Add(__L("GRPNODE.geometricscale"), &value);

	return true;
}




/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::ReadGRPNODE
*/	
/**	
//	
//	reads a node
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			26/11/2015 13:28:17
//	
//	@return 			bool : 
//
//  @param				node : 
//  @param				object : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::ReadGRPNODE(GRPNODE& node, GGFOBJECT* object)
{
	ReadGRPNAMED(node, object);
	ReadGRPATTACHEABLE(node, object);
	ReadGRPUSERDATACONTAINER(*node.GetUserData(), object);

	node.NodeMode									= object->Get(__L("GRPNODE.NodeMode"))->GetValue<GRPNODE_MODE>();

	node.Value										= object->Get(__L("GRPNODE.Value"))->Getdouble();
	node.managed									= object->Get(__L("GRPNODE.managed"))->GetBOOL();

	node.tag											= object->Get(__L("GRPNODE.tag"))->GETXQWORD();

	node.managed									= object->Get(__L("GRPNODE.managed"))->GetBOOL();

	node.UsesGeometricTranslation = object->Get(__L("GRPNODE.UsesGeometricTranslation"))->GetBOOL();
	node.UsesGeometricScale				= object->Get(__L("GRPNODE.UsesGeometricScale"))->GetBOOL();
	node.UsesGeometricRotation		= object->Get(__L("GRPNODE.UsesGeometricRotation"))->GetBOOL();

	node.UsesRotationOffset				= object->Get(__L("GRPNODE.UsesRotationOffset"))->GetBOOL();
	node.UsesRotationPivot				= object->Get(__L("GRPNODE.UsesRotationPivot"))->GetBOOL();
	node.UsesScalingOffset				= object->Get(__L("GRPNODE.UsesScalingOffset"))->GetBOOL();
	node.UsesScalingPivot					= object->Get(__L("GRPNODE.UsesScalingPivot"))->GetBOOL();
	node.UsesPreRotation					= object->Get(__L("GRPNODE.UsesPreRotation"))->GetBOOL();
	node.UsesPostRotation					= object->Get(__L("GRPNODE.UsesPostRotation"))->GetBOOL();

	node.visibility								= object->Get(__L("GRPNODE.visibility"))->Getdouble();
	node.inherited_visibility			= object->Get(__L("GRPNODE.inherited_visibility"))->Getdouble();

	node.mustinheritvisibility		= object->Get(__L("GRPNODE.mustinheritvisibility"))->GetBOOL();
	node.rendereable							= object->Get(__L("GRPNODE.rendereable"))->GetBOOL();

	// read material, and material attributes
	node.material = NULL;

	XQWORD ptr = 0;
	object->Get(__L("GRPNODE.material"))->buffer.Get((XBYTE*)&ptr, file->info.ptrsize, 0);

	GRPMATERIAL* parent = (GRPMATERIAL*)GetLoadedObject(ptr);

	if((ptr != 0) && (parent == NULL))
		{
			// parent recursive read
			//XQWORD index = file->seekmap.Find(ptr);
			XQWORD index = file->seekmap.seekmaptree.Get(ptr);
			if(index != NOTFOUND)
				{
					GRPMATERIAL * material = new GRPMATERIAL();
					if(!material) return false;

					ReadGRPMATERIAL(*material, file->objects.FastGet((int)index));

					GRPMATERIALLIBRARY::Get()->Add(material);
					XQWORD index2 = file->seekmap.Get(file->objects.FastGet((int)index)->tag);
					loadedelements.Add(index2, material);
					node.material = material;
				}
			else
				{
					XDEBUG_PRINTCOLOR(4,__L("ERROR: looking for an object not present in seekmap"));
				}
		}
	else
		{
			node.material = parent;
		}

	

	ReadGRPMATERIALATTRIBUTES(node.BaseMaterial, object);


	node.degrees									= object->Get(__L("GRPNODE.degrees"))->Getfloat();

	node.RotationOffset						= object->Get(__L("GRPNODE.RotationOffset"))->GetGRPVECTOR();
	node.RotationPivot						= object->Get(__L("GRPNODE.RotationPivot"))->GetGRPVECTOR();
	node.ScalingOffset						= object->Get(__L("GRPNODE.ScalingOffset"))->GetGRPVECTOR();
	node.ScalingPivot							= object->Get(__L("GRPNODE.ScalingPivot"))->GetGRPVECTOR();

	node.prerotation							= object->Get(__L("GRPNODE.prerotation"))->GetGRPVECTOR();
	node.postrotation							= object->Get(__L("GRPNODE.postrotation"))->GetGRPVECTOR();

	node.InheritType							= object->Get(__L("GRPNODE.InheritType"))->Getint();


	node.SetPosition(object->Get(__L("GRPNODE.position"))->GetGRPVECTOR().vector[0],
									 object->Get(__L("GRPNODE.position"))->GetGRPVECTOR().vector[1],
									 object->Get(__L("GRPNODE.position"))->GetGRPVECTOR().vector[2]);

	node.SetRotation(object->Get(__L("GRPNODE.rotation"))->GetGRPVECTOR().vector[0],
									 object->Get(__L("GRPNODE.rotation"))->GetGRPVECTOR().vector[1],
									 object->Get(__L("GRPNODE.rotation"))->GetGRPVECTOR().vector[2]);

	node.SetScale(object->Get(__L("GRPNODE.scale"))->GetGRPVECTOR().vector[0],
								object->Get(__L("GRPNODE.scale"))->GetGRPVECTOR().vector[1],
								object->Get(__L("GRPNODE.scale"))->GetGRPVECTOR().vector[2]);

	node.SetGeometricPosition(object->Get(__L("GRPNODE.geometricposition"))->GetGRPVECTOR().vector[0],
														object->Get(__L("GRPNODE.geometricposition"))->GetGRPVECTOR().vector[1],
														object->Get(__L("GRPNODE.geometricposition"))->GetGRPVECTOR().vector[2]);

	node.SetGeometricRotation(object->Get(__L("GRPNODE.geometricrotation"))->GetGRPVECTOR().vector[0],
														object->Get(__L("GRPNODE.geometricrotation"))->GetGRPVECTOR().vector[1],
						 	  						object->Get(__L("GRPNODE.geometricrotation"))->GetGRPVECTOR().vector[2]);

	node.SetGeometricScale(object->Get(__L("GRPNODE.geometricscale"))->GetGRPVECTOR().vector[0],
												 object->Get(__L("GRPNODE.geometricscale"))->GetGRPVECTOR().vector[1],
												 object->Get(__L("GRPNODE.geometricscale"))->GetGRPVECTOR().vector[2]);

	return true;
}

/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::WriteNodes
*/	
/**	
//	
//	writes all the nodes in a scene
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			25/11/2015 12:00:39
//	
//	@return 			bool : 
//
//  @param				scene : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::WriteNodes(GRPSCENE* scene)
{
	XVECTOR<GRPNODE*>* nodes = scene->GetNodes();
	XDWORD nodesize = nodes->GetSize();

	XDWORD skipped = 0;
	XDWORD total = 0;


	// search the definition of this object and assign it to the object
	XVECTOR<GGFOBJECTDEFINITION*>* definitions = file->GetDefinitions();
	GGFOBJECTDEFINITION* definition = NULL;
	for(XDWORD i = 0; i < definitions->GetSize(); i++)
	{
		if(definitions->FastGet(i)->id == __L("GRPNODE"))
		{
			definition = definitions->FastGet(i);
			break;
		}
	}

	for(XDWORD i = 0; i < nodesize; i++)
		{
			GRPNODE* node = nodes->FastGet(i);

			if(nodes->FastGet(i)->classtype != GRPNAMED_TYPE_GRPNODE)
				{
					if((scene->GetCameras()->Find((GRPCAMERA*)nodes->FastGet(i)) != NOTFOUND)
					|| (scene->GetLights()->Find((GRPLIGHT*)nodes->FastGet(i)) != NOTFOUND))
					{
						skipped++;
						continue;
					}
				}

			GGFOBJECT* obj = new GGFOBJECT( file->info, NULL);
			if(!obj)
				{
					return false;
				}

			XSTRING str;
			str.Set(__L("GRPNODE"));
			obj->id.FromXSTRING(&str);
			obj->tag = (XQWORD)nodes->FastGet(i);
			obj->definition = definition;

			WriteGRPNODE(*nodes->FastGet(i), obj);

			if(!file->AddObject(obj))
				{
					XDEBUG_PRINTCOLOR(4,__L("GGF: couldn't add %s"), __L("GRPNODE"));
				}
			else
			{
				total++;
			}

		}

	XDEBUG_PRINTCOLOR(2,__L("GGF: Added nodes %d"), total);
	XDEBUG_PRINTCOLOR(2,__L("GGF: Skipped nodes %d"), skipped);

	return true;
}




/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::Write
*/	
/**	
//	
//	writes a scene
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			25/11/2015 15:47:27
//	
//	@return 			bool : 
//
//  @param				scene : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::Write(GRPSCENE* scene)
{
	this->scene = scene;

	GGFOBJECTDEFINITION* definition = GetDefinition(__L("GRPNODE"));
	if(!definition) return false;
	DefineGRPNODE(definition);
	file->AddDefinition(definition);

	definition = GetDefinition(__L("GRPOBJECT"));
	if(!definition) return false;
	DefineGRPOBJECT(definition);
	file->AddDefinition(definition);

	definition = GetDefinition(__L("GRPVIEWPORT"));
	if(!definition) return false;
	DefineGRPVIEWPORT(definition);
	file->AddDefinition(definition);

	definition = GetDefinition(__L("GRPCAMERA"));
	if(!definition) return false;
	DefineGRPCAMERA(definition);
	file->AddDefinition(definition);

	definition = GetDefinition(__L("GRPELEMENT"));
	if(!definition) return false;
	DefineGRPELEMENT(definition);
	file->AddDefinition(definition);

	definition = GetDefinition(__L("GRPLIGHT"));
	if(!definition) return false;
	DefineGRPLIGHT(definition);
	file->AddDefinition(definition);

	definition = GetDefinition(__L("GRPMATERIAL"));
	if(!definition) return false;
	DefineGRPMATERIAL(definition);
	file->AddDefinition(definition);

	definition = GetDefinition(__L("GRPTEXTURE"));
	if(!definition) return false;
	DefineGRPTEXTURE(definition);
	file->AddDefinition(definition);

	definition = GetDefinition(__L("GRPANIMATIONCONTROL"));
	if(!definition) return false;
	DefineGRPANIMATIONCONTROL(definition);
	file->AddDefinition(definition);

	definition = GetDefinition(__L("GRPANIMATIONSTACK"));
	if(!definition) return false;
	DefineGRPANIMATIONSTACK(definition);
	file->AddDefinition(definition);

	definition = GetDefinition(__L("GRPANIMATIONLAYER"));
	if(!definition) return false;
	DefineGRPANIMATIONLAYER(definition);
	file->AddDefinition(definition);

	definition = GetDefinition(__L("GRPANIMATIONNODE"));
	if(!definition) return false;
	DefineGRPANIMATIONNODE(definition);
	file->AddDefinition(definition);

	definition = GetDefinition(__L("GRPANIMATIONCURVE"));
	if(!definition) return false;
	DefineGRPANIMATIONCURVE(definition);
	file->AddDefinition(definition);

	definition = GetDefinition(__L("GRPBEZIERSPLINE"));
	if(!definition) return false;
	DefineGRPBEZIERSPLINE(definition);
	file->AddDefinition(definition);

	// Animation Control
	GGFOBJECT* obj = new GGFOBJECT( file->info, NULL);
	if(!obj)
		{
			return false;
		}

	XSTRING str;
	str.Set(__L("GRPANIMATIONCONTROL"));
	obj->id.FromXSTRING(&str);
	obj->tag = (XQWORD)(scene->GetAnimationControl());

	// search the definition of this object and assign it to the object
	XVECTOR<GGFOBJECTDEFINITION*>* definitions = file->GetDefinitions();
	for(XDWORD i = 0; i < definitions->GetSize(); i++)
	{
		if(definitions->FastGet(i)->id == __L("GRPANIMATIONCONTROL"))
		{
			obj->definition = definitions->FastGet(i);
			break;
		}
	}

	WriteGRPANIMATIONCONTROL(*scene->GetAnimationControl(), obj);

	file->AddObject(obj);


	WriteNodes(scene);
	WriteObjects(scene);
	WriteViewPorts(scene);
	WriteCameras(scene);
	WriteLights(scene);
	WriteMaterials(scene);
	WriteTextures(scene);
	WriteAnimationStacks(scene);

	return true;
}




/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::Read
*/	
/**	
//	
//	reads a scene
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			25/11/2015 15:47:37
//	
//	@return 			bool : 
//
//  @param				scene : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::Read(GRPSCENE* scene)
{
	this->scene = scene;

	XDWORD objectssize = file->objects.GetSize();

	//loadedelements.SetAddInLimit(objectssize+1);

	for(XDWORD i = 0; i < objectssize; i++)
		{
			// if object already loaded skip it
			XQWORD tag = file->seekmap.Get(file->objects.FastGet(i)->tag);
			if(loadedelements.Find(tag) != NOTFOUND)
				{
					//XSTRING str;
					//file->objects.FastGet(i)->id.ToXSTRING(&str);
					//XDEBUG_PRINTCOLOR(4,__L("GGF: PTR already loaded %s"), str.Get());
					continue;
				}

			if(file->objects.FastGet(i)->id == __L("GRPVIEWPORT"))
				{
					GRPVIEWPORT* viewport = new GRPVIEWPORT();

					ReadGRPVIEWPORT(*viewport, file->objects.FastGet(i));

					viewport->Select();
					scene->Add(viewport);

					// need to add this to every object type
					XQWORD index = file->seekmap.Get(file->objects.FastGet(i)->tag);
					loadedelements.Add(index, viewport);
				}
			else if(file->objects.FastGet(i)->id == __L("GRPCAMERA"))
				{
					GRPCAMERA* camera = new GRPCAMERA();

					ReadGRPCAMERA(*camera, file->objects.FastGet(i));

					XQWORD index = file->seekmap.Get(file->objects.FastGet(i)->tag);
					loadedelements.Add(index, camera);
					scene->Add(camera);
					scene->Add((GRPNODE*)camera); // needed as the camera is a node and needs to be handled by the scene
					scene->SetCurrentCamera(scene->GetCameras()->Find(camera));
				}
			else if(file->objects.FastGet(i)->id == __L("GRPNODE"))
				{
					GRPNODE* node = new GRPNODE();

					ReadGRPNODE(*node, file->objects.FastGet(i));

					XQWORD index = file->seekmap.Get(file->objects.FastGet(i)->tag);
					loadedelements.Add(index, node);
					scene->Add(node);
				}
			else if(file->objects.FastGet(i)->id == __L("GRPOBJECT"))
				{
					GRPOBJECT* obj = new GRPOBJECT();
					if(!obj) return false;

					ReadGRPOBJECT(*obj, file->objects.FastGet(i));

					XQWORD index = file->seekmap.Get(file->objects.FastGet(i)->tag);
					loadedelements.Add(index, obj);
					scene->Add(obj);
				}
			else if(file->objects.FastGet(i)->id == __L("GRPELEMENT"))
				{
					GRPELEMENT* element = new GRPELEMENT();
					if(!element) return false;

					ReadGRPELEMENT(*element, file->objects.FastGet(i));

					XQWORD index = file->seekmap.Get(file->objects.FastGet(i)->tag);
					loadedelements.Add(index, element);
					// shouldn't add to scene, as an object is created for them
					//scene->Add(element);
				}
			else if(file->objects.FastGet(i)->id == __L("GRPLIGHT"))
				{
					GRPLIGHT* light = new GRPLIGHT();
					if(!light) return false;
					
					ReadGRPLIGHT(*light, file->objects.FastGet(i));
					
					XQWORD index = file->seekmap.Get(file->objects.FastGet(i)->tag);
					loadedelements.Add(index, light);
					scene->Add(light);
					scene->Add((GRPNODE*)light);
				}
			else if(file->objects.FastGet(i)->id == __L("GRPMATERIAL"))
				{
					GRPMATERIAL * material = new GRPMATERIAL();
					if(!material) return false;

					ReadGRPMATERIAL(*material, file->objects.FastGet(i));

					material->GetAttributes()->UpdateMaterialMatrix();

					GRPMATERIALLIBRARY::Get()->Add(material);
					XQWORD index = file->seekmap.Get(file->objects.FastGet(i)->tag);
					loadedelements.Add(index, material);
				}
			else if(file->objects.FastGet(i)->id == __L("GRPANIMATIONSTACK"))
			{
				GRPANIMATIONSTACK* stack = new GRPANIMATIONSTACK();
				if(!stack) return false;

				ReadGRPANIMATIONSTACK(*stack, file->objects.FastGet(i));

				scene->GetAnimationControl()->AddStack(stack);
				XQWORD index = file->seekmap.Get(file->objects.FastGet(i)->tag);
				loadedelements.Add(index, stack);
			}
			else if(file->objects.FastGet(i)->id == __L("GRPANIMATIONCONTROL"))
			{
				ReadGRPANIMATIONCONTROL(*scene->GetAnimationControl(), file->objects.FastGet(i));
			}
		}

	return true;
}


/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::WriteObjects
*/	
/**	
//	
//	writes the objects
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			24/11/2015 12:31:48
//	
//	@return 			bool : 
//
//  @param				scene : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::WriteObjects(GRPSCENE* scene)
{
	XVECTOR<GRPOBJECT*>* objects = scene->GetObjects();
	XDWORD objectsize = objects->GetSize();

	// search the definition of this object and assign it to the object
	XVECTOR<GGFOBJECTDEFINITION*>* definitions = file->GetDefinitions();
	GGFOBJECTDEFINITION* definition = NULL;
	for(XDWORD i = 0; i < definitions->GetSize(); i++)
	{
		if(definitions->FastGet(i)->id == __L("GRPOBJECT"))
		{
			definition = definitions->FastGet(i);
			break;
		}
	}

	for(XDWORD i = 0; i < objectsize; i++)
		{
			GGFOBJECT* obj = new GGFOBJECT( file->info, NULL);
			if(!obj)
				{
					return false;
				}

			XSTRING str;
			str.Set(__L("GRPOBJECT"));
			obj->id.FromXSTRING(&str);
			obj->tag = (XQWORD)objects->Get(i);
			obj->definition = definition;

			WriteGRPOBJECT(*objects->FastGet(i), obj);			

			if(!file->AddObject(obj))
				{
					XDEBUG_PRINTCOLOR(4,__L("GGF: couldn't add %s"), __L("GRPOBJECT"));
				}
		}

	return true;
}





/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::WriteViewPorts
*/	
/**	
//	
//	writes all the viewports
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			25/11/2015 11:33:30
//	
//	@return 			bool : 
//
//  @param				scene : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::WriteViewPorts(GRPSCENE* scene)
{
	XVECTOR<GRPVIEWPORT*>* viewports = scene->GetViewports();

	// search the definition of this object and assign it to the object
	XVECTOR<GGFOBJECTDEFINITION*>* definitions = file->GetDefinitions();
	GGFOBJECTDEFINITION* definition = NULL;
	for(XDWORD i = 0; i < definitions->GetSize(); i++)
	{
		if(definitions->FastGet(i)->id == __L("GRPVIEWPORT"))
		{
			definition = definitions->FastGet(i);
			break;
		}
	}

	for(XDWORD i = 0; i < viewports->GetSize(); i++)
		{
			GGFOBJECT* obj = new GGFOBJECT( file->info, NULL);
			if(!obj)
				{
					return false;
				}

			obj->definition = definition;

			WriteGRPVIEWPORT(*viewports->FastGet(i), obj);

			if(!file->AddObject(obj))
				{
					XDEBUG_PRINTCOLOR(4,__L("GGF: couldn't add %s"), __L("GRPVIEWPORT"));
				}
		}

	return true;
}





/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::WriteMaterials
*/	
/**	
//	
//	writes all materials
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			30/11/2015 10:03:36
//	
//	@return 			bool : 
//
//  @param				scene : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::WriteMaterials(GRPSCENE* scene)
{
	XMAP<XSTRING*, GRPMATERIAL*>* materials = GRPMATERIALLIBRARY::Get()->GetLibrary();

	// search the definition of this object and assign it to the object
	XVECTOR<GGFOBJECTDEFINITION*>* definitions = file->GetDefinitions();
	GGFOBJECTDEFINITION* definition = NULL;
	for(XDWORD i = 0; i < definitions->GetSize(); i++)
	{
		if(definitions->FastGet(i)->id == __L("GRPMATERIAL"))
		{
			definition = definitions->FastGet(i);
			break;
		}
	}

	for(XDWORD i = 0; i < materials->GetSize(); i++)
		{
			GGFOBJECT* obj = new GGFOBJECT( file->info, NULL);
			if(!obj)
				{
					return false;
				}

			XSTRING str;
			str.Set(__L("GRPMATERIAL"));
			obj->id.FromXSTRING(&str);
			obj->tag = (XQWORD)materials->GetElement(i);
			obj->definition = definition;

			GRPMATERIAL* m = materials->GetElement(i);

			WriteGRPMATERIAL(*m, obj);

			if(!file->AddObject(obj))
				{
					XDEBUG_PRINTCOLOR(4,__L("GGF: couldn't add %s"), __L("GRPMATERIAL"));
				}
		}

	return true;
}





/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::WriteTextures
*/	
/**	
//	
//	writes all textures
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			30/11/2015 10:03:26
//	
//	@return 			bool : 
//
//  @param				scene : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::WriteTextures(GRPSCENE* scene)
{
	XMAP<XSTRING*, GRPTEXTURE*>* textures = GRPMATERIALLIBRARY::Get()->GetTextures();

	// search the definition of this object and assign it to the object
	XVECTOR<GGFOBJECTDEFINITION*>* definitions = file->GetDefinitions();
	GGFOBJECTDEFINITION* definition = NULL;
	for(XDWORD i = 0; i < definitions->GetSize(); i++)
	{
		if(definitions->FastGet(i)->id == __L("GRPTEXTURE"))
		{
			definition = definitions->FastGet(i);
			break;
		}
	}

	for(XDWORD i = 0; i < textures->GetSize(); i++)
		{
			GGFOBJECT* obj = new GGFOBJECT( file->info, NULL);
			if(!obj)
				{
					return false;
				}

			XSTRING str;
			str.Set(__L("GRPTEXTURE"));
			obj->id.FromXSTRING(&str);
			obj->tag = (XQWORD)textures->GetElement(i);
			obj->definition = definition;

			GRPTEXTURE* t = textures->GetElement(i);

			WriteGRPTEXTURE(*t, obj);

			if(!file->AddObject(obj))
				{
					XDEBUG_PRINTCOLOR(4,__L("GGF: couldn't add %s"), __L("GRPTEXTURE"));
				}
		}

	return true;
}



/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::WriteAnimationStacks
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/12/2015 12:37:17
//	
//	@return 			bool : 
//
//  @param				scene : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::WriteAnimationStacks(GRPSCENE* scene)
{
	GRPANIMATIONCONTROL* control = scene->GetAnimationControl();
	XVECTOR<GRPANIMATIONSTACK*>* stacks = control->GetStacks();

	// search the definition of this object and assign it to the object
	XVECTOR<GGFOBJECTDEFINITION*>* definitions = file->GetDefinitions();
	GGFOBJECTDEFINITION* definition = NULL;
	for(XDWORD i = 0; i < definitions->GetSize(); i++)
	{
		if(definitions->FastGet(i)->id == __L("GRPANIMATIONSTACK"))
		{
			definition = definitions->FastGet(i);
			break;
		}
	}

	for(XDWORD i = 0; i < stacks->GetSize(); i++)
		{
			GGFOBJECT* obj = new GGFOBJECT( file->info, NULL);

			if(!obj)
				{
					return false;
				}

			XSTRING str;
			str.Set(__L("GRPANIMATIONSTACK"));
			obj->id.FromXSTRING(&str);
			obj->tag = (XQWORD)stacks->FastGet(i);
			obj->definition = definition;

			GRPANIMATIONSTACK* t = stacks->FastGet(i);

			WriteGRPANIMATIONSTACK(*t, obj);

			if(!file->AddObject(obj))
				{
					XDEBUG_PRINTCOLOR(4,__L("GGF: couldn't add %s"), __L("GRPANIMATIONSTACK"));
					return false;
				}

			// now write it's layers, this will add the layers to the object
			WriteAnimationLayers(stacks->FastGet(i));
		}

	return true;
}





/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::WriteAnimationLayers
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/12/2015 16:38:30
//	
//	@return 			bool : 
//
//  @param				stack : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::WriteAnimationLayers(GRPANIMATIONSTACK* stack)
{
	XVECTOR<GRPANIMATIONLAYER*>* layers = stack->GetLayers();

	// search the definition of this object and assign it to the object
	XVECTOR<GGFOBJECTDEFINITION*>* definitions = file->GetDefinitions();
	GGFOBJECTDEFINITION* definition = NULL;
	for(XDWORD i = 0; i < definitions->GetSize(); i++)
	{
		if(definitions->FastGet(i)->id == __L("GRPANIMATIONLAYER"))
		{
			definition = definitions->FastGet(i);
			break;
		}
	}

	for(XDWORD i = 0; i < layers->GetSize(); i++)
		{
			GGFOBJECT* obj = new GGFOBJECT( file->info, NULL);

			if(!obj)
				{
					return false;
				}

			XSTRING str;
			str.Set(__L("GRPANIMATIONLAYER"));
			obj->id.FromXSTRING(&str);
			obj->tag = (XQWORD)layers->FastGet(i);
			obj->definition = definition;

			GRPANIMATIONLAYER* t = layers->FastGet(i);

			WriteGRPANIMATIONLAYER(*t, obj);

			if(!file->AddObject(obj))
				{
					XDEBUG_PRINTCOLOR(4,__L("GGF: couldn't add %s"), __L("GRPANIMATIONLAYER"));
					return false;
				}

			WriteAnimationNodes(layers->FastGet(i));
			WriteAnimationCurves(layers->FastGet(i));
		}

	return true;
}





/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::WriteAnimationNodes
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/12/2015 17:44:41
//	
//	@return 			bool : 
//
//  @param				layer : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::WriteAnimationNodes(GRPANIMATIONLAYER* layer)
{
	XVECTOR<GRPANIMATIONNODE*>* nodes = layer->GetAnimationNodes();

	// search the definition of this object and assign it to the object
	XVECTOR<GGFOBJECTDEFINITION*>* definitions = file->GetDefinitions();
	GGFOBJECTDEFINITION* definition = NULL;
	for(XDWORD i = 0; i < definitions->GetSize(); i++)
	{
		if(definitions->FastGet(i)->id == __L("GRPANIMATIONNODE"))
		{
			definition = definitions->FastGet(i);
			break;
		}
	}

	for(XDWORD i = 0; i < nodes->GetSize(); i++)
		{
			GGFOBJECT* obj = new GGFOBJECT( file->info, NULL);
			if(!obj)
				{
					return false;
				}

			XSTRING str;
			str.Set(__L("GRPANIMATIONNODE"));
			obj->id.FromXSTRING(&str);
			obj->tag = (XQWORD)nodes->FastGet(i);
			obj->definition = definition;

			GRPANIMATIONNODE* t = nodes->FastGet(i);

			WriteGRPANIMATIONNODE(*t, obj);

			if(!file->AddObject(obj))
				{
					XDEBUG_PRINTCOLOR(4,__L("GGF: couldn't add %s"), __L("GRPANIMATIONNODE"));
					return false;
				}

			// write curves in here? probably not
			//WriteAnimationNodes(layers->FastGet(i));
		}

	return true;
}



/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::WriteAnimationCurves
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			02/12/2015 10:26:02
//	
//	@return 			bool : 
//
//  @param				layer : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::WriteAnimationCurves(GRPANIMATIONLAYER* layer)
{
	XVECTOR<GRPANIMATIONCURVE*>* curves = layer->GetCurves();

	// search the definition of this object and assign it to the object
	XVECTOR<GGFOBJECTDEFINITION*>* definitions = file->GetDefinitions();
	GGFOBJECTDEFINITION* definition = NULL;
	for(XDWORD i = 0; i < definitions->GetSize(); i++)
	{
		if(definitions->FastGet(i)->id == __L("GRPANIMATIONCURVE"))
		{
			definition = definitions->FastGet(i);
			break;
		}
	}

	for(XDWORD i = 0; i < curves->GetSize(); i++)
		{
			GGFOBJECT* obj = new GGFOBJECT( file->info, NULL);
				if(!obj)
					{
						return false;
					}

				XSTRING str;
				str.Set(__L("GRPANIMATIONCURVE"));
				obj->id.FromXSTRING(&str);
				obj->tag = (XQWORD)curves->FastGet(i);
				obj->definition = definition;

				GRPANIMATIONCURVE* t = curves->FastGet(i);

				WriteGRPANIMATIONCURVE(*t, obj);

				if(!file->AddObject(obj))
					{
						XDEBUG_PRINTCOLOR(4,__L("GGF: couldn't add %s"), __L("GRPANIMATIONCURVE"));
						return false;
					}

				// write the bezier splines
				WriteBezierSplines(curves->FastGet(i));
		}


	return true;
}




/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::WriteBezierSplines
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			02/12/2015 11:49:20
//	
//	@return 			bool : 
//
//  @param				curve : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::WriteBezierSplines(GRPANIMATIONCURVE* curve)
{
	XVECTOR<GRPBEZIERSPLINE*>* splines = &(curve->bcs);

	// search the definition of this object and assign it to the object
	XVECTOR<GGFOBJECTDEFINITION*>* definitions = file->GetDefinitions();
	GGFOBJECTDEFINITION* definition = NULL;
	for(XDWORD i = 0; i < definitions->GetSize(); i++)
	{
		if(definitions->FastGet(i)->id == __L("GRPBEZIERSPLINE"))
		{
			definition = definitions->FastGet(i);
			break;
		}
	}

	for(XDWORD i = 0; i < splines->GetSize(); i++)
		{
			GGFOBJECT* obj = new GGFOBJECT( file->info, NULL);
				if(!obj)
					{
						return false;
					}

				XSTRING str;
				str.Set(__L("GRPBEZIERSPLINE"));
				obj->id.FromXSTRING(&str);
				obj->tag = (XQWORD)splines->FastGet(i);
				obj->definition = definition;

				GRPBEZIERSPLINE* t = splines->FastGet(i);

				WriteGRPBEZIERSPLINE(*t, obj);

				if(!file->AddObject(obj))
					{
						XDEBUG_PRINTCOLOR(4,__L("GGF: couldn't add %s"), __L("GRPBEZIERSPLINE"));
						return false;
					}
		}

	return true;
}


/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::WriteCameras
*/	
/**	
//	
//	writes the scene cameras
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			25/11/2015 15:52:14
//	
//	@return 			bool : 
//
//  @param				scene : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::WriteCameras(GRPSCENE* scene)
{
	XDWORD camerasnum = scene->GetCameras()->GetSize();

	// search the definition of this object and assign it to the object
	XVECTOR<GGFOBJECTDEFINITION*>* definitions = file->GetDefinitions();
	GGFOBJECTDEFINITION* definition = NULL;
	for(XDWORD i = 0; i < definitions->GetSize(); i++)
	{
		if(definitions->FastGet(i)->id == __L("GRPCAMERA"))
		{
			definition = definitions->FastGet(i);
			break;
		}
	}

	for(XDWORD i = 0; i < camerasnum; i++)
	{
			GGFOBJECT* obj = new GGFOBJECT( file->info, NULL);
			if(!obj)
				{
					return false;
				}

			XSTRING str;
			str.Set(__L("GRPCAMERA"));
			obj->id.FromXSTRING(&str);
			obj->tag = (XQWORD)scene->GetCameras()->FastGet(i);
			obj->definition = definition;

			WriteGRPCAMERA(*scene->GetCameras()->FastGet(i), obj);

			if(!file->AddObject(obj))
				{
					XDEBUG_PRINTCOLOR(4,__L("GGF: couldn't add %s"), __L("GRPCAMERA"));
				}
	}

	return true;
}




/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::WriteLights
*/	
/**	
//	
//	writes the lights
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			30/11/2015 10:07:45
//	
//	@return 			bool : 
//
//  @param				scene : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::WriteLights(GRPSCENE* scene)
{
	XDWORD lightsnum = scene->GetLights()->GetSize();

	// search the definition of this object and assign it to the object
	XVECTOR<GGFOBJECTDEFINITION*>* definitions = file->GetDefinitions();
	GGFOBJECTDEFINITION* definition = NULL;
	for(XDWORD i = 0; i < definitions->GetSize(); i++)
	{
		if(definitions->FastGet(i)->id == __L("GRPLIGHT"))
		{
			definition = definitions->FastGet(i);
			break;
		}
	}

	for(XDWORD i = 0; i < lightsnum; i++)
		{
			GRPLIGHT* light = scene->GetLights()->FastGet(i);

			GGFOBJECT* obj = new GGFOBJECT( file->info, NULL);
			if(!obj)
				{
					return false;
				}

			XSTRING str;
			str.Set(__L("GRPLIGHT"));
			obj->id.FromXSTRING(&str);
			obj->tag = (XQWORD)scene->GetLights()->FastGet(i);
			obj->definition = definition;

			WriteGRPLIGHT(*light, obj);

			if(!file->AddObject(obj))
				{
					XDEBUG_PRINTCOLOR(4,__L("GGF: couldn't add %s"), __L("GRPLIGHT"));
				}
		}

	return true;
}




/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::DefineGRPUSERDATACONTAINER
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/12/2015 16:15:50
//	
//	@return 			bool : 
//
//  @param				definition : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::DefineGRPUSERDATACONTAINER(GGFOBJECTDEFINITION* definition)
{
	RegisterDefinition(__L("GRPUSERDATACONTAINER.nelements"), sizeof(XDWORD), definition);

	RegisterDefinition(__L("GRPUSERDATACONTAINER.keysvector"), sizeof(XDWORD), definition);
	RegisterDefinition(__L("GRPUSERDATACONTAINER.valuesvector"), sizeof(XDWORD), definition);

	return true;
}



/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::WriteGRPUSERDATACONTAINER
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/12/2015 16:12:11
//	
//	@return 			bool : 
//
//  @param				container : 
//  @param				object : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::WriteGRPUSERDATACONTAINER(GRPUSERDATACONTAINER& container, GGFOBJECT* object)
{
	GGFVALUE value;
	value.SetXDWORD(container.Get()->GetSize());
	object->Add(__L("GRPUSERDATACONTAINER.nelements"), &value);

	XBUFFER keys( false);
	XBUFFER values( false);

	keys.Resize(container.Get()->GetSize()*sizeof(XDWORD));
	values.Resize(container.Get()->GetSize()*sizeof(XDWORD));

	for(XDWORD i = 0; i < container.Get()->GetSize(); i++)
		{
			GGFSTRING name( container.Get()->GetKey(i)->Get());

			GGFDATA* datakey = new GGFDATA();

			if(!datakey) return false;

			datakey->buffer.Resize(name.Size());
			name.WriteTo(datakey->buffer);
	
			XDWORD keyindex = file->AddData(datakey);

			name.FromXSTRING(container.Get()->GetElement(i));

			GGFDATA* datavalue = new GGFDATA();

			if(!datavalue) return false;

			datavalue->buffer.Resize(name.Size());
			name.WriteTo(datavalue->buffer);
		
			XDWORD valueindex = file->AddData(datavalue);
			keys.Set((XBYTE*)&keyindex, sizeof(XDWORD));
			values.Set((XBYTE*)&valueindex, sizeof(XDWORD));
		}

	GGFDATA* keysvector = new GGFDATA();
	if(!keysvector) return false;

	keysvector->buffer.Resize(keys.GetSize());
	keysvector->buffer.Set(keys.Get(), keys.GetSize());

	GGFDATA* valuesvector = new GGFDATA();
	if(!valuesvector) return false;

	valuesvector->buffer.Resize(keys.GetSize());
	valuesvector->buffer.Set(values.Get(), values.GetSize());

	XDWORD keysvectorindex = file->AddData(keysvector);
	XDWORD valuesvectorindex = file->AddData(valuesvector);


	value.SetValue(keysvectorindex);
	object->Add(__L("GRPUSERDATACONTAINER.keysvector"), &value);

	value.SetValue(valuesvectorindex);
	object->Add(__L("GRPUSERDATACONTAINER.valuesvector"), &value);

	return true;
}




/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::ReadGRPUSERDATACONTAINER
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/12/2015 16:31:16
//	
//	@return 			bool : 
//
//  @param				container : 
//  @param				object : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::ReadGRPUSERDATACONTAINER(GRPUSERDATACONTAINER& container, GGFOBJECT* object)
{
	XDWORD nelements = object->Get(__L("GRPUSERDATACONTAINER.nelements"))->GETXDWORD();
	XDWORD keysvectorindex = object->Get(__L("GRPUSERDATACONTAINER.keysvector"))->GETXDWORD();
	XDWORD valuesvectorindex = object->Get(__L("GRPUSERDATACONTAINER.valuesvector"))->GETXDWORD();

	GGFDATA* keysvector = file->data.Get(keysvectorindex);
	GGFDATA* valuesvector = file->data.Get(valuesvectorindex);

	XBUFFER keys( false);
	XBUFFER values( false);

	keys.Resize(keysvector->buffer.GetSize());
	values.Resize(valuesvector->buffer.GetSize());

	keys.Set(keysvector->buffer.Get(), keysvector->buffer.GetSize());
	values.Set(valuesvector->buffer.Get(), valuesvector->buffer.GetSize());

	keys.SetPosition(0);
	values.SetPosition(0);

	for(XDWORD i = 0; i < nelements; i++)
	{
		// read every data container entry
		XDWORD keyindex;
		XDWORD valueindex;
		keys.Get((XBYTE*)&keyindex, sizeof(XDWORD));
		values.Get((XBYTE*)&valueindex, sizeof(XDWORD));

		GGFDATA* keydata = file->data.Get(keyindex);
		GGFDATA* valuedata = file->data.Get(valueindex);

		GGFSTRING keyggfstring;
		keyggfstring.ReadFrom(keydata->buffer);

		GGFSTRING valueggfstring;
		valueggfstring.ReadFrom(valuedata->buffer);

		XSTRING* key = new XSTRING();
		XSTRING* value = new XSTRING();

		keyggfstring.ToXSTRING(key);
		valueggfstring.ToXSTRING(value);

		container.Get()->Add(key, value);
	}

	return true;
}

/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::DefineGRPMATERIAL
*/	
/**	
//	
//	defines a material
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			24/11/2015 12:31:27
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::DefineGRPMATERIAL(GGFOBJECTDEFINITION* definition)
{
	DefineGRPINTERFACEANIMABLE(definition);

	RegisterDefinition(__L("GRPMATERIAL.isusable"), sizeof(XBYTE), definition);

	DefineGRPMATERIALATTRIBUTES(definition);

	// string into data area
	RegisterDefinition(__L("GRPMATERIAL.ShaderModelName"), sizeof(XDWORD), definition);

	// list of layers
	RegisterDefinition(__L("GRPMATERIAL.layers"), sizeof(file->info.ptrsize), definition);

	return true;
}




/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::WriteGRPMATERIAL
*/	
/**	
//	
//	writes a material into a ggf
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			27/11/2015 13:28:51
//	
//	@return 			bool : 
//
//  @param				material : 
//  @param				object : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::WriteGRPMATERIAL(GRPMATERIAL& material, GGFOBJECT* object)
{
	WriteGRPINTERFACEANIMABLE(material, object);
	WriteGRPMATERIALATTRIBUTES(*material.GetAttributes(), object);

	GGFVALUE value;
	XSTRING str;
	value.SetValue(material.IsUsable());
	str.Set(__L("GRPMATERIAL.isusable"));
	object->Add(&str, &value);

	for(XDWORD i = 0; i < material.GetLayers()->GetSize(); i++)
		{
			value.SetValue(material.GetLayer(i));
			str.Set(__L("GRPMATERIAL.layers"));
			object->Add(&str, &value);
		}

	// shader model name into data area
	GGFSTRING name;
	name.FromXSTRING(material.GetShadingModelName());

	GGFDATA* data = new GGFDATA();

	data->buffer.Resize(name.Size());
	name.WriteTo(data->buffer);
	
	XDWORD index = file->AddData(data);

	value.SetValue(index);
	str.Set(__L("GRPMATERIAL.ShaderModelName"));
	object->Add(&str, &value);

	return true;
}




/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::ReadGRPMATERIAL
*/	
/**	
//	
//	reads a material
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			30/11/2015 16:35:28
//	
//	@return 			bool : 
//
//  @param				material : 
//  @param				object : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::ReadGRPMATERIAL(GRPMATERIAL& material, GGFOBJECT* object)
{
	// need to create a new material, BUT there is no simple way of doign that load

	ReadGRPINTERFACEANIMABLE(material, object);
	ReadGRPMATERIALATTRIBUTES(*material.GetAttributes(), object);

	XDWORD properties = object->indexedvalues.GetSize();//object->values.GetSize();


	XDWORD index = object->Get(__L("GRPMATERIAL.ShaderModelName"))->GETXDWORD();
	GGFDATA* data = file->data.Get(index);

	GGFSTRING ggfstr;
	ggfstr.ReadFrom(data->buffer);

	XSTRING name;
	ggfstr.ToXSTRING(&name);
	
	material.SetShadingModelName(name);



	XLIST<GGFVALUE*> layers;
	object->GetMultiple(__L("GRPMATERIAL.layers"), layers);
	XLISTDATA<GGFVALUE*>*it = layers.GetFirstData();
	
	while(!IsLast(it))
	{
		// look if layer exists by object pointer
		XQWORD ptr = 0;
		// get the ptr from specific element
		it->data->buffer.Get((XBYTE*)&ptr, file->info.ptrsize, 0);
		
		GRPTEXTURE* texture = (GRPTEXTURE*)GetLoadedObject(ptr);
		
		if((ptr != 0) && (texture == NULL))
			{
				// read texture name
				// run a load of the texture
				// add the texture as a layer
				// add the texture to the loadedelements

				// read element
				//XQWORD index = file->seekmap.Find(ptr);
				XQWORD index = file->seekmap.seekmaptree.Get(ptr);
				if(index != NOTFOUND)
					{
						GGFOBJECT* obj = file->objects.Get((XDWORD)index);
						XDWORD dataindex = obj->Get(__L("GRPTEXTURE.name"))->GETXDWORD();
						GGFDATA* data = file->data.Get(dataindex);
						GGFSTRING ggfstring;
						ggfstring.ReadFrom(data->buffer);
						XSTRING name;
						ggfstring.ToXSTRING(&name);

						// FILENAME
						dataindex = obj->Get(__L("GRPTEXTURE.filename"))->GETXDWORD();
						data = file->data.Get(dataindex);
						ggfstring.ReadFrom(data->buffer);
						XSTRING filename;
						ggfstring.ToXSTRING(&filename);


						XPATH resourcefile;
						GetFilePathTexture(filename.Get(), resourcefile);

						GRPMATERIALLIBRARY::Get()->Load(resourcefile.Get(), name.Get());

						GRPTEXTURE* t =GRPMATERIALLIBRARY::Get()->GetTexture(name.Get());
						if(t != NULL)
							{
								material.AddLayer(t);
								loadedelements.Add(index, t);
							}
					}
				else
					{
						XDEBUG_PRINTCOLOR(4,__L("ERROR: looking for an object not present in seekmap"));
					}
			}
		else
			{
				material.AddLayer(texture);
			}

		it = Increment(it);
	}

	return true;
}


/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::DefineGRPTEXTURE
*/	
/**	
//	
//	defines a texture
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			27/11/2015 13:43:38
//	
//	@return 			bool : 
//
//  @param				definition : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::DefineGRPTEXTURE(GGFOBJECTDEFINITION* definition)
{
	// texture name into data area
	RegisterDefinition(__L("GRPTEXTURE.name"), sizeof(XDWORD), definition);
	RegisterDefinition(__L("GRPTEXTURE.filename"), sizeof(XDWORD), definition);


	RegisterDefinition(__L("GRPTEXTURE.sframe"), sizeof(float), definition);
	RegisterDefinition(__L("GRPTEXTURE.layertype"), sizeof(GRPTEXTURELAYERTYPE), definition);

	return true;
}



/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::WriteGRPTEXTURE
*/	
/**	
//	
//	writes a texture
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			27/11/2015 13:44:30
//	
//	@return 			bool : 
//
//  @param				texture : 
//  @param				object : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::WriteGRPTEXTURE(GRPTEXTURE& texture, GGFOBJECT* object)
{
	// shader model name into data area
	GGFVALUE value;
	XSTRING str;
	GGFSTRING name;
	XPATH path(texture.GetName()->Get());

	XSTRING filename;
	path.GetNamefileExt(filename);

	//name.FromXSTRING(&filename);
	name.FromXSTRING(texture.GetName());

	GGFDATA* data = new GGFDATA();

	data->buffer.Resize(name.Size());
	name.WriteTo(data->buffer);
	
	XDWORD index = file->AddData(data);

	value.SetValue(index);
	str.Set(__L("GRPTEXTURE.name"));
	object->Add(&str, &value);

	// FILENAME
	name.FromXSTRING(texture.GetFileName());

	data = new GGFDATA();

	data->buffer.Resize(name.Size());
	name.WriteTo(data->buffer);
	
	index = file->AddData(data);

	value.SetValue(index);
	str.Set(__L("GRPTEXTURE.filename"));
	object->Add(&str, &value);

	// frame
	value.SetValue(texture.GetFrame());
	str.Set(__L("GRPTEXTURE.sframe"));
	object->Add(&str, &value);

	value.SetValue(texture.GetLayertype());
	str.Set(__L("GRPTEXTURE.layertype"));
	object->Add(&str, &value);

	return true;
}




/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::ReadGRPTEXTURE
*/	
/**	
//	
//	loads a texture
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			30/11/2015 12:17:39
//	
//	@return 			bool : 
//
//  @param				texture : 
//  @param				object : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::ReadGRPTEXTURE(GRPTEXTURE** texture, GGFOBJECT* object)
{
	XDWORD index = object->Get(__L("GRPTEXTURE.name"))->GETXDWORD();

	GGFDATA* data = file->data.Get(index);
	GGFSTRING ggfstring;
	XSTRING str;

	ggfstring.ReadFrom(data->buffer);
	ggfstring.ToXSTRING(&str);

	index = object->Get(__L("GRPTEXTURE.filename"))->GETXDWORD();

	data = file->data.Get(index);
	XSTRING filename;

	ggfstring.ReadFrom(data->buffer);
	ggfstring.ToXSTRING(&filename);

	XPATH resourcefile;
	//GetFilePathTexture(str.Get(), resourcefile);
	GetFilePathTexture(filename.Get(), resourcefile);

	GRPMATERIALLIBRARY::Get()->Load(resourcefile.Get(), str.Get());


	// will need to probably loop over to find the correct name
	for(XDWORD i = 0; i < GRPMATERIALLIBRARY::Get()->GetTextures()->GetSize(); i++)
		{
			if(*GRPMATERIALLIBRARY::Get()->GetTextures()->GetKey(i) == resourcefile)
				{
					*texture = GRPMATERIALLIBRARY::Get()->GetTextures()->GetElement(i);
					break;
				}
		}

	return true;
}




/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::DefineGRPMATERIALATTRIBUTES
*/	
/**	
//	
//	defines material attributes
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			24/11/2015 12:32:01
//	
//	@return 			bool : 
//
//  @param				definition : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::DefineGRPMATERIALATTRIBUTES(GGFOBJECTDEFINITION* definition)
{
	RegisterDefinition(__L("GRPMATERIALATTRIBUTES.illumination"), sizeof(XBYTE), definition);

	RegisterDefinition(__L("GRPMATERIALATTRIBUTES.glossiness"), sizeof(float), definition);
	RegisterDefinition(__L("GRPMATERIALATTRIBUTES.shininess"), sizeof(float), definition);
	RegisterDefinition(__L("GRPMATERIALATTRIBUTES.shininessFactor"), sizeof(float), definition);
	RegisterDefinition(__L("GRPMATERIALATTRIBUTES.dissolve"), sizeof(float), definition);

	RegisterDefinition(__L("GRPMATERIALATTRIBUTES.specular"), 4*sizeof(float), definition);
	RegisterDefinition(__L("GRPMATERIALATTRIBUTES.specularFactor"), sizeof(float), definition);

	RegisterDefinition(__L("GRPMATERIALATTRIBUTES.ambient"), 4*sizeof(float), definition);
	RegisterDefinition(__L("GRPMATERIALATTRIBUTES.diffuse"), 4*sizeof(float), definition);
	RegisterDefinition(__L("GRPMATERIALATTRIBUTES.emission"), 4*sizeof(float), definition);

	RegisterDefinition(__L("GRPMATERIALATTRIBUTES.intensity"), sizeof(float), definition);
	RegisterDefinition(__L("GRPMATERIALATTRIBUTES.transparency"), sizeof(float), definition);

	return true;
}



/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::WriteGRPMATERIALATTRIBUTES
*/	
/**	
//	
//	writes material attributes
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			24/11/2015 12:32:13
//	
//	@return 			bool : 
//
//  @param				materialattributes : 
//  @param				object : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::WriteGRPMATERIALATTRIBUTES(GRPMATERIALATTRIBUTES & materialattributes, GGFOBJECT* object)
{
	XSTRING str;
	GGFVALUE value;
	value.SetValue(materialattributes.GetGlossiness());
	str.Set(__L("GRPMATERIALATTRIBUTES.glossiness"));
	object->Add(&str, &value);

	value.SetBOOL(materialattributes.IsIlluminated());
	str.Set(__L("GRPMATERIALATTRIBUTES.illumination"));
	object->Add(&str, &value);

	value.SetValue(materialattributes.GetShininess());
	str.Set(__L("GRPMATERIALATTRIBUTES.shininess"));
	object->Add(&str, &value);

	value.SetValue(materialattributes.GetShininessFactor());
	str.Set(__L("GRPMATERIALATTRIBUTES.shininessFactor"));
	object->Add(&str, &value);

	value.SetValue(materialattributes.GetDissolve());
	str.Set(__L("GRPMATERIALATTRIBUTES.dissolve"));
	object->Add(&str, &value);

	value.SetGRPCOLOR(*materialattributes.GetSpecular());
	str.Set(__L("GRPMATERIALATTRIBUTES.specular"));
	object->Add(&str, &value);

	value.SetValue(materialattributes.GetSpecularFactor());
	str.Set(__L("GRPMATERIALATTRIBUTES.specularFactor"));
	object->Add(&str, &value);

	value.SetGRPCOLOR(*materialattributes.GetAmbient());
	str.Set(__L("GRPMATERIALATTRIBUTES.ambient"));
	object->Add(&str, &value);

	value.SetGRPCOLOR(*materialattributes.GetDiffuse());
	str.Set(__L("GRPMATERIALATTRIBUTES.diffuse"));
	object->Add(&str, &value);

	value.SetGRPCOLOR(*materialattributes.GetEmission());
	str.Set(__L("GRPMATERIALATTRIBUTES.emission"));
	object->Add(&str, &value);

	value.SetValue(materialattributes.GetIntensity());
	str.Set(__L("GRPMATERIALATTRIBUTES.intensity"));
	object->Add(&str, &value);

	value.SetValue(materialattributes.GetTransparency());
	str.Set(__L("GRPMATERIALATTRIBUTES.transparency"));
	object->Add(&str, &value);

	return true;
}




/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::ReadGRPMATERIALATTRIBUTES
*/	
/**	
//	
//	reads material attributes
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			26/11/2015 13:44:59
//	
//	@return 			bool : 
//
//  @param				materialattributes : 
//  @param				object : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::ReadGRPMATERIALATTRIBUTES(GRPMATERIALATTRIBUTES & materialattributes, GGFOBJECT* object)
{
	bool b = object->Get(__L("GRPMATERIALATTRIBUTES.illumination"))->GetBOOL();
	materialattributes.SetIsIlluminated(b);

	float f = object->Get(__L("GRPMATERIALATTRIBUTES.glossiness"))->Getfloat();
	materialattributes.SetGlossiness(f);
	f = object->Get(__L("GRPMATERIALATTRIBUTES.shininess"))->Getfloat();
	materialattributes.SetShininess(f);
	f = object->Get(__L("GRPMATERIALATTRIBUTES.shininessFactor"))->Getfloat();
	materialattributes.SetShininessFactor(f);
	f = object->Get(__L("GRPMATERIALATTRIBUTES.dissolve"))->Getfloat();
	materialattributes.SetDissolve(f);

	GRPCOLOR c = object->Get(__L("GRPMATERIALATTRIBUTES.specular"))->GetGRPCOLOR();
	materialattributes.SetSpecular(c);
	f = object->Get(__L("GRPMATERIALATTRIBUTES.specularFactor"))->Getfloat();
	materialattributes.SetSpecularFactor(f);

	c = object->Get(__L("GRPMATERIALATTRIBUTES.ambient"))->GetGRPCOLOR();
	materialattributes.SetAmbient(c);
	c = object->Get(__L("GRPMATERIALATTRIBUTES.diffuse"))->GetGRPCOLOR();
	materialattributes.SetDiffuse(c);
	c = object->Get(__L("GRPMATERIALATTRIBUTES.emission"))->GetGRPCOLOR();
	materialattributes.SetEmission(c);

	f = object->Get(__L("GRPMATERIALATTRIBUTES.intensity"))->Getfloat();
	materialattributes.SetIntensity(f);
	f = object->Get(__L("GRPMATERIALATTRIBUTES.transparency"))->Getfloat();
	materialattributes.SetTransparency(f);

	materialattributes.UpdateMaterialMatrix();

	return true;
}


/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::DefineGRPNAMED
*/	
/**	
//	
//	defines named
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			24/11/2015 12:32:22
//	
//	@return 			bool : 
//
//  @param				definition : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::DefineGRPNAMED(GGFOBJECTDEFINITION* definition)
{
	// First test, add the node definition
	RegisterDefinition(__L("GRPNAMED.tag"), sizeof(XQWORD), definition);
	RegisterDefinition(__L("GRPNAMED.name"), sizeof(XDWORD), definition);

	return true;
}



/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::WriteGRPNAMED
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			24/11/2015 12:32:29
//	
//	@return 			bool : 
//
//  @param				named : 
//  @param				object : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::WriteGRPNAMED(GRPNAMED & named, GGFOBJECT* object)
{
	XSTRING str;
	GGFVALUE value;
	value.SetValue(named.tag);
	str.Set(__L("GRPNAMED.tag"));
	object->Add(&str, &value);

	GGFSTRING name;
	name.FromXSTRING(&named.name);

	GGFDATA* data = new GGFDATA();

	data->buffer.Resize(name.Size());
	name.WriteTo(data->buffer);
	
	XDWORD index = file->AddData(data);

	value.SetValue(index);
	str.Set(__L("GRPNAMED.name"));
	object->Add(&str, &value);

	return true;
}





/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::ReadGRPNAMED
*/	
/**	
//	
//	reads a named object
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			25/11/2015 12:00:11
//	
//	@return 			bool : 
//
//  @param				namedattrbiutes : 
//  @param				object : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::ReadGRPNAMED(GRPNAMED & namedattrbiutes, GGFOBJECT* object)
{
	namedattrbiutes.tag = object->Get(__L("GRPNAMED.tag"))->GETXQWORD();

	// need to get the string form the data area
	XDWORD dataindex = object->Get(__L("GRPNAMED.name"))->GETXDWORD();

	GGFDATA* data = file->data.Get(dataindex);

	// convert the data to a GGFSTRING and then XSTRING
	GGFSTRING ggfstr;
	ggfstr.ReadFrom(data->buffer);

	XSTRING name;
	ggfstr.ToXSTRING(&name);

	namedattrbiutes.name = name;

	return true;
}

/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::DefineGRPINTERFACEANIMABLE
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			24/11/2015 12:32:35
//	
//	@return 			bool : 
//
//  @param				definition : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::DefineGRPINTERFACEANIMABLE(GGFOBJECTDEFINITION* definition)
{
	return DefineGRPNAMED(definition);
}



/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::WriteGRPINTERFACEANIMABLE
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			24/11/2015 12:32:42
//	
//	@return 			bool : 
//
//  @param				materialattributes : 
//  @param				object : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::WriteGRPINTERFACEANIMABLE(GRPINTERFACEANIMABLE & materialattributes, GGFOBJECT* object)
{
	return WriteGRPNAMED(materialattributes, object);
}




/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::ReadGRPINTERFACEANIMABLE
*/	
/**	
//	
//	reads an interface animable
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			30/11/2015 10:16:00
//	
//	@return 			bool : 
//
//  @param				interfaceanimable : 
//  @param				object : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::ReadGRPINTERFACEANIMABLE(GRPINTERFACEANIMABLE& interfaceanimable, GGFOBJECT* object)
{
	ReadGRPNAMED(interfaceanimable, object);

	return true;
}



/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::DefineGRPATTACHEABLE
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			24/11/2015 12:32:47
//	
//	@return 			bool : 
//
//  @param				definition : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::DefineGRPATTACHEABLE(GGFOBJECTDEFINITION* definition)
{
	RegisterDefinition(__L("GRPATTACHEABLE.parent"), sizeof(void*), definition);
	RegisterDefinition(__L("GRPATTACHEABLE.IsOrphan"), sizeof(XBYTE), definition);
	RegisterDefinition(__L("GRPATTACHEABLE.Attachees"), sizeof(int), definition);
	RegisterDefinition(__L("GRPATTACHEABLE.IsAttacheable"), sizeof(XBYTE), definition);

	return true;
}




/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::WriteGRPATTACHEABLE
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			24/11/2015 12:32:54
//	
//	@return 			bool : 
//
//  @param				attacheable : 
//  @param				object : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::WriteGRPATTACHEABLE(GRPATTACHEABLE& attacheable, GGFOBJECT* object)
{
	GGFVALUE value;
	value.SetValue(attacheable.parent);
	object->Add(__L("GRPATTACHEABLE.parent"), &value);

	value.SetXBYTE(attacheable.IsOrphan);
	object->Add(__L("GRPATTACHEABLE.IsOrphan"), &value);

	value.SetValue(attacheable.Attachees);
	object->Add(__L("GRPATTACHEABLE.Attachees"), &value);

	value.SetXBYTE(attacheable.IsAttacheable);
	object->Add(__L("GRPATTACHEABLE.IsAttacheable"), &value);

	return true;
}





/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::ReadGRPATTACHEABLE
*/	
/**	
//	
//	reads an attacheable object
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			26/11/2015 12:40:48
//	
//	@return 			bool : 
//
//  @param				attacheable : 
//  @param				object : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::ReadGRPATTACHEABLE(GRPATTACHEABLE& attacheable, GGFOBJECT* object)
{
	attacheable.IsOrphan			= object->Get(__L("GRPATTACHEABLE.IsOrphan"))->GetBOOL();
	attacheable.IsAttacheable = object->Get(__L("GRPATTACHEABLE.IsAttacheable"))->GetBOOL();
	attacheable.Attachees			= object->Get(__L("GRPATTACHEABLE.Attachees"))->Getint();

	XQWORD ptr = 0;
	object->Get(__L("GRPATTACHEABLE.parent"))->buffer.Get((XBYTE*)&ptr, file->info.ptrsize, 0);

	GRPNODE* parent = (GRPNODE*)GetLoadedObject(ptr);

	if((ptr != 0) && (parent == NULL))
		{
			// parent recursive read
			//XQWORD index = file->seekmap.Find(ptr);
			XQWORD index = file->seekmap.seekmaptree.Get(ptr);
			if(index != NOTFOUND)
				{
					GGFOBJECT* obj = file->objects.Get((XDWORD)index);
					GRPNODE* p = ReadGRPNODEINHERITANCE(obj);
					attacheable.parent = p;
				}
			else
				{
					XDEBUG_PRINTCOLOR(4,__L("ERROR: looking for an object not present in seekmap"));
				}
		}
	else
		{
			attacheable.parent = (GRPNODE*)parent;
		}

	return true;
}


/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::DefineGRPELEMENT
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			24/11/2015 12:33:00
//	
//	@return 			bool : 
//
//  @param				definition : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::DefineGRPELEMENT(GGFOBJECTDEFINITION* definition)
{
	DefineGRPNAMED(definition);
	DefineGRPATTACHEABLE(definition);

	RegisterDefinition(__L("GRPELEMENT.node"), sizeof(void*), definition);
	RegisterDefinition(__L("GRPELEMENT.visibility"), sizeof(float), definition);
	RegisterDefinition(__L("GRPELEMENT.instance"), sizeof(void*), definition);

	RegisterDefinition(__L("GRPELEMENT.isinstance"), sizeof(XBYTE), definition);
	RegisterDefinition(__L("GRPELEMENT.isreferible"), sizeof(XBYTE), definition);

	// list of refereers

	RegisterDefinition(__L("GRPELEMENT.isloaded"), sizeof(XBYTE), definition);

	RegisterDefinition(__L("GRPELEMENT.deformer"), sizeof(void*), definition);
	RegisterDefinition(__L("GRPELEMENT.skin"), sizeof(void*), definition);

	RegisterDefinition(__L("GRPELEMENT.rendermode"), sizeof(GRPRENDERMODE), definition);

	RegisterDefinition(__L("GRPELEMENT.iswireframe"), sizeof(XBYTE), definition);

	RegisterDefinition(__L("GRPELEMENT.renderpriority"), sizeof(int), definition);
	RegisterDefinition(__L("GRPELEMENT.renderpass"), sizeof(int), definition);

	// Mesh information
	RegisterDefinition(__L("GRPELEMENT.nvertexperface"), sizeof(int), definition);
	RegisterDefinition(__L("GRPELEMENT.nvertexdata"), sizeof(XDWORD), definition);

	// into Data
	RegisterDefinition(__L("GRPELEMENT.vertexdata"), sizeof(XDWORD), definition);
	RegisterDefinition(__L("GRPELEMENT.indexdata"), sizeof(XDWORD), definition);

	// not data
	RegisterDefinition(__L("GRPELEMENT.nindexdata"), sizeof(XDWORD), definition);

	RegisterDefinition(__L("GRPELEMENT.IsProgram"), sizeof(XBYTE), definition);
	RegisterDefinition(__L("GRPELEMENT.Program"), 3*sizeof(char), definition);

	DefineGRPVERTEX(definition);

	// data
	//XVECTOR<GRPFACE*>								faces;
	//	XVECTOR<XDWORD>									indices;
	//	XDWORD													maxindex;

	return true;
}



/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::WriteGRPELEMENT
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			24/11/2015 12:33:06
//	
//	@return 			bool : 
//
//  @param				element : 
//  @param				object : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::WriteGRPELEMENT(GRPELEMENT& element, GGFOBJECT* object)
{
	if(!object->definition)
		{
			// search the definition of this object and assign it to the object
			XVECTOR<GGFOBJECTDEFINITION*>* definitions = file->GetDefinitions();
			GGFOBJECTDEFINITION* definition = NULL;
			for(XDWORD i = 0; i < definitions->GetSize(); i++)
				{
					if(definitions->FastGet(i)->id == __L("GRPNODE"))
						{
							definition = definitions->FastGet(i);
							break;
						}
				}

			object->definition = definition;
		}

	WriteGRPNAMED(element, object);
	WriteGRPATTACHEABLE(element, object);

	GGFVALUE value;
	value.SetValue(element.GetNode());
	object->Add(__L("GRPELEMENT.node"), &value);

	value.SetValue(element.GetVisibility());
	object->Add(__L("GRPELEMENT.visibility"), &value);

	value.SetValue(element.GetInstance());
	object->Add(__L("GRPELEMENT.instance"), &value);

	value.SetXBYTE(element.IsInstance());
	object->Add(__L("GRPELEMENT.isinstance"), &value);

	value.SetXBYTE(element.GetIsReferible());
	object->Add(__L("GRPELEMENT.isreferible"), &value);
	
	value.SetXBYTE(element.GetIsLoaded());
	object->Add(__L("GRPELEMENT.isloaded"), &value);

	value.SetValue(element.GetDeformer());
	object->Add(__L("GRPELEMENT.deformer"), &value);

	value.SetValue(element.GetSkin());
	object->Add(__L("GRPELEMENT.skin"), &value);
	
	value.SetValue(element.GetRenderMode());
	object->Add(__L("GRPELEMENT.rendermode"), &value);

	value.SetXBYTE(element.GetIsWireframe());
	object->Add(__L("GRPELEMENT.iswireframe"), &value);

	value.SetValue(element.GetRenderPriority());
	object->Add(__L("GRPELEMENT.renderpriority"), &value);

	value.SetValue(element.GetRenderPass());
	object->Add(__L("GRPELEMENT.renderpass"), &value);

	value.SetValue(element.GetMesh()->GetNvertexperface());
	object->Add(__L("GRPELEMENT.nvertexperface"), &value);

	value.SetValue(element.GetMesh()->GetNvertexData());
	object->Add(__L("GRPELEMENT.nvertexdata"), &value);

	value.SetBOOL(element.GetShader() != NULL);
	object->Add(__L("GRPELEMENT.IsProgram"), &value);

	if(element.GetShader())
	{
		value.SetCharString(element.GetShader()->name, 3);
		object->Add(__L("GRPELEMENT.Program"), &value);
	}

	// make an XBUFFER big enough to hold all of the vertex data
	// write it into the data section

	// vertex data

	WriteGRPVERTEX(element, element.GetMesh()->GetVertexData(), object, element.GetMesh()->GetNvertexData());

	// index data
	GGFDATA* data = new GGFDATA();

	data->buffer.Resize(element.GetMesh()->GetNindexData()*sizeof(XWORD));
	data->buffer.Set((XBYTE*)element.GetMesh()->GetIndexData(), element.GetMesh()->GetNindexData()*sizeof(XWORD));
	
	XDWORD index = file->AddData(data);
	value.SetValue(index);
	object->Add(__L("GRPELEMENT.indexdata"), &value);

	value.SetValue(element.GetMesh()->GetNindexData());
	object->Add(__L("GRPELEMENT.nindexdata"), &value);

	return true;
}



/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::ReadGRPELEMENT
*/	
/**	
//	
//	reads a grpelement
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			26/11/2015 17:56:12
//	
//	@return 			bool : 
//
//  @param				element : 
//  @param				object : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::ReadGRPELEMENT(GRPELEMENT& element, GGFOBJECT* object)
{
	ReadGRPNAMED(element, object);
	ReadGRPATTACHEABLE(element, object);

	GRPNODE* node = NULL;
	XQWORD ptr = 0;
	object->Get(__L("GRPELEMENT.node"))->buffer.Get((XBYTE*)&ptr, file->info.ptrsize, 0);
	node = (GRPNODE*)GetLoadedObject(ptr);


	bool isprogram = object->Get(__L("GRPELEMENT.IsProgram"))->GetBOOL();

	if(isprogram)
		{
			char key[3];
			object->Get(__L("GRPELEMENT.Program"))->GetCharString(key, 3);
			element.SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey(key));
		}
	

	// load the node form ggf if not in memory
	if((ptr != 0) && (node == NULL))
		{
			//XQWORD index = file->seekmap.Find(ptr);
			XQWORD index = file->seekmap.seekmaptree.Get(ptr);
			if(index == NOTFOUND) return false;

			GGFOBJECT* obj = file->objects.Get((XDWORD)index);
			node = new GRPNODE();
			if(!node) return false;
			ReadGRPNODE(*node, obj);

			if(!(obj->id == __L("GRPNODE")))
					{
						XDEBUG_PRINTCOLOR(4,__L("ERROR: Not a real GRPNODe"));
					}

			element.SetNode(node);
			loadedelements.Add(index, node);
			scene->Add(node);
		}
	else
		{
			element.SetNode(node);
		}

	element.SetVisibility(object->Get(__L("GRPELEMENT.visibility"))->Getfloat());

	ptr = 0;
	object->Get(__L("GRPELEMENT.instance"))->buffer.Get((XBYTE*)&ptr, file->info.ptrsize, 0);
	if(ptr == 0)
		{
			// do nothing?
			//element.IsInstance(false);
		}
	else
		{

			// check if instance exists, and load it if necessary
			GRPELEMENT* instance = (GRPELEMENT*)GetLoadedObject(ptr);
			if(!instance)
				{
					//XQWORD index = file->seekmap.Find(ptr);
					XQWORD index = file->seekmap.seekmaptree.Get(ptr);
					if(index == NOTFOUND) return false;
					GGFOBJECT* instanceobj = file->objects.Get((XDWORD)index);
					// need to load the instance
					instance = new GRPELEMENT();
					if(!instance) return false;
					ReadGRPELEMENT(*instance, instanceobj);
					// add to scene?
					loadedelements.Add(index, instance);
					scene->Add(instance);
				}
			
			element.Reference(instance);
		}
	//
	//// list of refereers
	//
	// as the object won't start loaded, no matter the previous state
	element.SetIsLoaded(false);

	// POINTER POINTER POINTER POINTER POINTER POINTER POINTER
	//RegisterDefinition(__L("GRPELEMENT.deformer"), sizeof(void*), definition);
	//RegisterDefinition(__L("GRPELEMENT.skin"), sizeof(void*), definition);
	//

	element.SetRenderMode(object->Get(__L("GRPELEMENT.rendermode"))->GetValue<GRPRENDERMODE>());
	element.SetWireframe(object->Get(__L("GRPELEMENT.iswireframe"))->GetBOOL());
	element.SetRenderPriority(object->Get(__L("GRPELEMENT.renderpriority"))->Getint());
	element.SetRenderPass(object->Get(__L("GRPELEMENT.renderpass"))->Getint());
	//
	//// Mesh information
	int nvertexface = object->Get(__L("GRPELEMENT.nvertexperface"))->Getint();
	XDWORD nvertexdata = object->Get(__L("GRPELEMENT.nvertexdata"))->GETXDWORD();

	// give another pointer as vertex data?
	// probably
	GRPVERTEX* vtx = new GRPVERTEX[nvertexdata];
	if(!vtx) return false;

	ReadGRPVERTEX(element, vtx, object, nvertexdata);

	// this maybe inside ReadGRPVERTEX ??
	element.SetNvertexData(nvertexdata);
	element.SetNVertex(nvertexdata);
	element.SetVertexData(vtx);

	// read indices
	XDWORD nindexdata = object->Get(__L("GRPELEMENT.nindexdata"))->GETXDWORD();
	element.SetNindexData(nindexdata);

	XWORD* indexdata = new XWORD[nindexdata];
	if(!indexdata) return false;
	// load index data from buffer
	XDWORD indexdataindex = object->Get(__L("GRPELEMENT.indexdata"))->GETXDWORD();
	GGFDATA* indexdatadata = file->data.Get(indexdataindex);
	indexdatadata->buffer.Get((XBYTE*)indexdata, nindexdata*sizeof(XWORD), 0);
	element.SetIndexData(indexdata);

	return true;
}


/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::DefineGRPVERTEX
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			24/11/2015 12:33:12
//	
//	@return 			bool : 
//
//  @param				definition : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::DefineGRPVERTEX(GGFOBJECTDEFINITION* definition)
{
	RegisterDefinition(__L("GRPVERTEX.position"), sizeof(XDWORD), definition);
	RegisterDefinition(__L("GRPVERTEX.normal"), sizeof(XDWORD), definition);

	RegisterDefinition(__L("GRPVERTEX.texturecoordinates"), sizeof(XDWORD), definition);
	RegisterDefinition(__L("GRPVERTEX.instanceID"), sizeof(float), definition);

	RegisterDefinition(__L("GRPVERTEX.positionMorph1"), sizeof(XDWORD), definition);
	RegisterDefinition(__L("GRPVERTEX.normalMorph1"), sizeof(XDWORD), definition);

	RegisterDefinition(__L("GRPVERTEX.positionMorph2"), sizeof(XDWORD), definition);
	RegisterDefinition(__L("GRPVERTEX.normalMorph2"), sizeof(XDWORD), definition);

	RegisterDefinition(__L("GRPVERTEX.positionMorph3"), sizeof(XDWORD), definition);
	RegisterDefinition(__L("GRPVERTEX.normalMorph3"), sizeof(XDWORD), definition);

	RegisterDefinition(__L("GRPVERTEX.positionMorph4"), sizeof(XDWORD), definition);
	RegisterDefinition(__L("GRPVERTEX.normalMorph4"), sizeof(XDWORD), definition);

	return true;
}



/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::WriteGRPVERTEX
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			24/11/2015 12:33:18
//	
//	@return 			bool : 
//
//  @param				element : 
//  @param				vertex : 
//  @param				object : 
//  @param				number : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::WriteGRPVERTEX(GRPELEMENT& element, GRPVERTEX* vertex, GGFOBJECT* object, XDWORD number)
{
	// allocate and one loop or not allocate and several loops?
	GGFDATA* position = new GGFDATA();
	position->buffer.Resize(element.GetNvertexData()*sizeof(float)*3);
	GGFDATA* normal = new GGFDATA();
	normal->buffer.Resize(element.GetNvertexData()*sizeof(float)*3);

	GGFDATA* texturecoordinates = new GGFDATA();
	texturecoordinates->buffer.Resize(element.GetNvertexData()*sizeof(float)*2);
	GGFDATA* instanceID = new GGFDATA();
	instanceID->buffer.Resize(element.GetNvertexData()*sizeof(float)*1);

	GGFDATA* positionMorph1 = new GGFDATA();
	positionMorph1->buffer.Resize(element.GetNvertexData()*sizeof(float)*3);
	GGFDATA* normalMorph1 = new GGFDATA();
	normalMorph1->buffer.Resize(element.GetNvertexData()*sizeof(float)*3);

	GGFDATA* positionMorph2 = new GGFDATA();
	positionMorph2->buffer.Resize(element.GetNvertexData()*sizeof(float)*3);
	GGFDATA* normalMorph2 = new GGFDATA();
	normalMorph2->buffer.Resize(element.GetNvertexData()*sizeof(float)*3);

	GGFDATA* positionMorph3 = new GGFDATA();
	positionMorph3->buffer.Resize(element.GetNvertexData()*sizeof(float)*3);
	GGFDATA* normalMorph3 = new GGFDATA();
	normalMorph3->buffer.Resize(element.GetNvertexData()*sizeof(float)*3);

	GGFDATA* positionMorph4 = new GGFDATA();
	positionMorph4->buffer.Resize(element.GetNvertexData()*sizeof(float)*3);
	GGFDATA* normalMorph4 = new GGFDATA();
	normalMorph4->buffer.Resize(element.GetNvertexData()*sizeof(float)*3);

	for(XDWORD i = 0; i < element.GetNvertexData(); i++)
		{
			GRPVERTEX* vtx = vertex + i;
			position->buffer.Set((XBYTE*)vtx->position, sizeof(float)*3);
			normal->buffer.Set((XBYTE*)vtx->normal, sizeof(float)*3);

			texturecoordinates->buffer.Set((XBYTE*)vtx->texturecoordinates, sizeof(float)*2);
//			instanceID->buffer.Set((XBYTE*)&(vtx->instanceID), sizeof(float)*1);

			#ifdef GRPDEFORMERS_ACTIVE

			positionMorph1->buffer.Set((XBYTE*)vtx->positionMorph1, sizeof(float)*3);
			//normalMorph1->buffer.Set((XBYTE*)vtx->normalMorph1, sizeof(float)*3);

			positionMorph2->buffer.Set((XBYTE*)vtx->positionMorph2, sizeof(float)*3);
			//normalMorph2->buffer.Set((XBYTE*)vtx->normalMorph2, sizeof(float)*3);

			positionMorph3->buffer.Set((XBYTE*)vtx->positionMorph3, sizeof(float)*3);
			//normalMorph3->buffer.Set((XBYTE*)vtx->normalMorph3, sizeof(float)*3);

			positionMorph4->buffer.Set((XBYTE*)vtx->positionMorph4, sizeof(float)*3);			
			//normalMorph4->buffer.Set((XBYTE*)vtx->normalMorph4, sizeof(float)*3);

			#endif

		}

	// add attributes to the data section
	GGFVALUE value;
	value.SetValue(file->AddData(position));
	object->Add(__L("GRPVERTEX.position"), &value);

	value.SetValue(file->AddData(normal));
	object->Add(__L("GRPVERTEX.normal"), &value);

	value.SetValue(file->AddData(texturecoordinates));
	object->Add(__L("GRPVERTEX.texturecoordinates"), &value);

	value.SetValue(file->AddData(instanceID));
	object->Add(__L("GRPVERTEX.instanceID"), &value);

	value.SetValue(file->AddData(positionMorph1));
	object->Add(__L("GRPVERTEX.positionMorph1"), &value);

	value.SetValue(file->AddData(normalMorph1));
	object->Add(__L("GRPVERTEX.normalMorph1"), &value);

	value.SetValue(file->AddData(positionMorph2));
	object->Add(__L("GRPVERTEX.positionMorph2"), &value);

	value.SetValue(file->AddData(normalMorph2));
	object->Add(__L("GRPVERTEX.normalMorph2"), &value);

	value.SetValue(file->AddData(positionMorph3));
	object->Add(__L("GRPVERTEX.positionMorph3"), &value);

	value.SetValue(file->AddData(normalMorph3));
	object->Add(__L("GRPVERTEX.normalMorph3"), &value);

	value.SetValue(file->AddData(positionMorph4));
	object->Add(__L("GRPVERTEX.positionMorph4"), &value);

	value.SetValue(file->AddData(normalMorph4));
	object->Add(__L("GRPVERTEX.normalMorph4"), &value);

	return true;
}



/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::ReadGRPVERTEX
*/	
/**	
//	
//	reads vertex data
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			26/11/2015 17:41:38
//	
//	@return 			bool : 
//
//  @param				element : 
//  @param				vertex : 
//  @param				object : 
//  @param				number : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::ReadGRPVERTEX(GRPELEMENT& element, GRPVERTEX* vertex, GGFOBJECT* object, XDWORD number)
{
	XDWORD positionindex						= object->Get(__L("GRPVERTEX.position"))->GETXDWORD();
	XDWORD normalindex							= object->Get(__L("GRPVERTEX.normal"))->GETXDWORD();
	
	XDWORD texturecoordinatesindex	= object->Get(__L("GRPVERTEX.texturecoordinates"))->GETXDWORD();
	XDWORD instanceIDindex					= object->Get(__L("GRPVERTEX.instanceID"))->GETXDWORD();
	
	XDWORD positionMorph1index			= object->Get(__L("GRPVERTEX.positionMorph1"))->GETXDWORD();
	XDWORD normalMorph1index				= object->Get(__L("GRPVERTEX.normalMorph1"))->GETXDWORD();
	
	XDWORD positionMorph2index			= object->Get(__L("GRPVERTEX.positionMorph2"))->GETXDWORD();
	XDWORD normalMorph2index				= object->Get(__L("GRPVERTEX.normalMorph2"))->GETXDWORD();
	
	XDWORD positionMorph3index			= object->Get(__L("GRPVERTEX.positionMorph3"))->GETXDWORD();
	XDWORD normalMorph3index				= object->Get(__L("GRPVERTEX.normalMorph3"))->GETXDWORD();

	XDWORD positionMorph4index			= object->Get(__L("GRPVERTEX.positionMorph4"))->GETXDWORD();
	XDWORD normalMorph4index				= object->Get(__L("GRPVERTEX.normalMorph4"))->GETXDWORD();

	GGFDATA* positiondata							= file->data.Get(positionindex);
	GGFDATA* normaldata								= file->data.Get(normalindex);
	GGFDATA* texturecoordinatesdata		= file->data.Get(texturecoordinatesindex);
	GGFDATA* instanceIDdata						= file->data.Get(instanceIDindex);
	GGFDATA* positionMorph1data				= file->data.Get(positionMorph1index);
	GGFDATA* normalMorph1data					= file->data.Get(normalMorph1index);
	GGFDATA* positionMorph2data				= file->data.Get(positionMorph2index);
	GGFDATA* normalMorph2data					= file->data.Get(normalMorph2index);
	GGFDATA* positionMorph3data				= file->data.Get(positionMorph3index);
	GGFDATA* normalMorph3data					= file->data.Get(normalMorph3index);
	GGFDATA* positionMorph4data				= file->data.Get(positionMorph4index);
	GGFDATA* normalMorph4data					= file->data.Get(normalMorph4index);

	positiondata->buffer.SetPosition(0);
	normaldata->buffer.SetPosition(0);
	texturecoordinatesdata->buffer.SetPosition(0);
	instanceIDdata->buffer.SetPosition(0);
	positionMorph1data->buffer.SetPosition(0);
	normalMorph1data->buffer.SetPosition(0);
	positionMorph2data->buffer.SetPosition(0);
	normalMorph2data->buffer.SetPosition(0);
	positionMorph3data->buffer.SetPosition(0);
	normalMorph3data->buffer.SetPosition(0);
	positionMorph4data->buffer.SetPosition(0);
	normalMorph4data->buffer.SetPosition(0);

	for(XDWORD i = 0; i < number; i++)
		{
			// put every piece of data into their vertices
			GRPVERTEX* vtx = vertex + i;
			positiondata->buffer.Get((XBYTE*)vtx->position, sizeof(float)*3);
			normaldata->buffer.Get((XBYTE*)vtx->normal, sizeof(float)*3);
		
			texturecoordinatesdata->buffer.Get((XBYTE*)vtx->texturecoordinates, sizeof(float)*2);
		//	instanceIDdata->buffer.Get((XBYTE*)&(vtx->instanceID), sizeof(float)*1);

		#ifdef GRPDEFORMERS_ACTIVE

			positionMorph1data->buffer.Get((XBYTE*)vtx->positionMorph1, sizeof(float)*3);
			//normalMorph1data->buffer.Get((XBYTE*)vtx->normalMorph1, sizeof(float)*3);
		
			positionMorph2data->buffer.Get((XBYTE*)vtx->positionMorph2, sizeof(float)*3);
			//normalMorph2data->buffer.Get((XBYTE*)vtx->normalMorph2, sizeof(float)*3);
		
			positionMorph3data->buffer.Get((XBYTE*)vtx->positionMorph3, sizeof(float)*3);
			//normalMorph3data->buffer.Get((XBYTE*)vtx->normalMorph3, sizeof(float)*3);
		
			positionMorph4data->buffer.Get((XBYTE*)vtx->positionMorph4, sizeof(float)*3);
			//normalMorph4data->buffer.Get((XBYTE*)vtx->normalMorph4, sizeof(float)*3);
		#endif
		}

	return true;
}




/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::DefineGRPLIGHT
*/	
/**	
//	
//	defines a light
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			27/11/2015 14:21:25
//	
//	@return 			bool : 
//
//  @param				definition : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::DefineGRPLIGHT(GGFOBJECTDEFINITION* definition)
{
	DefineGRPNODE(definition);

	RegisterDefinition(__L("GRPLIGHT.target"), sizeof(void*), definition);
	RegisterDefinition(__L("GRPLIGHT.DecayType"), sizeof(GRPLIGHTDECAYTYPE), definition);

	RegisterDefinition(__L("GRPLIGHT.Type"), sizeof(GRPLIGHTTYPE), definition);
	RegisterDefinition(__L("GRPLIGHT.Direction"), 3*sizeof(float), definition);

	RegisterDefinition(__L("GRPLIGHT.Strength"), sizeof(float), definition);
	RegisterDefinition(__L("GRPLIGHT.Multiplier"), sizeof(float), definition);

	RegisterDefinition(__L("GRPLIGHT.ConstantAttenuation"), sizeof(float), definition);
	RegisterDefinition(__L("GRPLIGHT.LinearAttenuation"), sizeof(float), definition);
	RegisterDefinition(__L("GRPLIGHT.QuadraticAttenuation"), sizeof(float), definition);
	RegisterDefinition(__L("GRPLIGHT.CubicAttenuation"), sizeof(float), definition);

	RegisterDefinition(__L("GRPLIGHT.Diffuse"), 4*sizeof(float), definition);
	RegisterDefinition(__L("GRPLIGHT.Ambient"), 4*sizeof(float), definition);
	RegisterDefinition(__L("GRPLIGHT.Specular"), 4*sizeof(float), definition);

	RegisterDefinition(__L("GRPLIGHT.SpotInner"), sizeof(float), definition);
	RegisterDefinition(__L("GRPLIGHT.SpotOuter"), sizeof(float), definition);

	RegisterDefinition(__L("GRPLIGHT.LightScaleFactor"), sizeof(float), definition);

	return true;
}





/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::WriteGRPLIGHT
*/	
/**	
//	
//	writes a light
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			27/11/2015 14:23:25
//	
//	@return 			bool : 
//
//  @param				light : 
//  @param				object : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::WriteGRPLIGHT(GRPLIGHT& light, GGFOBJECT* object)
{
	WriteGRPNODE(light, object);

	GGFVALUE value;
	value.SetValue(light.GetTarget());
	object->Add(__L("GRPLIGHT.target"), &value);


	value.SetValue(light.GetDecayType());
	object->Add(__L("GRPLIGHT.DecayType"), &value);


	value.SetValue(light.GetType());
	object->Add(__L("GRPLIGHT.Type"), &value);

	value.SetGRPVECTOR(*light.GetDirection());
	object->Add(__L("GRPLIGHT.Direction"), &value);


	value.SetValue(light.GetStrength());
	object->Add(__L("GRPLIGHT.Strength"), &value);

	value.SetValue(light.GetMultiplier());
	object->Add(__L("GRPLIGHT.Multiplier"), &value);


	value.SetValue(light.GetConstantAttenuation());
	object->Add(__L("GRPLIGHT.ConstantAttenuation"), &value);

	value.SetValue(light.GetLinearAttenuation());
	object->Add(__L("GRPLIGHT.LinearAttenuation"), &value);

	value.SetValue(light.GetQuadraticAttenuation());
	object->Add(__L("GRPLIGHT.QuadraticAttenuation"), &value);

	value.SetValue(light.GetCubicAttenuation());
	object->Add(__L("GRPLIGHT.CubicAttenuation"), &value);
	

	value.SetGRPCOLOR(light.GetDiffuse());
	object->Add(__L("GRPLIGHT.Diffuse"), &value);

	value.SetGRPCOLOR(light.GetAmbient());
	object->Add(__L("GRPLIGHT.Ambient"), &value);

	value.SetGRPCOLOR(light.GetSpecular());
	object->Add(__L("GRPLIGHT.Specular"), &value);
	
	
	value.SetValue(light.GetInnerSpot());
	object->Add(__L("GRPLIGHT.SpotInner"), &value);
	
	value.SetValue(light.GetOuterSpot());
	object->Add(__L("GRPLIGHT.SpotOuter"), &value);
	

	value.SetValue(light.GetScaleFactor());
	object->Add(__L("GRPLIGHT.LightScaleFactor"), &value);

	return true;
}




/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::ReadGRPLIGHT
*/	
/**	
//	
//	reads a light
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			27/11/2015 16:32:26
//	
//	@return 			bool : 
//
//  @param				light : 
//  @param				object : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::ReadGRPLIGHT(GRPLIGHT& light, GGFOBJECT* object)
{
	ReadGRPNODE(light, object);

	GRPNODE* node = NULL;
	XQWORD ptr = 0;
	object->Get(__L("GRPLIGHT.target"))->buffer.Get((XBYTE*)&ptr, file->info.ptrsize, 0);
	node = (GRPNODE*)GetLoadedObject(ptr);

	// load the node form ggf if not in memory
	if((ptr != 0) && (node == NULL))
		{
			//XQWORD index = file->seekmap.Find(ptr);
			XQWORD index = file->seekmap.seekmaptree.Get(ptr);
			if(index == NOTFOUND) return false;

			GGFOBJECT* obj = file->objects.Get((XDWORD)index);
			node = new GRPNODE();
			if(!node) return false;
			ReadGRPNODE(*node, obj);

			if(!(obj->id == __L("GRPNODE")))
					{
						XDEBUG_PRINTCOLOR(4,__L("ERROR: Not a real GRPNODe"));
					}

			light.SetTarget(node);
			loadedelements.Add(index, node);
			scene->Add(node);
		}
	else
		{
			light.SetTarget(node);
		}

	light.SetDecayType(object->Get(__L("GRPLIGHT.DecayType"))->GetValue<GRPLIGHTDECAYTYPE>());

	light.SetType(object->Get(__L("GRPLIGHT.Type"))->GetValue<GRPLIGHTTYPE>());
	
	GRPVECTOR v = object->Get(__L("GRPLIGHT.Direction"))->GetGRPVECTOR();

	light.SetDirection(v);

	light.SetStrength(object->Get(__L("GRPLIGHT.Strength"))->Getfloat());

	light.SetMultiplier(object->Get(__L("GRPLIGHT.Multiplier"))->Getfloat());

	float cn, l, q, c = 0;
	cn = object->Get(__L("GRPLIGHT.ConstantAttenuation"))->Getfloat();
	l = object->Get(__L("GRPLIGHT.LinearAttenuation"))->Getfloat();
	q = object->Get(__L("GRPLIGHT.QuadraticAttenuation"))->Getfloat();
	c = object->Get(__L("GRPLIGHT.CubicAttenuation"))->Getfloat();
	light.SetAttenuation(cn, l, q, c);

	GRPCOLOR color = object->Get(__L("GRPLIGHT.Diffuse"))->GetGRPCOLOR();
	light.SetDiffuse(color);

	color = object->Get(__L("GRPLIGHT.Ambient"))->GetGRPCOLOR();
	light.SetAmbient(color);

	color = object->Get(__L("GRPLIGHT.Specular"))->GetGRPCOLOR();
	light.SetSpecular(color);

	light.SetInnerSpot(object->Get(__L("GRPLIGHT.SpotInner"))->Getfloat());

	light.SetOuterSpot(object->Get(__L("GRPLIGHT.SpotOuter"))->Getfloat());

	light.SetScaleFactor(object->Get(__L("GRPLIGHT.LightScaleFactor"))->Getfloat());

	return true;
}



/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::DefineGRPVIEWPORT
*/	
/**	
//	
//	defines a viewport
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			25/11/2015 10:21:14
//	
//	@return 			bool : 
//
//  @param				definition : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::DefineGRPVIEWPORT(GGFOBJECTDEFINITION* definition)
{
	RegisterDefinition(__L("GRPVIEWPORT.x"), sizeof(float), definition);
	RegisterDefinition(__L("GRPVIEWPORT.y"), sizeof(float), definition);

	RegisterDefinition(__L("GRPVIEWPORT.width"), sizeof(float), definition);
	RegisterDefinition(__L("GRPVIEWPORT.height"), sizeof(float), definition);

	RegisterDefinition(__L("GRPVIEWPORT.pixelwidth"), sizeof(float), definition);
	RegisterDefinition(__L("GRPVIEWPORT.pixelheight"), sizeof(float), definition);

	RegisterDefinition(__L("GRPVIEWPORT.realpixelwidth"), sizeof(float), definition);
	RegisterDefinition(__L("GRPVIEWPORT.realpixelheight"), sizeof(float), definition);

	RegisterDefinition(__L("GRPVIEWPORT.projectiontype"), sizeof(GRPPROJECTIONTYPE), definition);
	RegisterDefinition(__L("GRPVIEWPORT.projectionfrustumnear"), sizeof(float), definition);
	RegisterDefinition(__L("GRPVIEWPORT.projectionfrustumfar"), sizeof(float), definition);

	RegisterDefinition(__L("GRPVIEWPORT.fieldofview"), sizeof(float), definition);
	RegisterDefinition(__L("GRPVIEWPORT.AspectRatio"), sizeof(float), definition);

	RegisterDefinition(__L("GRPVIEWPORT.forceaspectratio"), sizeof(XBYTE), definition);
	RegisterDefinition(__L("GRPVIEWPORT.forcedaspectratio"), sizeof(float), definition);

	return true;
}



/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::WriteGRPVIEWPORT
*/	
/**	
//	
//	writes a viewport
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			25/11/2015 11:08:09
//	
//	@return 			bool : 
//
//  @param				viewport : 
//  @param				object : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::WriteGRPVIEWPORT(GRPVIEWPORT& viewport, GGFOBJECT* object)
{
	XSTRING str;
	str.Set(__L("GRPVIEWPORT"));
	object->id.FromXSTRING(&str);
	object->tag = (XQWORD)&viewport;

	float x, y;
	viewport.GetPosition(x, y);

	GGFVALUE value;
	value.SetValue(x);
	object->Add(__L("GRPVIEWPORT.x"), &value);

	value.SetValue(y);
	object->Add(__L("GRPVIEWPORT.y"), &value);

	viewport.GetSize(x, y);
	value.SetValue(x);
	object->Add(__L("GRPVIEWPORT.width"), &value);

	value.SetValue(y);
	object->Add(__L("GRPVIEWPORT.height"), &value);

	viewport.GetWindowSize(x, y);
	value.SetValue(x);
	object->Add(__L("GRPVIEWPORT.pixelwidth"), &value);

	value.SetValue(y);
	object->Add(__L("GRPVIEWPORT.pixelheight"), &value);

	viewport.GetRealWindowSize(x, y);
	value.SetValue(x);
	object->Add(__L("GRPVIEWPORT.realpixelwidth"), &value);

	value.SetValue(y);
	object->Add(__L("GRPVIEWPORT.realpixelheight"), &value);

	value.SetValue(viewport.GetProjection());
	object->Add(__L("GRPVIEWPORT.projectiontype"), &value);

	viewport.GetProjectionFrustum(x, y);
	value.SetValue(x);
	object->Add(__L("GRPVIEWPORT.projectionfrustumnear"), &value);

	value.SetValue(y);
	object->Add(__L("GRPVIEWPORT.projectionfrustumfar"), &value);

	value.SetValue(viewport.GetFieldOfView());
	object->Add(__L("GRPVIEWPORT.fieldofview"), &value);

	value.SetValue(viewport.GetAspectRatio());
	object->Add(__L("GRPVIEWPORT.AspectRatio"), &value);

	value.SetBOOL(viewport.IsForcedAspectRatio());
	object->Add(__L("GRPVIEWPORT.forceaspectratio"), &value);

	value.SetValue(viewport.GetForcedAspectRatio());
	object->Add(__L("GRPVIEWPORT.forcedaspectratio"), &value);

	return true;
}




/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::ReadGRPVIEWPORT
*/	
/**	
//	
//	reads a grpviewport
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			25/11/2015 12:17:33
//	
//	@return 			bool : 
//
//  @param				viewport : 
//  @param				object : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::ReadGRPVIEWPORT(GRPVIEWPORT& viewport, GGFOBJECT* object)
{
	float x									= object->Get(__L("GRPVIEWPORT.x"))->Getfloat();
	float y									= object->Get(__L("GRPVIEWPORT.y"))->Getfloat();
	viewport.SetPosition(x, y);

	float width							= object->Get(__L("GRPVIEWPORT.width"))->Getfloat();
	float height						= object->Get(__L("GRPVIEWPORT.height"))->Getfloat();
	viewport.SetSize(width, height);

	float pixelwidth				= object->Get(__L("GRPVIEWPORT.pixelwidth"))->Getfloat();
	float pixelheight				= object->Get(__L("GRPVIEWPORT.pixelheight"))->Getfloat();
	viewport.SetWindowSize(pixelwidth, pixelheight);

	float realpixelwidth		= object->Get(__L("GRPVIEWPORT.realpixelwidth"))->Getfloat();
	float realpixelheight		= object->Get(__L("GRPVIEWPORT.realpixelheight"))->Getfloat();
	viewport.SetWindowSize(realpixelwidth, realpixelheight);

	GRPPROJECTIONTYPE type	= object->Get(__L("GRPVIEWPORT.projectiontype"))->GetValue<GRPPROJECTIONTYPE>();
	viewport.SetProjection(type);

	float nearfrustum				= object->Get(__L("GRPVIEWPORT.projectionfrustumnear"))->Getfloat();
	float farfrustum				= object->Get(__L("GRPVIEWPORT.projectionfrustumfar"))->Getfloat();
	viewport.SetProjectionFrustum(nearfrustum, farfrustum);

	float fov								= object->Get(__L("GRPVIEWPORT.fieldofview"))->Getfloat();
	viewport.SetFieldOfView(fov);

	float aspectratio				= object->Get(__L("GRPVIEWPORT.AspectRatio"))->Getfloat();
	viewport.SetAspectRatio(aspectratio);

	bool forceaspectratio		= object->Get(__L("GRPVIEWPORT.forceaspectratio"))->GetBOOL();
	float forcedaspectratio = object->Get(__L("GRPVIEWPORT.forcedaspectratio"))->Getfloat();
	viewport.ForceAspectRatio(forceaspectratio, forcedaspectratio);

	return true;
}

/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::DefineGRPCAMERA
*/	
/**	
//	
//	defines a camera
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			25/11/2015 10:47:38
//	
//	@return 			bool : 
//
//  @param				definition : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::DefineGRPCAMERA(GGFOBJECTDEFINITION* definition)
{
	DefineGRPNODE(definition);

	RegisterDefinition(__L("GRPCAMERA.Type"), sizeof(GRPCAMERA_TYPE), definition);

	RegisterDefinition(__L("GRPCAMERA.interestposition"), 3*sizeof(float), definition);

	RegisterDefinition(__L("GRPCAMERA.AspectWidth"), sizeof(float), definition);
	RegisterDefinition(__L("GRPCAMERA.AspectHeight"), sizeof(float), definition);
	RegisterDefinition(__L("GRPCAMERA.FilmWidth"), sizeof(float), definition);
	RegisterDefinition(__L("GRPCAMERA.FilmHeight"), sizeof(float), definition);

	RegisterDefinition(__L("GRPCAMERA.NearPlane"), sizeof(float), definition);
	RegisterDefinition(__L("GRPCAMERA.FarPlane"), sizeof(float), definition);

	RegisterDefinition(__L("GRPCAMERA.fov"), sizeof(float), definition);
	RegisterDefinition(__L("GRPCAMERA.lens"), sizeof(float), definition);

	RegisterDefinition(__L("GRPCAMERA.FocalLength"), sizeof(float), definition);

	RegisterDefinition(__L("GRPCAMERA.BackPlaneDistance"), sizeof(float), definition);
	RegisterDefinition(__L("GRPCAMERA.BackPlaneScaleX"), sizeof(float), definition);
	RegisterDefinition(__L("GRPCAMERA.BackPlaneScaleY"), sizeof(float), definition);

	RegisterDefinition(__L("GRPCAMERA.UseFocalLength"), sizeof(XBYTE), definition);

	RegisterDefinition(__L("GRPCAMERA.ImagePlane"), sizeof(void*), definition);
	RegisterDefinition(__L("GRPCAMERA.target"), sizeof(void*), definition);
	RegisterDefinition(__L("GRPCAMERA.upnode"), sizeof(void*), definition);

	RegisterDefinition(__L("GRPCAMERA.l"), 3*sizeof(float), definition);

	RegisterDefinition(__L("GRPCAMERA.x"), 3*sizeof(float), definition);
	RegisterDefinition(__L("GRPCAMERA.y"), 3*sizeof(float), definition);
	RegisterDefinition(__L("GRPCAMERA.z"), 3*sizeof(float), definition);

	RegisterDefinition(__L("GRPCAMERA.eye"), 3*sizeof(float), definition);
	RegisterDefinition(__L("GRPCAMERA.obj"), 3*sizeof(float), definition);
	RegisterDefinition(__L("GRPCAMERA.up"), 3*sizeof(float), definition);

	return true;
}



/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::WriteGRPCAMERA
*/	
/**	
//	
//	writes the camera
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			25/11/2015 11:59:34
//	
//	@return 			bool : 
//
//  @param				camera : 
//  @param				object : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::WriteGRPCAMERA(GRPCAMERA& camera, GGFOBJECT* object)
{
	// base class
	WriteGRPNODE(camera, object);

	GGFVALUE value;
	value.SetValue(camera.Type);
	object->Add(__L("GRPCAMERA.Type"), &value);

	value.SetGRPVECTOR(camera.interestposition);
	object->Add(__L("GRPCAMERA.interestposition"), &value);

	value.SetValue(camera.AspectWidth);
	object->Add(__L("GRPCAMERA.AspectWidth"), &value);

	value.SetValue(camera.AspectHeight);
	object->Add(__L("GRPCAMERA.AspectHeight"), &value);

	value.SetValue(camera.FilmWidth);
	object->Add(__L("GRPCAMERA.FilmWidth"), &value);

	value.SetValue(camera.FilmHeight);
	object->Add(__L("GRPCAMERA.FilmHeight"), &value);

	value.SetValue(camera.NearPlane);
	object->Add(__L("GRPCAMERA.NearPlane"), &value);

	value.SetValue(camera.FarPlane);
	object->Add(__L("GRPCAMERA.FarPlane"), &value);

	value.SetValue(camera.fov);
	object->Add(__L("GRPCAMERA.fov"), &value);

	value.SetValue(camera.lens);
	object->Add(__L("GRPCAMERA.lens"), &value);

	value.SetValue(camera.FocalLength);
	object->Add(__L("GRPCAMERA.FocalLength"), &value);

	value.SetValue(camera.BackPlaneDistance);
	object->Add(__L("GRPCAMERA.BackPlaneDistance"), &value);

	value.SetValue(camera.BackPlaneScaleX);
	object->Add(__L("GRPCAMERA.BackPlaneScaleX"), &value);

	value.SetValue(camera.BackPlaneScaleY);
	object->Add(__L("GRPCAMERA.BackPlaneScaleY"), &value);

	value.SetBOOL(camera.UseFocalLength);
	object->Add(__L("GRPCAMERA.UseFocalLength"), &value);

	value.SetValue(camera.ImagePlane);
	object->Add(__L("GRPCAMERA.ImagePlane"), &value);

	value.SetValue(camera.target);
	object->Add(__L("GRPCAMERA.target"), &value);

	value.SetValue(camera.upnode);
	object->Add(__L("GRPCAMERA.upnode"), &value);

	value.SetGRPVECTOR(camera.l);
	object->Add(__L("GRPCAMERA.l"), &value);

	value.SetGRPVECTOR(camera.z);
	object->Add(__L("GRPCAMERA.z"), &value);

	value.SetGRPVECTOR(camera.x);
	object->Add(__L("GRPCAMERA.x"), &value);

	value.SetGRPVECTOR(camera.y);
	object->Add(__L("GRPCAMERA.y"), &value);

	value.SetGRPVECTOR(camera.eye);
	object->Add(__L("GRPCAMERA.eye"), &value);

	value.SetGRPVECTOR(camera.obj);
	object->Add(__L("GRPCAMERA.obj"), &value);

	value.SetGRPVECTOR(camera.up);
	object->Add(__L("GRPCAMERA.up"), &value);

	return true;
}




/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::ReadGRPCAMERA
*/	
/**	
//	
//	reads the camera
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			26/11/2015 13:57:33
//	
//	@return 			bool : 
//
//  @param				camera : 
//  @param				object : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::ReadGRPCAMERA(GRPCAMERA& camera, GGFOBJECT* object)
{
	ReadGRPNODE(camera, object);

	camera.Type								= object->Get(__L("GRPCAMERA.Type"))->GetValue<GRPCAMERA_TYPE>();

	camera.interestposition		= object->Get(__L("GRPCAMERA.Type"))->GetGRPVECTOR();

	camera.AspectWidth				= object->Get(__L("GRPCAMERA.AspectWidth"))->Getfloat();
	camera.AspectHeight				= object->Get(__L("GRPCAMERA.AspectHeight"))->Getfloat();
	camera.FilmWidth					= object->Get(__L("GRPCAMERA.FilmWidth"))->Getfloat();
	camera.FilmHeight					= object->Get(__L("GRPCAMERA.FilmHeight"))->Getfloat();

	camera.NearPlane					= object->Get(__L("GRPCAMERA.NearPlane"))->Getfloat();
	camera.FarPlane						= object->Get(__L("GRPCAMERA.FarPlane"))->Getfloat();

	camera.fov								= object->Get(__L("GRPCAMERA.fov"))->Getfloat();
	camera.lens								= object->Get(__L("GRPCAMERA.lens"))->Getfloat();

	camera.FocalLength				= object->Get(__L("GRPCAMERA.FocalLength"))->Getfloat();

	camera.BackPlaneDistance	= object->Get(__L("GRPCAMERA.BackPlaneDistance"))->Getfloat();
	camera.BackPlaneScaleX		= object->Get(__L("GRPCAMERA.BackPlaneScaleX"))->Getfloat();
	camera.BackPlaneScaleY		= object->Get(__L("GRPCAMERA.BackPlaneScaleY"))->Getfloat();

	camera.UseFocalLength			= object->Get(__L("GRPCAMERA.UseFocalLength"))->GetBOOL();

	// POINTERS!!
	//camera.ImagePlane = object->Get(__L("GRPCAMERA.ImagePlane"))->GetValue<GRPCAMERA_TYPE>();
	camera.ImagePlane = NULL; // not reading it for now

	//camera.target = object->Get(__L("GRPCAMERA.target"))->GetValue<GRPCAMERA_TYPE>();
	GRPNODE* node = NULL;
	XQWORD ptr = 0;
	object->Get(__L("GRPCAMERA.target"))->buffer.Get((XBYTE*)&ptr, file->info.ptrsize, 0);
	node = (GRPNODE*)GetLoadedObject(ptr);

	// load the node form ggf if not in memory
	if((ptr != 0) && (node == NULL))
		{
			//XQWORD index = file->seekmap.Find(ptr);
			XQWORD index = file->seekmap.seekmaptree.Get(ptr);
			if(index == NOTFOUND) return false;

			GGFOBJECT* obj = file->objects.Get((XDWORD)index);
			node = new GRPNODE();
			if(!node) return false;
			ReadGRPNODE(*node, obj);

			if(!(obj->id == __L("GRPNODE")))
					{
						XDEBUG_PRINTCOLOR(4,__L("ERROR: Not a real GRPNODe"));
					}

			camera.SetTarget(node);
			loadedelements.Add(index, node);
			scene->Add(node);
		}
	else
		{
			camera.SetTarget(node);
		}


	//camera.upnode = object->Get(__L("GRPCAMERA.upnode"))->GetValue<GRPCAMERA_TYPE>();
	node = NULL;
	ptr = 0;
	object->Get(__L("GRPCAMERA.upnode"))->buffer.Get((XBYTE*)&ptr, file->info.ptrsize, 0);
	node = (GRPNODE*)GetLoadedObject(ptr);

	// load the node form ggf if not in memory
	if((ptr != 0) && (node == NULL))
		{
			//XQWORD index = file->seekmap.Find(ptr);
			XQWORD index = file->seekmap.seekmaptree.Get(ptr);
			if(index == NOTFOUND) return false;

			GGFOBJECT* obj = file->objects.Get((XDWORD)index);
			node = new GRPNODE();
			if(!node) return false;
			ReadGRPNODE(*node, obj);

			if(!(obj->id == __L("GRPNODE")))
					{
						XDEBUG_PRINTCOLOR(4,__L("ERROR: Not a real GRPNODe"));
					}

			camera.SetUpNode(node);
			loadedelements.Add(index, node);
			scene->Add(node);
		}
	else
		{
			camera.SetUpNode(node);
		}


	camera.l									= object->Get(__L("GRPCAMERA.l"))->GetGRPVECTOR();

	camera.x									= object->Get(__L("GRPCAMERA.x"))->GetGRPVECTOR();
	camera.y									= object->Get(__L("GRPCAMERA.y"))->GetGRPVECTOR();
	camera.z									= object->Get(__L("GRPCAMERA.z"))->GetGRPVECTOR();

	camera.eye								= object->Get(__L("GRPCAMERA.eye"))->GetGRPVECTOR();
	camera.obj								= object->Get(__L("GRPCAMERA.obj"))->GetGRPVECTOR();
	camera.up									= object->Get(__L("GRPCAMERA.up"))->GetGRPVECTOR();

	return true;
}



bool GRPSERIALIZERGGF::DefineGRPANIMATIONCONTROL(GGFOBJECTDEFINITION* definition)
{
	RegisterDefinition(__L("GRPANIMATIONCONTROL.controltimeFPS"), sizeof(XDWORD), definition);
	RegisterDefinition(__L("GRPANIMATIONCONTROL.controltimeprotocoltype"), sizeof(XDWORD), definition);
	return true;
}

bool GRPSERIALIZERGGF::WriteGRPANIMATIONCONTROL(GRPANIMATIONCONTROL& control, GGFOBJECT* object)
{
	GGFVALUE value;

	value.SetXDWORD(control.GetControlTimeFPS());
	object->Add(__L("GRPANIMATIONCONTROL.controltimeFPS"), &value);

	value.SetXDWORD(control.GetControlTimeProtocolType());
	object->Add(__L("GRPANIMATIONCONTROL.controltimeprotocoltype"), &value);

	return true;
}

bool GRPSERIALIZERGGF::ReadGRPANIMATIONCONTROL(GRPANIMATIONCONTROL& control, GGFOBJECT* object)
{
	control.SetControlTimeFPS((GRPANIMATIONCONTROLTIMEFPS)object->Get(__L("GRPANIMATIONCONTROL.controltimeFPS"))->GETXDWORD());
	control.SetControlTimeProtocolType((GRPANIMATIONCONTROLTIMEPROTOCOLTYPE)object->Get(__L("GRPANIMATIONCONTROL.controltimeprotocoltype"))->GETXDWORD());

	return true;
}





/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::DefineGRPANIMATIONSTACK
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/12/2015 11:27:42
//	
//	@return 			bool : 
//
//  @param				definition : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::DefineGRPANIMATIONSTACK(GGFOBJECTDEFINITION* definition)
{
	DefineGRPNAMED(definition);

	RegisterDefinition(__L("GRPANIMATIONSTACK.layer"), sizeof(void*), definition);

	RegisterDefinition(__L("GRPANIMATIONSTACK.maxtime"), sizeof(XQWORD), definition);
	RegisterDefinition(__L("GRPANIMATIONSTACK.mintime"), sizeof(XQWORD), definition);
	RegisterDefinition(__L("GRPANIMATIONSTACK.minframes"), sizeof(XQWORD), definition);
	RegisterDefinition(__L("GRPANIMATIONSTACK.currentlayer"), sizeof(XQWORD), definition);
	RegisterDefinition(__L("GRPANIMATIONSTACK.status"), sizeof(XDWORD), definition);
	RegisterDefinition(__L("GRPANIMATIONSTACK.active"), sizeof(XBYTE), definition);

	RegisterDefinition(__L("GRPANIMATIONSTACK.stackFrame"), sizeof(XQWORD), definition);
	RegisterDefinition(__L("GRPANIMATIONSTACK.laststackFrame"), sizeof(XQWORD), definition);
	RegisterDefinition(__L("GRPANIMATIONSTACK.synced"), sizeof(XBYTE), definition);

	// anijmation control pointer should be placed automatically, no need to save it

	return true;
}



/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::WriteGRPANIMATIONSTACK
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/12/2015 12:39:54
//	
//	@return 			bool : 
//
//  @param				stack : 
//  @param				object : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::WriteGRPANIMATIONSTACK(GRPANIMATIONSTACK& stack, GGFOBJECT* object)
{
	WriteGRPNAMED(stack, object);
	//write pointers
	GGFVALUE value;
	for(XDWORD i = 0; i < stack.GetLayers()->GetSize(); i++)
		{
			value.SetValue(stack.GetLayers()->FastGet(i));
			object->Add(__L("GRPANIMATIONSTACK.layer"), &value);
		}

	value.SetValue(stack.GetMaxTime());
	object->Add(__L("GRPANIMATIONSTACK.maxtime"), &value);
	
	value.SetValue(stack.GetMinTime());
	object->Add(__L("GRPANIMATIONSTACK.mintime"), &value);

	value.SetValue(stack.GetMinFrame());
	object->Add(__L("GRPANIMATIONSTACK.minframes"), &value);

	value.SetValue(stack.GetCurrentLayer());
	object->Add(__L("GRPANIMATIONSTACK.currentlayer"), &value);

	//RegisterDefinition(__L("GRPANIMATIONSTACK.status"), sizeof(XDWORD), definition);
	// this is a string for the enum, need a simpler way of writing/reading strings
	//value.SetValue(stack.GetMinFrame());
	//object->Add(__L("GRPANIMATIONSTACK.status"), &value);
	GGFSTRING* enumvalue = GetStatusKey(stack.GetStatus());
	WriteString(object, __L("GRPANIMATIONSTACK.status"), enumvalue);

	value.SetBOOL(stack.IsActive());
	bool active = stack.IsActive();
	object->Add(__L("GRPANIMATIONSTACK.active"), &value);

	value.SetValue(stack.GetStackFrame());
	object->Add(__L("GRPANIMATIONSTACK.stackFrame"), &value);

	value.SetValue(stack.GetLastStackFrame());
	object->Add(__L("GRPANIMATIONSTACK.laststackFrame"), &value);

	value.SetValue(stack.IsSynced());
	object->Add(__L("GRPANIMATIONSTACK.synced"), &value);

	return true;
}





/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::ReadGRPANIMATIONSTACK
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			04/12/2015 9:42:01
//	
//	@return 			bool : 
//
//  @param				stack : 
//  @param				object : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::ReadGRPANIMATIONSTACK(GRPANIMATIONSTACK& stack, GGFOBJECT* object)
{
	ReadGRPNAMED(stack, object);

	// need to use iterator in here
	XBINARYMAP<XDWORD, GGFVALUE*>::XITERATOR it;
	for(it = object->indexedvalues.Begin(); it != object->indexedvalues.End(); it++)
		{
			if(object->definition->FastGetEntry(it.GetKey())->id == __L("GRPANIMATIONSTACK.maxtime"))
				{
					
					stack.SetMaxTime(it.GetElement()->GETXQWORD());
				}
			else if(object->definition->FastGetEntry(it.GetKey())->id == __L("GRPANIMATIONSTACK.mintime"))
			{
				stack.SetMinTime(it.GetElement()->GETXQWORD());
			}
			else if(object->definition->FastGetEntry(it.GetKey())->id == __L("GRPANIMATIONSTACK.minframes"))
				{
					stack.SetMinFrame(it.GetElement()->GETXQWORD());
				}
			else if(object->definition->FastGetEntry(it.GetKey())->id == __L("GRPANIMATIONSTACK.currentlayer"))
				{
					stack.SetCurrentLayer(it.GetElement()->Getint());
				}
			else if(object->definition->FastGetEntry(it.GetKey())->id == __L("GRPANIMATIONSTACK.status"))
				{
					GGFSTRING ggfstring;
					ReadString(object, __L("GRPANIMATIONSTACK.status"), &ggfstring);
					GRPANIMATION_STATUS status = GetStatusValue(&ggfstring);
					stack.SetStatus(status);
				}
			else if(object->definition->FastGetEntry(it.GetKey())->id == __L("GRPANIMATIONSTACK.active"))
				{
					bool active = it.GetElement()->GetBOOL();
					stack.SetActive(active);
				}
			else if(object->definition->FastGetEntry(it.GetKey())->id == __L("GRPANIMATIONSTACK.stackFrame"))
				{
					stack.SetStackFrame(it.GetElement()->GETXQWORD());
				}
			else if(object->definition->FastGetEntry(it.GetKey())->id == __L("GRPANIMATIONSTACK.laststackframe"))
				{
					stack.SetLastStackFrame(it.GetElement()->GETXQWORD());
				}
			else if(object->definition->FastGetEntry(it.GetKey())->id == __L("GRPANIMATIONSTACK.synced"))
				{
					stack.SetIsSynced(it.GetElement()->GetBOOL());
				}
			else if(object->definition->FastGetEntry(it.GetKey())->id == __L("GRPANIMATIONSTACK.layer"))
				{
					XQWORD ptr = 0;
					it.GetElement()->buffer.Get((XBYTE*)&ptr, file->info.ptrsize, 0);

					GRPANIMATIONLAYER* layer = (GRPANIMATIONLAYER*)GetLoadedObject(ptr);
					
					if((ptr != 0) && (layer == NULL))
						{
							//XQWORD index = file->seekmap.Find(ptr);
							XQWORD index = file->seekmap.seekmaptree.Get(ptr);
							if(index != NOTFOUND)
								{
									GGFOBJECT* obj = file->objects.Get((XDWORD)index);
									GRPANIMATIONLAYER* l = new GRPANIMATIONLAYER();

									ReadGRPANIMATIONLAYER(*l, obj);

									l->SetStack(&stack);

									l->SetActive(stack.IsActive());

									stack.GetLayers()->Add(l);
									loadedelements.Add(index, l);
								}
							else
								{
									XDEBUG_PRINTCOLOR(4,__L("ERROR: looking for an object not present in seekmap"));
								}
						}
					else
						{
							stack.GetLayers()->Add(layer);
						}
				}
		}

	return true;
}




/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::DefineGRPANIMATIONLAYER
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/12/2015 13:21:30
//	
//	@return 			bool : 
//
//  @param				definition : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::DefineGRPANIMATIONLAYER(GGFOBJECTDEFINITION* definition)
{
	DefineGRPNAMED(definition);

	RegisterDefinition(__L("GRPANIMATIONLAYER.animationnodes"), sizeof(void*), definition);
	RegisterDefinition(__L("GRPANIMATIONLAYER.curves"), sizeof(void*), definition);

	RegisterDefinition(__L("GRPANIMATIONLAYER.loopmode"), sizeof(XDWORD), definition);
	RegisterDefinition(__L("GRPANIMATIONLAYER.speedmultiplier"), sizeof(float), definition);

	RegisterDefinition(__L("GRPANIMATIONLAYER.minframe"), sizeof(XQWORD), definition);
	RegisterDefinition(__L("GRPANIMATIONLAYER.maxframe"), sizeof(XQWORD), definition);
	RegisterDefinition(__L("GRPANIMATIONLAYER.localframe"), sizeof(XQWORD), definition);
	RegisterDefinition(__L("GRPANIMATIONLAYER.laststackframe"), sizeof(XQWORD), definition);

	RegisterDefinition(__L("GRPANIMATIONLAYER.synced"), sizeof(XBYTE), definition);
	RegisterDefinition(__L("GRPANIMATIONLAYER.active"), sizeof(XBYTE), definition);
	RegisterDefinition(__L("GRPANIMATIONLAYER.finished"), sizeof(XBYTE), definition);

	RegisterDefinition(__L("GRPANIMATIONLAYER.status"), sizeof(XDWORD), definition);

	RegisterDefinition(__L("GRPANIMATIONLAYER.stack"), sizeof(void*), definition);

	return true;
}




/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::WriteGRPANIMATIONLAYER
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/12/2015 17:40:06
//	
//	@return 			bool : 
//
//  @param				layer : 
//  @param				object : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::WriteGRPANIMATIONLAYER(GRPANIMATIONLAYER& layer, GGFOBJECT* object)
{
	WriteGRPNAMED(layer, object);
	// write nodes and curves
	XVECTOR<GRPANIMATIONCURVE*>* curves = layer.GetCurves();
	XVECTOR<GRPANIMATIONNODE*>* nodes = layer.GetAnimationNodes();

	GGFVALUE value;

	for(XDWORD i =  0; i < curves->GetSize(); i++)
	{
		value.SetValue(curves->FastGet(i));
		object->Add(__L("GRPANIMATIONLAYER.curves"), &value);
	}

	for(XDWORD i =  0; i < nodes->GetSize(); i++)
	{
		value.SetValue(nodes->FastGet(i));
		object->Add(__L("GRPANIMATIONLAYER.animationnodes"), &value);
	}


	// this write but I was reading
	//GGFSTRING ggfstr;
	//ReadString(object, __L("GRPANIMATIONLAYER.loopmode"), &ggfstr);
	//layer.SetLoopMode(GetLoopValue(&ggfstr));
	//
	//layer.SetSpeedMultiplier(object->Get(__L("GRPANIMATIONLAYER.speedmultiplier"))->Getfloat());
	//
	//layer.SetMinFrame(object->Get(__L("GRPANIMATIONLAYER.minframe"))->GETXQWORD());
	//layer.SetMaxFrame(object->Get(__L("GRPANIMATIONLAYER.maxframe"))->GETXQWORD());
	//layer.SetLocalFrame(object->Get(__L("GRPANIMATIONLAYER.localframe"))->GETXQWORD());
	//layer.SetLastStackFrame(object->Get(__L("GRPANIMATIONLAYER.laststackframe"))->GETXQWORD());

	WriteString(object, __L("GRPANIMATIONLAYER.loopmode"), GetLoopKey(layer.GetLoopMode()));

	
	value.Setfloat(layer.GetSpeedMultiplier());
	object->Add(__L("GRPANIMATIONLAYER.speedmultiplier"), &value);

	value.SetXQWORD(layer.GetMinTime());
	object->Add(__L("GRPANIMATIONLAYER.minframe"), &value);

	value.SetXQWORD(layer.GetMaxTime());
	object->Add(__L("GRPANIMATIONLAYER.maxframe"), &value);

	value.SetXQWORD(layer.GetLocalTime());
	object->Add(__L("GRPANIMATIONLAYER.localframe"), &value);

	value.SetXQWORD(layer.GetLastStackTime());
	object->Add(__L("GRPANIMATIONLAYER.laststackframe"), &value);

	value.SetBOOL(layer.IsSync());
	object->Add(__L("GRPANIMATIONLAYER.synced"), &value);

	value.SetBOOL(layer.IsActive());
	object->Add(__L("GRPANIMATIONLAYER.active"), &value);

	value.SetBOOL(layer.IsFinished());
	object->Add(__L("GRPANIMATIONLAYER.finished"), &value);
	
	WriteString(object, __L("GRPANIMATIONLAYER.status"), GetStatusKey((GRPANIMATION_STATUS)layer.GetStatus()));

	value.SetValue(layer.GetStack());
	object->Add(__L("GRPANIMATIONLAYER.stack"), &value);

	return true;
}






/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::ReadGRPANIMATIONLAYER
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			03/12/2015 17:47:32
//	
//	@return 			bool : 
//
//  @param				layer : 
//  @param				object : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::ReadGRPANIMATIONLAYER(GRPANIMATIONLAYER& layer, GGFOBJECT* object)
{
	ReadGRPNAMED(layer, object);

	XBINARYMAP<XDWORD, GGFVALUE*>::XITERATOR it;
	for(it = object->indexedvalues.Begin(); it != object->indexedvalues.End(); it++)
		{
			if(object->definition->FastGetEntry(it.GetKey())->id == __L("GRPANIMATIONLAYER.loopmode"))
				{
					GGFSTRING ggfstring;

					ReadString(object, __L("GRPANIMATIONLAYER.loopmode"), &ggfstring);
					
					layer.SetLoopMode(GetLoopValue(&ggfstring));
				}
			else if(object->definition->FastGetEntry(it.GetKey())->id == __L("GRPANIMATIONLAYER.speedmultiplier"))
				{
					layer.SetSpeedMultiplier(it.GetElement()->Getfloat());
				}
			else if(object->definition->FastGetEntry(it.GetKey())->id == __L("GRPANIMATIONLAYER.minframe"))
				{
					layer.SetMinTime(it.GetElement()->GETXQWORD());
				}
			else if(object->definition->FastGetEntry(it.GetKey())->id == __L("GRPANIMATIONLAYER.maxframe"))
				{
					layer.SetMaxTime(it.GetElement()->GETXQWORD());
				}
			else if(object->definition->FastGetEntry(it.GetKey())->id == __L("GRPANIMATIONLAYER.localframe"))
				{
					layer.SetLocalTime(it.GetElement()->GETXQWORD());
				}
			else if(object->definition->FastGetEntry(it.GetKey())->id == __L("GRPANIMATIONLAYER.laststackframe"))
				{
					layer.SetLastStackTime(it.GetElement()->GETXQWORD());
				}
			else if(object->definition->FastGetEntry(it.GetKey())->id == __L("GRPANIMATIONLAYER.synced"))
				{
					layer.SetSync(it.GetElement()->GetBOOL());
				}
			else if(object->definition->FastGetEntry(it.GetKey())->id == __L("GRPANIMATIONLAYER.active"))
				{
					layer.SetActive(it.GetElement()->GetBOOL());
				}
			else if(object->definition->FastGetEntry(it.GetKey())->id == __L("GRPANIMATIONLAYER.finished"))
				{
					layer.SetIsFinished(it.GetElement()->GetBOOL());
				}
			else if(object->definition->FastGetEntry(it.GetKey())->id == __L("GRPANIMATIONLAYER.status"))
				{
					GGFSTRING ggfstring;
					ReadString(object, __L("GRPANIMATIONLAYER.status"), &ggfstring);
					layer.SetStatus(GetStatusValue(&ggfstring));
				}
			else if(object->definition->FastGetEntry(it.GetKey())->id == __L("GRPANIMATIONLAYER.stack"))
				{
			// Probably set by the serializing stack
				}
			/*else if(*object->values.GetKey(i) == __L("GRPANIMATIONLAYER.curves"))
				{
					XQWORD ptr = 0;
					object->values.GetElement(i)->buffer.SetPosition(0);
					object->values.GetElement(i)->buffer.Get((XBYTE*)&ptr, file->info.ptrsize);
					
					GRPANIMATIONCURVE* curve = (GRPANIMATIONCURVE*)GetLoadedObject(ptr);
					if((ptr != 0) && (curve == NULL))
						{
							XQWORD index = file->seekmap.Find(ptr);
							if(index != NOTFOUND)
								{
									GGFOBJECT* obj = file->objects.Get((XDWORD)index);
									GRPANIMATIONCURVE* c = new GRPANIMATIONCURVE();
									ReadGRPANIMATIONCURVE(*c, obj);

									layer.GetCurves()->Add(c);
									loadedelements.Add(index, c);
								}
							else
								{
									XDEBUG_PRINTCOLOR(4,__L("ERROR: looking for an object not present in seekmap"));
								}
						}
					else
						{
							layer.GetCurves()->Add(curve);
						}
				}*/
			else if(object->definition->FastGetEntry(it.GetKey())->id == __L("GRPANIMATIONLAYER.animationnodes"))
				{
					// probably read nodes before curves
					XQWORD ptr = 0;
					it.GetElement()->buffer.SetPosition(0);
					it.GetElement()->buffer.Get((XBYTE*)&ptr, file->info.ptrsize);

					GRPANIMATIONNODE* node = (GRPANIMATIONNODE*)GetLoadedObject(ptr);
					if((ptr != 0) && (node == NULL))
						{
							//XQWORD index = file->seekmap.Find(ptr);
							XQWORD index = file->seekmap.seekmaptree.Get(ptr);
							if(index != NOTFOUND)
								{
									GGFOBJECT* obj = file->objects.Get((XDWORD)index);
									GRPANIMATIONNODE* n = new GRPANIMATIONNODE();

									n->SetLayer(&layer);

									ReadGRPANIMATIONNODE(*n, obj);
									layer.GetAnimationNodes()->Add(n);
									loadedelements.Add(index, n);
									
								}
							else
								{
									XDEBUG_PRINTCOLOR(4,__L("ERROR: looking for an object not present in seekmap"));
								}
						}
					else
						{
							layer.GetAnimationNodes()->Add(node);
							node->SetLayer(&layer);
						}
				}
		}

	return true;
}



/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::DefineGRPANIMATIONNODE
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/12/2015 17:39:57
//	
//	@return 			bool : 
//
//  @param				definition : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::DefineGRPANIMATIONNODE(GGFOBJECTDEFINITION* definition)
{
	DefineGRPNAMED(definition);

	RegisterDefinition(__L("GRPANIMATIONNODE.target"), sizeof(void*), definition);

	RegisterDefinition(__L("GRPANIMATIONNODE.targetTag"), sizeof(XQWORD), definition);
	RegisterDefinition(__L("GRPANIMATIONNODE.targetName"), sizeof(XDWORD), definition);

	RegisterDefinition(__L("GRPANIMATIONNODE.layer"), sizeof(void*), definition);

	RegisterDefinition(__L("GRPANIMATIONNODE.type"), sizeof(XDWORD), definition);

	RegisterDefinition(__L("GRPANIMATIONNODE.curves"), sizeof(void*), definition);
	RegisterDefinition(__L("GRPANIMATIONNODE.curvesaxis"), sizeof(XDWORD), definition);


	// these are XMAP entries
	RegisterDefinition(__L("GRPANIMATIONNODE.ncurvesconnections"), sizeof(XDWORD), definition);
	RegisterDefinition(__L("GRPANIMATIONNODE.CurveConnections.keys"), sizeof(XDWORD), definition);
	RegisterDefinition(__L("GRPANIMATIONNODE.CurveConnections.values"), sizeof(XDWORD), definition);

	RegisterDefinition(__L("GRPANIMATIONNODE.ndefaultvalues"), sizeof(XDWORD), definition);
	RegisterDefinition(__L("GRPANIMATIONNODE.DefaultNodeValues.keys"), sizeof(XDWORD), definition);
	RegisterDefinition(__L("GRPANIMATIONNODE.DefaultNodeValues.values"), sizeof(XDWORD), definition);

	RegisterDefinition(__L("GRPANIMATIONNODE.minframe"), sizeof(XQWORD), definition);
	RegisterDefinition(__L("GRPANIMATIONNODE.maxframe"), sizeof(XQWORD), definition);

	return true;
}






/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::WriteGRPANIMATIONNODE
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/12/2015 17:39:48
//	
//	@return 			bool : 
//
//  @param				node : 
//  @param				object : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::WriteGRPANIMATIONNODE(GRPANIMATIONNODE& node, GGFOBJECT* object)
{
	WriteGRPNAMED(node, object);

	GGFVALUE value;
	value.SetValue(node.GetTarget());
	object->Add(__L("GRPANIMATIONNODE.target"), &value);

	value.SetValue(node.GetTargetTag());
	object->Add(__L("GRPANIMATIONNODE.targetTag"), &value);

	WriteString(object, __L("GRPANIMATIONNODE.targetName"), node.GetTargetName()->Get());

	value.SetValue(node.GetLayer());
	object->Add(__L("GRPANIMATIONNODE.layer"), &value);

	WriteString(object, __L("GRPANIMATIONNODE.type"), GetTypeKey(node.GetType()));

	for(XDWORD i = 0; i < node.GetCurves()->GetSize(); i++)
		{
			value.SetValue(node.GetCurves()->FastGet(i));
			object->Add(__L("GRPANIMATIONNODE.curves"), &value);
		}

	for(XDWORD i = 0; i < node.GetCurves()->GetSize(); i++)
		{
			value.SetValue(node.GetCurveAxis()->FastGet(i));
			object->Add(__L("GRPANIMATIONNODE.curvesaxis"), &value);
		}

	// MAPS MAPS MAPS MAPS
	// maybe easier if other functions handled them, as the function can be huge
	//RegisterDefinition(__L("GRPANIMATIONNODE.CurveConnections"), sizeof(void*), definition);
	//RegisterDefinition(__L("GRPANIMATIONNODE.DefaultNodeValues"), sizeof(void*), definition);
	WriteCurveConnections(node, object);
	WriteDefaultNodeValues(node, object);

	value.SetValue(node.GetMinTime());
	object->Add(__L("GRPANIMATIONNODE.minframe"), &value);

	value.SetValue(node.GetMaxTime());
	object->Add(__L("GRPANIMATIONNODE.maxframe"), &value);

	return true;
}






/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::ReadGRPANIMATIONNODE
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			04/12/2015 11:08:08
//	
//	@return 			bool : 
//
//  @param				node : 
//  @param				object : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::ReadGRPANIMATIONNODE(GRPANIMATIONNODE& node, GGFOBJECT* object)
{
	ReadGRPNAMED(node, object);

	XDWORD properties = object->indexedvalues.GetSize();//object->values.GetSize();

	XBINARYMAP<XDWORD, GGFVALUE*>::XITERATOR it;
	for(it = object->indexedvalues.Begin(); it != object->indexedvalues.End(); it++)
		{
			if(object->definition->FastGetEntry(it.GetKey())->id == __L("GRPANIMATIONNODE.target"))
				{
					XQWORD ptr = 0;
					it.GetElement()->buffer.SetPosition(0);
					it.GetElement()->buffer.Get((XBYTE*)&ptr, file->info.ptrsize);

					GRPINTERFACEANIMABLE* anim = (GRPINTERFACEANIMABLE*)GetLoadedObject(ptr);
					node.SetTarget(anim);
				}
			else if(object->definition->FastGetEntry(it.GetKey())->id == __L("GRPANIMATIONNODE.targetTag"))
				{
					node.SetTargetTag(it.GetElement()->GETXQWORD());
				}
			else if(object->definition->FastGetEntry(it.GetKey())->id == __L("GRPANIMATIONNODE.targetName"))
				{
					XSTRING str;
					GGFSTRING ggfstr;
					ReadString(object, __L("GRPANIMATIONNODE.targetName"), &ggfstr);
					ggfstr.ToXSTRING(&str);
					node.SetTargetName(str.Get());
				}
			else if(object->definition->FastGetEntry(it.GetKey())->id == __L("GRPANIMATIONNODE.type"))
				{
					GGFSTRING ggfstring;
					ReadString(object, __L("GRPANIMATIONNODE.type"), &ggfstring);
					node.SetType(GetTypeValue(&ggfstring));
				}
			else if(object->definition->FastGetEntry(it.GetKey())->id == __L("GRPANIMATIONNODE.minframe"))
				{
					node.SetMinTime(it.GetElement()->GETXQWORD());
				}
			else if(object->definition->FastGetEntry(it.GetKey())->id == __L("GRPANIMATIONNODE.maxframe"))
				{
					node.SetMaxTime(it.GetElement()->GETXQWORD());
				}
			else if(object->definition->FastGetEntry(it.GetKey())->id == __L("GRPANIMATIONNODE.curvesaxis"))
				{
					XDWORD value = it.GetElement()->GETXDWORD();
					node.GetCurveAxis()->Add(value);
				}
			else if(object->definition->FastGetEntry(it.GetKey())->id == __L("curves"))
				{
					//load GRPANIMATIONCURVE
					XQWORD ptr;
					it.GetElement()->buffer.SetPosition(0);
					it.GetElement()->buffer.Get((XBYTE*)&ptr, file->info.ptrsize);

					GRPANIMATIONCURVE* curve = (GRPANIMATIONCURVE*)GetLoadedObject(ptr);
					if((ptr != 0) && (curve != NULL))
						{
							//XQWORD index = file->seekmap.Find(ptr);
							XQWORD index = file->seekmap.seekmaptree.Get(ptr);
							if(index != NOTFOUND)
								{
									GGFOBJECT* obj = file->objects.Get((XDWORD)index);
									GRPANIMATIONCURVE* c = new GRPANIMATIONCURVE();
									if(!c) return false;

									ReadGRPANIMATIONCURVE(*c, obj);

									c->ParentNode = &node;
									node.GetCurves()->Add(c);
									node.GetLayer()->AddCurve(c);
									loadedelements.Add(index, c);
								}
							else
								{
									XDEBUG_PRINTCOLOR(4,__L("ERROR: looking for an object not present in seekmap"));
								}
						}
					else
						{
							node.GetCurves()->Add(curve);
							curve->ParentNode = &node;
							node.GetLayer()->AddCurve(curve);
						}

				}
		}

	ReadCurveConnections(node, object);
	ReadDefaultNodeValues(node, object);

	return true;
}



/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::WriteCurveConnections
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			02/12/2015 9:36:26
//	
//	@return 			bool : 
//
//  @param				node : 
//  @param				object : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::WriteCurveConnections(GRPANIMATIONNODE& node, GGFOBJECT* object)
{
	XDWORD ncurvesconnections = node.GetCurveConnections()->GetSize();

	GGFVALUE value;
	value.SetValue(ncurvesconnections);
	object->Add(__L("GRPANIMATIONNODE.ncurvesconnections"), &value);


	//RegisterDefinition(__L("GRPANIMATIONNODE.CurveConnections.keys"), sizeof(XDWORD), definition);
	//RegisterDefinition(__L("GRPANIMATIONNODE.CurveConnections.values"), sizeof(XDWORD), definition);

	XBUFFER keysvector( false);
	XBUFFER curvesptrvector( false);

	keysvector.Resize(ncurvesconnections*sizeof(XDWORD));
	curvesptrvector.Resize(ncurvesconnections*sizeof(void*));

	keysvector.SetPosition(0);
	curvesptrvector.SetPosition(0);

	for(XDWORD i = 0; i < ncurvesconnections; i++)
		{
			XDWORD key = node.GetCurveConnections()->GetKey(i);
			keysvector.Set((XBYTE*)&key, sizeof(XDWORD));
			void * value = node.GetCurveConnections()->GetElement(i);
			curvesptrvector.Set((XBYTE*)&value, sizeof(void*));
		}

	GGFDATA* keydata = new GGFDATA();
	GGFDATA* valuedata = new GGFDATA();

	keydata->buffer.Resize(keysvector.GetSize());
	valuedata->buffer.Resize(curvesptrvector.GetSize());

	keydata->buffer.Set(keysvector.Get(), keysvector.GetSize());
	valuedata->buffer.Set(curvesptrvector.Get(), curvesptrvector.GetSize());

	value.SetValue(file->AddData(keydata));
	object->Add(__L("GRPANIMATIONNODE.CurveConnections.keys"), &value);

	value.SetValue(file->AddData(valuedata));
	object->Add(__L("GRPANIMATIONNODE.CurveConnections.values"), &value);

	return true;
}





/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::ReadCurveConnections
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			03/12/2015 13:40:06
//	
//	@return 			bool : 
//
//  @param				node : 
//  @param				object : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::ReadCurveConnections(GRPANIMATIONNODE& node, GGFOBJECT* object)
{
	XDWORD ncurvesconnections = object->Get(__L("GRPANIMATIONNODE.ncurvesconnections"))->GETXDWORD();
	XDWORD keysindex					= object->Get(__L("GRPANIMATIONNODE.CurveConnections.keys"))->GETXDWORD();
	XDWORD valuesindex				= object->Get(__L("GRPANIMATIONNODE.CurveConnections.values"))->GETXDWORD();

	GGFDATA* keysdata		= file->data.Get(keysindex);
	GGFDATA* valuesdata = file->data.Get(valuesindex);

	keysdata->buffer.SetPosition(0);
	valuesdata->buffer.SetPosition(0);

	for(XDWORD i = 0; i < ncurvesconnections; i++)
		{
			XDWORD key		= 0;
			XQWORD value	= 0;

			keysdata->buffer.Get((XBYTE*)&key, sizeof(key));
			valuesdata->buffer.Get((XBYTE*)&value, file->info.ptrsize);

			// load the curve
			GRPANIMATIONCURVE* curve = (GRPANIMATIONCURVE*)GetLoadedObject(value);
			if((value != 0) && (curve == NULL))
				{
					//XDWORD index = (XDWORD)file->seekmap.Find(value);
					XQWORD index = file->seekmap.seekmaptree.Get(value);
					if(index != NOTFOUND)
						{
							GGFOBJECT* obj = file->objects.Get((XDWORD)index);
							GRPANIMATIONCURVE* c = new GRPANIMATIONCURVE();
							if(!c) return false;

							ReadGRPANIMATIONCURVE(*c, obj);

							node.AddConnection((GRPANIMATIONCURVEAXIS)key, c);
							node.GetLayer()->AddCurve(c);
							c->ParentNode = &node;
							XQWORD qindex = index;
							loadedelements.Add(qindex, c);
						}
					else
						{
							XDEBUG_PRINTCOLOR(4, __L("ERROR: looking for and object not present in seekmap"));
						}
				}
			else
				{
					node.AddConnection((GRPANIMATIONCURVEAXIS)key, curve);
					node.GetLayer()->AddCurve(curve);
					curve->ParentNode = &node;
				}
		}


	return true;
}




/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::WriteDefaultNodeValues
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			02/12/2015 9:36:33
//	
//	@return 			bool : 
//
//  @param				node : 
//  @param				object : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::WriteDefaultNodeValues(GRPANIMATIONNODE& node, GGFOBJECT* object)
{
	XDWORD ndefaultvalues = node.GetDefaultValues()->GetSize();
	GGFVALUE value;
	value.SetValue(ndefaultvalues);
	object->Add(__L("GRPANIMATIONNODE.ndefaultvalues"), &value);

	XBUFFER keysvector( false);
	XBUFFER valuesvector( false);

	keysvector.Resize(ndefaultvalues*sizeof(XDWORD));
	valuesvector.Resize(ndefaultvalues*sizeof(float));

	keysvector.SetPosition(0);
	valuesvector.SetPosition(0);

	for(XDWORD i = 0; i < ndefaultvalues; i++)
		{
			XDWORD key = node.GetDefaultValues()->GetKey(i);
			keysvector.Set((XBYTE*)&key, sizeof(XDWORD));
			float value = node.GetDefaultValues()->GetElement(i);
			valuesvector.Set((XBYTE*)&value, sizeof(float));
		}

	GGFDATA* keydata = new GGFDATA();
	GGFDATA* valuedata = new GGFDATA();

	keydata->buffer.Resize(keysvector.GetSize());
	valuedata->buffer.Resize(valuesvector.GetSize());

	keydata->buffer.Set(keysvector.Get(), keysvector.GetSize());
	valuedata->buffer.Set(valuesvector.Get(), valuesvector.GetSize());

	value.SetValue(file->AddData(keydata));
	object->Add(__L("GRPANIMATIONNODE.DefaultNodeValues.keys"), &value);

	value.SetValue(file->AddData(valuedata));
	object->Add(__L("GRPANIMATIONNODE.DefaultNodeValues.values"), &value);

	return true;
}



/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::ReadDefaultNodeValues
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			03/12/2015 11:35:21
//	
//	@return 			bool : 
//
//  @param				node : 
//  @param				object : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::ReadDefaultNodeValues(GRPANIMATIONNODE& node, GGFOBJECT* object)
{
	XDWORD ndefaultvalues = object->Get(__L("GRPANIMATIONNODE.ndefaultvalues"))->GETXDWORD();
	XDWORD keysindex			= object->Get(__L("GRPANIMATIONNODE.DefaultNodeValues.keys"))->GETXDWORD();
	XDWORD valuesindex		= object->Get(__L("GRPANIMATIONNODE.DefaultNodeValues.values"))->GETXDWORD();

	GGFDATA * keysdata		= file->data.Get(keysindex);
	GGFDATA * valuesdata	= file->data.Get(valuesindex);

	keysdata->buffer.SetPosition(0);
	valuesdata->buffer.SetPosition(0);

	for(XDWORD i = 0; i < ndefaultvalues; i++)
		{
			XDWORD key = 0;
			float value = 0;
			keysdata->buffer.Get((XBYTE*)&key, sizeof(key));
			valuesdata->buffer.Get((XBYTE*)&value, sizeof(value));
			node.GetDefaultValues()->Add(key, value);
		}

	return true;
}

/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::DefineGRPANIMATIONCURVE
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/12/2015 13:21:21
//	
//	@return 			bool : 
//
//  @param				definition : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::DefineGRPANIMATIONCURVE(GGFOBJECTDEFINITION* definition)
{
	DefineGRPNAMED(definition);

	RegisterDefinition(__L("GRPANIMATIONCURVE.Active"), sizeof(XBYTE), definition);

	RegisterDefinition(__L("GRPANIMATIONCURVE.target"), sizeof(float), definition);

	// VECTORS
	RegisterDefinition(__L("GRPANIMATIONCURVE.keytime"), sizeof(XDWORD), definition);
	RegisterDefinition(__L("GRPANIMATIONCURVE.keyvalues"), sizeof(XDWORD), definition);
	RegisterDefinition(__L("GRPANIMATIONCURVE.keyattributes"), sizeof(XDWORD), definition);
	RegisterDefinition(__L("GRPANIMATIONCURVE.keyattrdatafloat"), sizeof(XDWORD), definition);
	RegisterDefinition(__L("GRPANIMATIONCURVE.keyattrrefcount"), sizeof(XDWORD), definition);

	RegisterDefinition(__L("GRPANIMATIONCURVE.UnpackedAttributes"), sizeof(XDWORD), definition);
	RegisterDefinition(__L("GRPANIMATIONCURVE.fkeytime"), sizeof(XDWORD), definition);
	// VECTORS END

	RegisterDefinition(__L("GRPANIMATIONCURVE.currentkey"), sizeof(int), definition);

	// save pointers and just load them?
	RegisterDefinition(__L("GRPANIMATIONCURVE.bcs"), sizeof(void*), definition);

	RegisterDefinition(__L("GRPANIMATIONCURVE.ParentNode"), sizeof(void*), definition);

	RegisterDefinition(__L("GRPANIMATIONCURVE.firsttime"), sizeof(XQWORD), definition);
	RegisterDefinition(__L("GRPANIMATIONCURVE.lasttime"), sizeof(XQWORD), definition);
	RegisterDefinition(__L("GRPANIMATIONCURVE.firstvalue"), sizeof(float), definition);
	RegisterDefinition(__L("GRPANIMATIONCURVE.lastvalue"), sizeof(float), definition);
	RegisterDefinition(__L("GRPANIMATIONCURVE.nkeys"), sizeof(int), definition);
	RegisterDefinition(__L("GRPANIMATIONCURVE.bcsSize"), sizeof(int), definition);
	RegisterDefinition(__L("GRPANIMATIONCURVE.nkeyattrrefcount"), sizeof(int), definition);

	return true;
}





/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::WriteGRPANIMATIONCURVE
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			02/12/2015 12:21:23
//	
//	@return 			bool : 
//
//  @param				curve : 
//  @param				object : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::WriteGRPANIMATIONCURVE(GRPANIMATIONCURVE& curve, GGFOBJECT* object)
{
	WriteGRPNAMED(curve, object);

	GGFVALUE value;
	value.SetBOOL(curve.Active);
	object->Add(__L("GRPANIMATIONCURVE.Active"), &value);
	
	value.SetValue(curve.target);
	object->Add(__L("GRPANIMATIONCURVE.target"), &value);

	// VECTORS VECTORS
	XDWORD index = WriteVector(curve.keytime);
	value.SetValue(index);
	object->Add(__L("GRPANIMATIONCURVE.keytime"), &value);

	index = WriteVector(curve.keyvalues);
	value.SetValue(index);
	object->Add(__L("GRPANIMATIONCURVE.keyvalues"), &value);

	index = WriteVector(curve.keyattributes);
	value.SetValue(index);
	object->Add(__L("GRPANIMATIONCURVE.keyattributes"), &value);

	index = WriteVector(curve.keyattrdatafloat);
	value.SetValue(index);
	object->Add(__L("GRPANIMATIONCURVE.keyattrdatafloat"), &value);

	index = WriteVector(curve.keyattrrefcount);
	value.SetValue(index);
	object->Add(__L("GRPANIMATIONCURVE.keyattrrefcount"), &value);

	index = WriteVector(curve.UnpackedAttributes);
	value.SetValue(index);
	object->Add(__L("GRPANIMATIONCURVE.UnpackedAttributes"), &value);

	index = WriteVector(curve.fkeytime);
	value.SetValue(index);
	object->Add(__L("GRPANIMATIONCURVE.fkeytime"), &value);
	// END VECTORS


	value.SetValue(curve.currentkey);
	object->Add(__L("GRPANIMATIONCURVE.currentkey"), &value);
	//

	for(XDWORD i = 0; i < curve.bcs.GetSize(); i++)
		{
			value.SetValue(curve.bcs.FastGet(i));
			object->Add(__L("GRPANIMATIONCURVE.bcs"), &value);
		}


	value.SetValue(curve.ParentNode);
	object->Add(__L("GRPANIMATIONCURVE.ParentNode"), &value);
	
	value.SetValue(curve.firsttime);
	object->Add(__L("GRPANIMATIONCURVE.firsttime"), &value);
	
	value.SetValue(curve.lasttime);
	object->Add(__L("GRPANIMATIONCURVE.lasttime"), &value);
	
	value.SetValue(curve.firstvalue);
	object->Add(__L("GRPANIMATIONCURVE.firstvalue"), &value);
	
	value.SetValue(curve.lastvalue);
	object->Add(__L("GRPANIMATIONCURVE.lastvalue"), &value);
	
	value.SetValue(curve.nkeys);
	object->Add(__L("GRPANIMATIONCURVE.nkeys"), &value);
	
	value.SetValue(curve.bcsSize);
	object->Add(__L("GRPANIMATIONCURVE.bcsSize"), &value);
	
	value.SetValue(curve.nkeyattrrefcount);
	object->Add(__L("GRPANIMATIONCURVE.nkeyattrrefcount"), &value);

	return true;
}





/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::ReadGRPANIMATIONCURVE
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			03/12/2015 9:57:10
//	
//	@return 			bool : 
//
//  @param				curve : 
//  @param				object : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::ReadGRPANIMATIONCURVE(GRPANIMATIONCURVE& curve, GGFOBJECT* object)
{
	ReadGRPNAMED(curve, object);
	

	XBINARYMAP<XDWORD, GGFVALUE*>::XITERATOR it;

	for(it = object->indexedvalues.Begin(); it != object->indexedvalues.End(); it++)
		{
			if(object->definition->FastGetEntry(it.GetKey())->id == __L("GRPANIMATIONCURVE.Active"))
				{
					curve.Active = it.GetElement()->GetBOOL();
				}
			else if(object->definition->FastGetEntry(it.GetKey())->id == __L("GRPANIMATIONCURVE.target"))
				{
					curve.target = it.GetElement()->Getfloat();
				}
			else if(object->definition->FastGetEntry(it.GetKey())->id == __L("GRPANIMATIONCURVE.keytime"))
				{
					XDWORD index = it.GetElement()->GETXDWORD();
					ReadVector(curve.keytime, index);
				}
			else if(object->definition->FastGetEntry(it.GetKey())->id == __L("GRPANIMATIONCURVE.keyvalues"))
				{
					XDWORD index = it.GetElement()->GETXDWORD();
					ReadVector(curve.keyvalues, index);
				}
			else if(object->definition->FastGetEntry(it.GetKey())->id == __L("GRPANIMATIONCURVE.keyattributes"))
				{
					XDWORD index = it.GetElement()->GETXDWORD();
					ReadVector(curve.keyattributes, index);
				}
			else if(object->definition->FastGetEntry(it.GetKey())->id == __L("GRPANIMATIONCURVE.keyattrdatafloat"))
				{
					XDWORD index = it.GetElement()->GETXDWORD();
					ReadVector(curve.keyattrdatafloat, index);
				}
			else if(object->definition->FastGetEntry(it.GetKey())->id == __L("GRPANIMATIONCURVE.keyattrrefcount"))
				{
					XDWORD index = it.GetElement()->GETXDWORD();
					ReadVector(curve.keyattrrefcount, index);
				}
			else if(object->definition->FastGetEntry(it.GetKey())->id == __L("GRPANIMATIONCURVE.UnpackedAttributes"))
				{
					XDWORD index = it.GetElement()->GETXDWORD();
					ReadVector(curve.UnpackedAttributes, index);
				}
			else if(object->definition->FastGetEntry(it.GetKey())->id == __L("GRPANIMATIONCURVE.fkeytime"))
				{
					XDWORD index = it.GetElement()->GETXDWORD();
					ReadVector(curve.fkeytime, index);
				}
			else if(object->definition->FastGetEntry(it.GetKey())->id == __L("GRPANIMATIONCURVE.bcs"))
				{
					// find the object of the spline
					XQWORD ptr = 0;
					it.GetElement()->buffer.Get((XBYTE*)&ptr, file->info.ptrsize, 0);

					// check if already loaded
					GRPBEZIERSPLINE* spline = (GRPBEZIERSPLINE*)GetLoadedObject(ptr);
					if((spline == NULL) && (ptr != 0))
						{
							// if not load it
							//XQWORD index = file->seekmap.Find(ptr);
							XQWORD index = file->seekmap.seekmaptree.Get(ptr);
							if(index != NOTFOUND)
								{
									GGFOBJECT* obj = file->objects.Get((int)index);
									GRPBEZIERCURVE* bezierspline = new GRPBEZIERCURVE();
									if(!bezierspline) return false;
									ReadGRPBEZIERSPLINE(*bezierspline, obj);
									curve.bcs.Add(bezierspline);
									// not tag!!
									loadedelements.Add(index, bezierspline); // need to check
								}
						}
					else
						{
							curve.bcs.Add(spline);
						}
				}
			else if(object->definition->FastGetEntry(it.GetKey())->id == __L("GRPANIMATIONCURVE.ParentNode"))
				{
					// need to check and load parent node if needed(not probable)
					// should not be followed but set by loading node?
				}
			else if(object->definition->FastGetEntry(it.GetKey())->id == __L("GRPANIMATIONCURVE.firsttime"))
				{
					curve.firsttime = it.GetElement()->GETXQWORD();
				}
			else if(object->definition->FastGetEntry(it.GetKey())->id == __L("GRPANIMATIONCURVE.lasttime"))
				{
					curve.lasttime = it.GetElement()->GETXQWORD();
				}
			else if(object->definition->FastGetEntry(it.GetKey())->id == __L("GRPANIMATIONCURVE.firstvalue"))
				{
					curve.firstvalue = it.GetElement()->Getfloat();
				}
			else if(object->definition->FastGetEntry(it.GetKey())->id == __L("GRPANIMATIONCURVE.lastvalue"))
				{
					curve.lastvalue = it.GetElement()->Getfloat();
				}
			else if(object->definition->FastGetEntry(it.GetKey())->id == __L("GRPANIMATIONCURVE.nkeys"))
				{
					curve.nkeys = it.GetElement()->Getint();
				}
			else if(object->definition->FastGetEntry(it.GetKey())->id == __L("GRPANIMATIONCURVE.bcsSize"))
				{
					curve.bcsSize = it.GetElement()->Getint();
				}
			else if(object->definition->FastGetEntry(it.GetKey())->id == __L("GRPANIMATIONCURVE.nkeyattrrefcount"))
				{
					curve.nkeyattrrefcount = it.GetElement()->Getint();
				}
		}

	return true;
}



/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::DefineGRPBEZIERSPLINE
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/12/2015 14:12:51
//	
//	@return 			bool : 
//
//  @param				definition : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::DefineGRPBEZIERSPLINE(GGFOBJECTDEFINITION* definition)
{
	RegisterDefinition(__L("GRPBEZIERSPLINE.points"), sizeof(XDWORD), definition);
	RegisterDefinition(__L("GRPBEZIERSPLINE.Anchors"), sizeof(XDWORD), definition);

	return true;
}





/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::WriteGRPBEZIERSPLINE
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			02/12/2015 10:34:17
//	
//	@return 			bool : 
//
//  @param				spline : 
//  @param				object : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::WriteGRPBEZIERSPLINE(GRPBEZIERSPLINE& spline, GGFOBJECT* object)
{
	// Points
	XBUFFER outervector( false);
	XBUFFER innervector( false);
	XBUFFER data( false);

	XVECTOR<XVECTOR<GRPPOINT*>*>* points = spline.GetPoints();

	// add the points to the data area
	outervector.Resize(points->GetSize()*sizeof(XDWORD));
	outervector.SetPosition(0);

	// GRPPOINT: index + x,y,z,w
	data.Resize(sizeof(XDWORD) + 4*sizeof(float));

	GGFVALUE value;

	// for ever inner vector
	for(XDWORD i = 0; i < points->GetSize(); i++)
		{
			XVECTOR<GRPPOINT*>* inner = points->FastGet(i);
			innervector.Resize(inner->GetSize()*sizeof(XDWORD));
			innervector.SetPosition(0);
			// for every point add it to the data area
			for(XDWORD j = 0; j < inner->GetSize(); j++)
				{
					value.SetGRPPOINT(*(inner->FastGet(j)));
					data.Set(value.buffer.Get(), value.buffer.GetSize(), 0);

					// write index to the inner vector
					GGFDATA* writedata = new GGFDATA();
					writedata->buffer.Resize(data.GetSize());
					writedata->buffer.Set(data.Get(), data.GetSize());
					XDWORD index = file->AddData(writedata);
					innervector.Set((XBYTE*)&index, sizeof(XDWORD));
				}

			// write inner vector to data area
			GGFDATA* innerdata = new GGFDATA();
			innerdata->buffer.Resize(innervector.GetSize());
			innerdata->buffer.Set(innervector.Get(), innervector.GetSize());

			// write index of inner vector to the outer vector
			XDWORD innerindex = file->AddData(innerdata);
			outervector.Set((XBYTE*)&innerindex, sizeof(XDWORD));
		}

	// write outer vector to the data area
	GGFDATA* outerdata = new GGFDATA();
	outerdata->buffer.Resize(outervector.GetSize());
	outerdata->buffer.Set(outervector.Get(), outervector.GetSize());

	XDWORD outerindex = file->AddData(outerdata);
	value.SetValue(outerindex);
	object->Add(__L("GRPBEZIERSPLINE.points"), &value);


	// Anchors
	XBUFFER anchorsvector( false);
	XVECTOR<GRPBEZIERANCHORS*>* anchors = spline.GetAnchors();
	anchorsvector.Resize(anchors->GetSize()*4*(sizeof(XDWORD) + 3*sizeof(float)));
	anchorsvector.SetPosition(0);

	for(XDWORD i = 0; i < anchors->GetSize(); i++)
		{
			// write each anchor to the vector
			anchorsvector.Set((XBYTE*)&(anchors->FastGet(i)->StartA.index), sizeof(XDWORD));
			anchorsvector.Set((XBYTE*)&(anchors->FastGet(i)->StartA.x), sizeof(float));
			anchorsvector.Set((XBYTE*)&(anchors->FastGet(i)->StartA.y), sizeof(float));
			anchorsvector.Set((XBYTE*)&(anchors->FastGet(i)->StartA.z), sizeof(float));
			anchorsvector.Set((XBYTE*)&(anchors->FastGet(i)->StartA.w), sizeof(float));

			anchorsvector.Set((XBYTE*)&(anchors->FastGet(i)->AnchorA.index), sizeof(XDWORD));
			anchorsvector.Set((XBYTE*)&(anchors->FastGet(i)->AnchorA.x), sizeof(float));
			anchorsvector.Set((XBYTE*)&(anchors->FastGet(i)->AnchorA.y), sizeof(float));
			anchorsvector.Set((XBYTE*)&(anchors->FastGet(i)->AnchorA.z), sizeof(float));
			anchorsvector.Set((XBYTE*)&(anchors->FastGet(i)->AnchorA.w), sizeof(float));

			anchorsvector.Set((XBYTE*)&(anchors->FastGet(i)->AnchorB.index), sizeof(XDWORD));
			anchorsvector.Set((XBYTE*)&(anchors->FastGet(i)->AnchorB.x), sizeof(float));
			anchorsvector.Set((XBYTE*)&(anchors->FastGet(i)->AnchorB.y), sizeof(float));
			anchorsvector.Set((XBYTE*)&(anchors->FastGet(i)->AnchorB.z), sizeof(float));
			anchorsvector.Set((XBYTE*)&(anchors->FastGet(i)->AnchorB.w), sizeof(float));

			anchorsvector.Set((XBYTE*)&(anchors->FastGet(i)->EndB.index), sizeof(XDWORD));
			anchorsvector.Set((XBYTE*)&(anchors->FastGet(i)->EndB.x), sizeof(float));
			anchorsvector.Set((XBYTE*)&(anchors->FastGet(i)->EndB.y), sizeof(float));
			anchorsvector.Set((XBYTE*)&(anchors->FastGet(i)->EndB.z), sizeof(float));
			anchorsvector.Set((XBYTE*)&(anchors->FastGet(i)->EndB.w), sizeof(float));
		}

	// add the vector to the data area
	GGFDATA* anchordata = new GGFDATA();
	anchordata->buffer.Resize(anchorsvector.GetSize());
	anchordata->buffer.Set(anchorsvector.Get(), anchorsvector.GetSize());

	XDWORD anchorsindex = file->AddData(anchordata);

	value.SetValue(anchorsindex);
	object->Add(__L("GRPBEZIERSPLINE.Anchors"), &value);

	return true;
}




/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::ReadGRPBEZIERSPLINE
*/	
/**	
//	
//	reads a bezier spline
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			02/12/2015 17:09:06
//	
//	@return 			bool : 
//
//  @param				spline : 
//  @param				object : 
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::ReadGRPBEZIERSPLINE(GRPBEZIERSPLINE& spline, GGFOBJECT* object)
{
	XDWORD pointsindex = object->Get(__L("GRPBEZIERSPLINE.points"))->GETXDWORD();
	XDWORD anchorsindex = object->Get(__L("GRPBEZIERSPLINE.Anchors"))->GETXDWORD();

	// Points
	GGFDATA* outerdata = file->data.Get(pointsindex);
	outerdata->buffer.SetPosition(0);
	XDWORD outersize = outerdata->buffer.GetSize()/sizeof(XDWORD); // maybe this calculation is wrong

	for(XDWORD i = 0; i < outersize; i++)
		{
			XDWORD innerindex;
			outerdata->buffer.Get((XBYTE*)&innerindex, sizeof(XDWORD));
			GGFDATA* innerdata = file->data.Get(innerindex);
			XDWORD innersize = innerdata->buffer.GetSize()/sizeof(XDWORD);
			innerdata->buffer.SetPosition(0);
			XVECTOR<GRPPOINT*>* inner = new XVECTOR<GRPPOINT*>();

			for(XDWORD j = 0; j < innersize; j++)
				{
					XDWORD pointindex;
					innerdata->buffer.Get((XBYTE*)&pointindex, sizeof(XDWORD));
					GRPPOINT* point = new GRPPOINT();
					GGFDATA* pointdata = file->data.Get(pointindex);

					GGFVALUE value;
					value.buffer.Resize(pointdata->buffer.GetSize());
					value.buffer.Set(pointdata->buffer.Get(), pointdata->buffer.GetSize(), 0);
					*point = value.GetGRPPOINT();
					inner->Add(point);
				}

			spline.GetPoints()->Add(inner);
		}

	// Anchors
	GGFDATA* anchorsdata = file->data.Get(anchorsindex);
	XDWORD anchorsnum = anchorsdata->buffer.GetSize()/(4*(sizeof(XDWORD) + 3*sizeof(float)));

	for(XDWORD i = 0; i < anchorsnum; i++)
		{
			GRPPOINT StartA;
			GRPPOINT AnchorA;
			GRPPOINT AnchorB;
			GRPPOINT EndB;

			// Read the specific anchor
			anchorsdata->buffer.Get((XBYTE*)&(StartA.index), sizeof(XDWORD));
			anchorsdata->buffer.Get((XBYTE*)&(StartA.x), sizeof(float));
			anchorsdata->buffer.Get((XBYTE*)&(StartA.y), sizeof(float));
			anchorsdata->buffer.Get((XBYTE*)&(StartA.z), sizeof(float));
			anchorsdata->buffer.Get((XBYTE*)&(StartA.w), sizeof(float));

			anchorsdata->buffer.Get((XBYTE*)&(AnchorA.index), sizeof(XDWORD));
			anchorsdata->buffer.Get((XBYTE*)&(AnchorA.x), sizeof(float));
			anchorsdata->buffer.Get((XBYTE*)&(AnchorA.y), sizeof(float));
			anchorsdata->buffer.Get((XBYTE*)&(AnchorA.z), sizeof(float));
			anchorsdata->buffer.Get((XBYTE*)&(AnchorA.w), sizeof(float));

			anchorsdata->buffer.Get((XBYTE*)&(AnchorB.index), sizeof(XDWORD));
			anchorsdata->buffer.Get((XBYTE*)&(AnchorB.x), sizeof(float));
			anchorsdata->buffer.Get((XBYTE*)&(AnchorB.y), sizeof(float));
			anchorsdata->buffer.Get((XBYTE*)&(AnchorB.z), sizeof(float));
			anchorsdata->buffer.Get((XBYTE*)&(AnchorB.w), sizeof(float));

			anchorsdata->buffer.Get((XBYTE*)&(EndB.index), sizeof(XDWORD));
			anchorsdata->buffer.Get((XBYTE*)&(EndB.x), sizeof(float));
			anchorsdata->buffer.Get((XBYTE*)&(EndB.y), sizeof(float));
			anchorsdata->buffer.Get((XBYTE*)&(EndB.z), sizeof(float));
			anchorsdata->buffer.Get((XBYTE*)&(EndB.w), sizeof(float));


			GRPBEZIERANCHORS* anchor = new GRPBEZIERANCHORS(StartA, AnchorA, AnchorB, EndB);
			spline.GetAnchors()->Add(anchor);
		}

	return true;
}


/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::SetupAnimationEnums
*/	
/**	
//	
//	setups all the enums in animations
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/12/2015 10:58:23
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::SetupAnimationEnums()
{
	//animationtangentmode;

	animationtangentmode.Add(new GGFSTRING( __L("GRPANIMATIONCURVETANGENTMODE_CUBIC")),			GRPANIMATIONCURVETANGENTMODE_CUBIC);
	animationtangentmode.Add(new GGFSTRING( __L("GRPANIMATIONCURVETANGENTMODE_LINEAR")),			GRPANIMATIONCURVETANGENTMODE_LINEAR);
	animationtangentmode.Add(new GGFSTRING( __L("GRPANIMATIONCURVETANGENTMODE_CONSTANT")),		GRPANIMATIONCURVETANGENTMODE_CONSTANT);

	//animationcurvetype;

	animationcurvetype.Add(new GGFSTRING( __L("GRPANIMATIONCURVETYPE_UNKNOWN")),							GRPANIMATIONCURVETYPE_UNKNOWN);
	animationcurvetype.Add(new GGFSTRING( __L("GRPANIMATIONCURVETYPE_TRANSLATION")),					GRPANIMATIONCURVETYPE_TRANSLATION);
	animationcurvetype.Add(new GGFSTRING( __L("GRPANIMATIONCURVETYPE_ROTATION")),						GRPANIMATIONCURVETYPE_ROTATION);
	animationcurvetype.Add(new GGFSTRING( __L("GRPANIMATIONCURVETYPE_SCALE")),								GRPANIMATIONCURVETYPE_SCALE);
	animationcurvetype.Add(new GGFSTRING( __L("GRPANIMATIONCURVETYPE_VISIBILITY")),					GRPANIMATIONCURVETYPE_VISIBILITY);
	animationcurvetype.Add(new GGFSTRING( __L("GRPANIMATIONCURVETYPE_DIFFUSECOLOR")),				GRPANIMATIONCURVETYPE_DIFFUSECOLOR);
	animationcurvetype.Add(new GGFSTRING( __L("GRPANIMATIONCURVETYPE_DIFFUSEFACTOR")),				GRPANIMATIONCURVETYPE_DIFFUSEFACTOR);
	animationcurvetype.Add(new GGFSTRING( __L("GRPANIMATIONCURVETYPE_SPECULARCOLOR")),				GRPANIMATIONCURVETYPE_SPECULARCOLOR);
	animationcurvetype.Add(new GGFSTRING( __L("GRPANIMATIONCURVETYPE_EMISSIVECOLOR")),				GRPANIMATIONCURVETYPE_EMISSIVECOLOR);
	animationcurvetype.Add(new GGFSTRING( __L("GRPANIMATIONCURVETYPE_AMBIENTCOLOR")),				GRPANIMATIONCURVETYPE_AMBIENTCOLOR);
	animationcurvetype.Add(new GGFSTRING( __L("GRPANIMATIONCURVETYPE_SHININESSEXPONENT")),		GRPANIMATIONCURVETYPE_SHININESSEXPONENT);
	animationcurvetype.Add(new GGFSTRING( __L("GRPANIMATIONCURVETYPE_SPECULARFACTOR")),			GRPANIMATIONCURVETYPE_SPECULARFACTOR);
	animationcurvetype.Add(new GGFSTRING( __L("GRPANIMATIONCURVETYPE_TRANSPARENCY")),				GRPANIMATIONCURVETYPE_TRANSPARENCYFACTOR);
	animationcurvetype.Add(new GGFSTRING( __L("GRPANIMATIONCURVETYPE_FOV")),									GRPANIMATIONCURVETYPE_FOV);
	animationcurvetype.Add(new GGFSTRING( __L("GRPANIMATIONCURVETYPE_FIELDOFVIEWX")),				GRPANIMATIONCURVETYPE_FIELDOFVIEWX);
	animationcurvetype.Add(new GGFSTRING( __L("GRPANIMATIONCURVETYPE_FIELDOFVIEWY")),				GRPANIMATIONCURVETYPE_FIELDOFVIEWY);
	animationcurvetype.Add(new GGFSTRING( __L("GRPANIMATIONCURVETYPE_INTENSITY")),						GRPANIMATIONCURVETYPE_INTENSITY);
	animationcurvetype.Add(new GGFSTRING( __L("GRPANIMATIONCURVETYPE_ROLL")),								GRPANIMATIONCURVETYPE_ROLL);
	animationcurvetype.Add(new GGFSTRING( __L("GRPANIMATIONCURVETYPE_PITCH")),								GRPANIMATIONCURVETYPE_PITCH);
	animationcurvetype.Add(new GGFSTRING( __L("GRPANIMATIONCURVETYPE_YAW")),									GRPANIMATIONCURVETYPE_YAW);
	animationcurvetype.Add(new GGFSTRING( __L("GRPANIMATIONCURVETYPE_DEFORMPERCENT")),				GRPANIMATIONCURVETYPE_DEFORMPERCENT);
	animationcurvetype.Add(new GGFSTRING( __L("GRPANIMATIONCURVETYPE_USERVALUE")),						GRPANIMATIONCURVETYPE_USERVALUE);
	animationcurvetype.Add(new GGFSTRING( __L("GRPANIMATIONCURVETYPE_MAX")),									GRPANIMATIONCURVETYPE_MAX);


	//animationcurveaxis;

	animationcurveaxis.Add(new GGFSTRING( __L("GRPANIMATIONCURVEAXIS_VISIBILITY")),					GRPANIMATIONCURVEAXIS_VISIBILITY);
	animationcurveaxis.Add(new GGFSTRING( __L("GRPANIMATIONCURVEAXIS_X")),										GRPANIMATIONCURVEAXIS_X);
	animationcurveaxis.Add(new GGFSTRING( __L("GRPANIMATIONCURVEAXIS_Y")),										GRPANIMATIONCURVEAXIS_Y);
	animationcurveaxis.Add(new GGFSTRING( __L("GRPANIMATIONCURVEAXIS_Z")),										GRPANIMATIONCURVEAXIS_Z);
	animationcurveaxis.Add(new GGFSTRING( __L("GRPANIMATIONCURVEAXIS_YAW")),									GRPANIMATIONCURVEAXIS_YAW);
	animationcurveaxis.Add(new GGFSTRING( __L("GRPANIMATIONCURVEAXIS_PITCH")),								GRPANIMATIONCURVEAXIS_PITCH);
	animationcurveaxis.Add(new GGFSTRING( __L("GRPANIMATIONCURVEAXIS_ROLL")),								GRPANIMATIONCURVEAXIS_ROLL);
	animationcurveaxis.Add(new GGFSTRING( __L("GRPANIMATIONCURVECOLOR_R")),									GRPANIMATIONCURVECOLOR_R);
	animationcurveaxis.Add(new GGFSTRING( __L("GRPANIMATIONCURVECOLOR_G")),									GRPANIMATIONCURVECOLOR_G);
	animationcurveaxis.Add(new GGFSTRING( __L("GRPANIMATIONCURVECOLOR_B")),									GRPANIMATIONCURVECOLOR_B);
	animationcurveaxis.Add(new GGFSTRING( __L("GRPANIMATIONCURVECOLOR_A")),									GRPANIMATIONCURVECOLOR_A);
	animationcurveaxis.Add(new GGFSTRING( __L("GRPANIMATIONCURVECOLOR_DIFFUSEFACTOR")),			GRPANIMATIONCURVECOLOR_DIFFUSEFACTOR);
	animationcurveaxis.Add(new GGFSTRING( __L("GRPANIMATIONCURVECOLOR_SPECULARFACTOR")),			GRPANIMATIONCURVECOLOR_SPECULARFACTOR);
	animationcurveaxis.Add(new GGFSTRING( __L("GRPANIMATIONCURVECOLOR_TRANSPARENCYFACTOR")), GRPANIMATIONCURVECOLOR_TRANSPARENCYFACTOR);
	animationcurveaxis.Add(new GGFSTRING( __L("GRPANIMATIONCURVECOLOR_INTENSITY")),					GRPANIMATIONCURVECOLOR_INTENSITY);
	animationcurveaxis.Add(new GGFSTRING( __L("GRPANIMATIONCURVEFOV_FIELDOFVIEW")),					GRPANIMATIONCURVEFOV_FIELDOFVIEW);
	animationcurveaxis.Add(new GGFSTRING( __L("GRPANIMATIONCURVEFOV_FIELDOFVIEWX")),					GRPANIMATIONCURVEFOV_FIELDOFVIEWX);
	animationcurveaxis.Add(new GGFSTRING( __L("GRPANIMATIONCURVEFOV_FIELDOFVIEWY")),					GRPANIMATIONCURVEFOV_FIELDOFVIEWY);
	animationcurveaxis.Add(new GGFSTRING( __L("GRPANIMATIONCURVETYPE_SHININESSFACTOR")),			GRPANIMATIONCURVETYPE_SHININESSFACTOR);
	animationcurveaxis.Add(new GGFSTRING( __L("GRPANIMATIONCURVEAXIS_DEFORMPERCENT")),				GRPANIMATIONCURVEAXIS_DEFORMPERCENT);
	animationcurveaxis.Add(new GGFSTRING( __L("GRPANIMATIONCURVEAXIS_UNKNOWN")),							GRPANIMATIONCURVEAXIS_UNKNOWN);
	animationcurveaxis.Add(new GGFSTRING( __L("GRPANIMATIONCURVEAXIS_USERVALUE")),						GRPANIMATIONCURVEAXIS_USERVALUE);
	animationcurveaxis.Add(new GGFSTRING( __L("GRPANIMATIONCURVEAXIS_MAX")),									GRPANIMATIONCURVEAXIS_MAX);

	//animationloopmode;

	animationloopmode.Add(new GGFSTRING( __L("GRPANIMATIONLOOPMODE_NONE")),									GRPANIMATIONLOOPMODE_NONE);
	animationloopmode.Add(new GGFSTRING( __L("GRPANIMATIONLOOPMODE_REPEAT")),								GRPANIMATIONLOOPMODE_REPEAT);

	//animationstatus;

	animationstatus.Add(new GGFSTRING( __L("GRPANIMATION_STATUS_UNKNOWN")),									GRPANIMATION_STATUS_UNKNOWN);
	animationstatus.Add(new GGFSTRING( __L("GRPANIMATION_STATUS_STARTED")),									GRPANIMATION_STATUS_STARTED);
	animationstatus.Add(new GGFSTRING( __L("GRPANIMATION_STATUS_PLAYING")),									GRPANIMATION_STATUS_PLAYING);
	animationstatus.Add(new GGFSTRING( __L("GRPANIMATION_STATUS_STOPPED")),									GRPANIMATION_STATUS_STOPPED);
	animationstatus.Add(new GGFSTRING( __L("GRPANIMATION_STATUS_FINISHED")),									GRPANIMATION_STATUS_FINISHED);
	animationstatus.Add(new GGFSTRING( __L("GRPANIMATION_STATUS_PAUSED")),										GRPANIMATION_STATUS_PAUSED);
	animationstatus.Add(new GGFSTRING( __L("GRPANIMATION_STATUS_INACTIVE")),									GRPANIMATION_STATUS_INACTIVE);

	return true;
}


/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::CleanAnimationEnums
*/	
/**	
//	
//	cleans all the animation enums
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/12/2015 10:58:34
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPSERIALIZERGGF::CleanAnimationEnums()
{
	//animationtangentmode;

	for(XDWORD i = 0; i < animationtangentmode.GetSize(); i++)
	{
		delete animationtangentmode.GetKey(i);
	}


	//animationcurvetype;

	for(XDWORD i = 0; i < animationcurvetype.GetSize(); i++)
	{
		delete animationcurvetype.GetKey(i);
	}

	//animationcurveaxis;

	for(XDWORD i = 0; i < animationcurveaxis.GetSize(); i++)
	{
		delete animationcurveaxis.GetKey(i);
	}

	//animationloopmode;

	for(XDWORD i = 0; i < animationloopmode.GetSize(); i++)
	{
		delete animationloopmode.GetKey(i);
	}

	//animationstatus;

	for(XDWORD i = 0; i < animationstatus.GetSize(); i++)
	{
		delete animationstatus.GetKey(i);
	}

	return true;
}




/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::GetTangentKey
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/12/2015 11:14:24
//	
//	@return 			GGFSTRING* : 
//
//  @param				e : 
*/
/*-----------------------------------------------------------------*/
GGFSTRING* GRPSERIALIZERGGF::GetTangentKey(GRPANIMATIONCURVETANGENTMODE e)
{
	for(XDWORD i = 0; i < animationtangentmode.GetSize(); i++)
		{
			if(animationtangentmode.GetElement(i) == e)
				{
					return animationtangentmode.GetKey(i);
				}
		}

	return NULL;
}



/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::GetTangentValue
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/12/2015 11:14:31
//	
//	@return 			GRPANIMATIONCURVETANGENTMODE : 
//
//  @param				str : 
*/
/*-----------------------------------------------------------------*/
GRPANIMATIONCURVETANGENTMODE GRPSERIALIZERGGF::GetTangentValue(GGFSTRING* str)
{
	for(XDWORD i = 0; i < animationtangentmode.GetSize(); i++)
		{
			if(*animationtangentmode.GetKey(i) == *str)
				{
					return (GRPANIMATIONCURVETANGENTMODE)animationtangentmode.GetElement(i);
				}
		}

	return GRPANIMATIONCURVETANGENTMODE_LINEAR;
}





/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::GetTangentValue
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/12/2015 11:14:43
//	
//	@return 			GRPANIMATIONCURVETANGENTMODE : 
//
//  @param				str : 
*/
/*-----------------------------------------------------------------*/
GRPANIMATIONCURVETANGENTMODE GRPSERIALIZERGGF::GetTangentValue(XCHAR* str)
{
	XSTRING string(str);

	for(XDWORD i = 0; i < animationtangentmode.GetSize(); i++)
		{
			if(*animationtangentmode.GetKey(i) == string)
				{
					return (GRPANIMATIONCURVETANGENTMODE)animationtangentmode.GetElement(i);
				}
		}

	return GRPANIMATIONCURVETANGENTMODE_LINEAR;
}




/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::GetTypeKey
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/12/2015 11:14:49
//	
//	@return 			GGFSTRING* : 
//
//  @param				e : 
*/
/*-----------------------------------------------------------------*/
GGFSTRING* GRPSERIALIZERGGF::GetTypeKey(GRPANIMATIONCURVETYPE e)
{
	for(XDWORD i = 0; i < animationcurvetype.GetSize(); i++)
		{
			if(animationcurvetype.GetElement(i) == e)
				{
					return animationcurvetype.GetKey(i);
				}
		}

	return NULL;
}




/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::GetTypeValue
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/12/2015 11:14:57
//	
//	@return 			GRPANIMATIONCURVETYPE : 
//
//  @param				str : 
*/
/*-----------------------------------------------------------------*/
GRPANIMATIONCURVETYPE GRPSERIALIZERGGF::GetTypeValue(GGFSTRING* str)
{
	for(XDWORD i = 0; i < animationcurvetype.GetSize(); i++)
		{
			if(*animationcurvetype.GetKey(i) == *str)
				{
					return (GRPANIMATIONCURVETYPE)animationcurvetype.GetElement(i);
				}
		}
	
	return GRPANIMATIONCURVETYPE_UNKNOWN;
}




/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::GetTypeValue
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/12/2015 11:15:03
//	
//	@return 			GRPANIMATIONCURVETYPE : 
//
//  @param				str : 
*/
/*-----------------------------------------------------------------*/
GRPANIMATIONCURVETYPE GRPSERIALIZERGGF::GetTypeValue(XCHAR* str)
{
	XSTRING string(str);

	for(XDWORD i = 0; i < animationcurvetype.GetSize(); i++)
		{
			if(*animationcurvetype.GetKey(i) == string)
				{
					return (GRPANIMATIONCURVETYPE)animationcurvetype.GetElement(i);
				}
		}

	return GRPANIMATIONCURVETYPE_UNKNOWN;
}




/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::GetAxisKey
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/12/2015 11:15:11
//	
//	@return 			GGFSTRING* : 
//
//  @param				e : 
*/
/*-----------------------------------------------------------------*/
GGFSTRING* GRPSERIALIZERGGF::GetAxisKey(GRPANIMATIONCURVEAXIS e)
{
	for(XDWORD i = 0; i < animationcurveaxis.GetSize(); i++)
		{
			if(animationcurveaxis.GetElement(i) == e)
				{
					return animationcurveaxis.GetKey(i);
				}
		}

	return NULL;
}




/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::GetAxisValue
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/12/2015 11:15:18
//	
//	@return 			GRPANIMATIONCURVEAXIS : 
//
//  @param				str : 
*/
/*-----------------------------------------------------------------*/
GRPANIMATIONCURVEAXIS GRPSERIALIZERGGF::GetAxisValue(GGFSTRING* str)
{
	for(XDWORD i = 0; i < animationcurveaxis.GetSize(); i++)
		{
			if(*animationcurveaxis.GetKey(i) == *str)
			{
				return (GRPANIMATIONCURVEAXIS)animationcurveaxis.GetElement(i);
			}
		}

	return GRPANIMATIONCURVEAXIS_UNKNOWN;
}




/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::GetAxisValue
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/12/2015 11:15:24
//	
//	@return 			GRPANIMATIONCURVEAXIS : 
//
//  @param				str : 
*/
/*-----------------------------------------------------------------*/
GRPANIMATIONCURVEAXIS GRPSERIALIZERGGF::GetAxisValue(XCHAR* str)
{
	XSTRING string(str);

	for(XDWORD i = 0; i < animationcurveaxis.GetSize(); i++)
		{
			if(*animationcurveaxis.GetKey(i) == string)
			{
				return (GRPANIMATIONCURVEAXIS)animationcurveaxis.GetElement(i);
			}
		}

	return GRPANIMATIONCURVEAXIS_UNKNOWN;
}




/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::GetLoopKey
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/12/2015 11:15:31
//	
//	@return 			GGFSTRING* : 
//
//  @param				e : 
*/
/*-----------------------------------------------------------------*/
GGFSTRING* GRPSERIALIZERGGF::GetLoopKey(GRPANIMATIONLOOPMODE e)
{
	for(XDWORD i = 0; i < animationloopmode.GetSize(); i++)
		{
			if(animationloopmode.GetElement(i) == e)
			{
				return animationloopmode.GetKey(i);
			}
		}

	return NULL;
}




/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::GetLoopValue
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/12/2015 11:15:37
//	
//	@return 			GRPANIMATIONLOOPMODE : 
//
//  @param				str : 
*/
/*-----------------------------------------------------------------*/
GRPANIMATIONLOOPMODE GRPSERIALIZERGGF::GetLoopValue(GGFSTRING* str)
{
	for(XDWORD i = 0; i < animationloopmode.GetSize(); i++)
		{
			if(*animationloopmode.GetKey(i) == *str)
			{
				return (GRPANIMATIONLOOPMODE)animationloopmode.GetElement(i);
			}
		}

	return GRPANIMATIONLOOPMODE_NONE;
}





/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::GetLoopValue
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/12/2015 11:15:44
//	
//	@return 			GRPANIMATIONLOOPMODE : 
//
//  @param				str : 
*/
/*-----------------------------------------------------------------*/
GRPANIMATIONLOOPMODE GRPSERIALIZERGGF::GetLoopValue(XCHAR* str)
{
	XSTRING string(str);

	for(XDWORD i = 0; i < animationloopmode.GetSize(); i++)
		{
			if(*animationloopmode.GetKey(i) == string)
			{
				return (GRPANIMATIONLOOPMODE)animationloopmode.GetElement(i);
			}
		}

	return GRPANIMATIONLOOPMODE_NONE;
}




/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::GetStatusKey
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/12/2015 11:15:51
//	
//	@return 			GGFSTRING* : 
//
//  @param				e : 
*/
/*-----------------------------------------------------------------*/
GGFSTRING* GRPSERIALIZERGGF::GetStatusKey(GRPANIMATION_STATUS e)
{
	for(XDWORD i = 0; i < animationstatus.GetSize(); i++)
		{
			if(animationstatus.GetElement(i) == e)
			{
				return animationstatus.GetKey(i);
			}
		}

	return NULL;
}




/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::GetStatusValue
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/12/2015 11:15:57
//	
//	@return 			GRPANIMATION_STATUS : 
//
//  @param				str : 
*/
/*-----------------------------------------------------------------*/
GRPANIMATION_STATUS GRPSERIALIZERGGF::GetStatusValue(GGFSTRING* str)
{
	for(XDWORD i = 0; i < animationstatus.GetSize(); i++)
		{
			if(*animationstatus.GetKey(i) == *str)
			{
				return (GRPANIMATION_STATUS)animationstatus.GetElement(i);
			}
		}

	return GRPANIMATION_STATUS_UNKNOWN;
}





/*-------------------------------------------------------------------
//	GRPSERIALIZERGGF::GetStatusValue
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/12/2015 11:16:04
//	
//	@return 			GRPANIMATION_STATUS : 
//
//  @param				str : 
*/
/*-----------------------------------------------------------------*/
GRPANIMATION_STATUS GRPSERIALIZERGGF::GetStatusValue(XCHAR* str)
{
	XSTRING string(str);

	for(XDWORD i = 0; i < animationstatus.GetSize(); i++)
		{
			if(*animationstatus.GetKey(i) == string)
			{
				return (GRPANIMATION_STATUS)animationstatus.GetElement(i);
			}
		}

	return GRPANIMATION_STATUS_UNKNOWN;
}
