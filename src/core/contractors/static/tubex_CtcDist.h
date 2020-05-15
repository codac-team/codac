/** 
 *  \file
 *  CtcDist class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_CTCDIST_H__
#define __TUBEX_CTCDIST_H__

#include "tubex_CtcFunction.h"

namespace tubex
{
  /**
   * \class CtcDist
   */
  class CtcDist : public CtcFunction
  {
    public:

      CtcDist();
      void contract(ibex::IntervalVector& a, ibex::IntervalVector& b, ibex::Interval& d);
  };
}

#endif