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
        return POS_INFINITY;
      
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
}