/*------------------------------------------------------------------------------------------
//	GRPPASS.H
*/	
/**	
// \class 
//   
//  Holds render passes
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 12/07/2016 12:12:42
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPPASS_H_
#define _GRPPASS_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XVector.h"
#include "XList.h"
#include "XOrderedList.h"

#include "GRPNamed.h"
#include "GRPColor.h"
#include "GRPSubject.h"
#include "GRPMatrix.h"
#include "GRPDrawable.h"
#include "GRPCamera.h"
#include "GRPRect.h"

#include "GRPPassManager.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	

class GRPELEMENT;
class GRPSHADERPROGRAM;
class GRPSCENE;
class GRPCAMERA;
class GRPVIEWPORT;
class GRPRENDERTARGET;

enum GRPPASS_TYPE_FLAGS
{
	GRPPASS_TYPE_NONE									=0<<0,
	GRPPASS_TYPE_TEXTUREMODE					=1<<0,
	GRPPASS_TYPE_DEPTHTEST						=1<<1,
	GRPPASS_TYPE_DEPTHWRITE						=1<<2,
	GRPPASS_TYPE_DEPTHFUNC						=1<<3,
	GRPPASS_TYPE_CLEARDEPTH						=1<<4,
	GRPPASS_TYPE_COLORWRITE						=1<<5,
	GRPPASS_TYPE_CULLFACE							=1<<6,
	GRPPASS_TYPE_ALPHABLEND						=1<<7,
	GRPPASS_TYPE_SCISSOR							=1<<8,
	GRPPASS_TYPE_STENCIL							=1<<9,
	GRPPASS_TYPE_STENCIL_OPERATION		=1<<10,
	GRPPASS_TYPE_STENCIL_MASK					=1<<11,
	GRPPASS_TYPE_STENCIL_FUNCTION			=1<<12,
	GRPPASS_TYPE_ALL									=GRPPASS_TYPE_TEXTUREMODE		| 	
																		 GRPPASS_TYPE_DEPTHTEST			|	
																		 GRPPASS_TYPE_DEPTHWRITE		|	
																		 GRPPASS_TYPE_DEPTHFUNC			|
																		 GRPPASS_TYPE_CLEARDEPTH		|	
																		 GRPPASS_TYPE_COLORWRITE		|	
																		 GRPPASS_TYPE_CULLFACE			|
																		 GRPPASS_TYPE_ALPHABLEND		|	
																		 GRPPASS_TYPE_SCISSOR	
};


enum GRPPASS_TYPE{
	GRPPASS_CUSTOM,
	GRPPASS_BLENDING,
	GRPPASS_REFLECTION,
	GRPPASS_STENCIL,
	GRPPASS_SHADOW_SPOT,
	GRPPASS_SHADOW_POINT,
	GRPPASS_BACKGROUND,	
	GRPPASS_SKYBOX,
	GRPPASS_ENVMAP,
	GRPPASS_VIEWPORT,
	GRPPASS_OVERLAY,
	GRPPASS_DEPTH
};

enum GRPPASS_CLONE_TYPE
{
		GRPPASS_CLONE_TYPE_NOCLONE,
		GRPPASS_CLONE_TYPE_REFERENCE,
		GRPPASS_CLONE_TYPE_COPY,
		GRPPASS_CLONE_TYPE_FILTER
};

class XMUTEX;
 
class INPDEVICE;

/*---- CLASS -----------------------------------------------------------------------------*/
class GRPPASS : public GRPSUBJECT, public GRPOBSERVER
{
public:
	friend class GRPPASSMANAGER;
	friend class GRPSCENE;
	friend class GRPELEMENT;

	virtual													~GRPPASS								();

	template<class T>
						void									SetRenderComparator			()															
						{
							DestroyRenderOrder();
							this->RenderOrder = new XORDEREDLIST<GRPELEMENT*, T>(); 
							this->OrderClone=false;
						}

	class	COMPARATOR : public XCOMPARATOR<GRPPASS> 		{ public:		int Compare(GRPPASS* test1,GRPPASS* test2); 	};

