//------------------------------------------------------------------------------------------
//	XPATH.CPP
//	
//	Path Functions
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 28/06/2002
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
		
//---- INCLUDES ----------------------------------------------------------------------------

#include "XFactory.h"
#include "XDir.h"
#include "XDebug.h"

#include "XPathsManager.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	
XPATHSMANAGER*	XPATHSMANAGER::instance = NULL;

//---- CLASS MEMBERS -----------------------------------------------------------------------


/*-------------------------------------------------------------------
//	XPATHSMANAGERSECTION::XPATHSMANAGERSECTION
*/	
/**	
//	
//	Class Constructor XPATHSMANAGERSECTION
//	
//	@author				Abraham J. Velez
//	@version			28/02/2016 19:16:47
//	
*/
/*-----------------------------------------------------------------*/
XPATHSMANAGERSECTION::XPATHSMANAGERSECTION()
{	
	Clean();															

	xpath = new XPATH();

}



/*-------------------------------------------------------------------
//	XPATHSMANAGERSECTION::~XPATHSMANAGERSECTION
*/	
/**	
//	
//	 Class Destructor XPATHSMANAGERSECTION
//	
//	@author				Abraham J. Velez
//	@version			28/02/2016 19:16:54
//	
*/
/*-----------------------------------------------------------------*/
XPATHSMANAGERSECTION::~XPATHSMANAGERSECTION()
{ 
	if(xpath)
		{
			delete xpath;
		}

	Clean();															
}





//-------------------------------------------------------------------
//  XPATHSMANAGER::XPATHSMANAGER
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			12/08/2003 10:14:53
//	
//	@return 			
//	*/
//-------------------------------------------------------------------
XPATHSMANAGER::XPATHSMANAGER()
{
	Clean();

	appexecpath = new XPATH;
}



//-------------------------------------------------------------------
//  XPATHSMANAGER::~XPATHSMANAGER
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			12/08/2003 10:14:53
//	
//	@return 			
//	*/
//-------------------------------------------------------------------
XPATHSMANAGER::~XPATHSMANAGER()
{
	DeleteAllPathSections();

	if(appexecpath)
		{
			delete appexecpath;
		}
	
	Clean();
}




/*-------------------------------------------------------------------
//  XPATHSMANAGER::GetPathOfSection
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			10/02/2014 10:42:09
//	
//	@return 			bool : 
//	@param				sectiontype : 
//  @param				xpath : 
//  @param				addroot : 
*/
/*-----------------------------------------------------------------*/
bool XPATHSMANAGER::GetPathOfSection(XPATHSMANAGERSECTIONTYPE sectiontype, XPATH& xpath, bool addroot)
{
	xpath.Empty();

	if(addroot && (sectiontype!=XPATHSMANAGERSECTIONTYPE_ROOT))
		{
			for(XDWORD c=0; c<pathsections.GetSize(); c++)
				{
					XPATHSMANAGERSECTION* pathsection = pathsections.Get(c);
					if(pathsection)
						{
							if(pathsection->type == XPATHSMANAGERSECTIONTYPE_ROOT)
								{
									xpath = pathsection->xpath->Get();
									xpath.Slash_Add();
									break;
								}
						}
				}
		}

	for(XDWORD c=0; c<pathsections.GetSize(); c++)
		{
			XPATHSMANAGERSECTION* pathsection = pathsections.Get(c);
			if(pathsection)
				{
					if(pathsection->type == sectiontype)
						{
							xpath += pathsection->xpath->Get();
							break;
						}
				}
		}

	return xpath.IsEmpty()?false:true;
}



/*-------------------------------------------------------------------
//  XPATHSMANAGER::GetPathSection
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			12/12/2013 13:57:30
//	
//	@return 			XPATHSMANAGERSECTION* : 
//	@param				sectiontype : 
*/
/*-----------------------------------------------------------------*/
XPATHSMANAGERSECTION* XPATHSMANAGER::GetPathSection(XPATHSMANAGERSECTIONTYPE sectiontype)
{
	if(pathsections.IsEmpty()) return NULL; 

	for(XDWORD c=0; c<pathsections.GetSize(); c++)
		{
			XPATHSMANAGERSECTION* pathsection = pathsections.Get(c);
			if(pathsection)
				{
					if(pathsection->type == sectiontype)
						{
							return pathsection; 
						}
				}
		}

	return NULL;
}



