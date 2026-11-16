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
    /* class to handle reachability steps on a specific domain,
       has two methods : inflate (on DomA) and enclose (on DomA and DomB)
       see codac2_ReachabilityStep.h */
    template <typename DomA, typename DomB>
    class ReachabilityStep;

    /* DiffInclusionBase is an abstract class which considers only
     * functions (IntervalVector,IntervalVector) -> IntervalVector.
     * Below DiffInclusion extends DiffInclusionBase to handle other
     * functions */
    class DiffInclusionBase
    {
       public:
          virtual IntervalVector f_eval(const EvalMode& m, const Interval &t,
			  	        const IntervalVector& x, 
				        const IntervalVector& u) const =0;

          virtual VectorType f_eval_diff(const Interval &t,
			  	        const IntervalVector& x, 
				        const IntervalVector& u) const =0;

          std::tuple<IntervalVector,IntervalMatrix,IntervalMatrix>
		split_diff_matrix(const IntervalMatrix &da) const {
             if (_time_dependant) {
		return {
		   da.col(0),
                   da.middleCols(1,_x_shape.first*_x_shape.second),
                   da.rightCols(_u_shape.first*_u_shape.second)
                };
             } else {
		return {
		   IntervalVector::Zero(_x_shape.first*_x_shape.second),
                   da.leftCols(_x_shape.first*_x_shape.second),
                   da.rightCols(_u_shape.first*_u_shape.second)
                };
             }
          }

       protected:
          DiffInclusionBase(bool td) : _time_dependant(td) { }

          IntervalVector approx_RK4_base(const Interval &time_step, 
		const IntervalVector& init, const IntervalVector& u,
		bool forward, bool end_gate) const;

          bool basic_contract_base(const Interval &time_step, 
		const IntervalVector& init, 
		IntervalVector& in_domain, IntervalVector& out_gate,
		const IntervalVector& u, double inflation_ratio, 
		double inflation_base, int attempts, bool forward) const;

          bool contract_order1_base(const Interval &time_step, 
		const Zonotope& init, 
		IntervalVector& in_domain, Zonotope& out_gate,
		const IntervalVector& u, double inflation_ratio, 
		double inflation_base, int attempts, bool forward) const;


          const bool _time_dependant = false;
          std::pair<Index,Index> _x_shape;
          std::pair<Index,Index> _u_shape;

       private:
         /* general templated single-step fixpoint-based integration scheme, 
          * starts with an initial set and an approximation of the reachable set
          * 1) Increasing chain loop to get a post-fixpoint (widening process)
          *   a) intersect with current out => B
          *   b) compute guaranteed enclosing set A (function 2)
          *   c) check inclusion A <= B
          *   d) if inclusion false, enlarge and loop
          *   e) stops if inclusion is true or nb attempts too large (failure)
          * 2) If 3) failed and unbounded out, returns false
          * 3) Decreasing chain loop (narrowing process)
          *   a) starts with previous results if increasing succeeds, or 
          *      with current out => B
          *   b) compute guaranteed enclosing set A (and gate) (function 2)
          *   c) check inclusion B <= A
          *   d) if inclusion true or nb attempts limited, stops
          *   e) otherwise, B=A and loop
          * 4) returns true if 2 succeeded or one step of 4 succeeded,
	  * otherwise false
          */
         template<typename DomA, typename DomB>
           std::pair<bool,DomA> general_integration_step(DomA& approx,
	     DomB& in_domain, const ReachabilityStep<DomA,DomB> &reach,
             int widen_attempts, int narrow_attempts) const {
             int nb_tries=0;
             DomB inside_in;
             bool success=false;
             while (nb_tries<widen_attempts) { /* inflation steps */
                reach.inflate(approx);
                DomB inside = reach.intersect(in_domain,approx);
                std::tie(approx,std::ignore) = reach.enclose(inside); 
                DomB new_inside = reach.intersect(in_domain,approx);
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
             while (nb_tries<narrow_attempts) { /* reduction steps */
                DomB inside = in_domain;
                std::tie(approx,gate)= reach.enclose(inside);
                in_domain = reach.intersect(inside,approx);
                if (inside.is_subset(in_domain)) break;
                success=true;
                nb_tries++;
             }
             return { success, gate };
         }

    };

	


    /** diffential inclusion x' = f(x,u) or f(t,x,u).
     *  x and u can be scalar, interval or matrix. 
     *  For practical reasons (on the interface), we consider
     *  exactly one u. (TODO: lift this restriction ?) */
    template<typename XVar=VectorVar, typename UVar=VectorVar>
    class DiffInclusion : public DiffInclusionBase
    {
        public:

	/* Type : e.g. VectorType (for AnalyticFunction<XType>) */
        using XType = typename ExprType<XVar>::Type;
        /* X,U : e.g. IntervalVector */
        using X = XType::Domain;
        using U = typename ExprType<UVar>::Type::Domain;
        

        DiffInclusion(const AnalyticFunction<XType>& f, 
		      bool time_dependant=false) : 
			DiffInclusionBase(time_dependant), _f(f) 
		 {
           int x_arg=(time_dependant ? 1 : 0);
 	   assert_release(f.args().size() == x_arg+2 && 
			"DiffInclusion: wrong args size");
	   /* NdD : I don't understand why output_shape is not
	      a virtual method of VarBase but maybe some variable types
              cannot have any "shape"... */
           _u_shape = dynamic_cast<const UVar&>(*(f.args()[x_arg+1])).
			output_shape();
           if (time_dependant) {
              assert_release((f.args()[0]->size() == 1)
		&& "DiffInclusion: wrong time size");
           }
           _x_shape = dynamic_cast<const XVar&>(*(f.args()[x_arg])).
			output_shape();
           assert_release(_x_shape == f.output_shape()
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
        X approx_RK4(const Interval &time_step, const X& init, const U& u) {
            return 
	    to_X(approx_RK4_base(time_step,
				from_X(init),from_U(u),forward,end_gate));
        }

	
        /** \brief basic enclosing step
         *  \param time_step time stem (backward or forward)
         *  \param init_domain initial values
         *  \param in_domain   inside values (may be contracted)
         *  \param out_gate    final values (may be contracted)
         *  \param u uncertainties
         *  \param inflation_ratio inflation ratio (default 0.1)
         *  \param inflation_base  inflation base  (default 1e-2)
         *  \param attempts max number of attempts      (default 3)
         *  \return false if no improvement was found */
        template<bool forward=true>
        bool basic_contract(const Interval &time_step, 
		const X& init, X& in_domain, X& out_gate,
		const U& u, double inflation_ratio=1.1, 
		double inflation_base=1e-2, int attempts=3) {
            IntervalVector in_domain_base = from_X(in_domain);
            IntervalVector out_gate_base = from_X(out_gate);
            bool ret =  basic_contract_base(time_step,
	 	from_X(init), in_domain_base, out_gate_base,
		from_U(u), inflation_ratio, inflation_base,attempts,forward);
            in_domain = to_X(in_domain_base);
            out_gate = to_X(out_gate_base);
            return ret;
        }

        template<bool forward=true>
        bool contract_order1(const Interval &time_step, 
		const Zonotope& init, X& in_domain, Zonotope& out_gate,
		const U& u, double inflation_ratio=1.1, 
		double inflation_base=1e-2, int attempts=3) {
            IntervalVector in_domain_base = from_X(in_domain);
            bool ret =  contract_order1_base(time_step,
	 	init, in_domain_base, out_gate,
		from_U(u), inflation_ratio, inflation_base,attempts,forward);
            in_domain = to_X(in_domain_base);
            return ret;
        }

        inline IntervalVector f_eval(const EvalMode& m, const Interval &t,
			  	     const IntervalVector& x, 
				     const IntervalVector& u) const override { 
             if constexpr(std::is_same_v<X,IntervalVector>
			&& std::is_same_v<U,IntervalVector>) {
               if (_time_dependant) 
	 	  return _f.eval(m,t,x,u);
               else 
	          return _f.eval(m,x,u);
	     } else {
               if (_time_dependant) 
	 	  return from_X(_f.eval(m,t,to_X(x),to_U(u)));
               else 
	          return from_X(_f.eval(m,to_X(x),to_U(u)));
             }
        }

        inline VectorType f_eval_diff(const Interval &t,
			  	     const IntervalVector& x, 
				     const IntervalVector& u) const override { 
             if constexpr(std::is_same_v<X,IntervalVector>
			&& std::is_same_v<U,IntervalVector>) {
               if (_time_dependant) 
	 	  return _f.eval_diff(t,x,u);
               else 
	          return _f.eval_diff(x,u);
	     } else {
               if (_time_dependant) {
	 	  XType ret = _f.eval_and_diff(t,to_X(x),to_U(u));
	          return { from_X(ret.m), 
		  	   from_X(ret.a), 
			   ret.da,
			   ret.def_domain };
	       }
               else { 
	          XType ret = _f.eval_and_diff(to_X(x),to_U(u));
	          return { from_X(ret.m), 
		  	   from_X(ret.a), 
			   ret.da,
			   ret.def_domain };
	       }
             }
        }

      protected:
        const AnalyticFunction<XType> _f;

        inline IntervalVector from_X(const X& x) const {
             if constexpr(std::is_same_v<X,Interval>) {
                return { x };
             } else {
                return x.template reshaped<Eigen::ColMajor>();
             }
        }
        inline Vector from_X(const XType::Scalar& x) const {
             if constexpr(std::is_same_v<X,Interval>) {
                return { x };
             } else {
                return x.template reshaped<Eigen::ColMajor>();
             }
        }
        inline X to_X(const IntervalVector& x) const {
             if constexpr(std::is_same_v<X,Interval>) {
                return  x[0];
             } else {
                return x.template reshaped<Eigen::ColMajor>
			(_x_shape.first,_x_shape.second);
             }
        }
        
        inline IntervalVector from_U(const U& u) const {
             if constexpr(std::is_same_v<U,Interval>) {
                return { u };
             } else {
                return u.template reshaped<Eigen::ColMajor>();
             }
        }
        inline U to_U(const IntervalVector& u) const {
             if constexpr(std::is_same_v<U,Interval>) {
                return  u[0];
             } else {
                return u.template reshaped<Eigen::ColMajor>
			(_u_shape.first,_u_shape.second);
             }
        }
        
    };
}
    

