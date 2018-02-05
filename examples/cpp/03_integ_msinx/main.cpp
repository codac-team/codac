/* ============================================================================
 *  tubex-lib - Guaranteed integration example: xdot = -sin(x)
 *
 *  Example from the paper "Guaranteed Computation of Robot Trajectories"
 *  Simon Rohou, Luc Jaulin, Lyudmila Mihaylova, Fabrice Le Bars, Sandor M. Veres
 *
 *  Here we consider the following initial value problem:
 *    xdot = -sin(x)
 *    x0   = 1
 *
 *  This code will show successive contractions of tube [x](·): first five
 *  computations and final result when reaching a fix point. Light gray
 *  areas represent the tube part that has been contracted during
 *  the contraction step.
 *
 *  Note: example replaced by the Lissajous example in the paper
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2016
 * ---------------------------------------------------------------------------- */

#include "Tube.h"
#include "VibesFigure_Tube.h"

using namespace std;
using namespace ibex;
using namespace tubex;

int main(int argc, char *argv[])
{
  /* =========== PARAMETERS =========== */

    double x0 = 1.;
    Interval domain(0,10);
    double timestep = 0.01;

    float epsilon = 0.1;

  /* =========== INTERVAL INTEGRATION =========== */

    int i = 0;

    stack<Tube> s;
    s.push(Tube(domain, timestep, Interval(-1,1)));

    vector<Tube*> v_solutions;

    while(!s.empty())
    {
      Tube x = s.top();
      s.pop();

      //cout << "Contractions" << endl;
      double volume_x_ctc;

      Tube a(domain, timestep);
      Tube b(domain, timestep);

      do
      {
        //cout << "Step " << i << "... \tpress ENTER to continue" << flush;
        volume_x_ctc = x.volume(); // check tube's volume to detect a fixpoint

        // Contractors based on elementary constraints:
        a &= sin(x);
        b &= a.primitive();
        x &= x0 - b;

        i++;
        
        //if(argc == 1) cin.ignore(); // press ENTER to continue
      } while(!x.isEmpty() && volume_x_ctc != x.volume()); // up to the fixpoint

      if(!x.isEmpty())
      {
        // Bisection

        float max_diam = 0.;
        int index_max_diam;
        for(int u = 0 ; u < x.size() ; u++)
        {
          if(x[u].diam() > max_diam)
          {
            max_diam = x[u].diam();
            index_max_diam = u;
          }
        }

        double t_bisection = x.domain(index_max_diam).mid();
        double t_diam = x[t_bisection].diam();

        if(t_diam > epsilon)
        {
          float ratio = 0.6;

          Tube derivative = -sin(x);
          pair<Tube,Tube> p = x.bisect(derivative, t_bisection, ratio);

          if(!p.first.isEmpty()) s.push(p.first);
          if(!p.second.isEmpty()) s.push(p.second);
        }

        else
          v_solutions.push_back(new Tube(x));
      }
    }

    cout << v_solutions.size() << " solutions" << endl;
    VibesFigure_Tube::show(v_solutions[0], "Tube [x](·)", 200, 50);


  /* =========== END =========== */

    VibesFigure_Tube::endDrawing();

  // Checking if this example is still working:
  //return (volume_x_bis <= 9.15) ? EXIT_SUCCESS : EXIT_FAILURE;
    return EXIT_SUCCESS;
}