/** 
 *  codac2_pave.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_pave.h"
#include <chrono>
#include <mutex>
#include <condition_variable>

using namespace std;
using namespace codac2;

class SharedTreeDataInOut 
{
  public:
      std::list<std::shared_ptr<PavingInOut_Node>> workList;

      std::mutex mutex;
      std::condition_variable cv;

      int activeWorkers = 0;

      bool finished = false;

      SharedTreeDataInOut(std::shared_ptr<PavingInOut_Node> input)
      {
        workList.push_back(input);
      }
};

class SharedTreeDataOut 
{
  public:
      std::list<std::shared_ptr<PavingOut_Node>> workList;

      std::mutex mutex;
      std::condition_variable cv;

      int activeWorkers = 0;

      bool finished = false;

      SharedTreeDataOut(std::shared_ptr<PavingOut_Node> input)
      {
        workList.push_back(input);
      }
};

namespace codac2
{
  PavingOut pave(const IntervalVector& x0, std::shared_ptr<const CtcBase<IntervalVector>> c,
    double eps, bool verbose)
  {
    return pave(x0, *c, eps, verbose);
  }

  PavingOut pave(const IntervalVector& x0, const CtcBase<IntervalVector>& c, double eps, bool verbose)
  {
    double time = 0;
    return pave(x0, c, eps, time, verbose);
  }

  PavingOut pave(const IntervalVector& x0, const CtcBase<IntervalVector>& c, double eps, double& time, bool verbose)
  {
    if (nb_threads()==1)
      return pave_monothread(x0, c, eps, time, verbose);
    else
      return pave_multithread(x0, c, eps, time, verbose);
  }

  PavingOut pave_monothread(const IntervalVector& x0, const CtcBase<IntervalVector>& c, double eps, double& time, bool verbose)
  {
    assert_release(eps > 0.);
    assert_release(!x0.is_empty());
    
    clock_t t_start = clock();
    Index n_boundary = 0;

    PavingOut p(x0);
    // In order to be able to reconstruct the initial box, the first level represents the
    // initial domain x0 (the left node is x0, the right one is an empty box).
    p.tree()->bisect();
    p.tree()->left()->boxes() = { x0 };
    get<0>(p.tree()->right()->boxes()).set_empty();

    std::shared_ptr<PavingOut_Node> n;
    list<std::shared_ptr<PavingOut_Node>> l { p.tree()->left() };

    while(!l.empty())
    {
      n = l.front();
      l.pop_front();

      c.contract(get<0>(n->boxes()));

      if(!get<0>(n->boxes()).is_empty())
      {
        if(get<0>(n->boxes()).max_diam() > eps)
        {
          n->bisect();
          l.push_back(n->left());
          l.push_back(n->right());
        }
        
        else if(verbose)
          n_boundary++;
      }
    }

    time = (double)(clock()-t_start)/CLOCKS_PER_SEC;

    if(verbose)
      printf("Computation time: %.4fs, %ld boxes\n", time, n_boundary);
    return p;
  }
  
  PavingOut pave_multithread(const IntervalVector& x0, const CtcBase<IntervalVector>& c, double eps, double& time, bool verbose)
  {
    assert_release(eps > 0.);
    assert_release(!x0.is_empty());
    
    auto start_time = std::chrono::high_resolution_clock::now();

    int nthreads = nb_threads();

    PavingOut p(x0);
    // In order to be able to reconstruct the initial box, the first level represents the
    // initial domain x0 (the left node is x0, the right one is an empty box).
    p.tree()->bisect();
    p.tree()->left()->boxes() = { x0 };
    get<0>(p.tree()->right()->boxes()).set_empty();

    SharedTreeDataOut shared_tree_data(p.tree()->left());

    auto worker = [&](SharedTreeDataOut& tree_data) 
    {
      while (true) 
      {
        std::shared_ptr<PavingOut_Node> n;
        {
            std::unique_lock<std::mutex> lock(tree_data.mutex);

            tree_data.cv.wait(lock, [&]() 
            {
              return !tree_data.workList.empty() || tree_data.finished;
            });

            if (tree_data.finished && tree_data.workList.empty())
                return;

            n = tree_data.workList.front();
            tree_data.workList.pop_front();

            tree_data.activeWorkers++;
        }

        c.contract(get<0>(n->boxes()));

        if(!get<0>(n->boxes()).is_empty())
        {
          if(get<0>(n->boxes()).max_diam() > eps)
          {
            n->bisect();
            {
              std::unique_lock<std::mutex> lock(tree_data.mutex);

              tree_data.workList.push_back(n->left());
              tree_data.workList.push_back(n->right());
            }
          }
        }

        {
          std::unique_lock<std::mutex> lock(tree_data.mutex);
          tree_data.activeWorkers--;
          if (tree_data.workList.empty() && tree_data.activeWorkers == 0) 
          {
            tree_data.finished = true;
          }
        }
        tree_data.cv.notify_all();
      }
    };

    std::vector<std::thread> threads;
    for (int tid = 0; tid < nthreads; tid++)
      threads.emplace_back(worker, std::ref(shared_tree_data));      

    for (auto& th : threads) th.join();

    std::chrono::duration<double> elapsed = std::chrono::high_resolution_clock::now() - start_time;

    time = elapsed.count();

    if(verbose)
    {
      printf("Number of thread used: %d\n", nthreads);
      printf("Computation time: %.4fs\n", time);    
    }

    return p;
  }

  PavingInOut pave(const IntervalVector& x0, std::shared_ptr<const SepBase> s,
    double eps, bool verbose)
  {
    return pave(x0, *s, eps, verbose);
  }

  PavingInOut pave(const IntervalVector& x0, const SepBase& s, double eps, bool verbose)
  {
    if (nb_threads()==1)
      return pave_monothread(x0, s, eps, verbose);
    else
      return pave_multithread(x0, s, eps, verbose);
  }

  PavingInOut pave_monothread(const IntervalVector& x0, const SepBase& s, double eps, bool verbose)
  {
    assert_release(eps > 0.);
    assert_release(!x0.is_empty());
    
    clock_t t_start = clock();

    PavingInOut p(x0);
    std::shared_ptr<PavingInOut_Node> n;
    list<std::shared_ptr<PavingInOut_Node>> l { p.tree() };

    while(!l.empty())
    {
      n = l.front();
      l.pop_front();

      auto xs = s.separate(get<0>(n->boxes()));
      auto boundary = (xs.inner & xs.outer);
      n->boxes() = { xs.outer, xs.inner };

      if(!boundary.is_empty() && boundary.max_diam() > eps)
      {
        n->bisect();
        l.push_back(n->left());
        l.push_back(n->right());
      }
    }

    if(verbose)
      printf("Computation time: %.4fs\n", (double)(clock()-t_start)/CLOCKS_PER_SEC);
    return p;
  }

  PavingInOut pave_multithread(const IntervalVector& x0, const SepBase& s, double eps, bool verbose)
  {
    assert_release(eps > 0.);
    assert_release(!x0.is_empty());
    
    auto start_time = std::chrono::high_resolution_clock::now();

    int nthreads = nb_threads();

    PavingInOut p(x0);
    SharedTreeDataInOut shared_tree_data(p.tree());

    auto worker = [&](SharedTreeDataInOut& tree_data) 
    {
      while (true) 
      {
        std::shared_ptr<PavingInOut_Node> n;
        {
            std::unique_lock<std::mutex> lock(tree_data.mutex);

            tree_data.cv.wait(lock, [&]() 
            {
              return !tree_data.workList.empty() || tree_data.finished;
            });

            if (tree_data.finished && tree_data.workList.empty())
                return;

            n = tree_data.workList.front();
            tree_data.workList.pop_front();

            tree_data.activeWorkers++;
        }

        auto xs = s.separate(get<0>(n->boxes()));
        auto boundary = (xs.inner & xs.outer);
        n->boxes() = { xs.outer, xs.inner };

        if(!boundary.is_empty() && boundary.max_diam() > eps)
        {
          n->bisect();
          {
            std::unique_lock<std::mutex> lock(tree_data.mutex);

            tree_data.workList.push_back(n->left());
            tree_data.workList.push_back(n->right());
          }
        }
        {
          std::unique_lock<std::mutex> lock(tree_data.mutex);
          tree_data.activeWorkers--;
          if (tree_data.workList.empty() && tree_data.activeWorkers == 0) 
          {
            tree_data.finished = true;
          }
        }
        tree_data.cv.notify_all();
      }
    };

    std::vector<std::thread> threads;
    for (int tid = 0; tid < nthreads; tid++)
      threads.emplace_back(worker, std::ref(shared_tree_data));      

    for (auto& th : threads) th.join();

    if(verbose)
    {
      printf("Number of thread used: %d\n", nthreads);
      std::chrono::duration<double> elapsed = std::chrono::high_resolution_clock::now() - start_time;
      printf("Computation time: %.4fs\n", elapsed.count());    
    }
    return p;
  }

  PavingInOut regular_pave(const IntervalVector& x0,
    const std::function<BoolInterval(const IntervalVector&)>& test,
    double eps, bool verbose)
  {
    assert_release(eps > 0.);
    assert_release(!x0.is_empty());

    clock_t t_start = clock();

    PavingInOut p(x0);
    std::list<std::shared_ptr<PavingInOut_Node>> l { p.tree() };

    while(!l.empty())
    {
      auto n = l.front();
      l.pop_front();

      assert(n->is_leaf());
      auto b = test(std::get<1>(n->boxes()));

      switch(b)
      {
        case BoolInterval::TRUE:
          std::get<1>(n->boxes()).set_empty();
          break;

        case BoolInterval::FALSE:
          std::get<0>(n->boxes()).set_empty();
          break;

        default:
          if(n->unknown().max_diam() > eps)
          {
            n->bisect();
            l.push_back(n->left());
            l.push_back(n->right());
          }
      }
    }

    if(verbose)
      printf("Computation time: %.4fs\n", (double)(clock()-t_start)/CLOCKS_PER_SEC);
    return p;
  }

  PavingInOut regular_pave_multithread(const IntervalVector& x0,
    const std::function<BoolInterval(const IntervalVector&)>& test,
    double eps, bool verbose)
  {
    assert_release(eps > 0.);
    assert_release(!x0.is_empty());

    auto start_time = std::chrono::high_resolution_clock::now();

    int nthreads = nb_threads();

    PavingInOut p(x0);

    SharedTreeDataInOut shared_tree_data(p.tree());

    auto worker = [&](SharedTreeDataInOut& tree_data) 
    {
      while (true) 
      {
        std::shared_ptr<PavingInOut_Node> n;
        {
            std::unique_lock<std::mutex> lock(tree_data.mutex);

            tree_data.cv.wait(lock, [&]() 
            {
              return !tree_data.workList.empty() || tree_data.finished;
            });

            if (tree_data.finished && tree_data.workList.empty())
                return;

            n = tree_data.workList.front();
            tree_data.workList.pop_front();

            tree_data.activeWorkers++;
        }

        auto b = test(std::get<1>(n->boxes()));
        switch(b)
        {
          case BoolInterval::TRUE:
            std::get<1>(n->boxes()).set_empty();
            break;

          case BoolInterval::FALSE:
            std::get<0>(n->boxes()).set_empty();
            break;

          default:
            if(n->unknown().max_diam() > eps)
            {
              n->bisect();
              {
                std::unique_lock<std::mutex> lock(tree_data.mutex);

                tree_data.workList.push_back(n->left());
                tree_data.workList.push_back(n->right());
              }
            }
        }
        {
          std::unique_lock<std::mutex> lock(tree_data.mutex);
          tree_data.activeWorkers--;
          if (tree_data.workList.empty() && tree_data.activeWorkers == 0) 
          {
            tree_data.finished = true;
          }
        }
        tree_data.cv.notify_all();
      }
    };

    std::vector<std::thread> threads;
    for (int tid = 0; tid < nthreads; tid++)
      threads.emplace_back(worker,std::ref(shared_tree_data));      

    for (auto& th : threads) th.join();

    if (verbose)
    {
      printf("Number of thread used: %d\n", nthreads);
      std::chrono::duration<double> elapsed = std::chrono::high_resolution_clock::now() - start_time;
      printf("Computation time: %.4fs\n", elapsed.count());
    }

    return p;
  }

  PavingInOut pave_tube(const IntervalVector& x0, const SlicedTube<IntervalVector>& f, double eps, bool verbose)
  {
    return regular_pave(x0,
      [&f](const IntervalVector& x) -> BoolInterval
      {
        bool is_out = true;
        for(const auto& s : f)
        {
          if(!s.is_gate() && s.codomain().intersects(x))
          {
            is_out = false;
            if(s.codomain().is_superset(x))
              return BoolInterval::TRUE;
          }
        }

        if(is_out)
          return BoolInterval::FALSE;
        return BoolInterval::UNKNOWN;
      },
      eps, verbose);
  }
}