/** 
 *  VIBesFigPaving class
 * ----------------------------------------------------------------------------
 *  \date       2016
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac_VIBesFigPaving.h"

using namespace std;
using namespace ibex;

namespace codac
{
  SetColorMap DEFAULT_SET_COLOR_MAP({
    { SetValue::IN, "#9C9C9C[green]" },
    { SetValue::UNKNOWN, "#9C9C9C[yellow]" },
    { SetValue::OUT, "#9C9C9C[cyan]" },
    { SetValue::PENUMBRA, "#9C9C9C[white]" },
  });

  SetColorMap LIE_SET_COLOR_MAP({
    { SetValue::IN, "grey[white]" },
    { SetValue::UNKNOWN, "#D55E00[#CC79A7]" },
    { SetValue::OUT, "#009E73[#56B4E9]" },
    { SetValue::PENUMBRA, "#9C9C9C[#E69F00]" },
  });

  VIBesFigPaving::VIBesFigPaving(const string& fig_name, const Paving *paving)
    : VIBesFig(fig_name), m_paving(paving)
  {
    assert(paving);

    set_properties(100, 100, 500, 500); // default properties
    axis_limits(paving->box());
    set_color_map(DEFAULT_SET_COLOR_MAP);
  }

  void VIBesFigPaving::set_color_map(const SetColorMap& color_map)
  {
    if(color_map.find(SetValue::IN) != color_map.end())
      vibes::newGroup("val_in", color_map.at(SetValue::IN), vibesParams("figure", name()));
    if(color_map.find(SetValue::UNKNOWN) != color_map.end())
      vibes::newGroup("val_unknown", color_map.at(SetValue::UNKNOWN), vibesParams("figure", name()));
    if(color_map.find(SetValue::OUT) != color_map.end())
      vibes::newGroup("val_out", color_map.at(SetValue::OUT), vibesParams("figure", name()));
    if(color_map.find(SetValue::PENUMBRA) != color_map.end())
      vibes::newGroup("val_penumbra", color_map.at(SetValue::PENUMBRA), vibesParams("figure", name()));
  }

  void VIBesFigPaving::show()
  {
    // todo: deal with color maps defined with any kind of values
    vibes::clearGroup(name(), "val_in");
    vibes::clearGroup(name(), "val_unknown");
    vibes::clearGroup(name(), "val_out");
    vibes::clearGroup(name(), "val_penumbra");
    draw_paving(m_paving);
  }

  void VIBesFigPaving::draw_paving(const Paving *paving)
  {
    assert(paving);
    
    if(paving->is_leaf())
    {
      string color_group;
      switch(paving->value())
      {
        case SetValue::IN:
          color_group = "val_in";
          break;

        case SetValue::OUT:
          color_group = "val_out";
          break;

        case SetValue::PENUMBRA:
          color_group = "val_penumbra";
          break;

        case SetValue::UNKNOWN:
        default:
          color_group = "val_unknown";
      }

      draw_box(paving->box(), vibesParams("figure", name(), "group", color_group));
    }

    else
    {
      draw_paving(paving->get_first_subpaving());
      draw_paving(paving->get_second_subpaving());
    }
  }
}