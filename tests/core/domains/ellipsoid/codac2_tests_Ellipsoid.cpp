/**
 *  Codac tests
 *
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Morgan Louedec, (Simon Rohou)
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_Ellipsoid.h>

using namespace std;
using namespace codac2;

Ellipsoid e1() {
    return Ellipsoid(Vector({1., 0.}), Matrix({{0.05, 0.0},
                                               {0.,   0.05}}));
}

Ellipsoid e4() {
    return Ellipsoid(Vector({1., 0., 0.}),
                     Matrix({{1.,  0.5, 0.},
                             {0.5, 2.,  0.2},
                             {0.,  0.2, 3.}}));
}

Ellipsoid e5() {
    return Ellipsoid(Vector({1., 0., 0.}),
                     Matrix({{0.7, 0.35, 0.},
                             {0.35, 1.4, 0.14},
                             {0.,  0.14, 2.1}}));
}

Ellipsoid e6(){
    return Ellipsoid(Vector({1., 0., 0.}),
                     Matrix({{2., 0.,  0.5},
                             {0., 1.,  0.2},
                             {0., 0.2, 3.}}));
}

TEST_CASE("Ellipsoids")
{
    Ellipsoid e = e1();
    CHECK(e.size() == 2);
    CHECK(e.mu == Vector({1., 0.}));
    CHECK(e.G == Matrix({{0.05, 0.0},
                         {0.,   0.05}}));

    VectorVar x(2);
    AnalyticFunction h {
            {x}, vec(x[0]+0.1*x[1],-0.2*sin(x[0])+0.9*x[1])};

    Matrix A = h.diff(e.mu).mid();
    Vector b(h.eval(e.mu).mid()._e - A._e*e.mu._e);
    Ellipsoid e2 = linear_mapping(e, A, b);
    Vector mu2({1 , -0.168294});
    Matrix G2({{0.0500001 , 0.00500001},{-0.00540302 , 0.0450001}});
    CHECK(e2.size() == 2);
    CHECK((e2.mu._e-mu2._e).norm() < 1e-6);
    CHECK((e2.G._e-G2._e).norm() < 1e-6);

    Ellipsoid e3 = nonlinear_mapping(e, h);
    Vector mu3({1. , -0.168294});
    Matrix G3({{0.0550724 , 0.00550724},{-0.00595114 , 0.0495651}});
    CHECK(e3.size() == 2);
    CHECK((e3.mu._e-mu3._e).norm() < 1e-6);
    CHECK((e3.G._e-G3._e).norm() < 1e-6);

    Ellipsoid e4_ = e4();
    Ellipsoid e5_ = e5();
    Ellipsoid e6_ = e6();

    bool res1 = concentric_inclusion_test(e5_, e4_);
    bool res2 = concentric_inclusion_test(e4_, e5_);
    bool res3 = concentric_inclusion_test(e6_, e4_);
    bool res4 = concentric_inclusion_test(e5_, e6_);
    CHECK(res1 == true);
    CHECK(res2 == false);
    CHECK(res3 == false);
    CHECK(res4 == false);

    Matrix A2({{0.9,0.5},{-.5,1.1}});
    Vector b2({0.1,0.2});

    Ellipsoid e7 = linear_mapping(e, A2, b2);
    Ellipsoid e8 = linear_mapping_guaranteed(e, A2, b2);
    CHECK((e7.mu._e - e8.mu._e).norm()<1e-6);
    CHECK((e7.G._e - e8.G._e).norm()<0.04);


}