						void									ClearAllObjects								();

						GRPRENDERLIST					GetRenderOrder								();
						void									SetRenderOrder								(GRPRENDERLIST queue);
						void									DestroyRenderOrder  					();
						void									ApplyFlags										();
						void									SetScissor										(bool onoff=true, int xo =0, int yo=0, int xf=0, int yf=0);
								
						void									Clear													();

	virtual		void									Start													() {};
	virtual		void									OnDraw												(GRPELEMENT*) {};
	virtual		void									Begin													();	
	virtual		void									End														();
	virtual   void									Finish												() {};

	virtual		bool									Add														(INPDEVICE*) { return false; }
	virtual		bool									Add														(GRPELEMENT*);
	virtual		bool									Remove												(GRPELEMENT*);
						
	virtual		bool									OnAdded												() { this->SetCamera(camera_name.Get()); return true; }

						bool									Notify												(GRPMSGTYPE type, GRPSUBJECT* subject);
						GRPSHADERPROGRAM*			GetProgram										();
	virtual		void									SetProgram										(GRPSHADERPROGRAM*	program);
						
	virtual		bool									Draw													(GRPPASS*);
	virtual		bool									Draw													();
	virtual		bool									DrawRenderOrderWithOverride		();
	virtual		bool									DrawRenderOrder								();

							//---------------- matrices
		
						bool									CalculateMatrices							();

							//---------------- creators

	virtual		void									CreateCamera									(GRPCAMERA_TYPE type=GRPCAMERA_TYPE_FREE);
	virtual		void									CreateViewport								(GRPPROJECTIONTYPE isOrtho=GRPPROJECTIONTYPE_ORTHO);
	virtual		void									SetRect												(GRPRECTFLOAT* rect);
	virtual		void									SetRect												(float x, float y, float xf, float yf);
	virtual		GRPRECTFLOAT*					GetRect												();
	virtual 	void									SetCamera											(XCHAR*	camera_name);
	virtual		void									SetViewport										(XCHAR*	viewport_name);
	virtual		void									SetLight											(XCHAR*	light_name);

							//---------------- getter setters

						void									SetClone											(bool b);
						bool									GetClone											()																																										{ return this->mustClone;								}
						void									SetClearColor									(GRPCOLOR c)																																					{ this->clearcolor=c;										}

						XDWORD								GetSubjectType								()																																										{ return GRPNAMED_TYPE_GRPPASS;					}
						GRPVIEWPORT*					GetViewport										();
						GRPSCENE*							GetScene											()																																										{ return holderscene;										}
						GRPCAMERA*						GetCamera											();																																										
						GRPRENDERTARGET*			GetRenderTarget								()																																										{ return target;												}	
						GRPSHADERPROGRAM*			GetShader											()																																										{ return shader;												}	
								
virtual			void									CreateRenderTarget						();
						
virtual			void									SetViewport										(GRPVIEWPORT*				viewport		)																											{ this->viewport=viewport;							}	
						void									SetScene											(GRPSCENE*					holderscene	)																											{ this->holderscene=holderscene;				}
						void									SetCamera											(GRPCAMERA*					camera			)																											{ this->camera=camera;									}						
						
						void									SetRenderTarget								(GRPRENDERTARGET*		target			)																											{ this->target=target;									}	
						void									SetShader											(GRPSHADERPROGRAM*	shader			)																											{ this->shader=shader;									}	
							
						XDWORD								GetType												()																																										{ return type;													}	
						void									SetType												(XDWORD							type				)																											{ this->type=type;											}	
						XDWORD								AddType												(GRPPASS_TYPE_FLAGS	type				)																											{ return (this->type|= type);						}	
						XDWORD								RemoveType										(GRPPASS_TYPE_FLAGS	type				)																											{ return (this->type&= ~type);					}	
							
						GRPMATRIX*						GetViewMatrix									()																																										{ return viewmatrix;										}					
						GRPMATRIX*						GetProjectionMatrix						()																																										{ return projectionmatrix;							}
						GRPMATRIX*						GetViewProjection							()																																										{ return &viewprojection;								}

