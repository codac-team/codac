/** 
 *  \file codac2_RS_box.h
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Damien Massé
 *  \copyright  Copyright 2026 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_AnalyticFunction.h"
#include "codac2_DiffInclusion.h"
#include "codac2_ReachabilityStep.h"

namespace codac2
{
  /* IntervalVector, 0-order reachability step */
  class RS_box : public 
			ReachabilityStep<IntervalVector,IntervalVector>
  {
      public:
        RS_box(const DiffInclusionBase *diffinclusion,
		double inflation_ratio=1.1, double inflation_base=1e-2) :
		ReachabilityStep(diffinclusion), 
	        _inflation_ratio(inflation_ratio),
	        _inflation_base(inflation_base) {
        }

        std::pair<double,double> get_inflation_options() const {
	   return { _inflation_ratio, _inflation_base };
        }
        void set_inflation_options(std::pair<double,double> options) {
	   std::tie(_inflation_ratio,_inflation_base) = 
			options;
        }

        IntervalVector approx() const { return approx_base(_init); }
        void inflate(IntervalVector &a) const override;
        std::pair<IntervalVector,IntervalVector> enclose
			(const IntervalVector &b) const override;
        IntervalVector intersect(const IntervalVector &b, 
					const IntervalVector &a)
			 const {
	    return (b & a);
        }
        static const IntervalVector &
			intVect_from_DomainA(const IntervalVector &a) {
            return a;
        }
        static const IntervalVector &
			intVect_from_DomainB(const IntervalVector &b) {
            return b;
        }


      private:
        double _inflation_ratio;
        double _inflation_base;
  };


}
