/* ============================================================================
 *  tube-lib - VibesFigure_Tube class
 * ============================================================================
 *  Copyright : Copyright 2016 Simon Rohou
 *  License   : This program can be distributed under the terms of
 *              the Apache License, Version 2.0. See the file LICENSE.
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "VibesFigure_Tube.h"

using namespace std;
using namespace ibex;

VibesFigure_Tube::VibesFigure_Tube(const string& name, Tube *tube) : VibesFigure(name)
{
  m_tube = tube;
  m_tube_copy = NULL;
  m_id_map_scalar_values = 0;
  setColors("#A0A0A0[#FFEA00]", "#888888[#FFBB00]");
  m_need_to_update_axis = true;
}

VibesFigure_Tube::~VibesFigure_Tube()
{
  delete m_tube_copy;
}

void VibesFigure_Tube::setColors(string slices_color, string slices_contracted_color, string background_color, string truth_color)
{
  if(slices_contracted_color == "")
    slices_contracted_color = slices_color;

  vibes::newGroup("slices_old_background", background_color, vibesParams("figure", m_name));
  vibes::newGroup("slices", slices_color, vibesParams("figure", m_name));
  vibes::newGroup("slices_contracted", slices_contracted_color, vibesParams("figure", m_name));
  vibes::newGroup("true_values", truth_color, vibesParams("figure", m_name));
}

void VibesFigure_Tube::show() const
{
  return show(m_tube->size());
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

void VibesFigure_Tube::show(int slices_limit, bool update_background) const
{
  if(m_need_to_update_axis)
  {
    double image_lb, image_ub;

    if(!m_tube->image().is_unbounded())
    {
      image_lb = m_tube->image().lb();
      image_ub = m_tube->image().ub();
    }

    else // some slices can be [-oo,+oo], maybe not all of them
    {
      image_lb = NAN;
      image_ub = NAN;

      for(int i = 0 ; i < m_tube->size() ; i++)
      {
        Interval slice = (*m_tube)[i];
        if(!slice.is_unbounded())
        {
          image_lb = isnan(image_lb) || image_lb > slice.lb() ? slice.lb() : image_lb;
          image_ub = isnan(image_ub) || image_ub < slice.ub() ? slice.ub() : image_ub;
        }
      }
    }

    vibes::axisLimits(m_tube->domain().lb(), m_tube->domain().ub(), image_lb, image_ub, m_name);
    m_need_to_update_axis = false;
  }

  vibes::Params params_slices, params_contracted_slices;

  if(m_tube_copy != NULL && update_background)
  {
    vibes::clearGroup(m_name, "slices_old_background");
    params_slices = vibesParams("figure", m_name, "group", "slices_old_background");
    vector<double> v_x, v_y;
    computePolygonEnvelope(*m_tube_copy, v_x, v_y);
    vibes::drawPolygon(v_x, v_y, params_slices);
  }

  vibes::clearGroup(m_name, "slices");
  vibes::clearGroup(m_name, "slices_contracted");
  params_slices = vibesParams("figure", m_name, "group", "slices");
  params_contracted_slices = vibesParams("figure", m_name, "group", "slices_contracted");

  if(slices_limit <= 0)
    slices_limit = m_tube->size();

  for(int i = 0 ; i < m_tube->size() ; i += max((int)(m_tube->size() / slices_limit), 1))
  {
    vibes::Params params = params_slices;
    Interval y_i = (*m_tube)[i];
    if(m_tube_copy != NULL && y_i != (*m_tube_copy)[i]) // contraction
      params = params_contracted_slices;
    drawSlice(m_tube->domain(i), y_i, params);
  }

  if(update_background)
  {
    if(m_tube_copy != NULL)
      delete m_tube_copy;
    m_tube_copy = new Tube(*m_tube);
  }

  if(m_tube->image() == Interval::ALL_REALS)
    m_need_to_update_axis = true;
}

void VibesFigure_Tube::drawSlice(const Interval& intv_t, const Interval& intv_y, const vibes::Params& params) const
{
  vibes::drawBox(intv_t.lb(), intv_t.ub(),
                 intv_y.lb(), intv_y.ub(),
                 params);
}

void VibesFigure_Tube::computePolygonEnvelope(const Tube& tube, vector<double>& v_x, vector<double>& v_y) const
{
  for(int i = 0 ; i < tube.size() ; i++)
  {
    IntervalVector sliceBox = tube.sliceBox(i);
    v_x.push_back(sliceBox[0].lb());
    v_x.push_back(sliceBox[0].ub());
    v_y.push_back(sliceBox[1].ub());
    v_y.push_back(sliceBox[1].ub());
  }

  for(int i = tube.size() - 1 ; i >= 0 ; i--)
  {
    IntervalVector sliceBox = tube.sliceBox(i);
    v_x.push_back(sliceBox[0].ub());
    v_x.push_back(sliceBox[0].lb());
    v_y.push_back(sliceBox[1].lb());
    v_y.push_back(sliceBox[1].lb());
  }
}
