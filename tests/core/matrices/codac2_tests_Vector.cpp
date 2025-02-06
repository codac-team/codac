/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_Vector.h>

using namespace std;
using namespace codac2;

TEST_CASE("Vector")
{
  size_t i = 0;
  Vector a(3), b {{-1},{2},{-3}};
  for(const auto& bi : b)
    a[i++] = bi;
  CHECK(a == b);
}