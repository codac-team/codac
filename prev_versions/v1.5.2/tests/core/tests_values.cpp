#include "catch_interval.hpp"
#include "codac_CtcEval.h"
#include "codac_VIBesFigTube.h"
#include "codac_VIBesFigTubeVector.h"
#include "tests_predefined_tubes.h"
#include "vibes.h"

// Using #define so that we can access protected methods
// of the class for tests purposes
#define protected public
#include "codac_CtcDeriv.h"

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace codac;

#define VIBES_DRAWING 0

TEST_CASE("Tube values")
{
  SECTION("Test x1")
  {
    Tube x = tube_test_1();
    x.set(Interval(-4,2), 14); // to test primitives pre-computation

    CHECK(x.nb_slices() == 46);

    CHECK(x( 0) == Interval(4,8));
    CHECK(x( 1) == Interval(2,7));
    CHECK(x( 2) == Interval(1,6));
    CHECK(x( 3) == Interval(-4,4));
    CHECK(x( 4) == Interval(-7,-1));
    CHECK(x( 5) == Interval(-9,-5));
    CHECK(x( 6) == Interval(-10,-6));
    CHECK(x( 7) == Interval(-11,-7));
    CHECK(x( 8) == Interval(-10,-6));
    CHECK(x( 9) == Interval(-9,-4));
    CHECK(x(10) == Interval(-8,-5));
    CHECK(x(11) == Interval(-7,-4));
    CHECK(x(12) == Interval(-6,-2));
    CHECK(x(13) == Interval(-5,-1));
    CHECK(x(14) == Interval(-4,2));
    CHECK(x(15) == Interval(-2,4));
    CHECK(x(16) == Interval(0,6));
    CHECK(x(17) == Interval(2,7));
    CHECK(x(18) == Interval(4,8));
    CHECK(x(19) == Interval(6,9));
    CHECK(x(20) == Interval(7,10));
    CHECK(x(21) == Interval(8,11));
    CHECK(x(22) == Interval(9,12));
    CHECK(x(23) == Interval(8,13));
    CHECK(x(24) == Interval(7,12));
    CHECK(x(25) == Interval(5,11));
    CHECK(x(26) == Interval(3,10));
    CHECK(x(27) == Interval(4,9));
    CHECK(x(28) == Interval(5,8));
    CHECK(x(29) == Interval(4,7));
    CHECK(x(30) == Interval(3,6));
    CHECK(x(31) == Interval(3,5));
    CHECK(x(32) == Interval(2,5));
    CHECK(x(33) == Interval(2,5));
    CHECK(x(34) == Interval(1,5));
    CHECK(x(35) == Interval(2,4));
    CHECK(x(36) == Interval(1,4));
    CHECK(x(37) == Interval(0,4));
    CHECK(x(38) == Interval(-1,3));
    CHECK(x(39) == Interval(-1,3));
    CHECK(x(40) == Interval(-1,4));
    CHECK(x(41) == Interval(0,5));
    CHECK(x(42) == Interval(1,6));
    CHECK(x(43) == Interval(0,5));
    CHECK(x(44) == Interval(-1,4));
    CHECK(x(45) == Interval(-1,3));

    CHECK(x(7) == Interval(-11,-7));
    CHECK(x(3.5) == Interval(-4,4));
    CHECK(x(17.5) == Interval(2,7));
    CHECK(x(32.5) == Interval(2,5));
    CHECK(x(17.0) == Interval(2,6)); // t belongs to two slices
    CHECK(x(16.9999999) == Interval(0,6));
    CHECK(x(17.0000001) == Interval(2,7));
    CHECK(x(Interval(9.5,17.5)) == Interval(-9,7));
    CHECK(x(Interval(16.1,37.9)) == Interval(0,13));
    CHECK(x(Interval(37.4,45.6)) == Interval(-1,6));
    CHECK(x(Interval(14.0,27.0)) == Interval(-4,13));
    CHECK(x(Interval(19.2,35.7)) == Interval(1,13));
    CHECK(x(Interval(15.0,16.0)) == Interval(-2,4));
    CHECK(x(Interval(15.2,15.3)) == Interval(-2,4));
    CHECK(x(15.2) == Interval(-2,4));
    CHECK(x(Interval(0.)) == Interval(4,8));
    CHECK(x(Interval(5.)) == Interval(-7,-5));
    CHECK(x(Interval(24.)) == Interval(8,12));
    CHECK(x(Interval(46.)) == Interval(-1,3));
    CHECK(x.codomain() == Interval(-11,13));
  }

  SECTION("Setting values over an interval domain")
  {
    Tube x(Interval(0.,10.));

    // Unbounded interval domain
    x.set(Interval(6.,10.));
    CHECK(x.nb_slices() == 1);
    CHECK(x.slice(0)->tdomain() == Interval(0.,10.));
    CHECK(x.slice(0)->codomain() == Interval(6.,10.));

    // Bounded interval domain
    x.set(Interval(2.,4.), Interval(2.,3.));
    CHECK(x.nb_slices() == 3);
    CHECK(x.slice(0)->tdomain() == Interval(0.,2.));
    CHECK(x(0) == Interval(6.,10.));
    CHECK(x.slice(1)->tdomain() == Interval(2.,3.));
    CHECK(x(1) == Interval(2.,4.));
    CHECK(x.slice(2)->tdomain() == Interval(3.,10.));
    CHECK(x(2) == Interval(6.,10.));

    // Gates, slices intersection
    x.set(Interval(1.,3.), Interval(0.,2.));
    CHECK(x.nb_slices() == 3);
    CHECK(x(0) == Interval(1.,3.));
    CHECK(x(0.) == Interval(1.,3.));
    CHECK(x(2.) == Interval(2.,3.));
    CHECK(x(2.01) == Interval(2.,4.));
    CHECK(x(3.) == Interval::EMPTY_SET);
    x.set(Interval(4.,5.), Interval(3.,4.));
    CHECK(x(3.) == Interval(4.));
  }

  SECTION("Setting values over an interval domain (bis)")
  {
    Tube x(Interval(0.,8.));
    x.set(Interval(-3.,1.), 0.);
    x.set(Interval(0.,1.), 2.);
    x.set(Interval(-1.,1.), 4.);
    x.set(Interval(-1.,0.), 6.);
    x.set(Interval(-2.,1.), 8.);

    CHECK(x.nb_slices() == 4);

    x.set(Interval(-3.,2.), Interval(0.,2.));
    x.set(Interval(-1.,3.), Interval(2.,4.));
    x.set(Interval(-3.,2.), Interval(4.,6.));
    x.set(Interval(-2.,1.), Interval(6.,8.));

    CHECK(x(0) == Interval(-3.,2.));
    CHECK(x(1) == Interval(-1.,3.));
    CHECK(x(2) == Interval(-3.,2.));
    CHECK(x(3) == Interval(-2.,1.));
    CHECK(x.codomain() == Interval(-3.,3.));

    // The gates have been changed too
    CHECK(x(0.) == Interval(-3.,2.));
    CHECK(x(2.) == Interval(-1.,2.));
    CHECK(x(4.) == Interval(-1.,2.));
    CHECK(x(6.) == Interval(-2.,1.));
    CHECK(x(8.) == Interval(-2.,1.));

    // Setting again the gates
    x.set(Interval(-3.,1.), 0.);
    x.set(Interval(0.,1.), 2.);
    x.set(Interval(-1.,1.), 4.);
    x.set(Interval(-1.,0.), 6.);
    x.set(Interval(-2.,1.), 8.);

    // Checking new values
    CHECK(x(0.) == Interval(-3.,1.));
    CHECK(x(2.) == Interval(0.,1.));
    CHECK(x(4.) == Interval(-1.,1.));
    CHECK(x(6.) == Interval(-1.,0.));
    CHECK(x(8.) == Interval(-2.,1.));

    // New setting over all slices
    x.set(Interval(-1.5,1.5), Interval(1.,7.));

    // Checking gates...
    CHECK(x(0.) == Interval(-3.,1.)); // not changed
    CHECK(x(1.) == Interval(-1.5,1.5)); // new gate
    CHECK(x(2.) == Interval(-1.5,1.5));
    CHECK(x(4.) == Interval(-1.5,1.5));
    CHECK(x(6.) == Interval(-1.5,1.5));
    CHECK(x(7.) == Interval(-1.5,1.)); // new gate
    CHECK(x(8.) == Interval(-2.,1.)); // not changed

    // ...and values
    CHECK(x(0) == Interval(-3.,2.)); // not changed
    CHECK(x(1) == Interval(-1.5,1.5));
    CHECK(x(2) == Interval(-1.5,1.5));
    CHECK(x(3) == Interval(-1.5,1.5));
    CHECK(x(4) == Interval(-1.5,1.5));
    CHECK(x(5) == Interval(-2.,1.)); // not changed
  }

  SECTION("Setting values at some precise time input")
  {
    Tube x(Interval(0.,10.));
    x.set(Interval(3.,4.), 2.);
    CHECK(x.nb_slices() == 2);
    CHECK(x.slice(0)->tdomain() == Interval(0.,2.));
    CHECK(x.slice(1)->tdomain() == Interval(2.,10.));
    CHECK(x(0) == Interval::ALL_REALS);
    CHECK(x(1) == Interval::ALL_REALS);
    CHECK(x(2.) == Interval(3.,4.));
  }

  SECTION("Test x4")
  {
    Tube x = tube_test4();
    CHECK(x.codomain() == Interval(-1.5,2));
    CHECK(x(0) == Interval(1.,2.));
    CHECK(x(10.2) == Interval(0.5,1.0));
    CHECK(x(11.0) == Interval::EMPTY_SET);
    CHECK(x(1.5) == Interval(1.,2.));
  }
}

