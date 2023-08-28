/** 
 *  \file
 *  AbstractDomain class
 * ----------------------------------------------------------------------------
 *  \date       2022
 *  \author     Damien Massé
 *  \copyright  Copyright 2022 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC2_ABSTRACTDOMAIN_H__
#define __CODAC2_ABSTRACTDOMAIN_H__

#include "codac_IntervalVector.h"

namespace codac2
{
  using codac::IntervalVector;

  /**
   * \class AbstractDomain
   * \brief ...
   */
  class AbstractDomain
  {
    public:

      AbstractDomain();
      codac::IntervalVector box() const;

    protected:

  };
}

#endif