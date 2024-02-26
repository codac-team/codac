#include "catch_interval.hpp"
#include "tests_predefined_tubes.h"
#include <cstdio>

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace codac;

#define TEST_COMPUTATION_TIMES 0
#define COEFF_COMPUTATION_TIME 1.

TEST_CASE("Computing tube's primitive", "[core]")
{
  SECTION("Test tube3")
  {
    double t[2];
    clock_t t_start;

    for(int i = 0 ; i < 2 ; i++)
    {
      Tube tube = tube_test3();
      if(i == 0) tube.enable_synthesis(SynthesisMode::BINARY_TREE);

      t_start = clock();
      CHECK(ApproxIntv(tube(0)) == Interval(1,3));
      CHECK(ApproxIntv(tube(1)) == Interval(0,2));
      CHECK(ApproxIntv(tube(2)) == Interval(-1,1));
      CHECK(ApproxIntv(tube(3)) == Interval(-2,0));
      CHECK(ApproxIntv(tube(4)) == Interval(-3,-1));
      t[i] = (double)(clock() - t_start)/CLOCKS_PER_SEC;

      Tube tube_primitive = tube.primitive();
      if(i == 0) tube.enable_synthesis(SynthesisMode::BINARY_TREE);

      t_start = clock();
      CHECK(ApproxIntv(tube_primitive(0)) == Interval(0,3));
      CHECK(ApproxIntv(tube_primitive(1)) == Interval(1,5));
      CHECK(ApproxIntv(tube_primitive(2)) == Interval(0,6));
      CHECK(ApproxIntv(tube_primitive(3)) == Interval(-2,6));
      CHECK(ApproxIntv(tube_primitive(4)) == Interval(-5,6));
      t[i] += (double)(clock() - t_start)/CLOCKS_PER_SEC;

      tube.enable_synthesis(SynthesisMode::NONE);
      tube_primitive.enable_synthesis(SynthesisMode::NONE);
    }

    //if(TEST_COMPUTATION_TIMES) CHECK(COEFF_COMPUTATION_TIME*t[0] < t[1]);
  }

  SECTION("Test tube4")
  {
    for(int i = 0 ; i < 2 ; i++)
    {
      Tube tube = tube_test4();
      tube.set(Interval(-1,1), Interval(10,11));
      if(i == 0) tube.enable_synthesis(SynthesisMode::BINARY_TREE);

      Tube tube_primitive = tube.primitive();
      if(i == 0) tube.enable_synthesis(SynthesisMode::BINARY_TREE);

      CHECK(ApproxIntv(tube_primitive(Interval(0,1))) == Interval(0,2));
      CHECK(ApproxIntv(tube_primitive(Interval(1,2))) == Interval(1,4));
      CHECK(ApproxIntv(tube_primitive(Interval(2,3))) == Interval(2,6));
      CHECK(ApproxIntv(tube_primitive(Interval(3,4))) == Interval(3,8));
      CHECK(ApproxIntv(tube_primitive(Interval(4,5))) == Interval(4,10));
      CHECK(ApproxIntv(tube_primitive(Interval(5,6))) == Interval(5,12));
      CHECK(ApproxIntv(tube_primitive(Interval(6,7))) == Interval(6,14));
      CHECK(ApproxIntv(tube_primitive(Interval(7,8))) == Interval(7,16));
      CHECK(ApproxIntv(tube_primitive(Interval(8,9))) == Interval(8,18));
      CHECK(ApproxIntv(tube_primitive(Interval(9,10))) == Interval(9,19.5));
      CHECK(ApproxIntv(tube_primitive(Interval(10,11))) == Interval(8.5,20.5));
      CHECK(ApproxIntv(tube_primitive(Interval(11,12))) == Interval(7,20.5));
      CHECK(ApproxIntv(tube_primitive(Interval(12,13))) == Interval(6,21));
      CHECK(ApproxIntv(tube_primitive(Interval(13,14))) == Interval(6,22.5));
      CHECK(ApproxIntv(tube_primitive(Interval(14,15))) == Interval(6.5,24.5));
      CHECK(ApproxIntv(tube_primitive(Interval(15,16))) == Interval(7.5,26.5));
      CHECK(ApproxIntv(tube_primitive(Interval(16,17))) == Interval(8.5,28.5));
      CHECK(ApproxIntv(tube_primitive(Interval(17,18))) == Interval(9.5,30.5));
      CHECK(ApproxIntv(tube_primitive(Interval(18,19))) == Interval(10.5,32.5));
      CHECK(ApproxIntv(tube_primitive(Interval(19,20))) == Interval(11.5,34.5));
      CHECK(ApproxIntv(tube_primitive(Interval(20,21))) == Interval(12.5,36.5));

      CHECK(ApproxIntv(tube_primitive(0)) == Interval(0,2));
      CHECK(ApproxIntv(tube_primitive(1)) == Interval(1,4));
      CHECK(ApproxIntv(tube_primitive(2)) == Interval(2,6));
      CHECK(ApproxIntv(tube_primitive(3)) == Interval(3,8));
      CHECK(ApproxIntv(tube_primitive(4)) == Interval(4,10));
      CHECK(ApproxIntv(tube_primitive(5)) == Interval(5,12));
      CHECK(ApproxIntv(tube_primitive(6)) == Interval(6,14));
      CHECK(ApproxIntv(tube_primitive(7)) == Interval(7,16));
      CHECK(ApproxIntv(tube_primitive(8)) == Interval(8,18));
      CHECK(ApproxIntv(tube_primitive(9)) == Interval(9,19.5)); // domain [9,10]
      CHECK(ApproxIntv(tube_primitive(10.)) == Interval(9.5,19.5)); // singleton
      CHECK(ApproxIntv(tube_primitive(10+1)) == Interval(8.5,20.5)); // domain [10,10.2]
      CHECK(ApproxIntv(tube_primitive(11+1)) == Interval(7,20.5));
      CHECK(ApproxIntv(tube_primitive(12+1)) == Interval(6,21));
      CHECK(ApproxIntv(tube_primitive(13+1)) == Interval(6,22.5));
      CHECK(ApproxIntv(tube_primitive(14+1)) == Interval(6.5,24.5));
      CHECK(ApproxIntv(tube_primitive(15+1)) == Interval(7.5,26.5));
      CHECK(ApproxIntv(tube_primitive(16+1)) == Interval(8.5,28.5));
      CHECK(ApproxIntv(tube_primitive(17+1)) == Interval(9.5,30.5));
      CHECK(ApproxIntv(tube_primitive(18+1)) == Interval(10.5,32.5));
      CHECK(ApproxIntv(tube_primitive(19+1)) == Interval(11.5,34.5));
      CHECK(ApproxIntv(tube_primitive(20+1)) == Interval(12.5,36.5));

      tube.enable_synthesis(SynthesisMode::NONE);
      tube_primitive.enable_synthesis(SynthesisMode::NONE);
    }
  }

  SECTION("Test tube1")
  {
    for(int i = 0 ; i < 2 ; i++)
    {
      Tube tube = tube_test_1();
      tube.set(Interval(-4,2), 14); // to test primitives pre-computation
      if(i == 0) tube.enable_synthesis(SynthesisMode::BINARY_TREE);

      Tube tube_primitive = tube.primitive();
      if(i == 0) tube.enable_synthesis(SynthesisMode::BINARY_TREE);

      CHECK(ApproxIntv(tube_primitive(0)) == Interval(0,8));
      CHECK(ApproxIntv(tube_primitive(1)) == Interval(4,15));
      CHECK(ApproxIntv(tube_primitive(2)) == Interval(6,21));

      tube.enable_synthesis(SynthesisMode::NONE);
      tube_primitive.enable_synthesis(SynthesisMode::NONE);
    }
  }

  SECTION("Test tube4(05)")
  {
    for(int i = 0 ; i < 2 ; i++)
    {
      Tube tube = tube_test4_05();
      if(i == 0) tube.enable_synthesis(SynthesisMode::BINARY_TREE);

      Tube tube_primitive = tube.primitive();
      if(i == 0) tube.enable_synthesis(SynthesisMode::BINARY_TREE);

      CHECK(ApproxIntv(tube_primitive(0)) == Interval(0.0,1));
      CHECK(ApproxIntv(tube_primitive(1)) == Interval(0.5,2));
      CHECK(ApproxIntv(tube_primitive(2)) == Interval(1.0,3));
      CHECK(ApproxIntv(tube_primitive(3)) == Interval(1.5,4));
      CHECK(ApproxIntv(tube_primitive(4)) == Interval(2.0,5));
      CHECK(ApproxIntv(tube_primitive(5)) == Interval(2.5,6));
      CHECK(ApproxIntv(tube_primitive(6)) == Interval(3.0,7));
      CHECK(ApproxIntv(tube_primitive(7)) == Interval(3.5,8));
      CHECK(ApproxIntv(tube_primitive(8)) == Interval(4.0,9));
      CHECK(ApproxIntv(tube_primitive(9)) == Interval(4.5,10));
      CHECK(ApproxIntv(tube_primitive(10)) == Interval(5.0,11));
      CHECK(ApproxIntv(tube_primitive(11)) == Interval(5.5,12));
      CHECK(ApproxIntv(tube_primitive(12)) == Interval(6.0,13));
      CHECK(ApproxIntv(tube_primitive(13)) == Interval(6.5,14));
      
      tube.enable_synthesis(SynthesisMode::NONE);
      tube_primitive.enable_synthesis(SynthesisMode::NONE);
    }
  }
}

