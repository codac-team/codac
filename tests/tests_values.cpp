#include "tests.h"

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace tubex;

TEST_CASE("Tube values")
{
  SECTION("Test tube1")
  {
    Tube tube = tubeTest1();
    tube.set(Interval(-4,2), 14); // to test primitives pre-computation

    CHECK(tube.nbSlices() == 46);

    CHECK(tube[ 0] == Interval(4,8));
    CHECK(tube[ 1] == Interval(2,7));
    CHECK(tube[ 2] == Interval(1,6));
    CHECK(tube[ 3] == Interval(-4,4));
    CHECK(tube[ 4] == Interval(-7,-1));
    CHECK(tube[ 5] == Interval(-9,-5));
    CHECK(tube[ 6] == Interval(-10,-6));
    CHECK(tube[ 7] == Interval(-11,-7));
    CHECK(tube[ 8] == Interval(-10,-6));
    CHECK(tube[ 9] == Interval(-9,-4));
    CHECK(tube[10] == Interval(-8,-5));
    CHECK(tube[11] == Interval(-7,-4));
    CHECK(tube[12] == Interval(-6,-2));
    CHECK(tube[13] == Interval(-5,-1));
    CHECK(tube[14] == Interval(-4,2));
    CHECK(tube[15] == Interval(-2,4));
    CHECK(tube[16] == Interval(0,6));
    CHECK(tube[17] == Interval(2,7));
    CHECK(tube[18] == Interval(4,8));
    CHECK(tube[19] == Interval(6,9));
    CHECK(tube[20] == Interval(7,10));
    CHECK(tube[21] == Interval(8,11));
    CHECK(tube[22] == Interval(9,12));
    CHECK(tube[23] == Interval(8,13));
    CHECK(tube[24] == Interval(7,12));
    CHECK(tube[25] == Interval(5,11));
    CHECK(tube[26] == Interval(3,10));
    CHECK(tube[27] == Interval(4,9));
    CHECK(tube[28] == Interval(5,8));
    CHECK(tube[29] == Interval(4,7));
    CHECK(tube[30] == Interval(3,6));
    CHECK(tube[31] == Interval(3,5));
    CHECK(tube[32] == Interval(2,5));
    CHECK(tube[33] == Interval(2,5));
    CHECK(tube[34] == Interval(1,5));
    CHECK(tube[35] == Interval(2,4));
    CHECK(tube[36] == Interval(1,4));
    CHECK(tube[37] == Interval(0,4));
    CHECK(tube[38] == Interval(-1,3));
    CHECK(tube[39] == Interval(-1,3));
    CHECK(tube[40] == Interval(-1,4));
    CHECK(tube[41] == Interval(0,5));
    CHECK(tube[42] == Interval(1,6));
    CHECK(tube[43] == Interval(0,5));
    CHECK(tube[44] == Interval(-1,4));
    CHECK(tube[45] == Interval(-1,3));

    CHECK(tube[7] == Interval(-11,-7));
    CHECK(tube[3.5] == Interval(-4,4));
    CHECK(tube[17.5] == Interval(2,7));
    CHECK(tube[32.5] == Interval(2,5));
    CHECK(tube[17.0] == Interval(2,6)); // t belongs to two slices
    CHECK(tube[16.9999999] == Interval(0,6));
    CHECK(tube[17.0000001] == Interval(2,7));
    CHECK(tube[Interval(9.5,17.5)] == Interval(-9,7));
    CHECK(tube[Interval(16.1,37.9)] == Interval(0,13));
    CHECK(tube[Interval(37.4,45.6)] == Interval(-1,6));
    CHECK(tube[Interval(14.0,27.0)] == Interval(-4,13));
    CHECK(tube[Interval(19.2,35.7)] == Interval(1,13));
    CHECK(tube[Interval(15.0,16.0)] == Interval(-2,4));
    CHECK(tube[Interval(15.2,15.3)] == Interval(-2,4));
    CHECK(tube[15.2] == Interval(-2,4));
    CHECK(tube[Interval(0.)] == Interval(4,8));
    CHECK(tube[Interval(5.)] == Interval(-7,-5));
    CHECK(tube[Interval(24.)] == Interval(8,12));
    CHECK(tube[Interval(46.)] == Interval(-1,3));
    CHECK(tube.codomain() == Interval(-11,13));
  }

  SECTION("Setting values over an interval domain")
  {
    Tube tube(Interval(0.,10.));

    // Unbounded interval domain
    tube.set(Interval(6.,10.));
    CHECK(tube.nbSlices() == 1);
    CHECK(tube.getSlice(0)->domain() == Interval(0.,10.));
    CHECK(tube.getSlice(0)->codomain()[0] == Interval(6.,10.));

    // Bounded interval domain
    tube.set(Interval(2.,4.), Interval(2.,3.));
    CHECK(tube.nbSlices() == 3);
    CHECK(tube.getSlice(0)->domain() == Interval(0.,2.));
    CHECK(tube[0] == Interval(6.,10.));
    CHECK(tube.getSlice(1)->domain() == Interval(2.,3.));
    CHECK(tube[1] == Interval(2.,4.));
    CHECK(tube.getSlice(2)->domain() == Interval(3.,10.));
    CHECK(tube[2] == Interval(6.,10.));

    // Gates, slices intersection
    tube.set(Interval(1.,3.), Interval(0.,2.));
    CHECK(tube.nbSlices() == 3);
    CHECK(tube[0] == Interval(1.,3.));
    CHECK(tube[0.] == Interval(1.,3.));
    CHECK(tube[2.] == Interval(2.,3.));
    CHECK(tube[2.01] == Interval(2.,4.));
    CHECK(tube[3.] == Interval::EMPTY_SET);
    tube.set(Interval(4.,5.), Interval(3.,4.));
    CHECK(tube[3.] == Interval(4.));
  }

  SECTION("Setting values over an interval domain (bis)")
  {
    Tube tube(Interval(0.,8.));
    tube.set(Interval(-3.,1.), 0.);
    tube.set(Interval(0.,1.), 2.);
    tube.set(Interval(-1.,1.), 4.);
    tube.set(Interval(-1.,0.), 6.);
    tube.set(Interval(-2.,1.), 8.);

    CHECK(tube.nbSlices() == 4);

    tube.set(Interval(-3.,2.), Interval(0.,2.));
    tube.set(Interval(-1.,3.), Interval(2.,4.));
    tube.set(Interval(-3.,2.), Interval(4.,6.));
    tube.set(Interval(-2.,1.), Interval(6.,8.));

    CHECK(tube[0] == Interval(-3.,2.));
    CHECK(tube[1] == Interval(-1.,3.));
    CHECK(tube[2] == Interval(-3.,2.));
    CHECK(tube[3] == Interval(-2.,1.));
    CHECK(tube.codomain() == Interval(-3.,3.));

    // The gates have been changed too
    CHECK(tube[0.] == Interval(-3.,2.));
    CHECK(tube[2.] == Interval(-1.,2.));
    CHECK(tube[4.] == Interval(-1.,2.));
    CHECK(tube[6.] == Interval(-2.,1.));
    CHECK(tube[8.] == Interval(-2.,1.));

    // Setting again the gates
    tube.set(Interval(-3.,1.), 0.);
    tube.set(Interval(0.,1.), 2.);
    tube.set(Interval(-1.,1.), 4.);
    tube.set(Interval(-1.,0.), 6.);
    tube.set(Interval(-2.,1.), 8.);

    // Checking new values
    CHECK(tube[0.] == Interval(-3.,1.));
    CHECK(tube[2.] == Interval(0.,1.));
    CHECK(tube[4.] == Interval(-1.,1.));
    CHECK(tube[6.] == Interval(-1.,0.));
    CHECK(tube[8.] == Interval(-2.,1.));

    // New setting over all slices
    tube.set(Interval(-1.5,1.5), Interval(1.,7.));

    // Checking gates...
    CHECK(tube[0.] == Interval(-3.,1.)); // not changed
    CHECK(tube[1.] == Interval(-1.5,1.5)); // new gate
    CHECK(tube[2.] == Interval(-1.5,1.5));
    CHECK(tube[4.] == Interval(-1.5,1.5));
    CHECK(tube[6.] == Interval(-1.5,1.5));
    CHECK(tube[7.] == Interval(-1.5,1.)); // new gate
    CHECK(tube[8.] == Interval(-2.,1.)); // not changed

    // ...and values
    CHECK(tube[0] == Interval(-3.,2.)); // not changed
    CHECK(tube[1] == Interval(-1.5,1.5));
    CHECK(tube[2] == Interval(-1.5,1.5));
    CHECK(tube[3] == Interval(-1.5,1.5));
    CHECK(tube[4] == Interval(-1.5,1.5));
    CHECK(tube[5] == Interval(-2.,1.)); // not changed
  }

  SECTION("Setting values at some precise time input")
  {
    Tube tube(Interval(0.,10.));
    tube.set(Interval(3.,4.), 2.);
    CHECK(tube.nbSlices() == 2);
    CHECK(tube.getSlice(0)->domain() == Interval(0.,2.));
    CHECK(tube.getSlice(1)->domain() == Interval(2.,10.));
    CHECK(tube[0] == Interval::ALL_REALS);
    CHECK(tube[1] == Interval::ALL_REALS);
    CHECK(tube[2.] == Interval(3.,4.));
  }

  SECTION("Test tube4")
  {
    Tube tube = tubeTest4();
    CHECK(tube.codomain() == Interval(-1.5,2));
    CHECK(tube[0] == Interval(1.,2.));
    CHECK(tube[10.2] == Interval(0.5,1.0));
    CHECK(tube[11.0] == Interval::EMPTY_SET);
    CHECK(tube[1.5] == Interval(1.,2.));
  }
}

