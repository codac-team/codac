/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_OctaSym.h>
#include <codac2_AnalyticFunction.h>
#include <codac2_peibos.h>
#include <codac2_Approx.h>

using namespace std;
using namespace codac2;

TEST_CASE("Peibos")
{
  
  // 2D checks of the PEIBOS algorithm

  VectorVar y_2d(2);
  AnalyticFunction f_2d({y_2d},{sqr(y_2d[0])-sqr(y_2d[1])+y_2d[0],
                             2.*y_2d[0]*y_2d[1] + y_2d[1]});

  VectorVar X_2d(1);
  AnalyticFunction psi0_2d ({X_2d},{cos(X_2d[0]*PI/4.),sin(X_2d[0]*PI/4.)});

  OctaSym id_2d ({1,2});
  OctaSym s ({-2,1});

  auto v_par_2d = PEIBOS(f_2d, psi0_2d, {id_2d,s,s*s,s.invert()}, 0.25, {-0.2,0.});

  Vector b0 ({-0.5,0.});
  Vector b1 ({0.,1.45});
  Vector b2 ({-1.165,0.});

  int count_b0 = 0; int count_b1 = 0; int count_b2 = 0;

  for (const auto& p : v_par_2d)
  {
    if ((p.contains(b0))==BoolInterval::TRUE)
      count_b0++;
    if ((p.contains(b1))==BoolInterval::TRUE)
      count_b1++;
    if ((p.contains(b2))==BoolInterval::TRUE)
      count_b2++;
  }

  CHECK(count_b0 == 0);
  CHECK(count_b1 == 1);
  CHECK(count_b2 == 2);

  // 3D checks of the PEIBOS algorithm

  VectorVar y_3d(3);
  AnalyticFunction f_3d({y_3d},{y_3d[0],y_3d[1],y_3d[2]});

  VectorVar X_3d(2);
  AnalyticFunction psi0_3d ({X_3d},{1/sqrt(1+sqr(X_3d[0])+sqr(X_3d[1])),X_3d[0]/sqrt(1+sqr(X_3d[0])+sqr(X_3d[1])),X_3d[1]/sqrt(1+sqr(X_3d[0])+sqr(X_3d[1]))});

  OctaSym id_3d ({1,2,3});
  OctaSym s1 ({-2,1,3});
  OctaSym s2 ({3,2,-1});

  auto v_par_3d = PEIBOS(f_3d, psi0_3d, {id_3d,s1,s1*s1,s1.invert(),s2,s2.invert()}, 2.); 

  CHECK(v_par_3d.size() == 6);

  CHECK(Approx(v_par_3d[0].z,1e-6) == Vector({1.,0.,0.})); 
  CHECK(Approx(v_par_3d[1].z,1e-6) == Vector({0.,1.,0.}));
  CHECK(Approx(v_par_3d[2].z,1e-6) == Vector({-1.,0.,0.}));
  CHECK(Approx(v_par_3d[3].z,1e-6) == Vector({0.,-1.,0.}));
  CHECK(Approx(v_par_3d[4].z,1e-6) == Vector({0.,0.,-1.}));
  CHECK(Approx(v_par_3d[5].z,1e-6) == Vector({0.,0.,1.}));

  double a = 4.35066;

  CHECK(Approx(v_par_3d[0].A,1e-5) == Matrix({{0.,0.,a},{a+1,0.,0.},{0.,a+1,0.}}));
  CHECK(Approx(v_par_3d[1].A,1e-5) == Matrix({{-(a+1),0.,0.},{0.,0.,a},{0.,a+1,0.}}));
  CHECK(Approx(v_par_3d[2].A,1e-5) == Matrix({{0.,0.,a},{-(a+1),0.,0.},{0.,a+1,0.}}));
  CHECK(Approx(v_par_3d[3].A,1e-5) == Matrix({{a+1,0.,0.},{0.,0.,a},{0.,a+1,0.}}));
  CHECK(Approx(v_par_3d[4].A,1e-5) == Matrix({{0.,a+1,0.},{a+1,0.,0.},{0.,0.,a}}));
  CHECK(Approx(v_par_3d[5].A,1e-5) == Matrix({{0.,-(a+1),0.},{a+1,0.,0.},{0.,0.,a}}));
}