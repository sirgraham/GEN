/**-------------------------------------------------------------------------------------------------------------------
* @file     XAVLTree.h
*
* @class    XAVLTREE
* @brief    AVL Tree class (https://en.wikipedia.org/wiki/AVL_tree)
* @ingroup  UTILS
*
* @author   Imanol Celaya Ruiz de Alegria
* @date     13/05/2018 13:24:29
*
* Copyright(c) 2005 - 2018 GEN Group.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
* documentation files(the "Software"), to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense, and/ or sell copies of the Software,
* and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of
* the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
* THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*---------------------------------------------------------------------------------------------------------------------*/

#ifndef _XAVLTREE_H_
#define _XAVLTREE_H_

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XBase.h"
#include "XList.h"

#include "XMemory.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

template <class T>
struct XLESS
{
  public:

    bool                                  operator()                                    (const T& a, const T& b)
                                          {
                                            return a < b;
                                          }

    bool                                  operator()                                    (T& a, T& b)
                                          {
                                            return a < b;
                                          }

};


template <class K>
class XAVLNODE
{
  public:
                                          XAVLNODE                                      (const K& k)
                                          {
                                            Clean();

                                            key     = k;
                                          }

                                          XAVLNODE                                      (XAVLNODE<K> & rhs)
                                          {
                                            Clean();
                                            *this = rhs;
                                          }

    virtual                              ~XAVLNODE                                      ()
                                          {
                                            Clean();
                                          }

    XAVLNODE<K> &                         operator=(XAVLNODE<K>& rhs)
                                          {
                                            key = rhs.key;
                                            height = rhs.height;
                                            left = rhs.left;
                                            right = rhs.right;

                                            return *this;
                                          }


    K                                     key;
    int                                   height; // not to be recursive
    int                                   balance;

    XAVLNODE*                             left;
    XAVLNODE*                             right;
    XAVLNODE*                             parent;


  private:

    void                                  Clean                                         ()
                                          {
                                            parent  = NULL;
                                            left    = NULL;
                                            right   = NULL;
                                            balance = 0;
                                            height  = 0;
                                          }
};


template <class K, class Comparator = XLESS<K> >
class XAVLTREE
{
  public:
    class XITERATOR;


                                          XAVLTREE                                      (bool multi = true)
                                          {
                                            Clean();
                                            ismulti = multi;
                                          }

                                          XAVLTREE                                      (XAVLTREE<K, Comparator>& rhs)
                                          {
                                            *this = rhs;
                                          }

    XAVLTREE<K, Comparator>&              operator=                                     (XAVLTREE<K, Comparator>& rhs)
                                          {
                                            this->Copy(rhs.GetRoot());
                                            this->ismulti = rhs.ismulti;
                                            return *this;
                                          }

    virtual                              ~XAVLTREE                                      ()
                                          {
                                            //delete root;
                                            DeleteNodes(root);

                                            // need to make function to recursively delete the tree

                                            Clean();
                                          }

    XAVLNODE<K>*                          GetRoot                                       ()                                            { return root; }


    bool                                  Add                                           (const K& key)
                                          {
                                            //root = Insert(root, key, element);
                                            //FixHeight(root);
                                            Insert(root, key);
                                            return true;
                                          }

