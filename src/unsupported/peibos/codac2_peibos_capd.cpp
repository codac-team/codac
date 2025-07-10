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
  IMapWrapper::IMapWrapper(const string& s) : gamma(s)
  {}

  void IMapWrapper::setParameter(const string& name, double value)
  {
    gamma.setParameter(name, value);
  }

  void IMapWrapper::setParameter(const string& name, Interval value)
  {
    gamma.setParameter(name, to_capd(value));
  }

  vector<Parallelepiped> PEIBOS(const IMapWrapper& i_map_wrapper, double tf, const AnalyticFunction<VectorType>& psi_0, const vector<vector<int>>& generators , double epsilon, bool verbose)
  {
    return PEIBOS(i_map_wrapper, tf, psi_0, generators, epsilon, Vector::zero(psi_0.output_size()), verbose);
  }

  vector<Parallelepiped> PEIBOS(const IMapWrapper& i_map_wrapper, double tf, const AnalyticFunction<VectorType>& psi_0, const vector<vector<int>>& generators , double epsilon, const Vector& offset, bool verbose)
  {
    int m = psi_0.input_size();
    int n = psi_0.output_size();

    assert(offset.size() == n);
    assert (m < n);
    assert (generators.size() > 0 && (int) generators[0].size() == n);

    clock_t t_start = clock();

    vector<Parallelepiped> output;
    
    // CAPD solver setup
    capd::IMap g (i_map_wrapper.gamma);
    capd::IOdeSolver solver(g, 20);

    
    solver.setAbsoluteTolerance(1e-20);
    solver.setRelativeTolerance(1e-20);

    capd::ITimeMap timeMap(solver);
    capd::ITimeMap timeMap_punc(solver);

    capd::interval initialTime(0.);
    capd::interval finalTime(tf);

    // Generate the symmetries from the generators
    vector<OctaSym> symmetries = generate_symmetries(generators, psi_0);
    vector<IntervalVector> boxes;
    double true_eps = split(Interval(-1.,1.)*IntervalVector::Ones(m), epsilon, boxes);
    
    for (const auto& symmetry : symmetries)
    {
      for (const auto& X : boxes)
      {

        // To get the flow function and its Jacobian (monodromy matrix) for [x]
        IntervalVector Y = symmetry(psi_0.eval(X)) + offset;

        capd::IMatrix monodromyMatrix(n,n);
        capd::ITimeMap::SolutionCurve solution(initialTime); 
        capd::IVector c =to_capd(Y);

        capd::C1Rect2Set s(c);
        timeMap(finalTime, s, solution);
        capd::IVector result = timeMap(finalTime, s, monodromyMatrix);
        auto JJf=to_codac(monodromyMatrix);

        // To get the flow function and its Jacobian (monodromy matrix) for x_hat
        auto xc = X.mid();
        auto yc = (symmetry(psi_0.eval(xc)) + offset).mid();

        capd::IMatrix monodromyMatrix_punc(n,n);
        capd::ITimeMap::SolutionCurve solution_punct(initialTime);
        capd::IVector c_punct =to_capd(IntervalVector(yc));

        capd::C1Rect2Set s_punct(c_punct);
        timeMap_punc(finalTime, s_punct, solution_punct);      
        capd::IVector result_punct = timeMap_punc(finalTime, s_punct, monodromyMatrix_punc);
        auto JJf_point=to_codac(monodromyMatrix_punc);

        // Center of the parallelepiped
        Vector z = Vector(to_codac(result).mid());
        
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

  map<double,vector<Parallelepiped>> PEIBOS(const IMapWrapper& i_map_wrapper, double tf, double dt, const AnalyticFunction<VectorType>& psi_0, const vector<vector<int>>& generators , double epsilon, bool verbose)
  {
    return PEIBOS(i_map_wrapper, tf, dt, psi_0, generators, epsilon, Vector::zero(psi_0.output_size()), verbose);
  }

  map<double,vector<Parallelepiped>> PEIBOS(const IMapWrapper& i_map_wrapper, double tf, double dt, const AnalyticFunction<VectorType>& psi_0, const vector<vector<int>>& generators , double epsilon, const Vector& offset, bool verbose)
  {
    map<double,vector<Parallelepiped>> output;

    for (double t=0.;t<=tf;t+=dt)
      output[t] = vector<Parallelepiped>();
     
    int m = psi_0.input_size();
    int n = psi_0.output_size();

    assert(offset.size() == n);
    assert (m < n);
    assert (generators.size() > 0 && (int) generators[0].size() == n);

    clock_t t_start = clock();

    // CAPD solver setup
    capd::IMap g (i_map_wrapper.gamma);
    capd::IOdeSolver solver(g, 20);

    
    solver.setAbsoluteTolerance(1e-20);
    solver.setRelativeTolerance(1e-20);

    capd::ITimeMap timeMap(solver);
    capd::ITimeMap timeMap_punc(solver);

    capd::interval initialTime(0.);
    capd::interval finalTime(tf);

    // Generate the symmetries from the generators
    vector<OctaSym> symmetries = generate_symmetries(generators, psi_0);
    vector<IntervalVector> boxes;
    double true_eps = split(Interval(-1.,1.)*IntervalVector::Ones(m), epsilon, boxes);
    
    for (const auto& symmetry : symmetries)
    {
      for (const auto& X : boxes)
      {
        vector<Parallelepiped> to_add;

        // To get the flow function and its Jacobian (monodromy matrix) for [x]
        IntervalVector Y = symmetry(psi_0.eval(X)) + offset;

        capd::IMatrix monodromyMatrix(n,n);
        capd::ITimeMap::SolutionCurve solution(initialTime); 
        capd::IVector c =to_capd(Y);

        capd::C1Rect2Set s(c);
        timeMap(finalTime, s, solution);

        auto xc = X.mid();
        auto yc = (symmetry(psi_0.eval(xc)) + offset).mid();

        capd::ITimeMap::SolutionCurve solution_punct(initialTime);
        capd::IVector c_punct =to_capd(IntervalVector(yc));

        capd::C1Rect2Set s_punct(c_punct);
        timeMap_punc(finalTime, s_punct, solution_punct);   

        for (double t=0.;t<=tf;t+=dt)
        {
          auto result = solution(t);
          auto JJf = to_codac(solution.derivative(t));

          auto result_punct = solution_punct(t);
          auto JJf_point = to_codac(solution_punct.derivative(t));

          // Center of the parallelepiped
          Vector z = Vector(to_codac(result).mid());
          
          auto p = parallelepiped_inclusion(z, JJf, JJf_point, psi_0, symmetry, X, true_eps);

          output[t].push_back(p);
        }
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