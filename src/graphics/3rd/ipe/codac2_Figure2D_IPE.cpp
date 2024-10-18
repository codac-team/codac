/** 
 *  codac2_Figure2D_IPE.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <cstdio>
#include "codac2_Figure2D_IPE.h"

using namespace std;
using namespace codac2;


Figure2D_IPE::Figure2D_IPE(const Figure2D& fig)
  : OutputFigure2D(fig), _f(fig.name() + ".xml"),
    _f_temp_content(fig.name() + "_tmp.xml")
{
  list<Color> codac_colors {
    Color::none(),
    Color::black(),
    Color::white(),
    Color::green(),
    Color::blue(),
    Color::cyan(),
    Color::yellow(),
    Color::red(),
    Color::dark_gray(),
    Color::purple(),
    Color::dark_green()
  };

  for(const auto& ci : codac_colors)
    // substr is needed to remove the "#" at the beginning of hex_str (deprecated by IPE)
    _colors.emplace(ci.hex_str.substr(1), ci);
}

Figure2D_IPE::~Figure2D_IPE()
{
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

void Figure2D_IPE::update_axes()
{
  _ratio = {
    _ipe_grid_size/_fig.axes()[0].limits.diam(),
    _ipe_grid_size/_fig.axes()[1].limits.diam()
  };
}

void Figure2D_IPE::update_window_properties()
{

}

void Figure2D_IPE::center_viewbox(const Vector& c, const Vector& r)
{
  assert(_fig.size() <= c.size() && _fig.size() <= r.size());
  assert(r.min_coeff() > 0.);
}

void Figure2D_IPE::begin_path(const StyleProperties& s)
{
  // substr is needed to remove the "#" at the beginning of hex_str (deprecated by IPE)
  _colors.emplace(s.stroke_color.hex_str.substr(1), s.stroke_color);
  _colors.emplace(s.fill_color.hex_str.substr(1), s.fill_color);

  _f_temp_content << "\n \
    <path layer=\"alpha\" \n \
    stroke=\"codac_color_" << s.stroke_color.hex_str.substr(1) << "\" \n \
    fill=\"codac_color_" << s.fill_color.hex_str.substr(1) << "\" \n \
    opacity=\"" << (int)(10*round(10.*s.fill_color.alpha)) << "%\" \n \
    stroke-opacity=\"" << (int)(10*round(10.*s.stroke_color.alpha)) << "%\" \n \
    pen=\"normal\"> \n ";
}

void Figure2D_IPE::draw_point(const Vector& c, const StyleProperties& s)
{
  assert(_fig.size() <= c.size());
  _colors.emplace(s.stroke_color.hex_str.substr(1), s.stroke_color);
  _colors.emplace(s.fill_color.hex_str.substr(1), s.fill_color);

  _f_temp_content << "\n \
    <use layer=\"alpha\" \n \
    name=\"mark/fdisk(sfx)\"  \n \
    pos=\"" << scale_x(c[i()]) << " " << scale_y(c[j()]) << "\" \n \
    stroke=\"codac_color_" << s.stroke_color.hex_str.substr(1) << "\" \n \
    fill=\"codac_color_" << s.fill_color.hex_str.substr(1) << "\" \n \
    opacity=\"" << (int)(10*round(10.*s.fill_color.alpha)) << "%\" \n \
    stroke-opacity=\"" << (int)(10*round(10.*s.stroke_color.alpha)) << "%\" \n \
    size=\"normal\"/>";
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
  // todo: draw tip

  begin_path(s);

  for(size_t k = 0 ; k < x.size() ; k++)
  {
    assert(_fig.size() <= x[k].size());
    _f_temp_content << scale_x(x[k][i()]) << " " << scale_y(x[k][j()]) << ((k == 0 ? " m \n" : " l \n"));
  }

  _f_temp_content << "</path>";
}

void Figure2D_IPE::draw_polygone(const std::vector<Vector>& x, const StyleProperties& s)
{
  draw_polyline(x,0.,s);
}

void Figure2D_IPE::draw_pie(const Vector& c, const Interval& r, const Interval& theta, const StyleProperties& s)
{
  assert(_fig.size() <= c.size());
  assert(r.lb() >= 0.);
  // Not implemented yet
}

void Figure2D_IPE::draw_ellipse(const Vector& c, const Vector& ab, double theta, const StyleProperties& s)
{
  assert(c.size() == 2);
  assert(ab.size() == 2);
  // Not implemented yet
}

void Figure2D_IPE::draw_tank(const Vector& x, float size, const StyleProperties& s)
{
  assert(_fig.size() <= x.size()+1);
  assert(j()+1 < x.size());
  assert(size >= 0.);
  // Not implemented yet
}

void Figure2D_IPE::draw_AUV(const Vector& x, float size, const StyleProperties& s)
{
  assert(_fig.size() <= x.size()+1);
  assert(j()+1 < x.size());
  assert(size >= 0.);
  // Not implemented yet
}

double Figure2D_IPE::scale_x(double x) const
{
  return (x-_fig.axes()[0].limits.lb())*_ratio[0];
}

double Figure2D_IPE::scale_y(double y) const
{
  return (y-_fig.axes()[1].limits.lb())*_ratio[1];
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

  for(const auto& [k,c] : _colors)
    _f << "<color name=\"codac_color_" << k << "\" "
      << "value=\"" << c.r << " " << c.g << " " << c.b << "\" /> \n";

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
    <textsize name=\"footnote\" value=\"\footnotesize\"/> \n \
    <textsize name=\"huge\" value=\"\\huge\"/> \n \
    <textsize name=\"large\" value=\"\\large\"/> \n \
    <textsize name=\"small\" value=\"\\small\"/> \n \
    <textsize name=\"tiny\" value=\"\tiny\"/> \n \
    <textstyle name=\"center\" begin=\"\begin{center}\" end=\"\end{center}\"/> \n \
    <textstyle name=\"item\" begin=\"\begin{itemize}\\item{}\" end=\"\end{itemize}\"/> \n \
    <textstyle name=\"itemize\" begin=\"\begin{itemize}\" end=\"\end{itemize}\"/> \n \
    <tiling name=\"falling\" angle=\"-60\" step=\"4\" width=\"1\"/> \n \
    <tiling name=\"rising\" angle=\"30\" step=\"4\" width=\"1\"/> \n \
    </ipestyle> \n \
    <page> \n \
    <layer name=\"alpha\"/> \n \
    <view layers=\"alpha\" active=\"alpha\"/>";
}