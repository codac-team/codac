#include "catch_interval.hpp"
#include "codac_Point.h"
#include "codac_Edge.h"
#include "codac_VIBesFig.h"

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace codac;

#define VIBES_DRAWING 1

TEST_CASE("Geometry")
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

  SECTION("Intersection, line/line, another case")
  {
    Edge e1(Point(2,0), Point(6,4));
    Edge e2(Point(6,5), Point(5,6));
    Point p = Edge::proj_intersection(e1,e2);
    CHECK(p == Point(6.5,4.5));
  }

  SECTION("Edges, proj intersection")
  {
    //Point p1(-0.707106781186547461715008466854,0.707106781186547572737310929369);
    //Point p2(6.12323399573676603586882014729e-17,1);
    //Point p3(-1,1.22464679914735320717376402946e-16);
    //Point p4(-0.707106781186547683759613391885,-0.707106781186547461715008466854);

    Point p1(-0.7,0.7);
    Point p2(0.,1.);
    Point p3(-1,0.);
    Point p4(-0.7,-0.7);

    Edge e1(p1,p2), e2(p3,p4);

    Point inter = Edge::proj_intersection(e1, e2);
    CHECK(!inter.does_not_exist());
    CHECK(ApproxPoint(inter) == Point(-1.2068965517241383445, 0.48275862068965491591));
  }
}