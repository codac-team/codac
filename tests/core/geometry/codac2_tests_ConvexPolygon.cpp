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
#include <codac2_ConvexPolygon.h>
#include <codac2_Figure2D.h>
#include <codac2_Approx.h>
#include <codac2_geometry.h>

using namespace std;
using namespace codac2;

TEST_CASE("ConvexPolygon - base")
{
  {
    ConvexPolygon p(vector<IntervalVector>({{1,2},{1,2}}));
    CHECK(p.vertices().size() == 1);
  }

  {
    ConvexPolygon p(vector<IntervalVector>({{1,3},{1,2},{1,2}}));
    CHECK(p.vertices().size() == 2);
  }

  {
    ConvexPolygon p(vector<IntervalVector>({{1,2},{1,3},{1,2},{1,2}}));
    CHECK(p.vertices().size() == 2);
  }

  {
    ConvexPolygon p(vector<IntervalVector>({{1,2},{1,3},{1,3},{1,2},{1,2}}));
    CHECK(p.vertices().size() == 2);
  }

  {
    CHECK(convex_hull(vector<IntervalVector>({{1,2},{1,2},{1,2}})).size() == 1);
    ConvexPolygon p(vector<IntervalVector>({{1,2},{1,2},{1,2}}));
    CHECK(p.vertices().size() == 1);
  }

  {
    ConvexPolygon p(vector<IntervalVector>({{1,2},{1,3}}));
    CHECK(p.vertices().size() == 2);
  }

  {
    ConvexPolygon p(vector<IntervalVector>({{1,2},{1,3},{1,2}}));
    CHECK(p.vertices().size() == 2);
  }

  {
    ConvexPolygon p(vector<IntervalVector>({{1,2},{1,3},{1,2},{1,2}}));
    CHECK(p.vertices().size() == 2);
  }

  {
    ConvexPolygon p(vector<IntervalVector>({{1,2},{1,3},{1,3},{1,2}}));
    CHECK(p.vertices().size() == 2);
  }
}

TEST_CASE("ConvexPolygon - degenerate cases")
{
  {
    ConvexPolygon p1({{0,4}});
    CHECK(p1.edges().size() == 1);
    CHECK(p1.edges()[0][0] == IntervalVector({0,4}));
    CHECK(p1.edges()[0][1] == IntervalVector({0,4}));
  }

  {
    ConvexPolygon p1({{0,4},{2,8}});
    CHECK(p1.edges().size() == 1);
    CHECK(p1.edges()[0][0] == IntervalVector({0,4}));
    CHECK(p1.edges()[0][1] == IntervalVector({2,8}));
    CHECK(p1 == ConvexPolygon({{2,8},{0,4}}));
  }
}

