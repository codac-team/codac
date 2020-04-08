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
#include "tubex_Polygon.h"

namespace tubex
{
  const Polygon operator+(const Polygon& x);
  const Polygon operator+(const Polygon& x, const ibex::IntervalVector& v);
  const Polygon operator+(const ibex::IntervalVector& v, const Polygon& x);

  const Polygon operator-(const Polygon& x);
  const Polygon operator-(const Polygon& x, const ibex::IntervalVector& v);
  const Polygon operator-(const ibex::IntervalVector& v, const Polygon& x);

  const Polygon operator*(const ibex::IntervalMatrix& m, const Polygon& x);
}

#endif