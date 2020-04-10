/** 
 *  \file
 *  Arithmetic operations on polygons
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_POLYGON_ARITHMETIC_H__
#define __TUBEX_POLYGON_ARITHMETIC_H__

#include "ibex_IntervalVector.h"
#include "ibex_IntervalMatrix.h"
#include "tubex_ConvexPolygon.h"

namespace tubex
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