TEST_CASE("ConvexPolygon - intersection")
{
  {
    ConvexPolygon p1({{0,4},{4,8},{7,0}});
    ConvexPolygon p2({{-1,2},{-1,3},{5,3},{5,2}});

    CHECK((p1 & p2) == ConvexPolygon({
        {3.5,2},{5,2},{5,3},{1.75,3}
      }));
  }
  
  {
    ConvexPolygon p1({{3,2},{1,6},{6,5}});
    ConvexPolygon p2(IntervalVector({{0,4},{0,4}}));

    CHECK((p1 & p2) == ConvexPolygon({
        {3,2},{2,4},{4,4},{4,3}
      }));
  }

  {
    ConvexPolygon p1({{1,2},{3,4},{5,1},{2,1}});
    ConvexPolygon p2(IntervalVector({{2,6},{2,6}}));

    CHECK(Approx<Polygon>(p1 & p2) == ConvexPolygon({
        {2,2},{2,3},{3,4},{4.+1./3.,2.}
      }));
  }

  { // Big box
    ConvexPolygon p1({{1,2},{3,4},{5,1},{2,1}});
    ConvexPolygon p2(IntervalVector({{-10,10},{-10,10}}));

    CHECK((p1 & p2) == p1); // same polygon
  }

  { // Inner box
    ConvexPolygon p1({{1,2},{3,4},{5,1},{2,1}});
    ConvexPolygon p2(IntervalVector({{2.8,3},{2.8,3}}));

    CHECK((p1 & p2) == p2); // same box
  }

  {
    ConvexPolygon p1({{2,1},{3,1},{4,2},{4,3},{3,4},{2,4},{1,3},{1,2}});
    ConvexPolygon p2(IntervalVector({{1,4},{1,4}}));

    CHECK((p1 & p2) == p1); // same polygon
  }

  { // Shifted polygon points declaration
    ConvexPolygon p1({{3,4},{2,4},{1,3},{1,2},{2,1},{3,1},{4,2},{4,3}});
    ConvexPolygon p2(IntervalVector({{1,4},{1,4}}));

    CHECK((p1 & p2) == p1); // same polygon
  }

  { // Degenerate case
    ConvexPolygon p1({{4000,200}});
    ConvexPolygon p2(IntervalVector({4000,200}));

    CHECK(p1.box() == IntervalVector({4000,200}));
    CHECK(p2.box() == IntervalVector({4000,200}));
    CHECK((p1 & p2) == p1); // same polygon
  }

  {
    ConvexPolygon p1({{1,1},{2,4},{7,5},{6,2}});
    ConvexPolygon p2(IntervalVector({{2,6},{1,5}}));

    auto q = p1 & p2;
    CHECK(Approx<Polygon>(q) == Polygon({{2,1.2},{6,2},{6,4.8},{2,4}}));
    CHECK(q.edges().size() == 4);
    CHECK(q.vertices().size() == 4);
  }

  {
    ConvexPolygon p1({{1,1},{2,4},{7,5},{6,2}});
    ConvexPolygon p2(IntervalVector({{3,5},{1,5}}));

    auto q = p1 & p2;
    CHECK(Approx<Polygon>(q) == Polygon({{3,1.4},{5,1.8},{5,4.6},{3,4.2}}));
    CHECK(q.edges().size() == 4);
    CHECK(q.vertices().size() == 4);
  }

  { // Degenerated box
    ConvexPolygon p1({{1,1},{2,4},{7,5},{6,2}});
    ConvexPolygon p2(IntervalVector({{4},{1,5}}));

    CHECK(p1.edges()[0] == Segment({{1,1},{6,2}}));
    CHECK(p1.edges()[1] == Segment({{6,2},{7,5}}));
    CHECK(p1.edges()[2] == Segment({{7,5},{2,4}}));
    CHECK(p1.edges()[3] == Segment({{2,4},{1,1}}));
    CHECK(p1.edges().size() == 4);

    CHECK(p2.edges()[0] == Segment({{4,1},{4,5}}));
    CHECK(p2.edges().size() == 1);

    CHECK((p1.edges()[1] & p2.edges()[0]) == IntervalVector::empty(2));
    CHECK(Approx(p1.edges()[0] & p2.edges()[0]) == IntervalVector({4,1.6}));
    CHECK((p1.edges()[3] & p2.edges()[0]) == IntervalVector::empty(2));
    CHECK(Approx(p1.edges()[2] & p2.edges()[0]) == IntervalVector({4,4.4}));

    auto q = p1 & p2;
    CHECK(Approx<Polygon>(q) == Polygon({{4,4.4},{4,1.6}}));
    CHECK(q.vertices().size() == 2);
  }

  { // Degenerated polygon
    ConvexPolygon p1({{1,1},{2,4},{7,5},{6,2}});
    ConvexPolygon p2({{4,1},{4,5}});

    auto q = p1 & p2;
    
    CHECK(Approx<Polygon>(q) == Polygon({{4,4.4},{4,1.6}}));
    CHECK(q.vertices().size() == 2);
  }

  { // Point intersection
    ConvexPolygon p1({{1,1},{2,4},{7,5},{6,2}});
    ConvexPolygon p2({{2,4},{-4,4},{0,8}});

    auto q = p1 & p2;
    
    CHECK(q == Polygon({{2,4}}));
    CHECK(q.vertices().size() == 1);
  }

  { // Point intersection, line polygon
    ConvexPolygon p1({{1,1},{2,4},{7,5},{6,2}});
    ConvexPolygon p2({{2,4},{-4,4}});

    auto q = p1 & p2;
    
    CHECK(q == Polygon({{2,4}}));
    CHECK(q.vertices().size() == 1);
  }

  { // Empty intersection
    ConvexPolygon p1({{1,1},{2,4},{7,5},{6,2}});
    ConvexPolygon p2({{5,1.5},{8,2},{8,0},{5,0}});

    auto q = p1 & p2;
    
    CHECK(q == Polygon::empty());
    CHECK(q.vertices().size() == 0);
    CHECK(q.is_empty());
  }

  { // Empty intersection, degenerate case
    ConvexPolygon p1({{1,1},{2,4},{7,5},{6,2}});
    ConvexPolygon p2({{5,1.5},{80,2}});

    auto q = p1 & p2;
    
    CHECK(q == Polygon::empty());
    CHECK(q.vertices().size() == 0);
    CHECK(q.is_empty());
  }

  { // Intersection of empty polygons
    ConvexPolygon p1 = ConvexPolygon::empty();
    ConvexPolygon p2 = ConvexPolygon::empty();
    auto q = p1 & p2;
    CHECK(q == Polygon::empty());
    CHECK(q.is_empty());
  }

  { // Intersection of a polygon and one empty polygon
    ConvexPolygon p1({{1,1},{2,4},{7,5},{6,2}});
    ConvexPolygon p2 = ConvexPolygon::empty();
    auto q = p1 & p2;
    CHECK(q == Polygon::empty());
    CHECK(q.is_empty());
  }

  { // Intersection of a polygon and one empty polygon (line)
    ConvexPolygon p1({{5,1.5},{80,2}});
    ConvexPolygon p2 = ConvexPolygon::empty();
    auto q = p1 & p2;
    CHECK(q == Polygon::empty());
    CHECK(q.is_empty());
  }

  { // Other test with inflated points
    ConvexPolygon p1(IntervalVector({{-4,4},{-3,3}}));
    IntervalVector a1({-4,-6}), a2({-4,6});
    a1.inflate(1e-10); a2.inflate(1e-10);
    ConvexPolygon p2({Segment(a1,a2)});
    auto q = p1 & p2;
    CHECK(q.edges().size() == 1);
    CHECK(Approx(q.edges()[0][0],1e-5) == IntervalVector({-4,-3}));
    CHECK(Approx(q.edges()[0][1],1e-5) == IntervalVector({-4,3}));
  }

  { // Parallel edges
    ConvexPolygon p1({
      {-1,-10},
      {3,-10},
      {3,1},
      {-1,6},
    });
    ConvexPolygon p2({
      {-1,10},
      {-1,-1},
      {3,-6},
      {3,10},
    });
    auto q = p1 & p2;
    CHECK(q == ConvexPolygon({
      {3,-6},
      {3,1},
      {-1,6},
      {-1,-1},
    }));
  }

  { // Parallel edges towards infinity
    ConvexPolygon p1({
      {-1,next_float(-oo)},
      {3,next_float(-oo)},
      {3,1},
      {-1,6},
    });
    ConvexPolygon p2({
      {-1,prev_float(oo)},
      {-1,-1},
      {3,-6},
      {3,prev_float(oo)},
    });

    auto q = p1 & p2;
    CHECK(q == ConvexPolygon({
      {3,-6},
      {3,1},
      {-1,6},
      {-1,-1},
    }));
  }

  {
    ConvexPolygon p1({{4,3.5},{5,4},{4,4.5}});
    ConvexPolygon p2({{4,3.5},{5,3.5},{5,4.25},{4.5,4.25},{4,4}});
    //DefaultFigure::draw_polygon(p1, StyleProperties({Color::black(),Color::none()},"1e-2"));
    //DefaultFigure::draw_polygon(p2, {Color::red(),Color::none()});
    //DefaultFigure::draw_polygon(p1 & p2, {Color::none(),Color::blue(0.3)});
    CHECK(Approx(p1 & p2) == ConvexPolygon({{4,4},{4,3.5},{5,4},{4.5,4.25}}));
  }

  {
    ConvexPolygon p1({{4,4},{4,3.5},{5,4},{4.5,4.25}});
    ConvexPolygon p2(IntervalVector({{4,5},{4.1}}));
    //DefaultFigure::draw_polygon(p1, StyleProperties({Color::black(),Color::none()},"1e-2"));
    //DefaultFigure::draw_polygon(p2, {Color::red(),Color::none()});
    //DefaultFigure::draw_polygon(p1 & p2, {Color::none(),Color::blue(0.3)});
    CHECK(Approx(p1 & p2, 1e-10) == ConvexPolygon(IntervalVector({{4.2,4.8},{4.1}})));
  }

  {
    ConvexPolygon p1({{4,4},{4,3.5},{5,4},{4.5,4.25}});
    ConvexPolygon p2(IntervalVector({{4,5},Interval(41)/10}));
    //DefaultFigure::draw_polygon(p1, StyleProperties({Color::black(),Color::none()},"1e-2"));
    //DefaultFigure::draw_polygon(p2, {Color::red(),Color::none()});
    //DefaultFigure::draw_polygon(p1 & p2, {Color::none(),Color::blue(0.3)});
    CHECK(Approx(p1 & p2, 1e-10) == ConvexPolygon(IntervalVector({{4.2,4.8},Interval(41)/10})));
  }

  {
    ConvexPolygon p1(std::vector<IntervalVector>({
      IntervalVector({{0.0999999, 0.100001},{0.989016, 0.989017}}),
      IntervalVector({{0.0999999, 0.100001},{1.0015, 1.00151}}),
      IntervalVector({{0, 0},{0.999999, 1}}),
      IntervalVector({{0, 0},{1, 1.00001}})
    }));

    ConvexPolygon p2(std::vector<IntervalVector>({
      IntervalVector({{0, 0},{0.987514, 0.987515}}),
      IntervalVector({{0.1, 0.100001},{0.989016, 0.989017}}),
      IntervalVector({{0.1, 0.100001},{1.0015, 1.00151}}),
      IntervalVector({{0, 0},{1.01248, 1.01249}}),
    }));

    CHECK(Approx(p1 & p2, 1e-5) == ConvexPolygon(std::vector<IntervalVector>({
      {{0.0997289, 0.100002},{0.989016, 0.989938}},
      {{0.0995829, 0.100002},{1.00059, 1.00151}},
      {{0, 0},{0.999547, 1.00047}}
    })));

    p1 &= p2;
  }

  {
    IntervalVector m({6.5,1});
    ConvexPolygon p1(m);
    ConvexPolygon p2(std::vector<IntervalVector>({
      {{6.49999, 6.50001},{0.759358, 0.759359}},
      {{6.49999, 6.50001},{1.19382, 1.19383}},
      {{6.4, 6.40001},{1.20537, 1.20538}},
      {{6.4, 6.40001},{0.78097, 0.780971}},
    }));

    Segment s(IntervalVector({{6.49999, 6.50001},{0.759358, 0.759359}}), IntervalVector({{6.49999, 6.50001},{1.19382, 1.19383}}));
    CHECK(s.contains(m) == BoolInterval::UNKNOWN);

    //DefaultFigure::draw_polygon(p1, StyleProperties({Color::black(),Color::none()},"1e-2"));
    //DefaultFigure::draw_point(p1.box().mid(), StyleProperties({Color::black(),Color::none()},"1e-2"));
    //DefaultFigure::draw_polygon(p2, {Color::red(),Color::none()});
    //DefaultFigure::draw_polygon(p1 & p2, {Color::none(),Color::blue(0.3)});

    CHECK((p1 & p2) == ConvexPolygon(std::vector<IntervalVector>({m})));
  }
}