/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_SepTest.h>
#include <codac2_Approx.h>
#include <codac2_SepWrapper.h>

using namespace std;
using namespace codac2;


TEST_CASE("SepTest")
{
  IntervalVector X ({{1,3},{2,8},{-1,1}});
  SepWrapper<IntervalVector> sep_wrapper (X);
  SepTest sep_test (sep_wrapper);

  IntervalVector x1 ({{1.5,2.5},{2.5,7.5},{-0.5,0.5}});
  IntervalVector x2 ({{10,11},{10,11},{10,11}});
  IntervalVector x3 ({{1,3},{2,8},{-1,1}});

  BoxPair xs = sep_test.separate(x1);
  CHECK(xs.inner==IntervalVector::empty(3));
  CHECK(xs.outer==x1);

  xs = sep_test.separate(x2);
  CHECK(xs.inner==x2);
  CHECK(xs.outer==IntervalVector::empty(3));

  xs = sep_test.separate(x3);
  CHECK(xs.inner==x3);
  CHECK(xs.outer==x3);
}