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
  Solver::Solver(const Vector& max_thickness, float refining_fxpt_ratio, float propa_fxpt_ratio, float cid_fxpt_ratio)
  {
    m_max_thickness = max_thickness;
    m_refining_fxpt_ratio = refining_fxpt_ratio;
    m_propa_fxpt_ratio = propa_fxpt_ratio;
    m_cid_fxpt_ratio = cid_fxpt_ratio;

    // Embedded graphics
    vibes::beginDrawing();
    m_fig = new VibesFigure_Tube("Solver", max_thickness.size());
    m_fig->set_properties(100,100,700,500);
  }

  Solver::~Solver()
  {
    delete m_fig;
    vibes::endDrawing();
  }

  const list<TubeVector> Solver::solve(const TubeVector& x0, void (*ctc_func)(TubeVector&))
  {
    int i = 0;
    clock_t t_start = clock();
    m_fig->show(true);

    // todo: check dim max_thickness vector
    stack<TubeVector> s;
    s.push(x0);
    list<TubeVector> l_solutions;

    while(!s.empty())
    {
      TubeVector x = s.top();
      s.pop();

      bool emptiness;
      double volume_before_refining;

      do
      {
        volume_before_refining = x.volume();

        // 1. Refining

          double t_refining = x.get_wider_slice()->domain().mid();
          x.sample(t_refining);

        // 2. Propagations up to the fixed point

          propagation(x, ctc_func, m_propa_fxpt_ratio);

        // 3. CID up to the fixed point
      
          emptiness = x.is_empty();
          if(!emptiness)
          {
            cid(x, ctc_func);
            emptiness = x.is_empty();
          }

      } while(!emptiness
           && !stopping_condition_met(x)
           && !fixed_point_reached(x.dim(), volume_before_refining, x.volume(), m_refining_fxpt_ratio));

      // 4. Bisection

        if(!emptiness)
        {
          if(stopping_condition_met(x))
          {
            l_solutions.push_back(x);

            // Displaying solution
            ostringstream o; o << "solution_" << i;
            m_fig->add_tube(&l_solutions.back(), o.str());
            m_fig->show(true);
            i++;
          }

          else
          {
            cout << "Bisection..." << endl;
            double t_bisection = x.get_largest_slice()->domain().mid();
            pair<TubeVector,TubeVector> p_x = x.bisect(t_bisection);
            s.push(p_x.first);
            s.push(p_x.second);
          }
        }

      cout << "\rsolutions: " << l_solutions.size() << "  (" << (int)((double)(clock() - t_start)/CLOCKS_PER_SEC) << "s)   " << flush;
    }

    cout << endl;
    printf("Time taken: %.2fs\n", (double)(clock() - t_start)/CLOCKS_PER_SEC);
    if(l_solutions.size() == 0)
      cout << "no solution found" << endl;

    int j = 0;
    list<TubeVector>::iterator it;
    for(it = l_solutions.begin(); it != l_solutions.end(); ++it)
    {
      j++;
      cout << j << ": "
           << *it <<  ", tf↦" << (*it)[it->domain().ub()]
           << " (max thickness: " << it->max_thickness() << ")"
           << endl;
    }

    return l_solutions;
  }

  VibesFigure_Tube* Solver::figure()
  {
    return m_fig;
  }

  bool Solver::stopping_condition_met(const TubeVector& x)
  {
    Vector x_max_thickness = x.max_thickness();
    for(int i = 0 ; i < x.dim() ; i++)
      if(x_max_thickness[i] > m_max_thickness[i])
        return false;
    return true;
  }

  bool Solver::fixed_point_reached(int n, double volume_before, double volume_after, float fxpt_ratio)
  {
    return (pow(volume_after, 1./n) / pow(volume_before, 1./n)) > (1. - fxpt_ratio);
  }

  void Solver::propagation(TubeVector &x, void (*ctc_func)(TubeVector&), float propa_fxpt_ratio)
  {
    bool emptiness;
    double volume_before_ctc;

    do
    {
      volume_before_ctc = x.volume();
      ctc_func(x);
      emptiness = x.is_empty();
    } while(!emptiness
         && !stopping_condition_met(x)
         && !fixed_point_reached(x.dim(), volume_before_ctc, x.volume(), propa_fxpt_ratio));
  }

  void Solver::cid(TubeVector &x, void (*ctc_func)(TubeVector&))
  {
    double t_bisection;
    x.max_gate_thickness(t_bisection);
    pair<TubeVector,TubeVector> p_x = x.bisect(t_bisection);
    pair<TubeVector,TubeVector> p_x_1 = p_x.first.bisect(t_bisection);
    pair<TubeVector,TubeVector> p_x_2 = p_x.second.bisect(t_bisection);
    x.set_empty();

    stack<TubeVector> s;
    //s.push(p_x.first);
    //s.push(p_x.second);
    s.push(p_x_1.first);
    s.push(p_x_1.second);
    s.push(p_x_2.first);
    s.push(p_x_2.second);

    while(!s.empty())
    {
      TubeVector branch_x = s.top();
      s.pop();
      propagation(branch_x, ctc_func, m_cid_fxpt_ratio);
      x |= branch_x;
    }
  }
  
  bool Solver::solution_encloses(const list<TubeVector>& l_solutions, const TrajectoryVector& truth)
  {
    list<TubeVector>::const_iterator it;
    for(it = l_solutions.begin(); it != l_solutions.end(); ++it)
      if(it->encloses(truth))
        return true;
    return false;
  }
}