/** 
 *  \file
 *  Arithmetic operations on polygons
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_POLYGON_ARITHMETIC_H__
#define __CODAC_POLYGON_ARITHMETIC_H__

#include "codac_IntervalVector.h"
#include "codac_IntervalMatrix.h"
#include "codac_ConvexPolygon.h"

namespace codac
{
  const ConvexPolygon operator+(const ConvexPolygon& x);
  const ConvexPolygon operator+(const ConvexPolygon& x, const IntervalVector& v);
  const ConvexPolygon operator+(const IntervalVector& v, const ConvexPolygon& x);

  const ConvexPolygon operator-(const ConvexPolygon& x);
  const ConvexPolygon operator-(const ConvexPolygon& x, const IntervalVector& v);
  const ConvexPolygon operator-(const IntervalVector& v, const ConvexPolygon& x);

  const ConvexPolygon operator*(const IntervalMatrix& m, const ConvexPolygon& x);

  std::vector<ThickPoint> inter_thickpoints(const ConvexPolygon& p1, const ConvexPolygon& p2);
  const ConvexPolygon operator&(const ConvexPolygon& p1, const ConvexPolygon& p2);
  const ConvexPolygon operator&(const IntervalVector& p1, const ConvexPolygon& p2);
  const ConvexPolygon operator&(const ConvexPolygon& p1, const IntervalVector& p2);

  const ConvexPolygon operator|(const ConvexPolygon& p1, const ConvexPolygon& p2);
  const ConvexPolygon operator|(const IntervalVector& p1, const ConvexPolygon& p2);
  const ConvexPolygon operator|(const ConvexPolygon& p1, const IntervalVector& p2);
}

#endif