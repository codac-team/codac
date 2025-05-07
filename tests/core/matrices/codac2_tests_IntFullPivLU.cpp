/** 
 *  Codac tests
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Damien Massé
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <catch2/catch_test_macros.hpp>
#include <codac2_IntvFullPivLU.h>
#include <codac2_BoolInterval.h>

using namespace std;
using namespace codac2;

TEST_CASE("IntvFullPivLU")
{
  /* ``classic'' invertible matrix */
  {
    Matrix M 
    { { 1, -4,  3, 7 },
      { 2,  1, -4, 6 },
      { 5,  2,  1, 9 },
      { -1, 0,  3, 2 } };
    IntvFullPivLU LUdec(M);
    CHECK(LUdec.is_injective()==BoolInterval::TRUE);
    CHECK(LUdec.is_surjective()==BoolInterval::TRUE);
    CHECK(LUdec.rank()==Interval(4));
    CHECK((LUdec.determinant()+602).mag()<=1e-10);
    CHECK((LUdec.reconstructed_matrix()-M).lpNorm<Eigen::Infinity>().ub()<=1e-10);
    IntervalMatrix I1 = LUdec.solve(IntervalMatrix::Identity(4,4));
    CHECK((IntervalMatrix(M)*I1-IntervalMatrix::Identity(4,4))
  		.lpNorm<Eigen::Infinity>().ub()<=1e-10);
  }
  /* singular matrix , but approximations may make it non singular */
  {
    Matrix M 
    { { 1, -4, 6, 7 },
      { 2,  1, 3, 6 },
      { 5,  2, 8, 9 },
      { -1, 0, -2, 2 } };
    IntvFullPivLU LUdec(M);
    CHECK(LUdec.is_injective()==BoolInterval::UNKNOWN);
    CHECK(LUdec.is_surjective()==BoolInterval::UNKNOWN);
    CHECK(LUdec.rank()==Interval(3,4));
    CHECK(LUdec.determinant().mag()<=1e-10);
    CHECK((LUdec.reconstructed_matrix()-M).lpNorm<Eigen::Infinity>().ub()<=1e-10);
    IntervalMatrix K = LUdec.kernel();
    CHECK(K.cols()==1);
    CHECK((IntervalMatrix(M)*K).lpNorm<Eigen::Infinity>().ub()<=1e-10);
  }
  
  /* rectangular matrix, full rank */
  {
    Matrix M 
    { { 1, -4, 6, 7, 6 },
      { 2,  1, 3, 6, -2 },
      { 5,  2, 2, 9, -1 } };
    IntvFullPivLU LUdec(M);
    CHECK(LUdec.is_injective()==BoolInterval::FALSE);
    CHECK(LUdec.is_surjective()==BoolInterval::TRUE);
    CHECK(LUdec.rank()==Interval(3));
    CHECK((LUdec.reconstructed_matrix()-M).lpNorm<Eigen::Infinity>().ub()<=1e-10);
    IntervalMatrix K = LUdec.kernel();
    CHECK(K.cols()==2);
    CHECK((IntervalMatrix(M)*K).lpNorm<Eigen::Infinity>().ub()<=1e-10);
    IntervalMatrix Img = LUdec.image(M);
    CHECK(Img.cols()==3);
    IntervalMatrix I1 = LUdec.solve(IntervalMatrix::Identity(3,3));
    CHECK((IntervalMatrix(M)*I1-IntervalMatrix::Identity(3,3))
  		.lpNorm<Eigen::Infinity>().ub()<=1e-10);
  }
  
  /* rectangular matrix, full rank */
  {
    Matrix M 
    { { 1, -4, 6 },
      { 2,  1, 3 },
      { 5,  2, 2 },
      { 1,  3, -3},
      { 0,  0, 1 } };
    IntvFullPivLU LUdec(M);
    CHECK(LUdec.is_injective()==BoolInterval::TRUE);
    CHECK(LUdec.is_surjective()==BoolInterval::FALSE);
    CHECK(LUdec.rank()==Interval(3));
    CHECK((LUdec.reconstructed_matrix()-M).lpNorm<Eigen::Infinity>().ub()<=1e-10);
  }
  
  
  /* interval matrix, full rank */
  {
    IntervalMatrix M 
    { { 1, {-4,0}, {5,6}, 7, 6 },
      { 2,  {1,3}, 3, {3,6}, -2 },
      { 5,  {0,2}, 2, 9, {-4,-1} } };
    IntvFullPivLU LUdec(M);
    CHECK(M.is_subset(LUdec.reconstructed_matrix()));
    CHECK(LUdec.is_injective()==BoolInterval::FALSE);
    CHECK(LUdec.is_surjective()==BoolInterval::TRUE);
    CHECK(LUdec.rank()==Interval(3));
    IntervalMatrix K = LUdec.kernel();
    CHECK(K.cols()==2);
    IntervalMatrix Img = LUdec.image(M);
    CHECK(Img.cols()==3);
  }
  
  /* interval matrix,  not full rank */
  {
    IntervalMatrix M 
    { { {0.1,0.2}, {0.1,0.2}, {0.1,0.2}, {0.1,0.2}, {0.1,0.2} },
      { {0.1,0.2}, {0.1,0.2}, {0.1,0.2}, {0.1,0.2}, {0.1,0.2} },
      { {0.1,0.2}, {0.1,0.2}, {0.1,0.2}, {0.1,0.2}, {0.1,0.2} } };
    IntvFullPivLU LUdec(M);
    CHECK(M.is_subset(LUdec.reconstructed_matrix()));
    CHECK(LUdec.is_injective()==BoolInterval::FALSE);
    CHECK(LUdec.is_surjective()==BoolInterval::UNKNOWN);
    CHECK(LUdec.rank()==Interval(1,3));
    IntervalMatrix K = LUdec.kernel();
    CHECK(K.cols()==4);
    IntervalMatrix Img = LUdec.image(M);
    CHECK(Img.cols()==1);
  }
  
}
