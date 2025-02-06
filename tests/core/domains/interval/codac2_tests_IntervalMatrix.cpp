/** 
 *  Codac tests
 *  
 *  Most of these tests come from the IBEX library (Gilles Chabert)
 *  See more: https://ibex-lib.readthedocs.io
 *  They have been revised to fit with Codac (v2)
 *
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Gilles Chabert, Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_Matrix.h>
#include <codac2_IntervalMatrix.h>
#include <codac2_IntervalVector.h>
#include <codac2_Approx.h>

using namespace std;
using namespace codac2;

IntervalMatrix M1()
{
  return IntervalMatrix {
    {{0,1},{0,2},{0,3}},
    {{-1,0},{-2,0},{-3,0}}
  };
}

IntervalMatrix M2() // the transpose of M1
{
  return IntervalMatrix {
    {{0,1},{-1,0}},
    {{0,2},{-2,0}},
    {{0,3},{-3,0}}
  };
}

IntervalMatrix M3() // non-null intersection with M1
{
  return IntervalMatrix {
    {{1,2},{1,2},{2,4}},
    {{-2,-1},{-2,-1},{-4,-2}}
  };
}

TEST_CASE("IntervalMatrix")
{
  {
    IntervalMatrix m1(2,3);
    IntervalMatrix m2(3,2);
    CHECK(m1 != m2);
    CHECK(!(m1 == m2));
  }

  {
    IntervalMatrix m1(3,2);
    IntervalMatrix m2(2,2);
    CHECK(m1 != m2);
    CHECK(!(m1 == m2));
  }

  {
    IntervalMatrix m1(2,3);
    IntervalMatrix m2(2,3);

    CHECK(m1.rows() == 2);
    CHECK(m1.cols() == 3);
    CHECK(m2.rows() == 2);
    CHECK(m2.cols() == 3);

    m1(0,0) = 1.;
    m1(0,1) = 2.;
    m1(0,2) = 3.;
    m1(1,0) = 4.;
    m1(1,1) = 5.;
    m1(1,2) = 6.;
    m2(0,0) = 1.;
    m2(0,1) = 2.;
    m2(0,2) = 3.;
    m2(1,0) = 4.;
    m2(1,1) = 5.;
    m2(1,2) = 6.;

    CHECK(m1 == m2);
    CHECK(!(m1 != m2));

    m2(1,2) = 7.;
    CHECK(m1 != m2);
    CHECK(!(m1 == m2));
  }

  {
    IntervalMatrix m1(2,3);
    IntervalMatrix m2(2,3);
    m1(1,1) = -1;
    m2(1,1) = -2;
    CHECK(m1 != m2);
    CHECK(!(m1 == m2));
    m1.set_empty();
    m2.set_empty();
    CHECK(m1 == m2);
    CHECK(!(m1 != m2));
  }

  {
    IntervalMatrix m(2,3);
    CHECK(m.rows() == 2);
    CHECK(m.cols() == 3);
    CHECK(m(0,0) == Interval(-oo,oo));
    CHECK(m(0,1) == Interval(-oo,oo));
    CHECK(m(0,2) == Interval(-oo,oo));
    CHECK(m(1,0) == Interval(-oo,oo));
    CHECK(m(1,1) == Interval(-oo,oo));
    CHECK(m(1,2) == Interval(-oo,oo));

    CHECK(m == IntervalMatrix(m));
    CHECK(m == (IntervalMatrix(2,3)=m));
  }

  {
    IntervalMatrix m1 = IntervalMatrix::zero(2,3);
    auto r1_0 = IntervalVector({{0,1},{0,2},{0,3}});
    CHECK(r1_0.cols() == 1);
    CHECK(r1_0.rows() == 3);
    auto r1 = IntervalVector({{0,1},{0,2},{0,3}}).transpose().eval();
    CHECK(r1.cols() == 3);
    CHECK(r1.rows() == 1);
    auto r2 = IntervalVector({{-1,0},{-2,0},{-3,0}}).transpose().eval();
    CHECK(r2.cols() == 3);
    CHECK(r2.rows() == 1);
    CHECK(r1 == IntervalMatrix({
      { {0,1}, {0,2}, {0,3} }
    }));
    m1.row(0) = r1;
    m1.row(1) = r2;

    IntervalVector c1({{0,1},{-1,0}});
    IntervalVector c2({{0,2},{-2,0}});
    IntervalVector c3({{0,3},{-3,0}});

    CHECK(m1.rows() == 2);
    CHECK(m1.cols() == 3);
    CHECK((m1.row(0) == r1));
    CHECK((m1.row(1) == r2));
    CHECK((m1.row(0) == r1));
    CHECK((m1.row(1) == r2));

    CHECK((m1.col(0) == c1));
    CHECK((m1.col(1) == c2));
    CHECK((m1.col(2) == c3));
    CHECK(m1(0,0) == Interval(0,1));
    CHECK(m1(0,1) == Interval(0,2));
    CHECK(m1(0,2) == Interval(0,3));
    CHECK(m1(1,0) == Interval(-1,0));
    CHECK(m1(1,1) == Interval(-2,0));
    CHECK(m1(1,2) == Interval(-3,0));

    CHECK(m1 == IntervalMatrix(m1));
    CHECK(m1 == (IntervalMatrix(2,3)=m1));
  }

  {
    Interval x(-1,2);
    IntervalMatrix m = IntervalMatrix::constant(2,3,x);

    CHECK(m.rows() == 2);
    CHECK(m.cols() == 3);
    for(size_t i = 0 ; i < 2 ; i++)
      for(size_t j = 0 ; j < 3 ; j++)
        CHECK(m(i,j) == x);

    CHECK(m == IntervalMatrix(m));
    CHECK(m == (IntervalMatrix(2,3)=m));
  }

  {
    double _m[][2] = {
      {0,1}, {0,2}, {0,3},
      {-1,0},{-2,0},{-3,0}
    };
    IntervalMatrix m(2,3,_m);
    CHECK(m == M1());
  }

  {
    IntervalMatrix m {
      {{0,1}, {0,2}, {0,3}},
      {{-1,0},{-2,0},{-3,0}}
    };
    CHECK(m == M1());
  }

  {
    IntervalMatrix m(2,3);
    // accessor (row,col)
    m(0,0) = Interval(0,1);
    m(0,1) = Interval(0,2);
    m(0,2) = Interval(0,3);
    m(1,0) = Interval(-1,0);
    m(1,1) = Interval(-2,0);
    m(1,2) = Interval(-3,0);
    CHECK(m == M1());
  }

  {
    CHECK(IntervalMatrix::empty(2,3).rows() == 2);
    CHECK(IntervalMatrix::empty(2,3).cols() == 3);

    CHECK(IntervalMatrix(IntervalMatrix::empty(2,3)) == IntervalMatrix::empty(2,3));
    CHECK((IntervalMatrix(2,3)=IntervalMatrix::empty(2,3)) == IntervalMatrix::empty(2,3));

    CHECK(!IntervalMatrix(2,3).is_empty());
    CHECK(IntervalMatrix::empty(2,3).is_empty());

    IntervalMatrix m(2,3);
    m.set_empty();
    CHECK(m.is_empty());
  }

  // Intersection of a matrix with itself
  {
    CHECK((M1()&=M1()) == M1());
  }

  // Intersection of two overlapping matrices
  {
    double _m[][2] = {
      {1,1},  {1,2},  {2,3},
      {-1,-1},{-2,-1},{-3,-2}
    };

    CHECK((M1()&=M3()) == IntervalMatrix(2,3,_m));
  }

  // Intersection of two non-overlapping matrices
  {
    IntervalMatrix m3(M3());
    m3(1,2) = Interval(-5,-4);
    CHECK((M1()&=m3).is_empty());
  }

  {
    IntervalMatrix m1(M1());

    IntervalVector v(2);
    v[0] = Interval(1,2);
    v[1] = Interval(-2,-1);

    m1.col(1) = v;

    double _m2[][2] = {
      {0,1}, {1,2}, {0,3},
      {-1,0},{-2,-1},{-3,0}
    };

    IntervalMatrix m2(2,3,_m2);
    CHECK(m1 == m2);
  }

  {
    CHECK(M1().block(0,0,2,3) == M1());
  }

  {
    double _r0[][2] = { {0,1}, {0,2}, {0,3} };
    CHECK(M1().block(0,0,1,3) == IntervalMatrix(1,3,_r0));
  }

  {
    double _r1[][2] = { {-1,0},{-2,0},{-3,0} };
    CHECK(M1().block(1,0,1,3) == IntervalMatrix(1,3,_r1));
  }

  {
    CHECK(M1().block(0,0,2,3) == M1());
  }

  {
    double _c0[][2] = { {0,1}, {-1,0} };
    CHECK(M1().block(0,0,2,1) == IntervalMatrix(2,1,_c0));
  }

  {
    double _c1[][2] = { {0,2}, {-2,0} };
    CHECK(M1().block(0,1,2,1) == IntervalMatrix(2,1,_c1));
  }

  {
    double _c2[][2] = { {0,3}, {-3,0} };
    CHECK(M1().block(0,2,2,1) == IntervalMatrix(2,1,_c2));
  }

  {
    double _c12[][2] = { {0,2}, {0,3}, {-2,0}, {-3,0} };
    CHECK(M1().block(0,1,2,2) == IntervalMatrix(2,2,_c12));
  }

  {
    IntervalMatrix m1(2,2);
    IntervalVector r1({{0,1},{0,2}});
    IntervalVector r2({{-1,0},{-2,0}});
    m1.row(0) = r1.transpose().eval();
    m1.row(1) = r2.transpose().eval();
    m1.resize_save_values(2,3);
    m1(0,2) = Interval(0,3);
    m1(1,2) = Interval(-3,0);

    CHECK(m1 == M1());
  }

  {
    IntervalMatrix m1(1,3);
    IntervalVector r1({{0,1},{0,2},{0,3}});
    m1.row(0) = r1.transpose().eval();
    m1.resize_save_values(2,3);
    m1(1,0) = Interval(-1,0);
    m1(1,1) = Interval(-2,0);
    m1(1,2) = Interval(-3,0);

    CHECK(m1 == M1());
  }

  {
    IntervalMatrix e(IntervalMatrix::empty(1,1));
    e.resize_save_values(2,3);
    CHECK(e.is_empty());
  }

  {
    IntervalMatrix m1(M1());
    IntervalMatrix m2(-m1);
    for(size_t i = 0 ; i < 2 ; i++)
      for(size_t j = 0 ; j < 3 ; j++)
        CHECK(m2(i,j) == -m1(i,j));
  }

  {
    CHECK((-IntervalMatrix::empty(2,3)).is_empty());
  }

  {
    IntervalMatrix m1(M1());
    IntervalMatrix m2(m1+m1);

    for(size_t i = 0 ; i < 2 ; i++)
      for(size_t j = 0 ; j < 3 ; j++)
        CHECK(m2(i,j) == m1(i,j)+m1(i,j));

    CHECK(m2 == (IntervalMatrix(m1)+=m1));
  }

  {
    IntervalMatrix m1(IntervalMatrix::empty(2,3));
    IntervalMatrix m2(2,3);

    CHECK((m1+m2).is_empty());
    CHECK((m1+=m2).is_empty());
    CHECK((m2+=m1).is_empty());
  }

  {
    IntervalMatrix m1(M1());
    IntervalMatrix m2(m1-m1);
    for(size_t i = 0 ; i < 2 ; i++)
      for(size_t j = 0 ; j < 3 ; j++)
        CHECK(m2(i,j) == m1(i,j)-m1(i,j));

    CHECK(m2 == (IntervalMatrix(m1)-=m1));
  }

  {
    IntervalMatrix m1(IntervalMatrix::empty(2,3));
    IntervalMatrix m2(2,3);

    CHECK((m1-m2).is_empty());
    CHECK((m1-=m2).is_empty());
    CHECK((m2-=m1).is_empty());
  }

  {
    IntervalMatrix m1(M1());
    IntervalMatrix m2(M2());
    IntervalMatrix m3(m1*m2);
    CHECK(m3.rows() == 2);
    CHECK(m3.cols() == 2);

    for(size_t i = 0 ; i < 2 ; i++)
      for(size_t j = 0 ; j < 2 ; j++)
        CHECK(m3(i,j) == m1(i,0)*m2(0,j)+m1(i,1)*m2(1,j)+m1(i,2)*m2(2,j));

    // Not supported by Eigen: CHECK(m3 == (IntervalMatrix(m1)*=m2));
  }

  {
    IntervalMatrix m1(IntervalMatrix::empty(2,3));
    IntervalMatrix m2(3,2);

    CHECK((m1*m2).is_empty());
    // Not supported by Eigen: CHECK((m1*=m2).is_empty());
    // Not supported by Eigen: CHECK((m2*=m1).is_empty());
  }

  {
    IntervalMatrix M1 = Matrix::eye(2,2);
    CHECK(M1 == IntervalMatrix{
      { {1,1},{0,0} },
      { {0,0},{1,1} }
    });

    IntervalMatrix M2 = 2.*Matrix::eye(2,2);
    CHECK(M2 == IntervalMatrix{
      { {2,2},{0,0} },
      { {0,0},{2,2} }
    });

    Matrix M3_degenerate = -1.*Matrix::eye(2,2);
    CHECK(M3_degenerate == Matrix{
      { -1,0 },
      { 0,-1 }
    });

    IntervalMatrix M3_Matrix = Interval(-1,1)*Matrix::eye(2,2);
    CHECK(M3_Matrix == IntervalMatrix{
      { {-1,1},{0,0} },
      { {0,0},{-1,1} }
    });

    IntervalMatrix M3_IntervalMatrix = Interval(-1,1)*IntervalMatrix::eye(2,2);
    CHECK(M3_IntervalMatrix == IntervalMatrix{
      { {-1,1},{0,0} },
      { {0,0},{-1,1} }
    });
  }

  {
    IntervalMatrix M1 = 2.*Matrix::eye(3,3);
    IntervalVector V1(3);
    V1[0] = 3; V1[1] = 4; V1[2] = 5;
    IntervalMatrix res(4,4);
    res.block(0,0,3,3) = M1;
    res.block(0,3,3,1) = V1;
    res.block(3,0,1,3) = IntervalVector::ones(3).transpose().eval();
    res(3,3) = 6;
    double _expected[16] = {
      2,0,0,3,
      0,2,0,4,
      0,0,2,5,
      1,1,1,6 };
    CHECK((res == Matrix(4,4,_expected)));
  }

  {
    Matrix m1 {
      { 0,2 },
      { 3,10 }
    };
    Matrix m2 {
      { -1,7 },
      { 8,4 }
    };

    CHECK((IntervalMatrix(m1) | m2) == IntervalMatrix{
      { {-1,0},{2,7} },
      { {3,8},{4,10} }
    });
  }

  {
    IntervalMatrix m1 {
      { {0,1},{0,2} },
      { {0,0},Interval::empty() }
    };
    IntervalMatrix m2 {
      { {-oo,oo},{-1,3} },
      { {2,4},{1,1} }
    };

    CHECK((m1 | m2) == IntervalMatrix{
      { {-oo,oo},{-1,3} },
      { {0,4},{1,1} }
    });
  }

  {
    IntervalMatrix m1 {
      { {0,1},{0,2} },
      { {0,0},Interval::empty() }
    };
    IntervalMatrix m2 {
      { {-oo,oo},{-1,3} },
      { {2,4},{1,1} }
    };

    CHECK((m1 & m2) == IntervalMatrix{
      { {0,1},{0,2} },
      { Interval::empty(),Interval::empty() }
    });
  }
}

TEST_CASE("IntervalMatrix - mixing type")
{
  {
    Matrix m1 {
      { {1},{2} },
      { {3},{4} }
    };

    // Interval - Matrix
    CHECK((Interval(-1,1) * m1) == IntervalMatrix{
      { {-1,1},{-2,2} },
      { {-3,3},{-4,4} }
    });

    // Matrix - Interval
    CHECK((m1 / Interval(2)) == IntervalMatrix{
      { {1./2.},{2./2.} },
      { {3./2.},{4./2.} }
    });

    IntervalVector v1({
      -1,
      -2
    });

    CHECK(v1[0] == -1);
    CHECK(v1[1] == -2);

    IntervalVector iv1({
      {-1,1},
      {-1,1}
    });

    // Matrix - IntervalVector
    CHECK((m1 * iv1) == IntervalVector{
      { {-3,3} },
      { {-7,7} }
    });

    // double - IntervalVector
    CHECK(((double)-3 * iv1) == IntervalVector{
      { {-3,3} },
      { {-3,3} }
    });

    // Interval - Vector
    CHECK((Interval(-1,1) * v1) == IntervalVector{
      { {-1,1} },
      { {-2,2} }
    });

    IntervalMatrix im1 {
      { {-1,1},{-2,2} },
      { {-3,3},{-4,4} }
    };

    // Matrix - IntervalMatrix
    CHECK((m1 + im1) == IntervalMatrix{
      { {0,2},{0,4} },
      { {0,6},{0,8} }
    });

    // IntervalMatrix - Matrix
    CHECK((im1 + m1) == IntervalMatrix{
      { {0,2},{0,4} },
      { {0,6},{0,8} }
    });

    // IntervalMatrix - Matrix block
    CHECK((im1 + m1.block(0,0,2,2)) == IntervalMatrix{
      { {0,2},{0,4} },
      { {0,6},{0,8} }
    });

    // Matrix block - IntervalMatrix
    CHECK((m1.block(0,0,2,2) + im1) == IntervalMatrix{
      { {0,2},{0,4} },
      { {0,6},{0,8} }
    });

    // Matrix - IntervalMatrix
    CHECK((m1 - im1) == IntervalMatrix{
      { {0,2},{0,4} },
      { {0,6},{0,8} }
    });

    // IntervalMatrix - Matrix
    CHECK((im1 - m1) == IntervalMatrix{
      { {-2,0},{-4,0} },
      { {-6,0},{-8,0} }
    });

    // Matrix - col of IntervalMatrix
    CHECK((m1 * im1.col(0)) == IntervalMatrix({
      { {-7,7} },
      { {-15,15} }
    }));

    // Row of Matrix - col of Matrix
    CHECK((m1.row(1) * m1.col(0)) == Matrix({{15}}));

    // Row of Matrix - col of IntervalMatrix
    CHECK((m1.row(1) * im1.col(0)) == IntervalMatrix({{{-15,15}}}));

    // Row of IntervalMatrix - col of IntervalMatrix
    CHECK((im1.row(1) * im1.col(0)) == IntervalMatrix({{{-15,15}}}));
  }

  {
    Matrix m1 {
      { {1},{2} },
      { {3},{4} }
    };

    Matrix m2 {
      { {2},{3} },
      { {4},{5} }
    };

    IntervalMatrix im(m1,m2);
    CHECK(im == IntervalMatrix{
        { {1,2},{2,3} },
        { {3,4},{4,5} }
      });

    m2(0,1) = 1.;

    IntervalMatrix im_empty(m1,m2);
    CHECK(im_empty == IntervalMatrix::empty(2,2));
  }

  {
    IntervalMatrix m1 = IntervalMatrix::ones(3,3);
    IntervalMatrix m2 = IntervalMatrix::zero(3,3);
    CHECK(m1.volume() == 0.);
    CHECK(m2.volume() == 0.);
    CHECK((m1+m2).volume() == 0.);
  }

  {
    IntervalMatrix m1 {
      { {1,2},{2,3} },
      { {3,4},{4,5} }
    };
    Matrix m2 {
      { 1.5, 2.5 },
      { 3.5, 4.5 }
    };

    CHECK(m1.contains(m2));
    CHECK(m2.template cast<Interval>().is_strict_subset(m1));

    Matrix m3(2,2);
    m3.init(m2+m2);
    CHECK(m3 == Matrix({
      { 3, 5 },
      { 7, 9 }
    }));

    IntervalMatrix m4(m2+m2);
    CHECK(m4 == IntervalMatrix({
      {{3,3},{5,5}},
      {{7,7},{9,9}}
    }));

    IntervalMatrix m5(m2*m2);
    CHECK(m5 == IntervalMatrix({
      {{11,11},{15,15}},
      {{21,21},{29,29}}
    }));
  }

  {
    IntervalMatrix test1 {
      { {0,1}, {0,2}, {0,0} }
    };
    CHECK(test1.rows() == 1);
    CHECK(test1.cols() == 3);
    CHECK(test1(0,0) == Interval(0,1));
    CHECK(test1(0,1) == Interval(0,2));
    CHECK(test1(0,2) == Interval(0,0));

    IntervalMatrix test2 {
      {0,1},
      {2,3},
      {4,5}
    };
    CHECK(test2.rows() == 3);
    CHECK(test2.cols() == 2);
    CHECK(test2(0,0) == Interval(0));
    CHECK(test2(0,1) == Interval(1));
    CHECK(test2(1,0) == Interval(2));
    CHECK(test2(1,1) == Interval(3));
    CHECK(test2(2,0) == Interval(4));
    CHECK(test2(2,1) == Interval(5));

    IntervalMatrix test3 {
      {{1,oo},{2,oo}},
      {{3,oo},{4,oo}},
      {{5,oo},{6,oo}}
    };
    CHECK(test3.rows() == 3);
    CHECK(test3.cols() == 2);
    CHECK(test3(0,0) == Interval(1,oo));
    CHECK(test3(0,1) == Interval(2,oo));
    CHECK(test3(1,0) == Interval(3,oo));
    CHECK(test3(1,1) == Interval(4,oo));
    CHECK(test3(2,0) == Interval(5,oo));
    CHECK(test3(2,1) == Interval(6,oo));

    IntervalMatrix test4 {
      {{1,2}},
      {{3,4}},
      {{5,6}}
    };
    CHECK(test4.rows() == 3);
    CHECK(test4.cols() == 1);
    CHECK(test4(0,0) == Interval(1,2));
    CHECK(test4(1,0) == Interval(3,4));
    CHECK(test4(2,0) == Interval(5,6));

    IntervalVector iv({{1,2},{3,4},{5,6}});
    IntervalMatrix test5(iv);
    CHECK(test5.rows() == 3);
    CHECK(test5.cols() == 1);
    CHECK(test5(0,0) == Interval(1,2));
    CHECK(test5(1,0) == Interval(3,4));
    CHECK(test5(2,0) == Interval(5,6));
  }
}

#if 0
// Tests from the IBEX lib that are not considered in this file:

{
  RNG::srand(1);
  IntervalMatrix M = Matrix::rand(2);
  Matrix R=M.rad();
  CHECK(R[0][0] == M[0][0].rad());
  CHECK(R[0][1] == M[0][1].rad());
  CHECK(R[1][0] == M[1][0].rad());
  CHECK(R[1][1] == M[1][1].rad());
}

{  RNG::srand(1);
  IntervalMatrix M = Matrix::rand(2);
  Matrix R=M.diam();
  CHECK(R[0][0] == M[0][0].diam());
  CHECK(R[0][1] == M[0][1].diam());
  CHECK(R[1][0] == M[1][0].diam());
  CHECK(R[1][1] == M[1][1].diam());
}
#endif