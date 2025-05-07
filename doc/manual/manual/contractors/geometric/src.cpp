/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_CtcDist.h>
#include <codac2_CtcProj.h>
#include <codac2_Approx.h>
#include <codac2_cart_prod.h>
#include <codac2_CtcInter.h>
#include <codac2_drawwhilepaving.h>
#include <codac2_math.h>

using namespace std;
using namespace codac2;

TEST_CASE("CtcDist - manual")
{
  {
    // [ctcdist-1-beg]
    IntervalVector x({{2,5},{2,6},{0,0},{0,0},{1,3}});
    CtcDist c;
    c.contract(x);
    // x = [ [2, 2.23607] ; [2, 2.23607] ; <0, 0> ; <0, 0> ; [2.82842, 3] ]
    // [ctcdist-1-end]
  }

  {
    // [ctcdist-2-beg]
    Interval a1(2,5), a2(2,6), b1(0), b2(0), d(1,3);
    CtcDist c;
    c.contract(a1,a2,b1,b2,d);
    // a1 = [2, 2.23607] ; a2 = [2, 2.23607] ; b1 = <0, 0> ; b2 = <0, 0> ; d = [2.82842, 3]
    // [ctcdist-2-end]
  }

  {
    // [ctcdist-3-beg]
    Vector b1({1,2}), b2({3.6,2.4});
    Interval y1(2,2.4), y2(4.1,4.5);

    CtcDist c; // generic distance contractor
    CtcProj c1(c, {0,1}, cart_prod(b1,y1)); // projection involving data
    CtcProj c2(c, {0,1}, cart_prod(b2,y2));
    // [ctcdist-3-end]

    if(false) // graphic outputs is disabled for tests
    {
      // [ctcdist-4-beg]
      draw_while_paving( // calling a paver algorithm for graphic output
        {{-3,5},{-2.5,5.5}}, // initial 2d box
        c1 & c2, // intersection of the two projected contractors
        0.1 // paver precision
      );

      DefaultFigure::draw_ring(b1, y1);
      DefaultFigure::draw_ring(b2, y2);
      DefaultFigure::draw_circle({0,0}, 0.1, {Color::red(),Color::red()});
      // [ctcdist-4-end]
    }
  }
}