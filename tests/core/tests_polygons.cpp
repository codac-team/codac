#include "catch_interval.hpp"
#include "codac_VIBesFig.h"
#include "codac_polygon_arithmetic.h"

// Using #define so that we can access protected methods
// of the class for tests purposes
#define protected public
#include "codac_CtcDeriv.h"
#include "codac_GrahamScan.h"

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace codac;

#define VIBES_DRAWING 1

TEST_CASE("Polygons")
{
  // todo: test Polygon constructor with redundant points

  SECTION("Polygon from IntervalVector")
  {
    IntervalVector iv(2);
    iv[0] = Interval(-1.,5.);
    iv[1] = Interval(10.,11.);

    ConvexPolygon p(iv);
    CHECK(p.nb_vertices() == 4);
    CHECK(p.box() == iv);
    CHECK(p[0] == Vector({-1.,10.}));
    CHECK(p[1] == Vector({5.,10.}));
    CHECK(p[2] == Vector({5.,11.}));
    CHECK(p[3] == Vector({-1.,11.}));
  }

  SECTION("Polygon from IntervalVector (unbounded case)")
  {
    // not anymore supported IntervalVector iv1(2);
    // not anymore supported iv1[0] = Interval(-1.,5.);
    // not anymore supported ConvexPolygon p(iv1);
    // not anymore supported 
    // not anymore supported CHECK(p[0] == Point(-1.,Interval::ALL_REALS));
    // not anymore supported CHECK(p[1] == Point(5.,Interval::ALL_REALS));
    // not anymore supported CHECK(p[2] == Point(5.,Interval::ALL_REALS));
    // not anymore supported CHECK(p[3] == Point(-1.,Interval::ALL_REALS));
  }

  SECTION("Polygon from points (box shape)")
  {
    vector<Point> v_pts;
    v_pts.push_back(Point(2.,4.));
    v_pts.push_back(Point(2.,6.));
    v_pts.push_back(Point(4.,6.));
    v_pts.push_back(Point(4.,4.));

    ConvexPolygon p1(v_pts);
    CHECK(p1.nb_vertices() == 4);

    IntervalVector iv(2);
    iv[0] = Interval(2.,4.);
    iv[1] = Interval(4.,6.);
    CHECK(p1.box() == iv);

    ConvexPolygon p2(iv);
    CHECK(p2.nb_vertices() == 4);
    CHECK(p1 == p2);

    // Polygon defined by points in different order
    v_pts.clear();
    v_pts.push_back(Point(2.,6.));
    v_pts.push_back(Point(4.,6.));
    v_pts.push_back(Point(4.,4.));
    v_pts.push_back(Point(2.,4.));
    ConvexPolygon p3(v_pts);
    CHECK(p3 == p1);
    CHECK(p3 == p2);
  }

  SECTION("Polygon from points (unbounded case)")
  {
    // not supported anymore IntervalVector iv1(2);
    // not supported anymore iv1[0] = Interval(-1.,5.);
    // not supported anymore iv1[1] = Interval::ALL_REALS;
    // not supported anymore ConvexPolygon p1(iv1);
    // not supported anymore 
    // not supported anymore vector<Point> v_pts;
    // not supported anymore v_pts.push_back(Point(-1.,Interval::ALL_REALS));
    // not supported anymore v_pts.push_back(Point(-1.,Interval::ALL_REALS));
    // not supported anymore v_pts.push_back(Point(5.,Interval::ALL_REALS));
    // not supported anymore v_pts.push_back(Point(5.,Interval::ALL_REALS));
    // not supported anymore Polygon p2(v_pts);
    // not supported anymore 
    // not supported anymore CHECK(p1 == p2);
    // not supported anymore CHECK(p1.box() == iv1);
    // not supported anymore CHECK(p2.box() == iv1);
  }

  SECTION("Polygon from points (random shape)")
  {
    vector<Point> v_p;
    v_p.push_back(Point(1.,3.));
    v_p.push_back(Point(3.,3.));
    v_p.push_back(Point(5.,6.));
    v_p.push_back(Point(5.,2.));
    v_p.push_back(Point(3.,2.));

    ConvexPolygon p(v_p);
    CHECK(p.nb_vertices() == 4);

    IntervalVector iv(2);
    iv[0] = Interval(1.,5.);
    iv[1] = Interval(2.,6.);
    CHECK(p.box() == iv);

    v_p.clear(); // points in different order
    v_p.push_back(Point(5.,2.));
    v_p.push_back(Point(3.,2.));
    v_p.push_back(Point(1.,3.));
    v_p.push_back(Point(5.,6.));

    ConvexPolygon p_result(v_p);
    CHECK(p_result == p);
  }

  SECTION("Polygon from points (comparison)")
  {
    vector<Point> v_p;
    v_p.push_back(Point(-1., -1.));
    v_p.push_back(Point(-3.5, -3.5));
    v_p.push_back(Point(1.5, 1.5));
    v_p.push_back(Point(3., -2.));
    v_p.push_back(Point(3., 0.));
    v_p.push_back(Point(0., 3.));
    v_p.push_back(Point(-1., 2.));
    ConvexPolygon p1(v_p);

    v_p.clear();
    v_p.push_back(Point(-1., -1.));
    v_p.push_back(Point(-1., 2.));
    v_p.push_back(Point(0., 3.));
    v_p.push_back(Point(3., 0.));
    v_p.push_back(Point(3., -2.));
    v_p.push_back(Point(1.5, 1.5));
    v_p.push_back(Point(-3.5, -3.5));
    ConvexPolygon p2(v_p);

    CHECK(p1 == p2);
    CHECK(ApproxConvexPolygon(p1) == p2); // testing Approx* class too
    CHECK_FALSE(p1 != p2);
    CHECK_FALSE(ApproxConvexPolygon(p1) != p2); // testing Approx* class too
  }
}

