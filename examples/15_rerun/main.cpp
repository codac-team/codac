#include <codac>
#include <iostream>

using namespace std;
using namespace codac2;

int main()
{
  cout << "=== Codac Rerun Visualizer Example ===" << endl;

  // =========================================================================
  // 1. 2D Figure with Rerun
  // =========================================================================
  cout << "Creating 2D figure with Rerun..." << endl;
  Figure2D fig2d("Codac_2D_Rerun", GraphicOutput::RERUN);
  fig2d.set_axes(axis(0, {-10, 10}, "x"), axis(1, {-10, 10}, "y"));

  // Boxes
  fig2d.draw_box({{-2, 2}, {-2, 2}}, {Color::blue(), Color::cyan(0.4)});
  fig2d.draw_box({{4, 6}, {4, 7}}, {Color::red(), Color::yellow(0.5)});

  // Robots
  fig2d.draw_tank({2.0, 1.0, 3.14 / 4.0}, 1.5, {{Color::black(), Color::green()}, "vehicles"});
  fig2d.draw_AUV({-4.0, 3.0, -3.14 / 6.0}, 2.0, {{Color::black(), Color::yellow()}, "vehicles"});
  fig2d.draw_motor_boat({-3.0, -4.0, 0.0}, 1.5, {{Color::black(), Color::blue()}, "vehicles"});

  // Circle and Ellipse
  fig2d.draw_circle({0.0, 0.0}, 3.0, Color::purple());
  fig2d.draw_ellipse({-3.0, 2.0}, {1.0, 2.5}, 0.5, {Color::dark_green(), Color::green(0.3)});

  // 2D Trajectory
  SampledTraj<Vector> traj2d;
  for(double t = 0.0; t <= 10.0; t += 0.1)
  {
    traj2d.set(Vector({t - 5.0, 3.0 * std::sin(t)}), t);
  }
  fig2d.draw_trajectory(traj2d, {Color::red(), "trajectory", "w:2.0"});

  // 2D Tube
  SlicedTube<IntervalVector> tube2d(create_tdomain({0.0, 5.0}, 0.2), IntervalVector(2));
  for(auto it = tube2d.tdomain()->begin(); it != tube2d.tdomain()->end(); ++it)
  {
    double t = it->mid();
    IntervalVector slice_box({ {t - 2.5 - 0.3, t - 2.5 + 0.3}, {std::cos(t) - 0.4, std::cos(t) + 0.4} });
    tube2d.slice(it)->set(slice_box, false);
  }
  fig2d.draw_tube(tube2d, {Color::blue(0.8), "tube"});

  // Lines, arrows, polylines
  fig2d.draw_line({-8.0, -8.0}, {-2.0, -5.0}, Color::dark_gray());
  fig2d.draw_arrow({0.0, -6.0}, {3.0, -6.0}, 0.5, Color::red());
  fig2d.draw_text("Codac 2D Rerun Export", {-8.0, 8.0}, 1.0, Color::black());

  // Save 2D figure to .rrd file
  fig2d.save("figure_2d.rrd");
  cout << "Saved 2D figure to figure_2d.rrd" << endl;


  // =========================================================================
  // 2. 3D Figure with Rerun
  // =========================================================================
  cout << "Creating 3D figure with Rerun..." << endl;
  Figure3D fig3d("Codac_3D_Rerun", GraphicOutput::RERUN);

  // Coordinate axes
  fig3d.draw_axes(2.0, Vector::zero(3));

  // 3D Boxes
  fig3d.draw_box({{-1, 1}, {-1, 1}, {-1, 1}}, {Color::blue(0.6), "box3d"});
  fig3d.draw_box({{3, 5}, {3, 5}, {1, 3}}, {Color::red(0.6), "box3d"});

  // 3D Vehicles / Robots
  fig3d.draw_car({-3, 0, 0}, 0.5 * Matrix::eye(3, 3), {Color::yellow(0.8), "robots"});
  fig3d.draw_plane({3, 0, 2}, 0.6 * Matrix::eye(3, 3), true, {Color::dark_gray(0.8), "robots"});
  fig3d.draw_AUV({0, -4, 1}, 0.7 * Matrix::eye(3, 3), {Color::cyan(0.8), "robots"});

  // 3D Trajectory
  SampledTraj<Vector> traj3d;
  for(double t = 0.0; t <= 10.0; t += 0.1)
  {
    double r = 3.0;
    traj3d.set(Vector({r * std::cos(t), r * std::sin(t), 0.5 * t}), t);
  }
  fig3d.draw_trajectory(traj3d, {Color::red(), "trajectory", "w:2.0"});

  // 3D Sphere and Ellipsoid
  fig3d.draw_sphere({0, 0, 4}, Matrix::eye(3, 3), {Color::green(0.6), "sphere"});
  fig3d.draw_ellipsoid(Ellipsoid({-3, -3, 2}, Matrix({{1.5, 0, 0}, {0, 0.8, 0}, {0, 0, 0.5}})),
                       {Color::purple(0.6), "ellipsoid"});

  // 3D Arrow and Zonotope
  fig3d.draw_arrow({0, 0, 0}, 2.0 * Matrix::eye(3, 3), Color::red());
  fig3d.draw_zonotope({{2, -2, 2},
                       {{0.3, -0.2, 0.1},
                        {0.1,  0.3, 0.0},
                        {0.0,  0.1, 0.4}}},
                      {Color::dark_green(0.8), "zonotope"});

  // 3D Paving with contractor
  VectorVar x(3);
  AnalyticFunction f { {x},
    {
      sqr(x[0]) + sqr(x[1]) + sqr(x[2]) - 4.0
    }
  };
  CtcInverse ctc(f, IntervalVector({Interval(-0.2, 0.2)}));
  auto p_ctc = pave({{-3, 3}, {-3, 3}, {-3, 3}}, ctc, 0.3);
  fig3d.draw_paving(p_ctc, {Color::yellow(0.5), "paving"});

  // Save 3D figure to .rrd file
  fig3d.save("figure_3d.rrd");
  cout << "Saved 3D figure to figure_3d.rrd" << endl;

  cout << "Visualization files successfully generated!" << endl;
  cout << "To view in Rerun: rerun figure_2d.rrd (or rerun figure_3d.rrd)" << endl;

  return 0;
}
