#include "catch_interval.hpp"
#include "tubex_CtcDelay.h"
#include "tubex_capd2tubex.h"

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
    auto j = R"(
    {
      "config":
      {
        "function":"var:x,y;fun: x^3+x*y^2-x+y, y^3+x^2*y-x-y;",
        "fixed_step": true,
        "integration_time": 5,
        "integration_step": 0.001,
        "nb_fixed_parameters":0,
        "parameters_name":[],
        "parameters_value":[],
        "dimensions": 2,
        "initial_coordinates": [[0.5,0.5],[0,0]],

        "output_file": "reference_output.txt",
        "tubex_output": "tube.tube"
      }
    }
    )"_json;

    vector<IntervalVector> ibex_curve = capd2ibex(j);
    TubeVector output = capd2tubex(j);
    IntervalVector a1 = output(1.0);
    IntervalVector expected(2);
    expected[0] = Interval(0.1121125007098844, 0.1123948125529081);
    expected[1] = Interval(-0.1748521323811479, -0.1747971075720925);
    REQUIRE(ApproxIntvVector(a1) == expected);
  }
}