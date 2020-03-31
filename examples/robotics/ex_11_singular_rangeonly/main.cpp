#include <ibex.h>
#include <tubex.h>
#include <tubex-rob.h>
#include <tubex-3rd.h>

using namespace std;
using namespace ibex;
using namespace tubex;

Vector euler(const Vector& prev_x, const Vector& u, double dt)
{
  Vector x(prev_x);
  x[2] += dt*(u[0]-0.1*pow(x[2],2));
  x[3] += dt*(u[1]);
  x[0] += dt*(x[2]*cos(x[3]));
  x[1] += dt*(x[2]*sin(x[3]));
  return x;
}

Vector controller(const Vector& boat_x, const IntervalVector& estim_scout_x, const Vector& relative_pos)
{
  Vector q({
    boat_x[0]+cos(boat_x[3])*relative_pos[0]-sin(boat_x[3])*relative_pos[1],
    boat_x[1]+sin(boat_x[3])*relative_pos[0]+cos(boat_x[3])*relative_pos[1]
  });

  Vector qdot({
    boat_x[2]*cos(boat_x[3])-boat_x[4]*(q[1]-boat_x[1]),
    boat_x[2]*sin(boat_x[3])+boat_x[4]*(q[0]-boat_x[0])
  });

  Vector e = q-estim_scout_x.mid().subvector(0,1);
  Vector w = qdot+0.2*e;

  Vector u({
    atan(norm(w)-estim_scout_x.mid()[2]),
    atan(tan((atan2(w[1],w[0])-estim_scout_x.mid()[3])/2.))
  });

  if(estim_scout_x.max_diam() > 2.)
    u[1] = 1.;

  return u;
}

