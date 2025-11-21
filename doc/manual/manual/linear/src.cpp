/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Damien Massé
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <map>
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

  {
    // [vector-map-compare-1-beg]
    // std::map<codac2::Vector, int> m; // does not compile
    std::map<codac2::Vector, int, codac2::VectorCompare> m;
    codac2::Vector v1({1,3}), v2({-42,10});
    m[v1] = 10;
    m[v2] = 20;
    // [vector-map-compare-1-end]
  }
}