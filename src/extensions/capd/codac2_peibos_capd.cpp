/**
 *  codac2_peibos_capd.cpp
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <cassert>
#include "codac2_AnalyticFunction.h"
#include "codac2_peibos_capd.h"
#include "codac2_threading.h"

#include <algorithm>
#include <chrono>

using namespace std;

namespace codac2
{
  std::map<double, std::vector<PEIBOS_CAPD_Result>> PEIBOS(const capd::IMap& i_map, double tf, double dt, const AnalyticFunction<VectorType>& psi_0, const vector<OctaSym>& Sigma, double epsilon, bool verbose)
  {
    return PEIBOS(i_map, tf, dt, psi_0, Sigma, epsilon, Vector::zero(psi_0.output_size()), verbose);
  }

  std::map<double, std::vector<PEIBOS_CAPD_Result>> PEIBOS(const capd::IMap& i_map, double tf, double dt, const AnalyticFunction<VectorType>& psi_0, const vector<OctaSym>& Sigma, double epsilon, const Vector& offset, bool verbose)
  {
    std::vector<double> time_points;
    for (double t = 0.; t <= tf; t += dt)
        time_points.push_back(t);
        
    if (time_points.back() < tf)
        time_points.push_back(tf);

    int m = psi_0.input_size();

    assert_release(offset.size() == psi_0.output_size());
    assert_release(m < psi_0.output_size());
    assert_release(Sigma.size() > 0 && (int) Sigma[0].size() ==  psi_0.output_size());

    auto start_time = std::chrono::high_resolution_clock::now();

    capd::interval initialTime(0.);
    capd::interval finalTime(tf);

    vector<IntervalVector> boxes;
    double true_eps = split(Interval(-1.,1.)*IntervalVector::Ones(m), epsilon, boxes);

    int nthreads = nb_threads();
    std::vector<std::map<double, std::vector<PEIBOS_CAPD_Result>>> thread_outputs(nthreads);

    struct WorkItem { const OctaSym* sigma; const IntervalVector* box; };
    std::vector<WorkItem> work;
    work.reserve(Sigma.size() * boxes.size());
    for (const auto& sigma : Sigma)
        for (const auto& box : boxes)
            work.push_back({&sigma, &box});

    auto worker = [&](int start, int end, int tid) 
    {
      auto& local_output = thread_outputs[tid];

      capd::IMap g(i_map);
      capd::IOdeSolver solver(g, 30);

      capd::ITimeMap timeMap(solver);
      capd::ITimeMap timeMap_punct(solver);

      for (int i = start; i < end; ++i) 
      {
        const auto& sigma = *work[i].sigma;
        const auto& X = *work[i].box;

        PEIBOS_CAPD_Key key{X, psi_0, sigma, offset};

        IntervalVector Y = sigma(psi_0.eval(X)) + offset;
        capd::ITimeMap::SolutionCurve solution(initialTime);
        capd::IVector c = to_capd(Y);
        capd::C1Rect2Set s(c);
        timeMap(finalTime, s, solution);

        auto xc = X.mid();
        auto yc = (sigma(psi_0.eval(xc)) + offset).mid();
        capd::ITimeMap::SolutionCurve solution_punct(initialTime);
        capd::IVector c_punct = to_capd(IntervalVector(yc));
        capd::C1Rect2Set s_punct(c_punct);
        timeMap_punct(finalTime, s_punct, solution_punct);

        for (auto t : time_points) 
        {
            IntervalVector z = to_codac(solution_punct(t));
            IntervalMatrix Jf = to_codac(solution.derivative(t));
            local_output[t].emplace_back(key, z, Jf);
        }
      }
    };

    std::vector<std::thread> threads;
    int chunk_size = (int(work.size()) + nthreads - 1) / nthreads;

    for (int t = 0; t < nthreads; ++t) 
    {
      int start = t * chunk_size;
      int end = std::min(start + chunk_size, (int)work.size());
      if (start >= end) break;
      threads.emplace_back(worker, start, end, t);
    }

    for (auto& th : threads) th.join();

    std::map<double, std::vector<PEIBOS_CAPD_Result>> output;

    for (auto& vec : thread_outputs)
        for (auto t : time_points)
            for (auto& item : vec[t])
                output[t].push_back(item);
       
    
    if (verbose)
    {
      printf("\nPEIBOS statistics:\n");
      printf("------------------\n");
      printf("Real epsilon: %.4f\n", true_eps);
      printf("Number of thread used: %d\n", nthreads);
      std::chrono::duration<double> elapsed = std::chrono::high_resolution_clock::now() - start_time;
      printf("Computation time: %.4fs\n\n", elapsed.count());
    }

    return output;
  }

  std::map<double,std::vector<Parallelepiped>> reach_set(const std::map<double, std::vector<PEIBOS_CAPD_Result>>& peibos_output)
  {
    std::map<double,std::vector<Parallelepiped>> output;

    for (const auto& [time,vec] : peibos_output)
    {
      for (const auto& [key, z, Jf] : vec)
      {
        auto p = parallelepiped_inclusion(z, Jf, Jf.mid(), key.psi_0, key.sigma, key.box);

        output[time].push_back(p);
      }
    }
    
    return output;
  }

}
