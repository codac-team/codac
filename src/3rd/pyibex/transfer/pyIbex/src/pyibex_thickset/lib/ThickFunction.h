//============================================================================
//                               P Y I B E X
// File        : ThickFunction.h
// Author      : Benoit Desrochers
// Copyright   : Benoit Desrochers
// License     : See the LICENSE file
// Created     : Aug  31, 2016
//============================================================================

#ifndef __THICKFUNCTION__
#define __THICKFUNCTION__

#include <ThickInterval.h>
#include <ibex_Function.h>
using ibex::IntervalVector;

using RawFunc = std::function<IntervalVector (const IntervalVector& )>;

class ThickFunction {
public:
  // ThickFunction(Function &f_lb, Function& f_ub):
  //     f_lb(&f_lb), f_ub(&f_ub), has_function(true)
  // {
  //
  // };
  ThickFunction(RawFunc& f_lb, RawFunc& f_ub):
    f_lb(f_lb), f_ub(f_ub)
  {}

  ThickBox eval(const IntervalVector& box){
    return ThickBox(f_lb(box), f_ub(box));
  }
// bool has_function;
private:
  RawFunc f_lb, f_ub;
  // Function *f_lb, *f_ub;

};


#endif // __THICKFUNCTION__
