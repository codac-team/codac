/** 
 *  \file codac2_RS_box.cpp
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Damien Massé
 *  \copyright  Copyright 2026 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_AnalyticFunction.h"
#include "codac2_DiffInclusion.h"
#include "codac2_ReachabilityStep.h"
#include "codac2_RS_box.h"

namespace codac2
{
   void RS_box::inflate(IntervalVector &a) const {
       IntervalVector a_ins = a - a.mid();
       a += _inflation_ratio*a_ins;
       a.inflate(_inflation_base);
   }

   std::pair<IntervalVector,IntervalVector> 
		RS_box::enclose(const IntervalVector &b) const {
        IntervalVector evolution =
           _diffinclusion->f_eval(EvalMode::DEFAULT,_time_step,b,_u);
        return { _init+_time_interval*evolution,
                 _init+_time_bound*evolution };
   }

}
