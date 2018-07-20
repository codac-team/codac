/* ============================================================================
 *  tubex-lib - TubeSlice class (operators)
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "tubex_TubeSlice.h"
#include "tubex_DomainException.h"
#include "tubex_SlicingException.h"
#include "tubex_DimensionException.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  #define assignment_op_slice(f) \
    \
    TubeSlice& TubeSlice::f(const TubeSlice& slice_x) \
    { \
      DomainException::check(*this, slice_x); \
      DimensionException::check(*this, slice_x); \
      IntervalVector y = codomain(); y.f(slice_x.codomain()); set_envelope(y); \
      y = input_gate(); y.f(slice_x.input_gate()); set_input_gate(y); \
      y = output_gate(); y.f(slice_x.output_gate()); set_output_gate(y); \
      return *this; \
    } \
    \
    TubeSlice& TubeSlice::f(const TrajectoryVector& traj_x) \
    { \
      DomainException::check(*this, traj_x); \
      DimensionException::check(*this, traj_x); \
      IntervalVector y = codomain(); y.f(traj_x[domain()]); set_envelope(y); \
      y = input_gate(); y.f(traj_x[Interval(domain().lb())]); set_input_gate(y); \
      y = output_gate(); y.f(traj_x[Interval(domain().ub())]); set_output_gate(y); \
      return *this; \
    }

  assignment_op_slice(operator+=);
  assignment_op_slice(operator-=);
  assignment_op_slice(operator|=);
  assignment_op_slice(operator&=);

  // Note: operator*= and operator/= are not defined for vector items
}