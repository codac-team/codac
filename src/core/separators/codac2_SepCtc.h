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
#include "codac2_CollectionSep.h"

namespace codac2
{
  class SepCtcIn : public Ctc_<IntervalVector>
  {
    public:

      template<typename S, // S should be some Sep class
        typename = typename std::enable_if<std::is_base_of<Sep,S>::value>::type>
      SepCtcIn(const S& s)
       : _seps(s)
      { }

      SepCtcIn(const SepCtcIn& s)
        : _seps(s._seps)
      { }

      virtual std::shared_ptr<Ctc> copy() const
      {
        return std::make_shared<SepCtcIn>(*this);
      }

      void contract(IntervalVector& x) const
      {
        auto x_sep = _seps._v_sep.front()->separate(x);
        x &= x_sep.in;
      }

    protected:

      const CollectionSep _seps;
  };

  class SepCtcOut : public Ctc_<IntervalVector>
  {
    public:

      template<typename S, // S should be some Sep class
        typename = typename std::enable_if<std::is_base_of<Sep,S>::value>::type>
      SepCtcOut(const S& s)
       : _seps(s)
      { }

      SepCtcOut(const SepCtcOut& s)
        : _seps(s._seps)
      { }

      virtual std::shared_ptr<Ctc> copy() const
      {
        return std::make_shared<SepCtcOut>(*this);
      }

      void contract(IntervalVector& x) const
      {
        auto x_sep = _seps._v_sep.front()->separate(x);
        x &= x_sep.out;
      }

    protected:

      const CollectionSep _seps;
  };
}