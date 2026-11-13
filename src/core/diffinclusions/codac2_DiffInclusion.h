/** 
 *  \file codac2_DiffInclusion.h
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Damien Massé
 *  \copyright  Copyright 2026 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_AnalyticFunction.h"

namespace codac2
{
    /** diffential inclusion x' = f(x,u) or f(t,x,u).
     *  x and u can be scalar, interval or matrix. 
     *  For practical reasons (on the interface), we consider
     *  one (and only one) u */
    template<typename X, typename U>
    class DiffInclusion 
    {
        public:

        using OutputType = typename ExprType<X>::Type;

        DiffInclusion(const AnalyticFunction<OutputType>& f, 
		      bool time_dependant=false) : _f(f), 
			_time_dependant(time_dependant) {
           int x_arg=(time_dependant ? 1 : 0);
 	   assert_release((f.args().size() == x_arg+2)
		&& "DiffInclusion: wrong args size");
           if (time_dependant) {
              assert_release((f.args()[0]->size() == 1)
		&& "DiffInclusion: wrong time size");
           }
           assert_release((f.args()[x_arg]->size() == f.output_size())
		&& "DiffInclusion: wrong output size");
        }

        /** \brief approximation of IVP on a step using RK4. 
         * Does not return a guaranteed result, used for
         * approximate enclosing 
         * \param time_step time stem (backward or forward)
         * \param init initial values
         * \param u uncertainties
         */
        template<bool forward=true, bool end_gate=false>
        X approx_RK4(const Interval &time_step, const X& init,
			const U& u) {
            double start_time = (forward ? time_step.lb() : time_step.ub());
            double step_size = (forward ? time_step.diam() : -time_step.diam());
            X k1 = f_eval(EvalMode::NATURAL,start_time,init,u);
            X k2 = f_eval(EvalMode::NATURAL,start_time+0.5*step_size,
				init+0.5*step_size*k1, u);
            X k3 = f_eval(EvalMode::NATURAL,start_time+0.5*step_size,
				init+0.5*step_size*k2, u);
            X k4 = f_eval(EvalMode::NATURAL,start_time+step_size,
				init+k3, u);
            X ret = init + (step_size/6.0)*(k1 + 2.0*k2 + 2.0*k3 + k4);
            if (end_gate) return ret;
            X inter = init + Interval(0,0.25)*step_size*(k1+k2);
            return (inter | ret);
        }
        
        /** \brief basic enclosing step
         *  \param time_step time stem (backward or forward)
         *  \param init_domain initial values
         *  \param in_domain   inside values (may be contracted)
         *  \param out_domain  final values (may be contracted)
         *  \param u uncertainties
         *  \param inflation_ratio inflation ratio (default 0.1)
         *  \param inflation_base  inflation base  (default 1e-2)
         *  \param attempts max number of attempts      (default 3)
         *  \return false if no improvement was found */
        template<bool forward=true>
        bool basic_contract(const Interval &time_step, 
		const X& init, X& in_domain, X& out_domain,
		const U& u, double inflation_ratio=1.1, 
		double inflation_base=1e-2, int attempts=3) {
            double start_time = (forward ? time_step.lb() : time_step.ub());
            Interval time_interval = time_step-start_time;
            X v1 = this->template approx_RK4<forward>(time_step,
			init, u);
            int nbtries=0;
            X evolution;
            while (nbtries<attempts) { /* inflation steps */
               X v1ins = v1 - v1.mid();
               v1 += inflation_ratio*v1ins;
               v1.inflate(inflation_base);
               v1 &= in_domain;
               evolution = f_eval(EvalMode::DEFAULT,time_step,v1,u);
               X old_v1 = v1;
               v1 = init + time_interval*evolution;
               v1 &= in_domain;
               if (v1.is_subset(old_v1)) break;
               nbtries++;
            }
            bool success= (nbtries<attempts);
            if (!success) {
               if (in_domain.is_unbounded()) return false;
               v1 = in_domain;
            }
            nbtries=0;
            while (nbtries<attempts) { /* reduction steps */
               evolution = f_eval(EvalMode::DEFAULT,time_step,v1,u);
               X old_v1 = v1;
               v1 = init + time_interval*evolution;
               v1 &= in_domain;
               if (old_v1.is_subset(v1)) break;
               nbtries++;
            }
            success=(nbtries>0);
            if (success) {
               in_domain=v1;
               Interval time_bound = 
			(forward ? time_step.ub()-Interval(start_time) :
			     time_step.lb()-Interval(start_time));
               /* note: diam() is rounded upward, but for time_bound we don't
                  want any rounding */
               out_domain &= init + time_bound * evolution;
            }
            return success;
        }

      protected:
        const AnalyticFunction<OutputType> _f;
        bool _time_dependant = false;

        inline X f_eval(const EvalMode& m, const Interval &t,
				const X& x, const U& u) {
             if (_time_dependant) 
		return _f.eval(m,t,x,u);
             else 
		return _f.eval(m,x,u);
        }
    };
}
    

