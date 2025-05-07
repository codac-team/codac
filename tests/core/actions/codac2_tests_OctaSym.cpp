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