    XAVLNODE<K>*                          Insert                                        (XAVLNODE<K>* i, const K& key)
                                          {
                                            XAVLNODE<K>* node = i;
                                            XAVLNODE<K>* parent = NULL;
                                            XAVLNODE<K>* q = NULL;
                                            // find place to insert
                                            bool issmaller = false;
                                            while(node)
                                              {
                                                if((issmaller = islessimp(key, node->key)))
                                                  {
                                                    parent = node;
                                                    node = node->left;
                                                  }
                                                else
                                                  {
                                                    if(!ismulti)
                                                      {
                                                        // if this is the case, it's the same node, and we are not multi
                                                        if(!islessimp(node->key, key))
                                                          {
                                                            return NULL;
                                                          }
                                                      }

                                                    parent = node;
                                                    node = node->right;
                                                  }

                                                if(parent->balance)
                                                  {
                                                    q = parent;
                                                  }
                                              }

                                            XAVLNODE<K>* add = node = new XAVLNODE<K>(key);
                                            nelements++;
                                            if(!node)
                                              {
                                                return NULL;
                                              }

                                            if(!(node->parent = parent))
                                              {
                                                root = node;
                                              }
                                            else
                                              {
                                                if(issmaller)
                                                  {
                                                    parent->left = node;
                                                  }
                                                else
                                                  {
                                                    parent->right = node;
                                                  }

                                                while(parent != q)
                                                  {
                                                    parent->balance = (parent->right == node) * 2 - 1;
                                                    node = parent;
                                                    parent = node->parent;
                                                  }

                                                if(q)
                                                  {
                                                    if(q->left == node)
                                                      {
                                                        if(--q->balance == -2)
                                                          {
                                                            if(q->left->balance > 0)
                                                              {
                                                                RotateLeft(q->left);
                                                              }
                                                            RotateRight(q);
                                                          }
                                                      }
                                                    else
                                                      {
                                                        if(++q->balance == 2)
                                                          {
                                                            if(q->right->balance < 0)
                                                              {
                                                                RotateRight(q->right);
                                                              }
                                                            RotateLeft(q);
                                                          }
                                                      }
                                                  }
                                              }

                                            return NULL;
                                          }

    K                                     Get                                           (const K& key)
                                          {
                                            if(root != NULL)
                                              {
                                                if((!islessimp(root->key, key)) && (!islessimp(key, root->key)))
                                                  {
                                                    //return E();
                                                    //return root->elements.GetFirst();
                                                    return root->key;
                                                  }

                                                XAVLNODE<K>* n = (root->key < key) ? root->right : root->left;

                                                while(n != NULL)
                                                  {
                                                    // need to check this, avoid as many comparisons as possible
                                                    if((!islessimp(n->key, key)) && (!islessimp(key, n->key)))
                                                      {
                                                        //return E();
                                                        //return n->elements.GetFirst();
                                                        return n->key;
                                                      }

                                                    n = (islessimp(n->key, key)) ? n->right : n->left;
                                                  }
                                              }

                                            return K(); // WILL have to check this
                                          }

    XLIST<K>                              GetMultiple                                   (const K& key)
                                          {
                                            XLIST<K> list;

                                            GetMultiple(key, list);

                                            return list;
                                          }

    void                                  GetMultiple                                   (const K& key, XLIST<K>& list)
                                          {
                                            FillList(root, key, &list);
                                          }

    int                                   Find                                          (const K& key)
                                          {
                                            XDWORD index = 0;
                                            if(root != NULL)
                                              {
                                                if((!islessimp(root->key, key)) && (!islessimp(key, root->key)))
                                                  {
                                                    return index;
                                                  }

                                                XAVLNODE<K>* n = (islessimp(root->key, key)) ? root->right : root->left;
                                                index++;

                                                while(n != NULL)
                                                  {
                                                    // need to check this, avoid as many comparisons as possible
                                                    if((!islessimp(n->key, key)) && (!islessimp(key, n->key)))
                                                      {
                                                        return index;
                                                      }

                                                    n = (islessimp(n->key, key)) ? n->right : n->left;
                                                    index++;
                                                  }
                                              }

                                            return NOTFOUND; // WILL have to check this
                                          }



    bool                                  Delete                                        (const K& key)
                                          {
                                            if(root == NULL)
                                              {
                                                return false;
                                              }

                                            root = remove(root, key);

                                            return true;
                                          }

    bool                                  Delete(XITERATOR it)
                                          {
                                            remove(it.current);
                                            return false;
                                          }

    typename XAVLTREE::XITERATOR          Begin                                         ()
                                          {
                                            typename XAVLTREE::XITERATOR it;

                                            it.current = root;

                                            if(root)
                                              {
                                                while(it.current->left != NULL)
                                                {
                                                  it.current->left->parent = it.current;
                                                  it.current = it.current->left;
                                                }
                                              }

                                            return it;
                                          }

