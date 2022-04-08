//============================================================================
//                                  I B E X
// File        : pyibex_SepUnionBBox.cpp
// Author      : Benoit Desrochers
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 14, 2012
// Last Update : May 14, 2012
//============================================================================

#include "codac_CtcHull.h"
#include <cassert>
#include <list>
// #include "vibes.h"

using ibex::Sep;
using ibex::IntervalVector;
using std::list;

namespace codac {

CtcHull::CtcHull(Sep& sep, double eps, Bsc& bsc) :
  Ctc(sep.nb_var), sep(sep), eps(eps), bsc(bsc) {

}

void CtcHull::contract(IntervalVector &box){
  // vibes::beginDrawing();
  IntervalVector x_inside = IntervalVector::empty(box.size());
  list<IntervalVector> lst = {box};
  while (lst.size() > 0){
    IntervalVector X(lst.front()); lst.pop_front();
    if (X.is_subset(x_inside))
      continue;

    IntervalVector xin(X), xout(X);
    sep.separate(xin, xout);
    if (xin.is_empty() || xin != X){
      IntervalVector *res;
      int n = X.diff(xin, res);
      for (int i = 0; i < n; i++){
        x_inside |= res[i];
      }
      delete[] res;
    }
    X &= (xin & xout);
    if (!X.is_empty() && X.max_diam() > eps ){
        std::pair<IntervalVector, IntervalVector> boxes = bsc.bisect(X);
        lst.push_back(boxes.first);
        lst.push_back(boxes.second);
    } else if (!X.is_empty() && X.max_diam() <= eps){
      x_inside |= X;
    }
  }

  box &= x_inside;
}

} // end namespace pyibex
