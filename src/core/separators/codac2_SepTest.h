/** 
 *  \file codac2_SepTest.h
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Maël Godard
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <type_traits>
#include "codac2_Sep.h"
#include "codac2_Collection.h"
#include "codac2_template_tools.h"

namespace codac2
{
  /**
   * \brief A separator which tests the result of another separator. 
   * It can be used to get the result of the separator without the contraction.
   */
  class SepTest : public Sep<SepTest>
  {
    public:

      template<typename S>
        requires IsSepBaseOrPtr<S>
      SepTest(const S& s)
        : Sep<SepTest>(size_of(s)), _sep(s)
      { }

      BoxPair separate(const IntervalVector& x) const;

    protected:

      const Collection<SepBase>& _sep;
  };
}