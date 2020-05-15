#include "catch_interval.hpp"
#include "tubex_CtcDelay.h"
#include "tubex_TubeVectorODE.h"

using namespace std;
using namespace Catch;
using namespace Detail;
using namespace ibex;
using namespace tubex;


TEST_CASE("TubeVectorODE_1")
{
    SECTION("CAPD_MODE")
    {
        Interval domain(0,5);
        TFunction f("x","y", "(x^3+x*y^2-x+y; y^3+x^2*y-x-y)");
        IntervalVector x0(2);
        x0[0]=Interval(0.5,0.5);
        x0[1]=Interval(0,0);
        double timestep = 0.001;
        int mode = CAPD_MODE;
        TubeVector output = TubeVectorODE(domain,f,x0,timestep,mode);
        IntervalVector a1 = output(1.0);
        IntervalVector expected(2);
        expected[0] = Interval(0.1121125007098844, 0.1123948125529081);
        expected[1] = Interval(-0.1748521323811479, -0.1747971075720925);
        REQUIRE(ApproxIntvVector(a1) == expected);

    }
}