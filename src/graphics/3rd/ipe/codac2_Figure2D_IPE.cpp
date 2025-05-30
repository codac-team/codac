/** 
 *  codac2_Figure2D_IPE.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <cstdio>
#include <sstream>
#include "codac2_Figure2D_IPE.h"
#include "codac2_math.h"

using namespace std;
using namespace codac2;


Figure2D_IPE::Figure2D_IPE(const Figure2D& fig)
  : OutputFigure2D(fig), _f(fig.name() + ".xml"),
    _f_temp_content(fig.name() + "_tmp.xml"),
    _x_offset(0.03*_fig.axes()[0].limits.diam()),
    _y_offset(0.03*_fig.axes()[1].limits.diam())
{
  list<Color> codac_colors {
    Color::none(), Color::black(), Color::white(),
    Color::light_gray(), Color::gray(), Color::dark_gray(),
    Color::light_green(), Color::green(), Color::dark_green(),
    Color::light_blue(), Color::blue(), Color::dark_blue(),
    Color::light_cyan(), Color::cyan(), Color::dark_cyan(),
    Color::light_yellow(), Color::yellow(), Color::dark_yellow(),
    Color::light_orange(), Color::orange(), Color::dark_orange(),
    Color::light_red(), Color::red(), Color::dark_red(),
    Color::light_brown(), Color::brown(), Color::dark_brown(),
    Color::light_purple(), Color::purple(), Color::dark_purple(),
    Color::light_pink(), Color::pink(), Color::dark_pink()
  };

  for(const auto& ci : codac_colors)
    // substr is needed to remove the "#" at the beginning of hex_str (deprecated by IPE)
    _colors.emplace(ci.hex_str().substr(1), ci);

  _layers.push_back("alpha");
  _layers.push_back("axes");
}

Figure2D_IPE::~Figure2D_IPE()
{ 
  draw_axes();
  print_header_page();
  _f_temp_content.close();
  _f.close();

  _f = std::ofstream(_fig.name() + ".xml", std::ofstream::binary | std::ofstream::app);
  std::ifstream f_temp_content(_fig.name() + "_tmp.xml", std::ofstream::binary);
  _f << f_temp_content.rdbuf();
  f_temp_content.close();
  std::remove((_fig.name() + "_tmp.xml").c_str());
  _f.close();

  _f = std::ofstream(_fig.name() + ".xml", std::ofstream::app);
  _f << "\n</page>\n</ipe>";
  _f.close();
}

const std::vector<double> generate_axis_ticks(double min_val, double max_val) {
    std::vector<double> ticks;

    double range = max_val - min_val;
    double raw_step = range / 5;

    double magnitude = std::pow(10, std::floor(std::log10(raw_step)));
    double normalized_step = raw_step / magnitude;

    double step;
    if (normalized_step < 1.5) 
      step = 1.0;
    else if (normalized_step < 3) 
      step = 2.0;
    else if (normalized_step < 7) 
      step = 5.0;
    else 
      step = 10.0;

    step *= magnitude;

    double first_tick = std::ceil(min_val / step) * step;

    for (double tick = first_tick; tick <= max_val +step/10.; tick += step)
        ticks.push_back(tick);

    return ticks;
}

const std::string format_number(double num, double step) 
{
    string result;
    string sign = "";
    if (num < 0)
    {
      sign = "-";
      num = -num;
    }
    
    int precision = std::floor(std::log10(step)); // precision required for the axis label

    int int_part = num / 1; // integer part of the number

    result = sign;  // sign of the number
    result += to_string(int_part);
    
    if (precision >= 0) // the number is an integer
    {
      return result;
    }

    else
    {
      result += ".";
      double remainder = num - ((double) int_part);  // remainder to add
      int remainder_to_int = std::round(remainder * std::pow(10, -precision));
      int length_of_remainder =  std::floor(std::log10(remainder_to_int)) + 1; // for example 12 has a length of 2

      // this part is need for the specific case where a number like 1. is represented as 0.999... (int part gives 0 instead of 1)
      if (length_of_remainder > -precision)
        {
          if (to_string(remainder_to_int) == "10")
          {
            int_part++;
            result = sign;
            result += to_string(int_part);
          }
        }

      else{
        if (length_of_remainder>0) // this assertion is useful to avoid issues with the approximation of 0
        {
          // we add the necessary zeros after the comma
          for (int i =0; i < (-precision-length_of_remainder); i++)
            result += "0";
          // and we add the remainder
          result += to_string(remainder_to_int);
        }
      }
    }
    return result;
}

void Figure2D_IPE::update_axes()
{
  // The offsets are set to 3% of the axis diameters minimum
  _x_offset = 0.03*_fig.axes()[0].limits.diam();
  _y_offset = 0.03*_fig.axes()[1].limits.diam();

  // Generate ticks for the axes
  _x_ticks = generate_axis_ticks(_fig.axes()[0].limits.lb(), _fig.axes()[0].limits.ub());
  _y_ticks = generate_axis_ticks(_fig.axes()[1].limits.lb(), _fig.axes()[1].limits.ub());

  // Adjust the x_offset to get all the vertical labels to fit the page
  for (const auto& y_tick : _y_ticks) 
  {
    auto formatted_y_tick = format_number(y_tick,(_y_ticks[1] - _y_ticks[0]));
    _x_offset = std::max(_x_offset, (0.02+0.0095*(formatted_y_tick.size()-1))*_fig.axes()[0].limits.diam()); // the 0.0095 is empirical, it is used to displace the vertical label according to its length
  }

  _ratio = {
    _ipe_grid_size/(_fig.axes()[0].limits.diam()+_x_offset),
    _ipe_grid_size/(_fig.axes()[1].limits.diam()+_y_offset)
  };
}

void Figure2D_IPE::update_window_properties()
{

}

void Figure2D_IPE::center_viewbox([[maybe_unused]] const Vector& c, [[maybe_unused]] const Vector& r)
{
  assert(_fig.size() <= c.size() && _fig.size() <= r.size());
  assert(r.min_coeff() > 0.);
}

std::string ipe_str(const Color& c)
{
  return c.hex_str().substr(1);
}

int ipe_opacity(const Color& c)
{
  return (int)(10.*round(10.*(c.model()==Model::RGB ? (c[3]/255.):(c[3]/100.))));
}

std::string to_ipe_linestyle(const std::string& ls)
{
  if (ls == "-")
    return "normal";
  else if (ls == "--")
    return "dashed";
  else if (ls == "..")
    return "dotted";
  else if (ls == "-.")
    return "dash dotted";
  else if (ls == "-..")
    return "dash dot dotted";
  else
    return "solid";
}

void Figure2D_IPE::begin_path(const StyleProperties& s, bool tip)
{
  // substr is needed to remove the "#" at the beginning of hex_str (deprecated by IPE)
  _colors.emplace(ipe_str(s.stroke_color), s.stroke_color);
  _colors.emplace(ipe_str(s.fill_color), s.fill_color);

  if (std::find(_layers.begin(), _layers.end(), s.layer) == _layers.end() && s.layer != "")
      _layers.push_back(s.layer); 

  _f_temp_content << "\n \
    <path layer=\"" << s.layer << "\" \n \
    stroke=\"codac_color_" << ipe_str(s.stroke_color) << "\" \n \
    fill=\"codac_color_" << ipe_str(s.fill_color) << "\" \n \
    opacity=\"" << ipe_opacity(s.fill_color) << "%\" \n \
    stroke-opacity=\"" << ipe_opacity(s.stroke_color) << "%\" \n \
    dash=\"" << to_ipe_linestyle(s.line_style) << "\" \n \
    pen=\"heavier\"";
  if (tip)
    _f_temp_content << "\n \
    arrow=\"normal/normal\"";
  _f_temp_content << "> \n";
}

void Figure2D_IPE::begin_path_with_matrix(const Vector& x, float length, const StyleProperties& s)
{
  // substr is needed to remove the "#" at the beginning of hex_str (deprecated by IPE)
  _colors.emplace(ipe_str(s.stroke_color), s.stroke_color);
  _colors.emplace(ipe_str(s.fill_color), s.fill_color);

  if ((std::find(_layers.begin(), _layers.end(), s.layer) == _layers.end()) && s.layer != "")
      _layers.push_back(s.layer); 

  _f_temp_content << "\n \
    <path layer=\"" << s.layer << "\" \n \
    stroke=\"codac_color_" << ipe_str(s.stroke_color) << "\" \n \
    fill=\"codac_color_" << ipe_str(s.fill_color) << "\" \n \
    opacity=\"" << ipe_opacity(s.fill_color) << "%\" \n \
    stroke-opacity=\"" << ipe_opacity(s.stroke_color) << "%\" \n \
    dash=\"" << to_ipe_linestyle(s.line_style) << "\" \n \
    pen=\"heavier\" \n \
    matrix=";

  // Matrix is composed of the 4 components of the 2D transformation matrix and the translation vector
  _f_temp_content << "\"" << scale_length(length) * std::cos(x[j()+1]) << " " << scale_length(length) * std::sin(x[j()+1]) << " "
                  << - scale_length(length) * std::sin(x[j()+1]) << " " << scale_length(length) * std::cos(x[j()+1]) << " " 
                  << scale_x(x[i()]) << " " << scale_y(x[j()]) << "\">\n";
}


void Figure2D_IPE::draw_text(const Vector& c, const Vector& r, const std::string& text, const StyleProperties& s)
{
  assert(_fig.size() <= c.size());
  _colors.emplace(ipe_str(s.stroke_color), s.stroke_color);
  _colors.emplace(ipe_str(s.fill_color), s.fill_color);

  _f_temp_content << "\n \
    <text layer=\"" << s.layer << "\" \n \
    transformations=\"translations\" \n \
    pos=\"" << scale_x(c[i()]) << " " << scale_y(c[j()]) << "\" \n \
    stroke=\"codac_color_" << ipe_str(s.stroke_color) << "\" \n \
    fill=\"codac_color_" << ipe_str(s.fill_color) << "\" \n \
    opacity=\"" << ipe_opacity(s.fill_color) << "%\" \n \
    stroke-opacity=\"" << ipe_opacity(s.stroke_color) << "%\" \n \
    type=\"label\" \n \
    width=\"" << scale_length(r[i()]) << "\" \n \
    height=\"" << scale_length(r[j()]) << "\" \n \
    depth=\"0\" \n \
    valign=\"baseline\">" << text << "</text>";
}

void Figure2D_IPE::draw_axes()
{
  draw_polyline({{_fig.axes()[0].limits.lb(),_fig.axes()[1].limits.lb()},
                 {_fig.axes()[0].limits.ub(),_fig.axes()[1].limits.lb()}}, 0., StyleProperties({Color::black(),Color::black()}, "axes"));

  draw_polyline({{_fig.axes()[0].limits.lb(),_fig.axes()[1].limits.lb()},
                 {_fig.axes()[0].limits.lb(),_fig.axes()[1].limits.ub()}}, 0., StyleProperties({Color::black(),Color::black()}, "axes"));

  for (const auto& x_tick : _x_ticks) 
  {
    auto formatted_x_tick = format_number(x_tick,(_x_ticks[1] - _x_ticks[0]));
    draw_polyline({{x_tick,_fig.axes()[1].limits.lb()-0.02*_fig.axes()[1].limits.diam()},{x_tick,_fig.axes()[1].limits.lb()}}, 0., StyleProperties({Color::black(),Color::black()}, "axes"));
    draw_text({x_tick+0.005*_fig.axes()[0].limits.diam(),_fig.axes()[1].limits.lb()-0.02*_fig.axes()[1].limits.diam()}, {_fig.axes()[0].limits.diam(),_fig.axes()[1].limits.diam()}, formatted_x_tick, StyleProperties({Color::black(),Color::black()}, "axes"));
  }

  for (const auto& y_tick : _y_ticks) 
  {
    auto formatted_y_tick = format_number(y_tick,(_y_ticks[1] - _y_ticks[0]));
    draw_polyline({{_fig.axes()[0].limits.lb()-0.02*_fig.axes()[0].limits.diam(),y_tick},{_fig.axes()[0].limits.lb(),y_tick}}, 0., StyleProperties({Color::black(),Color::black()}, "axes"));
    draw_text({_fig.axes()[0].limits.lb()-(0.02+0.0095*(formatted_y_tick.size()-1))*_fig.axes()[0].limits.diam(),y_tick+0.005*_fig.axes()[1].limits.diam()}, {_fig.axes()[0].limits.diam(),_fig.axes()[1].limits.diam()}, formatted_y_tick, StyleProperties({Color::black(),Color::black()}, "axes"));
  }
}

void Figure2D_IPE::draw_point(const Vector& c, const StyleProperties& s)
{
  assert(_fig.size() <= c.size());
  _colors.emplace(ipe_str(s.stroke_color), s.stroke_color);
  _colors.emplace(ipe_str(s.fill_color), s.fill_color);

  _f_temp_content << "\n \
    <use layer=\"" << s.layer << "\" \n \
    name=\"mark/fdisk(sfx)\"  \n \
    pos=\"" << scale_x(c[i()]) << " " << scale_y(c[j()]) << "\" \n \
    stroke=\"codac_color_" << ipe_str(s.stroke_color) << "\" \n \
    fill=\"codac_color_" << ipe_str(s.fill_color) << "\" \n \
    opacity=\"" << ipe_opacity(s.fill_color) << "%\" \n \
    stroke-opacity=\"" << ipe_opacity(s.stroke_color) << "%\" \n \
    size=\"normal\"\n/>";
}

void Figure2D_IPE::draw_box(const IntervalVector& x, const StyleProperties& s)
{
  assert(_fig.size() <= x.size());
  draw_polyline({
    {x[i()].lb(),x[j()].lb()}, {x[i()].ub(),x[j()].lb()},
    {x[i()].ub(),x[j()].ub()}, {x[i()].lb(),x[j()].ub()},
    {x[i()].lb(),x[j()].lb()}},
    0., s);
}

void Figure2D_IPE::draw_circle(const Vector& c, double r, const StyleProperties& s)
{
  assert(_fig.size() <= c.size());
  assert(r > 0.);

  begin_path(s);
  _f_temp_content << scale_length(r) << " 0 0 " << scale_length(r) << " "
                  << scale_x(c[i()]) << " " << scale_y(c[j()]) << " e \n";
  _f_temp_content << "</path>";
}

void Figure2D_IPE::draw_ring(const Vector& c, const Interval& r, const StyleProperties& s)
{
  assert(_fig.size() <= c.size());
  assert(!r.is_empty() && r.lb() >= 0.);

  begin_path(s);
  _f_temp_content << scale_length(r.lb()) << " 0 0 " << scale_length(r.lb()) << " "
                  << scale_x(c[i()]) << " " << scale_y(c[j()]) << " e \n";
  _f_temp_content << scale_length(r.ub()) << " 0 0 " << scale_length(r.ub()) << " "
                  << scale_x(c[i()]) << " " << scale_y(c[j()]) << " e \n";
  _f_temp_content << "</path>";
}

void Figure2D_IPE::draw_polyline(const std::vector<Vector>& x, float tip_length, const StyleProperties& s)
{
  assert(x.size() > 1);
  assert(tip_length >= 0.);

  begin_path(s, tip_length>2e-3*_fig.scaled_unit());
  for(size_t k = 0 ; k < x.size() ; k++)
  {
    assert(_fig.size() <= x[k].size());
    _f_temp_content << scale_x(x[k][i()]) << " " << scale_y(x[k][j()]) << ((k == 0 ? " m \n" : " l \n"));
  }

  _f_temp_content << "</path>";
}

void Figure2D_IPE::draw_polygon(const std::vector<Vector>& x, const StyleProperties& s)
{
  assert(x.size() > 1);

  std::vector<Vector> x_temp = x;
  x_temp.push_back(x[0]);
  draw_polyline(x_temp, 0., s);
}

void Figure2D_IPE::draw_pie(const Vector& c, const Interval& r, const Interval& theta, const StyleProperties& s)
{
  assert(_fig.size() <= c.size());
  assert(r.lb() >= 0.);
  
  begin_path(s);

  Vector point1 ({r.lb() * std::cos(theta.lb()), r.lb() * std::sin(theta.lb())});
  Vector point2 ({r.ub() * std::cos(theta.lb()), r.ub() * std::sin(theta.lb())});
  Vector point3 ({r.ub() * std::cos(theta.ub()), r.ub() * std::sin(theta.ub())});
  Vector point4 ({r.lb() * std::cos(theta.ub()), r.lb() * std::sin(theta.ub())});

  _f_temp_content << scale_x(c[0] + point1[0]) << " " << scale_y(c[1] + point1[1]) << " m \n";
  _f_temp_content << scale_x(c[0] + point2[0]) << " " << scale_y(c[1] + point2[1]) << " l \n";
  _f_temp_content << scale_length(r.ub()) << " 0 0 " << scale_length(r.ub()) << " "
                  << scale_x(c[i()]) << " " << scale_y(c[j()]) << " "
                  << scale_x(c[0] + point3[0]) << " " << scale_y(c[1] + point3[1]) << " a \n";
  _f_temp_content << scale_x(c[0] + point4[0]) << " " << scale_y(c[1] + point4[1]) << " l \n";
  _f_temp_content << scale_length(r.lb()) << " 0 0 " << - scale_length(r.lb()) << " "
                  << scale_x(c[i()]) << " " << scale_y(c[j()]) << " "
                  << scale_x(c[0] + point1[0]) << " " << scale_y(c[1] + point1[1]) << " a \n";

  _f_temp_content << "</path>";
}

void Figure2D_IPE::draw_ellipse(const Vector& c, const Vector& ab, double theta, const StyleProperties& s)
{
  assert(c.size() == 2);
  assert(ab.size() == 2);

  begin_path(s);
  _f_temp_content << scale_length(ab[0]) * std::cos(theta) << " " << scale_length(ab[0]) * std::sin(theta) << " " 
                  << - scale_length(ab[1]) * std::sin(theta) << " " << scale_length(ab[1]) * std::cos(theta) << " " 
                  << scale_x(c[i()]) << " " << scale_y(c[j()]) << " e \n";
  _f_temp_content << "</path>";
}

void Figure2D_IPE::draw_tank(const Vector& x, float size, const StyleProperties& s)
{
  assert(_fig.size() <= x.size()+1);
  assert(j()+1 < x.size());
  assert(size >= 0.);
  
  float length = size/4.0; // from VIBes : initial vehicle's length is 4

  begin_path_with_matrix(x,length,s);
  constexpr char tank_shape[] = " 1 -1.5 m \n -1 -1.5 l \n 0 -1.5 l \n 0 -1 l \n -1 -1 l \n -1 1 l \n 0 1 l \n 0 1.5 l \n -1 1.5 l \n 1 1.5 l \n 0 1.5 l \n 0 1 l \n 3 0.5 l \n 3 -0.5 l \n 0 -1 l \n 0 -1.5 l \n";
  _f_temp_content << tank_shape << "</path>";
}

void Figure2D_IPE::draw_AUV(const Vector& x, float size, const StyleProperties& s)
{
  assert(_fig.size() <= x.size()+1);
  assert(j()+1 < x.size());
  assert(size >= 0.);

  float length = size/7.0; // from VIBes : initial vehicle's length is 7

  _f_temp_content << "\n<group layer=\"" << s.layer<< "\">\n";

  // Body
  begin_path_with_matrix(x,length,s);
  constexpr char body_shape[] = " -4 0 m \n -2 1 l \n 2 1 l \n 2.17365 0.984808 l \n 2.34202 0.939693 l \n 2.5 0.866025 l \n 2.64279 0.766044 l \n 2.76604 0.642788 l \n 2.86603 0.5 l \n 2.93969 0.34202 l \n 2.98481 0.173648 l \n 3 0 l \n 2.98481 -0.173648 l \n 2.93969 -0.34202 l \n 2.86603 -0.5 l \n 2.76604 -0.642788 l \n 2.64279 -0.766044 l \n 2.5 -0.866025 l \n 2.34202 -0.939693 l \n 2.17365 -0.984808 l \n 2 -1 l \n -2 -1 l \n -4 0 l \n";
  _f_temp_content << body_shape;
  _f_temp_content << "</path>\n";

  // Propulsion unit
  constexpr char propeller_shape[] = " -4 1 m \n -3.25 1 l \n -3.25 -1 l \n -4 -1 l \n -4 1 l \n";
  begin_path_with_matrix(x,length,s);
  _f_temp_content << propeller_shape << "</path>\n";

  _f_temp_content << "</group>";
}

void Figure2D_IPE::draw_motor_boat(const Vector& x, float size, const StyleProperties& s)
{
  assert(_fig.size() <= x.size()+1);
  assert(j()+1 < x.size());
  assert(size >= 0.);
  
  float length = size/408.0; // from VIBes : initial vehicle's length is 408

  StyleProperties s_edge = s; s_edge.fill_color = Color::none();
  StyleProperties s_fill = s; s_fill.fill_color = s.stroke_color;

  _f_temp_content << "\n<group layer=\"" << s.layer<< "\">\n";

  // Body shape
  begin_path_with_matrix(x,length,s);
  constexpr char body_shape[] = "-72 -80 m \n -72 80 l \n 120 80 l \n 184 80 \n 264 64 \n 312 32 \n 328 0 c \n 312 -32 \n 264 -64 \n 184 -80 \n 120 -80 \n -72 -80 c \n";
  _f_temp_content << body_shape << "</path>";

  // Left prop
  begin_path_with_matrix(x,length,s_fill);
  constexpr char left_prop[] = "-72 48 m \n -72 16 l \n -80 16 l \n -80 48 l \n h \n";
  _f_temp_content << left_prop << "</path>";

  // Right prop
  begin_path_with_matrix(x,length,s_fill);
  constexpr char right_prop[] = "-72 -16 m \n -72 -48 l \n -80 -48 l \n -80 -16 l \n h \n";
  _f_temp_content << right_prop << "</path>";

  // Hull details
  begin_path_with_matrix(x,length,s_edge);
  constexpr char hull_details[] = "120 80 m \n 104 64 l \n -56 64 l \n -56 -64 l \n 104 -64 l \n 120 -80 l \n";
  _f_temp_content << hull_details << "</path>";

  // Engine
  begin_path_with_matrix(x,length,s_fill);
  constexpr char engine[] = "-24 32 m \n -24 -32 l \n 40 -32 l \n 40 32 l \n h \n";
  _f_temp_content << engine << "</path>";

  // Circle
  begin_path_with_matrix(x,length,s_edge);
  constexpr char circle[] = "22.6274 \n 0 \n 0 \n 22.6274 \n 200 \n 0 \n e \n";
  _f_temp_content << circle << "</path>";

  _f_temp_content << "</group>";
}

double Figure2D_IPE::scale_x(double x) const
{
  return (x-_fig.axes()[0].limits.lb()+_x_offset)*_ratio[0];
}

double Figure2D_IPE::scale_y(double y) const
{
  return (y-_fig.axes()[1].limits.lb()+_y_offset)*_ratio[1];
}

double Figure2D_IPE::scale_length(double x) const
{
  // Assuming _ratio[0] == _ratio[1]
  return x*_ratio[0];
}

void Figure2D_IPE::print_header_page()
{
  _f << "<?xml version=\"1.0\"?> \n \
    <!DOCTYPE ipe SYSTEM \"ipe.dtd\"> \n \
    <ipe version=\"70218\" creator=\"Ipe 7.2.24\"> \n \
    <info created=\"D:20240517161412\" modified=\"D:20240517162731\"/> \n \
    <ipestyle name=\"codac\"> \n \
    <symbol name=\"arrow/arc(spx)\"> \n \
    <path stroke=\"sym-stroke\" fill=\"sym-stroke\" pen=\"sym-pen\"> \n \
    0 0 m \n \
    -1 0.333 l \n \
    -1 -0.333 l \n \
    h \n \
    </path> \n \
    </symbol> \n \
    <symbol name=\"arrow/farc(spx)\"> \n \
    <path stroke=\"sym-stroke\" fill=\"white\" pen=\"sym-pen\"> \n \
    0 0 m \n \
    -1 0.333 l \n \
    -1 -0.333 l \n \
    h \n \
    </path> \n \
    </symbol> \n \
    <symbol name=\"arrow/ptarc(spx)\"> \n \
    <path stroke=\"sym-stroke\" fill=\"sym-stroke\" pen=\"sym-pen\"> \n \
    0 0 m \n \
    -1 0.333 l \n \
    -0.8 0 l \n \
    -1 -0.333 l \n \
    h \n \
    </path> \n \
    </symbol> \n \
    <symbol name=\"arrow/fptarc(spx)\"> \n \
    <path stroke=\"sym-stroke\" fill=\"white\" pen=\"sym-pen\"> \n \
    0 0 m \n \
    -1 0.333 l \n \
    -0.8 0 l \n \
    -1 -0.333 l \n \
    h \n \
    </path> \n \
    </symbol> \n \
    <symbol name=\"mark/circle(sx)\" transformations=\"translations\"> \n \
    <path fill=\"sym-stroke\"> \n \
    0.6 0 0 0.6 0 0 e \n \
    0.4 0 0 0.4 0 0 e \n \
    </path> \n \
    </symbol> \n \
    <symbol name=\"mark/disk(sx)\" transformations=\"translations\"> \n \
    <path fill=\"sym-stroke\"> \n \
    0.6 0 0 0.6 0 0 e \n \
    </path> \n \
    </symbol> \n \
    <symbol name=\"mark/fdisk(sfx)\" transformations=\"translations\"> \n \
    <group> \n \
    <path fill=\"sym-fill\"> \n \
    0.5 0 0 0.5 0 0 e \n \
    </path> \n \
    <path fill=\"sym-stroke\" fillrule=\"eofill\"> \n \
    0.6 0 0 0.6 0 0 e \n \
    0.4 0 0 0.4 0 0 e \n \
    </path> \n \
    </group> \n \
    </symbol> \n \
    <symbol name=\"mark/box(sx)\" transformations=\"translations\"> \n \
    <path fill=\"sym-stroke\" fillrule=\"eofill\"> \n \
    -0.6 -0.6 m \n \
    0.6 -0.6 l \n \
    0.6 0.6 l \n \
    -0.6 0.6 l \n \
    h \n \
    -0.4 -0.4 m \n \
    0.4 -0.4 l \n \
    0.4 0.4 l \n \
    -0.4 0.4 l \n \
    h \n \
    </path> \n \
    </symbol> \n \
    <symbol name=\"mark/square(sx)\" transformations=\"translations\"> \n \
    <path fill=\"sym-stroke\"> \n \
    -0.6 -0.6 m \n \
    0.6 -0.6 l \n \
    0.6 0.6 l \n \
    -0.6 0.6 l \n \
    h \n \
    </path> \n \
    </symbol> \n \
    <symbol name=\"mark/fsquare(sfx)\" transformations=\"translations\"> \n \
    <group> \n \
    <path fill=\"sym-fill\"> \n \
    -0.5 -0.5 m \n \
    0.5 -0.5 l \n \
    0.5 0.5 l \n \
    -0.5 0.5 l \n \
    h \n \
    </path> \n \
    <path fill=\"sym-stroke\" fillrule=\"eofill\"> \n \
    -0.6 -0.6 m \n \
    0.6 -0.6 l \n \
    0.6 0.6 l \n \
    -0.6 0.6 l \n \
    h \n \
    -0.4 -0.4 m \n \
    0.4 -0.4 l \n \
    0.4 0.4 l \n \
    -0.4 0.4 l \n \
    h \n \
    </path> \n \
    </group> \n \
    </symbol> \n \
    <symbol name=\"mark/cross(sx)\" transformations=\"translations\"> \n \
    <group> \n \
    <path fill=\"sym-stroke\"> \n \
    -0.43 -0.57 m \n \
    0.57 0.43 l \n \
    0.43 0.57 l \n \
    -0.57 -0.43 l \n \
    h \n \
    </path> \n \
    <path fill=\"sym-stroke\"> \n \
    -0.43 0.57 m \n \
    0.57 -0.43 l \n \
    0.43 -0.57 l \n \
    -0.57 0.43 l \n \
    h \n \
    </path> \n \
    </group> \n \
    </symbol> \n \
    <symbol name=\"arrow/fnormal(spx)\"> \n \
    <path stroke=\"sym-stroke\" fill=\"white\" pen=\"sym-pen\"> \n \
    0 0 m \n \
    -1 0.333 l \n \
    -1 -0.333 l \n \
    h \n \
    </path> \n \
    </symbol> \n \
    <symbol name=\"arrow/pointed(spx)\"> \n \
    <path stroke=\"sym-stroke\" fill=\"sym-stroke\" pen=\"sym-pen\"> \n \
    0 0 m \n \
    -1 0.333 l \n \
    -0.8 0 l \n \
    -1 -0.333 l \n \
    h \n \
    </path> \n \
    </symbol> \n \
    <symbol name=\"arrow/fpointed(spx)\"> \n \
    <path stroke=\"sym-stroke\" fill=\"white\" pen=\"sym-pen\"> \n \
    0 0 m \n \
    -1 0.333 l \n \
    -0.8 0 l \n \
    -1 -0.333 l \n \
    h \n \
    </path> \n \
    </symbol> \n \
    <symbol name=\"arrow/linear(spx)\"> \n \
    <path stroke=\"sym-stroke\" pen=\"sym-pen\"> \n \
    -1 0.333 m \n \
    0 0 l \n \
    -1 -0.333 l \n \
    </path> \n \
    </symbol> \n \
    <symbol name=\"arrow/fdouble(spx)\"> \n \
    <path stroke=\"sym-stroke\" fill=\"white\" pen=\"sym-pen\"> \n \
    0 0 m \n \
    -1 0.333 l \n \
    -1 -0.333 l \n \
    h \n \
    -1 0 m \n \
    -2 0.333 l \n \
    -2 -0.333 l \n \
    h \n \
    </path> \n \
    </symbol> \n \
    <symbol name=\"arrow/double(spx)\"> \n \
    <path stroke=\"sym-stroke\" fill=\"sym-stroke\" pen=\"sym-pen\"> \n \
    0 0 m \n \
    -1 0.333 l \n \
    -1 -0.333 l \n \
    h \n \
    -1 0 m \n \
    -2 0.333 l \n \
    -2 -0.333 l \n \
    h \n \
    </path> \n \
    </symbol> \n \
    <symbol name=\"arrow/mid-normal(spx)\"> \n \
    <path stroke=\"sym-stroke\" fill=\"sym-stroke\" pen=\"sym-pen\"> \n \
    0.5 0 m \n \
    -0.5 0.333 l \n \
    -0.5 -0.333 l \n \
    h \n \
    </path> \n \
    </symbol> \n \
    <symbol name=\"arrow/mid-fnormal(spx)\"> \n \
    <path stroke=\"sym-stroke\" fill=\"white\" pen=\"sym-pen\"> \n \
    0.5 0 m \n \
    -0.5 0.333 l \n \
    -0.5 -0.333 l \n \
    h \n \
    </path> \n \
    </symbol> \n \
    <symbol name=\"arrow/mid-pointed(spx)\"> \n \
    <path stroke=\"sym-stroke\" fill=\"sym-stroke\" pen=\"sym-pen\"> \n \
    0.5 0 m \n \
    -0.5 0.333 l \n \
    -0.3 0 l \n \
    -0.5 -0.333 l \n \
    h \n \
    </path> \n \
    </symbol> \n \
    <symbol name=\"arrow/mid-fpointed(spx)\"> \n \
    <path stroke=\"sym-stroke\" fill=\"white\" pen=\"sym-pen\"> \n \
    0.5 0 m \n \
    -0.5 0.333 l \n \
    -0.3 0 l \n \
    -0.5 -0.333 l \n \
    h \n \
    </path> \n \
    </symbol> \n \
    <symbol name=\"arrow/mid-double(spx)\"> \n \
    <path stroke=\"sym-stroke\" fill=\"sym-stroke\" pen=\"sym-pen\"> \n \
    1 0 m \n \
    0 0.333 l \n \
    0 -0.333 l \n \
    h \n \
    0 0 m \n \
    -1 0.333 l \n \
    -1 -0.333 l \n \
    h \n \
    </path> \n \
    </symbol> \n \
    <symbol name=\"arrow/mid-fdouble(spx)\"> \n \
    <path stroke=\"sym-stroke\" fill=\"white\" pen=\"sym-pen\"> \n \
    1 0 m \n \
    0 0.333 l \n \
    0 -0.333 l \n \
    h \n \
    0 0 m \n \
    -1 0.333 l \n \
    -1 -0.333 l \n \
    h \n \
    </path> \n \
    </symbol> \n \
    <anglesize name=\"22.5 deg\" value=\"22.5\"/> \n \
    <anglesize name=\"30 deg\" value=\"30\"/> \n \
    <anglesize name=\"45 deg\" value=\"45\"/> \n \
    <anglesize name=\"60 deg\" value=\"60\"/> \n \
    <anglesize name=\"90 deg\" value=\"90\"/> \n \
    <arrowsize name=\"large\" value=\"10\"/> \n \
    <arrowsize name=\"small\" value=\"5\"/> \n \
    <arrowsize name=\"tiny\" value=\"3\"/> \n";

  for(auto& [k,c] : _colors)
    {
      Color c_rgb = c.rgb();
      _f << "<color name=\"codac_color_" << k << "\" "
        << "value=\"" << (float) (c_rgb[0]/255.) << " " <<(float) (c_rgb[1]/255.) << " " <<(float) (c_rgb[2]/255.) << "\" /> \n";
    }

  _f << "<dashstyle name=\"dash dot dotted\" value=\"[4 2 1 2 1 2] 0\"/> \n \
    <dashstyle name=\"dash dotted\" value=\"[4 2 1 2] 0\"/> \n \
    <dashstyle name=\"dashed\" value=\"[4] 0\"/> \n \
    <dashstyle name=\"dotted\" value=\"[1 3] 0\"/> \n \
    <gridsize name=\"10 pts (~3.5 mm)\" value=\"10\"/> \n \
    <gridsize name=\"14 pts (~5 mm)\" value=\"14\"/> \n \
    <gridsize name=\"16 pts (~6 mm)\" value=\"16\"/> \n \
    <gridsize name=\"20 pts (~7 mm)\" value=\"20\"/> \n \
    <gridsize name=\"28 pts (~10 mm)\" value=\"28\"/> \n \
    <gridsize name=\"32 pts (~12 mm)\" value=\"32\"/> \n \
    <gridsize name=\"4 pts\" value=\"4\"/> \n \
    <gridsize name=\"56 pts (~20 mm)\" value=\"56\"/> \n \
    <gridsize name=\"8 pts (~3 mm)\" value=\"8\"/> \n";

  for(float i = 0. ; i < 1. ; i+=0.1)
    _f << "<opacity name=\"" << (int)(i*100.) << "%\" value=\"" << i << "\"/> \n";

  _f << "<opacity name=\"100%\" value=\"1.0\"/> \n \
    <pen name=\"fat\" value=\"1.2\"/> \n \
    <pen name=\"heavier\" value=\"0.8\"/> \n \
    <pen name=\"ultrafat\" value=\"1.5\"/> \n \
    <symbolsize name=\"large\" value=\"5\"/> \n \
    <symbolsize name=\"small\" value=\"2\"/> \n \
    <symbolsize name=\"tiny\" value=\"1.1\"/> \n \
    <textsize name=\"Huge\" value=\"\\Huge\"/> \n \
    <textsize name=\"LARGE\" value=\"\\LARGE\"/> \n \
    <textsize name=\"Large\" value=\"\\Large\"/> \n \
    <textsize name=\"footnote\" value=\"\\footnotesize\"/> \n \
    <textsize name=\"huge\" value=\"\\huge\"/> \n \
    <textsize name=\"large\" value=\"\\large\"/> \n \
    <textsize name=\"small\" value=\"\\small\"/> \n \
    <textsize name=\"tiny\" value=\"\\tiny\"/> \n \
    <textstyle name=\"center\" begin=\"\\begin{center}\" end=\"\\end{center}\"/> \n \
    <textstyle name=\"item\" begin=\"\\begin{itemize}\\item{}\" end=\"\\end{itemize}\"/> \n \
    <textstyle name=\"itemize\" begin=\"\\begin{itemize}\" end=\"\\end{itemize}\"/> \n \
    <tiling name=\"falling\" angle=\"-60\" step=\"4\" width=\"1\"/> \n \
    <tiling name=\"rising\" angle=\"30\" step=\"4\" width=\"1\"/> \n \
    </ipestyle> \n \
    <page> \n ";
  for (const auto& layer : _layers)
    _f << "<layer name=\"" << layer << "\"/> \n";
  _f << "<view layers=\" ";
  for (const auto& layer : _layers)
    _f  << layer <<  " ";
  _f << "\" active=\"alpha\"/> \n ";
}