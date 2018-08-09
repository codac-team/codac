/* ============================================================================
 *  tubex-lib - Arithmetic
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef __TUBEX_ARITHMETIC_H__
#define __TUBEX_ARITHMETIC_H__

#include "ibex_Interval.h"
#include "ibex_IntervalVector.h"
#include "tubex_Tube.h"
#include "tubex_TubeVector.h"

namespace tubex
{
  // Scalar outputs

    const Tube cos(const Tube& x);
    const Tube sin(const Tube& x);
    const Tube abs(const Tube& x);
    const Tube sqr(const Tube& x);
    const Tube sqrt(const Tube& x);
    const Tube exp(const Tube& x);
    const Tube log(const Tube& x);
    const Tube tan(const Tube& x);
    const Tube acos(const Tube& x);
    const Tube asin(const Tube& x);
    const Tube atan(const Tube& x);
    const Tube cosh(const Tube& x);
    const Tube sinh(const Tube& x);
    const Tube tanh(const Tube& x);
    const Tube acosh(const Tube& x);
    const Tube asinh(const Tube& x);
    const Tube atanh(const Tube& x);

    const Tube atan2(const Tube& x1, const Tube& x2);
    const Tube atan2(const Tube& x1, const ibex::Interval& x2);
    const Tube atan2(const ibex::Interval& x1, const Tube& x2);

    const Tube pow(const Tube& x, int p);
    const Tube pow(const Tube& x, double p);
    const Tube pow(const Tube& x, const ibex::Interval& p);
    const Tube root(const Tube& x, int p);

    const Tube operator+(const Tube& x);
    const Tube operator+(const Tube& x1, const Tube& x2);
    const Tube operator+(const Tube& x1, const ibex::Interval& x2);
    const Tube operator+(const ibex::Interval& x1, const Tube& x2);

    const Tube operator-(const Tube& x);
    const Tube operator-(const Tube& x1, const Tube& x2);
    const Tube operator-(const Tube& x1, const ibex::Interval& x2);
    const Tube operator-(const ibex::Interval& x1, const Tube& x2);

    const Tube operator*(const Tube& x1, const Tube& x2);
    const Tube operator*(const Tube& x1, const ibex::Interval& x2);
    const Tube operator*(const ibex::Interval& x1, const Tube& x2);

    const Tube operator/(const Tube& x1, const Tube& x2);
    const Tube operator/(const Tube& x1, const ibex::Interval& x2);
    const Tube operator/(const ibex::Interval& x1, const Tube& x2);

    const Tube operator|(const Tube& x1, const Tube& x2);
    const Tube operator|(const Tube& x1, const ibex::Interval& x2);
    const Tube operator|(const ibex::Interval& x1, const Tube& x2);

    const Tube operator&(const Tube& x1, const Tube& x2);
    const Tube operator&(const Tube& x1, const ibex::Interval& x2);
    const Tube operator&(const ibex::Interval& x1, const Tube& x2);

  // Vector outputs

    const TubeVector operator+(const TubeVector& x);
    const TubeVector operator+(const TubeVector& x1, const TubeVector& x2);
    const TubeVector operator+(const TubeVector& x1, const ibex::IntervalVector& x2);
    const TubeVector operator+(const ibex::IntervalVector& x1, const TubeVector& x2);

    const TubeVector operator-(const TubeVector& x);
    const TubeVector operator-(const TubeVector& x1, const TubeVector& x2);
    const TubeVector operator-(const TubeVector& x1, const ibex::IntervalVector& x2);
    const TubeVector operator-(const ibex::IntervalVector& x1, const TubeVector& x2);

    const TubeVector operator*(const ibex::Interval& x1, const TubeVector& x2);
    const TubeVector operator*(const Tube& x1, const ibex::IntervalVector& x2);
    const TubeVector operator*(const Tube& x1, const TubeVector& x2);

    const TubeVector operator|(const TubeVector& x1, const TubeVector& x2);
    const TubeVector operator|(const TubeVector& x1, const ibex::IntervalVector& x2);
    const TubeVector operator|(const ibex::IntervalVector& x1, const TubeVector& x2);

    const TubeVector operator&(const TubeVector& x1, const TubeVector& x2);
    const TubeVector operator&(const TubeVector& x1, const ibex::IntervalVector& x2);
    const TubeVector operator&(const ibex::IntervalVector& x1, const TubeVector& x2);
}

#endif