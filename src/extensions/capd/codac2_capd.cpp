/**
 *  codac2_capd.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Maël Godard
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <cassert>
#include "codac2_capd.h"

using namespace std;

namespace codac2
{
  capd::Interval to_capd(const Interval &x)
  {
    return {x.lb(), x.ub()};
  }

  Interval to_codac(const capd::Interval &x)
  {
    return {x.leftBound(), x.rightBound()};
  }

  capd::IVector to_capd(const IntervalVector &x)
  {
    capd::IVector y(x.size());
    for (Index i = 0; i < (Index)x.size(); i++)
    {
      y[i] = to_capd(x[i]);
    }
    return y;
  }

  IntervalVector to_codac(const capd::IVector &x)
  {
    IntervalVector y(x.dimension());
    for (Index i = 0; i < (Index)x.dimension(); i++)
    {
      y[i] = to_codac(x[i]);
    }
    return y;
  }

  capd::IMatrix to_capd(const IntervalMatrix &x)
  {
    capd::IMatrix y(x.rows(), x.cols());
    for (Index i = 0; i < x.rows(); i++)
      for (Index j = 0; j < x.cols(); j++)
        y[i][j] = to_capd(x(i, j));
    return y;
  }

  IntervalMatrix to_codac(const capd::IMatrix &x)
  {
    IntervalMatrix y(x.numberOfRows(), x.numberOfColumns());
    for (Index i = 0; i < (Index)x.numberOfRows(); i++)
      for (Index j = 0; j < (Index)x.numberOfColumns(); j++)
        y(i, j) = to_codac(x[i][j]);
    return y;
  }


}
