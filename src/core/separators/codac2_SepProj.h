/** 
 *  \file
 *  SepProj class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Benoit Desrochers
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#pragma once

#include "codac2_Sep.h"
#include "codac2_Collection.h"

namespace codac2
{
  class SepProj : public Sep
  {
    public:

      template<typename S, typename = typename std::enable_if<
          std::is_base_of_v<Sep,S>
        >::type>
      SepProj(const S& s, const IntervalVector& y)
        : _sep(s), _y(y)
      { }

      virtual std::shared_ptr<Sep> copy() const;
      BoxPair separate(const IntervalVector& x) const;
      BoxPair separate(const IntervalVector& x, double eps) const;

    protected:

      IntervalVector extract_x(const IntervalVector& w) const;
      IntervalVector extract_y(const IntervalVector& w) const;

      const Collection<Sep> _sep;
      const IntervalVector _y;
  };
}