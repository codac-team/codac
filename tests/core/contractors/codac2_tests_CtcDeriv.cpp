/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_CtcDeriv.h>
#include <codac2_SlicedTube.h>
#include <codac2_Approx.h>
#include <iomanip>

using namespace std;
using namespace codac2;

TEST_CASE("CtcDeriv")
{
  SECTION("Test slice, envelope contraction")
  {
    auto tdomain = create_tdomain({-1,3});
    SlicedTube<Interval> x(tdomain, {-10,20});
    SlicedTube<Interval> v(tdomain, {-1,1});

    auto sx = x.first_slice();
    auto sv = v.first_slice();

    CHECK(sx->t0_tf() == Interval(-1,3));
    CHECK(sx->codomain() == Interval(-10,20));
    x.set({-1,2},-1);
    x.set({-2,0},3);

    CtcDeriv ctc_deriv(TimePropag::FWD_BWD, false);
    ctc_deriv.contract(*sx, *sv);

    CHECK(sx->input_gate() == Interval(-1,2));
    CHECK(sx->output_gate() == Interval(-2,0));
    CHECK(sx->codomain() == Interval(-3.5,3));
  }

  SECTION("Test slice, output gate contraction")
  {
    auto tdomain = create_tdomain({-1,3});
    SlicedTube<Interval> x(tdomain, {-5,3});
    SlicedTube<Interval> v(tdomain, -1);

    auto sx = x.first_slice();
    auto sv = v.first_slice();

    CHECK(sx->t0_tf() == Interval(-1,3));
    CHECK(sx->codomain() == Interval(-5,3));
    x.set({-1,3},-1);
    x.set({-5,0.5},3);

    CtcDeriv ctc_deriv(TimePropag::FWD_BWD, false);
    ctc_deriv.contract(*sx, *sv);

    CHECK(sx->input_gate() == Interval(-1,3));
    CHECK(sx->output_gate() == Interval(-5,-1));
    CHECK(sx->codomain() == Interval(-5,3));
  }

  SECTION("Test slice, complete contraction (degenerate tube)")
  {
    auto tdomain = create_tdomain({-1,3});
    SlicedTube<Interval> x(tdomain, {-5,3});
    SlicedTube<Interval> v(tdomain, {-1,1});

    auto sx = x.first_slice();
    auto sv = v.first_slice();

    CHECK(sx->t0_tf() == Interval(-1,3));
    CHECK(sx->codomain() == Interval(-5,3));
    x.set({1,3},-1);
    x.set({-4,-3},3);

    CtcDeriv ctc_deriv(TimePropag::FWD_BWD, false);
    ctc_deriv.contract(*sx, *sv);

    CHECK(sx->input_gate() == Interval(1));
    CHECK(sx->output_gate() == Interval(-3));
    CHECK(sx->codomain() == Interval(-3,1));
    CHECK(sx->operator()(-1, *sv) == Interval(1));
    CHECK(sx->operator()(1, *sv) == Interval(-1));
    CHECK(sx->operator()(3, *sv) == Interval(-3));
    CHECK(sx->operator()(0, *sv) == Interval(0));
  }

  SECTION("Test slice, empty case")
  {
    auto tdomain = create_tdomain({-1,3});
    SlicedTube<Interval> x(tdomain, {-10,20});
    SlicedTube<Interval> v(tdomain, Interval::empty());

    auto sx = x.first_slice();
    auto sv = v.first_slice();

    CHECK(sx->t0_tf() == Interval(-1,3));
    CHECK(sx->codomain() == Interval(-10,20));
    x.set({-1,2},-1);
    x.set({-2,0},3);

    CtcDeriv ctc_deriv(TimePropag::FWD_BWD, false);
    ctc_deriv.contract(*sx, *sv);

    CHECK(sx->input_gate().is_empty());
    CHECK(sx->output_gate().is_empty());
    CHECK(sx->codomain().is_empty());
  }

  SECTION("Test slice, empty case (bis)")
  {
    auto tdomain = create_tdomain({-1,3});
    SlicedTube<Interval> x(tdomain, {-10,20});
    SlicedTube<Interval> v(tdomain, {-1,1});

    auto sx = x.first_slice();
    auto sv = v.first_slice();

    CHECK(sx->t0_tf() == Interval(-1,3));
    CHECK(sx->codomain() == Interval(-10,20));
    x.set({-1,2},-1);
    x.set(Interval::empty(),3);

    CtcDeriv ctc_deriv(TimePropag::FWD_BWD, false);
    ctc_deriv.contract(*sx, *sv);

    CHECK(sx->input_gate().is_empty());
    CHECK(sx->output_gate().is_empty());
    CHECK(sx->codomain().is_empty());
  }

  SECTION("Test slice, unbounded slice")
  {
    auto tdomain = create_tdomain({-1,3});
    SlicedTube<Interval> x(tdomain, Interval());
    SlicedTube<Interval> v(tdomain, {0,1});

    auto sx = x.first_slice();
    auto sv = v.first_slice();

    CHECK(sx->input_gate() == Interval());
    CHECK(sx->output_gate() == Interval());
    CHECK(sx->codomain() == Interval());

    CtcDeriv ctc_deriv(TimePropag::FWD_BWD, false);
    ctc_deriv.contract(*sx, *sv);

    CHECK(sx->input_gate() == Interval());
    CHECK(sx->output_gate() == Interval());
    CHECK(sx->codomain() == Interval());
  }

  SECTION("Test slice, unbounded derivative (1)")
  {
    auto tdomain = create_tdomain({-1,3});
    SlicedTube<Interval> x(tdomain, Interval());
    SlicedTube<Interval> v(tdomain, Interval());

    auto sx = x.first_slice();
    auto sv = v.first_slice();

    CHECK(sx->input_gate() == Interval());
    CHECK(sx->output_gate() == Interval());
    CHECK(sx->codomain() == Interval());

    CtcDeriv ctc_deriv(TimePropag::FWD_BWD, false);
    ctc_deriv.contract(*sx, *sv);

    CHECK(sx->input_gate() == Interval());
    CHECK(sx->output_gate() == Interval());
    CHECK(sx->codomain() == Interval());
  }

  SECTION("Test slice, unbounded derivative ([-oo,1])")
  {
    auto tdomain = create_tdomain({-1,3});
    SlicedTube<Interval> x(tdomain, Interval());
    SlicedTube<Interval> v(tdomain, Interval(-oo,1));

    auto sx = x.first_slice();
    auto sv = v.first_slice();

    x.set({-1,2},-1);
    x.set({-2,0},3);

    CHECK(sx->input_gate() == Interval(-1,2));
    CHECK(sx->output_gate() == Interval(-2,0));
    CHECK(sx->codomain() == Interval());
    CHECK(sv->codomain() == Interval(-oo,1));

    CtcDeriv ctc_deriv(TimePropag::FWD_BWD, false);
    ctc_deriv.contract(*sx, *sv);

    CHECK(sx->input_gate() == Interval(-1,2));
    CHECK(sx->output_gate() == Interval(-2,0));
    CHECK(sx->codomain() == Interval(-6,6));
    CHECK(sv->codomain() == Interval(-oo,1));
  }

  SECTION("Test slice, unbounded derivative ([1,oo])")
  {
    auto tdomain = create_tdomain({-1,3});
    SlicedTube<Interval> x(tdomain, Interval());
    SlicedTube<Interval> v(tdomain, Interval(-1,oo));

    auto sx = x.first_slice();
    auto sv = v.first_slice();

    x.set({-1,2},-1);
    x.set({-2,0},3);

    CHECK(sx->input_gate() == Interval(-1,2));
    CHECK(sx->output_gate() == Interval(-2,0));
    CHECK(sx->codomain() == Interval());
    CHECK(sv->codomain() == Interval(-1,oo));

    CtcDeriv ctc_deriv(TimePropag::FWD_BWD, false);
    ctc_deriv.contract(*sx, *sv);
    
    CHECK(sx->input_gate() == Interval(-1,2));
    CHECK(sx->output_gate() == Interval(-2,0));
    CHECK(sx->codomain() == Interval(-5,4));
    CHECK(sv->codomain() == Interval(-1,oo));
  }

  SECTION("Test fwd")
  {
    auto tdomain = create_tdomain({0,6},1.);
    SlicedTube<Interval> x_fwd(tdomain, Interval());
    x_fwd.set({-1,1},{0,1});
    SlicedTube<Interval> v(tdomain, Interval(-0.5,1));
    SlicedTube<Interval> x_fwdbwd(x_fwd);

    CtcDeriv ctc_deriv_fwd(TimePropag::FWD, false);
    CtcDeriv ctc_deriv_fwdbwd(TimePropag::FWD_BWD, false);

    ctc_deriv_fwd.contract(x_fwd, v);
    ctc_deriv_fwdbwd.contract(x_fwdbwd, v);
    
    CHECK(x_fwd == x_fwdbwd);

    // Checking structure
    vector<const Slice<Interval>*> sx_fwd;
    for(const auto& s : x_fwd)
      if(!s.is_gate())
        sx_fwd.push_back(&s);
    CHECK(sx_fwd[0]->codomain() == Interval(-1,1));
    CHECK(sx_fwd[1]->codomain() == Interval(-1.5,2));
    CHECK(sx_fwd[2]->codomain() == Interval(-2,3));
    CHECK(sx_fwd[3]->codomain() == Interval(-2.5,4));
    CHECK(sx_fwd[4]->codomain() == Interval(-3,5));
    CHECK(sx_fwd[5]->codomain() == Interval(-3.5,6));
    CHECK(x_fwd.codomain() == Interval(-3.5,6));
  }

  SECTION("Test bwd")
  {
    auto tdomain = create_tdomain({0,6},1.);
    SlicedTube<Interval> x_bwd(tdomain, Interval());
    x_bwd.set({-1,1},{5,6});
    SlicedTube<Interval> v(tdomain, Interval(-1,0.5));
    SlicedTube<Interval> x_fwdbwd(x_bwd);

    CtcDeriv ctc_deriv_bwd(TimePropag::BWD, false);
    CtcDeriv ctc_deriv_fwdbwd(TimePropag::FWD_BWD, false);

    ctc_deriv_bwd.contract(x_bwd, v);
    ctc_deriv_fwdbwd.contract(x_fwdbwd, v);
    
    CHECK(x_bwd == x_fwdbwd);

    // Checking structure
    vector<const Slice<Interval>*> sx_bwd;
    for(const auto& s : x_bwd)
      if(!s.is_gate())
        sx_bwd.push_back(&s);
    CHECK(sx_bwd[0]->codomain() == Interval(-3.5,6));
    CHECK(sx_bwd[1]->codomain() == Interval(-3,5));
    CHECK(sx_bwd[2]->codomain() == Interval(-2.5,4));
    CHECK(sx_bwd[3]->codomain() == Interval(-2,3));
    CHECK(sx_bwd[4]->codomain() == Interval(-1.5,2));
    CHECK(sx_bwd[5]->codomain() == Interval(-1,1));
    CHECK(x_bwd.codomain() == Interval(-3.5,6));
  }

  SECTION("Test fwd/bwd")
  {
    auto tdomain = create_tdomain({0,6},1.);
    SlicedTube<Interval> x(tdomain, Interval());
    x.set({-1,1},{0,1});
    x.set({-1,1},{5,6});
    SlicedTube<Interval> v(tdomain, Interval(-1,0.5));

    CtcDeriv ctc_deriv(TimePropag::FWD_BWD, false);
    ctc_deriv.contract(x, v);

    // Checking structure
    vector<const Slice<Interval>*> sx;
    for(const auto& s : x)
      if(!s.is_gate())
        sx.push_back(&s);
    CHECK(sx[0]->codomain() == Interval(-1,1));
    CHECK(sx[1]->codomain() == Interval(-2,1.5));
    CHECK(Approx(sx[2]->codomain()) == Interval(-7./3.,2.));
    CHECK(Approx(sx[3]->codomain()) == Interval(-2.,7./3.));
    CHECK(sx[4]->codomain() == Interval(-1.5,2.));
    CHECK(sx[5]->codomain() == Interval(-1,1));
    CHECK(Approx(x.codomain()) == Interval(-7./3.,7./3.));
  }

  SECTION("Test fwd/bwd (example from tubint paper)")
  {
    auto tdomain = create_tdomain({0,5},1.);
    SlicedTube<Interval> x(tdomain, Interval());
    x.set({0},0);
    x.set({4},5);
    SlicedTube<Interval> v(tdomain, Interval());
    v.set({1,2},{0,1});
    v.set({0.5,1.5},{1,2});
    v.set({0,0.5},{2,3});
    v.set({0},{3,4});
    v.set({-0.5,0.5},{4,5});

    CtcDeriv ctc_deriv(TimePropag::FWD_BWD, false);
    ctc_deriv.contract(x, v);

    // Checking structure
    vector<const Slice<Interval>*> sx;
    for(const auto& s : x)
      if(!s.is_gate())
        sx.push_back(&s);
    CHECK(sx[0]->codomain() == Interval(0,2));
    CHECK(sx[1]->codomain() == Interval(1.5,3.5));
    CHECK(sx[2]->codomain() == Interval(3,4));
    CHECK(sx[3]->codomain() == Interval(3.5,4));
    CHECK(sx[4]->codomain() == Interval(3.5,4.25));
    CHECK(x.codomain() == Interval(0,4.25));
  }

  SECTION("Test fwd/bwd (other example)")
  {
    auto tdomain = create_tdomain({0,26});
    SlicedTube<Interval> x(tdomain, {-1,7});
    x.set({2,3},0);
    x.set({3,4},4);
    x.set({1},8);
    x.set({1},12);
    x.set({5.5},14);
    x.set({5.5},16);
    x.set({-1},18);
    x.set({5.5},20);
    x.set({5.5},23);
    x.set({5.5},23); // setting gate again
    SlicedTube<Interval> v(tdomain, {-1,1});

    vector<Slice<Interval>*> sx, sv;
    for(auto& s : x)
      if(!s.is_gate())
        sx.push_back(&s);
    for(auto& s : v)
      if(!s.is_gate())
        sv.push_back(&s);

    sx[8]->set({-oo,oo});

    sv[0]->set({-1.5,4});
    sv[1]->set({-0.75,-0.5});
    sv[2]->set({-1./3,1});
    sv[3]->set({4.5/2});
    sv[4]->set({0});
    sv[5]->set({-oo,-6.5/2});
    sv[6]->set({6.5/2,oo});
    sv[7]->set({-oo,oo});
    sv[8]->set({-oo,oo});

    CHECK(sx.size() == 9);
    CHECK(sv.size() == 9);
    CHECK(x.nb_slices() == 18);
    CHECK(v.nb_slices() == 18);

    CtcDeriv ctc_deriv(TimePropag::FWD_BWD, false);
    ctc_deriv.contract(x, v);

    CHECK(sx[0]->codomain() == Interval(-1,7));
    CHECK(sx[1]->codomain() == Interval(1,4));
    CHECK(Approx(sx[2]->codomain()) == Interval(0,2));
    CHECK(sx[3]->codomain() == Interval(1,5.5));
    CHECK(sx[4]->codomain() == Interval(5.5));
    CHECK(x.codomain() == Interval(-oo,oo));

    ctc_deriv.contract(*sx[0], *sv[0]);
    ctc_deriv.contract(*sx[1], *sv[1]);
    ctc_deriv.contract(*sx[2], *sv[2]);
    ctc_deriv.contract(*sx[3], *sv[3]);
    ctc_deriv.contract(*sx[4], *sv[4]);
    ctc_deriv.contract(*sx[5], *sv[5]);
    ctc_deriv.contract(*sx[6], *sv[6]);
    ctc_deriv.contract(*sx[7], *sv[7]);
    ctc_deriv.contract(*sx[8], *sv[8]);

    CHECK(x(0.) == Interval(2,3));
    CHECK(sx[0]->codomain() == Interval(-1,7));
    CHECK(x(4.) == Interval(3,4));
    CHECK(sx[1]->codomain() == Interval(1,4));
    CHECK(x(8.) == Interval(1.));
    CHECK(Approx(sx[2]->codomain()) == Interval(0,2));
    CHECK(x(12.) == Interval(1));
    CHECK(sx[3]->codomain() == Interval(1,5.5));
    CHECK(x(14.) == Interval(5.5));
    CHECK(sx[4]->codomain() == Interval(5.5));
    CHECK(x(16.) == Interval(5.5));
    CHECK(sx[5]->codomain() == Interval(-1,5.5));
    CHECK(x(18.) == Interval(-1));
    CHECK(sx[6]->codomain() == Interval(-1,5.5));
    CHECK(x(20.) == Interval(5.5));
    CHECK(sx[7]->codomain() == Interval(-1,7));
    CHECK(x(23.) == Interval(5.5));

    CHECK(sx[8]->codomain() == Interval());
    CHECK(x(26.) == Interval());
    CHECK(x.codomain() == Interval());
  }

  SECTION("From: Test slice, output gate contraction")
  {
    auto tdomain = create_tdomain({-1,3});
    SlicedTube<Interval> x(tdomain, {-5,3});
    SlicedTube<Interval> v(tdomain, {-1,-1});

    auto sx = x.first_slice();
    auto sv = v.first_slice();

    x.set({-1,3},-1);
    x.set({-5,0.5},3);

    CtcDeriv ctc_deriv(TimePropag::FWD_BWD, false);
    ctc_deriv.contract(x,v);

    CHECK(sx->input_gate() == Interval(-1,3));
    CHECK(sx->output_gate() == Interval(-5,-1));
    CHECK(sx->operator()(Interval(2.), *sv) == Interval(-4,0));
    CHECK(sx->operator()(Interval(-1,3), *sv) == Interval(-5,3));
  }

  SECTION("From: Test slice, complete contraction (degenerate tube)")
  {
    auto tdomain = create_tdomain({-1,3});
    SlicedTube<Interval> x(tdomain, {-5,3});
    SlicedTube<Interval> v(tdomain, {-1,1});

    auto sx = x.first_slice();
    auto sv = v.first_slice();

    x.set({1,3},-1);
    x.set({-4,-3},3);

    CtcDeriv ctc_deriv(TimePropag::FWD_BWD, false);
    ctc_deriv.contract(x,v);

    CHECK(sx->operator()(Interval(0.5,2), *sv) == Interval(-2,-0.5));
    CHECK(sx->operator()(Interval(1), *sv) == Interval(-1));
    CHECK(sx->operator()(Interval(-1,3), *sv) == Interval(-3,1));
  }
}