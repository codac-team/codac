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
  setColors("gray[yellow]");
  m_need_to_update_axis = true;
}

VibesFigure_Tube::~VibesFigure_Tube()
{
  delete m_tube_copy;
}

void VibesFigure_Tube::setColors(string slices_color, string background_color, string truth_color)
{
  vibes::newGroup("slices_old_background", background_color, vibesParams("figure", m_name));
  vibes::newGroup("slices", slices_color, vibesParams("figure", m_name));
  vibes::newGroup("true_values", truth_color, vibesParams("figure", m_name));
}

void VibesFigure_Tube::show() const
{
  return show(m_tube->size());
}

void VibesFigure_Tube::showScalarValues(const map<double,double>& map_scalar_values, const string& color) const
{
  m_id_map_scalar_values ++;
  std::ostringstream o;
  o << "scalar_values_" << std::hex << m_id_map_scalar_values;
  vibes::newGroup(o.str(), color, vibesParams("figure", m_name));

  vector<double> v_x, v_y;
  typename map<double,double>::const_iterator it_scalar_values;
  for(it_scalar_values = map_scalar_values.begin(); it_scalar_values != map_scalar_values.end(); it_scalar_values++)
  {
    v_x.push_back(it_scalar_values->first);
    v_y.push_back(it_scalar_values->second);
  }

  vibes::drawLine(v_x, v_y, vibesParams("figure", m_name, "group", o.str()));
}

void VibesFigure_Tube::show(int slices_limit, bool update_background) const
{
  if(m_need_to_update_axis)
  {
    vibes::axisLimits(m_tube->getT().lb(), m_tube->getT().ub(), 
                      m_tube->getY().lb(), m_tube->getY().ub(),
                      m_name);
  }

  vibes::Params params;

  if(m_tube_copy != NULL && update_background)
  {
    vibes::clearGroup(m_name, "slices_old_background");
    params = vibesParams("figure", m_name, "group", "slices_old_background");
    for(int i = 0 ; i < m_tube_copy->size() ; i++)
      drawSlice(m_tube_copy->getT(i), (*m_tube_copy)[i], params);
  }

  vibes::clearGroup(m_name, "slices");
  params = vibesParams("figure", m_name, "group", "slices");

  if(slices_limit <= 0)
    slices_limit = m_tube->size();

  for(int i = 0 ; i < m_tube->size() ; i += max((int)(m_tube->size() / slices_limit), 1))
    drawSlice(m_tube->getT(i), (*m_tube)[i], params);

  if(update_background)
  {
    if(m_tube_copy != NULL)
      delete m_tube_copy;
    m_tube_copy = new Tube(*m_tube);
  }

  if(m_tube->getY() == Interval::ALL_REALS)
    m_need_to_update_axis = true;
}

void VibesFigure_Tube::drawSlice(const Interval& intv_t, const Interval& intv_y, const vibes::Params& params) const
{
  vibes::drawBox(intv_t.lb(), intv_t.ub(),
                 intv_y.lb(), intv_y.ub(),
                 params);
}
