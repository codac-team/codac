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

#include <string>
#include "tubex_VibesFigure_Map.h"
#include "tubex_colors.h"
#include "tubex_Tube.h"
#include "tubex_Trajectory.h"

using namespace std;
using namespace ibex;

#define GRAY_DISPLAY_MODE 0

namespace tubex
{
  VibesFigure_Map::VibesFigure_Map(const string& fig_name)
    : VibesFigure(fig_name)
  {
    
  }

  VibesFigure_Map::~VibesFigure_Map()
  {
    typename map<const TubeVector*,FigMapTubeParams>::const_iterator it;
    for(it = m_map_tubes.begin(); it != m_map_tubes.end(); it++)
      if(it->second.tube_copy != NULL)
        delete it->second.tube_copy;
  }

  void VibesFigure_Map::add_tubevector(const TubeVector *tube, const string& name, int index_x, int index_y)
  {
    assert(tube != NULL);
    assert(index_x != index_y);
    assert(index_x >= 0 && index_x < tube->size());
    assert(index_y >= 0 && index_y < tube->size());
    assert(m_map_tubes.find(tube) == m_map_tubes.end()
      && "tube must not have been previously added");

    m_map_tubes[tube];
    m_map_tubes[tube].index_x = index_x;
    m_map_tubes[tube].index_y = index_y;
    set_tubevector_name(tube, name);

    vibes::newGroup("tube_" + name + "_bckgrnd", "lightgray[lightgray]", vibesParams("figure", this->name()));
    vibes::newGroup("tube_" + name, "gray[gray]", vibesParams("figure", this->name()));
  }

  void VibesFigure_Map::set_tubevector_name(const TubeVector *tube, const string& name)
  {
    assert(tube != NULL);
    assert(m_map_tubes.find(tube) != m_map_tubes.end()
      && "unknown tube, must be added beforehand");

    m_map_tubes[tube].name = name;
  }
  
  void VibesFigure_Map::remove_tubevector(const TubeVector *tube)
  {
    assert(tube != NULL);
    assert(m_map_tubes.find(tube) != m_map_tubes.end()
      && "unable to remove, unknown tube");

    if(m_map_tubes[tube].tube_copy != NULL)
      delete m_map_tubes[tube].tube_copy;
    m_map_tubes.erase(tube);
  }

  void VibesFigure_Map::add_trajectoryvector(const TrajectoryVector *traj, const string& name, int index_x, int index_y, int index_heading, const string& color)
  {
    assert(traj != NULL);
    assert(m_map_trajs.find(traj) == m_map_trajs.end()
      && "traj must not have been previously added");
    assert(index_x != index_y && index_x != index_heading && index_y != index_heading);
    assert(index_x >= 0 && index_x < traj->size());
    assert(index_y >= 0 && index_y < traj->size());
    assert(index_heading == -1 || (index_heading >= 0 && index_heading < traj->size()));

    m_map_trajs[traj];
    m_map_trajs[traj].index_x = index_x;
    m_map_trajs[traj].index_y = index_y;
    m_map_trajs[traj].index_heading = index_heading;

    set_trajectoryvector_name(traj, name);
    set_trajectoryvector_color(traj, color);
  }

  void VibesFigure_Map::set_trajectoryvector_name(const TrajectoryVector *traj, const string& name)
  {
    assert(traj != NULL);
    assert(m_map_trajs.find(traj) != m_map_trajs.end()
      && "unknown traj, must be added beforehand");

    m_map_trajs[traj].name = name;
  }
  
  void VibesFigure_Map::set_trajectoryvector_color(const TrajectoryVector *traj, const string& color)
  {
    assert(traj != NULL);
    assert(m_map_trajs.find(traj) != m_map_trajs.end()
      && "unable to remove, unknown traj");

    if(GRAY_DISPLAY_MODE)
      m_map_trajs[traj].color = color;

    else
      m_map_trajs[traj].color = "white";

    // Related groups are created during the display procedure
    // so that trajectories stay on top of the tubes.
  }
  