TEST_CASE("Polygons (intersections)")
{
  SECTION("Intersection, line/polygon")
  {
    vector<Point> v_p;
    v_p.push_back(Point(1.,0.));
    v_p.push_back(Point(2.,0.));
    v_p.push_back(Point(4.,2.));
    v_p.push_back(Point(4.,4.));
    v_p.push_back(Point(3.,7.));
    v_p.push_back(Point(2.,7.));
    v_p.push_back(Point(0.,5.));
    v_p.push_back(Point(0.,3.));
    ConvexPolygon p(v_p);
    CHECK(p.nb_vertices() == 8);

    CHECK(p.edges()[0] == Edge(Point(1.,0.), Point(2.,0.)));
    CHECK(p.edges()[1] == Edge(Point(2.,0.), Point(4.,2.)));

    IntervalVector inter(2), x(2);
    
    Edge edge = Edge(Point(1.,0.), Point(2.,0.));
    x[0] = Interval(0.5,4.); x[1] = Interval(-1.,1.);
    inter = edge & x;
    CHECK(inter[0] == Interval(1.,2.));
    CHECK(inter[1] == Interval(0.));

    // Horizontal edge
    Edge edge1(Point(2.,0.), Point(9.,4.));
    Edge edge2(Point(3.,2.), Point(7.,2.));
    Point pt_inter = edge1 & edge2;
    CHECK(ApproxIntv(pt_inter.x()) == Point(5.5,2.).x());
    CHECK(ApproxIntv(pt_inter.y()) == Point(5.5,2.).y());

    // Vertical edge
    edge1 = Edge(Point(1.,4.), Point(3.,2.));
    edge2 = Edge(Point(2.,1.), Point(2.,7.));
    pt_inter = edge1 & edge2;
    CHECK(ApproxIntv(pt_inter.x()) == Point(2.,3.).x());
    CHECK(ApproxIntv(pt_inter.y()) == Point(2.,3.).y());

    // No intersection
    edge1 = Edge(Point(2.,0.), Point(4.,2.));
    edge2 = Edge(Point(0.,-1.), Point(4.,-1.));
    pt_inter = edge1 & edge2;
    CHECK(ApproxIntv(pt_inter.x()) == Interval::EMPTY_SET);
    CHECK(ApproxIntv(pt_inter.y()) == Interval::EMPTY_SET);

    edge = Edge(Point(2.,0.), Point(4.,2.));
    inter = edge & x;
    CHECK(inter[0] == Interval(2.,3.));
    CHECK(inter[1] == Interval(0.,1.));

    edge = Edge(Point(4.,2.), Point(4.,4.));
    inter = edge & x;
    CHECK(inter[0] == Interval::EMPTY_SET);
    CHECK(inter[1] == Interval::EMPTY_SET);

    edge = Edge(Point(4.,4.), Point(3.,7.));
    inter = edge & x;
    CHECK(inter[0] == Interval::EMPTY_SET);
    CHECK(inter[1] == Interval::EMPTY_SET);
  }

  SECTION("Intersection, box/polygon")
  {
    vector<Point> v_p;
    v_p.push_back(Point(1.,0.));
    v_p.push_back(Point(2.,0.));
    v_p.push_back(Point(4.,2.));
    v_p.push_back(Point(4.,4.));
    v_p.push_back(Point(3.,7.));
    v_p.push_back(Point(2.,7.));
    v_p.push_back(Point(0.,5.));
    v_p.push_back(Point(0.,3.));
    ConvexPolygon p(v_p);
    IntervalVector x(2), box_inter(2);

    x[0] = Interval(0.5,4.); x[1] = Interval(-1.,1.);
    box_inter = p.fast_intersection(x);
    CHECK(box_inter[0] == Interval(2./3.,3.));
    CHECK(box_inter[1] == Interval(0.,1.));

    x[0] = Interval(2.,6.); x[1] = Interval(2.,3.);
    box_inter = p.fast_intersection(x);
    CHECK(box_inter[0] == Interval(2.,4.));
    CHECK(box_inter[1] == Interval(2.,3.));

    x[0] = Interval(3.,6.); x[1] = Interval(5.,6.);
    box_inter = p.fast_intersection(x);
    CHECK(ApproxIntv(box_inter[0]) == Interval(3.,11./3.));
    CHECK(ApproxIntv(box_inter[1]) == Interval(5.,6.));

    x[0] = Interval(2.5,5.); x[1] = Interval(7.,10.);
    box_inter = p.fast_intersection(x);
    CHECK(ApproxIntv(box_inter[0]) == Interval(2.5,3.));
    CHECK(ApproxIntv(box_inter[1]) == Interval(7.));

    x[0] = Interval(2.,3.); x[1] = Interval(3.,5.);
    box_inter = p.fast_intersection(x);
    CHECK(box_inter == x);

    x[0] = Interval(-99.,0.); x[1] = Interval(1.,7.);
    box_inter = p.fast_intersection(x);
    CHECK(ApproxIntv(box_inter[0]) == Interval(0.));
    CHECK(ApproxIntv(box_inter[1]) == Interval(3.,5.));

    x[0] = Interval(1.,2.); x[1] = Interval(3.); // degenerate case
    box_inter = p.fast_intersection(x);
    CHECK(ApproxIntv(box_inter[0]) == Interval(1.,2.));
    CHECK(ApproxIntv(box_inter[1]) == Interval(3.));

    x[0] = Interval::ALL_REALS; x[1] = Interval::ALL_REALS; // whole envelope
    box_inter = p.fast_intersection(x);
    CHECK(box_inter == p.box());

    x[0] = Interval(3.); x[1] = Interval(1.); // degenerate case
    box_inter = p.fast_intersection(x);
    CHECK(ApproxIntv(box_inter[0]) == Interval(3.));
    CHECK(ApproxIntv(box_inter[1]) == Interval(1.));

    x[0] = Interval(1.); x[1] = Interval::ALL_REALS; // degenerate case
    box_inter = p.fast_intersection(x);
    CHECK(ApproxIntv(box_inter[0]) == Interval(1.));
    CHECK(ApproxIntv(box_inter[1]) == Interval(0.,6.));

    x[0] = Interval(0.5,3.5); x[1] = Interval(0.5,6.5); // no contraction
    box_inter = p.fast_intersection(x);
    CHECK(ApproxIntv(box_inter[0]) == Interval(0.5,3.5));
    CHECK(ApproxIntv(box_inter[1]) == Interval(0.5,6.5));
  }

  SECTION("Intersection, box/polygon, another test")
  {
    vector<Point> v_p;
    v_p.push_back(Point(1.,1.));
    v_p.push_back(Point(3.,1.));
    v_p.push_back(Point(11.,3.));
    v_p.push_back(Point(13.,5.));
    v_p.push_back(Point(14.,9.));
    v_p.push_back(Point(10.,13.));
    v_p.push_back(Point(8.,14.));
    v_p.push_back(Point(6.,13.));
    v_p.push_back(Point(3.,10.));
    v_p.push_back(Point(1.,6.));
    v_p.push_back(Point(1.,3.));
    ConvexPolygon p(v_p);
    IntervalVector x(2), box_inter(2);

    CHECK(p.encloses(Point(7.,7.)) == YES);
    CHECK(p.encloses(Point(2.,7.)) == YES);
    CHECK(p.encloses(Point(10.,3.)) == YES);
    CHECK(p.encloses(Point(11.,9.)) == YES);
    CHECK(p.encloses(Point(13.5,9.)) == YES);
    CHECK(p.encloses(Point(5.8,12.2)) == YES);

    CHECK(p.encloses(Point(1.,4.)) == MAYBE);
    CHECK(p.encloses(Point(3.,10.)) == MAYBE);
    CHECK(p.encloses(Point(8.,14.)) == MAYBE);
    CHECK(p.encloses(Point(2.,8.)) == MAYBE);
    CHECK(p.encloses(Point(2.5,9.)) == MAYBE);
    CHECK(p.encloses(Point(5.5,12.5)) == MAYBE);
    CHECK(p.encloses(Point(1.,5.)) == MAYBE);
    CHECK(p.encloses(Point(1.,1.)) == MAYBE);

    CHECK(p.encloses(Point(10.,2.)) == NO);
    CHECK(p.encloses(Point(0.0,0.0)) == NO);
    CHECK(p.encloses(Point(0.0,0.9)) == NO);
    CHECK(p.encloses(Point(0.9,0.0)) == NO);
    CHECK(p.encloses(Point(0.9,0.9)) == NO);
    CHECK(p.encloses(Point(0.5,1.)) == NO);
    CHECK(p.encloses(Point(5.2,12.8)) == NO);
    CHECK(p.encloses(Point(1.,14.)) == NO);
    CHECK(p.encloses(Point(1.,13.5)) == NO);
    CHECK(p.encloses(Point(14.,1.)) == NO);
    CHECK(p.encloses(Point(14.,14.)) == NO);
    CHECK(p.encloses(Point(5.,14.)) == NO);

    x[0] = Interval(0.,2.); x[1] = Interval(0.,2.);
    box_inter = p.fast_intersection(x);
    CHECK(box_inter[0] == Interval(1.,2.));
    CHECK(box_inter[1] == Interval(1.,2.));

    x[0] = Interval(0.,1.); x[1] = Interval(0.,1.);
    box_inter = p.fast_intersection(x);
    CHECK(box_inter[0] == Interval(1.,1.));
    CHECK(box_inter[1] == Interval(1.,1.));

    x[0] = Interval(0.,0.9); x[1] = Interval(0.,0.9);
    box_inter = p.fast_intersection(x);
    CHECK(box_inter[0] == Interval::EMPTY_SET);
    CHECK(box_inter[1] == Interval::EMPTY_SET);
    
    x[0] = Interval(-70.,1.); x[1] = Interval::ALL_REALS;
    box_inter = p.fast_intersection(x);
    CHECK(box_inter[0] == Interval(1.));
    CHECK(box_inter[1] == Interval(1.,6.));

    x[0] = Interval(7.,8.); x[1] = Interval(7.,8.);
    box_inter = p.fast_intersection(x);
    CHECK(box_inter[0] == Interval(7.,8.));
    CHECK(box_inter[1] == Interval(7.,8.));

    x[0] = Interval(7.); x[1] = Interval(7.);
    box_inter = p.fast_intersection(x);
    CHECK(box_inter[0] == Interval(7.));
    CHECK(box_inter[1] == Interval(7.));

    x[0] = Interval(6.); x[1] = Interval(13.);
    box_inter = p.fast_intersection(x);
    CHECK(box_inter[0] == Interval(6.));
    CHECK(box_inter[1] == Interval(13.));

    x[0] = Interval(7.,13.); x[1] = Interval(1.,2.);
    box_inter = p.fast_intersection(x);
    CHECK(box_inter[0] == Interval(7.));
    CHECK(box_inter[1] == Interval(2.));

    x[0] = Interval(12.,16.); x[1] = Interval(3.,12.);
    box_inter = p.fast_intersection(x);
    CHECK(box_inter[0] == Interval(12.,14.));
    CHECK(box_inter[1] == Interval(4.,11.));

    x[0] = Interval(7.); x[1] = Interval::POS_REALS;
    box_inter = p.fast_intersection(x);
    CHECK(box_inter[0] == Interval(7.));
    CHECK(box_inter[1] == Interval(2.,13.5));

    x[0] = Interval::POS_REALS; x[1] = Interval::POS_REALS;
    box_inter = p.fast_intersection(x);
    CHECK(box_inter[0] == Interval(1.,14.));
    CHECK(box_inter[1] == Interval(1.,14.));

    x[0] = Interval::POS_REALS; x[1] = Interval(8.);
    box_inter = p.fast_intersection(x);
    CHECK(box_inter[0] == Interval(2.,13.75));
    CHECK(box_inter[1] == Interval(8.));

    x[0] = Interval::POS_REALS; x[1] = Interval(15.);
    box_inter = p.fast_intersection(x);
    CHECK(box_inter[0] == Interval::EMPTY_SET);
    CHECK(box_inter[1] == Interval::EMPTY_SET);

    x[0] = Interval::NEG_REALS; x[1] = Interval(13.);
    box_inter = p.fast_intersection(x);
    CHECK(box_inter[0] == Interval::EMPTY_SET);
    CHECK(box_inter[1] == Interval::EMPTY_SET);

    x[0] = Interval(1.); x[1] = Interval(2.,4.);
    box_inter = p.fast_intersection(x);
    CHECK(box_inter[0] == Interval(1.));
    CHECK(box_inter[1] == Interval(2.,4.));

    x[0] = Interval(2.,7.); x[1] = Interval(1.,2.);
    box_inter = p.fast_intersection(x);
    CHECK(box_inter[0] == Interval(2.,7.));
    CHECK(box_inter[1] == Interval(1.,2.));

    x[0] = Interval::EMPTY_SET; x[1] = Interval::EMPTY_SET;
    box_inter = p.fast_intersection(x);
    CHECK(box_inter[0] == Interval::EMPTY_SET);
    CHECK(box_inter[1] == Interval::EMPTY_SET);
  }
}

