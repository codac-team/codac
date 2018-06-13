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

  double Solver::tubeVectorVolume(const vector<Tube>& v_x) const
  {
    double volume = 0.;
    for(int i = 0 ; i < v_x.size() ; i++)
      volume += v_x[i].volume();
    return volume;
  }

  double Solver::tubeVectorIsEmpty(const vector<Tube>& v_x) const
  {
    for(int i = 0 ; i < v_x.size() ; i++)
      if(v_x[i].isEmpty())
        return true;
    return false;
  }

  vector<vector<Tube> > Solver::solve(vector<Tube>& v_x, void (*ctc_func)(std::vector<Tube>&), float max_thickness, float refining_ratio)
  {
    stack<vector<Tube> > s;
    s.push(v_x);
    vector<vector<Tube> > v_solutions;

    while(!s.empty())
    {
      vector<Tube> v_x = s.top();
      s.pop();

      bool emptiness;
      double volume, volume_before_refining;

      do
      {
        volume_before_refining = tubeVectorVolume(v_x);

        // 1. Refining

          int first_id_max_thickness_x0;
          double max_thickness_x0 = v_x[0].maxThickness(first_id_max_thickness_x0);
          double t_refining = v_x[0].getSlice(first_id_max_thickness_x0)->domain().mid();
          //double t_refining = v_x[0].getWiderSlice()->domain().mid();

          for(int i = 0 ; i < v_x.size() ; i++)
            v_x[i].sample(t_refining);

        // 2. Contractions up to the fixed point

          double volume_before_ctc;

          do
          {
            volume_before_ctc = tubeVectorVolume(v_x);
            ctc_func(v_x);
            emptiness = tubeVectorIsEmpty(v_x);
            volume = tubeVectorVolume(v_x);
          } while(!emptiness && volume != volume_before_ctc); // fixed point

      } while(!emptiness && (volume / volume_before_refining) < (1. - refining_ratio));

      // 3. Bisection

        if(!emptiness)
        {
          int first_id_max_thickness_x0;
          double max_thickness_x0 = v_x[0].maxThickness(first_id_max_thickness_x0);
          double t_bisection = v_x[0].getSlice(first_id_max_thickness_x0)->domain().mid();

          if(max_thickness_x0 > max_thickness)
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