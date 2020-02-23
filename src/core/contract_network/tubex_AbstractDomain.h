/** 
 *  \file
 *  AbstractDomain class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_ABSTRACT_DOMAIN_H__
#define __TUBEX_ABSTRACT_DOMAIN_H__

#include "ibex_Interval.h"
#include "ibex_IntervalVector.h"
#include "tubex_Tube.h"
#include "tubex_TubeVector.h"

namespace tubex
{
  class AbstractDomain
  {
    public:

      AbstractDomain(ibex::Interval *i);
      AbstractDomain(ibex::IntervalVector *iv);
      AbstractDomain(tubex::Tube *t);
      AbstractDomain(tubex::TubeVector *tv);

      double volume() const;

    //protected:

      ibex::Interval *m_i = NULL;
      ibex::IntervalVector *m_iv = NULL;
      tubex::Tube *m_t = NULL;
      tubex::TubeVector *m_tv = NULL;
  };
}

#endif