/*------------------------------------------------------------------------------------------
//  XPAIR.H
*/
/**
// \file
//
//  reusable XPAIR class
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 18/07/2016 10:08:17
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _XPAIR_H_
#define _XPAIR_H_


/*---- INCLUDES --------------------------------------------------------------------------*/


/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/
template <class L, class R>
class XPAIR
{
public:
                                                XPAIR                                     ()
                                                {
                                                  Clean();
                                                }


                                                XPAIR                                     (L left, R right)
                                                {
                                                  Clean();
                                                  this->left = left;
                                                  this->right = right;
                                                }

                                                XPAIR                                     (const XPAIR<L, R>& rhs)
                                                {
                                                  *this = rhs;
                                                }

  XPAIR &                                       operator=                                 (const XPAIR<L, R>& rhs)
                                                {
                                                  left = rhs.left;
                                                  right = rhs.right;
                                                  return *this;
                                                }

  bool                                          operator ==                               (const XPAIR<L, R>& rhs) const
                                                {
                                                  return (left == rhs.left) && (right == rhs.right);
                                                }

  L                                             left;
  R                                             right;

  private:

    void                                        Clean                                     ()
                                                {
                                                  left  = L();
                                                  right = R();
                                                }
};


template <class T1, class T2>
bool operator<(const XPAIR<T1, T2>& x, const XPAIR<T1, T2>& y)
{
  return x.left < y.left; // SGI stl did the same, not much sense comparing the right element
}


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

