#include <codac>

using namespace std;
using namespace codac2;

void show_color(Color (*color)(float), int i)
{
  DefaultFigure::draw_box(
    IntervalVector({ std::floor(i/3.), (double)(i%3) }).inflate(.4),
    { color(1.), color(1.) });
}

int main()
{
  int i = 0;

  show_color(&Color::light_gray, i++);
  show_color(&Color::gray, i++);
  show_color(&Color::dark_gray, i++);

  show_color(&Color::light_green, i++);
  show_color(&Color::green, i++);
  show_color(&Color::dark_green, i++);

  show_color(&Color::light_blue, i++);
  show_color(&Color::blue, i++);
  show_color(&Color::dark_blue, i++);

  show_color(&Color::light_cyan, i++);
  show_color(&Color::cyan, i++);
  show_color(&Color::dark_cyan, i++);

  show_color(&Color::light_yellow, i++);
  show_color(&Color::yellow, i++);
  show_color(&Color::dark_yellow, i++);

  show_color(&Color::light_orange, i++);
  show_color(&Color::orange, i++);
  show_color(&Color::dark_orange, i++);

  show_color(&Color::light_red, i++);
  show_color(&Color::red, i++);
  show_color(&Color::dark_red, i++);

  show_color(&Color::light_brown, i++);
  show_color(&Color::brown, i++);
  show_color(&Color::dark_brown, i++);

  show_color(&Color::light_purple, i++);
  show_color(&Color::purple, i++);
  show_color(&Color::dark_purple, i++);

  show_color(&Color::light_pink, i++);
  show_color(&Color::pink, i++);
  show_color(&Color::dark_pink, i++);

  DefaultFigure::set_axes(axis(0,{-.5,(i/3)-1+.5}), axis(1,{-.5,2+.5}));
  DefaultFigure::selected_fig()->auto_scale();
}
