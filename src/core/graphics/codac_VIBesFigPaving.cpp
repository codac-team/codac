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
  VIBesFigPaving::VIBesFigPaving(const string& fig_name, const Paving *paving)
    : VIBesFig(fig_name), m_paving(paving)
  {
    assert(paving != NULL);

    set_properties(100, 100, 500, 500); // default properties
    axis_limits(paving->box());

    // Default color map
    map<SetValue,string> color_map;
    color_map[SetValue::IN] = "#9C9C9C[green]";
    color_map[SetValue::UNKNOWN] = "#9C9C9C[yellow]";
    color_map[SetValue::OUT] = "#9C9C9C[cyan]";
    color_map[SetValue::UNCHECKED] = "#9C9C9C[magenta]";
    color_map[SetValue::PENUMBRA] = "#9C9C9C[white]";
    set_color_map(color_map);
  }

  void VIBesFigPaving::set_color_map(const map<SetValue,string>& color_map)
  {
    // todo: deal with color maps defined with any kind of values
    vibes::newGroup("val_in", color_map.at(SetValue::IN), vibesParams("figure", name()));
    vibes::newGroup("val_unknown", color_map.at(SetValue::UNKNOWN), vibesParams("figure", name()));
    vibes::newGroup("val_out", color_map.at(SetValue::OUT), vibesParams("figure", name()));
    vibes::newGroup("val_unchecked", color_map.at(SetValue::UNCHECKED), vibesParams("figure", name()));
    vibes::newGroup("val_penumbra", color_map.at(SetValue::PENUMBRA), vibesParams("figure", name()));
  }

  void VIBesFigPaving::show()
  {
    // todo: deal with color maps defined with any kind of values
    vibes::clearGroup(name(), "val_in");
    vibes::clearGroup(name(), "val_unknown");
    vibes::clearGroup(name(), "val_out");
    vibes::clearGroup(name(), "val_unchecked");
    vibes::clearGroup(name(), "val_penumbra");
    draw_paving(m_paving);
  }

  void VIBesFigPaving::draw_paving(const Paving *paving)
  {
    assert(paving != NULL);
    
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

        case SetValue::UNCHECKED:
          color_group = "val_unchecked";
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