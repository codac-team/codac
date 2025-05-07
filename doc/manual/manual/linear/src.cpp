/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Damien Massé
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_inversion.h>

using namespace std;
using namespace codac2;

TEST_CASE("linear - manual")
{
  {
    // [linear-1-beg]
    Matrix A({
      { 1, 2, 0 },
      { 3, 4, 1 },
      { 0, 1, 0 },
    });

    IntervalMatrix B = inverse_enclosure(A);
    // B == [[     <1, 1> ,    <-0, 0> ,        <-2, -2> ]
    //       [    <-0, 0> ,    <-0, 0> , [0.9999, 1.001] ]
    //       [   <-3, -3> ,     <1, 1> ,  [1.999, 2.001] ]]

    bool i = (A.template cast<Interval>()*B).contains(Matrix::eye(3,3));
    // i == true
    // [linear-1-end]

    CHECK((A.template cast<Interval>()*B).contains(Matrix::eye(3,3)));
    CHECK(i);
  }
}