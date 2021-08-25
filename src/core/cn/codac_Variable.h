/** 
 *  \file
 *  Variable class
 * ----------------------------------------------------------------------------
 *  \date       2021
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_VARIABLE_H__
#define __CODAC_VARIABLE_H__

#include "codac_Interval.h"
#include "codac_Domain.h"

namespace codac
{
  class Variable : public Domain
  {
    public:

      Variable(const Interval& i);
      Variable(const IntervalVector& iv);
      Variable(const Tube& t);
      Variable(const TubeVector& tv);


    protected:

  };
}

#endif