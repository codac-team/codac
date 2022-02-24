/** 
 *  \file
 *  Arithmetic operations on tubes
 * ----------------------------------------------------------------------------
 *  \date       2015
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).

      * \param x
      * \return Tube output
      */

#ifndef __CODAC_TUBE_ARITHMETIC_H__
#define __CODAC_TUBE_ARITHMETIC_H__

#include "codac_Interval.h"
#include "codac_IntervalVector.h"
#include "codac_Tube.h"
#include "codac_TubeVector.h"

namespace codac
{
  /// \name Scalar outputs
  /// @{

    /** \brief \f$\cos([x](\cdot))\f$
      * \param x
      * \return Tube output
      */
    const Tube cos(const Tube& x);

    /** \brief \f$\sin([x](\cdot))\f$
      * \param x
      * \return Tube output
      */
    const Tube sin(const Tube& x);

    /** \brief \f$\mid[x](\cdot)\mid\f$
      * \param x
      * \return Tube output
      */
    const Tube abs(const Tube& x);

    /** \brief \f$[x]^2(\cdot)\f$
      * \param x
      * \return Tube output
      */
    const Tube sqr(const Tube& x);

    /** \brief \f$\sqrt{[x](\cdot)}\f$
      * \param x
      * \return Tube output
      */
    const Tube sqrt(const Tube& x);

    /** \brief \f$\exp([x](\cdot))\f$
      * \param x
      * \return Tube output
      */
    const Tube exp(const Tube& x);

    /** \brief \f$\log([x](\cdot))\f$
      * \param x
      * \return Tube output
      */
    const Tube log(const Tube& x);

    /** \brief \f$\tan([x](\cdot))\f$
      * \param x
      * \return Tube output
      */
    const Tube tan(const Tube& x);

    /** \brief \f$\arccos([x](\cdot))\f$
      * \param x
      * \return Tube output
      */
    const Tube acos(const Tube& x);

    /** \brief \f$\arcsin([x](\cdot))\f$
      * \param x
      * \return Tube output
      */
    const Tube asin(const Tube& x);

    /** \brief \f$\arctan([x](\cdot))\f$
      * \param x
      * \return Tube output
      */
    const Tube atan(const Tube& x);

    /** \brief \f$\cosh([x](\cdot))\f$
      * \param x
      * \return Tube output
      */
    const Tube cosh(const Tube& x);

    /** \brief \f$\sinh([x](\cdot))\f$
      * \param x
      * \return Tube output
      */
    const Tube sinh(const Tube& x);

    /** \brief \f$\tanh([x](\cdot))\f$
      * \param x
      * \return Tube output
      */
    const Tube tanh(const Tube& x);

    /** \brief \f$\mathrm{arccosh}([x](\cdot))\f$
      * \param x
      * \return Tube output
      */
    const Tube acosh(const Tube& x);

    /** \brief \f$\mathrm{arcsinh}([x](\cdot))\f$
      * \param x
      * \return Tube output
      */
    const Tube asinh(const Tube& x);

    /** \brief \f$\mathrm{arctanh}([x](\cdot))\f$
      * \param x
      * \return Tube output
      */
    const Tube atanh(const Tube& x);

    /** \brief \f$\mathrm{arctan2}([y](\cdot),[x](\cdot))\f$
      * \param y
      * \param x
      * \return Tube output
      */
    const Tube atan2(const Tube& y, const Tube& x);

    /** \brief \f$\mathrm{arctan2}([y](\cdot),[x])\f$
      * \param y
      * \param x
      * \return Tube output
      */
    const Tube atan2(const Tube& y, const Interval& x);

    /** \brief \f$\mathrm{arctan2}([y],[x](\cdot))\f$
      * \param y
      * \param x
      * \return Tube output
      */
    const Tube atan2(const Interval& y, const Tube& x);

