/** 
 *  \file codac2_CtcInnerOuter.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <type_traits>
#include "codac2_Sep.h"
#include "codac2_Collection.h"
#include "codac2_template_tools.h"

namespace codac2
{
  class CtcInner : public Ctc_<IntervalVector>
  {
    public:

      template<typename S, typename = typename std::enable_if<
          std::is_base_of_v<Sep,S> || std::is_base_of_v<S,std::shared_ptr<Sep>>
        >::type>
      CtcInner(const S& s)
        : Ctc_<IntervalVector>(size_of(s)), _seps(s)
      { }

      std::shared_ptr<Ctc_<IntervalVector>> copy() const
      {
        return std::make_shared<CtcInner>(*this);
      }

      void contract(IntervalVector& x) const
      {
        auto x_sep = _seps.front().separate(x);
        x &= x_sep.inner;
      }

    protected:

      const Collection<Sep> _seps;
  };

  class CtcOuter : public Ctc_<IntervalVector>
  {
    public:

      template<typename S, typename = typename std::enable_if<
          std::is_base_of_v<Sep,S> || std::is_base_of_v<S,std::shared_ptr<Sep>>
        >::type>
      CtcOuter(const S& s)
        : Ctc_<IntervalVector>(size_of(s)), _seps(s)
      { }

      std::shared_ptr<Ctc_<IntervalVector>> copy() const
      {
        return std::make_shared<CtcOuter>(*this);
      }

      void contract(IntervalVector& x) const
      {
        auto x_sep = _seps.front().separate(x);
        x &= x_sep.outer;
      }

    protected:

      const Collection<Sep> _seps;
  };
}