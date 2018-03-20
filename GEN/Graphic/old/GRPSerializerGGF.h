/*------------------------------------------------------------------------------------------
//	GRPSERIALIZERGGF.H
*/	
/**	
// \file 
//   
//  serializes/deserializes a ggf file
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 19/11/2015 10:21:38
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPSERIALIZERGGF_H_
#define _GRPSERIALIZERGGF_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
#include "GRPAnimation.h"

#include "GRPFileGGF.h"

#include "XBase.h"
#include "XString.h"
#include "XMap.h"
#include "XBuffer.h"
#include "XPath.h"

#include "XBinaryMap.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
class		GRPFILEGGF;
class		GRPSCENE;
class		GRPNAMED;
class		GRPNODE;
class		GRPOBJECT;
class		GRPCAMERA;
class		GRPELEMENT;
struct	GRPVERTEX;
class		GRPVIEWPORT;
class		GGFOBJECTDEFINITION;
class		GRPMATERIALATTRIBUTES;
class		GRPMATERIAL;
class		GRPTEXTURE;
class		GRPINTERFACEANIMABLE;
class		GRPATTACHEABLE;
class		GGFOBJECT;
class		GRPLIGHT;
class		GRPBEZIERSPLINE;
class		GRPUSERDATACONTAINER;

class		GGFSTRING;

class		XFACTORY;
class		XPUBLISHER;

class GRPSERIALIZERGGF
{
	public:

																										GRPSERIALIZERGGF										(GRPFILEGGF* file)
																										{
																											Clean();
																											
																											;
																											this->file = file;

																											//loadedelements.SetIsMulti(true);
																											SetupAnimationEnums();
																										}

		virtual																				 ~GRPSERIALIZERGGF										()
																										{
																											CleanAnimationEnums();
																											Clean();
																										}

		GGFOBJECTDEFINITION*														GetDefinition												(XCHAR* definition);
		GGFOBJECTDEFINITION*														GetDefinition												(XSTRING* definition);

		void																						WriteString													(GGFOBJECT* object, XCHAR* attribute, XCHAR* string);
		void																						WriteString													(GGFOBJECT* object, XCHAR* attribute, GGFSTRING* string);

		void																						ReadString													(GGFOBJECT* object, XCHAR* attribute, XSTRING* string);
		void																						ReadString													(GGFOBJECT* object, XCHAR* attribute, GGFSTRING* string);

		template <class T>
		XDWORD																					WriteVector													(XVECTOR<T>& vector);

		template <class T>
		void																						ReadVector													(XVECTOR<T>& vector, XDWORD index);

		bool																						GetFilePathTexture									(XCHAR* file, XPATH& path);

		void*																						GetLoadedObject											(XQWORD tag);

		void																						RegisterDefinition									(XCHAR* name, XDWORD size, GGFOBJECTDEFINITION* definition);

		
		bool																						Write																(GRPSCENE* scene);
		bool																						Read																(GRPSCENE* scene);

		bool																						WriteNodes													(GRPSCENE* scene);
		bool																						WriteCameras												(GRPSCENE* scene);
		bool																						WriteLights													(GRPSCENE* scene);
		bool																						WriteObjects												(GRPSCENE* scene);
		bool																						WriteViewPorts											(GRPSCENE* scene);
		
		bool																						WriteMaterials											(GRPSCENE* scene);
		bool																						WriteTextures												(GRPSCENE* scene);

		bool																						WriteAnimationStacks								(GRPSCENE* scene);
		bool																						WriteAnimationLayers								(GRPANIMATIONSTACK* stack);
		bool																						WriteAnimationNodes									(GRPANIMATIONLAYER* layer);
		bool																						WriteAnimationCurves								(GRPANIMATIONLAYER* layer);
		bool																						WriteBezierSplines									(GRPANIMATIONCURVE* curve);

		// SCENE ELEMENTS
		bool																						DefineGRPUSERDATACONTAINER					(GGFOBJECTDEFINITION* definition);
		bool																						WriteGRPUSERDATACONTAINER						(GRPUSERDATACONTAINER& container, GGFOBJECT* object);
		bool																						ReadGRPUSERDATACONTAINER						(GRPUSERDATACONTAINER& container, GGFOBJECT* object);

		bool																						DefineGRPOBJECT											(GGFOBJECTDEFINITION* definition);
		bool																						WriteGRPOBJECT											(GRPOBJECT& obj, GGFOBJECT* object);
		bool																						ReadGRPOBJECT												(GRPOBJECT& obj, GGFOBJECT* object);

		GRPNODE*																				ReadGRPNODEINHERITANCE							(GGFOBJECT* object);

		bool																						DefineGRPNODE												(GGFOBJECTDEFINITION* definition);
		bool																						WriteGRPNODE												(GRPNODE& node, GGFOBJECT* object);
		bool																						ReadGRPNODE													(GRPNODE& node, GGFOBJECT* object);

		bool																						DefineGRPMATERIAL										(GGFOBJECTDEFINITION* definition);
		bool																						WriteGRPMATERIAL										(GRPMATERIAL& material, GGFOBJECT* object);
		bool																						ReadGRPMATERIAL											(GRPMATERIAL& material, GGFOBJECT* object);

		bool																						DefineGRPTEXTURE										(GGFOBJECTDEFINITION* definition);
		bool																						WriteGRPTEXTURE											(GRPTEXTURE& texture, GGFOBJECT* object);
		bool																						ReadGRPTEXTURE											(GRPTEXTURE** texture, GGFOBJECT* object);

		bool																						DefineGRPMATERIALATTRIBUTES					(GGFOBJECTDEFINITION* definition);
		bool																						WriteGRPMATERIALATTRIBUTES					(GRPMATERIALATTRIBUTES & materialattributes, GGFOBJECT* object);
		bool																						ReadGRPMATERIALATTRIBUTES						(GRPMATERIALATTRIBUTES & materialattributes, GGFOBJECT* object);

		bool																						DefineGRPNAMED											(GGFOBJECTDEFINITION* definition);
		bool																						WriteGRPNAMED												(GRPNAMED & namedattributes, GGFOBJECT* object);
		bool																						ReadGRPNAMED												(GRPNAMED & namedattrbiutes, GGFOBJECT* object);

		bool																						DefineGRPINTERFACEANIMABLE					(GGFOBJECTDEFINITION* definition);
		bool																						WriteGRPINTERFACEANIMABLE						(GRPINTERFACEANIMABLE & materialattributes, GGFOBJECT* object);
		bool																						ReadGRPINTERFACEANIMABLE						(GRPINTERFACEANIMABLE& interfaceanimable, GGFOBJECT* object);

		bool																						DefineGRPATTACHEABLE								(GGFOBJECTDEFINITION* definition);
		bool																						WriteGRPATTACHEABLE									(GRPATTACHEABLE& attacheable, GGFOBJECT* object);
		bool																						ReadGRPATTACHEABLE									(GRPATTACHEABLE& attacheable, GGFOBJECT* object);

		bool																						DefineGRPELEMENT										(GGFOBJECTDEFINITION* definition);
		bool																						WriteGRPELEMENT											(GRPELEMENT& element, GGFOBJECT* object);
		bool																						ReadGRPELEMENT											(GRPELEMENT& element, GGFOBJECT* object);

		bool																						DefineGRPVERTEX											(GGFOBJECTDEFINITION* definition);
		bool																						WriteGRPVERTEX											(GRPELEMENT& element, GRPVERTEX* vertex, GGFOBJECT* object, XDWORD number);
		bool																						ReadGRPVERTEX												(GRPELEMENT& element, GRPVERTEX* vertex, GGFOBJECT* object, XDWORD number);

		bool																						DefineGRPLIGHT											(GGFOBJECTDEFINITION* definition);
		bool																						WriteGRPLIGHT												(GRPLIGHT& light, GGFOBJECT* object);
		bool																						ReadGRPLIGHT												(GRPLIGHT& light, GGFOBJECT* object);

		// GENERAL SCENE
		bool																						DefineGRPVIEWPORT										(GGFOBJECTDEFINITION* definition);
		bool																						WriteGRPVIEWPORT										(GRPVIEWPORT& viewport, GGFOBJECT* object);
		bool																						ReadGRPVIEWPORT											(GRPVIEWPORT& viewport, GGFOBJECT* object);

		bool																						DefineGRPCAMERA											(GGFOBJECTDEFINITION* definition);
		bool																						WriteGRPCAMERA											(GRPCAMERA& camera, GGFOBJECT* object);
		bool																						ReadGRPCAMERA												(GRPCAMERA& camera, GGFOBJECT* object);

		/// ANIMATIONS
		bool																						DefineGRPANIMATIONCONTROL						(GGFOBJECTDEFINITION* definition);
		bool																						WriteGRPANIMATIONCONTROL						(GRPANIMATIONCONTROL& control, GGFOBJECT* object);
		bool																						ReadGRPANIMATIONCONTROL							(GRPANIMATIONCONTROL& control, GGFOBJECT* object);

		bool																						DefineGRPANIMATIONSTACK							(GGFOBJECTDEFINITION* definition);
		bool																						WriteGRPANIMATIONSTACK							(GRPANIMATIONSTACK& stack, GGFOBJECT* object);
		bool																						ReadGRPANIMATIONSTACK								(GRPANIMATIONSTACK& stack, GGFOBJECT* object);

		bool																						DefineGRPANIMATIONLAYER							(GGFOBJECTDEFINITION* definition);
		bool																						WriteGRPANIMATIONLAYER							(GRPANIMATIONLAYER& layer, GGFOBJECT* object);
		bool																						ReadGRPANIMATIONLAYER								(GRPANIMATIONLAYER& layer, GGFOBJECT* object);

		bool																						DefineGRPANIMATIONNODE							(GGFOBJECTDEFINITION* definition);
		bool																						WriteGRPANIMATIONNODE								(GRPANIMATIONNODE& node, GGFOBJECT* object);
		bool																						ReadGRPANIMATIONNODE								(GRPANIMATIONNODE& node, GGFOBJECT* object);

		bool																						WriteCurveConnections								(GRPANIMATIONNODE& node, GGFOBJECT* object);
		bool																						ReadCurveConnections								(GRPANIMATIONNODE& node, GGFOBJECT* object);

		bool																						WriteDefaultNodeValues							(GRPANIMATIONNODE& node, GGFOBJECT* object);
		bool																						ReadDefaultNodeValues								(GRPANIMATIONNODE& node, GGFOBJECT* object);

		bool																						DefineGRPANIMATIONCURVE							(GGFOBJECTDEFINITION* definition);
		bool																						WriteGRPANIMATIONCURVE							(GRPANIMATIONCURVE& curve, GGFOBJECT* object);
		bool																						ReadGRPANIMATIONCURVE								(GRPANIMATIONCURVE& curve, GGFOBJECT* object);

		bool																						DefineGRPBEZIERSPLINE								(GGFOBJECTDEFINITION* definition);
		bool																						WriteGRPBEZIERSPLINE								(GRPBEZIERSPLINE& spline, GGFOBJECT* object);
		bool																						ReadGRPBEZIERSPLINE									(GRPBEZIERSPLINE& spline, GGFOBJECT* object);

		bool																						SetupAnimationEnums									();
		bool																						CleanAnimationEnums									();
		
		GGFSTRING*																			GetTangentKey(GRPANIMATIONCURVETANGENTMODE e);
		GRPANIMATIONCURVETANGENTMODE										GetTangentValue(GGFSTRING* str);
		GRPANIMATIONCURVETANGENTMODE										GetTangentValue(XCHAR* str);

		GGFSTRING*																			GetTypeKey(GRPANIMATIONCURVETYPE e);
		GRPANIMATIONCURVETYPE														GetTypeValue(GGFSTRING* str);
		GRPANIMATIONCURVETYPE														GetTypeValue(XCHAR* str);

		GGFSTRING*																			GetAxisKey(GRPANIMATIONCURVEAXIS e);
		GRPANIMATIONCURVEAXIS														GetAxisValue(GGFSTRING* str);
		GRPANIMATIONCURVEAXIS														GetAxisValue(XCHAR* str);

		GGFSTRING*																			GetLoopKey(GRPANIMATIONLOOPMODE e);
		GRPANIMATIONLOOPMODE														GetLoopValue(GGFSTRING* str);
		GRPANIMATIONLOOPMODE														GetLoopValue(XCHAR* str);

		GGFSTRING*																			GetStatusKey(GRPANIMATION_STATUS e);
		GRPANIMATION_STATUS															GetStatusValue(GGFSTRING* str);
		GRPANIMATION_STATUS															GetStatusValue(XCHAR* str);

	protected:

		GRPFILEGGF*																			file;
		GRPSCENE*																				scene;

		XBINARYMAP<XQWORD, void*>												loadedelements;

		XMAP<GGFSTRING*, int>														animationtangentmode;
		XMAP<GGFSTRING*, int>														animationcurvetype;
		XMAP<GGFSTRING*, int>														animationcurveaxis;
		XMAP<GGFSTRING*, int>														animationloopmode;
		XMAP<GGFSTRING*, int>														animationstatus;

	private:
		void																						Clean																()
																										{																											
																											file				= NULL;
																										}
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
template <class T>
XDWORD GRPSERIALIZERGGF::WriteVector(XVECTOR<T>& vector)
{
	XBUFFER buffer( false);

	buffer.Resize(vector.GetSize()*sizeof(T));
	for(XDWORD i = 0; i < vector.GetSize(); i++)
		{
			T value = vector.FastGet(i);
			buffer.Set((XBYTE*)&value, sizeof(T));
		}

	GGFDATA* data = new GGFDATA();
	data->buffer.Resize(buffer.GetSize());
	data->buffer.Set(buffer.Get(), buffer.GetSize());
	return file->AddData(data);
}


template <class T>
void GRPSERIALIZERGGF::ReadVector(XVECTOR<T>& vector, XDWORD index)
{
	GGFDATA* data = file->data.Get(index);
	XDWORD nelements = data->buffer.GetSize()/sizeof(T);

	data->buffer.SetPosition(0);

	for(XDWORD i = 0; i < nelements; i++)
		{
			T value;
			data->buffer.Get((XBYTE*)&value, sizeof(T));
			vector.Add(value);
		}
}

#endif

