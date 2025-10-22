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
#include <codac2_Slice.h>
#include <codac2_CtcDeriv.h>
#include <codac2_TDomain.h>
#include <codac2_SlicedTube.h>
#include <codac2_Approx.h>
#include <codac2_Figure2D.h>

using namespace std;
using namespace codac2;

TEST_CASE("polygon_slice")
{
  SECTION("Polygons from Slice, test 1")
  {
    auto tdomain = create_tdomain({-1,3});
    SlicedTube<Interval> x(tdomain, Interval());
    SlicedTube<Interval> v(tdomain, {-1,1});

    auto sx = x.first_slice();
    auto sv = v.first_slice();

    CHECK(sx->t0_tf() == Interval(-1,3));
    CHECK(sx->codomain() == Interval());
    x.set({-1,2},-1);
    x.set({-2,0},3);

    CtcDeriv ctc_deriv;
    ctc_deriv.contract(*sx, *sv);

    ConvexPolygon p1 = sx->polygon_slice(*sv);
    CHECK(p1.box()[0] == Interval(-1.,3.));
    CHECK(p1.box()[1] == Interval(-3.5,3.));

    vector<Vector> v_pts {
      {-1,-1},{-1,2},{0,3},{3,0},{3,-2},{1.5,-3.5}
    };
    ConvexPolygon p2(v_pts);

    CHECK(Approx(p1) == p2);
  }

  SECTION("Polygons from Slice, test 2")
  {
    auto tdomain = create_tdomain({-1,3});
    SlicedTube<Interval> x(tdomain, {-5,3});
    SlicedTube<Interval> v(tdomain, {-1});

    auto sx = x.first_slice();
    auto sv = v.first_slice();

    x.set({-1,3},-1);
    x.set({-5,0.5},3);

    CtcDeriv ctc_deriv;
    ctc_deriv.contract(*sx, *sv);

    ConvexPolygon p1 = sx->polygon_slice(*sv);
    CHECK(p1.box()[0] == Interval(-1.,3.));
    CHECK(p1.box()[1] == Interval(-5,3.));

    vector<Vector> v_pts {
      {-1,-1},{-1,3},{3,-1},{3,-5},{-1,1}
    };
    ConvexPolygon p2(v_pts);

    CHECK(Approx(p1) == p2);
  }

  SECTION("Polygons from Slice, test 3, degenerate case")
  {
    auto tdomain = create_tdomain({-1,3});
    SlicedTube<Interval> x(tdomain, {-5,3});
    SlicedTube<Interval> v(tdomain, {-1,1});

    auto sx = x.first_slice();
    auto sv = v.first_slice();

    x.set({1,3},-1);
    x.set({-4,-3},3);

    CtcDeriv ctc_deriv;
    ctc_deriv.contract(*sx, *sv);

    ConvexPolygon p1 = sx->polygon_slice(*sv);
    CHECK(p1.box()[0] == Interval(-1.,3.));
    CHECK(p1.box()[1] == Interval(-3,1.));

    //DefaultFigure::draw_polygon(p1);

    vector<Vector> v_pts {
      {-1,1},{3,-3}
    };
    ConvexPolygon p2(v_pts);

    CHECK(Approx(p1) == p2);
    CHECK(p1.size() == 1);
  }

  SECTION("Polygons from Slice, test 4")
  {
    auto tdomain = create_tdomain({0,4});
    SlicedTube<Interval> x(tdomain, {-1,7});
    SlicedTube<Interval> v(tdomain, {-1.5,4});

    auto sx = x.first_slice();
    auto sv = v.first_slice();

    x.set({2,3},0);
    x.set({3,4},4);

    CtcDeriv ctc_deriv;
    ctc_deriv.contract(*sx, *sv);

    ConvexPolygon p1 = sx->polygon_slice(*sv);
    CHECK(p1.box()[0] == Interval(0,4));
    CHECK(p1.box()[1] == Interval(-1,7));

    vector<Vector> v_pts {
      {0,2},{0,3},{1,7},{2,7},
      {4,4},{4,3},{3,-1},{2,-1}
    };
    ConvexPolygon p2(v_pts);

    CHECK(Approx(p1) == p2);
  }

  SECTION("Polygons from Slice, test 4b")
  {
    auto tdomain = create_tdomain({4,8});
    SlicedTube<Interval> x(tdomain, {-1,7});
    SlicedTube<Interval> v(tdomain, {-0.75,-0.5});

    auto sx = x.first_slice();
    auto sv = v.first_slice();

    x.set({3,4},4);
    x.set({1},8);

    CtcDeriv ctc_deriv;
    ctc_deriv.contract(*sx, *sv);

    ConvexPolygon p1 = sx->polygon_slice(*sv);
    CHECK(p1.box()[0] == Interval(4,8));
    CHECK(p1.box()[1] == Interval(1,4));

    vector<Vector> v_pts {
      {4,3},{4,4},{8,1}
    };
    ConvexPolygon p2(v_pts);

    //DefaultFigure::draw_polygon(p1);

    CHECK(Approx(p1,1e-10) == p2);
  }

  SECTION("Polygons from Slice, test 5")
  {
    auto tdomain = create_tdomain({8,12});
    SlicedTube<Interval> x(tdomain, {-1,7});
    SlicedTube<Interval> v(tdomain, {-1./3.,1});

    auto sx = x.first_slice();
    auto sv = v.first_slice();

    x.set({1},8);
    x.set({1},12);

    CtcDeriv ctc_deriv;
    ctc_deriv.contract(*sx, *sv);

    ConvexPolygon p1 = sx->polygon_slice(*sv);
    CHECK(p1.box()[0] == Interval(8,12));
    CHECK(Approx(p1.box()[1]) == Interval(0,2));

    vector<Vector> v_pts {
      {8,1},{9,2},{12,1},{11,0}
    };
    ConvexPolygon p2(v_pts);

    //DefaultFigure::draw_polygon(p1);

    CHECK(Approx(p1,1e-10) == p2);
  }

  SECTION("Polygons from Slice, test 6")
  {
    auto tdomain = create_tdomain({12,14});
    SlicedTube<Interval> x(tdomain, {-1,7});
    SlicedTube<Interval> v(tdomain, Interval(4.5)/2);

    auto sx = x.first_slice();
    auto sv = v.first_slice();

    x.set({1},12);
    x.set({5.5},14);

    CtcDeriv ctc_deriv;
    ctc_deriv.contract(*sx, *sv);

    ConvexPolygon p1 = sx->polygon_slice(*sv);
    CHECK(p1.box()[0] == Interval(12,14));
    CHECK(Approx(p1.box()[1]) == Interval(1,5.5));

    vector<Vector> v_pts {
      {12,1},{14,5.5}
    };
    ConvexPolygon p2(v_pts);

    //DefaultFigure::draw_polygon(p1);

    CHECK(Approx(p1,1e-10) == p2);
  }
}