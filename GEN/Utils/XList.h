/*------------------------------------------------------------------------------------------
//  XLIST.H
*/
/**
// \file
//
//  double linked list
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 22/10/2015 13:24:31
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _XLIST_H_
#define _XLIST_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>

#include "XBase.h"
#include "XMemory.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

/*---- CLASS -----------------------------------------------------------------------------*/

template <class T>
class XLISTDATA
{
  template <class U>
  friend class XLIST;

public:
                                    XLISTDATA                         ()
                                    {
                                      Clean();
                                    }


    virtual                        ~XLISTDATA                         ()
                                    {
                                      Delete();
                                      Clean();
                                    }

    bool                            Copy                                  (XLISTDATA* listdata)
                                    {
                                      if(!listdata) return false;

                                      active = listdata->active;
                                      data   = listdata->data;

                                      return true;
                                    }


    bool                            Delete                                ()
                                    {
                                      if(!active) return false;

                                      active = false;
                                      data   = T();

                                      return true;
                                    }

    bool                            active;
    T                               data;
    XLISTDATA<T>*                   next;
    XLISTDATA<T>*                   prev;
private:
    void                            Clean                                 ()
                                    {
                                      active = false;
                                      data   = T();

                                      next    = 0;
                                      prev    = 0;
                                    }


};

template <class T>
XLISTDATA<T>* Increment(XLISTDATA<T>* data)
{
  return data->next;
}

template <class T>
XLISTDATA<T>* Decrement(XLISTDATA<T>* data)
{
  return data->prev;
}

template <class T>
bool IsFirst(XLISTDATA<T>* data)
{
  return data->prev == NULL;
}

template <class T>
bool IsLast(XLISTDATA<T>* data)
{
  return data == NULL;
}

template <class T>
class XLIST
{
  public:
    class XITERATOR;
                                    XLIST                                 ()
                                    {
                                      Clean();
                                    }
    virtual                        ~XLIST                                 ()
                                    {
                                      DeleteAll();
                                      Clean();
                                    }

    XLIST<T>&                       operator=(XLIST<T>& rhs)
                                    {
                                      XLISTDATA<T>* it = rhs.GetFirstData();

                                      while(!IsLast(it))
                                        {
                                          Add(it->data);

                                          it = Increment(it);
                                        }

                                      return *this;
                                    }


    bool                            IsEmpty                               () { return nelements == 0; }
    XDWORD                          GetSize                               () { return nelements; }

    bool                            Resize                                (XDWORD newsize)
                                    {
                                      if(newsize == 0)
                                        {
                                          return DeleteAll();
                                        }

                                      if(newsize >= nelements)
                                        {
                                          // should insert new elements at the tail
                                          XDWORD toinsert = newsize - nelements;

                                          XLISTDATA<T>* data = lastelement;

                                          for(XDWORD i = 0; i < toinsert; i++)
                                            {
                                              XLISTDATA<T>* newdata = new XLISTDATA<T>();
                                              if(!newdata) return false;

                                              newdata->prev = data;
                                              data->next = newdata;
                                              lastelement = newdata;
                                              data = newdata;
                                            }

                                          nelements = newsize;
                                          lastelement = data;
                                          return true;
                                        }

                                      XDWORD todelete = nelements - newsize;

                                      XLISTDATA<T>* data = lastelement;
                                      XLISTDATA<T>* previous;
                                      for(XDWORD i = 0; i < todelete; i++)
                                        {
                                          previous = data->prev;

                                          data->Delete();
                                          delete data;

                                          data = previous;
                                        }

                                      lastelement = data;

                                      nelements = newsize;
                                      return true;
                                    }

    void                            SetIsMulti                            (bool ismulti) { this->ismulti = ismulti; }

    virtual bool                    Add                                   (T element)
                                    {
                                      if(!ismulti)
                                        {
                                          if(Find(element) != NOTFOUND)
                                            {
                                              return false;
                                            }
                                        }

                                      // new element to add
                                      XLISTDATA<T>* data = new XLISTDATA<T>();
                                      data->active = true;
                                      data->data = element;

                                      // special case if the list is empty
                                      if(nelements == 0)
                                        {
                                          firstelement = data;
                                          lastelement = data;
                                          nelements++;
                                          return true;
                                        }

                                      // fix pointers
                                      lastelement->next = data;
                                      data->prev = lastelement;
                                      nelements++;
                                      lastelement = data;
                                      return true;
                                    }

