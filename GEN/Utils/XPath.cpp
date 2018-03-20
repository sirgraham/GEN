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

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "XFactory.h"
#include "XDir.h"
#include "XString.h"
#include "XDebug.h"

#include "XPath.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	

//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  XPATH::XPATH
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			12/08/2003 10:14:53
//	
//	@return 			
//	*/
//-------------------------------------------------------------------
XPATH::XPATH() : XSTRING()
{

}


/*-------------------------------------------------------------------
//  XPATH::XPATH
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			23/03/2011 11:39:10
//	
//	@return				
//	*/
/*-----------------------------------------------------------------*/
XPATH::XPATH(int size) : XSTRING(size)
{

}


/*-------------------------------------------------------------------
//  XPATH::XPATH
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			23/03/2011 11:39:10
//	
//	@return				
//	*/
/*-----------------------------------------------------------------*/
XPATH::XPATH(const char* path) : XSTRING(path)
{
	
}



/*-------------------------------------------------------------------
//  XPATH::XPATH
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			23/03/2011 11:39:10
//	
//	@return				
//	*/
/*-----------------------------------------------------------------*/
XPATH::XPATH(const XCHAR* path) : XSTRING(path)
{
	
}


/*-------------------------------------------------------------------
//  XPATH::XPATH
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			23/03/2011 11:39:10
//	
//	@return				
//	*/
/*-----------------------------------------------------------------*/
XPATH::XPATH(const XCHAR* path,int size) : XSTRING(path,size)
{

}

																

/*-------------------------------------------------------------------
//  XPATH::XPATH
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			23/03/2011 11:39:10
//	
//	@return				
//	*/
/*-----------------------------------------------------------------*/
XPATH::XPATH(XPATH& path) : XSTRING(path)
{

}



/*-------------------------------------------------------------------
//  XPATH::XPATH
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			23/03/2011 11:39:10
//	
//	@return				
//	*/
/*-----------------------------------------------------------------*/
XPATH::XPATH(XSTRING& string) : XSTRING(string)
{
	
}



/*-------------------------------------------------------------------
//  XPATH::XPATH
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			23/03/2011 11:39:10
//	
//	@return				
//	*/
/*-----------------------------------------------------------------*/
XPATH::XPATH(XWORD* path) : XSTRING(path)
{
	
}


//-------------------------------------------------------------------
//  XPATH::~XPATH
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			12/08/2003 10:14:53
//	
//	@return 			
//	*/
//-------------------------------------------------------------------
XPATH::~XPATH()
{

}


/*-------------------------------------------------------------------
//  XPATH::GetDrive
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/03/2011 15:57:05
//	
//	@return				bool : 
//	@param				drive : 
*/
/*-----------------------------------------------------------------*/
bool XPATH::GetDrive(XSTRING& drive)
{
	Split(&drive,NULL,NULL,NULL);

	if(drive.IsEmpty()) return false;

	return true;
}
         


/*-------------------------------------------------------------------
//  XPATH::GetPath
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			13/04/2008 10:16:59
//	
//	@return				bool : 
//	@param				path : 
*/
/*-----------------------------------------------------------------*/
bool XPATH::GetPath(XSTRING& path)
{
	XPATH xpath;

	Split(NULL,&xpath,NULL,NULL);

	if(xpath.IsEmpty()) return false;

	xpath.Slash_Add();

	path = xpath;

	return true;
}


/*-------------------------------------------------------------------
//  XPATH::GetPathAndNamefile
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			13/04/2008 10:18:32
//	
//	@return				bool : 
//	@param				pathnamefile : 
*/
/*-----------------------------------------------------------------*/
bool XPATH::GetDriveAndPath(XSTRING& drivepath)
{
	XSTRING drive;
	XPATH   path;
	
	Split(&drive, &path, NULL, NULL);	
	
	drivepath  = drive;
	drivepath += path;

	if(drivepath.IsEmpty()) return false;

	return true;
}




/*-------------------------------------------------------------------
//  XPATH::GetPathAndNamefile
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			13/04/2008 10:18:32
//	
//	@return				bool : 
//	@param				pathnamefile : 
*/
/*-----------------------------------------------------------------*/
bool XPATH::GetPathAndNamefile(XSTRING& pathnamefile)
{
	XPATH      path;
	XSTRING name;

	Split(NULL,&path,&name,NULL);	

	path.Slash_Add();
	
	pathnamefile  = path;
	pathnamefile += name;

	if(pathnamefile.IsEmpty()) return false;

	return true;
}



