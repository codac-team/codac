/** 
 *  ColorMap class
 * ----------------------------------------------------------------------------
 *  \date       2019
 *  \author     Simon Rohou
 *  \copyright  Copyright 2019 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "tubex_ColorMap.h"
#include "tubex_Exception.h"
#include "tubex_VIBesFig.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  ColorMap::ColorMap(int interpol_mode)
    : m_interpol_mode(interpol_mode)
  {
    assert(interpol_mode == InterpolationMode::RGB || interpol_mode == InterpolationMode::HSV);
  }

  void ColorMap::add_color_point(rgb color, float index)
  {
    m_colormap[index] = color;
  }

  void ColorMap::add_color_point(hsv color, float index)
  {
    m_colormap[index] = hsv2rgb(color);
  }

  rgb ColorMap::color(double ratio) const
  {
    assert(m_colormap.size() >= 2 && "color map defined by at least two colors");
    assert(Interval(0.,1.).contains(ratio) && "ratio between 0 and 1");

    Interval map_domain = Interval(m_colormap.begin()->first,prev(m_colormap.end())->first);
    float real_index = map_domain.lb() + ratio*map_domain.diam();

    if(m_colormap.find(real_index) == m_colormap.end()) // color interpolation
    {
      typename map<float,rgb>::const_iterator it_ub;
      it_ub = m_colormap.lower_bound(real_index);
      rgb rgb_lb = prev(it_ub)->second;
      rgb rgb_ub = it_ub->second;

      float local_ratio = (real_index - prev(it_ub)->first) / (it_ub->first - prev(it_ub)->first);
      
      switch(m_interpol_mode)
      {
        case InterpolationMode::RGB:
          return make_rgb((float)(rgb_lb.r + (rgb_ub.r - rgb_lb.r) * local_ratio),
                          (float)(rgb_lb.g + (rgb_ub.g - rgb_lb.g) * local_ratio),
                          (float)(rgb_lb.b + (rgb_ub.b - rgb_lb.b) * local_ratio),
                          (float)(rgb_lb.alpha + (rgb_ub.alpha - rgb_lb.alpha) * local_ratio));

        case InterpolationMode::HSV:
        {
          hsv hsv_lb = rgb2hsv(rgb_lb);
          hsv hsv_ub = rgb2hsv(rgb_ub);

          if(fabs(hsv_lb.h - hsv_ub.h) > 180.)
            hsv_lb.h += 360.;

          return hsv2rgb(make_hsv((float)fmod(hsv_lb.h + (hsv_ub.h - hsv_lb.h) * local_ratio, 360.),
                                  (float)(hsv_lb.s + (hsv_ub.s - hsv_lb.s) * local_ratio),
                                  (float)(hsv_lb.v + (hsv_ub.v - hsv_lb.v) * local_ratio),
                                  (float)(hsv_lb.alpha + (hsv_ub.alpha - hsv_lb.alpha) * local_ratio)));
        }

        default:
          throw Exception("ColorMap::color",
                          "unable color interpolation mode");
      }
    }

    else // color key
      return m_colormap.at(real_index);
  }

  rgb ColorMap::color(double t, const Trajectory& traj) const
  {
    assert(traj.domain().contains(t));
    assert(!traj.not_defined());

    Interval traj_envelope = traj.codomain();
    return color(traj(t) - traj_envelope.lb() / traj_envelope.diam());
  }

  void ColorMap::displayColorMap() const
  {
    VIBesFig fig_map("Colormap");
    fig_map.set_properties(50, 50, 500, 500);
    fig_map.axis_limits(IntervalVector(2, Interval(0.,1.)));

    for(float a = 0. ; a < 1. ; a+=0.001)
    {
      IntervalVector box(2);
      box[0] = Interval(a,a+0.01);
      box[1] = Interval(0.,1.);
      fig_map.draw_box(box, rgb2hex(color(a)) + "[" + rgb2hex(color(a)) + "]");
    }

    fig_map.show();
  }
}