    int                             Find                                  (T element)
                                    {
                                      int index = 0;

                                      XLISTDATA<T>* data = firstelement;

                                      while(data)
                                        {
                                          if(data->data == element)
                                            {
                                              return index;
                                            }

                                          data = data->next;
                                          index++;
                                        }

                                      return NOTFOUND;
                                    }

    // overload not by return, need to select a name
    XLISTDATA<T>*                   FindData                              (T element)
                                    {
                                      XDWORD index = 0;

                                      XLISTDATA<T>* data = firstelement;

                                      while(data)
                                        {
                                          if(data->data == element)
                                            {
                                              return data;
                                            }

                                          data = data->next;
                                          index++;
                                        }

                                      return NULL;
                                    }

    T                               Get                                   (XDWORD index)
                                    {
                                      if(index >= nelements)
                                        {
                                          return T();
                                        }

                                      XLISTDATA<T>* data = firstelement;

                                      for(XDWORD i = 0; i < index; i++)
                                        {
                                          data = data->next;
                                        }

                                      return data->data;
                                    }

    XLISTDATA<T>*                   GetData                                   (XDWORD index)
                                    {
                                      if(index >= nelements)
                                        {
                                          return T();
                                        }

                                      XLISTDATA<T>* data = firstelement;

                                      for(XDWORD i = 0; i < index; i++)
                                        {
                                          data = data->next;
                                        }

                                      return data;
                                    }

    bool                            Set                                   (XDWORD index, T element)
                                    {
                                      if(index >= nelements)
                                        {
                                          return false;
                                        }

                                      XLISTDATA<T>* data = firstelement;

                                      for(XDWORD i = 0; i < index; i++)
                                        {
                                          data = data->next;
                                        }

                                      data->data = element;
                                      return true;
                                    }

    T                               GetFirst                              () { return firstelement->data; }
    XLISTDATA<T>*                   GetFirstData                          () { return firstelement;       }
    T                               GetLast                               () { return lastelement->data;  }
    XLISTDATA<T>*                   GetLastData                           () { return lastelement;        }

    bool                            Insert                                (XDWORD index, T element)
                                    {
                                      if(index >= nelements) return false;

                                      if(!ismulti)
                                        {
                                          if(Find(element) != NOTFOUND)
                                            {
                                              return false;
                                            }
                                        }

                                      // special cases for first and last elements
                                      if(index == 0)
                                        {
                                          Insert(firstelement, element);
                                          return true;
                                        }

                                      if(index == (nelements-1))
                                        {
                                          Insert(lastelement, element);
                                          return true;
                                        }

                                      // general middle case
                                      XLISTDATA<T>* data = firstelement;

                                      for(XDWORD i = 0; i < nelements; i++)
                                        {
                                          if(i == index)
                                            {
                                              return Insert(data, element);
                                            }

                                          data = data->next;
                                        }

                                      return false;
                                    }

    bool                            Insert                                (XLISTDATA<T>* position, T element)
                                    {
                                      if(nelements == 0)
                                        {
                                          XLISTDATA<T>* newdata = new XLISTDATA<T>();
                                          if(!newdata) return false;

                                          newdata->data = element;
                                          firstelement = newdata;
                                          lastelement = newdata;
                                          nelements++;
                                          return true;
                                        }

                                      // special case, we take for granted it's one past the last
                                      if(position == NULL)
                                        {
                                          Add(element);
                                        }

                                      // last element is a general case
                                      if(position == firstelement)
                                        {
                                          XLISTDATA<T>* newdata = new XLISTDATA<T>();
                                          newdata->data = element;
                                          newdata->next = firstelement;
                                          firstelement->prev = newdata;
                                          firstelement = newdata;
                                          nelements++;
                                          return true;
                                        }

                                      XLISTDATA<T>* previous = position->prev;
                                      XLISTDATA<T>* newdata = new XLISTDATA<T>();
                                      if(!newdata) return false;

                                      newdata->data = element;
                                      newdata->next = position;
                                      newdata->prev = previous;
                                      previous->next = newdata;
                                      position->prev = newdata;

                                      nelements++;

                                      return true;
                                    }

