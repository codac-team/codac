/* ============================================================================
 *  tubex-lib - CtcDeriv class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#include "tubex_CtcDeriv.h"
#include "tubex_DomainException.h"
#include "tubex_EmptyException.h"

using namespace std;
using namespace ibex;

namespace tubex
{
  CtcDeriv::CtcDeriv(Tube *x, const Tube *v, const ibex::Interval& x0) : m_x(x), m_v(v), m_x0(x0)
  {

  }

  bool CtcDeriv::contract()
  {
    bool contraction = contractFwd();
    contraction |= contractBwd();
    return contraction;
  }

  bool CtcDeriv::contractFwd() // temporal forward
  {
    checkStructures(*m_x, *m_v);
    checkEmptiness(*m_v);

    int size = m_x->size();
    bool contraction = false;

    Interval next_y = (*m_x)[0];
    Interval y_front = next_y & m_x0;

    for(int i = 0 ; i < size ; i++) // from the past to the future
    {
      double dt = m_x->domain(i).diam();
      Interval y_old = next_y;
      Interval y_new = y_old & (y_front + (*m_v)[i] * Interval(0., dt));
      contraction |= y_new.diam() < y_old.diam();
      m_x->set(y_new, i);

      // Discontinuous
      if(y_new.is_empty())
      {
        m_x->set(Interval::EMPTY_SET);
        contraction = true;
        break;
      }

      // Preparing next slice computation
      if(i < size - 1)
      {
        y_front = y_old & (y_front + (*m_v)[i] * dt);
        next_y = (*m_x)[i + 1];
        y_front &= next_y;
      }
    }

    return contraction;
  }

  bool CtcDeriv::contractBwd() // temporal backward
  {
    checkStructures(*m_x, *m_v);
    checkEmptiness(*m_v);

    int size = m_x->size();
    bool contraction = false;

    Interval next_y = (*m_x)[size - 1];
    Interval y_front = next_y & next_y - (*m_v)[size - 1] * m_v->domain(size - 1).diam();
    next_y = (*m_x)[max(0, size - 2)];

    for(int i = max(0, size - 2) ; i >= 0 ; i--) // from the future to the past
    {
      double dt = m_x->domain(i).diam();
      Interval y_old = (*m_x)[i];
      Interval y_new = y_old & (y_front - (*m_v)[i] * Interval(0., dt));
      contraction |= y_new.diam() < y_old.diam();
      m_x->set(y_new, i);

      // Discontinuous
      if(y_new.is_empty())
      {
        m_x->set(Interval::EMPTY_SET);
        contraction = true;
        break;
      }

      if(i > 0)
      {
        y_front = y_old & (y_front - (*m_v)[i] * dt);
        next_y = (*m_x)[i - 1];
        y_front &= next_y;
      }
    }

    return contraction;
  }
}