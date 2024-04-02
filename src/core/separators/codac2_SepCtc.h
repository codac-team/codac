/** 
 *  \file
 *  SepCtc* class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#pragma once

#include <type_traits>
#include "codac2_Sep.h"
#include "codac2_Collection.h"

namespace codac2
{
  class SepCtcIn : public Ctc_<IntervalVector>
  {
    public:

      template<typename S, typename = typename std::enable_if<
          std::is_base_of_v<Sep,S>
        >::type>
      SepCtcIn(const S& s)
        : _seps(s)
      { }

      virtual std::shared_ptr<Ctc> copy() const
      {
        return std::make_shared<SepCtcIn>(*this);
      }

      void contract(IntervalVector& x) const
      {
        auto x_sep = _seps.front().separate(x);
        x &= x_sep.in;
      }

    protected:

      const Collection<Sep> _seps;
  };

  class SepCtcOut : public Ctc_<IntervalVector>
  {
    public:

      template<typename S, typename = typename std::enable_if<
          std::is_base_of_v<Sep,S>
        >::type>
      SepCtcOut(const S& s)
        : _seps(s)
      { }

      virtual std::shared_ptr<Ctc> copy() const
      {
        return std::make_shared<SepCtcOut>(*this);
      }

      void contract(IntervalVector& x) const
      {
        auto x_sep = _seps.front().separate(x);
        x &= x_sep.out;
      }

    protected:

      const Collection<Sep> _seps;
  };
}