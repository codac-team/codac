/** 
 *  codac2_FigureIPE.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <cstdio>
#include "codac2_FigureIPE.h"

using namespace std;
using namespace codac2;


FigureIPE::FigureIPE(const Figure& fig)
  : OutputFigure(fig), _f(fig.name() + ".xml"),
    _f_temp_content(fig.name() + "_tmp.xml")
{ }

FigureIPE::~FigureIPE()
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

void FigureIPE::update_axes()
{
  _ratio = {
    _ipe_grid_size/_fig.axes()[0].limits.diam(),
    _ipe_grid_size/_fig.axes()[1].limits.diam()
  };
}

void FigureIPE::update_window_properties()
{

}

void FigureIPE::center_viewbox(const Vector& c, const Vector& r)
{

}

void FigureIPE::draw_box(const IntervalVector& x, const StyleProperties& s)
{
  assert(_fig.size() <= x.size());
  int i = _fig.axes()[0].dim_id; int j = _fig.axes()[1].dim_id;
  draw_polyline({{x[i].lb(),x[j].lb()}, {x[i].ub(),x[j].lb()}, {x[i].ub(),x[j].ub()}, {x[i].lb(),x[j].ub()}, {x[i].lb(),x[j].lb()}}, s);
}

void FigureIPE::draw_polyline(const vector<Vector>& x, const StyleProperties& s)
{
  assert(x.size() > 1);
  int i = _fig.axes()[0].dim_id; int j = _fig.axes()[1].dim_id;

  _colors.emplace(s.stroke_color.to_hex_str(""), s.stroke_color);
  _colors.emplace(s.fill_color.to_hex_str(""), s.fill_color);

  _f_temp_content << "\n \
    <path layer=\"alpha\" \n \
    stroke=\"codac_color_" << s.stroke_color.to_hex_str("") << "\" \n \
    fill=\"codac_color_" << s.fill_color.to_hex_str("") << "\" \n \
    pen=\"ultrafat\"> \n ";

  bool first_vertex = true;
  for(size_t k = 0 ; k < x.size() ; k++)
  {
    assert(_fig.size() <= x[k].size());
    _f_temp_content << scale_x(x[k][0]) << " " << scale_y(x[k][1]) << ((k == 0 ? " m \n" : " l \n"));
  }

  _f_temp_content << "</path>";
}

void FigureIPE::draw_circle(const Vector& c, double r, const StyleProperties& s)
{
  // Not implemented yet
}

double FigureIPE::scale_x(double x) const
{
  return (x-_fig.axes()[0].limits.lb())*_ratio[0];
}

double FigureIPE::scale_y(double y) const
{
  return (y-_fig.axes()[1].limits.lb())*_ratio[1];
}

void FigureIPE::print_header_page()
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
      << "value=\"" << c.r << " " << c.g << " " << c.b << "\"/> \n";

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
    <gridsize name=\"8 pts (~3 mm)\" value=\"8\"/> \n \
    <opacity name=\"10%\" value=\"0.1\"/> \n \
    <opacity name=\"30%\" value=\"0.3\"/> \n \
    <opacity name=\"50%\" value=\"0.5\"/> \n \
    <opacity name=\"75%\" value=\"0.75\"/> \n \
    <pen name=\"fat\" value=\"1.2\"/> \n \
    <pen name=\"heavier\" value=\"0.8\"/> \n \
    <pen name=\"ultrafat\" value=\"0.1\"/> \n \
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