    /** \brief \f$[x]^p(\cdot)\f$
      * \param x
      * \param p
      * \return Tube output
      */
    const Tube pow(const Tube& x, int p);

    /** \brief \f$[x]^p(\cdot)\f$
      * \param x
      * \param p
      * \return Tube output
      */
    const Tube pow(const Tube& x, double p);

    /** \brief \f$[x]^{[p]}(\cdot)\f$
      * \param x
      * \param p
      * \return Tube output
      */
    const Tube pow(const Tube& x, const Interval& p);

    /** \brief \f$\sqrt[p]{[x](\cdot)}\f$
      * \param x
      * \param p
      * \return Tube output
      */
    const Tube root(const Tube& x, int p);

    // todo: atan2, pow with Trajectory as parameter

    /** \brief \f$\min([y](\cdot),[x](\cdot))\f$
      * \param y
      * \param x
      * \return Tube output
      */
    const Tube min(const Tube& y, const Tube& x);

    /** \brief \f$\min([y](\cdot),[x])\f$
      * \param y
      * \param x
      * \return Tube output
      */
    const Tube min(const Tube& y, const Interval& x);

    /** \brief \f$\min([y],[x](\cdot))\f$
      * \param y
      * \param x
      * \return Tube output
      */
    const Tube min(const Interval& y, const Tube& x);

    /** \brief \f$\max([y](\cdot),[x](\cdot))\f$
      * \param y
      * \param x
      * \return Tube output
      */
    const Tube max(const Tube& y, const Tube& x);

    /** \brief \f$\max([y](\cdot),[x])\f$
      * \param y
      * \param x
      * \return Tube output
      */
    const Tube max(const Tube& y, const Interval& x);

    /** \brief \f$\max([y],[x](\cdot))\f$
      * \param y
      * \param x
      * \return Tube output
      */
    const Tube max(const Interval& y, const Tube& x);

    /** \brief \f$[x](\cdot)\f$
      * \param x
      * \return Tube output
      */
    const Tube operator+(const Tube& x);

    /** \brief \f$[x](\cdot)+[y](\cdot)\f$
      * \param x
      * \param y
      * \return Tube output
      */
    const Tube operator+(const Tube& x, const Tube& y);

    /** \brief \f$[x](\cdot)+[y]\f$
      * \param x
      * \param y
      * \return Tube output
      */
    const Tube operator+(const Tube& x, const Interval& y);

    /** \brief \f$[x]+[y](\cdot)\f$
      * \param x
      * \param y
      * \return Tube output
      */
    const Tube operator+(const Interval& x, const Tube& y);

    /** \brief \f$[x](\cdot)+y(\cdot)\f$
      * \param x
      * \param y
      * \return Tube output
      */
    const Tube operator+(const Tube& x, const Trajectory& y);

    /** \brief \f$x(\cdot)+[y](\cdot)\f$
      * \param x
      * \param y
      * \return Tube output
      */
    const Tube operator+(const Trajectory& x, const Tube& y);

    /** \brief \f$-[x](\cdot)\f$
      * \param x
      * \return Tube output
      */
    const Tube operator-(const Tube& x);

    /** \brief \f$[x](\cdot)-[y](\cdot)\f$
      * \param x
      * \param y
      * \return Tube output
      */
    const Tube operator-(const Tube& x, const Tube& y);

    /** \brief \f$[x](\cdot)-[y]\f$
      * \param x
      * \param y
      * \return Tube output
      */
    const Tube operator-(const Tube& x, const Interval& y);

    /** \brief \f$[x]-[y](\cdot)\f$
      * \param x
      * \param y
      * \return Tube output
      */
    const Tube operator-(const Interval& x, const Tube& y);

    /** \brief \f$[x](\cdot)-y(\cdot)\f$
      * \param x
      * \param y
      * \return Tube output
      */
    const Tube operator-(const Tube& x, const Trajectory& y);

