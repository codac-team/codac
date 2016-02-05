/* ============================================================================
 *  IBEX-Robotics - Tube_VibesFigure class
 * ============================================================================
 *  Copyright : Simon Rohou
 *  License   : This program can be distributed under the terms of
 *              the MIT license. See the file LICENSE.
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "Tube_VibesFigure.h"

using namespace std;

VibesFigure_Tube::VibesFigure_Tube(const string& name, Tube *tube) : VibesFigure(name)
{
  m_tube = tube;
  m_tube_copy = NULL;
  vibes::axisLimits(m_tube->getT().lb(), m_tube->getT().ub(), 
                    m_tube->getY().lb(), m_tube->getY().ub());
  vibes::newGroup("slices_old_background", "lightGray[lightGray]", vibesParams("figure", m_name));
  vibes::newGroup("slices", "gray[yellow]", vibesParams("figure", m_name));
}

VibesFigure_Tube::~VibesFigure_Tube()
{
  delete m_tube_copy;
}

void VibesFigure_Tube::show() const
{
  return show(m_tube->size());
}

void VibesFigure_Tube::show(int slices_limit) const
{
  vibes::Params params;

  if(m_tube_copy != NULL)
  {
    vibes::clearGroup(m_name, "slices_old_background");
    params = vibesParams("figure", m_name, "group", "slices_old_background");
    for(int i = 0 ; i < m_tube_copy->size() ; i++)
      drawSlice(m_tube_copy->getT(i), (*m_tube_copy)[i], params);
  }

  vibes::clearGroup(m_name, "slices");
  params = vibesParams("figure", m_name, "group", "slices");
  for(int i = 0 ; i < m_tube->size() ; i += max((int)(m_tube->size() / slices_limit), 1))
    drawSlice(m_tube->getT(i), (*m_tube)[i], params);

  if(m_tube_copy != NULL)
    delete m_tube_copy;
  m_tube_copy = new Tube(*m_tube);
}

void VibesFigure_Tube::drawSlice(const ibex::Interval& intv_t, const ibex::Interval& intv_y, const vibes::Params& params) const
{
  vibes::drawBox(intv_t.lb(), intv_t.ub(),
                 intv_y.lb(), intv_y.ub(),
                 params);
}