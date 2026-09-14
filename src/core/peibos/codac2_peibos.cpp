/** 
 *  codac2_peibos.cpp
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_AnalyticFunction.h"
#include "codac2_OctaSym.h"
#include "codac2_peibos.h"
#include "codac2_peibos_tools.h"
#include "codac2_OctaSym_operator.h"
#include "codac2_threading.h"

#include <algorithm>
#include <chrono>

using namespace codac2;

namespace codac2
{
  Parallelepiped parallelepiped_inclusion(const IntervalVector& Y, const IntervalMatrix& Jf, const Matrix& Jf_tild, const AnalyticFunction<VectorType>& psi_0, const OctaSym& sigma, const IntervalVector& X)
  {
    // Computation of the Jacobian of g = f o sigma(psi_0)
    IntervalMatrix Jg = Jf * (sigma.permutation_matrix().template cast<Interval>()) * psi_0.diff(X);

    Vector z = Y.mid();
    // A is an approximation of the Jacobian of g at the center of X
    Matrix A = (Jf_tild * sigma.permutation_matrix() * (psi_0.diff(X.mid()).mid()));

    // Maximum error computation
    double rho = error_peibos(Y, z, Jg, A, X);

    // Inflation of the parallelepiped
    Matrix A_inf = inflate_flat_parallelepiped(A, X.rad(), rho);

    return Parallelepiped(z, A_inf);
  }

  std::vector<Parallelepiped> PEIBOS(const AnalyticFunction<VectorType>& f, const AnalyticFunction<VectorType>& psi_0, const std::vector<OctaSym>& Sigma, double epsilon, bool verbose)
  {
    return PEIBOS(f, psi_0, Sigma, epsilon, Vector::zero(psi_0.output_size()), verbose);
  }

  std::vector<Parallelepiped> PEIBOS(const AnalyticFunction<VectorType>& f, const AnalyticFunction<VectorType>& psi_0, const std::vector<OctaSym>& Sigma, double epsilon, const Vector& offset, bool verbose)
  {
    Index m = psi_0.input_size();

    assert_release (f.input_size() == psi_0.output_size() && "output size of psi_0 must match input size of f");
    assert_release (offset.size() == psi_0.output_size() && "offset size must match output size of psi_0");
    assert_release (f.output_size() >= f.input_size() && "output size of f must be greater than input size of f");
    assert_release (m < psi_0.output_size());
    assert_release (Sigma.size() > 0 && (int) Sigma[0].size() == psi_0.output_size() && "no generator given or wrong dimension of generator (must match output size of psi_0)");

    auto start_time = std::chrono::high_resolution_clock::now();

    std::vector<IntervalVector> boxes;
    double true_eps = split(IntervalVector::constant(m,{-1,1}), epsilon, boxes);

    int nthreads = nb_threads();
    std::vector<std::vector<Parallelepiped>> thread_outputs(nthreads);

    struct WorkItem { const OctaSym* sigma; const IntervalVector* box; };
    std::vector<WorkItem> work;
    work.reserve(Sigma.size() * boxes.size());
    for (const auto& sigma : Sigma)
        for (const auto& box : boxes)
            work.push_back({&sigma, &box});
    
    std::vector<Parallelepiped> local_output;
    VectorVar x(m);

    auto worker = [&](int start, int end, int tid) 
    {
      auto& local_output = thread_outputs[tid];
      for (int i = start; i < end; ++i) 
      {
        const auto& sigma = *work[i].sigma;
        const auto& X = *work[i].box;

        AnalyticFunction g_i ({x}, f(sigma(psi_0(x))+offset));
        local_output.push_back(g_i.parallelepiped_eval(X));
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

    std::vector<Parallelepiped> output;
    output.reserve(Sigma.size() * boxes.size());

    for (auto& vec : thread_outputs)
        for (auto& el : vec)
            output.emplace_back(std::move(el));


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
}