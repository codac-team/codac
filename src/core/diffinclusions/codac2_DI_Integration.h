/** 
 *  \file codac2_DI_Integration.h
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
#include "codac2_SlicedTube.h"
#include "codac2_TimePropag.h"
#include <memory>

namespace codac2
{
  template <typename DI, typename IS>
  class DI_Integration {
     public:
       using XType = DI::XType;
       /* X,U : e.g. IntervalVector */
       using X = DI::X;
       using U = DI::U;
  
       using DomA = IS::DomainA;
       using DomB = IS::DomainB;

       DI_Integration(const DI& di, int widen_attempts=3, 
		int narrow_attempts=3) : 
		_di(std::make_unique<DI>(di)), 
		_is(std::make_unique<IS>(&(*_di))), 
		_widen_attempts(widen_attempts),
		_narrow_attempts(narrow_attempts) { }

       
       std::unique_ptr<DI> &get_differentialinclusion() {
 	   return _is;
       }
       std::unique_ptr<IS> &get_reachabilitystep() {
 	   return _is;
       }
  
       std::pair<bool,DomA> integration_step(DomA& approx,
               DomB& in_domain) const {
           int nb_tries=0;
           DomB inside_in;
           bool success=false;
           while (nb_tries<_widen_attempts) { /* inflation steps */
              _is->inflate(approx);
              DomB inside = _is->intersect(in_domain,approx);
              std::tie(approx,std::ignore) = _is->enclose(inside); 
              DomB new_inside = _is->intersect(in_domain,approx);
              if (new_inside.is_subset(inside)) {
                  in_domain = new_inside;
                   success=true;
                   break;
              }
              nb_tries++;
          }
          /* success = true : approx and in_domain are guaranteed; gate
           *                  is not computed.
           * success = false : approx is not sufficient, 
                               in_domain has not changed */
          if (in_domain.is_unbounded()) return { false,approx };
             nb_tries=0;
             DomA gate;
             while (nb_tries<_narrow_attempts) { /* reduction steps */
                DomB inside = in_domain;
                std::tie(approx,gate)= _is->enclose(inside);
                in_domain = _is->intersect(inside,approx);
                if (inside.is_subset(in_domain)) break;
                success=true;
                nb_tries++;
          }
          return { success, gate };
      }
  
      bool contract_one_step(const Interval &time_step,
  		const DomA& init, DomB& in_domain, DomA& out_gate,
  		const U& u, bool forward=true) {
           IntervalVector u_ = _di->from_U(u);
           _is->update_time(time_step, forward ? IS::FWD : IS::BWD);
           _is->update_init(init);
           _is->update_u(u_);
           DomA z1 = _is->approx();
           bool result;
           DomA out_gate2;
           std::tie(result,out_gate2) = 
                  this->integration_step(z1,in_domain);
  	 if (result) 
  	     out_gate = (out_gate2 & in_domain); /* FIXME : intersection? */
           return result;
      }
  
      std::pair<bool,DomA> integrate_to_time_indom(const Interval &time_step,
                  const DomA& init, const DomB& in_domain, 
                  const U& u, double t_steps) {
           assert_release(t_steps!=0.0);
           bool forward=(t_steps>0);
           double cur_time = (forward ? time_step.lb() : -time_step.ub());
           double end_time = (forward ? time_step.ub() : -time_step.lb());
           if (!forward) t_steps = -t_steps;
           DomA current(init);
           while (cur_time<end_time) {
              double nxt_time=cur_time+t_steps;
              if (nxt_time>end_time) nxt_time=end_time;
              Interval small_step(cur_time,nxt_time);
              DomB in_domain2(in_domain);
              DomA out_gate2;
              bool result = this->contract_one_step(
  			(forward ? small_step : -small_step),
  		current, in_domain2, out_gate2, u, forward);
              if (!result) return { false, current };
  			 /* FIXME : use smaller steps ? */
              current = out_gate2;
              if (_is->intersect(in_domain,current).is_empty()) 
  		return { true, IS::empty_DomA() };
              cur_time=nxt_time;
           }
           return { true, current };
      }
  
      std::pair<bool,X> integrate_to_time(const Interval &time_step,
                  const X& init, const X& in_domain, 
                  const U& u, double t_steps) {
           DomA ninit(_di->from_X(init));
           DomB indomain(_di->from_X(in_domain));
           auto res = this->integrate_to_time_indom(time_step,
  			ninit, indomain, u, t_steps);
           return { res.first, _di->to_X(IS::intVect_from_DomainA(res.second)) };
      }

      SlicedTube<X> create_sliced_tube
		(const std::shared_ptr<TDomain>& tdomain,
			const X&init, const X& in_domain, 
			const U& u, bool forward=true) {
            DomB indomain(_di->from_X(in_domain));
            SlicedTube<X> built(tdomain, in_domain);
            DomA current(_di->from_X(init));
	    bool empty = init.is_empty();
            auto sl = (forward ? built.first_slice() : built.last_slice());
            while (sl!=nullptr) {
	       if (empty) {
                   sl->codomain().set_empty();
               } else 
	       if (sl->is_gate()) {
                   sl->codomain() &=
			_di->to_X(IS::intVect_from_DomainA(current));
               } else {
                   DomB indomain2(indomain);
                   DomA out_gate2;
                   bool result = this->contract_one_step(
  			sl->t0_tf(),
  		        current, indomain2, out_gate2, u, forward);
                   if (result) {
		      sl->codomain() &= 
			_di->to_X(IS::intVect_from_DomainB(indomain2));
		      current = out_gate2;
		      if (sl->codomain().is_empty() ||
				current.is_empty()) empty=true;
                   } else {
		      current = DomA(_di->from_X(in_domain));
		   }
               }
	       sl = (forward ? sl->next_slice() : sl->prev_slice());
            }
	    return built;
      }
			
      void contract_sliced_tube
		(SlicedTube<X>& tube,
			const U& u, bool forward=true) {
            Interval global_time=tube.t0_tf();
            auto sl = (forward ? tube.first_slice() : tube.last_slice());
            DomA current(_di->from_X(sl->codomain()));
	    bool empty = sl->codomain().is_empty();
            while (sl!=nullptr) {
	       if (empty) {
                   sl->codomain().set_empty();
               } else 
	       if (sl->is_gate()) {
                   sl->codomain() &=
			_di->to_X(IS::intVect_from_DomainA(current));
		   if (sl->codomain().is_empty()) empty=true;
		   current &= _di->from_X(sl->codomain());
		   if (current.is_empty()) empty=true;
               } else {
                   DomB indomain2(_di->from_X(sl->codomain()));
                   DomA out_gate2;
                   current &= indomain2;
                   bool result = this->contract_one_step(
  			sl->t0_tf(),
  		        current, indomain2, out_gate2, u, forward);
                   if (result) {
		      sl->codomain() &= 
			_di->to_X(IS::intVect_from_DomainB(indomain2));
		      current = out_gate2;
		      if (sl->codomain().is_empty() ||
				current.is_empty()) empty=true;
                   } else {
		      current = DomA(_di->from_X(sl->codomain()));
		   }
               }
	       sl = (forward ? sl->next_slice() : sl->prev_slice());
            }
      }

      void contract_sliced_tube
		(SlicedTube<X>& tube,
			const U& u, TimePropag dir) {
	    if (dir == TimePropag::FWD) {
	       this->contract_sliced_tube(tube,u,true);
	    } else if (dir == TimePropag::BWD) {
	       this->contract_sliced_tube(tube,u,false);
            } else {
	       for (int i=0;i<2;i++) {
	         this->contract_sliced_tube(tube,u,true);
	         this->contract_sliced_tube(tube,u,false);
	       }   /* TODO : check the changes ? */
            }
      }

    private:
      std::unique_ptr<DI> _di;  
      std::unique_ptr<IS> _is;
      int _widen_attempts;
      int _narrow_attempts;
  }; 

}

