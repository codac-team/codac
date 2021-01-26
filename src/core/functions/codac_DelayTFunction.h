/** 
 *  DelayTFunction class
 * ----------------------------------------------------------------------------
 *  \date       2018
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_DELAYTFUNCTION_H__
#define __CODAC_DELAYTFUNCTION_H__

#include "codac_TFnc.h"

namespace codac
{
  class DelayTFunction : public TFnc
  {
    public:

      DelayTFunction(double delay);

      using TFnc::eval; // todo: check this
      const ibex::Interval eval(int slice_id, const TubeVector& x) const;
      const ibex::Interval eval(const ibex::Interval& t, const TubeVector& x) const;
      const ibex::IntervalVector eval_vector(int slice_id, const TubeVector& x) const;
      const ibex::IntervalVector eval_vector(const ibex::Interval& t, const TubeVector& x) const;

    protected:

      double m_delay;
  };
}

#endif