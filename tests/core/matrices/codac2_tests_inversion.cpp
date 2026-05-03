/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Damien Massé
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_Vector.h>
#include <codac2_Matrix.h>
#include <codac2_IntervalMatrix.h>
#include <codac2_inversion.h>
#include <codac2_Approx.h>

#include <iostream>

using namespace std;
using namespace codac2;

TEST_CASE("Square matrices")
{
  IntervalMatrix x({
  	{ {0.0,0.0}, {-0.1,-0.1}, {0.2,0.2}  },
  	{ {0.0,0.0}, {-0.2,-0.2}, {0.1,0.1} },
  	{ {0.1,0.1}, {-0.1,-0.1}, {0.1,0.1} }
  });

  double a=0.0;

  IntervalMatrix y = infinite_sum_enclosure(x,a);
  CHECK(Approx(Interval(a))==Interval(5.0)/Interval(2.0));

  IntervalMatrix z = inverse_enclosure(x);
  CHECK(z(1,2)==Interval(0));
  CHECK(Approx<IntervalMatrix>(z*x)==IntervalMatrix::Identity(3,3));

  Matrix u( {
     { 1,2,3 },
     { 1,3,5 },
     { 3,4,5 }
   });

   IntervalMatrix v=inverse_enclosure(u);
  CHECK((u.template cast<Interval>()*v).contains(Matrix::Identity(3,3)));

  Matrix w({
    { 1, 2, 0 },
    { 3, 4, 1 },
    { 0, 1, 0 },
  });

  y = inverse_enclosure(w);
  CHECK((w.template cast<Interval>()*y).contains(Matrix::Identity(3,3)));
}

TEST_CASE("Non-square matrices")
{
  IntervalMatrix x({
  	{ {0.0,0.0}, {-0.1,-0.1}, {0.2,0.2}, {0.1,0.1}  },
  	{ {0.0,0.0}, {-0.2,-0.2}, {0.1,0.1}, {0.3,0.3} },
  	{ {0.1,0.1}, {-0.1,-0.1}, {0.1,0.1}, {-0.1,-0.1} }
  });

  IntervalMatrix z = right_inverse_enclosure(x);
  CHECK(Approx<IntervalMatrix>(x*z,1e-8)==IntervalMatrix::Identity(3,3));

  Matrix u( {
     { 1,2,3 },
     { 1,3,5 },
     { 2,6,10 },
     { 3,4,5 },
     { 2,1,3 },
     { 6,2,4 }
   });

  IntervalMatrix v=left_inverse_enclosure(u);
  CHECK((v*u.template cast<Interval>()).contains(Matrix::Identity(3,3)));

  Matrix w({
    { 1, 2, 0, 3, 1, 5 },
    { 0, 4, 1, 8, 2, 4 },
    { 0, 1, 0, 4, 5, 1 }
  });

  IntervalMatrix y = right_inverse_enclosure(w);
  CHECK((w.template cast<Interval>()*y).contains(Matrix::Identity(3,3)));
}
