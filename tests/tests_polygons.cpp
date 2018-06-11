#include "tests.h"

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace tubex;

TEST_CASE("Polygon")
{
  // todo: test Polygon constructor with redundant points

  SECTION("Polygon from IntervalVector")
  {
    /*IntervalVector iv(2);
    iv[0] = Interval(-1.,5.);
    iv[1] = Interval(10.,11.);

    Polygon p(iv);
    CHECK(p.nbVertices() == 4);
    CHECK(p.box() == iv);
    CHECK(p[0] == point(-1.,10.));
    CHECK(p[1] == point(-1.,11.));
    CHECK(p[2] == point(5.,11.));
    CHECK(p[3] == point(5.,10.));*/
  }
/*
  SECTION("Polygon from IntervalVector (unbounded case)")
  {
    IntervalVector iv1(2);
    iv1[0] = Interval(-1.,5.);
    Polygon p(iv1);
    
    CHECK(p[0] == point(-1.,NEG_INFINITY));
    CHECK(p[1] == point(-1.,POS_INFINITY));
    CHECK(p[2] == point(5.,POS_INFINITY));
    CHECK(p[3] == point(5.,NEG_INFINITY));
  }

  SECTION("Polygon from points (box shape)")
  {
    vector<Point> v_pts;
    v_pts.push_back(point(2.,4.));
    v_pts.push_back(point(2.,6.));
    v_pts.push_back(point(4.,6.));
    v_pts.push_back(point(4.,4.));

    Polygon p1(v_pts);
    CHECK(p1.nbVertices() == 4);

    IntervalVector iv(2);
    iv[0] = Interval(2.,4.);
    iv[1] = Interval(4.,6.);
    CHECK(p1.box() == iv);

    Polygon p2(iv);
    CHECK(p2.nbVertices() == 4);
    CHECK(p1 == p2);

    // Polygon defined by points in different order
    v_pts.clear();
    v_pts.push_back(point(2.,6.));
    v_pts.push_back(point(4.,6.));
    v_pts.push_back(point(4.,4.));
    v_pts.push_back(point(2.,4.));
    Polygon p3(v_pts);
    CHECK(p3 == p1);
    CHECK(p3 == p2);
  }
/*
  SECTION("Polygon from points (unbounded case)")
  {
    IntervalVector iv1(2);
    iv1[0] = Interval(-1.,5.);
    Polygon p1(iv1);

    vector<Point> v_pts;
    v_pts.push_back(point(-1.,NEG_INFINITY));
    v_pts.push_back(point(-1.,POS_INFINITY));
    v_pts.push_back(point(5.,POS_INFINITY));
    v_pts.push_back(point(5.,NEG_INFINITY));
    Polygon p2(v_pts);

    //CHECK(p1 == p2);
    CHECK(p1.box() == iv1);
    CHECK(p2.box() == iv1);
  }

  SECTION("Polygon from points (random shape)")
  {
    vector<double> v_x, v_y;
    v_x.push_back(1.); v_y.push_back(3.);
    v_x.push_back(3.); v_y.push_back(3.);
    v_x.push_back(5.); v_y.push_back(6.);
    v_x.push_back(5.); v_y.push_back(2.);
    v_x.push_back(3.); v_y.push_back(2.);

    Polygon p(v_x, v_y);
    CHECK(p.nbVertices() == 5);

    IntervalVector iv(2);
    iv[0] = Interval(1.,5.);
    iv[1] = Interval(2.,6.);
    CHECK(p.box() == iv);

    v_x.clear(); v_y.clear(); // points in different order
    v_x.push_back(3.); v_y.push_back(2.);
    v_x.push_back(3.); v_y.push_back(3.);
    v_x.push_back(1.); v_y.push_back(3.);
    v_x.push_back(5.); v_y.push_back(2.);
    v_x.push_back(5.); v_y.push_back(6.);

    Polygon p_result(v_x, v_y);
    CHECK(p_result == p);
  }

  SECTION("Intersection, line/polygon, 1")
  {
    vector<double> v_x, v_y;
    v_x.push_back(1.); v_y.push_back(3.);
    v_x.push_back(5.); v_y.push_back(6.);
    Polygon p1(v_x, v_y);
    CHECK(p1.nbVertices() == 4);

    IntervalVector iv(2);
    iv[0] = Interval(2.,4.);
    iv[1] = Interval(4.,6.);
    Polygon p2(iv);
    CHECK(p2.nbVertices() == 5);

    v_x.clear(); v_y.clear();
    v_x.push_back(2. + 1./3.); v_y.push_back(4.);
    v_x.push_back(4.); v_y.push_back(5.25);
    Polygon expected_result(v_x, v_y);
    Polygon p3 = p1 & p2;
    p3.makeConvex();
    expected_result.makeConvex();
    //CHECK(p3 == expected_result);
    CHECK(ApproxIntv(p3.box()[0]) == expected_result.box()[0]);
    CHECK(ApproxIntv(p3.box()[1]) == expected_result.box()[1]);
  }

  SECTION("Intersection, line/polygon, 2")
  {
    vector<double> v_x, v_y;
    v_x.push_back(-1.); v_y.push_back(-5.);
    v_x.push_back(-1.); v_y.push_back(3.);
    v_x.push_back(3.); v_y.push_back(3.);
    v_x.push_back(3.); v_y.push_back(-5.);
    v_x.push_back(-1.); v_y.push_back(-5.);
    Polygon p1(v_x, v_y);

    v_x.clear(); v_y.clear();
    v_x.push_back(-1.); v_y.push_back(1.);
    v_x.push_back(3.); v_y.push_back(-3.);
    Polygon p2(v_x, v_y);

    Polygon p3 = p1 & p2;
    CHECK(p3.box()[0] == Interval(-1.,3.));
    CHECK(p3.box()[1] == Interval(-3.,1.));
  }

  SECTION("Intersection, box/polygon")
  {
    vector<Point> v_pts;
    v_pts.push_back(point(1.,3.));
    v_pts.push_back(point(5.,6.));
    v_pts.push_back(point(5.,2.));
    v_pts.push_back(point(3.,2.));
    ConvexPolygon p1(v_pts);

    IntervalVector iv(2);
    iv[0] = Interval(2.,4.);
    iv[1] = Interval(4.,6.);
    ConvexPolygon p2(iv);

    //ConvexPolygon p3 = p1 & p2;
    CHECK(p3.nbVertices() == 3);

    v_pts.clear();
    v_pts.push_back(Point(2. + 1./3., 4.));
    v_pts.push_back(Point(4., 5.25));
    v_pts.push_back(Point(4., 4.));
    v_pts.push_back(Point(2. + 1./3., 4.));
    ConvexPolygon expected_result(v_pts);
    CHECK(p3.nbVertices() == expected_result.nbVertices());
    //CHECK(p3 == expected_result);
  }*/
}