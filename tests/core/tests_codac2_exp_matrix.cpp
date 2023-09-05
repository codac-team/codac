#include "catch_interval.hpp"
#include "vibes.h"

// Using #define so that we can access protected methods
// of the class for tests purposes
#define protected public
#include "codac2_TDomain.h"

#include "codac_predef_values.h"
#include "codac2_expIMat.h"

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace ibex;
using namespace codac2;
using codac::oo;


TEST_CASE("Test codac2::exp_matrix")
{
  SECTION("Test Utilities")
  {
    Interval I1(-1.0,2.0);
    CHECK(quad_I(I1,1.0) == Interval(-0.25,6.0));
    Interval I2(-2.0,-1.0);
    CHECK(quad_II(I1,I2) == Interval(-1.0,3.0));
    
    IntervalMatrix M1(2,2);
    M1[0][0]=Interval(-1.0,2.0); M1[0][1]=Interval(-1.0,0.0);
    M1[1][0]=Interval(-2.0,1.0); M1[1][1]=Interval(-3.0,1.0);
    IntervalMatrix MRes = square_IntervalMatrix(M1);
    CHECK(MRes[0][0] == Interval(-1.0,6.0));
    CHECK(MRes[0][1] == Interval(-3.0,4.0));
    CHECK(MRes[1][0] == Interval(-6.0,8.0));
    CHECK(MRes[1][1] == Interval(-1.0,11.0));
    MRes = quad_M(M1,1.0);
    CHECK(MRes[0][0] == Interval(-1.25,8.0));
    CHECK(MRes[0][1] == Interval(-4.0,3.0));
    CHECK(MRes[1][0] == Interval(-8.0,6.0));
    CHECK(MRes[1][1] == Interval(-1.25,8.0));
    MRes = quad_Mtau(M1,3.0);
    CHECK(MRes[0][0] == Interval(-3,12.0));
    CHECK(MRes[0][1] == Interval(-6.0,1.0));
    CHECK(MRes[1][0] == Interval(-12.0,6.0));
    CHECK(MRes[1][1] == Interval(-9.0,6.0));
    IntervalMatrix M2(2,2);
    M2[0][0]=Interval(0.0,1.0); M2[0][1]=Interval(1.0,2.0);
    M2[1][0]=Interval(-1.0,1.0); M2[1][1]=Interval(0.0,2.0);
    MRes = quad_MM(M1,M2);
    CHECK(MRes[0][0] == Interval(-2.25,8.0));
    CHECK(MRes[0][1] == Interval(-4.0,2.0));
    CHECK(MRes[1][0] == Interval(-9.0,9.0));
    CHECK(MRes[1][1] == Interval(-3.0,11.0));
    MRes = quad_MMtau(M1,M2);
    CHECK(MRes[0][0] == Interval(-2.25,8.0));
    CHECK(MRes[0][1] == Interval(-4.0,2.0));
    CHECK(MRes[1][0] == Interval(-9.0,9.0));
    CHECK(MRes[1][1] == Interval(-7.0,11.0));
    MRes = quad_prod(M1,M2,1.0,0.5,-0.5,true);
    CHECK(MRes[0][0] == Interval(-2.5,3.0));
    CHECK(MRes[0][1] == Interval(-4.0,0.0));
    CHECK(MRes[1][0] == Interval(-5.0,3.0));
    CHECK(MRes[1][1] == Interval(-7.5,1.5));

    M1[0][0]=Interval(-1.1,-1.0); M1[0][1]=Interval(-2.1,-2.0);
    M1[1][0]=Interval(0.3,0.4); M1[1][1]=Interval(-0.1,0.1);
    MRes = power_IntervalMatrix(M1,7);
    CHECK(MRes[0][0].is_superset(Interval(-0.931974,0.0223009)));
    CHECK(MRes[0][1].is_superset(Interval(-0.996794,0.759539)));
    CHECK(MRes[1][0].is_superset(Interval(-0.113931,0.189866)));
    CHECK(MRes[1][1].is_superset(Interval(-0.759287,-0.0297359)));

    MRes[0][0]=Interval(-0.1,-0.05); MRes[0][1]=Interval(0.01,0.02);
    MRes[1][0]=Interval(-0.2,-0.1); MRes[1][1]=Interval(-0.01,0.01);
    squaring_IntervalMatrix(MRes,3);
    CHECK(MRes[0][0].is_superset(Interval(-0.645166,-0.358930)));
    CHECK(MRes[0][1].is_superset(Interval(0.0538516,0.137787)));
    CHECK(MRes[1][0].is_superset(Interval(-1.37787,-0.538516)));
    CHECK(MRes[1][1].is_superset(Interval(-0.17359,0.0588608)));

    MRes[0][0]=Interval(-0.1,-0.05); MRes[0][1]=Interval(0.01,0.02);
    MRes[1][0]=Interval(-0.2,-0.1); MRes[1][1]=Interval(-0.01,0.01);
    M1[0][0]=Interval(-1.1,-1.0); M1[0][1]=Interval(-2.1,-2.0);
    M1[1][0]=Interval(0.3,0.4); M1[1][1]=Interval(-0.1,0.1);
    primSquaring_IntervalMatrix(MRes,M1,3);
    CHECK(MRes[0][0].is_superset(Interval(-0.45513,-0.0881056)));
    CHECK(MRes[0][1].is_superset(Interval(0.0201222,0.0901534)));
    CHECK(MRes[1][0].is_superset(Interval(-0.20642,-0.0700366)));
    CHECK(MRes[1][1].is_superset(Interval(-0.0205162,0.0102563)));
  }
    
  SECTION("Test Inversions")
  {
    Matrix Mc(3,3);
    Mc[0][0] = -1.0; Mc[0][1] = -0.6;  Mc[0][2]= 2.3;
    Mc[1][0] = 1.0;  Mc[1][1] = -2.2;  Mc[1][2]= -0.5;
    Mc[2][0] = 0.1;  Mc[2][1] = 0.0;   Mc[2][2]= 1.3;
    Matrix Mr(3,3);
    Mr[0][0] = -1.0; Mr[0][1] = -0.6;  Mr[0][2]= 2.3;
    Mr[1][0] = 1.0;  Mr[1][1] = -2.2;  Mr[1][2]= -0.5;
    Mr[2][0] = 0.1;  Mr[2][1] = 0.0;   Mr[2][2]= 1.3;
    bool res = inv_Matrix(Mc,Mr);
    CHECK(res);
    CHECK(fabs(Mr[0][0]-1.0)<1e-7); CHECK(fabs(Mr[0][1])<1e-7); CHECK(fabs(Mr[0][2])<1e-7);
    CHECK(fabs(Mr[1][0])<1e-7); CHECK(fabs(Mr[1][1]-1.0)<1e-7); CHECK(fabs(Mr[1][2])<1e-7);
    CHECK(fabs(Mr[2][0])<1e-7); CHECK(fabs(Mr[2][1])<1e-7); CHECK(fabs(Mr[2][2]-1.0)<1e-7);
    
    IntervalMatrix M1(3,3);
    M1[0][0]=Interval(-1.0,-0.9); M1[0][1]=Interval(-0.9,-0.6); M1[0][2]=Interval(0.2,0.3);
    M1[1][0]=Interval(-1.1,-1.0); M1[1][1]=Interval(0.4,0.7); M1[1][2]=Interval(-1.2,-1.1);
    M1[2][0]=Interval(-0.1,0.1); M1[2][1]=Interval(0.5,0.6); M1[2][2]=Interval(0.6,1.0);

    IntervalMatrix MRes = inv_IntervalMatrix(M1);
    CHECK(MRes[0][0].is_superset(Interval(-0.78297,-0.436536)));
    CHECK(MRes[0][1].is_superset(Interval(-0.575230,-0.279090)));
    CHECK(MRes[0][2].is_superset(Interval(-0.759680,-0.199420)));
    CHECK(MRes[1][0].is_superset(Interval(-0.712737,-0.273331)));
    CHECK(MRes[1][1].is_superset(Interval(0.269088,0.639116)));
    CHECK(MRes[1][2].is_superset(Interval(0.50032,1.15333)));
    CHECK(MRes[2][0].is_superset(Interval(0.178140,0.543293)));
    CHECK(MRes[2][1].is_superset(Interval(-0.457124,-0.161029)));
    CHECK(MRes[2][2].is_superset(Interval(0.511416,1.08404)));

    MRes = M1;
    inv_IntervalMatrix(M1,MRes);

    CHECK(MRes[0][0].is_superset(Interval(0.794873,1.25993)));
    CHECK(MRes[0][1].is_superset(Interval(-0.412584,0.358581)));
    CHECK(MRes[0][2].is_superset(Interval(-0.418668,0.290761)));
    CHECK(MRes[1][0].is_superset(Interval(-0.306873,0.288394)));
    CHECK(MRes[1][1].is_superset(Interval(0.693366,1.4425)));
    CHECK(MRes[1][2].is_superset(Interval(-0.43752,0.540532)));
    CHECK(MRes[2][0].is_superset(Interval(-0.279459,0.242863)));
    CHECK(MRes[2][1].is_superset(Interval(-0.349762,0.300642)));
    CHECK(MRes[2][2].is_superset(Interval(0.6707,1.49615)));


    M1[0][0]=Interval(-1.0,-0.95); M1[0][1]=Interval(-0.9,-0.8); M1[0][2]=Interval(0.2,0.3);
    M1[1][0]=Interval(-1.1,-1.0); M1[1][1]=Interval(0.4,0.5); M1[1][2]=Interval(-1.1,-1.1);
    M1[2][0]=Interval(-1.0,-0.95); M1[2][1]=Interval(-0.3,-0.2); M1[2][2]=Interval(-0.45,-0.35);
    IntervalMatrix E(3,3);
    IntervalMatrix comp(3,3);
    comp = Matrix::eye(3);
    std::vector<int> vectres=  construct_free_set(M1,MRes,E,NULL);
    std::cout << vectres.size() << " " << vectres[0] << " " << vectres[1] << "\n";
    std::cout <<  MRes << "\n" << E << "\n";

    vectres=  construct_free_set(M1,MRes,E,&comp);
    std::cout << vectres.size() << " " << vectres[0] << " " << vectres[1] << " " << vectres[2] << "\n";
    std::cout <<  MRes << "\n" << E << "\n";
  }
    
}
