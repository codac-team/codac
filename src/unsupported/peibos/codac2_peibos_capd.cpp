/** 
 *  codac2_peibos.cpp
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_peibos_capd.h"

using namespace std;
using namespace codac2;

namespace codac2
{

  vector<Parallelepiped> PEIBOS2D(const capd::IMap& gamma, double tf, const AnalyticFunction<VectorType>& psi_0, const vector<vector<int>>& generators , double epsilon, const Vector& offset)
  {
    vector<Parallelepiped> output;

    // CAPD solver setup
    capd::IMap g (gamma);
    capd::IOdeSolver solver(g, 20);
    solver.setAbsoluteTolerance(1e-20);
    solver.setRelativeTolerance(1e-20);

    capd::ITimeMap timeMap(solver);
    capd::ITimeMap timeMap_punc(solver);

    capd::interval initialTime(0.);
    capd::interval finalTime(tf);

    // Generate the symmetries from the generators
    vector<OctaSym> symmetries = generate_symmetries(generators, psi_0);
    for (int i = 0; i < ((int) symmetries.size()); i++)
    {
      OctaSym symmetry = symmetries[i];
      for (double t = -1; t < 1; t += epsilon)
      {

        // To get the flow function and its Jacobian (monodromy matrix) for [x]
        IntervalVector X({{t,t+epsilon}});
        IntervalVector Y = symmetry(psi_0.eval(X)) + offset;

        capd::IMatrix monodromyMatrix(2,2);
        capd::ITimeMap::SolutionCurve solution(initialTime); 
        capd::IVector c(2);
        c[0] = to_capd(Y[0]);
        c[1] = to_capd(Y[1]);
        capd::C1Rect2Set s(c);
        timeMap(finalTime, s, solution);
        capd::IVector result = timeMap(finalTime, s, monodromyMatrix);
        IntervalMatrix JJf=to_codac(monodromyMatrix);

        // To get the flow function and its Jacobian (monodromy matrix) for x_hat
        auto xc = X.mid();
        auto yc = (symmetry(psi_0.eval(xc)) + offset).mid();

        capd::IMatrix monodromyMatrix_punc(2,2);
        capd::ITimeMap::SolutionCurve solution_punct(initialTime);
        capd::IVector c_punct(2);

        c_punct[0] = to_capd(yc[0]);
        c_punct[1] = to_capd(yc[1]);
        capd::C1Rect2Set s_punct(c_punct);
        timeMap_punc(finalTime, s_punct, solution_punct);      
        capd::IVector result_punct = timeMap_punc(finalTime, s_punct, monodromyMatrix_punc);
        IntervalMatrix JJf_punc=to_codac(monodromyMatrix_punc);

        // Center of the parallelepiped
        Vector z = Vector(to_codac(result).mid());
        
        // Maximum error computation
        double rho = error( JJf, JJf_punc, psi_0, symmetry, X);

        IntervalMatrix Jz = (JJf_punc * IntervalMatrix(symmetry.permutation_matrix()) * psi_0.diff(xc)).mid();

        // Inflation of the parallelepiped
        Matrix A = inflate_flat_parallelepiped(Jz.mid(), epsilon, rho);

        output.push_back(Parallelepiped(z, A));

      }
    }
    return output;
  }

  vector<Parallelepiped> PEIBOS3D(const capd::IMap& gamma, double tf, const AnalyticFunction<VectorType>& psi_0, const vector<vector<int>>& generators , double epsilon, const Vector& offset)
  {
    vector<Parallelepiped> output;
    
    // CAPD solver setup
    capd::IMap g (gamma);
    capd::IOdeSolver solver(g, 20);

    
    solver.setAbsoluteTolerance(1e-20);
    solver.setRelativeTolerance(1e-20);

    capd::ITimeMap timeMap(solver);
    capd::ITimeMap timeMap_punc(solver);

    capd::interval initialTime(0.);
    capd::interval finalTime(tf);

    // Generate the symmetries from the generators
    vector<OctaSym> symmetries = generate_symmetries(generators, psi_0);
    for (int i = 0; i < ((int) symmetries.size()); i++)
    {
      OctaSym symmetry = symmetries[i];
      for (double t1 = -1; t1 < 1; t1 += epsilon)
      {
        for (double t2 = -1;t2 < 1; t2+=epsilon)
        {

          // To get the flow function and its Jacobian (monodromy matrix) for [x]
          IntervalVector X({{t1,t1+epsilon},{t2,t2+epsilon}});
          IntervalVector Y = symmetry(psi_0.eval(X)) + offset;

          capd::IMatrix monodromyMatrix(3,3);
          capd::ITimeMap::SolutionCurve solution(initialTime); 
          capd::IVector c(3);
          c[0] = to_capd(Y[0]);
          c[1] = to_capd(Y[1]);
          c[2] = to_capd(Y[2]);
          capd::C1Rect2Set s(c);
          timeMap(finalTime, s, solution);
          capd::IVector result = timeMap(finalTime, s, monodromyMatrix);
          IntervalMatrix JJf=to_codac(monodromyMatrix);

          // To get the flow function and its Jacobian (monodromy matrix) for x_hat
          auto xc = X.mid();
          auto yc = (symmetry(psi_0.eval(xc)) + offset).mid();

          capd::IMatrix monodromyMatrix_punc(3,3);
          capd::ITimeMap::SolutionCurve solution_punct(initialTime);
          capd::IVector c_punct(3);

          c_punct[0] = to_capd(yc[0]);
          c_punct[1] = to_capd(yc[1]);
          c_punct[2] = to_capd(yc[2]);
          capd::C1Rect2Set s_punct(c_punct);
          timeMap_punc(finalTime, s_punct, solution_punct);      
          capd::IVector result_punct = timeMap_punc(finalTime, s_punct, monodromyMatrix_punc);
          IntervalMatrix JJf_punc=to_codac(monodromyMatrix_punc);

          // Center of the parallelepiped
          Vector z = Vector(to_codac(result).mid());
          
          // Maximum error computation
          double rho = error( JJf, JJf_punc, psi_0, symmetry, X);

          IntervalMatrix Jz = (JJf_punc * IntervalMatrix(symmetry.permutation_matrix()) * psi_0.diff(xc)).mid();

          // Inflation of the parallelepiped
          Matrix A = inflate_flat_parallelepiped(Jz.mid(), epsilon, rho);

          output.push_back(Parallelepiped(z, A));

        }
      }
    }
    return output;
  }

  vector<Parallelepiped> PEIBOS(const capd::IMap& gamma, double tf, const AnalyticFunction<VectorType>& psi_0, const vector<vector<int>>& generators , double epsilon)
  {
    return PEIBOS(gamma, tf, psi_0, generators, epsilon, Vector::zero(psi_0.output_size()));
  }

  vector<Parallelepiped> PEIBOS(const capd::IMap& gamma, double tf, const AnalyticFunction<VectorType>& psi_0, const vector<vector<int>>& generators , double epsilon, const Vector& offset)
  {
    if (psi_0.output_size()==2)
    {
      return PEIBOS2D(gamma, tf, psi_0, generators, epsilon, offset);
    }
    else if (psi_0.output_size()==3)
    {
      return PEIBOS3D(gamma, tf, psi_0, generators, epsilon, offset);
    }
    else
    {
      throw std::invalid_argument("PEIBOS only supports 2D and 3D functions.");
    }
  }

}