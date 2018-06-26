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

#include "tubex_TubeVector.h"
#include "tubex_TubeSlice.h"
#include "tubex_Exception.h"
#include "tubex_VibesFigure_Tube.h"

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
      DimensionException(const TrajectoryVector& traj, const ibex::IntervalVector& box);
      DimensionException(const TrajectoryVector& traj, const ibex::Function& f);
      DimensionException(const TrajectoryVector& traj1, const TrajectoryVector& traj2);
      DimensionException(const TubeSlice& x, int i);
      DimensionException(const TubeSlice& x, const ibex::IntervalVector& box);
      DimensionException(const TubeSlice& x, const TrajectoryVector& traj);
      DimensionException(const TubeSlice& x, const ibex::Function& f);
      DimensionException(const TubeSlice& x1, const TubeSlice& x2);
      DimensionException(const TubeVector& x, const ibex::IntervalVector& box);
      DimensionException(const TubeVector& x, const TrajectoryVector& traj);
      DimensionException(const TubeVector& x, const ibex::Function& f);
      DimensionException(const TubeVector& x1, const TubeVector& x2);
      DimensionException(const VibesFigure_Tube& fig, const TubeVector& x);
      DimensionException(const VibesFigure_Tube& fig, const TrajectoryVector& traj);

      static void check(int dim);
      static void checkScalar(int dim);
      static void check(const ibex::Vector& x1, const ibex::Vector& x2);
      static void check(const TrajectoryVector& traj, const ibex::Function& f);
      static void check(const TrajectoryVector& traj, const ibex::IntervalVector& box);
      static void check(const TrajectoryVector& traj1, const TrajectoryVector& traj2);
      static void check(const TubeSlice& x, int i);
      static void check(const TubeSlice& x, const ibex::IntervalVector& box);
      static void check(const TubeSlice& x, const TrajectoryVector& traj);
      static void check(const TubeSlice& x, const ibex::Function& f);
      static void check(const TubeSlice& x1, const TubeSlice& x2);
      static void check(const TubeVector& x, const ibex::IntervalVector& box);
      static void check(const TubeVector& x, const TrajectoryVector& traj);
      static void check(const TubeVector& x, const ibex::Function& f);
      static void check(const TubeVector& x1, const TubeVector& x2);
      static void check(const VibesFigure_Tube& fig, const TubeVector& x);
      static void check(const VibesFigure_Tube& fig, const TrajectoryVector& traj);
  };
}

#endif