TEST_CASE("Polygons from Slice")
{
  SECTION("Unbounded case, POS_INFINITY")
  {
    // not supported anymore vector<Point> v_p;
    // not supported anymore v_p.push_back(Point(1.,0.));
    // not supported anymore v_p.push_back(Point(2.,0.));
    // not supported anymore v_p.push_back(Point(4.,2.));
    // not supported anymore v_p.push_back(Point(4.,4.));
    // not supported anymore v_p.push_back(Point(3.,Interval(3.,POS_INFINITY)));
    // not supported anymore v_p.push_back(Point(2.,Interval(3.,POS_INFINITY)));
    // not supported anymore v_p.push_back(Point(0.,5.));
    // not supported anymore v_p.push_back(Point(0.,3.));
    // not supported anymore ConvexPolygon p(v_p);
    // not supported anymore IntervalVector x(2), box_inter(2);
    // not supported anymore 
    // not supported anymore CHECK(p.encloses(Point(3.5,8.)));
    // not supported anymore 
    // not supported anymore x[0] = Interval(0.5,4.); x[1] = Interval(-1.,1.);
    // not supported anymore box_inter = p & x;
    // not supported anymore CHECK(box_inter[0] == Interval(2./3.,3.));
    // not supported anymore CHECK(box_inter[1] == Interval(0.,1.));
    // not supported anymore 
    // not supported anymore x[0] = Interval(2.,6.); x[1] = Interval(2.,3.);
    // not supported anymore box_inter = p & x;
    // not supported anymore CHECK(box_inter[0] == Interval(2.,4.));
    // not supported anymore CHECK(box_inter[1] == Interval(2.,3.));
    // not supported anymore 
    // not supported anymore x[0] = Interval(3.,6.); x[1] = Interval(5.,6.);
    // not supported anymore box_inter = p & x;
    // not supported anymore CHECK(ApproxIntv(box_inter[0]) == Interval(3.,4.));
    // not supported anymore CHECK(ApproxIntv(box_inter[1]) == Interval(5.,6.));
    // not supported anymore 
    // not supported anymore x[0] = Interval(2.5,5.); x[1] = Interval(7.,10.);
    // not supported anymore box_inter = p & x;
    // not supported anymore CHECK(ApproxIntv(box_inter[0]) == Interval(2.5,4.));
    // not supported anymore CHECK(ApproxIntv(box_inter[1]) == Interval(7.,10.));
    // not supported anymore 
    // not supported anymore x[0] = Interval(2.,3.); x[1] = Interval(3.,5.);
    // not supported anymore box_inter = p & x;
    // not supported anymore CHECK(box_inter == x);
    // not supported anymore 
    // not supported anymore x[0] = Interval(-99.,0.); x[1] = Interval(1.,7.);
    // not supported anymore box_inter = p & x;
    // not supported anymore CHECK(ApproxIntv(box_inter[0]) == Interval(0.));
    // not supported anymore CHECK(ApproxIntv(box_inter[1]) == Interval(3.,7.));
    // not supported anymore 
    // not supported anymore x[0] = Interval(0.,2.); x[1] = Interval(3.); // degenerate case
    // not supported anymore box_inter = p & x;
    // not supported anymore CHECK(ApproxIntv(box_inter[0]) == Interval(0.,2.));
    // not supported anymore CHECK(ApproxIntv(box_inter[1]) == Interval(3.));
    // not supported anymore 
    // not supported anymore x[0] = Interval::ALL_REALS; x[1] = Interval::ALL_REALS; // whole envelope
    // not supported anymore box_inter = p & x;
    // not supported anymore CHECK(ApproxIntv(box_inter[0]) == Interval(0.,4.));
    // not supported anymore CHECK(ApproxIntv(box_inter[1]) == Interval(0.,POS_INFINITY));
    // not supported anymore 
    // not supported anymore x[0] = Interval(3.); x[1] = Interval(1.); // degenerate case
    // not supported anymore box_inter = p & x;
    // not supported anymore CHECK(ApproxIntv(box_inter[0]) == Interval(3.));
    // not supported anymore CHECK(ApproxIntv(box_inter[1]) == Interval(1.));
    // not supported anymore 
    // not supported anymore x[0] = Interval(1.); x[1] = Interval::ALL_REALS; // degenerate case
    // not supported anymore box_inter = p & x;
    // not supported anymore CHECK(ApproxIntv(box_inter[0]) == Interval(1.));
    // not supported anymore CHECK(ApproxIntv(box_inter[1]) == Interval(0.,POS_INFINITY));
    // not supported anymore 
    // not supported anymore x[0] = Interval(0.5,3.5); x[1] = Interval(0.5,6.5); // no contraction
    // not supported anymore box_inter = p & x;
    // not supported anymore CHECK(ApproxIntv(box_inter[0]) == Interval(0.5,3.5));
    // not supported anymore CHECK(ApproxIntv(box_inter[1]) == Interval(0.5,6.5));
  }

  SECTION("Polygons from Slice, test 1")
  {
    Slice x(Interval(-1.,3.)); // unbounded
    x.set_input_gate(Interval(-1.,2.));
    x.set_output_gate(Interval(-2.,0.));

    Slice v(Interval(-1.,3.), Interval(-1.,1.));

    CtcDeriv ctc_deriv;
    ctc_deriv.contract(x, v);

    ConvexPolygon p1 = x.polygon(v);
    CHECK(p1.box()[0] == Interval(-1.,3.));
    CHECK(p1.box()[1] == Interval(-3.5,3.));

    vector<Point> v_pts;
    v_pts.push_back(Point(-1.,-1.));
    v_pts.push_back(Point(-1.,2.));
    v_pts.push_back(Point(0.,3.));
    v_pts.push_back(Point(3.,0.));
    v_pts.push_back(Point(3.,-2.));
    v_pts.push_back(Point(1.5,-3.5));
    ConvexPolygon p2(v_pts);

    CHECK(ApproxConvexPolygon(p1) == p2);
  }

  SECTION("Polygons from Slice, test 2")
  {
    Slice x(Interval(-1.,3.), Interval(-5.,3.));
    x.set_input_gate(Interval(-1.,3.));
    x.set_output_gate(Interval(-5.,0.5));

    Slice v(Interval(-1.,3.), Interval(-1.));

    CtcDeriv ctc_deriv;
    ctc_deriv.contract(x, v);

    ConvexPolygon p1 = x.polygon(v);
    CHECK(p1.box()[0] == Interval(-1.,3.));
    CHECK(p1.box()[1] == Interval(-5.,3.));

    vector<Point> v_pts;
    v_pts.push_back(Point(-1.,-1.));
    v_pts.push_back(Point(-1.,3.));
    v_pts.push_back(Point(3.,-1.));
    v_pts.push_back(Point(3.,-5.));
    v_pts.push_back(Point(-1.,-1.));
    ConvexPolygon p2(v_pts);

    CHECK(ApproxConvexPolygon(p1) == p2);
  }

  SECTION("Polygons from Slice, test 3, degenerate case")
  {
    Slice x(Interval(-1.,3.), Interval(-5.,3.));
    x.set_input_gate(Interval(1.,3.));
    x.set_output_gate(Interval(-4.,-3.));

    Slice v(Interval(-1.,3.), Interval(-1.,1.));

    CtcDeriv ctc_deriv;
    ctc_deriv.contract(x, v);
    ConvexPolygon p1 = x.polygon(v);

    CHECK(p1.box()[0] == Interval(-1.,3.));
    CHECK(p1.box()[1] == Interval(-3.,1.));

    vector<Point> v_pts;
    v_pts.push_back(Point(-1.,1.));
    v_pts.push_back(Point(3.,-3.));
    ConvexPolygon p2(v_pts);

    CHECK(p1.nb_vertices() == 2);
    CHECK(ApproxConvexPolygon(p1) == p2);
  }

  SECTION("Polygons from Slice, test 4")
  {
    Slice x(Interval(0.,4.), Interval(-1.,7.));
    x.set_input_gate(Interval(2.,3.));
    x.set_output_gate(Interval(3.,4.));

    Slice v(Interval(0.,4.), Interval(-1.5,4.));

    CtcDeriv ctc_deriv;
    ctc_deriv.contract(x, v);

    ConvexPolygon p1 = x.polygon(v);
    CHECK(p1.box()[0] == Interval(0.,4.));
    CHECK(p1.box()[1] == Interval(-1.,7.));

    vector<Point> v_pts;
    v_pts.push_back(Point(0.,2.));
    v_pts.push_back(Point(0.,3.));
    v_pts.push_back(Point(1.,7.));
    v_pts.push_back(Point(2.,7.));
    v_pts.push_back(Point(4.,4.));
    v_pts.push_back(Point(4.,3.));
    v_pts.push_back(Point(3.,-1.));
    v_pts.push_back(Point(2.,-1.));
    ConvexPolygon p2(v_pts);

    CHECK(ApproxConvexPolygon(p1) == p2);
  }

  SECTION("Polygons from Slice, test 4")
  {
    Slice x(Interval(4.,8.), Interval(-1.,7.));
    x.set_input_gate(Interval(3.,4.));
    x.set_output_gate(Interval(1.));

    Slice v(Interval(4.,8.), Interval(-0.75,-0.5));

    CtcDeriv ctc_deriv;
    ctc_deriv.contract(x, v);

    ConvexPolygon p1 = x.polygon(v);
    CHECK(p1.box()[0] == Interval(4.,8.));
    CHECK(p1.box()[1] == Interval(1.,4.));

    vector<Point> v_pts;
    v_pts.push_back(Point(4.,3.));
    v_pts.push_back(Point(4.,4.));
    v_pts.push_back(Point(8.,1.));
    ConvexPolygon p2(v_pts);

    CHECK(ApproxConvexPolygon(p1) == p2);
  }

  SECTION("Polygons from Slice, test 5")
  {
    Slice x(Interval(8.,12.), Interval(-1.,7.));
    x.set_input_gate(Interval(1.));
    x.set_output_gate(Interval(1.));

    Slice v(Interval(8.,12.), Interval(-1./3.,1.));

    CtcDeriv ctc_deriv;
    ctc_deriv.contract(x, v);

    ConvexPolygon p1 = x.polygon(v);
    CHECK(p1.box()[0] == Interval(8.,12.));
    CHECK(ApproxIntv(p1.box()[1]) == Interval(0.,2.));

    vector<Point> v_pts;
    v_pts.push_back(Point(8.,1.));
    v_pts.push_back(Point(9.,2.));
    v_pts.push_back(Point(12.,1.));
    v_pts.push_back(Point(11.,0.));
    ConvexPolygon p2(v_pts);

    CHECK(ApproxConvexPolygon(p1) == p2);
  }

  SECTION("Polygons from Slice, test 6")
  {
    Slice x(Interval(12.,14.), Interval(-1.,7.));
    x.set_input_gate(Interval(1.));
    x.set_output_gate(Interval(5.5));

    Slice v(Interval(12.,14.), Interval(4.5)/2.);

    CtcDeriv ctc_deriv;
    ctc_deriv.contract(x, v);

    ConvexPolygon p1 = x.polygon(v);
    CHECK(p1.box()[0] == Interval(12.,14.));
    CHECK(ApproxIntv(p1.box()[1]) == Interval(1.,5.5));

    vector<Point> v_pts;
    v_pts.push_back(Point(12.,1.));
    v_pts.push_back(Point(14,5.5));
    ConvexPolygon p2(v_pts);

    CHECK(ApproxConvexPolygon(p1) == p2);
  }
}

