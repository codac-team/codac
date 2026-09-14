/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_CtcWrapper.h>
#include <codac2_Approx.h>

#include <codac2_Figure2D.h>

using namespace std;
using namespace codac2;

TEST_CASE("CtcWrapper - Parallelepiped - manual")
{
  {
    // [ctcparallelepiped-1-beg]
    IntervalVector x ({{0,5}, {0,5}});
    Parallelepiped p (Vector({1.5,2.8}), Matrix({{0.5,0.4},{0,0.2}}));
    CtcWrapper c(p);
    c.contract(x);
    // x = [ [0.599999, 2.40001] ; [2.59999, 3] ]
    // [ctcparallelepiped-1-end]

    CHECK(Approx(x) == IntervalVector({{0.6, 2.4}, {2.6, 3}}));
  }
}
