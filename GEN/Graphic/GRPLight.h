/*------------------------------------------------------------------------------------------
//	GRPLIGHT.H
*/	
/**	
// \class 
//   
//  Class object for 3D Vector Light
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 26/06/2014 17:51:22
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPLIGHT_H_
#define _GRPLIGHT_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "GRPOpenGL.h"
#include "GRPColor.h"
#include "GRPVector.h"
#include "GRPNode.h"
#include "GRPShader.h"
#include "GRPCamera.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
enum GRPLIGHTTYPE
{
			GRPLIGHTTYPE_UNKNOWN			=-1,
			GRPLIGHTTYPE_POINT				=0, 	//Point light sources give off light equally in all directions, so require only position not direction.
			GRPLIGHTTYPE_DIRECTIONAL	=1,	  //Directional lights simulate parallel light beams from a distant source, hence have direction but no position.
			GRPLIGHTTYPE_SPOTLIGHT		=2,		//Spotlights simulate a cone of light from a source so require position and direction, plus extra values for falloff.
			GRPLIGHTTYPE_DAYLIGHT			=3		//Unidirectional light from sun and sky , parallel casting
};

enum GRPLIGHTDECAYTYPE
{
			GRPLIGHTDECAYTYPE_NONE			= 0,
			GRPLIGHTDECAYTYPE_CONSTANT	= 1<<1,
			GRPLIGHTDECAYTYPE_LINEAR		= 1<<2,
			GRPLIGHTDECAYTYPE_QUADRATIC = 1<<3,
			GRPLIGHTDECAYTYPE_CUBIC			= 1<<4
	
};


//http://www.learnopengl.com/#!Advanced-Lighting/Shadows/Shadow-Mapping
//Few tiny optimizations I can think of:
//- Use a smaller resolution shadow map
//- A different PCF algorithm that is more efficient and still generates good results
//- Keep the depth map generation shaders as simple as possible
//- Use much simpler (less detailled) models for generating shadow map; this is often hardly noticeable.
//- Fit the orthographic/perspective frustum exactly around the area you want to shadow. This greatly increases the depth precision and thus allows you to lower the resolution of your shadow map.
//- Use different shadow algorithms based on the type of scene and your needs: try and look into Variance shadow mapping to see if that's something you like.
//- Determine which of the geometry is static and keep two shadow maps: one for static objects and the other for dynamic objects. Then create the static shadow map once, and update the dynamic
//shadow map each (or once every multiple frames). This ONLY works when the light source doesn't move as else EVERYTHING is dynamic from the shadow's point of view.

class GRPLIGHT : public GRPCAMERA
{
	public:

 																GRPLIGHT										()																																				{ Clean();GRPNAMED::classtype = GRPNAMED_TYPE_GRPLIGHT;}
																~GRPLIGHT										();

					void									SetType											(GRPLIGHTTYPE f)																													{ Type=f; 													}		
					
					void									SetStrength									(GLFLOAT f)																																{ Strength=f;												}				
					void									SetMultiplier								(GLFLOAT multiplier)																											{ Multiplier=multiplier;						}

					void									SetAmbient									(GRPCOLOR c)																															{ Ambient=c;  											}
					void									SetDiffuse									(GRPCOLOR c)																															{ Diffuse=c;  											}
					void									SetSpecular									(GRPCOLOR c)																															{ Specular=c;  											}
					
					void									SetInnerSpot								(GLFLOAT Inner)																														{ SpotInner=Inner;									}
					void									SetOuterSpot								(GLFLOAT Outer)																														{ SpotOuter=Outer;									}	

					void									SetScaleFactor							(GLFLOAT scale)																														{ LightScaleFactor=scale;						}

					void									SetDecayType								(GRPLIGHTDECAYTYPE t)																											{ DecayType=t;											}					
					void									SetConstantAttenuation			(float c)																																	{ ConstantAttenuation=c;						}
					void									SetLinearAttenuation				(float l)																																	{ LinearAttenuation=l;							}
					void									SetQuadraticAttenuation			(float q)																																	{ QuadraticAttenuation=q;						}
					void									SetCubicAttenuation					(float cb)																																{ CubicAttenuation=cb;							}

					GLFLOAT								GetStrength									()																																				{ return Strength;									}
					GLFLOAT								GetMultiplier								()																																				{ return Multiplier;								}	

					GRPCOLOR							GetAmbient									()																																				{ return Ambient;  									}
					GRPCOLOR							GetDiffuse									()																																				{ return Diffuse;  									}
					GRPCOLOR							GetSpecular									()																																				{ return Specular; 									}



					GRPMATRIX*						GetLightDataMatrix					()																																				{ return &LightDataMatrix;					}		
					GRPMATRIX*						GetLightColorMatrix					()																																				{ return &LightColorMatrix;					}		
					
					GLFLOAT								GetInnerSpot								()																																				{ return SpotInner;									}
					GLFLOAT								GetOuterSpot								()																																				{ return SpotOuter;									}	
					GLFLOAT								GetScaleFactor							()																																				{ return LightScaleFactor;					}

					GRPLIGHTDECAYTYPE			GetDecayType								()																																				{ return DecayType;									}		
					float									GetConstantAttenuation			()																																				{ return ConstantAttenuation;				}
					float									GetLinearAttenuation				()																																				{ return LinearAttenuation;					}
					float									GetQuadraticAttenuation			()																																				{ return QuadraticAttenuation;			}
					float									GetCubicAttenuation					()																																				{ return CubicAttenuation;					}

					void									SetDirection								(GRPVECTOR& direction)																										{ Direction = direction;						}
					GRPVECTOR*						GetDirection								()																																				{ return &Direction;								}
					XDWORD								GetSubjectType							()																																				{	return GRPNAMED_TYPE_GRPLIGHT;		}
					GRPLIGHTTYPE					GetType											()																																				{ return Type;											}

					float									GetShadowQuality						()																																				{ return shadowquality;							}
					void									SetShadowQuality						(GLFLOAT q)																																{ this->shadowquality=q;						}
					float									GetBias											()																																				{ return bias;											}
					void									SetBias											(GLFLOAT q)																																{ this->bias=q;											}

					void									SetAttenuation							(GLFLOAT constant,GLFLOAT linear,GLFLOAT quadric,GLFLOAT cubic);				
					void									Update											(GRPMATRIX* viewmatrix);
					void									BuildData										();
					bool									CopyFrom										(GRPLIGHT* light);
					bool									SetValue										(GRPANIMATIONCURVETYPE type,GRPANIMATIONCURVEAXIS subtype, double value,double base);
					bool									BuildShadowMap							();
					bool									SetViewport									(GRPVIEWPORT* viewport);					
					
					GRPLIGHTDECAYTYPE			DecayType;

					GLUINT								Program;

					GRPLIGHTTYPE					Type;
					GRPVECTOR							Direction;
				
					GLFLOAT								Strength;
					GLFLOAT								Multiplier;

					GLFLOAT								ConstantAttenuation;
					GLFLOAT								LinearAttenuation;
					GLFLOAT								QuadraticAttenuation;
					GLFLOAT								CubicAttenuation;
			
					GRPCOLOR							Diffuse;
					GRPCOLOR							Ambient;
					GRPCOLOR							Specular;

					GLFLOAT								SpotInner;
					GLFLOAT								SpotOuter;

					GRPMATRIX							LightDataMatrix;
					GRPMATRIX							LightColorMatrix;
					GRPMATRIX							lightSpaceMatrix;

					GLFLOAT								LightScaleFactor;

					GLFLOAT								shadowquality;

					float									bias;
private:

				void Clean()
				{
						Type										= GRPLIGHTTYPE_POINT;		
					Direction								= GRPVECTOR(-1.0f,-1.0f,-1.0f);		

					DecayType								= GRPLIGHTDECAYTYPE_NONE;

					SpotInner								= 0.0f;
					SpotOuter								= 0.0f;

					ConstantAttenuation			= 1.00f;
					LinearAttenuation				= 0.00f;
					QuadraticAttenuation		= 0.00f;
					CubicAttenuation				= 0.00f;

					Strength								= 100.0f;

					Ambient									= GRPCOLOR(0.1f,		0.1f,		0.1f,		1.0f);
					Diffuse									= GRPCOLOR(1.0f,		1.0f,		1.0f,		1.0f);
					Specular								= GRPCOLOR(1.0f,		1.0f,		1.0f,		1.0f);	

					target									= NULL;

					SpotInner								= 45.0f;
					SpotOuter								= 48.0f;

					Multiplier							= 1.0f;
					LightScaleFactor				= 1.0f;
					shadowquality						= 1.0f;

					bias										= 0.000317754078f;//0.000125100036f;

			this->SetName(__L("Default Light Name"));
		}
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