TEST_CASE("Computing integration from 0, double argument", "[core]")
{
  SECTION("Test tube1")
  {
    Tube tube = tube_test_1();
    tube.set(Interval(-4,2), 14); // to test primitives pre-computation
    tube.enable_synthesis(SynthesisMode::BINARY_TREE); // first: testing with tree synthesis

    double t[2];
    clock_t t_start;

    for(int i = 0 ; i < 2 ; i++)
    {
      t_start = clock();
      CHECK(ApproxIntv(tube.integral(0)) == Interval(0));
      CHECK(ApproxIntv(tube.integral(0.1)) == Interval(0.4,0.8));
      CHECK(ApproxIntv(tube.integral(1.1)) == Interval(4.2,8.7));
      CHECK(ApproxIntv(tube.integral(0.5)) == Interval(2,4));
      CHECK(ApproxIntv(tube.integral(1.0)) == Interval(4,8));
      CHECK(ApproxIntv(tube.integral(1.5)) == Interval(5,11.5));
      CHECK(ApproxIntv(tube.integral(2.0)) == Interval(6,15));
      CHECK(ApproxIntv(tube.integral(2.5)) == Interval(6.5,18));
      CHECK(ApproxIntv(tube.integral(2.9)) == Interval(6.9,20.4));
      CHECK(ApproxIntv(tube.integral(3.0)) == Interval(7,21));
      CHECK(ApproxIntv(tube.integral(3.1)) == Interval(6.6,21.4));
      CHECK(ApproxIntv(tube.integral(4.0)) == Interval(3,25));
      t[i] = (double)(clock() - t_start)/CLOCKS_PER_SEC;

      tube.enable_synthesis(SynthesisMode::NONE); // second: without tree synthesis
    }

    if(TEST_COMPUTATION_TIMES) CHECK(COEFF_COMPUTATION_TIME*t[0] < t[1]);
  }

  SECTION("Test tube1(01)")
  {
    Tube tube = tube_test_1_01();
    tube.enable_synthesis(SynthesisMode::BINARY_TREE); // first: testing with tree synthesis

    double t[2];
    clock_t t_start;

    for(int i = 0 ; i < 2 ; i++)
    {
      t_start = clock();
      CHECK(ApproxIntv(tube.integral(0)) == Interval(0));
      CHECK(ApproxIntv(tube.integral(0.1)) == Interval(0.4,0.8));
      CHECK(ApproxIntv(tube.integral(1.1)) == Interval(4.2,8.7));
      CHECK(ApproxIntv(tube.integral(0.5)) == Interval(2,4));
      CHECK(ApproxIntv(tube.integral(1.0)) == Interval(4,8));
      CHECK(ApproxIntv(tube.integral(1.5)) == Interval(5,11.5));
      CHECK(ApproxIntv(tube.integral(2.0)) == Interval(6,15));
      CHECK(ApproxIntv(tube.integral(2.5)) == Interval(6.5,18));
      CHECK(ApproxIntv(tube.integral(2.9)) == Interval(6.9,20.4));
      CHECK(ApproxIntv(tube.integral(3.0)) == Interval(7,21));
      CHECK(ApproxIntv(tube.integral(3.1)) == Interval(6.6,21.4));
      CHECK(ApproxIntv(tube.integral(4.0)) == Interval(3,25));
      t[i] = (double)(clock() - t_start)/CLOCKS_PER_SEC;

      tube.enable_synthesis(SynthesisMode::NONE); // second: without tree synthesis
    }

    if(TEST_COMPUTATION_TIMES) CHECK(COEFF_COMPUTATION_TIME*t[0] < t[1]);
  }
  
  SECTION("Test tube4")
  {
    Tube tube = tube_test4();
    tube.set(Interval(-1,1), Interval(10,11));
    tube.enable_synthesis(SynthesisMode::BINARY_TREE); // first: testing with tree synthesis

    double t[2];
    clock_t t_start;

    for(int i = 0 ; i < 2 ; i++)
    {
      t_start = clock();
      CHECK(ApproxIntv(tube.integral(0.0)) == Interval(0.0));
      CHECK(ApproxIntv(tube.integral(0.1)) == Interval(0.1,0.2));
      CHECK(ApproxIntv(tube.integral(0.9)) == Interval(0.9,1.8));
      CHECK(ApproxIntv(tube.integral(1.0)) == Interval(1,2));
      CHECK(ApproxIntv(tube.integral(1.2)) == Interval(1.2,2.4));
      CHECK(ApproxIntv(tube.integral(2.0)) == Interval(2.0,4.0));
      CHECK(ApproxIntv(tube.integral(3.0)) == Interval(3.0,6.0));
      CHECK(ApproxIntv(tube.integral(5.0)) == Interval(5.0,10.0));
      CHECK(ApproxIntv(tube.integral(7.0)) == Interval(7.0,14.0));
      CHECK(ApproxIntv(tube.integral(9.0)) == Interval(9.0,18.0));
      CHECK(ApproxIntv(tube.integral(10.0)) == Interval(9.5,19.5));
      CHECK(ApproxIntv(tube.integral(10.1)) == Interval(9.4,19.6));
      CHECK(ApproxIntv(tube.integral(11.0)) == Interval(8.5,20.5));
      CHECK(ApproxIntv(tube.integral(11.5)) == Interval(7.75,20.25));
      CHECK(ApproxIntv(tube.integral(12.5)) == Interval(6.5,20.5));
      CHECK(ApproxIntv(tube.integral(12.6)) == Interval(6.4,20.6));
      CHECK(ApproxIntv(tube.integral(14.5)) == Interval(7,23.5));
      t[i] = (double)(clock() - t_start)/CLOCKS_PER_SEC;

      tube.enable_synthesis(SynthesisMode::NONE); // second: without tree synthesis
    }

    if(TEST_COMPUTATION_TIMES) CHECK(COEFF_COMPUTATION_TIME*t[0] < t[1]);
  }
}