TEST_CASE("Polygons (intersections, again)")
{
  SECTION("Polygons intersections, test 1")
  {
    IntervalVector box(2, Interval(0.,4.));

    vector<Point> v_points;
    v_points.push_back(Point(3.,2.));
    v_points.push_back(Point(1.,6.));
    v_points.push_back(Point(6.,5.));
    ConvexPolygon p(v_points);

    ConvexPolygon box_inter = p & box;
    CHECK(box_inter.box() == IntervalVector(2, Interval(2.,4.)));

    ConvexPolygon p_inter = p & box;
    CHECK(p_inter.nb_vertices() == 4);

    v_points.clear();
    v_points.push_back(Point(3.,2.));
    v_points.push_back(Point(2.,4.));
    v_points.push_back(Point(4.,4.));
    v_points.push_back(Point(4.,3.));
    ConvexPolygon p_truth(v_points);
    CHECK(p_truth == p_inter);
  }

  SECTION("Polygons intersections, test 2")
  {
    IntervalVector box(2, Interval(2.,6.));

    vector<Point> v_points;
    v_points.push_back(Point(1.,2.));
    v_points.push_back(Point(3.,4.));
    v_points.push_back(Point(5.,1.));
    v_points.push_back(Point(2.,1.));
    ConvexPolygon p(v_points);

    ConvexPolygon box_inter = p & box;
    IntervalVector box_truth(2);
    box_truth[0] = Interval(2.,4.+1./3.);
    box_truth[1] = Interval(2.,4.);
    CHECK(ApproxIntvVector(box_inter.box()) == box_truth);

    ConvexPolygon p_inter = p & box;
    CHECK(p_inter.nb_vertices() == 4);

    v_points.clear();
    v_points.push_back(Point(2.,2.));
    v_points.push_back(Point(2.,3.));
    v_points.push_back(Point(3.,4.));
    v_points.push_back(Point(4.+1./3.,2.));
    ConvexPolygon p_truth(v_points);

    CHECK(ApproxConvexPolygon(p_truth, 0.01) == p_inter);
  }

  SECTION("Polygons intersections, test 3 (big box)")
  {
    IntervalVector box(2, Interval(-10.,10.));

    vector<Point> v_points;
    v_points.push_back(Point(1.,2.));
    v_points.push_back(Point(3.,4.));
    v_points.push_back(Point(5.,1.));
    v_points.push_back(Point(2.,1.));
    ConvexPolygon p(v_points);

    ConvexPolygon p_inter = p & box;
    CHECK(p_inter == p); // same polygon
  }

  SECTION("Polygons intersections, test 4 (inner box)")
  {
    IntervalVector box(2, Interval(2.8,3.));

    vector<Point> v_points;
    v_points.push_back(Point(1.,2.));
    v_points.push_back(Point(3.,4.));
    v_points.push_back(Point(5.,1.));
    v_points.push_back(Point(2.,1.));
    ConvexPolygon p(v_points);

    ConvexPolygon box_inter = p & box;
    ConvexPolygon p_truth(box);
    CHECK(box_inter == p_truth);

    ConvexPolygon p_inter = p & box;
    CHECK(p_inter == p_truth);
  }

  SECTION("Polygons intersections, test 5")
  {
    IntervalVector box(2, Interval(1.,4.));

    vector<Point> v_points;
    v_points.push_back(Point(2.,1.));
    v_points.push_back(Point(3.,1.));
    v_points.push_back(Point(4.,2.));
    v_points.push_back(Point(4.,3.));
    v_points.push_back(Point(3.,4.));
    v_points.push_back(Point(2.,4.));
    v_points.push_back(Point(1.,3.));
    v_points.push_back(Point(1.,2.));
    reverse(v_points.begin(), v_points.end());
    ConvexPolygon p(v_points);

    ConvexPolygon p_inter = p & box;
    CHECK(p_inter == p); // same polygon
  }

  SECTION("Polygons intersections, test 6 (shited polygon points declaration)")
  {
    IntervalVector box(2, Interval(1.,4.));

    vector<Point> v_points;
    v_points.push_back(Point(3.,4.));
    v_points.push_back(Point(2.,4.));
    v_points.push_back(Point(1.,3.));
    v_points.push_back(Point(1.,2.));
    v_points.push_back(Point(2.,1.));
    v_points.push_back(Point(3.,1.));
    v_points.push_back(Point(4.,2.));
    v_points.push_back(Point(4.,3.));
    reverse(v_points.begin(), v_points.end());
    ConvexPolygon p(v_points);

    ConvexPolygon p_inter = p & box;
    CHECK(p_inter == p); // same polygon
  }

  SECTION("Polygons intersections, test 7 (degenerate case)")
  {
    IntervalVector x(2);
    x[0] = 4000.;
    x[1] = 200.;

    vector<Point> v_pts;
    v_pts.push_back(Point(4000.,200.));
    ConvexPolygon p(v_pts);

    ConvexPolygon inter = p & x;
    CHECK(p == inter);
  }

  SECTION("Polygons intersections, test 8")
  {
    IntervalVector box(2);
    box[0] = Interval(2.,6.);
    box[1] = Interval(1.,5.);

    vector<Point> v_points;
    v_points.push_back(Point(1.,1.));
    v_points.push_back(Point(2.,4.));
    v_points.push_back(Point(7.,5.));
    v_points.push_back(Point(6.,2.));
    ConvexPolygon p(v_points);

    IntervalVector box_inter = p.fast_intersection(box);
    IntervalVector box_truth(2);
    box_truth[0] = Interval(2.,6.);
    box_truth[1] = Interval(1.+(1./5.),4.+(4./5.));
    CHECK(ApproxIntvVector(box_inter) == box_truth);

    ConvexPolygon p_inter = p & box;
    v_points.clear();
    v_points.push_back(Point(2.,1.+(1./5.)));
    v_points.push_back(Point(2.,4.));
    v_points.push_back(Point(6.,4.+(4./5.)));
    v_points.push_back(Point(6.,2.));
    ConvexPolygon p_truth(v_points);
    CHECK(p_truth.is_subset(p_inter) != NO);
    CHECK(ApproxVector(p_inter.vertices()[1]) == p_inter.vertices()[2]);
  }

  SECTION("Polygons intersections, test 9")
  {
    IntervalVector box(2);
    box[0] = 4.;
    box[0].inflate(1.);
    box[1] = Interval(1.,5.);

    vector<Point> v_points;
    v_points.push_back(Point(1.,1.));
    v_points.push_back(Point(2.,4.));
    v_points.push_back(Point(7.,5.));
    v_points.push_back(Point(6.,2.));
    ConvexPolygon p(v_points);

    IntervalVector box_inter = p.fast_intersection(box);
    IntervalVector box_truth(2);
    box_truth[0] = Interval(3.,5.);
    box_truth[1] = Interval(1.+(2./5.),4.+(3./5.));
    CHECK(ApproxIntvVector(box_inter) == box_truth);

    ConvexPolygon p_inter = p & box;
    v_points.clear();
    v_points.push_back(Point(3.,1.+(2./5.)));
    v_points.push_back(Point(5.,1.+(4./5.)));
    v_points.push_back(Point(5.,4.+(3./5.)));
    v_points.push_back(Point(3.,4.+(1./5.)));
    ConvexPolygon p_truth(v_points);
    CHECK(p_truth.is_subset(p_inter) != NO);
  }

  SECTION("Polygons intersections, test 10 (degenerated box)")
  {
    IntervalVector box(2);
    box[0] = 4.;
    box[1] = Interval(1.,5.);

    vector<Point> v_points;
    v_points.push_back(Point(1.,1.));
    v_points.push_back(Point(2.,4.));
    v_points.push_back(Point(7.,5.));
    v_points.push_back(Point(6.,2.));
    ConvexPolygon p(v_points);

    IntervalVector box_inter = p.fast_intersection(box);
    IntervalVector box_truth(2);
    box_truth[0] = 4.;
    box_truth[1] = Interval(1.+(3./5.),4.+(2./5.));
    CHECK(ApproxIntvVector(box_inter) == box_truth);

    ConvexPolygon p_inter = p & box;
    v_points.clear();
    v_points.push_back(Point(4.,1.+(3./5.)));
    v_points.push_back(Point(4.,4.+(2./5.)));
    ConvexPolygon p_truth(v_points);
    CHECK(p_truth.is_subset(p_inter) != NO);
  }
}