  void VibesFigure_Map::remove_trajectoryvector(const TrajectoryVector *traj)
  {
    assert(traj != NULL);
    assert(m_map_trajs.find(traj) != m_map_trajs.end()
      && "unable to remove, unknown traj");

    m_map_trajs.erase(traj);
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
  
  void VibesFigure_Map::add_observation(const IntervalVector& obs, const TrajectoryVector *traj, const string& color)
  {
    assert(traj != NULL);
    assert(m_map_trajs.find(traj) != m_map_trajs.end()
      && "unknown traj, must be added beforehand");

    // Simply directly drawn
    draw_observation(obs, traj, color, vibesParams("figure", name(), "group", "obs"));
  }
  
  void VibesFigure_Map::add_observations(const vector<IntervalVector>& v_obs, const TrajectoryVector *traj, const string& color)
  {
    assert(traj != NULL);
    assert(m_map_trajs.find(traj) != m_map_trajs.end()
      && "unknown traj, must be added beforehand");

    // Simply directly drawn
    for(int i = 0 ; i < v_obs.size() ; i++)
      add_observation(v_obs[i], traj, color);
  }

  void VibesFigure_Map::show()
  {
    typename map<const TubeVector*,FigMapTubeParams>::const_iterator it_tubes;
    for(it_tubes = m_map_tubes.begin(); it_tubes != m_map_tubes.end(); it_tubes++)
      m_view_box |= draw_tube(it_tubes->first);

    // Trajectories are drawn on top of the tubes
    typename map<const TrajectoryVector*,FigMapTrajParams>::const_iterator it_trajs;
    for(it_trajs = m_map_trajs.begin(); it_trajs != m_map_trajs.end(); it_trajs++)
      m_view_box |= draw_trajectory(it_trajs->first);

    axis_limits(m_view_box, true, 0.05);
  }

  void VibesFigure_Map::show(float robot_size)
  {
    assert(robot_size > 0.);
    m_robot_size = robot_size;
    show();
  }

  const IntervalVector VibesFigure_Map::draw_tube(const TubeVector *tube)
  {
    assert(tube != NULL);
    assert(m_map_tubes.find(tube) != m_map_tubes.end()
      && "unknown tube, must be added beforehand");

    IntervalVector viewbox(2, Interval::EMPTY_SET);

    // Computing viewbox

      Vector pos_lb(2), pos_ub(2);
      for(int i = 0 ; i < 2 ; i++)
      {
        int index = (i == 0) ? m_map_tubes[tube].index_x : m_map_tubes[tube].index_y;

        if(!(*tube)[index].codomain().is_unbounded())
        {
          pos_lb[i] = (*tube)[index].codomain().lb();
          pos_ub[i] = (*tube)[index].codomain().ub();
        }
        
        else // some slices can be [-oo,+oo], maybe not all of them
        {
          pos_lb[i] = NAN;
          pos_ub[i] = NAN;
          
          for(int k = 0 ; k < tube->nb_slices() ; k++) // todo: slice ptr iteration
          {
            Interval slice = (*tube)[index](k);
            if(!slice.is_unbounded())
            {
              pos_lb[i] = std::isnan(pos_lb[i]) || pos_lb[i] > slice.lb() ? slice.lb() : pos_lb[i];
              pos_ub[i] = std::isnan(pos_ub[i]) || pos_ub[i] < slice.ub() ? slice.ub() : pos_ub[i];
            }
          }
        }
      }
     
      viewbox = IntervalVector(pos_lb) | pos_ub;

    // Displaying tube

      draw_tube_slices(tube);

      // The background is the previous version of the tube (before contraction).
      // If a copy of the tube has not been done,
      // we make one and no display is done.
      if(m_map_tubes[tube].tube_copy == NULL)
        m_map_tubes[tube].tube_copy = new TubeVector(*tube);
        // todo: store only necesary components?

    return viewbox;
  }

  const IntervalVector VibesFigure_Map::draw_trajectory(const TrajectoryVector *traj, float points_size)
  {
    assert(traj != NULL);
    assert(m_map_trajs.find(traj) != m_map_trajs.end()
      && "unknown traj, must be added beforehand");
    assert(points_size >= 0.);

    std::ostringstream o;
    o << "traj_" << m_map_trajs[traj].name;
    string group_name = o.str();
    vibes::clearGroup(name(), group_name);
    vibes::newGroup(group_name, m_map_trajs[traj].color, vibesParams("figure", name()));

    IntervalVector viewbox(2, Interval::EMPTY_SET);
    if(traj->domain().is_unbounded() || traj->domain().is_empty())
      return viewbox;

    vector<double> v_x, v_y;
    int index_x = m_map_trajs[traj].index_x;
    int index_y = m_map_trajs[traj].index_y;

    if((*traj)[index_x].get_map().size() != 0)
    {
      typename map<double,double>::const_iterator it_scalar_values_x, it_scalar_values_y;
      it_scalar_values_x = (*traj)[index_x].get_map().begin();
      it_scalar_values_y = (*traj)[index_y].get_map().begin();
      // todo: display heading if available

      while(it_scalar_values_x != (*traj)[index_x].get_map().end())
      {
        if(points_size != 0.)
          vibes::drawPoint(it_scalar_values_x->second, it_scalar_values_y->second,
                           points_size,
                           vibesParams("figure", name(), "group", group_name));

        else
        {
          v_x.push_back(it_scalar_values_x->second);
          v_y.push_back(it_scalar_values_y->second);
        }

        viewbox[0] |= Interval(it_scalar_values_x->second);
        viewbox[1] |= Interval(it_scalar_values_y->second);

        it_scalar_values_x++;
        it_scalar_values_y++;
      }
    }

    else
    {
      for(double t = traj->domain().lb() ; t <= traj->domain().ub() ; t+=traj->domain().diam()/TRAJMAP_NB_DISPLAYED_POINTS)
      {
        double x = (*traj)[index_x](t);
        double y = (*traj)[index_y](t);

        if(points_size != 0.)
          vibes::drawPoint(x, y, points_size, vibesParams("figure", name(), "group", group_name));

        else
        {
          v_x.push_back(x);
          v_y.push_back(y);
        }

        viewbox[0] |= x;
        viewbox[1] |= y;
      }
    }

    vibes::Params params = vibesParams("figure", name(), "group", group_name);
    vibes::drawLine(v_x, v_y, params);
    draw_vehicle(traj->domain().ub(), traj, params);
    return viewbox;
  }

  void VibesFigure_Map::draw_tube_slices(const TubeVector *tube)
  {
    assert(tube != NULL);
    assert(m_map_tubes.find(tube) != m_map_tubes.end()
      && "unknown tube, must be added beforehand");

    // Reduced number of slices:
    int step = max((int)(tube->nb_slices() / MAP_SLICES_NUMBER_TO_DISPLAY), 1);

    // 1. Background:
    {
      ostringstream o;
      o << "tube_" << m_map_tubes[tube].name << "_bckgrnd";
      string group_name = o.str();
      vibes::clearGroup(name(), group_name);
      vibes::Params params = vibesParams("figure", name(), "group", group_name);

      // if available, the copy of the tube (old version) is displayed
      if(m_map_tubes[tube].tube_copy != NULL)
      {
        string color = DEFAULT_MAPBCKGRND_COLOR;

        for(int k = 0 ; k < m_map_tubes[tube].tube_copy->nb_slices() ;
            k += step * 2) // less slices for the background
        {
          IntervalVector box(2);
          box[0] = (*m_map_tubes[tube].tube_copy)[m_map_tubes[tube].index_x](k);
          box[1] = (*m_map_tubes[tube].tube_copy)[m_map_tubes[tube].index_y](k);
          draw_box(box, color, params);
        }
      }
    }

    // 2. Foreground
    {
      if((*tube)[m_map_tubes[tube].index_x].is_empty() || (*tube)[m_map_tubes[tube].index_y].is_empty())
        cout << "VibesFigure_Map: warning, empty tube " << m_map_tubes[tube].name << endl;

      ostringstream o;
      o << "tube_" << m_map_tubes[tube].name;
      string group_name = o.str();
      vibes::clearGroup(name(), group_name);
      vibes::Params params = vibesParams("figure", name(), "group", group_name);

      for(int k = tube->nb_slices() - 1 ; k >= 0 ; k -= step)
      {
        string color = shaded_slice_color((1. * k) / tube->nb_slices());

        IntervalVector box(2);
        box[0] = (*tube)[m_map_tubes[tube].index_x](k);
        box[1] = (*tube)[m_map_tubes[tube].index_y](k);
        draw_box(box, color, params);
      }
    }
  }

  const string VibesFigure_Map::shaded_slice_color(float r) const
  {
    assert(Interval(0.,1.).contains(r));

    float color_gray_min = 128, color_gray_max = 220;

    if(!GRAY_DISPLAY_MODE)
    {
      color_gray_min = 127.5;
      color_gray_max = 229.5;
    }

    int g = color_gray_min + (color_gray_max - color_gray_min) * r;

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

  void VibesFigure_Map::draw_vehicle(double t, const TrajectoryVector *traj, const vibes::Params& params)
  {
    assert(traj != NULL);
    assert(m_map_trajs.find(traj) != m_map_trajs.end()
      && "unknown traj, must be added beforehand");
    assert(traj->domain().contains(t));

    double robot_x = (*traj)[m_map_trajs[traj].index_x](t);
    double robot_y = (*traj)[m_map_trajs[traj].index_y](t);

    double next_t;
    float delta_t = traj->domain().diam() / 10000.;
    if(t >= traj->domain().lb() + delta_t) next_t = t - delta_t;
    else next_t = t + delta_t;

    double robot_next_x = (*traj)[m_map_trajs[traj].index_x](next_t);
    double robot_next_y = (*traj)[m_map_trajs[traj].index_y](next_t);
    float angle = std::atan2(robot_y - robot_next_y, robot_x - robot_next_x);
    if(next_t > t) angle += M_PI;

    vibes::drawAUV(robot_x, robot_y, angle * 180. / M_PI, m_robot_size, "gray[yellow]", params);
  }

  void VibesFigure_Map::draw_beacon(const Beacon& beacon, const string& color, const vibes::Params& params)
  {
    vibes::newGroup("beacons", DEFAULT_BEACON_COLOR, vibesParams("figure", name()));
    IntervalVector drawn_box = beacon.pos().subvector(0,1);
    draw_box(drawn_box.inflate(2.), color, params);
  }

  void VibesFigure_Map::draw_observation(const IntervalVector& obs, const TrajectoryVector *traj, const string& color, const vibes::Params& params)
  {
    assert(obs.size() == 3);
    assert(traj != NULL);
    assert(m_map_trajs.find(traj) != m_map_trajs.end()
      && "unknown traj, must be added beforehand");

    vibes::newGroup("obs", DEFAULT_OBS_COLOR, vibesParams("figure", name()));
    vector<double> v_x, v_y;
    v_x.push_back((*traj)[m_map_trajs[traj].index_x](obs[0].mid()));
    v_y.push_back((*traj)[m_map_trajs[traj].index_y](obs[0].mid()));
    v_x.push_back(v_x[0] + std::cos(obs[2].mid()) * obs[1].mid());
    v_y.push_back(v_y[0] + std::sin(obs[2].mid()) * obs[1].mid());
    draw_line(v_x, v_y, color, params);
  }
}