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
#define PATH_PLANNING 10
#define SOLVER_TEST DELAY_BVP

double obstacle_radius = 0.95;

void createObstacles(vector<IntervalVector>& v_obstacles);
void displayPathPlanningMap(const vector<TubeVector>& v_x, const vector<IntervalVector>& v_obstacles);

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
      TubeVector y(x, IntervalVector(image_dim()));

      const TubeSlice *x_slice = x.get_first_slice();
      TubeSlice *y_slice = y.get_first_slice();

      while(x_slice != NULL)
      {
        y_slice->set_input_gate(eval(x_slice->domain().lb(), x));
        y_slice->set_envelope(eval(x_slice->domain(), x));

        x_slice = x_slice->next_slice();
        y_slice = y_slice->next_slice();
      }

      x_slice = x.get_last_slice();
      y_slice = y.get_last_slice();
      y_slice->set_output_gate(eval(x_slice->domain().ub(), x));

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

    #if SOLVER_TEST == IVP_XMSIN_FWD
      ctc_picard.contract_fwd(f, x);
    #elif SOLVER_TEST == IVP_XMSIN_BWD
      ctc_picard.contract_bwd(f, x);
    #endif

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

#elif SOLVER_TEST == PATH_PLANNING
      
  void contract(TubeVector& x)
  {
    Variable vx, vt, vy, vcos, vsin;

    //{
    //  SystemFactory fac;
    //  fac.add_var(vt);
    //  fac.add_var(vx);
    //  fac.add_var(vy);
    //  fac.add_var(vcos);
    //  fac.add_var(vsin);
    //  fac.add_ctr(sqr(vcos) + sqr(vsin) = 1);
    //  fac.add_ctr(cos(asin(vsin)) = vcos);
    //  System sys(fac);
    //  ibex::CtcHC4 hc4(sys);
    //  tubex::CtcHC4 ctc_hc4;
    //  ctc_hc4.contract(hc4, x);
    //}

    tubex::Function f("x","y", "costheta", "sintheta",
                      "(costheta;sintheta;0.25*[-1,0];0.25*[-1,0])");
    
    //tubex::CtcPicard ctc_picard;
    //ctc_picard.contract(f, x);
    
    TubeVector v = f.eval(x);

    //CtcDeriv ctc_deriv;
    //ctc_deriv.contract(x, v);

    vector<IntervalVector> v_obstacles;
    createObstacles(v_obstacles);
    {
      SystemFactory fac;
      fac.add_var(vt);
      fac.add_var(vx);
      fac.add_var(vy);
      fac.add_var(vcos);
      fac.add_var(vsin);
      fac.add_ctr(sqr(vcos) + sqr(vsin) = 1);
      fac.add_ctr(cos(asin(vsin)) = vcos);
      for(int i = 0 ; i < v_obstacles.size() ; i++)
        fac.add_ctr(sqrt(sqr(vx-v_obstacles[i][0]) + sqr(vy-v_obstacles[i][1])) > obstacle_radius);
      System sys(fac);
      ibex::CtcHC4 hc4(sys);
      tubex::CtcHC4 ctc_hc4;
      ctc_hc4.contract(hc4, x);
    }

    {
      CtcEval ctc_eval(false);
      Interval t(4., 4.06);
      IntervalVector z(4);
      z[0] = Interval(5.98,6.06);
      z[1] = Interval(4.89,4.91);
      z[2] = Interval(0.9525,0.9755);
      z[3] = Interval(-0.304,-0.22);
      ctc_eval.contract(t, z, x, v);
    }

    {
      CtcEval ctc_eval(true);
      Interval t(7.9374,8.);
      IntervalVector z(4);
      z[0] = Interval(8.49735, 8.5);
      z[1] = Interval(2.23252, 2.26765);
      z[2] = Interval(0.1262, 0.229281);
      z[3] = Interval(-0.992005, -0.97336);
      ctc_eval.contract(t, z, x, v);
    }
  }

#endif


