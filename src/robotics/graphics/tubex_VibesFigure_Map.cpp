/* ============================================================================
 *  tubex-lib - VibesFigure_Map class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "tubex_VibesFigure_Map.h"
#include "tubex_Colors.h"

using namespace std;
using namespace ibex;

#define GRAY_DISPLAY_MODE         0
#define SLICES_NUMBER_TO_DISPLAY  1000

namespace tubex
{
  VibesFigure_Map::VibesFigure_Map(const string& fig_name, const TubeVector *tube, const TrajectoryVector *traj)
    : VibesFigure_Tube(fig_name, 1) // one layer
  {
    add_tube(tube, DEFAULT_TUBE_NAME);
    add_trajectory(traj, DEFAULT_TRAJ_NAME, GRAY_DISPLAY_MODE ? DEFAULT_TRAJ_COLOR : "white");

    vibes::newGroup("beacons", DEFAULT_BEACON_COLOR, vibesParams("figure", name()));
  }

  void VibesFigure_Map::add_beacon(const Beacon& beacon, const string& color)
  {
    // Simply directly drawn
    draw_beacon(beacon, color, vibesParams("figure", name(), "group", "beacons"));
  }

  void VibesFigure_Map::add_beacons(const vector<Beacon>& v_beacons, const string& color)
  {
    // Simply directly drawn
    for(int i = 0 ; i < v_beacons.size() ; i++)
      add_beacon(v_beacons[i], color);
  }

  const IntervalVector VibesFigure_Map::draw_tube(const TubeVector *tube, bool detail_slices)
  {
    IntervalVector viewbox(2, Interval::EMPTY_SET);

    // Computing viewbox

      Vector image_lb(2), image_ub(2);
      for(int k = 0 ; k < 2 ; k++)
      {
        if(!tube->codomain().is_unbounded())
        {
          image_lb[k] = tube->codomain()[k].lb();
          image_ub[k] = tube->codomain()[k].ub();
        }

        else // some slices can be [-oo,+oo], maybe not all of them
        {
          image_lb[k] = NAN;
          image_ub[k] = NAN;

          for(int i = 0 ; i < tube->nb_slices() ; i++) // todo: slice ptr iteration
          {
            Interval slice = (*tube)[i][k];
            if(!slice.is_unbounded())
            {
              image_lb[k] = std::isnan(image_lb[k]) || image_lb[k] > slice.lb() ? slice.lb() : image_lb[k];
              image_ub[k] = std::isnan(image_ub[k]) || image_ub[k] < slice.ub() ? slice.ub() : image_ub[k];
            }
          }
        }
      }

      viewbox = IntervalVector(image_lb) | image_ub;

    // Displaying tube

      ostringstream o;
      o << "tube_" << m_map_tubes[tube].name;
      string group_name = o.str();
      string group_name_bckgrnd = o.str() + "_old";

      // First, displaying background.
      // The background is the previous version of the tube (before contraction).
      {
        if(m_map_tubes[tube].tube_copy == NULL)
        {
          // If a copy of the tube has not been done,
          // we make one and no display is done.
          m_map_tubes[tube].tube_copy = new TubeVector(*tube);
        }

        else
        {
          // Otherwise, the copy is displayed and then updated
          // with the current version of the tube.

          vibes::clearGroup(name(), group_name_bckgrnd);
          vibes::Params params_background = vibesParams("figure", name(), "group", group_name_bckgrnd);
          draw_tube_slices(m_map_tubes[tube].tube_copy, params_background, true);

          //delete m_map_tubes[tube].tube_copy;
        }
      }

      // Second, the foreground: actual values of the tube.
      {
        vibes::Params params_foreground = vibesParams("group", group_name);
        vibes::clearGroup(name(), group_name);
        draw_tube_slices(tube, params_foreground, false);
      }

    return keep_ratio(viewbox);
  }

  const IntervalVector VibesFigure_Map::draw_trajectory(const TrajectoryVector *traj, float points_size)
  {
    IntervalVector viewbox(2, Interval::EMPTY_SET);

    std::ostringstream o;
    o << "traj_" << m_map_trajs[traj].name;
    string group_name = o.str();
    vibes::clearGroup(name(), group_name);

    if(traj->domain().is_unbounded() || traj->domain().is_empty())
      return viewbox;

    vector<double> v_x, v_y;

    if(traj->get_map().size() != 0)
    {
      typename map<double,Vector>::const_iterator it_scalar_values;
      for(it_scalar_values = traj->get_map().begin(); it_scalar_values != traj->get_map().end(); it_scalar_values++)
      {
        if(points_size != 0.)
          vibes::drawPoint(it_scalar_values->second[0], it_scalar_values->second[1],
                           points_size,
                           vibesParams("figure", name(), "group", group_name));

        else
        {
          v_x.push_back(it_scalar_values->second[0]);
          v_y.push_back(it_scalar_values->second[1]);
        }

        viewbox |= it_scalar_values->second.subvector(0,1);
      }

      vibes::drawLine(v_x, v_y, vibesParams("figure", name(), "group", group_name));
    }

    return keep_ratio(viewbox);
  }

  void VibesFigure_Map::draw_tube_slices(const TubeVector *tube, const vibes::Params& params, bool background_tube)
  {
    int step = max((int)(tube->nb_slices() / SLICES_NUMBER_TO_DISPLAY), 1);

    int i = 0;
    string color;
    if(background_tube)
      color = DEFAULT_BCKGRND_COLOR;

    for(const Slice *s = tube->get_first_slice() ; s != NULL ; )
    {
      if(!background_tube)
        color = shaded_slice_color((1. * i) / tube->nb_slices());
      draw_slice(*s, color, params);

      // Reduced number of slices
      for(int j = 0 ; j < step && s != NULL ; j++)
      {
        i++;
        s = s->next_slice();
      }
    }
  }

  const string VibesFigure_Map::shaded_slice_color(float r) const
  {
    float color_gray_min = 128, color_gray_max = 220;

    if(!GRAY_DISPLAY_MODE)
    {
      color_gray_min = 127.5;
      color_gray_max = 229.5;
    }

    int g = color_gray_min + (color_gray_max - color_gray_min) * (1. - r);

    hsv hsv_value;
    hsv_value.v = g / 255.;
    hsv_value.h = 0.;
    hsv_value.s = 0.;

    if(!GRAY_DISPLAY_MODE)
    {
      hsv_value.h = 200.;
      hsv_value.s = 0.40;
    }

    string hex_color = rgb2hex(hsv2rgb(hsv_value));
    return hex_color + "[" + hex_color + "]";
  }

  void VibesFigure_Map::draw_slice(const Slice& slice, const std::string& color, const vibes::Params& params)
  {
    if(slice.codomain().subvector(0,1).is_empty())
      return; // no display

    draw_box(slice.codomain().subvector(0,1), color, params);
  }

  void VibesFigure_Map::draw_beacon(const Beacon& beacon, const string& color, const vibes::Params& params)
  {
    IntervalVector drawn_box = beacon.pos().subvector(0,1);
    draw_box(drawn_box.inflate(2.), color, params);
  }

  const IntervalVector VibesFigure_Map::keep_ratio(const IntervalVector& viewbox) const
  {
    IntervalVector ratio_box(2);
    ratio_box[0] = viewbox[0].mid();
    ratio_box[0].inflate(viewbox.max_diam() / 2.);
    ratio_box[1] = viewbox[1].mid();
    ratio_box[1].inflate(viewbox.max_diam() / 2.);
    return ratio_box;
  }
}