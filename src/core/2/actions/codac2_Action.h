/** 
 *  \file
 *
 * ----------------------------------------------------------------------------
 *  \date       2023
 *  \author     Simon Rohou, Luc Jaulin
 *  \copyright  Copyright 2022 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC2_ACTION_H__
#define __CODAC2_ACTION_H__

#include <vector>
#include "codac_IntervalVector.h"
#include "codac_Ctc.h"

namespace codac2
{
  class CtcAction;

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
      CtcAction operator()(codac::Ctc& ctc) const;
      codac::IntervalVector operator()(const codac::IntervalVector& x) const;
      OctaSym invert() const;

      friend std::ostream& operator<<(std::ostream& str, const OctaSym& s);
  };
  
  OctaSym operator*(const OctaSym& s1, const OctaSym& s2);
}

#endif