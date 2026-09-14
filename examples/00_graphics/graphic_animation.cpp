#include <codac>
#include <chrono>
#include <thread>

using namespace std;
using namespace codac2;

int main()
{
  Figure2D fig ("Animation",GraphicOutput::VIBES|GraphicOutput::IPE);
  fig.set_window_properties({50,50},{500,500}); // position, window size
  fig.set_axes(axis(0,{-10,10}), axis(1,{-10,10}));

  double theta=0.;
  int steps = 30;

  for (int i = 0; i < steps; ++i)
  {
    fig.clear();
    fig.draw_point({5.0*cos(theta), 5.0*sin(theta)}, Color::red());
    theta += 2.0 * M_PI / (double) steps;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  Figure2D fig_save ("Animation with exports",GraphicOutput::VIBES|GraphicOutput::IPE);
  fig_save.set_window_properties({600,50},{500,500}); // position, window size
  fig_save.set_axes(axis(0,{-0.5,4}), axis(1,{-0.5,1.5}));

  Vector X({0,0,0});

  ColorMap cmap = ColorMap::rainbow();
  for (double t = 0; t < 2.*PI; t+=PI/20.)
  {
    fig_save.draw_tank(X, 0.05, cmap.color(t/(2.*PI)));
    X+=Vector({0.1*cos(X[2]),0.1*sin(X[2]),0.1*cos(t)});
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    if (Interval(PI).inflate(0.1).contains(t))
    {
      fig_save.save("animation_tank.png");
      fig_save.save("animation_tank.jpg");
      fig_save.save("animation_tank.bmp");
      fig_save.save("animation_tank.svg");
    }
  }
}