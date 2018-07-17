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
#include "tubex_SlicingException.h"

using namespace std;
using namespace ibex;

// todo: optimize this by avoiding copy of tubes

namespace tubex
{
  #define assignment_op_vector(f) \
    \
    const TubeVector& TubeVector::f(const TubeVector& x) \
    { \
      DimensionException::check(*this, x); \
      SlicingException::check(*this, x); \
      DomainException::check(*this, x); \
      TubeVector copy(*this); \
      TubeSlice *slice = get_first_slice(); \
      TubeSlice *slice_copy = copy.get_first_slice(); \
      const TubeSlice *slice_x = x.get_first_slice(); \
      while(slice != NULL) \
      { \
        IntervalVector input_gate = slice_copy->input_gate(); \
        IntervalVector envelope = slice_copy->codomain(); \
        IntervalVector output_gate = slice_copy->output_gate(); \
        slice->set_envelope(envelope.f(slice_x->codomain())); \
        slice->set_input_gate(input_gate.f(slice_x->input_gate())); \
        slice->set_output_gate(output_gate.f(slice_x->output_gate())); \
        slice = slice->next_slice(); \
        slice_x = slice_x->next_slice(); \
        slice_copy = slice_copy->next_slice(); \
      } \
      return *this; \
    } \
    \
    const TubeVector& TubeVector::f(const TrajectoryVector& x) \
    { \
      DimensionException::check(*this, x); \
      DomainException::check(*this, x); \
      TubeVector copy(*this); \
      TubeSlice *slice = get_first_slice(); \
      TubeSlice *slice_copy = copy.get_first_slice(); \
      while(slice != NULL) \
      { \
        IntervalVector input_gate = slice_copy->input_gate(); \
        IntervalVector envelope = slice_copy->codomain(); \
        IntervalVector output_gate = slice_copy->output_gate(); \
        slice->set_envelope(envelope.f(x[slice->domain()])); \
        slice->set_input_gate(input_gate.f(x[slice->domain().lb()])); \
        slice->set_output_gate(output_gate.f(x[slice->domain().ub()])); \
        slice = slice->next_slice(); \
        slice_copy = slice_copy->next_slice(); \
      } \
      return *this; \
    } \
    \
    const TubeVector& TubeVector::f(const IntervalVector& x) \
    { \
      DimensionException::check(*this, x); \
      TubeVector copy(*this); \
      TubeSlice *slice = get_first_slice(); \
      TubeSlice *slice_copy = copy.get_first_slice(); \
      while(slice != NULL) \
      { \
        IntervalVector input_gate = slice_copy->input_gate(); \
        IntervalVector envelope = slice_copy->codomain(); \
        IntervalVector output_gate = slice_copy->output_gate(); \
        slice->set_envelope(envelope.f(x)); \
        slice->set_input_gate(input_gate.f(x)); \
        slice->set_output_gate(output_gate.f(x)); \
        slice = slice->next_slice(); \
        slice_copy = slice_copy->next_slice(); \
      } \
      return *this; \
    }

  assignment_op_vector(operator+=);
  assignment_op_vector(operator-=);
  assignment_op_vector(operator|=);
  assignment_op_vector(operator&=);
  assignment_op_vector(operator|);
  assignment_op_vector(operator&);

  // Note: operator*= and operator/= are not defined for vector items

