/** 
 *  \file codac2_CtcDeriv.h
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Simon Rohou
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <numeric>
#include "codac2_Ctc.h"
#include "codac2_TimePropag.h"
#include "codac2_ConvexPolygon.h"
#include "codac2_TDomain.h"

namespace codac2
{
  template<class T>
  class Slice;

  template<typename T>
  class SlicedTube;
  
  class CtcDeriv
  {
    public:

      CtcDeriv(const TimePropag& time_propag = TimePropag::FWD_BWD, bool fast_mode = true);

      void restrict_tdomain(const Interval& tdomain)
      {
        _tdomain = tdomain;
      }

      template<typename T>
      inline void contract(Slice<T>& x, const Slice<T>& v, const std::vector<Index>& ctc_indices = {}) const
        requires std::is_same_v<T,Interval> || std::is_same_v<T,IntervalVector>
      {
        assert(x.size() == v.size());
        assert(x.t0_tf() == v.t0_tf());
        
        T input = x.input_gate();
        T output = x.output_gate();
        T envelope = x.codomain();

        if constexpr(std::is_same_v<T,Interval>)
          contract(x.t0_tf(), envelope, input, output, v.codomain(), _time_propag, _fast_mode);

        else if constexpr(std::is_same_v<T,IntervalVector>)
        {
          std::vector<Index> ctc_indices_(ctc_indices);
          if(ctc_indices_.empty())
          {
            // Then all the dimensions will be contracted
            ctc_indices_ = std::vector<Index>(x.size());
            std::iota(ctc_indices_.begin(), ctc_indices_.end(), 0);
          }

          for(auto i : ctc_indices_)
            contract(x.t0_tf(), envelope[i], input[i], output[i], v.codomain()[i], _time_propag, _fast_mode);
        }

        auto x_next = x.next_slice();
        if(x_next && x_next->is_gate())
          x_next->set(output, false);

        auto x_prev = x.prev_slice();
        if(x_prev && x_prev->is_gate())
          x_prev->set(input, false);

        x.set(envelope, false);
      }

      template<typename T>
      inline void contract(SlicedTube<T>& x, const SlicedTube<T>& v, const std::vector<Index>& ctc_indices = {}) const
        requires std::is_same_v<T,Interval> || std::is_same_v<T,IntervalVector>
      {
        assert_release(TDomain::are_same(x.tdomain(), v.tdomain()));
        Interval t = x.tdomain()->t0_tf() & _tdomain;
        auto it_beg = x.tdomain()->sample(t.lb(),true);
        auto it_end = x.tdomain()->sample(t.ub(),true);

        // Store gates locally during contraction to preserve boundary information
        // and ensure efficient propagation between slices.
        // Only explicitly defined gates are retained in the TDomain.
        // Temporary gates are discarded after contraction.
        std::vector<std::shared_ptr<Slice<T>>> sx;
        std::vector<std::shared_ptr<const Slice<T>>> sv;
        for(auto it = it_beg ; it != std::next(it_end) ; it++)
          if(!it->is_gate())
          {
            sx.push_back(x.slice(it));
            sv.push_back(v.slice(it));
          }

        if(sx.empty())
          return;

        std::vector<T> gates;
        gates.reserve(sx.size()+1);
        gates.push_back(sx.front()->input_gate());
        for(const auto& s : sx)
          gates.push_back(s->output_gate());

        auto contract_slice = [&](std::size_t i)
        {
          T envelope = sx[i]->codomain();

          if constexpr(std::is_same_v<T,Interval>)
            contract(sx[i]->t0_tf(), envelope, gates[i], gates[i+1],
              sv[i]->codomain(), _time_propag, _fast_mode);

          else if constexpr(std::is_same_v<T,IntervalVector>)
          {
            std::vector<Index> ctc_indices_(ctc_indices);
            if(ctc_indices_.empty())
            {
              ctc_indices_.resize(sx[i]->size());
              std::iota(ctc_indices_.begin(), ctc_indices_.end(), 0);
            }

            for(auto j : ctc_indices_)
              contract(sx[i]->t0_tf(), envelope[j], gates[i][j], gates[i+1][j],
                sv[i]->codomain()[j], _time_propag, _fast_mode);
          }

          sx[i]->set(envelope, false);
        };

        if((_time_propag & TimePropag::FWD) == TimePropag::FWD)
          for(std::size_t i = 0 ; i < sx.size() ; i++)
            contract_slice(i);

        if((_time_propag & TimePropag::BWD) == TimePropag::BWD)
          for(std::size_t i = sx.size() ; i-- > 0 ; )
            contract_slice(i);
          
        // Update only the gates explicitly defined in the TDomain.
        // Temporary gates are discarded after contraction.
        for(std::size_t i = 0 ; i < sx.size() ; i++)
        {
          auto prev = sx[i]->prev_slice();
          if(prev && prev->is_gate())
            prev->set(gates[i], false);

          auto next = sx[i]->next_slice();
          if(next && next->is_gate())
            next->set(gates[i+1], false);
        }
      }

      static ConvexPolygon polygon_slice(
        const Interval& t, const Interval& envelope,
        const Interval& input, const Interval& proj_input,
        const Interval& output, const Interval& proj_output,
        const Interval& v);

    protected:

      static void contract(
        const Interval& t, Interval& envelope,
        Interval& input, Interval& output,
        const Interval& v, const TimePropag& time_propag, bool fast_mode);

      const TimePropag _time_propag;
      const bool _fast_mode;
      Interval _tdomain;
  };
}