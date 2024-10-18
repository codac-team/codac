/** 
 *  codac2_linear_ctc.cpp
 *  Contractors for linear systems of equations
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Luc Jaulin
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_linear_ctc.h"

using namespace std;
using namespace codac2;

void CtcGaussElim::contract(IntervalMatrix& A, IntervalVector& x, IntervalVector& b) const
{
  assert_release(A.is_squared() && A.nb_rows() == x.size() && A.nb_rows() == b.size());

  IntervalMatrix A_ = A;
  IntervalVector b_ = b;

  size_t n = A_.nb_rows();
  for(size_t i = 0 ; i < n ; i++)
    if(A_(i,i).contains(0.))
      return;

  for(size_t i = 0 ; i < n-1 ; i++)
    for(size_t j = i+1 ; j < n ; j++)
    {
      Interval aj = A_(j,i)/A_(i,i);
      b_[j] -= aj*b_[i];
      for(size_t k = i+1 ; k < n ; k++)
        A_(j,k) -= aj*A_(i,k);
    }

  for(int i = n-1 ; i >= 0 ; i--)
  {
    Interval sum = 0.;
    for(size_t j = i+1 ; j < n ; j++)
      sum += A_(i,j)*x[j];
    x[i] &= (b_[i]-sum)/A_(i,i);
  }
}

void CtcGaussSeidel::contract(IntervalMatrix& A, IntervalVector& x, IntervalVector& b) const
{
  assert_release(A.is_squared() && A.nb_rows() == x.size() && A.nb_rows() == b.size());

  auto ext_diag = A;
  for(size_t i = 0 ; i < A.nb_rows() ; i++)
    ext_diag(i,i) = 0.;
  x &= IntervalVector(A._e.diagonal().asDiagonal().inverse()*(b._e-ext_diag._e*x._e));
}