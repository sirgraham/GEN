//------------------------------------------------------------------------------------------
//	GGFCONVERTER.H
//	
/**
// \class
//
//  GGF Converter
//
//	@author	 Diego Martinez Ruiz de Gaona
//	@version 13/06/2017
*/
//------------------------------------------------------------------------------------------

#ifndef _GGFCONVERTER_H_
#define _GGFCONVERTER_H_

//---- INCLUDES ----------------------------------------------------------------------------

#include "GRPApplication.h"
#include "GRPFile3D.h"
#include "XMap.h"
#include "XPath.h"
#include "XFileXML.h"
#include "XLoader.h"
#include "XConsole.h"
#include "GRPLoader.h"


//---- DEFINES & ENUMS  --------------------------------------------------------------------

#define GGFCONVERTER_VERSION												0
#define GGFCONVERTER_SUBVERSION											0
#define GGFCONVERTER_SUBVERSIONERR									0

#define GGFCONVERTER_NAMEAPP			 									__L("GGFConverter")
#define GGFCONVERTER_ENTERPRISE	 										__L("")

#define GGFCONVERTER_YEAROFCREATION									2017

#define GGFCONVERTER_CFGNAMEFILE	 									__L("GGFConverter")

#define GGFCONVERTER_DIRECTORYMAIN									__L("resources")
#define GGFCONVERTER_DIRECTORYWEB										__L("web")
#define GGFCONVERTER_DIRECTORYGRAPHICS							__L("graphics")

#define GGFCONVERTER_LOGSECTIONIDGENERIC					 __L("Generic")


class GRPCONSOLE;
class GRPFILE3D;
class GRPOBJECT;
class XCONSOLE;


class GGFCONVERTER_TASK : public GRPLOADERTASK
{
public:

	GGFCONVERTER_TASK(XCONSOLE* x) : xconsole(x), GRPLOADERTASK(__L("console_loader")),showheader(true) {};

	void Print	(XDWORD priority,	XCHAR* text);
	void Output	(XDWORD priority, XCHAR* mask,...);

	XCONSOLE* xconsole;

	void  ShowHeader(bool b) { this->showheader=b;}
	bool showheader;
};

//---- CLASS -------------------------------------------------------------------------------
class GGFCONVERTER : public XAPPLICATION
{
public:
																				GGFCONVERTER		() { Clean();}
																				~GGFCONVERTER		();

		bool																Ini							();
		bool																Update					();
		bool																End							();

		bool																LoadInput				();
		bool																SaveOutput			();
		bool																CopySoundFiles	();
		bool																CopyGraphicFiles();
		bool																CopyFontFiles		();
		bool																CopyDependencies();

		bool																FirstUpdate			();
		//bool																InitConsole			();				
		void																HandleEvent			(XEVENT* event);
		GRPOBJECT*													LoadObject			(	GRPOBJECT*	obj,
																													XCHAR*			path,
																													XDWORD			flags				=GRPFILE3D_INCLUDE_ALL, 
																													XDWORD			verboseLevel=GRPFILE3D_VERBOSE_LEVEL_WARNINGS | GRPFILE3D_VERBOSE_LEVEL_ERRORS,
																													XDWORD			NextStackID=0);
		GRPSCENE*														GetMainScene		() { return this->scene;}
		bool																PathManagement	(XCHAR* addpath);

protected:

		GRPCONSOLE*													console;		
		GRPFILE3D*													file3d;
		

		XMAP<XSTRING*,XSTRING*>							options;
		XVECTOR<XSTRING*>										dependencies;

		XSTRING															origin;
		XSTRING															output;
		XSTRING															inputdir;

		GRPOBJECT*													object;
		XFILEXML2														xml;
		XPATH																rootpath;
		GRPSCENE*														scene;
		XCONSOLE*														xconsole;
		bool																showheader;
		GGFCONVERTER_TASK*									task;
		XDIR*																copier;
		
private:

		virtual void												Clean						()
		{
			task			=NULL;
			file3d		=NULL;
			console		=NULL;	
			object		=NULL;
			showheader=true;			
			copier		=NULL;
			xconsole	=NULL;
		}

};

//---- INLINE FUNCTIONS --------------------------------------------------------------------



#endif
