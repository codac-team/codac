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
    CHECK(p.unsorted_vertices().size() == 1);
  }

  {
    ConvexPolygon p(vector<IntervalVector>({{1,3},{1,2},{1,2}}));
    CHECK(p.unsorted_vertices().size() == 2);
  }

  {
    ConvexPolygon p(vector<IntervalVector>({{1,2},{1,3},{1,2},{1,2}}));
    CHECK(p.unsorted_vertices().size() == 2);
  }

  {
    ConvexPolygon p(vector<IntervalVector>({{1,2},{1,3},{1,3},{1,2},{1,2}}));
    CHECK(p.unsorted_vertices().size() == 2);
  }

  {
    CHECK(convex_hull(vector<IntervalVector>({{1,2},{1,2},{1,2}})).size() == 1);
    ConvexPolygon p(vector<IntervalVector>({{1,2},{1,2},{1,2}}));
    CHECK(p.unsorted_vertices().size() == 1);
  }

  {
    ConvexPolygon p(vector<IntervalVector>({{1,2},{1,3}}));
    CHECK(p.unsorted_vertices().size() == 2);
  }

  {
    ConvexPolygon p(vector<IntervalVector>({{1,2},{1,3},{1,2}}));
    CHECK(p.unsorted_vertices().size() == 2);
  }

  {
    ConvexPolygon p(vector<IntervalVector>({{1,2},{1,3},{1,2},{1,2}}));
    CHECK(p.unsorted_vertices().size() == 2);
  }

  {
    ConvexPolygon p(vector<IntervalVector>({{1,2},{1,3},{1,3},{1,2}}));
    CHECK(p.unsorted_vertices().size() == 2);
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

    CHECK((p1 & p2) == p1); // same polygon
  }

  {
    ConvexPolygon p1({{1,1},{2,4},{7,5},{6,2}});
    ConvexPolygon p2(IntervalVector({{2,6},{1,5}}));

    auto q = p1 & p2;
    CHECK(Approx<Polygon>(q) == Polygon({{2,1.2},{6,2},{6,4.8},{2,4}}));
    CHECK(q.edges().size() == 4);
    CHECK(q.unsorted_vertices().size() == 4);
  }

  {
    ConvexPolygon p1({{1,1},{2,4},{7,5},{6,2}});
    ConvexPolygon p2(IntervalVector({{3,5},{1,5}}));

    auto q = p1 & p2;
    CHECK(Approx<Polygon>(q) == Polygon({{3,1.4},{5,1.8},{5,4.6},{3,4.2}}));
    CHECK(q.edges().size() == 4);
    CHECK(q.unsorted_vertices().size() == 4);
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
    CHECK(q.unsorted_vertices().size() == 2);
  }

  { // Degenerated polygon
    ConvexPolygon p1({{1,1},{2,4},{7,5},{6,2}});
    ConvexPolygon p2({{4,1},{4,5}});

    auto q = p1 & p2;
    
    CHECK(Approx<Polygon>(q) == Polygon({{4,4.4},{4,1.6}}));
    CHECK(q.unsorted_vertices().size() == 2);
  }

  { // Point intersection
    ConvexPolygon p1({{1,1},{2,4},{7,5},{6,2}});
    ConvexPolygon p2({{2,4},{-4,4},{0,8}});

    auto q = p1 & p2;
    
    CHECK(q == Polygon({{2,4}}));
    CHECK(q.unsorted_vertices().size() == 1);
  }

  { // Point intersection, line polygon
    ConvexPolygon p1({{1,1},{2,4},{7,5},{6,2}});
    ConvexPolygon p2({{2,4},{-4,4}});

    auto q = p1 & p2;
    
    CHECK(q == Polygon({{2,4}}));
    CHECK(q.unsorted_vertices().size() == 1);
  }

  { // Empty intersection
    ConvexPolygon p1({{1,1},{2,4},{7,5},{6,2}});
    ConvexPolygon p2({{5,1.5},{8,2},{8,0},{5,0}});

    auto q = p1 & p2;
    
    CHECK(q == Polygon::empty());
    CHECK(q.unsorted_vertices().size() == 0);
    CHECK(q.is_empty());
  }

  { // Empty intersection, degenerate case
    ConvexPolygon p1({{1,1},{2,4},{7,5},{6,2}});
    ConvexPolygon p2({{5,1.5},{80,2}});

    auto q = p1 & p2;
    
    CHECK(q == Polygon::empty());
    CHECK(q.unsorted_vertices().size() == 0);
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
}