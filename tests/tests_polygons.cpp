#include "tests.h"
#include "tubex_CtcDeriv.h"

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
    IntervalVector iv(2);
    iv[0] = Interval(-1.,5.);
    iv[1] = Interval(10.,11.);

    Polygon p(iv);
    CHECK(p.nb_vertices() == 4);
    CHECK(p.box() == iv);
    CHECK(p[0] == Point(-1.,10.));
    CHECK(p[1] == Point(-1.,11.));
    CHECK(p[2] == Point(5.,11.));
    CHECK(p[3] == Point(5.,10.));
  }

  SECTION("Polygon from IntervalVector (unbounded case)")
  {
    IntervalVector iv1(2);
    iv1[0] = Interval(-1.,5.);
    Polygon p(iv1);
    
    CHECK(p[0] == Point(-1.,Interval::ALL_REALS));
    CHECK(p[1] == Point(-1.,Interval::ALL_REALS));
    CHECK(p[2] == Point(5.,Interval::ALL_REALS));
    CHECK(p[3] == Point(5.,Interval::ALL_REALS));
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

    CHECK(p.get_edges()[0] == Edge(Point(1.,0.), Point(2.,0.)));
    CHECK(p.get_edges()[1] == Edge(Point(2.,0.), Point(4.,2.)));

    IntervalVector inter(2), x(2);
    
    Edge edge = p.get_edges()[0];
    x[0] = Interval(0.5,4.); x[1] = Interval(-1.,1.);
    inter = edge & x;
    CHECK(inter[0] == Interval(1.,2.));
    CHECK(inter[1] == Interval(0.));

    // Horizontal edge
    Edge edge1(Point(2.,0.), Point(9.,4.));
    Edge edge2(Point(3.,2.), Point(7.,2.));
    Point pt_inter = edge1 & edge2;
    CHECK(ApproxIntv(pt_inter.t()) == Point(5.5,2.).t());
    CHECK(ApproxIntv(pt_inter.x()) == Point(5.5,2.).x());

    // Vertical edge
    edge1 = Edge(Point(1.,4.), Point(3.,2.));
    edge2 = Edge(Point(2.,1.), Point(2.,7.));
    pt_inter = edge1 & edge2;
    CHECK(ApproxIntv(pt_inter.t()) == Point(2.,3.).t());
    CHECK(ApproxIntv(pt_inter.x()) == Point(2.,3.).x());

    // No intersection
    edge1 = Edge(Point(2.,0.), Point(4.,2.));
    edge2 = Edge(Point(0.,-1.), Point(4.,-1.));
    pt_inter = edge1 & edge2;
    CHECK(ApproxIntv(pt_inter.t()) == Interval::EMPTY_SET);
    CHECK(ApproxIntv(pt_inter.x()) == Interval::EMPTY_SET);

    edge = p.get_edges()[1];
    inter = edge & x;
    CHECK(inter[0] == Interval(2.,3.));
    CHECK(inter[1] == Interval(0.,1.));

    edge = p.get_edges()[2];
    inter = edge & x;
    CHECK(inter[0] == Interval::EMPTY_SET);
    CHECK(inter[1] == Interval::EMPTY_SET);

    edge = p.get_edges()[3];
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

    CHECK(p.encloses(Point(7.,7.)));
    CHECK(p.encloses(Point(1.,4.)));
    CHECK(p.encloses(Point(3.,10.)));
    CHECK(p.encloses(Point(8.,14.)));
    CHECK(p.encloses(Point(2.,8.)));
    CHECK(p.encloses(Point(2.5,9.)));
    CHECK_FALSE(p.encloses(Point(10.,2.)));
    CHECK_FALSE(p.encloses(Point(0.0,0.0)));
    CHECK_FALSE(p.encloses(Point(0.0,0.9)));
    CHECK_FALSE(p.encloses(Point(0.9,0.0)));
    CHECK_FALSE(p.encloses(Point(0.9,0.9)));

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

  SECTION("Polygons from TubeSlice, test 1")
  {
    TubeSlice x(Interval(-1.,3.), 1); // unbounded
    x.set_input_gate(IntervalVector(1, Interval(-1.,2.)));
    x.set_output_gate(IntervalVector(1, Interval(-2.,0.)));

    TubeSlice v(Interval(-1.,3.), IntervalVector(1, Interval(-1.,1.)));

    CtcDeriv ctc_deriv;
    ctc_deriv.contract(x, v);

    ConvexPolygon p1 = x.polygon(0, v);
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

  SECTION("Polygons from TubeSlice, test 2")
  {
    TubeSlice x(Interval(-1.,3.), IntervalVector(1, Interval(-5.,3.)));
    x.set_input_gate(IntervalVector(1, Interval(-1.,3.)));
    x.set_output_gate(IntervalVector(1, Interval(-5.,0.5)));

    TubeSlice v(Interval(-1.,3.), IntervalVector(1, Interval(-1.)));

    CtcDeriv ctc_deriv;
    ctc_deriv.contract(x, v);

    ConvexPolygon p1 = x.polygon(0, v);
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

  SECTION("Polygons from TubeSlice, test 3, degenerate case")
  {
    TubeSlice x(Interval(-1.,3.), IntervalVector(1, Interval(-5.,3.)));
    x.set_input_gate(IntervalVector(1, Interval(1.,3.)));
    x.set_output_gate(IntervalVector(1, Interval(-4.,-3.)));

    TubeSlice v(Interval(-1.,3.), IntervalVector(1, Interval(-1.,1.)));

    CtcDeriv ctc_deriv;
    ctc_deriv.contract(x, v);

    ConvexPolygon p1 = x.polygon(0, v);
    CHECK(p1.box()[0] == Interval(-1.,3.));
    CHECK(p1.box()[1] == Interval(-3.,1.));

    vector<Point> v_pts;
    v_pts.push_back(Point(-1.,1.));
    v_pts.push_back(Point(3.,-3.));
    ConvexPolygon p2(v_pts);

    CHECK(ApproxPolygon(p1) == p2);
  }

  SECTION("Polygons from TubeSlice, test 4")
  {
    TubeSlice x(Interval(0.,4.), IntervalVector(1, Interval(-1.,7.)));
    x.set_input_gate(IntervalVector(1, Interval(2.,3.)));
    x.set_output_gate(IntervalVector(1, Interval(3.,4.)));

    TubeSlice v(Interval(0.,4.), IntervalVector(1, Interval(-1.5,4.)));

    CtcDeriv ctc_deriv;
    ctc_deriv.contract(x, v);

    ConvexPolygon p1 = x.polygon(0, v);
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

  SECTION("Polygons from TubeSlice, test 4")
  {
    TubeSlice x(Interval(4.,8.), IntervalVector(1, Interval(-1.,7.)));
    x.set_input_gate(IntervalVector(1, Interval(3.,4.)));
    x.set_output_gate(IntervalVector(1, Interval(1.)));

    TubeSlice v(Interval(4.,8.), IntervalVector(1, Interval(-0.75,-0.5)));

    CtcDeriv ctc_deriv;
    ctc_deriv.contract(x, v);

    ConvexPolygon p1 = x.polygon(0, v);
    CHECK(p1.box()[0] == Interval(4.,8.));
    CHECK(p1.box()[1] == Interval(1.,4.));

    vector<Point> v_pts;
    v_pts.push_back(Point(4.,3.));
    v_pts.push_back(Point(4.,4.));
    v_pts.push_back(Point(8.,1.));
    ConvexPolygon p2(v_pts);

    CHECK(ApproxPolygon(p1) == p2);
  }

  SECTION("Polygons from TubeSlice, test 5")
  {
    TubeSlice x(Interval(8.,12.), IntervalVector(1, Interval(-1.,7.)));
    x.set_input_gate(IntervalVector(1, Interval(1.)));
    x.set_output_gate(IntervalVector(1, Interval(1.)));

    TubeSlice v(Interval(8.,12.), IntervalVector(1, Interval(-1./3.,1.)));

    CtcDeriv ctc_deriv;
    ctc_deriv.contract(x, v);

    ConvexPolygon p1 = x.polygon(0, v);
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

  SECTION("Polygons from TubeSlice, test 6")
  {
    TubeSlice x(Interval(12.,14.), IntervalVector(1, Interval(-1.,7.)));
    x.set_input_gate(IntervalVector(1, Interval(1.)));
    x.set_output_gate(IntervalVector(1, Interval(5.5)));

    TubeSlice v(Interval(12.,14.), IntervalVector(1, Interval(4.5)/2.));

    CtcDeriv ctc_deriv;
    ctc_deriv.contract(x, v);

    ConvexPolygon p1 = x.polygon(0, v);
    CHECK(p1.box()[0] == Interval(12.,14.));
    CHECK(ApproxIntv(p1.box()[1]) == Interval(1.,5.5));

    vector<Point> v_pts;
    v_pts.push_back(Point(12.,1.));
    v_pts.push_back(Point(14,5.5));
    ConvexPolygon p2(v_pts);

    CHECK(ApproxPolygon(p1) == p2);
  }
}