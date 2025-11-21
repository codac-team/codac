/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <iostream>
#include <catch2/catch_test_macros.hpp>
#include <codac2_Polygon.h>
#include <codac2_ConvexPolygon.h>

using namespace std;
using namespace codac2;

TEST_CASE("Polygon base")
{
  Polygon p1({{3,4}});
  CHECK(p1 == Polygon({{3,4}}));

  Polygon p2({{3,4},{1,2}});
  CHECK(p2 == Polygon({{3,4},{1,2}}));
  CHECK(p2 == Polygon({{1,2},{3,4}}));

  Polygon p3({{3,4},{1,2},{5,1}});
  CHECK(p3 == Polygon({{3,4},{1,2},{5,1}}));
  CHECK(p3 == Polygon({{1,2},{5,1},{3,4}}));
  CHECK(p3 == Polygon({{5,1},{1,2},{3,4}}));
  CHECK(p3 == Polygon({{1,2},{3,4},{5,1}}));
}

TEST_CASE("Empty polygon")
{
  Polygon p1 = Polygon::empty();
  CHECK(p1.contains(IntervalVector({1,1})) == BoolInterval::FALSE);
  CHECK(p1.contains(IntervalVector(2)) == BoolInterval::FALSE);
  CHECK(p1.is_empty());
  CHECK(p1.edges().size() == 0);
  CHECK(p1.vertices().size() == 0);
}

TEST_CASE("Polygon contains")
{
  Polygon p1({{3,-1},{3,4},{5,6},{-1,1}});
  CHECK(p1.contains({3.1,3}) == BoolInterval::FALSE);
  CHECK(p1.contains({2.9,3}) == BoolInterval::TRUE);
  CHECK(p1.contains({3,3}) == BoolInterval::TRUE);
  CHECK(p1.contains({0,1}) == BoolInterval::TRUE);
  CHECK(p1.contains({4,1}) == BoolInterval::FALSE);
  CHECK(p1.contains({2,4}) == BoolInterval::FALSE);
  CHECK(p1.contains({2.8,4}) == BoolInterval::TRUE);
  CHECK(p1.contains({3,4}) == BoolInterval::TRUE);
  CHECK(p1.contains({4,4}) == BoolInterval::FALSE);
  CHECK(p1.contains({5,6}) == BoolInterval::TRUE);
  CHECK(p1.contains({6,6}) == BoolInterval::FALSE);

  {
    Segment transect(Vector({next_float(-oo),3}), Vector({3,3}));
    Segment e1(Vector({5,6}),Vector({-1,1})), e2(Vector({3,-1}),Vector({3,4}));
    CHECK(transect.intersects(e1) == BoolInterval::TRUE);
    CHECK(transect.intersects(e2) == BoolInterval::TRUE);
  }

  Polygon p2({{0,0},{0,1},{1,1},{1,0}});
  CHECK(p2.contains({0,0}) == BoolInterval::TRUE);
  CHECK(p2.contains({1,1}) == BoolInterval::TRUE);
  CHECK(p2.contains({0,2}) == BoolInterval::FALSE);
  CHECK(p2.contains({2,0}) == BoolInterval::FALSE);
  CHECK(p2.contains({0.5,1}) == BoolInterval::TRUE);
  CHECK(p2.contains({1,0.5}) == BoolInterval::TRUE);

  {
    Segment transect(Vector({next_float(-oo),2}), Vector({0,2}));
    Segment e1(Vector({0,0}),Vector({0,1}));
    Segment e2(Vector({0,1}),Vector({1,1}));
    Segment e3(Vector({1,1}),Vector({1,0}));
    Segment e4(Vector({1,0}),Vector({0,0}));

    CHECK(transect.intersects(e1) == BoolInterval::FALSE);
    CHECK(transect.intersects(e2) == BoolInterval::FALSE);
    CHECK(transect.intersects(e3) == BoolInterval::FALSE);
    CHECK(transect.intersects(e4) == BoolInterval::FALSE);
  }

  Polygon p3({{0,1},{1,0},{0,0}});
  CHECK(p3.contains({1,1}) == BoolInterval::FALSE);
}

TEST_CASE("Polygon contains - degenerated cases")
{
  Polygon p1({{1,1}});
  CHECK(p1.contains(IntervalVector({1,1})) == BoolInterval::TRUE);
  CHECK(p1.contains(IntervalVector({2,1})) == BoolInterval::FALSE);
  CHECK(p1.contains(IntervalVector({2,2})) == BoolInterval::FALSE);
  CHECK(p1.contains(IntervalVector({1,2})) == BoolInterval::FALSE);
  CHECK(p1.contains(IntervalVector({-10,1})) == BoolInterval::FALSE);
  CHECK(p1.contains(IntervalVector({10,1})) == BoolInterval::FALSE);

  Polygon p2({{1,1},{3,2}});
  CHECK(p2.contains(IntervalVector({1,1})) == BoolInterval::TRUE);
  CHECK(p2.contains(IntervalVector({3,2})) == BoolInterval::TRUE);
  CHECK(p2.contains(IntervalVector({2,1.5})) == BoolInterval::TRUE);

  Polygon p3({{1,1},{3,1}});
  CHECK(p3.contains(IntervalVector({1,1})) == BoolInterval::TRUE);
  CHECK(p3.contains(IntervalVector({3,1})) == BoolInterval::TRUE);
  CHECK(p3.contains(IntervalVector({2,1})) == BoolInterval::TRUE);

  Polygon p4({{1,1},{1,3}});
  CHECK(p4.contains(IntervalVector({1,1})) == BoolInterval::TRUE);
  CHECK(p4.contains(IntervalVector({1,3})) == BoolInterval::TRUE);
  CHECK(p4.contains(IntervalVector({1,2})) == BoolInterval::TRUE);
}

TEST_CASE("Polygon contains - limit cases")
{
  {
    IntervalVector x({5,3.5});
    Polygon p({{4,3.5},{5,4},{4,4.5}});
    //DefaultFigure::draw_polygon(p, Color::red());
    //DefaultFigure::draw_point(x);
    CHECK(p.contains(x) == BoolInterval::FALSE);
  }

  {
    ConvexPolygon p1({{4,3.5},{5,4},{4.5,4.25},{4,4}});
    CHECK(p1.contains({4.5,4.25}) == BoolInterval::TRUE);
    ConvexPolygon p2({{4,4.25},{5,4.25}});
    auto i = p1 & p2;
    CHECK(i == ConvexPolygon(IntervalVector({4.5,4.25})));
    CHECK(i.vertices().size() == 1);
  }
}