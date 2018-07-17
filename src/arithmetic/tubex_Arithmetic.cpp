/* ============================================================================
 *  tubex-lib - Arithmetic
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
#include "ibex_IntervalVector.h"
#include "tubex_Arithmetic.h"
#include "tubex_DimensionException.h"
#include "tubex_DomainException.h"
#include "tubex_SamplingException.h"
#include "tubex_TubeSlice.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  #define unary_op(f) \
    \
    const Tube f(const Tube& x) \
    { \
      Tube result(x); \
      TubeSlice *slice_result = result.get_first_slice(); \
      const TubeSlice *slice_x = x.get_first_slice(); \
      while(slice_result != NULL) \
      { \
        slice_result->set_envelope(IntervalVector(1, ibex::f(slice_x->codomain()[0]))); \
        slice_result->set_input_gate(IntervalVector(1, ibex::f(slice_x->input_gate()[0]))); \
        slice_result->set_output_gate(IntervalVector(1, ibex::f(slice_x->output_gate()[0]))); \
        slice_result = slice_result->next_slice(); \
        slice_x = slice_x->next_slice(); \
      } \
      return result; \
    }

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

  const TubeVector operator+(const TubeVector& x)
  {
    return x;
  }

  const TubeVector operator-(const TubeVector& x)
  {
    TubeVector result(x);
    TubeSlice *slice_result = result.get_first_slice();
    const TubeSlice *slice_x = x.get_first_slice();
    while(slice_result != NULL)
    {
      slice_result->set_envelope(-slice_x->codomain());
      slice_result->set_input_gate(-slice_x->input_gate());
      slice_result->set_output_gate(-slice_x->output_gate());
      slice_result = slice_result->next_slice();
      slice_x = slice_x->next_slice();
    }
    return result;
  }

  #define unary_param_op(f, p) \
    \
    const Tube f(const Tube& x, p param) \
    { \
      Tube result(x); \
      TubeSlice *slice_result = result.get_first_slice(); \
      const TubeSlice *slice_x = x.get_first_slice(); \
      while(slice_result != NULL) \
      { \
        slice_result->set_envelope(IntervalVector(1, ibex::f(slice_x->codomain()[0], param))); \
        slice_result->set_input_gate(IntervalVector(1, ibex::f(slice_x->input_gate()[0], param))); \
        slice_result->set_output_gate(IntervalVector(1, ibex::f(slice_x->output_gate()[0], param))); \
        slice_result = slice_result->next_slice(); \
        slice_x = slice_x->next_slice(); \
      } \
      return result; \
    }

  unary_param_op(pow, int);
  unary_param_op(pow, double);
  unary_param_op(pow, const Interval&);
  unary_param_op(root, int);
  
  #define binary_op_scalar_commutative_types(f) \
    \
    const Tube f(const Tube& x1, const Tube& x2) \
    { \
      SamplingException::check(x1, x2); \
      DomainException::check(x1, x2); \
      Tube result(x1); \
      TubeSlice *slice = result.get_first_slice(); \
      const TubeSlice *slice_x1 = x1.get_first_slice(); \
      const TubeSlice *slice_x2 = x2.get_first_slice(); \
      while(slice != NULL) \
      { \
        slice->set_envelope(IntervalVector(1, f(slice_x1->codomain()[0], slice_x2->codomain()[0]))); \
        slice->set_input_gate(IntervalVector(1, f(slice_x1->input_gate()[0], slice_x2->input_gate()[0]))); \
        slice->set_output_gate(IntervalVector(1, f(slice_x1->output_gate()[0], slice_x2->output_gate()[0]))); \
        slice = slice->next_slice(); \
        slice_x1 = slice_x1->next_slice(); \
        slice_x2 = slice_x2->next_slice(); \
      } \
      return result; \
    } \
    \
    const Tube f(const Tube& x1, const Trajectory& x2) \
    { \
      DomainException::check(x1, x2); \
      Tube result(x1); \
      TubeSlice *slice = result.get_first_slice(); \
      const TubeSlice *slice_x1 = x1.get_first_slice(); \
      while(slice != NULL) \
      { \
        slice->set_envelope(IntervalVector(1, f(slice_x1->codomain()[0], Interval(x2[slice->domain()])))); \
        slice->set_input_gate(IntervalVector(1, f(slice_x1->input_gate()[0], Interval(x2[slice->domain().lb()])))); \
        slice->set_output_gate(IntervalVector(1, f(slice_x1->output_gate()[0], Interval(x2[slice->domain().ub()])))); \
        slice = slice->next_slice(); \
        slice_x1 = slice_x1->next_slice(); \
      } \
      return result; \
    } \
    \
    const Tube f(const Trajectory& x1, const Tube& x2) \
    { \
      DomainException::check(x2, x1); \
      Tube result(x2); \
      TubeSlice *slice = result.get_first_slice(); \
      const TubeSlice *slice_x2 = x2.get_first_slice(); \
      while(slice != NULL) \
      { \
        slice->set_envelope(IntervalVector(1, f(Interval(x1[slice->domain()]), slice_x2->codomain()[0]))); \
        slice->set_input_gate(IntervalVector(1, f(Interval(x1[slice->domain().lb()]), slice_x2->input_gate()[0]))); \
        slice->set_output_gate(IntervalVector(1, f(Interval(x1[slice->domain().ub()]), slice_x2->output_gate()[0]))); \
        slice = slice->next_slice(); \
        slice_x2 = slice_x2->next_slice(); \
      } \
      return result; \
    } \
    \
    const Tube f(const Tube& x1, const Interval& x2) \
    { \
      Tube result(x1); \
      TubeSlice *slice = result.get_first_slice(); \
      const TubeSlice *slice_x1 = x1.get_first_slice(); \
      while(slice != NULL) \
      { \
        slice->set_envelope(IntervalVector(1, f(slice_x1->codomain()[0], x2))); \
        slice->set_input_gate(IntervalVector(1, f(slice_x1->input_gate()[0], x2))); \
        slice->set_output_gate(IntervalVector(1, f(slice_x1->output_gate()[0], x2))); \
        slice = slice->next_slice(); \
        slice_x1 = slice_x1->next_slice(); \
      } \
      return result; \
    } \
    \
    const Tube f(const Interval& x1, const Tube& x2) \
    { \
      Tube result(x2); \
      TubeSlice *slice = result.get_first_slice(); \
      const TubeSlice *slice_x2 = x2.get_first_slice(); \
      while(slice != NULL) \
      { \
        slice->set_envelope(IntervalVector(1, f(x1, slice_x2->codomain()[0]))); \
        slice->set_input_gate(IntervalVector(1, f(x1, slice_x2->input_gate()[0]))); \
        slice->set_output_gate(IntervalVector(1, f(x1, slice_x2->output_gate()[0]))); \
        slice = slice->next_slice(); \
        slice_x2 = slice_x2->next_slice(); \
      } \
      return result; \
    }

  binary_op_scalar_commutative_types(operator+);
  binary_op_scalar_commutative_types(operator-);
  binary_op_scalar_commutative_types(operator*);
  binary_op_scalar_commutative_types(operator|);
  binary_op_scalar_commutative_types(operator&);
  binary_op_scalar_commutative_types(atan2);
  
  #define binary_op_scalar_noncommutative_types(f) \
    \
    const Tube f(const Tube& x1, const Tube& x2) \
    { \
      SamplingException::check(x1, x2); \
      DomainException::check(x1, x2); \
      Tube result(x1); \
      TubeSlice *slice = result.get_first_slice(); \
      const TubeSlice *slice_x1 = x1.get_first_slice(); \
      const TubeSlice *slice_x2 = x2.get_first_slice(); \
      while(slice != NULL) \
      { \
        slice->set_envelope(IntervalVector(1, f(slice_x1->codomain()[0], slice_x2->codomain()[0]))); \
        slice->set_input_gate(IntervalVector(1, f(slice_x1->input_gate()[0], slice_x2->input_gate()[0]))); \
        slice->set_output_gate(IntervalVector(1, f(slice_x1->output_gate()[0], slice_x2->output_gate()[0]))); \
        slice = slice->next_slice(); \
        slice_x1 = slice_x1->next_slice(); \
        slice_x2 = slice_x2->next_slice(); \
      } \
      return result; \
    } \
    \
    const Tube f(const Tube& x1, const Interval& x2) \
    { \
      Tube result(x1); \
      TubeSlice *slice = result.get_first_slice(); \
      const TubeSlice *slice_x1 = x1.get_first_slice(); \
      while(slice != NULL) \
      { \
        slice->set_envelope(IntervalVector(1, f(slice_x1->codomain()[0], x2))); \
        slice->set_input_gate(IntervalVector(1, f(slice_x1->input_gate()[0], x2))); \
        slice->set_output_gate(IntervalVector(1, f(slice_x1->output_gate()[0], x2))); \
        slice = slice->next_slice(); \
        slice_x1 = slice_x1->next_slice(); \
      } \
      return result; \
    } \
    \
    const Tube f(const Tube& x1, const Trajectory& x2) \
    { \
      DomainException::check(x1, x2); \
      Tube result(x1); \
      TubeSlice *slice = result.get_first_slice(); \
      const TubeSlice *slice_x1 = x1.get_first_slice(); \
      while(slice != NULL) \
      { \
        slice->set_envelope(IntervalVector(1, f(slice_x1->codomain()[0], Interval(x2[slice->domain()])))); \
        slice->set_input_gate(IntervalVector(1, f(slice_x1->input_gate()[0], Interval(x2[slice->domain().lb()])))); \
        slice->set_output_gate(IntervalVector(1, f(slice_x1->output_gate()[0], Interval(x2[slice->domain().ub()])))); \
        slice = slice->next_slice(); \
        slice_x1 = slice_x1->next_slice(); \
      } \
      return result; \
    }

  binary_op_scalar_commutative_types(operator/);

  #define binary_op(f) \
    \
    const TubeVector f(const TubeVector& x1, const TubeVector& x2) \
    { \
      DimensionException::check(x1, x2); \
      SamplingException::check(x1, x2); \
      DomainException::check(x1, x2); \
      TubeVector result(x1); \
      TubeSlice *slice = result.get_first_slice(); \
      const TubeSlice *slice_x1 = x1.get_first_slice(); \
      const TubeSlice *slice_x2 = x2.get_first_slice(); \
      while(slice != NULL) \
      { \
        slice->set_envelope(f(slice_x1->codomain(), slice_x2->codomain())); \
        slice->set_input_gate(f(slice_x1->input_gate(), slice_x2->input_gate())); \
        slice->set_output_gate(f(slice_x1->output_gate(), slice_x2->output_gate())); \
        slice = slice->next_slice(); \
        slice_x1 = slice_x1->next_slice(); \
        slice_x2 = slice_x2->next_slice(); \
      } \
      return result; \
    } \
    \
    const TubeVector f(const TubeVector& x1, const TrajectoryVector& x2) \
    { \
      DimensionException::check(x1, x2); \
      TubeVector result(x1); \
      TubeSlice *slice = result.get_first_slice(); \
      const TubeSlice *slice_x1 = x1.get_first_slice(); \
      while(slice != NULL) \
      { \
        slice->set_envelope(f(slice_x1->codomain(), x2[slice->domain()])); \
        slice->set_input_gate(f(slice_x1->input_gate(), x2[slice->domain().lb()])); \
        slice->set_output_gate(f(slice_x1->output_gate(), x2[slice->domain().ub()])); \
        slice = slice->next_slice(); \
        slice_x1 = slice_x1->next_slice(); \
      } \
      return result; \
    } \
    \
    const TubeVector f(const TrajectoryVector& x1, const TubeVector& x2) \
    { \
      DimensionException::check(x2, x1); \
      TubeVector result(x2); \
      TubeSlice *slice = result.get_first_slice(); \
      const TubeSlice *slice_x2 = x2.get_first_slice(); \
      while(slice != NULL) \
      { \
        slice->set_envelope(f(x1[slice->domain()], slice_x2->codomain())); \
        slice->set_input_gate(f(x1[slice->domain().lb()], slice_x2->input_gate())); \
        slice->set_output_gate(f(x1[slice->domain().ub()], slice_x2->output_gate())); \
        slice = slice->next_slice(); \
        slice_x2 = slice_x2->next_slice(); \
      } \
      return result; \
    } \
    \
    const TubeVector f(const TubeVector& x1, const IntervalVector& x2) \
    { \
      DimensionException::check(x1, x2); \
      TubeVector result(x1); \
      TubeSlice *slice = result.get_first_slice(); \
      const TubeSlice *slice_x1 = x1.get_first_slice(); \
      while(slice != NULL) \
      { \
        slice->set_envelope(f(slice_x1->codomain(), x2)); \
        slice->set_input_gate(f(slice_x1->input_gate(), x2)); \
        slice->set_output_gate(f(slice_x1->output_gate(), x2)); \
        slice = slice->next_slice(); \
        slice_x1 = slice_x1->next_slice(); \
      } \
      return result; \
    } \
    \
    const TubeVector f(const IntervalVector& x1, const TubeVector& x2) \
    { \
      DimensionException::check(x2, x1); \
      TubeVector result(x2); \
      TubeSlice *slice = result.get_first_slice(); \
      const TubeSlice *slice_x2 = x2.get_first_slice(); \
      while(slice != NULL) \
      { \
        slice->set_envelope(f(x1, slice_x2->codomain())); \
        slice->set_input_gate(f(x1, slice_x2->input_gate())); \
        slice->set_output_gate(f(x1, slice_x2->output_gate())); \
        slice = slice->next_slice(); \
        slice_x2 = slice_x2->next_slice(); \
      } \
      return result; \
    }

  binary_op(operator+);
  binary_op(operator-);

  #define binary_op_with_scalar_1(f) \
    \
    const TubeVector f(const Tube& x1, const TubeVector& x2) \
    { \
      SamplingException::check(x1, x2); \
      DomainException::check(x1, x2); \
      TubeVector result(x2); \
      TubeSlice *slice = result.get_first_slice(); \
      const TubeSlice *slice_x1 = x1.get_first_slice(); \
      const TubeSlice *slice_x2 = x2.get_first_slice(); \
      while(slice != NULL) \
      { \
        slice->set_envelope(ibex::f(slice_x1->codomain()[0], slice_x2->codomain())); \
        slice->set_input_gate(ibex::f(slice_x1->input_gate()[0], slice_x2->input_gate())); \
        slice->set_output_gate(ibex::f(slice_x1->output_gate()[0], slice_x2->output_gate())); \
        slice = slice->next_slice(); \
        slice_x1 = slice_x1->next_slice(); \
        slice_x2 = slice_x2->next_slice(); \
      } \
      return result; \
    } \
    \
    const TubeVector f(const Trajectory& x1, const TubeVector& x2) \
    { \
      DomainException::check(x2, x1); \
      TubeVector result(x2); \
      TubeSlice *slice = result.get_first_slice(); \
      const TubeSlice *slice_x2 = x2.get_first_slice(); \
      while(slice != NULL) \
      { \
        slice->set_envelope(ibex::f(Interval(x1[slice->domain()]), slice_x2->codomain())); \
        slice->set_input_gate(ibex::f(Interval(x1[slice->domain().lb()]), slice_x2->input_gate())); \
        slice->set_output_gate(ibex::f(Interval(x1[slice->domain().ub()]), slice_x2->output_gate())); \
        slice = slice->next_slice(); \
        slice_x2 = slice_x2->next_slice(); \
      } \
      return result; \
    } \
    \
    const TubeVector f(const Interval& x1, const TubeVector& x2) \
    { \
      TubeVector result(x2); \
      TubeSlice *slice = result.get_first_slice(); \
      const TubeSlice *slice_x2 = x2.get_first_slice(); \
      while(slice != NULL) \
      { \
        slice->set_envelope(ibex::f(x1, slice_x2->codomain())); \
        slice->set_input_gate(ibex::f(x1, slice_x2->input_gate())); \
        slice->set_output_gate(ibex::f(x1, slice_x2->output_gate())); \
        slice = slice->next_slice(); \
        slice_x2 = slice_x2->next_slice(); \
      } \
      return result; \
    }

  binary_op_with_scalar_1(operator*);
}