    bool                            InsertPrevious                        (XLISTDATA<T>* position, T element)
                                    {
                                      Insert(position, element);
                                    }

    bool                            InsertNext                            (XLISTDATA<T>* position, T element)
                                    {
                                      Insert(position->next, element);
                                    }

    bool                            Delete                                (T element)
                                    {
                                      XLISTDATA<T>* data = firstelement;

                                      if(nelements == 1)
                                        {
                                          if(firstelement->data != element) return false;

                                          DeleteAll();
                                          return true;
                                        }

                                      for(XDWORD i = 0; i < nelements; i++)
                                        {
                                          if(data->data == element)
                                            {
                                              data->Delete();

                                              // check if data is first or last element
                                              if(data == firstelement)
                                                {
                                                  firstelement = firstelement->next;
                                                  firstelement->prev = 0;
                                                }
                                              else if(data == lastelement)
                                                {
                                                  lastelement = lastelement->prev;
                                                  lastelement->next = 0;
                                                }
                                              else
                                                {
                                                  data->prev->next = data->next;
                                                  data->next->prev = data->prev;
                                                }

                                              delete data;
                                              nelements--;

                                              return true;
                                          }

                                          data = data->next;
                                        }

                                      return false;
                                    }

        bool                        Delete                                (XLISTDATA<T>* element)
                                    {
                                      if(nelements == 0) return false; // something went wrong

                                      if(nelements == 1)
                                        {
                                          return DeleteAll();
                                        }

                                      // general case
                                      if(element == firstelement)
                                        {
                                          firstelement = firstelement->next;
                                          firstelement->prev = 0;
                                          element->Delete();
                                          delete element;
                                          nelements--;
                                          return true;
                                        }

                                      if(element == lastelement)
                                        {
                                          lastelement = lastelement->prev;
                                          lastelement->next = 0;
                                          element->Delete();
                                          delete element;
                                          nelements--;
                                          return true;
                                        }

                                      element->prev->next = element->next;
                                      element->next->prev = element->prev;

                                      element->Delete();
                                      delete element;

                                      nelements--;

                                      return true;
                                    }


    bool                            DeleteLast                            ()
                                    {
                                      if(nelements == 0) return false;

                                      if(nelements == 1)
                                        {
                                          DeleteAll();
                                          return true;
                                        }

                                      XLISTDATA<T>* data = lastelement;
                                      lastelement = data->prev;
                                      lastelement->next = 0;

                                      data->Delete();
                                      nelements--;

                                      delete data;
                                      return true;
                                    }

    bool                            DeleteIndex                           (XDWORD index)
                                    {
                                      if(index >= nelements)
                                        {
                                          return false;
                                        }

                                      if(nelements == 1)
                                        {
                                          DeleteAll();
                                          return true;
                                        }

                                      if(index == 0)
                                        {
                                          XLISTDATA<T>* data = firstelement;

                                          firstelement = data->next;
                                          firstelement->prev = 0;

                                          data->Delete();
                                          delete data;
                                          nelements--;

                                          return true;
                                        }
                                      else if(index == (nelements-1))
                                        {
                                          DeleteLast();

                                          return true;
                                        }

                                      XLISTDATA<T>* data = firstelement;

                                      for(XDWORD i = 0; i < index; i++)
                                        {
                                          data = data->next;
                                        }

                                      // delete the element here
                                      XLISTDATA<T>* prev = data->prev;
                                      XLISTDATA<T>* next = data->next;

                                      prev->next = next;
                                      next->prev = prev;

                                      data->Delete();
                                      delete data;
                                      nelements--;

                                      return true;
                                    }

    bool                            DeleteAll                             ()
                                    {
                                      XLISTDATA<T>* data = firstelement;
                                      XLISTDATA<T>* datanext;

                                      for(XDWORD i = 0; i < nelements; i++)
                                        {
                                          datanext = data->next;

                                          data->Delete();
                                          delete data;

                                          data = datanext;
                                        }

                                      firstelement = NULL;
                                      nelements = 0;
                                      return true;
                                    }

