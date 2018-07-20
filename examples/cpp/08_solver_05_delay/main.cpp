#include "ibex.h"
#include "tubex.h"
#include "tubex-solver.h"

using namespace std;
using namespace ibex;
using namespace tubex;

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

      if((t - m_delay).lb() <= x.domain().lb())
        eval_result |= eval(t, x[t]);

      if((t - m_delay).ub() >= x.domain().lb())
        eval_result |= exp(m_delay) * x[(t - m_delay) & x.domain()];

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

void contract(TubeVector& x)
{
  double delay = 0.5;
  FncDelayCustom f(delay);

  CtcPicard ctc_picard(true, 1.1);
  ctc_picard.contract(f, x);

  CtcDelay ctc_delay(true);
  TubeVector y(x, IntervalVector(x.dim(), Interval::ALL_REALS));
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
    Trajectory truth(domain, tubex::Function("exp(t)"));
    //delete?
      double t_value = domain.lb();
      IntervalVector init_value(1, exp(t_value));
      x.set(init_value, t_value);

  /* =========== SOLVER =========== */

    tubex::Solver solver(epsilon, 0.005, 0.005, 1.);
    solver.figure()->add_trajectory(&truth, "truth", "blue");
    list<TubeVector> l_solutions = solver.solve(x, &contract);


  // Checking if this example is still working:
  return (solver.solution_encloses(l_solutions, truth)) ? EXIT_SUCCESS : EXIT_FAILURE;
}