  #define assignment_op_vector_with_scalar_converted_to_vector(f) \
    \
    const TubeVector& TubeVector::f(const Tube& x) \
    { \
      DomainException::check(*this, x); \
      if(TubeVector::share_same_slicing(*this, x)) \
      { \
        int n = dim(); \
        TubeVector copy(*this); \
        TubeSlice *slice = get_first_slice(); \
        TubeSlice *slice_copy = copy.get_first_slice(); \
        const TubeSlice *slice_x = x.get_first_slice(); \
        while(slice != NULL) \
        { \
          IntervalVector input_gate = slice_copy->input_gate(); \
          IntervalVector envelope = slice_copy->codomain(); \
          IntervalVector output_gate = slice_copy->output_gate(); \
          slice->set_envelope(envelope.f(IntervalVector(n, slice_x->codomain()[0]))); \
          slice->set_input_gate(input_gate.f(IntervalVector(n, slice_x->input_gate()[0]))); \
          slice->set_output_gate(output_gate.f(IntervalVector(n, slice_x->output_gate()[0]))); \
          slice = slice->next_slice(); \
          slice_x = slice_x->next_slice(); \
          slice_copy = slice_copy->next_slice(); \
        } \
      } \
      else \
        SlicingException::check(*this, x); \
      return *this; \
    } \
    \
    const TubeVector& TubeVector::f(const Trajectory& x) \
    { \
      DomainException::check(*this, x); \
      int n = dim(); \
      TubeVector copy(*this); \
      TubeSlice *slice = get_first_slice(); \
      TubeSlice *slice_copy = copy.get_first_slice(); \
      while(slice != NULL) \
      { \
        IntervalVector input_gate = slice_copy->input_gate(); \
        IntervalVector envelope = slice_copy->codomain(); \
        IntervalVector output_gate = slice_copy->output_gate(); \
        slice->set_envelope(envelope.f(IntervalVector(n, x[slice->domain()]))); \
        slice->set_input_gate(input_gate.f(IntervalVector(n, x[slice->domain().lb()]))); \
        slice->set_output_gate(output_gate.f(IntervalVector(n, x[slice->domain().ub()]))); \
        slice = slice->next_slice(); \
        slice_copy = slice_copy->next_slice(); \
      } \
      return *this; \
    } \
    \
    const TubeVector& TubeVector::f(const Interval& x) \
    { \
      TubeVector copy(*this); \
      int n = dim(); \
      TubeSlice *slice = get_first_slice(); \
      TubeSlice *slice_copy = copy.get_first_slice(); \
      while(slice != NULL) \
      { \
        IntervalVector input_gate = slice_copy->input_gate(); \
        IntervalVector envelope = slice_copy->codomain(); \
        IntervalVector output_gate = slice_copy->output_gate(); \
        slice->set_envelope(envelope.f(IntervalVector(n, x))); \
        slice->set_input_gate(input_gate.f(IntervalVector(n, x))); \
        slice->set_output_gate(output_gate.f(IntervalVector(n, x))); \
        slice = slice->next_slice(); \
        slice_copy = slice_copy->next_slice(); \
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
      SlicingException::check(*this, x); \
      TubeVector copy(*this); \
      TubeSlice *slice = get_first_slice(); \
      TubeSlice *slice_copy = copy.get_first_slice(); \
      const TubeSlice *slice_x = x.get_first_slice(); \
      while(slice != NULL) \
      { \
        IntervalVector input_gate = slice_copy->input_gate(); \
        IntervalVector envelope = slice_copy->codomain(); \
        IntervalVector output_gate = slice_copy->output_gate(); \
        slice->set_envelope(envelope.f(slice_x->codomain()[0])); \
        slice->set_input_gate(input_gate.f(slice_x->input_gate()[0])); \
        slice->set_output_gate(output_gate.f(slice_x->output_gate()[0])); \
        slice = slice->next_slice(); \
        slice_x = slice_x->next_slice(); \
        slice_copy = slice_copy->next_slice(); \
      } \
      return *this; \
    } \
    \
    const TubeVector& TubeVector::f(const Trajectory& x) \
    { \
      DomainException::check(*this, x); \
      TubeVector copy(*this); \
      TubeSlice *slice = get_first_slice(); \
      TubeSlice *slice_copy = copy.get_first_slice(); \
      while(slice != NULL) \
      { \
        IntervalVector input_gate = slice_copy->input_gate(); \
        IntervalVector envelope = slice_copy->codomain(); \
        IntervalVector output_gate = slice_copy->output_gate(); \
        slice->set_envelope(envelope.f(x[slice->domain()])); \
        slice->set_input_gate(input_gate.f(x[slice->domain().lb()])); \
        slice->set_output_gate(output_gate.f(x[slice->domain().ub()])); \
        slice = slice->next_slice(); \
        slice_copy = slice_copy->next_slice(); \
      } \
      return *this; \
    } \
    \
    const TubeVector& TubeVector::f(const Interval& x) \
    { \
      TubeVector copy(*this); \
      TubeSlice *slice = get_first_slice(); \
      TubeSlice *slice_copy = copy.get_first_slice(); \
      while(slice != NULL) \
      { \
        IntervalVector input_gate = slice_copy->input_gate(); \
        IntervalVector envelope = slice_copy->codomain(); \
        IntervalVector output_gate = slice_copy->output_gate(); \
        slice->set_envelope(envelope.f(x)); \
        slice->set_input_gate(input_gate.f(x)); \
        slice->set_output_gate(output_gate.f(x)); \
        slice = slice->next_slice(); \
        slice_copy = slice_copy->next_slice(); \
      } \
      return *this; \
    }

  assignment_op_vector_with_scalar(operator*=);
}