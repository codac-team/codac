#include <codac>
using namespace codac2;

int main()
{
  // Observation point and obstacle segments
  Vector a({1,1});
  std::vector<Segment> l = {
    {{1,4},{2,3}},
    {{2,3},{2.5,1}},
    {{4,0.5},{3.5,-0.5}}
  };

  // Set up the figure
  DefaultFigure::set_axes(axis(0,{-1,6}),axis(1,{-1,6}));
  
  // Show the observation point and the segments
  DefaultFigure::draw_circle(a, 0.05, {{Color::dark_green(),Color::green()},"w:0.025","z:5"});

  for(const auto& s : l)
    DefaultFigure::draw_line(s, {Color::red(),"w:0.05","z:5"});

  // Paving of the visibility separator
  DefaultFigure::pave(
    {{-1,6},{-1,6}},
    SepVisible(a,l),
    1e-1
  );
}