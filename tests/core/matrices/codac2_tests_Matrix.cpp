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

TEST_CASE("Matrix")
{
  CHECK_THROWS(Vector({1,2})*Vector({3,4}) == Vector({5,5}));
}