/*------------------------------------------------------------------------------------------
//	DIOWEBPAGEHTMLCREATOR.CPP
//	
//	DIO Web Page HTML Creator class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 08/01/2014 17:18:15
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "XFactory.h"
#include "XFileTXT.h"
#include "XBuffer.h"

#include "DIOWebServer.h"
#include "DIOWebPageHTMLCreator.h"

#include "XDebug.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  DIOWEBPAGEHTMLCREATOR::DIOWEBPAGEHTMLCREATOR
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			07/02/2014 11:00:26
//	
//	@return 			
//  @param				xpublisher : 
//  @param				connexion : 
*/
/*-----------------------------------------------------------------*/
DIOWEBPAGEHTMLCREATOR::DIOWEBPAGEHTMLCREATOR(DIOWEBSERVER_CONNEXION* connexion) : XSTRING()
{
	Clean();
	
	this->connexion		 = connexion;

	RegisterEvent(DIOWEBPAGEHTMLCREATORXEVENTTYPE_XTAGFOUND);
}




//-------------------------------------------------------------------
//  DIOWEBPAGEHTMLCREATOR::~DIOWEBPAGEHTMLCREATOR
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/02/2007 17:46:41
//	
//	@return			 
//	*/
//-------------------------------------------------------------------
DIOWEBPAGEHTMLCREATOR::~DIOWEBPAGEHTMLCREATOR()
{	
	DeRegisterEvent(DIOWEBPAGEHTMLCREATORXEVENTTYPE_XTAGFOUND);

	Clean();
}





/*-------------------------------------------------------------------
//  DIOWEBPAGEHTMLCREATOR::Printf
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			18/01/2011 11:24:11
//	
//	@return				
bool : 
//	@param				mask : 
//  @param				... : 
*/
/*-----------------------------------------------------------------*/
bool DIOWEBPAGEHTMLCREATOR::Printf(XCHAR* mask,...)
{
	XSTRING outstring;

	va_list arg;
			
	va_start(arg, mask);
	
	outstring.FormatArg(mask, &arg, true);
		
	va_end(arg);


  //--------------------------------------------------
  // To See better the sorce code of WEB page
		
	if(connexion)
		{
			connexion->Send(outstring.Get());
			#ifdef XDEBUG
			//connexion->Send(__L("\n"));
			#endif
		}
	 else
	  {
			Add(outstring.Get());
			#ifdef XDEBUG
			//Add(__L("\n"));
			#endif
		}
	
	return true;
}


/*-------------------------------------------------------------------
//	DIOWEBPAGEHTMLCREATOR::Print
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			11/11/2015 15:56:45
//	
//	@return 			bool : 
//
//  @param				text : 
*/
/*-----------------------------------------------------------------*/
bool DIOWEBPAGEHTMLCREATOR::Print(XCHAR* text)
{		
	if(connexion)
		{
			connexion->Send(text);
			#ifdef XDEBUG
			//connexion->Send(__L("\n"));
			#endif
		}
	 else
	  {
			Add(text);
			#ifdef XDEBUG
			//Add(__L("\n"));
			#endif
		}
	
	return true;
}



/*-------------------------------------------------------------------
//  DIOWEBPAGEHTMLCREATOR::Table_Ini
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			29/11/2013 13:11:53
//	
//	@return 			bool : 
//	@param				border : 
*/
/*-----------------------------------------------------------------*/
bool DIOWEBPAGEHTMLCREATOR::Table_Ini(int border)
{
	this->tableborder = border;

	Printf(__L("<table style=\"border-collapse: collapse; font-family: Arial; font-size: 0.85em;\">"), border);	
	 
	return true;
}




