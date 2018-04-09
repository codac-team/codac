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
  int Trajectory::nb_traj = 0;
  vector<string> Trajectory::v_traj_names;

  Trajectory::Trajectory(const string& name) : m_function(NULL), m_domain(Interval::EMPTY_SET)
  {
    setName(name);
  }

  Trajectory::Trajectory(const Function& f, const Interval& domain, const string& name) : m_function(new Function(f)), m_domain(domain)
  {
    setName(name);
  }

  Trajectory::Trajectory(const map<double,double>& map_values, const string& name) : m_function(NULL), m_domain(Interval::EMPTY_SET), m_map_values(map_values)
  {
    setName(name);
    typename map<double,double>::const_iterator it_map;
    for(it_map = m_map_values.begin() ; it_map != m_map_values.end() ; it_map++)
      m_domain |= it_map->first;
  }

  Trajectory::~Trajectory()
  {
    if(m_function != NULL)
      delete m_function;
  }

  void Trajectory::setName(const string& name)
  {
    if(name == "")
    {
      Trajectory::nb_traj ++;
      std::ostringstream o;
      o << "x" << std::hex << Trajectory::nb_traj;
      m_name = o.str();
    }

    else
    {
      for(int i = 0 ; i < Trajectory::v_traj_names.size() ; i++)
        if(Trajectory::v_traj_names[i] == name)
          cout << "Trajectory::setName(): warning, trajectory \"" << name << "\" already exists" << endl;
      m_name = name;
    }
    
    Trajectory::v_traj_names.push_back(m_name);
  }

  const string& Trajectory::name() const
  {
    return m_name;
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
    return (*this)[m_domain];
  }

  double& Trajectory::set(double t, double y)
  {
    m_map_values[t] = y;
    m_domain |= t;
    return m_map_values[t];
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
      return m_map_values.at(t);

    else
    {
      typename map<double,double>::const_iterator it_lower, it_upper;
      it_lower = m_map_values.lower_bound(t);
      it_upper = it_lower;
      it_lower--;

      return it_lower->second +
             (t - it_lower->first) * (it_upper->second - it_lower->second) /
             (it_upper->first - it_lower->first);
    }
  }
  
  const Interval Trajectory::operator[](const Interval& t) const
  {
    DomainException::check(*this, t);
    // todo
  }

  void Trajectory::truncateDomain(const Interval& domain)
  {
    map<double,double>::iterator it = m_map_values.begin();
    while(it != m_map_values.end())
    {
      if(!domain.contains(it->first)) it = m_map_values.erase(it);
      else ++it;
    }

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