    /** \brief \f$x(\cdot)-[y](\cdot)\f$
      * \param x
      * \param y
      * \return Tube output
      */
    const Tube operator-(const Trajectory& x, const Tube& y);

    /** \brief \f$[x](\cdot)\cdot[y](\cdot)\f$
      * \param x
      * \param y
      * \return Tube output
      */
    const Tube operator*(const Tube& x, const Tube& y);

    /** \brief \f$[x](\cdot)\cdot[y]\f$
      * \param x
      * \param y
      * \return Tube output
      */
    const Tube operator*(const Tube& x, const Interval& y);

    /** \brief \f$[x]\cdot[y](\cdot)\f$
      * \param x
      * \param y
      * \return Tube output
      */
    const Tube operator*(const Interval& x, const Tube& y);

    /** \brief \f$[x](\cdot)\cdot y(\cdot)\f$
      * \param x
      * \param y
      * \return Tube output
      */
    const Tube operator*(const Tube& x, const Trajectory& y);

    /** \brief \f$x(\cdot)\cdot[y](\cdot)\f$
      * \param x
      * \param y
      * \return Tube output
      */
    const Tube operator*(const Trajectory& x, const Tube& y);

    /** \brief \f$[x](\cdot)/[y](\cdot)\f$
      * \param x
      * \param y
      * \return Tube output
      */
    const Tube operator/(const Tube& x, const Tube& y);

    /** \brief \f$[x](\cdot)/[y]\f$
      * \param x
      * \param y
      * \return Tube output
      */
    const Tube operator/(const Tube& x, const Interval& y);

    /** \brief \f$[x]/[y](\cdot)\f$
      * \param x
      * \param y
      * \return Tube output
      */
    const Tube operator/(const Interval& x, const Tube& y);

    /** \brief \f$[x](\cdot)/y(\cdot)\f$
      * \param x
      * \param y
      * \return Tube output
      */
    const Tube operator/(const Tube& x, const Trajectory& y);

    /** \brief \f$x(\cdot)/[y](\cdot)\f$
      * \param x
      * \param y
      * \return Tube output
      */
    const Tube operator/(const Trajectory& x, const Tube& y);

    /** \brief \f$[x](\cdot)\sqcup[y](\cdot)\f$
      * \param x
      * \param y
      * \return Tube output
      */
    const Tube operator|(const Tube& x, const Tube& y);

    /** \brief \f$[x](\cdot)\sqcup[y]\f$
      * \param x
      * \param y
      * \return Tube output
      */
    const Tube operator|(const Tube& x, const Interval& y);

    /** \brief \f$[x]\sqcup[y](\cdot)\f$
      * \param x
      * \param y
      * \return Tube output
      */
    const Tube operator|(const Interval& x, const Tube& y);

    /** \brief \f$[x](\cdot)\sqcup y(\cdot)\f$
      * \param x
      * \param y
      * \return Tube output
      */
    const Tube operator|(const Tube& x, const Trajectory& y);

    /** \brief \f$x(\cdot)\sqcup [y](\cdot)\f$
      * \param x
      * \param y
      * \return Tube output
      */
    const Tube operator|(const Trajectory& x, const Tube& y);

    /** \brief \f$[x](\cdot)\cap[y](\cdot)\f$
      * \param x
      * \param y
      * \return Tube output
      */
    const Tube operator&(const Tube& x, const Tube& y);

    /** \brief \f$[x](\cdot)\cap[y]\f$
      * \param x
      * \param y
      * \return Tube output
      */
    const Tube operator&(const Tube& x, const Interval& y);

    /** \brief \f$[x]\cap[y](\cdot)\f$
      * \param x
      * \param y
      * \return Tube output
      */
    const Tube operator&(const Interval& x, const Tube& y);

    /** \brief \f$[x](\cdot)\cap y(\cdot)\f$
      * \param x
      * \param y
      * \return Tube output
      */
    const Tube operator&(const Tube& x, const Trajectory& y);