TEST_CASE("Testing enclosed bounds (x evaluations)")
{
  SECTION("Test x1")
  {
    Tube x1 = tube_test_1();
    x1.set(Interval(-4,2), 14); // to test primitives pre-computation

    CHECK(ApproxIntvPair(x1.eval(x1.tdomain()))  == make_pair(Interval(-11,9), Interval(-7,13)));
    CHECK(x1.eval(Interval(0.5,25.5)) == make_pair(Interval(-11,9), Interval(-7,13)));
    CHECK(x1.eval(Interval(7.1,19.8)) == make_pair(Interval(-11,6), Interval(-7,9)));
    CHECK(x1.eval(Interval(6.0,9.0)) == make_pair(Interval(-11,-9), Interval(-7,-6)));
    CHECK(x1.eval(Interval(0.)) == make_pair(Interval(4), Interval(8)));
    CHECK(x1.eval(Interval(5.)) == make_pair(Interval(-7), Interval(-5)));
    CHECK(x1.eval(Interval(5.2)) == make_pair(Interval(-9), Interval(-5)));
    CHECK(x1.eval(Interval(23.2,23.8)) == make_pair(Interval(8), Interval(13)));
    CHECK(x1.eval(Interval(23.,24.)) == make_pair(Interval(8.,9.), Interval(12.,13.)));
    CHECK(x1.eval(Interval(22.,25.)) == make_pair(Interval(7,9), Interval(11,13)));
    CHECK(x1.eval(Interval(21.,23.)) == make_pair(Interval(8,9), Interval(10,12)));
  }

  SECTION("Test x4")
  {
    Tube x4 = tube_test4();
    x4.set(Interval(-1,1), Interval(10,11));
    Tube x4_primitive = x4.primitive();
    CHECK(x4_primitive.eval(Interval(12.5,14.5)) == make_pair(Interval(6,6.5), Interval(21,24.5)));
  }
}