/*-------------------------------------------------------------------
//  DIOWEBPAGEHTMLCREATOR::Table_Line
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			29/11/2013 12:07:57
//	
//	@return 			bool : 
//	@param				columns : 
//  @param				... : 
*/
/*-----------------------------------------------------------------*/
bool DIOWEBPAGEHTMLCREATOR::Table_Line(int columns, ...)
{
	va_list arg;

	va_start(arg,columns); 
	
	Printf(__L("<tr>"));

	for(int c=0;c<columns;c++)
		{		
			int													width = va_arg(arg, int);
			DIOWEBPAGEHTMLCREATORALIGN	align = (DIOWEBPAGEHTMLCREATORALIGN)va_arg(arg, int);
			XCHAR*											text  = va_arg(arg, XCHAR*);
			XSTRING											alignstr;				
			XSTRING											textrev;

			textrev.FormatArg(text, &arg, true);

			alignstr.Empty();

			switch(align)
				{
					case DIOWEBPAGEHTMLCREATORALIGN_NONE	 : alignstr = __L("");				break;
					case DIOWEBPAGEHTMLCREATORALIGN_LEFT   : alignstr = __L("left");	  break;
					case DIOWEBPAGEHTMLCREATORALIGN_RIGHT  : alignstr = __L("right");		break;
					case DIOWEBPAGEHTMLCREATORALIGN_CENTER : alignstr = __L("center");  break;
				}			

			Printf(__L("<td style=\"border: %dpx solid silver;\" width=\"%d\" align=\"%s\">%s</td>"), tableborder, width, alignstr.Get(), textrev.Get());
				
		}
	
	Printf(__L("</tr>"));	

	va_end(arg);

	return true;
}



/*-------------------------------------------------------------------
//  DIOWEBPAGEHTMLCREATOR::Table_PrintLine
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			29/11/2013 12:07:38
//	
//	@return 			bool : 
//	@param				colorbackground : 
//  @param				columns : 
//  @param				... : 
*/
/*-----------------------------------------------------------------*/
bool DIOWEBPAGEHTMLCREATOR::Table_Line(XCHAR* colorbackground, int columns, ...)
{
	va_list	arg;
	
	va_start(arg, columns); 

	XSTRING tr;
	XSTRING color;
	color = colorbackground;

	if(!color.IsEmpty())
		{
			tr = __L("<tr bgcolor=\"");
			tr += colorbackground;
			tr += "\">";
		}
	 else tr = __L("<tr>");
				
	Printf(tr.Get());

	for(int c=0;c<columns;c++)
		{		
			int													width = va_arg(arg, int);
			DIOWEBPAGEHTMLCREATORALIGN	align = (DIOWEBPAGEHTMLCREATORALIGN)va_arg(arg, int);
			XCHAR*											text  = va_arg(arg, XCHAR*);
			XSTRING											alignstr;
			XSTRING											textrev;

			textrev.FormatArg(text, &arg, true);

			alignstr.Empty();

			switch(align)
				{
					case DIOWEBPAGEHTMLCREATORALIGN_NONE	 : alignstr = __L("");				break;
					case DIOWEBPAGEHTMLCREATORALIGN_LEFT	 : alignstr = __L("left");	  break;
					case DIOWEBPAGEHTMLCREATORALIGN_RIGHT  : alignstr = __L("right");		break;
					case DIOWEBPAGEHTMLCREATORALIGN_CENTER : alignstr = __L("center");  break;
				}			

			Printf(__L("<td style=\"border: %dpx solid silver;\" width=\"%d\" align=\"%s\">%s</td>"), tableborder, width, alignstr.Get(), textrev.Get());									
		}
	
	Printf(__L("</tr>"));	

	va_end(arg);

	return true;
}




