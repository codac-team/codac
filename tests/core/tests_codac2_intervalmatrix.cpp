#include "catch_interval.hpp"
#include "vibes.h"

#include "codac2_Matrix.h"
#include "codac2_IntervalVector.h"
#include "codac2_IntervalMatrix.h"

using namespace Catch;
using namespace Detail;
using namespace std;
using namespace codac2;

// Most of these tests come from the IBEX library (G. Chabert)
// They have been revised to fit the codac2::IntervalMatrix class

IntervalMatrix M1()
{
  IntervalMatrix m(2,3);
  double _r1[][2]={{0,1},{0,2},{0,3}};
  double _r2[][2]={{-1,0},{-2,0},{-3,0}};
  IntervalVector r1(3,_r1);
  IntervalVector r2(3,_r2);
  m.row(0)=r1;
  m.row(1)=r2;
  return m;
}


IntervalMatrix M2() // the transpose of M1
{
  IntervalMatrix m(3,2);
  double _c1[][2]={{0,1},{-1,0}};
  double _c2[][2]={{0,2},{-2,0}};
  double _c3[][2]={{0,3},{-3,0}};
  IntervalVector c1(2,_c1);
  IntervalVector c2(2,_c2);
  IntervalVector c3(2,_c3);
  m.row(0)=c1;
  m.row(1)=c2;
  m.row(2)=c3;
  return m;
}

IntervalMatrix M3() // non-null intersection with M1
{
  IntervalMatrix m(2,3);
  double _r1[][2]={{1,2},{1,2},{2,4}};
  double _r2[][2]={{-2,-1},{-2,-1},{-4,-2}};
  IntervalVector r1(3,_r1);
  IntervalVector r2(3,_r2);
  m.row(0)=r1;
  m.row(1)=r2;
  return m;
}


