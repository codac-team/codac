/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_Parallelepiped.h>

using namespace std;
using namespace codac2;

TEST_CASE("Parallelepiped")
{
  Parallelepiped p(Vector({0,2,4}), Matrix({{0.5,0.,0.},{0.,1.,0.},{0.,1.,1.}}));
  CHECK(p.box() == IntervalVector({{-0.5,0.5},{1.,3.},{2.,6.}}));
  CHECK(p.contains(Vector({0.1,2.1,4.1})));

  Zonotope z = p.proj({2,1,0});
  CHECK(z.z == Vector({4,2,0}));
  CHECK(z.A == Matrix({{0.,1.,1.},{0.,1.,0.},{0.5,0.,0.}}));
}