/*-------------------------------------------------------------------
//  XPATH::GetNamefile
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			16/03/2011 11:33:00
//	
//	@return				bool : 
//	@param				namefile : 
*/
/*-----------------------------------------------------------------*/
bool XPATH::GetNamefile(XSTRING& namefile)
{	
	Split(NULL,NULL,&namefile,NULL);
	
	if(namefile.IsEmpty()) return false;

	return true;
}




/*-------------------------------------------------------------------
//  XPATH::GetNamefileExt
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			13/04/2008 10:23:30
//	
//	@return				bool : 
//	@param				namefileext : 
*/
/*-----------------------------------------------------------------*/
bool XPATH::GetNamefileExt(XSTRING& namefileext)
{	
	XSTRING name;
	XSTRING ext;

	Split(NULL,NULL,&name,&ext);

	namefileext  = name;
	namefileext += ext;

	if(namefileext.IsEmpty()) return false;

	return true;
}




/*-------------------------------------------------------------------
//  XPATH::GetExt
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/03/2011 18:33:50
//	
//	@return				bool : 
//	@param				ext : 
*/
/*-----------------------------------------------------------------*/
bool XPATH::GetExt(XSTRING& ext)
{
	Split(NULL,NULL,NULL,&ext);

	if(ext.IsEmpty()) return false;

	return true;
}



/*-------------------------------------------------------------------
//  XPATH::GetPathInSequence
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			19/03/2012 16:49:34
//	
//	@return 			bool : 
//	@param				index : 
//  @param				pathpart : 
*/
/*-----------------------------------------------------------------*/
bool XPATH::GetPathInSequence(int index,XSTRING& pathpart)
{
	pathpart.Empty();
	
	int   slashcount = 0;
	XDWORD c					 = 0;

	while(c<GetSize()) 
		{
		  if((Get()[c]=='/') || (Get()[c]=='\\'))
				{
					if(slashcount==index) 
						{
						  return true; 
						}
					 else 
						{
							pathpart.Empty();
							slashcount++;
						}
				
				} else pathpart.Add(Get()[c]);
					
			c++;
		}

	pathpart.Empty();

	return false;
}



/*-------------------------------------------------------------------
//  XPATH::GetDrive
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/03/2011 15:57:05
//	
//	@return				const XCHAR* : 
*/
/*-----------------------------------------------------------------*/
const XCHAR* XPATH::SetOnlyDrive()
{
	XSTRING string;

	string.Empty();

	GetDrive(string);

	Set(string);
	
	return string.Get();
}



/*-------------------------------------------------------------------
//  XPATH::SetOnlyPath
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			16/03/2011 13:19:46
//	
//	@return				const XCHAR* : 					
*/
/*-----------------------------------------------------------------*/
const XCHAR* XPATH::SetOnlyPath()
{
	XSTRING string;

	string.Empty();

	GetPath(string);

	Set(string);
	
	return string.Get();
}




/*-------------------------------------------------------------------
//  XCHAR* XPATH::SetOnlyDriveAndPath
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			10/01/2014 16:01:29
//	
//	@return 			const : 
//	*/
/*-----------------------------------------------------------------*/
const XCHAR* XPATH::SetOnlyDriveAndPath()
{
	XSTRING string;

	string.Empty();

	GetDriveAndPath(string);

	Set(string);
	
	return string.Get();
}





/*-------------------------------------------------------------------
//  XPATH::SetOnlyPathAndNamefile
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			13/04/2008 10:18:32
//	
//	@return				const XCHAR* : 
*/
/*-----------------------------------------------------------------*/
const XCHAR* XPATH::SetOnlyPathAndNamefile()
{
	XSTRING string;

	string.Empty();

	GetPathAndNamefile(string);

	Set(string);
	
	return string.Get();
}



