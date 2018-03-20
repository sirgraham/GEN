/*------------------------------------------------------------------------------------------
//	GRPSCENE.H
*/	
/**	
// \class 
//   
//  Holds scene objects, cameras and lights, and renderers. World Matrix. 
//  Render Events
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 26/06/2014 17:04:42
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPSCENE_H_
#define _GRPSCENE_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/


#include "XVector.h"
#include "XEvent.h"
#include "XSubject.h"

#include "GRPPass.h"
#include "GRPMatrix.h"
#include "GRPColor.h"
#include "GRPShaderLibrary.h"
#include "GRPElement.h"
#include "GRPVisualDebugger.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

enum GRPSCENE_INIT_FLAGS
{
	GRPSCENE_INIT_FLAGS_NONE							=		0,
	GRPSCENE_INIT_FLAGS_CREATE_PASS				=		1<<0,
	GRPSCENE_INIT_FLAGS_CREATE_CAMERA			=		1<<1,
	GRPSCENE_INIT_FLAGS_CREATE_VIEWPORT		=		1<<2,	
	GRPSCENE_INIT_FLAGS_CREATE_ALL				=		GRPSCENE_INIT_FLAGS_CREATE_VIEWPORT		| 
																						GRPSCENE_INIT_FLAGS_CREATE_CAMERA			| 
																						GRPSCENE_INIT_FLAGS_CREATE_PASS
};


/*---- CLASS -----------------------------------------------------------------------------*/

class XPATHS;	
class XFACTORY;
class GRPRENDERER; 
class GRPPATH;
class GRPOBJECT;
class GRPVIEWPORT;
class GRPSCREEN;
class GRPSCENE;
class GRPLIGHT;
class GRPMATERIALLIBRARY;
class GRPMATERIAL;
class GRPOBJECTBUILDER;
class GRPFACTORY;
class GRPCAMERA;
class GRPSHADERPROGRAM;
class GRPBUFFEROPENGLRENDER;
class GRPNODE;
class GRPANIMATIONCONTROL;
class GRPVISUALDEBUGGER;
class GRPTEXTBOX;
class GRPSHADERLINK;
//class GRPSPRITE;
class GRPRENDERTARGET;
class GRPPARTICLESYSTEM;
class GRPPASS;

class GRPSCENE : public XSUBJECT, public GRPSUBJECT, public GRPOBSERVER
{
	public:
																			GRPSCENE												();
																			GRPSCENE												(GRPSCREEN* screen);
		virtual												 	 ~GRPSCENE												();

		GRPNAMED_TYPE											GetType													() { return GRPNAMED_TYPE_GRPSCENE;}

		void															SetDebug												();

		GRPNODE*													FindNode												(XCHAR* name);
		GRPELEMENT*												FindElementByNodeName						(XCHAR* name);
		GRPOBJECT*												FindObject											(XCHAR* name);
		GRPELEMENT*												FindElement											(XCHAR* name);
		GRPCAMERA*												FindCamera											(XCHAR* name); 
		GRPVIEWPORT*											FindViewport										(XCHAR* name);
		GRPLIGHT*													FindLight												(XCHAR* name);	
		

		GRPNODE*													CreateNode											();
		GRPNODE*													CreateNode											(GRPVECTOR*);
		GRPNODE*													CreateNode											(GLFLOAT	x,		GLFLOAT y,		GLFLOAT z);
		GRPCAMERA*												CreateCamera										();
		GRPLIGHT*													CreateLight											();
		GRPRENDERTARGET*									CreateRenderTarget							(XDWORD		width,XDWORD	height);

		GRPVIEWPORT*											CreateViewport									();
		GRPPASS*													CreatePass											(XCHAR*		name,	int			priority=100,		bool		 addToPasses=true);


		bool															InitializeLightning							();
		bool															PrepareLightning								();

		virtual	bool											Ini															(XDWORD flags=GRPSCENE_INIT_FLAGS_CREATE_ALL);
						void											Render													();

						void											RenderFrame											();
	
		virtual void											End															();		
	
		GRPCAMERA*												SetCurrentCamera								(XDWORD			i);
		GRPCAMERA*												SelectCamera										(GRPCAMERA* i);
		GRPVIEWPORT*											SetCurrentViewport							(XDWORD			i);

		void															ShowDebug												(GRPDEBUG_FLAG b);

		XDWORD														GetNodeHierarchy								(XVECTOR<GRPNODE*>* v,		GRPNODE*		master);			

		void															PrepareScene										();
		void															PrepareShaders									();
		void															ReorderScene										();
		
		bool															Notify													(GRPMSGTYPE					msg,	GRPSUBJECT* notifier);
	
		bool															Add															(GRPOBJECT*					obj, bool checkshaders=true);		
		bool															Add															(GRPNODE*						n);																		
		bool															Add															(GRPMATERIAL*				mat);
		bool															Add															(GRPELEMENT*				element);
		bool															Add															(GRPLIGHT*					n);
		bool															Add															(GRPPASS*						pass, bool				copyRenderOrder=false);
		bool															Add															(GRPCAMERA*					cam);

