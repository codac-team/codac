/** 
 *  Slice class (operators)
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac_Slice.h"

using namespace std;
using namespace ibex;

namespace codac
{
  #define macro_assignment_op_slice(f) \
    \
    Slice& Slice::f(double x) \
    { \
      Interval y = codomain(); y.f(x); set_envelope(y, false); \
      y = input_gate(); y.f(x); set_input_gate(y, false); \
      y = output_gate(); y.f(x); set_output_gate(y, false); \
      return *this; \
    } \
    \
    Slice& Slice::f(const Slice& s_x) \
    { \
      assert(tdomain() == s_x.tdomain()); \
      Interval y = codomain(); y.f(s_x.codomain()); set_envelope(y, false); \
      y = input_gate(); y.f(s_x.input_gate()); set_input_gate(y, false); \
      y = output_gate(); y.f(s_x.output_gate()); set_output_gate(y, false); \
      return *this; \
    } \
    \
    Slice& Slice::f(const Trajectory& traj_x) \
    { \
      assert(tdomain().is_subset(traj_x.tdomain())); \
      Interval y = codomain(); y.f(traj_x(tdomain())); set_envelope(y, false); \
      y = input_gate(); y.f(traj_x(Interval(tdomain().lb()))); set_input_gate(y, false); \
      y = output_gate(); y.f(traj_x(Interval(tdomain().ub()))); set_output_gate(y, false); \
      return *this; \
    }

  macro_assignment_op_slice(operator+=);
  macro_assignment_op_slice(operator-=);
  macro_assignment_op_slice(operator*=);
  macro_assignment_op_slice(operator/=);
  macro_assignment_op_slice(operator|=);
  macro_assignment_op_slice(operator&=);

  // Note: operator*= and operator/= are not defined for vector items
}