int main(int argc, char *argv[])
{
  /* =========== PARAMETERS =========== */

    #if SOLVER_TEST == IVP_XMSIN_FWD || SOLVER_TEST == IVP_XMSIN_BWD

      Vector epsilon(1, 0.05);
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

      Vector epsilon(1, 0.05);
      Interval domain(0.,1.);
      TubeVector x(domain, 1);
      x.set(IntervalVector(1, 1.), 0.); // initial condition
      bool show_details = true;

    #elif SOLVER_TEST == DAE

      Vector epsilon(2, 0.15);
      Interval domain(0.,2.);
      TubeVector x(domain, IntervalVector(4, Interval(-1.,1.)));
      IntervalVector init(4);
      init[0] = 0.; init[1] = 1.;
      //init[0] = Interval(cos(3.*M_PI/4.));
      //init[1] = Interval(sin(3.*M_PI/4.));
      x.set(init, domain.lb());
      bool show_details = true;

    #elif SOLVER_TEST == SINGULARITY

      Vector epsilon(2, 0.05);
      Interval domain(0.,1.5);
      TubeVector x(domain, IntervalVector(2, Interval(-2.,2.)));
      IntervalVector init(2);
      init[0] = 1.;
      //init[1] = -1.;
      x.set(init, domain.lb());
      bool show_details = true;

    #elif SOLVER_TEST == BVP

      Vector epsilon(1, 0.05);
      Interval domain(0.,1.);
      TubeVector x(domain, 1);
      bool show_details = true;

    #elif SOLVER_TEST == DELAY_BVP

      Vector epsilon(1, 0.05);
      Interval domain(0.,1.);
      TubeVector x(domain, 1);
      bool show_details = true;

    #elif SOLVER_TEST == BVP_CP2010

      Vector epsilon(1, 0.1);
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

      Vector epsilon(1, 0.05);
      Interval domain(0.,5.);
      TubeVector x(domain, 1);
      double t_value = domain.lb();
      IntervalVector init_value(1, Interval(2.*atan(exp(-t_value)*tan(0.5))));
      x.set(init_value, t_value);
      bool show_details = true;

    #elif SOLVER_TEST == PATH_PLANNING

      Vector epsilon(4);
      epsilon[0] = 4.;
      epsilon[1] = 4.;
      epsilon[2] = 0.12;
      epsilon[3] = 0.12;

      Interval domain(0.,8.);
      IntervalVector codomain(4);
      codomain[0] = Interval(0.,8.5);
      codomain[1] = Interval(-0.5,12.);
      codomain[2] = Interval(-1.,1.);
      codomain[3] = Interval(-1.,1.);
      TubeVector x(domain, codomain);

      IntervalVector init_value(4);
      init_value[0] = 0.5+1.5;
      init_value[1] = 5.;
      init_value[2] = cos(0.);
      init_value[3] = sin(0.);
      x.set(init_value, domain.lb());

      bool show_details = true;

      vector<IntervalVector> v_obstacles;
      createObstacles(v_obstacles);

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

      fig.set_properties(100,100,700,500);
      Trajectory truth(domain, tubex::Function("2.*atan(exp(-t)*tan(0.5))"));
      fig.add_trajectory(&truth, "truth1", "blue");

    #elif SOLVER_TEST == IVP_PICARD

      fig.set_properties(100,100,700,500);

    #elif SOLVER_TEST == DAE

      fig.set_properties(100,100,700,500);
      TrajectoryVector truth1(domain, tubex::Function("(sin(t);cos(t))"));
      fig.add_trajectory(&truth1, "truth1", "blue");

    #elif SOLVER_TEST == SINGULARITY

      fig.set_properties(100,100,700,500);
      TrajectoryVector truth1(domain, tubex::Function("(1-t,-1)"));
      fig.add_trajectory(&truth1, "truth1", "blue");

    #elif SOLVER_TEST == BVP

      fig.set_properties(100,100,700,350);
      Trajectory truth1(domain, tubex::Function("exp(t)/sqrt(1+exp(2))"));
      fig.add_trajectory(&truth1, "truth1", "blue");
      Trajectory truth2(domain, tubex::Function("-exp(t)/sqrt(1+exp(2))"));
      fig.add_trajectory(&truth2, "truth2", "red");

    #elif SOLVER_TEST == BVP_CP2010

      fig.set_properties(100,100,700,350);

    #elif SOLVER_TEST == DELAY

      fig.set_properties(100,100,700,350);
      Trajectory truth(domain, tubex::Function("exp(t)"));
      fig.add_trajectory(&truth, "truth", "blue");

    #elif SOLVER_TEST == DELAY_BVP

      fig.set_properties(100,100,700,350);
      Trajectory truth1(domain, tubex::Function("exp(t)/sqrt(1+exp(2))"));
      fig.add_trajectory(&truth1, "truth1", "blue");
      Trajectory truth2(domain, tubex::Function("-exp(t)/sqrt(1+exp(2))"));
      fig.add_trajectory(&truth2, "truth2", "red");

    #elif SOLVER_TEST == PATH_PLANNING

      fig.set_properties(100,100,700,350);

    #endif

    for(int i = 0 ; i < v_solutions.size() ; i++)
    {
      cout << (i+1) << ": " << v_solutions[i] <<  ", tf↦" << v_solutions[i][v_solutions[i].domain().ub()] << endl;
      ostringstream o;
      o << "solution_" << i;
      fig.add_tube(&v_solutions[i], o.str());
      //fig.set_tube_derivative(&v_solutions[i], &v_solutions[i]);
    }

    #if SOLVER_TEST == PATH_PLANNING
      displayPathPlanningMap(v_solutions, v_obstacles);
    #endif

    if(!v_solutions.empty())
      fig.show(show_details);
    vibes::end_drawing();

  return EXIT_SUCCESS;
}



