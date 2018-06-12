#include "tubex.h"

using namespace std;
using namespace ibex;
using namespace tubex;

#define IVP 1
#define BVP 2
#define SOLVER_TEST IVP

void contract(vector<Tube>& v_x)
{
  #if SOLVER_TEST == IVP

    v_x[0].ctcFwdBwd(-sin(v_x[0]));

  #elif SOLVER_TEST == BVP
    
    Variable vx0, vx1;
    SystemFactory fac;
    fac.add_var(vx0);
    fac.add_var(vx1);
    fac.add_ctr(sqr(vx0) + sqr(vx1) = 1);
    System sys(fac);
    ibex::CtcHC4 hc4(sys);

    IntervalVector bounds(2);
    bounds[0] &= v_x[0][0.];
    bounds[1] &= v_x[0][1.];
    hc4.contract(bounds);
    v_x[0].set(bounds[0], 0.);
    v_x[0].set(bounds[1], 1.);

    v_x[0].ctcFwdBwd(v_x[0]);

  #endif
}

int main(int argc, char *argv[])
{
  /* =========== PARAMETERS =========== */

    vector<Tube> v;

    #if SOLVER_TEST == IVP

      Interval domain(0.,10.);
      float epsilon = 0.1;
      v.push_back(Tube(domain, Interval(-1.,1.)));
      v[0].set(1., 0.); // initial condition

    #elif SOLVER_TEST == BVP

      Interval domain(0.,1.);
      float epsilon = 0.051;
      v.push_back(Tube(domain, Interval(-1.,1.)));

    #endif

  /* =========== SOLVER =========== */

    stack<vector<Tube> > s;
    s.push(v);
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
          contract(v_x);

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

      cout << "solutions: " << v_solutions.size() << endl;
    }

    vibes::beginDrawing();
    VibesFigure_Tube fig("Solver");

    #if SOLVER_TEST == IVP

      fig.setProperties(100,100,700,700);

    #elif SOLVER_TEST == BVP

      fig.setProperties(100,100,700,350);
      Trajectory truth1(domain, Function("t", "exp(t)/sqrt(1+exp(2))"));
      fig.addTrajectory(&truth1, "truth1", "blue");
      Trajectory truth2(domain, Function("t", "-exp(t)/sqrt(1+exp(2))"));
      fig.addTrajectory(&truth2, "truth2", "red");

    #endif

    for(int i = 0 ; i < v_solutions.size() ; i++)
    {
      ostringstream o;
      o << "solution_" << i;
      fig.addTube(&v_solutions[i][0], o.str());
      fig.setTubeDerivative(&v_solutions[i][0], &v_solutions[i][0]);
    }

    fig.show(false);
    vibes::endDrawing();

  return EXIT_SUCCESS;
}