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

#include "tubex_VibesFigure_Tube.h"

 // a real value to display unbounded slices
#include <limits>
#define BOUNDED_INFINITY std::numeric_limits<float>::max()

using namespace std;
using namespace ibex;

namespace tubex
{
  std::vector<VibesFigure_Tube*> VibesFigure_Tube::v_graphics;
   
  void VibesFigure_Tube::show(Tube *tube, const string& name, int x, int y)
  {
    std::map<double,double> empty_map;
    show(tube, name, empty_map, x, y);
  }
   
  void VibesFigure_Tube::show(Tube *tube, const string& name, const std::map<double,double>& map_scalar_values, int x, int y)
  {
    if(VibesFigure_Tube::v_graphics.size() == 0)
    {
      vibes::beginDrawing();
      vibes::axisAuto();
    }

    VibesFigure_Tube *figtube = NULL;
    for(int i = 0 ; i < v_graphics.size() ; i++)
    {
      if(v_graphics[i]->name() == name || name == "" && v_graphics[i]->m_v_tubes.size() > 0 && v_graphics[i]->m_v_tubes[0] == tube)
      {
        figtube = v_graphics[i];
        break;
      }
    }

    if(figtube == NULL)
    {
      figtube = new VibesFigure_Tube(name, tube);
      figtube->setProperties(x, y, 700, 350);
      string fg = "#A2A2A2", bg = "#D2D2D2";
      figtube->setColors(fg + "[" + fg + "]", fg + "[" + fg + "]", bg + "[" + bg + "]");
      figtube->showScalarValues(map_scalar_values);
      v_graphics.push_back(figtube);
    }

    figtube->show();
  }
   
  void VibesFigure_Tube::show(const std::vector<Tube*> v_tubes, const string& name, int x, int y)
  {
    std::map<double,double> empty_map;
    show(v_tubes, name, empty_map, x, y);
  }
   
  void VibesFigure_Tube::show(const std::vector<Tube*> v_tubes, const string& name, const std::map<double,double>& map_scalar_values, int x, int y)
  {
    if(VibesFigure_Tube::v_graphics.size() == 0)
    {
      vibes::beginDrawing();
      vibes::axisAuto();
    }

    VibesFigure_Tube *figtube = NULL;
    for(int i = 0 ; i < v_graphics.size() ; i++)
    {
      if(v_graphics[i]->name() == name)
      {
        figtube = v_graphics[i];
        break;
      }
    }

    if(figtube == NULL)
    {
      figtube = new VibesFigure_Tube(name, v_tubes);
      figtube->setProperties(x, y, 700, 350);
      string fg = "#A2A2A2", bg = "#D2D2D2";
      figtube->setColors(fg + "[" + fg + "]", fg + "[" + fg + "]", bg + "[" + bg + "]");
      figtube->showScalarValues(map_scalar_values);
      v_graphics.push_back(figtube);
    }

    figtube->show();
  }

  void VibesFigure_Tube::endDrawing()
  {
    for(int i = 0 ; i < v_graphics.size() ; i++)
      delete v_graphics[i];
    vibes::endDrawing();
  }


  VibesFigure_Tube::VibesFigure_Tube(const string& name, Tube *tube) : m_tubes_box(2, Interval::EMPTY_SET), VibesFigure(name)
  {
    m_v_tubes.push_back(tube);
    m_v_tubes_copy.push_back(NULL);
    m_id_map_scalar_values = 0;
    setColors("#A0A0A0[#FFEA00]", "#888888[#FFBB00]");
    m_need_to_update_axis = true;
    m_tubes_box[0] |= tube->domain();
    m_tubes_box[1] |= tube->image();
  }

