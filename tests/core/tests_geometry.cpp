#include "catch_interval.hpp"
#include "codac_ThickPoint.h"
#include "codac_ThickEdge.h"
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
    ThickPoint p1(0.,0.);
    ThickPoint p2(0.,1.);
    ThickPoint p3(0.,10.);
    ThickPoint p4(1.,10.);
    ThickPoint p5(0.,9.+0.00000001*Interval(-1.,1.));
    ThickPoint p6(0.+0.00000001*Interval(-1.,1.),9.);
    ThickPoint p7(0.+0.00000001*Interval(-1.,1.),9.+0.00000001*Interval(-1.,1.));

    CHECK(ThickPoint::aligned(p1, p2, p3) == YES);
    CHECK(ThickPoint::aligned(p1, p2, p4) == NO);
    CHECK(ThickPoint::aligned(p1, p1, p4) == YES);
    CHECK(ThickPoint::aligned(p1, p2, p5) == YES);
    CHECK(ThickPoint::aligned(p1, p2, p6) == MAYBE);
    CHECK(ThickPoint::aligned(p1, p2, p7) == MAYBE);
  }

  SECTION("Contains")
  {
    ThickPoint p1(0.,0.), p2(1.5,3.), p3(0.,2.);
    ThickEdge e1(ThickPoint(-10.,-10.), ThickPoint(10.,10.));
    ThickEdge e2(ThickPoint(1.,1.), ThickPoint(10.,10.));
    ThickEdge e3(ThickPoint(0.,0.), ThickPoint(2.,4.));

    CHECK(e1.contains(p1) != NO);
    CHECK(e2.contains(p1) == NO);
    CHECK(e3.contains(p2) != NO);
    CHECK(e3.contains(p3) == NO);
  }

  SECTION("Intersection, line/line")
  {
    CHECK((ThickEdge(ThickPoint(0.,0.), ThickPoint(0.,0.)) & ThickEdge(ThickPoint(0.,0.), ThickPoint(5.,0.))) == ThickPoint(0.,0.)); // degenerate line, horizontal edge line
    CHECK((ThickEdge(ThickPoint(0.,0.), ThickPoint(0.,0.)) & ThickEdge(ThickPoint(0.,0.), ThickPoint(0.,5.))) == ThickPoint(0.,0.)); // degenerate line, vertical edge line

    CHECK((ThickEdge(ThickPoint(0.,0.), ThickPoint(0.,1.)) & ThickEdge(ThickPoint(0.,0.), ThickPoint(0.,5.))) == ThickPoint(0.,Interval(0.,1.))); // vertical edge line (colinear)
    CHECK((ThickEdge(ThickPoint(0.,0.), ThickPoint(0.,1.)) & ThickEdge(ThickPoint(0.,0.), ThickPoint(0.,0.5))) == ThickPoint(0.,Interval(0.,0.5))); // vertical edge line (colinear)
    CHECK((ThickEdge(ThickPoint(0.,0.), ThickPoint(1.,0.)) & ThickEdge(ThickPoint(0.,0.), ThickPoint(5.,0.))) == ThickPoint(Interval(0.,1.),0.)); // horizontal edge line (colinear)
    CHECK((ThickEdge(ThickPoint(0.,0.), ThickPoint(1.,0.)) & ThickEdge(ThickPoint(0.,0.), ThickPoint(0.5,0.))) == ThickPoint(Interval(0.,0.5),0.)); // horizontal edge line (colinear)

    CHECK((ThickEdge(ThickPoint(7.,4.), ThickPoint(7.,5.)) & ThickEdge(ThickPoint(7.,6.), ThickPoint(7.,8.))) == ThickPoint(Interval::EMPTY_SET,Interval::EMPTY_SET)); // vertical edge line (colinear), no intersection
    CHECK((ThickEdge(ThickPoint(4.,7.), ThickPoint(5.,7.)) & ThickEdge(ThickPoint(6.,7.), ThickPoint(8.,7.))) == ThickPoint(Interval::EMPTY_SET,Interval::EMPTY_SET)); // horizontal edge line (colinear), no intersection

    CHECK((ThickEdge(ThickPoint(0.,0.), ThickPoint(3.,0.)) & ThickEdge(ThickPoint(1.,2.), ThickPoint(1.,-999.))) == ThickPoint(1.,0.)); // perpendicular intersection
    CHECK((ThickEdge(ThickPoint(0.5,-1.), ThickPoint(0.5,5.)) & ThickEdge(ThickPoint(-3.,2.), ThickPoint(5.,2.))) == ThickPoint(0.5,2.)); // perpendicular intersection

    CHECK((ThickEdge(ThickPoint(0.,0.), ThickPoint(3.,0.)) & ThickEdge(ThickPoint(1.,-10.), ThickPoint(1.,-999.))) == ThickPoint(Interval::EMPTY_SET,Interval::EMPTY_SET)); // perpendicular lines, no intersection
    CHECK((ThickEdge(ThickPoint(0.5,-1.), ThickPoint(0.5,5.)) & ThickEdge(ThickPoint(-3.,-2.), ThickPoint(5.,-2.))) == ThickPoint(Interval::EMPTY_SET,Interval::EMPTY_SET)); // perpendicular lines, no intersection

    CHECK((ThickEdge(ThickPoint(8.,4.), ThickPoint(9.,2.)) & ThickEdge(ThickPoint(7.,3.), ThickPoint(9.,3.))) == ThickPoint(8.5, 3.)); // perpendicular oblique lines
    CHECK((ThickEdge(ThickPoint(8.,4.), ThickPoint(9.,2.)) & ThickEdge(ThickPoint(8.5,8.), ThickPoint(8.5,0.))) == ThickPoint(8.5, 3.)); // perpendicular oblique lines

    CHECK((ThickEdge(ThickPoint(8.,4.), ThickPoint(9.,2.)) & ThickEdge(ThickPoint(7.,3.), ThickPoint(7.5,3.))) == ThickPoint(Interval::EMPTY_SET,Interval::EMPTY_SET)); // secant oblique lines, no intersection
    CHECK((ThickEdge(ThickPoint(8.,4.), ThickPoint(9.,2.)) & ThickEdge(ThickPoint(8.5,8.), ThickPoint(8.5,7.))) == ThickPoint(Interval::EMPTY_SET,Interval::EMPTY_SET)); // secant oblique lines, no intersection
    CHECK((ThickEdge(ThickPoint(6.,-1.), ThickPoint(8.,1.)) & ThickEdge(ThickPoint(7.5,0.), ThickPoint(9.,0.))) == ThickPoint(Interval::EMPTY_SET,Interval::EMPTY_SET)); // secant oblique lines, no intersection
    CHECK((ThickEdge(ThickPoint(6.,-1.), ThickPoint(8.,1.)) & ThickEdge(ThickPoint(6.5,0.5), ThickPoint(6.5,2.))) == ThickPoint(Interval::EMPTY_SET,Interval::EMPTY_SET)); // secant oblique lines, no intersection
  
    // Other tests
    CHECK((ThickEdge(ThickPoint(8.,14.), ThickPoint(6.,13.)) & ThickEdge(ThickPoint(1.,1.), ThickPoint(1.,14.))) == ThickPoint(Interval::EMPTY_SET,Interval::EMPTY_SET));
  }

  SECTION("Intersection, line/line, unbounded case")
  {


  }

  SECTION("Intersection, line/line, another case")
  {
    ThickEdge e1(ThickPoint(2,0), ThickPoint(6,4));
    ThickEdge e2(ThickPoint(6,5), ThickPoint(5,6));
    ThickPoint p = ThickEdge::proj_intersection(e1,e2);
    CHECK(p == ThickPoint(6.5,4.5));
  }

  SECTION("ThickEdges, proj intersection")
  {
    //ThickPoint p1(-0.707106781186547461715008466854,0.707106781186547572737310929369);
    //ThickPoint p2(6.12323399573676603586882014729e-17,1);
    //ThickPoint p3(-1,1.22464679914735320717376402946e-16);
    //ThickPoint p4(-0.707106781186547683759613391885,-0.707106781186547461715008466854);

    ThickPoint p1(-0.7,0.7);
    ThickPoint p2(0.,1.);
    ThickPoint p3(-1,0.);
    ThickPoint p4(-0.7,-0.7);

    ThickEdge e1(p1,p2), e2(p3,p4);

    ThickPoint inter = ThickEdge::proj_intersection(e1, e2);
    CHECK(!inter.does_not_exist());
    CHECK(ApproxThickPoint(inter) == ThickPoint(-1.2068965517241383445, 0.48275862068965491591));
  }
}