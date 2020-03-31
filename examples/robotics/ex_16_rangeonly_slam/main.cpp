#include <ibex.h>
#include <tubex.h>
#include <tubex-rob.h>

using namespace std;
using namespace ibex;
using namespace tubex;

int main()
{
  /* =========== CREATING DATA =========== */

    srand(time(NULL)); // for random values

    float dt = 0.01;
    float anim_dt = 0.04; // animation delay
    Interval tdomain(0.,15.); // [t0,tf]
    
    // Initial pose x0=(0,0,2)
    Vector x0({0.,0.,2.});

    // System input
    Trajectory u = Trajectory(tdomain, tubex::Function("3*(sin(t)^2)+t/100"));

    // Noise
    Interval i_n(-0.03,0.03); // the noises are known to be bounded by i_n
    Trajectory n_u = RandTrajectory(tdomain, dt, i_n); // input noise
    Trajectory n_theta = RandTrajectory(tdomain, dt, i_n); // heading noise

    // Actual trajectories (state + derivative)
    TrajectoryVector v_truth(3), x_truth(3);
    v_truth[2] = u + n_u;
    x_truth[2] = v_truth[2].primitive() + x0[2];
    v_truth[0] = 10.*cos(x_truth[2]);
    v_truth[1] = 10.*sin(x_truth[2]);
    x_truth[0] = v_truth[0].primitive() + x0[0];
    x_truth[1] = v_truth[1].primitive() + x0[1];

    // Bounded trajectories (dead reckoning)
    TubeVector v(tdomain, dt, 3), x(tdomain, dt, 3);
    v[2] = Tube(u, dt).inflate(i_n.rad()); // command u with bounded uncertainties
    x[2] = Tube(x_truth[2] + n_theta, dt).inflate(i_n.rad()); // heading measurement with bounded uncertainties
    v[0] = 10.*cos(x[2]);
    v[1] = 10.*sin(x[2]);
    x[0] = v[0].primitive() + x0[0]; // dead reckoning
    x[1] = v[1].primitive() + x0[1];

    // Set of landmarks
    vector<Vector> v_m;
    v_m.push_back({6.,12.});
    v_m.push_back({-2.,-5.});
    v_m.push_back({-3.,20.});
    v_m.push_back({3.,4.});


  /* =========== GRAPHICS =========== */

    vibes::beginDrawing();

    VIBesFigMap fig_map("Map");
    fig_map.set_properties(50, 50, 900, 900);
    fig_map.add_tube(&x, "x", 0, 1);
    fig_map.add_trajectory(&x_truth, "truth", 0, 1, "white");
    fig_map.smooth_tube_drawing(true);
    fig_map.add_landmarks(v_m, 0.4);
    fig_map.show(0.);


  /* =========== CONTRACTOR NETWORK =========== */

    vector<IntervalVector> v_m_boxes(v_m.size(), IntervalVector(2));

    // Creating contractors:

    CtcDeriv ctc_deriv;
    CtcEval ctc_eval;
    ibex::CtcFwdBwd ctc_dist(*new ibex::Function("x", "y", "bx", "by", "range", "sqrt((x-bx)^2+(y-by)^2)-range"));

    // Contractor Network:

    ContractorNetwork cn;

    cn.add(ctc_deriv, {x, v});
    cn.contract(true);

    double prev_t_obs = tdomain.lb();
    for(double t = tdomain.lb() ; t < tdomain.ub() ; t+=dt)
    {
      if(t - prev_t_obs > 2*dt) // new observation each dt*10 seconds
      {
        // Creating new observation to a random landmark

          int landmark_id = rand() % v_m.size(); // a random landmark is perceived

          Vector pos_x = x_truth(t).subvector(0,1);
          Vector pos_b = v_m[landmark_id];

          Interval y = sqrt(pow(pos_x[0]-pos_b[0],2)+pow(pos_x[1]-pos_b[1],2));
          y.inflate(0.03); // adding range bounded uncertainty

          prev_t_obs = t;

        // Adding related observation constraints to the network

          // Alias (for ease of reading)
          IntervalVector &b = v_m_boxes[landmark_id];

          // Intermediate variables
          Interval& ti = cn.create_var(t);
          Interval& yi = cn.create_var(y);
          IntervalVector& xi = cn.create_var(IntervalVector(3));

          // Contractors
          cn.add(ctc_eval, {ti, xi, x, v});
          cn.add(ctc_dist, {xi[0], xi[1], b[0], b[1], yi});
      }

      double contraction_time = cn.contract_during(anim_dt);
      usleep(max(0.,anim_dt-contraction_time)*1e6); // animation delay

      fig_map.draw_box(x(max(0.,ibex::previous_float(t))).subvector(0,1));
    }

    fig_map.show();


  /* =========== ENDING =========== */

    vibes::endDrawing();
    return EXIT_SUCCESS;
}