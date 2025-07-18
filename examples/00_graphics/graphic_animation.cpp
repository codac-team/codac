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
}