/*-------------------------------------------------------------------
//  XPATH::SetOnlyNamefile
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			18/03/2011 10:39:51
//	
//	@return				const XCHAR* : 
//	*/
/*-----------------------------------------------------------------*/
const XCHAR* XPATH::SetOnlyNamefile()
{
	XSTRING string;

	string.Empty();

	GetNamefile(string);

	Set(string);
	
	return string.Get();
}



/*-------------------------------------------------------------------
//  XPATH::SetOnlyNamefileExt
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			18/03/2011 10:39:51
//	
//	@return				const XCHAR* : 
//	*/
/*-----------------------------------------------------------------*/
const XCHAR* XPATH::SetOnlyNamefileExt()
{
	XSTRING string;

	string.Empty();

	GetNamefileExt(string);

	Set(string);
	
	return string.Get();
}



/*-------------------------------------------------------------------
//  XPATH::SetOnlyExt
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			18/03/2011 10:39:51
//	
//	@return				const XCHAR* : 
//	*/
/*-----------------------------------------------------------------*/
const XCHAR* XPATH::SetOnlyExt()
{
	XSTRING string;

	string.Empty();

	GetExt(string);

	Set(string);
	
	return string.Get();
}



/*-------------------------------------------------------------------
//  XPATH::DeleteDrive
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/05/2011 12:21:08
//	
//	@return				bool : 
//	*/
/*-----------------------------------------------------------------*/
bool XPATH::DeleteDrive()
{	
	if(IsEmpty())		 return false;	
	if(GetSize()<=2) return false;
	
	if(Get()[1]!=__C(':')) return false;

	XPATH xpath2;
	
	xpath2 = Get();
	xpath2.DeleteCharacters(0,2);

	Set(xpath2.Get());

	return true;
}




/*-------------------------------------------------------------------
//	XPATH::Slash_HaveAtLast
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			10/07/2015 13:39:41
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool XPATH::Slash_HaveAtLast()
{	
	if((Character_GetLast() == __C('\\')) || (Character_GetLast() == __C('/'))) return true;

	return false;
}



/*-------------------------------------------------------------------
//  XPATH::Slash_Add
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/03/2011 16:12:51
//	
//	@return				bool : 
//	*/
/*-----------------------------------------------------------------*/
bool XPATH::Slash_Add()
{	
	int						count[2] = { 0 , 0 };	
	XCHAR* str						 = Get();
	bool					status	 = false; 

	int c=0;
	while(str[c])
		{
			if(str[c]==__C('/'))  count[0]++;
			if(str[c]==__C('\\')) count[1]++;	

			c++;
		}

	if(!Slash_HaveAtLast())
		{
			if(count[0]>count[1])
						 Add("/");
				else Add("\\"); 

			Slash_Normalize();

			status = true;
		}

	return status;
}



/*-------------------------------------------------------------------
//  XPATH::Slash_Normalize
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/03/2011 16:37:40
//	
//	@return				bool : 
//	@param				inverse : 
*/
/*-----------------------------------------------------------------*/
bool XPATH::Slash_Normalize(bool inverse)
{
	if(IsEmpty()) return false;

	XCHAR* str = Get();
	int						c		=	0;

	while(str[c])
		{
			if(inverse)
			 {  
				 if(str[c]==__C('/'))  str[c] = __C('\\');				 
			 }
			else
			 {
				 if(str[c]==__C('\\')) str[c] = __C('/');
			 }

			c++;
		}

	return true;
}


/*-------------------------------------------------------------------
//  XPATH::Slash_Delete
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			21/03/2011 12:39:37
//	
//	@return				bool : 
//	*/
/*-----------------------------------------------------------------*/
bool XPATH::Slash_Delete()
{	
	bool status = false;

	if(Slash_HaveAtLast())
		{
			DeleteCharacter('\\' , XSTRINGCONTEXT_ATEND);
			DeleteCharacter('/'  , XSTRINGCONTEXT_ATEND);
			
			status = true;
		}

	return status;
}




