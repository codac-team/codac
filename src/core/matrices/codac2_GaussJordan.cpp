/** 
 *  codac2_GaussJordan.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_GaussJordan.h"

using namespace std;

namespace codac2
{
  typedef Eigen::Matrix<double,Dynamic,Dynamic> Mat;

  Mat rising(const Mat& R_, const Mat& U_, const Mat& A)
  {
    Mat R = R_, U = U_;
    size_t n = A.rows(), m = A.cols();
    size_t p = m-n;

    for(int i = n-1 ; i > 0 ; i--)
    {
      Mat K = U(i,i+p)*Mat::Identity(n,n);
      K.block(0,i,i,1) = -U.block(0,i+p,i,1);
      R = K*R;
      U = R*A;
    }

    return R;
  }

  Mat precond(const Mat& P, const Mat& L, const Mat& U)
  {
    Mat A = P.inverse()*L*U;
    Mat R = (P.inverse()*L).inverse();
    return rising(R,U,A);
  }

  Matrix gauss_jordan(const Matrix& A)
  {
    size_t n = A.nb_rows(), m = A.nb_cols();
    Eigen::FullPivLU<Mat> lu(A._e);

    Mat L = Mat::Identity(n,n);
    if(std::pow(L.determinant(),2) < 1e-5)
    {
      cout << "[Matrix gauss_jordan(const Matrix& A)] -> eye matrix" << endl;
      return Matrix::eye(n,n);
    }
    L.block(0,0,n,m).triangularView<Eigen::StrictlyLower>() = lu.matrixLU();

    Mat P = lu.permutationP();
    Mat U = lu.matrixLU().triangularView<Eigen::Upper>();

    return precond(P,L,U);
  }
}