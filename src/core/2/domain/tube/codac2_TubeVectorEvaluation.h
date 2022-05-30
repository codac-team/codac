/** 
 *  \file
 *  
 * ----------------------------------------------------------------------------
 *  \date       2022
 *  \author     Simon Rohou
 *  \copyright  Copyright 2022 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC2_TUBEVECTOREVALUATION_H__
#define __CODAC2_TUBEVECTOREVALUATION_H__

#include <memory>
#include "codac_Interval.h"
#include "codac_Trajectory.h"
#include "codac2_TubeVector.h"
#include "ibex_Interval.h"

namespace codac2
{
  using codac::Interval;

  class TubeVector;

  class TubeVectorEvaluation
  {
    public:

      const TubeVectorEvaluation& operator=(const IntervalVector& x);
      operator IntervalVector() const;

      friend std::ostream& operator<<(std::ostream& os, const TubeVectorEvaluation& x);
      

    protected:

      friend class TubeVector;
      explicit TubeVectorEvaluation(TubeVector& tubevector, double t);
      explicit TubeVectorEvaluation(TubeVector& tubevector, const Interval& t);

      const Interval _t;
      TubeVector& _tubevector;
  };

} // namespace codac

#endif