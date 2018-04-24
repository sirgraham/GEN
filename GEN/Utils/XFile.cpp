//------------------------------------------------------------------------------------------
//	XFILE.CPP
//	
//	File Functions Class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 16/12/2003 21:55:49
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include <string.h>

#include "XFactory.h"
#include "XDebug.h"

#include "Cipher.h"

#include "XFile.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	
#ifdef XDEBUG		
XFILEOPENLIST	XFileOpenList;
#endif
	
//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  XFILECONTAINER::XFILECONTAINER
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			16/12/2003 21:56:07
//	
//	@return				
//	*/
//-------------------------------------------------------------------
XFILE::XFILE()
{
	Clean();
}


//-------------------------------------------------------------------
//  XFILE::~XFILE
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			24/03/2004 11:47:31
//	
//	@return				
//	*/
//-------------------------------------------------------------------
XFILE::~XFILE()
{
	if(isopen) this->Close();
	
	Clean();
}



//-------------------------------------------------------------------
//  XFILE::EraseBlock
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			10/01/2001 17:11:38
//	
//	@return 			bool : 
//	@param				size : 
*/
//-------------------------------------------------------------------
bool XFILE::EraseBlock(int size)
{
	XBYTE* tbuffer;	
	int		 position;
	int		 sizetomove;
	int    nblocks;
	int    rest;
	bool	 status = true;  

	if(!isopen) return false; 

	if(!GetPosition(position)) return false;

	sizetomove = GetSize() - position - size;

	tbuffer= new XBYTE [XFILE_MAXBUFFER];
	if(!tbuffer) return false;

	nblocks = (sizetomove / XFILE_MAXBUFFER);
	rest    = (sizetomove % XFILE_MAXBUFFER);

	for(int c=0; c<nblocks; c++)
		{
		  SetPosition(position + size + (c*XFILE_MAXBUFFER)); 
			if(Read(tbuffer, XFILE_MAXBUFFER))
				{
					SetPosition(position + (c*XFILE_MAXBUFFER)); 
					Write(tbuffer, XFILE_MAXBUFFER);
				}			
		} 

  SetPosition(position + size + (nblocks*XFILE_MAXBUFFER)); 
	if(Read(tbuffer, rest))
		{
			SetPosition(position + (nblocks*XFILE_MAXBUFFER)); 
			Write(tbuffer, rest);	
		}		

	SetSize(GetSize() - size);

	delete [] tbuffer;

	return status;
}



//-------------------------------------------------------------------
//  XFILE::InsertBlock
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			10/01/2001 17:11:47
//
//	@return 			bool :
//	@param				buffer :
//  @param				size :
*/
//-------------------------------------------------------------------
bool XFILE::InsertBlock(XBYTE* buffer,int size)
{
	XBYTE*	movebuffer;
	int			origin;
	int			position;
 	
  bool	  status = true;  

	if(!isopen) return false; 
		  
	if(GetPosition(position)!=true) return false;
	
  origin = position;

	movebuffer= new XBYTE [XFILE_MAXBUFFER];
	if(!movebuffer) return false;

	ActualizeSize();

	int sizefile      = GetSize();
	int sizetomove		= sizefile-position;
	int nblockstomove = (sizetomove / XFILE_MAXBUFFER);
	int resttomove		= (sizetomove % XFILE_MAXBUFFER);
	int c;

	position = GetSize() - XFILE_MAXBUFFER;
	
	for(c=0; c<nblockstomove+1; c++)
		{
			int	rb;
			
			if(c>=nblockstomove)  
				{
					rb = resttomove;	
					position =  origin;					
				}
			 else 
			  {
				  rb = XFILE_MAXBUFFER;
				}
			
			SetPosition(position); 
			Read(movebuffer, &rb);

			SetPosition(position+size); 
			Write(movebuffer,rb);														

			position -= XFILE_MAXBUFFER;
		}

	SetPosition(origin); 	
  Write(buffer,size);
	
  if(status==true) ActualizeSize(); 

	delete [] movebuffer;
	
  return status;
}





