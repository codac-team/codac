/** 
 *  \file
 *  CtcDistance class
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_CTCDISTANCE_H__
#define __TUBEX_CTCDISTANCE_H__

#include "tubex_CtcFunction.h"

namespace tubex
{
  /**
   * \class CtcDistance
   */
  class CtcDistance : public CtcFunction
  {
    public:

      CtcDistance();

      void contract(std::vector<Domain*>& v_domains);
  };
}

#endif