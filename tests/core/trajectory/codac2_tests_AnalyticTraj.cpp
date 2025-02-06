/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_AnalyticTraj.h>
#include <codac2_Approx.h>

using namespace std;
using namespace codac2;

double prim_value(double x)
{
  return 1.+(1./3.)*std::pow(x,3);
}

TEST_CASE("AnalyticTraj")
{
  ScalarVar t;
  AnalyticFunction f {
    {t},
    sqr(t)
  };

  AnalyticTraj traj(f, {-1,10});

  CHECK(traj.tdomain() == Interval(-1,10));
  CHECK(traj.codomain() == Interval(0,100));
  CHECK(traj.size() == 1);
  CHECK(Approx(traj(5.3),1e-3) == 28.09);
  CHECK(traj(traj.tdomain()) == traj.codomain());
  CHECK(traj({-oo,oo}) == Interval(-oo,oo));

  // Testing sampled trajectory from analytic trajectory
  auto sampled_traj = traj.sampled(0.01);
  CHECK(sampled_traj.tdomain() == Interval(-1,10));
  CHECK(Approx(sampled_traj.codomain()) == Interval(0,100));
  CHECK(sampled_traj.size() == 1);
  CHECK(Approx(sampled_traj(sampled_traj.tdomain())) == sampled_traj.codomain());
  CHECK(sampled_traj({-oo,oo}) == Interval(-oo,oo));
  CHECK(Approx(sampled_traj(0.)) == 0.);
  CHECK(Approx(sampled_traj(2.),1e-2) == 4.);
  CHECK(Approx(sampled_traj(5.),1e-2) == 25.);
  CHECK(Approx(sampled_traj(5.3),1e-2) == 28.09);
  CHECK(Approx(sampled_traj(9.),1e-2) == 81.);

  // Testing primitive computation from analytic trajectory
  double x0 = f.real_eval(-1); // == 1.
  auto sampled_prim_traj = traj.primitive(x0,0.01);
  CHECK(sampled_prim_traj.tdomain() == Interval(-1,10));
  CHECK(Approx(sampled_prim_traj.codomain(),4e-1) == Interval(prim_value(0),prim_value(10.)));
  CHECK(sampled_prim_traj(-1.) == x0);
  CHECK(Approx(sampled_prim_traj(0.),4e-1) == prim_value(0.));
  CHECK(Approx(sampled_prim_traj(2.),4e-1) == prim_value(2.));
  CHECK(Approx(sampled_prim_traj(5.),4e-1) == prim_value(5.));
  CHECK(Approx(sampled_prim_traj(5.3),4e-1) == prim_value(5.3));
  CHECK(Approx(sampled_prim_traj(9.),4e-1) == prim_value(9.));
}