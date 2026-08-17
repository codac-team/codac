/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Simon Rohou
 *  \copyright  Copyright 2026 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_CtcInter.h>

using namespace std;
using namespace codac2;

TEST_CASE("CtcInter - manual")
{
  // [ctcinter-1-beg]
  CtcWrapper c1(IntervalVector({{-10,10},{-2,2}}));
  CtcWrapper c2(IntervalVector({{-12,2},{0,4}}));

  CtcInter c3(c1,c2);
  // or...
  auto c3bis = c1 & c2;
  // c3 and c3bis are a CtcInter<IntervalVector> gathering the two contractors.
  // [ctcinter-1-end]

  // [ctcinter-2-beg]
  IntervalVector x({{-oo,oo},{-oo,oo}});
  c3.contract(x);
  // x = [ [-10, 2] ; [0, 2] ]
  // [ctcinter-2-end]

  // [ctcinter-3-beg]
  CtcInter c4(2); // initially neutral conjunction on 2d boxes
  c4 &= c1;
  c4 &= c2;

  auto n = c4.nb();
  // n = 2
  // [ctcinter-3-end]
  CHECK(n == 2);
}
