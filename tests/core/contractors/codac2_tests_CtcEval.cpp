/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_CtcEval.h>
#include <codac2_Approx.h>
#include <codac2_Figure2D.h>
#include <iomanip>

using namespace std;
using namespace codac2;

TEST_CASE("CtcEval")
{
  SECTION("Test CtcEval, 1")
  {
    auto tdomain = create_tdomain({-1,7}, 2., true);
    SlicedTube x(tdomain, Interval());
    SlicedTube v(tdomain, Interval());

    CHECK(x.nb_slices() == 9);

    v.set_ith_slice({-1}, 0*2+1);
    v.set_ith_slice({-1,1}, 1*2+1);
    v.set_ith_slice({-1}, 2*2+1);
    v.set_ith_slice({-1,1}, 3*2+1);

    CHECK(!v.is_empty());

    Interval t(0,6), z(-1,1);

    CtcEval ctc_eval;
    ctc_eval.contract(t, z, x, v);

    CHECK(x.nb_slices() == 13);

    CHECK(x(-1) == Interval());
    CHECK(x(0) == Interval(-2,7));
    CHECK(x(1) == Interval(-3,6));
    CHECK(x(3) == Interval(-4,4));
    CHECK(x(5) == Interval(-6,2));
    CHECK(x(6) == Interval(-7,2));
    CHECK(x(7) == Interval());
  }

  SECTION("Test CtcEval, 2")
  {
    ScalarVar t;
    AnalyticFunction fv({t}, -sin(t)+1e-3*Interval(-1,1));

    auto tdomain = create_tdomain({0,20},0.1,true);
    SlicedTube x(tdomain, Interval(-10,10)), v(tdomain, fv);
    x.set(1,0);
    
    CtcDeriv ctc_deriv;
    ctc_deriv.contract(x, v);

    CtcEval ctc_eval;
    SlicedTube xc(x); SlicedTube vc(v);

    IntervalVector y1({11.98,1});
    ctc_eval.contract(y1[0], y1[1], xc, vc);

    IntervalVector y2({6.5,1});
    ctc_eval.contract(y2[0], y2[1], xc, vc);

    ctc_deriv.contract(xc, vc);

    CHECK(xc(tdomain->t0_tf().lb()) == 1.);
    CHECK(xc(y1[0]) == y1[1]);
    CHECK(xc(y2[0]) == y2[1]);
    CHECK(Approx(xc(tdomain->t0_tf().ub()),1e-5) == Interval(0.294849,0.857381));
  }

  SECTION("Test CtcEval, 3, from tubeval paper")
  {
    auto tdomain = create_tdomain({-PI,PI/2},1e-2,true);
    ScalarVar t;
    AnalyticFunction fv({t}, cos(t)+Interval(-0.1,0.1));
    SlicedTube v(tdomain, fv);
    SlicedTube x = v.primitive();
    for(auto& sx : x)
      if(!sx.is_gate())
        sx.set(sx.codomain() + Interval(-0.1,0.1));
    for(auto& sx : x)
      if(sx.is_gate())
        sx.set(sx.codomain() + Interval(-0.1,0.1));

    Interval ti(-0.5,0.3);
    Interval yi(0.3,1.1);

    //DefaultFigure::plot_tube(x, {Color::light_gray(),Color::light_gray()});
    //DefaultFigure::draw_box(cart_prod(ti,yi), Color::gray());

    CtcEval ctc_eval;
    ctc_eval.contract(ti, yi, x, v);

    //CtcDeriv ctc_deriv;
    //ctc_deriv.contract(x, v);

    //DefaultFigure::plot_tube(x, {Color::dark_blue(),Color::blue()});
    //DefaultFigure::draw_box(cart_prod(ti,yi), Color::red());

    CHECK(Approx(ti,1e-5) == Interval(-0.113064,0.3));
    CHECK(Approx(yi,1e-5) == Interval(0.299999,0.749899));
    CHECK(Approx(x.codomain(),1e-5) == Interval(-1.26758,1.58623));
    CHECK(Approx(v.codomain(),1e-5) == Interval(-1.10001,1.10001));
    CHECK(Approx(ti.lb(),1e-5) == -0.113063);
    CHECK(Approx(x(ti.lb()),1e-5) == Interval(-0.149899,0.300296));
    CHECK(Approx(ti.ub(),1e-5) == 0.3);
    CHECK(Approx(x(ti.ub()),1e-5) == Interval(0.299999,0.749899));
  }

  SECTION("Test CtcEval, 4")
  {
    auto tdomain = create_tdomain({0,11},1,true);
    SlicedTube x(tdomain, Interval()), v(tdomain, Interval(-1.5,-1));

    vector<Slice<Interval>*> sx;
    for(auto& s : x)
      if(!s.is_gate())
        sx.push_back(&s);

    sx[0]->set({6,8});

    CtcEval ctc_eval;
    CtcDeriv ctc_deriv;
    ctc_deriv.contract(x, v);

    CHECK(sx[0]->codomain() == Interval(6,8)); 
    CHECK(sx[1]->codomain() == Interval(4.5,7));
    CHECK(sx[2]->codomain() == Interval(3,6));
    CHECK(sx[3]->codomain() == Interval(1.5,5));
    CHECK(sx[4]->codomain() == Interval(-0,4));
    CHECK(sx[5]->codomain() == Interval(-1.5,3));
    CHECK(sx[6]->codomain() == Interval(-3,2));
    CHECK(sx[7]->codomain() == Interval(-4.5,1));
    CHECK(sx[8]->codomain() == Interval(-6,0));
    CHECK(sx[9]->codomain() == Interval(-7.5,-1));
    CHECK(sx[10]->codomain() == Interval(-9,-2));

    Interval t, y;
    SlicedTube x_raw(x);
    SlicedTube v_raw(v);

    x = x_raw;
    v = v_raw;
    t = Interval(3.5,6);
    y = Interval(3.5,6);
    ctc_eval.contract(t, y, x, v);

    sx.clear();
    for(auto& s : x)
      if(!s.is_gate())
        sx.push_back(&s);

    CHECK(t == Interval(3.5,4.5));
    CHECK(y == Interval(3.5,4.5));
    CHECK(sx[0]->codomain() == Interval(6,8));
    CHECK(sx[1]->codomain() == Interval(4.5,7));
    CHECK(sx[2]->codomain() == Interval(3,6));
    CHECK(sx[3]->codomain() == Interval(1.5,5));
    CHECK(sx[3]->t0_tf() == Interval(3,3.5));
    CHECK(sx[4]->codomain() == Interval(2.75,4.75));
    CHECK(sx[4]->t0_tf() == Interval(3.5,4));
    CHECK(sx[5]->codomain() == Interval(2,4));
    CHECK(sx[5]->t0_tf() == Interval(4,4.5));
    CHECK(sx[6]->codomain() == Interval(0,4));
    CHECK(sx[6]->t0_tf() == Interval(4.5,5));
    CHECK(sx[7]->codomain() == Interval(-1.5,3));
    CHECK(sx[7]->t0_tf() == Interval(5,6));

    //ctc_deriv.contract(x, v);

    //DefaultFigure::plot_tube(x_raw, {Color::light_gray(),Color::light_gray()});
    //DefaultFigure::plot_tube(x, {Color::dark_blue(),Color::light_blue()});
    //DefaultFigure::draw_line(Segment({{3.5,4.5},{11,4.5-1*(11-3.5)}}), Color::purple());
    //DefaultFigure::draw_line(Segment({{3.5,3.5},{11,3.5-1.5*(11-3.5)}}), Color::purple());
    //DefaultFigure::draw_box({{3.5,6},{3.5,6}}, Color::light_gray());
    //DefaultFigure::draw_box(cart_prod(t,y), Color::red());
  }

  SECTION("Test CtcEval, 5, multi")
  {
    auto tdomain = create_tdomain({0,10*PI},1e-1,true);
    ScalarVar t;
    AnalyticFunction fv({t}, cos(t)), fx({t}, sin(t)+Interval(-0.1,0.1));
    SlicedTube v(tdomain, fv);
    SlicedTube x(tdomain, fx);

    CtcDeriv ctc_deriv;
    ctc_deriv.contract(x, v);

    //DefaultFigure::plot_tube(x, {Color::light_gray(),Color::light_gray()});

    Interval ti(11.42,23.15), yi(-1.2,-1);
    //DefaultFigure::draw_box(cart_prod(ti,yi), Color::gray());

    CtcEval ctc_eval;
    ctc_eval.contract(ti,yi,x,v);
    ctc_deriv.contract(x, v);

    //DefaultFigure::plot_tube(x, {Color::blue(),Color::light_blue()});
    //DefaultFigure::draw_box(cart_prod(ti,yi), Color::red());

    CHECK(ti == Interval(11.42,23.15));
    CHECK(Approx(yi) == Interval(-1.1,-1));
    CHECK(Approx(x(17.27),1e-5) == Interval(-1.10001,-0.897816));
  }
}