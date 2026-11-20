/** 
 *  \file codac2_ReachabilityStep.h
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Damien Massé
 *  \copyright  Copyright 2026 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_AnalyticFunction.h"
#include "codac2_DiffInclusion.h"

namespace codac2
{
  /* class to handle reachability steps on a specific domain,
     has two methods : inflate (on DomA) and enclose (on DomA and DomB)
     DomA can be inflated, temporaly propagated but maybe not compared or
     intersected (IntervalVector, Zonotope, Polytope)
     DomB can be (partially) intersected with DomA, intersected with DomB,
     and inclusion can be tested (IntervalVector, maybe Polytope)
     Reachability step includes the underlying differential equation,
     initial step, temporal interval, direction, 
     value of the uncertainties (maybe), and maybe other elements(?)
  */
  template <typename DomA, typename DomB> 
  class ReachabilityStep {
      public:
         using DomainA = DomA;
         using DomainB = DomB;
         enum StepDirection { FWD, BWD };         

         /* approximation step on interval vectors , to be used
          * as approx if needed */
         IntervalVector approx_base(const IntervalVector &init) const {
            IntervalVector k1 = _diffinclusion->f_eval(EvalMode::NATURAL,
			_start_time,init,_u);
            IntervalVector k2 = _diffinclusion->f_eval(EvalMode::NATURAL,
		_start_time+0.5*_time_bound, init+0.5*_time_bound*k1, _u);
            IntervalVector k3 = _diffinclusion->f_eval(EvalMode::NATURAL,
		_start_time+0.5*_time_bound, init+0.5*_time_bound*k2, _u);
            IntervalVector k4 = _diffinclusion->f_eval(EvalMode::NATURAL,
		_start_time+_time_bound, init+k3, _u);
            IntervalVector ret = init + 
			(_time_bound/6.0)*(k1 + 2.0*k2 + 2.0*k3 + k4);
            IntervalVector inter = init + Interval(0,0.25)*_time_bound*(k1+k2);
            return (inter | ret);
         }

         virtual DomA approx() const = 0;
	 virtual void inflate(DomA &a) const =0;
         virtual std::pair<DomA,DomA> enclose(const DomB &b) const =0;
         virtual DomB intersect(const DomB &b, const DomA &a) const = 0;

         void update_time(const Interval &time_step, StepDirection dir) {
             _dir = dir;
             _time_step = time_step;
             _start_time = (dir==FWD ? _time_step.lb() : _time_step.ub());
             _time_interval = time_step-_start_time;
             _time_bound =
                   (dir==FWD ? time_step.ub()-Interval(_start_time) :
                        time_step.lb()-Interval(_start_time));
         }

         void update_init(const DomA &init) {
             _init = init;
         }

         void update_u(const IntervalVector &u) {
             _u=u;
         }

      protected:
         ReachabilityStep(const DiffInclusionBase *diffinclusion) :
			_diffinclusion(diffinclusion) { }

         const DiffInclusionBase *_diffinclusion;
         StepDirection _dir;
	 double _start_time;
         Interval _time_step;     /* time value in the integration */
         Interval _time_interval; /* interval (0, end_time) or (-end_time,0) */
         Interval _time_bound;    /* (end_time) or (-end_time) */
	 DomA _init;
	 IntervalVector _u;
  };

  /* IntervalVector, 0-order reachability step */
  class ReachabilityStep0 : public 
			ReachabilityStep<IntervalVector,IntervalVector>
  {
      public:
        ReachabilityStep0(const DiffInclusionBase *diffinclusion,
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

  /* Zonotope, 1-order reachability step */
  class ReachabilityStep1 : public 
			ReachabilityStep<Zonotope,IntervalVector>
  {
      public:
        ReachabilityStep1(const DiffInclusionBase *diffinclusion,
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
