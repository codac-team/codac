/** 
 *  Domain class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "tubex_Domain.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  Domain::Domain(const Domain& ad)
    : m_type(ad.m_type), m_v_ctc(ad.m_v_ctc), m_volume(ad.m_volume)
  {
    switch(ad.m_type)
    {
      case DomainType::INTERVAL:
        m_i = ad.m_i;
        break;

      case DomainType::INTERVAL_VECTOR:
        m_iv = ad.m_iv;
        break;

      case DomainType::SLICE:
        m_s = ad.m_s;
        break;

      case DomainType::TUBE:
        m_t = ad.m_t;
        break;

      case DomainType::TUBE_VECTOR:
        m_tv = ad.m_tv;
        break;

      default:
        assert(false && "unhandled case");
    }
  }

  Domain::Domain(double d)
    : m_type(DomainType::INTERVAL), m_i(*new Interval(d)) // todo: manage delete
  {

  }

  Domain::Domain(ibex::Interval& i)
    : m_type(DomainType::INTERVAL), m_i(i)
  {

  }

  Domain::Domain(ibex::Vector& v)
    : m_type(DomainType::INTERVAL_VECTOR), m_iv(*new IntervalVector(v)) // todo: manage delete
  {

  }

  Domain::Domain(ibex::IntervalVector& iv)
    : m_type(DomainType::INTERVAL_VECTOR), m_iv(iv)
  {

  }

  Domain::Domain(tubex::Slice& s)
    : m_type(DomainType::SLICE), m_s(s)
  {
    m_s.get() &= Slice(m_s.get().domain(),Interval(-99999.,99999.)); // todo: remove this
  }

  Domain::Domain(tubex::Tube& t)
    : m_type(DomainType::TUBE), m_t(t)
  {
    m_t.get() &= Interval(-99999.,99999.); // todo: remove this
  }

  Domain::Domain(tubex::TubeVector& tv)
    : m_type(DomainType::TUBE_VECTOR), m_tv(tv)
  {
    m_tv.get() &= IntervalVector(m_tv.get().size(), Interval(-99999.,99999.)); // todo: remove this
  }

  Domain::~Domain()
  {
    
  }

  DomainType Domain::type() const
  {
    return m_type;
  }

  ibex::Interval& Domain::interval()
  {
    assert(m_type == DomainType::INTERVAL);
    return const_cast<Interval&>(static_cast<const Domain&>(*this).interval());
  }

  const ibex::Interval& Domain::interval() const
  {
    assert(m_type == DomainType::INTERVAL);
    return m_i.get();
  }

  ibex::IntervalVector& Domain::interval_vector()
  {
    assert(m_type == DomainType::INTERVAL_VECTOR);
    return const_cast<IntervalVector&>(static_cast<const Domain&>(*this).interval_vector());
  }

  const ibex::IntervalVector& Domain::interval_vector() const
  {
    assert(m_type == DomainType::INTERVAL_VECTOR);
    return m_iv.get();
  }

  tubex::Slice& Domain::slice()
  {
    assert(m_type == DomainType::SLICE);
    return const_cast<Slice&>(static_cast<const Domain&>(*this).slice());
  }

  const tubex::Slice& Domain::slice() const
  {
    assert(m_type == DomainType::SLICE);
    return m_s.get();
  }

  tubex::Tube& Domain::tube()
  {
    assert(m_type == DomainType::TUBE);
    return const_cast<Tube&>(static_cast<const Domain&>(*this).tube());
  }

  const tubex::Tube& Domain::tube() const
  {
    assert(m_type == DomainType::TUBE);
    return m_t.get();
  }

  tubex::TubeVector& Domain::tube_vector()
  {
    assert(m_type == DomainType::TUBE_VECTOR);
    return const_cast<TubeVector&>(static_cast<const Domain&>(*this).tube_vector());
  }

  const tubex::TubeVector& Domain::tube_vector() const
  {
    assert(m_type == DomainType::TUBE_VECTOR);
    return m_tv.get();
  }

  vector<Contractor*>& Domain::contractors()
  {
    return m_v_ctc;
  }

  double Domain::compute_volume() const
  {
    switch(m_type)
    {
      case DomainType::INTERVAL:
        if(m_i.get().is_empty())
          return 0.;
        
        else if(m_i.get().is_unbounded())
          return 999999.; // todo: manager the unbounded case for fixed point detection
        
        else
          return m_i.get().diam();

      case DomainType::INTERVAL_VECTOR:
        return m_iv.get().volume();

      case DomainType::SLICE:
      {
        double vol = m_s.get().volume();

        // todo: clean the following:

        if(m_s.get().input_gate().is_empty())
          vol += 0.;
        else if(m_s.get().input_gate().is_unbounded())
          vol += 999999.; // todo: manager the unbounded case for fixed point detection
        else
          vol += m_s.get().input_gate().diam();

        if(m_s.get().output_gate().is_empty())
          vol += 0.;
        else if(m_s.get().output_gate().is_unbounded())
          vol += 999999.; // todo: manager the unbounded case for fixed point detection
        else
          vol += m_s.get().output_gate().diam();

        return vol;
      }

      case DomainType::TUBE:
      {
        double vol = m_t.get().volume();
        vol += m_t.get().first_slice()->input_gate().diam();
        for(const Slice *s = m_t.get().first_slice() ; s != NULL ; s = s->next_slice())
          vol += s->output_gate().diam();
        return vol;
      }

      case DomainType::TUBE_VECTOR:
      {
        // todo: improve this
        double vol = 0.;

        for(int i = 0 ; i < m_tv.get().size() ; i++)
        {
          vol += m_tv.get()[i].volume();
          vol += m_tv.get()[i].first_slice()->input_gate().diam();
          for(const Slice *s = m_tv.get()[i].first_slice() ; s != NULL ; s = s->next_slice())
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
      case DomainType::INTERVAL:
        return m_i.get().is_empty();

      case DomainType::INTERVAL_VECTOR:
        return m_iv.get().is_empty();

      case DomainType::SLICE:
        return m_s.get().is_empty();

      case DomainType::TUBE:
        return m_t.get().is_empty();

      case DomainType::TUBE_VECTOR:
        return m_tv.get().is_empty();

      default:
        assert(false && "unhandled case");
    }

    return false;
  }
  
  bool Domain::operator==(const Domain& x) const
  {
    if(m_type != x.m_type)
      return false;

    switch(m_type)
    {
      case DomainType::INTERVAL:
        return &m_i.get() == &x.m_i.get();

      case DomainType::INTERVAL_VECTOR:
        return &m_iv.get() == &x.m_iv.get();

      case DomainType::SLICE:
        return &m_s.get() == &x.m_s.get();

      case DomainType::TUBE:
        return &m_t.get() == &x.m_t.get();

      case DomainType::TUBE_VECTOR:
        return &m_tv.get() == &x.m_tv.get();

      default:
        assert(false && "unhandled case");
        return false;
    }
  }
  
  bool Domain::operator!=(const Domain& x) const
  {
    if(m_type != x.m_type)
      return true;

    switch(m_type)
    {
      case DomainType::INTERVAL:
        return &m_i.get() != &x.m_i.get();

      case DomainType::INTERVAL_VECTOR:
        return &m_iv.get() != &x.m_iv.get();

      case DomainType::SLICE:
        return &m_s.get() != &x.m_s.get();

      case DomainType::TUBE:
        return &m_t.get() != &x.m_t.get();

      case DomainType::TUBE_VECTOR:
        return &m_tv.get() != &x.m_tv.get();

      default:
        assert(false && "unhandled case");
        return false;
    }
  }

  ostream& operator<<(ostream& str, const Domain& x)
  {
    switch(x.m_type)
    {
      case DomainType::INTERVAL:
        str << "Interval: " << x.m_i.get() << flush;
        break;

      case DomainType::INTERVAL_VECTOR:
        str << "IntervalVector: " << x.m_iv.get() << flush;
        break;

      case DomainType::SLICE:
        str << "Slice: " << x.m_s.get() << flush;
        break;

      case DomainType::TUBE:
        str << "Tube: " << x.m_t.get() << flush;
        break;

      case DomainType::TUBE_VECTOR:
        str << "TubeVector: " << x.m_tv.get() << flush;
        break;

      default:
        assert(false && "unhandled case");
        break;
    }

    return str;
  }
  
  void Domain::add_data(double t, const Interval& y, ContractorNetwork& cn)
  {
    assert(m_type == DomainType::TUBE);

    m_map_data_s_lb.emplace(t, y.lb());
    m_map_data_s_ub.emplace(t, y.ub());
    Trajectory traj_lb(m_map_data_s_lb);
    Trajectory traj_ub(m_map_data_s_ub);

    Slice *prev_s = NULL;
    if(m_t.get().domain().contains(t))
      prev_s = m_t.get().slice(t)->prev_slice();
    else if(t > m_t.get().domain().ub())
      prev_s = m_t.get().last_slice();

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
    assert(m_type == DomainType::TUBE_VECTOR);
    assert(m_tv.get().size() == y.size());

    m_map_data_lb.emplace(t, y.lb());
    m_map_data_ub.emplace(t, y.ub());
    TrajectoryVector traj_lb(m_map_data_lb);
    TrajectoryVector traj_ub(m_map_data_ub);

    for(int i = 0 ; i < y.size() ; i++)
    {
      Slice *prev_s = NULL;
      if(m_tv.get()[i].domain().contains(t))
        prev_s = m_tv.get()[i].slice(t)->prev_slice();
      else if(t > m_tv.get()[i].domain().ub())
        prev_s = m_tv.get()[i].last_slice();

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
}