  VibesFigure_Tube::VibesFigure_Tube(const string& name, vector<Tube*> v_tubes) : m_tubes_box(2, Interval::EMPTY_SET), VibesFigure(name)
  {
    string border_color = "#ffffff00";
    map<string,string> colors_map;
    colors_map["slices_old_background"] = border_color + "[red]";
    colors_map["slices"] = border_color + "[red]";
    colors_map["slices_contracted"] = border_color + "[red]";
    m_color_tubes.push_back(colors_map);
    colors_map["slices_old_background"] = border_color + "[blue]";
    colors_map["slices"] = border_color + "[blue]";
    colors_map["slices_contracted"] = border_color + "[blue]";
    m_color_tubes.push_back(colors_map);
    colors_map["slices_old_background"] = border_color + "[green]";
    colors_map["slices"] = border_color + "[green]";
    colors_map["slices_contracted"] = border_color + "[green]";
    m_color_tubes.push_back(colors_map);
    colors_map["slices_old_background"] = border_color + "[#FF8A1F]";
    colors_map["slices"] = border_color + "[#FF8A1F]";
    colors_map["slices_contracted"] = border_color + "[#FF8A1F]";
    m_color_tubes.push_back(colors_map);

    for(int i = 0 ; i < v_tubes.size() ; i++)
    {
      m_v_tubes.push_back(v_tubes[i]);
      m_v_tubes_copy.push_back(NULL);
      m_tubes_box[0] |= v_tubes[i]->domain();
      m_tubes_box[1] |= v_tubes[i]->image();
    }

    m_id_map_scalar_values = 0;
    setColors("#A0A0A0[#FFEA00]", "#888888[#FFBB00]");
    m_need_to_update_axis = false;
    axisLimits(m_tubes_box[0].lb(), m_tubes_box[0].ub(), m_tubes_box[1].lb(), m_tubes_box[1].ub());
  }

  VibesFigure_Tube::~VibesFigure_Tube()
  {
    for(int i = 0 ; i < m_v_tubes_copy.size() ; i++)
      delete m_v_tubes_copy[i];
  }

  void VibesFigure_Tube::setColors(string slices_color, string slices_contracted_color, string background_color, string truth_color)
  {
    if(slices_contracted_color == "")
      slices_contracted_color = slices_color;

    int nb_colors = m_color_tubes.size();
    for(int i = 0 ; i < m_v_tubes.size() ; i++)
    {
      if(m_v_tubes.size() > 1)
      {
        background_color = m_color_tubes[i % nb_colors]["slices_old_background"];
        slices_color = m_color_tubes[i % nb_colors]["slices"];
        slices_contracted_color = m_color_tubes[i % nb_colors]["slices_contracted"];
      }

      vibes::newGroup(groupNameSuffix("slices_old_background", i), background_color, vibesParams("figure", m_name));
      vibes::newGroup(groupNameSuffix("slices", i), slices_color, vibesParams("figure", m_name));
      vibes::newGroup(groupNameSuffix("slices_contracted", i), slices_contracted_color, vibesParams("figure", m_name));
    }
  }

  void VibesFigure_Tube::show()
  {
    return show(false, 0);
  }

  void VibesFigure_Tube::showScalarValues(const map<double,double>& map_scalar_values, const string& color, double points_size) const
  {
    m_id_map_scalar_values ++;
    std::ostringstream o;
    o << "scalar_values_" << std::hex << m_id_map_scalar_values;
    vibes::newGroup(o.str(), color, vibesParams("figure", m_name));

    vector<double> v_x, v_y;
    typename map<double,double>::const_iterator it_scalar_values;
    for(it_scalar_values = map_scalar_values.begin(); it_scalar_values != map_scalar_values.end(); it_scalar_values++)
    {
      if(points_size != 0)
        vibes::drawPoint(it_scalar_values->first,
                         it_scalar_values->second,
                         points_size,
                         vibesParams("figure", m_name, "group", o.str()));

      else
      {
        v_x.push_back(it_scalar_values->first);
        v_y.push_back(it_scalar_values->second);
      }
    }

    if(points_size == 0.)
      vibes::drawLine(v_x, v_y, vibesParams("figure", m_name, "group", o.str()));
  }

