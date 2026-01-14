/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_peibos.h>

using namespace std;
using namespace codac2;

TEST_CASE("tools - manual")
{
  {
    // [peibos-1-beg]
    // Definition of f
    VectorVar y_2d(2);
    double a = 1.4; double b = 0.3;
    AnalyticFunction f_2d({y_2d},{y_2d[1]+1-a*sqr(y_2d[0]),b*y_2d[0]});
    
    // Definition of the gnomonic atlas
    VectorVar X_2d(1);
    AnalyticFunction psi0_2d ({X_2d},{sin(X_2d[0]*PI/4.),cos(X_2d[0]*PI/4.)});
    OctaSym id_2d ({1,2});
    OctaSym s ({-2,1});
  
    // Call to PEIBOS
    auto v_par_2d = PEIBOS(f_2d, psi0_2d, {id_2d,s,s*s,s.invert()}, 0.2);
    // [peibos-1-end]

    // [peibos-2-beg]
    // Definition of f
    VectorVar y_3d(3);
    AnalyticFunction f_3d({y_3d},{sqr(y_3d[0])-sqr(y_3d[1])+y_3d[0],2*y_3d[0]*y_3d[1]+y_3d[1],y_3d[2]});
    
    // Definition of the gnomonic atlas
    VectorVar X_3d(2);
    AnalyticFunction psi0_3d ({X_3d},{1/sqrt(1+sqr(X_3d[0])+sqr(X_3d[1])),X_3d[0]/sqrt(1+sqr(X_3d[0])+sqr(X_3d[1])),X_3d[1]/sqrt(1+sqr(X_3d[0])+sqr(X_3d[1]))});
    OctaSym id_3d ({1,2,3});
    OctaSym s1 ({-2,1,3});
    OctaSym s2 ({3,2,-1});

    // Call to PEIBOS
    auto v_par_3d = PEIBOS(f_3d, psi0_3d, {id_3d,s1,s1*s1,s1.invert(),s2,s2.invert()}, 0.2);
    // [peibos-2-end]
  }
}