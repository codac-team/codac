#include "ibex.h"
#include "tubex.h"
#include "tubex-solver.h"

using namespace std;
using namespace ibex;
using namespace tubex;

class FncDelayCustom : public tubex::Fnc
{
  public: 

    FncDelayCustom(double delay) : Fnc(1,1), m_delay(delay) { }
    const Interval eval(const Interval& t, const TubeVector& x) const { /* scalar case not defined */ }
    const IntervalVector eval_vector(const Interval& t, const IntervalVector& x) const { return x; }

    const IntervalVector eval_vector(const Interval& t, const TubeVector& x) const
    {
      IntervalVector eval_result(x.size(), Interval::EMPTY_SET);

      if((t - m_delay).lb() <= x.domain().lb())
        eval_result |= eval_vector(t, x(t));

      if((t - m_delay).ub() >= x.domain().lb())
        eval_result |= exp(m_delay) * x((t - m_delay) & x.domain());

      return eval_result;
    }

  protected:

    double m_delay = 0.;
};

void contract(TubeVector& x)
{
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
    Interval domain(0.,5.);
    TubeVector x(domain, n);
    TrajectoryVector truth(domain, tubex::Function("exp(t)"));
    //delete?
      double t_value = domain.lb();
      IntervalVector init_value(1, exp(t_value));
      x.set(init_value, t_value);

  /* =========== SOLVER =========== */

    tubex::Solver solver(epsilon, 0.005, 0.005, 1.);
    solver.figure()->add_trajectoryvector(&truth, "truth", "blue");
    list<TubeVector> l_solutions = solver.solve(x, &contract);


  // Checking if this example is still working:
  return (solver.solution_encloses(l_solutions, truth)) ? EXIT_SUCCESS : EXIT_FAILURE;
}