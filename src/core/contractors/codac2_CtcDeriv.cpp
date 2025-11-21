/** 
 *  codac2_CtcDeriv.cpp
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Simon Rohou
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_CtcDeriv.h"
#include "codac2_Slice.h"
#include "codac2_Interval.h"
#include "codac2_trunc.h"

using namespace std;
using namespace codac2;

CtcDeriv::CtcDeriv(const TimePropag& time_propag, bool fast_mode)
  : _time_propag(time_propag), _fast_mode(fast_mode)
{ }

ConvexPolygon CtcDeriv::polygon_slice(
  const Interval& t, const Interval& envelope,
  const Interval& input, const Interval& proj_input,
  const Interval& output, const Interval& proj_output,
  const Interval& v)
{
  if(v.is_empty() || input.is_empty() || output.is_empty())
    return ConvexPolygon::empty();

  ConvexPolygon p(cart_prod(t,trunc(envelope)));

  ConvexPolygon p_fwd(std::vector<Vector>({
    {t.ub(), trunc(proj_output.ub())},
    {t.lb(), trunc(input.ub())},
    {t.lb(), trunc(input.lb())},
    {t.ub(), trunc(proj_output.lb())}
  }), !false); // false: avoid costly computation of convex hull (vertices are already ordered)
  p &= p_fwd;

  ConvexPolygon p_bwd(std::vector<Vector>({
    {t.lb(), trunc(proj_input.ub())},
    {t.lb(), trunc(proj_input.lb())},
    {t.ub(), trunc(output.lb())},
    {t.ub(), trunc(output.ub())}
  }), !false); // false: avoid costly computation of convex hull (vertices are already ordered)
  p &= p_bwd;
    
  return p;
}

void CtcDeriv::contract(
  const Interval& t, Interval& envelope,
  Interval& input, Interval& output,
  const Interval& v, const TimePropag& time_propag,
  bool fast_mode)
{
  Interval proj_input;
  Interval proj_output;

  // /!\ .diam() method is not reliable (floating point result)
  // -> We need to compute the diameter with intervals
  Interval d = Interval(t.ub())-Interval(t.lb());

  if((time_propag & TimePropag::FWD) == TimePropag::FWD)
    proj_output &= input + d * v;

  if((time_propag & TimePropag::BWD) == TimePropag::BWD)
    proj_input &= output - d * v;

  if(!fast_mode && time_propag == TimePropag::FWD_BWD
    && (proj_output.is_superset(output) || proj_input.is_superset(input)))
  {
    // Optimal envelope
    envelope = untrunc(CtcDeriv::polygon_slice(
        t, envelope,
        input, proj_input,
        output, proj_output,
        v
      ).box()[1]);
  }

  else
  {
    if((time_propag & TimePropag::FWD) == TimePropag::FWD)
      envelope &= input + Interval(0,d.ub()) * v;

    if((time_propag & TimePropag::BWD) == TimePropag::BWD)
      envelope &= output - Interval(0,d.ub()) * v;
  }

  input &= proj_input;
  output &= proj_output;
}