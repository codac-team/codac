/** 
 *  Trajectory class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
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
      : m_traj_def_type(TrajDefnType::MAP_OF_VALUES)
    {
      m_map_values = map<double,double>();
    }

    Trajectory::Trajectory(const Trajectory& traj)
    {
      *this = traj;
    }

    Trajectory::Trajectory(const Interval& tdomain, const TFunction& f)
      : m_tdomain(tdomain), m_traj_def_type(TrajDefnType::ANALYTIC_FNC), m_function(new TFunction(f))
    {
      assert(valid_tdomain(tdomain));
      assert(f.image_dim() == 1);
      assert(f.nb_vars() == 0 && "function's inputs must be limited to system variable");
      // todo: check thickness of f? (only thin functions should be allowed)

      m_codomain = m_function->eval(tdomain);
    }

    Trajectory::Trajectory(const Interval& tdomain, const TFunction& f, double timestep)
      : Trajectory(tdomain, f)
    {
      assert(timestep > 0.);
      sample(timestep);
    }

    Trajectory::Trajectory(const map<double,double>& map_values)
      : m_traj_def_type(TrajDefnType::MAP_OF_VALUES), m_map_values(map_values)
    {
      assert(!map_values.empty());

      // Temporal domain:
      map<double,double>::const_iterator
        last_it = map_values.end(); last_it--; // accessing last value
      m_tdomain = Interval(map_values.begin()->first, last_it->first);

      // Codomain:
      compute_codomain();
    }

    Trajectory::~Trajectory()
    {
      if(m_traj_def_type == TrajDefnType::ANALYTIC_FNC && m_function != NULL)
        delete m_function;
    }

    const Trajectory& Trajectory::operator=(const Trajectory& x)
    {
      m_tdomain = x.m_tdomain;
      m_codomain = x.m_codomain;
      m_traj_def_type = x.m_traj_def_type;

      switch(m_traj_def_type)
      {
        case TrajDefnType::ANALYTIC_FNC:
          delete m_function;
          m_function = new TFunction(*x.m_function);
          break;

        case TrajDefnType::MAP_OF_VALUES:
          m_map_values = x.m_map_values;
          break;

        default:
          assert(false && "unhandled case");
      }

      return *this;
    }

    int Trajectory::size() const
    {
      return 1;
    }

    const Interval Trajectory::tdomain() const
    {
      return m_tdomain;
    }
    
    TrajDefnType Trajectory::definition_type() const
    {
      return m_traj_def_type;
    }

    // Accessing values

    const map<double,double>& Trajectory::sampled_map() const
    {
      assert(m_traj_def_type == TrajDefnType::MAP_OF_VALUES);
      return m_map_values;
    }

    const TFunction* Trajectory::tfunction() const
    {
      assert(m_traj_def_type == TrajDefnType::ANALYTIC_FNC);
      return m_function;
    }

    const Interval Trajectory::codomain() const
    {
      return codomain_box()[0];
    }

    double Trajectory::operator()(double t) const
    {
      assert(tdomain().contains(t));

      switch(m_traj_def_type)
      {
        case TrajDefnType::ANALYTIC_FNC:
          return m_function->eval(t).mid(); // /!\ an approximation is made here

        case TrajDefnType::MAP_OF_VALUES:
          if(m_map_values.find(t) != m_map_values.end()) // key exists
            return m_map_values.at(t);
            // todo: optimize this to avoid double reading of the map?

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

        default:
          assert(false && "unhandled case");
          return 0.;
      }
    }

    const Interval Trajectory::operator()(const Interval& t) const
    {
      assert(tdomain().is_superset(t));

      if(m_tdomain == t)
        return m_codomain;

      Interval eval = Interval::EMPTY_SET;

      switch(m_traj_def_type)
      {
        case TrajDefnType::ANALYTIC_FNC:
          eval = m_function->eval(t);
          break;

        case TrajDefnType::MAP_OF_VALUES:
          eval |= (*this)(t.lb());
          eval |= (*this)(t.ub());

          for(map<double,double>::const_iterator it = m_map_values.lower_bound(t.lb()) ;
              it != m_map_values.upper_bound(t.ub()) ; it++)
            eval |= it->second;
          break;

        default:
          assert(false && "unhandled case");
      }

      return eval;
    }
    
    double Trajectory::first_value() const
    {
      switch(m_traj_def_type)
      {
        case TrajDefnType::ANALYTIC_FNC:
          return m_function->eval(m_tdomain.lb()).mid(); // /!\ an approximation is made here

        case TrajDefnType::MAP_OF_VALUES:
          return m_map_values.begin()->second;

        default:
          assert(false && "unhandled case");
          return 0.;
      }
    }

    double Trajectory::last_value() const
    {
      switch(m_traj_def_type)
      {
        case TrajDefnType::ANALYTIC_FNC:
          return m_function->eval(m_tdomain.ub()).mid(); // /!\ an approximation is made here

        case TrajDefnType::MAP_OF_VALUES:
          return m_map_values.rbegin()->second;

        default:
          assert(false && "unhandled case");
          return 0.;
      }
    }

    // Tests

    bool Trajectory::not_defined() const
    {
      switch(m_traj_def_type)
      {
        case TrajDefnType::ANALYTIC_FNC:
          return m_function == NULL;

        case TrajDefnType::MAP_OF_VALUES:
          return m_map_values.empty();

        default:
          assert(false && "unhandled case");
          return true;
      }
    }

    bool Trajectory::operator==(const Trajectory& x) const
    {
      assert((m_traj_def_type == TrajDefnType::MAP_OF_VALUES || x.m_traj_def_type == TrajDefnType::MAP_OF_VALUES)
        && "operator== not implemented in case of a Trajectory defined by a TFunction");

      if(m_traj_def_type == TrajDefnType::MAP_OF_VALUES && x.m_traj_def_type == TrajDefnType::MAP_OF_VALUES)
      {
        if(m_tdomain != x.tdomain() || m_codomain != x.codomain())
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
      assert((m_traj_def_type == TrajDefnType::MAP_OF_VALUES && x.m_traj_def_type == TrajDefnType::MAP_OF_VALUES)
        && "operator!= not implemented in case of a Trajectory defined by a TFunction");
      return tdomain() != x.tdomain() || codomain() != x.codomain() || !(*this == x);
    }

    // Setting values

    void Trajectory::set(double y, double t)
    {
      assert(m_traj_def_type == TrajDefnType::MAP_OF_VALUES
        && "Trajectory already defined by a TFunction");
      
      m_tdomain |= t;

      bool update_codomain = m_map_values.find(t) != m_map_values.end() // key already exists
            && m_codomain.contains(m_map_values.at(t)); // and new value inside codomain hull

      m_map_values.erase(t);
      m_map_values.emplace(t, y);

      if(update_codomain) // the new codomain may be a subset of the old one
        compute_codomain();

      else
        m_codomain |= y; // simple union
    }

    Trajectory& Trajectory::truncate_tdomain(const Interval& t)
    {
      assert(valid_tdomain(t));
      assert(tdomain().is_superset(t));

      if(m_traj_def_type == TrajDefnType::MAP_OF_VALUES)
      {
        double y_lb = (*this)(t.lb());
        double y_ub = (*this)(t.ub());

        map<double,double>::iterator it = m_map_values.begin();
        while(it != m_map_values.end())
        {
          if(!t.contains(it->first)) it = m_map_values.erase(it);
          else ++it;
        }

        m_map_values[t.lb()] = y_lb; // clean truncation
        m_map_values[t.ub()] = y_ub;
      }

      m_tdomain &= t;
      compute_codomain();
      return *this;
    }
      
    Trajectory& Trajectory::shift_tdomain(double shift_ref)
    {
      if(m_traj_def_type == TrajDefnType::MAP_OF_VALUES)
      {
        map<double,double> map_temp = m_map_values;
        m_map_values.clear();

        for(map<double,double>::iterator it = map_temp.begin() ; it != map_temp.end() ; it++)
          m_map_values.emplace(it->first + shift_ref, it->second);
      }

      m_tdomain += shift_ref;
      compute_codomain();
      return *this;
    }
    
    Trajectory& Trajectory::sample(double dt)
    {
      assert(dt > 0.);

      map<double,double> new_map;
      
      if(m_traj_def_type == TrajDefnType::MAP_OF_VALUES)
        new_map = m_map_values;

      double t;
      for(t = m_tdomain.lb() ; t < m_tdomain.ub() ; t+=dt)
        if(new_map.find(t) == new_map.end()) // if key does not exist already
          new_map[t] = (*this)(t); // evaluation/interpolation
      new_map[m_tdomain.ub()] = (*this)(m_tdomain.ub());

      if(m_traj_def_type == TrajDefnType::ANALYTIC_FNC)
      {
        m_traj_def_type = TrajDefnType::MAP_OF_VALUES;
        delete m_function;
      }

      m_map_values = new_map;
      // Note : no need to update the codomain, it will not be changed by this method.
      return *this;
    }
    
    Trajectory& Trajectory::sample(const Trajectory& x)
    {
      assert(tdomain() == x.tdomain());
      assert(x.m_traj_def_type == TrajDefnType::MAP_OF_VALUES && "trajectory x has to be sampled");
      
      map<double,double> new_map;

      if(m_traj_def_type == TrajDefnType::MAP_OF_VALUES)
        new_map = m_map_values;

      for(auto const& it : x.sampled_map())
        if(new_map.find(it.first) == new_map.end()) // if key does not exist already
          new_map[it.first] = (*this)(it.first); // evaluation/interpolation

      if(m_traj_def_type == TrajDefnType::ANALYTIC_FNC)
      {
        m_traj_def_type = TrajDefnType::MAP_OF_VALUES;
        delete m_function;
      }

      m_map_values = new_map;
      // Note : no need to update the codomain, it will not be changed by this method.
      return *this;
    }
    
    Trajectory& Trajectory::make_continuous()
    {
      assert(m_traj_def_type == TrajDefnType::MAP_OF_VALUES
        && "not usable for trajectories defined by TFunction");

      const Interval periodicity = codomain();
      m_codomain = Interval::EMPTY_SET;

      double prev_value = 0., value_mod = 0.;
      map<double,double> m_continuous_values(m_map_values);

      for(const auto& it : m_map_values)
      {
        if(!m_continuous_values.empty())
        {
          if(prev_value - it.second > periodicity.ub())
            value_mod += periodicity.diam();
          else if(prev_value - it.second < periodicity.lb())
            value_mod -= periodicity.diam();
        }

        prev_value = it.second;
        m_continuous_values[it.first] += value_mod;
        m_codomain |= m_continuous_values[it.first];
      }

      m_map_values = m_continuous_values;
      return *this;
    }

    // Integration
    
    const Trajectory Trajectory::primitive(double c) const
    {
      assert(m_traj_def_type == TrajDefnType::MAP_OF_VALUES
        && "integration timestep requested for trajectories defined by TFunction");
      
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

      double t = tdomain().lb(), prev_t = t, val = c;
      Trajectory x;

      while(t < tdomain().ub())
      {
        if(t != tdomain().lb())
          val += ((*this)(t-dt) + (*this)(t)) * dt / 2.;

        x.set(val, t);
        prev_t = t;
        t += dt;
      }

      t = tdomain().ub();
      val += ((*this)(prev_t) + (*this)(t)) * (t - prev_t) / 2.;
      x.set(val, t);

      return x;
    }

    const Trajectory Trajectory::diff() const
    {
      Trajectory d;

      switch(m_traj_def_type)
      {
        case TrajDefnType::ANALYTIC_FNC:
          d = Trajectory(tdomain(), m_function->diff());
          break;

        case TrajDefnType::MAP_OF_VALUES: // finite difference computation
          assert(m_map_values.size() > 1);
          
          for(map<double,double>::const_iterator it = m_map_values.begin() ; it != m_map_values.end() ; it++)
            d.set(finite_diff(it->first), it->first);

          assert(d.tdomain() == tdomain());
          break;

        default:
          assert(false && "unhandled case");
      }

      return d;
    }

    double Trajectory::finite_diff(double t) const
    {
      assert(m_traj_def_type == TrajDefnType::MAP_OF_VALUES);
      assert(m_map_values.find(t) != m_map_values.end()); // key exists
      assert(m_map_values.size() > 2);

      double h = next(m_map_values.begin())->first - m_map_values.begin()->first;

      vector<double> fwd;
      map<double,double>::const_iterator it_fwd = m_map_values.find(t);
      double x = it_fwd->second;

      it_fwd++;
      while(fwd.size() < 4 && it_fwd != m_map_values.end())
      {
        fwd.push_back(it_fwd->second);
        it_fwd++;
      }

      vector<double> bwd;
      map<double,double>::const_iterator it_bwd = m_map_values.find(t);

      if(it_bwd != m_map_values.begin())
      {
        it_bwd--;
        while(bwd.size() < 4)
        {
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
          default:
            assert(false && "unhandled case");
            return 0.;
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
          default:
            assert(false && "unhandled case");
            return 0.;
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
          default:
            assert(false && "unhandled case");
            return 0.;
        }
    }

    // String
    
    std::ostream& operator<<(std::ostream& str, const Trajectory& x)
    {
      str << "Trajectory " << x.tdomain() << "↦" << x.codomain();

      switch(x.m_traj_def_type)
      {
        case TrajDefnType::ANALYTIC_FNC:
          str << " (Fnc object)";
          break;

        case TrajDefnType::MAP_OF_VALUES:
          if(x.m_map_values.size() < 10)
          {
            str << ", " << x.m_map_values.size() << " pts: { ";
            for(map<double,double>::const_iterator it = x.m_map_values.begin() ; it != x.m_map_values.end() ; it++)
              str << "(" << it->first << "," << it->second << ") ";
            str << "} ";
          }

          else
            str << ", " << x.m_map_values.size() << " points";

          break;

        default:
          str << " (def ERROR)";
          break;
      }

      str << flush;
      return str;
    }

  // Protected methods

    const IntervalVector Trajectory::codomain_box() const
    {
      return IntervalVector(m_codomain);
    }

    void Trajectory::compute_codomain()
    {
      switch(m_traj_def_type)
      {
        case TrajDefnType::ANALYTIC_FNC:
          m_codomain = m_function->eval(m_tdomain);
          break;

        case TrajDefnType::MAP_OF_VALUES:
          m_codomain = Interval::EMPTY_SET;
          for(map<double,double>::iterator it = m_map_values.begin() ; it != m_map_values.end() ; it++)
            m_codomain |= it->second;
          break;

        default:
          assert(false && "unhandled case");
      }
    }
}