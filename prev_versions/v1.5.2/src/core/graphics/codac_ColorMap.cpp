/** 
 *  ColorMap class
 * ----------------------------------------------------------------------------
 *  \date       2019
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac_ColorMap.h"
#include "codac_Exception.h"
#include "codac_VIBesFig.h"

using namespace std;
using namespace ibex;

namespace codac
{
  ColorMap::ColorMap(InterpolMode interpol_mode)
    : m_interpol_mode(interpol_mode)
  {
    assert(interpol_mode == InterpolMode::RGB || interpol_mode == InterpolMode::HSV);
  }
  
  const ColorMap& ColorMap::operator=(const ColorMap& x)
  {
    m_colormap = x.m_colormap;
    m_interpol_mode = x.m_interpol_mode;
    return *this;
  }

  void ColorMap::add_color_point(rgb color, float index)
  {
    m_colormap[index] = color;
  }

  void ColorMap::add_color_point(hsv color, float index)
  {
    m_colormap[index] = hsv2rgb(color);
  }

  void ColorMap::reverse()
  {
    map<float,rgb> reversed_map;
    for(map<float,rgb>::iterator it = m_colormap.begin() ; it != m_colormap.end() ; it++)
      reversed_map[1.-it->first] = it->second;
    m_colormap = reversed_map;
  }

  void ColorMap::set_opacity(float alpha)
  {
    assert(alpha >= 0. && alpha <= 1.);
    for(map<float,rgb>::iterator it = m_colormap.begin() ; it != m_colormap.end() ; it++)
      it->second.alpha = alpha;
  }

  bool ColorMap::is_opaque() const
  {
    for(map<float,rgb>::const_iterator it = m_colormap.begin() ; it != m_colormap.end() ; it++)
      if(it->second.alpha != 1.)
        return false;
    return true;
  }

  rgb ColorMap::color(double r) const
  {
    assert(m_colormap.size() >= 2 && "color map defined by at least two colors");

    if(std::isnan(r)) // undefined ratio
      return make_rgb((float)0.5, 0.5, 0.5);
    assert(Interval(0.,1.).contains(r) && "r between 0 and 1");

    Interval map_domain = Interval(m_colormap.begin()->first,prev(m_colormap.end())->first);
    float real_index = map_domain.lb() + r*map_domain.diam();

    if(m_colormap.find(real_index) == m_colormap.end()) // color interpolation
    {
      typename map<float,rgb>::const_iterator it_ub;
      it_ub = m_colormap.lower_bound(real_index);
      rgb rgb_lb = prev(it_ub)->second;
      rgb rgb_ub = it_ub->second;

      float local_ratio = (real_index - prev(it_ub)->first) / (it_ub->first - prev(it_ub)->first);
      
      switch(m_interpol_mode)
      {
        case InterpolMode::RGB:
          return make_rgb((float)(rgb_lb.r + (rgb_ub.r - rgb_lb.r) * local_ratio),
                          (float)(rgb_lb.g + (rgb_ub.g - rgb_lb.g) * local_ratio),
                          (float)(rgb_lb.b + (rgb_ub.b - rgb_lb.b) * local_ratio),
                          (float)(rgb_lb.alpha + (rgb_ub.alpha - rgb_lb.alpha) * local_ratio));

        case InterpolMode::HSV:
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
          throw Exception(__func__, "unable color interpolation mode");
      }
    }

    else // color key
      return m_colormap.at(real_index);
  }

  rgb ColorMap::color(double t, const Trajectory& f) const
  {
    assert(f.tdomain().contains(t));
    assert(!f.not_defined());

    Interval traj_envelope = f.codomain();
    return color((f(t) - traj_envelope.lb()) / traj_envelope.diam());
  }

  void ColorMap::displayColorMap(const string& fig_name) const
  {
    VIBesFig fig_map(fig_name);
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

  // Predefined color maps
  
  ColorMap make_haxby()
  {
    ColorMap map(InterpolMode::RGB);
    map.add_color_point(make_rgb(39,90,211), 0);
    map.add_color_point(make_rgb(40,123,245), 1);
    map.add_color_point(make_rgb(45,155,253), 2);
    map.add_color_point(make_rgb(73,209,255), 3);
    map.add_color_point(make_rgb(100,230,254), 4);
    map.add_color_point(make_rgb(118,235,226), 5);
    map.add_color_point(make_rgb(135,236,187), 6);
    map.add_color_point(make_rgb(194,252,165), 7);
    map.add_color_point(make_rgb(217,251,151), 8);
    map.add_color_point(make_rgb(233,241,131), 9);
    map.add_color_point(make_rgb(252,201,96), 10);
    map.add_color_point(make_rgb(255,184,84), 11);
    map.add_color_point(make_rgb(255,170,75), 12);
    map.add_color_point(make_rgb(255,167,83), 13);
    map.add_color_point(make_rgb(255,200,158), 14);
    map.add_color_point(make_rgb(255,233,217), 15);
    return map;
  }

  const ColorMap ColorMap::HAXBY = make_haxby();
  
  ColorMap make_default()
  {
    ColorMap map(InterpolMode::RGB);
    map.add_color_point(make_rgb(10,0,121), 0);
    map.add_color_point(make_rgb(40,0,150), 1);
    map.add_color_point(make_rgb(20,5,175), 2);
    map.add_color_point(make_rgb(0,10,200), 3);
    map.add_color_point(make_rgb(0,25,212), 4);
    map.add_color_point(make_rgb(0,40,224), 5);
    map.add_color_point(make_rgb(26,102,240), 6);
    map.add_color_point(make_rgb(13,129,248), 7);
    map.add_color_point(make_rgb(25,175,255), 8);
    map.add_color_point(make_rgb(50,190,255), 9);
    map.add_color_point(make_rgb(68,202,255), 10);
    map.add_color_point(make_rgb(97,225,240), 11);
    map.add_color_point(make_rgb(106,235,225), 12);
    map.add_color_point(make_rgb(124,235,200), 13);
    map.add_color_point(make_rgb(138,236,174), 14);
    map.add_color_point(make_rgb(172,245,168), 15);
    map.add_color_point(make_rgb(205,255,162), 16);
    map.add_color_point(make_rgb(223,245,141), 17);
    map.add_color_point(make_rgb(240,236,121), 18);
    map.add_color_point(make_rgb(247,215,104), 19);
    map.add_color_point(make_rgb(255,189,87), 20);
    map.add_color_point(make_rgb(255,160,69), 21);
    map.add_color_point(make_rgb(244,117,75), 22);
    map.add_color_point(make_rgb(238,80,78), 23);
    map.add_color_point(make_rgb(255,90,90), 24);
    map.add_color_point(make_rgb(255,124,124), 25);
    map.add_color_point(make_rgb(255,158,158), 26);
    map.add_color_point(make_rgb(245,179,174), 27);
    map.add_color_point(make_rgb(255,196,196), 28);
    map.add_color_point(make_rgb(255,215,215), 29);
    map.add_color_point(make_rgb(255,235,235), 31);
    map.add_color_point(make_rgb(255,254,253), 32);
    return map;
  }

  const ColorMap ColorMap::DEFAULT = make_default();
  
  ColorMap make_blue_tube()
  {
    ColorMap map(InterpolMode::RGB);
    map.add_color_point(make_rgb(76,110,127), 0.);
    map.add_color_point(make_rgb(136,197,228), 1.);
    return map;
  }

  const ColorMap ColorMap::BLUE_TUBE = make_blue_tube();
  
  ColorMap make_red_tube()
  {
    ColorMap map(InterpolMode::RGB);
    map.add_color_point(make_rgb(169,55,0), 0.);
    map.add_color_point(make_rgb(241,140,54), 1.);
    return map;
  }

  const ColorMap ColorMap::RED_TUBE = make_red_tube();
  
  ColorMap make_rainbow()
  {
    ColorMap map(InterpolMode::HSV);
    for(int h = 300 ; h > 0 ; h-=10)
      map.add_color_point(make_hsv(h,100,100), (300.-h)/300.);
    return map;
  }

  const ColorMap ColorMap::RAINBOW = make_rainbow();
}