						void									SetClear											(XDWORD							flags				)																											{ this->hasToClear					=	flags;		}
						void									SetActive											(bool								active			)																											{ this->active							=	active;		}
						bool									GetActive											()																																										{ return this->active;									}


						void									SetDepthTestIsActive					(bool								active			)																											{ this->DepthTestIsActive		=	active;		}
						void									SetDepthWritingIsActive				(bool								active			)																											{ this->DepthWritingIsActive=	active;		}
						void									SetDepthFunction							(GRPDEPTHFUNC				func				)																											{ this->DepthFunction				=	func;			}
						void									SetGlobalShaderOverride				(bool								onoff				)																											{ this->globalshaderoverride= onoff;		}
						void									SetPriority										(int								priority		)																											{ this->priority						=	priority;	}
						int										GetPriority										(		)																																									{ return this->priority;								}

						void									SetColorWritingIsActive				(bool									onoff			)																											{ this->ColorWritingIsActive= onoff;		}

						void									SetStencilWriting							(XDWORD tag=0x01);
						void									SetStencilTesting							(XDWORD tag = 0x01);
						void									DisableStencil								();
						void									SetStencilTestIsActive				(bool									onoff			)																											{ this->StencilTestIsActive	=	onoff;		}
						void									SetStencilFailOperation				(GRPSTENCILOPERATION	operation	)																											{ StencilFailOperation			=	operation;}
						void									SetStencilDepthFailOperation	(GRPSTENCILOPERATION	operation	)																											{ StencilDepthFailOperation	=	operation;}
						void									SetStencilPassOperation				(GRPSTENCILOPERATION	operation	)																											{ StencilPassOperation			=	operation;}
						void									SetStencilFunction						(GRPSTENCILFUNCTION		function, XDWORD value)																					{ StencilFunction						= function; StencilValue = value;		}
						void									SetStencilMask								(XDWORD								maskvalue	)																											{ this->StencilMask					= maskvalue;}
						void									SetStencilValue								(XDWORD								value			)																											{ this->StencilValue				= value;		}
						
						void									SetBlending										(bool onoff, GRPBLENDINGMODE mode=GRPBLENDINGMODE_NONE)																{ this->AlphaBlendingIsActive = onoff; this->AlphaBlendingMode=mode;} 
						void									SetFlipMatrix									(bool									onoff			)																											{ this->flipmatrixIsActive=onoff;				}
						GRPMATRIX*						GetFlipMatrix									()																																										{ return &this->flipmatrix;							}


virtual			void									SetNode												(GRPNODE*)									{}
virtual			void									SetMaterial										(GRPMATERIAL*)							{}
virtual			void									SetMaterialName								(XCHAR*n)										{ this->material_name.Set(n); }
virtual			GRPMATERIAL*					GetMaterial										()													{ return NULL; }
virtual			GRPTEXTURE*						GetTexture										()													{ return NULL; }
virtual			void									SetLight											(GRPLIGHT*)									{}

virtual			bool									SetValue											(XDWORD type, double value) { return false; }
virtual			bool									SetValue											(XDWORD type, XDWORD value) { return false; }


						XVECTOR<XSTRING*>*		GetAcceptElements							() { return &this->acceptElements; }
						XVECTOR<XSTRING*>*		GetRejectElements							() { return &this->rejectElements; }

						//-------------------- pass tree
						XORDEREDLIST<GRPPASS*,
								GRPPASS::COMPARATOR>*	GetPasses									() { return &this->passes; }
						
							
protected:

						//-------------------- matrices

						GRPMATRIX							viewprojection;
						GRPMATRIX							worldmatrix;		
						GRPMATRIX*					  viewmatrix;	
						GRPMATRIX*						projectionmatrix;

						GRPMATRIX							flipmatrix;
						bool									flipmatrixIsActive;

						int										iterations;
						int										currentiteration;
						int										priority;

						bool									globalshaderoverride;
						bool									lightingIsEnabled;



																	GRPPASS												(XCHAR* name);

						bool									active;

						XDWORD								type;

						GRPSCENE*							holderscene;


