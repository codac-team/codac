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

using namespace std;
using namespace ibex;

namespace tubex
{
  #define unary_op(f) \
    \
    const Tube f(const Tube& x1) \
    { \
      Tube x2(x1); \
      const TubeSlice *slice_x1, *first_slice_x1 = x1.get_first_slice(); \
      TubeSlice *slice_x2, *first_slice_x2 = x2.get_first_slice(); \
      slice_x1 = first_slice_x1; \
      slice_x2 = first_slice_x2; \
      while(slice_x1 != NULL) \
      { \
        IntervalVector box = slice_x1->codomain(); \
        for(int i = 0 ; i < box.size() ; i++) \
          box[i] = ibex::f(box[i]); \
        slice_x2->set_envelope(box); \
        slice_x1 = slice_x1->next_slice(); \
        slice_x2 = slice_x2->next_slice(); \
      } \
      slice_x1 = first_slice_x1; \
      slice_x2 = first_slice_x2; \
      while(slice_x1 != NULL) \
      { \
        if(slice_x1 == first_slice_x1) \
        { \
          IntervalVector box = slice_x1->input_gate(); \
          for(int i = 0 ; i < box.size() ; i++) \
            box[i] = ibex::f(box[i]); \
          slice_x2->set_input_gate(box); \
        } \
        IntervalVector box = slice_x1->output_gate(); \
        for(int i = 0 ; i < box.size() ; i++) \
          box[i] = ibex::f(box[i]); \
        slice_x2->set_output_gate(box); \
        slice_x1 = slice_x1->next_slice(); \
        slice_x2 = slice_x2->next_slice(); \
      } \
      return x2; \
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
    return 0. - x;
  }

  #define unary_param_op(f, p) \
    \
    const Tube f(const Tube& x1, p param) \
    { \
      Tube x2(x1); \
      const TubeSlice *slice_x1, *first_slice_x1 = x1.get_first_slice(); \
      TubeSlice *slice_x2, *first_slice_x2 = x2.get_first_slice(); \
      slice_x1 = first_slice_x1; \
      slice_x2 = first_slice_x2; \
      while(slice_x1 != NULL) \
      { \
        IntervalVector box = slice_x1->codomain(); \
        for(int i = 0 ; i < box.size() ; i++) \
          box[i] = ibex::f(box[i], param); \
        slice_x2->set_envelope(box); \
        slice_x1 = slice_x1->next_slice(); \
        slice_x2 = slice_x2->next_slice(); \
      } \
      slice_x1 = first_slice_x1; \
      slice_x2 = first_slice_x2; \
      while(slice_x1 != NULL) \
      { \
        if(slice_x1 == first_slice_x1) \
        { \
          IntervalVector box = slice_x1->input_gate(); \
          for(int i = 0 ; i < box.size() ; i++) \
          box[i] = ibex::f(box[i], param); \
          slice_x2->set_input_gate(box); \
        } \
        IntervalVector box = slice_x1->output_gate(); \
        for(int i = 0 ; i < box.size() ; i++) \
          box[i] = ibex::f(box[i], param); \
        slice_x2->set_output_gate(box); \
        slice_x1 = slice_x1->next_slice(); \
        slice_x2 = slice_x2->next_slice(); \
      } \
      return x2; \
    }

  unary_param_op(pow, int);
  unary_param_op(pow, double);
  unary_param_op(pow, const Interval&);
  unary_param_op(root, int);
  
