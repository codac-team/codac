//============================================================================
//                               P Y I B E X
// File        : SepPaving.h
// Author(s)   : Benoit Desrochers, Luc Jaulin
// Copyright   : Benoit Desrochers
// License     : LGPLv3, See the LICENSE file
// Created     : Sep  15, 2015
//============================================================================

#include <pyibex_SepPaving.h>
// #include <vibes.h>
#include "ibex_Pdc.h"
#include <functional>   // std::logical_and
#include <numeric>      // std::accumulate
#include <list>
#include <iostream>
#include <fstream>

using std::list;
using std::cerr;
using std::endl;
using ibex::BoolInterval;

//#include "vibes.h"

//IntervalVector(*F_sivia) (const IntervalVector&);  // Global variable used in SIVIA;
//SepPaving X_sivia;

namespace pyibex {

SepPaving::SepPaving(int dim) : Sep(dim), root(IntervalVector(dim)), bisector(LargestFirst(0, 0.5)) { } ;

SepPaving::SepPaving(const IntervalVector& p):
 	Sep(p.size()), root(p), bisector(LargestFirst(0, 0.45)) { }

SepPaving::SepPaving(const IntervalVector& p, const LargestFirst& bisector):
 	Sep(p.size()), root(p), bisector(bisector) { }

SepPaving::SepPaving(const SepPaving& p):
  Sep(2), root(p.root), bisector(p.bisector) {}

SepPaving::SepPaving(IntervalVector& p,Sep& sep,double eps, bool display):
  Sep(p.size()), root(p), bisector(LargestFirst(0, 0.45)){
    // if (display){
      // this->Sivia_visu(sep, eps);
    // }else {
      this->Sivia(sep, eps);
    // }
}


SepPaving::SepPaving(const std::string& filename) : Sep(1), bisector(LargestFirst(0, 0.5)), root(IntervalVector(1)){
  std::ifstream myfile;
  myfile.open(filename, std::ios::binary | std::ios::in);
  root = std::move(*Node::load(myfile));
  myfile.close();
}

void SepPaving::save(const std::string& filename){
  std::ofstream myfile;
  myfile.open(filename, std::ios::binary | std::ios::out);
  root.save(myfile);
  myfile.close();
}

//
// void SepPaving::ctcOutside(IntervalVector& box){
//   if ( !box.is_subset(root.getBox()) )
//     return;
//   IntervalVector res(IntervalVector::empty(box.size()));
//   std::list<Node*> L;
//   L.push_back(&root);
//   while (!L.empty()){
//     Node* n=L.front();    L.pop_front();
//     IntervalVector tmp = (n->getBox() & box);
//     if ( !tmp.is_empty() && !tmp.is_flat() ){
//       if (n->isLeaf()){
//         if (n->value() != OUT){
//           res = res | n->getBox();
//         }
//       } else {
//         L.push_back(n->left());
//         L.push_back(n->right());
//       }
//     }
//   }
//   box &= res;
// }
// //
// void SepPaving::ctcInside(IntervalVector& box){
//   if ( !box.is_subset(root.getBox()) )
//     return;
//   IntervalVector res(IntervalVector::empty(box.size()));
//   std::list<Node*> L;
//   L.push_back(&root);
//   while (!L.empty()){
//     Node* n=L.front();    L.pop_front();
//     IntervalVector tmp = (n->getBox() & box);
//     if ( !tmp.is_empty() && !tmp.is_flat() ){
//       if (n->isLeaf()){
//         if (n->value() != IN){
//           res = res | n->getBox();
//         }
//       } else {
//         L.push_back(n->left());
//         L.push_back(n->right());
//       }
//     }
//   }
//   box &= res;
// }




void SepPaving::visit(SepVisitor &visitor)
{
    visitor.pre_visit(*this);
    root.visit(visitor);
		visitor.post_visit(*this);
}

//----------------------------------------------------------------------
SepPaving& SepPaving::Reunite()
{
		root.reunite();
    return (*this);
}


void SepPaving::separate(IntervalVector& xin, IntervalVector& xout){
  IntervalVector X0(xin | xout);
  IntervalVector X(xin & xout);
  IntervalVector xborder(IntervalVector::empty(xin.size()));

  list<Node*> L= {&root};

  int k = 0, j = 0;
  // Get the boundary
  while (!L.empty()){
      Node* n=L.front();    L.pop_front();
      IntervalVector nX(n->m_box_in & n->m_box_out);
      if (n->isLeaf()) {
        xborder |= (X & nX);
      } else {
        IntervalVector Xleft  = n->left()->m_box_in & n->left()->m_box_out & X;
        IntervalVector Xright =  n->right()->m_box_in & n->right()->m_box_out & X;
        if (!Xleft.is_empty() && !Xleft.is_subset(xborder)){
          L.push_back(n->left());
        }
        if ( !Xright.is_empty() && !Xright.is_subset(xborder)){
          L.push_back(n->right());
        }
      }
  }

  // used Test
  xin &= xborder;
  xout &= xborder;
  IntervalVector *res;
  int ndiff = X0.diff(xborder, res);
  for(int  i = 0; i < ndiff; i++){
    BoolInterval isInside = root.isInside(res[i].mid());
    if (isInside  == ibex::YES){
      xout |= res[i];
    } else if (isInside == ibex::NO){
      xin |= res[i];
    } else if (isInside == ibex::EMPTY_BOOL){
      xin |= res[i];
    } else {
      xin |= res[i];
      xout |= res[i];
    }
  }
  delete[] res;
}


SepPaving& SepPaving::Sivia(Sep &sep, double eps)
{
    list<Node*> L= {&root};
    // L.push_back(&root);
    int k = 0, j = 0;
    while (!L.empty()){
        k++;
        Node* n=L.front();    L.pop_front();
        IntervalVector x0(n->m_box_in & n->m_box_out);
        sep.separate(n->m_box_in, n->m_box_out);
        IntervalVector x(n->m_box_in & n->m_box_out);
        if (!x.is_empty() && x.max_diam()>eps){
          j++;
          if(n->isLeaf()){
            // cerr << "bisect " << n->value() << endl;
            n->bisect(bisector);
          }
          L.push_back(n->left());
          L.push_back(n->right());
        }
    }
    std::cerr << "k = " << k  << "|| j = " << j << std::endl;
    return (*this);
}

void SepPaving::Sivia_ops(Sep &sep, double eps, bool intersection){
  list<Node*> L= {&root};
  // L.push_back(&root);
  int k = 0, j = 0;
  while (!L.empty()){
      k++;
      Node* n=L.front();    L.pop_front();
      IntervalVector X0(n->m_box_in | n->m_box_out);
      IntervalVector xin(X0);
      IntervalVector xout(X0);
      sep.separate(xin, xout);
      IntervalVector nxin_old(n->m_box_in);
      IntervalVector nxout_old(n->m_box_out);
      if (intersection == false){
        n->m_box_in &= xin;
        n->m_box_out |= xout;
      } else {
        n->m_box_in |= xin;
        n->m_box_out &= xout;
      }

      IntervalVector X(n->m_box_in & n->m_box_out);

      if ( !X.is_empty() && X.max_diam() > eps){
        if (n->isLeaf()){
          n->bisect(bisector);
        }else{
          n->bisect_max(bisector);
        }
        n->left()->m_box_out &= nxout_old;
        n->right()->m_box_out &= nxout_old;
        n->left()->m_box_in &= nxin_old;
        n->right()->m_box_in &= nxin_old;

        if (n->right()->m_box_out.is_flat()){
          n->right()->m_box_out.set_empty();
        }
        if( n->right()->m_box_in.is_flat()){
          n->right()->m_box_in.set_empty();
        }

        L.push_back(n->left());
        L.push_back(n->right());
      } else if (X.is_empty() || X.max_diam() < eps) {
        n->clear();
      }
  }
}

} // end namespace ibex
