/** 
 *  Trajectory class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2019 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

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

    const Interval Trajectory::domain() const
    {
      return m_domain;
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
      set_map_value(y, t);
    }

    void Trajectory::truncate_domain(const Interval& t)
    {
      assert(valid_domain(t));
      assert(domain().is_superset(t));

      double y_lb = (*this)(t.lb());
      double y_ub = (*this)(t.ub());

      map<double,double>::iterator it = m_map_values.begin();
      while(it != m_map_values.end())
      {
        if(!t.contains(it->first)) it = m_map_values.erase(it);
        else ++it;
      }

      m_map_values[t.lb()] = y_lb; // clean truncature
      m_map_values[t.ub()] = y_ub;

      compute_codomain();
      m_domain &= t;
    }

    void Trajectory::shift_domain(double shift_ref)
    {
      map<double,double> map_temp = m_map_values;
      m_map_values.clear();

      for(map<double,double>::iterator it = map_temp.begin() ; it != map_temp.end() ; it++)
        m_map_values.emplace(it->first - shift_ref, it->second);

      m_domain -= shift_ref;
      compute_codomain();
    }
    
    void Trajectory::discretize(double dt)
    {
      assert(dt > 0.);
      assert(m_function != NULL && "discretization available for analytic functions only");

      for(double t = m_domain.lb() ; t < m_domain.ub() ; t+=dt)
        set_map_value((*this)(t), t);
      set_map_value((*this)(m_domain.ub()), m_domain.ub());

      delete m_function;
      m_function = NULL;
    }

    // Integration
    
    const Trajectory Trajectory::primitive(double c) const
    {
      assert(m_function == NULL && "integration timestep requested for trajectories defined by Function");
      
      double val;
      Trajectory x;

      for(map<double,double>::const_iterator it = m_map_values.begin() ; it != m_map_values.end() ; it++)
      {
        if(it == m_map_values.begin())
          val = c;

        else
          val += (prev(it)->second + it->second) * (it->first - prev(it)->first) / 2.;

        x.set(val, it->first);
      }

      return x;
    }
    
    const Trajectory Trajectory::primitive(double c, double dt) const
    {
      assert(dt > 0.);

      double t, prev_t, val = c;
      Trajectory x;

      for(t = domain().lb() ; t < domain().ub() ; t+=dt)
      {
        if(t != domain().lb())
          val += ((*this)(t-dt) + (*this)(t)) * dt / 2.;

        x.set(val, t);
        prev_t = t;
      }

      t = domain().ub();
      val += ((*this)(prev_t) + (*this)(t)) * (t - prev_t) / 2.;
      x.set(val, t);

      return x;
    }

    const Trajectory Trajectory::diff() const
    {
      if(m_function != NULL)
        return Trajectory(domain(), m_function->diff());

      else // finite difference computation
      {
        assert(m_map_values.size() > 1);
        Trajectory d;

        for(map<double,double>::const_iterator it = m_map_values.begin() ; it != m_map_values.end() ; it++)
        {
          double t = it->first;
          double x = it->second;
          d.set(finite_diff(t), t);
        }

        assert(d.domain() == domain());
        return d;
      }
    }

    double Trajectory::finite_diff(double t) const
    {
      assert(m_function == NULL);
      assert(m_map_values.find(t) != m_map_values.end()); // key exists
      assert(m_map_values.size() > 2);

      //double prev_t = t;
      double h = next(m_map_values.begin())->first - m_map_values.begin()->first;

      vector<double> fwd;
      map<double,double>::const_iterator it_fwd = m_map_values.find(t);
      double x = it_fwd->second;

      it_fwd++;
      while(fwd.size() < 4 && it_fwd != m_map_values.end())
      {
        //assert(it_fwd->first - prev_t == h && "constant timestep required");
        //prev_t = it_fwd->first;
        fwd.push_back(it_fwd->second);
        it_fwd++;
      }

      //prev_t = t;
      vector<double> bwd;
      map<double,double>::const_iterator it_bwd = m_map_values.find(t);

      if(it_bwd != m_map_values.begin())
      {
        it_bwd--;
        while(bwd.size() < 4)
        {
          //assert(prev_t - it_bwd->first == h && "constant timestep required");
          //prev_t = it_bwd->first;
          bwd.push_back(it_bwd->second);
          if(it_bwd == m_map_values.begin())
            break;
          it_bwd--;
        }
      }

      if(fwd.size() == bwd.size()) // central finite difference
        switch(fwd.size())
        {
          case 1:
            return ((-1./2.)*bwd[0] + (1./2.)*fwd[0]) / h;
          case 2:
            return ((1./12.)*bwd[1] + (-2./3.)*bwd[0] + (2./3.)*fwd[0] + (-1./12.)*fwd[1]) / h;
          case 3:
            return ((-1./60.)*bwd[2] + (3./20.)*bwd[1] + (-3./4.)*bwd[0] + (3./4.)*fwd[0] + (-3./20.)*fwd[1] + (1./60.)*fwd[2]) / h;
          case 4:
            return ((1./280.)*bwd[3] + (-4./105.)*bwd[2] + (1./5.)*bwd[1] + (-4./5.)*bwd[0] + (4./5.)*fwd[0] + (-1./5.)*fwd[1] + (4./105.)*fwd[2] + (-1./280.)*fwd[3]) / h;
        }

      else if(fwd.size() > bwd.size()) // forward finite difference
        switch(fwd.size())
        {
          case 1:
            return ((-1./1.)*x + (1./1.)*fwd[0]) / h;
          case 2:
            return ((-3./2.)*x + (2./1.)*fwd[0] + (-1./2.)*fwd[1]) / h;
          case 3:
            return ((-11./6.)*x + (3./1.)*fwd[0] + (-3./2.)*fwd[1] + (1./3.)*fwd[2]) / h;
          case 4:
            return ((-25./12.)*x + (4./1.)*fwd[0] + (-3./1.)*fwd[1] + (4./3.)*fwd[2] + (-1./4.)*fwd[3]) / h;
        }

      else // backward finite difference
        switch(bwd.size())
        {
          case 1:
            return ((1./1.)*x + (-1./1.)*bwd[0]) / h;
          case 2:
            return ((3./2.)*x + (-2./1.)*bwd[0] + (1./2.)*bwd[1]) / h;
          case 3:
            return ((11./6.)*x + (-3./1.)*bwd[0] + (3./2.)*bwd[1] + (-1./3.)*bwd[2]) / h;
          case 4:
            return ((25./12.)*x + (-4./1.)*bwd[0] + (3./1.)*bwd[1] + (-4./3.)*bwd[2] + (1./4.)*bwd[3]) / h;
        }
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

    void Trajectory::set_map_value(double y, double t)
    {
      m_domain |= t;

      bool update_codomain = m_map_values.find(t) != m_map_values.end() // key already exists
                          && m_codomain.contains(y); // and new value is inside the current enclosure

      m_map_values.erase(t);
      m_map_values.emplace(t, y);

      if(update_codomain) // the new codomain is a subset of the old one
        compute_codomain();

      else
        m_codomain |= y; // simple union
    }

    void Trajectory::compute_codomain()
    {
      m_codomain.set_empty();
      for(map<double,double>::iterator it = m_map_values.begin() ; it != m_map_values.end() ; it++)
        m_codomain |= it->second;
    }
}