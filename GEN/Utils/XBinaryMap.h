/*------------------------------------------------------------------------------------------
//	XBINARYMAP.H
*/	
/**	
// \file 
//   
//  binary map
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 24/12/2015 10:10:22
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _XBINARYMAP_H_
#define _XBINARYMAP_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XAVLTree.h"
#include "XPair.h"

#include "XMemory.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
template <class K, class E, class Comparator = XLESS<XPAIR<K, E> > , class Container = XAVLTREE<XPAIR<K, E>, Comparator> >
class XBINARYMAP
{	
	public:

		class XITERATOR;

																								XBINARYMAP																			(bool multi = true) : container(multi)
																								{
																									Clean();
																								}

		virtual																		 ~XBINARYMAP																			()
																								{
																									Clean();
																								}

		void																				Add																							(K key, E element)
																								{
																									container.Add(XPAIR<K, E>(key, element));
																								}

		E																						Get																							(K key)
																								{
																									XPAIR<K, E> p = container.Get(XPAIR<K, E>(key, E()));
																									return p.right;	
																								}

		bool																				Delete																					(const K& key)
																								{
																									return container.Delete(key);
																								}

		bool																				Delete																					(XITERATOR it)
																								{
																									return container.Delete(it.ite);
																								}

		XDWORD																			GetSize																					()
																								{
																									return container.GetSize();
																								}

		typename XBINARYMAP::XITERATOR							Begin																						()
																								{
																									XITERATOR it;
																									it.ite = container.Begin();
																									return it;
																								}
	
		typename XBINARYMAP::XITERATOR							End																							()
																								{
																									XITERATOR it;
																									it.ite = container.End();
																									return it;
																								}

			XLIST<E>																	GetMultiple																		(K& key)
																								{
																									XLIST<E> ret;

																									GetMultiple(XPAIR<K, E>(key, E()), ret);

																									return ret;
																								}

		void																				GetMultiple																		(K& key, XLIST<E>& list)
																								{
																									XLIST<XPAIR<K, E> > lst;
																									container.GetMultiple(XPAIR<K, E>(key, E()), lst);

																									// now transform lst to the list
																									typename XLIST<XPAIR<K, E> >::XITERATOR it;
																									for(it = lst.Begin(); it != lst.End(); it++)
																										{
																											list.Add((*it).right);
																										}
																								}

		int																					Find																					(K& key)
																								{
																									return container.Find(XPAIR<K, E>(key, E()));
																								}

	////////////////////////////////////////////////////////////////////////////
	//                           BINARYMAP ITERATOR                           //
	////////////////////////////////////////////////////////////////////////////
	class XITERATOR
	{
		friend class XBINARYMAP;

		public:
	
																								XITERATOR																				()
																								{
																								}
	
																								XITERATOR																				(typename XBINARYMAP::XITERATOR& rhs)
																								{
																									*this = rhs;
																								}
	
				typename XBINARYMAP::XITERATOR &				operator=																				(const typename XBINARYMAP::XITERATOR& rhs)
																								{
																									ite = rhs.ite;
																									return *this;
																								}
	
				bool																		operator!=																			(const typename XBINARYMAP::XITERATOR & rhs)
																								{
																									return ite != rhs.ite;
																								}
	
				typename XBINARYMAP::XITERATOR					operator++																			()
																								{
																									++ite;
	
																									return *this;
																								}
	
				typename XBINARYMAP::XITERATOR					operator++																			(int) // postfix
																								{
																									typename XBINARYMAP::XITERATOR it = *this;
	
																									(*this).ite++;
	
																									return it;
																								}
	
				E																				operator*																				()
																								{
																									return (*ite).right;
																								}
	
				K																				GetKey																					()
																								{
																									return (*ite).left;
																								}
	
				E																				GetElement																			()
																								{
																									return (*ite).right;
																								}

		private:

			typename Container::XITERATOR							ite;
};

////////////////////////////////////////////////////////////////////////////////
//                           END BINARYMAP ITERATOR                           


	private:

		void																				Clean																						()
																								{
																								
																								}

		Container																		container;
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

