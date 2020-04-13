/** 
 *  Domain class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <boost/algorithm/string/replace.hpp>
#include "tubex_Domain.h"
#include "tubex_Figure.h" // for add_suffix

using namespace std;
using namespace ibex;

namespace tubex
{
  Domain::Domain(const Domain& ad)
    : m_type(ad.m_type), m_extern_object_type(ad.m_extern_object_type)
  {
    m_volume = ad.m_volume;
    m_v_ctc = ad.m_v_ctc;

    // todo: verify the copy of the above pointers
    // todo: is this constructor useful?

    switch(ad.m_type)
    {
      case Type::INTERVAL:
        if(ad.m_i_ptr != NULL)
        {
          m_i_ptr = new Interval(*ad.m_i_ptr);
          m_ref_values_i = reference_wrapper<Interval>(*m_i_ptr);
        }

        else
        {
          m_ref_values_i = reference_wrapper<Interval>(ad.m_ref_values_i);
        }
        break;

      case Type::INTERVAL_VECTOR:
        if(ad.m_iv_ptr != NULL)
        {
          m_iv_ptr = new IntervalVector(*ad.m_iv_ptr);
          m_ref_values_iv = reference_wrapper<IntervalVector>(*m_iv_ptr);
        }

        else
        {
          m_ref_values_iv = reference_wrapper<IntervalVector>(ad.m_ref_values_iv);
        }
        break;

      case Type::SLICE:
        m_ref_values_s = ad.m_ref_values_s;
        break;

      case Type::TUBE:
        m_ref_values_t = ad.m_ref_values_t;
        break;

      case Type::TUBE_VECTOR:
        m_ref_values_tv = ad.m_ref_values_tv;
        break;

      default:
        assert(false && "unhandled case");
    }
    
    switch(ad.m_extern_object_type)
    {
      case ExternalRef::DOUBLE:
        m_ref_extern_object_d = ad.m_ref_extern_object_d;
        break;

      case ExternalRef::INTERVAL:
        m_ref_extern_object_i = ad.m_ref_extern_object_i;
        break;

      case ExternalRef::VECTOR:
        m_ref_extern_object_v = ad.m_ref_extern_object_v;
        break;

      case ExternalRef::INTERVAL_VECTOR:
        m_ref_extern_object_iv = ad.m_ref_extern_object_iv;
        break;

      case ExternalRef::SLICE:
        m_ref_extern_object_s = ad.m_ref_extern_object_s;
        break;

      case ExternalRef::TUBE:
        m_ref_extern_object_t = ad.m_ref_extern_object_t;
        break;

      case ExternalRef::TUBE_VECTOR:
        m_ref_extern_object_tv = ad.m_ref_extern_object_tv;
        break;

      default:
        assert(false && "unhandled case");
    }
  }

  Domain::Domain(double& d)
    : m_type(Type::INTERVAL), m_extern_object_type(ExternalRef::DOUBLE)
  {
    m_i_ptr = new Interval(d);
    m_ref_values_i = reference_wrapper<Interval>(*m_i_ptr);
    m_ref_extern_object_d = reference_wrapper<double>(d);
  }

  Domain::Domain(Interval& i)
    : m_type(Type::INTERVAL), m_extern_object_type(ExternalRef::INTERVAL)
  {
    m_i_ptr = NULL;
    m_ref_values_i = reference_wrapper<Interval>(i);
    m_ref_extern_object_i = reference_wrapper<Interval>(i);
  }

  Domain::Domain(const Interval& i)
    : m_type(Type::INTERVAL), m_extern_object_type(ExternalRef::NONE)
  {
    m_i_ptr = new Interval(i);
    m_ref_values_i = reference_wrapper<Interval>(*m_i_ptr);
  }

  Domain::Domain(Vector& v)
    : m_type(Type::INTERVAL_VECTOR), m_extern_object_type(ExternalRef::VECTOR)
  {
    m_iv_ptr = new IntervalVector(v);
    m_ref_values_iv = reference_wrapper<IntervalVector>(*m_iv_ptr);
    m_ref_extern_object_v = reference_wrapper<Vector>(v);
  }

  Domain::Domain(IntervalVector& iv)
    : m_type(Type::INTERVAL_VECTOR), m_extern_object_type(ExternalRef::INTERVAL_VECTOR)
  {
    m_iv_ptr = NULL;
    m_ref_values_iv = reference_wrapper<IntervalVector>(iv);
    m_ref_extern_object_iv = reference_wrapper<IntervalVector>(iv);
  }

  Domain::Domain(const IntervalVector& iv)
    : m_type(Type::INTERVAL_VECTOR), m_extern_object_type(ExternalRef::NONE)
  {
    m_iv_ptr = new IntervalVector(iv);
    m_ref_values_iv = reference_wrapper<IntervalVector>(*m_iv_ptr);
  }

  Domain::Domain(Slice& s)
    : m_type(Type::SLICE), m_extern_object_type(ExternalRef::SLICE)
  {
    m_ref_values_s = reference_wrapper<Slice>(s);
    m_ref_extern_object_s = reference_wrapper<Slice>(s);

    // todo: remove this (unbounded domains not supported for some contractors)
    s &= Slice(s.domain(),Interval(-99999.,99999.)); 
  }

  Domain::Domain(Tube& t)
    : m_type(Type::TUBE), m_extern_object_type(ExternalRef::TUBE)
  {
    m_ref_values_t = reference_wrapper<Tube>(t);
    m_ref_extern_object_t = reference_wrapper<Tube>(t);

    // todo: remove this (unbounded domains not supported for some contractors)
    t &= Interval(-99999.,99999.);
  }

  Domain::Domain(const Tube& t)
    : m_type(Type::TUBE), m_extern_object_type(ExternalRef::NONE)
  {
    m_t_ptr = new Tube(t);
    m_ref_values_t = reference_wrapper<Tube>(*m_t_ptr);

    // todo: remove this (unbounded domains not supported for some contractors)
    *m_t_ptr &= Interval(-99999.,99999.);
  }

  Domain::Domain(TubeVector& tv)
    : m_type(Type::TUBE_VECTOR), m_extern_object_type(ExternalRef::TUBE_VECTOR)
  {
    m_ref_values_tv = reference_wrapper<TubeVector>(tv);
    m_ref_extern_object_tv = reference_wrapper<TubeVector>(tv);

    // todo: remove this (unbounded domains not supported for some contractors)
    tv &= IntervalVector(tv.size(), Interval(-99999.,99999.));
  }

  Domain::Domain(const TubeVector& tv)
    : m_type(Type::TUBE_VECTOR), m_extern_object_type(ExternalRef::NONE)
  {
    m_tv_ptr = new TubeVector(tv);
    m_ref_values_tv = reference_wrapper<TubeVector>(*m_tv_ptr);

    // todo: remove this (unbounded domains not supported for some contractors)
    *m_tv_ptr &= IntervalVector(tv.size(), Interval(-99999.,99999.));
  }

  Domain::~Domain()
  {
    if(m_i_ptr != NULL) delete m_i_ptr;
    if(m_iv_ptr != NULL) delete m_iv_ptr;
  }

  Domain::Type Domain::type() const
  {
    return m_type;
  }

  Interval& Domain::interval()
  {
    assert(m_type == Type::INTERVAL);
    return const_cast<Interval&>(static_cast<const Domain&>(*this).interval());
  }

  const Interval& Domain::interval() const
  {
    assert(m_type == Type::INTERVAL);

    if(m_extern_object_type == ExternalRef::NONE)
      return *m_i_ptr;

    else
    {
      assert(m_extern_object_type == ExternalRef::DOUBLE || m_extern_object_type == ExternalRef::INTERVAL);
      return m_ref_values_i.get();
    }
  }

  IntervalVector& Domain::interval_vector()
  {
    assert(m_type == Type::INTERVAL_VECTOR);
    return const_cast<IntervalVector&>(static_cast<const Domain&>(*this).interval_vector());
  }

  const IntervalVector& Domain::interval_vector() const
  {
    assert(m_type == Type::INTERVAL_VECTOR);

    if(m_extern_object_type == ExternalRef::NONE)
      return *m_iv_ptr;

    else
    {
      assert(m_extern_object_type == ExternalRef::VECTOR || m_extern_object_type == ExternalRef::INTERVAL_VECTOR);
      return m_ref_values_iv.get();
    }
  }

  Slice& Domain::slice()
  {
    assert(m_type == Type::SLICE);
    return const_cast<Slice&>(static_cast<const Domain&>(*this).slice());
  }

  const Slice& Domain::slice() const
  {
    assert(m_type == Type::SLICE);
    return m_ref_values_s.get();
  }

  Tube& Domain::tube()
  {
    assert(m_type == Type::TUBE);
    return const_cast<Tube&>(static_cast<const Domain&>(*this).tube());
  }

  const Tube& Domain::tube() const
  {
    assert(m_type == Type::TUBE);
    return m_ref_values_t.get();
  }

  TubeVector& Domain::tube_vector()
  {
    assert(m_type == Type::TUBE_VECTOR);
    return const_cast<TubeVector&>(static_cast<const Domain&>(*this).tube_vector());
  }

  const TubeVector& Domain::tube_vector() const
  {
    assert(m_type == Type::TUBE_VECTOR);
    return m_ref_values_tv.get();
  }

  vector<Contractor*>& Domain::contractors()
  {
    return m_v_ctc;
  }

  double Domain::compute_volume() const
  {
    switch(m_type)
    {
      case Type::INTERVAL:
        if(interval().is_empty())
          return 0.;
        
        else if(interval().is_unbounded())
          return 999999.; // todo: manager the unbounded case for fixed point detection
        
        else
          return interval().diam();

      case Type::INTERVAL_VECTOR:
        return interval_vector().volume();

      case Type::SLICE:
      {
        double vol = slice().volume();

        // todo: clean the following:

        if(slice().input_gate().is_empty())
          vol += 0.;
        else if(slice().input_gate().is_unbounded())
          vol += 999999.; // todo: manager the unbounded case for fixed point detection
        else
          vol += slice().input_gate().diam();

        if(slice().output_gate().is_empty())
          vol += 0.;
        else if(slice().output_gate().is_unbounded())
          vol += 999999.; // todo: manager the unbounded case for fixed point detection
        else
          vol += slice().output_gate().diam();

        return vol;
      }

      case Type::TUBE:
      {
        double vol = tube().volume();
        vol += tube().first_slice()->input_gate().diam();
        for(const Slice *s = tube().first_slice() ; s != NULL ; s = s->next_slice())
          vol += s->output_gate().diam();
        return vol;
      }

      case Type::TUBE_VECTOR:
      {
        // todo: improve this
        double vol = 0.;

        for(int i = 0 ; i < tube_vector().size() ; i++)
        {
          vol += tube_vector()[i].volume();
          vol += tube_vector()[i].first_slice()->input_gate().diam();
          for(const Slice *s = tube_vector()[i].first_slice() ; s != NULL ; s = s->next_slice())
            vol += s->output_gate().diam();
        }
        
        return vol;
      }

      default:
        assert(false && "unhandled case");
    }

    return 0.;
  }

  double Domain::get_saved_volume() const
  {
    return m_volume;
  }

  void Domain::set_volume(double vol)
  {
    m_volume = vol;
  }

  bool Domain::is_empty() const
  {
    switch(m_type)
    {
      case Type::INTERVAL:
        return interval().is_empty();

      case Type::INTERVAL_VECTOR:
        return interval_vector().is_empty();

      case Type::SLICE:
        return slice().is_empty();

      case Type::TUBE:
        return tube().is_empty();

      case Type::TUBE_VECTOR:
        return tube_vector().is_empty();

      default:
        assert(false && "unhandled case");
    }

    return false;
  }
  
  bool Domain::operator==(const Domain& x) const
  {
    if(m_extern_object_type != x.m_extern_object_type)
      return false;

    assert(m_type == x.m_type);

    switch(m_extern_object_type)
    {
      case ExternalRef::NONE:
        return false; // references cannot be compared
        
      case ExternalRef::DOUBLE:
        return &m_ref_extern_object_d.get() == &x.m_ref_extern_object_d.get();

      case ExternalRef::INTERVAL:
        return &m_ref_extern_object_i.get() == &x.m_ref_extern_object_i.get();

      case ExternalRef::VECTOR:
        return &m_ref_extern_object_v.get() == &x.m_ref_extern_object_v.get();

      case ExternalRef::INTERVAL_VECTOR:
        return &m_ref_extern_object_iv.get() == &x.m_ref_extern_object_iv.get();

      case ExternalRef::SLICE:
        return &m_ref_extern_object_s.get() == &x.m_ref_extern_object_s.get();

      case ExternalRef::TUBE:
        return &m_ref_extern_object_t.get() == &x.m_ref_extern_object_t.get();

      case ExternalRef::TUBE_VECTOR:
        return &m_ref_extern_object_tv.get() == &x.m_ref_extern_object_tv.get();

      default:
        assert(false && "unhandled case");
        return false;
    }
  }
  
  bool Domain::operator!=(const Domain& x) const
  {
    if(m_extern_object_type != x.m_extern_object_type)
      return true;

    assert(m_type == x.m_type);

    switch(m_extern_object_type)
    {
      case ExternalRef::NONE:
        return true; // references cannot be compared
        
      case ExternalRef::DOUBLE:
        return &m_ref_extern_object_d.get() != &x.m_ref_extern_object_d.get();

      case ExternalRef::INTERVAL:
        return &m_ref_extern_object_i.get() != &x.m_ref_extern_object_i.get();

      case ExternalRef::VECTOR:
        return &m_ref_extern_object_v.get() != &x.m_ref_extern_object_v.get();

      case ExternalRef::INTERVAL_VECTOR:
        return &m_ref_extern_object_iv.get() != &x.m_ref_extern_object_iv.get();

      case ExternalRef::SLICE:
        return &m_ref_extern_object_s.get() != &x.m_ref_extern_object_s.get();

      case ExternalRef::TUBE:
        return &m_ref_extern_object_t.get() != &x.m_ref_extern_object_t.get();

      case ExternalRef::TUBE_VECTOR:
        return &m_ref_extern_object_tv.get() != &x.m_ref_extern_object_tv.get();

      default:
        assert(false && "unhandled case");
        return false;
    }
  }
  
  bool Domain::is_component_of(const Domain& x) const
  {
    int id;
    return is_component_of(x, id);
  }

  bool Domain::is_component_of(const Domain& x, int& component_id) const
  {
    if((m_type == Type::INTERVAL && x.type() == Type::INTERVAL_VECTOR) || (m_type == Type::TUBE && x.type() == Type::TUBE_VECTOR))
    {
      switch(x.type())
      {
        case Type::INTERVAL_VECTOR:
          for(int i = 0 ; i < x.m_ref_extern_object_iv.get().size() ; i++)
            if(&x.m_ref_extern_object_iv.get()[i] == &m_ref_extern_object_i.get())
            {
              component_id = i;
              return true;
            }
          break;

        default:
          return false;
      }
    }

    return false;
  }

  ostream& operator<<(ostream& str, const Domain& x)
  {
    switch(x.m_type)
    {
      case Domain::Type::INTERVAL:
        str << "Interval: " << x.interval() << flush;
        break;

      case Domain::Type::INTERVAL_VECTOR:
        str << "IntervalVector: " << x.interval_vector() << flush;
        break;

      case Domain::Type::SLICE:
        str << "Slice: " << x.slice() << flush;
        break;

      case Domain::Type::TUBE:
        str << "Tube: " << x.tube() << flush;
        break;

      case Domain::Type::TUBE_VECTOR:
        str << "TubeVector: " << x.tube_vector() << flush;
        break;

      default:
        assert(false && "unhandled case");
        break;
    }

    return str;
  }
  
  void Domain::add_data(double t, const Interval& y, ContractorNetwork& cn)
  {
    assert(m_type == Type::TUBE);

    m_map_data_s_lb.emplace(t, y.lb());
    m_map_data_s_ub.emplace(t, y.ub());
    Trajectory traj_lb(m_map_data_s_lb);
    Trajectory traj_ub(m_map_data_s_ub);

    Slice *prev_s = NULL;
    if(tube().domain().contains(t))
      prev_s = tube().slice(t)->prev_slice();
    else if(t > tube().domain().ub())
      prev_s = tube().last_slice();

    while(prev_s != NULL && prev_s->domain().is_subset(traj_lb.domain()))
    {
      Interval new_slice_envelope = (traj_lb(prev_s->domain()) | traj_ub(prev_s->domain()));

      if(!prev_s->codomain().is_superset(new_slice_envelope))
        break;

      prev_s->set_envelope(new_slice_envelope);

      // Flags a new change on the domain
      cn.propagate_ctc_from_domain(cn.add_domain(new Domain(*prev_s)));

      prev_s = prev_s->prev_slice();
    }
  }
  
  void Domain::add_data(double t, const IntervalVector& y, ContractorNetwork& cn)
  {
    assert(m_type == Type::TUBE_VECTOR);
    assert(tube_vector().size() == y.size());

    m_map_data_lb.emplace(t, y.lb());
    m_map_data_ub.emplace(t, y.ub());
    TrajectoryVector traj_lb(m_map_data_lb);
    TrajectoryVector traj_ub(m_map_data_ub);

    for(int i = 0 ; i < y.size() ; i++)
    {
      Slice *prev_s = NULL;
      if(tube_vector()[i].domain().contains(t))
        prev_s = tube_vector()[i].slice(t)->prev_slice();
      else if(t > tube_vector()[i].domain().ub())
        prev_s = tube_vector()[i].last_slice();

      while(prev_s != NULL && prev_s->domain().is_subset(traj_lb.domain()))
      {
        Interval new_slice_envelope = (traj_lb[i](prev_s->domain()) | traj_ub[i](prev_s->domain()));

        if(!prev_s->codomain().is_superset(new_slice_envelope))
          break;

        prev_s->set_envelope(new_slice_envelope);

        // Flags a new change on the domain
        cn.propagate_ctc_from_domain(cn.add_domain(new Domain(*prev_s)));

        prev_s = prev_s->prev_slice();
      }
    }
  }
  
  const string Domain::name(const vector<Domain*>& v_domains)
  {
    string output_name = m_name;

    if(output_name.empty()) // looking for dependencies
    {
      for(const auto& dom : v_domains)
      {
        int component_id = 0;
        if(is_component_of(*dom, component_id))
        {
          output_name = dom->name(v_domains); // parent name
          boost::algorithm::replace_all(output_name, "\\mathbf", ""); // removing bold font
          output_name = Figure::add_suffix(output_name, component_id+1); // adding component id
        }
      }
    }

    if(m_type == Type::INTERVAL_VECTOR || m_type == Type::TUBE_VECTOR)
      output_name = "\\mathbf{" + output_name + "}";

    if(m_type == Type::TUBE || m_type == Type::TUBE_VECTOR)
      output_name += "(\\cdot)";

    return output_name;
  }
  
  void Domain::set_name(const string& name)
  {
    m_name = name;
  }
}