/*-------------------------------------------------------------------
//  DIOWEBPAGEHTMLCREATOR::Table_LineColor
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			03/02/2014 17:49:35
//	
//	@return 			bool : 
//	@param				columns : 
//  @param				... : 
*/
/*-----------------------------------------------------------------*/
bool DIOWEBPAGEHTMLCREATOR::Table_LineColor(int columns, ...)
{
	va_list arg;

	va_start(arg,columns); 
	
	Printf(__L("<tr>"));

	for(int c=0;c<columns;c++)
		{		
			int													width			= va_arg(arg, int);
			XCHAR*											colortxt	= va_arg(arg, XCHAR*);
			XCHAR*											colorbkg	= va_arg(arg, XCHAR*);
			DIOWEBPAGEHTMLCREATORALIGN	align			= (DIOWEBPAGEHTMLCREATORALIGN)va_arg(arg, int);
			XCHAR*											text			= va_arg(arg, XCHAR*);
			XSTRING											alignstr;
			XSTRING											textrev;

			textrev.FormatArg(text, &arg, true);

			alignstr.Empty();

			switch(align)
				{
					case DIOWEBPAGEHTMLCREATORALIGN_NONE	 : alignstr = __L("");				break;
					case DIOWEBPAGEHTMLCREATORALIGN_LEFT   : alignstr = __L("left");	  break;
					case DIOWEBPAGEHTMLCREATORALIGN_RIGHT  : alignstr = __L("right");		break;
					case DIOWEBPAGEHTMLCREATORALIGN_CENTER : alignstr = __L("center");  break;
				}			
		
			Printf(__L("<td bgcolor=\"%s\" style=\"border: %dpx solid silver;\" width=\"%d\" align=\"%s\"><font color=\"%s\">%s</font></td>"), colorbkg, tableborder, width, alignstr.Get(), colortxt, textrev.Get());										
		}
	
	Printf(__L("</tr>"));	

	va_end(arg);

	return true;
}



/*-------------------------------------------------------------------
//  DIOWEBPAGEHTMLCREATOR::Table_End
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			29/11/2013 12:07:34
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool DIOWEBPAGEHTMLCREATOR::Table_End()
{
	Printf(__L("</table>"));
	return true;
}




/*-------------------------------------------------------------------
//  DIOWEBPAGEHTMLCREATOR::AddAutoRefresh
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			08/01/2014 17:47:05
//	
//	@return 			bool : 
//	@param				refresh : 
//  @param				url : 
*/
/*-----------------------------------------------------------------*/
bool DIOWEBPAGEHTMLCREATOR::AddAutoRefresh(int refresh, XSTRING* url)
{
	if(refresh<0) return false;	

	XSTRING meta;
	
	if(url)
				meta.Format(__L("<META http-equiv=\"refresh\" content=\"%d;URL=%s\"/>"), refresh, url->Get());		
	 else meta.Format(__L("<META http-equiv=\"refresh\" content=\"%d;\"/>")			 , refresh);	
	
	Add(meta.Get());
	
	return true;
}




/*-------------------------------------------------------------------
//  DIOWEBPAGEHTMLCREATOR::AddFile
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			10/01/2014 9:26:03
//	
//	@return 			bool : 
//	@param				xpath : 
*/
/*-----------------------------------------------------------------*/
bool DIOWEBPAGEHTMLCREATOR::AddFile(XPATH& xpath)
{
  XFILETXT filetxt;      
  bool     status = true;
  bool     intag  = false;

  if(!filetxt.Open(xpath)) return false;

  if(filetxt.ReadAllFile())
    { 
			Empty();

      XBUFFER LFdata;  

      filetxt.GetLF(LFdata);

      for(int c=0;c<filetxt.GetNLines();c++)
        {
          XSTRING* line = filetxt.GetLine(c);
          if(line)
            { 
              XSTRING inioutput;
              XSTRING endoutput;
              int     result;
                                    
              result = CheckXTAG(line, intag, inioutput, endoutput);
              switch(result)
                {
                  case DIOWEBPAGEHTMLCREATORTAGXENGINESTATUS_NOTFOUND		: { if(!line->IsEmpty())
																																							{
																																								if(Add(*line)) Add(filetxt.GetLF()->Get());                      																																									
																																							}                                                                             
																																					}
																																					break;

									case DIOWEBPAGEHTMLCREATORTAGXENGINESTATUS_FOUNDEND		: { if(!inioutput.IsEmpty())
																																							{
																																								if(Add(inioutput)) Add(filetxt.GetLF()->Get()); 																																								
																																							}      

																																						DIOWEBPAGEHTMLCREATORXEVENT xevent(this, DIOWEBPAGEHTMLCREATORXEVENTTYPE_XTAGFOUND);							
	
																																						xevent.SetPageHTMLCreator(this);
																																						xevent.SetXTAG(&lastXTAG);
	
																																						PostEvent(&xevent);
																																						
																																						if(!endoutput.IsEmpty())
																																							{
																																								if(Add(endoutput)) Add(filetxt.GetLF()->Get()); 																																								
																																							}              
																																					}	  
																																					break;

                  case DIOWEBPAGEHTMLCREATORTAGXENGINESTATUS_FOUNDNOTEND: { if(!inioutput.IsEmpty())
																																							{
																																								if(Add(inioutput)) Add(filetxt.GetLF()->Get()); 																																								
																																							} 

																																						if(!endoutput.IsEmpty())
																																							{
																																								if(Add(endoutput)) Add(filetxt.GetLF()->Get()); 																																								
																																							}      
																																					}
																																					break; 
                }
            }
        }
		}

  filetxt.Close(); 

	return status;
}





