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

#define GRAPHICS 1

using namespace std;
using namespace ibex;

namespace tubex
{
  Solver::Solver(const Vector& max_thickness)
  {
    m_max_thickness = max_thickness;

    #if GRAPHICS // embedded graphics
      vibes::beginDrawing();
      m_fig = new VibesFigure_TubeVector("Solver");
      m_fig->set_properties(100, 100, 700, 500);
    #endif
  }

  Solver::~Solver()
  {
    #if GRAPHICS
      delete m_fig;
      vibes::endDrawing();
    #endif
  }

  void Solver::set_refining_fxpt_ratio(float refining_fxpt_ratio)
  {
    assert(Interval(0.,1.).contains(refining_fxpt_ratio));
    m_refining_fxpt_ratio = refining_fxpt_ratio;
  }

  void Solver::set_propa_fxpt_ratio(float propa_fxpt_ratio)
  {
    assert(Interval(0.,1.).contains(propa_fxpt_ratio));
    m_propa_fxpt_ratio = propa_fxpt_ratio;
  }

  void Solver::set_cid_fxpt_ratio(float cid_fxpt_ratio)
  {
    assert(Interval(0.,1.).contains(cid_fxpt_ratio));
    m_cid_fxpt_ratio = cid_fxpt_ratio;
  }

  const list<TubeVector> Solver::solve(const TubeVector& x0, void (*ctc_func)(TubeVector&))
  {
    assert(x0.size() == m_max_thickness.size());

    int i = 0;
    clock_t t_start = clock();

    #if GRAPHICS
      m_fig->show(true);
    #endif

    int prev_level = 0;
    list<pair<int,TubeVector> > s;
    s.push_back(make_pair(0, x0));
    list<TubeVector> l_solutions;

    while(!s.empty())
    {
      int level = s.front().first;
      if(level != prev_level && s.size() >= 8)
      {
        cout << "clustering (" << s.size() << " items)" << endl;
        //clustering(s);
        prev_level = level;
      }

      TubeVector x = s.front().second;
      s.pop_front();

      bool emptiness;
      double volume_before_refining;
      
      do
      {
        volume_before_refining = x.volume();

        // 1. Refining

          if(m_refining_fxpt_ratio != 0.)
          {
            double t_refining = x[0].wider_slice()->domain().mid();
            x.sample(t_refining);
          }

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
           && !fixed_point_reached(volume_before_refining, x.volume(), m_refining_fxpt_ratio));

      // 4. Bisection

        if(!emptiness)
        {
          if(stopping_condition_met(x))
          {
            l_solutions.push_back(x);

            #if GRAPHICS // displaying solution
              i++;
              ostringstream o; o << "solution_" << i;
              m_fig->add_tubevector(&l_solutions.back(), o.str());
              m_fig->show(true);
            #endif
          }

          else
          {
            cout << "Bisection... (level " << level << ")" << endl;
            double t_bisection = x[0].largest_slice()->domain().mid();
            pair<TubeVector,TubeVector> p_x = x.bisect(t_bisection);
            level++; // deeper
            s.push_back(make_pair(level, p_x.first));
            s.push_back(make_pair(level, p_x.second));
          }
        }

      cout << "\rSolutions: " << l_solutions.size() << "  (" << (int)((double)(clock() - t_start)/CLOCKS_PER_SEC) << "s)   " << flush;
    }

    cout << endl;
    printf("Time taken: %.2fs\n", (double)(clock() - t_start)/CLOCKS_PER_SEC);

    int j = 0;
    list<TubeVector>::iterator it;
    for(it = l_solutions.begin(); it != l_solutions.end(); ++it)
    {
      j++;
      cout << "  " << j << ": "
           << *it <<  ", tf↦" << (*it)(it->domain().ub())
           << " (max thickness: " << it->max_thickness() << ")"
           << endl;
    }

    return l_solutions;
  }

  void Solver::clustering(list<pair<int,TubeVector> >& l_tubes)
  {
    assert(!l_tubes.empty());

    list<pair<int,TubeVector> > l_clustered;
    list<pair<int,TubeVector> >::iterator it1, it2;
    for(it1 = l_tubes.begin(); it1 != l_tubes.end(); ++it1)
    {
      bool clustering = false;
      for(it2 = l_clustered.begin(); it2 != l_clustered.end(); ++it2)
      {
        if(it1->second.overlaps(it2->second, 0.9))
        {
          it2->second = it2->second | it1->second;
          clustering = true;
        }
      }

      if(!clustering)
        l_clustered.push_back(*it1);
    }

    l_tubes = l_clustered;
  }

  VibesFigure_TubeVector* Solver::figure()
  {
    return m_fig;
  }

  bool Solver::stopping_condition_met(const TubeVector& x)
  {
    assert(x.size() == m_max_thickness.size());

    Vector x_max_thickness = x.max_thickness();
    for(int i = 0 ; i < x.size() ; i++)
      if(x_max_thickness[i] > m_max_thickness[i])
        return false;
    return true;
  }

  bool Solver::fixed_point_reached(double volume_before, double volume_after, float fxpt_ratio)
  {
    if(fxpt_ratio == 0. || volume_after == volume_before)
      return true;

    assert(Interval(0.,1.).contains(fxpt_ratio));
    int n = m_max_thickness.size();
    return (std::pow(volume_after, 1./n) / std::pow(volume_before, 1./n)) >= fxpt_ratio;
  }

  void Solver::propagation(TubeVector &x, void (*ctc_func)(TubeVector&), float propa_fxpt_ratio)
  {
    assert(Interval(0.,1.).contains(propa_fxpt_ratio));

    if(propa_fxpt_ratio == 0.)
      return;
    
    bool emptiness;
    double volume_before_ctc;

    do
    {
      volume_before_ctc = x.volume();
      ctc_func(x);
      emptiness = x.is_empty();
    } while(!emptiness
         && !stopping_condition_met(x)
         && !fixed_point_reached(volume_before_ctc, x.volume(), propa_fxpt_ratio));
  }

  void Solver::cid(TubeVector &x, void (*ctc_func)(TubeVector&))
  {
    if(m_cid_fxpt_ratio == 0.)
      return;

    double t_bisection;
    x[0].max_gate_thickness(t_bisection);
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
  
  const BoolInterval Solver::solutions_contain(const list<TubeVector>& l_solutions, const TrajectoryVector& truth)
  {
    assert(!l_solutions.empty());

    BoolInterval result = NO;
    list<TubeVector>::const_iterator it;
    for(it = l_solutions.begin() ; it != l_solutions.end() ; ++it)
    {
      assert(truth.size() == it->size());
      BoolInterval b = it->contains(truth);
      if(b == YES) return YES;
      else if(b == MAYBE) result = MAYBE;
    }
    
    return result;
  }
}