TEST_CASE("Testing enclosed bounds (tube evaluations)")
{
  SECTION("Test tube1")
  {
    Tube tube1 = tubeTest1();
    tube1.set(Interval(-4,2), 14); // to test primitives pre-computation

    CHECK(ApproxIntvPair(tube1.eval())  == make_pair(Interval(-11,9), Interval(-7,13)));
    CHECK(tube1.eval(Interval(0.5,25.5)) == make_pair(Interval(-11,9), Interval(-7,13)));
    CHECK(tube1.eval(Interval(7.1,19.8)) == make_pair(Interval(-11,6), Interval(-7,9)));
    CHECK(tube1.eval(Interval(6.0,9.0)) == make_pair(Interval(-11,-9), Interval(-7,-6)));
    CHECK(tube1.eval(Interval(0.)) == make_pair(Interval(4), Interval(8)));
    CHECK(tube1.eval(Interval(5.)) == make_pair(Interval(-7), Interval(-5)));
    CHECK(tube1.eval(Interval(5.2)) == make_pair(Interval(-9), Interval(-5)));
    CHECK(tube1.eval(Interval(23.2,23.8)) == make_pair(Interval(8), Interval(13)));
    CHECK(tube1.eval(Interval(23.,24.)) == make_pair(Interval(8.,9.), Interval(12.,13.)));
    CHECK(tube1.eval(Interval(22.,25.)) == make_pair(Interval(7,9), Interval(11,13)));
    CHECK(tube1.eval(Interval(21.,23.)) == make_pair(Interval(8,9), Interval(10,12)));
  }

  SECTION("Test tube4")
  {
    Tube tube4 = tubeTest4();
    tube4.set(Interval(-1,1), Interval(10,11));
    Tube tube4_primitive = tube4.primitive();
    //CHECK(tube4_primitive.eval(Interval(12.5,14.5)) == make_pair(Interval(6,6.5), Interval(21,24.5)));/*
  }
}

