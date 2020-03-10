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
  Interval AbstractDomain::m_dump_i = Interval();
  IntervalVector AbstractDomain::m_dump_iv = IntervalVector(1);
  Tube AbstractDomain::m_dump_t = Tube(Interval(0.,1.));
  TubeVector AbstractDomain::m_dump_tv = TubeVector(Interval(0.,1.));

  AbstractDomain::AbstractDomain(ibex::Interval& i)
    : m_type(INTERVAL), m_i(i), m_iv(m_dump_iv), m_t(m_dump_t), m_tv(m_dump_tv)
  {

  }

  AbstractDomain::AbstractDomain(ibex::IntervalVector& iv)
    : m_type(INTERVAL_VECTOR), m_i(m_dump_i), m_iv(iv), m_t(m_dump_t), m_tv(m_dump_tv)
  {

  }

  AbstractDomain::AbstractDomain(tubex::Tube& t)
    : m_type(TUBE), m_i(m_dump_i), m_iv(m_dump_iv), m_t(t), m_tv(m_dump_tv)
  {
    m_t &= Interval(-99999.,99999.); // todo: remove this
  }

  AbstractDomain::AbstractDomain(tubex::TubeVector& tv)
    : m_type(TUBE_VECTOR), m_i(m_dump_i), m_iv(m_dump_iv), m_t(m_dump_t), m_tv(tv)
  {
    m_tv &= IntervalVector(m_tv.size(), Interval(-99999.,99999.)); // todo: remove this
  }

  DomainType AbstractDomain::type() const
  {
    return m_type;
  }

  double AbstractDomain::volume() const
  {
    switch(m_type)
    {
      case INTERVAL:
        if(m_i.is_empty())
          return 0.;
        
        else if(m_i.is_unbounded())
          return 999999.; // todo: manager the unbounded case for fixed point detection
        
        else
          return m_i.diam();

      case INTERVAL_VECTOR:
        return m_iv.volume();

      case TUBE:
        return m_t.volume();

      case TUBE_VECTOR:
        return m_tv.volume();

      default:
        assert(false && "unhandled case");
    }
  }

  bool AbstractDomain::is_empty() const
  {
    switch(m_type)
    {
      case INTERVAL:
        return m_i.is_empty();

      case INTERVAL_VECTOR:
        return m_iv.is_empty();

      case TUBE:
        return m_t.is_empty();

      case TUBE_VECTOR:
        return m_tv.is_empty();

      default:
        assert(false && "unhandled case");
    }
  }

  ostream& operator<<(ostream& str, const AbstractDomain& x)
  {
    switch(x.m_type)
    {
      case INTERVAL:
        str << x.m_i << flush;
        break;

      case INTERVAL_VECTOR:
        str << x.m_iv << flush;
        break;

      case TUBE:
        str << x.m_t << flush;
        break;

      case TUBE_VECTOR:
        str << x.m_tv << flush;
        break;

      default:
        assert(false && "unhandled case");
        break;
    }

    return str;
  }
}