TEST_CASE("Testing set inversion")
{
  SECTION("Scalar set inversion (Slice)")
  {
    Slice slice(Interval(0.,1.), Interval(0.,10.));
    slice.set_input_gate(Interval(2.,3.));
    slice.set_output_gate(Interval(5.,6.));
    CHECK(slice.invert(Interval(4.,6.), Interval(0.)) == Interval::EMPTY_SET);
    CHECK(slice.invert(Interval(2.5,6.), Interval(0.)) == 0.);
    CHECK(slice.invert(Interval(0.,1.), Interval(1.)) == Interval::EMPTY_SET);
    CHECK(slice.invert(Interval(2.5,6.), Interval(1.)) == 1.);
    CHECK(slice.invert(Interval(2.5,6.), Interval(0.2,0.5)) == Interval(0.2,0.5));
    CHECK(slice.invert(Interval(2.5,6.), Interval(0.2)) == Interval(0.2));
  }

  SECTION("Scalar set inversion (Slice, unbounded derivative)")
  {
    Slice sx(Interval(0.,1.), Interval(0.,10.));
    Slice sv(Interval(0.,1.), Interval::all_reals());
    CHECK(sx.invert(5., sv, sx.tdomain()) == Interval(0.,1.));
    CHECK(sx.invert(15., sv, sx.tdomain()) == Interval::empty_set());
  }

  SECTION("Scalar set inversion (Tube)")
  {
    Tube x = tube_test_1();
    x.set(Interval(-4,2), 14); // to test primitives pre-computation
    CHECK(x.invert(Interval(0.), x.tdomain()) == Interval(3.0,46.0));
    CHECK(x.invert(Interval(-7.), x.tdomain()) == Interval(4.0,12.0));
    CHECK(x.invert(Interval::ALL_REALS, x.tdomain()) == Interval(0.0,46.0));
    CHECK(x.invert(Interval(-12.0,14.0), x.tdomain()) == Interval(0.0,46.0));
    CHECK(x.invert(Interval(-20,-18), x.tdomain()) == Interval::EMPTY_SET);
    CHECK(x.invert(Interval(-1.0,1.0), x.tdomain()) == Interval(2.0,46.0));
    CHECK(x.invert(Interval(-10.5), x.tdomain()) == Interval(7.0,8.0));
    CHECK(x.invert(Interval(-12.0,-7.0), x.tdomain()) == Interval(4.0,12.0));
    CHECK(x.invert(Interval(10.0,11.0), x.tdomain()) == Interval(20.0,27.0));
    CHECK(x.invert(Interval(6.01,7.0), x.tdomain()) == Interval(0.0,30.0));
    CHECK(x.invert(Interval(6.0,7.0), x.tdomain()) == Interval(0.0,43.0));
    CHECK(x.invert(Interval(5.9,7.0), x.tdomain()) == Interval(0.0,43.0));
  }
  
  SECTION("Vector set inversion")
  {
    Tube x = tube_test_1();
    x.set(Interval(-4,2), 14); // to test primitives pre-computation

    vector<Interval> v;

    x.invert(Interval(0.), v, x.tdomain());
    CHECK(v.size() == 4);

    if(v.size() == 4)
    {
      CHECK(v[0] == Interval(3.0,4.0));
      CHECK(v[1] == Interval(14.0,17.0));
      CHECK(v[2] == Interval(37.0,42.0));
      CHECK(v[3] == Interval(43.0,46.0));
    }

    // The same, with a custom domain:
    x.invert(Interval(0.), v, Interval(3.8,42.5));
    CHECK(v.size() == 3);

    if(v.size() == 3)
    {
      CHECK(v[0] == Interval(3.8,4.0));
      CHECK(v[1] == Interval(14.0,17.0));
      CHECK(v[2] == Interval(37.0,42.0));
    }

    x.invert(Interval(-1.0,1.0), v, x.tdomain());
    CHECK(v.size() == 4);

    if(v.size() == 4)
    {
      CHECK(v[0] == Interval(2.0,5.0));
      CHECK(v[1] == Interval(13.0,17.0));
      CHECK(v[2] == Interval(34.0,35.0));
      CHECK(v[3] == Interval(36.0,46.0));
    }

    // The same, with a custom domain (empty):
    x.invert(Interval(-1.0,1.0), v, Interval::empty_set());
    CHECK(v.size() == 0);

    x.invert(Interval(-6.9999), v, x.tdomain());
    CHECK(v.size() == 2);

    if(v.size() == 2)
    {
      CHECK(v[0] == Interval(4.,7.));
      CHECK(v[1] == Interval(8.,12.));
    }

    x.invert(Interval::ALL_REALS, v, x.tdomain());
    CHECK(v.size() == 1);

    if(v.size() == 1)
      CHECK(v[0] == Interval(0.0,46.0));

    x.invert(Interval(-30.0,-29.0), v, x.tdomain());
    CHECK(v.size() == 0);

    x.invert(Interval(3.5), v, x.tdomain());
    CHECK(v.size() == 5);

    if(v.size() == 5)
    {
      CHECK(v[0] == Interval(1.0,4.0));
      CHECK(v[1] == Interval(15.0,18.0));
      CHECK(v[2] == Interval(26.0,27.0));
      CHECK(v[3] == Interval(30.0,38.0));
      CHECK(v[4] == Interval(40.0,45.0));
    }

    x.invert(Interval(9.5,30.0), v, x.tdomain());
    CHECK(v.size() == 1);

    if(v.size() == 1)
      CHECK(v[0] == Interval(20.0,27.0));

    x.invert(Interval(12.0,13.0), v, x.tdomain());
    CHECK(v.size() == 1);

    if(v.size() == 1)
      CHECK(v[0] == Interval(22.0,25.0));

    x.invert(Interval(-4.0,-3.0), v, x.tdomain());
    CHECK(v.size() == 3);

    if(v.size() == 3)
    {
      CHECK(v[0] == Interval(3.0,5.0));
      CHECK(v[1] == Interval(9.0,10.0));
      CHECK(v[2] == Interval(11.0,15.0));
    }
  }

  SECTION("Invert method with derivative")
  {
    Tube x(Interval(0., 5.), 1.0);
    Tube v(x);

    x.set(Interval(0.), 0.);
    x.set(Interval(4.), 5.);

    v.set(Interval(1.,2.), 0);
    v.set(Interval(0.5,1.5), 1);
    v.set(Interval(0.,0.5), 2);
    v.set(Interval(0.), 3);
    v.set(Interval(-0.5,0.5), 4);

    CtcDeriv ctc;
    ctc.contract(x, v);

    CHECK(x.invert(x.codomain(), x.tdomain()) == x.tdomain());
    CHECK(x.invert(Interval::ALL_REALS, x.tdomain()) == x.tdomain());

    // Using derivative
    CHECK(x.invert(x.codomain(), v, x.tdomain()) == x.tdomain());
    CHECK(x.invert(Interval::ALL_REALS, v, x.tdomain()) == x.tdomain());
    CHECK(x.invert(Interval(0.), v, x.tdomain()) == Interval(0.));
    CHECK(x.invert(Interval(4.25), v, x.tdomain()) == Interval(4.5));
    CHECK(x.invert(Interval(4.), v, x.tdomain()) == Interval(3.,5.));
    CHECK(ApproxIntv(x.invert(Interval(4.1), v, x.tdomain())) == Interval(4.2,4.8));
    CHECK(x.invert(Interval(3.5), v, x.tdomain()) == Interval(2.,4.));
    CHECK(x.invert(Interval::EMPTY_SET, v, x.tdomain()) == Interval::EMPTY_SET);
    CHECK(x.invert(Interval(10.), v, x.tdomain()) == Interval::EMPTY_SET);
    CHECK(x.invert(Interval(2.,3.), v, x.tdomain()) == Interval(1.,2.));
    CHECK(x.invert(Interval(1.), v, x.tdomain()) == Interval(0.5,0.75));
    CHECK(x.invert(Interval(3.5,4.), v, x.tdomain()) == Interval(2.,5.));
  }

  SECTION("Another test")
  {
    float dt = 0.05;
    // to test the interest of trees, use a very small dt
    Interval domain(-20., 20.);
    Tube x(domain, dt, TFunction("[-1,1]*(t^2+1)"));
    CHECK(x.invert(0., x.tdomain()) == domain);
  }
}