TEST_CASE("Testing set inversion")
{
  SECTION("Scalar set inversion (TubeSlice)")
  {
    TubeSlice slice(Interval(0.,1.), IntervalVector(1, Interval(0.,10.)));
    slice.setInputGate(IntervalVector(1, Interval(2.,3.)));
    slice.setOutputGate(IntervalVector(1, Interval(5.,6.)));
    CHECK(slice.invert(IntervalVector(1, Interval(4.,6.)), Interval(0.)) == Interval::EMPTY_SET);
    CHECK(slice.invert(IntervalVector(1, Interval(2.5,6.)), Interval(0.)) == 0.);
    CHECK(slice.invert(IntervalVector(1, Interval(0.,1.)), Interval(1.)) == Interval::EMPTY_SET);
    CHECK(slice.invert(IntervalVector(1, Interval(2.5,6.)), Interval(1.)) == 1.);
    CHECK(slice.invert(IntervalVector(1, Interval(2.5,6.)), Interval(0.2,0.5)) == Interval(0.2,0.5));
    CHECK(slice.invert(IntervalVector(1, Interval(2.5,6.)), Interval(0.2)) == Interval(0.2));
  }

  SECTION("Scalar set inversion (Tube)")
  {
    Tube tube = tubeTest1();
    tube.set(Interval(-4,2), 14); // to test primitives pre-computation
    CHECK(tube.invert(Interval(0.)) == Interval(3.0,46.0));
    CHECK(tube.invert(Interval(-7.)) == Interval(4.0,12.0));
    CHECK(tube.invert(Interval::ALL_REALS) == Interval(0.0,46.0));
    CHECK(tube.invert(Interval(-12.0,14.0)) == Interval(0.0,46.0));
    CHECK(tube.invert(Interval(-20,-18)) == Interval::EMPTY_SET);
    CHECK(tube.invert(Interval(-1.0,1.0)) == Interval(2.0,46.0));
    CHECK(tube.invert(Interval(-10.5)) == Interval(7.0,8.0));
    CHECK(tube.invert(Interval(-12.0,-7.0)) == Interval(4.0,12.0));
    CHECK(tube.invert(Interval(10.0,11.0)) == Interval(20.0,27.0));
    CHECK(tube.invert(Interval(6.01,7.0)) == Interval(0.0,30.0));
    CHECK(tube.invert(Interval(6.0,7.0)) == Interval(0.0,43.0));
    CHECK(tube.invert(Interval(5.9,7.0)) == Interval(0.0,43.0));
  }
  
  SECTION("Vector set inversion")
  {
    Tube tube = tubeTest1();
    tube.set(Interval(-4,2), 14); // to test primitives pre-computation

    vector<Interval> v;

    tube.invert(Interval(0.), v);
    CHECK(v.size() == 4);

    if(v.size() == 4)
    {
      CHECK(v[0] == Interval(3.0,4.0));
      CHECK(v[1] == Interval(14.0,17.0));
      CHECK(v[2] == Interval(37.0,42.0));
      CHECK(v[3] == Interval(43.0,46.0));
    }

    // The same, with a custom domain:
    tube.invert(Interval(0.), v, Interval(3.8,42.5));
    CHECK(v.size() == 3);

    if(v.size() == 3)
    {
      CHECK(v[0] == Interval(3.8,4.0));
      CHECK(v[1] == Interval(14.0,17.0));
      CHECK(v[2] == Interval(37.0,42.0));
    }

    tube.invert(Interval(-1.0,1.0), v, Interval::ALL_REALS);
    CHECK(v.size() == 4);

    if(v.size() == 4)
    {
      CHECK(v[0] == Interval(2.0,5.0));
      CHECK(v[1] == Interval(13.0,17.0));
      CHECK(v[2] == Interval(34.0,35.0));
      CHECK(v[3] == Interval(36.0,46.0));
    }

    // The same, with a custom domain (empty):
    tube.invert(Interval(-1.0,1.0), v, Interval::EMPTY_SET);
    CHECK(v.size() == 0);

    tube.invert(Interval(-6.9999), v);
    CHECK(v.size() == 2);

    if(v.size() == 2)
    {
      CHECK(v[0] == Interval(4.,7.));
      CHECK(v[1] == Interval(8.,12.));
    }

    tube.invert(Interval::ALL_REALS, v);
    CHECK(v.size() == 1);

    if(v.size() == 1)
      CHECK(v[0] == Interval(0.0,46.0));

    tube.invert(Interval(-30.0,-29.0), v);
    CHECK(v.size() == 0);

    tube.invert(Interval(3.5), v);
    CHECK(v.size() == 5);

    if(v.size() == 5)
    {
      CHECK(v[0] == Interval(1.0,4.0));
      CHECK(v[1] == Interval(15.0,18.0));
      CHECK(v[2] == Interval(26.0,27.0));
      CHECK(v[3] == Interval(30.0,38.0));
      CHECK(v[4] == Interval(40.0,45.0));
    }

    tube.invert(Interval(9.5,30.0), v);
    CHECK(v.size() == 1);

    if(v.size() == 1)
      CHECK(v[0] == Interval(20.0,27.0));

    tube.invert(Interval(12.0,13.0), v);
    CHECK(v.size() == 1);

    if(v.size() == 1)
      CHECK(v[0] == Interval(22.0,25.0));

    tube.invert(Interval(-4.0,-3.0), v);
    CHECK(v.size() == 3);

    if(v.size() == 3)
    {
      CHECK(v[0] == Interval(3.0,5.0));
      CHECK(v[1] == Interval(9.0,10.0));
      CHECK(v[2] == Interval(11.0,15.0));
    }
  }
}

