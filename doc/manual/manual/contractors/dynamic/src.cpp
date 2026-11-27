/**
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Simon Rohou
 *  \copyright  Copyright 2026 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <cmath>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <codac2_CtcEval.h>
#include <codac2_CtcDeriv.h>

using namespace std;
using namespace codac2;

TEST_CASE("CtcEval - manual, scalar observation")
{
  // [ctceval-scalar-beg]
  auto td = create_tdomain({-PI,PI/2.},0.01,true);
  ScalarVar s;
  SlicedTube v(td, AnalyticFunction({s},cos(s)+Interval(-0.1,0.1)));
  auto x = v.primitive(Interval(-0.1,0.1));

  Interval t(-0.5,0.3), z(0.3,1.1);
  CtcEval ctc_eval;
  ctc_eval.contract(t,z,x,v);
  // [ctceval-scalar-end]

  CHECK(!x.is_empty());
  CHECK(t.is_subset(Interval(-0.5,0.3)));
  CHECK(z.is_subset(Interval(0.3,1.1)));
  CHECK(t.lb() > -0.5);
  CHECK(z.ub() < 1.1);
  double ref = std::sin(0.25)+0.1+0.03*0.25;
  CHECK(t.contains(0.25));
  CHECK(z.contains(ref));

  // [ctceval-propagation-beg]
  CtcDeriv ctc_deriv;
  ctc_deriv.contract(x,v);
  // [ctceval-propagation-end]

  CHECK(!x.is_empty());
  CHECK(x(0.25).contains(ref));
}

TEST_CASE("CtcEval - manual, several observations")
{
  auto td = create_tdomain({-PI,PI/2.},0.01,true);
  ScalarVar s;
  SlicedTube v(td, AnalyticFunction({s},cos(s)+Interval(-0.1,0.1)));
  auto x = v.primitive(Interval(-0.1,0.1));

  // [ctceval-multi-beg]
  vector<Interval> t = {{-0.5,0.3},{-0.6,0.8},{-2.3,-2.2}};
  vector<Interval> z = {{0.3,1.1},{-0.5,-0.4},{-0.8,-0.7}};

  CtcEval ctc_eval;
  CtcDeriv ctc_deriv;
  auto primitives = v.partial_primitives();

  for(int iter = 0 ; iter < 2 ; iter++)
  {
    for(size_t i = 0 ; i < t.size() ; i++)
      ctc_eval.contract(t[i],z[i],x,v,primitives); // Optional primitives argument for faster integration
    ctc_deriv.contract(x,v);
  }
  // [ctceval-multi-end]

  REQUIRE(!x.is_empty());
  vector<double> ref_times = {0.25,-0.55,-2.25};
  for(size_t i = 0 ; i < t.size() ; i++)
  {
    double ti = ref_times[i];
    double zi = std::sin(ti)+0.1+0.03*ti;
    CHECK(t[i].contains(ti));
    CHECK(z[i].contains(zi));
    CHECK(x(ti).contains(zi));
  }
}

TEST_CASE("CtcEval - manual, 2d localization")
{
  // [ctceval-localization-beg]
  auto td = create_tdomain({0,5},0.01,true);
  ScalarVar s;
  SlicedTube v(td, AnalyticFunction({s},{-2.*sin(s),2.*cos(2.*s)}));
  v.inflate(0.01);

  SlicedTube x(td, IntervalVector(2));
  Interval t(4.3,4.4);
  IntervalVector z({{-0.73,-0.69},{0.64,0.68}});

  CtcEval ctc_eval;
  ctc_eval.contract(t,z,x,v);
  CtcDeriv().contract(x,v);
  // [ctceval-localization-end]

  REQUIRE(!x.is_empty());
  CHECK(!x.is_unbounded());
  CHECK(t == Interval(4.3,4.4));
  CHECK(z == IntervalVector({{-0.73,-0.69},{0.64,0.68}}));
  CHECK(x(0.).contains(Vector({2.,0.})));
  CHECK(x(4.35).contains(Vector({2.*std::cos(4.35),std::sin(8.7)})));

  // [ctceval-inversion-beg]
  SlicedTube y(td, AnalyticFunction({s},{2.*cos(s),sin(2.*s)}));
  y.inflate(0.05);
  t = td->t0_tf();
  z = IntervalVector({{-1.,0.},{0.4,1.2}});

  ctc_eval.contract(t,z,y,v);
  // [ctceval-inversion-end]

  REQUIRE(!y.is_empty());
  CHECK(t.is_subset(Interval(4.,4.7)));
  CHECK(t.contains(4.35));
  CHECK(z.is_subset(IntervalVector({{-1.,0.},{0.4,1.2}})));
  CHECK(z[0].ub() < 0.);
  CHECK(z[1].ub() < 1.2);
  CHECK(z.contains(Vector({2.*std::cos(4.35),std::sin(8.7)})));
}

TEST_CASE("CtcEval - manual, known observation time")
{
  auto td = create_tdomain({0,3},1.,true);
  SlicedTube x(td, Interval()), v(td, Interval(1.));
  Interval t(1.5), z(2.5);
  CtcEval().contract(t,z,x,v);
  CHECK(x(1.5) == Interval(2.5));
  CtcDeriv().contract(x,v);

  CHECK(t == Interval(1.5));
  CHECK(z == Interval(2.5));
  CHECK(x(0.) == Interval(1.));
  CHECK(x(3.) == Interval(4.));
}