  #define binary_op_scalar_commutative_types(f) \
    \
    const Tube f(const Tube& tube_x1, const Tube& tube_x2) \
    { \
      DomainException::check(tube_x1, tube_x2); \
      SamplingException::check(tube_x1, tube_x2); \
      Tube new_tube_x(tube_x1); \
      TubeSlice *slice_x, *first_slice_x = new_tube_x.get_first_slice(); \
      const TubeSlice *slice_x1, *first_slice_x1 = tube_x1.get_first_slice(); \
      const TubeSlice *slice_x2, *first_slice_x2 = tube_x2.get_first_slice(); \
      slice_x = first_slice_x; \
      slice_x1 = first_slice_x1; \
      slice_x2 = first_slice_x2; \
      while(slice_x1 != NULL) \
      { \
        slice_x->set_envelope(IntervalVector(1, ibex::f(slice_x1->codomain()[0], slice_x2->codomain()[0]))); \
        slice_x = slice_x->next_slice(); \
        slice_x1 = slice_x1->next_slice(); \
        slice_x2 = slice_x2->next_slice(); \
      } \
      slice_x = first_slice_x; \
      slice_x1 = first_slice_x1; \
      slice_x2 = first_slice_x2; \
      while(slice_x1 != NULL) \
      { \
        if(slice_x == first_slice_x) \
          slice_x->set_input_gate(IntervalVector(1, ibex::f(slice_x1->input_gate()[0], slice_x2->input_gate()[0]))); \
        slice_x->set_output_gate(IntervalVector(1, ibex::f(slice_x1->output_gate()[0], slice_x2->output_gate()[0]))); \
        slice_x = slice_x->next_slice(); \
        slice_x1 = slice_x1->next_slice(); \
        slice_x2 = slice_x2->next_slice(); \
      } \
      return new_tube_x; \
    } \
    \
    const Tube f(const Tube& tube_x1, const Interval& x2) \
    { \
      Tube new_tube_x(tube_x1); \
      TubeSlice *slice_x, *first_slice_x = new_tube_x.get_first_slice(); \
      const TubeSlice *slice_x1, *first_slice_x1 = tube_x1.get_first_slice(); \
      slice_x = first_slice_x; \
      slice_x1 = first_slice_x1; \
      while(slice_x1 != NULL) \
      { \
        slice_x->set_envelope(IntervalVector(1, ibex::f(slice_x1->codomain()[0], x2))); \
        slice_x = slice_x->next_slice(); \
        slice_x1 = slice_x1->next_slice(); \
      } \
      slice_x = first_slice_x; \
      slice_x1 = first_slice_x1; \
      while(slice_x1 != NULL) \
      { \
        if(slice_x == first_slice_x) \
          slice_x->set_input_gate(IntervalVector(1, ibex::f(slice_x1->input_gate()[0], x2))); \
        slice_x->set_output_gate(IntervalVector(1, ibex::f(slice_x1->output_gate()[0], x2))); \
        slice_x = slice_x->next_slice(); \
        slice_x1 = slice_x1->next_slice(); \
      } \
      return new_tube_x; \
    } \
    \
    const Tube f(const Interval& x1, const Tube& tube_x2) \
    { \
      Tube new_tube_x(tube_x2); \
      TubeSlice *slice_x, *first_slice_x = new_tube_x.get_first_slice(); \
      const TubeSlice *slice_x2, *first_slice_x2 = tube_x2.get_first_slice(); \
      slice_x = first_slice_x; \
      slice_x2 = first_slice_x2; \
      while(slice_x2 != NULL) \
      { \
        slice_x->set_envelope(IntervalVector(1, ibex::f(x1, slice_x2->codomain()[0]))); \
        slice_x = slice_x->next_slice(); \
        slice_x2 = slice_x2->next_slice(); \
      } \
      slice_x = first_slice_x; \
      slice_x2 = first_slice_x2; \
      while(slice_x2 != NULL) \
      { \
        if(slice_x == first_slice_x) \
          slice_x->set_input_gate(IntervalVector(1, ibex::f(x1, slice_x2->input_gate()[0]))); \
        slice_x->set_output_gate(IntervalVector(1, ibex::f(x1, slice_x2->output_gate()[0]))); \
        slice_x = slice_x->next_slice(); \
        slice_x2 = slice_x2->next_slice(); \
      } \
      return new_tube_x; \
    } \
    \
    const Tube f(const Tube& tube_x1, const Trajectory& x2) \
    { \
      DomainException::check(tube_x1, x2); \
      Tube new_tube_x(tube_x1); \
      TubeSlice *slice_x, *first_slice_x = new_tube_x.get_first_slice(); \
      slice_x = first_slice_x; \
      while(slice_x != NULL) \
      { \
        slice_x->set_envelope(IntervalVector(1, ibex::f(slice_x->codomain()[0], x2[slice_x->domain()]))); \
        slice_x = slice_x->next_slice(); \
      } \
      slice_x = first_slice_x; \
      while(slice_x != NULL) \
      { \
        if(slice_x == first_slice_x) \
          slice_x->set_input_gate(IntervalVector(1, ibex::f(slice_x->input_gate()[0], x2[Interval(slice_x->domain().lb())]))); \
        slice_x->set_output_gate(IntervalVector(1, ibex::f(slice_x->output_gate()[0], x2[Interval(slice_x->domain().ub())]))); \
        slice_x = slice_x->next_slice(); \
      } \
      return new_tube_x; \
    } \
    \
    const Tube f(const Trajectory& x1, const Tube& tube_x2) \
    { \
      DomainException::check(tube_x2, x1); \
      Tube new_tube_x(tube_x2); \
      TubeSlice *slice_x, *first_slice_x = new_tube_x.get_first_slice(); \
      slice_x = first_slice_x; \
      while(slice_x != NULL) \
      { \
        slice_x->set_envelope(IntervalVector(1, ibex::f(x1[slice_x->domain()], slice_x->codomain()[0]))); \
        slice_x = slice_x->next_slice(); \
      } \
      slice_x = first_slice_x; \
      while(slice_x != NULL) \
      { \
        if(slice_x == first_slice_x) \
          slice_x->set_input_gate(IntervalVector(1, ibex::f(x1[Interval(slice_x->domain().lb())], slice_x->input_gate()[0]))); \
        slice_x->set_output_gate(IntervalVector(1, ibex::f(x1[Interval(slice_x->domain().ub())], slice_x->output_gate()[0]))); \
        slice_x = slice_x->next_slice(); \
      } \
      return new_tube_x; \
    }