TEST_CASE("Testing thickness evaluation")
{
  SECTION("tubeTest1")
  {
    Tube tube = tubeTest1();
    int slice_id;
    CHECK(tube.maxThickness(slice_id) == 8.);
    CHECK(slice_id == 3);
  }

  SECTION("tubeTest2")
  {
    Tube tube = tubeTest2();
    int slice_id;
    CHECK(tube.maxThickness(slice_id) == 4.);
    CHECK(slice_id == 1);
  }
}

TEST_CASE("Testing equalities")
{
  SECTION("TubeSlice")
  {
    TubeSlice slice1(Interval(0.,1.), IntervalVector(1, Interval(1.,3.)));
    TubeSlice slice2(Interval(0.,1.), IntervalVector(1, Interval(1.,3.)));
    CHECK(slice1 == slice2);
    CHECK_FALSE(slice1 != slice2);

    slice1.set(IntervalVector(1, Interval(10.,10.5)));
    CHECK(slice1 != slice2);
    CHECK_FALSE(slice1 == slice2);

    slice1 = slice2;
    CHECK(slice1 == slice2);
    CHECK_FALSE(slice1 != slice2);

    slice1 = slice2;
    slice1.setInputGate(IntervalVector(1, Interval(2.)));
    CHECK(slice1 != slice2);
    CHECK_FALSE(slice1 == slice2);

    slice1 = slice2;
    slice1.setOutputGate(IntervalVector(1, Interval(6.)));
    CHECK(slice1 != slice2);
    CHECK_FALSE(slice1 == slice2);
  }

  SECTION("Tube")
  {
    Tube tube1(Interval(0.,1.), 0.1, Interval(1.,3.));
    Tube tube2(Interval(0.,1.), 0.1, Interval(1.,3.));

    CHECK(tube1 == tube2);
    CHECK_FALSE(tube1 != tube2);

    tube1.set(Interval(2.,3.), 2);
    CHECK(tube1 != tube2);
    CHECK_FALSE(tube1 == tube2);

    tube1 = tube2;
    CHECK(tube1 == tube2);
    CHECK_FALSE(tube1 != tube2);

    tube1.set(Interval(2.,3.), 0.5);
    CHECK(tube1 != tube2);
    CHECK_FALSE(tube1 == tube2);

    tube1.set(Interval(1.,3.), 0.5);
    CHECK(tube1 == tube2);
    CHECK_FALSE(tube1 != tube2);

    tube1.set(Interval(1.,2.2), 1.);
    CHECK(tube1 != tube2);
    CHECK_FALSE(tube1 == tube2);

    tube1.set(Interval(1.,3.2), 1.); // gate changed to [1.,3.]
    CHECK(tube1 == tube2);
    CHECK_FALSE(tube1 != tube2);

    tube1.set(Interval(1.,2.2), 1.);
    CHECK(tube1 != tube2);
    CHECK_FALSE(tube1 == tube2);

    Tube tube_a1 = tubeTest1();
    Tube tube_b1 = tubeTest1();
    CHECK(tube_a1 == tube_a1);
    CHECK(tube_a1 == tube_b1);
    CHECK_FALSE(tube_a1 != tube_a1);
    CHECK_FALSE(tube_a1 != tube_b1);

    Tube tube_a2 = tubeTest4();
    Tube tube_b2 = tubeTest4();
    CHECK(tube_a2 == tube_a2);
    CHECK(tube_a2 == tube_b2);
    CHECK_FALSE(tube_a2 != tube_a2);
    CHECK_FALSE(tube_a2 != tube_b2);
  }
}

