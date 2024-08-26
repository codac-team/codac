/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_CtcSegment.h>

using namespace std;
using namespace codac2;

TEST_CASE("CtcSegment")
{
  {
    IntervalVector x(2);
    CtcSegment c({{3},{-1}},{{3},{4}});

    x = IntervalVector(2);
    c.contract(x);
    CHECK(x == IntervalVector({{3,3},{-1,4}}));

    x = IntervalVector({{3.0,3.16},{2.5,3.2}});
    c.contract(x);
    CHECK(x == IntervalVector({{3,3},{2.5,3.2}}));

    x = IntervalVector({{3.02,3.16},{2.5,3.2}});
    c.contract(x);
    CHECK(x.is_empty());
  }

  {
    IntervalVector x(2);
    CtcSegment c({{-1},{-1}},{{1},{1}});

    x = IntervalVector(2);
    c.contract(x);
    CHECK(x == IntervalVector({{-1,1},{-1,1}}));

    x = IntervalVector({{-oo,oo},{0}});
    c.contract(x);
    CHECK(x == IntervalVector({{0},{0}}));

    x = IntervalVector({{0,oo},{0,oo}});
    c.contract(x);
    CHECK(x == IntervalVector({{0,1},{0,1}}));

    x = IntervalVector({{2,oo},{2,oo}});
    c.contract(x);
    CHECK(x.is_empty());
  }
}

TEST_CASE("CtcSegment - tests from Codac1")
{
  // Test_CtcSegment01
  {
    CtcSegment c({{0},{0}}, {{10},{20}});
    IntervalVector x({{-5,50},{-5, 50}});
    c.contract(x);
    CHECK(x == IntervalVector({{0,10},{0,20}}));
  }

  // Test_CtcSegment02
  {
    CtcSegment c({{0},{0}}, {{10},{20}});
    IntervalVector x({{10,50},{20,50}});
    c.contract(x);
    CHECK(x == IntervalVector({{10,10},{20,20}}));
  }

  // Test_contract_degenerate
  {
    CtcSegment c({{5},{5}}, {{5},{5}});
    IntervalVector x({{-5,50},{-5,50}});
    c.contract(x);
    CHECK(x == IntervalVector({{5,5},{5,5}}));
  }

  // Test_contract_degenerate_x
  {
    CtcSegment c({{5},{5}}, {{10},{5}});
    IntervalVector x({{-50,50},{-50,50}});
    c.contract(x);
    CHECK(x == IntervalVector({{5,10},{5,5}}));
  }

  // Test_contract_degenerate_y
  {
    CtcSegment c({{-5},{-5}}, {{-5},{15}});
    IntervalVector x({Interval(-50, 50), Interval(-50, 50)});
    c.contract(x);
    CHECK(x == IntervalVector({{-5,-5},{-5,15}}));
  }

  // Test_contract_empty
  {
    CtcSegment c({{0},{0}}, {{10},{20}});
    IntervalVector x({{-5,-2},{-5,50}});
    c.contract(x);
    CHECK(x.is_empty());
  }

  // Test_call_with_empty_x
  {
    CtcSegment c({{0},{0}}, {{10},{20}});
    IntervalVector x(2);
    c.contract(x);
    CHECK(x == IntervalVector({{0,10},{0,20}}));
  }
}