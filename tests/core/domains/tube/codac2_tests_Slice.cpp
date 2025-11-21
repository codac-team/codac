/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <iostream>
#include <catch2/catch_test_macros.hpp>
#include <codac2_Slice.h>
#include <codac2_TDomain.h>
#include <codac2_SlicedTube.h>
#include <codac2_CtcDeriv.h>
#include <codac2_Approx.h>
#include <codac2_Figure2D.h>

using namespace std;
using namespace codac2;

TEST_CASE("Slice")
{
  SECTION("Slice inversion")
  {
    auto tdomain = create_tdomain({0,1});
    SlicedTube<Interval> x(tdomain, {0,10});
    auto sx = x.first_slice();
    x.set({2,3},0);
    x.set({5,6},1);
    CHECK(sx->invert({4,6}, 0) == Interval::empty());
    CHECK(sx->invert({2.5,6}, 0) == 0.);
    CHECK(sx->invert({0,1}, 1) == Interval::empty());
    CHECK(sx->invert({2.5,6}, 1) == 1.);
    CHECK(sx->invert({2.5,6}, {0.2,0.5}) == Interval(0.2,0.5));
    CHECK(sx->invert({2.5,6}, 0.2) == Interval(0.2));
  }

  SECTION("Slice inversion, unbounded derivative")
  {
    auto tdomain = create_tdomain({0,1});
    SlicedTube<Interval> x(tdomain, {0,10});
    SlicedTube<Interval> v(tdomain, Interval());
    auto sx = x.first_slice();
    auto sv = v.first_slice();
    CHECK(sx->invert(5., *sv, sx->t0_tf()) == Interval(0,1));
    CHECK(sx->invert(15., *sv, sx->t0_tf()) == Interval::empty());
  }

  SECTION("Slice inversion, other case")
  {
    auto tdomain = create_tdomain({6.4,6.5});
    SlicedTube<Interval> x(tdomain, {0.759,1.205});
    SlicedTube<Interval> v(tdomain, {-0.216,-0.115});
    auto sx = x.first_slice();
    auto sv = v.first_slice();

    x.set({0.781,1.205},6.4);
    x.set({0.759,1.194},6.5);

    CtcDeriv ctc_deriv;
    ctc_deriv.contract(x,v);

    CHECK(!sx->codomain().is_empty());
    CHECK(Approx(sx->codomain(),1e-4) == Interval(0.7594, 1.20501));
    CHECK(Approx(sx->output_gate(),1e-4) == Interval(0.7594, 1.19351));

    auto p = sx->polygon_slice(*sv);

    CHECK(Approx(p,1e-4) == ConvexPolygon(std::vector<IntervalVector>({
      {{6.49999, 6.5},{0.7594, 0.759401}},
      {{6.49999, 6.5},{1.1935, 1.19351}},
      {{6.4, 6.40001},{1.20499, 1.20501}},
      {{6.4, 6.40001},{0.780999, 0.781001}}
    })));

    IntervalVector pt({sx->t0_tf().ub(),1});

    CHECK(p.contains(pt) == BoolInterval::UNKNOWN);
    CHECK(p.box()[0].ub() == sx->t0_tf().ub());
    
    auto p_inter = (p&ConvexPolygon(pt));
    CHECK(p_inter == ConvexPolygon(pt));
  }
}