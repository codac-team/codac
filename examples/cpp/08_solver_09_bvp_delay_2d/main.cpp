#include "ibex.h"
#include "tubex.h"
#include "tubex-solver.h"

using namespace std;
using namespace ibex;
using namespace tubex;

class FncDelayCustom : public tubex::Fnc
{
  public: 

    FncDelayCustom() : Fnc(2,2) { };
    const TubeVector eval(const TubeVector& x) const { return Fnc::eval(x); }

    const IntervalVector periodic_eval(const Interval& t, const TubeVector& x, double delay) const
    {
      double period = x.domain().diam();
      Interval t_delayed = t - delay;
      Interval a = Interval(t_delayed.lb(), x.domain().lb()) & t_delayed;
      Interval b = Interval(x.domain().lb(), t_delayed.ub()) & t_delayed;

      if(!a.is_empty())
      {
        double a1 = fmod(a.lb() + 4.*period, period);
        double a2 = fmod(a.ub() + 4.*period, period);
        if(a2 == 0.) a2 = period;
        a = Interval(min(a1,a2), max(a1,a2));
      }

      IntervalVector result(x.dim(), Interval::EMPTY_SET);
      if(!a.is_empty()) result |= x[a];
      if(!b.is_empty()) result |= x[b];
      return result;
    }

    const IntervalVector eval(const Interval& t, const TubeVector& x) const
    {
      IntervalVector eval_result(x.dim());

      IntervalVector intv_1 = periodic_eval(t, x, 1.);
      IntervalVector intv_075 = periodic_eval(t, x, 0.75);
      IntervalVector intv_05 = periodic_eval(t, x, 0.5);
      IntervalVector intv_025 = periodic_eval(t, x, 0.25);

      eval_result[0] = x[t][0] * (3. - sin(t) - (3. - cos(t)) * x[t][0] - (((2. + sin(t)) / 24.) * (intv_1[1] + 6.*intv_075[1] + 4.*intv_05[1] + 10.*intv_025[1] + 3*x[t][1])));
      eval_result[1] = x[t][1] * (6. - cos(t) - (10. - sin(t)) * x[t][1] - (((2. + sin(t)) / 24.) * (intv_1[0] + 6.*intv_075[0] + 4.*intv_05[0] + 10.*intv_025[0] + 3*x[t][0])));

      return eval_result;
    }
};

void contract(TubeVector& x)
{
  FncDelayCustom f;

  CtcPicard ctc_picard(false, 1.1);
  ctc_picard.contract(f, x, FORWARD | BACKWARD);

  CtcDeriv ctc_deriv(false);
  ctc_deriv.contract(x, f.eval(x), FORWARD | BACKWARD);
}

int main()
{
  /* =========== PARAMETERS =========== */

    int n = 2;
    Vector epsilon(n, 0.1);
    Interval domain(0.,2.*M_PI);
    TubeVector x(domain, IntervalVector(n, Interval(-9.,9.)));

    IntervalVector init(2);
    init[0] = Interval(1.25,1.3);
    init[1] = Interval(0.25,0.3);
    x.set(init, x.domain().lb());
    init[0] = Interval(1.25,1.3);
    init[1] = Interval(0.25,0.3);
    x.set(init, x.domain().ub());

  /* =========== SOLVER =========== */

    tubex::Solver solver(epsilon, 0.001, 0.005, 1.);
    list<TubeVector> l_solutions = solver.solve(x, &contract);

  return EXIT_SUCCESS;
}