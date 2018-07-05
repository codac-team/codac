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
#define SOLVER_TEST DELAY


#if SOLVER_TEST == DELAY
  
  class FncDelay : public tubex::Fnc
  {
    public: 

      FncDelay(double delay) : Fnc(1,1), m_delay(delay)
      {

      }

      const IntervalVector eval(const Interval& t, const IntervalVector& x) const
      {
        
      }

      const TubeVector eval(const TubeVector& x) const
      {
        // todo: check dim x regarding f. f.imgdim can be of 0 and then x 1 in order to keep slicing pattern
        TubeVector y(x, IntervalVector(x.dim()));

        Interval t_delayed;
        const TubeSlice *x_slice = x.getFirstSlice();
        TubeSlice *y_slice = y.getFirstSlice();

        while(y_slice != NULL)
        {
          t_delayed = x_slice->domain().lb() - m_delay;
          if(t_delayed.is_subset(x.domain()))
            y_slice->setInputGate(x[t_delayed]);
          
          t_delayed = x_slice->domain() - m_delay;
          if(t_delayed.is_subset(x.domain()))
            y_slice->setEnvelope(x[t_delayed]);

          x_slice = x_slice->nextSlice();
          y_slice = y_slice->nextSlice();
        }

        x_slice = x.getLastSlice();
        y_slice = y.getLastSlice();
        t_delayed = x_slice->domain().ub() - m_delay;
          if(t_delayed.is_subset(x.domain()))
            y_slice->setOutputGate(x[t_delayed]);

        return y;
      }

    protected:

      double m_delay = 0.;
  };

  void contract(TubeVector& x)
  {
    FncDelay f(0.5);

    //if(v_x[0].codomain().is_unbounded())
    {
      //tubex::CtcPicard ctc_picard;
      //ctc_picard.contract(f, x);
    }

    CtcDeriv ctc_deriv;
    ctc_deriv.contract(x, exp(0.5) * f.eval(x));
  }

#elif SOLVER_TEST == IVP_XMSIN_FWD || SOLVER_TEST == IVP_XMSIN_BWD

  void contract(TubeVector& x)
  {
    tubex::Function f("x", "-sin(x)");

    //if(x.codomain().is_unbounded())
    {
      tubex::CtcPicard ctc_picard;
      ctc_picard.contract(f, x);
    }

    CtcDeriv ctc_deriv;
    ctc_deriv.contract(x, f.eval(x));
  }

#elif SOLVER_TEST == IVP_PICARD

  void contract(TubeVector& x)
  {
    tubex::Function f("x", "-x");

    //if(x.codomain().is_unbounded())
    {
      tubex::CtcPicard ctc_picard;
      ctc_picard.contract(f, x);
    }

    CtcDeriv ctc_deriv;
    ctc_deriv.contract(x, f.eval(x));
  }

#elif SOLVER_TEST == DAE

  void contract(TubeVector& x)
  {
    //Variable vx, vy;
    //tubex::Function f("x", "y", "(y, [-oo,oo])");//vx, vy, Return(vy,"[-oo,oo]"));

    //if(x.codomain().is_unbounded())
    //{
    //  tubex::CtcPicard ctc_picard;
    //  ctc_picard.contract(f, x);
    //}

    Variable vx0, vx1;
    SystemFactory fac;
    fac.add_var(vx0);
    fac.add_var(vx1);
    fac.add_ctr(sqr(vx0) + sqr(vx1) = 1);
    System sys(fac);
    ibex::CtcHC4 hc4(sys);
    tubex::CtcHC4 ctc_hc4;
    ctc_hc4.contract(hc4, x);

    TubeVector xdot(x);
    TubeSlice *xdot_slice = xdot.getFirstSlice(), *x_slice = x.getFirstSlice();
    while(xdot_slice != NULL)
    {
      IntervalVector box(2);
      box[0] = x_slice->inputGate()[1];
      xdot_slice->setInputGate(box);
      box[0] = x_slice->outputGate()[1];
      xdot_slice->setOutputGate(box);
      box[0] = x_slice->codomain()[1];
      xdot_slice->setEnvelope(box);

      x_slice = x_slice->nextSlice();
      xdot_slice = xdot_slice->nextSlice();
    }

    CtcDeriv ctc_deriv;
    ctc_deriv.contract(x, xdot);
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

    //if(x.codomain().is_unbounded())
    {
      tubex::CtcPicard ctc_picard;
      ctc_picard.contract(f, x, true);
    }
    
    CtcDeriv ctc_deriv;
    ctc_deriv.contract(x, f.eval(x));
  }

#elif SOLVER_TEST == BVP_CP2010
      
  void contract(TubeVector& x)
  {
    Variable x1, x2, x1dot, x2dot;
    
    tubex::Function f(x1,
               x2,
        Return(x2,
               0.1 * x1 * exp((20.*0.4*(1.-x1)) / (1. + 0.4 * (1.-x1)))));
    
    /*Function f(x1,
               x2,
               x1dot,
               x2dot,
        Return(x2,
               0.1 * x1 * exp((20.*0.4*(1.-x1)) / (1. + 0.4 * (1.-x1))),
               Interval::ALL_REALS,
               Interval::ALL_REALS));*/

    /*Function g(x1,
               x2,
               x1dot,
               x2dot,
        Return(x1,
               x2,
               x2,
               0.1 * x1 * exp((20.*0.4*(1.-x1)) / (1. + 0.4 * (1.-x1)))));

    x &= TubeVector(g, x);*/

    //if(x.codomain().is_unbounded())
    {
      //cout << "starting Picard " << x.nbSlices() << endl;
      tubex::CtcPicard ctc_picard;
      ctc_picard.contract_fwd(f, x);
      ctc_picard.contract_bwd(f, x);
      //cout << "ending Picard" << endl;
    }
    
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

      float epsilon = 0.5;
      Interval domain(0.,1.);
      TubeVector x(domain, IntervalVector(2, Interval(-1.,1.)));
      bool show_details = true;

    #elif SOLVER_TEST == BVP

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
      TubeVector x(domain, IntervalVector(1, Interval(-99999.,99999.)));
      x.set(IntervalVector(1, 1.), 0.); // initial condition
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

    #endif

    for(int i = 0 ; i < v_solutions.size() ; i++)
    {
      cout << i << ": " << v_solutions[i] << endl;
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