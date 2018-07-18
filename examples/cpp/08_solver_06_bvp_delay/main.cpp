#include "ibex.h"
#include "tubex.h"

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
  // Boundary constraints

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

  // Differential constraint

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

int main(int argc, char *argv[])
{
  /* =========== PARAMETERS =========== */

    int n = 1;
    Vector epsilon(n, 0.05);
    Interval domain(0.,1.);
    TubeVector x(domain, n);
    Trajectory truth1(domain, tubex::Function("exp(t)/sqrt(1+exp(2))"));
    Trajectory truth2(domain, tubex::Function("-exp(t)/sqrt(1+exp(2))"));

  /* =========== SOLVER =========== */

    tubex::Solver solver(epsilon, 0.005, 0.005, 1.);
    vector<TubeVector> v_solutions = solver.solve(x, &contract);

  /* =========== GRAPHICS =========== */

    vibes::beginDrawing();
    VibesFigure_Tube fig("Solver", x.dim());
    fig.set_properties(100,100,700,500);
    fig.add_trajectory(&truth1, "truth1", "blue");
    fig.add_trajectory(&truth2, "truth2", "red");

    for(int i = 0 ; i < v_solutions.size() ; i++)
    {
      ostringstream o; o << "solution_" << i;
      fig.add_tube(&v_solutions[i], o.str());
    }

    fig.show(true);
    vibes::endDrawing();


  // Checking if this example is still working:
  return (solver.solution_encloses(v_solutions, truth1)
       && solver.solution_encloses(v_solutions, truth2)) ? EXIT_SUCCESS : EXIT_FAILURE;
}