						GRPCAMERA*						camera;
						XSTRING								camera_name;
						bool									ownscamera;

						GRPVIEWPORT*					viewport;
						XSTRING								viewport_name;
						bool									ownsviewport;

						XSTRING								light_name;

						GRPRENDERTARGET*			target;
						GRPSHADERPROGRAM*			shader;
						GRPCOLOR							clearcolor;						

						XDWORD								hasToClear;
						bool									ScissorTestIsActive;
						
						bool									StencilTestIsActive;
						GRPSTENCILOPERATION		StencilFailOperation;
						GRPSTENCILOPERATION		StencilDepthFailOperation;
						GRPSTENCILOPERATION		StencilPassOperation;
						GRPSTENCILFUNCTION		StencilFunction;
						XDWORD								StencilValue;
						XDWORD								StencilMask;

						bool									TextureIsActive;

						bool									DepthTestIsActive;
						bool									DepthWritingIsActive;
						GRPDEPTHFUNC					DepthFunction;
						float									ClearDepth;

						bool									ColorWritingIsActive;

						bool									BackFaceCullingIsActive;
						GRPCULLMODE						BackFaceCullingMode;

						bool									AlphaBlendingIsActive;
						GRPBLENDINGMODE				AlphaBlendingMode;

						GRPRECTINT						Scissor;

						GRPRENDERLIST					RenderOrder;
						bool									OrderClone;
						bool								  mustClone;

						GRPPASS_TYPE					passtype;

						XMUTEX*								mutex;
						GRPRECTFLOAT					rect;

						bool									isloaded;
						XSTRING								material_name;

						XVECTOR<XSTRING*>		  acceptElements;
						XVECTOR<XSTRING*>		  rejectElements;

						XORDEREDLIST<GRPPASS*,
								GRPPASS::COMPARATOR> passes;

private:

						void Clean()
						{
							isloaded										=	false;
							passtype										= GRPPASS_CUSTOM;
							iterations									= 1;
							currentiteration						= 0;
							
							flipmatrixIsActive					= false;
							flipmatrix.LoadIdentity();

							active											=	true;
							holderscene									=	NULL;
							
							camera											=	NULL;
							ownscamera									= false;

							viewport										=	NULL;
							ownsviewport								= false;

							target											=	NULL;
							shader											=	NULL;
							clearcolor									=	GRPCOLOR::BLACK;

							ScissorTestIsActive					=	false;
							StencilTestIsActive					=	false;							
							StencilFailOperation				=	GRPSTENCILOPERATION_KEEP;
							StencilDepthFailOperation		=	GRPSTENCILOPERATION_KEEP;
							StencilPassOperation				=	GRPSTENCILOPERATION_KEEP;
							StencilFunction							= GRPSTENCILFUNCTION_ALWAYS;
							StencilMask									=	0xFF;
							StencilValue								= 0x01;
							hasToClear									=	GRPPASS_CLEAR_FLAG_NONE;
							globalshaderoverride				=	false;

							TextureIsActive							=	true;
							DepthTestIsActive						=	true;
							DepthWritingIsActive				=	true;
							DepthFunction								=	GRPDEPTHFUNC_LESS;
							ClearDepth									=	1.0f;
							ColorWritingIsActive				=	true;
							BackFaceCullingIsActive			=	true;
							BackFaceCullingMode					=	GRPCULLMODE_BACK;
							AlphaBlendingIsActive				=	false;
							type												=	GRPPASS_TYPE_COLORWRITE | GRPPASS_TYPE_DEPTHTEST | GRPPASS_TYPE_DEPTHWRITE;
									
							viewmatrix									= NULL;	
							projectionmatrix						= NULL;		
								
							Scissor											= GRPRECTINT(0,0,0,0);
							rect												= GRPRECTFLOAT(0.0, 0.0, 1.0, 1.0);

							RenderOrder									=	NULL;
							OrderClone									= false;
							mustClone										= false;

							lightingIsEnabled						= true;

							priority										= 100;

							worldmatrix.LoadIdentity		();
							viewprojection.LoadIdentity	();
						}

};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

