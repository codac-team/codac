/**
 * \file codac2_PosetMax.h   Managing a set of maximal elements in a poset
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Damien Massé
 *  \copyright  Copyright 2025
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#pragma once

#include <utility>
#include <bitset>
#include <cmath>
#include <set>

#include "codac2_Index.h"

namespace codac2 {
   /* the poset is an (ordered) vector of Index */
   /* used in .h because of its templated version */

/*** elements ****************************************************/
/*** we may store more that set of elems to speed up computation */

/** \brief Pair of an element of type T and an (ordered) vector of
 *  Index, along with the partial (subset) order on the vector */
template <typename T>
struct PosetElem {
   T a;
   std::vector<Index> elems;

   /** \brief empty element */
   PosetElem() {}

   /** \brief Element with empty list
    *  \param a key of type T */
   PosetElem(const T &a) : a(a) {
   }

   /** \brief Construct an element (a,e1)
    *  \param a key
    *  \param e1 set of indices (supposed to be ordered) */
   PosetElem(const T &a, const std::vector<Index> &e1) : a(a), elems(e1) {
   }

   /** \brief build an element (a, e1 inter e2)
    *  \param a key
    *  \param e1 ordered set
    *  \param e2 ordered set */
   PosetElem(const T &a, const std::vector<Index> &e1, const std::vector<Index> &e2) : a(a) {
       Index i=0,j=0;
       while(i<(Index)e1.size() && j<(Index)e2.size()) {
         if (e1[i]==e2[j]) { 
           elems.push_back(e1[i]);
           i++; j++;
         } else if (e1[i]<e2[j]) i++;
         else j++;
       }
   }

   /** \brief build an element (a, e1 inter e2 minus rejectF)
    *  \param a key
    *  \param e1 ordered set
    *  \param e2 ordered set
    *  \param rejectF element to remove from e1 inter e2
    */
   PosetElem(const T &a, const std::vector<Index> &e1, const std::vector<Index> &e2, const Index rejectF) : a(a) {
       Index i=0,j=0;
       while(i<(Index)e1.size() && j<(Index)e2.size()) {
         if (e1[i]==e2[j]) { 
           if (e1[i]!=rejectF) 
             elems.push_back(e1[i]);
           i++; j++;
         } else if (e1[i]<e2[j]) i++;
         else j++;
       }
   }

   /** \brief swap two elements
    *  \param p2 the element to be swapped with (*this)
    */
   void swap(PosetElem &p2) {
      std::swap(this->a,p2.a);
      this->elems.swap(p2.elems);
   }
 
   /** \brief comparison between the set of indices
    *  \param pm the element to be compared
    *  \return -1 if (*this).elems is included in pm.elems
    *           0 if both sets are equal
    *           1 if (*this).elems is a superset of pm.elems
    *           2 if both sets are not comparable
    */
   int comp(const PosetElem &pm) const { 
                /* -1 if elems included in pm.elems; 0 if ==,
                   +1 elems includes pm.elems, +2 if not comparable */
       const std::vector<Index> &e1=elems;
       const std::vector<Index> &e2=pm.elems;
       std::vector<long int>::size_type i=0,j=0;
       bool leq=(e1.size()<=e2.size()), geq=(e1.size()>=e2.size()); 
       bool same_length = (e1.size()==e2.size());
                        /* leq=F,geq=T => +1
                           leq=F,geq=F => 2
                           leq=T,geq=F => -1
                           leq=T,geq=T => 0 */
       while(i<e1.size() && j<e2.size()) {
         if (e1[i]<e2[j]) { 
            if (!geq || same_length) return 2;
            leq=false; i++;
         } else if (e1[i]>e2[j]) {
            if (!leq || same_length) return 2;
            geq=false; j++;
         } else { i++; j++; }
       }
       if (i<e1.size()) {
          if (!geq) return 2;
          leq=false;
       } 
       if (j<e2.size() || !geq) {
          if (!leq) return 2;
          return -1;
       } 
       return (leq ? 0 : 1);
   }
};

/** \brief stucture to handle sets of maximal elements */
template <typename T>
struct PosetMaxSet : std::vector<PosetElem<T>> {

   /** \brief build and add a new element
    *  \param a key
    *  \param elems the set of indices (ordered)
    *  \return true if the element is added, false otherwise
    */
   bool addElement(const T &a, std::vector<Index> &elems) {
       PosetElem e(a);
       std::swap(e.elems,elems);
       return this->addElement(std::move(e));
   }

   /** \brief move in a new element (if maximal)
    *  \param elem the element 
    *  \return true if the element is added, false otherwise
    */
   bool addElement(PosetElem<T> &&elem) {
      bool ismax=true;
      unsigned long int k=0,l=this->size();
      while (k<l) {
          int u=elem.comp((*this)[k]);
          if (u==2) { 
             k++;
             continue;
          }
          if (u==-1 || u==0) { ismax=false; break;  }
          if (u==1) {
             l--;
             if (l==k) break;
             std::swap((*this)[k],(*this)[l]);
          }
      }
      if (ismax) {
         if (l==this->size()) {
             this->push_back(std::move(elem));
         } else {
             (*this)[l] = std::move(elem);
             this->resize(l+1);
         }
      } else if (l!=this->size()) { /* should not happen */
         this->resize(l);
      }
      return ismax;
   }
};

}
