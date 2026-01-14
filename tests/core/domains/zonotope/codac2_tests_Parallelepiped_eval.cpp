/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_AnalyticFunction.h>
#include <codac2_Approx.h>

using namespace std;
using namespace codac2;

TEST_CASE("Parallelepiped_eval")
{
  ScalarVar x1;
  ScalarVar x2;
  VectorVar x(2);

  AnalyticFunction f1({x1},{x1,sqr(x1)});

  AnalyticFunction f2({x1,x2}, {x1, x2, sqr(x1)+sqr(x2)});
  AnalyticFunction f3({x}, {x[0], x[1], sqr(x[0])+sqr(x[1])});

  auto p1a = f1.parallelepiped_eval(Interval(-0.1,0.1));
  auto p1b = f1.parallelepiped_eval(1.0);

  CHECK(Approx(p1a.z,1e-6) == Vector({0,0}));
  CHECK(Approx(p1a.A,1e-6) == Matrix({{0.12,0},{0,0.02}}));
  CHECK(Approx(p1b.z,1e-6) == Vector({1,1}));
  CHECK(Approx(p1b.A,1e-6) == Matrix({{0,0},{0,0}}));

  auto pa = f2.parallelepiped_eval(Interval(-0.1,0.1), Interval(-0.1,0.1));
  auto pb = f2.parallelepiped_eval(1.0, Interval(-1,1));

  CHECK(Approx(pa.z,1e-6) == Vector({0,0,0}));
  CHECK(Approx(pa.A,1e-6) == Matrix({{0.14,0,0},{0,0.14,0},{0,0,0.04}}));
  CHECK(Approx(pb.z,1e-6) == Vector({1,0,1}));
  CHECK(Approx(pb.A,1e-5) == Matrix({{0.894428,0,1.78886},{0,3,0},{1.78886,0,-0.894427}}));


  double dx = 0.4;
  double x0 = -2.0;
  while (x0<2.0)
  {
      Interval X0(x0, x0+dx);
      double y0=-2.0;
      while (y0<2.0)
      {
        Interval Y0(y0, y0+dx);
        
        auto p2 = f2.parallelepiped_eval(X0,Y0);
        auto p3 = f3.parallelepiped_eval(IntervalVector({X0,Y0}));

        CHECK(Approx(p2.z,1e-6) == p3.z);
        CHECK(Approx(p2.A,1e-6) == p3.A);
        y0+=dx;
      }
      x0+=dx;
  }

}