    typename XAVLTREE::XITERATOR          End()
                                          {
                                            typename XAVLTREE::XITERATOR it;

                                            it.current = NULL;

                                            return it;
                                          }

      XDWORD                              GetSize                                       ()
                                          {
                                            return nelements;
                                          }

/*--- XAVLTREE ITERATOR ---------------------------------------------------------------------------------------------------------*/


class XITERATOR
{
  public:

    friend class XAVLTREE;

                                          XITERATOR                                     ()
                                          {
                                          }

                                          XITERATOR                                     (typename XAVLTREE::XITERATOR& rhs)
                                          {
                                            *this = rhs;
                                          }

    typename XAVLTREE::XITERATOR &        operator=                                     (const typename XAVLTREE::XITERATOR& rhs)
                                          {
                                            current = rhs.current;
                                            return *this;
                                          }

    typename XAVLTREE::XITERATOR          operator++                                    ()
                                          {
                                            if(current->right != NULL)
                                              {
                                                current = current->right;

                                                while(current->left != NULL)
                                                  {
                                                    current = current->left;
                                                  }
                                              }
                                            else
                                              {
                                                XAVLNODE<K>* y = current->parent;

                                                // special root case
                                                if(y == NULL)
                                                  {
                                                    current = NULL;
                                                    return *this;
                                                  }
                                                else
                                                  {
                                                    while(current == y->right)
                                                      {
                                                        current = y;
                                                        y = y->parent;

                                                        if(y == NULL)
                                                          {
                                                            current = NULL;
                                                            return *this;
                                                          }
                                                      }
                                                  }

                                                if(current->right != y)
                                                  {
                                                    current = y;
                                                  }
                                              }

                                            return *this;
                                          }

    typename XAVLTREE::XITERATOR          operator++                                    (int) // postfix
                                          {
                                            typename XAVLTREE::XITERATOR it = *this;
                                            ++(*this);

                                            return it;
                                          }

    bool                                  operator==                                    (const typename XAVLTREE::XITERATOR& rhs)
                                          {
                                            return (current == rhs.current);
                                          }

    bool                                  operator!=                                    (const typename XAVLTREE::XITERATOR& rhs)
                                          {
                                            return !(*this == rhs);
                                          }

    K                                     operator*                                     ()
                                          {
                                            if(!current) return K();
                                            return current->key;
                                            //return current->elements.GetFirst();
                                          }

    K                                     GetKey()
                                          {
                                            if(!current) return K();
                                            return current->key;
                                          }

  private:

    XAVLNODE<K>*                          current;
    XAVLNODE<K>*                          root;
};


/*--- XAVLTREE ITERATOR ---------------------------------------------------------------------------------------------------------*/

  protected:

    void                                  FillList                                      (XAVLNODE<K>* n, const K& key, XLIST<K>* list)
                                          {
                                            // finds all ocurrences of nodes in the tree taking advantage of rotations being invariant
                                            if(n == NULL)
                                              {
                                                return;
                                              }

                                            // in order visit
                                            // visit left
                                            FillList(n->left, key, list);

                                            // visit me
                                            //if I'm bigger return and don't continue
                                            if(islessimp(key, n->key))
                                              {
                                                return;
                                              }
                                            else if(!islessimp(n->key, key))
                                              {
                                                // neither bigger or smaller, must be equal
                                                list->Add(n->key);
                                              }

                                            // visit right
                                            FillList(n->right, key, list);
                                          }

    XAVLNODE<K>*                          Remove                                        (XAVLNODE<K>* p)
                                          {
                                            if(p->left == NULL && p->right == NULL)
                                              {
                                                if(p->parent->left == p)
                                                  {
                                                    p->parent->left = NULL;
                                                    delete p;
                                                  }
                                                else if(p->parent->right == p)
                                                  {
                                                    p->parent->right == NULL;
                                                    delete p;
                                                  }
                                              }



                                            return NULL;
                                          }

    XAVLNODE<K>*                          Remove                                        (XAVLNODE<K>* p, const K& key)
                                          {
                                            return NULL;
                                          }

    XAVLNODE<K>*                          FindMin                                       (XAVLNODE<K>* p)
                                          {
                                            return (p->left) ? FindMin(p->left) : p;
                                          }

