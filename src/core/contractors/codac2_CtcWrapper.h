/** 
 *  \file
 *  CtcWrapper class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC2_CTCWRAPPER__
#define __CODAC2_CTCWRAPPER__

#include "codac2_Ctc.h"

namespace codac2
{
  template<typename T>
  class Ctc_ : public Ctc
  {
    public:

      virtual void contract(T& x) const = 0;
  };

  template<typename T>
  class CtcWrapper_ : public Ctc_<T>
  {
    public:

      CtcWrapper_(const T& y)
        : _y(y)
      { }

      void contract(T& x) const
      {
        x &= _y;
      }

    protected:

      const T _y;
  };
}

#endif