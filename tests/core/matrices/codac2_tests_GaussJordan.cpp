/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Damien Massé
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_GaussJordan.h>
#include <codac2_BoolInterval.h>
#include <codac2_IntervalVector.h>
#include <iostream>

using namespace std;
using namespace codac2;

TEST_CASE("GaussJordan")
{
  /* rectangular matrix, full rank */
  {
    Matrix M 
    { { 1, -4, 6, 7, 6 },
      { 2,  1, 3, 6, -2 },
      { 5,  2, 2, 9, -1 } };
    Matrix pre = gauss_jordan(M);
    /* check that pre*M is "band matrix" */
    Matrix P = pre*M;
    double mx = 0.0;
    for (Index i=1;i<M.rows();i++) 
      for (Index j=0;j<i;j++) {
        mx = std::max(mx,std::abs(P(i,j)));
        mx = std::max(mx,std::abs(P(M.rows()-1-i,M.cols()-1-j)));
    }
    CHECK(mx<1e-10);
  }
  
}
