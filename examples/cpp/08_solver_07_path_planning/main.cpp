#include "ibex.h"
#include "tubex.h"

using namespace std;
using namespace ibex;
using namespace tubex;

double obstacle_radius = 0.95;

void createObstacles(vector<IntervalVector>& v_obstacles);
void displayPathPlanningMap(const list<TubeVector>& l_x, const vector<IntervalVector>& v_obstacles);

void contract(TubeVector& x)
{
  vector<IntervalVector> v_obstacles;
  createObstacles(v_obstacles);
  {
    Variable vx, vt, vy, vcos, vsin;
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
    tubex::CtcHC4 ctc_hc4(true);
    ctc_hc4.contract(hc4, x);
  }

  tubex::Function f("x","y", "costheta", "sintheta",
                    "(costheta;sintheta;0.25*[-1,0];0.25*[-1,0])");
  TubeVector v = f.eval(x);

  {
    CtcEval ctc_eval(true, false);
    Interval t(4., 4.06);
    IntervalVector z(4);
    z[0] = Interval(5.98,6.06);
    z[1] = Interval(4.89,4.91);
    z[2] = Interval(0.9525,0.9755);
    z[3] = Interval(-0.304,-0.22);
    ctc_eval.contract(t, z, x, v);
  }

  {
    CtcEval ctc_eval(true, true);
    Interval t(7.9374,8.);
    IntervalVector z(4);
    z[0] = Interval(8.49735, 8.5);
    z[1] = Interval(2.23252, 2.26765);
    z[2] = Interval(0.1262, 0.229281);
    z[3] = Interval(-0.992005, -0.97336);
    ctc_eval.contract(t, z, x, v);
  }
}

int main(int argc, char *argv[])
{
  /* =========== PARAMETERS =========== */

    int n = 4;

    Vector epsilon(n);
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

    vector<IntervalVector> v_obstacles;
    createObstacles(v_obstacles);

  /* =========== SOLVER =========== */

    tubex::Solver solver(epsilon, 0.005, 0.005, 1.);
    list<TubeVector> l_solutions = solver.solve(x, &contract);

  /* =========== GRAPHICS =========== */

    vibes::beginDrawing();
    displayPathPlanningMap(l_solutions, v_obstacles);
    vibes::endDrawing();


  // Checking if this example is still working:
  return (!l_solutions.empty()) ? EXIT_SUCCESS : EXIT_FAILURE;
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

void displayPathPlanningMap(const list<TubeVector>& l_x, const vector<IntervalVector>& v_obstacles)
{
  const string fig_name = "Map (top view): [x](·)x[y](·)";
  const int slices_number_to_display = 500;

  vibes::newFigure(fig_name);
  vibes::setFigureProperties(
            vibesParams("figure", fig_name, "x", 100, "y", 100, "width", 600, "height", 600));
  vibes::axisLimits(0., 10., 0., 10.);

  for(int i = 0 ; i < v_obstacles.size() ; i++)
    vibes::drawCircle(v_obstacles[i][0].mid(), v_obstacles[i][1].mid(), obstacle_radius, "#779CA3[#A5C8CE]", vibesParams("figure", fig_name));

  list<TubeVector>::const_iterator it;
  for(it = l_x.begin(); it != l_x.end(); ++it)
  {
    // Robot's tubes projection
    int startpoint;
    for(int i = 0 ; i < it->nb_slices() ; i += max((int)(it->nb_slices() / slices_number_to_display), 1))
      startpoint = i;

    for(int i = startpoint ; i >= 0; i -= max((int)(it->nb_slices() / slices_number_to_display), 1))
    {
      Interval intv_x = (*it)[i][0];
      Interval intv_y = (*it)[i][1];
      if(!intv_x.is_unbounded() && !intv_y.is_unbounded())
        vibes::drawBox(intv_x.lb(), intv_x.ub(), intv_y.lb(), intv_y.ub(),
                       "#DEDEDE[#DEDEDE]", vibesParams("figure", fig_name));
    }
  }
}