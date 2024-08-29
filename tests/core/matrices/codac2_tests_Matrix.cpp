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
#include <codac2_Matrix.h>
#include <codac2_arithmetic.h>

using namespace std;
using namespace codac2;

TEST_CASE("Matrix")
{
  Matrix x({
  	{ -1, 3  },
  	{ -6, -9 }
  });

  CHECK(abs(x) == Matrix({
  	{ 1, 3 },
  	{ 6, 9 }
  }));
}