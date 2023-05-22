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

#ifndef __CODAC2_CTCACTION_H__
#define __CODAC2_CTCACTION_H__

#include "codac2_Action.h"

namespace codac2
{
  /**
   * \class CtcAction
   */
  class CtcAction : public codac::Ctc
  {
    public:

      CtcAction(Ctc& ctc, const OctaSym& s);
      void contract(codac::IntervalVector& x);

    protected:

      codac::Ctc& _ctc;
      const OctaSym _s, __s;
  };
}

#endif