  binary_op_scalar_commutative_types(operator+);
  binary_op_scalar_commutative_types(operator-);
  binary_op_scalar_commutative_types(operator*);
  binary_op_scalar_commutative_types(atan2);
  
  #define binary_op_scalar_noncommutative_types(f) \
    \
    const Tube f(const Tube& tube_x1, const Tube& tube_x2) \
    { \
      DomainException::check(tube_x1, tube_x2); \
      SamplingException::check(tube_x1, tube_x2); \
      Tube new_tube_x(tube_x1); \
      TubeSlice *slice_x, *first_slice_x = new_tube_x.get_first_slice(); \
      const TubeSlice *slice_x1, *first_slice_x1 = tube_x1.get_first_slice(); \
      const TubeSlice *slice_x2, *first_slice_x2 = tube_x2.get_first_slice(); \
      slice_x = first_slice_x; \
      slice_x1 = first_slice_x1; \
      slice_x2 = first_slice_x2; \
      while(slice_x1 != NULL) \
      { \
        slice_x->set_envelope(IntervalVector(1, ibex::f(slice_x1->codomain()[0], slice_x2->codomain()[0]))); \
        slice_x = slice_x->next_slice(); \
        slice_x1 = slice_x1->next_slice(); \
        slice_x2 = slice_x2->next_slice(); \
      } \
      slice_x = first_slice_x; \
      slice_x1 = first_slice_x1; \
      slice_x2 = first_slice_x2; \
      while(slice_x1 != NULL) \
      { \
        if(slice_x == first_slice_x) \
          slice_x->set_input_gate(IntervalVector(1, ibex::f(slice_x1->input_gate()[0], slice_x2->input_gate()[0]))); \
        slice_x->set_output_gate(IntervalVector(1, ibex::f(slice_x1->output_gate()[0], slice_x2->output_gate()[0]))); \
        slice_x = slice_x->next_slice(); \
        slice_x1 = slice_x1->next_slice(); \
        slice_x2 = slice_x2->next_slice(); \
      } \
      return new_tube_x; \
    } \
    \
    const Tube f(const Tube& tube_x1, const Interval& x2) \
    { \
      Tube new_tube_x(tube_x1); \
      TubeSlice *slice_x, *first_slice_x = new_tube_x.get_first_slice(); \
      const TubeSlice *slice_x1, *first_slice_x1 = tube_x1.get_first_slice(); \
      slice_x = first_slice_x; \
      slice_x1 = first_slice_x1; \
      while(slice_x1 != NULL) \
      { \
        slice_x->set_envelope(IntervalVector(1, ibex::f(slice_x1->codomain()[0], x2))); \
        slice_x = slice_x->next_slice(); \
        slice_x1 = slice_x1->next_slice(); \
      } \
      slice_x = first_slice_x; \
      slice_x1 = first_slice_x1; \
      while(slice_x1 != NULL) \
      { \
        if(slice_x == first_slice_x) \
          slice_x->set_input_gate(IntervalVector(1, ibex::f(slice_x1->input_gate()[0], x2))); \
        slice_x->set_output_gate(IntervalVector(1, ibex::f(slice_x1->output_gate()[0], x2))); \
        slice_x = slice_x->next_slice(); \
        slice_x1 = slice_x1->next_slice(); \
      } \
      return new_tube_x; \
    } \
    \
    const Tube f(const Tube& tube_x1, const Trajectory& x2) \
    { \
      DomainException::check(tube_x1, x2); \
      Tube new_tube_x(tube_x1); \
      TubeSlice *slice_x, *first_slice_x = new_tube_x.get_first_slice(); \
      slice_x = first_slice_x; \
      while(slice_x != NULL) \
      { \
        slice_x->set_envelope(IntervalVector(1, ibex::f(slice_x->codomain()[0], x2[slice_x->domain()]))); \
        slice_x = slice_x->next_slice(); \
      } \
      slice_x = first_slice_x; \
      while(slice_x != NULL) \
      { \
        if(slice_x == first_slice_x) \
          slice_x->set_input_gate(IntervalVector(1, ibex::f(slice_x->input_gate()[0], x2[Interval(slice_x->domain().lb())]))); \
        slice_x->set_output_gate(IntervalVector(1, ibex::f(slice_x->output_gate()[0], x2[Interval(slice_x->domain().ub())]))); \
        slice_x = slice_x->next_slice(); \
      } \
      return new_tube_x; \
    }

