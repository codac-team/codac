/** 
 *  VIBesFigTube class
 * ----------------------------------------------------------------------------
 *  \date       2016
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac_Interval.h"
#include "codac_IntervalVector.h"
#include "codac_Tools.h"
#include "codac_ConvexPolygon.h"
#include "codac_VIBesFigTube.h"
#include "codac_Exception.h"

using namespace std;
using namespace ibex;

namespace codac
{
  VIBesFigTube::VIBesFigTube(const string& fig_name, const Tube *tube, const Trajectory *traj)
    : VIBesFig(fig_name)
  {
    set_properties(100, 100, 600, 300); // default properties
    if(tube) add_tube(tube, DEFAULT_TUBE_NAME);
    if(traj) add_trajectory(traj, DEFAULT_TRAJ_NAME);
  }

  VIBesFigTube::~VIBesFigTube()
  {
    typename map<const Tube*,FigTubeParams>::const_iterator it;
    for(it = m_map_tubes.begin(); it != m_map_tubes.end(); it++)
      if(it->second.tube_copy)
        delete it->second.tube_copy;
  }
  
  void VIBesFigTube::show()
  {
    show(false);
  }
  
  void VIBesFigTube::show(bool detail_slices)
  {
    typename map<const Tube*,FigTubeParams>::const_iterator it_tubes;
    for(it_tubes = m_map_tubes.begin(); it_tubes != m_map_tubes.end(); it_tubes++)
      m_view_box |= draw_tube(it_tubes->first, detail_slices);

    // Trajectories are drawn on top of the tubes
    typename map<const Trajectory*,FigTrajParams>::const_iterator it_trajs;
    for(it_trajs = m_map_trajs.begin(); it_trajs != m_map_trajs.end(); it_trajs++)
      m_view_box |= draw_trajectory(it_trajs->first);

    // Cursor is drawn over the tubes
    vibes::clearGroup(name(), "cursor");
    vibes::newGroup("cursor", "#B13200", vibesParams("figure", name()));
    if(m_display_cursor)
    {
      vector<double> v_x, v_y;
      v_x.push_back(m_cursor); v_y.push_back(m_view_box[1].lb());
      v_x.push_back(m_cursor); v_y.push_back(m_view_box[1].ub());
      draw_line(v_x, v_y, vibesParams("figure", name(), "group", "cursor"));
    }
    
    axis_limits(m_view_box);
  }

  void VIBesFigTube::set_cursor(double t)
  {
    m_cursor = t;
    show_cursor(true);
  }

  void VIBesFigTube::show_cursor(bool display)
  {
    m_display_cursor = display;
  }

  void VIBesFigTube::add_tube(const Tube *tube, const string& name, const string& color_frgrnd, const string& color_bckgrnd)
  {
    assert(tube);
    if(m_map_tubes.find(tube) != m_map_tubes.end())
      throw Exception(__func__, "tube must not have been previously added");

    m_map_tubes[tube];
    set_tube_name(tube, name);

    m_map_tubes[tube].m_colors[TubeColorType::BACKGROUND] = color_bckgrnd;
    m_map_tubes[tube].m_colors[TubeColorType::FOREGROUND] = color_frgrnd;
    m_map_tubes[tube].m_colors[TubeColorType::SLICES] = DEFAULT_SLICES_COLOR;
    m_map_tubes[tube].m_colors[TubeColorType::POLYGONS] = DEFAULT_POLYGONS_COLOR;
    m_map_tubes[tube].m_colors[TubeColorType::GATES] = DEFAULT_GATES_COLOR;
    create_groups_color(tube);
  }
  
  void VIBesFigTube::add_tubes(const TubeVector *tubevector, const string& name, const string& color_frgrnd, const string& color_bckgrnd)
  {
    assert(tubevector);
    add_tubes(tubevector, 0, tubevector->size()-1, name, color_frgrnd, color_bckgrnd);
  }
  
  void VIBesFigTube::add_tubes(const TubeVector *tubevector, int start_index, int end_index, const string& name, const string& color_frgrnd, const string& color_bckgrnd)
  {
    assert(tubevector);
    assert(start_index <= end_index && start_index >= 0 && end_index < tubevector->size());
    for(int i = start_index ; i <= end_index ; i++)
      add_tube(&(*tubevector)[i], name + std::to_string(i+1), color_frgrnd, color_bckgrnd);
  }

  void VIBesFigTube::set_tube_name(const Tube *tube, const string& name)
  {
    assert(tube);
    if(m_map_tubes.find(tube) == m_map_tubes.end())
      throw Exception(__func__, "unknown tube, must be added beforehand");

    m_map_tubes[tube].name = name;
  }
  
  void VIBesFigTube::set_tube_derivative(const Tube *tube, const Tube *derivative)
  {
    assert(tube && derivative);
    if(m_map_tubes.find(tube) == m_map_tubes.end())
      throw Exception(__func__, "unknown tube, must be added beforehand");

    m_map_tubes[tube].tube_derivative = derivative;
  }

  void VIBesFigTube::set_tube_color(const Tube *tube, const string& color_frgrnd, const string& color_bckgrnd)
  {
    assert(tube);
    if(m_map_tubes.find(tube) == m_map_tubes.end())
      throw Exception(__func__, "unknown tube, must be added beforehand");

    m_map_tubes[tube].m_colors[TubeColorType::BACKGROUND] = color_bckgrnd;
    m_map_tubes[tube].m_colors[TubeColorType::FOREGROUND] = color_frgrnd;
    create_groups_color(tube);
  }

  void VIBesFigTube::set_tube_color(const Tube *tube, TubeColorType color_type, const string& color)
  {
    assert(tube);
    if(m_map_tubes.find(tube) == m_map_tubes.end())
      throw Exception(__func__, "unknown tube, must be added beforehand");

    m_map_tubes[tube].m_colors[color_type] = color;
    create_groups_color(tube);
  }
  
  void VIBesFigTube::reset_tube_background(const Tube *tube)
  {
    assert(tube);
    if(m_map_tubes.find(tube) == m_map_tubes.end())
      throw Exception(__func__, "unable to reset the background, unknown tube");
    delete m_map_tubes[tube].tube_copy;
    m_map_tubes[tube].tube_copy = nullptr;
  }
  
  void VIBesFigTube::remove_tube(const Tube *tube)
  {
    assert(tube);
    if(m_map_tubes.find(tube) == m_map_tubes.end())
      throw Exception(__func__, "unable to remove, unknown tube");

    if(m_map_tubes[tube].tube_copy)
      delete m_map_tubes[tube].tube_copy;
    m_map_tubes.erase(tube);
  }

  void VIBesFigTube::add_trajectory(const Trajectory *traj, const string& name, const string& color)
  {
    assert(traj);
    if(m_map_trajs.find(traj) != m_map_trajs.end())
      throw Exception(__func__, "trajectory must not have been previously added");

    m_map_trajs[traj];
    set_trajectory_name(traj, name);
    set_trajectory_color(traj, color);
  }
  
  void VIBesFigTube::add_trajectories(const TrajectoryVector *trajvector, const string& name, const string& color)
  {
    assert(trajvector);
    add_trajectories(trajvector, 0, trajvector->size()-1, name, color);
  }
  
  void VIBesFigTube::add_trajectories(const TrajectoryVector *trajvector, int start_index, int end_index, const string& name, const string& color)
  {
    assert(trajvector);
    assert(start_index <= end_index && start_index >= 0 && end_index < trajvector->size());
    for(int i = start_index ; i <= end_index ; i++)
      add_trajectory(&(*trajvector)[i], name + std::to_string(i+1), color);
  }

  void VIBesFigTube::set_trajectory_name(const Trajectory *traj, const string& name)
  {
    assert(traj);
    if(m_map_trajs.find(traj) == m_map_trajs.end())
      throw Exception(__func__, "unknown trajectory, must be added beforehand");

    m_map_trajs[traj].name = name;
  }
  
  void VIBesFigTube::set_trajectory_color(const Trajectory *traj, const string& color)
  {
    assert(traj);
    if(m_map_trajs.find(traj) == m_map_trajs.end())
      throw Exception(__func__, "unknown trajectory, must be added beforehand");

    m_map_trajs[traj].color = color;

    // Related groups are created during the display procedure
    // so that trajectories stay on top of the tubes.
  }
  
  void VIBesFigTube::set_trajectory_points_size(const Trajectory *traj, double points_size)
  {
    assert(traj);
    if(m_map_trajs.find(traj) == m_map_trajs.end())
      throw Exception(__func__, "unknown trajectory, must be added beforehand");
    assert(points_size >= 0.);

    m_map_trajs[traj].points_size = points_size;
  }
  
  void VIBesFigTube::remove_trajectory(const Trajectory *traj)
  {
    assert(traj);
    if(m_map_trajs.find(traj) == m_map_trajs.end())
      throw Exception(__func__, "unable to remove, unknown trajectory");

    m_map_trajs.erase(traj);
  }

  void VIBesFigTube::create_group_color(const Tube *tube, TubeColorType color_type)
  {
    assert(tube);
    if(m_map_tubes.find(tube) == m_map_tubes.end())
      throw Exception(__func__, "unknown tube, must be added beforehand");

    // Creating group:
    ostringstream o;
    o << "tube_" << m_map_tubes[tube].name;
    string group_name = o.str();

    string suffix;
    switch(color_type)
    {
      case TubeColorType::FOREGROUND:
        suffix = "";
        break;

      case TubeColorType::SLICES:
        suffix = "_slices";
        break;

      case TubeColorType::POLYGONS:
        suffix = "_polygons";
        break;

      case TubeColorType::GATES:
        suffix = "_gates";
        break;

      case TubeColorType::BACKGROUND:
        suffix = "_old";
        break;
    }

    vibes::newGroup(group_name + suffix,
                    m_map_tubes[tube].m_colors[color_type],
                    vibesParams("figure", name()));
  }

  void VIBesFigTube::create_groups_color(const Tube *tube)
  {
    // All groups are created again to keep a correct display order
    create_group_color(tube, TubeColorType::BACKGROUND);
    create_group_color(tube, TubeColorType::FOREGROUND);
    create_group_color(tube, TubeColorType::SLICES);
    create_group_color(tube, TubeColorType::POLYGONS);
    create_group_color(tube, TubeColorType::GATES); // layer on top of the other
  }

  const IntervalVector VIBesFigTube::draw_tube(const Tube *tube, bool detail_slices)
  {
    assert(tube);
    if(m_map_tubes.find(tube) == m_map_tubes.end())
      throw Exception(__func__, "unknown tube, must be added beforehand");

    IntervalVector viewbox(2, Interval::EMPTY_SET);

    // Computing viewbox

      double image_lb, image_ub;

      if(!tube->codomain().is_unbounded())
      {
        image_lb = tube->codomain().lb();
        image_ub = tube->codomain().ub();
      }

      else // some slices can be [-oo,+oo], maybe not all of them
      {
        image_lb = NAN;
        image_ub = NAN;

        for(const Slice *s = tube->first_slice() ; s ; s = s->next_slice())
        {
          Interval codomain = s->codomain();
          if(!codomain.is_unbounded())
          {
            image_lb = std::isnan(image_lb) || image_lb > codomain.lb() ? codomain.lb() : image_lb;
            image_ub = std::isnan(image_ub) || image_ub < codomain.ub() ? codomain.ub() : image_ub;
          }
        }
      }

      viewbox[0] = tube->tdomain();
      viewbox[1] = Interval(image_lb, image_ub);

    // Displaying tube

      ostringstream o;
      o << "tube_" << m_map_tubes[tube].name;
      string group_name = o.str();
      string group_name_bckgrnd = o.str() + "_old";

      // Two display modes available:
      // - one in which each slice is shown
      // - one in which only the polygon envelope of the tube is shown

      // First, displaying background.
      // The background is the previous version of the tube (before contraction).
      // Always displayed as a polygon.
      {
        if(m_map_tubes[tube].tube_copy == nullptr)
        {
          // If a copy of the tube has not been done,
          // we make one and no display is done.

          if(!tube->codomain().is_unbounded())
            m_map_tubes[tube].tube_copy = new Tube(*tube);
        }

        else
        {
          // Otherwise, the copy is displayed and then updated
          // with the current version of the tube.

          vibes::clearGroup(name(), group_name_bckgrnd);
          vibes::Params params_background = vibesParams("figure", name(), "group", group_name_bckgrnd);
          if(!m_map_tubes[tube].tube_copy->is_empty())
            draw_polygon(m_map_tubes[tube].tube_copy->polygon_envelope(), params_background);
        }
      }

      // Second, the foreground: actual values of the tube.
      // Can be either displayed slice by slice or with a polygon envelope.
      {
        vibes::clearGroup(name(), group_name);
        vibes::clearGroup(name(), group_name + "_polygons");
        vibes::clearGroup(name(), group_name + "_gates");
        vibes::clearGroup(name(), group_name + "_slices");

        if(detail_slices)
        {
          vibes::Params params_foreground_slices = vibesParams("group", group_name + "_slices");
          vibes::Params params_foreground_polygons = vibesParams("group", group_name + "_polygons");
          vibes::Params params_foreground_gates = vibesParams("group", group_name + "_gates", "FixedScale", true);

          const Slice *slice = tube->first_slice();
          const Slice *deriv_slice = nullptr;

          if(m_map_tubes[tube].tube_derivative)
            deriv_slice = m_map_tubes[tube].tube_derivative->first_slice();

          draw_gate(slice->input_gate(), tube->tdomain().lb(), params_foreground_gates);

          while(slice)
          {
            if(deriv_slice)
              draw_slice(*slice, *deriv_slice, params_foreground_slices, params_foreground_polygons);
            else
              draw_slice(*slice, params_foreground_slices);

            draw_gate(slice->output_gate(), slice->tdomain().ub(), params_foreground_gates);
            slice = slice->next_slice();
            
            if(deriv_slice)
              deriv_slice = deriv_slice->next_slice();
          }
        }

        else
        {
          vibes::Params params_foreground = vibesParams("group", group_name);
          if(tube->is_empty())
            cout << "Tube graphics: warning, empty tube (" << name() << "),"
                 << " try again by drawing slices" << endl;
          else
            draw_polygon(tube->polygon_envelope(), params_foreground);
        }
      }

    return viewbox;
  }

  void VIBesFigTube::draw_slice(const Slice& slice, const vibes::Params& params)
  {
    if(slice.codomain().is_empty())
      return; // no display
    draw_box(slice.box(), params);
  }

  void VIBesFigTube::draw_slice(const Slice& slice, const Slice& deriv_slice, const vibes::Params& params_slice, const vibes::Params& params_polygon)
  {
    assert(slice.tdomain() == deriv_slice.tdomain());

    if(slice.codomain().is_empty())
      return; // no display

    draw_slice(slice, params_slice);
    draw_polygon(slice.polygon(deriv_slice), params_polygon);
  }

  void VIBesFigTube::draw_gate(const Interval& gate, double t, const vibes::Params& params)
  {
    if(gate.is_empty())
      return; // no display
    
    else if(gate.is_degenerated())
      draw_point(ThickPoint(t, gate.lb()), params);

    else
    {
      IntervalVector gate_box(2);
      gate_box[0] = t; gate_box[0].inflate(ibex::next_float(0.));
      gate_box[1] = trunc_inf(gate);
      draw_box(gate_box, params);
    }
  }
  
  const IntervalVector VIBesFigTube::draw_trajectory(const Trajectory *traj)
  {
    assert(traj);
    assert(!traj->not_defined());
    if(m_map_trajs.find(traj) == m_map_trajs.end())
      throw Exception(__func__, "unknown trajectory, must be added beforehand");

    IntervalVector viewbox(2, Interval::EMPTY_SET);

    // Trajectories are drawn on top of the tubes,
    // so related groups are created again:
    std::ostringstream o;
    o << "traj_" << m_map_trajs[traj].name;
    string group_name = o.str();
    vibes::clearGroup(name(), group_name);
    vibes::newGroup(group_name, m_map_trajs[traj].color, vibesParams("figure", name()));

    if(traj->tdomain().is_unbounded() || traj->tdomain().is_empty())
      return viewbox;

    // Two display modes available:
    // - one by Fnc
    // - one by map<double,double>

    vector<double> v_x, v_y;

    if(traj->definition_type() == TrajDefnType::MAP_OF_VALUES)
    {
      typename map<double,double>::const_iterator it_scalar_values;
      for(it_scalar_values = traj->sampled_map().begin(); it_scalar_values != traj->sampled_map().end(); it_scalar_values++)
      {
        if(m_map_trajs[traj].points_size != 0.)
          draw_point(ThickPoint(it_scalar_values->first, it_scalar_values->second), m_map_trajs[traj].points_size, vibesParams("figure", name(), "group", group_name));

        else
        {
          v_x.push_back(it_scalar_values->first);

          if(std::isnan(it_scalar_values->second))
          {
            cout << "Trajectory graphics: warning, nan values (" << name() << "),"
                 << " at " << it_scalar_values->first << endl;
            return viewbox;
          }

          v_y.push_back(it_scalar_values->second);
        }

        viewbox[0] |= it_scalar_values->first;
        viewbox[1] |= it_scalar_values->second;
      }
    }

    else
    {
      for(double t = traj->tdomain().lb() ; t <= traj->tdomain().ub() ; t+=traj->tdomain().diam()/TRAJ_NB_DISPLAYED_POINTS)
      {
        if(m_map_trajs[traj].points_size != 0.)
          draw_point(ThickPoint(t, (*traj)(t)), m_map_trajs[traj].points_size, vibesParams("figure", name(), "group", group_name));

        else
        {
          v_x.push_back(t);
          v_y.push_back((*traj)(t));
        }

        viewbox[0] |= t;
        viewbox[1] |= (*traj)(t);
      }
    }

    if(v_x.size() != 0)
      vibes::drawLine(v_x, v_y, vibesParams("figure", name(), "group", group_name));
    
    return viewbox;
  }
}