/*-------------------------------------------------------------------
//  XPATHSMANAGER::AddPathSection
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			12/12/2013 13:34:21
//	
//	@return 			bool : 
//	@param				sectiontype : 
//  @param				path : 
*/
/*-----------------------------------------------------------------*/
bool XPATHSMANAGER::AddPathSection(XPATHSMANAGERSECTIONTYPE sectiontype, XSTRING& xpath)
{
	XPATHSMANAGERSECTION* xpathsection = GetPathSection(sectiontype);
	if(!xpathsection) 
		{
			xpathsection = new XPATHSMANAGERSECTION();
			if(!xpathsection) return false;

			xpathsection->type	= sectiontype;
			(*xpathsection->xpath) = xpath.Get();

			if(!xpathsection->xpath->IsEmpty()) 
				{	
					xpathsection->xpath->Slash_Add();    
					xpathsection->xpath->Slash_Normalize();
				}

			pathsections.Add(xpathsection);
		}
	 else
		{
			(*xpathsection->xpath) = xpath;
			xpathsection->xpath->Slash_Add();    
			xpathsection->xpath->Slash_Normalize();
		}

	return true;
}



/*-------------------------------------------------------------------
//  XPATHSMANAGER::AddPathSection
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			12/12/2013 13:25:18
//	
//	@return 			bool : 
//	@param				sectiontype : 
//  @param				string : 
*/
/*-----------------------------------------------------------------*/
bool XPATHSMANAGER::AddPathSection(XPATHSMANAGERSECTIONTYPE sectiontype,const XCHAR* string)
{
	XSTRING path(string);
	
	return AddPathSection(sectiontype,path);
}




/*-------------------------------------------------------------------
//  XPATHSMANAGER::DeleteAllPathSections
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			12/12/2013 13:36:39
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool XPATHSMANAGER::DeleteAllPathSections()
{
	if(pathsections.IsEmpty()) return false;

	pathsections.DeleteContents();

	pathsections.DeleteAll();

	return true;
}



/*-------------------------------------------------------------------
//	XPATHSMANAGER::CreateAllPathSectionOnDisk
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			28/03/2014 9:03:38
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool XPATHSMANAGER::CreateAllPathSectionOnDisk()
{	
	XDIR* xdir = xfactory->Create_Dir();
	if(!xdir) return false;
	
	bool status = true;
	
	for(XDWORD c=0; c<pathsections.GetSize(); c++)
		{
			XPATHSMANAGERSECTION* pathsection = pathsections.Get(c);
			if(pathsection)
				{
					XPATH xpath;
									
					if(GetPathOfSection(pathsection->type, xpath, true)) 
						{
							if(!xdir->Exist(xpath.Get()))
								{
									if(!xdir->Make(xpath.Get())) 
										{
											status = false;
										}

									//XDEBUG_PRINTCOLOR((status?2:4), __L("Dir Create: %s "), xpath->Get());
								}
							 else
							  {
									//XDEBUG_PRINTCOLOR(2, __L("Dir Exist: %s "), xpath->Get());
								}
						}
				}
		}

	xfactory->Delete_Dir(xdir);	

	return status;
}



/*-------------------------------------------------------------------
//	XPATHSMANAGER::AdjustRootPathDefault
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			23/08/2016 11:15:32
//	
//	@return 			bool : 
//
//  @param				nameapp : 
//  @param				recopilatedir : 
*/
/*-----------------------------------------------------------------*/
bool XPATHSMANAGER::AdjustRootPathDefault(XCHAR* nameapp, XCHAR* recopilatedir)
{
	if(!xfactory) return false;
		
	XDIR* xdir = xfactory->Create_Dir();
	if(!xdir) return false;
	
	XPATH xpathroot;
	bool	status;
		
	GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpathroot);
	status = xdir->Exist(xpathroot);		

	if(status && recopilatedir)
		{
			xpathroot.Add(recopilatedir);
			xpathroot.Slash_Add();
			status = xdir->Exist(xpathroot);		
		}		
	
	if(status) 
		{
			AddPathSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpathroot.Get());
		}
	 else
		{
			XPATH developedpath;

			GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, developedpath);

			developedpath += __L("../../");

			#ifdef WINDOWS			
			developedpath += __L("../");
			#endif

			if(recopilatedir)
				{									
					developedpath += recopilatedir;
					developedpath.Slash_Add();					
				}

			status = xdir->Exist(developedpath);
						
			if(status) AddPathSection(XPATHSMANAGERSECTIONTYPE_ROOT, developedpath.Get());													
		}
	
	xfactory->Delete_Dir(xdir);	
	
	return status;
}





/*-------------------------------------------------------------------
//	XPATHSMANAGER::AdjustRootPathDefault
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			23/08/2016 11:15:20
//	
//	@return 			bool : 
//
//  @param				nameapp : 
//  @param				recopilatedir : 
*/
/*-----------------------------------------------------------------*/
bool XPATHSMANAGER::AdjustRootPathDefault(XSTRING& nameapp, XCHAR* recopilatedir)
{
	return AdjustRootPathDefault(nameapp.Get(), recopilatedir);
}