TEST_CASE("Testing isSubset()")
{
  SECTION("TubeSlice")
  {
    TubeSlice slice1(Interval(0.,1.), IntervalVector(1, Interval(1.,3.)));
    TubeSlice slice2(Interval(0.,1.), IntervalVector(1, Interval(0.,4.)));
    TubeSlice slice3(Interval(0.,1.4), IntervalVector(1, Interval(0.,4.)));

    CHECK_THROWS(slice1.isSubset(slice3));
    CHECK(slice1.isSubset(slice2));

    slice1.setInputGate(IntervalVector(1, Interval(0.5,2.)));
    CHECK(slice1.isSubset(slice2));
    CHECK(slice1[0.][0] == Interval(1.,2.));

    slice1.setOutputGate(IntervalVector(1, Interval(0.5,2.)));
    CHECK(slice1.isSubset(slice2));
    CHECK(slice1[1.][0] == Interval(1.,2.));
  }

  SECTION("Tube")
  {
    Tube tube1(Interval(0.,10.), 1., Interval(0.,5.));
    Tube tube2(Interval(0.,10.), 1., Interval(0.,5.));
    Tube tube3(Interval(0.,10.), 0.5);

    CHECK_THROWS(tube1.isSubset(tube3));
    CHECK(tube1.isSubset(tube2));
    CHECK(!tube1.isStrictSubset(tube2));
    tube1.set(Interval(1.,4.));
    CHECK(tube1.isSubset(tube2));

    tube1 = tube2;
    CHECK(tube1.isSubset(tube2));
    CHECK(!tube1.isStrictSubset(tube2));
    tube1.set(Interval(1.,5.), 2); // strict subset due to a slice only
    CHECK(tube1.isSubset(tube2));
    CHECK(tube1 != tube2);
    CHECK(tube1.isStrictSubset(tube2));

    tube1 = tube2;
    CHECK(tube1.isSubset(tube2));
    CHECK(!tube1.isStrictSubset(tube2));
    tube1.set(Interval(1.,5.), 2.); // strict subset due to a gate only
    CHECK(tube1.isSubset(tube2));
    CHECK(tube1.isStrictSubset(tube2));
  }
}

