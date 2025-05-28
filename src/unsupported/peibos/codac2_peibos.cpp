/** 
 *  codac2_peibos.cpp
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_peibos.h"

using namespace std;
using namespace codac2;

namespace codac2
{

  bool contains (const vector<OctaSym>& existing_symmetries, const OctaSym& symmetry_to_test, const AnalyticFunction<VectorType>& psi_0)
  {
    IntervalVector test_box =  Interval(-1.,1.)*IntervalVector::Ones(psi_0.input_size());
    IntervalVector psi_0_x = psi_0.eval(EvalMode::NATURAL,test_box);
    
    for (const OctaSym& s : existing_symmetries)
    {
      if ((s(psi_0_x)) == (symmetry_to_test(psi_0_x)))
      {
        return true;
      }
    }
    return false;
  }

  vector<OctaSym> generate_symmetries (const vector<vector<int>>& generators, const AnalyticFunction<VectorType>& psi_0)
  {
    vector<OctaSym> symmetries;

    // Add the generators
    for (int i = 0; i < ((int) generators.size()); i++)
    {
      const OctaSym& symmetry = OctaSym(generators[i]);
      symmetries.push_back(symmetry);
    }

    // Add the inverses
    for (int i = 0; i < ((int) generators.size()); i++)
    {
      const OctaSym& symmetry = OctaSym(generators[i]);
      if (!contains(symmetries, symmetry.invert(), psi_0))
        symmetries.push_back(symmetry.invert());
    }

    // Add the squares
    for (auto i = 0; i < ((int) generators.size()); i++)
    {
      const OctaSym& symmetry = OctaSym(generators[i]);
      if (!contains(symmetries, symmetry*symmetry, psi_0))
        symmetries.push_back(symmetry*symmetry);
    }

    // Add the products
    for (int i = 0; i < ((int) generators.size()); i++)
    {
      for (int j = 0; j < (int) generators.size(); j++)
      {
        if (i != j)
        {
          const OctaSym& symmetry1 = OctaSym(generators[i]);
          const OctaSym& symmetry2 = OctaSym(generators[j]);
          if (!contains(symmetries, symmetry1*symmetry2, psi_0))
            symmetries.push_back(symmetry1*symmetry2);
        }
      }
    }

    // Add the products of square and another symmetry
    for (int i = 0; i < ((int) generators.size()); i++)
    {
      for (int j = 0; j < (int) generators.size(); j++)
      {
        const OctaSym& symmetry1 = OctaSym(generators[i]);
        const OctaSym& symmetry2 = OctaSym(generators[j]);
        if (!contains(symmetries, symmetry1*symmetry1*symmetry2, psi_0))
          symmetries.push_back(symmetry1*symmetry2);
      }
    }

    return symmetries;
  }

  double error(const IntervalMatrix& JJf, const IntervalMatrix& JJf_point, const AnalyticFunction<VectorType>& psi_0, const OctaSym& symmetry, const IntervalVector& X)
  {
    auto xc = X.mid();

    auto dX=X-xc;
    IntervalMatrix JJg_punc=JJf_point*(symmetry.permutation_matrix().template cast<Interval>())*psi_0.diff(xc);

    IntervalMatrix JJg=JJf*(symmetry.permutation_matrix().template cast<Interval>())*psi_0.diff(X);

    auto E = (JJg - JJg_punc)*dX;
    auto N = sqr(E[0]) + sqr(E[1]);

    return std::sqrt(N.ub());
  }

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

  Matrix inflate_flat_parallelepiped(const Matrix& Jz, double epsilon, double rho)
  {

    Index m = Jz.cols();
    Index n = Jz.rows();

    Matrix A (n, m);
    
    for (int i = 0; i < m; i++)
      A.col(i) = Jz.col(i)*0.5 * epsilon;

    Eigen::FullPivLU<Eigen::MatrixXd> lu_decomp(A.transpose());
    Eigen::MatrixXd N = lu_decomp.kernel();

    Matrix A_tild (n,n);
    A_tild << A, N;

    Matrix Q = (A_tild.transpose() * A_tild).inverse();

    Matrix mult (n, n);
    for (int i = 0; i < n; i++)
      mult(i,i) = rho*std::sqrt(Q(i,i));

    for (int i = 0; i < m; i++)
      mult(i,i) ++;
    
    return A_tild*mult;
  }

  Parallelepiped parallelepiped_inclusion(const Vector& z, const IntervalMatrix& JJf, const IntervalMatrix& JJf_point, const AnalyticFunction<VectorType>& psi_0, const OctaSym& symmetry, const IntervalVector& X, double true_eps)
  {
    // Maximum error computation
    double rho = error( JJf, JJf_point, psi_0, symmetry, X);

    auto Jz = (JJf_point * (symmetry.permutation_matrix().template cast<Interval>()) * psi_0.diff(X.mid())).mid();

    // Inflation of the parallelepiped

    auto A = inflate_flat_parallelepiped(Jz, true_eps, rho);

    return Parallelepiped(z, A);
  }

  vector<Parallelepiped> PEIBOS(const AnalyticFunction<VectorType>& f, const AnalyticFunction<VectorType>& psi_0, const vector<vector<int>>& generators , double epsilon, bool verbose)
  {
    return PEIBOS(f, psi_0, generators, epsilon, Vector::Zero(psi_0.output_size()), verbose);
  }

  vector<Parallelepiped> PEIBOS(const AnalyticFunction<VectorType>& f, const AnalyticFunction<VectorType>& psi_0, const vector<vector<int>>& generators , double epsilon, const Vector& offset, bool verbose)
  {
    Index m = psi_0.input_size();
    Index n = psi_0.output_size();

    assert_release (f.input_size() == n && "output size of psi_0 must match input size of f");
    assert_release (offset.size() == n && "offset size must match output size of psi_0");
    assert_release (m < n);
    assert_release (generators.size() > 0 && (int) generators[0].size() == n && "no generator given or wrong dimension of generator (must match output size of psi_0)");

    clock_t t_start = clock();

    vector<Parallelepiped> output;

    // Generate the symmetries from the generators
    vector<OctaSym> symmetries = generate_symmetries(generators, psi_0);
    vector<IntervalVector> boxes;
    double true_eps = split(Interval(-1.,1.)*IntervalVector::Ones(m), epsilon, boxes);

    for (const auto& symmetry : symmetries)
    {
      for (const auto& X : boxes)
      {

        IntervalVector Y = symmetry(psi_0.eval(X)) + offset;

        auto JJf=f.diff(Y);

        auto xc = X.mid();
        auto yc = (symmetry(psi_0.eval(xc)) + offset).mid();

        auto JJf_point=f.diff(yc).mid();

        // Center of the parallelepiped
        auto z = f.eval(yc).mid();

        auto p = parallelepiped_inclusion(z, JJf, JJf_point, psi_0, symmetry, X, true_eps);
        
        output.push_back(p);

      }
    }

    if (verbose)
    {
      printf("\nPEIBOS statistics:\n");
      printf("------------------\n");
      printf("Number of symmetries: %ld\n", symmetries.size());
      printf("Real epsilon: %.4f\n", true_eps);
      printf("Computation time: %.4fs\n\n", (double)(clock()-t_start)/CLOCKS_PER_SEC);
    }

    return output;

  }

}