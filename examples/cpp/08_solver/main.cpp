#include "ibex.h"
#include "tubex.h"
#include <time.h>

using namespace std;
using namespace ibex;
using namespace tubex;

#define IVP_XMSIN_FWD 1
#define IVP_XMSIN_BWD 2
#define BVP 3
#define DAE 4
#define IVP_PICARD 5
#define BVP_CP2010 6
#define DELAY 7
#define DELAY_BVP 8
#define SINGULARITY 9
#define SOLVER_TEST SINGULARITY


class FncDelayCustom : public tubex::Fnc
{
  public: 

    FncDelayCustom(double delay) : Fnc(1,1), m_delay(delay)
    {

    }

    const IntervalVector eval(const Interval& t, const IntervalVector& x) const
    {
      return x;
    }

    const IntervalVector eval(const Interval& t, const TubeVector& x) const
    {
      IntervalVector eval_result(x.dim(), Interval::EMPTY_SET);

      if(m_delay > t.lb())
        eval_result |= eval(t, x[t]);

      if(m_delay < t.ub())
        //eval_result |= IntervalVector(x.dim(), Interval::ALL_REALS);
        eval_result |= exp(m_delay) * x[t - m_delay];

      return eval_result;
    }

    const TubeVector eval(const TubeVector& x) const
    {
      // todo: check dim x regarding f. f.imgdim can be of 0 and then x 1 in order to keep slicing pattern
      TubeVector y(x, IntervalVector(imageDim()));

      const TubeSlice *x_slice = x.getFirstSlice();
      TubeSlice *y_slice = y.getFirstSlice();

      while(x_slice != NULL)
      {
        y_slice->setInputGate(eval(x_slice->domain().lb(), x));
        y_slice->setEnvelope(eval(x_slice->domain(), x));

        x_slice = x_slice->nextSlice();
        y_slice = y_slice->nextSlice();
      }

      x_slice = x.getLastSlice();
      y_slice = y.getLastSlice();
      y_slice->setOutputGate(eval(x_slice->domain().ub(), x));

      return y;
    }

  protected:

    double m_delay = 0.;
};

#if SOLVER_TEST == DELAY

  void contract(TubeVector& x)
  {
    double delay = 0.5;
    FncDelayCustom f(delay);

    tubex::CtcPicard ctc_picard(1.1);
    ctc_picard.contract(f, x);

    CtcDelay ctc_delay;
    TubeVector y(x, IntervalVector(x.dim(), Interval::ALL_REALS));
    ctc_delay.contract(delay, x, y);

    CtcDeriv ctc_deriv;
    ctc_deriv.contract(x, exp(delay) * y);
  }

#elif SOLVER_TEST == DELAY_BVP

  void contract(TubeVector& x)
  {
    Variable vx0, vx1;
    SystemFactory fac;
    fac.add_var(vx0);
    fac.add_var(vx1);
    fac.add_ctr(sqr(vx0) + sqr(vx1) = 1);
    System sys(fac);
    ibex::CtcHC4 hc4(sys);
    
    IntervalVector bounds(2);
    bounds[0] = x[0.][0];
    bounds[1] = x[1.][0];
    hc4.contract(bounds);
    x.set(IntervalVector(1,bounds[0]), 0.);
    x.set(IntervalVector(1,bounds[1]), 1.);

    double delay = 0.5;
    FncDelayCustom f(delay);

    tubex::CtcPicard ctc_picard(1.1);
    ctc_picard.contract(f, x);
    
    CtcDelay ctc_delay;
    TubeVector y(x, IntervalVector(x.dim(), Interval::ALL_REALS));
    ctc_delay.contract(delay, x, y);

    CtcDeriv ctc_deriv;
    ctc_deriv.contract(x, exp(delay) * y);
  }

#elif SOLVER_TEST == IVP_XMSIN_FWD || SOLVER_TEST == IVP_XMSIN_BWD

  void contract(TubeVector& x)
  {
    tubex::Function f("x", "-sin(x)");

    tubex::CtcPicard ctc_picard;
    ctc_picard.contract(f, x);

    CtcDeriv ctc_deriv;
    ctc_deriv.contract(x, f.eval(x));
  }

#elif SOLVER_TEST == IVP_PICARD

  void contract(TubeVector& x)
  {
    tubex::Function f("x", "-x");

    tubex::CtcPicard ctc_picard;
    ctc_picard.contract(f, x);

    CtcDeriv ctc_deriv;
    ctc_deriv.contract(x, f.eval(x));
  }