/*-------------------------------------------------------------------
//  XPATH::AddToNameFile
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			13/01/2013 1:36:38
//	
//	@return 			bool : 
//	@param				isprefix : 
//  @param				tag : 
*/
/*-----------------------------------------------------------------*/
bool XPATH::AddToNameFile(bool isprefix, XCHAR* tag)
{
	XSTRING  drive;
	XPATH	   xpath;
	XSTRING  name;
	XSTRING  newname;
	XSTRING  ext;

	if(!Split(&drive,&xpath,&name,&ext)) return false;

	//if(!drive.IsEmpty()) drive.Add(__L("/"));
	
	xpath.Slash_Add();

	if(isprefix)
		{
			newname  = tag;
			newname += name;
		}
	 else
	  {
			newname  = name;
			newname += tag;
		}

	Format(__L("%s%s%s%s"),drive.Get(),xpath.Get(),newname.Get(),ext.Get());

	return true;
}




/*-------------------------------------------------------------------
//  XPATH::Create
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			22/03/2011 17:28:59
//	
//	@return				bool : 
//	@param				ini : 
//  @param				... : 
*/
/*-----------------------------------------------------------------*/
bool XPATH::Create(int nelements,...)
{
	va_list	arg;

	Empty();

	va_start(arg, nelements); 
  
	int c=0;
	while(c<nelements)
		{
			XPATH xpath;
				
			xpath = (XCHAR*)va_arg(arg, XCHAR*);
			if(!xpath.IsEmpty())
				{				
					if(c && xpath.Character_GetFirst()!=__C('.'))
						{
							Slash_Add();
						}

					Add(xpath); 
				}

			c++;
		}			

	va_end(arg);

	Slash_Normalize();

	return true;
}



/*-------------------------------------------------------------------
//	XPATH::Create
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			28/03/2014 10:35:42
//	
//	@return 			bool : 
//
 
//  @param				sectiontype : 
//  @param				nelements : 
//  @param				... : 
*/
/*-----------------------------------------------------------------*/
bool XPATH::Create(XPATHSMANAGERSECTIONTYPE sectiontype, int nelements, ...)
{
	XPATH	  xpathsection;	
	va_list arg;
	bool		status = true;

	Empty();

	XPATHSMANAGER::GetInstance().GetPathOfSection(sectiontype, xpathsection);			

	Add(xpathsection); 

	va_start(arg, nelements); 
  
	int c=0;
	while(c<nelements)
		{
			XPATH xpath;
				
			xpath = (XCHAR*)va_arg(arg, XCHAR*);
			if(!xpath.IsEmpty())
				{				
					if(c && xpath.Character_GetFirst()!=__C('.'))
						{
							Slash_Add();
						}

					Add(xpath); 
				}

			c++;
		}			

	va_end(arg);

	Slash_Normalize();
	
	return status;
}




//-------------------------------------------------------------------
//  XPATH::Split
/**
//	
//	@author        Abraham J. Velez
//	@version       25/2/2003   18:19:8
//	
//	@return        bool : 
//	@param	       drive : 
//	@param	       path : 
//	@param	       name : 
//	@param	       ext : 
*/
//-------------------------------------------------------------------
bool XPATH::Split(XSTRING* drive, XPATH* xpath, XSTRING* name, XSTRING* ext)
{	
	XCHAR* pathstr = Get();
	int    idrive	 = 0;
	int    iname	 = 0;
	int    iext		 = 0;
	int		 c;

	if(IsEmpty()) return false;
	if(!pathstr)  return false;
	
	c = 0;
	while(pathstr[c])
		{
			if(pathstr[c]==__C(':')) 
				{
					if(drive) Copy(0,c+1,(*drive));

					idrive = c+1;
					break;
				}

			c++;		
		}

	c = (GetSize()-1);
	while(pathstr[c])
		{
			if((pathstr[c]==__C('\\')) || (pathstr[c]==__C('/')))
				{
					break;
				}

			if(pathstr[c]==__C('.')) 
				{
					if(ext) Copy(c, GetSize(), (*ext));

					iext = c;
					break;
				}

			c--;		
		}
	 
	c = (iext)?iext:GetSize();	
	while(c>=0)
		{
			if((!c) || pathstr[c]==__C('\\') || pathstr[c]==__C('/')  || pathstr[c]==__C(':')) 
				{
					if(pathstr[c]==__C('\\') || pathstr[c]==__C('/') || pathstr[c]==__C(':')) c++;	

					if(name) Copy(c, (iext)?iext:GetSize(), (*name)); 

					iname = c;
					break;
				}

			c--;		
		}

	if(xpath && iname) Copy(idrive, iname, (*xpath));			
		
	return true;  
}




