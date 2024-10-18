/** 
 *  Codac - Examples
 *  Localization of a robot in a map of walls, based on two kinds of observations:
 *   - range-and-bearing: the robot is equipPed with a forward looking sonar
 *   - range-only: the robot only measures its distance to the closest wall
 * ----------------------------------------------------------------------------
 *
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <codac.h>

using namespace std;
using namespace codac;

int main()
{
  /* =================== Parameters, truth and data =================== */

    vector<Vector> v_corners {
      {-4,6},{2.5,6},{2.5,-1},{4.5,-1},{4.5,4},
      {11.5,6},{19,4},{19,-1},{25,-1},{25,-5},
      {16.5,-5},{16.5,1},{14.5,1},{14.5,-5},{8.5,-5},
      {8.5,1},{6.5,1},{6.5,-5},{3,-5},{-4,1}
    };

    vector<Wall> v_walls; // building walls from corners
    for(size_t i = 0 ; i < v_corners.size() ; i++)
      v_walls.push_back({v_corners[i],v_corners[(i+1)%v_corners.size()]});

    float dt = 0.01;        // timestep for tubes accuracy
    float dt_traj = 0.1*dt; // timestep for simulation
    Interval tdomain(0,10); // temporal limits [t_0,t_f]

    // Analytical true trajectory (not used for resolution)
    TrajectoryVector x_truth(tdomain, TFunction("( \
      2*t+(1+cos(t*pi))/2 ; \
      3*tanh(4*(1+cos((t+0.5)/2*pi))/2-2) ; \
      atan2( \
        -3*pi*(1-sqr(tanh(2*cos((2*t+1)*pi/4))))*sin((2*t+1)*pi/4), \
        2-pi*sin(pi*t)/2) ; \
      sqrt( \
        sqr(  -3*pi*(1-sqr(tanh(2*cos((2*t+1)*pi/4))))*sin((2*t+1)*pi/4)  ) + \
        sqr(  2-pi*sin(pi*t)/2  ) \
      ))"));

    // Continuous measurements coming from the truth
    Trajectory& measured_psi = x_truth[2].sample(dt_traj).make_continuous();
    measured_psi += RandTrajectory(tdomain, dt_traj, Interval(-0.01,0.01)); // adding some noise
    Trajectory& measured_speed = x_truth[3].sample(dt_traj);
    measured_speed += RandTrajectory(tdomain, dt_traj, Interval(-0.01,0.01)); // adding some noise

  /* =============== Defining domains for the variables =============== */

    TubeVector x(tdomain, dt, 4); // 4d tube for state vectors
    TubeVector v(tdomain, dt, 4); // 4d tube for derivatives of the states
    TubeVector u(tdomain, dt, 2); // 2d tube for inputs of the system

    x[2] = Tube(measured_psi, dt).inflate(0.05); // measured_psi/speed are continuous measurements..
    x[3] = Tube(measured_speed, dt).inflate(0.1); // ..of the headings and speeds

  /* =============== Defining contractors to deal with equations =============== */

    CtcFunction ctc_f(
      Function("v[4]", "x[4]", "u[2]",
               "(v[0]-x[3]*cos(x[2]) ; v[1]-x[3]*sin(x[2]) ; v[2]-u[0] ; v[3]-u[1])"));

    CtcFunction ctc_plus(Function("a", "b", "c", "a+b-c")); // constraint a+b-c=0 <=> a+c=c
    CtcFunction ctc_minus(Function("a", "b", "c", "a-b-c")); // constraint a-b-c=0 <=> a-b=c

    CtcUnion ctc_walls(2); // contractor for the constraint \mathbf{q} \in \matbb{W} with \mathbb{W} the set of walls
    for(const auto& wi : v_walls)
      ctc_walls |= CtcSegment(wi.c1[0],wi.c1[1],wi.c2[0],wi.c2[1]);

    // We also use the predefined contractors ctc::polar, ctc::eval, ctc::dist,  no need to build them

  /* =============== Adding the contractors to a network =============== */

    ContractorNetwork cn;     // creating a network
    cn.add(ctc_f, {v, x, u}); // adding the f contractor, that is common over [t0,tf]

    vibes::beginDrawing();
    VIBesFigMap fig("Map");
    fig.set_properties(50, 50, 900, 500);
    fig.add_tube(&x, "x", 0, 1);
    fig.add_trajectory(&x_truth, "x*", 0, 1, 2);
    fig.smooth_tube_drawing(true);
    fig.show(0.);
    fig.axis_limits(-5,26,-8.5,8.5);

    for(const auto& wi : v_walls) // drawing the map of walls
      fig.draw_line({wi.c1[0],wi.c2[0]}, {wi.c1[1],wi.c2[1]}, "lightGray");

    bool dist_only = !true; // in order to test two scenarii

    if(dist_only) // For this scenario we assume that \mathbf{x}(0) is known
    {
      Interval& t0 = cn.create_interm_var(0.);
      IntervalVector& p0 = cn.create_interm_var(x_truth(0.));
      cn.add(ctc::eval, {t0, p0, x, v}); // <=> x(t0)=p0
    }

    // For each observation of the environment:
    for(double t = tdomain.lb() ; t < tdomain.ub() ; t += 0.8)
    {
      Vector pi_truth = x_truth(t).subvector(0,1);

      Interval& ti = cn.create_interm_var(Interval(t));
      IntervalVector& pi = cn.create_interm_var(IntervalVector(4));
      IntervalVector& qi = cn.create_interm_var(IntervalVector(2));

      cn.add(ctc::eval, {ti, pi, x, v}); // <=> x(ti)=pi
      cn.add(ctc_walls, {qi}); // <=> \mathbf{q]_i \in \mathbb{W}

      if(!dist_only) // range-and-bearing observation (forward-looking sonar)
      {
        IntervalVector& di = cn.create_interm_var(IntervalVector(2));
        IntervalVector& yi = cn.create_interm_var(IntervalVector(2));
        yi[0] = shorter_dist_to_walls(v_walls, pi_truth, x_truth(t)[2]);
        yi[1] = x_truth(t)[2]; // bearing is zero, azimuth is the heading

        yi[0].inflate(0.02); yi[1].inflate(0.05);

        cn.add(ctc_minus, {qi, cn.subvector(pi,0,1), di}); // <=> qi-pi[0,1]=di
        cn.add(ctc::polar, {di, yi}); // di[0]=y1[0]*cos(yi[1]), di[1]=y1[0]*sin(yi[1])

        fig.draw_pie(pi_truth[0], pi_truth[1], yi[0] | 0., yi[1], "lightGray[#FFFFFF88]");
        fig.draw_pie(pi_truth[0], pi_truth[1], yi[0], yi[1]);
      }

      else // range-only observation
      {
        Interval& yi = cn.create_interm_var(shorter_contact_to_walls(v_walls, pi_truth)[0]);
        yi.inflate(0.02);

        cn.add(ctc::dist, {cn.subvector(pi,0,1), qi, yi}); // <=> (pi[0]-qi[0])^2+(pi[1]-qi[1])^2=yi^2

        fig.draw_circle(pi_truth[0], pi_truth[1], yi.mid(), "black[#80808088]");
      }

      vibes::drawVehicle(x_truth(t)[0], x_truth(t)[1], x_truth(t)[2]*180/M_PI, 0.7, "black[yellow]");
    }
    
    cn.contract(true);

    fig.show(0.);
    fig.axis_limits(-5,26,-8.5,8.5);

    vibes::endDrawing();
    return x.contains(x_truth) != BoolInterval::NO ? EXIT_SUCCESS : EXIT_FAILURE;
}