  void VibesFigure_Tube::show(bool detail_slices, int slices_limit, bool update_background)
  {
    vibes::clearGroup(m_name, "transparent_box");
    vibes::drawBox(m_tubes_box, vibesParams("figure", m_name, "group", "transparent_box"));

    for(int a = 0 ; a < m_v_tubes.size() ; a++)
    {
      if(m_need_to_update_axis)
      {
        double image_lb, image_ub;

        if(!m_v_tubes[a]->image().is_unbounded())
        {
          image_lb = m_v_tubes[a]->image().lb();
          image_ub = m_v_tubes[a]->image().ub();
        }

        else // some slices can be [-oo,+oo], maybe not all of them
        {
          image_lb = NAN;
          image_ub = NAN;

          for(int i = 0 ; i < m_v_tubes[a]->size() ; i++)
          {
            Interval slice = (*m_v_tubes[a])[i];
            if(!slice.is_unbounded())
            {
              image_lb = std::isnan(image_lb) || image_lb > slice.lb() ? slice.lb() : image_lb;
              image_ub = std::isnan(image_ub) || image_ub < slice.ub() ? slice.ub() : image_ub;
            }
          }
        }

        axisLimits(m_v_tubes[a]->domain().lb(), m_v_tubes[a]->domain().ub(), image_lb, image_ub);
        m_need_to_update_axis = false;
      }

      vibes::Params params_slices, params_contracted_slices;

      if(m_v_tubes_copy[a] != NULL && update_background)
      {
        vibes::clearGroup(m_name, groupNameSuffix("slices_old_background", a));
        params_slices = vibesParams("figure", m_name, "group", groupNameSuffix("slices_old_background", a));
        vector<double> v_x, v_y;
        computePolygonEnvelope(*m_v_tubes_copy[a], v_x, v_y);
        vibes::drawPolygon(v_x, v_y, params_slices);
      }

      vibes::clearGroup(m_name, groupNameSuffix("slices", a));
      vibes::clearGroup(m_name, groupNameSuffix("slices_contracted", a));
      params_slices = vibesParams("figure", m_name, "group", groupNameSuffix("slices", a));
      params_contracted_slices = vibesParams("figure", m_name, "group", groupNameSuffix("slices_contracted", a));
      
      if(!detail_slices)
      {
        vibes::Params params = params_slices;
        vector<double> v_x, v_y;
        computePolygonEnvelope(*m_v_tubes[a], v_x, v_y);
        vibes::drawPolygon(v_x, v_y, params_slices);
      }

      else
      {
        if(slices_limit <= 0)
          slices_limit = m_v_tubes[a]->size();

        for(int i = 0 ; i < m_v_tubes[a]->size() ; i += max((int)(m_v_tubes[a]->size() / slices_limit), 1))
        {
          vibes::Params params = params_slices;
          Interval y_i = (*m_v_tubes[a])[i];
          if(m_v_tubes_copy[a] != NULL && y_i != (*m_v_tubes_copy[a])[i]) // contraction
            params = params_contracted_slices;
          drawSlice(m_v_tubes[a]->domain(i), y_i, params);
        }
      }

      if(update_background)
      {
        if(m_v_tubes_copy[a] != NULL)
          delete m_v_tubes_copy[a];
        m_v_tubes_copy[a] = new Tube(*m_v_tubes[a]);
        m_tubes_box[0] |= m_v_tubes[a]->domain();
        m_tubes_box[1] |= m_v_tubes[a]->image();
      }

      if(m_v_tubes[a]->image() == Interval::ALL_REALS)
        m_need_to_update_axis = true;
    }
  }

  void VibesFigure_Tube::drawSlice(const Interval& intv_t, const Interval& intv_y, const vibes::Params& params) const
  {
    if(intv_y.is_empty())
      return; // no display

    double lb = std::isinf(intv_y.lb()) ? -BOUNDED_INFINITY : intv_y.lb();
    double ub = std::isinf(intv_y.ub()) ? BOUNDED_INFINITY : intv_y.ub();
    vibes::drawBox(intv_t.lb(), intv_t.ub(), lb, ub, params);
  }

  void VibesFigure_Tube::computePolygonEnvelope(const Tube& tube, vector<double>& v_x, vector<double>& v_y) const
  {
    if(tube.isPartiallyEmpty()) cout << "Tube graphics: warning, tube " << m_name << " partially empty" << endl;

    for(int i = 0 ; i < tube.size() ; i++)
    {
      IntervalVector sliceBox = tube.sliceBox(i);

      v_x.push_back(sliceBox[0].lb());
      v_x.push_back(sliceBox[0].ub());
      v_y.push_back(std::isinf(sliceBox[1].ub()) ? BOUNDED_INFINITY : sliceBox[1].ub());
      v_y.push_back(std::isinf(sliceBox[1].ub()) ? BOUNDED_INFINITY : sliceBox[1].ub());
    }

    for(int i = tube.size() - 1 ; i >= 0 ; i--)
    {
      IntervalVector sliceBox = tube.sliceBox(i);

      v_x.push_back(sliceBox[0].ub());
      v_x.push_back(sliceBox[0].lb());
      v_y.push_back(std::isinf(sliceBox[1].lb()) ? -BOUNDED_INFINITY : sliceBox[1].lb());
      v_y.push_back(std::isinf(sliceBox[1].lb()) ? -BOUNDED_INFINITY : sliceBox[1].lb());
    }
  }
}