TEST_CASE("Polygons (Graham scan)")
{
  SECTION("Polygons, orientations")
  {
    IntervalVector p1({0.,0.});
    IntervalVector p2({1.,1.});
    IntervalVector p3({0.,2.});
    IntervalVector p4({2.,2.});
    IntervalVector p5({2.2,2.});
    CHECK(GrahamScan::orientation(p1, p2, p3) == OrientationInterval::COUNTERCLOCKWISE);
    CHECK(GrahamScan::orientation(p1, p2, p4) == OrientationInterval::UNDEFINED);
    CHECK(GrahamScan::orientation(p1, p2, p5) == OrientationInterval::CLOCKWISE);
  }

  SECTION("Polygons, Graham scan")
  {
    vector<Vector> v_pts;
    v_pts.push_back(Vector({0.,3.}));
    v_pts.push_back(Vector({1.,1.}));
    v_pts.push_back(Vector({2.,2.}));
    v_pts.push_back(Vector({4.,4.}));
    v_pts.push_back(Vector({0.,0.}));
    v_pts.push_back(Vector({1.,2.}));
    v_pts.push_back(Vector({3.,1.}));
    v_pts.push_back(Vector({3.,3.}));
    ConvexPolygon hull = GrahamScan::convex_hull(v_pts);

    CHECK(hull.vertices()[0] == Vector({0.,0.}));
    CHECK(hull.vertices()[1] == Vector({3.,1.}));
    CHECK(hull.vertices()[2] == Vector({4.,4.}));
    CHECK(hull.vertices()[3] == Vector({0.,3.}));

    v_pts.clear();
    v_pts.push_back(Vector({1.,3.}));
    v_pts.push_back(Vector({1.,4.}));
    v_pts.push_back(Vector({1.5,2.}));
    v_pts.push_back(Vector({2.,1.}));
    v_pts.push_back(Vector({2.,2.}));
    v_pts.push_back(Vector({3.,0.}));
    v_pts.push_back(Vector({3.,3.}));
    v_pts.push_back(Vector({3.,4.5}));
    v_pts.push_back(Vector({4.,2.5}));
    v_pts.push_back(Vector({4.,4.}));
    v_pts.push_back(Vector({5.,1.}));
    v_pts.push_back(Vector({5.,2.}));
    v_pts.push_back(Vector({4.,0.}));
    v_pts.push_back(Vector({5.,0.}));
    v_pts.push_back(Vector({5.,5.}));
    v_pts.push_back(Vector({6.,0.}));
    v_pts.push_back(Vector({7.,2.}));
    hull = GrahamScan::convex_hull(v_pts);

    CHECK(hull.vertices()[0] == Vector({3.,0.}));
    CHECK(hull.vertices()[1] == Vector({6.,0.}));
    CHECK(hull.vertices()[2] == Vector({7.,2.}));
    CHECK(hull.vertices()[3] == Vector({5.,5.}));
    CHECK(hull.vertices()[4] == Vector({3.,4.5}));
    CHECK(hull.vertices()[5] == Vector({1.,4.}));
    CHECK(hull.vertices()[6] == Vector({1.,3.}));

    //vibes::beginDrawing();
    //VIBesFig fig("poly");
    //fig.set_properties(100, 100, 400, 400);
    //fig.clear();
    //fig.draw_points(v_pts, 0.1, "blue");
    //fig.draw_polygon(GrahamScan::convex_hull(v_pts), "red");
    //fig.axis_limits(IntervalVector(2, Interval(-1.,8.)));
    //fig.show();
  }

  SECTION("Polygons, Graham scan, other")
  {
    vector<Point> v_pts;
    v_pts.push_back(Point(0.307007,0.0340742));
    v_pts.push_back(Point(0.0340742,0.307));
    v_pts.push_back(Point(0.1,0.2));
    v_pts.push_back(Point(0.1,0.2));
    v_pts.push_back(Point(0.2,0.1));
    v_pts.push_back(Point(0.1,0.1));

    ConvexPolygon s(v_pts);

    //#if VIBES_DRAWING // drawing results
    //  vibes::beginDrawing();
    //  VIBesFig fig("test");
    //  fig.set_properties(100, 100, 400, 400);
    //  fig.draw_polygon(s, "red");
    //  fig.draw_points(v_pts, 0.01, "blue");
    //  fig.axis_limits(fig.view_box() | s.box());
    //  vibes::endDrawing();
    //#endif
  }

  SECTION("Polygons, Graham scan, circle")
  {
    IntervalVector box(2, Interval(0.,1.));
    ConvexPolygon p(box);

    // One box is rotated/intersected several times

    for(double theta = 0. ; theta < 4.*M_PI ; theta+=M_PI/24.)
    {
      ConvexPolygon p_rotated(p);
      p_rotated.rotate(theta, box.mid());
      p = p & p_rotated;
      p.simplify(300);
    }

    //#if VIBES_DRAWING // drawing results
    //  vibes::beginDrawing();
    //  VIBesFig fig("test");
    //  fig.set_properties(100, 100, 400, 400);
    //  fig.draw_polygon(p, "red");
    //  fig.draw_circle(box.mid()[0], box.mid()[1], box[0].rad());
    //  fig.axis_limits(box, 0.1);
    //  vibes::endDrawing();
    //#endif

    vector<Vector> v_pts = p.vertices();

    bool contains = true;
    for(const auto& pt : v_pts)
      contains &= Interval(0.50,0.51).contains(sqrt(pow(pt[0]-0.5, 2) + pow(pt[1]-0.5, 2)));
    CHECK(contains);
  }
}

