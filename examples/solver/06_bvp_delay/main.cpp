#include "ibex.h"
#include "tubex.h"
#include "tubex-solver.h"

using namespace std;
using namespace ibex;
using namespace tubex;

class FncDelayCustom : public tubex::Fnc
{
  public: 

    FncDelayCustom(double delay) : Fnc(1,1), m_delay(delay) { };
    const Interval eval(const Interval& t, const TubeVector& x) const { /* not defined */ };

    const IntervalVector eval_vector(const Interval& t, const TubeVector& x) const
    {
      IntervalVector eval_result(x.size(), Interval::EMPTY_SET);

      if((t - m_delay).lb() <= x.domain().lb())
        eval_result |= x(t);

      if((t - m_delay).ub() >= x.domain().lb())
        eval_result |= exp(m_delay) * x((t - m_delay) & x.domain());

      return eval_result;
    }

  protected:

    double m_delay = 0.;
};

void contract(TubeVector& x)
{
  // Boundary constraints

    Variable vx0, vx1;
    SystemFactory fac;
    fac.add_var(vx0);
    fac.add_var(vx1);
    fac.add_ctr(sqr(vx0) + sqr(vx1) = 1);
    System sys(fac);
    ibex::CtcHC4 hc4(sys);
    IntervalVector bounds(2);
    bounds[0] = x[0](0.);
    bounds[1] = x[0](1.);
    hc4.contract(bounds);
    x.set(IntervalVector(bounds[0]), 0.);
    x.set(IntervalVector(bounds[1]), 1.);

  // Differential constraint

    double delay = 0.5;
    FncDelayCustom f(delay);

    CtcPicard ctc_picard(true, 1.1);
    ctc_picard.contract(f, x);

    CtcDelay ctc_delay(true);
    TubeVector y(x, IntervalVector(x.size(), Interval::ALL_REALS));
    ctc_delay.contract(delay, x, y);

    CtcDeriv ctc_deriv(true);
    ctc_deriv.contract(x, exp(delay) * y);
}

int main()
{
  /* =========== PARAMETERS =========== */

    int n = 1;
    Vector epsilon(n, 0.05);
    Interval domain(0.,1.);
    TubeVector x(domain, n);
    TrajectoryVector truth1(domain, tubex::Function("exp(t)/sqrt(1+exp(2))"));
    TrajectoryVector truth2(domain, tubex::Function("-exp(t)/sqrt(1+exp(2))"));

  /* =========== SOLVER =========== */

    tubex::Solver solver(epsilon, 0.005, 0.005, 1.);
    solver.figure()->add_trajectoryvector(&truth1, "truth1", "blue");
    solver.figure()->add_trajectoryvector(&truth2, "truth2", "red");
    list<TubeVector> l_solutions = solver.solve(x, &contract);


  // Checking if this example is still working:
  return (solver.solution_encloses(l_solutions, truth1)
       && solver.solution_encloses(l_solutions, truth2)) ? EXIT_SUCCESS : EXIT_FAILURE;
}