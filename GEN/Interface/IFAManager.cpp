/*------------------------------------------------------------------------------------------
//	IFAMANAGER.CPP
//	
//	IFA Manager class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 18/01/2012 19:56:53
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.			 
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "IFAForm.h"
#include "IFABitmap.h"
#include "IFAText.h"
#include "IFAButton.h"
#include "IFAMenu.h"

#include "IFAManager.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/





/*-------------------------------------------------------------------
//  IFAMANAGER::IFAMANAGER
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			23/01/2012 19:45:14
//	
//	@return				
//	@param				canvas : 
//  @param				scene : 
//  @param				screen : 
//  @param				inpmanager :  
//  @param				skinmanager : 
*/
/*-----------------------------------------------------------------*/
IFAMANAGER::IFAMANAGER(GRPCANVAS* canvas, GRPSCENE* scene, GRPSCREEN* screen, INPMANAGER* inpmanager, XFACTORY* xfactory, IFASKINMANAGER* skinmanager)
{
	Clean();

	this->canvas			= canvas;	
	this->scene				= scene;
	this->screen			= screen;
	this->inpmanager	= inpmanager;
	
	this->publisher		= publisher;
	this->skinmanager	= skinmanager;
}



/*-------------------------------------------------------------------
//  IFAMANAGER::~IFAMANAGER
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			18/01/2012 20:04:06
//	
//	@return				
//	*/
/*-----------------------------------------------------------------*/
IFAMANAGER::~IFAMANAGER()
{
	Clean();

	DeleteAllElements(true);
}



/*-------------------------------------------------------------------
// IFAMANAGER::LoadElements
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			18/01/2012 17:39:37
//	
//	@return				bool : 
//  @param				xpath : 
//  @param				namefile : 
*/
/*-----------------------------------------------------------------*/
bool IFAMANAGER::LoadElements(XPATH& xpath,XCHAR* namefile)
{
	if(!xfactory) return false;

	XFILEXML xmlfile();
	XPATH		 xpathfile;
	XPATH		 xpathcollisions;
	bool		 status = true;

	xpathfile = xpath.Get();
	xpathfile.Slash_Add();
	xpathfile += namefile;
		
	if(xmlfile.Open(xpathfile))
		{
			int index = 0;

			xmlfile.ReadAndDecodeAllLines();	

			XFILEXMLELEMENT* father	 = xmlfile.GetRoot();			
			if(father) 
				{	
					int index = 0;

					while(1)
						{
							if(!AddElement(xmlfile,father,index,NULL)) break;										
						}					

				} else status = false;

			xmlfile.Close();

		} else status = false;
	
	return status;
}




/*-------------------------------------------------------------------
//  IFAMANAGER::DeleteAllElements
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			23/01/2012 13:38:50
//	
//	@return				bool : 
//	@param				deletecontent : 
*/
/*-----------------------------------------------------------------*/
bool IFAMANAGER::DeleteAllElements(bool deletecontent)
{
	if(elements.IsEmpty()) return false;

	elements.DeleteContents();

	elements.DeleteAll();

	return true;
}




/*-------------------------------------------------------------------
//  IFAMANAGER::Clean
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			18/01/2012 20:04:44
//	
//	@return				void : 
//	*/
/*-----------------------------------------------------------------*/
void IFAMANAGER::Clean()
{
	canvas			= NULL;
	scene				= NULL;
	screen			= NULL;
	inpmanager	= NULL;	
	publisher		= NULL;
	skinmanager	= NULL;
}




/*-------------------------------------------------------------------
//  IFAMANAGER::AddElement
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			23/01/2012 18:28:36
//	
//	@return				bool : 
//	@param				xmlfile : 
//  @param				father : 
//  @param				index : 
//  @param				form : 
*/
/*-----------------------------------------------------------------*/
bool IFAMANAGER::AddElement(XFILEXML& xmlfile,XFILEXMLELEMENT* father,int& index,IFAFORM* form)
{
	XFILEXMLELEMENT* element = xmlfile.SearchElement(__L("ELEMENT"),index,father);
	if(!element) return false;
				
	index++;

	XSTRING type;
	XSTRING group;
		
	element->GetValueAttribute(__L("TYPE")	, type);
	element->GetValueAttribute(__L("GROUP")	, group);

	if(group.Compare(IFAMANAGER_INTERFACE)) return false;
		
	if(!type.Compare(IFAMANAGER_TYPEFORM))
		{
			IFAFORM* _form = (IFAFORM*)AddElement(element);
			if(_form) 
				{
					elements.Add(_form);
				} 
			 else return false;
						
			while(AddElement(xmlfile,element,index,_form))
				{

				}
		}
	 else
		{
			IFAOBJ* ifaobj = AddElement(element);
			if(ifaobj) 
				{   
					if(form) 
					   		form->AddObj(ifaobj); 
					 else elements.Add(ifaobj);
				}
		 	 else return false;
		}

	return true;
}




/*-------------------------------------------------------------------
//  IFAMANAGER::AddElement
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			23/01/2012 18:30:18
//	
//	@return				IFAOBJ* : 
//	@param				element : 
*/
/*-----------------------------------------------------------------*/
IFAOBJ* IFAMANAGER::AddElement(XFILEXMLELEMENT* element)
{
	if(!element) return NULL;
							
	XSTRING type;
	XSTRING group;
	XSTRING name;
									
	element->GetValueAttribute(__L("TYPE")	, type);
	element->GetValueAttribute(__L("GROUP")	, group);
	element->GetValueAttribute(__L("NAME")	,	name);

	if(group.Compare(IFAMANAGER_INTERFACE)) return NULL;

	if(!type.Compare(IFAMANAGER_TYPEFORM))
		{
			IFAFORM* form = new IFAFORM(canvas,screen,inpmanager,xfactory,skinmanager,IFAOBJTYPE_FORM);
			if(!form) return NULL;

			form->SetName(name);

			return form;
		}

	if(!type.Compare(IFAMANAGER_TYPEBITMAP))
		{	
			IFABITMAP* bitmap = new IFABITMAP(canvas, screen, inpmanager, xfactory, skinmanager, IFAOBJTYPE_BITMAP);
			if(!bitmap) return NULL;		

			bitmap->SetName(name);
				
			return bitmap;	
		}

	if(!type.Compare(IFAMANAGER_TYPETEXT))
		{	
			IFATEXT* text = new IFATEXT(canvas, screen, inpmanager, xfactory, skinmanager, IFAOBJTYPE_TEXT);
			if(!text) return NULL;		

			text->SetName(name);
				
			return text;	
		}
	
	if(!type.Compare(IFAMANAGER_TYPEBUTTON))
		{	
			IFABUTTON* button = new IFABUTTON(canvas, screen, inpmanager, xfactory, publisher, skinmanager, IFAOBJTYPE_BUTTON);
			if(!button) return NULL;		

			button->SetName(name);
				
			return button;	
		}


	if(!type.Compare(IFAMANAGER_TYPEMENU))
		{	
			IFAMENU* menu = new IFAMENU(canvas, scene, screen, inpmanager, xfactory, publisher, skinmanager, IFAOBJTYPE_BUTTON);
			if(!menu) return NULL;		

			menu->SetName(name);
				
			return menu;	
		}


	return NULL;
}
