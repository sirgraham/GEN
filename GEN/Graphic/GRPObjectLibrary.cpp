
/*------------------------------------------------------------------------------------------
//	GRPOBJECTLIBRARY.CPP
//	
//	Manages mesh library
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 08/09/2016 12:17:41
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "grpobjectlibrary.h"
#include "GRPSubject.h"
#include "GRPElement.h"

#include "GRPLoader.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/

GRPOBJECTLIBRARY* GRPOBJECTLIBRARY::instance=NULL;



/*-------------------------------------------------------------------
//	XCHAR*
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			14/09/2016 14:10:08
//	
//	@return 			GRPOBJECTLIBRARYITEM::operator : 
//
*/
/*-----------------------------------------------------------------*/
GRPOBJECTLIBRARYITEM::operator XCHAR* ()
{
	return name.Get();
}

/*-------------------------------------------------------------------
//	GRPMESH*
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			14/09/2016 14:10:25
//	
//	@return 			GRPOBJECTLIBRARYITEM::operator : 
//
*/
/*-----------------------------------------------------------------*/
GRPOBJECTLIBRARYITEM::operator GRPMESH* ()
{
	return mesh;
}

/*-------------------------------------------------------------------
//	GRPMATERIAL*
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			14/09/2016 14:10:33
//	
//	@return 			GRPOBJECTLIBRARYITEM::operator : 
//
*/
/*-----------------------------------------------------------------*/
GRPOBJECTLIBRARYITEM::operator GRPMATERIAL* ()
{
	return material;
}

/*-------------------------------------------------------------------
//	GRPSHADERPROGRAM*
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			14/09/2016 14:10:41
//	
//	@return 			GRPOBJECTLIBRARYITEM::operator : 
//
*/
/*-----------------------------------------------------------------*/
GRPOBJECTLIBRARYITEM::operator GRPSHADERPROGRAM* ()
{
	return shader;
}


/*-------------------------------------------------------------------
//	GRPELEMENT*
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			14/09/2016 14:10:49
//	
//	@return 			GRPOBJECTLIBRARYITEM::operator : 
//
*/
/*-----------------------------------------------------------------*/
GRPOBJECTLIBRARYITEM::operator GRPELEMENT* ()
{
	GRPELEMENT* element=new GRPELEMENT();
							element->SetNode(new GRPNODE());
							element->Reference		(mesh);
							element->SetProgram		(shader);

							if (element->GetNode())
									element->GetNode()->SetMaterial	(material);

	return element;
}


/*-------------------------------------------------------------------
//	GRPOBJECTLIBRARY::Get
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			08/09/2016 12:19:52
//	
//	@return 			GRPOBJECTLIBRARY* : 
//
*/
/*-----------------------------------------------------------------*/
GRPOBJECTLIBRARY* GRPOBJECTLIBRARY::Get()
{
	if (GRPOBJECTLIBRARY::instance==NULL)
			GRPOBJECTLIBRARY::instance=new GRPOBJECTLIBRARY();
	return GRPOBJECTLIBRARY::instance;
}

/*-------------------------------------------------------------------
//	GRPOBJECTLIBRARY::Destroy
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			08/09/2016 12:19:58
//	
//	@return 			GRPOBJECTLIBRARY* : 
//
*/
/*-----------------------------------------------------------------*/
void GRPOBJECTLIBRARY::Destroy()
{
		if (GRPOBJECTLIBRARY::instance!=NULL)
			delete(GRPOBJECTLIBRARY::instance);
			
	GRPOBJECTLIBRARY::instance=NULL;
}

/*-------------------------------------------------------------------
//	GRPOBJECTLIBRARY::~GRPOBJECTLIBRARY
*/	
/**	
//	
//	 Class Destructor GRPOBJECTLIBRARY
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			08/09/2016 12:23:57
//	
*/
/*-----------------------------------------------------------------*/
GRPOBJECTLIBRARY::~GRPOBJECTLIBRARY()
{
	Publish(GRPMSG_DESTROYED);

	this->meshes.DeleteKeyContents			();
	this->meshes.DeleteElementContents	();
	this->meshes.DeleteAll							();

	this->library.DeleteKeyContents			();
	this->library.DeleteElementContents	();	
	this->library.DeleteAll							();

	delete(loader);
}




/*-------------------------------------------------------------------
//	GRPOBJECTLIBRARY::IfExists
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			09/09/2016 12:52:38
//	
//	@return 			bool : 
//
//  @param				name : 
*/
/*-----------------------------------------------------------------*/
int GRPOBJECTLIBRARY::GetMeshIndex(XCHAR* name)
{
	for (XDWORD e=0;e<meshes.GetSize();e++)
	{
		if (meshes.GetKey(e)->Compare(name)==0)
		{
			return e;
		}
	}

	return -1;
}

