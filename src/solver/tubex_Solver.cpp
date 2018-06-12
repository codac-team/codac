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

#include "tubex_Solver.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  Solver::Solver()
  {

  }

  vector<vector<Tube> > Solver::solve(vector<Tube>& v_x, void (*ctc_func)(std::vector<Tube>&), float epsilon)
  {
    stack<vector<Tube> > s;
    s.push(v_x);
    vector<vector<Tube> > v_solutions;

    while(!s.empty())
    {
      vector<Tube> v_x = s.top();
      s.pop();

      // 1. Contractions up to the fixed point

        bool emptiness;
        double volume, new_volume;

        do
        {
          volume = 0.;
          for(int i = 0 ; i < v_x.size() ; i++)
            volume += v_x[i].volume();
          ctc_func(v_x);

          emptiness = false;
          new_volume = 0.;
          for(int i = 0 ; i < v_x.size() ; i++)
          {
            emptiness |= v_x[i].isEmpty();
            volume += v_x[i].volume();
          }

        } while(!emptiness && volume != volume);

      // 2. Bisection

        if(!emptiness)
        {
          int first_id_max_thickness_x0;
          double max_thickness_x0 = v_x[0].maxThickness(first_id_max_thickness_x0);
          double t_bisection = v_x[0].getSlice(first_id_max_thickness_x0)->domain().mid();

          if(max_thickness_x0 > epsilon)
          {
            vector<Tube> v_first, v_second;

            for(int i = 0 ; i < v_x.size() ; i++)
            {
              pair<Tube,Tube> p_x = v_x[i].bisect(t_bisection);
              v_first.push_back(p_x.first);
              v_second.push_back(p_x.second);
            }

            s.push(v_first);
            s.push(v_second);
          }

          else
          {
            vector<Tube> v;
            for(int i = 0 ; i < v_x.size() ; i++)
              v.push_back(Tube(v_x[i]));
            v_solutions.push_back(v);
          }
        }

      cout << "\rsolutions: " << v_solutions.size() << "  " << flush;
    }

    cout << endl;
    return v_solutions;
  }
}