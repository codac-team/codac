/** 
 *  codac2_ColorMap.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_ColorMap.h"

using namespace std;
using namespace codac2;

ColorMap::ColorMap(Model m_) : 
  m(m_)
{ }

Color ColorMap::color(float r) const
{
    assert (this->size() >= 2);
    if(std::isnan(r)) // undefined ratio
      return Color({0.5, 0.5, 0.5});
    assert(Interval(0.,1.).contains(r));

    Interval map_domain = Interval(this->begin()->first,prev(this->end())->first);
    float real_index = map_domain.lb() + r*map_domain.diam();

    if(this->find(real_index) == this->end()) // color interpolation
    {
      typename map<float,Color>::const_iterator it_ub;
      it_ub = this->lower_bound(real_index);
      Color color_lb = prev(it_ub)->second;
      Color color_ub = it_ub->second;

      // Interpolation according to the ColorMap model

      if (m == Model::RGB)
      {
        color_lb = color_lb.rgb();
        color_ub = color_ub.rgb();
      }

      else if (m == Model::HSV)
      {
        color_lb = color_lb.hsv();
        color_ub = color_ub.hsv();
      }
      

      float local_ratio = (real_index - prev(it_ub)->first) / (it_ub->first - prev(it_ub)->first);
 
    return Color({(color_lb[0] + (color_ub[0] - color_lb[0]) * local_ratio),
                    (color_lb[1] + (color_ub[1] - color_lb[1]) * local_ratio),
                    (color_lb[2] + (color_ub[2] - color_lb[2]) * local_ratio),
                    (color_lb[3] + (color_ub[3] - color_lb[3]) * local_ratio)},color_lb.model());

    }

    else // color key
      return this->at(real_index);
}