/*-------------------------------------------------------------------
//	GRPOBJECTLIBRARY::GetMesh
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			09/09/2016 12:56:44
//	
//	@return 			GRPMESH* : 
//
//  @param				name : 
*/
/*-----------------------------------------------------------------*/
GRPMESH* GRPOBJECTLIBRARY::GetMesh(XCHAR* name)
{
		for (XDWORD e=0;e<meshes.GetSize();e++)
	{
		if (meshes.GetKey(e)->Compare(name)==0)
		{
			return meshes.GetElement(e);
		}
	}
	return NULL;
}

/*-------------------------------------------------------------------
//	GRPOBJECTLIBRARY::Create
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			14/09/2016 12:23:08
//	
//	@return 			GRPELEMENT* : 
//
//  @param				name : 
*/
/*-----------------------------------------------------------------*/
GRPELEMENT*	GRPOBJECTLIBRARY::CreateElement(XCHAR* name)
{
	GRPOBJECTLIBRARYITEM* item=GetElement(name);
	if (item==NULL)
		return NULL;

	return (GRPELEMENT*)*item;
}

GRPOBJECT*	GRPOBJECTLIBRARY::CreateObject			(XCHAR* name)
{
	return NULL;
}

/*-------------------------------------------------------------------
//	GRPOBJECTLIBRARY::IndexElement
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			09/09/2016 13:48:08
//	
//	@return 			bool : 
//
//  @param				name : 
//  @param				mesh : 
*/
/*-----------------------------------------------------------------*/
bool GRPOBJECTLIBRARY::IndexElement(XCHAR* name,GRPELEMENT* element)
{
	GRPOBJECTLIBRARYITEM* item=new GRPOBJECTLIBRARYITEM();
												item->name=name;												
												item->mesh=element->GetMesh();												
												item->shader=element->GetProgram();
												if (element->GetNode())
												item->material=element->GetNode()->GetMaterial();
												else
													item->material=NULL;

	XSTRING*	string=new XSTRING();
						string->Set(name);
	this->library.Add(string,item);
	return true;
}

/*-------------------------------------------------------------------
//	GRPOBJECTLIBRARY::GetElement
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			09/09/2016 12:52:55
//	
//	@return 			bool : 
//
//  @param				name : 
*/
/*-----------------------------------------------------------------*/
GRPOBJECTLIBRARYITEM* GRPOBJECTLIBRARY::GetElement(XCHAR* name)
{
	for (XDWORD e=0;e<library.GetSize();e++)
	{
		if (library.GetKey(e)->Compare(name)==0)
		{
			return library.GetElement(e);
		}
	}
	return NULL;
}


/*-------------------------------------------------------------------
//	GRPOBJECTLIBRARY::IndexMesh
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			09/09/2016 12:53:15
//	
//	@return 			bool : 
//
//  @param				name : 
//  @param				mesh : 
*/
/*-----------------------------------------------------------------*/
bool GRPOBJECTLIBRARY::IndexMesh(XCHAR* name,GRPMESH* mesh)
{
	int n=GetMeshIndex(name);
	if (n==-1)
	{
		XSTRING* string=new XSTRING(name);
		return meshes.Add(string,mesh);		
	}
		
	return meshes.Set(meshes.GetKey(n),mesh);	
}


/*-------------------------------------------------------------------
//	GRPOBJECTLIBRARY::GetLoader
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/10/2016 17:09:07
//	
//	@return 			GRPLOADER* : 
//
*/
/*-----------------------------------------------------------------*/
GRPLOADER*	 GRPOBJECTLIBRARY::GetLoader() 
{
	return loader; 
}

/*-------------------------------------------------------------------
//	GRPOBJECTLIBRARY::GetSubjectType
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			08/09/2016 12:26:02
//	
//	@return 			XDWORD : GRPNAMED_TYPE_OBJECT_LIBRARY
//
*/
/*-----------------------------------------------------------------*/
XDWORD GRPOBJECTLIBRARY::GetSubjectType()
{
	return GRPNAMED_TYPE_OBJECT_LIBRARY;
}

/*-------------------------------------------------------------------
//	GRPOBJECTLIBRARY::GRPOBJECTLIBRARY
*/	
/**	
//	
//	Class Constructor GRPOBJECTLIBRARY
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			08/09/2016 12:23:01
//	
*/
/*-----------------------------------------------------------------*/
GRPOBJECTLIBRARY::GRPOBJECTLIBRARY() 
{
	Clean();
	this->loader=new GRPLOADER();
}