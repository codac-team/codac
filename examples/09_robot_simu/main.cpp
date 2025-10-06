#include <codac>
using namespace codac2;

int main()
{
  std::list<Vector> wpts {
    {2,1}, {4,0}, {6,2}, {8,0}, {6,2}
  };

  RobotSimulator s;
  SampledTraj<Vector> u;
  auto x = s.simulate({0,0,0,0}, 1e-2, wpts, u);

  Figure2D g("Robot simulation", GraphicOutput::VIBES);

  g.draw_tank(x(5.), 0.5, {Color::black(),Color::yellow()});
  g.draw_trajectory(x);
  for(const auto& wpt : wpts)
    g.draw_circle(wpt, 0.1, Color::red());

  g.set_axes(
    {0,x.codomain()[0].inflate(1.)},
    {1,x.codomain()[1].inflate(1.)}
  ).auto_scale();
}