/* ============================================================================
 *  tubex-lib - VibesFigure_Tube class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "ibex.h"
#include "vibes.h"
#include "tubex_VibesFigure_Tube.h"

// a real value to display unbounded slices
#include <limits>
#define BOUNDED_INFINITY numeric_limits<float>::max()

using namespace std;
using namespace ibex;

namespace tubex
{
  // Static items

    vector<VibesFigure_Tube*> VibesFigure_Tube::v_vibesfig_tube;

    void VibesFigure_Tube::draw(const string& fig_name, int x, int y)
    {
      draw(fig_name, NULL, NULL, x, y);
    }

    void VibesFigure_Tube::draw(const string& fig_name, Tube *tube, int x, int y)
    {
      draw(fig_name, tube, NULL, x, y);
    }

    void VibesFigure_Tube::draw(const string& fig_name, Trajectory *traj, int x, int y)
    {
      draw(fig_name, NULL, traj, x, y);
    }
    
    void VibesFigure_Tube::draw(const string& fig_name, Tube *tube, Trajectory *traj, int x, int y)
    {
      if(VibesFigure_Tube::v_vibesfig_tube.size() == 0)
      {
        vibes::beginDrawing();
        vibes::axisAuto();
      }

      VibesFigure_Tube *figtube = NULL;
      for(int i = 0 ; i < v_vibesfig_tube.size() ; i++)
      {
        if(v_vibesfig_tube[i]->name() == fig_name)
        {
          figtube = v_vibesfig_tube[i];
          break;
        }
      }

      if(figtube == NULL)
      {
        figtube = new VibesFigure_Tube(fig_name);
        v_vibesfig_tube.push_back(figtube);
      }

      figtube->setProperties(x, y, 700, 350);
      if(tube != NULL) figtube->addTube(tube);
      if(traj != NULL) figtube->addTrajectory(traj);
      figtube->show();
    }

    void VibesFigure_Tube::endDrawing()
    {
      for(int i = 0 ; i < v_vibesfig_tube.size() ; i++)
        delete v_vibesfig_tube[i];
      vibes::endDrawing();
    }

  // Non-static items

    VibesFigure_Tube::VibesFigure_Tube(const string& fig_name, Tube *tube, Trajectory *traj) : VibesFigure(fig_name)
    {
      if(tube != NULL) addTube(tube);
      if(traj != NULL) addTrajectory(traj);
      m_view_box = IntervalVector(2, Interval::EMPTY_SET);
    }

    VibesFigure_Tube::~VibesFigure_Tube()
    {
      m_m_tubes_copy.clear();
    }

    void VibesFigure_Tube::addTube(Tube *tube, const string& color_frgrnd, const string& color_bckgrnd)
    {
      if(idTube(tube) != -1)
        cout << "Warning VibesFigure_Tube::addTube(): tube already added" << endl;

      else
        m_v_tubes.push_back(tube);

      setTubeColor(tube, color_frgrnd, color_bckgrnd);
    }

    void VibesFigure_Tube::setTubeColor(Tube *tube, const string& color_frgrnd, const string& color_bckgrnd)
    {
      if(idTube(tube) == -1)
        cout << "Warning VibesFigure_Tube::setTubeColor(): unknown tube" << endl;

      m_m_tubes_color[tube][TUBE_FOREGROUND_COLOR] = color_frgrnd + "[" + color_frgrnd + "]";
      m_m_tubes_color[tube][TUBE_BACKGROUND_COLOR] = color_bckgrnd + "[" + color_bckgrnd + "]";
      m_m_tubes_color[tube][TUBE_SLICES_COLOR] = "lightGray[" + color_frgrnd + "]";
    }
    
    void VibesFigure_Tube::removeTube(Tube *tube)
    {
      if(idTube(tube) == -1)
        cout << "Warning VibesFigure_Tube::removeTube(): unable to remove" << endl;

      m_m_tubes_color.erase(tube);
      m_m_tubes_copy.erase(tube);
      int i;
      for(i = 0 ; i < m_v_tubes.size() ; i++)
        if(m_v_tubes[i] == tube)
          break;
      m_v_tubes.erase(m_v_tubes.begin() + i);
    }

    int VibesFigure_Tube::idTube(Tube *tube)
    {
      int i;
      bool tube_exists = false;
      for(i = 0 ; i < m_v_tubes.size() ; i++)
        if(m_v_tubes[i] == tube)
        {
          tube_exists = true;
          break;
        }
      return tube_exists ? i : -1;
    }

    void VibesFigure_Tube::addTrajectory(Trajectory *traj, const string& color)
    {
      if(idTrajectory(traj) != -1)
        cout << "Warning VibesFigure_Tube::addTrajectory(): trajectory already added" << endl;

      else
        m_v_traj.push_back(traj);
      
      m_m_traj_color[traj] = color;
    }
    
    void VibesFigure_Tube::setTrajectoryColor(Trajectory *traj, const string& color)
    {
      if(idTrajectory(traj) == -1)
        cout << "Warning VibesFigure_Tube::setTrajectoryColor(): unknown trajectory" << endl;

      m_m_traj_color[traj] = color;
    }
    
    void VibesFigure_Tube::removeTrajectory(Trajectory *traj)
    {
      if(idTrajectory(traj) == -1)
        cout << "Warning VibesFigure_Tube::removeTrajectory(): unable to remove" << endl;

      m_m_traj_color.erase(traj);
      int i;
      for(i = 0 ; i < m_v_traj.size() ; i++)
        if(m_v_traj[i] == traj)
          break;
      m_v_traj.erase(m_v_traj.begin() + i);
    }

    int VibesFigure_Tube::idTrajectory(Trajectory *traj)
    {
      int i;
      bool traj_exists = false;
      for(i = 0 ; i < m_v_traj.size() ; i++)
        if(m_v_traj[i] == traj)
        {
          traj_exists = true;
          break;
        }
      return traj_exists ? i : -1;
    }
    
    void VibesFigure_Tube::show()
    {
      show(false);
    }
    
    void VibesFigure_Tube::show(bool detail_slices)
    {
      for(int i = 0 ; i < m_v_tubes.size() ; i++)
        m_view_box |= drawTube(m_v_tubes[i]);

      // Trajectories are drawn on top of the tubes
      for(int i = 0 ; i < m_v_traj.size() ; i++)
        m_view_box |= drawTrajectory(m_v_traj[i]);

      axisLimits(m_view_box);

      // The following is used as a calibration of the SVG file
      vibes::clearGroup(m_name, "transparent_box");
      vibes::drawBox(m_view_box, vibesParams("figure", m_name, "group", "transparent_box"));
    }

    const IntervalVector VibesFigure_Tube::drawTube(Tube *tube, bool detail_slices)
    {
      IntervalVector viewbox(2, Interval::EMPTY_SET);

      // Computing viewbox

        double image_lb, image_ub;

        if(!tube->image().is_unbounded())
        {
          image_lb = tube->image().lb();
          image_ub = tube->image().ub();
        }

        else // some slices can be [-oo,+oo], maybe not all of them
        {
          image_lb = NAN;
          image_ub = NAN;

          for(int i = 0 ; i < tube->size() ; i++)
          {
            Interval slice = (*tube)[i];
            if(!slice.is_unbounded())
            {
              image_lb = std::isnan(image_lb) || image_lb > slice.lb() ? slice.lb() : image_lb;
              image_ub = std::isnan(image_ub) || image_ub < slice.ub() ? slice.ub() : image_ub;
            }
          }
        }

        viewbox[0] = tube->domain();
        viewbox[1] = Interval(image_lb, image_ub);

      // Displaying tube

        ostringstream o;
        o << "tube_" << tube->name();
        string group_name = o.str();
        string group_name_bckgrnd = o.str() + "_old";

        // Two display modes available:
        // - one in which each slice is shown
        // - one in which only the polygon envelope of the tube is shown

        // First, displaying background.
        // The background is the previous version of the tube (before contraction).
        // Always displayed as a polygon.
        {
          if(m_m_tubes_copy.find(tube) == m_m_tubes_copy.end())
          {
            // If a copy of the tube has not been done,
            // we make one and no display is done.
          }

          else
          {
            // Otherwise, the copy is displayed and then updated
            // with the current version of the tube.

            vector<double> v_x, v_y;
            computePolygonEnvelope(*m_m_tubes_copy[tube], v_x, v_y);
            vibes::clearGroup(m_name, group_name_bckgrnd);
            vibes::newGroup(group_name_bckgrnd, m_m_tubes_color[tube][TUBE_BACKGROUND_COLOR], vibesParams("figure", m_name));
            vibes::Params params_background = vibesParams("figure", m_name, "group", group_name_bckgrnd);
            vibes::drawPolygon(v_x, v_y, params_background);

            delete m_m_tubes_copy[tube];
          }

          m_m_tubes_copy[tube] = new Tube(*tube);
        }

        // Second, the foreground: actual values of the tube.
        // Can be either displayed slice by slice or with a polygon envelope.

        vibes::Params params_foreground = vibesParams("figure", m_name, "group", group_name);
        vibes::clearGroup(m_name, group_name);

        if(detail_slices)
        {
          vibes::newGroup(group_name, m_m_tubes_color[tube][TUBE_SLICES_COLOR], vibesParams("figure", m_name));
          for(int i = 0 ; i < tube->size() ; i++)
            drawSlice(tube->sliceBox(i), params_foreground);
        }

        else
        {
          vector<double> v_x, v_y;
          computePolygonEnvelope(*tube, v_x, v_y);
          vibes::newGroup(group_name, m_m_tubes_color[tube][TUBE_FOREGROUND_COLOR], vibesParams("figure", m_name));
          vibes::drawPolygon(v_x, v_y, params_foreground);
        }

      return viewbox;
    }

    void VibesFigure_Tube::computePolygonEnvelope(const Tube& tube, vector<double>& v_x, vector<double>& v_y)
    {
      if(tube.isPartiallyEmpty()) cout << "Tube graphics: warning, tube " << tube.name() << " partially empty" << endl;

      for(int i = 0 ; i < tube.size() ; i++)
      {
        IntervalVector slice_box = tube.sliceBox(i);
        slice_box[1] &= Interval(-BOUNDED_INFINITY,BOUNDED_INFINITY);
        v_x.push_back(slice_box[0].lb()); v_x.push_back(slice_box[0].ub());
        v_y.push_back(slice_box[1].ub()); v_y.push_back(slice_box[1].ub());
      }

      for(int i = tube.size() - 1 ; i >= 0 ; i--)
      {
        IntervalVector slice_box = tube.sliceBox(i);
        slice_box[1] &= Interval(-BOUNDED_INFINITY,BOUNDED_INFINITY);
        v_x.push_back(slice_box[0].ub()); v_x.push_back(slice_box[0].lb());
        v_y.push_back(slice_box[1].lb()); v_y.push_back(slice_box[1].lb());
      }
    }

    void VibesFigure_Tube::drawSlice(const IntervalVector& slice, const vibes::Params& params) const
    {
      if(slice[1].is_empty())
        return; // no display

      IntervalVector boundedSlice(slice);
      boundedSlice[1] &= Interval(-BOUNDED_INFINITY,BOUNDED_INFINITY);
      vibes::drawBox(boundedSlice, params);
    }
    
    const IntervalVector VibesFigure_Tube::drawTrajectory(Trajectory *traj, float points_size)
    {
      IntervalVector viewbox(2, Interval::EMPTY_SET);

      std::ostringstream o;
      o << "traj_" << traj->name();
      vibes::newGroup(o.str(), m_m_traj_color[traj], vibesParams("figure", m_name));

      if(traj->domain().is_unbounded() || traj->domain().is_empty())
        return viewbox;

      // Two display modes available:
      // - one by Function
      // - one by map<double,double>

      vector<double> v_x, v_y;

      if(traj->getFunction() == NULL)
      {
        typename map<double,double>::const_iterator it_scalar_values;
        for(it_scalar_values = traj->getMap().begin(); it_scalar_values != traj->getMap().end(); it_scalar_values++)
        {
          if(points_size != 0.)
            vibes::drawPoint(it_scalar_values->first, it_scalar_values->second,
                             points_size, vibesParams("figure", m_name, "group", o.str()));

          else
          {
            v_x.push_back(it_scalar_values->first);
            v_y.push_back(it_scalar_values->second);
          }

          viewbox[0] |= it_scalar_values->first;
          viewbox[1] |= it_scalar_values->second;
        }
      }

      else
      {
        for(double t = traj->domain().lb() ; t <= traj->domain().ub() ; t+=traj->domain().diam()/TRAJ_NB_DISPLAYED_POINTS)
        {
          if(points_size != 0.)
            vibes::drawPoint(t, (*traj)[t], points_size, vibesParams("figure", m_name, "group", o.str()));

          else
          {
            v_x.push_back(t);
            v_y.push_back((*traj)[t]);
          }

          viewbox[0] |= t;
          viewbox[1] |= (*traj)[t];
        }
      }

      if(v_x.size() != 0)
        vibes::drawLine(v_x, v_y, vibesParams("figure", m_name, "group", o.str()));

      return viewbox;
    }
}