TEST_CASE("Computing integration from 0, interval argument", "[core]")
{
  SECTION("Test tube1")
  {
    Tube tube = tube_test_1();
    tube.set(Interval(-4,2), 14); // to test primitives pre-computation
    tube.enable_synthesis(SynthesisMode::BINARY_TREE); // first: testing with tree synthesis

    double t[2];
    clock_t t_start;

    for(int i = 0 ; i < 2 ; i++)
    {
      t_start = clock();
      CHECK(ApproxIntv(tube.integral(Interval(0))) == Interval(0));
      CHECK(ApproxIntv(tube.integral(Interval(0.1))) == Interval(0.4,0.8));
      CHECK(ApproxIntv(tube.integral(Interval(0.5))) == Interval(2,4));
      CHECK(ApproxIntv(tube.integral(Interval(1.0))) == Interval(4,8));
      CHECK(ApproxIntv(tube.integral(Interval(1.5))) == Interval(5,11.5));
      CHECK(ApproxIntv(tube.integral(Interval(2.0))) == Interval(6,15));
      CHECK(ApproxIntv(tube.integral(Interval(2.5))) == Interval(6.5,18));
      CHECK(ApproxIntv(tube.integral(Interval(3.0))) == Interval(7,21));
      CHECK(ApproxIntv(tube.integral(Interval(3.0))) == Interval(7,21));
      CHECK(ApproxIntv(tube.integral(Interval(0.0,0.0))) == Interval(0,0));
      CHECK(ApproxIntv(tube.integral(Interval(0.0,0.5))) == Interval(0,4));
      CHECK(ApproxIntv(tube.integral(Interval(0.0,1.0))) == Interval(0,8));
      CHECK(ApproxIntv(tube.integral(Interval(0.0,1.5))) == Interval(0,11.5));
      CHECK(ApproxIntv(tube.integral(Interval(0.0,2.0))) == Interval(0,15));
      CHECK(ApproxIntv(tube.integral(Interval(1.0,1.0))) == Interval(4,8));
      CHECK(ApproxIntv(tube.integral(Interval(1.0,1.5))) == Interval(4,11.5));
      CHECK(ApproxIntv(tube.integral(Interval(1.0,2.0))) == Interval(4,15));
      CHECK(ApproxIntv(tube.integral(Interval(1.1,1.9))) == Interval(4.2,14.3));
      CHECK(ApproxIntv(tube.integral(Interval(1.1,2.1))) == Interval(4.2,15.6));
      CHECK(ApproxIntv(tube.integral(Interval(6,7))) == Interval(-23,19));
      CHECK(ApproxIntv(tube.integral(Interval(0,46))) == Interval(-85,194));
      t[i] = (double)(clock() - t_start)/CLOCKS_PER_SEC;

      tube.enable_synthesis(SynthesisMode::NONE); // second: without tree synthesis
    }

    if(TEST_COMPUTATION_TIMES) CHECK(COEFF_COMPUTATION_TIME*t[0] < t[1]);
  }

  SECTION("Test tube1(01)")
  {
    Tube tube = tube_test_1_01();
    tube.enable_synthesis(SynthesisMode::BINARY_TREE); // first: testing with tree synthesis

    double t[2];
    clock_t t_start;

    for(int i = 0 ; i < 2 ; i++)
    {
      t_start = clock();
      CHECK(ApproxIntv(tube.integral(Interval(0))) == Interval(0));
      CHECK(ApproxIntv(tube.integral(Interval(0.1))) == Interval(0.4,0.8));
      CHECK(ApproxIntv(tube.integral(Interval(0.5))) == Interval(2,4));
      CHECK(ApproxIntv(tube.integral(Interval(1.0))) == Interval(4,8));
      CHECK(ApproxIntv(tube.integral(Interval(1.5))) == Interval(5,11.5));
      CHECK(ApproxIntv(tube.integral(Interval(2.0))) == Interval(6,15));
      CHECK(ApproxIntv(tube.integral(Interval(2.5))) == Interval(6.5,18));
      CHECK(ApproxIntv(tube.integral(Interval(3.0))) == Interval(7,21));
      CHECK(ApproxIntv(tube.integral(Interval(3.0))) == Interval(7,21));
      CHECK(ApproxIntv(tube.integral(Interval(0.0,0.0))) == Interval(0,0));
      CHECK(ApproxIntv(tube.integral(Interval(0.0,0.5))) == Interval(0,4));
      CHECK(ApproxIntv(tube.integral(Interval(0.0,1.0))) == Interval(0,8));
      CHECK(ApproxIntv(tube.integral(Interval(0.0,1.5))) == Interval(0,11.5));
      CHECK(ApproxIntv(tube.integral(Interval(0.0,2.0))) == Interval(0,15));
      CHECK(ApproxIntv(tube.integral(Interval(1.0,1.0))) == Interval(4,8));
      CHECK(ApproxIntv(tube.integral(Interval(1.0,1.5))) == Interval(4,11.5));
      CHECK(ApproxIntv(tube.integral(Interval(1.0,2.0))) == Interval(4,15));
      CHECK(ApproxIntv(tube.integral(Interval(1.1,1.9))) == Interval(4.2,14.3));
      CHECK(ApproxIntv(tube.integral(Interval(1.1,2.1))) == Interval(4.2,15.6));
      CHECK(ApproxIntv(tube.integral(Interval(6,7))) == Interval(-23,19));
      CHECK(ApproxIntv(tube.integral(Interval(0,46))) == Interval(-85,194));
      t[i] = (double)(clock() - t_start)/CLOCKS_PER_SEC;

      tube.enable_synthesis(SynthesisMode::NONE); // second: without tree synthesis
    }

    if(TEST_COMPUTATION_TIMES) CHECK(COEFF_COMPUTATION_TIME*t[0] < t[1]);
  }

  SECTION("Test tube4")
  {
    Tube tube = tube_test4();
    tube.set(Interval(-1,1), Interval(10,11));
    tube.enable_synthesis(SynthesisMode::BINARY_TREE); // first: testing with tree synthesis

    double t[2];
    clock_t t_start;

    for(int i = 0 ; i < 2 ; i++)
    {
      t_start = clock();
      CHECK(ApproxIntv(tube.integral(Interval(12.5))) == Interval(6.5,20.5));
      CHECK(ApproxIntv(tube.integral(Interval(14.5))) == Interval(7,23.5));
      pair<Interval,Interval> p_intv = tube.partial_integral(Interval(12.5,14.5));
      CHECK(p_intv.first == Interval(6.,7.));
      CHECK(p_intv.second == Interval(20.5,23.5));
      CHECK(ApproxIntv(tube.integral(Interval(12.5,14.5))) == Interval(6.0,23.5));
      CHECK(ApproxIntv(tube.integral(Interval(0))) == Interval(0));
      CHECK(ApproxIntv(tube.integral(Interval(10.2))) == Interval(9.3,19.7));
      CHECK(ApproxIntv(tube.integral(Interval(1.5,3.5))) == Interval(1.5,7));
      CHECK(ApproxIntv(tube.integral(Interval(9,21))) == Interval(6,36.5));
      t[i] = (double)(clock() - t_start)/CLOCKS_PER_SEC;

      tube.enable_synthesis(SynthesisMode::NONE); // second: without tree synthesis
    }

    if(TEST_COMPUTATION_TIMES) CHECK(COEFF_COMPUTATION_TIME*t[0] < t[1]);
  }

  SECTION("Test tube4(05)")
  {
    Tube tube = tube_test4_05();
    tube.set(Interval(-1,1), Interval(10,11));
    tube.enable_synthesis(SynthesisMode::BINARY_TREE); // first: testing with tree synthesis

    double t[2];
    clock_t t_start;

    for(int i = 0 ; i < 2 ; i++)
    {
      t_start = clock();
      CHECK(ApproxIntv(tube.integral(Interval(12.5))) == Interval(6.5,20.5));
      CHECK(ApproxIntv(tube.integral(Interval(14.5))) == Interval(7,23.5));
      pair<Interval,Interval> p_intv = tube.partial_integral(Interval(12.5,14.5));
      CHECK(p_intv.first == Interval(6.,7.));
      CHECK(p_intv.second == Interval(20.5,23.5));
      CHECK(ApproxIntv(tube.integral(Interval(12.5,14.5))) == Interval(6.0,23.5));
      CHECK(ApproxIntv(tube.integral(Interval(0))) == Interval(0));
      CHECK(ApproxIntv(tube.integral(Interval(10.2))) == Interval(9.3,19.7));
      CHECK(ApproxIntv(tube.integral(Interval(1.5,3.5))) == Interval(1.5,7));
      CHECK(ApproxIntv(tube.integral(Interval(9,21))) == Interval(6,36.5));
      t[i] = (double)(clock() - t_start)/CLOCKS_PER_SEC;

      tube.enable_synthesis(SynthesisMode::NONE); // second: without tree synthesis
    }

    if(TEST_COMPUTATION_TIMES) CHECK(COEFF_COMPUTATION_TIME*t[0] < t[1]);
  }
}