TEST_CASE("Testing encloses()")
{
  SECTION("TubeSlice")
  {
    TubeSlice slice1(Interval(0.,1.), IntervalVector(1, Interval(1.,3.)));
    // todo
  }

  SECTION("Tube")
  {
    Tube tube1(Interval(0.,10.), 1., Interval(0.,5.));
    // todo
  }
}

TEST_CASE("Testing isEmpty()")
{
  SECTION("TubeSlice")
  {
    TubeSlice slice1(Interval(0.,1.), IntervalVector(1, Interval(1.,3.)));
    CHECK(!slice1.isEmpty());
    slice1.setInputGate(IntervalVector(1, Interval(5.)));
    CHECK(slice1.isEmpty());
    slice1.setInputGate(IntervalVector(1, Interval(2.)));
    CHECK(!slice1.isEmpty());
    slice1.setOutputGate(IntervalVector(1, Interval(5.)));
    CHECK(slice1.isEmpty());
    slice1.setOutputGate(IntervalVector(1, Interval(2.)));
    CHECK(!slice1.isEmpty());
    slice1.set(IntervalVector(1, Interval::EMPTY_SET));
    CHECK(slice1.isEmpty());
    slice1.set(IntervalVector(1, Interval::ALL_REALS));
    CHECK(!slice1.isEmpty());
  }

  SECTION("Tube")
  {
    Tube tube1(Interval(0.,10.), Interval(0.,5.));
    CHECK(!tube1.isEmpty());
    tube1.set(Interval(6.), 2.3);
    CHECK(tube1.isEmpty());
    tube1.set(Interval(5.), 2.3);
    CHECK(!tube1.isEmpty());
    tube1.set(Interval::EMPTY_SET, 0);
    CHECK(tube1.isEmpty());
    tube1.set(Interval(4.,5.));
    CHECK(!tube1.isEmpty());
    tube1.set(Interval::EMPTY_SET, 10.);
    CHECK(tube1.isEmpty());
  }

  SECTION("Tube test")
  {
    Tube tube = tubeTest1();
    CHECK_FALSE(tube.isEmpty());
    tube.set(Interval::EMPTY_SET);
    CHECK(tube.isEmpty());
    tube = tubeTest1();
    tube.set(Interval::EMPTY_SET, 10);
    CHECK(tube.isEmpty());
    tube.set(Interval(5.), 10);
    CHECK(tube.isEmpty());
  }
}

