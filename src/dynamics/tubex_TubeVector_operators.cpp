/* ============================================================================
 *  tubex-lib - TubeVector class (operators)
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
#include "tubex_TubeVector.h"
#include "tubex_TrajectoryVector.h"
#include "tubex_DomainException.h"
#include "tubex_DimensionException.h"
#include "tubex_SamplingException.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  #define assignment_op_vector(f) \
    \
    const TubeVector& TubeVector::f(const TubeVector& x) \
    { \
      SamplingException::check(*this, x); \
      DimensionException::check(*this, x); \
      IntervalVector y(x.dim()); \
      TubeSlice *slice, *first_slice = get_first_slice(); \
      const TubeSlice *slice_x, *first_slice_x = x.get_first_slice(); \
      slice = first_slice; \
      slice_x = first_slice_x; \
      while(slice != NULL) \
      { \
        y = slice->codomain(); \
        y.f(slice_x->codomain()); \
        slice->set_envelope(y); \
        slice = slice->next_slice(); \
        slice_x = slice_x->next_slice(); \
      } \
      slice = first_slice; \
      slice_x = first_slice_x; \
      while(slice != NULL) \
      { \
        if(slice == first_slice) \
        { \
          y = slice->input_gate(); \
          y.f(slice_x->input_gate()); \
          slice->set_input_gate(y); \
        } \
        y = slice->output_gate(); \
        y.f(slice_x->output_gate()); \
        slice->set_output_gate(y); \
        slice = slice->next_slice(); \
        slice_x = slice_x->next_slice(); \
      } \
      return *this; \
    } \
    \
    const TubeVector& TubeVector::f(const TrajectoryVector& x) \
    { \
      DomainException::check(*this, x); \
      DimensionException::check(*this, x); \
      IntervalVector y(x.dim()); \
      TubeSlice *slice, *first_slice = get_first_slice(); \
      slice = first_slice; \
      while(slice != NULL) \
      { \
        y = slice->codomain(); \
        y.f(x[slice->domain()]); \
        slice->set_envelope(y); \
        slice = slice->next_slice(); \
      } \
      slice = first_slice; \
      while(slice != NULL) \
      { \
        if(slice == first_slice) \
        { \
          y = slice->input_gate(); \
          y.f(x[Interval(slice->domain().lb())]); \
          slice->set_input_gate(y); \
        } \
        y = slice->output_gate(); \
        y.f(x[Interval(slice->domain().ub())]); \
        slice->set_output_gate(y); \
        slice = slice->next_slice(); \
      } \
      return *this; \
    } \
    \
    const TubeVector& TubeVector::f(const IntervalVector& x) \
    { \
      DimensionException::check(*this, x); \
      IntervalVector y(x); \
      TubeSlice *slice, *first_slice = get_first_slice(); \
      slice = first_slice; \
      while(slice != NULL) \
      { \
        y = slice->codomain(); \
        y.f(x); \
        slice->set_envelope(y); \
        slice = slice->next_slice(); \
      } \
      slice = first_slice; \
      while(slice != NULL) \
      { \
        if(slice == first_slice) \
        { \
          y = slice->input_gate(); \
          y.f(x); \
          slice->set_input_gate(y); \
        } \
        y = slice->output_gate(); \
        y.f(x); \
        slice->set_output_gate(y); \
        slice = slice->next_slice(); \
      } \
      return *this; \
    }

  assignment_op_vector(operator+=);
  assignment_op_vector(operator-=);
  assignment_op_vector(operator|=);
  assignment_op_vector(operator&=);
  //assignment_op_vector(operator|);
  //assignment_op_vector(operator&);

  // Note: operator*= and operator/= are not defined for vector items

  #define assignment_op_vector_with_scalar_converted_to_vector(f) \
    \
    const TubeVector& TubeVector::f(const Tube& x) \
    { \
      /* todo: SamplingException::check(*this, x); */\
      IntervalVector y(dim()); \
      TubeSlice *slice, *first_slice = get_first_slice(); \
      const TubeSlice *slice_x, *first_slice_x = x.get_first_slice(); \
      slice = first_slice; \
      slice_x = first_slice_x; \
      while(slice != NULL) \
      { \
        y = slice->codomain(); \
        y.f(IntervalVector(y.size(), slice_x->codomain()[0])); \
        slice->set_envelope(y); \
        slice = slice->next_slice(); \
        slice_x = slice_x->next_slice(); \
      } \
      slice = first_slice; \
      slice_x = first_slice_x; \
      while(slice != NULL) \
      { \
        if(slice == first_slice) \
        { \
          y = slice->input_gate(); \
          y.f(IntervalVector(y.size(), slice_x->input_gate()[0])); \
          slice->set_input_gate(y); \
        } \
        y = slice->output_gate(); \
        y.f(IntervalVector(y.size(), slice_x->output_gate()[0])); \
        slice->set_output_gate(y); \
        slice = slice->next_slice(); \
        slice_x = slice_x->next_slice(); \
      } \
      return *this; \
    } \
    \
    const TubeVector& TubeVector::f(const Trajectory& x) \
    { \
      DomainException::check(*this, (TrajectoryVector)x); \
      IntervalVector y(dim()); \
      TubeSlice *slice, *first_slice = get_first_slice(); \
      slice = first_slice; \
      while(slice != NULL) \
      { \
        y = slice->codomain(); \
        y.f(IntervalVector(y.size(), x[slice->domain()])); \
        slice->set_envelope(y); \
        slice = slice->next_slice(); \
      } \
      slice = first_slice; \
      while(slice != NULL) \
      { \
        if(slice == first_slice) \
        { \
          y = slice->input_gate(); \
          y.f(IntervalVector(y.size(), x[slice->domain().lb()])); \
          slice->set_input_gate(y); \
        } \
        y = slice->output_gate(); \
        y.f(IntervalVector(y.size(), x[slice->domain().ub()])); \
        slice->set_output_gate(y); \
        slice = slice->next_slice(); \
      } \
      return *this; \
    } \
    \
    const TubeVector& TubeVector::f(const Interval& x) \
    { \
      IntervalVector y(dim()); \
      TubeSlice *slice, *first_slice = get_first_slice(); \
      slice = first_slice; \
      while(slice != NULL) \
      { \
        y = slice->codomain(); \
        y.f(IntervalVector(y.size(), x)); \
        slice->set_envelope(y); \
        slice = slice->next_slice(); \
      } \
      slice = first_slice; \
      while(slice != NULL) \
      { \
        if(slice == first_slice) \
        { \
          y = slice->input_gate(); \
          y.f(IntervalVector(y.size(), x)); \
          slice->set_input_gate(y); \
        } \
        y = slice->output_gate(); \
        y.f(IntervalVector(y.size(), x)); \
        slice->set_output_gate(y); \
        slice = slice->next_slice(); \
      } \
      return *this; \
    }

  assignment_op_vector_with_scalar_converted_to_vector(operator+=);
  assignment_op_vector_with_scalar_converted_to_vector(operator-=);
  assignment_op_vector_with_scalar_converted_to_vector(operator|=);
  assignment_op_vector_with_scalar_converted_to_vector(operator&=);

  #define assignment_op_vector_with_scalar(f) \
    \
    const TubeVector& TubeVector::f(const Tube& x) \
    { \
      /* todo: SamplingException::check(*this, x); */\
      IntervalVector y(dim()); \
      TubeSlice *slice, *first_slice = get_first_slice(); \
      const TubeSlice *slice_x, *first_slice_x = x.get_first_slice(); \
      slice = first_slice; \
      slice_x = first_slice_x; \
      while(slice != NULL) \
      { \
        y = slice->codomain(); \
        y.f(slice_x->codomain()[0]); \
        slice->set_envelope(y); \
        slice = slice->next_slice(); \
        slice_x = slice_x->next_slice(); \
      } \
      slice = first_slice; \
      slice_x = first_slice_x; \
      while(slice != NULL) \
      { \
        if(slice == first_slice) \
        { \
          y = slice->input_gate(); \
          y.f(slice_x->input_gate()[0]); \
          slice->set_input_gate(y); \
        } \
        y = slice->output_gate(); \
        y.f(slice_x->output_gate()[0]); \
        slice->set_output_gate(y); \
        slice = slice->next_slice(); \
        slice_x = slice_x->next_slice(); \
      } \
      return *this; \
    } \
    \
    const TubeVector& TubeVector::f(const Trajectory& x) \
    { \
      DomainException::check(*this, (TrajectoryVector)x); \
      IntervalVector y(dim()); \
      TubeSlice *slice, *first_slice = get_first_slice(); \
      slice = first_slice; \
      while(slice != NULL) \
      { \
        y = slice->codomain(); \
        y.f(x[slice->domain()]); \
        slice->set_envelope(y); \
        slice = slice->next_slice(); \
      } \
      slice = first_slice; \
      while(slice != NULL) \
      { \
        if(slice == first_slice) \
        { \
          y = slice->input_gate(); \
          y.f(x[slice->domain().lb()]); \
          slice->set_input_gate(y); \
        } \
        y = slice->output_gate(); \
        y.f(x[slice->domain().ub()]); \
        slice->set_output_gate(y); \
        slice = slice->next_slice(); \
      } \
      return *this; \
    } \
    \
    const TubeVector& TubeVector::f(const Interval& x) \
    { \
      IntervalVector y(dim()); \
      TubeSlice *slice, *first_slice = get_first_slice(); \
      slice = first_slice; \
      while(slice != NULL) \
      { \
        y = slice->codomain(); \
        y.f(x); \
        slice->set_envelope(y); \
        slice = slice->next_slice(); \
      } \
      slice = first_slice; \
      while(slice != NULL) \
      { \
        if(slice == first_slice) \
        { \
          y = slice->input_gate(); \
          y.f(x); \
          slice->set_input_gate(y); \
        } \
        y = slice->output_gate(); \
        y.f(x); \
        slice->set_output_gate(y); \
        slice = slice->next_slice(); \
      } \
      return *this; \
    }

  assignment_op_vector_with_scalar(operator*=);
}