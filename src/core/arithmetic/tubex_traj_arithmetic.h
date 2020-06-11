/** 
 *  \file
 *  Arithmetic operations on trajectories
 * ----------------------------------------------------------------------------
 *  \date       2019
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_TRAJ_ARITHMETIC_H__
#define __TUBEX_TRAJ_ARITHMETIC_H__

#include "tubex_Trajectory.h"
#include "tubex_TrajectoryVector.h"

namespace tubex
{
  class Trajectory;
  class TrajectoryVector;

  /// \name Scalar outputs
  /// @{

    /** \brief \f$\cos(x(\cdot))\f$ */
    const Trajectory cos(const Trajectory& x);
    /** \brief \f$\sin(x(\cdot))\f$ */
    const Trajectory sin(const Trajectory& x);
    /** \brief \f$\mid x(\cdot)\mid\f$ */
    const Trajectory abs(const Trajectory& x);
    /** \brief \f$x^2(\cdot)\f$ */
    const Trajectory sqr(const Trajectory& x);
    /** \brief \f$\sqrt{x(\cdot)}\f$ */
    const Trajectory sqrt(const Trajectory& x);
    /** \brief \f$\exp(x(\cdot))\f$ */
    const Trajectory exp(const Trajectory& x);
    /** \brief \f$\log(x(\cdot))\f$ */
    const Trajectory log(const Trajectory& x);
    /** \brief \f$\tan(x(\cdot))\f$ */
    const Trajectory tan(const Trajectory& x);
    /** \brief \f$\arccos(x(\cdot))\f$ */
    const Trajectory acos(const Trajectory& x);
    /** \brief \f$\arcsin(x(\cdot))\f$ */
    const Trajectory asin(const Trajectory& x);
    /** \brief \f$\arctan(x(\cdot))\f$ */
    const Trajectory atan(const Trajectory& x);
    /** \brief \f$\cosh(x(\cdot))\f$ */
    const Trajectory cosh(const Trajectory& x);
    /** \brief \f$\sinh(x(\cdot))\f$ */
    const Trajectory sinh(const Trajectory& x);
    /** \brief \f$\tanh(x(\cdot))\f$ */
    const Trajectory tanh(const Trajectory& x);
    /** \brief \f$\mathrm{arccosh}(x(\cdot))\f$ */
    const Trajectory acosh(const Trajectory& x);
    /** \brief \f$\mathrm{arcsinh}(x(\cdot))\f$ */
    const Trajectory asinh(const Trajectory& x);
    /** \brief \f$\mathrm{arctanh}(x(\cdot))\f$ */
    const Trajectory atanh(const Trajectory& x);

    /** \brief \f$\mathrm{arctan2}(y(\cdot),x(\cdot))\f$ */
    const Trajectory atan2(const Trajectory& y, const Trajectory& x);
    /** \brief \f$\mathrm{arctan2}(y(\cdot),x)\f$ */
    const Trajectory atan2(const Trajectory& y, double x);
    /** \brief \f$\mathrm{arctan2}(y, x(\cdot))\f$ */
    const Trajectory atan2(double y, const Trajectory& x);

    /** \brief \f$x^p(\cdot)\f$ */
    const Trajectory pow(const Trajectory& x, int p);
    /** \brief \f$x^p(\cdot)\f$ */
    const Trajectory pow(const Trajectory& x, double p);
    /** \brief \f$\sqrt[p]{x(\cdot)}\f$ */
    const Trajectory root(const Trajectory& x, int p);

    /** \brief \f$x(\cdot)\f$ */
    const Trajectory operator+(const Trajectory& x);
    /** \brief \f$x(\cdot)+y(\cdot)\f$ */
    const Trajectory operator+(const Trajectory& x, const Trajectory& y);
    /** \brief \f$x(\cdot)+y\f$ */
    const Trajectory operator+(const Trajectory& x, double y);
    /** \brief \f$x+y(\cdot)\f$ */
    const Trajectory operator+(double x, const Trajectory& y);

    /** \brief \f$-x(\cdot)\f$ */
    const Trajectory operator-(const Trajectory& x);
    /** \brief \f$x(\cdot)-y(\cdot)\f$ */
    const Trajectory operator-(const Trajectory& x, const Trajectory& y);
    /** \brief \f$x(\cdot)-y\f$ */
    const Trajectory operator-(const Trajectory& x, double y);
    /** \brief \f$x-y(\cdot)\f$ */
    const Trajectory operator-(double x, const Trajectory& y);

    /** \brief \f$x(\cdot)\cdot y(\cdot)\f$ */
    const Trajectory operator*(const Trajectory& x, const Trajectory& y);
    /** \brief \f$x(\cdot)\cdot y\f$ */
    const Trajectory operator*(const Trajectory& x, double y);
    /** \brief \f$x\cdot y(\cdot)\f$ */
    const Trajectory operator*(double x, const Trajectory& y);

    /** \brief \f$x(\cdot)/y(\cdot)\f$ */
    const Trajectory operator/(const Trajectory& x, const Trajectory& y);
    /** \brief \f$x(\cdot)/y\f$ */
    const Trajectory operator/(const Trajectory& x, double y);
    /** \brief \f$x/y(\cdot)\f$ */
    const Trajectory operator/(double x, const Trajectory& y);

  /// @}
  /// \name Vector outputs
  /// @{

    /** \brief \f$\mathbf{x}(\cdot)\f$ */
    const TrajectoryVector operator+(const TrajectoryVector& x);
    /** \brief \f$\mathbf{x}(\cdot)+\mathbf{y}(\cdot)\f$ */
    const TrajectoryVector operator+(const TrajectoryVector& x, const TrajectoryVector& y);
    /** \brief \f$\mathbf{x}(\cdot)+\mathbf{y}\f$ */
    const TrajectoryVector operator+(const TrajectoryVector& x, const ibex::Vector& y);
    /** \brief \f$\mathbf{x}+\mathbf{y}(\cdot)\f$ */
    const TrajectoryVector operator+(const ibex::Vector& x, const TrajectoryVector& y);

    /** \brief \f$-\mathbf{x}(\cdot)\f$ */
    const TrajectoryVector operator-(const TrajectoryVector& x);
    /** \brief \f$\mathbf{x}(\cdot)-\mathbf{y}(\cdot)\f$ */
    const TrajectoryVector operator-(const TrajectoryVector& x, const TrajectoryVector& y);
    /** \brief \f$\mathbf{x}(\cdot)-\mathbf{y}\f$ */
    const TrajectoryVector operator-(const TrajectoryVector& x, const ibex::Vector& y);
    /** \brief \f$\mathbf{x}-\mathbf{y}(\cdot)\f$ */
    const TrajectoryVector operator-(const ibex::Vector& x, const TrajectoryVector& y);

    /** \brief \f$x\cdot\mathbf{y}(\cdot)\f$ */
    const TrajectoryVector operator*(double x, const TrajectoryVector& y);
    /** \brief \f$x(\cdot)\cdot\mathbf{y}(\cdot)\f$ */
    const TrajectoryVector operator*(const Trajectory& x, const TrajectoryVector& y);
    /** \brief \f$x(\cdot)\cdot\mathbf{y}\f$ */
    const TrajectoryVector operator*(const Trajectory& x, const ibex::Vector& y);
    /** \brief \f$x(\cdot)\cdot\mathbf{y}\f$ */
    const TrajectoryVector operator*(const ibex::Matrix& x, const TrajectoryVector& y);

    /** \brief \f$\mathbf{x}(\cdot)/y\f$ */
    const TrajectoryVector operator/(const TrajectoryVector& x, double y);
    /** \brief \f$\mathbf{x}(\cdot)/y(\cdot)\f$ */
    const TrajectoryVector operator/(const TrajectoryVector& x, const Trajectory& y);
    /** \brief \f$\mathbf{x}/y(\cdot)\f$ */
    const TrajectoryVector operator/(const ibex::Vector& x, const Trajectory& y);

    /** \brief \f$\mathbf{x}(\cdot)\times\mathbf{y}\f$ (or \f$\mathbf{x}(\cdot)\wedge\mathbf{y}\f$ in physics) */
    const TrajectoryVector vecto_product(const TrajectoryVector& x, const ibex::Vector& y);
    /** \brief \f$\mathbf{x}\times\mathbf{y}(\cdot)\f$ (or \f$\mathbf{x}\wedge\mathbf{y}(\cdot)\f$ in physics) */
    const TrajectoryVector vecto_product(const ibex::Vector& x, const TrajectoryVector& y);

    /** \brief \f$\mid\mathbf{x}(\cdot)\mid\f$ */
    const TrajectoryVector abs(const TrajectoryVector& x);

  /// @}
}

#endif