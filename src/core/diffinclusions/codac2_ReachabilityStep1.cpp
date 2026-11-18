/** 
 *  \file codac2_ReachabilityStep1.cpp
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Damien Massé
 *  \copyright  Copyright 2026 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <iostream>
#include "codac2_AnalyticFunction.h"
#include "codac2_DiffInclusion.h"
#include "codac2_ReachabilityStep.h"

namespace codac2
{
   void ReachabilityStep1::inflate(Zonotope &a) const {
       a.A *= (1.0+_inflation_ratio);
       IntervalVector bx = IntervalVector::Constant
		(a.c.size(),Interval(-_inflation_base,_inflation_base));
       a += bx;
   }

   std::pair<Zonotope,Zonotope> 
		ReachabilityStep1::enclose(const IntervalVector &b) const {
        /* evaluation on whole set */
        VectorType retWhole =
	   _diffinclusion->f_eval_diff(_time_step,_init.box(),_u);
        if (!retWhole.def_domain) { /* we can't differentiate ! */
           /* create a zonotope enclosing _time_interval * retWhole.a
           */
           Matrix Min { _time_interval.rad()*retWhole.a.mid() };
           Zonotope Zin(_time_interval.mid()*retWhole.a.mid(), Min);
           Zin += (_time_interval-_time_interval.mid())*retWhole.a.rad();
           IntervalVector nxtGate = _time_bound * retWhole.a;
           Zonotope gate(_init);
           gate += nxtGate;
           return {
              _init + Zin,
              gate
	   };
        }
        
        auto splitmat = _diffinclusion->split_diff_matrix(retWhole.da);
        IntervalVector lincorr_u = std::get<2>(splitmat)*(_u-_u.mid());
        IntervalVector quadcorr_x 
			= std::get<1>(splitmat)*retWhole.a
			+ std::get<0>(splitmat);

        /* evaluation on initial states, u_mid */
        VectorType retInit =
	   _diffinclusion->f_eval_diff(_start_time,_init.box(),_u.mid());
	assert_release(retInit.def_domain);
        splitmat = _diffinclusion->split_diff_matrix(retInit.da);

        /* creation of the reachable block */
        IntervalMatrix m1 = IntervalMatrix::Identity(b.size(),b.size()) 
			+ _time_interval*std::get<1>(splitmat);
	Zonotope zinside = _init.transform(m1);
        zinside += (_time_interval*(retInit.m+lincorr_u)
			 + 0.5*sqr(_time_interval)*retInit.m);
		/* FIXME : make a zonotope ? */
       
        /* creation of the reachable gate */
        m1 = IntervalMatrix::Identity(b.size(),b.size()) 
			+ _time_bound*std::get<1>(splitmat);
	Zonotope zgate = _init.transform(m1);
        zgate += (_time_bound*(retInit.m+lincorr_u)
			 + 0.5*sqr(_time_bound)*quadcorr_x);
        return { zinside, zgate };
   }

   IntervalVector ReachabilityStep1::intersect(const IntervalVector &b, 
					const Zonotope &a) const {
       return b & a.box();
   }

}
