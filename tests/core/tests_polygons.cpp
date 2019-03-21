#include "tests.h"

#include "tubex_GrahamScan.h"
#include "tubex_VIBesFig.h"

#define protected public     // Using #define so that we can access protected
#include "tubex_CtcDeriv.h"

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace tubex;

#define VIBES_DRAWING 0

TEST_CASE("Point")
{
  SECTION("Alignements")
  {
    Point p1(0.,0.);
    Point p2(0.,1.);
    Point p3(0.,10.);
    Point p4(1.,10.);
    Point p5(0.,9.+0.00000001*Interval(-1.,1.));
    Point p6(0.+0.00000001*Interval(-1.,1.),9.);
    Point p7(0.+0.00000001*Interval(-1.,1.),9.+0.00000001*Interval(-1.,1.));

    CHECK(Point::aligned(p1, p2, p3) == YES);
    CHECK(Point::aligned(p1, p2, p4) == NO);
    CHECK(Point::aligned(p1, p1, p4) == YES);
    CHECK(Point::aligned(p1, p2, p5) == YES);
    CHECK(Point::aligned(p1, p2, p6) == MAYBE);
    CHECK(Point::aligned(p1, p2, p7) == MAYBE);
  }

  SECTION("Contains")
  {
    Point p1(0.,0.), p2(1.5,3.), p3(0.,2.);
    Edge e1(Point(-10.,-10.), Point(10.,10.));
    Edge e2(Point(1.,1.), Point(10.,10.));
    Edge e3(Point(0.,0.), Point(2.,4.));

    CHECK(e1.contains(p1) != NO);
    CHECK(e2.contains(p1) == NO);
    CHECK(e3.contains(p2) != NO);
    CHECK(e3.contains(p3) == NO);
  }
}

