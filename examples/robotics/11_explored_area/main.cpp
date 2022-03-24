/** 
 *  Codac - Examples
 *  Guaranteed explored area
 * ----------------------------------------------------------------------------
 *
 *  \brief      Example from the paper "Computing a Guaranteed Approximation of the Zone Explored by a Robot"
 *              Benoît Desrochers, Luc Jaulin
 *              IEEE Transactions on Automatic Control, 62(1):425-430

 *  \brief      Guaranteed explored area
 *  \date       2021
 *  \author     Simon Rohou, Benoît Desrochers
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <codac.h>

using namespace std;
using namespace codac;

class ExploredArea : public Paving
{
  public:

    ExploredArea(const IntervalVector& init_box) : Paving(init_box, SetValue::UNKNOWN)
    {

    }

    void compute(float precision, const TubeVector& x)
    {
      assert(precision > 0.);

      const IntervalVector& z = box();

      TubeVector u({ cos(x[2] + M_PI/2.), sin(x[2] + M_PI/2.) });
      TubeVector d = z - x.subvector(0,1);

      Tube visibility = u[0]*d[1] - u[1]*d[0];
      Tube scope = abs(u[0]*d[0] + u[1]*d[1]) - 2.;

      vector<Interval> v_t; // kernel of visibility tube
      visibility.invert(0., v_t, visibility.tdomain());

      // Dark test

      bool dark_test = true;
      for(const auto& t : v_t)
      {
        if(!scope(t).is_subset(Interval(0,oo)))
        {
          dark_test = false;
          break;
        }
      }

      if(dark_test)
      {
        set_value(SetValue::OUT);
        return;
      }

      // Clarity test

      for(const auto& t : v_t)
      {
        if((visibility(t.lb()) * visibility(t.ub())).is_subset(Interval(-oo,0))
            && scope(t).is_subset(Interval(-oo,0)))
        {
          set_value(SetValue::IN);
          return;
        }
      }

      // Penumbra test

      bool clarity_test_possible = false;

      vector<TrajectoryVector> v_a({
        TrajectoryVector({ x[0].lb(), x[1].lb() }),
        TrajectoryVector({ x[0].lb(), x[1].ub() }),
        TrajectoryVector({ x[0].ub(), x[1].ub() }),
        TrajectoryVector({ x[0].ub(), x[1].lb() })
      });

      vector<Tube> v_visibility_a, v_scope_a;

      // Clarity test possible
      for(const auto& x_a : v_a)
      {
        TubeVector d_a = TubeVector(x.subvector(0,1)).set(z); // same sampling
        d_a -= x_a;

        Tube visibility_a = u[0]*d_a[1] - u[1]*d_a[0];
        Tube scope_a = abs(u[0]*d_a[0] + u[1]*d_a[1]) - 2.;

        v_visibility_a.push_back(visibility_a);
        v_scope_a.push_back(scope_a);

        vector<Interval> v_t_a;
        visibility_a.invert(0., v_t_a, visibility_a.tdomain());

        for(const auto& t : v_t_a)
        {
          if((visibility_a(t.lb()) * visibility_a(t.ub())).is_subset(Interval(-oo,0))
            && scope_a(t).is_subset(Interval(-oo,0)))
          {
            clarity_test_possible = true;
            break;
          }
        }

        if(clarity_test_possible)
          break;
      }
      
      // Dark test possible

      if(clarity_test_possible)
      {
        for(size_t i = 0 ; i < v_a.size() ; i++)
        {
          bool dark_test_possible = true;

          if(i >= v_visibility_a.size())
          {
            TubeVector d_a = TubeVector(x.subvector(0,1)).set(z); // same sampling
            d_a -= v_a[i];

            v_visibility_a.push_back(u[0]*d_a[1] - u[1]*d_a[0]);
            v_scope_a.push_back(abs(u[0]*d_a[0] + u[1]*d_a[1]) - 2.);
          }

          vector<Interval> v_t_a;
          v_visibility_a[i].invert(0., v_t_a, v_visibility_a[i].tdomain());

          for(const auto& t : v_t_a)
          {
            if(!v_scope_a[i](t).is_subset(Interval(0,oo)))
            {
              dark_test_possible = false;
              break;
            }
          }

          if(dark_test_possible && clarity_test_possible)
          {
            set_value(SetValue::PENUMBRA);
            return;
          }
        }
      }

      if(box().max_diam() < precision)
      {
        /*VIBesFigTube fig_scope("scope", &scope);
        fig_scope.set_properties(100, 800, 500, 250);
        fig_scope.show();

        VIBesFigTube fig_visibility("visibility", &visibility);
        fig_visibility.set_properties(600, 800, 500, 250);
        fig_visibility.show();*/

        set_value(SetValue::UNKNOWN);
      }

      else
      {
        bisect();
        ((ExploredArea*)m_first_subpaving)->compute(precision, x);
        ((ExploredArea*)m_second_subpaving)->compute(precision, x);
      }
    }
};