#elif SOLVER_TEST == DAE

  void contract(TubeVector& x)
  {
    // Constraint x^2+y^2=1
    Variable vt, vx, vy, vxdot, vydot;
    SystemFactory fac1;
    fac1.add_var(vt);
    fac1.add_var(vx);
    fac1.add_var(vy);
    fac1.add_var(vxdot);
    fac1.add_var(vydot);
    fac1.add_ctr(sqr(vx) + sqr(vy) = 1);
    System sys1(fac1);
    ibex::CtcHC4 hc41(sys1);

    SystemFactory fac2;
    fac2.add_var(vt);
    fac2.add_var(vx);
    fac2.add_var(vy);
    fac2.add_var(vxdot);
    fac2.add_var(vydot);
    fac2.add_ctr(vx*vxdot+vy*vydot=0);
    System sys2(fac2);
    ibex::CtcHC4 hc42(sys2);

    tubex::CtcHC4 ctc_hc4;
    ctc_hc4.contract(hc41, x);
    ctc_hc4.contract(hc42, x);

    // Constraint xdot = y
    tubex::Function f("x", "y", "xdot", "ydot", "(y;[-1,1];[-1,1];[-1,1])"); // testcase 1
    //tubex::Function f("x", "y", "(y^2;[-10,10])"); // testcase 2
    tubex::CtcPicard ctc_picard; // by Picard
    ctc_picard.contract(f, x);
    CtcDeriv ctc_deriv; // by Cd/dt
    ctc_deriv.contract(x, f.eval(x));
  }

#elif SOLVER_TEST == SINGULARITY

  void contract(TubeVector& x)
  {
    Variable vt, vy, vydot;
    SystemFactory fac;
    fac.add_var(vt);
    fac.add_var(vy);
    fac.add_var(vydot);
    fac.add_ctr(vy+vy*vydot=0);
    System sys(fac);
    ibex::CtcHC4 hc4(sys);
    tubex::CtcHC4 ctc_hc4;
    ctc_hc4.contract(hc4, x);

    tubex::Function f("y", "ydot", "(ydot;[-999,999])");
    tubex::CtcPicard ctc_picard; // by Picard
    //ctc_picard.contract(f, x);
    CtcDeriv ctc_deriv; // by Cd/dt
    ctc_deriv.contract(x, f.eval(x));
  }

#elif SOLVER_TEST == BVP
      
  void contract(TubeVector& x)
  {
    Variable vx0, vx1;
    SystemFactory fac;
    fac.add_var(vx0);
    fac.add_var(vx1);
    fac.add_ctr(sqr(vx0) + sqr(vx1) = 1);
    System sys(fac);
    ibex::CtcHC4 hc4(sys);
    
    IntervalVector bounds(2);
    bounds[0] = x[0.][0];
    bounds[1] = x[1.][0];
    hc4.contract(bounds);
    x.set(IntervalVector(1,bounds[0]), 0.);
    x.set(IntervalVector(1,bounds[1]), 1.);
    
    tubex::Function f("x", "x");

    tubex::CtcPicard ctc_picard;
    ctc_picard.contract(f, x);
    
    CtcDeriv ctc_deriv;
    ctc_deriv.contract(x, f.eval(x));
  }

#elif SOLVER_TEST == BVP_CP2010
      
  void contract(TubeVector& x)
  {
    tubex::Function f("x1", "x2", "(x2 ; 0.1 * x1 * exp((20.*0.4*(1.-x1)) / (1. + 0.4 * (1.-x1))))");
    
    tubex::CtcPicard ctc_picard;
    ctc_picard.contract(f, x);
    
    CtcDeriv ctc_deriv;
    ctc_deriv.contract(x, f.eval(x));
  }

#endif


