/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_OctaSym.h>
#include <codac2_OctaSym_operator.h>

using namespace std;
using namespace codac2;

TEST_CASE("OctaSym")
{
  OctaSym a({-2,1});
  CHECK(a.invert() == OctaSym({2,-1}));

  IntervalVector x({{-1,1},{5,6}});
  CHECK(a(x) == IntervalVector({{-6,-5},{-1,1}}));
  CHECK(a.invert()(a(x)) == x);

  OctaSym b({2,-1});
  CHECK(b.invert() == OctaSym({-2,1}));
  CHECK(b*b == OctaSym({-1,-2}));

  OctaSym c({-2,1,3});
  CHECK(c.permutation_matrix() == Matrix({{0,-1,0},{1,0,0},{0,0,1}}));
}

TEST_CASE("OctaSym as operator")
{
  OctaSym a({3,1,-2});
  VectorVar x(3);
  AnalyticFunction f({x}, a(2*x));
  CHECK(f.eval(IntervalVector({{1},{2},{3}})) == IntervalVector({{6},{2},{-4}}));
  CHECK(f.eval(IntervalVector({{-oo,oo},{-oo,oo},{-oo,oo}})) == IntervalVector(3));
  CHECK(f.eval(IntervalVector::empty(3)) == IntervalVector::empty(3));
}