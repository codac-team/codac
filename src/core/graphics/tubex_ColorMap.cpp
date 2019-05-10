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

using namespace std;
using namespace ibex;

namespace tubex
{
  ColorMap::ColorMap()
  {
    
  }

  void ColorMap::add_color_point(rgb color, float index)
  {
    m_colormap[index] = color;
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
  
      return make_rgb((float)(rgb_lb.r + (rgb_ub.r - rgb_lb.r) * local_ratio),
                      (float)(rgb_lb.g + (rgb_ub.g - rgb_lb.g) * local_ratio),
                      (float)(rgb_lb.b + (rgb_ub.b - rgb_lb.b) * local_ratio),
                      (float)(rgb_lb.alpha + (rgb_ub.alpha - rgb_lb.alpha) * local_ratio));
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
}