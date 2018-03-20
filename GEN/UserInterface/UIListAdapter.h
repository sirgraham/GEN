/*------------------------------------------------------------------------------------------
//	UILISTADAPTER.H
*/	
/**	
// \file 
//   
//  list adapter for lists
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 13/10/2016 15:59:39
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _UILISTADAPTER_H_
#define _UILISTADAPTER_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
#include "XVector.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/

class UIWIDGET;
class UIMANAGER;

class UILISTADAPTERBASE
{
public:
	UILISTADAPTERBASE(UIMANAGER* uimanager)
	{
		this->uimanager = uimanager;
	}

	virtual ~UILISTADAPTERBASE() {}

	virtual UIWIDGET* GetWidget(XDWORD position) = 0;
	virtual XDWORD GetSize() = 0;

protected:
	UIMANAGER* uimanager;
};

template <class T>
class UILISTADAPTER : public UILISTADAPTERBASE
{
public:

																		UILISTADAPTER										(UIMANAGER* uimanager) : UILISTADAPTERBASE(uimanager)
																		{
																			Clean();

																			items.SetIsMulti(true);
																		}

	virtual													 ~UILISTADAPTER										()
																		{
																			Clean();
																		}

	T																	GetItem													(int position)
																		{
																			return items.Get(position);
																		}

	int																FindItem												(T item)
																		{
																			return items.Find(item);
																		}

	void															DeleteItem											(int position)
																		{
																			items.DeleteIndex(position);
																		}

	void															AddItem													(T item)
																		{
																			items.Add(item);
																		}

	void															InsertItem											(int position, T item)
																		{
																			items.Insert(position, item);
																		}

	virtual XDWORD										GetSize													()
																		{
																			return items.GetSize();
																		}

	void															SetWidth												(XDWORD width)													{ this->width = width;		}
	void															SetHeight												(XDWORD height)													{ this->height = height;	}

	virtual UIWIDGET*									GetWidget												(XDWORD position) = 0;

protected:

	XDWORD														width;
	XDWORD														height;

	XVECTOR<T>												items;
	
private:
	void															Clean														()
																		{
																			width = 0;
																			height = 0;
																		}
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