TEST_CASE("Polygons (operations)")
{
  SECTION("Polygons, rotation")
  {
   vector<Point> v_pts;
   v_pts.push_back(Point(1.,1.));
   v_pts.push_back(Point(3.,2.));
   v_pts.push_back(Point(2.,4.));
   
   ConvexPolygon p(v_pts);
   p.rotate(-M_PI/2., Vector({3.,1.}));
   
   // Rotated polygon truth
   vector<Point> v_pts_rot_truth;
   v_pts_rot_truth.push_back(Point(4.,1.));
   v_pts_rot_truth.push_back(Point(3.,3.));
   v_pts_rot_truth.push_back(Point(6.,2.));
   ConvexPolygon p_rot_truth(v_pts_rot_truth);
   
   CHECK(ApproxConvexPolygon(p) == p_rot_truth);
  }

  SECTION("Polygons, arithmetic")
  {
    vector<Point> v_truth(3);

    double theta = -M_PI/2.;

    IntervalMatrix rot_mat(2,2);
    rot_mat[0][0] = cos(theta); rot_mat[0][1] = -sin(theta);
    rot_mat[1][0] = sin(theta); rot_mat[1][1] =  cos(theta);

    IntervalVector c(2);
    c[0] = 3.; c[1] = 1.;

    vector<Point> v_pts;
    v_pts.push_back(Point(1.,1.));
    v_pts.push_back(Point(3.,2.));
    v_pts.push_back(Point(2.,4.));
    ConvexPolygon p(v_pts);

    v_truth[0] = Point(1.,1.);
    v_truth[1] = Point(3.,2.);
    v_truth[2] = Point(2.,4.);
    CHECK(ApproxConvexPolygon(p) == ConvexPolygon(v_truth));

    p = p - c;

    v_truth[0] = Point(1.-3.,1.-1.);
    v_truth[1] = Point(3.-3.,2.-1.);
    v_truth[2] = Point(2.-3.,4.-1.);
    CHECK(ApproxConvexPolygon(p) == ConvexPolygon(v_truth));

    p = rot_mat * p;

    v_truth[0] = Point(4.-3.,1.-1.);
    v_truth[1] = Point(3.-3.,3.-1.);
    v_truth[2] = Point(6.-3.,2.-1.);
    CHECK(ApproxConvexPolygon(p) == ConvexPolygon(v_truth));

    p = p + c;

    v_truth[0] = Point(4.,1.);
    v_truth[1] = Point(3.,3.);
    v_truth[2] = Point(6.,2.);
    CHECK(ApproxConvexPolygon(p) == ConvexPolygon(v_truth));
  }
}

