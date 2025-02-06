/** 
 *  \file codac2_Wrapper.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_Interval.h"
#include "codac2_Vector.h"
#include "codac2_Matrix.h"
#include "codac2_IntervalVector.h"
#include "codac2_IntervalMatrix.h"

namespace codac2
{
  template<typename T>
  struct Wrapper
  { };

  template<>
  struct Wrapper<int> {
    using Domain = Interval;
  };

  template<>
  struct Wrapper<Index> {
    using Domain = Interval;
  };

  template<>
  struct Wrapper<double> {
    using Domain = Interval;
  };

  template<>
  struct Wrapper<Interval> {
    using Domain = Interval;
  };

  template<>
  struct Wrapper<Vector> {
    using Domain = IntervalVector;
  };

  template<>
  struct Wrapper<IntervalVector> {
    using Domain = IntervalVector;
  };

  template<>
  struct Wrapper<Matrix> {
    using Domain = IntervalMatrix;
  };

  template<>
  struct Wrapper<IntervalMatrix> {
    using Domain = IntervalMatrix;
  };
}