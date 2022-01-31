//============================================================================
//                                  I B E X
// File        : pyibex_SepUnionBBox.cpp
// Author      : Benoit Desrochers
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : May 14, 2012
// Last Update : May 14, 2012
//============================================================================

#include "pyibex_SepUnionBbox.h"
#include <cassert>

using ibex::Sep;
using ibex::IntervalVector;


namespace pyibex {

SepUnionBbox::SepUnionBbox(const ibex::Array<Sep>& list, std::vector<IntervalVector>& bbox) :
  Sep(list[0].nb_var), list(list), bbox(bbox) {

}

void SepUnionBbox::separate(IntervalVector &x_in, IntervalVector &x_out){
  assert(x_in==x_out);
  IntervalVector x(x_in & x_out);
  IntervalVector result_x_out(IntervalVector::empty(x_out.size()));
  IntervalVector savebox(x_out);

    for (int i=0; i<list.size(); i++) {
      if (x.is_disjoint(bbox[i])) continue;
      if (i>0) x_out=savebox;
      x_out &= x_in;
      list[i].separate(x_in,x_out);
      result_x_out |= x_out;
    }
    x_out = result_x_out;

}

} // end namespace pyibex
