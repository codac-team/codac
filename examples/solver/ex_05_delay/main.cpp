/** 
 *  tubex-lib - Examples
 *  Solver testcase
 * ----------------------------------------------------------------------------
 *
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2019 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "tubex.h"
#include "tubex-solver.h"

using namespace std;
using namespace ibex;
using namespace tubex;

class FncDelayCustom : public tubex::Fnc
{
  public: 

    FncDelayCustom(double delay) : Fnc(1, 1, true), m_delay(delay) { }
    const Interval eval(int slice_id, const TubeVector& x) const { cout << "not defined 1" << endl; }
    const Interval eval(const Interval& t, const TubeVector& x) const { cout << "not defined 2" << endl; }
    const Interval eval(const IntervalVector& x) const { cout << "not defined 3" << endl; }
    const IntervalVector eval_vector(const IntervalVector& x) const { cout << "not defined 4" << endl; }

    const IntervalVector eval_vector(int slice_id, const TubeVector& x) const
    {
      Interval t = x[0].slice(slice_id)->domain();
      return eval_vector(t, x);
    }

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
  double delay = 0.5;
  FncDelayCustom f(delay);

  CtcPicard ctc_picard;
  ctc_picard.contract(f, x, FORWARD | BACKWARD);

  // Computing the derivative v from a delay of x
  CtcDelay ctc_delay;
  TubeVector v(x, IntervalVector(x.size()));
  ctc_delay.contract(delay, x, v);
  v *= exp(delay);

  CtcDeriv ctc_deriv;
  ctc_deriv.set_fast_mode(true);
  ctc_deriv.contract(x, v, FORWARD | BACKWARD);
}

int main()
{
  /* =========== PARAMETERS =========== */

    Tube::enable_syntheses(false);
    int n = 1;
    Vector epsilon(n, 10.);
    Interval domain(0.,5.);
    TubeVector x(domain, n);
    TrajectoryVector truth(domain, tubex::Function("exp(t)"));
    //delete?
      double t_value = domain.lb();
      IntervalVector init_value(1, exp(t_value));
      x.set(init_value, t_value);

  /* =========== SOLVER =========== */

    tubex::Solver solver(epsilon);
    solver.set_refining_fxpt_ratio(1.);
    solver.set_propa_fxpt_ratio(1.);
    solver.set_cid_fxpt_ratio(0.);
    solver.figure()->add_trajectoryvector(&truth, "truth");
    list<TubeVector> l_solutions = solver.solve(x, &contract);


  // Checking if this example still works:
  return (solver.solutions_contain(l_solutions, truth) == YES) ? EXIT_SUCCESS : EXIT_FAILURE;
}