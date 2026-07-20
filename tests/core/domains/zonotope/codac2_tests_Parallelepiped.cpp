/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_Approx.h>
#include <codac2_Parallelepiped.h>

using namespace std;
using namespace codac2;

TEST_CASE("Parallelepiped")
{
  Parallelepiped p(Vector({0,2,4}), Matrix({{0.5,0.,0.},{0.,1.,0.},{0.,1.,1.}}));
  
  CHECK(p.box() == IntervalVector({{-0.5,0.5},{1.,3.},{2.,6.}}));
  CHECK((p.contains(Vector({0.1,2.1,4.1})))==BoolInterval::TRUE);
  CHECK((p.contains(Vector({20.,20.,20.})))==BoolInterval::FALSE);

  CHECK((p.is_superset(IntervalVector({{0.,0.1},{2.,2.1},{4.,4.1}})))==BoolInterval::TRUE);
  CHECK((p.is_superset(IntervalVector({{-10.,-9.},{15.,16.},{12.,13.}})))==BoolInterval::FALSE);
  CHECK((p.is_superset(IntervalVector({{0.,5.},{2.,7.},{4.,9.}})))==BoolInterval::UNKNOWN);

  Zonotope z = p.proj({2,1,0});
  CHECK(z.c == Vector({4,2,0}));
  CHECK(z.A == Matrix({{0.,1.,1.},{0.,1.,0.},{0.5,0.,0.}}));
  CHECK(z.box() == IntervalVector({{2.,6.},{1.,3.},{-0.5,0.5}}));

  Parallelepiped p1 ({1.5,2.8},Matrix({{0.5,0.4},{0,0.2}}));

  IntervalVector x0 ({{0,5},{0,5}});
  IntervalVector x1 ({{0,0.5},{0,0.5}});
  IntervalVector x2 ({{1.7,5},{0,3}});

  CHECK(Approx(x0 & p1) == IntervalVector({{0.6,2.4},{2.6,3}}));
  CHECK(Approx((p1 & x0).c) == p1.c);
  CHECK(Approx((p1 & x0).A) == p1.A);

  CHECK((x1 & p1).is_empty());
  CHECK((p1 & x1).is_empty());

  CHECK(Approx(x2 & p1) == IntervalVector({{1.7,2.4},{2.6,3}}));
  CHECK(Approx((p1 & x2).c) == Vector({1.65, 2.8}));
  CHECK(Approx((p1 & x2).A) == Matrix({{0.35,0.4},{0,0.2}}));

  Parallelepiped p2 ({-5,-4,3,8},Matrix({{0.5,0.4,0.1,0},{0,0.2,0.3,0},{0,0,0.1,0.4},{0,0,0,0.2}}));
  Parallelepiped p_empty = Parallelepiped::empty(4);

  IntervalVector x3 ({{-10,10},{-10,10},{-10,10},{-10,10}});
  IntervalVector x4 ({{-0.5,0.5},{-0.5,0.5},{-0.5,0.5},{-0.5,0.5}});
  IntervalVector x_empty = IntervalVector::empty(4);

  CHECK(Approx(x3 & p2) == IntervalVector({{-6,-4},{-4.5,-3.5},{2.5,3.5},{7.8,8.2}}));
  CHECK(Approx((p2 & x3).c) == Vector({-5,-4,3,8}));
  CHECK(Approx((p2 & x3).A) == Matrix({{0.5,0.4,0.1,0},{0,0.2,0.3,0},{0,0,0.1,0.4},{0,0,0,0.2}}));

  CHECK((x4 & p2).is_empty());
  CHECK((p2 & x4).is_empty());

  CHECK((x_empty & p2).is_empty());
  CHECK((p_empty & x3).is_empty());
}

