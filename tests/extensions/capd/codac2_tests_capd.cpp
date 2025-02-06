/**
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_capd.h>

using namespace std;
using namespace codac2;

TEST_CASE("CAPD")
{
  {
    Interval codac_interval(0., 2.);
    capd::Interval capd_interval=to_capd(codac_interval);
    CHECK(capd_interval == capd::Interval(0., 2.));
    CHECK(to_codac(capd_interval) == Interval(0., 2.));
  }

  {
    IntervalVector codac_interval_vector({Interval(0., 2.), Interval(1., 3.)});
    capd::IVector capd_interval_vector=to_capd(codac_interval_vector);
    CHECK(capd_interval_vector == capd::IVector({capd::Interval(0., 2.), capd::Interval(1., 3.)}));
    CHECK(to_codac(capd_interval_vector) == IntervalVector({Interval(0., 2.), Interval(1., 3.)}));
  }

  {
    IntervalMatrix codac_interval_matrix({{Interval(0., 2.), Interval(1., 3.)}, {Interval(2., 4.), Interval(3., 5.)}});
    capd::IMatrix capd_interval_matrix=to_capd(codac_interval_matrix);
    CHECK(capd_interval_matrix == capd::IMatrix({{capd::Interval(0., 2.), capd::Interval(1., 3.)}, {capd::Interval(2., 4.), capd::Interval(3., 5.)}}));
    CHECK(to_codac(capd_interval_matrix) == IntervalMatrix({{Interval(0., 2.), Interval(1., 3.)}, {Interval(2., 4.), Interval(3., 5.)}}));
  }
}