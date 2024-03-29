/** 
 *  \file
 *  Sep class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC2_SEP__
#define __CODAC2_SEP__

#include <map>
#include "codac2_IntervalVector.h"
#include "codac2_CtcInverse.h"

namespace codac2
{
  struct BoxPair
  {
    IntervalVector in;
    IntervalVector out;
  };

  class Sep
  {
    public:

      virtual BoxPair separate(const IntervalVector& x) const = 0;
  };
  
  class SepCtcIn : public Ctc_<IntervalVector>
  {
    public:

      SepCtcIn(const Sep& s)
        : _s(s)
      { }

      void contract(IntervalVector& x) const
      {
        auto x_sep = _s.separate(x);
        x &= x_sep.in;
      }

    protected:

      const Sep& _s;
  };

  class SepCtcOut : public Ctc_<IntervalVector>
  {
    public:

      SepCtcOut(const Sep& s)
        : _s(s)
      { }

      void contract(IntervalVector& x) const
      {
        auto x_sep = _s.separate(x);
        x &= x_sep.out;
      }

    protected:

      const Sep& _s;
  };
}

#endif