TEST_CASE("Computing integration from 0, partial integration", "[core]")
{
  SECTION("Test tube4")
  {
    Tube tube = tube_test4();
    tube.set(Interval(-1,1), Interval(10,11));
    tube.enable_synthesis(SynthesisMode::BINARY_TREE); // first: testing with tree synthesis

    double t[2];
    clock_t t_start;

    for(int i = 0 ; i < 2 ; i++)
    {
      t_start = clock();
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(0))) == make_pair(Interval(0.), Interval(0.)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(0.2))) == make_pair(Interval(0.2), Interval(0.4)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(0.1))) == make_pair(Interval(0.1), Interval(0.2)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(0.9))) == make_pair(Interval(0.9), Interval(1.8)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(1.0))) == make_pair(Interval(1), Interval(2)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(1.2))) == make_pair(Interval(1.2), Interval(2.4)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(2.0))) == make_pair(Interval(2.0), Interval(4.0)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(3.0))) == make_pair(Interval(3.0), Interval(6.0)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(5.0))) == make_pair(Interval(5.0), Interval(10.0)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(7.0))) == make_pair(Interval(7.0), Interval(14.0)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(9.0))) == make_pair(Interval(9.0), Interval(18.0)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(10.0))) == make_pair(Interval(9.5), Interval(19.5)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(10.1))) == make_pair(Interval(9.4), Interval(19.6)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(11.0))) == make_pair(Interval(8.5), Interval(20.5)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(12.5))) == make_pair(Interval(6.5), Interval(20.5)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(12.6))) == make_pair(Interval(6.4), Interval(20.6)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(14.5))) == make_pair(Interval(7), Interval(23.5)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(12.5,13))) == make_pair(Interval(6,6.5), Interval(20.5,21)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(13,14))) == make_pair(Interval(6,6.5), Interval(21,22.5)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(14,14.5))) == make_pair(Interval(6.5,7), Interval(22.5,23.5)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(12.5,14.5))) == make_pair(Interval(6.0,7.0), Interval(20.5,23.5)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(1.5,3.5))) == make_pair(Interval(1.5,3.5), Interval(3,7)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(9,21))) == make_pair(Interval(6,13.5), Interval(18,36.5)));
      t[i] = (double)(clock() - t_start)/CLOCKS_PER_SEC;

      tube.enable_synthesis(SynthesisMode::NONE); // second: without tree synthesis
    }

    if(TEST_COMPUTATION_TIMES) CHECK(COEFF_COMPUTATION_TIME*t[0] < t[1]);
  }

  SECTION("Test tube4(05)")
  {
    Tube tube = tube_test4_05();
    tube.enable_synthesis(SynthesisMode::BINARY_TREE); // first: testing with tree synthesis

    double t[2];
    clock_t t_start;

    for(int i = 0 ; i < 2 ; i++)
    {
      t_start = clock();
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(0))) == make_pair(Interval(0.), Interval(0.)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(0.2))) == make_pair(Interval(0.2), Interval(0.4)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(0.1))) == make_pair(Interval(0.1), Interval(0.2)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(0.9))) == make_pair(Interval(0.9), Interval(1.8)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(1.0))) == make_pair(Interval(1), Interval(2)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(1.2))) == make_pair(Interval(1.2), Interval(2.4)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(2.0))) == make_pair(Interval(2.0), Interval(4.0)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(3.0))) == make_pair(Interval(3.0), Interval(6.0)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(5.0))) == make_pair(Interval(5.0), Interval(10.0)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(7.0))) == make_pair(Interval(7.0), Interval(14.0)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(9.0))) == make_pair(Interval(9.0), Interval(18.0)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(10.0))) == make_pair(Interval(9.5), Interval(19.5)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(10.1))) == make_pair(Interval(9.4), Interval(19.6)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(11.0))) == make_pair(Interval(8.5), Interval(20.5)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(12.5))) == make_pair(Interval(6.5), Interval(20.5)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(12.6))) == make_pair(Interval(6.4), Interval(20.6)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(14.5))) == make_pair(Interval(7), Interval(23.5)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(12.5,13))) == make_pair(Interval(6,6.5), Interval(20.5,21)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(13,14))) == make_pair(Interval(6,6.5), Interval(21,22.5)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(14,14.5))) == make_pair(Interval(6.5,7), Interval(22.5,23.5)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(12.5,14.5))) == make_pair(Interval(6.0,7.0), Interval(20.5,23.5)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(1.5,3.5))) == make_pair(Interval(1.5,3.5), Interval(3,7)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(9,21))) == make_pair(Interval(6,13.5), Interval(18,36.5)));
      t[i] = (double)(clock() - t_start)/CLOCKS_PER_SEC;

      tube.enable_synthesis(SynthesisMode::NONE); // second: without tree synthesis
    }

    if(TEST_COMPUTATION_TIMES) CHECK(COEFF_COMPUTATION_TIME*t[0] < t[1]);
  }

  SECTION("Test tube1")
  {
    Tube tube = tube_test_1();
    tube.set(Interval(-4,2), 14); // to test primitives pre-computation
    tube.enable_synthesis(SynthesisMode::BINARY_TREE); // first: testing with tree synthesis

    double t[2];
    clock_t t_start;

    for(int i = 0 ; i < 2 ; i++)
    {
      t_start = clock();
      CHECK(ApproxIntvPair(tube.partial_integral(tube.tdomain())) == make_pair(Interval(-85,7), Interval(-16,194)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(0.,46.))) == make_pair(Interval(-85,7), Interval(-16,194)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(7.))) == make_pair(Interval(-23), Interval(13)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(0.,7.))) == make_pair(Interval(-23,7.0), Interval(0,25)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(2.,6.))) == make_pair(Interval(-13,7), Interval(15,25)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(6.,7.))) == make_pair(Interval(-23,-13), Interval(13,19)));

      Interval integrale_lb = Interval::EMPTY_SET;
      Interval integrale_ub = Interval::EMPTY_SET;
      for(double t = 0.0 ; t <= 46.0 ; t += 0.2)
      {
        integrale_lb |= tube.partial_integral(Interval(0, t)).first;
        integrale_ub |= tube.partial_integral(Interval(0, t)).second;
      }
      CHECK(integrale_lb.is_subset(Interval(-85,7)));
      CHECK(integrale_ub.is_subset(Interval(-16,194)));
      t[i] = (double)(clock() - t_start)/CLOCKS_PER_SEC;

      tube.enable_synthesis(SynthesisMode::NONE); // second: without tree synthesis
    }

    if(TEST_COMPUTATION_TIMES) CHECK(COEFF_COMPUTATION_TIME*t[0] < t[1]);
  }

  SECTION("Test tube1(01)")
  {
    Tube tube = tube_test_1_01();
    tube.enable_synthesis(SynthesisMode::BINARY_TREE); // first: testing with tree synthesis

    double t[2];
    clock_t t_start;

    for(int i = 0 ; i < 2 ; i++)
    {
      t_start = clock();
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(0.,46.))) == make_pair(Interval(-85,7), Interval(-16,194)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(7.))) == make_pair(Interval(-23), Interval(13)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(0.,7.))) == make_pair(Interval(-23,7.0), Interval(0,25)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(2.,6.))) == make_pair(Interval(-13,7), Interval(15,25)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(6.,7.))) == make_pair(Interval(-23,-13), Interval(13,19)));
      t[i] = (double)(clock() - t_start)/CLOCKS_PER_SEC;

      tube.enable_synthesis(SynthesisMode::NONE); // second: without tree synthesis
    }

    if(TEST_COMPUTATION_TIMES) CHECK(COEFF_COMPUTATION_TIME*t[0] < t[1]);
  }
}