TEST_CASE("Polygons (Graham scan, again)")
{
  SECTION("Polygons, Graham scan, step by step")
  {
    vector<Vector> v_pts;
    v_pts.push_back(Vector({0.,0.}));
    v_pts.push_back(Vector({8.,8.}));
    v_pts.push_back(Vector({10.,1.}));
    v_pts.push_back(Vector({4.,4.}));
    v_pts.push_back(Vector({-10.,1.}));
    v_pts.push_back(Vector({2.,2.}));
    v_pts.push_back(Vector({6.,3.}));
    v_pts.push_back(Vector({6.,1.}));
    v_pts.push_back(Vector({10.,4.}));

    CHECK(GrahamScan::orientation(Vector({0.,0.}), Vector({2.,2.}), Vector({2.,2.})) == OrientationInterval::UNDEFINED);
    CHECK(GrahamScan::orientation(Vector({0.,0.}), Vector({2.,2.}), Vector({4.,4.})) == OrientationInterval::UNDEFINED);
    CHECK(GrahamScan::orientation(Vector({0.,0.}), Vector({8.,8.}), Vector({4.,4.})) == OrientationInterval::UNDEFINED);
    CHECK(GrahamScan::orientation(Vector({0.,0.}), Vector({10.,1.}), Vector({4.,4.})) == OrientationInterval::COUNTERCLOCKWISE);
    CHECK(GrahamScan::orientation(Vector({0.,0.}), Vector({2.,2.}), Vector({10.,1.})) == OrientationInterval::CLOCKWISE);

    // Sort n-1 points with respect to the first point.

      // A point p1 comes before p2 in sorted ouput if p2
      // has larger polar angle (in counterclockwise
      // direction) than p1

    Vector p0 = v_pts[0];
    sort(v_pts.begin(), v_pts.end(), PointsSorter(p0));

    CHECK(v_pts.size() == 9);
    CHECK(v_pts[0] == Vector({0.,0.}));
    CHECK(v_pts[1] == Vector({10.,1.}));
    CHECK(v_pts[2] == Vector({6.,1.}));
    CHECK(v_pts[3] == Vector({10.,4.}));
    CHECK(v_pts[4] == Vector({6.,3.}));
    CHECK(v_pts[5] == Vector({2.,2.}));
    CHECK(v_pts[6] == Vector({4.,4.}));
    CHECK(v_pts[7] == Vector({8.,8.}));
    CHECK(v_pts[8] == Vector({-10.,1.}));

    // If two or more points make same angle with p0,
    // remove all but the one that is farthest from p0

      // Remember that, in above sorting, our criteria was
      // to keep the farthest point at the end when more than
      // one points have same angle.
      size_t m = 1; // Initialize size of modified array
      for(size_t i = 1 ; i < v_pts.size() ; i ++)
      {
        // Keep removing i while angle of i and i+1 is same
        // with respect to p0
        while(i < v_pts.size()-1 && Point::aligned(Point(p0), Point(v_pts[i]), Point(v_pts[i+1])) == YES)
          i++; 
        v_pts[m] = v_pts[i];
        m++; // Update size of modified array
      }

    CHECK(m == 7);
    CHECK(v_pts[0] == Vector({0.,0.}));
    CHECK(v_pts[1] == Vector({10.,1.}));
    CHECK(v_pts[2] == Vector({6.,1.}));
    CHECK(v_pts[3] == Vector({10.,4.}));
    CHECK(v_pts[4] == Vector({6.,3.}));
    CHECK(v_pts[5] == Vector({8.,8.}));
    CHECK(v_pts[6] == Vector({-10.,1.}));

    // Create an empty stack and push first three points to it.

      vector<Vector> v_hull;

      stack<Vector> s;
      s.push(v_pts[0]);
      s.push(v_pts[1]);
      s.push(v_pts[2]);

    CHECK(s.top() == v_pts[2]);
    CHECK(GrahamScan::next_to_top(s) == v_pts[1]);

    // Process remaining n-3 points

      for(size_t i = 3 ; i < m ; i++)
      {
        // Keep removing top while the angle formed by
        // points next-to-top, top, and v_pts[i] makes
        // a non-left turn
        while(s.size() > 1 && GrahamScan::orientation(GrahamScan::next_to_top(s), s.top(), v_pts[i]) == OrientationInterval::CLOCKWISE)
          s.pop();
        s.push(v_pts[i]);
      }

      while(!s.empty())
      {
        v_hull.push_back(s.top());
        s.pop();
      }
      reverse(v_hull.begin(), v_hull.end());

    CHECK(v_hull.size() == 5);
  }

  SECTION("Polygons, Graham scan, other example")
  {
    vector<Vector> v_pts;

    Vector p1({-4041.935273669676917052129283547401428223,-5492.667604696881426207255572080612182617});
    Vector p2({9206.843580880462468485347926616668701172,6551.674997467660432448610663414001464844});
    Vector p3({-4041.935273669676917052129283547401428223,-5492.667604696874150249641388654708862305});

    CHECK(p1[0] == p3[0]);
    CHECK(Point::aligned(Point(p1), Point(p2), Point(p3)) == NO);

    // 0
    v_pts.push_back(p1);
    // 1
    v_pts.push_back(Vector({-2103.177277725693329557543620467185974121,-5492.667604696881426207255572080612182617}));
    // 2
    v_pts.push_back(Vector({5720.923292917194885376375168561935424805,-975.4210340695084369144751690328121185303}));
    // 3
    v_pts.push_back(Vector({9206.843580880462468485347926616668701172,5062.370015818080901226494461297988891602}));
    // 4
    v_pts.push_back(Vector({52.79381299725321952109879930503666400909,5062.370015818080901226494461297988891602}));
    // 5
    v_pts.push_back(p3);
    // 6
    v_pts.push_back(p2);
    // 7
    v_pts.push_back(Vector({52.79381299725321952109879930503666400909,6551.674997467660432448610663414001464844}));
    // 8
    v_pts.push_back(Vector({-4041.935273669676917052129283547401428223,-540.603823869623056452837772667407989502}));

    vector<Vector> v_save(v_pts);

    vector<Vector> v_pts_bis;
    v_pts_bis.push_back(v_pts[4]);
    v_pts_bis.push_back(v_pts[6]);
    v_pts_bis.push_back(v_pts[3]);
    v_pts_bis.push_back(p1);
    v_pts_bis.push_back(p3);
    sort(v_pts_bis.begin(), v_pts_bis.end(), PointsSorter(p1));

    CHECK(v_pts_bis[0] == p1);
    CHECK(v_pts_bis[1] == v_pts[3]);
    CHECK(v_pts_bis[2] == v_pts[6]);
    CHECK(v_pts_bis[3] == v_pts[4]);
    CHECK(v_pts_bis[4] == p3);

    // Find the bottommost point

      size_t id_min = 0;
      double y_min = v_pts[0][1];

      for(size_t i = 1 ; i < v_pts.size() ; i++)
      {
        double y = v_pts[i][1];

        // Pick the bottom-most or chose the left most point in case of tie
        if((y < y_min) || (y_min == y && v_pts[i][0] < v_pts[id_min][0]))
        {
          y_min = v_pts[i][1];
          id_min = i;
        }
      }

    CHECK(p1 == v_pts[id_min]);

    sort(v_pts.begin(), v_pts.end(), PointsSorter(p1));

    CHECK(GrahamScan::orientation(p1, v_save[2], v_save[3]) == OrientationInterval::COUNTERCLOCKWISE);
    CHECK(GrahamScan::orientation(p1, v_save[6], v_save[5]) == OrientationInterval::COUNTERCLOCKWISE);

    CHECK(v_pts[0] == p1);
    CHECK(v_pts[1] == v_save[1]);
    CHECK(v_pts[2] == v_save[2]);
    CHECK(v_pts[3] == v_save[3]);
    CHECK(v_pts[4] == v_save[6]);
    CHECK(v_pts[5] == v_save[4]);
    CHECK(v_pts[6] == v_save[7]);
    CHECK(v_pts[7] == v_save[5]);
    CHECK(v_pts[8] == v_save[8]);

    //ConvexPolygon nc_polyg(v_pts);
    //CHECK(nc_polyg.vertices().size() == 9);
    ConvexPolygon polyg(v_pts);
    CHECK(polyg.vertices().size() == 7);

    //#if VIBES_DRAWING // drawing results
    //  vibes::beginDrawing();
    //  VIBesFig fig("polytest");
    //  fig.set_properties(100+1000, 1000+500, 400, 400);
    //  fig.draw_polygon(polyg, "black[#FD9335]");
    //  fig.axis_limits(fig.view_box() | polyg.box());
    //  vibes::endDrawing();
    //#endif
  }
}

