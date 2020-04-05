#define CATCH_CONFIG_MAIN
#include "tubex_Capd2Tubex.h"
#include "json.hpp"
#include "catch/catch.hpp"
#include "catch/catch_interval.hpp"


using namespace std;
using namespace Catch;
using namespace Detail;
using namespace ibex;
using namespace tubex;
using json = nlohmann::json;


TEST_CASE("Capd2Tubex_1")
{

    SECTION("Test tube creation")
    {
        ifstream i("parameters/travis_test.json");
        json j;
        i >> j;
        vector<IntervalVector> ibex_curve = capd2ibex(j);
        TubeVector output = capd2tubex(j);
        IntervalVector a1 = output(1.0);
        IntervalVector expected(2);
        expected[0] = Interval(0.1121125007098844, 0.1123948125529081);
        expected[1] = Interval(-0.1748521323811479, -0.1747971075720925);
        REQUIRE( ApproxIntvVector(a1) == expected);
    }

}

