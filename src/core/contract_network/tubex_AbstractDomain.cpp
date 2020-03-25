/** 
 *  AbstractDomain class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "tubex_AbstractDomain.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  AbstractDomain::AbstractDomain(const AbstractDomain& ad)
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

  AbstractDomain::AbstractDomain(ibex::Interval& i)
    : m_type(DomainType::INTERVAL), m_i(i)
  {

  }

  AbstractDomain::AbstractDomain(ibex::IntervalVector& iv)
    : m_type(DomainType::INTERVAL_VECTOR), m_iv(iv)
  {

  }

  AbstractDomain::AbstractDomain(tubex::Slice& s)
    : m_type(DomainType::SLICE), m_s(s)
  {
    m_s.get() &= Slice(m_s.get().domain(),Interval(-99999.,99999.)); // todo: remove this
  }

  AbstractDomain::AbstractDomain(tubex::Tube& t)
    : m_type(DomainType::TUBE), m_t(t)
  {
    m_t.get() &= Interval(-99999.,99999.); // todo: remove this
  }

  AbstractDomain::AbstractDomain(tubex::TubeVector& tv)
    : m_type(DomainType::TUBE_VECTOR), m_tv(tv)
  {
    m_tv.get() &= IntervalVector(m_tv.get().size(), Interval(-99999.,99999.)); // todo: remove this
  }

  AbstractDomain::~AbstractDomain()
  {
    
  }

  DomainType AbstractDomain::type() const
  {
    return m_type;
  }

  ibex::Interval& AbstractDomain::interval()
  {
    assert(m_type == DomainType::INTERVAL);
    return const_cast<Interval&>(static_cast<const AbstractDomain&>(*this).interval());
  }

  const ibex::Interval& AbstractDomain::interval() const
  {
    assert(m_type == DomainType::INTERVAL);
    return m_i.get();
  }

  ibex::IntervalVector& AbstractDomain::interval_vector()
  {
    assert(m_type == DomainType::INTERVAL_VECTOR);
    return const_cast<IntervalVector&>(static_cast<const AbstractDomain&>(*this).interval_vector());
  }

  const ibex::IntervalVector& AbstractDomain::interval_vector() const
  {
    assert(m_type == DomainType::INTERVAL_VECTOR);
    return m_iv.get();
  }

  tubex::Slice& AbstractDomain::slice()
  {
    assert(m_type == DomainType::SLICE);
    return const_cast<Slice&>(static_cast<const AbstractDomain&>(*this).slice());
  }

  const tubex::Slice& AbstractDomain::slice() const
  {
    assert(m_type == DomainType::SLICE);
    return m_s.get();
  }

  tubex::Tube& AbstractDomain::tube()
  {
    assert(m_type == DomainType::TUBE);
    return const_cast<Tube&>(static_cast<const AbstractDomain&>(*this).tube());
  }

  const tubex::Tube& AbstractDomain::tube() const
  {
    assert(m_type == DomainType::TUBE);
    return m_t.get();
  }

  tubex::TubeVector& AbstractDomain::tube_vector()
  {
    assert(m_type == DomainType::TUBE_VECTOR);
    return const_cast<TubeVector&>(static_cast<const AbstractDomain&>(*this).tube_vector());
  }

  const tubex::TubeVector& AbstractDomain::tube_vector() const
  {
    assert(m_type == DomainType::TUBE_VECTOR);
    return m_tv.get();
  }

  vector<AbstractContractor*>& AbstractDomain::contractors()
  {
    return m_v_ctc;
  }

  double AbstractDomain::compute_volume() const
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
  }

  double AbstractDomain::get_saved_volume() const
  {
    return m_volume;
  }

  void AbstractDomain::set_volume(double vol)
  {
    m_volume = vol;
  }

  bool AbstractDomain::is_empty() const
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
  }
  
  bool AbstractDomain::operator==(const AbstractDomain& x) const
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
  
  bool AbstractDomain::operator!=(const AbstractDomain& x) const
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

  ostream& operator<<(ostream& str, const AbstractDomain& x)
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
}