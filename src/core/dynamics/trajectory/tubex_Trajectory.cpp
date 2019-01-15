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

using namespace std;
using namespace ibex;

namespace tubex
{
  // Public methods

    // Definition

    Trajectory::Trajectory()
    {
      
    }

    Trajectory::Trajectory(const Trajectory& traj)
    {
      *this = traj;
    }

    Trajectory::Trajectory(const Interval& domain, const tubex::Function& f)
      : m_domain(domain), m_function(new tubex::Function(f))
    {
      assert(valid_domain(domain));
      assert(f.image_dim() == 1);
      assert(f.nb_vars() == 0 && "function's inputs must be limited to system variable");
      // todo: check thickness of f? (only thin functions should be allowed)

      m_codomain = m_function->eval(domain);
    }

    Trajectory::Trajectory(const map<double,double>& map_values)
    {
      typename map<double,double>::const_iterator it_map;
      for(it_map = map_values.begin() ; it_map != map_values.end() ; it_map++)
        set(it_map->second, it_map->first);
    }

    Trajectory::~Trajectory()
    {
      if(m_function != NULL)
        delete m_function;
    }

    const Trajectory& Trajectory::operator=(const Trajectory& x)
    {
      m_domain = x.m_domain;
      m_codomain = x.m_codomain;

      if(m_function != NULL)
        delete m_function;

      if(x.m_function == NULL)
        m_function = NULL;

      else
        m_function = new tubex::Function(*x.m_function);

      m_map_values = x.m_map_values;
      return *this;
    }

    int Trajectory::size() const
    {
      return 1;
    }

    // Accessing values

    const map<double,double>& Trajectory::sampled_map() const
    {
      return m_map_values;
    }

    const tubex::Function* Trajectory::function() const
    {
      return m_function;
    }

    const Interval Trajectory::domain() const
    {
      return m_domain;
    }

    const Interval Trajectory::codomain() const
    {
      return codomain_box()[0];
    }

    double Trajectory::operator()(double t) const
    {
      assert(domain().contains(t));

      if(m_function != NULL)
        return m_function->eval(t).mid(); // /!\ an approximation is made here

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

    const Interval Trajectory::operator()(const Interval& t) const
    {
      assert(domain().is_superset(t));

      if(m_domain == t)
        return m_codomain;

      else if(m_function != NULL)
        return m_function->eval(t);

      else
      {
        Interval eval = Interval::EMPTY_SET;
        eval |= (*this)(t.lb());
        eval |= (*this)(t.ub());

        for(map<double,double>::const_iterator it = m_map_values.lower_bound(t.lb()) ;
            it != m_map_values.upper_bound(t.ub()) ; it++)
          eval |= it->second;

        return eval;
      }
    }
    
    // Tests

    bool Trajectory::not_defined() const
    {
      return m_function == NULL && m_map_values.empty();
    }

    bool Trajectory::operator==(const Trajectory& x) const
    {
      assert(m_function == NULL || x.function() == NULL
        && "operator== not implemented in case of a Trajectory defined by a Function");

      if(m_function == NULL && x.function() == NULL)
      {
        if(m_domain != x.domain() || m_codomain != x.codomain())
          return false;

        typename map<double,double>::const_iterator it_map;
        for(it_map = m_map_values.begin() ; it_map != m_map_values.end() ; it_map++)
        {
          if(x.sampled_map().find(it_map->first) == x.sampled_map().end())
            return false;

          if(it_map->second != x.sampled_map().at(it_map->first))
            return false;
        }

        return true;
      }

      else
        return false;
    }
    
    bool Trajectory::operator!=(const Trajectory& x) const
    {
      assert(m_function == NULL && x.function() == NULL
        && "operator!= not implemented in case of a Trajectory defined by a Function");
      return domain() != x.domain() || codomain() != x.codomain() || !(*this == x);
    }

    // Setting values

    void Trajectory::set(double y, double t)
    {
      assert(m_function == NULL && "Trajectory already defined by a Function");
      m_domain |= t;
      m_codomain |= y;
      m_map_values.erase(t);
      m_map_values.emplace(t, y);
    }

    void Trajectory::truncate_domain(const Interval& t)
    {
      assert(valid_domain(t));
      assert(domain().is_superset(t));

      map<double,double>::iterator it = m_map_values.begin();
      while(it != m_map_values.end())
      {
        if(!t.contains(it->first)) it = m_map_values.erase(it);
        else ++it;
      }

      m_codomain.set_empty();

      for(map<double,double>::iterator it = m_map_values.begin() ; it != m_map_values.end() ; it++)
        m_codomain |= it->second;

      m_domain &= t;
    }

    void Trajectory::shift_domain(double shift_ref)
    {
      map<double,double> map_temp = m_map_values;
      m_map_values.clear();

      for(map<double,double>::iterator it = map_temp.begin() ; it != map_temp.end() ; it++)
        m_map_values.emplace(it->first - shift_ref, it->second);

      m_domain -= shift_ref;
    }
    
    // Integration
    
    const Trajectory Trajectory::integrate() const
    {
      assert(m_function == NULL && "integration timestep requested for trajectories defined by Function");
      
      Trajectory x;
      double prev_x = 0., prev_t;
      x.set(prev_x, domain().lb());

      map<double,double>::const_iterator it = m_map_values.begin();
      it++;
      for( ; it != m_map_values.end() ; it++)
      {
        prev_x += it->second * (it->first - prev_t);
        prev_t = it->first;
        x.set(prev_x, it->first);
      }

      return x;
    }
    
    const Trajectory Trajectory::integrate(double dt) const
    {
      assert(dt > 0.);

      Trajectory x;
      double prev_x = 0.;
      x.set(prev_x, domain().lb());

      for(double t = domain().lb() + dt ; t < domain().ub() ; t+=dt)
      {
        prev_x += (*this)(t)*dt;
        x.set(prev_x, t);
      }

      return x;
    }

    // String
    
    std::ostream& operator<<(std::ostream& str, const Trajectory& x)
    {
      str << "Trajectory " << x.domain() << "↦" << x.codomain();

      if(x.m_function != NULL)
        str << " (Fnc object)";

      else if(!x.m_map_values.empty())
      {
        if(x.m_map_values.size() < 10)
        {
          str << ", " << x.m_map_values.size() << " pts: { ";
          for(map<double,double>::const_iterator it = x.m_map_values.begin() ; it != x.m_map_values.end() ; it++)
            str << "(" << it->first << "," << it->second << ") ";
          str << "} ";
        }

        else
          str << ", " << x.m_map_values.size() << " points";
      }

      str << flush;
      return str;
    }

  // Protected methods

    const IntervalVector Trajectory::codomain_box() const
    {
      return IntervalVector(m_codomain);
    }
}