TEST_CASE("Polygon")
{
  // todo: test Polygon constructor with redundant points

  SECTION("Polygon from IntervalVector")
  {
    IntervalVector iv(2);
    iv[0] = Interval(-1.,5.);
    iv[1] = Interval(10.,11.);

    Polygon p(iv);
    CHECK(p.nb_vertices() == 4);
    CHECK(p.box() == iv);
    CHECK(p[0] == Point(-1.,10.));
    CHECK(p[1] == Point(5.,10.));
    CHECK(p[2] == Point(5.,11.));
    CHECK(p[3] == Point(-1.,11.));
  }

  SECTION("Polygon from IntervalVector (unbounded case)")
  {
    IntervalVector iv1(2);
    iv1[0] = Interval(-1.,5.);
    Polygon p(iv1);
    
    CHECK(p[0] == Point(-1.,Interval::ALL_REALS));
    CHECK(p[1] == Point(5.,Interval::ALL_REALS));
    CHECK(p[2] == Point(5.,Interval::ALL_REALS));
    CHECK(p[3] == Point(-1.,Interval::ALL_REALS));
  }

  SECTION("Polygon from points (box shape)")
  {
    vector<Point> v_pts;
    v_pts.push_back(Point(2.,4.));
    v_pts.push_back(Point(2.,6.));
    v_pts.push_back(Point(4.,6.));
    v_pts.push_back(Point(4.,4.));

    Polygon p1(v_pts);
    CHECK(p1.nb_vertices() == 4);

    IntervalVector iv(2);
    iv[0] = Interval(2.,4.);
    iv[1] = Interval(4.,6.);
    CHECK(p1.box() == iv);

    Polygon p2(iv);
    CHECK(p2.nb_vertices() == 4);
    CHECK(p1 == p2);

    // Polygon defined by points in different order
    v_pts.clear();
    v_pts.push_back(Point(2.,6.));
    v_pts.push_back(Point(4.,6.));
    v_pts.push_back(Point(4.,4.));
    v_pts.push_back(Point(2.,4.));
    Polygon p3(v_pts);
    CHECK(p3 == p1);
    CHECK(p3 == p2);
  }

  SECTION("Polygon from points (unbounded case)")
  {
    IntervalVector iv1(2);
    iv1[0] = Interval(-1.,5.);
    iv1[1] = Interval::ALL_REALS;
    Polygon p1(iv1);

    vector<Point> v_pts;
    v_pts.push_back(Point(-1.,Interval::ALL_REALS));
    v_pts.push_back(Point(-1.,Interval::ALL_REALS));
    v_pts.push_back(Point(5.,Interval::ALL_REALS));
    v_pts.push_back(Point(5.,Interval::ALL_REALS));
    Polygon p2(v_pts);

    CHECK(p1 == p2);
    CHECK(p1.box() == iv1);
    CHECK(p2.box() == iv1);
  }

  SECTION("Polygon from points (random shape)")
  {
    vector<Point> v_p;
    v_p.push_back(Point(1.,3.));
    v_p.push_back(Point(3.,3.));
    v_p.push_back(Point(5.,6.));
    v_p.push_back(Point(5.,2.));
    v_p.push_back(Point(3.,2.));

    Polygon p(v_p);
    CHECK(p.nb_vertices() == 5);

    IntervalVector iv(2);
    iv[0] = Interval(1.,5.);
    iv[1] = Interval(2.,6.);
    CHECK(p.box() == iv);

    v_p.clear(); // points in different order
    v_p.push_back(Point(5.,2.));
    v_p.push_back(Point(3.,2.));
    v_p.push_back(Point(1.,3.));
    v_p.push_back(Point(3.,3.));
    v_p.push_back(Point(5.,6.));

    Polygon p_result(v_p);
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
    Polygon p1(v_p);

    v_p.clear();
    v_p.push_back(Point(-1., -1.));
    v_p.push_back(Point(-1., 2.));
    v_p.push_back(Point(0., 3.));
    v_p.push_back(Point(3., 0.));
    v_p.push_back(Point(3., -2.));
    v_p.push_back(Point(1.5, 1.5));
    v_p.push_back(Point(-3.5, -3.5));
    Polygon p2(v_p);

    CHECK(p1 == p2);
    CHECK(ApproxPolygon(p1) == p2); // testing Approx* class too
    CHECK_FALSE(p1 != p2);
    CHECK_FALSE(ApproxPolygon(p1) != p2); // testing Approx* class too
  }

  SECTION("Intersection, line/line")
  {
    CHECK((Edge(Point(0.,0.), Point(0.,0.)) & Edge(Point(0.,0.), Point(5.,0.))) == Point(0.,0.)); // degenerate line, horizontal edge line
    CHECK((Edge(Point(0.,0.), Point(0.,0.)) & Edge(Point(0.,0.), Point(0.,5.))) == Point(0.,0.)); // degenerate line, vertical edge line

    CHECK((Edge(Point(0.,0.), Point(0.,1.)) & Edge(Point(0.,0.), Point(0.,5.))) == Point(0.,Interval(0.,1.))); // vertical edge line (colinear)
    CHECK((Edge(Point(0.,0.), Point(0.,1.)) & Edge(Point(0.,0.), Point(0.,0.5))) == Point(0.,Interval(0.,0.5))); // vertical edge line (colinear)
    CHECK((Edge(Point(0.,0.), Point(1.,0.)) & Edge(Point(0.,0.), Point(5.,0.))) == Point(Interval(0.,1.),0.)); // horizontal edge line (colinear)
    CHECK((Edge(Point(0.,0.), Point(1.,0.)) & Edge(Point(0.,0.), Point(0.5,0.))) == Point(Interval(0.,0.5),0.)); // horizontal edge line (colinear)

    CHECK((Edge(Point(7.,4.), Point(7.,5.)) & Edge(Point(7.,6.), Point(7.,8.))) == Point(Interval::EMPTY_SET,Interval::EMPTY_SET)); // vertical edge line (colinear), no intersection
    CHECK((Edge(Point(4.,7.), Point(5.,7.)) & Edge(Point(6.,7.), Point(8.,7.))) == Point(Interval::EMPTY_SET,Interval::EMPTY_SET)); // horizontal edge line (colinear), no intersection

    CHECK((Edge(Point(0.,0.), Point(3.,0.)) & Edge(Point(1.,2.), Point(1.,-999.))) == Point(1.,0.)); // perpendicular intersection
    CHECK((Edge(Point(0.5,-1.), Point(0.5,5.)) & Edge(Point(-3.,2.), Point(5.,2.))) == Point(0.5,2.)); // perpendicular intersection

    CHECK((Edge(Point(0.,0.), Point(3.,0.)) & Edge(Point(1.,-10.), Point(1.,-999.))) == Point(Interval::EMPTY_SET,Interval::EMPTY_SET)); // perpendicular lines, no intersection
    CHECK((Edge(Point(0.5,-1.), Point(0.5,5.)) & Edge(Point(-3.,-2.), Point(5.,-2.))) == Point(Interval::EMPTY_SET,Interval::EMPTY_SET)); // perpendicular lines, no intersection

    CHECK((Edge(Point(8.,4.), Point(9.,2.)) & Edge(Point(7.,3.), Point(9.,3.))) == Point(8.5, 3.)); // perpendicular oblique lines
    CHECK((Edge(Point(8.,4.), Point(9.,2.)) & Edge(Point(8.5,8.), Point(8.5,0.))) == Point(8.5, 3.)); // perpendicular oblique lines

    CHECK((Edge(Point(8.,4.), Point(9.,2.)) & Edge(Point(7.,3.), Point(7.5,3.))) == Point(Interval::EMPTY_SET,Interval::EMPTY_SET)); // secant oblique lines, no intersection
    CHECK((Edge(Point(8.,4.), Point(9.,2.)) & Edge(Point(8.5,8.), Point(8.5,7.))) == Point(Interval::EMPTY_SET,Interval::EMPTY_SET)); // secant oblique lines, no intersection
    CHECK((Edge(Point(6.,-1.), Point(8.,1.)) & Edge(Point(7.5,0.), Point(9.,0.))) == Point(Interval::EMPTY_SET,Interval::EMPTY_SET)); // secant oblique lines, no intersection
    CHECK((Edge(Point(6.,-1.), Point(8.,1.)) & Edge(Point(6.5,0.5), Point(6.5,2.))) == Point(Interval::EMPTY_SET,Interval::EMPTY_SET)); // secant oblique lines, no intersection
  
    // Other tests
    CHECK((Edge(Point(8.,14.), Point(6.,13.)) & Edge(Point(1.,1.), Point(1.,14.))) == Point(Interval::EMPTY_SET,Interval::EMPTY_SET));
  }

  SECTION("Intersection, line/line, unbounded case")
  {


  }

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
    box_inter = p & x;
    CHECK(box_inter[0] == Interval(2./3.,3.));
    CHECK(box_inter[1] == Interval(0.,1.));

    x[0] = Interval(2.,6.); x[1] = Interval(2.,3.);
    box_inter = p & x;
    CHECK(box_inter[0] == Interval(2.,4.));
    CHECK(box_inter[1] == Interval(2.,3.));

    x[0] = Interval(3.,6.); x[1] = Interval(5.,6.);
    box_inter = p & x;
    CHECK(ApproxIntv(box_inter[0]) == Interval(3.,11./3.));
    CHECK(ApproxIntv(box_inter[1]) == Interval(5.,6.));

    x[0] = Interval(2.5,5.); x[1] = Interval(7.,10.);
    box_inter = p & x;
    CHECK(ApproxIntv(box_inter[0]) == Interval(2.5,3.));
    CHECK(ApproxIntv(box_inter[1]) == Interval(7.));

    x[0] = Interval(2.,3.); x[1] = Interval(3.,5.);
    box_inter = p & x;
    CHECK(box_inter == x);

    x[0] = Interval(-99.,0.); x[1] = Interval(1.,7.);
    box_inter = p & x;
    CHECK(ApproxIntv(box_inter[0]) == Interval(0.));
    CHECK(ApproxIntv(box_inter[1]) == Interval(3.,5.));

    x[0] = Interval(1.,2.); x[1] = Interval(3.); // degenerate case
    box_inter = p & x;
    CHECK(ApproxIntv(box_inter[0]) == Interval(1.,2.));
    CHECK(ApproxIntv(box_inter[1]) == Interval(3.));

    x[0] = Interval::ALL_REALS; x[1] = Interval::ALL_REALS; // whole envelope
    box_inter = p & x;
    CHECK(box_inter == p.box());

    x[0] = Interval(3.); x[1] = Interval(1.); // degenerate case
    box_inter = p & x;
    CHECK(ApproxIntv(box_inter[0]) == Interval(3.));
    CHECK(ApproxIntv(box_inter[1]) == Interval(1.));

    x[0] = Interval(1.); x[1] = Interval::ALL_REALS; // degenerate case
    box_inter = p & x;
    CHECK(ApproxIntv(box_inter[0]) == Interval(1.));
    CHECK(ApproxIntv(box_inter[1]) == Interval(0.,6.));

    x[0] = Interval(0.5,3.5); x[1] = Interval(0.5,6.5); // no contraction
    box_inter = p & x;
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
    box_inter = p & x;
    CHECK(box_inter[0] == Interval(1.,2.));
    CHECK(box_inter[1] == Interval(1.,2.));

    x[0] = Interval(0.,1.); x[1] = Interval(0.,1.);
    box_inter = p & x;
    CHECK(box_inter[0] == Interval(1.,1.));
    CHECK(box_inter[1] == Interval(1.,1.));

    x[0] = Interval(0.,0.9); x[1] = Interval(0.,0.9);
    box_inter = p & x;
    CHECK(box_inter[0] == Interval::EMPTY_SET);
    CHECK(box_inter[1] == Interval::EMPTY_SET);
    
    x[0] = Interval(-70.,1.); x[1] = Interval::ALL_REALS;
    box_inter = p & x;
    CHECK(box_inter[0] == Interval(1.));
    CHECK(box_inter[1] == Interval(1.,6.));

    x[0] = Interval(7.,8.); x[1] = Interval(7.,8.);
    box_inter = p & x;
    CHECK(box_inter[0] == Interval(7.,8.));
    CHECK(box_inter[1] == Interval(7.,8.));

    x[0] = Interval(7.); x[1] = Interval(7.);
    box_inter = p & x;
    CHECK(box_inter[0] == Interval(7.));
    CHECK(box_inter[1] == Interval(7.));

    x[0] = Interval(6.); x[1] = Interval(13.);
    box_inter = p & x;
    CHECK(box_inter[0] == Interval(6.));
    CHECK(box_inter[1] == Interval(13.));

    x[0] = Interval(7.,13.); x[1] = Interval(1.,2.);
    box_inter = p & x;
    CHECK(box_inter[0] == Interval(7.));
    CHECK(box_inter[1] == Interval(2.));

    x[0] = Interval(12.,16.); x[1] = Interval(3.,12.);
    box_inter = p & x;
    CHECK(box_inter[0] == Interval(12.,14.));
    CHECK(box_inter[1] == Interval(4.,11.));

    x[0] = Interval(7.); x[1] = Interval::POS_REALS;
    box_inter = p & x;
    CHECK(box_inter[0] == Interval(7.));
    CHECK(box_inter[1] == Interval(2.,13.5));

    x[0] = Interval::POS_REALS; x[1] = Interval::POS_REALS;
    box_inter = p & x;
    CHECK(box_inter[0] == Interval(1.,14.));
    CHECK(box_inter[1] == Interval(1.,14.));

    x[0] = Interval::POS_REALS; x[1] = Interval(8.);
    box_inter = p & x;
    CHECK(box_inter[0] == Interval(2.,13.75));
    CHECK(box_inter[1] == Interval(8.));

    x[0] = Interval::POS_REALS; x[1] = Interval(15.);
    box_inter = p & x;
    CHECK(box_inter[0] == Interval::EMPTY_SET);
    CHECK(box_inter[1] == Interval::EMPTY_SET);

    x[0] = Interval::NEG_REALS; x[1] = Interval(13.);
    box_inter = p & x;
    CHECK(box_inter[0] == Interval::EMPTY_SET);
    CHECK(box_inter[1] == Interval::EMPTY_SET);

    x[0] = Interval(1.); x[1] = Interval(2.,4.);
    box_inter = p & x;
    CHECK(box_inter[0] == Interval(1.));
    CHECK(box_inter[1] == Interval(2.,4.));

    x[0] = Interval(2.,7.); x[1] = Interval(1.,2.);
    box_inter = p & x;
    CHECK(box_inter[0] == Interval(2.,7.));
    CHECK(box_inter[1] == Interval(1.,2.));

    x[0] = Interval::EMPTY_SET; x[1] = Interval::EMPTY_SET;
    box_inter = p & x;
    CHECK(box_inter[0] == Interval::EMPTY_SET);
    CHECK(box_inter[1] == Interval::EMPTY_SET);
  }

  SECTION("Unbounded case, POS_INFINITY")
  {
    /*vector<Point> v_p;
    v_p.push_back(Point(1.,0.));
    v_p.push_back(Point(2.,0.));
    v_p.push_back(Point(4.,2.));
    v_p.push_back(Point(4.,4.));
    v_p.push_back(Point(3.,Interval(3.,POS_INFINITY)));
    v_p.push_back(Point(2.,Interval(3.,POS_INFINITY)));
    v_p.push_back(Point(0.,5.));
    v_p.push_back(Point(0.,3.));
    ConvexPolygon p(v_p);
    IntervalVector x(2), box_inter(2);

    CHECK(p.encloses(Point(3.5,8.)));

    x[0] = Interval(0.5,4.); x[1] = Interval(-1.,1.);
    box_inter = p & x;
    CHECK(box_inter[0] == Interval(2./3.,3.));
    CHECK(box_inter[1] == Interval(0.,1.));

    x[0] = Interval(2.,6.); x[1] = Interval(2.,3.);
    box_inter = p & x;
    CHECK(box_inter[0] == Interval(2.,4.));
    CHECK(box_inter[1] == Interval(2.,3.));

    x[0] = Interval(3.,6.); x[1] = Interval(5.,6.);
    box_inter = p & x;
    CHECK(ApproxIntv(box_inter[0]) == Interval(3.,4.));
    CHECK(ApproxIntv(box_inter[1]) == Interval(5.,6.));

    x[0] = Interval(2.5,5.); x[1] = Interval(7.,10.);
    box_inter = p & x;
    CHECK(ApproxIntv(box_inter[0]) == Interval(2.5,4.));
    CHECK(ApproxIntv(box_inter[1]) == Interval(7.,10.));

    x[0] = Interval(2.,3.); x[1] = Interval(3.,5.);
    box_inter = p & x;
    CHECK(box_inter == x);
cout << "-------------" << endl;
    x[0] = Interval(-99.,0.); x[1] = Interval(1.,7.);
    box_inter = p & x;
    CHECK(ApproxIntv(box_inter[0]) == Interval(0.));
    CHECK(ApproxIntv(box_inter[1]) == Interval(3.,7.));

    /*x[0] = Interval(0.,2.); x[1] = Interval(3.); // degenerate case
    box_inter = p & x;
    CHECK(ApproxIntv(box_inter[0]) == Interval(0.,2.));
    CHECK(ApproxIntv(box_inter[1]) == Interval(3.));

    x[0] = Interval::ALL_REALS; x[1] = Interval::ALL_REALS; // whole envelope
    box_inter = p & x;
    CHECK(ApproxIntv(box_inter[0]) == Interval(0.,4.));
    CHECK(ApproxIntv(box_inter[1]) == Interval(0.,POS_INFINITY));

    x[0] = Interval(3.); x[1] = Interval(1.); // degenerate case
    box_inter = p & x;
    CHECK(ApproxIntv(box_inter[0]) == Interval(3.));
    CHECK(ApproxIntv(box_inter[1]) == Interval(1.));

    x[0] = Interval(1.); x[1] = Interval::ALL_REALS; // degenerate case
    box_inter = p & x;
    CHECK(ApproxIntv(box_inter[0]) == Interval(1.));
    CHECK(ApproxIntv(box_inter[1]) == Interval(0.,POS_INFINITY));

    x[0] = Interval(0.5,3.5); x[1] = Interval(0.5,6.5); // no contraction
    box_inter = p & x;
    CHECK(ApproxIntv(box_inter[0]) == Interval(0.5,3.5));
    CHECK(ApproxIntv(box_inter[1]) == Interval(0.5,6.5));*/
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

    CHECK(ApproxPolygon(p1) == p2);
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

    CHECK(ApproxPolygon(p1) == p2);
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

    CHECK(ApproxPolygon(p1) == p2);
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

    CHECK(ApproxPolygon(p1) == p2);
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

    CHECK(ApproxPolygon(p1) == p2);
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

    CHECK(ApproxPolygon(p1) == p2);
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

    CHECK(ApproxPolygon(p1) == p2);
  }

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

    ConvexPolygon p_inter = ConvexPolygon::intersect(p, box);
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

    ConvexPolygon p_inter = ConvexPolygon::intersect(p, box);
    CHECK(p_inter.nb_vertices() == 4);

    v_points.clear();
    v_points.push_back(Point(2.,2.));
    v_points.push_back(Point(2.,3.));
    v_points.push_back(Point(3.,4.));
    v_points.push_back(Point(4.+1./3.,2.));
    ConvexPolygon p_truth(v_points);
    CHECK(ApproxPolygon(p_truth) == p_inter);
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

    ConvexPolygon box_inter = p & box;
    v_points.clear();
    v_points.push_back(Point(1.,1.));
    v_points.push_back(Point(1.,4.));
    v_points.push_back(Point(5.,4.));
    v_points.push_back(Point(5.,1.));
    ConvexPolygon p_truth(v_points);
    CHECK(box_inter == p_truth);

    ConvexPolygon p_inter = ConvexPolygon::intersect(p, box);
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

    ConvexPolygon p_inter = ConvexPolygon::intersect(p, box);
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

    ConvexPolygon box_inter = p & box;
    ConvexPolygon p_truth(box);
    CHECK(box_inter == p_truth);

    ConvexPolygon p_inter = ConvexPolygon::intersect(p, box);
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

    ConvexPolygon box_inter = p & box;
    ConvexPolygon p_truth(box);
    CHECK(box_inter == p_truth);

    ConvexPolygon p_inter = ConvexPolygon::intersect(p, box);
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

    ConvexPolygon inter = ConvexPolygon::intersect(p, x);
    CHECK(p == inter);
  }

  SECTION("Merging similar points")
  {
    Point p1(Interval(2.,4.), Interval(3.,4.));
    Point p2(Interval(4.,5.), Interval(2.,3.));
    CHECK(p1.x().intersects(p2.x()));
    CHECK(p1.y().intersects(p2.y()));

    vector<Point> v_pts;
    v_pts.push_back(Point(Interval(2.,3.), Interval(3.,6.)));
    v_pts.push_back(Point(Interval(3.,4.), Interval(3.,4.)));
    v_pts.push_back(Point(Interval(4.,5.), Interval(2.,3.)));
    v_pts.push_back(Point(Interval(3.,5.), Interval(0.,1.)));
    v_pts.push_back(Point(Interval(7.,8.), Interval(4.,6.)));
    v_pts.push_back(Point(Interval(7.5,9.), Interval(3.,4.5)));

    CHECK(v_pts.size() == 6);
    v_pts = Point::merge_close_points(v_pts);
    CHECK(v_pts.size() == 3);
    CHECK(v_pts[0] == Point(Interval(2.,5.), Interval(2.,6.)));
    CHECK(v_pts[1] == Point(Interval(3.,5.), Interval(0.,1.)));
    CHECK(v_pts[2] == Point(Interval(7.,9.), Interval(3.,6.)));
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

    IntervalVector box_inter = p & box;
    IntervalVector box_truth(2);
    box_truth[0] = Interval(2.,6.);
    box_truth[1] = Interval(1.+(1./5.),4.+(4./5.));
    CHECK(ApproxIntvVector(box_inter) == box_truth);

    ConvexPolygon p_inter = ConvexPolygon::intersect(p, box);
    v_points.clear();
    v_points.push_back(Point(2.,1.+(1./5.)));
    v_points.push_back(Point(2.,4.));
    v_points.push_back(Point(6.,4.+(4./5.)));
    v_points.push_back(Point(6.,2.));
    ConvexPolygon p_truth(v_points);
    CHECK(ApproxPolygon(p_inter).polygon().nb_vertices() == p_truth.nb_vertices());
    CHECK(ApproxPolygon(p_inter) == p_truth);
    CHECK(ApproxPoint(p_inter.vertices()[1]) == p_inter.vertices()[2]);
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

    IntervalVector box_inter = p & box;
    IntervalVector box_truth(2);
    box_truth[0] = Interval(3.,5.);
    box_truth[1] = Interval(1.+(2./5.),4.+(3./5.));
    CHECK(ApproxIntvVector(box_inter) == box_truth);

    ConvexPolygon p_inter = ConvexPolygon::intersect(p, box);
    v_points.clear();
    v_points.push_back(Point(3.,1.+(2./5.)));
    v_points.push_back(Point(5.,1.+(4./5.)));
    v_points.push_back(Point(5.,4.+(3./5.)));
    v_points.push_back(Point(3.,4.+(1./5.)));
    ConvexPolygon p_truth(v_points);
    CHECK(ApproxPolygon(p_inter) == p_truth);
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

    IntervalVector box_inter = p & box;
    IntervalVector box_truth(2);
    box_truth[0] = 4.;
    box_truth[1] = Interval(1.+(3./5.),4.+(2./5.));
    CHECK(ApproxIntvVector(box_inter) == box_truth);

    ConvexPolygon p_inter = ConvexPolygon::intersect(p, box);
    v_points.clear();
    v_points.push_back(Point(4.,1.+(3./5.)));
    v_points.push_back(Point(4.,4.+(2./5.)));
    ConvexPolygon p_truth(v_points);
    CHECK(ApproxPolygon(p_inter) == p_truth);
  }

  SECTION("Polygons, orientations")
  {
    Point p1(0.,0.);
    Point p2(1.,1.);
    Point p3(0.,2.);
    Point p4(2.,2.);
    Point p5(2.2,2.);
    CHECK(GrahamScan::orientation(p1, p2, p3) == COUNTERCLOCKWISE);
    CHECK(GrahamScan::orientation(p1, p2, p4) == UNDEFINED);
    CHECK(GrahamScan::orientation(p1, p2, p5) == CLOCKWISE);
  }

  SECTION("Polygons, Graham scan")
  {
    vector<Point> v_pts;
    v_pts.push_back(Point(0.,3.));
    v_pts.push_back(Point(1.,1.));
    v_pts.push_back(Point(2.,2.));
    v_pts.push_back(Point(4.,4.));
    v_pts.push_back(Point(0.,0.));
    v_pts.push_back(Point(1.,2.));
    v_pts.push_back(Point(3.,1.));
    v_pts.push_back(Point(3.,3.));
    ConvexPolygon hull = GrahamScan::convex_hull(v_pts);

    CHECK(hull.vertices()[0] == Point(0.,0.));
    CHECK(hull.vertices()[1] == Point(3.,1.));
    CHECK(hull.vertices()[2] == Point(4.,4.));
    CHECK(hull.vertices()[3] == Point(0.,3.));

    v_pts.clear();
    v_pts.push_back(Point(1.,3.));
    v_pts.push_back(Point(1.,4.));
    v_pts.push_back(Point(1.5,2.));
    v_pts.push_back(Point(2.,1.));
    v_pts.push_back(Point(2.,2.));
    v_pts.push_back(Point(3.,0.));
    v_pts.push_back(Point(3.,3.));
    v_pts.push_back(Point(3.,4.5));
    v_pts.push_back(Point(4.,2.5));
    v_pts.push_back(Point(4.,4.));
    v_pts.push_back(Point(5.,1.));
    v_pts.push_back(Point(5.,2.));
    v_pts.push_back(Point(4.,0.));
    v_pts.push_back(Point(5.,0.));
    v_pts.push_back(Point(5.,5.));
    v_pts.push_back(Point(6.,0.));
    v_pts.push_back(Point(7.,2.));
    hull = GrahamScan::convex_hull(v_pts);

    CHECK(hull.vertices()[0] == Point(3.,0.));
    CHECK(hull.vertices()[1] == Point(6.,0.));
    CHECK(hull.vertices()[2] == Point(7.,2.));
    CHECK(hull.vertices()[3] == Point(5.,5.));
    CHECK(hull.vertices()[4] == Point(3.,4.5));
    CHECK(hull.vertices()[5] == Point(1.,4.));
    CHECK(hull.vertices()[6] == Point(1.,3.));

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

    #if VIBES_DRAWING // drawing results
      vibes::beginDrawing();
      VIBesFig fig("test");
      fig.set_properties(100, 100, 400, 400);
      fig.draw_polygon(s, "red");
      fig.draw_points(v_pts, 0.01, "blue");
      fig.axis_limits(fig.view_box() | s.box());
      vibes::endDrawing();
    #endif
  }

  SECTION("Polygons, Graham scan, circle")
  {
    IntervalVector box(2, Interval(0.,1.));
    ConvexPolygon p(box);

    // One box is rotated/intersected several times

    int k = 0;
    for(double a = 0. ; a < 2.*M_PI ; a+=M_PI/12.)
    {
      ConvexPolygon q(box);
      q.rotate(a);
      p = ConvexPolygon::intersect(p, q);
    }

    vector<Point> v_pts = p.vertices();
    for(int i = 0 ; i < v_pts.size() ; i++)
      CHECK(sqrt(pow(v_pts[i].x() - 0.5, 2) + pow(v_pts[i].y() - 0.5, 2)).is_subset(Interval(0.48,0.52)));
  }

  SECTION("Polygons, Graham scan, step by step")
  {
    vector<Point> v_pts;
    v_pts.push_back(Point(0.,0.));
    v_pts.push_back(Point(8.,8.));
    v_pts.push_back(Point(10.,1.));
    v_pts.push_back(Point(4.,4.));
    v_pts.push_back(Point(-10.,1.));
    v_pts.push_back(Point(2.,2.));
    v_pts.push_back(Point(6.,3.));
    v_pts.push_back(Point(6.,1.));
    v_pts.push_back(Point(10.,4.));

    CHECK(GrahamScan::orientation(Point(0.,0.), Point(2.,2.), Point(2.,2.)) == UNDEFINED);
    CHECK(GrahamScan::orientation(Point(0.,0.), Point(2.,2.), Point(4.,4.)) == UNDEFINED);
    CHECK(GrahamScan::orientation(Point(0.,0.), Point(8.,8.), Point(4.,4.)) == UNDEFINED);
    CHECK(GrahamScan::orientation(Point(0.,0.), Point(10.,1.), Point(4.,4.)) == COUNTERCLOCKWISE);
    CHECK(GrahamScan::orientation(Point(0.,0.), Point(2.,2.), Point(10.,1.)) == CLOCKWISE);

    // Sort n-1 points with respect to the first point.

      // A point p1 comes before p2 in sorted ouput if p2
      // has larger polar angle (in counterclockwise
      // direction) than p1

    Point p0 = v_pts[0];
    sort(v_pts.begin(), v_pts.end(), PointsSorter(p0));

    CHECK(v_pts.size() == 9);
    CHECK(v_pts[0] == Point(0.,0.));
    CHECK(v_pts[1] == Point(10.,1.));
    CHECK(v_pts[2] == Point(6.,1.));
    CHECK(v_pts[3] == Point(10.,4.));
    CHECK(v_pts[4] == Point(6.,3.));
    CHECK(v_pts[5] == Point(2.,2.));
    CHECK(v_pts[6] == Point(4.,4.));
    CHECK(v_pts[7] == Point(8.,8.));
    CHECK(v_pts[8] == Point(-10.,1.));

    // If two or more points make same angle with p0,
    // remove all but the one that is farthest from p0

      // Remember that, in above sorting, our criteria was
      // to keep the farthest point at the end when more than
      // one points have same angle.
      int m = 1; // Initialize size of modified array
      for(int i = 1 ; i < v_pts.size() ; i ++)
      {
        // Keep removing i while angle of i and i+1 is same
        // with respect to p0
        while(i < v_pts.size()-1 && Point::aligned(p0, v_pts[i], v_pts[i+1]) == YES)
          i++; 
        v_pts[m] = v_pts[i];
        m++; // Update size of modified array
      }

    CHECK(m == 7);
    CHECK(v_pts[0] == Point(0.,0.));
    CHECK(v_pts[1] == Point(10.,1.));
    CHECK(v_pts[2] == Point(6.,1.));
    CHECK(v_pts[3] == Point(10.,4.));
    CHECK(v_pts[4] == Point(6.,3.));
    CHECK(v_pts[5] == Point(8.,8.));
    CHECK(v_pts[6] == Point(-10.,1.));

    // Create an empty stack and push first three points to it.

      vector<Point> v_hull;

      stack<Point> s;
      s.push(v_pts[0]);
      s.push(v_pts[1]);
      s.push(v_pts[2]);

    CHECK(s.top() == v_pts[2]);
    CHECK(GrahamScan::next_to_top(s) == v_pts[1]);

    // Process remaining n-3 points

      for(int i = 3 ; i < m ; i++)
      {
        // Keep removing top while the angle formed by
        // points next-to-top, top, and v_pts[i] makes
        // a non-left turn
        while(s.size() > 1 && GrahamScan::orientation(GrahamScan::next_to_top(s), s.top(), v_pts[i]) == CLOCKWISE)
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
    vector<Point> v_pts;

    Point p1(-4041.935273669676917052129283547401428223,-5492.667604696881426207255572080612182617);
    Point p2(9206.843580880462468485347926616668701172,6551.674997467660432448610663414001464844);
    Point p3(-4041.935273669676917052129283547401428223+0.,-5492.667604696874150249641388654708862305+0.);

    CHECK(Point::aligned(p1, p2, p3) == NO);

    v_pts.push_back(p1);
    v_pts.push_back(Point(-2103.177277725693329557543620467185974121,-5492.667604696881426207255572080612182617));
    v_pts.push_back(Point(5720.923292917194885376375168561935424805,-975.4210340695084369144751690328121185303));
    v_pts.push_back(Point(9206.843580880462468485347926616668701172,5062.370015818080901226494461297988891602));
    v_pts.push_back(Point(52.79381299725321952109879930503666400909,5062.370015818080901226494461297988891602));
    v_pts.push_back(p3);
    v_pts.push_back(p2);
    v_pts.push_back(Point(52.79381299725321952109879930503666400909,6551.674997467660432448610663414001464844));
    v_pts.push_back(Point(-4041.935273669676917052129283547401428223,-540.603823869623056452837772667407989502));

    // Find the bottommost point

      int id_min = 0;
      double y_min = v_pts[0].y().lb();

      for(int i = 1 ; i < v_pts.size() ; i++)
      {
        double y = v_pts[i].y().lb();

        // Pick the bottom-most or chose the left most point in case of tie
        if((y < y_min) || (y_min == y && v_pts[i].x().lb() < v_pts[id_min].x().lb()))
        {
          y_min = v_pts[i].y().lb();
          id_min = i;
        }
      }

    //CHECK(p1 == v_pts[id_min]);

    Polygon nc_polyg(v_pts);
    //CHECK(nc_polyg.vertices().size() == 8);
    ConvexPolygon polyg(v_pts);
    //CHECK(polyg.vertices().size() == 7);

    #if VIBES_DRAWING // drawing results
      vibes::beginDrawing();
      VIBesFig fig("polytest");
      fig.set_properties(100, 1000, 400, 400);
      fig.draw_polygon(polyg, "black[#FD9335]");
      fig.axis_limits(fig.view_box() | polyg.box());
      vibes::endDrawing();
    #endif
  }
}