int main(int argc, char *argv[])
{
  /* =========== PARAMETERS =========== */

    #if SOLVER_TEST == IVP_XMSIN_FWD || SOLVER_TEST == IVP_XMSIN_BWD

      float epsilon = 0.051;
      Interval domain(0.,10.);
      TubeVector x(domain, 1);

      #if SOLVER_TEST == IVP_XMSIN_FWD
        double t_value = domain.lb();
      #else
        double t_value = domain.ub();
      #endif

      IntervalVector init_value(1, Interval(2.*atan(exp(-t_value)*tan(0.5))));
      x.set(init_value, t_value);
      bool show_details = true;

    #elif SOLVER_TEST == IVP_PICARD

      float epsilon = 0.05;
      Interval domain(0.,1.);
      TubeVector x(domain, 1);
      x.set(IntervalVector(1, 1.), 0.); // initial condition
      bool show_details = true;

    #elif SOLVER_TEST == DAE

      float epsilon = 0.15;
      Interval domain(0.,2.);
      TubeVector x(domain, IntervalVector(4, Interval(-1.,1.)));
      IntervalVector init(4);
      init[0] = 0.; init[1] = 1.;
      //init[0] = Interval(cos(3.*M_PI/4.));
      //init[1] = Interval(sin(3.*M_PI/4.));
      x.set(init, domain.lb());
      bool show_details = true;

    #elif SOLVER_TEST == SINGULARITY

      float epsilon = 0.05;
      Interval domain(0.,1.5);
      TubeVector x(domain, IntervalVector(2, Interval(-2.,2.)));
      IntervalVector init(2);
      init[0] = 1.;
      //init[1] = -1.;
      x.set(init, domain.lb());
      bool show_details = true;

    #elif SOLVER_TEST == BVP

      float epsilon = 0.05;
      Interval domain(0.,1.);
      TubeVector x(domain, 1);
      bool show_details = true;

    #elif SOLVER_TEST == DELAY_BVP

      float epsilon = 0.05;
      Interval domain(0.,1.);
      TubeVector x(domain, 1);
      bool show_details = true;

    #elif SOLVER_TEST == BVP_CP2010

      float epsilon = 0.1;
      Interval domain(0.,1.);
      IntervalVector codomain(2), x0(codomain), xf(codomain);

      codomain[0] = Interval(0.,1.);
      codomain[1] = Interval(0.,2.);
      TubeVector x(domain, codomain);

      // 0.5058725840206
      // 0.9226804137526
      x0[0] = Interval(0.5058725840206).inflate(0.01); x0[1] = 0.;
      /*xf[1] = Interval(0.7379775914054797).inflate(0.000001);*/ xf[0] = 1.;
      x.set(x0, 0.);
      x.set(xf, 1.);
      bool show_details = true;

    #elif SOLVER_TEST == DELAY

      float epsilon = 0.05;
      Interval domain(0.,1.);
      TubeVector x(domain, 1);
      double t_value = domain.lb();
      IntervalVector init_value(1, Interval(2.*atan(exp(-t_value)*tan(0.5))));
      x.set(init_value, t_value);
      bool show_details = true;

    #endif

  /* =========== SOLVER =========== */

    tubex::Solver solver;
    clock_t tStart = clock();
    vector<TubeVector> v_solutions = solver.solve(x, &contract, epsilon);
    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

    if(v_solutions.size() == 0)
      cout << "no solution found" << endl;

  /* =========== GRAPHICS =========== */

    vibes::beginDrawing();
    VibesFigure_Tube fig("Solver", x.dim());

    #if SOLVER_TEST == IVP_XMSIN_FWD || SOLVER_TEST == IVP_XMSIN_BWD

      fig.setProperties(100,100,700,500);
      Trajectory truth(domain, tubex::Function("2.*atan(exp(-t)*tan(0.5))"));
      fig.addTrajectory(&truth, "truth1", "blue");

    #elif SOLVER_TEST == IVP_PICARD

      fig.setProperties(100,100,700,500);

    #elif SOLVER_TEST == DAE

      fig.setProperties(100,100,700,500);
      TrajectoryVector truth1(domain, tubex::Function("(sin(t);cos(t))"));
      fig.addTrajectory(&truth1, "truth1", "blue");

    #elif SOLVER_TEST == SINGULARITY

      fig.setProperties(100,100,700,500);
      TrajectoryVector truth1(domain, tubex::Function("(1-t,-1)"));
      fig.addTrajectory(&truth1, "truth1", "blue");

    #elif SOLVER_TEST == BVP

      fig.setProperties(100,100,700,350);
      Trajectory truth1(domain, tubex::Function("exp(t)/sqrt(1+exp(2))"));
      fig.addTrajectory(&truth1, "truth1", "blue");
      Trajectory truth2(domain, tubex::Function("-exp(t)/sqrt(1+exp(2))"));
      fig.addTrajectory(&truth2, "truth2", "red");

    #elif SOLVER_TEST == BVP_CP2010

      fig.setProperties(100,100,700,350);

    #elif SOLVER_TEST == DELAY

      fig.setProperties(100,100,700,350);
      Trajectory truth(domain, tubex::Function("exp(t)"));
      fig.addTrajectory(&truth, "truth", "blue");

    #elif SOLVER_TEST == DELAY_BVP

      fig.setProperties(100,100,700,350);
      Trajectory truth1(domain, tubex::Function("exp(t)/sqrt(1+exp(2))"));
      fig.addTrajectory(&truth1, "truth1", "blue");
      Trajectory truth2(domain, tubex::Function("-exp(t)/sqrt(1+exp(2))"));
      fig.addTrajectory(&truth2, "truth2", "red");

    #endif

    for(int i = 0 ; i < v_solutions.size() ; i++)
    {
      cout << (i+1) << ": " << v_solutions[i] <<  ", tf↦" << v_solutions[i][v_solutions[i].domain().ub()] << endl;
      ostringstream o;
      o << "solution_" << i;
      fig.addTube(&v_solutions[i], o.str());
      //fig.setTubeDerivative(&v_solutions[i], &v_solutions[i]);
    }

    if(!v_solutions.empty())
      fig.show(show_details);
    vibes::endDrawing();

  return EXIT_SUCCESS;
}