    /** \brief \f$x(\cdot)\cap [y](\cdot)\f$
      * \param x
      * \param y
      * \return Tube output
      */
    const Tube operator&(const Trajectory& x, const Tube& y);

  /// @}
  /// \name Vector outputs
  /// @{

    /** \brief \f$[\mathbf{x}](\cdot)\f$
      * \param x
      * \return TubeVector output
      */
    const TubeVector operator+(const TubeVector& x);

    /** \brief \f$[\mathbf{x}](\cdot)+[\mathbf{y}](\cdot)\f$
      * \param x
      * \param y
      * \return TubeVector output
      */
    const TubeVector operator+(const TubeVector& x, const TubeVector& y);

    /** \brief \f$[\mathbf{x}](\cdot)+[\mathbf{y}]\f$
      * \param x
      * \param y
      * \return TubeVector output
      */
    const TubeVector operator+(const TubeVector& x, const IntervalVector& y);

    /** \brief \f$[\mathbf{x}]+[\mathbf{y}](\cdot)\f$
      * \param x
      * \param y
      * \return TubeVector output
      */
    const TubeVector operator+(const IntervalVector& x, const TubeVector& y);

    /** \brief \f$[\mathbf{x}](\cdot)+\mathbf{y}(\cdot)\f$
      * \param x
      * \param y
      * \return TubeVector output
      */
    const TubeVector operator+(const TubeVector& x, const TrajectoryVector& y);

    /** \brief \f$\mathbf{x}(\cdot)+[\mathbf{y}](\cdot)\f$
      * \param x
      * \param y
      * \return TubeVector output
      */
    const TubeVector operator+(const TrajectoryVector& x, const TubeVector& y);

    /** \brief \f$-[\mathbf{x}](\cdot)\f$
      * \param x
      * \return TubeVector output
      */
    const TubeVector operator-(const TubeVector& x);

    /** \brief \f$[\mathbf{x}](\cdot)-[\mathbf{y}](\cdot)\f$
      * \param x
      * \param y
      * \return TubeVector output
      */
    const TubeVector operator-(const TubeVector& x, const TubeVector& y);

    /** \brief \f$[\mathbf{x}](\cdot)-[\mathbf{y}]\f$
      * \param x
      * \param y
      * \return TubeVector output
      */
    const TubeVector operator-(const TubeVector& x, const IntervalVector& y);

    /** \brief \f$[\mathbf{x}]-[\mathbf{y}](\cdot)\f$
      * \param x
      * \param y
      * \return TubeVector output
      */
    const TubeVector operator-(const IntervalVector& x, const TubeVector& y);

    /** \brief \f$[\mathbf{x}](\cdot)-\mathbf{y}(\cdot)\f$
      * \param x
      * \param y
      * \return TubeVector output
      */
    const TubeVector operator-(const TubeVector& x, const TrajectoryVector& y);

    /** \brief \f$\mathbf{x}(\cdot)-[\mathbf{y}](\cdot)\f$
      * \param x
      * \param y
      * \return TubeVector output
      */
    const TubeVector operator-(const TrajectoryVector& x, const TubeVector& y);

    /** \brief \f$[x](\cdot)\cdot[\mathbf{y}](\cdot)\f$
      * \param x
      * \param y
      * \return TubeVector output
      */
    const TubeVector operator*(const Tube& x, const TubeVector& y);

    /** \brief \f$[x]\cdot[\mathbf{y}](\cdot)\f$
      * \param x
      * \param y
      * \return TubeVector output
      */
    const TubeVector operator*(const Interval& x, const TubeVector& y);

    /** \brief \f$[x](\cdot)\cdot[\mathbf{y}]\f$
      * \param x
      * \param y
      * \return TubeVector output
      */
    const TubeVector operator*(const Tube& x, const IntervalVector& y);

