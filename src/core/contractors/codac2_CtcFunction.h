/** 
 *  \file
 *  CtcFunction class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC2_CTCFUNCTION__
#define __CODAC2_CTCFUNCTION__

#include <map>
#include "codac2_Function.h"

namespace codac2
{
  template<typename T>
  class CtcFunction
  {
    public:

      CtcFunction(const Function<T>& f, const T& y)
        : _f(f), _y(y)
      { }

      template<typename... X>
      void contract(X&... x) const
      {
        ValuesMap v;
        _f.fill_from_args(v, x...);

        // Forward/backward algorithm:
        _f.expr()->fwd_eval(v);
        _f.expr()->value(v) &= _y;
        _f.expr()->bwd_eval(v);

        _f.intersect_from_args(v, x...);
      }

    protected:

      const Function<T> _f;
      const T _y;
  };

  template<typename T>
  class CtcFunctionOnBox : public BoxCtc, public CtcFunction<T>
  {
    public:

      CtcFunctionOnBox(const Function<T>& f, const T& y)
        : CtcFunction<T>(f,y)
      {
        assert(f.args().size() == 1);
      }

      void contract(IntervalVector& x) const
      {
        CtcFunction<T>::contract(x);
      }
  };
}

#endif