/* ============================================================================
 *  tubex-lib - DimensionException class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2015
 * ---------------------------------------------------------------------------- */

#ifndef DimensionException_HEADER
#define DimensionException_HEADER

#include "tubex_Tube.h"
#include "tubex_TubeSlice.h"
#include "tubex_Exception.h"

namespace tubex
{
  /**
   * \brief Dimension error exception.
   *
   * Thrown when two vector objects are not of same dimension.
   */
  class DimensionException : public Exception
  {
    public:

      DimensionException(int dim);
      DimensionException(const ibex::Vector& x1, const ibex::Vector& x2);
      DimensionException(const Trajectory& traj, const ibex::IntervalVector& box);
      DimensionException(const Trajectory& traj, const ibex::Function& f);
      DimensionException(const Trajectory& traj1, const Trajectory& traj2);
      DimensionException(const TubeSlice& x, const ibex::IntervalVector& box);
      DimensionException(const TubeSlice& x, const Trajectory& traj);
      DimensionException(const TubeSlice& x, const ibex::Function& f);
      DimensionException(const TubeSlice& x1, const TubeSlice& x2);
      DimensionException(const Tube& x, const ibex::IntervalVector& box);
      DimensionException(const Tube& x, const Trajectory& traj);
      DimensionException(const Tube& x, const ibex::Function& f);
      DimensionException(const Tube& x1, const Tube& x2);

      static void check(int dim);
      static void check(const ibex::Vector& x1, const ibex::Vector& x2);
      static void check(const Trajectory& traj, const ibex::Function& f);
      static void check(const Trajectory& traj, const ibex::IntervalVector& box);
      static void check(const Trajectory& traj1, const Trajectory& traj2);
      static void check(const TubeSlice& x, const ibex::IntervalVector& box);
      static void check(const TubeSlice& x, const Trajectory& traj);
      static void check(const TubeSlice& x, const ibex::Function& f);
      static void check(const TubeSlice& x1, const TubeSlice& x2);
      static void check(const Tube& x, const ibex::IntervalVector& box);
      static void check(const Tube& x, const Trajectory& traj);
      static void check(const Tube& x, const ibex::Function& f);
      static void check(const Tube& x1, const Tube& x2);
  };
}

#endif