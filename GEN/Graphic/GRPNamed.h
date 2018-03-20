/*------------------------------------------------------------------------------------------
//	GRPNAMED.H
*/	
/**	
// \class 
//   
//  Virtual name interface
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 24/02/2015 16:17:02
//	Last Modification	:	
*/	
//*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPNAMED_H_
#define _GRPNAMED_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XFString.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

enum GRPNAMED_TYPE
{
	GRPNAMED_TYPE_UNKNOWN = 0x0,
	GRPNAMED_TYPE_GRAPHICSETTINGS,
	GRPNAMED_TYPE_GRPANIMATIONCURVE,
	GRPNAMED_TYPE_GRPANIMATIONLAYER,
	GRPNAMED_TYPE_GRPANIMATIONNODE,
	GRPNAMED_TYPE_GRPANIMATIONSTACK,
	GRPNAMED_TYPE_GRPDEFORMER,
	GRPNAMED_TYPE_GRPSUBDEFORMER,
	GRPNAMED_TYPE_GRPELEMENT,
	GRPNAMED_TYPE_GRPINTERFACEANIMABLE,
	GRPNAMED_TYPE_GRPOBJECT,
	GRPNAMED_TYPE_GRPSKINCLUSTER,
	GRPNAMED_TYPE_GRPSKINPOSE,
	GRPNAMED_TYPE_GRPSKIN,
	GRPNAMED_TYPE_GRPPARTICLEPOOL,
	GRPNAMED_TYPE_GRPPARTICLESYSTEM,	
	GRPNAMED_TYPE_GRPMATERIAL,
	GRPNAMED_TYPE_GRPTEXTURE,
	GRPNAMED_TYPE_GRPNODE,
	GRPNAMED_TYPE_GRPTEXTBOX,
	GRPNAMED_TYPE_GRPCONSOLE,
	//GRPNAMED_TYPE_GRPSPRITE,
	//GRPNAMED_TYPE_GRPSPRITEPOINT,
	//GRPNAMED_TYPE_GRPSPRITEQUAD,
	//GRPNAMED_TYPE_GRPSPRITESTRIP,
	GRPNAMED_TYPE_GRPCAMERA,
	GRPNAMED_TYPE_GRPLIGHT,
	GRPNAMED_TYPE_GRPPATH,
	GRPNAMED_TYPE_GRPSKINLIMB,
	GRPNAMED_TYPE_GRPBILLBOARD,
	GRPNAMED_TYPE_GRPSCENE,
	GRPNAMED_TYPE_GRPMILESTONE,
	GRPNAMED_TYPE_GRPSTATE,
	GRPNAMED_TYPE_GRPLAYER,
	GRPNAMED_TYPE_GRPOVERLAY,
	GRPNAMED_TYPE_GRPPASS,
	GRPNAMED_TYPE_OBJECT_LIBRARY,
	GRPNAMED_TYPE_TASK,
	GRPNAMED_TYPE_TASKMANAGER,
	GRPNAMED_TYPE_FONT,
	GRP_NAMED_TYPE_SHADER,
	GRPNAMED_TYPE_MAX,	
	GRPNAMED_TYPE_WIDGET,
	AI_LOGIC,
	AI_CONTROLLER,
	AI_ANIMATION,
	AI_STATE,
	AI_OVERLAY,
	SND_MANAGER,
	SND_EVENT
	
};
	
/*---- CLASS -----------------------------------------------------------------------------*/


class GRPNAMED
{
	public:

										GRPNAMED			()	              { Clean();																	}
										GRPNAMED			(XCHAR* name)			{ Clean(); this->name=name;									}
		virtual        ~GRPNAMED			()	              { Clean();																	}
	
		virtual XQWORD					GetTag				()								{ return this->tag;													}
		virtual void						SetTag				(XQWORD tag)			{ this->tag =	tag;													}
		virtual XFSTRING*				GetName				()								{ return &name;															}
		virtual void						SetName				(XCHAR* name)			{ this->name.Set(name);											}
	
						GRPNAMED_TYPE		GetType				()								{ return this->classtype;										}

		XQWORD					tag;
		XFSTRING				name;
	
		GRPNAMED_TYPE		classtype;

	private:

		void            Clean         ()
										{
											tag = 0;
											classtype = GRPNAMED_TYPE_UNKNOWN;
										}
};
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

