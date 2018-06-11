#include "tubex.h"

using namespace std;
using namespace ibex;
using namespace tubex;

int main(int argc, char *argv[])
{
  /* =========== PARAMETERS =========== */

    Interval domain(0.,1.);
    double timestep = 0.001;
    float epsilon = 0.01;

  /* =========== SYSTEM =========== */

    Variable vx0, vx1;

    SystemFactory fac;
    fac.add_var(vx0);
    fac.add_var(vx1);
    fac.add_ctr(sqr(vx0) + sqr(vx1) = 1);

    System sys(fac);
    CtcHC4 hc4(sys);

  /* =========== RESOLUTION =========== */

    stack<Tube> s;
    vector<Tube*> v_solutions;
    s.push(Tube(domain, timestep, Interval(-10.,10.))); // initial domain

    // Note: an initial bounded image (e.g. [-10,10]) is mandatory
    // in order to enable contractions with the ctcEval operator

    while(!s.empty())
    {
      Tube x = s.top(); s.pop();

      // 1. Contractions up to the fixed point

        IntervalVector bounds(2);
        double volume_x;

        do
        {
          volume_x = x.volume(); // check tube's volume to detect a fixed point

          // Contractions

            bounds[0] &= x[0.];
            bounds[1] &= x[1.];
            hc4.contract(bounds);

            // BVP, constraints at 0 and 1:
            x.set(bounds[0], 0.); // the derivative of x is x
            x.set(bounds[1], 1.);

            x.ctcFwdBwd(x);

        } while(!x.isEmpty() && volume_x != x.volume()); // up to the fixed point

      // 2. Bisection

        if(!x.isEmpty())
        {
          int first_id_max_thickness;
          double max_thickness = x.maxThickness(first_id_max_thickness);
          double t_bisection = x.getSlice(first_id_max_thickness)->domain().mid(); // bisection in the middle of a slice

          cout << "first_id_max_thickness " << first_id_max_thickness << endl;
          cout << "t_bisection " << t_bisection << endl;
          cout << "max_thickness " << max_thickness << endl;

          if(max_thickness > epsilon)
          {
            pair<Tube,Tube> p = x.bisect(t_bisection);
            s.push(p.first);
            s.push(p.second);
          }

          else
          {
            v_solutions.push_back(new Tube(x));
          }
        }
    }

    // Graphics

    vibes::beginDrawing();

    VibesFigure_Tube fig("Solver");
    fig.setProperties(100,100,1000,450);
    for(int i = 0 ; i < v_solutions.size() ; i++)
    {
      ostringstream o;
      o << "solution_" << i;
      fig.addTube(v_solutions[i], o.str());
    }

    fig.show();
    vibes::endDrawing();

  return EXIT_SUCCESS;
}