    bool                            ResizeContents                        (XDWORD newsize)
                                    {
                                      if(newsize == 0)
                                        {
                                          DeleteAll();
                                          return true;
                                        }

                                      if(newsize >= nelements)
                                        {
                                          return false;
                                        }

                                      XDWORD todelete = nelements - newsize;

                                      XLISTDATA<T>* data = lastelement;
                                      XLISTDATA<T>* previous;
                                      for(XDWORD i = 0; i < todelete; i++)
                                        {
                                          previous = data->prev;

                                          data->Delete();
                                          delete data;

                                          data = previous;
                                        }

                                      lastelement = data;
                                      lastelement->next = 0;

                                      nelements = newsize;
                                      return true;
                                    }

    bool                            DeleteContents                        ()
                                    {
                                      if(nelements == 0)
                                        {
                                          return false;
                                        }

                                      XLISTDATA<T>* current = firstelement;
                                      XLISTDATA<T>* next = firstelement->next;

                                      for(XDWORD i = 0; i < nelements; i++)
                                        {
                                          next = current->next;

                                          delete current->data;
                                          current->Delete();
                                          delete current;

                                          current = next;
                                        }

                                      firstelement = NULL;
                                      nelements = 0;
                                      return true;
                                    }

    bool                            Swap                                (XDWORD firstindex, XDWORD secondindex)
                                    {
                                      if(firstindex >= nelements) return false;
                                      if(secondindex >= nelements) return false;

                                      XLISTDATA<T>* first = firstelement;
                                      XLISTDATA<T>* second = firstelement;

                                      if((firstindex + secondindex) > nelements)
                                        {
                                          XLISTDATA<T>* data = firstelement;
                                          for(XDWORD i = 0; i < nelements; i++)
                                            {
                                              if(i == firstindex)
                                                {
                                                  first = data;
                                                }

                                              if(i == secondindex)
                                                {
                                                  second = data;
                                                }

                                              data = data->next;
                                            }
                                        }
                                      else
                                        {
                                          for(XDWORD i = 0; i < firstindex; i++)
                                            {
                                              first = first->next;
                                            }

                                          for(XDWORD i = 0; i < secondindex; i++)
                                            {
                                              second = second->next;
                                            }
                                        }

                                      T s = second->data;
                                      second->data = first->data;
                                      first->data = s;

                                      return true;
                                    }

  typename XLIST::XITERATOR         Begin                                           ()
                                    {
                                      XITERATOR it;
                                      it.data = GetFirstData();
                                      return it;
                                    }

  typename XLIST::XITERATOR         End                                             ()
                                    {
                                      XITERATOR it;
                                      it.data = NULL;
                                      return it;
                                    }

  ////////////////////////////////////////////////////////////////////////
  //                           XLIST ITERATOR                           //
  ////////////////////////////////////////////////////////////////////////
  class XITERATOR
  {
    friend class XLIST;
  public:

                                              XITERATOR                                       ()
                                              {
                                              }

                                              XITERATOR                                       (typename XLIST::XITERATOR& rhs)
                                              {
                                                *this = rhs;
                                              }

      typename XLIST::XITERATOR &             operator=                                       (const typename XLIST::XITERATOR& rhs)
                                              {
                                                data = rhs.data;
                                                return *this;
                                              }

      bool                                    operator!=                                      (const typename XLIST::XITERATOR & rhs)
                                              {
                                                return data != rhs.data;
                                              }

      typename XLIST::XITERATOR               operator++                                      ()
                                              {
                                                data = data->next;

                                                return *this;
                                              }

      typename XLIST::XITERATOR               operator++                                      (int) // postfix
                                              {
                                                typename XLIST::XITERATOR it = *this;

                                                data = data->next;

                                                return it;
                                              }

      T                                       operator*                                       ()
                                              {
                                                return data->data;
                                              }

  private:
    XLISTDATA<T>*                             data;
  };
  ////////////////////////////////////////////////////////////////////////////
  //                           END XLIST ITERATOR                           //
  ////////////////////////////////////////////////////////////////////////////


  protected:

  private:
    void                            Clean                                 ()
                                    {
                                      ismulti         = true;
                                      nelements       = 0;
                                      firstelement    = NULL;
                                      lastelement     = NULL;
                                    }

    bool                            ismulti;
    XDWORD                          nelements;
    XLISTDATA<T>*                   firstelement;
    XLISTDATA<T>*                   lastelement;
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

