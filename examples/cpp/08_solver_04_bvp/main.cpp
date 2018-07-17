#include "ibex.h"
#include "tubex.h"

using namespace std;
using namespace ibex;
using namespace tubex;

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
  
  // Differential equation

    tubex::Function f("x", "x");

    CtcPicard ctc_picard(true);
    ctc_picard.contract(f, x);
    
    CtcDeriv ctc_deriv(true);
    ctc_deriv.contract(x, f.eval(x));
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

    tubex::Solver solver;
    vector<TubeVector> v_solutions = solver.solve(x, &contract, epsilon);

  /* =========== GRAPHICS =========== */

    vibes::beginDrawing();
    VibesFigure_Tube fig("Solver", x.dim());
    fig.set_properties(100,100,700,500);
    fig.add_trajectory(&truth1, "truth1", "blue");
    fig.add_trajectory(&truth2, "truth2", "red");

    for(int i = 0 ; i < v_solutions.size() ; i++)
    {
      cout << (i+1) << ": " << v_solutions[i] <<  ", tf↦" << v_solutions[i][v_solutions[i].domain().ub()] << endl;
      ostringstream o; o << "solution_" << i;
      fig.add_tube(&v_solutions[i], o.str());
    }

    fig.show(true);
    vibes::endDrawing();


  // Checking if this example is still working:
  return (solver.solution_encloses(v_solutions, truth1)
       && solver.solution_encloses(v_solutions, truth2)) ? EXIT_SUCCESS : EXIT_FAILURE;
}