  binary_op_scalar_commutative_types(operator/);

  #define binary_op(f) \
    \
    const TubeVector f(const TubeVector& tube_x1, const TubeVector& tube_x2) \
    { \
      DomainException::check(tube_x1, tube_x2); \
      SamplingException::check(tube_x1, tube_x2); \
      DimensionException::check(tube_x1, tube_x2); \
      TubeVector new_tube_x(tube_x1); \
      TubeSlice *slice_x, *first_slice_x = new_tube_x.get_first_slice(); \
      const TubeSlice *slice_x1, *first_slice_x1 = tube_x1.get_first_slice(); \
      const TubeSlice *slice_x2, *first_slice_x2 = tube_x2.get_first_slice(); \
      slice_x = first_slice_x; \
      slice_x1 = first_slice_x1; \
      slice_x2 = first_slice_x2; \
      while(slice_x1 != NULL) \
      { \
        slice_x->set_envelope(ibex::f(slice_x1->codomain(), slice_x2->codomain())); \
        slice_x = slice_x->next_slice(); \
        slice_x1 = slice_x1->next_slice(); \
        slice_x2 = slice_x2->next_slice(); \
      } \
      slice_x = first_slice_x; \
      slice_x1 = first_slice_x1; \
      slice_x2 = first_slice_x2; \
      while(slice_x1 != NULL) \
      { \
        if(slice_x == first_slice_x) \
          slice_x->set_input_gate(ibex::f(slice_x1->input_gate(), slice_x2->input_gate())); \
        slice_x->set_output_gate(ibex::f(slice_x1->output_gate(), slice_x2->output_gate())); \
        slice_x = slice_x->next_slice(); \
        slice_x1 = slice_x1->next_slice(); \
        slice_x2 = slice_x2->next_slice(); \
      } \
      return new_tube_x; \
    } \
    \
    const TubeVector f(const TubeVector& tube_x1, const IntervalVector& x2) \
    { \
      DimensionException::check(tube_x1, x2); \
      TubeVector new_tube_x(tube_x1); \
      TubeSlice *slice_x, *first_slice_x = new_tube_x.get_first_slice(); \
      const TubeSlice *slice_x1, *first_slice_x1 = tube_x1.get_first_slice(); \
      slice_x = first_slice_x; \
      slice_x1 = first_slice_x1; \
      while(slice_x1 != NULL) \
      { \
        slice_x->set_envelope(ibex::f(slice_x1->codomain(), x2)); \
        slice_x = slice_x->next_slice(); \
        slice_x1 = slice_x1->next_slice(); \
      } \
      slice_x = first_slice_x; \
      slice_x1 = first_slice_x1; \
      while(slice_x1 != NULL) \
      { \
        if(slice_x == first_slice_x) \
          slice_x->set_input_gate(ibex::f(slice_x1->input_gate(), x2)); \
        slice_x->set_output_gate(ibex::f(slice_x1->output_gate(), x2)); \
        slice_x = slice_x->next_slice(); \
        slice_x1 = slice_x1->next_slice(); \
      } \
      return new_tube_x; \
    } \
    \
    const TubeVector f(const IntervalVector& x1, const TubeVector& tube_x2) \
    { \
      DimensionException::check(tube_x2, x1); \
      TubeVector new_tube_x(tube_x2); \
      TubeSlice *slice_x, *first_slice_x = new_tube_x.get_first_slice(); \
      const TubeSlice *slice_x2, *first_slice_x2 = tube_x2.get_first_slice(); \
      slice_x = first_slice_x; \
      slice_x2 = first_slice_x2; \
      while(slice_x2 != NULL) \
      { \
        slice_x->set_envelope(ibex::f(x1, slice_x2->codomain())); \
        slice_x = slice_x->next_slice(); \
        slice_x2 = slice_x2->next_slice(); \
      } \
      slice_x = first_slice_x; \
      slice_x2 = first_slice_x2; \
      while(slice_x2 != NULL) \
      { \
        if(slice_x == first_slice_x) \
          slice_x->set_input_gate(ibex::f(x1, slice_x2->input_gate())); \
        slice_x->set_output_gate(ibex::f(x1, slice_x2->output_gate())); \
        slice_x = slice_x->next_slice(); \
        slice_x2 = slice_x2->next_slice(); \
      } \
      return new_tube_x; \
    } \
    \
    const TubeVector f(const TubeVector& tube_x1, const TrajectoryVector& x2) \
    { \
      DomainException::check(tube_x1, x2); \
      DimensionException::check(tube_x1, x2); \
      TubeVector new_tube_x(tube_x1); \
      TubeSlice *slice_x, *first_slice_x = new_tube_x.get_first_slice(); \
      slice_x = first_slice_x; \
      while(slice_x != NULL) \
      { \
        slice_x->set_envelope(ibex::f(slice_x->codomain(), x2[slice_x->domain()])); \
        slice_x = slice_x->next_slice(); \
      } \
      slice_x = first_slice_x; \
      while(slice_x != NULL) \
      { \
        if(slice_x == first_slice_x) \
          slice_x->set_input_gate(ibex::f(slice_x->input_gate(), x2[Interval(slice_x->domain().lb())])); \
        slice_x->set_output_gate(ibex::f(slice_x->output_gate(), x2[Interval(slice_x->domain().ub())])); \
        slice_x = slice_x->next_slice(); \
      } \
      return new_tube_x; \
    } \
    \
    const TubeVector f(const TrajectoryVector& x1, const TubeVector& tube_x2) \
    { \
      DomainException::check(tube_x2, x1); \
      DimensionException::check(tube_x2, x1); \
      TubeVector new_tube_x(tube_x2); \
      TubeSlice *slice_x, *first_slice_x = new_tube_x.get_first_slice(); \
      slice_x = first_slice_x; \
      while(slice_x != NULL) \
      { \
        slice_x->set_envelope(ibex::f(x1[slice_x->domain()], slice_x->codomain())); \
        slice_x = slice_x->next_slice(); \
      } \
      slice_x = first_slice_x; \
      while(slice_x != NULL) \
      { \
        if(slice_x == first_slice_x) \
          slice_x->set_input_gate(ibex::f(x1[Interval(slice_x->domain().lb())], slice_x->input_gate())); \
        slice_x->set_output_gate(ibex::f(x1[Interval(slice_x->domain().ub())], slice_x->output_gate())); \
        slice_x = slice_x->next_slice(); \
      } \
      return new_tube_x; \
    }