TEST_CASE("Testing set inversion in vector case")
{
  SECTION("test1")
  {
    TubeVector x(2, tube_test_1());
    x[1] = x[1] - 3.; // shift

    IntervalVector inv_val(2, 0.5);
    vector<Interval> v_t;
    x.invert(inv_val, v_t, x.tdomain());

    if(false & VIBES_DRAWING)
    {
      vibes::beginDrawing();
      VIBesFigTubeVector fig_x("x", &x);
      fig_x.set_properties(100, 100, 600, 250);
      fig_x.draw_box(x.tdomain(), inv_val);
      fig_x.show(true);

      for(size_t i = 0 ; i < v_t.size() ; i++)
        fig_x.draw_box(v_t[i], x.codomain(), "red[#FFD58C66]");

      vibes::endDrawing();
    }

    CHECK(v_t.size() == 5);
    CHECK(v_t[0] == Interval(3.,4.));
    CHECK(v_t[1] == Interval(15.,17.));
    CHECK(v_t[2] == Interval(37.,38.));
    CHECK(v_t[3] == Interval(40.,42.));
    CHECK(v_t[4] == Interval(43.,45.));

    // Union inversion:
    Interval inv = x.invert(inv_val, x.tdomain());
    CHECK(inv == Interval(3.,45.));

    // Restricted domain

    Interval restricted(15.2,39.);
    x.invert(inv_val, v_t, restricted);

    CHECK(v_t.size() == 2);
    CHECK(v_t[0] == Interval(15.2,17.));
    CHECK(v_t[2] == Interval(37.,38.));

    inv = x.invert(inv_val, restricted);
    CHECK(inv == Interval(15.2,38.));
  }
}

TEST_CASE("Testing thickness evaluation")
{
  SECTION("tube_test_1")
  {
    Tube x = tube_test_1();
    CHECK(x.max_diam() == 8.);
    CHECK(x.index(x.largest_slice()) == 3);
  }

  SECTION("tube_test2")
  {
    Tube x = tube_test2();
    int slice_id;
    CHECK(x.max_diam() == 4.);
    CHECK(x.index(x.largest_slice()) == 1);
  }
}

TEST_CASE("Testing equalities")
{
  SECTION("Slice")
  {
    Slice slice1(Interval(0.,1.), Interval(1.,3.));
    Slice slice2(Interval(0.,1.), Interval(1.,3.));
    CHECK(slice1 == slice2);
    CHECK_FALSE(slice1 != slice2);

    slice1.set(Interval(10.,10.5));
    CHECK(slice1 != slice2);
    CHECK_FALSE(slice1 == slice2);

    slice1 = slice2;
    CHECK(slice1 == slice2);
    CHECK_FALSE(slice1 != slice2);

    slice1 = slice2;
    slice1.set_input_gate(Interval(2.));
    CHECK(slice1 != slice2);
    CHECK_FALSE(slice1 == slice2);

    slice1 = slice2;
    slice1.set_output_gate(Interval(6.));
    CHECK(slice1 != slice2);
    CHECK_FALSE(slice1 == slice2);
  }

  SECTION("Tube")
  {
    Tube x1(Interval(0.,1.), 0.1, Interval(1.,3.));
    Tube x2(Interval(0.,1.), 0.1, Interval(1.,3.));

    CHECK(x1 == x2);
    CHECK_FALSE(x1 != x2);

    x1.set(Interval(2.,3.), 2);
    CHECK(x1 != x2);
    CHECK_FALSE(x1 == x2);

    x1 = x2;
    CHECK(x1 == x2);
    CHECK_FALSE(x1 != x2);

    x1.set(Interval(2.,3.), 0.5);
    x2.sample(0.5);
    CHECK(x1 != x2);
    CHECK_FALSE(x1 == x2);

    x1.set(Interval(1.,3.), 0.5);
    CHECK(x1 == x2);
    CHECK_FALSE(x1 != x2);

    x1.set(Interval(1.,2.2), 1.);
    CHECK(x1 != x2);
    CHECK_FALSE(x1 == x2);

    x1.set(Interval(1.,3.2), 1.); // gate changed to [1.,3.]
    CHECK(x1 == x2);
    CHECK_FALSE(x1 != x2);

    x1.set(Interval(1.,2.2), 1.);
    CHECK(x1 != x2);
    CHECK_FALSE(x1 == x2);

    Tube x_a1 = tube_test_1();
    Tube x_b1 = tube_test_1();
    CHECK(x_a1 == x_a1);
    CHECK(x_a1 == x_b1);
    CHECK_FALSE(x_a1 != x_a1);
    CHECK_FALSE(x_a1 != x_b1);

    Tube x_a2 = tube_test4();
    Tube x_b2 = tube_test4();
    CHECK(x_a2 == x_a2);
    CHECK(x_a2 == x_b2);
    CHECK_FALSE(x_a2 != x_a2);
    CHECK_FALSE(x_a2 != x_b2);
  }
}

