/* ============================================================================
 *  tubex-lib - Tube arithmetic
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "ibex_Interval.h"
#include "tubex_operators.h"
#include "tubex_DomainException.h"
#include "tubex_StructureException.h"
#include "tubex_DimensionException.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  /*#include "tubex_operators_unary.cpp"

    unary_op(cos);
    unary_op(sin);
    unary_op(abs);
    unary_op(sqr);
    unary_op(sqrt);
    unary_op(exp);
    unary_op(log);
    unary_op(tan);
    unary_op(acos);
    unary_op(asin);
    unary_op(atan);
    unary_op(cosh);
    unary_op(sinh);
    unary_op(tanh);
    unary_op(acosh);
    unary_op(asinh);
    unary_op(atanh);
   
    Tube operator+(const Tube& x)
    {
      return x;
    }
    
    TubeSlice operator+(const TubeSlice& x)
    {
      return x;
    }
 
    Tube operator-(const Tube& x)
    {
      return IntervalVector(x.dim(), 0.) - x;
    }

    TubeSlice operator-(const TubeSlice& x)
    {
      return IntervalVector(x.dim(), 0.) - x;
    }

  #include "tubex_operators_unary_param.cpp"

    unary_param_op(pow, int);
    unary_param_op(pow, double);
    unary_param_op(pow, const Interval&);
    unary_param_op(root, int);

  #include "tubex_operators_binary.cpp"

    binary_op(atan2);
    binary_op(operator+);
    binary_op(operator-);
    binary_op(operator*);
    binary_op(operator/);
    binary_op(operator|);
    binary_op(operator&);*/

  #include "tubex_operators_assignment.cpp"

    //assignment_op(operator+=);
    //assignment_op(operator-=);
    //assignment_op(operator*=);
    //assignment_op(operator/=);
    assignment_op(operator|=);
    assignment_op(operator&=);
}