		bool															Add															(GRPVIEWPORT*				vp)																{	return GetViewports()->Add(vp);										}
		bool															Add															(GRPPATH*						vp)																{	return GetPaths()->Add(vp);												}
		bool															Add															(GRPRENDERTARGET*		rt)																{	return GetRenderTargets()->Add(rt);								}
	
	//GRPPARTICLESYSTEM*								CreateParticleSystem						();
	//bool															Add															(GRPSPRITE*					n);																	
	//bool															Add															(GRPPARTICLESYSTEM* particlesystem)										{ return GetParticleSystems()->Add(particlesystem);	}				
		

		XVECTOR<GRPNODE*					>*			GetNodes												()																										{ return &nodes;																		}		
		XVECTOR<GRPOBJECT*				>*			GetObjects											()																										{ return &objects;																	}
		XVECTOR<GRPCAMERA*				>*			GetCameras											()																										{ return &cameras;																	}		
		XVECTOR<GRPLIGHT*					>*			GetLights												()																										{ return &lights;																		}
		XVECTOR<GRPVIEWPORT*			>*			GetViewports										()																										{ return &viewports;																}
		XVECTOR<GRPPATH*					>*			GetPaths												()																										{ return &paths;																		}

		XVECTOR<GRPOBJECT*				>*			GetDebugObjects									()																										{ return &debugObjects;															}
		XVECTOR<GRPRENDERTARGET*	>*			GetRenderTargets								()																										{ return &rendertargets;														}
//	XVECTOR<GRPPARTICLESYSTEM*>*			GetParticleSystems							()																										{ return &particlesystems;													}
	
		GRPRENDERER*											GetRenderer											()																										{ return renderer;																	}
		XDWORD														GetNFacesRendered								()																										{ return nFacesRendered;														}
		void															SetNFacesRendered								(XDWORD p)																						{ this->nFacesRendered=p;														}

		XDWORD														GetCurrentCameraIndex						()																										{ return this->CurrentCamera;												}
		XORDEREDLIST<GRPPASS*,
								GRPPASS::COMPARATOR>*	GetPasses												()																										{ return &this->Passes;															}
		GRPPASS*													GetPass													()																										{ return mainpass; }
		XDWORD														GetSubjectType									()																										{	return GRPNAMED_TYPE_GRPSCENE;										}

		GRPSCREEN*												GetScreen												()																										{ return screen;																		}
		GRPCAMERA*												GetCamera												()																										{ return this->GetCameras()->Get(CurrentCamera);		}
		int																GetCurrentCamera								()																										{ return CurrentCamera;															}
		GRPVIEWPORT*											GetViewport											()																										{ return this->GetViewports()->Get(CurrentViewport);}
		int																GetCurrentViewport							()																										{ return CurrentViewport;														}
		void															SetScreen												(GRPSCREEN*		p)																			{ this->screen=p;																		}					

		void															SelectPass											(GRPPASS* pass)																				{ this->pass=pass;																	}


		GRPRENDERER*											renderer;
		GRPVISUALDEBUGGER*								GetVisualDebugger	() { return visualdebugger;}
		GRPVISUALDEBUGGER*								visualdebugger;

		void															AddToRenderOrder									(GRPELEMENT* element);
 		void															RemoveFromRenderOrder							(GRPELEMENT* element);


		void															Lock															() { this->lock->Lock();}
		void															UnLock														() { this->lock->UnLock();}


	protected:	

		GLFLOAT*													LightMatrices;
		GLFLOAT*													LightColorMatrices;
		
		bool															EnableLightPass;
		bool															EnableShadowMapPass;

		//----------------- status

		XDWORD														CurrentCamera;
		XDWORD														CurrentViewport;		
		XDWORD														nFacesRendered;

		GRPPASS*													mainpass;

		GRPPASS*													pass;
		
		//------------------ facade
	
		GRPSCREEN*												screen;		

		
	//-------------------- collections
		
		XORDEREDLIST<GRPPASS*,GRPPASS::COMPARATOR> Passes;

		XVECTOR<GRPNODE*>									nodes;		
		XVECTOR<GRPOBJECT*>								debugObjects;		
		XVECTOR<GRPOBJECT*>								objects;		
		XVECTOR<GRPCAMERA*>								cameras;
		XVECTOR<GRPLIGHT*>								lights;
		XVECTOR<GRPVIEWPORT*>							viewports;
		XVECTOR<GRPPATH*>									paths;

		XVECTOR<GRPRENDERTARGET*>					rendertargets;
	//	XVECTOR<GRPPARTICLESYSTEM*>				particlesystems;

		XMUTEX*														lock;


private:


		void															RemoveFromNodes										(GRPNODE* node);


		void															Clean															()
																			{
																			mainpass							=	NULL;
																			pass									= NULL;																

																			screen								= NULL;
																			
																			LightMatrices					= NULL;
																			LightColorMatrices		= NULL;

																			CurrentCamera					= 0;
																			nFacesRendered				=	0;																					
																			CurrentViewport				=	0;
																					
																			lock									=NULL;
																			EnableShadowMapPass		= true;

																		}
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