TEST_CASE("Testing is_subset()")
{
  SECTION("Slice")
  {
    Slice slice1(Interval(0.,1.), Interval(1.,3.));
    Slice slice2(Interval(0.,1.), Interval(0.,4.));
    Slice slice3(Interval(0.,1.4), Interval(0.,4.));

    // todo: CHECK_THROWS(slice1.is_subset(slice3));
    CHECK(slice1.is_subset(slice2));

    slice1.set_input_gate(Interval(0.5,2.));
    CHECK(slice1.is_subset(slice2));
    CHECK(slice1(0.) == Interval(1.,2.));

    slice1.set_output_gate(Interval(0.5,2.));
    CHECK(slice1.is_subset(slice2));
    CHECK(slice1(1.) == Interval(1.,2.));
  }

  SECTION("Tube")
  {
    Tube x1(Interval(0.,10.), 1., Interval(0.,5.));
    Tube x2(Interval(0.,10.), 1., Interval(0.,5.));

    CHECK(Interval(0.,5.).is_subset(Interval(0.,5.)));
    CHECK(x1.is_subset(x2));
    CHECK(!x1.is_strict_subset(x2));
    x1.set(Interval(1.,4.));
    CHECK(x1.is_subset(x2));

    x1 = x2;
    CHECK(x1.is_subset(x2));
    CHECK(!x1.is_strict_subset(x2));
    x1.set(Interval(1.,5.), 2); // strict subset due to a slice only
    CHECK(x1.is_subset(x2));
    CHECK(x1 != x2);
    CHECK(x1.is_strict_subset(x2));

    x1 = x2;
    CHECK(x1.is_subset(x2));
    CHECK(!x1.is_strict_subset(x2));
    x1.set(Interval(1.,5.), 2.); // strict subset due to a gate only
    CHECK(x1.is_subset(x2));
    CHECK(x1.is_strict_subset(x2));
  }

  SECTION("TubeVector")
  {
    TubeVector x1(Interval(0.,10.), 1., IntervalVector(2, Interval(0.,5.)));
    TubeVector x2(Interval(0.,10.), 1., IntervalVector(2, Interval(0.,5.)));

    CHECK(x1.is_subset(x2));
    CHECK(!x1.is_strict_subset(x2));
    x1[0].set(Interval(1.,4.));
    CHECK(x1.is_subset(x2));

    x1 = x2;
    CHECK(x1.is_subset(x2));
    CHECK(!x1.is_strict_subset(x2));
    x1[1].set(Interval(1.,5.), 2); // strict subset due to a slice only
    CHECK(x1.is_subset(x2));
    CHECK(x1 != x2);
    CHECK(x1.is_strict_subset(x2));

    x1 = x2;
    CHECK(x1.is_subset(x2));
    CHECK(!x1.is_strict_subset(x2));
    x1[1].set(Interval(1.,5.), 2.); // strict subset due to a gate only
    CHECK(x1.is_subset(x2));
    CHECK(x1.is_strict_subset(x2));
  }
}

TEST_CASE("Testing encloses()")
{
  SECTION("Slice")
  {
    Slice slice1(Interval(0.,1.), Interval(1.,3.));
    // todo
  }

  SECTION("Tube")
  {
    Tube x1(Interval(0.,10.), 1., Interval(0.,5.));
    // todo
  }
}

TEST_CASE("Testing is_empty()")
{
  SECTION("Slice")
  {
    Slice slice1(Interval(0.,1.), Interval(1.,3.));
    CHECK(!slice1.is_empty());
    slice1.set_input_gate(Interval(5.));
    CHECK(slice1.is_empty());
    slice1.set_input_gate(Interval(2.));
    CHECK(!slice1.is_empty());
    slice1.set_output_gate(Interval(5.));
    CHECK(slice1.is_empty());
    slice1.set_output_gate(Interval(2.));
    CHECK(!slice1.is_empty());
    slice1.set(Interval::EMPTY_SET);
    CHECK(slice1.is_empty());
    slice1.set(Interval::ALL_REALS);
    CHECK(!slice1.is_empty());
  }

  SECTION("Tube")
  {
    Tube x1(Interval(0.,10.), Interval(0.,5.));
    CHECK(!x1.is_empty());
    x1.set(Interval(6.), 2.3);
    CHECK(x1.is_empty());
    x1.set(Interval(5.), 2.3);
    CHECK(!x1.is_empty());
    x1.set(Interval::EMPTY_SET, 0);
    CHECK(x1.is_empty());
    x1.set(Interval(4.,5.));
    CHECK(!x1.is_empty());
    x1.set(Interval::EMPTY_SET, 10.);
    CHECK(x1.is_empty());
  }

  SECTION("Tube test")
  {
    Tube x = tube_test_1();
    CHECK_FALSE(x.is_empty());
    x.set(Interval::EMPTY_SET);
    CHECK(x.is_empty());
    x = tube_test_1();
    x.set(Interval::EMPTY_SET, 10);
    CHECK(x.is_empty());
    x.set(Interval(5.), 10);
    CHECK(x.is_empty());
  }
}

TEST_CASE("Testing inflate()")
{
  SECTION("Slice")
  {
    Slice slice(Interval(0.,10.), Interval(0.));

    CHECK(slice.codomain() == Interval(0.));

    slice.inflate(0.2);

    CHECK(slice.codomain() == Interval(-0.2,0.2));

    slice.inflate(1.);

    CHECK(ApproxIntv(slice.codomain()) == Interval(-1.2,1.2));
    CHECK(ApproxIntv(slice.input_gate()) == Interval(-1.2,1.2));
    CHECK(ApproxIntv(slice.output_gate()) == Interval(-1.2,1.2));

    slice.set_input_gate(Interval(0.,0.5));
    slice.set_output_gate(Interval(0.,0.3));

    CHECK(slice(0.) == Interval(0.,0.5));
    CHECK(slice(10.) == Interval(0.,0.3));

    slice.inflate(1.);

    CHECK(ApproxIntv(slice.codomain()) == Interval(-2.2,2.2));
    CHECK(ApproxIntv(slice.input_gate()) == Interval(-1.,1.5));
    CHECK(ApproxIntv(slice.output_gate()) == Interval(-1.,1.3));
  }

  SECTION("Tube")
  {
    Tube x(Interval(0.,10.), 0.8, Interval(0.));
    CHECK(x.codomain() == Interval(0.));
    CHECK(x(3) == Interval(0.));
    CHECK(x.slice(3)->input_gate() == Interval(0.));
    CHECK(x.slice(6)->input_gate() == Interval(0.));
    x.inflate(0.2);
    CHECK(x.codomain() == Interval(-0.2,0.2));
    CHECK(x(6) == Interval(-0.2,0.2));
    CHECK(x.slice(6)->input_gate() == Interval(-0.2,0.2));
    x.inflate(1.);
    CHECK(ApproxIntv(x.codomain()) == Interval(-1.2,1.2));
    CHECK(ApproxIntv(x(9)) == Interval(-1.2,1.2));
    CHECK(ApproxIntv(x.slice(9)->input_gate()) == Interval(-1.2,1.2));
    double t = x.slice(9)->tdomain().lb();
    x.set(Interval(3.,7.), 8);
    x.set(Interval(3.,7.), 9);
    x.set(Interval(4.,6.), t);
    CHECK(ApproxIntv(x(t)) == Interval(4.,6.));
    x.inflate(2.);
    CHECK(ApproxIntv(x(8)) == Interval(1.,9.));
    CHECK(ApproxIntv(x(9)) == Interval(1.,9.));
    CHECK(ApproxIntv(x(t)) == Interval(2.,8.));
  }
}

