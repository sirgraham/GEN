//----------s--------------------------------------------------------------------------------
//	XPATH.H
//	
/**	
// \class 
//   
//  Path Functions
//   
//	@author	 Abraham J. Velez
//	@version 09/08/2002
*/	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#ifndef _XPATH_H_
#define _XPATH_H_
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "XBase.h"
#include "XString.h"	
#include "XVector.h"	

#include "XPathsManager.h"
	
//---- DEFINES & ENUMS  --------------------------------------------------------------------

//---- CLASS -------------------------------------------------------------------------------


class XPATH : public XSTRING
{
	public:
															XPATH													();	
															XPATH													(int size);
															XPATH													(const char* path);
															XPATH													(const XCHAR* path);										
															XPATH													(const XCHAR* path, int size);										
															XPATH													(XPATH& path);
															XPATH													(XSTRING& path);												
															XPATH													(XWORD* path);
		
		virtual									 ~XPATH													();
		
		void											operator =										(const char* path)						{ Set(path);														}
		void											operator =										(const XCHAR* path)						{ Set(path);														}
		void											operator =										(XPATH& path)									{ Set(path);														}
		void											operator =										(XSTRING& string)							{ Set(string);													}
		void											operator =										(XWORD* path)									{ Set(path);														}
		
		void											operator +=										(const char* path)						{ Add(path);														}
		void											operator +=										(const XCHAR* path)						{ Add(path);														}
		void											operator +=										(XPATH& path)									{ Add(path);														}
		void											operator +=										(XSTRING& string)							{ Add(string);													}
		void											operator +=										(XCHAR path)									{ Add(path);														}

		bool											operator <										(XPATH& path) 								{ return (bool)(Compare(path) == -1);		}
		bool											operator >										(XPATH& path)									{ return (bool)(Compare(path) ==  1);		}
		bool											operator <=										(XPATH& path) 								{ return (bool)(Compare(path) !=  1);		}
		bool											operator >=										(XPATH& path)									{ return (bool)(Compare(path) != -1);		}
		bool											operator ==										(XPATH& path)									{ return (bool)(Compare(path) ==  0);		}
		bool											operator !=										(XPATH& path)								  { return (bool)(Compare(path) !=  0);		}

		bool											operator <										(XSTRING& string) 						{ return (bool)(Compare(string) == -1);	}
		bool											operator >										(XSTRING& string)							{ return (bool)(Compare(string) ==  1);	}
		bool											operator <=										(XSTRING& string) 						{ return (bool)(Compare(string) !=  1);	}
		bool											operator >=										(XSTRING& string)							{ return (bool)(Compare(string) != -1);	}
		bool											operator ==										(XSTRING& string)							{ return (bool)(Compare(string) ==  0);	}
		bool											operator !=										(XSTRING& string)							{ return (bool)(Compare(string) !=  0);	}

		XCHAR											operator []										(int position)								
															{ 
																if(IsEmpty())						return 0;
																if(position<0)					return text[0];
																if(position>=(int)size)	return text[size-1];	
																return text[position];																																			
															}
				
		bool											GetDrive											(XSTRING& drive);		
		bool											GetPath												(XSTRING& path);
		bool											GetDriveAndPath								(XSTRING& drivepath);
		bool											GetPathAndNamefile						(XSTRING& pathnamefile);		
		bool											GetNamefile										(XSTRING& namefile);		
		bool											GetNamefileExt								(XSTRING& namefileext);		
		bool											GetExt												(XSTRING& ext);	

		bool											GetPathInSequence							(int index, XSTRING& pathpart);

		const XCHAR*							SetOnlyDrive									();
		const XCHAR*							SetOnlyPath										();
		const XCHAR*							SetOnlyDriveAndPath						();
		const XCHAR*							SetOnlyPathAndNamefile				();
		const XCHAR*							SetOnlyNamefile								();
		const XCHAR*							SetOnlyNamefileExt						();
		const XCHAR*							SetOnlyExt										();

		bool											DeleteDrive										();
		
		bool											Slash_HaveAtLast							();
		bool											Slash_Add											();
		bool											Slash_Normalize								(bool inverse = false);
		bool											Slash_Delete									();

		bool											AddToNameFile									(bool isprefix, XCHAR* tag);
	
		bool											Create												(int nelements, ...);
		bool											Create												(XPATHSMANAGERSECTIONTYPE sectiontype, int nelements, ...);
	
	private:

		bool										  Split													(XSTRING* drive, XPATH* xpath, XSTRING* name, XSTRING* ext);		
};

	
//---- INLINE FUNCTIONS --------------------------------------------------------------------


#endif

