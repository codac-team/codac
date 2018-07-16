/* ============================================================================
 *  tubex-lib - Tube class (operators)
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "tubex_Tube.h"
#include "tubex_Trajectory.h"
#include "tubex_DomainException.h"
#include "tubex_DimensionException.h"
#include "tubex_SamplingException.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  #define assignment_op_scalar(f) \
    \
    const Tube& Tube::f(const Tube& x) \
    { \
      SamplingException::check(*this, x); \
      Interval y; \
      TubeSlice *slice, *first_slice = get_first_slice(); \
      const TubeSlice *slice_x, *first_slice_x = x.get_first_slice(); \
      slice = first_slice; \
      slice_x = first_slice_x; \
      while(slice != NULL) \
      { \
        y = slice->codomain()[0]; \
        y.f(slice_x->codomain()[0]); \
        slice->set_envelope(IntervalVector(1, y)); \
        slice = slice->next_slice(); \
        slice_x = slice_x->next_slice(); \
      } \
      slice = first_slice; \
      slice_x = first_slice_x; \
      while(slice != NULL) \
      { \
        if(slice == first_slice) \
        { \
          y = slice->input_gate()[0]; \
          y.f(slice_x->input_gate()[0]); \
          slice->set_input_gate(IntervalVector(1, y)); \
        } \
        y = slice->output_gate()[0]; \
        y.f(slice_x->output_gate()[0]); \
        slice->set_output_gate(IntervalVector(1, y)); \
        slice = slice->next_slice(); \
        slice_x = slice_x->next_slice(); \
      } \
      return *this; \
    } \
    \
    const Tube& Tube::f(const Interval& x) \
    { \
      Interval y; \
      TubeSlice *slice, *first_slice = get_first_slice(); \
      slice = first_slice; \
      while(slice != NULL) \
      { \
        y = slice->codomain()[0]; \
        y.f(x); \
        slice->set_envelope(IntervalVector(1, y)); \
        slice = slice->next_slice(); \
      } \
      slice = first_slice; \
      while(slice != NULL) \
      { \
        if(slice == first_slice) \
        { \
          y = slice->input_gate()[0]; \
          y.f(x); \
          slice->set_input_gate(IntervalVector(1, y)); \
        } \
        y = slice->output_gate()[0]; \
        y.f(x); \
        slice->set_output_gate(IntervalVector(1, y)); \
        slice = slice->next_slice(); \
      } \
      return *this; \
    } \
    \
    const Tube& Tube::f(const Trajectory& x) \
    { \
      DomainException::check(*this, x); \
      Interval y; \
      TubeSlice *slice, *first_slice = get_first_slice(); \
      slice = first_slice; \
      while(slice != NULL) \
      { \
        y = slice->codomain()[0]; \
        y.f(x[slice->domain()]); \
        slice->set_envelope(IntervalVector(1, y)); \
        slice = slice->next_slice(); \
      } \
      slice = first_slice; \
      while(slice != NULL) \
      { \
        if(slice == first_slice) \
        { \
          y = slice->input_gate()[0]; \
          y.f(x[Interval(slice->domain().lb())]); \
          slice->set_input_gate(IntervalVector(1, y)); \
        } \
        y = slice->output_gate()[0]; \
        y.f(x[Interval(slice->domain().ub())]); \
        slice->set_output_gate(IntervalVector(1, y)); \
        slice = slice->next_slice(); \
      } \
      return *this; \
    }

  //assignment_op_scalar(operator*=);
  assignment_op_scalar(operator/=);
  //assignment_op_scalar(operator&=);
  //assignment_op_scalar(operator|=);

  // Note: other operators are defined in the vector case
}