TEST_CASE("Testing volume()")
{
  SECTION("Slice")
  {
    Slice empty_slice(Interval(0.,1.), Interval::EMPTY_SET);
    CHECK(empty_slice.codomain().is_empty());
    CHECK(empty_slice.volume() == 0.);

    Slice unbounded_slice(Interval(0.,1.));
    CHECK(unbounded_slice.codomain().is_unbounded());
    CHECK(unbounded_slice.volume() == POS_INFINITY);

    Slice flat_slice(Interval(0.,1.), Interval(3.));
    CHECK_FALSE(flat_slice.codomain().is_unbounded());
    CHECK(flat_slice.volume() == 0.);

    Slice bounded_slice(Interval(0.,2.), Interval(2.,12.));
    CHECK_FALSE(bounded_slice.codomain().is_unbounded());
    CHECK(bounded_slice.volume() == 20.);
  }

  SECTION("Tube")
  {
    Tube x1 = tube_test_1();
    x1.set(Interval(-4,2), 14);
    CHECK(x1.volume() == 197.);

    Tube x4 = tube_test4();
    CHECK(x4.volume() == 9.+2.+1.+2.+1.+(21.-14.)*1.);
    x4.set(Interval::NEG_REALS, Interval(3.,4.));
    CHECK(x4.volume() == POS_INFINITY);
    x4.set(Interval::EMPTY_SET, Interval(3.,3.2));
    CHECK(x4.volume() == POS_INFINITY);
    x4.set(Interval::EMPTY_SET, Interval(3.,4.));
    CHECK_FALSE(x4.volume() == POS_INFINITY);

    Tube empty_tube(Interval(0.,1.), 0.1, Interval::EMPTY_SET);
    CHECK(empty_tube.codomain().is_empty());
    CHECK(empty_tube.volume() == 0.);

    Tube unbounded_tube(Interval(0.,1.), 0.1);
    CHECK(unbounded_tube.codomain().is_unbounded());
    CHECK(unbounded_tube.volume() == POS_INFINITY);

    Tube flat_tube(Interval(0.,1.), 0.1, Interval(3.));
    CHECK_FALSE(flat_tube.codomain().is_unbounded());
    CHECK(flat_tube.volume() == 0.);

    Tube bounded_tube(Interval(0.,2.), 0.1, Interval(2.,12.));
    CHECK_FALSE(bounded_tube.codomain().is_unbounded());
    CHECK(Approx(bounded_tube.volume()) == 20.);
  }
}

