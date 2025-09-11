/** 
 *  \file codac2_SolutionCurveWrapper.h
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <capd/capdlib.h>
#include "codac2_assert.h"

namespace codac2
{
  class SolutionCurveWrapper : public capd::ITimeMap::SolutionCurve
  {
    public:

      SolutionCurveWrapper(const capd::ITimeMap::SolutionCurve& base);

      std::vector<capd::Interval> get_domain() const;

      double t0() const;

      double tf() const;

      int dimension() const;

  };
}