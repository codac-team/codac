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
  AbstractDomain::AbstractDomain(ibex::Interval *i) : m_i(i)
  {

  }

  AbstractDomain::AbstractDomain(ibex::IntervalVector *iv) : m_iv(iv)
  {

  }

  AbstractDomain::AbstractDomain(tubex::Tube *t) : m_t(t)
  {
    *m_t &= Interval(-99999.,99999.); // todo: remove this
  }

  AbstractDomain::AbstractDomain(tubex::TubeVector *tv) : m_tv(tv)
  {
    *m_tv &= IntervalVector(m_tv->size(), Interval(-99999.,99999.)); // todo: remove this
  }

  double AbstractDomain::volume() const
  {
    if(m_i != NULL)
    {
      if(m_i->is_empty())
        return 0.;
      
      else if(m_i->is_unbounded())
        return 999999.; // todo: manager the unbounded case for fixed point detection
      
      else
        return m_i->diam();
    }

    else if(m_iv != NULL)
      return m_iv->volume();

    else if(m_t != NULL)
      return m_t->volume();

    else if(m_tv != NULL)
      return m_tv->volume();

    else
      assert(false && "unhandled case");
  }

  bool AbstractDomain::is_empty() const
  {
    if(m_i != NULL)
      return m_i->is_empty();

    else if(m_iv != NULL)
      return m_iv->is_empty();

    else if(m_t != NULL)
      return m_t->is_empty();

    else if(m_tv != NULL)
      return m_tv->is_empty();

    else
      assert(false && "unhandled case");
  }

  ostream& operator<<(ostream& str, const AbstractDomain& x)
  {
    if(x.m_i != NULL)
      str << *x.m_i << flush;

    else if(x.m_iv != NULL)
      str << *x.m_iv << flush;

    else if(x.m_t != NULL)
      str << *x.m_t << flush;

    else if(x.m_tv != NULL)
      str << *x.m_tv << flush;

    else
      assert(false && "unhandled case");

    return str;
  }
}