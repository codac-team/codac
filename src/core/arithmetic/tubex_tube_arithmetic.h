/** 
 *  \file
 *  Arithmetic operations on tubes
 * ----------------------------------------------------------------------------
 *  \date       2015
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_TUBE_ARITHMETIC_H__
#define __TUBEX_TUBE_ARITHMETIC_H__

#include "ibex_Interval.h"
#include "ibex_IntervalVector.h"
#include "tubex_Tube.h"
#include "tubex_TubeVector.h"

namespace tubex
{
  /// \name Scalar outputs
  /// @{

    /** \brief \f$\cos([x](\cdot))\f$ */
    const Tube cos(const Tube& x);
    /** \brief \f$\sin([x](\cdot))\f$ */
    const Tube sin(const Tube& x);
    /** \brief \f$\mid[x](\cdot)\mid\f$ */
    const Tube abs(const Tube& x);
    /** \brief \f$[x]^2(\cdot)\f$ */
    const Tube sqr(const Tube& x);
    /** \brief \f$\sqrt{[x](\cdot)}\f$ */
    const Tube sqrt(const Tube& x);
    /** \brief \f$\exp([x](\cdot))\f$ */
    const Tube exp(const Tube& x);
    /** \brief \f$\log([x](\cdot))\f$ */
    const Tube log(const Tube& x);
    /** \brief \f$\tan([x](\cdot))\f$ */
    const Tube tan(const Tube& x);
    /** \brief \f$\arccos([x](\cdot))\f$ */
    const Tube acos(const Tube& x);
    /** \brief \f$\arcsin([x](\cdot))\f$ */
    const Tube asin(const Tube& x);
    /** \brief \f$\arctan([x](\cdot))\f$ */
    const Tube atan(const Tube& x);
    /** \brief \f$\cosh([x](\cdot))\f$ */
    const Tube cosh(const Tube& x);
    /** \brief \f$\sinh([x](\cdot))\f$ */
    const Tube sinh(const Tube& x);
    /** \brief \f$\tanh([x](\cdot))\f$ */
    const Tube tanh(const Tube& x);
    /** \brief \f$\mathrm{arccosh}([x](\cdot))\f$ */
    const Tube acosh(const Tube& x);
    /** \brief \f$\mathrm{arcsinh}([x](\cdot))\f$ */
    const Tube asinh(const Tube& x);
    /** \brief \f$\mathrm{arctanh}([x](\cdot))\f$ */
    const Tube atanh(const Tube& x);

    /** \brief \f$\mathrm{arctan2}([y](\cdot),[x](\cdot))\f$ */
    const Tube atan2(const Tube& y, const Tube& x);
    /** \brief \f$\mathrm{arctan2}([y](\cdot),[x])\f$ */
    const Tube atan2(const Tube& y, const ibex::Interval& x);
    /** \brief \f$\mathrm{arctan2}([y],[x](\cdot))\f$ */
    const Tube atan2(const ibex::Interval& y, const Tube& x);

    /** \brief \f$[x]^p(\cdot)\f$ */
    const Tube pow(const Tube& x, int p);
    /** \brief \f$[x]^p(\cdot)\f$ */
    const Tube pow(const Tube& x, double p);
    /** \brief \f$[x]^{[p]}(\cdot)\f$ */
    const Tube pow(const Tube& x, const ibex::Interval& p);
    /** \brief \f$\sqrt[p]{[x](\cdot)}\f$ */
    const Tube root(const Tube& x, int p);

    // todo: atan2, pow with Trajectory as parameter

    /** \brief \f$[x](\cdot)\f$ */
    const Tube operator+(const Tube& x);
    /** \brief \f$[x](\cdot)+[y](\cdot)\f$ */
    const Tube operator+(const Tube& x, const Tube& y);
    /** \brief \f$[x](\cdot)+[y]\f$ */
    const Tube operator+(const Tube& x, const ibex::Interval& y);
    /** \brief \f$[x]+[y](\cdot)\f$ */
    const Tube operator+(const ibex::Interval& x, const Tube& y);
    /** \brief \f$[x](\cdot)+y(\cdot)\f$ */
    const Tube operator+(const Tube& x, const Trajectory& y);
    /** \brief \f$x(\cdot)+[y](\cdot)\f$ */
    const Tube operator+(const Trajectory& x, const Tube& y);

    /** \brief \f$-[x](\cdot)\f$ */
    const Tube operator-(const Tube& x);
    /** \brief \f$[x](\cdot)-[y](\cdot)\f$ */
    const Tube operator-(const Tube& x, const Tube& y);
    /** \brief \f$[x](\cdot)-[y]\f$ */
    const Tube operator-(const Tube& x, const ibex::Interval& y);
    /** \brief \f$[x]-[y](\cdot)\f$ */
    const Tube operator-(const ibex::Interval& x, const Tube& y);
    /** \brief \f$[x](\cdot)-y(\cdot)\f$ */
    const Tube operator-(const Tube& x, const Trajectory& y);
    /** \brief \f$x(\cdot)-[y](\cdot)\f$ */
    const Tube operator-(const Trajectory& x, const Tube& y);

    /** \brief \f$[x](\cdot)\cdot[y](\cdot)\f$ */
    const Tube operator*(const Tube& x, const Tube& y);
    /** \brief \f$[x](\cdot)\cdot[y]\f$ */
    const Tube operator*(const Tube& x, const ibex::Interval& y);
    /** \brief \f$[x]\cdot[y](\cdot)\f$ */
    const Tube operator*(const ibex::Interval& x, const Tube& y);
    /** \brief \f$[x](\cdot)\cdot y(\cdot)\f$ */
    const Tube operator*(const Tube& x, const Trajectory& y);
    /** \brief \f$x(\cdot)\cdot[y](\cdot)\f$ */
    const Tube operator*(const Trajectory& x, const Tube& y);

    /** \brief \f$[x](\cdot)/[y](\cdot)\f$ */
    const Tube operator/(const Tube& x, const Tube& y);
    /** \brief \f$[x](\cdot)/[y]\f$ */
    const Tube operator/(const Tube& x, const ibex::Interval& y);
    /** \brief \f$[x]/[y](\cdot)\f$ */
    const Tube operator/(const ibex::Interval& x, const Tube& y);
    /** \brief \f$[x](\cdot)/y(\cdot)\f$ */
    const Tube operator/(const Tube& x, const Trajectory& y);
    /** \brief \f$x(\cdot)/[y](\cdot)\f$ */
    const Tube operator/(const Trajectory& x, const Tube& y);

    /** \brief \f$[x](\cdot)\sqcup[y](\cdot)\f$ */
    const Tube operator|(const Tube& x, const Tube& y);
    /** \brief \f$[x](\cdot)\sqcup[y]\f$ */
    const Tube operator|(const Tube& x, const ibex::Interval& y);
    /** \brief \f$[x]\sqcup[y](\cdot)\f$ */
    const Tube operator|(const ibex::Interval& x, const Tube& y);
    /** \brief \f$[x](\cdot)\sqcup y(\cdot)\f$ */
    const Tube operator|(const Tube& x, const Trajectory& y);
    /** \brief \f$x(\cdot)\sqcup [y](\cdot)\f$ */
    const Tube operator|(const Trajectory& x, const Tube& y);

    /** \brief \f$[x](\cdot)\cap[y](\cdot)\f$ */
    const Tube operator&(const Tube& x, const Tube& y);
    /** \brief \f$[x](\cdot)\cap[y]\f$ */
    const Tube operator&(const Tube& x, const ibex::Interval& y);
    /** \brief \f$[x]\cap[y](\cdot)\f$ */
    const Tube operator&(const ibex::Interval& x, const Tube& y);
    /** \brief \f$[x](\cdot)\cap y(\cdot)\f$ */
    const Tube operator&(const Tube& x, const Trajectory& y);
    /** \brief \f$x(\cdot)\cap [y](\cdot)\f$ */
    const Tube operator&(const Trajectory& x, const Tube& y);

  /// @}
  /// \name Vector outputs
  /// @{

    /** \brief \f$[\mathbf{x}](\cdot)\f$ */
    const TubeVector operator+(const TubeVector& x);
    /** \brief \f$[\mathbf{x}](\cdot)+[\mathbf{y}](\cdot)\f$ */
    const TubeVector operator+(const TubeVector& x, const TubeVector& y);
    /** \brief \f$[\mathbf{x}](\cdot)+[\mathbf{y}]\f$ */
    const TubeVector operator+(const TubeVector& x, const ibex::IntervalVector& y);
    /** \brief \f$[\mathbf{x}]+[\mathbf{y}](\cdot)\f$ */
    const TubeVector operator+(const ibex::IntervalVector& x, const TubeVector& y);
    /** \brief \f$[\mathbf{x}](\cdot)+\mathbf{y}(\cdot)\f$ */
    const TubeVector operator+(const TubeVector& x, const TrajectoryVector& y);
    /** \brief \f$\mathbf{x}(\cdot)+[\mathbf{y}](\cdot)\f$ */
    const TubeVector operator+(const TrajectoryVector& x, const TubeVector& y);

    /** \brief \f$-[\mathbf{x}](\cdot)\f$ */
    const TubeVector operator-(const TubeVector& x);
    /** \brief \f$[\mathbf{x}](\cdot)-[\mathbf{y}](\cdot)\f$ */
    const TubeVector operator-(const TubeVector& x, const TubeVector& y);
    /** \brief \f$[\mathbf{x}](\cdot)-[\mathbf{y}]\f$ */
    const TubeVector operator-(const TubeVector& x, const ibex::IntervalVector& y);
    /** \brief \f$[\mathbf{x}]-[\mathbf{y}](\cdot)\f$ */
    const TubeVector operator-(const ibex::IntervalVector& x, const TubeVector& y);
    /** \brief \f$[\mathbf{x}](\cdot)-\mathbf{y}(\cdot)\f$ */
    const TubeVector operator-(const TubeVector& x, const TrajectoryVector& y);
    /** \brief \f$\mathbf{x}(\cdot)-[\mathbf{y}](\cdot)\f$ */
    const TubeVector operator-(const TrajectoryVector& x, const TubeVector& y);

    /** \brief \f$[x](\cdot)\cdot[\mathbf{y}](\cdot)\f$ */
    const TubeVector operator*(const Tube& x, const TubeVector& y);
    /** \brief \f$[x]\cdot[\mathbf{y}](\cdot)\f$ */
    const TubeVector operator*(const ibex::Interval& x, const TubeVector& y);
    /** \brief \f$[x](\cdot)\cdot[\mathbf{y}]\f$ */
    const TubeVector operator*(const Tube& x, const ibex::IntervalVector& y);
    /** \brief \f$x(\cdot)\cdot[\mathbf{y}](\cdot)\f$ */
    const TubeVector operator*(const Trajectory& x, const TubeVector& y);

    /** \brief \f$[\mathbf{x}](\cdot)/[y](\cdot)\f$ */
    const TubeVector operator/(const TubeVector& x, const Tube& y);
    /** \brief \f$[\mathbf{x}](\cdot)/[y]\f$ */
    const TubeVector operator/(const TubeVector& x, const ibex::Interval& y);
    /** \brief \f$[\mathbf{x}]/[y](\cdot)\f$ */
    const TubeVector operator/(const ibex::IntervalVector& x, const Tube& y);
    /** \brief \f$[\mathbf{x}](\cdot)/y(\cdot)\f$ */
    const TubeVector operator/(const TubeVector& x, const Trajectory& y);

    /** \brief \f$[\mathbf{x}](\cdot)\sqcup[\mathbf{y}](\cdot)\f$ */
    const TubeVector operator|(const TubeVector& x, const TubeVector& y);
    /** \brief \f$[\mathbf{x}](\cdot)\sqcup[\mathbf{y}]\f$ */
    const TubeVector operator|(const TubeVector& x, const ibex::IntervalVector& y);
    /** \brief \f$[\mathbf{x}]\sqcup[\mathbf{y}](\cdot)\f$ */
    const TubeVector operator|(const ibex::IntervalVector& x, const TubeVector& y);
    /** \brief \f$[\mathbf{x}](\cdot)\sqcup\mathbf{y}(\cdot)\f$ */
    const TubeVector operator|(const TubeVector& x, const TrajectoryVector& y);
    /** \brief \f$\mathbf{x}(\cdot)\sqcup[\mathbf{y}](\cdot)\f$ */
    const TubeVector operator|(const TrajectoryVector& x, const TubeVector& y);

    /** \brief \f$[\mathbf{x}](\cdot)\cap[\mathbf{y}](\cdot)\f$ */
    const TubeVector operator&(const TubeVector& x, const TubeVector& y);
    /** \brief \f$[\mathbf{x}](\cdot)\cap[\mathbf{y}]\f$ */
    const TubeVector operator&(const TubeVector& x, const ibex::IntervalVector& y);
    /** \brief \f$[\mathbf{x}]\cap[\mathbf{y}](\cdot)\f$ */
    const TubeVector operator&(const ibex::IntervalVector& x, const TubeVector& y);
    /** \brief \f$[\mathbf{x}](\cdot)\cap\mathbf{y}(\cdot)\f$ */
    const TubeVector operator&(const TubeVector& x, const TrajectoryVector& y);
    /** \brief \f$\mathbf{x}(\cdot)\cap[\mathbf{y}](\cdot)\f$ */
    const TubeVector operator&(const TrajectoryVector& x, const TubeVector& y);

    /** \brief \f$\mid\mathbf{x}(\cdot)\mid\f$ */
    const TubeVector abs(const TubeVector& x);

  /// @}
}

#endif