  binary_op(operator+);
  binary_op(operator-);

  #define binary_op_with_scalar_commutative_types(f) \
    \
    const TubeVector f(const Tube& tube_x1, const TubeVector& tube_x2) \
    { \
      DomainException::check(tube_x1, tube_x2); \
      SamplingException::check(tube_x1, tube_x2); \
      TubeVector new_tube_x(tube_x1); \
      TubeSlice *slice_x, *first_slice_x = new_tube_x.get_first_slice(); \
      const TubeSlice *slice_x1, *first_slice_x1 = tube_x1.get_first_slice(); \
      const TubeSlice *slice_x2, *first_slice_x2 = tube_x2.get_first_slice(); \
      slice_x = first_slice_x; \
      slice_x1 = first_slice_x1; \
      slice_x2 = first_slice_x2; \
      while(slice_x1 != NULL) \
      { \
        slice_x->set_envelope(ibex::f(slice_x1->codomain()[0], slice_x2->codomain())); \
        slice_x = slice_x->next_slice(); \
        slice_x1 = slice_x1->next_slice(); \
        slice_x2 = slice_x2->next_slice(); \
      } \
      slice_x = first_slice_x; \
      slice_x1 = first_slice_x1; \
      slice_x2 = first_slice_x2; \
      while(slice_x1 != NULL) \
      { \
        if(slice_x == first_slice_x) \
          slice_x->set_input_gate(ibex::f(slice_x1->input_gate()[0], slice_x2->input_gate())); \
        slice_x->set_output_gate(ibex::f(slice_x1->output_gate()[0], slice_x2->output_gate())); \
        slice_x = slice_x->next_slice(); \
        slice_x1 = slice_x1->next_slice(); \
        slice_x2 = slice_x2->next_slice(); \
      } \
      return new_tube_x; \
    } \
    \
    const TubeVector f(const TubeVector& tube_x1, const Tube& tube_x2) \
    { \
      DomainException::check(tube_x1, tube_x2); \
      SamplingException::check(tube_x1, tube_x2); \
      TubeVector new_tube_x(tube_x1); \
      TubeSlice *slice_x, *first_slice_x = new_tube_x.get_first_slice(); \
      const TubeSlice *slice_x1, *first_slice_x1 = tube_x1.get_first_slice(); \
      const TubeSlice *slice_x2, *first_slice_x2 = tube_x2.get_first_slice(); \
      slice_x = first_slice_x; \
      slice_x1 = first_slice_x1; \
      slice_x2 = first_slice_x2; \
      while(slice_x1 != NULL) \
      { \
        slice_x->set_envelope(ibex::f(slice_x1->codomain(), slice_x2->codomain()[0])); \
        slice_x = slice_x->next_slice(); \
        slice_x1 = slice_x1->next_slice(); \
        slice_x2 = slice_x2->next_slice(); \
      } \
      slice_x = first_slice_x; \
      slice_x1 = first_slice_x1; \
      slice_x2 = first_slice_x2; \
      while(slice_x1 != NULL) \
      { \
        if(slice_x == first_slice_x) \
          slice_x->set_input_gate(ibex::f(slice_x1->input_gate(), slice_x2->input_gate()[0])); \
        slice_x->set_output_gate(ibex::f(slice_x1->output_gate(), slice_x2->output_gate()[0])); \
        slice_x = slice_x->next_slice(); \
        slice_x1 = slice_x1->next_slice(); \
        slice_x2 = slice_x2->next_slice(); \
      } \
      return new_tube_x; \
    } \
    \
    const TubeVector f(const Interval& x1, const TubeVector& tube_x2) \
    { \
      TubeVector new_tube_x(tube_x2); \
      TubeSlice *slice_x, *first_slice_x = new_tube_x.get_first_slice(); \
      slice_x = first_slice_x; \
      while(slice_x != NULL) \
      { \
        slice_x->set_envelope(ibex::f(x1, slice_x->codomain())); \
        slice_x = slice_x->next_slice(); \
      } \
      slice_x = first_slice_x; \
      while(slice_x != NULL) \
      { \
        if(slice_x == first_slice_x) \
          slice_x->set_input_gate(ibex::f(x1, slice_x->input_gate())); \
        slice_x->set_output_gate(ibex::f(x1, slice_x->output_gate())); \
        slice_x = slice_x->next_slice(); \
      } \
      return new_tube_x; \
    } \
    \
    const TubeVector f(const TubeVector& tube_x1, const Interval& x2) \
    { \
      TubeVector new_tube_x(tube_x1); \
      TubeSlice *slice_x, *first_slice_x = new_tube_x.get_first_slice(); \
      slice_x = first_slice_x; \
      while(slice_x != NULL) \
      { \
        slice_x->set_envelope(ibex::f(slice_x->codomain(), x2)); \
        slice_x = slice_x->next_slice(); \
      } \
      slice_x = first_slice_x; \
      while(slice_x != NULL) \
      { \
        if(slice_x == first_slice_x) \
          slice_x->set_input_gate(ibex::f(slice_x->input_gate(), x2)); \
        slice_x->set_output_gate(ibex::f(slice_x->output_gate(), x2)); \
        slice_x = slice_x->next_slice(); \
      } \
      return new_tube_x; \
    } \
    \
    const TubeVector f(const Trajectory& x1, const TubeVector& tube_x2) \
    { \
      DomainException::check(tube_x2, x1); \
      TubeVector new_tube_x(tube_x2); \
      TubeSlice *slice_x, *first_slice_x = new_tube_x.get_first_slice(); \
      slice_x = first_slice_x; \
      while(slice_x != NULL) \
      { \
        slice_x->set_envelope(ibex::f(x1[slice_x->domain()], slice_x->codomain())); \
        slice_x = slice_x->next_slice(); \
      } \
      slice_x = first_slice_x; \
      while(slice_x != NULL) \
      { \
        if(slice_x == first_slice_x) \
          slice_x->set_input_gate(ibex::f(x1[Interval(slice_x->domain().lb())], slice_x->input_gate())); \
        slice_x->set_output_gate(ibex::f(x1[Interval(slice_x->domain().ub())], slice_x->output_gate())); \
        slice_x = slice_x->next_slice(); \
      } \
      return new_tube_x; \
    } \
    \
    const TubeVector f(const TubeVector& tube_x1, const Trajectory& x2) \
    { \
      DomainException::check(tube_x1, x2); \
      TubeVector new_tube_x(tube_x1); \
      TubeSlice *slice_x, *first_slice_x = new_tube_x.get_first_slice(); \
      slice_x = first_slice_x; \
      while(slice_x != NULL) \
      { \
        slice_x->set_envelope(ibex::f(slice_x->codomain(), x2[slice_x->domain()])); \
        slice_x = slice_x->next_slice(); \
      } \
      slice_x = first_slice_x; \
      while(slice_x != NULL) \
      { \
        if(slice_x == first_slice_x) \
          slice_x->set_input_gate(ibex::f(slice_x->input_gate(), x2[Interval(slice_x->domain().lb())])); \
        slice_x->set_output_gate(ibex::f(slice_x->output_gate(), x2[Interval(slice_x->domain().ub())])); \
        slice_x = slice_x->next_slice(); \
      } \
      return new_tube_x; \
    }

  binary_op_with_scalar_commutative_types(operator+);
  binary_op_with_scalar_commutative_types(operator-);
  binary_op_with_scalar_commutative_types(operator*);
}