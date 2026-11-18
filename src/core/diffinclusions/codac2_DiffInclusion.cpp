/** 
 *  \file codac2_DiffInclusion.cpp
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Damien Massé
 *  \copyright  Copyright 2026 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <iostream>
#include <functional>
#include "codac2_Matrix.h"
#include "codac2_Interval.h"
#include "codac2_IntervalMatrix.h"
#include "codac2_IntervalVector.h"
#include "codac2_DiffInclusion.h"
#include "codac2_ReachabilityStep.h"

namespace codac2 {

   IntervalVector 
	DiffInclusionBase::approx_RK4_base(const Interval &time_step,
                  const IntervalVector& init, const IntervalVector& u,
		  bool forward, bool end_gate) const {
       double start_time = (forward ? time_step.lb() : time_step.ub());
       double step_size = (forward ? time_step.diam() : -time_step.diam());
       IntervalVector k1 = f_eval(EvalMode::NATURAL,start_time,init,u);
       IntervalVector k2 = f_eval(EvalMode::NATURAL,start_time+0.5*step_size,
                           init+0.5*step_size*k1, u);
       IntervalVector k3 = f_eval(EvalMode::NATURAL,start_time+0.5*step_size,
                           init+0.5*step_size*k2, u);
       IntervalVector k4 = f_eval(EvalMode::NATURAL,start_time+step_size,
                           init+k3, u);
       IntervalVector ret = init + (step_size/6.0)*(k1 + 2.0*k2 + 2.0*k3 + k4);
       if (end_gate) return ret;
       IntervalVector inter = init + Interval(0,0.25)*step_size*(k1+k2);
       return (inter | ret);
     }

}