TEST_CASE("Computing integration, two interval bounds", "[core]")
{
  SECTION("Test tube1")
  {
    Tube tube = tube_test_1();
    tube.set(Interval(-4,2), 14); // to test primitives pre-computation
    tube.enable_synthesis(SynthesisMode::BINARY_TREE); // first: testing with tree synthesis

    double t[2];
    clock_t t_start;

    for(int i = 0 ; i < 2 ; i++)
    {
      t_start = clock();
      CHECK(ApproxIntv(tube.integral(Interval(20), Interval(20))) == Interval(0.));
      CHECK(ApproxIntv(tube.integral(Interval(2.1), Interval(2.1))) == Interval(0.));
      CHECK(ApproxIntv(tube.integral(Interval(12.2), Interval(12.2))) == Interval(0.));
      CHECK(ApproxIntv(tube.integral(Interval(12.2), Interval(12.2))) == Interval(0.));
      CHECK(ApproxIntv(tube.integral(Interval(17.), Interval(20.))) == Interval(12,24));
      CHECK(ApproxIntv(tube.integral(Interval(28.), Interval(33.))) == Interval(17,31));
      CHECK(ApproxIntv(tube.integral(Interval(20.), Interval(28.))) == Interval(51,88));
      CHECK(ApproxIntv(tube.integral(Interval(17.), Interval(33.))) == Interval(80,143));
      CHECK(ApproxIntv(tube.integral(Interval(22.5), Interval(23))) == Interval(4.5,6));
      CHECK(ApproxIntv(tube.integral(Interval(8.5), Interval(10.5))) == Interval(-18,-9.5));
      CHECK(ApproxIntv(tube.integral(Interval(0), Interval(7))) == Interval(-23,13));
      CHECK(ApproxIntv(tube.integral(Interval(2), Interval(6))) == Interval(-19,4));
      CHECK(ApproxIntv(tube.integral(Interval(0,2), Interval(6,7))) == Interval(-29,19));
      CHECK(ApproxIntv(tube.integral(Interval(0), Interval(17))) == Interval(-85,-4));
      CHECK(ApproxIntv(tube.integral(Interval(0), Interval(38))) == Interval(1,161));
      CHECK(ApproxIntv(tube.integral(Interval(0), Interval(17,38))) == Interval(-85,161));
      CHECK(ApproxIntv(tube.integral(Interval(10), Interval(13))) == Interval(-21,-11));
      CHECK(ApproxIntv(tube.integral(Interval(17), Interval(20))) == Interval(12,24));
      CHECK(ApproxIntv(tube.integral(Interval(13), Interval(17))) == Interval(-11,11));
      CHECK(ApproxIntv(tube.integral(Interval(10,13), Interval(17))) == Interval(-32,11));
      CHECK(ApproxIntv(tube.integral(Interval(13), Interval(17,20))) == Interval(-11,35));
      CHECK(ApproxIntv(tube.integral(Interval(10,13), Interval(17,20))) == Interval(-32,35));
      CHECK(ApproxIntv(tube.integral(Interval(5,10), Interval(31,42))) == Interval(-6,180));
      t[i] = (double)(clock() - t_start)/CLOCKS_PER_SEC;

      tube.enable_synthesis(SynthesisMode::NONE); // second: without tree synthesis
    }

    if(TEST_COMPUTATION_TIMES) CHECK(COEFF_COMPUTATION_TIME*t[0] < t[1]);
  }

  SECTION("Test tube1(01)")
  {
    Tube tube = tube_test_1_01();
    tube.enable_synthesis(SynthesisMode::BINARY_TREE); // first: testing with tree synthesis

    double t[2];
    clock_t t_start;

    for(int i = 0 ; i < 2 ; i++)
    {
      t_start = clock();
      CHECK(ApproxIntv(tube.integral(Interval(20), Interval(20))) == Interval(0.));
      CHECK(ApproxIntv(tube.integral(Interval(2.1), Interval(2.1))) == Interval(0.));
      CHECK(ApproxIntv(tube.integral(Interval(12.2), Interval(12.2))) == Interval(0.));
      CHECK(ApproxIntv(tube.integral(Interval(12.2), Interval(12.2))) == Interval(0.));
      CHECK(ApproxIntv(tube.integral(Interval(17.), Interval(20.))) == Interval(12,24));
      CHECK(ApproxIntv(tube.integral(Interval(28.), Interval(33.))) == Interval(17,31));
      CHECK(ApproxIntv(tube.integral(Interval(20.), Interval(28.))) == Interval(51,88));
      CHECK(ApproxIntv(tube.integral(Interval(17.), Interval(33.))) == Interval(80,143));
      CHECK(ApproxIntv(tube.integral(Interval(22.5), Interval(23))) == Interval(4.5,6));
      CHECK(ApproxIntv(tube.integral(Interval(8.5), Interval(10.5))) == Interval(-18,-9.5));
      CHECK(ApproxIntv(tube.integral(Interval(0), Interval(7))) == Interval(-23,13));
      CHECK(ApproxIntv(tube.integral(Interval(2), Interval(6))) == Interval(-19,4));
      CHECK(ApproxIntv(tube.integral(Interval(0,2), Interval(6,7))) == Interval(-29,19));
      CHECK(ApproxIntv(tube.integral(Interval(0), Interval(17))) == Interval(-85,-4));
      CHECK(ApproxIntv(tube.integral(Interval(0), Interval(38))) == Interval(1,161));
      CHECK(ApproxIntv(tube.integral(Interval(0), Interval(17,38))) == Interval(-85,161));
      CHECK(ApproxIntv(tube.integral(Interval(10), Interval(13))) == Interval(-21,-11));
      CHECK(ApproxIntv(tube.integral(Interval(17), Interval(20))) == Interval(12,24));
      CHECK(ApproxIntv(tube.integral(Interval(13), Interval(17))) == Interval(-11,11));
      CHECK(ApproxIntv(tube.integral(Interval(10,13), Interval(17))) == Interval(-32,11));
      CHECK(ApproxIntv(tube.integral(Interval(13), Interval(17,20))) == Interval(-11,35));
      CHECK(ApproxIntv(tube.integral(Interval(10,13), Interval(17,20))) == Interval(-32,35));
      CHECK(ApproxIntv(tube.integral(Interval(5,10), Interval(31,42))) == Interval(-6,180));
      t[i] = (double)(clock() - t_start)/CLOCKS_PER_SEC;

      tube.enable_synthesis(SynthesisMode::NONE); // second: without tree synthesis
    }

    if(TEST_COMPUTATION_TIMES) CHECK(COEFF_COMPUTATION_TIME*t[0] < t[1]);
  }

  SECTION("Test tube4")
  {
    Tube tube = tube_test4();
    tube.set(Interval(-1,1), Interval(10,11));
    tube.enable_synthesis(SynthesisMode::BINARY_TREE); // first: testing with tree synthesis

    double t[2];
    clock_t t_start;

    for(int i = 0 ; i < 2 ; i++)
    {
      t_start = clock();
      CHECK(ApproxIntv(tube.integral(Interval(0,8), Interval(15,21))) == Interval(-0.5,36.5));
      CHECK(ApproxIntv(tube.integral(Interval(0,8), Interval(9,21))) == Interval(-2,36.5));
      t[i] = (double)(clock() - t_start)/CLOCKS_PER_SEC;

      tube.enable_synthesis(SynthesisMode::NONE); // second: without tree synthesis
    }

    if(TEST_COMPUTATION_TIMES) CHECK(COEFF_COMPUTATION_TIME*t[0] < t[1]);
  }

  SECTION("Test tube4(05)")
  {
    Tube tube = tube_test4_05();
    tube.enable_synthesis(SynthesisMode::BINARY_TREE); // first: testing with tree synthesis

    double t[2];
    clock_t t_start;

    for(int i = 0 ; i < 2 ; i++)
    {
      t_start = clock();
      CHECK(ApproxIntv(tube.integral(Interval(0,8), Interval(15,21))) == Interval(-0.5,36.5));
      CHECK(ApproxIntv(tube.integral(Interval(0,8), Interval(9,21))) == Interval(-2,36.5));
      t[i] = (double)(clock() - t_start)/CLOCKS_PER_SEC;

      tube.enable_synthesis(SynthesisMode::NONE); // second: without tree synthesis
    }

    if(TEST_COMPUTATION_TIMES) CHECK(COEFF_COMPUTATION_TIME*t[0] < t[1]);
  }
}

