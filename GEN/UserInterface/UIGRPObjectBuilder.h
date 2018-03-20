/*------------------------------------------------------------------------------------------
//	UIGRPOBJECTBUILDER.H
*/	
/**	
// \file 
//   
//  build and caches corresponding GRPElements to use by the UI
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 09/08/2016 17:03:02
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _UIGRPOBJECTBUILDER_H_
#define _UIGRPOBJECTBUILDER_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
#include "XBinaryMap.h"
	
#include "XString.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
// struct that defines an object property

class GRPSCENE;
class GRPOBJECT;
class GRPBB;
class GRPELEMENT;
class GRPMATERIAL;
class GRPTEXTBOX;
class UI3DOBJECTLOADER;

//bool operator<(const UIGRPOBJECTBUILDER::PROPERTIES lhs, const UIGRPOBJECTBUILDER::PROPERTIES rhs);

class UIGRPOBJECTBUILDER
{
public:

	struct PROPERTIES
	{
																							PROPERTIES																		();
																							PROPERTIES																		(const PROPERTIES& rhs);

		PROPERTIES&																operator=																			(const PROPERTIES& rhs);
		bool																			operator==																		(const PROPERTIES& rhs);
		bool																			operator!=																		(const PROPERTIES& rhs);
		bool																			operator<																			(const PROPERTIES& rhs);
		bool																			operator<																			(PROPERTIES rhs);

		float																			cornerradious;

	private:
		void																			Clean																					()
																							{
																								cornerradious = 0;
																							}
	};

	struct TEXTPROPERTIES
	{
																							TEXTPROPERTIES																();
																							TEXTPROPERTIES																(const TEXTPROPERTIES& rhs);
																							~TEXTPROPERTIES();

		TEXTPROPERTIES&														operator=																			(const TEXTPROPERTIES& rhs);
		bool																			operator==																		(const TEXTPROPERTIES& rhs);
		bool																			operator!=																		(const TEXTPROPERTIES& rhs);
		bool																			operator<																			(const TEXTPROPERTIES& rhs);
		bool																			operator<																			(TEXTPROPERTIES rhs);

		XSTRING* string; // poinetr because by copy weird things seem to happen
		bool bold;
		bool italic;

	private:
		void																			Clean																					()
																							{
																								string	= NULL;
																								bold		= false;
																								italic	= false;
																							}
	};

	struct OBJECTENTRY
	{
	public:
																							OBJECTENTRY																		()
																							{
																								Clean();
																							}

																							OBJECTENTRY																		(const OBJECTENTRY& rhs);

		OBJECTENTRY&															operator=																			(const OBJECTENTRY& rhs);
		bool																			operator==																		(const OBJECTENTRY& rhs);
		bool																			operator!=																		(const OBJECTENTRY& rhs);
		bool																			operator<																			(const OBJECTENTRY& rhs);
		bool																			operator<																			(OBJECTENTRY rhs);

																							GRPOBJECT* obj;
																							GRPBB* bb;
	protected:
	private:
		void																			Clean																					()
																							{
																								obj = NULL;
																								bb = NULL;
																							}
	};

																							UIGRPOBJECTBUILDER														(GRPSCENE* scene);
	virtual																		 ~UIGRPOBJECTBUILDER														();

	GRPELEMENT*																	GetElement																		(PROPERTIES properties);
	GRPTEXTBOX*																	GetText																				(TEXTPROPERTIES properties);
	OBJECTENTRY*																Get3DObject																		(XCHAR* path);

protected:

	UI3DOBJECTLOADER*														objectloader;
	GRPSCENE*																		scene;
	GRPMATERIAL *																material;
	XBINARYMAP<PROPERTIES, GRPELEMENT*>					elements;
	XBINARYMAP<TEXTPROPERTIES, GRPTEXTBOX*>			texts;
	XBINARYMAP<XSTRING, OBJECTENTRY*>						objects;

private:
	void																				Clean																					()
																							{
																								objectloader	= NULL;
																								scene					= NULL;
																								material			= NULL;
																							}
};

	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