TEST_CASE("Polygons (simplification)")
{
  SECTION("Polygons, simplification, test1")
  {
    vector<Vector> v_pts;
    v_pts.push_back(Vector({2.,0.}));
    v_pts.push_back(Vector({6.,4.}));
    v_pts.push_back(Vector({6.,5.}));
    v_pts.push_back(Vector({5.,6.}));
    v_pts.push_back(Vector({4.,6.}));
    v_pts.push_back(Vector({2.,3.}));

    ConvexPolygon p(v_pts);
    ConvexPolygon simple_p5(p), simple_p4(p), simple_p3(p);
    simple_p5.simplify(5);
    simple_p4.simplify(4);
    simple_p3.simplify(3);

    CHECK(p.nb_vertices() == 6);
    CHECK(simple_p3.nb_vertices() == 3);
    CHECK(simple_p4.nb_vertices() == 4);
    CHECK(simple_p5.nb_vertices() == 5);

    //#if VIBES_DRAWING // drawing results
    //  vibes::beginDrawing();
    //  VIBesFig fig("polytest 1");
    //  fig.set_properties(100+1000, 1000+500, 400, 400);
    //  fig.draw_polygon(p, "black[#B8DDFF]");
    //  fig.draw_polygon(simple_p3, "red");
    //  fig.draw_polygon(simple_p4, "green");
    //  fig.draw_polygon(simple_p5, "blue");
    //  fig.axis_limits(simple_p3.box() | simple_p4.box() | simple_p5.box() | p.box());
    //  vibes::endDrawing();
    //#endif
  }

  SECTION("Polygons, simplification, test2")
  {
    vector<Point> v_pts;
    v_pts.push_back(Point(2.,0.));
    v_pts.push_back(Point(0.,0.));
    v_pts.push_back(Point(0.,2.));
    v_pts.push_back(Point(2., 2.));

    ConvexPolygon p(v_pts), p_simple(p);
    CHECK(p.nb_vertices() == 4);
    p_simple.simplify(3);
    CHECK(p_simple.nb_vertices() == 4);

    //#if VIBES_DRAWING // drawing results
    //  vibes::beginDrawing();
    //  VIBesFig fig("polytest 2");
    //  fig.set_properties(100+1000, 1000+500, 400, 400);
    //  fig.draw_polygon(p, "black[#B8DDFF]");
    //  fig.draw_polygon(p_simple, "red");
    //  fig.axis_limits(p_simple.box() | p.box());
    //  vibes::endDrawing();
    //#endif
  }

  SECTION("Polygons, simplification, test3")
  {
    vector<Point> v_pts;
    int nb_pts = 8;
    for(int a = 0 ; a < nb_pts ; a++)
      v_pts.push_back(Point(cos(a*2.*M_PI/nb_pts),sin(a*2.*M_PI/nb_pts)));

    ConvexPolygon p(v_pts);
    vector<ConvexPolygon> simplified(6,p);

    for(size_t i = 0 ; i < simplified.size() ; i++)
      simplified[i].simplify(i+3);

    CHECK(p.nb_vertices() == 8);

    for(size_t i = 0 ; i < simplified.size() ; i++)
    {
      CHECK(simplified[i].nb_vertices() == max(4,(int)i+3));

      //#if VIBES_DRAWING // drawing results
      //  vibes::beginDrawing();
      //  VIBesFig fig(Figure::add_suffix("polytest edges:",i+3));
      //  fig.set_properties(100+i*410, 100, 400, 400);
      //  fig.draw_polygon(p, "white[#B8DDFF]");
      //  fig.draw_polygon(simplified[i], "purple");
      //  fig.draw_polygon(p, "#B8DDFF");
      //  fig.axis_limits(p.box());
      //  vibes::endDrawing();
      //#endif
    }
  }

  SECTION("Simplification, random polygon")
  {
    IntervalVector box(2,Interval(0.,10.));

    srand(time(nullptr));

    vector<Vector> v_pts;
    for(int i = 0 ; i < 500 ; i++)
    {
      Vector pt(2);
      pt[0] = (rand()/double(RAND_MAX))*box[0].diam()/2.;
      pt[1] = (rand()/double(RAND_MAX))*2.*M_PI;
      v_pts.push_back(Vector({box[0].mid()+pt[0]*cos(pt[1]),box[1].mid()+pt[0]*sin(pt[1])}));
    }

    ConvexPolygon p(v_pts);
    ConvexPolygon p4(p);
    p4.simplify(4);

    //vibes::beginDrawing();
    //VIBesFig fig("random polygon");
    //fig.set_properties(100, 100, 600, 600);
    //for(const auto& a : v_pts)
    //  fig.draw_circle(a[0], a[1], 0.01, "blue[blue]");
    //fig.draw_polygon(p, "red");
    //fig.draw_polygon(p4, "green");
    //fig.axis_limits(box.inflate(1.));
    //fig.show();
    //vibes::endDrawing();
  }

  SECTION("Testing is_subset")
  {
    ConvexPolygon p1(IntervalVector(2,Interval(-10.,10.)));
    ConvexPolygon p2(IntervalVector(2,Interval(-5.,5.)));
    CHECK(p2.is_subset(p1) == YES);

    ConvexPolygon p3(IntervalVector(2,Interval(-10.,5.)));
    CHECK(p3.is_subset(p1) == MAYBE);

    ConvexPolygon p4(IntervalVector(2,Interval(20.,25.)));
    CHECK(p4.is_subset(p1) == NO);

    ConvexPolygon p5(IntervalVector(2,0.));
    CHECK(p5.is_subset(p1) == YES);

    ConvexPolygon p6(IntervalVector(2,60.));
    CHECK(p6.is_subset(p1) == NO);

    ConvexPolygon p7(IntervalVector(2,10.));
    CHECK(p7.is_subset(p1) == MAYBE);
  }
}