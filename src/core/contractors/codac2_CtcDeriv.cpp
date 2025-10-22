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

using namespace std;
using namespace codac2;

Interval slice_hull(const Interval& dt,
  const Interval& x, const Interval& input_x, const Interval& output_x,
  const Interval& v)
{
  Interval hull = x;
  hull &= input_x  + dt*v;
  hull &= output_x - dt*v;
  return hull;
}

CtcDeriv::CtcDeriv()
{

}

void CtcDeriv::contract(Slice<Interval>& x, const Slice<Interval>& v) const
{
  assert_release(x.t0_tf() == v.t0_tf());
  
  Interval ingate = x.input_gate();
  Interval outgate = x.output_gate();

  // Gates contraction
  ingate &= x.output_gate() - x.t0_tf().diam() * v.codomain();
  outgate &= x.input_gate() + x.t0_tf().diam() * v.codomain();

  if(outgate.is_superset(x.output_gate()) || ingate.is_superset(x.input_gate()))
  {
    auto prev_x = x.prev_slice();
    auto next_x = x.next_slice();

    // Gates needed for polygon computation
    if(prev_x->is_gate())
      prev_x->set(ingate, false);
    if(next_x->is_gate())
      next_x->set(outgate, false);

    // Optimal envelope
    x.set(x.polygon_slice(v).box()[1], false);
  }

  else
  {
    x.set(x.codomain()
      & (ingate + Interval(0,x.t0_tf().diam()) * v.codomain())
      & (outgate - Interval(0,x.t0_tf().diam()) * v.codomain())
    , false);
  }
}

void CtcDeriv::contract(Slice<IntervalVector>& x, const Slice<IntervalVector>& v) const
{
  assert_release(x.size() == v.size());
  assert_release(x.t0_tf() == v.t0_tf());

  auto x_prev = x.prev_slice();
  auto x_next = x.next_slice();
  
  auto hull = x.codomain();
  auto input_gate = x.input_gate();
  auto output_gate = x.output_gate();

  for(Index i = 0 ; i < x.size() ; i++)
  {
    hull[i] = slice_hull({0,x.t0_tf().diam()},
      x.codomain()[i],x.input_gate()[i],x.output_gate()[i],
      v.codomain()[i]
    );

    if(x_next && x_next->is_gate())
      output_gate[i] &= input_gate[i]+x.t0_tf().diam()*v.codomain()[i];

    if(x_prev && x_prev->is_gate())
      input_gate[i] &= output_gate[i]-x.t0_tf().diam()*v.codomain()[i];
  }

  if(x_prev && x_prev->is_gate())
    x_prev->set(input_gate, false);

  if(x_next && x_next->is_gate())
    x_next->set(output_gate, false);

  x.set(hull, false); // disable propagation to adjacent slices
  // v remains unchanged
}