    XAVLNODE<K>*                          RemoveMin                                     (XAVLNODE<K>* p)
                                          {
                                            if(!p->left)
                                              {
                                                return p->right;
                                              }

                                            p->left = RemoveMin(p->left);
                                            return Rebalance(p);
                                          }


    bool                                  RotateLeft                                    (XAVLNODE<K>* n)
                                          {
                                            XAVLNODE<K>* rlink = n->right;
                                            ImpRotateLeft(n);
                                            bool hc = rlink->balance != 0;
                                            n->balance      -= 1 + __MAX(rlink->balance, 0);
                                            rlink->balance  -= 1 - __MIN(n->balance, 0);
                                            return hc;
                                          }

    bool                                  RotateRight                                   (XAVLNODE<K>* n)
                                          {
                                            XAVLNODE<K>* llink = n->left;
                                            ImpRotateRight(n);
                                            bool hc = llink->balance != 0;
                                            n->balance      += 1 - __MIN(llink->balance, 0);
                                            llink->balance  += 1 + __MAX(n->balance, 0);
                                            return hc;
                                          }

    void                                  ImpRotateLeft                                 (XAVLNODE<K>* n)
                                          {
                                            XAVLNODE<K>* nr = n->right;
                                            if((n->right = nr->left) != NULL)
                                              {
                                                n->right->parent = n;
                                              }

                                            nr->left = n;

                                            XAVLNODE<K>* p = n->parent;
                                            n->parent = nr;
                                            if((nr->parent = p) != NULL)
                                              {
                                                if(p->left == n)
                                                  {
                                                    p->left = nr;
                                                  }
                                                else
                                                  {
                                                    p->right = nr;
                                                  }
                                              }
                                            else
                                              {
                                                root = nr;
                                              }
                                          }

    void                                  ImpRotateRight                                (XAVLNODE<K>* n)
                                          {
                                            XAVLNODE<K>* nl = n->left;

                                            if((n->left = nl->right) != NULL)
                                              {
                                                n->left->parent = n;
                                              }
                                            nl->right = n;

                                            XAVLNODE<K>* p = n->parent;
                                            n->parent = nl;
                                            if((nl->parent = p) != NULL)
                                              {
                                                if(p->left == n)
                                                  {
                                                    p->left = nl;
                                                  }
                                                else
                                                  {
                                                    p->right = nl;
                                                  }
                                              }
                                            else
                                              {
                                                root = nl;
                                              }
                                          }



    int                                   Height                                        (XAVLNODE<K>* n)
                                          {
                                            if(n == NULL)
                                              {
                                                return -1;
                                                //return 0;
                                              }

                                            return n->height;
                                          }

    void                                  FixHeight                                     (XAVLNODE<K>* n)
                                          {
                                            int hl = Height(n->left);
                                            int hr = Height(n->right);

                                            //n->height = ((hl < hr) ? hr : hl) + 1;
                                            n->height = __MAX(hl, hr) + 1;
                                          }

    void                                  Copy(XAVLNODE<K>*node)
                                          {
                                            if(!node)
                                              {
                                                return;
                                              }

                                            // InOrder traversal
                                            this->Copy(node->left);
                                            this->Add(node->element);
                                            this->Copy(node->right);
                                          }


    int                                   SetBalance                                    (XAVLNODE<K>* n)
                                          {
                                            return Height(n->right) - Height(n->left);
                                          }

    void                                  DeleteNodes                                   (XAVLNODE<K>* n)
                                          {
                                            if(!n)
                                            {
                                              return;
                                            }

                                            DeleteNodes(n->left);
                                            DeleteNodes(n->right);

                                            delete n;
                                          }

    XAVLNODE<K>*                          root;
    Comparator                            islessimp;

    bool                                  ismulti;

    XDWORD                                nelements;

  private:

    void                                  Clean                                         ()
                                          {
                                            root        = NULL;
                                            ismulti     = false;
                                            nelements   = 0;
                                          }
};



/*---- INLINE FUNCTIONS ----------------------------------------------------------------------------------------------*/

#endif



