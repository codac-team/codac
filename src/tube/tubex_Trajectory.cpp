/* ============================================================================
 *  tubex-lib - Trajectory class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2018
 * ---------------------------------------------------------------------------- */

#include <sstream>
#include "tubex_Trajectory.h"
#include "tubex_DomainException.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  Trajectory::Trajectory()
  {

  }

  Trajectory::Trajectory(const Function& f, const Interval& domain) : m_function(new Function(f)), m_domain(domain)
  {

  }

  Trajectory::Trajectory(const map<double,double>& map_values) : m_map_values(map_values)
  {
    typename map<double,double>::const_iterator it_map;
    for(it_map = m_map_values.begin() ; it_map != m_map_values.end() ; it_map++)
      set(it_map->first, it_map->second);
  }

  Trajectory::~Trajectory()
  {
    if(m_function != NULL)
      delete m_function;
  }

  const map<double,double> Trajectory::getMap() const
  {
    return m_map_values;
  }

  const Function* Trajectory::getFunction() const
  {
    return m_function;
  }

  const Interval Trajectory::domain() const
  {
    return m_domain;
  }

  const Interval Trajectory::codomain() const
  {
    return m_codomain;
  }

  const double Trajectory::operator[](double t) const
  {
    DomainException::check(*this, t);

    if(m_function != NULL)
    {
      IntervalVector box(1, Interval(t));
      return m_function->eval(box).mid();
    }

    else if(m_map_values.find(t) != m_map_values.end())
      return m_map_values.at(t); // key exists

    else
    {
      typename map<double,double>::const_iterator it_lower, it_upper;
      it_lower = m_map_values.lower_bound(t);
      it_upper = it_lower;
      it_lower--;

      // Linear interpolation
      return it_lower->second +
             (t - it_lower->first) * (it_upper->second - it_lower->second) /
             (it_upper->first - it_lower->first);
    }
  }
  
  const Interval Trajectory::operator[](const Interval& t) const
  {
    DomainException::check(*this, t);

    if(m_function != NULL)
    {
      IntervalVector box(1, Interval(t));
      return m_function->eval(box);
    }

    else
    {
      Interval eval(Interval::EMPTY_SET);
      eval |= (*this)[t.lb()];
      eval |= (*this)[t.ub()];

      for(map<double,double>::const_iterator it = m_map_values.lower_bound(t.lb()) ;
          it != m_map_values.upper_bound(t.ub()) ; it++)
        eval |= it->second;
    }
  }

  double& Trajectory::set(double t, double y)
  {
    m_map_values[t] = y;
    m_domain |= t;
    m_codomain |= y;
    return m_map_values[t];
  }

  void Trajectory::truncateDomain(const Interval& domain)
  {
    map<double,double>::iterator it = m_map_values.begin();
    while(it != m_map_values.end())
    {
      if(!domain.contains(it->first)) it = m_map_values.erase(it);
      else ++it;
    }

    m_codomain.set_empty();
    for(map<double,double>::iterator it = m_map_values.begin() ; it != m_map_values.end() ; it++)
      m_codomain |= it->second;

    m_domain &= domain;
  }

  void Trajectory::shiftDomain(double shift_ref)
  {
    map<double,double> map_temp = m_map_values;
    m_map_values.clear();

    for(map<double,double>::iterator it = map_temp.begin() ; it != map_temp.end() ; it++)
      m_map_values[it->first - shift_ref] = it->second;

    m_domain -= shift_ref;
  }
}