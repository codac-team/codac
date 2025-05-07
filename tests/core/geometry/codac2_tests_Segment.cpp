/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_Segment.h>
#include <codac2_Approx.h>

using namespace std;
using namespace codac2;

Interval hull(double x)
{
  if(x == -oo) return { -oo, next_float(-oo) };
  if(x == oo)  return { previous_float(oo), oo };
  else return { x };
}

TEST_CASE("Segment")
{
  SECTION("intersects")
  {
    CHECK(Segment({{0,0},{10,0}}).intersects(Segment({{4,0},{6,0}})) == BoolInterval::TRUE);
    CHECK(Segment({{0,0},{10,0}}).intersects(Segment({{5,0},{15,0}})) == BoolInterval::TRUE);
    CHECK(Segment({{0,0},{10,0}}).intersects(Segment({{5,-5},{5,10}})) == BoolInterval::TRUE);
    CHECK(Segment({{0,0},{10,0}}).intersects(Segment({{15,-5},{15,10}})) == BoolInterval::FALSE);
    CHECK(Segment({{0,0},{10,10}}).intersects(Segment({{1,1},{9,9}})) == BoolInterval::TRUE);
    CHECK(Segment({{1,1},{9,9}}).intersects(Segment({{0,0},{10,10}})) == BoolInterval::TRUE);

    CHECK(Segment(IntervalVector({hull(-oo),2}),Vector({0,2})).intersects(Segment(Vector({0,0}),Vector({0,1}))) == BoolInterval::FALSE);
    CHECK(Segment(IntervalVector({hull(-oo),2}),Vector({0,2})).intersects(Segment(Vector({0,0}),Vector({0,2}))) == BoolInterval::TRUE);
  }

  SECTION("contains bis")
  {
    Vector p1({0,0.}), p2({1.5,3.}), p3({0,2.});
    Segment e1(Vector({-10,-10.}), Vector({10,10.}));
    Segment e2(Vector({1.,1.}), Vector({10,10.}));
    Segment e3(Vector({0,0.}), Vector({2.,4.}));

    CHECK(e1.contains(p1) != BoolInterval::FALSE);
    CHECK(e2.contains(p1) == BoolInterval::FALSE);
    CHECK(e3.contains(p2) != BoolInterval::FALSE);
    CHECK(e3.contains(p3) == BoolInterval::FALSE);
  }

  SECTION("proj_intersection")
  {
    CHECK(proj_intersection({{0,0},{1,1}}, {{3,3},{4,4}})
      == IntervalVector(2));

    CHECK(proj_intersection({{0,0},{1,0}}, {{0,2},{1,2}})
      == IntervalVector::empty(2));

    CHECK(proj_intersection({{2,0},{6,4}},{{6,5},{5,6}})
      == Approx(IntervalVector({6.5,4.5})));

    CHECK(proj_intersection({{-0.7,0.7},{0,1}}, {{-1,0},{-0.7,-0.7}})
      == Approx(IntervalVector({-1.2069,0.4828}), 1e-4));

    CHECK(proj_intersection({{1,2},{2,3}}, {{8,4},{7,4}})
      == IntervalVector({3,4}));

    CHECK(proj_intersection({{4,11},{8,11}}, {{12,12},{12,16}})
      == IntervalVector({12,11}));

    CHECK(proj_intersection({{1,2},{1,2}}, {{8,4},{7,4}})
      == IntervalVector(2));

    CHECK(proj_intersection({{0,1},{0,2}}, {{2,1},{2,2}})
      == IntervalVector::empty(2));

    CHECK(proj_intersection({{-4,4},{0,0}}, {{0,0},{4,4}})
      == IntervalVector({0,0}));

    CHECK(proj_intersection({{-4,4},{0,0}}, {{4,4},{0,0}})
      == IntervalVector({0,0}));

    CHECK(proj_intersection({{0,4},{4,0}}, {{4,4},{0,0}})
      == IntervalVector({2,2}));
  }

  SECTION("intersection")
  {
    CHECK((Segment({0,0},{0,0}) & Segment({0,0},{5,0})) == IntervalVector({0,0})); // degenerate line, horizontal edge line
    CHECK((Segment({0,0},{0,0}) & Segment({0,0},{0,5})) == IntervalVector({0,0})); // degenerate line, vertical edge line

    CHECK((Segment({0,0},{0,1}) & Segment({0,0},{0,5.0})) == IntervalVector({0,{0,1.}})); // vertical edge line (colinear)
    CHECK((Segment({0,0},{0,1}) & Segment({0,0},{0,0.5})) == IntervalVector({0,{0,0.5}})); // vertical edge line (colinear)
    CHECK((Segment({0,0},{1,0}) & Segment({0,0},{5.0,0})) == IntervalVector({{0,1.},0})); // horizontal edge line (colinear)
    CHECK((Segment({0,0},{1,0}) & Segment({0,0},{0.5,0})) == IntervalVector({{0,0.5},0})); // horizontal edge line (colinear)

    CHECK((Segment({7,4},{7,5}) & Segment({7,6},{7,8})) == IntervalVector::empty(2)); // vertical edge line (colinear), no intersection
    CHECK((Segment({4,7},{5,7}) & Segment({6,7},{8,7})) == IntervalVector::empty(2)); // horizontal edge line (colinear), no intersection

    CHECK((Segment({0,0},{3,0}) & Segment({1,2},{1,-999})) == IntervalVector({1,0})); // perpendicular intersection
    CHECK((Segment({0.5,-1},{0.5,5}) & Segment({-3,2},{5,2})) == IntervalVector({0.5,2})); // perpendicular intersection

    CHECK((Segment({0,0},{3,0}) & Segment({1,-10},{1,-999})) == IntervalVector::empty(2)); // perpendicular lines, no intersection
    CHECK((Segment({0.5,-1},{0.5,5}) & Segment({-3,-2},{5,-2})) == IntervalVector::empty(2)); // perpendicular lines, no intersection

    CHECK((Segment({8,4},{9,2}) & Segment({7,3},{9,3})) == IntervalVector({8.5,3})); // perpendicular oblique lines
    CHECK((Segment({8,4},{9,2}) & Segment({8.5,8},{8.5,0})) == IntervalVector({8.5,3})); // perpendicular oblique lines

    CHECK((Segment({8,4},{9,2}) & Segment({7.,3},{7.5,3})) == IntervalVector::empty(2)); // secant oblique lines, no intersection
    CHECK((Segment({8,4},{9,2}) & Segment({8.5,8},{8.5,7})) == IntervalVector::empty(2)); // secant oblique lines, no intersection
    CHECK((Segment({6,-1},{8,1}) & Segment({7.5,0},{9,0})) == IntervalVector::empty(2)); // secant oblique lines, no intersection
    CHECK((Segment({6,-1},{8,1}) & Segment({6.5,0.5},{6.5,2})) == IntervalVector::empty(2)); // secant oblique lines, no intersection
  
    // Other tests
    CHECK((Segment({8,14},{6,13}) & Segment({1,1},{1,14})) == IntervalVector::empty(2));
    CHECK((Segment({hull(-oo),0},{1,0}) & Segment({0,1},{0,-1})) == IntervalVector::zero(2));
    CHECK((Segment({0,0},{1,2}) & Segment({0.5,0},{0.5,2})) == IntervalVector({0.5,1}));
    CHECK((Segment({-1,-1},{0,0}) & Segment({9,3},{0,0})) == IntervalVector({0,0}));
    CHECK((Segment({0,0},{10,0}) & Segment({2,0},{8,0})) == IntervalVector({{2,8},{0}})); // colinear case
    CHECK((Segment({0,0},{10,10}) & Segment({2,2},{8,8})) == IntervalVector({{2,8},{2,8}})); // colinear case
    CHECK((Segment({0,0},{10,0}) & Segment({2,1},{8,1})) == IntervalVector::empty(2)); // parallel case

    CHECK((Segment({2,0},{6,4}) & Segment({6,5},{5,6})) == IntervalVector::empty(2));
    CHECK(proj_intersection(Segment({2,0},{6,4}), Segment({6,5},{5,6})) == IntervalVector({6.5,4.5}));
  }
}