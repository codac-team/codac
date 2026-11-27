/** 
 *  \file codac2_CtcEval.h
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Simon Rohou
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <utility>
#include "codac2_Ctc.h"
#include "codac2_SlicedTube.h"

namespace codac2
{
  /**
   * \class CtcEval
   * \brief Evaluation contractor \f$\mathcal{C}_\textrm{eval}\f$ for scalar or vector tubes.
   *
   * Given a tube \f$[x](\cdot)\f$, a derivative tube \f$[v](\cdot)\f$
   * and an observation \f$[t]\times[z]\f$, the contractor reduces
   * \f$[t]\f$, \f$[z]\f$ and \f$[x](\cdot)\f$ subject to
   * \f[
   *   z=x(t),\qquad \dot{x}(\cdot)=v(\cdot)
   * \f]
   * The derivative tube is not contracted.
   */
  class CtcEval
  {
    public:

      /**
       * \brief Creates the evaluation contractor \f$\mathcal{C}_\textrm{eval}\f$.
       */
      CtcEval()
      { }

      /**
       * \brief \f$\mathcal{C}_\textrm{eval}\big([t],[z],[x](\cdot),[v](\cdot)\big)\f$:
       *        contracts the observation \f$[t]\times[z]\f$ and the tube \f$[x](\cdot)\f$
       *        subject to \f$z=x(t)\f$ and \f$\dot{x}(\cdot)=v(\cdot)\f$.
       *
       * \tparam T scalar or vector interval type
       * \param t temporal interval \f$[t]\f$ of the evaluation
       * \param z interval or box \f$[z]\f$ enclosing the evaluation
       * \param x scalar or vector tube \f$[x](\cdot)\f$
       * \param v derivative tube \f$[v](\cdot)\f$
       * \note The tubes must share the same TDomain, which may be refined by adding gates
       */
      template<typename T>
      void contract(Interval& t, T& z, SlicedTube<T>& x, const SlicedTube<T>& v) const
        requires std::is_same_v<T,Interval> || std::is_same_v<T,IntervalVector>
      {
        contract_impl<T>(t,z,x,v,nullptr);
      }

      /**
       * \brief \f$\mathcal{C}_\textrm{eval}\big([t],[z],[x](\cdot),[v](\cdot)\big)\f$:
       *        contracts the observation \f$[t]\times[z]\f$ and the tube \f$[x](\cdot)\f$
       *        subject to \f$z=x(t)\f$ and \f$\dot{x}(\cdot)=v(\cdot)\f$,
       *        using precomputed partial primitives of the derivative tube.
       *
       * For \f$[v](\cdot)=[v^-(\cdot),v^+(\cdot)]\f$, the provided tubes enclose
       * the partial primitives
       * \f[
       *   p^-(t)=\int_{t_0}^{t}v^-(s)\,ds,\qquad
       *   p^+(t)=\int_{t_0}^{t}v^+(s)\,ds
       * \f]
       *
       * \tparam T scalar or vector interval type
       * \param t temporal interval \f$[t]\f$ of the evaluation
       * \param z interval or box \f$[z]\f$ enclosing the evaluation
       * \param x scalar or vector tube \f$[x](\cdot)\f$
       * \param v derivative tube \f$[v](\cdot)\f$
       * \param primitives pair of tubes returned by ``v.partial_primitives()``
       * \note All tubes must share the same TDomain, which may be refined by adding gates
       * \note The primitives can be reused after temporal refinement by duplication of slices,
       *       provided the derivative envelopes and temporal extent remain unchanged
       */
      template<typename T>
      void contract(Interval& t, T& z, SlicedTube<T>& x, const SlicedTube<T>& v,
        const std::pair<SlicedTube<T>,SlicedTube<T>>& primitives) const
        requires std::is_same_v<T,Interval> || std::is_same_v<T,IntervalVector>
      {
        assert_release(x.tdomain() == v.tdomain());
        assert_release(primitives.first.tdomain() == v.tdomain());
        assert_release(primitives.second.tdomain() == v.tdomain());
        contract_impl(t,z,x,v,&primitives);
      }

    private:

      template<typename T>
      void contract_impl(Interval& t, T& z, SlicedTube<T>& x, const SlicedTube<T>& v,
        const std::pair<SlicedTube<T>,SlicedTube<T>>* primitives) const
      {
        assert_release(TDomain::are_same(x.tdomain(), v.tdomain()));
        assert_release(x.size() == v.size());
        assert_release(z.size() == x.size());

        if(t.is_empty() || z.is_empty() || x.is_empty() || v.is_empty())
        {
          t.set_empty();
          z.set_empty();
          x.set(x.empty_value());
          return;
        }

        t &= x.invert(z, v, t);

        if(!t.is_empty())
        {
          z &= x(t, v);

          if(t.is_degenerated())
          {
            x.set(x(t.lb()) & z, t.lb());
            // This will create a new gate at t
            return;
          }

          // Sampling the tube with the creation of two gates
          auto it_t_beg = x.tdomain()->sample(t.lb(),true);
          auto it_t = it_t_beg;
          auto it_t_end = std::next(x.tdomain()->sample(t.ub(),true));

          // For each t in [t]
          while(it_t != it_t_end)
          {
            if(it_t->is_gate())
            {
              T slices_union = x.empty_value();

              // For each t1 in [t]
              auto it_t1 = it_t_beg;
              auto it_t1_end = it_t_end;
              while(it_t1 != it_t1_end)
              {
                if(it_t1->is_gate())
                {
                  T y1 = x(it_t1->lb()) & z;

                  if(!y1.is_empty())
                  {
                    // Transporting y1 to t
                    if(it_t1 != it_t)
                    {
                      if(primitives)
                        y1 += v.integral(Interval(it_t1->lb()),Interval(it_t->lb()),*primitives);
                      else
                        y1 += v.integral(Interval(it_t1->lb()),Interval(it_t->lb()));
                    }

                    slices_union |= y1;
                  }
                }

                it_t1++;
              }

              x.slice(it_t)->set(slices_union);
            }

            it_t++;
          }

          CtcDeriv ctc_deriv(TimePropag::FWD_BWD, true);
          ctc_deriv.restrict_tdomain(t);
          ctc_deriv.contract(x,v);
        }

        if(t.is_empty() || z.is_empty() || x.is_empty())
        {
          t.set_empty();
          z.set_empty();
          x.set(x.empty_value());
        }
      }
  };
}