/*-------------------------------------------------------------------
//  DIOWEBPAGEHTMLCREATOR::CheckXTAG
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			09/01/2014 9:21:56
//	
//	@return 			DIOWEBPAGEHTMLCREATORTAGXENGINESTATUS : 
//	@param				originalline : 
//  @param				intag : 
//  @param				inioutputline : 
//  @param				endoutputline : 
*/
/*-----------------------------------------------------------------*/
DIOWEBPAGEHTMLCREATORTAGXENGINESTATUS DIOWEBPAGEHTMLCREATOR::CheckXTAG(XSTRING* originalline,bool& intag, XSTRING& inioutputline,XSTRING& endoutputline)
{
  int index[3];   

  inioutputline.Empty();
  endoutputline.Empty();

  if(!intag)
    {
      index[0] = originalline->Find(__L("<?"), true);
      if(index[0]!=XSTRING_NOTFOUND)
        {
          index[1] = originalline->Find(DIOWEBPAGEHTMLCREATOR_TAGXENGINE, true);
          if(index[1]!=XSTRING_NOTFOUND)
            {
              lastXTAG.Empty();

              originalline->Copy(0,index[0],inioutputline);

              index[2] = originalline->Find(__L("?>"),true);
              if(index[2]!=XSTRING_NOTFOUND)
                {
                  XSTRING endline;

                  originalline->Copy(index[1]+3,index[2],lastXTAG);                  
                  originalline->Copy(index[2]+2,endoutputline);
                  
                  return DIOWEBPAGEHTMLCREATORTAGXENGINESTATUS_FOUNDEND;
                  
                } 
               else 
                { 
                  originalline->Copy(index[1]+3,lastXTAG);
                  intag = true;
                  return  DIOWEBPAGEHTMLCREATORTAGXENGINESTATUS_FOUNDNOTEND;
                }
            }
        }

      inioutputline = originalline->Get();
    }
   else 
    {    
      index[0] = originalline->Find(__L("?>"),true);
      if(index[0]!=XSTRING_NOTFOUND)
        {               
          XSTRING TAGstring;     

          originalline->Copy(0,index[0],TAGstring);
          lastXTAG+=TAGstring.Get();

          originalline->Copy(index[0]+2,endoutputline);

          intag = false;

          return DIOWEBPAGEHTMLCREATORTAGXENGINESTATUS_FOUNDEND;
        }
        
      lastXTAG += originalline->Get();
      
      inioutputline.Empty();
      endoutputline.Empty();

      return DIOWEBPAGEHTMLCREATORTAGXENGINESTATUS_FOUNDNOTEND;
    }

  return DIOWEBPAGEHTMLCREATORTAGXENGINESTATUS_NOTFOUND;
}




