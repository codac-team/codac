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
    CtcSegment c({{{3},{-1}},{{3},{4}}});

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
    CtcSegment c({{{-1},{-1}},{{1},{1}}});

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