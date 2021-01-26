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

#include "ibex_IntervalVector.h"
#include "ibex_IntervalMatrix.h"
#include "codac_ConvexPolygon.h"

namespace codac
{
  const ConvexPolygon operator+(const ConvexPolygon& x);
  const ConvexPolygon operator+(const ConvexPolygon& x, const ibex::IntervalVector& v);
  const ConvexPolygon operator+(const ibex::IntervalVector& v, const ConvexPolygon& x);

  const ConvexPolygon operator-(const ConvexPolygon& x);
  const ConvexPolygon operator-(const ConvexPolygon& x, const ibex::IntervalVector& v);
  const ConvexPolygon operator-(const ibex::IntervalVector& v, const ConvexPolygon& x);

  const ConvexPolygon operator*(const ibex::IntervalMatrix& m, const ConvexPolygon& x);

  const ConvexPolygon operator&(const ConvexPolygon& p1, const ConvexPolygon& p2);
  const ConvexPolygon operator&(const ibex::IntervalVector& p1, const ConvexPolygon& p2);
  const ConvexPolygon operator&(const ConvexPolygon& p1, const ibex::IntervalVector& p2);
}

#endif