void createObstacles(vector<IntervalVector>& v_obstacles)
{
  IntervalVector box(2);
  box[0] = Interval(2.25); box[1] = Interval(6.);
  v_obstacles.push_back(box);
  box[0] = Interval(4.25); box[1] = Interval(6.);
  v_obstacles.push_back(box);
  box[0] = Interval(2.25); box[1] = Interval(4.);
  v_obstacles.push_back(box);
  box[0] = Interval(4.25); box[1] = Interval(4.);
  v_obstacles.push_back(box);

  for(double a = M_PI/2. ; a > -M_PI-0.2 ; a-= 0.1)
  {
    box[0] = cos(a) * 7.5/2.; box[1] = sin(a) * 7.5/2.;
    box[0] += 6.; box[1] += 2.25;
    v_obstacles.push_back(box);
    box[0] = cos(a) * 3./2.; box[1] = sin(a) * 3./2.;
    box[0] += 6.; box[1] += 2.25;
    v_obstacles.push_back(box);
  }
}

void displayPathPlanningMap(const vector<TubeVector>& v_x, const vector<IntervalVector>& v_obstacles)
{
  const string fig_name = "Map (top view): [x](·)x[y](·)";
  const int slices_number_to_display = 500;

  vibes::newFigure(fig_name);
  vibes::setFigureProperties(
            vibesParams("figure", fig_name, "x", 100, "y", 100, "width", 600, "height", 600));
  vibes::axis_limits(0., 10., 0., 10.);

  for(int i = 0 ; i < v_obstacles.size() ; i++)
    vibes::drawCircle(v_obstacles[i][0].mid(), v_obstacles[i][1].mid(), obstacle_radius, "#779CA3[#A5C8CE]", vibesParams("figure", fig_name));

  for(int k = 0 ; k < v_x.size() ; k++)
  {
    // Robot's tubes projection
    int startpoint;
    for(int i = 0 ; i < v_x[k].nb_slices() ; i += max((int)(v_x[k].nb_slices() / slices_number_to_display), 1))
      startpoint = i;

    for(int i = startpoint ; i >= 0; i -= max((int)(v_x[k].nb_slices() / slices_number_to_display), 1))
    {
      Interval intv_x = v_x[k][i][0];
      Interval intv_y = v_x[k][i][1];
      if(!intv_x.is_unbounded() && !intv_y.is_unbounded())
        vibes::drawBox(intv_x.lb(), intv_x.ub(), intv_y.lb(), intv_y.ub(),
                       "#DEDEDE[#DEDEDE]", vibesParams("figure", fig_name));
    }
  }
}