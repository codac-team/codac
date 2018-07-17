/* ============================================================================
 *  tubex-lib - Solver class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2018
 * ---------------------------------------------------------------------------- */

#include <time.h>
#include "tubex_Solver.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  Solver::Solver()
  {

  }

  const vector<TubeVector> Solver::solve(const TubeVector& x0,
                                         void (*ctc_func)(TubeVector&),
                                         const Vector& max_thickness,
                                         float refining_ratio,
                                         float fixed_point_ratio)
  {
    clock_t t_start = clock();

    // todo: check dim max_thickness vector
    stack<TubeVector> s;
    s.push(x0);
    vector<TubeVector> v_solutions;
    clock_t tStart = clock();

    while(!s.empty())
    {
      TubeVector x = s.top();
      s.pop();

      bool emptiness;
      double volume, volume_before_refining;

      do
      {
        volume_before_refining = x.volume();

        // 1. Refining

          double t_refining = x.get_wider_slice()->domain().mid();
          x.sample(t_refining);

        // 2. Propagations up to the fixed point

          double volume_before_ctc;

          do
          {
            volume_before_ctc = x.volume();
            ctc_func(x);
            //cid(x, ctc_func);
            emptiness = x.is_empty();
            volume = x.volume();
          } while(!emptiness && (volume / volume_before_ctc) < (1. - fixed_point_ratio));

      } while(!emptiness && (volume / volume_before_refining) < (1. - refining_ratio));

      // 3. Bisection

        if(!emptiness)
        {
          int first_id_max_thickness;
          Vector x_max_thickness = x.max_thickness(first_id_max_thickness);
          double t_bisection = x.get_slice(first_id_max_thickness)->domain().mid();

          bool is_thin_enough = true;
          for(int i = 0 ; i < x.dim() ; i++)
            is_thin_enough &= x_max_thickness[i] < max_thickness[i];

          if(is_thin_enough)
            v_solutions.push_back(x);

          else
          {
            pair<TubeVector,TubeVector> p_x = x.bisect(t_bisection);
            s.push(p_x.first);
            s.push(p_x.second);
          }
        }

      cout << "\rsolutions: " << v_solutions.size() << "  (" << (int)((double)(clock() - tStart)/CLOCKS_PER_SEC) << "s)   " << flush;
    }

    cout << endl;
    printf("Time taken: %.2fs\n", (double)(clock() - t_start)/CLOCKS_PER_SEC);
    if(v_solutions.size() == 0)
      cout << "no solution found" << endl;

    return v_solutions;
  }

  void Solver::cid(TubeVector &x, void (*ctc_func)(TubeVector&))
  {
    if(!x.is_empty())
    {
      double t_bisection;
      x.max_gate_thickness(t_bisection);
      pair<TubeVector,TubeVector> p_x = x.bisect(t_bisection);
      ctc_func(p_x.first);
      ctc_func(p_x.second);
      x = p_x.first;
      x |= p_x.second;
    }
  }
  
  bool Solver::solution_encloses(const vector<TubeVector>& v_solutions, const TrajectoryVector& truth)
  {
    for(int i = 0 ; i < v_solutions.size() ; i++)
      if(v_solutions[i].encloses(truth))
        return true;
    return false;
  }
}