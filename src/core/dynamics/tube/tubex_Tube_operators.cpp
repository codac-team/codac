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
#include "tubex_SlicingException.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  #define assignment_op_scalar(f) \
    \
    const Tube& Tube::f(const Tube& x) \
    { \
      SlicingException::check(*this, x); \
      DomainException::check(*this, x); \
      Tube copy(*this); \
      TubeSlice *slice = get_first_slice(); \
      TubeSlice *slice_copy = copy.get_first_slice(); \
      const TubeSlice *slice_x = x.get_first_slice(); \
      while(slice != NULL) \
      { \
        Interval input_gate = slice_copy->input_gate(); \
        Interval envelope = slice_copy->codomain(); \
        Interval output_gate = slice_copy->output_gate(); \
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
    const Tube& Tube::f(const Trajectory& x) \
    { \
      DomainException::check(*this, x); \
      Tube copy(*this); \
      TubeSlice *slice = get_first_slice(); \
      TubeSlice *slice_copy = copy.get_first_slice(); \
      while(slice != NULL) \
      { \
        Interval input_gate = slice_copy->input_gate(); \
        Interval envelope = slice_copy->codomain(); \
        Interval output_gate = slice_copy->output_gate(); \
        slice->set_envelope(envelope.f(x(slice->domain()))); \
        slice->set_input_gate(input_gate.f(x(slice->domain().lb()))); \
        slice->set_output_gate(output_gate.f(x(slice->domain().ub()))); \
        slice = slice->next_slice(); \
        slice_copy = slice_copy->next_slice(); \
      } \
      return *this; \
    } \
    \
    const Tube& Tube::f(const Interval& x) \
    { \
      Tube copy(*this); \
      TubeSlice *slice = get_first_slice(); \
      TubeSlice *slice_copy = copy.get_first_slice(); \
      while(slice != NULL) \
      { \
        Interval input_gate = slice_copy->input_gate(); \
        Interval envelope = slice_copy->codomain(); \
        Interval output_gate = slice_copy->output_gate(); \
        slice->set_envelope(envelope.f(x)); \
        slice->set_input_gate(input_gate.f(x)); \
        slice->set_output_gate(output_gate.f(x)); \
        slice = slice->next_slice(); \
        slice_copy = slice_copy->next_slice(); \
      } \
      return *this; \
    }

  assignment_op_scalar(operator+=);
  assignment_op_scalar(operator-=);
  assignment_op_scalar(operator*=);
  assignment_op_scalar(operator/=);
  assignment_op_scalar(operator|=);
  assignment_op_scalar(operator&=);

  // Note: other operators are defined in the vector case*/
}