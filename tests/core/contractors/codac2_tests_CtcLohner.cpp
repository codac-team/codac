/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Auguste Bourgois, Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_CtcLohner.h>
#include <codac2_Approx.h>
#include <codac2_Figure2D.h>

using namespace codac2;

TEST_CASE("CtcLohner")
{
  {
    VectorVar x(1);
    AnalyticFunction f ({x}, x);
    CtcLohner ctc(f);

    auto t = create_tdomain({0.5,1.0});
    double t_lb = t->t0_tf().lb();
    SlicedTube tube(t,IntervalVector(1));
    tube.set(IntervalVector({{1.5,1.5}}),t_lb);

    ctc.contract(tube, TimePropag::FWD);

    Interval expected = 1.5 + Interval(0., 0.5) * Interval(1., 2.);

    CHECK(tube(t->t0_tf())[0].is_superset(expected)); // legacy test
    CHECK(Approx(tube(t->t0_tf()), 1e-5) == IntervalVector({{1.36106, 2.73938}})); // new test
  }
  
  {
    VectorVar x(1);
    AnalyticFunction f({x}, {-sin(x[0])});
    CtcLohner ctc(f);

    auto t = create_tdomain({0.5,1.0});
    double t_lb = t->t0_tf().lb(), t_ub = t->t0_tf().ub();
    SlicedTube tube(t,IntervalVector(1));
    tube.set(IntervalVector({{2. * atan(exp(-t_lb) * tan(0.5))}}),t_lb);

    ctc.contract(tube, TimePropag::FWD);

    Interval expected = Interval(2. * atan(exp(-t_lb) * tan(0.5))) | Interval(2. * atan(exp(-t_ub) * tan(0.5)));

    CHECK(tube(t->t0_tf())[0].is_superset(expected)); // legacy test
    CHECK(Approx(tube(t->t0_tf()), 1e-5) == IntervalVector({{0.335538, 0.669785}})); // new test
  }


  {
    VectorVar x(1);
    AnalyticFunction f({x}, -x);
    CtcLohner ctc(f);

    auto t = create_tdomain({0.5,1.0});
    double t_lb = t->t0_tf().lb(), t_ub = t->t0_tf().ub();
    SlicedTube tube(t,IntervalVector(1));
    tube.set(IntervalVector({{exp(-t_lb)}}),t_lb);

    ctc.contract(tube, TimePropag::FWD);
    IntervalVector expected = IntervalVector({Interval(exp(-t_lb))}) | IntervalVector({Interval(exp(-t_ub))});
    
    CHECK(tube(t->t0_tf()).is_superset(expected)); // legacy test
    CHECK(Approx(tube(t->t0_tf()), 1e-5) == IntervalVector({{0.300877, 0.636858}})); // new test
  }

  {
    VectorVar x(2);
    AnalyticFunction f({x}, {-x[0],x[1]});
    CtcLohner ctc(f);

    auto t = create_tdomain({0.,1.0},0.1);
    double t_lb = t->t0_tf().lb(), t_ub = t->t0_tf().ub();
    SlicedTube tube(t,IntervalVector(2));
    tube.set(IntervalVector({exp(Interval(t_lb)),exp(Interval(t_lb))}),t_lb);

    ctc.contract(tube, TimePropag::FWD);
    IntervalVector expected_0 ({exp(t_lb),exp(t_lb)});
    CHECK(tube(t_lb).is_superset(expected_0)); // legacy test
    CHECK(Approx(tube(t_lb), 1e-5) == IntervalVector({{0.999999, 1.00001}, {0.999999, 1.00001}})); // new test
    
    IntervalVector expected_1 ({exp(-t_ub),exp(t_ub)});
    CHECK(tube(t_ub).is_superset(expected_1)); // legacy test
    CHECK(Approx(tube(t_ub), 1e-5) == IntervalVector({{0.366389, 0.368753}, {2.71268, 2.7285}})); // new test
  }

  {
    VectorVar x(2);
    AnalyticFunction f({x}, {-x[0],x[1]});
    CtcLohner ctc(f);

    auto t = create_tdomain({0.,1.0},0.1);
    double t_lb = t->t0_tf().lb(), t_ub = t->t0_tf().ub();
    SlicedTube tube(t,IntervalVector(2));
    tube.set(IntervalVector({exp(Interval(-t_ub)),exp(Interval(t_ub))}),t_ub);

    ctc.contract(tube, TimePropag::BWD);

    IntervalVector expected_0 ({exp(t_lb),exp(t_lb)});
    CHECK(tube(t_lb).is_superset(expected_0)); // legacy test
    CHECK(Approx(tube(t_lb), 1e-5) == IntervalVector({{0.997939, 1.00376}, {0.99595, 1.00238}})); // new test
    
    IntervalVector expected_1 ({exp(-t_ub),exp(t_ub)});
    CHECK(tube(t_ub).is_superset(expected_1)); // legacy test
    CHECK(Approx(tube(t_ub), 1e-5) == IntervalVector({{0.367879, 0.36788}, {2.71828, 2.71829}})); // new test
  }

  {
    VectorVar x(2);
    AnalyticFunction f({x}, {-x[0],x[1]});
    CtcLohner ctc(f);

    auto t = create_tdomain({0.,1.0},0.1);
    double t_lb = t->t0_tf().lb(), t_ub = t->t0_tf().ub();
    SlicedTube tube(t,IntervalVector(2));
    tube.set(IntervalVector({exp(Interval(-t->t0_tf().mid())),exp(Interval(t->t0_tf().mid()))}),t->t0_tf().mid());

    ctc.contract(tube);
    
    IntervalVector expected_0 ({exp(t_lb),exp(t_lb)});
    CHECK(tube(t_lb).is_superset(expected_0)); // legacy test
    CHECK(Approx(tube(t_lb), 1e-5) == IntervalVector({{0.998973, 1.00188}, {0.997977, 1.00119}})); // new test
    
    IntervalVector expected_1 ({exp(-t_ub),exp(t_ub)});
    CHECK(tube(t_ub).is_superset(expected_1)); // legacy test
    CHECK(Approx(tube(t_ub), 1e-5) == IntervalVector({{0.367135, 0.368315}, {2.71549, 2.72338}})); // new test
  }
}

TEST_CASE("Example")
{
  IntervalVector x0({Interval(1,1),Interval(1,1)});
  x0.inflate(0.1);

  auto ta =create_tdomain({0.,10.0},0.2);
  SlicedTube a(ta, IntervalVector(2));
  a.set(x0,0.0);

  auto tb =create_tdomain({0.,10.0},0.01);
  SlicedTube b(tb, IntervalVector(2));
  b.set(x0,0.0);

  VectorVar x(2);
  AnalyticFunction f({x}, {-x[0],-sin(x[1])});
  CtcLohner ctc_lohner(f);

  ctc_lohner.contract(a);
  ctc_lohner.contract(b);

  // DefaultFigure::plot_tube(a[0]);
  // DefaultFigure::plot_tube(b[0],{Color::blue(),Color::blue()});
}
