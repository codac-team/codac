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
 *  \author     Simon Rohou (inspired from original pyIbex code of Benoît Desrochers)
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

    void compute(float precision, const TubeVector& x, float L, VIBesFigMap& fig)
    {
      assert(x.size() > 2);
      assert(precision > 0.);

      const IntervalVector& z = box();

      TubeVector u({ cos(x[2] + M_PI/2.), sin(x[2] + M_PI/2.) });
      TubeVector d = z - x.subvector(0,1);

      Tube visibility = u[0]*d[1] - u[1]*d[0];
      Tube scope = abs(u[0]*d[0] + u[1]*d[1]) - L;

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
          fig.draw_box(z, vibesParams("figure", fig.name(), "group", "out"));
          return;
        }

      // Clarity test

        for(const auto& t : v_t)
        {
          if((visibility(t.lb()) * visibility(t.ub())).is_subset(Interval(-oo,0))
              && scope(t).is_subset(Interval(-oo,0)))
          {
            fig.draw_box(z, vibesParams("figure", fig.name(), "group", "in"));
            return;
          }
        }

      // Penumbra test

        // Some extreme trajectories are selected (bounds of the 2d tube x)
        vector<TrajectoryVector> v_a({
          TrajectoryVector({ x[0].lb(), x[1].lb() }),
          TrajectoryVector({ x[0].lb(), x[1].ub() }),
          TrajectoryVector({ x[0].ub(), x[1].ub() }),
          TrajectoryVector({ x[0].ub(), x[1].lb() })
        });

        vector<Tube> v_visibility_a, v_scope_a; // these tubes will be computed only once

        // Clarity test possible for one trajectory

        bool clarity_test_possible = false;

        for(const auto& x_a : v_a)
        {
          TubeVector d_a = TubeVector(x.subvector(0,1)).set(z); // same sampling
          d_a -= x_a;

          Tube visibility_a = u[0]*d_a[1] - u[1]*d_a[0];
          Tube scope_a = abs(u[0]*d_a[0] + u[1]*d_a[1]) - L;

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
        
        // Dark test possible for one trajectory

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
              v_scope_a.push_back(abs(u[0]*d_a[0] + u[1]*d_a[1]) - L);
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

            // Both tests are possible for some different trajectories, then => penumbra
            if(dark_test_possible && clarity_test_possible)
            {
              fig.draw_box(z, vibesParams("figure", fig.name(), "group", "penumbra"));
              return;
            }
          }
        }

      // Unknown cases

        if(box().max_diam() < precision)
        {
          fig.draw_box(z, vibesParams("figure", fig.name(), "group", "unknown"));
        }

        else
        {
          bisect();
          ((ExploredArea*)m_first_subpaving)->compute(precision, x, L, fig);
          ((ExploredArea*)m_second_subpaving)->compute(precision, x, L, fig);
        }
    }
};

int main()
{
  /* =================== 0. Parameters, truth and data =================== */

    float L = 3.2;                                    // sensor range
    float dt = 0.01;                                  // timestep for simulation/tubes accuracy
    Interval tdomain(0,6);                            // temporal limits [t_0,t_f]=[0,3]
    IntervalVector box_area({{-19,16},{-17,10}});     // exploration space

    TrajectoryVector x_truth(tdomain, TFunction("( \
      10*cos(t)-t ; \
      5*sin(2*t)-t ; \
      atan2((10*cos(2*t)-1),(-10*sin(t)-1)) ; \
      sqrt((-10*sin(t)-1)^2+(10*cos(2*t)-1)^2))"));   // actual trajectory


  /* =================== 1. Defining tubes =================== */

    TubeVector x(tdomain, dt, 4);                     // 4d tube for state vectors

    x[2] = Tube(x_truth[2].sample(dt).make_continuous(), dt).inflate(0.015);
    x[3] = Tube(x_truth[3], dt).inflate(0.015);

    // Dead reckoning:
    x[0] = (x[3]*cos(x[2])).primitive(x_truth.first_value()[0]);
    x[1] = (x[3]*sin(x[2])).primitive(x_truth.first_value()[1]);


  /* =================== 2. Graphics =================== */

    vibes::beginDrawing();

    VIBesFigMap fig_map("Map");
    fig_map.set_properties(100, 50, 800, 600);

    // Creating color groups for the paving
    vibes::newGroup("in", "#858585[white]", vibesParams("figure", fig_map.name()));
    vibes::newGroup("out", "#858585[black]", vibesParams("figure", fig_map.name()));
    vibes::newGroup("penumbra", "#858585[#BFBFBF]", vibesParams("figure", fig_map.name()));
    vibes::newGroup("unknown", "#858585[#858585]", vibesParams("figure", fig_map.name()));

    // Adding objects
    fig_map.add_tube(&x, "x", 0, 1);
    fig_map.add_trajectory(&x_truth, "x_truth", 0, 1, "white");

    // Drawing sonar range
    double t = tdomain.lb(), delta_t = tdomain.diam() / 6.;

    do // drawing red swath at different times
    {
      t = min(t, tdomain.ub());

      IntervalVector p = x_truth(t);
      ConvexPolygon swath(IntervalVector({p[0] + Interval(-0.1,0.1), p[1] + Interval(-L,L)}));
      swath.rotate(p[2], p.subvector(0,1));
      fig_map.draw_polygon(swath, "red[red]");
      fig_map.draw_vehicle(t, &x_truth, 1.); // drawing AUV position at t

      t += delta_t;
    } while(t < tdomain.ub() + delta_t);

    fig_map.axis_limits(box_area, true);
    fig_map.show(0.);


  /* =================== 3. Computed the guaranteed explored area =================== */

    ExploredArea paving_area(box_area);

    clock_t t_start = clock();
    paving_area.compute(0.3, x, L, fig_map);
    printf("Paving computation time: %.2fs\n", (double)(clock() - t_start)/CLOCKS_PER_SEC);

    vibes::endDrawing();

  return EXIT_SUCCESS;
}