/*-------------------------------------------------------------------
//	XFILE::OverwriteContent
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			12/06/2014 16:54:32
//	
//	@return 			bool : 
//
//  @param				xpath : 
*/
/*-----------------------------------------------------------------*/
bool XFILE::OverwriteContent(XCHAR* xpath)
{
	bool status=true;

	if(!Open(xpath,false)) return false;
		
	GetSize();

	XBYTE*			buffer;
  const	int	  sizeblock = 1024;
	int				  block			=	(cachesize/sizeblock);
	int				  mod				=	(cachesize%sizeblock);	

  buffer = new XBYTE [sizeblock];
	if(!buffer) return false;
					
	memset(buffer,0,sizeblock);

	for(int c=0;c<block;c++)
		{
			if(Write(buffer,sizeblock)!=true) 
				{									
					status=false;									
					break;
				} 
		}
					
	if(status==true)
		{
			if(Write(buffer,mod)!=true)  status=false;
		}

	delete [] buffer;

	Close();

	return status;
}






//-------------------------------------------------------------------
//  XFILECONTAINER::CreatePrimaryFile
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			16/12/2003 21:56:17
//	
//	@return				bool : 

*/
//-------------------------------------------------------------------
bool XFILECONTAINER::CreatePrimaryFile()
{	  
	file = xfactory->Create_File();
	if(!file) return false;

  eraseprimaryfile = true;

  return true;
}


//-------------------------------------------------------------------
//  XFILECONTAINER::GetPrimaryFile
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			16/12/2003 21:56:23
//	
//	@return				XFILE* : 
//	*/
//-------------------------------------------------------------------
XFILE* XFILECONTAINER::GetPrimaryFile()
{
  return file;
}


//-------------------------------------------------------------------
//  XFILECONTAINER::DeletePrimaryFile
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			16/12/2003 21:56:31
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool XFILECONTAINER::DeletePrimaryFile()
{
  if(!file)  return false;

  if(!eraseprimaryfile) return false;

	xfactory->Delete_File(file);

	file = NULL;

	return true;
}







#ifdef XDEBUG	


/*-------------------------------------------------------------------
//	XFILEOPENLIST::~XFILEOPENLIST
*/	
/**	
//	
//	 Class Destructor XFILEOPENLIST
//	
//	@author				Abraham J. Velez
//	@version			12/06/2014 17:18:27
//	
*/
/*-----------------------------------------------------------------*/
XFILEOPENLIST::~XFILEOPENLIST()
{	
	
}


//-------------------------------------------------------------------
// XFILEOPENLIST::Add
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			13/03/2002 18:54:20
//	
//	@return 			bool : 
//	@param				xfile : 
*/
//-------------------------------------------------------------------
bool XFILEOPENLIST::Add(XFILE* xfile)
{
	filelist.Add(xfile);

	return true;
}


//-------------------------------------------------------------------
// XFILEOPENLIST::Del
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			13/03/2002 18:54:26
//	
//	@return 			bool : 
//	@param				xfile : 
*/
//-------------------------------------------------------------------
bool XFILEOPENLIST::Del(XFILE* xfile)
{
	if(filelist.IsEmpty()) return false;

	filelist.Delete(xfile);

	return true;
}


//-------------------------------------------------------------------
// XFILEOPENLIST::CloseAll
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			13/03/2002 18:54:32
//	
//	@return 			bool : 
//	*/
//-------------------------------------------------------------------
bool XFILEOPENLIST::CloseAll()
{	
	if(filelist.IsEmpty()) return false;

	for(XDWORD c=0;c<filelist.GetSize();c++)
		{
			XFILE* xfile = (XFILE*)filelist.Get(c);

			if(xfile) xfile->Close();
		}
	
	return true;
}




/*-------------------------------------------------------------------
//	XFILEOPENLIST::DisplayAll
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			13/06/2014 11:26:07
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool XFILEOPENLIST::DisplayAll()
{
	XDWORD nfilesopen =	filelist.GetSize();
		
	if(!nfilesopen) 
		{
			XDEBUG_PRINTHEADER(__L("ALL FILES HAVE BEEN CLOSED"));		
		}
	 else
		{				
			XDEBUG_PRINTHEADER(__L("NOT ALL FILES ARE CLOSED"));						
			
			XDEBUG_PRINT(__L("Number files not closed: %d"), nfilesopen);			
		
			XDEBUG_PRINT(__L("Path + File Name"));

			for(XDWORD c=0; c<nfilesopen; c++)
				{
					XFILE* xfile = (XFILE*)filelist.Get(c);						
					
					if(xfile) XDEBUG_PRINT(__L("%s")	,	xfile->GetPathNameFile());												
				}				

			XDEBUG_PRINTHEADER(NULL);
		}

	CloseAll();

	filelist.DeleteContents();
	filelist.DeleteAll();

	return true;
}



	
#endif