TEST_CASE("Computing partial integration, two interval bounds", "[core]")
{
  SECTION("Test tube1")
  {
    Tube tube = tube_test_1();
    tube.set(Interval(-4,2), 14); // to test primitives pre-computation
    tube.enable_synthesis(SynthesisMode::BINARY_TREE); // first: testing with tree synthesis

    double t[2];
    clock_t t_start;

    for(int i = 0 ; i < 2 ; i++)
    {
      t_start = clock();
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(0), Interval(0))) == make_pair(Interval(0.), Interval(0.)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(20), Interval(20))) == make_pair(Interval(0.), Interval(0.)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(22,23), Interval(24,25))) == make_pair(Interval(8,24), Interval(13,37)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(17,20), Interval(28,33))) == make_pair(Interval(51,80), Interval(88,143)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(0.), Interval(7.))) == make_pair(Interval(-23), Interval(13)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(2.), Interval(6.))) == make_pair(Interval(-19), Interval(4)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(0,2), Interval(6,7))) == make_pair(Interval(-29,-13), Interval(-2,19)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(10,13), Interval(17,20))) == make_pair(Interval(-32,1), Interval(0,35)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(0.), Interval(46.))) == make_pair(Interval(-3), Interval(194)));
      t[i] = (double)(clock() - t_start)/CLOCKS_PER_SEC;

      tube.enable_synthesis(SynthesisMode::NONE); // second: without tree synthesis
    }

    if(TEST_COMPUTATION_TIMES) CHECK(COEFF_COMPUTATION_TIME*t[0] < t[1]);
  }

  SECTION("Test tube1(01)")
  {
    Tube tube = tube_test_1_01();
    tube.enable_synthesis(SynthesisMode::BINARY_TREE); // first: testing with tree synthesis

    double t[2];
    clock_t t_start;

    for(int i = 0 ; i < 2 ; i++)
    {
      t_start = clock();
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(0), Interval(0))) == make_pair(Interval(0.), Interval(0.)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(20), Interval(20))) == make_pair(Interval(0.), Interval(0.)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(22,23), Interval(24,25))) == make_pair(Interval(8,24), Interval(13,37)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(17,20), Interval(28,33))) == make_pair(Interval(51,80), Interval(88,143)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(0.), Interval(7.))) == make_pair(Interval(-23), Interval(13)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(2.), Interval(6.))) == make_pair(Interval(-19), Interval(4)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(0,2), Interval(6,7))) == make_pair(Interval(-29,-13), Interval(-2,19)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(10,13), Interval(17,20))) == make_pair(Interval(-32,1), Interval(0,35)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(0.), Interval(46.))) == make_pair(Interval(-3), Interval(194)));
      t[i] = (double)(clock() - t_start)/CLOCKS_PER_SEC;

      tube.enable_synthesis(SynthesisMode::NONE); // second: without tree synthesis
    }

    if(TEST_COMPUTATION_TIMES) CHECK(COEFF_COMPUTATION_TIME*t[0] < t[1]);
  }

  SECTION("Test tube4")
  {
    Tube tube = tube_test4();
    tube.set(Interval(-1,1), Interval(10,11));
    tube.enable_synthesis(SynthesisMode::BINARY_TREE); // first: testing with tree synthesis

    double t[2];
    clock_t t_start;

    for(int i = 0 ; i < 2 ; i++)
    {
      t_start = clock();
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(0.1,1.1), Interval(2.6,3.2))) == make_pair(Interval(1.5,3.1), Interval(3.0,6.2)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(8.6,9.9), Interval(13.2,13.6))) == make_pair(Interval(-3.35,-2.3), Interval(1.95,4.7)));
      t[i] = (double)(clock() - t_start)/CLOCKS_PER_SEC;

      tube.enable_synthesis(SynthesisMode::NONE); // second: without tree synthesis
    }

    if(TEST_COMPUTATION_TIMES) CHECK(COEFF_COMPUTATION_TIME*t[0] < t[1]);
  }

  SECTION("Test tube4(05)")
  {
    Tube tube = tube_test4_05();
    tube.enable_synthesis(SynthesisMode::BINARY_TREE); // first: testing with tree synthesis

    double t[2];
    clock_t t_start;

    for(int i = 0 ; i < 2 ; i++)
    {
      t_start = clock();
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(0.1,1.1), Interval(2.6,3.2))) == make_pair(Interval(1.5,3.1), Interval(3.0,6.2)));
      CHECK(ApproxIntvPair(tube.partial_integral(Interval(8.6,9.9), Interval(13.2,13.6))) == make_pair(Interval(-3.35,-2.3), Interval(1.95,4.7)));
      t[i] = (double)(clock() - t_start)/CLOCKS_PER_SEC;

      tube.enable_synthesis(SynthesisMode::NONE); // second: without tree synthesis
    }

    if(TEST_COMPUTATION_TIMES) CHECK(COEFF_COMPUTATION_TIME*t[0] < t[1]);
  }

  SECTION("Test heavy integral computation")
  {
    double t[2];
    clock_t t_start;

    Tube tube(Interval(0.,10.), 0.1, TFunction("sin(t)"));

    tube.enable_synthesis(SynthesisMode::BINARY_TREE); // first: testing with tree synthesis
    Interval intv_t = Interval(tube.tdomain().lb() + tube.tdomain().diam() / 4.,
                               tube.tdomain().ub() - tube.tdomain().diam() / 4.);
    intv_t = tube.tdomain();

    t_start = clock();
    tube.integral(intv_t);
    t[0] = (double)(clock() - t_start)/CLOCKS_PER_SEC;

    tube.enable_synthesis(SynthesisMode::NONE); // second: without tree synthesis

    t_start = clock();
    tube.integral(intv_t);
    t[1] = (double)(clock() - t_start)/CLOCKS_PER_SEC;

    if(TEST_COMPUTATION_TIMES) CHECK(COEFF_COMPUTATION_TIME*t[0] < t[1]);
  }
}