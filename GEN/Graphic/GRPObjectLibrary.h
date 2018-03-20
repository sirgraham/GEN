/*------------------------------------------------------------------------------------------
//	GRPOBJECTLIBRARY.H
*/	
/**	
// \class 
//   
//  Object Library
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 02/09/2016 10:27:51
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPOBJECTLIBRARY_H_
#define _GRPOBJECTLIBRARY_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "XMap.h"
#include "GRPSubject.h"
#include "GRPElement.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
class GRPMESH;
class XSTRING;
class GRPMATERIAL;
class GRPSHADERPROGRAM;
class GRPELEMENT;

/*---- CLASS -----------------------------------------------------------------------------*/

class GRPOBJECTLIBRARYITEM : public GRPNAMED
{
public:

		XSTRING										file;
		GRPMATERIAL*							material;					
		GRPMESH*									mesh;					
		GRPSHADERPROGRAM*					shader;

		operator XCHAR*						();
		operator GRPMESH*					();
		operator GRPMATERIAL*			();
		operator GRPSHADERPROGRAM*();
		operator GRPELEMENT*			();
};

class GRPLOADER;
class GRPOBJECT;

class GRPOBJECTLIBRARY : public GRPSUBJECT
{
public:
	
	static GRPOBJECTLIBRARY*												Get								();
	static void																			Destroy						();	
	virtual	XDWORD																	GetSubjectType		();
	virtual																					~GRPOBJECTLIBRARY	();


	int																							GetMeshIndex			(XCHAR* name);
	GRPOBJECTLIBRARYITEM*														GetElement				(XCHAR* name);
	GRPMESH*																				GetMesh						(XCHAR* name);
	GRPELEMENT*																			CreateElement			(XCHAR* name);
	GRPOBJECT*																			CreateObject			(XCHAR* name);

	bool																						IndexElement			(XCHAR* name,	GRPELEMENT*		mesh);
	bool																						IndexMesh					(XCHAR* name,	GRPMESH*			mesh);

	GRPLOADER*																			GetLoader					();

protected:



static		GRPOBJECTLIBRARY*												instance;
																									GRPOBJECTLIBRARY	();

					GRPLOADER*															loader;
					XMAP<XSTRING*,GRPMESH*>									meshes;
					XMAP<XSTRING*,GRPOBJECTLIBRARYITEM*>		library;
					XMAP<XSTRING*,GRPOBJECT*>								entities;

private:

					void Clean()
					{
						loader=NULL;
					}
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

