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
    IntervalVector test_box =  Interval(-1.,1.)*IntervalVector::Ones(symmetry.size()-1);
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
      {
        symmetries.push_back(symmetry.invert());
      }
    }

    // Add the squares
    for (auto i = 0; i < ((int) generators.size()); i++)
    {
      OctaSym symmetry = OctaSym(generators[i]);
      if (!contains(symmetries, symmetry*symmetry, psi_0))
      {
        symmetries.push_back(symmetry*symmetry);
      }
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
          {
            symmetries.push_back(symmetry1*symmetry2);
          }
        }
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

  Matrix inflate_flat_parallelepiped_3D (IntervalMatrix Jz, double epsilon, double rho)
  {
    Eigen::Matrix<double,3,1> a1 ((Jz.col(0) * 0.5 * epsilon).mid());
    Eigen::Matrix<double,3,1> a2 ((Jz.col(1) * 0.5 * epsilon).mid());
    Eigen::Matrix<double,3,1> a3 = a1.cross(a2);

    double norm_a1 = a1.norm();
    double norm_a2 = a2.norm();

    a1*=(1+rho*norm_a1/a3.norm());
    a2*=(1+rho*norm_a2/a3.norm());


    a3*=(rho/a3.norm());

    Matrix A = Matrix({{a1[0], a2[0], a3[0]}, {a1[1], a2[1], a3[1]}, {a1[2], a2[2], a3[2]}});
    
    return A;
  }

  Matrix inflate_flat_parallelepiped_2D (IntervalMatrix Jz, double epsilon, double rho)
  {
    Vector a1 = Vector((Jz * 0.5 * epsilon).mid());
    Vector a2 ({ -a1[1], a1[0] });

    double norm_a1 = std::sqrt(std::pow(a1[0],2) + std::pow(a1[1],2));

    Matrix A ({{(1+rho/norm_a1)*a1[0], (rho/norm_a1)*a2[0]}, {(1+rho/norm_a1)*a1[1], (rho/norm_a1)*a2[1]}});

    return A;
  }

  vector<Parallelepiped> PEIBOS2D(const AnalyticFunction<VectorType>& f, const AnalyticFunction<VectorType>& psi_0, vector<vector<int>> generators , double epsilon, Vector offset)
  {
    vector<Parallelepiped> output;

    // Generate the symmetries from the generators
    vector<OctaSym> symmetries = generate_symmetries(generators, psi_0);
    for (int i = 0; i < ((int) ((int) symmetries.size())); i++)
    {
      OctaSym symmetry = symmetries[i];
      
      for (double t = -1; t < 1; t += epsilon)
      {

        IntervalVector X({{t,t+epsilon}});
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
        Matrix A = inflate_flat_parallelepiped_2D(Jz, epsilon, rho);

        output.push_back(Parallelepiped(z, A));

      }
    }

    return output;
  }

  vector<Parallelepiped> PEIBOS3D(const AnalyticFunction<VectorType>& f, const AnalyticFunction<VectorType>& psi_0, vector<vector<int>> generators , double epsilon, Vector offset)
  {
    vector<Parallelepiped> output;

    // Generate the symmetries from the generators
    vector<OctaSym> symmetries = generate_symmetries(generators, psi_0);
    for (int i = 0; i < ((int) ((int) symmetries.size())); i++)
    {
      OctaSym symmetry = symmetries[i];
      
      for (double t1 = -1; t1 < 1; t1 += epsilon)
      {
        for (double t2 = -1;t2 < 1; t2+=epsilon)
        {

          IntervalVector X({{t1,t1+epsilon},{t2,t2+epsilon}});
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

          Matrix A = inflate_flat_parallelepiped_3D(Jz, epsilon, rho);
          auto angle = acos((Jz.col(0)/Jz.col(0).norm()).dot(Jz.col(1)/Jz.col(1).norm()));

          if (Jz.col(0)==Jz.col(1) || A.is_nan() || IntervalMatrix(A).is_unbounded() || abs(angle).ub()<1e-3) // handle degenerated case (and almost degenerated cases)
            {
              z = f.eval(Y).mid();
              Vector vars = (f.eval(Y) - z).ub();
              A = Matrix({{vars[0], 0, 0}, {0, vars[1], 0}, {0, 0, vars[2]}});
            }

          output.push_back(Parallelepiped(z, A));

        }
      }
    }

    return output;
  }

  vector<Parallelepiped> PEIBOS(const AnalyticFunction<VectorType>& f, const AnalyticFunction<VectorType>& psi_0, vector<vector<int>> generators , double epsilon)
  {
    return PEIBOS(f, psi_0, generators, epsilon, Vector::zero(psi_0.output_size()));
  }

  vector<Parallelepiped> PEIBOS(const AnalyticFunction<VectorType>& f, const AnalyticFunction<VectorType>& psi_0, vector<vector<int>> generators , double epsilon, Vector offset)
  {
    if (psi_0.output_size()==2)
    {
      return PEIBOS2D(f, psi_0, generators, epsilon, offset);
    }
    else if (psi_0.output_size()==3)
    {
      return PEIBOS3D(f, psi_0, generators, epsilon, offset);
    }
    else
    {
      throw std::invalid_argument("PEIBOS only supports 2D and 3D functions.");
    }
  }

}