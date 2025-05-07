/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_SampledTraj.h>
#include <codac2_Traj_operator.h>
#include <codac2_SampledTraj_operations.h>
#include <codac2_Approx.h>
#include <codac2_Figure2D.h>
#include <codac2_math.h>

using namespace std;
using namespace codac2;

TEST_CASE("SampledTraj")
{
  SampledTraj<Vector> x({
    { 0.25, {-0.5,0.5} },
    { 1., {0,0} },
    { 2., {1,0} },
    { 3., {1,1} },
    { 4., {-1,1} },
    { 5., {-1,-1} },
    { 6., {2,-1} }
  });

  CHECK(x.tdomain() == Interval(0.25,6));
  CHECK(x.size() == 2);
  CHECK(x.nb_samples() == 7);
  CHECK(!x.is_empty());
  CHECK(x.codomain() == IntervalVector({{-1,2},{-1,1}}));
  CHECK(x(0.25) == Vector({-0.5,0.5}));
  CHECK(x(1.) == Vector({0,0}));
  CHECK(x(6.) == Vector({2,-1}));
  // Interpolations:
  CHECK(Approx(x(0.75)) == Vector({-1/6.,1/6.}));
  CHECK(x(1.5) == Vector({0.5,0}));
  CHECK(x(5.5) == Vector({0.5,-1}));
  CHECK(x(Interval(1,4)) == IntervalVector({{-1,1},{0,1}}));

  auto x_sampled = x.sampled(0.1);
  CHECK(x_sampled.tdomain() == Interval(0.25,6));
  CHECK(x_sampled.size() == 2);
  CHECK(x_sampled.nb_samples() > 5*x.nb_samples()); // approx
  CHECK(!x_sampled.is_empty());
  CHECK(x_sampled.codomain() == IntervalVector({{-1,2},{-1,1}}));
  CHECK(x_sampled(0.25) == Vector({-0.5,0.5}));
  CHECK(x_sampled(1.) == Vector({0,0}));
  CHECK(x_sampled(6.) == Vector({2,-1}));
  // Interpolations:
  CHECK(Approx(x_sampled(0.75)) == Vector({-1/6.,1/6.}));
  CHECK(Approx(x_sampled(1.5)) == Vector({0.5,0}));
  CHECK(Approx(x_sampled(5.5)) == Vector({0.5,-1}));
  CHECK(x_sampled(Interval(1,4)) == IntervalVector({{-1,1},{0,1}}));

  //DefaultFigure::set_window_properties({75,75},{700,700});
  //DefaultFigure::draw_trajectory(x, Color::blue());
  //DefaultFigure::draw_trajectory(x_sampled, Color::red());
}

TEST_CASE("SampledTraj as operator (1d case)")
{
  ScalarVar t;
  AnalyticFunction f { {t}, cos(t) };
  AnalyticTraj analytic_traj(f, {-PI,PI});
  auto sampled_traj = analytic_traj.sampled(1e-2);
  auto g = sampled_traj.as_function();

  AnalyticFunction h { {t}, g(t) };

  for(double t = -PI ; t < PI ; t+=1e-2)
    CHECK(Approx(h.real_eval(t),1e-8) == cos(t));
}

TEST_CASE("SampledTraj as operator (nd case)")
{
  ScalarVar t;
  AnalyticFunction f {
    {t},
    vec(2*cos(t),sin(2*t))
  };

  auto analytic_traj = AnalyticTraj(f, {0,5});
  auto sampled_traj = analytic_traj.sampled(1e-2);
  auto g = sampled_traj.as_function();

  {
    AnalyticFunction h {
      {t},
      g(t)
    };

    for(double t = 0 ; t < 5 ; t+=1e-2)
      CHECK(Approx(h.real_eval(t),1e-8) == Vector({2*cos(t),sin(2*t)}));
  }
  {
    AnalyticFunction h {
      {t},
      { g(t)[0],g(t)[1] }
    };

    for(double t = 0 ; t < 5 ; t+=1e-2)
      CHECK(Approx(h.real_eval(t),1e-8) == Vector({2*cos(t),sin(2*t)}));
  }
}

TEST_CASE("SampledTraj: operations")
{
  ScalarVar t;
  AnalyticFunction h { {t}, t };
  auto analytic_traj = AnalyticTraj(h, {-PI,PI});
  SampledTraj x = analytic_traj.sampled(1e-2);
  CHECK(Approx(cos(x).codomain(),1e-5) == Interval(-1,1));
  x = cos(x) + 4.;
  x -= 42.;
  CHECK(Approx(x.codomain(),1e-5) == Interval(-1,1)+4.-42.);
}