TEST_CASE("Testing inflate()")
{
  SECTION("TubeSlice")
  {
    TubeSlice slice(Interval(0.,10.), IntervalVector(1, Interval(0.)));

    CHECK(slice.codomain()[0] == Interval(0.));

    slice.inflate(0.2);

    CHECK(slice.codomain()[0] == Interval(-0.2,0.2));

    slice.inflate(1.);

    CHECK(ApproxIntv(slice.codomain()[0]) == Interval(-1.2,1.2));
    CHECK(ApproxIntv(slice.inputGate()[0]) == Interval(-1.2,1.2));
    CHECK(ApproxIntv(slice.outputGate()[0]) == Interval(-1.2,1.2));

    slice.setInputGate(IntervalVector(1, Interval(0.,0.5)));
    slice.setOutputGate(IntervalVector(1, Interval(0.,0.3)));

    CHECK(slice[0.][0] == Interval(0.,0.5));
    CHECK(slice[10.][0] == Interval(0.,0.3));

    slice.inflate(1.);

    CHECK(ApproxIntv(slice.codomain()[0]) == Interval(-2.2,2.2));
    CHECK(ApproxIntv(slice.inputGate()[0]) == Interval(-1.,1.5));
    CHECK(ApproxIntv(slice.outputGate()[0]) == Interval(-1.,1.3));
  }

  SECTION("Tube")
  {
    Tube tube(Interval(0.,10.), 0.8, Interval(0.));
    CHECK(tube.codomain() == Interval(0.));
    CHECK(tube[3] == Interval(0.));
    CHECK(tube.getSlice(3)->inputGate()[0] == Interval(0.));
    CHECK(tube.getSlice(6)->inputGate()[0] == Interval(0.));
    tube.inflate(0.2);
    CHECK(tube.codomain() == Interval(-0.2,0.2));
    CHECK(tube[6] == Interval(-0.2,0.2));
    CHECK(tube.getSlice(6)->inputGate()[0] == Interval(-0.2,0.2));
    tube.inflate(1.);
    CHECK(ApproxIntv(tube.codomain()) == Interval(-1.2,1.2));
    CHECK(ApproxIntv(tube[9]) == Interval(-1.2,1.2));
    CHECK(ApproxIntv(tube.getSlice(9)->inputGate()[0]) == Interval(-1.2,1.2));
    double t = tube.getSlice(9)->domain().lb();
    tube.set(Interval(3.,7.), 8);
    tube.set(Interval(3.,7.), 9);
    tube.set(Interval(4.,6.), t);
    CHECK(ApproxIntv(tube[t]) == Interval(4.,6.));
    tube.inflate(2.);
    CHECK(ApproxIntv(tube[8]) == Interval(1.,9.));
    CHECK(ApproxIntv(tube[9]) == Interval(1.,9.));
    CHECK(ApproxIntv(tube[t]) == Interval(2.,8.));
  }
}

TEST_CASE("Testing volume()")
{
  SECTION("TubeSlice")
  {
    TubeSlice slice(Interval(0.,10.), IntervalVector(1, Interval(4.,5.)));
    CHECK(slice.volume() == 10.);
  }

  SECTION("Tube")
  {
    Tube tube1 = tubeTest1();
    tube1.set(Interval(-4,2), 14);
    CHECK(tube1.volume() == 197.);
    Tube tube4 = tubeTest4();
    CHECK(tube4.volume() == 9.+2.+1.+2.+1.+(21.-14.)*1.);
  }
}