TEST_CASE("Tests from IBEX IntervalMatrix")
{
  SECTION("eq01")
  {
    IntervalMatrix m(2,3);
    IntervalMatrix m2(3,2);
    CHECK(m!=m2);
    CHECK(!(m==m2));
  }

  SECTION("eq02")
  {
    IntervalMatrix m(3,2);
    IntervalMatrix m2(2,2);
    CHECK(m!=m2);
    CHECK(!(m==m2));
  }

  SECTION("eq03")
  {
    IntervalMatrix m(2,3);
    IntervalMatrix m2(2,3);

    CHECK(m.rows()==2);
    CHECK(m.cols()==3);
    CHECK(m2.rows()==2);
    CHECK(m2.cols()==3);

     m(0,0)=1;
     m(0,1)=2;
     m(0,2)=3;
     m(1,0)=4;
     m(1,1)=5;
     m(1,2)=6;
    m2(0,0)=1;
    m2(0,1)=2;
    m2(0,2)=3;
    m2(1,0)=4;
    m2(1,1)=5;
    m2(1,2)=6;

    CHECK(m==m2);
    CHECK(!(m!=m2));

    m2(1,2)=7;
    CHECK(m!=m2);
    CHECK(!(m==m2));
  }

  SECTION("eq04")
  {
    IntervalMatrix m(2,3);
    IntervalMatrix m2(2,3);
     m(1,1)=-1;
    m2(1,1)=-2;
    CHECK(m!=m2);
    CHECK(!(m==m2));
    m.set_empty();
    m2.set_empty();
    CHECK(m==m2);
    CHECK(!(m!=m2));
  }

  SECTION("cons01")
  {
    IntervalMatrix m(2,3);
    CHECK(m.rows()==2);
    CHECK(m.cols()==3);
    CHECK(m(0,0)==Interval::all_reals());
    CHECK(m(0,1)==Interval::all_reals());
    CHECK(m(0,2)==Interval::all_reals());
    CHECK(m(1,0)==Interval::all_reals());
    CHECK(m(1,1)==Interval::all_reals());
    CHECK(m(1,2)==Interval::all_reals());
    CHECK(m==IntervalMatrix(m));
    CHECK(m==(IntervalMatrix(2,3)=m));
  }

  SECTION("cons02")
  {
    IntervalMatrix m(2,3);
    double _r1[][2]={{0,1},{0,2},{0,3}};
    double _r2[][2]={{-1,0},{-2,0},{-3,0}};
    IntervalVector r1(3,_r1);
    IntervalVector r2(3,_r2);
    m.row(0) = r1;
    m.row(1) = r2;

    double _c1[][2]={{0,1},{-1,0}};
    double _c2[][2]={{0,2},{-2,0}};
    double _c3[][2]={{0,3},{-3,0}};
    IntervalVector c1(2,_c1);
    IntervalVector c2(2,_c2);
    IntervalVector c3(2,_c3);

    CHECK(m.rows()==2);
    CHECK(m.cols()==3);
    // not supported CHECK(m[0]==r1);
    // not supported CHECK(m[1]==r2);
// not working    CHECK(m.row(0)==r1);
// not working    CHECK(m.row(1)==r2);
    CHECK(m.col(0)==c1);
    CHECK(m.col(1)==c2);
    CHECK(m.col(2)==c3);
    CHECK(m(0,0)==Interval(0,1));
    CHECK(m(0,1)==Interval(0,2));
    CHECK(m(0,2)==Interval(0,3));
    CHECK(m(1,0)==Interval(-1,0));
    CHECK(m(1,1)==Interval(-2,0));
    CHECK(m(1,2)==Interval(-3,0));
    CHECK(m==IntervalMatrix(m));
    CHECK(m==(IntervalMatrix(2,3)=m));
  }

  SECTION("cons03")
  {
    Interval x(-1,2);
    IntervalMatrix m(2,3,x);

    CHECK(m.rows()==2);
    CHECK(m.cols()==3);
    for (int i=0; i<2; i++) {
        for (int j=0; j<3; j++)
          CHECK(m(i,j)==x);
    }

    CHECK(m==IntervalMatrix(m));
    CHECK(m==(IntervalMatrix(2,3)=m));
  }

  SECTION("cons04")
  {
    double _m[][2]={ {0,1}, {0,2}, {0,3},
                   {-1,0},{-2,0},{-3,0} };
    IntervalMatrix m(2,3,_m);
    CHECK(m==M1());
  }

  SECTION("consInitList")
  {
    IntervalMatrix m{
      {{0,1}, {0,2}, {0,3}},
      {{-1,0},{-2,0},{-3,0}}
    };
    CHECK(m == M1());
  }

  SECTION("empty01")
  {
    CHECK(IntervalMatrix::empty_set(2,3).rows()==2);
    CHECK(IntervalMatrix::empty_set(2,3).cols()==3);
    CHECK(IntervalMatrix(IntervalMatrix::empty_set(2,3))==IntervalMatrix::empty_set(2,3));
    CHECK((IntervalMatrix(2,3)=IntervalMatrix::empty_set(2,3))==IntervalMatrix::empty_set(2,3));
  }

  SECTION("is_empty01")
  {
    CHECK(!IntervalMatrix(2,3).is_empty());
  }

  SECTION("is_empty02")
  {
    CHECK(IntervalMatrix::empty_set(2,3).is_empty());
  }

  SECTION("set_empty01")
  {
    IntervalMatrix m(2,3);
    m.set_empty();
    CHECK(m.is_empty());
  }

  // intersection of a matrix with itself
  SECTION("inter01")
  {
    CHECK((M1()&=M1())==M1());
  }

  // intersection of two overlapping matrices
  SECTION("inter02")
  {
    double _m[][2]={{1,1},  {1,2},  {2,3},
                {-1,-1},{-2,-1},{-3,-2}};

    CHECK((M1()&=M3())==IntervalMatrix(2,3,_m));
  }

  // intersection of two non-overlapping matrices
  SECTION("inter03")
  {
    IntervalMatrix m3(M3());
    m3(1,2)=Interval(-5,-4);
    CHECK((M1()&=m3).is_empty());
  }

  SECTION("set_col01")
  {
    IntervalMatrix m(M1());

    IntervalVector v(2);
    v[0]=Interval(1,2);
    v[1]=Interval(-2,-1);

    m.col(1)=v;

    double _m2[][2]={ {0,1}, {1,2}, {0,3},
                  {-1,0},{-2,-1},{-3,0} };
    IntervalMatrix m2(2,3,_m2);

    CHECK(m==m2);
  }

  SECTION("rows01")
  {
    CHECK(M1().block(0,0,2,3)==M1());
  }

  SECTION("rows02")
  {
    double _r0[][2]={ {0,1}, {0,2}, {0,3} };
    CHECK(M1().block(0,0,1,3)==IntervalMatrix(1,3,_r0));
  }

  SECTION("rows03")
  {
    double _r1[][2]={ {-1,0},{-2,0},{-3,0} };
    CHECK(M1().block(1,0,1,3)==IntervalMatrix(1,3,_r1));
  }

  SECTION("cols01")
  {
    CHECK(M1().block(0,0,2,3)==M1());
  }

  SECTION("cols02")
  {
    double _c0[][2]={ {0,1}, {-1,0} };
    CHECK(M1().block(0,0,2,1)==IntervalMatrix(2,1,_c0));
  }

  SECTION("cols03")
  {
    double _c1[][2]={ {0,2}, {-2,0} };
    CHECK(M1().block(0,1,2,1)==IntervalMatrix(2,1,_c1));
  }

  SECTION("cols04")
  {
    double _c2[][2]={ {0,3}, {-3,0} };
    CHECK(M1().block(0,2,2,1)==IntervalMatrix(2,1,_c2));
  }

  SECTION("cols04")
  {
    double _c12[][2]={ {0,2}, {0,3}, {-2,0}, {-3,0} };
    CHECK(M1().block(0,1,2,2)==IntervalMatrix(2,2,_c12));
  }

  SECTION("resize01")
  {
    IntervalMatrix m(2,2);
    double _r1[][2]={{0,1},{0,2}};
    double _r2[][2]={{-1,0},{-2,0}};
    IntervalVector r1(2,_r1);
    IntervalVector r2(2,_r2);
    m.row(0)=r1;
    m.row(1)=r2;
    m.resize(2,3);
    m(0,2)=Interval(0,3);
    m(1,2)=Interval(-3,0);

    CHECK(m==M1());
  }

  SECTION("resize02")
  {
    IntervalMatrix m(1,3);
    double _r1[][2]={{0,1},{0,2},{0,3}};
    IntervalVector r1(3,_r1);
    m.row(0)=r1;
    m.resize(2,3);
    m(1,0)=Interval(-1,0);
    m(1,1)=Interval(-2,0);
    m(1,2)=Interval(-3,0);

    CHECK(m==M1());
  }

  SECTION("resize03")
  {
    IntervalMatrix e(IntervalMatrix::empty_set(1,1));
    e.resize(2,3);
    CHECK(e.is_empty());
  }

  SECTION("minus01")
  {
    IntervalMatrix m(M1());
    IntervalMatrix m2(-m);
    for (int i=0; i<2; i++) {
      for (int j=0; j<3; j++) {
        CHECK(m2(i,j)==-m(i,j));
      }
    }
  }

  SECTION("minus02")
  {
    CHECK(-IntervalMatrix::empty_set(2,3).is_empty());
  }

  SECTION("add01")
  {
    IntervalMatrix m(M1());
    IntervalMatrix m2(m+m);

    for (int i=0; i<2; i++) {
      for (int j=0; j<3; j++) {
        CHECK(m2(i,j)==m(i,j)+m(i,j));
      }
    }

    CHECK(m2==(IntervalMatrix(m)+=m));
  }

  SECTION("add02")
  {
    IntervalMatrix m1(IntervalMatrix::empty_set(2,3));
    IntervalMatrix m2(2,3);

    CHECK((m1+m2).is_empty());
    CHECK((m1+=m2).is_empty());
    CHECK((m2+=m1).is_empty());
  }

  SECTION("sub01")
  {
    IntervalMatrix m(M1());
    IntervalMatrix m2(m-m);
    for (int i=0; i<2; i++) {
      for (int j=0; j<3; j++) {
        CHECK(m2(i,j)==m(i,j)-m(i,j));
      }
    }

    CHECK(m2==(IntervalMatrix(m)-=m));
  }

  SECTION("sub02")
  {
    IntervalMatrix m1(IntervalMatrix::empty_set(2,3));
    IntervalMatrix m2(2,3);

    CHECK((m1-m2).is_empty());
    CHECK((m1-=m2).is_empty());
    CHECK((m2-=m1).is_empty());
  }

  SECTION("mul01")
  {
    IntervalMatrix m(M1());
    IntervalMatrix m2(M2());
    IntervalMatrix m3(m*m2);
    CHECK(m3.rows()==2);
    CHECK(m3.cols()==2);

    for (int i=0; i<2; i++) {
      for (int j=0; j<2; j++)
        CHECK(m3(i,j)==m(i,0)*m2(0,j)+m(i,1)*m2(1,j)+m(i,2)*m2(2,j));
    }

    CHECK(m3==(IntervalMatrix(m)*=m2));
  }

  SECTION("mul02")
  {
    IntervalMatrix m1(IntervalMatrix::empty_set(2,3));
    IntervalMatrix m2(3,2);

    CHECK(IntervalMatrix(m1*m2).is_empty());
    CHECK(IntervalMatrix(m1*=m2).is_empty());
    CHECK(IntervalMatrix(m2*=m1).is_empty());
  }

  SECTION("put01")
  {
    // deprecated in codac (use Eigen instead) IntervalMatrix M1=2*Matrix::eye(3);
    // deprecated in codac (use Eigen instead) IntervalVector V1(3);
    // deprecated in codac (use Eigen instead) V1[0]=3; V1[1]=4; V1[2]=5;
    // deprecated in codac (use Eigen instead) IntervalMatrix res(4,4);
    // deprecated in codac (use Eigen instead) res.put(0,0,M1);
    // deprecated in codac (use Eigen instead) res.put(0,3,V1,false);
    // deprecated in codac (use Eigen instead) res.put(3,0,Vector::ones(3),true);
    // deprecated in codac (use Eigen instead) res[3][3]=6;
    // deprecated in codac (use Eigen instead) double _expected[16] = { 2,0,0,3,
    // deprecated in codac (use Eigen instead)              0,2,0,4,
    // deprecated in codac (use Eigen instead)              0,0,2,5,
    // deprecated in codac (use Eigen instead)              1,1,1,6 };
    // deprecated in codac (use Eigen instead) CHECK(res==(Matrix(4,4,_expected)));
  }
}

#if 0

// Tests from IBEX that are not (yet) considered in Codac:

void TestIntervalMatrix::rad01() {
  RNG::srand(1);
  IntervalMatrix M=Matrix::rand(2);
  Matrix R=M.rad();
  CHECK(R[0][0]==M[0][0].rad());
  CHECK(R[0][1]==M[0][1].rad());
  CHECK(R[1][0]==M[1][0].rad());
  CHECK(R[1][1]==M[1][1].rad());
}

void TestIntervalMatrix::diam01() {
  RNG::srand(1);
  IntervalMatrix M=Matrix::rand(2);
  Matrix R=M.diam();
  CHECK(R[0][0]==M[0][0].diam());
  CHECK(R[0][1]==M[0][1].diam());
  CHECK(R[1][0]==M[1][0].diam());
  CHECK(R[1][1]==M[1][1].diam());
}

#endif