    /** \brief \f$x(\cdot)\cdot[\mathbf{y}](\cdot)\f$
      * \param x
      * \param y
      * \return TubeVector output
      */
    const TubeVector operator*(const Trajectory& x, const TubeVector& y);

    /** \brief \f$[\mathbf{x}](\cdot)/[y](\cdot)\f$
      * \param x
      * \param y
      * \return TubeVector output
      */
    const TubeVector operator/(const TubeVector& x, const Tube& y);

    /** \brief \f$[\mathbf{x}](\cdot)/[y]\f$
      * \param x
      * \param y
      * \return TubeVector output
      */
    const TubeVector operator/(const TubeVector& x, const Interval& y);

    /** \brief \f$[\mathbf{x}]/[y](\cdot)\f$
      * \param x
      * \param y
      * \return TubeVector output
      */
    const TubeVector operator/(const IntervalVector& x, const Tube& y);

    /** \brief \f$[\mathbf{x}](\cdot)/y(\cdot)\f$
      * \param x
      * \param y
      * \return TubeVector output
      */
    const TubeVector operator/(const TubeVector& x, const Trajectory& y);

    /** \brief \f$[\mathbf{x}](\cdot)\sqcup[\mathbf{y}](\cdot)\f$
      * \param x
      * \param y
      * \return TubeVector output
      */
    const TubeVector operator|(const TubeVector& x, const TubeVector& y);

    /** \brief \f$[\mathbf{x}](\cdot)\sqcup[\mathbf{y}]\f$
      * \param x
      * \param y
      * \return TubeVector output
      */
    const TubeVector operator|(const TubeVector& x, const IntervalVector& y);

    /** \brief \f$[\mathbf{x}]\sqcup[\mathbf{y}](\cdot)\f$
      * \param x
      * \param y
      * \return TubeVector output
      */
    const TubeVector operator|(const IntervalVector& x, const TubeVector& y);

    /** \brief \f$[\mathbf{x}](\cdot)\sqcup\mathbf{y}(\cdot)\f$
      * \param x
      * \param y
      * \return TubeVector output
      */
    const TubeVector operator|(const TubeVector& x, const TrajectoryVector& y);

    /** \brief \f$\mathbf{x}(\cdot)\sqcup[\mathbf{y}](\cdot)\f$
      * \param x
      * \param y
      * \return TubeVector output
      */
    const TubeVector operator|(const TrajectoryVector& x, const TubeVector& y);

    /** \brief \f$[\mathbf{x}](\cdot)\cap[\mathbf{y}](\cdot)\f$
      * \param x
      * \param y
      * \return TubeVector output
      */
    const TubeVector operator&(const TubeVector& x, const TubeVector& y);

    /** \brief \f$[\mathbf{x}](\cdot)\cap[\mathbf{y}]\f$
      * \param x
      * \param y
      * \return TubeVector output
      */
    const TubeVector operator&(const TubeVector& x, const IntervalVector& y);

    /** \brief \f$[\mathbf{x}]\cap[\mathbf{y}](\cdot)\f$
      * \param x
      * \param y
      * \return TubeVector output
      */
    const TubeVector operator&(const IntervalVector& x, const TubeVector& y);

    /** \brief \f$[\mathbf{x}](\cdot)\cap\mathbf{y}(\cdot)\f$
      * \param x
      * \param y
      * \return TubeVector output
      */
    const TubeVector operator&(const TubeVector& x, const TrajectoryVector& y);

    /** \brief \f$\mathbf{x}(\cdot)\cap[\mathbf{y}](\cdot)\f$
      * \param x
      * \param y
      * \return TubeVector output
      */
    const TubeVector operator&(const TrajectoryVector& x, const TubeVector& y);

    /** \brief \f$\mid\mathbf{x}(\cdot)\mid\f$
      * \param x
      * \return TubeVector output
      */
    const TubeVector abs(const TubeVector& x);

  /// @}
}

#endif