TEST_CASE("Interpol")
{
  SECTION("Test slice, envelope contraction")
  {
    Slice x(Interval(-1.,3.), Interval(-10.,20.));
    x.set_input_gate(Interval(-1.,2.));
    x.set_output_gate(Interval(-2.,0.));
    Slice v(x.tdomain(), Interval(-1.,1.));
    
    bool contraction;
    Interval t;
    CtcDeriv ctc;
    ctc.contract(x, v); // reaching a consistency state

    t = Interval(0.5,2.);
    CHECK(x.interpol(t, v) == Interval(-3.5,2.5));

    t = Interval(-1.);
    CHECK(x.interpol(t, v) == Interval(-1.,2.));

    t = Interval(-0.5,0.);
    CHECK(x.interpol(t, v) == Interval(-2.,3.));

    t = Interval(1.5);
    CHECK(x.interpol(t, v) == Interval(-3.5,1.5));

    t = Interval(2.5);
    CHECK(x.interpol(t, v) == Interval(-2.5,0.5));

    t = Interval(3.);
    CHECK(x.interpol(t, v) == Interval(-2.,0.));

    t = Interval(-1.,3.);
    CHECK(x.interpol(t, v) == Interval(-3.5,3.));

    t = Interval(-10.,3.) & x.tdomain();
    CHECK(x.interpol(t, v) == Interval(-3.5,3.));
  }

  SECTION("Interpolation over a tube (example from tubint paper)")
  {
    Tube x(Interval(0.,5.), 1.);
    Tube v(x);

    x.set(Interval(0.), 0.);
    x.set(Interval(4.), 5.);

    v.set(Interval(1.,2.), 0);
    v.set(Interval(0.5,1.5), 1);
    v.set(Interval(0.,0.5), 2);
    v.set(Interval(0.), 3);
    v.set(Interval(-0.5,0.5), 4);

    Interval t ,y;
    bool contraction;
    CtcEval ctc_eval;
    ctc_eval.preserve_slicing(false);

    // Tube evaluation before x contraction
    y = Interval::ALL_REALS;
    t = Interval::ALL_REALS;
    //contraction = ctc_eval.contract(t, y, x, v);
    ////CHECK(contraction);
    //CHECK(x.codomain() == Interval::ALL_REALS);
    //CHECK(y == Interval::ALL_REALS);
    //CHECK(t == Interval(0.,5.));

    CtcDeriv ctc_deriv;
    ctc_deriv.contract(x, v); // contraction of the x
    //CHECK_FALSE(ctc_eval.contract(x, v)); // fixed point already reached
    CHECK(x.codomain() == Interval(0.,4.25));
    CHECK(x(0) == Interval(0.,2.));
    CHECK(x(1) == Interval(1.5,3.5));
    CHECK(x(2) == Interval(3.,4.));
    CHECK(x(3) == Interval(3.5,4.));
    CHECK(x(4) == Interval(3.5,4.25));

    if(VIBES_DRAWING) // drawing results
    {
      //vibes::beginDrawing();
      //VIBesFigTube fig_x("tubint", &x);
      //fig_x.set_properties(100, 100, 500, 500);
      //fig_x.set_tube_derivative(&x, &v);
      //fig_x.show(true);
      //vibes::endDrawing();
    }

    // Tube evaluation

    IntervalVector box(2);
    Tube x_raw = x, v_raw = v;

    CHECK(x.interpol(Interval(0.,5.), v) == Interval(0.,4.25));
    CHECK(x.interpol(x.tdomain(), v) == Interval(0.,4.25));
    CHECK(x.interpol(Interval(1.,3.), v) == Interval(1.5,4.));
    CHECK(x.interpol(Interval(2.2,2.7), v) == Interval(3.1,3.85));
    CHECK(x.interpol(Interval(4.5), v) == Interval(3.75,4.25));
    CHECK(x.interpol(Interval(4.1,4.9), v) == Interval(3.55,4.25));

    if(VIBES_DRAWING) vibes::beginDrawing();

    x = x_raw;
    v = v_raw;
    box[0] = Interval(0.25,0.75);
    box[1] = Interval(0.5,1.5);
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "tubint", "blue"));
    ctc_eval.contract(box[0], box[1], x, v);
    CHECK(box[0] == Interval(0.25,0.75));
    CHECK(box[1] == Interval(0.5,1.5));
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "tubint", "red"));

    x = x_raw;
    v = v_raw;
    box[0] = Interval(-1.,6.);
    box[1] = Interval(-0.5,4.5);
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "tubint", "blue"));
    ctc_eval.contract(box[0], box[1], x, v);
    CHECK(box[0] == Interval(0.,5.));
    CHECK(box[1] == Interval(0.,4.25));
    CHECK(x == x_raw); // the tube should not have been contracted
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "tubint", "red"));

    x = x_raw;
    v = v_raw;
    box[0] = Interval(0.4,0.8);
    box[1] = Interval(0.3,0.6);
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "tubint", "blue"));
    ctc_eval.contract(box[0], box[1], x, v);
    CHECK(box[0] == Interval(0.4,0.55));
    CHECK(box[1] == Interval(0.4,0.6));
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "tubint", "red"));

    x = x_raw;
    v = v_raw;
    box[0] = Interval(1.2,1.5);
    box[1] = Interval(1.,4.);
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "tubint", "blue"));
    ctc_eval.contract(box[0], box[1], x, v);
    CHECK(box[0] == Interval(1.2,1.5));
    CHECK(ApproxIntv(box[1]) == Interval(1.8,2.75));
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "tubint", "red"));

    x = x_raw;
    v = v_raw;
    box[0] = Interval(1.8,3.1);
    box[1] = Interval(2.5,4.2);
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "tubint", "blue"));
    ctc_eval.contract(box[0], box[1], x, v);
    CHECK(box[0] == Interval(1.8,3.1));
    CHECK(ApproxIntv(box[1]) == Interval(2.7,4.));
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "tubint", "red"));

    x = x_raw;
    v = v_raw;
    box[0] = Interval(0.2);
    box[1] = Interval(0.1,0.8);
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "tubint", "blue"));
    ctc_eval.contract(box[0], box[1], x, v);
    CHECK(box[0] == Interval(0.2));
    CHECK(ApproxIntv(box[1]) == Interval(0.2,0.4));
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "tubint", "red"));

    x = x_raw;
    v = v_raw;
    box[0] = Interval(0.8);
    box[1] = Interval(0.8,1.8);
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "tubint", "blue"));
    ctc_eval.contract(box[0], box[1], x, v);
    CHECK(box[0] == Interval(0.8));
    CHECK(box[1] == Interval(1.1,1.6));
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "tubint", "red"));

    x = x_raw;
    v = v_raw;
    box[0] = Interval(0.5,2.5);
    box[1] = Interval(2.2);
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "tubint", "blue"));
    ctc_eval.contract(box[0], box[1], x, v);
    CHECK(ApproxIntv(box[0]) == Interval(1.1+1./30.,1.4+2./30.));
    CHECK(box[1] == Interval(2.2));
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "tubint", "red"));

    x = x_raw;
    v = v_raw;
    box[0] = Interval(4.1,4.9);
    box[1] = Interval(4.1,4.5);
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "tubint", "blue"));
    ctc_eval.contract(box[0], box[1], x, v);
    CHECK(ApproxIntv(box[0]) == Interval(4.2,4.8));
    CHECK(box[1] == Interval(4.1,4.25));
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "tubint", "red"));

    x = x_raw;
    v = v_raw;
    box[0] = Interval(4.2,4.7);
    box[1] = Interval(3.55,3.8);
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "tubint", "blue"));
    ctc_eval.contract(box[0], box[1], x, v);
    CHECK(ApproxIntv(box[0]) == Interval(4.2,4.6));
    CHECK(box[1] == Interval(3.6,3.8));
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "tubint", "red"));

    if(VIBES_DRAWING) vibes::endDrawing();
  }

  SECTION("Interpolation, isolated example, epsilon inflation required")
  {
    Tube x(Interval(8., 12.));
    Tube v(x);
    v.set(Interval(-1./3.,1.));
    x.set(Interval(1.), 8.);
    x.set(Interval(1.), 12.);

    CtcDeriv ctc_deriv;
    ctc_deriv.contract(x, v); // contraction of the x

    CHECK(x.nb_slices() == 1);

    #if VIBES_DRAWING // drawing results
      vibes::beginDrawing();
      VIBesFigTube fig_x("test", &x);
      fig_x.set_properties(100, 100, 1000, 500);
      fig_x.set_tube_derivative(&x, &v);
      fig_x.show(true);
    #endif

    CtcEval ctc_eval;
    ctc_eval.preserve_slicing(false);
    IntervalVector box(2);
    box[0] = Interval(9.,11.8);
    box[1] = Interval(-1.,0.5);

    ctc_eval.contract(box[0], box[1], x, v);
    CHECK(x.nb_slices() == 3);
    CHECK(ApproxIntv(box[0]) == Interval(9.5,11.5));
    CHECK(ApproxIntv(box[1]) == Interval(0.,0.5));

    #if VIBES_DRAWING // drawing results
      fig_x.show(true);
      vibes::drawBox(box, vibesParams("figure", "test", "red"));
      vibes::end_drawing();
    #endif
  }

  SECTION("Interpolation over a x (other example)")
  {
    Tube x(Interval(0., 26.), Interval(-1.,7.));

    x.set(Interval(2.,3.), 0.);
    x.set(Interval(3.,4.), 4.);
    x.set(Interval(1.), 8.);
    x.set(Interval(1.), 12.);
    x.set(Interval(5.5), 14.);
    x.set(Interval(5.5), 16.);
    x.set(Interval(-1.), 18.);
    x.set(Interval(5.5), 20.);
    x.set(Interval(5.5), 23.);
    x.set(Interval::ALL_REALS, 8);
    x.set(Interval(5.5), 23.); // setting gate again

    Tube v(x);
    v.set(Interval(-1.,1.));
    v.set(Interval(-1.5,4.), 0);
    v.set(Interval(-0.75,-0.5), 1);
    v.set(Interval(-1./3.,1.), 2);
    v.set(Interval(4.5/2.), 3);
    v.set(Interval(0.), 4);
    v.set(Interval(NEG_INFINITY,-6.5/2.), 5);
    v.set(Interval(6.5/2.,POS_INFINITY), 6);
    v.set(Interval::ALL_REALS, 7);
    v.set(Interval::ALL_REALS, 8);

    CtcEval ctc_eval;
    ctc_eval.preserve_slicing(false);
    CtcDeriv ctc_deriv;
    ctc_deriv.contract(x, v);

    #if VIBES_DRAWING // drawing results
      vibes::beginDrawing();
      x &= Interval(-1.,7.);
      VIBesFigTube fig_x("test", &x);
      fig_x.set_properties(100, 100, 1000, 500);
      fig_x.set_tube_derivative(&x, &v);
      fig_x.show(true);
    #endif

    IntervalVector box(2);
    Tube x_raw = x, v_raw = v;

    x = x_raw;
    v = v_raw;
    box[0] = Interval(8.1,11.8);
    box[1] = Interval(1.9,3.);
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "test", "blue"));
    ctc_eval.contract(box[0], box[1], x, v);
    CHECK(ApproxIntv(box[0]) == Interval(8.9,9.3));
    CHECK(ApproxIntv(box[1]) == Interval(1.9,2.));
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "test", "red"));

    x = x_raw;
    v = v_raw;
    box[0] = Interval(9.,11.8);
    box[1] = Interval(0.2,0.5);
    CHECK(ApproxIntv(x.invert(box[1], v, box[0])) == Interval(9.5,11.5));
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "test", "blue"));
    ctc_eval.contract(box[0], box[1], x, v);
    CHECK(x.nb_slices() == 11);
    CHECK(ApproxIntv(box[0]) == Interval(9.5,11.5));
    CHECK(ApproxIntv(box[1]) == Interval(0.2,0.5));
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "test", "red"));

    x = x_raw;
    v = v_raw;
    box[0] = Interval(10.,11.3);
    box[1] = Interval(-0.2,0.25);
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "test", "blue"));
    ctc_eval.contract(box[0], box[1], x, v);
    CHECK(ApproxIntv(box[0]) == Interval(10.25,11.25));
    CHECK(ApproxIntv(box[1]) == Interval(0.,0.25));
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "test", "red"));

    x = x_raw;
    v = v_raw;
    box[0] = Interval(11.8,12.4);
    box[1] = Interval(0.6,1.3);
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "test", "blue"));
    ctc_eval.contract(box[0], box[1], x, v);
    CHECK(ApproxIntv(box[0]) == Interval(11.8,12.1+1./30.));
    CHECK(ApproxIntv(box[1]) == Interval(0.8,1.3));
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "test", "red"));

    x = x_raw;
    v = v_raw;
    box[0] = Interval(12.6,13.);
    box[1] = Interval(1.5,4.5);
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "test", "blue"));
    ctc_eval.contract(box[0], box[1], x, v);
    CHECK(ApproxIntv(box[0]) == Interval(12.6,13.));
    CHECK(ApproxIntv(box[1]) == Interval(2.35,3.25));
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "test", "red"));

    x = x_raw;
    v = v_raw;
    box[0] = Interval(14.2,15.);
    box[1] = Interval(5.,6.);
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "test", "blue"));
    ctc_eval.contract(box[0], box[1], x, v);
    CHECK(ApproxIntv(box[0]) == Interval(14.2,15.));
    CHECK(ApproxIntv(box[1]) == Interval(5.5));
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "test", "red"));

    x = x_raw;
    v = v_raw;
    box[0] = Interval(13.5,15.5);
    box[1] = Interval(3.2,6.5);
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "test", "blue"));
    ctc_eval.contract(box[0], box[1], x, v);
    CHECK(ApproxIntv(box[0]) == Interval(13.5,15.5));
    CHECK(ApproxIntv(box[1]) == Interval(4.375,5.5));
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "test", "red"));

    x = x_raw;
    v = v_raw;
    box[0] = Interval(7.8,9.);
    box[1] = Interval(1.05,1.3);
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "test", "blue"));
    ctc_eval.contract(box[0], box[1], x, v);
    CHECK(ApproxIntv(box[0]) == Interval(7.8,9.));
    CHECK(ApproxIntv(box[1]) == Interval(1.05,1.3));
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "test", "red"));

    x = x_raw;
    v = v_raw;
    box[0] = Interval(16.2,19.5);
    box[1] = Interval(0.,3.);
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "test", "blue"));
    ctc_eval.contract(box[0], box[1], x, v);
    CHECK(ApproxIntv(box[0]) == Interval(16.76923076923076649791255476884543895721,19.23076923076923350208744523115456104279));
    CHECK(ApproxIntv(box[1]) == Interval(0.,3.));
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "test", "red"));

    x = x_raw;
    v = v_raw;
    box[0] = Interval(0.2,3.);
    box[1] = Interval(6.,8.);
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "test", "blue"));
    ctc_eval.contract(box[0], box[1], x, v);
    CHECK(ApproxIntv(box[0]) == Interval(0.75,2.+2./3.));
    CHECK(ApproxIntv(box[1]) == Interval(6.,7.));
    if(VIBES_DRAWING) vibes::drawBox(box, vibesParams("figure", "test", "red"));

    if(VIBES_DRAWING) vibes::endDrawing();
  }

  SECTION("Shifting domains")
  {
    Tube x(Interval(0., 1.), 0.1, TFunction("cos(t)"));
    CHECK(x.tdomain() == Interval(0.,1.));
    CHECK(x(0.5).contains(cos(0.5)));
    x.shift_tdomain(-10.);
    CHECK(x.tdomain() == Interval(0.,1.) - 10.);
    CHECK(x(0.5-10.).contains(cos(0.5)));
  }

  SECTION("Constructor with list")
  {
    // Defined by maps of values

    TubeVector test1(4, Tube(Interval(0., 1.), 0.1, TFunction("cos(t)")));
    TubeVector test2({
        Tube(Interval(0., 1.), 0.1, TFunction("cos(t)")),
        Tube(Interval(0., 1.), 0.1, TFunction("cos(t)")),
        Tube(Interval(0., 1.), 0.1, TFunction("cos(t)")),
        Tube(Interval(0., 1.), 0.1, TFunction("cos(t)"))
    });

    CHECK(test1 == test2);
    CHECK(test1[0] == test2[0]);
  }

  SECTION("is_empty")
  {
    Tube x1(Interval(0.,10.), 0.001);
    CHECK(!x1.is_empty());

    TubeVector x2(Interval(0.,10.), 0.001, 4);
    CHECK(!x2.is_empty());
  }
}