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
   * \class OctaSym
   */
  class OctaSym : public std::vector<int>
  {
    public:

      OctaSym(std::initializer_list<int> s) : std::vector<int>(s) {}
  };

  /**
   * \class CtcAction
   */
  class Action
  {
    public:

      Action(const OctaSym& s);
      Action(const OctaSym& s, const OctaSym& _s);
      codac::IntervalVector operator()(const codac::IntervalVector& x) const;
      CtcAction operator()(codac::Ctc& ctc) const;

    protected:

      static codac::Interval sign(int a)
      {
        return (a > 0) ? 1 : ((a < 0) ? -1 : 0);
      }

      const OctaSym _s, __s;
  };
}

#endif