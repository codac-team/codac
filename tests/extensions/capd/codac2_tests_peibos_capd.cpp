/**
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_AnalyticFunction.h>
#include <codac2_peibos_capd.h>

using namespace std;
using namespace codac2;

TEST_CASE("PEIBOS_CAPD")
{
  {
    capd::IMap vectorField_pend("var:t,w;fun:w,-sin(t) - 0.5*w;");
    VectorVar X(1);
    AnalyticFunction psi0_pend ({X},{0.1*X[0],0.1});
    auto peibos_output_pend = PEIBOS(vectorField_pend, 1.0, 0.5, psi0_pend, {OctaSym({1,2})}, 0.2, {-M_PI/2.,0.});
    auto m_v_par_2d_pend = reach_set(peibos_output_pend);
  }
}