//------------------------------------------------------------------------------------------
//  XVECTOR.H
//
/**
// \class
//
//  eXtended Vector Class
//
//  @author  Abraham J. Velez
//  @version 19/08/2006 16:46:37
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _XVECTOR_H_
#define _XVECTOR_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>

#include "XBase.h"

#include "XMemory.h"


//---- DEFINES & ENUMS  --------------------------------------------------------------------

#ifdef MICROCONTROLLER
#define XVECTOR_DEFAULTADDINLIMIT   2
#else
#define XVECTOR_DEFAULTADDINLIMIT   10
#endif

//---- CLASS -------------------------------------------------------------------------------

template<class T>
class XVECTOR
{
  public:
                                    XVECTOR                               ()
                                    {
                                      Clean();
                                    }


    virtual                        ~XVECTOR                               ()
                                    {
                                      DeleteAll();
                                      Clean();
                                    }


    bool                            IsEmpty                               ()                                              { return nelements?false:true;                          }
    XDWORD                          GetSize                               ()                                              { return nelements;                                     }

    int                             GetAddInLimit                         ()                                              { return addinlimit;                                    }
    void                            SetAddInLimit                         (int addinlimit = XVECTOR_DEFAULTADDINLIMIT)    { this->addinlimit = addinlimit?addinlimit:1;           }

    void                            SetIsMulti                            (bool ismulti)                                  { this->ismulti = ismulti;                              }

    bool                            Resize                                (XDWORD newsize)
                                    {
                                      XDWORD _nelements = nelements;

                                      if(newsize == _nelements) return false;

                                      if(newsize > _nelements)
                                        {
                                          for(XDWORD c=0;c<(newsize - _nelements);c++)
                                            {
                                              Add(0);
                                              //Add(T()); // can't go adding T() yet
                                            }
                                        }
                                       else
                                        {
                                          for(XDWORD c=0;c<(_nelements - newsize);c++)
                                            {
                                              DeleteLast();
                                            }
                                        }

                                      return true;
                                    }


    virtual bool                    Add                                   (T element)
                                    {
                                      if(!ismulti)
                                        {
                                          for(XDWORD c=0; c<nelements; c++)
                                            {
                                              if(array[c] == element) return false;
                                            }
                                        }

                                      //if(!AdjustArraySize(true)) return false;
                                      if(!ResizeAdd())
                                        return false;

                                      array[nelements] = element;

                                      nelements++;

                                      return true;
                                    }


    int                             Find                                  (T element)
                                    {
                                      if(!array) return NOTFOUND;

                                      for(int c=nelements-1; c>=0; c--)
                                        {
                                          if(array[c] == element) return c;
                                        }

                                      return NOTFOUND;
                                    }


    T                               Get                                   (XDWORD index)
                                    {
                                      if(!array) return T();
                                      if(index >= nelements)  return T();

                                      return array[index];
                                    }

    T*                              GetPointer                            (XDWORD index)
                                    {
                                      if(!array) return 0;
                                      if(index >= nelements)  return 0;

                                      return &array[index];
                                    }

    inline T                        FastGet                               (XDWORD index)
                                    {
                                      return array[index];
                                    }


    T                               GetLast                               ()                                              { return Get(nelements-1);                              }


    bool                            Set                                   (XDWORD index, T element)
                                    {
                                      if(index >= nelements)  return false;
                                      array[index] = element;

                                      return true;
                                    }


    bool                            Insert                                (XDWORD index, T element)
                                    {
                                      //if((index < 0)  || ((XDWORD)index >= nelements))  return false;

                                      if (index>nelements)
                                        index=nelements;

                                      if(!Add(element)) return false;

                                      for(XDWORD c=GetSize()-1; c>(XDWORD)index; c--)
                                        {
                                          array[c] = array[c-1];
                                        }

                                      return Set(index, element);
                                    }


    virtual bool                    Delete                                (T element)
                                    {
                                      if(!array) return false;

                                      for(XDWORD c=0;c<nelements;c++)
                                        {
                                          if(array[c] == element)
                                            {
                                              //array[c] = (T)0;
                                              ResizeRemove(c);
                                              nelements--;
                                              //AdjustArraySize(false);

                                              return true;
                                            }
                                        }

                                      return false;
                                    }


    bool                            DeleteLast                            ()
                                    {
                                      if(!array) return false;
                                      //Delete(GetLast());

                                      DeleteIndex(nelements-1);

                                      return true;
                                    }


    virtual bool                    DeleteIndex                           (XDWORD index)
                                    {
                                      if(!array) return false;
                                      if(index >= nelements)  return false;

                                      ResizeRemove(index);
                                      //array[index] = (T)0;
                                      nelements--;
                                      //AdjustArraySize(false);

                                      return true;
                                    }


    void                            FastDeleteIndex                       (XDWORD index)
                                    {
                                      ResizeRemove(index);
                                      //array[index] = (T)0;
                                      nelements--;
                                      //AdjustArraySize(false);
                                    }


    bool                            DeleteAll                             ()
                                    {
                                      if(!array) return false;

                                      delete [] array;

                                      array      = NULL;
                                      narray     = 0;
                                      nelements  = 0;

                                      return true;
                                    }

    bool                            ResizeContents                        (int newsize)
                                    {

                                      int nSize=(int)GetSize();

                                      if(newsize<nSize)
                                        {
                                          for(int counter=newsize; counter<nSize; counter++)
                                            {
                                              T element = GetLast();
                                              if(element)
                                                {
                                                  DeleteLast();
                                                  delete element;
                                                }
                                            }
                                        }
                                       else
                                        {
                                          for(int counter=nelements; counter<newsize; counter++)
                                            {
                                              ResizeAdd();
                                            }
                                        }


                                      return true;
                                    }


    bool                            DeleteContentsInstanced()
                                    {
                                      /*
                                      while (nelements>0)
                                      {
                                        T element = array[0];
                                        while  (this->Delete(element));
                                        delete element;
                                      }
                                      */

                                      if(!nelements) return false;

                                      for(XDWORD s=0; s<nelements; s++)
                                        {
                                          T element = array[s];
                                          if (element)
                                            {
                                              for(XDWORD c=s+1; c<nelements; c++)
                                                {
                                                  if(array[c] == element)
                                                      array[c] = NULL;
                                                }

                                              delete(element);
                                              array[s] = NULL;
                                            }

                                        }
                                      nelements = 0;
                                      return true;
                                    }

    bool                            DeleteContents                        ()
                                    {
                                      if(!nelements) return false;

                                      for(XDWORD c=0; c<nelements; c++)
                                        {
                                          //T element = Get(c);
                                          T element = array[c];
                                          if(element)
                                            {
                                              //Set(c, NULL);
                                              array[c]=NULL;
                                              delete element;
                                            }
                                        }
                                      nelements=0;
                                      return true;
                                    }


    bool                            Swap                                  (XDWORD firstindex,XDWORD secondindex)
                                    {
                                      if(firstindex   >=  nelements)  return false;
                                      if(secondindex  >=  nelements)  return false;

                                      T element = array[secondindex];

                                      array[secondindex] = array[firstindex];
                                      array[firstindex]  = element;

                                      return true;
                                    }


    bool                            Copy                                  (XVECTOR<T>* origin)
                                    {
                                      if(this == origin)
                                        {
                                          return false;
                                        }

                                      DeleteAll();

                                      nelements   = origin->GetSize();

                                      XDWORD  _narray   = nelements;
                                      int     sizearray = (_narray*sizeof(T));

                                      array = new T[_narray];
                                      if(!array) return false;

                                      // memcpy in here instead of loop
                                      memcpy(array, origin->array, sizearray);
                                      //for(XDWORD e=0; e<nelements; e++)
                                      //  {
                                      //    array[e] = (T)origin->FastGet(e);
                                      //  }

                                      return true;
                                    }

  private:
                                    XVECTOR(const XVECTOR<T> & rhs)
                                    {

                                    }


    void                            Clean                                 ()
                                    {
                                      ismulti     = true;
                                      addinlimit  = XVECTOR_DEFAULTADDINLIMIT;
                                      nelements   = 0;
                                      narray      = 0;
                                      array       = NULL;
                                    }

    bool                            ResizeAdd                             ()
                                    {
                                      // althougb if nelement gets bigger than narray we have a serious problem
                                      if((nelements >= narray) || (!narray))
                                        {
                                          XDWORD newsize = nelements + addinlimit;

                                          T* newarray = new T[newsize];
                                          if(!newarray) return false;

                                          if(array)
                                            {
                                              memcpy(newarray, array, nelements*sizeof(T));
                                              delete [] array;
                                            }

                                          narray = newsize;
                                          array = newarray;

                                          return true;
                                        }

                                      return true;
                                    }

    bool                            ResizeRemove                          (XDWORD index)
                                    {
                                      // takes the index of the element to remove and copies over the memory to avoid allocations/deallocations
                                      if(index <= (nelements-1))
                                        {
                                          array[index].~T();

                                          if(index != (nelements-1))
                                            {
                                              // if we just take elements we are accessing one more memory location
                                              XDWORD movedelta = (nelements-1) - index;
                                              memmove(&array[index], &array[index+1], movedelta*sizeof(T));
                                            }

                                          return true;
                                        }

                                      return false;
                                    }

    bool                            ismulti;
    int                             addinlimit;
    XDWORD                          nelements;
    XDWORD                          narray;
    T*                              array;
};

#endif