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

    const IntervalVector eval(const Interval& t, const TubeVector& x) const
    {
      IntervalVector eval_result(x.dim());
      PeriodicFunction pf;

      IntervalVector intv_1 = pf.eval(t - 1., x);
      IntervalVector intv_075 = pf.eval(t - 0.75, x);
      IntervalVector intv_05 = pf.eval(t - 0.5, x);
      IntervalVector intv_025 = pf.eval(t - 0.25, x);

      eval_result[0] = x[t][0]
        * (3. - sin(t) - (3. - cos(t)) * x[t][0] 
            - (((2. + sin(t)) / 24.) * (intv_1[1] + 6.*intv_075[1] + 4.*intv_05[1] + 10.*intv_025[1] + 3*x[t][1])));
      eval_result[1] = x[t][1]
        * (6. - cos(t) - (10. - sin(t)) * x[t][1]
            - (((2. + sin(t)) / 24.) * (intv_1[0] + 6.*intv_075[0] + 4.*intv_05[0] + 10.*intv_025[0] + 3*x[t][0])));

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
    Vector epsilon(n, 0.4);
    Interval domain(0.,2.*M_PI);
    IntervalVector codomain(2);
    codomain[0] = Interval(0.3,1.4);
    codomain[1] = Interval(0.2,0.625);
    TubeVector x(domain, codomain);

    IntervalVector init(2);
    init[0] = Interval(1.25,1.3);
    init[1] = Interval(0.25,0.3);
    x.set(init, x.domain().lb());
    init[0] = Interval(1.25,1.3);
    init[1] = Interval(0.25,0.3);
    x.set(init, x.domain().ub());

  /* =========== SOLVER =========== */

    tubex::Solver solver(epsilon, 0.01, 0.01, 1.);
    list<TubeVector> l_solutions = solver.solve(x, &contract);

  return EXIT_SUCCESS;
}