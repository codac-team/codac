#include "tests.h"

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace tubex;

TEST_CASE("Polygon")
{
  SECTION("Polygon from IntervalVector")
  {
    /*IntervalVector iv(2);
    iv[0] = Interval(-1.,5.);
    iv[1] = Interval(10.,11.);

    Polygon p(iv);
    CHECK(p.nbPoints() == 5);
    CHECK(p.box() == iv);
    Polygon p_convex = p;
    p_convex.makeConvex();
    p_convex.makeConvex(); // should be ineffective
    CHECK(p_convex.nbPoints() == 5);
    CHECK(p == p_convex);
    CHECK_FALSE(p != p_convex);
  }

  SECTION("Polygon from IntervalVector (unbounded case)")
  {
    IntervalVector iv1(2);
    iv1[0] = Interval(-1.,5.);
    CHECK_THROWS(Polygon p(iv1););

    IntervalVector iv2(2);
    vector<IntervalVector> v_boxes;
    v_boxes.push_back(iv1);
    v_boxes.push_back(iv2);
    CHECK_THROWS(Polygon p(v_boxes););
  }

  SECTION("Polygon from points (box shape)")
  {
    vector<double> v_x, v_y;
    v_x.push_back(2.); v_y.push_back(4.);
    v_x.push_back(4.); v_y.push_back(4.);
    v_x.push_back(2.); v_y.push_back(6.);
    v_x.push_back(4.); v_y.push_back(6.);

    Polygon p(v_x, v_y);
    CHECK(p.nbPoints() == 5);

    IntervalVector iv(2);
    iv[0] = Interval(2.,4.);
    iv[1] = Interval(4.,6.);
    CHECK(p.box() == iv);
    Polygon p2(iv);
    p.makeConvex();
    CHECK(p.nbPoints() == 5);
    CHECK(p == p2);
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
    CHECK(p.nbPoints() == 5);

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
    CHECK(p1.nbPoints() == 4);

    IntervalVector iv(2);
    iv[0] = Interval(2.,4.);
    iv[1] = Interval(4.,6.);
    Polygon p2(iv);
    CHECK(p2.nbPoints() == 5);

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
    vector<double> v_x, v_y;
    v_x.push_back(1.); v_y.push_back(3.);
    v_x.push_back(5.); v_y.push_back(6.);
    v_x.push_back(5.); v_y.push_back(2.);
    v_x.push_back(3.); v_y.push_back(2.);
    v_x.push_back(1.); v_y.push_back(3.);
    Polygon p1(v_x, v_y);

    IntervalVector iv(2);
    iv[0] = Interval(2.,4.);
    iv[1] = Interval(4.,6.);
    Polygon p2(iv);

    Polygon p3 = p1 & p2;
    CHECK(p3.nbPoints() == 4);

    v_x.clear(); v_y.clear();
    v_x.push_back(2. + 1./3.); v_y.push_back(4.);
    v_x.push_back(4.); v_y.push_back(5.25);
    v_x.push_back(4.); v_y.push_back(4.);
    v_x.push_back(2. + 1./3.); v_y.push_back(4.);
    Polygon expected_result(v_x, v_y);
    CHECK(p3.nbPoints() == expected_result.nbPoints());
    CHECK(p3 == expected_result);*/
  }
}