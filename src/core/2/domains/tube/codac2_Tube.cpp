/** 
 *  \file
 *  
 * ----------------------------------------------------------------------------
 *  \date       2022
 *  \author     Simon Rohou
 *  \copyright  Copyright 2022 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac2_Tube.h"
#include "codac2_Interval.h"
#include "codac_polygon_arithmetic.h"
#include "codac_Tube.h" // to be removed
#include "codac_TubeVector.h" // to be removed

using namespace std;

namespace codac2
{
  codac::Tube to_codac1(const Tube<Interval>& x)
  {
    codac::Tube x_(x.t0_tf());
    for(const auto& s : x)
      if(!s.t0_tf().is_unbounded()) // temporaly unbounded tslices not supported in codac1
        x_.set(s.codomain(), s.t0_tf());
    for(const auto& s : x) // setting gate (were overwritten)
      if(s.t0_tf().is_degenerated())
        x_.set(s.codomain(), s.t0_tf());
    return x_;
  }

  codac::TubeVector to_codac1(const Tube<IntervalVector>& x)
  {
    codac::TubeVector x_(x.t0_tf(), x.size());
    for(const auto& s : x)
      if(!s.t0_tf().is_unbounded()) // temporaly unbounded tslices not supported in codac1
        x_.set(to_codac1<>(s.codomain()), static_cast<ibex::Interval>(s.t0_tf()));
    for(const auto& s : x) // setting gate (were overwritten)
      if(s.t0_tf().is_degenerated())
        x_.set(to_codac1<>(s.codomain()), static_cast<ibex::Interval>(s.t0_tf()));
    return x_;
  }

  codac::TubeVector to_codac1_poly(const Tube<codac::ConvexPolygon>& x)
  {
    codac::TubeVector x_(x.t0_tf(), x.size());
    for(const auto& s : x)
      if(!s.t0_tf().is_unbounded()) // temporaly unbounded tslices not supported in codac1
        x_.set(s.codomain().box(), s.t0_tf());
    for(const auto& s : x) // setting gate (were overwritten)
      if(s.t0_tf().is_degenerated())
        x_.set(s.codomain().box(), s.t0_tf());
    return x_;
  }

  Tube<Interval> to_codac2(const codac::Tube& x_)
  {
    auto tdomain = create_tdomain(x_.tdomain());
    for(const codac::Slice *s = x_.first_slice() ; s != nullptr ; s=s->next_slice())
      tdomain->sample(s->tdomain().lb(), true); // with gate
    tdomain->sample(x_.tdomain().ub(), true); // with gate
    Tube<Interval> x(tdomain, codac::Interval());
    for(auto& s : x) // includes gates
      s.set(x_(s.t0_tf()));
    return x;
  }

  Tube<IntervalVector> to_codac2(const codac::TubeVector& x_)
  {
    auto tdomain = create_tdomain(x_[0].tdomain());
    for(const codac::Slice *s = x_[0].first_slice() ; s != nullptr ; s=s->next_slice())
      tdomain->sample(s->tdomain().lb(), true); // with gate
    tdomain->sample(x_[0].tdomain().ub(), true); // with gate
    Tube<IntervalVector> x(tdomain, IntervalVector(x_.size()));
    for(auto& s : x) // includes gates
      s.set(to_codac2<>(x_(s.t0_tf())));
    return x;
  }

  Tube<codac::ConvexPolygon> to_codac2_poly(const codac::TubeVector& x_)
  {
    assert(x_.size() == 2);
    auto tdomain = create_tdomain(x_[0].tdomain());
    for(const codac::Slice *s = x_[0].first_slice() ; s != nullptr ; s=s->next_slice())
      tdomain->sample(s->tdomain().lb(), true); // with gate
    tdomain->sample(x_[0].tdomain().ub(), true); // with gate
    Tube<codac::ConvexPolygon> x(tdomain, codac::ConvexPolygon());
    for(auto& s : x) // includes gates
      s.set(codac::ConvexPolygon(x_(s.t0_tf())));
    return x;
  }

  template <>
  Tube<IntervalVector>::Tube(const std::shared_ptr<TDomain>& tdomain, const TFnc& f) :
    Tube(tdomain, IntervalVector(f.image_dim()))
  {
    assert(f.nb_var() == 0 && "function's inputs must be limited to system variable");

    for(auto& s : *this)
      s.set(to_codac2(f.eval_vector(s.t0_tf())));
  }
} // namespace codac