/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_AnalyticFunction.h>
#include <codac2_analytic_operations.h>

using namespace std;
using namespace codac2;

TEST_CASE("AnalyticFunction")
{
  {
    VectorVar x(2);
    AnalyticFunction f({x}, x[0]*(x[0]+x[1])+sqr(x[1]));
    CHECK(f.diff(Vector({2,3}))(0,0) == 7);
    CHECK(f.diff(Vector({2,3}))(0,1) == 8);
  }
  {
    ScalarVar x,y;
    AnalyticFunction f({x,y}, x*(x+y)+sqr(y));
    CHECK(f.diff(Interval(2),Interval(3))(0,0) == 7);
    CHECK(f.diff(Interval(2),Interval(3))(0,1) == 8);
  }
}