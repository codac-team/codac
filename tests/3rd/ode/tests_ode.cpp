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
        Interval domain(0.0,5.0);
        TFunction f("x","y", "(x^3+x*y^2-x+y; y^3+x^2*y-x-y)");
        IntervalVector x0(2);
        x0[0]=Interval(0.5,0.5);
        x0[1]=Interval(0,0);
        double timestep = 0.001;
        int mode = CAPD_MODE;
        TubeVector output = TubeVectorODE(domain,f,x0,timestep,mode);
        IntervalVector a_end = output(5.0);
        IntervalVector expected(2);
        expected[0] = Interval(0.00110085102557442, 0.001103485435051887);
        expected[1] = Interval(0.003730335202989557, 0.00373517121588826);
        REQUIRE(ApproxIntvVector(a_end) == expected);

    }
}