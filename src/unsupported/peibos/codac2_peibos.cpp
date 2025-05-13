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

  double distance_from_line_to_origin(Eigen::Matrix<double,3,1> a, Eigen::Matrix<double,3,1> b)
  {
    return (a.cross(b)).norm()/((b-a).norm());
  }

  bool contains (vector<OctaSym> symmetries, OctaSym symmetry, const AnalyticFunction<VectorType>& psi_0)
  {
    IntervalVector test_box =  Interval(-1.,1.)*IntervalVector::Ones(psi_0.input_size());
    IntervalVector psi_0_x = psi_0.eval(EvalMode::NATURAL,test_box);
    
    for (OctaSym s : symmetries)
    {
      if ((s(psi_0_x)) == (symmetry(psi_0_x)))
      {
        return true;
      }
    }
    return false;
  }

  vector<OctaSym> generate_symmetries (vector<vector<int>> generators, const AnalyticFunction<VectorType>& psi_0)
  {
    vector<OctaSym> symmetries;

    // Add the generators
    for (int i = 0; i < ((int) generators.size()); i++)
    {
      OctaSym symmetry = OctaSym(generators[i]);
      symmetries.push_back(symmetry);
    }

    // Add the inverses
    for (int i = 0; i < ((int) generators.size()); i++)
    {
      OctaSym symmetry = OctaSym(generators[i]);
      if (!contains(symmetries, symmetry.invert(), psi_0))
        symmetries.push_back(symmetry.invert());
    }

    // Add the squares
    for (auto i = 0; i < ((int) generators.size()); i++)
    {
      OctaSym symmetry = OctaSym(generators[i]);
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
          OctaSym symmetry1 = OctaSym(generators[i]);
          OctaSym symmetry2 = OctaSym(generators[j]);
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
        OctaSym symmetry1 = OctaSym(generators[i]);
        OctaSym symmetry2 = OctaSym(generators[j]);
        if (!contains(symmetries, symmetry1*symmetry1*symmetry2, psi_0))
          symmetries.push_back(symmetry1*symmetry2);
      }
    }

    return symmetries;
  }

  double error(IntervalMatrix JJf, IntervalMatrix JJf_punc, const AnalyticFunction<VectorType>& psi_0, OctaSym symmetry, IntervalVector X)
  {
    auto xc = X.mid();

    IntervalVector dX=X-xc;
    IntervalMatrix JJg_punc=JJf_punc*IntervalMatrix(symmetry.permutation_matrix())*psi_0.diff(xc);

    IntervalMatrix JJg=JJf*IntervalMatrix(symmetry.permutation_matrix())*psi_0.diff(X);

    IntervalVector E = (JJg - JJg_punc)*dX;
    Interval N = sqr(E[0]) + sqr(E[1]);

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

  Matrix inflate_flat_parallelepiped(Matrix Jz, double epsilon, double rho)
  {

    int m = Jz.cols();
    int n = Jz.rows();

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


  vector<Parallelepiped> PEIBOS(const AnalyticFunction<VectorType>& f, const AnalyticFunction<VectorType>& psi_0, vector<vector<int>> generators , double epsilon)
  {
    return PEIBOS(f, psi_0, generators, epsilon, Vector::Zero(psi_0.output_size()));
  }

  vector<Parallelepiped> PEIBOS(const AnalyticFunction<VectorType>& f, const AnalyticFunction<VectorType>& psi_0, vector<vector<int>> generators , double epsilon, Vector offset)
  {
    int m = psi_0.input_size();
    int n = psi_0.output_size();

    assert (f.input_size() == n);
    assert(offset.size() == n);
    assert (m < n);
    assert (generators.size() > 0 && (int) generators[0].size() == n);

    clock_t t_start = clock();

    vector<Parallelepiped> output;

    // Generate the symmetries from the generators
    vector<OctaSym> symmetries = generate_symmetries(generators, psi_0);
    vector<IntervalVector> boxes;
    double true_eps = split(Interval(-1.,1.)*IntervalVector::Ones(psi_0.input_size()), epsilon, boxes);

    for (const auto& symmetry : symmetries)
    {
      for (const auto& X : boxes)
      {

        IntervalVector Y = symmetry(psi_0.eval(X)) + offset;

        IntervalMatrix JJf=f.diff(Y);

        auto xc = X.mid();
        auto yc = (symmetry(psi_0.eval(xc)) + offset).mid();

        IntervalMatrix JJf_punc=f.diff(yc).mid();

        // Center of the parallelepiped
        Vector z = f.eval(yc).mid();

        // Maximum error computation
        double rho = error( JJf, JJf_punc, psi_0, symmetry, X);

        IntervalMatrix Jz = (JJf_punc * IntervalMatrix(symmetry.permutation_matrix()) * psi_0.diff(xc)).mid();

        // Inflation of the parallelepiped

        Matrix A = inflate_flat_parallelepiped(Jz.mid(), true_eps, rho);

        if (A.is_nan() || IntervalMatrix(A).is_unbounded()) // handle degenerated case (and almost degenerated cases)
          {
            z = f.eval(Y).mid();
            Vector vars = (f.eval(Y) - z).ub();
            A = vars * Matrix::Identity(n,n);
          }

        output.push_back(Parallelepiped(z, A));

      }
    }

    printf("\nPEIBOS statistics:\n");
    printf("------------------\n");
    printf("Number of symmetries: %ld\n", symmetries.size());
    printf("Real epsilon: %.4f\n", true_eps);
    printf("Computation time: %.4fs\n\n", (double)(clock()-t_start)/CLOCKS_PER_SEC);

    return output;

  }

}