int main()
{
  /* =========== CREATING DATA =========== */

    Interval tdomain(0.,150.);
    double dt = 0.1; // timestep
    double anim_dt = 0.12;

    // Boat values

      Vector boat_x0({0.,0.,1.,0.,0.});
      TrajectoryVector boat_u(tdomain, tubex::Function("(0. ; 0.05*(1+sin(0.1*t)))"), dt);

      TrajectoryVector boat_x(5); // (px,py,v,theta,thetadot)
      boat_x[2] = boat_u[0].primitive() + boat_x0[2];
      boat_x[3] = boat_u[1].primitive() + boat_x0[3];
      boat_x[4] = boat_u[1];
      boat_x[0] = (boat_x[2]*cos(boat_x[3])).primitive() + boat_x0[0];
      boat_x[1] = (boat_x[2]*sin(boat_x[3])).primitive() + boat_x0[1];

    // Scouts (two robots)

      // Actual trajectories:
      vector<TrajectoryVector> v_scout_x(2, TrajectoryVector(4));
      vector<TrajectoryVector> v_scout_u(2, TrajectoryVector(2));

      // Unbounded TubeVectors:
      vector<TubeVector> v_scout_tube_x(2, TubeVector(tdomain, dt, 4)); 
      vector<TubeVector> v_scout_tube_xdot(2, TubeVector(tdomain, dt, 4)); 
      vector<TubeVector> v_scout_tube_u(2, TubeVector(tdomain, dt, 2)); // unbounded 2d tube vector


  /* =========== GRAPHICS =========== */

    vibes::beginDrawing();

    VIBesFigMap fig_map("Map");
    fig_map.set_properties(1450, 50, 1000, 600);
    fig_map.add_trajectory(&boat_x, "x*", 0, 1, 3, "red");
    fig_map.add_trajectory(&v_scout_x[0], "scout1*", 0, 1, 3, "green");
    fig_map.add_trajectory(&v_scout_x[1], "scout2*", 0, 1, 3, "blue");
    fig_map.smooth_tube_drawing(true);


  /* =========== CREATING CONTRACTORS =========== */

    // Creating contractors

      tubex::CtcDeriv ctc_deriv;
      tubex::CtcEval ctc_eval;
      tubex::CtcStatic ctc_f(new ibex::CtcFwdBwd(
        *new ibex::Function("xdot[4]", "x[4]", "u[2]",
                            "(xdot[0]-x[2]*cos(x[3]) ; \
                              xdot[1]-x[2]*sin(x[3]) ; \
                              xdot[2]-(u[0]-0.1*x[2]^2) ; \
                              xdot[3]-u[1])")));
      ibex::CtcFwdBwd ctc_dist(*new ibex::Function("x", "y", "bx", "by", "range", "sqrt((x-bx)^2+(y-by)^2)-range"));

    // Creating a contractor network for each scout

      vector<ContractorNetwork> cn(2);
      for(int i = 0 ; i < 2 ; i++)
      {
        cn[i].set_fixedpoint_ratio(0.000001);//01);
        cn[i].add(ctc_deriv, {v_scout_tube_x[i], v_scout_tube_xdot[i]});
        cn[i].add(ctc_f, {v_scout_tube_xdot[i], v_scout_tube_x[i], v_scout_tube_u[i]});
      }


  /* =========== SIMULATION =========== */

    double prev_t = tdomain.lb();
    for(double t = tdomain.lb() ; t < tdomain.ub() ; t += dt)
    {
      double last_ctc_t = (t == tdomain.lb()) ? t : v_scout_tube_x[0][0].slice(t)->prev_slice()->domain().lb();

      for(int i = 0 ; i < 2 ; i++)
      {
        Vector u(2), x(4);

        if(t == tdomain.lb()) // initial condition
        {
          x = (i == 0) ? Vector({3.,2.,0.,0.}) : Vector({4.,-2.,0.,0.});
          u = Vector(2,0.);
        } 

        else // simulation
        {
          // Controller
          IntervalVector estim_scout_x = v_scout_tube_x[i](last_ctc_t);
          u = controller(boat_x(t-dt), estim_scout_x, i == 0 ? Vector({3.,2.}) : Vector({4.,-2.}));

          // Evolution simulation
          x = euler(v_scout_x[i](prev_t), u, dt);
        }

        // Updating simulated values (truth):
        v_scout_x[i].set(x, t);
        v_scout_u[i].set(u, t);

        // Update of CN with new data
        cn[i].add_data(v_scout_tube_u[i], t, IntervalVector(u).inflate(0.1)); // input vector
        cn[i].add_data(v_scout_tube_x[i][2], t, Interval(x[2]).inflate(0.1)); // speed measurement
        cn[i].add_data(v_scout_tube_x[i][3], t, Interval(x[3]).inflate(0.01)); // heading measurement
      }

      for(int i = 0 ; i < 2 ; i++)
      {
        // Estimated state

          // New observations: 
          {
            IntervalVector& p = cn[i].create_var(IntervalVector(4));
            cn[i].add(ctc_eval, {t, p[0], v_scout_tube_x[i][0], v_scout_tube_xdot[i][0]});
            cn[i].add(ctc_eval, {t, p[1], v_scout_tube_x[i][1], v_scout_tube_xdot[i][1]});

            // boat <-> scout
            Interval& d0i = cn[i].create_var(sqrt(pow(boat_x[0](t)-v_scout_x[i][0](t),2)+pow(boat_x[1](t)-v_scout_x[i][1](t),2)));
            d0i.inflate(0.03);
            cn[i].add(ctc_dist, {p[0], p[1], boat_x[0](t), boat_x[1](t), d0i});

            // scout <-> scout
            Interval& dij = cn[i].create_var(sqrt(pow(v_scout_x[(i+1)%2][0](t)-v_scout_x[i][0](t),2)+pow(v_scout_x[(i+1)%2][1](t)-v_scout_x[i][1](t),2)));
            dij.inflate(0.03);
            cn[i].add(ctc_dist, {p[0], p[1], v_scout_x[(i+1)%2][0](t), v_scout_x[(i+1)%2][1](t), dij});
          }
          
          // Estimation
          double contraction_time = cn[i].contract_during(anim_dt);
          //cn[i].contract(true);

        // Display
        usleep(max(0.,anim_dt-contraction_time)*1e6); // animation delay
        vibes::drawBox(v_scout_tube_x[i](last_ctc_t).subvector(0,1));
        cout << v_scout_tube_x[i](last_ctc_t).subvector(0,1) << endl;
      }

      fig_map.set_restricted_tdomain(Interval(tdomain.lb(),t));
      fig_map.show(1.);
      
      prev_t = t;

      //usleep(10000.);
    }

  /* =========== ENDING =========== */
  
  vibes::endDrawing();
  return EXIT_SUCCESS;
}