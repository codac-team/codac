/** 
 *  codac2_peibos.cpp
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_peibos.h"
#include "codac2_Parallelepiped_eval.h"

using namespace std;
using namespace codac2;

namespace codac2
{
  double split (const IntervalVector& X, double eps, vector<IntervalVector>& boxes)
  {
    if (X.max_diam()<=eps)
    {
      boxes.push_back(X);
      return X.max_diam();
    }
    else
    {
      auto p = X.bisect_largest(0.5);
      double diam1 = split(p.first,eps,boxes);
      double diam2 = split (p.second,eps,boxes);
      return std::max(diam1,diam2);
    }
  }

  double error(const IntervalVector& Y, const Vector& z, const IntervalMatrix& Jf, const Matrix& A, const IntervalVector& X)
  {
    auto xc = X.mid();

    auto dX = X-xc;

    auto E = (Y - z) + (Jf - A)*dX;

    return E.norm().ub();
  }

  Matrix inflate_flat_parallelepiped(const Matrix& A, const Vector& e_vec, double rho)
  {
    Index m = A.cols();
    Index n = A.rows();

    IntervalMatrix A_int (A);

    IntvFullPivLU LUdec((IntervalMatrix) A_int.transpose());
    IntervalMatrix N = LUdec.kernel();

    IntervalMatrix A_tild (n,n);
    A_tild << A, N;

    IntervalMatrix Q = inverse_enclosure(A_tild.transpose() * A_tild);

    IntervalMatrix mult = IntervalMatrix::Zero(n,n);
    for (int i = 0; i < n; i++)
      mult(i,i) = rho*sqrt(Q(i,i));

    for (int i = 0; i < m; i++)
      mult(i,i) += e_vec(i);

    // From here we have an IntervalMatrix A_inf, meaning that each generator is an interval vector
    IntervalMatrix A_inf = A_tild * mult;

    // The initial parallelepiped is <y> = Y*[-1,1]^n
    Matrix Y = A_inf.smig();

    // The following is similar to the previous operations. The difference is that here the Matrix Y is square and not interval

    // rho2 is the sum of the radiuses of the circle enclosing each interval generator
    Interval rho2 (0.);

    for (int i = 0; i < n; i++)
      rho2 += A_inf.col(i).rad().norm();

    IntervalMatrix Q2 = inverse_enclosure(Y.transpose() * Y);

    IntervalMatrix Y2 = IntervalMatrix::Zero(n,n);

    for (int i = 0; i < n; i++)
      Y2.col(i) = Y.col(i)*(1+rho2*sqrt(Q2(i,i)));
    
    return Y2.smag();
  }

  Parallelepiped parallelepiped_inclusion(const IntervalVector& Y, const IntervalMatrix& Jf, const Matrix& Jf_tild, const AnalyticFunction<VectorType>& psi_0, const OctaSym& sigma, const IntervalVector& X)
  {
    // Computation of the Jacobian of g = f o sigma(psi_0)
    IntervalMatrix Jg = Jf * (sigma.permutation_matrix().template cast<Interval>()) * psi_0.diff(X);

    Vector z = Y.mid();
    // A is an approximation of the Jacobian of g at the center of X
    Matrix A = (Jf_tild * sigma.permutation_matrix() * (psi_0.diff(X.mid()).mid()));

    // Maximum error computation
    double rho = error(Y, z, Jg, A, X);

    // Inflation of the parallelepiped
    auto A_inf = inflate_flat_parallelepiped(A, X.rad(), rho);

    return Parallelepiped(z, A_inf);
  }

  vector<Parallelepiped> PEIBOS(const AnalyticFunction<VectorType>& f, const AnalyticFunction<VectorType>& psi_0, const vector<OctaSym>& Sigma, double epsilon, bool verbose)
  {
    return PEIBOS(f, psi_0, Sigma, epsilon, Vector::Zero(psi_0.output_size()), verbose);
  }

  vector<Parallelepiped> PEIBOS(const AnalyticFunction<VectorType>& f, const AnalyticFunction<VectorType>& psi_0, const vector<OctaSym>& Sigma, double epsilon, const Vector& offset, bool verbose)
  {
    Index m = psi_0.input_size();

    assert_release (f.input_size() == psi_0.output_size() && "output size of psi_0 must match input size of f");
    assert_release (offset.size() == psi_0.output_size() && "offset size must match output size of psi_0");
    assert_release (m < psi_0.output_size());
    assert_release (Sigma.size() > 0 && (int) Sigma[0].size() == psi_0.output_size() && "no generator given or wrong dimension of generator (must match output size of psi_0)");

    clock_t t_start = clock();

    vector<Parallelepiped> output;

    vector<IntervalVector> boxes;
    double true_eps = split(IntervalVector::constant(m,{-1,1}), epsilon, boxes);

    for (const auto& sigma : Sigma)
    {
      VectorVar x(m);
      AnalyticFunction g_i ({x}, f(sigma(psi_0(x))+offset));

      for (const auto& X : boxes)        
        output.push_back(g_i.parallelepiped_eval(X));
    }

    if (verbose)
    {
      printf("\nPEIBOS statistics:\n");
      printf("------------------\n");
      printf("Real epsilon: %.4f\n", true_eps);
      printf("Computation time: %.4fs\n\n", (double)(clock()-t_start)/CLOCKS_PER_SEC);
    }

    return output;
  }
}