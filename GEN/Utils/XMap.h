//------------------------------------------------------------------------------------------
//	XMAP.H
//	
/**	
// \class 
//   
//  eXtended Map Class
//   
//	@author	 Abraham J. Velez
//	@version 21/08/2006 22:18:05
*/	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
	
#ifndef _XMAP_H_
#define _XMAP_H_
	
	
//---- INCLUDES ----------------------------------------------------------------------------
	
#include "XBase.h"
#include "XVector.h"
	
//---- DEFINES & ENUMS  --------------------------------------------------------------------

#ifdef MICROCONTROLLER
#define XMAP_ADDINLIMIT						2
#else
#define XMAP_ADDINLIMIT						10
#endif

//---- CLASS -------------------------------------------------------------------------------
	
template<class KEY, class ELEMENT> 
class XMAP
{							
	public:
																		XMAP																	()																							
																		{	
																				// DIEGO: duplicate element bug fix
																				// SetIsMulti(true);

																				keys.SetIsMulti(false);
																				elements.SetIsMulti(true);
																		}
		virtual												 ~XMAP																	()																							{																				}


		bool														IsEmpty																()																							{ return keys.GetSize()?false:true;			}	
		XDWORD													GetSize																()																							{ return keys.GetSize();								}


		int															GetAddInLimit													()																							{	return keys.GetAddInLimit();					}
		bool														SetAddInLimit													(int addinlimit = XMAP_ADDINLIMIT)							
																		{
																			keys.SetAddInLimit(addinlimit);
																			elements.SetAddInLimit(addinlimit);
	
																			return true;
																		}
		
		void														SetIsMulti														(bool ismulti)																	
																		{	
																		  keys.SetIsMulti(ismulti);
																			elements.SetIsMulti(ismulti);
																		}	
																		

		int															CountKey															(KEY key)
																		{
																			int count = 0;

																			for(XDWORD c=0; c<keys.GetSize(); c++)
																				{
																					if(key==keys.Get(c)) count++;
																				}

																			return count;
																		}

		
		bool														Add																		(KEY key, ELEMENT element)
																		{
																			if(!keys.Add(key)) return false;
	
																			bool status = elements.Add(element);
																			if(!status) keys.Delete(key);

																			return status;
																		}
		
	
		int															Find																	(KEY key)																				{	return keys.Find(key);								}
		
		KEY															GetKey																(int index)																			{	return keys.Get(index);								}
		KEY															FastGetKey														(int index)																			{	return keys.FastGet(index);						}
		ELEMENT													GetElement														(int index)																			{	return elements.Get(index);						}
		ELEMENT													FastGetElement												(int index)																			{	return elements.FastGet(index);				}

		ELEMENT													Get																		(KEY key)																				
																		{
																			int position = NOTFOUND;
																			if (( position = keys.Find(key)) != NOTFOUND)
																			return elements.FastGet(position);

																			return (ELEMENT)0;
																		}

		bool														Set																		(KEY key,ELEMENT element)																				
																		{
																			int index=keys.Find(key);

																			if (index!=NOTFOUND)
																				return elements.Set(index,element);	

																			return false;
																		}



    bool														Insert																(int index, KEY key, ELEMENT element)
																		{
																			if(!keys.Insert(index, key)) return false;
																			return elements.Insert(index, element);
																		}

   
		bool														Delete																(KEY key)
																		{
																			int index = keys.Find(key);
																			if(index==NOTFOUND) return false;

																			ELEMENT element = elements.Get(index);

																			if(!keys.Delete(key)) return false;

																			return elements.Delete(element);
																		}
		

		bool														Delete																(KEY key, ELEMENT element)
																		{ 
																			bool	 status = false;
																			XDWORD c      = 0;

																			while(c<keys.GetSize())
																				{
																					KEY			_key		 = keys.Get(c);
																					ELEMENT _element = elements.Get(c);

																					if((_key == key) && (_element == element))
																						{
																							keys.DeleteIndex(c);
																							elements.DeleteIndex(c);

																							status = true;

																						} else c++;
																				}

																			return status;
																		}


		bool														DeleteAll															()
																		{
																			if(!keys.DeleteAll()) return false;
																			return elements.DeleteAll();
																		}


		bool														DeleteKeyContents											()																									{ return keys.DeleteContents();			}
		bool														DeleteElementContents									()																									{ return elements.DeleteContents();	}

    
    XVECTOR<KEY>										keys;
		XVECTOR<ELEMENT>								elements;

		ELEMENT													operator[](KEY k)
																		{
																				return this->Get(k);
																		}

	private:
	

		bool														CreateArraySize												(int nelements)
																		{
																			if(!keys.CreateArraySize(nelements)) return false;
	
																			return elements.CreateArraySize(nelements);
																		}


};
	
//---- INLINE FUNCTIONS --------------------------------------------------------------------
	
#endif

