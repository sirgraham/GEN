/*------------------------------------------------------------------------------------------
//  XORDEREDLIST.H
*/
/**
// \class
//
//  Ordered list
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 29/10/2015 16:58:53
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _XORDEREDLIST_H_
#define _XORDEREDLIST_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XList.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/

template <class T>
class XCOMPARATOR
{
public:
  virtual int Compare(T* a,T* b)=0;
};

template <class T, class Compare>
class XORDEREDLIST : public XLIST<T>
{

public:
            XORDEREDLIST  ()
            {
            Clean();

            }

  virtual   ~XORDEREDLIST ()
            {
            Clean();
            }

  virtual bool      Add                                   (T element)
                    {
                      XLISTDATA<T>* current=this->GetFirstData();
                      while (current)
                      {
                        int result=comparator.Compare(element,current->data);
                        switch(result)
                        {
                        case  0: return false; //the item already exists
                        case  1: return this->Insert(current,element);
                        }
                        current=current->next;
                      }

                      return XLIST<T>::Add(element);
                    }


protected:

  Compare comparator;

private:

  virtual void Clean()
  {

  }

};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

