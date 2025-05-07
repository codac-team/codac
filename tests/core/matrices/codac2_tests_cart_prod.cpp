/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_cart_prod.h>
#include <codac2_Vector.h>
#include <codac2_Matrix.h>
#include <codac2_IntervalVector.h>
#include <codac2_IntervalMatrix.h>

using namespace std;
using namespace codac2;

TEST_CASE("type traits")
{
  CHECK(is_interval_based_v<int> == false);
  CHECK(is_interval_based_v<Index> == false);
  CHECK(is_interval_based_v<size_t> == false);
  CHECK(is_interval_based_v<float> == false);
  CHECK(is_interval_based_v<double> == false);
  CHECK(is_interval_based_v<Vector> == false);
  CHECK(is_interval_based_v<Matrix> == false);

  CHECK(is_interval_based_v<Interval> == true);
  CHECK(is_interval_based_v<IntervalVector> == true);
  CHECK(is_interval_based_v<IntervalMatrix> == true);
}

TEST_CASE("cart_prod Vector")
{
  CHECK(cart_prod(1,2,3.5) == Vector({1,2,3.5}));
  CHECK(cart_prod(1,2,3.5,Vector::ones(2)) == Vector({1,2,3.5,1,1}));
  CHECK(cart_prod(Vector::ones(2),1,2,3.5) == Vector({1,1,1,2,3.5}));
  CHECK(cart_prod(Vector::zero(3)) == Vector::zero(3));
  CHECK(cart_prod(Vector::zero(3),Vector::ones(4)) == Vector({0,0,0,1,1,1,1}));
}

TEST_CASE("cart_prod IntervalVector")
{
  CHECK(cart_prod(IntervalVector({{0,1},{2,3},{4,5}}),IntervalVector::empty(3)).is_empty());
  CHECK(cart_prod(IntervalVector::empty(3),IntervalVector({{0,1},{2,3},{4,5}})).is_empty());
  CHECK(cart_prod(IntervalVector({{0,1},{2,3},{4,5}})) == IntervalVector({{0,1},{2,3},{4,5}}));
  CHECK(cart_prod(IntervalVector::empty(3)) == IntervalVector::empty(3));
  CHECK(cart_prod(IntervalVector({{0,1},{2,3},{4,5}}),IntervalVector({{8,9}})) == IntervalVector({{0,1},{2,3},{4,5},{8,9}}));
  CHECK(cart_prod(25.,IntervalVector({{0,1},{2,3},{4,5}}),IntervalVector({{8,9}}),Vector::ones(3)) == IntervalVector({{25},{0,1},{2,3},{4,5},{8,9},{1},{1},{1}}));
}