int main()
{
  /* =================== 0. Parameters, truth and data ==================== */

    float dt = 0.01;                                  // timestep for simulation/tubes accuracy
    Interval tdomain(0,3);                           // temporal limits [t_0,t_f]=[0,3]

    TrajectoryVector x_truth(tdomain, TFunction("( \
      10*cos(t)+t ; \
      5*sin(2*t)+t ; \
      atan2((10*cos(2*t)+1),(-10*sin(t)+1)) ; \
      sqrt((-10*sin(t)+1)^2+(10*cos(2*t)+1)^2))")); // actual trajectory

    // Continuous measurements coming from the truth
    Trajectory& measured_psi = x_truth[2].sample(dt).make_continuous();
    measured_psi += RandTrajectory(tdomain, dt, Interval(-0.03,0.03)); // adding some noise
    Trajectory& measured_speed = x_truth[3].sample(dt);
    measured_speed += RandTrajectory(tdomain, dt, Interval(-0.03,0.03)); // adding some noise


  /* =============== 1. Defining domains for our variables ================ */

    TubeVector x(tdomain, dt, 4);                     // 4d tube for state vectors
    TubeVector u(tdomain, dt, 2);                     // 2d tube for inputs of the system

    x[2] = Tube(measured_psi, dt).inflate(0.03);      // measured_psi is a set of measurements
    x[3] = Tube(measured_speed, dt).inflate(0.03);


  /* =========== 2. Defining contractors to deal with equations =========== */

    TubeVector v({
      x[3]*cos(x[2]),
      x[3]*sin(x[2]),
      u[0], u[1] });

    x[0] = v[0].primitive(x_truth.first_value()[0]);
    x[1] = v[1].primitive(x_truth.first_value()[1]);


  /* =========== GRAPHICS =========== */

    vibes::beginDrawing();

    VIBesFigMap fig_map("Map");
    fig_map.set_properties(100, 50, 500, 500);
    fig_map.add_tube(&x, "x", 0, 1);
    fig_map.add_trajectory(&x_truth, "x_truth", 0, 1, "white");
    fig_map.show(0.);


  /* =========== 2.  =========== */

    x[0] = v[0].primitive(x_truth.first_value()[0]);
    x[1] = v[1].primitive(x_truth.first_value()[1]);

    //IntervalVector z({ Interval(20), Interval(20) });
    //z.inflate(0.05);
    IntervalVector z = fig_map.view_box();
    z.inflate(4.);
    ExploredArea area(z);

    clock_t t_start = clock();
    x.enable_synthesis(SynthesisMode::BINARY_TREE);
    cout << "Computing the explored area... (may take some time)" << endl;
    area.compute(0.1, x);
    printf("Paving computation time: %.2fs\n", (double)(clock() - t_start)/CLOCKS_PER_SEC);


  /* =========== 2.  =========== */

    VIBesFigPaving fig_pav("Explored area", &area);
    fig_pav.set_properties(600, 50, 800, 800);

    map<SetValue,string> colormap;
    colormap[SetValue::IN] = "#858585[white]";
    colormap[SetValue::OUT] = "#858585[black]";
    colormap[SetValue::PENUMBRA] = "#858585[#BFBFBF]";
    colormap[SetValue::UNKNOWN] = "#858585[#858585]";
    fig_pav.set_color_map(colormap);
    fig_pav.show();

    vibes::endDrawing();

  return EXIT_SUCCESS;
}