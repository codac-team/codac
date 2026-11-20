/** 
 *  \file codac2_RS_zonotope.h
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
  /* Zonotope, 1-order reachability step */
  class RS_zonotope : public 
			ReachabilityStep<Zonotope,IntervalVector>
  {
      public:
        RS_zonotope(const DiffInclusionBase *diffinclusion,
		double max_zonotope_order=50.0,
		std::pair<double,double> inflation_options={1.1,1e-2}) :
		ReachabilityStep(diffinclusion), 
	        _inflation_ratio(inflation_options.first),
	        _inflation_base(inflation_options.second),
		_max_zonotope_order(max_zonotope_order) {
        }

        std::pair<double,double> get_inflation_options() const {
	   return { _inflation_ratio, _inflation_base };
        }
        void set_inflation_options(std::pair<double,double> options) {
	   std::tie(_inflation_ratio,_inflation_base) = 
			options;
        }

        double get_max_zonotope_order() const {
           return _max_zonotope_order;
        }
        void set_max_zonotope_order(double value) {
           _max_zonotope_order=value;
        }

        static const IntervalVector 
			intVect_from_DomainA(const Zonotope &a) {
            return a.box();
        }
        static const IntervalVector &
			intVect_from_DomainB(const IntervalVector &b) {
            return b;
        }

        Zonotope approx() const { return Zonotope(approx_base(_init.box())); }
        void inflate(Zonotope &a) const override;
        std::pair<Zonotope,Zonotope> enclose
			(const IntervalVector &b) const override;
        IntervalVector intersect(const IntervalVector &b, const Zonotope &a)
			 const override;

      private:
        double _inflation_ratio;
        double _inflation_base;
        double _max_zonotope_order;
  };

}
