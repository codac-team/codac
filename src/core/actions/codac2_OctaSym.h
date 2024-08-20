/** 
 *  \file codac2_OctaSym.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Luc Jaulin
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <vector>
#include "codac2_Vector.h"
#include "codac2_IntervalVector.h"
#include "codac2_CtcWrapper.h"

namespace codac2
{
  class CtcAction;
  class Sep;
  class SepAction;
  class SetExpr;

  /**
   * \class Action
   */
  class Action
  {

  };

  /**
   * \class OctaSym
   */
  class OctaSym : public std::vector<int>, public Action
  {
    public:

      OctaSym(std::initializer_list<int> s);
      OctaSym(const std::vector<int>& s);

      OctaSym invert() const;

      IntervalVector operator()(const IntervalVector& x) const;

      template<typename C, typename = typename std::enable_if<
          std::is_base_of_v<Ctc_<IntervalVector>,C>
        >::type>
      CtcAction operator()(const C& c) const;
      // -> is defined in CtcAction class

      template<typename S, typename = typename std::enable_if<
          std::is_base_of_v<Sep,S>
        >::type>
      SepAction operator()(const S& s) const;
      // -> is defined in SepAction class

      std::shared_ptr<SetExpr> operator()(const std::shared_ptr<SetExpr>& x1) const;
      // -> is defined in set operations file

      friend std::ostream& operator<<(std::ostream& str, const OctaSym& s);
  };

  OctaSym operator*(const OctaSym& s1, const OctaSym& s2);
}