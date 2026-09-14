/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_Zonotope.h>

using namespace std;
using namespace codac2;

TEST_CASE("Zonotope")
{
  Zonotope Z1 (Vector({2,1}), Matrix({{0.2  ,0.08}, {0.04,0.18}}));
  Zonotope Z2 (Vector({2,0.5}), Matrix({{-0.2}, {0.1}}));
  auto Zs = Z1+Z2;

  Vector c ({4,1.5});
  Matrix A ({{0.2, 0.08, -0.2},
              {0.04, 0.18, 0.1}});
  CHECK(Zs.c == c);
  CHECK(Zs.A == A);
}

