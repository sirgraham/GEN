/*------------------------------------------------------------------------------------------
// XDictionary.HPP
*/
/**
// \class
//
// Manages dictionaries
//
//	@author: Diego Martinez Ruiz de Gaona 
//
//	Date of Creation : 12/02/2018 11:02:00 
//	Last Modification : 
*/
/* GEN  Copyright (C).  All right reserved.
/*------------------------------------------------------------------------------------------*/


#ifndef _XDICTIONARY_HPP
#define _XDICTIONARY_HPP
/*---- INCLUDES --------------------------------------------------------------------------*/

#include <XList.h>

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

/*---- CLASS -----------------------------------------------------------------------------*/

template<class T>
class XDICTIONARYENTRY
{
public:
		XSTRING key;
		T value;
};


template<class T>
class XDICTIONARY
{
public:

		XDICTIONARY()
		{
				Clean();
		}
		~XDICTIONARY()
		{
				entries.DeleteContents();
				entries.DeleteAll();
		}

		T& operator [](int n)
		{
				return entries[n];			
		}

		T& operator [](XCHAR* key)
		{
				for (XDWORD e = 0; e < entries.GetSize(); e++)
				{
						if (entries.Get(e)->key == key)
								return entries.Get(e)->value;
				}				
				entries.Add(new XDICTIONARYENTRY<T>);
				entries.GetLast()->key = key;
				entries.GetLast()->value = T();								
				return entries.GetLast()->value;
		}

		XLIST<XDICTIONARYENTRY<T>*> entries;

protected:
		
private:
		void Clean()
		{

		}
};



/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif